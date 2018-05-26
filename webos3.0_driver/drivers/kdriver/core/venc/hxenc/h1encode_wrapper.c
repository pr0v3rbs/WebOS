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
 *  wrapper api implementation for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2013.01.16
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/module.h>

#include "base_types.h"
#include "venc_drv.h"
#include "h1encode_wrapper.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#define HXENC_API(__function, args...) ( pstHxencApi != NULL && pstHxencApi->__function != NULL) ? \
													pstHxencApi->__function(args) : HXENC_API_NOT_SUPPORT
#define HXENC_CHECK_API(__function)		( pstHxencApi != NULL && pstHxencApi->__function != NULL )

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static HXENC_WRAPPER_API_T *pstHxencApi;

/*========================================================================================
	Implementation Group
========================================================================================*/
int HXENC_API_Init( HXENC_WRAPPER_API_T *pstAPI )
{
	int ret = RET_ERROR;

	printk("HXENC_API_Init: START\n");
	
	if ( pstAPI != NULL )
	{
		pstHxencApi = pstAPI;

#if 0
		if ( pstHxencApi->HXENC_Init == NULL )
		{
			VENC_ERROR("%s, HXENC_Init is NULL\n", __F__ );
			return RET_ERROR;
		}
		
		if ( pstHxencApi->HXENC_MemallocInit == NULL )
		{
			VENC_ERROR("%s, HXENC_MemallocInit is NULL\n", __F__ );
			return RET_ERROR;
		}
#endif

		if ( pstHxencApi->HXENC_H264Init == NULL )
		{
			VENC_ERROR("%s, HXENC_H264Init is NULL\n", __F__ );
			return RET_ERROR;
		}

		if ( pstHxencApi->HXENC_VP8Init == NULL )
		{
			VENC_ERROR("%s, HXENC_VP8Init is NULL\n", __F__ );
			return RET_ERROR;
		}
		
		ret = RET_OK;
	}

	printk("HXENC_API_Init: END\n");

	return ret;
}

EXPORT_SYMBOL(HXENC_API_Init);

#ifndef VENC_HXENC_BUILTIN
#if 0
int	HXENC_Init( UINT32  )
{
	if ( HXENC_CHECK_API( HXENC_Init ) )
	{
		return HXENC_API( HXENC_Init, regBase, regSize );
	}
	else
	{
		VENC_ERROR("HXENC_Init is NULL\n");
		return RET_ERROR;
	}
}

void HXENC_Cleanup( void )
{
	HXENC_API( HXENC_Cleanup );
}

int HXENC_MemallocInit( UINT32  startAddress, UINT32  endAddress )
{
	return HXENC_API( HXENC_MemallocInit, startAddress, endAddress );
}

int HXENC_MemallocReadStatus( char * buffer )
{
	return HXENC_API( HXENC_MemallocReadStatus, buffer );
}

#endif

UINT32  HXENC_ReadIRQ( int id )
{
	return HXENC_API( HXENC_ReadIRQ, id );
}

void HXENC_WriteIRQ( int id, UINT32  value )
{
	HXENC_API( HXENC_WriteIRQ, id, value );
}

int HXENC_WakeupIRQ( int id )
{
	return HXENC_API( HXENC_WakeupIRQ, id );
}

int HXENC_ReadVersion( char *buffer )
{
	int len = 0;
	
	if ( pstHxencApi == NULL || buffer == NULL )
	{
		return 0;
	}

	len += sprintf( buffer+len, "HXENC [Module build]\n");
	len += sprintf( buffer+len, "	Version: %d\n", pstHxencApi->version );
	len += sprintf( buffer+len, "	Compile: %s\n", pstHxencApi->compile );

	return len;
}

HXENC_RET_T HXENC_H264Init	(const HXENC_CONFIG_T *pstConfig, HXENC_H264INST_T *instAddr)
{
	return HXENC_API( HXENC_H264Init, pstConfig, instAddr );
}

HXENC_RET_T HXENC_H264Release (HXENC_H264INST_T inst)
{
	return HXENC_API( HXENC_H264Release, inst );
}

