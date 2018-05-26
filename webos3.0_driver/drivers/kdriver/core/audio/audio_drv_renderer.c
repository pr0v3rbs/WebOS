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



/** @file	audio_kdrv_renderer.c
 *
 *  main driver implementation for  audio render device.
 *  audio render device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.25
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#include "base_drv.h"
#include "audio_cfg.h"
#include "audio_drv.h"

#include "buffer/adec_buf.h"
#include "buffer/adec_buf_helper.h"

#include "audio_imc_func.h"
#include "audio_drv_debug.h"
#include "audio_drv_manager.h"
#include "audio_drv_decoder.h"
#include "audio_drv_renderer.h"
#include "audio_drv_master.h"

// A/V Lipsync
#define TIMESTAMP_MASK						0x0FFFFFFF
#define TIMESTAMP_MASK_NO_PCR				0x7FFFFFFF
#define AUD_REN_DEFAULT_VOLUME				1024
#define AUD_REN_DEFULT_PORT					0xFF

// Mixer
#define AUD_REN_PORT_ENABLE					0x1
#define AUD_REN_PORT_DISABLE				0x0

#define AUD_REN_NUM_OF_CHANNEL_OF_SRC_MIX	 2		// channel of SRC output & MIX input
#define AUD_REN_BITS_PER_SAMPLE_OF_SRC_MIX	32		// bits per sample of SRC output & MIX input
#define AUD_REN_INIT_MUTE_LENGTH_MIX	  5120		// initial mute delay 112ms(1024, 23ms) & MIX input

#define COUNT_MIN_CHECK_START	1
#define COUNT_MAX_CHECK_START	3

// RTS operation mode
#define RTS_OP_MODE_CONTINUE		0			// Continuous mode (Control SRC). Default.
#define RTS_OP_MODE_SKIPPING		1			// Skipping mode (Flush Buffers).
#define RTS_OP_MODE_HYBRID			2			// Hybrid mode

#define RTS_THRESHOLD_SKIPPING		(384*60)	// 60ms: buffering level of RTS skipping mode
#define RTS_RECOVER_SKIPPING		(384*20)	// 20ms

#define RTS_THRESHOLD_UPPER			(384*40)	// (384*30)	// 30ms: 48KHz Stereo 32Bits ±âÁØ
#define RTS_RECOVER_UPPER			(384*20)	// (384*10)	// 10ms
#define RTS_THRESHOLD_LOWER			(384*0)		// (384*1)		//  1ms
#define RTS_RECOVER_LOWER			(384*30)	// (384*20)	// 20ms

// RTS output frequency
#define RTS_FAST_FREQ				47875		// 47750
#define RTS_NORMAL_FREQ				48000
#define RTS_SLOW_FREQ				48000		// 48125		// 48250

// Fade In/Out control - for RTS
#define FADE_IN_OUT_DISABLE			0			// Disable
#define FADE_IN_OUT_ENABLE_DEFAULT	1			// Enable
#define FADE_LENGTH_DEFAULT			8			// 9(512sample)->8(256smaple) for Reducing PCM Delay
#define FADE_LENGTH_RTS				7
#define FADE_LENGTH_1				1
#define FADE_LENGTH_0				0

// MIX wait length
#define MIX_WAIT_LENGTH_DEFAULT		512			// 2048(17408byte)->512(5120byte) for reducing PCM Delay
#define MIX_WAIT_LENGTH_RTS			256
#define MIX_WAIT_LENGTH_HDMI		256
#define MIX_WAIT_LENGTH_EMP			2048

#define MOD_CTRL_SRC_NUM			2

#define AUD_INPUTCTRL_DEFAULT_VOLUME	0x00800000
#define AUD_INPUTCTRL_DEFAULT_DELAY		0

#define DEFAULT_BYTE_PER_SEC			48000 * 2 * 4

//Presented PTS of LIP module(for checking A/V lipsync)
#define AUD_PRESENTED_PTS0_REG_ADDR		0x674
#define AUD_PRESENTED_PTS1_REG_ADDR		0x678

extern UINT32	_gModTypeNum[ADEC_MOD_TYPE_MAX];
extern VDEC_LIPSYNC_REG_T	*g_pVDEC_LipsyncReg;	//VDEC H/W Reg. for Lip Sync Control with ADEC H/W Reg.

// For Main Decoder Index for Clock Setting and SPDIF ES Output
extern AUD_DECODER_ID_T _gMainDecoderIndex;

AUD_RENDER_INFO_T	_gRenderInfo[DEV_REN_NUM];

UINT32	_gRenderCheckStartCount[DEV_REN_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BOOLEAN	_gRenSemaInit = FALSE;

spinlock_t	_gpRenSema;		// render spin_lock

#define AUD_REN_LOCK_INIT()		spin_lock_init(&_gpRenSema)
#define AUD_REN_LOCK()			spin_lock(&_gpRenSema)
#define AUD_REN_UNLOCK()		spin_unlock(&_gpRenSema)

#ifdef REN_USE_BUF_LOCK
#define AUD_REN_BUF_LOCK()		spin_lock(&_gpRenSema)
#define AUD_REN_BUF_UNLOCK()	spin_unlock(&_gpRenSema)
#else
#define AUD_REN_BUF_LOCK()
#define AUD_REN_BUF_UNLOCK()
#endif

AUD_RENDER_INPUT_CTRL_T	_gInputCtrlInfo[NUM_OF_INPUTCTRL] = {
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
	{ .ui32InputCtrlVolume = AUD_INPUTCTRL_DEFAULT_VOLUME,	.ui32InputCtrlMute = FALSE, .ui32InputCtrlDelay = AUD_INPUTCTRL_DEFAULT_DELAY, },
};

UINT32 _gRenderCheckSum = 0;	// to fix audio decoder not working

#ifdef DISABLE_SCART_HW_BYPASS
BOOLEAN	_gRenderScartReset = FALSE;
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

static SINT32 AUDIO_SetLipsync(UINT32 allocDev, LX_AUD_RENDER_PARAM_LIPSYNC_T *pParamLipsync);
static SINT32 AUDIO_SetBasetime(UINT32 allocDev, LX_AUD_RENDER_PARAM_BASETIME_T *pParamBasetime);
static SINT32 AUDIO_SetClockType(UINT32 allocDev, LX_AUD_RENDER_CLK_TYPE_T clockType);
static SINT32 AUDIO_EnableRTS(UINT32 allocDev, UINT32 bOnOff);
static SINT32 _AUDIO_SetRTSParam(UINT32 allocDev, LX_AUD_RENDER_RTS_PARAM_T *pRTSParam);
static SINT32 AUDIO_SetTrickPlayMode(UINT32 allocDev, UINT32 trickMode);
static SINT32 AUDIO_SetNodelayParam(UINT32 allocDev, LX_AUD_RENDER_PARAM_NODELAY_T *pParamNodelay);
static SINT32 AUDIO_GetPresentedPTS(UINT32 allocDev, UINT32 *pPresentedPTS);
static SINT32 AUDIO_SetChannelVolume(UINT32 allocDev, LX_AUD_RENDER_CH_VOLUME_T *pChVol);
static SINT32 _AUDIO_SetCtrlSrc(AUD_RENDER_INFO_T *pRenInfo);

/**
 * Register Notification.
 * @param 	pRenInfo		[in] renderer information.
 * @param 	pFuncImcNoti	[in] pointer to callback function.
 * @param 	allocMod		[in] allocated module.
 * @param 	event			[in] event.
 * @return 	void.
 * @see		AUDIO_SetClockType().
 */
static void _AUDIO_RegisterRendererNoti(
	AUD_RENDER_INFO_T* pRenInfo,
	PFN_ImcNoti	pFuncImcNoti,
	UINT32 allocMod,
	UINT32 event,
	IMC_ACTION_REPEAT_TYPE repeatType,
	SINT32 notiLevel)
{
	UINT32							actionID;
	AUD_EVENT_T						*pRenEvent 		= NULL;

	if(pRenInfo->ui32EventNum >= AUD_EVENT_NUM)
	{
		AUD_KDRV_ERROR("RenEventNum(%d) is over AUD_EVENT_NUM. \n", pRenInfo->ui32EventNum);
		return;
	}

	actionID = AUDIO_IMC_RegisterEvent((void*)pRenInfo, pFuncImcNoti, allocMod,	\
		event, repeatType, notiLevel);

	if(repeatType != IMC_ACTION_ONCE)
	{
		pRenEvent = &pRenInfo->renderEvent[pRenInfo->ui32EventNum];
		pRenEvent->event = event;
		pRenEvent->actionID = actionID;
		pRenEvent->moduleID = allocMod;
		pRenInfo->ui32EventNum++;
	}

	AUD_KDRV_IMC_NOTI("[0x%x 0x%x, 0x%x] \n", event, actionID, allocMod);

	return;
}

static SINT32 _AUDIO_InitRendererParam(UINT32 allocDev)
{
	ULONG	flags;
	SINT32	renIndex = 0;

	AUD_RENDER_INFO_T 	*pRenInfo = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenInfo = &_gRenderInfo[renIndex];
	memset(&(pRenInfo->renderStatus), 0, sizeof(LX_AUD_RENDERED_STATUS_T));

	memset(&(pRenInfo->renderEvent[0]), 0, sizeof(AUD_EVENT_T)*AUD_EVENT_NUM);
	pRenInfo->ui32EventNum = 0;

	memset(&(pRenInfo->renderConnect[0]), 0, sizeof(AUD_RENDER_CONNECT_T)*AUD_RENDER_CONNECT_NUM);
	pRenInfo->ui32ConnectNum = 0;

	memset(&(pRenInfo->renderBasetime), 0, sizeof(LX_AUD_RENDER_PARAM_BASETIME_T));

	pRenInfo->ui32AllocDev = allocDev;
	pRenInfo->status = AUD_STATUS_NONE;
	pRenInfo->bStartCalled = FALSE;
	pRenInfo->bClosed = FALSE;
	pRenInfo->bResetting = FALSE;
	pRenInfo->ui32Volume = AUD_REN_DEFAULT_VOLUME;
	pRenInfo->ui32MixerPort = AUD_REN_DEFULT_PORT;
	pRenInfo->renderParam.samplingFreq = LX_AUD_SAMPLING_FREQ_NONE;
	pRenInfo->bLipsyncOnOff = FALSE;
	pRenInfo->renderNodelay.ui32OnOff		= FALSE;
	pRenInfo->renderNodelay.ui32UThreshold	= 0xFFFFFFFF;
	pRenInfo->renderNodelay.ui32LThreshold	= 0xFFFFFFFF;
	pRenInfo->ui32Delay = 0;
#ifdef USE_LISPYNC_FREE
	pRenInfo->bUnderflowCalled = FALSE;
	pRenInfo->ui64TickUnderflow = 0;
	pRenInfo->ui32CountUnderflow = 0;
#endif
	// the setting of RTS module
	pRenInfo->rtsParam.bRTSOnOff			= FALSE;
	pRenInfo->rtsParam.opMode				= RTS_OP_MODE_HYBRID;
	pRenInfo->rtsParam.upperThreshold		= RTS_THRESHOLD_UPPER;
	pRenInfo->rtsParam.recoverUpper		= RTS_RECOVER_UPPER;
	pRenInfo->rtsParam.lowerThreshold		= RTS_THRESHOLD_LOWER;
	pRenInfo->rtsParam.recoverLower		= RTS_RECOVER_LOWER;
	pRenInfo->rtsParam.skippingThreshold	= RTS_THRESHOLD_SKIPPING;
	pRenInfo->rtsParam.recoverSkipping		= RTS_RECOVER_SKIPPING;
	pRenInfo->rtsParam.fastFreq			= RTS_FAST_FREQ;
	pRenInfo->rtsParam.normalFreq			= RTS_NORMAL_FREQ;
	pRenInfo->rtsParam.slowFreq			= RTS_SLOW_FREQ;
	pRenInfo->rtsParam.mixFadeLen			= FADE_LENGTH_RTS;
	pRenInfo->rtsParam.mixWaitLen			= MIX_WAIT_LENGTH_RTS;

	_gRenderCheckStartCount[renIndex] = 0;

	//spin lock for protection
	spin_lock_irqsave(&gAudEventSpinLock, flags);

	//Reset a audio SET event type for next event.
	gAudSetEvent[allocDev].allocDev = LX_AUD_DEV_NONE;
	gAudSetEvent[allocDev].eventMsg = LX_AUD_EVENT_MASK_NONE;

	//Reset a audio GET event type for next event.
	gAudGetEvent[allocDev].allocDev = LX_AUD_DEV_NONE;
	gAudGetEvent[allocDev].eventMsg = LX_AUD_EVENT_MASK_NONE;

	//spin unlock for protection
	spin_unlock_irqrestore(&gAudEventSpinLock, flags);

	AUD_KDRV_PRINT("renIndex %x.\n", renIndex);

	return RET_OK;
}

/**
 * open handler for audio render device
 *
 */
SINT32	KDRV_AUDIO_OpenRenderer(struct inode *inode, struct file *filp)
{
	LX_AUD_DEV_TYPE_T		devType = LX_AUD_DEV_TYPE_NONE;
	LX_AUD_DEV_T 			allocDev = LX_AUD_DEV_NONE;
	AUD_DEVICE_T			*pDev = NULL;

	if(_gRenSemaInit == TRUE)
		AUD_REN_LOCK();

	filp->private_data = kmalloc(sizeof(AUD_DEVICE_T), GFP_KERNEL);
	pDev = (AUD_DEVICE_T*)filp->private_data;

	devType = LX_AUD_DEV_TYPE_REN;
	pDev->devType = LX_AUD_DEV_TYPE_REN;

	// to fix audio decoder not working
	_gRenderCheckSum++;
	pDev->checksum = _gRenderCheckSum;

	if(_gRenSemaInit == TRUE)
		AUD_REN_UNLOCK();

	allocDev = AUDIO_OpenRenderer();
	if(allocDev == LX_AUD_DEV_NONE)
	{
		AUD_KDRV_ERROR("AUDIO_AllocDev(%d) failed.\n", devType);
		kfree(filp->private_data);
		return RET_ERROR;
	}

	pDev->allocDev = allocDev ;
	filp->private_data = pDev;

	AUD_KDRV_DEBUG("Open Render device %d file\n", allocDev);

	return RET_OK;
}

/**
 * close handler for audio render device
 *
 */
