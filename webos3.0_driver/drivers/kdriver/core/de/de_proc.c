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
 *  Linux proc interface for de device.
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

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include "de_drv.h"
#include "de_def.h"
#include "proc_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_COMMAND,
	PROC_ID_MAX,
};

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_de_device_proc_table[] =
{
	{ "author",		PROC_ID_AUTHOR  , OS_PROC_FLAG_READ },
	{ "command",	PROC_ID_COMMAND , OS_PROC_FLAG_WRITE },
	{ NULL, 		PROC_ID_MAX		, 0 }
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * read_proc implementation of de device 
 *
*/
static int	_DE_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "dongho7.park (dongho7.park@lge.com)" );
		}
		break;	
		
		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}
		
	return ret;
}

/*
 * write_proc implementation of de device 
 *
*/
static int _DE_WriteProcFunction( UINT32 procId, char* command )
{
	/* TODO: add your proc_write implementation */	
	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			DE_PRINT("command string : %s\n", command );
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

/**
 * initialize proc utility for de device
 *
 * @see DE_Init
*/
void	DE_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( DE_MODULE, _g_de_device_proc_table, 
											_DE_ReadProcFunction,
											_DE_WriteProcFunction );
}

/**
 * cleanup proc utility for de device
 *
 * @see DE_Cleanup
*/
void	DE_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( DE_MODULE );	
}

/** @} */

