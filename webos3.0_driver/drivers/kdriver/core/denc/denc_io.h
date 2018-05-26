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
 * date       2010.03.17
 * note       Additional information.
 *
 * @addtogroup lg1150_denc
 * @{
 */

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

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
int DENC_IO_IPC_Init(unsigned long arg);
int DENC_IO_IPC_Cleanup(unsigned long arg);
int DENC_IO_Resume(void);
int DENC_IO_Init(unsigned long arg);
int DENC_IO_OnOff(unsigned long arg);
int DENC_IO_SetOutPutVideoStd(unsigned long arg);
int DENC_IO_SetSource(unsigned long arg);
int DENC_IO_TtxEnable(unsigned long arg);
int DENC_IO_WssEnable(unsigned long arg);
int DENC_IO_VpsEnable(unsigned long arg);
int DENC_IO_SetTtxData(unsigned long arg);
int DENC_IO_SetWssData(unsigned long arg);
int DENC_IO_SetVpsData(unsigned long arg);
int DENC_IO_ColorBarEnable(unsigned long arg);
int DENC_IO_VdacPowerControl(unsigned long arg);
int DENC_IO_MuteControl(unsigned long arg);
int DENC_IO_StripCaptureControl(unsigned long arg);

/**  @} */
