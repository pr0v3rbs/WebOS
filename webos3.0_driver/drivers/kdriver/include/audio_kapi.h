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


/** @file audio_kadp.h
 *
 *  This is a wrapper function file for AUD_ddi.c to process AUDIO function.
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

#ifndef	_AUDIO_KAPI_H_
#define	_AUDIO_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
// The Size of AUD KDRV IPC Command
#define LX_IPC_ARRAY_SIZE 					20
#define LX_AUD_FILENAME_SIZE 				40
#define LX_AUD_MOD_NAME_SIZE				10


#undef IC_CERTIFICATION

#undef ENABLE_SE_96KHZ_FOR_H15_ONLY


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO Device Type Index.
 */
typedef enum {
	LX_AUD_DEV_TYPE_NONE,		//	0,
	LX_AUD_DEV_TYPE_DEC,		//	1,
	LX_AUD_DEV_TYPE_ENC,		//	2,
	LX_AUD_DEV_TYPE_CAP,		//	3,
	LX_AUD_DEV_TYPE_REN,		//	4,
	LX_AUD_DEV_TYPE_MAS,		//	5,
	LX_AUD_DEV_TYPE_MAX,
} LX_AUD_DEV_TYPE_T;

/**
 * AUDIO Allocated Device Index.
 * refer "module/adec_module_def.h"'s module defines.
 */
typedef enum {
	LX_AUD_DEV_NONE		=   0,
	LX_AUD_DEV_DEC0		=	1,
	LX_AUD_DEV_DEC1		=	2,
	LX_AUD_DEV_ENC0		=	3,
	LX_AUD_DEV_ENC1		=	4,
	LX_AUD_DEV_CAP0		=	5,
	LX_AUD_DEV_CAP1		=	6,
	LX_AUD_DEV_CAP2		=	7,
	LX_AUD_DEV_CAP3		=	8,
	LX_AUD_DEV_CAP4		=	9,
	LX_AUD_DEV_CAP5		=  10,
	LX_AUD_DEV_REN0		=  11,
	LX_AUD_DEV_REN1		=  12,
	LX_AUD_DEV_REN2		=  13,
	LX_AUD_DEV_REN3		=  14,
	LX_AUD_DEV_REN4		=  15,
	LX_AUD_DEV_REN5		=  16,
	LX_AUD_DEV_REN6		=  17,
	LX_AUD_DEV_REN7		=  18,
	LX_AUD_DEV_REN8		=  19,
	LX_AUD_DEV_REN9		=  20,
	LX_AUD_DEV_REN10	=  21,
	LX_AUD_DEV_REN11	=  22,
	LX_AUD_DEV_MAS0		=  23,
	LX_AUD_DEV_MAX		=  24,
	//To Do: need to cover all device(resource) in adec_module_def.h
} LX_AUD_DEV_T;

/**
 * AUDIO Input Port.
 */
typedef enum {
	LX_AUD_INPUT_NONE			=	0,
	LX_AUD_INPUT_TP0			=	1,
	LX_AUD_INPUT_TP1			=	2,
	LX_AUD_INPUT_SIF			=	3,
	LX_AUD_INPUT_ADC			=	4,
	LX_AUD_INPUT_HDMI			=	5,
	LX_AUD_INPUT_I2S			=	6,
	LX_AUD_INPUT_SYSTEM			=	7,
	LX_AUD_INPUT_SYSTEM_CLIP	=	8,
	LX_AUD_INPUT_HDMI0			=	9,
	LX_AUD_INPUT_HDMI1			=  10,
	LX_AUD_INPUT_HDMI2			=  11,
	LX_AUD_INPUT_HDMI3			=  12,
	LX_AUD_INPUT_MAX,
} LX_AUD_INPUT_T;

/**
 * AUDIO SIF Input Source Type.
 *
 */
typedef enum
{
	LX_AUD_SIF_INPUT_EXTERNAL	= 0,			///< External SIF Input(GBB)
	LX_AUD_SIF_INPUT_INTERNAL	= 1,			///< Internal SIF Input(ABB)
} LX_AUD_SIF_INPUT_T;

