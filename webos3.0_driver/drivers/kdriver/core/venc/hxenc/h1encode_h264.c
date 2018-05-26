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
#define H264_ENABLE_FIXED_FRAME_RATE_FLAG

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/kthread.h>
#include <asm/io.h>			// for ioremap()/iounmap()
//#include <linux/wait.h> 	// for interruptible_sleep_on_timeout()
#include <linux/slab.h>
#include <linux/delay.h>	// for msleep()

#include "../venc_drv.h"

#include "h1encode.h"

#ifdef VENC_HXENC_BUILTIN
#include "hxenc_api.h"
#else
#include "h1encode_wrapper.h"
#endif

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define VENC_ALIGNMENT_SIZE		4

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
long long _NowUS( void );

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static int _H264Open( H1ENCODE_API_T *pstEncoder); //, H1ENCODE_INPUT_CONFIG_T *pConfig );
static int _H264Close( H1ENCODE_API_T *pstEncoder );
static int _H264Encode( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
static int _H264EncodeAsync( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
static int _H264EncodeDone( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );

// setter
static int _H264SetBitrate( H1ENCODE_API_T *pstEncoder, u32 bitrate, BOOLEAN bEnableCBR );
static int _H264SetGOPLength( H1ENCODE_API_T *pstEncoder, u32 gopLength );
static int _H264SetQPValue( H1ENCODE_API_T *pstEncoder, u32 qp );

// getter
static int  _H264GetQPValue( H1ENCODE_API_T *pstEncoder, u32 *pQP );


/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
#if 0
static const char *streamType[2] = {
	"BYTE_STREAM",
	"NAL_UNITS"
};

static const char *viewMode[6] = {
	"H264_DOUBLE_BUFFER",
	"H264_SINGLE_BUFFER",
	"MVC_INTER_VIEW_PRED",
	"MVC_INTER_PRED",
	"BASE_VIEW_MULTI_BUFFER",
	"INTERLACED"
};
#endif

static const u8 H264NAL_AUD[][8] = {
	{ 0x00, 0x00, 0x00, 0x01, 0x09, 0x10, 0x00, 0x00 },	// IDR-Frame
	{ 0x00, 0x00, 0x00, 0x01, 0x09, 0x20, 0x00, 0x00 },	// B-Frame
	{ 0x00, 0x00, 0x00, 0x01, 0x09, 0x30, 0x00, 0x00 },	// P-Frame
};

// SPS, PPS
static u8 h264NALHeader[256];
static u8 h264NALHeaderLength;

#ifdef H1ENCODE_DEBUG_MEM_LEAK
atomic_t api_count = ATOMIC_INIT(0);
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/

H1ENCODE_API_T *H264Alloc( void )
{
	H1ENCODE_API_T *pstEncoder = (H1ENCODE_API_T *)kcalloc( 1, sizeof(H1ENCODE_API_T), GFP_KERNEL );

	TRACE_ENTER();

	if ( pstEncoder != NULL )
	{
		memset(pstEncoder, 0x0, sizeof(H1ENCODE_API_T));
		H1EncodeAPISetDefault( pstEncoder );

		pstEncoder->eEncodeType = LX_VENC_ENCODE_TYPE_H264;

		// allocate encoding functions
		pstEncoder->Open 	= _H264Open;
		pstEncoder->Close 	= _H264Close;
		pstEncoder->Encode 	= _H264Encode;
		pstEncoder->EncodeAsync 	= _H264EncodeAsync;
		pstEncoder->EncodeDone 		= _H264EncodeDone;

		// override default functions
		pstEncoder->SetBitrate 		= _H264SetBitrate;
		pstEncoder->SetGOPLength 	= _H264SetGOPLength;
		pstEncoder->SetQPValue 		= _H264SetQPValue;

		pstEncoder->GetQPValue 		= _H264GetQPValue;

#ifdef H1ENCODE_DEBUG_MEM_LEAK
		atomic_inc( &api_count );
#endif

	}

	TRACE_EXIT();

	return pstEncoder;
}

void H264Release(H1ENCODE_API_T * pstEncoder)
{
	TRACE_ENTER();

	if ( pstEncoder != NULL )
	{
		kfree(pstEncoder);

#ifdef H1ENCODE_DEBUG_MEM_LEAK
		atomic_dec( &api_count );
#endif

	}

	TRACE_EXIT();
}

void H264EncTrace(const char *msg)
{
	VENC_TRACE("%s\n", msg);
}

static int _H264Open( H1ENCODE_API_T *pstEncoder )
{
	HXENC_RET_T ret;
	HXENC_CONFIG_T	config = {0};
	HXENC_RATECTRL_T rcCfg = {0};
	HXENC_CODINGCTRL_T codingCfg = {0};
	HXENC_PREPROCESSING_CFG_T preCfg = {0};

	int inputWidth, inputHeight;
	int minBufferLength = H1ENCODE_OUTBUF_MIN;

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

#ifdef H264_ENABLE_FIXED_FRAME_RATE_FLAG
	// H.264 framerate calcultation
	// fixed_frame_rate_flag is 1, framerate = time_scale / (2 * num_units_in_tick)
	// progressive type should be doubled the frameRateNum.
	// but, fixed_frame_rate_flag to 0 on google h1 encoder
	if ( pstEncoder->eFrameType == LX_VENC_FRAME_TYPE_PROGRESSIVE )
	{
		pstEncoder->frameRateNum = pstEncoder->frameRateNum * 2;
	}
#endif

	config.coreid = pstEncoder->id;
	config.frameRateDenom = pstEncoder->frameRateDenom;
	config.frameRateNum = pstEncoder->frameRateNum;
	config.width = pstEncoder->width;
	config.height = pstEncoder->height;
	config.eFrameType = pstEncoder->eFrameType;

	if ((ret = HXENC_H264Init( &config, (HXENC_H264INST_T *)&pstEncoder->pEncInst )) != HXENC_OK )
	{
		VENC_ERROR("HXENC_H264Init - failed.(%d)\n", ret);

		return RET_ERROR;
	}

	rcCfg.bEnableTargetBitrate = TRUE;
	rcCfg.bEnableGOPLength = TRUE;
	rcCfg.gopLength = pstEncoder->gopLength;
	if ( pstEncoder->bitrate >= H1ENCODE_BITRATE_MIN && pstEncoder->bitrate <= H1ENCODE_BITRATE_MAX )
	{
		rcCfg.bitPerSecond = pstEncoder->bitrate;

		// Target Bitrates in DVR Settings,
		// DTV Low (720p) : 2,748,000 bits
		// ATV High (480i): 2,562,000 bits
		// ATV Low (480i) : 1,933,000 bits

		minBufferLength = MAX(pstEncoder->bitrate/8*2, H1ENCODE_OUTBUF_MIN);
		minBufferLength = MIN(pstEncoder->memOSB.u32Size, minBufferLength);
	}

	pstEncoder->minBufferLength = minBufferLength;
	VENC_INFO("bitPerSecond = %d, minBufferLength = %d\n", rcCfg.bitPerSecond, minBufferLength);

	if ( pstEncoder->bEnableCBR )
	{
		rcCfg.bEnableCBR = TRUE;
		rcCfg.qpMin = 25;

		if ( rcCfg.bitPerSecond > 0 )	rcCfg.hrdCpbSize = rcCfg.bitPerSecond >> 2;
	}

	if((ret = HXENC_H264SetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetRateCtrl - failed.(%d)\n", ret);

		_H264Close(pstEncoder);

		return RET_ERROR;
	}

	// fieldOrder(0:bottom-first, 1:top-first)
	codingCfg.fieldOrder = pstEncoder->topFieldFirst ? 1 : 0;

#ifdef H1ENCODE_ENABLE_SAR
	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SAR ) )
	{
		if ( pstEncoder->decWidth > 0 && pstEncoder->decHeight > 0 )
		{
			// Calc SAR value
			codingCfg.sarWidth	= pstEncoder->decWidth;
			codingCfg.sarHeight = pstEncoder->decHeight;

			// temporory :
			// If there is no SAR infomation, de writes sar infomation equal to input resolution.
			// So, in this situations, sar set 1:1.
			if ( pstEncoder->decWidth == config.width && pstEncoder->decHeight == config.height )
			{
				codingCfg.sarWidth	= 0;
				codingCfg.sarHeight = 0;
			}
		}
		else
		{
			// If a DAR invalid, SAR to set to 1:1.
			codingCfg.sarWidth	= 0;
			codingCfg.sarHeight = 0;
		}
	}
#endif

	if((ret = HXENC_H264SetCodingCtrl(pstEncoder->pEncInst, &codingCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetCodingCtrl - failed.(%d)\n", ret);
		_H264Close(pstEncoder);

		return RET_ERROR;
	}

	inputWidth = pstEncoder->stride;
	inputHeight	= pstEncoder->height;

	if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
	{
		if ( inputWidth == 4096 )
		{
			inputWidth = 2048;
		}
		else
		{
			inputHeight = inputHeight / 2;
		}
	}

	preCfg.inputWidth = inputWidth;
	preCfg.inputHeight = inputHeight;
	preCfg.inputType = HXENC_YUV420_SEMIPLANAR;

#ifdef H1ENCODE_ENABLE_SCD
	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SCD ) )
	{
			preCfg.videoStabilization = 1;
	}
#endif

	if((ret = HXENC_H264SetPreProcessing(pstEncoder->pEncInst, &preCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetPreProcessing - failed.(%d)\n", ret);
		_H264Close(pstEncoder);

		return RET_ERROR;
	}

	// Reset some variables
	pstEncoder->isEncodeOpened = TRUE;
	pstEncoder->bIsFirstFrame = TRUE;

	return RET_OK;
}


static int _H264Close( H1ENCODE_API_T *pstEncoder )
{
	HXENC_RET_T ret;

	//TRACE_ENTER();

	if ( !pstEncoder->isEncodeOpened )
	{
		return RET_ERROR;
	}

	// pH264->streamList를 비워야 함.
	// _StreamInfoClear( &pstEncoder->streamList );

	if((ret = HXENC_H264Release( (HXENC_H264INST_T)pstEncoder->pEncInst )) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264Release - failed.(%d)\n", ret);

		return RET_ERROR;
	}

	pstEncoder->isEncodeOpened = FALSE;

	//TRACE_EXIT();

	return RET_OK;
}

static int _H264SetBitrate( H1ENCODE_API_T *pstEncoder, u32 bitrate, BOOLEAN bEnableCBR )
{
	HXENC_RET_T ret;
	HXENC_RATECTRL_T rcCfg = {0};

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( bitrate < H1ENCODE_BITRATE_MIN )
	{
		bitrate = H1ENCODE_BITRATE_MIN;
	}

	if ( bitrate > H1ENCODE_BITRATE_MAX )
	{
		bitrate = H1ENCODE_BITRATE_MAX;
	}

	if ( pstEncoder->isEncodeOpened )
	{
		rcCfg.bEnableTargetBitrate = TRUE;
		rcCfg.bitPerSecond = bitrate;
		rcCfg.bEnableCBR = bEnableCBR;

		if(( ret = HXENC_H264SetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264SetRateCtrl - failed.(%d)\n", ret);
			return RET_ERROR;
		}
	}

	printk("%s :: bitrate[%d] bEnableCBR[%d]\n", __F__, pstEncoder->bitrate, pstEncoder->bEnableCBR );

	pstEncoder->bitrate = bitrate;
	pstEncoder->bEnableCBR = bEnableCBR;

	return RET_OK;
}

static int _H264SetGOPLength( H1ENCODE_API_T *pstEncoder, u32 gopLength )
{
	HXENC_RET_T ret;
	HXENC_RATECTRL_T rcCfg = {0};

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( gopLength < H1ENCODE_GOPLENGTH_MIN )
	{
		gopLength = H1ENCODE_GOPLENGTH_MIN;
	}

	if ( gopLength > H1ENCODE_GOPLENGTH_MAX )
	{
		gopLength = H1ENCODE_GOPLENGTH_MAX;
	}

	if ( pstEncoder->isEncodeOpened )
	{
		rcCfg.bEnableGOPLength = TRUE;
		rcCfg.gopLength = gopLength;

		if(( ret = HXENC_H264SetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264SetRateCtrl - failed.(%d)\n", ret);
			return RET_ERROR;
		}
	}

	pstEncoder->gopLength = gopLength;

	return RET_OK;
}

static int _H264SetQPValue( H1ENCODE_API_T *pstEncoder, u32 qp )
{
	HXENC_RET_T ret;
	HXENC_RATECTRL_T rcCfg = {0};

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( qp > H1ENCODE_QP_MAX )
	{
		qp = H1ENCODE_QP_MAX;
	}

	if ( pstEncoder->isEncodeOpened )
	{
		rcCfg.bEnableFixedQP = TRUE;
		rcCfg.qp = qp;

		if(( ret = HXENC_H264SetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264SetRateCtrl - failed.(%d)\n", ret);
			return RET_ERROR;
		}
	}

	pstEncoder->qp = qp;

	return RET_OK;
}

static int _H264GetQPValue( H1ENCODE_API_T *pstEncoder, u32 *pQP )
{
	HXENC_RET_T ret;
	HXENC_RATECTRL_T rcCfg = {0};
	u32 qp = 0;

	if ( pstEncoder == NULL || pQP == NULL )
	{
		return RET_ERROR;
	}

	if ( pstEncoder->isEncodeOpened )
	{
		if(( ret = HXENC_H264GetRateCtrl(pstEncoder->pEncInst, &rcCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264GetRateCtrl - failed.(%d)\n", ret);
			return RET_ERROR;
		}

		qp = rcCfg.qp;
	}

	*pQP = qp;

	return RET_OK;
}

static void _H264SetNextInput( H1ENCODE_MEM_T *pstMem, HXENC_INPUT_T *pstIn, UINT32 minLength )
{
	u32 nextOutBufferSize = pstMem->u32Size - pstMem->u32WriteOffset;

	if ( nextOutBufferSize < minLength )
	{
		//gH1Encode.u32OutBufferEndOffset = pH264->memCPB.u32WriteOffsetiteOffset;

		pstMem->u32WriteOffset = 0;
		//pstMem->u32OutBufferRd = 0;
	}

	// 출력 버퍼 설정
	pstIn->pOutBuf = (u32 *)(pstMem->u32Virt + pstMem->u32WriteOffset);
	pstIn->busOutBuf = pstMem->u32Phys + pstMem->u32WriteOffset;
	pstIn->outBufSize = pstMem->u32Size - pstMem->u32WriteOffset;

#if 0
	VENC_DEBUG("[%s] 0x%08x, 0x%08x, 0x%08x\n", __F__, (u32)pstIn->pOutBuf, (u32)pstIn->busOutBuf, (u32)pstIn->outBufSize);
#endif
	return;
}

static u32 _H264SetWriteOffset( H1ENCODE_MEM_T *pstMem, u32 streamSize, BOOLEAN bNeedAlign )
{
	u32 paddingSize = 0;

	// add to Write offset by streamSize
	pstMem->u32WriteOffset += streamSize;

	if (bNeedAlign)
	{
		paddingSize = MROUND64( pstMem->u32WriteOffset ) - pstMem->u32WriteOffset;

		if (paddingSize > 0)
		{
			u32 start = pstMem->u32Virt + pstMem->u32WriteOffset;

			// clear padding data.
			memset ( (void *)start, 0x0, paddingSize );
			pstMem->u32WriteOffset += paddingSize;

			VENC_DEBUG("[ESBuffer][ALIGN] streamSize:%d, paddingSize:%d\n", streamSize, paddingSize);
		}
	}

	return paddingSize;
}

static int _H264Encode( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_ERROR;
	HXENC_INPUT_T	encIn = {0};
	HXENC_H264_OUTPUT_T encOut = {0};

	u32 encodingTime = 0;
	struct list_head *pStreamList;
	u64 monitorTime[2] = {};

	if ( pstEncoder == NULL || pstEncoder->pEncInst == NULL
		|| pstEncoder->isEncodeOpened == FALSE )
	{
		return RET_ERROR;
	}

	ret = HXENC_FRAME_READY;

	if ( pstEncoder->bIsFirstFrame )
		encIn.timeIncrement = 0;
#ifdef H264_ENABLE_FIXED_FRAME_RATE_FLAG
	else if ( pstEncoder->eFrameType == LX_VENC_FRAME_TYPE_PROGRESSIVE )
		encIn.timeIncrement = pstEncoder->frameRateDenom * 2;
#endif
	else
		encIn.timeIncrement = pstEncoder->frameRateDenom;

	if ( pstEncoder->bIsFirstFrame )
	{
		_H264SetNextInput( &pstEncoder->memOSB, &encIn, pstEncoder->minBufferLength );
		ret = HXENC_H264StrmStart( pstEncoder->pEncInst, &encIn, &encOut );
		if ( ret == HXENC_OK )
		{
			memcpy( h264NALHeader, encIn.pOutBuf, encOut.streamSize );
			h264NALHeaderLength = encOut.streamSize;

#ifdef VENC_ALIGNMENT_SIZE
			if ((h264NALHeaderLength % VENC_ALIGNMENT_SIZE) > 0)
			{
				int padding = VENC_ALIGNMENT_SIZE - (h264NALHeaderLength % VENC_ALIGNMENT_SIZE);

				memset(h264NALHeader+h264NALHeaderLength, 0x0, padding);
				h264NALHeaderLength += padding;
			}
#endif
		}
		else
		{
			VENC_ERROR("HXENC_H264StrmStart : %d\n", ret);
		}

		pstEncoder->tb = 0;
	}

	if ( pstEncoder->intraPeriodCnt == 0 )
	{
		encIn.codingType = HXENC_INTRA_FRAME;
	}
	else if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE && pstEncoder->intraPeriodCnt == 1 )
	{
		encIn.codingType = HXENC_NONIDR_INTRA_FRAME;
	}
	else
	{
		encIn.codingType = HXENC_PREDICTED_FRAME;
	}

	// Setup encoder input
	{
		encIn.busLuma 		= pstEncParams->u32YBase;	//Physical address of Y plain (Y)
		encIn.busChromaU 	= pstEncParams->u32CBase;	//Physical address of C plain (UV)
		encIn.busChromaV 	= 0; 						//Not used in 4:2:0 format
#ifdef H1ENCODE_ENABLE_SCD
		encIn.busLumaStab	= pstEncParams->u32YBaseNext;
#endif
	}

	_H264SetNextInput( &pstEncoder->memOSB, &encIn, pstEncoder->minBufferLength );

	H1EncodeSetTime( pstEncoder->id, 0 );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		monitorTime[0] = _NowMS();
	}

	ret = HXENC_H264StrmEncode( pstEncoder->pEncInst, &encIn, &encOut );

	encodingTime = H1EncodeGetTime( pstEncoder->id );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		UINT32 hw_ms = encodingTime / 1000;
		UINT32 hw_us = encodingTime % 1000;

		monitorTime[1] = _NowMS();

		VENC_DEBUG("Encoding Time: [HW Clock: %d.%03d(ms)], [SW Timer: %lld(ms)]",
			hw_ms, hw_us, monitorTime[1] - monitorTime[0]);
	}

	switch( ret )
	{
		case HXENC_FRAME_READY:
		{
			H1ENCODE_AU_TYPE_T au_type;

			if ( encOut.streamSize == 0 )
			{
				VENC_WARN("Output stream size is 0.\n");
				break;
			}

			if (encIn.codingType == HXENC_INTRA_FRAME)
				au_type = H1ENCODE_AU_TYPE_I;
			else
				au_type = H1ENCODE_AU_TYPE_P;

			pStreamList = _StreamListNew();
			pstEncParams->pStreamList = pStreamList;

			// Adding AUD
			if (encIn.codingType == HXENC_INTRA_FRAME)
			{
				_StreamInfoPush( pStreamList, (u32)H264NAL_AUD[0], 8 );
			}
			else
			{
				_StreamInfoPush( pStreamList, (u32)H264NAL_AUD[2], 8 );
			}

			// Adding SPS/PPS
			if ( pstEncoder->intraPeriodCnt == 0 )
			{
				_StreamInfoPush( pStreamList, (u32)h264NALHeader, h264NALHeaderLength );
			}

			// Add ES Stream

			encOut.streamSize += _H264SetWriteOffset( &pstEncoder->memOSB, encOut.streamSize, TRUE );
			_StreamInfoPush( pStreamList, (u32)encIn.pOutBuf, encOut.streamSize );

			pstEncParams->au_type 			= au_type;
			pstEncParams->u32EncodingTime 	= encodingTime;
			pstEncParams->pMemESB			= &pstEncoder->memESB;
			pstEncParams->pMemAUI			= &pstEncoder->memAUI;
			pstEncParams->u32FrameIndex		= pstEncoder->codedFrameCnt;
#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
			pstEncParams->tb				= pstEncoder->tb;
			pstEncParams->interlaced		= (pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE);
#endif

			_WorkQEnqueue( pstEncoder, pstEncParams );
		}
			break;
		default:
		{
			VENC_ERROR("HXENC_H264StrmEncode:NOT_OK(%d)\n", ret);
		}
			break;
	}

	if (encOut.codingType != HXENC_NOTCODED_FRAME)
	{
		if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
		{
			pstEncoder->tb++;
			pstEncoder->tb = pstEncoder->tb % 2;
		}

		pstEncoder->intraPeriodCnt++;

#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
		if ( pstEncoder->tb == 0 && pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
		{
			pstEncoder->codedFrameCnt++;
		}
#else
		pstEncoder->codedFrameCnt++;
#endif
	}

	if ( pstEncoder->intraPeriodCnt > (pstEncoder->gopLength- 1) )
	{
		pstEncoder->intraPeriodCnt = 0;
	}

#if 0
	// !!! NEED TEST
	if ( pstEncoder->isNeedEndStream )
	{
		_H264StreamEnd( pH264 );
		pstEncoder->isNeedEndStream = FALSE;
	}
#endif

	if ( pstEncoder->bIsFirstFrame  )
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

static u64 monitorTime[4] = {};

static int _H264EncodeAsync( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_ERROR;
	HXENC_INPUT_T	encIn = {};
	//HXENC_H264_OUTPUT_T encOut = {0};

	//u32 encodingTime = 0;
	//struct list_head *pStreamList;

	if ( pstEncoder == NULL || pstEncoder->pEncInst == NULL
		|| pstEncoder->isEncodeOpened == FALSE )
	{
		return RET_ERROR;
	}

	ret = HXENC_FRAME_READY;

	if ( pstEncoder->bIsFirstFrame )
		encIn.timeIncrement = 0;
#ifdef H264_ENABLE_FIXED_FRAME_RATE_FLAG
	else if ( pstEncoder->eFrameType == LX_VENC_FRAME_TYPE_PROGRESSIVE )
		encIn.timeIncrement = pstEncoder->frameRateDenom * 2;
#endif
	else
		encIn.timeIncrement = pstEncoder->frameRateDenom;

	if ( pstEncoder->bIsFirstFrame )
	{
		HXENC_H264_OUTPUT_T encOut = {};

		_H264SetNextInput( &pstEncoder->memOSB, &encIn, pstEncoder->minBufferLength );
		ret = HXENC_H264StrmStart( pstEncoder->pEncInst, &encIn, &encOut );
		if ( ret == HXENC_OK )
		{
			memcpy( h264NALHeader, encIn.pOutBuf, encOut.streamSize );
			h264NALHeaderLength = encOut.streamSize;

#ifdef VENC_ALIGNMENT_SIZE
			if ((h264NALHeaderLength % VENC_ALIGNMENT_SIZE) > 0)
			{
				int padding = VENC_ALIGNMENT_SIZE - (h264NALHeaderLength % VENC_ALIGNMENT_SIZE);

				memset(h264NALHeader+h264NALHeaderLength, 0x0, padding);
				h264NALHeaderLength += padding;
			}
#endif

		}
		else
		{
			VENC_ERROR("HXENC_H264StrmStart : %d\n", ret);
		}

		pstEncoder->tb = 0;
	}

	if ( pstEncoder->intraPeriodCnt == 0 )
	{
		encIn.codingType = HXENC_INTRA_FRAME;
	}
	else if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE && pstEncoder->intraPeriodCnt == 1 )
	{
		encIn.codingType = HXENC_NONIDR_INTRA_FRAME;
	}
	else
	{
		encIn.codingType = HXENC_PREDICTED_FRAME;
	}

	// Setup encoder input
	{
		encIn.busLuma 		= pstEncParams->u32YBase;	//Physical address of Y plain (Y)
		encIn.busChromaU 	= pstEncParams->u32CBase;	//Physical address of C plain (UV)
		encIn.busChromaV 	= 0; 						//Not used in 4:2:0 format
#ifdef H1ENCODE_ENABLE_SCD
		encIn.busLumaStab	= pstEncParams->u32YBaseNext;
#endif
		encIn.timestamp		= pstEncParams->u32Timestamp;
	}

	_H264SetNextInput( &pstEncoder->memOSB, &encIn, pstEncoder->minBufferLength );

	H1EncodeSetTime( pstEncoder->id, 0 );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		monitorTime[pstEncoder->id*2] = _NowMS();
	}

	ret = HXENC_H264StrmEncodeAsync( pstEncoder->pEncInst, &encIn );

	//VENC_ERROR("HXENC_H264StrmEncodeAsync = %d\n", ret);

	return ret;
}


static int _H264EncodeDone( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	HXENC_RET_T ret = HXENC_ERROR;
	HXENC_INPUT_T		encIn = {};
	HXENC_H264_OUTPUT_T encOut = {};

	u32 encodingTime = 0;

	if ( pstEncoder == NULL || pstEncoder->pEncInst == NULL
		|| pstEncoder->isEncodeOpened == FALSE )
	{
		return RET_ERROR;
	}

	encodingTime = H1EncodeGetTime(pstEncoder->id);

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_DEBUG_WORST ) )
	{
		mdelay( 5 + encodingTime % 7 );
		encodingTime = H1EncodeGetTime(pstEncoder->id);
	}
	else if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
	{
		UINT32 hw_ms = encodingTime / 1000;
		UINT32 hw_us = encodingTime % 1000;

		monitorTime[pstEncoder->id*2+1] = _NowMS();

		VENC_DEBUG("[%d] Encoding Time: [HW Clock: %d.%03d(ms)], [SW Timer: %lld(ms)]",
			pstEncoder->id, hw_ms, hw_us, monitorTime[pstEncoder->id*2+1] - monitorTime[pstEncoder->id*2]);
	}

	// Get result of encode
	ret = HXENC_H264StrmEncodeDone( pstEncoder->pEncInst, &encIn, &encOut );

	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_DEBUG_WORST ) )
	{
		if ( (pstEncoder->codedFrameCnt + 1) % 30 == 0 )
		{
			VENC_DEBUG("[%d] Make delay for testing.\n", pstEncoder->id);
			mdelay( 13 );
		}

		if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) )
		{
			UINT32 hw_ms = encodingTime / 1000;
			UINT32 hw_us = encodingTime % 1000;

			monitorTime[pstEncoder->id*2+1] = _NowMS();

			VENC_DEBUG("[%d] Encoding Time: [HW Clock: %d.%03d(ms)], [SW Timer: %lld(ms)]",
				pstEncoder->id, hw_ms, hw_us, monitorTime[pstEncoder->id*2+1] - monitorTime[pstEncoder->id*2]);
		}
	}

	switch( ret )
	{
		case HXENC_FRAME_READY:
		{
			H1ENCODE_AU_TYPE_T au_type;
			struct list_head *pStreamList;

			if ( encOut.streamSize == 0 )
			{
				VENC_WARN("Output stream size is 0.\n");
				break;
			}

			if (encOut.codingType == HXENC_INTRA_FRAME)
				au_type = H1ENCODE_AU_TYPE_I;
			else
				au_type = H1ENCODE_AU_TYPE_P;

			pStreamList = _StreamListNew();
			pstEncParams->pStreamList = pStreamList;

			// Adding AUD
			if (encOut.codingType == HXENC_INTRA_FRAME)
			{
				_StreamInfoPush( pStreamList, (u32)H264NAL_AUD[0], 8 );
			}
			else
			{
				_StreamInfoPush( pStreamList, (u32)H264NAL_AUD[2], 8 );
			}

			// Adding SPS/PPS
			if ( pstEncoder->intraPeriodCnt == 0 )
			{
				_StreamInfoPush( pStreamList, (u32)h264NALHeader, h264NALHeaderLength );
			}

			// Add ES Stream

			encOut.streamSize += _H264SetWriteOffset( &pstEncoder->memOSB, encOut.streamSize, TRUE );
			_StreamInfoPush( pStreamList, (u32)encIn.pOutBuf, encOut.streamSize );

			//printk("%s: after _H264SetWriteOffset()\n", __F__);
			pstEncParams->id 				= pstEncoder->id;
			pstEncParams->u32Timestamp		= encIn.timestamp;
			pstEncParams->au_type 			= au_type;
			pstEncParams->u32EncodingTime 	= encodingTime;
			pstEncParams->pMemESB			= &pstEncoder->memESB;
			pstEncParams->pMemAUI			= &pstEncoder->memAUI;
			pstEncParams->u32FrameIndex		= pstEncoder->codedFrameCnt;
#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
			pstEncParams->tb				= pstEncoder->tb;
			pstEncParams->interlaced		= (pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE);
#endif
			if (pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE)
			{
				pstEncParams->frameType		= pstEncoder->tb ? LX_VENC_FRAME_TYPE_BOTTOM : LX_VENC_FRAME_TYPE_TOP;
			}
			else
			{
				pstEncParams->frameType		= LX_VENC_FRAME_TYPE_PROGRESSIVE;
			}

			_WorkQEnqueue( pstEncoder, pstEncParams );
		}
			break;
		default:
		{
			VENC_ERROR("HXENC_H264StrmEncode:NOT_OK(%d)\n", ret);

			if ( ret == HXENC_HW_TIMEOUT )
			{
				VENC_ERROR("encodingTime = %d.%dms\n", encodingTime/1000, encodingTime%1000);
			}
		}
			break;
	}

	if (encOut.codingType != HXENC_NOTCODED_FRAME)
	{
		if ( pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
		{
			pstEncoder->tb++;
			pstEncoder->tb = pstEncoder->tb % 2;
		}

		pstEncoder->intraPeriodCnt++;

#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
		if ( pstEncoder->tb == 0 && pstEncoder->eFrameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
		{
			pstEncoder->codedFrameCnt++;
		}
#else
		pstEncoder->codedFrameCnt++;
#endif
	}

	if ( pstEncoder->intraPeriodCnt > (pstEncoder->gopLength- 1) )
	{
		pstEncoder->intraPeriodCnt = 0;
	}

#if 0
	// !!! NEED TEST
	if ( pstEncoder->isNeedEndStream )
	{
		_H264StreamEnd( pH264 );
		pstEncoder->isNeedEndStream = FALSE;
	}
#endif

	if ( pstEncoder->bIsFirstFrame  )
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

