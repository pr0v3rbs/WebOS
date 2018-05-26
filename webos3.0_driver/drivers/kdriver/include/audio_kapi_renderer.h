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


/** @file audio_kadp_renderer.h
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

#ifndef	_AUDIO_KAPI_RENDERER_H_
#define _AUDIO_KAPI_RENDERER_H_

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
#define	AUD_REN_IOC_MAGIC					'R'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define AUD_REN_IOW_CONNECT					_IOW(AUD_REN_IOC_MAGIC,	2, UINT32)
#define AUD_REN_IOW_DISCONNECT				_IOW(AUD_REN_IOC_MAGIC,	3, UINT32)
#define AUD_REN_IO_START					_IO(AUD_REN_IOC_MAGIC,	4)
#define AUD_REN_IO_STOP						_IO(AUD_REN_IOC_MAGIC,	5)
#define AUD_REN_IO_FLUSH					_IO(AUD_REN_IOC_MAGIC,	6)
#define AUD_REN_IOW_FEED					_IOW(AUD_REN_IOC_MAGIC,	7,	LX_AUD_RENDER_FEED_T)
#define AUD_REN_IOW_SET_PARAM				_IOW(AUD_REN_IOC_MAGIC,	8,	LX_AUD_RENDER_PARAM_T)

#define AUD_REN_IOW_SET_LIPSYNCPARAM		_IOW(AUD_REN_IOC_MAGIC,	10,	LX_AUD_RENDER_PARAM_LIPSYNC_T)
#define AUD_REN_IOW_ENABLE_LIPSYNC			_IOW(AUD_REN_IOC_MAGIC,	11,	UINT32)
#define AUD_REN_IOW_SET_BASETIME			_IOW(AUD_REN_IOC_MAGIC,	14,	LX_AUD_RENDER_PARAM_BASETIME_T)
#define AUD_REN_IOW_SET_CLOCKTYPE			_IOW(AUD_REN_IOC_MAGIC,	15,	LX_AUD_RENDER_PARAM_CLOCKTYPE_T)
#define AUD_REN_IOR_GET_KDRV_HANDLE			_IOR(AUD_REN_IOC_MAGIC, 16, UINT32)
#define AUD_REN_IO_CLOSE_DEVICE				_IO(AUD_REN_IOC_MAGIC,  17)
#define AUD_REN_IOR_GET_STATUS				_IOR(AUD_REN_IOC_MAGIC, 18, LX_AUD_RENDERED_STATUS_T)
#define AUD_REN_IOW_ENABLE_RTS				_IOW(AUD_REN_IOC_MAGIC,	19,	UINT32)
#define AUD_REN_IOW_TRICK_PLAY				_IOW(AUD_REN_IOC_MAGIC,	20,	UINT32)
#define AUD_REN_IOW_SET_NODELAYPARAM		_IOW(AUD_REN_IOC_MAGIC,	21,	LX_AUD_RENDER_PARAM_NODELAY_T)
#define AUD_REN_IOW_SET_INDEX_CTX			_IOW(AUD_REN_IOC_MAGIC, 22, LX_AUD_DEV_INDEX_CTX_T)
#define AUD_REN_IOW_SET_INPUTCTRL_VOLUME	_IOW(AUD_REN_IOC_MAGIC,	23,	UINT32)
#define AUD_REN_IOW_SET_INPUTCTRL_MUTE		_IOW(AUD_REN_IOC_MAGIC,	24,	UINT32)
#define AUD_REN_IOW_SET_INPUTCTRL_DELAY		_IOW(AUD_REN_IOC_MAGIC,	25,	UINT32)

#define AUD_REN_IOR_GET_PRESENTED_PTS		_IOR(AUD_REN_IOC_MAGIC,	27,	UINT32)
#define AUD_REN_IOW_SET_CH_VOLUME			_IOW(AUD_REN_IOC_MAGIC,	28,	LX_AUD_RENDER_CH_VOLUME_T)
#define AUD_REN_IOW_SET_RTS_PARAM			_IOW(AUD_REN_IOC_MAGIC, 29, LX_AUD_RENDER_RTS_PARAM_T)

#define AUD_REN_IOC_MAXNR					200

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO Renderer Clock Type Index.
 */