SINT32	KDRV_AUDIO_CloseRenderer(struct inode *inode, struct file *filp)
{
	SINT32	retVal = 0;
	SINT32	renIndex = 0;
	AUD_DEVICE_T *pDev;
	LX_AUD_DEV_T allocDev = LX_AUD_DEV_NONE;

	pDev = (AUD_DEVICE_T*)filp->private_data;
	if(pDev == NULL)
	{
		AUD_KDRV_ERROR("private_data is NULL\n");
		return RET_ERROR;
	}

	allocDev = pDev->allocDev;

	renIndex = GET_REN_INDEX(allocDev);
	if ((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renderer index(%d) is invalid!!!\n", renIndex);
		return RET_ERROR;
	}

	// to fix audio decoder not working
	if(_gRenderInfo[renIndex].ui32CheckSum == pDev->checksum)
	{
		retVal = AUDIO_CloseRenderer(allocDev);
		if(retVal != RET_OK)
			AUD_KDRV_PRINT("AUDIO_CloseRenderer() failed or already closed.\n");
	}
	else
	{
		AUD_KDRV_INFO("checksums are different(%d.%d)\n",
			_gRenderInfo[renIndex].ui32CheckSum, pDev->checksum);
	}

	kfree(pDev);

	AUD_KDRV_DEBUG("Close Render device %d file\n", allocDev);

	return retVal;
}


/**
 * ioctl handler for audio render device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlRenderer(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
long KDRV_AUDIO_IoctlRenderer(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	SINT32			retVal = RET_OK;
	SINT32			err = 0;
	UINT32			allocDev;
	AUD_DEVICE_T	*pDev = NULL;

	pDev = (AUD_DEVICE_T*)filp->private_data;
	if(pDev == NULL)
		return RET_ERROR;
	/*
	* check if IOCTL command is valid or not.
	* - if magic value doesn't match, return error (-ENOTTY)
	* - if command is out of range, return error (-ENOTTY)
	*
	* note) -ENOTTY means "Inappropriate ioctl for device.
	*/
	if(_IOC_TYPE(cmd) != AUD_REN_IOC_MAGIC)
	{
		DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if(_IOC_NR(cmd) > AUD_REN_IOC_MAXNR)
	{
		DBG_PRINT_WARNING("out of ioctl command. cmd_idx=%d\n", _IOC_NR(cmd) );
		return -ENOTTY;
	}

	/*
	* check if user memory is valid or not.
	* if memory can't be accessed from kernel, return error (-EFAULT)
	*/
	if(_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if(err)
	{
		DBG_PRINT_WARNING("memory access error. cmd_idx=%d, rw=%c%c, memptr=%p\n",
							_IOC_NR(cmd),
							(_IOC_DIR(cmd) & _IOC_READ)? 'r':'-',
							(_IOC_DIR(cmd) & _IOC_WRITE)? 'w':'-',
							(void*)arg );
		return -EFAULT;
	}

	allocDev = pDev->allocDev;

	AUD_KDRV_TRACE("cmd = %08X (cmd_idx=%d) allocDev = %d\n", cmd, _IOC_NR(cmd), allocDev);

	switch(cmd)
	{
		case AUD_REN_IOW_SET_PARAM:
		{
			LX_AUD_RENDER_PARAM_T		renParam;

			if(copy_from_user(&renParam, (void __user *)arg, sizeof(LX_AUD_RENDER_PARAM_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_PARAM] Called\n");
			retVal = AUDIO_SetRendererParam(allocDev, &renParam);
		}
		break;

		case AUD_REN_IO_START:
		{
			AUD_KDRV_TRACE("[AUD_REN_IOW_START] Called\n");
			retVal = AUDIO_StartRenderer(allocDev);
		}
		break;

		case AUD_REN_IO_STOP:
		{
			AUD_KDRV_TRACE("[AUD_REN_IOW_STOP] Called\n");
			retVal = AUDIO_StopRenderer(allocDev);
		}
		break;

		case AUD_REN_IO_FLUSH:
		{
			AUD_KDRV_TRACE("[AUD_REN_IOW_FLUSH] Called\n");
			retVal = AUDIO_FlushRenderer(allocDev);
		}
		break;

		case AUD_REN_IOW_FEED:
		{
			LX_AUD_RENDER_FEED_T		renFeed;

			if(copy_from_user(&renFeed, (void __user *)arg, sizeof(LX_AUD_RENDER_FEED_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_FEED] Called\n");
			retVal = AUDIO_FeedRen(allocDev, &renFeed);
		}
		break;

		case AUD_REN_IOW_SET_LIPSYNCPARAM:
		{
			LX_AUD_RENDER_PARAM_LIPSYNC_T		paramLipsync;

			if(copy_from_user(&paramLipsync, (void __user *)arg, sizeof(LX_AUD_RENDER_PARAM_LIPSYNC_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_LIPSYNCPARAM] Called\n");
			retVal = AUDIO_SetLipsync(allocDev, &paramLipsync);
		}
		break;

		case AUD_REN_IOW_ENABLE_LIPSYNC:
		{
			UINT32		bOnOff;

			if(copy_from_user(&bOnOff, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_ENABLE_LIPSYNC] Called\n");
			retVal = AUDIO_EnableLipsync(allocDev, bOnOff);
		}
		break;

		case AUD_REN_IOW_SET_BASETIME:
		{
			LX_AUD_RENDER_PARAM_BASETIME_T		paramBasetime;

			if(copy_from_user(&paramBasetime, (void __user *)arg, sizeof(LX_AUD_RENDER_PARAM_BASETIME_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_BASETIME] Called\n");
			retVal = AUDIO_SetBasetime(allocDev, &paramBasetime);
		}
		break;

		case AUD_REN_IOW_SET_CLOCKTYPE:
		{
			LX_AUD_RENDER_PARAM_CLOCKTYPE_T		paramClockType;

			if(copy_from_user(&paramClockType, (void __user *)arg, sizeof(LX_AUD_RENDER_PARAM_CLOCKTYPE_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_CLOCKTYPE] Called\n");
			retVal = AUDIO_SetClockType(allocDev, paramClockType.clocktype);
		}
		break;

		case AUD_REN_IOW_CONNECT:
		{
			UINT32	connectDev;

			if(copy_from_user(&connectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_CONNECT] Called\n");
			retVal = AUDIO_ConnectRenderer(allocDev, connectDev);

		}
		break;

		case AUD_REN_IOW_DISCONNECT:
		{
			UINT32	disconnectDev;

			if(copy_from_user(&disconnectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_DISCONNECT] Called\n");
			retVal = AUDIO_DisconnectRenderer(allocDev, disconnectDev);
		}
		break;

		case AUD_REN_IOR_GET_KDRV_HANDLE:
		{
			AUD_KDRV_TRACE("[AUD_REN_IOW_DISCONNECT] Called\n");

			if(copy_to_user((void *)arg, (void *)&(pDev->allocDev), sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_REN_IO_CLOSE_DEVICE:
		{
			AUD_KDRV_TRACE("[AUD_REN_IO_CLOSE_DEVICE] Called\n");
			retVal = AUDIO_CloseRenderer(allocDev);
		}
		break;

		case AUD_REN_IOR_GET_STATUS:
		{
			LX_AUD_RENDERED_STATUS_T renderedStatus;

			AUD_KDRV_TRACE("[AUD_REN_IOW_GET_STATUS] Called\n");

			retVal = AUDIO_GetRenderedStatus(allocDev, &renderedStatus);

			if(copy_to_user((void *)arg, (void *)&(renderedStatus), sizeof(LX_AUD_RENDERED_STATUS_T)))
				return RET_ERROR;
		}
		break;

		case AUD_REN_IOW_ENABLE_RTS:
		{
			UINT32		bOnOff;

			if(copy_from_user(&bOnOff, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_ENABLE_RTS] Called\n");
			retVal = AUDIO_EnableRTS(allocDev, bOnOff);
		}
		break;

		case AUD_REN_IOW_SET_RTS_PARAM:
		{
			LX_AUD_RENDER_RTS_PARAM_T 		rtsParam;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_RTS_PARAM] Called\n");

			if(copy_from_user(&rtsParam, (void __user *)arg, sizeof(LX_AUD_RENDER_RTS_PARAM_T)))
				return RET_ERROR;

			retVal = _AUDIO_SetRTSParam(allocDev, &rtsParam);
		}
		break;

		case AUD_REN_IOW_TRICK_PLAY:
		{
			UINT32		trickMode;

			if(copy_from_user(&trickMode, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_TRICK_PLAY] Called\n");
			retVal = AUDIO_SetTrickPlayMode(allocDev, trickMode);
		}
		break;

		case AUD_REN_IOW_SET_NODELAYPARAM:
		{
			LX_AUD_RENDER_PARAM_NODELAY_T		paramNodelay;

			if(copy_from_user(&paramNodelay, (void __user *)arg, sizeof(LX_AUD_RENDER_PARAM_NODELAY_T)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_NODELAYPARAM] Called\n");
			retVal = AUDIO_SetNodelayParam(allocDev, &paramNodelay);
		}
		break;

		case AUD_REN_IOW_SET_INDEX_CTX:
		{
			LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

			if ( copy_from_user(&devIndexCtx, (void __user *)arg, sizeof(LX_AUD_DEV_INDEX_CTX_T)) )
			{
				AUD_KDRV_ERROR("[AUD_REN_IOW_SET_INDEX_CTX] copy_from_user() Err\n");
				return RET_ERROR;
			}

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_INDEX_CTX] id =%d, ctx = %p\n",
				devIndexCtx.ui32Index, devIndexCtx.pCtx);
			retVal = AUDIO_SetDevIndexCtx(allocDev, &devIndexCtx);
		}
		break;

		case AUD_REN_IOW_SET_INPUTCTRL_VOLUME:
		{
			UINT32		volume;

			if(copy_from_user(&volume, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_INPUTCTRL_VOLUME] Called\n");
			retVal = AUDIO_SetInputCtrlVolume(allocDev, volume);
		}
		break;

		case AUD_REN_IOW_SET_INPUTCTRL_MUTE:
		{
			UINT32		mute;

			if(copy_from_user(&mute, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_INPUTCTRL_MUTE] Called\n");
			retVal = AUDIO_SetInputCtrlMute(allocDev, mute);
		}
		break;

		case AUD_REN_IOW_SET_INPUTCTRL_DELAY:
		{
			UINT32		delay;

			if(copy_from_user(&delay, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_INPUTCTRL_DELAY] Called\n\n");
			retVal = AUDIO_SetInputCtrlDelay(allocDev, delay);
		}
		break;

		case AUD_REN_IOR_GET_PRESENTED_PTS:
		{
			UINT32 		presentedPTS;

			AUD_KDRV_TRACE("[AUD_REN_IOR_GET_PRESENTED_PTS] Called\n");

			retVal = AUDIO_GetPresentedPTS(allocDev, &presentedPTS);

			if(copy_to_user((void *)arg, (void *)&(presentedPTS), sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_REN_IOW_SET_CH_VOLUME:
		{
			LX_AUD_RENDER_CH_VOLUME_T 		chVol;

			AUD_KDRV_TRACE("[AUD_REN_IOW_SET_CH_VOLUME] Called\n");

			if(copy_from_user(&chVol, (void __user *)arg, sizeof(LX_AUD_RENDER_CH_VOLUME_T)))
				return RET_ERROR;

			retVal = AUDIO_SetChannelVolume(allocDev, &chVol);
		}
		break;

		default:
		{
			/* redundant check but it seems more readable */
			AUD_KDRV_ERROR("Invalid IOCTL Call!!!\n");
			retVal = RET_INVALID_IOCTL;
		}
		break;
	}

	return retVal;
}

static ADEC_BUF_T* _AUDIO_GetRendererWriteStructure(UINT32 allocDev)
{
	ADEC_MODULE_ID			mod = ADEC_MODULE_NOT_DEF;
	ADEC_BUF_T				*pWriterStruct = NULL;
	SINT32					renIndex = 0;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return NULL;
	}

	// Get Module connected to Buffer
	if(_gRenderInfo[renIndex].renderParam.lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
	{
		mod = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
		if(mod == ADEC_MODULE_NOT_DEF)
		{
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_PCMCVT);
			return NULL;
		}
	}
	else
	{
		mod = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		if(mod == ADEC_MODULE_NOT_DEF)
		{
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_INPUTCTRL);
			return NULL;
		}
	}

	pWriterStruct = AUDIO_GetBuffer(mod, IN_PORT);

	AUD_KDRV_PRINT("dev(%d) mod(%d).\n", allocDev, mod);

	return pWriterStruct;
}

static void _AUDIO_EnableMixerPort(UINT32 port, BOOLEAN onOff)
{
	ADEC_MODULE_ID	mixModule = ADEC_MODULE_MIX_0;
	MixCmdEnablePort	mixEnable;

	mixEnable.port = port;

	if(onOff == ON)
		mixEnable.enable = AUD_REN_PORT_ENABLE;
	else
		mixEnable.enable = AUD_REN_PORT_DISABLE;

	AUDIO_IMC_SendCmdParam(MIX_CMD_ENABLE_PORT, mixModule, sizeof(MixCmdEnablePort), &mixEnable);
}

static void _AUDIO_SetSRCFormat(UINT32 allocMod, UINT32 sampleFreq, UINT32 numOfChannel, UINT32 bitPerSample, UINT32 endianType, UINT32 signedType)
{
	PcmcvtCmdSetAllfmt 	 	srcSetFmt;
	PcmcvtCmdSetOverProtect srcSetOverProtect;

	srcSetFmt.InFs		= sampleFreq;
	srcSetFmt.InCh		= numOfChannel;
	srcSetFmt.InFormat	= bitPerSample;
	srcSetFmt.InEndian	= endianType;	// 0 is little , 1 is big
	srcSetFmt.InSigned	= signedType;	// 0 is signed , 1 is unsigned
	srcSetFmt.OutFs		= LX_AUD_SAMPLING_FREQ_48_KHZ;
	srcSetFmt.OutCh		= AUD_REN_NUM_OF_CHANNEL_OF_SRC_MIX;
	srcSetFmt.OutFormat	= AUD_REN_BITS_PER_SAMPLE_OF_SRC_MIX;

	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_ALLFMT, allocMod, sizeof(PcmcvtCmdSetAllfmt), &srcSetFmt);

	srcSetOverProtect.over_protect = NOT_USE_OVERFLOW_PROTECTION;
	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OVERPROTECT, allocMod, sizeof(PcmcvtCmdSetOverProtect), &srcSetOverProtect);

	return;
}

static void _AUDIO_SetMixerConfig(UINT32 port, UINT32 fade_length, UINT32 wait_length)
{
	ADEC_MODULE_ID	mixModule = ADEC_MODULE_MIX_0;

	ImcCmdFlushParam	flushParam;
	MixCmdSetExt		mixSetExt;

	_AUDIO_EnableMixerPort(port, AUD_REN_PORT_DISABLE);

	flushParam.num_of_port	= 1;
	flushParam.port_list[0]	= port;

	AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, mixModule, sizeof(ImcCmdFlushParam), &flushParam);

	mixSetExt.port = port;
	mixSetExt.wait_length = wait_length;

	if((port == 0) || (port == 1))
	{
		mixSetExt.initial_mute_length = 0;
	}
	else
	{
		mixSetExt.initial_mute_length = 1024;
	}

	if(fade_length == FADE_LENGTH_0)
	{
		mixSetExt.enable_auto_fade_in	= FADE_IN_OUT_DISABLE;
		mixSetExt.enable_auto_fade_out	= FADE_IN_OUT_DISABLE;
		fade_length = FADE_LENGTH_DEFAULT;
	}
	else
	{
		mixSetExt.enable_auto_fade_in	= FADE_IN_OUT_ENABLE_DEFAULT;
		mixSetExt.enable_auto_fade_out	= FADE_IN_OUT_ENABLE_DEFAULT;
	}

	mixSetExt.fade_length_in_bit = fade_length;
	mixSetExt.init_delay_length = 0;

	AUDIO_IMC_SendCmdParam(MIX_CMD_SET_EXT, mixModule, sizeof(MixCmdSetExt), &mixSetExt);

	_AUDIO_EnableMixerPort(port, AUD_REN_PORT_ENABLE);

	return;
}

static SINT32 _AUDIO_RenDecInfoCb(void *_param, SINT32 _paramLen, void *_cbParam)
{
	ULONG	flags;

	LX_AUD_DEV_T allocDev = LX_AUD_DEV_REN0;

	UINT32 allocMod_SRC = ADEC_MODULE_NOT_DEF;
	UINT32 allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;

	DecAC3EsInfo			ac3EsInfo = {0};
	LipsyncCmdSetFs			setEsLipsyncFs;

	DecEvtESDecInfoParam	*pEsInfoParam = (DecEvtESDecInfoParam*)_param;

	AUD_RENDER_INFO_T		*pRenInfo = (AUD_RENDER_INFO_T*) _cbParam;
	LX_AUD_RENDER_PARAM_T	*pRenParam = &(pRenInfo->renderParam);
	AUD_DEV_INFO_T			*pDevInfo = NULL;

	allocDev = pRenInfo->ui32AllocDev;

	/* Get a render number for allocated decoder. */
	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
		return RET_ERROR;
	}

	// Print For Debug
	if(_paramLen != sizeof(DecEvtESDecInfoParam))
	{
		AUD_KDRV_ERROR("Ren Info : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtESDecInfoParam), _paramLen);
		return RET_ERROR;
	}

	if(pEsInfoParam->sample_rate == LX_AUD_SAMPLING_FREQ_NONE)
	{
		AUD_KDRV_ERROR("Ren Info : Sampling Frequency Error[%d]\n", pEsInfoParam->sample_rate);
		return RET_ERROR;
	}

	if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
	{
		allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
		if(allocMod_SRC == ADEC_MODULE_NOT_DEF )
		{
			AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed \n", ADEC_MOD_TYPE_PCMCVT);
			return RET_ERROR;
		}

		//Set a SRC config if any value is changed.
		if( (pRenParam->samplingFreq		!= pEsInfoParam->sample_rate)		\
		  ||(pRenParam->ui32NumOfChannel != pEsInfoParam->num_of_channel)	\
		  ||(pRenParam->ui32BitPerSample != pEsInfoParam->bit_per_sample)	\
		  ||(pRenParam->endianType		!= pEsInfoParam->input_endian)		\
		  ||(pRenParam->signedType		!= pEsInfoParam->input_signed) )
		{
			_AUDIO_SetSRCFormat(allocMod_SRC, 			  	  pEsInfoParam->sample_rate,	\
								pEsInfoParam->num_of_channel, pEsInfoParam->bit_per_sample,	\
								pEsInfoParam->input_endian,	  pEsInfoParam->input_signed);
		}

		/* Set a SPDIF sampling frequency and DSP SPDIF module to set Fs. */
		// Set a clock setting for main decoder number
		if( (pRenParam->samplingFreq != pEsInfoParam->sample_rate)
		&&(pDevInfo->index == _gMainDecoderIndex) )
		{
			KDRV_AUDIO_UpdateSamplingFreq(pEsInfoParam->sample_rate, LX_AUD_MASTER_SPDIF_PCM);
		}
	}
	else
	{
		//Set a sampling frequency for SPDIF ES config if any value is changed.
		if(pRenParam->samplingFreq != pEsInfoParam->sample_rate)
		{
			allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF )
			{
				AUD_KDRV_ERROR("ModuleType[%d] is not available.\n", ADEC_MOD_TYPE_INPUTCTRL);
				return RET_ERROR;
			}

			/* Set a current value */
			setEsLipsyncFs.Fs = pEsInfoParam->sample_rate;

			/* If lipsync type is ES, set a sampling frequency seeting to compute delay */
			AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_FS, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetFs), &setEsLipsyncFs);

			/* Set a SPDIF sampling frequency and DSP SPDIF module to set Fs. */
			KDRV_AUDIO_UpdateSamplingFreq(pEsInfoParam->sample_rate, LX_AUD_MASTER_SPDIF_ES);

			//Check a E-AC3 Audio Format.
			if(pEsInfoParam->media_type == LX_AUD_CODEC_AC3)
			{
				memcpy(&ac3EsInfo, pEsInfoParam->es_info, sizeof(DecAC3EsInfo));
			}
			else if(pEsInfoParam->media_type == ADEC_MEDIA_DTS_M6)
			{
				DecDTSM6EsInfo	dtsM6EsInfo;

				memcpy(&dtsM6EsInfo, pEsInfoParam->es_info, sizeof(DecDTSM6EsInfo));

				if(dtsM6EsInfo.dts_type == 3)	// DTS_CD : 3
				{
					// Set IEC958 Valid
					SpdifCmdSetIec958valid		setIec958Valid;

					setIec958Valid.value = 0;
					AUDIO_IMC_SendCmdParam(SPDIF_CMD_SET_IEC958VALID, ADEC_MODULE_OUT_SPDIF, sizeof(SpdifCmdSetIec958valid), &setIec958Valid);
				}
			}

			/* Check a AAC/DTS audio codec and sampling frequency */
			if( (pEsInfoParam->media_type  == LX_AUD_CODEC_AAC) 		\
			  &&(pEsInfoParam->sample_rate != LX_AUD_SAMPLING_FREQ_48_KHZ) )
			{
				//Set a event if allocated device and event message is set for RENx.
				if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
				  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_PCM_ONLY) )
				{
					//spin lock for protection
					spin_lock_irqsave(&gAudEventSpinLock, flags);

					//Set a audio GET event type for next event.
					gAudGetEvent[allocDev].allocDev  = allocDev;
					gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_PCM_ONLY;

					//spin unlock for protection
					spin_unlock_irqrestore(&gAudEventSpinLock, flags);
				}

				AUD_KDRV_IMC_NOTI("Ren Info(%d) : AAC SPDIF PCM Fs(%d)\n", allocDev, pEsInfoParam->sample_rate);
			}
			/* Check a E-AC3 audio codec and sampling frequency */
			else if( (pEsInfoParam->media_type  == LX_AUD_CODEC_AC3 && ac3EsInfo.EAC3 == 1)		\
				   &&(pEsInfoParam->sample_rate != LX_AUD_SAMPLING_FREQ_48_KHZ) )
			{
				//Set a event if allocated device and event message is set for RENx.
				if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
				  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_PCM_ONLY) )
				{
					//spin lock for protection
					spin_lock_irqsave(&gAudEventSpinLock, flags);

					//Set a audio GET event type for next event.
					gAudGetEvent[allocDev].allocDev  = allocDev;
					gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_PCM_ONLY;

					//spin unlock for protection
					spin_unlock_irqrestore(&gAudEventSpinLock, flags);
				}

				AUD_KDRV_IMC_NOTI("Ren Info(%d) : E-AC3 SPDIF PCM Fs(%d)\n", allocDev, pEsInfoParam->sample_rate);
			}
		}
	}

	// Restart when sampling frequency changes.
	if(pRenInfo->status == AUD_STATUS_START)
	{
		if(pRenParam->samplingFreq != pEsInfoParam->sample_rate)
		{
			AUDIO_StopRenderer(allocDev);
			AUDIO_FlushRenderer(allocDev);
			AUDIO_StartRenderer(allocDev);
		}
	}

	//Update a render information by decoder information.
	pRenParam->ui32NumOfChannel = pEsInfoParam->num_of_channel;
	pRenParam->ui32BitPerSample = pEsInfoParam->bit_per_sample;
	pRenParam->samplingFreq 	   = pEsInfoParam->sample_rate;
	pRenParam->endianType 	   = pEsInfoParam->input_endian;
	pRenParam->signedType 	   = pEsInfoParam->input_signed;

	//Set a CTRLSRC config if any value is changed.
	if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
	{
		if(pRenParam->input == LX_AUD_INPUT_SIF)
		{
			if(_AUDIO_SetCtrlSrc(pRenInfo) == RET_ERROR)
			{
				AUD_KDRV_ERROR("_AUDIO_SetCtrlSrc failed.\n");
			}
		}
		else if ( (pRenParam->input == LX_AUD_INPUT_HDMI) \
				||(pRenParam->input >= LX_AUD_INPUT_HDMI0 && pRenParam->input <= LX_AUD_INPUT_HDMI3) )
		{
			if(_AUDIO_SetCtrlSrc(pRenInfo) == RET_ERROR)
			{
				AUD_KDRV_ERROR("_AUDIO_SetCtrlSrc failed.\n");
			}
		}
		else if ((pRenParam->input == LX_AUD_INPUT_SYSTEM)
			&& (pEsInfoParam->media_type == ADEC_MEDIA_DTS_M6))
		{
			PcmcvtCmdSetSrcRunningMode	srcRunMode;

			srcRunMode.mode = 1;
			AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_SRC_RUNNING_MODE, allocMod_SRC, sizeof(PcmcvtCmdSetSrcRunningMode), &srcRunMode);
		}
	}

	//Starts a render by decoder information.
	if((pRenInfo->status != AUD_STATUS_START) &&
		(pRenInfo->bStartCalled == TRUE))
	{
		// flush for no-lipsync inputs.
		if ((pRenInfo->renderParam.input == LX_AUD_INPUT_SIF)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_ADC)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI0)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI1)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI2)
			|| (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI3))
		{
			AUDIO_FlushRenderer(allocDev);
		}
		AUDIO_StartRenderer(allocDev);
	}

#ifdef DISABLE_SCART_HW_BYPASS
	/* Check a SCART Reset Status afer boot only. */
	if(_gRenderScartReset == FALSE)
	{
		// Set a SCART Reset Event
		g_suiWaitQForMonitor = LX_AUD_TASK_RESET_SCART;

		//Send event for monitor task
		wake_up_interruptible_all(&Monitor_WaitQ);

		/* Do not SCART Reset after boot. */
		_gRenderScartReset = TRUE;
	}
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

	if((pEsInfoParam->media_type == (ADEC_MEDIA_TYPE)LX_AUD_CODEC_PCM)	\
	 &&(pRenInfo->renderParam.lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM))
	{
		AUD_KDRV_IMC_NOTI("Ren Info(%d) : Fs(%d), Ch(%d), Bs(%d), En(%d), Si(%d)\n", \
						allocDev, pRenParam->samplingFreq, pRenParam->ui32NumOfChannel,	\
						pRenParam->ui32BitPerSample, pRenParam->endianType, pRenParam->signedType);
	}
	else
	{
		AUD_KDRV_IMC_NOTI("Ren Info(%d) : Fs(%d), Ch(%d), Bs(%d)\n", \
						allocDev, pRenParam->samplingFreq, pRenParam->ui32NumOfChannel, pRenParam->ui32BitPerSample);
	}
	return RET_OK;
}

static SINT32 _AUDIO_RenBasetimeCb(void *_param, SINT32 _paramLen, void *_cbParam)
{
	UINT32					i;
	SINT32					decIndex = 0;
	SINT32					retVal = RET_ERROR;

	LipsyncEvtNopcrBaseline	*pBasetime = (LipsyncEvtNopcrBaseline*)_param;
	AUD_RENDER_INFO_T		*pRenInfo = (AUD_RENDER_INFO_T*) _cbParam;
	LX_AUD_DEV_T			allocDev = LX_AUD_DEV_REN0;
	LX_AUD_DEV_T			ConnectedDev = LX_AUD_DEV_DEC0;
	AUD_DEV_INFO_T			*pDevInfo = NULL;

	LX_AUD_RENDER_PARAM_BASETIME_T	basetimeParam;
	LX_AUD_DEV_INDEX_CTX_T 	indexCtx;

	allocDev = pRenInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(LipsyncEvtNopcrBaseline))
	{
		AUD_KDRV_ERROR("Ren Basetime : Param Length Error : [Expected:%d][Input:%d]\n", sizeof(LipsyncEvtNopcrBaseline), _paramLen);
		return RET_ERROR;
	}

	for(i = 0; i < AUD_RENDER_CONNECT_NUM; i++)
	{
		if(pRenInfo->renderConnect[i].devType == LX_AUD_DEV_TYPE_DEC)
		{
			ConnectedDev = pRenInfo->renderConnect[i].connectDev;
		}
	}

	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_DEBUG("pDevInfo[%d] is NULL.\n", allocDev);
		return RET_ERROR;
	}

	if(pDevInfo->index == DEFAULT_DEV_INDEX)
	{
		decIndex = GET_DEC_INDEX(ConnectedDev);
		if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
		{
			AUD_KDRV_ERROR("decIndex[%d] is not available. \n", decIndex);
			return RET_ERROR;
		}
	}
	else
	{
		decIndex = pDevInfo->index;
	}

	BASE_AVLIPSYNC_LOCK();

	if(pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_MASTER)
	{
		if(decIndex == 0)
		{
			basetimeParam.ui64ClockBaseTime  = (UINT64)(pBasetime->baseCT);
			basetimeParam.ui64StreamBaseTime = (UINT64)(pBasetime->baseST);

			(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_CBT, (UINT32)(basetimeParam.ui64ClockBaseTime  & TIMESTAMP_MASK_NO_PCR));
			(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_SBT, (UINT32)(basetimeParam.ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR));
		}
		else
		{
			basetimeParam.ui64ClockBaseTime  = (UINT64)(pBasetime->baseCT);
			basetimeParam.ui64StreamBaseTime = (UINT64)(pBasetime->baseST);

			(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_CBT, (UINT32)(basetimeParam.ui64ClockBaseTime  & TIMESTAMP_MASK_NO_PCR));
			(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_SBT, (UINT32)(basetimeParam.ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR));
		}

		// set basetime for es.
		indexCtx.ui32Index = pDevInfo->index;
		indexCtx.devType = LX_AUD_DEV_TYPE_REN;
		indexCtx.devOutType = LX_AUD_DEV_OUT_ES;
		retVal = AUDIO_GetDevIndexCtx(&indexCtx);
		if(retVal == RET_OK)
			AUDIO_SetBasetime(indexCtx.dev, &basetimeParam);
		else
			AUD_KDRV_DEBUG("ES RenIndex(%d) doesn't exist. \n", pDevInfo->index);
	}
	else //if(pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_NO_PCR)
	{
		if(decIndex == 0)
		{
			if((g_pVDEC_LipsyncReg->vdec_dec0_cbt == 0xFFFFFFFF)
			 ||(g_pVDEC_LipsyncReg->vdec_dec0_sbt == 0xFFFFFFFF))
			{
				basetimeParam.ui64ClockBaseTime  = (UINT64)(pBasetime->baseCT);
				basetimeParam.ui64StreamBaseTime = (UINT64)(pBasetime->baseST);
			}
			else
			{
				basetimeParam.ui64ClockBaseTime  = (UINT64)(g_pVDEC_LipsyncReg->vdec_dec0_cbt);
				basetimeParam.ui64StreamBaseTime = (UINT64)(g_pVDEC_LipsyncReg->vdec_dec0_sbt);
			}

			(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_CBT, (UINT32)(basetimeParam.ui64ClockBaseTime  & TIMESTAMP_MASK_NO_PCR));
			(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_SBT, (UINT32)(basetimeParam.ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR));
		}
		else
		{
			if((g_pVDEC_LipsyncReg->vdec_dec1_cbt == 0xFFFFFFFF)
			 ||(g_pVDEC_LipsyncReg->vdec_dec1_sbt == 0xFFFFFFFF))
			{
				basetimeParam.ui64ClockBaseTime  = (UINT64)(pBasetime->baseCT);
				basetimeParam.ui64StreamBaseTime = (UINT64)(pBasetime->baseST);
			}
			else
			{
				basetimeParam.ui64ClockBaseTime  = (UINT64)(g_pVDEC_LipsyncReg->vdec_dec1_cbt);
				basetimeParam.ui64StreamBaseTime = (UINT64)(g_pVDEC_LipsyncReg->vdec_dec1_sbt);
			}

			(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_CBT, (UINT32)(basetimeParam.ui64ClockBaseTime  & TIMESTAMP_MASK_NO_PCR));
			(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_SBT, (UINT32)(basetimeParam.ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR));
		}
	}

	AUDIO_SetBasetime(allocDev, &basetimeParam);

	BASE_AVLIPSYNC_UNLOCK();

	AUD_KDRV_NOTI("Ren Basetime(%d) : Bst(0x%08X), Bct(0x%08X)\n", allocDev, pBasetime->baseST, pBasetime->baseCT);
	return RET_OK;
}

/**
 * Present End Callback function.
 * @param 	_param			[out] parameters.
 * @param 	_paramLen		[out] length of parameters.
 * @param 	_cbParam		[in] callback parameters.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_SetRendererParam
 */
static SINT32 _AUDIO_RenPresentEndCb(void *_param, SINT32 _paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_REN0;
	SINT32	renIndex = 0;
	ULONG	flags;

	LipsyncEvtPresentEnd	*pPresentEnd = (LipsyncEvtPresentEnd *)_param;
	AUD_RENDER_INFO_T		*pRenInfo = (AUD_RENDER_INFO_T *)_cbParam;

	allocDev = pRenInfo->ui32AllocDev;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	//Set a Present End Flag.
	pRenInfo->renderStatus.bPresentEnd = TRUE;

	//Set a event if allocated device and event message is set for RENx.
	if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
	  &&(gAudSetEvent[allocDev].eventMsg  & LX_AUD_EVENT_PRESENT_END) )
	{
		//spin lock for protection
		spin_lock_irqsave(&gAudEventSpinLock, flags);

		//Set a audio GET event type for next event.
		gAudGetEvent[allocDev].allocDev  = allocDev;
		gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_PRESENT_END;

		//spin unlock for protection
		spin_unlock_irqrestore(&gAudEventSpinLock, flags);
	}

	AUD_KDRV_IMC_NOTI("Ren(%d) PresentEnd : remain %d ms\n", allocDev, pPresentEnd->remain_ms);
	return RET_OK;
}

/**
 * Tone Detect Callback function.
 * @param 	_param			[out] parameters.
 * @param 	_paramLen		[out] length of parameters.
 * @param 	_cbParam		[in] callback parameters.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_SetRendererParam
 */
static SINT32 _AUDIO_RenToneDetectCb(void *_param, SINT32 _paramLen, void *_cbParam)
{
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_REN0;
	SINT32	renIndex = 0;
	ULONG	flags;

	SrcEvtDetectTone		*pToneDetect = (SrcEvtDetectTone *)_param;
	AUD_RENDER_INFO_T		*pRenInfo = (AUD_RENDER_INFO_T *)_cbParam;

	allocDev = pRenInfo->ui32AllocDev;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	//Set a event if allocated device and event message is set for RENx.
	if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
	  &&(gAudSetEvent[allocDev].eventMsg & (LX_AUD_EVENT_1KHZ_TONE_ON | LX_AUD_EVENT_1KHZ_TONE_OFF)) )
	{
		//spin lock for protection
		spin_lock_irqsave(&gAudEventSpinLock, flags);

		//Set a audio GET event type for next event.
		gAudGetEvent[allocDev].allocDev = allocDev;

		if(pToneDetect->istone == 1)
			gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_1KHZ_TONE_ON;
		else
			gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_1KHZ_TONE_OFF;

		//spin unlock for protection
		spin_unlock_irqrestore(&gAudEventSpinLock, flags);
	}

	AUD_KDRV_IMC_NOTI("Ren(%d) isTone : %x.\n", allocDev, pToneDetect->istone);
	return RET_OK;
}

/**
 * callback function to update rendered status.
 * @see		_AUDIO_RegisterRenderEvent().
 */
static SINT32 _AUDIO_RenStatusCb(void *_param, int _paramLen, void *_cbParam)
{
	LipsyncEvtPresentIndex	*pRenIndexParam;
	AUD_RENDER_INFO_T 	*pRenInfo = NULL;

	pRenIndexParam = (LipsyncEvtPresentIndex *)_param;
	pRenInfo   = (AUD_RENDER_INFO_T *)_cbParam;

	if(_paramLen != sizeof(DecEvtDecIndexParam))
	{
		AUD_KDRV_ERROR("Dec Index Param : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtDecIndexParam), _paramLen);
		return RET_ERROR;
	}

	//Copy a additional decoded info.
	pRenInfo->renderStatus.ui32RenderedIndex = pRenIndexParam->index;

#ifdef AUD_RENDER_TIME_STAMP
	pRenInfo->renderStatus.ui64Timestamp 	 = pRenInfo->ui64Timestamp[pRenIndexParam->index % MPB_AUI_INDEX_COUNT];
#endif

	AUD_KDRV_IMC_NOTI("Ren Index : Idx(%5d), Ts(%10d)\n", pRenIndexParam->index, pRenIndexParam->timestamp);
	return RET_OK;
}

/**
 * callback function to notificate DTS SPDIF Type inforamtion.
 * @see		_AUDIO_RegisterDecoderEvent().
 */
static SINT32 _AUDIO_SpdifTypeCb(void *_param, int _paramLen, void *_cbParam)
{
	ULONG	flags;
	LX_AUD_DEV_T	allocDev = LX_AUD_DEV_DEC0;

	DecEvtReqSpdifType			*pSpdifType = NULL;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;

	pSpdifType = (DecEvtReqSpdifType *)_param;
	pRenInfo	= (AUD_RENDER_INFO_T *)_cbParam;
	allocDev	= pRenInfo->ui32AllocDev;

	// Print For Debug
	if(_paramLen != sizeof(DecEvtReqSpdifType))
	{
		AUD_KDRV_ERROR("TP Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtReqSpdifType), _paramLen);
		return RET_ERROR;
	}

	/* Check a DTS audio codec and SPDIF PCM Event. */
	if( (pSpdifType->media_type == LX_AUD_CODEC_DTS_M6)	\
	  &&(pSpdifType->spdif_type == ADEC_SPDIF_TYPE_PCM) )
	{
		//Set a event if allocated device and event message is set for DECx.
		if( (gAudSetEvent[allocDev].allocDev != LX_AUD_DEV_NONE)		\
		  &&(gAudSetEvent[allocDev].eventMsg & LX_AUD_EVENT_DEC_PCM_ONLY) )
		{
			//spin lock for protection
			spin_lock_irqsave(&gAudEventSpinLock, flags);

			//Set a audio GET event type for next event.
			gAudGetEvent[allocDev].allocDev  = allocDev;
			gAudGetEvent[allocDev].eventMsg |= LX_AUD_EVENT_DEC_PCM_ONLY;

			//spin unlock for protection
			spin_unlock_irqrestore(&gAudEventSpinLock, flags);

			AUD_KDRV_IMC_NOTI("Dec Info(%d) : DTS SPDIF PCM(%d)\n", allocDev, pSpdifType->spdif_type);
		}
	}

	AUD_KDRV_IMC_NOTI("Dev(%d) : mediaType(%d), spdifType(%d)\n", allocDev, pSpdifType->media_type, pSpdifType->spdif_type);
	return RET_OK;
}

/**
 * callback function to update delay.
 * @see		_AUDIO_RegisterRenderEvent().
 */
static SINT32 _AUDIO_RenDelayCb(void *_param, int _paramLen, void *_cbParam)
{
	InputctrlEvtSystemDelay	*pSystemDelay;
	AUD_RENDER_INFO_T 	*pRenInfo = NULL;

	pSystemDelay = (InputctrlEvtSystemDelay*)_param;
	pRenInfo = (AUD_RENDER_INFO_T*)_cbParam;

	// Print For Debug
	if(_paramLen != sizeof(InputctrlEvtSystemDelay))
	{
		AUD_KDRV_ERROR("REN Error : Param Length Error[Expected:%d][Input:%d]\n", sizeof(InputctrlEvtSystemDelay), _paramLen);
		return RET_ERROR;
	}

	if(pRenInfo != NULL)
	{
		AUD_KDRV_IMC_NOTI("totalDelay(%d), inputCtrlDelay(%d) \n ",
			pSystemDelay->total_system_delay,	pSystemDelay->inputctrl_delay);

		pRenInfo->ui32Delay = pSystemDelay->total_system_delay;
	}

	return RET_OK;
}

#ifdef USE_LIPSYNC_FREE
#define NUM_OF_LIPSYNC_FREE		10
#define TIME_OF_LIPSYNC_FREE	1000
#define ADDITIONAL_DELAY_100MS	100

/**
 * Underflow Callback function.
 * @param 	_param			[out] parameters.
 * @param 	_paramLen		[out] length of parameters.
 * @param 	_cbParam		[in] callback parameters.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_SetRendererParam
 */
static SINT32 _AUDIO_RenMixerUnderflowCb(void *_param, SINT32 _paramLen, void *_cbParam)
{
	LX_AUD_DEV_T			allocDev = LX_AUD_DEV_NONE;
	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	SINT32					renIndex = 0;
	UINT32					elapsedTime = 0;

	MixerEvtUnderflow		*pUnderflow = (MixerEvtUnderflow *)_param;

	pRenInfo = (AUD_RENDER_INFO_T*)_cbParam;
	allocDev = pRenInfo->ui32AllocDev;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF )
	{
		if(pUnderflow->portNum == pRenInfo->ui32MixerPort)
		{
			//Get a current tick time for first underflow
			if(pRenInfo->ui32CountUnderflow == 0)
			{
				pRenInfo->ui64TickUnderflow = jiffies;
			}

			pRenInfo->ui32CountUnderflow++;

			//Get a elapsed mili-second time
			elapsedTime = (UINT32)jiffies_to_msecs(jiffies - pRenInfo->ui64TickUnderflow);

			if(pRenInfo->ui32CountUnderflow > NUM_OF_LIPSYNC_FREE && elapsedTime > TIME_OF_LIPSYNC_FREE)
			{
				if(_gRenderInfo[renIndex].bUnderflowCalled == FALSE)
				{
					AUD_MOD_INFO_T				*pModInfo = NULL;
					LipsyncCmdSetBound 			setLipsyncBound;
					InputctrlCmdSetDelay		setInputCtrlDelay;

					_gRenderInfo[renIndex].bUnderflowCalled = TRUE;

					// Set Lipsync for PCM
					setLipsyncBound.offset = pRenInfo->renderLipsync.ui32Offset + pRenInfo->ui32InputCtrlDelay + ADDITIONAL_DELAY_100MS;
					setLipsyncBound.lbound = pRenInfo->renderLipsync.ui32Lbound + ADDITIONAL_DELAY_100MS;
					setLipsyncBound.ubound = pRenInfo->renderLipsync.ui32Ubound + ADDITIONAL_DELAY_100MS;
					setLipsyncBound.freerunlbound = pRenInfo->renderLipsync.ui32Freerunlbound;
					setLipsyncBound.freerunubound = pRenInfo->renderLipsync.ui32Freerunubound;

					AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BOUND, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBound), &setLipsyncBound);

					setInputCtrlDelay.delay = 0;
					AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);

					// Set Lipsync for ES
					allocMod_INPUTCTRL = allocMod_INPUTCTRL + MAX_DEV_INDEX;	//for ES
					pModInfo = AUDIO_GetModuleInfo(allocMod_INPUTCTRL);

					if(pModInfo != NULL)
					{
						if(pModInfo->status == AUD_STATUS_START)
						{
							setLipsyncBound.offset = pRenInfo->renderLipsync.ui32Offset + pRenInfo->ui32InputCtrlDelay + ADDITIONAL_DELAY_100MS;
							setLipsyncBound.lbound = pRenInfo->renderLipsync.ui32Lbound + ADDITIONAL_DELAY_100MS;
							setLipsyncBound.ubound = pRenInfo->renderLipsync.ui32Ubound + ADDITIONAL_DELAY_100MS;
							setLipsyncBound.freerunlbound = pRenInfo->renderLipsync.ui32Freerunlbound;
							setLipsyncBound.freerunubound = pRenInfo->renderLipsync.ui32Freerunubound;

							AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BOUND, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBound), &setLipsyncBound);

							setInputCtrlDelay.delay = 0;
							AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);
						}
					}
				}
			}
		}
	}

	AUD_KDRV_IMC_NOTI("Mixer Port(%d) Underflow : temp %x.\n", pUnderflow->portNum, pUnderflow->tmp);

	return RET_OK;
}
#endif

