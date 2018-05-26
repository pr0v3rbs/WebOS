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
 *  hw configuration file for lgbus device
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2013.07.04
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

#ifndef	_LGBUS_HW_H_
#define	_LGBUS_HW_H_

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
typedef struct
{
	int (*init)(void);
	void (*cleanup)(void);

	int (*open)(void);
	int (*close)(void);

	int (*resume)(void);
	int (*suspend)(void);

	int (*mon_getinfo)(LX_LGBUS_MON_INFO_T *info);
	int (*mon_start)(LX_LGBUS_MON_CFG_T *cfg);
	int (*mon_read)(LX_LGBUS_MON_DATA_T *data);
	int (*mon_stop)(void);
	int (*mon_readreg)(LX_LGBUS_MON_REG_T *reg_data);
	int (*mon_writereg)(LX_LGBUS_MON_REG_T *reg_data);

} LGBUS_HW_FUNC_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
LGBUS_HW_FUNC_T* LGBUS_H13_Init(void);
LGBUS_HW_FUNC_T* LGBUS_M14_Init(void);
LGBUS_HW_FUNC_T* LGBUS_H14_Init(void);
LGBUS_HW_FUNC_T* LGBUS_H15_Init(void);

LGBUS_HW_FUNC_T* LGBUS_M14_A0_Init(void);
LGBUS_HW_FUNC_T* LGBUS_M14_B0_Init(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LGBUS_HW_H_ */

/** @} */

