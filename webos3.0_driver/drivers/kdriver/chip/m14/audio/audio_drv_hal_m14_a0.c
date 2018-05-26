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
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date	2012.05.03
 *  note	Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <linux/rmap.h>
#include <linux/file.h>		/** < For Audio Memory debug */
#include <linux/syscalls.h>
#include <linux/fcntl.h>

#include <asm/io.h>			/**< For ioremap_nocache */
#include <asm/uaccess.h>
#include "proc_util.h"
#include "debug_util.h"

#include "base_types.h"
#include "base_device.h"
#include "os_util.h"

#include "audio_cfg.h"
#include "audio_drv.h"
#include "audio_buffer.h"

#include "audio_reg_m14_a0.h"


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


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern LX_AUD_KDRV_T	g_AudKdrv;
extern IPC_RW_OFFSET_ADDR_T g_IPCRWOffset;

//DSP virtual address (ioremap)
extern UINT32	*g_pui32DSP0CodecVirAddr;
extern UINT32	*g_pui32DSP1CodecVirAddr;

//IPC virtual address (ioremap)
extern UINT32	*g_pui32IPC_VirAddr;
extern UINT32	*g_pui32SEParamVirAddr;

extern	BOOLEAN	g_bEnableDspMonitor;

extern LX_AUD_BUF_MEM_CFG_S_T	_gAuBufMemCfg[AUD_BUFFER_MAX];
extern UINT32	_gBufTypeNum[AUD_BUFFER_TYPE_MAX];
extern UINT32	_gBufTypeBase[AUD_BUFFER_TYPE_MAX];

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/* define AUDIO register */
static volatile LX_AUD_REG_M14_A0_T	*g_pRealRegAud = NULL;
static LX_AUD_REG_M14_A0_T *g_pTempRegAud = NULL;


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * ioremap audio memory
 * @see
