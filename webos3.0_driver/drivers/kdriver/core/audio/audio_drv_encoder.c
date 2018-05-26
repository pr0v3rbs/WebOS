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



/** @file	audio_kdrv_encoder.c
 *
 *  main driver implementation for  audio encoder device.
 *  audio encoder device will teach you how to make device driver with new platform.
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

#include "buffer/adec_buf_helper.h"

#include "audio_imc_func.h"

#include "audio_drv_decoder.h"
#include "audio_drv_encoder.h"
#include "audio_drv_decoder.h"
#include "audio_drv_master.h"
#include "audio_drv_manager.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC	2		// channel of SRC output & ENC input
#define AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC	32		// bits per sample of SRC output & ENC input

#define AUD_ENC_DEFAULT_BITRATE				72000	// Default Bitrate

#define COUNT_MIN_CHECK_START				1
#define COUNT_MAX_CHECK_START				25

OS_SEM_T	_gpEncSema;		// encoder semaphore
BOOLEAN		_gpEncSemaInit = FALSE;

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define AUD_ENC_LOCK_INIT()		OS_InitMutex(&_gpEncSema, OS_SEM_ATTR_DEFAULT)
#define AUD_ENC_LOCK()			OS_LockMutex(&_gpEncSema)
#define AUD_ENC_UNLOCK()			OS_UnlockMutex(&_gpEncSema)

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
AUD_ENCODER_INFO_T	_gEncoderInfo[DEV_ENC_NUM];

UINT32	_gEncoderCheckStartCount[DEV_ENC_NUM] = {0, 0};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static SINT32 AUDIO_StartEncoder(UINT32 allocDev);
static SINT32 AUDIO_StopEncoder(UINT32 allocDev);
static SINT32 AUDIO_FlushEncoder(UINT32 allocDev);
static SINT32 AUDIO_ConnectEncoder(UINT32 allocDev, UINT32 connectDev);
static SINT32 AUDIO_DisconnectEncoder(UINT32 allocDev, UINT32 disconnectDev);
static SINT32 AUDIO_SetEncoderParam(UINT32 allocDev, LX_AUD_ENCODER_PARAM_T *pParam);
static SINT32 AUDIO_CloseEncoder(UINT32 allocDev);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
UINT32	_gEncoderCheckSum = 0;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * initialize for audio encoder device
 *
 */
static SINT32 _AUDIO_InitEncoderParam(UINT32 allocDev)
{
	ULONG	flags;
	SINT32	encIndex = 0;

	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR(" encIndex[%d] is not available. \n", encIndex);
		return RET_ERROR;
	}

	if(_gpEncSemaInit == TRUE)
		AUD_ENC_LOCK();
	memset(&(_gEncoderInfo[encIndex].encoderEvent[0]), 0, sizeof(AUD_EVENT_T)*AUD_EVENT_NUM);
	_gEncoderInfo[encIndex].ui32EventNum = 0;

	memset(&(_gEncoderInfo[encIndex].encoderConnect), 0, sizeof(AUD_CONNECT_T));

	_gEncoderInfo[encIndex].samplingFreq	 = LX_AUD_SAMPLING_FREQ_NONE;
	_gEncoderInfo[encIndex].ui32NumOfChannel = AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC;
	_gEncoderInfo[encIndex].ui32BitPerSample = AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC;
	_gEncoderInfo[encIndex].ui32Volume 		 = 0xFFFFFFFF;
	_gEncoderInfo[encIndex].ui32ChannelMode	 = 0;	//Stereo

	_gEncoderInfo[encIndex].ui32AllocDev 	= allocDev;
	_gEncoderInfo[encIndex].status	 	 	= AUD_STATUS_NONE;
	_gEncoderInfo[encIndex].bDecInfoStarted = FALSE;
	_gEncoderInfo[encIndex].bResetting 	 	= FALSE;
	_gEncoderInfo[encIndex].bClosed 	 	= FALSE;
	_gEncoderInfo[encIndex].ui32CheckSum	= _gEncoderCheckSum;

	//Initalize a global variable.
	_gEncoderCheckStartCount[encIndex] = 0;

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

	if(_gpEncSemaInit == TRUE)
		AUD_ENC_UNLOCK();

	//Initialize AUDIO Decoder semaphore
	if(_gpEncSemaInit == FALSE)
	{
		AUD_ENC_LOCK_INIT();
		_gpEncSemaInit = TRUE;
	}

	AUD_KDRV_PRINT("%s : encIndex %x.\n", __F__, encIndex);
	return RET_OK;
}

/**
 * Set a SRC format for audio encoder device
 *
 */
static void _AUDIO_SetSRCEncoder(UINT32 allocMod, UINT32 sampleFreq, UINT32 numOfChannel, UINT32 bitPerSample)
{
	PcmcvtCmdSetAllfmt 			srcSetFmt;
	PcmcvtCmdSetOverProtect 	srcSetOverProtect;

	//Set a SRC format for encoder input buffer
	srcSetFmt.InFs		= sampleFreq;
	srcSetFmt.InCh		= numOfChannel;
	srcSetFmt.InFormat	= bitPerSample;
	srcSetFmt.InEndian	= 0;	// 0 is little , 1 is big
	srcSetFmt.InSigned	= 0;	// 0 is signed , 1 is unsigned
	srcSetFmt.OutFs		= LX_AUD_SAMPLING_FREQ_48_KHZ;
	srcSetFmt.OutCh		= AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC;
	srcSetFmt.OutFormat	= AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC;

	(void)AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_ALLFMT, allocMod, sizeof(PcmcvtCmdSetAllfmt), &srcSetFmt);

	//Set a overflow protection mode in SRC module to avoid dsp not running if encoder buffer is overflow case.
	srcSetOverProtect.over_protect = USE_OVERFLOW_PROTECTION;

	(void)AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_OVERPROTECT, allocMod, sizeof(PcmcvtCmdSetOverProtect), &srcSetOverProtect);
	return;
}

/**
 * Set a audio encoder channel mode from decoded information.
 *
 * @param 	pEsInfoParam	[in] decoder ES info from decoder.
 * @return 	Channel Mode(0 : Stereo, 1 : Dual Mono).
 * @see		_AUDIO_DecodedInfoCb().
 */
