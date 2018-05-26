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

/** @file pe_reg_def_h15.h
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2011.02.17
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */

#ifndef _PE_REG_DEF_H15_H_
#define _PE_REG_DEF_H15_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
/* define for using h15a wcp(ov) register
* should be undef, because of h15a ov removed */
/* 20140526, sj.youm, undef for bringup */
#undef PE_HW_H15_USE_H15A_WCP_REG

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
/* H15A_DE_BASE */
#define PE_H15A_DE_BASE				(0xC8000000)

#define PE_H15A_HDMI_A_OFST			(0x000100)
#define PE_H15A_HDMI_B_OFST			(0x000140)
#define PE_H15A_HDMI_C_OFST			(0x000180)
#define PE_H15A_HDMI_D_OFST			(0x0001C0)
#define PE_H15A_CVI_A_OFST			(0x000200)
#define PE_H15A_CVI_B_OFST			(0x000300)
#define PE_H15A_CVI_C_OFST			(0x000400)
#define PE_H15A_CVI_D_OFST			(0x000500)

#define PE_H15A_ND1_OFST			(0x004100)
#define PE_H15A_ND2_OFST			(0x005100)
#define PE_H15A_NDS_OFST			(0x006100)
#define PE_H15A_DNR1_OFST			(0x007100)
#define PE_H15A_DNR2_OFST			(0x007300)

#define PE_H15A_SC_V1_OFST			(0x000044)
#define PE_H15A_SC_V2_OFST			(0x000060)
#define PE_H15A_SC_H1_OFST			(0x000074)
#define PE_H15A_SC_H2_OFST			(0x000090)

#define PE_H15A_MSC_Y_OFST			(0x007500)
#define PE_H15A_MSC_Y_M1_OFST		(0x007500)
#define PE_H15A_MSC_Y_M2_OFST		(0x0075A4)
#define PE_H15A_MSC_C_OFST			(0x007700)
#define PE_H15A_MSC_C_M1_OFST		(0x007700)
#define PE_H15A_MSC_C_M2_OFST		(0x0077A4)
#define PE_H15A_SSC_OFST			(0x006500)

#define PE_H15A_VCP_DE_OFST			(0x00C000)
#define PE_H15A_VCP_DISP_OFST		(0x00C400)
#define PE_H15A_VCP_SO_OFST			(0x00CC00)
#define PE_H15A_VCP_HT_OFST			(0x00CE00)

#define PE_H15A_SR_SHP_OFST			(0x00A000)
#define PE_H15A_SR_F1_OFST			(0x008000)
#define PE_H15A_SR_S1_OFST			(0x009000)
#define PE_H15A_SR_GSC_OFST			(0x00A800)

#define PE_H15A_SHP_TOP_OFST		(0x00D000)
#define PE_H15A_P1_SHP_L_OFST		(0x00D400)
#define PE_H15A_P1_SHP_R_OFST		(0x00DA00)

#define PE_H15A_P1_CCO_E_OFST		(0x025000)
#define PE_H15A_P1_CCO_O_OFST		(0x025800)

#define PE_H15A_LED_OFST			(0x020000)
#define PE_H15A_LED_RP_OFST			(0x020540)

#define PE_H15A_REG_WRITE_BASE		(0x0)
#define PE_H15A_ND_ND_1_2_DIFF		(PE_H15A_ND2_OFST - PE_H15A_ND1_OFST)
#define PE_H15A_ND_ND_1_S_DIFF		(PE_H15A_NDS_OFST - PE_H15A_ND1_OFST)
#define PE_H15A_ND_DNR_12_DIFF		(PE_H15A_DNR2_OFST - PE_H15A_DNR1_OFST)
#define PE_H15A_P1_SHP_LR_DIFF		(PE_H15A_P1_SHP_R_OFST - PE_H15A_P1_SHP_L_OFST)
#define PE_H15A_P1_CCO_EO_DIFF		(PE_H15A_P1_CCO_O_OFST - PE_H15A_P1_CCO_E_OFST)
#define PE_H15A_MSC_Y_M12_DIFF		(PE_H15A_MSC_Y_M2_OFST - PE_H15A_MSC_Y_M1_OFST)
#define PE_H15A_MSC_C_M12_DIFF		(PE_H15A_MSC_C_M2_OFST - PE_H15A_MSC_C_M1_OFST)

