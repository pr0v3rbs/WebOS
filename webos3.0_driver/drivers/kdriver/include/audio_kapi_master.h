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


/** @file audio_kadp_master.h
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

#ifndef	_AUDIO_KAPI_MASTER_H_
#define _AUDIO_KAPI_MASTER_H_

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
#define	AUD_MASTER_IOC_MAGIC		'M'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
/* IO Command Name */
#define AUD_MASTER_IOW_SET_VOLUME						_IOW(AUD_MASTER_IOC_MAGIC,  0, LX_AUD_MASTER_CONFIG_VOLUME_T)
#define AUD_MASTER_IOW_SET_MUTE							_IOW(AUD_MASTER_IOC_MAGIC,  1, LX_AUD_MASTER_CONFIG_MUTE_T)
#define AUD_MASTER_IOW_SET_DELAY						_IOW(AUD_MASTER_IOC_MAGIC,  2, LX_AUD_MASTER_CONFIG_DELAY_T)
#define AUD_MASTER_IOW_SET_SPDIF_OUTPUT					_IOW(AUD_MASTER_IOC_MAGIC,  3, LX_AUD_MASTER_SPDIF_OUTPUT_T)
#define AUD_MASTER_IOW_SET_SPDIF_SCMS					_IOW(AUD_MASTER_IOC_MAGIC,  4, LX_AUD_MASTER_SPDIF_SCMS_T)
#define AUD_MASTER_IOW_SET_BALANCE						_IOW(AUD_MASTER_IOC_MAGIC,  5, LX_AUD_MASTER_CONFIG_BALANCE_T)
#define AUD_MASTER_IOW_SET_BASS							_IOW(AUD_MASTER_IOC_MAGIC,  6, LX_AUD_MASTER_CONFIG_BASS_T)
#define AUD_MASTER_IOW_SET_TREBLE						_IOW(AUD_MASTER_IOC_MAGIC,  7, LX_AUD_MASTER_CONFIG_TREBLE_T)
#define AUD_MASTER_IOW_SET_ADC_INPUT_PORT				_IOW(AUD_MASTER_IOC_MAGIC,  8, UINT32)

#define AUD_MASTER_IOW_SET_SPK_OUTPUT					_IOW(AUD_MASTER_IOC_MAGIC, 10, LX_AUD_MASTER_SPK_OUTPUT_T)
#define AUD_MASTER_IOW_SET_MIXER_VOLUME					_IOW(AUD_MASTER_IOC_MAGIC, 12, LX_AUD_MASTER_CONFIG_MIXER_VOLUME_T)
#define AUD_MASTER_IOW_SET_OUT_MODE						_IOW(AUD_MASTER_IOC_MAGIC, 13, LX_AUD_MASTER_CONFIG_OUT_MODE_T)
#define AUD_MASTER_IOW_SET_DAC_VOLUME					_IOW(AUD_MASTER_IOC_MAGIC, 14, LX_AUD_MASTER_CONFIG_DAC_VOLUME_T)
#define AUD_MASTER_IOW_SET_DAC_MUTE						_IOW(AUD_MASTER_IOC_MAGIC, 15, LX_AUD_MASTER_CONFIG_DAC_MUTE_T)
#define AUD_MASTER_IOW_SET_SE_FN						_IOW(AUD_MASTER_IOC_MAGIC, 16, LX_AUD_MASTER_PARAM_SE_FN_T)
#define AUD_MASTER_IOR_GET_KDRV_HANDLE					_IOR(AUD_MASTER_IOC_MAGIC, 17, UINT32)

