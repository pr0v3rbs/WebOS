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



/** @file	audio_kdrv_decoder.c
 *
 *  main driver implementation for  audio decoder device.
 *  audio decoder device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.25
 *  note		Additional information.
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
#include "audio_cfg.h"
#include "audio_drv.h"

#include "audio_imc_func.h"
#include "audio_buffer.h"
#include "common/adec_media_type.h"
#include "buffer/adec_buf_helper.h"

#include "audio_drv_decoder.h"
#include "audio_drv_master.h"
#include "audio_drv_manager.h"
#include "audio_drv_renderer.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* define WMA and WMAPro codec parameters */
/****************************************************************
*	Format Tag
*****************************************************************
*	0x0160: WMA version1
*	0x0161:	WMA v7, v8, v9 (WMA version2)
*	0x0162:	WMA Professional (WMA version3)
*	0x0163:	WMA Lossless
****************************************************************/

/****************************************************************
*	DSI (Decoder Specific Info) of WMA
*****************************************************************
*	WMA1
*	size: 4 bytes
*	Samples Per Block(2bytes) + Encode Options(2bytes)
*****************************************************************
*	WMA2
*	size: 10 bytes
*	Samples Per Block(4bytes) + Encode Options(2bytes) + Super Block Align(4bytes)
*****************************************************************
*	WMA3 - WMA Pro(v10, Lossless)
*	size: 18 bytes
*	Samples Per Block(2bytes) + Channel Mask(4bytes) + Advanced Encode Options2(8bytes) + Encode Option(2bytes) + Advanced Encode Option(2bytes)
*	// Valid Bits Per Sample(2bytes) + Channel Mask(4bytes) + reserved1(4bytes) + reserved2(4bytes) + Encode Options(2bytes) + reserved3(2bytes)
****************************************************************/
#define LENGTH_OF_DSI_FOR_WMA1			4
#define LENGTH_OF_DSI_FOR_WMA2			10
#define LENGTH_OF_DSI_FOR_WMA3			18

#define WMA1_FORMAT_TAG					0x160
#define WMA2_FORMAT_TAG					0x161
#define WMAPRO_FORMAT_TAG				0x162
#define WMALOSSLESS_FORMAT_TAG			0x163

#define ADEC_READ16_LE(buf)				(buf)[0] | ((UINT16)(buf)[1]<<8)
#define ADEC_READ24_LE(buf)				(buf)[0] | ((UINT32)(buf)[1]<<8) | ((UINT32)(buf)[2]<<16)
#define ADEC_READ32_LE(buf)				(buf)[0] | ((UINT32)(buf)[1]<<8) | ((UINT32)(buf)[2]<<16) | ((UINT32)(buf)[3]<<24)
#define ADEC_READ64_LE(buf)				ADEC_READ32_LE((buf)) | ((UINT64)ADEC_READ32_LE(&(buf)[4]))<<32

#define ADEC_READ16_BE(buf)				((UINT16)(buf)[0]<<8) | (buf)[1]
#define ADEC_READ24_BE(buf)				((UINT32)(buf)[0]<<16) | ((UINT32)(buf)[1]<<8) | (buf)[2]
#define ADEC_READ32_BE(buf)				((UINT32)(buf)[0]<<24) | ((UINT32)(buf)[1]<<16) | ((UINT32)(buf)[2]<<8) | (buf)[3]
#define ADEC_READ64_BE(buf)				(((UINT64)ADEC_READ32_BE((buf)))<<32) | ADEC_READ32_BE(&(buf)[4])

/* define RA8 codec parameters */
#define MIN_LENGTH_OF_DSI_FOR_RA8		8
#define MIN_LENGTH_OF_DSI_FOR_RA8_V13	16
#define RA8_MAJOR_V1					1
#define RA8_MINOR_V3					3

#define MAX_LENGTH_OF_DSI				20

OS_SEM_T	_gpDecSema;		// decoder semaphore
BOOLEAN		_gpDecSemaInit = FALSE;

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define AUD_DEC_LOCK_INIT()		OS_InitMutex(&_gpDecSema, OS_SEM_ATTR_DEFAULT)
#define AUD_DEC_LOCK()			OS_LockMutex(&_gpDecSema)
#define AUD_DEC_UNLOCK()		OS_UnlockMutex(&_gpDecSema)

#ifdef USE_BUF_LOCK
#define AUD_DEC_BUF_LOCK()		OS_LockMutex(&_gpDecSema)
#define AUD_DEC_BUF_UNLOCK()	OS_UnlockMutex(&_gpDecSema)
#else
#define AUD_DEC_BUF_LOCK()
#define AUD_DEC_BUF_UNLOCK()
#endif
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
AUD_DECODER_INFO_T		_gDecoderInfo[DEV_DEC_NUM];
UINT8					_gDecoderDSI[DEV_DEC_NUM][MAX_LENGTH_OF_DSI];

BOOLEAN					_gbUseReformatter = TRUE;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int MIXED_IP_AUDIO_ResetAADClockRegisters( void );

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_AUD_CLOCK_SRC_T	g_setClockSrc;	// DTO Rate
extern UINT32 g_DuringReset;

// For Main Decoder Index for Clock Setting and SPDIF ES Output
extern AUD_DECODER_ID_T _gMainDecoderIndex;
extern LX_AUD_KDRV_T	g_AudKdrv;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static SINT32 AUDIO_InitDecoder(UINT32 allocDev);
static SINT32 AUDIO_StartDecoder(UINT32 allocDev);
static SINT32 AUDIO_StopDecoder(UINT32 allocDev);
static SINT32 AUDIO_FlushDecoder(UINT32 allocDev);
static SINT32 AUDIO_SetDecoderOutMode(UINT32 allocDev, LX_AUD_DECODER_OUT_MODE_T outMode);
static SINT32 AUDIO_SetDecoderParam(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam);
static SINT32 AUDIO_FeedDec(UINT32 allocDev, LX_AUD_DECODER_FEED_T *pDecFeed);
static SINT32 AUDIO_GetDecodedInfo(UINT32 allocDev, LX_AUD_DECODED_INFO_T *pDecodedInfo);
static SINT32 AUDIO_IsDecodable(UINT32 allocDev, UINT32 *pIsDecodable);
static SINT32 AUDIO_CloseDecoder(UINT32 allocDev);
static SINT32 AUDIO_SetDecoderMode(UINT32 allocDev, LX_AUD_DECODER_MODE_T *pDecMode);
static SINT32 AUDIO_GetDecodedStatus(UINT32 allocDev, LX_AUD_DECODED_STATUS_T *pDecodedStatus);
static SINT32 AUDIO_SetDecoderCertiParam(UINT32 allocDev, UINT32 decCertiParam);
static void _AUDIO_RegisterDecoderEvent(UINT32 allocDev);
static SINT32 AUDIO_SetDecoderParamExt(UINT32 allocDev, LX_AUD_DECODER_PARAM_EXT_T *pDecParamExt);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
UINT32					_gDecoderCheckSum = 0;


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * open handler for audio decoder device
 *
 */
SINT32	KDRV_AUDIO_OpenDecoder(struct inode *inode, struct file *filp)
{
	ULONG						flags;
	SINT32						decIndex = 0;
	BOOLEAN						bBufferRepeat;		//The status of buffer repeat is set previously.

	LX_AUD_DEV_TYPE_T			devType = LX_AUD_DEV_TYPE_DEC;
	LX_AUD_DEV_T				allocDev = LX_AUD_DEV_NONE;
	LX_AUD_EVENT_TYPE_T			*pSetEventType = NULL;
	LX_AUD_EVENT_TYPE_T			*pGetEventType = NULL;

	AUD_DEVICE_T				*pDev = NULL;
	AUD_DECODER_INFO_T			*pDecInfo = NULL;


	if(_gpDecSemaInit == TRUE)
		AUD_DEC_LOCK();

	filp->private_data = kmalloc(sizeof(AUD_DEVICE_T), GFP_KERNEL);
	pDev = (AUD_DEVICE_T*)filp->private_data;

	/* Set device */
	pDev->devType = devType;

	// to fix audio decoder not working
	_gDecoderCheckSum++;
	pDev->checksum = _gDecoderCheckSum;

	allocDev = AUDIO_AllocDev(devType);
	if(allocDev == LX_AUD_DEV_NONE)
	{
		kfree(filp->private_data);
		if(_gpDecSemaInit == TRUE)
			AUD_DEC_UNLOCK();
		AUD_KDRV_ERROR("devType[%d] is not available \n", devType);
		return RET_ERROR;
	}

	pDev->allocDev = allocDev;
	filp->private_data = pDev;

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		AUDIO_FreeDev(allocDev);
		kfree(filp->private_data);
		if(_gpDecSemaInit == TRUE)
			AUD_DEC_UNLOCK();
		return RET_ERROR;
	}

	pDecInfo = &(_gDecoderInfo[decIndex]);

	//Copy a previous buffer repeat variable
	bBufferRepeat = pDecInfo->bBufferRepeat;

	//Clears a all decoder information.
	memset(&pDecInfo->decParam, 0, sizeof(LX_AUD_DECODER_PARAM_T));
	memset(&pDecInfo->decParamExt, 0, sizeof(LX_AUD_DECODER_PARAM_EXT_T));
	memset(&pDecInfo->decMode, 0, sizeof(LX_AUD_DECODER_MODE_T));
	memset(&pDecInfo->decodedInfo, 0, sizeof(LX_AUD_DECODED_INFO_T));
	memset(&pDecInfo->decodedStatus, 0, sizeof(LX_AUD_DECODED_STATUS_T));
	memset(&pDecInfo->decEvent, 0, sizeof(AUD_EVENT_T)*AUD_EVENT_NUM);
	memset(&pDecInfo->ui64Timestamp, 0, sizeof(UINT64)*CPB_AUI_INDEX_COUNT);

	pDecInfo->ui32AllocDev 		= allocDev;
	pDecInfo->status			= AUD_STATUS_NONE;
	pDecInfo->bClosed 			= FALSE;
	pDecInfo->bResetting 		= FALSE;
	pDecInfo->decIsDecodable	= FALSE;
	pDecInfo->ui32EventNum		= 0;
	pDecInfo->bBufferRepeat 	= bBufferRepeat;
	pDecInfo->ui32ChannelNum	= 0;
	pDecInfo->bSpdifESAvail		= FALSE;
	pDecInfo->codecDetailType	= LX_AUD_CODEC_DETAIL_NONE;

	// initialize decoder mode
	pDecInfo->decMode.drc 	  	= LX_AUD_DECODER_DRC_LINE;
	pDecInfo->decMode.downmix   = LX_AUD_DECODER_DOWNMIX_LORO;
	pDecInfo->decMode.aacBypass = LX_AUD_DECODER_AAC_BYPASS_OFF;
	pDecInfo->decMode.tpTimerInterval = 0x2000;

	// initialize decoded information
	pDecInfo->decodedInfo.samplingFreq     = LX_AUD_SAMPLING_FREQ_NONE;
	pDecInfo->decodedInfo.ui32BitPerSample = 0;
	pDecInfo->decodedInfo.esInfo.adecFormat= LX_AUD_CODEC_UNKNOWN;
	pDecInfo->decodedInfo.esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;

	// to fix audio decoder not working
	pDecInfo->ui32CheckSum = _gDecoderCheckSum;

	//spin lock for protection
	spin_lock_irqsave(&gAudEventSpinLock, flags);

	//Reset a audio SET event type for next event.
	pSetEventType = &gAudSetEvent[allocDev];
	pSetEventType->allocDev = LX_AUD_DEV_NONE;
	pSetEventType->eventMsg = LX_AUD_EVENT_MASK_NONE;

	//Reset a audio GET event type for next event.
	pGetEventType = &gAudGetEvent[allocDev];
	pGetEventType->allocDev = LX_AUD_DEV_NONE;
	pGetEventType->eventMsg = LX_AUD_EVENT_MASK_NONE;

	//spin unlock for protection
	spin_unlock_irqrestore(&gAudEventSpinLock, flags);

	if(_gpDecSemaInit == TRUE)
		AUD_DEC_UNLOCK();

	//Initialize AUDIO Decoder semaphore
	if(_gpDecSemaInit == FALSE)
	{
		AUD_DEC_LOCK_INIT();
		_gpDecSemaInit = TRUE;
	}

	AUD_KDRV_PRINT("Open Decoder device %d\n", allocDev);

	return RET_OK;
}

/**
 * close handler for audio decoder device
 *
 */
SINT32	KDRV_AUDIO_CloseDecoder(struct inode *inode, struct file *filp)
{
	SINT32				retVal = 0;
	SINT32				decIndex = 0;

	AUD_DEVICE_T		*pDev = NULL;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;
	LX_AUD_DEV_T		allocDev = LX_AUD_DEV_NONE;

	pDev = (AUD_DEVICE_T*)filp->private_data;
	if(pDev == NULL)
	{
		AUD_KDRV_ERROR("private_data is NULL\n");
		return RET_ERROR;
	}

	allocDev = pDev->allocDev;
	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	pDecInfo = &(_gDecoderInfo[decIndex]);

	// to fix audio decoder not working
	if(pDecInfo->ui32CheckSum == pDev->checksum)
	{
		retVal = AUDIO_CloseDecoder(allocDev);
		if(retVal != RET_OK)
			AUD_KDRV_PRINT("AUDIO_CloseDecoder() failed or already closed.\n");
	}
	else
	{
		AUD_KDRV_INFO("checksums are different(%d.%d)\n",
			pDecInfo->ui32CheckSum, pDev->checksum);
	}

	kfree(pDev);

	AUD_KDRV_PRINT("Close Decoder device %d\n", allocDev);

	return retVal;
}


