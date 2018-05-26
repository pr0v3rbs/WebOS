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
 * GFX module control Drivers linux module
 *
 *  author      sinchul.jung (sinchul.jung@lge.com)
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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>		 /**< printk() */
#include <linux/slab.h>			 /**< kmalloc() */
#include <linux/vmalloc.h>		 /**< kmalloc() */
#include <linux/fs.h>			 /**< everything\ldots{} */
#include <linux/types.h>		 /**< size_t */
#include <linux/fcntl.h>		 /**< O_ACCMODE */
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>		 /**< For request_region, check_region etc */
#include <asm/io.h>				 /**< For ioremap_nocache */
#include <linux/workqueue.h>	 /**< For working queue */
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>

#include "gfx_impl.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define ROUND32(width) ((((width) + 31) / 32) * 32 )

/*----------------------------------------------------------------------------------------
	Type Definitions
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
GFX_SURFACE_OBJ_T*				g_gfx_surf_list;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int 		GFX_AllocSurfaceMem			(UINT32 surface_fd);
static int 		GFX_FreeSurfaceMem			(UINT32 surface_fd);

static BOOLEAN	GFX_CheckSurfacePhysAddr 	(UINT32 phys_addr);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32					g_cr_idx;
static BOOLEAN					g_rgn_valid_mark[GFX_MAX_RGN_NUM];
static OS_RGN_T					g_rgn[GFX_MAX_RGN_NUM];

static LX_GFX_PHYS_MEM_INFO_T	g_gfx_mem_pool_info;		/** GFX memory pool info */
static int						g_gfx_mem_pool_alloc_size;	/** monitor surface allocation size */
static int						g_gfx_mem_pool_alloc_cnt;	/** monitor surface allocation count */

/*========================================================================================
	Implementation Group
========================================================================================*/

/** initialize GFX memory for surface
 *
 * @param	void
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_InitSurfaceMemory(void)
{
	int	i;
	int	ret = RET_ERROR;

	g_cr_idx = 0;
	g_gfx_mem_pool_info.phys_addr 	= gMemCfgGfx.surface[0].base;
	g_gfx_mem_pool_info.length	 	= gMemCfgGfx.surface[0].size;
	g_gfx_mem_pool_info.offset	 	= 0;

	g_gfx_mem_pool_alloc_size		= 0;
	g_gfx_mem_pool_alloc_cnt		= 0;

	memset(g_rgn_valid_mark, 0x0, sizeof(BOOLEAN)*GFX_MAX_RGN_NUM);
	memset(g_rgn, 0x0, sizeof(OS_RGN_T)*GFX_MAX_RGN_NUM);

	g_gfx_surf_list = (GFX_SURFACE_OBJ_T *)vmalloc(sizeof(GFX_SURFACE_OBJ_T)*GFX_MAX_SURFACE);
	GFX_CHECK_ERROR( NULL == g_gfx_surf_list, goto func_exit, "can't alloc surf_list\n");

	memset((void *)g_gfx_surf_list, 0x0, sizeof(GFX_SURFACE_OBJ_T)*GFX_MAX_SURFACE);

	for ( i=0; i<GFX_MAX_RGN_NUM; i++)
	{
		if ( gMemCfgGfx.surface[i].size > 0 )
		{
			ret = OS_InitRegionEx( &g_rgn[i], OS_RGN_MGR_TYPE_HMA, (void*)gMemCfgGfx.surface[i].base, gMemCfgGfx.surface[i].size, PAGE_SIZE );
			GFX_CHECK_ERROR( ret<0 , goto func_exit, "can't alloc surf[%d] region. sz %d\n", i, gMemCfgGfx.surface[i].size );

			g_rgn_valid_mark[i] = TRUE;
			printk("gfx surf rgn[%d] phys 0x%08x, len 0x%08x (%d MB)\n", i, gMemCfgGfx.surface[i].base, gMemCfgGfx.surface[i].size, gMemCfgGfx.surface[i].size>>20 );
		}
		else
		{
			g_rgn_valid_mark[i] = FALSE;
		}
	}

	ret = RET_OK; /* all work done */
func_exit:
	return ret;
}

