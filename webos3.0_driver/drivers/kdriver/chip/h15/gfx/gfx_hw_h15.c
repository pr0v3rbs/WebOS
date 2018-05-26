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
 *	GFX hw control
 *
 *  author      raxis.lim (raxis.lim@lge.com)
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg1150_gfx
 *  @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "gfx_impl.h"

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
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static	void CPC_InitClk(void)
{
	/* raxis.lim (2014/12/10) - diable GFX/GPU BW limiter
	 * - GPU/GFX BW workaround is not related to OSD local BW issue
	 * - some web app shows low performance
	 */
#if 0
	printk("INFO - H15 GFX BW (WR 700MB)\n");
	OS_WrReg(0xC880A2B0,0x64B540);	// BW CLK= 400MHz fixed
	OS_WrReg(0xC880A240,0xb20400);	// WR BW = 700MB
#endif

#if 0
	printk("INFO - H15 GPU BW (RD/WR 1.5GB)\n");
	OS_WrReg(0xC8C00040,0x1ec3);	// WR 1.5GB/sec
	OS_WrReg(0xC8C00044,0x1ec3);	// RD 1.5GB/sec
	OS_WrReg(0xC8C000B0,0x924 );	// sampling interval 5us
#endif
}

/*========================================================================================
	Implementation Group
========================================================================================*/
int GFX_H15_InitHW(void)
{
	CPC_InitClk();
	return RET_OK;
}

int	GFX_H15_ShutdownHW(void)
{
	return RET_OK;
}

void	GFX_H15_InitScaler(void)
{

}

int GFX_H15_RunFlushCommand(void)
{
	return RET_OK;
}

int GFX_H15_RunStartCommand(void)
{
	return RET_OK;
}

int GFX_H15_PauseCommand(void)
{
	return RET_OK;
}

int GFX_H15_ResumeCommand(void)
{
	return RET_OK;
}

int GFX_H15_SwResetCommand(void)
{
	return RET_OK;
}

int GFX_H15_StopCommand(void)
{
	return RET_OK;
}

void GFX_H15_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue)
{
	queue->status[0] = 0;
	queue->status[1] = 0;
	queue->status[2] = 0;
}

BOOLEAN GFX_H15_IsGfxIdle(void)
{
	return TRUE;
}

void GFX_H15_GetOperationStatus(GFX_CMD_OP_T *operate)
{

}

void GFX_H15_SetOperationStatus(GFX_CMD_OP_T *operate)
{

}

void GFX_H15_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port)
{

}

void GFX_H15_SetInputConfigure(GFX_PORT_CONFIGURE_T *port)
{

}

void GFX_H15_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{

}

void GFX_H15_SetBlendingOut(GFX_ENUM_OUT_T type)
{

}

void GFX_H15_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{

}

void GFX_H15_GetOutputConfigure(GFX_OUT_CONFIGURE_T *port)
{

}

void GFX_H15_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port)
{

}

void GFX_H15_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler)
{

}

void GFX_H15_SetInterruptMode(UINT32 uiMode)
{
	/* do nothing */
}

/* what is it ? */
void GFX_H15_SetAckMode(UINT32 *mode)
{

}

/* what is it ? */
void GFX_H15_GetAckMode(UINT32 *mode)
{
	*mode = 0;
}

int GFX_H15_SetCLUT(int port, int size , UINT32 *data)
{
	return RET_OK;
}

void GFX_H15_SetCommandDelayReg(UINT16 delay)
{

}

UINT16 GFX_H15_GetCommandDelayReg(void)
{
	return 0x0;
}

void GFX_H15_DumpRegister(void)
{

}

int GFX_H15_RunSuspend(void)
{
	return RET_OK;
}

int GFX_H15_RunResume(void)
{
	CPC_InitClk();
	return RET_OK;
}

int GFX_H15_ClkCheck(void)
{
	return TRUE;
}

int GFX_H15_RunClkOffCommand(void)
{
	return RET_OK;
}

int GFX_H15_RunClkOnCommand(void)
{
	return RET_OK;
}

/** @} */

