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



/** @file hdmi_kapi.h
 *
 *  application interface header for hdmi device
 *
 *  @author		sh.myoung (sh.myoung@lge.com)
 *  @version		0.9
 *  @date		2010.02.21
 *
 *  @addtogroup lg1150_hdmi
 *	@{
 */

#ifndef	_HDMI_KAPI_H_
#define	_HDMI_KAPI_H_

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
#define	HDMI_IOC_MAGIC		'a'
/**
@name HDMI IOCTL List
ioctl list for hdmi device.
@{

@def HDMI_IO_INIT
Initialize HDMI module.
Set init Rx HDCP production key.

@def HDMI_IOR_GET_MODE
Get HDMI/DVI mode Information.
Checks whether HDMI format or DVI mode in HDMI port

@def HDMI_IOR_GET_ASPECTRATIO
Get the aspect ratio of the HDMI.
This information is generated from the 2 bits aspect ratio information(M1,M0)
of the HDMI AVI(Auxiliary Video Information) InfoFrame packet.

@def HDMI_IOR_GET_TIMING_INFO
Get the Timing Info of HDMI.

@def HDMI_IOR_GET_HDMI_STATUS
Get the information of HDMI.
Those are HDMI mode, HDCP, colordonmain, colorimetry and hotplug information.

@def HDMI_IOR_GET_HDMI_VSI
Get HDMI VSI(Vendor Specific Information) inforFrame data.

@def HDMI_IOR_GET_AVI_INFO
Get HDMI AVI(Auxiliary Video Information) inforFrame data.

@def HDMI_IOW_SET_HPD
Control HDMI HOT Plug Detect.
Enable hotplug GPIO/PIN output and set output value

@def HDMI_IOW_SET_PORT
Set the Input port of HDMI.

@def HDMI_IOR_GET_HDMI_SPD
Get HDMI SPD(Source Product Description) InfoFrame data.

@def HDMI_IOW_SET_POWER
This ioctl control power on/off of HDMI block to reduce power consumption.

@def HDMI_IOW_SET_3D_N_UD
Set HDMI Data Control for support UD and 3D(SS Full, Frame Packing 1080p@60).

@def HDMI_IOR_PHY_REG_RD
Read HDMI PHY Register Value.

@def HDMI_IOW_PHY_REG_WR
Write HDMI PHY Register Value.

@def HDMI_IOW_HDCP_ENABLE
Set HDCP Enable : 1, disable : 0.

@def HDMI_IOW_SET_HDCP
Set HDCP key info to HDMI Link Register

@def HDMI_IOR_GET_AUDIO_INFO
Gets a HDMI audio info related to AUDIO module.
This info. has a No audio, PCM, AC3 or DTS info. to DDI function from HDMI port input.

@def HDMI_IOR_GET_COPY_INFO
Gets a HDMI audio copyright info. to AUDIO module.
This info. has a copy free, nore more and once info. to DDI function from HDMI port input.

@def HDMI_IOW_SET_ARC
Set HDMI ARC(Audio Return Channel) Control.

@def HDMI_IOW_SET_MUTE
Set HDMI Video and Audio Mute.

@def HDMI_IOR_DEBUG_GET_AUDIO_INFO
Gets a HDMI audio debug info. related to AUDIO module.
This info. has a No audio, PCM, AC3 or DTS info. to DDI function from HDMI port input. \n
And, other information about audio related.

@def HDMI_IOW_WRITE_EDID_DATA
Writes EDID Data to the edid register in HDMI Link Register

@def HDMI_IOR_READ_EDID_DATA
Read EDID Data from the edid register in HDMI Link Register

@def HDMI_IOR_GET_PHY_STATUS
Read HDMI phy status

@def HDMI_IOW_GET_HDCP
Get HDCP key data from Link Register

@def HDMI_IOR_GET_AKSV_DATA
Read HDMI HDCP information

@def HDMI_IOW_SEND_RCP
Send Remote Control Packet to source.

@def HDMI_IOW_SEND_WRITEBURST
Send Burst Data to Source

@def HDMI_IOR_READ_WRITEBURST
Read Burst Data from source

@def HDMI_IOW_CALL_TYPE
Inform kernel driver HAL function call

@def HDMI_IOR_GET_DRIVER_STATUS
Get HDMI internal status (for debugging)

@def HDMI_IOW_CONTROL_KEY
Restrore and rewrite hdmi key data

@def HDMI_IOW_DEBUG_HDMI_DRV
Set HDMI driver debugger functions (for debugging)

@def HDMI_IOW_HPD_HDCP22_CONTROL
Set HDMI HDP Enable/Disable for HDCP2.2 

@def HDMI_IOW_SET_MHL_ODT
Set MHL ODT Impedance Value (46/50/55 Ohm)
*/


/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define HDMI_IOW_INIT						_IOW(HDMI_IOC_MAGIC,  0, LX_HDMI_INIT_T)
#define HDMI_IOW_SET_PORT					_IOW(HDMI_IOC_MAGIC,  1, UINT32)
#define HDMI_IOR_GET_MODE					_IOR(HDMI_IOC_MAGIC,  2, LX_HDMI_MODE_T )
#define HDMI_IOR_GET_ASPECTRATIO			_IOR(HDMI_IOC_MAGIC,  3, LX_HDMI_ASPECTRATIO_T)
#define HDMI_IOR_GET_COLOR_DOMAIN			_IOR(HDMI_IOC_MAGIC,  4, LX_HDMI_COLOR_DOMAIN_T)
#define HDMI_IOR_GET_TIMING_INFO			_IOR(HDMI_IOC_MAGIC,  5, LX_HDMI_TIMING_INFO_T)

#define HDMI_IOR_GET_AVI_INFO				_IOR(HDMI_IOC_MAGIC,  10, LX_HDMI_INFO_PACKET_T)
#define HDMI_IOR_GET_VSI_INFO				_IOR(HDMI_IOC_MAGIC,  11, LX_HDMI_INFO_PACKET_T)
#define HDMI_IOR_GET_SPD_INFO				_IOR(HDMI_IOC_MAGIC,  12, LX_HDMI_INFO_PACKET_T)
#define HDMI_IOR_GET_INFOFRAME				_IOR(HDMI_IOC_MAGIC,  13, LX_HDMI_INFO_PACKET_T)

#define HDMI_IOW_SET_POWER					_IOW(HDMI_IOC_MAGIC,  20, UINT32)
#define HDMI_IOW_SET_HPD					_IOW(HDMI_IOC_MAGIC,  21, LX_HDMI_HPD_T)
#define HDMI_IOR_GET_HDMI_STATUS			_IOR(HDMI_IOC_MAGIC,  22, LX_HDMI_STATUS_T)
//#define HDMI_IOW_SET_UD_3D_MODE				_IOW(HDMI_IOC_MAGIC,  23, LX_HDMI_UD_3D_MODE_T)

//For HDMI HDCP debug
#define HDMI_IOR_REG_RD						_IOR(HDMI_IOC_MAGIC,  30, UINT32)
#define HDMI_IOW_REG_WR						_IOW(HDMI_IOC_MAGIC,  31, LX_HDMI_PHY_REG_WRITE_T)
#define HDMI_IOW_HDCP_ENABLE				_IOW(HDMI_IOC_MAGIC,  32, BOOLEAN)
#define HDMI_IOW_SET_HDCP					_IOW(HDMI_IOC_MAGIC,  33, LX_HDMI_HDCP_T )
#define HDMI_IOW_INTERFACE					_IOW(HDMI_IOC_MAGIC,  34, S_INTERFACE_REG)
#define HDMI_IOW_DEBUG_PRINT_CTRL			_IOW(HDMI_IOC_MAGIC,  35, LX_HDMI_DEBUG_CTRL_T)

//audio related
#define HDMI_IOR_GET_AUDIO_INFO				_IOWR(HDMI_IOC_MAGIC,  50, LX_HDMI_AUDIO_INFO_T)
#define HDMI_IOR_GET_COPY_INFO				_IOWR(HDMI_IOC_MAGIC,  51, LX_HDMI_AUDIO_COPY_INFO_T)
#define HDMI_IOW_SET_ARC					_IOW(HDMI_IOC_MAGIC,  52, LX_HDMI_ARC_CTRL_T)
#define HDMI_IOW_SET_MUTE					_IOW(HDMI_IOC_MAGIC,  53, LX_HDMI_MUTE_CTRL_T)
#define HDMI_IOR_DEBUG_GET_AUDIO_INFO		_IOWR(HDMI_IOC_MAGIC,  54, LX_HDMI_DEBUG_AUDIO_INFO_T)