/**
 * Audio Event Message Callback Status.
 * This enumeration describes the AUDIO callback status.
 *
 */
typedef  enum
{
	LX_AUD_EVENT_MASK_NONE			= 0x0000,		///< None
	LX_AUD_EVENT_DSP_RESET			= 0x0001,		///< Event => DECx, ENCx, CAPx : DSP Reset Event, Notify to app. after DSP Reset(Request playing again)
	LX_AUD_EVENT_PRESENT_END		= 0x0002,		///< Event => RENx : Render Present End Event, Notify to app. after Render is completed.
	LX_AUD_EVENT_UNDERFLOW			= 0x0004,		///< Event => RENx : Underflow Event, Notify to app. after Render is in underflow.
	LX_AUD_EVENT_DEC_ERROR			= 0x0008,		///< Event => DECx : Decoder Error Event, Notify to app. after Decoder is in error.
	LX_AUD_EVENT_DEC_AAC51_DUAL		= 0x0010,		///< Event => DECx : Decoder AAC 5.1CH Event, Notify to app. when dual decoding is in error.(DSP Performance Issue)
	LX_AUD_EVENT_DEC_PCM_ONLY		= 0x0020,		///< Event => RENx : Render AAC/DTS SPDIF PCM Event, Notify to app. when decoder is not available for SPDIF ES output.(Not 48Khz. or DTS ES Size Big)
	LX_AUD_EVENT_DEC_AAC_CODEC		= 0x0040,		///< Event => DECx : Decoder AAC/HEAAC Codec Event, Notify to app. when decoded TP stream is AAC codec detected..
	LX_AUD_EVENT_SPDIF_CHANGED		= 0x0080,		///< Event => RENx : SPDIF Event, Render Wireless Sound Bar Event, Notify to app. when wireless sound bar is enabled or not for changing Lip Sync Offset.
	LX_AUD_EVENT_1KHZ_TONE_ON		= 0x0100,		///< Event => MAS  : 1KHZ Tone On Detected Event, 1KHz Tone is detected.
	LX_AUD_EVENT_1KHZ_TONE_OFF		= 0x0200,		///< Event => MAS  : 1KHZ Tone Off Detected Event, 1KHz Tone is un-detected.
	LX_AUD_EVENT_DEC_DECODED_INFO	= 0x0400,		///< Event => DECx : Decoder Decoding Info. Event, Notify to app. after Decoder is started.
	LX_AUD_EVENT_DECODED_END		= 0x0800,		///< Event => DECx : Decoder Decoded End Event, Notify to app. after Decoder Decoding is completed.
	LX_AUD_EVENT_MASK_ALL			= 0xFFFF,		///< Event => All event.
} LX_AUD_EVENT_MSG_T;

/**
 * Audio Output Port Number.
 * Audio output port number is defined.
 *
 */
typedef enum
{
	LX_AUD_OUTPUT_NONE	= 0x00,		///< None
	LX_AUD_OUTPUT_DAC0	= 0X01,		///< DAC Output 0(SCART)
	LX_AUD_OUTPUT_DAC1	= 0X02,		///< DAC Output 1(Line Out)
	LX_AUD_OUTPUT_DAC2	= 0X04,		///< DAC Output 2(HeadPhone)
	LX_AUD_OUTPUT_DAC3	= 0X08,		///< DAC Output 3
	LX_AUD_OUTPUT_I2S0	= 0X10,		///< I2S Output 0(AMP)
	LX_AUD_OUTPUT_I2S1	= 0X20,		///< I2S Output 1(SCART)
	LX_AUD_OUTPUT_I2S2	= 0X40,		///< I2S Output 2(Line Out)
	LX_AUD_OUTPUT_I2S3	= 0X80,		///< I2S Output 3(HeadPhone) for Gain
	LX_AUD_OUTPUT_ALL	= 0XFF		///< ALL Output Port
} LX_AUD_OUTPUT_PORT_T;

/**
 * Audio DAC control mode for volume and mute control.
 * Audio DAC control mode is defined.
 *
 */
