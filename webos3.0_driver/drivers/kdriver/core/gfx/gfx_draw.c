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
 *	GFX drawing utilites for other module
 *
 *  author	raxis.lim    (raxis.lim@lge.com)
 *
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg1150_gfx
 *  @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/memory.h>
#include <asm/uaccess.h>            /* to verify pointers from user space */
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>

#include "gfx_impl.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
static void GFX_WaitGfxSync( LX_GFX_MANUAL_BLEND_CTRL_T* blend_ctrl, BOOLEAN fAutoReset );

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
    Implementation Group
========================================================================================*/
int     GFX_AllocARGBSurfaceEx( int w, int h, UINT32 phys_addr, UINT32 stride )
{
    LX_GFX_SURFACE_SETTING_T            surface_settings;
    LX_GFX_SURFACE_SETTING_PARAM_T      surface_param;

    memset( &surface_settings, 0x0, sizeof(LX_GFX_SURFACE_SETTING_T) );

    if ( phys_addr )
    {
        surface_settings.type       = LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
        surface_settings.phys_addr  = (void*)phys_addr;
    }
    else
    {
        surface_settings.type       = LX_GFX_SURFACE_TYPE_MEM_BUFFER;
    }
    surface_settings.pixel_format   = LX_GFX_PIXEL_FORMAT_ARGB8888;
    surface_settings.width          = w;
    surface_settings.height         = h;
    surface_settings.alignment      = 4;
    surface_settings.stride         = stride;

    memcpy( &surface_param.surface_setting, &surface_settings, sizeof(LX_GFX_SURFACE_SETTING_T) );

    GFX_AllocSurface( &surface_param );

    return surface_param.surface_fd;
}

int     GFX_Alloc8BppSurfaceEx( int w, int h, UINT32 phys_addr, UINT32 stride )
{
    LX_GFX_SURFACE_SETTING_T            surface_settings;
    LX_GFX_SURFACE_SETTING_PARAM_T      surface_param;

    memset( &surface_settings, 0x0, sizeof(LX_GFX_SURFACE_SETTING_T) );

    if ( phys_addr )
    {
        surface_settings.type       = LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
        surface_settings.phys_addr  = (void*)phys_addr;
    }
    else
    {
        surface_settings.type       = LX_GFX_SURFACE_TYPE_MEM_BUFFER;
    }
    surface_settings.pixel_format   = LX_GFX_PIXEL_FORMAT_INDEX_8;
    surface_settings.width          = w;
    surface_settings.height         = h;
    surface_settings.alignment      = 4;
    surface_settings.stride         = stride;

    memcpy( &surface_param.surface_setting, &surface_settings, sizeof(LX_GFX_SURFACE_SETTING_T) );

    GFX_AllocSurface( &surface_param );

    return surface_param.surface_fd;
}

