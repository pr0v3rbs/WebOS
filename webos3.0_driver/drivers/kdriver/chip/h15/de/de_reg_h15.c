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
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2011.02.18
 * note		  Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/
#define USE_DE_CVI_ACCESS_REGISTER_ON_MCU_PART
#undef	USE_DE_DOES_RESET_IN_SUSPEND_RESUME
#undef  USE_FRAME_COPY_TO_NFS_FILE

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <asm/io.h>

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <asm/uaccess.h>

#include "base_types.h"
#include "os_util.h"

#include "de_model.h"
#include "de_ver_def.h"

#ifdef USE_KDRV_CODES_FOR_H15
#include "de_kapi.h"
#include "de_def.h"
#include "de_prm_def.h"
#include "de_hal_def.h"
#include "de_cfg.h"
#include "de_drv.h"

#include "de_cfg_h15.h"
#include "de_ipc_def_h15.h"
#include "de_int_def_h15.h"
#include "de_reg_def_h15.h"
#include "de_reg_h15.h"
#include "de_prm_h15.h"

#include "de_ctr_reg_h15.h"		  // 0x0000
#include "de_ipc_reg_h15.h"		  // 0x4e00
#include "de_vbi_reg_h15.h"		  // 0x0f00
#include "de_cvi_reg_h15.h"		  // 0x0200
#include "de_cvd_reg_h15.h"		  // 
#include "de_atp_reg_h15.h"		  // 
#include "de_smux_reg_h15.h"	  // 


#ifdef USE_CTOP_CODES_FOR_H15
#include "../sys/sys_regs.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define  SIZE_OF_IPC_FOR_CPU 16
#define USE_VIDEO_MCU_RUN_IN_DDR0   0x00000000
#define USE_VIDEO_MCU_ROM_BASE_ADDR 0x50000000
#define USE_VIDEO_MCU_ROM_FW_OFFSET 0x00000000
#define DE_DDR_FIRMWARE_TAG_COUNT    2

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
#define NUM_OF_CVI_FIR_COEF 8

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/
#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
extern LX_DE_MEM_CFG_T *gpDeMem;
#endif
#ifdef INCLUDE_KDRV_BE
#include "../be/be_cfg.h"
extern BE_MEM_CFG_T *gpBeMem;
#endif
/*----------------------------------------------------------------------------------------
 *	 global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 global Variables
 *---------------------------------------------------------------------------------------*/
DE_IPC_REG_H15_T gDE_IPC_H15;
DE_VBI_REG_H15_T gDE_VBI_H15;

DE_CVC_REG_H15_T gDE_CVC_H15;
DE_VSD_REG_H15_T gDE_VSD_H15;
DE_CCO_REG_H15_T gDE_CCO_H15;
DE_IMX_REG_H15_T gDE_IMX_H15;
DE_ND0_REG_H15_T gDE_ND0_H15;
DE_ND1_REG_H15_T gDE_ND1_H15;
DE_SUB_REG_H15_T gDE_SUB_H15;
DE_CVI_REG_H15_T gDE_CVI_H15;
DE_CVD_REG_H15_T gDE_CVD_H15;
DE_SMM_REG_H15_T gDE_SMM_H15;

LX_DE_IN_SRC_T	g_WinsrcMap_H15[2] = { LX_DE_IN_SRC_MVI,	LX_DE_IN_SRC_MVI};
UINT32			g_WinsrcPort_H15[2] = { 0, 0};
UINT32          g_HdmiPort = 0;

LX_DE_OPER_CONFIG_T		g_SrcOperType_H15 = LX_DE_OPER_ONE_WIN;
LX_DE_SUB_OPER_CONFIG_T	g_SrcSubOperType_H15 = LX_DE_SUB_OPER_OFF;
UINT16						g_SrcOperCtrlFlag_H15 = 0;
UINT16						g_SrcSubOperCtrlFlag_H15 = 0;

LX_DE_DISPLAY_DEVICE_T	g_Display_type_H15 = LX_DE_DIS_DEV_LCD;
LX_DE_DISPLAY_MIRROR_T	g_Display_mirror_H15 = LX_DE_DIS_MIRROR_OFF;
LX_DE_PANEL_TYPE_T 		g_Display_size_H15 = LX_PANEL_TYPE_3840;
LX_DE_FRC_PATH_T		g_Frc_type_H15 = LX_DE_FRC_PATH_EXTERNAL;
LX_DE_3D_CTRL_T			g_Trid_type_H15 = LX_DE_3D_CTRL_ON;

typedef struct {
	 SRC_MAP_DATA    src;
	 BOOLEAN         valid;
	 CVI_DATA_IN_SEL cvi_0;
	 CVI_DATA_IN_SEL cvi_1;
	 CVI_DATA_IN_SEL cvi_2;
	 CVI_DATA_IN_SEL cvi_3;
} LX_DE_SRC_CONFIG;

LX_DE_SRC_CONFIG sSrc_map_H15[2][LX_DE_MULTI_IN_MAX + 1] =
{
	// A0
	{	/* src_map       valid   cvi_0       cvi_1        cvi_2       cvi_3 */
		{  SRC_CVD_ADC,  TRUE,   CLK_CVD,    CLK_NONE,    CLK_ADC,    CLK_NONE},
		{  SRC_CVD_HDA,  TRUE,   CLK_CVD,    CLK_NONE,    CLK_HDA,    CLK_HDA },
		{  SRC_CVD_HDC,  TRUE,   CLK_CVD,    CLK_NONE,    CLK_HDC,    CLK_HDC },
		{  SRC_CVD_CVD,  TRUE,   CLK_CVD,    CLK_NONE,    CLK_CVD,    CLK_NONE},
		{  SRC_ADC_CVD,  TRUE,   CLK_ADC,    CLK_NONE,    CLK_CVD,    CLK_NONE},
		{  SRC_ADC_HDA,  TRUE,   CLK_ADC,    CLK_NONE,    CLK_HDA,    CLK_HDA },
		{  SRC_ADC_HDC,  TRUE,   CLK_ADC,    CLK_NONE,    CLK_HDC,    CLK_HDC },
		{  SRC_ADC_ADC,  TRUE,   CLK_ADC,    CLK_NONE,    CLK_ADC,    CLK_NONE},
		{  SRC_HDA_HDA,  TRUE,   CLK_HDA,    CLK_HDA,     CLK_HDA,    CLK_HDA },
		{  SRC_HDA_ADC,  TRUE,   CLK_HDA,    CLK_HDA,     CLK_ADC,    CLK_NONE},
		{  SRC_HDA_HDC,  TRUE,   CLK_HDA,    CLK_HDA,     CLK_HDC,    CLK_HDC },
		{  SRC_HDA_HDA,  TRUE,   CLK_HDA,    CLK_HDA,     CLK_HDA,    CLK_HDA },
		{  SRC_HDC_CVD,  TRUE,   CLK_HDC,    CLK_HDC,     CLK_CVD,    CLK_NONE},
		{  SRC_HDC_ADC,  TRUE,   CLK_HDC,    CLK_HDC,     CLK_ADC,    CLK_NONE},
		{  SRC_HDC_HDA,  TRUE,   CLK_HDC,    CLK_HDC,     CLK_HDA,    CLK_HDA },
		{  SRC_HDC_HDC,  TRUE,   CLK_HDC,    CLK_HDC,     CLK_HDC,    CLK_HDC }
	},
	// B0
	{
	}
};


/*----------------------------------------------------------------------------------------
 *	 Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
int DE_REG_H15_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_T intrType, UINT32 *pVideoIrqStatus);

/*----------------------------------------------------------------------------------------
 *	 Static Variables
 *---------------------------------------------------------------------------------------*/
#ifndef _H15_FPGA_TEMPORAL_
static UINT32		   *spVideoIPCofCPU[2]	  = {NULL};
#endif
#ifdef USE_CTOP_CODES_FOR_H15
static DE_DPLL_SET_T sDisplayPll_H15[] = {
	{ DCLK_61_875	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_65		,{ 0 ,0x2 ,0x2 } },
	{ DCLK_66_462	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_66_528	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_36_818	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_36_855	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_54		,{ 0 ,0x2 ,0x2 } },
	{ DCLK_54_054	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_74_1758	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_74_25	,{ 0 ,0x2 ,0x3 } },
	{ DCLK_80_109	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_80_190	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_81		,{ 0 ,0x2 ,0x2 } },
	{ DCLK_27		,{ 0 ,0x2 ,0x2 } },
	{ DCLK_13_5		,{ 0 ,0x2 ,0x2 } },
	{ DCLK_27_027	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_13_5135	,{ 0 ,0x2 ,0x2 } },
	{ DCLK_148_3516 ,{ 0 ,0x2 ,0x2 } },
	{ DCLK_148_5	,{ 0x4 ,0xA ,0x2 } },
	{ DCLK_85_86	,{ 0 ,0x2 ,0x2 } },
};
#endif
static BOOLEAN sbDeUdMode = FALSE;
static LX_DE_CVI_SRC_TYPE_T sCviSrcType;
static LX_DE_MULTI_WIN_SRC_T sDeMultiWinSrc = {0,0};
static LX_DE_CH_MEM_T *gpAdrPreW;
static LX_DE_CH_MEM_T *gpAdrGrap;

/*========================================================================================
 *	 Implementation Group
 *=======================================================================================*/

