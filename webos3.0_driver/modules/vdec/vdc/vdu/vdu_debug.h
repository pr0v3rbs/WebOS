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
 * Video decoding unit debug header for vdec device.
 * VDEC device will teach you how to make device driver with lg1154 platform.
 *
 * author     Youngwoo Jin(youngwoo.jin@lge.com)
 * version    1.0
 * date       2013.10.03
 * note       Additional information.
 *
 */

#ifndef _VDU_DEBUG_H_
#define _VDU_DEBUG_H_

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
#include "vdu_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/
#define VDU_DBG_POSTPONE_CHECK		(-1)
#define VDU_DBG_START_CHECK			(-2)

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Function Prototype Declaration
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Extern Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Generic Usage Functions
------------------------------------------------------------------------------*/
void		VDU_DBG_Init(void);
void		VDU_DBG_Open(VDU_HANDLE_T hInstance, VDU_OPEN_PARAM_T *pstOpenParam);
void		VDU_DBG_Close(VDU_HANDLE_T hInstance);
void		VDU_DBG_CheckDecTime(VDU_HANDLE_T hInstance, SINT32 si32DecTime);
BOOLEAN		VDU_DBG_WaitStepDecoding(VDU_HANDLE_T hInstance);
void		VDU_DBG_PrintResultInfo(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult);
void		VDU_DBG_PrintFrameInfo(VDU_HANDLE_T hInstance, VDU_FRM_INFO_T* pstFrameInfo);

#ifdef __cplusplus
}
#endif

#endif /* _VDU_DEBUG_H_ */

