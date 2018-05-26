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
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "pm_drv.h"
//#include "pm_reg.h"
#include "sys_regs.h"
#include "sys_io.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PM_LOCK_INIT(dev)		OS_InitMutex(&dev->lock, OS_SEM_ATTR_DEFAULT)
#define PM_LOCK(dev)			OS_LockMutex(&dev->lock)
#define PM_UNLOCK(dev)		OS_UnlockMutex(&dev->lock)

#define PM_EX_LOCK_INIT(dev)	OS_InitMutex(&dev->ex_lock, OS_SEM_ATTR_DEFAULT)
#define PM_EX_LOCK(dev)		OS_LockMutex(&dev->ex_lock)
#define PM_EX_UNLOCK(dev)		OS_UnlockMutex(&dev->ex_lock)


#if 0
#define PM_CORE_DEBUG(format, args...)	PM_ERROR(format, ##args)
#else
#define PM_CORE_DEBUG(format, args...)	do{}while(0)
#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT8	direction;
	UINT8	data;
} PM_PM_DATA_T;


typedef struct PM_DEV
{
	OS_SEM_T	lock;


#ifdef KDRV_CONFIG_PM
	int			(*Resume)	(void);
	int			(*Suspend)	(void);
	PM_PM_DATA_T*	pmdata;
#endif
} PM_DEV_T;


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

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*************************************************************************
* Set Pinmux
*************************************************************************/

int PM_DevInit(void)
{
	return 0;
}

int PM_DevResume(void)
{
	return 0;
}
int PM_DevSuspend(void)
{
	return 0;
}

/** @} */
