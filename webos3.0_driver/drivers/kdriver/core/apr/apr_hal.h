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
 *  hal header file for APR device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.06
 *  note		Additional information.
 *
 *  @addtogroup APR
 *	@{
 */

#ifndef _APR_HAL_H_
#define _APR_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"

#ifdef  __cplusplus
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
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int APR_HAL_Init(void);
int APR_HAL_Uninit(void);
int APR_HAL_InitHardware(void);
int APR_HAL_CollectBasicData(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);
int APR_HAL_GetOVInfo(APR_CFG_T *apr_cfg, APR_CAP_SRC_T ov_src, APR_OV_INFO_T *ov_info);
int APR_HAL_GetHistInfo(APR_HST_INFO_T *hist_info);
int APR_HAL_GetUSecTimerVal(UINT32 *tv);
int APR_HAL_SetFd3Config(APR_CFG_T *apr_cfg);
int APR_HAL_CollectFd3Data(APR_DTA_T *apr_data);
int APR_HAL_DetectBlackBar(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);
int APR_HAL_SetFd3SubtConfig(APR_CFG_T *apr_cfg, int alt_seg);
int APR_HAL_SetCurrentInfo(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);
int APR_HAL_GetCapacity(APR_CAPA_INFO_T *capa);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _APR_HAL_H_ */

/** @} */




