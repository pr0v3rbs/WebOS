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



/** @file afe_kapi.h
 *
 *  application interface header for afe device
 *
 *  @author		wonsik.do (wonsik.do@lge.com)
 *  @version		1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_afe
 *	@{
 */

#ifndef	_AFE_KAPI_H_
#define	_AFE_KAPI_H_

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
#define	AFE_IOC_MAGIC		'a'

#define		BIT0		0x00000001

// For L9 4X oversampling for 480/576 I/P component inputs , This is workaround  for 72Mhz Noise on shibasoku generator.
//#define COMP_4X_OVERSAMPLING
#undef L8_KDRIVER_SUPPORT
/**
@name AFE IOCTL List
ioctl list for afe device.

@{

@def AFE_IO_ADC_INIT
Ininitializes registers of 3CH ADC block.

@def AFE_IOW_CVD_INIT
Ininitializes registers of Main or Sub CVD(3D Comb) block.
UINT8 arguments selects between Main and Sub CVDs

@def AFE_IOR_CVD_GET_STATES
Reads CVD's status registers(CVD1 Status Register1/2/3)
Read value will be used to determine CVBS sync status.
Parameter is UINIT32
|00000000|CVD1 STAUS REGISTER 3|CVD1 STAUS REGISTER 2|CVD1 STAUS REGISTER 1|

@def AFE_IOW_SET_HSYNC_DETECTION_FOR_TUNING
This IOCTL is used not to store garbage channels during analog channel scan.

@def AFE_IOW_SET_SCART_OVERLAY
Enables(or Disable) RGB Overlay on CVBS of FULL SCART.

@def AFE_IOW_CVD_SET_SOURCE_TYPE
Selects input port of CVD(CVBS1 to CVBS8) and source type (CVBS/S-Video).
For S-Video two input ports needed.

@def AFE_IOW_ADC_SET_SOURCE_TYPE
Selects input port of 3CH ADC.(RGB/YPbPr1/YPbPr2) and source type.

@def AFE_IOW_SET_ANALOG_COLOR_SYSTEM
Selects Color Standard of CVD.
TV for KOR/US can use this fuction to fix CVBS input color standard to NTSC.

@def AFE_IOR_GET_FB_STATUS
Reads voltage level of Scart FB(Fast Blank) pin.

@def AFE_IOR_GET_SCART_AR
Reads voltage level of scart 8 pin, and returns ASPECT RATIO info.

@def AFE_IOR_CVD_GET_TIMING_INFO
Gets color standard and timing information of CVBS signal.

@def AFE_IOR_CVD_GET_SVIDEO_TIMING_INFO
Gets color standard and timing information of S-VIDEO signal.

@def AFE_IOR_ADC_GET_TIMING_INFO
Gets the timiming information of ADC input signal.

@def AFE_IOW_SET_TTX_VIDEO_STANDARD
When color system is changed, this fuction change/updates TTX registers.

@def AFE_IOR_PC_AUTO_ADJUST
Perform PC Auto Adjust routine and return updated adjust values.

@def AFE_IOW_PC_ADJUST
Perform manual PC Adjust, according to the parameters.

@def AFE_IOW_ADC_AUTO_CALIBRATION
Perform ADC Auto Calibration routine.

@def AFE_IOW_ADC_SET_GAIN
Sets the Gain Register of 3CH ADC with given gain values

@def AFE_IOW_ADC_SET_OFFSET
Sets the Offset Register of 3CH ADC with given offset values

@def AFE_IOR_ADC_GET_GAIN
Reads the Gain Registers.

@def AFE_IOR_ADC_GET_OFFSET
Reads the Offset Registers.

@def AFE_IOW_SET_COMP_SYNC_LEVEL
Adjust Componenet Sync(SOG) level with give value(0~31)

@def AFE_IOW_SET_3DCOMB_VALUE
This function changes 3D comb filter values (when color system is chaged)

@def AFE_IOR_GET_PCMODE_INFO
This fuction reads current PC mode information from driver

//@def AFE_IOR_GET_PCMODE_TABLE
//This fuction read default PC mode table value from driver

@def AFE_IOW_SET_PCMODE_TABLE
This fuction send default PC mode table value to driver

@def AFE_IOW_SET_USER_PCMODE_RESOLUTION
This fuction select one resoultion among followings(LX_RES_1024x768, LX_RES_1280x768, LX_RES_1360x768, LX_RES_1366x768)

@def AFE_IOW_ADC_SET_SCART_MODE
If the parameter is TRUE, this ioctl enables SCART RGB input through ADC's component1 path, and select clk source from CVD(Not from LLPLL of ADC), if FALSE selects component or RGB-PC inputs.

@def AFE_IOW_VPORT_OUTPUT
This fuction select vport output source(CVD/HDMI/ADC) and output channel(CVI_A/CVI_B).

@def AFE_IOW_CVD_PICTURE_ENHANCEMENT
Set picture enhancement registers and on/off picture enhancement.

@def AFE_IOR_VPORT_REG_READ
Reads Vport Register

@def AFE_IOW_VPORT_REG_WRITE
Write Vport Register

@def AFE_IOW_CVD_STOP_TIMER
Stop CVD main/chb timer

@def AFE_IOW_CVD_START_TIMER
Start CVD main/chb timer

@def AFE_IOW_CVD_PROGRAM_COLOR_SYSTEM
Program CVD registers to the specified color system

@def AFE_IOW_ADC_SET_LLPLL
Set the LLPLL by index value

@def AFE_IOW_CVD_SET_VBI_TYPE
Set VBI decoder type(analogttx or analogcc)

@def AFE_IOR_ADC_SYNC_EXIST
Check Sync status on selectec ADC port

@def AFE_IOW_ADC_TEST
Used only for test

@def AFE_IOW_ADC_RESET_DIGITAL
This ioctl reset digital block 3ch adc.

@def AFE_IOW_ADC_RESET_DIGITAL_24MHZ
This ioctl reset digital block 3ch adc 24MHZ.

@def AFE_IOW_ADC_RESET_LLPLL
This ioctl reset llpll block 3ch adc.

@def AFE_IOW_CVD_SET_SW_3DCOMB_CONTROL
This ioctl enable/disable SW 3DCOMB control and set threshold values.

@def AFE_IOW_ADC_POWER_CONTROL
This ioctl control power on/off of ADC block to reduce power consumption.

@def AFE_IOW_CVD_POWER_CONTROL
This ioctl control power on/off of CVD block to reduce power consumption.

@def AFE_IOR_ADC_READ_PIXEL_VALUE
This ioctl read digital R/G/B pixel value(for test purpose)

@def AFE_IOW_ADC_ENABLE_PERIODIC_SIGNAL_INFOR_READ
Enable/Disable Periodic Input Format Read at adc_gettiminginfo()

@def AFE_IOW_ADC_EXECUTE_FORMAT_DETECTION
Excute Format Detection(IFM()) work_queue to force input signal detection.

@def AFE_IOW_ADC_ENABLE_COMPONENT_AUTO_PHASE
Enable/Disable execution of auto phase workequeue on component signal.

@def AFE_IOW_CVD_ENABLE_DETECTION_INTERRUPT
Enable/Disable CVD Detection Interrupts.

@def AFE_IOR_CVD_GET_STATES_DETAIL
Get Detailed states of CVD

@def	AFE_IOW_CVD_BYPASS_CONTROL
Select a CVBS source for CVBS buf_out1/2

@def AFE_IOW_CVD_VDAC_POWER_CONTROL
Controls power on/off of VDAC block power to mute/unmute DENC output.

@def AFE_IOW_LVDS_SRC_CONTROL
Select A-Die HS-LVDS power/data-type/source settings

@def AFE_IOR_GET_MEM_CFG
Read kernel memory configuration from each revision.

@def	AFE_IOR_READ_ACE_REG			
Read Register Value of ACE(A-Die)

@def	AFE_IOW_WRITE_ACE_REG			
Write Register Value to ACE(A-Die)

@def	AFE_IOR_GET_CK_DETCTION
Read CVD Crunky Detection Registers

@def AFE_IOW_DEBUG_PRINT_CTRL
Enable or disable debug print for debug.
This function is used to debug ADEC module for debug purpose.

@def AFE_IOR_DETECT_COMPONENT_PSEUDO_PULSE
Get Component Pseudo Pulse State.

@def AFE_IOW_SET_CVD_BLACK_LEVEL
Send Current UI(PE) black level value to CVD Kernel Driver.
NTSC-M and NTSC-J have different blanking setup level.(PAL and PAL-N also)
To set valid blanking level, CVD driver use PE black level value.

@def AFE_IOW_CVD_CHANNEL_CHANGE
Notify UI ATV Channel Change Condition to CVD Kernel Driver.

@def AFE_IOR_CVD_GET_NOISE_STATUS
Read CVD Noise Status Value(10bit)

@def AFE_IOW_CVD_VDAC_MUTE_CONTROL
Control VDAC Mute using vdac swrst (not using VDAC PDB)

@def AFE_IOR_GET_COMPONENT_VBI_CP
Get Copy Protection Informations in Component VBI

@def AFE_IO_FLUSH_COMPONENT_VBI_CP
Flush Component VBI Copy Protection Data

@def AFE_IOW_SET_INTERNAL_DEMOD_MODE
Set Internal/External Demod Mode to CVD

control complex parameter to afe module.
control codec_type, speed, and additional parameter.
no need to use "see also", exact struct name shall be linked automatically.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
//#define	AFE_IO_RESET				_IO(AFE_IOC_MAGIC,  0 )
//#define AFE_IOR_CHIP_REV_INFO		_IOR(AFE_IOC_MAGIC, 1, CHIP_REV_INFO_T )
//#define AFE_IOW_WRITE_UINT32		_IOW(AFE_IOC_MAGIC, 2, UINT32 )
//#define AFE_IOW_WRITE_STRUCT		_IOW(AFE_IOC_MAGIC, 3, LX_AFE_IOCTL_PARAM_T)
#define AFE_IO_ADC_INIT			_IO(AFE_IOC_MAGIC, 4)
#define AFE_IOW_CVD_INIT			_IOW(AFE_IOC_MAGIC, 5, UINT32)
#define AFE_IOR_CVD_GET_STATES		_IOWR(AFE_IOC_MAGIC, 6, LX_AFE_CVD_STATES_INFO_T)
#define AFE_IOW_SET_HSYNC_DETECTION_FOR_TUNING	_IOW(AFE_IOC_MAGIC, 7, BOOLEAN)
#define AFE_IOW_SET_SCART_OVERLAY	_IOW(AFE_IOC_MAGIC, 8, BOOLEAN)
//#define AFE_IOW_CVD_SET_PORT		_IOW(AFE_IOC_MAGIC, 9, LX_AFE_CVD_PORT_T)
//#define AFE_IOW_ADC_SET_PORT		_IOW(AFE_IOC_MAGIC, 10, LX_AFE_ADC_PORT_T)
#define AFE_IOW_CVD_SET_SOURCE_TYPE		_IOW(AFE_IOC_MAGIC, 11, LX_AFE_CVD_SET_INPUT_T)
#define AFE_IOW_ADC_SET_SOURCE_TYPE		_IOW(AFE_IOC_MAGIC, 12, LX_AFE_ADC_SET_INPUT_T)
#define AFE_IOW_SET_ANALOG_COLOR_SYSTEM 	_IOW(AFE_IOC_MAGIC, 13, LX_AFE_VIDEO_SYSTEM_INFO_T)
#define AFE_IOR_GET_FB_STATUS		_IOR(AFE_IOC_MAGIC, 14, LX_AFE_SCART_MODE_T)
#define AFE_IOR_GET_SCART_AR		_IOWR(AFE_IOC_MAGIC, 15, LX_AFE_SCART_AR_INFO_T)
#define AFE_IOR_CVD_GET_TIMING_INFO		_IOWR(AFE_IOC_MAGIC, 20, LX_AFE_CVD_TIMING_INFO_T)
#define AFE_IOR_CVD_GET_SVIDEO_TIMING_INFO		_IOWR(AFE_IOC_MAGIC, 21, LX_AFE_CVD_TIMING_INFO_T)
#define AFE_IOR_ADC_GET_TIMING_INFO		_IOR(AFE_IOC_MAGIC, 22, LX_AFE_ADC_TIMING_INFO_T)
#define AFE_IOW_SET_TTX_VIDEO_STANDARD	_IOW(AFE_IOC_MAGIC, 23, LX_AFE_VIDEO_SYSTEM_T)
#define AFE_IOR_PC_AUTO_ADJUST		_IOR(AFE_IOC_MAGIC, 24, LX_AFE_ADJ_PC_T)
#define AFE_IOW_PC_ADJUST			_IOW(AFE_IOC_MAGIC, 25, LX_AFE_ADJ_PCMODE_INFO_T)
#define AFE_IOW_ADC_AUTO_CALIBRATION	_IOW(AFE_IOC_MAGIC, 26, LX_AFE_ADC_CALIB_INFO_T)
#define AFE_IOW_ADC_SET_GAIN			_IOW(AFE_IOC_MAGIC, 27, LX_AFE_ADC_GAIN_VALUE_T)
#define AFE_IOW_ADC_SET_OFFSET			_IOW(AFE_IOC_MAGIC, 28, LX_AFE_ADC_OFFSET_VALUE_T)
#define AFE_IOR_ADC_GET_GAIN			_IOR(AFE_IOC_MAGIC, 29, LX_AFE_ADC_GAIN_VALUE_T)
#define AFE_IOR_ADC_GET_OFFSET			_IOR(AFE_IOC_MAGIC, 30, LX_AFE_ADC_OFFSET_VALUE_T)
#define AFE_IOW_SET_COMP_SYNC_LEVEL	_IOW(AFE_IOC_MAGIC, 31, UINT32)
#define	AFE_IOW_SET_3DCOMB_VALUE	_IOW(AFE_IOC_MAGIC, 32, LX_AFE_CVD_PQ_MODE_T)
#define AFE_IOR_GET_PCMODE_INFO	_IOR(AFE_IOC_MAGIC, 33, LX_AFE_PCMODE_INFO_T)
//#define AFE_IOR_GET_PCMODE_TABLE	_IOR(AFE_IOC_MAGIC, 34, LX_AFE_PCMODE_TABLE_T)
#define AFE_IOW_SET_PCMODE_TABLE	_IOW(AFE_IOC_MAGIC, 35, LX_AFE_SET_PCMODE_TABLE_T)
#define	AFE_IOW_SET_USER_PCMODE_RESOLUTION	_IOW(AFE_IOC_MAGIC, 36, LX_AFE_RESOLUTION_TYPE_T)

#define	AFE_IOW_INTERFACE	_IOW(AFE_IOC_MAGIC, 37, LX_AFE_RESOLUTION_TYPE_T)
#define AFE_IOW_ADC_SET_SCART_MODE		_IOW(AFE_IOC_MAGIC, 38, BOOLEAN)
#define AFE_IOW_VPORT_OUTPUT		_IOW(AFE_IOC_MAGIC, 39, LX_AFE_VPORT_OUT_INFO_T)
#define AFE_IOW_CVD_PICTURE_ENHANCEMENT 	_IOW(AFE_IOC_MAGIC, 40, LX_AFE_PICTURE_ENHANCEMENT_ONOFF_T)
#define AFE_IOR_VPORT_REG_READ			_IOR(AFE_IOC_MAGIC, 41, LX_AFE_REG_RW_T)
#define AFE_IOW_VPORT_REG_WRITE			_IOW(AFE_IOC_MAGIC, 42, LX_AFE_REG_RW_T)
#define AFE_IOW_CVD_STOP_TIMER			_IOW(AFE_IOC_MAGIC, 43, LX_AFE_CVD_TIMER_T)
#define AFE_IOW_CVD_START_TIMER			_IOW(AFE_IOC_MAGIC, 44, LX_AFE_CVD_TIMER_T)
#define AFE_IOW_CVD_PROGRAM_COLOR_SYSTEM	_IOW(AFE_IOC_MAGIC, 45, LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T)
#define AFE_IOW_ADC_SET_LLPLL			_IOW(AFE_IOC_MAGIC, 46, UINT32)
#define AFE_IOW_CVD_SET_VBI_TYPE		_IOW(AFE_IOC_MAGIC, 47, UINT32)
#define AFE_IOR_ADC_SYNC_EXIST			_IOR(AFE_IOC_MAGIC, 48, UINT32)
#define AFE_IOW_ADC_TEST				_IOW(AFE_IOC_MAGIC, 49, UINT32)
#define AFE_IOW_CVD_TEST				_IOW(AFE_IOC_MAGIC, 50, UINT32)
#define AFE_IOW_ADC_RESET_DIGITAL		_IOW(AFE_IOC_MAGIC, 51, UINT32)
#define AFE_IOW_ADC_RESET_LLPLL			_IOW(AFE_IOC_MAGIC, 52, UINT32)
#define AFE_IOW_CVD_SET_SW_3DCOMB_CONTROL	_IOW(AFE_IOC_MAGIC, 53, UINT32)
#define AFE_IOW_ADC_POWER_CONTROL			_IOW(AFE_IOC_MAGIC, 54, UINT32)
#define AFE_IOW_CVD_POWER_CONTROL			_IOW(AFE_IOC_MAGIC, 55, UINT32)
#define AFE_IOR_ADC_READ_PIXEL_VALUE		_IOR(AFE_IOC_MAGIC, 56, UINT32)
#define AFE_IOW_ADC_ENABLE_PERIODIC_SIGNAL_INFO_READ		_IOW(AFE_IOC_MAGIC, 57, UINT32)
#define AFE_IOW_ADC_EXECUTE_FORMAT_DETECTION		_IOW(AFE_IOC_MAGIC, 58, UINT32)
#define AFE_IOW_ADC_ENABLE_COMPONENT_AUTO_PHASE		_IOW(AFE_IOC_MAGIC, 59, UINT32)
#define AFE_IOW_CVD_ENABLE_DETECTION_INTERRUPT		_IOW(AFE_IOC_MAGIC, 60, UINT32)
#define	AFE_IOR_CVD_GET_STATES_DETAIL	_IOR(AFE_IOC_MAGIC, 61, UINT32)
#define AFE_IOR_CVD_GET_3DCOMB_PARAM	_IOR(AFE_IOC_MAGIC, 62, UINT32)
#define AFE_IOR_CVD_SET_3DCOMB_PARAM	_IOR(AFE_IOC_MAGIC, 63, UINT32)
#define	AFE_IOW_CVD_BYPASS_CONTROL		_IOW(AFE_IOC_MAGIC, 64, UINT32)
#define	AFE_IOW_CVD_VDAC_POWER_CONTROL		_IOW(AFE_IOC_MAGIC, 65, UINT32)
#define	AFE_IOW_LVDS_SRC_CONTROL				_IOW(AFE_IOC_MAGIC, 66, UINT32)
#define AFE_IOW_CVD_SET_HSTATE_MAX					_IOW(AFE_IOC_MAGIC, 67, UINT32)
#define AFE_IOW_ADC_RESET_DIGITAL_24MHZ		_IOW(AFE_IOC_MAGIC, 68, UINT32)
#define	AFE_IOR_GET_MEM_CFG					_IOR(AFE_IOC_MAGIC, 69, UINT32)
#define	AFE_IOR_READ_ACE_REG					_IOR(AFE_IOC_MAGIC, 70, UINT32)
#define	AFE_IOW_WRITE_ACE_REG					_IOW(AFE_IOC_MAGIC, 71, UINT32)
#define	AFE_IOR_GET_CK_DETCTION					_IOW(AFE_IOC_MAGIC, 72, UINT32)
#define AFE_IOW_DEBUG_PRINT_CTRL					_IOW(AFE_IOC_MAGIC,  73, UINT32)
#define AFE_IOR_DETECT_COMPONENT_PSEUDO_PULSE		_IOR(AFE_IOC_MAGIC,  74, UINT32)
#define AFE_IOW_SET_CVD_BLACK_LEVEL		_IOR(AFE_IOC_MAGIC,  75, UINT32)
#define AFE_IOR_GET_ACE_FB_STATUS		_IOR(AFE_IOC_MAGIC, 76, LX_AFE_SCART_MODE_T)
#define AFE_IOW_SET_SCART_BYPASS_MODE		_IOW(AFE_IOC_MAGIC, 77, LX_AFE_SCART_BYPASS_MODE_T)
#define AFE_IOW_CVD_CHANNEL_CHANGE		_IOW(AFE_IOC_MAGIC, 78, UINT32)
#define AFE_IOR_CVD_GET_NOISE_STATUS		_IOR(AFE_IOC_MAGIC, 79, UINT32)
#define	AFE_IOW_CVD_VDAC_MUTE_CONTROL		_IOW(AFE_IOC_MAGIC, 80, UINT32)
#define	AFE_IOR_GET_COMPONENT_VBI_CP		_IOR(AFE_IOC_MAGIC, 81, UINT32)
#define	AFE_IO_FLUSH_COMPONENT_VBI_CP		_IO(AFE_IOC_MAGIC, 82)
#define	AFE_IOW_SET_INTERNAL_DEMOD_MODE		_IOW(AFE_IOC_MAGIC, 83, BOOLEAN)
#define AFE_IOC_MAXNR		84
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

#define SC_YPBPR_720X480_60_SW_PATCH FALSE

///<Flags for mode table
#define AFE_PCMODE_FLAG_NULL		0x00
#define AFE_PCMODE_FLAG_POR_HPVP	0x01
#define AFE_PCMODE_FLAG_POR_HPVN	0x02
#define AFE_PCMODE_FLAG_POR_HNVP	0x04
#define AFE_PCMODE_FLAG_POR_HNVN	0x08
#define AFE_PCMODE_FLAG_INTERLACE	0x10
#define AFE_PCMODE_FLAG_EURO_HDTV_BIT	0x20
#define AFE_PCMODE_FLAG_YPBPR_BIT 	0x40
#define AFE_PCMODE_FLAG_HDTV_BIT	0x80

///<Flags for CVD status
#define LX_NO_SIGNAL 			0x00000001	///< No signal detection
#define LX_HLOCK				0x00000002	///< Horizontal Line Lock
#define LX_VLOCK				0x00000004	///< Vertical Line Lock
#define LX_CHROMALOCK			0x00000008	///< Choma PLL locked to color burst
#define CK_VBI_DETECTED			0x00000010			///< Crunky PSP detection
#define CK_COLOURSTRIPSE		0x00000020			///< Crunky CS detected.
#define LX_PROSCAN_DETECTED 	0x00000100	///< Progressive Scan detected.
#define LX_HNON_STANDARD		0x00000200	///< Horizontal freq. non-standard input signal detected.
#define LX_VNON_STANDARD		0x00000400	///< Vertical fre. non-standard input signal detected.
#define LX_NOBURST_DETECTED		0x00000800 		///< Chroma burst detection indicator, check the datasheet
#define LX_STATUS_COMB3D_OFF	0x00001000	///< On/Off status of the 3D comb filter
#define LX_PAL_DETECTED			0x00010000		///< PAL color mode detected.
#define LX_SECAM_DETECTED		0x00020000	///< SECAM color mode detected.
#define LX_625LINES_DETECTED	0x00040000	///< 625 Scan Line detected
#define LX_NOISY				0x00080000	///< Noisy Signal detected.
#define LX_VCR					0x00100000	///< VCR detected
#define LX_CS0_PAL_DETECTED		0x00200000	///< CS0 PAL Detected
#define LX_CS0_SECAM_DETECTED	0x00400000	///< CS0 SECAM Detected
#define LX_CS0_CHROMALOCK		0x00800000	///< CS0 CHROMALOCK
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

typedef struct
{
	unsigned int read_write;
	unsigned int sub;
	unsigned int addr;
	unsigned int size;
	unsigned int value;

}S_INTERFACE_REG;

/**
 * This enum is used to select AFE output source
 * @see LX_AFE_VPORT_OUT_INFO_T
 */
