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

#ifndef  DE_REG_DEF_M14_INC
#define  DE_REG_DEF_M14_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define DE_VERSION_L9_A0  0x09A0
#define DE_VERSION_L9_B0  0x09B0
#define DE_VERSION_M14_A0 0x13A0
#define DE_VERSION_M14_B0 0x13B0

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define DTVSOC_DE_M14_BASE              M14_DE_BASE
#define DTVSOC_DE_M14B_BASE             M14_B0_DE_BASE
#define DTVSOC_CTOP_CTRL_M14_BASE       (M14_TOPCTRL_BASE + 0x10)

#define DE_DEA_REG_M14_OFFSET           0x0000
#define DE_DEB_REG_M14_OFFSET           0x1000
#define DE_DEC_REG_M14_OFFSET           0x2000
#define DE_DED_REG_M14_OFFSET           0x3000
#define DE_DEE_REG_M14_OFFSET           0x4000
#define DE_DEF_REG_M14_OFFSET		    0x5000

#define DE_CVA_REG_M14_OFFSET           0x0100
#define DE_CVB_REG_M14_OFFSET           0x0200

#define DE_DLR_REG_M14_OFFSET           0x0300

#define DE_P0L_REG_M14_OFFSET           0x0400
#define DE_P0R_REG_M14_OFFSET           0x2400

#define DE_MIA_REG_M14_OFFSET           0x0800
#define DE_MIB_REG_M14_OFFSET           0x1b00
#define DE_MIC_REG_M14_OFFSET           0x2800
#define DE_MID_REG_M14_OFFSET		    0x3900
#define DE_MIF_REG_M14_OFFSET		    0x5900

#define DE_MSL_REG_M14_OFFSET           0x1100
#define DE_MSR_REG_M14_OFFSET           0x3100

#define DE_OVL_REG_M14_OFFSET           0x1200
#define DE_OVR_REG_M14_OFFSET           0x3200

#define DE_DPP_REG_M14_OFFSET           0x1800

#define DE_SSC_REG_M14_OFFSET           0x2100
#define DE_ATP_REG_M14_OFFSET		    0x3800
#define DE_CVD_REG_M14_OFFSET           0x4100

#define DE_IPC_FRM_M14_OFFSET           0x4c00

#define DE_VDI_REG_M14_OFFSET           0x4d00
#define DE_IPC_REG_M14_OFFSET           0x4e00
#define DE_OSD_REG_M14_OFFSET		    0x5100
#define DE_P1L_REG_M14_OFFSET           0x5300
#define DE_DVR_REG_M14_OFFSET           0x5c00
//#define DE_P1R_REG_M14_OFFSET           0x3300
//#define DE_OIF_REG_M14_OFFSET		    0x5C00
//#define DE_D3P_REG_M14_OFFSET		    0x5D20

#define DE_DEA_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DEA_REG_M14_OFFSET)
#define DE_DEB_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DEB_REG_M14_OFFSET)
#define DE_DEC_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DEC_REG_M14_OFFSET)
#define DE_DED_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DED_REG_M14_OFFSET)
#define DE_DEE_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DEE_REG_M14_OFFSET)
#define DE_DEF_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_DEF_REG_M14_OFFSET)

#define DE_CVA_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_CVA_REG_M14_OFFSET)
#define DE_CVB_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_CVB_REG_M14_OFFSET)

//#define DE_DLR_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DLR_REG_M14_OFFSET)

#define DE_P0L_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_P0L_REG_M14_OFFSET)
#define DE_P0R_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_P0R_REG_M14_OFFSET)

#define DE_MIA_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MIA_REG_M14_OFFSET)
#define DE_MIB_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MIB_REG_M14_OFFSET)
#define DE_MIC_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MIC_REG_M14_OFFSET)
#define DE_MID_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MID_REG_M14_OFFSET)
#define DE_MIF_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_MIF_REG_M14_OFFSET)

#define DE_MSL_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MSL_REG_M14_OFFSET)
#define DE_MSR_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_MSR_REG_M14_OFFSET)

