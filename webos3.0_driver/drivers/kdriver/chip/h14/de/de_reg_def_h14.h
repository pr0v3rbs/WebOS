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

#ifndef  DE_REG_DEF_H14_INC
#define  DE_REG_DEF_H14_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define DE_VERSION_L9_A0  0x09A0
#define DE_VERSION_L9_B0  0x09B0
#define DE_VERSION_H13_A0 0x13A0
#define DE_VERSION_H13_B0 0x13B0
#define DE_VERSION_H14_A0 0x14A0


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define DTVSOC_DE_H14_BASE              	H14_DE_BASE
#define DTVSOC_CTOP_CTRL_H14_BASE       (H14_TOPCTRL_BASE + 0x10)

#define DE_DEA_REG_H14_OFFSET           0x0000	// check
#define DE_DEB_REG_H14_OFFSET           0x1000	// check
#define DE_DEC_REG_H14_OFFSET           0x2000	// check
#define DE_DED_REG_H14_OFFSET           0x3000	// check
#define DE_DEE_REG_H14_OFFSET           0x4000	// check
#define DE_DEF_REG_H14_OFFSET		    0x5000	// check

#define DE_CVA_REG_H14_OFFSET           0x0100 // check
#define DE_CVB_REG_H14_OFFSET           0x0200 // check

#define DE_DLR_REG_H14_OFFSET           0x0300 // question

#define DE_P0L_REG_H14_OFFSET           0x0400
#define DE_P0R_REG_H14_OFFSET           0x2400

//#define DE_MIA_REG_H14_OFFSET         0x0800 // h13b0
//#define DE_MIB_REG_H14_OFFSET         0x1b00 // h13b0
//#define DE_MIC_REG_H14_OFFSET         0x2800 // h13b0
//#define DE_MID_REG_H14_OFFSET		    0x3900 // h13b0
//#define DE_MIF_REG_H14_OFFSET		    0x5900 // h13b0
#define DE_MIA_REG_H14_OFFSET           0x0e00 // check
#define DE_MIB_REG_H14_OFFSET           0x1e00 // check
#define DE_MIC_REG_H14_OFFSET           0x2e00 // check
#define DE_MID_REG_H14_OFFSET		    0x3e00 // check
#define DE_MIF_REG_H14_OFFSET		    0x5e00 // check

#define DE_MSL_REG_H14_OFFSET           0x1100 // check
#define DE_MSR_REG_H14_OFFSET           0x3100 // check

#define DE_OVL_REG_H14_OFFSET           0x1200 // check (wcp)
#define DE_OVR_REG_H14_OFFSET           0x3200 // check (wcp)

#define DE_DPP_REG_H14_OFFSET           0x1800 // check (remove h14)

#define DE_SSC_REG_H14_OFFSET           0x2100 // check
#define DE_ATP_REG_H14_OFFSET		    0x3800	// check
#define DE_CVD_REG_H14_OFFSET           0x4100 // check

#define DE_IPC_FRM_H14_OFFSET           0x4c00

#define DE_VDI_REG_H14_OFFSET           0x4d00 // check
#define DE_IPC_REG_H14_OFFSET           0x4e00 // check
#define DE_OSD_REG_H14_OFFSET		    0x5100 // check
#define DE_P1L_REG_H14_OFFSET           0x5300
#define DE_DVR_REG_H14_OFFSET           0x5c00 // check
//#define DE_P1R_REG_H14_OFFSET           0x3300
//#define DE_OIF_REG_H14_OFFSET		    0x5C00
//#define DE_D3P_REG_H14_OFFSET		    0x5D20

#define DE_DEA_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DEA_REG_H14_OFFSET)
#define DE_DEB_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DEB_REG_H14_OFFSET)
#define DE_DEC_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DEC_REG_H14_OFFSET)
#define DE_DED_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DED_REG_H14_OFFSET)
#define DE_DEE_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DEE_REG_H14_OFFSET)
#define DE_DEF_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_DEF_REG_H14_OFFSET)

#define DE_CVA_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_CVA_REG_H14_OFFSET)
#define DE_CVB_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_CVB_REG_H14_OFFSET)

//#define DE_DLR_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DLR_REG_H14_OFFSET)

#define DE_P0L_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_P0L_REG_H14_OFFSET)
#define DE_P0R_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_P0R_REG_H14_OFFSET)

#define DE_MIA_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MIA_REG_H14_OFFSET)
#define DE_MIB_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MIB_REG_H14_OFFSET)
#define DE_MIC_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MIC_REG_H14_OFFSET)
#define DE_MID_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MID_REG_H14_OFFSET)
#define DE_MIF_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_MIF_REG_H14_OFFSET)

#define DE_MSL_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MSL_REG_H14_OFFSET)
#define DE_MSR_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_MSR_REG_H14_OFFSET)

#define DE_OVL_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_OVL_REG_H14_OFFSET)
#define DE_OVR_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_OVR_REG_H14_OFFSET)

#define DE_DPP_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DPP_REG_H14_OFFSET)

#define DE_P1L_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_P1L_REG_H14_OFFSET)
//#define DE_P1R_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_P1R_REG_H14_OFFSET)

#define DE_SSC_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_SSC_REG_H14_OFFSET)
//#define DE_T3D_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_T3D_REG_H14_OFFSET)
#define DE_CVD_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_CVD_REG_H14_OFFSET)
#define DE_IPC_FRM_H14_BASE             (DTVSOC_DE_H14_BASE + DE_IPC_FRM_H14_OFFSET)
#define DE_VDI_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_VDI_REG_H14_OFFSET)
#define DE_IPC_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_IPC_REG_H14_OFFSET)
#define DE_OSD_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_OSD_REG_H14_OFFSET)
#define DE_DVR_REG_H14_BASE             (DTVSOC_DE_H14_BASE + DE_DVR_REG_H14_OFFSET)
//#define DE_OIF_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_OIF_REG_H14_OFFSET)
//#define DE_D3P_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_D3P_REG_H14_OFFSET)
#define DE_ATP_REG_H14_BASE		   		(DTVSOC_DE_H14_BASE + DE_ATP_REG_H14_OFFSET)
#define DE_IPC1_REG_H14_BASE             0xC0024000

#define VDEC_CPU_IPC_OFFSET			   (0x0E00)
#define VDEC_CPC_IPC_OFFSET			   (0x40 * 2)
#define VDEC_CPD_IPC_OFFSET			   (0x40 * 3)

#define VDEC_CPU_IPC_BASE			   (H14_VDEC_BASE      + VDEC_CPU_IPC_OFFSET)
#define VDEC_CPC_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPC_IPC_OFFSET)
#define VDEC_CPD_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPD_IPC_OFFSET)

#define DPPB_TOP_REG_H14_BASE          0xC0024000
#define DPPB_TOP_IPC_H14_OFFSET        0x00000080 

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