/**
 * ioctl handler for audio decoder device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static SINT32 KDRV_AUDIO_IoctlDecoder (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
long KDRV_AUDIO_IoctlDecoder(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	SINT32				retVal = 0;
	SINT32				err = 0;
	AUD_DEVICE_T		*pDev = NULL;
	UINT32				allocDev;

	pDev = (AUD_DEVICE_T*)filp->private_data;
	if(pDev == NULL)
		return RET_ERROR;

	allocDev = pDev->allocDev;

	/*
	* check if IOCTL command is valid or not.
	* - if magic value doesn't match, return error (-ENOTTY)
	* - if command is out of range, return error (-ENOTTY)
	*
	* note) -ENOTTY means "Inappropriate ioctl for device.
	*/
	if (_IOC_TYPE(cmd) != AUD_DEC_IOC_MAGIC)
	{
		DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > AUD_DEC_IOC_MAXNR)
	{
		DBG_PRINT_WARNING("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
		return -ENOTTY;
	}

	/*
	* check if user memory is valid or not.
	* if memory can't be accessed from kernel, return error (-EFAULT)
	*/
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err)
	{
		DBG_PRINT_WARNING("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
							_IOC_NR(cmd),
							(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
							(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
							(void*)arg );
		return -EFAULT;
	}

	AUD_KDRV_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd) );

	switch(cmd)
	{
		case AUD_DEC_IO_START:
		{
			AUD_KDRV_TRACE("[AUD_DEC_IOW_START] Called\n");
			retVal = AUDIO_StartDecoder(allocDev);
		}
		break;

		case AUD_DEC_IO_STOP:
		{
			AUD_KDRV_TRACE("[AUD_DEC_IOW_STOP] Called\n");
			retVal = AUDIO_StopDecoder(allocDev);
		}
		break;

		case AUD_DEC_IO_FLUSH:
		{
			AUD_KDRV_TRACE("[AUD_DEC_IOW_FLUSH] Called\n");
			retVal = AUDIO_FlushDecoder(allocDev);
		}
		break;

		case AUD_DEC_IOW_SET_OUT_MODE:
		{
			LX_AUD_DECODER_OUT_MODE_T	decOutMode;

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_OUT_MODE] Called\n");

			if ( copy_from_user(&decOutMode, (void __user *)arg, sizeof(LX_AUD_DECODER_OUT_MODE_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_OUT_MODE] copy_from_user() Err\n");
				return RET_ERROR;
			}

			retVal = AUDIO_SetDecoderOutMode(allocDev, decOutMode);
		}
		break;

		case AUD_DEC_IOW_FEED:
		{
			LX_AUD_DECODER_FEED_T		decFeedData;

			if ( copy_from_user(&decFeedData, (void __user *)arg, sizeof(LX_AUD_DECODER_FEED_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_FEED] copy_from_user() Err\n");
				return RET_ERROR;
			}

			retVal = AUDIO_FeedDec(allocDev, &decFeedData);
			AUD_KDRV_TRACE("[AUD_DEC_IOW_FEED] bufSize=%u, bufStatus=%d, TS=%llu\n", decFeedData.ui32BufferSize, decFeedData.statusBuffer, decFeedData.ui64TimeStamp);
		}
		break;

		case AUD_DEC_IOW_SET_PARAM:
		{
			LX_AUD_DECODER_PARAM_T		decParam;

			if ( copy_from_user(&decParam, (void __user *)arg, sizeof(LX_AUD_DECODER_PARAM_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_DECODING] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_DECODING] codec =%d\n", decParam.codecType);
			retVal = AUDIO_SetDecoderParam(allocDev, &decParam);
		}
		break;

		case AUD_DEC_IOW_GET_KDRV_HANDLE:
		{
			AUD_KDRV_TRACE("[AUD_DEC_IOW_GET_KDRV_HANDLE] Called\n");

			if (copy_to_user((void *)arg, (void *)&(pDev->allocDev), sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_DEC_IOR_GET_INFO:
		{
			LX_AUD_DECODED_INFO_T esInfo;

			AUD_KDRV_TRACE("[AUD_DEC_IOW_GET_ES_INFO] Called\n");

			AUDIO_GetDecodedInfo(allocDev, &esInfo);

			if (copy_to_user((void *)arg, (void *)&esInfo, sizeof(LX_AUD_DECODED_INFO_T)))
				return RET_ERROR;
		}
		break;

		case AUD_DEC_IOW_IS_DECODABLE:
		{
			UINT32 isDecodable;

			AUD_KDRV_TRACE("[AUD_DEC_IOW_IS_DECODABLE] Called\n");

			AUDIO_IsDecodable(allocDev, &isDecodable);

			if (copy_to_user((void *)arg, (void *)&isDecodable, sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_DEC_IO_CLOSE_DEVICE:
		{
			AUD_KDRV_TRACE("[AUD_DEC_IO_CLOSE_DEVICE] Called\n");
			retVal = AUDIO_CloseDecoder(pDev->allocDev);
		}
		break;

		case AUD_DEC_IOW_SET_MODE:
		{
			LX_AUD_DECODER_MODE_T		decMode;

			if ( copy_from_user(&decMode, (void __user *)arg, sizeof(LX_AUD_DECODER_MODE_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_MODE] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_MODE] drc=%d, downmix=%d, aac=%d, tp=%d, ad=%d, sub=%d\n", \
				decMode.drc, decMode.downmix, decMode.aacBypass,
				decMode.tpTimerInterval, decMode.bSetAD, decMode.bSetSubDec);
			retVal = AUDIO_SetDecoderMode(allocDev, &decMode);
		}
		break;

		case AUD_DEC_IOR_GET_STATUS:
		{
			LX_AUD_DECODED_STATUS_T decodedStatus;

			AUD_KDRV_TRACE("[AUD_DEC_IOW_GET_STATUS] Called\n");

			AUDIO_GetDecodedStatus(allocDev, &decodedStatus);

			if (copy_to_user((void *)arg, (void *)&decodedStatus, sizeof(LX_AUD_DECODED_STATUS_T)))
				return RET_ERROR;
		}
		break;

		case AUD_DEC_IOW_SET_CERTI_PARAM:
		{
			UINT32		decCertiParam;

			if ( copy_from_user(&decCertiParam, (void __user *)arg, sizeof(UINT32)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_DECODING] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_CERTI_PARAM] certi_param =%d\n", decCertiParam);
			retVal = AUDIO_SetDecoderCertiParam(allocDev, decCertiParam);
		}
		break;

		case AUD_DEC_IOW_SET_INDEX_CTX:
		{
			LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

			if ( copy_from_user(&devIndexCtx, (void __user *)arg, sizeof(LX_AUD_DEV_INDEX_CTX_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_INDEX_CTX] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_INDEX_CTX] id =%d, ctx = %p\n", devIndexCtx.ui32Index, devIndexCtx.pCtx);

			retVal = AUDIO_SetDevIndexCtx(allocDev, &devIndexCtx);
		}
		break;

		case AUD_DEC_IOW_SET_PARAM_EXT:
		{
			LX_AUD_DECODER_PARAM_EXT_T	setParamExt;

			if ( copy_from_user(&setParamExt, (void __user *)arg, sizeof(LX_AUD_DECODER_PARAM_EXT_T)) )
			{
				AUD_KDRV_ERROR("[AUD_DEC_IOW_SET_INDEX_CTX] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_DEC_IOW_SET_SEAMLESS] param = %d\n", setParamExt.param[0]);

			retVal = AUDIO_SetDecoderParamExt(allocDev, &setParamExt);
		}
		break;

		default:
		{
			/* redundant check but it seems more readable */
			DBG_PRINT_ERROR("Invalid IOCTL Call!!!\n");
			retVal = RET_INVALID_IOCTL;
		}
		break;
	}

	return retVal;
}

/**
 * Notify AAC 5.1 Dual Decoding Case to disable PIP HP audio output.
 *
 * @param 	decIndex		[in] decoder index.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_FeedDec(), AUDIO_GetDecodedStatus().
 */
static SINT32 _AUDIO_NotifyAAC51DualDecoding(UINT32 allocDev)
{
	ULONG	flags;
	SINT32	decIndex = 0;

	BOOLEAN	notiFlag = FALSE;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	/* Check a audio dsp performance issue. */
	if((_gDecoderInfo[AUD_DECODER_0].decodedInfo.codecType == LX_AUD_CODEC_AAC)	\
	 &&(_gDecoderInfo[AUD_DECODER_1].decodedInfo.codecType == LX_AUD_CODEC_AAC) )	//AAC Dual Decoding.
	{
		/* Check a AD On status to disable above AD 2 channel audio. */
		if(_gDecoderInfo[decIndex].decMode.bSetAD == FALSE)
		{
			/* Check a AAC 5.1 Dual Decoding to disable Sub 5.1 channel audio. */
			if((_gDecoderInfo[AUD_DECODER_0].ui32ChannelNum == 6)	\
			 &&(_gDecoderInfo[AUD_DECODER_1].ui32ChannelNum == 6) )	//AAC Dual 5.1CH
			{
				notiFlag = TRUE;
			}
		}
		else
		{
			/* Check a AD On status to disable above AD 2 channel audio. */
			if(_gDecoderInfo[AUD_DECODER_0].ui32ChannelNum == 6 && _gDecoderInfo[AUD_DECODER_1].ui32ChannelNum > 2)		//AAC AD above 2 CH
			{
				notiFlag = TRUE;
			}
			else if(_gDecoderInfo[AUD_DECODER_1].ui32ChannelNum == 6 && _gDecoderInfo[AUD_DECODER_0].ui32ChannelNum > 2)//AAC AD above 2 CH
			{
				notiFlag = TRUE;
			}
		}
	}

	/* Check a AAC 5.1 dual or AD above 2 channel flag to stop sub audio decoding in app. level. */
	if(notiFlag == TRUE)
	{
		//Set a event if allocated device and event message is set for DEC0.
		if( (gAudSetEvent[LX_AUD_DEV_DEC0].allocDev != LX_AUD_DEV_NONE)		\
		  &&(gAudSetEvent[LX_AUD_DEV_DEC0].eventMsg & LX_AUD_EVENT_DEC_AAC51_DUAL) )
		{
			//spin lock for protection
			spin_lock_irqsave(&gAudEventSpinLock, flags);

			//Set a audio GET event type for next event.
			gAudGetEvent[LX_AUD_DEV_DEC0].allocDev  = LX_AUD_DEV_DEC0;
			gAudGetEvent[LX_AUD_DEV_DEC0].eventMsg |= LX_AUD_EVENT_DEC_AAC51_DUAL;

			//spin unlock for protection
			spin_unlock_irqrestore(&gAudEventSpinLock, flags);
		}

		//Set a event if allocated device and event message is set for DEC0.
		if( (gAudSetEvent[LX_AUD_DEV_DEC1].allocDev != LX_AUD_DEV_NONE)		\
		  &&(gAudSetEvent[LX_AUD_DEV_DEC1].eventMsg & LX_AUD_EVENT_DEC_AAC51_DUAL) )
		{
			//spin lock for protection
			spin_lock_irqsave(&gAudEventSpinLock, flags);

			//Set a audio GET event type for next event.
			gAudGetEvent[LX_AUD_DEV_DEC1].allocDev  = LX_AUD_DEV_DEC1;
			gAudGetEvent[LX_AUD_DEV_DEC1].eventMsg |= LX_AUD_EVENT_DEC_AAC51_DUAL;

			//spin unlock for protection
			spin_unlock_irqrestore(&gAudEventSpinLock, flags);
		}
	}

	AUD_KDRV_IMC_NOTI("Dec Info(%d) : NotifyAAC51Dual(%d, %d), notiFlag = %d\n", allocDev, _gDecoderInfo[AUD_DECODER_0].ui32ChannelNum, _gDecoderInfo[AUD_DECODER_1].ui32ChannelNum, notiFlag);

	return RET_OK;
}

/**
 * Get Decoder Writer Structure.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	if succeeded - writer structure, else - NULL.
 * @see		AUDIO_FeedDec(), AUDIO_GetDecodedStatus().
 */
static ADEC_BUF_T* _AUDIO_GetDecoderWriteStructure(UINT32 allocDev)
{
	ADEC_BUF_T				*pWriterStruct = NULL;
	ADEC_MODULE_ID			bufModule = ADEC_MODULE_NOT_DEF;

	// Get a CPB buffer
	bufModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
	if(bufModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
		return NULL;
	}

	pWriterStruct = AUDIO_GetBuffer(bufModule, OUT_PORT);

	AUD_KDRV_PRINT("allocDev %x %x %p.\n", allocDev, bufModule, pWriterStruct);

	return pWriterStruct;
}

/**
 * Set parameters for WMAPRO.
 *
 * @param 	moduleId		[in] a allocated module.
 * @param 	pDecParam		[in] parameters to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_SetDecoderParam().
 */
static SINT32 _AUDIO_SetDecoderParam_WMAPRO(ADEC_MODULE_ID moduleId, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	UINT32			wmaFormatTag	= pDecParam->wmaproInfo.ui32WmaFormatTag;
	UINT32			blockAlign	 	= pDecParam->wmaproInfo.ui32BlockAlign;
	UINT32			avgByteRate		= pDecParam->wmaproInfo.ui32AvgBytesPerSec;
	UINT32			lengthOfDSI		= pDecParam->ui32LengthOfDSI;
	UINT32			channelMask		= 0;
	UINT32			advEncodeOpt2	= 0;
	UINT16			encodeOpt		= 0;
	UINT16			advEncodeOpt	= 0;
	UINT8			*pDSIBuffer		= (UINT8 *)pDecParam->pDSIBuffer;
	WmaCmdSetParam	wmaParam;

	if (pDSIBuffer == NULL)
	{
		AUD_KDRV_ERROR("pDSIBuffer is NULL\n");
		return RET_ERROR;
	}

	wmaParam.samp_freq		= pDecParam->samplingFreq;
	wmaParam.num_chan		= pDecParam->wmaproInfo.ui32NumberOfChannel;
	wmaParam.bits_per_sampS	= pDecParam->wmaproInfo.ui32PcmBitsPerSample;
	wmaParam.fmt_tag		= wmaFormatTag;
	wmaParam.blk_align		= blockAlign;
	wmaParam.avg_byterate	= avgByteRate;

	if((wmaFormatTag == WMAPRO_FORMAT_TAG) || (wmaFormatTag == WMALOSSLESS_FORMAT_TAG))
	{
		//Check Sanity
		if(lengthOfDSI != LENGTH_OF_DSI_FOR_WMA3)
		{
			AUD_KDRV_ERROR("DSI length(%d) is less than WMA3 info\n", lengthOfDSI);
			return RET_ERROR;
		}

		//Get a parameters from DSI buffer.
		channelMask		= ADEC_READ32_LE(pDSIBuffer + 2);
		advEncodeOpt2	= ADEC_READ32_LE(pDSIBuffer + 6);
		encodeOpt		= ADEC_READ16_LE(pDSIBuffer + 14);
		advEncodeOpt	= ADEC_READ16_LE(pDSIBuffer + 16);
	}
	else if(wmaFormatTag == WMA2_FORMAT_TAG)
	{
		if(lengthOfDSI != LENGTH_OF_DSI_FOR_WMA2)
		{
			AUD_KDRV_ERROR("DSI length(%d) is less than WMA2 info\n", lengthOfDSI);
			return RET_ERROR;
		}

		//Get a parameters from DSI buffer.
		encodeOpt		= ADEC_READ16_LE(pDSIBuffer + 4);

		//encodeOpt size is 6 bits.(2014.04.10)
		encodeOpt		= encodeOpt & 0x3F;
	}
	else				// WMA1
	{
		if(lengthOfDSI != LENGTH_OF_DSI_FOR_WMA1)
		{
			AUD_KDRV_ERROR("DSI length(%d) is less than WMA1 info\n", lengthOfDSI);
			return RET_ERROR;
		}

		//Get a parameters from DSI buffer.
		encodeOpt		= ADEC_READ16_LE(pDSIBuffer + 2);
	}

	wmaParam.encode_opt			= encodeOpt;
	wmaParam.ch_mask			= channelMask;
	wmaParam.adv_encode_opt		= advEncodeOpt;
	wmaParam.adv_encode_opt2	= advEncodeOpt2;

	AUDIO_IMC_SendCmdParam(WMA_CMD_SET_PARAM, moduleId, sizeof(WmaCmdSetParam), &wmaParam);

	AUD_KDRV_DEBUG_TMP("WMA Info: freq=%u, ch#=%u, width=%u, tag=0x%X, align=%u, encodeOpt=0x%08X, avgByteRate=%u, chMask=0x%08X, advEncodeOpt=0x%08X, advEncodeOpt2=0x%08X\n",
					wmaParam.samp_freq,
					wmaParam.num_chan,
					wmaParam.bits_per_sampS,
					wmaParam.fmt_tag,
					wmaParam.blk_align,
					wmaParam.encode_opt,
					wmaParam.avg_byterate,
					wmaParam.ch_mask,
					wmaParam.adv_encode_opt,
					wmaParam.adv_encode_opt2);

	return RET_OK;
}

/**
 * Set parameters for RA8.
 *
 * @param 	moduleId		[in] a allocated module.
 * @param 	pDecParam		[in] parameters to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_SetDecoderParam().
 */
static SINT32 _AUDIO_SetDecoderParam_RA8(ADEC_MODULE_ID moduleId, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	Ra8CmdSetParam	raParam;
	UINT32			leafSize		= pDecParam->raInfo.ui32LeafSize;			// leaf size
	UINT32			lengthOfDSI		= pDecParam->ui32LengthOfDSI;
	UINT8			*pDSIBuffer		= (UINT8 *)pDecParam->pDSIBuffer;
	UINT8			majorVer;
	UINT8			minorVer;
	UINT16			samplesPerFrame;		// number of samples per frame
	UINT16			freqResponse;			// determines frequency response
	UINT16			cplStartRegion	= 0;	// related to joint stereo encoding
	UINT16			cplQuantBits	= 0;	// related to joint stereo encoding

	if(pDSIBuffer == NULL)
	{
		AUD_KDRV_ERROR("pDSIBuffer is NULL\n");
		return RET_ERROR;
	}

	// 8bytes, 12bytes or 16bytes
	if(lengthOfDSI < MIN_LENGTH_OF_DSI_FOR_RA8)
	{
		AUD_KDRV_ERROR("DSI length(%d) is less than RA info\n", lengthOfDSI);
		return RET_ERROR;
	}

	majorVer		= pDSIBuffer[0];
	minorVer		= pDSIBuffer[3];
	samplesPerFrame = ADEC_READ16_BE(pDSIBuffer + 4);
	freqResponse	= ADEC_READ16_BE(pDSIBuffer + 6);

	if((majorVer >= RA8_MAJOR_V1) && (minorVer >= RA8_MINOR_V3) && (lengthOfDSI >= MIN_LENGTH_OF_DSI_FOR_RA8_V13))
	{
		cplStartRegion = ADEC_READ16_BE(pDSIBuffer + 12);
		cplQuantBits   = ADEC_READ16_BE(pDSIBuffer + 14);
	}

	raParam.nsamples	= (UINT32)samplesPerFrame;
	raParam.regions		= (UINT32)freqResponse;
	raParam.bits		= leafSize;
	raParam.cplstart	= (UINT32)cplStartRegion;
	raParam.cplqbits	= (UINT32)cplQuantBits;
	raParam.nchannels	= pDecParam->raInfo.ui32NumberOfChannel;

	AUDIO_IMC_SendCmdParam(RA8_CMD_SET_PARAM, moduleId, sizeof(Ra8CmdSetParam), &raParam);

	AUD_KDRV_DEBUG_TMP("RA Info: samplesPerFrame=%u, freqResponse=%u, leafSize=%u, cplStartRegion=%u, cplQuantBits=%u, nchannels=%u\n",
					raParam.nsamples,
					raParam.regions,
					raParam.bits,
					raParam.cplstart,
					raParam.cplqbits,
					raParam.nchannels);

	return RET_OK;
}

/**
 * Set codec parameters for audio decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecParam		[in] parameters to set.
 * @return 	decoder module.
 * @see		AUDIO_SetDecoderParam().
 */
static ADEC_MODULE_ID _AUDIO_SetDecoderParam_Codec(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	SINT32						decIndex = 0;

	ADEC_MODULE_ID				decoderModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				managerModule = ADEC_MODULE_NOT_DEF;

	LX_AUD_DECODER_OUT_MODE_T 	outMode = LX_AUD_DECODER_OUT_LR;
	AUD_DECODER_INFO_T			*pDecInfo = NULL;
	LX_AUD_DECODER_PARAM_EXT_T	*pDecInfoParamExt = NULL;
	AUD_DEV_INFO_T				*pDevInfo = NULL;

	CmCmdCreateModule			createModule;
	BypassCmdSetMode 			bypassParam;

	//Get a decoder index from allocated device.
	decIndex = GET_DEC_INDEX(allocDev);

	pDecInfo = &(_gDecoderInfo[decIndex]);
	pDecInfoParamExt = &(pDecInfo->decParamExt);
	pDevInfo = AUDIO_GetDevInfo(allocDev);

	if(pDecParam->codecType == LX_AUD_CODEC_AAC_BYPASS)
	{
		//Get a decoder module from allocated module.
		decoderModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_BYPASS);
		if(decoderModule == ADEC_MODULE_NOT_DEF)
		{
			decoderModule = AUDIO_AllocModule(ADEC_MOD_TYPE_BYPASS, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
			if(decoderModule == ADEC_MODULE_NOT_DEF)
			{
				AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", decoderModule);
				return ADEC_MODULE_NOT_DEF;
			}
		}

		bypassParam.mode = AU_BASED_MODE ;
		bypassParam.over_protect = ENABLE_OVERFLOW_PROTECTION;
		(void)AUDIO_IMC_SendCmdParam(BYPASS_CMD_SET_MODE, decoderModule, sizeof(BypassCmdSetMode), &bypassParam);

		return decoderModule;
	}

	//Get a decoder module from allocated module.
	decoderModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(decoderModule == ADEC_MODULE_NOT_DEF)
	{
		if(pDevInfo->index < DEV_DEC_NUM)
		{
			decoderModule = ADEC_MODULE_DEC_0 + pDevInfo->index;
			decoderModule = AUDIO_AllocModule(ADEC_MOD_TYPE_DECODER, ADEC_CORE_DSP0, decoderModule);
		}
		else
		{
			decoderModule = AUDIO_AllocModule(ADEC_MOD_TYPE_DECODER, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
		}
	}

	//Create Decoder module before connect
	managerModule = AUDIO_GetManagerModule(decoderModule);
	if(managerModule != ADEC_MODULE_NOT_DEF)
	{
		/* Check a audio codec type. */
		switch(pDecParam->codecType)
		{
			case LX_AUD_CODEC_PCM:
			case LX_AUD_CODEC_AC3:
			case LX_AUD_CODEC_AAC:
			case LX_AUD_CODEC_DRA:
			case LX_AUD_CODEC_MP3:
			case LX_AUD_CODEC_WMA_PRO:
			case LX_AUD_CODEC_DTS_M6:
			case LX_AUD_CODEC_VORBIS:
			case LX_AUD_CODEC_AMR_WB:
			case LX_AUD_CODEC_AMR_NB:
			case LX_AUD_CODEC_RA8:
			case LX_AUD_CODEC_FLAC:
			case LX_AUD_CODEC_AAC_ENC:
			case LX_AUD_CODEC_AAC_BYPASS:
				break;

			default:
				AUD_KDRV_ERROR("Not Supported Audio Codec.[%d] \n", pDecParam->codecType);
				return ADEC_MODULE_NOT_DEF;
		}

		createModule.module_id = decoderModule;
		createModule.module_param.codec_param.media_type = (ADEC_MEDIA_TYPE)pDecParam->codecType;

		AUDIO_IMC_SendCmdParam(CM_CMD_CREATE_MODULE, managerModule, sizeof(CmCmdCreateModule), &createModule);
	}

	//Set a param of PCM decoder
	if(pDecParam->codecType == LX_AUD_CODEC_PCM)
	{
		PcmDecCmdSetParam 	pcmDecParam;

		pcmDecParam.i_samp_freq	= pDecParam->samplingFreq;

		//If PCM data is from memory, set a pcm decoding information from app.
		if(pDecParam->input == LX_AUD_INPUT_SYSTEM)
		{
			pcmDecParam.i_pcm_wd_sz = pDecParam->pcmInfo.ui32PcmBitsPerSample;
			pcmDecParam.i_num_chan	= pDecParam->pcmInfo.ui32NumberOfChannel;
			pcmDecParam.i_endian	= pDecParam->pcmInfo.ui32Endian;
			pcmDecParam.i_signed	= pDecParam->pcmInfo.ui32Signed;

			/* Set a default decoder output mode for memory */
			outMode = LX_AUD_DECODER_OUT_LR;
			(void)AUDIO_IMC_SendCmdParam(ADEC_CMD_SET_OUTMODE, decoderModule, sizeof(LX_AUD_DECODER_OUT_MODE_T), &outMode);

			pDecInfo->decOutMode = outMode;
		}
		else
		{
			//This is a pre-defined value in audio DSP module.
			pcmDecParam.i_pcm_wd_sz = 32;
			pcmDecParam.i_num_chan	= 2;
			pcmDecParam.i_endian	= LX_AUD_RENDER_LITTLE_ENDIAN;
			pcmDecParam.i_signed	= LX_AUD_RENDER_SIGNED;
		}

		AUDIO_IMC_SendCmdParam(PCMDEC_CMD_SET_PARAM, decoderModule, sizeof(PcmDecCmdSetParam), &pcmDecParam);
	}
	//Set a param of DDC decoder
	else if(pDecParam->codecType == LX_AUD_CODEC_AC3)
	{
		DdcCmdSetParam 		ddcParam;

		ddcParam.drc_mode	  = pDecInfo->decMode.drc;		// LX_ADEC_DOLBY_LINE_MODE;	// ATSC = LX_ADEC_DOLBY_LINE_MODE, DVB = LX_ADEC_DOLBY_RF_MODE
		ddcParam.downmix_mode = pDecInfo->decMode.downmix;	// LX_ADEC_LTRT_MODE;		// DDC = LX_ADEC_LTRT_MODE, DDT = LX_ADEC_LORO_MODE

		//To check SPDIF DD/DD+ Bypass mode.
		ddcParam.bypass_mode  = pDecInfoParamExt->param[0];	//Bypass mode : 0 = DD(default), 1 = DD+

		//To check sub decoding mode.
		ddcParam.isSubDec  = (UINT32)pDecInfo->decMode.bSetSubDec;	//Sub Decoding mode for SPDIF ES Not Support(Main Dec(SPDIF ES Support).

		//Set a AD Mode for Implement a meta data processing and mix volume for overload.
		ddcParam.isADMode  = (UINT32)pDecInfo->decMode.bSetAD;	// AD mode : 0 = Off(default), 1 = On(DEC0, DEC1 must set)

		AUDIO_IMC_SendCmdParam(DDC_CMD_SET_PARAM, decoderModule, sizeof(DdcCmdSetParam), &ddcParam);
		AUD_KDRV_DEBUG("MS10_DDC : isSubDec(%d), isADMode(%d)\n", ddcParam.isSubDec, ddcParam.isADMode);
	}
	// set param of MP3 decoder
	else if(pDecParam->codecType == LX_AUD_CODEC_MP3)
	{
		Mp3CmdSetParam	mp3Param;

		if(pDecParam->input == LX_AUD_INPUT_SYSTEM)
		{
			mp3Param.drc_mode = 0;
			mp3Param.isSubDec = 0;
			mp3Param.isADMode = 0;
		}
		else
		{
			mp3Param.drc_mode = (UINT32)pDecInfo->decMode.drc;
			mp3Param.isSubDec = (UINT32)pDecInfo->decMode.bSetSubDec;
			mp3Param.isADMode = (UINT32)pDecInfo->decMode.bSetAD;	// AD mode : 0 = Off(default), 1 = On(DEC0, DEC1 must set)
		}

		AUDIO_IMC_SendCmdParam(MP3_CMD_SET_PARAM, decoderModule, sizeof(Mp3CmdSetParam), &mp3Param);
		AUD_KDRV_DEBUG("MP3(isSubDec %d, isADMode %d)\n", mp3Param.isSubDec, mp3Param.isADMode);
	}
	// set param of DDT decoder
	else if(pDecParam->codecType == LX_AUD_CODEC_AAC)
	{
		DdtCmdSetParam 		ddtParam;

		ddtParam.drc_mode	  = pDecInfo->decMode.drc;		// LX_ADEC_DOLBY_LINE_MODE;	// ATSC = LX_ADEC_DOLBY_LINE_MODE, DVB = LX_ADEC_DOLBY_RF_MODE
		ddtParam.downmix_mode = pDecInfo->decMode.downmix;	// LX_ADEC_LORO_MODE;		// DDC = LX_ADEC_LTRT_MODE, DDT = LX_ADEC_LORO_MODE

		//To check sub decoding mode.
		ddtParam.isSubDec  = (UINT32)pDecInfo->decMode.bSetSubDec;	//Sub Decoding mode for SPDIF ES Not Support(Main Dec(SPDIF ES Support).

		//Set a AD Mode for Implement a meta data processing and mix volume for overload.
		ddtParam.isADMode  = (UINT32)pDecInfo->decMode.bSetAD;	// AD mode : 0 = Off(default), 1 = On(DEC0, DEC1 must set)

		//Set a SPDIF AAC ES Bypass mode
		ddtParam.iec_fmt   = (UINT32)pDecInfo->decMode.aacBypass;

		//Set input type
		if ( (pDecParam->input == LX_AUD_INPUT_HDMI)	\
		   ||(pDecParam->input >= LX_AUD_INPUT_HDMI0 && pDecParam->input <= LX_AUD_INPUT_HDMI3) )
		{
			ddtParam.inputtype = (MS10_DDT_INPUT_T)LX_AUD_INPUT_HDMI;
		}
		else
		{
			ddtParam.inputtype = (MS10_DDT_INPUT_T)pDecParam->input;
		}

		//Set Transcoding
		ddtParam.transcoding = pDecInfoParamExt->param[0];	// 0 = Enable(default), 1 = Disable

		AUDIO_IMC_SendCmdParam(DDT_CMD_SET_PARAM, decoderModule, sizeof(DdtCmdSetParam), &ddtParam);
		AUD_KDRV_DEBUG("MS10_DDT(isSubDec %d, iec_fmt %d, isADMode %d input %d)\n", ddtParam.isSubDec, ddtParam.iec_fmt, ddtParam.isADMode, ddtParam.inputtype);
	}
	// set param of WMA
	else if(pDecParam->codecType == LX_AUD_CODEC_WMA_PRO)
	{
		if(_AUDIO_SetDecoderParam_WMAPRO(decoderModule, pDecParam) == RET_ERROR)
			return ADEC_MODULE_NOT_DEF;
	}
	// set param of RA8
	else if(pDecParam->codecType == LX_AUD_CODEC_RA8)
	{
		if(_AUDIO_SetDecoderParam_RA8(decoderModule, pDecParam) == RET_ERROR)
			return ADEC_MODULE_NOT_DEF;
	}
	else if(pDecParam->codecType == LX_AUD_CODEC_DTS_M6)
	{
		DtsM6CmdSetParam	dtsm6Param;

		dtsm6Param.isSubDec =  (UINT32)pDecInfo->decMode.bSetSubDec;
		dtsm6Param.dts_type =  1;	// 0 : DTS-CA, 1 : DTS-HD, 2 : DTS-LBR
		dtsm6Param.dts_seamless = pDecInfo->decParamExt.param[0];	// 0 : Off(Default), 1 : On

		AUDIO_IMC_SendCmdParam(DTSM6_CMD_SET_PARAM, decoderModule, sizeof(DtsM6CmdSetParam), &dtsm6Param);
		AUD_KDRV_DEBUG("DTS_M6(isSubDec %d, type %d, seamless %d)\n",
			dtsm6Param.isSubDec, dtsm6Param.dts_type, dtsm6Param.dts_seamless);
	}

	//Send a IMC command - INIT : This command is only called when decoder codec is created once.
	(void)AUDIO_IMC_SendCmd(ADEC_CMD_INIT, decoderModule);

	AUD_KDRV_PRINT("decoderModule = %d\n", decoderModule);
	return decoderModule;
}

/**
 * Set input parameters for audio decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecParam		[in] parameters to set.
 * @return 	input module.
 * @see		AUDIO_SetDecoderParam().
 */
static ADEC_MODULE_ID _AUDIO_SetDecoderParam_Input(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	ADEC_MODULE_ID		inputModule		= ADEC_MODULE_NOT_DEF;
	AUD_DECODER_INFO_T	*pDecInfo = NULL;
	SINT32				decIndex = 0;

	TpCmdSetParam		tpSetParam;
	SifCmdSetFmt		sifSetFmt;
	AdcCmdSetFmt		adcSetFmt;
	HdmiCmdSetFmt		hdmiSetFmt;
	HdmiCmdSetMode		hdmiSetMode;

	//Get a decoder index
	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	pDecInfo = &(_gDecoderInfo[decIndex]);

	//Get a input module from audio source
	if(pDecParam->input == LX_AUD_INPUT_SYSTEM)
	{
		// if already alloceated, return it.
		inputModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
		if(inputModule == ADEC_MODULE_NOT_DEF)
		{
			inputModule = AUDIO_AllocModule(ADEC_MOD_TYPE_EXT_BUF, ADEC_CORE_ARM, ADEC_MODULE_NOT_DEF);
		}
	}
	else
	{
		// if already alloceated, return it.
		inputModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
		if(inputModule == ADEC_MODULE_NOT_DEF)
		{
			inputModule = AUDIO_GetModuleInputType(pDecParam->input);
			if(inputModule == ADEC_MODULE_NOT_DEF)
			{
				AUD_KDRV_ERROR("Input[%d] is not available \n", pDecParam->input);
				return ADEC_MODULE_NOT_DEF;
			}

			inputModule = AUDIO_AllocModule(ADEC_MOD_TYPE_SOURCE, ADEC_CORE_DSP0, inputModule);
		}
	}

	if(inputModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", inputModule);
		return ADEC_MODULE_NOT_DEF;
	}

	//Set a input parameter for each audio input port.
	if(pDecParam->input == LX_AUD_INPUT_TP0 || pDecParam->input == LX_AUD_INPUT_TP1)
	{
		tpSetParam.codec_type = (ADEC_MEDIA_TYPE)pDecParam->codecType;

		//Set interrupt_type : TP Processing Type
		if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			//TP Interrupt
			tpSetParam.interval		  = 0;
			tpSetParam.interrupt_type = 1;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			//TP Interrupt
			tpSetParam.interval		  = 0;
			tpSetParam.interrupt_type = 1;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			//TP Interrupt
			tpSetParam.interval		  = 0;
			tpSetParam.interrupt_type = 1;
		}
		else
		{
			//Timer Interrupt (Default)
			tpSetParam.interval		  = pDecInfo->decMode.tpTimerInterval;
			tpSetParam.interrupt_type = 2;
		}

		AUDIO_IMC_SendCmdParam(TP_CMD_SET_PARAM, inputModule, sizeof(TpCmdSetParam), &tpSetParam);
	}
	else if(pDecParam->input == LX_AUD_INPUT_SIF)
	{
		sifSetFmt.format				= 0;
		sifSetFmt.resolution			= 3;
		sifSetFmt.sck_polarity_control	= 0;
		sifSetFmt.lrck_polarity_control	= 0;
		AUDIO_IMC_SendCmdParam(SIF_CMD_SET_FMT, inputModule, sizeof(SifCmdSetFmt), &sifSetFmt);
	}
	else if(pDecParam->input == LX_AUD_INPUT_ADC)
	{
		adcSetFmt.format 	 = 0;
		adcSetFmt.resolution = 3;

		//Set I2S and 24 bit mode for I2S input : H13 ACE bug is fixed.
		if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			//24 bit(0x03)
			adcSetFmt.sck_polarity_control = 0;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			//24 bit(0x03)
			adcSetFmt.sck_polarity_control = 0;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			//24 bit(0x03)
			adcSetFmt.sck_polarity_control = 0;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
		{
			//24 bit(0x03)
			adcSetFmt.sck_polarity_control = 0;
		}
		else
		{
			//24 bit(0x03)
			adcSetFmt.sck_polarity_control = 0;
		}
		adcSetFmt.lrck_polarity_control = 0;
		AUDIO_IMC_SendCmdParam(ADC_CMD_SET_FMT, inputModule, sizeof(AdcCmdSetFmt), &adcSetFmt);
	}
	else if ( (pDecParam->input == LX_AUD_INPUT_HDMI)	\
			||(pDecParam->input >= LX_AUD_INPUT_HDMI0 && pDecParam->input <= LX_AUD_INPUT_HDMI3) )
	{
		if(pDecParam->codecType == LX_AUD_CODEC_PCM)
		{
			hdmiSetFmt.format 				= 0;
			hdmiSetFmt.resolution 			= 3;
			hdmiSetFmt.sck_polarity_control = 0;
			hdmiSetFmt.lrck_polarity_control= 0;
			AUDIO_IMC_SendCmdParam(HDMI_CMD_SET_FMT, inputModule, sizeof(HdmiCmdSetFmt), &hdmiSetFmt);

			hdmiSetMode.mode = 0;

			//Set a default detail audio codec type for PCM audio codec.
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_PCM;
		}
		else if(pDecParam->codecType == LX_AUD_CODEC_AC3_IEC60958)
		{
			hdmiSetMode.mode = 2;

			pDecParam->codecType = LX_AUD_CODEC_AC3;
		}
		else if(pDecParam->codecType == LX_AUD_CODEC_DTS_CD)
		{
			hdmiSetMode.mode = 3;

			pDecParam->codecType = LX_AUD_CODEC_DTS_M6;

			//Set a default detail audio codec type for DTS-CD audio codec.
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS_CD;
		}
		else
		{
			hdmiSetMode.mode = 1;

			//Set a default detail audio codec type for DTS audio codec.
			if(pDecParam->codecType == LX_AUD_CODEC_DTS_M6)
			{
				pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS;
			}
		}

		if(pDecParam->input >= LX_AUD_INPUT_HDMI0)
		{
			hdmiSetMode.port = pDecParam->input - LX_AUD_INPUT_HDMI0;
		}
		else
		{
			hdmiSetMode.port = 0;
		}

		AUDIO_IMC_SendCmdParam(HDMI_CMD_SET_MODE, inputModule, sizeof(hdmiSetMode), &hdmiSetMode);
	}

	AUD_KDRV_PRINT("inputModule = %d\n", inputModule);

	return inputModule;
}

/**
 * Set a audio channel mode from decoded information.
 *
 * @param 	pEsInfoParam	[in] decoder ES info from decoder.
 * @param 	pDecodedInfo	[in] decoded ES info for notity.
 * @return 	Channel Number.
 * @see		_AUDIO_DecodedInfoCb().
 */
static UINT8 _AUDIO_SetChannelMode(DecEvtESDecInfoParam *pEsInfoParam, LX_AUD_DECODED_INFO_T *pDecodedInfo)
{
	UINT8	channelNum;

	LX_AUD_CODEC_T	adecFormat;

	adecFormat = pEsInfoParam->media_type;

	//Set a default audio channel number and mode.
	channelNum = 2;
	pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;

	if(adecFormat == LX_AUD_CODEC_PCM)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case PCMDEC_CHANNELMODE_1CH:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case PCMDEC_CHANNELMODE_2CH:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case PCMDEC_CHANNELMODE_3CH:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case PCMDEC_CHANNELMODE_4CH:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case PCMDEC_CHANNELMODE_5CH:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case PCMDEC_CHANNELMODE_6CH:
				channelNum = 6;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_AC3)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MS10DDC_CHANNELMODE_DUAL_MONO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_DUAL_MONO;
				break;

			case MS10DDC_CHANNELMODE_MONO_CENTER:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case MS10DDC_CHANNELMODE_STEREO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case MS10DDC_CHANNELMODE_3_0_CHANNEL:
			case MS10DDC_CHANNELMODE_2_1_CHANNEL:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case MS10DDC_CHANNELMODE_3_1_CHANNEL:
			case MS10DDC_CHANNELMODE_2_2_CHANNEL:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case MS10DDC_CHANNELMODE_3_2_CHANNEL:
				channelNum = 6;		//5.1ch
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_AAC)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MS10DDT_CHANNELMODE_MONO:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case MS10DDT_CHANNELMODE_PARAMETRIC_STEREO:
			case MS10DDT_CHANNELMODE_STEREO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case MS10DDT_CHANNELMODE_DUAL_CHANNEL:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_DUAL_MONO;
				break;

			case MS10DDT_CHANNELMODE_3_CHANNEL_FRONT:
			case MS10DDT_CHANNELMODE_3_CHANNEL_SURR:
			case MS10DDT_CHANNELMODE_2_1_STEREO:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case MS10DDT_CHANNELMODE_4_CHANNEL_2SURR:
			case MS10DDT_CHANNELMODE_4_CHANNEL_1SURR:
			case MS10DDT_CHANNELMODE_3_1_CHANNEL_FRONT:
			case MS10DDT_CHANNELMODE_3_1_CHANNEL_SURR:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case MS10DDT_CHANNELMODE_5_CHANNEL:
			case MS10DDT_CHANNELMODE_4_1_CHANNEL_2SURR:
			case MS10DDT_CHANNELMODE_4_1_CHANNEL_1SURR:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case MS10DDT_CHANNELMODE_5_1_CHANNEL:
				channelNum = 6;	//AAC 5.1 Ch. -> Not support for dual decoding.
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_DRA)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case DRADEC_CHANNELMODE_1_0_CHANNEL:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_MONO;
				break;

			case DRADEC_CHANNELMODE_1_1_CHANNEL:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_MONO;
				break;

			case DRADEC_CHANNELMODE_2_0_CHANNEL:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_STEREO;
				break;

			case DRADEC_CHANNELMODE_2_1_CHANNEL:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_2_1_FL_FR_LFE;
				break;

			case DRADEC_CHANNELMODE_3_0_CHANNEL:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_3_0_FL_FR_RC;
				break;

			case DRADEC_CHANNELMODE_3_1_CHANNEL:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_3_1_FL_FR_RC_LFE;
				break;

			case DRADEC_CHANNELMODE_4_0_CHANNEL:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_4_0_FL_FR_RL_RR;
				break;

			case DRADEC_CHANNELMODE_4_1_CHANNEL:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_4_1_FL_FR_RL_RR_LFE;
				break;

			case DRADEC_CHANNELMODE_5_0_CHANNEL:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_5_0_FL_FR_FC_RL_RR;
				break;

			case DRADEC_CHANNELMODE_5_1_CHANNEL:
				channelNum = 6;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_5_1_FL_FR_FC_RL_RR_LFE;
				break;

			case DRADEC_CHANNELMODE_UNDEFINED:
			case DRADEC_CHANNELMODE_6_0_CHANNEL:
			case DRADEC_CHANNELMODE_6_1_CHANNEL:
			case DRADEC_CHANNELMODE_7_0_CHANNEL:
			case DRADEC_CHANNELMODE_7_1_CHANNEL:
			case DRADEC_CHANNELMODE_8_0_CHANNEL:
			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_CH_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_MP3)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MP3DEC_CHANNELMODE_STEREO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case MP3DEC_CHANNELMODE_JOINT_STEREO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_JOINT_STEREO;
				break;

			case MP3DEC_CHANNELMODE_DUAL_MONO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_DUAL_MONO;
				break;

			case MP3DEC_CHANNELMODE_MONO:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_WMA_PRO)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case WMADEC_CHANNELMODE_STEREO:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case WMADEC_CHANNELMODE_MONO:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case WMADEC_CHANNELMODE_THREE:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case WMADEC_CHANNELMODE_BACK:
			case WMADEC_CHANNELMODE_QUAD:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case WMADEC_CHANNELMODE_5:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case WMADEC_CHANNELMODE_5DOT1:
				channelNum = 6;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case WMADEC_CHANNELMODE_6DOT1:
				channelNum = 7;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case WMADEC_CHANNELMODE_7DOT1:
				channelNum = 8;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_DTS_M6)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case DTSDEC_CHANNELMODE_1CH:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case DTSDEC_CHANNELMODE_2CH:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case DTSDEC_CHANNELMODE_3CH:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case DTSDEC_CHANNELMODE_4CH:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case DTSDEC_CHANNELMODE_5CH:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case DTSDEC_CHANNELMODE_6CH:
				channelNum = 6;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_VORBIS)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case OGGDEC_CHANNELMODE_1CH:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case OGGDEC_CHANNELMODE_2CH:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_AMR_WB)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case AMRWBDEC_CHANNELMODE_MONO:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_AMR_NB)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case AMRNBDEC_CHANNELMODE_MONO:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_RA8)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case RA8DEC_CHANNELMODE_1CH:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case RA8DEC_CHANNELMODE_2CH:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_FLAC)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case FLACDEC_CHANNELMODE_1CH:
				channelNum = 1;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MONO;
				break;

			case FLACDEC_CHANNELMODE_2CH:
				channelNum = 2;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_STEREO;
				break;

			case FLACDEC_CHANNELMODE_3CH:
				channelNum = 3;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case FLACDEC_CHANNELMODE_4CH:
				channelNum = 4;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case FLACDEC_CHANNELMODE_5CH:
				channelNum = 5;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case FLACDEC_CHANNELMODE_6CH:
				channelNum = 6;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case FLACDEC_CHANNELMODE_7CH:
				channelNum = 7;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			case FLACDEC_CHANNELMODE_8CH:
				channelNum = 8;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_MULTI;
				break;

			default:
				channelNum = 0;
				pDecodedInfo->esInfo.audioMode = LX_AUD_DECODER_MODE_UNKNOWN;
				break;
		}
	}

	return channelNum;
}

