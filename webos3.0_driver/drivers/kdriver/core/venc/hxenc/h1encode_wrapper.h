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

#ifndef _H1ENCOODE_WRAPPER_H_
#define _H1ENCOODE_WRAPPER_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include "hxenc_api.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef struct {
#if 0
	int (*HXENC_Init)	( u32  regBase, u32  regSize );
	void (*HXENC_Cleanup) ( void );

	int (*HXENC_MemallocInit)	( u32  start, u32  end );
	int (*HXENC_MemallocReadStatus) ( char * buffer );
#endif

	u32 (*HXENC_ReadIRQ)		( int id );
	void (*HXENC_WriteIRQ)		( int id, u32  value );
	int	(*HXENC_WakeupIRQ)		( int id );

	HXENC_RET_T (*HXENC_H264Init)	(const HXENC_CONFIG_T *pstConfig, HXENC_H264INST_T *instAddr);
	HXENC_RET_T (*HXENC_H264Release) (HXENC_H264INST_T inst);
	HXENC_RET_T (*HXENC_H264SetRateCtrl) (HXENC_H264INST_T inst, const HXENC_RATECTRL_T *pstRateCtrl);
	HXENC_RET_T (*HXENC_H264GetRateCtrl) (HXENC_H264INST_T inst, HXENC_RATECTRL_T *pstRateCtrl);
	HXENC_RET_T (*HXENC_H264SetCodingCtrl) (HXENC_H264INST_T inst, const HXENC_CODINGCTRL_T *pstCodingCtrl);
	HXENC_RET_T (*HXENC_H264SetPreProcessing) (HXENC_H264INST_T isnt, const HXENC_PREPROCESSING_CFG_T *pstPreCfg);
	HXENC_RET_T (*HXENC_H264StrmStart) (HXENC_H264INST_T isnt, const HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput);
	HXENC_RET_T (*HXENC_H264StrmEncode) (HXENC_H264INST_T inst, const HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput);
	HXENC_RET_T (*HXENC_H264StrmEncodeAsync) (HXENC_H264INST_T inst, const HXENC_INPUT_T *pstInput);
	HXENC_RET_T (*HXENC_H264StrmEncodeDone) (HXENC_H264INST_T inst, HXENC_INPUT_T *pstInput, HXENC_H264_OUTPUT_T *pstOutput);
	
	HXENC_RET_T (*HXENC_VP8Init)		( const HXENC_CONFIG_T *pstConfig, HXENC_VP8INST_T *pInst );	
	HXENC_RET_T (*HXENC_VP8Release) 	(HXENC_VP8INST_T inst);
	HXENC_RET_T (*HXENC_VP8SetRateCtrl)	( HXENC_VP8INST_T inst, const HXENC_RATECTRL_T *pstRateCtrl );
	HXENC_RET_T (*HXENC_VP8SetCodingCtrl)( HXENC_VP8INST_T inst, const HXENC_CODINGCTRL_T *pstCodingCtrl );
	HXENC_RET_T (*HXENC_VP8SetPreProcessing)( HXENC_VP8INST_T inst, const HXENC_PREPROCESSING_CFG_T *pstPreProcCfg );
	HXENC_RET_T (*HXENC_VP8StrmEncode)	( HXENC_VP8INST_T inst, const HXENC_INPUT_T *pstInput, HXENC_VP8_OUTPUT_T *pstOutput );
	HXENC_RET_T (*HXENC_VP8StrmEncodeAsync)	( HXENC_VP8INST_T inst, const HXENC_INPUT_T *pstInput );
	HXENC_RET_T (*HXENC_VP8StrmEncodeDone)	( HXENC_VP8INST_T inst, HXENC_INPUT_T *pstInput, HXENC_VP8_OUTPUT_T *pstOutput );

	u32	version;
	char compile[20];
} HXENC_WRAPPER_API_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/
int HXENC_API_Init( HXENC_WRAPPER_API_T *pstAPI );

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

/*========================================================================================
	Implementation Group
========================================================================================*/

/*-----------------------------------------------------------------------------
	Static Function Definition
-----------------------------------------------------------------------------*/

#endif // _H1ENCOODE_WRAPPER_H_

/** @} */

