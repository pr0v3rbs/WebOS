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
 *  date		2012.08.08
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


#include "base_types.h"
#include "os_util.h"
#include "../venc_cfg.h"
#include "../venc_drv.h"

#include "venc_h13b0_regprep.h"
#include "venc_kapi.h"

#include "h1encode.h"

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

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/
#ifdef H1ENCODE_DEBUG_MEM_LEAK
extern atomic_t encparams_count;
#endif

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static void _DefaultSetOSB( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memOSB);
static void _DefaultSetESB( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memESB);
static void _DefaultSetAUI( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memAUI);

static int _DefaultSetFrameType( H1ENCODE_API_T *pstEncoder, H1ENCODE_FRAME_TYPE_T frameType );
static int _DefaultSetFrameSize( H1ENCODE_API_T *pstEncoder, u32 width, u32 height, u32 stride);
static int _DefaultSetBitrate( H1ENCODE_API_T *pstEncoder, u32 targetBitrate, BOOLEAN bEnableCBR );
static int _DefaultSetGOPLength( H1ENCODE_API_T *pstEncoder, u32 gopLength );
static int _DefaultSetFrameRate( H1ENCODE_API_T *pstEncoder, u32 frameRate );
static int _DefaultSetQPValue( H1ENCODE_API_T *pstEncoder, u32 qp );

static int _DefaultGetBitrate( H1ENCODE_API_T *pstEncoder, u32 *pBitrate );
static int _DefaultGetGOPLength( H1ENCODE_API_T *pstEncoder, u32 *pGopLength );
static int _DefaultGetFrameRate( H1ENCODE_API_T *pstEncoder, u32 *pFrameRate );
static int _DefaultGetQPValue( H1ENCODE_API_T *pstEncoder, u32 *pQP );

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
#if 0
static const u32 H1ENCODE_VUI_FRAMERATE[][3] = {
	// num_units_in_tick, time_scale, framerate code
	{	 0,	     0, LX_VENC_FRAME_RATE_NONE },	// NONE
	{ 1001,  48000, LX_VENC_FRAME_RATE_23HZ },	// 23.98 Hz				H1ENCODE_FRAMERATECODE_MIN
	{ 1000,  48000, LX_VENC_FRAME_RATE_24HZ },	// 24 Hz
	{ 1000,  50000, LX_VENC_FRAME_RATE_25HZ },	// 25 Hz
	{ 1001,  60000, LX_VENC_FRAME_RATE_29HZ },	// 29.97 Hz
	{ 1000,  60000, LX_VENC_FRAME_RATE_30HZ },	// 30 Hz
	{ 1000, 100000, LX_VENC_FRAME_RATE_50HZ },	// 50 Hz
	{ 1001, 120000, LX_VENC_FRAME_RATE_59HZ },	// 59.94 Hz
	{ 1000, 120000, LX_VENC_FRAME_RATE_60HZ },	// 60 Hz
	{ 1000,  30000, LX_VENC_FRAME_RATE_15HZ },	// 30 Hz (Only for LGE)	H1ENCODE_FRAMERATECODE_MAX
};
#else
static const u32 H1ENCODE_VUI_FRAMERATE[][3] = {
	// num_units_in_tick, time_scale, framerate code
	{	 0,	     0, LX_VENC_FRAME_RATE_NONE },	// NONE
	{ 1001,  24000, LX_VENC_FRAME_RATE_23HZ },	// 23.98 Hz				H1ENCODE_FRAMERATECODE_MIN
	{ 1000,  24000, LX_VENC_FRAME_RATE_24HZ },	// 24 Hz
	{ 1000,  25000, LX_VENC_FRAME_RATE_25HZ },	// 25 Hz
	{ 1001,  30000, LX_VENC_FRAME_RATE_29HZ },	// 29.97 Hz
	{ 1000,  30000, LX_VENC_FRAME_RATE_30HZ },	// 30 Hz
	{ 1000,  50000, LX_VENC_FRAME_RATE_50HZ },	// 50 Hz
	{ 1001,  60000, LX_VENC_FRAME_RATE_59HZ },	// 59.94 Hz
	{ 1000,  60000, LX_VENC_FRAME_RATE_60HZ },	// 60 Hz
	{ 1000,  15000, LX_VENC_FRAME_RATE_15HZ },	// 30 Hz (Only for LGE)	H1ENCODE_FRAMERATECODE_MAX
};
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/