typedef enum
{
   LX_AFE_VPORT_OUT_CVD = 0x0,
   LX_AFE_VPORT_OUT_HDMI = 0x1,
   LX_AFE_VPORT_OUT_ADC = 0x2,
} LX_AFE_VPORT_OUT_SRC_T;

/**
 * This enum is used to select CVI input port between CVI_A and CVI_B
 * @see LX_AFE_VPORT_OUT_INFO_T
 */
typedef enum
{
   LX_AFE_VPORT_OUT_A,
   LX_AFE_VPORT_OUT_B,
} LX_AFE_VPORT_OUT_CH_T;

/**
 * This structure is used to select output source and CVI input port
 * @see AFE_IOW_VPORT_OUTPUT
 */
typedef struct
{
	LX_AFE_VPORT_OUT_SRC_T sel_out_src;
	LX_AFE_VPORT_OUT_CH_T sel_out_ch;
} LX_AFE_VPORT_OUT_INFO_T;

/**
 * This enum is used to select between Main CVD and Sub CVD
 * Sub CVD is used to capture video's for Channel Browser.
 * @see LX_AFE_CVD_STATES_INFO_T
 */
typedef enum {
	LX_CVD_MAIN = 0, ///< select main cvd
	LX_CVD_SUB = 1, ///< select sub cvd
	LX_CVD_MAX_NUM = 2, ///< max cvd number
} LX_AFE_CVD_SELECT_T;

