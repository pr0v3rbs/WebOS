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
 * video decoding unit driver implementation for VDEC device.
 * VDEC device will teach you how to make device driver with lg1154 platform.
 *
 * @author     Youngwoo Jin(youngwoo.jin@lge.com)
 * @version    1.0
 * @date       2013.01.06
 * @note       Additional information.
 *
 * @addtogroup lg1154_vdec
 * @{
 */

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	File Inclusions
------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/delay.h>

#include "vdu_drv.h"
#include "vdu_dec.h"
#include "vdu_debug.h"
#include "../../log.h"
#include "../../hal/vdec_base.h"
#include "../../hal/vdc_hal_api.h"

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define NUMBER_OF_SEARCHING_I				(30)
#define WAIT_TIME_OUT_VALUE					(500)
#define USERDATA_BUFFER_SIZE				(0x0400 * (VDU_DEC_NUM_OF_MAX_DPB))
#define MAX_NUM_OF_CORES					(3)
#define CORE_NAME_CNM						"cnm"
#define CORE_NAME_HEVC						"hevc"
#define CORE_NAME_GX						"gx"

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define _MemAlloc(_size, _align, _name)		({									\
	vdec_alloc((_size), (_align), (_name));										\
})
#define _MemFree(_addr)						({									\
	if( (_addr) != VDU_INVALID_ADDRESS ) {										\
		vdec_free((_addr));														\
		(_addr) = VDU_INVALID_ADDRESS;											\
	}																			\
})
#define _MemMap(_addr, _size)				({									\
	(UINT32)vdec_remap((_addr), (_size));										\
})
#define _MemUnmap(_addr)					({									\
	if( (_addr) != VDU_INVALID_ADDRESS ) {										\
		vdec_unmap((void*)(_addr));												\
		(_addr) = VDU_INVALID_ADDRESS;											\
	}																			\
})
#define _Malloc(_size)						({									\
	kmalloc((_size), (in_interrupt())? GFP_ATOMIC : GFP_KERNEL);				\
})
#define _MFree(_addr)						({									\
	if( (_addr) != NULL ) {														\
		kfree(_addr);															\
		(_addr) = NULL;															\
	}																			\
})
#define _LockInstance(_inst, _flags)		({									\
	spin_lock_irqsave(&((_inst)->stLock), *(_flags));							\
})
#define _UnlockInstance(_inst, _flags)		({									\
	spin_unlock_irqrestore(&((_inst)->stLock), *(_flags));						\
})

/*------------------------------------------------------------------------------
	Type Definitions
------------------------------------------------------------------------------*/
typedef struct tasklet_struct		TASKLET_T;
typedef struct work_struct          WORK_T;
typedef struct workqueue_struct     WORKQUEUE_T;

typedef struct
{
	BOOLEAN				bTopFieldFirst;
	BOOLEAN				bRepeatFirstField;
	VDU_FRM_SCAN_T		eFrameScanType;
} AVC_PIC_STRUCT_T;

typedef enum
{
	CORE_NUM_CNM			= 0,
	CORE_NUM_HEVC			= 1,
	CORE_NUM_GX				= 2,

	CORE_NUM_MIN			= CORE_NUM_CNM,
	CORE_NUM_MAX			= CORE_NUM_GX,
	CORE_NUM_INVALID
} CORE_NUM_T;

typedef struct
{
	const char*				pchName;
	VDU_DEC_EXECUTE_FN_T	pfnExecute;
} CORE_INFO_T;

typedef enum
{
	CALLBACK_STATE_IDLE		= 0,
	CALLBACK_STATE_RUNNING	= 1,
	CALLBACK_STATE_PENDING	= 2,

	CALLBACK_STATE_MIN		= CALLBACK_STATE_IDLE,
	CALLBACK_STATE_MAX		= CALLBACK_STATE_PENDING,
	CALLBACK_STATE_INVALID
} CALLBACK_STATE_T;

typedef struct CALLBACK_UNIT_T {
	VDU_DEC_RESULT_T		stDecResult;
	struct CALLBACK_UNIT_T*	pstNext;
} CALLBACK_UNIT_T;

typedef struct
{
	spinlock_t				stLock;

	BOOLEAN					bInUsed;
	BOOLEAN					bRunning;
	BOOLEAN					bFlushing;

	VDU_PIC_SCAN_T			ePicScanMode;
	BOOLEAN					bEnableUserData;

	struct {
		UINT32					ui32ResetAddr;
		UINT32					ui32WriteAddr;
		BOOLEAN					ui32StreamEndAddr;
	}						stFeedInfo;

	struct {
		UINT16					ui16NumberOfSearch;
		SINT8					si8FirstFrameIndex;
		UINT32					ui32GarbageFramesBit;
		UINT16					ui16ValidThreshold;
	}						stValidFrameInfo;

	struct {
		SINT32					si32FramePackArrange;
		UINT8					ui8PicTimingStruct;
	}						stPreDisplayInfo;

	struct {
		UINT8					ui8NumberOfBuffers;
		UINT32					ui32Stride;
		UINT32					ui32Height;
		UINT32					aui32AddressList[VDU_DEC_NUM_OF_MAX_DPB];
	}						stExternBuffer;

	VDU_RESULT_T			stNotiInfo;

	VDU_DEC_INSTANCE_T*		pstDecInfo;
	VDU_DEC_EXECUTE_FN_T	pfnExecute;

	CORE_INFO_T*			pstCore;
} INSTANCE_T;

/*------------------------------------------------------------------------------
	External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Static Function Prototypes Declarations
------------------------------------------------------------------------------*/
static INSTANCE_T*	_GetInstance(VDU_HANDLE_T hInstance, ULONG* pulFlags);
static INSTANCE_T*	_WaitDone(VDU_HANDLE_T hInstance, ULONG* pulFlags);

// Checking invalid frames functions
static BOOLEAN		_InitCheckingInvalidFrame(INSTANCE_T* pstInstance, UINT16 ui16Threshold);
static BOOLEAN		_CheckInvalidFrame(INSTANCE_T* pstInstance, VDU_DEC_RESULT_T* pstDecResult, ULONG* pulFlags);
static BOOLEAN		_DidCheckValidIFrame(INSTANCE_T* pstInstance);
static BOOLEAN		_DidFinishCheckInvalidFrame(INSTANCE_T* pstInstance);
static void			_FinishCheckInvalidFrame(INSTANCE_T* pstInstance);

// Callback functions
static void			_Callback(VDU_DEC_INSTANCE_T* pstDecInfo, VDU_DEC_RESULT_T* pstDecResult);

// Frame informaiton set functions
static BOOLEAN		_SetDisplayFrameInfo(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											INSTANCE_T* pstInstance);
static BOOLEAN		_SetSizeInfo(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType,
									BOOLEAN bInterlacedSequence);
static BOOLEAN		_SetAspectRatio(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType);
static BOOLEAN		_SetFrameRate(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType);
static BOOLEAN		_SetScanTypeAndPeriod(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											VDU_CODEC_T eCodecType,
											BOOLEAN bInterlacedSequence,
											UINT8* pui8PrePicTimingStruct);
static BOOLEAN		_SetFramePackArrange(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											VDU_CODEC_T eCodecType,
													SINT32* psi32PreFramePackArrange);
static BOOLEAN		_SetActiveFormatDescription(VDU_FRM_INFO_T* pstFrameInfo,
												VDU_DEC_FRAME_INFO_T* pstPictureInfo,
												VDU_CODEC_T eCodecType);

// Utilities functions
static UINT8		_GetDisplayAspectRatio(UINT32 ui32DisplayWidth, UINT32 ui32DisplayHeight);
static UINT32		_GetGcd(UINT32 ui32Bigger, UINT32 ui32Smaller);

/*------------------------------------------------------------------------------
	global Variables
------------------------------------------------------------------------------*/
logm_define(vdec_vdu, log_level_warning);
logm_define(vdec_gxref, log_level_warning);

/*------------------------------------------------------------------------------
	Static Variables
------------------------------------------------------------------------------*/
static VDU_CALLBACK_FN_T	_gpfnCallback = NULL;
static CORE_INFO_T			_gastCoreInfo[MAX_NUM_OF_CORES] = {
	{ .pchName = CORE_NAME_CNM, .pfnExecute = NULL },
	{ .pchName = CORE_NAME_HEVC, .pfnExecute = NULL },
	{ .pchName = CORE_NAME_GX, .pfnExecute = NULL }	};
static INSTANCE_T			_gastInstancePool[VDU_MAX_NUM_OF_INSTANCES];
static VDU_DEC_BUFFER_T		_gastUserdataBufferPool[VDU_MAX_NUM_OF_INSTANCES];

static int					_giForceLinear = 0;
static int					vdu_test_gx=0;
static int					vdu_test_gx_mpeg2=0;

module_param_named(vdu_force_linear, _giForceLinear, int, 0644);
module_param(vdu_test_gx, int, 0644);
module_param(vdu_test_gx_mpeg2, int, 0644);

