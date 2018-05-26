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


/*****************************************************************************
**
**  Name: ademod_common_demod.h
**
**  Description:    ABB Demodulator software interface.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_Demod_Open
**                  LX_ADEMOD_Result  ADEMOD_Demod_Close
**                  LX_ADEMOD_Result  ADEMOD_Demodulate
**                  LX_ADEMOD_Result  ADEMOD_Register_Set
**                  LX_ADEMOD_Result  ADEMOD_Register_Get
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Set
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Get
**                  LX_ADEMOD_Result  ADEMOD_Firmware_Load
**                  LX_ADEMOD_Result  ADEMOD_AGC_Set
**                  LX_ADEMOD_Result  ADEMOD_AGC_Get
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Set
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Get
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Set
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Get
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Write
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Read
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result  ADEMOD_Demod_Open
**                  LX_ADEMOD_Result  ADEMOD_Demod_Close
**                  LX_ADEMOD_Result  ADEMOD_Demodulate
**                  LX_ADEMOD_Result  ADEMOD_Register_Set
**                  LX_ADEMOD_Result  ADEMOD_Register_Get
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Set
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Get
**                  LX_ADEMOD_Result  ADEMOD_Firmware_Load
**                  LX_ADEMOD_Result  ADEMOD_AGC_Set
**                  LX_ADEMOD_Result  ADEMOD_AGC_Get
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Set
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Get
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Set
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Get
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Write
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Read
**
**  Dependencies:  
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_COMMON_DEMOD_H_
#define _ADEMOD_COMMON_DEMOD_H_

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif



#ifndef LX_ABB_H15_DEVICE
#define LX_ABB_H15_DEVICE     (0x11500000)
#endif


// Small Target API model
#ifndef UINT64
typedef unsigned long long		UINT64;
#endif


#ifndef SINT64
typedef signed long long		SINT64;
#endif

#define	INT64	SINT64




#define LX_ADEMOD_REGISTER_CACHE_ENABLED		(1)
#define MAX_REG_CACHE_SIZE				(4000)


/*
**  Definitions
*/
#ifndef TRUE
#define TRUE     (1)
#endif
#ifndef FALSE
#define FALSE    (0)
#endif

/*
**  Data Types
*/
#ifndef BOOL
typedef int				BOOL;
#endif

#ifndef UINT8
typedef unsigned char   UINT8;          /*  type corresponds to 8 bits      */
#endif

#ifndef UINT16
typedef unsigned short	UINT16;         /*  type corresponds to 16 bits     */
#endif

#ifndef INT16
typedef short			INT16;			/*  type corresponds to 16 bits     */
#endif

#ifndef UINT32
typedef unsigned int    UINT32;         /*  type corresponds to 32 bits     */
#endif

#ifndef Handle_t
typedef void *          Handle_t;       /*  memory pointer type             */
#endif

#ifndef LX_ADEMOD_Result
typedef UINT32          LX_ADEMOD_Result;      /*  return codes                    */
#endif

/*
** Fresco configuration flags
*/
#define LX_ADEMOD_CONFIG_FLAG_DISABLED			(0x00000000L)
#define LX_ADEMOD_CONFIG_FLAG_HOST_COMMANDS	(0x00000001L)
#define LX_ADEMOD_CONFIG_FLAG_AUDIO_TESTING	(0x00000002L)
#define LX_ADEMOD_CONFIG_FLAG_BASEBAND_AUDIO	(0x00000004L)
#define LX_ADEMOD_CONFIG_FLAG_DETECTION		(0x00000008L)
#define LX_ADEMOD_CONFIG_FLAG_RFIFAGC			(0x00000010L)
#define LX_ADEMOD_CONFIG_FLAG_TBLMODE			(0x00000020L)
#define LX_ADEMOD_CONFIG_FLAG_SIG_MON			(0x00000040L)
#define LX_ADEMOD_CONFIG_FLAG_SW_AGC			(0x00000080L)
#define LX_ADEMOD_CONFIG_FLAG_DVBT_OPTIM		(0x00000100L)
#define LX_ADEMOD_CONFIG_FLAG_DEBUG			(0x00000200L)
#define LX_ADEMOD_CONFIG_FLAG_DEBUG_PRINTS		(0x00000400L)
#define LX_ADEMOD_CONFIG_FLAG_DEBUG_BRAVOBAUN	(0x00000800L)
#define LX_ADEMOD_CONFIG_FLAG_IF2				(0x00001000L)
#define LX_ADEMOD_CONFIG_FLAG_SECAM_AM			(0x00002000L)
#define LX_ADEMOD_CONFIG_FLAG_QUALITY_MODE		(0x00004000L)
#define LX_ADEMOD_CONFIG_FLAG_VACANT1			(0x00008000L)
#define LX_ADEMOD_CONFIG_FLAG_POWERUPSM_DIS	(0x00010000L)
#define LX_ADEMOD_CONFIG_FLAG_SDA_WAND_EN		(0x00020000L)
#define LX_ADEMOD_CONFIG_FLAG_SCL_WAND_EN		(0x00040000L)
//#define LX_ADEMOD_CONFIG_FLAG_AGC_OUT_WAND_EN	(0x00080000L) // This flag is replaced by AGC flag LX_ADEMOD_FFLAG_AGC_OPENDRAIN
#define LX_ADEMOD_CONFIG_FLAG_SCL_TUN_WAND_DIS (0x00100000L)
#define LX_ADEMOD_CONFIG_FLAG_SDA_TUN_WAND_DIS (0x00200000L)
#define LX_ADEMOD_CONFIG_FLAG_EXT_DAC_CTRL_DIS	(0x00400000L) // when set to 0, enables DAC to be controlled by external
													  // (digital demod) AGC voltage in Low IF mode.