typedef enum
{
	LX_AUD_DAC_NONE			= 0x00,		///< None
	LX_AUD_DAC_DSP			= 0x01,		///< DAC Control by DSP
	LX_AUD_DAC_MIXEDIP		= 0x02,		///< DAC Control by Mixed IP
	LX_AUD_DAC_ALL			= 0x03,		///< DAC Control by DSP & Mixed IP
	LX_AUD_DAC_SCART		= 0x04,		///< SCART Volume Control by Mixed IP(12 bB boost for NICAM)
	LX_AUD_DAC_SCART_RESET	= 0x08,		///< SCART Reset Control by Mixed IP
} LX_AUD_DAC_MODE_T;

/**
 * AUDIO DECODER Codec Type.
 */
typedef enum {
	LX_AUD_CODEC_UNKNOWN,			///< 0
	LX_AUD_CODEC_PCM,				///< 1
	LX_AUD_CODEC_AC3,				///< 2(AC3, E-AC3(DD+))
	LX_AUD_CODEC_AAC,				///< 3(AAC, HE-AAC)
	LX_AUD_CODEC_DRA,				///< 4
	LX_AUD_CODEC_MP3,				///< 5
	LX_AUD_CODEC_WMA_PRO,			///< 6
	LX_AUD_CODEC_DTS_M6,			///< 7
	LX_AUD_CODEC_VORBIS,			///< 8(OGG)
	LX_AUD_CODEC_AMR_WB,			///< 9
	LX_AUD_CODEC_AMR_NB,			///< 10
	LX_AUD_CODEC_ADPCM,				///< 11
	LX_AUD_CODEC_RA8,				///< 12
	LX_AUD_CODEC_FLAC,				///< 13
	LX_AUD_CODEC_AAC_ENC,			///< 14
	LX_AUD_CODEC_AC3_ENC,			///< 15
	LX_AUD_CODEC_AAC_BYPASS,		///< 16 Virtual Codec Name
	LX_AUD_CODEC_AC3_IEC60958,		///< 17(AC3 HDMI, IEC60958), Virtual Codec Name
	LX_AUD_CODEC_DTS_CD,			///< 18(DTS HDMI), Virtual Codec Name
#ifdef IC_CERTIFICATION
	LX_AUD_CODEC_DTS_ENC,			///< 19
#endif
	LX_AUD_CODEC_MAX,				///< Max. Value
} LX_AUD_CODEC_T;

/**
 * AUDIO DECODER Codec Detail Type.
 */
typedef enum {
	LX_AUD_CODEC_DETAIL_NONE,				///< 0
	LX_AUD_CODEC_DETAIL_AC3,				///< 1
	LX_AUD_CODEC_DETAIL_EAC3,				///< 2
	LX_AUD_CODEC_DETAIL_AAC,				///< 3
	LX_AUD_CODEC_DETAIL_HEAACV1,			///< 4
	LX_AUD_CODEC_DETAIL_HEAACV2, 			///< 5
	LX_AUD_CODEC_DETAIL_DTS,				///< 6
	LX_AUD_CODEC_DETAIL_DTS_HD_MA,			///< 7
	LX_AUD_CODEC_DETAIL_DTS_EXPRESS, 		///< 8
	LX_AUD_CODEC_DETAIL_DTS_CD, 			///< 9
	LX_AUD_CODEC_DETAIL_PCM, 				///< 10
	LX_AUD_CODEC_DETAIL_MAX,				///< Max. Value
} LX_AUD_CODEC_DETAIL_T;

/**
 * AUDIO Sampling Frequency Index.
 */