int DE_REG_H15_InitAddrSwitch(void)
{

	return 0;
}
/**
 * @callgraph
 * @callergraph
 *
 * @brief Initialize Reigerter Physical Address to Virtual Address and Make Shadow Register
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_InitPHY2VIRT(void)
{
	int ret = RET_OK;

	gDE_IPC_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_IPC_REG_H15A0_T));
	gDE_IPC_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_IPC_REG_H15_BASE, sizeof(DE_IPC_REG_H15A0_T));
	gDE_VBI_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_VBI_REG_H15A0_T));
	gDE_VBI_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_VBI_REG_H15_BASE, sizeof(DE_VBI_REG_H15A0_T));

	gDE_CVC_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_CVC_REG_H15A0_T));
	gDE_CVC_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_CVC_REG_H15_BASE, sizeof(DE_CVC_REG_H15A0_T));
	gDE_VSD_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_VSD_REG_H15A0_T));
	gDE_VSD_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_VSD_REG_H15_BASE, sizeof(DE_VSD_REG_H15A0_T));
	gDE_CCO_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_CCO_REG_H15A0_T));
	gDE_CCO_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_CCO_REG_H15_BASE, sizeof(DE_CCO_REG_H15A0_T));
	gDE_IMX_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_IMX_REG_H15A0_T));
	gDE_IMX_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_IMX_REG_H15_BASE, sizeof(DE_IMX_REG_H15A0_T));
	gDE_ND0_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_ND0_REG_H15A0_T));
	gDE_ND0_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_ND0_REG_H15_BASE, sizeof(DE_ND0_REG_H15A0_T));
	gDE_ND1_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_ND1_REG_H15A0_T));
	gDE_ND1_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_ND1_REG_H15_BASE, sizeof(DE_ND1_REG_H15A0_T));
	gDE_SUB_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_SUB_REG_H15A0_T));
	gDE_SUB_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_SUB_REG_H15_BASE, sizeof(DE_SUB_REG_H15A0_T));
	gDE_CVI_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_CVI_REG_H15A0_T));
	gDE_CVI_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_CVI_REG_H15_BASE, sizeof(DE_CVI_REG_H15A0_T));
	gDE_CVD_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_CVD_REG_H15A0_T));
	gDE_CVD_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_CVD_REG_H15_BASE, sizeof(DE_CVD_REG_H15A0_T));
	gDE_SMM_H15.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(DE_SMM_REG_H15A0_T));
	gDE_SMM_H15.phys.addr  = (volatile UINT32 *)ioremap(DE_SMM_REG_H15_BASE, sizeof(DE_SMM_REG_H15A0_T));

	gpDeMem = &gMemCfgDe[4];
	gpAdrPreW = &gMemCfgDePreW[4];
	gpAdrGrap = &gMemCfgDeGrap[4];

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Free memory alocated in Shadow Register
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_FreePHY2VIRT(void)
{
	OS_Free((void *)gDE_IPC_H15.shdw.addr);
	if (gDE_IPC_H15.phys.addr) iounmap((void *)gDE_IPC_H15.phys.addr);
	OS_Free((void *)gDE_VBI_H15.shdw.addr);
	if (gDE_VBI_H15.phys.addr) iounmap((void *)gDE_VBI_H15.phys.addr);
	OS_Free((void *)gDE_CVC_H15.shdw.addr);
	if (gDE_CVC_H15.phys.addr) iounmap((void *)gDE_CVC_H15.phys.addr);
	OS_Free((void *)gDE_VSD_H15.shdw.addr);
	if (gDE_VSD_H15.phys.addr) iounmap((void *)gDE_VSD_H15.phys.addr);
	OS_Free((void *)gDE_CCO_H15.shdw.addr);
	if (gDE_CCO_H15.phys.addr) iounmap((void *)gDE_CCO_H15.phys.addr);
	OS_Free((void *)gDE_IMX_H15.shdw.addr);
	if (gDE_IMX_H15.phys.addr) iounmap((void *)gDE_IMX_H15.phys.addr);
	OS_Free((void *)gDE_ND0_H15.shdw.addr);
	if (gDE_ND0_H15.phys.addr) iounmap((void *)gDE_ND0_H15.phys.addr);
	OS_Free((void *)gDE_ND1_H15.shdw.addr);
	if (gDE_ND1_H15.phys.addr) iounmap((void *)gDE_ND1_H15.phys.addr);
	OS_Free((void *)gDE_SUB_H15.shdw.addr);
	if (gDE_SUB_H15.phys.addr) iounmap((void *)gDE_SUB_H15.phys.addr);
	OS_Free((void *)gDE_CVI_H15.shdw.addr);
	if (gDE_CVI_H15.phys.addr) iounmap((void *)gDE_CVI_H15.phys.addr);
	OS_Free((void *)gDE_CVD_H15.shdw.addr);
	if (gDE_CVD_H15.phys.addr) iounmap((void *)gDE_CVD_H15.phys.addr);
	OS_Free((void *)gDE_SMM_H15.shdw.addr);
	if (gDE_SMM_H15.phys.addr) iounmap((void *)gDE_SMM_H15.phys.addr);

	return RET_OK;
}

 /**
 * @callgraph
 * @callergraph
 *
 * @brief Get Status of Interrupt which is one of either MCU or CPU
 *
 * @param ipcType [IN] one of either MCU and CPU
 * @param pStatus [OUT] status pointer of Interrupt
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_H15_GetStatusReg(UINT32 mcu_id,           \
                            VIDEO_IPC_TYPE_T ipcType,\
                            UINT32 *pStatus)
{
	int ret = RET_OK;

	switch (ipcType) {
		case VIDEO_IPC_MCU :
		case VIDEO_DMA_MCU :
		case VIDEO_JPG_MCU :
		case VIDEO_WEL_MCU :
		case VIDEO_WER_MCU :
			DE_IPC_H15_RdFL(int_intr_status);
			*pStatus = DE_IPC_H15_Rd(int_intr_status);
			break;
		case VIDEO_IPC_CPU :
		case VIDEO_DMA_CPU :
		case VIDEO_JPG_CPU :
		case VIDEO_WEL_CPU :
		case VIDEO_WER_CPU :
			DE_IPC_H15_RdFL(ext_intr_status);
			*pStatus = DE_IPC_H15_Rd(ext_intr_status);
			break;
		default :
			BREAK_WRONG(ipcType);
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make inerrupt status for watch dog either happen or clear
 *
 * @param turnOn [IN] determine to turn On or Off
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_WDG_H15_WakeUpReg(UINT32 mcu_id, BOOLEAN turnOn)
{
	if (turnOn) {
		DE_IPC_H15_FLWf(ext_intr_event, wdg_interrupt_event, 1);
	} else {
		DE_IPC_H15_FLWf(ext_intr_clear, wdg_interrupt_clear, 1);
	}

	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make interrupt be happen according to ipcType which is either of MCU or CPU
 *
 * @param ipcType [IN] one of either MCU and CPU
 * @param turnOn  [IN] maket Interrupt enable or clear
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_H15_WakeUpReg(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, BOOLEAN turnOn)
{
	int ret = RET_OK;

	turnOn &= 0x1;
	switch (ipcType) {
		case VIDEO_IPC_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_IPC_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_DMA_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_DMA_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_JPG_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_JPG_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_USB_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_USB_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_WEL_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_WEL_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_WER_MCU :
			if (turnOn) {
				DE_IPC_H15_FLWf(int_intr_event,\
						ipc_interrupt_event_mcu,\
						GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(int_intr_clear,\
						ipc_interrupt_clear_mcu,\
						GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			}
			break;
		case VIDEO_WER_CPU :
			if (turnOn) {
				DE_IPC_H15_FLWf(ext_intr_event,\
						ipc_interrupt_event_arm,\
						GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			} else {
				DE_IPC_H15_FLWf(ext_intr_clear,\
						ipc_interrupt_clear_arm,\
						GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			}
			break;
		default :
			BREAK_WRONG(ipcType);
	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief make interrupt be happen according to ipcType which is either of MCU or CPU
 *
 * @param ipcType [IN] one of either MCU and CPU
 * @param turnOn  [IN] maket Interrupt enable or clear
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IPC_H15_ClearStatus(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus)
{
	int ret = RET_OK;

	switch (ipcType) {
		case VIDEO_IPC_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_IPC_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_IPC_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_DMA_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_DMA_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_DMA_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_JPG_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_JPG_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_JPG_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_USB_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_USB_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_USB_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_WEL_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_WEL_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_WEL_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_WER_MCU :
			DE_IPC_H15_FLWf(int_intr_clear,\
					ipc_interrupt_clear_mcu,\
					GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			break;
		case VIDEO_WER_CPU :
			DE_IPC_H15_FLWf(ext_intr_clear,\
					ipc_interrupt_clear_arm,\
					GET_PMSK(VIDEO_WER_INTERRUPT_ARM_BIT, 1));
			break;
		default :
			BREAK_WRONG(ipcType);
			break;
	}
	return ret;
}
/**
 * @callgraph
 * @callergraph
 *
 * @brief Write Register
 *
 * @param addr [IN] accessing for register
 * @param value [IN] acccesing for register
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_WD(UINT32 addr, UINT32 value)
{
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;

	do {
		recvAddr = addr;
		if		(recvAddr <= 0x6000) recvAddr += DTVSOC_DE_H15_BASE;
		else if (recvAddr <= 0xffff) recvAddr += DTVSOC_DE_H15_BASE;
		currAddr = recvAddr;
		currAddr >>= 2;
		currAddr <<= 2;

		virtAddr = (UINT32)ioremap(currAddr, 0x8);
		if (currAddr == recvAddr) {
			REG_WD(virtAddr, value);
			break;
		}
		currValue = REG_RD(virtAddr);
		nextValue = REG_RD((virtAddr+4));

		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);

		currValue  = GET_BITS(currValue ,0	  ,dBit);
		currValue += GET_PVAL(value		,dBit ,wBit);

		nextValue  = GET_PVAL(nextValue ,dBit ,wBit);
		nextValue += GET_BITS(value		,0	  ,dBit);
		REG_WD(virtAddr		,currValue);
		REG_WD((virtAddr+4) ,nextValue);
	} while (0);
	if (virtAddr) iounmap((void *)virtAddr);

		return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Write Register
 *
 * @param addr [IN] accessing for register
 *
 * @return value of register
 */
