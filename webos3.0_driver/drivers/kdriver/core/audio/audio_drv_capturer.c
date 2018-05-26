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



/** @file	audio_kdrv_capturer.c
 *
 *  main driver implementation for  audio capturer device.
 *  audio capturer device will teach you how to make device driver with new platform.
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
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "audio_cfg.h"
#include "audio_drv.h"
#include "audio_buffer.h"
#include "audio_imc_func.h"

#include "buffer/adec_buf.h"

#include "audio_drv_master.h"
#include "audio_drv_manager.h"
#include "audio_drv_renderer.h"
#include "audio_drv_capturer.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
OS_SEM_T	_gpCapSema;		// capturer semaphore
BOOLEAN		_gpCapSemaInit = FALSE;

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define AUD_PCM_1SEC				(48000*2*2*1)		///< 1 second PCM audio data(48Khz * 16 bits * 2 channel * 1 second)
#define AUD_PCM_200MSEC				(AUD_PCM_1SEC/5)	///< 200 mseconds PCM audio data

#define AUD_CAP_LOCK_INIT()		OS_InitMutex(&_gpCapSema, OS_SEM_ATTR_DEFAULT)
#define AUD_CAP_LOCK()			OS_LockMutex(&_gpCapSema)
#define AUD_CAP_UNLOCK()		OS_UnlockMutex(&_gpCapSema)

#ifdef USE_BUF_LOCK
#define AUD_CAP_BUF_LOCK()		OS_LockMutex(&_gpCapSema)
#define AUD_CAP_BUF_UNLOCK()	OS_UnlockMutex(&_gpCapSema)
#else
#define AUD_CAP_BUF_LOCK()
#define AUD_CAP_BUF_UNLOCK()
#endif
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
AUD_CAPTURER_INFO_T _gCapturerInfo[DEV_CAP_NUM];

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static SINT32 AUDIO_StartCapturer(UINT32 allocDev);
static SINT32 AUDIO_StopCapturer(UINT32 allocDev);
static SINT32 AUDIO_FlushCapturer(UINT32 allocDev);
static SINT32 AUDIO_CloseCapture(UINT32 allocDev);
static SINT32 AUDIO_SetCaptureParam(UINT32 allocDev, LX_AUD_CAPTURE_PARAM_T *pCapParam);
static SINT32 AUDIO_GetCapturedInfo(UINT32 allocDev, LX_AUD_CAPTURE_CAPTURED_INFO_T *pCapInfo);
static SINT32 AUDIO_GetCapturedData(UINT32 allocDev, LX_AUD_CAPTURE_CAPTURED_DATA_T *pCapData);
static SINT32 AUDIO_ConnectCapturer(UINT32 allocDev, UINT32 connectDev);
static SINT32 AUDIO_DisconnectCapturer(UINT32 allocDev, UINT32 disconnectDev);
static SINT32 AUDIO_SetCaptureVolume(UINT32 allocDev, UINT32 volume);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
UINT32			_gCaptureCheckSum = 0;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * open handler for audio Capture device
 *
 */
SINT32	KDRV_AUDIO_OpenCapture(struct inode *inode, struct file *filp)
{
	ULONG		flags;
	SINT32		retVal = RET_OK;
	SINT32		capIndex = 0;
	UINT32		nMajor;
	UINT32		nMinor;

	LX_AUD_DEV_TYPE_T		devType = LX_AUD_DEV_TYPE_NONE;
	LX_AUD_DEV_T 			allocDev = LX_AUD_DEV_NONE;
	AUD_DEVICE_T			*pDev = NULL;

	if(_gpCapSemaInit == TRUE)
		AUD_CAP_LOCK();

	nMajor = imajor(inode);
	nMinor = iminor(inode);

	filp->private_data = kmalloc(sizeof(AUD_DEVICE_T), GFP_KERNEL);
	pDev = (AUD_DEVICE_T*)filp->private_data;

	/* Set device */
	devType = DEV_TYPE_BASE + nMinor;
	pDev->devType = devType;

	// to fix audio capturer not working
	_gCaptureCheckSum++;
	pDev->checksum = _gCaptureCheckSum;

	allocDev = AUDIO_AllocDev(devType);
	if(allocDev == LX_AUD_DEV_NONE)
	{
		kfree(filp->private_data);
		if(_gpCapSemaInit == TRUE)
			AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("devType[%d] is not available \n", devType);
		return RET_ERROR;
	}

	pDev->allocDev =  allocDev;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUDIO_FreeDev(allocDev);
		kfree(filp->private_data);
		if(_gpCapSemaInit == TRUE)
			AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	memset(&(_gCapturerInfo[capIndex]), 0x0, sizeof(AUD_CAPTURER_INFO_T));
	_gCapturerInfo[capIndex].ui32AllocDev = allocDev;
	_gCapturerInfo[capIndex].ui32Volume = 0xFFFFFFFF;
	_gCapturerInfo[capIndex].status = AUD_STATUS_NONE;
	_gCapturerInfo[capIndex].bClosed = FALSE;
	_gCapturerInfo[capIndex].ui32CheckSum = _gCaptureCheckSum;

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

	if(_gpCapSemaInit == TRUE)
		AUD_CAP_UNLOCK();

	//Initialize AUDIO Decoder semaphore
	if(_gpCapSemaInit == FALSE)
	{
		AUD_CAP_LOCK_INIT();
		_gpCapSemaInit = TRUE;
	}

	AUD_KDRV_PRINT("Open Capture device file  (%d:%d)\n", nMajor, nMinor);

	return retVal;
}

/**
 * close handler for audio Capture device
 *
 */
