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
 *  application interface header for sys device
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2010.12.13
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

#ifndef	_SYS_KAPI_H_
#define	_SYS_KAPI_H_

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
#define	SYS_IOC_MAGIC		'a'
/**
@name SYS IOCTL List
ioctl list for sys device.

@{
@def SYS_IO_RESET
Reset sys module.
When sys device receives above IOCTL, then reset itself.

@def SYS_IOR_CHIP_REV_INFO
Reads chip revision information.
Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.

@def SYS_IOW_WRITE_UINT32
write 32bit value to sys module.
detailed...[minimum two lines of comment required. one line of comment does not generated automatically]

@def SYS_IOW_WRITE_STRUCT
control complex parameter to sys module.
control codec_type, speed, and additional parameter.
no need to use "see also", exact struct name shall be linked automatically.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define	SYS_IO_RESET				_IO(SYS_IOC_MAGIC,  0 )
#define SYS_IOR_CHIP_REV_INFO		_IOR(SYS_IOC_MAGIC, 1, CHIP_REV_INFO_T )

#define SYS_IOW_WRITE_MEM			_IOW(SYS_IOC_MAGIC, 2, LX_SYS_MEM_T )
#define SYS_IORW_READ_MEM			_IOWR(SYS_IOC_MAGIC, 3, LX_SYS_MEM_T )

#define SYS_IOW_WRITE_MEM_ARRAY		_IOW(SYS_IOC_MAGIC, 4, LX_SYS_MEM_ARRAY_T )
#define SYS_IORW_READ_MEM_ARRAY		_IOWR(SYS_IOC_MAGIC, 5, LX_SYS_MEM_ARRAY_T )

#define SYS_IOW_SPREAD_SPRECTRUM	_IOW(SYS_IOC_MAGIC, 6, LX_SPREAD_SPECTRUM_T )

#define SYS_IORW_DIAGNOSIS			_IOWR(SYS_IOC_MAGIC, 10, LX_DIAG_PARAM_T )


#define SYS_IOC_MAXNR				10
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * sys ioctl parameter.
 * useful when complex parameter should be passed to sys kernel driver.
 * @see SYS_IOW_WRITE_STRUCT
 */
typedef struct
{
	UINT32	addr;
	UINT32	value;
}	LX_SYS_MEM_T;

typedef struct
{
	UINT32	addr;
	UINT32	size;
	void	*data;
}	LX_SYS_MEM_ARRAY_T;


/**
 * Diagnosis module types
 */
typedef enum
{
	LX_DIAG_AAD = 0,
	LX_DIAG_AUDIO,
	LX_DIAG_AFE,
	LX_DIAG_CI,
	LX_DIAG_DE,
	LX_DIAG_DEMOD,
	LX_DIAG_DENC,
	LX_DIAG_DIMM,
	LX_DIAG_FBDEV,
	LX_DIAG_FMT3D,
	LX_DIAG_GFX,
	LX_DIAG_GPU,
	LX_DIAG_GPIO,
	LX_DIAG_HDMI,
	LX_DIAG_I2C,
	LX_DIAG_SPI,
	LX_DIAG_PE,
	LX_DIAG_PVR,
	LX_DIAG_SDEC,
	LX_DIAG_VBI,
	LX_DIAG_VENC,
	LX_DIAG_PNG,
	LX_DIAG_SE,
	LX_DIAG_SYS,
	LX_DIAG_SCI,
	LX_DIAG_CLK,
	LX_DIAG_MONITOR,
	LX_DIAG_MJPEG,
	LX_DIAG_VDEC,
	LX_DIAG_APR,
	LX_DIAG_BE,
	LX_DIAG_OVI,

	LX_DIAG_MODULE_MAX,
} LX_DIAG_MODULE_T;

/**
 * Diagnosis checker type
 */
typedef enum
{
	LX_DIAG_CHECKER_PANIC,
	LX_DIAG_CHECKER_STATUS,
	LX_DIAG_CHECKER_RANGE,
	LX_DIAG_CHECKER_IRQ,

	LX_DIAG_CHECKER_TYPE_MAX
} LX_DIAG_CHECKER_TYPE_T;


typedef enum
{
	LX_DIAG_REPORT_TYPE_PRINT,
	LX_DIAG_REPORT_TYPE_STORAGE,

	LX_DIAG_REPORT_TYPE_MAX
} LX_DIAG_REPORT_TYPE_T;


typedef struct
{
	LX_DIAG_MODULE_T		module;
	LX_DIAG_CHECKER_TYPE_T	type;

} LX_DIAG_PARAM_T;


typedef enum
{
	LX_SS_CPU_PLL = 0,		// CPU PLL
	LX_SS_MAIN_PLL_0,		// Main PLL 0
	LX_SS_MAIN_PLL_1,		// Main PLL 1
	LX_SS_MAIN_PLL_2,		// Main PLL 2
	LX_SS_DISPLAY,			// Display PLL
} LX_SS_PLL_T;

typedef enum
{
	LX_SS_RATIO__OFF = 0,
	LX_SS_RATIO_0_25,		// 0.25%
	LX_SS_RATIO_0_50,		// 0.50%
	LX_SS_RATIO_0_75,		// 0.75%
	LX_SS_RATIO_1_00,		// 1.00%
	LX_SS_RATIO_1_25,		// 1.25%
	LX_SS_RATIO_1_50,		// 1.50%
	LX_SS_RATIO_1_75,
} LX_SS_RATIO_T;


typedef struct
{
	LX_SS_PLL_T		pll;
	LX_SS_RATIO_T	ratio;
} LX_SPREAD_SPECTRUM_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SYS_DRV_H_ */

/** @} */
