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
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.06
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
#include "ipc_def_h13.h"
#include "ipc_drv.h"
#include "be_fwl_hw_h13.h"
#if CONFIG_LX_H13_CHIP_FIRMWARE
#include "frc_fw_h13b0.h"
#include "frc_fw_h13b0_uhd.h"
#include "sre_fw_h13b0.h"
#include "wcg_fw_h13b0.h"
#include "led_fw_h13b0.h"
#include "hdr_fw_h13b0.h"
#endif
#include "../reg/be_reg_dppb_h13.h"

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

extern DPPB_TOP_REG_H13_T gDPPB_TOP_H13;

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
BE_FWL_HAL_FW_T gHeader_Firmware_H13B0[BE_FW_DWLD_TYPE_MAXNUM] = 
	{
#if CONFIG_LX_H13_CHIP_FIRMWARE
		{ BE_FW_DWLD_TYPE_FRC, sizeof(ui32FrcFirmware_H13B0), (char *)ui32FrcFirmware_H13B0, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, sizeof(ui32SreFirmware_H13B0), (char *)ui32SreFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, sizeof(ui32WcgFirmware_H13B0), (char *)ui32WcgFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_LED, sizeof(ui32LedFirmware_H13B0), (char *)ui32LedFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, sizeof(ui32HdrFirmware_H13B0), (char *)ui32HdrFirmware_H13B0, FALSE }
#else
		{ BE_FW_DWLD_TYPE_FRC, 0, NULL, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_LED, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, 0, NULL, FALSE }
#endif
	};

BE_FWL_HAL_FW_T gHeader_Firmware_H13B0_UHD[BE_FW_DWLD_TYPE_MAXNUM] = 
	{
#if CONFIG_LX_H13_CHIP_FIRMWARE
		{ BE_FW_DWLD_TYPE_FRC, sizeof(ui32FrcFirmware_H13B0_UHD), (char *)ui32FrcFirmware_H13B0_UHD, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, sizeof(ui32SreFirmware_H13B0), (char *)ui32SreFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, sizeof(ui32WcgFirmware_H13B0), (char *)ui32WcgFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_LED, sizeof(ui32LedFirmware_H13B0), (char *)ui32LedFirmware_H13B0, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, sizeof(ui32HdrFirmware_H13B0), (char *)ui32HdrFirmware_H13B0, FALSE }
#else
		{ BE_FW_DWLD_TYPE_FRC, 0, NULL, TRUE },
		{ BE_FW_DWLD_TYPE_SRE, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_WCG, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_LED, 0, NULL, FALSE },
		{ BE_FW_DWLD_TYPE_HDR, 0, NULL, FALSE }
#endif
	};

