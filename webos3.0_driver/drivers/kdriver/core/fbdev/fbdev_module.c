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
 *  main driver implementation for fbdev device.
 *
 *  author      raxis.lim (raxis.lim@lge.com)
 *  version     1.0
 *  date        2009.12.30
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
#include "fbdev_impl.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"

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
    External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void     FBDEV_H13_InitHWCtx (FBDEV_HW_CTX_T*  hal);
extern void     FBDEV_M14_InitHWCtx (FBDEV_HW_CTX_T*  hal);
extern void     FBDEV_H14_InitHWCtx (FBDEV_HW_CTX_T*  hal);
extern void     FBDEV_H15_InitHWCtx (FBDEV_HW_CTX_T*  hal);

/*----------------------------------------------------------------------------------------
    External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static ENDIAN_INFORM_T	FBDEV_GetOSDImageEndian( int bpp );

/*----------------------------------------------------------------------------------------
    Static Variables
----------------------------------------------------------------------------------------*/
/** get endian value for given bpp information
 *
 *
 */
static ENDIAN_INFORM_T	FBDEV_GetOSDImageEndian( int bpp )
{
    ENDIAN_INFORM_T endian;

    switch(bpp)
    {
        case 8:
#ifdef FBDEV_SUPPORT_8BPP_EMUL
            endian.header   = 4;
            endian.palette  = 4;
            endian.image    = 4;
#else
            endian.header   = 4;
            endian.palette  = 4;
            endian.image    = 7;
#endif
            break;
        case 16:
            endian.header   = 4;
            endian.palette  = 4;
            endian.image    = 6;
            break;

        case 32:
            endian.header   = 4;
            endian.palette  = 4;
            endian.image    = 4;
            break;

        default:
            endian.header   = 4;
            endian.palette  = 4;
            endian.image    = 4;
            break;
    }

	return endian;
}

void FBDEV_SetOSDImageEndian(int fb_dev_id, int bpp)
{

	ENDIAN_INFORM_T endian = FBDEV_GetOSDImageEndian( bpp );
	FBDEV_WriteOSDEndian(fb_dev_id , &endian);

	/* raxis.lim (2012/05/23) -- when pixel format changes, this function is always called. */
	{
		FBDEV_HW_EVENT_DATA_T evdata;

		memset( &evdata, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));
		evdata.pxl_fmt_change.fb_dev_id = fb_dev_id;
		evdata.pxl_fmt_change.bits_per_pixel = bpp;

		FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_PXL_FMT_CHANGE, &evdata );
	}
}

UINT8   FBDEV_GetOSDPxlFmt             (int bpp)
{
	switch(bpp)
	{
		case 8:		return ARGB8888;
		case 16:	return RGB565;
		case 32:
		default:	return ARGB8888;
	}
}

UINT8   FBDEV_GetOSDPxlDepth           (int bpp)
{
	switch(bpp)
	{
#ifdef FBDEV_SUPPORT_8BPP_EMUL
		case 8:		return PIX_DEPTH_32_BPP;
#else
		case 8:		return PIX_DEPTH_8_BPP;
#endif
		case 16:	return PIX_DEPTH_16_BPP;
		case 32:
		default:	return PIX_DEPTH_32_BPP;
	}
}

FBDEV_CTX_T*    FBDEV_GetCtx        (void)
{
	return g_dm_ctx;
}

FBDEV_WIN_CTX_T* FBDEV_GetWinCtx (int layer)
{
	switch( layer )
	{
		case LX_FBDEV_ID_OSD0:
		case LX_FBDEV_ID_OSD1:
		case LX_FBDEV_ID_OSD2:
		case LX_FBDEV_ID_OSD3:	return g_dm_ctx->wctx[layer];

		default:				return NULL;
	}
}

struct scanvas*     FBDEV_GetWinCanvas( int layer )
{
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(layer);

	return (wctx)? &wctx->pixel : NULL;
}


