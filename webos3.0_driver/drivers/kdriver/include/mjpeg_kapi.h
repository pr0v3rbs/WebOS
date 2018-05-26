/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  application interface header for mjpeg device
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.04.09
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

#ifndef	_MJPEG_KAPI_H_
#define	_MJPEG_KAPI_H_

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define LX_MJPEG_MIN_VSIZE			16
#define LX_MJPEG_MIN_HSIZE			16
#define LX_MJPEG_MAX_VSIZE			(32*1024)	// 32k
#define LX_MJPEG_MAX_HSIZE			(32*1024)	// 32k

#define	MJPEG_IOC_MAGIC		'm'
/**
@name MJPEG IOCTL List
ioctl list for MJPEG device.

@{
@def MJPEG_IO_RESET
Reset MJPEG module.
When MJPEG device receives above IOCTL, then reset itself.

@def MJPEG_IOR_CHIP_REV_INFO
Reads chip revision information.
Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.

@def MJPEG_IOW_WRITE_UINT32
write 32bit value to MJPEG module.
detailed...[minimum two lines of comment required. one line of comment does not generated automatically]

@def MJPEG_IOW_WRITE_STRUCT
control complex parameter to MJPEG module.
control codec_type, speed, and additional parameter.
no need to use "see also", exact struct name shall be linked automatically.

*/
/*------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
------------------------------------------------------------------------------*/
#define	LX_MJPEG_IO_SW_RESET				_IO(MJPEG_IOC_MAGIC,  0 )
#define LX_MJPEG_IOR_GET_BUFFER_INFO		_IOR(MJPEG_IOC_MAGIC, 1, LX_MEM_BUF_T )
#define	LX_MJPEG_IOW_ENCODE_INIT			_IOW(MJPEG_IOC_MAGIC, 2, LX_MJPEG_ENC_INIT_PARAM_T )
#define LX_MJPEG_IOW_ENCODE_START			_IOW(MJPEG_IOC_MAGIC, 3, LX_MJPEG_ENC_START_PARAM_T )
#define LX_MJPEG_IOR_ENCODE_STOP			_IOR(MJPEG_IOC_MAGIC, 4, LX_MJPEG_ENC_STOP_PARAM_T )
#define	LX_MJPEG_IOW_DECODE_INIT			_IOW(MJPEG_IOC_MAGIC, 5, LX_MJPEG_DEC_INIT_PARAM_T )
#define LX_MJPEG_IOW_DECODE_START			_IOW(MJPEG_IOC_MAGIC, 6, LX_MJPEG_DEC_START_PARAM_T )
#define LX_MJPEG_IOR_DECODE_STOP			_IOR(MJPEG_IOC_MAGIC, 7, LX_MJPEG_DEC_STOP_PARAM_T )
#define LX_MJPEG_IOR_DECODE_SEQUENCE_INIT	_IOWR(MJPEG_IOC_MAGIC,8, LX_MJPEG_DEC_SEQ_PARAM_T )
#define LX_MJPEG_IOW_DECODE_PARTIAL			_IO(MJPEG_IOC_MAGIC, 9 )

#define LX_MJPEG_IOR_GET_CFG				_IOR(MJPEG_IOC_MAGIC,10, LX_MJPEG_CFG_T )

#define MJPEG_IOC_MAXNR				10
/** @} */

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define MROUND( number, multiple ) ((((number) / (multiple)) + (((number) % (multiple)) ? 1 : 0) ) * (multiple) )

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
/**
 * MJPEG header information.
 * useful when .
 * @see
 */
typedef struct {
	UINT8 ui8SamplingPrecision;
	UINT16 ui16Height;
	UINT16 ui16Width;
	UINT8  ui8NumberOfComponents;
	struct{
		UINT8 ui8ComponentID;
		UINT8 ui8HSampleFreq;
		UINT8 ui8VSampleFreq;
		UINT8 ui8QTableID;
	} stComponents[3];
} LX_MJPEG_FRAME_HEADER_T;

typedef struct {
	UINT8 ui8NumberOfComponents;
	struct{
		UINT8 ui8ComponentID;
		UINT8 ui8DC_HuffmanTableID;
		UINT8 ui8AC_HuffmanTableID;
	} stComponents[3];
	UINT8 ui8Ss;
	UINT8 ui8Se;
	UINT8 ui8Ah;
	UINT8 ui8Al;
} LX_MJPEG_SCAN_HEADER_T;

typedef struct {
	UINT8 ui8Pq;
	UINT8 ui8Tq;
	UINT8 ui8Elements[64];
} LX_MJPEG_QUANTIZATION_TABLE_T;