BE_FWL_HAL_FW_FILE_T gFile_Firmware_H13B0[BE_FW_DWLD_TYPE_MAXNUM] = 
	{
		{ BE_FW_DWLD_TYPE_FRC, "/root/h13_firmware/frc_fw_h13b0.bin", TRUE },
		{ BE_FW_DWLD_TYPE_SRE, "/root/h13_firmware/sre_fw_h13b0.bin", FALSE },
		{ BE_FW_DWLD_TYPE_WCG, "/root/h13_firmware/wcg_fw_h13b0.bin", FALSE },
		{ BE_FW_DWLD_TYPE_LED, "/root/h13_firmware/led_fw_h13b0.bin", FALSE },
		{ BE_FW_DWLD_TYPE_HDR, "/root/h13_firmware/hdr_fw_h13b0.bin", FALSE }
	}; 

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static BOOLEAN _FWL_HW_H13_IsForUHD(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
BE_FWL_HAL_FW_FILE_T *BE_FWL_HW_H13_GetFwBinInfo(UINT8 ucIndex)
{
	if (ucIndex >= BE_FW_DWLD_TYPE_MAXNUM)
	{
		BE_ERROR("BE_FWL_HW  ERROR! ucIndex(0x%x) is invalid!\n", ucIndex);
		return NULL;
	}
	
	return &gFile_Firmware_H13B0[ucIndex];
}

BE_FWL_HAL_FW_T *BE_FWL_HW_H13_GetFwHeaderInfo(UINT8 ucIndex)
{
	if (ucIndex >= BE_FW_DWLD_TYPE_MAXNUM)
	{
		BE_ERROR("BE_FWL_HW  ERROR! ucIndex(0x%x) is invalid!\n", ucIndex);
		return NULL;
	}

	return _FWL_HW_H13_IsForUHD() ? &gHeader_Firmware_H13B0_UHD[ucIndex] : &gHeader_Firmware_H13B0[ucIndex];
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
UINT32 BE_FWL_HW_H13_FW_Download(UINT8 ucIndex, UINT8 *pucBuff, UINT32 ulSize, BOOLEAN bIsMcuFw)
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
		
		if(bIsMcuFw)
		{
			BOOLEAN         run_stall = 0;
			
			DPPB_TOP_H13_RdFL(top_runstall);
			DPPB_TOP_H13_Rd01(top_runstall, runstall, run_stall);
			if(!run_stall) break;
		}
		
		lpAddr = (char *)ioremap(pFwMem->fw_base, pFwMem->fw_size);
		CHECK_NULL(lpAddr);
		memcpy(lpAddr, pucBuff, ulSize);

		if(bIsMcuFw)
		{
			DPPB_TOP_H13_RdFL(top_tr_addr_offset_0);
			DPPB_TOP_H13_Wr01(top_tr_addr_offset_0, tr_addr_offset_0, pFwMem->fw_base);
			DPPB_TOP_H13_WrFL(top_tr_addr_offset_0);
			
			msleep(10); // wait ddr to ddr transition

			DPPB_TOP_H13_Wr01(top_runstall, runstall, 0);
			DPPB_TOP_H13_WrFL(top_runstall);

			msleep(10); // wait ddr to ddr/mcu-iram transition
		}

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
BOOLEAN BE_FWL_HW_H13_MCUisAlive(void)
{
	BOOLEAN isFwRun = FALSE;

#if 0
	DPPB_TOP_H13_FLRf(top_stat_vector_sel, stat_vector_sel, isFwRun);
	return TRUE;//isFwRun;
#else
	UINT32 pc;

	DPPB_TOP_H13_FLWr(top_pdebugenable, TRUE);
	DPPB_TOP_H13_FLRd(top_pdebugpc, pc);
	if(pc >= 0x40000600)
	{
		isFwRun = TRUE;
	}
	DPPB_TOP_H13_FLWr(top_pdebugenable, FALSE);

	return isFwRun;
#endif
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
UINT32 BE_FWL_HW_H13_GetFwMemBaseSize(UINT8 ucIndex)
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

int BE_FWL_HW_H13_FrcFwInit(void)
{
	int ret = RET_OK;
	BE_IPC_INIT_T fwTopInitParam;
	BE_IPC_FRC_INIT_T fwFrcInitParam;
	BE_H3D_INIT_BLOCK_T fwH3dInitParam;
	BE_IPC_PE_INIT_BLOCK_T fwPeInitParam;

	do{
		fwTopInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_TOP_INIT, &fwTopInitParam, sizeof(BE_IPC_INIT_T));
		if(ret) BREAK_WRONG(ret);

		fwFrcInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_FRC_INIT_BLOCK, &fwFrcInitParam, sizeof(BE_IPC_FRC_INIT_T));
		if(ret) BREAK_WRONG(ret);

		fwH3dInitParam.bInitA3C = TRUE;
		fwH3dInitParam.bInitT3C = TRUE;
		ret = BE_KIPC_SetData(BE_H3D_INIT_BLOCK, &fwH3dInitParam, sizeof(BE_H3D_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);

		fwPeInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_PE_INIT_BLOCK, &fwPeInitParam, sizeof(BE_IPC_PE_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_FWL_HW_H13_SreFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;
	BE_IPC_SRE_INIT_BLOCK_T fwSreInitParam;
	BE_IPC_SRE_LUT_ADDR_T fwSreLutAddr;
	BE_IPC_SRE_LOAD_LUT_T fwSreLoadLutParam;
	
	do{
		fwSreInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_SRE_INIT_BLOCK, &fwSreInitParam, sizeof(BE_IPC_SRE_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);

		fwSreLutAddr.u32SreLutDDRAddr = u32FwBaseAddr;
		ret = BE_KIPC_SetData(BE_SRE_SET_LUT_ADDR, &fwSreLutAddr, sizeof(BE_IPC_SRE_LUT_ADDR_T));
		if(ret) BREAK_WRONG(ret);

		fwSreLoadLutParam.bLoad = TRUE;
		ret = BE_KIPC_SetData(BE_SRE_LOAD_LUT, &fwSreLoadLutParam, sizeof(BE_IPC_SRE_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_FWL_HW_H13_WcgFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;
	BE_IPC_WCG_INIT_BLOCK_T fwWcgInitParam;
	BE_IPC_WCG_LUT_ADDR_T fwWcgLutAddr;
	BE_IPC_WCG_LOAD_LUT_T fwWcgLoadLutParam;

	do{
		fwWcgInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_WCG_INIT_BLOCK, &fwWcgInitParam, sizeof(BE_IPC_WCG_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);

		fwWcgLutAddr.u32WcgLutDDRAddr = u32FwBaseAddr;
		ret = BE_KIPC_SetData(BE_WCG_SET_LUT_ADDR, &fwWcgLutAddr, sizeof(BE_IPC_WCG_LUT_ADDR_T));
		if(ret) BREAK_WRONG(ret);

		fwWcgLoadLutParam.bLoad = TRUE;
		ret = BE_KIPC_SetData(BE_WCG_LOAD_LUT, &fwWcgLoadLutParam, sizeof(BE_IPC_WCG_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_FWL_HW_H13_LedFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;
	BE_IPC_LED_INIT_BLOCK_T fwLedInitParam;
	BE_IPC_LED_LUT_ADDR_T fwLedLutAddr;
	BE_IPC_LED_LOAD_LUT_T fwLedLoadLutParam;

	do{
		fwLedInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_LED_INIT_BLOCK, &fwLedInitParam, sizeof(BE_IPC_LED_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);

		fwLedLutAddr.u32LedLutDDRAddr = u32FwBaseAddr;
		ret = BE_KIPC_SetData(BE_LED_SET_LUT_ADDR, &fwLedLutAddr, sizeof(BE_IPC_LED_LUT_ADDR_T));
		if(ret) BREAK_WRONG(ret);

		fwLedLoadLutParam.bLoad = TRUE;
		ret = BE_KIPC_SetData(BE_LED_LOAD_LUT, &fwLedLoadLutParam, sizeof(BE_IPC_LED_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

int BE_FWL_HW_H13_HdrFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;
	BE_IPC_HDR_INIT_BLOCK_T fwHdrInitParam;
	BE_IPC_HDR_LUT_ADDR_T fwHdrLutAddr;
	BE_IPC_HDR_LOAD_LUT_T fwHdrLoadLutParam;

	do{
		fwHdrInitParam.bInit = TRUE;
		ret = BE_KIPC_SetData(BE_HDR_INIT_BLOCK, &fwHdrInitParam, sizeof(BE_IPC_HDR_INIT_BLOCK_T));
		if(ret) BREAK_WRONG(ret);

		fwHdrLutAddr.u32HdrLutDDRAddr = u32FwBaseAddr;
		ret = BE_KIPC_SetData(BE_HDR_SET_LUT_ADDR, &fwHdrLutAddr, sizeof(BE_IPC_HDR_LUT_ADDR_T));
		if(ret) BREAK_WRONG(ret);

		fwHdrLoadLutParam.bLoad = TRUE;
		ret = BE_KIPC_SetData(BE_HDR_LOAD_LUT, &fwHdrLoadLutParam, sizeof(BE_IPC_HDR_LOAD_LUT_T));
		if(ret) BREAK_WRONG(ret);
	} while(0);
	
	return ret;
}

static BOOLEAN _FWL_HW_H13_IsForUHD(void)
{
	BOOLEAN isForUhd = FALSE;
	volatile UINT32 *oif_vch_config_reg_addr = NULL;

	do{
		oif_vch_config_reg_addr = (volatile UINT32 *)ioremap(0xC0049000, sizeof(UINT32));
		if (oif_vch_config_reg_addr == NULL) break;

		isForUhd = ((*oif_vch_config_reg_addr) & 0x0FFFF000) == 0x0C780000 ? TRUE : FALSE;

	} while(0);	

	if (oif_vch_config_reg_addr) iounmap((void *)oif_vch_config_reg_addr);

	return isForUhd;
}
