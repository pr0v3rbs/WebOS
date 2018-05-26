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
 *  mjpeg encode/decode interface header.
 *
 *  @author		youngwoo.jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.04.11
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

#ifndef	_MJPEG_CORE_H_
#define	_MJPEG_CORE_H_

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
#define QUANT_ELEMENT( s, tb )		(((tb)*(s) + 50)/100)

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Generic Usage Functions
-----------------------------------------------------------------------------*/
UINT32	MJPEG_ENC_Init(LX_MJPEG_IMAGE_INFO_T *i_pstImageInfo, LX_MJPEG_IMAGE_BUF_T *i_pstImageBuf, LX_MJPEG_OPTION_T *i_pstOption, LX_MJPEG_HEADER_T *o_pstHeader );
void	MJPEG_ENC_Start(LX_MEM_BUF_T *i_pstDataBuf);
UINT32	MJPEG_ENC_Continue(LX_MEM_BUF_T *i_pstDataBuf);
UINT32	MJPEG_ENC_End(void);
UINT32	MJPEG_DEC_Init(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_BUF_T *i_pstImageBuf, LX_MJPEG_OPTION_T *i_pstOption, LX_MJPEG_IMAGE_INFO_T *o_pstImageInfo );
void	MJPEG_DEC_Start(LX_MEM_BUF_T *i_pstDataBuf, UINT32 i_ui32DataLength, BOOLEAN bEndofStream);
void	MJPEG_DEC_Continue(LX_MEM_BUF_T *i_pstDataBuf, UINT32 i_ui32DataLength, BOOLEAN bEndofStream);
void	MJPEG_DEC_PartialContinue( UINT32 u32PartialBuffer );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MJPEG_CORE_H_ */

/** @} */

