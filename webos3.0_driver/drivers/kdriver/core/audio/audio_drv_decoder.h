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



/** @file audio_drv_decoder.h
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

#ifndef	_AUDIO_DRV_DECODER_H_
#define	_AUDIO_DRV_DECODER_H_

#include "audio_kapi_decoder.h"
#include "audio_drv_manager.h"
#include "audio_buffer.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO DECODER Index.
 */
 typedef enum
{
	AUD_DECODER_0,
	AUD_DECODER_1,
} AUD_DECODER_ID_T;

/**
 * AUDIO DECODER Decoding Parameter.
 */
typedef struct
{
	UINT32							ui32AllocDev;
	LX_AUD_DECODER_PARAM_T			decParam;
	LX_AUD_DECODER_PARAM_EXT_T		decParamExt;
	LX_AUD_DECODER_MODE_T			decMode;
	LX_AUD_DECODED_INFO_T			decodedInfo;
	LX_AUD_DECODED_STATUS_T			decodedStatus;
	LX_AUD_DECODER_OUT_MODE_T		decOutMode;
	AUD_EVENT_T						decEvent[AUD_EVENT_NUM];
	UINT64							ui64Timestamp[CPB_AUI_INDEX_COUNT];
	AUD_STATUS_T					status;
	UINT32							bClosed;
	UINT32							bResetting;
	UINT32							decIsDecodable;
	UINT32							ui32EventNum;
	BOOLEAN							bBufferRepeat;		///< The status of buffer repeat is set previously.
	UINT32							ui32ChannelNum;		///< The decoded number of channel of original ES.
	BOOLEAN							bSpdifESAvail;		///< Whether SPDIF ES output is available or not.
	LX_AUD_CODEC_DETAIL_T			codecDetailType;	///< Audio Codec Type Detail Info.
	UINT32							ui32CheckSum;		///< add checksum to fix audio decoder not working.
} AUD_DECODER_INFO_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
SINT32	KDRV_AUDIO_OpenDecoder(struct inode *inode, struct file *filp);
SINT32	KDRV_AUDIO_CloseDecoder(struct inode *inode, struct file *filp);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
SINT32	KDRV_AUDIO_IoctlDecoder(struct inode *, struct file *, unsigned int, unsigned long);
#else
long	KDRV_AUDIO_IoctlDecoder(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

void 	KDRV_AUDIO_SetDTOIntSrc(AUD_INT_T interrupt);
void	KDRV_AUDIO_ResetDecoder(void);
void	KDRV_AUDIO_ResetDecoderParam(void);
void	KDRV_AUDIO_RestartDecoder(void);

void	KDRV_AUDIO_GetDecodedInfo(LX_AUD_GET_DECODED_INFO_T *pDecInfo);
void KDRV_AUDIO_SetUseReformatter(BOOLEAN bUseReformatter);

SINT32 AUDIO_GetDecStartInfo(UINT32 allocDev, UINT32 *pStarted);
void KDRV_AUDIO_DestroyDecLib(void);
void KDRV_AUDIO_GetDecoderParam(UINT32 allocDev, LX_AUD_DECODER_PARAM_T *pDecParam);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_DECODER_H_ */