void GFX_FillSurface	( int fd, int x, int y, int w, int h, UINT32 color )
{
	LX_GFX_PORT_T	src0_port;
	LX_GFX_PORT_T	src1_port;
	LX_GFX_PORT_T	dst_port;

	LX_GFX_MANUAL_BLEND_CTRL_PARAM_T	blend_ctrl;
	memset( &blend_ctrl, 0x0, sizeof(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T) );

	/* basic error check */
	if ( w <= 0 || h <= 0 ) return;

	memset( &src0_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &src1_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &dst_port,  0x0, sizeof(LX_GFX_PORT_T));

	dst_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	dst_port.surface_fd 	= fd;
	dst_port.rect.x			= x;
	dst_port.rect.y			= y;
	dst_port.rect.w			= w;
	dst_port.rect.h			= h;

	src1_port.surface_type  = LX_GFX_SURFACE_TYPE_COLOR;
	src1_port.surface_color.pixel_format = LX_GFX_PIXEL_FORMAT_INDEX_0;
	src1_port.surface_color.pixel_value  = color;
	src1_port.rect.x        = 0;
	src1_port.rect.y        = 0;
	src1_port.rect.w        = w;
	src1_port.rect.h        = h;

	if ( lx_chip_rev() >= LX_CHIP_REV(L9,B0) )
	{
		src0_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
		src0_port.surface_fd 	= fd;
		src0_port.rect.x		= x;
		src0_port.rect.y		= y;
		src0_port.rect.w		= w;
		src0_port.rect.h		= h;

		blend_ctrl.op_mode	= LX_GFX_OP_MODE_TWO_SRC_PORT;
		blend_ctrl.out_sel	= LX_GFX_OUT_SEL_ROP;
		blend_ctrl.rop		= LX_GFX_ROP_DST;
		blend_ctrl.constant_color = 0x0;
	}
	else
	{
		src0_port.surface_type  = LX_GFX_SURFACE_TYPE_COLOR;
		src0_port.surface_color.pixel_format = LX_GFX_PIXEL_FORMAT_INDEX_0;
		src0_port.surface_color.pixel_value  = 0x00000000;
		src0_port.rect.x        = 0;
		src0_port.rect.y        = 0;
		src0_port.rect.w        = w;
		src0_port.rect.h        = h;

		blend_ctrl.op_mode	= LX_GFX_OP_MODE_WRITE;
		blend_ctrl.out_sel	= LX_GFX_OUT_SEL_CONST_COLOR;
		blend_ctrl.rop		= LX_GFX_ROP_NONE;
		blend_ctrl.constant_color = 0x0;
	}

	memcpy( &blend_ctrl.src0, &src0_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.src1, &src1_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.dst,  &dst_port, sizeof(LX_GFX_PORT_T));

	GFX_RunBlendOp(&blend_ctrl, g_gfx_cfg.screen_blit_cmd_delay );
	GFX_WaitGfxSync ( &blend_ctrl, FALSE );
}

void GFX_ClearSurface	( int fd, int w, int h )
{
	GFX_FillSurface( fd, 0, 0, w, h, 0x0 );
}

void     GFX_StretchSurface ( int src, int dst, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh )
{
	LX_GFX_PORT_T	src0_port;
	LX_GFX_PORT_T	src1_port;
	LX_GFX_PORT_T	dst_port;

	LX_GFX_MANUAL_BLEND_CTRL_PARAM_T	blend_ctrl;
	memset( &blend_ctrl, 0x0, sizeof(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T) );

	memset( &src0_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &src1_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &dst_port,  0x0, sizeof(LX_GFX_PORT_T));

	src0_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	src0_port.surface_fd 	= src;
	src0_port.rect.x		= sx;
	src0_port.rect.y		= sy;
	src0_port.rect.w		= sw;
	src0_port.rect.h		= sh;

	src1_port.surface_type  = LX_GFX_SURFACE_TYPE_COLOR;
	src1_port.surface_color.pixel_format = LX_GFX_PIXEL_FORMAT_INDEX_0;
	src1_port.surface_color.pixel_value  = 0x00000000;
	src1_port.rect.x        = 0;
	src1_port.rect.y        = 0;
	src1_port.rect.w        = sw;
	src1_port.rect.h        = sh;

	dst_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	dst_port.surface_fd 	= dst;
	dst_port.rect.x			= dx;
	dst_port.rect.y			= dy;
	dst_port.rect.w			= sw;
	dst_port.rect.h			= sh;

	blend_ctrl.scaler.mode 		= LX_GFX_SCALER_MODE_BILINEAR;
	blend_ctrl.scaler.input.w	= sw;
	blend_ctrl.scaler.input.h	= sh;
	blend_ctrl.scaler.output.w	= dw;
	blend_ctrl.scaler.output.h	= dh;

	blend_ctrl.op_mode			= LX_GFX_OP_MODE_TWO_SRC_PORT;
	blend_ctrl.out_sel			= LX_GFX_OUT_SEL_ROP;
	blend_ctrl.rop				= LX_GFX_ROP_SRC;
	blend_ctrl.constant_color 	= 0x0;

	memcpy( &blend_ctrl.src0, &src0_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.src1, &src1_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.dst,  &dst_port, sizeof(LX_GFX_PORT_T));

	GFX_RunBlendOp(&blend_ctrl, g_gfx_cfg.screen_blit_cmd_delay );
	GFX_WaitGfxSync ( &blend_ctrl, TRUE );
}