/**
 * This structure is used to receive CVD's status register information from kernel driver.
 * @see AFE_IOW_CVD_SET_SOURCE_TYPE
 */
typedef struct {
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	UINT32						cvd_status;		///< status register value of cvd
} LX_AFE_CVD_STATES_INFO_T;
/**
 * This enum is used to select input port of CVD.
 * @see AFE_CVD_SET_INPUT_T
 */
typedef enum {
	LX_AFE_CVBS_IN1 = 1,		///< CVBS_Input_1
	LX_AFE_CVBS_IN2 = 2,		///< CVBS_Input_2
	LX_AFE_CVBS_IN3 = 3,		///< CVBS_Input_3
	LX_AFE_CVBS_IN4 = 4,		///< CVBS_Input_4
	LX_AFE_CVBS_IN5 = 5,		///< CVBS_Input_5
	LX_AFE_CVBS_IN6 = 6,		///< CVBS_Input_6
	LX_AFE_CVBS_IN7 = 7,		///< CVBS_Input_7
	LX_AFE_CVBS_IN8 = 8,		///< CVBS_Input_8
	LX_AFE_CVBS_NONE,		///< No input port selection
	LX_AFE_CVBS_IN_NUM,		///< CVBS input port number
} LX_AFE_CVD_INPUT_PORT_T;

/**
 * This enum is used to select input source type of CVD.
 * @see AFE_CVD_SET_INPUT_T
 */
typedef enum {
	LX_CVD_INPUT_SOURCE_CVBS = 0,	///< input signal is CVBS
	LX_CVD_INPUT_SOURCE_SVIDEO = 1,	///< input signal is S-VIDEO
	LX_CVD_INPUT_SOURCE_NUM,	///< input signal number
} LX_AFE_CVD_SOURCE_TYPE_T;

/**
 * This enum is used to set attributes of input CVD source type whether it is RF or AV.
 * @see AFE_CVD_SET_INPUT_T
 */