SINT32	KDRV_AUDIO_CloseCapture(struct inode *inode, struct file *filp)
{
	SINT32 retVal = 0;
	UINT32 nMajor;
	UINT32 nMinor;
	SINT32 capIndex = 0;

	AUD_DEVICE_T	*pDev = NULL;
	AUD_CAPTURER_INFO_T *pCapInfo = NULL;

	nMajor = imajor(inode);
	nMinor = iminor(inode);

	pDev = (AUD_DEVICE_T*)filp->private_data;
	if(pDev == NULL)
	{
		AUD_KDRV_ERROR("private_data is NULL\n");
		return RET_ERROR;
	}

	capIndex = GET_CAP_INDEX(pDev->allocDev);
	if ((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capture index(%d) is invalid!!!\n", capIndex);
		return RET_ERROR;
	}

	pCapInfo = &(_gCapturerInfo[capIndex]);

	// to fix audio capturer not working
	if(pCapInfo->ui32CheckSum == pDev->checksum)
	{
		retVal = AUDIO_CloseCapture(pDev->allocDev);
		if(retVal != RET_OK)
			AUD_KDRV_PRINT("AUDIO_CloseCapture() Failed or already closed.\n");
	}
	else
	{
		AUD_KDRV_INFO("checksums are different(%d.%d)\n",
			pCapInfo->ui32CheckSum, pDev->checksum);
	}

	kfree(filp->private_data);

	AUD_KDRV_PRINT("Close Capture device file  (%d:%d)\n", nMajor, nMinor);

	return retVal;
}

static SINT32	AUDIO_CloseCapture(UINT32 allocDev)
{
	SINT32							capIndex = 0;

	ADEC_MODULE_ID					allocMod_SRC = ADEC_MODULE_NOT_DEF;
	AUD_CAPTURER_INFO_T				*pCapInfo = NULL;
	PcmcvtCmdSetGain				srcSetGain;
	PcmcvtCmdSetOutSample			srcOutSample;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	pCapInfo = &_gCapturerInfo[capIndex];

	if(pCapInfo->bClosed == TRUE)
	{
		AUD_CAP_UNLOCK();
		AUD_KDRV_PRINT("capIndex[%d] is already closed.\n", capIndex);
		return RET_OK;
	}

	AUDIO_DisconnectAllModules(allocDev);

	// Initialize SRC
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
	{
		srcSetGain.GainEnable	= 0;
		srcSetGain.Gain			= 0xFFFFFFFF;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_GAIN, allocMod_SRC, sizeof(PcmcvtCmdSetGain), &srcSetGain);

		pCapInfo->ui32Volume = 0xFFFFFFFF;

		srcOutSample.samples = 256;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OUT_SAMPLE, allocMod_SRC, sizeof(PcmcvtCmdSetOutSample), &srcOutSample);
	}

	AUDIO_FreeDev(allocDev);

	pCapInfo->bClosed = TRUE;

	AUD_CAP_UNLOCK();

	AUD_KDRV_PRINT("alloc Device %d.\n", allocDev);

	return RET_OK;
}