/**
 * Alloc Renderer Module.
 * @param 	modID			[in] Module ID.
 * @return 	if is used - TRUE, else - FALSE.
 * @see		AUDIO_ConnectRenderer
 */
static SINT32 _AUDIO_SetCtrlSrc(AUD_RENDER_INFO_T *pRenInfo)
{
	UINT32	i;

	ADEC_MODULE_ID					allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					allocMod_CTRL_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID					allocMod_INPUT = ADEC_MODULE_NOT_DEF;
	LX_AUD_DEV_T					connectDev = ADEC_MODULE_NOT_DEF;

	LX_AUD_RENDER_PARAM_T			*pRenParam = NULL;
	LX_AUD_SAMPLING_FREQ_T			setSpeakerFreq = LX_AUD_SAMPLING_FREQ_48_KHZ;

	CtrlsrcCmdSetSrcModule			setSrcModule;
	CtrlsrcCmdSetIntInfo			setIntInfo;
	CtrlsrcCmdSetSrcrunningmode		setRuningMode;

	if(pRenInfo==NULL)
	{
		AUD_KDRV_ERROR("pRenInfo is NULL!!!\n");
		return RET_ERROR;
	}

	pRenParam = &(pRenInfo->renderParam);

	allocMod_SRC = AUDIO_FindModuleType(pRenInfo->ui32AllocDev, ADEC_MOD_TYPE_PCMCVT);
	if (allocMod_SRC == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed!!!\n", ADEC_MOD_TYPE_PCMCVT);
		return RET_ERROR;
	}

	allocMod_CTRL_SRC = AUDIO_FindModuleType(pRenInfo->ui32AllocDev, ADEC_MOD_TYPE_CTRLSRC);
	if(allocMod_CTRL_SRC == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed!!!\n", ADEC_MOD_TYPE_CTRLSRC);
		return RET_ERROR;
	}

	setSrcModule.src_module_id = allocMod_SRC;
	AUDIO_IMC_SendCmdParam(CTRLSRC_CMD_SET_SRC_MODULE, allocMod_CTRL_SRC, sizeof(CtrlsrcCmdSetSrcModule), &setSrcModule);

	setSpeakerFreq = MIXED_IP_AUDIO_GetSpeakerFreq();

	if(pRenParam->input == LX_AUD_INPUT_SIF)
	{
		setIntInfo.RefInt	= AUD_INT_AADIN;
		setIntInfo.RefIntFs	= LX_AUD_SAMPLING_FREQ_48_KHZ;
		setIntInfo.TarInt	= AUD_INT_PCM;
		setIntInfo.TarIntFs = setSpeakerFreq;
	}
	else if ( (pRenParam->input == LX_AUD_INPUT_HDMI)	\
			||(pRenParam->input >= LX_AUD_INPUT_HDMI0 && pRenParam->input <= LX_AUD_INPUT_HDMI3) )
	{
		for (i = 0; i < pRenInfo->ui32ConnectNum; i++)
		{
			if(pRenInfo->renderConnect[i].devType == LX_AUD_DEV_TYPE_DEC)
			{
				connectDev = pRenInfo->renderConnect[i].connectDev;
			}
		}

		allocMod_INPUT = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_SOURCE);
		if(allocMod_INPUT != ADEC_MODULE_NOT_DEF)
		{
			if(allocMod_INPUT == ADEC_MODULE_HDMI_IN_1)
				setIntInfo.RefInt	= AUD_INT_HDMIIN1;
			else
				setIntInfo.RefInt	= AUD_INT_HDMIIN0;
		}
		else
		{
			setIntInfo.RefInt	= AUD_INT_HDMIIN0;
		}
		setIntInfo.RefIntFs	= pRenParam->samplingFreq;
		setIntInfo.TarInt	= AUD_INT_PCM;
		setIntInfo.TarIntFs = setSpeakerFreq;
	}

	AUDIO_IMC_SendCmdParam(CTRLSRC_CMD_SET_INTINFO, allocMod_CTRL_SRC, sizeof(CtrlsrcCmdSetIntInfo), &setIntInfo);

	if(pRenParam->bSetMainRen)
	{
		setRuningMode.src_running_mode = 0;
	}
	else
	{
		setRuningMode.src_running_mode = 1;
	}

	AUDIO_IMC_SendCmdParam(CTRLSRC_CMD_SET_SRCRUNNINGMODE, allocMod_CTRL_SRC, sizeof(CtrlsrcCmdSetSrcrunningmode), &setRuningMode);

	return RET_OK;
}

