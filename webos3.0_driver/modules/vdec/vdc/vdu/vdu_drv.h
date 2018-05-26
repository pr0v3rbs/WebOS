/* *****************************************************************************
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * ****************************************************************************/

/** @file
 *
 * video decoding unit driver header for vdec device.
 * VDEC device will teach you how to make device driver with lg1154 platform.
 *
 * author     Youngwoo Jin(youngwoo.jin@lge.com)
 * version    1.0
 * date       2013.01.06
 * note       Additional information.
 *
 */

#ifndef _VDU_DRV_H_
#define _VDU_DRV_H_

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "include/vdec_kapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define VDU_MAX_NUM_OF_INSTANCES	(3)
#define VDU_INVALID_HANDLE			(0xFFFFFFFF)
#define VDU_INVALID_ADDRESS			(0x00000000)
#define VDU_INVALID_FRAME_INDEX		(-1)
#define VDU_FRAME_INDEX_SKIPPED		(-10)
#define VDU_OPTION_INIT_VALUE		{	.ePicScanMode = VDU_PIC_SCAN_INVALID,	\
										.eUserData	  = VDU_USERDATA_INVALID }

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
typedef UINT32	VDU_HANDLE_T;

typedef enum {
	VDU_RET_OK					= 0,
	VDU_RET_INVALID_CH			= 1,
	VDU_RET_INVALID_PARAM		= 2,
	VDU_RET_DPB_FULL			= 3,
	VDU_RET_SEQ_INIT_FAIL		= 4,
	VDU_RET_BUSY				= 5,
	VDU_RET_ERROR				= 6,

	VDU_RET_MIN					= VDU_RET_OK,
	VDU_RET_MAX					= VDU_RET_ERROR,
	VDU_RET_INVALID
} VDU_RET_T;

typedef enum {
	VDU_STATUS_NULL				= 0,
	VDU_STATUS_READY			= 1,
	VDU_STATUS_BUSY				= 2,

	VDU_STATUS_MIN				= VDU_STATUS_NULL,
	VDU_STATUS_MAX				= VDU_STATUS_BUSY,
	VDU_STATUS_INVALID
} VDU_STATUS_T;

typedef enum {
	VDU_NOTI_SUCCESS			=  0,
	VDU_NOTI_DISPLAY			=  1,
	VDU_NOTI_DECODING_ERROR		=  2,
	VDU_NOTI_CPB_EMPTY			=  3,
	VDU_NOTI_HEADER_NOT_FOUND	=  4,
	VDU_NOTI_NOT_SUPPORTED		=  5,
	VDU_NOTI_LACK_OF_RESOURCES	=  6,
	VDU_NOTI_DPB_FULL			=  7,
	VDU_NOTI_DPB_EMPTY			=  8,
	VDU_NOTI_SUSPEND			=  9,
	VDU_NOTI_FRAME_SKIP			= 10,
	VDU_NOTI_SKIP_EMPTY			= 11,
	VDU_NOTI_REQUEST_AU			= 12,
	VDU_NOTI_END_OF_STREAM		= 13,

	VDU_NOTI_MIN				= VDU_NOTI_SUCCESS,
	VDU_NOTI_MAX				= VDU_NOTI_END_OF_STREAM,
	VDU_NOTI_INVALID
} VDU_NOTI_T;

typedef enum {
	VDU_FRM_SCAN_INVALID		= 0,
	VDU_FRM_SCAN_TOP_FIRST		= 1,
	VDU_FRM_SCAN_BOTTOM_FIRST	= 2,
	VDU_FRM_SCAN_PROGRESSIVE	= 3,
	VDU_FRM_SCAN_TOP_ONLY		= 4,
	VDU_FRM_SCAN_BOTTOM_ONLY	= 5,

	VDU_FRM_SCAN_MIN			= VDU_FRM_SCAN_TOP_FIRST,
	VDU_FRM_SCAN_MAX			= VDU_FRM_SCAN_BOTTOM_ONLY,
} VDU_FRM_SCAN_T;

typedef enum {
	VDU_FRM_3D_LEFT				= 0,
	VDU_FRM_3D_RIGHT			= 1,
	VDU_FRM_3D_NONE				= 2,

	VDU_FRM_3D_MIN				= VDU_FRM_3D_LEFT,
	VDU_FRM_3D_MAX				= VDU_FRM_3D_NONE,
	VDU_FRM_3D_INVALID
} VDU_FRM_3D_T;

