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


/** @file audio_kadp_decoder.h
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

#ifndef	_AUDIO_KAPI_DECODER_H_
#define	_AUDIO_KAPI_DECODER_H_

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
#define	AUD_DEC_IOC_MAGIC						'D'

// define Audio Decoder Parameter Number
#define NUM_OF_CODEC_NONE_INFO					0
#define NUM_OF_CODEC_WMAPRO_INFO				5
#define NUM_OF_CODEC_AAC_INFO					2
#define NUM_OF_CODEC_RA_INFO					3
#define NUM_OF_CODEC_ADPCM_INFO					2
#define NUM_OF_CODEC_AC3_INFO					1
#define NUM_OF_CODEC_DDT_INFO					1
#define NUM_OF_CODEC_DTS_INFO					1
#define NUM_OF_CODEC_MAX_INFO					10

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define AUD_DEC_IOW_GET_KDRV_HANDLE				_IOR(AUD_DEC_IOC_MAGIC, 2, UINT32)
#define AUD_DEC_IO_START						_IO(AUD_DEC_IOC_MAGIC, 3)
#define AUD_DEC_IO_STOP							_IO(AUD_DEC_IOC_MAGIC, 4)
#define AUD_DEC_IO_FLUSH						_IO(AUD_DEC_IOC_MAGIC, 5)
#define AUD_DEC_IOW_FEED						_IOW(AUD_DEC_IOC_MAGIC, 6, LX_AUD_DECODER_FEED_T)
#define AUD_DEC_IOW_SET_PARAM					_IOW(AUD_DEC_IOC_MAGIC, 7, LX_AUD_DECODER_PARAM_T)
#define AUD_DEC_IOW_IS_DECODABLE				_IOW(AUD_DEC_IOC_MAGIC, 8, UINT32)
#define AUD_DEC_IOR_GET_INFO					_IOR(AUD_DEC_IOC_MAGIC, 9, LX_AUD_DECODED_INFO_T)
#define AUD_DEC_IO_CLOSE_DEVICE					_IO(AUD_DEC_IOC_MAGIC, 10)
#define AUD_DEC_IOW_SET_MODE					_IOW(AUD_DEC_IOC_MAGIC, 11, LX_AUD_DECODER_MODE_T)
#define AUD_DEC_IOR_GET_STATUS					_IOR(AUD_DEC_IOC_MAGIC, 12, LX_AUD_DECODED_STATUS_T)
#define AUD_DEC_IOW_SET_OUT_MODE				_IOW(AUD_DEC_IOC_MAGIC, 13, LX_AUD_DECODER_OUT_MODE_T)
#define AUD_DEC_IOW_SET_CERTI_PARAM				_IOW(AUD_DEC_IOC_MAGIC, 14, UINT32)
#define AUD_DEC_IOW_SET_INDEX_CTX				_IOW(AUD_DEC_IOC_MAGIC, 15, LX_AUD_DEV_INDEX_CTX_T)
#define AUD_DEC_IOW_SET_PARAM_EXT				_IOW(AUD_DEC_IOC_MAGIC, 16, LX_AUD_DECODER_PARAM_EXT_T)

#define AUD_DEC_IOC_MAXNR						200

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO DECODER SPEAKER OUTPUT Type.
 */
typedef enum {
	LX_AUD_DECODER_OUT_LR,				///< Left Right output
	LX_AUD_DECODER_OUT_LL,				///< Left Left output
	LX_AUD_DECODER_OUT_RR,				///< Right Right output
	LX_AUD_DECODER_OUT_MIX,				///< Mix output
	LX_AUD_DECODER_OUT_MAX,				///< Max marker for this enumeration
}  LX_AUD_DECODER_OUT_MODE_T;

/**
 * AUDIO DECODER ADPCM CODEC TYPE.
 */
