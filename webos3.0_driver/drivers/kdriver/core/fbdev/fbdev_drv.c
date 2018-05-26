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
/*
/@file name : fbdev_module.c
/@description : fbdev ioctl's function
**/

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#define USE_FBDEV_VSYNC_UEVENT						/* support vsync event notifier */
#undef	USE_FBDEV_ISR_TIME_CHK						/* support vsync ISR duration monitor */

#if CONFIG_LX_BOARD_FPGA
#define	FBDEV_DEFAULT_VSYNC_USEC_TIMEOUT	4000000 /* max 4M us = 4000 ms = 0.25 fps */
#else
#define	FBDEV_DEFAULT_VSYNC_USEC_TIMEOUT	50000	/* max 50,000 us = 50 ms = 20 fps */
#endif

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>		 /**< FBDEV_WARN() */
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>			 /**< kmalloc() */
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <asm/io.h>
#include <linux/fs.h>			 /**< everything\ldots{} */
#include <linux/console.h>		 /** acquire_console_sem */
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#include <asm/hardware.h>
#endif

#include <asm/uaccess.h>

#include <linux/device.h>

#include <linux/interrupt.h>	 /** irqreturn_t */
#include <linux/irq.h>
#include <asm/irq.h>

#include "fbdev_impl.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"
#include "gfx_cfg.h"
#include "gfx_draw.h"

/*----------------------------------------------------------------------------------------
    Constant Definitions
----------------------------------------------------------------------------------------*/
static int FBDEV_freeze		(struct device* dev);
static int FBDEV_thaw		(struct device* dev);
static int FBDEV_restore	(struct device* dev);
static int FBDEV_suspend	(struct device* dev);
static int FBDEV_resume		(struct device* dev);

#define LCD_PANEL_CLOCK 	180000	 // not be used

/*----------------------------------------------------------------------------------------
    Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern void		FBDEV_PROC_Init		(void);
extern void		FBDEV_PROC_Cleanup	(void);

/*----------------------------------------------------------------------------------------
    Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    global Variables
----------------------------------------------------------------------------------------*/
UINT32				g_fbdev_vsync_cnt		= 0;
int    			 	g_fbdev_debug_fd		= -1;
int					g_fbdev_trace_depth		= 0;
int					g_fbdev_vsync_debug		= 1;
FBDEV_FRC_CTRL_T	g_fbdev_frc_ctrl;

static int			g_fbdev_irq_run_flag  = 0;				// isr handler is active ?
static DEFINE_SPINLOCK(g_fbdev_vsync_check_spinlock);		// spinlock for check vsync state
static DECLARE_WAIT_QUEUE_HEAD(g_fbdev_vsync_wq);			// for sync

FBDEV_HW_CTX_T		g_fbdev_hw_ctx;

/* default vsync timeout value in usec */
int					g_fbdev_vsync_timeout	= FBDEV_DEFAULT_VSYNC_USEC_TIMEOUT;
module_param_named( fbdev_vsync_timeout, g_fbdev_vsync_timeout, int, 0644 );

/* if enabled, g_fbdev_vsync_timeout is tuned automatically */
int					g_fbdev_use_vsync_auto_tune = 0;
module_param_named( fbdev_vsync_auto_tune, g_fbdev_use_vsync_auto_tune, int, 0644 );

/* multiply vsync-wait timeout for debug/test */
static int			g_fbdev_vsync_cnt_on_flip = 1;
module_param_named( fbdev_vsync_cnt_on_flip, g_fbdev_vsync_cnt_on_flip, int, 0644 );

/* if enable, unncessary vsync-wait is disabled when image on the front buffer is updated */
static int			g_fbdev_use_flip_check	= 1;
module_param_named( fbdev_flip_check, g_fbdev_use_flip_check, int, 0644 );

/* if enabled, isr time infor is printed periodically */
int					g_fbdev_use_isr_mon	= 0;
module_param_named( fbdev_isr_mon, g_fbdev_use_isr_mon, int, 0644 );

/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static irqreturn_t FBDEV_ISRHandler(int irq, void *dev_id, struct pt_regs *regs);

static int FBDEV_open(struct fb_info *info , int user);
static int FBDEV_close(struct fb_info *info, int user);
static int FBDEV_ioctl(struct fb_info *info, unsigned int cmd , unsigned long arg);

static FBDEV_WIN_CTX_T*		FBDEV_InitWinCtx(int fb_dev_id, char* name);
static void 				FBDEV_InitLinuxFB(FBDEV_WIN_CTX_T *wctx, struct fb_var_screeninfo *var, char *id);
static int					FBDEV_WaitVSync(int fb_dev_id, int vsync_cnt_on_flip);

#ifdef USE_FBDEV_VSYNC_UEVENT
static FBDEV_VSYNC_UEVENT_T	vsync_ev_ctrl;
static int		g_fbdev_vsync_ev_init = 0;
static ssize_t	vsync_show_event(struct device* dev, struct device_attribute* attr, char* buf);

static DEVICE_ATTR(vsync_event, S_IRUGO, vsync_show_event, NULL);

static struct attribute* vsync_fs_attrs[] =
{
	&dev_attr_vsync_event.attr,
	NULL,
};
static struct attribute_group vsync_fs_attr_group =
{
	.attrs = vsync_fs_attrs,
};
#endif

/*----------------------------------------------------------------------------------------
    Static Variables
----------------------------------------------------------------------------------------*/
static FBDEV_CTX_T	g_dm_ctx_static;
FBDEV_CTX_T*		g_dm_ctx = &g_dm_ctx_static;

static FBDEV_DRVDATA_T* g_fbdev_drv_data = NULL;

/*========================================================================================
    Implementation Group
========================================================================================*/
/**	update screen !!
 *
 *	@param fb_dev_id [IN] OSD id
 *	@param fb_screen [IN] new screen description
 *	@param b_free_run [IN] if TRUE, OSD doesn't wait for vsync, if FALSE, OSD waits for vsync (default)
 *	@param RET_OK if success, RET_ERROR otherwise
 *
 */
int	FBDEV_FlipFBImage( int fb_dev_id, LX_FBDEV_FB_IMAGE_T* fb_screen, BOOLEAN b_free_run )
{
	// TODO: check
	BOOLEAN b_hdr_update = TRUE;
	BOOLEAN	b_vsync_wait = TRUE;
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx( fb_dev_id );

	FBDEV_CHECK_ERROR( NULL==wctx, return RET_ERROR, "invalid layer %d\n", fb_dev_id );

	FBDEV_NOTI("fb(%d) <begin>\n", fb_dev_id );

	FBDEV_MON_NotiRefresh(fb_dev_id);

	if ( g_fbdev_use_flip_check	)
	{
		/* raxis.lim (2011/11/19)
		 * if requst address is the same as current view pointer, DO NOT update header nor wait vsync.
		 * But since we should support the various OSD mode such as UD mode, FRC SEP with L9B0, it is necessary to write OSD header !!
		 * As you know, FBDEV_WriteOSDHeader function have many chip workaround code !!! it is good to enter that function to cope with any situation.
		 *
		 * more code review is needed to check the below code works well with the old L9 chips.
		 */
		if ( wctx->pixel.osd_bm_paddr == fb_screen->phys_addr )
		{
			switch( fb_dev_id )
			{
				case LX_FBDEV_ID_OSD3:	/* cursor */
				{
					if (lx_chip_rev() >= LX_CHIP_REV(L9,B1) ) { b_hdr_update = TRUE; b_vsync_wait = FALSE; }
				}
				break;

				case LX_FBDEV_ID_OSD0:
				case LX_FBDEV_ID_OSD1:
				case LX_FBDEV_ID_OSD2:	/* graphic */
				default:
				{
					if (lx_chip_rev() >= LX_CHIP_REV(L9,B1) ) { b_hdr_update = TRUE; b_vsync_wait = FALSE; }
				}
				break;
			}
		}
	}

	/* update OSD header info every screen update
	 *
	 * @note width & height info is not used !!!
	 */
	if ( b_hdr_update )
	{
		UINT8	curr_bpp = wctx->pixel.bits_per_pixel;

		wctx->pixel.osd_bm_paddr	= fb_screen->phys_addr;
		wctx->pixel.stride			= fb_screen->stride;
		wctx->pixel.bits_per_pixel	= fb_screen->bits_per_pixel;
//		wctx->pixel.pixel_depth		= FBDEV_GetOSDPxlDepth( fb_screen->bits_per_pixel );
//		wctx->pixel.pixel_fmt		= FBDEV_GetOSDPxlFmt( fb_screen-bits_per_pixel );

//		wctx->fb_base_phys		 	= fb_screen->phys_addr;
//		wctx->screen_dma			= fb_screen->phys_addr;

//		printk("[fb] osd %d <begin> - bm 0x%08x pitch %d, bpp %d\n", fb_dev_id, wctx->pixel.osd_bm_paddr, wctx->pixel.stride, wctx->pixel.bits_per_pixel );

		if ( curr_bpp != fb_screen->bits_per_pixel )
		{
			//printk("[fb] POSD %d - change endian from %d bpp to %d bpp\n", fb_dev_id,  curr_bpp, fb_screen->bits_per_pixel );
			FBDEV_SetOSDImageEndian( fb_dev_id, fb_screen->bits_per_pixel );
		}

		FBDEV_WriteOSDHeader(&wctx->pixel);
	}

	/* raxis.lim (2012/06/14) -- fbdev supports freerun mode ( non-vsync )  */
	if ( b_vsync_wait == TRUE && wctx->status.b_osd_hdr_write == TRUE && b_free_run == FALSE )
	{
		FBDEV_WaitVSync( fb_dev_id, wctx->vsync_cnt_on_flip );
	}

	FBDEV_NOTI("fb(%d) <end>\n", fb_dev_id );

	return RET_OK;
}

int	FBDEV_UpdateFBScreen( int fb_dev_id, LX_FBDEV_FB_SCREEN_T* fb_screen )
{
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx( fb_dev_id );

	FBDEV_CHECK_ERROR( NULL==wctx, return RET_ERROR, "invalid layer %d\n", fb_dev_id );
	FBDEV_CHECK_ERROR( LX_FBDEV_SCREEN_OPMASK_NONE==fb_screen->opmask, return RET_ERROR, "invalid opmask : 0x0\n" );

	/* disable osd header during processing 3D OSD and layer control */
	wctx->status.b_osd_hdr_write = FALSE;

	if ( fb_screen->opmask & LX_FBDEV_SCREEN_OPMASK_LAYER_CTRL )
	{
		FBDEV_SetConfig( wctx, &fb_screen->layer_ctrl );
	}

	if ( fb_screen->opmask & LX_FBDEV_SCREEN_OPMASK_TRID_CTRL )
	{
		FBDEV_SetTridCtrl( fb_dev_id, &fb_screen->trid_ctrl );
	}

	/* reenable osd header before updating fb image */
	wctx->status.b_osd_hdr_write = TRUE;

	if ( fb_screen->opmask & LX_FBDEV_SCREEN_OPMASK_IMG_CTRL )
	{
		FBDEV_FlipFBImage( fb_dev_id, &fb_screen->img_ctrl, FALSE );
	}
	else
	{
		FBDEV_WriteOSDHeader(&wctx->pixel);
	}

	return RET_OK;
}

/*========================================================================================
    Implementation Group ( FB interface )
========================================================================================*/
/**
 *      Pans the display.
 *      @var: frame buffer variable screen structure
 *      @info: frame buffer structure that represents a single frame buffer
 *
 *      Pan (or wrap, depending on the `vmode' field) the display using the
 *      `xoffset' and `yoffset' fields of the `var' structure.
 *      If the values don't fit, return -EINVAL.
 *
 *      Returns negative errno on error, or zero on success.
 */