UINT32 DE_REG_H15_RD(UINT32 addr)
{
	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;

	do {
		recvAddr = addr;
		if		(recvAddr <= 0x6000) recvAddr += DTVSOC_DE_H15_BASE;
		else if (recvAddr <= 0xffff) recvAddr += DTVSOC_DE_H15_BASE;

		currAddr = recvAddr;
		currAddr >>= 2;
		currAddr <<= 2;
		virtAddr = (UINT32)ioremap(currAddr, 0x8);

		value = REG_RD(virtAddr);
		if (currAddr == recvAddr) break;

		nextValue = REG_RD(virtAddr+4);
		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);
		value  = GET_BITS(value, dBit, wBit);
		value += GET_PVAL(nextValue, wBit, dBit);
	} while (0);
	if (virtAddr) iounmap((void *)virtAddr);

	return value;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set Frame rate of Display
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetFrRate(DE_DPLL_CLK_T dclk)
{
	int ret = RET_OK;
#ifdef USE_CTOP_CODES_FOR_H15
#ifdef USE_KDRV_CODES_FOR_H15
	DE_DPLL_DIV_T *pDisplayPll = NULL;
	int i;

	do {
		for (i=0;i<ARRAY_SIZE(sDisplayPll_H15);i++) {
			if (sDisplayPll_H15[i].clk != dclk) continue;
			pDisplayPll =  &sDisplayPll_H15[i].div;
			break;
		}
		CHECK_KNULL(pDisplayPll);
#if 1 //default  de = 148.5 MHz(?) , dpll = 297MHz(?)
		CTOP_CTRL_H15A0_RdFL(DPE, ctr42);
		CTOP_CTRL_H15A0_Wr01(DPE, ctr42, disp_nsc_ctrl, pDisplayPll->dpllM);
		CTOP_CTRL_H15A0_Wr01(DPE, ctr42, disp_npc_ctrl, pDisplayPll->dpllN);
		CTOP_CTRL_H15A0_WrFL(DPE, ctr42);
#endif
	} while (0);
#endif
#endif
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get Frame rate of Display
 *
 * @param pFrRate [OUT] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_GetFrRate(LX_DE_FR_RATE_T *pstParams)
{
	int ret = RET_OK;

	pstParams->isForceFreeRun = FALSE;
	pstParams->fr_rate = 30;

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set background color (non-active region color).
 *
 * @param [IN] structure pointer to carry infomation about non-active region color
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_SetBgColor(LX_DE_COLOR_T *pBackColor)
{
	int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
	DE_OVL_H15_RdFL(ovl_ov_ctrl2);

	pBackColor->r >>= 4;
	pBackColor->b >>= 4;
	DE_OVL_H15_Wr01(ovl_ov_ctrl2, back_color_cr, pBackColor->r);
	DE_OVL_H15_Wr01(ovl_ov_ctrl2, back_color_cb, pBackColor->b);
	DE_OVL_H15_Wr01(ovl_ov_ctrl2, back_color_yy, pBackColor->g);

	DE_OVL_H15_WrFL(ovl_ov_ctrl2);
#endif
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set blank color of selected window
 *
 * @param pWinBlank [IN] structure pointer to carry information about window Id, whether Turn On or Off and blank color
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetWinBlank(LX_DE_SET_WIN_BLANK_T *pWinBlank)
{
	int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
	do {
		DE_OVL_H15_RdFL(ovl_ov_ctrl4);
		switch (pWinBlank->win_id) {
			case LX_DE_WIN_MAIN :
				DE_OVL_H15_Wr01(ovl_ov_ctrl4, w0_dark_en, pWinBlank->bEnable);
				break;
			case LX_DE_WIN_SUB :
				DE_OVL_H15_Wr01(ovl_ov_ctrl4, w1_dark_en, pWinBlank->bEnable);
				break;
			default :
				BREAK_WRONG(pWinBlank->win_id);
		}
		if (ret) break;
		DE_OVL_H15_WrFL(ovl_ov_ctrl4);

		pWinBlank->win_color.r >>= 4;
		pWinBlank->win_color.b >>= 4;
		DE_OVR_H15_RdFL(ovr_ov_ctrl2);
		DE_OVR_H15_Wr01(ovr_ov_ctrl2, dark_color_cr, pWinBlank->win_color.r);
		DE_OVR_H15_Wr01(ovr_ov_ctrl2, dark_color_cb, pWinBlank->win_color.b);
		DE_OVR_H15_Wr01(ovr_ov_ctrl2, dark_color_yy, pWinBlank->win_color.g);
		DE_OVR_H15_WrFL(ovr_ov_ctrl2);
	} while (0);
#endif
	return ret;
}

BOOLEAN DE_REG_H15_CheckIrq4Vsync(UINT32 mcu_id)
{
	int ret;
	H15A0_INTR_CPU_FLAG_T		 intr_flag_cpu;
	H15A0_SUB_INTR_REG_T		 sub_intr_reg;
	UINT32 videoIntrDe;
	BOOLEAN vsyncIrq = 0;

	do {
		vsyncIrq = 0;
		ret = DE_REG_H15_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_FUNC, &videoIntrDe);
		if (ret) break;

		intr_flag_cpu  = *(H15A0_INTR_CPU_FLAG_T	*)&videoIntrDe;
		if (!intr_flag_cpu.sub_mcu_intr_cpu) break;
		ret = DE_REG_H15_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_SUB, &videoIntrDe);
		if (ret) break;

		sub_intr_reg = *(H15A0_SUB_INTR_REG_T *)&videoIntrDe;
		if (!sub_intr_reg.intr_i5_intr_pulse) break;
		vsyncIrq = 1;

	} while (0);
	return vsyncIrq;
}

int DE_REG_H15_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_T intrType, UINT32 *pVideoIrqStatus)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pVideoIrqStatus);
		switch (intrType) {
			case VIDEO_INTR_TYPE_CVI :
				break;
			case VIDEO_INTR_TYPE_IMX :
				break;
			case VIDEO_INTR_TYPE_ND1 :
				break;
			case VIDEO_INTR_TYPE_ND2 :
				break;
			case VIDEO_INTR_TYPE_GSC :
				break;
			case VIDEO_INTR_TYPE_VSD :
				break;
			case VIDEO_INTR_TYPE_CCO :
				break;
			case VIDEO_INTR_TYPE_SUB :
				DE_SUB_H15_RdFL(intr_reg);
				*pVideoIrqStatus = DE_SUB_H15_Rd(intr_reg);		
				DE_SUB_H15_Wr01(intr_reg, intr_i5_intr_pulse, 1);
				DE_SUB_H15_WrFL(intr_reg);
				break;
			case VIDEO_INTR_TYPE_FUNC :
				DE_IPC_H15_RdFL(intr_cpu_flag);
				*pVideoIrqStatus = DE_IPC_H15_Rd(intr_cpu_flag);
				break;
			default :
				BREAK_WRONG(intrType);
		}
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief copy frame buffer of certain block size and position.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetCviVideoFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams)
{
	int ret = RET_OK;
#ifdef USE_FRAME_COPY_TO_NFS_FILE
    OS_FILE_T   fw_file;
    char filePath[200] = "/home/jaemo.kim/res/lglib/kdrv/";
    char fileName[200];
#endif
	LX_DE_GRAB_COLOR_T *pFrameColor = NULL;
	UINT8  *pFrameAddrY8 = NULL;
	UINT8  *pFrameAddrC8 = NULL;
	UINT8  *pFrameAddrY2 = NULL;
	UINT8  *pFrameAddrC2 = NULL;
	UINT16 phyFrameAddrY8;
	UINT16 phyFrameAddrC8;
	UINT16 phyFrameAddrY2;
	UINT16 phyFrameAddrC2;

	UINT32 grabX;
	UINT32 grabY;
	UINT32 grabW;
	UINT32 grabH;
	UINT32 frameW;
	UINT32 frameH;
	UINT32 offsetY;
	UINT32 offsetC;
	UINT32 startY;
	UINT32 pixelG8;
	UINT32 pixelB8;
	UINT32 pixelR8;
	UINT32 pixelG2;
	UINT32 pixelB2;
	UINT32 pixelR2;
	UINT8  pixelY2;
	UINT8  pixelC2;
	UINT32 x;
	UINT32 y;
	UINT32 posY;
	UINT32 posC;
	UINT32 divY;
	UINT32 remY;
	UINT32 divC;
	UINT32 remC;
	UINT32 framSize;
	UINT32 grabSize;
	UINT32 grabStepW;
	UINT32 bankPrewY;
	UINT32 bankPrewC;
	UINT32 bankOffsetY;
	UINT32 bankOffsetC;
	UINT32 grabBaseAddr;
	UINT32 smuxSample;
	UINT8 colorDepth;
	UINT32 phyAddrY8;
	UINT32 phyAddrC8;
	UINT32 phyAddrY2;
	UINT32 phyAddrC2;

	do {
		CHECK_KNULL(pstParams);

		phyFrameAddrY8 = GET_BITS(pstParams->region.realPixelGrabW ,16 ,16);
		phyFrameAddrY2 = GET_BITS(pstParams->region.realPixelGrabW ,0  ,16);
		phyFrameAddrC8 = GET_BITS(pstParams->region.realPixelGrabH ,16 ,16);
		phyFrameAddrC2 = GET_BITS(pstParams->region.realPixelGrabH ,0  ,16);

		grabW		   = GET_BITS(pstParams->region.pixelGrabW	   ,16 ,16);
		grabH		   = GET_BITS(pstParams->region.pixelGrabW	   ,0  ,16);
		grabX		   = GET_BITS(pstParams->region.pixelGrabH	   ,16 ,16);
		grabY		   = GET_BITS(pstParams->region.pixelGrabH	   ,0  ,16);
		frameW		   = GET_BITS(pstParams->region.pixelGrabX	   ,16 ,16);
		frameH		   = GET_BITS(pstParams->region.pixelGrabX	   ,0  ,16);

		smuxSample	   = GET_BITS(pstParams->region.pixelGrabY	   ,12 ,4);
		bankPrewY	   = GET_BITS(pstParams->region.colorSpace	   ,4  ,4);
		bankPrewC	   = GET_BITS(pstParams->region.colorSpace	   ,0  ,4);
		grabStepW	   = GET_BITS(pstParams->bReadOnOff			   ,16 ,16);
		colorDepth	   = GET_BITS(pstParams->region.colorDepth 	   ,0  ,4);

		grabSize = grabW  * grabH;
		framSize = frameW * frameH;
		if (!grabSize) break;
		if (!framSize) break;

		grabBaseAddr = GET_SVAL(pstParams->region.colorDepth,4,4,28);
		bankOffsetY  = bankPrewY * PIXEL_PER_BANK;
		bankOffsetC  = bankPrewC * PIXEL_PER_BANK;
		phyAddrY8 = (CONV_MEM_ROW2BYTE(phyFrameAddrY8) + bankOffsetY) | grabBaseAddr;
		phyAddrY2 = (CONV_MEM_ROW2BYTE(phyFrameAddrY2) + bankOffsetY) | grabBaseAddr;
		phyAddrC8 = (CONV_MEM_ROW2BYTE(phyFrameAddrC8) + bankOffsetC) | grabBaseAddr;
		phyAddrC2 = (CONV_MEM_ROW2BYTE(phyFrameAddrC2) + bankOffsetC) | grabBaseAddr;

		pFrameAddrY8 = (UINT8 *)ioremap(phyAddrY8, framSize);
		pFrameAddrY2 = (UINT8 *)ioremap(phyAddrY2, framSize/4);
		pFrameAddrC8 = (UINT8 *)ioremap(phyAddrC8, framSize*smuxSample);
		pFrameAddrC2 = (UINT8 *)ioremap(phyAddrC2, framSize/4*smuxSample);

#ifdef USE_FRAME_COPY_TO_NFS_FILE
		do {
			sprintf(fileName, "%sVideo_Prew__%dx%d.Y.img",filePath, frameW, frameH);
			DE_PRINT("Writing %s\n", fileName);
			if ( RET_OK != OS_OpenFile( &fw_file, fileName, O_CREAT|O_RDWR|O_LARGEFILE, 0666 ) )
			{
				DE_PRINT("<error> can't open fw_file (%s)\n", fileName);
				if (ret) BREAK_WRONG(ret);
			}
			ret = OS_WriteFile(&fw_file, (char *)pFrameAddrY8, framSize);
			if (ret != framSize) BREAK_WRONG(ret);
			ret = OS_CloseFile( &fw_file );
			if (ret) BREAK_WRONG(ret);
			DE_PRINT("Done %s\n", fileName);

			sprintf(fileName, "%sVideo_Prew__%dx%d.C.img",filePath, frameW, frameH);
			DE_PRINT("Writing %s\n", fileName);
			if ( RET_OK != OS_OpenFile( &fw_file, fileName, O_CREAT|O_RDWR|O_LARGEFILE, 0666 ) )
			{
				DE_PRINT("<error> can't open fw_file (%s)\n", fileName);
				BREAK_WRONG(ret);
			}
			ret = OS_WriteFile(&fw_file, (char *)pFrameAddrC8, framSize*smuxSample);
			if (ret != framSize*smuxSample) BREAK_WRONG(ret);
			ret = OS_CloseFile( &fw_file );
			if (ret) BREAK_WRONG(ret);
			DE_PRINT("Done %s\n", fileName);
		} while (0);
		if (ret) BREAK_WRONG(ret);
#endif
		grabSize = 0;
		offsetY  = 0;
		offsetC  = 0;
		for (y=0;y<grabH;y++) {
			startY = (grabY + y) * frameW + grabX;
			grabSize = y * grabStepW;
			for (x=0;x<grabW;x++) {
				pFrameColor = &pstParams->color[grabSize+x];
				posY  = offsetY + (startY + x);
				posC  = offsetC + (startY + x) * smuxSample;
				posC &= GET_RMSK(0,1);
				if (GET_BITS(pstParams->bReadOnOff,0,1)) {
					pixelG8 = pFrameAddrY8[posY+0];
					pixelB8 = pFrameAddrC8[posC+0];
					pixelR8 = pFrameAddrC8[posC+1];
					pFrameColor->pixelGrabY  = GET_SVAL(pixelG8, 0,8,2);
					pFrameColor->pixelGrabCb = GET_SVAL(pixelB8, 0,8,2);
					pFrameColor->pixelGrabCr = GET_SVAL(pixelR8, 0,8,2);
				} else {
					pixelG8 = GET_BITS(pFrameColor->pixelGrabY	,2,8);
					pixelB8 = GET_BITS(pFrameColor->pixelGrabCb ,2,8);
					pixelR8 = GET_BITS(pFrameColor->pixelGrabCr ,2,8);
					pFrameAddrY8[posY+0] = pixelG8;
					pFrameAddrC8[posC+0] = pixelB8;
					pFrameAddrC8[posC+1] = pixelR8;
				}

				if (!colorDepth) continue;
				divY = posY/4;
				remY = posY%4;
				divC = posC/4;
				remC = posC%4;

				if (GET_BITS(pstParams->bReadOnOff,0,1)) {
					pixelY2 = pFrameAddrY2[divY];
					pixelC2 = pFrameAddrC2[divC];
					pixelG2 = GET_BITS(pixelY2, (remY+0)*2, 2);
					pixelB2 = GET_BITS(pixelC2, (remC+0)*2, 2);
					pixelR2 = GET_BITS(pixelC2, (remC+1)*2, 2);

					pFrameColor->pixelGrabY  |= GET_SVAL(pixelG2, 0,2,0);
					pFrameColor->pixelGrabCb |= GET_SVAL(pixelB2, 0,2,0);
					pFrameColor->pixelGrabCr |= GET_SVAL(pixelR2, 0,2,0);
				} else {
					pixelG2 = GET_BITS(pFrameColor->pixelGrabY	,0,2);
					pixelB2 = GET_BITS(pFrameColor->pixelGrabCb ,0,2);
					pixelR2 = GET_BITS(pFrameColor->pixelGrabCr ,0,2);

					pixelY2 = pFrameAddrY2[divY];
					pixelC2 = pFrameAddrC2[divC];
					pixelY2 |= GET_SVAL(pixelG2, 0, 2, (remY+0)*2);
					pixelC2 |= GET_SVAL(pixelB2, 0, 2, (remC+0)*2);
					pixelC2 |= GET_SVAL(pixelR2, 0, 2, (remC+1)*2);
					pFrameAddrY2[divY] = pixelY2;
					pFrameAddrC2[divC] = pixelC2;
				}
			}
		}
		pstParams->region.realPixelGrabW = grabW;
		pstParams->region.realPixelGrabH = grabH;
		pstParams->region.colorSpace	 = 0;
	} while (0);

	if(pFrameAddrY8) iounmap(pFrameAddrY8);
	if(pFrameAddrC8) iounmap(pFrameAddrC8);
	if(pFrameAddrY2) iounmap(pFrameAddrY2);
	if(pFrameAddrC2) iounmap(pFrameAddrC2);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief copy frame buffer of certain block size and position.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetCviRWFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams,\
									LX_DE_CVI_RW_PREW_FRAME_T *prewInfo)
{
	int ret = RET_OK;
	LX_DE_GRAB_COLOR_T *pFrameColor = NULL;
	UINT8  *pFrameAddrY8 = NULL;
	UINT8  *pFrameAddrC8 = NULL;
	UINT8  *pFrameAddrY2 = NULL;
	UINT8  *pFrameAddrC2 = NULL;

	UINT32 grabX;
	UINT32 grabY;
	UINT32 grabW;
	UINT32 grabH;
	UINT32 frameW;
	UINT32 frameH;
	UINT32 offsetY;
	UINT32 offsetC;
	UINT32 startY;
	UINT32 pixelG8;
	UINT32 pixelB8;
	UINT32 pixelR8;
	UINT32 pixelG2;
	UINT32 pixelB2;
	UINT32 pixelR2;
	UINT8  pixelY2;
	UINT8  pixelC2;
	UINT32 x;
	UINT32 y;
	UINT32 posY;
	UINT32 posC;
	UINT32 divY;
	UINT32 remY;
	UINT32 divC;
	UINT32 remC;
	UINT32 framSize;
	UINT32 grabSize;
	UINT32 grabStepW;
	UINT32 smuxSample;
	UINT8 colorDepth;
	UINT32 phyAddrY8;
	UINT32 phyAddrC8;
	UINT32 phyAddrY2;
	UINT32 phyAddrC2;
	UINT32 stride;

	do {
		CHECK_KNULL(pstParams);
		CHECK_KNULL(prewInfo);


		phyAddrY8 = prewInfo->frameAddr.phyFrameAddrY8;
		phyAddrY2 = prewInfo->frameAddr.phyFrameAddrY2;
		phyAddrC8 = prewInfo->frameAddr.phyFrameAddrC8;
		phyAddrC2 = prewInfo->frameAddr.phyFrameAddrC2;
		stride    = prewInfo->frameAddr.stride;
		smuxSample= (UINT32)prewInfo->csample;
		frameW	  = prewInfo->rect.w;
		frameH	  = prewInfo->rect.h;

		if(frameH > (1080+200)) // 4k
		{
			grabX     = pstParams->region.pixelGrabX >> 1;
			DE_TRACE("4k input !!!\n");
		}
		else                    // 2k
			grabX     = pstParams->region.pixelGrabX;
		grabY     = pstParams->region.pixelGrabY;
		grabW     = pstParams->region.pixelGrabW;
		grabH     = pstParams->region.pixelGrabH;
		colorDepth= pstParams->region.colorDepth;

		DE_TRACE("## Command info.\n");
		DE_TRACE("readOnOff[%d]\n", pstParams->bReadOnOff);
		DE_TRACE("win[%d] x/y/w/h %d/%d/%d/%d\n",\
				pstParams->region.win_id, \
				pstParams->region.pixelGrabX,\
				pstParams->region.pixelGrabY,\
				pstParams->region.pixelGrabW,\
				pstParams->region.pixelGrabH);
		DE_TRACE("color depth(8/10)[%d],space(yuv/rgb)[%d]\n",\
				pstParams->region.colorDepth,\
				pstParams->region.colorSpace);
		DE_TRACE("## PREW info.\n");
		DE_TRACE("stride %d, color(420/422/444) [%d]\n",\
				prewInfo->frameAddr.stride,\
				prewInfo->csample);
		DE_TRACE("rect w/h [%d/%d]\n", prewInfo->rect.w, prewInfo->rect.h);
		DE_TRACE("frame address Y8/Y2[0x%08x/0x%08x] C8/C2[0x%08x/0x%08x]\n",\
				prewInfo->frameAddr.phyFrameAddrY8,\
				prewInfo->frameAddr.phyFrameAddrY2,\
				prewInfo->frameAddr.phyFrameAddrC8,\
				prewInfo->frameAddr.phyFrameAddrC2);
		
		framSize  = frameW * frameH;
		if(framSize > 3840*2160*2)
		{
			DE_WARN("Invalid cvi frame size !!! W/H(%d/%d)\n",\
				frameW, frameH);
			break;
		}

		if(smuxSample == 0) smuxSample = 1;

		pFrameAddrY8 = (UINT8 *)ioremap(phyAddrY8, framSize);
		pFrameAddrY2 = (UINT8 *)ioremap(phyAddrY2, framSize/4);
		pFrameAddrC8 = (UINT8 *)ioremap(phyAddrC8, framSize*smuxSample);
		pFrameAddrC2 = (UINT8 *)ioremap(phyAddrC2, framSize/4*smuxSample);

		offsetY  = 0;
		offsetC  = 0;
		grabStepW = grabW;

		DE_TRACE("X/Y/W/H(%d/%d/%d/%d)\n", grabX,grabY,grabW,grabH);
		DE_TRACE("frameW[%d]frameH[%d]grabStepW[%d]smuxSample[%d]colorDepth[%d]\n", \
				frameW, frameH, grabStepW, smuxSample, colorDepth);

		if(((grabX + grabW) >= frameW) || ((grabY + grabH) >= frameH))
		{
			DE_WARN("Invalid grab position !!! X/Y/W/H(%d/%d/%d/%d)\n",\
				grabX,grabY,grabW,grabH);
			break;
		}

		for(y=0;y<grabH;y++)
		{
//			DE_TRACE("Y[%d]\n", y);
			startY = (grabY + y) * stride + grabX;
			grabSize = y * grabStepW;
			for(x=0;x<grabW;x++)
			{
//				DE_TRACE("X[%d] ", x);
				pFrameColor = &pstParams->color[grabSize+x];
				posY  = offsetY + (startY + x);
				posC  = offsetC + (startY + x) * smuxSample;
				posC &= GET_RMSK(0,1);
				if(pstParams->bReadOnOff) {
					pixelG8 = pFrameAddrY8[posY+0];
					pixelB8 = pFrameAddrC8[posC+0];
					pixelR8 = pFrameAddrC8[posC+1];
//					DE_TRACE("GBR8[%d/%d/%d]\n", pixelG8, pixelB8, pixelR8);
					pFrameColor->pixelGrabY  = GET_SVAL(pixelG8, 0,8,2);
					pFrameColor->pixelGrabCb = GET_SVAL(pixelB8, 0,8,2);
					pFrameColor->pixelGrabCr = GET_SVAL(pixelR8, 0,8,2);
				} else {
					pixelG8 = GET_BITS(pFrameColor->pixelGrabY	,2,8);
					pixelB8 = GET_BITS(pFrameColor->pixelGrabCb ,2,8);
					pixelR8 = GET_BITS(pFrameColor->pixelGrabCr ,2,8);
					pFrameAddrY8[posY+0] = pixelG8;
					pFrameAddrC8[posC+0] = pixelB8;
					pFrameAddrC8[posC+1] = pixelR8;
				}

//				DE_TRACE("startY/posY/posC[%d/%d/%d]\n", startY, posY, posC);

				if (!colorDepth) continue;
				divY = posY/4;
				remY = posY%4;
				divC = posC/4;
				remC = posC%4;
#if 0
				DE_TRACE("divY/remY/divC/remC [%d/%d/%d/%d]\n",\
						divY,remY,divC,remC);
#endif
				if (pstParams->bReadOnOff) {
					pixelY2 = pFrameAddrY2[divY];
					pixelC2 = pFrameAddrC2[divC];
					pixelG2 = GET_BITS(pixelY2, (remY+0)*2, 2);
					pixelB2 = GET_BITS(pixelC2, (remC+0)*2, 2);
					pixelR2 = GET_BITS(pixelC2, (remC+1)*2, 2);
//					DE_TRACE("GBR2[%d/%d/%d]\n", pixelG2, pixelB2, pixelR2);

					pFrameColor->pixelGrabY  |= GET_SVAL(pixelG2, 0,2,0);
					pFrameColor->pixelGrabCb |= GET_SVAL(pixelB2, 0,2,0);
					pFrameColor->pixelGrabCr |= GET_SVAL(pixelR2, 0,2,0);
#if 0
					DE_TRACE("YCbCr[%d/%d/%d]\n", pFrameColor->pixelGrabY,\
							pFrameColor->pixelGrabCb, \
							pFrameColor->pixelGrabCr);
#endif
				} else {
					pixelG2 = GET_BITS(pFrameColor->pixelGrabY	,0,2);
					pixelB2 = GET_BITS(pFrameColor->pixelGrabCb ,0,2);
					pixelR2 = GET_BITS(pFrameColor->pixelGrabCr ,0,2);

					pixelY2 = pFrameAddrY2[divY];
					pixelC2 = pFrameAddrC2[divC];
					pixelY2 |= GET_SVAL(pixelG2, 0, 2, (remY+0)*2);
					pixelC2 |= GET_SVAL(pixelB2, 0, 2, (remC+0)*2);
					pixelC2 |= GET_SVAL(pixelR2, 0, 2, (remC+1)*2);
					pFrameAddrY2[divY] = pixelY2;
					pFrameAddrC2[divC] = pixelC2;
				}
			}
		}
		pstParams->region.realPixelGrabW = grabW;
		pstParams->region.realPixelGrabH = grabH;
		pstParams->region.colorSpace	 = 0;
	} while (0);

	if(pFrameAddrY8) iounmap(pFrameAddrY8);
	if(pFrameAddrC8) iounmap(pFrameAddrC8);
	if(pFrameAddrY2) iounmap(pFrameAddrY2);
	if(pFrameAddrC2) iounmap(pFrameAddrC2);

	return ret;
}