/**
 * callback function to update Decoded ES inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_DecodedInfoCb(void *_param, int _paramLen, void *_cbParam)
{
	UINT8	channelNum = 2;
	ULONG	flags;

	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;

	DecEvtESDecInfoParam	*pEsInfoParam = NULL;
	AUD_DECODER_INFO_T		*pDecInfo 	  = NULL;
	LX_AUD_DECODED_INFO_T 	*pDecodedInfo = NULL;

	pEsInfoParam = (DecEvtESDecInfoParam *)_param;
	pDecInfo 	 = (AUD_DECODER_INFO_T *)_cbParam;
	allocDev	 = pDecInfo->ui32AllocDev;
	pDecodedInfo = &(pDecInfo->decodedInfo);

	// Print For Debug
	if(_paramLen != sizeof(DecEvtESDecInfoParam))
	{
		AUD_KDRV_ERROR("Dec Es Info : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtESDecInfoParam), _paramLen);
		return RET_ERROR;
	}

	//Set a audio codec type.
	pDecodedInfo->esInfo.adecFormat = (LX_AUD_CODEC_T)pEsInfoParam->media_type;

	//Set a audio channel mode.
	channelNum = _AUDIO_SetChannelMode(pEsInfoParam, pDecodedInfo);
	pDecInfo->ui32ChannelNum = channelNum;

	//Copy a additional decoded info.
	pDecodedInfo->codecType		   = (LX_AUD_CODEC_T)pEsInfoParam->media_type;
	pDecodedInfo->samplingFreq	   = pEsInfoParam->sample_rate;
	pDecodedInfo->ui32AvgBitRate   = pEsInfoParam->avg_bit_rate;
	pDecodedInfo->ui32NumOfChannel = pEsInfoParam->num_of_channel;
	pDecodedInfo->ui32BitPerSample = pEsInfoParam->bit_per_sample;

	//Set a audio SPDIF ES Available.
	if(pDecodedInfo->codecType == LX_AUD_CODEC_AC3)
	{
		pDecInfo->bSpdifESAvail = TRUE;
	}
	else if(pDecodedInfo->codecType == LX_AUD_CODEC_AAC)
	{
		if(pDecodedInfo->samplingFreq == LX_AUD_SAMPLING_FREQ_48_KHZ)
		{
			pDecInfo->bSpdifESAvail = TRUE;
		}
	}
	else if(pDecodedInfo->codecType == LX_AUD_CODEC_DTS_M6)
	{
		pDecInfo->bSpdifESAvail = TRUE;
	}
	else
	{
		pDecInfo->bSpdifESAvail = FALSE;
	}

	//Copy a additional decoded es info for app.
	if(pDecodedInfo->esInfo.adecFormat == LX_AUD_CODEC_AC3)
	{
		DecAC3EsInfo	ac3EsInfo;

		pDecodedInfo->esInfo.ac3ESInfo.bitRate 	  = pEsInfoParam->avg_bit_rate/(1000*32);
		pDecodedInfo->esInfo.ac3ESInfo.sampleRate = pEsInfoParam->sample_rate/1000;
		pDecodedInfo->esInfo.ac3ESInfo.channelNum = channelNum;

		memcpy(&ac3EsInfo, pEsInfoParam->es_info, sizeof(DecAC3EsInfo));
		pDecodedInfo->esInfo.ac3ESInfo.EAC3 = ac3EsInfo.EAC3;

		//Set a audio SPDIF ES Available for E-AC3 if not 48Khz.
		if(ac3EsInfo.EAC3 == 1 && pDecodedInfo->samplingFreq != LX_AUD_SAMPLING_FREQ_48_KHZ)
		{
			pDecInfo->bSpdifESAvail = FALSE;
		}

		//Set a detail audio codec type.
		if(ac3EsInfo.EAC3 == 0)
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_AC3;
		}
		else
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_EAC3;
		}
	}
	else if(pDecodedInfo->esInfo.adecFormat == LX_AUD_CODEC_MP3)
	{
		DecMPEGEsInfo	mpegEsInfo;

		pDecodedInfo->esInfo.mpegESInfo.bitRate    = pEsInfoParam->avg_bit_rate/(1000*32);
		pDecodedInfo->esInfo.mpegESInfo.sampleRate = pEsInfoParam->sample_rate/1000;
		pDecodedInfo->esInfo.mpegESInfo.channelNum = channelNum;

		memcpy(&mpegEsInfo, pEsInfoParam->es_info, sizeof(DecMPEGEsInfo));
		pDecodedInfo->esInfo.mpegESInfo.layer = mpegEsInfo.layer;
	}
	else if(pDecodedInfo->esInfo.adecFormat == LX_AUD_CODEC_AAC)
	{
		DecHEAACEsInfo	heaacEsInfo;

		memcpy(&heaacEsInfo, pEsInfoParam->es_info, sizeof(DecHEAACEsInfo));
		pDecodedInfo->esInfo.heAAcEsInfo.version = heaacEsInfo.version;
		pDecodedInfo->esInfo.heAAcEsInfo.transmissionformat = heaacEsInfo.transmissionformat;
		pDecodedInfo->esInfo.heAAcEsInfo.channelNum = channelNum;

		//Set a detail audio codec type.
		if(heaacEsInfo.version == 0)
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_AAC;
		}
		else if(heaacEsInfo.version == 1)
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_HEAACV1;
		}
		else
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_HEAACV2;
		}

		//Check a chip revision
		if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			//Support a AAC 5.1 dual decoding case in H14 Chip.
			AUD_KDRV_IMC_NOTI("Support AAC 5.1 Dual Decoding(0x%X)\n", lx_chip_rev());
		}
		else
		{
			//Check a AAC 5.1 dual decoding case.
			(void)_AUDIO_NotifyAAC51DualDecoding(allocDev);
		}

		//Check a AAC Codec decoding case.
		if( (allocDev == LX_AUD_DEV_DEC0 && pDecInfo->decParam.input == LX_AUD_INPUT_TP0)
		  ||(allocDev == LX_AUD_DEV_DEC1 && pDecInfo->decParam.input == LX_AUD_INPUT_TP0) )
		{
			//Set a event if allocated device and event message is set for DEC0.
			if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
			  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_AAC_CODEC) )
			{
				//spin lock for protection
				spin_lock_irqsave(&gAudEventSpinLock, flags);

				//Set a audio GET event type for next event.
				gAudGetEvent[allocDev].allocDev  = allocDev;
				gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_AAC_CODEC;

				//spin unlock for protection
				spin_unlock_irqrestore(&gAudEventSpinLock, flags);
			}
		}
	}
	else if (pDecodedInfo->esInfo.adecFormat == LX_AUD_CODEC_DTS_M6)
	{
		//Check a detail audio codec type for DTS-CD audio codec.
		if(pDecInfo->codecDetailType != LX_AUD_CODEC_DETAIL_DTS_CD)
		{
			DecDTSM6EsInfo	dtsM6EsInfo;

			memcpy(&dtsM6EsInfo, pEsInfoParam->es_info, sizeof(DecDTSM6EsInfo));

			//Set a detail audio codec type.
			if(dtsM6EsInfo.dts_type == 0)
			{
				pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS;
			}
			else if(dtsM6EsInfo.dts_type == 1)
			{
				pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS;
				//pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS_HD_MA;		//temp after new library.
			}
			else
			{
				pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS_EXPRESS;
			}

			AUD_KDRV_DEBUG("Dec Info(%d) : a(%d), b(%d), c(%d)\n", \
							allocDev, dtsM6EsInfo.dts_type, dtsM6EsInfo.es_format, dtsM6EsInfo.es_fsize);
		}
	}

	//Set a decoded information notification for LMF play case.
	if( (allocDev == LX_AUD_DEV_DEC0 && pDecInfo->decParam.input == LX_AUD_INPUT_SYSTEM)	\
	  ||(allocDev == LX_AUD_DEV_DEC1 && pDecInfo->decParam.input == LX_AUD_INPUT_SYSTEM) )
	{
		//Set a event if allocated device and event message is set for DEC0.
		if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
		  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_DECODED_INFO) )
		{
			//spin lock for protection
			spin_lock_irqsave(&gAudEventSpinLock, flags);

			//Set a audio GET event type for next event.
			gAudGetEvent[allocDev].allocDev  = allocDev;
			gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_DECODED_INFO;

			//spin unlock for protection
			spin_unlock_irqrestore(&gAudEventSpinLock, flags);
		}
	}

	//For debug print.
	if(pDecodedInfo->esInfo.audioMode == LX_AUD_DECODER_MODE_DUAL_MONO)
	{
		AUD_KDRV_DEBUG("Dec Info(DUAL_MONO) : channelNum(%d)\n", channelNum);
	}
	else if(pDecodedInfo->esInfo.audioMode == LX_AUD_DECODER_MODE_MULTI)
	{
		AUD_KDRV_DEBUG("Dec Info(MULTI) : channelNum(%d)\n", channelNum);
	}
	else if(pDecodedInfo->esInfo.audioMode == LX_AUD_DECODER_MODE_MONO)
	{
		AUD_KDRV_DEBUG("Dec Info(MONO) : channelNum(%d)\n", channelNum);
	}
	else if(pDecodedInfo->esInfo.audioMode == LX_AUD_DECODER_MODE_JOINT_STEREO)
	{
		AUD_KDRV_DEBUG("Dec Info(JOINT_STEREO) : CN(%d)\n", channelNum);
	}

	AUD_KDRV_IMC_NOTI("Dec Info(%d) : Type(%d), Fs(%d), Ch(%d), Bs(%d), Br(%d), Cm(%d)\n", \
					allocDev, pEsInfoParam->media_type, pEsInfoParam->sample_rate, pEsInfoParam->num_of_channel, pEsInfoParam->bit_per_sample, pEsInfoParam->avg_bit_rate, pEsInfoParam->channel_mode);
	return RET_OK;
}

/**
 * callback function to update decodable inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_TpIsDecodableCb(void *_param, int	_paramLen, void *_cbParam)
{
	UINT32					*pIsDecodable = NULL;
	AUD_DECODER_INFO_T		*pDecInfo 	 = NULL;
	TpEvtESExistInfoParam	*pEsExistInfo = NULL;

	pEsExistInfo = (TpEvtESExistInfoParam *)_param;
	pDecInfo = (AUD_DECODER_INFO_T *)_cbParam;
	pIsDecodable = &(pDecInfo->decIsDecodable);

	// Print For Debug
	if (_paramLen != sizeof(TpEvtESExistInfoParam))
	{
		AUD_KDRV_ERROR("Decodable : Param Length Error[Expected:%d][Input:%d]\n", sizeof(TpEvtESExistInfoParam), _paramLen);
		return RET_ERROR;
	}

	//Sanity Check
	if(pDecInfo->ui32AllocDev < LX_AUD_DEV_DEC0 || pDecInfo->ui32AllocDev > LX_AUD_DEV_DEC1)
	{
		AUD_KDRV_ERROR("Check Decodable : ui32AllocDev [%d]\n", pDecInfo->ui32AllocDev);
		return RET_ERROR;
	}

	*pIsDecodable = pEsExistInfo->es_exist;

	AUD_KDRV_IMC_NOTI("Dec Decodable(%d) : %d\n", pDecInfo->ui32AllocDev, pEsExistInfo->es_exist);
	return RET_OK;
}

/**
 * callback function to notificate Decoder error inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_DecErrorCb(void *_param, int _paramLen, void *_cbParam)
{
	ULONG	flags;
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;

	DecEvtDecErrorParam		*pErrorParam = NULL;
	AUD_DECODER_INFO_T		*pDecInfo 	 = NULL;

	pErrorParam = (DecEvtDecErrorParam *)_param;
	pDecInfo	= (AUD_DECODER_INFO_T *)_cbParam;
	allocDev	= pDecInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(DecEvtDecErrorParam))
	{
		AUD_KDRV_ERROR("Dec Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtDecErrorParam), _paramLen);
		return RET_ERROR;
	}

	if ( pDecInfo->ui32AllocDev < LX_AUD_DEV_DEC0 || pDecInfo->ui32AllocDev > LX_AUD_DEV_DEC1)
	{
		AUD_KDRV_ERROR("Check CB ui32AllocDev [%d][%x]\n",pDecInfo->ui32AllocDev,pDecInfo->ui32AllocDev  );
		return RET_ERROR;
	}

	if(pErrorParam->error_type == 1 || pErrorParam->error_type == 2) // fatal decoding error(1), Sampling rate º¯°æ½Ã Error 2
	{
		AUDIO_StopDecoder(pDecInfo->ui32AllocDev);

		if(pDecInfo->decParam.input != LX_AUD_INPUT_SYSTEM)
		{
			AUDIO_FlushDecoder(pDecInfo->ui32AllocDev);
		}

		AUDIO_InitDecoder(pDecInfo->ui32AllocDev);
		AUDIO_StartDecoder(pDecInfo->ui32AllocDev);
	}
	else if(pErrorParam->error_type == 3)
	{
		if(pDecInfo->decParam.codecType == LX_AUD_CODEC_DTS_M6)
		{
			if ( (pDecInfo->decParam.input == LX_AUD_INPUT_HDMI)	\
			   ||(pDecInfo->decParam.input >= LX_AUD_INPUT_HDMI0 && pDecInfo->decParam.input <= LX_AUD_INPUT_HDMI3) )
			{
				AUDIO_StopDecoder(pDecInfo->ui32AllocDev);
				AUDIO_FlushDecoder(pDecInfo->ui32AllocDev);

				AUDIO_InitDecoder(pDecInfo->ui32AllocDev);
				AUDIO_StartDecoder(pDecInfo->ui32AllocDev);
			}
		}
	}

	//Set a event if allocated device and event message is set for DEC0.
	if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
	  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_ERROR) )
	{
		//spin lock for protection
		spin_lock_irqsave(&gAudEventSpinLock, flags);

		//Set a audio GET event type for next event.
		gAudGetEvent[allocDev].allocDev  = allocDev;
		gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_ERROR;

		//spin unlock for protection
		spin_unlock_irqrestore(&gAudEventSpinLock, flags);
	}

	AUD_KDRV_IMC_NOTI("Dec Error(%d) : Type(%d), Err(%d)\n", allocDev, pErrorParam->media_type, pErrorParam->error_type);
	return RET_OK;
}

/**
 * callback function to notificate TP input error inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_TpErrorCb(void *_param, int _paramLen, void *_cbParam)
{
	ULONG	flags;
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;

	TpEvtPdecErrorParam		*pErrorParam = NULL;
	AUD_DECODER_INFO_T		*pDecInfo 	 = NULL;

	pErrorParam = (TpEvtPdecErrorParam *)_param;
	pDecInfo	= (AUD_DECODER_INFO_T *)_cbParam;
	allocDev	= pDecInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(TpEvtPdecErrorParam))
	{
		AUD_KDRV_ERROR("TP Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(TpEvtPdecErrorParam), _paramLen);
		return RET_ERROR;
	}

	if ( pDecInfo->ui32AllocDev < LX_AUD_DEV_DEC0 || pDecInfo->ui32AllocDev > LX_AUD_DEV_DEC1)
	{
		AUD_KDRV_ERROR("Check CB ui32AllocDev [%d][%x]\n",pDecInfo->ui32AllocDev,pDecInfo->ui32AllocDev  );
		return RET_ERROR;
	}

	if(pErrorParam->error_type == 1) // fatal error
	{
		AUDIO_StopDecoder(pDecInfo->ui32AllocDev);
		AUDIO_FlushDecoder(pDecInfo->ui32AllocDev);
		AUDIO_InitDecoder(pDecInfo->ui32AllocDev);
		AUDIO_StartDecoder(pDecInfo->ui32AllocDev);
	}

	//Set a event if allocated device and event message is set for DEC0.
	if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
	  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_ERROR) )
	{
		//spin lock for protection
		spin_lock_irqsave(&gAudEventSpinLock, flags);

		//Set a audio GET event type for next event.
		gAudGetEvent[allocDev].allocDev  = allocDev;
		gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_ERROR;

		//spin unlock for protection
		spin_unlock_irqrestore(&gAudEventSpinLock, flags);
	}

	AUD_KDRV_IMC_NOTI("TP Error(%d) : Type(%d), Err(%d)\n", allocDev, pErrorParam->media_type, pErrorParam->error_type);
	return RET_OK;
}

/**
 * callback function to notificate SIF input frequency error inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_SifFreqErrorCb(void *_param, int _paramLen, void *_cbParam)
{
	UINT32	allocDev = LX_AUD_DEV_DEC0;
	UINT32	ui32ResetCount = 0;

	SifEvtUnstableFreq		*pErrorParam = NULL;
	AUD_DECODER_INFO_T		*pDecInfo 	 = NULL;
	AUD_DEV_INFO_T			*pDevInfo	 = NULL;

	pErrorParam = (SifEvtUnstableFreq *)_param;
	pDecInfo	= (AUD_DECODER_INFO_T *)_cbParam;
	allocDev	= pDecInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(SifEvtUnstableFreq))
	{
		AUD_KDRV_ERROR("IF Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(SifEvtUnstableFreq), _paramLen);
		return RET_ERROR;
	}

	if ( pDecInfo->ui32AllocDev < LX_AUD_DEV_DEC0 || pDecInfo->ui32AllocDev > LX_AUD_DEV_DEC1)
	{
		AUD_KDRV_ERROR("Check CB ui32AllocDev [%d][%x]\n",pDecInfo->ui32AllocDev,pDecInfo->ui32AllocDev  );
		return RET_ERROR;
	}

	/* Get a decoder number for allocated decoder. */
	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
		return RET_ERROR;
	}

	// Check a main decoder number
	if(pDevInfo->index != _gMainDecoderIndex)
	{
		AUD_KDRV_IMC_NOTI("SIF Error : index(%d), main(%d)\n", pDevInfo->index, _gMainDecoderIndex);
		return RET_OK;
	}

	//Resets a AAD clock registers.
	(void)MIXED_IP_AUDIO_ResetAADClockRegisters();

	//Update DSP1 Clock Reset Counter for debug
	ui32ResetCount = AUDIO_ReadReg(LX_AUD_REG_DEBUG_DSP1_RESET);
	(void)AUDIO_WriteReg(LX_AUD_REG_DEBUG_DSP1_RESET, ui32ResetCount + 0x10000);
	AUD_KDRV_NOTI("AAD ClockFreq(%d) not changed(count = 0x%X)\n", pErrorParam->frequency, ui32ResetCount + 0x10000);

	AUD_KDRV_IMC_NOTI("SIF Error(%d) : Freq(%d)\n", allocDev, pErrorParam->frequency);
	return RET_OK;
}

