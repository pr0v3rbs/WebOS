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
 * audio buffer functions.
 * Manage audio buffer.
 *
 * author     wonchang.shin (wonchang.shin@lge.com)
 * version    1.0
 * date       2011.05.01
 *
 * @addtogroup lg1152_adec
 * @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "audio_drv.h"
#include "audio_buffer.h"
#include "audio_drv_manager.h"

#include "buffer/adec_inter_buf_writer.h"
#include "buffer/adec_inter_buf_reader.h"

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
extern UINT32	*g_pui32DSP0CodecVirAddr;
extern UINT32	*g_pui32DSP1CodecVirAddr;

extern UINT32	*g_pui32IPC_VirAddr;
extern UINT32	*g_pui32SEParamVirAddr;

extern UINT32	_gModTypeNum[ADEC_MOD_TYPE_MAX];
extern UINT32	_gModTypeBase[ADEC_MOD_TYPE_MAX];

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
LX_AUD_BUF_MEM_CFG_S_T	_gAuBufMemCfg[AUD_BUFFER_MAX] =
{
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_NONE,
	},
	// CPB 2
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CPB,
	},
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CPB,
	},
	// DPB 2
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_DPB,
	},
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_DPB,
	},
	// MPB 8
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	{
		.pWriterReaderStruct 		= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_MPB,
	},
	// IEC 2
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_IEC,
	},
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_IEC,
	},
	// CAP 8
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	{
		.pWriterReaderStruct = NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_CAPB,
	},
	// ENC 1
	{
		.pWriterReaderStruct= NULL,
		.pMemVirBase		= NULL,
		.MemPhyBase			= 0,
		.MemSize			= 0,
		.bufType			= AUD_BUFFER_TYPE_ENC,
	},
};

UINT32	_gBufTypeNum[AUD_BUFFER_TYPE_MAX];
UINT32	_gBufTypeBase[AUD_BUFFER_TYPE_MAX];

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void ARM_DCache_Invalidate(void* _addr, unsigned int _length, void* _adap);
static void ARM_DCache_Writeback(void* _addr, unsigned int _length, void* _adap);
static unsigned char* ARM_Mem_Translator(unsigned char* _addr, void* _adap);

static SINT32  AUDIO_Create_Interbuf_Writer_N_Reader(
		void **ppWriterStruct, ARM_PROCESSOR_ADAP * bufAdapIf, UINT32 *pMemVirBase,
		UINT32 MemPhyBase, UINT32 MemSize, void *pRegBase, AUD_BUFFER_TYPE_T type );


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
ARM_PROCESSOR_ADAP _gBuf_ArmWriterAdap[AUD_BUFFER_MAX];

 /*========================================================================================
	Implementation Group
========================================================================================*/
static void ARM_DCache_Invalidate(void* _addr, unsigned int _length, void* _adap)
{
	return;
}

static void ARM_DCache_Writeback(void* _addr, unsigned int _length, void* _adap)
{
	return;
}

static unsigned char* ARM_Mem_Translator(unsigned char* _addr, void* _adap)
{
	ARM_PROCESSOR_ADAP* adap;
	int offset;

	adap = (ARM_PROCESSOR_ADAP*) _adap;

	if( adap->memBase > _addr || adap->memBase + adap->memSize < _addr )
	{
	    AUD_ASSERT(0,
       	 (unsigned char*)NULL,
        	"exceed size(addr = 0x%X).\n", (unsigned int)_addr);
		return _addr;
	}

	offset = (unsigned char*)_addr - adap->memBase;

	return adap->dummyAddr + offset;/**/
}

/**
 * ioremap audio buffer memory
 * @see
*/
SINT32 AUDIO_IORemapBuffer( void )
{
	SINT32	ret = RET_OK;
	SINT32	i;

	// initailize module info
	memset(&_gBufTypeNum[0], 0, sizeof(UINT32)*AUD_BUFFER_TYPE_MAX);

	for(i = 1; i < AUD_BUFFER_MAX ; i++)
	{
		if(_gAuBufMemCfg[i].bufType != _gAuBufMemCfg[i-1].bufType)
		{
			_gBufTypeBase[_gAuBufMemCfg[i].bufType] = i;
			AUD_KDRV_PRINT("bufType(%d) bufTypeBase(%d).\n", _gAuBufMemCfg[i].bufType, _gBufTypeBase[_gAuBufMemCfg[i].bufType]);
		}

		_gBufTypeNum[_gAuBufMemCfg[i].bufType]++;

		_gBuf_ArmWriterAdap[i].pfnInvalidate = ARM_DCache_Invalidate;
		_gBuf_ArmWriterAdap[i].pfnWriteback = ARM_DCache_Writeback;
		_gBuf_ArmWriterAdap[i].pfnAddrTrans = ARM_Mem_Translator;
	}

	return ret;
}