void GFX_BlitSurface	( int src0, int src1, int dst, int sx, int sy, int sw, int sh, int dx, int dy, BOOLEAN fBlend )
{
	LX_GFX_PORT_T	src0_port;
	LX_GFX_PORT_T	src1_port;
	LX_GFX_PORT_T	dst_port;

	LX_GFX_MANUAL_BLEND_CTRL_PARAM_T	blend_ctrl;
	memset( &blend_ctrl, 0x0, sizeof(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T) );

	memset( &src0_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &src1_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &dst_port,  0x0, sizeof(LX_GFX_PORT_T));

	src0_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	src0_port.surface_fd 	= src0;
	src0_port.rect.x		= sx;
	src0_port.rect.y		= sy;
	src0_port.rect.w		= sw;
	src0_port.rect.h		= sh;

	if ( src1 > 0 )
	{
		src1_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
		src1_port.surface_fd 	= src1;
	}
	else
	{
		src1_port.surface_type				= LX_GFX_SURFACE_TYPE_COLOR;
	    src1_port.surface_color.pixel_format= LX_GFX_PIXEL_FORMAT_INDEX_0;
  		src1_port.surface_color.pixel_value	= 0x0;
	}
	src1_port.rect.x		= dx;
	src1_port.rect.y		= dy;
	src1_port.rect.w		= sw;
	src1_port.rect.h		= sh;

	dst_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	dst_port.surface_fd 	= dst;
	dst_port.rect.x			= dx;
	dst_port.rect.y			= dy;
	dst_port.rect.w			= sw;
	dst_port.rect.h			= sh;

	if ( fBlend )
	{
		src0_port.port_flag	|= LX_GFX_PORT_FLAG_PREMULTIPLY;
		src1_port.port_flag	|= LX_GFX_PORT_FLAG_PREMULTIPLY;
		dst_port.port_flag	|= LX_GFX_PORT_FLAG_DEMULTIPLY;

		blend_ctrl.xor_alpha_en 	= FALSE;
		blend_ctrl.xor_color_en		= FALSE;
		blend_ctrl.mux_alpha_m0		= LX_GFX_MUX_ALPHA_M0_ALPHA0;
		blend_ctrl.mux_color_m0		= LX_GFX_MUX_COLOR_M0_COLOR0;
		blend_ctrl.comp_rule_alpha	= LX_GFX_COMP_RULE_FACTOR;
		blend_ctrl.comp_rule_color	= LX_GFX_COMP_RULE_FACTOR;
		blend_ctrl.mux_factor0_alpha= LX_GFX_MUX_FACTOR_ONE;
		blend_ctrl.mux_factor1_alpha= LX_GFX_MUX_FACTOR_ONE_MINUS_SRC0_ALPHA;
		blend_ctrl.mux_factor0_color= LX_GFX_MUX_FACTOR_ONE;
		blend_ctrl.mux_factor1_color= LX_GFX_MUX_FACTOR_ONE_MINUS_SRC0_ALPHA;

		blend_ctrl.op_mode	= LX_GFX_OP_MODE_TWO_SRC_PORT;
		blend_ctrl.out_sel	= LX_GFX_OUT_SEL_BLEND;
		blend_ctrl.rop		= LX_GFX_ROP_NONE;
		blend_ctrl.constant_color = 0x0;
	}
	else
	{
		blend_ctrl.op_mode	= LX_GFX_OP_MODE_TWO_SRC_PORT;
		blend_ctrl.out_sel	= LX_GFX_OUT_SEL_ROP;
		blend_ctrl.rop		= LX_GFX_ROP_SRC;
		blend_ctrl.constant_color = 0x0;
	}

	memcpy( &blend_ctrl.src0, &src0_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.src1, &src1_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.dst,  &dst_port, sizeof(LX_GFX_PORT_T));

	GFX_RunBlendOp(&blend_ctrl, g_gfx_cfg.screen_blit_cmd_delay );
	GFX_WaitGfxSync ( &blend_ctrl, FALSE );
}