SINT32 AUDIO_FeedRen(UINT32 allocDev, LX_AUD_RENDER_FEED_T *pRenFeed)
{
	SINT32					retVal = 0;
	UINT32					ui32FreeAuCount;
	UINT64					ui64TS90kHzTick;
	SINT32					renIndex = 0;

	ADEC_BUF_T				*pWriterStruct = NULL;
	ADEC_AU_INFO_T			info = {0, };

	LX_AUD_RENDER_FEED_T	renFeedData;
	AUD_RENDER_INFO_T	*pRenInfo = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_BUF_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];

	pWriterStruct = _AUDIO_GetRendererWriteStructure(allocDev);
	if(pWriterStruct == NULL)
	{
		AUD_REN_BUF_UNLOCK();
		AUD_KDRV_ERROR("Feed : pWriterStruct is NULL !!!\n");
		return RET_ERROR;
	}

	//Check a repeat number and buffer status to initialize audio buffer
	if(pRenInfo->bBufferRepeat == TRUE)
	{
		//buffer init : buffer is not flushed if repeat is set previously.
		pWriterStruct->init(pWriterStruct);

		//Clear a buffer repeat variable.
		pRenInfo->bBufferRepeat = FALSE;
	}

	memcpy(&renFeedData, pRenFeed, sizeof(LX_AUD_RENDER_FEED_T));

	if(renFeedData.ui32BufferSize > 0 || renFeedData.statusBuffer == LX_AUD_BUFFER_END)
	{
		AUD_KDRV_PRINT("Feed => Free:%d, Used:%d\n", pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_used_size(pWriterStruct));

		/* Compute free au count */
		ui32FreeAuCount = pWriterStruct->get_max_au(pWriterStruct) - pWriterStruct->get_au_cnt(pWriterStruct);

		/* Check buffer overflow and AUI overflow */
		if( (pWriterStruct->get_free_size(pWriterStruct) >=  renFeedData.ui32BufferSize) &&(ui32FreeAuCount > 1) )
		{
			if (renFeedData.ui32BufferSize > 0)
			{
				/* Check a Google TVP Path to use 90Khz gstc clock value. */
				if(renFeedData.bUseGstcClock == FALSE)
				{
					ui64TS90kHzTick = renFeedData.ui64TimeStamp;
					if(ui64TS90kHzTick != 0xFFFFFFFFFFFFFFFFULL)
					{
						ui64TS90kHzTick *= 9;
						do_div(ui64TS90kHzTick, 100000);	// = Xns * 90 000 / 1 000 000 000
					}
				}
				else
				{
					ui64TS90kHzTick = renFeedData.ui64TimeStamp;
				}

				//Set NEW AUI info.
				info.size			= (UINT64)renFeedData.ui32BufferSize;
				info.timestamp		= (UINT32)(ui64TS90kHzTick) & TIMESTAMP_MASK;	//28 bit
				info.first_module_gstc = AUDIO_ReadReg(LX_AUD_REG_GSTC_LOW);
				info.gstc			= 0;
				info.index			= pRenInfo->renderStatus.ui32FeededIndex;
				info.error			= 0;
				info.discontinuity	= 0;
				info.lastAu			= 0;

				AUD_KDRV_PRINT("pts[%10d], dts[%10d], idx[%5d], size[%5llu]\n", info.timestamp, info.gstc, info.index, info.size);

				//Write AUI info.
				retVal = pWriterStruct->create_new_au(&info, pWriterStruct);
				if(retVal < RET_OK)
				{
					AUD_REN_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => create_new_au(ret = %d, cnt = %d, free = %d)!!!\n", \
								retVal, pWriterStruct->get_au_cnt(pWriterStruct), pWriterStruct->get_free_size(pWriterStruct));
					return RET_ERROR;
				}

			    //Write Mix data
			    retVal = pWriterStruct->write_data(renFeedData.pBuffer, renFeedData.ui32BufferSize, pWriterStruct);
				if(retVal < RET_OK)
				{
					AUD_REN_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => write_data(ret = %d, cnt = %d, free = %d)!!!\n", \
								retVal, pWriterStruct->get_au_cnt(pWriterStruct), pWriterStruct->get_free_size(pWriterStruct));
					return RET_ERROR;
				}
			}

			//Check buffer status
			if(renFeedData.statusBuffer == LX_AUD_BUFFER_END)
			{
				//Set last buffer
				retVal = BufHelper_SetLast(pWriterStruct);
				if(retVal < RET_OK)
				{
					AUD_REN_BUF_UNLOCK();
					AUD_KDRV_ERROR("Feed => BufHelper_SetLast(ret = %d, free = %d, cnt = %d)!!!\n", \
								retVal, pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
					return RET_ERROR;
				}

				//Set a repeat function and variable for buffer init.
				if(renFeedData.ui32RepeatNumber > 1)
				{
					pRenInfo->bBufferRepeat = TRUE;

					(void)pWriterStruct->set_repeat(renFeedData.ui32RepeatNumber - 1, pWriterStruct);
				}

				AUD_KDRV_DEBUG("Feed => BufHelper_SetLast(ret = %d, free = %d, cnt = %d)!!!\n", \
							retVal, pWriterStruct->get_free_size(pWriterStruct), pWriterStruct->get_au_cnt(pWriterStruct));
			}

#ifdef AUD_RENDER_TIME_STAMP
			pRenInfo->ui64Timestamp[pRenInfo->renderStatus.ui32FeededIndex % MPB_AUI_INDEX_COUNT] = renFeedData.ui64TimeStamp;
#endif
			pRenInfo->renderStatus.ui32FeededIndex++;

			pRenInfo->renderStatus.ui32FeededCount++;
		}
		else
		{
			AUD_KDRV_DEBUG("Feed => free = %d, count = %d!!!\n", \
						pWriterStruct->get_free_size(pWriterStruct), \
						pWriterStruct->get_au_cnt(pWriterStruct));

			retVal = RET_ERROR;
		}
	}
	else
	{
		AUD_KDRV_ERROR("Feed => BufferSize = %u\n", renFeedData.ui32BufferSize);
		retVal = RET_ERROR;
	}

	AUD_REN_BUF_UNLOCK();

	AUD_KDRV_PRINT("bufSize=%u, bufStatus=%d, TS=%llu\n", renFeedData.ui32BufferSize, renFeedData.statusBuffer, renFeedData.ui64TimeStamp);

	return retVal;
}

/**
 * Start for renderer.
 * @see
*/
SINT32 AUDIO_StartRenderer(UINT32 allocDev)
{
	SINT32			retVal = RET_OK;
	SINT32			renIndex = 0;

	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	LX_AUD_RENDER_PARAM_T	*pRenParam = NULL;

	ADEC_MODULE_ID			allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_SOLA = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_CTRLSRC = ADEC_MODULE_NOT_DEF;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &(_gRenderInfo[renIndex]);
	pRenParam = &(pRenInfo->renderParam);

	pRenInfo->bStartCalled = TRUE;

	if(pRenParam->samplingFreq == LX_AUD_SAMPLING_FREQ_NONE)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_PRINT("Sampling Frequency is not available.[%d]\n", allocDev );
		return RET_OK;
	}

	// Start Input Control
	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
	{
		LipsyncCmdSetIdletime	idleTime;

		// fix 1~2sec mute from pause to play in ATV timeshift mode.
		if ((pRenInfo->status == AUD_STATUS_STOP)
			&& ((pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_PCR_M)
			|| (pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_PCR_A)))
		{
			idleTime.msec = 300;
		}
		else
		{
			idleTime.msec = 60;
		}
		AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_IDLETIME, allocMod_INPUTCTRL, \
			sizeof(LipsyncCmdSetIdletime), &idleTime);

		if(pRenInfo->trickMode == LX_AUD_TRICK_ONEFRAME_DECODE)
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocMod_INPUTCTRL);
		}
		else
		{
			if ((pRenInfo->status == AUD_STATUS_NONE)
				|| (pRenInfo->status == AUD_STATUS_FLUSH))
			{
				AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocMod_INPUTCTRL);
			}
			AUDIO_IMC_SendCmd(ADEC_CMD_START, allocMod_INPUTCTRL);
		}
	}

	// Start SOLA
	if (TRUE == pRenParam->bSetTrickMode)
	{
		allocMod_SOLA = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOLA);
		if(allocMod_SOLA != ADEC_MODULE_NOT_DEF)
		{
			if ((pRenInfo->status == AUD_STATUS_NONE)
				|| (pRenInfo->status == AUD_STATUS_FLUSH))
			{
				AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocMod_SOLA);
			}
			AUDIO_IMC_SendCmd(ADEC_CMD_START, allocMod_SOLA);
		}
	}

	// Start SRC
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
	{
		if ((pRenInfo->status == AUD_STATUS_NONE)
			|| (pRenInfo->status == AUD_STATUS_FLUSH))
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocMod_SRC);
		}
		AUDIO_IMC_SendCmd(ADEC_CMD_START, allocMod_SRC);
	}

	// set RTS mode
	if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
	{
		LX_AUD_RENDER_RTS_PARAM_T	*pRTSParam;
		RtsCmdSetThreashold			rtsThreshold;
		RtsCmdSetOutFreq			rtsOutFreq;
		RtsCmdSetPortMultiplier		rtsPortMultiplier;
		RtsCmdSetOperationMode		rtsOperationMode;
		RtsCmdSetSkippingConfig		rtsSkippingConfig;

		pRTSParam = &_gRenderInfo[renIndex].rtsParam;

		// set RTS threshold - continuous mode
		rtsThreshold.upper_th					= pRTSParam->upperThreshold;
		rtsThreshold.lower_th					= pRTSParam->lowerThreshold;
		rtsThreshold.recover_upper				= pRTSParam->recoverUpper;
		rtsThreshold.recover_lower				= pRTSParam->recoverLower;
		AUDIO_IMC_SendCmdParam(RTS_CMD_SET_THRESHOLD, ADEC_MODULE_RTS, sizeof(RtsCmdSetThreashold), &rtsThreshold);

		// set RTS output frequency
		rtsOutFreq.fast_freq					= pRTSParam->fastFreq;
		rtsOutFreq.normal_freq					= pRTSParam->normalFreq;
		rtsOutFreq.slow_freq					= pRTSParam->slowFreq;
		AUDIO_IMC_SendCmdParam(RTS_CMD_SET_OUT_FREQ, ADEC_MODULE_RTS, sizeof(RtsCmdSetOutFreq), &rtsOutFreq);

		// set port multplier - only input port 0 (the input port of SRC module)
		rtsPortMultiplier.ref_port				= MOD_REF_PORT(0);
		rtsPortMultiplier.numerator				= 384000;			// 48000Hz * 2ch * 32bits
		rtsPortMultiplier.denominator			= (UINT32)(pRenParam->samplingFreq * pRenParam->ui32NumOfChannel * (pRenParam->ui32BitPerSample >> 3));
		AUDIO_IMC_SendCmdParam(RTS_CMD_SET_PORT_MULTPLIER, ADEC_MODULE_RTS, sizeof(RtsCmdSetPortMultiplier), &rtsPortMultiplier);

		// Set RTS threshold - skipping mode
		rtsSkippingConfig.skipping_threshold	= pRTSParam->skippingThreshold;
		rtsSkippingConfig.skipping_recovery		= pRTSParam->recoverSkipping;
		//rtsSkippingConfig.mixer_module_id		= ADEC_MODULE_MIX_0;
		//rtsSkippingConfig.mixer_port			= MOD_IN_PORT(0);
		AUDIO_IMC_SendCmdParam(RTS_CMD_SET_SKIPPING_CONFIG, ADEC_MODULE_RTS, sizeof(RtsCmdSetSkippingConfig), &rtsSkippingConfig);

		// Set RTS operation mode
		rtsOperationMode.op_mode				= pRTSParam->opMode;
		AUDIO_IMC_SendCmdParam(RTS_CMD_SET_OPERATION_MODE, ADEC_MODULE_RTS, sizeof(RtsCmdSetOperationMode), &rtsOperationMode);

		// INIT & START
		//AUDIO_IMC_SendCmd(ADEC_CMD_INIT, ADEC_MODULE_RTS);
		AUDIO_IMC_SendCmd(ADEC_CMD_START, ADEC_MODULE_RTS);
	}

	// set nodelay mode
	if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
	{
		UINT32		allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
		LipsyncCmdSetNodelay	setNodelayParam;

		allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
		{
			setNodelayParam.onoff			= pRenInfo->renderNodelay.ui32OnOff;
			setNodelayParam.upperthreshold	= pRenInfo->renderNodelay.ui32UThreshold;
			setNodelayParam.lowerthreshold	= pRenInfo->renderNodelay.ui32LThreshold;
			setNodelayParam.prebyteper1sec	= (UINT32)(pRenParam->samplingFreq * pRenParam->ui32NumOfChannel * (pRenParam->ui32BitPerSample >> 3));
			setNodelayParam.posbyteper1sec	= 384000;

			AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_NODELAY, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetNodelay), &setNodelayParam);
		}
	}

	// Start CTRLSRC
	allocMod_CTRLSRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_CTRLSRC);
	if(allocMod_CTRLSRC != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_START, allocMod_CTRLSRC);
	}

	pRenInfo->status = AUD_STATUS_START;

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("renIndex : %d \n", renIndex);

	return retVal;
}