*/
SINT32 M14_A0_AUDIO_IOREMAP( void )
{
	SINT32 ret = RET_OK;
	SINT32 i, k;
	UINT32 searchStart, searchEnd;

	AUD_KDRV_PRINT("pstAudRegCfg->reg_base_addr[%x] pstAudRegCfg->reg_size[%x]\n", pstAudRegCfg->reg_base_addr, pstAudRegCfg->reg_size);

	if(g_pRealRegAud == NULL)
	{
		//ioremap for HW register
		g_pRealRegAud = (volatile LX_AUD_REG_M14_A0_T *)ioremap(pstAudRegCfg->reg_base_addr, pstAudRegCfg->reg_size);
		if(g_pRealRegAud == NULL)
		{
			AUD_KDRV_ERROR("ERROR : can't allocate for register\n");
			return -EIO;
		}
	}

	if(g_pTempRegAud == NULL)
	{
		//ioremap for HW register
		g_pTempRegAud = (LX_AUD_REG_M14_A0_T *)kmalloc(sizeof(LX_AUD_REG_M14_A0_T), GFP_KERNEL);
		if(g_pTempRegAud == NULL)
		{
			AUD_KDRV_ERROR("ERROR : can't allocate for register\n");
			return -EIO;
		}
	}

	//Set a IPC RW Address
	g_IPCRWOffset.p_aud_ipc_a2p_rptr = (void *)&g_pRealRegAud->aud_ipc_a2p_rptr;
	g_IPCRWOffset.p_aud_ipc_a2p_wptr = (void *)&g_pRealRegAud->aud_ipc_a2p_wptr;
	g_IPCRWOffset.p_aud_ipc_p2a_rptr = (void *)&g_pRealRegAud->aud_ipc_p2a_rptr;
	g_IPCRWOffset.p_aud_ipc_p2a_wptr = (void *)&g_pRealRegAud->aud_ipc_p2a_wptr;
	g_IPCRWOffset.p_aud_ipc_a2d_rptr = (void *)&g_pRealRegAud->aud_ipc_a2d_rptr;
	g_IPCRWOffset.p_aud_ipc_a2d_wptr = (void *)&g_pRealRegAud->aud_ipc_a2d_wptr;
	g_IPCRWOffset.p_aud_ipc_d2a_rptr = (void *)&g_pRealRegAud->aud_ipc_d2a_rptr;
	g_IPCRWOffset.p_aud_ipc_d2a_wptr = (void *)&g_pRealRegAud->aud_ipc_d2a_wptr;
	g_IPCRWOffset.p_aud_ipc_d2p_rptr = (void *)&g_pRealRegAud->aud_ipc_d2p_rptr;
	g_IPCRWOffset.p_aud_ipc_d2p_wptr = (void *)&g_pRealRegAud->aud_ipc_d2p_wptr;
	g_IPCRWOffset.p_aud_ipc_p2d_rptr = (void *)&g_pRealRegAud->aud_ipc_p2d_rptr;
	g_IPCRWOffset.p_aud_ipc_p2d_wptr = (void *)&g_pRealRegAud->aud_ipc_p2d_wptr;

	g_IPCRWOffset.p_aud_ipc_dbgp_rptr = (void *)&g_pRealRegAud->aud_ipc_dbgp_rptr;
	g_IPCRWOffset.p_aud_ipc_dbgp_wptr = (void *)&g_pRealRegAud->aud_ipc_dbgp_wptr;
	g_IPCRWOffset.p_aud_ipc_dbgd_rptr = (void *)&g_pRealRegAud->aud_ipc_dbgd_rptr;
	g_IPCRWOffset.p_aud_ipc_dbgd_wptr = (void *)&g_pRealRegAud->aud_ipc_dbgd_wptr;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_CPB];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->cpb0_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->cpb0_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->cpb0_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_cpb0_wo;

	_gAuBufMemCfg[searchStart+1].MemPhyBase = pstMemCfgAud->cpb1_memory_base;
	_gAuBufMemCfg[searchStart+1].MemSize = pstMemCfgAud->cpb1_memory_size;
	_gAuBufMemCfg[searchStart+1].MemName = pstMemCfgAud->cpb1_memory_name;
	_gAuBufMemCfg[searchStart+1].pRegBase = (void *)&g_pRealRegAud->aud_buf_cpb1_wo;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_DPB];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->dpb0_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->dpb0_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->dpb0_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_dpb0_wo;

	_gAuBufMemCfg[searchStart+1].MemPhyBase = pstMemCfgAud->dpb1_memory_base;
	_gAuBufMemCfg[searchStart+1].MemSize = pstMemCfgAud->dpb1_memory_size;
	_gAuBufMemCfg[searchStart+1].MemName = pstMemCfgAud->dpb1_memory_name;
	_gAuBufMemCfg[searchStart+1].pRegBase = (void *)&g_pRealRegAud->aud_buf_dpb1_wo;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_MPB];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->mpb0_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->mpb0_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->mpb0_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb0_wo;

	_gAuBufMemCfg[searchStart+1].MemPhyBase = pstMemCfgAud->mpb1_memory_base;
	_gAuBufMemCfg[searchStart+1].MemSize = pstMemCfgAud->mpb1_memory_size;
	_gAuBufMemCfg[searchStart+1].MemName = pstMemCfgAud->mpb1_memory_name;
	_gAuBufMemCfg[searchStart+1].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb1_wo;

	_gAuBufMemCfg[searchStart+2].MemPhyBase = pstMemCfgAud->mpb2_memory_base;
	_gAuBufMemCfg[searchStart+2].MemSize = pstMemCfgAud->mpb2_memory_size;
	_gAuBufMemCfg[searchStart+2].MemName = pstMemCfgAud->mpb2_memory_name;
	_gAuBufMemCfg[searchStart+2].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb2_wo;

	_gAuBufMemCfg[searchStart+3].MemPhyBase = pstMemCfgAud->mpb3_memory_base;
	_gAuBufMemCfg[searchStart+3].MemSize = pstMemCfgAud->mpb3_memory_size;
	_gAuBufMemCfg[searchStart+3].MemName = pstMemCfgAud->mpb3_memory_name;
	_gAuBufMemCfg[searchStart+3].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb3_wo;

	_gAuBufMemCfg[searchStart+4].MemPhyBase = pstMemCfgAud->mpb4_memory_base;
	_gAuBufMemCfg[searchStart+4].MemSize = pstMemCfgAud->mpb4_memory_size;
	_gAuBufMemCfg[searchStart+4].MemName = pstMemCfgAud->mpb4_memory_name;
	_gAuBufMemCfg[searchStart+4].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb4_wo;

	_gAuBufMemCfg[searchStart+5].MemPhyBase = pstMemCfgAud->mpb5_memory_base;
	_gAuBufMemCfg[searchStart+5].MemSize = pstMemCfgAud->mpb5_memory_size;
	_gAuBufMemCfg[searchStart+5].MemName = pstMemCfgAud->mpb5_memory_name;
	_gAuBufMemCfg[searchStart+5].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb5_wo;

	_gAuBufMemCfg[searchStart+6].MemPhyBase = pstMemCfgAud->mpb6_memory_base;
	_gAuBufMemCfg[searchStart+6].MemSize = pstMemCfgAud->mpb6_memory_size;
	_gAuBufMemCfg[searchStart+6].MemName = pstMemCfgAud->mpb6_memory_name;
	_gAuBufMemCfg[searchStart+6].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb6_wo;

	_gAuBufMemCfg[searchStart+7].MemPhyBase = pstMemCfgAud->mpb7_memory_base;
	_gAuBufMemCfg[searchStart+7].MemSize = pstMemCfgAud->mpb7_memory_size;
	_gAuBufMemCfg[searchStart+7].MemName = pstMemCfgAud->mpb7_memory_name;
	_gAuBufMemCfg[searchStart+7].pRegBase = (void *)&g_pRealRegAud->aud_buf_mpb7_wo;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_IEC];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->iec0_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->iec0_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->iec0_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_iec_wo;

	_gAuBufMemCfg[searchStart+1].MemPhyBase = pstMemCfgAud->iec1_memory_base;
	_gAuBufMemCfg[searchStart+1].MemSize = pstMemCfgAud->iec1_memory_size;
	_gAuBufMemCfg[searchStart+1].MemName = pstMemCfgAud->iec1_memory_name;
	_gAuBufMemCfg[searchStart+1].pRegBase = (void *)&g_pRealRegAud->aud_buf_iec1_wo;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_ENC];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->enc_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->enc_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->enc_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_dec_sink0_wo;

	searchStart = _gBufTypeBase[AUD_BUFFER_TYPE_CAPB];
	_gAuBufMemCfg[searchStart].MemPhyBase = pstMemCfgAud->cap0_memory_base;
	_gAuBufMemCfg[searchStart].MemSize = pstMemCfgAud->cap0_memory_size;
	_gAuBufMemCfg[searchStart].MemName = pstMemCfgAud->cap0_memory_name;
	_gAuBufMemCfg[searchStart].pRegBase = (void *)&g_pRealRegAud->aud_buf_sys_sink0_wo;

	_gAuBufMemCfg[searchStart+1].MemPhyBase = pstMemCfgAud->cap1_memory_base;
	_gAuBufMemCfg[searchStart+1].MemSize = pstMemCfgAud->cap1_memory_size;
	_gAuBufMemCfg[searchStart+1].MemName = pstMemCfgAud->cap1_memory_name;
	_gAuBufMemCfg[searchStart+1].pRegBase = (void *)&g_pRealRegAud->aud_buf_sys_sink1_wo;

	_gAuBufMemCfg[searchStart+2].MemPhyBase = pstMemCfgAud->cap2_memory_base;
	_gAuBufMemCfg[searchStart+2].MemSize = pstMemCfgAud->cap2_memory_size;
	_gAuBufMemCfg[searchStart+2].MemName = pstMemCfgAud->cap2_memory_name;
	_gAuBufMemCfg[searchStart+2].pRegBase = (void *)&g_pRealRegAud->aud_buf_dec_sink1_wo;

	_gAuBufMemCfg[searchStart+3].MemPhyBase = pstMemCfgAud->cap3_memory_base;
	_gAuBufMemCfg[searchStart+3].MemSize = pstMemCfgAud->cap3_memory_size;
	_gAuBufMemCfg[searchStart+3].MemName = pstMemCfgAud->cap3_memory_name;
	_gAuBufMemCfg[searchStart+3].pRegBase = (void *)&g_pRealRegAud->__rsvd_04[0];

	_gAuBufMemCfg[searchStart+4].MemPhyBase = pstMemCfgAud->cap4_memory_base;
	_gAuBufMemCfg[searchStart+4].MemSize = pstMemCfgAud->cap4_memory_size;
	_gAuBufMemCfg[searchStart+4].MemName = pstMemCfgAud->cap4_memory_name;
	_gAuBufMemCfg[searchStart+4].pRegBase = (void *)&g_pRealRegAud->__rsvd_04[4];

	_gAuBufMemCfg[searchStart+5].MemPhyBase = pstMemCfgAud->cap5_memory_base;
	_gAuBufMemCfg[searchStart+5].MemSize = pstMemCfgAud->cap5_memory_size;
	_gAuBufMemCfg[searchStart+5].MemName = pstMemCfgAud->cap5_memory_name;
	_gAuBufMemCfg[searchStart+5].pRegBase = (void *)&g_pRealRegAud->__rsvd_04[8];

	_gAuBufMemCfg[searchStart+6].MemPhyBase = pstMemCfgAud->cap6_memory_base;
	_gAuBufMemCfg[searchStart+6].MemSize = pstMemCfgAud->cap6_memory_size;
	_gAuBufMemCfg[searchStart+6].MemName = pstMemCfgAud->cap6_memory_name;
	_gAuBufMemCfg[searchStart+6].pRegBase = (void *)&g_pRealRegAud->__rsvd_04[12];

	_gAuBufMemCfg[searchStart+7].MemPhyBase = pstMemCfgAud->cap7_memory_base;
	_gAuBufMemCfg[searchStart+7].MemSize = pstMemCfgAud->cap7_memory_size;
	_gAuBufMemCfg[searchStart+7].MemName = pstMemCfgAud->cap7_memory_name;
	_gAuBufMemCfg[searchStart+7].pRegBase = (void *)&g_pRealRegAud->__rsvd_04[16];

	//Mapping virtual memory address for dsp0 fw download memory
	if (NULL == g_pui32DSP0CodecVirAddr)
	{
		g_pui32DSP0CodecVirAddr = (UINT32*) ioremap(pstMemCfgAud->dsp0_memory_base + DSP0_VIR_ADDR_OFFSET,
													DSP0_VIR_MEM_SIZE);
		if (NULL == g_pui32DSP0CodecVirAddr)
		{
			AUD_KDRV_ERROR("AUDIO_LoadCodec : DSP0 Virtual address ERR!");
			return RET_ERROR;
		}
	}
	AUD_KDRV_PRINT("DSP0 CODEC Download address-phy [0x%08X]\n", (UINT32) pstMemCfgAud->dsp0_memory_base + DSP0_VIR_ADDR_OFFSET);
	AUD_KDRV_PRINT("DSP0 CODEC Download address-virt[0x%08X]\n", (UINT32) g_pui32DSP0CodecVirAddr);

	//Mapping virtual memory address for dsp1 fw download memory
	if (NULL == g_pui32DSP1CodecVirAddr)
	{
		g_pui32DSP1CodecVirAddr = (UINT32*) ioremap(pstMemCfgAud->dsp1_memory_base + DSP1_VIR_ADDR_OFFSET,
													DSP1_VIR_MEM_SIZE);
		if (NULL == g_pui32DSP1CodecVirAddr)
		{
			AUD_KDRV_ERROR("AUDIO_LoadCodec : DSP1 Virtual address ERR!");
			return RET_ERROR;
		}
	}
	AUD_KDRV_PRINT("DSP1 CODEC Download address-phy [0x%08X]\n", (UINT32) pstMemCfgAud->dsp1_memory_base + DSP1_VIR_ADDR_OFFSET);
	AUD_KDRV_PRINT("DSP1 CODEC Download address-virt[0x%08X]\n", (UINT32) g_pui32DSP1CodecVirAddr);

	// Mapping virtual memory address for CPB buffer
	for(k = 1; k < AUD_BUFFER_TYPE_MAX; k++)
	{
		searchStart = _gBufTypeBase[k];
		searchEnd = searchStart + _gBufTypeNum[k];

		for(i = searchStart; i < searchEnd; i++)
		{
			//Mapping virtual memory address for CPB0
			if (_gAuBufMemCfg[i].pMemVirBase == NULL)
			{
				_gAuBufMemCfg[i].pMemVirBase = (UINT32*) ioremap( _gAuBufMemCfg[i].MemPhyBase,_gAuBufMemCfg[i].MemSize);
				if (_gAuBufMemCfg[i].pMemVirBase == NULL)
				{
					AUD_KDRV_ERROR("%s : %s Virtual address ERR!", __F__, _gAuBufMemCfg[i].MemName);
					break;
				}
			}
			AUD_KDRV_PRINT("%s address-phy [0x%08X]\n", _gAuBufMemCfg[i].MemName,(UINT32)_gAuBufMemCfg[i].MemPhyBase);
			AUD_KDRV_PRINT("%s address-virt [0x%08X]\n", _gAuBufMemCfg[i].MemName,(UINT32)_gAuBufMemCfg[i].pMemVirBase);
		}
	}

	//Mapping virtual memory address for sound engine(SE) parameter memory
	if (NULL == g_pui32SEParamVirAddr)
	{
		g_pui32SEParamVirAddr = (UINT32 *) ioremap(pstMemCfgAud->se_memory_base,
												   pstMemCfgAud->se_memory_size);
		if (NULL == g_pui32SEParamVirAddr)
		{
			AUD_KDRV_ERROR ("AUDIO_LoadCodec : LGSE Param Virtual address ERR!");
			return RET_ERROR;
		}
	}
	AUD_KDRV_PRINT("AUD SE address-phy [0x%08X]\n", (UINT32)pstMemCfgAud->se_memory_base);
	AUD_KDRV_PRINT("AUD SE address-virt[0x%08X]\n", (UINT32)g_pui32SEParamVirAddr);

	// Mapping virtual memory address for IPC
	if (NULL == g_pui32IPC_VirAddr)
	{
		g_pui32IPC_VirAddr = (UINT32*) ioremap(pstMemCfgAud->ipc_memory_base, pstMemCfgAud->ipc_memory_size);
		if (NULL == g_pui32IPC_VirAddr)
		{
			AUD_KDRV_ERROR("%s : IPC Base address ERR!", __F__);
			return RET_ERROR;
		}
	}
	AUD_KDRV_PRINT("IPC address-phy [0x%08X]\n", (UINT32) pstMemCfgAud->ipc_memory_base);
	AUD_KDRV_PRINT("IPC address-virt[0x%08X]\n", (UINT32) g_pui32IPC_VirAddr);

	return ret;
}