#define AUD_MASTER_IOW_CONNECT_OUTPUT					_IOW(AUD_MASTER_IOC_MAGIC, 18, LX_AUD_MASTER_CONNECT_OUTPUT_T)
#define AUD_MASTER_IOW_DISCONNECT_OUTPUT				_IOW(AUD_MASTER_IOC_MAGIC, 19, LX_AUD_MASTER_DISCONNECT_OUTPUT_T)
#define AUD_MASTER_IOR_GET_SPDIF_OUTPUT					_IOR(AUD_MASTER_IOC_MAGIC, 20, LX_AUD_MASTER_SPDIF_OUTPUT_T)
#define AUD_MASTER_IOWR_GET_SE_FN						_IOWR(AUD_MASTER_IOC_MAGIC,21, LX_AUD_MASTER_PARAM_SE_FN_T)

#define AUD_MASTER_IOW_ENABLE_SOUNDBAR					_IOW(AUD_MASTER_IOC_MAGIC, 22, LX_AUD_MASTER_SOUNDBAR_T)
#define AUD_MASTER_IOW_SET_SOUNDBAR_PARAM				_IOW(AUD_MASTER_IOC_MAGIC, 23, LX_AUD_MASTER_SOUNDBAR_PARAM_T)
#define AUD_MASTER_IOR_GET_SOUNDBAR_STATUS				_IOR(AUD_MASTER_IOC_MAGIC, 24, LX_AUD_MASTER_SOUNDBAR_STATUS_T)

#define AUD_MASTER_IOW_SET_SPDIF_ONOFF					_IOW(AUD_MASTER_IOC_MAGIC, 26, UINT32)
#define AUD_MASTER_IOW_SET_SOUNDBAR_POWER				_IOW(AUD_MASTER_IOC_MAGIC, 27, UINT32)

#define AUD_MASTER_IOW_SET_I2S_NUM						_IOW(AUD_MASTER_IOC_MAGIC, 28, UINT32)
#define AUD_MASTER_IOW_SET_SPDIF_CATEGORY_CODE			_IOW(AUD_MASTER_IOC_MAGIC, 29, UINT32)

/* Get GSTC */
#define AUD_MASTER_IOR_GET_GSTC							_IOR(AUD_MASTER_IOC_MAGIC, 30, UINT64)

/* Get SE Param */
#define AUD_MASTER_IOWR_GET_SE_PARAM					_IOWR(AUD_MASTER_IOC_MAGIC,31, LX_AUD_MASTER_PARAM_SE_FN_T)

/* Event */
#define AUD_MASTER_IOW_SET_EVENT						_IOW(AUD_MASTER_IOC_MAGIC, 40, LX_AUD_EVENT_TYPE_T)
#define AUD_MASTER_IORW_GET_EVENT						_IOWR(AUD_MASTER_IOC_MAGIC,41, LX_AUD_EVENT_TYPE_T)

/* Sound Bar & SPDIF */
#define AUD_MASTER_IOW_SET_SOUNDBAR_CMD					_IOW(AUD_MASTER_IOC_MAGIC, 50, LX_AUD_MASTER_SOUNDBAR_CMD_T)
#define AUD_MASTER_IOW_SET_MAIN_DECODER					_IOW(AUD_MASTER_IOC_MAGIC, 51, UINT32)
#define AUD_MASTER_IOR_GET_MAIN_DECODER					_IOR(AUD_MASTER_IOC_MAGIC, 52, UINT32)

/* AAD */
#define AUD_MASTER_IO_RESET_AAD_MODULE					_IO( AUD_MASTER_IOC_MAGIC, 60)
#define AUD_MASTER_IO_SET_AAD_SIF_INPUT					_IOW(AUD_MASTER_IOC_MAGIC, 61, LX_AUD_SIF_INPUT_T)

/* Get Delay */
#define AUD_MASTER_IOR_GET_DELAY						_IOWR(AUD_MASTER_IOC_MAGIC, 70, UINT32)
#define AUD_IOWR_GET_INDEX_DELAY						_IOWR(AUD_MASTER_IOC_MAGIC, 71, LX_AUD_GET_INDEX_DELAY_T)