/**
 *
 * allocate surface memory as imcoming param-LX_GFX_SURFACE_SETTING_PARAM_T
 *
 * @param	LX_GFX_SURFACE_SETTING_PARAM_T
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_AllocSurface(LX_GFX_SURFACE_SETTING_PARAM_T *surface)
{
	int i;
	int slot = -1;
	int ret  = RET_ERROR;

	GFX_TRACE_BEGIN();

	/* I would like to give the special fd to the external memory based surface */
	if( (ULONG)surface->surface_setting.phys_addr ) { i = 0; }
	else 											{ i = 0x80; }

	/* serach empty slot */
	for ( /* nop */ ; i < GFX_MAX_SURFACE; i++ )
	{
		if ( !g_gfx_surf_list[i].bAlloc ) { slot = i; break; }
	}

	GFX_CHECK_ERROR( slot < 0, goto func_exit, "surface pool(max %d) is full\n", GFX_MAX_SURFACE );
	GFX_PRINT("%s : empty slot = %d\n", __F__, slot );

	surface->surface_fd 					= slot;
	g_gfx_surf_list[slot].bAlloc 			= 1;
	g_gfx_surf_list[slot].surf.surface_fd	= slot;
	g_gfx_surf_list[slot].surf.type 		= surface->surface_setting.type;
	g_gfx_surf_list[slot].surf.pixel_format = surface->surface_setting.pixel_format;
	g_gfx_surf_list[slot].surf.width 		= surface->surface_setting.width;
	g_gfx_surf_list[slot].surf.height 		= surface->surface_setting.height;
	g_gfx_surf_list[slot].surf.alignment 	= surface->surface_setting.alignment;

	g_gfx_surf_list[slot].cIdx				= g_cr_idx++;
	g_gfx_surf_list[slot].cTick				= OS_GetMsecTicks();

	/* get task name */
	get_task_comm( g_gfx_surf_list[slot].psname, current );
	g_gfx_surf_list[slot].psname[TASK_COMM_LEN] = 0x0;

	GFX_TRACE("%s:%d -- fd %d, dim %dx%d\n", __F__, __L__, slot, g_gfx_surf_list[slot].surf.width, g_gfx_surf_list[slot].surf.height );

	/* if application doesn't know stride, caculate it here based on HW limitation
	 *
 	 * 1) basic alignment limitation is at least 4-byte
	 * 2) some HW such as OSD requires 16-byte aligned stride
	 * 3) 16x16 is the macro block size of MPEG
	 *
	 * As result, 16byte aligned stride is best choice to satify all surface usage.
	 *
	 * for exmaple, 32bppt surface with width 1366 produces stride value 5472 (same as 1368*4 )
	 *
	 */
	if(surface->surface_setting.stride == 0)
	{
		UINT32 stride;
		UINT32 width = surface->surface_setting.width;

		switch(surface->surface_setting.pixel_format)
		{
			case LX_GFX_PIXEL_FORMAT_INDEX_0:	stride = 0; break;

			case LX_GFX_PIXEL_FORMAT_INDEX_1:	stride = ROUND32(width*1)/8; break;
			case LX_GFX_PIXEL_FORMAT_INDEX_2:	stride = ROUND32(width*2)/8; break;
			case LX_GFX_PIXEL_FORMAT_INDEX_4:	stride = ROUND32(width*4)/8; break;

			case LX_GFX_PIXEL_FORMAT_INDEX_8:
			case LX_GFX_PIXEL_FORMAT_ALPHA_8:	stride = ROUND32(width*8)/8; break;

			case LX_GFX_PIXEL_FORMAT_Y8__Cb8_444__Cr8_444 :
			case LX_GFX_PIXEL_FORMAT_Cb8_420__Cr8_420:
			case LX_GFX_PIXEL_FORMAT_Cb8_422__Cr8_422: stride = ROUND32(width*8)/8; break;

			case LX_GFX_PIXEL_FORMAT_CbCr_420:		/*16*/
			case LX_GFX_PIXEL_FORMAT_CbCr_422:		/*16*/
			case LX_GFX_PIXEL_FORMAT_CbCr_444:		/*16*/
			case LX_GFX_PIXEL_FORMAT_Y0Cb0Y1Cr0_422:/*16*/ stride = ROUND32(width*16)/8; break;

			case LX_GFX_PIXEL_FORMAT_YCbCr655:
			case LX_GFX_PIXEL_FORMAT_AYCbCr2644:
			case LX_GFX_PIXEL_FORMAT_AYCbCr4633:
			case LX_GFX_PIXEL_FORMAT_AYCbCr6433:
			case LX_GFX_PIXEL_FORMAT_RGB565:
			case LX_GFX_PIXEL_FORMAT_ARGB1555:
			case LX_GFX_PIXEL_FORMAT_ARGB4444:
			case LX_GFX_PIXEL_FORMAT_ARGB6343:	stride = ROUND32(width*16)/8; break;

			case LX_GFX_PIXEL_FORMAT_AYCbCr8888:
			case LX_GFX_PIXEL_FORMAT_ARGB8888:	stride = ROUND32(width*32)/8; break;
		}

#if 0
		/* make 16 byte aligned stride */
		g_gfx_surf_list[slot].surf.stride = LX_CALC_ALIGNED_VALUE(stride,4);
#else
		/* FIXME: disable 16byte align due to some side effect - HAL_GAL_DecodeImage etc */
		g_gfx_surf_list[slot].surf.stride = stride;
#endif
	}
	else
	{
		g_gfx_surf_list[slot].surf.stride = surface->surface_setting.stride;
	}

	/*	raxis.lim (2010/05/13)
	 *	-- 	we need fill surface memory info even though it is from external memory
	 *		since application can query surfaec memory regardless of surface type.
	 *		but it is important that valid information may be only physical address or length.
	 */
	if( surface->surface_setting.phys_addr )
	{
		g_gfx_surf_list[slot].surf.phys_addr= surface->surface_setting.phys_addr;
		g_gfx_surf_list[slot].mem.phys_addr = surface->surface_setting.phys_addr;
		g_gfx_surf_list[slot].mem.length    = surface->surface_setting.stride * surface->surface_setting.height;
		g_gfx_surf_list[slot].mem.offset    = (UINT32)surface->surface_setting.phys_addr - (UINT32)g_gfx_mem_pool_info.phys_addr;

		ret = RET_OK;
	}
	else
	{
		ret = GFX_AllocSurfaceMem(surface->surface_fd);
	}

	GFX_CHECK_ERROR( ret != RET_OK, g_gfx_surf_list[slot].bAlloc = 0; goto func_exit, "fail to alloc surface (%d)\n", surface->surface_fd );

	g_gfx_surf_list[slot].bPalette = 0;

	/* return surface_settings data */
	memcpy( &surface->surface_setting, &g_gfx_surf_list[slot].surf, sizeof(LX_GFX_SURFACE_SETTING_T));

	ret = RET_OK;