/**
 * ioctl handler for audio Capture device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlCapture(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
long KDRV_AUDIO_IoctlCapture(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	SINT32				retVal = 0;
	SINT32				err = 0;
	AUD_DEVICE_T		*pDev = NULL;
	LX_AUD_DEV_T		allocDev;

	pDev = (AUD_DEVICE_T*)filp->private_data;

	allocDev = pDev->allocDev;

	/*
	* check if IOCTL command is valid or not.
	* - if magic value doesn't match, return error (-ENOTTY)
	* - if command is out of range, return error (-ENOTTY)
	*
	* note) -ENOTTY means "Inappropriate ioctl for device.
	*/
	if (_IOC_TYPE(cmd) != AUD_CAP_IOC_MAGIC)
	{
		DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > AUD_CAP_IOC_MAXNR)
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

	AUD_KDRV_TRACE("cmd = %08X (cmd_idx=%d)\n", cmd, _IOC_NR(cmd));

	switch(cmd)
	{
		case AUD_CAP_IO_START:
		{
			AUD_KDRV_TRACE("[AUD_CAP_IO_START] Called\n");
			AUDIO_StartCapturer(allocDev);
			retVal = RET_OK;
		}
		break;

		case AUD_CAP_IO_STOP:
		{
			AUD_KDRV_TRACE("[AUD_CAP_IO_STOP] Called\n");
			AUDIO_StopCapturer(allocDev);
			retVal = RET_OK;
		}
		break;

		case AUD_CAP_IO_FLUSH:
		{
			AUD_KDRV_TRACE("[AUD_CAP_IO_FLUSH] Called\n");
			AUDIO_FlushCapturer(allocDev);
			retVal = RET_OK;
		}
		break;

		case AUD_CAP_IOW_SET_VOLUME:
		{
			UINT32		capVolume;

			AUD_KDRV_TRACE("[AUD_CAP_IOW_SET_VOLUME] Called\n");

			if(copy_from_user(&capVolume, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			retVal = AUDIO_SetCaptureVolume(allocDev, capVolume);
		}
		break;

		case AUD_CAP_IOW_SET_PARAM:
		{
			LX_AUD_CAPTURE_PARAM_T		capParam;

			AUD_KDRV_TRACE("[AUD_CAP_IOW_SET_PARAM] Called\n");

			if(copy_from_user(&capParam, (void __user *)arg, sizeof(LX_AUD_CAPTURE_PARAM_T)))
				return RET_ERROR;

			retVal = AUDIO_SetCaptureParam(allocDev, &capParam);
		}
		break;

		case AUD_CAP_IOR_GET_CAPTURED_INFO:
		{
			LX_AUD_CAPTURE_CAPTURED_INFO_T	capInfo;

			AUD_KDRV_TRACE("[AUD_CAP_IOR_GET_CAPTURED_INFO] Called\n");

			retVal = AUDIO_GetCapturedInfo(allocDev, &capInfo);
			if(retVal != RET_OK)
				return RET_ERROR;

			if(copy_to_user((void *)arg, (void *)&capInfo, sizeof(LX_AUD_CAPTURE_CAPTURED_INFO_T)))
				return RET_ERROR;
		}
		break;

		case AUD_CAP_IOWR_GET_CAPTURED_DATA:
		{
			LX_AUD_CAPTURE_CAPTURED_DATA_T	capData;

			AUD_KDRV_TRACE("[AUD_CAP_IOWR_GET_CAPTURED_DATA] Called\n");

			if(copy_from_user(&capData, (void __user *)arg, sizeof(LX_AUD_CAPTURE_CAPTURED_DATA_T)))
				return RET_ERROR;

			retVal = AUDIO_GetCapturedData(allocDev, &capData);
			if(retVal != RET_OK)
				return RET_ERROR;

			if(copy_to_user((void *)arg, (void *)&capData, sizeof(LX_AUD_CAPTURE_CAPTURED_DATA_T)))
				return RET_ERROR;
		}
		break;

		case AUD_CAP_IOR_GET_KDRV_HANDLE:
		{
			AUD_KDRV_TRACE("[AUD_CAP_IOW_GET_KDRV_HANDLE] Called\n");

			if(copy_to_user((void *)arg, (void *)&(pDev->allocDev), sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_CAP_IOW_CONNECT:
		{
			UINT32									connectDev;

			AUD_KDRV_TRACE("[AUD_CAP_IOW_CONNECT] Called\n");

			if(copy_from_user(&connectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			retVal = AUDIO_ConnectCapturer(allocDev, connectDev);
		}
		break;

		case AUD_CAP_IOW_DISCONNECT:
		{
			UINT32									disconnectDev;

			AUD_KDRV_TRACE("[AUD_CAP_IOW_DISCONNECT] Called\n");

			if(copy_from_user(&disconnectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			retVal = AUDIO_DisconnectCapturer(allocDev, disconnectDev);
		}
		break;

		case AUD_CAP_IO_CLOSE_DEVICE:
		{
			AUD_KDRV_TRACE("[AUD_CAP_IO_CLOSE_DEVICE] Called\n");
			retVal = AUDIO_CloseCapture(pDev->allocDev);
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
 * Set SRC format.
 *
 * @param 	allocMod		[in] a allocated SRC module.
 * @param 	sampleFreq		[in] sampling frequency.
 * @param 	numOfChannel	[in] the number of channels.
 * @param 	bitPerSample	[in] the bit per a sample.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		AUDIO_ConnectCapturer()
 */
static void _AUDIO_SetSRCFormatForCatupre(UINT32 allocMod, LX_AUD_PCM_FORMAT_T	*pInPcmFormat, LX_AUD_PCM_FORMAT_T *pOutPcmFormat)
{
	PcmcvtCmdSetAllfmt		srcSetFmt;
	PcmcvtCmdSetOverProtect srcOverProtect;
	PcmcvtCmdSetOutSample		srcOutSample;
	PcmcvtCmdSetSrcRunningMode	runMode;

	srcSetFmt.InFs		= pInPcmFormat->samplingFreq;
	srcSetFmt.InCh		= pInPcmFormat->ui32NumOfChannel;
	srcSetFmt.InFormat	= pInPcmFormat->ui32BitPerSample;
	srcSetFmt.InEndian	= pInPcmFormat->endianType;
	srcSetFmt.InSigned	= pInPcmFormat->signedType;
	srcSetFmt.OutFs		= pOutPcmFormat->samplingFreq;
	srcSetFmt.OutCh		= pOutPcmFormat->ui32NumOfChannel;
	srcSetFmt.OutFormat	= pOutPcmFormat->ui32BitPerSample;
	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_ALLFMT, allocMod, sizeof(PcmcvtCmdSetAllfmt), &srcSetFmt);

	srcOverProtect.over_protect = USE_OVERFLOW_PROTECTION;
	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OVERPROTECT, allocMod, sizeof(PcmcvtCmdSetOverProtect), &srcOverProtect);

	if(pOutPcmFormat->samplingFreq == LX_AUD_SAMPLING_FREQ_16_KHZ)
	{
		srcOutSample.samples = 64;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OUT_SAMPLE, allocMod, sizeof(PcmcvtCmdSetOutSample), &srcOutSample);

		runMode.mode = 1;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_SRC_RUNNING_MODE, allocMod, sizeof(PcmcvtCmdSetSrcRunningMode), &runMode);
	}
	else if(pOutPcmFormat->samplingFreq == LX_AUD_SAMPLING_FREQ_11_025KHZ)
	{
		srcOutSample.samples = 32;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OUT_SAMPLE, allocMod, sizeof(PcmcvtCmdSetOutSample), &srcOutSample);
	}

	return;
}

/**
 * Start Capturer.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_RestartCapturer()
 */
static SINT32 AUDIO_StartCapturer(UINT32 allocDev)
{
	SINT32			capIndex = 0;
	UINT32			i;

	AUD_DEV_INFO_T			*pDevInfo = NULL;
	AUD_MOD_INFO_T			*pModInfo = NULL;
	AUD_CAPTURER_INFO_T		*pCapInfo = NULL;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	pCapInfo = &_gCapturerInfo[capIndex];

	//get alloc module info in given device node
	pDevInfo = AUDIO_GetDevInfo(allocDev);

	for(i = 0; i < pDevInfo->numMod; i++)
	{
		if(pDevInfo->pModInfo[i] != NULL)
		{
			pModInfo = pDevInfo->pModInfo[i];
			if(pModInfo->modType != ADEC_MOD_TYPE_EXT_BUF)
			{
				AUDIO_IMC_SendCmd(ADEC_CMD_INIT, pModInfo->mod);
				AUDIO_IMC_SendCmd(ADEC_CMD_START, pModInfo->mod);
			}
		}
	}

	pCapInfo->status = AUD_STATUS_START;

	AUD_CAP_UNLOCK();

	return RET_OK;
}

/**
 * Stop Capturer.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_ResetCapturer()
 */
static SINT32 AUDIO_StopCapturer(UINT32 allocDev)
{
	UINT32			i;
	SINT32			capIndex = 0;

	AUD_DEV_INFO_T			*pDevInfo = NULL;
	AUD_MOD_INFO_T			*pModInfo = NULL;
	AUD_CAPTURER_INFO_T		*pCapInfo = NULL;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	pCapInfo = &_gCapturerInfo[capIndex];

	//get alloc module info in given device node
	pDevInfo = AUDIO_GetDevInfo(allocDev);

	for(i = 0; i < pDevInfo->numMod; i++)
	{
		if(pDevInfo->pModInfo[i] != NULL)
		{
			pModInfo = pDevInfo->pModInfo[i];
			if(pModInfo->modType != ADEC_MOD_TYPE_EXT_BUF)
				AUDIO_IMC_SendCmd(ADEC_CMD_STOP, pModInfo->mod);
		}
	}

	pCapInfo->status = AUD_STATUS_STOP;

	AUD_CAP_UNLOCK();

	return RET_OK;
}

/**
 * Flush Capturer.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_ResetCapturer()
 */
static SINT32 AUDIO_FlushCapturer(UINT32 allocDev)
{
	UINT32			i;
	SINT32			capIndex = 0;

	AUD_DEV_INFO_T			*pDevInfo = NULL;
	AUD_MOD_INFO_T			*pModInfo = NULL;

	ADEC_BUF_T  			*pCapReader = NULL;
	ADEC_MODULE_ID			bufModule = ADEC_MODULE_NOT_DEF;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("renIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	//get alloc module info in given device node
	pDevInfo = AUDIO_GetDevInfo(allocDev);

	for(i = 0; i < pDevInfo->numMod; i++)
	{
		if(pDevInfo->pModInfo[i] != NULL)
		{
			pModInfo = pDevInfo->pModInfo[i];
			if(pModInfo->modType != ADEC_MOD_TYPE_EXT_BUF)
			{
				ImcCmdFlushParam 		flushParam;

				memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
				flushParam.num_of_port = 1;
				flushParam.port_list[0] = MOD_IN_PORT(0);

				AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, pModInfo->mod, sizeof(ImcCmdFlushParam), &flushParam);
			}
		}
	}

	// Buffer flush
	bufModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
	if(bufModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
		goto flush_error;
	}

	pCapReader = AUDIO_GetBuffer(bufModule, IN_PORT);
	if(pCapReader == NULL)
	{
		AUD_KDRV_ERROR("pCapReader is NULL.\n");
		goto flush_error;
	}
	AUD_CAP_UNLOCK();

	AUD_CAP_BUF_LOCK();
	pCapReader->init(pCapReader);
	AUD_CAP_BUF_UNLOCK();

	return RET_OK;

flush_error:
	AUD_CAP_UNLOCK();
	return RET_ERROR;
}

/**
 * Sets volume for capturer.
 * Controls the volume function.
 * @see
*/
static SINT32 AUDIO_SetCaptureVolume(UINT32 allocDev, UINT32 volume)
{
	SINT32						capIndex = 0;

	ADEC_MODULE_ID				allocMod_SRC = ADEC_MODULE_NOT_DEF;
	AUD_CAPTURER_INFO_T			*pCapInfo;

	PcmcvtCmdSetGain			srcSetGain;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	pCapInfo = &_gCapturerInfo[capIndex];

	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF )
	{
		srcSetGain.GainEnable	= 1;
		srcSetGain.Gain			= volume;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_GAIN, allocMod_SRC, sizeof(PcmcvtCmdSetGain), &srcSetGain);
	}

	pCapInfo->ui32Volume = volume;

	AUD_KDRV_PRINT("vol %x \n", volume);
	return RET_OK;
}


/**
 * Connect other device to Capturer.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @param 	connectDev		[in] Device to connect.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlCapture(), KDRV_AUDIO_ResetCapturerParam().
 */
static SINT32 AUDIO_ConnectCapturer(UINT32 allocDev, UINT32 connectDev)
{
	SINT32 	capIndex = 0;
	SINT32	retVal = RET_OK;

	AUD_DEV_INFO_T			*pCapDevInfo;
	AUD_DEV_INFO_T			*pConnectDevInfo;
	LX_AUD_CAPTURE_PARAM_T	*pCapInfoParam;
	ADEC_MODULE_ID 			outMod = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			inMod = ADEC_MODULE_NOT_DEF;

	BypassCmdSetMode		bypassMode;

	/* Get a Capturer Index */
	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available.\n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	pCapInfoParam = &_gCapturerInfo[capIndex].capParam;

	pCapDevInfo = AUDIO_GetDevInfo(allocDev);
	pConnectDevInfo = AUDIO_GetDevInfo(connectDev);

	if(pCapDevInfo == NULL || pConnectDevInfo == NULL)
	{
		AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("Check connectDevInfo[%p] capDevInfo[%p].\n", pConnectDevInfo, pConnectDevInfo);
		return RET_ERROR;
	}

	if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_ENC)
	{
		outMod = AUDIO_FindTailModule(connectDev);
		if(outMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindTailModule[%d] failed. \n", connectDev);
			return RET_ERROR;
		}

		inMod = AUDIO_FindHeadModule(allocDev);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindTailModule[%d] failed. \n", allocDev);
			return RET_ERROR;
		}

		retVal = AUDIO_ConnectModules(outMod, inMod, AUD_BUFFER_TYPE_CAPB);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_ConnecModules(%d.%d) failed(%d)!!!\n", outMod, inMod, retVal);
			return RET_ERROR;
		}
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		if(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_MIXER)
		{
			//mix end capture
			outMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_MIXER);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed. \n", ADEC_MOD_TYPE_MIXER);
				return RET_ERROR;
			}
		}
		else // if(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_SE)
		{
			//se end capture
			outMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_SE);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed. \n", ADEC_MOD_TYPE_SE);
				return RET_ERROR;
			}
		}

		// get DSP1 PCMCVT 10, 11 for capture.
		inMod = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP1, ADEC_MODULE_PCMCVT_P10);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			inMod = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP1, ADEC_MODULE_PCMCVT_P11);
			if(inMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed. \n", ADEC_MOD_TYPE_PCMCVT);
				return RET_ERROR;
			}
		}

		retVal = AUDIO_InsertModuleFirst(allocDev, inMod, AUD_BUFFER_TYPE_CAPB);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d.%d) failed(%d)!!!\n", allocDev, inMod, retVal);
			return RET_ERROR;
		}

		//Set PCM format to SRC
		_AUDIO_SetSRCFormatForCatupre(inMod,
									&(pCapInfoParam->inPcmFormat),
									&(pCapInfoParam->outPcmFormat));

		retVal = AUDIO_ConnectModules(outMod, inMod, AUD_BUFFER_TYPE_NONE);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_ConnecModules(%d) failed!!!\n", retVal);
			return RET_ERROR;
		}

		// SRC INIT & START
		AUDIO_IMC_SendCmd(ADEC_CMD_INIT, inMod);
		AUDIO_IMC_SendCmd(ADEC_CMD_START, inMod);
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		if (pCapInfoParam->capInput  == LX_AUD_CAPTURE_INPUT_DECODER_ES
		  &&pCapInfoParam->codecType == LX_AUD_CODEC_AAC_BYPASS)
		{
			//BYPASS_ES_DSP0 module
			outMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_BYPASS);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed. \n", connectDev);
				return RET_ERROR;
			}

			//cap module
			inMod = AUDIO_FindHeadModule(allocDev);
			if(inMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindHeadModule[%d] failed. \n", allocDev);
				return RET_ERROR;
			}

			retVal = AUDIO_ConnectModules(outMod, inMod, AUD_BUFFER_TYPE_CAPB);
			if(retVal < 0)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnecModules(%d,%d) failed(%d)!!!\n", outMod, inMod, retVal);
				return RET_ERROR;
			}
		}
		else
		{
			outMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType[%d] failed. \n", connectDev);
				return RET_ERROR;
			}

			if((pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_DECODER_ES) ||
			   (pCapInfoParam->outPcmFormat.samplingFreq == LX_AUD_SAMPLING_FREQ_NONE))
			{
				// get Bypass DSP0 in DEC device
				inMod = AUDIO_AllocModule(ADEC_MOD_TYPE_BYPASS, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
				if(inMod == ADEC_MODULE_NOT_DEF)
				{
					AUD_CAP_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed.\n", ADEC_MOD_TYPE_BYPASS);
					return RET_ERROR;
				}

				// Set a default lip sync on and buffer overflow disable mode
				bypassMode.mode = AU_BASED_MODE ;
				bypassMode.over_protect	 = NO_OVERFLOW_PROTECTION;
				AUDIO_IMC_SendCmdParam(BYPASS_CMD_SET_MODE, inMod, sizeof(BypassCmdSetMode), &bypassMode);
			}
			else
			{
				// get SRC DSP0 in DEC device
				inMod = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
				if(inMod == ADEC_MODULE_NOT_DEF)
				{
					AUD_CAP_UNLOCK();
					AUD_KDRV_ERROR("AUDIO_AllocModule[%d] failed.\n", ADEC_MOD_TYPE_PCMCVT);
					return RET_ERROR;
				}

				//Send a IMC command - Set PCM format
				_AUDIO_SetSRCFormatForCatupre(inMod,
											&(pCapInfoParam->inPcmFormat),
											&(pCapInfoParam->outPcmFormat));
			}

			retVal = AUDIO_InsertModuleFirst(allocDev, inMod, AUD_BUFFER_TYPE_CAPB);
			if(retVal < 0)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d,%d) failed(%d)!!!\n", allocDev, inMod, retVal);
				return RET_ERROR;
			}

			retVal = AUDIO_ConnectModules(outMod, inMod, AUD_BUFFER_TYPE_NONE);
			if(retVal < 0)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_ConnecModules(%d,%d) failed(%d)!!!\n", allocDev, inMod, retVal);
				return RET_ERROR;
			}
		}
	}

	_gCapturerInfo[capIndex].captureConnect.connectDev = connectDev;

	AUD_CAP_UNLOCK();

	AUD_KDRV_PRINT("AUDIO_ConnectCapturer : A%d, C%d\n", allocDev, connectDev);

	return retVal;
}