SINT32 AUDIO_StopRenderer(UINT32 allocDev)
{
	UINT32			i;
	SINT32			retVal = RET_OK;
	SINT32			renIndex = 0;
	SINT32			decIndex = 0;

	LX_AUD_DEV_T			ConnectedDev = LX_AUD_DEV_NONE;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	AUD_DEV_INFO_T			*pDevInfo = NULL;
	ADEC_MODULE_ID			allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_SOLA = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_CTRLSRC = ADEC_MODULE_NOT_DEF;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];

	// Stop SRC
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocMod_SRC);
	}

	// Stop SOLA
	if (TRUE == pRenInfo->renderParam.bSetTrickMode)
	{
		allocMod_SOLA = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOLA);
		if(allocMod_SOLA != ADEC_MODULE_NOT_DEF)
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocMod_SOLA);
		}
	}

	// Stop Input Control
	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocMod_INPUTCTRL);
	}

	if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, ADEC_MODULE_RTS);
	}

	// Stop CTRLSRC
	allocMod_CTRLSRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_CTRLSRC);
	if(allocMod_CTRLSRC != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocMod_CTRLSRC);
	}

	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_DEBUG("pDevInfo[%d] is NULL.\n", allocDev);
		return RET_ERROR;
	}

	if(pDevInfo->index == DEFAULT_DEV_INDEX)
	{
		for(i = 0; i < AUD_RENDER_CONNECT_NUM; i++)
		{
			if(pRenInfo->renderConnect[i].devType == LX_AUD_DEV_TYPE_DEC)
			{
				ConnectedDev = pRenInfo->renderConnect[i].connectDev;
			}
		}

		if(ConnectedDev == LX_AUD_DEV_NONE)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_PRINT("No Connected Decoder Device. \n");
			return RET_OK;
		}

		decIndex = GET_DEC_INDEX(ConnectedDev);
		if((decIndex < 0) || (decIndex >= DEV_DEC_NUM))
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("decIndex[%d] is not available. \n", decIndex);
			return RET_ERROR;
		}
	}
	else
	{
		decIndex = pDevInfo->index;
	}

	BASE_AVLIPSYNC_LOCK();

	//Clear & save a av lip sync register value.
	if(decIndex == 0)
	{
		(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_CBT, 0xFFFFFFFF);
		(void)AUDIO_WriteReg(LX_AUD_REG_DEC0_SBT, 0xFFFFFFFF);
	}
	else
	{
		(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_CBT, 0xFFFFFFFF);
		(void)AUDIO_WriteReg(LX_AUD_REG_DEC1_SBT, 0xFFFFFFFF);
	}

	BASE_AVLIPSYNC_UNLOCK();

	pRenInfo->status = AUD_STATUS_STOP;
	pRenInfo->bStartCalled = FALSE;

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("renIndex : %d \n", renIndex);

	return retVal;
}

SINT32 AUDIO_FlushRenderer(UINT32 allocDev)
{
	SINT32					retVal = RET_OK;
	SINT32					renIndex = 0;
	LX_AUD_RENDER_PARAM_T	*pRenderParam = NULL;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	AUD_MOD_INFO_T			*pModInfo = NULL;

	ADEC_MODULE_ID			allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_SOLA = ADEC_MODULE_NOT_DEF;

	ImcCmdFlushParam		flushParam;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];
	pRenderParam = &(pRenInfo->renderParam);

	memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
	flushParam.num_of_port	= 1;
	flushParam.port_list[0]	= MOD_IN_PORT(0);

	// Flush SRC
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, allocMod_SRC, sizeof(ImcCmdFlushParam), &flushParam);
	}

	// Flush SOLA
	if (TRUE == pRenderParam->bSetTrickMode)
	{
		allocMod_SOLA = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOLA);
		if(allocMod_SOLA != ADEC_MODULE_NOT_DEF)
		{
			AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, allocMod_SOLA, sizeof(ImcCmdFlushParam), &flushParam);
		}
	}

	// Flush Input Control
	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
	{
		pModInfo = AUDIO_GetModuleInfo(allocMod_INPUTCTRL);
		if(pModInfo != NULL)
		{
			AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, allocMod_INPUTCTRL, sizeof(ImcCmdFlushParam), &flushParam);
		}
	}

	pRenInfo->status = AUD_STATUS_FLUSH;

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("renIndex : %d \n", renIndex);

	return retVal;
}

/**
 * Sets volume for input control.
 * Controls the volume function.
 * @see
*/
SINT32 AUDIO_SetInputCtrlVolume(UINT32 allocDev, UINT32 volume)
{
	SINT32						renIndex = 0;
	InputctrlCmdSetGain			setInputCtrlGain;
	ADEC_MODULE_ID				inputCtrlModule = ADEC_MODULE_INPUTCTRL_0;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("renIndex[%d] is not available.\n", renIndex);
		return RET_OK;
	}

	inputCtrlModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(inputCtrlModule != ADEC_MODULE_NOT_DEF)
	{
		setInputCtrlGain.GainEnable = TRUE;
		setInputCtrlGain.Gain = volume;
		AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_GAIN, inputCtrlModule, sizeof(InputctrlCmdSetGain), &setInputCtrlGain);
	}

	AUD_KDRV_PRINT("vol %x \n", volume);

	return RET_OK;
}

/**
 * Sets mute for input control.
 * Controls the mute function.
 * @see
*/
SINT32 AUDIO_SetInputCtrlMute(UINT32 allocDev, UINT32 mute)
{
	SINT32						renIndex = 0;
	InputctrlCmdSetMute			setInputCtrlMute;
	ADEC_MODULE_ID				inputCtrlModule = ADEC_MODULE_INPUTCTRL_0;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("renIndex[%d] is not available.\n", renIndex);
		return RET_OK;
	}

	inputCtrlModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(inputCtrlModule != ADEC_MODULE_NOT_DEF)
	{
		setInputCtrlMute.Mute = mute;
		AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_MUTE, inputCtrlModule, sizeof(InputctrlCmdSetMute), &setInputCtrlMute);
	}

	AUD_KDRV_PRINT("mute %x \n", mute);

	return RET_OK;
}

/**
 * Sets delay for input control.
 * Controls the delay function.
 * @see
*/
SINT32 AUDIO_SetInputCtrlDelay(UINT32 allocDev, UINT32 delay)
{
	SINT32						renIndex = 0;
	InputctrlCmdSetDelay		setInputCtrlDelay;
	ADEC_MODULE_ID				inputCtrlModule = ADEC_MODULE_INPUTCTRL_0;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("renIndex[%d] is not available.\n", renIndex);
		return RET_OK;
	}

	inputCtrlModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(inputCtrlModule != ADEC_MODULE_NOT_DEF)
	{
		setInputCtrlDelay.delay = delay;
		AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, inputCtrlModule, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);
	}

	_gRenderInfo[renIndex].ui32InputCtrlDelay = delay;

	AUD_KDRV_PRINT("delay %x \n", delay);

	return RET_OK;
}

/**
 * Sets the AV Lipsync mechanism.
 * Controls the lipsync function manually.
 * @see
*/
static SINT32 AUDIO_SetLipsync(UINT32 allocDev, LX_AUD_RENDER_PARAM_LIPSYNC_T *pParamLipsync)
{
	SINT32				renIndex = 0;

	LipsyncCmdSetBound	setLipsyncBound;
	LipsyncCmdSetFs	setEsLipsyncFs;

	ADEC_MODULE_ID		allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	LX_AUD_RENDER_PARAM_T	*pRenParam = NULL;
	LX_AUD_RENDER_PARAM_LIPSYNC_T	*pRenLipsync = NULL;

	if(pParamLipsync == NULL)
	{
		AUD_KDRV_ERROR("pParamLipsync is NULL\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenParam = &_gRenderInfo[renIndex].renderParam;
	pRenLipsync = &_gRenderInfo[renIndex].renderLipsync;

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed \n", ADEC_MOD_TYPE_INPUTCTRL);
		return RET_ERROR;
	}

	setLipsyncBound.ubound = pParamLipsync->ui32Ubound;
	setLipsyncBound.lbound = pParamLipsync->ui32Lbound;
	setLipsyncBound.offset = pParamLipsync->ui32Offset;
	setLipsyncBound.freerunubound = pParamLipsync->ui32Freerunubound;
	setLipsyncBound.freerunlbound = pParamLipsync->ui32Freerunlbound;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BOUND, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBound), &setLipsyncBound);

	/* If lipsync type is ES, set a sampling frequency seeting to compute delay */
	if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_ES)
	{
		/* Set a default value */
		if(pRenParam->samplingFreq != LX_AUD_SAMPLING_FREQ_NONE)
		{
			setEsLipsyncFs.Fs = pRenParam->samplingFreq;
		}
		else
		{
			setEsLipsyncFs.Fs = LX_AUD_SAMPLING_FREQ_48_KHZ;
		}
		AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_FS, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetFs), &setEsLipsyncFs);
	}

	// save lipsync param
	pRenLipsync->ui32Ubound = pParamLipsync->ui32Ubound;
	pRenLipsync->ui32Lbound = pParamLipsync->ui32Lbound;
	pRenLipsync->ui32Offset = pParamLipsync->ui32Offset;
	pRenLipsync->ui32Freerunubound = pParamLipsync->ui32Freerunubound;
	pRenLipsync->ui32Freerunlbound = pParamLipsync->ui32Freerunlbound;

	return RET_OK;
}

/**
 * Get the AV Lipsync On Off.
 * @see
*/
SINT32 AUDIO_GetLipsyncOnOff(UINT32 allocDev, BOOLEAN *pLipsyncOnOff)
{
	SINT32				renIndex = 0;

	if(pLipsyncOnOff == NULL)
	{
		AUD_KDRV_ERROR("pLipsyncOnOff is NULL\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("renIndex[%d] is not available.\n", renIndex);
		return RET_OK;
	}

	*pLipsyncOnOff = _gRenderInfo[renIndex].bLipsyncOnOff;

	return RET_OK;
}

/**
 * Sets the AV Lipsync mechanism.
 * Controls the lipsync function manually.
 * @see
*/
SINT32 AUDIO_EnableLipsync(UINT32 allocDev, UINT32 bOnOff)
{
	SINT32	renIndex;

	ADEC_MODULE_ID		allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;

	LipsyncCmdSetOnoff	setLipsyncOnOff;

#ifdef USE_LIPSYNC_FREE
	LX_AUD_INPUT_T		inputPort = LX_AUD_INPUT_NONE;	///< input port
#endif

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed\n", ADEC_MOD_TYPE_INPUTCTRL);
		return RET_ERROR;
	}

	setLipsyncOnOff.onoff = bOnOff;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_ONOFF, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetOnoff), &setLipsyncOnOff);

	_gRenderInfo[renIndex].bLipsyncOnOff = bOnOff;

#ifdef USE_LIPSYNC_FREE
	inputPort = _gRenderInfo[renIndex].renderParam.input;

	if( (inputPort == LX_AUD_INPUT_TP0 || inputPort == LX_AUD_INPUT_TP1)	\
	  &&(bOnOff == ON) )
	{
		AUD_RENDER_INFO_T	*pRenInfo = NULL;

		pRenInfo = &_gRenderInfo[renIndex];

		_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenMixerUnderflowCb, ADEC_MODULE_MIX_0, MIXER_EVT_UNDERFLOW, IMC_ACTION_REPEAT, 1);
	}
#endif

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("AUDIO_EnableLipsync(%s)\n", (bOnOff == TRUE ? "On" : "Off"));

	return RET_OK;
}

/**
 * Sets Basetime Parameters for Clock.
 * Controls the AV Lipsync function.
 * @see
*/
static SINT32 AUDIO_SetBasetime(UINT32 allocDev, LX_AUD_RENDER_PARAM_BASETIME_T *pParamBasetime)
{
	SINT32				renIndex;
	ADEC_MODULE_ID		allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;

	AUD_RENDER_INFO_T	*pRenInfo = NULL;

	LipsyncCmdSetBase	basetime;

	if(pParamBasetime == NULL)
	{
		AUD_KDRV_ERROR("pParamBasetime is NULL\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenInfo = &_gRenderInfo[renIndex];

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType[%d.%d] failed \n", allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		return RET_ERROR;
	}

	basetime.clockbase	= (UINT32)(pParamBasetime->ui64ClockBaseTime & TIMESTAMP_MASK);
	basetime.streambase	= (UINT32)(pParamBasetime->ui64StreamBaseTime & TIMESTAMP_MASK);

	// Change timestamp mask for NO PCR Mode
	if((pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_NO_PCR)
		|| (pRenInfo->renderClockType == LX_AUD_RENDER_CLK_TYPE_MASTER))
	{
		basetime.clockbase	= (UINT32)(pParamBasetime->ui64ClockBaseTime  & TIMESTAMP_MASK_NO_PCR);
		basetime.streambase = (UINT32)(pParamBasetime->ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR);
		AUD_KDRV_NOTI("Set Basetime(%d) : Bst(%10d), Bct(%10d)\n", allocDev, basetime.streambase, basetime.clockbase);
	}

	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BASE, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBase), &basetime);

	pRenInfo->renderBasetime.ui64ClockBaseTime = basetime.clockbase;
	pRenInfo->renderBasetime.ui64StreamBaseTime = basetime.streambase;

	AUD_KDRV_PRINT("basetime - stream(%10d), clock(%10d)\n", basetime.streambase, basetime.clockbase);

	return RET_OK;
}

/**
 * Sets Clock Type Parameters for lipcync.
 * Controls the AV Lipsync function.
 * @see
*/
static SINT32 AUDIO_SetClockType(UINT32 allocDev, LX_AUD_RENDER_CLK_TYPE_T clockType)
{
	SINT32					renIndex;

	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;

	LipsyncCmdSetclocktype	setLipsyncClockType;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenInfo = &_gRenderInfo[renIndex];

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed \n", ADEC_MOD_TYPE_INPUTCTRL);
		return RET_ERROR;
	}

	setLipsyncClockType.clocktype = clockType;
 	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_CLOCKTYPE, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetclocktype), &setLipsyncClockType);

	// Register Basetime
	if((clockType == LX_AUD_RENDER_CLK_TYPE_NO_PCR)
		|| (clockType == LX_AUD_RENDER_CLK_TYPE_MASTER))
	{
		_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenBasetimeCb, allocMod_INPUTCTRL, LIPSYNC_EVT_NOPCR_BASELINE, IMC_ACTION_REPEAT, 1);
	}

	pRenInfo->renderClockType = clockType;

	AUD_KDRV_PRINT("Dev %x, clocktype %d\n", allocDev, clockType);

	return RET_OK;
}