/**
 * callback function to update decoded status.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_DecStatusCb(void *_param, int _paramLen, void *_cbParam)
{
	DecEvtDecIndexParam	*pDecIndexParam;
	AUD_DECODER_INFO_T 	*pDecInfo = NULL;

	pDecIndexParam = (DecEvtDecIndexParam *)_param;
	pDecInfo   = (AUD_DECODER_INFO_T *)_cbParam;

	if(_paramLen != sizeof(DecEvtDecIndexParam))
	{
		AUD_KDRV_ERROR("Dec Index Param : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtDecIndexParam), _paramLen);
		return RET_ERROR;
	}

	//Copy a additional decoded info.
	pDecInfo->decodedStatus.ui32DecodedIndex = pDecIndexParam->index;
	pDecInfo->decodedStatus.ui64Timestamp 	 = pDecInfo->ui64Timestamp[pDecIndexParam->index % CPB_AUI_INDEX_COUNT];

	// Print For Debug
	AUD_KDRV_IMC_NOTI("Dec Index : Idx(%5d), Ts(%10d)\n", pDecIndexParam->index, pDecIndexParam->timestamp);
	return RET_OK;
}

/**
 * callback function to update decoded end.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_DecodedEndCb(void *_param, int	_paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;
	SINT32	decIndex = 0;
	ULONG	flags;

	AUD_DECODER_INFO_T 	*pDecInfo = NULL;

	pDecInfo = (AUD_DECODER_INFO_T *)_cbParam;
	allocDev = pDecInfo->ui32AllocDev;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decIndex[%d] is not available.\n", decIndex);
		return RET_ERROR;
	}

	//Set a event if allocated device and event message is set for DECx.
	if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
	  &&(gAudSetEvent[allocDev].eventMsg  & LX_AUD_EVENT_DECODED_END) )
	{
		//spin lock for protection
		spin_lock_irqsave(&gAudEventSpinLock, flags);

		//Set a audio GET event type for next event.
		gAudGetEvent[allocDev].allocDev  = allocDev;
		gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DECODED_END;

		//spin unlock for protection
		spin_unlock_irqrestore(&gAudEventSpinLock, flags);
	}

	AUD_KDRV_IMC_NOTI("Dec(%d) Decoded End\n", pDecInfo->ui32AllocDev);
	return RET_OK;
}

/**
 * callback function to notificate DTS SPDIF Type inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_SpdifTypeCb(void *_param, int _paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;

	DecEvtReqSpdifType		*pSpdifType = NULL;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	pSpdifType = (DecEvtReqSpdifType *)_param;
	pDecInfo   = (AUD_DECODER_INFO_T *)_cbParam;
	allocDev   = pDecInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(DecEvtReqSpdifType))
	{
		AUD_KDRV_ERROR("TP Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtReqSpdifType), _paramLen);
		return RET_ERROR;
	}

	/* Check a DTS audio codec and SPDIF PCM Event. */
	if(pSpdifType->media_type == LX_AUD_CODEC_DTS_M6)
	{
		//SPDIF ES/PCM Support for DTS_M6 codec type.
		if(pSpdifType->spdif_type == ADEC_SPDIF_TYPE_ES)
		{
			pDecInfo->bSpdifESAvail = TRUE;
		}
		else if(pSpdifType->spdif_type == ADEC_SPDIF_TYPE_DTSCD)
		{
			pDecInfo->bSpdifESAvail = TRUE;
		}
		else
		{
			pDecInfo->bSpdifESAvail = FALSE;
		}
	}

	AUD_KDRV_IMC_NOTI("Dev(%d) : mediaType(%d), spdifType(%d)\n", allocDev, pSpdifType->media_type, pSpdifType->spdif_type);
	return RET_OK;
}

