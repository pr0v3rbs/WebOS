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


/** @file slt_kapi.h
 *
 *  application interface header for hdmi device
 *
 *  @author		wunseo.yeo
 *  @version	1.0
 *  @date		2014.07.02
 *
 *  @addtogroup lg1150_slt
 *	@{
 */

#ifndef	_SLT_KAPI_H_
#define	_SLT_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	SLT_IOC_MAGIC		'T'

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define SLT_IOW_SET_TEST_MODE			_IOW (SLT_IOC_MAGIC,  0, LX_SLT_TEST_MODE_E)
#define SLT_IO_CRC_INTR_ENABLE			_IO  (SLT_IOC_MAGIC,  1)
#define SLT_IO_CRC_INTR_DISABLE			_IO  (SLT_IOC_MAGIC,  2)
#define SLT_IOR_GET_CRC_DATA			_IOR (SLT_IOC_MAGIC,  3, LX_SLT_CRC_DATA_T)
                                                                   
#define SLT_IOC_MAXNR				4


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

#define SLT_CRC_NUM		300

typedef struct
{
    UINT32	data0[SLT_CRC_NUM];
  //  UINT32	data1[SLT_CRC_NUM];
} LX_SLT_CRC_DATA_T;

typedef enum
{
    SLT_TEST_GET_CRC = 0,
    SLT_TEST_CHECK_FIXED_CRC,
} LX_SLT_TEST_MODE_E;



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SLT_KAPI_H_ */

