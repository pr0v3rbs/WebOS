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


/** @file apr_reg_h13a0.h
 *
 *  driver header for 3d format auto detection. ( used only within kdriver )
 *	
 *	@author		justine.jeong
 *	@version	0.1
 *	@note		
 *	@date		2012.07.10
 *	@see		
 */

#ifndef	_APR_REG_M14_H_
#define	_APR_REG_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

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
int APR_M14_Init(void);
int APR_M14_Uninit(void);
int APR_M14_Cleanup(void);
int APR_M14_InitHardware(void);
int APR_M14_CollectBasicData(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);
int APR_M14_GetOVInfo(APR_CFG_T *apr_cfg, APR_CAP_SRC_T ov_src, APR_OV_INFO_T *ov_info);
int APR_M14_GetHistInfo(APR_HST_INFO_T *hist_info);
int APR_M14_GetUSecTimerVal(UINT32 *tv);
int APR_M14_SetFd3SubtConfig(APR_CFG_T *apr_cfg, int alt_seg);
int APR_M14_SetFd3Config(APR_CFG_T *apr_cfg);
int APR_M14_CollectFd3Data(APR_DTA_T *pData);
int APR_M14_DetectBlackBar(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);
int APR_M14_SetCurrentInfo(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _APR_REG_M14_H_ */

