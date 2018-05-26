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
 *  register access interface for the LG1150 hardware pvr
 *
 *  author		Murugan Durairaj (murugan.d@lge.com)
 *  author		modified by ki beom kim (kibeom.kim@lge.com)
 *  version		1.1
 *  date		2010.03.20
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef _PVR_REG_H_
#define _PVR_REG_H_


/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "pvr_kapi.h"

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

int		PVR_REG_Init(void);

void	PVR_REG_EnableIntr(UINT32 bits, BOOLEAN enable);

void	DVR_DN_EnableReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_DN_GetBufBoundReg( LX_PVR_CH_T ch, UINT32 *base, UINT32 *end );
void	DVR_DN_GetWritePtrReg( LX_PVR_CH_T ch, UINT32 *wptr);
void	DVR_DN_SetWritePtrReg(LX_PVR_CH_T ch, UINT32 wptr);
void	DVR_DN_ResetBlock(LX_PVR_CH_T ch);
void	DVR_DN_SetBufBoundReg(LX_PVR_CH_T ch, UINT32 base, UINT32 end);
void	DVR_DN_EnableINTReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_DN_ConfigureIntrLevel(LX_PVR_CH_T ch ,UINT32 bufNumLim, UINT32 pktNumLim );
void	DVR_DN_GetPacketBufLimit(LX_PVR_CH_T ch ,UINT32 *pktNumLim, UINT32 *bufNumLim );
void	DVR_DN_GetBufInfo(LX_PVR_CH_T ch, UINT32 *buf_num, UINT32 *pkt_cnt);
void	DVR_DN_PauseReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_DN_SetPIDReg(LX_PVR_CH_T ch, UINT32 pid);


void	DVR_UP_EnableReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_UP_SetNumOfSyncError(LX_PVR_CH_T ch, UINT8 num);
void	DVR_UP_SetInputTSMode(LX_PVR_CH_T ch, UINT8 packet_len);
void	DVR_UP_CopyTimeStamp(LX_PVR_CH_T ch);
void	DVR_UP_AutoTimeStampCopyReg(LX_PVR_CH_T ch ,BOOLEAN enable);
BOOLEAN	DVR_UP_GetEnable(LX_PVR_CH_T ch);
BOOLEAN	DVR_UP_GetPause(LX_PVR_CH_T ch);
void	DVR_UP_SetSpeedReg(LX_PVR_CH_T ch ,SINT32 speed);
void	DVR_UP_GetBufBoundReg(LX_PVR_CH_T ch, UINT32 *base, UINT32 *end);
void	DVR_UP_GetPointersReg(LX_PVR_CH_T ch, UINT32 *wptr, UINT32 *rptr);
void	DVR_UP_SetWritePtrReg(LX_PVR_CH_T ch, UINT32 wptr);
void	DVR_UP_SetReadPtrReg(LX_PVR_CH_T ch, UINT32 rptr);
void	DVR_UP_ResetBlock(LX_PVR_CH_T ch);
void	DVR_UP_EnableEmptyLevelReg(LX_PVR_CH_T ch, BOOLEAN enable);
void	DVR_UP_EnableErrIntr(LX_PVR_CH_T ch, BOOLEAN enable);
void	DVR_UP_PauseReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_UP_RepPauseReg(LX_PVR_CH_T ch ,BOOLEAN enable, UINT32 pptr);
void	DVR_UP_SetEmptyLevelReg(LX_PVR_CH_T ch, UINT32 level);
void	DVR_UP_SetBufBoundReg(LX_PVR_CH_T ch, UINT32 base, UINT32 end);
void	DVR_UP_SetIntrAck(LX_PVR_CH_T ch, UINT8 value);
void	DVR_UP_SetWaitCycle (LX_PVR_CH_T ch, UINT16 waitCycle);
void	DVR_UP_SetTimeStampClkSource(LX_PVR_CH_T ch, UINT32 fixed_clk);
UINT32	DVR_UP_GetErrorStat(LX_PVR_CH_T ch);
void	DVR_UP_SetTimeStampCheck(LX_PVR_CH_T ch ,BOOLEAN disable);
void	DVR_UP_ChangePlaymode(LX_PVR_CH_T ch , UINT8 playMode );
void	DVR_UP_GetTSCJitterReg(LX_PVR_CH_T ch, UINT32 *jitter);
UINT32	DVR_UP_GetStat0(LX_PVR_CH_T ch);


void	DVR_PIE_EnableINTReg(LX_PVR_CH_T ch ,BOOLEAN enable);
void	DVR_PIE_SetModeReg(LX_PVR_CH_T ch, UINT32 mode);
void	DVR_PIE_ConfigureSCD(LX_PVR_CH_T ch, UINT8 index, UINT32 mask, UINT32 value, UINT8 enable );
void	DVR_PIE_GscdByteInfoConfig(LX_PVR_CH_T ch, UINT8 byteSel0, UINT8 byteSel1, UINT8 byteSel2, UINT8 byteSel3 );
void	DVR_PIE_EnableSCDReg(LX_PVR_CH_T ch, BOOLEAN enable);
void	DVR_PIE_SetIntrAck(LX_PVR_CH_T ch);
void	DVR_PIE_ResetBlock(LX_PVR_CH_T ch);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_IO_H_ */

/** @} */