#define HDMI_IOW_WRITE_EDID_DATA			_IOW(HDMI_IOC_MAGIC,	55, LX_HDMI_EDID_DATA_T)
#define HDMI_IOR_READ_EDID_DATA				_IOR(HDMI_IOC_MAGIC,	56, LX_HDMI_EDID_DATA_T)
#define HDMI_IOR_GET_PHY_STATUS				_IOR(HDMI_IOC_MAGIC,	57, LX_HDMI_PHY_INFORM_T)
#define HDMI_IOR_GET_HDCP					_IOW(HDMI_IOC_MAGIC,  58, LX_HDMI_HDCP_T )
#define HDMI_IOR_GET_AKSV_DATA				_IOR(HDMI_IOC_MAGIC,	59, LX_HDMI_AKSV_DATA_T)
#define HDMI_IOW_THREAD_CONTROL				_IOW(HDMI_IOC_MAGIC,	60, UINT32)
#define HDMI_IOW_SEND_RCP					_IOW(HDMI_IOC_MAGIC,    61, UINT8)
#define HDMI_IOW_SEND_WRITEBURST			_IOW(HDMI_IOC_MAGIC,    62, LX_HDMI_MHL_WRITEBURST_DATA_T)
#define HDMI_IOR_READ_WRITEBURST			_IOR(HDMI_IOC_MAGIC,    63, LX_HDMI_MHL_WRITEBURST_DATA_T)
#define HDMI_IOW_CALL_TYPE					_IOW(HDMI_IOC_MAGIC,	64, LX_HDMI_CALL_TYPE_T)
#define HDMI_IOW_RESET_INTERNAL_EDID		_IOW(HDMI_IOC_MAGIC,	65, LX_HDMI_RESET_INTERNAL_EDID_T)
#define HDMI_IOW_ENABLE_EXTERNAL_DDC_ACCESS	_IOW(HDMI_IOC_MAGIC,	67, LX_HDMI_EXT_DDC_ACCESS_T)
#define HDMI_IOR_RECEIVE_RCP				_IOW(HDMI_IOC_MAGIC,	68, LX_HDMI_RCP_RECEIVE_T)
#define HDMI_IOR_GET_DRIVER_STATUS			_IOR(HDMI_IOC_MAGIC,	69, LX_HDMI_DRIVER_STATUS_T)
#define HDMI_IOW_KEY_CONTROL				_IOW(HDMI_IOC_MAGIC,	70, LX_HDMI_CONTROL_KEY_TYPE_T)
#define HDMI_IOW_DEBUG_HDMI_DRV				_IOW(HDMI_IOC_MAGIC,	71, LX_HDMI_DRV_DEBUG_T)
#define HDMI_IOW_ENABLE_MHL					_IOW(HDMI_IOC_MAGIC,	72, BOOLEAN)
#define HDMI_IOW_HPD_HDCP22_CONTROL			_IOW(HDMI_IOC_MAGIC,	73, LX_HDMI_HPD_HDCP22_CONTROL_T)
#define HDMI_IOW_SET_MHL_ODT				_IOW(HDMI_IOC_MAGIC,	74, LX_HDMI_MHL_ODT_IMPEDANCE_T)
#define HDMI_IOC_MAXNR				75


/** @} */
#define LX_HDMI_PACKET_DATA_LENGTH	28							///< LX_HDMI_PACKET_DATA_LENGTH
#define LX_HDMI_VSI_PACKET_DATA_LENGTH 28						///< HDMI VSI Packet Data length
#define LX_HDMI_VSI_INFOFRAME_PACKET_LEN 28						///< HDMI VSI Inforframe packet length
#define LX_HDMI_VSI_VENDOR_SPECIFIC_REGID_LEN 3					///< HDMI VSI Vendor specific Regid length
#define LX_HDMI_VSI_VENDOR_SPECIFIC_PAYLOAD_LEN \
	(LX_HDMI_VSI_INFOFRAME_PACKET_LEN - LX_HDMI_VSI_VENDOR_SPECIFIC_REGID_LEN)		///< HDMI VSI Vendor specific payload length
#define LX_HDCP_KEY_SIZE	288			///< HDCP key size
#define LX_HDMI_SPD_VENDOR_NAME_LEN  	8					///< Vendor Name Data Length of SPD
#define LX_HDMI_SPD_PRODUCT_DESC_LEN    16					///< Product Description Data Length of SPD
#define LX_EDID_DATA_SIZE	256			///< EDID data size
#define	LX_WRITEBURST_SIZE	16			///< MHL Write Burst Byte Size


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * HDMI Link index.
 */
typedef enum
{
	LX_HDMI_LINK0 	 = 0,			///< HDMI Link Number 0
	LX_HDMI_LINK1 	 = 1,			///< HDMI Link Number 1
	LX_HDMI_LINK_MAX = 2,			///< HDMI Link Number Max for H15 HDMI 1.4 Link
} LX_HDMI_LINK_NUMBER_T;

/**
 * HDMI source window index.
 */
typedef enum
{
	LX_HDMI_MAIN_WINDOW = 0,		///< main window
	LX_HDMI_SUB_WINDOW				///< sub window
} LX_HDMI_WINDOW_IDX_T;

/**
 * HDMI Mute Control Mode.
 */
typedef enum
{
	LX_HDMI_MUTE_NONE = 0,			///< none
	LX_HDMI_VIDEO_MUTE,				///< video mute
	LX_HDMI_AUDIO_MUTE,				///< audio mute
	LX_HDMI_AV_MUTE,				///< video, audio mute
} LX_HDMI_MUTE_MODE_T;

/**
 * HDMI VSI Packet Status.
 * VSI Packet Status TYPE is defined.
 */
typedef enum
{
	LX_HDMI_VSI_PACKET_STATUS_NOT_RECEIVED = 0,		///< initial state
	LX_HDMI_VSI_PACKET_STATUS_STOPPED,				///< Stoped
	LX_HDMI_VSI_PACKET_STATUS_UPDATED,				///< Updated
	LX_HDMI_VSI_PACKET_STATUS_MAX					///< Max
} LX_HDMI_PACKET_STATUS_T;

/**
 * HDMI AVI Color Space.
 * AVI Color Space TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_COLORSPACE_RGB = 0,			///< RGB
	LX_HDMI_AVI_COLORSPACE_YCBCR422,		///< YCbCr422
	LX_HDMI_AVI_COLORSPACE_YCBCR444,		///< YCbCr444
	LX_HDMI_AVI_COLORSPACE_YCBCR420,		///< YCbCr420
	//LX_HDMI_AVI_COLORSPACE_FUTURE			///< Future
} LX_HDMI_AVI_COLORSPACE_T;

/**
 * HDMI AVI ACTIVE FORMAT INFO.
 * TVI ACTIVE FORMAT DATA is defined.
 *
 */
typedef enum
{
	LX_HDMI_AVI_ACTIVE_INFO_NO_DATA = 0,   	///< ACTIVE FORMAT No data
	LX_HDMI_AVI_ACTIVE_INFO_VALID			///< Valid data
} LX_HDMI_AVI_ACTIVE_FORMAT_INFO_T;

/**
 * HDMI AVI BAR INFO.
 * AVI BAR DATA is defined.
 */
typedef enum
{
	LX_HDMI_AVI_BAR_INFO_INVALID,					///< Invalid
	LX_HDMI_AVI_BAR_INFO_VERTICALVALID,				///< V Valid
	LX_HDMI_AVI_BAR_INFO_HORIZVALID,				///< H Valid
	LX_HDMI_AVI_BAR_INFO_VERTHORIZVALID				///< V and H Valid
} LX_HDMI_AVI_BAR_INFO_T;