/* initialize HW Ctx variable */
void FBDEV_InitHWCtx ( void )
{
	int	i;

    /* initialize HW Ctx (Hardware Abstraction Layer) */
	if		(0)												{ /* do nothing */ }
#ifdef INCLUDE_H15_CHIP_KDRV
    if		( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15 ) ) 	{ FBDEV_H15_InitHWCtx( &g_fbdev_hw_ctx ); }
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
    else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14 ) ) 	{ FBDEV_H14_InitHWCtx( &g_fbdev_hw_ctx ); }
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
    else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14 ) ) 	{ FBDEV_M14_InitHWCtx( &g_fbdev_hw_ctx ); }
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
    else if	( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13 ) ) 	{ FBDEV_H13_InitHWCtx( &g_fbdev_hw_ctx ); }
#endif
    else 													{ DBG_PRINT_ERROR("unknown chip 0x%x\n", lx_chip_rev() ); }

	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		g_fbdev_hw_ctx.fb[i].trid_ctrl.mode		= LX_FBDEV_TRID_MODE_NONE;
		g_fbdev_hw_ctx.fb[i].trid_ctrl.disparity= 0x0;
	}
}

/* get HW ctx */
FBDEV_HW_CTX_T*			FBDEV_GetHWCtx(void)
{
	return &g_fbdev_hw_ctx;
}

/** initialize layer's memory and palette.
 *
 * @param	struct fb_info *info
 * @return	void
 *
 */
void FBDEV_InitCanvas(FBDEV_WIN_CTX_T *wctx)
{
	CSC_INFORM_T				csc_inform;
	ENDIAN_INFORM_T				endian_inform;

	int							fb_dev_id = wctx->fb_dev_id;
	struct fb_var_screeninfo*	vinfo	= &wctx->fbinfo.var;
	struct scanvas* 			hCanvas	= &wctx->pixel;

	/* configure OSD.
	 * this function will inform OSD hardware to read OSD header data from the designated DDR ( canv->baddr )
 	 */
	FBDEV_InitOSDLayer( fb_dev_id );

	/* set the default CSC param
	 *
	 * [note] We don't use YCbCr color space, so any CSC is not necessary.
	 * [note] CSC is not set anymore after this point.
	 */
	memset( &csc_inform, 0x0, sizeof(CSC_INFORM_T));
	csc_inform.bypass = 1;

	/* determine OSD endian info
	 * [note] You can see FBDEV_GetImangeEndian function process only RGB color space.
	 * 		  If you need to need YCbCr color space, we should upgrade that function.
	 * [note] endian is not set anymore after this point. ( one exception is the change of pixel format )
	 */
	memset( &endian_inform, 0x0, sizeof(ENDIAN_INFORM_T) );
	endian_inform = FBDEV_GetOSDImageEndian( vinfo->bits_per_pixel );

    FBDEV_WriteOSDEndian( fb_dev_id, &endian_inform);
    FBDEV_WriteOSDCSC	( fb_dev_id, &csc_inform);

	/* initialize image data */
	hCanvas->fb_dev_id			= fb_dev_id;
	hCanvas->osd_hdr_addr		= wctx->osd_hdr_base;
	hCanvas->osd_plte_paddr 	= (gMemCfgFBDev[fb_dev_id].mem_palette_base);
	hCanvas->osd_bm_paddr		= (gMemCfgFBDev[fb_dev_id].mem_frame_base);

	hCanvas->input_win.x		= 0;
	hCanvas->input_win.y		= 0;
	hCanvas->input_win.w		= vinfo->xres;		/* default input_win to be same as var->xres */
	hCanvas->input_win.h		= vinfo->yres;		/* default input_win to be same as var->xres */

	hCanvas->output_win.x		= 0;
	hCanvas->output_win.y		= 0;
	hCanvas->output_win.w		= vinfo->xres;		/* default output_win to be same as var->xres */
	hCanvas->output_win.h		= vinfo->yres;		/* default output_win to be same as var->xres */

	hCanvas->reserver_screen_en	= FALSE;
	hCanvas->pixel_order_en		= FALSE;
	hCanvas->color_key_en		= FALSE;
	hCanvas->global_alpha_en	= g_fbdev_cfg->fb[fb_dev_id].b_use_global_alpha;
	hCanvas->blend_alpha_en		= FALSE;

	hCanvas->bits_per_pixel		= vinfo->bits_per_pixel;
	hCanvas->global_alpha		= 0xff;
	hCanvas->blend_alpha		= 0xff;

	hCanvas->palette			= (UINT32*)OS_Malloc( sizeof(UINT32) * 256 );

	/* calucate stride */
	hCanvas->stride				= vinfo->xres_virtual * vinfo->bits_per_pixel / 8;

#if 0
	/* sync current data to OSD header register. */
	FBDEV_WriteOSDHeader( hCanvas );
#endif
}

