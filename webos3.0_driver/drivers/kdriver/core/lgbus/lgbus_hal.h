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
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2013.07.04
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef	_LGBUS_HAL_H_
#define	_LGBUS_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "lgbus_kapi.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

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
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int LGBUS_HAL_Init(void);
void LGBUS_HAL_Cleanup(void);

int LGBUS_HAL_Open(void);
int LGBUS_HAL_Close(void);
int LGBUS_HAL_Resume(void);
int LGBUS_HAL_Suspend(void);

int LGBUS_HAL_MonGetInfo(LX_LGBUS_MON_INFO_T *info);
int LGBUS_HAL_MonStart(LX_LGBUS_MON_CFG_T *cfg);
int LGBUS_HAL_MonRead(LX_LGBUS_MON_DATA_T *data);
int LGBUS_HAL_MonStop(void);
int LGBUS_HAL_MonReadReg(LX_LGBUS_MON_REG_T *reg_data);
int LGBUS_HAL_MonWriteReg(LX_LGBUS_MON_REG_T *reg_data);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LGBUS_HAL_H_ */

/** @} */

