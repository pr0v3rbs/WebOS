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



/** @file audio_drv_encoder.h
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

#ifndef	_AUDIO_DRV_ENCODER_H_
#define	_AUDIO_DRV_ENCODER_H_

#include "audio_kapi_encoder.h"
#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * AUDIO Encoder Information.
 */
typedef struct {
	UINT32							ui32AllocDev;
	LX_AUD_ENCODER_PARAM_T			encoderParam;
	AUD_EVENT_T						encoderEvent[AUD_EVENT_NUM];

	LX_AUD_SAMPLING_FREQ_T			samplingFreq;		///< Audio Sampling Frequency Info.
	UINT32							ui32NumOfChannel;	///< The number of channnel of PCM
	UINT32							ui32BitPerSample;	///< The bit per sample of PCM data
	UINT32							ui32Volume;			///< The volume for DEC SRC
	UINT32							ui32ChannelMode;	///< The channel mode for AENC Module.
	UINT32							ui32EventNum;
	AUD_STATUS_T					status;
	UINT32							bDecInfoStarted;
	UINT32							bClosed;
	UINT32							bResetting;			// For Reset Recovery
	AUD_CONNECT_T					encoderConnect;		// For Reset Recovery
	UINT32							ui32CheckSum;		///< add checksum to fix audio capture not working.

#ifdef IC_CERTIFICATION
	UINT32							encoderCertiParam;
#endif
} AUD_ENCODER_INFO_T;

SINT32	KDRV_AUDIO_OpenEncoder(struct inode *inode, struct file *filp);
SINT32	KDRV_AUDIO_CloseEncoder(struct inode *inode, struct file *filp);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlEncoder(struct inode *, struct file *, unsigned int, unsigned long);
#else
long KDRV_AUDIO_IoctlEncoder(struct file *filp, unsigned int cmd, unsigned long arg);
#endif


void	KDRV_AUDIO_ResetEncoder(void);
void	KDRV_AUDIO_ResetEncoderParam(void);
void	KDRV_AUDIO_RestartEncoder(void);
void	KDRV_AUDIO_RedemandDecodedNofiEncoder(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_ENCODER_H_ */


