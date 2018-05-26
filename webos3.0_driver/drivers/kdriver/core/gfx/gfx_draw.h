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
	Extern Function Prototype Declaration ( GFX Basic Renderer )
----------------------------------------------------------------------------------------*/
extern	void	GFX_LockDevice			(void);
extern	void	GFX_UnlockDevice		(void);

extern	int		GFX_GetSurfaceFdList	(int* surf_list, int max_surf, int* num_surf);

#define 		GFX_AllocARGBSurface(w,h,phys_addr)  GFX_AllocARGBSurfaceEx(w,h,phys_addr,(phys_addr)?(w)*4:0x0)
extern int      GFX_AllocARGBSurfaceEx	( int w, int h, UINT32 phys_addr, UINT32 stride );
extern int      GFX_Alloc8BppSurfaceEx	( int w, int h, UINT32 phys_addr, UINT32 stride );
extern int		GFX_SetSurfacePalette   ( int dst_fd, int size , UINT32 *data);
extern int		GFX_FreeSurface			( int dst_fd );
extern int      GFX_GetSurfaceInfo		( int dst_fd, LX_GFX_SURFACE_SETTING_T* surface_info );
extern void     GFX_BlitSurface			( int src0_fd, int src1_fd, int dst_fd, int sx, int sy, int sw, int wh, int dx, int dy, BOOLEAN fBlend );
extern void     GFX_StretchSurface		( int src_fd, int dst_fd, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh );
extern void     GFX_ClearSurface		( int dst_fd, int w, int h );
extern void     GFX_FillSurface			( int dst_fd, int x, int y, int w, int h, UINT32 color );
extern void     GFX_FadeSurface			( int src_fd, int dst_fd, int w, int h, UINT32 alpha );
extern void		GFX_RotateSurface		( int src_fd, int dst_fd, int sx, int sy, int sw, int sh, int dx, int dy, UINT32 degree );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_DRV_H_ */

/** @} */