static UINT8 _AUDIO_SetEncoderChMode(DecEvtESDecInfoParam *pEsInfoParam)
{
	UINT8	channelMode;

	LX_AUD_CODEC_T	adecFormat;

	//Set a codec type.
	adecFormat = pEsInfoParam->media_type;

	//Set a default audio stereo channel number.
	channelMode = 0;

	//Set a dual mono channel mode for TP codec only
	if(adecFormat == LX_AUD_CODEC_AC3)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MS10DDC_CHANNELMODE_DUAL_MONO:
				channelMode = 1;
				break;

			default:
				channelMode = 0;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_AAC)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MS10DDT_CHANNELMODE_DUAL_CHANNEL:
				channelMode = 1;
				break;

			default:
				channelMode = 0;
				break;
		}
	}
	else if(adecFormat == LX_AUD_CODEC_MP3)
	{
		switch(pEsInfoParam->channel_mode)
		{
			case MP3DEC_CHANNELMODE_DUAL_MONO:
				channelMode = 1;
				break;

			default:
				channelMode = 0;
				break;
		}
	}

	return channelMode;
}

/**
 * open handler for audio encoder device
 *
 */
SINT32	KDRV_AUDIO_OpenEncoder(struct inode *inode, struct file *filp)
{
	UINT32 	nMajor;
	UINT32 	nMinor;

	LX_AUD_DEV_TYPE_T	devType = LX_AUD_DEV_TYPE_NONE;
	LX_AUD_DEV_T 		allocDev = LX_AUD_DEV_NONE;
	AUD_DEVICE_T		*pDev = NULL;

	nMajor = imajor(inode);
	nMinor = iminor(inode);

	filp->private_data = kmalloc(sizeof(AUD_DEVICE_T), GFP_KERNEL);
	pDev = (AUD_DEVICE_T*)filp->private_data;

	/* Set device */
	devType = DEV_TYPE_BASE + nMinor;
	pDev->devType = devType;

	allocDev = AUDIO_AllocDev(devType);
	if(allocDev == LX_AUD_DEV_NONE)
	{
		AUD_KDRV_ERROR("%s.%d] devType[%d] is not available \n", __F__, __L__, devType);
		kfree(filp->private_data);
		return RET_ERROR;
	}

	pDev->allocDev = allocDev ;

	//Initialze a audio encoder module.
	if(_AUDIO_InitEncoderParam(pDev->allocDev) == RET_ERROR)
	{
		AUDIO_FreeDev(pDev->allocDev);
		kfree(filp->private_data);
		return RET_ERROR;
	}

	AUD_KDRV_PRINT("%s.%d] Open Encoder device file (%d:%d)\n", __F__, __L__, nMajor, nMinor);

	return RET_OK;
}

/**
 * close handler for audio encoder device
 *
 */