typedef enum {
	LX_AUD_SAMPLING_FREQ_NONE		=	     0,
	LX_AUD_SAMPLING_FREQ_4_KHZ		=	  4000,
	LX_AUD_SAMPLING_FREQ_8_KHZ		=	  8000,
	LX_AUD_SAMPLING_FREQ_11_025KHZ	=	 11025,
	LX_AUD_SAMPLING_FREQ_12_KHZ		=	 12000,
	LX_AUD_SAMPLING_FREQ_16_KHZ		=	 16000,
	LX_AUD_SAMPLING_FREQ_22_05KHZ	=	 22050,
	LX_AUD_SAMPLING_FREQ_24_KHZ		=	 24000,
	LX_AUD_SAMPLING_FREQ_32_KHZ		=	 32000,
	LX_AUD_SAMPLING_FREQ_44_1KHZ	=	 44100,
	LX_AUD_SAMPLING_FREQ_48_KHZ		=	 48000,
	LX_AUD_SAMPLING_FREQ_64_KHZ		=	 64000,
	LX_AUD_SAMPLING_FREQ_88_2KHZ	=	 88200,
	LX_AUD_SAMPLING_FREQ_96_KHZ		=	 96000,
	LX_AUD_SAMPLING_FREQ_128_KHZ	=	128000,
	LX_AUD_SAMPLING_FREQ_176_4KHZ	=	176400,
	LX_AUD_SAMPLING_FREQ_192_KHZ	=	192000,
	LX_AUD_SAMPLING_FREQ_768_KHZ	=	768000,
	LX_AUD_SAMPLING_FREQ_DEFAULT	=	999000,
} LX_AUD_SAMPLING_FREQ_T;

/**
 * AUDIO BUFFER Status Type.
 */
typedef  enum
{
	LX_AUD_BUFFER_NONE		= 0,
	LX_AUD_BUFFER_START		= 1,
	LX_AUD_BUFFER_MIDDLE	= 2,
	LX_AUD_BUFFER_END		= 3,
} LX_AUD_BUFFER_T;

/**
 * AUDIO REG(Register) Mode.
 * This enumeration describes the AUDIO REG mode to read and write.
 *
 */
typedef  enum
{
	LX_AUD_REG_READ			= 0,		///< Read
	LX_AUD_REG_WRITE		= 1,		///< Write
	LX_AUD_REG_WRNRD		= 2,		///< Write and Read
	LX_AUD_REG_READ_ARRAY	= 3,		///< Read a set of register
	LX_AUD_REG_READ_ALL		= 4,		///< Read a full register
	LX_AUD_REG_GET_MAX_ADDR	= 5,		///< Get a max address for AUD
	LX_AUD_REG_READ_DSP1_PC	= 6,		///< Read DSP1 PC callstack trace
	LX_AUD_REG_READ_DESC	= 7,		///< Read Register with description
	LX_AUD_REG_READ_FW_VER	= 8,		///< Read DSP FW Vesrion Info.

	LX_AUD_CTOP_REG_READ		 = 10,		///< Read(CTOP CTRL REG)
	LX_AUD_CTOP_REG_WRITE		 = 11,		///< Write(CTOP CTRL REG)
	LX_AUD_CTOP_REG_WRNRD		 = 12,		///< Write and Read(CTOP CTRL REG)
	LX_AUD_CTOP_REG_READ_ARRAY	 = 13,		///< Read a set of register (CTOP CTRL REG)
	LX_AUD_CTOP_REG_READ_ALL	 = 14,		///< Read a full register (CTOP CTRL REG)
	LX_AUD_CTOP_REG_GET_MAX_ADDR = 15,		///< Get a max address for CTOP
	LX_AUD_CTOP_REG_SET_CPU_JTAG = 16,		///< Set a AUD DSP JTAG for CPU Port
	LX_AUD_CTOP_REG_SET_PER_JTAG = 17,		///< Set a AUD DSP JTAG for PERiperal Port

	LX_AUD_ANALOG_REG_READ		 = 20,		///< Read(ANALOG REG)
	LX_AUD_ANALOG_REG_WRITE		 = 21,		///< Write(ANALOG REG)
	LX_AUD_ANALOG_REG_WRNRD		 = 22,		///< Write and Read(ANALOG REG)
	LX_AUD_ANALOG_REG_READ_ARRAY = 23,		///< Read a set of register (ANALOG REG)
	LX_AUD_SET_SPDIF_DTO_A_CLOCK = 24,		///< Set a SPDIF DTO A Clock
} LX_AUD_REG_MODE_T;

/**
 * AUDIO Device Output Type.
 */
typedef enum {
	LX_AUD_DEV_OUT_NONE,		//	0,
	LX_AUD_DEV_OUT_PCM,			//	1,
	LX_AUD_DEV_OUT_ES,			//	2,
	LX_AUD_DEV_OUT_PCM_ES,		//	3,
} LX_AUD_DEV_OUT_TYPE_T;