/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_CviMap(LX_DE_WIN_ID_T win_id, \
		LX_DE_IN_SRC_T in_port, \
		UINT32 port_num)
{
	int ret = RET_OK;
	UINT32 sel_cvi[2] = {0, 0};
	UINT32 sel_clk[2] = {0, 0};

	do {
		sel_cvi[0] = CVI_NONE;
		sel_cvi[1] = CVI_NONE;
		sel_clk[0] = CLK_NONE;
		sel_clk[1] = CLK_NONE;

		switch(win_id)
		{
			case LX_DE_WIN_MAIN:
			case LX_DE_WIN_1:
			case LX_DE_WIN_2:
			case LX_DE_WIN_3:
				if(in_port == LX_DE_IN_SRC_CVBS || \
						in_port == LX_DE_IN_SRC_ATV || \
						in_port == LX_DE_IN_SRC_SCART)
				{
					sel_cvi[0] = CVI_CVD;
					sel_clk[0] = CLK_CVD;
				}
				if(in_port == LX_DE_IN_SRC_YPBPR || \
						in_port == LX_DE_IN_SRC_VGA)
				{
					sel_cvi[0] = CVI_ADC;
					sel_clk[0] = CLK_ADC;
				}
				if(in_port == LX_DE_IN_SRC_HDMI)
				{
					switch(g_HdmiPort)
					{
						case HDMI_PORT_0:
							sel_cvi[0] = CVI_HDMI_A_L;
							sel_cvi[1] = CVI_HDMI_A_R;
							sel_clk[0] = CLK_HDA;
							sel_clk[1] = CLK_HDA;
							break;
						case HDMI_PORT_1:
							sel_cvi[0] = CVI_HDMI_B_L;
							sel_cvi[1] = CVI_HDMI_B_R;
							sel_clk[0] = CLK_HDB;
							sel_clk[1] = CLK_HDB;
							break;
						case HDMI_PORT_2:
							sel_cvi[0] = CVI_HDMI_C_L;
							sel_cvi[1] = CVI_HDMI_C_R;
							sel_clk[0] = CLK_HDC;
							sel_clk[1] = CLK_HDC;
							break;
						case HDMI_PORT_3:
							sel_cvi[0] = CVI_HDMI_D_L;
							sel_cvi[1] = CVI_HDMI_D_R;
							sel_clk[0] = CLK_HDD;
							sel_clk[1] = CLK_HDD;
							break;
					}
				}
				break;
			default:
				break;
		}

		if(in_port == LX_DE_IN_SRC_NONE) break;
		
		DE_CVI_H15_RdFL(cv0_top_ctrl);
		DE_CVI_H15_RdFL(cv1_top_ctrl);
		DE_CVI_H15_RdFL(cv2_top_ctrl);
		DE_CVI_H15_RdFL(cv3_top_ctrl);
		CTOP_CTRL_H15A0_RdFL(CVI, ctr03);
		switch(win_id)
		{
			case LX_DE_WIN_MAIN:
				DE_CVI_H15_Wr01(cv0_top_ctrl, data_in_sel, sel_cvi[0]);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi0_sel, sel_clk[0]);
				if(sel_cvi[1] != CVI_NONE)
				{
					DE_CVI_H15_Wr01(cv1_top_ctrl, data_in_sel, sel_cvi[1]);
					CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi1_sel, sel_clk[1]);
				}
				break;
			case LX_DE_WIN_1:
				DE_CVI_H15_Wr01(cv1_top_ctrl, data_in_sel, sel_cvi[0]);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi1_sel, sel_clk[0]);
				break;
			case LX_DE_WIN_2:
				DE_CVI_H15_Wr01(cv2_top_ctrl, data_in_sel, sel_cvi[0]);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi2_sel, sel_clk[0]);
				#if 1
				if(sel_cvi[1] != CVI_NONE)
				{
					DE_CVI_H15_Wr01(cv3_top_ctrl, data_in_sel, sel_cvi[1]);
					CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi3_sel, sel_clk[1]);
				}
				#endif
				break;
			case LX_DE_WIN_3:
				DE_CVI_H15_Wr01(cv3_top_ctrl, data_in_sel, sel_cvi[0]);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr03, cvi3_sel, sel_clk[0]);
				break;
			default : 
				break;
		}
