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
#ifndef  DE_REG_H15_INC
#define  DE_REG_H15_INC

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
typedef enum {
	HDMI_PORT_0 = 0,
	HDMI_PORT_1 = 1,
	HDMI_PORT_2 = 2,
	HDMI_PORT_3 = 3
} HDMI_PORT_NUM;

typedef enum {
	CLK_ADC = 0,
	CLK_CVD = 1,
	CLK_HDA = 8,
	CLK_HDB = 10,
	CLK_HDC = 12,
	CLK_HDD = 14,
	CLK_NONE  = 0xFF
} CVI_SEL_ENUM;

typedef enum {
	CVI_HDMI_A_L = 0, // HDMI 2.0 input
	CVI_HDMI_A_R,
	CVI_HDMI_B_L,
	CVI_HDMI_B_R,
	CVI_HDMI_C_L, // HDMI 1.4 input
	CVI_HDMI_C_R,
	CVI_HDMI_D_L,
	CVI_HDMI_D_R,
	CVI_CVD       = 8,	
	CVI_ADC,
	CVI_NONE      = 0xFF
} CVI_DATA_IN_SEL;

typedef enum { // HDA = HDMI_A/B , HDC = HDMI_C/D
	SRC_CVD_ADC,
	SRC_CVD_HDA,
	SRC_CVD_HDC,
	SRC_CVD_CVD,
	SRC_ADC_CVD,
	SRC_ADC_HDA,
	SRC_ADC_HDC,
	SRC_ADC_ADC,
	SRC_HDA_CVD,
	SRC_HDA_ADC,
	SRC_HDA_HDC,
	SRC_HDA_HDA,
	SRC_HDC_CVD,
	SRC_HDC_ADC,
	SRC_HDC_HDA,
	SRC_HDC_HDC,
	SRC_MAP_MAX
} SRC_MAP_DATA;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int DE_REG_H15_InitPHY2VIRT(void);
int DE_REG_H15_FreePHY2VIRT(void);
int DE_REG_H15_InitAddrSwitch(void);
int DE_IPC_H15_WakeUpReg(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, BOOLEAN turnOn);
int DE_IPC_H15_ClearStatus(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
int DE_IPC_H15_GetStatusReg(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
int DE_WDG_H15_WakeUpReg(UINT32 mcu_id, BOOLEAN turnOn);
int DE_DMA_H15_WakeUpReg(BOOLEAN turnOn);
UINT32 DE_REG_H15_RD(UINT32 addr);
int DE_REG_H15_WD(UINT32 addr, UINT32 value);
int DE_REG_H15_SetBgColor(LX_DE_COLOR_T *pBackColor);
int DE_REG_H15_SetWinBlank(LX_DE_SET_WIN_BLANK_T *pWinBlank);

BOOLEAN DE_REG_H15_CheckIrq4Vsync(UINT32 mcu_id);
int DE_REG_H15_UpdateVideoIrqStatus(VIDEO_INTR_TYPE_T intrType, UINT32 *pVideoIrqStatus);
int DE_REG_H15_Init(LX_DE_PANEL_TYPE_T *pstParams);
int DE_REG_H15_SetFrRate(DE_DPLL_CLK_T dclk);
int DE_REG_H15_GetFrRate(LX_DE_FR_RATE_T *pstParams);
int DE_REG_H15_SetWinPortSrc(LX_DE_INPUT_CFG_T *pstParams);
int DE_REG_H15_SetCviSrcType(LX_DE_CVI_SRC_TYPE_T *pstParams);
int DE_REG_H15_GPIO_Init(void);
int DE_REG_H15_HDMI_Init(void);
int DE_REG_H15_LVDS_Init(void);
int DE_REG_H15_MISC_Init(void);
int DE_REG_H15_OSD_Init(void);
int DE_REG_H15_Uart0_Switch(int whichUart);
int DE_REG_H15_VSPMute(BOOLEAN turnOn);


#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_REG_H15_FW_Download(LX_DE_FW_DWLD_T *pstParams);
#endif

int DE_REG_H15_SetCviFir(LX_DE_CVI_FIR_T *pstParams);
int DE_REG_H15_SetCviTpg(LX_DE_CVI_TPG_T *pstParams);
int DE_REG_H15_SetCviCsampleMode(LX_DE_CSAMPLE_MODE_T *pstParams);
int DE_REG_H15_SetEdgeCrop(BOOLEAN *pstParams);
int DE_REG_H15_SetVcs(LX_DE_VCS_IPC_T *pstParams);
int DE_REG_H15_ResetDE(BOOLEAN bReset);
BOOLEAN DE_REG_H15_IPCisAlive(void);
int DE_REG_H15_SetCviVideoFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams);
int DE_REG_H15_SetCviRWFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams, LX_DE_CVI_RW_PREW_FRAME_T *prewInfo);
int DE_REG_H15_SelectMultiWinSrc(LX_DE_MULTI_WIN_SRC_T *pstParams);
int DE_REG_H15_SetUdMode(BOOLEAN *pstParams);
int DE_REG_H15_SetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);
int DE_REG_H15_GetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);

int DE_REG_H15_InitInterrupt(UINT32 mcu_id, BOOLEAN intr_en);
int DE_REG_H15_GetIrqNum(UINT32 mcu_id,      \
                         UINT32 *ipc_irq_num,\
                         UINT32 *sync_irq_num);
int DE_REG_H15_SetDebug(LX_DE_SET_DBG_T *pstParams);

int DE_REG_H15_SetInterfaceConfig(LX_DE_IF_CONFIG_T *pstParams);
int DE_REG_H15_GetSystemStatus(LX_DE_SYS_STATUS_T *pstParams);
int DE_REG_H15_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams);
int DE_REG_H15_SetOperation(LX_DE_OPERATION_CTRL_T *pstParams);
int DE_REG_H15_SetSubOperation(LX_DE_SUB_OPERATION_CTRL_T *pstParams);
int DE_REG_H15_GetFIR(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir);
int DE_REG_H15_FW_Verify(LX_DE_FW_DWLD_T *pstParams);
int DE_REG_H15_SetHdmiPort(LX_DE_HDMIPORT_T *pstParams);
int DE_REG_H15_GetCapacity(LX_DE_CAPACITY_T *pstParams);
/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_REG_H15_INC  ----- */
/**  @} */