typedef enum
{
	LX_AUD_DECODER_ADPCM_NONE			= 0,	///< Not defined codec
	LX_AUD_DECODER_ADPCM_IMA_QT			= 1,	///< ADPCM IMA_QT codec
	LX_AUD_DECODER_ADPCM_IMA_WAV		= 2,	///< ADPCM IMA_WAV codec
	LX_AUD_DECODER_ADPCM_IMA_DK3		= 3,	///< ADPCM IMA_DK3 codec
	LX_AUD_DECODER_ADPCM_IMA_DK4		= 4,	///< ADPCM IMA_DK4 codec
	LX_AUD_DECODER_ADPCM_IMA_WS			= 5,	///< ADPCM IMA_WS codec
	LX_AUD_DECODER_ADPCM_IMA_SMJPEG		= 6,	///< ADPCM IMA_SMJPEG codec
	LX_AUD_DECODER_ADPCM_MS				= 7,	///< ADPCM MS codec
	LX_AUD_DECODER_ADPCM_4XM			= 8,	///< ADPCM 4XM codec
	LX_AUD_DECODER_ADPCM_XA				= 9,	///< ADPCM XA codec
	LX_AUD_DECODER_ADPCM_ADX			= 10,	///< ADPCM ADX codec
	LX_AUD_DECODER_ADPCM_EA				= 11,	///< ADPCM EA codec
	LX_AUD_DECODER_ADPCM_G726			= 12,	///< ADPCM G726 codec
	LX_AUD_DECODER_ADPCM_CT				= 13,	///< ADPCM CT codec
	LX_AUD_DECODER_ADPCM_SWF			= 14,	///< ADPCM SWF codec
	LX_AUD_DECODER_ADPCM_YAMAHA			= 15,	///< ADPCM YAMAHA codec
	LX_AUD_DECODER_ADPCM_SBPRO_4		= 16,	///< ADPCM SBPRO_4 codec
	LX_AUD_DECODER_ADPCM_SBPRO_3		= 17,	///< ADPCM SBPRO_3 codec
	LX_AUD_DECODER_ADPCM_SBPRO_2		= 18,	///< ADPCM SBPRO_2 codec
	LX_AUD_DECODER_ADPCM_THP			= 19,	///< ADPCM THP codec
	LX_AUD_DECODER_ADPCM_IMA_AMV		= 20,	///< ADPCM IMA_AMV codec
	LX_AUD_DECODER_ADPCM_EA_R1			= 21,	///< ADPCM EA_R1 codec
	LX_AUD_DECODER_ADPCM_EA_R3			= 22,	///< ADPCM EA_R3 codec
	LX_AUD_DECODER_ADPCM_EA_R2			= 23,	///< ADPCM EA_R2 codec
	LX_AUD_DECODER_ADPCM_IMA_EA_SEAD	= 24,	///< ADPCM IMA_EA_SEAD codec
	LX_AUD_DECODER_ADPCM_IMA_EA_EACS	= 25,	///< ADPCM IMA_EA_EACS codec
	LX_AUD_DECODER_ADPCM_EA_XAS			= 26,	///< ADPCM EA_XAS codec
	LX_AUD_DECODER_ADPCM_EA_MAXIS_XA	= 27,	///< ADPCM EA_MAXIS_XA codec
	LX_AUD_DECODER_ADPCM_IMA_ISS		= 28,	///< ADPCM IMA_ISS codec
	LX_AUD_DECODER_ADPCM_G722			= 29,	///< ADPCM G722 codec
} LX_AUD_DECODER_ADPCM_T;

/**
 * AUDIO DECODER AC3 DOLBY DRC Mode Type.
 */
typedef  enum
{
	LX_AUD_DECODER_DRC_LINE				= 0,	///< LINE MODE
	LX_AUD_DECODER_DRC_RF				= 1,	///< RF MODE(DVB TP Only)
	LX_AUD_DECODER_DRC_OFF				= 2,	///< DRC OFF MODE
} LX_AUD_DECODER_DRC_T;

/**
 * AUDIO DECODER DOWNMIX Mode Type.
 */
typedef  enum
{
	LX_AUD_DECODER_DOWNMIX_LORO			= 0,	///< Left only/Right only(AAC)
	LX_AUD_DECODER_DOWNMIX_LTRT			= 1,	///< Left total/Right total(AC3)
} LX_AUD_DECODER_DOWNMIX_T;

