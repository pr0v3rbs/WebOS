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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     raxis.lim
 *  @version    1.0
 *  @date       2011-04-03
 *  @note       Additional information.
 */

#ifndef	_GFX_HAL_H_
#define	_GFX_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "gfx_hw.h"

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
typedef	struct
{
	const LX_GFX_CFG_T*	(*GetCfg)		(void);

	int		(*InitHW)					(void);
	int		(*ShutdownHW)				(void);
	int		(*RunSuspend)				(void);
	int		(*RunResume)				(void);

	int		(*RunFlushCommand)			(void);
	int		(*RunStartCommand)			(void);
	int		(*RunPauseCommand)			(void);
	int		(*RunResumeCommand)			(void);
	int		(*RunStopCommand)			(void);
	int		(*RunSwResetCommand)		(void);

	void	(*GetComQueueStatus)		(GFX_CMD_QUEUE_CTRL_T *queue);

	int		(*RunClkOnCommand)			(void);
	int		(*RunClkOffCommand)			(void);
	BOOLEAN (*ClkCheck)					(void);

	void 	(*GetOperationStatus)		(GFX_CMD_OP_T *operate);
	void 	(*SetOperationStatus)		(GFX_CMD_OP_T *operate);
	void 	(*GetInputConfigure)		(int iPort, GFX_PORT_CONFIGURE_T *port);
	void 	(*SetInputConfigure)		(GFX_PORT_CONFIGURE_T *port);
	void 	(*GetBlendConfigure)		(GFX_BLEND_CONFIGURE_T *blend);
	void 	(*SetBlendConfigure)		(GFX_BLEND_CONFIGURE_T *blend);
	void 	(*SetBlendingOut)			(GFX_ENUM_OUT_T type);
	void 	(*GetOutputConfigure)		(GFX_OUT_CONFIGURE_T *port);
	void 	(*SetOutputConfigure)		(GFX_OUT_CONFIGURE_T *port);
	void	(*SetScalerConfigure)		(GFX_SCALER_CONFIGURE_T* scaler);
	int 	(*SetCLUT)					(int port, int size , UINT32 *data);
	void	(*SetCommandDelayReg)		(UINT16 delay);
	UINT16	(*GetCommandDelayReg)		(void);

	BOOLEAN	(*IsGfxIdle)				(void);

	void	(*DumpRegister)				(void);
}
GFX_HAL_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	GFX_HAL_T	g_gfx_hal;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_HAL_H_ */

