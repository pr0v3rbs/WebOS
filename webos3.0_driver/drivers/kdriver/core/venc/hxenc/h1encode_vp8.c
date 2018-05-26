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
 *  register api implementation for venc device of H13.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2012.06.12
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
#include <linux/kthread.h>
#include <asm/io.h>			// for ioremap()/iounmap()
//#include <linux/wait.h> 	// for interruptible_sleep_on_timeout()
#include <linux/slab.h>
#include <linux/delay.h>	// for msleep()

#include "../venc_cfg.h"
#include "../venc_drv.h"

#include "venc_h13b0_regprep.h"

#include "h1encode.h"
#include "h1encode_wrapper.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/
H1ENCODE_API_T *H1EncodeAPISetDefault( H1ENCODE_API_T *pstEncoder );
long long _NowMS( void );

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static int _VP8Open( H1ENCODE_API_T *pstEncoder );
static int _VP8Close( H1ENCODE_API_T *pstEncoder );
static int _VP8Encode( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
static int _VP8SetBitrate( H1ENCODE_API_T *pstEncoder, UINT32 bitrate, BOOLEAN bEnableCBR );
static int _VP8EncodeAsync( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
static int _VP8EncodeDone( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
#if 0
static void _VP8IvfHeader(H1ENCODE_VP8_T *pVP8, UINT32 width, UINT32 height, UINT32 outputRateNumer, UINT32 outputRateDenom, UINT32 frameCntTotal);
static void _VP8IvfFrame(H1ENCODE_VP8_T *pVP8, UINT32 frameSize, UINT64 frameCntTotal );
#else
static void _VP8IvfHeader( UINT8 *data, UINT32 width, UINT32 height, UINT32 outputRateNumer, UINT32 outputRateDenom, UINT32 frameCntTotal);
static void _VP8IvfFrame( UINT8 *data, UINT32 frameSize, UINT64 frameCntTotal );
#endif

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static UINT8 dataIvfHeader[IVF_HDR_BYTES] = {0};
static UINT8 dataIvfFrames[8][IVF_FRM_BYTES];
static u64 monitorTime[4] = {};

/*========================================================================================
	Implementation Group
========================================================================================*/

H1ENCODE_API_T *VP8Alloc( void )
{
	H1ENCODE_API_T *pstEncoder = (H1ENCODE_API_T *)kcalloc( 1, sizeof(H1ENCODE_API_T), GFP_KERNEL );

	TRACE_ENTER();

	if ( pstEncoder != NULL )
	{
		memset(pstEncoder, 0x0, sizeof(H1ENCODE_API_T));
		H1EncodeAPISetDefault( pstEncoder );

		pstEncoder->eEncodeType = LX_VENC_ENCODE_TYPE_VP8;

		// allocate encoding functions
		pstEncoder->Open 	= _VP8Open;
		pstEncoder->Close 	= _VP8Close;
		pstEncoder->Encode 	= _VP8Encode;
		pstEncoder->EncodeAsync = _VP8EncodeAsync;
		pstEncoder->EncodeDone 	= _VP8EncodeDone;

		// override default functions
		pstEncoder->SetBitrate = _VP8SetBitrate;
	}

	TRACE_EXIT();

	return pstEncoder;
}

void VP8Release(H1ENCODE_API_T * pstEncoder)
{

	TRACE_ENTER();

	if ( pstEncoder != NULL )
	{
		kfree(pstEncoder);
	}

	TRACE_EXIT();
}


static int _VP8Open( H1ENCODE_API_T *pstEncoder )
{
	HXENC_RET_T ret;
	HXENC_CONFIG_T	config = {0};
	HXENC_RATECTRL_T rcCfg = {0};
	HXENC_CODINGCTRL_T codingCfg = {0};
	HXENC_PREPROCESSING_CFG_T preCfg = {0};

	//int inputWidth, inputHeight;
	int minBufferLength = H1ENCODE_OUTBUF_MIN;

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder->bIsFirstFrame = TRUE;
	pstEncoder->bIncludeIVFHeader = TRUE;

	if ( pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

#if 0
	if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
	{
		VENC_ERROR("VP8 is not support Interlaced input\n");
		return RET_ERROR;
	}
#endif

	config.frameRateDenom = pstEncoder->frameRateDenom;
	config.frameRateNum = pstEncoder->frameRateNum;		// this is the time resolution also
	config.width = pstEncoder->width;
	config.height = pstEncoder->height;

	if((ret = HXENC_VP8Init(&config, (HXENC_VP8INST_T *)&pstEncoder->pEncInst)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8Init - failed.(%d)\n", ret);
		return (int)ret;
	}

	rcCfg.bEnableTargetBitrate = TRUE;
	if ( pstEncoder->bitrate >= H1ENCODE_BITRATE_MIN && pstEncoder->bitrate <= H1ENCODE_BITRATE_MAX )
	{
		rcCfg.bitPerSecond = pstEncoder->bitrate;

		minBufferLength = MAX(pstEncoder->bitrate/8, H1ENCODE_OUTBUF_MIN);
		minBufferLength = MIN(pstEncoder->memOSB.u32Size, minBufferLength);
	}

	pstEncoder->minBufferLength = minBufferLength;
	VENC_INFO("bitPerSecond = %d, minBufferLength = %d\n", rcCfg.bitPerSecond, minBufferLength);

	if((ret = HXENC_VP8SetRateCtrl( (HXENC_VP8INST_T)pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8SetRateCtrl - failed.(%d)\n", ret);
		_VP8Close(pstEncoder);
		return RET_ERROR;
	}

    /* Encoder setup: coding control */
	if((ret = HXENC_VP8SetCodingCtrl(pstEncoder->pEncInst, &codingCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8SetCodingCtrl - failed.(%d)\n", ret);
		_VP8Close(pstEncoder);
		return RET_ERROR;
	}

    /* PreP setup */
	preCfg.inputType = HXENC_YUV420_SEMIPLANAR;
	preCfg.inputWidth = pstEncoder->stride;
	preCfg.inputHeight = pstEncoder->height;
	//preCfg.interlacedFrame = interlacedFrame;

	if((ret = HXENC_VP8SetPreProcessing(pstEncoder->pEncInst, &preCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8SetPreProcessing - failed.(%d)\n", ret);
		_VP8Close(pstEncoder);
		return (int)ret;
	}

	pstEncoder->isEncodeOpened = TRUE;

	//pVP8->frameCntTotal = 0;

	return RET_OK;
}


static int _VP8Close( H1ENCODE_API_T *pstEncoder )
{
	HXENC_RET_T ret = HXENC_OK;

	if ( pstEncoder == NULL )
	{
		VENC_ERROR("%s: invalid argument.\n", __FUNCTION__);
		return RET_ERROR;
	}

	if ( !pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

	if((ret = HXENC_VP8Release( (HXENC_VP8INST_T)pstEncoder->pEncInst ) ) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8Release - failed.(%d)\n", ret);
		return ret;
	}

	pstEncoder->isEncodeOpened = FALSE;

	return ret;
}

static int _VP8SetBitrate( H1ENCODE_API_T *pstEncoder, UINT32 bitrate, BOOLEAN bEnableCBR )
{
	// ignore bEnableCBR parameter.

	HXENC_RET_T ret = HXENC_OK;
	HXENC_RATECTRL_T rcCfg = {0};

	if ( pstEncoder == NULL )
	{
		VENC_ERROR("%s: invalid argument.\n", __FUNCTION__);
		return RET_ERROR;
	}

	rcCfg.bEnableTargetBitrate = TRUE;
	rcCfg.bitPerSecond = bitrate;

	if(( HXENC_VP8SetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8SetRateCtrl - failed.(%d)\n", ret);
		return RET_ERROR;
	}

	return RET_OK;
}

static int _VP8GetStreamList( H1ENCODE_API_T *pstEncoder, HXENC_VP8_OUTPUT_T *pstEncOut, struct list_head *pStreamList )
{
	int i = 0;

	if ( pstEncoder == NULL || pstEncOut == NULL || pStreamList == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	if ( pstEncOut->frameSize == 0 )
	{
		return RET_ERROR;
	}

	if ( pstEncoder->bIncludeIVFHeader )
	{
		if ( pstEncoder->bIsFirstFrame )
		{
			_VP8IvfHeader( dataIvfHeader, pstEncoder->width, pstEncoder->height, pstEncoder->frameRateNum, pstEncoder->frameRateDenom, 0 );
			_StreamInfoPush( pStreamList, (UINT32)dataIvfHeader, IVF_HDR_BYTES );
		}

		_VP8IvfFrame( dataIvfFrames[pstEncoder->outputFrameIndex], pstEncOut->frameSize, pstEncoder->codedFrameCnt );
		_StreamInfoPush( pStreamList, (UINT32)dataIvfFrames[pstEncoder->outputFrameIndex], IVF_FRM_BYTES );
	}

	for ( i = 0; i < 9; i++ )
	{
		if ( pstEncOut->streamSize[i] > 0 )
		{
			_StreamInfoPush( pStreamList, (UINT32)pstEncOut->pOutBuf[i], pstEncOut->streamSize[i] );
		}
	}

	return RET_OK;
}

static int _VP8Encode( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_FRAME_READY;
	HXENC_INPUT_T		encIn={0};
	HXENC_VP8_OUTPUT_T	encOut={0};

	UINT32 encodingTime;

	struct list_head *pStreamList = _StreamListNew();
	pstEncParams->pStreamList = pStreamList;

	//TRACE_ENTER();

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( !pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

	{	// Setup encoder output
		H1ENCODE_MEM_T *memOutput;

		// Set coding type
		if ( pstEncoder->intraPeriodCnt == 0 )
		{
			encIn.codingType = HXENC_INTRA_FRAME;
		}
		else
		{
			encIn.codingType = HXENC_PREDICTED_FRAME;
		}

		// Set time increment
		if ( pstEncoder->bIsFirstFrame )
		{
			encIn.timeIncrement = 0;
		}
		else
		{
			encIn.timeIncrement = pstEncoder->frameRateDenom;	// Same as cfg.frameRateDenom
		}

		memOutput = &pstEncoder->memOutputFrame[pstEncoder->outputFrameIndex];
		pstEncoder->outputFrameIndex++;
		if ( pstEncoder->outputFrameIndex >= pstEncoder->outputFrameCount )
		{
			pstEncoder->outputFrameIndex = 0;
		}

		encIn.pOutBuf = (UINT32 *)memOutput->u32Virt;
		encIn.busOutBuf = memOutput->u32Phys;
		encIn.outBufSize = memOutput->u32Size;

		encIn.busLuma = pstEncParams->u32YBase;		//Physical address
		encIn.busChromaU = pstEncParams->u32CBase; //Physical address
		encIn.busChromaV = 0; 	//Not used
	}

	H1EncodeSetTime( pstEncoder->id, 0 );
	ret = HXENC_VP8StrmEncode( pstEncoder->pEncInst, &encIn, &encOut );
	encodingTime = H1EncodeGetTime( pstEncoder->id );

	switch (ret)
	{
		case HXENC_FRAME_READY:
		{
			H1ENCODE_AU_TYPE_T au_type;

#if defined(H1ENCODE_DEBUG_DUMP)
			H1EncodeSaveStream( encOut.pOutBuf[i], encOut.streamSize[i] );
#endif

			if ( encOut.codingType == HXENC_INTRA_FRAME )
				au_type = H1ENCODE_AU_TYPE_I;
			else
				au_type = H1ENCODE_AU_TYPE_P;

			pstEncParams->id				= pstEncoder->id;
			pstEncParams->u32Timestamp		= encIn.timestamp;
			pstEncParams->au_type			= au_type;
			pstEncParams->u32EncodingTime	= encodingTime;
			pstEncParams->pMemESB			= &pstEncoder->memESB;
			pstEncParams->pMemAUI			= &pstEncoder->memAUI;
			pstEncParams->u32FrameIndex 	= pstEncoder->codedFrameCnt;

			_VP8GetStreamList( pstEncoder, &encOut, pStreamList );
			_WorkQEnqueue( pstEncoder, pstEncParams );
		}
		break;

		default:
			VENC_ERROR("HXENC_VP8StrmEncode() failed. - %d\n", ret);
		break;
	}

	if (encOut.codingType != HXENC_NOTCODED_FRAME)
	{
		pstEncoder->intraPeriodCnt++;
		pstEncoder->codedFrameCnt++;
	}

	if ( pstEncoder->intraPeriodCnt > (pstEncoder->gopLength- 1) )
	{
		pstEncoder->intraPeriodCnt = 0;
	}

	if ( pstEncoder->bIsFirstFrame )
	{
		pstEncoder->bIsFirstFrame = FALSE;
	}

	if ( ret == HXENC_FRAME_READY || ret == HXENC_OK )
	{
		return RET_OK;
	}
	else
	{
		return RET_ERROR;
	}
}

static int _VP8EncodeAsync( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_FRAME_READY;
	HXENC_INPUT_T		encIn={0};
	H1ENCODE_MEM_T *memOutput;

	//TRACE_ENTER();

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( !pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

	if ( pstEncoder->bIsFirstFrame )
	{
		encIn.timeIncrement = 0;
	}
	else
	{
		encIn.timeIncrement = pstEncoder->frameRateDenom;	// Same as cfg.frameRateDenom
	}

	memOutput = &pstEncoder->memOutputFrame[pstEncoder->outputFrameIndex];
	pstEncoder->outputFrameIndex++;
	if ( pstEncoder->outputFrameIndex >= pstEncoder->outputFrameCount )
	{
		pstEncoder->outputFrameIndex = 0;
	}

	if ( pstEncoder->intraPeriodCnt == 0 )
	{
		encIn.codingType = HXENC_INTRA_FRAME;
	}
#if 0	
	// VP8 can't not support interlaced type.
	else if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE && pstEncoder->intraPeriodCnt == 1 )
	{
		encIn.codingType = HXENC_NONIDR_INTRA_FRAME;
	}
#endif
	else
	{
		encIn.codingType = HXENC_PREDICTED_FRAME;
	}

	{
		encIn.pOutBuf = (UINT32 *)memOutput->u32Virt;
		encIn.busOutBuf = memOutput->u32Phys;
		encIn.outBufSize = memOutput->u32Size;

		encIn.busLuma = pstEncParams->u32YBase;		//Physical address
		encIn.busChromaU = pstEncParams->u32CBase; //Physical address
		encIn.busChromaV = 0; 	//Not used
	}

	H1EncodeSetTime( pstEncoder->id, 0 );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		monitorTime[pstEncoder->id*2] = _NowMS();
	}

	ret = HXENC_VP8StrmEncodeAsync( pstEncoder->pEncInst, &encIn );

	return ret;
}

static int _VP8EncodeDone( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_FRAME_READY;
	HXENC_INPUT_T		encIn={0};
	HXENC_VP8_OUTPUT_T	encOut={0};

	UINT32 encodingTime = 0;

	if ( pstEncoder == NULL || pstEncoder->pEncInst == NULL
		|| pstEncoder->isEncodeOpened == FALSE )
	{
		return RET_ERROR;
	}

	encodingTime = H1EncodeGetTime( pstEncoder->id );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		UINT32 hw_ms = encodingTime / 1000;
		UINT32 hw_us = encodingTime % 1000;

		monitorTime[pstEncoder->id*2+1] = _NowMS();

		VENC_DEBUG("[%d] Encoding Time: [HW Clock: %d.%03d(ms)], [SW Timer: %lld(ms)]",
			pstEncoder->id, hw_ms, hw_us, monitorTime[pstEncoder->id*2+1] - monitorTime[pstEncoder->id*2]);
	}

	// Get result of encode
	ret = HXENC_VP8StrmEncodeDone( pstEncoder->pEncInst, &encIn, &encOut );

	switch (ret)
	{
		case HXENC_FRAME_READY:
		{
			H1ENCODE_AU_TYPE_T au_type;
			struct list_head *pStreamList = _StreamListNew();
			pstEncParams->pStreamList = pStreamList;

#if defined(H1ENCODE_DEBUG_DUMP)
			H1EncodeSaveStream( encOut.pOutBuf[i], encOut.streamSize[i] );
#endif

			if ( encOut.codingType == HXENC_INTRA_FRAME )
				au_type = H1ENCODE_AU_TYPE_I;
			else
				au_type = H1ENCODE_AU_TYPE_P;

			pstEncParams->id				= pstEncoder->id;
			pstEncParams->u32Timestamp		= encIn.timestamp;
			pstEncParams->au_type			= au_type;
			pstEncParams->u32EncodingTime	= encodingTime;
			pstEncParams->pMemESB			= &pstEncoder->memESB;
			pstEncParams->pMemAUI			= &pstEncoder->memAUI;
			pstEncParams->u32FrameIndex 	= pstEncoder->codedFrameCnt;
			pstEncParams->frameType 		= LX_VENC_FRAME_TYPE_PROGRESSIVE;
			
			_VP8GetStreamList( pstEncoder, &encOut, pStreamList );
			_WorkQEnqueue( pstEncoder, pstEncParams );
		}
		break;

		default:
			VENC_ERROR("HXENC_VP8StrmEncode() failed. - %d\n", ret);
		break;
	}

	if (encOut.codingType != HXENC_NOTCODED_FRAME)
	{
		pstEncoder->intraPeriodCnt++;
		pstEncoder->codedFrameCnt++;
	}

	if ( pstEncoder->intraPeriodCnt > (pstEncoder->gopLength- 1) )
	{
		pstEncoder->intraPeriodCnt = 0;
	}

	if ( pstEncoder->bIsFirstFrame )
	{
		pstEncoder->bIsFirstFrame = FALSE;
	}

	if ( ret == HXENC_FRAME_READY || ret == HXENC_OK )
	{
		return RET_OK;
	}
	else
	{
		return RET_ERROR;
	}
}


static void _VP8IvfHeader( UINT8 *data, UINT32 width, UINT32 height, UINT32 outputRateNumer, UINT32 outputRateDenom, UINT32 frameCntTotal)
{
	//UINT8 *data = dataIvfHeader;

	if ( data == NULL )
	{
		return;
	}

    /* File header signature */
    data[0] = 'D';
    data[1] = 'K';
    data[2] = 'I';
    data[3] = 'F';

    /* File format version and file header size */
    data[6] = 32;

    /* Video data FourCC */
    data[8] = 'V';
    data[9] = 'P';
    data[10] = '8';
    data[11] = '0';

    /* Video Image width and height */
    data[12] = width & 0xff;
    data[13] = (width >> 8) & 0xff;
    data[14] = height & 0xff;
    data[15] = (height >> 8) & 0xff;

    /* Frame rate rate */
    data[16] = outputRateNumer & 0xff;
    data[17] = (outputRateNumer >> 8) & 0xff;
    data[18] = (outputRateNumer >> 16) & 0xff;
    data[19] = (outputRateNumer >> 24) & 0xff;

    /* Frame rate scale */
    data[20] = outputRateDenom & 0xff;
    data[21] = (outputRateDenom >> 8) & 0xff;
    data[22] = (outputRateDenom >> 16) & 0xff;
    data[23] = (outputRateDenom >> 24) & 0xff;

    /* Video length in frames */
    data[24] = frameCntTotal & 0xff;
    data[25] = (frameCntTotal >> 8) & 0xff;
    data[26] = (frameCntTotal >> 16) & 0xff;
    data[27] = (frameCntTotal >> 24) & 0xff;

    /* The Ivf "File Header" is in the beginning of the file */
#if defined(H1ENCODE_DEBUG_DUMP)
	H1EncodeSaveStream( (UINT32 *)data, IVF_HDR_BYTES );
#endif	// H1ENCODE_DEBUG_DUMP

	//_VP8StreamWrite( pVP8, H1ENCODE_AU_TYPE_START, (UINT32)data, IVF_HDR_BYTES );

}

static void _VP8IvfFrame( UINT8 *data, UINT32 frameSize, UINT64 frameCntTotal )
{
    int byteCnt = 0;
	//UINT8 *data = dataIvfFrame;

	if ( data == NULL )
	{
		return;
	}

    /* Frame size (4 bytes) */
    byteCnt = frameSize;
    data[0] =  byteCnt        & 0xff;
    data[1] = (byteCnt >> 8)  & 0xff;
    data[2] = (byteCnt >> 16) & 0xff;
    data[3] = (byteCnt >> 24) & 0xff;

    /* Timestamp (8 bytes) */
    data[4]  =  (frameCntTotal)        & 0xff;
    data[5]  = ((frameCntTotal) >> 8)  & 0xff;
    data[6]  = ((frameCntTotal) >> 16) & 0xff;
    data[7]  = ((frameCntTotal) >> 24) & 0xff;
    data[8]  = ((frameCntTotal) >> 32) & 0xff;
    data[9]  = ((frameCntTotal) >> 40) & 0xff;
    data[10] = ((frameCntTotal) >> 48) & 0xff;
    data[11] = ((frameCntTotal) >> 56) & 0xff;

#if defined(H1ENCODE_DEBUG_DUMP)
	H1EncodeSaveStream( (UINT32 *)data, IVF_FRM_BYTES );
#endif	// H1ENCODE_DEBUG_DUMP

	//_VP8StreamWrite( pVP8, H1ENCODE_AU_TYPE_START, (UINT32)data, IVF_FRM_BYTES );

}


