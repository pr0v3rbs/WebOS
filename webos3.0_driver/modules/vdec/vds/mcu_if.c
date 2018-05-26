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
 * main driver implementation for MCU I/F.
 *
 * author     taewoong.kim@lge.com
 * version    1.0
 * date       2013.08.09
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define LOG_NAME	vdec_mcuif
#define LOG_LEVEL	log_level_error

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#ifdef __XTENSA__
#include <stdio.h>
#include <string.h>
#else
#include <linux/kernel.h>
#include <asm/string.h>	// memset
#endif

#include "../hal/top_hal_api.h"
#include "../hal/vdec_base.h"
#include "hma_alloc.h"

#include "mcu_if.h"
#include "mmcu_pipe.h"

#include "log.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define CMD_MPIPE_SIZE	0x1000
#define REQ_MPIPE_SIZE	0x1000

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

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
static MMCU_PIPE_T			*gpstMpipeCmd;
static MMCU_PIPE_T			*gpstMpipeReq;
static MMCU_PIPE_ENTRY_T	gstPipeEntry;
static BOOLEAN				gbValidReqEntry = FALSE;


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

// TODO : add callback fn registeration & get rid of reverse reference!!
extern void _deif_callback(UINT8 u8DeIfCh, MMCU_PIPE_CMD eType, UINT8* pu8Data);
void _mpipe_recieve_callback(MMCU_PIPE_T *pstPipe)
{
	union pipe_data {
		S_DISPQ_BUF_T	stDispBuf;
		VSYNC_INFO_T	stVsyncInfo;
	} data;

	UINT8			u8DeIfCh;

re_pop:

	if ( gbValidReqEntry == FALSE )
	{
		if ( MPIPE_GetUsedSize(gpstMpipeReq) < sizeof(MMCU_PIPE_ENTRY_T) )
			return;

		MPIPE_Read(gpstMpipeReq, (UINT8*)&gstPipeEntry,
				sizeof(MMCU_PIPE_ENTRY_T));
		gbValidReqEntry = TRUE;
	}

	if ( gbValidReqEntry == TRUE )
	{
		//printk("used %d   req %d\n", MPIPE_GetUsedSize(gpstMpipeReq) , gstPipeEntry.size);
		if ( MPIPE_GetUsedSize(gpstMpipeReq) < gstPipeEntry.size )
			return;

		u8DeIfCh = (UINT8)gstPipeEntry.u32Param[0];
		MPIPE_Read(gpstMpipeReq, (UINT8*)&data, gstPipeEntry.size);
		gbValidReqEntry = FALSE;


		_deif_callback(u8DeIfCh, gstPipeEntry.eType, (UINT8*)&data);
	}

	goto re_pop;

	return;
}

void MCU_IF_Init(void)
{
	UINT8	*pu8CmdMpipe, *pu8ReqMpipe;

	log_noti("[MCU_IF] MCU I/F Init\n");

	pu8CmdMpipe = (UINT8*)hma_alloc_user("vdec", CMD_MPIPE_SIZE, 1<<8, "mpipe_cmd");
	if ( pu8CmdMpipe == NULL )
	{
		log_error("CMD MPIPE mem alloc fail!\n");
	}

	pu8ReqMpipe = (UINT8*) hma_alloc_user("vdec", REQ_MPIPE_SIZE, 1<<8, "mpipe_req");
	if ( pu8ReqMpipe == NULL )
	{
		log_error("REQ MPIPE mem alloc fail!\n");
	}

	if ( pu8CmdMpipe != NULL )
		gpstMpipeCmd = MPIPE_MakePipe("cmd", pu8CmdMpipe, CMD_MPIPE_SIZE);

	if ( pu8ReqMpipe != NULL )
		gpstMpipeReq = MPIPE_MakePipe("req", pu8ReqMpipe, REQ_MPIPE_SIZE);

	MPIPE_RegisterCallback(gpstMpipeReq, _mpipe_recieve_callback);

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
void _mpipe_sel_vsync(UINT8 u8DeIfCh, UINT8 u8VsyncCh)
{
	UINT32 u32Empty;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pipe full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_SEL_VSYNC;
	stPipeEntry.size = 0;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;
	stPipeEntry.u32Param[1] = (UINT32)u8VsyncCh;
	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));

	MPIPE_Flush(gpstMpipeCmd);
}

