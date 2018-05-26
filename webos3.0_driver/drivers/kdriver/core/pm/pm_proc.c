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
 *  main driver implementation for pm device.
 *	pm device will teach you how to make device driver with new platform.
 *
 *  author		hankyung.yu (hankyung.yu@lge.com)
 *  version		1.0
 *  date			2014.07.18
 *  note			Additional information.
 *
 *  @addtogroup lg1210_pm
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
#include <asm/uaccess.h>
#include "os_util.h"
#include "base_device.h"
#include "pm_drv.h"
#include "proc_util.h"
#include "debug_util.h"

#include "../../chip/h15/pm/pm_h15common.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
pms_proc_t *pms_proc = NULL;

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

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * read_proc implementation of pm device
 *
*/
int	_PM_ReadProcFunction(	UINT32 procId, char* buffer )
{
	return 0;
}

/**
 * write_proc implementation of pm device
 *
*/
int _PM_WriteProcFunction( UINT32 procId, char* command )
{
	return strlen(command);
}

/**
 * initialize proc utility for pm device
 *
 * @see PM_Init
*/
void	PM_PROC_Init (void)
{
	if (( lx_chip_rev() & 0xFFFFFF00) == (LX_CHIP_REV(H15,A0) & 0xFFFFFF00))
	{
		pms_proc = get_h15pm_proc();
		pms_proc->init();
	}
}

/**
 * cleanup proc utility for pm device
 *
 * @see PM_Cleanup
*/
void	PM_PROC_Cleanup (void)
{
	if (pms_proc)
		pms_proc->cleanup();
}

/** @} */

