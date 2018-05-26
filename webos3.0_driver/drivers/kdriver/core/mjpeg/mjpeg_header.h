/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  mjpeg header decoder interface header.
 *
 *  @author		youngwoo.jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.04.09
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

#ifndef	_MJPEG_HEADER_H_
#define	_MJPEG_HEADER_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include "base_types.h"
#include "mjpeg_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef enum {
	MJPEG_QUANTIZATION_TABLE	=	0,
	MJPEG_FRAME_HEADER			=	1,
	MJPEG_RESET_INTERVAL		=	2,
	MJPEG_HUFFMAN_TABLE 		=	3,
	MJPEG_SCAN_HEADER			=	4
} MJPEG_HEADER_LIST_T;

/*-----------------------------------------------------------------------------
	Generic Usage Functions
-----------------------------------------------------------------------------*/
UINT32 MJPEG_HDR_Decode(LX_MEM_BUF_T *i_pstBuffer,
								LX_MJPEG_HEADER_T *o_pstHeader, UINT8 *o_pui8CheckList);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MJPEG_HEADER_H_ */

/** @} */
