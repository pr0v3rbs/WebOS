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
 *
 *  author		won.hur (won.hur@lge.com)
 *  version		1.0
 *  date		2012.04.18
 *  note		Additional information.
 *
 *  @addtogroup lg115x_vbi
 *	@{
 */

#ifndef	_VBI_CFG_H_
#define	_VBI_CFG_H_

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
#define	VBI_MODULE			"vbi"
#define VBI_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/* Structure for VBI H/W FIXED Buffer Memory */
typedef struct
{
	char*       		vbi_register_name;
	unsigned int        vbi_register_base;
	unsigned int        vbi_register_size;
} LX_VBI_REG_CFG_T;

/* Structure for S/W Ring Buffer Memory */
typedef struct
{
	char*       		buf_memory_name;
	unsigned int        buf_memory_base;
	unsigned int        buf_memory_size;
} LX_VBI_MEM_CFG_T;

extern LX_VBI_MEM_CFG_T* gstMemCfgVBI;
extern LX_VBI_MEM_CFG_T gstBufMemCfgVBIRev[] ;
extern LX_VBI_REG_CFG_T* gstRegCfgVBI;
extern LX_VBI_REG_CFG_T gstVbiRegCfgVBIRev[] ;
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VBI_CFG_H_ */

/** @} */