/**
 * HDMI AVI SCAN INFO.
 * AVI SCAN TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_SCAN_INFO_NO_DATA,				///< Scan info no data
	LX_HDMI_AVI_SCAN_INFO_OVER_SCANNED,			///< over scan
	LX_HDMI_AVI_SCAN_INFO_UNDER_SACNNED,		///< under scan
	LX_HDMI_AVI_SCAN_INFO_FUTURE				///< Future
} LX_HDMI_AVI_SCAN_INFO_T;

/**
 * HDMI AVI COLORIMETRY.
 * AVI COLORIMETRY TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_COLORIMETRY_NO_DATA =  0,		///< COLORIMETRY No data
	LX_HDMI_AVI_COLORIMETRY_ITU601, 			///< ITU601(SMPTE170)
	LX_HDMI_AVI_COLORIMETRY_ITU709,				///< ITU709
	LX_HDMI_AVI_COLORIMETRY_EXTEND				///< Future
} LX_HDMI_AVI_COLORIMETRY_T;

/**
 * HDMI AVI PICTURE ARC.
 * AVI PICTURE ARC TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_PICTURE_ARC_NO_DATA = 0,			///< Picture ARC No data
	LX_HDMI_AVI_PICTURE_ARC_4_3,					///< ARC_4_3
	LX_HDMI_AVI_PICTURE_ARC_16_9,					///< ARC_16_9
	LX_HDMI_AVI_PICTURE_ARC_FUTURE					///< Future
} LX_HDMI_AVI_PICTURE_ARC_T;

/**
 * HDMI AVI ACTIVE FORMAT ARC.
 * AVI ACTIVE FORMAT ARC TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_SAME  =  8,		///< Active ARC smae
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_4_3   =  9,		///< Active ARC_4_3
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_16_9  = 10,		///< Active ARC_16_9
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_14_9  = 11,		///< Active ARC_14_9
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_OTHER =  0		///< Active ARC other
} LX_HDMI_AVI_ACTIVE_FORMAT_ARC_T;

/**
 * HDMI AVI IT CONTENT.
 * AVI IT CONTENT DATA is defined.
 */
typedef enum
{
	LX_HDMI_AVI_IT_CONTENT_NO_DATA     =  0,	///< IT Content NO data
	LX_HDMI_AVI_IT_CONTENT_ITCONTENT   =  1		///< IT Content
} LX_HDMI_AVI_IT_CONTENT_T;

/**
 * HDMI AVI EXT COLORIMETRY.
 * AVI EXT COLORIMETRY TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_EXT_COLORIMETRY_XVYCC601 = 0,		///< xvYCC601
	LX_HDMI_AVI_EXT_COLORIMETRY_XVYCC709,			///< xvYCC709
	LX_HDMI_AVI_EXT_COLORIMETRY_SYCC601,			///<
	LX_HDMI_AVI_EXT_COLORIMETRY_ADOBEYCC601,		///< 
	LX_HDMI_AVI_EXT_COLORIMETRY_ADOBERGB,			///<
	LX_HDMI_AVI_EXT_COLORIMETRY_BT2020CL,			///< 
	LX_HDMI_AVI_EXT_COLORIMETRY_BT2020NCL,			///< 
	LX_HDMI_AVI_EXT_COLORIMETRY_RESERVED			///< Reserved
}	LX_HDMI_AVI_EXT_COLORIMETRY_T;

/**
 * HDMI AVI RGB QUANTIZATION RANGE.
 * AVI RGB QUANTIZATION RANGE TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_DEFAULT    =  0,		///< QuanRange default
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_LIMITED,				///< QuanRange limit 15~235(220)
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_FULL,				///< QuanRange full  0 ~255(256)
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_RESERVED				///< QuanRange RESERVED
} LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T;

/**
 * HDMI AVI SCALING.
 * AVI SCALING TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_SCALING_NOKNOWN = 0,		///< Scaling unknown
	LX_HDMI_AVI_SCALING_HORIZ,				///< Scaling Horizontal
	LX_HDMI_AVI_SCALING_VERT,				///< Scaling Vertical
	LX_HDMI_AVI_SCALING_HORIZVERT			///< Scaling Horizontal and Vertical
} LX_HDMI_AVI_SCALING_T;

/**
 * HDMI AVI YCC QUANTIZATION RANGE.
 * AVI QUANTIZATION RANGE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_LIMITED = 0,		///< YCC Limited Range
	LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_FULL,			///< YCC Full Range
	LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_RESERVED			///< YCC RESERVED
} LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_T;

/**
 * HDMI AVI CONTENT TYPE.
 * AVI CONTENT TYPE is defined.
 */
typedef enum
{
	LX_HDMI_AVI_CONTENT_GRAPHICS = 0,		///< CONTENT GRAPHICS
	LX_HDMI_AVI_CONTENT_PHOTO,				///< CONTENT PHOTO
	LX_HDMI_AVI_CONTENT_CINEMA,				///< CONTENT CINEMA
	LX_HDMI_AVI_CONTENT_GAME				///< CONTENT GAME
} LX_HDMI_AVI_CONTENT_T;

/**
 * HDMI Audio Type.
 * HDMI Audio type is defined.
 */
typedef  enum
{
	LX_HDMI_AUDIO_DVI			= 0,		///< DVI Type
	LX_HDMI_AUDIO_NO_AUDIO		= 1,		///< No Audio Type
	LX_HDMI_AUDIO_PCM			= 2,		///< PCM Type
	LX_HDMI_AUDIO_AC3			= 3,		///< AC3 Type
	LX_HDMI_AUDIO_DTS			= 4,		///< DTS Type
	LX_HDMI_AUDIO_AAC			= 5,		///< AAC Type
	LX_HDMI_AUDIO_DEFAULT		= 6,		///< DEFAULT Type
	LX_HDMI_AUDIO_MPEG			= 10, 		///< MPEG Type
	LX_HDMI_AUDIO_DTS_HD_MA		= 11,		///< DTS-HD MA Type
	LX_HDMI_AUDIO_DTS_EXPRESS	= 12,		///< DTS-Express Type
	LX_HDMI_AUDIO_DTS_CD	 	= 13,		///< DTS-CD Type

	LX_HDMI_AUDIO_AC3_IEC60958	= 20,		///< AC3 Type, IEC60958 Format(Internal Use)
} LX_HDMI_AUDIO_TYPE_T;

/**
 * HDMI Audio Sample Frequency.
 * Sample frequency values merged from HDMI Audio InfoFrame and Audio Channel Status.
 * Also, this enumeration describes the sampling rate for ADEC driver.
 * If this value is changed, LX_ADEC_SAMPLING_FREQ_T is also changed in ADEC kernel driver.
 */
typedef enum
{
	LX_HDMI_SAMPLING_FREQ_NONE		=      0,		///< None
	LX_HDMI_SAMPLING_FREQ_4_KHZ		=   4000,		///< 4 Kbps
	LX_HDMI_SAMPLING_FREQ_8_KHZ		=   8000,		///< 8 Kbps
	LX_HDMI_SAMPLING_FREQ_11_025KHZ	=  11025,		///< 11.025 Kbps
	LX_HDMI_SAMPLING_FREQ_12_KHZ	=  12000,		///< 12 kbps
	LX_HDMI_SAMPLING_FREQ_16_KHZ	=  16000,		///< 16 Kbps
	LX_HDMI_SAMPLING_FREQ_22_05KHZ	=  22050,		///< 22.05 Kbps
	LX_HDMI_SAMPLING_FREQ_24_KHZ	=  24000,		///< 24 Kbps
	LX_HDMI_SAMPLING_FREQ_32_KHZ	=  32000,		///< 32 Kbps
	LX_HDMI_SAMPLING_FREQ_44_1KHZ	=  44100,		///< 44.1 Kbps
	LX_HDMI_SAMPLING_FREQ_48_KHZ	=  48000,		///< 48 Kbps
	LX_HDMI_SAMPLING_FREQ_64_KHZ	=  64000,		///< 64 Kbps
	LX_HDMI_SAMPLING_FREQ_88_2KHZ	=  88200,		///< 88.2 Kbps
	LX_HDMI_SAMPLING_FREQ_96_KHZ	=  96000,		///< 96 Kbps
	LX_HDMI_SAMPLING_FREQ_128_KHZ 	= 128000,		///< 128 Kbps
	LX_HDMI_SAMPLING_FREQ_176_4KHZ	= 176400,		///< 176.4 Kbps
	LX_HDMI_SAMPLING_FREQ_192_KHZ	= 192000,		///< 192 Kbps
	LX_HDMI_SAMPLING_FREQ_768_KHZ	= 768000,		///< 768 Kbps
	LX_HDMI_SAMPLING_FREQ_DEFAULT	= 999000		///< default
} LX_HDMI_SAMPLING_FREQ_T;

/**
 * HDMI Audio Copyright Info. Type.
 * HDMI Audio Copyright Information type is defined.
 */
typedef enum {
	LX_HDMI_AUDIO_COPY_FREE			=	0,			///< Copy Free state   (cp-bit : 1, L-bit : 0)
	LX_HDMI_AUDIO_COPY_NO_MORE		=	1,			///< Copy no more state(cp-bit : 0, L-bit : 1)
	LX_HDMI_AUDIO_COPY_ONCE			=	2,			///< Copy once state   (cp-bit : 0, L-bit : 0)
	LX_HDMI_AUDIO_COPY_NEVER		=	3,			///< Copy never state  (cp-bit : 0, L-bit : 1)
} LX_HDMI_AUDIO_COPY_T;