typedef enum {
	LX_CVD_INPUT_SOURCE_ATTRIBUTE_RF = 0,	///< input signal is from RF
	LX_CVD_INPUT_SOURCE_ATTRIBUTE_AV = 1,	///< input signal is from AV
	LX_CVD_INPUT_SOURCE_ATTRIBUTE_SCART = 2,	///< input signal is from SCART
	LX_CVD_INPUT_SOURCE_ATTRIBUTE_NUM,
} LX_AFE_CVD_SOURCE_ATTRIBUTE_T;

/**
 * This structure is used to transfer input port & input source type of CVD to kernel driver.
 * For S-VIDEO chroma_input_port value is needed.
 * @see AFE_IOW_CVD_SET_SOURCE_TYPE
 */
typedef struct {
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	LX_AFE_CVD_SOURCE_TYPE_T	cvd_input_source_type;	///< cvd input source type
	LX_AFE_CVD_INPUT_PORT_T	cvbs_input_port;			///< cvbs input port
	LX_AFE_CVD_INPUT_PORT_T	chroma_input_port;			///< chroma input port
	LX_AFE_CVD_SOURCE_ATTRIBUTE_T	cvd_input_source_attribute;
} LX_AFE_CVD_SET_INPUT_T;

/**
 * This enum is used to select input port of 3CH ADC.
 * @see AFE_ADC_SET_INPUT_T
 */
typedef enum {
	LX_ADC_RGB_IN,		///<RGB input port
	LX_ADC_YPbPr_IN1,	///< YPbPr 1 input port
	LX_ADC_YPbPr_IN2,	///< YPbPr 2 input port
	LX_ADC_IN_NUM,		///< ADC inpot port number
} LX_AFE_ADC_INPUT_PORT_T;

/**
 * This enum is used to select input source type of 3CH ADC.
 * @see AFE_ADC_SET_INPUT_T
 */
typedef enum {
	LX_ADC_INPUT_SOURCE_RGB_PC,		///< RGB PC
	LX_ADC_INPUT_SOURCE_RGB_SCART,	///< Scart RGB
	LX_ADC_INPUT_SOURCE_YPBPR,		///< YPbPr
	LX_ADC_INPUT_SOURCE_NONE,		///< ADC input source not selected
} LX_AFE_ADC_INPUT_SOURCE_TYPE_T;

/**
 * This structure is used to transfer input port & input source type of 3CH ADC to kernel driver.
 * @see AFE_IOW_ADC_SET_SOURCE_TYPE
 */
typedef struct {
	LX_AFE_ADC_INPUT_SOURCE_TYPE_T	adc_input_source_type;	///< adc input source type
	LX_AFE_ADC_INPUT_PORT_T	adc_input_port;					///< adc input port
}	LX_AFE_ADC_SET_INPUT_T;

/**
 * This enum is used to set color standard of CVD.
 * @see AFE_IOW_SET_ANALOG_COLOR_SYSTEM, LX_AFE_CVD_TIMING_INFO_T, LX_AFE_VIDEO_SYSTEM_INFO_T
 */
typedef enum {
	LX_DEFAULT = 0,
	LX_NTSC_M =1,			///< NTSC_M
	LX_NTSC_J =2,			///< NTSC_J
	LX_NTSC_443 =3,		///< NTSC 4.43
	LX_PAL_BG =4,			///< PAL BG
	LX_PAL_N =5,			///< PAL N
	LX_PAL_M =6,			///< PAL M
	LX_PAL_CN =7,			///< PAL Nc
	LX_PAL_60 =8,			///< PAL 60
	LX_SECAM =9,			///< SECAM
	LX_NON_STANDARD =10,	///< system unknown
	LX_MULTI,			///< system multi
	LX_VIDEO_SYSTEM_NUM,///< video system max num
} LX_AFE_VIDEO_SYSTEM_T;

/**
 * For analog color system bit masking
 *
 * @see
 */
typedef enum
{
    LX_VIDEO_MODE_NTSC        = 0,
    LX_VIDEO_MODE_PAL            ,
    LX_VIDEO_MODE_PAL_N          ,
    LX_VIDEO_MODE_PAL_M          ,
    LX_VIDEO_MODE_SECAM          ,
    LX_VIDEO_MODE_NTSC_443       ,
    LX_VIDEO_MODE_PAL_60         ,
    LX_VIDEO_MODE_UNKNOWN_525    ,
    LX_VIDEO_MODE_UNKNOWN_625    ,
    LX_VIDEO_MODE_UNKNOWN        ,
} LX_AFE_CVD_VIDEO_MODE_T;

/**
 * Analog TV color system to support .
 *
 * @see
*/
typedef enum
{
     LX_COLOR_SYSTEM_NTSC_M    = BIT0 << LX_VIDEO_MODE_NTSC     ,
     LX_COLOR_SYSTEM_PAL_G     = BIT0 << LX_VIDEO_MODE_PAL      ,
     LX_COLOR_SYSTEM_PAL_NC    = BIT0 << LX_VIDEO_MODE_PAL_N    ,
     LX_COLOR_SYSTEM_PAL_M     = BIT0 << LX_VIDEO_MODE_PAL_M    ,
     LX_COLOR_SYSTEM_SECAM     = BIT0 << LX_VIDEO_MODE_SECAM    ,
     LX_COLOR_SYSTEM_NTSC_443  = BIT0 << LX_VIDEO_MODE_NTSC_443 ,
     LX_COLOR_SYSTEM_PAL_60    = BIT0 << LX_VIDEO_MODE_PAL_60   ,
     LX_COLOR_SYSTEM_PAL_N                                         ,  /* TBD should be remove */
	 LX_COLOR_SYSTEM_MULTI		= (LX_COLOR_SYSTEM_NTSC_M | LX_COLOR_SYSTEM_PAL_G | LX_COLOR_SYSTEM_PAL_NC |LX_COLOR_SYSTEM_PAL_M |LX_COLOR_SYSTEM_SECAM | LX_COLOR_SYSTEM_NTSC_443 | LX_COLOR_SYSTEM_PAL_60),
     LX_COLOR_SYSTEM_UNKNOWN                                       ,  /* TBD should be remove */

} LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T;

/**
 * This structure is used to trasfer color standard of cvd to kernel driver.
 * @see AFE_IOW_SET_ANALOG_COLOR_SYSTEM
 */
typedef struct {
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T 	cvd_video_system;			///< cvd video system
} LX_AFE_VIDEO_SYSTEM_INFO_T;

/**
 * This structure is used to receive color standard and timing information of CVD from kernel driver.
 * @see AFE_IOR_CVD_GET_TIMING_INFO,AFE_IOR_CVD_GET_SVIDEO_TIMING_INFO
 */
typedef struct {
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	LX_AFE_VIDEO_SYSTEM_T	cvd_standard;	///< cvd video standard
	UINT16 u16_HFreq; 		///< Horizontal Frequncy (100Hz Unit)
	UINT16 u16_VFreq; 		///< Vertical Frequency (1/10Hz Unit) for CVD  VFREQ should be 500 or 599
	UINT16 u16_HTotal;		///< Horizontal Total pixel
	UINT16 u16_HStart;										///< Horizontal Capture Start
	UINT16 u16_VStart;										///< Vertical Capture Start
	UINT16 u16_HSize;										///< Horizontal Capture Range
	UINT16 u16_VSize;										///< Vertital Capture Range
	UINT16 u16_VFreq_Stable;		///< Vertical Frequency averaged value (1/10Hz Unit) for CVD  VFREQ should be 500 or 599
	UINT32	cvd_lock_stable_count;							///< Locked Stable Count
	UINT32	cvd_no_signal_count;							///< No Signal Count
} LX_AFE_CVD_TIMING_INFO_T;

/**
 * This structure is used to receive the timing information of 3CH ADC from kernel driver.
 * @see AFE_IOR_ADC_GET_TIMING_INFO
 */
typedef struct {
	UINT8 u8_Table_Idx;		///< Default Table Index
	UINT16 u16_HFreq; 		///< Horizontal Frequncy (100Hz Unit)
	UINT16 u16_VFreq; 		///< Vertical Frequency (1/10Hz Unit)
	UINT16 u16_HTotal;		///< Horizontal Total pixel
	UINT16 u16_VTotal; 		///< Vertica Total pixel
	UINT16 u16_HStart;		///< Horizontal Start pixel
	UINT16 u16_VStart;		///< Vertical Start lines
	UINT16 u16_HActive;		///< Horizontal active pixel
	UINT16 u16_VActive;		///< Vertical active lines
	UINT8 u8_ScanType;		///< Scan type (0:interlace, 1: progressive)
	UINT8 u16_Phase;			///< Sampling Phase
	UINT8	Sync_Exist;		///< ADC Sync detected???
	UINT8	Unstable;		///< IF 1, ADC driver is now searching resolution.
	UINT16 u16_Cur_HFreq;	///< Current Horizontal Frequency from Register Value (100Hz Unit)
	UINT16 u16_Cur_VFreq;	///< Current Vertical Frequency from Register Value (1/10Hz Unit)
	UINT16 u16_Cur_VTotal; 		///< Current Vertica Total pixel from Register Value
	UINT8	llpll_status;	///< Current Vertica Total pixel from Register Value
	UINT32	vs_width;		///< Width of vertical sync pulse (number of lines)
	UINT32	hs_width;		///< Width of horizontal sync pulse (number of dots)
	UINT16 u16_Prev_HFreq;	///< Prev Horizontal Frequency from Register Value (100Hz Unit)
	UINT16 u16_Prev_VFreq;	///< Prev Vertical Frequency from Register Value (1/10Hz Unit)
	UINT16 u16_Prev_VTotal; 		///< Prev Vertica Total pixel from Register Value
	UINT8	selmux;			///< 3ch ADC SELMUX value (00:ypbpr1, 01:ypbpr2, 10:rgb)
	LX_AFE_ADC_INPUT_SOURCE_TYPE_T adc_type;	///< ADC source type (0:RGB, 1:SCART_RGB, 2:YPbPr)
	UINT32	comp_sync_level;	/// < Component Sync Level Read by APA
	UINT32	comp_green_level;	/// < Component Green Level (vertical 10 bar 10th)

} LX_AFE_ADC_TIMING_INFO_T;
/**
 * This enum is used to select item to execute in PC auto adjust mode.
 * @see AFE_IOR_PC_AUTO_ADJUST
 */
