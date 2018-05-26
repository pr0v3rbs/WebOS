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

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define USE_GRAB_PIXEL_BY_CAPTURING_FRAME
#undef  USE_CVI_COPY_TO_NFS
#undef  USE_DSP_COPY_TO_NFS
#define USE_DE_WIN_BLANK_AT_MCU_PART
#define USE_DE_CVI_FIR_AT_MCU_PART
#define USE_DE_CVI_TPG_AT_MCU_PART
#define USE_DE_CVI_CSAMPLE_AT_MCU_PART
#ifdef USE_DE_CVI_FIR_AT_MCU_PART
#define USE_DE_CVI_FIR_FROM_HAL
#endif

#if defined(USE_CVI_COPY_TO_NFS) || defined(USE_DSP_COPY_TO_NFS)
#  define USE_FRAME_COPY_TO_NFS
#endif
/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_qkb.h"
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "base_types.h"
#include "os_util.h"

#include "de_model.h"
#include "de_ver_def.h"
#include "de_kapi.h"

#include "de_def.h"
#include "hal/ipc_hal.h"
#include "mcu/de_ipc_def.h"
#include "mcu/de_ipc.h"
#include "mcu/de_fw_def.h"
#include "mcu/de_fw.h"
//#include "mcu/ipc_cmd_header.h"
#ifdef USE_KDRV_CODES_FOR_L9
#include "l9/de/de_cfg_l9.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13
#include "h13/de/de_cfg_h13.h"
#endif
#include "de_hal.h"
#include "de_io.h"
#include "de_drv.h"
#ifdef INCLUDE_KDRV_VENC
#include "venc_cfg.h"
#endif
#include "pwm_hal.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------i-----------------------------------------------------------------*/
#define CP_FROM_USER(a,b,c)    if(!io_resume){ret = copy_from_user(a,b,c);if(ret)break;}\
	                           else{memcpy(a,b,c);}
#define CP_TO_USER(a,b,c)      if(!io_resume){ret = copy_to_user(a,b,c);if(ret)break;}\
	                           else{memcpy(a,b,c);}

#define MCU_Write(a,b,c) h_ipc->m_ipc_client->write( a , b , c )
#define MCU_Read(a,b,c)    h_ipc->m_ipc_client->read( a , b , c )
#define CHECK_MCU_IPC()  if(h_ipc == NULL                      ||\
							h_ipc->m_ipc_client->write == NULL ||\
							h_ipc->m_ipc_client->read  == NULL)  \
							{DE_ERROR("ipc handler is null\n");  \
							ret = RET_ERROR;return ret;}
#define CHECK_IO_RET()   if(ret != RET_OK) \
							{DE_ERROR("RET_ERROR\n");}

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_KDRV_VENC
extern LX_VENC_MEM_CFG_S_T *gpstVencMemConfig;
#endif

LX_DE_VTM_FRAME_INFO_T gVtmParams;
/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
struct ipc_handle *h_ipc = NULL;
char io_resume = FALSE;
LX_DE_CAPACITY_T g_capa;


/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
static void(*g_DEListener)(unsigned long) = NULL;

/**
 * @brief register DE event listener (temporary workaround only by raxis.lim (2014/10/22)
 *
 */
void DE_IO_AddListener( void(*fn)(unsigned long arg) )
{
	g_DEListener = fn;
}

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
int DE_IO_Resume(unsigned long arg)
{
	if(arg) io_resume = TRUE;
	else    io_resume = FALSE;
	return RET_OK;
}

int DE_IO_IPC_Init(unsigned long arg)
{
	int ret = RET_OK;

	ret = DE_HAL_Select_Chip();
	if(ret) return ret;

	memset(&g_capa, 0, sizeof(LX_DE_CAPACITY_T));
	ret = DE_HAL_GetCapacity(&g_capa);
	if(ret) return ret;

	ret = ipc_hal_init();
	if(ret) return ret;

	h_ipc = ipc_hal_open("de", MCU_ID_0);
	if(!h_ipc)
	{
		ret = RET_ERROR;
		DE_ERROR("ipc_hal open fail !\n");
	}

	DE_PRINT("mcu ipc handle = 0x%08x\n", (UINT32)h_ipc);

	return ret;
}