/**
 * HDMI Color Depth Type.
 * HDMI Color Depth is defined.
 */
typedef  enum
{
	LX_HDMI_COLOR_DEPTH_8 = 0,		///< Color depth 24 bits
	LX_HDMI_COLOR_DEPTH_10,			///< Color depth 30 bits
	LX_HDMI_COLOR_DEPTH_12			///< Color depth 36 bits
} LX_HDMI_COLOR_DEPTH_T;

/**
 * Video Format of HDMI Vendor Specific Infoframe.
 */
typedef enum
{
	LX_FORMAT_NO_ADDITIONAL_FORMAT, 			///< No additional HDMI video format
	LX_FORMAT_EXTENDED_RESOLUTION_FORMAT,		///< Extended resolution format(4K x 2K)
	LX_FORMAT_3D_FORMAT,						///< 3D format
}LX_HDMI_VSI_VIDEO_FORMAT_T;

/**
 * 3D Structure field of HDMI Vendor Specific Infoframe.
 */
typedef enum
{
	LX_HDMI_VSI_3D_STRUCTURE_FRAME_PACKING,				///< Frame packing
	LX_HDMI_VSI_3D_STRUCTURE_FIELD_ALTERNATIVE,			///< Field alternative
	LX_HDMI_VSI_3D_STRUCTURE_LINE_ALTERNATIVE,			///< Line alternative
	LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_FULL,			/// Side-by-Side (Full)

	LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH,					///< L + Depth
	LX_HDMI_VSI_3D_STRUCTURE_L_DEPTH_GRAPHICS,			///< L + Depth + Graphics

	LX_HDMI_VSI_3D_STRUCTURE_TOP_BOTTOM,				///< Top and Bottim
	LX_HDMI_VSI_3D_STRUCTURE_SIDEBYSIDE_HALF = 0x08,	///< Side-by-Side (half)
	LX_HDMI_VSI_3D_STRUCTURE_MAX						///< MAX
}LX_HDMI_VSI_3D_STRUCTURE_T;

/**
 * 3D Ext Data of HDMI Vendor Specific Infoframe.
 */
typedef enum
{
	LX_HDMI_VSI_3D_EXT_DATA_HOR_SUB_SAMPL_0,		///< Horizontal Sub-sampling - Odd/left picture, Odd/right picture
	LX_HDMI_VSI_3D_EXT_DATA_HOR_SUB_SAMPL_1,		///< Horizontal Sub-sampling - Odd/left picture, Even/right picture
	LX_HDMI_VSI_3D_EXT_DATA_HOR_SUB_SAMPL_2,		///< Horizontal Sub-sampling - Even/left picture, Odd/right picture
	LX_HDMI_VSI_3D_EXT_DATA_HOR_SUB_SAMPL_3,		///< Horizontal Sub-sampling - Even/left picture, Even/right picture

	LX_HDMI_VSI_3D_EXT_DATA_QUINCUNX_MATRIX_0,		///< Quincunx Matrix - Odd/left picture, Odd/right picture
	LX_HDMI_VSI_3D_EXT_DATA_QUINCUNX_MATRIX_1,		///< Quincunx Matrix - Odd/left picture, Even/right picture
	LX_HDMI_VSI_3D_EXT_DATA_QUINCUNX_MATRIX_2,		///< Quincunx Matrix - Even/left picture, Odd/right picture
	LX_HDMI_VSI_3D_EXT_DATA_QUINCUNX_MATRIX_3,		///< Quincunx Matrix - Even/left picture, Even/right picture

}LX_HDMI_VSI_3D_EXT_DATA_T;

/**
 * HDMI Video Format Identification Code for extended resolution transmission.
 *
*/
typedef enum
{
	LX_HDMI_VSI_VIC_RESERVED,			///< Reserved
	LX_HDMI_VSI_VIC_4K2K_30HZ,			///< 4K x 2K @ 30Hz
	LX_HDMI_VSI_VIC_4K2K_25HZ,			///< 4K x 2K @ 25Hz
	LX_HDMI_VSI_VIC_4K2K_24HZ,			///< 4K x 2K @ 24Hz
	LX_HDMI_VSI_VIC_4K2K_24HZ_SMPTE,	///< 4K x 2K @ 24Hz (SMPTE)

}LX_HDMI_VSI_VIC_T;

/**
 * HDMI Data Dridge Control Parameter for UD and specific 3D(SS Full & Frame Packing) format operating.
 *
*/
typedef enum
{
	LX_HDMI_UD_3D_OFF,			///< OFF - 2D (No Data Dridge)
	LX_HDMI_UD,					///< UD Format
	LX_HDMI_3D_SS_FULL,			///< 3D Side by Side Full
	LX_HDMI_3D_FP_60HZ,			///< 3D Frame Packing 1080p@60Hz
	LX_HDMI_UD_SCALING,			///< UD Fromat Scaling for home dash mode
}LX_HDMI_UD_3D_MODE_T;


/**
 * 3D Video Full Timing Information.
 *
*/
typedef enum
{
	LX_HDMI_EXT_2D_FORMAT,				///< 2D format
	LX_HDMI_EXT_3D_FRAMEPACK,			///< 3D frame packing format
	LX_HDMI_EXT_3D_FIELD_ALTERNATIVE,	///< 3D field alternative format
	LX_HDMI_EXT_3D_LINE_ALTERNATIVE, 	///< 3D line alternative format(row interleaving)
	LX_HDMI_EXT_3D_SBSFULL,				///< 3D Side by Side Full format
	LX_HDMI_EXT_3D_L_DEPTH,				///< 3D L + Depth format
	LX_HDMI_EXT_3D_L_GRAPHICS,			///< 3D L + Depth + Graphics format
	LX_HDMI_EXT_3D_TNB,					///< 3D Top and Bottim format
	LX_HDMI_EXT_3D_SBS,					///< 3D Side by Side Half format
	LX_HDMI_EXT_4K_2K,					///< 4K x 2K format
	LX_HDMI_EXT_MAX						///< Ext format Max
} LX_HDMI_EXT_FORMAT_INFO_T;

/**
 * HDMI AVI Color Space.
 * AVI Color Space TYPE is defined.
 */
typedef enum
{
	LX_HDMI_CSC_HDMI_RGB = 0,			///< HDMI RGB
	LX_HDMI_CSC_HDMI_YCBCR422,		///< HDMI YCbCr422
	LX_HDMI_CSC_HDMI_YCBCR444,		///< HDMI YCbCr444
	LX_HDMI_CSC_HDMI_FUTURE,			///< HDMI Future(unknown)
	LX_HDMI_CSC_DVI_RGB				///< Future
} LX_HDMI_COLORSPACE_T;


/**
 * HDMI AVI EXT COLORIMETRY.
 * AVI EXT COLORIMETRY TYPE is defined.
 */
typedef enum
{
	LX_HDMI_EXT_COLORIMETRY_XVYCC601 = 0,		///< xvYCC601
	LX_HDMI_EXT_COLORIMETRY_XVYCC709,			///< xvYCC709
	LX_HDMI_EXT_COLORIMETRY_SYCC601,			///< sYCC601
	LX_HDMI_EXT_COLORIMETRY_ADOBE,				///< Adobe YCC601
	LX_HDMI_EXT_COLORIMETRY_RESERVED			///< Reserved
}	LX_HDMI_EXT_COLORIMETRY_T;
/**
 * HDMI AVI COLORIMETRY.
 * AVI COLORIMETRY TYPE is defined.
 */
typedef enum
{
	LX_HDMI_COLORIMETRY_NO_DATA =  0,		///< COLORIMETRY No data
	LX_HDMI_COLORIMETRY_ITU601, 				///< ITU601(SMPTE170)
	LX_HDMI_COLORIMETRY_ITU709,				///< ITU709
	LX_HDMI_COLORIMETRY_EXTEND				///< Cheack Extended Colorimetry
} LX_HDMI_COLORIMETRY_T;

/**
 * HDMI AVI IT CONTENT.
 * AVI IT CONTENT DATA is defined.
 */
typedef enum
{
	LX_HDMI_IT_CONTENT_NO_DATA     =  0,	///< IT Content NO data
	LX_HDMI_IT_CONTENT_ITCONTENT   =  1		///< IT Content
} LX_HDMI_IT_CONTENT_T;