/**
 * Write a Audio HW Register.
 * @see
*/
SINT32 M14_A0_AUDIO_WriteReg ( LX_AUD_REG_ADDRESS_T addr, UINT32 data )
{
	SINT32 ret = RET_OK;

	//Write Register
	if(addr == LX_AUD_REG_DEBUG_PRINT_MASK)
	{
		AUD_M14_A0_Wr(aud_dbg_print_mask, data);
		AUD_M14_A0_WrFL(aud_dbg_print_mask);
	}
	else if(addr == LX_AUD_REG_INT0_CLR)
	{
		AUD_M14_A0_Wr(aud_int0_clr, data);
		AUD_M14_A0_WrFL(aud_int0_clr);
	}
	else if(addr == LX_AUD_REG_INT1_CLR)
	{
		AUD_M14_A0_Wr(aud_int1_clr, data);
		AUD_M14_A0_WrFL(aud_int1_clr);
	}
	else if(addr == LX_AUD_REG_DSP0_GSTC)
	{
		AUD_M14_A0_Wr(aud_dsp0_gstc, data);
		AUD_M14_A0_WrFL(aud_dsp0_gstc);
	}
	else if(addr == LX_AUD_REG_DSP1_GSTC)
	{
		AUD_M14_A0_Wr(aud_dsp1_gstc, data);
		AUD_M14_A0_WrFL(aud_dsp1_gstc);
	}
	else if(addr == LX_AUD_REG_DEBUG_DSP0_RESET)
	{
		AUD_M14_A0_Wr(aud_dbg_dsp0_reset, data);
		AUD_M14_A0_WrFL(aud_dbg_dsp0_reset);
	}
	else if(addr == LX_AUD_REG_DEBUG_DSP1_RESET)
	{
		AUD_M14_A0_Wr(aud_dbg_dsp1_reset, data);
		AUD_M14_A0_WrFL(aud_dbg_dsp1_reset);
	}
	else if(addr == LX_AUD_REG_DEC0_CBT)
	{
		AUD_M14_A0_Wr(aud_dec0_cbt, data);
		AUD_M14_A0_WrFL(aud_dec0_cbt);
	}
	else if(addr == LX_AUD_REG_DEC0_SBT)
	{
		AUD_M14_A0_Wr(aud_dec0_sbt, data);
		AUD_M14_A0_WrFL(aud_dec0_sbt);
	}
	else if(addr == LX_AUD_REG_DEC1_CBT)
	{
		AUD_M14_A0_Wr(aud_dec1_cbt, data);
		AUD_M14_A0_WrFL(aud_dec1_cbt);
	}
	else if(addr == LX_AUD_REG_DEC1_SBT)
	{
		AUD_M14_A0_Wr(aud_dec1_sbt, data);
		AUD_M14_A0_WrFL(aud_dec1_sbt);
	}
	else if(addr == LX_AUD_REG_GSTC_LOW)
	{
		AUD_M14_A0_Wr(aud_gstcc0lreg, data);
		AUD_M14_A0_WrFL(aud_gstcc0lreg);
	}
	else if(addr == LX_AUD_REG_LEVELMO_L)
	{
		AUD_M14_A0_Wr(aud_levelmo_l, data);
		AUD_M14_A0_WrFL(aud_levelmo_l);
	}
	else if(addr == LX_AUD_REG_LEVELMO_R)
	{
		AUD_M14_A0_Wr(aud_levelmo_r, data);
		AUD_M14_A0_WrFL(aud_levelmo_r);
	}
	else if(addr == LX_AUD_REG_AAD_BYPASS)
	{
		//Enable SCART bypass function for AUD reg.
		AUD_M14_A0_RdFL(aad_bypass);
		AUD_M14_A0_Wr01(aad_bypass, aad2scartbypassctrl, data);
		AUD_M14_A0_WrFL(aad_bypass);
	}
	else if(addr == LX_AUD_REG_DSP0_IRQ)
	{
		AUD_M14_A0_Wr(aud_dirq0, data);
		AUD_M14_A0_WrFL(aud_dirq0);
	}
	else if(addr == LX_AUD_REG_DSP1_IRQ)
	{
		AUD_M14_A0_Wr(aud_dirq1, data);
		AUD_M14_A0_WrFL(aud_dirq1);
	}

	return ret;
}

/**
 * Read a Audio HW Register.
 * @see
*/
UINT32 M14_A0_AUDIO_ReadReg ( LX_AUD_REG_ADDRESS_T addr )
{
	UINT32 data = 0;
	UINT32 free_size = 0;
	UINT32 max_size  = 0;

	//Read Register
	if(addr == LX_AUD_REG_DEBUG_PRINT_MASK)
	{
		AUD_M14_A0_RdFL(aud_dbg_print_mask);
		data = AUD_M14_A0_Rd(aud_dbg_print_mask);
	}
	else if(addr == LX_AUD_REG_INT0_CLR)
	{
		AUD_M14_A0_RdFL(aud_int0_clr);
		data = AUD_M14_A0_Rd(aud_int0_clr);
	}
	else if(addr == LX_AUD_REG_INT1_CLR)
	{
		AUD_M14_A0_RdFL(aud_int1_clr);
		data = AUD_M14_A0_Rd(aud_int1_clr);
	}
	else if(addr == LX_AUD_REG_DSP0_GSTC)
	{
		AUD_M14_A0_RdFL(aud_dsp0_gstc);
		data = AUD_M14_A0_Rd(aud_dsp0_gstc);
	}
	else if(addr == LX_AUD_REG_DSP1_GSTC)
	{
		AUD_M14_A0_RdFL(aud_dsp1_gstc);
		data = AUD_M14_A0_Rd(aud_dsp1_gstc);
	}
	else if(addr == LX_AUD_REG_DEBUG_DSP0_RESET)
	{
		AUD_M14_A0_RdFL(aud_dbg_dsp0_reset);
		data = AUD_M14_A0_Rd(aud_dbg_dsp0_reset);
	}
	else if(addr == LX_AUD_REG_DEBUG_DSP1_RESET)
	{
		AUD_M14_A0_RdFL(aud_dbg_dsp1_reset);
		data = AUD_M14_A0_Rd(aud_dbg_dsp1_reset);
	}
	else if(addr == LX_AUD_REG_DEC0_CBT)
	{
		AUD_M14_A0_RdFL(aud_dec0_cbt);
		data = AUD_M14_A0_Rd(aud_dec0_cbt);
	}
	else if(addr == LX_AUD_REG_DEC0_SBT)
	{
		AUD_M14_A0_RdFL(aud_dec0_sbt);
		data = AUD_M14_A0_Rd(aud_dec0_sbt);
	}
	else if(addr == LX_AUD_REG_DEC1_CBT)
	{
		AUD_M14_A0_RdFL(aud_dec1_cbt);
		data = AUD_M14_A0_Rd(aud_dec1_cbt);
	}
	else if(addr == LX_AUD_REG_DEC1_SBT)
	{
		AUD_M14_A0_RdFL(aud_dec1_sbt);
		data = AUD_M14_A0_Rd(aud_dec1_sbt);
	}
	else if(addr == LX_AUD_REG_GSTC_LOW)
	{
		AUD_M14_A0_RdFL(aud_gstcc0lreg);
		data = AUD_M14_A0_Rd(aud_gstcc0lreg);
	}
	else if(addr == LX_AUD_REG_LEVELMO_L)
	{
		AUD_M14_A0_RdFL(aud_levelmo_l);
		data = AUD_M14_A0_Rd(aud_levelmo_l);
	}
	else if(addr == LX_AUD_REG_LEVELMO_R)
	{
		AUD_M14_A0_RdFL(aud_levelmo_r);
		data = AUD_M14_A0_Rd(aud_levelmo_r);
	}
	else if(addr == LX_AUD_REG_AAD_BYPASS)
	{
		AUD_M14_A0_RdFL(aad_bypass);
		AUD_M14_A0_Rd01(aad_bypass, aad2scartbypassctrl, data);
	}
	else if(addr == LX_AUD_REG_DSP0_IRQ)
	{
		AUD_M14_A0_RdFL(aud_dirq0);
		data = AUD_M14_A0_Rd(aud_dirq0);
	}
	else if(addr == LX_AUD_REG_DSP1_IRQ)
	{
		AUD_M14_A0_RdFL(aud_dirq1);
		data = AUD_M14_A0_Rd(aud_dirq1);
	}
	else if(addr == LX_AUD_REG_TPM_BUFSIZE)
	{
		AUD_M14_A0_RdFL(aud_tpm_bufsize);
		AUD_M14_A0_Rd01(aud_tpm_bufsize, free_size, free_size);
		AUD_M14_A0_Rd01(aud_tpm_bufsize, max_size, max_size);
		data = (max_size << 16) | free_size;
	}
	else if(addr == LX_AUD_REG_TPA_BUFSIZE)
	{
		AUD_M14_A0_RdFL(aud_tpa_bufsize);
		AUD_M14_A0_Rd01(aud_tpa_bufsize, free_size, free_size);
		AUD_M14_A0_Rd01(aud_tpa_bufsize, max_size, max_size);
		data = (max_size << 16) | free_size;
	}

	return data;
}

/**
 *  Resets a AAD Module using H/W reset signal.
 *  This function resets a AAD module to clear previous setting.
 * @see
*/
BOOLEAN M14_A0_AUDIO_CheckResetStatus ( UINT8 ui8ResetFlag )
{
	UINT32	dsp0bresetout;
	UINT32	dsp1bresetout;

	BOOLEAN bStatus = FALSE;

	if(ui8ResetFlag == AUD_OPTION_DSP0)
	{
		//Check a DSP0 reset value for in boot/app code
		AUD_M14_A0_RdFL(aud_swreset);
		AUD_M14_A0_Rd01(aud_swreset, dsp0bresetout, dsp0bresetout);	//12 dsp1bresetout : reset for DSP0

		//Reset Low : ADEC DSP0 : In boot code, DSP0 is running, so DSP0 memory data must be flushed.
		if(dsp0bresetout == 0x1)
		{
			bStatus = TRUE;
		}
	}
	else if(ui8ResetFlag == AUD_OPTION_DSP1)
	{
		//Check a DSP1 reset value for in boot/app code
		AUD_M14_A0_RdFL(aud_swreset);
		AUD_M14_A0_Rd01(aud_swreset, dsp1bresetout, dsp1bresetout);	//13 dsp1bresetout : reset for DSP1

		//Reset Low : ADEC DSP1 : In boot code, DSP1 is running, so DSP1 memory data must be flushed.
		if(dsp1bresetout == 0x1)
		{
			bStatus = TRUE;
		}
	}

	return bStatus;
}

