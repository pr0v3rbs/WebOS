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


/** @file audio_kadp_encoder_h13.h
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

#ifndef	_AUDIO_KAPI_ENCODER_H_
#define	_AUDIO_KAPI_ENCODER_H_

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
#define	AUD_ENC_IOC_MAGIC							'E'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define AUD_ENC_IOR_GET_KDRV_HANDLE					_IOR(AUD_ENC_IOC_MAGIC, 1, UINT32)
#define AUD_ENC_IOW_CONNECT							_IOW(AUD_ENC_IOC_MAGIC, 2, UINT32)
#define AUD_ENC_IOW_DISCONNECT						_IOW(AUD_ENC_IOC_MAGIC, 3, UINT32)
#define AUD_ENC_IO_START							_IO(AUD_ENC_IOC_MAGIC,  4)
#define AUD_ENC_IO_STOP								_IO(AUD_ENC_IOC_MAGIC,  5)
#define AUD_ENC_IO_FLUSH							_IO(AUD_ENC_IOC_MAGIC,  6)
#define AUD_ENC_IOW_SET_PARAM						_IOW(AUD_ENC_IOC_MAGIC, 7, LX_AUD_ENCODER_PARAM_T)
#define AUD_ENC_IO_CLOSE_DEVICE						_IO(AUD_ENC_IOC_MAGIC,  8)
#define AUD_ENC_IOW_SET_CERTI_PARAM					_IOW(AUD_ENC_IOC_MAGIC, 9, UINT32)

#define AUD_ENC_IOC_MAXNR							20


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO Encoder Bitrate.
 */
typedef enum {
	LX_AUD_ENCODER_BITRATE_48K		=	48000,				///< 48K Bitrate
	LX_AUD_ENCODER_BITRATE_56K		=	56000,				///< 56K Bitrate
	LX_AUD_ENCODER_BITRATE_64K		=	64000,				///< 64K Bitrate
	LX_AUD_ENCODER_BITRATE_80K		=	80000,				///< 80K Bitrate
	LX_AUD_ENCODER_BITRATE_112K		=  112000,				///< 112K Bitrate
	LX_AUD_ENCODER_BITRATE_128K		=  128000,				///< 128K Bitrate
	LX_AUD_ENCODER_BITRATE_160K		=  160000,				///< 160K Bitrate
	LX_AUD_ENCODER_BITRATE_192K		=  192000,				///< 192K Bitrate
	LX_AUD_ENCODER_BITRATE_224K		=  224000,				///< 224K Bitrate
	LX_AUD_ENCODER_BITRATE_256K		=  256000,				///< 256K Bitrate
	LX_AUD_ENCODER_BITRATE_320K		=  320000,				///< 320K Bitrate
	LX_AUD_ENCODER_BITRATE_MAX_NUM	=	   12,				///< max number is 12
} LX_AUD_ENCODER_BITRATE_T;


/**
 * AUDIO ENCODER Parameter.
 */
typedef struct {
	LX_AUD_CODEC_T				codecType;					///< Audio Encoder Codec Type
	UINT32						ui32NumOfChannel;			///< The number of channnel
	LX_AUD_ENCODER_BITRATE_T	bitRate;					///< The Bit Rate
	UINT32						ui32Volume;					///< The volume of encoder
} LX_AUD_ENCODER_PARAM_T;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KAPI_ENCODER_H_ */

/** @} */
