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
 *  fw loader hal header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.07
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_FWL_HAL_H_
#define _BE_FWL_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	int  index;
	char fwName[128];
	BOOLEAN	isMcuFw;	///< BOOLEAN for MCU firmware or not
}
BE_FWL_HAL_FW_FILE_T;

typedef struct {
	char index;
	int  size;
	char *pBuffer;
	BOOLEAN	isMcuFw;	///< BOOLEAN for MCU firmware or not
} BE_FWL_HAL_FW_T; 

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int BE_FWL_HAL_Init(void);
int BE_FWL_HAL_Close(void);
BE_FWL_HAL_FW_FILE_T *BE_FWL_HAL_FwBinInfo(UINT8 ucIndex);
BE_FWL_HAL_FW_T *BE_FWL_HAL_GetFwHeaderInfo(UINT8 ucIndex);
UINT32 BE_FWL_HAL_FwDownload(BE_FWL_HAL_FW_T *pstParams);
BOOLEAN BE_FWL_HAL_McuIsAlive(void);
UINT32 BE_FWL_HAL_GetFwMemBaseSize(UINT8 ucIndex);
int BE_FWL_HAL_FrcFwInit(void);
int BE_FWL_HAL_SreFwInit(UINT32 u32FwBaseAddr);
int BE_FWL_HAL_WcgFwInit(UINT32 u32FwBaseAddr);
int BE_FWL_HAL_LedFwInit(UINT32 u32FwBaseAddr);
int BE_FWL_HAL_HdrFwInit(UINT32 u32FwBaseAddr);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _BE_FWL_HAL_H_ */

/** @} */








