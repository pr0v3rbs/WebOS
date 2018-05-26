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

/** @file pe_reg_h15.h
 *
 *  driver header for picture enhance register read, write functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.02
 *	@see		
 */

#ifndef	_PE_REG_H15_H_
#define	_PE_REG_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "be_kapi.h"
#include "pe_reg_def_h15.h"
#include "pe_cco_reg_h15.h"
#include "pe_cvi_reg_h15.h"
#include "pe_dpe_reg_h15.h"
#include "pe_msc_reg_h15.h"
#include "pe_nd_reg_h15.h"
#include "pe_shp_reg_h15.h"
#include "pe_sr_reg_h15.h"
#include "pe_ss_reg_h15.h"
#include "pe_vcp_reg_h15.h"
#include "pe_ddr_reg_h15.h"

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
} PE_BFW_INFO_REG_H15A_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_REG_H15_WD(UINT32 addr, UINT32 value);
int PE_REG_H15_WrAllocatedAddr(UINT32 addr, UINT32 value);
UINT32 PE_REG_H15_RD(UINT32 addr);
int PE_REG_H15_WrReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_H15_RdReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_H15_InitPhyToVirt(void);
int PE_REG_H15_FreePhyToVirt(void);
int PE_REG_H15_SetTraceTable(const char* fn, UINT32 line, char *tbl, UINT32 ofst);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PE_CE_PE1_REG_TYPE_H15A0_T   gPE_CE_PE1_H15A0;
extern PE_CO_PE1_REG_TYPE_H15A0_T   gPE_CO_PE1_H15A0;
extern PE_HDMI_A_REG_TYPE_H15A0_T   gPE_HDMI_A_H15A0;
extern PE_HDMI_B_REG_TYPE_H15A0_T   gPE_HDMI_B_H15A0;
extern PE_HDMI_C_REG_TYPE_H15A0_T   gPE_HDMI_C_H15A0;
extern PE_HDMI_D_REG_TYPE_H15A0_T   gPE_HDMI_D_H15A0;
extern PE_CVI_A_REG_TYPE_H15A0_T    gPE_CVI_A_H15A0;
extern PE_CVI_B_REG_TYPE_H15A0_T    gPE_CVI_B_H15A0;
extern PE_CVI_C_REG_TYPE_H15A0_T    gPE_CVI_C_H15A0;
extern PE_CVI_D_REG_TYPE_H15A0_T    gPE_CVI_D_H15A0;
extern PE_LED_RP_REG_TYPE_H15A0_T   gPE_LED_RP_H15A0;
extern PE_MSC_Y_REG_TYPE_H15A0_T    gPE_MSC_Y_H15A0;
extern PE_MSC_C_REG_TYPE_H15A0_T    gPE_MSC_C_H15A0;
extern PE_ND_ND1_REG_TYPE_H15A0_T   gPE_ND_ND1_H15A0;
extern PE_ND_ND2_REG_TYPE_H15A0_T   gPE_ND_ND2_H15A0;
extern PE_ND_NDS_REG_TYPE_H15A0_T   gPE_ND_NDS_H15A0;
extern PE_ND_DNR1_REG_TYPE_H15A0_T  gPE_DNR1_H15A0;
extern PE_ND_DNR2_REG_TYPE_H15A0_T  gPE_DNR2_H15A0;
extern PE_SHP_TOP_REG_TYPE_H15A0_T  gPE_SHP_TOP_H15A0;
extern PE_SHPL_REG_TYPE_H15A0_T     gPE_SHPL_H15A0;
extern PE_SHPR_REG_TYPE_H15A0_T     gPE_SHPR_H15A0;
extern PE_SR_F1_REG_TYPE_H15A0_T    gPE_SR_F1_H15A0;
extern PE_SR_S1_REG_TYPE_H15A0_T    gPE_SR_S1_H15A0;
extern PE_SR_SHP_REG_TYPE_H15A0_T   gPE_SR_SHP_H15A0;
extern PE_SR_GSC_REG_TYPE_H15A0_T   gPE_SR_GSC_H15A0;
extern PE_SSC_REG_TYPE_H15A0_T      gPE_SSC_H15A0;
extern PE_VCP_DE_REG_TYPE_H15A0_T   gPE_VCP_DE_H15A0;
extern PE_VCP_DISP_REG_TYPE_H15A0_T gPE_VCP_DISP_H15A0;
extern PE_VCP_SO_REG_TYPE_H15A0_T   gPE_VCP_SO_H15A0;
extern PE_VCP_HT_REG_TYPE_H15A0_T   gPE_VCP_HT_H15A0;
extern PE_DDR_DB_REG_TYPE_H15_T     gPE_DDR_DB_H15;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_REG_H15_H_ */