SINT32	KDRV_AUDIO_CloseEncoder(struct inode *inode, struct file *filp)
{
	SINT32 retVal = 0;
	UINT32 nMajor;
	UINT32 nMinor;
	SINT32 encIndex = 0;

	AUD_DEVICE_T		*pDev = NULL;
	AUD_ENCODER_INFO_T *pEncInfo = NULL;

	nMajor = imajor(inode);
	nMinor = iminor(inode);

	pDev = (AUD_DEVICE_T*)filp->private_data;

	if(pDev == NULL)
	{
		AUD_KDRV_ERROR("%s.%d] private_data is NULL\n", __F__, __L__);
		return RET_ERROR;
	}

        encIndex = GET_ENC_INDEX(pDev->allocDev);
        if ((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
        {
                AUD_KDRV_ERROR("encoder index(%d) is invalid!!!\n", encIndex);
                return RET_ERROR;
        }

	pEncInfo = &(_gEncoderInfo[encIndex]);

	// to fix audio encoder not working
	if(pEncInfo->ui32CheckSum == pDev->checksum)
	{
		retVal = AUDIO_CloseEncoder(pDev->allocDev);
		if(retVal != RET_OK)
			AUD_KDRV_PRINT("%s.%d: AUDIO_CloseEncoder() failed or already closed.\n", __F__, __L__);
	}
	else
	{
		AUD_KDRV_INFO("checksums are different(%d.%d)\n",
			pEncInfo->ui32CheckSum, pDev->checksum);
	}

	kfree(pDev);

	AUD_KDRV_PRINT("%s.%d] Close Encoder device file  (%d:%d)\n", __F__, __L__, nMajor, nMinor);
	return retVal;
}

/**
 * ioctl handler for audio encoder device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlEncoder(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
long KDRV_AUDIO_IoctlEncoder(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	SINT32				retVal = 0;
	SINT32				err = 0;
	AUD_DEVICE_T		*pDev = NULL;
	UINT32				allocDev;

	pDev = (AUD_DEVICE_T*)filp->private_data;

	allocDev = pDev->allocDev;

	/*
	* check if IOCTL command is valid or not.
	* - if magic value doesn't match, return error (-ENOTTY)
	* - if command is out of range, return error (-ENOTTY)
	*
	* note) -ENOTTY means "Inappropriate ioctl for device.
	*/
	if(_IOC_TYPE(cmd) != AUD_ENC_IOC_MAGIC)
	{
		DBG_PRINT_WARNING("invalid magic. magic=0x%02X\n", _IOC_TYPE(cmd) );
		return -ENOTTY;
	}
	if(_IOC_NR(cmd) > AUD_ENC_IOC_MAXNR)
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
		case AUD_ENC_IO_START:
		{
			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_START] Called\n", __F__, __L__);
			retVal = AUDIO_StartEncoder(allocDev);
		}
		break;

		case AUD_ENC_IO_STOP:
		{
			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_STOP] Called\n", __F__, __L__);
			retVal = AUDIO_StopEncoder(allocDev);
		}
		break;

		case AUD_ENC_IO_FLUSH:
		{
			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_FLUSH] Called\n", __F__, __L__);
			retVal = AUDIO_FlushEncoder(allocDev);
		}
		break;

		case AUD_ENC_IOW_SET_PARAM:
		{
			LX_AUD_ENCODER_PARAM_T	encParam;

			AUD_KDRV_TRACE("AUD_ENC_IOW_SET_PARAM_CODING\n");

			if(copy_from_user(&encParam, (void __user *)arg, sizeof(LX_AUD_ENCODER_PARAM_T)))
				return RET_ERROR;

			retVal = AUDIO_SetEncoderParam(allocDev, &encParam);
		}
		break;

		case AUD_ENC_IOR_GET_KDRV_HANDLE:
		{
			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_GET_KDRV_HANDLE] Called\n", __F__, __L__);

			if(copy_to_user((void *)arg, (void *)&(pDev->allocDev), sizeof(UINT32)))
				return RET_ERROR;
		}
		break;

		case AUD_ENC_IOW_CONNECT:
		{
			UINT32									connectDev;

			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_CONNECT] Called\n", __F__, __L__);

			if(copy_from_user(&connectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			retVal = AUDIO_ConnectEncoder(allocDev, connectDev);
		}
		break;

		case AUD_ENC_IOW_DISCONNECT:
		{
			UINT32									disconnectDev;

			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IOW_DISCONNECT] Called\n", __F__, __L__);

			if(copy_from_user(&disconnectDev, (void __user *)arg, sizeof(UINT32)))
				return RET_ERROR;

			if(disconnectDev <= LX_AUD_DEV_NONE || disconnectDev >= LX_AUD_DEV_MAX)
			{
				AUD_KDRV_RM("[%s][%d] Check Arg\n",__F__,__L__);
				return RET_ERROR;
			}
			else
			{
				retVal = AUDIO_DisconnectEncoder(allocDev, disconnectDev);
			}
		}
		break;

		case AUD_ENC_IO_CLOSE_DEVICE:
		{
			AUD_KDRV_TRACE("%s.%d: [AUD_ENC_IO_CLOSE_DEVICE] Called\n", __F__, __L__);
			retVal = AUDIO_CloseEncoder(pDev->allocDev);
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

/**
 * Register Notification.
 * @param 	pEncInfo		[in] encoder information.
 * @param 	pFuncImcNoti	[in] pointer to callback function.
 * @param 	allocMod		[in] allocated module.
 * @param 	event			[in] event.
 * @return 	void.
 * @see		.
 */
static void _AUDIO_RegisterEncoderNoti(
	AUD_ENCODER_INFO_T* pEncInfo,
	PFN_ImcNoti	pFuncImcNoti,
	UINT32 allocMod,
	UINT32 event,
	IMC_ACTION_REPEAT_TYPE repeatType,
	SINT32 notiLevel)
{
	UINT32							actionID;
	AUD_EVENT_T						*pEncEvent = NULL;

	if(pEncInfo->ui32EventNum >= AUD_EVENT_NUM)
	{
		AUD_KDRV_ERROR("EncEventNum(%d) is over AUD_EVENT_NUM. \n", pEncInfo->ui32EventNum);
		return;
	}

	actionID = AUDIO_IMC_RegisterEvent((void*)pEncInfo, pFuncImcNoti, allocMod,	\
		event, repeatType, notiLevel);

	//if(repeatType != IMC_ACTION_ONCE)
	{
		pEncEvent = &pEncInfo->encoderEvent[pEncInfo->ui32EventNum];
		pEncEvent->event = event;
		pEncEvent->actionID = actionID;
		pEncEvent->moduleID = allocMod;
		pEncInfo->ui32EventNum++;
	}

	AUD_KDRV_PRINT("%s.%d] [0x%x 0x%x, 0x%x] \n", __F__, __L__, event, actionID, allocMod);
	return;
}

static SINT32 _AUDIO_EncDecodedInfoCb(void *_param, int _paramLen, void *_cbParam)
{
	SINT32 encIndex = 0;
	UINT32 channelMode  = 0;

	LX_AUD_DEV_T allocDev 	= LX_AUD_DEV_ENC0;
	ADEC_MODULE_ID allocMod_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID 	allocModule_ENC = ADEC_MODULE_NOT_DEF;

	DecEvtESDecInfoParam	*pDecInfoParam = (DecEvtESDecInfoParam *)_param;
	AUD_ENCODER_INFO_T		*pEncoderInfo  = (AUD_ENCODER_INFO_T *)_cbParam;

	allocDev = pEncoderInfo->ui32AllocDev;

	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR(" encIndex[%d] is not available. \n", encIndex);

		//Set a default encoder index.
		encIndex = 0;
	}

	// Print For Debug
	if(_paramLen != sizeof(DecEvtESDecInfoParam))
	{
		AUD_KDRV_ERROR("Enc Info : Param Length Error[Expected:%d][Input:%d]\n", sizeof(DecEvtESDecInfoParam), _paramLen);
		return RET_ERROR;
	}

	//Update a encoder information by decoder information.
	pEncoderInfo->bDecInfoStarted = TRUE;

	// Set SRC config
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("%s.%d] AUDIO_FindModuleType is not found \n", __F__, __L__);
		return RET_ERROR;
	}

	//Check a previous information if changed now.
	if( (pEncoderInfo->samplingFreq	    != pDecInfoParam->sample_rate)		\
	  ||(pEncoderInfo->ui32NumOfChannel != pDecInfoParam->num_of_channel)	\
	  ||(pEncoderInfo->ui32BitPerSample != pDecInfoParam->bit_per_sample) )
	{
		_AUDIO_SetSRCEncoder(allocMod_SRC,			    pDecInfoParam->sample_rate,
							 pDecInfoParam->num_of_channel, pDecInfoParam->bit_per_sample);
	}

	//Set a AAC ENC Config
	allocModule_ENC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(allocModule_ENC == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("%s.%d] AUDIO_FindModuleType is not found \n", __F__, __L__);
		return RET_ERROR;
	}

	//Set Encoder Params
	if(pEncoderInfo->encoderParam.codecType == LX_AUD_CODEC_AAC_ENC)
	{
		AacEncCmdSetParam 	aacEncSetParam;

		//Get a Channel Mode for decoder information.
		channelMode = _AUDIO_SetEncoderChMode(pDecInfoParam);

		//Check a previous channel mode for audio encoder.
		if(_gEncoderInfo[encIndex].ui32ChannelMode != channelMode)
		{
			//From SRC parameters for encoder
			aacEncSetParam.i_samp_freq = LX_AUD_SAMPLING_FREQ_48_KHZ;
			aacEncSetParam.i_pcm_wd_sz = AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC;
			aacEncSetParam.i_num_chan  = AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC;

			//Input Output Bitrate(72KBps to reduce feeding rate.)
			aacEncSetParam.i_bitrate   = AUD_ENC_DEFAULT_BITRATE;

			//Encode Parameter(Channel mode => 0 : stereo(default), 1 : dual-mono)
			aacEncSetParam.i_ch_mode   = channelMode;

			//Update a global value.
			_gEncoderInfo[encIndex].ui32ChannelMode	= channelMode;

			//Checks a encoder start status.
			if(pEncoderInfo->status != AUD_STATUS_START)
			{
				AUDIO_IMC_SendCmdParam(AACENC_CMD_SET_PARAM, allocModule_ENC, sizeof(AacEncCmdSetParam), &aacEncSetParam);

				AUD_KDRV_DEBUG("[%s] mod(%x), Fs(%d) Bs(%d) Ch(%d) Br(%d)\n", __F__, allocModule_ENC, aacEncSetParam.i_samp_freq,	\
								aacEncSetParam.i_pcm_wd_sz, aacEncSetParam.i_num_chan, aacEncSetParam.i_bitrate);
			}
			else
			{
				//Restarts a Audio Encoder to set channel mode if channel mode is changed.
				(void)AUDIO_IMC_SendCmd(ADEC_CMD_STOP, allocModule_ENC);
				(void)AUDIO_IMC_SendCmdParam(AACENC_CMD_SET_PARAM, allocModule_ENC, sizeof(AacEncCmdSetParam), &aacEncSetParam);
				(void)AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocModule_ENC);
				(void)AUDIO_IMC_SendCmd(ADEC_CMD_START, allocModule_ENC);

				AUD_KDRV_DEBUG("[%s] AENC Restarted!!! => Cm(%d)\n", __F__, channelMode);
			}
		}
	}
	else
	{
		AUD_KDRV_ERROR("[%s] This codec(%d) is not supported.\n", __F__, pEncoderInfo->encoderParam.codecType);
	}

	//Update a encoder information by decoder information.
	pEncoderInfo->samplingFreq 	   = pDecInfoParam->sample_rate;
	pEncoderInfo->ui32NumOfChannel = pDecInfoParam->num_of_channel;
	pEncoderInfo->ui32BitPerSample = pDecInfoParam->bit_per_sample;

	//Starts a encoder by decoder information.
	if(pEncoderInfo->status != AUD_STATUS_START)
		AUDIO_StartEncoder(allocDev);

	AUD_KDRV_DEBUG("Enc Info(%d) : Fs(%d), Ch(%d), Bs(%d), Cm(%d)\n", \
					allocDev, pEncoderInfo->samplingFreq, pEncoderInfo->ui32NumOfChannel, pEncoderInfo->ui32BitPerSample, channelMode);

	return RET_OK;
}

