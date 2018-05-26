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

/** @file pe_reg_def_m14.h
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

#ifndef _PE_REG_DEF_M14_H_
#define _PE_REG_DEF_M14_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
/* define for using m14b wcp(ov) register
* should be undef, because of m14b ov removed */
#undef PE_HW_M14_USE_M14B_WCP_REG

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_DTVSOC_DE_M14_BASE		M14_DE_BASE
#define PE_DTVSOC_BVE_M14_BASE		M14_BVE_BASE

/* M14B_DE_BASE */
#define PE_DTVSOC_DE_M14B_BASE		0xC0020000
#define PE_DTVSOC_BE_M14B_BASE		0xC0040000

/* A0 */
#define PE_REG_M14_WRITE_BASE		0x0
#define PE_CVA_REG_M14_OFFSET		0x0100
#define PE_CVB_REG_M14_OFFSET		0x0200
#define PE_CVI_REG_M14_LRDIFF		(PE_CVB_REG_M14_OFFSET-PE_CVA_REG_M14_OFFSET)
#define PE_P0L_REG_M14_OFFSET		0x0400
#define PE_P0R_REG_M14_OFFSET		0x2400
#define PE_PE0_REG_M14_LRDIFF		(PE_P0R_REG_M14_OFFSET-PE_P0L_REG_M14_OFFSET)
#define PE_MSL_REG_M14_OFFSET		0x1100
#define PE_MSR_REG_M14_OFFSET		0x3100
#define PE_MSC_REG_M14_LRDIFF		(PE_MSR_REG_M14_OFFSET-PE_MSL_REG_M14_OFFSET)
#define PE_SSC_REG_M14_OFFSET		0x2100
#define PE_OVL_REG_M14_OFFSET		0x1200
#define PE_OVR_REG_M14_OFFSET		0x3200
#define PE_WCP_REG_M14_LRDIFF		(PE_OVR_REG_M14_OFFSET-PE_OVL_REG_M14_OFFSET)
#define PE_P1L_REG_M14_OFFSET		0x5300
#define PE_BVE_REG_M14_OFFSET		0x40a4

#define PE_CVA_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_CVA_REG_M14_OFFSET)
#define PE_CVB_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_CVB_REG_M14_OFFSET)
#define PE_P0L_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_P0L_REG_M14_OFFSET)
#define PE_P0R_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_P0R_REG_M14_OFFSET)
#define PE_MSL_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_MSL_REG_M14_OFFSET)
#define PE_MSR_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_MSR_REG_M14_OFFSET)
#define PE_SSC_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_SSC_REG_M14_OFFSET)
#define PE_OVL_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_OVL_REG_M14_OFFSET)
#define PE_OVR_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_OVR_REG_M14_OFFSET)
#define PE_P1L_REG_M14_BASE			(PE_DTVSOC_DE_M14_BASE + PE_P1L_REG_M14_OFFSET)
#define PE_BVE_REG_M14_BASE			(PE_DTVSOC_BVE_M14_BASE + PE_BVE_REG_M14_OFFSET)

/* B0 */
#define PE_REG_M14B_WRITE_BASE		0x0

#define PE_CVA_REG_M14B_OFFSET		0x0100
#define PE_CVB_REG_M14B_OFFSET		0x0200
#define PE_CVI_REG_M14B_LRDIFF		(PE_CVB_REG_M14B_OFFSET-PE_CVA_REG_M14B_OFFSET)

#define PE_P0M_REG_M14B_OFFSET		0x2100
#define PE_P0D_REG_M14B_OFFSET		0x3A00
#define PE_P0S_REG_M14B_OFFSET		0x3100
#define PE_PE0_REG_M14B_MSDIFF		(PE_P0S_REG_M14B_OFFSET-PE_P0M_REG_M14B_OFFSET)
#define PE_MSC_REG_M14B_OFFSET		0x3800
#define PE_SSC_REG_M14B_OFFSET		0x3900
#define PE_PE1_REG_M14B_OFFSET		0x4300
#define PE_LED_REG_M14B_OFFSET		0x0540
#ifdef PE_HW_M14_USE_M14B_WCP_REG
#define PE_OVL_REG_M14B_OFFSET		0x4200
#define PE_OVR_REG_M14B_OFFSET		0xA200
#define PE_WCP_REG_M14B_LRDIFF		(PE_OVR_REG_M14B_OFFSET-PE_OVL_REG_M14B_OFFSET)
#endif

#define PE_CVA_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_CVA_REG_M14B_OFFSET)
#define PE_CVB_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_CVB_REG_M14B_OFFSET)
#define PE_P0M_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_P0M_REG_M14B_OFFSET)
#define PE_P0D_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_P0D_REG_M14B_OFFSET)
#define PE_P0S_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_P0S_REG_M14B_OFFSET)
#define PE_MSC_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_MSC_REG_M14B_OFFSET)
#define PE_PE1_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_PE1_REG_M14B_OFFSET)
#define PE_SSC_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_SSC_REG_M14B_OFFSET)
#define PE_LED_REG_M14B_BASE		(PE_DTVSOC_BE_M14B_BASE + PE_LED_REG_M14B_OFFSET)
#ifdef PE_HW_M14_USE_M14B_WCP_REG
#define PE_OVL_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_OVL_REG_M14B_OFFSET)
#define PE_OVR_REG_M14B_BASE		(PE_DTVSOC_DE_M14B_BASE + PE_OVR_REG_M14B_OFFSET)
#endif

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

#endif	/* _PE_REG_DEF_M14_H_ */

