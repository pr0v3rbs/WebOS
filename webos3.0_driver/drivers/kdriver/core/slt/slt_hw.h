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
 *  hw configuration file for slt device
 *
 *  @author		wunseo.yeo@lge.com
 *  @version	1.0
 *  @date		2014.08
 *
 *  @addtogroup lg115x_slt
 *	@{
 */

#ifndef	_SLT_HW_H_
#define	_SLT_HW_H_

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
typedef struct
{
	int (*init)(void);
	void (*cleanup)(void);

	int (*open)(void);
	int (*close)(void);

	void (*set_testmode)(LX_SLT_TEST_MODE_E *mode);
	void (*intr_enable)(void);
	void (*intr_disable)(void);
	int (*get_ref_crc)(LX_SLT_CRC_DATA_T *crc);

} SLT_HW_FUNC_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
SLT_HW_FUNC_T* SLT_H13_Init(void);
SLT_HW_FUNC_T* SLT_M14_Init(void);
SLT_HW_FUNC_T* SLT_H15_Init(void);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SLT_HW_H_ */

/** @} */

