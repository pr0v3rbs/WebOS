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
#define USE_DENC_TURN_ON_ONTY_MCU_PART
#define USE_DENC_TTXDATA_LOADED_AT_MCU
#define USE_DENC_TTXDATA_ONLY_CPU_PART
#define USE_DENC_VPSDATA_ONLY_CPU_PART
#undef  USE_DENC_WSSDATA_ONLY_CPU_PART
#undef  USE_DENC_TTXDATA_BY_DDR
//#define USE_DENC_TTXDATA_BY_SHARED_MEM

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "base_types.h"
#include "os_util.h"

#ifdef INCLUDE_KDRV_DE
#include "../de/de_model.h"

#include "../de/de_def.h"
#include "../de/mcu/de_ipc_def.h"
#include "../de/mcu/de_ipc.h"
#include "../de/hal/ipc_hal.h"
#endif

#include "denc_kapi.h"
#include "denc_drv.h"
#include "denc_io.h"
#include "denc_hal.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_KDRV_DE
#define MCU_Write(a,b,c) h_ipc->m_ipc_client->write( DENC_PACK_CMD(a) , b , c )
#define MCU_Read(a,b)    h_ipc->m_ipc_client->read( a , b )
#define MCU_Wrmem(a,b,c) h_ipc->m_ipc_client->wrmem( DENC_PACK_CMD(a) , b , c )
#define CHECK_MCU_IPC()  if(h_ipc == NULL                      ||\
							h_ipc->m_ipc_client->write == NULL ||\
							h_ipc->m_ipc_client->read  == NULL)  \
							{ret = RET_ERROR;return ret;}
#endif

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
struct denc_info {
	BOOLEAN vcr_en;
	BOOLEAN dac_en;
	BOOLEAN ttx_en;
	BOOLEAN vps_en;
	BOOLEAN wss_en;
	LX_DENC_VIDEO_SYSTEM_T std;
	LX_DENC_VIDEO_SOURCE_T src;

	UINT16 wss;
	UINT8 vps[16];
};

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_KDRV_DE
static struct ipc_handle *h_ipc = NULL;
#endif

struct denc_info denc_info;

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
int DENC_IO_IPC_Init(unsigned long arg)
{
	int ret = RET_OK;
	
#ifdef INCLUDE_KDRV_DE
	ret = ipc_hal_init();
	if(ret) return ret;

	h_ipc = ipc_hal_open("denc", MCU_ID_0);
	if(!h_ipc)
	{
		ret = RET_ERROR;
		return ret;
	}
#endif
	DENC_TRACE("done.\n");
	return ret;
}

int DENC_IO_IPC_Cleanup(unsigned long arg)
{
#ifdef INCLUDE_KDRV_DE
	ipc_hal_close(h_ipc);
#endif
	DENC_TRACE("done.\n");
	return RET_OK;
}

