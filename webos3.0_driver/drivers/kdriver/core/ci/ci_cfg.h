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
 *  main configuration file for ci device
 *	ci device will teach you how to make device driver with new platform.
 *
 *  author 		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef	_CI_CFG_H_
#define	_CI_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	CI_MODULE			"ci"
#define CI_MAX_DEVICE		1

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

#define OK			 0
#define NOT_OK		-1

//#define CI_ENABLE_ERR	/* it is recommended to enable this always */

//#define CI_ENABLE_INFO
//#define CI_ENABLE_DBG_INFO

//#define CIS_ENABLE_INFO
//#define	CIS_ENABLE_TUPLE_DUMP
//#define	CIS_ENABLE_PARSE_DUMP

//#define CI_IO_ENABLE_INFO
//#define CI_IO_DUMP

//#define CI_DEBUG_PRINT_ON

//#define CI_IO_DELAY_PRINT
//#define CI_IO_TIME_PRINT

//#define CI_BUILD_TIME_PRINT

//#define CI_MON_FR_RETRY

//#define CI_KERN_LOG

//#define CI_DATA_TEST_ON

#define CI_POL_CHECK_ON

#define CI_OPT_CHK_MODE

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_CFG_H_ */

/** @} */