/**
 * Disconnect Capture device From Other Device.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @param 	connectDev		[in] Device to connect.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlCapture(), KDRV_AUDIO_ResetCapturer().
 */
static SINT32 AUDIO_DisconnectCapturer(UINT32 allocDev, UINT32 disconnectDev)
{
	SINT32	capIndex = 0;
	SINT32	retVal = RET_OK;

	AUD_DEV_INFO_T	*pCapDevInfo;
	AUD_DEV_INFO_T	*pDisconnectDevInfo;
	ADEC_MODULE_ID	outMod = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID	inMod = ADEC_MODULE_NOT_DEF;
	LX_AUD_CAPTURE_PARAM_T 	 *pCapInfoParam;

	/* Get a Capturer Index */
	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	pCapInfoParam = &_gCapturerInfo[capIndex].capParam;

	pCapDevInfo = AUDIO_GetDevInfo(allocDev);
	pDisconnectDevInfo = AUDIO_GetDevInfo(disconnectDev);

	if(pCapDevInfo == NULL || pDisconnectDevInfo == NULL)
	{
		AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("Check pCapDevInfo[%p] pConnectDevInfo[%p] \n", pCapDevInfo, pDisconnectDevInfo);
		return RET_ERROR;
	}

	if(pDisconnectDevInfo->devType == LX_AUD_DEV_TYPE_ENC)
	{
		outMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_ENCODER);
		if(outMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed.\n", disconnectDev);
			return RET_ERROR;
		}

		inMod =	AUDIO_FindHeadModule(allocDev);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindHeadModule(%d) failed.\n", allocDev);
			return RET_ERROR;
		}
	}
	else if(pDisconnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		if(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_MIXER)
		{
			//mix end capture
			outMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_MIXER);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed.\n", disconnectDev);
				return RET_ERROR;
			}
		}
		else	//if(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_MIXER)
		{
			outMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_SE);
			if(outMod == ADEC_MODULE_NOT_DEF)
			{
				AUD_CAP_UNLOCK();
				AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed.\n", disconnectDev);
				return RET_ERROR;
			}
		}

		//bypass_dsp1
		inMod = AUDIO_FindHeadModule(allocDev);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindHeadModule(%d) failed.\n", allocDev);
			return RET_ERROR;
		}
	}
	else if(pDisconnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC &&
		pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_DECODER_ES &&
		pCapInfoParam->codecType == LX_AUD_CODEC_AAC_BYPASS )
	{
		//BYPASS_ES_DSP0 module
		outMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_BYPASS);
		if(outMod != ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed.\n", disconnectDev);
			return RET_ERROR;
		}

		//cap module
		inMod = AUDIO_FindHeadModule(allocDev);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindHeadModule(%d) failed.\n", allocDev);
			return RET_ERROR;
		}
	}
	else
	{
		outMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_DECODER);
		if(outMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d) failed.\n", disconnectDev);
			return RET_ERROR;
		}

		inMod = AUDIO_FindHeadModule(allocDev);
		if(inMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindHeadModule(%d) failed.\n", allocDev);
			return RET_ERROR;
		}
	}

	retVal = AUDIO_DisconnectModules(outMod, inMod);
	if(retVal < 0)
	{
		AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d.%d) failed(%d)!!!\n", allocDev, inMod, retVal);
		return RET_ERROR;
	}

	AUD_CAP_UNLOCK();

	return retVal;
}


