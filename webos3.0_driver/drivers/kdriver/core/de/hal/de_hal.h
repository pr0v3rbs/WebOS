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
 * date       2010.03.08
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef  DE_HAL_INC
#define  DE_HAL_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "mcu/de_ipc_def.h"
#include "mcu/de_vbi_def.h"

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
int DE_HAL_Select_Chip(void);
int DE_HAL_Init(LX_DE_PANEL_TYPE_T *pstParams);
int DE_HAL_InitRegister(void);
int DE_HAL_SetFrRate(LX_DE_FR_RATE_T *pstParams);
int DE_HAL_GetFrRate(LX_DE_FR_RATE_T *pstParams);
int DE_HAL_SetBgColor(LX_DE_COLOR_T *pBackColor);
int DE_HAL_SetWinBlank(LX_DE_SET_WIN_BLANK_T *pWinBlank);
int DE_HAL_GetMviColorimetry(LX_DE_MVI_COLORIMETRY_T *pstParams);
int DE_HAL_SetCviSrcType(LX_DE_CVI_SRC_TYPE_T *pstParams);
int DE_HAL_SetUart4MCU(BOOLEAN bUart4MCU);

#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_HAL_FW_Download(LX_DE_FW_DWLD_T *pstParams);
#endif

int DE_HAL_SetCviFir(LX_DE_CVI_FIR_T *pstParams);
int DE_HAL_SetCviTpg(LX_DE_CVI_TPG_T *pstParams);
int DE_HAL_SetCviCsampleMode(LX_DE_CSAMPLE_MODE_T *pstParams);
int DE_HAL_SetVcs(LX_DE_VCS_IPC_T *pstParams);

int DE_HAL_ResetDE(BOOLEAN bReset);
int DE_HAL_InitPHY2VIRT(void);
int DE_HAL_FreePHY2VIRT(void);
int DE_HAL_GPIO_Init(void);
int DE_HAL_HDMI_Init(void);
int DE_HAL_LVDS_Init(void);
int DE_HAL_MISC_Init(void);
int DE_HAL_OSD_Init(void);
int DE_HAL_FW_DownloadFromBin(void);
int DE_HAL_FW_DownloadFromHead(void);
int DE_HAL_IPCisAlive(void);

int DE_HAL_WriteReg(UINT32 addr, UINT32 value);
int DE_HAL_ReadReg(UINT32 addr, UINT32 *value);
int DE_HAL_SetCviVideoFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams);
int DE_HAL_SetCviRWFrameBuffer(LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams,\
								LX_DE_CVI_RW_PREW_FRAME_T *prewInfo);
int DE_HAL_SelectMultiWinSrc(LX_DE_MULTI_WIN_SRC_T *pstParams);
int DE_HAL_SetSCARTRGBBypassMode(int *pEnable);
int DE_HAL_SetUdMode(BOOLEAN *pstParams);
int DE_HAL_GetDebug(LX_DE_SET_DBG_T *pstParams);
int DE_HAL_SetDebug(LX_DE_SET_DBG_T *pstParams);

int DE_HAL_SetWinPortSrc(LX_DE_INPUT_CFG_T *pstParams);
int DE_HAL_SetInterfaceConfig(LX_DE_IF_CONFIG_T *pstParams);
int DE_HAL_GetSystemStatus(LX_DE_SYS_STATUS_T *pstParams);
int DE_HAL_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams);
int DE_HAL_SetOperation(LX_DE_OPERATION_CTRL_T *pstParams);
int DE_HAL_SetSubOperation(LX_DE_SUB_OPERATION_CTRL_T *pstParams);
int DE_HAL_SetCaptureControl(LX_DE_CAPTURE_CTRL_T *pstParams);
int DE_HAL_SwitchUART0(int whichUart);
int DE_HAL_SetTtx(VIDEO_TTX_PACK_T *pTtxPack);
int DE_HAL_SetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);
int DE_HAL_GetVTM(LX_DE_VTM_FRAME_INFO_T *pstParams);
int DE_HAL_GetFIR(LX_DE_CVI_SRC_TYPE_T *pstParams, LX_DE_CVI_FIR_T *fir);
int DE_HAL_VerifyFirmware(void);
int DE_HAL_SetSeamlessInfo(LX_DE_RECT_T *pstParams);
int DE_HAL_SetHDMIPort(LX_DE_HDMIPORT_T *pstParams);
int DE_HAL_GetCapacity(LX_DE_CAPACITY_T *pstParams);
#endif   /* ----- #ifndef DE_HAL_INC  ----- */
/**  @} */