/* Set Volume/Delay for index */
#define AUD_IOWR_GET_DEV_INDEX_CTX						_IOWR(AUD_MASTER_IOC_MAGIC,72, LX_AUD_DEV_INDEX_CTX_T)
#define AUD_IOW_SET_INPUT_VOLUME						_IOW(AUD_MASTER_IOC_MAGIC, 73, LX_AUD_SET_INPUT_VOLUME_T)
#define AUD_IOW_SET_INPUT_MUTE							_IOW(AUD_MASTER_IOC_MAGIC, 74, LX_AUD_SET_INPUT_MUTE_T)
#define AUD_IOW_SET_INPUT_DELAY							_IOW(AUD_MASTER_IOC_MAGIC, 75, LX_AUD_INPUT_DELAY_T)
#define AUD_IOR_GET_START_INFO							_IOR(AUD_MASTER_IOC_MAGIC, 76, LX_AUD_GET_START_INFO_T)
#define AUD_IOWR_GET_INPUT_DELAY						_IOWR(AUD_MASTER_IOC_MAGIC, 77, LX_AUD_INPUT_DELAY_T)

/* Connect/Disconnect Index to Output */
#define AUD_MASTER_IOW_CONNECT_INDEX_OUTPUT				_IOW(AUD_MASTER_IOC_MAGIC, 80, LX_AUD_MASTER_CONNECT_INDEX_OUTPUT_T)
#define AUD_MASTER_IOW_DISCONNECT_INDEX_OUTPUT			_IOW(AUD_MASTER_IOC_MAGIC, 81, LX_AUD_MASTER_DISCONNECT_INDEX_OUTPUT_T)

/* Download DSP Image for test */
#define AUD_MASTER_DOWNALOD_DSP0						_IOW(AUD_MASTER_IOC_MAGIC, 180, LX_AUD_CODEC_T)
#define AUD_MASTER_DOWNALOD_DSP1						_IOW(AUD_MASTER_IOC_MAGIC, 181, LX_AUD_CODEC_T)

/* Debug control */
#define AUD_IOWR_READ_AND_WRITE_REG						_IOWR(AUD_MASTER_IOC_MAGIC, 190, LX_AUD_REG_INFO_T)
#define AUD_IOW_CUSTOM_IMC								_IOWR(AUD_MASTER_IOC_MAGIC, 191, LX_AUD_CUSTOM_IMC_T)
#define AUD_IO_DEBUG_DEV_INFO							_IO(AUD_MASTER_IOC_MAGIC,   192)
#define AUD_IOW_DEBUG_PRINT_CTRL						_IOW(AUD_MASTER_IOC_MAGIC,  193, LX_AUD_DEBUG_CTRL_T)
#define AUD_IOW_DEBUG_MEM_DUMP							_IOW(AUD_MASTER_IOC_MAGIC,  194, LX_AUD_DEBUG_MEM_DUMP_T)
#define AUD_IO_DEBUG_ALLOC_INFO							_IO(AUD_MASTER_IOC_MAGIC,   195)
#define AUD_IOWR_DEBUG_GET_MOD_INFO						_IOWR(AUD_MASTER_IOC_MAGIC, 196, LX_AUD_DEBUG_MOD_INFO_T)
#define AUD_IOR_GET_FW_DEBUG_PRINT_INFO					_IOR(AUD_MASTER_IOC_MAGIC,	197, LX_AUD_FW_DEBUG_INFO_T)
#define AUD_IOW_SET_FW_DEBUG_MASK						_IOW(AUD_MASTER_IOC_MAGIC,	198, UINT32)
#define AUD_IOWR_GET_DECODED_INFO					_IOWR(AUD_MASTER_IOC_MAGIC,	199, LX_AUD_GET_DECODED_INFO_T)


#define AUD_MASTER_IOC_MAXNR							200

#define NUM_OF_START_INFO								14	// DEC ( 2)+ REN(12)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO MASTER Output Port.
 */