/** memory freeing for each header.
 *
 * @param	dm_win_info **win - osd plane information
 * @return	int 0 : OK , -1 : NOT OK
 */
int FBDEV_CleanupLayer(FBDEV_WIN_CTX_T *wctx)
{
	if ( wctx )
	{
		if ( wctx->osd_hdr_base ) 		iounmap((void *)wctx->osd_hdr_base);
		if ( wctx->osd_hdr_base_phys ) 	release_mem_region(wctx->osd_hdr_base_phys, wctx->osd_hdr_size);
		if ( wctx->osd_plte_base_phys ) 	release_mem_region(wctx->osd_plte_base_phys, wctx->osd_plte_size);

		kfree(wctx);
	}
	return (0);
}

/** memory allocation for each header.
 *
 * @param dm_win_info **win - osd plane information
 * @param unsigned long fb_base_phys - framebuffer physical address
 * @param unsigned long fb_size - framebuffer size
 * @param char *fbname - osd layer name
 * @return	int 0 : OK , -1 : NOT OK
 */
int FBDEV_InitLayer(FBDEV_WIN_CTX_T **win, unsigned long fb_base_phys, unsigned long fb_size, char *fbname)
{
	FBDEV_WIN_CTX_T *w;

	w = OS_Malloc(sizeof(FBDEV_WIN_CTX_T));
	FBDEV_CHECK_ERROR( !w, return RET_ERROR, "out of memory. can't alloc %s\n", fbname );

	memset(w, 0, sizeof(FBDEV_WIN_CTX_T));

	w->fb_base_phys = fb_base_phys;
	w->fb_size		= fb_size;

 	/* osd header address setting */
	if( is_win(fbname,OSD0) )
	{
		w->fb_dev_id			= 0;
		w->osd_hdr_base_phys 	= gMemCfgFBHdr[0].mem_header_base;
		w->osd_hdr_size 		= gMemCfgFBHdr[0].mem_header_size;
		w->osd_plte_base_phys 	= gMemCfgFBDev[0].mem_palette_base;
		w->osd_plte_size 		= gMemCfgFBDev[0].mem_palette_size ;
	}
	else if(is_win(fbname,OSD1) )
	{
		w->fb_dev_id			= 1;
		w->osd_hdr_base_phys 	= gMemCfgFBHdr[1].mem_header_base;
		w->osd_hdr_size 		= gMemCfgFBHdr[1].mem_header_size;
		w->osd_plte_base_phys 	= gMemCfgFBDev[1].mem_palette_base;
		w->osd_plte_size 		= gMemCfgFBDev[1].mem_palette_size;
	}
	else if( is_win(fbname,OSD2) )
	{
		w->fb_dev_id			= 2;
		w->osd_hdr_base_phys 	= gMemCfgFBHdr[2].mem_header_base;
		w->osd_hdr_size			= gMemCfgFBHdr[2].mem_header_size;
		w->osd_plte_base_phys	= gMemCfgFBDev[2].mem_palette_base;
		w->osd_plte_size		= gMemCfgFBDev[2].mem_palette_size;
	}
	else if(is_win(fbname,OSD_CURSOR) )
	{
		w->fb_dev_id			= 3;
		w->osd_hdr_base_phys	= gMemCfgFBHdr[3].mem_header_base;
		w->osd_hdr_size			= gMemCfgFBHdr[3].mem_header_size;
		w->osd_plte_base_phys	= gMemCfgFBDev[3].mem_palette_base;
		w->osd_plte_size		= gMemCfgFBDev[3].mem_palette_size;
	}
	else
	{
		FBDEV_ERROR("%s: cannot define osd header region\n", fbname);
	}

	w->osd_hdr_base = (unsigned long)ioremap(w->osd_hdr_base_phys, w->osd_hdr_size);
	FBDEV_CHECK_ERROR( !w->osd_hdr_base, goto release_fb, "%s: can't mmap fbmem\n", fbname );

	*win = w;
	return RET_OK;

release_fb:
	OS_Free(w);
	return RET_ERROR;
}