typedef enum {
	VDU_CODEC_MPEG2				=  0,
	VDU_CODEC_MPEG4				=  1,
	VDU_CODEC_H263				=  2,
	VDU_CODEC_SORENSON_SPARK	=  3,
	VDU_CODEC_XVID				=  4,
	VDU_CODEC_DIVX3				=  5,
	VDU_CODEC_DIVX4				=  6,
	VDU_CODEC_DIVX5				=  7,
	VDU_CODEC_H264_AVC			=  8,
	VDU_CODEC_H264_MVC			=  9,
	VDU_CODEC_VC1_RCV_V1		= 10,
	VDU_CODEC_VC1_RCV_V2		= 11,
	VDU_CODEC_VC1_ES			= 12,
	VDU_CODEC_RVX				= 13,
	VDU_CODEC_AVS				= 14,
	VDU_CODEC_THEORA			= 15,
	VDU_CODEC_VP3				= 16,
	VDU_CODEC_VP8				= 17,
	VDU_CODEC_HEVC				= 18,
	VDU_CODEC_VP9				= 19,

	VDU_CODEC_MIN				= VDU_CODEC_MPEG2,
	VDU_CODEC_MAX				= VDU_CODEC_VP9,
	VDU_CODEC_INVALID
} VDU_CODEC_T;

typedef enum {
	VDU_PICTURE_I				= 0,
	VDU_PICTURE_P				= 1,
	VDU_PICTURE_B				= 2,

	VDU_PICTURE_MIN				= VDU_PICTURE_I,
	VDU_PICTURE_MAX				= VDU_PICTURE_B,
	VDU_PICTURE_INVALID
} VDU_PICTURE_T;

typedef enum {
	VDU_PIC_SCAN_ALL			= 0,
	VDU_PIC_SCAN_I				= 1,
	VDU_PIC_SCAN_IP				= 2,
	VDU_PIC_SCAN_I_FIELD		= 3,

	VDU_PIC_SCAN_MIN			= VDU_PIC_SCAN_ALL,
	VDU_PIC_SCAN_MAX			= VDU_PIC_SCAN_I_FIELD,
	VDU_PIC_SCAN_INVALID
} VDU_PIC_SCAN_T;

typedef enum {
	VDU_USERDATA_DISABLE		= 0,
	VDU_USERDATA_ENABLE			= 1,

	VDU_USERDATA_MIN			= VDU_USERDATA_DISABLE,
	VDU_USERDATA_MAX			= VDU_USERDATA_ENABLE,
	VDU_USERDATA_INVALID
} VDU_USERDATA_T;

typedef enum {
	VDU_LINEAR_MODE_OFF			= 0,
	VDU_LINEAR_MODE_NONE		= 1,
	VDU_LINEAR_MODE_STATIC		= 2,
	VDU_LINEAR_MODE_DYNAMIC		= 3,

	VDU_LINEAR_MODE_MIN			= VDU_LINEAR_MODE_OFF,
	VDU_LINEAR_MODE_MAX			= VDU_LINEAR_MODE_DYNAMIC,
	VDU_LINEAR_MODE_INVALID
} VDU_LINEAR_MODE_T;

typedef enum {
    VDU_DEC_BODA_OPEN           = 0,
    VDU_DEC_G1_OPEN             = 1,
    VDU_DEC_G2_OPEN             = 2,
    VDU_DEC_HEVC_OPEN           = 3,

    VDU_DEC_CORE_OPEN_MIN       = VDU_DEC_BODA_OPEN,
    VDU_DEC_CORE_OPEN_MAX       = VDU_DEC_HEVC_OPEN,
    VDU_DEC_CORE_OPEN_INVALID
} VDU_DEC_CORE_OPEN_T;

typedef struct {
	VDU_CODEC_T				eCodecType;
	BOOLEAN					bNoDelayMode;
	BOOLEAN					bOneFrameDecoding;
	VDU_LINEAR_MODE_T		eExternLinearBufMode;
	UINT16					ui16ValidThreshold;
	UINT32					ui32CPBAddr;
	UINT32					ui32CPBSize;
	BOOLEAN					bForceUhdEnable;
    VDU_DEC_CORE_OPEN_T     eOpenDecoder;
} VDU_OPEN_PARAM_T;