/*==============================================================================
    Implementation Group
==============================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T 		VDU_Init(VDU_CALLBACK_FN_T pfnCallback)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	UINT8			ui8Index = 0;

	VDC_HAL_Init();

	_gpfnCallback = pfnCallback;

	for ( ui8Index = 0; ui8Index < VDU_MAX_NUM_OF_INSTANCES; ++ui8Index ) {
		spin_lock_init(&_gastInstancePool[ui8Index].stLock);
		_gastInstancePool[ui8Index].bInUsed = FALSE;
		_gastInstancePool[ui8Index].bRunning = FALSE;
		_gastInstancePool[ui8Index].pfnExecute = NULL;
		_gastInstancePool[ui8Index].pstCore = NULL;

		_gastUserdataBufferPool[ui8Index].ui32PhysAddr = (UINT32)_MemAlloc(USERDATA_BUFFER_SIZE, 1 << 12, "vpu_userdata");
		if ( _gastUserdataBufferPool[ui8Index].ui32PhysAddr == 0x00 ) {
			goto GOTO_END;
		}

		_gastUserdataBufferPool[ui8Index].ui32VirtAddr = (UINT32)_MemMap(_gastUserdataBufferPool[ui8Index].ui32PhysAddr, USERDATA_BUFFER_SIZE);
		if ( _gastUserdataBufferPool[ui8Index].ui32VirtAddr == 0x00 ) {
			goto GOTO_END;
		}

		_gastUserdataBufferPool[ui8Index].ui32Size = USERDATA_BUFFER_SIZE;
	}

	VDU_DBG_Init();

	eRetVal = VDU_RET_OK;

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Suspend(void)
{
	VDU_RET_T		eRetVal = VDU_RET_OK;
	UINT8			ui8InstNum = 0;
	UINT8			ui8Core = 0;

	for ( ui8InstNum = 0; ui8InstNum < VDU_MAX_NUM_OF_INSTANCES; ++ui8InstNum ) {
		VDU_HANDLE_T	hInstance = (VDU_HANDLE_T)ui8InstNum;
		INSTANCE_T* 	pstInstance = NULL;
		ULONG			ulFlags = 0x00;

		pstInstance = _WaitDone(hInstance, &ulFlags);
		if ( pstInstance == NULL ) {
			pstInstance = _GetInstance(hInstance, &ulFlags);
			if ( pstInstance == NULL ) {
				continue;
			} else {
				logm_error(vdec_vdu, "[%d]Fail to wait\n", (UINT32)hInstance);
			}
		}
		pstInstance->bFlushing = TRUE;
		_UnlockInstance(pstInstance, &ulFlags);
	}

	for ( ui8Core = 0; ui8Core < MAX_NUM_OF_CORES; ++ui8Core ) {
		if ( _gastCoreInfo[ui8Core].pfnExecute != NULL ) {
			eRetVal = _gastCoreInfo[ui8Core].pfnExecute(VDU_DEC_CMD_SUSPEND, NULL);
		} else {
			logm_error(vdec_vdu, "There is no function registered for core %d\n", ui8Core);
			continue;
		}
	}

    VDC_HAL_Suspend();

	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Resume(void)
{
	VDU_RET_T		eRetVal = VDU_RET_OK;
	UINT8			ui8InstNum = 0;
	UINT8			ui8Core = 0;

    VDC_HAL_Resume();

	for ( ui8Core = 0; ui8Core < MAX_NUM_OF_CORES; ++ui8Core ) {
		logm_info (vdec_vdu, "resume %s...\n", _gastCoreInfo[ui8Core].pchName);

		if ( _gastCoreInfo[ui8Core].pfnExecute != NULL ) {
			eRetVal = _gastCoreInfo[ui8Core].pfnExecute(VDU_DEC_CMD_RESUME, NULL);
		} else {
			logm_error(vdec_vdu, "There is no function registered for core %d\n", ui8Core);
			continue;
		}
	}


	for ( ui8InstNum = 0; ui8InstNum < VDU_MAX_NUM_OF_INSTANCES; ++ui8InstNum ) {
		INSTANCE_T* 	pstInstance = &_gastInstancePool[ui8InstNum];
		ULONG			ulFlags = 0x00;

		_LockInstance(pstInstance, &ulFlags);
		if ( pstInstance->bInUsed == TRUE ) {
			pstInstance->bFlushing = FALSE;
		}
		_UnlockInstance(pstInstance, &ulFlags);
	}
	logm_info (vdec_vdu, "resume done.\n");

	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Open(VDU_HANDLE_T *phInstance, VDU_OPEN_PARAM_T *pstOpenParam)
{
	VDU_RET_T			eRetVal = VDU_RET_ERROR;
	INSTANCE_T*			pstInstance = NULL;
	VDU_DEC_INSTANCE_T*	pstDecInfo = NULL;
	VDU_DEC_PARAM_T		stExeParam = { 0, };
	CHAR*				pchCoreName = NULL;
	UINT8				ui8Index = 0;
	ULONG				ulFlags = 0x00;

	if ( _giForceLinear != 0 ) {
		pstOpenParam->eExternLinearBufMode = VDU_LINEAR_MODE_NONE;
	}

	for ( *phInstance = 0; *phInstance < VDU_MAX_NUM_OF_INSTANCES; ++*phInstance ) {
		_LockInstance(&_gastInstancePool[(UINT32)*phInstance], &ulFlags);
		if ( _gastInstancePool[(UINT32)*phInstance].bInUsed == FALSE ) {
			pstInstance = &_gastInstancePool[(UINT32)*phInstance];
			break;
		}
		_UnlockInstance(&_gastInstancePool[(UINT32)*phInstance], &ulFlags);
	}

	if ( pstInstance == NULL ) {
		logm_warning (vdec_vdu, "Cannot open, all instances are occupied \n");
		goto GOTO_END;
	}

	VDU_DBG_Open(*phInstance, pstOpenParam);

	pstDecInfo = (VDU_DEC_INSTANCE_T*)_Malloc(sizeof(VDU_DEC_INSTANCE_T));
	if ( pstDecInfo == NULL ) {
		goto GOTO_ERROR;
	}

	pstInstance->bInUsed = TRUE;
	pstInstance->bRunning = FALSE;
	pstInstance->bFlushing = FALSE;
	pstInstance->ePicScanMode = (pstOpenParam->bOneFrameDecoding == TRUE)?
									VDU_PIC_SCAN_I : VDU_PIC_SCAN_ALL;
	pstInstance->bEnableUserData = FALSE;
	pstInstance->stFeedInfo.ui32ResetAddr = pstOpenParam->ui32CPBAddr;
	pstInstance->stFeedInfo.ui32WriteAddr = pstOpenParam->ui32CPBAddr;
	pstInstance->stFeedInfo.ui32StreamEndAddr = VDU_INVALID_ADDRESS;
	pstInstance->stPreDisplayInfo.si32FramePackArrange = -1;
	pstInstance->stPreDisplayInfo.ui8PicTimingStruct = 9;
	pstInstance->stExternBuffer.ui8NumberOfBuffers = 0;
	pstInstance->stExternBuffer.ui32Stride = 0;
	pstInstance->stExternBuffer.ui32Height = 0;

	pstInstance->pstDecInfo = pstDecInfo;
	pstInstance->pstDecInfo->hVduHandle = *phInstance;
	pstInstance->pstDecInfo->eCodecType = pstOpenParam->eCodecType;
	pstInstance->pstDecInfo->ui32CpbBaseAddr = pstOpenParam->ui32CPBAddr;
	pstInstance->pstDecInfo->ui32CpbEndAddr = pstOpenParam->ui32CPBAddr + pstOpenParam->ui32CPBSize;
	pstInstance->pstDecInfo->pstUserdataBuf = &_gastUserdataBufferPool[*phInstance];
	pstInstance->pstDecInfo->bForceUhdEnabled = pstOpenParam->bForceUhdEnable;
	pstInstance->pstDecInfo->bNoDelayMode = pstOpenParam->bNoDelayMode;
	pstInstance->pstDecInfo->bOneFrameDecoding = pstOpenParam->bOneFrameDecoding;
	pstInstance->pstDecInfo->eExternLinearBufMode = pstOpenParam->eExternLinearBufMode;
	pstInstance->pstDecInfo->pPrivateData = NULL;

	_InitCheckingInvalidFrame(pstInstance, pstOpenParam->ui16ValidThreshold);

	switch ( pstOpenParam->eCodecType ) {
	case VDU_CODEC_MPEG2:
		if ( vdu_test_gx_mpeg2 ) {
			pchCoreName = CORE_NAME_GX;
			break;
		}							// FALL-THROUGH
	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:
		if ( pstOpenParam->eOpenDecoder == VDU_DEC_G1_OPEN ||
             vdu_test_gx ) {
			pchCoreName = CORE_NAME_GX;
			break;
		}							// FALL-THROUGH
	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:			// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:			// FALL-THROUGH
	case VDU_CODEC_RVX:				// FALL-THROUGH
	case VDU_CODEC_AVS:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
#if defined (CHIP_NAME_m16)
        pchCoreName = CORE_NAME_GX;
#else
		pchCoreName = CORE_NAME_CNM;
#endif
		break;
	case VDU_CODEC_HEVC:
		pchCoreName = CORE_NAME_HEVC;
		break;
	case VDU_CODEC_VP9:
		pchCoreName = CORE_NAME_GX;
		break;
	default:
		break;
	}

	if ( pchCoreName == NULL ) {
		logm_error(vdec_vdu, "[%d]There is no core registered\n", (UINT32)*phInstance);
		goto GOTO_ERROR;
	}

	for ( ui8Index = 0; ui8Index < MAX_NUM_OF_CORES; ++ui8Index ) {
		if ( strcmp(_gastCoreInfo[ui8Index].pchName, pchCoreName) == 0 ) {
			pstInstance->pstCore = &_gastCoreInfo[ui8Index];
			pstInstance->pfnExecute = _gastCoreInfo[ui8Index].pfnExecute;
			break;
		}
	}

	if ( (ui8Index == MAX_NUM_OF_CORES) || (pstInstance->pfnExecute == NULL) ) {
		logm_error(vdec_vdu, "[%d]There is no core registered\n", (UINT32)*phInstance);
		goto GOTO_ERROR;
	}

	stExeParam.pstInstance = pstInstance->pstDecInfo;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_OPEN, &stExeParam);

	_LockInstance(pstInstance, &ulFlags);
GOTO_ERROR:
	if ( eRetVal != VDU_RET_OK ) {
		pstInstance->bInUsed = FALSE;
		VDU_DBG_Close(*phInstance);
	}

	_UnlockInstance(pstInstance, &ulFlags);

GOTO_END:
	if ( eRetVal != VDU_RET_OK ) {
		_MFree(pstDecInfo);
	}

	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Close(VDU_HANDLE_T hInstance)
{
	VDU_RET_T			eRetVal = VDU_RET_ERROR;
	INSTANCE_T*			pstInstance = NULL;
	VDU_DEC_INSTANCE_T*	pstDecInfo = NULL;
	VDU_DEC_PARAM_T		stExeParam = { 0, };
	ULONG				ulFlags = 0x00;

	pstInstance = _WaitDone(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		logm_error(vdec_vdu, "[%d]Fail to wait\n", (UINT32)hInstance);

		pstInstance = _GetInstance(hInstance, &ulFlags);
		if ( pstInstance == NULL ) {
			logm_error(vdec_vdu, "[%d]Invalid instance\n", (UINT32)hInstance);
			goto GOTO_END;
		}
	}

	stExeParam.pstInstance = pstInstance->pstDecInfo;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_CLOSE, &stExeParam);

	VDU_DBG_Close(hInstance);

	_LockInstance(pstInstance, &ulFlags);
	pstInstance->bInUsed = FALSE;
	pstDecInfo = pstInstance->pstDecInfo;
	_UnlockInstance(pstInstance, &ulFlags);

	_MFree(pstDecInfo);

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Flush(VDU_HANDLE_T hInstance)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	ULONG			ulFlags = 0x00;

	pstInstance = _WaitDone(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		logm_error(vdec_vdu, "[%d]Fail to wait\n", (UINT32)hInstance);

		pstInstance = _GetInstance(hInstance, &ulFlags);
		if ( pstInstance == NULL ) {
			logm_error(vdec_vdu, "[%d]Invalid instance\n", (UINT32)hInstance);
			goto GOTO_END;
		}
	}

	pstInstance->stFeedInfo.ui32ResetAddr = pstInstance->stFeedInfo.ui32WriteAddr;
	pstInstance->stFeedInfo.ui32StreamEndAddr = VDU_INVALID_ADDRESS;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = VDU_RET_OK;

	logm_noti(vdec_vdu, "[%d]Flush 0x%08X\n", (UINT32)hInstance, pstInstance->stFeedInfo.ui32ResetAddr);

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_Reset(VDU_HANDLE_T hInstance)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	ULONG			ulFlags = 0x00;
	UINT8			ui8InstIndex = 0;
	UINT8			ui8Index = 0;

	ui8InstIndex = (hInstance == VDU_INVALID_HANDLE)?
					VDU_MAX_NUM_OF_INSTANCES : (UINT8)hInstance;
	ui8Index = (hInstance == VDU_INVALID_HANDLE)?
					0 : (UINT8)hInstance;

	do {
		pstInstance = _GetInstance((VDU_HANDLE_T)ui8Index, &ulFlags);
		if ( pstInstance != NULL ) {
			if ( pstInstance->bRunning == TRUE ) {
				VDU_DEC_PARAM_T stExeParam = { 0, };

				pstInstance->bFlushing = TRUE;

				stExeParam.pstInstance = pstInstance->pstDecInfo;
				_UnlockInstance(pstInstance, &ulFlags);

				eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_RESET, &stExeParam);

				_LockInstance(pstInstance, &ulFlags);
				pstInstance->bRunning = FALSE;
				pstInstance->bFlushing = FALSE;
				pstInstance->stFeedInfo.ui32ResetAddr = pstInstance->stFeedInfo.ui32WriteAddr;
			}
			_UnlockInstance(pstInstance, &ulFlags);
		}

		++ui8Index;
	} while ( ui8Index < ui8InstIndex);

	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_StartDecoding(VDU_HANDLE_T hInstance, VDU_LINEAR_FRAMES_T* pstLinearFrames)
{
	VDU_RET_T				eRetVal = VDU_RET_ERROR;
	INSTANCE_T*				pstInstance = NULL;
	VDU_DEC_PARAM_T			stExeParam = { 0, };
	UINT32*					pui32AddressList = NULL;
	ULONG					ulFlags = 0x00;

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	if ( pstInstance->bFlushing == TRUE ) {
		logm_info(vdec_vdu, "[%d]Flushed\n", (UINT32)hInstance);
		goto GOTO_ERROR;
	}

	if ( pstInstance->bRunning == TRUE ) {
		logm_noti(vdec_vdu, "[%d]Duplicated to start decoding\n", (UINT32)hInstance);
		goto GOTO_ERROR;
	}

	if ( (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_OFF) &&
		 (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_NONE) ) {
		if ( pstLinearFrames != NULL ) {
			if ( (pstInstance->stExternBuffer.ui32Stride != pstLinearFrames->ui32Stride) ||
				 (pstInstance->stExternBuffer.ui32Height != pstLinearFrames->ui32Height) ) {
				pstInstance->stExternBuffer.ui32Stride = pstLinearFrames->ui32Stride;
				pstInstance->stExternBuffer.ui32Height = 1088;//pstLinearFrames->ui32Height;
			}

			if ( pstLinearFrames->aui32AddressList!= NULL ) {
				memcpy(pstInstance->stExternBuffer.aui32AddressList
						+ pstInstance->stExternBuffer.ui8NumberOfBuffers,
					   pstLinearFrames->aui32AddressList,
					   pstLinearFrames->ui8NumberOfFrames * sizeof(UINT32));

				pstInstance->stExternBuffer.ui8NumberOfBuffers += pstLinearFrames->ui8NumberOfFrames;

				logm_trace(vdec_vdu, "[%d]Input %d extern memories\n",
										(UINT32)hInstance,
										pstLinearFrames->ui8NumberOfFrames);
			}
		}

		if ( pstInstance->stExternBuffer.ui8NumberOfBuffers > 0 ) {
			pui32AddressList = _Malloc(pstInstance->stExternBuffer.ui8NumberOfBuffers * sizeof(UINT32));
			if ( pui32AddressList == NULL ) {
				logm_error(vdec_vdu, "[%d]Memory allocation error\n", (UINT32)hInstance);
				goto GOTO_ERROR;
			}

			stExeParam.stLinearFrames.ui8NumberOfFrames = pstInstance->stExternBuffer.ui8NumberOfBuffers;
			stExeParam.stLinearFrames.ui32Stride = pstInstance->stExternBuffer.ui32Stride;
			stExeParam.stLinearFrames.ui32Height = pstInstance->stExternBuffer.ui32Height;
			stExeParam.stLinearFrames.aui32AddressList = pui32AddressList;
			memcpy(pui32AddressList, pstInstance->stExternBuffer.aui32AddressList,
					stExeParam.stLinearFrames.ui8NumberOfFrames * sizeof(UINT32));
			pstInstance->stExternBuffer.ui8NumberOfBuffers = 0;
		}
	} else {
		stExeParam.stLinearFrames.ui8NumberOfFrames = 0;
		stExeParam.stLinearFrames.aui32AddressList = NULL;
	}
	_UnlockInstance(pstInstance, &ulFlags);

	if ( VDU_DBG_WaitStepDecoding(hInstance) == TRUE ) {
		eRetVal = VDU_RET_OK;
		goto GOTO_END;
	}

	VDU_DBG_CheckDecTime(hInstance, VDU_DBG_START_CHECK);

	_LockInstance(pstInstance, &ulFlags);
	stExeParam.bEnableUserData = pstInstance->bEnableUserData;
	stExeParam.ui32ResetAddr = pstInstance->stFeedInfo.ui32ResetAddr;
	if ( pstInstance->stFeedInfo.ui32ResetAddr != VDU_INVALID_ADDRESS ) {
		logm_noti(vdec_vdu, "[%d]Start with reset 0x%08X\n", (UINT32)hInstance, pstInstance->stFeedInfo.ui32ResetAddr);
		pstInstance->stFeedInfo.ui32ResetAddr = VDU_INVALID_ADDRESS;
		_InitCheckingInvalidFrame(pstInstance, pstInstance->stValidFrameInfo.ui16ValidThreshold);
	}

	stExeParam.ui32StreamEndAddr = pstInstance->stFeedInfo.ui32StreamEndAddr;
	if ( pstInstance->stFeedInfo.ui32StreamEndAddr != VDU_INVALID_ADDRESS ) {
		logm_noti(vdec_vdu, "[%d]End of stream 0x%08X\n", (UINT32)hInstance, pstInstance->stFeedInfo.ui32StreamEndAddr);
	}

	stExeParam.ui32WriteAddr = pstInstance->stFeedInfo.ui32WriteAddr;

	if ( (pstInstance->ePicScanMode != VDU_PIC_SCAN_ALL) ||
		 (_DidFinishCheckInvalidFrame(pstInstance) == TRUE) ) {
		stExeParam.ePicScanMode = pstInstance->ePicScanMode;
		_FinishCheckInvalidFrame(pstInstance);
	} else {
		stExeParam.ePicScanMode = _DidCheckValidIFrame(pstInstance)? VDU_PIC_SCAN_IP : VDU_PIC_SCAN_I;	// Skip B frame after I frame
		logm_info(vdec_vdu, "[%d]Decoded only %s-frames\n", (UINT32)hInstance, (stExeParam.ePicScanMode == VDU_PIC_SCAN_I)? "I" : "IP");
	}

	pstInstance->bRunning = TRUE;

	stExeParam.pstInstance = pstInstance->pstDecInfo;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_DECODE, &stExeParam);
/*	if ( eRetVal == VDU_RET_BUSY ) {
		VDU_DEC_RESULT_T	stDecResult = { 0, };

		stDecResult.bDecSuccess = FALSE;
		stDecResult.si8DecodedIndex = VDU_INVALID_FRAME_INDEX;
		stDecResult.si8DisplayIndex = VDU_INVALID_FRAME_INDEX;
		stDecResult.eNotiType = VDU_NOTI_INVALID;

		_Callback(pstInstance->pstDecInfo, &stDecResult);

		eRetVal = VDU_RET_OK;
	} else */if ( eRetVal != VDU_RET_OK ) {
		VDU_DBG_CheckDecTime(hInstance, VDU_DBG_POSTPONE_CHECK);
	}

	logm_trace(vdec_vdu, "[%d]Start decoding(%d)\n", (UINT32)hInstance, eRetVal);

	_LockInstance(pstInstance, &ulFlags);
	if ( eRetVal != VDU_RET_OK ) {
		pstInstance->bRunning = FALSE;
	} else {
		pstInstance->stFeedInfo.ui32StreamEndAddr = stExeParam.ui32StreamEndAddr;
	}