typedef enum {
	LX_AFE_PC_Auto_HSize,			///< PC Auto HSize(Clock)
	LX_AFE_PC_AUTO_PHASE,			///< PC Auto Phase
	LX_AFE_PC_AUTO_POSITION,		///< PC Auto Position
	LX_AFE_PC_Auto_ALL,				///< Execute All(HSize, Phase, Position)
	LX_AFE_PC_AUTO_NUM,		///< PC Auto Adjust mode max number
} LX_AFE_AUTO_ADJ_PCMODE_T;
/**
 * This structure is used to receive PC AUTO ADJUST results from kernel driver.
 * @see AFE_IOR_PC_AUTO_ADJUST
 */
typedef struct {
	LX_AFE_AUTO_ADJ_PCMODE_T	adjust_mode;	///< Select Item  to Execute in PC Auto Adjust
	SINT16 s16_PC_HDelay;	///< Horizontal Sync Delay value
	SINT16 s16_PC_VDelay;	///< Vertical Sync Delay value
	UINT16 u16_PC_Clock;		///< Clock
	UINT16 u16_PC_Phase;		///< Phase
	SINT16		result;			///< auto adjust result 0:OK, -1:NG
} LX_AFE_ADJ_PC_T;

/**
 * This enum is used to select PC manual adjust mode.
 * @see AFE_IOR_PC_AUTO_ADJUST
 */
typedef enum {
	LX_AFE_PC_HPOS,			///< PC Horizontal position
	LX_AFE_PC_VPOS,			///< PC Vertical position
	LX_AFE_PC_CLOCK,		///< PC clock
	LX_AFE_PC_HTOTAL,		///< PC Horizontal Total
	LX_AFE_PC_PHASE,		///< PC phase (value: 0 ~ 63)
	LX_AFE_PCMODE_NUM,		///< PC mode max number
} LX_AFE_ADJ_PCMODE_T;

/**
 * This structure is used to transfer PC manual adjust mode and it's value to kernel driver.
 * @see AFE_IOR_PC_AUTO_ADJUST
 */
typedef struct {
	LX_AFE_ADJ_PCMODE_T	mode;	///< item to adjust
	SINT16					value;		///< value
} LX_AFE_ADJ_PCMODE_INFO_T;

/**
 * This structure is used to transfer parameters for ADC AUTO Calibration to kernel driver.
 * @see AFE_IOW_PC_ADJUST
 */
typedef struct {
	BOOLEAN bInternal;		///< TRUE:internal, FALSE:external
	BOOLEAN bRGB;			///< TRUE:RGB, FALSE: YPbPr
	UINT16	TargetForRGain;	///< Target Red Value
	UINT16	TargetForGGain; ///< Target Green Value
	UINT16	TargetForBGain; ///< Target Blue Value
	BOOLEAN	bResult;		///< Auto Calibration Result
}	LX_AFE_ADC_CALIB_INFO_T;

/**
 * This structure is used to send/receive R/G/B Gain Value to/from kernel driver.
 * @see AFE_IOW_ADC_SET_GAIN, AFE_IOW_ADC_SET_OFFSET, AFE_IOW_ADC_GET_GAIN, AFE_IOW_ADC_GET_OFFSET
 */
typedef struct {
	UINT16	R_Gain_Value;	///< Red Gain Value
	UINT16	G_Gain_Value;	///< Green Gain Value
	UINT16	B_Gain_Value;	///< Blue Gain Value
}	LX_AFE_ADC_GAIN_VALUE_T;

/**
 * This structure is used to send/receive R/G/B Offset Value to/from kernel driver.
 * @see AFE_IOW_ADC_SET_GAIN, AFE_IOW_ADC_SET_OFFSET, AFE_IOW_ADC_GET_GAIN, AFE_IOW_ADC_GET_OFFSET
 */
typedef struct {
	UINT16 R_Offset_Value;	///< Red offset value
	UINT16 G_Offset_Value;	///< Green offset value
	UINT16 B_Offset_Value;	///< Blue offset value
} 	LX_AFE_ADC_OFFSET_VALUE_T;

/**
 * This enum is used to select Scart input mode.
 * @see AFE_IOR_CVD_GET_FB_STATUS
 */
typedef enum {
	LX_SCART_MODE_RGB,		///< Scart RGB mode
	LX_SCART_MODE_CVBS,		///< Scart CVBS mode
	LX_SCART_MODE_SVIDEO,	///< Scart SVIDEO mode
} LX_AFE_SCART_MODE_T;

/**
 * This enum is used to select Scart Aspect Ratio information.
 * @see AFE_IOR_GET_SCART_AR,LX_AFE_SCART_AR_INFO_T
 */
typedef enum {
	LX_SCART_AR_4_3,		///< Scart 4:3 Aspect Ratio
	LX_SCART_AR_16_9,		///< Scart 16:9 Aspect Raio
	LX_SCART_AR_INVALID,	///< Scart Aspect Raio invalid
} LX_AFE_SCART_AR_T;

/**
 * This enum is used to select one Scart between two Scart inputs.
 * @see LX_AFE_SCART_AR_INFO_T
 */
typedef enum {
	LX_SCART_ID_1,		///< Scart ID 1
	LX_SCART_ID_2,		///< Scart ID 2
	LX_SCART_ID_NUM,	///< scart ID max num
} LX_AFE_SCART_ID_T;

/**
 * This structure is used to receive Aspect Ration Value of given Scart_ID from kernel driver.
 * @see AFE_IORW_GET_SCART_AR
 */
typedef struct {
	LX_AFE_SCART_ID_T	Scart_Id;	///< Scart ID number
	LX_AFE_SCART_AR_T	Scart_AR;	///< Scart aspect ratio information
} LX_AFE_SCART_AR_INFO_T;

/**
 * This structure is used to Send 3D Comb filter Values to kernel driver.
 * @see AFE_IOW_SET_3DCOMB_VALUE
 */
typedef struct {
	UINT8	u8_index;						///<comb filter Index
	UINT32	u32_comb_fiter_register_00h;	///< TBD
	UINT32	u32_comb_fiter_register_01h;	///< TBD
	UINT32	u32_comb_fiter_register_02h;	///< TBD
} LX_AFE_3DCOMB_VALUE_T;

/**
 * This enum is used to set CVD PQ Value to selected Color System
 * @see AFE_IOW_SET_3DCOMB_VALUE
 */

typedef enum {
	LX_CVD_PQ_DEFAULT 	= 0,		///< Default CVD PQ Params
	LX_CVD_PQ_NTSC_M 	= 1,		///< CVD PQ Params for NTSC_M AV inputs
	LX_CVD_PQ_NTSC_M_RF = 2,		///< CVD PQ Params for NTSC_M RF input
	LX_CVD_PQ_PAL 		= 3,		///< CVD PQ Params for NTSC_M AV inputs
	LX_CVD_PQ_PAL_RF	 = 4,		///< CVD PQ Params for NTSC_M RF input
	LX_CVD_PQ_SECAM 	= 5,		///< CVD PQ Params for NTSC_M AV inputs
	LX_CVD_PQ_SECAM_RF 	= 6,		///< CVD PQ Params for NTSC_M RF input
	LX_CVD_PQ_NTSC_443 	= 7,		///< CVD PQ Params for NTSC_443 AV inputs
	LX_CVD_PQ_NTSC_443_RF = 8,		///< CVD PQ Params for NTSC_443 RF input
	LX_CVD_PQ_PAL_60 	= 9,		///< CVD PQ Params for PAL_60 AV inputs
	LX_CVD_PQ_PAL_60_RF = 10,		///< CVD PQ Params for PAL_60 RF input
	LX_CVD_PQ_PAL_M 	= 11,		///< CVD PQ Params for PAL_M AV inputs
	LX_CVD_PQ_PAL_M_RF 	= 12,		///< CVD PQ Params for PAL_M RF input
	LX_CVD_PQ_PAL_NC 	= 13,		///< CVD PQ Params for PAL_NC AV inputs
	LX_CVD_PQ_PAL_NC_RF = 14,		///< CVD PQ Params for PAL_NC RF input

	LX_CVD_PQ_AAF_A = 7,	///< CVD AAF A setting	for AAF filter test
	LX_CVD_PQ_AAF_6_0 = 8,	///< CVD AAF 6.0M for AAF filter test
	LX_CVD_PQ_AAF_5_5 = 9,	///< CVD AAF 5.5M for AAF filter test
	LX_CVD_PQ_AAF_4_5 = 10,	///< CVD AAF 4.5M for AAF filter test
	LX_CVD_PQ_NUM 		= 15,
} LX_AFE_CVD_PQ_MODE_T;

