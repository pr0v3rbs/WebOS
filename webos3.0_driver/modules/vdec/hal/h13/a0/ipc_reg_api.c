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
 * author     youngki.lyu (youngki.lyu@lge.com)
 * version    1.0
 * date       2010.04.30
 * note       Additional information.
 *
 * @addtogroup h13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#ifdef __XTENSA__
#include "../mcu_hal_api.h"
#else
#include <linux/kernel.h>
#endif

#include "../ipc_reg_api.h"

#include "../lg1154_vdec_base.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define VDEC_IPC_REG_BASE				(H13_VDEC_BASE + 0x1800)

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	UINT32	wr_offset;	// byte offset
	UINT32	rd_offset;	// byte offset
	UINT32	base;
	UINT32	size;
} buf_base_index_t;

typedef struct {
	UINT32	wr_offset;	// byte offset
	UINT32	rd_offset;	// byte offset
} buf_index_t;

typedef struct {
   buf_base_index_t	dbgb;             		// 00
   buf_base_index_t	cmdb;            		// 10
   buf_base_index_t	reqb;             		// 20

   buf_index_t			dispq[VDEC_NUM_OF_CHANNEL];			// 30
   buf_index_t			dispclearq[VDEC_NUM_OF_CHANNEL];		// 48
   buf_index_t			auib[VDEC_NUM_OF_CHANNEL];		// 60
   buf_index_t			cpb[VDEC_NUM_OF_CHANNEL];		// 78

   UINT32				ves_db;				// 90
   UINT32				auib_db;			// 94
   UINT32				cpb_db;				// 98
   UINT32				vdc_db;				// 9C
   UINT32				dpb_db;				// A0
   UINT32				lipsync_db;			// A4
   UINT32				rate_db;			// A8
   UINT32				pts_db;				// AC
   UINT32				de_if_db;			// B0
   UINT32				de_sync_db;			// B4
   UINT32				ramlog_db;			// B8
   UINT32				dispq_db;			// BC
   UINT32				dispclearq_db;		// C0

   UINT32				reserved[9];			// C4, C8, CC, D0, D4, D8, DC, E0, E4

   UINT32				mcu_sw_ver;			// E8
   UINT32				watchdog_cnt;		// EC

   UINT32				lipsync_base[4];		// F0 ~ FC, for LipSync
} s_reg_ipc_t;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
volatile static s_reg_ipc_t *stpIPC_Reg, reg;


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
void IPC_REG_Init(void)
{
	/* we do not use IPC registers */
	stpIPC_Reg = &reg;
}


UINT32 IPC_REG_CPB_GetWrOffset(UINT8 ch)
{
	return stpIPC_Reg->cpb[ch].wr_offset;
}

void IPC_REG_CPB_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset)
{
	stpIPC_Reg->cpb[ch].wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_CPB_GetRdOffset(UINT8 ch)
{
	return stpIPC_Reg->cpb[ch].rd_offset;
}

void IPC_REG_CPB_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset)
{
	stpIPC_Reg->cpb[ch].rd_offset = ui32RdOffset;
}


UINT32 IPC_REG_AUIB_GetWrOffset(UINT8 ch)
{
	return stpIPC_Reg->auib[ch].wr_offset;
}

void IPC_REG_AUIB_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset)
{
	stpIPC_Reg->auib[ch].wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_AUIB_GetRdOffset(UINT8 ch)
{
	return stpIPC_Reg->auib[ch].rd_offset;
}

void IPC_REG_AUIB_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset)
{
	stpIPC_Reg->auib[ch].rd_offset = ui32RdOffset;
}


UINT32 IPC_REG_DISPQ_GetWrOffset(UINT8 ch)
{
	return stpIPC_Reg->dispq[ch].wr_offset;
}

void IPC_REG_DISPQ_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset)
{
	stpIPC_Reg->dispq[ch].wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_DISPQ_GetRdOffset(UINT8 ch)
{
	return stpIPC_Reg->dispq[ch].rd_offset;
}

void IPC_REG_DISPQ_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset)
{
	stpIPC_Reg->dispq[ch].rd_offset = ui32RdOffset;
}


