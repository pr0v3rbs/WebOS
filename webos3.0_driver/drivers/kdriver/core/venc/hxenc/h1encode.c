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
#include <linux/slab.h>
#include <linux/delay.h>	// for msleep()

#include "base_types.h"
#include "os_util.h"
#include "venc_kapi.h"
#include "../venc_cfg.h"
#include "../venc_drv.h"

#include "h1encode.h"

#ifdef VENC_HXENC_BUILTIN
#include "hxenc_api.h"
#else
#include "h1encode_wrapper.h"
#endif

#include "hx280enc.h"
#include "memalloc.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

#ifdef H1ENCODE_USE_LOCK
#define H1ENCODE_LOCK(id)			spin_lock(&gpH1Encode[id].lock)
#define H1ENCODE_UNLOCK(id)			spin_unlock(&gpH1Encode[id].lock)
#define H1ENCODE_LOCK_IRQ(id)		spin_lock_irqsave(&gpH1Encode[id].lock, flags)
#define H1ENCODE_UNLOCK_IRQ(id)		spin_unlock_irqrestore(&gpH1Encode[id].lock, flags)
#else
#define H1ENCODE_LOCK(id)
#define H1ENCODE_UNLOCK(id)
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/
H1ENCODE_T 			*gpH1Encode;

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/
#ifdef H1ENCODE_DEBUG_MEM_LEAK
atomic_t encparams_count = ATOMIC_INIT( 0 );
#endif

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static int H1EncodeMemoryInit( int id );
static int H1EncodeMemoryCleanup( int id );
static void H1EncodeReset( H1ENCODE_T *pstEncode );

static int H1EncodeWaitDoneTimeout( int id, int timeout );
static void H1EncodeReadIPC( int id, H1ENCODE_IPC_STATUS_T *pstPrepStatus);
static int H1EncodeStop( int id );

static void _InterruptEnableVsync( int id, BOOLEAN enable );
#ifndef H1ENCODE_USE_NONBLOCKING_MODE
static void _VsyncWait( int id );
#endif
static int _EventWait( H1ENCODE_EVENT_T event, int timeout );
static void _EventPost( H1ENCODE_EVENT_T event );
static void _EventClear( void );

static void H1EncodeVsyncHandler( unsigned long temp );
int H1EncodeISRHandler0 ( void );
int H1EncodeISRHandler1 ( void );

static void H1EncodeDoVsync( int id );

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
static int H1EncodeEncode( int id, H1ENCODE_IPC_STATUS_T *pstPrep, u32 gstc );
static void H1EncodeEncodeDone( unsigned long id );

DECLARE_TASKLET( VENC_TASKLET_H1ENC_0, H1EncodeEncodeDone, 0 );
DECLARE_TASKLET( VENC_TASKLET_H1ENC_1, H1EncodeEncodeDone, 1 );
#else
static int H1EncodeTask( void* pParam );
#endif

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
//static struct task_struct *g_pstH1EncodeTask = NULL;
static H1ENCODE_OUTPUT_STATUS_T	gstOutputStatus;
static OS_EVENT_T	gstEventH1;