/** emulate global alpha modulation for POSD.
 *	POSD should made to GFX surface 
 *
 *	@param surf [IN] POSD surface
 *	@param w [IN] width of POSD
 *	@param h [IN] height of POSD
 *	@param alpha [IN] global alpha for POSD
 *
 */
void		GFX_FadeSurface		( int src, int dst, int w, int h, UINT32 alpha )
{
	LX_GFX_PORT_T	src0_port;
	LX_GFX_PORT_T	src1_port;
	LX_GFX_PORT_T	dst_port;

	LX_GFX_MANUAL_BLEND_CTRL_PARAM_T	blend_ctrl;
	memset( &blend_ctrl, 0x0, sizeof(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T) );

	/* alpha with 0xff doesn't need to support FADE */
	if ( alpha >= 0xff ) return;

	memset( &src0_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &src1_port, 0x0, sizeof(LX_GFX_PORT_T));
	memset( &dst_port,  0x0, sizeof(LX_GFX_PORT_T));

	src0_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	src0_port.surface_fd 	= src;
	src0_port.rect.x		= 0;
	src0_port.rect.y		= 0;
	src0_port.rect.w		= w;
	src0_port.rect.h		= h;

	src1_port.surface_type				= LX_GFX_SURFACE_TYPE_COLOR;
	src1_port.surface_color.pixel_format= LX_GFX_PIXEL_FORMAT_INDEX_0;
	src1_port.surface_color.pixel_value	= 0x0;

	dst_port.surface_type	= LX_GFX_SURFACE_TYPE_EXTERN_MEM_BUFFER;
	dst_port.surface_fd 	= dst;
	dst_port.rect.x			= 0;
	dst_port.rect.y			= 0;
	dst_port.rect.w			= w;
	dst_port.rect.h			= h;

#if 0
	src0_port.port_flag	|= LX_GFX_PORT_FLAG_PREMULTIPLY;
	src1_port.port_flag	|= LX_GFX_PORT_FLAG_PREMULTIPLY;
	dst_port.port_flag	|= LX_GFX_PORT_FLAG_DEMULTIPLY;
#endif

	blend_ctrl.xor_alpha_en 	= FALSE;
	blend_ctrl.xor_color_en		= FALSE;
	blend_ctrl.mux_alpha_m0		= LX_GFX_MUX_ALPHA_M0_ALPHA0_X_CONST_ALPHA;
	blend_ctrl.mux_color_m0		= LX_GFX_MUX_COLOR_M0_COLOR0;
	blend_ctrl.comp_rule_alpha	= LX_GFX_COMP_RULE_FACTOR;
	blend_ctrl.comp_rule_color	= LX_GFX_COMP_RULE_FACTOR;
	blend_ctrl.mux_factor0_alpha= LX_GFX_MUX_FACTOR_ONE;
	blend_ctrl.mux_factor1_alpha= LX_GFX_MUX_FACTOR_ZERO;//ONE_MINUS_SRC0_ALPHA;
	blend_ctrl.mux_factor0_color= LX_GFX_MUX_FACTOR_ONE;
	blend_ctrl.mux_factor1_color= LX_GFX_MUX_FACTOR_ZERO;//ONE_MINUS_SRC0_ALPHA;

	blend_ctrl.op_mode			= LX_GFX_OP_MODE_TWO_SRC_PORT;
	blend_ctrl.out_sel			= LX_GFX_OUT_SEL_BLEND;
	blend_ctrl.rop				= LX_GFX_ROP_NONE;
	blend_ctrl.constant_color	= (alpha<<24) | 0xffffff;

	memcpy( &blend_ctrl.src0, &src0_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.src1, &src1_port, sizeof(LX_GFX_PORT_T));
	memcpy( &blend_ctrl.dst,  &dst_port,  sizeof(LX_GFX_PORT_T));

	GFX_RunBlendOp(&blend_ctrl, g_gfx_cfg.screen_blit_cmd_delay );
	GFX_WaitGfxSync ( &blend_ctrl, FALSE );
}