/**
 * This structure is used to Read Current PC Mode Values from kernel driver.
 * @see AFE_IOR_GET_PCMODE_INFO
 */
typedef struct {
	SINT16 s16_PC_HPos;		///< PC horizontal Position
	SINT16 s16_PC_VPos;		///< PC Verical Position
	UINT16	u16_Phase;		///< PC Phase
	UINT8	u8_UI_Phase;	///< Converted UI Phase value(6bit)
	UINT8	u8_Mode_Idx;	///< Current Mode index
	SINT16	s16_PC_HTotal;	///< PC Horizontal Total
	UINT16	u16_HSyncWidth;	///< Width of Horizontal Sync Pulse
	UINT16	u16_VSyncWidth;	///< Width of Vertical Sync Pulse
} LX_AFE_PCMODE_INFO_T;

/**
 * This structure is used to Define Default PC/Component Mode Table. This table is sent to kernel driver.
 * @see LX_AFE_SET_PCMODE_TABLE_T
 */
typedef struct {
	UINT8 u8_Res_Idx;		///< Resolution Index
	UINT16 u16_HFreq;		///< Horizontal Frequency
	UINT16 u16_VFreq;		///< Vertical Frequency

	UINT16 u16_HStart;		///< Horizontal Start
	UINT16 u16_VStart;		///< Vertical Start

	UINT16 u16_HTotal;		///< Horizontal Total pixel
	UINT16 u16_VTotal;		///< Vertical Total Pixel
	UINT8 u8_VTotal_Tolerance;	///< Tolerance of Vertical Total
	//UINT8 u8_ADC_Phase;		///< ADC Phase
	UINT16 u16_ADC_Phase;		///< ADC Phase
	UINT8	u8_VSync_Shift;
	UINT8	u8_VSync_Width;
	UINT8	u8_HSync_Width;
	UINT8 u8_Status_Flag;	///< b0 : Vsync polarity(1/0=positive/negative), b1 : Hsync polarity, b2 : Sync Polarity care bit, b4 : interlace mode, b5 : euro HDTV Bit, b6 : YPbPr Bit, b7 : HDTV Bit
	//UINT16 u16_Width;
	//UINT16 u16_Heith;
} LX_AFE_PCMODE_MODETABLE_T;
/**
 * This structure is used to send Default PC/Component Mode table to kernel driver.
 * @see AFE_IOW_SET_PCMODE_TABLE
 */
typedef struct {
	UINT32	*pPCMode_Table;	///< Address pointer of PCMode Table
	UINT32	Table_Size;		///< Size of PCMode Table
	UINT8	Table_Count;	///< Table Count
}	LX_AFE_SET_PCMODE_TABLE_T;
/**
 * This structure defines PC/Component Resolution table.
 * @see LX_AFE_SET_PCMODE_TABLE_T
 */
typedef enum
{
    LX_RES_640X350,    // 00
    LX_RES_640X400,    // 01
    LX_RES_720X400,    // 02
    LX_RES_640X480,    // 03
    LX_RES_800X600,    // 04
    LX_RES_832X624,    // 05
    LX_RES_1024X768,   // 06
    LX_RES_1280X1024,  // 07
    LX_RES_1600X1200,  // 08
    LX_RES_1152X864,   // 09
    LX_RES_1152X870,   // 10
    LX_RES_1280X768,   // 11
    LX_RES_1280X960,   // 12
    LX_RES_720X480,    // 13
    LX_RES_1920X1080,  // 14

    LX_RES_1280X720,   // 15
    LX_RES_720X576,    // 16

    LX_RES_1920X1200,  // 17

    LX_RES_1400X1050,  // 18
    LX_RES_1440X900,   // 19
    LX_RES_1680X1050,  // 20

    LX_RES_1280X800,   // 21
    LX_RES_1600X1024,  // 22
    LX_RES_1600X900,   // 23
    LX_RES_1360X768,   // 24
    LX_RES_848X480,    // 25
    LX_RES_1920X1080P, // 26

    LX_RES_1366X768,   // 27
    LX_RES_864X648,    // 28
    LX_RES_1152X900,    // 29
    LX_RES_1152X720,    // 30
    LX_RES_1728X1080,    // 31

    LX_RES_MAXIMUM
} LX_AFE_RESOLUTION_TYPE_T;

typedef struct
{
	BOOLEAN ycrdc_lcr_onoff;
	BOOLEAN cyrdc_lcr_onoff;
} LX_AFE_PICTURE_ENHANCEMENT_ONOFF_T;

typedef struct
{
   UINT32	addr;
   UINT32	data;
} LX_AFE_REG_RW_T;

typedef struct
{
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	UINT32 timeout;
}	LX_AFE_CVD_TIMER_T;

typedef struct
{
	LX_AFE_CVD_SELECT_T			cvd_main_sub;	///< main/sub cvd select
	LX_AFE_VIDEO_SYSTEM_T 		color_system;
}	LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T;

/**
 * This enum is used to select VBI decoder type(TTX/VPS/WSS or ACC)
 * @see AFE_IOW_CVD_SET_VBI_TYPE
 */
typedef enum {
	LX_VBI_MODE_TTX,		///< TTX/VPS/WSS mode
	LX_VBI_MODE_ACC,		///< ACC mode
} LX_AFE_CVD_VBI_MODE_T;

/*
   * This struct is used to set values for SW 3DCOMB control
   * @see AFE_IOW_CVD_SET_SW_3DCOMB_CONTROL
   */

typedef struct
{
	UINT8	Enable;	///< enable/disable sw 3dcomb control
	UINT32	Hf_thr;	///< threshold value of high frequency
	UINT32	Lf_thr;	///< threshold value of low frequency
	UINT32 	Diff_thr;	///< threshold value of diffence of two value
	UINT32	Region_30_mode;
}	LX_AFE_CVD_SW_3DCOMB_CONTROL_T;

/*
   * This struct is used to set window size for pixel sum,
   and to read R/G/B values after summation.
   * @see AFE_IOR_ADC_READ_PIXEL_VALUE
   */

typedef struct
{
	UINT32	X_Pos;			///<	X_pos for pixel sum
	UINT32	Y_Pos;			///<	Y_pos for pixel sum
	UINT32	X_Size;			///<	X_size for pixel sum
	UINT32	Y_Size;			///<	Y_size for pixel sum
	UINT32	Sum_R_Value;	///<	Red value of pixel sum
	UINT32	Sum_G_Value;	///<	Green value of pixel sum
	UINT32	Sum_B_Value;	///<	Blue value of pixel sum
}	LX_AFE_ADC_PIXEL_VALUE_T;

/*
   * This struct is used to read detailed CVD states,
   * @see AFE_IOR_CVD_GET_STATES_DETAIL
   */