/**
 * Endian of PCM.
 */
typedef enum {
	LX_AUD_LITTLE_ENDIAN		= 0,			///< little endian
	LX_AUD_BIG_ENDIAN			= 1,			///< big endian
} LX_AUD_ENDIAN_T;

/**
 * Signed of PCM.
 */
typedef enum {
	LX_AUD_SIGNED				= 0,			///< signed PCM
	LX_AUD_UNSIGNED				= 1,			///< unsigned PCM
} LX_AUD_SIGNED_T;

/**
 * AUDIO PCM Format Type.
 * AUDIO PCM Format type is defined.
 *
 */
typedef struct LX_AUD_PCM_FORMAT{
	LX_AUD_SAMPLING_FREQ_T	samplingFreq;		///< The Sampling Frequency of PCM
	UINT32					ui32NumOfChannel;	///< The number of channnel of PCM
	UINT32					ui32BitPerSample; 	///< The bit per sample of PCM
	LX_AUD_ENDIAN_T			endianType;  		///< The endian of PCM(0 is little, 1 is big)
    LX_AUD_SIGNED_T			signedType;			///< The signed of PCM(0 is signed, 1 is unsigned)
} LX_AUD_PCM_FORMAT_T;

/**
 * AUDIO EVENT Type.
 * AUDIO Event type is defined.
 *
 */
typedef struct {
	LX_AUD_DEV_T			allocDev;	///< The allocated device.(Master device is not supported now.)
	LX_AUD_EVENT_MSG_T		eventMsg;	///< The SET or GET event message of allocated device.
} LX_AUD_EVENT_TYPE_T;

/**
 * AUDIO DAC Output Volume.
 * audio ioctl parameter.
 * @see AUD_IOW_SET_DAC_OUT_VOLUME
*/
typedef struct {
	LX_AUD_OUTPUT_PORT_T	portNum;	///< The number of DAC Port(DAC2 : HEADPHONE, DAC0 : SCART)
	UINT32 					swVolume;	///< The Volume Level for DSP
	UINT16 					hwVolume;	///< The Volume Level for Mixed IP
	LX_AUD_DAC_MODE_T		mode;		///< Volume control mode
	UINT8 					scartGain;	///< The SCART Gain db level for Mixed IP(0 ~ 12 dB)
} LX_AUD_DAC_VOL_T;

/**
 * AUDIO Output Port Mute.
 * audio ioctl parameter.
 * @see AUD_IOW_SET_OUTPUT_PORT_MUTE
*/
typedef struct {
	LX_AUD_OUTPUT_PORT_T	portNum;	///< The number of Output Port
	BOOLEAN					bSwOnOff;	///< Mute On/Off DSP : FALSE => off, TRUE => on
	BOOLEAN					bHwOnOff;	///< Mute On/Off Mixed IP : FALSE => off, TRUE => on
	LX_AUD_DAC_MODE_T		mode;		///< Mute control mode
} LX_AUD_OUTPUT_PORT_MUTE_T;

/**
 * AUDIO AUI(Access Unit Info) type.
 * refered to ADEC_AU_INFO_T
 */
typedef struct {
	UINT64					ui64Size;						// audio ES size
	UINT32					ui32TimeStamp;					// audio ES Timestamp.
	UINT32					ui32Gstc;						// audio ES gstc.
	UINT16					ui32Index;						// audio ES index
	UINT8					ui32Error;						// audio ES error
	UINT8					ui32discontinuity;				// audio ES discontinuity
	UINT32					ui32LastAu;						// last AU
	UINT32					ui32FirstModuleGstc;			// First Module GSTC
} LX_AUD_AU_INFO_T;

/**
 * AUDIO Index & context info.
 * adec ioctl parameter.
 * @see AUD_IOR_SET_DECODER_INDEX
 * @see AUD_IOR_SET_RENDERER_INDEX
*/
typedef struct {
	LX_AUD_DEV_TYPE_T		devType;					///< device type
	LX_AUD_DEV_T 			dev;						///< device id
	LX_AUD_DEV_OUT_TYPE_T	devOutType;					///< device output type
	UINT32					ui32Index;					///< device index
	void					*pCtx;						///< device context
} LX_AUD_DEV_INDEX_CTX_T;

