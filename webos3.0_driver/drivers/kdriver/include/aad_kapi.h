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


/** @file aad_kapi.h
 *
 *  application interface header for aad device
 *
 *  @author		JongSang Oh(jongsang.oh@lge.com)
 *  @version		1.0
 *  @date		2012.09.18
 *
 *  @addtogroup lg1150_aad
 *	@{
 */

#ifndef	_AAD_KAPI_H_
#define	_AAD_KAPI_H_

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
#define	AAD_IOC_MAGIC		'a'
/**
@name AAD IOCTL List
ioctl list for aad device.

@{

@def AAD_IOW_INIT_MODULE
Initializes the aad module.
AAD module shall be initialized itself with pre-defined configuration value.\n
AAD module includes Analog Audio Decoder parts.\n
And, serveral global variable is set by LX_AAD_INIT_TYPE_T value by each bit operation.\
LX_AAD_INIT_BTSC_SELECT type means that TV Systems for BTSC enabled.
LX_AAD_INIT_ASD_SELECT type means that System Search for ASD(Automatic System Detection) enabled.
LX_AAD_INIT_ASCS_SELECT type means that Stereo Search for ASCS(Automatic Stereo Carrier Search) enabled. ASCS applies to A2 and NICAM systems..
@li register device and interrupts.

@def AAD_IOW_SIF_SET_HIGH_DEV_MODE
Sets High Deviation Level Value.
These settings do not apply to the single carrier hierarchical modulation modes(EIAJ, BTSC and FM-Radio).

@def AAD_IOW_SIF_SET_BAND_SETUP
Sets a SIF Band(B/G, I, DK, L, M).
This function sets a SIF sound system.

@def AAD_IOW_SIF_SET_MODE_SETUP
Sets a SIF sound standard input mode.
This function sets a SIF sound standard.

@def AAD_IOW_SIF_SET_USER_ANALOG_MODE
Sets a user analog TV stereo mode.
Analog Audio Output Mode is setted to Analog Audio Decoder by User Mode or Analog Audio Task.

@def AAD_IOW_SIF_SET_THRESHOLD_LEVEL
Sets a threshold level of SIF sound system.
This function sets the threshold level of stereo or mono sound detection in current SIF input.

@def AAD_IORW_SIF_GET_BAND_DETECT
Sets a SIF Band(B/G, I, DK, L, M) and returns the strength of input signal quality.
Gets also a detected sound system and signal strength quality level.

@def AAD_IORW_SIF_CHECK_SOUND_MODE
Checks if a SIF standard such as NICAM, A2(DK), FM exists in the input SIF signal.
Gets also a detected sound standard and signal existence by input parameters only.

@def AAD_IOR_SIF_GET_THRESHOLD_LEVEL
Gets a threshold level of SIF sound system.
The current threshold level is returned.

@def AAD_IOR_SIF_GET_CUR_ANALOG_MODE
Gets the current analog audio mode.
This function checks the current analog audio broadcasting status.
Analog Audio Output Mode is set by User Analog Mode or Analog Audio Task function.

@def AAD_IOR_SIF_GET_SIF_STATUS
Checks whether SIF(Sound Intermediate Frequency) carrier exists or not.
This function is used for checking the SIF of the current analog signal.

@def AAD_IOW_SIF_SET_MONITOR_TASK
Enables or Disables a AAD Monitor task for detection A2 mono signal.
This function is used to workaround code for misdection Mono and Stereo signal.

@def AAD_IORW_SIF_READ_AND_WRITE_REG
Do read and write aad register.
This function is used to debug AAD module for debug purpose.

@def AAD_IOW_DEBUG_PRINT_CTRL
Enables or Disables a AAD kernel driver debug print.
This function is used to debug AAD module for debug purpose.


*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/

/* SIF(Sound Intermediate Frequency) control */
#define	AAD_IOW_INIT_MODULE							_IOW(AAD_IOC_MAGIC,  0, LX_AAD_INIT_TYPE_T)
#define AAD_IOW_SIF_SET_HIGH_DEV_MODE				_IOW(AAD_IOC_MAGIC,  1, LX_AAD_HIGH_DEV_SET_T)
#define AAD_IOW_SIF_SET_BAND_SETUP					_IOW(AAD_IOC_MAGIC,  2, LX_AAD_SIF_SOUNDSYSTEM_T)
#define AAD_IOW_SIF_SET_MODE_SETUP					_IOW(AAD_IOC_MAGIC,  3, LX_AAD_SIF_STANDARD_T)
#define AAD_IOW_SIF_SET_USER_ANALOG_MODE			_IOW(AAD_IOC_MAGIC,  4, LX_ATV_AUDIO_MODE_SET_T)
#define AAD_IOW_SIF_SET_THRESHOLD_LEVEL				_IOW(AAD_IOC_MAGIC,  5, LX_AAD_SIF_THRESHOLD_T)
#define AAD_IORW_SIF_GET_BAND_DETECT				_IOWR(AAD_IOC_MAGIC, 6, LX_AAD_SIF_SYSTEM_INFO_T)
#define AAD_IORW_SIF_CHECK_SOUND_MODE				_IOWR(AAD_IOC_MAGIC, 7, LX_AAD_SIF_STANDARD_INFO_T)
#define AAD_IOR_SIF_GET_THRESHOLD_LEVEL				_IOWR(AAD_IOC_MAGIC, 8, LX_AAD_SIF_THRESHOLD_T)
#define AAD_IOR_SIF_GET_CUR_ANALOG_MODE				_IOR(AAD_IOC_MAGIC,  9, LX_ATV_AUDIO_MODE_GET_T)
#define AAD_IOR_SIF_GET_SIF_STATUS					_IOR(AAD_IOC_MAGIC, 10, BOOLEAN)
#define AAD_IOW_SIF_SET_MONITOR_TASK				_IOW(AAD_IOC_MAGIC, 11, BOOLEAN)
#define AAD_IOW_SIF_SET_AAD_SIF_INPUT				_IOW(AAD_IOC_MAGIC, 12, LX_AAD_SIF_INPUT_T)