HXENC_RET_T HXENC_H264SetRateCtrl (HXENC_H264INST_T inst, const HXENC_RATECTRL_T *pstRateCtrl)
{
	return HXENC_API( HXENC_H264SetRateCtrl, inst, pstRateCtrl );
}

HXENC_RET_T HXENC_H264GetRateCtrl (HXENC_H264INST_T inst, HXENC_RATECTRL_T *pstRateCtrl)
{
	return HXENC_API( HXENC_H264GetRateCtrl, inst, pstRateCtrl );
}

HXENC_RET_T HXENC_H264SetCodingCtrl (HXENC_H264INST_T inst, const HXENC_CODINGCTRL_T *pstCodingCtrl)
{
	return HXENC_API( HXENC_H264SetCodingCtrl, inst, pstCodingCtrl );
}

HXENC_RET_T HXENC_H264SetPreProcessing (HXENC_H264INST_T isnt, const HXENC_PREPROCESSING_CFG_T *pstPreCfg)
{
	return HXENC_API( HXENC_H264SetPreProcessing, isnt, pstPreCfg );
}

HXENC_RET_T HXENC_H264StrmStart (HXENC_H264INST_T isnt, const HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput)
{
	return HXENC_API( HXENC_H264StrmStart, isnt, pstInput, pstOutput );
}

HXENC_RET_T HXENC_H264StrmEncode (HXENC_H264INST_T inst, const HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput)
{
	return HXENC_API( HXENC_H264StrmEncode, inst, pstInput, pstOutput );
}

HXENC_RET_T HXENC_H264StrmEncodeAsync (HXENC_H264INST_T inst, const HXENC_INPUT_T *pstInput)
{
	return HXENC_API( HXENC_H264StrmEncodeAsync, inst, pstInput );
}

HXENC_RET_T HXENC_H264StrmEncodeDone (HXENC_H264INST_T inst, HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput)
{
	return HXENC_API( HXENC_H264StrmEncodeDone, inst, pstInput, pstOutput );
}

HXENC_RET_T HXENC_VP8Init	(const HXENC_CONFIG_T * pstConfig, HXENC_VP8INST_T * instAddr)
{
	return HXENC_API( HXENC_VP8Init, pstConfig, instAddr );
}

HXENC_RET_T HXENC_VP8Release	(HXENC_VP8INST_T inst)
{
	return HXENC_API( HXENC_VP8Release, inst );
}

HXENC_RET_T HXENC_VP8SetRateCtrl	( HXENC_VP8INST_T inst, const HXENC_RATECTRL_T *pstRateCtrl )
{
	return HXENC_API( HXENC_VP8SetRateCtrl, inst, pstRateCtrl );
}

HXENC_RET_T HXENC_VP8SetCodingCtrl	( HXENC_VP8INST_T inst, const HXENC_CODINGCTRL_T *pstCodingCtrl )
{
	return HXENC_API( HXENC_VP8SetCodingCtrl, inst, pstCodingCtrl );
}

HXENC_RET_T HXENC_VP8SetPreProcessing	( HXENC_VP8INST_T inst, const HXENC_PREPROCESSING_CFG_T *pstPreProcCfg )
{
	return HXENC_API( HXENC_VP8SetPreProcessing, inst, pstPreProcCfg );
}

HXENC_RET_T HXENC_VP8StrmEncode	(HXENC_VP8INST_T inst, const HXENC_INPUT_T *pstInput, HXENC_VP8_OUTPUT_T *pstOutput )
{
	return HXENC_API( HXENC_VP8StrmEncode, inst, pstInput, pstOutput );
}

HXENC_RET_T HXENC_VP8StrmEncodeAsync	(HXENC_VP8INST_T inst, const HXENC_INPUT_T *pstInput )
{
	return HXENC_API( HXENC_VP8StrmEncodeAsync, inst, pstInput );
}

HXENC_RET_T HXENC_VP8StrmEncodeDone	(HXENC_VP8INST_T inst, HXENC_INPUT_T *pstInput, HXENC_VP8_OUTPUT_T *pstOutput )
{
	return HXENC_API( HXENC_VP8StrmEncodeDone, inst, pstInput, pstOutput );
}

#endif

/*-----------------------------------------------------------------------------
	Static Function Definition
-----------------------------------------------------------------------------*/

/** @} */