#define DE_OVL_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_OVL_REG_M14_OFFSET)
#define DE_OVR_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_OVR_REG_M14_OFFSET)

#define DE_DPP_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DPP_REG_M14_OFFSET)

#define DE_P1L_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_P1L_REG_M14_OFFSET)
//#define DE_P1R_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_P1R_REG_M14_OFFSET)

#define DE_SSC_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_SSC_REG_M14_OFFSET)
//#define DE_T3D_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_T3D_REG_M14_OFFSET)
#define DE_CVD_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_CVD_REG_M14_OFFSET)
#define DE_IPC_FRM_M14_BASE             (DTVSOC_DE_M14_BASE + DE_IPC_FRM_M14_OFFSET)
#define DE_VDI_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_VDI_REG_M14_OFFSET)
#define DE_IPC_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_IPC_REG_M14_OFFSET)
#define DE_OSD_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_OSD_REG_M14_OFFSET)
#define DE_DVR_REG_M14_BASE             (DTVSOC_DE_M14_BASE + DE_DVR_REG_M14_OFFSET)
//#define DE_OIF_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_OIF_REG_M14_OFFSET)
//#define DE_D3P_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_D3P_REG_M14_OFFSET)
#define DE_ATP_REG_M14_BASE		   		(DTVSOC_DE_M14_BASE + DE_ATP_REG_M14_OFFSET)
#define DE_IPC1_REG_M14_BASE              0xC0024000

#define VDEC_CPU_IPC_OFFSET			   (0x0E00)
#define VDEC_CPU_IPC_OFFSET_B0		   (0x1000)
#define VDEC_CPC_IPC_OFFSET			   (0x40 * 2)
#define VDEC_CPD_IPC_OFFSET			   (0x40 * 3)

#define VDEC_CPU_IPC_BASE			   (M14_VDEC_BASE      + VDEC_CPU_IPC_OFFSET)
#define VDEC_CPC_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPC_IPC_OFFSET)
#define VDEC_CPD_IPC_BASE			   (VDEC_CPU_IPC_BASE  + VDEC_CPD_IPC_OFFSET)
#define VDEC_CPC_IPC_BASE_B0		   (VDEC_CPU_IPC_BASE  + VDEC_CPC_IPC_OFFSET + VDEC_CPU_IPC_OFFSET_B0)
#define VDEC_CPD_IPC_BASE_B0		   (VDEC_CPU_IPC_BASE  + VDEC_CPD_IPC_OFFSET + VDEC_CPU_IPC_OFFSET_B0)

#define DPPB_TOP_REG_M14A0_BASE        0xC0024000
#define DPPB_TOP_IPC_M14A0_OFFSET      0x00000080 


// b0

#define DE_DEI_REG_M14B_OFFSET           0x0000
#define DE_DEM_REG_M14B_OFFSET           0x2000
#define DE_DEO_REG_M14B_OFFSET           0x3000
#define DE_DEP_REG_M14B_OFFSET           0x4000
#define DE_CVA_REG_M14B_OFFSET           0x0100
#define DE_CVB_REG_M14B_OFFSET           0x0200
#define DE_P0M_REG_M14B_OFFSET           0x2100
#define DE_P0D_REG_M14B_OFFSET           0x3A00
#define DE_P0S_REG_M14B_OFFSET           0x3100
#define DE_MII_REG_M14B_OFFSET           0x0E00
#define DE_MIM_REG_M14B_OFFSET           0x2E00
#define DE_MIO_REG_M14B_OFFSET           0x3E00
#define DE_MIOSD_REG_M14B_OFFSET         0x4E00
#define DE_MOF_REG_M14B_OFFSET           0x4D00
#define DE_MSC_REG_M14B_OFFSET           0x3800
#define DE_PE1_REG_M14B_OFFSET           0x4300
#define DE_SSC_REG_M14B_OFFSET           0x3900
#define DE_OSD_REG_M14B_OFFSET           0x4100
#define DE_CVD_REG_M14B_OFFSET           0x1100
#define DE_ATP_REG_M14B_OFFSET           0x3D00
#define DE_DEC3D_REG_M14B_OFFSET         0x0400
#define DE_FD3_REG_M14B_OFFSET           0x0D00
#define DE_IPC_FRM_M14B_OFFSET           0x5100
#define DE_IPC_REG_M14B_OFFSET           0x5000
#define DE_SMM_REG_M14B_OFFSET           0x0300
#define DE_VBI_REG_M14B_OFFSET           0x1D00
#define DE_VDO_GDI_REG_M14B_OFFSET       0x5400
#define DE_VDO_RC_REG_M14B_OFFSET        0x1C00
#define DE_VDO_SYNC_REG_M14B_OFFSET      0x1B00
#define DE_VDO_CH_REG_M14B_OFFSET        0x1000
#define DE_VDO_IPC_REG_M14B_OFFSET       0x5200
#define DE_COMP_GEN_REG_M14B_OFFSET     0x10000