/**
 * Sets Codec Parameters for Renderer.
 * Controls the Renderer Codec Param function.
 * @see
*/
SINT32 AUDIO_SetRendererParam(UINT32 allocDev, LX_AUD_RENDER_PARAM_T *pParamCodec)
{
	SINT32		retVal = 0;
	SINT32		renIndex = 0;

	ADEC_MODULE_ID			allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_SOLA = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			allocMod_CTRL_SRC = ADEC_MODULE_NOT_DEF;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	LX_AUD_RENDER_PARAM_T	*pRenParam = NULL;
	AUD_DEV_INFO_T			*pRenDevInfo = NULL;
	LipsyncCmdSetDatatype	setLipsyncDataType;
	PcmcvtCmdSetSrcRunningMode	srcRunMode;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];
	pRenParam = &_gRenderInfo[renIndex].renderParam;

	pRenDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pRenDevInfo == NULL)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_DEBUG("pRenDevInfo[%d] is NULL.\n", allocDev);
		return RET_ERROR;
	}

	/* Copy a renderer param to global variable. */
	pRenParam->input				= pParamCodec->input;
	pRenParam->lipsyncType			= pParamCodec->lipsyncType;
	pRenParam->ui32NumOfChannel	= pParamCodec->ui32NumOfChannel;
	pRenParam->ui32BitPerSample	= pParamCodec->ui32BitPerSample;
	pRenParam->samplingFreq		= pParamCodec->samplingFreq;
	pRenParam->endianType			= pParamCodec->endianType;			///< the endian of PCM
	pRenParam->signedType			= pParamCodec->signedType;			///< the signed of PCM
	pRenParam->bSetMainRen		= pParamCodec->bSetMainRen;			///< Sub Ren Mode
	pRenParam->bSetTrickMode		= pParamCodec->bSetTrickMode;		///< trick play mode

	if(pParamCodec->lipsyncType == LX_AUD_RENDER_LIPSYNC_ES)
	{
		allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
		{
			if((pRenDevInfo->index == 0) || (pRenDevInfo->index == 1))
			{
				allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_10 + pRenDevInfo->index;
				allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, allocMod_INPUTCTRL);
				if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
				{
					AUD_REN_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_INPUTCTRL);
					return RET_ERROR;
				}
			}
			else
			{
				allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
				if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
				{
					AUD_REN_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_INPUTCTRL);
					return RET_ERROR;
				}
			}

			retVal = AUDIO_InsertModuleLast(allocDev, allocMod_INPUTCTRL, AUD_BUFFER_TYPE_NONE);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}
		}

		setLipsyncDataType.datatype = LIPSYNC_DATATYPE_ES;
		AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_DATATYPE, allocMod_INPUTCTRL,
			sizeof(LipsyncCmdSetDatatype), &setLipsyncDataType);

		pRenDevInfo->devOutType = LX_AUD_DEV_OUT_ES;

		AUD_REN_UNLOCK();

		AUD_KDRV_PRINT("Dev %x, InFs %d, InFm %d\n",
			allocDev, pParamCodec->samplingFreq, pParamCodec->ui32BitPerSample);

		return RET_OK;
	}

	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
	{
		if(pRenDevInfo->index < MAX_DEV_INDEX)
		{
			allocMod_SRC = ADEC_MODULE_PCMCVT_P0 + pRenDevInfo->index;
			allocMod_SRC = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP1, allocMod_SRC);
		}
		else
		{
			allocMod_SRC = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
		}
		if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_PCMCVT);
			return RET_ERROR;
		}

		retVal = AUDIO_InsertModuleLast(allocDev, allocMod_SRC, AUD_BUFFER_TYPE_NONE);
		if(retVal < RET_OK)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}
	}

	_AUDIO_SetSRCFormat(allocMod_SRC, pParamCodec->samplingFreq,
						pParamCodec->ui32NumOfChannel, pParamCodec->ui32BitPerSample,
						pParamCodec->endianType, pParamCodec->signedType);

	if(pRenInfo->renderParam.bSetMainRen)
	{
		srcRunMode.mode = 1;
	}
	else
	{
		srcRunMode.mode = 0;
	}

	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_SRC_RUNNING_MODE, allocMod_SRC, sizeof(PcmcvtCmdSetSrcRunningMode), &srcRunMode);

	if (TRUE == pParamCodec->bSetTrickMode)
	{
		allocMod_SOLA = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOLA);
		if(allocMod_SOLA == ADEC_MODULE_NOT_DEF)
		{
			allocMod_SOLA = AUDIO_AllocModule(ADEC_MOD_TYPE_SOLA, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
			if(allocMod_SOLA == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_SOLA);
				return RET_ERROR;
			}

			retVal = AUDIO_InsertModuleLast(allocDev, allocMod_SOLA, AUD_BUFFER_TYPE_NONE);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}
		}
	}

	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		if(pRenDevInfo->index < MAX_DEV_INDEX)
		{
			allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_0 + pRenDevInfo->index;
			allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, allocMod_INPUTCTRL);
		}
		else
		{
			allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
		}

		if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_INPUTCTRL);
			return RET_ERROR;
		}

		retVal = AUDIO_InsertModuleLast(allocDev, allocMod_INPUTCTRL, AUD_BUFFER_TYPE_NONE);
		if(retVal < RET_OK)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("Connection_InsertLast(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}
	}

	setLipsyncDataType.datatype = LIPSYNC_DATATYPE_PCM;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_DATATYPE, allocMod_INPUTCTRL,
		sizeof(LipsyncCmdSetDatatype), &setLipsyncDataType);

	// insert input control
	allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
	{
		if(pRenDevInfo->index < MAX_DEV_INDEX)
		{
			allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_0 + pRenDevInfo->index;
			allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, allocMod_INPUTCTRL);
		}
		else
		{
			allocMod_INPUTCTRL = AUDIO_AllocModule(ADEC_MOD_TYPE_INPUTCTRL, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
		}

		if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
		{
			retVal = AUDIO_InsertModuleLast(allocDev, allocMod_INPUTCTRL, AUD_BUFFER_TYPE_NONE);
			if(retVal < RET_OK)
			{
				AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
			//	return RET_ERROR;
			}
		}
	}

	if ( (pRenParam->input == LX_AUD_INPUT_SIF)	\
		||(pRenParam->input == LX_AUD_INPUT_HDMI)	\
		||(pRenParam->input >= LX_AUD_INPUT_HDMI0 && pRenParam->input <= LX_AUD_INPUT_HDMI3) )
	{
		allocMod_CTRL_SRC = AUDIO_FindModuleType(pRenInfo->ui32AllocDev, ADEC_MOD_TYPE_CTRLSRC);
		if(allocMod_CTRL_SRC == ADEC_MODULE_NOT_DEF)
		{
			allocMod_CTRL_SRC = AUDIO_AllocModule(ADEC_MOD_TYPE_CTRLSRC, ADEC_CORE_DSP1, ADEC_MODULE_NOT_DEF);
			if(allocMod_CTRL_SRC == ADEC_MODULE_NOT_DEF)
			{
				AUD_KDRV_ERROR("AUDIO_AllocModule(%d) failed!!!\n", ADEC_MOD_TYPE_CTRLSRC);
				return RET_ERROR;
			}

			retVal = AUDIO_AddModule(pRenInfo->ui32AllocDev, allocMod_CTRL_SRC);
			if(retVal < RET_OK)
			{
				AUD_KDRV_ERROR("AUDIO_AddModule failed(%d)!!!\n", retVal);
				return RET_ERROR;
			}
		}
	}

	pRenDevInfo->devOutType = LX_AUD_DEV_OUT_PCM;

	// Register Tone Detect
	if(pRenInfo->renderParam.bSetMainRen == TRUE)
	{
		if(pRenInfo->renderParam.input == LX_AUD_INPUT_ADC)
			_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenToneDetectCb, allocMod_SRC, SRC_EVT_DETECT_TONE, IMC_ACTION_REPEAT, 1);
		else if ( (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI) \
				||(pRenInfo->renderParam.input >= LX_AUD_INPUT_HDMI0 && pRenInfo->renderParam.input <= LX_AUD_INPUT_HDMI3) )
			_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenToneDetectCb, allocMod_SRC, SRC_EVT_DETECT_TONE, IMC_ACTION_REPEAT, 1);
	}

	// Register Present End
	_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenPresentEndCb, allocMod_INPUTCTRL, LIPSYNC_EVT_PRESENT_END, IMC_ACTION_REPEAT, 1);

	// Register Decoding Index
	if((pRenParam->input == LX_AUD_INPUT_SYSTEM) ||
		(pRenParam->input == LX_AUD_INPUT_SYSTEM_CLIP))
		_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenStatusCb, allocMod_INPUTCTRL, LIPSYNC_EVT_PRESENTINDEX, IMC_ACTION_REPEAT, 100);

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("Dev %x, InFs %d, InBs %d\n", allocDev, pParamCodec->samplingFreq, pParamCodec->ui32BitPerSample);

	return RET_OK;
}


/**
 * Connect other device to renderer.
 * @see
*/
SINT32 AUDIO_ConnectRenderer(UINT32 allocDev, UINT32 connectDev)
{
	SINT32						retVal = 0;
	SINT32						renIndex = 0;

	AUD_DEV_INFO_T				*pRenDevInfo = NULL;
	AUD_DEV_INFO_T				*pConnectDevInfo = NULL;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;
	LX_AUD_RENDER_PARAM_T		*pRenParam = NULL;
	ADEC_MODULE_ID				managerModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_DEC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_ENC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_RTS = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_BypassDSP0 = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_MIXER = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_MPB = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_Head = ADEC_MODULE_NOT_DEF;

	InputctrlCmdSetGain			setInputCtrlGain;
	InputctrlCmdSetMute			setInputCtrlMute;
	InputctrlCmdSetDelay		setInputCtrlDelay;

	BypassCmdSetMode			bypassMode;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];
	pRenParam = &_gRenderInfo[renIndex].renderParam;

	// if connect with DEC, connect DPB with SRC
	// else if connect with REN, connect MPB with SRC
	pRenDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pRenDevInfo == NULL)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_DEBUG("AUDIO_GetDevInfo(%d)\n", allocDev);
		return RET_ERROR;
	}

	if(connectDev != LX_AUD_DEV_NONE)
	{
		pConnectDevInfo = AUDIO_GetDevInfo(connectDev);
	}
	else
	{
		LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

		devIndexCtx.devType = LX_AUD_DEV_TYPE_DEC;
		devIndexCtx.ui32Index = pRenDevInfo->index;
		devIndexCtx.devOutType = LX_AUD_DEV_OUT_PCM_ES;
		retVal = AUDIO_GetDevIndexCtx(&devIndexCtx);
		if(retVal != RET_OK)
		{
			AUD_KDRV_ERROR("AUDIO_GetDevIndexCtx(%d) failed!!!\n", retVal);
			AUD_REN_UNLOCK();
			return RET_ERROR;
		}

		connectDev = devIndexCtx.dev;
		pConnectDevInfo = AUDIO_GetDevInfo(connectDev);
	}

	// Sanity check for allocated module
	if(pConnectDevInfo == NULL)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_DEBUG("AUDIO_GetDevInfo(%d)\n", connectDev);
		return RET_ERROR;
	}

	if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
		{
			allocMod_BypassDSP0 = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_BYPASS);
			if(allocMod_BypassDSP0 == ADEC_MODULE_NOT_DEF)
			{
				allocMod_BypassDSP0 = AUDIO_AllocModule(ADEC_MOD_TYPE_BYPASS, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
				if(allocMod_BypassDSP0 == ADEC_MODULE_NOT_DEF)
				{
					AUD_REN_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_AllocModule(%d) failed \n", ADEC_MOD_TYPE_BYPASS);
					return RET_ERROR;
				}

				bypassMode.mode = AU_BASED_MODE ;
				bypassMode.over_protect = NO_OVERFLOW_PROTECTION;
				AUDIO_IMC_SendCmdParam(BYPASS_CMD_SET_MODE, allocMod_BypassDSP0, sizeof(BypassCmdSetMode), &bypassMode);

				retVal = AUDIO_InsertModuleLast(connectDev, allocMod_BypassDSP0, AUD_BUFFER_TYPE_NONE);
				if(retVal < RET_OK)
				{
					AUD_REN_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
					return RET_ERROR;
				}
			}

			retVal = AUDIO_ConnectDevices(connectDev, allocDev, AUD_BUFFER_TYPE_DPB);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnecDevices(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}

			// insert input control
			allocMod_INPUTCTRL= AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
			if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
			{
				if(pRenDevInfo->index < DEV_DEC_NUM)
				{
					setInputCtrlGain.GainEnable = TRUE;
					setInputCtrlGain.Gain = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlVolume;
					AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_GAIN, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetGain), &setInputCtrlGain);

					setInputCtrlMute.Mute = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlMute;
					AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_MUTE, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetMute), &setInputCtrlMute);

					setInputCtrlDelay.delay = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlDelay;
					AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);
				}
			}

			// set DPB to lipsync module's reference port
			allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
			if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_PCMCVT);
				return RET_ERROR;
			}

			allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_INPUTCTRL);
				return RET_ERROR;
			}

			managerModule = AUDIO_GetManagerModule(allocMod_INPUTCTRL);
			if(managerModule != ADEC_MODULE_NOT_DEF)
			{
				CmCmdSetRefPorts	setRefBufPort;

				setRefBufPort.ref_module = allocMod_INPUTCTRL;
				setRefBufPort.num_of_ref = 1;
				setRefBufPort.ref_port[0] = MOD_REF_PORT(8);
				setRefBufPort.src_module[0] = allocMod_SRC;
				setRefBufPort.src_port[0] = MOD_IN_PORT(0);
				AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, managerModule, sizeof(CmCmdSetRefPorts), &setRefBufPort);
			}
		}
		else	//if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_ES)
		{
			allocMod_DEC = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
			if(allocMod_DEC == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", connectDev, ADEC_MOD_TYPE_DECODER);
				return RET_ERROR;
			}

			allocMod_INPUTCTRL = AUDIO_FindHeadModule(allocDev);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindHeadModule failed!!!\n");
				return RET_ERROR;
			}

			if(pRenDevInfo->index < DEV_DEC_NUM)
			{
				setInputCtrlMute.Mute = _gInputCtrlInfo[pRenDevInfo->index + MAX_DEV_INDEX].ui32InputCtrlMute;
				AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_MUTE, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetMute), &setInputCtrlMute);

				setInputCtrlDelay.delay = _gInputCtrlInfo[pRenDevInfo->index + MAX_DEV_INDEX].ui32InputCtrlDelay;
				AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);
			}

			retVal = AUDIO_ConnectModules(allocMod_DEC, allocMod_INPUTCTRL, AUD_BUFFER_TYPE_IEC);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnecModules(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}
		}

		// DPB/IEC Buffer Flush
		allocMod_Head = AUDIO_FindHeadModule(allocDev);
		if(allocMod_Head != ADEC_MODULE_NOT_DEF)
		{
			ImcCmdFlushParam 		flushParam;

			memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
			flushParam.num_of_port	= 1;
			flushParam.port_list[0]	= MOD_IN_PORT(0);

			AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, allocMod_Head, sizeof(ImcCmdFlushParam), &flushParam);
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_REN)
	{
		// set MPB to lipsync module's reference port
		allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
		if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_PCMCVT);
			return RET_ERROR;
		}

		// get MBP
		allocMod_MPB = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
		if(allocMod_MPB == ADEC_MODULE_NOT_DEF)
		{
			allocMod_MPB = AUDIO_AllocModule(ADEC_MOD_TYPE_EXT_BUF, ADEC_CORE_ARM, ADEC_MODULE_NOT_DEF);
			if(allocMod_MPB == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed \n", ADEC_MOD_TYPE_EXT_BUF);
				return RET_ERROR;
			}

			// connect MBP to SRC module
			retVal = AUDIO_ConnectModules(allocMod_MPB, allocMod_SRC, AUD_BUFFER_TYPE_MPB);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnectModules(%d,%d) failed(%d)!!!\n", \
					allocMod_MPB, allocMod_SRC, retVal);
				return RET_ERROR;
			}

			retVal = AUDIO_AddModule(allocDev, allocMod_MPB);
			if(retVal < 0)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", \
					allocMod_MPB, retVal);
				return RET_ERROR;
			}
		}

		allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
		{
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed\n", allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		}
		else
		{
			managerModule = AUDIO_GetManagerModule(allocMod_INPUTCTRL);
			if(managerModule != ADEC_MODULE_NOT_DEF)
			{
				CmCmdSetRefPorts	setRefBufPort;

				setRefBufPort.ref_module	= allocMod_INPUTCTRL;
				setRefBufPort.num_of_ref	= 1;
				setRefBufPort.ref_port[0]	= MOD_REF_PORT(8);
				setRefBufPort.src_module[0]	= allocMod_SRC;
				setRefBufPort.src_port[0]	= MOD_IN_PORT(0);
				AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, managerModule, sizeof(CmCmdSetRefPorts), &setRefBufPort);
			}

			// set input control
			if(pRenDevInfo->index < MAX_DEV_INDEX)
			{
				setInputCtrlGain.GainEnable = TRUE;
				setInputCtrlGain.Gain = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlVolume;
				AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_GAIN, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetGain), &setInputCtrlGain);

				setInputCtrlMute.Mute = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlMute;
				AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_MUTE, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetMute), &setInputCtrlMute);

				setInputCtrlDelay.delay = _gInputCtrlInfo[pRenDevInfo->index].ui32InputCtrlDelay;
				AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_DELAY, allocMod_INPUTCTRL, sizeof(InputctrlCmdSetDelay), &setInputCtrlDelay);
			}
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		allocMod_INPUTCTRL = AUDIO_FindTailModule(allocDev);
		allocMod_MIXER = AUDIO_FindHeadModule(connectDev);

		if(pRenDevInfo->index < MAX_DEV_INDEX)
		{
			retVal = AUDIO_ConnectModulePort(allocMod_INPUTCTRL, allocMod_MIXER, pRenDevInfo->index);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnectModulePort(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}

			pRenInfo->ui32MixerPort = (UINT32)pRenDevInfo->index;
		}
		else
		{
			retVal = AUDIO_ConnectDevices(allocDev, connectDev, AUD_BUFFER_TYPE_NONE);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnectDevices(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}

			retVal = AUDIO_GetPort(allocMod_MIXER, IN_PORT, allocMod_INPUTCTRL);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_GetPort(%d.%d.%d) failed(%d)!!!\n", \
					allocMod_MIXER, IN_PORT, allocMod_INPUTCTRL, retVal);
				return RET_ERROR;
			}

			pRenInfo->ui32MixerPort = (UINT32)retVal;
		}

		if(pRenParam->input == LX_AUD_INPUT_SYSTEM)
		{
			// RTS setting
			if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
									pRenInfo->rtsParam.mixFadeLen,
									pRenInfo->rtsParam.mixWaitLen);
			}
			else	// Other EMP
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
										FADE_LENGTH_DEFAULT,
										MIX_WAIT_LENGTH_EMP);
			}
		}	// Others
		else if(pRenParam->input == LX_AUD_INPUT_SYSTEM_CLIP)
		{
			// RTS setting
			if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
									pRenInfo->rtsParam.mixFadeLen,
									pRenInfo->rtsParam.mixWaitLen);
			}
			else	// Other EMP
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
										FADE_LENGTH_1,
										MIX_WAIT_LENGTH_EMP);
			}
		}
		else if ( (pRenInfo->renderParam.input == LX_AUD_INPUT_HDMI) \
				||(pRenInfo->renderParam.input >= LX_AUD_INPUT_HDMI0 && pRenInfo->renderParam.input <= LX_AUD_INPUT_HDMI3) )
		{
			LX_AUD_DECODER_PARAM_T	decParam;
			LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

			devIndexCtx.devType = LX_AUD_DEV_TYPE_DEC;
			devIndexCtx.ui32Index = pRenDevInfo->index;

			AUDIO_GetDevIndexCtx(&devIndexCtx);

			KDRV_AUDIO_GetDecoderParam(devIndexCtx.dev, &decParam);
			if (decParam.codecType == LX_AUD_CODEC_AC3)
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
										FADE_LENGTH_0,
										MIX_WAIT_LENGTH_HDMI);
			}
			else
			{
				_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
										FADE_LENGTH_DEFAULT,
										MIX_WAIT_LENGTH_DEFAULT);
			}
		}	// Others
		else
		{
			_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
									FADE_LENGTH_DEFAULT,
									MIX_WAIT_LENGTH_DEFAULT);
		}

		// RTS setting
		if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
		{
			RtsCmdSetSrcModule	rtsSrcModule;
			CmCmdSetRefPorts	rtsRefBuffer;

			// get RTS
			allocMod_RTS = AUDIO_AllocModule(ADEC_MOD_TYPE_MANAGER, ADEC_CORE_DSP1, ADEC_MODULE_RTS);
			if (allocMod_RTS != ADEC_MODULE_NOT_DEF)
			{
				allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
				if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
				{
					// set PCMCVT module
					rtsSrcModule.src_module_id = allocMod_SRC;
					AUDIO_IMC_SendCmdParam(RTS_CMD_SET_SRC_MODULE, allocMod_RTS, sizeof(RtsCmdSetSrcModule), &rtsSrcModule);

					// set reference buffer - PCMCVT's input buffer
					rtsRefBuffer.ref_module		= allocMod_RTS;
					rtsRefBuffer.num_of_ref		= 1;
					rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(0);
					rtsRefBuffer.src_module[0]	= allocMod_SRC;
					rtsRefBuffer.src_port[0]	= MOD_IN_PORT(0);
					AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdSetRefPorts), &rtsRefBuffer);
				}

				allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
				if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
				{
					// set reference buffer - LIP's input buffer
					rtsRefBuffer.ref_module		= allocMod_RTS;
					rtsRefBuffer.num_of_ref		= 1;
					rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(1);
					rtsRefBuffer.src_module[0]	= allocMod_INPUTCTRL;
					rtsRefBuffer.src_port[0]	= MOD_IN_PORT(0);
					AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdSetRefPorts), &rtsRefBuffer);
				}