#define AAD_IORW_SIF_READ_AND_WRITE_REG				_IOWR(AAD_IOC_MAGIC, 20, LX_AAD_REG_INFO_T)
#define AAD_IOW_DEBUG_PRINT_CTRL					_IOW(AAD_IOC_MAGIC,  21, LX_AAD_DEBUG_CTRL_T)


#define AAD_IOC_MAXNR								30
/** @} */


// define DDI AAD and KADP debug print enable
//#define DDI_AAD_DEBUG_FORCED_ENABLE


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * AAD Init Module Type.
 * AAD Init Module Type is defined.\n
 * If type is LX_AAD_INIT_TYPE_NONE, AAD is set to default mode.
 *
 */
typedef  enum
{
	LX_AAD_INIT_TYPE_NONE			= 0,		///< INIT TYPE : NONE
	LX_AAD_INIT_ATSC_A2_SELECT		= 1,		///< INIT TYPE : TV Systems for A2 enabled in ATSC system
	LX_AAD_INIT_ATSC_BTSC_SELECT	= 2,		///< INIT TYPE : TV Systems for BTSC enabled in ATSC(Brazil, CO, CF) or DVB(Taiwan) system
	LX_AAD_INIT_DVB_SELECT 			= 3,		///< INIT TYPE : TV Systems for EU in DVB system
	LX_AAD_INIT_DVB_ID_SELECT		= 4,		///< INIT TYPE : TV Systems for ID(Indonesia) in DVB(PAL B/G) system
	LX_AAD_INIT_DVB_IN_SELECT		= 5,		///< INIT TYPE : TV Systems for IN(India) in DVB(PAL B) system
	LX_AAD_INIT_DVB_CN_SELECT		= 6,		///< INIT TYPE : TV Systems for CN(China) in DVB system
	LX_AAD_INIT_ATSC_BTSC_US_SELECT	= 7,		///< INIT TYPE : TV Systems for BTSC enabled in ATSC(US) system
	LX_AAD_INIT_DVB_AJJA_SELECT		= 8,		///< INIT TYPE : TV Systems for AJJA in DVB(NTSC M/N) system
	LX_AAD_INIT_TYPE_MAX						///< INIT TYPE : MAX
} LX_AAD_INIT_TYPE_T;