/**
 * Set a reset for ADEC block.
 * @see
*/
SINT32 M14_A0_AUDIO_SetReset ( UINT8 ui8ResetFlag )
{
	AUD_M14_A0_RdFL(aud_swreset);

	if (ui8ResetFlag == AUD_OPTION_DSP0)
	{
		/* Reset g_AudKdrv.IsDSP0Ready value */
		g_AudKdrv.IsDSP0Ready  = 0;

		/* Set DSP0 state after first call */
		g_AudKdrv.IsDSP0Reseted	= 1;

		AUD_M14_A0_Wr01(aud_swreset, dsp0ocdresetout, 1);		//10	dsp0ocdresetout    	reset for On chip debugger of DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp0bresetout, 1);		//12	dsp0bresetout      	reset for DSP0
	}
	else if (ui8ResetFlag == AUD_OPTION_DSP1)
	{
		/* Reset g_AudKdrv.IsDSP0Ready value */
		g_AudKdrv.IsDSP1Ready	= 0;

		/* Set DSP1 state after first call */
		g_AudKdrv.IsDSP1Reseted = 1;

		AUD_M14_A0_Wr01(aud_swreset, dsp1ocdresetout, 1);		//11	dsp1ocdresetout    	reset for On chip debugger of DSP1
		AUD_M14_A0_Wr01(aud_swreset, dsp1bresetout, 1);		//13	dsp1bresetout      	reset for DSP1
	}
	else if (ui8ResetFlag == AUD_OPTION_AAD)
	{
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 1);			//14 	aadresetout        	reset for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 1);		//15 	aadapbresetout     	reset for AAD register interface
	}
	else if (ui8ResetFlag == AUD_OPTION_OTHERS)
	{
		AUD_M14_A0_Wr01(aud_swreset, aresetout, 1);		//0	 	aresetout          	reset for operation of peripherals
		AUD_M14_A0_Wr01(aud_swreset, apbresetout, 1);		//1	 	apbresetout        	reset for register interface if this set, all registers are cleared
		AUD_M14_A0_Wr01(aud_swreset, aversetout, 1);			//2  	aversetout         	reset for TE data input interface,
		AUD_M14_A0_Wr01(aud_swreset, veresetout, 0);			 //3  	veresetout         	reset for TE data output interface (encoder)
		AUD_M14_A0_Wr01(aud_swreset, fs20resetout, 1);		//4	 	fs20resetout       	reset for audio PCM output interface
		AUD_M14_A0_Wr01(aud_swreset, fs21resetout, 1);		//5		fs21resetout       	reset for audio SPDIF output interface
		AUD_M14_A0_Wr01(aud_swreset, fs23resetout, 1);		//6 	fs23resetout		reset for audio HWSRC0 data input
		AUD_M14_A0_Wr01(aud_swreset, fs24resetout, 1);		//7 	fs24resetout		reset for audio HWSRC0 datat output
		AUD_M14_A0_Wr01(aud_swreset, asrcrstout, 1);		//9	 	asrcrstout         	reset for audio HWSRC operation
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 1);		//14 	aadresetout        	reest for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 1);	//15 	aadapbresetout     	reset for AAD register interface
		AUD_M14_A0_Wr01(aud_swreset, uartresetout, 1);		//16	uartresetout		reset for audio UART
	}
	else if (ui8ResetFlag == AUD_OPTION_ALL)
	{
		AUD_M14_A0_Wr01(aud_swreset, aresetout, 1);			//0 	aresetout			reset for operation of peripherals
		AUD_M14_A0_Wr01(aud_swreset, apbresetout, 1);				//1 	apbresetout 		reset for register interface if this set, all registers are cleared
		AUD_M14_A0_Wr01(aud_swreset, aversetout, 1);				//2 	aversetout			reset for TE data input interface,
		AUD_M14_A0_Wr01(aud_swreset, veresetout, 1);				//3 	veresetout			reset for TE data output interface (encoder)
		AUD_M14_A0_Wr01(aud_swreset, fs20resetout, 1);				//4 	fs20resetout		reset for audio PCM output interface
		AUD_M14_A0_Wr01(aud_swreset, fs21resetout, 1);			//5 	fs21resetout		reset for audio SPDIF output interface
		AUD_M14_A0_Wr01(aud_swreset, fs23resetout, 1);			//6 	fs23resetout		reset for audio HWSRC0 data input
		AUD_M14_A0_Wr01(aud_swreset, fs24resetout, 1);			//7 	fs24resetout		reset for audio HWSRC0 datat output
		AUD_M14_A0_Wr01(aud_swreset, asrcrstout, 1);			//9 	asrcrstout			reset for audio HWSRC operation
		AUD_M14_A0_Wr01(aud_swreset, dsp0ocdresetout, 1);		//10	dsp0ocdresetout    	reset for On chip debugger of DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp1ocdresetout, 1);		//11	dsp1ocdresetout    	reset for On chip debugger of DSP1
		AUD_M14_A0_Wr01(aud_swreset, dsp0bresetout, 1);			//12	dsp0bresetout      	reset for DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp1bresetout, 1);			//13	dsp1bresetout      	reset for DSP1
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 1);			//14	aadresetout 		reest for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 1);		//15	aadapbresetout		reset for AAD register interface
		AUD_M14_A0_Wr01(aud_swreset, uartresetout, 1);			//16	uartresetout		reset for audio UART
	}

	AUD_M14_A0_WrFL(aud_swreset);
	AUD_KDRV_PRINT("Reset reg val [0x%x]\n", AUD_M14_A0_Rd(aud_swreset) );

	//Set delay for reset high duration for stable operation
	if (ui8ResetFlag == AUD_OPTION_DSP0)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("ADEC_SetReset : AUD_OPTION_DSP0!!!\n");

		//Enable audio interrupt 0
		AUD_M14_A0_Wr(aud_int0_en, 0x1);
		AUD_M14_A0_WrFL(aud_int0_en);
	}
	else if (ui8ResetFlag == AUD_OPTION_DSP1)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("ADEC_SetReset : AUD_OPTION_DSP1!!!\n");

		//Enable audio interrupt 1
		AUD_M14_A0_Wr(aud_int1_en, 0x1);
		AUD_M14_A0_WrFL(aud_int1_en);
	}
	else if (ui8ResetFlag == AUD_OPTION_AAD)
	{
		udelay(AAD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("ADEC_SetReset : AUD_OPTION_AAD!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_OTHERS)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("ADEC_SetReset : AUD_OPTION_OTHERS!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_ALL)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("ADEC_SetReset : AUD_OPTION_ALL!!!\n");
	}

	return RET_OK;
}