typedef struct
{
	UINT8	No_Signal_Flag;			///<	no signal flag
	UINT8	HLock_Flag;			///<	horizontal lock flag
	UINT8	VLock_Flag;			///<	vertical lock flag
	UINT8	VLine_625_Flag;			///<	vline 625 flag
	UINT8	Pal_Flag;	///<	PAL flag
	UINT8	Secam_Flag;	///<	SECAM flag
	UINT8	Chromalock_Flag;	///<	Chromalock Flag
	UINT8	NoBurst_Flag;	///<	No Burst Flag
	UINT8	FC_Flag;	///<	FC_Flag (0: Less, 1: same , 2:more)
	UINT8	cvd_cordic_freq; 	///< cordic frequency
	UINT16	status_noise;	///<	status_noise
	UINT8	md_mode_value;		///<	md_mode_value;
	UINT8	motion_mode_value;		///<	md_mode_value;
	UINT8	MainCVD_State;			///< Main CVD State(Color System);
	UINT8	Current_State;			///< Current CVD State;
	UINT8	Next_State;			///< Next CVD State;
	UINT8	Pal_Flag_CS0;	///<	PAL flag
	UINT8	Secam_Flag_CS0;	///<	SECAM flag
	UINT8	Chromalock_Flag_CS0;	///<	Chromalock Flag
	UINT8	FC_Flag_CS0;	///<	FC_Flag (0: Less, 1: same , 2:more)
	UINT8	cvd_cordic_freq_CS0; 	///< cordic frequency
	UINT8	Pal_Flag_CS1;	///<	PAL flag
	UINT8	Secam_Flag_CS1;	///<	SECAM flag
	UINT8	Chromalock_Flag_CS1;	///<	Chromalock Flag
	UINT8	FC_Flag_CS1;	///<	FC_Flag (0: Less, 1: same , 2:more)
	UINT8	cvd_cordic_freq_CS1; 	///< cordic frequency

	UINT8	CVD_Color_System;		///< Current CVD Standard
	UINT8	CS0_Color_System;		///< Current CS0 Standard
	UINT8	CS1_Color_System;		///< Current CS1 Standard

	UINT32	color_system_support;		///< Color system  supported by CVD

	//120131 added for debug
	UINT32	tnr_x;					/// < Global Motion
	UINT32	tnr_s;					/// < Spatial Varriance
	UINT32	s_status;				/// < Test Pattern
	int	motion_diff;				/// < Motion Difference
	UINT32	motion_value;			/// < Motion Value
	UINT32	pattern_found;			/// < ColorBar Test Pattern found???
	UINT32	static_pattern_found;	/// < Static Test Pattern found???

	UINT32	agc_gain;				/// < CVD AGC Gain
	UINT32	cagc;					/// < CVD CAGC Value
	UINT32	saturation;				/// < CVD Saturation Value
	UINT8 hnon_standard;			/// < hnon_standard flag
	UINT8 vnon_standard;			/// < vnon_standard flag
	UINT32	burst_mag;				/// < burst_magnitude
	UINT32 vdetect_vcount;			/// < vdetect vcount value
	UINT8	agc_peak_en;			/// < agc peak en
	UINT8	agc_bypass;				/// < agc_bypass
	UINT8	tunning_mode;			/// < CVD in manual/auto tunning mode
	UINT8	lock_stable_count;			/// < CVD color standard stable count
	UINT8	no_signal_count;			/// < CVD no signal count
	UINT8	black_level;		/// < Current CVD Black Level
	UINT32	colorbar_diff;			/// < Lumnance Difference Sum with colorbar pattern
	UINT8	colorbar_75_100;		/// < Detected ColorBar is 75% or 100% colorbar
	UINT8	cvbs_icon;				/// < CVBS Main Clamp Current
	UINT8	cvbs_pdb;				/// < Power Down for CVBS Main
	UINT8	cvbs_cp;				/// < Charge Pump On/Off for CVBS Main
	UINT8	cvbs_insel;				/// < CVBS Main Input Selection
	UINT8	buf_ycm;				/// < Bypass Input Selection
	UINT8	buf_sel1;				/// < Buffer input selection for BUF1
	UINT8	buf_pdb1;				/// < Power Down for BUF1
	UINT8	vdac_pdb;				/// < Power Down for DAC
	UINT8	no_color_detected;		/// < CVD no color detection flag
	UINT8	analog_demod_type;		/// < Analog Demod Type (1:Internal, 0:external)
	UINT32	status_clamp_updn;		/// < Clamp Up/Down Pulse Status	
}	LX_AFE_CVD_STATES_DETAIL_T;

/**
 * This enum is used to select item to test
 * @see LX_AFE_ADC_TEST_PARAM_T
 */
typedef enum
{
	ADC_TEST_PHASE_READ,
	ADC_TEST_PHASE_WRITE,
	ADC_TEST_APA,
	ADC_TEST_SOGLVL_READ,
	ADC_TEST_SOGLVL_WRITE,
	ADC_TEST_HTOTAL_DIFF_ENABLE,
	ADC_TEST_SYNC_LOW_LEVEL_READ,
}	LX_AFE_ADC_TEST_ITEM_T;

/**
 * This struct is used for ADC Kernel Driver Test
 * @see AFE_IOW_ADC_TEST
 */
typedef struct
{
	LX_AFE_ADC_TEST_ITEM_T	item_to_test;		///< Select Item to Test

	UINT32	ADC_Phase_Value;		///< Phase Value for Read/Write

	UINT32	ADC_APA_Delay;			///< delay before APA execution
	UINT32	ADC_APA_Enable;			///< enable/disable APA
	UINT32	ADC_APA_Same_Count;			///<  Count value for ATA same result
	UINT32	ADC_APA_Execute;			///<  Execute APA
	UINT32	ADC_APA_Enable_Print;		///< Enable Printing APA Result
	UINT32	ADC_Soglvl_Value;		///< SOG Level Value for Read/Write
	UINT32	ADC_HTotal_Diff_Enable;		///< ADC HTotal Diff Workaround Enable
	UINT32	ADC_Sync_Low_Level_Read_Enable;		///< ADC Sync Low Level Read Workaround Enable
}	LX_AFE_ADC_TEST_PARAM_T;

/**
 * This enum is used to select item to test
 * @see LX_AFE_CVD_TEST_PARAM_T
 */
typedef enum
{
	CVD_TEST_PICTURE_ENHANCEMENT,
	CVD_TEST_ENABLE_STATUS_FLD_PRINT,
	CVD_TEST_ENABLE_3DCOMB_STATE_PRINT,
//	CVD_TEST_HSTATEMAX_READ,
	CVD_TEST_HSTATEMAX_WRITE,
	CVD_TEST_AGC_PEAK_NOMINAL,
	CVD_TEST_STABLE_SYNC_MODE,
	CVD_TEST_COMB2D_ONLY,
	CVD_TEST_BLUE_MODE,
	CVD_TEST_ADEMOD_TYPE,
	CVD_TEST_DMD_OUT,
	CVD_TEST_SCART_RGB,
	CVD_TEST_HSYNC_ENHANCE,
	CVD_TEST_VSYNC_ENHANCE,
	CVD_TEST_YCSEP,
	CVD_TEST_CLAMP_UPDN,
	CVD_TEST_DC_CLAMP_MODE,
	CVD_TEST_ABB_CLAMP_PARAM,
}	LX_AFE_CVD_TEST_ITEM_T;

typedef enum
{
	CVD_PE_AAF,
	CVD_PE_ADC,
	CVD_PE_CRESAMPLER,
	CVD_PE_DOT_CRAWL,
	CVD_PE_CROSS_COLOR_RECUDER,
	CVD_PE_MOTION_DETECTOR,
	CVD_PE_GLOBAL_CLASSIFIER,
	CVD_PE_OUTPUT_DELAY,
	CVD_PE_OUTPUT_RANGE,
	CVD_PE_BLENDING,
	CVD_PE_CHROMA_DEMODE,
	CVD_PE_NUM,

}	LX_AFE_CVD_PE_PARAM_T;

/**
 * This enum is used to debug test of scart out
 * @see LX_AFE_CVD_TEST_PARAM_T
 */
typedef enum
{
	CVD_SCART_OUT_DTV = 0,						///< Select DTV SCART out for test 
	CVD_SCART_OUT_ATV = 1,		///<  Select ATV Scart out for test
	CVD_SCART_OUT_ATV_INTERNAL = 2,	///< Select ATV Scart out from internal a-demod for test
	CVD_SCART_OUT_ATV_EXTERNAL = 3,	///< Select ATV Scart out from external a-demod for test
}	LX_AFE_CVD_SCART_OUT_MODE_T;

/**
 * This struct is used to test CVD Kernel Driver.
 * @see AFE_IOW_CVD_TEST
 */
typedef struct
{
	LX_AFE_CVD_TEST_ITEM_T	item_to_test;		///< Select Item to Test

	LX_AFE_CVD_PE_PARAM_T	cvd_pe_param;
	LX_AFE_CVD_PQ_MODE_T	cvd_pe_mode;
	BOOLEAN					bEnable_Status_Fld_Print;
	BOOLEAN					bEnable_3DCOMB_State_Print;
	UINT32					hstate_max_value;
	BOOLEAN					agc_peak_enable;
	int						agc_peak_white_ratio_th;
	int						agc_peak_x_avg_t_th;
	int						agc_peak_x_avg_s_th;
	int						agc_peak_s_staus_th;
	int						agc_peak_white_frame_max_th;
	int						agc_peak_white_frame_on;
	int						agc_peak_white_frame_off;
	int						agc_peak_noise_th;
	int						agc_peak_print_en;
	int						stable_sync_value;
	int						comb2d_only_test_enable;
	int						comb2d_only_on_time;
	int						comb2d_only_off_time;
	int						comb2d_only_md_on_time;
	int						comb2d_only_md_off_time;
	int						blue_mode_enable;
	int						internal_demod_type;
	int						dmd_out_enable;
	int						fb_en;
	int						blend_ratio;
	int						vsync_enhance_value;
	int						hsync_enhance_value;
	int						ycsep_blend_value;
	int						clamp_updn_value;
	int						dc_clamp_mode_value;
	int						abb_clamp_count;
	int						abb_clamp_step;
}	LX_AFE_CVD_TEST_PARAM_T;

/**
 * This enum is used to select CVBS Output Source
 * @see AFE_IOW_CVD_BYPASS_CONTROL
 */
typedef enum
{
	CVD_BYPASS_DAC	= 0,						///< Select DAC output (DTV out) for bypass
	CVD_BYPASS_CVBS_WITH_CLAMPING = 1,		///< Select CVBS(selected by buf_ycm) for bypass control, and enable clamping to input cvbs.
	CVD_BYPASS_CVBS_WITH_CLAMPING_AV = 2,	///< Select CVBS(selected by buf_ycm) for bypass control, and enable clamping to input cvbs & Enable CVBS Clamping Control(in av input mode)
	CVD_BYPASS_CVBS_WITHOUT_CLAMPING = 3,	///< Select CVBS(selected by buf_ycm) for bypass control, and disable clamping to input cvbs.
	CVD_BYPASS_CVBS_BUF_CLAMPING_ON = 4,	///< Select CVBS(select IN1) for bypass control, and enable clamping to input cvbs. (do not control cvd clamp)
	CVD_BYPASS_CVBS_BUF_CLAMPING_OFF = 5,	///< Select CVBS(select IN1) for bypass control, and disable clamping to input cvbs.(do not control cvd clamp)
	CVD_BYPASS_ABB = 6,						///< Select ABB(Internal Demod) for bypass control
	CVD_BYPASS_ABB_RC = 7,						///< Select ABB(Internal Demod) after rate convertor for bypass control (for test)
	CVD_BYPASS_MAX,
}	LX_AFE_CVD_BYPASS_SOURCE_T;