/**
 * HDMI AVI RGB QUANTIZATION RANGE.
 * AVI RGB QUANTIZATION RANGE TYPE is defined.
 */
typedef enum
{
	LX_HDMI_RGB_QUANTIZATION_RANGE_DEFAULT    =  0,		///< QuanRange default
	LX_HDMI_RGB_QUANTIZATION_RANGE_LIMITED,				///< QuanRange limit 15~235(220)
	LX_HDMI_RGB_QUANTIZATION_RANGE_FULL,				///< QuanRange full  0 ~255(256)
	LX_HDMI_RGB_QUANTIZATION_RANGE_RESERVED				///< QuanRange RESERVED
} LX_HDMI_RGB_QUANTIZATION_RANGE_T;

/**
 * HDMI InfoFrame TYPE is defined.
 */
typedef  enum
{
	LX_HDMI_INFO_AVI,
	LX_HDMI_INFO_VSI,
	LX_HDMI_INFO_SPD,
	LX_HDMI_INFO_INFO,
	LX_HDMI_INFO_MAX
} LX_HDMI_INFO_T;

/**
 * HDMI Task control Info is defined.
 */
typedef  enum
{
	HDMI_STATE_DISABLE,				//0
	HDMI_STATE_IDLE,				//1
	HDMI_STATE_NO_SCDT,				//2
	HDMI_STATE_SCDT,				//3
	HDMI_STATE_CHECK_SOURCE_MUTE,	//4
	HDMI_STATE_CHECK_AVI_NO,		//5
	HDMI_STATE_CHECK_AVI_CHG,		//6
	HDMI_STATE_INTE_CHECK,			//7
	HDMI_STATE_CHECK_MODE,			//8
	HDMI_STATE_READ,				//9
	HDMI_STATE_STABLE,				//10
	HDMI_STATE_MAX,					//11
} LX_HDMI_STATE_T;

///////////////////////////////////////////////////////////////////////////////////////
/**
 * hdmi ioctl parameter.
 * HDMI Init parameter.
 * Set HDMI Chip and Switch information.
 * @see HDMI_IOW_INIT
 */
typedef struct
{
	BOOLEAN		bHdmiSW;					///< HDMI SW 0: None, 1: Switch
} LX_HDMI_INIT_T;


/**
 * hdmi ioctl parameter.
 * HDMI mode parameter.
 * Read HDMI / DVI mode.
 * @see HDMI_IOR_GET_MODE
 */
typedef struct
{
	BOOLEAN bHDMI;   					///< 0 : DVI mode, 1 : HDMI Mode
} LX_HDMI_MODE_T;

/**
 * hdmi ioctl parameter.
 * HDMI ASPECTRATIO parameter.
 * Get HDMI ASPECTRATIO.
 * @see HDMI_IOR_GET_ASPECTRATIO
 *
 */
typedef struct
{
	LX_HDMI_AVI_PICTURE_ARC_T eAspectRatio;		///< LX_HDMI_AVI_PICTURE_ARC_T
} LX_HDMI_ASPECTRATIO_T;

/**
 * hdmi ioctl parameter.
 * HDMI ASPECTRATIO parameter.
 * Get HDMI ASPECTRATIO.
 * @see HDMI_IOR_GET_ASPECTRATIO
 */

typedef struct
{
	BOOLEAN bHdmiMode;						///< 0:DVI   1:HDMI
	LX_HDMI_AVI_COLORSPACE_T ePixelEncoding;		///< LX_HDMI_AVI_PICTURE_ARC_T
	LX_HDMI_AVI_COLORIMETRY_T eColorimetry;
	LX_HDMI_AVI_EXT_COLORIMETRY_T eExtColorimetry;
	LX_HDMI_AVI_IT_CONTENT_T eITContent;							///< LX_HDMI_AVI_IT_CONTENT_T
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T eRGBQuantizationRange;		///< LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T
#if 0
	LX_HDMI_COLORSPACE_T ePixelEncoding;		///< LX_HDMI_AVI_PICTURE_ARC_T
	LX_HDMI_COLORIMETRY_T eColorimetry;
	LX_HDMI_EXT_COLORIMETRY_T eExtColorimetry;
	LX_HDMI_IT_CONTENT_T eITContent;							///< LX_HDMI_AVI_IT_CONTENT_T
	LX_HDMI_RGB_QUANTIZATION_RANGE_T eRGBQuantizationRange;		///< LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T
#endif
} LX_HDMI_COLOR_DOMAIN_T;

/**
 * hdmi ioctl parameter.
 * HDMI TIMING INFO Parameter.
 * Get HDMI TIMING INFO.
 * @see HDMI_IOR_GET_TIMING_INFO
 */
typedef struct {
	int		prt_num;				/// < port selection information(-1: use previous selected port)
	UINT16 hFreq; 					///< Horizontal frequency(100 Hz unit)
	UINT16 vFreq; 					///< Veritical frequency(1/10 Hz unit)
	UINT16 hTotal; 					///< Horizontal total pixels
	UINT16 vTotal; 					///< Vertical total lines
	UINT16 hStart; 					///< Horizontal start pixel
	UINT16 vStart;					///< Vertical start lines
	UINT16 hActive;					///< Horizontal active pixel
	UINT16 vActive; 				///< Vertical active lines
	UINT16 scanType; 				///< Scan type (0 : interlace, 1 : progressive)
	//UINT16 HdmiLock;				///< Sampling phase
	LX_HDMI_EXT_FORMAT_INFO_T  extInfo;		///< Ext format Information (3D, 4Kx2K)
	LX_HDMI_STATE_T		state;
	UINT8	PixelRepeat;
} LX_HDMI_TIMING_INFO_T;

/**
 * hdmi ioctl parameter.
 * HDMI STATUS.
 * Get HDMI STATUS.
 * @see HDMI_IOR_GET_HDMI_STATUS
 */
typedef struct
{
	BOOLEAN bHdmiMode;						///< 0:DVI   1:HDMI
	BOOLEAN eHotPlug;						///< 0:Enabled  1:disabled
	LX_HDMI_COLOR_DEPTH_T eColorDepth;		///< 0:8bit  1:10bit  2:12bit
	UINT8 pixelRepet;						///< PixelRepetition
	LX_HDMI_AVI_COLORSPACE_T csc;		    ///< PixelEncoding
	//BOOLEAN eHDCP;
	//BOOLEAN validHdmiStatus;
} LX_HDMI_STATUS_T;

/**
 * hdmi ioctl parameter.
 * HDMI Packet parameter.
 * @see HDMI_VSI_T, HDMI_AVI_T
 */
typedef struct
{
	UINT8 type;            ///< packet type
	UINT8 version;         ///< packet version
	UINT8 length;          ///< packet length
	UINT8 dataBytes[LX_HDMI_PACKET_DATA_LENGTH]; ///< packet data
} LX_HDMI_PACKET_T;

/**
 * hdmi ioctl parameter.
 * HDMI VSI Info.
 * Get VSI Info.
 */
typedef struct
{
	UINT8 IEERegId[LX_HDMI_VSI_VENDOR_SPECIFIC_REGID_LEN];   	///< 24 bit IEEE Registration Identifier[3]
	UINT8 PayLoad[LX_HDMI_VSI_VENDOR_SPECIFIC_PAYLOAD_LEN]; 	///< Payload  [25]

	LX_HDMI_PACKET_T packet;                               		///< Packet statue
	LX_HDMI_PACKET_STATUS_T packetStatus;						///< Raw HDMI inforFrame packet data

	LX_HDMI_VSI_VIDEO_FORMAT_T eVideoFormat;	/**< HDMI VSI info */
	LX_HDMI_VSI_3D_STRUCTURE_T e3DStructure;	/**< HDMI VSI info */
	LX_HDMI_VSI_3D_EXT_DATA_T e3DExtData;		/**< HDMI VSI info */
	LX_HDMI_VSI_VIC_T eVSiVIC;					/**< HDMI VSI info */
} LX_HDMI_VSI_T;

/**
 * hdmi ioctl parameter.
 * HDMI VSI PACKET Info.
 * Get VSI PACKET Info.
 * @see HDMI_IOR_GET_HDMI_VSI
 */
typedef struct
{
	LX_HDMI_WINDOW_IDX_T srcIdx;		///< HDMI Window
	LX_HDMI_VSI_T eVsiPacket;			///< HDMI VSI INFO
} LX_HDMI_VSI_PACKET_T;