GOTO_ERROR:
	_UnlockInstance(pstInstance, &ulFlags);

	if ( pui32AddressList != NULL ) {
		_MFree(pui32AddressList);
	}

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_FeedAu(VDU_HANDLE_T hInstance, VDU_AU_T* pstAu)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	ULONG			ulFlags = 0x00;

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	if ( pstInstance->stFeedInfo.ui32StreamEndAddr != VDU_INVALID_ADDRESS ) {
		logm_warning(vdec_vdu, "[%d]Pre-stream data remained\n", (UINT32)hInstance);
		goto GOTO_ERROR;
	}

	if ( (pstAu->ui32StartAddr < pstInstance->pstDecInfo->ui32CpbBaseAddr) ||
		 (pstInstance->pstDecInfo->ui32CpbEndAddr <= pstAu->ui32StartAddr) ||
		 (pstAu->ui32Size == 0) ) {
		if ( pstAu->bEndOfStream == TRUE ) {
			pstInstance->stFeedInfo.ui32StreamEndAddr = pstInstance->stFeedInfo.ui32WriteAddr;
			eRetVal = VDU_RET_OK;
			logm_noti(vdec_vdu, "[%d]Feed end of stream\n", (UINT32)hInstance);
		} else {
			logm_error(vdec_vdu, "[%d]Invalid AU(0x%08X+0x%08X)\n",
								(UINT32)hInstance, pstAu->ui32StartAddr, pstAu->ui32Size);
		}

		goto GOTO_ERROR;
	}

	if ( pstInstance->stFeedInfo.ui32WriteAddr != pstAu->ui32StartAddr ) {
		pstInstance->stFeedInfo.ui32ResetAddr = pstAu->ui32StartAddr;
	}

	pstInstance->stFeedInfo.ui32WriteAddr = pstAu->ui32StartAddr + pstAu->ui32Size;
	if ( pstInstance->stFeedInfo.ui32WriteAddr >= pstInstance->pstDecInfo->ui32CpbEndAddr ) {
		pstInstance->stFeedInfo.ui32WriteAddr = pstInstance->pstDecInfo->ui32CpbBaseAddr
												+ (pstInstance->stFeedInfo.ui32WriteAddr
													- pstInstance->pstDecInfo->ui32CpbEndAddr);
	}

	logm_trace(vdec_vdu, "[%d]Feed 0x%08X/0x%08X\n",
							(UINT32)hInstance, pstAu->ui32StartAddr, pstAu->ui32Size);

	if ( pstAu->bEndOfStream == TRUE ) {
		pstInstance->stFeedInfo.ui32StreamEndAddr = pstInstance->stFeedInfo.ui32WriteAddr;
		logm_noti(vdec_vdu, "[%d]Feed end of stream\n", (UINT32)hInstance);
	}

	eRetVal = VDU_RET_OK;