int DE_IO_IPC_Cleanup(unsigned long arg)
{
	ipc_hal_close(h_ipc);
	return RET_OK;
}


/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize de moudle
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_Init(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_PANEL_TYPE_T stParams;
	UINT32 platformVersion;
	DE_IPC_CMD_T ipcCmd;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ipcCmd = DE_IPC_DE_INIT;
		if (lx_chip_rev( ) >= LX_CHIP_REV(H15,A0)) {
			stParams = LX_PANEL_TYPE_3840;
		} else {
			stParams = LX_PANEL_TYPE_1920;
		}
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_Init(&stParams);
		if (ret) break;
		ipcCmd = DE_IPC_SET_PLATFORM_VERSION;
		platformVersion = lx_chip_rev();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &platformVersion, sizeof(platformVersion));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get Information of firmware
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetFirmwareInfo(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_FIRMWARE_INFO_T stParams;
	LX_DE_FIRMWARE_INFO_T *pstParams = (LX_DE_FIRMWARE_INFO_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_FIRMWARE_INFO;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(LX_DE_FIRMWARE_INFO_T));
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set background color (non-active region color).
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetBgColor(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_COLOR_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_BG_COLOR;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_SetBgColor(&stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set blank of selected window and blank color
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetWinBlank(unsigned long arg)
{
	int ret = RET_OK;
#ifdef USE_DE_WIN_BLANK_AT_MCU_PART
	LX_DE_SET_WIN_BLANK_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_WIN_BLANK;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_SET_WIN_BLANK_T));
	} while(0);
#else
	LX_DE_SET_WIN_BLANK_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetWinBlank(&stParams);
	} while (0);
