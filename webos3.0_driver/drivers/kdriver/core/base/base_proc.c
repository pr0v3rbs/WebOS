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

/** @file
 *
 *	kdriver main
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.04
 *
 *  @addtogroup lg1150_base
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_drv.h"

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
extern void	BASE_DEVMEM_DumpMemCfg( struct seq_file* m );

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

/*========================================================================================
	Implementation Group
========================================================================================*/

/*========================================================================================
	Debug proc system
========================================================================================*/
enum {
	PROC_ID_MEMCFG	= 0,
	PROC_ID_DCHIP,
	PROC_ID_ACHIP,
	PROC_ID_PLATFORM,
	PROC_ID_GRAPHIC_RES,
	PROC_ID_REG_RD,
	PROC_ID_REG_WR,
};

OS_PROC_SEQRD_DEFINE(base_memcfg_dump_fops, BASE_DEVMEM_DumpMemCfg)

static OS_PROC_DESC_TABLE_T	_g_base_proc_table[] =
{
	{ "memcfg",			PROC_ID_MEMCFG,		OS_PROC_FLAG_FOP, (void*)&base_memcfg_dump_fops },

	{ "chip_rev",		PROC_ID_DCHIP, 		OS_PROC_FLAG_READ },
	{ "ace_rev",		PROC_ID_ACHIP, 		OS_PROC_FLAG_READ },
	{ "platform",		PROC_ID_PLATFORM, 	OS_PROC_FLAG_READ },
	{ "graphic_res",	PROC_ID_GRAPHIC_RES,OS_PROC_FLAG_READ },

	{ "reg_rd",			PROC_ID_REG_RD,		OS_PROC_FLAG_READ|OS_PROC_FLAG_WRITE },
	{ "reg_wr",			PROC_ID_REG_WR,		OS_PROC_FLAG_READ|OS_PROC_FLAG_WRITE },

	{ NULL, 			0,			 		0 },
};

static int	BASE_PROC_ReadProcFunction( UINT32 procId, char* buffer )
{
	int		len = 0;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_DCHIP:
		{
			len += sprintf( buffer, "0x%x\n", lx_chip_rev() );
		}
		break;

		case PROC_ID_ACHIP:
		{
			len += sprintf( buffer, "0x%x\n", lx_ace_rev() );
		}
		break;

		case PROC_ID_PLATFORM:
		{
			len += sprintf( buffer, "0x%x\n", lx_chip_plt() );
		}
		break;

		case PROC_ID_GRAPHIC_RES:
		{
			len += sprintf( buffer, "0x%x\n", lx_chip_graphic_res() );
		}
		break;

		case PROC_ID_REG_RD:
		{
			len += sprintf( buffer, "<usage> echo #addr > /proc/lg/base/reg_rd\n");
		}
		break;

		case PROC_ID_REG_WR:
		{
			len += sprintf( buffer, "<usage> echo #addr #val > /proc/lg/base/reg_wr\n");
		}
		break;

		default:
		{
			len = sprintf( buffer, "%s(%d)\n", "unimplemented read proc",procId );
		}
	}

	return len;
}

/*
 * write_proc implementation of ttx device
 *
*/
static int BASE_PROC_WriteProcFunction( UINT32 procId, char* command )
{
	/* TODO: add your proc_write implementation */
	switch( procId )
	{
        case PROC_ID_REG_RD:
        {
            UINT32 addr, val;
            sscanf( command, " %x", &addr );
            OS_RdReg( addr, &val );
            printk("\nAddr 0x%x = 0x%08x\n", addr, val );
        }
        break;

        case PROC_ID_REG_WR:
        {
            UINT32 addr, val;

            sscanf( command, " %x %x", &addr, &val );
            OS_WrReg( addr, val );
            OS_RdReg( addr, &val );
            printk("\nAddr 0x%x = 0x%08x\n", addr, val );
        }
        break;
	}

	return strlen(command);
}

/**
 * Initialize proc system for debug utility
 *
 */
void	BASE_PROC_Init ( void )
{
	OS_PROC_CreateEntryEx ( "base" ,	_g_base_proc_table,
										BASE_PROC_ReadProcFunction,
										BASE_PROC_WriteProcFunction );
}