void GFX_RotateSurface	( int src, int dst, int sx, int sy, int sw, int sh, int dx, int dy, UINT32 degree )
{
#define	USE_SRC_CACHE
#undef	USE_DST_CACHE

	OS_CACHE_MAP_T	src_cmap;
	OS_CACHE_MAP_T	dst_cmap;

	LX_GFX_SURFACE_SETTING_T src_setting;
	LX_GFX_SURFACE_SETTING_T dst_setting;
	int	rc;

	UINT32	src_offset=0;
	UINT32	dst_offset=0;
	UINT32	src_stride;
	UINT32	dst_stride;
	UINT8*	src_vaddr = NULL;
	UINT8*	dst_vaddr = NULL;
	int	ret = RET_ERROR;

	rc = GFX_GetSurfaceInfo ( src, &src_setting );
	GFX_CHECK_CODE( rc != RET_OK, goto func_exit, "can't get src info\n");
	rc = GFX_GetSurfaceInfo ( dst, &dst_setting );
	GFX_CHECK_CODE( rc != RET_OK, goto func_exit, "can't get dst info\n");

	/* get offset */
	src_offset = sy * src_setting.stride + (sx<<2) /* ARGB8888 */;
	dst_offset = dy * dst_setting.stride + (dx<<2) /* ARGB8888 */;
	src_stride = src_setting.stride;
	dst_stride = dst_setting.stride;

	/* ioremap */
	GFX_CHECK_CODE( RET_OK != OS_OpenCacheMap( &src_cmap, (ULONG)src_setting.phys_addr, src_setting.stride * src_setting.height ), goto func_exit, "src cache failed\n");
	GFX_CHECK_CODE( RET_OK != OS_OpenCacheMap( &dst_cmap, (ULONG)dst_setting.phys_addr, dst_setting.stride * dst_setting.height ), goto func_exit, "dst cache failed\n");

	src_vaddr = (UINT8*)src_cmap.virt_addr;
	dst_vaddr = (UINT8*)dst_cmap.virt_addr;

	OS_InvalCacheMap( &src_cmap, 0x0, src_cmap.length );

#define	SRC_XY_POS(x,y)		*(UINT32*)(src_vaddr + ((x)<<2) + (y)*src_stride)
#define	DST_XY_POS(x,y)		*(UINT32*)(dst_vaddr + ((x)<<2) + (y)*dst_stride)

	/* rotate */
	if ( degree == 90 )
	{
		int		x0, y0, x1, y1;
		UINT32	pxl;

		for ( y0 = 0; y0 < sh; y0++ )
		for ( x0 = 0; x0 < sw; x0++ )
		{
			pxl = SRC_XY_POS(sx+x0,sy+y0);
			x1 = (sh-1)-y0; y1 = x0;
			DST_XY_POS(dx+x1,dy+y1) = pxl;
		}
	}
	else if ( degree == 270 )
	{
		int		x0, y0, x1, y1;
		UINT32	pxl;

		for ( y0 = 0; y0 < sh; y0++ )
		for ( x0 = 0; x0 < sw; x0++ )
		{
			pxl = SRC_XY_POS(sx+x0,sy+y0);
			x1 = y0; y1 = (sw-1)-x0;
			DST_XY_POS(dx+x1,dy+y1) = pxl;
		}
	}

	OS_CleanCacheMap( &dst_cmap, 0x0, dst_cmap.length );
	ret = RET_OK; /* all work done */

func_exit:
	if (src_vaddr)	OS_CloseCacheMap(&src_cmap);
	if (dst_vaddr)	OS_CloseCacheMap(&dst_cmap);
}


static void GFX_WaitGfxSync( LX_GFX_MANUAL_BLEND_CTRL_T* blend_ctrl, BOOLEAN fAutoReset )
{
	int ret;
	UINT32 wait_time = GFX_CalcSyncTimeout( blend_ctrl, g_gfx_cfg.sync_wait_timeout );

	GFX_RunFlushCommand();
	ret = GFX_WaitSyncCommand( wait_time );
	if (ret != RET_OK)
	{
		GFX_ERROR("GFX kdriver draw failed\n");
		GFX_DumpHWOpParam( blend_ctrl, TRUE );
	}
}

