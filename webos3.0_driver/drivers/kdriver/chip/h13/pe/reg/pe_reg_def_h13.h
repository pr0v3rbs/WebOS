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

/** @file pe_reg_def_h13.h
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

#ifndef _PE_REG_DEF_H13_H_
#define _PE_REG_DEF_H13_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_DTVSOC_DE_H13_BASE		H13_DE_BASE
#define PE_DTVSOC_BVE_H13_BASE		H13_BVE_BASE

#define PE_REG_H13_WRITE_BASE		0x0

#define PE_CVA_REG_H13_OFFSET		0x0100
#define PE_CVB_REG_H13_OFFSET		0x0200
#define PE_CVI_REG_H13_LRDIFF		(PE_CVB_REG_H13_OFFSET-PE_CVA_REG_H13_OFFSET)

#define PE_P0L_REG_H13_OFFSET		0x0400
#define PE_P0R_REG_H13_OFFSET		0x2400
#define PE_PE0_REG_H13_LRDIFF		(PE_P0R_REG_H13_OFFSET-PE_P0L_REG_H13_OFFSET)

#define PE_MSL_REG_H13_OFFSET		0x1100
#define PE_MSR_REG_H13_OFFSET		0x3100
#define PE_MSC_REG_H13_LRDIFF		(PE_MSR_REG_H13_OFFSET-PE_MSL_REG_H13_OFFSET)

#define PE_SSC_REG_H13_OFFSET		0x2100

#define PE_OVL_REG_H13_OFFSET		0x1200
#define PE_OVR_REG_H13_OFFSET		0x3200
#define PE_WCP_REG_H13_LRDIFF		(PE_OVR_REG_H13_OFFSET-PE_OVL_REG_H13_OFFSET)

#define PE_DPP_REG_H13_OFFSET		0x1800
#define PE_P1L_REG_H13_OFFSET		0x5300

#define PE_BVE_REG_H13_OFFSET		0x40a4

#define PE_CVA_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_CVA_REG_H13_OFFSET)
#define PE_CVB_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_CVB_REG_H13_OFFSET)

#define PE_P0L_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_P0L_REG_H13_OFFSET)
#define PE_P0R_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_P0R_REG_H13_OFFSET)

#define PE_MSL_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_MSL_REG_H13_OFFSET)
#define PE_MSR_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_MSR_REG_H13_OFFSET)

#define PE_SSC_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_SSC_REG_H13_OFFSET)

#define PE_OVL_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_OVL_REG_H13_OFFSET)
#define PE_OVR_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_OVR_REG_H13_OFFSET)

#define PE_DPP_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_DPP_REG_H13_OFFSET)
#define PE_P1L_REG_H13_BASE			(PE_DTVSOC_DE_H13_BASE + PE_P1L_REG_H13_OFFSET)

#define PE_BVE_REG_H13_BASE			(PE_DTVSOC_BVE_H13_BASE + PE_BVE_REG_H13_OFFSET)

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

#endif	/* _PE_REG_DEF_H13_H_ */

