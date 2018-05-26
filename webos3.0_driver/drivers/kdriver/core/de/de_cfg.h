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
 *  main configuration file for de device
 *	de device will teach you how to make device driver with new platform.
 *
 *  author		dongho7.park (dongho7.park@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_de
 *	@{
 */

#ifndef	_DE_CFG_H_
#define	_DE_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

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
#define VIDEO_FIRMWARE_ROW_SIZE          (32)
#define VIDEO_ROW_STRIDE                 (16 * 1024)
#define CONV_ROW2BYTE(_r)                ((_r) * VIDEO_ROW_STRIDE)
#define CONV_BYTE2ROW(_r)				 (((_r) + VIDEO_ROW_STRIDE-1) / VIDEO_ROW_STRIDE)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	char*	fw_name;	///< name of firmware
	UINT32	fw_base;	///< base address of firmware
	UINT32	fw_size;	///< size of firmware
} LX_DE_CH_MEM_T;

typedef struct
{
	LX_DE_CH_MEM_T	fw[3];
	char*	frame_name;  ///< name of frame memory
	UINT32	frame_base;  ///< base of frame memory
	UINT32	frame_size;  ///< size of frame memory
} LX_DE_MEM_CFG_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int de_cfg_disply(void);
extern int de_cfg_get_fcmem(UINT32 *mem0, UINT32 *mem1, UINT32 *size);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_DE_MEM_CFG_T *gpDeMem, gMemCfgDe[];
extern LX_DE_CH_MEM_T gMemCfgDePreW[];
extern LX_DE_CH_MEM_T gMemCfgDeGrap[];
extern LX_DE_CH_MEM_T gMemCfgDeFW[];

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DE_CFG_H_ */

/** @} */

