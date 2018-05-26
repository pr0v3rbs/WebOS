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
 *  driver interface header for pvr device. ( used only within kdriver )
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  @author		Kyungseok Hyun(ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014.07.18
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef	_PVR_PIE_H_
#define	_PVR_PIE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pvr_core.h"

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

int PVR_PIE_Start(PVR_CTRL_T *ctrl);
int PVR_PIE_Stop(PVR_CTRL_T *ctrl);

int PVR_PIE_InitTable(PVR_CTRL_T *ctrl);
int PVR_PIE_SetType(PVR_CTRL_T *ctrl, LX_PVR_PIE_TYPE_T type);
//int PVR_PIE_GetTable(PVR_CTRL_T *ctrl, LX_PVR_BUFFER_T *ts_buf, LX_PVR_BUFFER_T *pie_buf);
int PVR_PIE_GetTable(PVR_CTRL_T *ctrl, LX_PVR_BUFFER_T *ts, LX_PVR_INDEX_T *index, UINT32 max_size);


int PVR_PIE_PutIndex(PVR_CTRL_T *ctrl, PVR_PIE_IND_T *index, UINT32 cnt);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_PIE_H_ */

/** @} */

