/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  main configuration file for mjpeg device
 *	mjpeg device will teach you how to make device driver with new platform.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.04.09
 *  note		Additional information.
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

#ifndef	_MJPEG_CFG_H_
#define	_MJPEG_CFG_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#define MJPEG_ENCODE_QUALITY		3	// 0: Low, 1 - Middle low, 2 - Middle high, 3 - High
/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define	MJPEG_MODULE			"mjpeg"
#define MJPEG_MAX_DEVICE		1

#define	MJPEG_TIMEOUT	100

#define MJPEG_USE_LOCK		// enable lock/unlock macro

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef struct
{
	char*		pcMemory_name;
	UINT32		ui32Memory_base;
	UINT32		ui32Memory_size;
} MJPEG_MEM_CFG_T;

/** MJPEG Def
 *
 *
 */
typedef struct
{
	UINT32			jpeg_reg_base;
	UINT32			icod_reg_base;
	UINT32			irq_num;
	UINT32			num_device;
} LX_MJPEG_DEF_T;

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/
extern MJPEG_MEM_CFG_T gMemCfgMjpeg;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MJPEG_CFG_H_ */

/** @} */