#if 0 // 20141007 Do not update these cvi registers.... (firmware control)
		DE_CVI_H15_WrFL(cv0_top_ctrl);
		DE_CVI_H15_WrFL(cv1_top_ctrl);
		DE_CVI_H15_WrFL(cv2_top_ctrl);
		DE_CVI_H15_WrFL(cv3_top_ctrl);
#endif
		CTOP_CTRL_H15A0_WrFL(CVI, ctr03);
	} while(0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetWinPortSrc(LX_DE_INPUT_CFG_T *pstParams)
{
	int ret = RET_OK;
	LX_DE_WIN_ID_T win_id = LX_DE_WIN_MAIN;
	LX_DE_IN_SRC_T in_port = LX_DE_IN_SRC_MVI;
	UINT32 srcPort = 0;

	do {
		CHECK_KNULL(pstParams);

		win_id  = pstParams->win_id;
		in_port = pstParams->inputSrc;
		srcPort = pstParams->inputSrcPort;
		
		ret = DE_REG_H15_CviMap(win_id, in_port, srcPort);

		DE_PRINT("Src map (NONE(0)/VGA(1)/YPBPR(2)/ATV(3)/CVD(4)/SCART(5)/HDMI(6)/MVI(7)/CPU(8))");
		DE_PRINT("  => win[%d] src[%d] port[%d]", win_id, in_port, srcPort);
	} while(0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set information which is comming from CVI port
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetCviSrcType(LX_DE_CVI_SRC_TYPE_T *pstParams)
{
	int ret = RET_OK;
	UINT32 hdmi_half_clk = 0;

	do {
		CHECK_KNULL(pstParams);
		memcpy(&sCviSrcType, pstParams, sizeof(LX_DE_CVI_SRC_TYPE_T));

		if(pstParams->size_offset.hsize > 2440 && pstParams->size_offset.vsize > 1400)
		{
			switch(pstParams->c_sample)
			{
				case LX_DE_CVI_420:	
					hdmi_half_clk = 0;
					break;
				case LX_DE_CVI_422:
				case LX_DE_CVI_444:
				default:
					hdmi_half_clk = 1;
					break;
			}
		}
		else
		{
			hdmi_half_clk = 0;
		}

		switch(g_HdmiPort)
		{
			case 0:
				CTOP_CTRL_H15A0_RdFL(CVI, ctr00);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr00, hdmi_half_sel, hdmi_half_clk);
				CTOP_CTRL_H15A0_WrFL(CVI, ctr00);
				break;
			case 1:
				CTOP_CTRL_H15A0_RdFL(CVI, ctr01);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr01, hdmi_half_sel_1, hdmi_half_clk);
				CTOP_CTRL_H15A0_WrFL(CVI, ctr01);
				break;
			case 2:
				CTOP_CTRL_H15A0_RdFL(CVI, ctr01);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr01, hdmi_half_sel_2, hdmi_half_clk);
				CTOP_CTRL_H15A0_WrFL(CVI, ctr01);
				break;
			case 3:
				CTOP_CTRL_H15A0_RdFL(CVI, ctr01);
				CTOP_CTRL_H15A0_Wr01(CVI, ctr01, hdmi_half_sel_3, hdmi_half_clk);
				CTOP_CTRL_H15A0_WrFL(CVI, ctr01);
				break;
		}
#if 0
		switch (pstParams->sampling) {
			case LX_DE_CVI_NORMAL_SAMPLING :
				cviSampling  = 0;
				xsize_divide = 0;
				break;
			case LX_DE_CVI_DOUBLE_SAMPLING :
				cviSampling  = 1;
				xsize_divide = 1;
				break;
			case LX_DE_CVI_QUAD_SAMPLING :
				cviSampling  = 3;
				xsize_divide = 2;
				break;
			default :
				BREAK_WRONG(pstParams->sampling);
		}
		if (ret) break;

		switch (pstParams->cvi_channel) {
			case LX_DE_CVI_CH_A :
#ifdef USE_CTOP_CODES_FOR_H13
				CTOP_CTRL_H15A0_RdFL(IMX, ctr00);
				CTOP_CTRL_H15A0_Wr01(IMX, ctr00, core_sel, xsize_divide);
				CTOP_CTRL_H15A0_WrFL(IMX, ctr00);
#endif
				break;
			case LX_DE_CVI_CH_B :
#ifdef USE_CTOP_CODES_FOR_H13
				CTOP_CTRL_H15A0_RdFL(SUB, ctr00);
				CTOP_CTRL_H15A0_Wr01(SUB, ctr00, core_sel, xsize_divide);
				CTOP_CTRL_H15A0_WrFL(SUB, ctr00);
#endif
				break;
			default :
				BREAK_WRONG(pstParams->cvi_channel);
		}
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set Frame rate of Display
 *
 * @param fr_rate [IN] value of Frame rate of Display
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_Init(LX_DE_PANEL_TYPE_T *pstParams)
{
	DE_REG_H15_InitAddrSwitch();

	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set de VCS parameter.
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetVcs(LX_DE_VCS_IPC_T *pstParams)
{
	int ret = RET_OK;
#ifndef _H15_FPGA_TEMPORAL_
	int channel;
	int inx;

	do {
		CHECK_KNULL(pstParams);
		channel = GET_BITS(pstParams->inx, 31,	1);
		inx		= GET_BITS(pstParams->inx,	0, 30);
		if (inx > SIZE_OF_IPC_FOR_CPU ) BREAK_WRONG(inx);
		spVideoIPCofCPU[channel][inx] = pstParams->data;
	} while (0);
#endif
	return ret;
}

int DE_REG_H15_GPIO_Init(void)
{
#ifdef	USE_CTOP_CODES_FOR_H15
#ifdef USE_VIDEO_UART2_FOR_MCU
	CTOP_CTRL_H15A0_RdFL(ND0, ctr33);
	CTOP_CTRL_H15A0_RdFL(CPU, ctr39);
	CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 2); // UART0 = cpu0
	CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart1_sel, 11); // UART1 = de
	CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart2_sel, 7); // UART2 = cpu1
	CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_mcu, 1); // 1:DE from UART1
	CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 5); // jtag1 = de
	CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
	CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
#endif
#endif
	return 0;
}

int DE_REG_H15_HDMI_Init(void)
{
	return 0;
}

int DE_REG_H15_LVDS_Init(void)
{
	return 0;
}

int DE_REG_H15_MISC_Init(void)
{
	return 0;
}

int DE_REG_H15_OSD_Init(void)
{

	return 0;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set uart for MCU or CPU
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_Uart0_Switch(int whichUart)
{
	int ret = RET_OK;

#ifdef	USE_CTOP_CODES_FOR_H15
#ifdef USE_DE_SWITCH_UART
	CTOP_CTRL_H15A0_RdFL(ND0, ctr33);
	CTOP_CTRL_H15A0_RdFL(CPU, ctr39);
	switch (whichUart)
	{
		case 0 :
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 2); // UART0 = cpu0
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart1_sel, 11); // UART1 = de
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_mcu, 1); // 1:DE from UART1
			break;
		case 1 :
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 11); // UART0 = de
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart1_sel, 2); // UART1 = cpu0
			CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_mcu, 0); // 0:DE from UART0
			break;
		case 0x80 :
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 2); // jtag1 = aud (default)
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 0); // jtag0 = cpu
			DE_PRINT("JTAG-1=aud\n");
			break;
		case 0x81 :
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 5); // jtag1 = de
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 0); // jtag0 = cpu
			DE_PRINT("JTAG-1=de\n");
			break;
		case 0x82 :
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 0); // jtag0 = cpu
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 5); // jtag1 = de
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, mcu_jtag, 0); // jtag1 = de
			DE_PRINT("JTAG-0=cpu\n");
			break;
		case 0x83 :
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 5); // jtag0 = de
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 1); // jtag1 = cpu
			CTOP_CTRL_H15A0_Wr01(CPU, ctr39, mcu_jtag, 1); // jtag0 = de
			DE_PRINT("JTAG-0=de\n");
			break;
		default :
			BREAK_WRONG(whichUart);
			break;
	}
	CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
	CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
#endif
#endif
	return ret;
}

