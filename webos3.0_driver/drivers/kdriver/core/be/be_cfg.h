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
 *  main configuration header file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.02.07
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_CFG_H_
#define _BE_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define BE_MODULE		"be"
#define BE_MAX_DEVICE	1

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

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
#define BE_CH_MEM_DOWNLOAD_FW_MAX	5
#define BE_CH_MEM_IPC_MAX	1
#define BE_CH_MEM_FW_MAX BE_CH_MEM_DOWNLOAD_FW_MAX+BE_CH_MEM_IPC_MAX
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	char*	fw_name;	///< name of firmware
	UINT32	fw_base;	///< base address of firmware
	UINT32	fw_size;		///< size of firmware
}
BE_CH_MEM_T;

typedef struct
{
	BE_CH_MEM_T fw[BE_CH_MEM_FW_MAX];
	char*	frame_name;	///< name of frame memory
	UINT32	frame_base;	///< base of frame memory
	UINT32	frame_size;	///< size of frame memory
}
BE_MEM_CFG_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
void BE_InitCfg ( void );

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern BE_MEM_CFG_T gMemCfgBe[];

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _BE_CFG_H_ */

/** @} */



