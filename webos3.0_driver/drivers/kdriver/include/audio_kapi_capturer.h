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


/** @file audio_kadp_capturer.h
 *
 *  This is a wrapper function file for adec_ddi.c to process ADEC function.
 *  The simple wrapper function for audio kernel module.
 *
 *  @author	Won Chang Shin(wonchang.shin@lge.com)
 *  @version   0.1
 *  @date		2012.04.18
 *  @note		The simple wrapper function for audio kernel module.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_KAPI_CAPTURER_H_
#define	_AUDIO_KAPI_CAPTURER_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "audio_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	AUD_CAP_IOC_MAGIC							'C'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define AUD_CAP_IOR_GET_KDRV_HANDLE					_IOR(AUD_CAP_IOC_MAGIC,  2, UINT32)
#define AUD_CAP_IOW_CONNECT							_IOW(AUD_CAP_IOC_MAGIC,  3, UINT32)
#define AUD_CAP_IOW_DISCONNECT						_IOW(AUD_CAP_IOC_MAGIC,  4, UINT32)
#define AUD_CAP_IO_START							_IO(AUD_CAP_IOC_MAGIC,   5)
#define AUD_CAP_IO_STOP								_IO(AUD_CAP_IOC_MAGIC,   6)
#define AUD_CAP_IO_FLUSH							_IO(AUD_CAP_IOC_MAGIC,   7)
#define AUD_CAP_IOW_SET_PARAM						_IOW(AUD_CAP_IOC_MAGIC,  8, LX_AUD_CAPTURE_PARAM_T)
#define AUD_CAP_IOR_GET_CAPTURED_INFO				_IOR(AUD_CAP_IOC_MAGIC,  9, LX_AUD_CAPTURE_CAPTURED_INFO_T)
#define AUD_CAP_IOWR_GET_CAPTURED_DATA				_IOWR(AUD_CAP_IOC_MAGIC,10, LX_AUD_CAPTURE_CAPTURED_DATA_T)
#define AUD_CAP_IO_CLOSE_DEVICE						_IO(AUD_CAP_IOC_MAGIC,  11)
#define AUD_CAP_IOW_SET_VOLUME						_IOW(AUD_CAP_IOC_MAGIC,12, UINT32)

#define AUD_CAP_IOC_MAXNR							20

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO CAPTURE input.
 */
typedef enum {
	LX_AUD_CAPTURE_INPUT_NONE,								///< None
	LX_AUD_CAPTURE_INPUT_ENCODER,							///< Encoder module in Encoder
	LX_AUD_CAPTURE_INPUT_MIXER,								///< Mixer module in Master
	LX_AUD_CAPTURE_INPUT_SE,								///< Sound Engine module in Master
	LX_AUD_CAPTURE_INPUT_DECODER_PCM,						///< Decoder Module's PCM output port in decoder
	LX_AUD_CAPTURE_INPUT_DECODER_ES,						///< Decoder Module's ES output port in decoder
	LX_AUD_CAPTURE_INPUT_I2S,								///< I2S_IN module in decoder
} LX_AUD_CAPTURE_INPUT_T;

/**
 * AUDIO CAPTURE Parameter.
 */
typedef struct {
	LX_AUD_CODEC_T				codecType;					///< Audio Capturer Codec TYpe
	LX_AUD_PCM_FORMAT_T			inPcmFormat;				///< The Format of input PCM
	LX_AUD_PCM_FORMAT_T			outPcmFormat;				///< The Format of output PCM(little endian and signed is fixed.)
	LX_AUD_CAPTURE_INPUT_T		capInput;					///< LX_AUD_CAPTURE_INPUT_T
	UINT32						ui32InputI2SMask;			///< The input I2S Mask : 0x1: pcmi4lrch0(default), 0x2: pcmi4lrch1, 0x4: pcmi4lrch2, 0x8: pcmi4lrch3
} LX_AUD_CAPTURE_PARAM_T;

/**
 * AUDIO CAPTURE Callback information.
 */
typedef struct {
	void						*pCapData;					///< the pointer to captured data
	UINT32						ui32CapDataSize;			///< the size of captured Data
} LX_AUD_CAPTURE_CB_INFO_T;

/**
 * AUDIO CAPTURE Buffer information.
 */
typedef struct {
	UINT32						ui32WriteIndex;				///< the write index in AUI Buffer
	UINT32						ui32SizeOfAu;				///< the size of AU
	UINT32						ui32RemainIndex;			///< the remain index in AUI Buffer
	UINT32						ui32UsedSize;				///< the used size in AUI Buffer
	UINT32						ui32FreeSize;				///< the free size in AUI Buffer
} LX_AUD_CAPTURE_CAPTURED_INFO_T;

/**
 * AUDIO CAPTURE information.
 */
typedef struct {
	LX_AUD_AU_INFO_T			auInfo;						///< the audio AU(Access Unit) info of buffer
	void						*pBuffer;					///< the pointer to buffer assigned by user
	UINT32						ui32BufSize;				///< the size of buffer
	UINT32						ui32RemainIndex;			///< the remain index in AUI Buffer
	UINT32						ui32UsedSize;				///< the used size in AUI Buffer
	UINT32						ui32FreeSize;				///< the free size in AUI Buffer
} LX_AUD_CAPTURE_CAPTURED_DATA_T;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KAPI_CAPTURER_H_ */

/** @} */