static int FBDEV_pan_display ( struct fb_var_screeninfo* var, struct fb_info* fbinfo )
{
	int rc = 0;
	FBDEV_WIN_CTX_T* wctx = (FBDEV_WIN_CTX_T *)fbinfo->par;
	int		fb_dev_id = wctx->fb_dev_id;
	UINT32	start = 0, offset = 0;

	/* support pan_display only OSD driver is configured to LinuxFB */
	FBDEV_CHECK_ERROR( FALSE==wctx->linux_fb_mode, return RET_ERROR, "fb(%d) LinuxFB disabled..\n", fb_dev_id );
	FBDEV_DEBUG("fb(%d)::FBDEV_pan_display\n", fb_dev_id );

	/* ignore OSD update if current OSD is for FRC header emulation */
	if ( fb_dev_id == g_fbdev_frc_ctrl.frc_hdr_emul_dev && g_fbdev_frc_ctrl.b_frc_sep_enable && g_fbdev_frc_ctrl.b_frc_hdr_emul )
	{
		FBDEV_WARN("<warning> ignore OSD(%d) for FRC simul\n", fb_dev_id );
		return RET_OK; /* do nothing */
	}

#if 0
	/* ignore OSD update if current OSD is freezed !!! */
	if ( wctx->freeze_ctrl_mask & LX_FBDEV_FREEZE_MASK_PAN_DISPLAY )
	{
		FBDEV_WARN("<warning> OSD(%d) is freezed. mask=0x%x\n", fb_dev_id, wctx->freeze_ctrl_mask );
		return RET_OK; /* do nothing */
	}
#endif

#ifdef FBDEV_FLIP_LOCK
	OS_LockMutex( &wctx->fb_dev_mtx );
#endif

	/* raxis.lim (2012/10/25)
	 * code block to handle input window offset is moved to each header handler function.
	 * ( h13/fbdev_hw_h13a0.c in case of H13 )
	 * but DON'T FORGET var->xoffset, var->yoffset should be processed here becuase this value is
	 * changed in application to handle double buffering
	 */
	offset	+= var->yoffset * fbinfo->fix.line_length;
	offset	+= var->xoffset * var->bits_per_pixel / 8;
	start	= (UINT32)fbinfo->fix.smem_start + offset;
	start	= LX_CALC_ALIGNED_VALUE(start,4);	/* gMAU requests 16byte aligned address */

	FBDEV_DEBUG("var->yoff = %d, xoff = %d, stride = %d, bpp %d, smem_start 0x%08x, fbmem 0x%08x\n",
		var->yoffset, var->xoffset, fbinfo->fix.line_length, var->bits_per_pixel, (UINT32)fbinfo->fix.smem_start, (UINT32)start );
	{
		BOOLEAN					b_free_run = FALSE;
		LX_FBDEV_FB_IMAGE_T 	fb_screen;
		memset( &fb_screen, 0x0, sizeof(LX_FBDEV_FB_IMAGE_T));

		fb_screen.phys_addr 	= start;
		fb_screen.stride		= fbinfo->fix.line_length; //wctx->pixel.stride;
		fb_screen.bits_per_pixel= fbinfo->var.bits_per_pixel;
//		fb_screen.width			= fbinfo->var.xres;
//		fb_screen.height		= fbinfo->var.yres;
//		fb_screen.pixel_format	= LX_FBDEV_PIXEL_FORMAT_RGB_8888;

		/* raxis.lim (2012/06/14) -- support non-vsync application :(  */
		b_free_run = ( var->sync == (UINT32)-1 )? TRUE: FALSE;
		//printk("[fb] %s : fb[%d] sync %d, free_run %d\n", __F__, fb_dev_id, var->sync, b_free_run );
		rc = FBDEV_FlipFBImage( fb_dev_id, &fb_screen, b_free_run );
	}

	FBDEV_CHECK_ERROR( RET_OK != rc, /* go */, "[fb%d] error during update_screen\n", fb_dev_id );

#ifdef FBDEV_FLIP_LOCK
	OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif

	return rc;
}

/** check that current setting parameter are correctly or not.
 *
 * @param	struct fb_var_screeninfo *var  , struct fb_info *info
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_check_var(struct fb_var_screeninfo *var, struct fb_info* fbinfo)
{
	const FBDEV_WIN_CTX_T *wctx = (const FBDEV_WIN_CTX_T *)fbinfo->par;
	int	fb_dev_id = wctx->fb_dev_id;
	UINT32	xres, yres, xres_virt, yres_virt;

	FBDEV_CHECK_ERROR( FALSE==wctx->linux_fb_mode, return RET_ERROR, "fb(%d) LinuxFB disabled..\n", fb_dev_id );
	FBDEV_DEBUG("fb(%d)::FBDEV_check_var\n", fb_dev_id );

	switch ( var->bits_per_pixel )
	{
		case 8: case 16: case 32: /* pass */ break;
		default:				  /* not supported */ return -EINVAL;
	}

	/* just check for input size not to be greater than physical max size.
	 * for the safe operation, x coordinate should be aligned by 8 byte ( OSD WORD unit )
	 */
	xres 		= LX_CALC_ALIGNED_VALUE( var->xres, 3 );
	yres 		= var->yres;
	xres_virt 	= LX_CALC_ALIGNED_VALUE( var->xres_virtual, 3 );
	yres_virt 	= var->yres_virtual;

	if ( xres > g_fbdev_cfg->fb[fb_dev_id].max_xres )		xres = g_fbdev_cfg->fb[fb_dev_id].max_xres;
	if ( yres > g_fbdev_cfg->fb[fb_dev_id].max_yres )		yres = g_fbdev_cfg->fb[fb_dev_id].max_yres;
	if ( xres_virt > g_fbdev_cfg->fb[fb_dev_id].max_xres )	xres_virt = g_fbdev_cfg->fb[fb_dev_id].max_xres;

	/* input size should not be greater than virtual screen size */
	if ( xres > xres_virt)	xres_virt = xres;
	if ( yres > yres_virt)	yres_virt = yres;

	var->xres 			= xres;
	var->yres 			= yres;
	var->xres_virtual 	= xres_virt;
	var->yres_virtual 	= yres_virt;

	return RET_OK;
}

/** parameter setting function.
 *
 * @param	struct fb_info *info
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int	FBDEV_set_par(struct fb_info* fbinfo)
{
	UINT32 pxl_fmt, pixel_depth;
	FBDEV_WIN_CTX_T*	wctx 		= (FBDEV_WIN_CTX_T *)fbinfo->par;
	int					fb_dev_id 	= wctx->fb_dev_id;

#ifdef FBDEV_FLIP_LOCK
	OS_LockMutex( &wctx->fb_dev_mtx );
#endif
	FBDEV_DEBUG("fb(%d)::FBDEV_set_par\n", fb_dev_id );

	/* raxis.lim (2010/12/10)
	 * By default, initialize input & output window based on var.xres & var.yres.
	 * [note] FBDEV_SetConfig() fills the real input & output window after calling this function.
	 */
	wctx->pixel.input_win.x = 0;
	wctx->pixel.input_win.y = 0;
	wctx->pixel.input_win.w = fbinfo->var.xres;
	wctx->pixel.input_win.h = fbinfo->var.yres;
	wctx->pixel.output_win.x = 0;
	wctx->pixel.output_win.y = 0;
	wctx->pixel.output_win.w = g_fbdev_cfg->display_width;
	wctx->pixel.output_win.h = g_fbdev_cfg->display_height;

	/* updaet fix info */
	fbinfo->fix.line_length		= (fbinfo->var.xres_virtual * fbinfo->var.bits_per_pixel) / 8;
	fbinfo->fix.smem_len		= PAGE_ALIGN( fbinfo->fix.line_length * fbinfo->var.yres_virtual );

	/* update stride & bits_per_pixel */
	wctx->pixel.stride			= fbinfo->fix.line_length;
	wctx->pixel.bits_per_pixel = fbinfo->var.bits_per_pixel;

#define CHECK_FB_PXLFMT(a,r,g,b)	fbinfo->var.transp.length == (a) && fbinfo->var.red.length == (r) && fbinfo->var.green.length == (g) && fbinfo->var.blue.length == (b)

	/* something.... */
	switch(fbinfo->var.bits_per_pixel)
	{
		case 8:
		{
			pxl_fmt		= LX_FBDEV_PIXEL_FORMAT_RGB_8888;
			pixel_depth	= PIX_DEPTH_8_BPP;
			FBDEV_SetOSDImageEndian( fb_dev_id, 8);
		}
		break;

		case 16:
		{
			if 		( CHECK_FB_PXLFMT(0,5,6,5) ) pxl_fmt = LX_FBDEV_PIXEL_FORMAT_RGB_565;
			if 		( CHECK_FB_PXLFMT(1,5,5,5) ) pxl_fmt = LX_FBDEV_PIXEL_FORMAT_RGB_1555;
			if 		( CHECK_FB_PXLFMT(6,3,4,3) ) pxl_fmt = LX_FBDEV_PIXEL_FORMAT_RGB_6343;
			else 	/* default ARGB4444 */		 pxl_fmt = LX_FBDEV_PIXEL_FORMAT_RGB_4444;

			pixel_depth 	= PIX_DEPTH_16_BPP;
			FBDEV_SetOSDImageEndian( fb_dev_id, 16);
		}
		break;

		case 32:
		default:
			pxl_fmt 	= LX_FBDEV_PIXEL_FORMAT_RGB_8888;
			pixel_depth	= PIX_DEPTH_32_BPP;
			FBDEV_SetOSDImageEndian( fb_dev_id, 32);
		break;
	}

//	wctx->pixel.pixel_fmt 	= pxl_fmt;
//	wctx->pixel.pixel_depth	= pixel_depth;

	/* sync data */
	FBDEV_WriteOSDHeader(&wctx->pixel);

#ifdef FBDEV_FLIP_LOCK
	OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif
	return RET_OK;
}

/** module mmap function.
 *
 * @param	int blank ,struct fb_info *info
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_mmap(struct fb_info *fbinfo,  struct vm_area_struct *vma)
{
	FBDEV_WIN_CTX_T *wctx = (FBDEV_WIN_CTX_T *)fbinfo->par;
	int		 fb_dev_id = wctx->fb_dev_id;
	unsigned long start;
	unsigned long len;
	unsigned long off = (vma->vm_pgoff << PAGE_SHIFT);
	int     ret = -1;

	/* support mmap only when OSD contains internal FB memory */
	FBDEV_CHECK_ERROR( FALSE==wctx->linux_fb_mode, return RET_ERROR, "fb(%d) LinuxFB disabled..\n", fb_dev_id );
	FBDEV_DEBUG("fb(%d)::FBDEV_mmap\n", fb_dev_id );

	if ( g_reg_io_ctx->slkrd_mmap_mask )
	{
		start = ( (fbinfo->fix.smem_start&0x0FFFFFFF)| g_reg_io_ctx->slkrd_mmap_mask );
	}
	else
	{
		start = fbinfo->fix.smem_start;
	}

	len = PAGE_ALIGN((start & ~PAGE_MASK) + g_fbdev_cfg->fb[fb_dev_id].max_size );	/* same result as gMemCfgFbdev.... */

	if ((vma->vm_end - vma->vm_start + off) > len)
	{
		FBDEV_TRACE_END();
		return -EINVAL;
	}

	off += start & PAGE_MASK;
	vma->vm_pgoff = off >> PAGE_SHIFT;
	vma->vm_flags |= VM_IO;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	ret = io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
		vma->vm_end - vma->vm_start,
		vma->vm_page_prot);

	return ret;
}

/**	set color map (palette)
 *
 */
static int FBDEV_setcmap( struct fb_cmap *cmap, struct fb_info *info )
{
	UINT32* palette;
	int i;
	FBDEV_WIN_CTX_T *wctx = (FBDEV_WIN_CTX_T *)info->par;
	FBDEV_DEBUG("fb(%d)::FBDEV_setcmap\n", wctx->fb_dev_id );

	if( cmap->start + cmap->len > 256 ) return RET_ERROR;

	palette = (UINT32*)OS_Malloc(sizeof(UINT32)*cmap->len);
	FBDEV_CHECK_ERROR( NULL == palette, return RET_ERROR, "out of memory\n");


	for( i=0; i<cmap->len; i++)
	{
		palette[i] =((cmap->transp[i]>>8)<<24)| ((cmap->red[i] >>8)<<16)|
					((cmap->green[i] >>8)<< 8)| ((cmap->blue[i]>>8)<< 0);
	}

	/* update palette */
#if 1
	memcpy( (void*)((UINT32*)wctx->osd_plte_base + cmap->start), palette, sizeof(UINT32)*cmap->len );
#else
//	FBDEV_SetPalette( wctx, palette + cmap->start, cmap->len - cmap->start  );
#endif

	OS_Free( palette );

	return RET_OK;
}

