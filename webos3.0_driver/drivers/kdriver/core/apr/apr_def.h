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
 *  definition header file for APR device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.06
 *  note		Additional information.
 *
 *  @addtogroup APR
 *	@{
 */

#ifndef _APR_DEF_H_
#define _APR_DEF_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#ifdef INCLUDE_H15_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H15
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H14
//#define _DISABLE_ACCESS_TO_PE_
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
#define USE_KDRV_CODES_FOR_M14
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
#define USE_KDRV_CODES_FOR_H13
#endif

#ifdef INCLUDE_L9_CHIP_KDRV
//#define USE_KDRV_CODES_FOR_L9  // 3D pattern detect -> d3d of pe/util (gp4: pe_d3d.c/pe_d3d_alg.c)
#endif

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
	
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define APR_ERROR(format, args...)		DBG_PRINT( g_apr_debug_fd,         \
													LX_LOGM_LEVEL_ERROR,   \
													format,                \
													##args )
#define APR_WARN(format, args...)		DBG_PRINT( g_apr_debug_fd,         \
													LX_LOGM_LEVEL_WARNING, \
													format,                \
													##args )
#define APR_PRINT(format, args...)		DBG_PRINT( g_apr_debug_fd,         \
													LX_LOGM_LEVEL_INFO,    \
													format,                \
													##args )
#define APR_NOTI(format, args...)		DBG_PRINT( g_apr_debug_fd,         \
													LX_LOGM_LEVEL_NOTI,    \
													format,                \
													##args )
#define APR_TRACE(format, args...)		DBG_PRINT( g_apr_debug_fd,         \
													LX_LOGM_LEVEL_TRACE,   \
													format,                \
													##args )

#define CHECK_KNULL(ptr)  if(!ptr) { APR_ERROR("%s: %s is null\n", __F__, #ptr);\
									 ret = RET_ERROR; break; }
#define BREAK_WRONG(val) { APR_ERROR("%s: break wrong %s(0x%x)\n", __F__, #val, val);\
									 ret = RET_ERROR; break; }
#define FMT_PTN_STR(fmt,ptn,str1,str2)      \
    switch (fmt)                            \
    {                                       \
	    case LX_APR_FMT_2D_2D :             \
	    	ptn = APR_OSD_2D;               \
		    str1  = "2d";                   \
		    str2  = "2d";                   \
		    break;                          \
	    case LX_APR_FMT_3D_3D :             \
	    	ptn = APR_OSD_MAX;              \
		    str1  = "3d";                   \
		    str2  = "3d";                   \
		    break;                          \
		case LX_APR_FMT_3D_SS :             \
    		ptn = APR_OSD_SS;               \
    		str1  = "side by side";         \
    		str2  = "ss";                   \
    		break;                          \
	    case LX_APR_FMT_3D_TB :             \
	    	ptn = APR_OSD_TB;               \
	    	str1  = "top and bottom";       \
	    	str2  = "tb";                   \
	    	break;                          \
    	case LX_APR_FMT_3D_CB :             \
	    	ptn = APR_OSD_CB;               \
	    	str1  = "checker board";        \
	    	str2  = "cb";                   \
	    	break;                          \
    	case LX_APR_FMT_3D_CC :             \
	    	ptn = APR_OSD_CC;               \
	    	str1  = "column by column";     \
	    	str2  = "cc";                   \
	    	break;                          \
    	case LX_APR_FMT_3D_LL :             \
	    	ptn = APR_OSD_LL;               \
    		str1  = "line by line";         \
    		str2  = "ll";                   \
	    	break;                          \
    	case LX_APR_FMT_UNKNOWN :           \
     	default :                           \
	    	ptn = APR_OSD_NONE;             \
	    	str1  = "unknown";              \
	    	str2  = "un";                   \
		    break;                          \
    }                                       \

#define OUT_TYPE_STR(out,str)               \
	switch (out)                            \
	{                                       \
		case LX_PE_OUT_3D_2D :              \
	    	str  = "3D-2D";                 \
			break;                          \
		case LX_PE_OUT_2D_3D :              \
	    	str  = "2D-3D";                 \
			break;                          \
		case LX_PE_OUT_UD :                 \
	    	str  = "UD";                    \
			break;                          \
		case LX_PE_OUT_TB :                 \
	    	str  = "SINGLE_TB";             \
			break;                          \
		case LX_PE_OUT_SS :                 \
	    	str  = "SINGLE_SS";             \
			break;                          \
		case LX_PE_OUT_FS :                 \
	    	str  = "SINGLE_FS";             \
			break;                          \
		case LX_PE_OUT_LA :                 \
	    	str  = "SINGLE_LA";             \
			break;                          \
		case LX_PE_OUT_DUAL_TB :            \
	    	str  = "DUAL_TB";               \
			break;                          \
		case LX_PE_OUT_DUAL_SS :            \
	    	str  = "DUAL_SS";               \
			break;                          \
		case LX_PE_OUT_DUAL_FULL :          \
	    	str  = "DUAL_FULL";             \
			break;                          \
		case LX_PE_OUT_PIP :                \
	    	str  = "PIP";                   \
			break;                          \
		case LX_PE_OUT_2D :                 \
		default :                           \
	    	str  = "2D";                    \
			break;                          \
	}                                       

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern	int		g_apr_debug_fd;

#endif /* _APR_DEF_H_ */

/** @} */