#endif
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get Information of selected window about size and offset of input window
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetInputWin(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_WIN_DIMENSION_T stParams;
	LX_DE_WIN_DIMENSION_T *pstParams = (LX_DE_WIN_DIMENSION_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_INPUT_WIN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_WIN_DIMENSION_T));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(LX_DE_WIN_DIMENSION_T));
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get Information of selected window about size and offset of output window
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetOutWin(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_WIN_DIMENSION_T stParams;
	LX_DE_WIN_DIMENSION_T *pstParams = (LX_DE_WIN_DIMENSION_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_OUT_WIN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_WIN_DIMENSION_T));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(LX_DE_WIN_DIMENSION_T));
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information of selected window about size and offset of input window
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetInputWin(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_WIN_DIMENSION_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_INPUT_WIN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set Information of selected window about size and offset of output window
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetOutWin(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_WIN_DIMENSION_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_OUT_WIN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Determine to make Display turn On or Off
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetDisOut(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_DIS_OUT;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Determine selected multi window to turn On or Off
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_MultiWinEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_MULTI_WIN_ENALBE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(BOOLEAN));
	} while(0);
	CHECK_IO_RET();
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
int DE_IO_SetCviSrcType(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CVI_SRC_TYPE_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_SRC_TYPE;
#ifdef USE_DE_CVI_FIR_FROM_HAL
	LX_DE_CVI_FIR_T fir;
	DE_IPC_CMD_T ipcCmd_fir = DE_IPC_SET_CVI_FIR;
#endif

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_DE_CVI_FIR_FROM_HAL
		ret = DE_HAL_GetFIR(&stParams, &fir);
		if (ret) break;
		ret = MCU_Write(DE_PACK_CMD(ipcCmd_fir), &fir, sizeof(fir));
		if (ret) break;
		ret = DE_HAL_SetCviFir(&fir);
		if (ret) break;
#endif
       	ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	       if (ret) break;
       	ret = DE_HAL_SetCviSrcType(&stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get MVI source colorimetry information.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetMviColorimetry(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_MVI_COLORIMETRY_T stParams;
	LX_DE_MVI_COLORIMETRY_T *pstParams = (LX_DE_MVI_COLORIMETRY_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_MVI_COLORIMETRY;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_MVI_COLORIMETRY_T));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(LX_DE_MVI_COLORIMETRY_T));
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		ret = DE_HAL_GetMviColorimetry(&stParams);  // NULL
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief determine to make selected surface to turn On or Off
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetZList(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_ZLIST_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_ZLIST;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_ZLIST_T));
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set display pannel formats which are pannel type and frame rate.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetDisFmt(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_DIS_FMT_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_DIS_FMT;
	LX_DE_FR_RATE_T frRate;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_Init(&stParams.panel_type);
		if (ret) break;
		ret = DE_HAL_GetFrRate(&frRate);
		if (ret) break;
		frRate.fr_rate = stParams.fr_rate;
		ret = DE_HAL_SetFrRate(&frRate);

		/* Set PWM frequency */
		if (lx_chip_rev( ) >= LX_CHIP_REV(H15,A0) )
		{
			UINT32 pwmFreq;

			if (frRate.fr_rate == 25 || frRate.fr_rate == 50)
			{
				pwmFreq = 50;
			}
			else
			{
				pwmFreq = 60;
			}
			BE_PWM_HAL_SetPwmSync(pwmFreq);
		}
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set display frame rate.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetOutFrRate(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_FR_RATE_T stParams;
	LX_DE_FR_RATE_T *pstParams = (LX_DE_FR_RATE_T *)arg;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_GetFrRate(&stParams);
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set display pannel formats which are pannel type and frame rate.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetFrRate(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_FR_RATE_T stParams;
#ifdef USE_SUPPORT_EUROPE
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_FR_RATE;
#endif

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_SUPPORT_EUROPE
		CHECK_MCU_IPC();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
#endif
		ret = DE_HAL_SetFrRate(&stParams);

		/* Set PWM frequency */
		if (lx_chip_rev( ) >= LX_CHIP_REV(H15,A0) )
		{
			UINT32 pwmFreq;

			if (stParams.fr_rate == 25 || stParams.fr_rate == 50)
			{
				pwmFreq = 50;
			}
			else
			{
				pwmFreq = 60;
			}
			BE_PWM_HAL_SetPwmSync(pwmFreq);
		}
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief de cvi video frame buffer freeze to read/write video frame before cvi csc parameter for self-diagnosis.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCviFreeze(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CVI_CAPTURE_FREEZE_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_FREEZE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
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
int DE_IO_SetCviVideoFrameBuffer(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CVI_RW_VIDEO_FRAME_T stParams;
	LX_DE_CVI_RW_VIDEO_FRAME_T frameInfo;
	LX_DE_CVI_RW_PREW_FRAME_T  prewInfo;
	LX_DE_GRAB_COLOR_T         *pFrameColor = NULL;
	UINT32                     phyFrameRows;
	UINT32                     phyFrameAddr = 0;
	UINT32                     frameSize;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_VIDEO_FRAME_BUFFER;
#ifdef USE_FRAME_COPY_TO_NFS
	OS_FILE_T   fw_file;
	char filePath[200];
	char fileName[100];
#else
	LX_DE_CVI_RW_VIDEO_FRAME_T *pstParams = (LX_DE_CVI_RW_VIDEO_FRAME_T*)arg;
#endif

	memset(&frameInfo, 0, sizeof(LX_DE_CVI_RW_VIDEO_FRAME_T));
	memset(&stParams, 0, sizeof(LX_DE_CVI_RW_VIDEO_FRAME_T));

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(LX_DE_CVI_RW_VIDEO_FRAME_T));

		if (lx_chip_rev( ) >= LX_CHIP_REV(M14,B0) ) {
			prewInfo.win_id = stParams.region.win_id;
			prewInfo.frameAddr.phyFrameAddrY8 = 0;
			ipcCmd = DE_IPC_SET_CVI_PIXEL_CAPTURE;
			ret = MCU_Write(DE_PACK_CMD(ipcCmd), &prewInfo, sizeof(prewInfo));
			if (ret) break;
			ret = MCU_Read(0, &prewInfo, sizeof(prewInfo));
			if (ret) break;
			DE_TRACE("win[%d] addrY8[%08x] addrC8[0x%08x] csample[%d] x/y/w/h[%d/%d/%d/%d]", \
				prewInfo.win_id, prewInfo.frameAddr.phyFrameAddrY8, prewInfo.frameAddr.phyFrameAddrC8,\
				prewInfo.csample, prewInfo.rect.x, prewInfo.rect.y, prewInfo.rect.w, prewInfo.rect.h );
			if(prewInfo.frameAddr.phyFrameAddrY8 == 0)
				BREAK_WRONG(ret);
			if(stParams.region.pixelGrabW > 1920 \
			|| stParams.region.pixelGrabH > 1080)
				break;
			frameSize    = stParams.region.pixelGrabW * \
						   stParams.region.pixelGrabH * \
						   sizeof(LX_DE_GRAB_COLOR_T);
			memcpy(&frameInfo, &stParams,\
					sizeof(LX_DE_CVI_RW_VIDEO_FRAME_T));
			pFrameColor = (LX_DE_GRAB_COLOR_T *)kmalloc(frameSize,\
					GFP_KERNEL);
			if(pFrameColor == NULL) break;
			frameInfo.color = pFrameColor;

			if(pstParams->bReadOnOff) {
				ret = DE_HAL_SetCviRWFrameBuffer(&frameInfo, &prewInfo);
				CP_TO_USER((void *)stParams.color, pFrameColor, frameSize);
			} else {
				CP_FROM_USER(pFrameColor, (void *)stParams.color, frameSize);
				ret = DE_HAL_SetCviRWFrameBuffer(&frameInfo, &prewInfo);
			}

			stParams.region.realPixelGrabW = frameInfo.region.realPixelGrabW;
			stParams.region.realPixelGrabH = frameInfo.region.realPixelGrabH;
			stParams.region.colorSpace     = frameInfo.region.colorSpace;

			CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
			if(pFrameColor)
			{
				kfree(pFrameColor);
				pFrameColor = NULL;
			}
			break;
		}

#ifdef USE_FRAME_COPY_TO_NFS
		stParams.region.pixelGrabW = 1920;
		stParams.region.pixelGrabH = 1080;
		stParams.region.pixelGrabX = 0;
		stParams.region.pixelGrabY = 0;
		stParams.bReadOnOff = 1;
#ifdef USE_DSP_COPY_TO_NFS
		stParams.region.win_id = LX_DE_WIN_MAX;
		stParams.region.colorDepth = 0;
#endif
#endif
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		ret = MCU_Read(0, &frameInfo, sizeof(frameInfo));
		if (ret) break;
		frameSize    = stParams.region.pixelGrabW * stParams.region.pixelGrabH * sizeof(LX_DE_GRAB_COLOR_T);
		phyFrameRows = GET_BITS(frameInfo.region.pixelGrabY ,16,16);
#ifdef USE_KDRV_CODES_FOR_H13
		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,A0) ) {
#ifdef USE_FRAME_COPY_TO_NFS
			strcpy(filePath, "/home/jaemo.kim/res/lglib/kdrv/");
#endif
			phyFrameAddr = CONV_MEM_ROW2BYTE(phyFrameRows) | GET_SVAL(frameInfo.region.colorDepth,4,4,28);
		} else
#endif
#ifdef USE_KDRV_CODES_FOR_L9
		if (lx_chip_rev( ) >= LX_CHIP_REV(L9,A0) ) {
#ifdef USE_FRAME_COPY_TO_NFS
			strcpy(filePath, "/mnt/lg/res/lglib/kdrv/");
#endif
			phyFrameAddr = VIDEO_L9_ROW2PHY(phyFrameRows);
		} else
#endif
		{
		}
		if(!phyFrameAddr) break;
		pFrameColor = (LX_DE_GRAB_COLOR_T *)ioremap(phyFrameAddr, frameSize);
		CHECK_KNULL(pFrameColor);
		frameInfo.color = pFrameColor;
		frameInfo.bReadOnOff |= GET_BITS(stParams.bReadOnOff,0,1);
		if (GET_BITS(stParams.bReadOnOff,0,1)) {
			ret = DE_HAL_SetCviVideoFrameBuffer(&frameInfo);
			if (ret) break;
#ifdef USE_FRAME_COPY_TO_NFS
			sprintf(fileName, "Captured_Video__%dx%d.444_60p.img", stParams.region.pixelGrabW, stParams.region.pixelGrabH);
			strcat(filePath, fileName);
			if ( RET_OK != OS_OpenFile( &fw_file, filePath, O_CREAT|O_RDWR|O_LARGEFILE, 0666 ) )
			{
				DE_ERROR("<error> can't open fw_file (%s)\n", filePath);
				BREAK_WRONG(ret);
			}
			ret = OS_WriteFile(&fw_file, (char *)pFrameColor, frameSize);
			if (ret != frameSize) BREAK_WRONG(ret);
			ret = OS_CloseFile( &fw_file );
			if (ret) BREAK_WRONG(ret);
#else
			CP_TO_USER((void *)stParams.color, pFrameColor, frameSize);
			if (ret) BREAK_WRONG(ret);
#endif
		} else {
			CP_FROM_USER(pFrameColor, (void *)stParams.color, frameSize);
			ret = DE_HAL_SetCviVideoFrameBuffer(&frameInfo);
		}
		stParams.region.realPixelGrabW = frameInfo.region.realPixelGrabW;
		stParams.region.realPixelGrabH = frameInfo.region.realPixelGrabH;
		stParams.region.colorSpace     = frameInfo.region.colorSpace;
#ifndef USE_FRAME_COPY_TO_NFS
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (ret)  BREAK_WRONG(ret);
#endif
	} while (0);
	if(pFrameColor) iounmap(pFrameColor);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Write Register
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_RegWr(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_REG_WR_T *pstParams = (LX_DE_REG_WR_T*)arg;
	LX_DE_REG_WR_T lReg;
	UINT32 msgCnt = sizeof(LX_DE_REG_WR_T);
#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
	DE_IPC_CMD_T ipcCmd = DE_IPC_REG_WD;
#endif

	do {
		CP_FROM_USER(&lReg, (void *)pstParams, msgCnt);
		ret = DE_HAL_WriteReg(lReg.wr_addr, lReg.wr_data);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Read Register
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_RegRd(unsigned long arg)
{
	int ret = RET_OK;
	UINT32 *pstParams = (UINT32*)arg;
	LX_DE_REG_WR_T lReg;
#ifdef USE_IPC_CHECK_TEMPORAL_DEFINITION
	DE_IPC_CMD_T ipcCmd = DE_IPC_REG_RD;
#endif

	do {
		CP_FROM_USER(&lReg.wr_addr, (void *)pstParams, sizeof(UINT32));
		ret = DE_HAL_ReadReg(lReg.wr_addr, &lReg.wr_data);
		if (ret) BREAK_WRONG(ret);
		CP_TO_USER((void *)pstParams, &lReg.wr_data, sizeof(UINT32));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set deinterlace on/off control
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetDeinterlace(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN  stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_DEINTERLACE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
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
int DE_IO_SetUart4MCU(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN  stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetUart4MCU(stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief control de UD mode (on/off)
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetUDMode(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN  stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_UD_MODE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_SetUdMode(&stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}


/**
 * @callgraph
 * @callergraph
 *
 * @brief set de CVE parameter.
 *
 * @param arg [OUT] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCve(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CVE_PARAM_T  stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while(0);
	CHECK_IO_RET();
	return ret;
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
int DE_IO_SetVcs(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_VCS_IPC_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetVcs(&stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
int DE_IO_FW_Download(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_FW_DWLD_T stParams;
	char *lpData = NULL;

	/* notify DE event */
	if (g_DEListener) g_DEListener(0x0);

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(LX_DE_FW_DWLD_T));
		if (!stParams.inx)  {
			ret = DE_FW_Download(FALSE);
			if(ret == RET_TIMEOUT)
			{
				DE_ERROR("<<<error>>> de firmware is not working\n");
				BREAK_WRONG(ret);
			}
			if(ret == RET_OK) ipc_hal_start(0, TRUE);
			break;
		}
		if (!stParams.size) break;
		if (stParams.size > 16*1024*1024 || stParams.size < 0) break;
		if (!stParams.pData) break;
		lpData = OS_KMalloc(stParams.size);
		CHECK_KNULL(lpData);
		CP_FROM_USER(lpData, (void *)stParams.pData, stParams.size);
		stParams.pData = lpData;
		ret = DE_HAL_FW_Download(&stParams);
		if(ret == RET_OK) ipc_hal_start(0, TRUE);
	} while (0);
	OS_Free(lpData);
	CHECK_IO_RET();
	return ret;
}
#endif

/**
 * @callgraph
 * @callergraph
 *
 * @brief send captured video fir coef for double/quad sampling case.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCviFir(unsigned long arg)
{
	int ret = RET_OK;
#ifndef USE_DE_CVI_FIR_FROM_HAL
	LX_DE_CVI_FIR_T stParams;
#ifdef USE_DE_CVI_FIR_AT_MCU_PART
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_FIR;
#endif

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_DE_CVI_FIR_AT_MCU_PART
		CHECK_MCU_IPC();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
#else
		ret = DE_HAL_SetCviFir(&stParams);
#endif
	} while (0);
#endif // #ifndef USE_DE_CVI_FIR_FROM_HAL
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set captured video test pattern generator to black.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCviTpg(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CVI_TPG_T stParams;
#ifdef USE_DE_CVI_TPG_AT_MCU_PART
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_TPG;
#endif

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_DE_CVI_TPG_AT_MCU_PART
		CHECK_MCU_IPC();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
#else
		ret = DE_HAL_SetCviTpg(&stParams);
#endif
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set captured video color sampling mode(sub sampling or 3 tap filtering).
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCviCsampleMode(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CSAMPLE_MODE_T stParams;
#ifdef USE_DE_CVI_CSAMPLE_AT_MCU_PART
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_CSAMPLE_MODE;
#endif

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_DE_CVI_CSAMPLE_AT_MCU_PART
		CHECK_MCU_IPC();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
#else
		ret = DE_HAL_SetCviCsampleMode(&stParams);
#endif
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set dvr control.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetDvrFmtCtrl(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_DVR_DISFMT_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_DVR_FMT_CTRL;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef INCLUDE_KDRV_VENC
		stParams.baseAddr = (gpstVencMemConfig)?gpstVencMemConfig->uiInBufBase:0x0;
#endif
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set frame caputuring mode On or Off.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetSubScFreeze(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_SUB_SC_FREEZE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief select source for multi window.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SelectMultiWinSrc(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_MULTI_WIN_SRC_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SELECT_MULTI_WIN_SRC;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_SelectMultiWinSrc(&stParams);
		if (ret) break;
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief select between SCART RGB bypass mode and CVD overlay mode
 *
 * @param arg [IN] bypass enable/disable
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetScartRGBBypassMode(unsigned long arg)
{
	int ret = RET_OK;
	int Enable;

	do {
		CP_FROM_USER(&Enable, (void *)arg, sizeof(int));
		//ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		//if (ret) break;
		ret = DE_HAL_SetSCARTRGBBypassMode(&Enable);
		if (ret) break;
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get debug data.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetDebug(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SET_DBG_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_GetDebug(&stParams);
		if (ret) break;
		CP_TO_USER((void *)arg, &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set debug data.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetDebug(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SET_DBG_T stParams;
#ifdef USE_DE_CVI_DELAY_ON_MCU_PART
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CVI_DELAY;
#endif

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
#ifdef USE_DE_CVI_DELAY_ON_MCU_PART
		CHECK_MCU_IPC();
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
#endif
		ret = DE_HAL_SetDebug(&stParams);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set low delay mode for frame delay
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetLowDelay(unsigned long arg)
{
    int ret = RET_OK;
    UINT32 stParams;
    DE_IPC_CMD_T ipcCmd = DE_IPC_SET_LOW_DELAY;

    do {
		CHECK_MCU_IPC();
        CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
        ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
    } while (0);
	CHECK_IO_RET();
    return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get low delay mode for frame delay
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetLowDelay(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_LOW_DELAY_T stParams;
	LX_DE_LOW_DELAY_T *pstParams = (LX_DE_LOW_DELAY_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_LOW_DELAY;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(stParams));
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set inforamtion for Multi vision
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetMultiVision(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_RECT_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_MULTI_VISION;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set information for video to memory
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetVTM(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_VTM_FRAME_INFO_T stParams;
	LX_DE_VTM_FRAME_INFO_T *pstParams = (LX_DE_VTM_FRAME_INFO_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_FRAME_FOR_WEBOS;
	char *p_shared_mem = NULL;
	int vtm_ready_count = 100;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		CP_FROM_USER(&gVtmParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetVTM(&stParams);
		if (ret) BREAK_WRONG(ret);
		p_shared_mem = ioremap(stParams.address, \
				sizeof(LX_DE_VTM_FRAME_INFO_T));
		if(p_shared_mem == NULL) BREAK_WRONG(ret);
		memset(p_shared_mem, 0, sizeof(LX_DE_VTM_FRAME_INFO_T));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		if(stParams.enable)
		{
			while(vtm_ready_count-- > 0)
			{
				LX_DE_VTM_FRAME_INFO_T *p = (LX_DE_VTM_FRAME_INFO_T *)p_shared_mem;
				if(p->y_frame[0] != 0) break;
				OS_MsecSleep(10);
			}
			if(vtm_ready_count <= 0) DE_WARN("not-ready-vtm\n");
			DE_TRACE("VTM ready count [%d] remains\n", vtm_ready_count);
		}
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (pstParams->stride > 2048 || pstParams->r_idx > 2 || pstParams->winSize.w > 1920 || pstParams->winSize.h > 1080)
		{
			DE_PRINT("skip this captured frame. (unstable frame buffer status)");
			return -2;
		}
		if (ret) BREAK_WRONG(ret);
	} while (0);
	if(p_shared_mem) iounmap(p_shared_mem);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get information for video to memory
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetVTM(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_VTM_FRAME_INFO_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		CP_TO_USER((void *)arg, &gVtmParams, sizeof(stParams));
#if  0 // Resource_mgr.c
		if(stParams.win_id  ==  0)
			ret = VIDEO_WEL_WaitVsync();
		else if(stParams.win_id  ==  1)
			ret = VIDEO_WER_WaitVsync();
		else
			ret = RET_INVALID_PARAMS;
#endif
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set 3D input/output control (onoff, format, lr order)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 **/
int DE_IO_Set3DInOutCtrl(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_3D_INOUT_CTRL_T stParams; // 20130327,  check this with mcu's kapi !!!
	DE_IPC_CMD_T ipcCmd = FMT3D_IPC_SET_INOUT_CTRL;
	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(FMT3D_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetInterfaceConfig(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_IF_CONFIG_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_IF_CONFIG;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = DE_HAL_SetInterfaceConfig(&stParams);
		if (ret) break;
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetSystemStatus(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SYS_STATUS_T stParams;
	LX_DE_SYS_STATUS_T *pstParams = (LX_DE_SYS_STATUS_T *)arg;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_GetSystemStatus(&stParams);
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set interface configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetSourceStatus(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SRC_STATUS_T stParams;
	LX_DE_SRC_STATUS_T *pstParams = (LX_DE_SRC_STATUS_T *)arg;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_GetSourceStatus(&stParams);
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}



/**
 * @callgraph
 * @callergraph
 *
 * @brief Set operation infomation
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetOperation(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_OPERATION_CTRL_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetOperation(&stParams);
		if (ret) break;
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set window configuration
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetSubOperation(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SUB_OPERATION_CTRL_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetSubOperation(&stParams);
		if (ret) break;
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set capture control
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetCaptureControl(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CAPTURE_CTRL_T stParams;
	LX_DE_CAPTURE_CTRL_T *pstParams = (LX_DE_CAPTURE_CTRL_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_CAPTURE;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		//		ret = DE_HAL_SetCaptureControl(&stParams);
		//		if (ret) break;
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(stParams));
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get scaler info
 *
 * @param arg [IN] win idx
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetScalerInfo(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SCALER_INFO_T stParams;
	LX_DE_SCALER_INFO_T *pstParams = (LX_DE_SCALER_INFO_T*)arg;
	DE_IPC_CMD_T ipcCmd = DE_IPC_GET_SCALER_INFO;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
		ret = MCU_Read(0, &stParams, sizeof(stParams));
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Set inner pattern
 *
 * @param arg [IN] pattern information
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetInnerPattern(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SCALER_INFO_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_INNER_PATTERN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_INNER_PATTERN_T));
	} while (0);
	CHECK_IO_RET();
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
#ifdef INCLUDE_KDRV_APR
// 20130222 maybe, this is unnecessary because of mute-control
#include "../apr/apr_core.h"
#endif

int DE_IO_SetWinPortSrc(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_INPUT_SOURCE_T stParams;
	LX_DE_INPUT_CFG_T stParams1; // old structure
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_WIN_PORT_SRC;
#ifdef INCLUDE_KDRV_APR
	LX_APR_CONTROL_T aprParams;
#endif
	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		stParams1.win_id       = stParams.srcId;
		stParams1.inputSrc     = stParams.srcType;
		stParams1.inputSrcPort = stParams.srcPort;

		ret = DE_HAL_SetWinPortSrc(&stParams1);
		if (ret) break;
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(stParams));
		if (ret) break;
#ifdef INCLUDE_KDRV_APR
		aprParams.cmd     = 1;
		aprParams.sel_alg = 1;
		aprParams.cap_enb = 0;
		aprParams.fd3_enb = 1;
		aprParams.fmt     = 0;
		APR_Core_SetControl(&aprParams);
#endif
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Determine to make selected window Freeze or Not with location info
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetWinFreeze(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_WIN_FREEZE_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_WIN_FREEZE;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_WIN_FREEZE_T));
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief verify firmware from ddr memory
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_VerifyFirmware(unsigned long arg)
{
	int ret = RET_OK;

	do {
		ret = DE_HAL_VerifyFirmware();
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set mirror output to denc (for scart-out)
 *
 * @param arg [IN]
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetSrcMirror(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SRC_MIRROR_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_SRC_MIRROR;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_SRC_MIRROR_T));
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set actual format detecting (for seamless switching with external specific input)
 *
 * @param arg [IN]
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetActFmtDetect(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_ACT_FORMAT_T stParams;
	BOOLEAN  ud_mode = FALSE;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_ACT_FMT_DETECT;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_ACT_FORMAT_T));
		if(stParams.enable == 3) // amazon mode
			ud_mode = TRUE;
		ret = DE_HAL_SetUdMode(&ud_mode);
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set hdmi port for window
 *
 * @param arg [IN]
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetHDMIPort(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_HDMIPORT_T stParams;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_SetHDMIPort(&stParams);
		if (ret) break;
	} while (0);
	CHECK_IO_RET()
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Get capacity of chip
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_GetCapacity(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_CAPACITY_T stParams;
	LX_DE_CAPACITY_T *pstParams = (LX_DE_CAPACITY_T *)arg;

	do {
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = DE_HAL_GetCapacity(&stParams);
		if (ret) break;
		CP_TO_USER((void *)pstParams, &stParams, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
	} while (0);
	CHECK_IO_RET();
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set sub-window index (0 : main or ignore, equal or greater than 1 : sub window)
 *
 * @param arg [IN]
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DE_IO_SetSubWindow(unsigned long arg)
{
	int ret = RET_OK;
	LX_DE_SET_SUB_WIN_T stParams;
	DE_IPC_CMD_T ipcCmd = DE_IPC_SET_MULTI_WIN;

	do {
		CHECK_MCU_IPC();
		CP_FROM_USER(&stParams, (void *)arg, sizeof(stParams));
		ret = MCU_Write(DE_PACK_CMD(ipcCmd), &stParams, sizeof(LX_DE_SET_SUB_WIN_T));
	} while(0);
	CHECK_IO_RET();
	return ret;
}

/**  @} */