func_exit:
	if ( ret != RET_OK )
	{
		/* raxis.lim (2012/11/12) print memory pool info for debug */
		LX_GFX_MEM_STAT_T	gfx_mem_stat;
		int					surface_cnt = 0;

		(void)GFX_GetSurfaceMemoryStat( &gfx_mem_stat );
		printk("gfx memory pool info = ptr %p, len %d KB, alloc %d KB, free %d KB\n",
				gfx_mem_stat.surface_mem_base, gfx_mem_stat.surface_mem_length>>10,
				gfx_mem_stat.surface_mem_alloc_size>>10, gfx_mem_stat.surface_mem_free_size>>10 );

		for ( i=0; i< GFX_MAX_SURFACE; i++ )
		{
			if ( g_gfx_surf_list[i].bAlloc ) surface_cnt++;
		}
		printk("total %d surface(s) allocated\n", surface_cnt );
	}
	GFX_TRACE_END();
	return ret;
}

static BOOLEAN	GFX_CheckSurfacePhysAddr ( UINT32 phys_addr )
{
	BOOLEAN	b_range_check = FALSE;

	b_range_check =	 ( ( g_rgn_valid_mark[0] && phys_addr >= gMemCfgGfx.surface[0].base && phys_addr < (UINT32)gMemCfgGfx.surface[0].base + gMemCfgGfx.surface[0].size )
				    || ( g_rgn_valid_mark[1] && phys_addr >= gMemCfgGfx.surface[1].base && phys_addr < (UINT32)gMemCfgGfx.surface[1].base + gMemCfgGfx.surface[1].size ) )? TRUE: FALSE;

	return b_range_check;
}