/*=============================================================================
	Implementation Group
=============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//
//	Stream Dump
//
////////////////////////////////////////////////////////////////////////////////

#if defined(H1ENCODE_DEBUG_DUMP)

static OS_FILE_T dump_file;
static BOOLEAN	isDumpOpened = FALSE;

static void H1EncodeSaveStream(u32* buf, u32 size)
{
	if ( !isDumpOpened )
	{
		char filepath[100];

		if (gpH1Encode[id].outputType == LX_VENC_ENCODE_TYPE_VP8)
		{
			sprintf(filepath, "h1enc_dump.vp8");
		}
		else
		{
			sprintf(filepath, "h1enc_dump.264");
		}

		if ( RET_OK != OS_OpenFile( &dump_file, filepath, O_CREAT|O_TRUNC|O_RDWR|O_LARGEFILE, 0666 ) )
		{
			return;
		}

		isDumpOpened = TRUE;
	}

#if 0
	if(endian == 1)
	 {
		 u32 i = 0, words = (size + 3) / 4;

		 while(words)
		 {
			 u32 val = strmbuf[i];
			 u32 tmp = 0;

			 tmp |= (val & 0xFF) << 24;
			 tmp |= (val & 0xFF00) << 8;
			 tmp |= (val & 0xFF0000) >> 8;
			 tmp |= (val & 0xFF000000) >> 24;
			 strmbuf[i] = tmp;
			 words--;
			 i++;
		 }

	 }
#endif

	OS_WriteFile( &dump_file, (char *)buf, size );

	VENC_DEBUG("[SAVESTRM] Addr: 0x%08x, Size: 0x%08x\n", (u32)buf, size);

}

static void H1EncodeDumpStop( void )
{
	if ( isDumpOpened )
	{
		OS_CloseFile( &dump_file );
		isDumpOpened = FALSE;
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////
//
//	Common
//
////////////////////////////////////////////////////////////////////////////////

void H1EncodeInitialize(void)
{
	int i = 0;

	TRACE_ENTER();

#ifdef H1ENCODE_ENABLE_SCD
	SET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SCD );
#endif
#ifdef H1ENCODE_ENABLE_SAR
	SET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SAR );
#endif
#ifdef H1ENCODE_ENABLE_FIELD_REPEAT
	SET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_FIELD_REPEAT);
#endif

	// Init core instance
	if ( gpstVencConfig->num_device <= 0 || gpstVencConfig->num_device > VENC_MAX_DEVICE )
	{
		VENC_ERROR("num_device is not vaild. (%d)\n", gpstVencConfig->num_device);
		return;
	}

	gpH1Encode = kzalloc( sizeof(H1ENCODE_T)*gpstVencConfig->num_device, GFP_KERNEL );
	if ( gpH1Encode == NULL )
	{
		VENC_ERROR("Can't allocate memory for gpH1Encode. (%d)\n", gpstVencConfig->num_device);
		return;
	}

	_EncParamInit();

	gpH1Encode[0].ISRHandler = H1EncodeISRHandler0;

	if ( gpstVencConfig->num_device > 1 )
	{
		gpH1Encode[1].ISRHandler = H1EncodeISRHandler1;
	}

	for ( i = 0; i < gpstVencConfig->num_device; i++ )
	{
		int ret;
		gpH1Encode[i].id = i;

		// Disable Interrupt
		VENC_REG_Init( i, gpstVencConfig->core[i] );
		VENC_REG_InterruptEnable( i, FALSE );
		_InterruptEnableVsync( i, FALSE );
		VENC_REG_Cleanup( i );

		H1EncodeReset( &gpH1Encode[i] );

#ifdef H1ENCODE_USE_LOCK
		spin_lock_init(&gpH1Encode[i].lock);
#endif

		// Init global variables
		gpH1Encode[i].stInput.gopLength = H1ENCODE_GOPLENGTH_DEFAULT;
		gpH1Encode[i].stInput.qpValue = -1;

		// Initialize for sync
		init_waitqueue_head( &gpH1Encode[i].wqEncodeVsync );

		// Initialize waitq for encoding done
		init_waitqueue_head( &gpH1Encode[i].wqEncodeDone );

#if 1
		ret = request_irq(gpstVencConfig->core[i].irq_num,
							(irq_handler_t)gpH1Encode[i].ISRHandler,
							IRQF_SHARED,
							"DTVSoC2VENC",
							(void *)gpstVencConfig);
		if ( ret != 0 )
		{
			VENC_DEBUG("request_irq in %s is failed\n", __FUNCTION__);
			return;
		}

		//VENC_ERROR("irq_num=%d\n", gpstVencConfig->core[i].irq_num );
#endif

#ifndef H1ENCODE_USE_NONBLOCKING_MODE
		char sTaskName[20];
		sprintf(sTaskName, "VENC_TASK_#%d", i);

		// Create kthread for encode task
		gpH1Encode[i].taskEncode = kthread_run( &H1EncodeTask, (void *)&gpH1Encode[i], sTaskName );

#ifdef H1ENCODE_ENABLE_TASK_PRIORITY_HIGH
		{
			// set priority of kthread
			struct sched_param param = { 0 };
			param.sched_priority = H1ENCODE_SCHED_PRIORITY;
			if ( sched_setscheduler( gpH1Encode[i].taskEncode, SCHED_RR, &param ) != 0 )
			{
				VENC_ERROR("[%s:%u] Fail set priority of kthread\n", __F__, __L__);
			}
		}
#endif
#endif

	}

	OS_InitEvent( &gstEventH1 );

	_WorkQInit();

	TRACE_EXIT();
}

void H1EncodeFinalize(void)
{
	int i = 0;

	TRACE_ENTER();

	for ( i = 0; i < gpstVencConfig->num_device; i++ )
	{
		free_irq( gpstVencConfig->core[i].irq_num, (void *)gpstVencConfig);

#ifndef H1ENCODE_USE_NONBLOCKING_MODE
		if ( gpH1Encode[i].taskEncode != NULL )
		{
			kthread_stop( gpH1Encode[i].taskEncode );
		}
#endif
	}

	_EncParamFinal();
	_WorkQCleanup();

	if ( gpH1Encode != NULL )
	{
		kfree( gpH1Encode );
	}

	TRACE_EXIT();
}

int H1EncodeOpen( int id )
{
	hx280enc_open( id );

	H1EncodeMemoryInit( id );

	VENC_REG_Init( id, gpstVencConfig->core[id] );

	_EncParamInit();

	VENC_REG_InterruptEnable( id, TRUE );
	_InterruptEnableVsync( id, FALSE );

	return RET_OK;
}

int H1EncodeClose( int id )
{
	if ( gpH1Encode[id].eStatus == H1ENCODE_STATUS_TYPE_START )
	{
		_InterruptEnableVsync( id, FALSE );

		H1EncodeStop(id);

		H1ENCODE_LOCK(id);
		gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_STOP;
		H1ENCODE_UNLOCK(id);
	}

	VENC_REG_InterruptEnable( id, FALSE );

	_EncParamFinal();
	VENC_REG_Cleanup( id );

	hx280enc_close( id );
	H1EncodeMemoryCleanup( id );

	return RET_OK;
}

static int H1EncodeMemoryInit( int id )
{
	LX_VENC_MEM_USAGE_T *mem_usage = NULL;

	if ( id > gpstVencConfig->num_device )
	{
		return RET_ERROR;
	}

	mem_usage = gpstVencConfig->core[id].mem_usage;

	UINT32 size = gpstVencMemConfig->uiVencBufSize / gpstVencConfig->num_device;
	UINT32 base = gpstVencMemConfig->uiVencBufBase + (size * id);

	//MemallocOpen( id, gpstVencMemConfig->uiVencBufBase, gpstVencMemConfig->uiVencBufSize );
	MemallocOpen(id, base, size);

	// Set CPB address & ioremap
	MemallocGetBuffer(id, mem_usage->cpb.size, &gpH1Encode[id].memOSB.u32Phys, mem_usage->cpb.name);
	gpH1Encode[id].memOSB.u32Size = mem_usage->cpb.size;
	gpH1Encode[id].memOSB.u32Virt = (u32)ioremap(gpH1Encode[id].memOSB.u32Phys, gpH1Encode[id].memOSB.u32Size);

	if ( (void*)gpH1Encode[id].memOSB.u32Virt == NULL )
	{
		VENC_ERROR("Error ioremap: Output frame buffer.\n");
		goto RETURN_ERROR;
	}

	// Set ES Buffer address & ioremap
	MemallocGetBuffer( id, mem_usage->esb.size, &gpH1Encode[id].memESB.u32Phys, mem_usage->esb.name );
	gpH1Encode[id].memESB.u32Size = mem_usage->esb.size;
	gpH1Encode[id].memESB.u32Virt =(u32)ioremap( gpH1Encode[id].memESB.u32Phys, gpH1Encode[id].memESB.u32Size );

	// Clear ES Buffer
	memset( (void *)gpH1Encode[id].memESB.u32Virt, 0x0, gpH1Encode[id].memESB.u32Size );

	if ( (void*)gpH1Encode[id].memESB.u32Virt == NULL )
	{
		VENC_ERROR("Error ioremap: ES buffer.\n");
		goto RETURN_ERROR;
	}

	// Set AUI Buffer address & ioremap
	MemallocGetBuffer( id, mem_usage->auib.size, &gpH1Encode[id].memAUI.u32Phys, mem_usage->auib.name );
	gpH1Encode[id].memAUI.u32Size = mem_usage->auib.size;
	gpH1Encode[id].memAUI.u32Virt =(u32)ioremap( gpH1Encode[id].memAUI.u32Phys, gpH1Encode[id].memAUI.u32Size );

	// Clear AUI Buffer
	memset( (void *)gpH1Encode[id].memAUI.u32Virt, 0x0, gpH1Encode[id].memAUI.u32Size );

	if ( (void*)gpH1Encode[id].memAUI.u32Virt == NULL )
	{
		VENC_ERROR("Error ioremap: AUI buffer.\n");
		goto RETURN_ERROR;
	}

	// Set Input frame image buffer for thumbnail
	if ( mem_usage->thumb.size > 0 )
	{
		MemallocGetBuffer( id, mem_usage->thumb.size, &gpH1Encode[id].memTFB.u32Phys, mem_usage->thumb.name );
		gpH1Encode[id].memTFB.u32Size = mem_usage->thumb.size;
		gpH1Encode[id].memTFB.u32Virt = (u32)ioremap( gpH1Encode[id].memTFB.u32Phys, gpH1Encode[id].memTFB.u32Size );

		if ( (void*)gpH1Encode[id].memTFB.u32Virt == NULL )
		{
			VENC_ERROR("Error ioremap: Input frame buffer.\n");
			goto RETURN_ERROR;
		}
	}

	return RET_OK;

RETURN_ERROR:
	H1EncodeMemoryCleanup( id );
	return RET_ERROR;

}

static int H1EncodeMemoryCleanup( int id )
{
	if ( gpH1Encode[id].memESB.u32Virt )
	{
		iounmap( (void *)gpH1Encode[id].memESB.u32Virt );
		MemallocFreeBuffer( id, gpH1Encode[id].memESB.u32Phys );
		gpH1Encode[id].memESB.u32Virt = 0;
	}

	if ( gpH1Encode[id].memAUI.u32Virt )
	{
		iounmap( (void *)gpH1Encode[id].memAUI.u32Virt );
		MemallocFreeBuffer( id, gpH1Encode[id].memAUI.u32Phys );
		gpH1Encode[id].memAUI.u32Virt = 0;
	}

	if ( gpH1Encode[id].memOSB.u32Virt )
	{
		iounmap( (void *)gpH1Encode[id].memOSB.u32Virt );
		MemallocFreeBuffer( id, gpH1Encode[id].memOSB.u32Phys );
		gpH1Encode[id].memOSB.u32Virt = 0;
	}

	if ( gpH1Encode[id].memTFB.u32Virt )
	{
		iounmap( (void *)gpH1Encode[id].memTFB.u32Virt );
		MemallocFreeBuffer( id, gpH1Encode[id].memTFB.u32Phys );
		gpH1Encode[id].memTFB.u32Virt = 0;
	}

	MemallocClose( id );

	return RET_OK;
}

int H1EncodeGetBufferInfo( int id, LX_VENC_BUFFER_T *pstParams )
{
	LX_MEMCFG_T* pMemCfg = NULL;
	UINT32 idOffset = 0;

	if ( pstParams == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	pMemCfg = (LX_MEMCFG_T *)gpstVencMemConfig;

	pstParams->ui32ESBufPhysBase= gpH1Encode[id].memESB.u32Phys;
	pstParams->ui32ESBufSize 	= gpH1Encode[id].memESB.u32Size;
	pstParams->ui32ESBufOffset 	= gpH1Encode[id].memESB.u32Phys - pMemCfg->base;

	pstParams->ui32AUIBufPhysBase = gpH1Encode[id].memAUI.u32Phys;
	pstParams->ui32AUIBufSize 	= gpH1Encode[id].memAUI.u32Size;
	pstParams->ui32AUIBufOffset = gpH1Encode[id].memAUI.u32Phys - pMemCfg->base;

	idOffset = id * gpstVencMemConfig->uiInBufSize / VENC_MAX_DEVICE;
	pstParams->ui32InBufPhysBase= gpstVencMemConfig->uiInBufBase + idOffset;
	pstParams->ui32InBufSize 	= gpstVencMemConfig->uiInBufSize / VENC_MAX_DEVICE;
	pstParams->ui32InBufOffset 	= gpstVencMemConfig->uiInBufBase - pMemCfg->base;

	return RET_OK;
}

// Reset internal variables
static void H1EncodeReset( H1ENCODE_T *pstH1Encode )
{
	if ( pstH1Encode == NULL )
	{
		return;
	}

//	pstH1Encode->u32OutBufferRd = 0;
//	pstH1Encode->u32OutBufferWr = 0;

	pstH1Encode->memESB.u32ReadOffset = 0;
	pstH1Encode->memESB.u32WriteOffset = 0;

	pstH1Encode->memAUI.u32ReadOffset = 0;
	pstH1Encode->memAUI.u32WriteOffset = 0;
	pstH1Encode->u32AUIIndex = 0;

	pstH1Encode->u32AUIStreamSize = 0;

	pstH1Encode->u32EncodedFrames = 0;

	memset( pstH1Encode->arEncodeTimes, 0x0, sizeof(pstH1Encode->arEncodeTimes) );
	pstH1Encode->u32EncodedTime = 0;
	pstH1Encode->timeIndex = 0;

	memset( pstH1Encode->arStreamSizes, 0x0, sizeof(pstH1Encode->arStreamSizes) );
	pstH1Encode->u32EncodedBitrate = 0;
	pstH1Encode->sizeIndex = 0;

	pstH1Encode->ui32CountFrameSkip = 0;
	pstH1Encode->ui32CountFrameError = 0;
	pstH1Encode->ui32CountFrameDelay = 0;

#ifdef H1ENCODE_CHECK_INTERLACED_INPUT
	#ifdef H1ENCODE_ENABLE_INTERLACED_BF
	pstH1Encode->prevFrameType = LX_VENC_FRAME_TYPE_TOP;
	#else
	pstH1Encode->prevFrameType = LX_VENC_FRAME_TYPE_BOTTOM;
	#endif

	if ( pstH1Encode->pstEncoder != NULL )
	{
		pstH1Encode->pstEncoder->bIsFirstFrame = TRUE;
	}
#endif
}


void H1EncodeSetEncodeType( int id, LX_VENC_ENCODE_TYPE_T eEncodeType )
{
	if ( eEncodeType == LX_VENC_ENCODE_TYPE_H264 || eEncodeType == LX_VENC_ENCODE_TYPE_VP8 )
	{
		gpH1Encode[id].outputType = eEncodeType;

		VENC_TRACE("Encode Type: %s\n", eEncodeType == LX_VENC_ENCODE_TYPE_H264 ? "H264" : "VP8");
	}
	else
	{
		VENC_ERROR("Wrong encode type.\n");
	}
}

int H1EncodeGetOutputBuffer( int id, LX_VENC_RECORD_OUTPUT_T *pstEncOutput )
{
	// Wait 100ms
	if ( H1EncodeWaitDoneTimeout( id, 100 ) != RET_OK )
	{
		return RET_ERROR;
	}

//	LOCK();

	pstEncOutput->ui32OffsetStart = gstOutputStatus.ui32ESRptr - gpH1Encode[id].memESB.u32Phys;
	pstEncOutput->ui32OffsetEnd	 = gstOutputStatus.ui32ESWptr - gpH1Encode[id].memESB.u32Phys;
	gstOutputStatus.ui32ESRptr = gstOutputStatus.ui32ESWptr;

	pstEncOutput->ui32AUIOffsetStart = gstOutputStatus.ui32AUIRptr - gpH1Encode[id].memAUI.u32Phys;
	pstEncOutput->ui32AUIOffsetEnd	 = gstOutputStatus.ui32AUIWptr - gpH1Encode[id].memAUI.u32Phys;
	gstOutputStatus.ui32AUIRptr = gstOutputStatus.ui32AUIWptr;

//	UNLOCK();

	return RET_OK;
}

u32 H1EncodeGetEncodedFrameCount( int id )
{
	return gpH1Encode[id].u32EncodedFrames;
}

u32 H1EncodeGetEncodeMsec( int id )
{
	if ( gpH1Encode[id].u32EncodedTime > 0 )
	{
		return gpH1Encode[id].u32EncodedTime / 1000;
	}
	else
	{
		return 0;
	}
}

s32	H1EncodeSetResolution( int id, u16 width, u16 height )
{
	if ( gpH1Encode[id].pstEncoder != NULL )
	{
		VENC_WARN("Should be called this function before starting.\n");
		return RET_ERROR;
	}

	if ( width == 0 || height == 0 )
	{
		return RET_ERROR;
	}

	if ( width > 1920 || height > 1088 )
	{
		// Not suppored resolution size.
		return RET_ERROR;
	}

	gpH1Encode[id].stInput.width = width;
	gpH1Encode[id].stInput.height = height;

	return RET_OK;
}

s32 H1EncodeGetResolution( int id, u16 *pWidth, u16 *pHeight )
{
	if ( pWidth == NULL || pHeight == NULL )
	{
		return RET_ERROR;
	}

	if( gpH1Encode[id].pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	*pWidth = gpH1Encode[id].pstEncoder->width;
	*pHeight = gpH1Encode[id].pstEncoder->height;

	return RET_OK;
}

s32 H1EncodeSetFramerate( int id, u32 framerate )
{
	u32		frameRateCode;
	u32 	frameRateDenom;
	u32		frameRateNum;

	switch( framerate )
	{
		case 24:
			frameRateCode = LX_VENC_FRAME_RATE_24HZ;
			frameRateDenom = 1000;
			frameRateNum = 24000;
			break;

		case 25:
			frameRateCode = LX_VENC_FRAME_RATE_25HZ;
			frameRateDenom = 1000;
			frameRateNum = 25000;
			break;

		case 30:
			frameRateCode = LX_VENC_FRAME_RATE_30HZ;
			frameRateDenom = 1000;
			frameRateNum = 30000;
			break;

		case 50:
			frameRateCode = LX_VENC_FRAME_RATE_50HZ;
			frameRateDenom = 1000;
			frameRateNum = 50000;
			break;

		case 60:
			frameRateCode = LX_VENC_FRAME_RATE_60HZ;
			frameRateDenom = 1000;
			frameRateNum = 60000;
			break;

		default:
			frameRateCode = LX_VENC_FRAME_RATE_NONE;
			frameRateDenom = 0;
			frameRateNum = 0;
			break;
	}

	gpH1Encode[id].stInput.frameRateCode 	= frameRateCode;
	gpH1Encode[id].stInput.frameRateDenom 	= frameRateDenom;
	gpH1Encode[id].stInput.frameRateNum		= frameRateNum;

	return RET_OK;
}

s32 H1EncodeGetFramerate(	int id,
							LX_VENC_RECORD_FRAME_RATE_T *peFramerate,
							UINT32	*pFramerate,
							LX_VENC_SOURCE_TYPE_T	*peSourceType )
{
	if ( gpH1Encode[id].pstEncoder == NULL)
	{
		return RET_ERROR;
	}

	if ( peFramerate == NULL ||
		pFramerate == NULL ||
		peSourceType == NULL )
	{
		return RET_ERROR;
	}

	if ( gpH1Encode[id].pstEncoder->GetFrameRate != NULL )
	{
		LX_VENC_RECORD_FRAME_RATE_T eFramerate = LX_VENC_FRAME_RATE_NONE;
		LX_VENC_SOURCE_TYPE_T eSourceType = LX_VENC_SOURCE_TYPE_FIELD;
		UINT32	nFramerate = 0;

		gpH1Encode[id].pstEncoder->GetFrameRate( gpH1Encode[id].pstEncoder, &eFramerate );

		switch( eFramerate )
		{
			case LX_VENC_FRAME_RATE_24HZ:	nFramerate = 24; break;
			case LX_VENC_FRAME_RATE_25HZ:	nFramerate = 25; break;
			case LX_VENC_FRAME_RATE_29HZ:	nFramerate = 29; break;
			case LX_VENC_FRAME_RATE_30HZ:	nFramerate = 30; break;
			case LX_VENC_FRAME_RATE_50HZ:	nFramerate = 50; break;
			case LX_VENC_FRAME_RATE_59HZ:	nFramerate = 59; break;
			case LX_VENC_FRAME_RATE_60HZ:	nFramerate = 60; break;
			case LX_VENC_FRAME_RATE_15HZ:	nFramerate = 15; break;
			default :						nFramerate =  0; break;
		}

		if ( gpH1Encode[id].pstEncoder->eFrameType == LX_VENC_FRAME_TYPE_PROGRESSIVE )
		{
			eSourceType = LX_VENC_SOURCE_TYPE_FRAME;
		}
		else
		{
			eSourceType = LX_VENC_SOURCE_TYPE_FIELD;
		}

		*peFramerate = eFramerate;
		*pFramerate = nFramerate;
		*peSourceType = eSourceType;

		//printk("nFramerate=%d, eSourceType=%d\n", nFramerate, eSourceType);
		return RET_OK;
	}
	else
	{
		return RET_ERROR;
	}
}

s32 H1EncodeSetBitrate( int id, u32 targetBitrate, BOOLEAN bEnableCBR )
{
	//printk("%s :: targetBitrate=%d, bEnableCBR=%d\n", __F__, targetBitrate, bEnableCBR);

	if ( gpH1Encode[id].pstEncoder == NULL || gpH1Encode[id].pstEncoder->SetBitrate == NULL )
	{
		gpH1Encode[id].stInput.targetBitrate = targetBitrate;
		gpH1Encode[id].stInput.bEnableCBR = bEnableCBR;
	}
	else if ( gpH1Encode[id].pstEncoder->SetBitrate != NULL )
	{
		gpH1Encode[id].pstEncoder->SetBitrate( gpH1Encode[id].pstEncoder, targetBitrate, bEnableCBR );
	}
	else
	{
		return RET_NOT_SUPPORTED;
	}

	return RET_OK;
}

u32 H1EncodeGetBitrate( int id )
{
	if ( gpH1Encode[id].u32EncodedBitrate > 0 )
	{
		return gpH1Encode[id].u32EncodedBitrate;
	}
	else
	{
		return 0;
	}
}

s32 H1EncodeSetGOP( int id, u32 gopLength )
{
	int ret = RET_ERROR;

	if ( gopLength < 1 || gopLength > 300 )
	{
		// Invalid parameter
		return RET_ERROR;
	}

	if ( gpH1Encode[id].pstEncoder == NULL || gpH1Encode[id].pstEncoder->SetGOPLength == NULL)
	{
		gpH1Encode[id].stInput.gopLength = gopLength;

		return RET_OK;
	}
	else if ( gpH1Encode[id].pstEncoder->SetGOPLength != NULL )
	{
		gpH1Encode[id].stInput.gopLength = gopLength;
		ret = gpH1Encode[id].pstEncoder->SetGOPLength( gpH1Encode[id].pstEncoder, gopLength );
	}
	else
	{
		return RET_NOT_SUPPORTED;
	}

	return ret;
}

u32 H1EncodeGetGOP( int id )
{
	u32 gopLength = H1ENCODE_GOPLENGTH_DEFAULT;

	if ( gpH1Encode[id].pstEncoder != NULL &&
		gpH1Encode[id].pstEncoder->GetGOPLength != NULL )
	{
		gpH1Encode[id].pstEncoder->GetGOPLength( gpH1Encode[id].pstEncoder, &gopLength );
	}

	return gopLength;
}

s32 H1EncodeSetQPValue( int id, u32 qp )
{
	int ret = RET_ERROR;

	if ( gpH1Encode[id].pstEncoder == NULL )
	{
		gpH1Encode[id].stInput.qpValue = qp;
		return RET_OK;
	}

	if ( gpH1Encode[id].pstEncoder->SetQPValue != NULL )
	{
		ret = gpH1Encode[id].pstEncoder->SetQPValue( gpH1Encode[id].pstEncoder, qp );
	}
	else
	{
		return RET_NOT_SUPPORTED;
	}

	return ret;
}

u32 H1EncodeGetQPValue( int id )
{
	u32 qp = 0;

	if ( gpH1Encode[id].pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	if ( gpH1Encode[id].pstEncoder->GetQPValue != NULL )
	{
		gpH1Encode[id].pstEncoder->GetQPValue( gpH1Encode[id].pstEncoder, &qp );
	}
	else
	{
		return RET_NOT_SUPPORTED;
	}

	return qp;
}

s32 H1EncodeSetSAR( int id, u32 sarWidth, u32 sarHeight )
{
	if ( gpH1Encode[id].pstEncoder == NULL )
	{
		gpH1Encode[id].stInput.sarWidth = sarWidth;
		gpH1Encode[id].stInput.sarHeight = sarHeight;

		return RET_OK;
	}
	else
	{
		return RET_NOT_SUPPORTED;
	}
}

s32 H1EncodeGetSAR( int id, u32 *pSarWidth, u32 *pSarHeight )
{
	if ( pSarWidth == NULL || pSarHeight == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	if ( gpH1Encode[id].pstEncoder == NULL )
	{
		return RET_ERROR;
	}
	else
	{
		*pSarWidth = gpH1Encode[id].stInput.sarWidth;
		*pSarHeight= gpH1Encode[id].stInput.sarHeight;

		return RET_OK;
	}
}

s32 H1EncodeSetInputSource( int id, LX_VENC_INPUT_SRC_T eInputSource )
{
	gpH1Encode[id].eInputSource = eInputSource;

	return RET_OK;
}

void H1EncodeSetTime( int id, u32 clock )
{
	VENC_REG_SetTime( id, clock );
}

u32 H1EncodeGetTime( int id )
{
	u32 time;
	time = VENC_REG_GetTime( id );

#if 0
	time = time / ( H1ENCODE_HW_CLOCK / 1000000 );	// microsec (㎲)
#else
	time = time / gpstVencConfig->core[id].hw_clock_mhz;	// microsec (㎲)
#endif
	//VENC_DEBUG("encoding time: %d\n", time);

	return time;	// return microsec
}

#if 0
static void H1EncodeCheckTime( H1ENCODE_T *pstEncode, u32 time )
{
	int i;
	int checkFrameCount = 10;
	u32 validValue = 0;
	u32 totalTime = 0;
	u32 avgTime = 0;
	u32 minTime = ~0;
	u32 maxTime = 0;

#if 0
	if ( checkFrameCount > H1ENCODE_CHECK_FRAME_MAX )
	{
		checkFrameCount = H1ENCODE_CHECK_FRAME_MAX;
	}
#endif

	pstEncode->arEncodeTimes[pstEncode->timeIndex] = time;
	pstEncode->timeIndex++;

	if ( pstEncode->timeIndex >= checkFrameCount )
	{
		u32 milliSec, microSec;

		for ( i = 0; i < checkFrameCount; i++ )
		{
			if (pstEncode->arEncodeTimes[i] > 0)
			{
				validValue++;
				totalTime += pstEncode->arEncodeTimes[i];

				minTime = MIN(minTime, pstEncode->arEncodeTimes[i]);
				maxTime = MAX(maxTime, pstEncode->arEncodeTimes[i]);
			}
		}

		if ( validValue > 0 )
		{
			avgTime = totalTime / validValue;

			milliSec = avgTime / 1000;
			microSec = avgTime % 1000;

			VENC_DEBUG("Encoding Time: Last %d Frames AVG[%u.%03u ms] MIN[%u ms] MAX[%u ms]\n",
				checkFrameCount, milliSec, microSec, (minTime / 1000), (maxTime / 1000));
		}
		else
		{
			VENC_DEBUG("Encoding Time: No valid values\n");
		}

		pstEncode->timeIndex = 0;
	}
}

static void H1EncodeCheckBitrate( H1ENCODE_T *pstEncode, u32 streamSize )
{
	int i;
	int checkFrameCount = H1ENCODE_CHECK_FRAME_MAX;
	u32 validValue = 0;
	u32 totalStreamSize = 0;
	u32 bitrate;

	if ( pstEncode->pstEncoder == NULL )
	{
		return;
	}

	pstEncode->arStreamSizes[pstEncode->sizeIndex] = streamSize;

	// Calculate the sum of stream sizes
	for ( i = 0; i < checkFrameCount; i++ )
	{
		if (pstEncode->arStreamSizes[i] > 0)
		{
			validValue++;
			totalStreamSize += pstEncode->arStreamSizes[i];
		}
	}

	// Calculate the bitrate at latest n frames
	if ( validValue > 0 && pstEncode->pstEncoder->frameRateDenom > 0 )
	{
		u32 tmp = totalStreamSize / validValue;
		tmp *= (u32) pstEncode->pstEncoder->frameRateNum;
		bitrate = (u32) (8 * (tmp / (u32) pstEncode->pstEncoder->frameRateDenom));

		pstEncode->u32EncodedBitrate = bitrate;

		VENC_DEBUG("Bitrate: %d\n", bitrate);
	}

	// Set the next index
	pstEncode->sizeIndex++;

	if ( pstEncode->sizeIndex >= checkFrameCount )
	{
		pstEncode->sizeIndex = 0;
	}

}
#endif

void H1EncodeUpdateStatus( int id, u32 frameCount, u32 streamSize, u32 encodingTime )
{
	if ( gpstVencConfig->num_device < id )
	{
		VENC_ERROR("Wrong id[%d]\n");
		return;
	}

	gpH1Encode[id].u32EncodedFrames += frameCount;
	gpH1Encode[id].u32EncodedTime = encodingTime;

	//H1EncodeCheckTime( &gpH1Encode[id]. encodingTime );
	//H1EncodeCheckBitrate( &gpH1Encode[id]. streamSize );

	return;
}

int H1EncodeReadStatus( char * buffer )
{
	int i = 0;
	int len = 0;
	const static char *COMMAND_STATUS[4] = {
		"STOP",
		"STOP_PREPARE",
		"START",
		"START_PREPARE"
	};

	if ( buffer == NULL )
	{
		return 0;
	}

	for ( i = 0; i < gpstVencConfig->num_device; i++ )
	{
		int id = i;

		H1ENCODE_LOCK(id);

		H1ENCODE_API_T *pstEncoder = gpH1Encode[id].pstEncoder;

		len += sprintf( buffer + len, "#### VENC Core#%d Status ####\n", i );

		if ( gpH1Encode[id].eStatus < 4 )
		{
			len += sprintf( buffer + len, "Command [%s]\n", COMMAND_STATUS[gpH1Encode[id].eStatus] );
		}

		if ( gpH1Encode[id].eStatus == H1ENCODE_STATUS_TYPE_START && pstEncoder != NULL )
		{
			len += sprintf( buffer + len, "Input Source Type: %s\n", (gpH1Encode[id].eInputSource == LX_VENC_INPUT_SRC_DTV) ? "DTV":"OTHERS" );
			len += sprintf( buffer + len, "Output Format Type: %s\n", (gpH1Encode[id].outputType == LX_VENC_ENCODE_TYPE_H264) ? "H.264":"VP8");
			len += sprintf( buffer + len, "Target Bitrate: %d bits/sec\n", gpH1Encode[id].stInput.targetBitrate ? gpH1Encode[id].stInput.targetBitrate : pstEncoder->bitrate );
			len += sprintf( buffer + len, "Resolution: %d x %d, Framerate: %d/%d\n", pstEncoder->width, pstEncoder->height, pstEncoder->frameRateDenom, pstEncoder->frameRateNum);
			len += sprintf( buffer + len, "GOP Length: %d\n", pstEncoder->gopLength);

		#ifdef H1ENCODE_ENABLE_SAR
			len += sprintf( buffer + len, "SAR: %d:%d\n", pstEncoder->decWidth, pstEncoder->decHeight );
		#endif
		}
		len += sprintf( buffer + len, "\n");

		len += sprintf( buffer + len, "Buffer Status:\n");
		if ( pstEncoder != NULL )
		{
			len += sprintf( buffer + len, "     ES Buffer: 0x%08X ~ 0x%08X [0x%08X]\n", gpH1Encode[id].memESB.u32Phys, gpH1Encode[id].memESB.u32Phys + gpH1Encode[id].memESB.u32Size, pstEncoder->memESB.u32WriteOffset);
			len += sprintf( buffer + len, "    AUI Buffer: 0x%08X ~ 0x%08X [0x%08X]\n", gpH1Encode[id].memAUI.u32Phys, gpH1Encode[id].memAUI.u32Phys + gpH1Encode[id].memAUI.u32Size, pstEncoder->memAUI.u32WriteOffset);
		}
		else
		{
			len += sprintf( buffer + len, "     ES Buffer: 0x%08X ~ 0x%08X\n", gpH1Encode[id].memESB.u32Phys, gpH1Encode[id].memESB.u32Phys + gpH1Encode[id].memESB.u32Size);
			len += sprintf( buffer + len, "    AUI Buffer: 0x%08X ~ 0x%08X\n", gpH1Encode[id].memAUI.u32Phys, gpH1Encode[id].memAUI.u32Phys + gpH1Encode[id].memAUI.u32Size);
		}

		len += sprintf( buffer + len, "Vsync Status: Total[%d], Skip[%d], Error[%d], Delay[%d]\n", gpH1Encode[id].ui32TotalVsync, gpH1Encode[id].ui32CountFrameSkip, gpH1Encode[id].ui32CountFrameError, gpH1Encode[id].ui32CountFrameDelay);
		len += sprintf( buffer + len, "\n");

		H1ENCODE_UNLOCK(id);
	}

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
	len += sprintf( buffer + len, "   Thread mode: Non-thread mode\n");
#else
	len += sprintf( buffer + len, "   Thread mode: Thread mode\n");
#endif

	//len += MemallocReadStatus( buffer + len );

#ifdef H1ENCODE_DEBUG_MEM_LEAK
	extern atomic_t aui_count;
	extern atomic_t api_count;

	len += sprintf( buffer + len, "DEBUG Check MEM Leak:\n");
	len += sprintf( buffer + len, "    ENC PARAM COUNT: %d\n", encparams_count.counter);
	len += sprintf( buffer + len, "          API COUNT: %d\n", api_count.counter);
	len += sprintf( buffer + len, "          AUI COUNT: %d\n", aui_count.counter);
	len += sprintf( buffer + len, "\n");
#endif

	len += HXENC_ReadVersion( buffer + len );

	return len;
}

static void H1EncodeUpdateTimestamp( int id )
{
#ifdef H1ENCODE_USE_VDEC_PTS
	if ( gpH1Encode[id].eInputSource == LX_VENC_INPUT_SRC_DTV )
	{
		//gpH1Encode[id].u32CurrentPTS = VENC_REG_GetSTC( id, LX_VENC_TIMESTAMP_PTS ) & 0x0FFFFFFF;	// use 28bits
		gpH1Encode[id].u32CurrentPTS = VENC_REG_GetSTC( id, LX_VENC_TIMESTAMP_PTS ) & 0x7FFFFFFF;	// use 31bits(2013.06.25)
	}
	else
#endif
	{
		gpH1Encode[id].u32CurrentPTS = VENC_REG_GetSTC( id, LX_VENC_TIMESTAMP_GSTC );
	}
}

u32 H1EncodeGetTimestamp( int id )
{
	return gpH1Encode[id].u32CurrentPTS;
}

int H1EncodeCheckIPCRegister( int id )
{
	H1ENCODE_IPC_STATUS_T stPrepStatus;

	//VENC_REG_DebugPrintRegs( id );

	H1EncodeReadIPC( id, &stPrepStatus);

	if ( stPrepStatus.decWidth == 0 && stPrepStatus.decHeight == 0 )
	{
		return RET_ERROR;
	}

	if ( stPrepStatus.decWidth > 1920 && stPrepStatus.decHeight > 1088 )
	{
		// Can't support frame size.
		return RET_ERROR;
	}

#if 0
	if ( (gpH1Encode[id].stInput.width != stPrepStatus.decWidth) ||
		(gpH1Encode[id].stInput.height != stPrepStatus.decHeight) )
	{
		VENC_WARN("Different between setting value and ipc register.\n");
	}
#endif

	return RET_OK;
}

#ifdef H1ENCODE_ENABLE_SAR
static BOOLEAN H1EncodeDiffPREP_SAR( H1ENCODE_IPC_STATUS_T *pstPrepA, H1ENCODE_IPC_STATUS_T *pstPrepB )
{
	if ( pstPrepA == NULL || pstPrepB == NULL )
	{
		return TRUE;
	}

	if ( pstPrepA->decWidth != pstPrepB->decWidth )
	{
		return TRUE;
	}

	if ( pstPrepA->decHeight != pstPrepB->decHeight )
	{
		return TRUE;
	}

	return FALSE; // size is not chagned.
}
#endif

static void H1EncodeReadIPC( int id, H1ENCODE_IPC_STATUS_T *pstPrepStatus )
{
	BOOLEAN _enableFlipTB = FALSE;

	// 사이즈, 포맷 정보를 PREP 레지스터에서 읽어옴.	 (DE/VDEC에서 기록하는 내용)
	u32 yBase, cBase;
	int inHeight, inWidth;
	int yStride, cStride;
	int frm_rate;
	H1ENCODE_FRAME_TYPE_T frameType;
	BOOLEAN	topFieldFirst = FALSE;

	LX_VENC_REG_IPC_STATUS_T ipcStatus;

#ifdef H1ENCODE_ENABLE_SAR
	int decHeight, decWidth;
#endif

	if ( pstPrepStatus == NULL )
	{
		return;
	}

	VENC_REG_CopyExternalRegs( id );
	VENC_REG_ReadIPCStatus( id, &ipcStatus );

	yBase = ipcStatus.yBase;
	cBase = ipcStatus.cBase;
	inHeight = ipcStatus.inHeight;
	inWidth = ipcStatus.inWidth;
	yStride = ipcStatus.yStride;
	cStride = ipcStatus.cStride;
	frameType = ipcStatus.frameType;
	frm_rate = ipcStatus.frameRateCode;
#ifdef H1ENCODE_ENABLE_FIELDORDER
	topFieldFirst = ipcStatus.topFieldFirst;
#else
	// Always set top-field first flag.
	topFieldFirst = TRUE;
#endif

#ifdef H1ENCODE_ENABLE_SAR
	decWidth = ipcStatus.decWidth;
	decHeight= ipcStatus.decHeight;
#endif

#ifdef H1ENCODE_DEBUG_SWAP_TB
	_enableFlipTB = TRUE;
#else
	_enableFlipTB = GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_FLIP_TB );
#endif

	if ( _enableFlipTB )
	{
		if ( frameType == LX_VENC_FRAME_TYPE_TOP )
		{
			frameType = LX_VENC_FRAME_TYPE_BOTTOM;
		}
		else if ( frameType == LX_VENC_FRAME_TYPE_BOTTOM )
		{
			frameType = LX_VENC_FRAME_TYPE_TOP;
		}
	}

	pstPrepStatus->yBase = yBase;
	pstPrepStatus->cBase = cBase;

	pstPrepStatus->inWidth = inWidth;
	pstPrepStatus->inHeight = inHeight;
	pstPrepStatus->yStride = yStride;
	pstPrepStatus->cStride = cStride;
	pstPrepStatus->frameType = frameType;
	pstPrepStatus->frameRateCode = frm_rate;
	pstPrepStatus->topFieldFirst = topFieldFirst;

#ifdef H1ENCODE_ENABLE_SAR
	pstPrepStatus->decWidth = decWidth;
	pstPrepStatus->decHeight = decHeight;
#endif

	return;
}

static void H1EncodeUpdateIPC( int id )
{
	H1ENCODE_IPC_STATUS_T stPrepStatus = {0};

	//memset( &stPrepStatus, 0x0, sizeof(H1ENCODE_IPC_STATUS_T) );

	// Read input frame infomation from PREP register (DE or VDEC writed)
	H1EncodeReadIPC( id, &stPrepStatus );

	gpH1Encode[id].stPrepStatus = stPrepStatus;

	_EventPost( H1ENCODE_EVENT_THUMBNAIL );

	return;
}

static void	H1EncodeMeasureFramerate( int id )
{
	u32 stc = 0;

	if ( gpH1Encode[id].stFramerate.count > 5 )
	{
		int i = 0;
		u32 nFramerate = 0;
		u32 diff_stc = 0;

		// drop first stc.
		for ( i = 2; i < 6; i++ )
		{
			diff_stc += gpH1Encode[id].stFramerate.gstc[i] - gpH1Encode[id].stFramerate.gstc[i-1];
		}

		diff_stc = diff_stc >> 2;
		nFramerate = 90000/diff_stc;

		if ( (90000%diff_stc) > (diff_stc>>1) )
			nFramerate += 1;

		VENC_ERROR("avg_diff_stc = %d, nFramerate = %d\n", diff_stc, nFramerate);

		H1EncodeSetFramerate( id, nFramerate );

		_EventPost( H1ENCODE_EVENT_FRAMERATE );
	}
	else
	{
		gpH1Encode[id].stFramerate.gstc[gpH1Encode[id].stFramerate.count] = VENC_REG_GetSTC( id, LX_VENC_TIMESTAMP_GSTC );
		gpH1Encode[id].stFramerate.count++;
	}
}

static void H1EncodeInputDetect( int id )
{
	H1ENCODE_IPC_STATUS_T stPrepStatus;

	H1EncodeReadIPC( id, &stPrepStatus );

	if ( stPrepStatus.inWidth > 0 && stPrepStatus.inHeight > 0 )
	{
		_EventPost( H1ENCODE_EVENT_INPUT );
	}

	return;
}

#define CALC_SIZE_YUV420( width, height ) ( ((width) * (height)) + ((width) * (height) / 2) )

static int _FrameCopy420( void *pY, void *pC, void *pTarget, u16 width, u16 height )
{
	int ySize = width * height;
	int cSize = width * height / 2;

	//ySize = MROUND( ySize, 32 );
	//cSize = MROUND( cSize, 32 );

	if ( pY == NULL || pC == NULL || pTarget == NULL )
	{
		return RET_ERROR;
	}

	// Copy Y Plane
	memcpy( (void *)pTarget, (void *)pY, ySize );

	// Copy C Plane
	memcpy( (void *)pTarget + ySize, (void *)pC, cSize );

	return RET_OK;
}

//
// Convert scan type from Interlaced to progressive
//
static int _ConvertScanI2P420( void *pSource, void *pTarget, u16 width, u16 height )
{
	//int totalLine = height + (height >> 1);
	u32 sourceOffset, targetOffset;
	u32 ySize = 0;
	void *tmp = NULL;
	int i;

	if ( pSource == NULL || pTarget == NULL )
	{
		return RET_ERROR;
	}

	if ( pSource == pTarget )
	{
		int totalSize = CALC_SIZE_YUV420( width, height );
		tmp = kmalloc( totalSize, GFP_KERNEL );

		memcpy( tmp, pSource, totalSize );

		pSource = tmp;
	}

	ySize = width * height;

	// Convert Y,C Plane
	for ( i = 0; i < (height / 2 + height /4); i++ )
	{
		sourceOffset = width * i;
		targetOffset = width * 2 * i;

		// odd
		memcpy( pTarget + targetOffset, pSource + sourceOffset, width );

		// even
		memcpy( pTarget + targetOffset + width, pSource + sourceOffset, width );
	}

	if ( tmp != NULL )
	{
		kfree( tmp );
	}

	return RET_OK;
}

//
// Convert image format (YUV420SemiPlanar -> YUV420Planar)
//
static int _ConvertYUV420_SP2P( void *pSource, void *pTarget, u16 width, u16 height )
{
	void *tmp = NULL;
	int i, j;
	int ySize, cbSize, crSize;
	u8 *cb, *cr;

	if ( pSource == NULL || pTarget == NULL )
	{
		return RET_ERROR;
	}

	if ( pSource == pTarget )
	{
		int totalSize = CALC_SIZE_YUV420( width, height );
		tmp = kmalloc( totalSize, GFP_KERNEL );

		memcpy( tmp, pSource, totalSize );

		pSource = tmp;
	}

	ySize = width * height;
	cbSize = (ySize >> 2);
	crSize = (ySize >> 2);

	cb = (u8 *)(pTarget + ySize + 0);
	cr = (u8 *)(pTarget + ySize + cbSize);

	// Copy Y Plane
	memcpy( pSource, pTarget, ySize );

	// Convert C Plane
	for ( i = 0; i < height / 4; i++ )
	{
		UINT32 sourceOffset = ySize + (i * width * 2);
		UINT32 targetOffset = i * width;

		for ( j = 0; j < width; j ++ )
		{
			cb[targetOffset + j] = *(char *)(pSource + sourceOffset++);
			cr[targetOffset + j] = *(char *)(pSource + sourceOffset++);
		}
	}

	if ( tmp != NULL )
	{
		kfree( tmp );
	}

	return RET_OK;
}

#if 0
static int _FrameRemovePadding420( void *pSource, void *pTarget, u16 width, u16 height, u16 stride )
{
	int totalLine = height + (height >> 1);
	int i;

	if ( pSource == NULL || pTarget == NULL )
	{
		return RET_ERROR;
	}

	for ( i = 0; i < totalLine; i++ )
	{
		u32 sourceOffset = stride * i;
		u32 targetOffset = width * i;

		memcpy( pTarget + targetOffset, pSource + sourceOffset, width );
	}

	return RET_OK;

}
#endif

int H1EncodeGetFrameImage( int id, LX_VENC_RECORD_FRAME_IMAGE_T *pstFrameImage )
{
	int ret = RET_ERROR;

	if ( pstFrameImage == NULL )
	{
		return ret;
	}

	ret = _EventWait( H1ENCODE_EVENT_THUMBNAIL, 100 );

	if ( ret != RET_OK )
	{
		VENC_WARN("Event timeout!!!\n");
		ret = RET_ERROR;
	}
	else
	{
		u32 yBase = gpH1Encode[id].stPrepStatus.yBase;
		u32 cBase = gpH1Encode[id].stPrepStatus.cBase;
		u16 width = gpH1Encode[id].stPrepStatus.inWidth;
		u16 height = gpH1Encode[id].stPrepStatus.inHeight;
		u16 stride = gpH1Encode[id].stPrepStatus.yStride;
		u32 size = height * stride;

		LX_VENC_RECORD_FRAME_TYPE_T frameType = gpH1Encode[id].stPrepStatus.frameType;

		void *pY = NULL;
		void *pC = NULL;

		pY = ioremap( yBase, size );
		pC = ioremap( cBase, size );

		if ( pY == NULL || pC == NULL )
		{
			goto Release;
		}
		else
		{
			u32 cbOffset, crOffset;

			if ( frameType == LX_VENC_RECORD_FRAME_TYPE_PROGRESSIVE )
			{
				_FrameCopy420( pY, pC, (void *)gpH1Encode[id].memTFB.u32Virt, stride, height );
			}
			else
			{
				_FrameCopy420( pY, pC, (void *)gpH1Encode[id].memTFB.u32Virt, stride, height/2 );
				_ConvertScanI2P420( (void *)gpH1Encode[id].memTFB.u32Virt, (void *)gpH1Encode[id].memTFB.u32Virt, stride, height );
			}

			_ConvertYUV420_SP2P( (void *)gpH1Encode[id].memTFB.u32Virt, (void *)gpH1Encode[id].memTFB.u32Virt, stride, height );

			cbOffset = (stride * height);
			crOffset = cbOffset + (stride * height / 4);

			pstFrameImage->type = LX_VENC_RECORD_FRAME_IMAGE_TYPE_420_PLANAR;
			pstFrameImage->ui32Size = CALC_SIZE_YUV420( stride, height );
			pstFrameImage->ui32Height = height;
			pstFrameImage->ui32Width = width;
			pstFrameImage->ui32Stride = stride;
			pstFrameImage->ui32YPhyAdd = gpH1Encode[id].memTFB.u32Phys;
			pstFrameImage->ui32UPhyAdd = gpH1Encode[id].memTFB.u32Phys + cbOffset;
			pstFrameImage->ui32VPhyAdd = gpH1Encode[id].memTFB.u32Phys + crOffset;

			 ret = RET_OK;
		}

Release:
		if ( pY != (void *)-1 )
		{
			iounmap( pY );
		}

		if ( pC != (void *)-1 )
		{
			iounmap( pC );
		}

	}

	return ret;
}

static void _CheckFramerateInformation( int id )
{
	H1ENCODE_IPC_STATUS_T stPrepStatus;

	if ( gpH1Encode[id].stInput.frameRateDenom > 0 && gpH1Encode[id].stInput.frameRateNum > 0 )
	{
		return;
	}

	H1EncodeReadIPC( id, &stPrepStatus );

	if ( stPrepStatus.frameRateCode == LX_VENC_FRAME_RATE_15HZ )
	{
		// Read ipc again..
		msleep_interruptible( 150 );
		H1EncodeReadIPC( id, &stPrepStatus );
	}

	if ( stPrepStatus.frameRateCode != LX_VENC_FRAME_RATE_NONE )
	{
		return;
	}
	else
	{
		// In the normal situatin, can't reach to this code block
		// because frame rate information from DE IPC are always valid.
		int ev_ret = RET_ERROR;

		gpH1Encode[id].stFramerate.count = 0;
		memset( gpH1Encode[id].stFramerate.gstc, 0x0, sizeof(gpH1Encode[id].stFramerate.gstc) );

		gpH1Encode[id].enableFramerateDetect = TRUE;

		// Auto-detected Framerate.
		_InterruptEnableVsync( id, TRUE );

		// Wait until measure framerate
		ev_ret = _EventWait( H1ENCODE_EVENT_FRAMERATE, 500 );

		gpH1Encode[id].enableFramerateDetect = FALSE;

		_InterruptEnableVsync( id, FALSE );
	}

}

static int _MakeFramerateInformation( int id, H1ENCODE_API_T * pstEncoder, LX_VENC_RECORD_FRAME_RATE_T eFramerate )
{
	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	// 20130207: Fix for frame rate problem.
	if ( gpH1Encode[id].stInput.frameRateDenom == 0 || gpH1Encode[id].stInput.frameRateNum == 0 )
	{
		VENC_INFO("The framerate set to a value from PREP register.\n");

		pstEncoder->SetFrameRate( pstEncoder, eFramerate );
	}
	else
	{
		VENC_INFO("The framerate set to a value from KADP. [%d,%d,%d]\n",
			gpH1Encode[id].stInput.frameRateCode, gpH1Encode[id].stInput.frameRateDenom, gpH1Encode[id].stInput.frameRateNum);

		pstEncoder->frameRateCode = gpH1Encode[id].stInput.frameRateCode;
		pstEncoder->frameRateDenom = gpH1Encode[id].stInput.frameRateDenom;
		pstEncoder->frameRateNum = gpH1Encode[id].stInput.frameRateNum;
	}

	if ( pstEncoder->frameRateDenom == 0 || pstEncoder->frameRateNum == 0 )
	{
		int ev_ret = RET_ERROR;

		gpH1Encode[id].stFramerate.count = 0;
		memset( gpH1Encode[id].stFramerate.gstc, 0x0, sizeof(gpH1Encode[id].stFramerate.gstc) );

		gpH1Encode[id].enableFramerateDetect = TRUE;

		// Auto-detected Framerate.
		_InterruptEnableVsync( id, TRUE );

		// Wait until measure framerate
		ev_ret = _EventWait( H1ENCODE_EVENT_FRAMERATE, 500 );

		gpH1Encode[id].enableFramerateDetect = FALSE;

		_InterruptEnableVsync( id, FALSE );

		if ( gpH1Encode[id].stInput.frameRateCode > 0
			&& gpH1Encode[id].stInput.frameRateDenom > 0
			&& gpH1Encode[id].stInput.frameRateNum > 0 )
		{
			VENC_INFO("The framerate set to Auto-Dectection. [%d,%d,%d]\n",
				gpH1Encode[id].stInput.frameRateCode, gpH1Encode[id].stInput.frameRateDenom, gpH1Encode[id].stInput.frameRateNum);

			pstEncoder->frameRateCode = gpH1Encode[id].stInput.frameRateCode;
			pstEncoder->frameRateDenom = gpH1Encode[id].stInput.frameRateDenom;
			pstEncoder->frameRateNum = gpH1Encode[id].stInput.frameRateNum;
		}
		else
		{
			H1ENCODE_IPC_STATUS_T stPrepStatus;

			H1EncodeReadIPC( id, &stPrepStatus );

			// Set Defatult framerate
			if ( stPrepStatus.frameType == LX_VENC_FRAME_TYPE_PROGRESSIVE )
			{
				H1EncodeSetFramerate( id, 30 );
			}
			else
			{
				H1EncodeSetFramerate( id, 60 );
			}
		}
	}

	return RET_OK;
}

static UINT32 _GetResolutionType( const UINT32 width, const UINT32 height )
{
	enum {
		RESOLUTION_UNKNOWN = 0,
		RESOLUTION_SD,
		RESOLUTION_HD,
		RESOLUTION_FHD,
	};
	UINT32 res = width * height;
	UINT32 resType = RESOLUTION_UNKNOWN;

	if ( res == 0 )
	{
		resType = RESOLUTION_UNKNOWN;
	}
	else if ( res <= 720 * 576 )
	{
		resType = RESOLUTION_SD;
	}
	else if ( res <= 1280 * 720 )
	{
		resType = RESOLUTION_HD;
	}
	else if ( res <= 1920 * 1088 )
	{
		resType = RESOLUTION_FHD;
	}

	return resType;
}

static BOOLEAN _CheckInputResolution( int id, H1ENCODE_IPC_STATUS_T *pstPrepStatus )
{
	int retry = 5;
	int i = 0;
	UINT8 resType = 0;
	BOOLEAN matched = FALSE;

	if ( pstPrepStatus == NULL )
	{
		return FALSE;
	}

	resType = _GetResolutionType( gpH1Encode[id].stInput.width, gpH1Encode[id].stInput.height );

	if ( resType == 0  )
	{
#if 0
		// Wait static delay for input transition.
		msleep_interruptible( 150 );
		H1EncodeReadIPC( pstPrepStatus );
#else
		int ev_ret = RET_ERROR;

		msleep_interruptible( 150 );

		// Wait until valid sync interrupt.
		gpH1Encode[id].enableInputDetect = TRUE;

		_EventClear();
		_InterruptEnableVsync( id, TRUE );

		// Wait until vsync occured.
		ev_ret = _EventWait( H1ENCODE_EVENT_INPUT, 300 );

		gpH1Encode[id].enableInputDetect = FALSE;

		_InterruptEnableVsync( id, FALSE );

		H1EncodeReadIPC( id, pstPrepStatus );
#endif

		return FALSE;
	}

	// 20131031(jaeseop.so)
	// WebOS에서 HAL을 통해서 width,height정보가 내려오지 않기 때문에
	// 아래 비교하는 로직으로 들어가는 경우는 없음.
	// 향후 사용할 수도 있기에 남겨둠.
	for( i = 0; i < retry; i++ )
	{
		H1ENCODE_IPC_STATUS_T stPrepStatus;
		UINT32 resTypeIPC = 0;

		H1EncodeReadIPC( id, &stPrepStatus );

		resTypeIPC = _GetResolutionType( stPrepStatus.inWidth, stPrepStatus.inHeight );

		if( resType == resTypeIPC )
		{
			// Copy ipc register values to input parameter.
			*pstPrepStatus = stPrepStatus;

			matched = TRUE;
			break;
		}

		VENC_INFO("SET[%d,%d] INPUT[%d,%d]\n",
			gpH1Encode[id].stInput.width, gpH1Encode[id].stInput.height,
			stPrepStatus.inWidth, stPrepStatus.inHeight);
		VENC_INFO("Delay 50ms for input transition.\n");

		msleep_interruptible( 50 );
	}

	return matched;
}

static int H1EncodeStart( int id, H1ENCODE_IPC_STATUS_T stPrepStatus )
{
	int ret = RET_ERROR;
	H1ENCODE_API_T * pstEncoder;

	TRACE_ENTER();

	if ( gpH1Encode[id].pstEncoder != NULL )
	{
		// Already allocate encoder instance
		return RET_ERROR;
	}

	if ( stPrepStatus.inWidth == 0 || stPrepStatus.inHeight == 0 )
	{
		VENC_WARN("[%d] Input resolution error.(%dx%d)\n", id, stPrepStatus.inWidth, stPrepStatus.inHeight );
		return RET_ERROR;
	}

	H1ENCODE_LOCK(id);

	if (gpH1Encode[id].outputType == LX_VENC_ENCODE_TYPE_VP8)
	{
		gpH1Encode[id].pstEncoder = VP8Alloc();
	}
	else
	{
		gpH1Encode[id].pstEncoder = H264Alloc();
	}

	gpH1Encode[id].ui32TotalVsync = 0;
#ifdef H1ENCODE_ENABLE_DROP_FIRST_FRAME
	gpH1Encode[id].remain_drop = H1ENCODE_DROP_FIRST_FRAME_COUNT;
#endif

	H1ENCODE_UNLOCK(id);

	pstEncoder = gpH1Encode[id].pstEncoder;

	if ( pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	// core id
	pstEncoder->id = id;
	pstEncoder->isBusy = FALSE;

	// Check function pointers
	if ( pstEncoder->SetMemOSB == NULL || pstEncoder->SetMemESB == NULL || pstEncoder->SetMemAUI == NULL )
	{
		VENC_ERROR("Memory setter function is NULL.\n");
		ret = RET_ERROR;
		goto func_exit;
	}

	if ( pstEncoder->SetFrameType == NULL || pstEncoder->SetFrameSize == NULL ||
		pstEncoder->SetGOPLength == NULL || pstEncoder->SetFrameRate == NULL ||
		pstEncoder->SetBitrate == NULL )
	{
		VENC_ERROR("Config setter function is NULL.\n");
		ret = RET_ERROR;
		goto func_exit;
	}

	if ( pstEncoder->Open == NULL || pstEncoder->Close == NULL )
	{
		VENC_ERROR("Open or Close function is NULL.\n");
		ret = RET_ERROR;
		goto func_exit;
	}

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
	if ( pstEncoder->EncodeAsync == NULL || pstEncoder->EncodeDone == NULL )
	{
		VENC_ERROR("Not support non-blocking mode encoding.\n");
	}
#else
	if ( pstEncoder->Encode == NULL )
	{
		VENC_ERROR("Not support thread mode encoding.\n");
	}
#endif

	// Set buffer infomation
	pstEncoder->SetMemOSB( pstEncoder, gpH1Encode[id].memOSB );
	pstEncoder->SetMemESB( pstEncoder, gpH1Encode[id].memESB );
	pstEncoder->SetMemAUI( pstEncoder, gpH1Encode[id].memAUI );

	// Init configuration
	pstEncoder->eFrameType = stPrepStatus.frameType;
	pstEncoder->topFieldFirst = stPrepStatus.topFieldFirst;
	pstEncoder->width = stPrepStatus.inWidth;
	pstEncoder->height = stPrepStatus.inHeight;
	pstEncoder->stride = stPrepStatus.yStride;
	if (  gpH1Encode[id].stInput.gopLength < 1 || gpH1Encode[id].stInput.gopLength > 300)
	{
		pstEncoder->gopLength = H1ENCODE_GOPLENGTH_DEFAULT;
	}
	else
	{
		pstEncoder->gopLength = gpH1Encode[id].stInput.gopLength;
	}

	_MakeFramerateInformation( id, pstEncoder, stPrepStatus.frameRateCode );

	if ( gpH1Encode[id].stInput.targetBitrate > 0 )
	{
		pstEncoder->bitrate = gpH1Encode[id].stInput.targetBitrate;
	}
	else if ( pstEncoder->bitrate == 0 )
	{
		if ( pstEncoder->width == 1920 && pstEncoder->height == 1080 )
		{
			pstEncoder->bitrate = H1ENCODE_BITRATE_FHD;
		}
		else
		{
			pstEncoder->bitrate = H1ENCODE_BITRATE_HD;
		}
	}

	if ( pstEncoder->bitrate > 0 )
	{
		pstEncoder->bEnableCBR = gpH1Encode[id].stInput.bEnableCBR;
	}

#ifdef H1ENCODE_ENABLE_SAR
	// Set SAR Infomation
	if ( gpH1Encode[id].stInput.sarWidth > 0 && gpH1Encode[id].stInput.sarHeight > 0 )
	{
		pstEncoder->decWidth = gpH1Encode[id].stInput.sarWidth;
		pstEncoder->decHeight = gpH1Encode[id].stInput.sarHeight;
	}
	else
	{
		//pstEncoder->decWidth = stPrepStatus.decWidth;
		//pstEncoder->decHeight = stPrepStatus.decHeight;
		pstEncoder->decWidth = 0;
		pstEncoder->decHeight = 0;
	}
#endif

	H1ENCODE_LOCK(id);

	// Open encoder instance ( h.264 or VP8 )
	ret = pstEncoder->Open( pstEncoder );

	H1ENCODE_UNLOCK(id);

func_exit:
	if ( ret != RET_OK && pstEncoder != NULL )
	{
		if ( gpH1Encode[id].outputType == LX_VENC_ENCODE_TYPE_VP8 )
		{
			VP8Release( pstEncoder );
		}
		else
		{
			H264Release( pstEncoder );
		}

		gpH1Encode[id].pstEncoder = NULL;
	}

	return ret;
}

static int H1EncodeStop( int id )
{
	int ret = RET_ERROR;
	int wait_count = 50;

	H1ENCODE_API_T *pstEncoder;

	TRACE_ENTER();

	if ( gpH1Encode[id].pstEncoder == NULL )
	{
		return RET_ERROR;
	}

	while( gpH1Encode[id].pstEncoder->isBusy )
	{
		if ( wait_count == 0 )
		{
			VENC_WARN("wait_count is zero.\n");
			break;
		}

		// Wait until idle state
		msleep_interruptible(10);
		wait_count--;
	}

	pstEncoder = gpH1Encode[id].pstEncoder;

	if ( pstEncoder->Close == NULL )
	{
		VENC_ERROR("Close function is NULL.\n");
		return RET_ERROR;
	}

	H1ENCODE_LOCK(id);

#ifdef H1ENCODE_ENABLE_SCD
	// free resource to avoid memory leak.
	if ( gpH1Encode[id].pstEncParamsPrev != NULL )
	{
		_EncParamFree( gpH1Encode[id].pstEncParamsPrev );
		gpH1Encode[id].pstEncParamsPrev = NULL;
	}
#endif

	ret = pstEncoder->Close( pstEncoder );

	H1ENCODE_UNLOCK(id);

	// Flush workqueue to avoid NULL pointer access after H1EncodeClose().
	_WorkQFlush();

	H1ENCODE_LOCK(id);

	if (gpH1Encode[id].outputType == LX_VENC_ENCODE_TYPE_VP8)
	{
		VP8Release( pstEncoder );
	}
	else
	{
		H264Release( pstEncoder );
	}

	gpH1Encode[id].stInput.frameRateCode = 0;
	gpH1Encode[id].stInput.frameRateDenom = 0;
	gpH1Encode[id].stInput.frameRateNum = 0;
	gpH1Encode[id].stInput.width = 0;
	gpH1Encode[id].stInput.height = 0;
	gpH1Encode[id].pstEncoder = NULL;

	H1ENCODE_UNLOCK(id);

	return ret;
}

#ifdef H1ENCODE_DEBUG_INTERLACED_DUMP
static void _DebugSaveField( void )
{
	u32 yBase;
	H1ENCODE_FRAME_TYPE_T frameType;
	u32 topBase = gpH1Encode[id].memTFB.u32Virt;
	u32 bottomBase = gpH1Encode[id].memTFB.u32Virt + 0x220000;

	yBase = gpH1Encode[id].stPrepStatus.yBase;
	frameType = gpH1Encode[id].stPrepStatus.frameType;

	u32 yVirt = (u32)ioremap( yBase, 2048 * 100 );

	static int top;
	static int bottom;

	if ( top == 0 && frameType == LX_VENC_FRAME_TYPE_TOP )
	{
		VENC_DEBUG("[T] 0x%08x\n", gpH1Encode[id].memTFB.u32Phys);
		memcpy( (void *)topBase, (void *)yVirt, 2048*100 );

		top = 1;
	}
	else if ( bottom == 0 && frameType == LX_VENC_FRAME_TYPE_BOTTOM )
	{
		VENC_DEBUG("[B] 0x%08x\n", gpH1Encode[id].memTFB.u32Phys + 0x220000);
		memcpy( (void *)bottomBase, (void *)yVirt, 2048*100 );

		bottom = 1;
	}

	iounmap( (void *)yVirt );
}
#endif

#ifdef H1ENCODE_USE_NONBLOCKING_MODE

static void H1EncodeEncodeDone( unsigned long id )
{
	int ret;
	H1ENCODE_API_T *pstEncoder;
	H1ENCODE_ENC_PARAMS_T *pstEncParams;
	unsigned long flags;

	pstEncoder = gpH1Encode[id].pstEncoder;

	if ( pstEncoder == NULL )
	{
		VENC_ERROR("pstEncoder is NULL\n");
		return;
	}

	pstEncParams = _EncParamAlloc();
	if ( pstEncParams == NULL )
	{
		VENC_ERROR("H1EncodeEncodeDone: Can't allocate pstEncParams.\n");
		return;
	}

	ret = pstEncoder->EncodeDone( pstEncoder, pstEncParams );

	if ( ret == RET_OK )
	{
		gpH1Encode[id].prevFrameType = pstEncParams->frameType;
	}

	H1ENCODE_LOCK_IRQ(id);
	pstEncoder->isBusy = FALSE;
	H1ENCODE_UNLOCK_IRQ(id);

#ifdef H1ENCODE_USE_WORKAROUND_HW_DELAY
	if ( gpH1Encode[id].savedIPC != NULL )
	{
		int ret = 0;

		VENC_DEBUG("[%d] Try encoding by saved IPC.\n", id);
		ret = H1EncodeEncode( id, gpH1Encode[id].savedIPC, gpH1Encode[id].savedGSTC );
		kfree(gpH1Encode[id].savedIPC);
		gpH1Encode[id].savedIPC = NULL;
	}
#endif

	return;
}

#endif

static int H1EncodeEncode( int id, H1ENCODE_IPC_STATUS_T *pstPrep, u32 gstc )
{
	int ret = RET_ERROR;
	H1ENCODE_FRAME_TYPE_T frameType;
	H1ENCODE_API_T *pstEncoder;
	H1ENCODE_ENC_PARAMS_T *pstEncParams;
	unsigned long flags;

	if ( gpH1Encode[id].pstEncoder == NULL || pstPrep == NULL )
	{
		return RET_ERROR;
	}

	pstEncoder = gpH1Encode[id].pstEncoder;

	if ( pstEncoder->isBusy )
	{
#ifdef H1ENCODE_USE_WORKAROUND_HW_DELAY
		H1ENCODE_LOCK_IRQ(id);
		if ( gpH1Encode[id].savedIPC == NULL )
		{
			gpH1Encode[id].savedIPC = kmalloc(sizeof(H1ENCODE_IPC_STATUS_T), GFP_KERNEL);
			*gpH1Encode[id].savedIPC = *pstPrep;
			gpH1Encode[id].savedGSTC = gstc;

			H1ENCODE_UNLOCK_IRQ(id);

			VENC_WARN("[%d] Encoder h/w is busy. save ipc to retry.\n", id);

			return RET_TRY_AGAIN;
		}
		H1ENCODE_UNLOCK_IRQ(id);
#endif
		VENC_ERROR("[%d] Encoder h/w is busy.\n", id);

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
		gpH1Encode[id].ui32CountFrameSkip++;
#endif

		return RET_ERROR;
	}

	pstEncParams = _EncParamAlloc();

	if ( pstEncParams == NULL )
	{
		VENC_ERROR("H1EncodeEncode: Can't allocate pstEncParams.\n");
		return RET_ERROR;
	}

	H1ENCODE_LOCK_IRQ(id);
	pstEncoder->isBusy = TRUE;
	H1ENCODE_UNLOCK_IRQ(id);

	pstEncParams->id = id;							// encoder core id
	pstEncParams->u32YBase = pstPrep->yBase;
	pstEncParams->u32CBase = pstPrep->cBase;
	pstEncParams->u32Timestamp 	= gstc;
	pstEncParams->frameType = pstPrep->frameType;	// for determining field or frame

#ifdef H1ENCODE_ENABLE_SCD
	if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SCD ) )
	{
		if ( gpH1Encode[id].pstEncParamsPrev == NULL )
		{
			gpH1Encode[id].pstEncParamsPrev = pstEncParams;

			H1ENCODE_LOCK_IRQ(id);
			pstEncoder->isBusy = FALSE;
			H1ENCODE_UNLOCK_IRQ(id);

			return RET_OK;
		}
		else
		{
			H1ENCODE_ENC_PARAMS_T *pstEncParamsTmp;

			// Swap current pstEncParams to gpH1Encode[id].pstEncParamsPrev
			pstEncParamsTmp = pstEncParams;
			pstEncParams = gpH1Encode[id].pstEncParamsPrev;
			gpH1Encode[id].pstEncParamsPrev = pstEncParamsTmp;

			if ( pstEncParams != NULL )
			{
				pstEncParams->u32YBaseNext = gpH1Encode[id].pstEncParamsPrev->u32YBase;
			}
		}
	}

	if ( pstEncParams == NULL )
	{
		H1ENCODE_LOCK_IRQ(id);
		pstEncoder->isBusy = FALSE;
		H1ENCODE_UNLOCK_IRQ(id);

		return RET_ERROR;
	}
#endif

#ifdef H1ENCODE_CHECK_INTERLACED_INPUT
	frameType = pstEncParams->frameType;

	if ( frameType != LX_VENC_FRAME_TYPE_PROGRESSIVE )
	{
#ifndef H1ENCODE_ENABLE_FIELDORDER
	#ifdef H1ENCODE_ENABLE_INTERLACED_BF
		if ( pstEncoder->bIsFirstFrame && frameType == LX_VENC_FRAME_TYPE_TOP )
	#else
		// if input frame is interaced, check frame type is top field first.
		if ( pstEncoder->bIsFirstFrame && frameType == LX_VENC_FRAME_TYPE_BOTTOM )
	#endif
#else
		if ( pstEncoder->bIsFirstFrame == TRUE )
		{
			if ( pstPrep->topFieldFirst )
			{
				gpH1Encode[id].prevFrameType = LX_VENC_FRAME_TYPE_BOTTOM;
			}
			else
			{
				gpH1Encode[id].prevFrameType = LX_VENC_FRAME_TYPE_TOP;
			}
		}

		if ( pstEncoder->bIsFirstFrame == TRUE && frameType == gpH1Encode[id].prevFrameType )
#endif
		{
			// Skip first unexpected field.
			//ret = RET_ERROR;
			_EncParamFree( pstEncParams );
			ret = RET_OK;

			H1ENCODE_LOCK_IRQ(id);
			pstEncoder->isBusy = FALSE;
			H1ENCODE_UNLOCK_IRQ(id);

			goto ENCODE_EXIT;
		}

		if ( gpH1Encode[id].prevFrameType == frameType )
		{
			if ( GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_FIELD_REPEAT ) )
			{
				H1ENCODE_ENC_PARAMS_T *pstEncParamsRepeat = NULL;
				pstEncParamsRepeat = _EncParamAlloc();

				VENC_WARN("[%d] Field repeating.. (%d)\n", id, frameType);

				if ( pstEncParamsRepeat != NULL )
				{
					*pstEncParamsRepeat = *pstEncParams;

					if ( frameType == LX_VENC_FRAME_TYPE_BOTTOM )
						pstEncParamsRepeat->frameType = LX_VENC_FRAME_TYPE_TOP;
					else
						pstEncParamsRepeat->frameType = LX_VENC_FRAME_TYPE_BOTTOM;

					ret = pstEncoder->Encode( pstEncoder, pstEncParamsRepeat );

					if ( ret != RET_OK )
						_EncParamFree( pstEncParamsRepeat );
				}
			}
			else
			{
				VENC_WARN("[%d] Unexpected field type. [%d]\n", id, frameType);
				// Skip the duplicated type of field.

				H1ENCODE_LOCK_IRQ(id);
				pstEncoder->isBusy = FALSE;
				H1ENCODE_UNLOCK_IRQ(id);

#if 0
				ret = RET_ERROR;
				goto ENCODE_END;
#else
				_EncParamFree( pstEncParams );
				return RET_ERROR;
#endif
			}
		}

		//gpH1Encode[id].prevFrameType = frameType;
	}
#endif

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
	if ( pstEncoder->EncodeAsync != NULL )
	{
		ret = pstEncoder->EncodeAsync( pstEncoder, pstEncParams );
	}
	else
	{
		VENC_ERROR("Not support non-block mode encoding.\n");
		goto ENCODE_END;
	}

	_EncParamFree( pstEncParams );
#else
	ret = pstEncoder->Encode( pstEncoder, pstEncParams );

	H1ENCODE_LOCK_IRQ(id);
	pstEncoder->isBusy = FALSE;
	H1ENCODE_UNLOCK_IRQ(id);
#endif

ENCODE_END:
	if ( ret != RET_OK )
	{
		VENC_WARN("[%d] Encode error!!!\n", id);

		pstEncoder->isBusy = FALSE;

		if ( pstEncParams != NULL )
		{
			_EncParamFree( pstEncParams );
		}
	}
#ifdef H1ENCODE_CHECK_INTERLACED_INPUT
	else
	{
#ifdef H1ENCODE_USE_NONBLOCKING_MODE
#else
		gpH1Encode[id].prevFrameType = frameType;
#endif
	}
#endif

#ifdef SUPPORT_VENC_DEVICE_FASYNC_FOPS
	if ( ret == RET_OK )
	{
		VENC_KillFasync( SIGIO );
	}
#endif
ENCODE_EXIT:
	return ret;
}

int H1EncodeCommand( int id, LX_VENC_RECORD_COMMAND_T cmd )
{
	int ret;

	if ( gpH1Encode[id].eStatus == H1ENCODE_STATUS_TYPE_STOP_PREPARE ||
		gpH1Encode[id].eStatus == H1ENCODE_STATUS_TYPE_START_PREPARE )
	{
		VENC_INFO( "Status is STOP/START PREPARE.\n");
		return RET_ERROR;
	}

	if (cmd == LX_VENC_COMMAND_REFRESH)
	{
		gstOutputStatus.ui32ESRptr = gstOutputStatus.ui32ESWptr = gpH1Encode[id].memESB.u32Phys;
		gstOutputStatus.ui32AUIRptr = gstOutputStatus.ui32AUIWptr = gpH1Encode[id].memAUI.u32Phys;
	}
	else
	{
		gstOutputStatus.ui32ESRptr = gstOutputStatus.ui32ESWptr;
		gstOutputStatus.ui32AUIRptr = gstOutputStatus.ui32AUIWptr;
	}

	if ( cmd == LX_VENC_COMMAND_REFRESH && gpH1Encode[id].eStatus != H1ENCODE_STATUS_TYPE_START )
	{
		H1ENCODE_IPC_STATUS_T stPrepStatus = {};

		H1ENCODE_LOCK(id);
		gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_START_PREPARE;
		H1ENCODE_UNLOCK(id);

	#if 0
		msleep( 100 );

		// Init 전에 picture size 및 format 정보를 읽어 옴.
		H1EncodeReadIPC( &stPrepStatus );
	#else
		// Read ipc registers and Check input resolution.
		// If the DE send vsync late, IPC register values are invalid.
		// So, wait until valid ipc and check frame information.
		_CheckInputResolution( id, &stPrepStatus );
	#endif

		_CheckFramerateInformation( id );

		// Index 및 offset 초기화
		H1EncodeReset( &gpH1Encode[id] );

		// H1 Init 및 Config 정보 설정
		ret = H1EncodeStart( id, stPrepStatus );

		if ( ret == RET_OK )
		{
			_EncParamClear();
			_InterruptEnableVsync( id, TRUE );

			H1ENCODE_LOCK(id);
			gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_START;
			H1ENCODE_UNLOCK(id);

			VENC_NOTI("[%d] Video encoding start..\n", id);
		}
		else
		{
			H1ENCODE_LOCK(id);
			gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_STOP;
			H1ENCODE_UNLOCK(id);

			VENC_NOTI("[%d] Can't start video encoding.\n", id);
		}
	}
	else if ( cmd == LX_VENC_COMMAND_PAUSE && gpH1Encode[id].eStatus != H1ENCODE_STATUS_TYPE_STOP )
	{
		H1ENCODE_LOCK(id);
		gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_STOP_PREPARE;
		H1ENCODE_UNLOCK(id);

		_InterruptEnableVsync( id, FALSE );

		ret = H1EncodeStop( id );

		VENC_NOTI("[%d] Video encoding stop..\n", id);

		H1ENCODE_LOCK(id);
		gpH1Encode[id].eStatus = H1ENCODE_STATUS_TYPE_STOP;
		H1ENCODE_UNLOCK(id);
	}
	else
	{
		VENC_ERROR("[%d] Invalid command. (%d)\n", id, cmd);
		return RET_ERROR;
	}

	if ( ret == RET_OK )
	{
		gpH1Encode[id].cmd = cmd;
	}

	return ret;
}

void H1EncodeAUIWrite( H1ENCODE_MEM_T *pMemAUI, const H1ENCODE_AUI_T stAUI )
{
	// au_type
	//	0: IDR picture
	//	1: non-IDR I picture
	// 	2: P picture
	//	3: B picture
	// unit_size: encoded size
	// unit_start: offset from output buffer base
	// timestamp: time at incoming time

	// +----------------------------------------------------------------+
	// |8bit|24bit      |32bit          |32bit          |32bit          | <= 16byte
	// +----+-----------+---------------+---------------+---------------+
	// |AU  |UINT_SIZE  |UINT_START     |INDEX          |TIMESTAMP      |
	// +----------------------------------------------------------------+
	H1ENCODE_AUI_T *pstAUI;

	pstAUI = (H1ENCODE_AUI_T *)(pMemAUI->u32Virt + pMemAUI->u32WriteOffset );
	*pstAUI = stAUI;

	pMemAUI->u32WriteOffset += sizeof(H1ENCODE_AUI_T);	// 128bits = 16bytes

	if ( pMemAUI->u32WriteOffset >= pMemAUI->u32Size )
	{
		pMemAUI->u32WriteOffset = 0;
	}

	return;
}

static void H1EncodePostDone( int id )
{
	//TRACE_ENTER();

	gpH1Encode[id].eventEncodeDoneWakeup++;
	wake_up_interruptible( &gpH1Encode[id].wqEncodeDone );

}

static int H1EncodeWaitDoneTimeout( int id, int timeout )
{
	int ret = RET_OK;

	//TRACE_ENTER();

	gpH1Encode[id].eventEncodeDoneWakeup = 0;

	if ( timeout > 0 )
	{
		ret = wait_event_interruptible_timeout( gpH1Encode[id].wqEncodeDone, gpH1Encode[id].eventEncodeDoneWakeup > 0, msecs_to_jiffies(timeout) );

		if ( ret == 0 )
		{
			ret = RET_TIMEOUT;
		}
		else
		{
			ret = RET_OK;
		}
	}
	else
	{
		wait_event_interruptible( gpH1Encode[id].wqEncodeDone, gpH1Encode[id].eventEncodeDoneWakeup > 0 );
	}

	return ret;
}

void H1EncodeNotifyDone( int id, H1ENCODE_MEM_T *pstESB, H1ENCODE_MEM_T *pstAUI )
{
	UINT32 ui32ESWptr, ui32ESRptr;
	UINT32 ui32AUIWptr, ui32AUIRptr;

	if ( pstESB == NULL || pstAUI == NULL )
	{
		VENC_ERROR("%s: pstESB or pstAUI is NULL.\n", __F__);
		return;
	}

	ui32ESWptr = pstESB->u32Phys + pstESB->u32WriteOffset;
	ui32ESRptr = pstESB->u32Phys + pstESB->u32ReadOffset;
	ui32AUIWptr = pstAUI->u32Phys + pstAUI->u32WriteOffset;
	ui32AUIRptr = pstAUI->u32Phys + pstAUI->u32ReadOffset;

#if 0
	if ( gui32NumberOfFrames < VENC_BIT_RATE_BUFFER ) { ++gui32NumberOfFrames; }

	gaFrameBytes[gui32BufferIndex] = (ui32ESRptr<ui32ESWptr)?
									(ui32ESWptr-ui32ESRptr) : (pstESB->u32Size-(ui32ESRptr-ui32ESWptr));
	gui32BufferIndex = (gui32BufferIndex+1)%VENC_BIT_RATE_BUFFER;
#endif

	// Flash ES buffer when buffer overflow
	if ( ( gstOutputStatus.ui32ESRptr <= ui32ESWptr) && (ui32ESWptr <= gstOutputStatus.ui32ESWptr) )
	{
		gstOutputStatus.ui32ESRptr = ui32ESRptr;
	}
	gstOutputStatus.ui32ESWptr = ui32ESWptr;

	// Flash AUI buffer when buffer overflow
	if ( ( gstOutputStatus.ui32AUIRptr <= ui32AUIWptr ) && (ui32AUIWptr <= gstOutputStatus.ui32AUIWptr) )
	{
		gstOutputStatus.ui32AUIRptr = ui32AUIRptr;
	}
	gstOutputStatus.ui32AUIWptr = ui32AUIWptr;

#if 0
	VENC_DEBUG("[%s] gui32ESRptr : 0x%8x, gui32ESWptr : 0x%08x\n",
		__FUNCTION__, gstOutputStatus.ui32ESRptr, gstOutputStatus.ui32ESWptr);
	VENC_DEBUG("[%s] gui32AUIRptr: 0x%8x, gui32AUIWptr: 0x%08x\n",
		__FUNCTION__, gstOutputStatus.ui32AUIRptr, gstOutputStatus.ui32AUIRptr);
#endif

	// 기존 venc와 호환을 위해서 GetOutput ioctl용 이벤트 kick
	//OS_SendEvent( &gstEvent, VENC_EVENT_ES_WPTR );
	H1EncodePostDone( id );
}

#ifndef H1ENCODE_USE_NONBLOCKING_MODE
static int H1EncodeTask( void* pParam )
{
	int ret;
	int id = 0;
	H1ENCODE_T * pstEncodeInst = (H1ENCODE_T *)pParam;

	if ( pstEncodeInst == NULL )
	{
		return RET_ERROR;
	}

	id = pstEncodeInst->id;

	while(1)
	{
		if ( kthread_should_stop() )
		{
			VENC_INFO("H1EncodeTask - exit!\n");

			break;
		}

		_VsyncWait(id);

		H1EncodeDoVsync(id);
	}

	return 0;
}
#endif

static void H1EncodeDoVsync( int id )
{
	int ret;
	unsigned long flags;

	if ( gpH1Encode[id].pstEncoder != NULL )
	{
		u32 gstc = H1EncodeGetTimestamp( id );
		H1ENCODE_IPC_STATUS_T stPrepStatus = {0};

		if ( gpH1Encode[id].eStatus != H1ENCODE_STATUS_TYPE_START )
		{
			return;
		}

		H1EncodeReadIPC( id, &stPrepStatus );

		ret = H1EncodeEncode( id, &stPrepStatus, gstc );

		H1ENCODE_LOCK_IRQ(id);
		if ( ret == RET_OK )
			gpH1Encode[id].encodedCountBeforeDiscard++;
		else if ( ret == RET_TRY_AGAIN )
			gpH1Encode[id].ui32CountFrameDelay++;
		else
			gpH1Encode[id].ui32CountFrameError++;
		H1ENCODE_UNLOCK_IRQ(id);

		#ifdef H1ENCODE_DEBUG_INTERLACED_DUMP
		if ( gpH1Encode[id].stPrepStatus.frameType == LX_VENC_FRAME_TYPE_TOP ||
			gpH1Encode[id].stPrepStatus.frameType == LX_VENC_FRAME_TYPE_BOTTOM )
		{
			_DebugSaveField();
		}
		#endif
	}
	else if ( gpH1Encode[id].enableInputDetect )
	{
		H1EncodeInputDetect( id );
	}
	else if ( gpH1Encode[id].enableFramerateDetect )
	{
		H1EncodeMeasureFramerate( id );
	}

	return;
}

static void _InterruptEnableVsync( int id, BOOLEAN enable )
{
	//VENC_DEBUG("H1EncodeVsync ( enable=%s ) \n", enable ? "TRUE": "FALSE");

	VENC_REG_InterruptEnableVsync( id, enable );

#if defined(H1ENCODE_DEBUG_DUMP)
	if ( enable == FALSE )
	{
		H1EncodeDumpStop();
	}
#endif

}

#ifndef H1ENCODE_USE_NONBLOCKING_MODE

static void _VsyncWait( int id )
{
	//int ret;
	int discardCount = 0;
	unsigned long flags;

#ifdef H1ENCODE_USE_VSYNC_BUSYWAIT
	while ( 1 )
	{
		ktime_t until = ktime_set( 0, 5000000 );	// 5ms
		ret = wait_event_interruptible_hrtimeout( gpH1Encode[id].wqEncodeDone, gpH1Encode[id].ui32CountVsync != 0, until );

		if ( gpH1Encode[id].ui32CountVsync != 0 )
		{
			break;
		}
	}
#else
	wait_event_interruptible( gpH1Encode[id].wqEncodeVsync, gpH1Encode[id].ui32CountVsync != 0);
#endif

	if ( gpH1Encode[id].ui32CountVsync > 1 )
	{
		u32 before = gpH1Encode[id].ui32CountFrameSkip;
		gpH1Encode[id].ui32CountFrameSkip += (gpH1Encode[id].ui32CountVsync - 1);

		VENC_DEBUG("[%d] %d vsync skip. (Total vsync skip: %d)\n", id, gpH1Encode[id].ui32CountFrameSkip - before, gpH1Encode[id].ui32CountFrameSkip);

		if ( before < gpH1Encode[id].ui32CountFrameSkip )
		{
			discardCount = gpH1Encode[id].ui32CountFrameSkip - before;
		}
		else if ( before > gpH1Encode[id].ui32CountFrameSkip )
		{
			discardCount = gpH1Encode[id].ui32CountFrameSkip + ((UINT32)0xFFFFFFFF - before);
		}
	}

	if ( discardCount > 0 )
	{
		VENC_WARN("[%d] discardCount [%d], encodedCount [%d]\n", id, discardCount, gpH1Encode[id].encodedCountBeforeDiscard);

		H1ENCODE_LOCK_IRQ(id);
		gpH1Encode[id].encodedCountBeforeDiscard = 0;
		H1ENCODE_UNLOCK_IRQ(id);
	}

	gpH1Encode[id].ui32CountVsync = 0;
}

static void _VsyncPost( int id )
{
	//gpH1Encode[id].ui32CountVsync++;

	wake_up_interruptible( &gpH1Encode[id].wqEncodeVsync );
}

#endif

static int _EventWait( H1ENCODE_EVENT_T event, int timeout )
{
	int ret = RET_ERROR;
	u32 recvEvent;

	if ( event == 0 )
	{
		return ret;
	}

	if ( timeout == 0 )
	{
		timeout = 100;
	}

	ret = OS_RecvEvent( &gstEventH1, event, &recvEvent, OS_EVENT_RECEIVE_ANY, timeout);

	return ret;
}

static void _EventPost( H1ENCODE_EVENT_T event )
{
	if ( event == 0 )
	{
		return;
	}

	OS_SendEvent( &gstEventH1, event );
}

static void _EventClear( void )
{
	OS_ClearEvent( &gstEventH1 );
}

#if !defined(H1ENCODE_USE_POLLING)
static void H1EncodeHX280ISR( unsigned long id )
{
	// reference from hx286enc_isr() at hx280enc.c:413
	u32 irq_status;

	// 1. save irq status & clear irq status
	irq_status = HXENC_ReadIRQ( id );

	if ( irq_status & 0x01 )
	{
		// 2. Clear IRQ and sllice ready interrupt bit
		HXENC_WriteIRQ( id, irq_status & (~0x101) );

		// Handle slice ready interrupts. The reference implementation
		// doesn't signal slice ready interrupts to EWL.
		// The EWL will poll the slices ready register value.
		if ( (irq_status & 0x1FE) == 0x100)
		{
			// Slice ready IRQ handled
			return;
		}

		// 2. kick event
		HXENC_WakeupIRQ( id );

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
		if ( gpH1Encode[id].pstEncoder != NULL )
		{
#if 0
			H1EncodeEncodeDone(id);
#else
			if ( id == 0 )
				tasklet_schedule(&VENC_TASKLET_H1ENC_0);
			else if ( id == 1 )
				tasklet_schedule(&VENC_TASKLET_H1ENC_1);
#endif
		}
#endif

#ifdef H1ENCODE_USE_LOGFILE
		do_gettimeofday(&_irqTime);
#endif
	}
}
#endif

#ifdef H1ENCODE_DEBUG_CHECK_VSYNC
static UINT32 jiffies_vsync;
#endif

static void H1EncodeVsyncHandler( unsigned long id )
{
#ifdef H1ENCODE_DEBUG_CHECK_VSYNC
	UINT32 jiffies_cur = jiffies;
#endif

#ifdef H1ENCODE_ENABLE_DROP_FIRST_FRAME
	if ( gpH1Encode[id].remain_drop > 0 )
	{
		gpH1Encode[id].remain_drop--;
		return;
	}
#endif
	// the number of overlaped vsync
	gpH1Encode[id].ui32CountVsync++;

#ifdef H1ENCODE_USE_NONBLOCKING_MODE
	H1EncodeDoVsync(id);
#else
	// Raise vsync interrupt
	_VsyncPost(id);
	//H1EncodeDoVsync();
#endif

	gpH1Encode[id].ui32TotalVsync++;

#ifdef H1ENCODE_DEBUG_CHECK_VSYNC
	if ( gpH1Encode[id].stInput.frameRateNum > 0 && gpH1Encode[id].stInput.frameRateDenom > 0 )
	{
		UINT32 interval = (jiffies_cur - jiffies_vsync);
		UINT32 expectedInterval = (gpH1Encode[id].stInput.frameRateDenom * 1000 / gpH1Encode[id].stInput.frameRateNum );

		if ( interval < expectedInterval - 1 )
		{
			VENC_WARN("Too short vsync interval: %d\n", interval);
		}
		else if ( interval > expectedInterval + 1 )
		{
			VENC_WARN("Too long vsync interval: %d\n", interval);
		}
	}

	jiffies_vsync = jiffies_cur;
#endif

}

int H1EncodeISRHandler( int id )
{
//	BOOLEAN bVsync = FALSE;
	u32 status;
	int ret = IRQ_NONE;

//	TRACE_ENTER();
	status = VENC_REG_InterruptRead( id );

	if ( status )
	{
		VENC_REG_InterruptClear( id );

		if ( status & 0x1 || status & 0x2 )	// vdec_intr_st, de_intr_st
		{
			if ( status & 0x2 )
			{
				VENC_REG_CopyExternalRegs( id );
			}

			//VENC_REG_PrintIPCStatus(id);

			H1EncodeUpdateTimestamp(id);
			H1EncodeUpdateIPC(id);

			H1EncodeVsyncHandler(id);

#ifdef SUPPORT_VENC_DEVICE_FASYNC_FOPS
			VENC_KillFasync( SIGUSR1 );
#endif
			ret = IRQ_HANDLED;
		}

#if !defined(H1ENCODE_USE_POLLING)
		if ( status & 0x4 )	// intr_h1enc
		{
			H1EncodeHX280ISR( id );

			ret = IRQ_HANDLED;
		}
#endif
	}

	return ret;
}

int H1EncodeISRHandler0 ( void )
{
	return H1EncodeISRHandler( 0 );
}

int H1EncodeISRHandler1 ( void )
{
	return H1EncodeISRHandler( 1 );
}

#ifdef SUPPORT_VENC_DEVICE_ENC_API
#define USE_ENC_API_LOCK

#ifdef USE_ENC_API_LOCK
#define ENC_API_LOCK()		spin_lock_irqsave(&handle->lock, flags)
#define ENC_API_UNLOCK()	spin_unlock_irqrestore(&handle->lock, flags)
#else
#define ENC_API_LOCK()
#define ENC_API_UNLOCK()
#endif

typedef struct {
	int			id;
	void*		inst;
#ifdef USE_ENC_API_LOCK
	spinlock_t	lock;
#endif

	BOOLEAN		isFirst;
	UINT8		headerData[64];
	UINT32		headerLength;

	UINT32		width;
	UINT32		height;
	UINT32		framerate;
	UINT32		gopLength;
	UINT32		bitrate;

	LX_VENC_IMAGE_TYPE_T	imageType;
	LX_VENC_ENCODE_TYPE_T	codecType;
	BOOLEAN		interlaced;
	BOOLEAN		topFieldFirst;

	UINT32		codedFrameCount;
	UINT32		intraPreiodCount;

	BOOLEAN		memallocChanged;		//  For DEBUG
} ENC_CTX_T;

ENC_CTX_T	enc_ctx_list[] = {
	[0] = {
		.id = -1,
	},
	[1] = {
		.id = -1,
	}
};

static ENC_CTX_T* _ENC_CtxNew(int id, LX_VENC_ENCODE_TYPE_T codecType)
{
	ENC_CTX_T* handle = &enc_ctx_list[id];

	if ( handle->id != -1 )
	{
		return NULL;
	}

	handle->id = id;
	handle->isFirst = TRUE;
	handle->codecType = codecType;

#ifdef USE_ENC_API_LOCK
	spin_lock_init(&handle->lock);
#endif

	//enc_ctx_list[0].inst = NULL;

	return handle;
}

static ENC_CTX_T* _ENC_CtxGet(int id)
{
	ENC_CTX_T* handle = &enc_ctx_list[id];

	if ( handle->id != -1)
	{
		return handle;
	}
	else
	{
		return NULL;
	}
}

static int _ENC_CtxDelete(ENC_CTX_T* enc_ctx)
{
	if (enc_ctx == NULL)
	{
		return RET_INVALID_PARAMS;
	}

	enc_ctx->id = -1;
	enc_ctx->inst = NULL;

	return RET_OK;
}

int ENC_H264Create(ENC_CTX_T *handle)
{
	HXENC_RET_T		ret;
	HXENC_CONFIG_T	config = {};
	HXENC_PREPROCESSING_CFG_T preCfg = {};
	HXENC_RATECTRL_T rcCfg = {};
	HXENC_CODINGCTRL_T codingCfg = {};

	VENC_TRACE("START\n");

	if ( handle == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	config.width = handle->width;
	config.height = handle->height;
	config.stride = handle->width;
	config.eFrameType = handle->interlaced ? 1 : HXENC_FRAME_TYPE_PROGRESSIVE;
	config.frameRateDenom = 1;
	config.frameRateNum = handle->framerate * config.frameRateDenom;
	config.coreid = handle->id;

	if ((ret = HXENC_H264Init( &config, (HXENC_H264INST_T *)&handle->inst )) != HXENC_OK )
	{
		VENC_ERROR("HXENC_H264Init - failed.(%d)\n", ret);
		goto func_error;
	}

	preCfg.inputWidth = handle->width;
	if ( handle->interlaced )
		preCfg.inputHeight = handle->height / 2;
	else
		preCfg.inputHeight = handle->height;
	preCfg.inputType = handle->imageType;
	preCfg.videoStabilization = 0;

	if((ret = HXENC_H264SetPreProcessing(handle->inst, &preCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetPreProcessing - failed.(%d)\n", ret);
		goto func_error;
	}

	if ( handle->gopLength > 0 )
	{
		rcCfg.bEnableGOPLength = TRUE;
		rcCfg.gopLength = handle->gopLength;
		//rcCfg.bitPerSecond = handle->bitrate;
	}

	if(( ret = HXENC_H264SetRateCtrl(handle->inst, &rcCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetRateCtrl - failed.(%d)\n", ret);
		goto func_error;
	}

	if ( handle->interlaced )
	{
		codingCfg.fieldOrder = handle->topFieldFirst ? 1 : 0;

		if((ret = HXENC_H264SetCodingCtrl(handle->inst, &codingCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264SetCodingCtrl - failed.(%d)\n", ret);
			goto func_error;
		}
	}

	VENC_TRACE("END\n");
	return RET_OK;

func_error:
	if ( handle != NULL && handle->inst != NULL )
	{
		HXENC_H264Release(handle->inst);
		handle->inst = NULL;
	}

	return RET_ERROR;
}

int ENC_H264Destroy(ENC_CTX_T *		handle)
{
	HXENC_RET_T		ret = HXENC_ERROR;

	if ( handle == NULL || handle->inst == NULL )
	{
		return RET_ERROR;
	}

	ret = HXENC_H264Release(handle->inst);

	return ret;
}

int ENC_H264Encode(ENC_CTX_T *handle, LX_VENC_ENC_ENCODE_T *pstEncode)
{
	HXENC_RET_T		ret = HXENC_ERROR;
	HXENC_INPUT_T		encIn = {};
	HXENC_H264_OUTPUT_T encOut = {};

	UINT32	bus_output = 0;
	UINT32	out_buf_size = 0;
	UINT32	stream_size = 0;

	unsigned long		flags;

	if (handle == NULL || handle->inst == NULL)
	{
		VENC_ERROR("handle or inst is NULL\n");
		return RET_ERROR;
	}

	if (pstEncode == NULL)
	{
		VENC_ERROR("pstEncode is NULL\n");
		return RET_ERROR;
	}

	ENC_API_LOCK();

	bus_output = pstEncode->bus_output;
	out_buf_size = pstEncode->out_buf_size;

	if (handle->isFirst)
	{
		encIn.pOutBuf = (u32 *)handle->headerData;
		encIn.outBufSize = sizeof(handle->headerData);

		ret = HXENC_H264StrmStart(handle->inst, &encIn, &encOut);

		if ( ret != HXENC_OK )
		{
			ENC_API_UNLOCK();
			VENC_ERROR("Error - HXENC_H264StrmStart\n");
			return RET_ERROR;
		}

		memcpy(handle->headerData, encIn.pOutBuf, encOut.streamSize);
		handle->headerLength = encOut.streamSize;

		memset(&encIn, 0x0, sizeof(encIn));
		memset(&encOut, 0x0, sizeof(encOut));

		handle->isFirst = FALSE;
		encIn.codingType	= HXENC_INTRA_FRAME;
		encIn.timeIncrement = 0;
	}
	else
	{
		encIn.codingType	= HXENC_PREDICTED_FRAME;
		encIn.timeIncrement = pstEncode->timeIncrement;
	}

	if ( handle->gopLength != 0 && (handle->intraPreiodCount >= handle->gopLength) )
	{
		encIn.codingType	= HXENC_INTRA_FRAME;
	}

	if ( encIn.codingType == HXENC_INTRA_FRAME )
	{
		void * pOutBuf = NULL;

		handle->intraPreiodCount = 0;

		pOutBuf = ioremap(bus_output, out_buf_size);

		if ( pOutBuf != NULL )
		{
			memset(pOutBuf, 0x0, sizeof(handle->headerData));
			memcpy(pOutBuf, handle->headerData, sizeof(handle->headerData));
			iounmap(pOutBuf);

			bus_output += sizeof(handle->headerData);
			out_buf_size -= sizeof(handle->headerData);
			stream_size += sizeof(handle->headerData);
		}
	}

	encIn.busLuma		= pstEncode->bus_luma;
	encIn.busChromaU	= pstEncode->bus_chroma_u;
	encIn.busChromaV	= pstEncode->bus_chroma_v;

	encIn.busOutBuf 	= bus_output;
	encIn.outBufSize	= out_buf_size;

	VENC_TRACE("bus_output=0x%08X, out_buf_size=%d\n", encIn.busOutBuf, encIn.outBufSize);

	encIn.pOutBuf = ioremap( encIn.busOutBuf, encIn.outBufSize );

	ENC_API_UNLOCK();

	ret = HXENC_H264StrmEncode( handle->inst, &encIn, &encOut );

	ENC_API_LOCK();

	iounmap( encIn.pOutBuf );

	if ( ret != HXENC_OK && ret != HXENC_FRAME_READY )
	{
		goto func_exit;
	}

	handle->codedFrameCount++;
	handle->intraPreiodCount++;

	stream_size += encOut.streamSize;
	pstEncode->stream_size = stream_size;
	pstEncode->codingType = encOut.codingType;

func_exit:
	ENC_API_UNLOCK();
	return ret;
}

int ENC_VP8Create(ENC_CTX_T *handle)
{
	HXENC_RET_T		ret;
	HXENC_CONFIG_T	config = {};
	HXENC_PREPROCESSING_CFG_T preCfg = {};
	HXENC_RATECTRL_T rcCfg = {};
	HXENC_CODINGCTRL_T codingCfg = {};

	VENC_TRACE("START\n");

	if ( handle == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	config.width = handle->width;
	config.height = handle->height;
	config.stride = handle->width;
	config.eFrameType = handle->interlaced ? 1 : HXENC_FRAME_TYPE_PROGRESSIVE;
	config.frameRateDenom = 1;
	config.frameRateNum = handle->framerate * config.frameRateDenom;
	config.coreid = handle->id;

	if ((ret = HXENC_VP8Init( &config, (HXENC_VP8INST_T *)&handle->inst )) != HXENC_OK )
	{
		VENC_ERROR("HXENC_H264Init - failed.(%d)\n", ret);
		goto func_error;
	}

	preCfg.inputWidth = handle->width;
	if ( handle->interlaced )
		preCfg.inputHeight = handle->height / 2;
	else
		preCfg.inputHeight = handle->height;
	preCfg.inputType = handle->imageType;
	preCfg.videoStabilization = 0;

	if((ret = HXENC_VP8SetPreProcessing(handle->inst, &preCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_H264SetPreProcessing - failed.(%d)\n", ret);
		goto func_error;
	}

#if 0
	if ( handle->gopLength > 0 )
	{
		rcCfg.bEnableGOPLength = TRUE;
		rcCfg.gopLength= handle->gopLength;
	}

	if((ret = HXENC_VP8SetRateCtrl((HXENC_VP8INST_T)handle->inst, &rcCfg)) != HXENC_OK)
	{
		VENC_ERROR("HXENC_VP8SetRateCtrl - failed.(%d)\n", ret);
		goto func_error;
	}
#endif

	if ( handle->interlaced )
	{
		codingCfg.fieldOrder = handle->topFieldFirst ? 1 : 0;

		if((ret = HXENC_VP8SetCodingCtrl(handle->inst, &codingCfg)) != HXENC_OK)
		{
			VENC_ERROR("HXENC_H264SetCodingCtrl - failed.(%d)\n", ret);
			goto func_error;
		}
	}

	VENC_TRACE("END\n");
	return RET_OK;

func_error:
	if ( handle != NULL && handle->inst != NULL )
	{
		HXENC_VP8Release(handle->inst);
		handle->inst = NULL;
	}

	return RET_ERROR;
}

int ENC_VP8Destroy(ENC_CTX_T *		handle)
{
	HXENC_RET_T		ret = HXENC_ERROR;

	if ( handle == NULL || handle->inst == NULL )
	{
		return RET_ERROR;
	}

	ret = HXENC_VP8Release(handle->inst);

	return ret;
}

int ENC_VP8Encode(ENC_CTX_T *handle, LX_VENC_ENC_ENCODE_T *pstEncode)
{
	HXENC_RET_T		ret = HXENC_ERROR;
	HXENC_INPUT_T		encIn = {};
	HXENC_VP8_OUTPUT_T	encOut = {};

	UINT32	stream_size = 0;
	UINT32	offset = 0;
	void *stream = NULL;

	unsigned long		flags;
	int i = 0;

	if (handle == NULL || handle->inst == NULL)
	{
		VENC_ERROR("handle or inst is NULL\n");
		return RET_ERROR;
	}

	if (pstEncode == NULL)
	{
		VENC_ERROR("pstEncode is NULL\n");
		return RET_ERROR;
	}

	ENC_API_LOCK();

	if (handle->isFirst)
	{
		encIn.codingType	= HXENC_INTRA_FRAME;
		handle->isFirst 	= FALSE;
	}
	else
	{
		encIn.codingType	= HXENC_PREDICTED_FRAME;
	}

	if ( handle->gopLength != 0 && (handle->intraPreiodCount >= handle->gopLength) )
	{
		encIn.codingType	= HXENC_INTRA_FRAME;
	}

	if ( encIn.codingType == HXENC_INTRA_FRAME )
	{
		handle->intraPreiodCount = 0;
	}

	encIn.busLuma		= pstEncode->bus_luma;
	encIn.busChromaU	= pstEncode->bus_chroma_u;
	encIn.busChromaV	= pstEncode->bus_chroma_v;

	encIn.busOutBuf 	= pstEncode->bus_output;
	encIn.outBufSize	= pstEncode->out_buf_size;

	VENC_TRACE("codingType=%d, bus_output=0x%08X, out_buf_size=%d\n",
		encIn.codingType, encIn.busOutBuf, encIn.outBufSize);

	encIn.pOutBuf		= ioremap( encIn.busOutBuf, encIn.outBufSize );

	ENC_API_UNLOCK();

	ret = HXENC_VP8StrmEncode( handle->inst, &encIn, &encOut );

	ENC_API_LOCK();

	if ( ret != HXENC_OK && ret != HXENC_FRAME_READY )
	{
		goto func_exit;
	}

	handle->codedFrameCount++;
	handle->intraPreiodCount++;

	for ( i = 0; i < 9; i++ )
	{
		if ( encOut.streamSize[i] > 0 )
		{
			stream_size += encOut.streamSize[i];
		}
	}

	stream = kmalloc(stream_size, GFP_KERNEL);

	// copy to tmp buffer
	for ( i = 0; i < 9; i++ )
	{
		if ( encOut.streamSize[i] > 0 )
		{
			memcpy( stream + offset, encOut.pOutBuf[i], encOut.streamSize[i] );
			offset += encOut.streamSize[i];

			VENC_TRACE("encOut={ .streamSize[%d]=%d }\n", i, encOut.streamSize[i]);
		}
	}

	VENC_TRACE("encOut={ .frameSize=%d }\n", encOut.frameSize);

	// copy to result buffer
	memcpy( encIn.pOutBuf, stream, stream_size );
	pstEncode->stream_size = stream_size;
	pstEncode->codingType = encOut.codingType;

func_exit:
	if ( stream != NULL )
	{
		kfree(stream);
	}
	iounmap( encIn.pOutBuf );

	ENC_API_UNLOCK();

	return ret;
}

int ENC_Create(int id, LX_VENC_ENC_CREATE_T *pstCreate)
{
	int ret = RET_ERROR;
	ENC_CTX_T *		handle = NULL;

	if ( pstCreate == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	handle = _ENC_CtxNew(id, pstCreate->codecType);
	if ( handle == NULL )
	{
		VENC_ERROR("Can't allocate new context\n");
		return RET_ERROR;
	}

	handle->width 		= pstCreate->width;
	handle->height 		= pstCreate->height;
	handle->imageType	= pstCreate->imageType;
	handle->framerate 	= pstCreate->framerate;
	handle->gopLength 	= pstCreate->gopLength;
	handle->bitrate 	= pstCreate->bitrate;
	handle->interlaced 	= pstCreate->interlaced;
	handle->topFieldFirst = pstCreate->topFieldFirst;

#if 0
	if ( pstCreate->memallocStart > 0 && pstCreate->memallocEnd > 0 )
	{
		// must use for debug only
		handle->memallocChanged = TRUE;
		HXENC_MemallocInit( pstCreate->memallocStart, pstCreate->memallocEnd );
	}
#endif

	if ( handle->codecType == LX_VENC_ENCODE_TYPE_VP8 )
	{
		ret = ENC_VP8Create(handle);
	}
	else
	{
		ret = ENC_H264Create(handle);
	}

	// return handle id
	pstCreate->id = handle->id;

	return ret;
}

int ENC_Destroy(int id, LX_VENC_ENC_DESTROY_T *pstDestroy)
{
	int ret = RET_ERROR;
	ENC_CTX_T *		handle = NULL;

	if ( pstDestroy == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	handle  = _ENC_CtxGet(pstDestroy->id);

	if ( handle == NULL )
	{
		return RET_ERROR;
	}

	if ( handle->codecType == LX_VENC_ENCODE_TYPE_VP8 )
	{
		ret = ENC_VP8Destroy(handle);
	}
	else
	{
		ret = ENC_H264Destroy(handle);
	}

	_ENC_CtxDelete(handle);

#if 0
	if ( handle->memallocChanged )
	{
		HXENC_MemallocInit( gpstVencMemConfig->uiH1EncBufBase, gpstVencMemConfig->uiH1EncBufBase + gpstVencMemConfig->uiH1EncBufSize);
	}
#endif

	return ret;
}

int ENC_Encode(int id, LX_VENC_ENC_ENCODE_T *pstEncode)
{
	int ret = RET_ERROR;
	ENC_CTX_T *		handle = NULL;
	unsigned long	flags;

	if ( pstEncode == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	handle = _ENC_CtxGet(pstEncode->id);

	if ( handle == NULL )
	{
		VENC_ERROR("handle is NULL\n");

		return RET_ERROR;
	}

	ENC_API_LOCK();

	H1EncodeSetTime( id, 0 );

	ENC_API_UNLOCK();

	if ( handle->codecType == LX_VENC_ENCODE_TYPE_VP8 )
	{
		ret = ENC_VP8Encode(handle, pstEncode);
	}
	else
	{
		ret = ENC_H264Encode(handle, pstEncode);
	}

	ENC_API_LOCK();
	pstEncode->duration = H1EncodeGetTime( id );
	ENC_API_UNLOCK();

	return ret;

}

#endif