/**
 * Clear a reset for AUDIO block.
 * @see
*/
SINT32 M14_A0_AUDIO_ClearReset ( UINT8 ui8ResetFlag )
{
	UINT32		aresetout, apbresetout;
	UINT32		flushDoneStatus = 1, timeout = 0;

	AUD_M14_A0_RdFL(aud_swreset);

	//Check a reset value for in boot code
	//Not reset a adec apb if boot code is reset before to protect a address switch malfunction
	AUD_M14_A0_Rd01(aud_swreset, aresetout, aresetout);		//0 	aresetout			reset for operation of peripherals
	AUD_M14_A0_Rd01(aud_swreset, apbresetout, apbresetout);		//1	apbresetout 		reset for register interface if this set, all registers are cleared

	if (ui8ResetFlag == AUD_OPTION_DSP0)
	{
		//Disable audio interrupt DSP0
		AUD_M14_A0_Wr(aud_int0_en, 0x0);
		AUD_M14_A0_WrFL(aud_int0_en);

		AUD_M14_A0_Wr01(aud_swreset, dsp0ocdresetout, 0);	//10 dsp0ocdresetout    	reset for On chip debugger of DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp0bresetout, 0);		//12 dsp0bresetout      	reset for DSP0
	}
	else if (ui8ResetFlag == AUD_OPTION_DSP1)
	{
		//Disable audio interrupt DSP1
		AUD_M14_A0_Wr(aud_int1_en, 0x0);
		AUD_M14_A0_WrFL(aud_int1_en);

		AUD_M14_A0_Wr01(aud_swreset, dsp1ocdresetout, 0);	//11 dsp1ocdresetout    	reset for On chip debugger of DSP1
		AUD_M14_A0_Wr01(aud_swreset, dsp1bresetout, 0);		//13 dsp1bresetout      	reset for DSP1
	}
	else if (ui8ResetFlag == AUD_OPTION_AAD)
	{
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 0);			//14 aadresetout        	reest for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 0);		//15 aadapbresetout     	reset for AAD register interface
	}
	else if (ui8ResetFlag == AUD_OPTION_OTHERS)
	{
		//This reset is cleared oncely after booting
		if(apbresetout == 0)
			AUD_M14_A0_Wr01(aud_swreset, aresetout, 0);		//0	 	aresetout          	reset for operation of peripherals

		//This reset is cleared oncely after booting
		if(apbresetout == 0)
			AUD_M14_A0_Wr01(aud_swreset, apbresetout, 0);		//1	 	apbresetout        	reset for register interface if this set, all registers are cleared

		AUD_M14_A0_Wr01(aud_swreset, aversetout, 0);					//2   aversetout         	reset for TE data input interface,
		AUD_M14_A0_Wr01(aud_swreset, veresetout, 0);			//3	 	veresetout         	reset for TE data output interface (encoder)

		AUD_M14_A0_Wr01(aud_swreset, fs20resetout, 0);			//4	 	fs20resetout       	reset for audio PCM output interface
		AUD_M14_A0_Wr01(aud_swreset, fs21resetout, 0);			//5	 	fs21resetout      	reset for audio SPDIF output interface
		AUD_M14_A0_Wr01(aud_swreset, fs23resetout, 0);			//6 	fs23resetout		reset for audio HWSRC0 data input
		AUD_M14_A0_Wr01(aud_swreset, fs24resetout, 0);			//7 	fs24resetout		reset for audio HWSRC0 datat output
		AUD_M14_A0_Wr01(aud_swreset, asrcrstout, 0);			//9	 	asrcrstout         	reset for audio HWSRC operation
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 0);			//14 	aadresetout        	reest for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 0);		//15 	aadapbresetout     	reset for AAD register interface
		AUD_M14_A0_Wr01(aud_swreset, uartresetout, 0);			//16	uartresetout		reset for audio UART
	}
	else if (ui8ResetFlag == AUD_OPTION_ALL)
	{
		//This reset is cleared oncely after booting
		if(apbresetout == 0)
			AUD_M14_A0_Wr01(aud_swreset, aresetout, 0);		//0	 	aresetout          	reset for operation of peripherals

		//This reset is cleared oncely after booting
		if(apbresetout == 0)
			AUD_M14_A0_Wr01(aud_swreset, apbresetout, 0);		//1	 	apbresetout        	reset for register interface if this set, all registers are cleared

		AUD_M14_A0_Wr01(aud_swreset, aversetout, 0);					//2   aversetout         	reset for TE data input interface,
		AUD_M14_A0_Wr01(aud_swreset, veresetout, 0);			//3	 	veresetout         	reset for TE data output interface (encoder)

		AUD_M14_A0_Wr01(aud_swreset, fs20resetout, 0);			//4	 	fs20resetout       	reset for audio PCM output interface
		AUD_M14_A0_Wr01(aud_swreset, fs21resetout, 0);			//5	 	fs21resetout      	reset for audio SPDIF output interface
		AUD_M14_A0_Wr01(aud_swreset, fs23resetout, 0);			//6 	fs23resetout		reset for audio HWSRC0 data input
		AUD_M14_A0_Wr01(aud_swreset, fs24resetout, 0);			//7 	fs24resetout		reset for audio HWSRC0 datat output
		AUD_M14_A0_Wr01(aud_swreset, asrcrstout, 0);			//9	 	asrcrstout         	reset for audio HWSRC operation
		AUD_M14_A0_Wr01(aud_swreset, dsp0ocdresetout, 0);		//10	dsp0ocdresetout    	reset for On chip debugger of DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp1ocdresetout, 0);		//11	dsp1ocdresetout    	reset for On chip debugger of DSP1
		AUD_M14_A0_Wr01(aud_swreset, dsp0bresetout, 0);			//12	dsp0bresetout      	reset for DSP0
		AUD_M14_A0_Wr01(aud_swreset, dsp1bresetout, 0);			//13	dsp1bresetout      	reset for DSP1
		AUD_M14_A0_Wr01(aud_swreset, aadresetout, 0);			//14	aadresetout 		reest for operation of Analog audio decoder
		AUD_M14_A0_Wr01(aud_swreset, aadapbresetout, 0);		//15	aadapbresetout		reset for AAD register interface
		AUD_M14_A0_Wr01(aud_swreset, uartresetout, 0);			//16	uartresetout		reset for audio UART
	}

	//Flush AUD DSP memory access
	if (g_AudKdrv.IsDSP0Reseted == 1 && ui8ResetFlag == AUD_OPTION_DSP0)
	{
		g_AudKdrv.IsDSP0Reseted = 0;

		//Stop AUD DSP 0
		AUD_M14_A0_Wr(aud_runstall0, 0x1);
		AUD_M14_A0_WrFL(aud_runstall0);

		//Set flush request register for ADEC DSP0
		CPU_TOP_M14A0_RdFL(flush_req);
		CPU_TOP_M14A0_Wr01(flush_req, aud_dsp0_flush_en, 0x1);
		CPU_TOP_M14A0_WrFL(flush_req);

		AUD_KDRV_DEBUG("Wait for AUD DSP0 Memory Flush!!!\n");

		//Check flush done for DSP0
		for(timeout = 0; timeout < AUD_DSP_FLUSH_ITERATION; timeout++)
		{
			udelay(1);

			CPU_TOP_M14A0_RdFL(flush_done_status);
			CPU_TOP_M14A0_Rd01(flush_done_status, aud_dsp0_flush_done, flushDoneStatus);

			if(flushDoneStatus)
				break;
			else
				AUD_KDRV_DEBUG("Wait for AUD DSP0 Memory Flush!!!\n");
		}

		//Check if timeout is reached
		if(timeout == AUD_DSP_FLUSH_ITERATION)
		{
			AUD_KDRV_DEBUG("CPU_TOP_MAU_FLUSH_DONE_REG : DSP0 ERROR!!!\n");
		}
	}
	else if (g_AudKdrv.IsDSP1Reseted == 1 && ui8ResetFlag == AUD_OPTION_DSP1)
	{
		g_AudKdrv.IsDSP1Reseted	= 0;

		//Stop AUD DSP 1
		AUD_M14_A0_Wr(aud_runstall1, 0x1);
		AUD_M14_A0_WrFL(aud_runstall1);

		//Set flush request register for ADEC DSP1
		CPU_TOP_M14A0_RdFL(flush_req);
		CPU_TOP_M14A0_Wr01(flush_req, aud_dsp1_flush_en, 0x1);
		CPU_TOP_M14A0_WrFL(flush_req);

		AUD_KDRV_DEBUG("Wait for AUD DSP1 Memory Flush!!!\n");

		//Check flush done for DSP1
		for(timeout = 0; timeout < AUD_DSP_FLUSH_ITERATION; timeout++)
		{
			udelay(1);

			CPU_TOP_M14A0_RdFL(flush_done_status);
			CPU_TOP_M14A0_Rd01(flush_done_status, aud_dsp1_flush_done, flushDoneStatus);

			if(flushDoneStatus)
				break;
			else
				AUD_KDRV_DEBUG("Wait for AUD DSP1 Memory Flush!!!\n");

		}

		//Check if timeout is reached
		if(timeout == AUD_DSP_FLUSH_ITERATION)
		{
			AUD_KDRV_DEBUG("CPU_TOP_MAU_FLUSH_DONE_REG : DSP1 ERROR!!!\n");
		}
	}

	AUD_M14_A0_WrFL(aud_swreset);
	AUD_KDRV_PRINT("Reset reg val [0x%x]\n", AUD_M14_A0_Rd(aud_swreset) );

	//Set delay for reset low duration
	if (ui8ResetFlag == AUD_OPTION_DSP0)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("AUDIO_ClearReset : AUD_OPTION_DSP0!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_DSP1)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("AUDIO_ClearReset : AUD_OPTION_DSP1!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_AAD)
	{
		udelay(AAD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("AUDIO_ClearReset : AUD_OPTION_AAD!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_OTHERS)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("AUDIO_ClearReset : AUD_OPTION_OTHERS!!!\n");
	}
	else if (ui8ResetFlag == AUD_OPTION_ALL)
	{
		udelay(AUD_RESET_DELAY);	//IC => 50 clk : 125ns
		AUD_KDRV_PRINT("AUDIO_ClearReset : AUD_OPTION_ALL!!!\n");
	}

	//Reset Flush Bit for ADEC DSP memory access
	if (g_AudKdrv.IsDSP0Reseted == 0 && ui8ResetFlag == AUD_OPTION_DSP0)
	{
		//Clear flush request register for ADEC DSP0
		CPU_TOP_M14A0_RdFL(flush_req);
		CPU_TOP_M14A0_Wr01(flush_req, aud_dsp0_flush_en, 0x0);
		CPU_TOP_M14A0_WrFL(flush_req);

		AUD_KDRV_PRINT("Reset ADEC DSP0 Memory Flush!!!\n");

		//Start ADEC DSP 0
		AUD_M14_A0_Wr(aud_runstall0, 0x0);
		AUD_M14_A0_WrFL(aud_runstall0);
	}
	else if (g_AudKdrv.IsDSP1Reseted == 0 && ui8ResetFlag == AUD_OPTION_DSP1)
	{
		//Clear flush request register for ADEC DSP1
		CPU_TOP_M14A0_RdFL(flush_req);
		CPU_TOP_M14A0_Wr01(flush_req, aud_dsp1_flush_en, 0x0);
		CPU_TOP_M14A0_WrFL(flush_req);

		AUD_KDRV_PRINT("Reset ADEC DSP1 Memory Flush!!!\n");

		//Start ADEC DSP 1
		AUD_M14_A0_Wr(aud_runstall1, 0x0);
		AUD_M14_A0_WrFL(aud_runstall1);
	}

	return RET_OK;
}

/**
 * Initialize Audio registers.
 * @see AUDIO_InitModule
*/
SINT32 M14_A0_AUDIO_InitRegister ( void )
{
 	UINT64	start = 0, elapsed = 0;

	//Get a current mili-second time
	start = jiffies;

	//Set download address to ADEC DSP memory
	//set address for DSP0
	AUD_M14_A0_Wr(aud_dsp0offset5, pstMemCfgAud->dsp0_memory_base + DSP0_VIR_ADDR_OFFSET);
	AUD_M14_A0_WrFL(aud_dsp0offset5);

	AUD_M14_A0_Wr(aud_dsp0offset6, pstMemCfgAud->dsp0_memory_base);
	AUD_M14_A0_WrFL(aud_dsp0offset6);

	//Share area for DSP0
	AUD_M14_A0_Wr(aud_dsp0offset61, pstMemCfgAud->dpb0_memory_base);
	AUD_M14_A0_WrFL(aud_dsp0offset61);

	//set address for DSP1
	AUD_M14_A0_Wr(aud_dsp1offset5, pstMemCfgAud->dsp1_memory_base + DSP1_VIR_ADDR_OFFSET);
	AUD_M14_A0_WrFL(aud_dsp1offset5);

	AUD_M14_A0_Wr(aud_dsp1offset6, pstMemCfgAud->dsp1_memory_base);
	AUD_M14_A0_WrFL(aud_dsp1offset6);

	//Share area for DSP1
	AUD_M14_A0_Wr(aud_dsp1offset61, pstMemCfgAud->dpb0_memory_base);
	AUD_M14_A0_WrFL(aud_dsp1offset61);

	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp0offset5[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp0offset5));
	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp0offset6[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp0offset6));
	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp0offset6[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp0offset61));
	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp1offset5[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp1offset5));
	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp1offset6[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp1offset6));
	AUD_KDRV_DEBUG_TMP("ADEC_RegInit - aud_dsp1offset6[0x%08X]\n", AUD_M14_A0_Rd(aud_dsp1offset61));

	//Set base address and size
	AUD_M14_A0_Wr(aud_mem_share_base, pstMemCfgAud->dpb0_memory_base);
	AUD_M14_A0_WrFL(aud_mem_share_base);
	AUD_M14_A0_RdFL(aud_mem_share_base);

	//Set a DSP1 Share Memory Address
	g_AudKdrv.p_aud_mem_share_base = (void *)&g_pRealRegAud->aud_mem_share_base;

	AUD_M14_A0_Wr(aud_mem_dsp0_base, pstMemCfgAud->dsp0_memory_base);
	AUD_M14_A0_WrFL(aud_mem_dsp0_base);

	AUD_M14_A0_Wr(aud_mem_dsp1_base, pstMemCfgAud->dsp1_memory_base);
	AUD_M14_A0_WrFL(aud_mem_dsp1_base);

	//Sound Engine parameters
	AUD_M14_A0_Wr(aud_mem_se_param_base, pstMemCfgAud->se_memory_base);
	AUD_M14_A0_WrFL(aud_mem_se_param_base);
	AUD_M14_A0_Wr(aud_mem_se_param_size, pstMemCfgAud->se_memory_size);
	AUD_M14_A0_WrFL(aud_mem_se_param_size);

	//IPC
	// from ARM to DSP0 (DSP0 - Write: IPC_WRITE_TO_DSP0)
	AUD_M14_A0_Wr(aud_mem_ipc_a2d_base, pstMemCfgAud->ipc_memory_base);
	AUD_M14_A0_WrFL(aud_mem_ipc_a2d_base);
	AUD_M14_A0_Wr(aud_mem_ipc_a2d_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_a2d_size);

	// from DSP0 to ARM (DSP0 - Read:  IPC_READ_FROM_DSP0)
	AUD_M14_A0_Wr(aud_mem_ipc_d2a_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 1));
	AUD_M14_A0_WrFL(aud_mem_ipc_d2a_base);
	AUD_M14_A0_Wr(aud_mem_ipc_d2a_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_d2a_size);

	// from ARM to DSP1 (DSP1 - Write: IPC_WRITE_TO_DSP1)
	AUD_M14_A0_Wr(aud_mem_ipc_a2p_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 2));
	AUD_M14_A0_WrFL(aud_mem_ipc_a2p_base);
	AUD_M14_A0_Wr(aud_mem_ipc_a2p_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_a2p_size);

	// from DSP1 to ARM (DSP1 - Read: IPC_READ_FROM_DSP1)
	AUD_M14_A0_Wr(aud_mem_ipc_p2a_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 3));
	AUD_M14_A0_WrFL(aud_mem_ipc_p2a_base);
	AUD_M14_A0_Wr(aud_mem_ipc_p2a_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_p2a_size);

	// from DSP0 to DSP1
	AUD_M14_A0_Wr(aud_mem_ipc_d2p_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 4));
	AUD_M14_A0_WrFL(aud_mem_ipc_d2p_base);
	AUD_M14_A0_Wr(aud_mem_ipc_d2p_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_d2p_size);

	// from DSP1 to DSP0
	AUD_M14_A0_Wr(aud_mem_ipc_p2d_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 5));
	AUD_M14_A0_WrFL(aud_mem_ipc_p2d_base);
	AUD_M14_A0_Wr(aud_mem_ipc_p2d_size, AUDIO_IPC_BUF_SIZE);
	AUD_M14_A0_WrFL(aud_mem_ipc_p2d_size);

	// DEBUG - DSP0
	AUD_M14_A0_Wr(aud_mem_ipc_dbgd_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 6));
	AUD_M14_A0_WrFL(aud_mem_ipc_dbgd_base);
	AUD_M14_A0_Wr(aud_mem_ipc_dbgd_size, (AUDIO_IPC_BUF_SIZE << 1));
	AUD_M14_A0_WrFL(aud_mem_ipc_dbgd_size);

	// DEBUG - DSP1
	AUD_M14_A0_Wr(aud_mem_ipc_dbgp_base, pstMemCfgAud->ipc_memory_base + (AUDIO_IPC_BUF_SIZE * 8));
	AUD_M14_A0_WrFL(aud_mem_ipc_dbgp_base);
	AUD_M14_A0_Wr(aud_mem_ipc_dbgp_size, (AUDIO_IPC_BUF_SIZE << 1));
	AUD_M14_A0_WrFL(aud_mem_ipc_dbgp_size);

	// Basetime of Lipsync
	AUD_M14_A0_Wr(aud_dec0_cbt, 0xFFFFFFFF);	// Init DEC0 Clock Basetime
	AUD_M14_A0_WrFL(aud_dec0_cbt);
	AUD_M14_A0_Wr(aud_dec0_sbt, 0xFFFFFFFF);	// Init DEC0 Stream Basetime
	AUD_M14_A0_WrFL(aud_dec0_sbt);
	AUD_M14_A0_Wr(aud_dec1_cbt, 0xFFFFFFFF);	// Init DEC1 Clock Basetime
	AUD_M14_A0_WrFL(aud_dec1_cbt);
	AUD_M14_A0_Wr(aud_dec1_sbt, 0xFFFFFFFF);	// Init DEC1 Stream Basetime
	AUD_M14_A0_WrFL(aud_dec1_sbt);
	AUD_M14_A0_Wr(aud_basepts, 0xFFFFFFFF);	// Init MHEG Stream Basetime
	AUD_M14_A0_WrFL(aud_basepts);
	AUD_M14_A0_Wr(aud_basestc, 0xFFFFFFFF);	// Init MHEG Clock Basetime
	AUD_M14_A0_WrFL(aud_basestc);

	// Presented PTS at DEC0
	AUD_M14_A0_Wr(aud_presented_pts_lip0, 0xFFFFFFFF);	// Init Presented PTS at DEC0
	AUD_M14_A0_WrFL(aud_presented_pts_lip0);

	// Presented PTS at DEC1
	AUD_M14_A0_Wr(aud_presented_pts_lip1, 0xFFFFFFFF);	// Init Presented PTS at DEC1
	AUD_M14_A0_WrFL(aud_presented_pts_lip1);

	// TP Buffer size at DEC0
	AUD_M14_A0_Wr01(aud_tpm_bufsize, max_size, 0x3A0);	// Init TP Buffer Max size at DEC0
	AUD_M14_A0_Wr01(aud_tpm_bufsize, free_size, 0x380);	// Init TP Buffer Free size at DEC0
	AUD_M14_A0_WrFL(aud_tpm_bufsize);

	// TP Buffer size at DEC1
	AUD_M14_A0_Wr01(aud_tpa_bufsize, max_size, 0x3A0);	// Init TP Buffer Max size at DEC1
	AUD_M14_A0_Wr01(aud_tpa_bufsize, free_size, 0x380);	// Init TP Buffer Free size at DEC1
	AUD_M14_A0_WrFL(aud_tpa_bufsize);

	//Check a chip revision
	if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		// Write a Main SOC Chip Version
		AUD_M14_A0_Wr(aud_ver_chip, lx_chip_rev());	// Init Main SOC Chip Version
		AUD_M14_A0_WrFL(aud_ver_chip);
	}
	else
	{
		AUD_KDRV_ERROR("<<< %s : LX_CHIP_REV => Unknown(0x%X)\n", __F__, lx_chip_rev());
		AUD_M14_A0_Wr(aud_ver_chip, LX_CHIP_REV(M14, A0));	// Init Main SOC Chip Version
		AUD_M14_A0_WrFL(aud_ver_chip);
	}