typedef enum {
	LX_AUD_MASTER_OUTPUT_SPEAKER, 						///< Speaker output
	LX_AUD_MASTER_OUTPUT_HP, 							///< Headphone output
	LX_AUD_MASTER_OUTPUT_SCART,							///< Audio SCART output
	LX_AUD_MASTER_OUTPUT_SPDIF,							///< SPDIF output
	LX_AUD_MASTER_OUTPUT_MAX							///< the max marker of this enumeration
} LX_AUD_MASTER_OUTPUT_T;

/**
 * AUDIO MASTER Output Mode.
 */
typedef enum {
	LX_AUD_MASTER_OUT_LR, 								///< Left Right Output
	LX_AUD_MASTER_OUT_LL, 								///< Left Left output
	LX_AUD_MASTER_OUT_RR,								///< Right Right output
	LX_AUD_MASTER_OUT_MIX,								///< Left Right Mixe output
	LX_AUD_MASTER_OUT_MODE_MAX, 						///< the max marker of this enumeration
} LX_AUD_MASTER_OUT_MODE_T;

/**
 * AUDIO MASTER SPDIF Output Type.
 */
typedef enum {
	LX_AUD_MASTER_SPDIF_PCM				=	0,			///< SPDIF PCM Output
	LX_AUD_MASTER_SPDIF_ES				=	1,			///< SPDIF ES Output
} LX_AUD_MASTER_SPDIF_OUTPUT_T;

/**
 * AUDIO MASTER SPDIF SCMS(Serial Copy Management System) state.
 */
typedef enum {
	LX_AUD_MASTER_SPDIF_COPY_FREE		=	0,			///< Copy Free state
	LX_AUD_MASTER_SPDIF_COPY_NO_MORE	=	1,			///< Copy no more state
	LX_AUD_MASTER_SPDIF_COPY_ONCE		=	2,			///< Copy once state
	LX_AUD_MASTER_SPDIF_COPY_NEVER		=	3,			///< Copy never state
} LX_AUD_MASTER_SPDIF_SCMS_T;

/**
 * AUDIO MASTER SE Function Mode.
 */
typedef enum {
	LX_AUD_MASTER_SE_FN_000				=	0,			///< SDV
	LX_AUD_MASTER_SE_FN_001				=	1,			///< AGC
	LX_AUD_MASTER_SE_FN_002				=	2,			///< DB
	LX_AUD_MASTER_SE_FN_003				=	3,			///< DEQ
	LX_AUD_MASTER_SE_FN_004 			=	4,			///< SSM
	LX_AUD_MASTER_SE_FN_005				=	5,			///< SO
	LX_AUD_MASTER_SE_FN_006				=	6,			///< not used
	LX_AUD_MASTER_SE_FN_007				=	7,			///< not used
	LX_AUD_MASTER_SE_FN_008				=	8,			///< HC
	LX_AUD_MASTER_SE_FN_009				=	9,			///< Sound Engine Volume
	LX_AUD_MASTER_SE_FN_010				=	10,			///< INCHECK
	LX_AUD_MASTER_SE_FN_011 			=	11,			///< IVSE
	LX_AUD_MASTER_SE_FN_012 			=	12,			///< not used
	LX_AUD_MASTER_SE_FN_013 			=	13,			///< NE
	LX_AUD_MASTER_SE_FN_014 			=	14,			///< ASC
	LX_AUD_MASTER_SE_FN_015 			=	15,			///< not used
	LX_AUD_MASTER_SE_FN_016 			=	16,			///< ELC
	LX_AUD_MASTER_SE_FN_017 			=	17, 		///< SXP
	LX_AUD_MASTER_SE_FN_019 			=	19, 		///< DRC
	LX_AUD_MASTER_SE_FN_020 			=	20, 		///< SPEAKEREQ
	LX_AUD_MASTER_SE_FN_022 			=	22, 		///< SSC
	LX_AUD_MASTER_SE_FN_023 			=	23, 		///< Reverberator
	LX_AUD_MASTER_SE_FN_024 			=	24, 		///< Harmonizer
	LX_AUD_MASTER_SE_FN_004_MODE1		=	41,			///< PEQ mode 1
	LX_AUD_MASTER_SE_FN_004_MODE2		=	42,			///< PEQ mode 2
	LX_AUD_MASTER_SE_FN_004_MODE3		=	43,			///< PEQ mode 3
	LX_AUD_MASTER_SE_FN_GET				=	100,		///< mode to get SE inforamtion
	LX_AUD_MASTER_SE_MAIN 				=	101,		///< MAIN
	LX_AUD_MASTER_SE_FN_SET 			=	102,		///< mode to set functions
	LX_AUD_MASTER_SE_FN_UPDATE_PARAM	=	103, 		///< mode to update setted parameters for reset
} LX_AUD_MASTER_SE_FN_T;