GOTO_ERROR:
	_UnlockInstance(pstInstance, &ulFlags);

GOTO_END:
	return eRetVal;
}
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_SetOption(VDU_HANDLE_T hInstance, VDU_OPTION_T* pstOption)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	ULONG			ulFlags = 0x00;

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	if( ( pstOption->eUserData > VDU_USERDATA_MAX ) &&
		( pstOption->ePicScanMode > VDU_PIC_SCAN_MAX ) ) {
		logm_error(vdec_vdu, "[%d]Invalid option parameter : User data(%d), Scan mode(%d)\n",
								(UINT32)hInstance, pstOption->eUserData, pstOption->ePicScanMode);
		goto GOTO_ERROR;
	}

	if( pstOption->eUserData <= VDU_USERDATA_MAX ) {
		pstInstance->bEnableUserData = (pstOption->eUserData == VDU_USERDATA_ENABLE)? TRUE : FALSE;
	}

	if( (pstOption->ePicScanMode <= VDU_PIC_SCAN_MAX) &&
		(pstInstance->pstDecInfo->bOneFrameDecoding == FALSE) ) {
		pstInstance->ePicScanMode = pstOption->ePicScanMode;
	}

	eRetVal = VDU_RET_OK;

	logm_info(vdec_vdu, "[%d]Set option : User data(%d), Scan mode(%d)\n",
							(UINT32)hInstance, pstOption->eUserData, pstOption->ePicScanMode);

GOTO_ERROR:
	_UnlockInstance(pstInstance, &ulFlags);

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_ClearFrame(VDU_HANDLE_T hInstance, SINT8 si8FrameIdx)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	VDU_DEC_PARAM_T stParam = { 0, };
	ULONG			ulFlags = 0x00;

	if ( (si8FrameIdx < 0) || (si8FrameIdx >= VDU_DEC_NUM_OF_MAX_DPB) ) {
		logm_error(vdec_vdu, "[%d]Invalid frame index %d\n", (UINT32)hInstance, si8FrameIdx);
		goto GOTO_END;
	}

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	stParam.pstInstance = pstInstance->pstDecInfo;
	stParam.si8FrameIndex = si8FrameIdx;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_CLEAR, &stParam);

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_GetStatus(VDU_HANDLE_T hInstance, VDU_STATUS_T *peStatus)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	VDU_DEC_PARAM_T stExeParam = { 0, };
	ULONG			ulFlags = 0x00;

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	stExeParam.pstInstance = pstInstance->pstDecInfo;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_CHECK, &stExeParam);

	*peStatus = (stExeParam.bReady == TRUE)? VDU_STATUS_READY : VDU_STATUS_BUSY;

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_GetLinearFrame(VDU_HANDLE_T hInstance, SINT8 si8FrameIdx, void* pFrameBuf)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	INSTANCE_T*		pstInstance = NULL;
	VDU_DEC_PARAM_T stParam = { 0, };
	ULONG			ulFlags = 0x00;

	pstInstance = _GetInstance(hInstance, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	}

	stParam.pstInstance = pstInstance->pstDecInfo;
	stParam.si8FrameIndex = si8FrameIdx;
	stParam.pFrameBuffer = pFrameBuf;
	_UnlockInstance(pstInstance, &ulFlags);

	eRetVal = pstInstance->pfnExecute(VDU_DEC_CMD_CONVERT, &stParam);

GOTO_END:
	return eRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
VDU_RET_T		VDU_DEC_RegisterCore(char* pchCoreName, VDU_DEC_EXECUTE_FN_T pfnExecute)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	UINT8			ui8Index = 0x00;
	CORE_INFO_T*	pstCore = NULL;
	VDU_DEC_PARAM_T	stParam = { NULL, };

	if ( (pchCoreName == NULL) || (pfnExecute == NULL) ) {
		logm_error(vdec_vdu, "Null parameter\n");
		goto GOTO_END;
	}

	if ( _gpfnCallback == NULL ) {
		logm_error(vdec_vdu, "VDU did not initialize\n");
		goto GOTO_END;
	}

	for ( ui8Index = 0; ui8Index < MAX_NUM_OF_CORES; ++ui8Index ) {
		if ( _gastCoreInfo[ui8Index].pchName == NULL )
			continue;

		if ( strcmp(_gastCoreInfo[ui8Index].pchName, pchCoreName) == 0 ) {
			pstCore = &_gastCoreInfo[ui8Index];
			break;
		}
	}

	if ( pstCore == NULL ) {
		logm_error(vdec_vdu, "Unknown core %s\n", pchCoreName);
		goto GOTO_END;
	}

	pstCore->pfnExecute = pfnExecute;

	stParam.pfnCallback = _Callback;

	eRetVal = pstCore->pfnExecute(VDU_DEC_CMD_INIT, &stParam);
	if ( eRetVal != VDU_RET_OK ) {
		goto GOTO_END;
	}

	printk(KERN_INFO "VDU Registered core %s\n", pchCoreName);

GOTO_END:
	return eRetVal;
}
EXPORT_SYMBOL(VDU_DEC_RegisterCore);

VDU_RET_T		VDU_DEC_UnregisterCore(char* pchCoreName)
{
	VDU_RET_T		eRetVal = VDU_RET_ERROR;
	UINT8			ui8Index = 0x00;
	CORE_INFO_T*	pstCore = NULL;

	if ( pchCoreName == NULL ) {
		logm_error(vdec_vdu, "Null parameter\n");
		goto GOTO_END;
	}

	for ( ui8Index = 0; ui8Index < MAX_NUM_OF_CORES; ++ui8Index ) {
		if ( _gastCoreInfo[ui8Index].pchName == NULL )
			continue;

		if ( strcmp(_gastCoreInfo[ui8Index].pchName, pchCoreName) == 0 ) {
			pstCore = &_gastCoreInfo[ui8Index];
			break;
		}
	}

	if ( pstCore == NULL ) {
		logm_error(vdec_vdu, "Unknown core %s\n", pchCoreName);
		goto GOTO_END;
	}

	eRetVal = pstCore->pfnExecute(VDU_DEC_CMD_DEINIT, NULL);
	if ( eRetVal != VDU_RET_OK ) {
		goto GOTO_END;
	}

	pstCore->pfnExecute = NULL;

GOTO_END:
	return eRetVal;
}
EXPORT_SYMBOL(VDU_DEC_UnregisterCore);