typedef struct {
	UINT32					ui32StartAddr;
	UINT32					ui32Size;
	BOOLEAN					bEndOfStream;
} VDU_AU_T;

typedef struct {
	VDU_PIC_SCAN_T			ePicScanMode;
	VDU_USERDATA_T			eUserData;
} VDU_OPTION_T;

typedef struct {
	UINT8					ui8NumberOfFrames;
	UINT32					ui32Stride;
	UINT32					ui32Height;
	UINT32*					aui32AddressList;
} VDU_LINEAR_FRAMES_T;

typedef struct {
	struct {
		UINT32					ui32Stride;
		UINT32					ui32PicWidth;
		UINT32					ui32PicHeight;
		UINT16					ui16CropLeft;
		UINT16					ui16CropRight;
		UINT16					ui16CropTop;
		UINT16					ui16CropBottom;
	}						stRect;

	struct {
		UINT16					ui16ParW;
		UINT16					ui16ParH;
		UINT8					ui8Mpeg2Dar;
	}						stAspectRatio;

	struct {
		UINT32					ui32TiledBase;
		UINT32					ui32Y;
		UINT32					ui32Cb;
		UINT32					ui32Cr;
	}						stAddress;

	VDEC_MAP_INFO_T*		pstMapInfo;

	struct {
		UINT32					ui32Residual;
		UINT32					ui32Divider;
		BOOLEAN					bVariable;
	}						stFrameRate;

	struct {
		VDU_FRM_SCAN_T			eFrameScanType;
		BOOLEAN					bTopFieldFirst;
		BOOLEAN					bRepeatFirstField;
		UINT8					ui8DisplayPeriod;
		BOOLEAN					bLowDelay;
		SINT8					si8FramePackArrange;	// 3D SEI
		VDU_FRM_3D_T			eOrderOf3D;
		UINT8					ui8ErrorRate;
		UINT8					ui8PictureType;
		BOOLEAN					bFieldPicture;
		BOOLEAN					bNonPairedField;
		SINT32					si32Mp2DispWidth;
		SINT32					si32Mp2DispHeight;
	}						stDispInfo;

	struct {
		UINT8					ui8ActiveFmtDesc;
		UINT32					ui32Address;
		UINT32					ui32Size;
	} 						stUserData;

	UINT8					ui8NumOfFrames;
} VDU_FRM_INFO_T;

typedef struct {
	VDU_NOTI_T				eNotiType;
	SINT8					si8FrmIndex;
	SINT8					si8DecodedIndex;
	BOOLEAN					bDecSuccess;
	BOOLEAN					bFieldSuccess;
	UINT32					ui32ReadPtr;
	VDU_FRM_INFO_T			stFrameInfo;
} VDU_RESULT_T;

typedef void (*VDU_CALLBACK_FN_T)(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult);

/*------------------------------------------------------------------------------
	Extern Function Prototype Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Generic Usage Functions
------------------------------------------------------------------------------*/
VDU_RET_T		VDU_Init(VDU_CALLBACK_FN_T pfnCallback);
VDU_RET_T		VDU_Suspend(void);
VDU_RET_T		VDU_Resume(void);
VDU_RET_T		VDU_Open(VDU_HANDLE_T *phInstance, VDU_OPEN_PARAM_T *pstOpenParam);
VDU_RET_T		VDU_Close(VDU_HANDLE_T hInstance);
VDU_RET_T		VDU_Flush(VDU_HANDLE_T hInstance);
VDU_RET_T		VDU_Reset(VDU_HANDLE_T hInstance);
VDU_RET_T		VDU_StartDecoding(VDU_HANDLE_T hInstance, VDU_LINEAR_FRAMES_T* pstLinearFrames);
VDU_RET_T		VDU_FeedAu(VDU_HANDLE_T hInstance, VDU_AU_T* pstAu);
VDU_RET_T		VDU_SetOption(VDU_HANDLE_T hInstance, VDU_OPTION_T* pstOption);
VDU_RET_T		VDU_ClearFrame(VDU_HANDLE_T hInstance, SINT8 si8FrameIdx);
VDU_RET_T		VDU_GetStatus(VDU_HANDLE_T hInstance, VDU_STATUS_T *peStatus);
VDU_RET_T		VDU_GetLinearFrame(VDU_HANDLE_T hInstance, SINT8 si8FrameIdx, void* pFrameBuf);

#ifdef __cplusplus
}
#endif

#endif /* _VDU_DRV_H_ */