/**
 * create audio buffer
 * @see
*/
SINT32 AUDIO_CreateBuffer( void )
{
	SINT32 ret = RET_OK;
	SINT32 i;

	for(i = 1; i < AUD_BUFFER_MAX; i++)
	{
		ret = AUDIO_Create_Interbuf_Writer_N_Reader(	\
			  (void *)&_gAuBufMemCfg[i].pWriterReaderStruct, (void *)&_gBuf_ArmWriterAdap[i], (UINT32 *)_gAuBufMemCfg[i].pMemVirBase,	\
			   _gAuBufMemCfg[i].MemPhyBase, _gAuBufMemCfg[i].MemSize, (void *)_gAuBufMemCfg[i].pRegBase, _gAuBufMemCfg[i].bufType );
		if(ret == WRITER_RET_OPEN_OK)
		{
			AUD_KDRV_PRINT("AUD_Create_Interbuf_Writer(%s) is Success!!!\n", _gAuBufMemCfg[i].MemName);
		}
		else
		{
			AUD_KDRV_ERROR("AUD_Create_Interbuf_Writer(%s) is Failed!!!\n", _gAuBufMemCfg[i].MemName);
		}
	}

	return RET_OK;
}

/**
AUDIO create interbuf writer and reader
*/
static SINT32  AUDIO_Create_Interbuf_Writer_N_Reader(
		void **ppWriterReaderStruct, ARM_PROCESSOR_ADAP * bufAdapIf, UINT32 *pMemVirBase,
		UINT32 MemPhyBase, UINT32 MemSize, void *pRegBase, AUD_BUFFER_TYPE_T  type)
{
	SINT32 ret = WRITER_RET_OPEN_OK;
	UINT32 cpuToDspBasDiff = 0,auiIndexCount = 0, MaxAccessSize = 0 ;
	UINT32 structSize;

	cpuToDspBasDiff 	= CAPB_CPU_TO_DSP_BASE_DIFF;
	switch( type	)
	{
		case AUD_BUFFER_TYPE_CPB:
			auiIndexCount 	= CPB_AUI_INDEX_COUNT;
			MaxAccessSize	= CPB_MAX_ACCESS_SIZE;
			break;
		case AUD_BUFFER_TYPE_DPB:
			auiIndexCount 	= DPB_AUI_INDEX_COUNT;
			MaxAccessSize	= DPB_MAX_ACCESS_SIZE;
			break;
		case AUD_BUFFER_TYPE_MPB:
			auiIndexCount 	= MPB_AUI_INDEX_COUNT;
			MaxAccessSize	= MPB_MAX_ACCESS_SIZE;
			break;
		case AUD_BUFFER_TYPE_IEC:
			auiIndexCount 	= IEC_AUI_INDEX_COUNT;
			MaxAccessSize	= IEC_MAX_ACCESS_SIZE;
			break;
		case AUD_BUFFER_TYPE_CAPB:
			auiIndexCount 	= CAPB_AUI_INDEX_COUNT;
			MaxAccessSize	= CAPB_MAX_ACCESS_SIZE;
			break;
		case AUD_BUFFER_TYPE_ENC:
			break;
		default:
			break;
	}

	//writer
	if(type != AUD_BUFFER_TYPE_ENC && type != AUD_BUFFER_TYPE_CAPB)
	{
		if(*ppWriterReaderStruct == NULL)
		{
			*ppWriterReaderStruct = OS_KMalloc(InterBufWriter_GetStructSize());
		}
	}
	//reader
	else
	{
		if(*ppWriterReaderStruct == NULL)
		{
			*ppWriterReaderStruct = OS_KMalloc(InterBufReader_GetStructSize());
		}
	}

	if(type  != AUD_BUFFER_TYPE_ENC)
	{
		structSize = CommonBuffer_GetStructSize(auiIndexCount);

		//Set a memory address for CPU and DSP
		bufAdapIf->dummyAddr = (unsigned char *)pMemVirBase;
		bufAdapIf->memBase   = (unsigned char *)(MemPhyBase - cpuToDspBasDiff);
		bufAdapIf->memSize   = MemSize;

		//Create buffer for mixer
		ret = CommonBuffer_Create(
		(unsigned char *)((MemPhyBase - cpuToDspBasDiff) + structSize),
		MemSize - structSize,
		MaxAccessSize,
		auiIndexCount,
		(ADEC_PROCESSOR_ADAP_IF_T *)bufAdapIf,
		pMemVirBase,
		NULL);
	}


	switch(type)
	{
		//writer
		case AUD_BUFFER_TYPE_CPB:
		case AUD_BUFFER_TYPE_DPB:
		case AUD_BUFFER_TYPE_MPB:
		case AUD_BUFFER_TYPE_IEC:
			if(ret != WRITER_RET_OPEN_OK)
			{
				AUD_KDRV_ERROR("ADEC Mix0 CommonBuffer_Create Error(ret = %d)!!!\n", ret);
			}
			else
			{
				ret = InterBufWriter_CreateFromCommonBuffer(
							(ADEC_PROCESSOR_ADAP_IF_T *)(bufAdapIf),
							(void *)pRegBase,
							pMemVirBase,
							*ppWriterReaderStruct);
			}
			break;
		//reader
		case AUD_BUFFER_TYPE_CAPB:
			if(ret != WRITER_RET_OPEN_OK)
			{
				AUD_KDRV_ERROR("ADEC CommonBuffer_Create Error(ret = %d)!!!\n", ret);
			}

			ret = InterBufReader_CreateFromCommonBuffer(
						(ADEC_PROCESSOR_ADAP_IF_T *)(bufAdapIf),
						(void *)pRegBase,
						pMemVirBase,
						*ppWriterReaderStruct);
			break;
		//reader
		case AUD_BUFFER_TYPE_ENC:
			ret = InterBufReader_CreateFromCommonBuffer(
						(ADEC_PROCESSOR_ADAP_IF_T *)(bufAdapIf),
						(void *)pRegBase,
						pMemVirBase,
						*ppWriterReaderStruct);
			break;
		default:
			break;
	}

    return ret;
}

