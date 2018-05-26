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



/** @file	audio_kdrv_master.c
 *
 *  main driver implementation for  audio master device.
 *  audio master device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.25
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>    /**< For isr */
#include <linux/irq.h>			/**< For isr */
#include <linux/ioport.h>		/**< For request_region, check_region etc */
#include <linux/rmap.h>
#include <linux/kthread.h>
#include <asm/io.h>				/**< For ioremap_nocache */
#include <asm/memory.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/version.h>

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"
#include "audio_cfg.h"
#include "audio_drv.h"
#include "sys_regs.h"	//for CTOP CTRL Reg. map

#include "audio_imc_func.h"
#include "audio_buffer.h"

#include "audio_drv_master.h"
#include "audio_drv_manager.h"
#include "audio_drv_decoder.h"
#include "audio_drv_renderer.h"
#include "audio_drv_debug.h"

#ifdef INCLUDE_H15_CHIP_KDRV
#include "../../chip/h15/audio/audio_drv_hal_h15_a0.h"
#endif	//#ifdef INCLUDE_H15_CHIP_KDRV

#ifdef INCLUDE_AUDIO_CHIP_KDRV
#ifdef INCLUDE_H14_CHIP_KDRV
#include "../../chip/h14/audio/audio_drv_hal_h14_a0.h"
#endif	//#ifdef INCLUDE_H14_CHIP_KDRV
#endif	//#ifdef INCLUDE_AUDIO_CHIP_KDRV

#ifdef INCLUDE_M14_CHIP_KDRV
#include "../../chip/m14/audio/audio_drv_hal_m14_a0.h"
#endif	//#ifdef INCLUDE_M14_CHIP_KDRV

#ifdef INCLUDE_H13_CHIP_KDRV
#include "../../chip/h13/audio/audio_drv_hal_h13.h"
#endif	//#ifdef INCLUDE_H13_CHIP_KDRV


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	LX_AUD_KDRV_T				g_AudKdrv;
extern	AUD_MASTER_INFO_T			_gMasterInfo;
extern	BOOLEAN						g_bEnableDspMonitor;
extern	UINT32						g_audModIpcPrint;
extern	UINT32						g_audIpcPrintCheckErrorFlag;

/**
 * show Audio Modulesfor debug
 * @see KDRV_AUDIO_CustomImc
*/
static void AUDIO_ShowModules( void )
{
	int i;
	ModuleInfo moduleInfo0, moduleInfo1;

	AUD_KDRV_PRINTF("\n");
	for(i = 1; i < ADEC_MODULE_MAX; i=i+2 )
	{
		ModuleList_GetModuleInfo(i, &moduleInfo0);
		ModuleList_GetModuleInfo(i + 1, &moduleInfo1);
		AUD_KDRV_PRINTF("%02d : 0x%02X : %10s\t%02d : 0x%02X : %10s\n", \
			moduleInfo0.module, moduleInfo0.module, moduleInfo0.name,
			moduleInfo1.module, moduleInfo1.module, moduleInfo1.name);
	}
	AUD_KDRV_PRINTF("\n");
}

/**
 * Set Firmware Debug Mask
 * @param 	mask		[in] mask.
 * @see KDRV_AUDIO_CustomImc
*/
void KDRV_AUDIO_SetFwDebugMask(UINT32 mask)
{
	if (mask == 0)
	{
		g_audModIpcPrint = 0;
		OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG0);
		OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG1);
		OS_DEBUG_DisableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_DBG_ERR);
	}
	else
	{
		g_audModIpcPrint = 1;
		OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG0, DBG_COLOR_CYAN);
		OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_MSG1, DBG_COLOR_GREEN);
		OS_DEBUG_EnableModuleByIndex(g_AudDebugFd, LX_LOGM_LEVEL_TRACE_IPC_DBG_ERR, DBG_COLOR_RED);
	}

	AUDIO_WriteDebugMask(mask);
}

/**
 * Read write to registers.
 * This function is used for verification.
 * @see
*/
SINT32 KDRV_AUDIO_ReadAndWriteReg ( LX_AUD_REG_INFO_T *pstRegInfo )
{
	SINT32 retVal = RET_OK;

	//Check a chip revision
	if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		#ifdef INCLUDE_H15_CHIP_KDRV
		retVal = H15_A0_AUDIO_ReadAndWriteReg(pstRegInfo);
		#endif
	}
#ifdef INCLUDE_AUDIO_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		#ifdef INCLUDE_H14_CHIP_KDRV
		retVal = H14_A0_AUDIO_ReadAndWriteReg(pstRegInfo);
		#endif
	}
