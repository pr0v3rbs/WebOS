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
 *  sdec driver
 *
 *  @author	Kyungseok Hyun ( ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014-08-14
 *  @note		Additional information.
 */



#ifndef _SDEC_CIPLUS_H_
#define _SDEC_CIPLUS_H_

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

int		SDEC_CIPLUS_Init(void);
void	SDEC_CIPLUS_Cleanup(void);

int		SDEC_CIPLUS_Suspend(void);
int		SDEC_CIPLUS_Resume(void);

int		SDEC_CIPLUS_AllocChannel(void);
int		SDEC_CIPLUS_OpenChannel(UINT8 idx);
int		SDEC_CIPLUS_CloseChannel(UINT8 idx);

int		SDEC_CIPLUS_ConfigureChannel(UINT8 idx, LX_SDEC_CIPLUS_CHANNEL_CONF_T *conf);
int		SDEC_CIPLUS_EnableChannel(UINT8 idx);
int		SDEC_CIPLUS_DisableChannel(UINT8 idx);


int		SDEC_CIPLUS_SetInputBuffer(UINT8 idx, UINT32 addr, UINT32 size);
int		SDEC_CIPLUS_SetOutputBuffer(UINT8 idx, UINT32 addr, UINT32 size);

int		SDEC_CIPLUS_GetInputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr);
int		SDEC_CIPLUS_GetOutputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr);

int		SDEC_CIPLUS_UpdateInputBufferWptr(UINT8 idx, UINT32 wptr);
int		SDEC_CIPLUS_UpdateOutputBufferRptr(UINT8 idx, UINT32 rptr);

int		SDEC_CIPLUS_AddPidFilter(UINT8 idx, UINT16 *pids, UINT32 npid);
int		SDEC_CIPLUS_RemovePidFilter(UINT8 idx, UINT16 *pids, UINT32 npid);
int		SDEC_CIPLUS_GetPidFilter(UINT8 idx, UINT16 *pids, UINT32 maxpid);

int		SDEC_CIPLUS_SetOutClock(LX_SDEC_CIPLUS_OUT_CLOCK_T clock);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

