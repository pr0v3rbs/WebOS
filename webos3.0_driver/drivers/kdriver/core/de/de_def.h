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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.02.23
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

#ifndef _DE_DEF_h
#define _DE_DEF_h

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define USE_DE_SWITCH_UART
#define USE_PQL_REG_DEFINED_IN_DE

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_CODEDDR
#define GET_BM(value, mask)         ((value) & (mask))
#define GET_CM(value, mask)         (((value) & (mask)) == (mask))
#define GET_MASK(nd,wd)			    (((0xFFFFFFFF)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_BITS(val,nd,wd)			(((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_PVAL(val,nd,wd)			(GET_BITS(val,nd,wd)<<(nd))
#define GET_SVAL(val,nd,wd,sd)		(GET_BITS(val,nd,wd)<<(sd))
#define GET_SEXT(val,nd,wd)		    (((int)(GET_BITS(val,nd,wd)<<(32-(wd))))/(1<<(32-(wd))))
#define GET_PMSK(    nd,wd)			((((0xFFFFFFFF)>>(nd))&(0xFFFFFFFF>>(32-(wd)))) << (nd))
#define GET_RMSK(    nd,wd)			(~GET_PMSK(nd,wd))

#define GET_RDIV(_x,_y)    			(((_x)+(_y)-1)/(_y))
#define GET_HDIV(_x,_y)    			(((_x)+(_y)/2)/(_y))
#define GET_RDUP(_x,_y)    			(GET_RDIV((_x),(_y))*(_y))

#define TO_STR(str) #str
#define CHECK_NULL(ptr)  if (!ptr) { ffprintf("%s is Null\n", #ptr); ret = RET_ERROR; break; }
#define CHECK_KNULL(ptr) if (!ptr) { ffprintk("%s is Null\n", #ptr); ret = RET_ERROR; break; }
#define BREAK_WRONG(val)           { ffprintk("Wrong %s(0x%x)\n", #val, val); ret = RET_ERROR; break; }
#define BREAK_SETV(_r, _v) 		   { _r = _v; break; }

#if 1
#define ffprintk(format, arg...)    DE_ERROR(format , ## arg)
#define ffprintq(format, arg...)    DE_ERROR(format , ## arg)
#define ffprintf(format, arg...)    DE_ERROR(format , ## arg)
#else // old macro
#define ffprintk(format, arg...)    printk("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)
#define ffprintq(format, arg...)    printk("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)
#define ffprintf(format, arg...)    printf("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)
#endif

#define REG_WD( addr, value )	    ( *( volatile UINT32 * )( addr )) = ( UINT32 )( value )
#define REG_RD( addr )	            ( *( volatile UINT32 * )( addr ))
#define DDR_WD( addr, value )	    ( *( UINT32 * )( addr ) ) = ( UINT32 )( value )
#define DDR_RD( addr )			    ( *( UINT32 * )( addr ) )


/* logm macro */
#define	DE_ERROR(format, args...)		DBG_PRINT(  g_de_debug_fd,        \
													LX_LOGM_LEVEL_ERROR,  \
													format,               \
													##args)
#define	DE_WARN(format, args...)		DBG_PRINT(  g_de_debug_fd,        \
													LX_LOGM_LEVEL_WARNING,\
													format,               \
													##args)
#define	DE_PRINT(format, args...)		DBG_PRINT(  g_de_debug_fd,        \
													LX_LOGM_LEVEL_INFO,   \
													format,               \
													##args)
#define	DE_NOTI(format, args...)		DBG_PRINT( g_de_debug_fd,         \
													LX_LOGM_LEVEL_NOTI,   \
													format,               \
													##args)
#define	DE_TRACE(format, args...)		DBG_PRINT( g_de_debug_fd,         \
													LX_LOGM_LEVEL_TRACE,  \
													format,               \
													##args)


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/
extern	int		g_de_debug_fd;

#endif

/**  @} */