#endif	//#ifdef INCLUDE_AUDIO_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		#ifdef INCLUDE_M14_CHIP_KDRV
		retVal = M14_A0_AUDIO_ReadAndWriteReg(pstRegInfo);
		#endif
	}
#ifdef INCLUDE_AUDIO_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		#ifdef INCLUDE_H13_CHIP_KDRV
		retVal = H13_AUDIO_ReadAndWriteReg(pstRegInfo);
		#endif
	}
#endif	//#ifdef INCLUDE_AUDIO_CHIP_KDRV
	else
	{
		AUD_KDRV_PRINTF("<<< %s : LX_CHIP_REV => Unknown(0x%X)\n", __F__, lx_chip_rev());
	}

	AUD_KDRV_PRINT("%s is called!!!\n", __F__);
	return retVal;
}

/**
 * Send custom IPC command.
 * @see
*/
SINT32 KDRV_AUDIO_CustomImc(LX_AUD_CUSTOM_IMC_T* pstCustomImc)
{
	UINT32 			dbgMask;

	AUD_KDRV_TRACE("%s func started\r\n", __F__);
	AUD_KDRV_PRINT("HDR : %d, SIZE : %d\r\n", pstCustomImc->hdr, pstCustomImc->bodySize);

	if((pstCustomImc->hdr == 0x01) || (pstCustomImc->hdr == 0x02))
	{
		g_AudKdrv.LoadCodec   = pstCustomImc->param[0];
		AUDIO_Restart();
	}
	else if((pstCustomImc->hdr == 0x03) || (pstCustomImc->hdr == 0x04))
	{
		g_AudKdrv.CopyDSP0 = pstCustomImc->param[0];
		g_AudKdrv.CopyDSP1 = pstCustomImc->param[1];
	}
	else if(pstCustomImc->hdr == 0x05)
	{
		g_audIpcPrintCheckErrorFlag = pstCustomImc->param[0];
	}
	else if(pstCustomImc->hdr == 0x06)
	{
		AUD_RunSuspend();

		g_AudKdrv.bSuspended = TRUE;
		g_AudKdrv.bInitDone = FALSE;
	}
	else if(pstCustomImc->hdr == 0x07)
	{
		AUD_RunResume();

		g_AudKdrv.bSuspended = FALSE;
		g_AudKdrv.bInitDone = TRUE;
	}
	else if(pstCustomImc->hdr == 0x10)
	{
		AUDIO_ShowModules();
	}
	else if(pstCustomImc->hdr == 0x11)
	{
		ADEC_MODULE_ID	audioModule;

		audioModule = pstCustomImc->param[0];
		AUDIO_IMC_SendCmd(ADEC_CMD_INIT, audioModule);
	}
	else if(pstCustomImc->hdr == 0x12)
	{
		ADEC_MODULE_ID	audioModule;

		audioModule = pstCustomImc->param[0];
		AUDIO_IMC_SendCmd(ADEC_CMD_START, audioModule);
	}
	else if(pstCustomImc->hdr == 0x13)
	{
		ADEC_MODULE_ID	audioModule;

		audioModule = pstCustomImc->param[0];
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, audioModule);
	}
	else if(pstCustomImc->hdr == 0x14)
	{
		ADEC_MODULE_ID		audioModule;
		ImcCmdFlushParam	flushParam;

		audioModule = pstCustomImc->param[0];
		memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
		flushParam.num_of_port	= 1;
		flushParam.port_list[0]	= MOD_IN_PORT(0);
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, audioModule, sizeof(ImcCmdFlushParam), &flushParam);
	}
	else if(pstCustomImc->hdr == 0x15)
	{
		BOOLEAN		bUseReformatter = TRUE;

		bUseReformatter = pstCustomImc->param[0];
		KDRV_AUDIO_SetUseReformatter(bUseReformatter);
	}
	else if(pstCustomImc->hdr == 0x20)
	{
		dbgMask = AUDIO_ReadDebugMask();

		if((dbgMask & (1<< ADEC_DBG_INDEX_MODULE_MOR)) == 0)
		{
			dbgMask |= 1<<ADEC_DBG_INDEX_MODULE_MOR;
			KDRV_AUDIO_SetFwDebugMask(dbgMask);
		}

		AUDIO_IMC_SendCmd(ADEC_CMD_MODULE_STATUS, pstCustomImc->param[0]);
	}
	else if(pstCustomImc->hdr == 0x21)
	{
		ADEC_MODULE_ID	modId = ADEC_MODULE_NOT_DEF;
		AUD_MOD_INFO_T	*pModInfo = NULL;

		dbgMask = AUDIO_ReadDebugMask();

		if((dbgMask & (1<< ADEC_DBG_INDEX_MODULE_MOR)) == 0)
		{
			dbgMask |= 1<<ADEC_DBG_INDEX_MODULE_MOR;
			KDRV_AUDIO_SetFwDebugMask(dbgMask);
		}

		for(modId = ADEC_MODULE_NOT_DEF; modId < ADEC_MODULE_MAX; modId++)
		{
			pModInfo = AUDIO_GetModuleInfo(modId);
			if(pModInfo != NULL)
			{
				if(pModInfo->allocated == TRUE)
					AUDIO_IMC_SendCmd(ADEC_CMD_MODULE_STATUS, modId);
				OS_MsecSleep(30);
			}
		}
	}
	else if(pstCustomImc->hdr == 0x33)
	{
		AUDIO_ShowIoremap();
	}
	else if(pstCustomImc->hdr == 0x45)
	{
		if(g_AudKdrv.bInitDone == TRUE)
		{
			AUD_KDRV_PRINTF("#####MIXED_IP_AUDIO_ResetClockRegisters######\n");

			//Reset a clockregisters if DSP checks a abnormal state for clock interrupt.
			(void)MIXED_IP_AUDIO_ResetClockRegisters();
		}
	}
	else if(pstCustomImc->hdr == 0x46 || pstCustomImc->hdr == 0x47)
	{
		(void)MIXED_IP_AUDIO_SetSPDIFSamplingFreq(_gMasterInfo.spdifSamplingFreq, pstCustomImc->param[0], AUD_DTO_A);
	}
	else if(pstCustomImc->hdr == 0x48)
	{
		if(g_AudKdrv.bInitDone == TRUE)
		{
			AUD_KDRV_PRINTF("#####MIXED_IP_AUDIO_ResetAADClockRegisters######\n");

			//Reset a clockregisters if DSP checks a abnormal state for clock interrupt.
			(void)MIXED_IP_AUDIO_ResetAADClockRegisters();
		}
	}
	else if(pstCustomImc->hdr == 0x49)
	{
		if(g_AudKdrv.bInitDone == TRUE)
		{
			AUD_KDRV_PRINTF("#####Re-RegisterMasterNoti after FW re-loading######\n");

			//Re-Register Master Notification after FW re-loading.
			(void)KDRV_AUDIO_RegisterMasterNoti();
		}
	}
	else if(pstCustomImc->hdr == 0x50)
	{
		AUD_KDRV_ERROR("KDRV_AUDIO_DRV_VERSION = %d\n", KDRV_AUDIO_DRV_VERSION);
		pstCustomImc->param[0] = KDRV_AUDIO_DRV_VERSION;
	}
	else if((pstCustomImc->hdr & 0xFFFFFFF0) == 0xB0)
	{
		dbgMask = AUDIO_ReadDebugMask();

		if((dbgMask & (1<< ADEC_DBG_INDEX_BUF_MOR)) == 0)
		{
			dbgMask |= 1<<ADEC_DBG_INDEX_BUF_MOR;
			KDRV_AUDIO_SetFwDebugMask(dbgMask);
		}

		if(pstCustomImc->hdr == 0xB0)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_BUF_REPORT, ADEC_MODULE_DBG_DSP0);
		}
		else if(pstCustomImc->hdr == 0xB1)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_BUF_REPORT, ADEC_MODULE_DBG_DSP1);
		}
		else if(pstCustomImc->hdr == 0xBB)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_BUF_REPORT, ADEC_MODULE_DBG_DSP0);
			OS_MsecSleep(500);
			AUDIO_IMC_SendCmd(DBG_CMD_BUF_REPORT, ADEC_MODULE_DBG_DSP1);
		}
	}
	else if((pstCustomImc->hdr & 0xFFFFFFF0) == 0xC0)
	{
		dbgMask = AUDIO_ReadDebugMask();

		if((dbgMask & (1<< ADEC_DBG_INDEX_PROFILER)) == 0)
		{
			dbgMask |= 1<<ADEC_DBG_INDEX_PROFILER;
			KDRV_AUDIO_SetFwDebugMask(dbgMask);
		}

		if(pstCustomImc->hdr == 0xC0)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_PROF_REPORT, ADEC_MODULE_DBG_DSP0);
		}
		else if(pstCustomImc->hdr == 0xC1)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_PROF_REPORT, ADEC_MODULE_DBG_DSP1);
		}
		else if(pstCustomImc->hdr == 0xCC)
		{
			AUDIO_IMC_SendCmd(DBG_CMD_PROF_REPORT, ADEC_MODULE_DBG_DSP0);
			OS_MsecSleep(500);
			AUDIO_IMC_SendCmd(DBG_CMD_PROF_REPORT, ADEC_MODULE_DBG_DSP1);
		}
	}
	else
	{
		if(pstCustomImc->bodySize == 0)
		{
			AUDIO_IMC_SendCmd(pstCustomImc->hdr, pstCustomImc->param[0]);
			AUD_KDRV_PRINTF("AUDIO_IMC_SendCmd(%d)\n", pstCustomImc->bodySize);
		}
		else
		{
			int i;
			AUDIO_IMC_SendCmdParam(pstCustomImc->hdr, pstCustomImc->param[0], pstCustomImc->bodySize, &(pstCustomImc->param[1]));
			for(i = 0; i < (pstCustomImc->bodySize / sizeof(UINT32)); i++)
				AUD_KDRV_PRINTF("AUDIO_IMC_SendCmdParam(%x %d %x)\n", pstCustomImc->hdr, pstCustomImc->bodySize, pstCustomImc->param[i+1]);
		}
	}

	return RET_OK;
}