void _mpipe_send_init_ch(UINT8 u8DeIfCh)
{
	UINT32 u32Empty;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pipe full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_INIT_CH;
	stPipeEntry.size = 0;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;

	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));
	MPIPE_Flush(gpstMpipeCmd);
}

void _mpipe_send_del_ch(UINT8 u8DeIfCh)
{
	UINT32 u32Empty;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pipe full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_SEL_VSYNC;
	stPipeEntry.size = 0;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;
	stPipeEntry.u32Param[1] = (UINT32)0xFF;
	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));

	MPIPE_Flush(gpstMpipeCmd);
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
void _mpipe_send_frame(UINT8 u8DeIfCh, S_DISPQ_BUF_T *pstDispBuf)
{
	UINT32 u32Empty;
	UINT32 u32Len;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Len = sizeof(S_DISPQ_BUF_T);
	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < u32Len + sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pip full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MMCU_PIPE_SET_NEWFRAME;
	stPipeEntry.size = u32Len;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;
	//stPipeEntry.u32Param[1] = (UINT32)DeCh;

	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));
	MPIPE_Write(gpstMpipeCmd, (UINT8*)pstDispBuf, u32Len);
	MPIPE_Flush(gpstMpipeCmd);

	return;
}

void _mpipe_send_set_dualout(UINT8 u8MirrorSrc, UINT8 u8MirrorPort)
{
	UINT32 u32Empty;
	UINT32 u32Len;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Len = 0;
	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < u32Len + sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pip full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_SET_DUALOUT;
	stPipeEntry.size = u32Len;
	stPipeEntry.u32Param[0] = (UINT32)u8MirrorSrc;
	stPipeEntry.u32Param[1] = (UINT32)u8MirrorPort;

	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));
	MPIPE_Flush(gpstMpipeCmd);

	return;
}

void _mpipe_send_set_repeat(UINT8 u8DeIfCh, UINT8 u8nTimes)
{
	UINT32 u32Empty;
	UINT32 u32Len;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Len = 0;
	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < u32Len + sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pip full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_SET_REPEAT;
	stPipeEntry.size = u32Len;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;
	stPipeEntry.u32Param[1] = (UINT32)u8nTimes;

	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));
	MPIPE_Flush(gpstMpipeCmd);

	return;
}

void _mpipe_send_target(UINT8 u8DeIfCh, UINT8 u8TargetDe)
{
	UINT32 u32Empty;
	UINT32 u32Len;
	MMCU_PIPE_ENTRY_T stPipeEntry;

	u32Len = 0;
	u32Empty = MPIPE_GetEmptySize(gpstMpipeCmd);
	if ( u32Empty < u32Len + sizeof(MMCU_PIPE_ENTRY_T))
	{
		MPIPE_Flush(gpstMpipeCmd);
		log_error("mcu pip full!\n");
		return;
	}

	stPipeEntry.delimiter = 0xABABABAB;
	stPipeEntry.eType = MPIPE_SEL_DE;
	stPipeEntry.size = u32Len;
	stPipeEntry.u32Param[0] = (UINT32)u8DeIfCh;
	stPipeEntry.u32Param[1] = (UINT32)u8TargetDe;

	MPIPE_Write(gpstMpipeCmd, (UINT8*)&stPipeEntry, sizeof(MMCU_PIPE_ENTRY_T));
	MPIPE_Flush(gpstMpipeCmd);

	return;
}