typedef struct {
	UINT8 ui8Tc;
	UINT8 ui8Th;
	UINT8 ui8CodeLengths[16];
	UINT8 ui8CodeValues[162];
} LX_MJPEG_HUFFMAN_TABLE_T;

typedef struct {
	LX_MJPEG_FRAME_HEADER_T			stFrameHeader;
	LX_MJPEG_SCAN_HEADER_T			stScanHeader;
	UINT16							ui16ResetInterval;
	LX_MJPEG_QUANTIZATION_TABLE_T	stQuantiztionTables[4];
	LX_MJPEG_HUFFMAN_TABLE_T		stHuffmanTables[4];
} LX_MJPEG_HEADER_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see
 */
typedef enum {
	LX_MJPEG_FORMAT_420 		= 0,
	LX_MJPEG_FORMAT_422 		= 1,
	LX_MJPEG_FORMAT_224 		= 2,
	LX_MJPEG_FORMAT_444 		= 3,
	LX_MJPEG_FORMAT_400 		= 4,

	LX_MJPEG_FORMAT_MIN 		= LX_MJPEG_FORMAT_420,
	LX_MJPEG_FORMAT_MAX 		= LX_MJPEG_FORMAT_420
} LX_MJPEG_FORMAT_T;


/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see
 */
typedef struct {
	UINT32		ui32AddressY;
	UINT32		ui32AddressCb;
	UINT32		ui32AddressCr;
	UINT32		ui32StrideY;
	UINT32		ui32StrideC;

	BOOLEAN		bUVInterleaved;
} LX_MJPEG_IMAGE_BUF_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see
 */
typedef struct {
	LX_MJPEG_FORMAT_T			eFormat;
	UINT16						ui16Height;
	UINT16						ui16Width;
} LX_MJPEG_IMAGE_INFO_T;

/**
 * MJPEG operation mode enumeration.
 *
 * @see
 */
typedef enum {
	LX_MJPEG_OPERATION_NORMAL	= 0,
	LX_MJPEG_OPERATION_PARTIAL	= 1,
} LX_MJPEG_OPERATION_T;

/**
 * MJPEG rotation degree enumeration.
 *
 * @see
 */
typedef enum {
	LX_MJPEG_ROTATE_0			= 0,
	LX_MJPEG_ROTATE_90			= 1,
	LX_MJPEG_ROTATE_180			= 2,
	LX_MJPEG_ROTATE_270			= 3
} LX_MJPEG_ROTATION_T;

/**
 * MJPEG mirroring enumeration.
 *
 * @see
 */
typedef enum {
	LX_MJPEG_MIRROR_NONE		= 0x00,
	LX_MJPEG_MIRROR_HORIZONTAL	= 0x01,
	LX_MJPEG_MIRROR_VERTICAL	= 0x02
} LX_MJPEG_MIRRORING_T;

/**
 * MJPEG scale enumeration.
 *
 * @see
 */
typedef enum {
	LX_MJPEG_SCALE_NONE			= 0,
	LX_MJPEG_SCALE_1_2			= 1,
	LX_MJPEG_SCALE_1_4			= 2,
	LX_MJPEG_SCALE_1_8			= 3
} LX_MJPEG_SCALING_T;

/**
 * MJPEG encoding quality enumeration.
 *
 * @see
 */
typedef enum {
	LX_MJPEG_QUALITY_LOW		= 0,
	LX_MJPEG_QUALITY_MID_LOW	= 1,
	LX_MJPEG_QUALITY_MID_HIGH	= 2,
	LX_MJPEG_QUALITY_HIGH		= 3,
	LX_MJPEG_QUALITY_USE_VALUE	= 4,	// using quality value
} LX_MJPEG_QUALITY_T;

/**
 * MJPEG option parameter.
 *
 * @see
 */
typedef struct {
	LX_MJPEG_ROTATION_T		eRotation;			
	LX_MJPEG_MIRRORING_T	eMirroring;
	LX_MJPEG_SCALING_T		eScaleHorizontal;	// Deocder only
	LX_MJPEG_SCALING_T		eScaleVertical;		// Decoder only
	LX_MJPEG_OPERATION_T	eOperation;			// Decoder only
	BOOLEAN					bQuality;			// Encoder only 
	UINT32					nQuality;			// Encoder only 0-100 (0:low quality, 100:hight quality)
} LX_MJPEG_OPTION_T;

#if 0
/**
 * MJPEG option parameter.
 *
 * @see
 */