/**
 * AUDIO MASTER Sound Engine Data Option.
 */
typedef enum {
	LX_AUD_MASTER_SE_DATA_INIT_ONLY,					///< Initial Value only
	LX_AUD_MASTER_SE_DATA_VARIABLES,					///< Variables
	LX_AUD_MASTER_SE_DATA_ALL,							///< All
} LX_AUD_MASTER_SE_DATA_T;

/**
 * AUDIO MASTER Sound Engine Variable Option.
 */
typedef enum {
	LX_AUD_MASTER_SE_VAR_00,							///< Variable 0
	LX_AUD_MASTER_SE_VAR_01,							///< Variable 1
	LX_AUD_MASTER_SE_VAR_02,							///< Variable 2
	LX_AUD_MASTER_SE_VAR_03,							///< Variable 3
	LX_AUD_MASTER_SE_VAR_04,							///< Variable 4
	LX_AUD_MASTER_SE_VAR_ALL,							///< Variable All
} LX_AUD_MASTER_SE_VAR_T;

/**
 * AUDIO MASTER Sound Engine Data Access Mode.
 */
typedef enum {
	LX_AUD_MASTER_SE_ACCESS_WRITE,						///< Write Mode
	LX_AUD_MASTER_SE_ACCESS_READ,						///< Read Mode
} LX_AUD_MASTER_SE_ACCESS_T;

/**
 * AUDIO MASTER Sound Bar Command Mode.
 */
typedef enum {
	LX_AUD_MASTER_SB_CMD_AUTO_VOLUME,					///< Auto Volume Mode
	LX_AUD_MASTER_SB_CMD_WOOFER_LEVEL,					///< Woofer Level Mode
	LX_AUD_MASTER_SB_CMD_WIRELESS_RESET,				///< Wireless Reset Mode
} LX_AUD_MASTER_CMD_MODE_T;

/**
 * AUDIO Mixed IP Power Block.
 *
 */
typedef  enum
{
	LX_AUD_MASTER_POWER_AAD,							///< Mixed IP AAD Power Block
	LX_AUD_MASTER_POWER_ADC,							///< Mixed IP ADC Power Block
	LX_AUD_MASTER_POWER_HPDRV,							///< Mixed IP Headphone Drive Power Block
	LX_AUD_MASTER_POWER_DAC,							///< Mixed IP DAC Power Block
	LX_AUD_MASTER_POWER_MIC,							///< Mixed IP Mic Power Block
	LX_AUD_MASTER_POWER_MAX,							///< the max marker of this enumeration
} LX_AUD_MASTER_POWER_BLOCK_T;

/**
 * AUDIO Mixed IP Power Mode.
 *
 */
typedef  enum
{
	LX_AUD_MASTER_POWER_DOWN,							///< Power down
	LX_AUD_MASTER_POWER_UP,								///< Power up
} LX_AUD_MASTER_POWER_MODE_T;