/** control OSD output connection
 *
 */
int     FBDEV_SetConnCtrl           (LX_FBDEV_CONN_CTRL_T* ctrl)
{
	int	ret = RET_OK;

	if ( lx_chip_rev() < LX_CHIP_REV(L9,A0) ) return RET_NOT_SUPPORTED;

	if ( g_fbdev_hw_ctx.SetConnCtrl )
	{
		ret = g_fbdev_hw_ctx.SetConnCtrl( ctrl );
	}

	return ret;
}

int		FBDEV_SetPivotCtrl			(LX_FBDEV_PIVOT_CTRL_T* ctrl)
{
	int	ret = RET_OK;
	FBDEV_CTX_T* fbctx = FBDEV_GetCtx();

	fbctx->global.pivot_mode = ctrl->mode;

	if ( g_fbdev_hw_ctx.SetPivotCtrl )
	{
		ret = g_fbdev_hw_ctx.SetPivotCtrl( ctrl );
	}

	return ret;
}

/** Set Palette
 *
 * @param	index		palette index
 * @param	pPalette	pointer to array bearing palette data
 *
 * @return	void
 */
void    FBDEV_SetPalette            (FBDEV_WIN_CTX_T *w, UINT32* pPalette, int num_pal )
{
	unsigned long base_phys;
	unsigned long size;
	struct scanvas* canvas = &w->pixel;

	if( num_pal > 256)
	{
		FBDEV_WARN("palette data is too big\n");
		num_pal = 256;
	}

	memcpy( canvas->palette, pPalette, sizeof(UINT32)*num_pal );

	base_phys	= w->osd_plte_base_phys;
	size		= w->osd_plte_size;

#ifdef FBDEV_SUPPORT_8BPP_EMUL
	/* do nothing */
#else
	OS_WrData( base_phys, pPalette, num_pal );
#endif

	return;
}

/*========================================================================================
    Implementation Group (HW Ctx Interface)
========================================================================================*/

/** initialize OSD IP HW and register
 *
 * @param
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_InitHardware(void)
{
	return g_fbdev_hw_ctx.InitHW();
}

/** de-initial OSD IP HW and register
 *
 * @param
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_ExitHardware(void)
{
	return g_fbdev_hw_ctx.ShutdownHW();
}

int	FBDEV_RunResume(void)
{
	return g_fbdev_hw_ctx.RunResume();
}

int FBDEV_RunSuspend(void)
{
	return g_fbdev_hw_ctx.RunSuspend();
}

/** Update OSD ZList
 *
 * @note layer id definition is 00: video, 01: OSD0, 02: OSD1, 03: OSD2
 */
int FBDEV_SetZList ( LX_FBDEV_ZLIST_T* zList )
{
	return g_fbdev_hw_ctx.SetZList( zList );
}

/** Get OSD ZList
 *
 * @note layer id definition is 00: video, 01: OSD0, 02: OSD1, 03: OSD2
 */
int FBDEV_GetZList( LX_FBDEV_ZLIST_T* zList )
{
	return g_fbdev_hw_ctx.GetZList ( zList );
}

int		FBDEV_SetViewCtrl			(LX_FBDEV_VIEW_CTRL_T* ctrl)
{
	FBDEV_CHECK_ERROR( NULL == g_fbdev_hw_ctx.SetViewCtrl, return RET_ERROR, "not supported function\n");

	return g_fbdev_hw_ctx.SetViewCtrl( ctrl );
}


/** OSD HW Layer enable/disable
 *
 * @param	int layer : osd layer ,
 *		enable : 0 : display off , 1 : display on
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_SetOSDEnable(int layer , int enable)
{
	return g_fbdev_hw_ctx.SetOSDEnable( layer, enable );
}

/** OSD HW Layer status
 *
 * @param	void
 * @return	int 1 : ON , 0 : OFF
 *
 */