#define LX_ADEMOD_CONFIG_FLAG_VGA_EN			(0x00800000L) // when set to 1, VGA is enabled, otherwise disabled.
#define LX_ADEMOD_CONFIG_FLAG_MONO_SENS_L_EN	(0x01000000L) // when set to 1, increased noise limited sensitivity in standard L, 0- normal sensitivity.
#define LX_ADEMOD_CONFIG_FLAG_VGA_STATUS		(0x02000000L) // VGA status: 1, VGA is enabled, otherwise disabled.
#define LX_ADEMOD_CONFIG_FLAG_I2CMODE_FAST_EN	(0x04000000L) // when set to 1, fast I2C mode is enabled, otherwise disabled.
#define LX_ADEMOD_CONFIG_FLAG_OFFCHIP_NF_EN	(0x08000000L) // when set to 1, the offchip noise filtering is enabled, otherwise disabled.
#define LX_ADEMOD_CONFIG_FLAG_PHASE_SEL_EN		(0x10000000L) // when set to 1, the phase selection is enabled, otherwise disabled.
#define LX_ADEMOD_CONFIG_FLAG_I2C_OPTIM_EN		(0x20000000L) // when set to 1, the i2c optimization is enabled, otherwise disabled.
//jeongpil.yun for M14
#define LX_ADEMOD_CONFIG_FLAG_I2C_MUTE			(0x40000000L) // when set to 1, the i2c is muted for spyder

/*
**  Error codes
*/
typedef enum LX_ADEMOD_Return_Codes_tag{
	retOK			=	0x00000000L,	// no error
	retErr			=	0x00000001L,	// unspecified generic error
	retErrBadHandle =	0x00000002L,	// bad handle
	retErrBadDevice =	0x00000004L,	// bad device
	retErrBadPtr    =   0x00000008L,    // bad pointer
	retErrBadParam  =	0x00000010L,	// bad parameter
	retErrBadInst   =	0x00000020L,	// bad instance
	retErrBadMode   =	0x00000040L,	// bad mode
	retErrBadContext=	0x00000080L,	// bad context
	retUserSizeTooBig = 0x00000100L,	// user data size exceeded limit
	retErrZeroDiv   =   0x00000200L,	// possible division by zero
//jeongpil.yun for M14
	retErrNoSupport =	0x00000400L,	// call is not supported  //jeongpil.yun for M14
} LX_ADEMOD_Return_Codes;

/*
**  Parallel port bus definition
*/
typedef enum {
	LPT_PORT_NONE,
	LPT_PORT_DATA,
	LPT_PORT_STATUS,
	LPT_PORT_CONTROL
} LX_ADEMOD_LPT_PORT;

typedef struct LX_ADEMOD_FportSignal_tag {
	unsigned char port;
	unsigned char bit;
	unsigned char invert;
}  LX_ADEMOD_FportSignal, *pFportSignal;

#define LPT1 0x378
#define LPT2 0x3BC
#define LPT3 0x278
#define LPT4 0x378       // not implemented - set to LPT1 for now,

#define BIT_NONE    0x0000
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080

/*
**  Input mode
*/
typedef enum LX_ADEMOD_InputMode_tag
{
    LX_ADEMOD_INPUTMODE_NONE,     // input mode has not been programmed
    LX_ADEMOD_INPUTMODE_LEGACY,   // legacy analog TV mode
    LX_ADEMOD_INPUTMODE_DIGITAL,  // digital mode
    LX_ADEMOD_INPUTMODE_DVBT = LX_ADEMOD_INPUTMODE_DIGITAL,     // digital DVBT mode
    LX_ADEMOD_INPUTMODE_ATSC,     // digital ATSC mode
} LX_ADEMOD_InputMode;

/*
**  Quality
*/
typedef enum LX_ADEMOD_QualityMode_tag
{
    LX_ADEMOD_QUALITYMODE_NORMAL, // 9 bit mode
    LX_ADEMOD_QUALITYMODE_HIGH,   // 12 bit mode
} LX_ADEMOD_QualityMode;

/*
**  I2C mode
*/
typedef enum LX_ADEMOD_i2cMode_tag
{
	LX_ADEMOD_I2CMODE_FAST_NONE,
    LX_ADEMOD_I2CMODE_FAST_36,
    LX_ADEMOD_I2CMODE_FAST_72,
    LX_ADEMOD_I2CMODE_FAST_144,
    LX_ADEMOD_I2CMODE_FAST_288,
} LX_ADEMOD_i2cMode;

/*
**  Format of the analog video signal
*/
typedef enum LX_ADEMOD_VideoStandard_tag {
    LX_ADEMOD_VIDEO_STANDARD_NONE				= 0x00000000,
    LX_ADEMOD_VIDEO_STANDARD_NTSC_M			= 0x00000001,
    LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J			= 0x00000002,
    LX_ADEMOD_VIDEO_STANDARD_NTSC_433			= 0x00000004,
    LX_ADEMOD_VIDEO_STANDARD_PAL_B				= 0x00000010,
    LX_ADEMOD_VIDEO_STANDARD_PAL_D				= 0x00000020,
    LX_ADEMOD_VIDEO_STANDARD_PAL_H				= 0x00000080,
    LX_ADEMOD_VIDEO_STANDARD_PAL_I				= 0x00000100,
    LX_ADEMOD_VIDEO_STANDARD_PAL_M				= 0x00000200,
    LX_ADEMOD_VIDEO_STANDARD_PAL_N				= 0x00000400,
    LX_ADEMOD_VIDEO_STANDARD_PAL_60			= 0x00000800,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_B			= 0x00001000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_D			= 0x00002000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_G			= 0x00004000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_H			= 0x00008000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_K			= 0x00010000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_K1			= 0x00020000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_L			= 0x00040000,
    LX_ADEMOD_VIDEO_STANDARD_SECAM_L1			= 0x00080000,
    LX_ADEMOD_VIDEO_STANDARD_PAL_NC			= 0x00100000,
	LX_ADEMOD_VIDEO_STANDARD_NTSC_50			= 0x00200000,
	LX_ADEMOD_VIDEO_STANDARD_PAL_G				= 0x00400000,
	LX_ADEMOD_VIDEO_STANDARD_PAL_K				= 0x00800000,
	LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM		= 0x01000000, // China, Hungary
} LX_ADEMOD_VideoStandard;

