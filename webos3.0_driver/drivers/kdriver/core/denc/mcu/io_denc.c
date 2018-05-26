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
 *       Filename:  io_denc.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  3/17/2010 2:20:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jaemo.kim (),
 *        Company:  SIC, LG Electronics
 *
 * =====================================================================================
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#ifdef USE_XTENSA
#include <string.h>
#include <stdlib.h>
#include <BSP/UART/stdio.h>

#include <XDE/CFG_VIDEO.h>
#include <Common/DDI_DTV.h>
#include <Common/MEM_DTV.h>

#include <XDE/IPC_VIDEO.h>
#include <XDE/REG_DLR.h>
#include <XDE/MEM_VIDEO.h>
#include <XDE/MCU_VIDEO.h>
#include <XDE/de_kapi.h>
#include <XDE/IO_VIDEO.h>
#include <XDE/denc_kapi.h>
#else
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "os_util.h"

#include "base_types.h"

#include "../de/de_model.h"
#include "../de/de_def.h"
#include "../de/mcu/de_ipc_def.h"
#include "../de/mcu/de_ipc.h"

#include "denc_kapi.h"
#include "mcu/io_denc.h"

#endif

#ifdef USE_VIDEO_TEST_MCU_IN_CPU
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
DTV_STATUS_T VIDEO_DENC_SetTtx(UINT8 *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
DTV_STATUS_T MCU_DENC_IO_Init(char *strBuff)
{
	DTV_STATUS_T ret = OK;

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_OnOff(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	VIDEO_WCP_TYPE_T lVideoWCP;
	VIDEO_WCP_TYPE_T lVideoWCP_VCR;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
        ret = VIDEO_GetWinOprMode(&lVideoWCP);
		lVideoWCP_VCR = lVideoWCP;
		if (ret) break;
        switch (lVideoWCP) {
			case VIDEO_OPR_DSP_NOP :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_VCR;
				break;
			case VIDEO_OPR_DSP_PIP :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_PIP_VCR;
				break;
			case VIDEO_OPR_DSP_DVR :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_VCR_DVR;
				break;
			case VIDEO_OPR_DSP_BRD :
				if (!stParams) break;
				lVideoWCP = VIDEO_OPR_VCR_DVR;
				break;

			case VIDEO_OPR_DSP_VCR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_NOP;
				break;
			case VIDEO_OPR_VCR_DVR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_DVR;
				break;
			case VIDEO_OPR_PIP_VCR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_PIP;
				break;
			case VIDEO_OPR_BRD_VCR :
				if (stParams) break;
				lVideoWCP = VIDEO_OPR_DSP_BRD;
				break;
			case VIDEO_OPR_PIP_DVR :
				if (!stParams) break;
			default :
				BREAK_WRONG(lVideoWCP);
				break;
		}
		if (lVideoWCP_VCR == lVideoWCP) break;
		ret = VIDEO_SetWinOprMode(lVideoWCP);
		if (ret) break;
		ret = VIDEO_MCU_SetFormatWithPath(VIDEO_WIN_PATH_MDI, VIDEO_SSYNC_1920x1080_59P);
		if (ret) break;
		if (!DE_VCR_isComingFromDsp()) break;
		ret = VIDEO_SetWinPathWithType(VIDEO_WIN_VCR, VIDEO_WIN_PATH_MDI);
		if (ret) break;
		ret = VIDEO_MCU_SetSyncOfVCR(VIDEO_VCRSYNC_720X480I);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_SetOutPutVideoStd(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DENC_VIDEO_SYSTEM_T stParams;
	VIDEO_VCRSYNC_TYPE_T dType;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
		switch (stParams) {
			case LX_DENC_NTSC_M :
			case LX_DENC_NTSC_J :
			case LX_DENC_PAL_M :
				dType = VIDEO_VCRSYNC_720X480I;
				break;
			case LX_DENC_PAL_BG :
			case LX_DENC_PAL_N :
			case LX_DENC_SECAM :
				dType = VIDEO_VCRSYNC_720X576I;
				break;
			default :
				BREAK_WRONG(stParams);
		}
		if (ret) break;
		ret = VIDEO_MCU_SetSyncOfVCR(dType);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_SetSource(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DENC_VIDEO_SOURCE_T stParams;

	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_TtxEnable(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_WssEnable(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_VpsEnable(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_SetTtxData(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	VIDEO_TTX_PACK_T   ttxPack;
	do {
		memcpy(&ttxPack, strBuff, sizeof(ttxPack));
		ret = VIDEO_DENC_SetTtx(&ttxPack);
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_SetWssData(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DENC_WSS_DATA_T stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_SetVpsData(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	LX_DENC_VPS_DATA_T stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

DTV_STATUS_T MCU_DENC_IO_ColorBarEnable(char *strBuff)
{
	DTV_STATUS_T ret = OK;
#ifdef USE_XTENSA
	BOOLEAN stParams;
	do {
		memcpy(&stParams, strBuff, sizeof(stParams));
	} while (0);
#endif

	return ret;
}

#endif