/**
 * AUDIO MASTER Speaker Output Source.
 */
typedef enum {
	LX_AUD_MASTER_SPK_NONE,								///< none
	LX_AUD_MASTER_SPK_PCM,								///< PCM data of DSP1. default
	LX_AUD_MASTER_SPK_PCM_0,							///< PCM data of DSP0
} LX_AUD_MASTER_SPK_OUTPUT_T;

/**
 * Audio Mixer ID.
 */
typedef  enum
{
	LX_AUD_MASTER_MIXER_0,								///< Mixer 0
	LX_AUD_MASTER_MIXER_1,								///< Mixer 1
	LX_AUD_MASTER_MIXER_2,								///< Mixer 2
	LX_AUD_MASTER_MIXER_3,								///< Mixer 3
	LX_AUD_MASTER_MIXER_4,								///< Mixer 4
	LX_AUD_MASTER_MIXER_5,								///< Mixer 5
	LX_AUD_MASTER_MIXER_6,								///< Mixer 6
	LX_AUD_MASTER_MIXER_7,								///< Mixer 7
	LX_AUD_MASTER_MIXER_MAX,							///< the max marker of this enumeration
} LX_AUD_MASTER_MIXER_T;

/**
 * AUDIO MASTER SE Function Parameters.
 */
typedef struct {
	LX_AUD_MASTER_SE_FN_T			fnMode;				///< function mode
	LX_AUD_MASTER_SE_DATA_T			dataOption;			///< data option
	LX_AUD_MASTER_SE_VAR_T			varOption;			///< variable option
	LX_AUD_MASTER_SE_ACCESS_T		accessMode;			///< access mode
	UINT32							*ui32Param;			///< the pointer to parameters
	UINT32							ui32NoParam;		///< the number of parameters
} LX_AUD_MASTER_PARAM_SE_FN_T;

/**
 * AUDIO MASTER Volume Configuration.
 */
typedef struct {
	LX_AUD_MASTER_OUTPUT_T			output;				///< output
	UINT32							ui32Volume;			///< gain. value = origin * gain. For example, if 0x800000, gain is 1, if 0x400000, 0.5.
} LX_AUD_MASTER_CONFIG_VOLUME_T;

/**
 * AUDIO MASTER DAC Volume Configuration.
 */
typedef struct {
	LX_AUD_OUTPUT_PORT_T			dacPort;			///< DAC port
	UINT32							ui32Volume;			///< gain. value = origin * gain. For example, if 0x800000, gain is 1, if 0x400000, 0.5.
} LX_AUD_MASTER_CONFIG_DAC_VOLUME_T;

/**
 * AUDIO MASTER Mute Configuration.
 */
typedef struct {
	LX_AUD_MASTER_OUTPUT_T			output;				///< output
	UINT32							ui32Mute;			///< mute on or off
} LX_AUD_MASTER_CONFIG_MUTE_T;

/**
 * AUDIO MASTER DAC Mute Configuration.
 */
typedef struct {
	LX_AUD_OUTPUT_PORT_T			dacPort;			///< DAC port
	UINT32							ui32Mute;			///< mute on or off
	LX_AUD_DAC_MODE_T				mode;				///< DAC mode
} LX_AUD_MASTER_CONFIG_DAC_MUTE_T;

/**
 * AUDIO MASTER Delay Time Configuration.
 */
typedef struct {
	LX_AUD_MASTER_OUTPUT_T			output;				///< output
	UINT32							ui32Delay;			///< delay. unit is milisecond.
} LX_AUD_MASTER_CONFIG_DELAY_T;

/**
 * AUDIO MASTER Output Mode Configuration.
 */
typedef struct {
	LX_AUD_MASTER_OUTPUT_T		output;					///< output
	LX_AUD_MASTER_OUT_MODE_T	outMode;				///< output mode
} LX_AUD_MASTER_CONFIG_OUT_MODE_T;