int DENC_IO_Resume(void)
{
	int ret = RET_OK;
	DE_IPC_CMD_T ipcCmd;
	BOOLEAN dacEnable;
	LX_DENC_WSS_DATA_T wss_data;
	LX_DENC_VPS_DATA_T vps_data;

	do {
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		
		ipcCmd = DENC_IPC_INIT;
		ret = MCU_Write(ipcCmd, NULL, 0);
		if (ret) break;
		ret = DENC_HAL_Init();

		ipcCmd = DENC_IPC_ONOFF;
		ret = MCU_Write(ipcCmd, &denc_info.vcr_en, sizeof(denc_info.vcr_en));
		if (ret) break;
		dacEnable = (denc_info.dac_en || denc_info.vcr_en)?TRUE:FALSE;
		ret = DENC_HAL_VdacPowerControl(&dacEnable);
		if (ret) break;

		ipcCmd = DENC_IPC_SET_OUTPUT_VIDEO_STD;
		ret = MCU_Write(ipcCmd, &denc_info.std, sizeof(denc_info.std));
		if (ret) break;
		ret = DENC_HAL_SetOutPutVideoStd(&denc_info.std);
		if (ret) break;
		
		ipcCmd = DENC_IPC_SET_SOURCE;
		ret = MCU_Write(ipcCmd, &denc_info.src, sizeof(denc_info.src));
		if (ret) break;
		ret = DENC_HAL_SetSource(&denc_info.src);
		if (ret) break;

		ipcCmd = DENC_IPC_WSS_ENABLE;
		ret = MCU_Write(ipcCmd, &denc_info.wss_en, sizeof(denc_info.wss_en));

		ipcCmd = DENC_IPC_VPS_ENABLE;
		ret = DENC_HAL_VpsEnable(&denc_info.vps_en);

		ipcCmd = DENC_IPC_SET_WSS_DATA;
		wss_data.Enable   = denc_info.wss_en;
		wss_data.WSS_DATA = denc_info.wss;
		ret = MCU_Write(ipcCmd, &wss_data, sizeof(wss_data));

		ipcCmd = DENC_IPC_SET_VPS_DATA;
		vps_data.Enable    = denc_info.vps_en;
		vps_data.pVPS_DATA = denc_info.vps;
		ret = DENC_HAL_SetVpsData(&vps_data);
#endif
	} while(0);

	DENC_TRACE("done.\n");
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Initialize DENC module.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_Init(unsigned long arg)
{
	int ret = RET_OK;
#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	void *pstParams = NULL;
	DE_IPC_CMD_T ipcCmd = DENC_IPC_INIT;
#endif
#endif

	do {
		memset(&denc_info, 0, sizeof(struct denc_info));
		denc_info.vcr_en = FALSE;
		denc_info.dac_en = FALSE;
		denc_info.ttx_en = TRUE;
		denc_info.vps_en = TRUE;
		denc_info.wss_en = TRUE;

#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, pstParams, 0);
		if (ret) break;
#endif
#endif
		ret = DENC_HAL_Init();
	} while (0);

	DENC_TRACE("done.\n");
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief NTSC/PAL/SECAM encoder enable(TRUE : enable, FALSE : disable)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_OnOff(unsigned long arg)
{
	int ret = RET_OK;
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_ONOFF;
	BOOLEAN dacEnable;
#endif

	do {
        ret = copy_from_user(&denc_info.vcr_en, (void __user *)arg, sizeof(denc_info.vcr_en));
		if (ret) BREAK_WRONG(ret);
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &denc_info.vcr_en, sizeof(denc_info.vcr_en));
#endif
#ifndef USE_DENC_TURN_ON_ONTY_MCU_PART
		if (ret) break;
		ret = DENC_HAL_OnOff(&denc_info.vcr_en);
#endif
		if (ret) break;
		dacEnable = (denc_info.dac_en || denc_info.vcr_en)?TRUE:FALSE;
		ret = DENC_HAL_VdacPowerControl(&dacEnable);
	} while (0);

	DENC_TRACE("vcr_en [%d] \n", denc_info.vcr_en);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Select the color standard of CVE.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_SetOutPutVideoStd(unsigned long arg)
{
	int ret = RET_OK;
	LX_DENC_VIDEO_SYSTEM_T stParams;
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_SET_OUTPUT_VIDEO_STD;
#endif

	do {
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		denc_info.std = stParams;
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &stParams, sizeof(stParams));
		if (ret) break;
#endif
		ret = DENC_HAL_SetOutPutVideoStd(&stParams);
	} while (0);

	DENC_TRACE("video system [%d] \n", stParams);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Select the input source of CVE.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_SetSource(unsigned long arg)
{
	int ret = RET_OK;
	LX_DENC_VIDEO_SOURCE_T stParams;
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_SET_SOURCE;
#endif

	do {
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		denc_info.src = stParams;
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &stParams, sizeof(stParams));
		if (ret) break;
#endif
		ret = DENC_HAL_SetSource(&stParams);
	} while (0);

	DENC_TRACE("video source [%d] \n", stParams);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Teletext insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_TtxEnable(unsigned long arg)
{
	int ret = RET_OK;
#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_TTX_ENABLE;
#endif
#endif

	do {
        ret = copy_from_user(&denc_info.ttx_en, (void __user *)arg, sizeof(denc_info.ttx_en));
		if (ret) BREAK_WRONG(ret);
#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &denc_info.ttx_en, sizeof(denc_info.ttx_en));
		if (ret) break;
#endif
#endif
		ret = DENC_HAL_TtxEnable(&denc_info.ttx_en);
	} while (0);

	DENC_TRACE("ttx_en [%d] \n", denc_info.ttx_en);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief wss insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_WssEnable(unsigned long arg)
{
	int ret = RET_OK;
#ifndef USE_DENC_WSSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_WSS_ENABLE;
#endif
#endif

	do {
        ret = copy_from_user(&denc_info.wss_en, (void __user *)arg, sizeof(denc_info.wss_en));
		if (ret) BREAK_WRONG(ret);
#ifdef USE_DENC_WSSDATA_ONLY_CPU_PART
		ret = DENC_HAL_WssEnable(&denc_info.wss_en);
#else
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &denc_info.wss_en, sizeof(denc_info.wss_en));
#endif
#endif
	} while (0);

	DENC_TRACE("wss_en [%d] \n", denc_info.wss_en);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief VPS insertion enable (TRUE : enable, FALSE : disable)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_VpsEnable(unsigned long arg)
{
	int ret = RET_OK;
#ifndef USE_DENC_VPSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_VPS_ENABLE;
#endif
#endif

	do {
        ret = copy_from_user(&denc_info.vps_en, (void __user *)arg, sizeof(denc_info.vps_en));
		if (ret) BREAK_WRONG(ret);
#ifndef USE_DENC_VPSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &denc_info.vps_en, sizeof(denc_info.vps_en));
		if (ret) break;
#endif
#endif
		ret = DENC_HAL_VpsEnable(&denc_info.vps_en);
	} while (0);

	DENC_TRACE("vps_en [%d] \n", denc_info.vps_en);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts Teletext data to VBI lines.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_SetTtxData(unsigned long arg)
{
	int ret = RET_OK;
	LX_DENC_TTX_DATA_T stParams;
	VIDEO_TTX_PACK_T   ttxPack;
#ifdef USE_DENC_TTXDATA_LOADED_AT_MCU
#ifdef INCLUDE_KDRV_DE
#ifndef USE_DENC_TTXDATA_BY_DDR
	DE_IPC_CMD_T ipcCmd = DENC_IPC_SET_TTX_DATA;
#endif
#endif
#endif

	do {
		DENC_TRACE("vcr/ttx en[%d/%d] \n", denc_info.vcr_en, denc_info.ttx_en);
		if (!denc_info.vcr_en) break;
		if (!denc_info.ttx_en) break;
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		if (48 != stParams.Size) BREAK_WRONG(stParams.Size);
		ttxPack.ttxLine = stParams.Line_Number;
		ret = copy_from_user(ttxPack.ttxData, (void __user *)stParams.pPacket_Addr, stParams.Size);
		if (ret) BREAK_WRONG(ret);
#ifdef USE_DENC_TTXDATA_LOADED_AT_MCU
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
#ifdef USE_DENC_TTXDATA_BY_DDR
		ret = VIDEO_DENC_SetTtx(&ttxPack);
#else
#ifdef USE_DENC_TTXDATA_BY_SHARED_MEM
		ret = MCU_Wrmem(ipcCmd, &ttxPack, sizeof(ttxPack));
#else
		ret = MCU_Write(ipcCmd, &ttxPack, sizeof(ttxPack));
#endif
#endif
#endif
#else
		stParams.pPacket_Addr = ttxPack.ttxData;
		ret = DENC_HAL_SetTtxData(&stParams);
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts WSS data to VBI line.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_SetWssData(unsigned long arg)
{
	int ret = RET_OK;
	LX_DENC_WSS_DATA_T stParams;
#ifndef USE_DENC_WSSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_SET_WSS_DATA;
#endif
#endif

	do {
		DENC_TRACE("vcr/wss en[%d/%d] \n", denc_info.vcr_en, denc_info.wss_en);
		if (!denc_info.vcr_en) break;
		if (!denc_info.wss_en) break;
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		denc_info.wss_en = stParams.Enable;
		denc_info.wss = stParams.WSS_DATA;
#ifdef USE_DENC_WSSDATA_ONLY_CPU_PART
		ret = DENC_HAL_SetWssData(&stParams);
#else
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &stParams, sizeof(stParams));
#endif
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Inserts VPS data to VBI lines.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_SetVpsData(unsigned long arg)
{
	int ret = RET_OK;
	LX_DENC_VPS_DATA_T stParams;
	UINT8 vpsData[16];
#ifndef USE_DENC_VPSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_SET_VPS_DATA;
#endif
#endif

	do {
		DENC_TRACE("vcr/vps en[%d/%d] \n", denc_info.vcr_en, denc_info.vps_en);
		if (!denc_info.vcr_en) break;
		if (!denc_info.vps_en) break;
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
        ret = copy_from_user(&vpsData, (void __user *)stParams.pVPS_DATA, 15);
		if (ret) BREAK_WRONG(ret);
		denc_info.vps_en = stParams.Enable;
		memcpy(&denc_info.vps, vpsData, 15);
#ifndef USE_DENC_VPSDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		vpsData[15] = stParams.Enable;
		ret = MCU_Write(ipcCmd, vpsData, 16);
#endif
#else
		stParams.pVPS_DATA = vpsData;
		ret = DENC_HAL_SetVpsData(&stParams);
#endif
	} while (0);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Enable/disable internal colorbar pattern (TRUE : enable, FALSE : disable)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_ColorBarEnable(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN stParams;
#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
	DE_IPC_CMD_T ipcCmd = DENC_IPC_COLORBAR_ENABLE;
#endif
#endif

	do {
        ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
#ifndef USE_DENC_TTXDATA_ONLY_CPU_PART
#ifdef INCLUDE_KDRV_DE
		CHECK_MCU_IPC();
		ret = MCU_Write(ipcCmd, &stParams, 0);
		if (ret) break;
#endif
#endif
		ret = DENC_HAL_ColorBarEnable(&stParams);
	} while (0);

	DENC_TRACE("en[%d] \n", stParams);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief Power On/Off VDAC & Buffer of DENC Block (0:off, 1:on)
 *
 * @param arg [IN] UINT32 data(not address)
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_VdacPowerControl(unsigned long arg)
{
	int ret = RET_OK;
	BOOLEAN dacEnable = 0;

	do {
        ret = copy_from_user(&denc_info.dac_en, (void __user *)arg, sizeof(denc_info.dac_en));
		if (ret) BREAK_WRONG(ret);

		dacEnable = (denc_info.dac_en || denc_info.vcr_en)?TRUE:FALSE;
		ret = DENC_HAL_VdacPowerControl(&dacEnable);
	} while (0);

	DENC_TRACE("cmd/local en[%d/%d] \n", denc_info.dac_en, dacEnable);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief set dark of DENC Block (0:off, 1:on)
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_MuteControl(unsigned long arg)
{
	int ret = RET_OK;
#ifdef INCLUDE_KDRV_DE
	BOOLEAN stParams;
	DE_IPC_CMD_T ipcCmd = DENC_IPC_MUTE_CONTROL;

	do {
		CHECK_MCU_IPC();
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		ret = MCU_Write(ipcCmd, &stParams, sizeof(stParams));
	} while(0);
#endif
	DENC_TRACE("mute en[%d] \n", stParams);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief turn on/off sub-scaler for 3D auto detectiong by capturing video.
 *
 * @param arg [IN] address of buffer to be carried
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int DENC_IO_StripCaptureControl(unsigned long arg)
{
	int ret = RET_OK;
#ifdef INCLUDE_KDRV_DE
	BOOLEAN stParams;
	DE_IPC_CMD_T ipcCmd = DENC_IPC_STRIP_CAPTURE_CONTROL;

	do {
		CHECK_MCU_IPC();
		ret = copy_from_user(&stParams, (void __user *)arg, sizeof(stParams));
		if (ret) BREAK_WRONG(ret);
		ret = MCU_Write(ipcCmd, &stParams, sizeof(stParams));
	} while(0);
#endif
	DENC_TRACE("strip capture en[%d] \n", stParams);
	return ret;
}

/**  @} */
