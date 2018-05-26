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

#ifndef _IO_VIDEO_h
#define _IO_VIDEO_h

#ifdef USE_LINUX_KERNEL
#define OK     RET_OK
#define NOT_OK RET_ERROR
#define DTV_STATUS_T int
#endif

DTV_STATUS_T MCU_VIDEO_IPC_PRINT(const char *file, const int line, const char *fn, char *cliBuff, UINT32 strLen) DE_CODEDDR;
DTV_STATUS_T MCU_VIDEO_IPC_IOCTL(char *cliBuff, UINT32 strLen) DE_CODEDDR;

DTV_STATUS_T MCU_DE_IO_Init(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetFirmwareInfo(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetInputSrc(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetCviSrcType(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetCviCsc(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetPostCsc(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetMviColorimetry(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetInputWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetInputWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetOutWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetOutWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetDisOut(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetDisFmt(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetFrRate(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetFreeze(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetZList(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetMultiWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_MultiWinEnable(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GrabPixel(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetMviTimingInfo(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetDeinterlace(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetCaptureWin(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetCaptureWin(char *strBuff) DE_CODEDDR;
//DTV_STATUS_T MCU_DE_IO_Set3DSrcMux(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetPre3DInMode(char *strBuff) DE_CODEDDR;
//DTV_STATUS_T MCU_DE_IO_SetPre3DOutMode(char *strBuff) DE_CODEDDR;
//DTV_STATUS_T MCU_DE_IO_SetPre3DInCH(char *strBuff) DE_CODEDDR;
//DTV_STATUS_T MCU_DE_IO_SetPre3DEnable(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_Set3DOprMode(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetPwm(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetCve(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetVcs(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_GetOutFrRate(char *strBuff) DE_CODEDDR;
#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
DTV_STATUS_T MCU_DE_IO_SetBgColor(char *strBuff) DE_CODEDDR;
DTV_STATUS_T MCU_DE_IO_SetWinBlank(char *strBuff) DE_CODEDDR;
#endif
DTV_STATUS_T MCU_SetPanelType(LX_DE_DIS_FMT_T *pstParams) DE_CODEDDR;
DTV_STATUS_T DE_GetPathOfCVE(LX_DE_CVE_PARAM_T *pDePathOfCve);

#endif