/**
 * callback function to update hdmi pcm format.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_HdmiPcmFormatCb(void *_param, int	_paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;
	SINT32	decIndex = 0;

	AUD_DECODER_INFO_T 	*pDecInfo = NULL;

	HdmiEvtPCMFormatParam	*pPcmFormatParam = NULL;

	pDecInfo = (AUD_DECODER_INFO_T *)_cbParam;
	pPcmFormatParam = (HdmiEvtPCMFormatParam *)_param;
	allocDev = pDecInfo->ui32AllocDev;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decIndex[%d] is not available.\n", decIndex);
		return RET_ERROR;
	}

	//Set a detail audio codec type.
	if(pPcmFormatParam->format == 0)	// 0 : PCM
	{
		pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_PCM;
	}
	else	// 1 : DTS-CD
	{
		AUD_DEV_INFO_T *pDevInfo = NULL;
		LX_AUD_DEV_INDEX_CTX_T	pcmRenIndexCtx;
		LX_AUD_DEV_INDEX_CTX_T	esRenIndexCtx;
		SINT32	retVal = RET_OK;

		if(pDecInfo->codecDetailType != LX_AUD_CODEC_DETAIL_DTS_CD)
		{
			pDecInfo->codecDetailType = LX_AUD_CODEC_DETAIL_DTS_CD;

			pDevInfo = AUDIO_GetDevInfo(allocDev);
			if(pDevInfo == NULL)
			{
				AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
				return RET_ERROR;
			}

			// find & Restart pcm renderer driver
			pcmRenIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
			pcmRenIndexCtx.ui32Index = pDevInfo->index;
			pcmRenIndexCtx.devOutType = LX_AUD_DEV_OUT_PCM;

			retVal = AUDIO_GetDevIndexCtx(&pcmRenIndexCtx);
			if(retVal != RET_OK)
			{
				AUD_KDRV_ERROR("AUDIO_GetDevIndexCtx(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}

			AUDIO_StopRenderer(pcmRenIndexCtx.dev);
			AUDIO_StopDecoder(pDecInfo->ui32AllocDev);

			AUDIO_FlushRenderer(pcmRenIndexCtx.dev);
			AUDIO_FlushDecoder(pDecInfo->ui32AllocDev);

			// find & Restart es renderer driver
			esRenIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
			esRenIndexCtx.ui32Index = pDevInfo->index;
			esRenIndexCtx.devOutType = LX_AUD_DEV_OUT_ES;

			retVal = AUDIO_GetDevIndexCtx(&esRenIndexCtx);
			if(esRenIndexCtx.dev != LX_AUD_DEV_NONE)
			{
				AUDIO_StopRenderer(esRenIndexCtx.dev);
				AUDIO_FlushRenderer(esRenIndexCtx.dev);
			}
		}

		AUD_KDRV_ERROR("Dev(%d) : codecDetailType(%d)\n", allocDev, pDecInfo->codecDetailType);
	}

	AUD_KDRV_IMC_NOTI("Dev(%d) : codecDetailType(%d)\n", allocDev, pDecInfo->codecDetailType);
	return RET_OK;
}

/**
 * callback function to update hdmi es discontinuity.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_HdmiEsDiscontinuityCb(void *_param, int	_paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;
	LX_AUD_DEV_INDEX_CTX_T	pcmRenIndexCtx;
	LX_AUD_DEV_INDEX_CTX_T	esRenIndexCtx;
	AUD_DEV_INFO_T *pDevInfo = NULL;
	SINT32	decIndex = 0;
	SINT32	retVal = RET_OK;

	AUD_DECODER_INFO_T 	*pDecInfo = NULL;
	HdmiEvtESDiscontinuityParam	*pEsDiscontinuityParam = NULL;

	pDecInfo = (AUD_DECODER_INFO_T *)_cbParam;
	pEsDiscontinuityParam = (HdmiEvtESDiscontinuityParam *)_param;
	allocDev = pDecInfo->ui32AllocDev;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decIndex[%d] is not available.\n", decIndex);
		return RET_ERROR;
	}

	//Set a detail audio codec type.
	if(pEsDiscontinuityParam->discontinuity == 1)	// 0 : Normal, 1 : Discontinuity
	{
		pDevInfo = AUDIO_GetDevInfo(allocDev);
		if(pDevInfo == NULL)
		{
			AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
			return RET_ERROR;
		}

		// find & Restart pcm renderer driver
		pcmRenIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
		pcmRenIndexCtx.ui32Index = pDevInfo->index;
		pcmRenIndexCtx.devOutType = LX_AUD_DEV_OUT_PCM;

		retVal = AUDIO_GetDevIndexCtx(&pcmRenIndexCtx);
		if(retVal != RET_OK)
		{
			AUD_KDRV_ERROR("AUDIO_GetDevIndexCtx(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}

		AUDIO_StopRenderer(pcmRenIndexCtx.dev);
		AUDIO_StopDecoder(pDecInfo->ui32AllocDev);

		AUDIO_FlushRenderer(pcmRenIndexCtx.dev);
		AUDIO_FlushDecoder(pDecInfo->ui32AllocDev);

		// find & Restart es renderer driver
		esRenIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
		esRenIndexCtx.ui32Index = pDevInfo->index;
		esRenIndexCtx.devOutType = LX_AUD_DEV_OUT_ES;

		retVal = AUDIO_GetDevIndexCtx(&esRenIndexCtx);
		if(esRenIndexCtx.dev != LX_AUD_DEV_NONE)
		{
			AUDIO_StopRenderer(esRenIndexCtx.dev);
			AUDIO_FlushRenderer(esRenIndexCtx.dev);
			AUDIO_StartRenderer(esRenIndexCtx.dev);
		}

		AUDIO_InitDecoder(pDecInfo->ui32AllocDev);
		AUDIO_StartRenderer(pcmRenIndexCtx.dev);
		AUDIO_StartDecoder(pDecInfo->ui32AllocDev);
	}

	AUD_KDRV_IMC_NOTI("Dev(%d) : discontinuity(%d)\n", allocDev, pEsDiscontinuityParam->discontinuity);
	return RET_OK;
}

/**
 * Register Notification.
 * @param 	pDecInfo		[in] decoder information.
 * @param 	pFuncImcNoti	[in] pointer to callback function.
 * @param 	allocMod		[in] allocated module.
 * @param 	event			[in] event.
 * @return 	void.
 * @see		_AUDIO_RegisterDecoderNoti().
 */
static void _AUDIO_RegisterDecoderNoti(AUD_DECODER_INFO_T* pDecInfo, PFN_ImcNoti pFuncImcNoti, UINT32 allocMod,	\
									   UINT32 event, IMC_ACTION_REPEAT_TYPE repeatType, SINT32 notiLevel)
{
	UINT32							actionID;
	AUD_EVENT_T						*pDecEvent = NULL;

	if(pDecInfo->ui32EventNum >= AUD_EVENT_NUM)
	{
		AUD_KDRV_ERROR("DecEventNum(%d) is over AUD_EVENT_NUM. \n", pDecInfo->ui32EventNum);
		return;
	}

	actionID = AUDIO_IMC_RegisterEvent((void*)pDecInfo, pFuncImcNoti, allocMod,	\
		event, repeatType, notiLevel);

	//if(repeatType != IMC_ACTION_ONCE)
	{
		pDecEvent = &pDecInfo->decEvent[pDecInfo->ui32EventNum];
		pDecEvent->event = event;
		pDecEvent->actionID = actionID;
		pDecEvent->moduleID = allocMod;
		pDecInfo->ui32EventNum++;
	}

	AUD_KDRV_IMC_NOTI("ev(0x%x) actID(0x%x), mod(0x%x).\n", event, actionID, allocMod);
	return;
}

