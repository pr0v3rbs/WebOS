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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     youngki.lyu@lge.com
 * version    1.0
 * date       2011.04.30
 * note       Additional information.
 *
 */

#ifndef _IPC_REG_API_H_
#define _IPC_REG_API_H_


#include "vdec_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


#define		IPC_REG_INVALID_OFFSET					0xFFFFFFFF

#define		IPC_REG_CEILING_4BYTES( _offset )			(((_offset) + 0x3) & (~0x3))



void IPC_REG_Init(void);

UINT32 IPC_REG_CPB_GetWrOffset(UINT8 ch);
void IPC_REG_CPB_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset);
UINT32 IPC_REG_CPB_GetRdOffset(UINT8 ch);
void IPC_REG_CPB_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset);

UINT32 IPC_REG_AUIB_GetWrOffset(UINT8 ch);
void IPC_REG_AUIB_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset);
UINT32 IPC_REG_AUIB_GetRdOffset(UINT8 ch);
void IPC_REG_AUIB_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset);

UINT32 IPC_REG_DISPQ_GetWrOffset(UINT8 ch);
void IPC_REG_DISPQ_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset);
UINT32 IPC_REG_DISPQ_GetRdOffset(UINT8 ch);
void IPC_REG_DISPQ_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset);

UINT32 IPC_REG_DISPCLEARQ_GetWrOffset(UINT8 ch);
void IPC_REG_DISPCLEARQ_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset);
UINT32 IPC_REG_DISPCLEARQ_GetRdOffset(UINT8 ch);
void IPC_REG_DISPCLEARQ_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset);

UINT32 IPC_REG_CMD_GetWrOffset(void);
void IPC_REG_CMD_SetWrOffset(UINT32 ui32WrOffset);
UINT32 IPC_REG_CMD_GetRdOffset(void);
void IPC_REG_CMD_SetRdOffset(UINT32 ui32RdOffset);

UINT32 IPC_REG_CMD_GetBaseAddr(void);
void IPC_REG_CMD_SetBaseAddr(UINT32 ui32Cmdptr);
UINT32 IPC_REG_CMD_GetBufSize(void);
void IPC_REG_CMD_SetBufSize(UINT32 ui32CmdSize);

UINT32 IPC_REG_REQ_GetWrOffset(void);
void IPC_REG_REQ_SetWrOffset(UINT32 ui32WrOffset);
UINT32 IPC_REG_REQ_GetRdOffset(void);
void IPC_REG_REQ_SetRdOffset(UINT32 ui32RdOffset);

UINT32 IPC_REG_REQ_GetBaseAddr(void);
void IPC_REG_REQ_SetBaseAddr(UINT32 ui32Reqptr);
UINT32 IPC_REG_REQ_GetBufSize(void);
void IPC_REG_REQ_SetBufSize(UINT32 ui32ReqSize);

UINT32 IPC_REG_DBG_GetWrOffset(void);
void IPC_REG_DBG_SetWrOffset(UINT32 ui32WrOffset);
UINT32 IPC_REG_DBG_GetRdOffset(void);
void IPC_REG_DBG_SetRdOffset(UINT32 ui32RdOffset);

UINT32 IPC_REG_DBG_GetBaseAddr(void);
void IPC_REG_DBG_SetBaseAddr(UINT32 ui32DBGptr);
UINT32 IPC_REG_DBG_GetBufSize(void);
void IPC_REG_DBG_SetBufSize(UINT32 ui32DBGSize);

UINT32 IPC_REG_VES_GetDb(void);
void IPC_REG_VES_SetDb(UINT32 ves_db);
UINT32 IPC_REG_AUIB_GetDb(void);
void IPC_REG_AUIB_SetDb(UINT32 auib_db);
UINT32 IPC_REG_CPB_GetDb(void);
void IPC_REG_CPB_SetDb(UINT32 cpb_db);
UINT32 IPC_REG_VDC_GetDb(void);
void IPC_REG_VDC_SetDb(UINT32 vdc_db);
UINT32 IPC_REG_DPB_GetDb(void);
void IPC_REG_DPB_SetDb(UINT32 dpb_db);
UINT32 IPC_REG_LIPSYNC_GetDb(void);
void IPC_REG_LIPSYNC_SetDb(UINT32 lipsync_db);
UINT32 IPC_REG_RATE_GetDb(void);
void IPC_REG_RATE_SetDb(UINT32 rate_db);
UINT32 IPC_REG_PTS_GetDb(void);
void IPC_REG_PTS_SetDb(UINT32 pts_db);
UINT32 IPC_REG_DE_IF_GetDb(void);
void IPC_REG_DE_IF_SetDb(UINT32 de_if_db);
UINT32 IPC_REG_DE_SYNC_GetDb(void);
void IPC_REG_DE_SYNC_SetDb(UINT32 de_sync_db);
UINT32 IPC_REG_RAMLOG_GetDb(void);
void IPC_REG_RAMLOG_SetDb(UINT32 ramlog_db);
UINT32 IPC_REG_DISPQ_GetDb(void);
void IPC_REG_DISPQ_SetDb(UINT32 dispq_db);
UINT32 IPC_REG_DISPCLEARQ_GetDb(void);
void IPC_REG_DISPCLEARQ_SetDb(UINT32 dispclearq_db);

UINT32 IPC_REG_Get_McuSwVer(void);
void IPC_REG_Set_McuSwVer(UINT32 mcu_sw_ver);
UINT32 IPC_REG_Get_WatchdogCnt(void);
void IPC_REG_Set_WatchdogCnt(UINT32 watchdog_cnt);


#ifdef __cplusplus
}
#endif

#endif /* _IPC_REG_API_H_ */

