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
 *  fw loader reg header file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2014.05.21
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_FWL_HW_H15_H_
#define _BE_FWL_HW_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "fwl_hal.h"

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
BE_FWL_HAL_FW_T *BE_FWL_HW_H15_GetFwHeaderInfo(UINT8 ucIndex);
UINT32 BE_FWL_HW_H15_FW_Download(UINT8 ucIndex, UINT8 *pucBuff, UINT32 ulSize, BOOLEAN bIsMcuFw);
BOOLEAN BE_FWL_HW_H15_MCUisAlive(void);
UINT32 BE_FWL_HW_H15_GetFwMemBaseSize(UINT8 ucIndex);
int BE_FWL_HW_H15_WcgFwInit(UINT32 u32FwBaseAddr);
int BE_FWL_HW_H15_LedFwInit(UINT32 u32FwBaseAddr);
int BE_FWL_HW_H15_DrcFwInit(UINT32 u32FwBaseAddr);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _BE_FWL_HW_H15_H_ */

/** @} */