#ifdef ENABLE_ADEC_JTAG
	//Check a chip revision
	if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		//Enable Wiggler JTAG for Audio DSP connection
#ifdef ENABLE_ADEC_JTAG_FOR_CPU
		CTOP_CTRL_M14B0_RdFL(LEFT, ctr95);
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag1_sel,	0x0);	//0 : VDEC,  2 : ADEC
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag0_sel,	0x2);	//0 : CPU,   2 : ADEC
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag_aud_sel,	0x1);	//0 : JTAG1, 1 : JTAG0
		CTOP_CTRL_M14B0_WrFL(LEFT, ctr95);
		AUD_KDRV_PRINT("ENABLE_ADEC_JTAG_FOR_CPU : 0x%X\n", CTOP_CTRL_M14B0_Rd(LEFT, ctr95));
#else
		CTOP_CTRL_M14B0_RdFL(LEFT, ctr95);
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag1_sel, 	0x2);		//0 : VDEC, 2 : ADEC
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag0_sel,	0x0);	//0 : CPU,   2 : ADEC
		CTOP_CTRL_M14B0_Wr01(LEFT, ctr95, jtag_aud_sel,	0x0);	//0 : JTAG1, 1 : JTAG0
		CTOP_CTRL_M14B0_WrFL(LEFT, ctr95);
		AUD_KDRV_PRINT("ENABLE_ADEC_JTAG_FOR_PERI : 0x%X\n", CTOP_CTRL_M14B0_Rd(LEFT, ctr95));