/**
 * Set Capturer Parameter.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @param 	pcapParam		[in] param to set.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlCapture(), KDRV_AUDIO_ResetCapturerParam().
 */
static SINT32 AUDIO_SetCaptureParam(UINT32 allocDev, LX_AUD_CAPTURE_PARAM_T *pCapParam)
{
	SINT32	capIndex = 0;
	SINT32	retVal = 0;

	LX_AUD_CAPTURE_PARAM_T	*pCapInfoParam;
	AUD_DEV_INFO_T			*pDevInfo = NULL;
	AUD_MOD_INFO_T 			*pModInfo = NULL;
	ADEC_MODULE_ID			modBuf = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			modI2sIn = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			modPcmCvt = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			modId = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID			modBypass = ADEC_MODULE_NOT_DEF;
	I2sCmdSetFmt			setI2sFmt;
	I2sCmdSetChMask			setChMask;
	BypassCmdSetMode		bypassMode;

	LX_AUD_SAMPLING_FREQ_T	setSpeakerFreq = LX_AUD_SAMPLING_FREQ_48_KHZ;

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_LOCK();

	//Get a speaker sampling frequency.
	setSpeakerFreq = MIXED_IP_AUDIO_GetSpeakerFreq();

	//Update inout sampling frequency from speaker output frequency.
	if(pCapParam->capInput == LX_AUD_CAPTURE_INPUT_I2S	\
	 ||pCapParam->capInput == LX_AUD_CAPTURE_INPUT_SE)
	{
		pCapParam->inPcmFormat.samplingFreq = setSpeakerFreq;
		AUD_KDRV_DEBUG("capInput[%d], samplingFreq = %d\n", pCapParam->capInput, pCapParam->inPcmFormat.samplingFreq);
	}

	pCapInfoParam = &_gCapturerInfo[capIndex].capParam;

	modBuf = AUDIO_AllocModule(ADEC_MOD_TYPE_EXT_BUF, ADEC_CORE_ARM, ADEC_MODULE_NOT_DEF);
	if(modBuf == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("ModType[%d] is not available \n", ADEC_MOD_TYPE_EXT_BUF);
		return RET_ERROR;
	}

	retVal = AUDIO_InsertModuleLast(allocDev, modBuf, AUD_BUFFER_TYPE_CAPB);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_InsertModuleLast(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	memcpy(pCapInfoParam, pCapParam, sizeof(LX_AUD_CAPTURE_PARAM_T));

	if(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_I2S)
	{
		modBuf = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
		if(modBuf == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
			return RET_ERROR;
		}

		modI2sIn = AUDIO_AllocModule(ADEC_MOD_TYPE_SOURCE, ADEC_CORE_DSP0, ADEC_MODULE_I2S_IN);
		if(modI2sIn == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AllocModule(%d.%d) failed.\n", allocDev, ADEC_MODULE_I2S_IN);
			return RET_ERROR;
		}

		// set format of EXT I2S
		setI2sFmt.format 			  	= 0;
		setI2sFmt.resolution 		  	= 3;
		setI2sFmt.sck_polarity_control 	= 0;
		setI2sFmt.lrck_polarity_control	= 0;
		AUDIO_IMC_SendCmdParam(I2S_CMD_SET_FMT, modI2sIn, sizeof(I2sCmdSetFmt), &setI2sFmt);

		// set Input I2S Port
		if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))	//A0
		{
			setChMask.ch_mask	= pCapInfoParam->ui32InputI2SMask;
		}
		else
		{
			setChMask.ch_mask	= 1;
			AUD_KDRV_NOTI("ch_mask(%d) is invalid in M14.\n", pCapInfoParam->ui32InputI2SMask);
		}
		AUDIO_IMC_SendCmdParam(I2S_CMD_SET_CH_MASK, modI2sIn, sizeof(I2sCmdSetChMask), &setChMask);

		modPcmCvt = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
		if(modPcmCvt == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AllocModule(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_PCMCVT);
			return RET_ERROR;
		}

		//Send a IMC command - Set PCM format
		_AUDIO_SetSRCFormatForCatupre(modPcmCvt,
									&(pCapInfoParam->inPcmFormat),
									&(pCapInfoParam->outPcmFormat));

		retVal = AUDIO_InsertModuleFirst(allocDev, modPcmCvt, AUD_BUFFER_TYPE_CAPB);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d,%d) failed(%d)!!!\n", allocDev, modPcmCvt, retVal);
			return RET_ERROR;
		}

		retVal = AUDIO_ConnectModules(modI2sIn, modPcmCvt, AUD_BUFFER_TYPE_NONE);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_ConnecModules(%d,%d) failed(%d)!!!\n", modI2sIn, modPcmCvt, retVal);
			return RET_ERROR;
		}

		retVal = AUDIO_AddModule(allocDev, modI2sIn);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", modI2sIn, retVal);
			return RET_ERROR;
		}
	}
	else if(pCapInfoParam->capInput > LX_AUD_CAPTURE_INPUT_I2S)
	{
		modBuf = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
		if(modBuf == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
			return RET_ERROR;
		}

		modId = pCapInfoParam->capInput & 0xFFF;
		pModInfo = AUDIO_GetModuleInfo(modId);
		if(pModInfo == NULL)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_GetModuleInfo(%d) failed.\n", modId);
			return RET_ERROR;
		}

		modBypass = AUDIO_AllocModule(ADEC_MOD_TYPE_BYPASS, pModInfo->coreType, ADEC_MODULE_NOT_DEF);
		if(modBypass == ADEC_MODULE_NOT_DEF)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_AllocModule(%d.%d.%d) failed.\n", ADEC_MOD_TYPE_BYPASS, modId, pModInfo->coreType);
			return RET_ERROR;
		}

		// Set a default lip sync on and buffer overflow disable mode
		bypassMode.mode = AU_BASED_MODE ;
		bypassMode.over_protect	 = NO_OVERFLOW_PROTECTION;
		AUDIO_IMC_SendCmdParam(BYPASS_CMD_SET_MODE, modBypass, sizeof(BypassCmdSetMode), &bypassMode);

		retVal = AUDIO_InsertModuleFirst(allocDev, modBypass, AUD_BUFFER_TYPE_CAPB);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_InsertModuleFirst(%d,%d) failed(%d)!!!\n", allocDev, modBypass, retVal);
			return RET_ERROR;
		}

		retVal = AUDIO_ConnectModules(modId, modBypass, AUD_BUFFER_TYPE_NONE);
		if(retVal < 0)
		{
			AUD_CAP_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_ConnecModules(%d,%d) failed(%d)!!!\n", modId, modBypass, retVal);
			return RET_ERROR;
		}
	}

	// set device output type
	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_CAP_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
		return RET_ERROR;
	}

	if((pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_ENCODER) ||
		(pCapInfoParam->capInput == LX_AUD_CAPTURE_INPUT_DECODER_ES))
	{
		pDevInfo->devOutType = LX_AUD_DEV_OUT_ES;
	}
	else
	{
		pDevInfo->devOutType = LX_AUD_DEV_OUT_PCM;
	}

	AUD_CAP_UNLOCK();

	return RET_OK;
}

