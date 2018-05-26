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



/** @file audio_drv_master.h
 *
 *  driver interface header for audio device. ( used only within kdriver )
 *	audio device will teach you how to make device driver with new platform.
 *
 *  @author	wonchang.shin (wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2012.04.25
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_MASTER_H_
#define	_AUDIO_DRV_MASTER_H_

#include "audio_kapi_master.h"
#include "audio_drv_manager.h"
#include "module/adec_module_list.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * AUDIO Master Output Control Module.
 */
typedef struct {
	ADEC_MODULE_ID				module;
	UINT32						ui32Volume;
	UINT32						ui32Mute;
	UINT32						ui32Delay;
	UINT32						ui32Balance;
	LX_AUD_SAMPLING_FREQ_T		samplingFreq;
	LX_AUD_MASTER_OUT_MODE_T	outMode;
	UINT32						ui32ConnectDev;
	UINT32						ui32ConnectIndex;
	UINT32						ui32UseSE;
} AUD_OUT_CTRL_MODULE_T;

/**
 * AUDIO DECODER Decoding Parameter.
 */
typedef struct
{
	UINT32							ui32AllocDev;
	AUD_OUT_CTRL_MODULE_T			outCtrlMod[LX_AUD_MASTER_OUTPUT_MAX];
	LX_AUD_MASTER_SOUNDBAR_STATUS_T	soundBarStatus;
	LX_AUD_MASTER_SPDIF_OUTPUT_T	spdifOutputFormat;	// 0 : PCM, 1 : ES
	AUD_EVENT_T						mstEvent[AUD_EVENT_NUM];
	LX_AUD_SAMPLING_FREQ_T			spdifSamplingFreq;
	UINT32							seModeParam[3];
	UINT32							ui32EventNum;
	UINT32							spdifOnOff;			// 0 : OFF, 1 : ON
	UINT32							spdifCategoryCode;	// US : 0x64, EU : 0x0C, JP : 0x04
	UINT32							ui32I2SNum;			// 2CH : 1, 4CH : 2,... for multichannel speakers
	OS_SEM_T						spdifSemaphore;
	UINT32							ui32Delay;			///< delay ms.
	UINT32							mixerInPortVolume[MAX_IN_PORT];		///< mixer input port volume
	UINT32							openCount;			///< device open count (reference count)
} AUD_MASTER_INFO_T;

SINT32	KDRV_AUDIO_OpenMaster(struct inode *inode, struct file *filp);
SINT32	KDRV_AUDIO_CloseMaster(struct inode *inode, struct file *filp);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlMaster(struct inode *, struct file *, unsigned int, unsigned long);
#else
long KDRV_AUDIO_IoctlMaster(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

SINT32 AUDIO_SetMixerVolume(LX_AUD_MASTER_CONFIG_MIXER_VOLUME_T *pMixerVolumeConfig);
SINT32 KDRV_AUDIO_UpdateSamplingFreq(LX_AUD_SAMPLING_FREQ_T samplingFreq, LX_AUD_MASTER_SPDIF_OUTPUT_T spdifOutputType);

#ifdef KDRV_CONFIG_PM
void	AUDIO_SE_CopyFrom(void);
void	AUDIO_SE_CopyTo(void);
#endif

void KDRV_AUDIO_ResetMaster(void);
void KDRV_AUDIO_ResetMasterParam(void);
void KDRV_AUDIO_RestartMaster(void);
void KDRV_AUDIO_ResetDisconnectMaster(void);
void KDRV_AUDIO_ResetConnectMaster(void);

UINT32 AUDIO_OpenMaster(void);
SINT32 AUDIO_CloseMaster(UINT32 allocDev);

SINT32 AUDIO_GetSoundBarStatus(LX_AUD_MASTER_SOUNDBAR_STATUS_T *pSoundbarStatus);
SINT32 AUDIO_ConnectOutput(UINT32 connectDev, LX_AUD_MASTER_OUTPUT_T output, UINT32 bUseSE);
SINT32 AUDIO_DisconnectOutput(UINT32 connectDev, LX_AUD_MASTER_OUTPUT_T output);

void KDRV_AUDIO_RegisterMasterNoti(void);

#ifdef DISABLE_SCART_HW_BYPASS
//extern
extern OS_SEM_T	_gMasterSema;		// master semaphore
#define AUD_MAS_LOCK()			OS_LockMutex(&_gMasterSema)
#define AUD_MAS_UNLOCK()		OS_UnlockMutex(&_gMasterSema)
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_MASTER_H_ */


