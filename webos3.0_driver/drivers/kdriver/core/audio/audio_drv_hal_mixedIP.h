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



/** @file
 *
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.24
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_HAL_MIXEDIP_H_
#define	_AUDIO_DRV_HAL_MIXEDIP_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "audio_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/* define SPDIF DTO_A Clock Range. */
#define ENLARGE_SPDIF_CLOCK_RANGE

//define APLL FCW default value for 12.288MHZ : low 4 bit and high 2 bit is ignored, use 16 bit value and value and freq. is reversed.
#define APLL_CLK_FCW_VALUE_12_288MHZ	0x046500	//288000
#define APLL_CLK_FCW_VALUE_10_754MHZ	0x050580	//329088 : 288000 + 41088
#define APLL_CLK_FCW_VALUE_13_824MHZ	0x03E800	//325600 : 288000 -32000
#define APLL_DSP_VALUE_12_288MHZ		(APLL_CLK_FCW_VALUE_12_288MHZ >> 4)		//18,000(0x4650)
#define APLL_DSP_VALUE_10_754MHZ		(APLL_CLK_FCW_VALUE_10_754MHZ >> 4)		//20,568(0x5058)
#define APLL_DSP_VALUE_13_824MHZ		(APLL_CLK_FCW_VALUE_13_824MHZ >> 4)		//16,000(0x3E80)

// DTO Rate
#define SPDIF_ES_OUT_DTO_CLOCK_TYPE		2
#define SPDIF_ES_OUT_DTO_CLOCK_NON_HDMI	0
#define SPDIF_ES_OUT_DTO_CLOCK_HDMI		1

#define SPDIF_ES_OUT_FREQ_NUMBER		3
#define SPDIF_ES_OUT_FREQ_32KHZ			2
#define	SPDIF_ES_OUT_FREQ_44_1KHZ		1
#define SPDIF_ES_OUT_FREQ_48KHZ			0

/* define M14B1 DTO_A Clock Normal Value. */
#define M14B1_DTO_A_CLOCK_48KHZ			0x136B06E7
#define M14B1_DTO_A_CLOCK_44_1KHZ		0x11D7218A
#define M14B1_DTO_A_CLOCK_32KHZ			0x0CF2049A

/* define M14B1 DTO_A Clock Step Value. */
#define M14B1_DTO_A_CLOCK_STEP_48KHZ		0x3506		//13574.22
#define M14B1_DTO_A_CLOCK_STEP_44_1KHZ		0x3506		//13574.22
#define M14B1_DTO_A_CLOCK_STEP_32KHZ		0x3506		//13574.22

/* define DTO_A Clock Table Value. */
#ifdef ENLARGE_SPDIF_CLOCK_RANGE
#define DTO_A_CLOCK_CENTER_TAP_VALUE			71
#define DTO_A_CLOCK_MIN_TAP_VALUE				 1
#define DTO_A_CLOCK_MAX_TAP_VALUE			   141
#define DTO_A_TABLE_DIFF_VALUE					60
#define DTO_A_TABLE_MIN_VALUE					61
#define DTO_A_TABLE_MAX_VALUE					81
#define DTO_A_TABLE_LOW_TO_HIGH_VALUE			21
#define DTO_A_TABLE_HIGH_TO_LOW_VALUE		   121
#define DTO_A_CLOCK_LOW_TAP_VALUE				 4
#define DTO_A_CLOCK_HIGH_TAP_VALUE				 4
#else
#define DTO_A_CLOCK_CENTER_TAP_VALUE			71
#define DTO_A_CLOCK_MIN_TAP_VALUE				 1
#define DTO_A_CLOCK_MAX_TAP_VALUE			   141
#define DTO_A_TABLE_DIFF_VALUE					60
#define DTO_A_TABLE_MIN_VALUE					61
#define DTO_A_TABLE_MAX_VALUE					81
#define DTO_A_TABLE_LOW_TO_HIGH_VALUE			21
#define DTO_A_TABLE_HIGH_TO_LOW_VALUE		   121
#define DTO_A_CLOCK_LOW_TAP_VALUE				 4
#define DTO_A_CLOCK_HIGH_TAP_VALUE				 4
#endif

/**
 * AUDIO Clock Source Type.
 * Audio Clock Source type is defined.
 *
 */
typedef  enum
{
	LX_AUD_CLOCK_UNKNOWN	= 0,	//Unknown
	LX_AUD_CLOCK_DTV		= 1,	//DTV to SPK(System)
	LX_AUD_CLOCK_ATV		= 2,	//ATV to SPK
	LX_AUD_CLOCK_HDMI		= 3,	//HDMI to SPK
	LX_AUD_CLOCK_ADC		= 4 	//ADC to SPK
} LX_AUD_CLOCK_SRC_T;


/**
 * ADEC SPDIF DTO Clock Rate Type.
 * Audio SPDIF DTO Clock Rate Type is defined.
 *
 */