/** module blank function.
 *
 * @param	int blank ,struct fb_info *info
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_blank( int blank ,struct fb_info *info)
{
	FBDEV_WIN_CTX_T *wctx = (FBDEV_WIN_CTX_T *)info->par;
	FBDEV_DEBUG("fb(%d)::FBDEV_setcmap\n", wctx->fb_dev_id );

	return RET_OK;
}

static struct fb_ops lg115x_fbdev_fops =
{
	.owner 			= THIS_MODULE,
	.fb_open 		= FBDEV_open,
	.fb_release		= FBDEV_close,
	.fb_check_var	= FBDEV_check_var,
	.fb_set_par		= FBDEV_set_par,
//	.fb_setcolreg	= FBDEV_setcolreg,
	.fb_pan_display	= FBDEV_pan_display,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_rotate		= NULL,
	.fb_sync		= NULL,
	.fb_mmap		= FBDEV_mmap,
	.fb_blank		= FBDEV_blank,
	.fb_ioctl		= FBDEV_ioctl,
	.fb_setcmap		= FBDEV_setcmap,
};

/*========================================================================================
    Implementation Group ( FB(OSD) native interface )
========================================================================================*/

/** Get the image configration ( ioctl interface )
 *
 * @param win [IN] osd plane information
 * @param config [OUT] image configuration
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_GetConfig(FBDEV_WIN_CTX_T* wctx, LX_FBDEV_CONFIG_T* config)
{
	int ret = 0;

	memcpy( &config->input_win,  &wctx->pixel.input_win,  sizeof(LX_RECT_T));
	memcpy( &config->output_win, &wctx->pixel.output_win, sizeof(LX_RECT_T));

	config->fbinfo.xres				= wctx->fbinfo.var.xres;
	config->fbinfo.yres				= wctx->fbinfo.var.yres;
	config->fbinfo.xres_virtual		= wctx->fbinfo.var.xres_virtual;
	config->fbinfo.yres_virtual		= wctx->fbinfo.var.yres_virtual;
	config->fbinfo.bits_per_pixel 	= wctx->fbinfo.var.bits_per_pixel;

	config->global_alpha_enable		= wctx->pixel.global_alpha_en;
	config->blend_alpha_enable		= wctx->pixel.blend_alpha_en;
	config->color_key_enable		= wctx->pixel.color_key_en;
	config->reverse_screen_enable	= wctx->pixel.reserver_screen_en;

	config->global_alpha			= wctx->pixel.global_alpha;
	config->blend_alpha				= wctx->pixel.blend_alpha;
	config->color_key				= wctx->pixel.color_key;
	config->vsync_enable			= wctx->status.b_use_vsync;

	return ret;
}

/** Set the image configration ( ioctl interface )
 *
 * @param	win [IN] osd plane information
 * @param	config [IN image configuration
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_SetConfig(FBDEV_WIN_CTX_T *wctx, LX_FBDEV_CONFIG_T *config)
{
	int ret = 0;

	FBDEV_TRACE_BEGIN();

#ifdef FBDEV_FLIP_LOCK
	OS_LockMutex( &wctx->fb_dev_mtx );
#endif
	FBDEV_NOTI("fb(%d) <begin>\n", wctx->fb_dev_id );

	/* raxis.lim (2011/11/22) -- DO NOT change the FB information. !!! */
#if 0
	wctx->fbinfo.var.xres			= config->fbinfo.xres;
	wctx->fbinfo.var.yres			= config->fbinfo.yres;
	wctx->fbinfo.var.xres_virtual	= config->fbinfo.xres_virtual;
	wctx->fbinfo.var.yres_virtual	= config->fbinfo.yres_virtual;
	wctx->fbinfo.var.bits_per_pixel= config->fbinfo.bits_per_pixel;
#endif

	wctx->status.b_use_vsync 		= config->vsync_enable;

    /* raxis.lim (2012/06/20) -- process reverse mode change */
	if ( wctx->pixel.reserver_screen_en != config->reverse_screen_enable )
	{
        FBDEV_HW_EVENT_DATA_T evdata;

        memset( &evdata, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));
        evdata.reverse_screen_change.fb_dev_id = wctx->fb_dev_id;
        evdata.reverse_screen_change.enable = config->reverse_screen_enable;

        FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_REVERSE_SCREEN_CHANGE, &evdata );

		wctx->pixel.reserver_screen_en = config->reverse_screen_enable;
	}

	/* raxis.lim (2012/01/27) -- update OSD header only value is changed.
	 * without this protection, OSD may flicker when multiple application tries to call FBDEV_WriteOSDHeader
	 */
	if ( wctx->pixel.global_alpha 		!= config->global_alpha 			||
		 wctx->pixel.global_alpha_en	!= config->global_alpha_enable		||
		 wctx->pixel.blend_alpha 		!= config->blend_alpha	   			||
		 wctx->pixel.blend_alpha_en  	!= config->blend_alpha_enable		||
		 wctx->pixel.color_key_en 		!= config->color_key_enable			||
		 wctx->pixel.color_key	  		!= config->color_key				||
		 memcmp( &wctx->pixel.input_win,  &config->input_win,  sizeof(LX_RECT_T)) 	||
         memcmp( &wctx->pixel.output_win, &config->output_win, sizeof(LX_RECT_T)) )
	{
		/* raxis.lim (2012/01/10) -- some platform should support pseudo global alpha level for POSD */
		wctx->pixel.global_alpha		= config->global_alpha;
		wctx->pixel.global_alpha_en		= config->global_alpha_enable;
		wctx->pixel.blend_alpha			= config->blend_alpha;
		wctx->pixel.blend_alpha_en		= config->blend_alpha_enable;
		wctx->pixel.reserver_screen_en 	= config->reverse_screen_enable;

		/* raxis.lim (2013/02/05) -- support color key */
		wctx->pixel.color_key			= config->color_key;
		wctx->pixel.color_key_en		= config->color_key_enable;

		/* if input & output window changed, update osd header again.
		 * [note] wctx->pixel.osd_bm_paddr should be updated after changing input_win.
		 *		  the simplest way to update this value is to call pan_display again.
		 */
		memcpy ( &wctx->pixel.input_win,  &config->input_win,  sizeof(LX_RECT_T));
		memcpy ( &wctx->pixel.output_win, &config->output_win, sizeof(LX_RECT_T));

		FBDEV_WriteOSDHeader(&wctx->pixel);
#if 0
		FBDEV_WaitVSync( wctx->fb_dev_id, wctx->vsync_cnt_on_flip );
#endif
	}

#ifdef FBDEV_FLIP_LOCK
	OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif
	FBDEV_NOTI("fb(%d) <end>\n", wctx->fb_dev_id );
	FBDEV_TRACE_END();

	return ret;
}

/** wait for vsync
 *
 *
 *
 */
static int	FBDEV_WaitVSync	( int fb_dev_id, int vsync_cnt_on_flip )
{
	UINT32	vsync_usec_tmout;
	int		rc = RET_OK;
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx( fb_dev_id );

	FBDEV_CHECK_ERROR( NULL==wctx, return RET_ERROR, "invalid layer %d\n", fb_dev_id );

	/* if application don't want to wait for the vsync, return function !! */
	if ( vsync_cnt_on_flip ==0 ) return RET_OK;

	/* if the default vsync_cnt is requested, I will wait for one vsync.... */
	if ( vsync_cnt_on_flip < 0 ) vsync_cnt_on_flip = g_fbdev_vsync_cnt_on_flip;

	//printk("[fb %d] vsync cnt %d, usec %d :%d:%d\n", fb_dev_id, vsync_cnt_on_flip, g_fbdev_vsync_timeout * vsync_cnt_on_flip, g_fbdev_cfg->b_use_irq, g_fbdev_cfg->fb[fb_dev_id].b_use_vsync );

	/* calculate the current vsync timeout */
	vsync_usec_tmout = g_fbdev_vsync_timeout * vsync_cnt_on_flip;

	/* this function is valid only when vsync_wait is enabled */
	if ( g_fbdev_cfg->b_use_irq && wctx->status.b_use_vsync )
	{
		ULONG flags;

        spin_lock_irqsave(&g_fbdev_vsync_check_spinlock, flags);
		wctx->vsync_cnt = vsync_cnt_on_flip;
		spin_unlock_irqrestore(&g_fbdev_vsync_check_spinlock, flags);

		rc = wait_event_interruptible_timeout (g_fbdev_vsync_wq, wctx->vsync_cnt == 0, usecs_to_jiffies(vsync_usec_tmout) );

		if ( g_fbdev_vsync_debug )
		{
			FBDEV_CHECK_ERROR( rc == 0 && wctx->vsync_cnt, /* nop */, "FB[%d] vsync timeout. HZ=%d, jf=%d, wcnt=%d \n",
				fb_dev_id, HZ, (UINT32)usecs_to_jiffies(vsync_usec_tmout), wctx->vsync_cnt );
			FBDEV_CHECK_ERROR( rc <  0, /* nop */, "FB[%d] vsync abort by signal !!!!\n", fb_dev_id );
		}
	}

	return RET_OK;
}