/**
 * AUDIO MASTER SPDIF Output Type Configuration.
 */
typedef struct {
	LX_AUD_MASTER_SPDIF_OUTPUT_T	spdifOutput;		///< SPDIF output type
} LX_AUD_MASTER_CONFIG_SPDIF_OUTPUT_T;

/**
 * AUDIO MASTER SPDIF SCMS Configuration.
 */
typedef struct {
	LX_AUD_MASTER_SPDIF_SCMS_T	spdifScms;				///< SPDIF SCMS state
} LX_AUD_MASTER_CONFIG_SPDIF_SCMS_T;

/**
 * AUDIO MASTER BALANCE Configuration.
 */
typedef struct {
	LX_AUD_MASTER_OUTPUT_T			output;				///< output
	UINT32							ui32Balance;		///< balance. 0 ~ 100. if 0, left speaker only outputs, if 100, right only.
} LX_AUD_MASTER_CONFIG_BALANCE_T;

/**
 * AUDIO MASTER BASS Configuration. not used.
 */
typedef struct {
	UINT32							ui32Bass;			///< bass
} LX_AUD_MASTER_CONFIG_BASS_T;

/**
 * AUDIO MASTER TREBLE Configuration.not used.
 */
typedef struct {
	UINT32							ui32Treble;			///< treble
} LX_AUD_MASTER_CONFIG_TREBLE_T;

/**
 * AUDIO Mixed IP Power Structure.
 */
typedef struct {
	LX_AUD_MASTER_POWER_BLOCK_T			powerBlock;		///< mixed ip power block
	LX_AUD_MASTER_POWER_MODE_T			powerMode;		///< mixed ip power mode
} LX_AUD_MASTER_CONFIG_POWER_CTRL_T;

/**
 * AUDIO MASTER Speaker Output source Configuration.
 */
typedef struct {
	LX_AUD_MASTER_SPK_OUTPUT_T	spkOutput;				///< speaker output source.
	UINT32						ui32I2SNum;				///< number of I2S.
	LX_AUD_SAMPLING_FREQ_T		samplingFreq;			///< sampling Frequency.
} LX_AUD_MASTER_CONFIG_SPK_OUTPUT_T;

/**
 * AUDIO MASTER Mixer Delay Time Configuration. not used.
 */
typedef struct {
	LX_AUD_MASTER_MIXER_T			mixer;				///< mixer id.
	UINT32							ui32Port;			///< mixer input port.
	UINT32							ui32Delay;			///< delay. unit is miliseconds.
} LX_AUD_MASTER_CONFIG_MIXER_DELAY_T;

/**
 * AUDIO MASTER Mixer Volume Configuration. not used.
 */
typedef struct {
	LX_AUD_MASTER_MIXER_T			mixer;				///< mixer id.
	UINT32							ui32Port;			///< mixer input port.
	UINT32							ui32Volume;			///< volume. 0 ~ 1024.
} LX_AUD_MASTER_CONFIG_MIXER_VOLUME_T;

/**
 * AUDIO MASTER Connect Configuration.
 */
typedef struct {
	UINT32						ui32KdrHandler;			///< allocated device.
	LX_AUD_MASTER_OUTPUT_T		output;					///< output
	UINT32						ui32UseSE;				///< use Sound Engine or not
} LX_AUD_MASTER_CONNECT_OUTPUT_T;

/**
 * AUDIO MASTER Disconnect Configuration.
 */
typedef struct {
	UINT32						ui32KdrHandler;			///< allocated device.
	LX_AUD_MASTER_OUTPUT_T		output;					///< output
} LX_AUD_MASTER_DISCONNECT_OUTPUT_T;

/**
 * AUDIO MASTER SOUND BAR PARAM.
 */
typedef struct {
	UINT32						ui32ID;					///< Sound Bar id
	UINT32						ui32Data;				///< Sound Bar data. 8th bit is mute, 0~7th bits are volume.
} LX_AUD_MASTER_SOUNDBAR_PARAM_T;