/*
**  Video bandwidth
*/
#define LX_ADEMOD_VIDEO_BANDWIDTH_NONE			(0)
#define LX_ADEMOD_VIDEO_BANDWIDTH_4p2MHz		(4200000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_5MHz			(5000000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_5p34MHz		(5340000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_5p35MHz		(5350000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_5p5MHz		(5500000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_6MHz			(6000000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_7MHz			(7000000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_8MHz			(8000000L)
#define LX_ADEMOD_VIDEO_BANDWIDTH_8p185MHz		(8185000L)

/*
**  Video frequency
*/
#define LX_ADEMOD_VIDEO_FREQUENCY_NONE			(0)
#define LX_ADEMOD_VIDEO_FREQUENCY_33p1			(33100000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_36MHz		(36000000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_36p125MHz	(36125000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_36p167MHz	(36167000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_38MHz		(38000000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_38p9MHz		(38900000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_44MHz		(44000000L)
#define LX_ADEMOD_VIDEO_FREQUENCY_45p75MHz		(45750000L)

/*
**  Format of the analog audio signal
*/
typedef enum LX_ADEMOD_AudioStandard_tag
{
	LX_ADEMOD_AUDIO_STANDARD_NONE,
	LX_ADEMOD_AUDIO_STANDARD_BTSC,
	LX_ADEMOD_AUDIO_STANDARD_EIAJ,
	LX_ADEMOD_AUDIO_STANDARD_A2,
	LX_ADEMOD_AUDIO_STANDARD_A2_1,
	LX_ADEMOD_AUDIO_STANDARD_A2_2,
	LX_ADEMOD_AUDIO_STANDARD_A2_3,
	LX_ADEMOD_AUDIO_STANDARD_NICAM,
	LX_ADEMOD_AUDIO_STANDARD_RADIO_US,
	LX_ADEMOD_AUDIO_STANDARD_RADIO_EU,
} LX_ADEMOD_AudioStandard;

/*
**  Analog audio mode
*/
typedef enum LX_ADEMOD_AudioMode_tag
{
	LX_ADEMOD_AUDIO_MODE_NONE,
	LX_ADEMOD_AUDIO_MODE_MONO,
	LX_ADEMOD_AUDIO_MODE_STEREO,
	LX_ADEMOD_AUDIO_MODE_SAP,
	LX_ADEMOD_AUDIO_MODE_ALT,
	LX_ADEMOD_AUDIO_MODE_FALLBACK,
	LX_ADEMOD_AUDIO_MODE_SIF,
} LX_ADEMOD_AudioMode;

/*
**  Audio frequency
*/
#define LX_ADEMOD_AUDIO_FREQUENCY_NONE			(0)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz		(4400000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p410MHz		(4410000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz		(4300000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p5MHz		(4500000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p724MHz		(4724000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p794MHz		(4794000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz		(4900000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz		(5250000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p26MHz		(5260000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz		(5266000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p430MHz		(5430000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p5MHz		(5500000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz		(5594000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p6MHz		(5600000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p672MHz		(5672000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p742MHz		(5742000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p85MHz		(5850000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p930MHz		(5930000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_5p941MHz		(5941000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6MHz			(6000000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p16MHz		(6160000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz		(6167000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p285MHz		(6285000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz		(6500000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p552MHz		(6552000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p643MHz		(6643000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p742MHz		(6742000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p75MHz		(6750000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_6p812MHz		(6812000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_7p1MHz		(7100000L)
#define LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz		(7220000L)

/*
**  AGC options
*/
typedef enum LX_ADEMOD_AGC_Options_tag {
	LX_ADEMOD_AGC_OPTION_VLTGPOL			= 0x00000001L,
	LX_ADEMOD_AGC_OPTION_RFDSBL			= 0x00000002L,
	LX_ADEMOD_AGC_OPTION_IFDSBL			= 0x00000004L,
	LX_ADEMOD_AGC_OPTION_EXTDSBL			= 0x00000008L,
	LX_ADEMOD_AGC_OPTION_OPEN_DRAIN		= 0x00000010L,
	LX_ADEMOD_AGC_OPTION_POSTSAW_EN		= 0x00000020L,
	LX_ADEMOD_AGC_OPTION_RFDSBLDIG			= 0x00000040L,
	LX_ADEMOD_AGC_OPTION_IFDSBLDIG			= 0x00000080L,
	LX_ADEMOD_AGC_OPTION_INSTABD_EN		= 0x00000100L,
	LX_ADEMOD_AGC_OPTION_BYPASSDIG			= 0x00000200L,
	LX_ADEMOD_AGC_OPTION_DSBLDIG			= 0x00000400L,
//	LX_ADEMOD_AGC_OPTION_MAG_NPRVSB		= 0x00000800L,
	LX_ADEMOD_AGC_OPTION_INSTABMON_SIGOPEN	= 0x00001000L,
	LX_ADEMOD_AGC_OPTION_DIG_GAINSEL		= 0x00002000L,
	LX_ADEMOD_AGC_OPTION_RFGAIN_INV		= 0x00004000L,
	LX_ADEMOD_AGC_OPTION_IFGAIN_INV		= 0x00008000L,
	LX_ADEMOD_AGC_OPTION_BITSPLIT_EN		= 0x00010000L,
	LX_ADEMOD_AGC_OPTION_SIFOUTDSBL		= 0x00020000L,
	LX_ADEMOD_AGC_OPTION_FASTMODE_EN		= 0x00040000L,
	LX_ADEMOD_AGC_OPTION_QNOISEFILTER_EN	= 0x00080000L,  //jeongpil.yun for M14
} LX_ADEMOD_AGC_Options;


/*
**  Audio options
*/
typedef enum LX_ADEMOD_Audio_Options_tag {
//	LX_ADEMOD_AUD_OPTION_BSEBAND_NSIFF		= 0x00000001L,
	LX_ADEMOD_AUD_OPTION_BUZZ_FIX_EN		= 0x00000002L,
	LX_ADEMOD_AUD_OPTION_AGC_EN			= 0x00000004L,
	LX_ADEMOD_AUD_OPTION_AGC_CLSLOOP_EN	= 0x00000008L,
	LX_ADEMOD_AUD_OPTION_SLAVETOVIDEO_DIS	= 0x00000010L,
} LX_ADEMOD_Audio_Options;

