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
 *  application interface header for template device
 *
 *  author		your name ( your e-mail)
 *  version		1.0
 *  date		2009.11.15
 *
 *  @addtogroup lg1150_template
 */

#ifndef	_SAMPLE_KAPI_H_
#define	_SAMPLE_KAPI_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	SAMPLE_IOC_MAGIC		'S'

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	ev;
}
LX_SAMPLE_TEST_EVENT_WRITER_T;

typedef struct
{
	UINT32	ev;
	UINT32	rev;
	UINT32	timeout;

#define	LX_SAMPLE_TEST_EVENT_READ_ANY	0
#define	LX_SAMPLE_TEST_EVENT_READ_ALL	1

	UINT32	option;
}
LX_SAMPLE_TEST_EVENT_READER_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/**
 * test command for template device
 *
 * IO comand naming rule
 *
 * MODULE_IO[R][W]_COMMAND
 *
 *
 */
#define	SAMPLE_IO_RESET				_IO(SAMPLE_IOC_MAGIC,  0 )
#define SAMPLE_IOR_CHIP_REV_INFO	_IOR(SAMPLE_IOC_MAGIC, 1, CHIP_REV_INFO_T )
#define SAMPLE_IOW_WRITE_UINT32		_IOW(SAMPLE_IOC_MAGIC, 2, UINT32 )

/* timer test */
#define SAMPLE_IOW_TEST_TIMEOUT		_IOW(SAMPLE_IOC_MAGIC, 3, UINT32 )
#define SAMPLE_IOW_TEST_TIMETICK	_IOW(SAMPLE_IOC_MAGIC, 4, UINT32 )

/* event test */
#define	SAMPLE_IO_TEST_CLR_EVENT	_IO(SAMPLE_IOC_MAGIC,  5 )
#define SAMPLE_IORW_TEST_RECV_EVENT	_IOWR(SAMPLE_IOC_MAGIC, 6, LX_SAMPLE_TEST_EVENT_READER_T )
#define SAMPLE_IOW_TEST_SEND_EVENT	_IOW(SAMPLE_IOC_MAGIC,  7, LX_SAMPLE_TEST_EVENT_WRITER_T )

#define SAMPLE_IOC_MAXNR			8

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SAMPLE_DRV_H_ */