/**
 * AUDIO DECODER Bypass Mode.
 */
typedef  enum
{
	LX_AUD_DECODER_BYPASS_DD			= 0,	///< DD(default)
	LX_AUD_DECODER_BYPASS_DDP			= 1,	///< DD PLUS
} LX_AUD_DECODER_BYPASS_MODE_T;

/**
 * AUDIO DECODER SPDIF AAC ES BYPASS Mode Type.
 */
typedef  enum
{
	LX_AUD_DECODER_AAC_BYPASS_OFF		= 0,	///< SPDIF AAC ES Bypass Off(AC3)
	LX_AUD_DECODER_AAC_BYPASS_ON		= 1,	///< SPDIF AAC ES Bypass Off(AAC)
} LX_AUD_DECODER_AAC_BYPASS_T;

/**
 * AUDIO DECODER Mode Parameter.
 */
typedef struct {
	LX_AUD_DECODER_DRC_T		drc;			///< drc mode
	LX_AUD_DECODER_DOWNMIX_T	downmix;		///< downmix mode
	LX_AUD_DECODER_AAC_BYPASS_T	aacBypass;		///< SPDIF AAC ES Bypass mode
	UINT32						tpTimerInterval;///< TP Input Timer Interval(Normal : 0x2000, MHEG Burst Mode : 0x200)
	BOOLEAN 					bSetAD;			///< AD(Audio Description) mode for DVB TP Stream.
	BOOLEAN 					bSetSubDec;		///< Sub Decoding mode for SPDIF ES Not Support(Main Dec(SPDIF ES Support).
} LX_AUD_DECODER_MODE_T;

/**
 * AUDIO DECODER WMAPRO information Parameter.
 */
typedef struct {
	UINT32						ui32NumberOfChannel;				///< The number of channel
	UINT32						ui32PcmBitsPerSample;				///< The PCM bits per a sample
	UINT32						ui32AvgBytesPerSec; 				///< The average bytes per seconds for WMA codec
	UINT32						ui32BlockAlign; 					///< The block alignment for WMA codec
	UINT32						ui32WmaFormatTag;					///< The format tag for WMA codec
} LX_AUD_WMAPRO_INFO_T;

/**
 * AUDIO DECODER ADPCM information Parameter.
 */
typedef struct {
	UINT32						ui32NumberOfChannel;				///< The number of channel
	UINT32						ui32PcmBitsPerSample;				///< The PCM bits per a sample
	UINT32						ui32AdpcmType; 						///< ADPCM type LX_AUD_DECODER_ADPCM_T
	UINT32						ui32BlockAlign; 					///< The block alignment for ADPCM codec
} LX_AUD_ADPCM_INFO_T;

/**
 * AUDIO DECODER RA information Parameter.
 */
typedef struct {
	UINT32						ui32NumberOfChannel;				///< The number of channel
	UINT32						ui32PcmBitsPerSample;				///< The PCM bits per a sample
	UINT32						ui32LeafSize; 						///< leaf size
} LX_AUD_RA_INFO_T;

/**
 * AUDIO DECODER PCM information Parameter.
 */
typedef struct {
	UINT32						ui32NumberOfChannel;				///< The number of channel
	UINT32						ui32PcmBitsPerSample;				///< The PCM bits per a sample
	UINT32						ui32Endian; 						///< 0 is little , 1 is big
	UINT32						ui32Signed; 						///< 0 is signed , 1 is unsigned
} LX_AUD_PCM_INFO_T;

/**
 * AUDIO DECODER Decoding Parameter.
 */
typedef struct
{
	LX_AUD_INPUT_T				input;								///< input port
	LX_AUD_CODEC_T				codecType;							///< audio codec type
	LX_AUD_SAMPLING_FREQ_T		samplingFreq;						///< sampling frequency
	UINT32 						ui32LengthOfDSI;					///< The length of DSI buffer
	void 						*pDSIBuffer;						///< The pointer of DSI(Decoding Specific Info.) buffer for AAC file decoding
	LX_AUD_WMAPRO_INFO_T		wmaproInfo;							///< The WMAPRO information
	LX_AUD_ADPCM_INFO_T			adpcmInfo;							///< The ADPCM information
	LX_AUD_RA_INFO_T			raInfo;								///< The RA8 information
	LX_AUD_PCM_INFO_T			pcmInfo;							///< The PCM information
} LX_AUD_DECODER_PARAM_T;