/**
 * Register Event.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	void.
 * @see		AUDIO_SetDecoderParam().
 */
static void _AUDIO_RegisterDecoderEvent(UINT32 allocDev)
{
	SINT32					decIndex = 0;
	ADEC_MODULE_ID			allocMod_Dec = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_Source = ADEC_MODULE_NOT_DEF;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return;
	}

	pDecInfo = &_gDecoderInfo[decIndex];

	// Decoder Modules are needed to initialize
	allocMod_Dec = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(allocMod_Dec != ADEC_MODULE_NOT_DEF)
	{
		_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_DecodedInfoCb, allocMod_Dec, DEC_EVT_ES_DEC_INFO, IMC_ACTION_REPEAT, 1);
		_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_DecErrorCb, allocMod_Dec, DEC_EVT_DEC_ERROR, IMC_ACTION_REPEAT, 1);

		if(pDecInfo->decParam.input == LX_AUD_INPUT_SYSTEM)
		{
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_DecStatusCb, allocMod_Dec, DEC_EVT_DECODED_INDEX, IMC_ACTION_REPEAT, 100);
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_DecodedEndCb, allocMod_Dec, DEC_EVT_DECODED_END, IMC_ACTION_ONCE, 1);
		}

		if(pDecInfo->decParam.codecType == LX_AUD_CODEC_DTS_M6)
		{
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_SpdifTypeCb,
				allocMod_Dec, DEC_EVT_REQ_SPDIF_TYPE, IMC_ACTION_REPEAT, 1);
		}
	}

	allocMod_Source = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
	if(allocMod_Source != ADEC_MODULE_NOT_DEF)
	{
		if((pDecInfo->decParam.input == LX_AUD_INPUT_TP0) || (pDecInfo->decParam.input == LX_AUD_INPUT_TP1))
		{
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_TpErrorCb,
				allocMod_Source, TP_EVT_PDEC_ERROR, IMC_ACTION_REPEAT, 1);
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_TpIsDecodableCb,
				allocMod_Source, TP_EVT_ES_EXIST_INFO, IMC_ACTION_REPEAT, 1);
		}
		else if(pDecInfo->decParam.input == LX_AUD_INPUT_SIF)
		{
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_SifFreqErrorCb,
				allocMod_Source, SIF_EVT_UNSTABLE_FREQ_IN_SIF, IMC_ACTION_REPEAT, 1);
		}
		else if ( (pDecInfo->decParam.input == LX_AUD_INPUT_HDMI)	\
				||(pDecInfo->decParam.input >= LX_AUD_INPUT_HDMI0 && pDecInfo->decParam.input <= LX_AUD_INPUT_HDMI3) )
		{
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_HdmiPcmFormatCb,
				allocMod_Source, HDMI_EVT_PCM_FORMAT, IMC_ACTION_REPEAT, 1);
			_AUDIO_RegisterDecoderNoti(pDecInfo, (PFN_ImcNoti)_AUDIO_HdmiEsDiscontinuityCb,
				allocMod_Source, HDMI_EVT_ES_DISCONTINUITY, IMC_ACTION_REPEAT, 1);
		}
	}

	return;
}

/**
 * Init Decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		_AUDIO_DecErrorCb().
 */
static SINT32 AUDIO_InitDecoder(UINT32 allocDev)
{
	UINT32					allocMod_Dec;
	SINT32					decIndex = 0;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &(_gDecoderInfo[decIndex]);

	//Get Decoder Module
	allocMod_Dec = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(allocMod_Dec != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocMod_Dec);
	}

	pDecInfo->status = AUD_STATUS_INIT;

	AUD_DEC_UNLOCK();

	return RET_OK;
}

/**
 * Start Decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder(), KDRV_AUDIO_RestartDecoder().
 */
static SINT32 AUDIO_StartDecoder(UINT32 allocDev)
{
	ADEC_MODULE_ID			moduleId = ADEC_MODULE_NOT_DEF;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;
	SINT32					decIndex = 0;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &(_gDecoderInfo[decIndex]);

	// Start bypass
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_BYPASS);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	// Start decoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	// Start reformatter
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_REFOMATTER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	// Start input
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	//update driver status.
	pDecInfo->status = AUD_STATUS_START;

	AUD_DEC_UNLOCK();

	return RET_OK;
}

/**
 * Stop Decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder(), KDRV_AUDIO_ResetDecoder().
 */
static SINT32 AUDIO_StopDecoder(UINT32 allocDev)
{
	ADEC_MODULE_ID			moduleId = ADEC_MODULE_NOT_DEF;
	AUD_MOD_INFO_T			*pModInfo = NULL;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;
	SINT32					decIndex = 0;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &(_gDecoderInfo[decIndex]);

	if((pDecInfo->decParam.input != LX_AUD_INPUT_TP0) &&
	   (pDecInfo->decParam.input != LX_AUD_INPUT_TP1))
	{
		// Stop input
		moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
		if(moduleId != ADEC_MODULE_NOT_DEF)
		{
			pModInfo = AUDIO_GetModuleInfo(moduleId);
			if(pModInfo->countOutConnection <= 1)
			{
				AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
			}
		}

		// Stop reformatter
		moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_REFOMATTER);
		if(moduleId != ADEC_MODULE_NOT_DEF)
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
		}
	}

	// Stop decoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
	}

	// Stop bypass
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_BYPASS);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
	}

	pDecInfo->status = AUD_STATUS_STOP;

	AUD_DEC_UNLOCK();

	return RET_OK;
}

/**
 * Flush Decoder.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder(), KDRV_AUDIO_ResetDecoder().
 */
static SINT32 AUDIO_FlushDecoder(UINT32 allocDev)
{
	ImcCmdFlushParam	flushParam;
	ADEC_MODULE_ID		moduleId = ADEC_MODULE_NOT_DEF;
	AUD_MOD_INFO_T		*pModInfo = NULL;
	AUD_DECODER_INFO_T	*pDecInfo = NULL;
	SINT32				decIndex = 0;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &(_gDecoderInfo[decIndex]);

	memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
	flushParam.num_of_port	= 1;
	flushParam.port_list[0]	= MOD_IN_PORT(0);

	if((pDecInfo->decParam.input == LX_AUD_INPUT_TP0) ||
	   (pDecInfo->decParam.input == LX_AUD_INPUT_TP1))
	{
		// Stop input
		moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
		if(moduleId != ADEC_MODULE_NOT_DEF)
		{
			pModInfo = AUDIO_GetModuleInfo(moduleId);
			if(pModInfo->countOutConnection <= 1)
			{
				AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
			}
		}

		// Stop reformatter
		moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_REFOMATTER);
		if(moduleId != ADEC_MODULE_NOT_DEF)
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
		}
	}

	// Flush input
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		pModInfo = AUDIO_GetModuleInfo(moduleId);
		if(pModInfo->countOutConnection <= 1)
		{
			AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
		}
	}

	// Flush reformatter
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_REFOMATTER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
	}

	// Flush decoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
	}

	// Flush bypass
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_BYPASS);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
	}

	AUD_DEC_UNLOCK();

	return RET_OK;
}

/**
 * Set a Decoder Output Mode.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	outMode			[in] output mode.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_SetDecoderOutMode(UINT32 allocDev, LX_AUD_DECODER_OUT_MODE_T outMode)
{
	SINT32		retVal = RET_OK;
	SINT32		decIndex = 0;

	AUD_DECODER_INFO_T	*pDecInfo = NULL;
	ADEC_MODULE_ID		moduleId = ADEC_MODULE_NOT_DEF;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	pDecInfo = &_gDecoderInfo[decIndex];

	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_SET_OUTMODE, moduleId, sizeof(LX_AUD_DECODER_OUT_MODE_T), &outMode);
	}

	pDecInfo->decOutMode = outMode;

	AUD_KDRV_PRINT("Output Mode[%d, %d] \n", allocDev, outMode);

	return retVal;
}

/**
 * Set decoder parameters.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecParam		[in] parameters to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder(), KDRV_AUDIO_ResetDecoderParam().
 */
static SINT32 AUDIO_SetDecoderParam(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	SINT32	decIndex = 0;
	SINT32	retVal 	 = 0;

	ReformCmdSetParam               setReformatterParam;

	ADEC_MODULE_ID					decoderModule	= ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					inputModule		= ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					reformModule	= ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					headModule		= ADEC_MODULE_NOT_DEF;
	AUD_BUFFER_TYPE_T				bufType			= AUD_BUFFER_TYPE_NONE;

	AUD_DECODER_INFO_T				*pDecInfo = NULL;
	AUD_DEV_INFO_T					*pDevInfo = NULL;
	LX_AUD_DECODER_PARAM_T			*pDecInfoParam = NULL;

	LX_AUD_CLOCK_SRC_T				ui8AdecClockSrc = LX_AUD_CLOCK_DTV;
	UINT32							spdifOutput 	= LX_AUD_MASTER_SPDIF_PCM;
	AUD_INT_T						interrupt		= AUD_INT_NOTDEF;

	//Sanity check
	if(pDecParam == NULL)
	{
		AUD_KDRV_ERROR("pDecParam is NULL\n");
		return RET_ERROR;
	}

	//Get a decoder index
	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d:%d) is invalid!!!\n", decIndex,allocDev );
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &_gDecoderInfo[decIndex];

	/* Get a decoder number for allocated decoder. */
	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_DEC_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
		return RET_ERROR;
	}

	//Copy a decoder parameter to global value
	pDecInfoParam = &(pDecInfo->decParam);
	memcpy(pDecInfoParam, pDecParam, sizeof(LX_AUD_DECODER_PARAM_T));

	if (g_AudKdrv.bSuspended != TRUE && g_DuringReset != TRUE)
	{
		if (pDecInfoParam->ui32LengthOfDSI > MAX_LENGTH_OF_DSI)
		{
			pDecInfoParam->ui32LengthOfDSI = MAX_LENGTH_OF_DSI;
		}

		if(pDecInfoParam->ui32LengthOfDSI != 0)
		{
			if ( copy_from_user(&_gDecoderDSI[decIndex], (void __user *)pDecParam->pDSIBuffer, sizeof(UINT8)*pDecInfoParam->ui32LengthOfDSI) )
			{
				AUD_DEC_UNLOCK();
				AUD_KDRV_ERROR("copy_from_user(ui32LengthOfDSI:%d) Err\n", pDecInfoParam->ui32LengthOfDSI);
				return RET_ERROR;
			}

			pDecInfoParam->pDSIBuffer = &_gDecoderDSI[decIndex];
		}
	}

	if(pDecParam->codecType == LX_AUD_CODEC_UNKNOWN ||
	   pDecParam->codecType == LX_AUD_CODEC_ADPCM ||
	   pDecParam->codecType == LX_AUD_CODEC_AC3_ENC)
	{
		AUD_DEC_UNLOCK();
		AUD_KDRV_ERROR("Not Supported Audio Codec.[%d] \n", pDecParam->codecType);
		return RET_ERROR;
	}

	// Set a codec parameters for audio input module.
	inputModule= _AUDIO_SetDecoderParam_Input(allocDev, pDecParam);

	//Change a original value.(temp code, 2013.11.05) after _AUDIO_SetDecoderParam_Input.
	//pDecParam->codecType = LX_AUD_CODEC_AC3;

	// Set a codec parameters for audio decoder module.
	decoderModule = _AUDIO_SetDecoderParam_Codec(allocDev, pDecParam);

	if(decoderModule == ADEC_MODULE_NOT_DEF || inputModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_DEC_UNLOCK();
		AUD_KDRV_ERROR("decoder(%d) or input(%d) module is invalid!!!\n", decoderModule, inputModule);
		return RET_ERROR;
	}

	if(_gbUseReformatter)
	{
		// Insert reformatter module
		if(pDecParam->codecType == LX_AUD_CODEC_AC3 ||
		   pDecParam->codecType == LX_AUD_CODEC_AAC ||
		   pDecParam->codecType == LX_AUD_CODEC_DRA ||
		   pDecParam->codecType == LX_AUD_CODEC_MP3 ||
		   pDecParam->codecType == LX_AUD_CODEC_DTS_M6)
		{
			if(pDecParam->codecType == LX_AUD_CODEC_DTS_M6 &&
				pDecInfo->codecDetailType == LX_AUD_CODEC_DETAIL_DTS_CD)
			{
				AUD_KDRV_DEBUG("If input is HDMI and DTS_CD, reformatter is not connected.\n");
			}
			else
			{
				reformModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_REFOMATTER);
				if(reformModule == ADEC_MODULE_NOT_DEF)
				{
					if(pDevInfo->index < DEV_DEC_NUM)
					{
						reformModule = ADEC_MODULE_REFORM_0 + pDevInfo->index;
						reformModule = AUDIO_AllocModule(ADEC_MOD_TYPE_REFOMATTER, ADEC_CORE_DSP0, reformModule);
					}
					else
					{
						reformModule = AUDIO_AllocModule(ADEC_MOD_TYPE_REFOMATTER, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
					}

					if(reformModule != ADEC_MODULE_NOT_DEF)
					{
						retVal = AUDIO_InsertModuleLast(allocDev, reformModule, AUD_BUFFER_TYPE_NONE);
						if(retVal < RET_OK)
						{
							AUD_DEC_UNLOCK();
							AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
							return RET_ERROR;
						}
					}
					else
					{
						AUD_DEC_UNLOCK();
						AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_REFOMATTER);
						return RET_ERROR;
					}
				}

				// reformatter setting
				setReformatterParam.codec_type = pDecParam->codecType;
				AUDIO_IMC_SendCmdParam(REFORM_CMD_SET_PARAM, reformModule, sizeof(setReformatterParam), &setReformatterParam);
			}
		}
	}

	// Insert decoder module
	retVal = AUDIO_InsertModuleLast(allocDev, decoderModule, AUD_BUFFER_TYPE_NONE);
	if(retVal != 0)
	{
		AUD_DEC_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	// Connect input module to decoder module
	if(pDecParam->input == LX_AUD_INPUT_SYSTEM)
	{
		bufType = AUD_BUFFER_TYPE_CPB;
		retVal = AUDIO_InsertModuleFirst(allocDev, inputModule, bufType);
		if(retVal != 0)
		{
			AUD_DEC_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}
	}
	else
	{
		headModule= AUDIO_FindHeadModule(allocDev);
		if(headModule == ADEC_MODULE_NOT_DEF)
		{
			AUD_DEC_UNLOCK();
			AUD_KDRV_ERROR("headModule(%d) module is invalid!!!\n", headModule);
			return RET_ERROR;
		}

		retVal = AUDIO_ConnectModules(inputModule, headModule, bufType);
		if(retVal != 0)
		{
			AUD_DEC_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_ConnectModules(%d.%d) failed(%d)!!!\n", inputModule, headModule, retVal);
			return RET_ERROR;
		}

		retVal = AUDIO_AddModule(allocDev, inputModule);
		if(retVal != 0)
		{
			AUD_DEC_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", inputModule, retVal);
			return RET_ERROR;
		}
	}

	// set device output type
	if( (pDecInfoParam->codecType == LX_AUD_CODEC_AC3) ||
	    (pDecInfoParam->codecType == LX_AUD_CODEC_AAC) ||
	    (pDecInfoParam->codecType == LX_AUD_CODEC_DTS_M6) )
	{
		pDevInfo->devOutType = LX_AUD_DEV_OUT_PCM_ES;
	}
	else
	{
		pDevInfo->devOutType = LX_AUD_DEV_OUT_PCM;
	}

	// Set a clock setting for main decoder number
	if(pDevInfo->index == _gMainDecoderIndex)
	{
		if(pDecParam->input == LX_AUD_INPUT_SIF)
		{
			//Set a SPDIF main audio source, SPDIF output type and DTO interrupt source.
			ui8AdecClockSrc = LX_AUD_CLOCK_ATV;
			spdifOutput 	= LX_AUD_MASTER_SPDIF_PCM;
			interrupt		= AUD_INT_PCM;

			#ifndef DISABLE_SCART_HW_BYPASS
			//Enable SCART bypass function for AUD reg.
			(void)AUDIO_WriteReg(LX_AUD_REG_AAD_BYPASS, 0X1);
			#endif	//#ifndef DISABLE_SCART_HW_BYPASS
		}
		else if ( (pDecParam->input == LX_AUD_INPUT_HDMI)	\
				||(pDecParam->input >= LX_AUD_INPUT_HDMI0 && pDecParam->input <= LX_AUD_INPUT_HDMI3) )
		{
			//Set a SPDIF main audio source, SPDIF output type and DTO interrupt source.
			ui8AdecClockSrc = LX_AUD_CLOCK_HDMI;
			if(pDecParam->codecType != LX_AUD_CODEC_PCM)
			{
				spdifOutput = LX_AUD_MASTER_SPDIF_ES;

				if(inputModule == ADEC_MODULE_HDMI_IN_0)
				{
					interrupt = AUD_INT_HDMIIN0;
				}
				else
				{
					interrupt = AUD_INT_HDMIIN1;
				}
			}
			else
			{
				spdifOutput = LX_AUD_MASTER_SPDIF_PCM;
				interrupt 	= AUD_INT_PCM;
			}

			#ifndef DISABLE_SCART_HW_BYPASS
			//Enable SCART bypass function for AUD reg.
			(void)AUDIO_WriteReg(LX_AUD_REG_AAD_BYPASS, 0X1);
			#endif	//#ifndef DISABLE_SCART_HW_BYPASS
		}
		else if(pDecParam->input == LX_AUD_INPUT_ADC)
		{
			//Set a SPDIF main audio source, SPDIF output type and DTO interrupt source.
			ui8AdecClockSrc = LX_AUD_CLOCK_ADC;
			spdifOutput 	= LX_AUD_MASTER_SPDIF_PCM;
			interrupt		= AUD_INT_PCM;

			#ifndef DISABLE_SCART_HW_BYPASS
			//Enable SCART bypass function for AUD reg.
			(void)AUDIO_WriteReg(LX_AUD_REG_AAD_BYPASS, 0X1);
			#endif	//#ifndef DISABLE_SCART_HW_BYPASS
		}
		else
		{
			//Set a SPDIF main audio source, SPDIF output type and DTO interrupt source.
			ui8AdecClockSrc = LX_AUD_CLOCK_DTV;
			spdifOutput 	= LX_AUD_MASTER_SPDIF_ES;
			interrupt		= AUD_INT_PCM;

			#ifndef DISABLE_SCART_HW_BYPASS
			//Disable SCART bypass function for AUD reg.
			(void)AUDIO_WriteReg(LX_AUD_REG_AAD_BYPASS, 0X0);
			#endif	//#ifndef DISABLE_SCART_HW_BYPASS
		}

		//Set default PCM frequency to 48Khz and main audio clock source and SPDIF default output to DSP
		(void)MIXED_IP_AUDIO_SetClockSource( LX_AUD_SAMPLING_FREQ_48_KHZ, ui8AdecClockSrc, spdifOutput);

		//Set DTO Interrupt Source to DSP
		(void)KDRV_AUDIO_SetDTOIntSrc(interrupt);

		//Set default PCM frequency to 48Khz and notify input sampling frequency to DSP
		(void)MIXED_IP_AUDIO_SetPcmClockRate(LX_AUD_SAMPLING_FREQ_48_KHZ, APLL_DSP_VALUE_12_288MHZ);

		//Set a SPDIF clock for M14 B0 Chip Bug
		if(lx_chip_rev() == LX_CHIP_REV(M14, B0)) // M14B0 only bug
		{
			//Set a default SPDIF sampling frequency to 48Khz : D_DTO
			(void)MIXED_IP_AUDIO_SetSPDIFSamplingFreq(LX_AUD_SAMPLING_FREQ_48_KHZ, DTO_A_CLOCK_CENTER_TAP_VALUE, AUD_DTO_D);
		}
		else
		{
			//Set a default SPDIF sampling frequency to 48Khz : A_DTO
			(void)MIXED_IP_AUDIO_SetSPDIFSamplingFreq(LX_AUD_SAMPLING_FREQ_48_KHZ, DTO_A_CLOCK_CENTER_TAP_VALUE, AUD_DTO_A);
		}

		AUD_KDRV_DEBUG_TMP("allocDev %d, index %d, Dec %d, MainIdx %d\n", allocDev, pDevInfo->index, decIndex, _gMainDecoderIndex);
	}

	//Register a decoder event
	_AUDIO_RegisterDecoderEvent(allocDev);

	AUD_DEC_UNLOCK();

	AUD_KDRV_PRINT("Dec %x, Input %d, Codec %d, Freq %d\n", decIndex, pDecParam->input, pDecParam->codecType, pDecParam->samplingFreq);

	return RET_OK;
}