#if 0
				allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
				if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
				{
					// set reference buffer - INCTRL's input buffer
					rtsRefBuffer.ref_module		= allocMod_RTS;
					rtsRefBuffer.num_of_ref		= 1;
					rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(2);
					rtsRefBuffer.src_module[0]	= allocMod_InputCtrl;
					rtsRefBuffer.src_port[0]	= MOD_IN_PORT(0);
					AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdSetRefPorts), &rtsRefBuffer);
				}
#endif
				if (0xFF != pRenInfo->ui32MixerPort)
				{
					// set reference buffer - MIX's input buffer
					rtsRefBuffer.ref_module		= allocMod_RTS;
					rtsRefBuffer.num_of_ref		= 1;
					rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(2);
					rtsRefBuffer.src_module[0]	= ADEC_MODULE_MIX_0;
					rtsRefBuffer.src_port[0]	= MOD_IN_PORT(pRenInfo->ui32MixerPort);
					AUDIO_IMC_SendCmdParam(CM_CMD_SET_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdSetRefPorts), &rtsRefBuffer);
				}
			}

			retVal = AUDIO_AddModule(allocDev, allocMod_RTS);
			if(retVal < 0)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", \
					allocMod_RTS, retVal);
				return RET_ERROR;
			}
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_ENC)
	{
		if(pRenInfo->renderParam.lipsyncType == LX_AUD_RENDER_LIPSYNC_ES)
		{
			allocMod_ENC = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_ENCODER);
			if(allocMod_ENC == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", connectDev, ADEC_MOD_TYPE_ENCODER);
				return RET_ERROR;
			}

			allocMod_INPUTCTRL = AUDIO_FindHeadModule(allocDev);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindHeadModule failed!!!\n");
				return RET_ERROR;
			}

			retVal = AUDIO_ConnectModules(allocMod_ENC, allocMod_INPUTCTRL, AUD_BUFFER_TYPE_IEC);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnecModules(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}
		}
	}
	else
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_ERROR("NOT valid connection\n");
		return RET_ERROR;
	}

	if(pRenInfo->bResetting == FALSE)
	{
		pRenInfo->renderConnect[pRenInfo->ui32ConnectNum].devType    = pConnectDevInfo->devType;
		pRenInfo->renderConnect[pRenInfo->ui32ConnectNum].connectDev = pConnectDevInfo->dev;
		pRenInfo->ui32ConnectNum++;
	}

	// Register Notification
	if (pConnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		allocMod_DEC = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
		if(allocMod_DEC != ADEC_MODULE_NOT_DEF)
		{
			_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenDecInfoCb, allocMod_DEC, DEC_EVT_ES_DEC_INFO, IMC_ACTION_REPEAT, 1);
			_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_SpdifTypeCb, allocMod_DEC, DEC_EVT_REQ_SPDIF_TYPE, IMC_ACTION_REPEAT, 1);
			(void)AUDIO_IMC_SendCmd(ADEC_CMD_GET_DECINFO, allocMod_DEC);
		}
	}

	AUD_REN_UNLOCK();

	AUD_KDRV_PRINT("allocDev[%d] connectDev[%d] \n", allocDev, connectDev);

	return RET_OK;
}

/**
 * Disconnect other device to renderer.
 * @see
*/
SINT32 AUDIO_DisconnectRenderer(UINT32 allocDev, UINT32 connectDev)
{
	SINT32						retVal = 0;
	SINT32						renIndex = 0;

	ADEC_MODULE_ID				managerModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_MPB = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_DEC = ADEC_MODULE_NOT_DEF;
	AUD_DEV_INFO_T 				*pRenDevInfo, * pConnectDevInfo;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];

	pRenDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pRenDevInfo == NULL)
	{
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed!!!\n", allocDev);
		AUD_REN_UNLOCK();
		return RET_ERROR;
	}

	if(connectDev != LX_AUD_DEV_NONE)
	{
		pConnectDevInfo = AUDIO_GetDevInfo(connectDev);
	}
	else
	{
		LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

		devIndexCtx.devType = LX_AUD_DEV_TYPE_DEC;
		devIndexCtx.ui32Index = pRenDevInfo->index;
		devIndexCtx.devOutType = LX_AUD_DEV_OUT_PCM_ES;
		retVal = AUDIO_GetDevIndexCtx(&devIndexCtx);
		if(retVal != RET_OK)
		{
			AUD_KDRV_ERROR("AUDIO_GetDevIndexCtx(%d) failed!!!\n", retVal);
			AUD_REN_UNLOCK();
			return RET_ERROR;
		}

		connectDev = devIndexCtx.dev;
		pConnectDevInfo = AUDIO_GetDevInfo(connectDev);
	}

	if(pConnectDevInfo == NULL)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_DEBUG("AUDIO_GetDevInfo(%d)\n", connectDev);
		return RET_ERROR;
	}

	if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		// if this dev is not a lip ES render
		if(pRenInfo->renderParam.lipsyncType == LX_AUD_RENDER_LIPSYNC_PCM)
		{
			retVal = AUDIO_DisconnectDevices(connectDev, allocDev);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_DisconnectDevices(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}

			// clear DPB from lipsync module's reference port
			allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_INPUTCTRL);
				return RET_ERROR;
			}

			managerModule = AUDIO_GetManagerModule(allocMod_INPUTCTRL);
			if(managerModule != ADEC_MODULE_NOT_DEF)
			{
				CmCmdClrRefPorts	clearRefBufPort;

				clearRefBufPort.module		= allocMod_INPUTCTRL;
				clearRefBufPort.num_of_ref	= 1;
				clearRefBufPort.ref_port[0]	= MOD_REF_PORT(8);
				AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, managerModule, sizeof(CmCmdClrRefPorts), &clearRefBufPort);
			}
		}
		else
		{
			allocMod_DEC = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
			if(allocMod_DEC == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed!!!\n", connectDev, ADEC_MOD_TYPE_DECODER);
				return RET_ERROR;
			}

			allocMod_INPUTCTRL = AUDIO_FindHeadModule(allocDev);
			if(allocMod_INPUTCTRL == ADEC_MODULE_NOT_DEF)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindHeadModule failed!!!\n");
				return RET_ERROR;
			}

			retVal = AUDIO_DisconnectModules(allocMod_DEC, allocMod_INPUTCTRL);
			if(retVal < RET_OK)
			{
				AUD_REN_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d) failed!!!\n", retVal);
				return RET_ERROR;
			}
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_REN)
	{
		allocMod_MPB = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
		if(allocMod_MPB == ADEC_MODULE_NOT_DEF)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_DEBUG("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
			return RET_OK;
		}

		allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
		if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_DEBUG("AUDIO_FindModuleType(%d.%d) failed!!!\n", allocDev, ADEC_MOD_TYPE_PCMCVT);
			return RET_OK;
		}

		// disconnect MBP to SRC module
		retVal = AUDIO_DisconnectModules(allocMod_MPB, allocMod_SRC);
		if(retVal < RET_OK)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d,%d) failed(%d)!!!\n", \
				allocMod_MPB, allocMod_SRC, retVal);
			return RET_ERROR;
		}

		retVal = AUDIO_RemoveModule(allocDev, allocMod_MPB);
		if(retVal < RET_OK)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_RemoveModule(%d,%d) failed(%d)!!!\n", \
				allocDev, allocMod_MPB, retVal);
			return RET_ERROR;
		}

		AUDIO_FreeModule(allocMod_MPB);

		// clear MPB from lipsync module's reference port
		allocMod_INPUTCTRL = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
		if(allocMod_INPUTCTRL != ADEC_MODULE_NOT_DEF)
		{
			managerModule = AUDIO_GetManagerModule(allocMod_INPUTCTRL);
			if(managerModule != ADEC_MODULE_NOT_DEF)
			{
				CmCmdClrRefPorts	clearRefBufPort;

				clearRefBufPort.module		= allocMod_INPUTCTRL;
				clearRefBufPort.num_of_ref	= 1;
				clearRefBufPort.ref_port[0]	= MOD_REF_PORT(8);
				AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, managerModule, sizeof(CmCmdClrRefPorts), &clearRefBufPort);
			}
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		// initialize mixer port
		_AUDIO_SetMixerConfig(pRenInfo->ui32MixerPort,
								FADE_LENGTH_DEFAULT,
								MIX_WAIT_LENGTH_DEFAULT);

		// RTS setting
		if (pRenInfo->rtsParam.bRTSOnOff == TRUE)
		{
			CmCmdClrRefPorts	rtsRefBuffer;

			rtsRefBuffer.module			= ADEC_MODULE_RTS;
			rtsRefBuffer.num_of_ref		= 1;
			rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(0);
			AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdClrRefPorts), &rtsRefBuffer);

			rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(1);
			AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdClrRefPorts), &rtsRefBuffer);

			rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(2);
			AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdClrRefPorts), &rtsRefBuffer);

			rtsRefBuffer.ref_port[0]	= MOD_REF_PORT(3);
			AUDIO_IMC_SendCmdParam(CM_CMD_CLR_REF_PORTS, ADEC_MODULE_MAN_DSP1, sizeof(CmCmdClrRefPorts), &rtsRefBuffer);
		}

		retVal = AUDIO_DisconnectDevices(allocDev, connectDev);
		if(retVal < RET_OK)
		{
			AUD_REN_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_DisconnectDevices(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}
	}
	else
	{
		AUD_KDRV_ERROR("devType(%d)is invalid.\n", pConnectDevInfo->devType);
	}

	AUD_REN_UNLOCK();

	return RET_OK;
}

/**
 * open handler for audio render device
 *
 */
UINT32	AUDIO_OpenRenderer(void)
{
	UINT32		allocDev = LX_AUD_DEV_NONE;
	SINT32		renIndex = 0;
	LX_AUD_DEV_TYPE_T		devType;

	if(_gRenSemaInit == TRUE)
		AUD_REN_LOCK();

	devType = LX_AUD_DEV_TYPE_REN;

	allocDev = AUDIO_AllocDev(devType);
	if(allocDev == LX_AUD_DEV_NONE)
	{
		if(_gRenSemaInit == TRUE)
			AUD_REN_UNLOCK();

		AUD_KDRV_ERROR("AUDIO_AllocDev(%d) failed.\n", devType);
		return LX_AUD_DEV_NONE;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		if(_gRenSemaInit == TRUE)
			AUD_REN_UNLOCK();

		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	if(_AUDIO_InitRendererParam(allocDev) == RET_ERROR)
	{
		AUD_KDRV_ERROR("_AUDIO_InitRendererParam[%d] failed.\n", allocDev);
		if(_gRenSemaInit == TRUE)
			AUD_REN_UNLOCK();

		AUDIO_FreeDev(allocDev);
		allocDev = LX_AUD_DEV_NONE;
	}

	if(_gRenSemaInit == TRUE)
		AUD_REN_UNLOCK();

	//Initialize AUDIO Renderer semaphore
	if(_gRenSemaInit == FALSE)
	{
		AUD_REN_LOCK_INIT();
		_gRenSemaInit = TRUE;
	}

	// to fix audio decoder not working
	_gRenderInfo[renIndex].ui32CheckSum = _gRenderCheckSum;

	AUD_KDRV_DEBUG("AUDIO_OpenRenderer : alloc %d.\n", allocDev);

	return allocDev;
}

/**
 * Function for Close Renderer.
 * Close Renderer temporary.
 * @see
*/
SINT32 AUDIO_CloseRenderer(UINT32 allocDev)
{
	SINT32					renIndex = 0;
	UINT32					i = 0;
	AUD_EVENT_T	 			*pRenEvent = NULL;
	AUD_RENDER_INFO_T		*pRenInfo = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_LOCK();

	pRenInfo = &_gRenderInfo[renIndex];

	if(pRenInfo->bClosed == TRUE)
	{
		AUD_REN_UNLOCK();
		AUD_KDRV_PRINT("renIndex[%d] is already closed. \n", renIndex);
		return RET_OK;
	}

	AUDIO_DisconnectAllModules(allocDev);

	AUDIO_FreeDev(allocDev);

	for(i = 0; i < pRenInfo->ui32EventNum; i++)
	{
		pRenEvent = &pRenInfo->renderEvent[i];
		AUDIO_IMC_CancelEvent(pRenEvent->event , pRenEvent->moduleID, pRenEvent->actionID);
		AUD_KDRV_PRINT("IMC_CancelEvent(%d) is (%x, %x, %x)!!!\n", renIndex, pRenEvent->event,	\
			pRenEvent->moduleID, pRenEvent->actionID);
	}

	//Clear a render resource.
	pRenInfo->ui32EventNum	= 0;
	pRenInfo->bClosed		= TRUE;
	pRenInfo->ui32AllocDev	= 0;

	AUD_REN_UNLOCK();

	AUD_KDRV_DEBUG("AUDIO_CloseRenderer : alloc %d.\n", allocDev);

	return RET_OK;
}

SINT32 AUDIO_GetRenderedStatus(UINT32 allocDev, LX_AUD_RENDERED_STATUS_T *pRenderedStatus)
{
	SINT32						renIndex = 0;
	ADEC_BUF_T					*pWriterStruct = NULL;
	LX_AUD_RENDERED_STATUS_T	*pRenderStatus = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	AUD_REN_BUF_LOCK();

	// get buffer
	pWriterStruct = _AUDIO_GetRendererWriteStructure(allocDev);
	if(pWriterStruct == NULL)
	{
		AUD_REN_BUF_UNLOCK();
		AUD_KDRV_DEBUG("_AUDIO_GetRendererWriteStructure(%d) failed !!!\n", allocDev);
		return RET_ERROR;
	}

	//Get a buffer info. from buffer interface
	pRenderStatus = &_gRenderInfo[renIndex].renderStatus;

	pRenderStatus->ui32MaxMemSize = pWriterStruct->get_max_size(pWriterStruct);
	pRenderStatus->ui32FreeMemSize = pWriterStruct->get_free_size(pWriterStruct);
	pRenderStatus->ui32MaxAuiSize = pWriterStruct->get_max_au(pWriterStruct);
	pRenderStatus->ui32FreeAuiSize = pWriterStruct->get_max_au(pWriterStruct) - pWriterStruct->get_au_cnt(pWriterStruct) - 1;

	memcpy(pRenderedStatus, pRenderStatus, sizeof(LX_AUD_RENDERED_STATUS_T));

	AUD_REN_BUF_UNLOCK();

	AUD_KDRV_PRINT("AUDIO_GetRenderedStatus(%d)\n", renIndex);

	return RET_OK;
}

/**
 * Sets the RTS mechanism.
 * Controls the RTS function manually.
 * @see
*/
static SINT32 AUDIO_EnableRTS(UINT32 allocDev, UINT32 bOnOff)
{
	SINT32	renIndex;

	AUD_KDRV_PRINT("set RTS to %s\n", (bOnOff == TRUE ? "On" : "Off"));

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	_gRenderInfo[renIndex].rtsParam.bRTSOnOff	= bOnOff;

	return RET_OK;
}

/**
 * Sets parameters for RTS.
 * Controls RTS module.
 * @see
*/
static SINT32 _AUDIO_SetRTSParam(UINT32 allocDev, LX_AUD_RENDER_RTS_PARAM_T *pRTSParam)
{
	SINT32	renIndex;
	LX_AUD_RENDER_RTS_PARAM_T		*pRenRtsParam = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenRtsParam = &_gRenderInfo[renIndex].rtsParam;

	// save the setting of RTS module
	pRenRtsParam->bRTSOnOff			= pRTSParam->bRTSOnOff;
	pRenRtsParam->opMode				= pRTSParam->opMode;
	pRenRtsParam->upperThreshold		= pRTSParam->upperThreshold;
	pRenRtsParam->recoverUpper			= pRTSParam->recoverUpper;
	pRenRtsParam->lowerThreshold		= pRTSParam->lowerThreshold;
	pRenRtsParam->recoverLower			= pRTSParam->recoverLower;
	pRenRtsParam->skippingThreshold		= pRTSParam->skippingThreshold;
	pRenRtsParam->recoverSkipping		= pRTSParam->recoverSkipping;
	pRenRtsParam->fastFreq				= pRTSParam->fastFreq;
	pRenRtsParam->normalFreq			= pRTSParam->normalFreq;
	pRenRtsParam->slowFreq				= pRTSParam->slowFreq;
	pRenRtsParam->mixFadeLen			= pRTSParam->mixFadeLen;
	pRenRtsParam->mixWaitLen			= pRTSParam->mixWaitLen;

	AUD_KDRV_PRINT("Set RTS: On/Off(%s), opMode(%u) upThres(%u), recoverU(%u), lowThres(%u), recoverL(%u), skipThres(%u), recoverS(%u), fast(%u), normal(%u), slow(%u), fadeLen(%u), waitLen(%u)\n",
								pRTSParam->bRTSOnOff == TRUE ? "On" : "Off",
								pRTSParam->opMode,
								pRTSParam->upperThreshold,
								pRTSParam->recoverUpper,
								pRTSParam->lowerThreshold,
								pRTSParam->recoverLower,
								pRTSParam->skippingThreshold,
								pRTSParam->recoverSkipping,
								pRTSParam->fastFreq,
								pRTSParam->normalFreq,
								pRTSParam->slowFreq,
								pRTSParam->mixFadeLen,
								pRTSParam->mixWaitLen );

	return RET_OK;
}

/**
 * Sets the Audio Trick Play mechanism.
 * Controls the Renderer Play Speed.
 * @see
*/
static SINT32 AUDIO_SetTrickPlayMode(UINT32 allocDev, UINT32 trickMode)
{
	SINT32				renIndex = 0;
	AUD_RENDER_INFO_T	*pRenInfo = NULL;

	ADEC_MODULE_ID		solaModule;
	ADEC_MODULE_ID		lipModule;
	SolaCmdSetRate		solaParam;
	LipsyncCmdSetRate	lipRate;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("render index(%d) is invalid!!!\n", renIndex);
		return RET_OK;
	}

	pRenInfo = &_gRenderInfo[renIndex];

	solaParam.OutSample	= 100;
	switch (trickMode)
	{
		case LX_AUD_TRICK_NORMAL_PLAY:
			solaParam.InSample	= 100;
			break;

		case LX_AUD_TRICK_SLOW_MOTION_0P25X:
			solaParam.InSample	= 25;
			break;

		case LX_AUD_TRICK_SLOW_MOTION_0P50X:
			solaParam.InSample	= 50;
			break;

		case LX_AUD_TRICK_SLOW_MOTION_0P80X:
			solaParam.InSample	= 80;
			break;

		case LX_AUD_TRICK_FAST_FOWARD_1P20X:
			solaParam.InSample	= 120;
			break;

		case LX_AUD_TRICK_FAST_FOWARD_1P50X:
			solaParam.InSample	= 150;
			break;

		case LX_AUD_TRICK_FAST_FOWARD_2P00X:
			solaParam.InSample	= 200;
			break;

		default:
			solaParam.InSample	= 100;
			break;
	}

	AUD_KDRV_PRINT("InSample[%d] OutSample[%d]\n", solaParam.InSample, solaParam.OutSample);

	// Get SOLA module
	solaModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_SOLA);
	if (solaModule != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(SOLA_CMD_SET_RATE, solaModule, sizeof(SolaCmdSetRate), &solaParam);
	}

	// Get LIP module
	lipModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if (lipModule != ADEC_MODULE_NOT_DEF)
	{
		lipRate.in	= solaParam.InSample;
		lipRate.out	= solaParam.OutSample;
		AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_RATE, lipModule, sizeof(LipsyncCmdSetRate), &lipRate);

		if((pRenInfo->trickMode == LX_AUD_TRICK_ONEFRAME_DECODE)
			&& (trickMode == LX_AUD_TRICK_NORMAL_PLAY || trickMode == LX_AUD_TRICK_NONE))
		{
			AUDIO_IMC_SendCmd(ADEC_CMD_START, lipModule);
		}
	}

	pRenInfo->trickMode = trickMode;

	AUD_KDRV_PRINT("trickMode(%d)\n", trickMode);

	return RET_OK;
}