/*
**  TWA Master options
*/
typedef enum LX_ADEMOD_TWA_Master_Options_tag {
	LX_ADEMOD_TWA_OPTION_SLAVE_ACK		= 0x00000001L,
	LX_ADEMOD_TWA_OPTION_IGNORE_DEV		= 0x00000002L,
	LX_ADEMOD_TWA_OPTION_AUTO_INCREMENT	= 0x00000004L,
	LX_ADEMOD_TWA_OPTION_SPC_RD_REG		= 0x00000008L,
} LX_ADEMOD_TWA_Master_Options;


/*
**  Performance mode
*/
typedef enum LX_ADEMOD_PerformanceMode_tag
{
	LX_ADEMOD_PERFORMANCE_MODE_NONE,
    LX_ADEMOD_PERFORMANCE_MODE_HIGH,
    LX_ADEMOD_PERFORMANCE_MODE_LOW,
} LX_ADEMOD_PerformanceMode;

/*
**  Overmodulation mode
*/
typedef enum LX_ADEMOD_OvermodulationMode_tag
{
	LX_ADEMOD_OVERMODULATION_MODE_DISABLED,
    LX_ADEMOD_OVERMODULATION_MODE_FLYWHEEL,
    LX_ADEMOD_OVERMODULATION_MODE_WEIGHTED,
} LX_ADEMOD_OvermodulationMode;


/*
**  AGC mode
*/
typedef enum LX_ADEMOD_AGC_Mode_Options_tag {
	LX_ADEMOD_AGC_MODE_NONE,
	LX_ADEMOD_AGC_MODE_RF,
	LX_ADEMOD_AGC_MODE_IF,
	LX_ADEMOD_AGC_MODE_DIG,
	LX_ADEMOD_AGC_MODE_DIG_NBND,
} LX_ADEMOD_AGC_Mode_Options;

/*
**  AGC CVBS mode
*/
typedef enum LX_ADEMOD_AGC_CVBS_Mode_Options_tag {
	LX_ADEMOD_DIGAGC_CVBS_MODE_NONCOHERENT,
	LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT,
	LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED,
} LX_ADEMOD_AGC_CVBS_Mode_Options;

/*
**  AGC Piece-wise LPF Gain mode
*/
typedef enum LX_ADEMOD_AGC_LPF_Mode_Options_tag {
	LX_ADEMOD_AGC_LPF_MODE_WIDEBAND,
	LX_ADEMOD_AGC_LPF_MODE_MIXED,
	LX_ADEMOD_AGC_LPF_MODE_NARROWBAND,
	LX_ADEMOD_AGC_LPF_MODE_HYBRID,  //jeongpil.yun for M14
} LX_ADEMOD_AGC_LPF_Mode_Options;

/*
**  Dac Cross-bar source
*/
typedef enum LX_ADEMOD_DAC_CrossbarSource_tag
{
    LX_ADEMOD_CROSSBAR_SOURCE_AUDIO1 = 0,
    LX_ADEMOD_CROSSBAR_SOURCE_AUDIO2 = 1,
    LX_ADEMOD_CROSSBAR_SOURCE_VIDEO  = 2,
    LX_ADEMOD_CROSSBAR_SOURCE_IFGAIN = 3,
    LX_ADEMOD_CROSSBAR_SOURCE_RFGAIN = 4,
} LX_ADEMOD_DAC_CrossbarSource;

/*
**  Preset Options
*/
typedef enum LX_ADEMOD_PresetOption_tag {
	LX_ADEMOD_PRESET_OPTION_NONE      = 0,
	LX_ADEMOD_PRESET_OPTION_NOMINAL   = 1,
	LX_ADEMOD_PRESET_OPTION_MIDDLE    = 1,
	LX_ADEMOD_PRESET_OPTION_MIN       = 2,
	LX_ADEMOD_PRESET_OPTION_MAX       = 3,
	LX_ADEMOD_PRESET_OPTION_WIDE      = 4,
	LX_ADEMOD_PRESET_OPTION_FAST      = 4,
	LX_ADEMOD_PRESET_OPTION_HIGH      = 4,
	LX_ADEMOD_PRESET_OPTION_LONG      = 4,
	LX_ADEMOD_PRESET_OPTION_NARROW    = 5,
	LX_ADEMOD_PRESET_OPTION_SLOW      = 5,
	LX_ADEMOD_PRESET_OPTION_LOW       = 5,
	LX_ADEMOD_PRESET_OPTION_SHORT     = 5,
	LX_ADEMOD_PRESET_OPTION_LPCOUPLE  = 6,
	LX_ADEMOD_PRESET_OPTION_DISABLE   = 7,
	LX_ADEMOD_PRESET_OPTION_SLOWER    = 8,
	LX_ADEMOD_PRESET_OPTION_FASTER    = 9,
} LX_ADEMOD_PresetOption;