/**
 * hdmi ioctl parameter.
 * HDMI AVI Info.
 * Get AVI Info.
 */
typedef struct
{
	LX_HDMI_AVI_COLORSPACE_T ePixelEncoding; 						///< LX_HDMI_AVI_COLORSPACE_T

	LX_HDMI_AVI_ACTIVE_FORMAT_INFO_T eActiveInfo; 					///< LX_HDMI_AVI_ACTIVE_FORMAT_INFO_T
	LX_HDMI_AVI_BAR_INFO_T    eBarInfo; 			 				///< LX_HDMI_AVI_BAR_INFO_T
	LX_HDMI_AVI_SCAN_INFO_T   eScanInfo;  		 					///< LX_HDMI_AVI_SCAN_INFO_T

	LX_HDMI_AVI_COLORIMETRY_T eColorimetry;							///< LX_HDMI_AVI_COLORIMETRY_T
	LX_HDMI_AVI_PICTURE_ARC_T ePictureAspectRatio; 					///< LX_HDMI_AVI_PICTURE_ARC_T
	LX_HDMI_AVI_ACTIVE_FORMAT_ARC_T eActiveFormatAspectRatio; 		///< LX_HDMI_AVI_ACTIVE_FORMAT_ARC_T

	LX_HDMI_AVI_IT_CONTENT_T eITContent;							///< LX_HDMI_AVI_IT_CONTENT_T
	LX_HDMI_AVI_EXT_COLORIMETRY_T eExtendedColorimetry;				///< LX_HDMI_AVI_EXT_COLORIMETRY_T
	LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T eRGBQuantizationRange;		///< LX_HDMI_AVI_RGB_QUANTIZATION_RANGE_T
	LX_HDMI_AVI_SCALING_T eScaling;									///< LX_HDMI_AVI_SCALING_T

	UINT8 VideoIdCode; 												///< VideoIdCode

	LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_T	eYCCQuantizationRange;	///< LX_HDMI_AVI_YCC_QUANTIZATION_RANGE_T
	LX_HDMI_AVI_CONTENT_T	eContentType;							///< LX_HDMI_AVI_CONTENT_T
	UINT8 PixelRepeat;												///< PixelRepeat

	/* bar info */
	UINT16 TopBarEndLineNumber;										///< Top Bar End LineNumber
	UINT16 BottomBarStartLineNumber;      							///< Bottom Bar Start LineNumber
	UINT16 LeftBarEndPixelNumber;									///< Left Bar End Pixel Number
	UINT16 RightBarEndPixelNumber;        							///< Right Bar End Pixel Number

	LX_HDMI_PACKET_STATUS_T packetStatus;							///< LX_HDMI_PACKET_STATUS_T
	LX_HDMI_PACKET_T packet;										///< LX_HDMI_PACKET_T

	BOOLEAN bHdmiMode;												///< 0:DVI   1:HDMI
} LX_HDMI_AVI_T;

/**
 * hdmi ioctl parameter.
 * HDMI AVI PACKET Info.
 * Get AVI PACKET Info.
 * @see HDMI_IOR_GET_HDMI_AVI
 */
typedef struct
{
	LX_HDMI_WINDOW_IDX_T srcIdx;		///< HDMI Window
	LX_HDMI_AVI_T eAviPacket;			///< HDMI AVI INFO
} LX_HDMI_AVI_PACKET_T;

/**
 * hdmi ioctl parameter.
 * HDMI HOTPLUG CONTROL.
 * Set HDMI HOTPLUG.
 * @see HDMI_IOW_SET_HPD, HDMI_IOW_SET_PORT
 */
typedef struct
{
	BOOLEAN bEnable;					///< Hotplug Enable
} LX_HDMI_HPD_T;

/**
 * hdmi ioctl parameter.
 * HDMI Auido and Video Info.
 * Get HDMI AUDIO AND VIDEO INFORMATION.
 * @see HDMI_IOR_GET_AUDIO_INFO
 */
typedef struct
{
  LX_HDMI_LINK_NUMBER_T		linkNumber;		///< HDMI Link Number.
  LX_HDMI_AUDIO_TYPE_T		audioType;		///< HDMI Audio Type.
  LX_HDMI_SAMPLING_FREQ_T	samplingFreq;	///< Audio Sampling Frequency
} LX_HDMI_AUDIO_INFO_T;

/**
 * hdmi ioctl parameter.
 * HDMI Auido Copyright Info.
 * Get HDMI Audio Copyright Information.
 * @see HDMI_IOR_GET_COPY_INFO
 */
typedef struct
{
  LX_HDMI_LINK_NUMBER_T	  	linkNumber;	  	///< HDMI Link Number.
  LX_HDMI_AUDIO_COPY_T		copyInfo;		///< Copyright Information.
} LX_HDMI_AUDIO_COPY_INFO_T;

/**
 * hdmi ioctl parameter.
 * HDMI ARC CONTROL.
 * @see HDMI_IOW_SET_ARC
 */
typedef struct {
  LX_HDMI_LINK_NUMBER_T	  linkNumber;	  	///< HDMI Link Number.
  BOOLEAN				  bARCEnable;		///< ARC  0 : Off, 1 : On
} LX_HDMI_ARC_CTRL_T;

/**
 * hdmi ioctl parameter.
 * HDMI MUTE CONTROL.
 * Set HDMI MUTE CONTROL.
 * @see HDMI_IOW_SET_MUTE
 */
typedef struct
{
  LX_HDMI_LINK_NUMBER_T	linkNumber;	  	///< HDMI Link Number.
  BOOLEAN 				bVideoMute;		///< Video Mute(1), Clear(0)
  BOOLEAN 				bAudioMute;		///< Audio Mute(1), Clear(0)
  LX_HDMI_MUTE_MODE_T	eMode;			///< Control mode -> 0 : none
} LX_HDMI_MUTE_CTRL_T;

/**
 * hdmi ioctl parameter.
 * HDMI DEBUG AUDIO CONTROL.
 * Get HDMI DEBUG AUDIO CONTROL.
 * @see HDMI_IOR_DEBUG_GET_AUDIO_INFO
 */
typedef struct
{
  LX_HDMI_LINK_NUMBER_T	  linkNumber;	  ///< HDMI Link Number.

  UINT64		tmdsClock;			///< HDMI measured clock value of TMDS clock
  UINT64		tmdsSamplingFreq;	///< HDMI sampling frequency in source device from TMDS clock

  UINT8			chanStatusByte0;	///< HDMI Audio Channel Status Bytes 0
  UINT8			chanStatusByte1;	///< HDMI Audio Channel Status Bytes 1
  UINT8			chanStatusByte2;	///< HDMI Audio Channel Status Bytes 2
  UINT8			chanStatusByte3;	///< HDMI Audio Channel Status Bytes 3
  UINT8			chanStatusByte4;	///< HDMI Audio Channel Status Bytes 4

  UINT16		burstInfoPc;		///< HDMI Audio Burst Info. of Pc
  UINT16		burstInfoPd;		///< HDMI Audio Burst Info. of Pd

  UINT8		  	infoFrameVersion;	///< HDMI Audio InfoFrame Version
  UINT8		  	infoFrameLength;	///< HDMI Audio InfoFrame Length

  UINT8		  	infoFrameByte1;	  	///< HDMI Audio InfoFrame Bytes 1
  UINT8		  	infoFrameByte2;	  	///< HDMI Audio InfoFrame Bytes 2
  UINT8		  	infoFrameByte3;	  	///< HDMI Audio InfoFrame Bytes 3
} LX_HDMI_DEBUG_AUDIO_INFO_T;

/**
 * hdmi ioctl parameter.
 * HDMI Packet parameter.
 * @see HDMI_VSI_T, HDMI_AVI_T
 */
typedef struct
{
	int		prt_num;				/// < port selection information(-1: use previous selected port)
	UINT8	InfoFrameType;		///< Info frame type
	UINT32 	header;			 	///< header data
	UINT32 	dataBytes[8]; 		///< packet data
} LX_HDMI_INFO_PACKET_T;

/**
 * hdmi ioctl parameter.
 * HDCP parameter.
 * Set HDCP
 * @see HDMI_IOW_SET_HDCP
 */
typedef struct
{
	UINT8 u8HdcpKey[LX_HDCP_KEY_SIZE];  ///< Tx HDCP key
} LX_HDMI_HDCP_T;

/**
 * hdmi ioctl parameter.
 * Get HDMI Color parameter.
 */