/**
 * AUDIO MASTER SOUND BAR COMMAND PARAM.
 */
typedef struct {
	LX_AUD_MASTER_CMD_MODE_T	cmdMode;			///< command mode
	UINT32						ui32CmdParam;		///< the command of parameters
} LX_AUD_MASTER_SOUNDBAR_CMD_T;

/**
 * AUDIO MASTER SOUND BAR/WIRELESS/FRAME SET STATUS.
 */
typedef struct {
	UINT32						ui32SoundBarOnOff;		///< Sound Bar on or not.
	UINT32						ui32WirelessOnOff;		///< Wireless Sound Bar on or not.
	UINT32						ui32SoundFrameOnOff;	///< Sound Frame on or not.
} LX_AUD_MASTER_SOUNDBAR_T;

/**
 * AUDIO MASTER SOUND BAR STATUS.
 */
typedef struct {
	UINT32						ui32SoundBarOnOff;		///< Sound Bar on or not.
	UINT32						ui32SubFrameID;			///< subframe id.
	UINT32						ui32SubFrameData;		///< subframe data.
	UINT32						ui32SubFrameCheckSum;	///< subframe checksum.
	UINT32						ui32WirelessOnOff;		///< Wireless Sound Bar on or not.
	UINT32						ui32SoundFrameOnOff;	///< Sound Frame on or not.
} LX_AUD_MASTER_SOUNDBAR_STATUS_T;

/**
 * AUDIO MASTER Connect Index to Output Configuration.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	LX_AUD_MASTER_OUTPUT_T			output;					///< output
	LX_AUD_MASTER_SPDIF_OUTPUT_T	spdifOutput;			///< spdif output
} LX_AUD_MASTER_CONNECT_INDEX_OUTPUT_T;

/**
 * AUDIO MASTER Disconnect Index to Output Configuration.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	LX_AUD_MASTER_OUTPUT_T			output;					///< output
	LX_AUD_MASTER_SPDIF_OUTPUT_T	spdifOutput;			///< spdif output
} LX_AUD_MASTER_DISCONNECT_INDEX_OUTPUT_T;

/**
 * Set input volume for index.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	UINT32							ui32Volume;				///< volume
} LX_AUD_SET_INPUT_VOLUME_T;

/**
 * Set input mute for index.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	UINT32							ui32Mute;				///< mute
	LX_AUD_DEV_OUT_TYPE_T			outType;				///< device output type.
} LX_AUD_SET_INPUT_MUTE_T;

/**
 * Set input delay for index.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	UINT32							ui32Delay;				///< delay
	LX_AUD_DEV_OUT_TYPE_T			outType;				///< device output type.
} LX_AUD_INPUT_DELAY_T;

/**
 * AUDIO MASTER Get Index Delay Configuration.
 */
typedef struct {
	UINT32							ui32Index;				///< allocated device.
	LX_AUD_DEV_TYPE_T				devType;				///< output
	UINT32							bLipsyncOnOff;			///< lipsync On
	UINT32							ui32Delay;				///< device delay time(ms)
} LX_AUD_GET_INDEX_DELAY_T;

/**
 * device started or not information.
 */
typedef struct {
	LX_AUD_DEV_TYPE_T				devType;				///< device type
	LX_AUD_DEV_T						dev;				///< device id
	LX_AUD_DEV_OUT_TYPE_T			devOutType;				///< device output type
	UINT32							ui32Index;				///< device index
	UINT32							bStarted;				///< device status is started or not
} LX_AUD_START_INFO_T;

/**
 * Get devices' started or not information.
 */
typedef struct {
	LX_AUD_START_INFO_T				startInfo[NUM_OF_START_INFO];				///< device started info
} LX_AUD_GET_START_INFO_T;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KAPI_MASTER_H_ */

/** @} */
