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

#ifndef	_FBDEV_HW_CTX_H_
#define	_FBDEV_HW_CTX_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_hw.h"

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
	void*						chip_priv_ctx;	/* chip dependent ctx */

	struct
	{
		LX_FBDEV_TRID_CTRL_T	trid_ctrl;
	}
	fb[LX_FBDEV_ID_MAX];

	LX_FBDEV_CFG_T*	(*GetCfg)		(void);

	int		(*InitHW)				(void);
	int		(*ShutdownHW)			(void);
	int		(*RunSuspend)			(void);
	int		(*RunResume)			(void);

	int 	(*SetOSDEnable)         (int layer , int enable);
	int 	(*GetOSDEnable)         (int layer);

	int		(*InitOSDLayer)			(int layer);
	int 	(*WriteOSDHeader)       (struct scanvas  *pstCanvas);

	int 	(*GetZList)             (LX_FBDEV_ZLIST_T* zList );
	int 	(*SetZList)             (LX_FBDEV_ZLIST_T* zList );

	int 	(*WriteOSDEndian)       (int layer , ENDIAN_INFORM_T *endian);
	int 	(*WriteOSDCSC)          (int layer , CSC_INFORM_T *csc);

	int 	(*GetInterruptStatus)   (INTERRUPT_INFORM_T intr);
	int 	(*SetInterruptClear)    (INTERRUPT_INFORM_T intr);
	int 	(*SetInterruptEnable)   (INTERRUPT_INFORM_T intr , int enable);

	int		(*SetViewCtrl)			(LX_FBDEV_VIEW_CTRL_T* ctrl);
	int     (*SetConnCtrl)			(LX_FBDEV_CONN_CTRL_T* pConnCtrl);
	int		(*SetPivotCtrl)			(LX_FBDEV_PIVOT_CTRL_T* ctrl);
	int		(*SetTridCtrl)			(int layer, LX_FBDEV_TRID_CTRL_T* ctrl);

	int		(*SetVideoMixerCtrl)	(LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl);

	void	(*EventCallback)		(FBDEV_HW_EVENT_T ev, FBDEV_HW_EVENT_DATA_T* data );
}
FBDEV_HW_CTX_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	FBDEV_HW_CTX_T	g_fbdev_hw_ctx;

extern	FBDEV_HW_CTX_T*			FBDEV_GetHWCtx(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _FBDEV_HW_CTX_H_ */

