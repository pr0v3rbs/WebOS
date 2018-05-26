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
 *  application interface header for venc device
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.05.19
 *
 *  @addtogroup lg1152_venc
 *	@{
 */

#ifndef	_VENC_KAPI_H_
#define	_VENC_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#undef SUPPORT_VENC_DEVICE_FASYNC_FOPS
#define SUPPORT_VENC_DEVICE_ENC_API

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
#define	VENC_IOC_MAGIC		'n'
/**
@name VENC IOCTL List
ioctl list for venc device.

@{
@def VENC_IO_RESET
Reset venc module.
When venc device receives above IOCTL, then reset itself.

@def VENC_IOR_CHIP_REV_INFO
Reads chip revision information.
Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.

@def VENC_IOW_WRITE_UINT32
write 32bit value to venc module.
detailed...[minimum two lines of comment required. one line of comment does not generated automatically]

@def VENC_IOW_WRITE_STRUCT
control complex parameter to venc module.
control codec_type, speed, and additional parameter.
no need to use "see also", exact struct name shall be linked automatically.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
//#define	LX_VENC_IO_INIT					_IO(VENC_IOC_MAGIC,	 0 )
#define LX_VENC_IOR_GET_BUFFER_INFO			_IOR(VENC_IOC_MAGIC, 1, LX_VENC_BUFFER_T )
#define	LX_VENC_IOW_RECORD_SET_INPUT		_IOW(VENC_IOC_MAGIC, 2, LX_VENC_RECORD_INPUT_T )
#define	LX_VENC_IOW_RECORD_SET_COMMAND		_IOW(VENC_IOC_MAGIC, 3, LX_VENC_RECORD_COMMAND_T )
#define	LX_VENC_IOW_RECORD_SET_GOP			_IOW(VENC_IOC_MAGIC, 5, UINT8 )
#define	LX_VENC_IOW_RECORD_SET_ENCODETYPE	_IOW(VENC_IOC_MAGIC, 6, UINT32 )
#define LX_VENC_IOW_RECORD_SET_RC			_IOW(VENC_IOC_MAGIC, 7, LX_VENC_RECORD_RC_T )
#define LX_VENC_IOW_RECORD_GET_INFO			_IOR(VENC_IOC_MAGIC, 8, LX_VENC_RECORD_INFO_T )
#define	LX_VENC_IOR_RECORD_GET_OUTPUT		_IOR(VENC_IOC_MAGIC, 9, LX_VENC_RECORD_OUTPUT_T )

#define LX_VENC_IOR_RECORD_GET_FRAME_IMAGE 	_IOR(VENC_IOC_MAGIC, 11, LX_VENC_RECORD_FRAME_IMAGE_T)
#define	LX_VENC_IOR_RECORD_CHECK_IPCREG		_IO(VENC_IOC_MAGIC, 12 )

#ifdef SUPPORT_VENC_DEVICE_ENC_API
#define LX_VENC_IOWR_ENC_CREATE				_IOWR(VENC_IOC_MAGIC, 21, LX_VENC_ENC_CREATE_T )
#define LX_VENC_IOW_ENC_DESTROY				_IOW(VENC_IOC_MAGIC, 22, LX_VENC_ENC_DESTROY_T )
#define LX_VENC_IOWR_ENC_ENCODE				_IOWR(VENC_IOC_MAGIC, 23, LX_VENC_ENC_ENCODE_T )
#endif

#define VENC_IOC_MAXNR					34
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 output type 
*/
typedef enum
{
	LX_VENC_ENCODE_TYPE_H264 = 0,
	LX_VENC_ENCODE_TYPE_VP8 = 1,

	LX_VENC_ENCODE_TYPE_MAX = LX_VENC_ENCODE_TYPE_VP8,
} LX_VENC_ENCODE_TYPE_T;

/**
 Device buffer information.
*/
typedef struct
{
	// Thumbnail
	UINT32	ui32PhysicalAddress;
	UINT32	ui32Size;

	// ES Buffer
	UINT32	ui32ESBufPhysBase;
	UINT32	ui32ESBufSize;
	UINT32	ui32ESBufOffset;	// offset from venc base address

	// AUI Buffer
	UINT32	ui32AUIBufPhysBase;
	UINT32	ui32AUIBufSize;
	UINT32	ui32AUIBufOffset;	// offset from venc base address

#if 0
	// TS Buffer
	UINT32	ui32TSBufPhysBase;
	UINT32	ui32TSBufSize;
	UINT32	ui32TSBufOffset;	// offset from venc base address
#endif

	// Input Buffer
	UINT32	ui32InBufPhysBase;
	UINT32	ui32InBufSize;
	UINT32	ui32InBufOffset;	// offset from venc base address
} LX_VENC_BUFFER_T;

/**
 VENC module input specifier.
*/
typedef enum
{
	LX_VENC_INPUT_CVD		= 0,		///< for test only.
	LX_VENC_INPUT_DVI		= 1,		///< input from Display engine module.
	LX_VENC_INPUT_USB		= 2,		///< input from Buffer such as USB Camera.
	LX_VENC_INPUT_MJPEG		= 3,		///< FIXME input from
	LX_VENC_INPUT_VDEC		= 4,		///< input from Video decoder module.

	LX_VENC_INPUT_MIN		= LX_VENC_INPUT_CVD,
	LX_VENC_INPUT_MAX		= LX_VENC_INPUT_VDEC
} LX_VENC_INPUT_T;

/**
 color type of input frame/field.
*/
typedef enum
{
	LX_VENC_COLOR_FORMAT_400		= 0,
	LX_VENC_COLOR_FORMAT_444		= 1,
	LX_VENC_COLOR_FORMAT_422		= 2,
	LX_VENC_COLOR_FORMAT_420		= 4,

	LX_VENC_COLOR_FORMAT_MIN		= LX_VENC_COLOR_FORMAT_400,
	LX_VENC_COLOR_FORMAT_MAX		= LX_VENC_COLOR_FORMAT_420
} LX_VENC_COLOR_FORMAT_T;

/**
 input source type to be encoded.
*/
typedef enum
{
	LX_VENC_SOURCE_TYPE_FIELD		= 0,	/// input is field source.
	LX_VENC_SOURCE_TYPE_FRAME		= 1,	/// input is frame source.
	LX_VENC_SOURCE_TYPE_AUTO		= 2,	/// FIXME input is automatically determined.

	LX_VENC_SOURCE_TYPE_MIN			= LX_VENC_SOURCE_TYPE_FIELD,
	LX_VENC_SOURCE_TYPE_MAX			= LX_VENC_SOURCE_TYPE_AUTO
} LX_VENC_SOURCE_TYPE_T;

/**
 input source YUV mode to be encoded.
*/
typedef enum
{
	LX_VENC_YUYV_MODE_YUYV			= 0,	/// YUYV
	LX_VENC_YUYV_MODE_UYVY			= 1,	/// UYUV
	LX_VENC_YUYV_MODE_INTERLEAVED	= 2,	/// Y plan + Cb/Cr interleaved

	LX_VENC_YUYV_MODE_MIN			= LX_VENC_YUYV_MODE_YUYV,
	LX_VENC_YUYV_MODE_MAX			= LX_VENC_YUYV_MODE_INTERLEAVED
} LX_VENC_YUYV_MODE_T;

/**
 input source type.
*/
typedef enum
{
	LX_VENC_INPUT_SRC_NONE		= 0,
	LX_VENC_INPUT_SRC_ATV		= 1,
	LX_VENC_INPUT_SRC_DTV		= 2,
	LX_VENC_INPUT_SRC_OTHERS	= 3,

	LX_VENC_INPUT_SRC_MIN			= LX_VENC_INPUT_SRC_NONE,
	LX_VENC_INPUT_SRC_MAX			= LX_VENC_INPUT_SRC_OTHERS
} LX_VENC_INPUT_SRC_T;

/**
 Input Configuration for ES Encoding.
*/
typedef struct
{
	UINT16	ui16Width;			///< input horizontal size in pixel.
	UINT16	ui16Height;			///< input vertical   size in pixel.
	UINT16	ui16Stride;			///< input frame/field stride in bytes.

#if 1
	// Unused fields
    UINT16
            eInputMode      : 4,    ///< input mode @ref LX_VENC_INPUT_T
            eSourceType     : 2,    ///< frame or field source @ref LX_VENC_SOURCE_TYPE_T
            eColorFormat    : 4,    ///< color format @ref LX_VENC_COLOR_FORMAT_T
            eYUYVMode       : 2,    ///< YUV mode @ref LX_VENC_YUYV_MODE_T
            _reserved;

    UINT32  ui32PictureOffset;
    UINT16  ui32HeaderSizeinPic;    ///< for ES Encoding only ignored when CHB mode.

    /* Parameters for Buffer base and End */
    void    *pInBuffBase;           ///<Physical base address
    UINT32  ui32InpBuffSize;        ///<Size of input YUV buffer
#endif

	UINT32	ui32FrameRate;			///<FrameRate
	LX_VENC_INPUT_SRC_T eInputSource;

	UINT16	nSARWidth;
	UINT16	nSARHeight;
} LX_VENC_RECORD_INPUT_T;

/**
 venc command.
*/
typedef enum
{
	LX_VENC_COMMAND_NOP				= 0,	/// NOP
	LX_VENC_COMMAND_REFRESH			= 1,	/// Refresh
	LX_VENC_COMMAND_START			= 2,	/// Start
	LX_VENC_COMMAND_PAUSE			= 3,	/// Pause
	LX_VENC_COMMAND_RESUME			= 4,	/// Resume

	LX_VENC_COMMAND_MIN				= LX_VENC_COMMAND_NOP,
	LX_VENC_COMMAND_MAX				= LX_VENC_COMMAND_RESUME
} LX_VENC_RECORD_COMMAND_T;

/**
 venc picture quality.
*/
typedef enum
{
	LX_VENC_QUALITY_LOW				= 0,
	LX_VENC_QUALITY_MID_LOW			= 1,
	LX_VENC_QUALITY_MIDDLE			= 2,
	LX_VENC_QUALITY_MID_HIGH		= 3,
	LX_VENC_QUALITY_HIGH			= 4,

	LX_VENC_QUALITY_MIN				= LX_VENC_QUALITY_LOW,
	LX_VENC_QUALITY_MAX				= LX_VENC_QUALITY_HIGH
} LX_VENC_RECORD_QUALITY_T;

/**
 venc frame rate.
*/
typedef enum
{
    LX_VENC_FRAME_RATE_NONE				= 0,
	LX_VENC_FRAME_RATE_23HZ				= 1,
	LX_VENC_FRAME_RATE_24HZ				= 2,
	LX_VENC_FRAME_RATE_25HZ				= 3,
	LX_VENC_FRAME_RATE_29HZ				= 4,
	LX_VENC_FRAME_RATE_30HZ				= 5,
	LX_VENC_FRAME_RATE_50HZ				= 6,
	LX_VENC_FRAME_RATE_59HZ				= 7,
	LX_VENC_FRAME_RATE_60HZ				= 8,
	LX_VENC_FRAME_RATE_15HZ				= 9,

	LX_VENC_FRAME_RATE_MIN				= LX_VENC_FRAME_RATE_23HZ,
	LX_VENC_FRAME_RATE_MAX				= LX_VENC_FRAME_RATE_15HZ
} LX_VENC_RECORD_FRAME_RATE_T;

/**
 venc encoding information.
*/
typedef struct
{
	LX_VENC_RECORD_FRAME_RATE_T	ui32FrameRate;
	UINT32						ui32BitRate;
	UINT16						ui16Width;
	UINT16						ui16Height;
	
	// jaeseop.so (2011/11/09)
	// add qp_level and GOP infomation
	UINT32						ui8QPLevel	: 8,
								ui8GopN		: 8,	// Length of GOP
								ui8GopM		: 8,	// Length of anchor
								ui8GopIdr	: 8;	// IDR Option(0:Only one IDR, 1: in the every GOP)

	UINT32						nSarWidth;
	UINT32						nSarHeight;
	LX_VENC_SOURCE_TYPE_T		eSourceType;
	UINT32						nFrameRate;
} LX_VENC_RECORD_INFO_T;


typedef enum
{
    LX_VENC_RECORD_FRAME_IMAGE_TYPE_422_PLANAR = 0,  //YUV422, planar
    LX_VENC_RECORD_FRAME_IMAGE_TYPE_422_PACKED,      //YUV422, packed
    LX_VENC_RECORD_FRAME_IMAGE_TYPE_420_PLANAR,      //YUV422, planar
    LX_VENC_RECORD_FRAME_IMAGE_TYPE_420_PACKED,      //YUV422, packed

    LX_VENC_RECORD_FRAME_IMAGE_TYPE_MIN = LX_VENC_RECORD_FRAME_IMAGE_TYPE_422_PLANAR,
    LX_VENC_RECORD_FRAME_IMAGE_TYPE_MAX = LX_VENC_RECORD_FRAME_IMAGE_TYPE_420_PACKED,
}LX_VENC_RECORD_FRAME_IMAGE_TYPE_T;

typedef enum
{
	LX_VENC_IMAGE_YUV420_PLANAR = 0,              /* YYYY... UUUU... VVVV...  */
	LX_VENC_IMAGE_YUV420_SEMIPLANAR,              /* YYYY... UVUVUV...        */
	LX_VENC_IMAGE_YUV420_SEMIPLANAR_VU,           /* YYYY... VUVUVU...        */
	LX_VENC_IMAGE_YUV422_INTERLEAVED_YUYV,        /* YUYVYUYV...              */
	LX_VENC_IMAGE_YUV422_INTERLEAVED_UYVY,        /* UYVYUYVY...              */
} LX_VENC_IMAGE_TYPE_T;

typedef enum
{
	LX_VENC_RECORD_FRAME_TYPE_INVALID = 0,
	LX_VENC_RECORD_FRAME_TYPE_TOP,
	LX_VENC_RECORD_FRAME_TYPE_BOTTOM,
	LX_VENC_RECORD_FRAME_TYPE_PROGRESSIVE,

	LX_VENC_RECORD_FRAME_TYPE_MIN = LX_VENC_RECORD_FRAME_TYPE_INVALID,
	LX_VENC_RECORD_FRAME_TYPE_MAX = LX_VENC_RECORD_FRAME_TYPE_PROGRESSIVE,
} LX_VENC_RECORD_FRAME_TYPE_T;

/**
 Input Frame Image information
*/
typedef struct
{
	LX_VENC_RECORD_FRAME_IMAGE_TYPE_T type;

	UINT32  ui32YPhyAdd; //if Y physical address, else physical address
	UINT32  ui32UPhyAdd; //if planar, U physical address
	UINT32  ui32VPhyAdd; //if planar, V physical address

	UINT32	ui32Size;
	UINT32  ui32Height;
	UINT32  ui32Width;
	UINT32	ui32Stride;
} LX_VENC_RECORD_FRAME_IMAGE_T;

/**
ES/AUI output information.
*/
typedef struct
{
	// ES output
    UINT32			ui32OffsetStart;
    UINT32			ui32OffsetEnd;

	// AUI output
	UINT32			ui32AUIOffsetStart;
	UINT32			ui32AUIOffsetEnd;
} LX_VENC_RECORD_OUTPUT_T;

typedef struct
{
	UINT32	ui32STCBase;
	UINT32	ui32STCExt;
} LX_VENC_RECORD_STC_T;

typedef struct
{
	UINT16	ui16Width;
	UINT16	ui16Height;
	UINT16	ui16Stride;

	UINT32	ui32YBase;
	UINT32	ui32CBase;

	LX_VENC_RECORD_FRAME_TYPE_T eFrameStruct;
} LX_VENC_DE_STATUS_T;

typedef enum
{
	LX_VENC_RC_TYPE_VBR = 0,
	LX_VENC_RC_TYPE_CBR,
	LX_VENC_RC_TYPE_FIXED_QP,
} LX_VENC_RC_TYPE_T;

#if 1
typedef enum
{
	LX_VENC_INTRA_FRAME,		// I-frame/key frame
	LX_VENC_PREDICTED_FRAME,	// P-frame 
} LX_VENC_CODING_TYPE;
#endif

/**
	Setting parameter for Racte control or quality
*/
typedef struct 
{
	LX_VENC_RC_TYPE_T	eRcType;
	UINT32 value;	// if VBR/CBR, the value means target bitrate per one second.
					// otherwise, the value means fixed qp value
} LX_VENC_RECORD_RC_T;

#ifdef SUPPORT_VENC_DEVICE_ENC_API
typedef struct
{
	int		id;
	
	UINT32	width;
	UINT32	height;
	UINT32	framerate;
	UINT32	gopLength;
	UINT32	bitrate;

	LX_VENC_IMAGE_TYPE_T	imageType;
	LX_VENC_ENCODE_TYPE_T	codecType;
	BOOLEAN interlaced;
	BOOLEAN topFieldFirst;

	UINT32	memallocStart;
	UINT32	memallocEnd;
} LX_VENC_ENC_CREATE_T;

typedef struct
{
	int		id;	
} LX_VENC_ENC_DESTROY_T;

typedef struct
{
	int		id;						// [IN]
	UINT32	bus_luma;				// [IN]
	UINT32	bus_chroma_u;			// [IN]
	UINT32	bus_chroma_v;			// [IN]
	
	void *	p_out_buf;				// [IN]
	UINT32	bus_output;				// [IN]
	UINT32	out_buf_size;			// [IN]
	UINT32	timeIncrement;			// [IN]

	LX_VENC_CODING_TYPE	codingType;	// [IN/OUT]
	UINT32	stream_size;			// [OUT]
	UINT32	duration;				// [OUT] for debug
} LX_VENC_ENC_ENCODE_T;
#endif

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VENC_DRV_H_ */

/** @} */