/**
 * AAD SIF High Deviation Level.
 * This enumeration describes the SIF High Deviation Level for analog audio decoder.
 *
 */
typedef  enum
{
	LX_AAD_HIGH_DEV_LEVEL_OFF	= 0,		///< High Deviation OFF(50KHz)
	LX_AAD_HIGH_DEV_LEVEL_1		= 1,		///< High Deviation Level 1(100KHz)
	LX_AAD_HIGH_DEV_LEVEL_2		= 2,		///< High Deviation Level 2(200KHz)
	LX_AAD_HIGH_DEV_LEVEL_3		= 3,		///< High Deviation Level 3(384KHz)
	LX_AAD_HIGH_DEV_LEVEL_4		= 4			///< High Deviation Level 4(540KHz)
} LX_AAD_HIGH_DEV_LEVEL_T;

/**
 * AAD SIF Sound System.
 * SIF Sound System is defined.
 *
 */
typedef  enum
{
	LX_AAD_SIF_SYSTEM_BG		= 0x00,		///< BG
	LX_AAD_SIF_SYSTEM_I			= 0x01,		///< I
	LX_AAD_SIF_SYSTEM_DK		= 0x02,		///< DK
	LX_AAD_SIF_SYSTEM_L			= 0x03,		///< L
	LX_AAD_SIF_SYSTEM_MN		= 0x04,		///< MN
	LX_AAD_SIF_SYSTEM_UNKNOWN	= 0xF0		///< UNKNOWN
} LX_AAD_SIF_SOUNDSYSTEM_T;

/**
 * AAD SIF Sound Standard.
 * SIF Sound Standard is defined.
 *
 */
typedef enum
{
	LX_AAD_SIF_MODE_DETECT		= 0,		///< MODE DETECT
	LX_AAD_SIF_BG_NICAM			= 1,		///< BG NICAM
	LX_AAD_SIF_BG_FM			= 2,		///< BG FM
	LX_AAD_SIF_BG_A2			= 3,		///< BG A2
	LX_AAD_SIF_I_NICAM			= 4,		///< I NICAM
	LX_AAD_SIF_I_FM				= 5,		///< I FM
	LX_AAD_SIF_DK_NICAM			= 6,		///< DK NICAM
	LX_AAD_SIF_DK_FM			= 7,		///< DK FM
	LX_AAD_SIF_DK1_A2			= 8,		///< DK1 A2
	LX_AAD_SIF_DK2_A2			= 9,		///< DK2 A2
	LX_AAD_SIF_DK3_A2			= 10,		///< DK3 A2
	LX_AAD_SIF_L_NICAM			= 11,		///< L NICAM
	LX_AAD_SIF_L_AM				= 12,		///< L AM
	LX_AAD_SIF_MN_A2			= 13,		///< MN A2
	LX_AAD_SIF_MN_BTSC			= 14,		///< MN BTSC
	LX_AAD_SIF_MN_EIAJ			= 15,		///< MN EIAJ
	LX_AAD_SIF_NUM_SOUND_STD	= 16		///< NUMBER OF SOUND STANDARD
} LX_AAD_SIF_STANDARD_T;

/**
 * AAD SET SIF Analog Audio SAP(Secondary Audio Program) Mode.
 * This enumeration contains the setting parameters of analog audio SAP mode.
 *
 */
