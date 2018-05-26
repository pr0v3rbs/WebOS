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
 * date       2010.02.19
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef _DE_IO_h
#define _DE_IO_h
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
int DE_IO_Resume(unsigned long arg);
int DE_IO_IPC_Init(unsigned long arg);
int DE_IO_IPC_Cleanup(unsigned long arg);
int DE_IO_Init(unsigned long arg);
int DE_IO_GetFirmwareInfo(unsigned long arg);
int DE_IO_GetInputWin(unsigned long arg);
int DE_IO_GetOutWin(unsigned long arg);
int DE_IO_SetInputWin(unsigned long arg);
int DE_IO_SetOutWin(unsigned long arg);
int DE_IO_SetDisOut(unsigned long arg);
int DE_IO_SetCviSrcType(unsigned long arg);
int DE_IO_GetMviColorimetry(unsigned long arg);
int DE_IO_SetZList(unsigned long arg);
int DE_IO_SetDisFmt(unsigned long arg);
int DE_IO_SetFrRate(unsigned long arg);
int DE_IO_SetBgColor(unsigned long arg);
int DE_IO_SetWinBlank(unsigned long arg);
int DE_IO_MultiWinEnable(unsigned long arg);
int DE_IO_RegRd(unsigned long arg);
int DE_IO_RegWr(unsigned long arg);
int DE_IO_GetOutFrRate(unsigned long arg);
int DE_IO_SetDeinterlace(unsigned long arg);
int DE_IO_SetUDMode(unsigned long arg);
int DE_IO_SetCve(unsigned long arg);
int DE_IO_SetVcs(unsigned long arg);
int DE_IO_SetUart4MCU(unsigned long arg);

#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_IO_FW_Download(unsigned long arg);
#endif

int DE_IO_SetCviFir(unsigned long arg);
int DE_IO_SetCviTpg(unsigned long arg);
int DE_IO_SetCviCsampleMode(unsigned long arg);
int DE_IO_SetDvrFmtCtrl(unsigned long arg);
int DE_IO_SetSubScFreeze(unsigned long arg);
int DE_IO_SetCviVideoFrameBuffer(unsigned long arg);
int DE_IO_SelectMultiWinSrc(unsigned long arg);
int DE_IO_SetScartRGBBypassMode(unsigned long arg);
int DE_IO_SetCviFreeze(unsigned long arg);
int DE_IO_GetDebug(unsigned long arg);
int DE_IO_SetDebug(unsigned long arg);
int DE_IO_SetLowDelay(unsigned long arg);
int DE_IO_GetLowDelay(unsigned long arg);
int DE_IO_Set3DInOutCtrl(unsigned long arg);

int DE_IO_SetInterfaceConfig(unsigned long arg);
int DE_IO_GetSystemStatus(unsigned long arg);
int DE_IO_GetSourceStatus(unsigned long arg);
int DE_IO_SetOperation(unsigned long arg);
int DE_IO_SetSubOperation(unsigned long arg);
int DE_IO_SetCaptureControl(unsigned long arg);
int DE_IO_SetMultiVision(unsigned long arg);
int DE_IO_SetVTM(unsigned long arg);
int DE_IO_GetVTM(unsigned long arg);
int DE_IO_GetScalerInfo(unsigned long arg);
int DE_IO_SetInnerPattern(unsigned long arg);
int DE_IO_SetWinPortSrc(unsigned long arg);
int DE_IO_SetWinFreeze(unsigned long arg);
int DE_IO_VerifyFirmware(unsigned long arg);
int DE_IO_SetSrcMirror(unsigned long arg);
int DE_IO_SetActFmtDetect(unsigned long arg);
int DE_IO_SetHDMIPort(unsigned long arg);
int DE_IO_GetCapacity(unsigned long arg);
int DE_IO_SetSubWindow(unsigned long arg);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif
/**  @} */