typedef enum
{
	LX_HDMI_CS_FULL_RGB = 0,				///< Full RGB (0 ~ 255)
	LX_HDMI_CS_LIMITED_RGB,					///< Limited RGB (16 ~235)

	LX_HDMI_CS_YCBCR422_601,				///< YCBCR422_601
	LX_HDMI_CS_YCBCR422_709,				///< YCBCR422_709
	LX_HDMI_CS_XVYCC422_601,				///< XVYCC422_601
	LX_HDMI_CS_XVYCC422_709,				///< XVYCC422_709
	LX_HDMI_CS_YCBCR422_2020_CL,			///< YCBCR420 ITU-R BT.2020 Constant Luminance
	LX_HDMI_CS_YCBCR422_2020_NCL,			///< YCBCR420 ITU-R BT.2020 Non-constant Luminace
	
	LX_HDMI_CS_YCBCR444_601,				///< YCBCR444_601
	LX_HDMI_CS_YCBCR444_709,				///< YCBCR444_709
	LX_HDMI_CS_XVYCC444_601,				///< XVYCC444_601
	LX_HDMI_CS_XVYCC444_709,				///< XVYCC444_709
	LX_HDMI_CS_YCBCR444_2020_CL,			///< YCBCR420 ITU-R BT.2020 Constant Luminance
	LX_HDMI_CS_YCBCR444_2020_NCL,			///< YCBCR420 ITU-R BT.2020 Non-constant Luminace
	
	LX_HDMI_CS_YCBCR420_601,				///< YCBCR420 ITU-R BT.601
	LX_HDMI_CS_YCBCR420_709,				///< YCBCR420 ITU-R BT.709
	LX_HDMI_CS_XVYCC420_601,
	LX_HDMI_CS_XVYCC420_709,
	LX_HDMI_CS_SYCC420_601,
	LX_HDMI_CS_ADOBEYCC420_601,
	LX_HDMI_CS_YCBCR420_2020_CL,			///< YCBCR420 ITU-R BT.2020 Constant Luminance
	LX_HDMI_CS_YCBCR420_2020_NCL,			///< YCBCR420 ITU-R BT.2020 Non-constant Luminace

	LX_DVI_CS_FULL_RGB 						///< Full RGB (0 ~ 255)
} LX_HDMI_COLOR_STATUS_T;

/**
 * hdmi ioctl parameter.
 * HDMI ARC Source.
 * Set HDMI input Sourc.
 */
typedef enum{
	LX_HDMI_ARC_SPDIF_FROM_L9D	= 0,		///< L9 Digital Source
	LX_HDMI_ARC_SPDIF_FROM_PIP0	= 2,		///< L9 HDMI_0 Source
	LX_HDMI_ARC_SPDIF_FROM_PIP1	= 3			///< L9 HDMI_1 Source
} LX_HDMI_ARC_PATH_T;

/**
 * hdmi ioctl parameter.
 * HDMI SPD PACKET Info.
 * Get SPD PACKET Info.
 * @see HDMI_IOR_GET_HDMI_SPD
 */
typedef struct
{
    UINT8 VendorName[LX_HDMI_SPD_VENDOR_NAME_LEN+1];  				///< Vendor Name
    UINT8 ProductDescription[LX_HDMI_SPD_PRODUCT_DESC_LEN+1];		///< Product Description
    UINT8 SourceDeviceInfo;											///< Source Device Info
} LX_HDMI_SPD_PACKET_T;


/**
 * HDMI PHY register value write type.
 * @see HDMI_IOW_REG_WR
 */
typedef struct {
	UINT32 wr_addr;			///< register address
	UINT32 wr_data;			///< write data
} LX_HDMI_PHY_REG_WRITE_T;



/**
* @brief HDMI Kernel Debug Print Control Parameters
 * @see LX_HDMI_DEBUG_CTRL_T
*/
typedef enum
{
	LX_HDMI_ERROR,
	LX_HDMI_WARN,
	LX_HDMI_NOTI,
	LX_HDMI_PRINT,
	LX_HDMI_DEBUG,
	LX_HDMI_TRACE,
	LX_HDMI_AUDIO,
	LX_HDMI_ATASK,
	LX_HDMI_VIDEO,
	LX_HDMI_TASK,
	LX_HDMI_INTR,
	LX_HDMI_MAX,          ///< max number of enurmeration
} LX_HDMI_DEBUG_TYPE_T;


/**
* @brief HDMI Kernel Debug Print Control
* @see HDMI_IOW_DEBUG_PRINT_CTRL
*/
typedef struct
{
	LX_HDMI_DEBUG_TYPE_T		printType;			///< The type of print module
	UINT32 						printColor;			///< The color of print
} LX_HDMI_DEBUG_CTRL_T;

/**
* @brief HDMI Phy Status types
* @see LX_HDMI_PHY_INFORM_T
*/
typedef enum{
	HDMI_PHY_STATUS_DISCONNECTED,	// < PHY Disconnected
	HDMI_PHY_STATUS_PORT_CHANGED,	// < Selected Port Changed
	HDMI_PHY_STATUS_EDID_RD_DONE,	// < EDID Read Done
	HDMI_PHY_STATUS_HDCP_AUTHED,	// < HDCP Authentication Done
} LX_HDMI_PHY_STATUS_T;

/**
* @brief HDMI Phy Information Structure
* @see HDMI_IOR_GET_PHY_STATUS
*/
typedef struct HDMI_PHY_INFORM {
	int		prt_sel;		// < port selection information
	int		hdmi_mode[4];		// < hdmi/dvi mode
	int 	hdmi5v[4];		//	< HDMI 5V detection on each ports
	int 	hpd_out[4];		//	< HPD out status on each ports
	int 	hpd_pol[4];		//	< HPD polarity on eash ports
	int 	hpd_in[4];		//	< HPD in status on each ports
	int		phy_pdb[4];		//  < PHY PDB inform on each ports
	int		phy_enable[4];		//  < PHY Enable inform on each ports
	int		phy_rstn[4];		//  < PHY Reset inform on each ports
	int 	edid_rd_done[4];		//  < EDID RD Done inform on each ports
	int 	scdt[4];				//  < SDCT inform on each ports
	UINT32		hdcp_authed[4];		//  < HDCP Authentication Done inform on each ports
	UINT32		hdcp_enc_en[4];		//  < HDCP Authentication Start inform on each ports
	int		cd_sense;				// < MHL CD Sence Detection flag(only in port 3)
	int		cbus_conn_done;				// < MHL CBUS Connection Done
	int		cbus_warb_done;				// < MHL CBUS Warb Done
	int		cbus_disconn;			// < MHL CBUS Disconnected
	int		phy_sink_cbus_zdis;			// < MHL Phy Sink CBUS Zdis
	int		cbus_st;					// < MHL CBUS Status
	UINT32	cbus_00;					// < MHL CBUS Register 00 Status
	UINT32	cbus_01;					// < MHL CBUS Register 01 Status
	LX_HDMI_PHY_STATUS_T	phy_status;	// current phy status
	UINT32	tmds_clock[4];				// Phy TMDS Clock
	UINT8	state_sink_rcp;				// MHL sink RCP State
	UINT8	module_init;					// HDMI module initialize/uninitialize
	UINT8	module_open;					// HDMI module open/close
	UINT8	module_conn;					// HDMI module connect/disconnect
	UINT8	hpd_enable;						// HPD out Enabled
	int		rstn_edid[4];				// sw reset status of edid on each port
	int		tcs_done[4];				// tcs_done status of each port
	int		tcs_min[4][3];				// tcs_done status of each port
	int		tcs_max[4][3];				// tcs_done status of each port
	int		tcs_result[4][3];				// tcs_done status of each port
	int		tcs_add[4];				// tcs_done status of each port
	unsigned long ecc_error_count[4];				// ecc error interrupt count
	unsigned long hdcp_error_count[4];				// hdcp error interrupt count
	unsigned long terc4_error_count[4];				// terc4 error interrupt count
	unsigned long loop_count[4];				// counting loop count
	unsigned long scdt_detect_count[4];				// scdt detect count
	unsigned long scdt_fall_count[4];				// scdt fall interrupt count
	int		mhl_pp_mode;				// MHL PP mode
	int		mhl_mode_enable;			// MHL Mode Enable Flag
	int		hpd_hdcp22_control[4];		// HPD out control by HDCP2.2 
	UINT32		hdcp22_status[4];		// HDCP 2.2 status register
	int		tcs_restart_count[4];		// tcs restart count of each port
} LX_HDMI_PHY_INFORM_T;