H1ENCODE_API_T *H1EncodeAPISetDefault( H1ENCODE_API_T *pstEncoder )
{
	TRACE_ENTER();

	if ( pstEncoder != NULL )
	{

#if 0
		// allocate encoding functions
		pstEncoder->Open 	= _H264Open;
		pstEncoder->Close 	= _H264Close;
		pstEncoder->Encode 	= _H264Encode;
#endif

		// allocate setter fucntions
		pstEncoder->SetMemOSB = _DefaultSetOSB;
		pstEncoder->SetMemESB = _DefaultSetESB;
		pstEncoder->SetMemAUI = _DefaultSetAUI;

		pstEncoder->SetFrameSize = _DefaultSetFrameSize;
		pstEncoder->SetFrameType = _DefaultSetFrameType;
		pstEncoder->SetGOPLength = _DefaultSetGOPLength;
		pstEncoder->SetBitrate   = _DefaultSetBitrate;
		pstEncoder->SetFrameRate = _DefaultSetFrameRate;
		pstEncoder->SetQPValue   = _DefaultSetQPValue;

		// allocate getter functions
		pstEncoder->GetGOPLength = _DefaultGetGOPLength;
		pstEncoder->GetBitrate   = _DefaultGetBitrate;
		pstEncoder->GetFrameRate = _DefaultGetFrameRate;
		pstEncoder->GetQPValue   = _DefaultGetQPValue;

	}

	return pstEncoder;
}


////////////////////////////////////////////////////////////////////////////////
//
//	Setter functions
//
////////////////////////////////////////////////////////////////////////////////

static void _DefaultSetOSB( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memOSB)
{
	if ( pstEncoder == NULL )
	{
		return;
	}

	pstEncoder->memOSB = memOSB;

	if (pstEncoder->eEncodeType == LX_VENC_ENCODE_TYPE_VP8)
	{
		// VP8 encoding시 내부적으로 output buffer를 2개로 나누어 사용함.
		// 그때, buffer를 연속적으로 관리할 수 없어서 전체 buffer를
		// n개로 나누어 할당하고 그 위치는 index로 관리함.

		u32 align = 0;
		u32 frameSize = 0;
		int i;
#if 0
		int frameCount = 0;

		frameCount = pstEncoder->memOSB.u32Size / H1ENCODE_OUTBUF_MIN;
		frameCount = ((frameCount >> 1) << 1);
		if ( frameCount <= 0 )
		{
			frameCount = 1;
		}

		//printk("internal coded buffer count = %d\n", frameCount);
#else
		int frameCount = 1;
#endif

		pstEncoder->outputFrameCount = frameCount;
		pstEncoder->outputFrameIndex = 0;
		align = MROUND(pstEncoder->memOSB.u32Size, 512);
		if ( align != pstEncoder->memOSB.u32Size )
		{
			align = align - 512;
		}
		frameSize = align / pstEncoder->outputFrameCount;

		for ( i = 0; i < pstEncoder->outputFrameCount; i++ )
		{
			H1ENCODE_MEM_T *mem;
			mem = &pstEncoder->memOutputFrame[i];

			mem->u32Phys = pstEncoder->memOSB.u32Phys + (frameSize * i);
			mem->u32Virt = pstEncoder->memOSB.u32Virt + (frameSize * i);
			mem->u32Size = frameSize;

			//VENC_DEBUG("[H1ENCODE_MEM_T] 0x%08x, 0x%08x, 0x%08x\n", mem->u32Phys, mem->u32Virt, mem->u32Size);
		}
	}
}