UINT32 IPC_REG_DISPCLEARQ_GetWrOffset(UINT8 ch)
{
	return stpIPC_Reg->dispclearq[ch].wr_offset;
}

void IPC_REG_DISPCLEARQ_SetWrOffset(UINT8 ch, UINT32 ui32WrOffset)
{
	stpIPC_Reg->dispclearq[ch].wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_DISPCLEARQ_GetRdOffset(UINT8 ch)
{
	return stpIPC_Reg->dispclearq[ch].rd_offset;
}

void IPC_REG_DISPCLEARQ_SetRdOffset(UINT8 ch, UINT32 ui32RdOffset)
{
	stpIPC_Reg->dispclearq[ch].rd_offset = ui32RdOffset;
}


UINT32 IPC_REG_CMD_GetWrOffset(void)
{
	return stpIPC_Reg->cmdb.wr_offset;
}

void IPC_REG_CMD_SetWrOffset(UINT32 ui32WrOffset)
{
	stpIPC_Reg->cmdb.wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_CMD_GetRdOffset(void)
{
	return stpIPC_Reg->cmdb.rd_offset;
}

void IPC_REG_CMD_SetRdOffset(UINT32 ui32RdOffset)
{
	stpIPC_Reg->cmdb.rd_offset = ui32RdOffset;
}

UINT32 IPC_REG_CMD_GetBaseAddr(void)
{
	return stpIPC_Reg->cmdb.base;
}

void IPC_REG_CMD_SetBaseAddr(UINT32 ui32Cmdptr)
{
	stpIPC_Reg->cmdb.base = ui32Cmdptr;
}

UINT32 IPC_REG_CMD_GetBufSize(void)
{
	return stpIPC_Reg->cmdb.size;
}

void IPC_REG_CMD_SetBufSize(UINT32 ui32CmdSize)
{
	stpIPC_Reg->cmdb.size = ui32CmdSize;
}

UINT32 IPC_REG_REQ_GetWrOffset(void)
{
	return stpIPC_Reg->reqb.wr_offset;
}

void IPC_REG_REQ_SetWrOffset(UINT32 ui32WrOffset)
{
	stpIPC_Reg->reqb.wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_REQ_GetRdOffset(void)
{
	return stpIPC_Reg->reqb.rd_offset;
}

void IPC_REG_REQ_SetRdOffset(UINT32 ui32RdOffset)
{
	stpIPC_Reg->reqb.rd_offset = ui32RdOffset;
}

UINT32 IPC_REG_REQ_GetBaseAddr(void)
{
	return stpIPC_Reg->reqb.base;
}

void IPC_REG_REQ_SetBaseAddr(UINT32 ui32Reqptr)
{
	stpIPC_Reg->reqb.base = ui32Reqptr;
}

UINT32 IPC_REG_REQ_GetBufSize(void)
{
	return stpIPC_Reg->reqb.size;
}

void IPC_REG_REQ_SetBufSize(UINT32 ui32ReqSize)
{
	stpIPC_Reg->reqb.size = ui32ReqSize;
}

UINT32 IPC_REG_DBG_GetWrOffset(void)
{
	return stpIPC_Reg->dbgb.wr_offset;
}

void IPC_REG_DBG_SetWrOffset(UINT32 ui32WrOffset)
{
	stpIPC_Reg->dbgb.wr_offset = ui32WrOffset;
}

UINT32 IPC_REG_DBG_GetRdOffset(void)
{
	return stpIPC_Reg->dbgb.rd_offset;
}

void IPC_REG_DBG_SetRdOffset(UINT32 ui32RdOffset)
{
	stpIPC_Reg->dbgb.rd_offset = ui32RdOffset;
}

UINT32 IPC_REG_DBG_GetBaseAddr(void)
{
	return stpIPC_Reg->dbgb.base;
}

void IPC_REG_DBG_SetBaseAddr(UINT32 ui32DBGptr)
{
	stpIPC_Reg->dbgb.base = ui32DBGptr;
}

UINT32 IPC_REG_DBG_GetBufSize(void)
{
	return stpIPC_Reg->dbgb.size;
}

void IPC_REG_DBG_SetBufSize(UINT32 ui32DBGSize)
{
	stpIPC_Reg->dbgb.size = ui32DBGSize;
}


UINT32 IPC_REG_VES_GetDb(void)
{
	return stpIPC_Reg->ves_db;
}

void IPC_REG_VES_SetDb(UINT32 ves_db)
{
	stpIPC_Reg->ves_db = ves_db;
}

UINT32 IPC_REG_AUIB_GetDb(void)
{
	return stpIPC_Reg->auib_db;
}

void IPC_REG_AUIB_SetDb(UINT32 auib_db)
{
	stpIPC_Reg->auib_db = auib_db;
}

UINT32 IPC_REG_CPB_GetDb(void)
{
	return stpIPC_Reg->cpb_db;
}

void IPC_REG_CPB_SetDb(UINT32 cpb_db)
{
	stpIPC_Reg->cpb_db = cpb_db;
}

UINT32 IPC_REG_VDC_GetDb(void)
{
	return stpIPC_Reg->vdc_db;
}

void IPC_REG_VDC_SetDb(UINT32 vdc_db)
{
	stpIPC_Reg->vdc_db = vdc_db;
}

UINT32 IPC_REG_DPB_GetDb(void)
{
	return stpIPC_Reg->dpb_db;
}

void IPC_REG_DPB_SetDb(UINT32 dpb_db)
{
	stpIPC_Reg->dpb_db = dpb_db;
}

UINT32 IPC_REG_LIPSYNC_GetDb(void)
{
	return stpIPC_Reg->lipsync_db;
}

void IPC_REG_LIPSYNC_SetDb(UINT32 lipsync_db)
{
	stpIPC_Reg->lipsync_db = lipsync_db;
}

UINT32 IPC_REG_RATE_GetDb(void)
{
	return stpIPC_Reg->rate_db;
}

void IPC_REG_RATE_SetDb(UINT32 rate_db)
{
	stpIPC_Reg->rate_db = rate_db;
}

UINT32 IPC_REG_PTS_GetDb(void)
{
	return stpIPC_Reg->pts_db;
}

void IPC_REG_PTS_SetDb(UINT32 pts_db)
{
	stpIPC_Reg->pts_db = pts_db;
}

UINT32 IPC_REG_DE_IF_GetDb(void)
{
	return stpIPC_Reg->de_if_db;
}

void IPC_REG_DE_IF_SetDb(UINT32 de_if_db)
{
	stpIPC_Reg->de_if_db = de_if_db;
}

UINT32 IPC_REG_DE_SYNC_GetDb(void)
{
	return stpIPC_Reg->de_sync_db;
}

void IPC_REG_DE_SYNC_SetDb(UINT32 de_sync_db)
{
	stpIPC_Reg->de_sync_db = de_sync_db;
}

UINT32 IPC_REG_RAMLOG_GetDb(void)
{
	return stpIPC_Reg->ramlog_db;
}

void IPC_REG_RAMLOG_SetDb(UINT32 ramlog_db)
{
	stpIPC_Reg->ramlog_db = ramlog_db;
}

UINT32 IPC_REG_DISPQ_GetDb(void)
{
	return stpIPC_Reg->dispq_db;
}

void IPC_REG_DISPQ_SetDb(UINT32 dispq_db)
{
	stpIPC_Reg->dispq_db = dispq_db;
}

UINT32 IPC_REG_DISPCLEARQ_GetDb(void)
{
	return stpIPC_Reg->dispclearq_db;
}

void IPC_REG_DISPCLEARQ_SetDb(UINT32 dispclearq_db)
{
	stpIPC_Reg->dispclearq_db = dispclearq_db;
}


UINT32 IPC_REG_Get_McuSwVer(void)
{
	return stpIPC_Reg->mcu_sw_ver;
}

void IPC_REG_Set_McuSwVer(UINT32 mcu_sw_ver)
{
	stpIPC_Reg->mcu_sw_ver = mcu_sw_ver;
}

UINT32 IPC_REG_Get_WatchdogCnt(void)
{
	return stpIPC_Reg->watchdog_cnt;
}

void IPC_REG_Set_WatchdogCnt(UINT32 watchdog_cnt)
{
	stpIPC_Reg->watchdog_cnt = watchdog_cnt;
}


/** @} */