int FBDEV_GetOSDEnable	(int layer)
{
	return g_fbdev_hw_ctx.GetOSDEnable( layer );
}

/** make the basic configuration for each OSD layer.
 *
 * [note] Do NOT use header setting in register
 *
 * @param	[IN] layer
 * @return	int 0 : OK , -1 : NOT OK
 */
int FBDEV_InitOSDLayer	(int layer)
{
	return g_fbdev_hw_ctx.InitOSDLayer(layer);
}

/** set endianess on each layer
 *
 * @param	ENDIAN_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_WriteOSDEndian(int layer , ENDIAN_INFORM_T *endian)
{
	return g_fbdev_hw_ctx.WriteOSDEndian( layer, endian );
}

/**
 *
 * set color space conversion on each layer
 *
 * @param	int layer , CSC_INFORM_T *csc
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_WriteOSDCSC(int layer , CSC_INFORM_T *csc)
{
	return g_fbdev_hw_ctx.WriteOSDCSC( layer, csc );
}

/** Get interrupt status
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_GetInterruptStatus(INTERRUPT_INFORM_T intr)
{
	return g_fbdev_hw_ctx.GetInterruptStatus( intr );
}

/** Clear interrupt status
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_SetInterruptClear(INTERRUPT_INFORM_T intr)
{
	return g_fbdev_hw_ctx.SetInterruptClear( intr );
}

/** Set interrupt enable/disable
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T , int : enable/disable
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_SetInterruptEnable(INTERRUPT_INFORM_T intr , int enable)
{
	return g_fbdev_hw_ctx.SetInterruptEnable( intr, enable );
}

/** Set Header information
 *
 * @param	SCanvas : pointer to structure bearing osd header information
 * @return	void
 *
 * @ingroup osd_ddi_func
 */
int FBDEV_WriteOSDHeader(struct scanvas *hCanvas)
{
	int	fb_dev_id;
	FBDEV_WIN_CTX_T* wctx;
	FBDEV_CTX_T* fbctx = FBDEV_GetCtx();

	FBDEV_CHECK_ERROR( NULL == hCanvas, return RET_ERROR, "null param\n");
	FBDEV_CHECK_ERROR( NULL == g_fbdev_hw_ctx.WriteOSDHeader, return RET_ERROR, "null callback\n");
	FBDEV_CHECK_CODE( fbctx->global.b_freeze, return RET_OK, "osd freezed\n");

	fb_dev_id = hCanvas->fb_dev_id;
	wctx = FBDEV_GetWinCtx(fb_dev_id);

	FBDEV_CHECK_ERROR( NULL == wctx, return RET_ERROR, "fb(%d) null winctx\n", fb_dev_id );
	FBDEV_CHECK_ERROR( FALSE == g_fbdev_cfg->fb[fb_dev_id].b_active, return RET_OK, "fb(%d) inactive. ignore osd_hdr update\n", fb_dev_id );

	FBDEV_CHECK_DEBUG( FALSE == wctx->status.b_osd_hdr_write, return RET_OK, "fb(%d) osd_hdr disabled. ignore osd_hdr update\n", fb_dev_id );

	return g_fbdev_hw_ctx.WriteOSDHeader( hCanvas );
}

/** control 3D OSD
 *
 */
int	FBDEV_SetTridCtrl( int layer, LX_FBDEV_TRID_CTRL_T* ctrl )
{
	int	ret = RET_OK;

	if ( g_fbdev_hw_ctx.SetTridCtrl )
	{
		g_fbdev_hw_ctx.fb[layer].trid_ctrl = *ctrl;
		ret = g_fbdev_hw_ctx.SetTridCtrl( layer, ctrl );
	}
	return ret;
}

/** control video mixer
 *
 */
int	FBDEV_SetVideoMixerCtrl( LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl)
{
	int	ret = RET_OK;

	if ( g_fbdev_hw_ctx.SetVideoMixerCtrl )
	{
		ret = g_fbdev_hw_ctx.SetVideoMixerCtrl( ctrl );
	}
	return ret;
}