static void _DefaultSetESB( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memESB)
{
	if ( pstEncoder == NULL )
	{
		return;
	}

	pstEncoder->memESB = memESB;
}

static void _DefaultSetAUI( H1ENCODE_API_T *pstEncoder, H1ENCODE_MEM_T memAUI)
{
	if ( pstEncoder == NULL )
	{
		return;
	}

	pstEncoder->memAUI = memAUI;
}

static int _DefaultSetFrameType( H1ENCODE_API_T *pstEncoder, H1ENCODE_FRAME_TYPE_T frameType )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder->eFrameType = frameType;

	return RET_OK;
}

static int _DefaultSetFrameSize( H1ENCODE_API_T *pstEncoder, u32 width, u32 height, u32 stride)
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder->width = width;
	pstEncoder->height = height;
	pstEncoder->stride = stride;

	return RET_OK;
}

static int _DefaultSetBitrate( H1ENCODE_API_T *pstEncoder, u32 targetBitrate, BOOLEAN bEnableCBR )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( targetBitrate < H1ENCODE_BITRATE_MIN )
	{
		targetBitrate = H1ENCODE_BITRATE_MIN;
	}
	else if ( targetBitrate > H1ENCODE_BITRATE_MAX )
	{
		targetBitrate = H1ENCODE_BITRATE_MAX;
	}

	pstEncoder->bitrate = targetBitrate;
	pstEncoder->bEnableCBR = bEnableCBR;

	return RET_OK;
}

static int _DefaultSetGOPLength( H1ENCODE_API_T *pstEncoder, u32 gopLength )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder->gopLength = gopLength;

	return RET_OK;
}

static int _DefaultSetFrameRate( H1ENCODE_API_T *pstEncoder, u32 frameRateCode )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( frameRateCode < H1ENCODE_FRAMERATECODE_MIN || frameRateCode > H1ENCODE_FRAMERATECODE_MAX )
	{
		return RET_ERROR;
	}

	pstEncoder->frameRateCode 	= frameRateCode;
	pstEncoder->frameRateDenom 	= H1ENCODE_VUI_FRAMERATE[frameRateCode][0];	// num_units_in_tick
	pstEncoder->frameRateNum	= H1ENCODE_VUI_FRAMERATE[frameRateCode][1];	// time_scale

	return RET_OK;
}

static int _DefaultSetQPValue( H1ENCODE_API_T *pstEncoder, u32 qp )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder->qp = qp;

	return RET_OK;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Getter functions
//
////////////////////////////////////////////////////////////////////////////////