typedef enum
{
	LX_ATV_SET_PAL_MONO					=	0x00,	///< PAL Mono
	LX_ATV_SET_PAL_MONO_FORCED			=	0x01,	///< PAL Mono Force Mono
	LX_ATV_SET_PAL_STEREO				=	0x02,	///< PAL Stereo
	LX_ATV_SET_PAL_STEREO_FORCED		=	0x03,	///< PAL Stereo Force Mono
	LX_ATV_SET_PAL_DUALI				=	0x04,	///< PAL Dual I
	LX_ATV_SET_PAL_DUALII				=	0x05,	///< PAL Dual II
	LX_ATV_SET_PAL_DUALI_II				=	0x06,	///< PAL Dual I+II
	LX_ATV_SET_PAL_NICAM_MONO			=	0x07,	///< PAL NICAM Mono
	LX_ATV_SET_PAL_NICAM_MONO_FORCED	=	0x08,	///< PAL NICAM Mono Force Mono
	LX_ATV_SET_PAL_NICAM_STEREO			=	0x09,	///< PAL NICAM Stereo
	LX_ATV_SET_PAL_NICAM_STEREO_FORCED	=	0x0A,	///< PAL NICAM Stereo Force Mono
	LX_ATV_SET_PAL_NICAM_DUALI			=	0x0B,	///< PAL NICAM Dual I
	LX_ATV_SET_PAL_NICAM_DUALII			=	0x0C,	///< PAL NICAM Dual II
	LX_ATV_SET_PAL_NICAM_DUALI_II		=	0x0D,	///< PAL NICAM Dual I+II
	LX_ATV_SET_PAL_NICAM_DUAL_FORCED	=	0x0E,	///< PAL NICAM Dual Forced Mono(Not Supported)
	LX_ATV_SET_PAL_UNKNOWN				=	0x0F,	///< PAL Unkown State
	LX_ATV_SET_NTSC_A2_MONO				=	0x10,	///< NTSC(A2) Mono
	LX_ATV_SET_NTSC_A2_STEREO			=	0x11,	///< NTSC(A2) Stereo
	LX_ATV_SET_NTSC_A2_SAP				=	0x12,	///< NTSC(A2) SAP
	LX_ATV_SET_NTSC_A2_UNKNOWN			=	0x13,	///< NTSC(A2) Unkown State
	LX_ATV_SET_NTSC_BTSC_MONO			=	0x14,	///< NTSC(BTSC) Mono
	LX_ATV_SET_NTSC_BTSC_STEREO			=	0x15,	///< NTSC(BTSC) Stereo
	LX_ATV_SET_NTSC_BTSC_SAP_MONO		=	0x16,	///< NTSC(BTSC) SAP Mono
	LX_ATV_SET_NTSC_BTSC_SAP_STEREO		=	0x17,	///< NTSC(BTSC) SAP Stereo
	LX_ATV_SET_NTSC_BTSC_UNKNOWN		=	0x18	///< NTSC(BTSC) Unkown State
} LX_ATV_AUDIO_MODE_SET_T;

/**
 * AAD SIF Mono/Stereo/SAP Detection Threshold Mode.
 * This enumeration describes the AAD SIF Mono/Stereo/SAP detection threshold mode.
 *
 */
typedef  enum
{
	LX_AAD_THRES_NONE				= 0x00,	///< None
	LX_AAD_THRES_CAR2_MAGITUDE		= 0x01,	///< The mode of threshold of Carrier 2 magnitude
	LX_AAD_THRES_CAR2_QUALITY_HI	= 0x02,	///< The mode of threshold of Carrier 2 quality for high
	LX_AAD_THRES_CAR2_QUALITY_LO	= 0x04,	///< The mode of threshold of Carrier 2 quality for low, UI uses this value for dominant value.
	LX_AAD_THRES_ID_MAGNITUDE		= 0x08,	///< The mode of threshold of stereo ID magnitude

	LX_AAD_THRES_NICAM_ERROR		= 0x10	///< The mode of threshold of NICAM Error Rate
} LX_AAD_SIF_THRES_MODE_T;

/**
 * AAD SIF Input Source Type.
 *
 */
typedef enum
{
	LX_AAD_SIF_INPUT_EXTERNAL	= 0,		///< External SIF Input(GBB)
	LX_AAD_SIF_INPUT_INTERNAL	= 1,		///< Internal SIF Input(ABB)
} LX_AAD_SIF_INPUT_T;