/**
 * Start Encoder.
 *
 * @param 	allocDev		[in] a allocated encoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlEncoder(), KDRV_AUDIO_RestartEncoder()
 */
static SINT32 AUDIO_StartEncoder(UINT32 allocDev)
{
	SINT32	retVal = RET_OK;
	SINT32	encIndex = 0;
	UINT32	connectDev;

	LX_AUD_DEV_TYPE_T	devType;
	ADEC_MODULE_ID		moduleId = ADEC_MODULE_NOT_DEF;
	AUD_ENCODER_INFO_T	*pEncInfo;


	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR(" encIndex[%d] is not available. \n", encIndex);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	pEncInfo = &_gEncoderInfo[encIndex];

	/* Check a decoder information for encoder module. */
	if(pEncInfo->bDecInfoStarted == FALSE)
	{
		/* Get a connected audio decoder information. */
		devType = pEncInfo->encoderConnect.devType;
		if(devType != LX_AUD_DEV_TYPE_DEC)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_ERROR("%s.%d] Audio devType is not decoder.\n", __F__, __L__);
			return RET_ERROR;
		}

		connectDev = pEncInfo->encoderConnect.connectDev;
		moduleId = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
		if(moduleId == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_ERROR("%s.%d: audio module is Unknown\n", __F__, __L__);
			return RET_ERROR;
		}

		/* Set a decoder notification to get a decoder information. */
		retVal = AUDIO_IMC_SendCmd(ADEC_CMD_GET_DECINFO, moduleId);

		AUD_ENC_UNLOCK();
		AUD_KDRV_PRINT("Send GET_DECINFO CMD [%d] \n", moduleId);
		return retVal;
	}

	// Start src
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_INIT, moduleId);
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	// Start encoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_INIT, moduleId);
		AUDIO_IMC_SendCmd(ADEC_CMD_START, moduleId);
	}

	pEncInfo->status = AUD_STATUS_START;

	AUD_ENC_UNLOCK();

	return retVal;
}

/**
 * Stop Encoder.
 *
 * @param 	allocDev		[in] a allocated encoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlEncoder(), KDRV_AUDIO_ResetEncoder()
 */
static SINT32 AUDIO_StopEncoder(UINT32 allocDev)
{
	SINT32			retVal = RET_OK;
	SINT32			encIndex = 0;
	ADEC_MODULE_ID	moduleId = ADEC_MODULE_NOT_DEF;
	AUD_ENCODER_INFO_T	*pEncInfo;

	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR(" encIndex[%d] is not available. \n", encIndex);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	pEncInfo = &_gEncoderInfo[encIndex];

	// Stop encoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
	}

	// Stop src
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmd(ADEC_CMD_STOP, moduleId);
	}

	pEncInfo->status = AUD_STATUS_STOP;

	AUD_ENC_UNLOCK();

	return retVal;
}

/**
 * Flush Encoder.
 *
 * @param 	allocDev		[in] a allocated encoder device.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlEncoder(), KDRV_AUDIO_ResetEncoder()
 */