/**
* @brief HDMI received AKSV data on selected port
* @see HDMI_IOR_GET_AKSV_DATA
*/
typedef struct HDMI_AKSV_DATA {
	int		prt_sel;		// < port selection information
	UINT8 	Aksv_array[5];		// < 5byte AKSV information
} LX_HDMI_AKSV_DATA_T;

/**
* @brief EDID data structure for read/write EDID data in link register
* @see HDMI_IOW_WRITE_EDID_DATA
* @see HDMI_IOW_READ_EDID_DATA
*/
typedef struct {
	int		prt_sel;		// < port selection information
	UINT8 	edid_data_array[LX_EDID_DATA_SIZE];	// < EDID data array
} LX_HDMI_EDID_DATA_T;

/**
* @brief MHL WriteBurst Data Stucture
* @see HDMI_IOW_SEND_WRITEBURST
* @see HDMI_IOW_READ_WRITEBURST
*/
typedef struct HDMI_MHL_WRITEBURST_DATA {
	UINT8 	burst_data[LX_WRITEBURST_SIZE];		// < 16byte write burst data
} LX_HDMI_MHL_WRITEBURST_DATA_T;

/**
* @brief HDMI HAL Calling Information
* @see HDMI_IOW_CALL_TYPE
*/
typedef enum{
	HDMI_CALL_TYPE_INIT,						// hdmi hal calling initialize
	HDMI_CALL_TYPE_UNINIT,						// hdmi hal calling uninitialize
	HDMI_CALL_TYPE_OPEN,						// hdmi hal calling open
	HDMI_CALL_TYPE_CLOSE,						// hdmi hal calling close
	HDMI_CALL_TYPE_CONN,						// hdmi hal calling connect
	HDMI_CALL_TYPE_DISCONN,						// hdmi hal calling disconnect
} LX_HDMI_CALL_TYPE_T;

typedef enum{
	HDMI_PORT_NOT_CONNECTED,
	HDMI_PORT_CONNECTED,
	HDMI_PORT_CONNECT_MAX,
} HDMI_PORT_CONNECTION_TYPE_T;
/**
* @brief Reset to stop interal EDID operation for external DDC EEPROM to work
* @see HDMI_IOW_RESET_INTERNAL_EDID
*/
typedef struct HDMI_RESET_INTERNAL_EDID {
	int		prt_num;			// < port selection information
	int		edid_resetn;		// < internal edid resetb value
} LX_HDMI_RESET_INTERNAL_EDID_T;

/**
* @brief Enable Access to external DDC EEPROM for read/write data
* @see HDMI_IOW_ENABLE_EXTERNAL_DDC_ACCESS
*/
typedef struct HDMI_EXT_DDC_ACCESS {
	int		prt_num;			// < port selection information
	int		enable;				// < enable external ddc eeprom access
} LX_HDMI_EXT_DDC_ACCESS_T;

/**
* @brief MHL RCP Receive Command Types
* @see LX_HDMI_RCP_RECEIVE_T
*/
typedef enum{
	HDMI_RCP_RECEIVE_CMD_CHECK_RCP,				// check RCP message received
	HDMI_RCP_RECEIVE_CMD_CLEAR,				// clear received message
	HDMI_RCP_RECEIVE_CMD_GET_MSG,				// get received message
	HDMI_RCP_RECEIVE_CMD_NONE,				// command none
} LX_HDMI_RCP_RECEIVE_CMD_TYPE_T;

/**
* @brief Structure used for RCP Receive Function
* @see HDMI_IOR_RECEIVE_RCP
*/
typedef struct HDMI_RCP_RECEIVE {
	LX_HDMI_RCP_RECEIVE_CMD_TYPE_T		rcp_receive_cmd;	// < RCP receive command type
	BOOLEAN		rcp_receive_flag;							// < RCP received flag
	UINT8		rcp_msg;									// < Received RCP message
} LX_HDMI_RCP_RECEIVE_T;

/**
* @brief HDMI driver internal status
* @see HDMI_IOR_GET_DRIVER_STATUS
*/
typedef struct LX_HDMI_DRIVER_STATUS {
	LX_HDMI_TIMING_INFO_T PrevTiming[2];
	UINT32 TimingReadCnt[2];
	UINT32 HDMIState[2];
	LX_HDMI_AVI_COLORSPACE_T	PrevPixelEncoding[2];
	LX_HDMI_INFO_PACKET_T PrevAVIPacket[2];
	LX_HDMI_INFO_PACKET_T PrevVSIPacket[2];
	BOOLEAN AVIReadState[2];
	BOOLEAN VSIState[2] 	;
	BOOLEAN AVIChangeState[2];
	BOOLEAN PortSelected[2];
	UINT32 MHLContentOff;

	BOOLEAN AudioMuteState[2];
	BOOLEAN AudioArcStatus[2];
	UINT32  IntrAudioState[2];
	UINT32  HdmiPortStableCount[2];
	UINT32  HdmiFreqErrorCount[2];
	UINT32  HdmiAudioThreadCount[2];
	UINT64  IntrBurstInfoCount[2];
	UINT64  TaskBurstInfoCount[2];
	LX_HDMI_AUDIO_INFO_T HdmiAudioInfo[2];

	UINT32 Intr_vf_stable[2];
	UINT32 Intr_avi[2];
	UINT32 Intr_src_mute[2];
	UINT32 Intr_vsi[2];

	UINT32 Force_thread_sleep;
	UINT32 HDMI_thread_running;
	UINT32 HDMI_interrupt_registered;

	UINT32 ChipPlatform;

	int Pathen_set_count;
	int CHG_AVI_count_MHL[2];
	int CHG_AVI_count_EQ[2];
	int TMDS_ERROR_EQ[2];

	/* H15 Added */
	UINT32 Driver_Revision;
	UINT32 Port_Connection_Status[2];
	int hdmi_mode[2];	//dvi(0), hdmi(1) mode;
	HDMI_PORT_CONNECTION_TYPE_T HDMIConnectState[2];
	int selected_port;
	UINT32 device_suspend_count;
	UINT32 device_resumed_count;
	UINT32 device_in_suspend_mode;

} LX_HDMI_DRIVER_STATUS_T;

/**
* @brief HDMI EDID control types
* @see HDMI_IOW_EDID_CONTROL
*/
typedef enum{
	HDMI_EDID_SAVE,				// save current hdmi edid data
	HDMI_EDID_CLEAR,				// clear hdmi edid data
	HDMI_EDID_RESTORE,			// restore saved hdmi edid data
	HDMI_EDID_NONE,				// command none
} LX_HDMI_CONTROL_EDID_TYPE_T;

/**
* @brief HDMI Key control types
* @see HDMI_IOW_KEY_CONTROL
*/
typedef enum{
	HDMI_KEY_SAVE,				// save current hdmi key data
	HDMI_KEY_CLEAR,				// clear hdmi key data
	HDMI_KEY_RESTORE,			// restore saved hdmi key data
	HDMI_KEY_NONE,				// command none
} LX_HDMI_CONTROL_KEY_TYPE_T;

/**
* @brief Structure used for HDMI drv debugger
* @see HDMI_IOW_DEBUG_HDMI_DRV
*/
typedef struct HDMI_DRV_DEBUG {
	UINT32		hdmi_debug_0;	// < 1 : workaround control
	UINT32		hdmi_debug_1;	// < hdmi key control type
	UINT32		hdmi_debug_2;	// < hdmi key control type
	UINT32		hdmi_debug_3;	// < hdmi key control type
	UINT32		hdmi_debug_4;	// < hdmi key control type
	UINT32		hdmi_debug_5;	// < hdmi key control type
} LX_HDMI_DRV_DEBUG_T;

/**
* @brief HPD enable control by HDCP 2.2 driver
* @see HDMI_IOW_RESET_INTERNAL_EDID
*/
typedef struct HDMI_HPD_HDCP22_CONTROL {
	int		prt_num;				// < port selection information
	int		hpd_hdcp22_control;		// < hpd enable control by hdcp2.2 driver
} LX_HDMI_HPD_HDCP22_CONTROL_T;

/**
* @brief MHL ODT Impedance Values
* @see HDMI_IOW_SET_MHL_ODT
*/
typedef enum{
	MHL_ODT_46_OHM,				// 46 Ohm
	MHL_ODT_50_OHM,				// 50 Ohm
	MHL_ODT_55_OHM,				// 55 Ohm
} LX_HDMI_MHL_ODT_IMPEDANCE_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _HDMI_DRV_H_ */

/** @} */