/*
**  Demodulator parameters
*/
typedef enum LX_ADEMOD_DemodParams_tag
{
	LX_ADEMOD_PARM_NONE,
	LX_ADEMOD_PARM_AUD_MODE,
	LX_ADEMOD_PARM_VID_FRQ,		// IF Input frequency
	LX_ADEMOD_PARM_AUD_FRQ,
	LX_ADEMOD_PARM_SMPL_RATE,
	LX_ADEMOD_PARM_PULLIN_RANGE,
	LX_ADEMOD_PARM_QLTY_MODE,
	LX_ADEMOD_PARM_SPCTR_INV,
	LX_ADEMOD_PARM_VID_BNDW,
	LX_ADEMOD_PARM_VID_GAIN,
	LX_ADEMOD_PARM_VID_OFFS,
	LX_ADEMOD_PARM_AUD_GAIN,
	LX_ADEMOD_PARM_AUD_CLIP,
	LX_ADEMOD_PARM_AUD_OFFS,
	LX_ADEMOD_PARM_LOCK_STAT,
	LX_ADEMOD_PARM_SNR,
	LX_ADEMOD_PARM_PWR,
	LX_ADEMOD_PARM_BER,
	LX_ADEMOD_PARM_GPIO,
	LX_ADEMOD_PARM_AGC_GAIN,
	LX_ADEMOD_PARM_AGC_OPTIONS,
	LX_ADEMOD_PARM_TS_OUTMODE,
	LX_ADEMOD_PARM_TS_CLKMODE,
	LX_ADEMOD_PARM_TS_CLKFREQ,
	LX_ADEMOD_PARM_TS_CLKPOL,
	LX_ADEMOD_PARM_TS_STARTPOL,
	LX_ADEMOD_PARM_TS_STARTDUR,
	LX_ADEMOD_PARM_TS_VALPOL,
	LX_ADEMOD_PARM_TS_ERRPOL,
	LX_ADEMOD_PARM_GRP_DEL,
	LX_ADEMOD_PARM_SYNC_DEC,
	LX_ADEMOD_PARM_DVBT_STAT,
	LX_ADEMOD_PARM_VIDCR_OVERMOD,
	LX_ADEMOD_PARM_ISALIVE_STAT,
	LX_ADEMOD_PARM_MICROALIVE_STAT,
	LX_ADEMOD_PARM_SYNC_STAT,
	LX_ADEMOD_PARM_CONFIG_OPTIONS,
	LX_ADEMOD_PARM_AGC_PARAMS,
	LX_ADEMOD_PARM_VSB_ENERGY,
	LX_ADEMOD_PARM_MAIN_CUTOFF,
	LX_ADEMOD_PARM_CARR_OFFS,
	LX_ADEMOD_PARM_PLL_STAT,
	LX_ADEMOD_PARM_API_VER,
	LX_ADEMOD_PARM_FW_VER,
	LX_ADEMOD_PARM_TS_CTRL,
	LX_ADEMOD_PARM_BULK_MODE,
	LX_ADEMOD_PARM_VIDCR_RANGE,
	LX_ADEMOD_PARM_AUD_SIF_BW,
	LX_ADEMOD_PARM_AUD_MIX,
	LX_ADEMOD_PARM_AGC_GAIN_THR,
	LX_ADEMOD_PARM_AGC_GAIN_PRESET,
	LX_ADEMOD_PARM_AGC_RFIIR_BW,
	LX_ADEMOD_PARM_AGC_LPD_ATTACK,
	LX_ADEMOD_PARM_AGC_LPD_DECAY,
	LX_ADEMOD_PARM_AGC_LPD_FASTDECAY,
	LX_ADEMOD_PARM_AGC_LPD_DECAYWIN,
	LX_ADEMOD_PARM_AGC_LPD_DECAYTHR,
	LX_ADEMOD_PARM_AGC_DECFACTOR,
	LX_ADEMOD_PARM_AGC_INSTAB_GAIN_ADJ,
	LX_ADEMOD_PARM_AGC_BNDW,
	LX_ADEMOD_PARM_AGC_SPEED,
	LX_ADEMOD_PARM_AGC_BACKOFF,
	LX_ADEMOD_PARM_DIGAGC_GAIN_THR,
	LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET,
	LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE,
	LX_ADEMOD_PARM_DIGAGC_LPD_ATTACK,
	LX_ADEMOD_PARM_DIGAGC_LPD_DECAY,
	LX_ADEMOD_PARM_DIGAGC_LPD_FASTDECAY,
	LX_ADEMOD_PARM_DIGAGC_LPD_DECAYWIN,
	LX_ADEMOD_PARM_DIGAGC_LPD_DECAYTHR,
	LX_ADEMOD_PARM_DIGAGC_BNDW,
	LX_ADEMOD_PARM_DIGAGC_SPEED,
	LX_ADEMOD_PARM_DIGAGC_BACKOFF,
	LX_ADEMOD_PARM_DIGAGC_BNDW_COH,
	LX_ADEMOD_PARM_DIGAGC_SPEED_COH,
	LX_ADEMOD_PARM_DIGAGC_BACKOFF_COH,
	LX_ADEMOD_PARM_VIDCR_FRLOOP_SPEED,
	LX_ADEMOD_PARM_VIDCR_FRLOOP_GAIN,
	LX_ADEMOD_PARM_VIDCR_PHLOOP_SPEED,
	LX_ADEMOD_PARM_VIDCR_PHLOOP_GAIN,
	LX_ADEMOD_PARM_VIDCR_LOCKDET,
	LX_ADEMOD_PARM_VIDCR_LOCKDETERR,
	LX_ADEMOD_PARM_VIDCR_LOCKDETLINES,
	LX_ADEMOD_PARM_VIDCR_UNLOCKDET,
	LX_ADEMOD_PARM_VIDCR_UNLOCKDETERR,
	LX_ADEMOD_PARM_VIDCR_UNLOCKDETLINES,
	LX_ADEMOD_PARM_VIDCR_MUTELOCKDET,
	LX_ADEMOD_PARM_VIDCR_MUTELOCKDETERR,
	LX_ADEMOD_PARM_VIDCR_MUTELOCKDETLINES,
	LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDET,
	LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETERR,
	LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETLINES,
	LX_ADEMOD_PARM_VIDCR_OVERMOD_WEIGHTING,
	LX_ADEMOD_PARM_VIDCR_OVERMOD_PHASEREV,
	LX_ADEMOD_PARM_VIDCR_RESET,
	LX_ADEMOD_PARM_VIDBW_VIRTUALFS,
	LX_ADEMOD_PARM_VIDBW_TINC,
	LX_ADEMOD_PARM_AUD_OPTIONS,
	LX_ADEMOD_PARM_AUD_SIF_FRQ,
	LX_ADEMOD_PARM_AUD_AGC_TARG,
	LX_ADEMOD_PARM_AUD_AGC_GAIN_THR,
	LX_ADEMOD_PARM_AUD_AGC_GAIN_PRESET,
	LX_ADEMOD_PARM_DAC_OPTIONS,
	LX_ADEMOD_PARM_VID_CLIP,
	LX_ADEMOD_PARM_DAC_SRCSEL,
	LX_ADEMOD_PARM_DAC_DITHER,
	LX_ADEMOD_PARM_VID_INSTAB,
	LX_ADEMOD_PARM_AUD_INSTAB,
	LX_ADEMOD_PARM_AGC_INSTAB,
	LX_ADEMOD_PARM_SYNC_OPTIONS,
	LX_ADEMOD_PARM_VIDCR_OUT_CF,
	LX_ADEMOD_PARM_AUD_AUTOMUTE,
	LX_ADEMOD_PARM_AUD_MUTESPEED,
	LX_ADEMOD_PARM_DIGAGC_NBND_THR,
	LX_ADEMOD_PARM_DIGAGC_NBND_OFFS,
	LX_ADEMOD_PARM_AUD_MONO_FRQ,
	LX_ADEMOD_PARM_AUD_DEVIATION,
	LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE,
	LX_ADEMOD_PARM_AUD_DEEMPHASIS,
	LX_ADEMOD_PARM_AUD_BSBNDEXTRAGAIN,
	LX_ADEMOD_PARM_AUD_SIF_OUT,
	LX_ADEMOD_PARM_FACTORY,
	LX_ADEMOD_PARM_DIGAGC_NBND_MODE,
	LX_ADEMOD_PARM_AUD_MONO_GAIN,
	LX_ADEMOD_PARM_AUD_BUZZDELAY,
	LX_ADEMOD_PARM_IF_LEVEL_STAT,
	LX_ADEMOD_PARM_VIDOFFS_TWEAK,
	LX_ADEMOD_PARM_LOWIF_BNDW,
	LX_ADEMOD_PARM_AGC_FASTMODE,
	LX_ADEMOD_PARM_AGC_OUTMODE,
	LX_ADEMOD_PARM_AGC_WANDMODE,
	LX_ADEMOD_PARM_RF_LEVEL_STAT,
	LX_ADEMOD_PARM_LOG_OPEN,
	LX_ADEMOD_PARM_LOG_CLOSE,
	LX_ADEMOD_PARM_SMPL_RATE_DIV,
	LX_ADEMOD_PARM_AGC_PHASE,
	LX_ADEMOD_PARM_I2C_WRITE_COUNT,
//jeongpil.yun for M14
	LX_ADEMOD_PARM_I2C_WRITE_OPTIM,
	LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET,
	LX_ADEMOD_PARM_DIGAGC_DECIMATION_RATIO,
	LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR,
	LX_ADEMOD_PARM_DIGAGC_LEVEL_DEP,
	LX_ADEMOD_PARM_DIGAGC_LEVEL_DEP_CTRL,
	LX_ADEMOD_PARM_DIGAGC_VSYNC_TILT,
	LX_ADEMOD_PARM_DIGAGC_VSYNC_TILT_CTRL,
	LX_ADEMOD_PARM_AGC_QNOISE_FILTER,
	LX_ADEMOD_PARM_CLOCK_DITHERING,

	LX_ADEMOD_PARM_VID_GAIN_RC,
	LX_ADEMOD_PARM_VID_OFFS_RC,
	LX_ADEMOD_PARM_VID_CLIP_RC,
	LX_ADEMOD_PARM_AUD_GAIN_RC,
	LX_ADEMOD_PARM_AUD_OFFS_RC,
	LX_ADEMOD_PARM_AUD_CLIP_RC,

//
	LX_ADEMOD_PARAM_MAXPARAM, // must be the last one
} LX_ADEMOD_DemodParams;

