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
 *  driver interface header for gpu device. ( used only within kdriver )
 *	gpu device will teach you how to make device driver with new platform.
 *
 *  author      raxis.lim (raxis.lim@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_gpu
 *	@{
 */

#ifndef	_GPU_DRV_H_
#define	_GPU_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "gpu_kapi.h"
#include "gpu_hw.h"

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
#define	GPU_PRINT(format, args...)			DBG_PRINT(  g_gpu_debug_fd, 0, format, ##args)
#define	GPU_PRINT_ERROR(format, args...)	DBG_PRINTX(  g_gpu_debug_fd, 1, format, ##args)
#define	GPU_TRACE(format, args...)			DBG_PRINTX( g_gpu_debug_fd, 0, format, ##args)
#define	GPU_ISR_PRINT(format, args...)		DBG_PRINT(  g_gpu_debug_fd, 3, format, ##args)
#define	GPU_DEBUG(format, args...)			DBG_PRINT( g_gpu_debug_fd, 2, format, ##args)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void	GPU_PreInit(void);
int		GPU_Init(void);
void	GPU_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_gpu_debug_fd;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GPU_DRV_H_ */

/** @} */
