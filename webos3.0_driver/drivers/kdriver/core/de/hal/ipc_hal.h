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
 *
 *  @author		
 *  @version	1.0 
 *  @date		
 *
 *  @addtogroup 
 *	@{
 */

#ifndef	_IPC_HAL_H_
#define	_IPC_HAL_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "ipc_cmd_header.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define MAX_DEV_NAME  16
#define MAX_IPC_LOG   200

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
struct ipc_client {
	int (*write)(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
	int (*read)( UINT32 mcu_id, void *pMsg, UINT32 msgCnt);
	int (*wrmem)(UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
};

struct ipc_handle {
	struct ipc_client *m_ipc_client;
	char *p_ready; // flag for firmware ready
	char dev_name[MAX_DEV_NAME];
	UINT32 mcu_id;
	void *prev;
};

struct ipc_log {
	UINT32 cmd[MAX_IPC_LOG];
	char  *buf[MAX_IPC_LOG];
	UINT32 cnt[MAX_IPC_LOG];
	UINT32 idx;
};

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
int ipc_hal_init(void);
int ipc_hal_cleanup(void);
struct ipc_handle *ipc_hal_open(char *name, MCU_ID_TYPE mcu_id);
int ipc_hal_close(struct ipc_handle *ipc_handle);
int ipc_hal_list(void);
int ipc_hal_start(UINT32 mcu_id, char flag);
int ipc_hal_trace(BOOLEAN p);

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _IPC_HAL_H_ */

/** @} */