static ModuleInfo gModInfo[ADEC_MODULE_MAX] = {
	{
		ADEC_MODULE_NOT_DEF,
		"NOT_DEF",
		ADEC_CORE_NOT_DEF,
		0,
		0,
	},
#ifdef ADEC_MODULE_DEF
#undef ADEC_MODULE_DEF
#endif
#ifdef __BUF_END
#undef __BUF_END
#endif
#define __BUF_END()	ADEC_BUF_NOT_DEF,ADEC_BUF_NOT_DEF, ADEC_BUF_NOT_DEF, ADEC_BUF_NOT_DEF,
#define __MOD_DECL(__MODULE, __MODULE_NAME, __MODULE_TYPE, __LOCATION, __NUM_IN, __NUM_OUT, __PORT_0_TYPE, __PORT_1_TYPE, __PORT_2_TYPE, __PORT_3_TYPE, ...) \
	{\
		__MODULE,\
		__MODULE_NAME,\
		__MODULE_TYPE,\
		__LOCATION,\
		__NUM_IN,\
		__NUM_OUT,\
		{\
			__PORT_0_TYPE,\
			__PORT_1_TYPE,\
			__PORT_2_TYPE,\
			__PORT_3_TYPE,\
		},\
	},
#define ADEC_MODULE_DEF(__MODULE, __MODULE_NAME, __LOCATION, __NUM_IN, __NUM_OUT, ...)\
	__MOD_DECL(__MODULE,__MODULE_NAME, __LOCATION, __NUM_IN, __NUM_OUT, __VA_ARGS__)
#include "module/adec_module_def.h"
#undef ADEC_MODULE_DEF

};