#define DE_DEI_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_DEI_REG_M14B_OFFSET)
#define DE_DEM_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_DEM_REG_M14B_OFFSET)
#define DE_DEO_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_DEO_REG_M14B_OFFSET)
#define DE_DEP_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_DEP_REG_M14B_OFFSET)
#define DE_CVA_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_CVA_REG_M14B_OFFSET)
#define DE_CVB_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_CVB_REG_M14B_OFFSET)
#define DE_P0M_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_P0M_REG_M14B_OFFSET)
#define DE_P0D_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_P0D_REG_M14B_OFFSET)
#define DE_P0S_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_P0S_REG_M14B_OFFSET)
#define DE_MII_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_MII_REG_M14B_OFFSET)
#define DE_MIM_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_MIM_REG_M14B_OFFSET)
#define DE_MIO_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_MIO_REG_M14B_OFFSET)
#define DE_MIOSD_REG_M14B_BASE           (DTVSOC_DE_M14B_BASE + DE_MIOSD_REG_M14B_OFFSET)
#define DE_MSC_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_MSC_REG_M14B_OFFSET)
#define DE_PE1_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_PE1_REG_M14B_OFFSET)
#define DE_SSC_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_SSC_REG_M14B_OFFSET)
#define DE_OSD_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_OSD_REG_M14B_OFFSET)
#define DE_CVD_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_CVD_REG_M14B_OFFSET)
#define DE_ATP_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_ATP_REG_M14B_OFFSET)
#define DE_DEC3D_REG_M14B_BASE           (DTVSOC_DE_M14B_BASE + DE_DEC3D_REG_M14B_OFFSET)
#define DE_FD3_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_FD3_REG_M14B_OFFSET)
#define DE_IPC_FRM_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_IPC_FRM_M14B_OFFSET)
#define DE_IPC_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_IPC_REG_M14B_OFFSET)
#define DE_SMM_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_SMM_REG_M14B_OFFSET)
#define DE_VBI_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_VBI_REG_M14B_OFFSET)
#define DE_VDO_GDI_REG_M14B_BASE         (DTVSOC_DE_M14B_BASE + DE_VDO_GDI_REG_M14B_OFFSET)
#define DE_VDO_RC_REG_M14B_BASE          (DTVSOC_DE_M14B_BASE + DE_VDO_RC_REG_M14B_OFFSET)
#define DE_VDO_SYNC_REG_M14B_BASE        (DTVSOC_DE_M14B_BASE + DE_VDO_SYNC_REG_M14B_OFFSET)
#define DE_VDO_CH_REG_M14B_BASE          (DTVSOC_DE_M14B_BASE + DE_VDO_CH_REG_M14B_OFFSET)
#define DE_VDO_IPC_REG_M14B_BASE         (DTVSOC_DE_M14B_BASE + DE_VDO_IPC_REG_M14B_OFFSET)
#define DE_COMP_GEN_REG_M14B_BASE        (DTVSOC_DE_M14B_BASE + DE_COMP_GEN_REG_M14B_OFFSET)
#define DE_ATP_REG_M14B_BASE             (DTVSOC_DE_M14B_BASE + DE_ATP_REG_M14B_OFFSET)

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
