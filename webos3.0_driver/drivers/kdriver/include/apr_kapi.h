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


/** @file apr_kapi.h
 *
 *	@author	    justine.jeong	
 *	@version	0.1
 *	@note		
 *	@date		2012.07.04
 *	@see		
 */

#ifndef	_APR_KAPI_H_
#define	_APR_KAPI_H_

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
#define	APR_IOC_MAGIC		'Z'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define APR_IOW_INITIALIZE          		_IOW(APR_IOC_MAGIC, 0, LX_APR_INIT_T)
#define APR_IOW_SETCONTROL             		_IOW(APR_IOC_MAGIC, 1, LX_APR_CONTROL_T)
#define APR_IOR_GETINFO             		_IOW(APR_IOC_MAGIC, 2, LX_APR_INFO_T)
#define APR_IOW_SETCOMMAND             		_IOW(APR_IOC_MAGIC, 3, LX_APR_COMMAND_T)

#define APR_IOC_MAXNR						10
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions (Common)
----------------------------------------------------------------------------------------*/

/**
 *	window index enumeration
 */
typedef enum {
	LX_APR_WIN_0 = 0,
	LX_APR_WIn_1
}
LX_APR_WIN_ID;

/**
 *	format type enumeration
 */
typedef enum {
	LX_APR_FMT_2D_2D = 0,
	LX_APR_FMT_3D_CB,// = LX_APR_FMT_3D_3D,
	LX_APR_FMT_3D_CC,
	LX_APR_FMT_3D_LL,
	LX_APR_FMT_3D_SS,
	LX_APR_FMT_3D_TB,
	LX_APR_FMT_3D_3D,
	LX_APR_FMT_UNKNOWN,
	LX_APR_FMT_MAX
}
LX_APR_FORMAT;

/**
 *	debug type enumeration
 */
typedef enum {
	LX_APR_CMD_OFF,     // 0
	LX_APR_CMD_ON,
	LX_APR_CMD_PATTERN,
	LX_APR_CMD_OSD_LOG,
	LX_APR_CMD_OSD_GRF,
	LX_APR_CMD_LOG,     // 5
	LX_APR_CMD_SIM_DATA,
	LX_APR_CMD_FIX_2D,
	LX_APR_CMD_FIX_3D,
	LX_APR_CMD_FIX_FULL_WIN,	
	LX_APR_CMD_STRING,
	LX_APR_CMD_SIM_SCRIPT,
	LX_APR_CMD_MAX
}
LX_APR_CMD;

/**
 *	special hidden command enumeration
 */
typedef enum {
	LX_APR_HCMD_INIT,
	LX_APR_HCMD_GET_INTERNAL_STATE,
	LX_APR_HCMD_DUMP_SCRIPT_DATA,
	LX_APR_HCMD_DUMP_EXCEL_DATA,
	LX_APR_HCMD_DUMP_FD3,
	LX_APR_HCMD_DUMP_HIST,
	LX_APR_HCMD_DUMP_INFO,
	LX_APR_HCMD_MAX
}
LX_APR_HIDDEN_CMD;

/**
 *	histogram debug type enumeration
 */
typedef enum {
	LX_APR_HST_OFF,
	LX_APR_HST_ON,
	LX_APR_HST_FMT,
	LX_APR_HST_RAW_DATA,
	LX_APR_HST_DUMP,
	LX_APR_HST_MAX
}
LX_APR_HST;

/**
 *	capture debug type enumeration
 */
typedef enum {
	LX_APR_CAP_OFF,
	LX_APR_CAP_ON,
	LX_APR_CAP_TIME,
	LX_APR_CAP_FMT,
	LX_APR_CAP_LOG,
	LX_APR_CAP_DRAW,
	LX_APR_CAP_DRW3,
	LX_APR_CAP_MAX
}
LX_APR_CAP;

/**
 *	dual screen format type enumeration
 */
typedef enum {
	LX_APR_DSC_TYPE_2D,
	LX_APR_DSC_TYPE_SS,
	LX_APR_DSC_TYPE_TB,
	LX_APR_DSC_TYPE_MAX
}
LX_APR_DSC_TYPE_T;

/**
 *	fd3 debug type enumeration
 */
typedef enum {
	LX_APR_FD3_OFF,
	LX_APR_FD3_ON,
	LX_APR_FD3_FMT,
	LX_APR_FD3_LOG,
	LX_APR_FD3_BBD,
	LX_APR_FD3_MAX
}
LX_APR_FD3;

/***************************************************************************/
/* ETC : detection 3d */
/***************************************************************************/
typedef struct {
	char str_buf[64];
	UINT32 x;
	UINT32 y;
}
LX_APR_CMD_STR_T;

/**
 *	pe d3d frame capture info type
 */
typedef struct {
	LX_APR_CAP cap_enb;
	UINT32 h_line_num;
	UINT32 v_line_num;
	UINT32 h_line_width;
	UINT32 v_line_width;
} LX_APR_CAP_INFO_T;

/**
 *	pe d3d setting control parameter type
 */
typedef struct
{
	LX_APR_CMD cmd;
	UINT32 sel_alg;
	LX_APR_CAP cap_enb;
	LX_APR_FD3 fd3_enb;
	LX_APR_FORMAT fmt;
	LX_APR_CMD_STR_T str;
} LX_APR_CONTROL_T;

/**
 *	pe d3d information control parameter type
 */
typedef struct
{
	BOOLEAN valid;
	LX_APR_FORMAT apr_inf;
	LX_APR_DSC_TYPE_T dsc_type; // dual screen info from hardware detection block
} LX_APR_INFO_T;

/**
 * auto pattern recognition.
 */
typedef struct {
	UINT32 temp;
}
LX_APR_INIT_T;

/**
 * set hidden command for debug.
 */
typedef struct {
	LX_APR_HIDDEN_CMD cmd;
}
LX_APR_COMMAND_T;


#endif //#ifndef	_APR_KAPI_H_
