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
 *  driver interface header for gfx device. ( used only within kdriver )
 *	this file lists exported function, types for the external modules.
 *
 *  @author		raxis.lim (raxis.lim@lge.com).lim (raxis.lim@lge.com)
 *  @version	1.0 
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_gfx
 *	@{
 */
#ifndef	_GFX_DRV_H_
#define	_GFX_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "gfx_kapi.h"

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
extern	void			GFX_PreInit					(void);
extern	int      		GFX_Init					(void);
extern	void     		GFX_Cleanup					(void);

extern	void			GFX_LockDevice				(void);
extern	void			GFX_UnlockDevice			(void);

extern	int				GFX_GetSurfaceMemoryStat    (LX_GFX_MEM_STAT_T* pMemStat );

/** allocate dynamic buffer from GFX surface memory 
 *
 *	@note buffer allocation may fail when too-big-sized buffer requested. so app should check if return address is NULL or NULL
 *
 *	@param size [IN] buffer size in byte
 *	@return physical address if success, 0x0 if failed
 */
extern	UINT32			GFX_AllocBuffer				(UINT32 size);

/** free dynamic buffer 
 *
 *	@param phys_addr [IN] physical address
 */
extern 	void			GFX_FreeBuffer				(UINT32 phys_addr );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_DRV_H_ */

/** @} */