void KDRV_AUDIO_GetModIdFromStr(LX_AUD_DEBUG_MOD_INFO_T *pModInfo)
{
	UINT32 i;

	for(i = 1;i < ADEC_MODULE_MAX; i++)
	{
		if(!strcmp(pModInfo->name, gModInfo[i].name))
		{
			pModInfo->module = gModInfo[i].module;
			AUD_KDRV_PRINTF("0x%02X : 0x%08X %s\n", i, pModInfo->module, pModInfo->name);
			break;
		}
	}
}

SINT32 KDRV_AUDIO_GetFwDebugInfo(LX_AUD_FW_DEBUG_INFO_T *pDebugInfo)
{
	SINT32 i;
	SINT32 max;
	UINT32 mask;

	if(pDebugInfo == NULL)
	{
		return -1;
	}

	mask = AUDIO_ReadDebugMask();
	max = ADEC_DBG_INDEX_MAX > 32 ? 32 : ADEC_DBG_INDEX_MAX;
	pDebugInfo->mask_value = mask;
	pDebugInfo->num_of_index = max;

	for (i = 0; i < max; i++)
	{
		pDebugInfo->nodes[i].debug_level = 0;
		pDebugInfo->nodes[i].index = 0;
		pDebugInfo->nodes[i].enable = (mask >> i) & 0x01;
		strncpy(pDebugInfo->nodes[i].index_name, DbgIndex_GetIndexName(i), 11);
	}

	return max;
}