#endif
	}
	else
	{
		//Enable Wiggler JTAG for Audio DSP connection
#ifdef ENABLE_ADEC_JTAG_FOR_CPU
		CTOP_CTRL_M14A0_RdFL(ctr32);
		CTOP_CTRL_M14A0_Wr01(ctr32, jtag1_sel,	 0x0);	//VDEC, default
		CTOP_CTRL_M14A0_Wr01(ctr32, jtag0_sel,	 0x2);	//0 : CPU,   2 : ADEC
		CTOP_CTRL_M14A0_Wr01(ctr32, aud_jtag_no, 0x1);	//0 : JTAG1, 1 : JTAG0
		CTOP_CTRL_M14A0_WrFL(ctr32);
		AUD_KDRV_PRINT("ENABLE_ADEC_JTAG_FOR_CPU : 0x%X\n", CTOP_CTRL_M14A0_Rd(ctr32));
#else
		CTOP_CTRL_M14A0_RdFL(ctr32);
		CTOP_CTRL_M14A0_Wr01(ctr32, jtag1_sel, 0x2);	//ADEC
		CTOP_CTRL_M14A0_WrFL(ctr32);
		AUD_KDRV_PRINT("ENABLE_ADEC_JTAG_FOR_PERI : 0x%X\n", CTOP_CTRL_M14A0_Rd(ctr32));
#endif
	}
	AUD_KDRV_PRINT("ADEC DSP Wiggler JTAG Selected Now!!!\n");
#endif	//#ifdef ENABLE_ADEC_JTAG

	//Get a elapsed mili-second time
	elapsed = (UINT32)jiffies_to_msecs(jiffies - start);

	AUD_KDRV_DEBUG_TMP("ADEC_RegInit in %d ms.\n", (UINT32)elapsed);
 	return RET_OK;
}

/**
 * Initialize register for check bit
 * @see AUDIO_InitModule
*/
SINT32 M14_A0_AUDIO_InitRegForCheckbit ( void )
{
	UINT32  i, j, k;
	UINT32	searchStart, searchEnd;

	for(j = 0; j < AUD_BUF_IF_REG_NUM; j++) // i means  wo, ro, wi, ri
	{
		for(k = 1; k < AUD_BUFFER_TYPE_MAX; k++)
		{
			searchStart = _gBufTypeBase[k];
			searchEnd = searchStart + _gBufTypeNum[k];

			for(i = searchStart; i < searchEnd; i++)
			{
				AUD_M14_A0_WrREG(((UINT32 *) _gAuBufMemCfg[i].pRegBase + j), 0xA0000000);
				AUD_KDRV_PRINT("0x%p = [%x]\n",((UINT32 *) _gAuBufMemCfg[i].pRegBase + j), AUD_M14_A0_RdREG(((UINT32 *) _gAuBufMemCfg[i].pRegBase + j)));
			}
		}
	}

	//DDCO(SPDIF)
	AUD_M14_A0_Wr(aud_buf_reverse_wo, 0xA0000000);
	AUD_M14_A0_WrFL(aud_buf_reverse_wo);

	AUD_M14_A0_Wr(aud_buf_reverse_ro, 0xA0000000);
	AUD_M14_A0_WrFL(aud_buf_reverse_ro);

	AUD_M14_A0_Wr(aud_buf_reverse_wi, 0xA0000000);
	AUD_M14_A0_WrFL(aud_buf_reverse_wi);

	AUD_M14_A0_Wr(aud_buf_reverse_ri, 0xA0000000);
	AUD_M14_A0_WrFL(aud_buf_reverse_ri);

	AUD_KDRV_PRINT("ADEC_InitRegForCheckbit\n");
	return RET_OK;
}

/**
 * Clear a DSP1 version registers.
 * @see
*/
SINT32 M14_A0_AUDIO_ClearDSP1VerInfo ( void )
{
	//Clear aud_dsp1_state reg. for codec ver and DSP1 running location
	AUD_M14_A0_RdFL(aud_ver_dsp1);
	AUD_M14_A0_Wr01(aud_ver_dsp1, ext,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1, flag,  0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1, fix,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1, minor, 0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1, major, 0x0);
	AUD_M14_A0_WrFL(aud_ver_dsp1);

	//Clear aud_dsp1_state reg. for codec ver and DSP1 running location
	AUD_M14_A0_RdFL(aud_ver_dsp1_common);
	AUD_M14_A0_Wr01(aud_ver_dsp1_common, ext,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1_common, flag,  0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1_common, fix,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1_common, minor, 0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp1_common, major, 0x0);
	AUD_M14_A0_WrFL(aud_ver_dsp1_common);

	AUD_KDRV_DEBUG_TMP("M14_A0_AUDIO_ClearDSP1VerInfo : Done!!!\n");
	return RET_OK;
}

/**
 * Display a DSP1 FW(FirmWare) Version Info.
 * @see
*/
void M14_A0_AUDIO_DisplayDSP1VerInfo ( UINT64 startTick )
{
	UINT8	ext, flag, fix, minor, major;
	UINT32	elapsed;

	//Get codec version
	AUD_M14_A0_RdFL(aud_ver_dsp1);
	AUD_M14_A0_Rd01(aud_ver_dsp1, ext,   ext);
	AUD_M14_A0_Rd01(aud_ver_dsp1, flag,  flag);
	AUD_M14_A0_Rd01(aud_ver_dsp1, fix,   fix);
	AUD_M14_A0_Rd01(aud_ver_dsp1, minor, minor);
	AUD_M14_A0_Rd01(aud_ver_dsp1, major, major);

	//Get a elapsed mili-second time
	elapsed = (UINT32)jiffies_to_msecs(jiffies - startTick);

	if(g_AudKdrv.IsDSP1Ready)
		AUD_KDRV_ERROR("DSP1 MAIN Ver. - %02x.%02x.%02x.%c.%02x - ready time = %d\n", major, minor, fix, flag, ext, elapsed);
	else
		AUD_KDRV_ERROR("DSP1 to Not Ready : DSP1 Ver. Info - %02x.%02x.%02x.%c.%02x - ready time = %d\n", major, minor, fix, flag, ext, elapsed);

	// Get common layer version
	if(g_AudKdrv.bInitDone == FALSE)
	{
		AUD_M14_A0_RdFL(aud_ver_dsp1_common);
		AUD_M14_A0_Rd01(aud_ver_dsp1_common, ext,   ext);
		AUD_M14_A0_Rd01(aud_ver_dsp1_common, flag,  flag);
		AUD_M14_A0_Rd01(aud_ver_dsp1_common, fix,   fix);
		AUD_M14_A0_Rd01(aud_ver_dsp1_common, minor, minor);
		AUD_M14_A0_Rd01(aud_ver_dsp1_common, major, major);
		AUD_KDRV_ERROR("DSP1  CMN Ver. - %02x.%02x.%02x.%c.%02x\n", major, minor, fix, flag, ext);
	}

	return;
}

/**
 * Clear a DSP0 version registers.
 * @see
*/
SINT32 M14_A0_AUDIO_ClearDSP0VerInfo ( void )
{
	//Clear aud_dsp0_state reg.
	AUD_M14_A0_RdFL(aud_ver_dsp0);
	AUD_M14_A0_Wr01(aud_ver_dsp0, ext,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0, flag,  0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0, fix,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0, minor, 0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0, major, 0x0);
	AUD_M14_A0_WrFL(aud_ver_dsp0);

	// Clear DSP0's Common Layer Version
	AUD_M14_A0_RdFL(aud_ver_dsp0_common);
	AUD_M14_A0_Wr01(aud_ver_dsp0_common, ext,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0_common, flag,  0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0_common, fix,   0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0_common, minor, 0x0);
	AUD_M14_A0_Wr01(aud_ver_dsp0_common, major, 0x0);
	AUD_M14_A0_WrFL(aud_ver_dsp0_common);

	AUD_KDRV_DEBUG_TMP("M14_A0_AUDIO_ClearDSP0VerInfo : Done!!!\n");
	return RET_OK;
}

/**
 * Display a DSP0 FW(FirmWare) Version Info.
 * @see
*/
void M14_A0_AUDIO_DisplayDSP0VerInfo ( UINT64 startTick )
{
	UINT8	ext, flag, fix, minor, major;
	UINT32	elapsed;

	//Get codec type and version
	AUD_M14_A0_RdFL(aud_ver_dsp0);
	AUD_M14_A0_Rd01(aud_ver_dsp0, ext,   ext);
	AUD_M14_A0_Rd01(aud_ver_dsp0, flag,  flag);
	AUD_M14_A0_Rd01(aud_ver_dsp0, fix,   fix);
	AUD_M14_A0_Rd01(aud_ver_dsp0, minor, minor);
	AUD_M14_A0_Rd01(aud_ver_dsp0, major, major);

	//Get a elapsed mili-second time
	elapsed = (UINT32)jiffies_to_msecs(jiffies - startTick);

	if(g_AudKdrv.IsDSP0Ready)
		AUD_KDRV_ERROR("DSP0 MAIN Ver. - %02x.%02x.%02x.%c.%02x - ready time = %d\n", major, minor, fix, flag, ext, elapsed);
	else
		AUD_KDRV_ERROR("DSP0 to Not Ready : DSP0 Ver. Info - %02x.%02x.%02x.%c.%02x - ready time = %d\n", major, minor, fix, flag, ext, elapsed);

	// Get Decoder's Common Layer Version
	if(g_AudKdrv.bInitDone == FALSE)
	{
		AUD_M14_A0_RdFL(aud_ver_dsp0_common);
		AUD_M14_A0_Rd01(aud_ver_dsp0_common, ext,   ext);
		AUD_M14_A0_Rd01(aud_ver_dsp0_common, flag,  flag);
		AUD_M14_A0_Rd01(aud_ver_dsp0_common, fix,   fix);
		AUD_M14_A0_Rd01(aud_ver_dsp0_common, minor, minor);
		AUD_M14_A0_Rd01(aud_ver_dsp0_common, major, major);
		AUD_KDRV_ERROR("DSP0  CMN Ver. - %02x.%02x.%02x.%c.%02x\n", major, minor, fix, flag, ext);
	}

	return;
}