/**
 * AAD SIF Mono/Stereo/SAP Detection Threshold Value.
 * aad ioctl parameter.
 * @see AAD_IOW_SIF_SET_THRESHOLD_LEVEL, AAD_IOR_SIF_GET_THRESHOLD_LEVEL
*/
typedef struct LX_AAD_SIF_THRESHOLD{
	LX_AAD_SIF_THRES_MODE_T		mode;	///< The mode of threshold value

	UINT16		carrier2Magitude;		///< The threshold of Carrier 2 magnitude

	UINT16		carrier2QualityHigh;	///< The high threshold of Carrier 2 quality
	UINT16		carrier2QualityLow;		///< The low threshold of Carrier 2 quality, UI uses this value for dominant value.
	UINT8		stereoIDMagHigh;		///< The high threshold of stereo ID magnitude
	UINT8		stereoIDMagLow;			///< The low threshold of stereo ID magnitude

	UINT8		nicamErrorRateHigh;		///< The high threshold of NICAM Error Rate
	UINT8		nicamErrorRateLow;		///< The low threshold of NICAM Error Rate
} LX_AAD_SIF_THRESHOLD_T;

/**
 * AAD SIF available Sound Standard at all and D/K system.
 * SIF available standard is defined.
 *
 */
typedef  enum
{
	LX_AAD_SIF_NICAM					= 0,    ///< NICAM
	LX_AAD_SIF_A2						= 1,    ///< A2(all system)
	LX_AAD_SIF_FM						= 2,    ///< FM : Mono
	LX_AAD_SIF_DETECTING_AVALIBILITY	= 3,	///< On detecting availability.
	LX_AAD_SIF_A2_DK1					= 11,	///< A2 in DK1 only(added for DDI_ADEC_SIF_CheckA2DK)
	LX_AAD_SIF_A2_DK2					= 12,	///< A2 in DK2 only(added for DDI_ADEC_SIF_CheckA2DK)
	LX_AAD_SIF_A2_DK3					= 13,	///< A2 in DK3 only(added for DDI_ADEC_SIF_CheckA2DK)
	LX_AAD_SIF_NOT_AVAILE				= 20	///< Not Availe
} LX_AAD_SIF_AVAILE_STANDARD_T;

/**
 * AAD SIF Existence Info.
 * SIF existence information is defined..
 *
 */
typedef  enum
{
	LX_AAD_SIF_ABSENT				= 0,    ///< SIF is absent.
	LX_AAD_SIF_PRESENT				= 1,    ///< SIF is present.
	LX_AAD_SIF_DETECTING_EXSISTANCE	= 2    	///< On detecting existence.
} LX_AAD_SIF_EXISTENCE_INFO_T;

/**
 * AAD GET SIF Analog Audio SAP(Secondary Audio Program) Mode.
 * This enumeration contains the current state of analog audio SAP mode.
 *
 */
typedef enum
{
	LX_ATV_GET_PAL_MONO				=	0x00,	///< PAL Mono
	LX_ATV_GET_PAL_STEREO			=	0x01,	///< PAL Stereo
	LX_ATV_GET_PAL_DUAL				=	0x02,	///< PAL Dual
	LX_ATV_GET_PAL_NICAM_MONO		=	0x03,	///< PAL NICAM Mono
	LX_ATV_GET_PAL_NICAM_STEREO		=	0x04,	///< PAL NICAM Stereo
	LX_ATV_GET_PAL_NICAM_DUAL		=	0x05,	///< PAL NICAM Dual
	LX_ATV_GET_PAL_UNKNOWN			=	0x06,	///< PAL Unkown State
	LX_ATV_GET_NTSC_A2_MONO			=	0x10,	///< NTSC(A2) Mono
	LX_ATV_GET_NTSC_A2_STEREO		=	0x11,	///< NTSC(A2) Stereo
	LX_ATV_GET_NTSC_A2_SAP			=	0x12,	///< NTSC(A2) SAP
	LX_ATV_GET_NTSC_A2_UNKNOWN		=	0x13,	///< NTSC(A2) Unkown State
	LX_ATV_GET_NTSC_BTSC_MONO		=	0x14,	///< NTSC(BTSC) Mono
	LX_ATV_GET_NTSC_BTSC_STEREO		=	0x15,	///< NTSC(BTSC) Stereo
	LX_ATV_GET_NTSC_BTSC_SAP_MONO	=	0x16,	///< NTSC(BTSC) SAP Mono
	LX_ATV_GET_NTSC_BTSC_SAP_STEREO	=	0x17,	///< NTSC(BTSC) SAP Stereo
	LX_ATV_GET_NTSC_BTSC_UNKNOWN	=	0x18	///< NTSC(BTSC) Unkown State
} LX_ATV_AUDIO_MODE_GET_T;