typedef enum {
	LX_AUD_RENDER_CLK_TYPE_PCR_M		=	0x1,		///< main PCR.
	LX_AUD_RENDER_CLK_TYPE_PCR_A		=	0x2,		///< associate PCR.
	LX_AUD_RENDER_CLK_TYPE_NO_PCR		=	0x11,		///< no PCR.
	LX_AUD_RENDER_CLK_TYPE_GSTC			=	0x100,		///< global STC.
	LX_AUD_RENDER_CLK_TYPE_MASTER 		=	0x200,		///< audio master.
} LX_AUD_RENDER_CLK_TYPE_T;

/**
 * AUDIO Renderer Lipsync Type Index.
 */
typedef enum {
	LX_AUD_RENDER_LIPSYNC_PCM			=	0x0,		///< input data is PCM
	LX_AUD_RENDER_LIPSYNC_ES			=	0x1,		///< input data is ES
} LX_AUD_RENDER_LIPSYNC_TYPE_T;

/**
 * AUDIO DVR Trick Mode Type.
 *
 */
typedef  enum
{
	LX_AUD_TRICK_NONE				= 0, 	///<  rate : None
	LX_AUD_TRICK_PAUSE				= 1, 	///<  rate : Pause
	LX_AUD_TRICK_NORMAL_PLAY		= 2, 	///<  rate : Normal Play
	LX_AUD_TRICK_SLOW_MOTION_0P25X 	= 3,	///<  rate : 0.25 硅加Play
	LX_AUD_TRICK_SLOW_MOTION_0P50X 	= 4,	///<  rate : 0.50 硅加Play
	LX_AUD_TRICK_SLOW_MOTION_0P80X 	= 5,	///<  rate : 0.80 硅加Play
	LX_AUD_TRICK_FAST_FOWARD_1P20X 	= 6, 	///<  rate : 1.20 硅加Play
	LX_AUD_TRICK_FAST_FOWARD_1P50X 	= 7, 	///<  rate : 1.50 硅加Play
	LX_AUD_TRICK_FAST_FOWARD_2P00X	= 8,	///<  rate : 2.00 硅加Play
	LX_AUD_TRICK_ONEFRAME_DECODE	= 9,	///<  rate : one frame decoding
} LX_AUD_TRICK_MODE_T;

/**
 * Endian of PCM.
 */
typedef enum {
	LX_AUD_RENDER_LITTLE_ENDIAN			= 0,			///< little endian
	LX_AUD_RENDER_BIG_ENDIAN			= 1,			///< big endian
} LX_AUD_RENDER_ENDIAN_T;

/**
 * Signed of PCM.
 */
typedef enum {
	LX_AUD_RENDER_SIGNED				= 0,			///< signed PCM
	LX_AUD_RENDER_UNSIGNED				= 1,			///< unsigned PCM
} LX_AUD_RENDER_SIGNED_T;

/**
 * AUDIO Renderer Lipsync Parameter.
 */
typedef struct {
	UINT32 						ui32Lbound;				///< lower bound. unit is miliseconds.
	UINT32 						ui32Ubound;				///< upper bound unit is miliseconds.
	UINT32 						ui32Offset;				///< offset unit is miliseconds.
	UINT32 						ui32Freerunlbound;		///< freerun lower bound. unit is miliseconds.
	UINT32 						ui32Freerunubound;		///< freerun upper bound. unit is miliseconds.
} LX_AUD_RENDER_PARAM_LIPSYNC_T;

/**
 * AUDIO Renderer Clock Basetime Parameter.
 */
typedef struct {
	UINT64						ui64ClockBaseTime;		///< clock base time.
	UINT64						ui64StreamBaseTime;		///< stream base time.
} LX_AUD_RENDER_PARAM_BASETIME_T;

/**
 * AUDIO Renderer Clock Type Parameter.
 */
typedef struct {
	LX_AUD_RENDER_CLK_TYPE_T	clocktype;				///< clock type
} LX_AUD_RENDER_PARAM_CLOCKTYPE_T;

/**
 * AUDIO Renderer Codec Parameter.
 */
typedef struct {
	LX_AUD_INPUT_T					input;				///< input port
	LX_AUD_RENDER_LIPSYNC_TYPE_T	lipsyncType;		///< lipsync type(PCM is PCM render, ES is SPDIF ES render)
	UINT32							ui32NumOfChannel;	///< The number of channnel of PCM
	UINT32							ui32BitPerSample;	///< The bit per sample of PCM data
	LX_AUD_SAMPLING_FREQ_T			samplingFreq;		///< Audio Sampling Frequency Info.
	LX_AUD_RENDER_ENDIAN_T			endianType;			///< The endian of PCM(0 is little, 1 is big)
	LX_AUD_RENDER_SIGNED_T			signedType;			///< The signed of PCM(0 is signed, 1 is unsigned)
	BOOLEAN 						bSetMainRen;		///< Main Render mode for Main SRC control.(Main : SIF, HDMI Input Only)
	BOOLEAN 						bSetTrickMode;		///< Render mode for trick play control.(TRUE : Decoder, FALSE : Mixer)
} LX_AUD_RENDER_PARAM_T;

