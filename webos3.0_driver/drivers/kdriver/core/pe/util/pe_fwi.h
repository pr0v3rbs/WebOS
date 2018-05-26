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

/** @file pe_fwi.h
 *
 *  fw ipc interface header for pe device. ( used only within kdriver )
 *	
 *	@author		
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

#ifndef	_PE_FWI_H_
#define	_PE_FWI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "de_model.h"
#include "mcu/de_ipc_def.h"
#include "hal/ipc_hal.h"
#include "ipc_def.h"
#include "ipc_drv.h"

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
//#define PE_FWI_SETDATA(cmd, msg, n)		DE_KIPC_SetData(cmd, msg, n)
#define PE_FWI_OFFSET(_str_)			_str_##_OFFSET
//#define PE_FWI_BE_SETDATA(cmd, msg, n)	BE_KIPC_SetData(cmd, msg, n)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset : 12;
	UINT32 length : 12;
	UINT32 win_id : 4;
}
PE_FWI_PQ_TABLE_HEAD_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int DE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
extern int BE_KIPC_SetData(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_FWI_H_ */