/**
 * AUDIO DECODED Info from DSP after decoding is completed.
 * adec ioctl parameter.
 * @see .
*/
typedef struct {
	UINT32					ui32DecoderNum;			///< Audio Decoder Number to Get Info.

	LX_AUD_CODEC_T			codecType;				///< Audio Codec Type Info.
	LX_AUD_SAMPLING_FREQ_T	samplingFreq;			///< Audio Sampling Frequency Info.
	UINT32					ui32AvgBitRate;			///< The average bitrate per seconds.
	UINT32					ui32NumOfChannel;		///< The number of channnel of PCM
	UINT32					ui32BitPerSample;		///< The bit per sample of PCM data

	UINT32					ui32AllocDev;			///< Audio Decoder Allocated Number.
	LX_AUD_INPUT_T			input;					///< Audio Input Info.
	UINT32					ui32IsDecodable;		///< The decodable status for TP0/1 input.
	UINT32					ui32ChannelNum;			///< The decoded number of channel of original ES.
	BOOLEAN					bSpdifESAvail;			///< Whether SPDIF ES output is available or not.

	LX_AUD_CODEC_DETAIL_T	codecDetailType;		///< Audio Codec Type Detail Info.
} LX_AUD_GET_DECODED_INFO_T;

/**
 * Get and Set AUDIO REG Info.
 * audio ioctl parameter.
 * @see AUD_IORW_READ_AND_WRITE_REG
*/
typedef struct {
	LX_AUD_REG_MODE_T	mode;				///< The operation mode to read and write
	UINT32				addr;				///< The address to read and write
	UINT32				readdata;			///< The data to read
	UINT32				writedata;			///< The data to write
} LX_AUD_REG_INFO_T;

/**
 * Set AUDIO IPC Command.
 * audio ioctl parameter.
 * @see AUD_IOW_CUSTOM_IPC
*/
typedef struct {
	UINT32				hdr;						///< The data of header for IPC command
	UINT32				bodySize;					///< The size of IPC command
	UINT32				param[LX_IPC_ARRAY_SIZE];	///< The data of parameters for IPC command
} LX_AUD_CUSTOM_IMC_T;

 /**
  * Enable or Disable AUDIO Debug Print.
  * audio ioctl parameter.
  * @see AUD_IOW_DEBUG_PRINT_CTRL
 */
 typedef struct {
	UINT32				printType;				///< The type of print module
	UINT32				printColor;				///< The color of print
} LX_AUD_DEBUG_CTRL_T;

 /**
  * Enable or Disable AUDIO Debug Print.
  * audio ioctl parameter.
  * @see AUD_IOW_DEBUG_PRINT_CTRL
 */
 typedef struct {
	UINT32				address;				///< The address to dump momory.
	UINT32				line;					///< The line to dump momory(1 line = 0x20bytes).
	UINT32				type;					///< The type to show dumped momory(terminal : 0, file : 1).
	char				fileName[LX_AUD_FILENAME_SIZE];			///< The name of file to dump memory.
} LX_AUD_DEBUG_MEM_DUMP_T;

 /**
  * Get AUDIO Module Info for Debug.
  * audio ioctl parameter.
  * @see AUD_IOW_DEBUG_PRINT_CTRL
 */
typedef struct {
	UINT32				module;					///< ADEC_MODULE_ID
	char				name[LX_AUD_MOD_NAME_SIZE];
} LX_AUD_DEBUG_MOD_INFO_T;

typedef struct {
	UINT32				index;
	UINT32				debug_level;
	UINT32				enable;
	char				index_name[12];
} LX_AUD_FW_DEBUG_INFO_NODE_T;

typedef struct {
	UINT32				num_of_index;
	UINT32				mask_value;
	LX_AUD_FW_DEBUG_INFO_NODE_T		nodes[32];
} LX_AUD_FW_DEBUG_INFO_T;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_KAPI_H_ */

/** @} */


