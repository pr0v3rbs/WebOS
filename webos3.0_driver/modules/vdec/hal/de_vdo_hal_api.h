/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  register interface header for png device. ( used only within kdriver )
 *	png device will teach you how to make device driver with new platform.
 *
 *  @author		Youngwoo Jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.10.21
 *
 *  @addtogroup h13
 *	@{
 */

#ifndef	_VDO_REG_API_H_
#define	_VDO_REG_API_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "vdec_base.h"
#include "hal/de_ipc_hal_api.h" // for struct S_DIS_BUF_T

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
void	DE_VDO_HAL_Init(void);
void	DE_VDO_HAL_Suspend(void);
void	DE_VDO_HAL_Resume(void);
void	DE_VDO_HAL_Update(UINT8 ui8VdoCh, S_DE_IPC_T *pDeIpc, UINT32 ui32FieldCount);
void	DE_VDO_HAL_InitCmd(void);
void	DE_VDO_HAL_RunCmd(UINT8 ui8DeIfCh[], UINT32 aui32DisplayInfo[], UINT32 ui32SyncField);
void	DE_VDO_HAL_SetVSyncSel(UINT8 ui8VdoCh, UINT32 ui32VSyncSrc);
void	DE_VDO_HAL_Register(UINT8 u8VdoCh, UINT8 u8DeIfCh, BOOLEAN bTiled);
void	DE_VDO_HAL_SetInterlaced(UINT8 ui8VdoCh, BOOLEAN bInterlaced);
#if defined (CHIP_NAME_d14)
void 	DE_VDO_HAL_SetSeamless(UINT8 ui8SeamlessMode);
void 	DE_VDO_HAL_Reset(void);
void	DE_VDO_HAL_SetVsiInfo(UINT8 ui8VdoCh, UINT32 ui32PicWidth, UINT32 ui32PicHeight, UINT32 ui32H_Offset, UINT32 ui32V_Offset);
#endif
#endif
