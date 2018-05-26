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

#ifndef	_IPC_PHYS_HAL_H_
#define	_IPC_PHYS_HAL_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "../mcu/de_ipc_def.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef struct {
	int (*get_irq_num)(UINT32 mcu_id, UINT32 *ipc_irq_num, UINT32 *sync_irq_num);
	int (*get_ipc_intr_status)(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
	int (*clear_ipc_intr_status)(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, UINT32 *pStatus);
	int (*clear_wdg_status)(UINT32 mcu_id, BOOLEAN turnOn);
	int (*wakeup_ipc)(UINT32 mcu_id, VIDEO_IPC_TYPE_T ipcType, BOOLEAN turnOn);
	int (*init_ipc)(UINT32 mcu_id, UINT32 *pBuffToMCU, UINT32 *pBuffFrMCU);
	int (*free_ipc)(UINT32 *pBuffToMCU, UINT32 *pBuffFrMCU);
	int (*init_interrupt)(UINT32 mcu_id, BOOLEAN intr_en);
	BOOLEAN (*check_irq4vsync)(UINT32 mcu_id);
} ipc_reg_cb_t;

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
int ipc_phys_hal_init(void);
int ipc_phys_hal_cleanup(void);
int ipc_phys_hal_GetIntrStatus(UINT32 mcu_id,           \
                               VIDEO_IPC_TYPE_T ipcType,\
                               UINT32 *pStatus);
int ipc_phys_hal_ClearIPCStatus(UINT32 mcu_id,\
                               VIDEO_IPC_TYPE_T ipcType,\
                               UINT32 *pStatus);
int ipc_phys_hal_ClearWDGStatus(UINT32 mcu_id);
int ipc_phys_hal_WakeUpIPC(UINT32 mcu_id);
int ipc_phys_hal_IPCInit(UINT32 mcu_id,      \
                         UINT32 *pBuffToMCU, \
                         UINT32 *pBuffFrMCU);
BOOLEAN ipc_phys_hal_IPCFree(UINT32 *pBuffToMCU, UINT32 *pBuffFrMCU);
int ipc_phys_hal_GetIrqNum(UINT32 mcu_id,      \
                           UINT32 *ipc_irq_num,\
                           UINT32 *sync_irq_num);
int ipc_phys_hal_InitInterrupt(UINT32 mcu_id, BOOLEAN intr_en);
BOOLEAN ipc_phys_hal_CheckIrq4Vsync(UINT32 mcu_id);
int ipc_phys_hal_WakeUpSeamless(UINT32 mcu_id);

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _IPC_HAL_H_ */

/** @} */