#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_REG_H15_FW_Download(LX_DE_FW_DWLD_T *pstParams)
{
	int ret = RET_OK;
	char						*lpAddr = NULL;
	LX_DE_CH_MEM_T				*pFwMem = NULL;
	BOOLEAN						run_stall = 0;
#if !defined(USE_DE_FIRMWARE_RUN_IN_PAK_H15)
	BOOLEAN						tx_done = 0;
	UINT32						edma_ctrl;
#endif
	UINT32						fwBaseAddr;
	UINT32 						fwSize;
	UINT32 						*pPreWBase;
	UINT32 						*pTagBase;
	LX_DE_IPC_SYSTEM_T          tag_info;
#define SYSTEM_MAGIC_WORD           0xDECAFE

	do {
		DE_NOTI("start download\n");
		CHECK_KNULL(pstParams);
		if (!pstParams->size) break;
		if (!pstParams->pData) break;
		DE_IPC_H15_RdFL(set_ctrl0);
		DE_IPC_H15_Rd01(set_ctrl0, run_stall, run_stall);

// do not check run_stall status for re-download
		//if (!run_stall) break;
#if 0
		CTOP_CTRL_H15_RdFL(ctr25);
		CTOP_CTRL_H15_Wr01(ctr25, swrst_dee_de_dp, 1);
		CTOP_CTRL_H15_WrFL(ctr25);
		CTOP_CTRL_H15_Wr01(ctr25, swrst_dee_de_dp, 0);
		CTOP_CTRL_H15_WrFL(ctr25);
#endif
		DE_IPC_H15_Wr01(set_ctrl0, run_stall, 1);
		DE_IPC_H15_Wr01(set_ctrl0, mcu_sw_reset, 0);
		DE_IPC_H15_WrFL(set_ctrl0);

#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_LBUS
		pFwMem = &gMemCfgDeFW[4];         // firmware on L-BUS , temporal configuration for B0 evaluation
#else
		pFwMem = (LX_DE_CH_MEM_T *)&gpDeMem->fw[0];
#endif
		CHECK_KNULL(pFwMem);

#ifdef USE_VIDEO_FOR_FPGA
		tag_info.system_info   = (SYSTEM_MAGIC_WORD << 8) | 2/*1:ASIC,2:FPGA*/;
#else
		tag_info.system_info   = (SYSTEM_MAGIC_WORD << 8) | 1/*1:ASIC,2:FPGA*/;
#endif
		tag_info.firmware_base = pFwMem->fw_base;
		tag_info.de_frame_base = gpDeMem->frame_base;
		tag_info.de_grab_base  = (gpAdrGrap->fw_base + 0x7FFC) & 0xFFFF8000; // for Row align (32KB)
		tag_info.de_prew_base  = gpAdrPreW->fw_base;
#ifdef INCLUDE_KDRV_BE
		tag_info.be_frame_base = gpBeMem->fw[0].fw_base;
#else
		tag_info.be_frame_base = 0;
#endif

		// tagging prew & grab buffer
		pPreWBase = (UINT32 *)ioremap((gpDeMem->frame_base-DE_DDR_FIRMWARE_TAG_COUNT*sizeof(UINT32)), DE_DDR_FIRMWARE_TAG_COUNT*sizeof(UINT32));
		if(pPreWBase)
		{
			pPreWBase[0] = tag_info.de_grab_base;
			pPreWBase[1] = tag_info.de_prew_base;
			DE_NOTI("Tag to FW (Grab & Prew base) -  0:0x%08X , 1:0x%08X (at 0x%08x)\n", \
					pPreWBase[0], pPreWBase[1], \
					(gpDeMem->frame_base-DE_DDR_FIRMWARE_TAG_COUNT*sizeof(UINT32)));
			iounmap((void*)pPreWBase);
		}

		// tagging magic , fw base and frame base
		pTagBase = (UINT32*)ioremap(DE_IPC_FRM_H15_BASE, DTVSOC_IPC_FROM_CPU_SIZE);
		if(pTagBase)
		{
			pTagBase[0] = tag_info.system_info;
			pTagBase[1] = tag_info.firmware_base;
			pTagBase[2] = tag_info.de_frame_base;
			pTagBase[3] = tag_info.de_grab_base;
			pTagBase[4] = tag_info.de_prew_base;
			pTagBase[5] = tag_info.be_frame_base;

			DE_NOTI("Tag to FW (fw & frame base) -  0:0x%08X , 1:0x%08X , 2:0x%08X (at 0x%08x)\n", \
					pTagBase[0], pTagBase[1], pTagBase[2], DE_IPC_FRM_H15_BASE);
			DE_NOTI("         grab/prew/be_frame -  3:0x%08x , 4:0x%08x , 5:0x%08x\n",\
					pTagBase[3], pTagBase[4], pTagBase[5]);
			iounmap((void*)pTagBase);
		}

		fwBaseAddr = pFwMem->fw_base;
		fwSize     = pFwMem->fw_size;
		if (fwSize < pstParams->size) fwSize = pstParams->size;
		fwSize  = GET_RDUP(fwSize, 4);
#if defined(USE_VIDEO_MCU_RUN_IN_DDR0) && (USE_VIDEO_MCU_RUN_IN_DDR0 > USE_VIDEO_MCU_ROM_BASE_ADDR)
		lpAddr	= (char *)ioremap((USE_VIDEO_MCU_RUN_IN_DDR0+USE_VIDEO_MCU_ROM_FW_OFFSET), pstParams->size);
#else
		lpAddr	= (char *)ioremap((fwBaseAddr+USE_VIDEO_MCU_ROM_FW_OFFSET), pstParams->size);
#endif
		CHECK_KNULL(lpAddr);
		memcpy(lpAddr, pstParams->pData, pstParams->size);
		if (lpAddr) iounmap((void*)lpAddr);
		switch (pstParams->inx)
		{
#if !defined(USE_DE_FIRMWARE_RUN_IN_PAK_H15)
			case 2 :
			case 1 :
#if   defined(USE_DE_FIRMWARE_RUN_IN_ROM_H15)
				DE_NOTI("Loading DE_FW_ROM_%s\n", (1==pstParams->inx)?"IRM":"DRM");
				break;
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_H15)
				DE_NOTI("Loading DE_FW_DDR_%s\n", (1==pstParams->inx)?"IRM":"DRM");
				break;
#elif defined(USE_DE_FIRMWARE_LOAD_DRM_IRM_EACH)
				DE_NOTI("Loading DE_FW_RAM_%s\n", (1==pstParams->inx)?"IRM":"DRM");
				if (1==pstParams->inx) break;
#else
				DE_NOTI("Loading DE_FW_DIRAM\n");
#endif
#endif
#if defined(USE_DE_FIRMWARE_RUN_IN_ROM_H15) || defined(USE_DE_FIRMWARE_RUN_IN_DDR_H15) || defined(USE_DE_FIRMWARE_RUN_IN_PAK_H15)
			case 3 :
#endif
#if     defined(USE_DE_FIRMWARE_RUN_IN_PAK_H15)
				DE_NOTI("Loading DE_FW_PAK_ADR5\n");
				if (fwBaseAddr < USE_VIDEO_MCU_ROM_BASE_ADDR) BREAK_WRONG(fwBaseAddr);
				DE_IPC_H15_FLWr(srom_boot_offset1, fwBaseAddr);
				DE_IPC_H15_FLWr(srom_boot_offset2, fwBaseAddr);
				DE_IPC_H15_Wr01(set_ctrl0, start_vector_sel, 0);
				DE_IPC_H15_Wr01(set_ctrl0, mcu_sw_reset, 1);
#elif   defined(USE_DE_FIRMWARE_RUN_IN_ROM_H15)
				DE_NOTI("Loading DE_FW_ROM_ADR5\n");
#if defined(USE_VIDEO_MCU_RUN_IN_DDR0) && (USE_VIDEO_MCU_RUN_IN_DDR0 > USE_VIDEO_MCU_ROM_BASE_ADDR)
				DE_NOTI("FW_BASE_ADDR is %x\n", USE_VIDEO_MCU_RUN_IN_DDR0);
				DE_IPC_H15_FLWr(srom_boot_offset1, USE_VIDEO_MCU_RUN_IN_DDR0);
#else
				if (fwBaseAddr < USE_VIDEO_MCU_ROM_BASE_ADDR) BREAK_WRONG(fwBaseAddr);
				DE_IPC_H15_FLWr(srom_boot_offset1, fwBaseAddr);
#endif
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_H15)
				DE_IPC_H15_FLWr(atlas_port_sel, 0x1434);
				DE_NOTI("Loading DE_FW_DDR_ADR6\n");
#endif
				msleep(1); // wait ddr to ddr transition
				DE_IPC_H15_Wr01(set_ctrl0, run_stall, 0);
				DE_IPC_H15_WrFL(set_ctrl0);
			default :
				break;
		}
	} while (0);

	return ret;
}
#endif

int DE_REG_H15_FW_Verify(LX_DE_FW_DWLD_T *pstParams)
{
	int ret = RET_OK;
	char						*lpAddr = NULL;
	LX_DE_CH_MEM_T				*pFwMem = NULL;
	BOOLEAN						run_stall = 0;
	UINT32						fwBaseAddr;
	UINT32 						fwSize;
	UINT32                      i;
	UINT32                      mismatch_count = 0;

	do {
		CHECK_KNULL(pstParams);
		if (!pstParams->size) break;
		if (!pstParams->pData) break;

		DE_PRINT("Verify start\n");

		DE_IPC_H15_RdFL(set_ctrl0);
		DE_IPC_H15_Rd01(set_ctrl0, run_stall, run_stall);
		DE_IPC_H15_Wr01(set_ctrl0, run_stall, 1);
		DE_IPC_H15_WrFL(set_ctrl0);

#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_LBUS
		pFwMem = &gMemCfgDeFW[4];         // firmware on L-BUS , temporal configuration for B0 evaluation
#else
		pFwMem = (LX_DE_CH_MEM_T *)&gpDeMem->fw[0];
#endif
		CHECK_KNULL(pFwMem);

		fwBaseAddr = pFwMem->fw_base;
		fwSize     = pFwMem->fw_size;
		if (fwSize < pstParams->size) fwSize = pstParams->size;
		fwSize  = GET_RDUP(fwSize, 4);
#if defined(USE_VIDEO_MCU_RUN_IN_DDR0) && (USE_VIDEO_MCU_RUN_IN_DDR0 > USE_VIDEO_MCU_ROM_BASE_ADDR)
		lpAddr	= (char *)ioremap((USE_VIDEO_MCU_RUN_IN_DDR0+USE_VIDEO_MCU_ROM_FW_OFFSET), pstParams->size);
#else
		lpAddr	= (char *)ioremap((fwBaseAddr+USE_VIDEO_MCU_ROM_FW_OFFSET), pstParams->size);
#endif
		CHECK_KNULL(lpAddr);
		for(i=0;i<pstParams->size;i++)
		{
			if(lpAddr[i] != pstParams->pData[i])
			{
				mismatch_count++;
				if(mismatch_count <= 4)
				{
					DE_ERROR("[%d]  : origin 0x%02x , read 0x%02x\n", i, pstParams->pData[i], lpAddr[i]);
				}
			}
		}

		DE_IPC_H15_RdFL(set_ctrl0);
		DE_IPC_H15_Rd01(set_ctrl0, run_stall, run_stall);
		DE_IPC_H15_Wr01(set_ctrl0, run_stall, 0);
		DE_IPC_H15_WrFL(set_ctrl0);

		if(mismatch_count) ret = RET_ERROR;

		DE_PRINT("Verify done.  base[0x%08x] size[%d] mismatch count [%d]\n",\
				fwBaseAddr, fwSize, mismatch_count);
	} while(0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief send color space conversion matrix and offset for each external source information.
 *
 * @param [IN] structure pointer to carry infomation about cvi FIR filter
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_SetCviFir(LX_DE_CVI_FIR_T *pstParams)
{
	int ret = RET_OK;
	DE_PARAM_TYPE_T tableId;
	UINT32 firTable[ARRAY_SIZE(pstParams->fir_coef)+ARRAY_SIZE(pstParams->fir_coef_CbCr)];
	UINT32 inx = 0;
	int i;

	do {
		CHECK_KNULL(pstParams);
		switch (pstParams->cvi_channel) {
			case LX_DE_CVI_CH_A :
				tableId = DE_CVM_FIR_COEF;
				break;
			case LX_DE_CVI_CH_B :
			case LX_DE_CVI_CH_C :
			case LX_DE_CVI_CH_D :
				tableId = DE_CVS_FIR_COEF;
				break;
			default :
				BREAK_WRONG(pstParams->cvi_channel);
		}
		if (ret) break;
		for (i=0;i<ARRAY_SIZE(pstParams->fir_coef);i++) firTable[inx++] = pstParams->fir_coef[i];
		for (i=0;i<ARRAY_SIZE(pstParams->fir_coef_CbCr);i++) firTable[inx++] = pstParams->fir_coef_CbCr[i];

#ifdef USE_PARM_CODES_FOR_H15
		ret = DE_PRM_H15_LoadTable(tableId, DE_PARAM_WRITE, pstParams->isEnable, firTable, inx);
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set captured video test pattern generator to mono-tone color.
 *
 * @param [IN] structure pointer to carry infomation about captured test pattern generator parameter.
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_SetCviTpg(LX_DE_CVI_TPG_T *pstParams)
{
	int ret = RET_OK;
#ifndef _H15_FPGA_TEMPORAL_
	unsigned char ptn_type;

	do {
		CHECK_KNULL(pstParams);
		//(TRUE==pstParams->isPtnOn)?(ptn_type = 0x1):(ptn_type= 0x0); // one color pattern or bypass
		ptn_type = (TRUE==pstParams->isPtnOn)?0x1:0x0; // one color pattern or bypass

		switch (pstParams->cvi_channel) {
			case LX_DE_CVI_CH_A :
				DE_CVA_H15_RdFL(cva_misc_ctrl);
				DE_CVA_H15_Wr01(cva_misc_ctrl, pattern_type,   ptn_type);
				DE_CVA_H15_WfCM(cva_misc_ctrl, pattern_csc,    (TRUE==pstParams->isGBR),   0x1);
				DE_CVA_H15_WfCM(cva_misc_ctrl, write_inhibit,  (TRUE==pstParams->isFrzOn), 0x1);
				DE_CVA_H15_Wr01(cva_misc_ctrl, pattern_detail, pstParams->ptnColor);
				DE_CVA_H15_WrFL(cva_misc_ctrl);
				break;
			case LX_DE_CVI_CH_B :
				DE_CVB_H15_RdFL(cvb_misc_ctrl);
				DE_CVB_H15_Wr01(cvb_misc_ctrl, pattern_type,   ptn_type);
				DE_CVB_H15_WfCM(cvb_misc_ctrl, pattern_csc,    (TRUE==pstParams->isGBR),   0x1);
				DE_CVB_H15_WfCM(cvb_misc_ctrl, write_inhibit,  (TRUE==pstParams->isFrzOn), 0x1);
				DE_CVB_H15_Wr01(cvb_misc_ctrl, pattern_detail, pstParams->ptnColor);
				DE_CVB_H15_WrFL(cvb_misc_ctrl);
				break;
			default :
				BREAK_WRONG(pstParams->cvi_channel);
		}
		if (ret) break;
	} while (0);
#endif
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set captured video color sampling mode(sub sampling or 3 tap filtering).
 *
 * @param [IN] structure pointer to carry infomation about captured color sampling parameter.
 *
 * @return RET_OK(0)
 */
int DE_REG_H15_SetCviCsampleMode(LX_DE_CSAMPLE_MODE_T *pstParams)
{
	int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->cvi_channel) {
			case LX_DE_CVI_CH_A :
				if ( lx_chip_rev() >= LX_CHIP_REV(H15,A0) ) {
					DE_CVA_H15A_WfCM(cva_dig_filt_ctrl3, fir_c_en, (TRUE==pstParams->is3tap), 0x1);
				}
				break;
			case LX_DE_CVI_CH_B :
				if ( lx_chip_rev() >= LX_CHIP_REV(H15,A0) ) {
					DE_CVB_H15A_WfCM(cvb_dig_filt_ctrl3, fir_c_en, (TRUE==pstParams->is3tap), 0x1);
				}
				break;
			default :
				BREAK_WRONG(pstParams->cvi_channel);
		}
		if (ret) break;
	} while (0);
#endif
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set edge crop
 *
 * @param [IN] structure pointer to carry infomation about PE0 black boundary detection status.
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetEdgeCrop(BOOLEAN *pstParams)
{
	int ret = RET_OK;

#ifdef USE_KDRV_CODES_FOR_H15
	do {
		;
	} while (0);
#endif

	return ret;
}

int DE_REG_H15_ResetDE(BOOLEAN bReset)
{
	int ret = RET_OK;

#ifdef USE_DE_DOES_RESET_IN_SUSPEND_RESUME
	bReset = (bReset)?TRUE:FALSE;
#if 0//def	USE_CTOP_CODES_FOR_H15
	if ( lx_chip_rev() >= LX_CHIP_REV(H15,B0) ) {
		CTOP_CTRL_H15_FLWr(ctr05_swrs_of_de, bReset?GET_PMSK(0,32):0);
		CTOP_CTRL_H15_RdFL(ctr06_swrst);
		CTOP_CTRL_H15_WfCM(ctr06_swrst, swrst_de_vd   ,bReset, 1);
		CTOP_CTRL_H15_WfCM(ctr06_swrst, swrst_de_apb  ,bReset, 1);
		CTOP_CTRL_H15_WfCM(ctr06_swrst, swrst_cvda    ,bReset, 1);
		CTOP_CTRL_H15_WrFL(ctr06_swrst);
	}
#endif
#endif
	return ret;
}

BOOLEAN DE_REG_H15_IPCisAlive(void)
{
	BOOLEAN isFwDownloaded = FALSE;
	DE_IPC_H15_FLRf(int_intr_enable, ipc_interrupt_enable_mcu, isFwDownloaded);
#ifdef USE_IPC_CONTROL_INTERRUPT_A_BIT
	return isFwDownloaded;
#else
	return GET_BITS(isFwDownloaded, VIDEO_IPC_INTERRUPT_ARM_BIT, 1);
#endif
}

int DE_REG_H15_SetUdMode(BOOLEAN *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);
		sbDeUdMode = (*pstParams)?TRUE:FALSE;
		if(sbDeUdMode)
		sCviSrcType.cvi_channel = LX_DE_CVI_CH_B;
		ret = DE_REG_H15_SetCviSrcType(&sCviSrcType);
		//if (ret) break;
		//g_de_CviCsc.cvi_channel = LX_DE_CVI_CH_B;
		//ret = DE_HAL_SetCviCsc(&g_de_CviCsc);
	} while (0);

	return ret;
}

int DE_REG_H15_SetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);
		pstParams->address = VIDEO_H15_FIRMWARE_MEM_BASE_WEB_OS;
	} while (0);

	return ret;
}