/**
 * Set decoder mode.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecMode		[in] decoder mode.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_SetDecoderMode(UINT32 allocDev, LX_AUD_DECODER_MODE_T *pDecMode)
{
	SINT32	decIndex = 0;
	AUD_DECODER_INFO_T	*pDecInfo = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	pDecInfo = &_gDecoderInfo[decIndex];
	pDecInfo->decMode.drc = pDecMode->drc;
	pDecInfo->decMode.downmix	= pDecMode->downmix;
	pDecInfo->decMode.aacBypass = pDecMode->aacBypass;
	pDecInfo->decMode.tpTimerInterval = pDecMode->tpTimerInterval;
	pDecInfo->decMode.bSetAD = pDecMode->bSetAD;
	pDecInfo->decMode.bSetSubDec = pDecMode->bSetSubDec;

	AUD_KDRV_PRINT("AUDIO_SetDecoderMode(%d %d %d %d %d %d)\n", pDecInfo->decMode.drc, pDecInfo->decMode.downmix,	\
														  	    pDecInfo->decMode.aacBypass, pDecInfo->decMode.tpTimerInterval,	\
														  	    pDecInfo->decMode.bSetAD, pDecInfo->decMode.bSetSubDec);
	return RET_OK;
}

/**
 * Feed data.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecFeed		[in] Feeding Data.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_FeedDec(UINT32 allocDev, LX_AUD_DECODER_FEED_T *pDecFeed)
{
	SINT32					retVal = 0;
	SINT32					decIndex = 0;
	UINT32					ui32FreeAuCount;
	UINT64					ui64TS90kHzTick;

	ADEC_BUF_T				*pWriterStruct = NULL;
	ADEC_AU_INFO_T			info = {0, };
	LX_AUD_DECODER_FEED_T	decFeedData;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	if (pDecFeed == NULL)
	{
		AUD_KDRV_ERROR("Feed : pDecFeed is NULL !!!\n" );
		return RET_ERROR;
	}

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_BUF_LOCK();

	pDecInfo = &_gDecoderInfo[decIndex];

	pWriterStruct = _AUDIO_GetDecoderWriteStructure(allocDev);
	if (pWriterStruct == NULL)
	{
		AUD_DEC_BUF_UNLOCK();
		AUD_KDRV_ERROR("Feed : pWriterStruct is NULL !!!\n" );
		return RET_ERROR;
	}

	//Check a repeat number and buffer status to initialize audio buffer
	if(pDecInfo->bBufferRepeat == TRUE)
	{
		//buffer init : buffer is not flushed if repeat is set previously.
		pWriterStruct->init(pWriterStruct);

		//Clear a buffer repeat variable.
		pDecInfo->bBufferRepeat = FALSE;
	}

	memcpy(&decFeedData, pDecFeed, sizeof(LX_AUD_DECODER_FEED_T));

	if(decFeedData.ui32BufferSize > 0 || decFeedData.statusBuffer == LX_AUD_BUFFER_END)
	{
		AUD_KDRV_PRINT("Feed => Free:%d, Used:%d\n", pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_used_size(pWriterStruct));

		/* Compute free au count */
		ui32FreeAuCount = pWriterStruct->get_max_au(pWriterStruct) - pWriterStruct->get_au_cnt(pWriterStruct);

		/* Check buffer overflow and AUI overflow */
		if( (pWriterStruct->get_free_size(pWriterStruct) >=  decFeedData.ui32BufferSize) &&(ui32FreeAuCount > 1) )
		{
			if (decFeedData.ui32BufferSize > 0)
			{
				/* Check a Google TVP Path to use 90Khz gstc clock value. */
				if(decFeedData.bUseGstcClock == FALSE)
				{
					ui64TS90kHzTick = decFeedData.ui64TimeStamp;
					if(ui64TS90kHzTick != 0xFFFFFFFFFFFFFFFFULL)
					{
						ui64TS90kHzTick *= 9;
						do_div(ui64TS90kHzTick, 100000);	// = Xns * 90 000 / 1 000 000 000
					}
				}
				else
				{
					ui64TS90kHzTick = decFeedData.ui64TimeStamp;
				}

				//Set NEW AUI info.
				info.size			= (UINT64)decFeedData.ui32BufferSize;
				info.timestamp		= (UINT32)(ui64TS90kHzTick) & 0x0FFFFFFF;	//28 bit
				info.first_module_gstc = AUDIO_ReadReg(LX_AUD_REG_GSTC_LOW);
				info.gstc			= 0;
				info.index			= pDecInfo->decodedStatus.ui32FeededIndex;
				info.error			= 0;
				info.discontinuity	= 0;
				info.lastAu			= 0;

				AUD_KDRV_PRINT("pts[%10d], dts[%10d], idx[%5d], size[%5llu]\n", info.timestamp, info.gstc, info.index, info.size);

				//Write AUI info.
				retVal = pWriterStruct->create_new_au(&info, pWriterStruct);
				if(retVal != RET_OK)
				{
					AUD_DEC_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => create_new_au(ret = %d, cnt = %d, free = %d)!!!\n",	\
								retVal, pWriterStruct->get_au_cnt(pWriterStruct), pWriterStruct->get_free_size(pWriterStruct));
					return RET_ERROR;
				}

			    //Write Mix data
			    retVal = pWriterStruct->write_data(decFeedData.pBuffer, decFeedData.ui32BufferSize, pWriterStruct);
				if(retVal != RET_OK)
				{
					AUD_DEC_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => write_data(ret = %d, cnt = %d, free = %d)!!!\n",	\
								retVal, pWriterStruct->get_au_cnt(pWriterStruct), pWriterStruct->get_free_size(pWriterStruct));
					return RET_ERROR;
				}
			}

			//Check buffer status
			if(decFeedData.statusBuffer == LX_AUD_BUFFER_END)
			{
				//Set last buffer
				retVal = BufHelper_SetLast(pWriterStruct);
				if(retVal != RET_OK)
				{
					AUD_DEC_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => BufHelper_SetLast(ret = %d, free = %d, cnt = %d)!!!\n",	\
								retVal, pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
					return RET_ERROR;
				}

				//Set a repeat function and variable for buffer init.
				if(decFeedData.ui32RepeatNumber > 1)
				{
					pDecInfo->bBufferRepeat = TRUE;

					(void)pWriterStruct->set_repeat(decFeedData.ui32RepeatNumber - 1, pWriterStruct);
				}

				AUD_KDRV_DEBUG("Feed => BufHelper_SetLast(ret = %d, free = %d, cnt = %d)!!!\n",	\
							retVal, pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
			}

			pDecInfo->ui64Timestamp[pDecInfo->decodedStatus.ui32FeededIndex % CPB_AUI_INDEX_COUNT] = decFeedData.ui64TimeStamp;
			pDecInfo->decodedStatus.ui32FeededIndex++;

			pDecInfo->decodedStatus.ui32FeededCount++;
		}
		else
		{
#ifndef IC_CERTIFICATION
			AUD_KDRV_DEBUG("Feed => free = %d, count = %d!!!\n", 	\
							pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
#else
			AUD_KDRV_DEBUG_TMP("Feed => free = %d, count = %d!!!\n", 	\
								pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
#endif
			retVal = RET_ERROR;
		}
	}
	else
	{
		AUD_KDRV_ERROR("Feed => decFeedData.ui32BufferSize = %u!!!\n", decFeedData.ui32BufferSize);
		retVal = RET_ERROR;
	}

	AUD_DEC_BUF_UNLOCK();
	AUD_KDRV_PRINT("[AUD_DEC_IOW_FEED] bufSize=%u, bufStatus=%d, TS=%llu\n", decFeedData.ui32BufferSize, decFeedData.statusBuffer, decFeedData.ui64TimeStamp);

	return retVal;
}

/**
 * Get decodable information.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pIsDecodable	[in] is input stream decodable or not.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_IsDecodable(UINT32 allocDev, UINT32 *pIsDecodable)
{
	SINT32							decIndex = 0;

	if(pIsDecodable == NULL)
	{
		AUD_KDRV_ERROR("pIsDecodable is NULL !!!\n");
		return RET_ERROR;
	}

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	*pIsDecodable = _gDecoderInfo[decIndex].decIsDecodable;

	AUD_KDRV_PRINT("Device(%d) isDecodable(%d).\n", allocDev, *pIsDecodable);

	return RET_OK;
}

/**
 * Get decoded element stream information.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecodedInfo	[in] decoded es information.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_GetDecodedInfo(UINT32 allocDev, LX_AUD_DECODED_INFO_T *pDecodedInfo)
{
	SINT32		decIndex = 0;

	if(pDecodedInfo == NULL)
	{
		AUD_KDRV_ERROR("pDecodedInfo is NULL !!!\n");
		return RET_ERROR;
	}

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	memcpy(pDecodedInfo, &(_gDecoderInfo[decIndex].decodedInfo), sizeof(LX_AUD_DECODED_INFO_T));

	AUD_KDRV_PRINT("Device(%d)\n", allocDev);
	return RET_OK;
}

/**
 * Get decoded status.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pDecodedStatus	[in] decoded status.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_GetDecodedStatus(UINT32 allocDev, LX_AUD_DECODED_STATUS_T *pDecodedStatus)
{
	UINT32		ui32BufStatus = 0;
	SINT32		decIndex = 0;

	ADEC_BUF_T				*pWriterStruct = NULL;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	if(pDecodedStatus == NULL)
	{
		AUD_KDRV_ERROR("pDecodedStatus is NULL !!!\n");
		return RET_ERROR;
	}

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_BUF_LOCK();

	pDecInfo = &_gDecoderInfo[decIndex];

	if(pDecInfo->decParam.input == LX_AUD_INPUT_SYSTEM)
	{
		pWriterStruct = _AUDIO_GetDecoderWriteStructure(allocDev);
		if (pWriterStruct == NULL)
		{
			AUD_DEC_BUF_UNLOCK();
			AUD_KDRV_ERROR("pWriterStruct is NULL !!!\n");
			return RET_ERROR;
		}

		//Get a buffer info. from buffer interface
		pDecInfo->decodedStatus.ui32MaxMemSize  = pWriterStruct->get_max_size(pWriterStruct);
		pDecInfo->decodedStatus.ui32FreeMemSize = pWriterStruct->get_free_size(pWriterStruct);
		pDecInfo->decodedStatus.ui32MaxAuiSize  = pWriterStruct->get_max_au(pWriterStruct);
		pDecInfo->decodedStatus.ui32FreeAuiSize = pWriterStruct->get_max_au(pWriterStruct) - pWriterStruct->get_au_cnt(pWriterStruct) - 1;
	}
	else if(pDecInfo->decParam.input == LX_AUD_INPUT_TP0)
	{
		//Check a TPM Buffer status
		ui32BufStatus = AUDIO_ReadReg(LX_AUD_REG_TPM_BUFSIZE);

		//Get a buffer info. from buffer interface
		pDecInfo->decodedStatus.ui32MaxMemSize  = (ui32BufStatus >> 16) << 8;
		pDecInfo->decodedStatus.ui32FreeMemSize = (ui32BufStatus & 0xFFFF) << 8;
	}
	else if(pDecInfo->decParam.input == LX_AUD_INPUT_TP1)
	{
		//Check a TPA Buffer status
		ui32BufStatus = AUDIO_ReadReg(LX_AUD_REG_TPA_BUFSIZE);

		//Get a buffer info. from buffer interface
		pDecInfo->decodedStatus.ui32MaxMemSize  = (ui32BufStatus >> 16) << 8;
		pDecInfo->decodedStatus.ui32FreeMemSize = (ui32BufStatus & 0xFFFF) << 8;
	}
	else
	{
		// TO_DO
	}

	memcpy(pDecodedStatus, &(pDecInfo->decodedStatus), sizeof(LX_AUD_DECODED_STATUS_T));

	AUD_DEC_BUF_UNLOCK();
	AUD_KDRV_PRINT("Device(%d)\n", decIndex);
	return RET_OK;
}

/**
 * Get decoder started or not.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pStarted		[out] started or not.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_GetStartInfo().
 */
SINT32 AUDIO_GetDecStartInfo(UINT32 allocDev, UINT32 *pStarted)
{
	SINT32					decIndex = 0;
	AUD_DECODER_INFO_T		*pDecInfo = NULL;

	if(pStarted == NULL)
	{
		AUD_KDRV_ERROR("pStarted is NULL !!!\n");
		return RET_ERROR;
	}

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_DEBUG_TMP("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_OK;
	}

	pDecInfo = &_gDecoderInfo[decIndex];

	if(pDecInfo->status == AUD_STATUS_START)
		*pStarted = TRUE;
	else
		*pStarted = FALSE;

	return RET_OK;
}

/**
 * Close decoder.
 *
 * @param 	audDev		[in] a allocated decoder device information.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32	AUDIO_CloseDecoder(UINT32 allocDev)
{
	UINT32		i = 0;
	SINT32		decIndex = 0;
	SINT32		retVal = RET_OK;

	AUD_EVENT_T						*pDecEvent = NULL;
	ADEC_MODULE_ID					inputModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					headModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					decoderModule = ADEC_MODULE_NOT_DEF;
	AUD_DECODER_INFO_T				*pDecInfo = NULL;
	CmCmdDestroyModule				destroyModule;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	AUD_DEC_LOCK();

	pDecInfo = &_gDecoderInfo[decIndex];
	if(pDecInfo->bClosed == TRUE)
	{
		AUD_DEC_UNLOCK();
		AUD_KDRV_PRINT("decIndex[%d] is already closed. \n", decIndex);
		return RET_OK;
	}

	// Disconnect header module to input module
	if(pDecInfo->decParam.input != LX_AUD_INPUT_SYSTEM)
	{
		inputModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOURCE);
		headModule = AUDIO_FindHeadModule(allocDev);

		if((inputModule != ADEC_MODULE_NOT_DEF) && (headModule != ADEC_MODULE_NOT_DEF))
		{
			retVal = AUDIO_DisconnectModules(inputModule, headModule);
			if(retVal != RET_OK)
			{
				AUD_DEC_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d.%d) failed. \n", inputModule, headModule);
				return retVal;
			}
		}
	}

	AUDIO_DisconnectAllModules(allocDev);

	decoderModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
	if(decoderModule != ADEC_MODULE_NOT_DEF)
	{
		destroyModule.module_id = decoderModule;
		AUDIO_IMC_SendCmdParam(CM_CMD_DESTROY_MODULE, AUDIO_GetManagerModule(decoderModule), sizeof(destroyModule), &destroyModule);
	}

	AUDIO_FreeDev(allocDev);

	for(i = 0; i < pDecInfo->ui32EventNum; i++)
	{
		pDecEvent = &(pDecInfo->decEvent[i]);
		AUDIO_IMC_CancelEvent(pDecEvent->event , pDecEvent->moduleID, pDecEvent->actionID);
	}

	//Clear a decoder resource.
	pDecInfo->ui32EventNum	= 0;
	pDecInfo->bClosed 	 	= TRUE;
	pDecInfo->ui32AllocDev	= 0;

	AUD_DEC_UNLOCK();

	AUD_KDRV_PRINT("Close Device %d.\n", allocDev);

	return RET_OK;
}

static SINT32 _AUDIO_SetDecoderParamExt_DDT(UINT32 allocDev, LX_AUD_DECODER_PARAM_EXT_T *pDecParamExt)
{
	SINT32							decIndex = 0;

	AUD_DECODER_INFO_T				*pDecInfo = NULL;
	LX_AUD_DECODER_PARAM_EXT_T		*pDecInfoParamExt = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_DEBUG_TMP("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	if (pDecParamExt->ui32NumOfParam > NUM_OF_CODEC_DDT_INFO)
	{
		AUD_KDRV_NOTI("numOfParam(%d) is invalid!!!\n", pDecParamExt->ui32NumOfParam);
		return RET_INVALID_PARAMS;
	}

	pDecInfo = &_gDecoderInfo[decIndex];
	pDecInfoParamExt = &pDecInfo->decParamExt;

	memcpy(pDecInfoParamExt, pDecParamExt, sizeof(LX_AUD_DECODER_PARAM_EXT_T));

	AUD_KDRV_DEBUG("MS10_DDT(transcoding %d)\n", pDecInfoParamExt->param[0]);

	return RET_OK;
}

static SINT32 _AUDIO_SetDecoderParamExt_DTS(UINT32 allocDev, LX_AUD_DECODER_PARAM_EXT_T *pDecParamExt)
{
	SINT32							decIndex = 0;

	AUD_DECODER_INFO_T				*pDecInfo = NULL;
	LX_AUD_DECODER_PARAM_EXT_T		*pDecInfoParamExt = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_DEBUG_TMP("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	if (pDecParamExt->ui32NumOfParam > NUM_OF_CODEC_DTS_INFO)
	{
		AUD_KDRV_NOTI("numOfParam(%d) is invalid!!!\n", pDecParamExt->ui32NumOfParam);
		return RET_INVALID_PARAMS;
	}

	pDecInfo = &_gDecoderInfo[decIndex];
	pDecInfoParamExt = &pDecInfo->decParamExt;

	memcpy(pDecInfoParamExt, pDecParamExt, sizeof(LX_AUD_DECODER_PARAM_EXT_T));
	AUD_KDRV_DEBUG("DTS_M6(dts_seamless %d)\n", pDecInfoParamExt->param[0]);

	return RET_OK;
}

static SINT32 _AUDIO_SetDecoderParamExt_AC3(UINT32 allocDev, LX_AUD_DECODER_PARAM_EXT_T *pDecParamExt)
{
	SINT32							decIndex = 0;

	AUD_DECODER_INFO_T				*pDecInfo = NULL;
	LX_AUD_DECODER_PARAM_EXT_T		*pDecInfoParamExt = NULL;

	decIndex = GET_DEC_INDEX(allocDev);
	if ((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_DEBUG_TMP("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	if (pDecParamExt->ui32NumOfParam > NUM_OF_CODEC_AC3_INFO)
	{
		AUD_KDRV_NOTI("numOfParam(%d) is invalid!!!\n", pDecParamExt->ui32NumOfParam);
		return RET_INVALID_PARAMS;
	}

	pDecInfo = &_gDecoderInfo[decIndex];
	pDecInfoParamExt = &pDecInfo->decParamExt;

	memcpy(pDecInfoParamExt, pDecParamExt, sizeof(LX_AUD_DECODER_PARAM_EXT_T));
	AUD_KDRV_DEBUG("MS10_DDT(bypass_mode %d)\n", pDecInfoParamExt->param[0]);

	return RET_OK;
}

static SINT32 AUDIO_SetDecoderParamExt(UINT32 allocDev, LX_AUD_DECODER_PARAM_EXT_T *pDecParamExt)
{
	if (pDecParamExt == NULL)
	{
		AUD_KDRV_ERROR("pDecParamExt is NULL !!!\n");
		return RET_ERROR;
	}

	if (pDecParamExt->codecType >= LX_AUD_CODEC_MAX)
	{
		AUD_KDRV_ERROR("codecType is invalid !!!\n");
		return RET_INVALID_PARAMS;
	}

	switch(pDecParamExt->codecType)
	{
		case LX_AUD_CODEC_AAC:
		{
			_AUDIO_SetDecoderParamExt_DDT(allocDev, pDecParamExt);
		}
		break;

		case LX_AUD_CODEC_DTS_M6:
		{
			_AUDIO_SetDecoderParamExt_DTS(allocDev, pDecParamExt);
		}
		break;

		case LX_AUD_CODEC_AC3:
		{
			_AUDIO_SetDecoderParamExt_AC3(allocDev, pDecParamExt);
		}
		break;

		default:
		{
			AUD_KDRV_ERROR("%d is not supported !!!\n", pDecParamExt->codecType);
			return RET_ERROR;
		}
	}

	return RET_OK;
}

/**
 * Set decoder parameters for certification.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	decCertiParam	[in] parameters to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_SetDecoderCertiParam(UINT32 allocDev, UINT32 decCertiParam)
{
#ifdef IC_CERTIFICATION
	SINT32							decIndex		= 0;
	LX_AUD_DECODER_PARAM_T			*pDecInfoParam	= NULL;
	ADEC_MODULE_ID					decoderModule	= ADEC_MODULE_NOT_DEF;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		AUD_KDRV_ERROR("decoder index(%d) is invalid!!!\n", decIndex);
		return RET_ERROR;
	}

	decoderModule = AUDIO_FindModuleType(allocDev, LX_AUD_DEC);
	if(decoderModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("audio module is Unknown\n");
		return RET_ERROR;
	}

	pDecInfoParam = &(_gDecoderInfo[decIndex].decParam);

	if(pDecInfoParam->codecType == LX_AUD_CODEC_AC3)
	{
		DdcCmdSetParam ddcParam;

		ddcParam.drc_mode = 0;
		ddcParam.downmix_mode = 1;
		ddcParam.bypass_mode = 0;
		ddcParam.isDualDec = 0;
		ddcParam.certi_param = decCertiParam;

		AUDIO_IMC_SendCmdParam(DDC_CMD_SET_PARAM, decoderModule, sizeof(DdcCmdSetParam), &ddcParam);
	}
	else if(pDecInfoParam->codecType == LX_AUD_CODEC_AAC)
	{
		DdtCmdSetParam ddtParam;

		ddtParam.drc_mode = 0;
		ddtParam.downmix_mode = 1;
		ddtParam.isDualDec = 0;
		ddtParam.iec_fmt = 0;
		ddtParam.certi_param = decCertiParam;

		AUDIO_IMC_SendCmdParam(DDT_CMD_SET_PARAM, decoderModule, sizeof(DdtCmdSetParam), &ddtParam);
	}
	else if(pDecInfoParam->codecType == LX_AUD_CODEC_DTS_M6)
	{
		DtsCmdSetParam dtsParam;

		dtsParam.certi_param = decCertiParam;

		AUDIO_IMC_SendCmdParam(DTS_CMD_SET_PARAM, decoderModule, sizeof(DtsCmdSetParam), &dtsParam);
	}

	AUD_KDRV_ERROR("certi_param : %x.\n", decCertiParam);
#endif

	return RET_OK;
}

/**
 * Sets DTO Interrupt Source.
 * @see
*/
void KDRV_AUDIO_SetDTOIntSrc(AUD_INT_T interrupt)
{
	DtoCtrlCmdSetIntNum	setIntSrc;

	setIntSrc.RefInt	= interrupt;
	setIntSrc.TarInt	= AUD_INT_SPDIF;

	AUDIO_IMC_SendCmdParam(DTOCTRL_CMD_SET_INTNUM, ADEC_MODULE_DTO_0, sizeof(DtoCtrlCmdSetIntNum), &setIntSrc);
	return;
}

/**
 * Reset decoder device.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_ResetDecoder(void)
{
	SINT32						retVal = RET_OK;
	SINT32						i = 0, j = 0;

	ADEC_MODULE_ID				decoderModule = ADEC_MODULE_NOT_DEF;
	AUD_EVENT_T					*pDecEvent = NULL;
	LX_AUD_DEV_T				allocDev;
	CmCmdDestroyModule			destroyModule;
	AUD_DEV_INFO_T				*pDevInfo = NULL;
	AUD_MOD_INFO_T				*pModInfo = NULL;
	AUD_DECODER_INFO_T			*pDecInfo = NULL;

	for(i = 0; i < DEV_DEC_NUM; i++)
	{
		pDecInfo = &_gDecoderInfo[i];
		if(pDecInfo->decParam.input != LX_AUD_INPUT_SYSTEM || g_AudKdrv.bSuspended == TRUE)
		{
			allocDev = pDecInfo->ui32AllocDev;
			if(allocDev < LX_AUD_DEV_DEC0 || allocDev > LX_AUD_DEV_DEC1)
			{
				AUD_KDRV_DEBUG("allocDev [%d]\n", allocDev);
				continue;
			}

			pDecInfo->bResetting = TRUE;

			AUDIO_StopDecoder(allocDev);
			AUDIO_FlushDecoder(allocDev);

			AUDIO_DisconnectAllModules(allocDev);
			decoderModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
			if(decoderModule != ADEC_MODULE_NOT_DEF )
			{
				destroyModule.module_id = decoderModule;
				AUDIO_IMC_SendCmdParam(CM_CMD_DESTROY_MODULE, AUDIO_GetManagerModule(decoderModule), sizeof(destroyModule), &destroyModule);
				AUD_KDRV_PRINT("CM_CMD_DESTROY_MODULE [%s] \n", ModuleList_GetModuleName(decoderModule));
			}

			pDevInfo = AUDIO_GetDevInfo(allocDev);
			if(pDevInfo == NULL)
			{
				AUD_KDRV_DEBUG("pDevInfo[%d] is NULL.\n", allocDev);
				continue;
			}

			for(j = (pDevInfo->numMod - 1); j >= 0; j--)
			{
				pModInfo = pDevInfo->pModInfo[j];
				if(pModInfo == NULL)
				{
					AUD_KDRV_DEBUG("pModInfo[%d] is NULL.\n", allocDev);
					continue;
				}

				retVal = AUDIO_RemoveModule(allocDev, pModInfo->mod);
				if(retVal < RET_OK)
				{
					AUD_KDRV_DEBUG("AUDIO_RemoveModule(%d, %d) failed(%d)!!!\n", allocDev, pModInfo->mod, retVal);
					continue;
				}

				AUDIO_FreeModule(pModInfo->mod);

				pDevInfo->pModInfo[j] = NULL;
			}

			for(j = 0; j < pDecInfo->ui32EventNum; j++)
			{
				pDecEvent = &(pDecInfo->decEvent[j]);
				AUDIO_IMC_CancelEvent(pDecEvent->event , pDecEvent->moduleID, pDecEvent->actionID);
			}

			pDecInfo->ui32EventNum = 0;
		}
	}
}

/**
 * Reset decoder parameters.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_ResetDecoderParam(void)
{
	AUD_DECODER_INFO_T			*pDecInfo = NULL;
	LX_AUD_DECODER_PARAM_T		*pDecInfoParam = NULL;
	UINT32						allocDev;
	SINT32						i = 0;

	for(i = 0; i < DEV_DEC_NUM; i++)
	{
		pDecInfo = &_gDecoderInfo[i];
		if(pDecInfo->decParam.input != LX_AUD_INPUT_SYSTEM || g_AudKdrv.bSuspended == TRUE)
		{
			allocDev = pDecInfo->ui32AllocDev;
			if(allocDev == 0)
				continue;

			pDecInfoParam = &(pDecInfo->decParam);
			AUDIO_SetDecoderParam(allocDev, pDecInfoParam);
		}
	}
}

/**
 * Restart decoder.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_RestartDecoder(void)
{
	UINT32						allocDev;
	SINT32						i = 0;
	AUD_DECODER_INFO_T			*pDecInfo = NULL;

	for(i = 0; i < DEV_DEC_NUM; i++)
	{
		pDecInfo = &_gDecoderInfo[i];
		if(pDecInfo->decParam.input != LX_AUD_INPUT_SYSTEM || g_AudKdrv.bSuspended == TRUE)
		{
			allocDev = pDecInfo->ui32AllocDev;
			if(allocDev == 0)
				continue;

			AUDIO_StartDecoder(allocDev);

			pDecInfo->bResetting = FALSE;
		}
	}
}

/**
 * Get a decoded Information for debug.
 *
 * @see	AUDIO_GetDecodedInfo().
 */
void KDRV_AUDIO_GetDecodedInfo(LX_AUD_GET_DECODED_INFO_T *pDecInfo)
{
	SINT32	retVal 	 = RET_OK;
	SINT32	decIndex = 0;

	LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

	/* Get a allocated device number from index. */
	devIndexCtx.devType   = LX_AUD_DEV_TYPE_DEC;
	devIndexCtx.ui32Index = pDecInfo->ui32DecoderNum;
	retVal = AUDIO_GetDevIndexCtx(&devIndexCtx);
	if(retVal != RET_OK)
	{
		AUD_KDRV_ERROR("AUDIO_GetDevIndexCtx(%d) failed!!!\n", retVal);
		return;
	}

	decIndex = GET_DEC_INDEX(devIndexCtx.dev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		//Clears a all decoder information.
		memset(pDecInfo, 0, sizeof(LX_AUD_GET_DECODED_INFO_T));

		AUD_KDRV_PRINT("decoder index(%d) is invalid!!!\n", decIndex);
		return;
	}

	//Copy a decoded info. from global saved value from decoder callback info.
	pDecInfo->codecType 		=  _gDecoderInfo[decIndex].decodedInfo.codecType;
	pDecInfo->samplingFreq 		=  _gDecoderInfo[decIndex].decodedInfo.samplingFreq;
	pDecInfo->ui32AvgBitRate 	=  _gDecoderInfo[decIndex].decodedInfo.ui32AvgBitRate;
	pDecInfo->ui32NumOfChannel	=  _gDecoderInfo[decIndex].decodedInfo.ui32NumOfChannel;
	pDecInfo->ui32BitPerSample 	=  _gDecoderInfo[decIndex].decodedInfo.ui32BitPerSample;

	//Copy a decoder info. from global saved value.
	pDecInfo->ui32AllocDev		=  _gDecoderInfo[decIndex].ui32AllocDev;
	pDecInfo->input 			=  _gDecoderInfo[decIndex].decParam.input;
	pDecInfo->ui32IsDecodable 	=  _gDecoderInfo[decIndex].decIsDecodable;
	pDecInfo->ui32ChannelNum 	=  _gDecoderInfo[decIndex].ui32ChannelNum;
	pDecInfo->bSpdifESAvail		=  _gDecoderInfo[decIndex].bSpdifESAvail;
	pDecInfo->codecDetailType	=  _gDecoderInfo[decIndex].codecDetailType;

	AUD_KDRV_PRINT("KDRV_AUDIO_GetDecodedInfo(%d)\n", decIndex);
	return;
}

/**
 * Get a decoder Parameter.
 *
 * @see	KDRV_AUDIO_GetDecoderParam().
 */
void KDRV_AUDIO_GetDecoderParam(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam)
{
	SINT32	retVal 	 = RET_OK;
	SINT32	decIndex = 0;

	decIndex = GET_DEC_INDEX(allocDev);
	if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
	{
		//Clears a all decoder information.
		memset(pDecParam, 0, sizeof(LX_AUD_DECODER_PARAM_T));

		AUD_KDRV_PRINT("decoder index(%d) is invalid!!!\n", decIndex);
		return;
	}

	memcpy(pDecParam, &_gDecoderInfo[decIndex].decParam, sizeof(LX_AUD_DECODER_PARAM_T));

	AUD_KDRV_PRINT("KDRV_AUDIO_GetDecoderParam(%d, %d)\n", decIndex, pDecParam->codecType);
	return;
}

/**
 * Set if use reformatter or not for debug.
 *
 * @see	AUDIO_SetUseReformatter().
 */
void KDRV_AUDIO_SetUseReformatter(BOOLEAN bUseReformatter)
{
	_gbUseReformatter = bUseReformatter;
	AUD_KDRV_PRINT("KDRV_AUDIO_SetUseReformatter(%d)\n", bUseReformatter);
	return;
}

#ifdef USE_DESTROY_DECLIB
/**
 * Set if use reformatter or not for debug.
 *
 * @see	AUDIO_SetUseReformatter().
 */
void KDRV_AUDIO_DestroyDecLib(void)
{
	if(((_gDecoderInfo[0].decParam.codecType == LX_AUD_CODEC_AC3) && (_gDecoderInfo[1].decParam.codecType == LX_AUD_CODEC_AC3))	\
	 ||((_gDecoderInfo[0].decParam.codecType == LX_AUD_CODEC_AAC) && (_gDecoderInfo[1].decParam.codecType == LX_AUD_CODEC_AAC)))
	{
		if(_gpDecSemaInit == TRUE)
			AUD_DEC_LOCK();

		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, ADEC_MODULE_DEC_0);
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, ADEC_MODULE_DEC_1);
		if(_gDecoderInfo[0].decParam.codecType == LX_AUD_CODEC_AAC)
		{
			AUDIO_IMC_SendCmd(DDT_CMD_DESTROY_LIB, ADEC_MODULE_DEC_0);
			AUDIO_IMC_SendCmd(DDT_CMD_DESTROY_LIB, ADEC_MODULE_DEC_1);
		}
		else
		{
			AUDIO_IMC_SendCmd(DDC_CMD_DESTROY_LIB, ADEC_MODULE_DEC_0);
			AUDIO_IMC_SendCmd(DDC_CMD_DESTROY_LIB, ADEC_MODULE_DEC_1);
		}

		if(_gpDecSemaInit == TRUE)
			AUD_DEC_UNLOCK();

		AUD_KDRV_DEBUG("DestroyDecLib : Codec(%d)\n", _gDecoderInfo[0].decParam.codecType);
	}


	return;
}
#endif
/** @} */