void* AUDIO_GetBuffer(UINT32 mod, UINT32 inOut)
{
	AUD_MOD_INFO_T			*pModInfo = NULL;
	LX_AUD_BUF_MEM_CFG_S_T	*pBufMemCfg = NULL;
	ADEC_BUF_T				*pBuf = NULL;

	if(mod <= ADEC_MODULE_NOT_DEF  || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_DEBUG("%s.%d] mod[%d] is invalid.\n", __F__, __L__, mod);
		return NULL;
	}

	pModInfo = AUDIO_GetModuleInfo(mod);
	if(pModInfo == NULL)
	{
		AUD_KDRV_DEBUG("%s.%d] pModInfo(%d) is NULL!!!\n", __F__, __L__, mod);
		return NULL;
	}

	if(inOut == IN_PORT)
		pBufMemCfg = pModInfo->inPort[0].pBufMemCfg;
	else
		pBufMemCfg = pModInfo->outPort[0].pBufMemCfg;

	if(pBufMemCfg == NULL || pBufMemCfg->pWriterReaderStruct == NULL)
	{
		AUD_KDRV_DEBUG("%s.%d] pBufMemCfg of %s is NULL!!!\n", __F__, __L__, \
			ModuleList_GetModuleName(mod));
		return NULL;
	}

	pBuf = pBufMemCfg->pWriterReaderStruct;

	return (void*)pBuf;
}