/**
 * AUDIO Renderer Volume Configuration.
 */
typedef struct {
	UINT32 						ui32Volume;				///< volume.
} LX_AUD_RENDER_CONFIG_VOLUME_T;

/**
 * AUDIO Renderer Lipsync Configuration.
 */
typedef struct {
	UINT32 						ui32OnOff;				///< lipsync ON, OFF
} LX_AUD_RENDER_CONFIG_LIPSYNC_T;

/**
 * AUDIO RENDERER Feed Information Parameter.
 */
typedef struct {
	void						*pBuffer;				///< the pointer to data buffer address in user space
	UINT32						ui32BufferSize;			///< the size of buffer
	LX_AUD_BUFFER_T				statusBuffer;			///< determines frequency response
	UINT64						ui64TimeStamp; 			///< the time stamp in nano-seconds
	UINT32						ui32RepeatNumber;		///< the number of repeat
	BOOLEAN 					bUseGstcClock;			///< the status of using GSTC clock for Google TVP Path
} LX_AUD_RENDER_FEED_T;

/**
 * AUDIO RENDERED Status from DSP.
 * adec ioctl parameter.
 * @see AUD_IOR_GET_RENDERED_STATUS
*/
typedef struct {
	UINT32					ui32MaxMemSize;				///< The Max size
	UINT32					ui32FreeMemSize;			///< The Free size
	UINT32					ui32MaxAuiSize;				///< The Max AUI(Access Unit Index) size
	UINT32					ui32FreeAuiSize;			///< The free AUI(Access Unit Index) size
	UINT32					ui32FeededIndex;			///< The Feeded Index
	UINT32					ui32FeededCount;			///< The Feeded Count
	UINT32					ui32RenderedIndex;			///< The Rendered Index
	UINT64					ui64Timestamp;				///< The Rendered timestamp
	BOOLEAN 				bPresentEnd;				///< The status of Present End.
} LX_AUD_RENDERED_STATUS_T;

/**
 * AUDIO Renderer Nodelay Parameter.
 */
typedef struct {
	UINT32 						ui32OnOff;				///< nodelay ON, OFF.
	UINT32 						ui32UThreshold;			///< upper threshold unit is miliseconds.
	UINT32 						ui32LThreshold;			///< lower threshold unit is miliseconds.
} LX_AUD_RENDER_PARAM_NODELAY_T;

/**
 * AUDIO Renderer Channel Volume.
 */
typedef struct {
	UINT32 						ui32LeftVol;				///< left channel Volume
	UINT32 						ui32RightVol;				///< right channel Volume
} LX_AUD_RENDER_CH_VOLUME_T;

/**
 * Setting of RTS module
 */
typedef struct {
	BOOLEAN						bRTSOnOff;				///< Enable/Disable RTS module

	UINT32						opMode;					///< operation mode. 0: Continuous, 1: Skipping, 2: Hybrid

	UINT32						upperThreshold;			///< upper threashold of buffering (from NORMAL to FAST)
	UINT32						recoverUpper;			///< recover threshold of buffering (from FAST to NORMAL)
	UINT32						lowerThreshold;			///< lower threshold of buffering (from NORMAL to SLOW)
	UINT32						recoverLower;			///< recover threshold of buffering (from SLOW to NORMAL)

	UINT32						skippingThreshold;		///< skipping threshold of buffering (from NORMAL to SKIP)
	UINT32						recoverSkipping;		///< recover threshold of buffering (from KIP to NORMAL)

	UINT32						fastFreq;				///< frequency of FAST
	UINT32						normalFreq;				///< frequency of NORMAL
	UINT32						slowFreq;				///< frequency of SLOW

	UINT32						mixFadeLen;				///< fade length of mixer
	UINT32						mixWaitLen;				///< wait length of mixer (sample)
} LX_AUD_RENDER_RTS_PARAM_T;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KADP_RENDERER_H_ */

/** @} */