/**
 * Get renderer delay.
 *
 * @param 	allocDev		[in] a allocated renderer device.
 * @param 	pDelay			[out] total delay ms.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
SINT32 AUDIO_GetRenderDelay(UINT32 allocDev, UINT32 *pDelay)
{
	SINT32					renIndex = 0;

	AUD_RENDER_INFO_T		*pRenInfo = NULL;
	LX_AUD_RENDER_PARAM_T	*pRenParam = NULL;
	ADEC_MODULE_ID			modInputCtrl = ADEC_MODULE_NOT_DEF;

	if(pDelay == NULL)
	{
		AUD_KDRV_ERROR("pDecodedStatus is NULL !!!\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("render index(%d) is invalid!!!\n", renIndex);
		*pDelay = 40;
		return RET_OK;
	}

	pRenInfo = &_gRenderInfo[renIndex];
	pRenParam = &(pRenInfo->renderParam);

	if(pRenParam->lipsyncType == LX_AUD_RENDER_LIPSYNC_ES)
	{
		AUD_KDRV_ERROR("ES is not supported!!!\n");
		return RET_ERROR;
	}

	// Find module
	modInputCtrl = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(modInputCtrl != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_GET_SYSTEMDELAY, modInputCtrl, 0, NULL);
		_AUDIO_RegisterRendererNoti(pRenInfo, (PFN_ImcNoti)_AUDIO_RenDelayCb, modInputCtrl, INPUTCTRL_EVT_SYSTEM_DELAY, IMC_ACTION_ONCE, 1);
	}

	OS_MsecSleep(30);

	if(pRenInfo->ui32Delay != 0 && pRenInfo->ui32Delay < 500)
	{
		// Normal
		*pDelay = pRenInfo->ui32Delay;
	}
	else
	{
		// Abnormal
		*pDelay = 40;
	}

	AUD_KDRV_PRINT("Device(%d) Delay(%d)\n", renIndex, *pDelay);

	return RET_OK;
}

/**
 * Get renderer delay.
 *
 * @param 	allocDev		[in] a allocated renderer device.
 * @param 	pPresentedPTS	[out] Presented PTS.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlDecoder().
 */
static SINT32 AUDIO_GetPresentedPTS(UINT32 allocDev, UINT32 *pPresentedPTS)
{
	AUD_DEV_INFO_T		*pDevInfo;
	LX_AUD_REG_INFO_T	regInfo;

	if(pPresentedPTS == NULL)
	{
		AUD_KDRV_ERROR("pPresentedPTS is NULL !!!\n");
		return RET_ERROR;
	}

	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_DEBUG("pDevInfo[%d] is NULL.\n", allocDev);
		return RET_ERROR;
	}

	regInfo.mode = LX_AUD_REG_READ;
	regInfo.readdata = 0xFFFFFFFF;

	if(pDevInfo->index == 0)
	{
		regInfo.addr = AUD_PRESENTED_PTS0_REG_ADDR;
	}
	else
	{
		regInfo.addr = AUD_PRESENTED_PTS1_REG_ADDR;
	}

	(void)KDRV_AUDIO_ReadAndWriteReg(&regInfo);

	*pPresentedPTS = regInfo.readdata;

	AUD_KDRV_PRINT("Device(%d) PresentedPTS(%d)\n", allocDev, *pPresentedPTS);

	return RET_OK;
}

/**
 * Get renderer started or not.
 *
 * @param 	allocDev		[in] a allocated decoder device.
 * @param 	pStarted		[out] started or not.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_GetStartInfo().
 */
SINT32 AUDIO_GetRenStartInfo(UINT32 allocDev, UINT32 *pStarted)
{
	SINT32					renIndex = 0;

	if(pStarted == NULL)
	{
		AUD_KDRV_ERROR("pStarted is NULL !!!\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("render index(%d) is invalid!!!\n", renIndex);
		return RET_OK;
	}

	if(_gRenderInfo[renIndex].status == AUD_STATUS_START)
		*pStarted = TRUE;
	else
		*pStarted = FALSE;

	return RET_OK;
}

/**
 * Sets channel volume for input control.
 * Controls the volume function.
 * @see
*/
SINT32 AUDIO_SetChannelVolume(UINT32 allocDev, LX_AUD_RENDER_CH_VOLUME_T *pChVol)
{
	SINT32						renIndex = 0;
	InputctrlCmdSetChGain			setInputCtrlChGain;
	ADEC_MODULE_ID				inputCtrlModule = ADEC_MODULE_INPUTCTRL_0;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_DEBUG_TMP("renIndex[%d] is not available.\n", renIndex);
		return RET_OK;
	}

	inputCtrlModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_INPUTCTRL);
	if(inputCtrlModule != ADEC_MODULE_NOT_DEF)
	{
		setInputCtrlChGain.LeftGain= pChVol->ui32LeftVol;
		setInputCtrlChGain.RightGain = pChVol->ui32RightVol;
		AUDIO_IMC_SendCmdParam(INPUTCTRL_CMD_SET_CH_GAIN, inputCtrlModule, sizeof(InputctrlCmdSetChGain), &setInputCtrlChGain);
	}

	_gRenderInfo[renIndex].ui32LeftVolume = pChVol->ui32LeftVol;
	_gRenderInfo[renIndex].ui32RightVolume = pChVol->ui32RightVol;

	AUD_KDRV_PRINT("left(%x), right(%x) \n", pChVol->ui32LeftVol, pChVol->ui32RightVol);

	return RET_OK;
}

void	KDRV_AUDIO_ResetRenderer(void)
{
	SINT32						retVal = RET_OK;
	UINT32						allocDev;
	SINT32						i = 0, j  = 0;

	AUD_EVENT_T	 				*pRenEvent = NULL;
	AUD_DEV_INFO_T				*pDevInfo = NULL;
	AUD_MOD_INFO_T				*pModInfo = NULL;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;

	for(i = 0; i < DEV_REN_NUM; i++)
	{
		pRenInfo = &_gRenderInfo[i];
		allocDev = pRenInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_REN0 || allocDev > LX_AUD_DEV_REN10)
		{
			AUD_KDRV_DEBUG("allocDev [%d]\n", allocDev);
			continue;
		}

		pRenInfo->bResetting = TRUE;

		AUDIO_StopRenderer(allocDev);
		AUDIO_FlushRenderer(allocDev);

		for(j = 0; j < pRenInfo->ui32ConnectNum; j++)
		{
			AUDIO_DisconnectRenderer(allocDev, pRenInfo->renderConnect[j].connectDev);
		}

		AUDIO_DisconnectAllModules(allocDev);
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
		}

		for(j = 0; j < pRenInfo->ui32EventNum; j++)
		{
			pRenEvent = &pRenInfo->renderEvent[j];
			AUDIO_IMC_CancelEvent(pRenEvent->event , pRenEvent->moduleID, pRenEvent->actionID);
		}

		pRenInfo->ui32EventNum = 0;
		pRenInfo->status = AUD_STATUS_NONE;
	}
}

void	KDRV_AUDIO_ResetRendererParam(void)
{
	UINT32						allocDev;
	SINT32						i = 0, j  = 0;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;
	LX_AUD_RENDER_PARAM_T		*pRenInfoParam = NULL;

	for(i = 0; i < DEV_REN_NUM; i++)
	{
		pRenInfo = &_gRenderInfo[i];
		allocDev = pRenInfo->ui32AllocDev;
		if(allocDev == 0)
			continue;

		pRenInfoParam = &(pRenInfo->renderParam);
		AUDIO_SetRendererParam(allocDev, pRenInfoParam);
		for(j = 0; j < pRenInfo->ui32ConnectNum; j++)
		{
			AUDIO_ConnectRenderer(allocDev, pRenInfo->renderConnect[j].connectDev);
		}

		AUDIO_SetInputCtrlDelay(allocDev, pRenInfo->ui32InputCtrlDelay);
		AUDIO_SetLipsync(allocDev, &(pRenInfo->renderLipsync));
		AUDIO_SetClockType(allocDev, pRenInfo->renderClockType);
		AUDIO_SetBasetime(allocDev, &(pRenInfo->renderBasetime));
		AUDIO_EnableLipsync(allocDev, pRenInfo->bLipsyncOnOff);
	}
}

void	KDRV_AUDIO_RestartRenderer(void)
{
	UINT32						allocDev;
	SINT32						i = 0;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;

	for(i = 0; i < DEV_REN_NUM; i++)
	{
		pRenInfo = &_gRenderInfo[i];
		allocDev = pRenInfo->ui32AllocDev;
		if(allocDev == 0)
			continue;

		AUDIO_StartRenderer(allocDev);

		pRenInfo->bResetting = FALSE;
	}
}

/**
 * Function for Get Renderer Information.
 * @see
*/
LX_AUD_RENDER_PARAM_T*	KDRV_AUDIO_GetRendererInfo(LX_AUD_DEV_T allocDev)
{
	SINT32	renIndex;
	LX_AUD_RENDER_PARAM_T *pRenParam = NULL;

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d.%d] is not available.\n", allocDev, renIndex);
		return NULL;
	}

	pRenParam = &(_gRenderInfo[renIndex].renderParam);

	return pRenParam;
}

/**
 * Redemand Decoded Notification.
 * @see
*/
void	KDRV_AUDIO_RedemandDecodedNofiRenderer(void)
{
	UINT32						tempCheckStartCount = 0;
	UINT32						allocDev;
	UINT32						i = 0, j = 0;
	UINT32						connectDev;
	ADEC_MODULE_ID				moduleId = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				tempModuleId = ADEC_MODULE_NOT_DEF;
	AUD_RENDER_INFO_T			*pRenInfo = NULL;

	for(i = 0; i < DEV_REN_NUM; i++)
	{
		pRenInfo = &_gRenderInfo[i];
		allocDev = pRenInfo->ui32AllocDev;
		if(allocDev == 0)
			continue;

		/* Set a decoder notification to get a decoder information. */
		if(pRenInfo->status != AUD_STATUS_START)
		{
			for(j = 0; j < AUD_RENDER_CONNECT_NUM; j++)
			{
				if(pRenInfo->renderConnect[j].devType == LX_AUD_DEV_TYPE_DEC)
				{
					connectDev = pRenInfo->renderConnect[j].connectDev;
					moduleId   = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
					if(moduleId == ADEC_MODULE_NOT_DEF)
					{
						AUD_KDRV_DEBUG("AUDIO_FindModuleType(%d) failed\n", connectDev);
						continue;
					}

					tempCheckStartCount = _gRenderCheckStartCount[i]++;

					if((tempCheckStartCount > COUNT_MIN_CHECK_START) && (tempCheckStartCount <= COUNT_MAX_CHECK_START))
					{
						if(moduleId != tempModuleId)
						{
							(void)AUDIO_IMC_SendCmd(ADEC_CMD_GET_DECINFO, moduleId);
							tempModuleId = moduleId;

							AUD_KDRV_DEBUG("Renderer : ADEC_CMD_GET_DECINFO.\n");
						}
					}
				}
			}
		}
	}

	return;
}

/**
 * Set nodelay mode.
 *
 * @param 	allocDev		[in] renderer device.
 * @param 	pParamNodelay	[in] nodelay param to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see
 */
static SINT32 AUDIO_SetNodelayParam(UINT32 allocDev,  LX_AUD_RENDER_PARAM_NODELAY_T *pParamNodelay)
{
	SINT32							renIndex = 0;
	LX_AUD_RENDER_PARAM_NODELAY_T	*pRenNoDelay = NULL;

	if(pParamNodelay == NULL)
	{
		AUD_KDRV_ERROR("pParamNodelay is NULL\n");
		return RET_ERROR;
	}

	renIndex = GET_REN_INDEX(allocDev);
	if((renIndex < 0) || (renIndex >= DEV_REN_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available.\n", renIndex);
		return RET_ERROR;
	}

	pRenNoDelay = &_gRenderInfo[renIndex].renderNodelay;

	pRenNoDelay->ui32OnOff		= pParamNodelay->ui32OnOff;
	pRenNoDelay->ui32UThreshold	= pParamNodelay->ui32UThreshold;
	pRenNoDelay->ui32LThreshold	= pParamNodelay->ui32LThreshold;

	return RET_OK;
}

/**
 * Sets Basetime Parameters for Clock.
 * Controls the AV Lipsync function.
 * @see
*/
SINT32 KDRV_AUDIO_SetBasetime(UINT32 index, LX_AUD_RENDER_PARAM_BASETIME_T *pParamBasetime)
{
	ADEC_MODULE_ID	allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;

	LipsyncCmdSetBase	basetime;

	if(pParamBasetime == NULL)
	{
		AUD_KDRV_ERROR("pParamBasetime is NULL\n");
		return RET_ERROR;
	}

	if(index >= DEV_DEC_NUM)
	{
		AUD_KDRV_ERROR("index is not available[%d].\n", index);
		return RET_ERROR;
	}

	basetime.clockbase	= (UINT32)(pParamBasetime->ui64ClockBaseTime & TIMESTAMP_MASK_NO_PCR);
	basetime.streambase	= (UINT32)(pParamBasetime->ui64StreamBaseTime & TIMESTAMP_MASK_NO_PCR);

	allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_0 + index;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BASE, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBase), &basetime);

	allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_10 + index;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_SET_BASE, allocMod_INPUTCTRL, sizeof(LipsyncCmdSetBase), &basetime);

	AUD_KDRV_NOTI("basetime - stream(%10d), clock(%10d)\n", basetime.streambase, basetime.clockbase);

	return RET_OK;
}

/**
 * Sets PlayToPts for One Frame Decoding.
 * Controls the AV Lipsync function.
 * @see
*/
SINT32 KDRV_AUDIO_SetPlayToPts(UINT32 index, UINT32 pts)
{
	SINT32	retVal = RET_OK;

	ADEC_MODULE_ID	allocMod_INPUTCTRL = ADEC_MODULE_NOT_DEF;
	AUD_DEV_INFO_T 	*pDevInfo = NULL;
	LX_AUD_DEV_T	dev = LX_AUD_DEV_NONE;
	LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

	LipsyncCmdPlayto	setPlayTo;

	if(index >= DEV_DEC_NUM)
	{
		if(_gRenderInfo[0].trickMode == LX_AUD_TRICK_ONEFRAME_DECODE)
		{
			dev = _gRenderInfo[0].ui32AllocDev;
		}
		else if(_gRenderInfo[1].trickMode == LX_AUD_TRICK_ONEFRAME_DECODE)
		{
			dev = _gRenderInfo[1].ui32AllocDev;
		}
		else
		{
			AUD_KDRV_ERROR("all dev are not one frame decode[%d].\n", index);
			return RET_ERROR;
		}

		pDevInfo = AUDIO_GetDevInfo(dev);
		if(pDevInfo == NULL)
		{
			AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d).\n", dev);
			return RET_ERROR;
		}

		index = pDevInfo->index;
	}

	setPlayTo.time_stamp = pts;

	allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_0 + index;
	AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_PLAYTO, allocMod_INPUTCTRL, sizeof(LipsyncCmdPlayto), &setPlayTo);

	// play to pts for es
	devIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
	devIndexCtx.ui32Index = index;
	devIndexCtx.devOutType = LX_AUD_DEV_OUT_ES;
	retVal = AUDIO_GetDevIndexCtx(&devIndexCtx);
	if(retVal == RET_OK)
	{
		pDevInfo = AUDIO_GetDevInfo(devIndexCtx.dev);
		if(pDevInfo != NULL)
		{
			if(pDevInfo->used == TRUE)
			{
				allocMod_INPUTCTRL = ADEC_MODULE_INPUTCTRL_10 + index;
				AUDIO_IMC_SendCmdParam(LIPSYNC_CMD_PLAYTO, allocMod_INPUTCTRL, sizeof(LipsyncCmdPlayto), &setPlayTo);
			}
		}
	}

	AUD_KDRV_NOTI("ADEC(%d) Play To PTS(%10d)\n", index, setPlayTo.time_stamp);

	return RET_OK;
}


EXPORT_SYMBOL(KDRV_AUDIO_SetBasetime);
EXPORT_SYMBOL(KDRV_AUDIO_SetPlayToPts);