typedef struct {
	LX_MJPEG_ROTATION_T		eRotation;
	LX_MJPEG_MIRRORING_T	eMirroring;
	LX_MJPEG_SCALING_T		eScaleHorizontal;
	LX_MJPEG_SCALING_T		eScaleVertical;
	LX_MJPEG_OPERATION_T	eOperation;
} LX_MJPEG_DECODE_OPTION_T;

/**
 * MJPEG option parameter.
 *
 * @see
 */
typedef struct {
	LX_MJPEG_SCALING_T		eScaleHorizontal;
	LX_MJPEG_SCALING_T		eScaleVertical;
	LX_MJPEG_QUALITY_T		eQuality;			// 0-4	(4: use nQuality, 3:high 0:low)
	UINT32					nQuality;			// 0-100 (0:low quality, 100:hight quality)
} LX_MJPEG_ENCODE_OPTION_T;
#endif

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOW_ENCODE_INIT
 */
typedef struct
{
	LX_MJPEG_IMAGE_INFO_T		stImageInfo;		/** information of image */
	LX_MJPEG_IMAGE_BUF_T		stImageBuf;			/** information of data buffer of image */
	LX_MJPEG_OPTION_T			stOption;
} LX_MJPEG_ENC_INIT_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOR_ENCODE_START
 */
typedef struct
{
	LX_MEM_BUF_T		stDataBuffer;				/** information of data buffer */
} LX_MJPEG_ENC_START_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOR_ENCODE_STATUS
 */
typedef struct
{
	LX_MJPEG_HEADER_T	stHeader;					/** jpeg header information */
	UINT32				ui32Length;					/** length written in kernel driver */
} LX_MJPEG_ENC_STOP_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOW_DECODE_INIT
 */
typedef struct
{
	LX_MJPEG_HEADER_T			stHeader;			/** jpeg header information */
	LX_MJPEG_IMAGE_BUF_T		stImageBuf;			/** output buffer to hold decoded data */
	LX_MJPEG_OPTION_T			stOption;
} LX_MJPEG_DEC_INIT_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOR_DECODE_START
 */
typedef struct
{
	LX_MEM_BUF_T	stDataBuffer;					/** information of data buffer */
	UINT32			ui32DataLength;					/** length of data */
	BOOLEAN			bEndOfStream;					/** end of stream */
	BOOLEAN			bSkipCheck;
} LX_MJPEG_DEC_START_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOR_DECODE_STATUS
 */
typedef struct
{
	LX_MJPEG_IMAGE_INFO_T	stImageInfo;			/** information of decoded data */
} LX_MJPEG_DEC_STOP_PARAM_T;

/**
 * MJPEG ioctl parameter.
 * useful when complex parameter should be passed to MJPEG kernel driver.
 * @see MJPEG_IOR_DECODE_START
 */
typedef struct
{
	LX_MEM_BUF_T		stDataBuffer;				/** information of data buffer */

	LX_MJPEG_HEADER_T	stHeader;					/** jpeg header information */
	UINT32				ui32Length;					/** decoded header length in kernel driver */
} LX_MJPEG_DEC_SEQ_PARAM_T;

/** MJPEG cfg
 *
 *
 */
typedef struct
{
	UINT16			max_decode_width;			/** maximum decodable width (default: 8k) */
	UINT16			max_decode_height;			/** maximum decodable width (default: 8k) */

#define	LX_MJPEG_DECODE_CAPS_NONE				0x00000000	/** none */
#define	LX_MJPEG_DECODE_CAPS_PROGRESSIVE		0x1	<< 1	/** if progressive jpeg can be decoded ? */
#define LX_MJPEG_DECODE_CAPS_PARTIAL			0x1 << 2	/** if partial mode can be used? */
	UINT32			decode_caps;

	UINT32			resvd[6];
} LX_MJPEG_CFG_T;

typedef enum
{
	LX_MJPEG_RESULT_DONE		= 0x1 << 1,	// 1
	LX_MJPEG_RESULT_ERROR		= 0x1 << 2,	// 2
	LX_MJPEG_RESULT_BBC		= 0x1 << 3,	// 4
	LX_MJPEG_RESULT_OVER		= 0x1 << 4,	// 8
	LX_MJPEG_RESULT_BUFFER0	= 0x1 << 5, // 16
	LX_MJPEG_RESULT_BUFFER1	= 0x1 << 6,
	LX_MJPEG_RESULT_BUFFER2	= 0x1 << 7,
	LX_MJPEG_RESULT_BUFFER3	= 0x1 << 8,

	LX_MJPEG_RESULT_BUFFER0_AND_BBC = 0x1 << 9,
} LX_MJPEG_RESULT_T;

/*------------------------------------------------------------------------------
	Extern Function Prototype Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Variables
------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MJPEG_KAPI_H_ */

/** @} */