/** free allocated surface memory which got from GFX_AllocSurface
 *
 * @param	int surface_fd
 * @return	int	0 : OK , -1 : NOT OK
 */
int GFX_FreeSurface(UINT32 surface_fd)
{
	ULONG phys_addr;
	int ret = RET_ERROR;
	GFX_TRACE_BEGIN();

	/* check error condition */
	GFX_CHECK_ERROR( surface_fd >= GFX_MAX_SURFACE,  goto func_exit, "invalid fd 0x%x(%d)\n", surface_fd, surface_fd );
	GFX_CHECK_ERROR( !g_gfx_surf_list[surface_fd].bAlloc, goto func_exit, "not allocated fd 0x%x(%d)\n", surface_fd, surface_fd );

	phys_addr = (UINT32)g_gfx_surf_list[surface_fd].surf.phys_addr;

	/* since surface is allocated with the external memory, we should check phys_addr range */
	if ( g_gfx_surf_list[surface_fd].surf.type == LX_GFX_SURFACE_TYPE_MEM_BUFFER && GFX_CheckSurfacePhysAddr(phys_addr) )
	{
       GFX_FreeSurfaceMem(surface_fd);
	}

	/*	raxis.lim (2010/07/01)
	 *	-- palettte memory should be free when paleltte data is available.
	 *	-- bPalette variable seems not enough to free palette data.
	 */
	if( g_gfx_surf_list[surface_fd].pal )
	{
		OS_Free( g_gfx_surf_list[surface_fd].pal );
	}

	memset( &g_gfx_surf_list[surface_fd], 0x0, sizeof(GFX_SURFACE_OBJ_T) );

	ret = RET_OK; /* all work done */
func_exit:
	GFX_TRACE_END();
	return ret;
}

int	GFX_QuerySurface (LX_GFX_SURFACE_QUERY_PARAM_T*  param)
{
	LX_GFX_SURFACE_SETTING_T*	ret_settings = NULL;
	int ret = RET_ERROR;

	GFX_TRACE_BEGIN();

	switch(param->query_type)
	{
		case LX_GFX_SURFACE_QUERY_TYPE_SURFACE_FD:
		{
			int	fd = param->query_data.surface_fd;

			GFX_CHECK_ERROR( fd < 0, goto func_exit, "invalid fd 0x%x(%d)\n", fd, fd );
			GFX_CHECK_ERROR( fd >= GFX_MAX_SURFACE,  goto func_exit, "invalid fd 0x%x(%d)\n", fd, fd );
			GFX_CHECK_ERROR( !g_gfx_surf_list[fd].bAlloc, goto func_exit, "non-allocated fd 0x%x(%d)\n", fd, fd );

			ret_settings = &(g_gfx_surf_list[fd].surf);
		}
		break;

		case LX_GFX_SURFACE_QUERY_TYPE_SURFACE_OFFSET:
		case LX_GFX_SURFACE_QUERY_TYPE_SURFACE_PHYS_ADDR:
		{
			int	i;
			ULONG phys_addr = ( param->query_type == LX_GFX_SURFACE_QUERY_TYPE_SURFACE_PHYS_ADDR ) ?
								param->query_data.surface_phys_addr :
								param->query_data.surface_offset + (UINT32)g_gfx_mem_pool_info.phys_addr ;

			for (i=0; i<GFX_MAX_SURFACE; i++ )
			{
				if ( g_gfx_surf_list[i].bAlloc && (UINT32)g_gfx_surf_list[i].surf.phys_addr == phys_addr )
				{
					ret_settings = &(g_gfx_surf_list[i].surf); break;
				}
			}
		}
		break;

		default:
			GFX_ERROR("uknown query type\n"); goto func_exit;
	}

	GFX_CHECK_CODE(NULL==ret_settings, goto func_exit, "sourface not found. type=%d value=0x%x\n", param->query_type, param->query_data.value );
	memcpy( &param->surface_setting, ret_settings, sizeof(LX_GFX_SURFACE_SETTING_T));

	ret = RET_OK;
func_exit:
	GFX_TRACE_END();
	return ret;
}