int DE_REG_H15_GetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);
/*
		if(pstParams->win_id  ==  0)
			ret = VIDEO_WEL_WaitVsync();
		else if(pstParams->win_id  ==  1)
			ret = VIDEO_WER_WaitVsync();
		else
			ret = RET_INVALID_PARAMS;
*/
	} while (0);

	return ret;
}

int DE_REG_H15_SelectMultiWinSrc(LX_DE_MULTI_WIN_SRC_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);
		sDeMultiWinSrc = *pstParams;
	} while (0);

	return ret;
}

/**
 * control vsync interrupt from de hardware
 */
int DE_REG_H15_InitInterrupt(UINT32 mcu_id, BOOLEAN intr_en)
{
	switch(mcu_id)
	{
		case 0:
			DE_SUB_H15_RdFL(ctrl_intr_pulse);
			DE_SUB_H15_RdFL(intr_mux);
			DE_SUB_H15_RdFL(intr_mask);

			DE_SUB_H15_Wr01(ctrl_intr_pulse, sel_src_i5_intr_pulse, 1); /* 0(src), 1(disp) */			
			DE_SUB_H15_Wr01(ctrl_intr_pulse, idx_src_i5_intr_pulse, 0); /* 0(mv_disp_pulse0) */
			
			DE_SUB_H15_Wr01(intr_mux, mux_i5_src_pulse, 1);			   /* to cpu */
			DE_SUB_H15_Wr01(intr_mask, mask_i5_src_pulse, intr_en?0:1); /* 0:unmask */
			
			DE_SUB_H15_WrFL(ctrl_intr_pulse);
			DE_SUB_H15_WrFL(intr_mux);
			DE_SUB_H15_WrFL(intr_mask);
			break;
		case 1:
			break;
		default:
			break;
	}

	return RET_OK;
}

/**
 * get irq number
 */
int DE_REG_H15_GetIrqNum(UINT32 mcu_id, UINT32 *ipc_irq_num, UINT32 *sync_irq_num)
{
	switch(mcu_id)
	{
		case 0:
			*ipc_irq_num  = H15_IRQ_IPC_BCPU;
			*sync_irq_num = H15_IRQ_DE_BCPU;
			break;
		case 1:
			break;
		default:
			return RET_ERROR;
			break;
	}
	return RET_OK;
}

/**
 * set debug data
 */
int DE_REG_H15_SetDebug(LX_DE_SET_DBG_T *pstParams)
{
	int ret = RET_OK;

#ifndef _H15_FPGA_TEMPORAL_
#ifndef USE_DE_CVI_DELAY_ON_MCU_PART
	UINT32 line_width = 0;
	UINT32 syncPosition = 0;
#endif

	do {
		CHECK_KNULL(pstParams);
		switch (pstParams->type) {
			case LX_DE_DBG_PIXEL_SHIFT :
#ifndef USE_DE_CVI_DELAY_ON_MCU_PART
				{
					if (pstParams->win_id == 0)
					{
						if (pstParams->bParam == 1)
						{
							DE_CVA_H15_RdFL(cva_top_ctrl);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_r, 1);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_g, 3);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_b, 3);
							DE_CVA_H15_WrFL(cva_top_ctrl);
						}
						else
						{
							DE_CVA_H15_RdFL(cva_top_ctrl);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_r, 0);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_g, 0);
							DE_CVA_H15_Wr01(cva_top_ctrl, yc_delay_mode_b, 0);
							DE_CVA_H15_WrFL(cva_top_ctrl);
						}
					}
					else
					{
						if (pstParams->bParam == 1)
						{
							DE_CVB_H15_RdFL(cvb_top_ctrl);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_r, 0x1);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_g, 0x3);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_b, 0x3);
							DE_CVB_H15_WrFL(cvb_top_ctrl);
						}
						else
						{
							DE_CVB_H15_RdFL(cvb_top_ctrl);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_r, 0x0);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_g, 0x0);
							DE_CVB_H15_Wr01(cvb_top_ctrl, yc_delay_mode_b, 0x0);
							DE_CVB_H15_WrFL(cvb_top_ctrl);
						}
					}
				}
#endif
				break;

			case LX_DE_DBG_SYNC_POSITION :
#ifndef USE_DE_CVI_DELAY_ON_MCU_PART
				{
					if (pstParams->win_id == 0)
					{
						if (pstParams->bParam == 1)
						{
							DE_CVA_H15_RdFL(cva_size_detect_read);
							DE_CVA_H15_Rd01(cva_size_detect_read, line_width_read, line_width);
							syncPosition = pstParams->u32Param - pstParams->u32ParamOne;		// vTotal - vActive
							syncPosition = ( syncPosition * line_width) >> 3;
						}
						else
						{
							syncPosition = 0;
						}

						DE_DEA_H15_RdFL(dea_msrc_sync_dly);
						DE_DEA_H15_Wr01(dea_msrc_sync_dly, dea_msrc_sync_dly, syncPosition);
						DE_DEA_H15_WrFL(dea_msrc_sync_dly);
					}
					else
					{
						if (pstParams->bParam == 1)
						{
							DE_CVB_H15_RdFL(cvb_size_detect_read);
							DE_CVB_H15_Rd01(cvb_size_detect_read, line_width_read, line_width);
							syncPosition = pstParams->u32Param - pstParams->u32ParamOne;		// vTotal - vActive
							syncPosition = ( syncPosition * line_width) >> 3;
						}
						else
						{
							syncPosition = 0;
						}

						DE_DEC_H15_RdFL(dec_msrc_sync_dly);
						DE_DEC_H15_Wr01(dec_msrc_sync_dly, dec_msrc_sync_dly, syncPosition);
						DE_DEC_H15_WrFL(dec_msrc_sync_dly);
					}
				}
#endif
				break;

			case LX_DE_DBG_SCALER_TPG :
				if (pstParams->win_id == 0)
				{
					DE_OVL_H15_RdFL(ovl_ov_ctrl4);
					DE_OVL_H15_Wr01(ovl_ov_ctrl4, w0_dark_en, (pstParams->bParam == 1)? 1:0);
					DE_OVL_H15_WrFL(ovl_ov_ctrl4);
				}
				else
				{
					// PIP
					DE_OVL_H15_RdFL(ovl_ov_ctrl4);
					DE_OVL_H15_Wr01(ovl_ov_ctrl4, w1_dark_en, (pstParams->bParam == 1)? 1:0);
					DE_OVL_H15_WrFL(ovl_ov_ctrl4);

					// UD & 3D
					DE_OVR_H15_RdFL(ovr_ov_ctrl4);
					DE_OVR_H15_Wr01(ovr_ov_ctrl4, w0_dark_en, (pstParams->bParam == 1)? 1:0);
					DE_OVR_H15_WrFL(ovr_ov_ctrl4);
				}
				break;

			case LX_DE_DBG_CVI_RESET :			// ON MCU Part
			default :
				break;
		}
	} while (0);
