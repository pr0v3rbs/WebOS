/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file pe_def.h
 *
 *  driver definition header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

#ifndef _PE_DEF_H_
#define _PE_DEF_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define USE_PE_REG_DEFINED_IN_DE

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_PRINT_COUNT_NUM		512
#define PE_TRACE_STR_SIZE		128
 
/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define GET_BM(value, mask) ((value) & (mask))
#define GET_CM(value, mask) (((value) & (mask)) == (mask))
#define GET_MASK(nd,wd)			    (((0xFFFFFFFF)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_BITS(val,nd,wd)			(((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_PVAL(val,nd,wd)			(GET_BITS(val,nd,wd)<<(nd))
#define GET_SVAL(val,nd,wd,sd)		(GET_BITS(val,nd,wd)<<(sd))
#define GET_SEXT(val,nd,wd)		    (((int)(GET_BITS(val,nd,wd)<<(32-(wd))))/(1<<(32-(wd))))
#define GET_PMSK(    nd,wd)			((((0xFFFFFFFF)>>(nd))&(0xFFFFFFFF>>(32-(wd)))) << (nd))
#define GET_RMSK(    nd,wd)			(~GET_PMSK(nd,wd))

#define TO_STR(str) #str
#define CHECK_NULL(ptr)  if (!ptr) { ffprintf("%s is Null\n", #ptr); ret = RET_ERROR; break; }
#define CHECK_KNULL(ptr) if (!ptr) { ffprintk("%s is Null\n", #ptr); ret = RET_ERROR; break; }
#define BREAK_WRONG(val)           { ffprintk("Wrong %s(0x%x)\n", #val, val); ret = RET_ERROR; break; }
#define BREAK_SETV(_r, _v) 		   { _r = _v; break; }

#define ffprintk(format, arg...)    printk("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)
#define ffprintq(format, arg...)    printk("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)
#define ffprintf(format, arg...)    printf("%s:%d:%s;" format , __FILE__ , __LINE__ , __FUNCTION__, ## arg)

#define REG_WD( addr, value )	    ( *( volatile UINT32 * )( addr )) = ( UINT32 )( value )
#define REG_RD( addr )	            ( *( volatile UINT32 * )( addr ))
#define DDR_WD( addr, value )	    ( *( UINT32 * )( addr ) ) = ( UINT32 )( value )
#define DDR_RD( addr )			    ( *( UINT32 * )( addr ) )

#define memset0(_var)		memset( &(_var), 0x00, sizeof(_var))
#define memset_0xff(_var)	memset( &(_var), 0xff, sizeof(_var))

#define PE_CONVHEX2DEC(hex,signbit)		\
	(((hex)&(1<<(signbit)))? ((hex)|(0xffffffff<<(signbit))):((hex)&(0xffffffff>>(31-(signbit)))))
#define PE_GET_MAX(x,y,z)			(((x)>=(y))? (((z)>=(x))? (z):(x)):(((z)>=(y))? (z):(y)))
#define PE_GET_MIN(x,y,z)			(((x)<=(y))? (((z)<=(x))? (z):(x)):(((z)<=(y))? (z):(y)))

#define PE_CHECK_WINID(_win_id)		\
	if ((_win_id)>LX_PE_WIN_NUM)	{ ffprintk("%s(%d) is wrong.\n",#_win_id,(_win_id)); ret = RET_ERROR; break; }
#define PE_CHECK_WIN0(_win_id)		(((_win_id)==LX_PE_WIN_0)||((_win_id)==LX_PE_WIN_ALL))
#define PE_CHECK_WIN1(_win_id)		(((_win_id)==LX_PE_WIN_1)||((_win_id)==LX_PE_WIN_ALL))
#define PE_GET_CHECKED_WINID(_win_id)	(((_win_id)==LX_PE_WIN_ALL)? LX_PE_WIN_0:(_win_id))

#define PE_CHECK_CODE(_checker, _action, fmt, args...)	\
	if (_checker){	PE_PRINT_ERROR(fmt, ##args);_action;	}

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif /* _PE_DEF_H_ */