/**
 * Stall a audio DSP module to reset.
 * @see
*/
SINT32 M14_A0_AUDIO_StallDspToReset ( void )
{
	//Stop AUD DSP 1
	AUD_M14_A0_Wr(aud_runstall1, 0x1);
	AUD_M14_A0_WrFL(aud_runstall1);

	//Stop AUD DSP 0
	AUD_M14_A0_Wr(aud_runstall0, 0x1);
	AUD_M14_A0_WrFL(aud_runstall0);

	return RET_OK;
}

/**
 * Read write to registers.
 * This function is used for verification.
 * @see
*/
SINT32 M14_A0_AUDIO_ReadAndWriteReg ( LX_AUD_REG_INFO_T *pstRegInfo )
{
	UINT8 slaveAddr = 0, regAddr = 0, value = 0;

	switch (pstRegInfo->mode)
	{
		case LX_AUD_REG_READ:
		{
			memcpy ( &pstRegInfo->readdata, (UINT32*)(((UINT32)g_pRealRegAud) + pstRegInfo->addr), sizeof (UINT32));
			AUD_KDRV_PRINT("<<< AUD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata);
		}
		break;

		case LX_AUD_REG_WRITE:
		{
			AUD_KDRV_PRINT(">>> AUD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
			memcpy((UINT32*)(((UINT32)g_pRealRegAud) + pstRegInfo->addr), (UINT32*)&pstRegInfo->writedata, sizeof (UINT32));
		}
		break;

		case LX_AUD_REG_WRNRD:
		{
			AUD_KDRV_PRINT(">>> AUD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
			memcpy((UINT32*)(((UINT32)g_pRealRegAud) + pstRegInfo->addr), (UINT32*)&pstRegInfo->writedata, sizeof (UINT32));

			memcpy((UINT32*)&pstRegInfo->readdata, (UINT32*)(((UINT32)g_pRealRegAud) + pstRegInfo->addr), sizeof (UINT32));
			AUD_KDRV_PRINT("<<< AUD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata);
		}
		break;

		case LX_AUD_REG_GET_MAX_ADDR:
		{
			pstRegInfo->readdata = pstAudRegCfg->reg_size - 4;
			AUD_KDRV_PRINT("<<< LX_AUD_REG_GET_MAX_ADDR	addr = 0x%2X\n", pstRegInfo->readdata);
		}
		break;

		case LX_AUD_REG_READ_ARRAY:
		case LX_AUD_REG_READ_ALL:
			return RET_ERROR;
			break;

		case LX_AUD_CTOP_REG_READ:
		{
			//Check a chip revision
			if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
			{
				if( pstRegInfo->addr <= sizeof (M14_B0_CTOP_AUD_TYPE))
				{
					pstRegInfo->readdata = CTOP_CTRL_M14B0_READ(AUD, pstRegInfo->addr);
				}
			}
			else
			{
				if( pstRegInfo->addr <= sizeof (CTOP_CTRL_REG_M14A0_T))
				{
					pstRegInfo->readdata = CTOP_CTRL_M14_READ(pstRegInfo->addr);
				}
			}
			AUD_KDRV_PRINT("<<< CTOP_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata);
		}
		break;

		case LX_AUD_CTOP_REG_WRITE:
		{
			//Check a chip revision
			if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
			{
				if( pstRegInfo->addr <= sizeof (M14_B0_CTOP_AUD_TYPE))
				{
					CTOP_CTRL_M14B0_WRITE(AUD, pstRegInfo->addr, pstRegInfo->writedata);
				}
			}
			else
			{
				if( pstRegInfo->addr <= sizeof (CTOP_CTRL_REG_M14A0_T))
				{
					CTOP_CTRL_M14_WRITE(pstRegInfo->addr, pstRegInfo->writedata);
				}
			}
			AUD_KDRV_PRINT(">>> CTOP_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
		}
		break;

		case LX_AUD_CTOP_REG_WRNRD:
		{
			//Check a chip revision
			if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
			{
				if((pstRegInfo->addr == 0x003C) && (pstRegInfo->writedata == 0x21000))
				{
					g_bEnableDspMonitor = FALSE;
				}

				if((pstRegInfo->addr == 0x003C) && (pstRegInfo->writedata == 0x00200))
				{
					g_bEnableDspMonitor = TRUE;
				}

				AUD_KDRV_PRINT(">>> CTOP_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);

				if(pstRegInfo->addr == 0x003C)
				{
					if( pstRegInfo->addr <= sizeof (M14_B0_CTOP_LEFT_TYPE))
					{
						CTOP_CTRL_M14B0_WRITE(LEFT, pstRegInfo->addr, pstRegInfo->writedata);
						pstRegInfo->readdata = CTOP_CTRL_M14B0_READ(LEFT, pstRegInfo->addr);
					}
				}
				else
				{
					if( pstRegInfo->addr <= sizeof (M14_B0_CTOP_AUD_TYPE))
					{
						CTOP_CTRL_M14B0_WRITE(AUD, pstRegInfo->addr, pstRegInfo->writedata);
						pstRegInfo->readdata = CTOP_CTRL_M14B0_READ(AUD, pstRegInfo->addr);
					}
				}
			}
			else
			{
				if((pstRegInfo->addr == 0x0080) && (pstRegInfo->writedata == 0x21000))
				{
					CTOP_CTRL_M14A0_RdFL(ctr96);
					CTOP_CTRL_M14A0_Wr01(ctr96, jtag0_disable, 0x0);
					CTOP_CTRL_M14A0_WrFL(ctr96);
					AUD_KDRV_ERROR("jtag0_enable : 0x%X\n", CTOP_CTRL_M14A0_Rd(ctr96));

					g_bEnableDspMonitor = FALSE;
				}

				if((pstRegInfo->addr == 0x0080) && (pstRegInfo->writedata == 0x00200))
				{

					CTOP_CTRL_M14A0_RdFL(ctr96);
					CTOP_CTRL_M14A0_Wr01(ctr96, jtag0_disable, 0x1);
					CTOP_CTRL_M14A0_WrFL(ctr96);
					AUD_KDRV_ERROR("jtag0_disable : 0x%X\n", CTOP_CTRL_M14A0_Rd(ctr96));

					g_bEnableDspMonitor = TRUE;
				}

				AUD_KDRV_PRINT(">>> CTOP_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
				if( pstRegInfo->addr <= sizeof (CTOP_CTRL_REG_M14A0_T))
				{
					CTOP_CTRL_M14_WRITE(pstRegInfo->addr, pstRegInfo->writedata);
					pstRegInfo->readdata = CTOP_CTRL_M14_READ(pstRegInfo->addr);
				}
			}
			AUD_KDRV_PRINT("<<< CTOP_REG_READ	addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata);
		}
		break;

		case LX_AUD_CTOP_REG_GET_MAX_ADDR:
		{
			//Check a chip revision
			if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
			{
				pstRegInfo->readdata = sizeof (M14_B0_CTOP_AUD_TYPE) - 4;
			}
			else
			{
				pstRegInfo->readdata = sizeof (CTOP_CTRL_REG_M14A0_T) - 4;
			}

			AUD_KDRV_PRINT("<<< LX_CTOP_REG_GET_MAX_ADDR	addr = 0x%2X\n", pstRegInfo->readdata);
		}
		break;

		case LX_AUD_CTOP_REG_READ_ARRAY:
		case LX_AUD_CTOP_REG_READ_ALL:
			return RET_ERROR;
			break;


		case LX_AUD_ANALOG_REG_READ:
		case LX_AUD_ANALOG_REG_READ_ARRAY:
		{
			slaveAddr = (pstRegInfo->addr >> 8) & 0xFF;
			regAddr   = (pstRegInfo->addr >> 0) & 0xFF;
			ANALOG_ReadRegister(slaveAddr, regAddr, &value);
			pstRegInfo->readdata = (UINT32)(value & 0xFF);

			AUD_KDRV_PRINT("<<< ANALOG_REG_READ  slaveAddr = 0x%2X, regAddr = 0x%2X, data = 0x%2X\n", slaveAddr, regAddr, pstRegInfo->readdata);
		}
		break;

		case LX_AUD_ANALOG_REG_WRITE:
		{
			slaveAddr = (pstRegInfo->addr >> 8) & 0xFF;
			regAddr   = (pstRegInfo->addr >> 0) & 0xFF;
			value     = pstRegInfo->writedata   & 0xFF;
			ANALOG_WriteRegister(slaveAddr, regAddr, value);
			AUD_KDRV_PRINT("<<< ANALOG_REG_WRITE  slaveAddr = 0x%2X, regAddr = 0x%2X, data = 0x%2X\n", slaveAddr, regAddr, pstRegInfo->writedata);
		}
		break;

		case LX_AUD_ANALOG_REG_WRNRD:
		{
			slaveAddr = (pstRegInfo->addr >> 8) & 0xFF;
			regAddr   = (pstRegInfo->addr >> 0) & 0xFF;
			value     = pstRegInfo->writedata   & 0xFF;
			ANALOG_WriteRegister(slaveAddr, regAddr, value);
			AUD_KDRV_PRINT("<<< ANALOG_REG_WRITE  slaveAddr = 0x%2X, regAddr = 0x%2X, data = 0x%2X\n", slaveAddr, regAddr, pstRegInfo->writedata);

			ANALOG_ReadRegister(slaveAddr, regAddr, &value);
			pstRegInfo->readdata = (UINT32)(value & 0xFF);

			AUD_KDRV_PRINT("<<< ANALOG_REG_READ  slaveAddr = 0x%2X, regAddr = 0x%2X, data = 0x%2X\n", slaveAddr, regAddr, pstRegInfo->readdata);
		}
		break;

		default:
			return RET_ERROR;
			break;
	}

	return RET_OK;
}