/** simplfied form of GFX_QuerySurface
 *
 *
 */
int      GFX_GetSurfaceInfo ( int surface_fd, LX_GFX_SURFACE_SETTING_T* surface_info )
{
	int	rc;
	LX_GFX_SURFACE_QUERY_PARAM_T query_param;

	memset( &query_param, 0x0, sizeof(LX_GFX_SURFACE_QUERY_PARAM_T) );
	query_param.query_type = LX_GFX_SURFACE_QUERY_TYPE_SURFACE_FD;
	query_param.query_data.surface_fd = surface_fd;

	rc = GFX_QuerySurface( &query_param );
	GFX_CHECK_CODE( rc != RET_OK, return rc, "invalid surface fd 0x%x \n", surface_fd );

	memcpy( surface_info, &query_param.surface_setting, sizeof(LX_GFX_SURFACE_SETTING_T) );

	return rc;
}

/**
 *
 * free allocated surface's image memory which got from GFX_FreeSurfaceMem
 *
 * @param	int surface_fd
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
static int GFX_FreeSurfaceMem(UINT32 surface_fd)
{
	int ret = RET_OK;

	g_gfx_mem_pool_alloc_size -= g_gfx_surf_list[surface_fd].mem.length;
	g_gfx_mem_pool_alloc_cnt  -= 1;

	{
		int	i;
		OS_RGN_INFO_T   mem_info;
		UINT32			surf_addr = (UINT32)g_gfx_surf_list[surface_fd].surf.phys_addr;

		for ( i=0; i<GFX_MAX_RGN_NUM && g_rgn_valid_mark[i]; i++)
		{
			(void)OS_GetRegionInfo ( &g_rgn[i], &mem_info );

			if ( surf_addr >= (UINT32)mem_info.phys_mem_addr && surf_addr < (UINT32)mem_info.phys_mem_addr+mem_info.length )
			{
				OS_FreeRegion ( &g_rgn[i], (void*)surf_addr );
			}
		}
	}

	return ret;
}

/**
 *
 * allocate surface's image memory which got from GFX_AllocSurfaceMem
 *
 * @param	int surface_fd , void *ptr
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
static int GFX_AllocSurfaceMem(UINT32 surface_fd)
{
	int		mem_length;
	void*	ptr=NULL ;
	int		ret = RET_ERROR;

	mem_length = g_gfx_surf_list[surface_fd].surf.height * g_gfx_surf_list[surface_fd].surf.stride;
	mem_length = LX_CALC_ALIGNED_VALUE( mem_length, 12 /* 4K */ );

	{
		int	i;
		for ( i=0; i<GFX_MAX_RGN_NUM && g_rgn_valid_mark[i]; i++ )
		{
			ptr = OS_MallocRegion ( &g_rgn[i], mem_length );
			if ( ptr ) { /* allocation success */ break; }
		}
		GFX_CHECK_ERROR( NULL == ptr, goto func_exit, "out of surface (%d)\n", i );
	}

	g_gfx_surf_list[surface_fd].surf.phys_addr  = (UINT32)ptr;
	g_gfx_surf_list[surface_fd].mem.phys_addr   = (UINT32)ptr;
	g_gfx_surf_list[surface_fd].mem.length      = mem_length;
	g_gfx_surf_list[surface_fd].mem.offset		= (UINT32)ptr - (UINT32)g_gfx_mem_pool_info.phys_addr;

	g_gfx_mem_pool_alloc_size += g_gfx_surf_list[surface_fd].mem.length;
	g_gfx_mem_pool_alloc_cnt  += 1;

	ret = RET_OK;
func_exit:
	return ret;
}