/**
 * AUDIO DECODER Feed Information Parameter.
 */
typedef struct {
	void						*pBuffer;							///< the pointer to data buffer address in user space
	UINT32						ui32BufferSize;						///< the size of buffer
	LX_AUD_BUFFER_T				statusBuffer;						///< determines frequency response
	UINT64						ui64TimeStamp; 						///< the time stamp in nano-seconds
	UINT32						ui32RepeatNumber;					///< the number of repeat
	BOOLEAN 					bUseGstcClock;						///< the status of using GSTC clock for Google TVP Path
} LX_AUD_DECODER_FEED_T;

/**
 * AUDIO DECODER Dual-mono Mode
 * This enumeration describes the  AUDIO DECODER audio mode.
 *
*/
typedef enum
{
	LX_AUD_DECODER_MODE_MONO	 		= 0,						///< Mono mode
	LX_AUD_DECODER_MODE_JOINT_STEREO 	= 1,						///< Joint Stereo mode
	LX_AUD_DECODER_MODE_STEREO		 	= 2,						///< Stereo mode
	LX_AUD_DECODER_MODE_DUAL_MONO 		= 3,						///< Dual Mono mode
	LX_AUD_DECODER_MODE_MULTI			= 4,						///< Multi mode
	LX_AUD_DECODER_MODE_UNKNOWN			= 5							///< Unknown mode
} LX_AUD_DECODER_AUDIO_MODE_T;

/**
 * AUDIO DECODER TP Channel Mode.
 * This enumeration describes the AUDIO DECODER channel mode.
 *
*/
typedef enum LX_AUD_DECODER_CHANNEL_MODE
{
	LX_AUD_DECODER_CH_MODE_MONO	 					= 0,
	LX_AUD_DECODER_CH_MODE_JOINT_STEREO 			= 1,
	LX_AUD_DECODER_CH_MODE_STEREO					= 2,
	LX_AUD_DECODER_CH_MODE_DUAL_MONO				= 3,
	LX_AUD_DECODER_CH_MODE_MULTI					= 4,
	LX_AUD_DECODER_CH_MODE_UNKNOWN					= 5,
	LX_AUD_DECODER_CH_MODE_2_1_FL_FR_LFE 			= 6,
	LX_AUD_DECODER_CH_MODE_3_0_FL_FR_RC	 			= 7,
	LX_AUD_DECODER_CH_MODE_3_1_FL_FR_RC_LFE			= 8,
	LX_AUD_DECODER_CH_MODE_4_0_FL_FR_RL_RR			= 9,
	LX_AUD_DECODER_CH_MODE_4_1_FL_FR_RL_RR_LFE		= 10,
	LX_AUD_DECODER_CH_MODE_5_0_FL_FR_FC_RL_RR		= 11,
	LX_AUD_DECODER_CH_MODE_5_1_FL_FR_FC_RL_RR_LFE	= 12,
} LX_AUD_DECODER_CHANNEL_MODE_T;

/**
 * AUDIO DECODER HE-AAC ES Info
 * This structure describes AUDIO DECODER HEAAC ES(Elementary Stream) information.
 * adec ioctl parameter.
 *
*/
typedef struct {
	UINT8 version;       											///< Version : AAC = 0x0, HE-AACv1 = 0x1, HE-AACv2 = 0x2
	UINT8 transmissionformat;     									///< Transmission format : LOAS/LATM = 0x0, ADTS = 0x1
	UINT8 channelNum;												///< Refer to LX_ADEC_AUDIO_MODE_T
} LX_AUD_DECODER_HEAAC_ES_INFO_T;