/**
 * This enum is used to select CVBS Input Source for Bypass
 * @see AFE_IOW_CVD_BYPASS_CONTROL
 */
typedef enum
{
	CVD_BYPASS_CVBS_IN1,				///< Select cvbs_in1 for cvbs bypass source.
	CVD_BYPASS_CVBS_IN3,				///< Select cvbs_in3 for cvbs bypass source.
}	LX_AFE_CVD_BYPASS_CVBS_SOURCE_T;

/**
 * This struct is used to select CVBS source for BUF_OUT
 * @see AFE_IOW_CVD_BYPASS_CONTROL
 */

typedef struct
{
	LX_AFE_CVD_BYPASS_SOURCE_T			buf_out_1_sel;			///< Select bypass source of buf out 1
	LX_AFE_CVD_BYPASS_SOURCE_T			buf_out_2_sel;			///< Select bypass source of buf out 2
	LX_AFE_CVD_BYPASS_CVBS_SOURCE_T		cvbs_source_sel;			///< Select cvbs source for bypass
}	LX_AFE_CVD_BYPASS_CONTROL_T;

/**
 * Select between LVDS 0 / 1
 * @see LX_AFE_LVDS_CONTROL_T
 */
typedef enum
{
	LX_AFE_LVDS0_SEL	= 0,		///< select hs-lvds 0
	LX_AFE_LVDS1_SEL	= 1,		///< select hs-lvds 1
}	LX_AFE_LVDS_SELECT_T;

/**
 * Power down control & PLL Reset
 * @see LX_AFE_LVDS_CONTROL_T
 */
typedef enum
{
	LX_AFE_LVDS_POWER_DOWN	= 0,		///< power down selected hslvds
	LX_AFE_LVDS_POWER_ON	= 1,		///< power on selected hslvds
	LX_AFE_LVDS_POWER_NO_CHANGE	= 0xff,		///< no change
}	LX_AFE_LVDS_PDB_T;

/**
 * Selects data type of selected HS-LVDS
 * @see LX_AFE_LVDS_CONTROL_T
 */
typedef enum
{
	LX_AFE_LVDS_TYPE_VESA	= 0,		///< select VESA for hs-lvds data type
	LX_AFE_LVDS_TYPE_JEIDA	= 1,		///< select JEIDA for hs-lvds data type
	LX_AFE_LVDS_TYPE_NO_CHANGE	= 0xff,		///< no change
}	LX_AFE_LVDS_DATA_TYPE_T;

/**
 * Selects data type of selected HS-LVDS
 * @see LX_AFE_LVDS_CONTROL_T
 */
typedef enum
{
	LX_AFE_LVDS_SOURCE_HDMI_NORMAL	= 0x0,	///< select HDMI normal for hs-lvds source
	LX_AFE_LVDS_SOURCE_3CH			= 0x1,	///< select 3CH ADC for hs-lvds source
	LX_AFE_LVDS_SOURCE_CVD			= 0x2,	///< select CVD for hs-lvds source
	LX_AFE_LVDS_SOURCE_HDMI_LEFT	= 0x4,	///< select HDMI left channel for hs-lvds source
	LX_AFE_LVDS_SOURCE_HDMI_RIGHT	= 0x5,	///< select HDMI right channel for hs-lvds source
	LX_AFE_LVDS_SOURCE_NO_CHANGE	= 0xff,		///< no change
}	LX_AFE_LVDS_SOURCE_T;

/**
 * Selects between normal mode and scart mix mode
 * @see LX_AFE_LVDS_CONTROL_T
 */
typedef enum
{
	LX_AFE_LVDS_MODE_NORMAL	= 0x0,	///< select lvds normal mode
	LX_AFE_LVDS_MODE_SCART_MIX			= 0x1,	///< select scart mix mode
	LX_AFE_LVDS_MODE_NO_CHANGE	= 0xff,		///< no change
}	LX_AFE_LVDS_MODE_T;
/**
 * Structure used to control hs-lvds setting values
 * @see AFE_IOW_LVDS_CONTROL_T
 */
typedef struct
{
	LX_AFE_LVDS_SELECT_T			lvds_sel;			///< Select hs-lvds 0 or 1
	LX_AFE_LVDS_PDB_T				lvds_power;			///< power down control
	LX_AFE_LVDS_DATA_TYPE_T			lvds_type;			///< Select data type of lvds
	LX_AFE_LVDS_SOURCE_T			lvds_source;		///< Select input source of lvds
	LX_AFE_LVDS_MODE_T				lvds_mode;			///< select lvds scart mix mode
}	LX_AFE_LVDS_SRC_CONTROL_T;

/*
   * This struct is used to read detailed CVD memory configuration,
   * @see AFE_IOR_GET_MEM_CFG
   */

typedef struct
{
	UINT32	mem_base_comb_buffer;				/// < memory base address of comb filter buffer
	UINT32 mem_size_comb_buffer;			/// < memory size of comb filter buffer
	UINT32	mem_base_cvd_reg;			/// < memory base address of CVD register
	UINT32	mem_size_cvd_reg;				/// < memory size of CVD register
}	LX_AFE_CVD_MEM_CFG_T;

/**
 * Structure used to read CVD input Crunky detection status
 * @see AFE_IOR_GET_CK_DETCTION
 */
typedef struct
{
	UINT32	ck_vbi_detected;					/// < Crunky PSP detection
	UINT32	ck_colorstrip_detected;				/// < number of Crunky CS detected
}	LX_AFE_CVD_CK_T;


 /**
  * Enable or Disable AFE Debug Print.
  * afe ioctl parameter.
  * @see AFE_IOW_DEBUG_PRINT_CTRL
 */

 typedef struct LX_AFE_DEBUG_CTRL{
	UINT32				printType;				///< The type of print module
	UINT32				printColor;				///< The color of print
	UINT32				printEnable;			///< Enable/Disable Debug Print
} LX_AFE_DEBUG_CTRL_T;

/**
 * Send UI Black Level Value to CVD Kernel Driver
 * @see AFE_IOW_SET_CVD_BLACK_LEVEL
 */
typedef enum
{
	LX_AFE_CVD_BLACK_LEVEL_LOW			= 0x0,	///< CVD Black Level Low (for 7.5 IRE Signal)
	LX_AFE_CVD_BLACK_LEVEL_HIGH			= 0x1,	///< CVD Black Level High (for 0 IRE Signal)
	LX_AFE_CVD_BLACK_LEVEL_AUTO			= 0x2,	///< CVD Black Level Auto
}	LX_AFE_CVD_BLACK_LEVEL_T;

 /**
  * Structure for Component Pseudo Pulse Detection.
  * afe ioctl parameter.
  * @see AFE_IOR_DETECT_COMPONENT_PSEUDO_PULSE
 */

typedef struct LX_AFE_ADC_COMP_PSEUDO_PULSE {
	UINT32				psp_detected;				///< pseudo pulse detected
	UINT32				vline_normal;				///< vline count on normal signal
	UINT32				vline_measured;				///< vline count measured
	UINT32				valid_signal;			///< return '1' when component SD video source
} LX_AFE_ADC_COMP_PSEUDO_PULSE_T;

/**
 * This enum is for H13 ACE A0 SCART RGB Mix Bug
 * @see AFE_IOW_SET_SCART_BYPASS_MODE
 */
typedef enum {
	LX_SCART_RGB_MODE_NORMAL,			///< Normal SCART RGB FB Mode
	LX_SCART_RGB_MODE_BYPASS,			///< Scart RGB Through Component Path
	LX_SCART_RGB_MODE_FORCED_BYPASS,	///< Force RGB Mode Through Component Path (Don't use FB status)
} LX_AFE_SCART_BYPASS_MODE_T;

 /**
  * Structure for Component VBI Copy Protection
  * @see AFE_IOR_GET_COMPONENT_VBI_CP
 */

typedef struct LX_AFE_ADC_COMP_VBI_CP {
	UINT32				cgms_cp_data;				///< component cgms copy protection data [3:0] = [CGMS_B0:CGMS_B1:APS_B2:APS_B3]
	UINT32				wss_cp_data;				///< component wss copy protection data [1:0] = [CGMS_B12:CGMS_B13]
	UINT32				cgms_data0;						///< vbi cgms even data
	UINT32				cgms_data1;						///< vbi cgms odd data
	UINT32				wss_data0;						///< vbi wss data 0
	UINT32				wss_data1;						///< vbi wss data 1
} LX_AFE_ADC_COMP_VBI_CP_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AFE_DRV_H_ */

/** @} */