static SINT32 AUDIO_FlushEncoder(UINT32 allocDev)
{
	SINT32				retVal = RET_OK;

	ImcCmdFlushParam	flushParam;
	ADEC_MODULE_ID		moduleId = ADEC_MODULE_NOT_DEF;

	if(allocDev < LX_AUD_DEV_ENC0 || allocDev > LX_AUD_DEV_ENC1)
	{
		AUD_KDRV_ERROR("%s: allocDev [%d]\n",__F__,allocDev);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	memset(&flushParam, 0, sizeof(ImcCmdFlushParam));
	flushParam.num_of_port = 1;
	flushParam.port_list[0] = MOD_IN_PORT(0);

	// Flush encoder
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
		AUD_KDRV_PRINT("ADEC_CMD_FLUSH [%s] \n", ModuleList_GetModuleName(moduleId));
	}

	// Flush src
	moduleId = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(moduleId != ADEC_MODULE_NOT_DEF)
	{
		AUDIO_IMC_SendCmdParam(ADEC_CMD_FLUSH, moduleId, sizeof(ImcCmdFlushParam), &flushParam);
		AUD_KDRV_PRINT("ADEC_CMD_FLUSH [%s] \n", ModuleList_GetModuleName(moduleId));
	}

	AUD_ENC_UNLOCK();

	return retVal;
}

/**
 * Connect other device to Encoder.
 *
 * @param 	allocDev		[in] a allocated encoder device.
 * @param 	connectDev		[in] Device to connect.
 * @return 	if succeeded - RET_OK, else - RET_ERROR.
 * @see		KDRV_AUDIO_IoctlEncoder(), KDRV_AUDIO_ResetEncoderParam()
*/
static SINT32 AUDIO_ConnectEncoder(UINT32 allocDev, UINT32 connectDev)
{
	SINT32						encIndex = 0;
	SINT32						retVal = 0;

	AUD_ENCODER_INFO_T			*pEncoderInfo = NULL;
	AUD_DEV_INFO_T				*pEncDevInfo = NULL;
	AUD_DEV_INFO_T				*pConnectDevInfo = NULL;
	ADEC_MODULE_ID				connectMod = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_SRC = ADEC_MODULE_NOT_DEF;
	AUD_BUFFER_TYPE_T			bufferType = AUD_BUFFER_TYPE_NONE;
	ADEC_CORE_TYPE				coreType = ADEC_CORE_NOT_DEF;

	PcmcvtCmdSetGain				srcSetGain;

	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR("encIndex[%d] is not available. \n", encIndex);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	pEncoderInfo = &_gEncoderInfo[encIndex];

	/* Get a allocated information */
	pEncDevInfo		= AUDIO_GetDevInfo(allocDev);
	pConnectDevInfo = AUDIO_GetDevInfo(connectDev);

	if(pConnectDevInfo == NULL || pEncDevInfo == NULL)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_DEBUG("%s.%d] Check pConnectDevInfo[%p] pEncDevInfo[%p] \n", __F__, __L__, pConnectDevInfo, pEncDevInfo);
		return RET_ERROR;
	}

	if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		connectMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
		if(connectMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_RMD("%s.%d] AUDIO_FindModuleType failed.\n", __F__, __L__);
			return RET_ERROR;
		}

		bufferType = AUD_BUFFER_TYPE_NONE;
		coreType = ADEC_CORE_DSP0;

		 _AUDIO_RegisterEncoderNoti(pEncoderInfo, (PFN_ImcNoti)_AUDIO_EncDecodedInfoCb, connectMod, DEC_EVT_ES_DEC_INFO, IMC_ACTION_REPEAT, 1);
	}
	else if(pConnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		connectMod = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_MIXER);
		if(connectMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_RMD("%s.%d] AUDIO_FindModuleType failed.\n", __F__, __L__);
			return RET_ERROR;
		}

		bufferType = AUD_BUFFER_TYPE_ENC;
		coreType = ADEC_CORE_DSP1;
	}
	else
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_RMD("%s.%d] devType(%d) is not supported.\n", __F__, __L__, pConnectDevInfo->devType);
		return RET_ERROR;
	}


	/* Allocate a SRC Module for Encoder */
	allocMod_SRC = AUDIO_AllocModule(ADEC_MOD_TYPE_PCMCVT, coreType, ADEC_MODULE_NOT_DEF);
	if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_ERROR("%s.%d] AUDIO_AllocModule[%d] failed. \n", __F__, __L__, ADEC_MOD_TYPE_PCMCVT);
		return RET_ERROR;
	}

	/* Set a default SRC format for encoder */
	_AUDIO_SetSRCEncoder(allocMod_SRC, LX_AUD_SAMPLING_FREQ_48_KHZ,	\
						 AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC, AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC);

	/* Set a SRC_DEC gain. */
	srcSetGain.GainEnable	= 1;
	srcSetGain.Gain			= pEncoderInfo->ui32Volume;
	AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_GAIN, allocMod_SRC, sizeof(PcmcvtCmdSetGain), &srcSetGain);

	/* Allocate a Encoder SRC to Encoder Module */
	retVal = AUDIO_InsertModuleFirst(allocDev, allocMod_SRC, bufferType);
	if(retVal != 0)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_ERROR("%s.%d] AUDIO_InsertModuleLast(%d) failed!!!\n", __F__, __L__, retVal);
		return RET_ERROR;
	}

	retVal = AUDIO_ConnectModules(connectMod, allocMod_SRC, AUD_BUFFER_TYPE_NONE);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("%s.%d] AUDIO_ConnecModules(%d) failed!!!\n", __F__, __L__, retVal);
		return RET_ERROR;
	}

	//Send a IMC command - INIT & START
	AUDIO_IMC_SendCmd(ADEC_CMD_INIT, allocMod_SRC);
	AUDIO_IMC_SendCmd(ADEC_CMD_START, allocMod_SRC);

	if(pEncoderInfo->bResetting == FALSE)
	{
		pEncoderInfo->encoderConnect.devType	= pConnectDevInfo->devType;
		pEncoderInfo->encoderConnect.connectDev = connectDev;
	}

	AUD_ENC_UNLOCK();

	AUD_KDRV_PRINT("%s.%d] ConnectEncoder : Fs(%d), Ch(%d), Bs(%d)\n", __F__, __L__, \
					pEncoderInfo->samplingFreq, pEncoderInfo->ui32NumOfChannel, pEncoderInfo->ui32BitPerSample);
	return RET_OK;
}