/**
 *
 * get surface's image memory inform
 *
 * @param	LX_GFX_SURFACE_MEM_INFO_PARAM_T *mem
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_GetSurfaceMemory(LX_GFX_SURFACE_MEM_INFO_PARAM_T *mem)
{
	memcpy( &mem->surface_mem, &(g_gfx_surf_list[mem->surface_fd].mem), sizeof(LX_GFX_PHYS_MEM_INFO_T));

	GFX_TRACE("fd:0x%x(%d), phys_addr:0x%x, offset:%d, length:%d\n",
				mem->surface_fd, mem->surface_fd, mem->surface_mem.phys_addr, mem->surface_mem.offset, mem->surface_mem.length );
	return RET_OK;
}


/**
 * mmap the surface's image physical memory to virtual memory for using user level
 *
 * @param	struct vm_area_struct *vma
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_SurfaceMmap(struct file *file, struct vm_area_struct *vma)
{
    ULONG	phy_start;
	ULONG	phy_end;
    ULONG	off	= vma->vm_pgoff << PAGE_SHIFT;
    ULONG	size= vma->vm_end - vma->vm_start;
    ULONG	end	= PAGE_ALIGN(off + size);

	GFX_DEV_CTX_T*	dev_ctx = file->private_data;

	/* check size is page aligned */
	if (size & (PAGE_SIZE-1)) return -EINVAL;

	phy_start = gMemCfgGfx.surface[0].base & PAGE_MASK;

#if 0
	phy_end   = PAGE_ALIGN( phy_start + mem_size);
	if ( phy_start <= offset && end <= phy_end ) goto allowed;
    return -EINVAL;
#else
	PARAM_UNUSED(phy_end);
	PARAM_UNUSED(end);
#endif

	if ( !dev_ctx->cached_mmap )
	{
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	}

	off +=  phy_start;
	vma->vm_pgoff = off >> PAGE_SHIFT;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	vma->vm_flags |= VM_RESERVED;
#else
	vma->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;