/**
 * Get Captured information.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @param 	pCapInfo		[in] captured information.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlCapture(), KDRV_AUDIO_ResetCapturerParam().
 */
static SINT32 AUDIO_GetCapturedInfo(UINT32 allocDev, LX_AUD_CAPTURE_CAPTURED_INFO_T *pCapInfo)
{
	SINT32			retVal = RET_OK;
	SINT32			usedSize = 0;
	SINT32			freeSize = 0;
	SINT32			auCount  = 0;
	SINT32 			auOffset = 0;
	SINT32			capIndex = 0;

	ADEC_BUF_T  	*pCapReader = NULL;
	LX_AUD_AU_INFO_T  auInfo;
	ADEC_MODULE_ID	bufModule = ADEC_MODULE_NOT_DEF;

	//Sanity check
	if(pCapInfo == NULL)
	{
		AUD_KDRV_ERROR("pCapInfo is NULL.\n");
		return RET_ERROR;
	}

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_BUF_LOCK();

	//Set a default value
	pCapInfo->ui32WriteIndex = 0;
	pCapInfo->ui32SizeOfAu = 0;
	pCapInfo->ui32RemainIndex = 0;
	pCapInfo->ui32UsedSize = 0;
	pCapInfo->ui32FreeSize = 0;

	//Assign a PCM buffer
	bufModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
	if(bufModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
		return RET_ERROR;
	}

	pCapReader = AUDIO_GetBuffer(bufModule, IN_PORT);
	if(pCapReader == NULL)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("pCapReader is NULL.\n");
		return RET_ERROR;
	}

	//Read a used PCM buffer size
	usedSize = pCapReader->get_used_size(pCapReader);
	freeSize = pCapReader->get_free_size(pCapReader);

	//Check a current buffer size
	if(usedSize <= 0)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_PRINT("used size = %d, free size = %d\n", usedSize, freeSize);
		return RET_OK;
	}

	//Read a ES AUI Count info.
	auCount = pCapReader->get_au_cnt(pCapReader);
	if(auCount < 1)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_PRINT("get_au_cnt(auCount = %d)\n", auCount);
		return RET_OK;
	}

	//Read a current ES AUI info.
	memset(&auInfo, 0, sizeof(ADEC_AU_INFO_T));
	retVal = pCapReader->get_au_info(0, &auOffset, (ADEC_AU_INFO_T *)&auInfo, pCapReader);
	if(retVal != RET_OK)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
		return retVal;
	}

	if(auInfo.ui64Size == 0 && auInfo.ui32LastAu == 1)
	{
		if (auCount > 1)
		{
			retVal = pCapReader->get_au_info(1, &auOffset, (ADEC_AU_INFO_T *)&auInfo, pCapReader);
			if(retVal != RET_OK)
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
				return retVal;
			}
		}
		else
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_PRINT("get_au_cnt(auCount = %d)\n", auCount);
			return RET_OK;
		}
	}

	if(((UINT32)auInfo.ui64Size + auOffset) == 0)
	{
		if (auCount > 1)
		{
			retVal = pCapReader->get_au_info(1, &auOffset, (ADEC_AU_INFO_T *)&auInfo, pCapReader);
			if(retVal != RET_OK)
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
				return retVal;
			}
		}
		else
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_PRINT("get_au_cnt(auCount = %d)\n", auCount);
			return RET_OK;
		}
	}

	//Check a current offset for AAC encoder
	if(_gCapturerInfo[capIndex].capParam.codecType == LX_AUD_CODEC_AAC_ENC)
	{
		if(auInfo.ui64Size + auOffset <= 0)
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_ERROR("AAC_ENC auInfo.size = %llu, auOffset = %d\n", 	\
							auInfo.ui64Size, auOffset);
			return RET_ERROR;
		}
	}

	//Set a current AU index and size of AU
	pCapInfo->ui32WriteIndex  = (UINT32)auInfo.ui32Index;
	pCapInfo->ui32SizeOfAu    = (UINT32)(auInfo.ui64Size + auOffset);
	pCapInfo->ui32RemainIndex = (UINT32)auCount;
	pCapInfo->ui32UsedSize    = usedSize;
	pCapInfo->ui32FreeSize    = freeSize;

	AUD_CAP_BUF_UNLOCK();
	AUD_KDRV_PRINT("wr idx = %d, size = %d\n", pCapInfo->ui32WriteIndex, pCapInfo->ui32SizeOfAu);
	return RET_OK;
}