#define PE_H15A_HDMI_A_BASE			(PE_H15A_DE_BASE + PE_H15A_HDMI_A_OFST)
#define PE_H15A_HDMI_B_BASE			(PE_H15A_DE_BASE + PE_H15A_HDMI_B_OFST)
#define PE_H15A_HDMI_C_BASE			(PE_H15A_DE_BASE + PE_H15A_HDMI_C_OFST)
#define PE_H15A_HDMI_D_BASE			(PE_H15A_DE_BASE + PE_H15A_HDMI_D_OFST)
#define PE_H15A_CVI_A_BASE			(PE_H15A_DE_BASE + PE_H15A_CVI_A_OFST)
#define PE_H15A_CVI_B_BASE			(PE_H15A_DE_BASE + PE_H15A_CVI_B_OFST)
#define PE_H15A_CVI_C_BASE			(PE_H15A_DE_BASE + PE_H15A_CVI_C_OFST)
#define PE_H15A_CVI_D_BASE			(PE_H15A_DE_BASE + PE_H15A_CVI_D_OFST)

#define PE_H15A_ND1_BASE			(PE_H15A_DE_BASE + PE_H15A_ND1_OFST)
#define PE_H15A_ND2_BASE			(PE_H15A_DE_BASE + PE_H15A_ND2_OFST)
#define PE_H15A_NDS_BASE			(PE_H15A_DE_BASE + PE_H15A_NDS_OFST)
#define PE_H15A_DNR1_BASE			(PE_H15A_DE_BASE + PE_H15A_DNR1_OFST)
#define PE_H15A_DNR2_BASE			(PE_H15A_DE_BASE + PE_H15A_DNR2_OFST)

#define PE_H15A_MSC_Y_BASE			(PE_H15A_DE_BASE + PE_H15A_MSC_Y_OFST)
#define PE_H15A_MSC_C_BASE			(PE_H15A_DE_BASE + PE_H15A_MSC_C_OFST)
#define PE_H15A_SSC_BASE			(PE_H15A_DE_BASE + PE_H15A_SSC_OFST)

#define PE_H15A_VCP_DE_BASE			(PE_H15A_DE_BASE + PE_H15A_VCP_DE_OFST)
#define PE_H15A_VCP_DISP_BASE		(PE_H15A_DE_BASE + PE_H15A_VCP_DISP_OFST)
#define PE_H15A_VCP_SO_BASE			(PE_H15A_DE_BASE + PE_H15A_VCP_SO_OFST)
#define PE_H15A_VCP_HT_BASE			(PE_H15A_DE_BASE + PE_H15A_VCP_HT_OFST)

#define PE_H15A_SR_SHP_BASE			(PE_H15A_DE_BASE + PE_H15A_SR_SHP_OFST)
#define PE_H15A_SR_F1_BASE			(PE_H15A_DE_BASE + PE_H15A_SR_F1_OFST)
#define PE_H15A_SR_S1_BASE			(PE_H15A_DE_BASE + PE_H15A_SR_S1_OFST)
#define PE_H15A_SR_GSC_BASE			(PE_H15A_DE_BASE + PE_H15A_SR_GSC_OFST)

#define PE_H15A_SHP_TOP_BASE		(PE_H15A_DE_BASE + PE_H15A_SHP_TOP_OFST)
#define PE_H15A_P1_SHP_L_BASE		(PE_H15A_DE_BASE + PE_H15A_P1_SHP_L_OFST)
#define PE_H15A_P1_SHP_R_BASE		(PE_H15A_DE_BASE + PE_H15A_P1_SHP_R_OFST)

#define PE_H15A_P1_CCO_E_BASE		(PE_H15A_DE_BASE + PE_H15A_P1_CCO_E_OFST)
#define PE_H15A_P1_CCO_O_BASE		(PE_H15A_DE_BASE + PE_H15A_P1_CCO_O_OFST)

#define PE_H15A_LED_BASE			(PE_H15A_DE_BASE + PE_H15A_LED_OFST)
#define PE_H15A_LED_RP_BASE			(PE_H15A_DE_BASE + PE_H15A_LED_RP_OFST)

#define PE_H15_DDR_DB_BASE			(gMemCfgBe[3].fw[5].fw_base)

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif	/* _PE_REG_DEF_H15_H_ */

