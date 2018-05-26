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
 *  main configuration file for lgbus device
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2013.07.04
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "os_util.h"

#include "lgbus_drv.h"

#include "lgbus_hal.h"
#include "lgbus_hw.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	LGBUS_HW_FUNC_T *func;

} LGBUS_HAL_T;


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
static LGBUS_HAL_T hal;

/*========================================================================================
	Implementation Group
========================================================================================*/

int LGBUS_HAL_Init(void)
{
	LGBUS_HW_FUNC_T *func = NULL;

	if(0){}
#ifdef INCLUDE_H13_CHIP_KDRV
	else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) ) func = LGBUS_H13_Init();
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) ) func = LGBUS_M14_Init();
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) ) func = LGBUS_H14_Init();
#endif
#ifdef INCLUDE_H15_CHIP_KDRV
	else if ( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) ) func = LGBUS_H15_Init();
#endif
	else LGBUS_ERROR("Not supported chip 0x%x\n", lx_chip_rev() );

	if(func == NULL)
		return -1;

	if(func->init() < 0)
		return -1;

	hal.func = func;

	return 0;
}

int LGBUS_HAL_Resume(void)
{
	if(hal.func && hal.func->resume)
		return hal.func->resume();

	return -1;
}

int LGBUS_HAL_Suspend(void)
{
	if(hal.func && hal.func->suspend)
		return hal.func->suspend();

	return -1;
}

void LGBUS_HAL_Cleanup(void)
{
	if(hal.func && hal.func->cleanup)
		return hal.func->cleanup();
}

int LGBUS_HAL_Open(void)
{
	if(hal.func && hal.func->open)
		return hal.func->open();

	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_Open);

int LGBUS_HAL_Close(void)
{
	if(hal.func && hal.func->close)
		return hal.func->close();
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_Close);

int LGBUS_HAL_MonGetInfo(LX_LGBUS_MON_INFO_T *info)
{
	if(hal.func && hal.func->mon_getinfo)
		return hal.func->mon_getinfo(info);
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonGetInfo);

int LGBUS_HAL_MonStart(LX_LGBUS_MON_CFG_T *cfg)
{
	if(hal.func && hal.func->mon_start)
		return hal.func->mon_start(cfg);
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonStart);

int LGBUS_HAL_MonRead(LX_LGBUS_MON_DATA_T *data)
{
	if(hal.func && hal.func->mon_read)
		return hal.func->mon_read(data);
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonRead);

int LGBUS_HAL_MonStop(void)
{
	if(hal.func && hal.func->mon_stop)
		return hal.func->mon_stop();
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonStop);

int LGBUS_HAL_MonReadReg(LX_LGBUS_MON_REG_T *reg_data)
{
	if(hal.func && hal.func->mon_readreg)
		return hal.func->mon_readreg(reg_data);
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonReadReg);

int LGBUS_HAL_MonWriteReg(LX_LGBUS_MON_REG_T *reg_data)
{
	if(hal.func && hal.func->mon_writereg)
		return hal.func->mon_writereg(reg_data);
	return -1;
}
EXPORT_SYMBOL(LGBUS_HAL_MonWriteReg);

/** @} */