/*========================================================================================
    Implementation Group (Ioctl Handler)
========================================================================================*/
/** module ioctl.
 *
 * @param	struct fb_info *info,  unsigned int cmd , unsigned long arg
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_ioctl(struct fb_info *info,  unsigned int cmd , unsigned long arg)
{
	int	rc;
	int ret = RET_ERROR;	/* return status of ioctl handler */

	FBDEV_CTX_T*	fbctx = FBDEV_GetCtx();
	FBDEV_WIN_CTX_T* wctx = (FBDEV_WIN_CTX_T *)info->par;
	int	fb_dev_id = wctx->fb_dev_id;

	FBDEV_CHECK_DEBUG( !g_fbdev_cfg->fb[fb_dev_id].b_active, /* nop */, "inactive fb(%d)\n", fb_dev_id );

	switch(cmd)
	{
		case FBDEV_IOR_CHIP_REV_INFO:
		{
			ret = RET_OK;
		}
		break;

		case FBDEV_IOR_GET_CFG:
		{
            rc = copy_to_user((void __user *)arg, (void *)g_fbdev_cfg , sizeof(LX_FBDEV_CFG_T));
            FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

            ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_SET_PALETTE:
		{
            LX_FBDEV_PALETTE_T*	input_pal = (LX_FBDEV_PALETTE_T*)arg;
			LX_FBDEV_PALETTE_T	fbdev_pal;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			fbdev_pal.palette_data = (UINT32*)OS_Malloc(sizeof(UINT32)*256);
			FBDEV_CHECK_ERROR( NULL == fbdev_pal.palette_data, goto func_exit, "pallete alloc error\n");

			rc = __get_user( fbdev_pal.palette_num, &(input_pal->palette_num));
			FBDEV_CHECK_ERROR( rc, OS_Free(fbdev_pal.palette_data); goto func_exit, "copy error\n");

			rc = copy_from_user( (void*)fbdev_pal.palette_data, (void __user*)input_pal->palette_data, sizeof(UINT32)*input_pal->palette_num );
			FBDEV_CHECK_ERROR( rc>0, OS_Free(fbdev_pal.palette_data); goto func_exit, "copy error\n");

			FBDEV_SetPalette( wctx, fbdev_pal.palette_data, fbdev_pal.palette_num );

			OS_Free( fbdev_pal.palette_data );
			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOR_GET_CONFIG:
		{
			LX_FBDEV_CONFIG_T	config;

//			/* support only POSD is enable(active)? */
//			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			FBDEV_GetConfig( wctx, &config );
			rc = copy_to_user( (void __user *)arg,(void *)&config,sizeof(LX_FBDEV_CONFIG_T));
			FBDEV_CHECK_ERROR( rc > 0, , "copy error\n");

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_SET_CONFIG:
		{
			LX_FBDEV_CONFIG_T	config;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&config , (void __user *)arg , sizeof(LX_FBDEV_CONFIG_T));
			FBDEV_CHECK_ERROR( rc > 0, break, "copy error\n");

			FBDEV_SetConfig( wctx, &config );

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOR_GET_POSITION:
		{
			LX_POSITION_T cur_pos;

			cur_pos.x 	= wctx->pixel.output_win.x;
			cur_pos.y 	= wctx->pixel.output_win.y;

			rc = copy_to_user((void __user *)arg , (void *)&cur_pos , sizeof(LX_POSITION_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_SET_POSITION:
		{
			LX_POSITION_T	new_pos;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&new_pos , (void __user *)arg , sizeof(LX_POSITION_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			wctx->pixel.csr_size.w		= 0;	/* use the full size */
			wctx->pixel.csr_size.h		= 0;	/* use the full size */
			wctx->pixel.csr_hotspot.x	= 0;	/* disable hotspot */
			wctx->pixel.csr_hotspot.y	= 0;	/* disable hotspot */

//			if ( new_pos.x < 0 ) new_pos.x = 0;
//			if ( new_pos.y < 0 ) new_pos.y = 0;

			wctx->pixel.output_win.x = new_pos.x;
			wctx->pixel.output_win.y = new_pos.y;

			FBDEV_WriteOSDHeader( &wctx->pixel );

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_SET_CURSOR_POSITION:
		{
			LX_FBDEV_CURSOR_POSITION_T	cursor;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&cursor , (void __user *)arg , sizeof(LX_FBDEV_CURSOR_POSITION_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

//			if ( cursor.pos.x < 0 ) cursor.pos.x = 0;
//			if ( cursor.pos.y < 0 ) cursor.pos.y = 0;

			wctx->pixel.csr_size.w		= cursor.size.w;
			wctx->pixel.csr_size.h		= cursor.size.h;
			wctx->pixel.csr_hotspot.x  = cursor.hotspot.x;
			wctx->pixel.csr_hotspot.y  = cursor.hotspot.y;
			wctx->pixel.output_win.x 	= cursor.pos.x;
			wctx->pixel.output_win.y 	= cursor.pos.y;

			FBDEV_NOTI("sz %d,%d hotspot %d,%d pos %d,%d\n", cursor.size.w, cursor.size.h, cursor.hotspot.x, cursor.hotspot.y, cursor.pos.x, cursor.pos.y );
			FBDEV_WriteOSDHeader( &wctx->pixel );

			/* raxis.lim (2012/01/03) -- when UD mode, all POSD should be merge into the single POSD. so cursor movement should wait for vsync */
			if ( fbctx->global.pivot_mode != LX_FBDEV_PIVOT_MODE_NONE )
			{
				FBDEV_WaitVSync( fb_dev_id, wctx->vsync_cnt_on_flip ); /* wait for vsync !! */
			}

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOR_GET_PHY_MEMORY:
		{
			LX_FBDEV_PHY_MEMORY_T phy_mem;
			UINT32	stride	= wctx->fbinfo.fix.line_length;
			UINT32	len 	= wctx->fbinfo.fix.smem_len;

			phy_mem.memptr = wctx->fb_base_phys;
			phy_mem.stride = stride;
			phy_mem.length = len;

			rc = copy_to_user((void __user *)arg , (void *)&phy_mem , sizeof(LX_FBDEV_PHY_MEMORY_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_SET_VISIBLE:
		{
			UINT32	enable;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&enable , (void __user *)arg , sizeof(UINT32));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			FBDEV_DEBUG("fb(%d)::FBDEV_IOW_SET_VISIBLE. val(%d)\n", fb_dev_id, enable);
			FBDEV_SetOSDEnable(wctx->pixel.fb_dev_id, enable);
#if 0
			/* OSD layer actually will be turned off but after vsync of the time of disabling the lyaer not at that time of disabling it.
			 * In curosr, if thw app try to re-draw the new cursor image after disabling it without waiting of vsync, the display would be cracked
			 */
			if ( lx_chip_plt() == LX_CHIP_PLATFORM_COSMO )
			{
				FBDEV_WaitVSync( fb_dev_id, wctx->vsync_cnt_on_flip ); /* wait for vsync !! */
			}
#endif
			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOR_GET_VISIBLE:
		{
			UINT32	enable;

			enable = (unsigned int)(FBDEV_GetOSDEnable(wctx->pixel.fb_dev_id));

			rc = copy_to_user((void __user *)arg , (void *)&enable , sizeof(UINT32));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOR_GET_FB_STATUS:
		{
			LX_FBDEV_FB_STATUS_T	fb_status;
			memset( &fb_status, 0x0, sizeof(LX_FBDEV_FB_STATUS_T));

			fb_status.b_visible = FBDEV_GetOSDEnable(wctx->pixel.fb_dev_id);
			fb_status.view_fb_phys_addr = wctx->pixel.osd_bm_paddr;

			rc = copy_to_user((void __user *)arg , (void *)&fb_status , sizeof(LX_FBDEV_FB_STATUS_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IO_WAITFORVSYNC:
		{
			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			FBDEV_DEBUG("fb(%d)::FBDEV_IO_WAITFORVSYNC\n", fb_dev_id );

#ifdef FBDEV_FLIP_LOCK
			OS_LockMutex( &wctx->fb_dev_mtx );
#endif
			FBDEV_WaitVSync( fb_dev_id, wctx->vsync_cnt_on_flip ); /* wait for vsync !! */
#ifdef FBDEV_FLIP_LOCK
			OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif
			ret = RET_OK; /* all work done. don't detect VSYNC timeout */
		}
		break;

		/*
		 * -1: donot care the vsync. use configuration setting
		 *  0: donot wait vsync.
		 * >0: wait vsyncs.
		 */
		case FBDEV_IOW_WAITFORVSYNC_ON_FLIP:
		{
			rc = get_user ( wctx->vsync_cnt_on_flip, (int*)arg );
		}
		break;

		case FBDEV_IORW_GET_ZLIST:
		{
			LX_FBDEV_ZLIST_T	zList;

			rc = FBDEV_GetZList( &zList );
			FBDEV_CHECK_ERROR( rc != RET_OK, goto func_exit, "can't get zlist\n");

			rc = copy_to_user((void __user *)arg , (void *)&zList , sizeof(LX_FBDEV_ZLIST_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_ZLIST:
		{
			LX_FBDEV_ZLIST_T	zList;

			rc = copy_from_user((void *)&zList , (void __user *)arg , sizeof(LX_FBDEV_ZLIST_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			rc = FBDEV_SetZList( &zList );
			FBDEV_CHECK_ERROR( rc != RET_OK, goto func_exit, "can't set zlist\n");

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_VIDEO_MIXER_CTRL:
		{
			LX_FBDEV_VIDEO_MIXER_CTRL_T	mix_ctrl;

			rc = copy_from_user((void *)&mix_ctrl , (void __user *)arg , sizeof(LX_FBDEV_VIDEO_MIXER_CTRL_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			rc = FBDEV_SetVideoMixerCtrl(&mix_ctrl);
			FBDEV_CHECK_ERROR( rc != RET_OK, goto func_exit, "can't set video mixer\n");

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_TRID_CTRL:
		{
			LX_FBDEV_TRID_CTRL_T	trid_ctrl;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&trid_ctrl , (void __user *)arg , sizeof(LX_FBDEV_TRID_CTRL_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			rc = FBDEV_SetTridCtrl( fb_dev_id, &trid_ctrl );
			FBDEV_CHECK_ERROR( rc != RET_OK, goto func_exit, "can't init FRC\n");

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_CONN_CTRL:
		{
			LX_FBDEV_CONN_CTRL_T	conn_ctrl;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&conn_ctrl , (void __user *)arg , sizeof(LX_FBDEV_CONN_CTRL_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			rc = FBDEV_SetConnCtrl( &conn_ctrl );
			FBDEV_CHECK_ERROR( rc != RET_OK, goto func_exit, "can't update the connection\n");

			/* when changing connection mode, reset vsync timoeut to be default */
			g_fbdev_vsync_timeout = FBDEV_DEFAULT_VSYNC_USEC_TIMEOUT;

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_FREEZE_CTRL:
		{
			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_PIVOT_CTRL:
		{
            LX_FBDEV_PIVOT_CTRL_T pivot_ctrl;

            rc = copy_from_user((void *)&pivot_ctrl , (void __user *)arg , sizeof(LX_FBDEV_PIVOT_CTRL_T));
            FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			FBDEV_DEBUG("fbdev::FBDEV_IOW_SET_PIVOT_CTRL. 0x%x\n", pivot_ctrl.mode );
			FBDEV_SetPivotCtrl( &pivot_ctrl );

			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_VIEW_CTRL:
		{
			LX_FBDEV_VIEW_CTRL_T	view_ctrl;

			rc = copy_from_user((void *)&view_ctrl , (void __user *)arg , sizeof(LX_FBDEV_VIEW_CTRL_T));
			FBDEV_CHECK_ERROR( rc > 0, goto func_exit, "copy error\n");

			FBDEV_DEBUG("fbdev::FBDEV_IOW_SET_VIEW_CTRL\n");
			FBDEV_SetViewCtrl( &view_ctrl );

			ret = RET_OK; /* all work done */
		}
		break;

		case FBDEV_IOW_FLIP_FB_IMAGE:
		{
			LX_FBDEV_FB_IMAGE_T	fb_screen;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&fb_screen , (void __user *)arg , sizeof(LX_FBDEV_FB_IMAGE_T));
			FBDEV_CHECK_ERROR( rc > 0, break, "copy error\n");

#ifdef FBDEV_FLIP_LOCK
			OS_LockMutex( &wctx->fb_dev_mtx );
#endif
			FBDEV_DEBUG("fb(%d)::FBDEV_IOW_FLIP_FB_IMAGE\n", fb_dev_id );
			FBDEV_FlipFBImage( fb_dev_id, &fb_screen, FALSE );
#ifdef FBDEV_FLIP_LOCK
			OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif
			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_UPDATE_FB_SCREEN:
		{
			LX_FBDEV_FB_SCREEN_T fb_screen;

			/* support only POSD is enable(active)? */
			if ( !g_fbdev_cfg->fb[fb_dev_id].b_active ) { ret = RET_OK; break; }

			rc = copy_from_user((void *)&fb_screen , (void __user *)arg , sizeof(LX_FBDEV_FB_SCREEN_T));
			FBDEV_CHECK_ERROR( rc > 0, break, "copy error\n");

#ifdef FBDEV_FLIP_LOCK
			OS_LockMutex( &wctx->fb_dev_mtx );
#endif
			FBDEV_DEBUG("fb(%d)::FBDEV_IOW_UPDATE_FB_SCREEN\n", fb_dev_id );
			FBDEV_UpdateFBScreen( fb_dev_id, &fb_screen );
#ifdef FBDEV_FLIP_LOCK
			OS_UnlockMutex( &wctx->fb_dev_mtx );
#endif
			ret = RET_OK;	/* all work done */
		}
		break;

		case FBDEV_IOW_SET_VSYNC_EVENT:
		{
#ifdef USE_FBDEV_VSYNC_UEVENT
			UINT32	en;
			rc = copy_from_user((void*)&en, (void __user*)arg, sizeof(UINT32));
			FBDEV_CHECK_ERROR( rc > 0, break, "copy error\n");

			vsync_ev_ctrl.vsync_irq_enabled = (en)? 1:0;
#endif
			ret = RET_OK;	/* all work done */
		}
		break;

		default:
		{
			/* redundant check but it seems more readable */
			FBDEV_ERROR("unknown/not handled ioctl cmd %08X\n", cmd );

			ret = -ENOTTY;
		}
	}

func_exit:
	return ret;
}

/*========================================================================================
    Implementation Group ( open & close )
========================================================================================*/
/** module open.
 *
 * @param	struct fb_info *info , int user
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_open(struct fb_info *info , int user)
{
    FBDEV_WIN_CTX_T* win;
	int	ret = RET_ERROR;
	int	fb_dev_id;

	FBDEV_TRACE_BEGIN();

	win = (FBDEV_WIN_CTX_T*)info->par;
	fb_dev_id = win->fb_dev_id;
	FBDEV_CHECK_ERROR( !win, goto func_exit, "null parameter\n");

	/* by default, user dont care about vsync. use the configuration setting */
	win->vsync_cnt_on_flip	= -1;
	win->vsync_cnt 			= 0;

	/*	raxis.lim (2010/11/20) -- register interrupt handler when IRQ enabled */
	if( win->users++ == 0 )
	{
		if ( g_fbdev_cfg->b_use_irq && !(info->state & (1<<1)) )
		{
			if ( !g_fbdev_irq_run_flag )
			{
				int ret = request_irq( g_reg_io_ctx->irq_num, (irq_handler_t)FBDEV_ISRHandler,0,"osd_irq", NULL);
				FBDEV_CHECK_ERROR( ret, /* nop */, "request_irq failed\n" );
#if defined(CONFIG_SMP) && defined(CONFIG_GENERIC_HARDIRQS)
//				irq_set_affinity( g_reg_io_ctx->irq_num, cpumask_of(1) );
#endif
				g_fbdev_irq_run_flag = 1;

#ifdef FBDEV_USE_MIXER_INTR
				/* register MIXER INTR !!!. instead, remove each OSD INTR !! */
				FBDEV_SetInterruptEnable(MIXER_INTR,1);
#endif
			}
			info->state |= (1<<1);
		}

		if(is_win(info->fix.id, OSD0))
		{
#ifndef FBDEV_USE_MIXER_INTR
			if ( g_fbdev_cfg->fb[0].b_use_vsync )		FBDEV_SetInterruptEnable(OSD_0_INTR,1);
#endif
			if ( g_fbdev_cfg->fb[0].b_show_on_startup )	FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD0,TRUE);
		}
		else if(is_win(info->fix.id , OSD1))
		{
#ifndef FBDEV_USE_MIXER_INTR
			if ( g_fbdev_cfg->fb[1].b_use_vsync )		FBDEV_SetInterruptEnable(OSD_1_INTR,1);
#endif
			if ( g_fbdev_cfg->fb[1].b_show_on_startup )	FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD1,TRUE);
		}
		else if(is_win(info->fix.id , OSD2))
		{
#ifndef FBDEV_USE_MIXER_INTR
			if ( g_fbdev_cfg->fb[2].b_use_vsync )		FBDEV_SetInterruptEnable(OSD_2_INTR,1);
#endif
			if ( g_fbdev_cfg->fb[2].b_show_on_startup )	FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD2,TRUE);
		}
		else if(is_win(info->fix.id , OSD_CURSOR))
		{
#ifndef FBDEV_USE_MIXER_INTR
			if ( g_fbdev_cfg->fb[3].b_use_vsync )		FBDEV_SetInterruptEnable(CRSR_INTR,1);
#endif
			if ( g_fbdev_cfg->fb[3].b_show_on_startup )	FBDEV_SetOSDEnable(LX_FBDEV_ID_CSR0,TRUE);
		}
		else
		{
			/* do nothing */
		}
	}

	/* initialize vsync uevent */
#ifdef USE_FBDEV_VSYNC_UEVENT
	if ( g_fbdev_vsync_ev_init == 0 )
	{
		vsync_ev_ctrl.dev = info->dev;
		ret = sysfs_create_group(&vsync_ev_ctrl.dev->kobj, &vsync_fs_attr_group);
		FBDEV_CHECK_ERROR(ret!=RET_OK, /* nop */, "sysfs group creatioin failed. ret=%d\n", ret );

		g_fbdev_vsync_ev_init = 1;

		kobject_uevent(&vsync_ev_ctrl.dev->kobj, KOBJ_ADD);
		spin_lock_init(&vsync_ev_ctrl.spinlock);
		init_completion(&vsync_ev_ctrl.vsync_completion);

		vsync_ev_ctrl.vsync_irq_enabled = 0; /* disable at start. TEST: enabled when vsync_show_event() is called */
	}
#endif

	/* generate POSD OPEN event */
	{
        FBDEV_HW_EVENT_DATA_T evdata;

        memset( &evdata, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));
        evdata.osd.fb_dev_id = fb_dev_id;

        FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_POSD_OPEN, &evdata );
	}

	ret = RET_OK;	/* all work done */
func_exit:
	FBDEV_TRACE_END();
	return ret;
}

/**
 *
 * module close.
 *
 * @param	struct fb_info *info , int user
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int FBDEV_close(struct fb_info *info , int user)
{
	int	ret = RET_ERROR;
    FBDEV_WIN_CTX_T *win;

	FBDEV_TRACE_BEGIN();

	win = (FBDEV_WIN_CTX_T*)info->par;
	FBDEV_CHECK_ERROR( !win, goto func_exit, "null parameter\n");

	/* raxis.lim (2011/03/29)
	 * DO NOT clear interrupt when FB closed.
	 * the below code seems to stop all interrupts of FB when a single FB device is closed.
	 */
#if 0
	if( --win->users == 0 && (info->state & (1<<1)) )
	{
		/* raxis.lim (2010/11/26) -- OSD irq is never freed */
		// free_irq(IRQ_OSD,NULL);
		info->state &= ~(1<<1);	// info state : 0bit(running, suspend) 1bit(irq enable, disable)
	}

	if(is_win(info->fix.id, OSD0))
	{
		FBDEV_SetInterruptEnable(OSD_0_INTR,FALSE);
		FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD0,FALSE);
	}
	else if(is_win(info->fix.id, OSD1))
	{
		FBDEV_SetInterruptEnable(OSD_1_INTR,FALSE);
		FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD1,FALSE);
	}
	else if(is_win(info->fix.id, OSD2))
	{
		FBDEV_SetInterruptEnable(OSD_2_INTR,FALSE);
		FBDEV_SetOSDEnable(LX_FBDEV_ID_OSD2,FALSE);
	}
	else if(is_win(info->fix.id, OSD_CURSOR))
	{
		FBDEV_SetInterruptEnable(CRSR_INTR,FALSE);
		FBDEV_SetOSDEnable(LX_FBDEV_ID_CSR0,FALSE);
	}
	else
	{

	}
#else
	--win->users;
#endif

	/* generate FB event */
	{
        FBDEV_HW_EVENT_DATA_T evdata;

        memset( &evdata, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));
        evdata.osd.fb_dev_id = win->fb_dev_id;

        FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_POSD_CLOSE, &evdata );
	}

func_exit:
	FBDEV_TRACE_END();

	return ret;
}

/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
 int /*__init*/ FBDEV_probe(struct platform_device *pdev)
{
	int	i;
	char* fbname_list[FBDEV_MAX_DEVICE] = { OSD0_FBNAME, OSD1_FBNAME, OSD2_FBNAME, OSD3_FBNAME };

	FBDEV_INFO("fbdev probe start\n");
	g_fbdev_drv_data = (FBDEV_DRVDATA_T *)kmalloc(sizeof(FBDEV_DRVDATA_T) , GFP_KERNEL);
	g_fbdev_drv_data->b_suspend = 0;

	memset( g_dm_ctx, 0x0, sizeof(FBDEV_CTX_T));

	g_dm_ctx->dev = &pdev->dev;

	/* allocate memory region for register */
	g_dm_ctx->mmio_base_phys	= g_reg_io_ctx->reg_base_addr;
	g_dm_ctx->mmio_size 		= g_reg_io_ctx->reg_size;

	g_dm_ctx->global.b_freeze	= FALSE;
	g_dm_ctx->global.pivot_mode	= LX_FBDEV_PIVOT_MODE_NONE;

	FBDEV_CHECK_ERROR( FBDEV_InitHardware() < 0, goto func_exit, "HW init fail\n");

	/* initialize osd layer & fb structure */
	for (i=0; i<FBDEV_MAX_DEVICE; i++)
	{
		g_dm_ctx->wctx[i] = FBDEV_InitWinCtx( i, fbname_list[i] );
		FBDEV_NOTI("fb(%d) probe ok\n", i );
		FBDEV_InitLinuxFB( g_dm_ctx->wctx[i], FBDEV_GetVarScreenObject(i), fbname_list[i] );
	}

func_exit:
	//FBDEV_INFO("fbdev proble <end>\n");
	return RET_OK;
}

#ifdef FBDEV_USE_FAKE_FBMEM
static ULONG FBDEV_GetGFXFBBuf ( int fb_dev_id )
{
	ULONG fb_base_phys;

	if ( fb_dev_id == 1 )
	{
		fb_base_phys = gMemCfgGfx.surface[0].base + gMemCfgGfx.surface[0].size
						- LX_CALC_ALIGNED_VALUE(g_fbdev_cfg->fb[1].max_size,12);
	}
	else if ( fb_dev_id == 0 )
	{
		fb_base_phys = gMemCfgGfx.surface[0].base + gMemCfgGfx.surface[0].size
						- LX_CALC_ALIGNED_VALUE(g_fbdev_cfg->fb[0].max_size,12)
						- LX_CALC_ALIGNED_VALUE(g_fbdev_cfg->fb[1].max_size,12);
	}
	else
	{
		fb_base_phys = 0;
	}

	return fb_base_phys;
}
#endif

/**	initialize OSD hw contex
 *
 */
static FBDEV_WIN_CTX_T*	FBDEV_InitWinCtx( int fb_dev_id, char* fbname )
{
	FBDEV_WIN_CTX_T *wctx;

	/* allocate window context and initialize it */
	wctx = OS_Malloc(sizeof(FBDEV_WIN_CTX_T));
	FBDEV_CHECK_ERROR( !wctx, return NULL, "out of memory. can't alloc wctx(%s)\n", fbname );

	memset(wctx, 0x0, sizeof(FBDEV_WIN_CTX_T));

	/* create per-device mutex */
	OS_InitMutex( &wctx->fb_dev_mtx, OS_SEM_ATTR_DEFAULT);

	wctx->dm					= g_dm_ctx;
	wctx->fb_dev_id				= fb_dev_id;
	wctx->status.b_use_vsync	= g_fbdev_cfg->fb[fb_dev_id].b_use_vsync;
	wctx->status.b_osd_hdr_write = TRUE;		/* osd write is enabled by default */

#if (CONFIG_LX_BOOT_LOGO==1)
	wctx->status.b_osd_hdr_flush = FALSE;		/* keep osd header to keep boot logo shown */
#else
	wctx->status.b_osd_hdr_flush = TRUE;		/* osd header will be upated by both kdrv or app */
#endif

	if ( g_fbdev_cfg->fb[fb_dev_id].b_use_builtin_fbmem )
	{
		wctx->linux_fb_mode	= TRUE;
		wctx->fb_base_phys 	= gMemCfgFBDev[fb_dev_id].mem_frame_base;
	}
	else
	{
		wctx->linux_fb_mode	= FALSE;
#ifdef FBDEV_USE_FAKE_FBMEM
		wctx->fb_base_phys	= FBDEV_GetGFXFBBuf(fb_dev_id);
		if(wctx->fb_base_phys == 0) wctx->fb_base_phys  = gMemCfgFBDev[fb_dev_id].mem_frame_base;
#else
		wctx->fb_base_phys 	= gMemCfgFBDev[fb_dev_id].mem_frame_base;
#endif
	}
	wctx->fb_size			= g_fbdev_cfg->fb[fb_dev_id].max_size;
	wctx->fb_base			= (ULONG)ioremap(wctx->fb_base_phys, wctx->fb_size);

	wctx->osd_plte_base_phys = gMemCfgFBDev[fb_dev_id].mem_palette_base;
	wctx->osd_plte_size 	= gMemCfgFBDev[fb_dev_id].mem_palette_size ;

	wctx->osd_hdr_base_phys	= gMemCfgFBHdr[fb_dev_id].mem_header_base;
	wctx->osd_hdr_size 		= gMemCfgFBHdr[fb_dev_id].mem_header_size;

	wctx->osd_hdr_base = (ULONG)ioremap(wctx->osd_hdr_base_phys, wctx->osd_hdr_size);
	FBDEV_CHECK_ERROR( !wctx->osd_hdr_base, goto err_exit, "%s: can't ioremap osd hdr\n", fbname );

	wctx->osd_plte_base = (ULONG)ioremap(wctx->osd_plte_base_phys, wctx->osd_plte_size * sizeof(UINT32) );
	FBDEV_CHECK_ERROR( !wctx->osd_hdr_base, goto err_exit, "%s: can't ioremap osd pal\n", fbname );

	/* We reserve one page for the palette, plus the size of the framebuffer. */
	// wctx->map_size	= PAGE_ALIGN(wctx->fbinfo.fix.smem_len);

	/* initialize osd header */
	FBDEV_InitCanvas(wctx);

	return wctx;
err_exit:
	if (wctx) FBDEV_CleanupLayer( wctx );

	return NULL;
}

/** initialize 'struct fb_info' of fb device based on the default param
 *	this function initializes 'struct var_screen_info' and 'struct fix_screen info'.
 *
 */
static void FBDEV_InitLinuxFB(FBDEV_WIN_CTX_T* wctx, struct fb_var_screeninfo* default_vinfo, char *id )
{
	int	fb_dev_id = wctx->fb_dev_id;
	struct fb_info *fbinfo = &(wctx->fbinfo);

	FBDEV_DEBUG("fb(%d) init FB variable\n", fb_dev_id );

	/* initialize the fb_info structure */
	fbinfo->flags			= FBINFO_DEFAULT;
	fbinfo->fbops			= &lg115x_fbdev_fops;
	fbinfo->pseudo_palette	= wctx->pseudo_palette;
	fbinfo->screen_base		= (char*)(wctx->fb_base);
	fbinfo->screen_size		= wctx->fb_size;
	fbinfo->par				= (void*)wctx;

	/* copy default values */
	memcpy (&fbinfo->var, default_vinfo, sizeof(struct fb_var_screeninfo));

	/* Initialize fixed screeninfo.
	 * The fixed screeninfo cannot be directly specified by the user, but
	 * it may change to reflect changes to the var info.
	 */
	strncpy(fbinfo->fix.id, id, sizeof(fbinfo->fix.id)-1);

	fbinfo->fix.line_length		= (fbinfo->var.xres_virtual * fbinfo->var.bits_per_pixel) / 8;
	fbinfo->fix.smem_start		= wctx->fb_base_phys;
	fbinfo->fix.smem_len		= wctx->fb_size;
	fbinfo->fix.type			= FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.visual			= (fbinfo->var.bits_per_pixel <= 8) ? FB_VISUAL_PSEUDOCOLOR : FB_VISUAL_TRUECOLOR;
	fbinfo->fix.xpanstep		= 0;
	fbinfo->fix.ypanstep		= 1;		 // paning enable
	fbinfo->fix.ywrapstep		= 0;
	fbinfo->fix.type_aux		= 0;
	fbinfo->fix.mmio_start		= wctx->dm->mmio_base_phys;
	fbinfo->fix.mmio_len		= wctx->dm->mmio_size;
	fbinfo->fix.accel			= FB_ACCEL_NONE;

	/* register posd framebuffer */
	FBDEV_CHECK_ERROR( register_framebuffer(&wctx->fbinfo)<0, /* nop */, "fb(%d) fbbuf reg failed\n", fb_dev_id );

	/* make initial param */
	FBDEV_set_par( &wctx->fbinfo );
}

/** start LinuxFB test mode.
 *	this function is called from fbdev_proc. main role is to make pseudo LinuxFB test envrionment when FBdev has no
 *	internal frame buffer.
 *
 */
int FBDEV_InitLinuxFBTestMode ( int fb_dev_id )
{
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx( fb_dev_id );
	FBDEV_CHECK_ERROR( NULL==wctx, return RET_ERROR, "invalid winctx. fb %d\n", fb_dev_id );
	FBDEV_CHECK_ERROR( TRUE==wctx->linux_fb_mode, return RET_OK, "fb(%d) LinuxFB already enabled..\n", fb_dev_id );

	wctx->linux_fb_mode	= TRUE;

	return RET_OK;
}

/*========================================================================================
    Implementation Group (IRQ)
========================================================================================*/

#ifdef USE_FBDEV_VSYNC_UEVENT
/** fbdev vsync uevent function */
static ssize_t vsync_show_event(struct device* dev, struct device_attribute* attr, char* buf)
{
	int	ret;
	ULONG flags;

	vsync_ev_ctrl.vsync_irq_enabled = 1;	/* TEST: force enable vysnc event notifier !! */

	do
	{
		spin_lock_irqsave(&vsync_ev_ctrl.spinlock, flags);

		if (vsync_ev_ctrl.wait_vsync_cnt == 0 )
		{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
			reinit_completion(&vsync_ev_ctrl.vsync_completion);
#else
			INIT_COMPLETION(vsync_ev_ctrl.vsync_completion);
#endif
		}
		vsync_ev_ctrl.wait_vsync_cnt++;
		spin_unlock_irqrestore(&vsync_ev_ctrl.spinlock, flags);

		ret = wait_for_completion_interruptible(&vsync_ev_ctrl.vsync_completion);
		FBDEV_CHECK_ERROR( ret != RET_OK, /* nop */, "event wait error. ret=%d\n", ret );
	}
	while(ret!=RET_OK);

	ret = snprintf(buf,PAGE_SIZE,"VSYNC=%llu", ktime_to_ns(vsync_ev_ctrl.vsync_time));
	buf[strlen(buf)+1] = '\0';

	return ret;
}
#endif

/** fbdev sync function.
 *
 * @param	void
 * @return	int
 *
 */
int FBDEV_SyncWithVsync(void)
{
	if ( g_fbdev_cfg->b_use_irq )
	{
		int i;
		ULONG flags;
		FBDEV_WIN_CTX_T* wctx;

		spin_lock_irqsave(&g_fbdev_vsync_check_spinlock, flags );

		for (i=0; i<FBDEV_MAX_DEVICE; i++)
		{
			wctx = FBDEV_GetWinCtx(i);
			if ( wctx && wctx->vsync_cnt > 0)
			{
				wctx->vsync_cnt-- ;
			}
		}
		spin_unlock_irqrestore(&g_fbdev_vsync_check_spinlock, flags);

#ifdef USE_FBDEV_VSYNC_UEVENT
		if(vsync_ev_ctrl.vsync_irq_enabled)
		{
			ULONG flags;

			vsync_ev_ctrl.vsync_time = ktime_get();

			spin_lock_irqsave(&vsync_ev_ctrl.spinlock, flags);
			if (vsync_ev_ctrl.wait_vsync_cnt > 0 )
			{
				complete_all(&vsync_ev_ctrl.vsync_completion);
				vsync_ev_ctrl.wait_vsync_cnt = 0;
			}
			spin_unlock_irqrestore(&vsync_ev_ctrl.spinlock, flags);
		}
#endif
		wake_up_interruptible_all(&g_fbdev_vsync_wq);
	}

	return 0;
}

/** fbdev interrupt handler.
 *
 * @param	void
 * @return	int
 *
 */
static irqreturn_t FBDEV_ISRHandler(int irq, void *dev_id, struct pt_regs *regs)
{
	UINT32	intr_status;
#ifdef USE_FBDEV_ISR_TIME_CHK
	ktime_t	t;
	UINT64	ns[2];
	t = ktime_get();
	ns[0] = ktime_to_ns(t);
#endif
	/* read current ISR status */
	intr_status = FBDEV_GetInterruptStatus(ALL_INTR);
	//FBDEV_INFO("osd ISR - status = 0x%x\n", intr_status );

	if ( g_fbdev_use_isr_mon )
	{
		static	int		b_first_vsync = 1;
		static	UINT64	start_tick;

		if ( b_first_vsync )
		{
			b_first_vsync = 0; start_tick = OS_GetMsecTicks();
		}
		else
		{
			if ( ++g_fbdev_vsync_cnt & 0xff00 ) /* vsync count >= 256 */
			{
				UINT64	curr_tick = OS_GetMsecTicks();
				int		diff_tick = (int)(curr_tick-start_tick);

				/* if DVB mode (50Hz) , ideal diff_tick will be 20.000 ms * 256 = 5120
				 * if DVB mode (48Hz) , ideal diff_tick will be 20.833 ms * 256 = 5333
				 * If UD  mode (30Hz) , local diff_tick will be 33.333 ms * 256 = 8532 ?
				 * else 60Hz
				 */
				if ( g_fbdev_use_vsync_auto_tune )
				{
					g_fbdev_vsync_timeout = (diff_tick>=8000)? 33333 : (diff_tick>=5300)? 20832: (diff_tick>=5000)? 19999: (diff_tick>=2200)? 16666: 8333;
					g_fbdev_vsync_timeout += jiffies_to_usecs(1);
				}

				printk("[osd] ISR - HZ=%d, max_vsync_timeout %d us (%d jiffies), diff_tm %d ms/256, avg_vsync %d.%d ms\n",
						HZ, g_fbdev_vsync_timeout, (UINT32)usecs_to_jiffies(g_fbdev_vsync_timeout), diff_tick,
						diff_tick/256, (diff_tick%256)*100/256 );

				start_tick = curr_tick;
				g_fbdev_vsync_cnt = 0;
			}
		}
	}
	else
	{
		if ( ++g_fbdev_vsync_cnt & 0xff00 ) g_fbdev_vsync_cnt = 0; /* vsync count >= 256 */
	}

	/* ignore handler when there is not any OSD interrupt */
	if( intr_status == 0 ) goto func_exit;

	/* raxis.lim (2010/11/20) -- ignore interrupt service when FB doesn't use IRQ */
	if ( !g_fbdev_cfg->b_use_irq )
	{
		FBDEV_SetInterruptClear( ALL_INTR ); goto func_exit;
	}

#ifndef FBDEV_USE_MIXER_INTR
	if( intr_status & (1<<OSD_0_INTR) )
	{
		FBDEV_SetInterruptClear( OSD_0_INTR );
	}

	if( intr_status & (1<<OSD_1_INTR) )
	{
		FBDEV_SetInterruptClear(OSD_1_INTR );
	}

	if( intr_status & (1<<OSD_2_INTR) )
	{
		FBDEV_SetInterruptClear(OSD_2_INTR );
	}

	if( intr_status & (1<<OSD_3_INTR) )
	{
		FBDEV_SetInterruptClear( CRSR_INTR );
	}
	if( intr_status & (1<<MIXER_INTR_EXT) )		/* ??? */
	{
		FBDEV_SetInterruptClear( MIXER_INTR_EXT );
	}

#endif

	FBDEV_MON_ISRTick();

	/* TODO (raxis.lim 2014/05/14) code fix for H15 ISR handler !! */
	if (lx_chip_rev() >= LX_CHIP_REV(H15,A0) )
	{
		FBDEV_SetInterruptClear( ALL_INTR );
		FBDEV_ISRTickHandler(g_fbdev_vsync_cnt);/* call chip dependent ISR */
		FBDEV_SyncWithVsync();					/* call ISR callback */
	}
	else
	{
		if( intr_status & (1<<MIXER_INTR) )
		{
			FBDEV_SetInterruptClear( MIXER_INTR );
			FBDEV_ISRTickHandler(g_fbdev_vsync_cnt);/* call chip dependent ISR */
			FBDEV_SyncWithVsync();					/* call ISR callback */
		}
	}

#ifdef USE_FBDEV_ISR_TIME_CHK
	t = ktime_get();
	ns[1] = ktime_to_ns(t);
	printk("fb intr tick = %d ns\n", (int)(ns[1]-ns[0]) );
#endif

func_exit:
	return IRQ_HANDLED;
}

/*========================================================================================
    Implementation Group
========================================================================================*/

/**
 *
 * module remove function. this function will be called in insmod fbdev module.
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int  FBDEV_remove(struct platform_device *pdev)
{
	FBDEV_DEBUG("released\n");

	return 0;
}

/**
 *
 * module remove function. this function will be called in insmod fbdev module.
 *
 * @param	struct device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static void  FBDEV_release(struct device *dev)
{
	FBDEV_DEBUG("released\n");
}

/*
 *	L8 fbdev platform device structure
 */
static struct platform_device fbdev_plt_device =
{
	.name = MODULE_NAME,
	.id = 0,
	.id = -1,
	.dev =
	{
		.coherent_dma_mask	= 0xffffffff,
		.release 			= FBDEV_release,
	},
};

static struct dev_pm_ops fbdev_pm_ops =
{
	.freeze		= FBDEV_freeze ,		// SNAPSHOT suspend
	.thaw		= FBDEV_thaw,			// SNAPSHOT resume right after making
	.restore	= FBDEV_restore,		// SNAPSHOT boot

	.suspend	= FBDEV_suspend,		// IBOOT suspend
	.resume		= FBDEV_resume ,		// IBOOT boot(resume)
};

static struct platform_driver fbdev_plt_driver =
{
	.probe		= FBDEV_probe,
	.remove		= FBDEV_remove,
	.driver		=
	{
		.name   = MODULE_NAME,
		.pm 	= &fbdev_pm_ops,
	},
};

/**
 *
 *
 *
 * @param
 * @return	int 0 : OK , -1 : NOT OK
 *
 */

#ifndef MODULE
/*
 * Only necessary if your driver takes special options,
 * otherwise we fall back on the generic fb_setup().
 */
int __init FBDEV_setup(char *options)
{
	/* Parse user speficied options (`video=l8fb:') */
	return RET_OK;
}
#endif

/*========================================================================================
    Implementation Group
========================================================================================*/
/* initialize all variable of FB device */
static void FBDEV_InitVar ( void )
{
	memset(&g_fbdev_frc_ctrl, 0x0, sizeof(FBDEV_FRC_CTRL_T));	/* clear FRC */
	memset(&g_fbdev_hw_ctx, 0x0, sizeof(FBDEV_HW_CTX_T));		/* clear HW Ctx */
	memset(g_dm_ctx, 0x0, sizeof(FBDEV_CTX_T));					/* celar dm_ctx */
}

/** initialize basic configuration for FB device.
 *	[note] this function is called before the real device intialization
 *
 */
void	FBDEV_PreInit	( void )
{
	FBDEV_InitVar	( );	/* initialize variable */
	FBDEV_InitHWCtx	( );	/* initialize HW context */
	FBDEV_InitCfg	( );	/* initialize configuration */
}

extern void DE_IO_AddListener(void(*fn)(unsigned long));

/** TODO: re-implement it */
void FBDEV_EventListener(unsigned long evnt_type)
{
	FBDEV_HW_EVENT_DATA_T evdata;

	memset( &evdata, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));

	switch(evnt_type)
	{
		case 0x0: /* DE firmware will be loaded */
		{
			FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_FREEZE_OFF, &evdata );
		}
		break;

		default: break;
	}
}

/** module initial function. this function will be called in insmod fbdev module.
 *
 * @param	void
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int  FBDEV_Init(void)
{
#if 1
	/* raxis.lim (2012/05/08)
	 * I decided to support the dynamic alloacation for FBDEV framebuffer memory.
	 * so gpFBdevRegCfg->frame_base_addr can't be determined before device memory allocation.
	 *
	 * [TODO] REWRITE !!
	 */
	if ( g_reg_io_ctx->frame_base_addr == 0x0 )
	{
		g_reg_io_ctx->frame_base_addr = gMemCfgFBDev[0].mem_frame_base;
	}

	/* application needs FB memory information
	 *
	 * [TODO] REWRITE !!
	 */
	g_fbdev_cfg->fb_mem_base = g_reg_io_ctx->frame_base_addr;
	g_fbdev_cfg->fb_mem_size = g_reg_io_ctx->frame_base_size;
#endif

	/* open debug(logm) handler */
	g_fbdev_debug_fd = DBG_OPEN( FBDEV_MODULE );
	if(g_fbdev_debug_fd < 0) return RET_ERROR;

	OS_DEBUG_EnableModuleByIndex( g_fbdev_debug_fd, LX_LOGM_LEVEL_ERROR,   DBG_COLOR_RED );
	OS_DEBUG_EnableModuleByIndex( g_fbdev_debug_fd, LX_LOGM_LEVEL_WARNING, DBG_COLOR_YELLOW );
	//OS_DEBUG_EnableModuleByIndex( g_fbdev_debug_fd, LX_LOGM_LEVEL_INFO, DBG_COLOR_NONE );
	//OS_DEBUG_EnableModuleByIndex( g_fbdev_debug_fd, 10, DBG_COLOR_NONE );
	//OS_DEBUG_EnableModuleByIndex( g_fbdev_debug_fd, 11, DBG_COLOR_NONE );

#ifndef MODULE
	{
		char *option = NULL;

		/* name register */
		FBDEV_CHECK_ERROR( RET_OK != fb_get_options("lg115x_fb", &option), return -ENODEV, "get fb_get_option fail\n");
		FBDEV_setup(option);
	}
#endif
	FBDEV_CHECK_ERROR( platform_driver_register(&fbdev_plt_driver) < 0, return RET_ERROR, "platform drv reg fail\n");
	FBDEV_CHECK_ERROR( platform_device_register(&fbdev_plt_device) != RET_OK, return RET_ERROR, "platform dev reg fail\n");

	/* add more initialization */
	FBDEV_PROC_Init();
	FBDEV_MON_Init();

	DE_IO_AddListener(FBDEV_EventListener);

	return RET_OK;
}

/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param	void
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
void  FBDEV_Cleanup(void)
{
	struct fb_info *info;

	FBDEV_PROC_Cleanup();

	// register memory release
	FBDEV_ExitHardware();

	info = &g_dm_ctx->wctx[0]->fbinfo;
	unregister_framebuffer(info);
	info = &g_dm_ctx->wctx[1]->fbinfo;
	unregister_framebuffer(info);
	info = &g_dm_ctx->wctx[2]->fbinfo;
	unregister_framebuffer(info);
	info = &g_dm_ctx->wctx[3]->fbinfo;
	unregister_framebuffer(info);

	// header&palette  memory release
	FBDEV_CleanupLayer(g_dm_ctx->wctx[0]);
	FBDEV_CleanupLayer(g_dm_ctx->wctx[1]);
	FBDEV_CleanupLayer(g_dm_ctx->wctx[2]);
	FBDEV_CleanupLayer(g_dm_ctx->wctx[3]);

	platform_driver_unregister(&fbdev_plt_driver);
	platform_device_unregister(&fbdev_plt_device);

	FBDEV_DEBUG("INFO(FBDEV_Exit)-  removing module\n");
}

/*========================================================================================
    Implementation Group (suspend/resume)
========================================================================================*/
static char* FBDEV_GetSnapshotBootMode(void)
{
	int		  ret = RET_ERROR;
	OS_FILE_T file;

#define SNAPSHOOT_BOOT_MODE	"/sys/power/mode"
	static char	buf[32];

	if( RET_OK != OS_OpenFile( &file, SNAPSHOOT_BOOT_MODE, O_RDONLY|O_LARGEFILE, 0666 ) )
	{
		printk("<error> can't open %s\n", SNAPSHOOT_BOOT_MODE); goto func_exit;
	}

	if ( 0 >= OS_ReadFile( &file, buf, 31 ) )
	{
		OS_CloseFile(&file);
		printk("<error> can't read %s", SNAPSHOOT_BOOT_MODE); goto func_exit;
   	}
	OS_CloseFile(&file);

	printk("++ snapshot mode = (%s)\n", buf );

	ret = RET_OK;
func_exit:
	return (ret==RET_OK)? buf: NULL;
}

#ifdef FBDEV_USE_FBMEM_BKUP

#define MAX_BK_SURF		400	/* FIXME: 400 is enough */

typedef struct
{
#define FBDEV_FBMEM_BKUP_IMG_NONE	0x00
#define FBDEV_FBMEM_BKUP_IMG_POSD	0x01
	UINT32	attrib;
	UINT32	phys_addr;
	UINT32	length;
	void*	virt_img;
}
FBDEV_FBMEM_BKUP_IMG_T;

static UINT32					fbmem_bkup_num  = 0;
static FBDEV_FBMEM_BKUP_IMG_T*	fbmem_bkup_list = NULL;

/** save fbmem to kernel memory and reutrn it */
static void* FBDEV_SaveFBImage( UINT32 fbmem_addr, UINT32 length )
{
	OS_CACHE_MAP_T cmap;
	void* virt_img = NULL;

    virt_img = vmalloc(length);
    FBDEV_CHECK_ERROR(virt_img==NULL, return NULL, "can't vmalloc %d\n", length);

    OS_OpenCacheMap(&cmap, (ULONG)fbmem_addr, length );
    OS_InvalCacheMap(&cmap, 0x0, length);

    memcpy((void*)virt_img, (void*)cmap.virt_addr, length);
    OS_CloseCacheMap(&cmap );

    FBDEV_DEBUG("fbmem backup ok. paddr 0x%x, virt_img 0x%p, length %d\n", fbmem_addr, virt_img, length );
    return virt_img;
}

static void* FBDEV_ClearFBImage( UINT32 fbmem_addr, UINT32 length )
{
	OS_CACHE_MAP_T cmap;
	void* virt_img = NULL;

	virt_img = vmalloc(length);
	FBDEV_CHECK_ERROR(virt_img==NULL, return NULL, "can't vmalloc %d\n", length);

	memset((void*)virt_img, 0x0, length);

	FBDEV_DEBUG("fbmem clear ok. paddr 0x%x, virt_img 0x%p, length %d\n", fbmem_addr, virt_img, length );
	return virt_img;
}

/** restore fbmem with kernel memory */
static void	FBDEV_RestoreFBImage( UINT32* virt_img, UINT32 fbmem_addr, UINT32 length, BOOLEAN b_restore_fb )
{
	OS_CACHE_MAP_T cmap;

	FBDEV_CHECK_ERROR( virt_img==NULL,	return, "virt_img NULL\n");
	FBDEV_CHECK_ERROR( fbmem_addr==0x0,	return, "phys_addr NULL\n");
    FBDEV_CHECK_ERROR( length==0,		return, "length zero\n");

	if (b_restore_fb)
	{
		OS_OpenCacheMap(&cmap, (ULONG)fbmem_addr, length);
		memcpy((void*)cmap.virt_addr, (void*)virt_img, length);

		OS_CleanCacheMap(&cmap, 0x0, length);
		OS_CloseCacheMap(&cmap);

		FBDEV_DEBUG("fbmem resotre ok. paddr 0x%x, virt_img 0x%p, length %d\n", fbmem_addr, virt_img, length );
	}

	vfree(virt_img);
}

void	FBDEV_BackupFBList(void)
{
	int		i;
	UINT32	fb_len;
	UINT32* virt_img;
	LX_GFX_SURFACE_SETTING_T surf_info;

	int		bk_img_idx = 0;
	int		fail_cnt = 0;
	int		num_surf = 0;
	int* 	surf_fd_list = NULL;

	/* if backup is already done, abort request. this can't be happended in the real app */
	FBDEV_CHECK_ERROR(fbmem_bkup_list, return, "fbmem_bkup_list is NOT NULL\n");

	GFX_LockDevice();

	surf_fd_list = vmalloc(MAX_BK_SURF*sizeof(int));
	FBDEV_CHECK_ERROR(NULL==surf_fd_list, goto func_exit, "can't alloc surf_fd_list. sz=%d\n", MAX_BK_SURF*sizeof(int));

	GFX_GetSurfaceFdList( surf_fd_list, MAX_BK_SURF, &num_surf );
	FBDEV_CHECK_ERROR(0==num_surf, goto func_exit, "num_surf zero\n");
	FBDEV_DEBUG("max actvie surf num = %d\n", num_surf );

	/* alloc fbmem holder */
	fbmem_bkup_list = vmalloc(num_surf*sizeof(FBDEV_FBMEM_BKUP_IMG_T));
	FBDEV_CHECK_ERROR(NULL==fbmem_bkup_list, goto func_exit, "can't alloc fbmem_bkup_list.sz=%d\n", num_surf*sizeof(FBDEV_FBMEM_BKUP_IMG_T));

#ifdef FBDEV_SUPPORT_OSD_BKUP
	/* save graphic frame buffer */
	for (i=0; i<2; i++ )
	{
		FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(i);
		FBDEV_CHECK_DEBUG( FALSE==FBDEV_GetOSDEnable(i), continue, "fb(%d) off, skip save framebuffer\n", i );

		fb_len = wctx->pixel.stride * wctx->pixel.input_win.h;
#ifdef FBDEV_SUPPORT_OSD_CLEAR_ON_BKUP
		virt_img = FBDEV_ClearFBImage((UINT32)wctx->pixel.osd_bm_paddr, fb_len);
#else
		virt_img = FBDEV_SaveFBImage((UINT32)wctx->pixel.osd_bm_paddr, fb_len);
#endif
		FBDEV_CHECK_ERROR(NULL==virt_img, fail_cnt++;continue, "can't save fb(%d)\n", i);

		fbmem_bkup_list[bk_img_idx].attrib	 = FBDEV_FBMEM_BKUP_IMG_POSD;
		fbmem_bkup_list[bk_img_idx].virt_img = virt_img;
		fbmem_bkup_list[bk_img_idx].phys_addr= (UINT32)wctx->pixel.osd_bm_paddr;
		fbmem_bkup_list[bk_img_idx].length   = fb_len;

		printk("save(clear) posd fb(%d) ok. paddr 0x%x, len %d\n", i, fbmem_bkup_list[bk_img_idx].phys_addr, fb_len);
		bk_img_idx++;
	}
#endif

#ifdef FBDEV_SUPPORT_CRSR_BKUP
	/* save cursor frame buffer */
	for (i=0; i<num_surf; i++)
	{
		int rc = GFX_GetSurfaceInfo(surf_fd_list[i], &surf_info);
		FBDEV_CHECK_ERROR(RET_OK!=rc, continue, "can't get surf fd 0x%x\n", surf_fd_list[i] );

		if (surf_info.width <= 256 && surf_info.height <= 256)
		{
			fb_len = surf_info.stride * surf_info.height;
			virt_img = FBDEV_SaveFBImage((UINT32)surf_info.phys_addr, fb_len);
			FBDEV_CHECK_ERROR(NULL==virt_img, fail_cnt++;continue, "can't save fb(%d)\n", i);

			fbmem_bkup_list[bk_img_idx].attrib	 = FBDEV_FBMEM_BKUP_IMG_NONE;
			fbmem_bkup_list[bk_img_idx].virt_img = virt_img;
			fbmem_bkup_list[bk_img_idx].phys_addr= (UINT32)surf_info.phys_addr;
			fbmem_bkup_list[bk_img_idx].length   = fb_len;
			bk_img_idx++;
		}
	}
#endif

	fbmem_bkup_num = bk_img_idx;
	FBDEV_DEBUG("total %d fbimg saved.. fail_cnt %d\n", fbmem_bkup_num, fail_cnt );
func_exit:
	if(surf_fd_list) vfree(surf_fd_list);
	GFX_UnlockDevice();
}

void FBDEV_RestoreFBList(void)
{
	int	i;

	BOOLEAN	b_restore_fb = TRUE;
	char* snap_boot_mode = FBDEV_GetSnapshotBootMode();

	FBDEV_CHECK_ERROR(NULL==fbmem_bkup_list, return, "fbmem_bkup_list NULL\n");
	FBDEV_CHECK_ERROR(0==fbmem_bkup_num, 	return, "fbmem_bkup_num  zero\n");

	FBDEV_DEBUG("restoring %d fbimg\n", fbmem_bkup_num);

	GFX_LockDevice();

	/* FIXME : check more situation when FB should be not restored */
	if (!strncmp(snap_boot_mode,"making",6))
	{
		FBDEV_WARN("<!> ignore FB restore - snap_making\n");
		b_restore_fb = FALSE;
	}
	else
	{
		FBDEV_WARN("<!> run FB restore\n");
	}

	for (i=0; i<fbmem_bkup_num; i++)
	{
		FBDEV_RestoreFBImage( fbmem_bkup_list[i].virt_img, fbmem_bkup_list[i].phys_addr, fbmem_bkup_list[i].length, b_restore_fb);
	}

	vfree(fbmem_bkup_list); fbmem_bkup_list=NULL;
	fbmem_bkup_num =0;

	GFX_UnlockDevice();
}

void FBDEV_ResumeFBList(void)
{
	int	i;

	FBDEV_CHECK_ERROR(NULL==fbmem_bkup_list, return, "fbmem_bkup_list NULL\n");
	FBDEV_CHECK_ERROR(0==fbmem_bkup_num, 	return, "fbmem_bkup_num  zero\n");

	FBDEV_DEBUG("destroying %d fbimg except POSD\n", fbmem_bkup_num);

	for (i=0; i<fbmem_bkup_num; i++)
	{
		/* restore only if POSD attrib is set */
		if (fbmem_bkup_list[i].attrib & FBDEV_FBMEM_BKUP_IMG_POSD )
		{
			FBDEV_RestoreFBImage( fbmem_bkup_list[i].virt_img, fbmem_bkup_list[i].phys_addr, fbmem_bkup_list[i].length, TRUE);
		}
		else
		{
			vfree(fbmem_bkup_list[i].virt_img);
		}
	}

	vfree(fbmem_bkup_list); fbmem_bkup_list=NULL;
	fbmem_bkup_num =0;
}

#else
void	FBDEV_BackupFBList(void)	{ FBDEV_WARN("not supported\n"); }
void	FBDEV_RestoreFBList(void)	{ FBDEV_WARN("not supported\n"); }
void	FBDEV_ResumeFBList(void)	{ FBDEV_WARN("not supported\n"); }

#endif /* FBDEV_USE_FBMEM_BKUP */


/**	SNAPSHOT-suspend
 *
 */
static int FBDEV_freeze(struct device* dev)
{
#ifdef KDRV_CONFIG_PM_FBDEV
	FBDEV_DRVDATA_T* drv_data = g_fbdev_drv_data;

	printk("FBDEV - SNAPSHOT SUSPEND(FREEZE)\n");

	/* backup visible status */
	drv_data->hybernation.visible_status = 0x0;

	if(g_fbdev_cfg->fb[0].b_active)
	{
		drv_data->hybernation.visible_status |= FBDEV_GetOSDEnable(0)? (1<<0): 0x0;
	}

	if(g_fbdev_cfg->fb[1].b_active)
	{
		drv_data->hybernation.visible_status |= FBDEV_GetOSDEnable(1)? (1<<1): 0x0;
	}

	if(g_fbdev_cfg->fb[2].b_active)
	{
		drv_data->hybernation.visible_status |= FBDEV_GetOSDEnable(2)? (1<<2): 0x0;
	}

	if(g_fbdev_cfg->fb[3].b_active)	/* cursor */
	{
		drv_data->hybernation.visible_status |= FBDEV_GetOSDEnable(3)? (1<<3): 0x0;
	}

	FBDEV_BackupFBList();

	/* run HW handler */
	FBDEV_RunSuspend( );

	drv_data->b_suspend = 1;
	printk("FBDEV - SNAPSHOT SUSPEND(FREEZE) DONE\n");
#endif
	return RET_OK;
}

/** resume module.
 *
 */
static int FBDEV_restore(struct device* dev)
{
#ifdef KDRV_CONFIG_PM_FBDEV
	int	i;
	BOOLEAN			 layer_onoff;
	FBDEV_WIN_CTX_T* wctx;
	FBDEV_DRVDATA_T* drv_data = g_fbdev_drv_data;

	printk("FBDEV - SNAPSHOT BOOT\n");

	FBDEV_CHECK_ERROR( drv_data->b_suspend == 0, return RET_ERROR, "FBDEV - resume FAIL\n");

	FBDEV_RestoreFBList();

	/* initialize some control variables before HW initialization */
#if (CONFIG_LX_BOOT_LOGO==1)
	for (i=0; i<FBDEV_MAX_DEVICE; i++)
	{
        wctx = FBDEV_GetWinCtx(i);

		if(wctx)
		{
			wctx->status.b_osd_hdr_flush = FALSE;   /* freeze osd header to keep boot logo shown */
		}
	}
#endif

	/* run HW handler */
	FBDEV_RunResume( );

	/* write osd header again to restore the previous state */
	for (i=0; i<FBDEV_MAX_DEVICE; i++)
	{
		wctx = FBDEV_GetWinCtx(i);
		FBDEV_CHECK_ERROR(NULL==wctx, /* nop */, "fb(%d) win ctx NULL\n", i );

		if (wctx)
		{
			layer_onoff = (drv_data->hybernation.visible_status & (1<<i))? TRUE:FALSE;

			/* raxis.lim (2014/10/18)
			 * We prefer osd freeze OFF for cursor layer when it is ON !!
			 * This will remove the garbage screen due to unitialized POSD3 ON.
			 * TODO: it's reasonable to disable the resume timer when IBOOT is activated
			 */
			if (i==LX_FBDEV_ID_CSR0 && layer_onoff )
			{
				wctx->status.b_osd_hdr_flush = TRUE;
			}

			FBDEV_SetOSDImageEndian( i, wctx->pixel.bits_per_pixel );
			FBDEV_WriteOSDHeader( &wctx->pixel );

			FBDEV_SetOSDEnable(i, layer_onoff);
		}
	}

	drv_data->b_suspend = 0;
	printk("FBDEV - SNAPSHOT BOOT DONE\n");
#endif
	return RET_OK;
}

/** IBOOT suspend handler
 *
 *
 */
static int FBDEV_suspend(struct device* dev)
{
	int rc;
	printk("FBDEV - IBOOT SUSPEND !!\n");
	rc = FBDEV_freeze(dev);
	printk("FBDEV - IBOOT SUSPEND END !!\n");

	return rc;
}

/** SNAPSHOT-resume right after making snapshot image
 *
 */
static int FBDEV_thaw(struct device* dev)
{
	int rc;
	printk("FBDEV - SNAPSHOT THAW(RESUME)\n");
	rc = FBDEV_restore(dev);
	printk("FBDEV - SNAPSHOT THAW(RESUME) DONE\n");

	return rc;
}

/** IBOOT-resume
 *
 *
 */
static int FBDEV_resume(struct device* dev)
{
#ifdef KDRV_CONFIG_PM_FBDEV
	int	i;
	BOOLEAN			 layer_onoff;
	FBDEV_WIN_CTX_T* wctx;
	FBDEV_DRVDATA_T* drv_data = g_fbdev_drv_data;

	printk("FBDEV - IBOOT RESUME\n");

	FBDEV_CHECK_ERROR( drv_data->b_suspend == 0, return RET_ERROR, "FBDEV - resume FAIL\n");

	/* destroy FB list */
	FBDEV_ResumeFBList();

	/* run HW handler */
	FBDEV_RunResume( );

	/* write osd header again to restore the previous state */
	for (i=0; i<FBDEV_MAX_DEVICE; i++)
	{
		wctx = FBDEV_GetWinCtx(i);
		FBDEV_CHECK_ERROR(NULL==wctx, /* nop */, "fb(%d) win ctx NULL\n", i );

		if (wctx)
		{
			wctx->status.b_osd_hdr_flush = TRUE;
			layer_onoff = (drv_data->hybernation.visible_status & (1<<i))? TRUE:FALSE;

			FBDEV_DEBUG("fb(%d) enable OSD flush now. osd status <%s>\n",i, (layer_onoff)?"ON":"OFF");

			FBDEV_WriteOSDHeader( &wctx->pixel );
			FBDEV_SetOSDEnable(i,layer_onoff);
		}
	}

	drv_data->b_suspend = 0;
	printk("FBDEV - IBOOT RESUME DONE\n");
#endif
	return RET_OK;
}

/**
 *	Declaration of the init and exit functions
 */

#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",FBDEV_Init);
#else
module_init(FBDEV_Init);
#endif
module_exit(FBDEV_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("FBDEV driver");
MODULE_LICENSE("GPL");
#endif

