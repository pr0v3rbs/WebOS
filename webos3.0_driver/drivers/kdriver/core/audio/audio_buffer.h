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
 *  buffer interface header for audio device.
 *	buffer interface.
 *
 *  @author		jongsang.oh (jongsang.oh@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1152_adec
 *	@{
 */

#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "common/adec_processor_adap.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	AUDIO_CPB_MAX		2
#define	AUDIO_DPB_MAX		2
#define	AUDIO_MIX_MAX		8
#define	AUDIO_CAP_MAX		3
#define	AUDIO_IEC_MAX		2

// define audio buffer number
#define AUD_BUF_IF_REG_NUM				4

// define CPB / MPB / ENC / CAP buffer enumeration
#define CPB_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)
#define DPB_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)
#define MPB_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)
#define ENC_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)
#define IEC_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)
#define CAPB_CPU_TO_DSP_BASE_DIFF		(pstMemCfgAud->dpb0_memory_base - 0x68000000)

#define CPB_MAX_ACCESS_SIZE         	(20*1024)
#define DPB_MAX_ACCESS_SIZE         	(20*1024)
#define MPB_MAX_ACCESS_SIZE         	(10*1024)
#define IEC_MAX_ACCESS_SIZE         	(10*1024)
#define CAPB_MAX_ACCESS_SIZE         	(20*1024)

#define CPB_AUI_INDEX_COUNT       		(0x0400)	// 1024
#define DPB_AUI_INDEX_COUNT       		(0x0400)	// 1024
#define MPB_AUI_INDEX_COUNT       		(0x0400)	// 1024
#define IEC_AUI_INDEX_COUNT       		(0x0400)	// 1024
#define CAPB_AUI_INDEX_COUNT      		(0x0400)	// 1024


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	PFN_ADEC_CACHE_CONTROL		pfnInvalidate;
	PFN_ADEC_CACHE_CONTROL		pfnWriteback;
	PFN_ADEC_ADDR_TRANSLATOR	pfnAddrTrans;
	UINT8*						dummyAddr;
	SINT32						memSize;
	UINT8*						memBase;
}ARM_PROCESSOR_ADAP;

/**
 * AUDIO Buffer Index
 */
typedef enum {
	AUD_BUFFER_NONE,
	AUD_BUFFER_CPB_0,
	AUD_BUFFER_CPB_1,
	AUD_BUFFER_DPB_0,
	AUD_BUFFER_DPB_1,
	AUD_BUFFER_MPB_0,
	AUD_BUFFER_MPB_1,
	AUD_BUFFER_MPB_2,
	AUD_BUFFER_MPB_3,
	AUD_BUFFER_MPB_4,
	AUD_BUFFER_MPB_5,
	AUD_BUFFER_MPB_6,
	AUD_BUFFER_MPB_7,
	AUD_BUFFER_IEC_0,
	AUD_BUFFER_IEC_1,
	AUD_BUFFER_CAPB_0,	// capture buffer
	AUD_BUFFER_CAPB_1,	// capture buffer
	AUD_BUFFER_CAPB_2,	// capture buffer
	AUD_BUFFER_CAPB_3,	// capture buffer
	AUD_BUFFER_CAPB_4,	// capture buffer
	AUD_BUFFER_CAPB_5,	// capture buffer
	AUD_BUFFER_CAPB_6,	// capture buffer
	AUD_BUFFER_CAPB_7,	// capture buffer
	AUD_BUFFER_ENC_0,
	AUD_BUFFER_MAX,
} AUD_BUFFER_T;

/**
 * AUDIO Buffer Type
 */
typedef enum {
	AUD_BUFFER_TYPE_NONE,	// None buffer
	AUD_BUFFER_TYPE_CPB,		// Coded PCM buffer
	AUD_BUFFER_TYPE_DPB,		// Decoded PCM buffer
	AUD_BUFFER_TYPE_MPB,		// Mixed PCM buffer
	AUD_BUFFER_TYPE_IEC,		// IEC buffer
	AUD_BUFFER_TYPE_ENC,		// encode buffer
	AUD_BUFFER_TYPE_CAPB,	// capture buffer
	AUD_BUFFER_TYPE_MAX,
} AUD_BUFFER_TYPE_T;

/**
AUDIO MIX Memory Map configuration.
*/
typedef struct
{
	void		*pWriterReaderStruct;
	UINT32		*pMemVirBase;
	void		*pRegBase;
	char		*MemName;
	UINT32		MemPhyBase;
	UINT32		MemSize;
	AUD_BUFFER_TYPE_T	bufType;
	BOOLEAN		bUsed;
} LX_AUD_BUF_MEM_CFG_S_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void ADEC_ARM_Create_Interbuf_Writer(void);
void ADEC_ARM_Create_Interbuf_Reader(void);

SINT32 AUDIO_IORemapBuffer(void);
SINT32 AUDIO_CreateBuffer(void);
#if 0 // audio_drv_hal_xx.c
SINT32 AUDIO_InitRegForCheckbit(void);
#endif

LX_AUD_BUF_MEM_CFG_S_T* AUDIO_AllocBufferCfg(AUD_BUFFER_TYPE_T bufType);
void AUDIO_FreeBufferCfg(LX_AUD_BUF_MEM_CFG_S_T*pBufCfg);
void* AUDIO_AllocBuffer(UINT32 mod, AUD_BUFFER_TYPE_T bufType);
void AUDIO_FreeBuffer(UINT32 mod);
void* AUDIO_GetBuffer(UINT32 mod, UINT32 inOut);

void AUDIO_ShowIoremap( void );
void AUDIO_ShowMemoryDump( LX_AUD_DEBUG_MEM_DUMP_T *pMemDump );

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif


#endif /* _AUDIO_BUFFER_H_ */
