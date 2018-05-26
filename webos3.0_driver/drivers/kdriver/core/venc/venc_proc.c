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
 *  Linux proc interface for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		Jaeseop So (jaeseop.so@lge.com)	
 *  version		1.5
 *  date		2012.08.17
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include "venc_drv.h"
#include "proc_util.h"
#include "debug_util.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_COMMAND,
	PROC_ID_STATUS,
	PROC_ID_MEMORY,
	PROC_ID_CFG_CTRL,
#ifdef	H1ENCODE_ENABLE_DELAYED_ENCODING
	PROC_ID_MAX_DELAY,
#endif
	PROC_ID_MAX,
};

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/
int VENC_HAL_PROC_ReadDeviceStatus( char *buffer );
int VENC_HAL_PROC_ReadMemoryStatus( char *buffer );

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/
#ifdef	H1ENCODE_ENABLE_DELAYED_ENCODING
extern UINT32 maxDelayedTime;
#endif

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_venc_device_proc_table[] =
{
	{ "author",		PROC_ID_AUTHOR  , OS_PROC_FLAG_READ },
	{ "command",	PROC_ID_COMMAND , OS_PROC_FLAG_WRITE },
	{ "status",		PROC_ID_STATUS  , OS_PROC_FLAG_READ },
	{ "mem",		PROC_ID_MEMORY  , OS_PROC_FLAG_READ },
	{ "cfgctrl",	PROC_ID_CFG_CTRL, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
#ifdef	H1ENCODE_ENABLE_DELAYED_ENCODING
	{ "maxDelayedTime",	PROC_ID_MAX_DELAY, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
#endif
	{ NULL, 		PROC_ID_MAX		, 0 }
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * read_proc implementation of venc device
 *
*/
static int	_VENC_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "Jaeseop So (jaeseop.so@lge.com)" );
		}
		break;

		case PROC_ID_STATUS:
		{
			int len = 0;

			len = VENC_HAL_PROC_ReadDeviceStatus(buffer);
			
			ret = len;
		}
		break;

		case PROC_ID_MEMORY:
		{
			int len = 0;

			len = VENC_HAL_PROC_ReadMemoryStatus(buffer);

			ret = len;
		}
		break;

		case PROC_ID_CFG_CTRL:
		{
			int len = 0;
			len += sprintf( buffer + len, "VENC CONFIG CTRL status\n");
			len += sprintf( buffer + len, "[%d] Flip Top/Bottom Field    : %s\n", VENC_CFG_CTRL_FLIP_TB, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_FLIP_TB ) ? "On":"Off" );
			len += sprintf( buffer + len, "[%d] Scene Chage Detect       : %s\n", VENC_CFG_CTRL_SCD, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SCD ) ? "On":"Off" );
			len += sprintf( buffer + len, "[%d] Sample Aspect Ratio      : %s\n", VENC_CFG_CTRL_SAR, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_SAR ) ? "On":"Off" );
			len += sprintf( buffer + len, "[%d] Encoder Monitor          : %s\n", VENC_CFG_CTRL_MONITOR, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_MONITOR ) ? "On":"Off" );
			len += sprintf( buffer + len, "[%d] Field Repeat             : %s\n", VENC_CFG_CTRL_FIELD_REPEAT, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_FIELD_REPEAT ) ? "On":"Off" );
			len += sprintf( buffer + len, "[%d] DEBUG Worst Case         : %s\n", VENC_CFG_CTRL_DEBUG_WORST, GET_VENC_CFG_CTRL_INDEX( VENC_CFG_CTRL_DEBUG_WORST ) ? "On":"Off" );
			len += sprintf( buffer + len, "\n");

			len += sprintf( buffer + len, "Usage: echo index enable > /proc/lg/venc/cfgctrl\n");
			len += sprintf( buffer + len, "       index : 0 ~ %d\n", VENC_CFG_CTRL_COUNT-1);
			len += sprintf( buffer + len, "       enable: 0(off), 1(on)\n");
			ret = len;
		}
		break;
#ifdef	H1ENCODE_ENABLE_DELAYED_ENCODING
		case PROC_ID_MAX_DELAY:
		{
			int len = 0;
			len += sprintf( buffer + len, "VENC MAX DELAY VALUE [%d]", maxDelayedTime);
			ret = len;
		}
		break;

#endif

		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}

	return ret;
}

/*
 * write_proc implementation of venc device
 *
*/
static int _VENC_WriteProcFunction( UINT32 procId, char* command )
{
	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			printk("command string : %s\n", command );
		}
		break;

		case PROC_ID_CFG_CTRL:
		{
			int index;
			int enable;

			sscanf( command, " %d %d", &index, &enable );

			if ( index >= 0 && index < VENC_CFG_CTRL_COUNT )
			{
				if ( enable )
				{
					SET_VENC_CFG_CTRL_INDEX( index );
				}
				else
				{
					UNSET_VENC_CFG_CTRL_INDEX( index );
				}
			}
		}
		break;
#ifdef	H1ENCODE_ENABLE_DELAYED_ENCODING
		case PROC_ID_MAX_DELAY:
		{
			int delay;

			sscanf( command, " %d", &delay );

			maxDelayedTime = delay;
		}
		break;
#endif

		default:
		{
			/* do nothing */
			printk( " %s(%d)\n", "unimplemented write proc", procId );
		}
		break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for venc device
 *
 * @see VENC_Init
*/
void	VENC_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( VENC_MODULE, _g_venc_device_proc_table,
											_VENC_ReadProcFunction,
											_VENC_WriteProcFunction );
}

/**
 * cleanup proc utility for venc device
 *
 * @see VENC_Cleanup
*/
void	VENC_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( VENC_MODULE );
}

/** @} */