/**
 * AUDIO DECODER MPEG ES Info
 * This structure describes AUDIO DECODER MPEG ES(Elementary Stream) information.
 * adec ioctl parameter.
 *
*/
typedef struct {
	UINT8 bitRate;													///< Bit rate(Value is divide by 1000.(Value/KBps))
	UINT8 sampleRate;												///< Sampling rate((Value is divide by 1000.(Value/KHz))
	UINT8 layer;													///< MPEG audio layer
	UINT8 channelNum; 												///< Refer LX_ADEC_AUDIO_MODE_T
} LX_AUD_DECODER_MPEG_ES_INFO_T;

/**
 * AUDIO DECODER AC3 ES Info
 * This structure describes AUDIO DECODER AC3/EAC3 ES(Elementary Stream) information.
 * adec ioctl parameter.
 *
*/
typedef struct {
	UINT8 bitRate;													///< Bit rate(Value is divide by 1000.(Value/KBps))
	UINT8 sampleRate;												///< Sampling rate((Value is divide by 1000.(Value/KHz))
	UINT8 channelNum;												///< Refer LX_ADEC_AUDIO_MODE_T
	UINT8 EAC3;       												///< EAC3 or not : AC3 0x0, EAC3 0x1
} LX_AUD_DECODER_AC3_ES_INFO_T;


/**
 * AUDIO DECODER ES Info.
 * adec ioctl parameter.
 * @see AUD_IOR_GET_ES_INFO
*/
typedef struct {
	LX_AUD_CODEC_T		 			adecFormat;						///< Audio codec Type
	LX_AUD_DECODER_AUDIO_MODE_T		audioMode;						///< Audio Mode refer to LX_AUD_DECODER_AUDIO_MODE_T
	LX_AUD_DECODER_HEAAC_ES_INFO_T 	heAAcEsInfo;					///< HE-AAC Audio ES Information
	LX_AUD_DECODER_MPEG_ES_INFO_T  	mpegESInfo;						///< MPEG Audio ES Information
	LX_AUD_DECODER_AC3_ES_INFO_T   	ac3ESInfo;						///< AC3 Audio ES Information
} LX_AUD_DECODER_ES_INFO_T;

/**
 * AUDIO DECODED Info from DSP after decoding is completed.
 * adec ioctl parameter.
 * @see AUD_IOR_GET_ES_INFO
*/
typedef struct {
	LX_AUD_DECODER_ES_INFO_T 		esInfo;							///< Audio ES Info.

	LX_AUD_CODEC_T					codecType;						///< Audio Codec Type Info.
	LX_AUD_SAMPLING_FREQ_T			samplingFreq;					///< Audio Sampling Frequency Info.
	UINT32							ui32AvgBitRate;					///< The average bitrate per seconds.
	UINT32							ui32NumOfChannel;				///< The number of channnel of PCM
	UINT32							ui32BitPerSample;				///< The bit per sample of PCM data
} LX_AUD_DECODED_INFO_T;

/**
 * AUDIO DECODED Status from DSP.
 * adec ioctl parameter.
 * @see AUD_IOR_GET_DECODED_STATUS
*/
typedef struct {
	UINT32		ui32MaxMemSize;										///< The Max size
	UINT32		ui32FreeMemSize;									///< The Free size
	UINT32		ui32MaxAuiSize;										///< The Max AUI(Access Unit Index) size
	UINT32		ui32FreeAuiSize;									///< The free AUI(Access Unit Index) size
	UINT32		ui32FeededIndex;									///< The Feeded Index
	UINT32		ui32FeededCount;									///< The Feeded Count
	UINT32		ui32DecodedIndex;									///< The Decoded Index
	UINT64		ui64Timestamp;										///< The Decoded timestamp
} LX_AUD_DECODED_STATUS_T;

/**
 * AUDIO DECODER Decoding Extended Parameters.
 */
typedef struct
{
	LX_AUD_CODEC_T		codecType;									///< audio codec type.
	UINT32				ui32NumOfParam;								///< number of parameters.
	UINT32				param[NUM_OF_CODEC_MAX_INFO];				///< param for DTS codec.
} LX_AUD_DECODER_PARAM_EXT_T;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KAPI_DECODER_H_ */

/** @} */