/**
 * Get Captured data.
 *
 * @param 	allocDev		[in] a allocated capture device.
 * @param 	pCapData		[in] captured data.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlCapture().
 */
static SINT32 AUDIO_GetCapturedData(UINT32 allocDev, LX_AUD_CAPTURE_CAPTURED_DATA_T *pCapData)
{
 	SINT32			retVal = RET_OK;
	SINT32			usedSize = 0;
	SINT32			freeSize = 0;
	SINT32 			auOffset = 0;
	SINT32 			auReadSize = 0;
	SINT32			auCount  = 0;
	SINT32			capIndex = 0;
	UINT8			*pDataBuf = NULL;

	ADEC_BUF_T		*pCapReader  = NULL;
	ADEC_MODULE_ID	bufModule = ADEC_MODULE_NOT_DEF;

	//Sanity check
	if((pCapData->pBuffer == NULL) && (pCapData->ui32BufSize != 0))
	{
		AUD_KDRV_ERROR("error!!!(pBuffer = %p, bufSize = %d)\n", pCapData->pBuffer, pCapData->ui32BufSize);
		return RET_ERROR;
	}

	capIndex = GET_CAP_INDEX(allocDev);
	if((capIndex < 0) || (capIndex >= DEV_CAP_NUM))
	{
		AUD_KDRV_ERROR("capIndex[%d] is not available. \n", capIndex);
		return RET_ERROR;
	}

	AUD_CAP_BUF_LOCK();

	bufModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_EXT_BUF);
	if(bufModule == ADEC_MODULE_NOT_DEF)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_FindModuleType(%d.%d) failed.\n", allocDev, ADEC_MOD_TYPE_EXT_BUF);
		return RET_ERROR;
	}

	pCapReader = AUDIO_GetBuffer(bufModule, IN_PORT);
	if(pCapReader == NULL)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("pCapReader is NULL.\n");
		return RET_ERROR;
	}

	//flush buffer
	if((pCapData->pBuffer == NULL) || (pCapData->ui32BufSize == 0))
	{
		//Read a used CAP(ES/PCM) buffer size
		usedSize = pCapReader->get_used_size(pCapReader);

		//Check a remain data size.
		if(usedSize > AUD_PCM_200MSEC)
		{
			//Flush a current AUI and AU data
			retVal	= pCapReader->flush_data(usedSize - AUD_PCM_200MSEC, pCapReader);
			if(retVal != RET_OK)
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_ERROR("flush_data(ret = %d)\n", retVal);
				return RET_ERROR;
			}
			else
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_PRINT("flush_data(ret = %d)\n", retVal);
				return RET_OK;
			}
		}
		else
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_DEBUG("flush_data(used = %d)(ret = %d)\n", usedSize, retVal);
			return RET_OK;
		}
	}

	//Set a default value
	memset((void *)&pCapData->auInfo, 0x0, sizeof(LX_AUD_AU_INFO_T));
	pCapData->ui32RemainIndex = 0;
	pCapData->ui32UsedSize = 0;
	pCapData->ui32FreeSize = 0;

	//Read a AUI Count info.
	auCount = pCapReader->get_au_cnt(pCapReader);
	if (auCount > 0)
	{
		pCapData->ui32RemainIndex = (UINT32)auCount;
	}
	else
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_PRINT("No AU Data\n");
		return RET_OK;
	}

	//Read a current AUI info.
	retVal = pCapReader->get_au_info(0, &auOffset, (ADEC_AU_INFO_T *)&pCapData->auInfo, pCapReader);
	if(retVal != RET_OK)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
		return retVal;
	}

	if(pCapData->auInfo.ui64Size == 0 && pCapData->auInfo.ui32LastAu == 1)
	{
		if (auCount > 1)
		{
			retVal = pCapReader->get_au_info(1, &auOffset, (ADEC_AU_INFO_T *)&pCapData->auInfo, pCapReader);
			if(retVal != RET_OK)
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
				return retVal;
			}
		}
		else
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_PRINT("AU count = %d\n", auCount);
			return RET_OK;
		}
	}

	if(((UINT32)pCapData->auInfo.ui64Size + auOffset) == 0)
	{
		if (auCount > 1)
		{
			retVal = pCapReader->get_au_info(1, &auOffset, (ADEC_AU_INFO_T *)&pCapData->auInfo, pCapReader);
			if(retVal != RET_OK)
			{
				AUD_CAP_BUF_UNLOCK();
				AUD_KDRV_ERROR("get_au_info(ret = %d)\n", retVal);
				return retVal;
			}
		}
		else
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_PRINT("AU count = %d\n", auCount);
			return RET_OK;
		}
	}

	//Read a used CAP buffer size
	usedSize = pCapReader->get_used_size(pCapReader);

	//Read a free CAP buffer size
	freeSize = pCapReader->get_free_size(pCapReader);

	//Check a AU read size and used size for AAC encoder
	if(_gCapturerInfo[capIndex].capParam.codecType == LX_AUD_CODEC_AAC_ENC)
	{
		if(pCapData->ui32BufSize != (UINT32)pCapData->auInfo.ui64Size)
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_ERROR("AAC_ENC bufSize error(buf = %d, auSize = %d, used = %d)\n", \
							pCapData->ui32BufSize, (UINT32)pCapData->auInfo.ui64Size, usedSize);

			return RET_ERROR;
		}
	}
	else	//LX_AUD_CODEC_PCM
	{
		if(pCapData->ui32BufSize > (UINT32)usedSize)
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_ERROR("PCM bufSize error(buf = %d, auSize = %d, used = %d)\n", \
							pCapData->ui32BufSize, (UINT32)pCapData->auInfo.ui64Size, usedSize);

			return RET_ERROR;
		}
	}

	//Compute a AU read offset and size
	auReadSize = (SINT32)pCapData->ui32BufSize;

	//Read a AU PCM data from buffer
	retVal = pCapReader->read_data(&pDataBuf, &auReadSize, NULL, pCapReader);
	if(retVal != RET_OK)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_GetCapturedData : read_data(ret = %d)\n", retVal);
		return retVal;
	}

	//Check a AU read size for AAC encoder : This code must be check if PCM data has error.
	if(_gCapturerInfo[capIndex].capParam.codecType == LX_AUD_CODEC_AAC_ENC)
	{
		if(auReadSize != (SINT32)pCapData->ui32BufSize)
		{
			pCapData->auInfo.ui64Size = (UINT64)auReadSize;

			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_ERROR("read size error(%d)\n", auReadSize);
			return RET_ERROR;
		}
	}

	//Check a copy size for allocated memory from user space memory.
	if(auReadSize <= (SINT32)pCapData->ui32BufSize)
	{
		//Copy a from AU ES data from buffer to user memory
		if(copy_to_user((void *)pCapData->pBuffer, (void *)pDataBuf, auReadSize))
		{
			AUD_CAP_BUF_UNLOCK();
			AUD_KDRV_ERROR("AUDIO_GetCapturedData : copy_to_user error!!!\n");
			return RET_ERROR;
		}
	}
	else
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("read size error(%d > %d)\n", auReadSize, pCapData->ui32BufSize);
		return RET_ERROR;
	}

	if(pCapData->ui32BufSize != auReadSize)
		AUD_KDRV_DEBUG("uiSize = %d, RdSize = %d\n", pCapData->ui32BufSize, auReadSize);

	//Copy a memory copied size
	pCapData->auInfo.ui64Size = (UINT64)auReadSize;

	//Flush a current AUI and AU data
	retVal	= pCapReader->flush_data(auReadSize, pCapReader);
	if(retVal != RET_OK)
	{
		AUD_CAP_BUF_UNLOCK();
		AUD_KDRV_ERROR("flush_data(ret = %d)\n", retVal);
		return retVal;
	}

	//Set a current AU index and size of AU
	pCapData->ui32RemainIndex = (UINT32)auCount;
	pCapData->ui32UsedSize = usedSize;
	pCapData->ui32FreeSize = freeSize;

	AUD_CAP_BUF_UNLOCK();
	AUD_KDRV_PRINT("auReadSize = %d\n", auReadSize);
	return RET_OK;
}