/**
 * Disconnect other device to Encoder.
 * @see
*/
static SINT32 AUDIO_DisconnectEncoder (UINT32 allocDev, UINT32 disconnectDev)
{
	SINT32						retVal = 0;

	ADEC_MODULE_ID				disconnectMod = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				allocMod_SRC = ADEC_MODULE_NOT_DEF;
	AUD_DEV_INFO_T				*pEncDevInfo = NULL;
	AUD_DEV_INFO_T				*pDisconnectDevInfo = NULL;

	AUD_ENC_LOCK();

	/* Get a allocated information */
	pEncDevInfo		= AUDIO_GetDevInfo(allocDev);
	pDisconnectDevInfo = AUDIO_GetDevInfo(disconnectDev);

	if(pDisconnectDevInfo == NULL || pEncDevInfo == NULL)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_DEBUG("%s.%d] Check pDisconnectDevInfo[%p] pEncDevInfo[%p] \n", __F__, __L__, pDisconnectDevInfo, pEncDevInfo);
		return RET_ERROR;
	}

	if(pDisconnectDevInfo->devType == LX_AUD_DEV_TYPE_DEC)
	{
		/* Find a BYPASS module in connected decoder module. */
		disconnectMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_DECODER);
		if(disconnectMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_RMD("%s.%d] AUDIO_ConnectEncoder : No Valid Connection Trial\n", __F__, __L__);
			return RET_ERROR;
		}
	}
	else if(pDisconnectDevInfo->devType == LX_AUD_DEV_TYPE_MAS)
	{
		disconnectMod = AUDIO_FindModuleType(disconnectDev, ADEC_MOD_TYPE_MIXER);
		if(disconnectMod == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_RMD("%s.%d] AUDIO_ConnectEncoder : No Valid Connection Trial\n", __F__, __L__);
			return RET_ERROR;
		}
	}
	else
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_RMD("No Valid Disconnection Trial\n");
		return RET_ERROR;
	}

	allocMod_SRC = AUDIO_FindHeadModule(allocDev);
	if(allocMod_SRC == ADEC_MODULE_NOT_DEF)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_ERROR("%s.%d] AUDIO_FindHeadModule is failed!!!\n", __F__, __L__);
		return RET_ERROR;
	}

	retVal = AUDIO_DisconnectModules(disconnectMod, allocMod_SRC);
	if(retVal != 0)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_ERROR("%s.%d] AUDIO_DisconnectModules(%d) is failed!!!\n", __F__, __L__, retVal);
		return RET_ERROR;
	}

	AUD_ENC_UNLOCK();
	return RET_OK;
}

static SINT32 AUDIO_SetEncoderParam(UINT32 allocDev, LX_AUD_ENCODER_PARAM_T *pParam)
{
	SINT32				encIndex = 0;
	SINT32				retVal = 0;

	ADEC_MODULE_ID		managerModule = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID		allocMod_ENC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID		allocMod_SRC = ADEC_MODULE_NOT_DEF;
	AUD_DEV_INFO_T		*pDevInfo = NULL;
	AUD_ENCODER_INFO_T	*pEncoderInfo = NULL;

	CmCmdCreateModule	createModule;

	if(pParam == NULL)
	{
		AUD_KDRV_ERROR("%s.%d: pParam is NULL.\n", __F__, __L__);
		return RET_ERROR;
	}

	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR(" encIndex[%d] is not available.\n", encIndex);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	pEncoderInfo = &_gEncoderInfo[encIndex];

	allocMod_ENC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(allocMod_ENC == ADEC_MODULE_NOT_DEF)
	{
		/* Allocate a Encoder Module */
		allocMod_ENC = AUDIO_AllocModule(ADEC_MOD_TYPE_ENCODER, ADEC_CORE_DSP0, ADEC_MODULE_NOT_DEF);
		if(allocMod_ENC == ADEC_MODULE_NOT_DEF)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_ERROR("%s.%d] AUDIO_AllocModule[%d] is failed. \n", __F__, __L__, ADEC_MOD_TYPE_ENCODER);
			return RET_ERROR;
		}

		/* Get a Manager Module for Encoder */
		managerModule = AUDIO_GetManagerModule(allocMod_ENC);
		if(managerModule != ADEC_MODULE_NOT_DEF)
		{
			createModule.module_id = allocMod_ENC;
			createModule.module_param.codec_param.media_type = pParam->codecType;

			AUDIO_IMC_SendCmdParam(CM_CMD_CREATE_MODULE, managerModule, sizeof(CmCmdCreateModule), &createModule);

			AUD_KDRV_PRINT("%s.%d: CM_CMD_CREATE_MODULE, ENC : 0x%x : %d\n", __F__, __L__, createModule.module_id, createModule.module_param.codec_param.media_type);
		}

		retVal = AUDIO_InsertModuleLast(allocDev, allocMod_ENC, AUD_BUFFER_TYPE_NONE);
		if(retVal != 0)
		{
			AUD_ENC_UNLOCK();
			AUD_KDRV_ERROR("%s.%d] AUDIO_InsertModuleLast(%d) is failed!!!\n", __F__, __L__, retVal);
			return RET_ERROR;
		}
	}

	//Copy a encoder parameters : bitrate only
	memcpy(&(pEncoderInfo->encoderParam), pParam, sizeof(LX_AUD_ENCODER_PARAM_T));

	//Set Encoder and SRC Params
	if(pParam->codecType == LX_AUD_CODEC_AAC_ENC)
	{
		AacEncCmdSetParam 	aacEncSetParam;

		//From SRC parameters for encoder
		aacEncSetParam.i_samp_freq = LX_AUD_SAMPLING_FREQ_48_KHZ;
		aacEncSetParam.i_pcm_wd_sz = AUD_ENC_BITS_PER_SAMPLE_OF_SRC_ENC;
		aacEncSetParam.i_num_chan  = AUD_ENC_NUM_OF_CHANNEL_OF_SRC_ENC;

		// Input Output Bitrate
		//aacEncSetParam.i_bitrate   = _gEncoderInfo[encIndex].encoderParam.bitRate;
		aacEncSetParam.i_bitrate   = AUD_ENC_DEFAULT_BITRATE;

		//Encode Parameter(Channel mode => 0 : stereo(default), 1 : dual-mono)
		aacEncSetParam.i_ch_mode   = 0;

		//Update a global value.
		pEncoderInfo->ui32ChannelMode	= 0;	//Stereo

		AUDIO_IMC_SendCmdParam(AACENC_CMD_SET_PARAM, allocMod_ENC, sizeof(AacEncCmdSetParam), &aacEncSetParam);

		AUD_KDRV_PRINT("%s.%d] mod(%x), Fs(%d) Bs(%d) Ch(%d) Br(%d)\n", __F__, __L__, allocMod_ENC, aacEncSetParam.i_samp_freq,	\
						aacEncSetParam.i_pcm_wd_sz, aacEncSetParam.i_num_chan, aacEncSetParam.i_bitrate);
	}
	else
	{
		AUD_KDRV_ERROR("%s.%d] This codecType(%d) is not supported.\n", __F__, __L__, pParam->codecType);
		return RET_ERROR;
	}

	/* Set a SRC_DEC gain. */
	allocMod_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocMod_SRC != ADEC_MODULE_NOT_DEF)
	{
		PcmcvtCmdSetGain		srcSetGain;

		srcSetGain.GainEnable	= 1;
		srcSetGain.Gain			= pParam->ui32Volume;
		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_GAIN, allocMod_SRC, sizeof(PcmcvtCmdSetGain), &srcSetGain);
	}

	pEncoderInfo->ui32Volume = pParam->ui32Volume;

	// set device output type
	pDevInfo = AUDIO_GetDevInfo(allocDev);
	if(pDevInfo == NULL)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_ERROR("AUDIO_GetDevInfo(%d) failed.\n", allocDev);
		return RET_ERROR;
	}

	pDevInfo->devOutType = LX_AUD_DEV_OUT_ES;

	AUD_ENC_UNLOCK();

	AUD_KDRV_DEBUG("bitRate[%d], volume[0x%X]", pParam->bitRate, pParam->ui32Volume);

	return RET_OK;
}