void* AUDIO_AllocBuffer(UINT32 mod, AUD_BUFFER_TYPE_T bufType)
{
	AUD_MOD_INFO_T			*pModInfo = NULL;
	LX_AUD_BUF_MEM_CFG_S_T	*pBufMemCfg = NULL;
	ADEC_BUF_T				*pBuf = NULL;

	UINT32					searchStart, searchEnd;

	searchStart = _gModTypeBase[ADEC_MOD_TYPE_EXT_BUF];
	searchEnd = searchStart + _gModTypeNum[ADEC_MOD_TYPE_EXT_BUF];

	if(mod < searchStart  || mod >= searchEnd)
	{
		AUD_KDRV_DEBUG("%s.%d] mod[%d] is invalid.\n", __F__, __L__, mod);
		return NULL;
	}

	pModInfo = AUDIO_GetModuleInfo(mod);
	if(pModInfo == NULL)
	{
		AUD_KDRV_DEBUG("%s.%d] pModInfo(%d) is NULL!!!\n", __F__, __L__, mod);
		return NULL;
	}

	if(bufType == AUD_BUFFER_TYPE_CAPB)
		pBufMemCfg = pModInfo->inPort[0].pBufMemCfg;
	else
		pBufMemCfg = pModInfo->outPort[0].pBufMemCfg;

	if(pBufMemCfg != NULL && pBufMemCfg->pWriterReaderStruct != NULL)
	{
	 	pBuf = pBufMemCfg->pWriterReaderStruct;
		return (void*)pBuf;
	}

	pBufMemCfg = AUDIO_AllocBufferCfg(bufType);
	if(pBufMemCfg == NULL)
	{
		AUD_KDRV_DEBUG("%s.%d] pBufMemCfg(%d) is NULL!!!\n", __F__, __L__, bufType);
		return NULL;
	}

	pBuf = pBufMemCfg->pWriterReaderStruct;

	if(bufType == AUD_BUFFER_TYPE_CAPB)
		pModInfo->inPort[0].pBufMemCfg = (void*)pBufMemCfg;
	else
		pModInfo->outPort[0].pBufMemCfg = (void*)pBufMemCfg;

	AUD_KDRV_RM("%s.%d] pBuf[%p] is created.\n", __F__, __L__, pBuf);

	return (void*)pBuf;
}

void AUDIO_FreeBuffer(UINT32 mod)
{
	UINT32					searchStart, searchEnd;

	AUD_MOD_INFO_T			*pModInfo = NULL;
	LX_AUD_BUF_MEM_CFG_S_T	*pBufMemCfg = NULL;
	ADEC_BUF_T				*pBuf = NULL;

	searchStart = _gModTypeBase[ADEC_MOD_TYPE_EXT_BUF];
	searchEnd = searchStart + _gModTypeNum[ADEC_MOD_TYPE_EXT_BUF];

	if(mod < searchStart  || mod >= searchEnd)
	{
		AUD_KDRV_ERROR("%s.%d] mod[%d] is invalid.\n", __F__, __L__, mod);
		return;
	}

	pModInfo = AUDIO_GetModuleInfo(mod);

	if(pModInfo->inPort[0].pBufMemCfg != NULL)
		pBufMemCfg = pModInfo->inPort[0].pBufMemCfg;
	else if(pModInfo->outPort[0].pBufMemCfg != NULL)
		pBufMemCfg = pModInfo->outPort[0].pBufMemCfg;
	else
	{
		AUD_KDRV_RM("%s.%d] pBufMemCfg[%d] is already freed.\n", __F__, __L__, mod);
		return;
	}

	AUDIO_FreeBufferCfg(pBufMemCfg);

	// Initialize Buffer Memory Configuration
	pModInfo->inPort[0].pBufMemCfg = NULL;
	pModInfo->outPort[0].pBufMemCfg = NULL;

	AUD_KDRV_RM("%s.%d] pBuf[%p] is freed.\n", __F__, __L__, pBuf);

	return;
}

LX_AUD_BUF_MEM_CFG_S_T* AUDIO_AllocBufferCfg(AUD_BUFFER_TYPE_T bufType)
{
	SINT32					i = 0;

	LX_AUD_BUF_MEM_CFG_S_T	*pBufMemCfg = NULL;

	for(i = 1; i < AUD_BUFFER_MAX; i++)
	{
		pBufMemCfg = &_gAuBufMemCfg[i];
		if(pBufMemCfg->bufType == bufType && pBufMemCfg->bUsed == 0)
		{
			break;
		}
	}

	pBufMemCfg->bUsed = 1;

	return pBufMemCfg;
}