static int _DefaultGetBitrate( H1ENCODE_API_T *pstEncoder, u32 *pBitrate )
{
	if ( pBitrate == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	*pBitrate = pstEncoder->bitrate;

	return RET_OK;
}

static int _DefaultGetGOPLength( H1ENCODE_API_T *pstEncoder, u32 *pGopLength )
{
	if ( pGopLength == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	*pGopLength = pstEncoder->gopLength;

	return RET_OK;
}

static int _DefaultGetFrameRate( H1ENCODE_API_T *pstEncoder, u32 *pFrameRate )
{
	if ( pFrameRate == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	*pFrameRate = pstEncoder->frameRateCode;

	return RET_OK;
}

static int _DefaultGetQPValue( H1ENCODE_API_T *pstEncoder, u32 *pQP )
{
	if ( pQP == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	*pQP = pstEncoder->qp;

	return RET_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
//	Common functions
//
////////////////////////////////////////////////////////////////////////////////

long long _NowMS( void )
{
	struct timeval now;
	long long msec;

	do_gettimeofday(&now);

	msec = (now.tv_sec * 1000 + now.tv_usec / 1000 );

	return msec;
}

long long _NowUS( void )
{
	struct timeval now;
	long long usec;

	do_gettimeofday(&now);

	usec = (now.tv_sec * 1000000 + now.tv_usec );

	return usec;
}

#ifdef H1ENCODE_USE_LOGFILE
static OS_FILE_T _logFile;

int _LOG_Write( const char * format, ... )
{
	int len;
	char buf[128];
	va_list args;

	va_start(args, format);
	len = snprintf(buf, sizeof(buf), format, args);
	va_end(args);

	if ( RET_OK != OS_OpenFile( &_logFile, LOGFILE_NAME, O_CREAT|O_APPEND|O_LARGEFILE, 0666 ) )
	{
		return RET_ERROR;
	}

	len = OS_WriteFile( &_logFile, (char *)buf, len );

	OS_CloseFile( &_logFile );

	return len;
}

int _LOG_WriteTime( struct timeval *pstTime )
{
	int len = 0;
	struct timeval curTime;

	if ( pstTime != NULL )
	{
		curTime = *pstTime;
	}
	else
	{
		do_gettimeofday(&curTime);
	}

#if 0
	len = _LOG_Write( "%lu.%06lu,", curTime.tv_sec, curTime.tv_usec );
#else
	char buf[128];

	len = snprintf(buf, sizeof(buf), "%lu.%06lu,", curTime.tv_sec, curTime.tv_usec );

	if ( RET_OK != OS_OpenFile( &_logFile, LOGFILE_NAME, O_CREAT|O_APPEND|O_LARGEFILE, 0666 ) )
	{
		return RET_ERROR;
	}

	len = OS_WriteFile( &_logFile, (char *)buf, len );

	OS_CloseFile( &_logFile );
#endif

	return len;
}

#else
#define H1ENCODE_LOG( format, args... )
#endif


////////////////////////////////////////////////////////////////////////////////
//
//	Memory Allocator
//
////////////////////////////////////////////////////////////////////////////////

#define H1ENCODE_USE_ENCPARAMS_KMALLOC

#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
#define H1ENCODE_ENCPARAM_LIST_SIZE	16
static H1ENCODE_ENC_PARAMS_T *_EncParamList[H1ENCODE_ENCPARAM_LIST_SIZE];
static BOOLEAN _EncParamStatus[H1ENCODE_ENCPARAM_LIST_SIZE];
#endif

void _EncParamInit( void )
{
#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
	int i;

	for ( i = 0; i < H1ENCODE_ENCPARAM_LIST_SIZE; i++ )
	{
		_EncParamList[i] = (H1ENCODE_ENC_PARAMS_T *)kmalloc( sizeof(H1ENCODE_ENC_PARAMS_T), GFP_KERNEL );
		_EncParamStatus[i] = FALSE;
	}
#else
	// NOP
#endif
}

void _EncParamFinal( void )
{
#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
	int i;

	for ( i = 0; i < H1ENCODE_ENCPARAM_LIST_SIZE; i++ )
	{
		if ( _EncParamList[i] != NULL )
		{
			kfree( _EncParamList[i] );
			_EncParamList[i] = NULL;
			_EncParamStatus[i] = TRUE;
		}
	}
#else
	// NOP
#endif
}

H1ENCODE_ENC_PARAMS_T * _EncParamAlloc( void )
{
#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
	int i;

	for ( i = 0; i < H1ENCODE_ENCPARAM_LIST_SIZE; i++ )
	{
		if ( _EncParamStatus[i] == FALSE )
		{
			_EncParamStatus[i] = TRUE;
			memset( _EncParamList[i], 0x0, sizeof(H1ENCODE_ENC_PARAMS_T) );
			return _EncParamList[i];
		}
	}

	return NULL;
#else
	H1ENCODE_ENC_PARAMS_T * pstEncParams = NULL;
	int retry = 5;

	while ( pstEncParams == NULL && retry > 0 )
	{
		pstEncParams = kmalloc( sizeof(H1ENCODE_ENC_PARAMS_T), GFP_ATOMIC );
		retry--;
	}

#ifdef H1ENCODE_DEBUG_MEM_LEAK
	atomic_inc( &encparams_count );
#endif

	return pstEncParams;
#endif
}

void _EncParamFree( H1ENCODE_ENC_PARAMS_T * pstEncParams )
{
#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
	int i;

	for ( i = 0; i < H1ENCODE_ENCPARAM_LIST_SIZE; i++ )
	{
		if ( _EncParamList[i] == pstEncParams )
		{
			_EncParamStatus[i] = FALSE;
			break;
		}
	}
#else
	if ( pstEncParams != NULL )
	{
		kfree( pstEncParams );

#ifdef H1ENCODE_DEBUG_MEM_LEAK
		atomic_dec( &encparams_count );
#endif

	}
#endif
}

void _EncParamClear( void )
{
#ifndef H1ENCODE_USE_ENCPARAMS_KMALLOC
	int i;

	for ( i = 0; i < H1ENCODE_ENCPARAM_LIST_SIZE; i++ )
	{
		if ( _EncParamList[i] != NULL )
		{
			_EncParamStatus[i] = FALSE;
		}
	}
#else
	// NOP
#endif
}


////////////////////////////////////////////////////////////////////////////////
//
//	WorkQueue
//
////////////////////////////////////////////////////////////////////////////////

static struct workqueue_struct *venc_workqueue;

int _WorkQInit( void )
{
	int ret = RET_OK;
	venc_workqueue = create_singlethread_workqueue("venc_workqueue");

	if ( venc_workqueue == NULL )
	{
		VENC_ERROR("workqueue allocation failed\n");
		ret = RET_ERROR;
	}

	return ret;
}

int _WorkQCleanup( void )
{
	if ( venc_workqueue == NULL )
	{
		VENC_ERROR("workqueue is NULL\n");
		return RET_ERROR;
	}

	destroy_workqueue( venc_workqueue );

	return RET_OK;
}

void _WorkQFlush( void )
{
	flush_workqueue( venc_workqueue );
}

void _WorkQEncodeBH( struct work_struct *work )
{
	H1ENCODE_ENC_PARAMS_T *pstEncParams = (H1ENCODE_ENC_PARAMS_T *) work;
	u32 u32ESBCurrentStart = 0;
	u32 u32ESBStreamSize = 0;
	u32 streamBase;
	u32 streamSize;

	H1ENCODE_MEM_T	*pMemESB = NULL;
	H1ENCODE_MEM_T	*pMemAUI = NULL;

	struct list_head *pStreamList;

#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
	static H1ENCODE_AUI_T	stTopFieldAUI;
#endif

	//TRACE_ENTER();

	if ( pstEncParams == NULL )
	{
		return;
	}

	pMemESB 	= pstEncParams->pMemESB;
	pMemAUI 	= pstEncParams->pMemAUI;
	pStreamList = pstEncParams->pStreamList;

	if ( pMemESB == NULL || pMemAUI == NULL || pStreamList == NULL )
	{
		VENC_ERROR("parameter is null\n");
		return;
	}

	u32ESBCurrentStart = pMemESB->u32WriteOffset;

	streamBase = pMemESB->u32Phys + pMemESB->u32WriteOffset;
	streamSize = _StreamListCopy2Mem( pStreamList, pMemESB );

	if ( pMemESB->u32WriteOffset < u32ESBCurrentStart )
	{
		// wrap-around
		u32ESBStreamSize = pMemESB->u32WriteOffset + ( pMemESB->u32Size - u32ESBCurrentStart);
	}
	else
	{
		u32ESBStreamSize = pMemESB->u32WriteOffset - u32ESBCurrentStart;
	}

	// Create aui data & Notify encode done
	//if ( au_type == H1ENCODE_AU_TYPE_I || au_type == H1ENCODE_AU_TYPE_P )
	{
		H1ENCODE_AUI_T stAUI;
		stAUI.au_type = pstEncParams->au_type;
		stAUI.unit_size = streamSize;
		stAUI.unit_start = streamBase;
#ifdef H1ENCODE_DEBUG_SAVE_DURATION
		stAUI.index		= (pstEncParams->u32FrameIndex & 0xFFFF);	// 16bit
		stAUI.duration 	= pstEncParams->u32EncodingTime;
#else
		stAUI.index 	= pstEncParams->u32FrameIndex;
#endif
		stAUI.timestamp = pstEncParams->u32Timestamp;

#if 0
		VENC_DEBUG("AUI={.type=%d .size=%08x .base=%08x .index=%4d .timestamp=%08x .dur=%4d}\n",
			pstEncParams->au_type, streamSize, streamBase, pstEncParams->u32FrameIndex,
			pstEncParams->u32Timestamp, pstEncParams->u32EncodingTime);
#endif

#ifdef H1ENCODE_DEBUG_AUI_SEQ
		SEQ_LOCK();

		if ( pstEncParams->u32FrameIndex != gNextAUISeq )
		{
			VENC_WARN("H1ENCODE_DEBUG_AUI_SEQ: AUI Sequence is changed.\n");
		}

		gNextAUISeq = pstEncParams->u32FrameIndex + 1;

		SEQ_UNLOCK();
#endif

#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
		if ( pstEncParams->interlaced == 1 && pstEncParams->tb == 0 )
		{
			stTopFieldAUI = stAUI;
		}
		else
		{
			if ( pstEncParams->interlaced == 1 )
			{
				u32 unit_size = stAUI.unit_size;

				stAUI = stTopFieldAUI;
				stAUI.unit_size += unit_size;
			}

			H1EncodeAUIWrite( pMemAUI, stAUI );

			//Should be calling H1EncodeUpdateStatus before calling EncodeNotifyDone.
			H1EncodeUpdateStatus( pstEncParams->id, 1, streamSize, pstEncParams->u32EncodingTime );
			H1EncodeNotifyDone( pstEncParams->id, pMemESB, pMemAUI );
		}
#else
		H1EncodeAUIWrite( pMemAUI, stAUI );

		// Should be calling H1EncodeUpdateStatus before calling EncodeNotifyDone.
		H1EncodeUpdateStatus( pstEncParams->id, 1, streamSize, pstEncParams->u32EncodingTime );
		H1EncodeNotifyDone( pstEncParams->id, pMemESB, pMemAUI );
#endif
		//bNeedAlign = FALSE;
	}

#if 0
	if ( FALSE )
	{
		u32 align = MROUND( pMemESB->u32WriteOffset, 16 );

		if (align - pMemESB->u32WriteOffset)
		{
			u32 paddingSize = align - pMemESB->u32WriteOffset;
			u32 start = pMemESB->u32Virt + pMemESB->u32WriteOffset;

			// clear padding data.
			memset ( (void *)start, 0x0, paddingSize );

			VENC_DEBUG("[ESBuffer][ALIGN] start: 0x%08x, end: 0x%08x, size: 0x%08x\n", start, align, paddingSize);
		}

		// 다음 데이터가 기록될 위치 조정
		pMemESB->u32WriteOffset = align;
	}
#endif

	// Free all related resource
	_StreamListFree( pStreamList );

	// work is pointer of H1ENCODE_ENC_PARAMS_T type.
	// it has been allocated by H1EncodeEncode function.
	// it should be freed by current function.
	_EncParamFree( pstEncParams );

	//TRACE_EXIT();

	return;
}

int _WorkQEnqueue( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams )
{
	int ret = RET_OK;

	//TRACE_ENTER();

	if ( venc_workqueue == NULL )
	{
		VENC_ERROR("workqueue is not created.\n");

		_EncParamFree( pstEncParams );

		ret = RET_ERROR;
	}
	else
	{
		if ( pstEncParams != NULL )
		{
			INIT_WORK( (struct work_struct *)pstEncParams, (void *)&_WorkQEncodeBH);
			ret = queue_work( venc_workqueue, (struct work_struct *)pstEncParams );
		}
	}

	//TRACE_EXIT();

	return ret;

}

