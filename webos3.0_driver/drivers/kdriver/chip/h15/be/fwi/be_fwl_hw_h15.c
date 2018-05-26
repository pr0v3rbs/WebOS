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
 *  fw loader reg driver file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2014.05.21
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/delay.h>


#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"
#include "be_cfg.h"
#include "ipc_def_h15.h"
#include "ipc_drv.h"
#include "be_fwl_hw_h15.h"
#if CONFIG_LX_H15_CHIP_FIRMWARE
#include "wcg_fw_h15a0.h"
#include "led_fw_h15a0.h"
#endif

#include "ipc_cmd_header.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern BE_MEM_CFG_T *gpBeMem;

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BE_FWL_HAL_FW_T gHeader_Firmware_H15A0[BE_FW_DWLD_TYPE_MAXNUM] =
	{
#if CONFIG_LX_H15_CHIP_FIRMWARE
		{ BE_FW_DWLD_TYPE_FRC, 0, NULL, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, sizeof(ui32WcgFirmware_H15A0), (char *)ui32WcgFirmware_H15A0, FALSE },
		{ BE_FW_DWLD_TYPE_LED, sizeof(ui32LedFirmware_H15A0), (char *)ui32LedFirmware_H15A0, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, 0, NULL, FALSE }
#else
		{ BE_FW_DWLD_TYPE_FRC, 0, NULL, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_LED, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, 0, NULL, FALSE }
#endif
	};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
BE_FWL_HAL_FW_T *BE_FWL_HW_H15_GetFwHeaderInfo(UINT8 ucIndex)
{
	if (ucIndex >= BE_FW_DWLD_TYPE_MAXNUM)
	{
		BE_ERROR("BE_FWL_HW  ERROR! ucIndex(0x%x) is invalid!\n", ucIndex);
		return NULL;
	}

	return &gHeader_Firmware_H15A0[ucIndex];
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief download BE firmware
 *
 * @param ucIndex [IN] index information for new firmware
 * @param pucBuff [IN] pointer information for new firmware
 * @param ulSize [IN] size information for new firmware
 * @param bIsMcuFw [IN] boolean information for MCU firmware or not
 *
 * @return FW base address if success, NULL for otherwise
 */
UINT32 BE_FWL_HW_H15_FW_Download(UINT8 ucIndex, UINT8 *pucBuff, UINT32 ulSize, BOOLEAN bIsMcuFw)
{
	int ret = RET_OK;
	BE_CH_MEM_T *pFwMem = NULL;
	char            *lpAddr = NULL;

	do {
		CHECK_NULL(pucBuff);
		CHECK_NULL(ulSize);
		if(ucIndex >= BE_CH_MEM_DOWNLOAD_FW_MAX)
		{
			BREAK_WRONG(ucIndex);
		}

		pFwMem = (BE_CH_MEM_T *) &gpBeMem->fw[ucIndex];
		CHECK_NULL(pFwMem);

		if (ulSize > pFwMem->fw_size) BREAK_WRONG(ulSize);
		
		lpAddr = (char *)ioremap(pFwMem->fw_base, pFwMem->fw_size);
		CHECK_NULL(lpAddr);
		memcpy(lpAddr, pucBuff, ulSize);

		iounmap(lpAddr);
	} while(0);

	return ret == RET_OK ? pFwMem->fw_base : (UINT32)NULL;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief check mcu is alive
 *
 * @param void
 *
 * @return TRUE if mcu run, FALSE for otherwise
 */
BOOLEAN BE_FWL_HW_H15_MCUisAlive(void)
{
	BOOLEAN isFwRun = FALSE;

	isFwRun = TRUE;

	return isFwRun;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief return firmware memory base size
 *
 * @param ucIndex [IN] index information for the firmware
 *
 * @return firmware memory base size if success, 0 for otherwise
 */
UINT32 BE_FWL_HW_H15_GetFwMemBaseSize(UINT8 ucIndex)
{
	int ret = RET_OK;

	do {
		if(ucIndex >= BE_CH_MEM_DOWNLOAD_FW_MAX)
		{
			BREAK_WRONG(ucIndex);
			break;
		}
	} while(0);

	return ret == RET_OK ? gpBeMem->fw[ucIndex].fw_size : (UINT32)0;
}

int BE_FWL_HW_H15_WcgFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	do{
		BE_IPC_B0_WCG_LOAD_LUT_T fwWcgLoadLutParam;

		fwWcgLoadLutParam.bWCGLutLoad = TRUE;
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_WCG_B0_LOAD_LUT), &fwWcgLoadLutParam, sizeof(BE_IPC_B0_WCG_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_FWL_HW_H15_LedFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	do{
		BE_IPC_B0_LED_LOAD_LUT_T fwLedLoadLutParam;

		fwLedLoadLutParam.uLEDLutLoad = TRUE;
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_LED_B0_LOAD_LUT), &fwLedLoadLutParam, sizeof(BE_IPC_B0_LED_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_FWL_HW_H15_DrcFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	do{
		BE_IPC_B0_DRC_LOAD_LUT_T fwDrcLoadLutParam;

		fwDrcLoadLutParam.bDRCLutLoad = TRUE;
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_DRC_B0_LOAD_LUT), &fwDrcLoadLutParam, sizeof(BE_IPC_B0_DRC_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}