void AUDIO_FreeBufferCfg(LX_AUD_BUF_MEM_CFG_S_T*pBufMemCfg)
{
	if(pBufMemCfg == NULL)
	{
		AUD_KDRV_ERROR("%s.%d] pBufMemCfg is already freed.!!!\n", __F__, __L__);
		return;
	}

	pBufMemCfg->bUsed = 0;

	return;
}


/**
 * show ioremap audio memory for debug
 * @see
*/
void AUDIO_ShowIoremap( void )
{
	SINT32 i;

	AUD_KDRV_PRINTF ( "\n");
	AUD_KDRV_PRINTF ( "DSP0        [0x%08X:0x%08X]\n", (UINT32) g_pui32DSP0CodecVirAddr, (UINT32)pstMemCfgAud->dsp0_memory_base + DSP0_VIR_ADDR_OFFSET);
	AUD_KDRV_PRINTF ( "DSP1        [0x%08X:0x%08X]\n", (UINT32) g_pui32DSP1CodecVirAddr,(UINT32)pstMemCfgAud->dsp1_memory_base + DSP1_VIR_ADDR_OFFSET);
	for(i = 1; i < AUD_BUFFER_MAX; i++)
	{
		AUD_KDRV_PRINTF("%s    [0x%08X:0x%08X:0x%p]\n", _gAuBufMemCfg[i].MemName,(UINT32)_gAuBufMemCfg[i].pMemVirBase, (UINT32)_gAuBufMemCfg[i].MemPhyBase, _gAuBufMemCfg[i].pRegBase);
	}
	AUD_KDRV_PRINTF("SE          [0x%08X:0x%08X]\n", (UINT32) g_pui32SEParamVirAddr, (UINT32)pstMemCfgAud->se_memory_base);
	AUD_KDRV_PRINTF("IPC         [0x%08X:0x%08X]\n", (UINT32) g_pui32IPC_VirAddr, (UINT32)pstMemCfgAud->ipc_memory_base);
	AUD_KDRV_PRINTF("\n");
}

/**
 * show audio memory for debug
 * @see
*/
void AUDIO_ShowMemoryDump( LX_AUD_DEBUG_MEM_DUMP_T *pMemDump )
{
	UINT32 i;
	UINT32 *memAddr = NULL;
	UINT32 address;
	UINT32 line;
	UINT32 type;
	char* fileName;

	address = pMemDump->address;
	line = pMemDump->line;
	type = pMemDump->type;

	if(address < 0x10000000)
	{
		AUD_KDRV_ERROR("address is not valid.\n");
		return;
	}

	memAddr = (UINT32 *)address;

	if(line >= 0x800000)
	{
		AUD_KDRV_ERROR("address is not valid.\n");
		return;
	}

	if(line == 0)
		line = 1;

	if(type == 0)
	{
		AUD_KDRV_PRINTF ( "\n");
		AUD_KDRV_PRINTF ( "         :    00       04       08       0C       10       14       18       1C\n");
		for(i = 0 ; i < line; i++)
		{
			AUD_KDRV_PRINTF ( "%08X : %08X %08X %08X %08X %08X %08X %08X %08X\n", (UINT32)(memAddr + 0x8*i),
				*(memAddr + 0x8*i + 0), *(memAddr + 0x8*i + 1), *(memAddr + 0x8*i + 2), *(memAddr + 0x8*i + 3),
				*(memAddr + 0x8*i + 4), *(memAddr + 0x8*i + 5), *(memAddr + 0x8*i + 6), *(memAddr + 0x8*i + 7));
		}
		AUD_KDRV_PRINTF("\n");
	}
	else
	{
		struct file *filp;
		mm_segment_t old_fs = get_fs();

		fileName = &(pMemDump->fileName[0]);

		if(fileName == NULL)
		{
			AUD_KDRV_ERROR("fileName is not valid.\n");
			return;
		}

		set_fs(KERNEL_DS);

		filp = filp_open(fileName, O_RDWR|O_CREAT, 0644);
		if(IS_ERR(filp))
		{
			AUD_KDRV_ERROR("open error(%s)\n", fileName);
			return;
		}
		else
		{
			AUD_KDRV_ERROR("open success(%s)\n", fileName);
		}

		vfs_write(filp, (const char *)memAddr, line*0x20, &filp->f_pos);

		filp_close(filp, NULL);

		set_fs(old_fs);
	}

	return;
}