#define LX_ADEMOD_REGISTER_SET		(0xffff)

/*
**  Demodulator parameters' size
*/
#define LX_ADEMOD_PARM_SIZE(id) ( \
							(id == LX_ADEMOD_PARM_GRP_DEL)              ? (80) : \
							(id == LX_ADEMOD_PARM_VID_FRQ)              ? (8)  : \
							(id == LX_ADEMOD_PARM_DVBT_STAT)            ? (16) : \
							(id == LX_ADEMOD_PARM_AUD_FRQ)              ? (6)  : \
							(id == LX_ADEMOD_PARM_LOG_OPEN)             ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_SIF_FRQ)          ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_MONO_FRQ)         ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_GAIN)             ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_OFFS)             ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_CLIP)             ? (4)  : \
							(id == LX_ADEMOD_PARM_AUD_AGC_TARG)         ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_AGC_GAIN_THR)     ? (2)  : \
							(id == LX_ADEMOD_PARM_AUD_SIF_OUT)			 ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_GAIN)             ? (15) : \
							(id == LX_ADEMOD_PARM_AGC_GAIN_THR)         ? (4)  : \
							(id == LX_ADEMOD_PARM_AGC_GAIN_PRESET)      ? (3)  : \
							(id == LX_ADEMOD_PARM_AGC_RFIIR_BW)         ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_LPD_ATTACK)       ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_LPD_DECAY)        ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_LPD_FASTDECAY)    ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_LPD_DECAYWIN)     ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_LPD_DECAYTHR)     ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_DECFACTOR)        ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_INSTAB_GAIN_ADJ)  ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_BNDW)             ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_SPEED)            ? (5)  : \
							(id == LX_ADEMOD_PARM_AGC_INSTAB)           ? (2)  : \
							(id == LX_ADEMOD_PARM_AGC_BACKOFF)          ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_GAIN_THR)      ? (6)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET)   ? (4)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_LPD_ATTACK)    ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_LPD_DECAYWIN)  ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_LPD_DECAYTHR)  ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_BNDW)          ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_SPEED)         ? (7)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR)	? (7)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_BACKOFF)       ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_BNDW_COH)      ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_SPEED_COH)     ? (5)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_BACKOFF_COH)   ? (2)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_NBND_THR)      ? (3)  : \
							(id == LX_ADEMOD_PARM_DIGAGC_NBND_OFFS) 	? (4)  : \
							(id == LX_ADEMOD_PARM_VIDCR_RANGE)          ? (3)  : \
							(id == LX_ADEMOD_PARM_VIDCR_OVERMOD)        ? (11) : \
							(id == LX_ADEMOD_PARM_VIDCR_FRLOOP_SPEED)   ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_FRLOOP_GAIN)    ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_PHLOOP_SPEED)   ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_PHLOOP_GAIN)    ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_LOCKDETERR)     ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_LOCKDETLINES)   ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_UNLOCKDETERR)   ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_UNLOCKDETLINES) ? (2)  : \
							(id == LX_ADEMOD_PARM_VIDCR_MUTELOCKDETERR)     ? (2)  	: \
							(id == LX_ADEMOD_PARM_VIDCR_MUTELOCKDETLINES)   ? (2)  	: \
							(id == LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETERR)   ? (2)  	: \
							(id == LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETLINES) ? (2) 	: \
							(id == LX_ADEMOD_PARM_VIDCR_OVERMOD_PHASEREV)   ? (3)  	: \
							(id == LX_ADEMOD_PARM_VIDCR_RESET)				? (4)  	: \
							(id == LX_ADEMOD_PARM_VIDBW_TINC)				? (3)  	: \
							(id == LX_ADEMOD_PARM_DAC_SRCSEL)				? (4) 	: \
							(id == LX_ADEMOD_PARM_DAC_DITHER)				? (4) 	: \
							(id == LX_ADEMOD_PARM_AUD_MUTESPEED)			? (3) 	: \
							(id == LX_ADEMOD_PARM_AUD_MONO_GAIN)			? (5)  	: \
							(id == LX_ADEMOD_PARM_IF_LEVEL_STAT)			? (2)  	: \
							(id == LX_ADEMOD_PARM_AUD_BUZZDELAY)			? (2)  	: \
							(id == LX_ADEMOD_PARM_RF_LEVEL_STAT)			? (4)  	: \
							(id == LX_ADEMOD_PARM_VID_GAIN_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_VID_OFFS_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_VID_CLIP_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_AUD_GAIN_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_AUD_OFFS_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_AUD_CLIP_RC)				? (2)	: \
							(id == LX_ADEMOD_PARM_TS_CTRL)					? (7)  	: 1 \
						 )



