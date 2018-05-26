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
#ifndef  DE_REG_H13_INC
#define  DE_REG_H13_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int DE_REG_H13_InitPHY2VIRT(void);
int DE_REG_H13_FreePHY2VIRT(void);
int DE_REG_H13_InitAddrSwitch(void);
int DE_IPC_H13_WakeUpReg(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, BOOLEAN turnOn);
int DE_IPC_H13_ClearStatus(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
int DE_IPC_H13_GetStatusReg(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
int DE_WDG_H13_WakeUpReg(UINT32 mcu_id, BOOLEAN turnOn);
int DE_DMA_H13_WakeUpReg(BOOLEAN turnOn);
UINT32 DE_REG_H13_RD(UINT32 addr);
int DE_REG_H13_WD(UINT32 addr, UINT32 value);
int DE_REG_H13_SetBgColor(LX_DE_COLOR_T *pBackColor);
int DE_REG_H13_SetWinBlank(LX_DE_SET_WIN_BLANK_T *pWinBlank);

BOOLEAN DE_REG_H13_CheckIrq4Vsync(UINT32 mcu_id);
int DE_REG_H13_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_T intrType, UINT32 *pVideoIrqStatus);
int DE_REG_H13_Init(LX_DE_PANEL_TYPE_T *pstParams);
int DE_REG_H13_SetFrRate(DE_DPLL_CLK_T dclk);
int DE_REG_H13_GetFrRate(LX_DE_FR_RATE_T *pstParams);
int DE_REG_H13_SetWinPortSrc(LX_DE_INPUT_CFG_T *pstParams);
int DE_REG_H13_SetCviSrcType(LX_DE_CVI_SRC_TYPE_T *pstParams);
int DE_REG_H13_GPIO_Init(void);
int DE_REG_H13_HDMI_Init(void);
int DE_REG_H13_LVDS_Init(void);
int DE_REG_H13_MISC_Init(void);
int DE_REG_H13_OSD_Init(void);
int DE_REG_H13_Uart0_Switch(int whichUart);
int DE_REG_H13_VSPMute(BOOLEAN turnOn);


#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_REG_H13_FW_Download(LX_DE_FW_DWLD_T *pstParams);
#endif

int DE_REG_H13_SetCviFir(LX_DE_CVI_FIR_T *pstParams);
int DE_REG_H13_SetCviTpg(LX_DE_CVI_TPG_T *pstParams);
int DE_REG_H13_SetCviCsampleMode(LX_DE_CSAMPLE_MODE_T *pstParams);
int DE_REG_H13_SetVcs(LX_DE_VCS_IPC_T *pstParams);
int DE_REG_H13_ResetDE(BOOLEAN bReset);
BOOLEAN DE_REG_H13_IPCisAlive(void);
int DE_REG_H13_SetCviVideoFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams);
int DE_REG_H13_SelectMultiWinSrc(LX_DE_MULTI_WIN_SRC_T *pstParams);
int DE_REG_H13_SetUdMode(BOOLEAN *pstParams);
int DE_REG_H13_SetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);
int DE_REG_H13_GetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);

int DE_REG_H13_InitInterrupt(UINT32 mcu_id, BOOLEAN intr_en);
int DE_REG_H13_GetIrqNum(UINT32 mcu_id,      \
                         UINT32 *ipc_irq_num,\
                         UINT32 *sync_irq_num);
int DE_REG_H13_SetDebug(LX_DE_SET_DBG_T *pstParams);

int DE_REG_H13_SetInterfaceConfig(LX_DE_IF_CONFIG_T *pstParams);
int DE_REG_H13_GetSystemStatus(LX_DE_SYS_STATUS_T *pstParams);
int DE_REG_H13_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams);
int DE_REG_H13_SetOperation(LX_DE_OPERATION_CTRL_T *pstParams);
int DE_REG_H13_SetSubOperation(LX_DE_SUB_OPERATION_CTRL_T *pstParams);
int DE_REG_H13_GetFIR(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir);
int DE_REG_H13_FW_Verify(LX_DE_FW_DWLD_T *pstParams);
int DE_REG_H13_SetHdmiPort(LX_DE_HDMIPORT_T *pstParams);
int DE_REG_H13_GetCapacity(LX_DE_CAPACITY_T *pstParams);
int DE_REG_H13_SetSeamlessInfo(LX_DE_RECT_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_REG_H13_INC  ----- */
/**  @} */
