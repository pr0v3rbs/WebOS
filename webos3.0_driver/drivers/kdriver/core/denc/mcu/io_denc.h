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

/*
 * =====================================================================================
 *
 *       Filename:  io_denc.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  3/17/2010 2:24:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jaemo.kim (),
 *        Company:  SIC, LG Electronics
 *
 * =====================================================================================
 */

#ifndef  IO_DENC_INC
#define  IO_DENC_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#ifdef USE_LINUX_KERNEL
#define OK     RET_OK
#define NOT_OK RET_ERROR
#define DTV_STATUS_T int
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
DTV_STATUS_T MCU_DENC_IO_Init(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_OnOff(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_SetOutPutVideoStd(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_SetSource(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_TtxEnable(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_WssEnable(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_VpsEnable(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_SetTtxData(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_SetWssData(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_SetVpsData(char *strBuff);
DTV_STATUS_T MCU_DENC_IO_ColorBarEnable(char *strBuff);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef IO_DENC_INC  ----- */