/**
 * AAD SIF REG(Register) Mode.
 * This enumeration describes the AAD REG mode to read and write.
 *
 */
typedef  enum
{
	LX_AAD_REG_READ			= 0,	///< Read
	LX_AAD_REG_WRITE		= 1,	///< Write
	LX_AAD_REG_WRNRD		= 2,	///< Write and Read
	LX_AAD_REG_READ_ARRAY	= 3,	///< Read a set of register
	LX_AAD_REG_READ_ALL		= 4,	///< Read a full register
	LX_AAD_REG_GET_MAX_ADDR = 5		///< Get a max address for AAD
} LX_AAD_REG_MODE_T;


/**
 * AAD SIF High Deviation Set Info.
 * aad ioctl parameter.
 * @see AAD_IOW_SIF_SET_HIGH_DEV_MODE
*/
typedef struct LX_AAD_HIGH_DEV_SET{
	BOOLEAN						bHighDevOnOff;		///< The Status of High Deviation to setted
	LX_AAD_HIGH_DEV_LEVEL_T		highDevLevel;		///< The Level of High Deviation to set
} LX_AAD_HIGH_DEV_SET_T;

/**
 * AAD SIF Sound System Detect Info.
 * aad ioctl parameter.
 * @see AAD_IORW_SIF_GET_BAND_DETECT
*/
typedef struct LX_AAD_SIF_SYSTEM_INFO{
	LX_AAD_SIF_SOUNDSYSTEM_T	setSystem;			///< The SIF Band(B/G, I, DK, L, M) to set
	BOOLEAN						bManualMode;		///< The status of manual mode to detect sound system
	LX_AAD_SIF_SOUNDSYSTEM_T	detectedSystem;		///< The SIF Band(B/G, I, DK, L, M) to be detected
	UINT32						bandStrength;		///< The quality of input signal for Carrier1
	UINT32						bandMagnitude;		///< The magnitude of input signal for Carrier1
} LX_AAD_SIF_SYSTEM_INFO_T;

/**
 * AAD SIF Sound Standard Detect Info.
 * aad ioctl parameter.
 * @see AAD_IORW_SIF_CHECK_SOUND_MODE
*/
typedef struct LX_AAD_SIF_STANDARD_INFO{
	LX_AAD_SIF_AVAILE_STANDARD_T	setStandard;		///< The SIF Mode(NICAM, FM, A2) to set
	LX_AAD_SIF_AVAILE_STANDARD_T	detectedStandard;	///< The SIF Mode(NICAM, FM, A2) to be detected
	LX_AAD_SIF_EXISTENCE_INFO_T		availability;		///< The SIF Existence Info.
} LX_AAD_SIF_STANDARD_INFO_T;


/**
 * AAD SIF REG Info.
 * aad ioctl parameter.
 * @see AAD_IORW_SIF_READ_AND_WRITE_REG
*/
typedef struct LX_AAD_REG_INFO{
	LX_AAD_REG_MODE_T	mode;				///< The operation mode to read and write
	UINT32				addr;				///< The address to read and write
	UINT32				readdata;			///< The data to read
	UINT32				writedata;			///< The data to write
} LX_AAD_REG_INFO_T;

 /**
  * Enable or Disable AAD Debug Print.
  * adec ioctl parameter.
  * @seeAAD_IOW_DEBUG_PRINT_CTRL
 */

 typedef struct LX_AAD_DEBUG_CTRL{
	UINT32				printType;			///< The type of print module
	UINT32				printColor;			///< The color of print
} LX_AAD_DEBUG_CTRL_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AAD_DRV_H_ */

/** @} */