#endif  // #ifndef _H15_FPGA_TEMPORAL_
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetInterfaceConfig(LX_DE_IF_CONFIG_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->config_type) {
			case LX_DE_CONFIG_TYPE_ALL :
				g_Display_type_H15 = pstParams->display_type;
				g_Display_mirror_H15 = pstParams->display_mirror;
				g_Frc_type_H15 = pstParams->frc_type;
				g_Trid_type_H15 = pstParams->trid_type;
				break;
			case LX_DE_CONFIG_TYPE_DISPLAY_DEVICE :
				g_Display_type_H15 = pstParams->display_type;
				break;
			case LX_DE_CONFIG_TYPE_DISPLAY_MIRROR:
				g_Display_mirror_H15 = pstParams->display_mirror;
				break;
			case LX_DE_CONFIG_TYPE_FRC :
				g_Frc_type_H15 = pstParams->frc_type;
				break;
			case LX_DE_CONFIG_TYPE_3D :
				g_Trid_type_H15 = pstParams->trid_type;
				break;
			case LX_DE_CONFIG_TYPE_MAX :
			default :
				BREAK_WRONG(pstParams->config_type);
		}
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_GetSystemStatus(LX_DE_SYS_STATUS_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->status_type) {
			case LX_DE_SYS_STATUS_ALL :
				pstParams->display_type = g_Display_type_H15;
				pstParams->display_mirror = g_Display_mirror_H15;
				pstParams->frc_type = g_Frc_type_H15;
				pstParams->trid_type = g_Trid_type_H15;
				pstParams->fc_mem = 0;
				pstParams->display_size = g_Display_size_H15;
				break;
			case LX_DE_SYS_STATUS_DISPALY_DEVICE :
				pstParams->display_type = g_Display_type_H15;
				break;
			case LX_DE_SYS_STATUS_DISPALY_MIRROR:
				pstParams->display_mirror = g_Display_mirror_H15;
				break;
			case LX_DE_SYS_STATUS_FRC :
				pstParams->frc_type = g_Frc_type_H15;
				break;
			case LX_DE_SYS_STATUS_3D :
				pstParams->trid_type = g_Trid_type_H15;
				break;
			case LX_DE_SYS_STATUS_FC_MEM:
				pstParams->fc_mem = 0;
				break;
			case LX_DE_SYS_STATUS_DISPALY_SIZE:
				pstParams->display_size = g_Display_size_H15;
				break;
			case LX_DE_SYS_STATUS_MAX :
			default :
				BREAK_WRONG(pstParams->status_type);
		}

	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->status_type) {
			case LX_DE_SRC_STATUS_ALL :
				pstParams->inSrc[LX_DE_WIN_MAIN] = g_WinsrcMap_H15[LX_DE_WIN_MAIN];
				pstParams->inSrcPort[LX_DE_WIN_MAIN] = g_WinsrcPort_H15[LX_DE_WIN_MAIN];
				pstParams->inSrc[LX_DE_WIN_SUB]	 = g_WinsrcMap_H15[LX_DE_WIN_SUB];
				pstParams->inSrcPort[LX_DE_WIN_SUB] = g_WinsrcPort_H15[LX_DE_WIN_SUB];

				pstParams->operType= g_SrcOperType_H15;
				pstParams->operCtrlFlag = g_SrcOperCtrlFlag_H15;

				pstParams->subOperType =  g_SrcSubOperType_H15;
				pstParams->subOperCtrlFlag = g_SrcSubOperCtrlFlag_H15;
				break;
			case LX_DE_SRC_STATUS_INPUT_SRC :
				pstParams->inSrc[LX_DE_WIN_MAIN] = g_WinsrcMap_H15[LX_DE_WIN_MAIN];
				pstParams->inSrcPort[LX_DE_WIN_MAIN] = g_WinsrcPort_H15[LX_DE_WIN_MAIN];
				pstParams->inSrc[LX_DE_WIN_SUB] = g_WinsrcMap_H15[LX_DE_WIN_SUB];
				pstParams->inSrcPort[LX_DE_WIN_SUB] = g_WinsrcPort_H15[LX_DE_WIN_SUB];
				break;
			case LX_DE_SRC_STATUS_OPER :
				pstParams->operType  = g_SrcOperType_H15;
				pstParams->operCtrlFlag = g_SrcOperCtrlFlag_H15;
				break;

			case LX_DE_SRC_STATUS_SUB_OPER :
				pstParams->subOperType =  g_SrcSubOperType_H15;
				pstParams->subOperCtrlFlag = g_SrcSubOperCtrlFlag_H15;
				break;

			case LX_DE_SRC_STATUS_MAX :
			default :
				BREAK_WRONG(pstParams->status_type);
		}
	} while (0);

	return ret;


}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetOperation(LX_DE_OPERATION_CTRL_T *pstParams)
{
	int ret = RET_OK;
	LX_DE_OPER_CONFIG_T	type = LX_DE_OPER_ONE_WIN;
	UINT16 flag = 0;

	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->operation) {
			case LX_DE_OPER_ONE_WIN :
				type = LX_DE_OPER_ONE_WIN;
				break;

			case LX_DE_OPER_TWO_WIN :
				if(pstParams->multiCtrl != FALSE)		type = LX_DE_OPER_TWO_WIN;
				flag = pstParams->multiCtrl;
				break;
			case LX_DE_OPER_3D :
				if(pstParams->ctrl3D.run_mode != LX_DE_3D_RUNMODE_OFF && pstParams->ctrl3D.run_mode != LX_DE_3D_RUNMODE_MAX)
				{
					type = LX_DE_OPER_3D;
					flag = pstParams->ctrl3D.in_img_fmt;
				}
				break;

			case LX_DE_OPER_UD :
				if(pstParams->udCtrl != LX_DE_UD_OFF && pstParams->udCtrl != LX_DE_UD_MAX)
				{
					type = LX_DE_OPER_UD;
					flag = pstParams->udCtrl;
				}
				break;

			case LX_DE_OPER_VENC :
#if 0
				if(pstParams->vencCtrl.bOnOff != FALSE) {
					type = LX_DE_OPER_VENC;
					enable = TRUE;
				}
				break;
#endif
			case LX_DE_OPER_MAX :
			default :
				BREAK_WRONG(pstParams->operation);
		}

		g_SrcOperType_H15 = type;
		g_SrcOperCtrlFlag_H15 = flag;
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information from which source is comming
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetSubOperation(LX_DE_SUB_OPERATION_CTRL_T *pstParams)
{
	int ret = RET_OK;
	UINT16 flag = 0;

	do {
		CHECK_KNULL(pstParams);

		switch (pstParams->operation) {
			case LX_DE_SUB_OPER_OFF :
				g_SrcSubOperType_H15 = LX_DE_SUB_OPER_OFF;
				break;
			case LX_DE_SUB_OPER_CAPTURE:
				g_SrcSubOperType_H15 = LX_DE_SUB_OPER_CAPTURE;
				flag = pstParams->capture_enable;
				break;
			case LX_DE_SUB_OPER_VENC:
				g_SrcSubOperType_H15 = LX_DE_SUB_OPER_VENC;
				flag = pstParams->vencCtrl.bOnOff;
				break;
			case LX_DE_SUB_OPER_SCART_OUT:
				g_SrcSubOperType_H15 = LX_DE_SUB_OPER_SCART_OUT;
				flag = 1;
				break;
			case LX_DE_SUB_OPER_MAX :
			default :
				BREAK_WRONG(pstParams->operation);
		}
	} while (0);

	g_SrcSubOperCtrlFlag_H15 = flag;
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get fir coefficient for cvi
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_GetFIR(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir)
{
	int ret = RET_OK;
	int i = 0;
	// CVI FIR coefficient for Y - 11 Tap,Cbcr - 11 Tap
	UINT16 coef_FIR_Y_Normal[] = {256, 0, 0, 0, 0, 0, 0, 0};
	UINT16 coef_FIR_Y_Double[] = {256, 0, 0, 0, 0, 0, 0, 0};
	UINT16 coef_FIR_Y_Quad[]   = {64, 56, 34, 12, 0, -6, 0, 0};	// 55 >> 56 --> sum 254 >> 256
	UINT16 coef_FIR_H15_CVD_CbCr_Normal[] = {160, 49, -1, 0, 0, 0};// CVI FIR Filter for 1 pixel & 422
	UINT16 coef_FIR_CbCr_Normal[] = {128, 74, 0, -11, 0, 1};// CVI FIR Filter for 1 pixel & 422
	UINT16 coef_FIR_CbCr_Double[] = {160, 0, 49, 0, -1, 0};	// 55 >> 56 --> sum 254 >> 256
	UINT16 coef_FIR_CbCr_Quad[] = {64, 56, 34, 12, 0, -6};

	do {
		CHECK_KNULL(pstParams);

		fir->cvi_channel = pstParams->cvi_channel;

		switch (pstParams->sampling) {
			case LX_DE_CVI_NORMAL_SAMPLING:
				fir->isEnable = TRUE;
				for (i=0; i<NUM_OF_CVI_FIR_COEF; i++) {
					fir->fir_coef[i] = coef_FIR_Y_Normal[i];

					if (i<(NUM_OF_CVI_FIR_COEF-2)) {
						if (pstParams->cvi_input_src == LX_DE_CVI_SRC_ATV || pstParams->cvi_input_src == LX_DE_CVI_SRC_CVBS)
							fir->fir_coef_CbCr[i] = coef_FIR_H15_CVD_CbCr_Normal[i];
						else
							fir->fir_coef_CbCr[i] = coef_FIR_CbCr_Normal[i];
					}
				}
				break;

			case LX_DE_CVI_DOUBLE_SAMPLING:
				fir->isEnable = TRUE;
				for (i=0; i<NUM_OF_CVI_FIR_COEF; i++) {
					fir->fir_coef[i] = coef_FIR_Y_Double[i];
					if (i<(NUM_OF_CVI_FIR_COEF-2))
						fir->fir_coef_CbCr[i] = coef_FIR_CbCr_Double[i];
				}
				break;

			case LX_DE_CVI_QUAD_SAMPLING:
				fir->isEnable = TRUE;
				for (i=0; i<NUM_OF_CVI_FIR_COEF; i++) {
					fir->fir_coef[i] = coef_FIR_Y_Quad[i];
					if (i<(NUM_OF_CVI_FIR_COEF-2))
						fir->fir_coef_CbCr[i] = coef_FIR_CbCr_Quad[i];
				}
				break;

			default:
				BREAK_WRONG(pstParams->sampling);
		}
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set hdmi in-port for cvi
 *
 * @param arg [IN] hdmi port num
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_SetHdmiPort(LX_DE_HDMIPORT_T *pstParams)
{
	int ret = RET_OK;

	g_HdmiPort = pstParams->port_num;

	DE_PRINT("win-hdmi map [%d / %d]", pstParams->win_id, pstParams->port_num);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get capacity
 *
 * @param arg [OUT] capa info
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_REG_H15_GetCapacity(LX_DE_CAPACITY_T *pstParams)
{
	int ret = RET_OK;

	pstParams->max_win = 4;
	pstParams->max_layer = 4;
	pstParams->max_in_size.w = 3840;
	pstParams->max_in_size.h = 2160;
	pstParams->max_out_size.w = 3840;
	pstParams->max_out_size.h = 2160;
	pstParams->max_out_fr_rate = 60;
	pstParams->mvir_start_num = 4;
	pstParams->default_3d_out_fmt = LX_DE_3D_IMG_OUT_SINGLE_TB;

	pstParams->vtm_capa.maxResolution.x      = 0;
	pstParams->vtm_capa.maxResolution.y      = 0;
	pstParams->vtm_capa.maxResolution.w      = 1920;
	pstParams->vtm_capa.maxResolution.h      = 1080;	
	pstParams->vtm_capa.bLeftTopAlign        = FALSE;
	pstParams->vtm_capa.bSupportInputVideoDeInterlacing   = FALSE;
	pstParams->vtm_capa.bSupportDisplayVideoDeInterlacing = TRUE;
	pstParams->vtm_capa.bSupportScaleUp      = TRUE;
	pstParams->vtm_capa.scaleUpLimitWidth    = 1920;
	pstParams->vtm_capa.scaleUpLimitHeight   = 1080;
	pstParams->vtm_capa.bSupportScaleDown    = TRUE;
	pstParams->vtm_capa.scaleDownLimitWidth  = 320;
	pstParams->vtm_capa.scaleDownLimitHeight = 240;
	pstParams->vtm_capa.locationLimit        = 2;

	DE_TRACE("max_win[%d], mvir_start_num[%d], 2d_3d_fmt[%d]", \
		pstParams->max_win, \
		pstParams->mvir_start_num, \
		pstParams->default_3d_out_fmt);

	return ret;
}

#endif
/**  @} */
