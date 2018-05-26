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
 *  application interface header for memprot device
 *
 *  author		sanghyeun.kim
 *  version		1.0
 *  date		2012.10.17
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

#ifndef	_MEMPROT_KAPI_H_
#define	_MEMPROT_KAPI_H_

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
#define	MEMPROT_IOC_MAGIC		'a'
/**
@name MEMPROT IOCTL List
ioctl list for memprot device.

@{
@def MEMPROT_IO_RESET
Reset memprot module.
When memprot device receives above IOCTL, then reset itself.

@def MEMPROT_IOR_CHIP_REV_INFO
Reads chip revision information.
Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.

@def MEMPROT_IOW_WRITE_UINT32
write 32bit value to memprot module.
detailed...[minimum two lines of comment required. one line of comment does not generated automatically]

@def MEMPROT_IOW_WRITE_STRUCT
control complex parameter to memprot module.
control codec_type, speed, and additional parameter.
no need to use "see also", exact struct name shall be linked automatically.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define	MEMPROT_IO_RESET				_IO(MEMPROT_IOC_MAGIC,  0 )
#define MEMPROT_IOR_CHIP_REV_INFO		_IOR(MEMPROT_IOC_MAGIC, 1, CHIP_REV_INFO_T )
#define MEMPROT_IOW_WRITE_UINT32		_IOW(MEMPROT_IOC_MAGIC, 2, UINT32 )
#define MEMPROT_IOW_WRITE_STRUCT		_IOW(MEMPROT_IOC_MAGIC, 3, MEMPROT_IOCTL_PARAM_T)

#define MEMPROT_IOC_MAXNR				4
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * memprot ioctl parameter.
 * useful when complex parameter should be passed to memprot kernel driver.
 * @see MEMPROT_IOW_WRITE_STRUCT
 */
typedef struct {
	UINT32	codec_type;				///< type of codec
	SINT32	speed;					///< decoding speed in 1/2^16 sec.
	UINT16	detailed_param[2];		///< optional parameter.
}	MEMPROT_IOCTL_PARAM_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MEMPROT_DRV_H_ */

/** @} */