/*
**  Sleep Modes
*/
typedef enum LX_ADEMOD_SleepMode_tag
{
	LX_ADEMOD_SLEEP_MODE_NONE, // wake up
	LX_ADEMOD_SLEEP_MODE_LEVEL_1, // sleep mode level 1
} LX_ADEMOD_SleepMode, *pADEMOD_SleepMode;


/*
**  Serial Master Clock speed
*/
typedef enum LX_ADEMOD_DvbtSerialClk_tag
{
	LX_ADEMOD_DVBT_SER_CLK_36MHz = 0,
	LX_ADEMOD_DVBT_SER_CLK_72MHz,
} LX_ADEMOD_DvbtSerialClk;

/*
**  Parallel Master Clock speed
*/
typedef enum LX_ADEMOD_DvbtParallelClk_tag
{
	LX_ADEMOD_DVBT_PAR_CLK_2MHz = 0,
	LX_ADEMOD_DVBT_PAR_CLK_4MHz,
	LX_ADEMOD_DVBT_PAR_CLK_9MHz,
	LX_ADEMOD_DVBT_PAR_CLK_18MHz,
} LX_ADEMOD_DvbtParallelClk;

/*
**  Gain
*/
typedef enum LX_ADEMOD_DvbtGain_tag
{
	LX_ADEMOD_DVBT_GAIN_X1 = 0,
	LX_ADEMOD_DVBT_GAIN_X2,
	LX_ADEMOD_DVBT_GAIN_X4,
	LX_ADEMOD_DVBT_GAIN_X8,
	LX_ADEMOD_DVBT_GAIN_X16,
	LX_ADEMOD_DVBT_GAIN_X32,
	LX_ADEMOD_DVBT_GAIN_X64,
} LX_ADEMOD_DvbtGain;

/*
**  DVB-T state
*/
typedef enum LX_ADEMOD_DvbtState_tag
{
	LX_ADEMOD_DVBT_STATE_IDLE = 0,
	LX_ADEMOD_DVBT_RESYNC,
	LX_ADEMOD_DVBT_RESYNCWAIT,
	LX_ADEMOD_DVBT_LOCKED,
	LX_ADEMOD_DVBT_RESTART,
	LX_ADEMOD_DVBT_MEASURE,
	LX_ADEMOD_DVBT_MEASUREWAIT,
	LX_ADEMOD_DVBT_MEASUREDONE,
	LX_ADEMOD_DVBT_FFTACQUIRE,
} LX_ADEMOD_DvbtState;

/*
**  FFT state
*/
typedef enum LX_ADEMOD_FftState_tag
{
	LX_ADEMOD_FFT_STATE_ACQUIRE0,
	LX_ADEMOD_FFT_ACQUIRE1,
	LX_ADEMOD_FFT_ACQUIRE2,
	LX_ADEMOD_FFT_ACQUIRE3,
	LX_ADEMOD_FFT_MONITOR,
} LX_ADEMOD_FftState;

/*
**  Demodulator instance
*/
#define MAX_SIZE_USER         (128)
typedef struct LX_ADEMOD_Demod_Instance_tag
{
    Handle_t    hUserData;
	UINT32      device_id;
	UINT32      input_mode;
	UINT32		video_std;
	UINT32		audio_std;
	UINT32		sleep_mode;
	UINT8		user[MAX_SIZE_USER];
}  LX_ADEMOD_Demod_Instance, *pLX_ADEMOD_Demod_Instance;


/*
**  Update flags:
**  LX_ADEMOD_UFLAG_UPDATE flag indicates that parameter has been modified and registers need to be updated.
**  This flag is used by ADEMOD_Parameter_Set() and other commands which have Update parameter.
*/
#define LX_ADEMOD_UFLAG_UPDATE				(0x80000000L) // update request

/*
**  Factory flags:
**  LX_ADEMOD_FFLAG_UPDATE flag indicates that factory settings are being updated.
**  This flag is used by ADEMOD_Parameter_Set() commands to indicate that this parameter
**  is being modified by factory setup and not by the user.
*/
#define LX_ADEMOD_FFLAG_UPDATE				(0x80000000L) // update request
#define LX_ADEMOD_FFLAG_A2					(0x00000001L) // A2
#define LX_ADEMOD_FFLAG_DBG_1				(0x00000002L) // deBug_1
#define LX_ADEMOD_FFLAG_E1					(0x00000004L) // E1   //jeognpil.yun for M14