static SINT32 AUDIO_CloseEncoder(UINT32 allocDev)
{
	SINT32 i = 0;
	SINT32 retVal = 0;
	SINT32 encIndex = 0;

	ADEC_MODULE_ID		allocModule_SRC = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID		allocModule_ENC = ADEC_MODULE_NOT_DEF;

	AUD_EVENT_T			*pEncEvent = NULL;
	AUD_ENCODER_INFO_T	*pEncoderInfo;

	PcmcvtCmdSetGain	srcSetGain;
	CmCmdDestroyModule	destroyModule;

	/* Get a Encoder Index */
	encIndex = GET_ENC_INDEX(allocDev);
	if((encIndex < 0) || (encIndex >= DEV_ENC_NUM))
	{
		AUD_KDRV_ERROR("encIndex[%d] is not available. \n", encIndex);
		return RET_ERROR;
	}

	AUD_ENC_LOCK();

	pEncoderInfo = &_gEncoderInfo[encIndex];

	if(pEncoderInfo->bClosed == TRUE)
	{
		AUD_ENC_UNLOCK();
		AUD_KDRV_PRINT(" decIndex[%d] is already closed. \n", encIndex);
		return RET_OK;
	}

	retVal = AUDIO_DisconnectAllModules(allocDev);
	if(retVal == RET_ERROR)
	{
		AUD_KDRV_ERROR("%s.%d] AUDIO_DisconnectAllModules is failed in dev[%d] \n", __F__, __L__, allocDev);
	}

	allocModule_SRC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_PCMCVT);
	if(allocModule_SRC != ADEC_MODULE_NOT_DEF)
	{
		srcSetGain.GainEnable = 0;
		srcSetGain.Gain		  = 0xFFFFFFFF;

		AUDIO_IMC_SendCmdParam(PCMCVT_CMD_SET_GAIN, allocModule_SRC, sizeof(PcmcvtCmdSetGain), &srcSetGain);

		pEncoderInfo->ui32Volume = 0xFFFFFFFF;
	}

	allocModule_ENC = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_ENCODER);
	if(allocModule_ENC != ADEC_MODULE_NOT_DEF)
	{
		destroyModule.module_id = allocModule_ENC;
		AUDIO_IMC_SendCmdParam(CM_CMD_DESTROY_MODULE, AUDIO_GetManagerModule(allocModule_ENC), sizeof(destroyModule), &destroyModule);
		AUD_KDRV_PRINT("%s.%d] CM_CMD_DESTROY_MODULE [%s] \n", __F__, __L__, ModuleList_GetModuleName(allocModule_ENC));
	}

	AUDIO_FreeDev(allocDev);

	for(i = 0; i < pEncoderInfo->ui32EventNum; i++)
	{
		pEncEvent = &(pEncoderInfo->encoderEvent[i]);

		AUDIO_IMC_CancelEvent(pEncEvent->event, pEncEvent->moduleID, pEncEvent->actionID);
		AUD_KDRV_PRINT("%s.%d] IMC_CancelEvent(%d) is (%d, %d, %d)!!!\n", __F__, __L__, encIndex, pEncEvent->event,	\
						pEncEvent->moduleID, pEncEvent->actionID);
	}

	//Clear a encoder resource.
	pEncoderInfo->ui32EventNum	= 0;
	pEncoderInfo->bClosed		= TRUE;
	pEncoderInfo->ui32AllocDev	= 0;

	AUD_ENC_UNLOCK();

	AUD_KDRV_PRINT("%s.%d] Close Device %d.\n", __F__, __L__, allocDev);
	return RET_OK;
}