#endif
	if (remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT, size, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

/** save the pallete date to surface
 *
 */
int GFX_SetSurfacePalette(int surface_fd, int size , UINT32 *data)
{
	int ret = RET_ERROR;

	GFX_CHECK_ERROR( g_gfx_surf_list[surface_fd].surf.width == 0 || g_gfx_surf_list[surface_fd].surf.height == 0,
						goto func_exit, "invalid palette op on free surface. fd 0x%x(%d)\n", surface_fd, surface_fd );

	/*	raxis.lim (2010/07/01) -- bug fix
	 *	-- 	the real palette memory is allocated when it is set for the first time.
	 *	-- 	We know the maximum palette size is 256 for 8bpp surface, so it is very normal
	 *		to allocate the maximum 256 palette entries.
	 */
	if ( !g_gfx_surf_list[surface_fd].pal )
	{
		g_gfx_surf_list[surface_fd].pal = (UINT32*)OS_Malloc( sizeof(UINT32)*256 );
	}

	GFX_CHECK_ERROR( !g_gfx_surf_list[surface_fd].pal, goto func_exit, "out of resource. check memory usage\n");

	memcpy( (void *)g_gfx_surf_list[surface_fd].pal , (void *)data , sizeof(UINT32)*size);

	/* 	raxis.lim (2010/07/01) -- bug fix
	 *	-- PalSize should be number of palette entry not byte size of palette data
	 *	-- bPalDownload field should be removed ( GFX should always download palette when blit time )
	 */
	g_gfx_surf_list[surface_fd].bPalette 	= 1;
	g_gfx_surf_list[surface_fd].bPalDownload= 0;
	g_gfx_surf_list[surface_fd].PalSize 	= size;

	GFX_PRINT("fd 0x%x(%d), palsz %d, datap %p\n",
				surface_fd, surface_fd, g_gfx_surf_list[surface_fd].PalSize, g_gfx_surf_list[surface_fd].pal );

	ret = RET_OK;
func_exit:
	return ret;
}

/** get the pallete date from surface
 *
 */
int GFX_GetSurfacePalette(int surface_fd, int size , UINT32 *data)
{
	int ret = RET_OK;

	if( (g_gfx_surf_list[surface_fd].surf.width == 0) || (g_gfx_surf_list[surface_fd].surf.height == 0) || (g_gfx_surf_list[surface_fd].pal == NULL ) )
	{
		GFX_ERROR("ERROR : tried to save palette data on free surface [%d]\n" , surface_fd);
		GFX_TRACE_END();
		return RET_ERROR;
	}

	memcpy( (void*)data, (void *)g_gfx_surf_list[surface_fd].pal, sizeof(UINT32)*size);

	GFX_PRINT("fd 0x%x(%d), palSize %d, palData %p\n",
				surface_fd, surface_fd, g_gfx_surf_list[surface_fd].PalSize, g_gfx_surf_list[surface_fd].pal );

	return ret;
}

/**	get memory statistics for GFX surface memory
 *	this function is used in IOCTL for debug, and in gfx proc.
 *	application can use this information to monitor memroy usage.
 *
 *	@param 	pMemStat [IN] pointer to LX_GFX_MEM_STAT_T ( surface memory info )
 *	@return RET_OK
 *
 */
int GFX_GetSurfaceMemoryStat ( LX_GFX_MEM_STAT_T* pMemStat )
{
	int	i;
	int	surf_cnt = 0;
	int rgn_total_mem = 0;
	UINT32	min_addr = (UINT32)-1;
	UINT32	max_addr = 0;

	pMemStat->surface_max_num		= GFX_MAX_SURFACE;
	pMemStat->surface_mem_base 		= (void*)gMemCfgGfx.surface[0].base;

	for (i=0;i<GFX_MAX_RGN_NUM && g_rgn_valid_mark[i]; i++)
	{
		UINT32 _start_addr	= gMemCfgGfx.surface[i].base;
		UINT32 _end_addr	= gMemCfgGfx.surface[i].base + gMemCfgGfx.surface[i].size;

		if ( _start_addr < min_addr ) min_addr = _start_addr;
		if ( _end_addr   > max_addr ) max_addr = _end_addr;
	}
	pMemStat->surface_mem_length = max_addr - min_addr;

	pMemStat->surface_mem_alloc_size= g_gfx_mem_pool_alloc_size;

	for (i=0;i<GFX_MAX_RGN_NUM && g_rgn_valid_mark[i]; i++)
	{
		rgn_total_mem += gMemCfgGfx.surface[0].size;
	}
	pMemStat->surface_mem_free_size = rgn_total_mem - g_gfx_mem_pool_alloc_size;

	/* surface count should include both internal surface and external surface */
	for ( i=0; i< GFX_MAX_SURFACE; i++ )
	{
		if ( g_gfx_surf_list[i].bAlloc ) surf_cnt++;
	}
	pMemStat->surface_alloc_num	= surf_cnt;

	return RET_OK;
}

/** allocate dynamic buffer from GFX surface memory
 *
 *  @note buffer allocation may fail when too-big-sized buffer requested. so app should check if return address is NULL or NULL
 *
 *  @param size [IN] buffer size in byte
 *  @return physical address if success, 0x0 if failed
 */
UINT32          GFX_AllocBuffer             (UINT32 size)
{
	void* ptr;

	if ( size == 0 ) size = (1<<PAGE_SHIFT);

//	GFX_LockDevice();
	ptr = OS_MallocRegion ( &g_rgn[0], size );
	GFX_CHECK_ERROR( NULL==ptr, /* nop */, "out of surf region ? sz %d\n", size );

//	GFX_UnlockDevice();
	return (UINT32)ptr;
}

/** free dynamic buffer
 *
 *  @param phys_addr [IN] physical address
 */
void            GFX_FreeBuffer              (UINT32 phys_addr )
{
//	GFX_LockDevice();
	GFX_CHECK_ERROR( 0x0==phys_addr, return, "null param\n");

	OS_FreeRegion ( &g_rgn[0], (void*)phys_addr );
//	GFX_UnlockDevice();
}

int     GFX_GetSurfaceFdList	(int* surf_list, int max_surf, int* num_surf )
{
	int	i;
	int	j;

	GFX_CHECK_ERROR( NULL==surf_list || NULL==num_surf, return RET_ERROR, "null param\n");

	for (i=0, j=0; i<GFX_MAX_SURFACE && j<max_surf; i++ )
	{
		if ( g_gfx_surf_list[i].bAlloc )
		{
			surf_list[j++] = i;
		}
	}
	*num_surf = j;
	return RET_OK;
}

/** @} */