/**
 * Reset Capture device.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_ResetCapturer(void)
{
	SINT32						retVal = RET_OK;
	UINT32						allocDev;
	SINT32						i = 0, j  = 0;

	AUD_DEV_INFO_T				*pDevInfo = NULL;
	AUD_MOD_INFO_T				*pModInfo = NULL;
	AUD_CAPTURER_INFO_T			*pCapInfo = NULL;

	for(i = 0; i < DEV_CAP_NUM; i++)
	{
		pCapInfo = &_gCapturerInfo[i];
		allocDev = pCapInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_CAP0 || allocDev > LX_AUD_DEV_CAP2)
		{
			AUD_KDRV_DEBUG("allocDev [%d]\n", allocDev);
			continue;
		}

		pCapInfo->bResetting = TRUE;

		AUDIO_StopCapturer(allocDev);
		AUDIO_FlushCapturer(allocDev);

		AUDIO_DisconnectCapturer(allocDev, pCapInfo->captureConnect.connectDev);

		AUDIO_DisconnectAllModules(allocDev);

		//get alloc module info in given device node
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
				AUD_KDRV_DEBUG("AUDIO_RemoveModule(%d, %d) failed(%d)!!!\n", \
					allocDev, pModInfo->mod, retVal);
				continue;
			}

			AUDIO_FreeModule(pModInfo->mod);
		}
	}
}

/**
 * Reset Capture parameters.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_ResetCapturerParam(void)
{
	UINT32						allocDev;
	UINT32						i = 0;

	LX_AUD_CAPTURE_PARAM_T		*pCapInfoParam = NULL;
	AUD_CAPTURER_INFO_T			*pCapInfo = NULL;

	for(i = 0; i < DEV_CAP_NUM; i++)
	{
		pCapInfo = &_gCapturerInfo[i];
		allocDev = pCapInfo->ui32AllocDev;
		if(allocDev == 0)
			continue;

		pCapInfoParam = &(pCapInfo->capParam);
		AUDIO_SetCaptureParam(allocDev, pCapInfoParam);
		AUDIO_ConnectCapturer(allocDev, pCapInfo->captureConnect.connectDev);
	}
}

/**
 * Restart Capture.
 *
 * @see		AUDIO_Restart().
 */
void	KDRV_AUDIO_RestartCapturer(void)
{
	UINT32						allocDev;
	UINT32						i = 0;

	AUD_CAPTURER_INFO_T			*pCapInfo = NULL;

	for(i = 0; i < DEV_CAP_NUM; i++)
	{
		pCapInfo = &_gCapturerInfo[i];
		allocDev = pCapInfo->ui32AllocDev;
		if(allocDev == 0)
			continue;

		AUDIO_StartCapturer(allocDev);

		pCapInfo->bResetting = FALSE;
	}
}