typedef  enum
{
	LX_AUD_SPDIF_DTO_UNKNOWN			= 0,	//Unknown
	LX_AUD_SPDIF_DTO_MINUS_19_5SAMPLES	= 1,	//-19.5 Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_12_5SAMPLES	= 2,	//-12.5 Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_10_9SAMPLES	= 3,	//-10.9 Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_9_4SAMPLES	= 4,	//-9.4  Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_7_8SAMPLES	= 5,	//-7.8  Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_6_3SAMPLES	= 6,	//-6.3  Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_4_7SAMPLES	= 7,	//-4.7  Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_3_3SAMPLES	= 8,	//-3.3 Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_1_6SAMPLES	= 9,	//-1.6 Samples Speed
	LX_AUD_SPDIF_DTO_MINUS_0_7SAMPLES	= 10,	//-0.7 Samples Speed
	LX_AUD_SPDIF_DTO_NORMAL				= 11,	//Normal Sample Speed
	LX_AUD_SPDIF_DTO_PLUS_0_7SAMPLES	= 12,	//+0.7 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_1_6SAMPLES	= 13,	//+1.6 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_3_3SAMPLES	= 14,	//+3.3 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_4_7SAMPLES	= 15,	//+4.7 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_6_3SAMPLES	= 16,	//+6.3 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_7_8SAMPLES	= 17,	//+7.8 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_9_4SAMPLES	= 18,	//+9.4 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_10_9SAMPLES	= 19,	//+10.9 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_12_5SAMPLES	= 20,	//+12.5 Samples Speed
	LX_AUD_SPDIF_DTO_PLUS_19_5SAMPLES	= 21,	//+19.5 Samples Speed
	LX_AUD_SPDIF_DTO_RATE_NUMBER		= 21	//Number of DTO Rate Step
} LX_AUD_SPDIF_DTO_RATE_T;

/**
 * AUDIO Mixed IP Power Block Mode.
 * AUDIO Mixed IP Power Block Mode is defined.
 *
 */
typedef  enum
{
	LX_AUD_POWER_AAD 		= 0,	//AAD
	LX_AUD_POWER_ADC		= 1,	//ADC
	LX_AUD_POWER_HPDRV		= 2,	//HPDRV
	LX_AUD_POWER_DAC		= 3,	//DAC(SCART)
	LX_AUD_POWER_MIC		= 4, 	//MIC
	LX_AUD_POWER_MAX		= 5		//Unknown
} LX_AUD_POWER_BLOCK_T;

/**
 * AUDIO Mixed IP Power Mode.
 * AUDIO Mixed IP Power Mode is defined.
 *
 */
typedef  enum
{
	LX_AUD_POWER_DOWN 	= 0,	//Power-down
	LX_AUD_POWER_UP		= 1		//Power-up
} LX_AUD_POWER_MODE_T;

/**
 * AUDIO Mixed IP Power Status
 * AUDIO Mixed IP Power Status is defined.
 *
*/
typedef struct LX_AUD_POWER_STATUS
{
	LX_AUD_POWER_BLOCK_T 	block;		///< AUDIO Mixed IP Power Block Mode.
	LX_AUD_POWER_MODE_T 	mode;		///< AUDIO Mixed IP Power Mode.
} LX_AUD_POWER_STATUS_T;

/**
 * DTO Index.
 * Audio DTO Index is defined.
 *
 */
typedef  enum
{
	AUD_DTO_D,			// 0 : DTO-D
	AUD_DTO_A,			// 1 : DTO-A
	AUD_DTO_MAX,		// 2 : MAX
} AUD_DTO_T;


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
int ANALOG_ReadRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 *value);
int ANALOG_WriteRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 value);

int H13_ANALOG_ReadRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 *value);
int H13_ANALOG_WriteRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 value);

int MIXED_IP_AUDIO_InitClockRegisters( void );
int MIXED_IP_AUDIO_ResetClockRegisters( void );
int MIXED_IP_AUDIO_ResetAADClockRegisters( void );
int MIXED_IP_AUDIO_SetClockSource( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, LX_AUD_CLOCK_SRC_T ui8AdecClockSrc, UINT32 spdifOutput );
int MIXED_IP_AUDIO_SetPcmClockRate ( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, UINT32 ui32PcmClock );
int MIXED_IP_AUDIO_SetSPDIFSamplingFreq ( LX_AUD_SAMPLING_FREQ_T ui32SamplingFreq, LX_AUD_SPDIF_DTO_RATE_T stSpdifDtoRate, AUD_DTO_T dto );

int MIXED_IP_AUDIO_InitCodecRegisters ( void );
int MIXED_IP_AUDIO_SetAADSIFInputPort( LX_AUD_SIF_INPUT_T sifSource );
int MIXED_IP_AUDIO_StartCodec ( void );
int MIXED_IP_AUDIO_StopCodec ( void );

int MIXED_IP_AUDIO_SetADCPortNumber ( UINT8 ui8PortNum );
int MIXED_IP_AUDIO_SetDACOutVolume ( LX_AUD_DAC_VOL_T stDACOutVol );
int MIXED_IP_AUDIO_SetDACOutMute ( LX_AUD_OUTPUT_PORT_MUTE_T stOutPortMute );

LX_AUD_SPDIF_DTO_RATE_T MIXED_IP_AUDIO_GetDTORate(LX_AUD_CLOCK_SRC_T clockSrc, LX_AUD_SAMPLING_FREQ_T samplingFreq, AUD_DTO_T dto);
void MIXED_IP_AUDIO_SetDTORate(LX_AUD_CLOCK_SRC_T clockSrc, LX_AUD_SAMPLING_FREQ_T samplingFreq, AUD_DTO_T dto, LX_AUD_SPDIF_DTO_RATE_T dtoRate);
void MIXED_IP_AUDIO_SetDTOIntSrc(LX_AUD_CLOCK_SRC_T clockSrc, UINT32 spdifOutput);
void MIXED_IP_AUDIO_StartDTO(LX_AUD_CLOCK_SRC_T clockSrc, UINT32 spdifOutput);
void MIXED_IP_AUDIO_SetDTOIntSamplingFreq(LX_AUD_CLOCK_SRC_T clockSrc, UINT32 spdifOutput, LX_AUD_SAMPLING_FREQ_T samplingFreq);
int MIXED_IP_AUDIO_SetSpeakerFreq(LX_AUD_SAMPLING_FREQ_T samplingFreq);
LX_AUD_SAMPLING_FREQ_T MIXED_IP_AUDIO_GetSpeakerFreq(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_HAL_MIXEDIP_H_ */

/** @} */