void	KDRV_AUDIO_ResetEncoder(void)
{
	SINT32						retVal = RET_OK;
	UINT32						allocDev;
	SINT32						i = 0, j  = 0;

	CmCmdDestroyModule			destroyModule;

	ADEC_MODULE_ID				encoderModule = ADEC_MODULE_NOT_DEF;
	AUD_DEV_INFO_T				*pDevInfo = NULL;
	AUD_MOD_INFO_T				*pModInfo = NULL;
	AUD_ENCODER_INFO_T			*pEncInfo = NULL;

	for(i = 0; i < DEV_ENC_NUM; i++)
	{
		pEncInfo = &_gEncoderInfo[i];
		allocDev = pEncInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_ENC0 || allocDev > LX_AUD_DEV_ENC1)
		{
			AUD_KDRV_DEBUG("%s.%d] allocDev [%d]\n", __F__, __L__, allocDev);
			continue;
		}

		pEncInfo->bResetting = TRUE;

		AUDIO_StopEncoder(allocDev);
		AUDIO_FlushEncoder(allocDev);

		AUDIO_DisconnectEncoder(allocDev, pEncInfo->encoderConnect.connectDev);

		AUDIO_DisconnectAllModules(allocDev);
		encoderModule = AUDIO_FindModuleType(allocDev, ADEC_MOD_TYPE_DECODER);
		if(encoderModule != ADEC_MODULE_NOT_DEF )
		{
			destroyModule.module_id = encoderModule;
			AUDIO_IMC_SendCmdParam(CM_CMD_DESTROY_MODULE, AUDIO_GetManagerModule(encoderModule), sizeof(destroyModule), &destroyModule);
			AUD_KDRV_PRINT("%s.%d] CM_CMD_DESTROY_MODULE [%s] \n", __F__, __L__, ModuleList_GetModuleName(encoderModule));
		}

		//get alloc module info in given device node
		pDevInfo = AUDIO_GetDevInfo(allocDev);
		if(pDevInfo == NULL)
		{
			AUD_KDRV_DEBUG("%s.%d] pDevInfo[%d] is NULL.\n", __F__, __L__, allocDev);
			continue;
		}

		for(j = (pDevInfo->numMod - 1); j >= 0; j--)
		{
			pModInfo = pDevInfo->pModInfo[j];
			if(pModInfo == NULL)
			{
				AUD_KDRV_DEBUG("%s.%d] pModInfo[%d] is NULL.\n", __F__, __L__, allocDev);
				continue;
			}

			retVal = AUDIO_RemoveModule(allocDev, pModInfo->mod);
			if(retVal < RET_OK)
			{
				AUD_KDRV_DEBUG("%s.%d] AUDIO_RemoveModule(%d, %d) is failed(%d)!!!\n", __F__, __L__, \
					allocDev, pModInfo->mod, retVal);
				continue;
			}

			AUDIO_FreeModule(pModInfo->mod);
		}
	}
}

void	KDRV_AUDIO_ResetEncoderParam(void)
{
	LX_AUD_ENCODER_PARAM_T		*pEncInfoParam = NULL;
	UINT32						allocDev;
	SINT32						i = 0;
	AUD_ENCODER_INFO_T			*pEncInfo = NULL;

	for(i = 0; i < DEV_ENC_NUM; i++)
	{
		pEncInfo = &_gEncoderInfo[i];
		allocDev = pEncInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_ENC0 || allocDev > LX_AUD_DEV_ENC1)
		{
			AUD_KDRV_DEBUG("%s.%d] allocDev [%d]\n", __F__, __L__, allocDev);
			continue;
		}

		pEncInfoParam = &(pEncInfo->encoderParam);
		AUDIO_SetEncoderParam(allocDev, pEncInfoParam);
		AUDIO_ConnectEncoder(allocDev, pEncInfo->encoderConnect.connectDev);
	}
}

void	KDRV_AUDIO_RestartEncoder(void)
{
	UINT32						allocDev;
	SINT32						i = 0;
	AUD_ENCODER_INFO_T			*pEncInfo = NULL;

	for(i = 0; i < DEV_ENC_NUM; i++)
	{
		pEncInfo = &_gEncoderInfo[i];
		allocDev = pEncInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_ENC0 || allocDev > LX_AUD_DEV_ENC1)
		{
			AUD_KDRV_DEBUG("%s.%d] allocDev [%d]\n", __F__, __L__, allocDev);
			continue;
		}

		AUDIO_StartEncoder(allocDev);

		pEncInfo->bResetting = FALSE;
	}
}

/**
 * Redemand Decoded Notification.
 * @see
*/
void	KDRV_AUDIO_RedemandDecodedNofiEncoder(void)
{
	UINT32						tempCheckStartCount = 0;
	UINT32						allocDev;
	SINT32						i = 0;
	UINT32						connectDev;
	ADEC_MODULE_ID				moduleId = ADEC_MODULE_NOT_DEF;
	ADEC_MODULE_ID				tempModuleId = ADEC_MODULE_NOT_DEF;
	AUD_ENCODER_INFO_T			*pEncInfo = NULL;

	for(i = 0; i < DEV_ENC_NUM; i++)
	{
		pEncInfo = &_gEncoderInfo[i];
		allocDev = pEncInfo->ui32AllocDev;
		if(allocDev < LX_AUD_DEV_ENC0 || allocDev > LX_AUD_DEV_ENC1)
		{
			continue;
		}

		/* Set a decoder notification to get a decoder information. */
		if(pEncInfo->status != AUD_STATUS_START)
		{
			if(pEncInfo->encoderConnect.devType == LX_AUD_DEV_TYPE_DEC)
			{
				connectDev = pEncInfo->encoderConnect.connectDev;
				moduleId   = AUDIO_FindModuleType(connectDev, ADEC_MOD_TYPE_DECODER);
				if(moduleId == ADEC_MODULE_NOT_DEF)
				{
					AUD_KDRV_DEBUG("%s.%d: audio module is Unknown\n", __F__, __L__);
					continue;
				}

				tempCheckStartCount = _gEncoderCheckStartCount[i]++;

				if((tempCheckStartCount > COUNT_MIN_CHECK_START) && (tempCheckStartCount <= COUNT_MAX_CHECK_START))
				{
					if(moduleId != tempModuleId)
					{
						(void)AUDIO_IMC_SendCmd(ADEC_CMD_GET_DECINFO, moduleId);
						tempModuleId = moduleId;

						AUD_KDRV_DEBUG("%s.%d] ADEC_CMD_GET_DECINFO.\n", __F__, __L__);
					}
				}
			}
		}
	}

	return;
}



