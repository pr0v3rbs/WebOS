/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  main driver implementation for slt device.
 *	slt device will teach you how to make device driver with new platform.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *
 *  @addtogroup lg115x_slt
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kthread.h>

#include "proc_util.h"
#include "os_util.h"
#include "sys_regs.h"

#include "slt_kapi.h"
#include "slt_drv.h"
#include "slt_proc.h"
#include "slt_cfg.h"
#include "slt_hal.h"


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
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _SLT_PROC_Reader(UINT32 procId, char* buffer);
static int _SLT_PROC_Writer( UINT32 procId, char* command );

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_slt_device_proc_table[] =
{
	{ "author",			PROC_ID_AUTHOR	, OS_PROC_FLAG_READ },
	{ "command",		PROC_ID_COMMAND , OS_PROC_FLAG_WRITE },
	{ "w",				PROC_ID_WRITE	, OS_PROC_FLAG_WRITE },
	{ "r",				PROC_ID_READ	, OS_PROC_FLAG_WRITE },
	{ "version",		PROC_ID_VERSION	, OS_PROC_FLAG_READ },
	{ NULL,				PROC_ID_MAX		, 0 }
};



/*========================================================================================
	Implementation Group
========================================================================================*/

static int _SLT_PROC_Reader(UINT32 procId, char* buffer)
{
	int		ret= RET_ERROR;

	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "wunseo.yeo@lge.com" );
		}
		break;

		case PROC_ID_VERSION:
		{
			ret = sprintf( buffer, "Compiled : %s,  %s\n", __DATE__, __TIME__ );
		}
		break;


		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}



    return ret;
}

static int _SLT_PROC_Writer( UINT32 procId, char* command )
{

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			SLT_PRINT("command string : %s\n", command );
		}
		break;

		case PROC_ID_WRITE:
		{
		}
		break;

		case PROC_ID_READ:
		{
		}
		break;

		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}

void SLT_PROC_Init(void)
{
	OS_PROC_CreateEntryEx(SLT_MODULE, _g_slt_device_proc_table,
										_SLT_PROC_Reader,
										_SLT_PROC_Writer);
}

void SLT_PROC_Cleanup(void)
{
	OS_PROC_RemoveEntry(SLT_MODULE);
}

/** @} */