/*
**  Device type
*/
#define LX_ADEMOD_DEVICE_TYPE_MASK			(0xffff0000)
#define LX_ADEMOD_DEVICE_INST_MASK			(0x000000ff)
#define LX_ADEMOD_DEVICE_DEBUG_MASK		(0x0000ff00)
#define LX_ADEMOD_DEVICE_TYPE(DeviceId)	(DeviceId & LX_ADEMOD_DEVICE_TYPE_MASK)
#define LX_ADEMOD_DEVICE_INST(DeviceId)	(DeviceId & LX_ADEMOD_DEVICE_INST_MASK)
#define LX_ADEMOD_DEVICE_DEBUG(DeviceId)	(DeviceId & LX_ADEMOD_DEVICE_DEBUG_MASK)

/*
**  DEBUG Flags
*/
#define LX_ADEMOD_DEMOD_DEBUG_HWSKIP_FLAG	(0x00000100) // when set, hardware will not be affected by the Open call
#define LX_ADEMOD_DEMOD_DEBUG_CACHE_FLAG	(0x00000200) // when set, cache will be used for register access
#define LX_ADEMOD_DEMOD_DEBUG_A2_FLAG		(0x00000400) // when set, hardware is A2, otherwise A1
#define LX_ADEMOD_DEMOD_deBug_1_FLAG		(0x00000800) // when set, deBug_1 is enabled, otherwise - disabled
#define LX_ADEMOD_DEMOD_DEBUG_I2COPT_FLAG	(0x00001000) // when set, cache will be used for optimizing I2C traffic



/******************************************************************************
**
**  Name: ADEMOD_Demod_Close
**
**  Description:    Closes instance of the demodulator.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Demod_Close(Handle_t hDevice);


/******************************************************************************
**
**  Name: ADEMOD_Demodulate
**
**  Description:    Starts demodulation in specified mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  InputMode    - The input mode. Can be one of the following:
**                                 LX_ADEMOD_INPUTMODE_LEGACY,
**                                 LX_ADEMOD_INPUTMODE_DVBT,
**                                 LX_ADEMOD_INPUTMODE_ATSC.
**                  VideoStandard - Legacy video standard (see LX_ADEMOD_VIDEO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_VIDEO_STANDARD_NONE.
**                  AudioStandard - Legacy audio standard (see LX_ADEMOD_AUDIO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_AUDIO_STANDARD_NONE.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Demodulate(Handle_t hDevice, UINT32 InputMode, UINT32 VideoStandard, UINT32 AudioStandard);


/******************************************************************************
**
**  Name: ADEMOD_Register_Set
**
**  Description:    Sets the value for specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (1 to 4 bytes).
**                  RegData      - Register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_Set(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 RegData);


/******************************************************************************
**
**  Name: ADEMOD_Register_SetX
**
**  Description:    Sets the value for specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (in bytes).
**                  RegData      - Register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  Note:			When called with UpdateNow = FALSE will optimize direct register
**					writes using cache.
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_SetX(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 RegData, BOOL UpdateNow);

/******************************************************************************
**
**  Name: ADEMOD_Register_Get
**
**  Description:    Gets the value of specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (1 to 4 bytes).
**                  pRegData     - Pointer to the register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_Get(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData);


/******************************************************************************
**
**  Name: ADEMOD_Parameter_Set
**
**  Description:    Sets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**                  UpdateNow    - When TRUE ?the value of the parameter will be
**                                 propagated into the registers during the function call;
**                                 When FALSE ?the value of the parameter will be
**                                 internally stored until any other function is called
**                                 with UpdateNow parameter equal TRUE or ADEMOD_Demodulate
**                                 function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Parameter_Set(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue, BOOL UpdateNow);


/******************************************************************************
**
**  Name: ADEMOD_Parameter_Get
**
**  Description:    Gets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Parameter_Get(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue);


/******************************************************************************
**
**  Name: ADEMOD_Firmware_Load
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pTable       - Pointer to the table containing firmware microcode,
**                  Size         - The size of the firmware microcode in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Firmware_Load(Handle_t hDevice, UINT8* pTable, UINT32 Size);


/******************************************************************************
**
**  Name: ADEMOD_AGC_Set
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see ADEMOD_AGC_Descriptor).
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or ADEMOD_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_AGC_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow);


/******************************************************************************
**
**  Name: ADEMOD_AGC_Get
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see ADEMOD_AGC_Descriptor).
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_AGC_Get(Handle_t hDevice, UINT32* pDescriptor);


/******************************************************************************
**
**  Name: ADEMOD_ChanScan_Set
**
**  Description:    Sets the channel scan profile for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or ADEMOD_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_ChanScan_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow);


/******************************************************************************
**
**  Name: ADEMOD_ChanScan_Get
**
**  Description:    Returns the channel scan status for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_ChanScan_Get(Handle_t hDevice, UINT32* pDescriptor);


/******************************************************************************
**
**  Name: ADEMOD_SleepMode_Set
**
**  Description:    Toggles the sleep mode on and off.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  Mode         - The sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_SleepMode_Set(Handle_t hDevice, UINT32 Mode);


/******************************************************************************
**
**  Name: ADEMOD_SleepMode_Get
**
**  Description:    Returns the sleep mode status.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pMode        - Pointer to the sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_SleepMode_Get(Handle_t hDevice, UINT32* pMode);


/******************************************************************************
**
**  Name: ADEMOD_HostI2C_Write
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be written,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_HostI2C_Write(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size);

/******************************************************************************
**
**  Name: ADEMOD_HostI2C_Read
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be read,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_HostI2C_Read(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size);

extern LX_ADEMOD_Result ADEMOD_GetContext(Handle_t hDevice, LX_ADEMOD_Demod_Instance * * ppLX_ADEMOD_instance); //LGIT 091010
#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_COMMON_DEMOD_H_