/*==============================================================================
    Static Implementation Group
==============================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static INSTANCE_T*	_GetInstance(VDU_HANDLE_T hInstance, ULONG* pulFlags)
{
	INSTANCE_T*		pstRetInstance = NULL;

	if ( hInstance < VDU_MAX_NUM_OF_INSTANCES ) {
		pstRetInstance = &_gastInstancePool[(UINT32)hInstance];
		_LockInstance(pstRetInstance, pulFlags);

		if ( pstRetInstance->bInUsed == FALSE ) {
			_UnlockInstance(pstRetInstance, pulFlags);
			pstRetInstance = NULL;
		}
	}

	return pstRetInstance;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static INSTANCE_T*	_WaitDone(VDU_HANDLE_T hInstance, ULONG* pulFlags)
{
	INSTANCE_T*		pstRetInstance = NULL;
	UINT64			ui64TimeOut = get_jiffies_64() + msecs_to_jiffies(WAIT_TIME_OUT_VALUE);

	logm_noti(vdec_vdu, "[%d]Start of Waiting\n", (UINT32)hInstance);

	do {
		pstRetInstance = _GetInstance(hInstance, pulFlags);
		if ( pstRetInstance == NULL ) {
			break;
		}

		pstRetInstance->bFlushing = TRUE;

		if ( pstRetInstance->bRunning == FALSE ) {
			VDU_DEC_PARAM_T stExeParam = { 0, };

			stExeParam.pstInstance = pstRetInstance->pstDecInfo;
			_UnlockInstance(pstRetInstance, pulFlags);

			pstRetInstance->pfnExecute(VDU_DEC_CMD_CHECK, &stExeParam);

			_LockInstance(pstRetInstance, pulFlags);
			if ( stExeParam.bRunning == FALSE ) {
				pstRetInstance->bFlushing = FALSE;
				break;
			}
		}
		_UnlockInstance(pstRetInstance, pulFlags);
		pstRetInstance = NULL;
		msleep(1);
	} while( time_before64(get_jiffies_64(), ui64TimeOut) );

	logm_noti(vdec_vdu, "[%d]End of waiting : %s\n",
							(UINT32)hInstance, (pstRetInstance == NULL)? "Fail" : "Success");

	return pstRetInstance;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_InitCheckingInvalidFrame(INSTANCE_T* pstInstance, UINT16 ui16Threshold)
{
	BOOLEAN			bRetVal = FALSE;

	pstInstance->stValidFrameInfo.ui16ValidThreshold = ui16Threshold;
	pstInstance->stValidFrameInfo.si8FirstFrameIndex = VDU_INVALID_FRAME_INDEX;
	pstInstance->stValidFrameInfo.ui16NumberOfSearch = NUMBER_OF_SEARCHING_I
							+ pstInstance->stValidFrameInfo.ui16ValidThreshold;
	pstInstance->stValidFrameInfo.ui32GarbageFramesBit = 0;

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_CheckInvalidFrame(INSTANCE_T* pstInstance, VDU_DEC_RESULT_T* pstDecResult, ULONG* pulFlags)
{
	BOOLEAN				bRetVal = FALSE;

	if ( _DidFinishCheckInvalidFrame(pstInstance) == FALSE ) {
		if ( pstDecResult->si8DecodedIndex >= 0 ) {
			switch ( pstDecResult->ePictureType ) {
			case VDU_PICTURE_I:
				if( (pstInstance->stValidFrameInfo.si8FirstFrameIndex != VDU_INVALID_FRAME_INDEX) ||
					(pstInstance->pstDecInfo->eCodecType == VDU_CODEC_HEVC) ) {
					_FinishCheckInvalidFrame(pstInstance);
				} else {
					pstInstance->stValidFrameInfo.si8FirstFrameIndex = pstDecResult->si8DecodedIndex;
					pstInstance->stValidFrameInfo.ui16NumberOfSearch = pstInstance->stValidFrameInfo.ui16ValidThreshold;
				}
				bRetVal = TRUE;
				break;

			case VDU_PICTURE_P:
				if ( pstInstance->stValidFrameInfo.si8FirstFrameIndex == VDU_INVALID_FRAME_INDEX ) {
					pstInstance->stValidFrameInfo.ui32GarbageFramesBit |= (1 << pstDecResult->si8DecodedIndex);
					pstInstance->stValidFrameInfo.ui16NumberOfSearch--;
					pstDecResult->si8DecodedIndex = -2;
				} else {
					_FinishCheckInvalidFrame(pstInstance);
					bRetVal = TRUE;
				}
				break;

			default:
				pstInstance->stValidFrameInfo.ui32GarbageFramesBit |= (1 << pstDecResult->si8DecodedIndex);
				pstInstance->stValidFrameInfo.ui16NumberOfSearch--;
				pstDecResult->si8DecodedIndex = -2;
			}
		} else if ( pstInstance->stValidFrameInfo.ui16NumberOfSearch > 0 ) {
			pstInstance->stValidFrameInfo.ui16NumberOfSearch--;
		}

		if ( (pstInstance->stValidFrameInfo.si8FirstFrameIndex != VDU_INVALID_FRAME_INDEX) &&
			 (pstInstance->stValidFrameInfo.si8FirstFrameIndex == pstDecResult->si8DisplayIndex) ) {
			_FinishCheckInvalidFrame(pstInstance);
		}
	} else {
		bRetVal = TRUE;
	}

	if ( (pstDecResult->si8DisplayIndex >= 0) &&
		 (pstInstance->stValidFrameInfo.ui32GarbageFramesBit & (1 << pstDecResult->si8DisplayIndex)) ) {
		pstInstance->stValidFrameInfo.ui32GarbageFramesBit &= ~(1 << pstDecResult->si8DisplayIndex);

		if ( (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_OFF) &&
			 (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_NONE) ) {
			pstInstance->stExternBuffer.aui32AddressList[pstInstance->stExternBuffer.ui8NumberOfBuffers]
				= pstDecResult->stPictureInfo.stAddress.ui32Y;
			pstInstance->stExternBuffer.ui8NumberOfBuffers += 1;
		} else {
			VDU_DEC_PARAM_T stParam = { NULL, };

			stParam.pstInstance = pstInstance->pstDecInfo;
			stParam.si8FrameIndex = pstDecResult->si8DisplayIndex;
			_UnlockInstance(pstInstance, pulFlags);

			pstInstance->pfnExecute(VDU_DEC_CMD_CLEAR, &stParam);

			_LockInstance(pstInstance, pulFlags);
		}

		pstDecResult->si8DisplayIndex = -3;
	}

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_DidCheckValidIFrame(INSTANCE_T* pstInstance)
{
	BOOLEAN			bRetVal = FALSE;

	if ( pstInstance->stValidFrameInfo.ui16NumberOfSearch <=
							pstInstance->stValidFrameInfo.ui16ValidThreshold ) {
		bRetVal = TRUE;
	}

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_DidFinishCheckInvalidFrame(INSTANCE_T* pstInstance)
{
	BOOLEAN			bRetVal = FALSE;

	if ( pstInstance->stValidFrameInfo.ui16NumberOfSearch == 0 ) {
		bRetVal = TRUE;
	}

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static void			_FinishCheckInvalidFrame(INSTANCE_T* pstInstance)
{
	pstInstance->stValidFrameInfo.ui16NumberOfSearch = 0;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static void 		_Callback(VDU_DEC_INSTANCE_T* pstDecInfo, VDU_DEC_RESULT_T* pstDecResult)
{
	INSTANCE_T*			pstInstance = NULL;
	VDU_HANDLE_T		hVduHandle = VDU_INVALID_HANDLE;
	VDU_RESULT_T*		pstNotiInfo = NULL;
	BOOLEAN				bSkipNotification = FALSE;
	SINT32				si32DecTime = VDU_DBG_POSTPONE_CHECK;
	ULONG				ulFlags = 0x00;

	pstInstance = _GetInstance(pstDecInfo->hVduHandle, &ulFlags);
	if ( pstInstance == NULL ) {
		goto GOTO_END;
	} else if( pstInstance->pstDecInfo != pstDecInfo ) {
		_UnlockInstance(pstInstance, &ulFlags);
		goto GOTO_END;
	}

	hVduHandle = pstInstance->pstDecInfo->hVduHandle;

	if ( (pstDecResult->eNotiType == VDU_NOTI_FRAME_SKIP) &&
		 (pstDecResult->si8DisplayIndex >= 0) ) {
		 	pstDecResult->eNotiType = VDU_NOTI_DISPLAY;
	}

	if ( pstDecResult->bDecSuccess == TRUE ) {
		si32DecTime = pstDecResult->ui32DecTime;

		if ( _CheckInvalidFrame(pstInstance, pstDecResult, &ulFlags) == FALSE ) {
			pstDecResult->eNotiType = VDU_NOTI_FRAME_SKIP;
			bSkipNotification = FALSE;
		}
	}

	pstNotiInfo = &pstInstance->stNotiInfo;
	pstNotiInfo->eNotiType = pstDecResult->eNotiType;
	pstNotiInfo->si8FrmIndex = pstDecResult->si8DisplayIndex;
	pstNotiInfo->si8DecodedIndex = pstDecResult->si8DecodedIndex;
	pstNotiInfo->bDecSuccess = pstDecResult->bDecSuccess;
	pstNotiInfo->bFieldSuccess = pstDecResult->bFieldSuccess;
	pstNotiInfo->ui32ReadPtr = pstDecResult->ui32ReadPtr;

	pstNotiInfo->stFrameInfo.ui8NumOfFrames = pstInstance->pstDecInfo->stSequence.ui8NumOfFrames;
	if ( (pstNotiInfo->si8FrmIndex >= 0) ||
		 (pstNotiInfo->eNotiType == VDU_NOTI_DPB_EMPTY) ) {
		_SetDisplayFrameInfo(&pstNotiInfo->stFrameInfo, &pstDecResult->stPictureInfo ,pstInstance);
		// Request by T. Kim(VDISP)
		pstNotiInfo->stFrameInfo.stDispInfo.ui8DisplayPeriod |= ((pstNotiInfo->si8FrmIndex << 3) & 0xF8);
	}

	if ( pstInstance->bFlushing == TRUE ) {
		if ( pstNotiInfo->si8FrmIndex >= 0 ) {
			if ( (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_OFF) &&
				 (pstInstance->pstDecInfo->eExternLinearBufMode != VDU_LINEAR_MODE_NONE) ) {
				pstInstance->stExternBuffer.aui32AddressList[pstInstance->stExternBuffer.ui8NumberOfBuffers]
					= pstDecResult->stPictureInfo.stAddress.ui32Y;
				pstInstance->stExternBuffer.ui8NumberOfBuffers += 1;
			} else {
				VDU_DEC_PARAM_T stParam = { NULL, };

				stParam.pstInstance = pstInstance->pstDecInfo;
				stParam.si8FrameIndex = pstDecResult->si8DisplayIndex;

				_UnlockInstance(pstInstance, &ulFlags);

				pstInstance->pfnExecute(VDU_DEC_CMD_CLEAR, &stParam);

				_LockInstance(pstInstance, &ulFlags);
			}
		}
		pstInstance->bRunning = FALSE;
		_UnlockInstance(pstInstance, &ulFlags);

	} else {
		if ( bSkipNotification == FALSE ) {
			VDU_DBG_PrintResultInfo(hVduHandle, pstNotiInfo);

			if ( pstDecResult->si8DisplayIndex >= 0 ) {
				VDU_DBG_PrintFrameInfo(hVduHandle, &pstNotiInfo->stFrameInfo);
			}
		}

		pstInstance->bRunning = FALSE;
		_UnlockInstance(pstInstance, &ulFlags);

		VDU_DBG_CheckDecTime(hVduHandle, si32DecTime);

		if ( bSkipNotification == FALSE ) {
			_gpfnCallback(hVduHandle, pstNotiInfo);
		}
	}

GOTO_END:
	return;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetDisplayFrameInfo(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											INSTANCE_T* pstInstance)
{
	BOOLEAN					bRetVal = FALSE;
	VDU_DEC_INSTANCE_T*		pstDecInfo = pstInstance->pstDecInfo;
	VDU_CODEC_T				eCodecType = pstDecInfo->eCodecType;
	BOOLEAN					bInterlacedSequence = pstDecInfo->stSequence.bInterlaced;

	_SetSizeInfo(pstFrameInfo, pstPictureInfo, eCodecType, bInterlacedSequence);
	_SetAspectRatio(pstFrameInfo, pstPictureInfo, eCodecType);
	pstFrameInfo->stAddress.ui32Y = pstPictureInfo->stAddress.ui32Y;
	pstFrameInfo->stAddress.ui32Cb = pstPictureInfo->stAddress.ui32Cb;
	pstFrameInfo->stAddress.ui32Cr = pstPictureInfo->stAddress.ui32Cr;
	pstFrameInfo->pstMapInfo = pstPictureInfo->pstMapInfo;
	_SetFrameRate(pstFrameInfo, pstPictureInfo, eCodecType);
	_SetScanTypeAndPeriod(pstFrameInfo, pstPictureInfo, eCodecType,
							bInterlacedSequence, &pstInstance->stPreDisplayInfo.ui8PicTimingStruct);
	pstFrameInfo->stDispInfo.bLowDelay = pstDecInfo->stSequence.bLowDelay;
	_SetFramePackArrange(pstFrameInfo, pstPictureInfo, eCodecType,
							&pstInstance->stPreDisplayInfo.si32FramePackArrange);
	pstFrameInfo->stDispInfo.ui8ErrorRate = pstPictureInfo->stDispInfo.ui8ErrorRate;
	pstFrameInfo->stDispInfo.ui8PictureType = pstPictureInfo->stDispInfo.ui8PictureType;
	pstFrameInfo->stDispInfo.bFieldPicture = pstPictureInfo->stDispInfo.bFieldPicture;

	if ( pstDecInfo->eCodecType == VDU_CODEC_MPEG2 ) {
		pstFrameInfo->stDispInfo.si32Mp2DispWidth = pstPictureInfo->stMpeg2Info.si32DispWidth;
		pstFrameInfo->stDispInfo.si32Mp2DispHeight = pstPictureInfo->stMpeg2Info.si32DispHeight;
	} else {
		pstFrameInfo->stDispInfo.si32Mp2DispWidth = 0;
		pstFrameInfo->stDispInfo.si32Mp2DispHeight = 0;
	}

	_SetActiveFormatDescription(pstFrameInfo, pstPictureInfo, eCodecType);
	pstFrameInfo->stUserData.ui32Address = pstPictureInfo->stUserData.ui32Address;
	pstFrameInfo->stUserData.ui32Size = pstPictureInfo->stUserData.ui32Size;

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetSizeInfo(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType,
									BOOLEAN bInterlacedSequence)
{
	BOOLEAN		bRetVal = FALSE;

	pstFrameInfo->stRect.ui32Stride = pstPictureInfo->stSizeInfo.ui32Stride;
	pstFrameInfo->stRect.ui32PicWidth = pstPictureInfo->stSizeInfo.ui32Width;
	pstFrameInfo->stRect.ui32PicHeight = pstPictureInfo->stSizeInfo.ui32Height;

	switch ( eCodecType ) {
	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:
		{
			// Support Only 4:2:0
			UINT16	ui16CropUnitX = 2;
			UINT16	ui16CropUnitY = 4 - ((bInterlacedSequence == FALSE)? 2 : 0);

			pstFrameInfo->stRect.ui16CropLeft = pstPictureInfo->stH264Info.ui16CropLeft;
			pstFrameInfo->stRect.ui16CropRight = pstPictureInfo->stH264Info.ui16CropRight;
			pstFrameInfo->stRect.ui16CropTop = pstPictureInfo->stH264Info.ui16CropTop;
			pstFrameInfo->stRect.ui16CropBottom = pstPictureInfo->stH264Info.ui16CropBottom;

			if( ((pstFrameInfo->stRect.ui16CropRight / ui16CropUnitX) >= 8) ||
				((pstFrameInfo->stRect.ui16CropBottom / ui16CropUnitY) >= 8) )
			{
				logm_noti(vdec_vdu, "Brainfart cropping, cropping disabled(%d/%d/%d/%d)\n",
										pstFrameInfo->stRect.ui16CropLeft, pstFrameInfo->stRect.ui16CropRight,
										pstFrameInfo->stRect.ui16CropTop, pstFrameInfo->stRect.ui16CropBottom);

				pstFrameInfo->stRect.ui16CropLeft = 0;
				pstFrameInfo->stRect.ui16CropRight = 0;
				pstFrameInfo->stRect.ui16CropTop = 0;
				pstFrameInfo->stRect.ui16CropBottom = 0;
			}
		}
		break;

	case VDU_CODEC_HEVC:
		pstFrameInfo->stRect.ui16CropLeft = pstPictureInfo->stHevcInfo.ui16CropLeft;
		pstFrameInfo->stRect.ui16CropRight = pstPictureInfo->stHevcInfo.ui16CropRight;
		pstFrameInfo->stRect.ui16CropTop = pstPictureInfo->stHevcInfo.ui16CropTop;
		pstFrameInfo->stRect.ui16CropBottom = pstPictureInfo->stHevcInfo.ui16CropBottom;
		break;

	case VDU_CODEC_MPEG2:			// FALL-THROUGH
	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:			// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:			// FALL-THROUGH
	case VDU_CODEC_RVX:				// FALL-THROUGH
	case VDU_CODEC_AVS:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	default:
		pstFrameInfo->stRect.ui16CropLeft = 0;
		pstFrameInfo->stRect.ui16CropRight = 0;
		pstFrameInfo->stRect.ui16CropTop = 0;
		pstFrameInfo->stRect.ui16CropBottom = 0;
		break;
	}

	// Check invalid crop size
	if ( pstFrameInfo->stRect.ui16CropLeft > pstFrameInfo->stRect.ui32PicWidth ) {
		pstFrameInfo->stRect.ui16CropLeft = 0;
	}

	if ( pstFrameInfo->stRect.ui16CropRight > pstFrameInfo->stRect.ui32PicWidth ) {
		pstFrameInfo->stRect.ui16CropRight = 0;
	}

	if ( pstFrameInfo->stRect.ui16CropTop > pstFrameInfo->stRect.ui32PicHeight ) {
		pstFrameInfo->stRect.ui16CropTop = 0;
	}

	if ( pstFrameInfo->stRect.ui16CropBottom > pstFrameInfo->stRect.ui32PicHeight ) {
		pstFrameInfo->stRect.ui16CropBottom = 0;
	}

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
* Sample Aspect Ratio(SAR) = Display Aspect Ratio(DAR) * horizontal size / vertical size
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetAspectRatio(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType)
{
	BOOLEAN			bRetVal = FALSE;
	const UINT8		aui8Dar[ 5][2] = { { 0, 0}, { 1, 1}, { 4, 3}, {16, 9}, {221, 100}	};								// MPEG2
	const UINT8		aui8Sar[17][2] = { { 0, 0}, { 1, 1}, {12,11}, {10,11}, {16,11}, {40,33},							// MPEG4 & VC-1 & H.264
										{24,11}, {20,11}, {32,11},	 {80,33}, {18,11}, {15,11}, {64,33}, {160,99},	// VC-1 & H.264
										{4, 3}, { 3, 2}, { 2, 1} };														// H.264
	UINT16			ui16DispWidth = pstPictureInfo->stSizeInfo.ui32Width;
	UINT16			ui16DispHeight = pstPictureInfo->stSizeInfo.ui32Height;
	UINT8			ui8RetAspectRatio = 0;
	UINT16			ui16RetSarW = 1;
	UINT16			ui16RetSarH = 1;
	UINT8			ui8ArInfo = 0;
	UINT8			ui8ArIdc = 0;

	switch ( eCodecType ) {
	case VDU_CODEC_MPEG2:			// FALL-THROUGH
	case VDU_CODEC_AVS:			// DAR --> SAR
		if ( eCodecType == VDU_CODEC_MPEG2 ) {
			ui8ArInfo = pstPictureInfo->stMpeg2Info.ui8DisplayAspectRatio & 0x0F;
		} else {
			ui8ArInfo = pstPictureInfo->stAvsInfo.ui8DisplayAspectRatio & 0x0F;
		}

		if ( (1 < ui8ArInfo) && (ui8ArInfo < 5) ) {
			UINT32	ui32SarW, ui32SarH;
			UINT32	ui32Gcd;

			ui16DispHeight = (ui16DispHeight == 1088)? 1080 : ui16DispHeight;
			ui32SarW = aui8Dar[ui8ArInfo][0] * ui16DispHeight;
			ui32SarH = aui8Dar[ui8ArInfo][1] * ui16DispWidth;

			ui32Gcd = (ui32SarW > ui32SarH)? _GetGcd(ui32SarW, ui32SarH) : _GetGcd(ui32SarH, ui32SarW);
			ui32Gcd = (ui32Gcd == 0)? 1 : ui32Gcd;

			ui32SarW = ui32SarW / ui32Gcd;
			ui32SarH = ui32SarH / ui32Gcd;

			if ( (ui32SarW > 0xFFFF) || (ui32SarH > 0xFFFF) ) {
				ui16RetSarW = 100;
				ui16RetSarH = 100 * ui32SarH / ui32SarW;
			} else {
				ui16RetSarW = (UINT16)ui32SarW;
				ui16RetSarH = (UINT16)ui32SarH;
			}

			ui8RetAspectRatio = ui8ArInfo;
		} else	{
			ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth, ui16DispHeight);
		}
		break;

	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:		// (Extendec)SAR--> DAR
		ui8ArInfo = pstPictureInfo->stMpeg4Info.ui8AspectRatioInfo & 0x0F;

		if ( (0 < ui8ArInfo) && (ui8ArInfo < 6) ) {
			ui16RetSarW = aui8Sar[ui8ArInfo][0];
			ui16RetSarH = aui8Sar[ui8ArInfo][1];
		} else if( ui8ArInfo == 0x0F ) {	// extended SAR
			ui16RetSarW = pstPictureInfo->stMpeg4Info.ui8AspectRatioWidth;
			ui16RetSarH = pstPictureInfo->stMpeg4Info.ui8AspectRatioHeight;
		} else {	// Unspecified
			logm_noti(vdec_vdu, "[MPEG4]Wrong Aspect ratio information %d\n", pstPictureInfo->stMpeg4Info.ui8AspectRatioInfo);
		}

		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;

	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:	// SAR --> DAR
		ui8ArIdc = pstPictureInfo->stH264Info.ui8AspectRatioIdc;

		if ( (0 < ui8ArIdc) && (ui8ArIdc < 17) ) {
			ui16RetSarW = aui8Sar[ui8ArIdc][0];
			ui16RetSarH = aui8Sar[ui8ArIdc][1];
		} else if( ui8ArIdc == 0xFF ) {	// extended SAR
			ui16RetSarW = pstPictureInfo->stH264Info.ui16AspectRatioWidth;
			ui16RetSarH = pstPictureInfo->stH264Info.ui16AspectRatioHeight;
		} else {	// Unspecified
			logm_noti(vdec_vdu, "[H264]Wrong Aspect ratio information %d\n", pstPictureInfo->stH264Info.ui8AspectRatioIdc);
		}

		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;

	case VDU_CODEC_HEVC:
		ui8ArIdc = pstPictureInfo->stHevcInfo.ui8AspectRatioIdc;

		if ( (0 < ui8ArIdc) && (ui8ArIdc < 17) ) {
			ui16RetSarW = aui8Sar[ui8ArIdc][0];
			ui16RetSarH = aui8Sar[ui8ArIdc][1];
		} else if( ui8ArIdc == 0xFF ) {	// extended SAR
			ui16RetSarW = pstPictureInfo->stHevcInfo.ui16AspectRatioWidth;
			ui16RetSarH = pstPictureInfo->stHevcInfo.ui16AspectRatioHeight;
		} else {	// Unspecified
			logm_noti(vdec_vdu, "[HEVC]Wrong Aspect ratio information %d\n", pstPictureInfo->stHevcInfo.ui8AspectRatioIdc);
		}

		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;

	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:		// SAR --> DAR
		ui8ArInfo = pstPictureInfo->stVc1Info.ui8AspectRatio & 0x0F;

		if( (0 < ui8ArInfo) && (ui8ArInfo < 14) ) {
			ui16RetSarW = aui8Sar[ui8ArInfo][0];
			ui16RetSarH = aui8Sar[ui8ArInfo][1];
		} else if( ui8ArInfo == 0x0F ) {
			ui16RetSarW = pstPictureInfo->stVc1Info.ui8AspectRatioWidth;
			ui16RetSarH = pstPictureInfo->stVc1Info.ui8AspectRatioHeight;
		} else {	// Unspecified
			logm_noti(vdec_vdu, "[VC1]Wrong Aspect ratio information %d\n", pstPictureInfo->stVc1Info.ui8AspectRatio);
		}

		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;

	case VDU_CODEC_RVX:
		ui8ArInfo = pstPictureInfo->stRvInfo.ui8AspectRatio & 0x0F;

		if ( (0 < ui8ArInfo) && (ui8ArInfo < 6) ) {
			ui16RetSarW = aui8Sar[ui8ArInfo][0];
			ui16RetSarH = aui8Sar[ui8ArInfo][1];
		} else if( ui8ArInfo == 0x0F ) {
			ui16RetSarW = pstPictureInfo->stRvInfo.ui8AspectRatioWidth;
			ui16RetSarH = pstPictureInfo->stRvInfo.ui8AspectRatioHeight;
		} else {	// Unspecified
			logm_noti(vdec_vdu, "[RV]Wrong Aspect ratio information %d\n", pstPictureInfo->stRvInfo.ui8AspectRatio);
		}

		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;

	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	default:
		ui8RetAspectRatio = _GetDisplayAspectRatio(ui16DispWidth * ui16RetSarW, ui16DispHeight * ui16RetSarH);
		break;
	}

	pstFrameInfo->stAspectRatio.ui16ParW = (ui16RetSarW == 0)? 1 : ui16RetSarW;
	pstFrameInfo->stAspectRatio.ui16ParH = (ui16RetSarH == 0)? 1 : ui16RetSarH;
	pstFrameInfo->stAspectRatio.ui8Mpeg2Dar = ui8RetAspectRatio;

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetFrameRate(VDU_FRM_INFO_T* pstFrameInfo,
									VDU_DEC_FRAME_INFO_T* pstPictureInfo,
									VDU_CODEC_T eCodecType)
{
	BOOLEAN		bRetVal = FALSE;

	pstFrameInfo->stFrameRate.ui32Residual = pstPictureInfo->stDispInfo.ui32FrameRateResidual;
	pstFrameInfo->stFrameRate.ui32Divider = pstPictureInfo->stDispInfo.ui32FrameRateDivider;

	switch ( eCodecType ) {
	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:
		pstFrameInfo->stFrameRate.bVariable = !pstPictureInfo->stH264Info.bFixedFrameRate;
		break;

	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_RVX:
		pstFrameInfo->stFrameRate.ui32Residual = 0;
		pstFrameInfo->stFrameRate.ui32Divider = 0;
		pstFrameInfo->stFrameRate.bVariable = FALSE;
		break;

	case VDU_CODEC_MPEG2:			// FALL-THROUGH
	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:			// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:			// FALL-THROUGH
	case VDU_CODEC_AVS:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	case VDU_CODEC_HEVC:			// FALL-THROUGH
	default:
		pstFrameInfo->stFrameRate.bVariable = FALSE;
		break;
	}

	if ( (pstFrameInfo->stFrameRate.ui32Residual == -1)	||
		 (pstFrameInfo->stFrameRate.ui32Divider == -1)	||
		 (pstFrameInfo->stFrameRate.ui32Residual ==  0)	||
		 (pstFrameInfo->stFrameRate.ui32Divider ==  0) ) {
		pstFrameInfo->stFrameRate.ui32Residual	= 0;
		pstFrameInfo->stFrameRate.ui32Divider	= 0;
	}

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*	PROG TFF  RFF  PicStruct									Period 1st 2nd 3rd
	1    0    0    3        1 Decoding 1 Display				1T     F   X   X
	1    1    0    3        1 Decoding 1 Display				1T     F   X   X
	1    0    1    3        1 Decoding 2 Display				2T     F   F   X
	1    1    1    3        1 Decoding 3 Display				3T     F   F   F
	0    0    0    3        B -> T (frame picture)				2T     B   T   X
	0    0    0    2        B (field pic.)(Last Decoded Pic.)	2T     T   B   X
	0    1    0    3        T -> B (frame picture)				2T     T   B   X
	0    1    0    1        T (field pic.)(Last Decoded Pic.)	2T     B   T   X
	0    0    0    3        B -> T								2T     B   T   X
	0    1    0    3        T -> B								2T     T   B   X
	0    0    1    3        B -> T -> B							3T     B   T   B
	0    1    1    3        T -> B -> T							3T     T   B   T
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetScanTypeAndPeriod(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											VDU_CODEC_T eCodecType,
											BOOLEAN bInterlacedSequence,
											UINT8* pui8PrePicTimingStruct)
{
	BOOLEAN					bRetVal = FALSE;
	const AVC_PIC_STRUCT_T	astAvcPicStruct[9] =	{	{ FALSE, FALSE, VDU_FRM_SCAN_PROGRESSIVE	},	// 0: 1 Frame Display
														{ FALSE, FALSE, VDU_FRM_SCAN_TOP_FIRST		},	// 1: T Field Display (Field Pic)
														{ TRUE , FALSE, VDU_FRM_SCAN_BOTTOM_FIRST	},	// 2: B Field Display (Field Pic)
														{ TRUE , FALSE, VDU_FRM_SCAN_TOP_FIRST		},	// 3: T->B Display
														{ FALSE, FALSE, VDU_FRM_SCAN_BOTTOM_FIRST	},	// 4: B->T Display
														{ TRUE , TRUE , VDU_FRM_SCAN_TOP_FIRST		},	// 5: T->B->T Display
														{ FALSE, TRUE , VDU_FRM_SCAN_BOTTOM_FIRST	},	// 6: B->T->B Display
														{ FALSE, TRUE , VDU_FRM_SCAN_PROGRESSIVE	},	// 7: 2 Frame Display
														{ TRUE , TRUE , VDU_FRM_SCAN_PROGRESSIVE	}	// 8: 3 Frame Display
													};	//	bTopFieldFirst / bRepeatFirstField / eFrameScanType
	BOOLEAN					bTopFieldFirst = FALSE;
	BOOLEAN					bRepeatFirstField = FALSE;
	VDU_FRM_SCAN_T			eFrameScanType = VDU_FRM_SCAN_INVALID;
	UINT8					ui8DisplayPeriod = 1;
	BOOLEAN					bNonPairedField = FALSE;

	switch ( eCodecType ) {
	case VDU_CODEC_MPEG2:
		bTopFieldFirst		= pstPictureInfo->stMpeg2Info.bTopFieldFirst;
		bRepeatFirstField	= pstPictureInfo->stMpeg2Info.bRepeatFirstField;

		if( bInterlacedSequence == TRUE ) {
			switch ( pstPictureInfo->stMpeg2Info.ui8PictureStructure ) {
			case 1:
				eFrameScanType	= VDU_FRM_SCAN_BOTTOM_FIRST;
				break;

			case 2:
				eFrameScanType	= VDU_FRM_SCAN_TOP_FIRST;
				break;

			case 3:
			default:
				if ( bTopFieldFirst == TRUE ) {
					eFrameScanType	= VDU_FRM_SCAN_TOP_FIRST;
				} else {
					eFrameScanType	= VDU_FRM_SCAN_BOTTOM_FIRST;
				}
				break;
			}
		} else {
			eFrameScanType	= VDU_FRM_SCAN_PROGRESSIVE;
		}

		if ( pstPictureInfo->stMpeg2Info.eNpfInfo != VDU_DEC_NPF_PAIRED) {
			bNonPairedField = TRUE;
		}
		break;

	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:
		bTopFieldFirst		= pstPictureInfo->stMpeg4Info.bTopFieldFirst;
		bRepeatFirstField	= FALSE;

		if( bInterlacedSequence == TRUE ) {
			if ( bTopFieldFirst == TRUE ) {
				eFrameScanType = VDU_FRM_SCAN_TOP_FIRST;
			} else {
				eFrameScanType = VDU_FRM_SCAN_BOTTOM_FIRST;
			}
		} else {
			eFrameScanType = VDU_FRM_SCAN_PROGRESSIVE;
		}
		break;

	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:
		if ( (pstPictureInfo->stH264Info.stSei.si32PicTimingStruct < 0) ||
			 (pstPictureInfo->stH264Info.stSei.si32PicTimingStruct >= 9) ) {
			pstPictureInfo->stH264Info.stSei.si32PicTimingStruct = *pui8PrePicTimingStruct;
		}

		if ( (pstPictureInfo->stH264Info.stSei.si32PicTimingStruct < 0) ||
			 (pstPictureInfo->stH264Info.stSei.si32PicTimingStruct >= 9) ) {
			bTopFieldFirst		= pstPictureInfo->stH264Info.bTopFieldFirst;
			bRepeatFirstField	= FALSE;

			if( bInterlacedSequence == FALSE ) {
				eFrameScanType = VDU_FRM_SCAN_PROGRESSIVE;
			} else if ( bTopFieldFirst == TRUE ) {
				eFrameScanType = VDU_FRM_SCAN_TOP_FIRST;
			} else {
				eFrameScanType = VDU_FRM_SCAN_BOTTOM_FIRST;
			}
		} else {
			const AVC_PIC_STRUCT_T*	pstAvcPicStruct = &astAvcPicStruct[pstPictureInfo->stH264Info.stSei.si32PicTimingStruct];

			bTopFieldFirst     	= pstAvcPicStruct->bTopFieldFirst;
			bRepeatFirstField 	= pstAvcPicStruct->bRepeatFirstField;
			eFrameScanType		= pstAvcPicStruct->eFrameScanType;
		}

		if ( pstPictureInfo->stH264Info.stSei.si32PicTimingStruct == 1 ) {
			eFrameScanType = VDU_FRM_SCAN_TOP_FIRST;
			ui8DisplayPeriod = 0;
		} else if ( pstPictureInfo->stH264Info.stSei.si32PicTimingStruct == 2 ) {
			eFrameScanType = VDU_FRM_SCAN_BOTTOM_FIRST;
			ui8DisplayPeriod = 0;
		}

		*pui8PrePicTimingStruct = pstPictureInfo->stH264Info.stSei.si32PicTimingStruct;

		if ( eFrameScanType != VDU_FRM_SCAN_PROGRESSIVE ) {
			if ( pstPictureInfo->stH264Info.eNpfInfo == VDU_DEC_NPF_BOTTOM_ONLY ) {
				eFrameScanType = VDU_FRM_SCAN_BOTTOM_ONLY;
			} else if ( pstPictureInfo->stH264Info.eNpfInfo == VDU_DEC_NPF_TOP_ONLY ) {
				eFrameScanType = VDU_FRM_SCAN_TOP_ONLY;
			}
		}

		if ( pstPictureInfo->stH264Info.eNpfInfo != VDU_DEC_NPF_PAIRED) {
			bNonPairedField = TRUE;
		}
		break;

	case VDU_CODEC_HEVC:
		if ( (pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct < 0) ||
			 (pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct >= 9) ) {
			pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct = *pui8PrePicTimingStruct;
		}

		if ( (pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct < 0) ||
			 (pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct >= 9) ) {
			bTopFieldFirst		= FALSE;
			bRepeatFirstField	= FALSE;

			if( bInterlacedSequence == FALSE ) {
				eFrameScanType = VDU_FRM_SCAN_PROGRESSIVE;
			} else {
				eFrameScanType = VDU_FRM_SCAN_BOTTOM_FIRST;
			}
		} else {
			const AVC_PIC_STRUCT_T*	pstAvcPicStruct = &astAvcPicStruct[pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct];

			bTopFieldFirst     	= pstAvcPicStruct->bTopFieldFirst;
			bRepeatFirstField 	= pstAvcPicStruct->bRepeatFirstField;
			eFrameScanType		= pstAvcPicStruct->eFrameScanType;
		}

		if ( pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct == 1 ) {
			eFrameScanType = VDU_FRM_SCAN_TOP_FIRST;
			ui8DisplayPeriod = 0;
		} else if ( pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct == 2 ) {
			eFrameScanType = VDU_FRM_SCAN_BOTTOM_FIRST;
			ui8DisplayPeriod = 0;
		}

		*pui8PrePicTimingStruct = pstPictureInfo->stHevcInfo.stSei.si32PicTimingStruct;

		if ( pstPictureInfo->stDispInfo.ui8ErrorRate == 100 ) {
			bNonPairedField = TRUE;
		}
		break;

	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:
		bTopFieldFirst		= pstPictureInfo->stVc1Info.bTopFieldFirst;
		bRepeatFirstField	= pstPictureInfo->stVc1Info.bRepeatFirstField;

		if( bInterlacedSequence == TRUE ) {
			if ( bTopFieldFirst == TRUE ) {
				eFrameScanType	= VDU_FRM_SCAN_TOP_FIRST;
			} else {
				eFrameScanType	= VDU_FRM_SCAN_BOTTOM_FIRST;
			}
		} else {
			eFrameScanType	= VDU_FRM_SCAN_PROGRESSIVE;
		}
		break;

	case VDU_CODEC_AVS:
		bTopFieldFirst		= pstPictureInfo->stAvsInfo.bTopFieldFirst;
		bRepeatFirstField	= pstPictureInfo->stAvsInfo.bRepeatFirstField;

		if( bInterlacedSequence == TRUE ) {
			if ( bTopFieldFirst == TRUE ) {
				eFrameScanType	= VDU_FRM_SCAN_TOP_FIRST;
			} else {
				eFrameScanType	= VDU_FRM_SCAN_BOTTOM_FIRST;
			}
		} else {
			eFrameScanType	= VDU_FRM_SCAN_PROGRESSIVE;
		}
		break;

	case VDU_CODEC_RVX:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	default:
		bTopFieldFirst		= FALSE;
		bRepeatFirstField	= FALSE;

		if( bInterlacedSequence == TRUE ) {
			eFrameScanType	= VDU_FRM_SCAN_BOTTOM_FIRST;
		} else {
			eFrameScanType	= VDU_FRM_SCAN_PROGRESSIVE;
		}
		break;

	}

	if ( (eFrameScanType == VDU_FRM_SCAN_TOP_FIRST) ||
		 (eFrameScanType == VDU_FRM_SCAN_BOTTOM_FIRST) ) {
		++ui8DisplayPeriod;		// Interlaced
	} else if ( (bTopFieldFirst == TRUE) && (bRepeatFirstField == TRUE) ) {
		++ui8DisplayPeriod;		// Progressive && TopFieldFirst
	}

	if ( bRepeatFirstField == TRUE ) {
		++ui8DisplayPeriod;
	}

	pstFrameInfo->stDispInfo.eFrameScanType		= eFrameScanType;
	pstFrameInfo->stDispInfo.bTopFieldFirst		= bTopFieldFirst;
	pstFrameInfo->stDispInfo.bRepeatFirstField	= bRepeatFirstField;
	pstFrameInfo->stDispInfo.ui8DisplayPeriod	= ui8DisplayPeriod;
	pstFrameInfo->stDispInfo.bNonPairedField	= bNonPairedField;

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* Sample Aspect Ratio(SAR) = Display Aspect Ratio(DAR) * horizontal size / vertical size
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetFramePackArrange(VDU_FRM_INFO_T* pstFrameInfo,
											VDU_DEC_FRAME_INFO_T* pstPictureInfo,
											VDU_CODEC_T eCodecType,
											SINT32* psi32PreFramePackArrange)
{
	BOOLEAN		bRetVal = FALSE;

	switch ( eCodecType ) {
	case VDU_CODEC_H264_AVC:
		if ( pstPictureInfo->stH264Info.stSei.bFramePackingArrangemenExist == TRUE) {
			if ( pstPictureInfo->stH264Info.stSei.bFramePackingArrangementCancelFlag == TRUE) {
				pstFrameInfo->stDispInfo.si8FramePackArrange = -1;
			} else {
				pstFrameInfo->stDispInfo.si8FramePackArrange = pstPictureInfo->stH264Info.stSei.si8FramePackingArrangementType;
			}

			*psi32PreFramePackArrange = pstFrameInfo->stDispInfo.si8FramePackArrange;
		} else {
			pstFrameInfo->stDispInfo.si8FramePackArrange = *psi32PreFramePackArrange;
		}

		if ( pstFrameInfo->stDispInfo.si8FramePackArrange == 5 ) {
			if ( pstPictureInfo->stH264Info.stSei.si8ContentInterpretationType == 0 ) {
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_NONE;
			} else if ( pstPictureInfo->stH264Info.stSei.si8ContentInterpretationType ==		// 1 : Frame0 == left, 2 : frame0 == right
						(pstPictureInfo->stH264Info.stSei.si8CurrentFrameIsFrame0Flag + 1) ) {	// Is frame0
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_RIGHT; // Right Frame
			} else {
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_LEFT; // Left Frame
			}
		} else {
			pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_NONE;
		}
		break;

	case VDU_CODEC_H264_MVC:
		pstFrameInfo->stDispInfo.si8FramePackArrange = 5;

		if ( pstPictureInfo->stH264Info.stSei.si8MvcViewIndexDecoded == 0 ) {
			pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_LEFT; // Base view
		} else {
			pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_RIGHT; // Non base view
		}
		break;

	case VDU_CODEC_HEVC:
		if ( pstPictureInfo->stHevcInfo.stSei.bFramePackingArrangemenExist == TRUE) {
			if ( pstPictureInfo->stHevcInfo.stSei.bFramePackingArrangementCancelFlag == TRUE) {
				pstFrameInfo->stDispInfo.si8FramePackArrange = -1;
			} else {
				pstFrameInfo->stDispInfo.si8FramePackArrange = pstPictureInfo->stHevcInfo.stSei.si8FramePackingArrangementType;
			}

			*psi32PreFramePackArrange = pstFrameInfo->stDispInfo.si8FramePackArrange;
		} else {
			pstFrameInfo->stDispInfo.si8FramePackArrange = *psi32PreFramePackArrange;
		}

		if ( pstFrameInfo->stDispInfo.si8FramePackArrange == 5 ) {
			if ( pstPictureInfo->stHevcInfo.stSei.si8ContentInterpretationType == 0 ) {
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_NONE;
			} else if ( pstPictureInfo->stHevcInfo.stSei.si8ContentInterpretationType ==		// 1 : Frame0 == left, 2 : frame0 == right
						(pstPictureInfo->stHevcInfo.stSei.si8CurrentFrameIsFrame0Flag + 1) ) {	// Is frame0
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_RIGHT; // Right Frame
			} else {
				pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_LEFT; // Left Frame
			}
		} else {
			pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_NONE;
		}
		break;

	case VDU_CODEC_MPEG2:			// FALL-THROUGH
	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:			// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:			// FALL-THROUGH
	case VDU_CODEC_RVX:				// FALL-THROUGH
	case VDU_CODEC_AVS:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	default:
		pstFrameInfo->stDispInfo.si8FramePackArrange = -2;
		pstFrameInfo->stDispInfo.eOrderOf3D = VDU_FRM_3D_NONE;
		break;
	}

	bRetVal = TRUE;

	return bRetVal;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* Sample Aspect Ratio(SAR) = Display Aspect Ratio(DAR) * horizontal size / vertical size
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static BOOLEAN		_SetActiveFormatDescription(VDU_FRM_INFO_T* pstFrameInfo,
												VDU_DEC_FRAME_INFO_T* pstPictureInfo,
												VDU_CODEC_T eCodecType)
{
	BOOLEAN		bRetVal = FALSE;

	switch ( eCodecType ) {
	case VDU_CODEC_MPEG2:
		pstFrameInfo->stUserData.ui8ActiveFmtDesc = pstPictureInfo->stMpeg2Info.ui8ActiveFmtDesc;
		break;

	case VDU_CODEC_H264_AVC:		// FALL-THROUGH
	case VDU_CODEC_H264_MVC:
		pstFrameInfo->stUserData.ui8ActiveFmtDesc = pstPictureInfo->stH264Info.ui8ActiveFmtDesc;
		break;

	case VDU_CODEC_MPEG4:			// FALL-THROUGH
	case VDU_CODEC_H263:			// FALL-THROUGH
	case VDU_CODEC_SORENSON_SPARK:	// FALL-THROUGH
	case VDU_CODEC_XVID:			// FALL-THROUGH
	case VDU_CODEC_DIVX3:			// FALL-THROUGH
	case VDU_CODEC_DIVX4:			// FALL-THROUGH
	case VDU_CODEC_DIVX5:			// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V1:		// FALL-THROUGH
	case VDU_CODEC_VC1_RCV_V2:		// FALL-THROUGH
	case VDU_CODEC_VC1_ES:			// FALL-THROUGH
	case VDU_CODEC_RVX:				// FALL-THROUGH
	case VDU_CODEC_AVS:				// FALL-THROUGH
	case VDU_CODEC_THEORA:			// FALL-THROUGH
	case VDU_CODEC_VP3:				// FALL-THROUGH
	case VDU_CODEC_VP8:				// FALL-THROUGH
	case VDU_CODEC_HEVC:			// FALL-THROUGH
	default:
		pstFrameInfo->stUserData.ui8ActiveFmtDesc = 0;
		break;
	}

	bRetVal = TRUE;

	return bRetVal;}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* Sample Aspect Ratio(SAR) = Display Aspect Ratio(DAR) * horizontal size / vertical size
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static UINT8		_GetDisplayAspectRatio(UINT32 ui32DisplayWidth, UINT32 ui32DisplayHeight)
{
	UINT8	ui8RetDisplayAr = 1;
	UINT32	ui32DispAr100;

	ui32DisplayHeight = (ui32DisplayHeight == 0)? 1 : ui32DisplayHeight;
	ui32DispAr100 = 100 * ui32DisplayWidth / ui32DisplayHeight;

	if ( ui32DispAr100 >= 221 * 100 / 100  ) {		// 221 : 100
		ui8RetDisplayAr = 4;
	} else if( ui32DispAr100 >= 16 * 100 / 9  ) {	// 16 : 9
		ui8RetDisplayAr = 3;
	} else if( ui32DispAr100 >= 4 * 100 / 3  ) {	// 4 : 3
		ui8RetDisplayAr = 2;
	}

	return ui8RetDisplayAr;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static UINT32		_GetGcd(UINT32 ui32Bigger, UINT32 ui32Smaller)
{
	UINT32		ui32RetVal = 0;

	if( ui32Smaller != 0 ) {
		ui32RetVal = ((ui32Bigger % ui32Smaller) == 0)?
						ui32Smaller : _GetGcd(ui32Smaller, ui32Bigger % ui32Smaller);
	}

	return ui32RetVal;
}

/** @} */
