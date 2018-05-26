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

#ifndef  DE_REG_DEF_H15_INC
#define  DE_REG_DEF_H15_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define DE_VERSION_L9_A0  0x09A0
#define DE_VERSION_L9_B0  0x09B0
#define DE_VERSION_H13_A0 0x13A0
#define DE_VERSION_H13_B0 0x13B0
#define DE_VERSION_H15_A0 0x14A0


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#if 0//def H15_DE_BASE
#define DTVSOC_DE_H15_BASE              H15_DE_BASE
#define DTVSOC_CTOP_CTRL_H15_BASE       (H15_TOPCTRL_BASE + 0x10)
#else
#define DTVSOC_DE_H15_BASE              0xC8000000
#define DTVSOC_CTOP_CTRL_H15_BASE       0xC0000000
#endif

#define DE_CVC_REG_H15_OFFSET           0x00000
#define DE_VBI_REG_H15_OFFSET           0x00F00
#define DE_IMX_REG_H15_OFFSET           0x01000
#define DE_SMM_REG_H15_OFFSET           0x01400
#define DE_ND0_REG_H15_OFFSET           0x04000
#define DE_ND1_REG_H15_OFFSET           0x05000
#define DE_SUB_REG_H15_OFFSET           0x06000
#define DE_GSC_REG_H15_OFFSET           0x07000
#define DE_VSD_REG_H15_OFFSET           0x0B000
#define DE_CCO_REG_H15_OFFSET           0x24000

#define DE_HDI_REG_H15_OFFSET           0x00100
#define DE_CVI_REG_H15_OFFSET           0x00200
#define DE_CVD_REG_H15_OFFSET           0x00600

#define DE_IPC_FRM_H15_OFFSET           0x26100
#define DE_IPC_REG_H15_OFFSET           0x26000

#define DE_ATP_REG_H15_OFFSET           0x06600

#define DE_IPC_FRM_H15_BASE             (DTVSOC_DE_H15_BASE + DE_IPC_FRM_H15_OFFSET)
#define DE_IPC_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_IPC_REG_H15_OFFSET)
#define DE_ATP_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_ATP_REG_H15_OFFSET)
#define DE_VBI_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_VBI_REG_H15_OFFSET)
#define DE_CVC_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_CVC_REG_H15_OFFSET)
#define DE_VSD_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_VSD_REG_H15_OFFSET)
#define DE_CCO_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_CCO_REG_H15_OFFSET)
#define DE_IMX_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_IMX_REG_H15_OFFSET)
#define DE_ND0_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_ND0_REG_H15_OFFSET)
#define DE_ND1_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_ND1_REG_H15_OFFSET)
#define DE_SUB_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_SUB_REG_H15_OFFSET)
#define DE_CVI_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_CVI_REG_H15_OFFSET)
#define DE_CVD_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_CVD_REG_H15_OFFSET)
#define DE_ATP_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_ATP_REG_H15_OFFSET)
#define DE_SMM_REG_H15_BASE             (DTVSOC_DE_H15_BASE + DE_SMM_REG_H15_OFFSET)


#define VDEC_CPU_IPC_OFFSET			   (0x0E00)
#define VDEC_CPC_IPC_OFFSET			   (0x40 * 2)
#define VDEC_CPD_IPC_OFFSET			   (0x40 * 3)

#define VDEC_CPU_IPC_BASE			   (H15_VDEC_BASE      + VDEC_CPU_IPC_OFFSET)
#define VDEC_CPC_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPC_IPC_OFFSET)
#define VDEC_CPD_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPD_IPC_OFFSET)

#ifndef H15_IRQ_IPC_BCPU
#define H15_IRQ_IPC_BCPU               H15_IRQ_IDS0_BCPU
#define H15_IRQ_DE_BCPU                H15_IRQ_IDS1_BCPU
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

#endif
/**  @} */