/** ISR based tick handelr
 *
 */
void FBDEV_ISRTickHandler ( UINT32 int_cnt )
{
	FBDEV_HW_EVENT_DATA_T data;
	memset( &data, 0x0, sizeof(FBDEV_HW_EVENT_DATA_T));

	data.vsync.sync_counter = int_cnt;

	FBDEV_ExecFBHWHandler( FBDEV_HW_EVENT_VSYNC, &data );
}

/** execute registered FB internal event to necessary module
 *	behaviour is different based on chip version
 *
 */
void FBDEV_ExecFBHWHandler ( FBDEV_HW_EVENT_T ev, FBDEV_HW_EVENT_DATA_T* data )
{
	if ( g_fbdev_hw_ctx.EventCallback )
	{
		g_fbdev_hw_ctx.EventCallback( ev, data );
	}
}

/*========================================================================================
    Implementation Group (MONITOR)
========================================================================================*/
typedef struct
{
	BOOLEAN			b_onoff;
	UINT32			isr_cnt;
	UINT32			refresh_cnt[LX_FBDEV_ID_MAX];

	UINT64			start_ns_tk;
	OS_TIMER_T		tmr;
}
FBDEV_FPS_MON_T;

static FBDEV_FPS_MON_T	g_fbdev_fps_mon;

static void	FBDEV_FPSMon_Report		( ULONG data )
{
	int		i;
	//UINT64	curr_ns_tk = OS_GetNsecTicks();
	UINT32	duration   = data;

	printk("FBDEV FPS Info (%d sec)\n", duration );
	printk("ISR: cnt=%d,fps=%d ", g_fbdev_fps_mon.isr_cnt, g_fbdev_fps_mon.isr_cnt/duration );

	for (i=0; i<LX_FBDEV_ID_MAX; i++)
	{
		if(g_fbdev_cfg->fb[i].b_active )
		{
			printk(", fb(%d) cnt=%d,fps=%d", i, g_fbdev_fps_mon.refresh_cnt[i], g_fbdev_fps_mon.refresh_cnt[i]/ duration );
		}
	}
	printk("\n");

	/* reinit data */
	g_fbdev_fps_mon.isr_cnt = 0;
	memset(g_fbdev_fps_mon.refresh_cnt, 0x0, sizeof(UINT32)*LX_FBDEV_ID_MAX);

	g_fbdev_fps_mon.start_ns_tk = OS_GetNsecTicks();

}

void		FBDEV_MON_Init		(void)
{
	memset(&g_fbdev_fps_mon, 0x0, sizeof(FBDEV_FPS_MON_T));

	OS_InitTimer( &g_fbdev_fps_mon.tmr );
}

void		FBDEV_MON_SetFPSMon	( BOOLEAN onoff )
{
	if (onoff)
	{
		g_fbdev_fps_mon.b_onoff = TRUE;
		g_fbdev_fps_mon.isr_cnt = 0;
		memset(g_fbdev_fps_mon.refresh_cnt, 0x0, sizeof(UINT32)*LX_FBDEV_ID_MAX);
		g_fbdev_fps_mon.start_ns_tk = OS_GetNsecTicks();

		OS_StartTimer( &g_fbdev_fps_mon.tmr, FBDEV_FPSMon_Report, OS_TIMER_TIMETICK, 2000, (ULONG)2 );
	}
	else
	{
		g_fbdev_fps_mon.b_onoff = FALSE;
		OS_StopTimer( &g_fbdev_fps_mon.tmr );
	}
}

void		FBDEV_MON_NotiRefresh( int fb_dev_id )
{
	if (g_fbdev_fps_mon.b_onoff)
	{
		if(LX_FBDEV_IS_VALID_ID(fb_dev_id))
		{
			g_fbdev_fps_mon.refresh_cnt[fb_dev_id]++;
		}
	}
}

void		FBDEV_MON_ISRTick	(void)
{
	if (g_fbdev_fps_mon.b_onoff)
	{
		g_fbdev_fps_mon.isr_cnt++;
	}
}

