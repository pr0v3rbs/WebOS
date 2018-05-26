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
 *  main configuration file for slt device
 *	slt device will teach you how to make device driver with new platform.
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
 *	@{
 */

#ifndef	_SLT_HAL_H_
#define	_SLT_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "slt_kapi.h"


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
int  SLT_HAL_Init(void);
void SLT_HAL_Cleanup(void);

int  SLT_HAL_Open(void);
int  SLT_HAL_Close(void);

void SLT_HAL_SetTestMode( LX_SLT_TEST_MODE_E *mode );
void SLT_HAL_IntrEnable( void );
void SLT_HAL_IntrDisable( void );
int  SLT_HAL_GetRefCrc(LX_SLT_CRC_DATA_T *data);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SLT_HAL_H_ */

/** @} */

