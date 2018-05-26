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
 *  Brief description. 
 *  Detailed description starts here. 
 *
 *  @author		raxis.lim
 *  @version	1.0 
 *  @date		2011-04-03
 *  @note		Additional information. 
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define	UPDATE_VOSD_IDX_INSIDE_ISR	/* update virtual osd index inside isr handler.
									 * sometimes, it's better to update osd index for every frame update 
									 * so, more test & analysis needed.
									 * the best method is to use tripple buffer
									 */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_drv.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"
#include "fbdev_hw_h14.h"
#include "fbdev_reg_h14.h"
#include "mif_osd_reg_h14.h"

#include "gfx_draw.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#undef FBDEV_DEBUG
//#define FBDEV_DEBUG		printk

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern  volatile OSD_H14_REG_T*     g_fbdev_h14_reg;
extern  OSD_H14_REG_T*              g_fbdev_h14_reg_cached;
extern  volatile MIF_OSD_H14_REG_T* g_mif_osd_h14_reg;
extern  MIF_OSD_H14_REG_T*          g_mif_osd_h14_reg_cached;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void		FBDEV_H14CMN_InitVOSD		( int fb_dev_id, UINT32 opmask );
static void		FBDEV_H14CMN_ExitVOSD		( int fb_dev_id );

static UINT32	FBDEV_H14CMN_RemapFRCHeader ( UINT32 in );
static void		FBDEV_H14CMN_InitOSDMerge	(void);
static void		FBDEV_H14CMN_ExitOSDMerge	(void);

static BOOLEAN	FBDEV_H14CMN_IsReverseOSD	( int fb_dev_id );

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DEFINE_SPINLOCK(g_osd_hdr_spinlock);

static int 	_g_fbdev_h14_v_phase_offset		= 0x27;	/* determined by test. used when reverse mode enabled */
module_param_named( fbdev_h14_v_phase_offset, _g_fbdev_h14_v_phase_offset, int, 0644 );

static int 	_g_fbdev_h14_mrb_bank_mode		= 0x0;	/* H14 H/W default. TODO: read the default value from H/W */
static int 	_g_fbdev_h14_mrb_bank_mode_req	= 0x0;
module_param_named( fbdev_h14_mrb_bank_mode, _g_fbdev_h14_mrb_bank_mode_req, int, 0644 );

/*----------------------------------------------------------------------------------------
	Static Functions
----------------------------------------------------------------------------------------*/
static BOOLEAN	FBDEV_H14CMN_IsReverseOSD	( int fb_dev_id )
{
	UINT32	v0, v1;

	OSD_H14_RdFL( osd_mixer_ctrl_sync ); v0 = OSD_H14_Rd00(osd_mixer_ctrl_sync, reg_sync_v_reverse);

	switch(fb_dev_id)
	{
		case 0: OSD_H14_RdFL( osd0_ctrl_main ); v1= OSD_H14_Rd00(osd0_ctrl_main, osd0_v_reverse_en); break;
		case 1: OSD_H14_RdFL( osd1_ctrl_main ); v1= OSD_H14_Rd00(osd1_ctrl_main, osd1_v_reverse_en); break;
		case 2: OSD_H14_RdFL( osd2_ctrl_main ); v1= OSD_H14_Rd00(osd2_ctrl_main, osd2_v_reverse_en); break;
		case 3: OSD_H14_RdFL( osd3_ctrl_main ); v1= OSD_H14_Rd00(osd3_ctrl_main, osd3_v_reverse_en); break;
		default: v1=FALSE; break;
	}

	return (v0 || v1)? TRUE:FALSE;
}

/** clear OSD region at the initialization stage.
 *
 */
static void FBDEV_H14CMN_FillOSDRegion( int fb_dev_id, UINT32 color )
{
    LX_FBDEV_FB_IMAGE_T fb_screen;
    LX_GFX_SURFACE_SETTING_T surf_info;

    /* alloc gfx surface & clear */
    int w = g_fbdev_cfg->display_width;
    int h = g_fbdev_cfg->display_height;

    int surf_fd = GFX_AllocARGBSurface( w, h, 0x0 );
    FBDEV_CHECK_CODE(surf_fd < 0, return, "can't alloc surface\n");

    GFX_FillSurface( surf_fd, 0, 0, w, h, color );

    memset( &fb_screen, 0x0, sizeof(LX_FBDEV_FB_IMAGE_T));

    GFX_GetSurfaceInfo(surf_fd, &surf_info);

    fb_screen.phys_addr     = (UINT32)surf_info.phys_addr;
    fb_screen.stride        = w<<2;
    fb_screen.bits_per_pixel= 32;

    FBDEV_FlipFBImage( fb_dev_id, &fb_screen, FALSE);

    GFX_FreeSurface(surf_fd);
}

/** enable OSD hdr flush
 *
 */
static void FBDEV_H14CMN_EnableOSDHdrFlush( ULONG data )
{
#if (CONFIG_LX_BOOT_LOGO==1)
    int i;

    FBDEV_INFO("enable osd_hdr_flush & exit boot logo stage\n");

    /* enable osd_hdr_flush & flush the pending osd header */
    for (i=0; i<FBDEV_MAX_DEVICE; i++)
    {
        FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(i);
        if(wctx) wctx->status.b_osd_hdr_flush = TRUE;
    }

#ifdef FBDEV_USE_HDR_RDY_REG
    //if( g_fbdev_h14_reg->osd0_ctrl_main.osd0_en )
    {
        FBDEV_INFO("fb(0) flush header\n");
        g_fbdev_h14_reg->osd0_ctrl_main.osd0_hdr_ready_flag = 0x1;
    }

    //if( g_fbdev_h14_reg->osd1_ctrl_main.osd1_en )
    {
        FBDEV_INFO("fb(1) flush header\n");
        g_fbdev_h14_reg->osd1_ctrl_main.osd1_hdr_ready_flag = 0x1;
    }

    //if( g_fbdev_h14_reg->osd3_ctrl_main.osd3_en )
    {
        FBDEV_INFO("fb(3) flush header\n");
        g_fbdev_h14_reg->osd3_ctrl_main.osd3_hdr_ready_flag = 0x1;
    }
#endif

    /* enable video_mix & reconfigure osd mixer */
    g_fbdev_h14_hw_ctx.video_mixer_ctrl.b_video_mix = TRUE;
    FBDEV_H14_InitOSDZOrder( );
#endif
}

/** 
 *
 */
void FBDEV_H14CMN_RunBootLogoTimer( UINT32 ms, BOOLEAN bForce )
{
#if (CONFIG_LX_BOOT_LOGO==1)
	static BOOLEAN		b_timer_req = TRUE;
	static OS_TIMER_T	osd_en_timer;


	if ( b_timer_req || bForce )
	{
		b_timer_req = FALSE;
		FBDEV_INFO("start boot timer %d ms (%d)\n", ms, bForce );

	#ifdef FBDEV_USE_COLD_BOOT_CLS
	/* clear screen when cold boot. bForce=FALSE !! */
	if (!bForce)
	{
		FBDEV_H14CMN_FillOSDRegion(CONFIG_LX_BOOT_LOGO_FBDEV, 0xff000000 /* black */ );
	}
	#endif
		OS_InitTimer ( &osd_en_timer );
		OS_StartTimer( &osd_en_timer, FBDEV_H14CMN_EnableOSDHdrFlush, OS_TIMER_TIMEOUT, ms, 0x0 );
	}
#else
	FBDEV_H14CMN_EnableOSDHdrFlush(0x0);
#endif
}

/*========================================================================================
    Implementation Group (OSD HW Initialization)
========================================================================================*/

/** make OSD to be initial state
 *	make the basic configuration for each OSD layer.
 *
 *	@param layerId
 *
 *	[OK]
 */
int FBDEV_H14CMN_InitOSDLayer ( int layer )
{
	int ret = RET_ERROR;

	/* please refer to OSD manual OSD?_CFG field */
	UINT32  hdr_src_sel;
	UINT32  osd_hdr_addr;

	if ( g_reg_io_ctx->hdr_op_method == FBDEV_HDR_OP_MMIO )
	{
		hdr_src_sel = 0x0;
	}
	else
	{
		hdr_src_sel = 0x1;
	}

	osd_hdr_addr = (gMemCfgFBHdr[layer].mem_header_base);

    switch( layer )
    {
        case LX_FBDEV_ID_OSD0:
#if (CONFIG_LX_BOOT_LOGO==1 && CONFIG_LX_BOOT_LOGO_FBDEV==0)
            g_fbdev_h14_reg->osd0_ctrl_main.osd0_en 				= 1;			/* enable OSD to keep logo */
#else
            g_fbdev_h14_reg->osd0_ctrl_main.osd0_en 				= 0;			/* disable OSD by default */
#endif
#ifdef FBDEV_USE_HDR_RDY_REG
			g_fbdev_h14_reg->osd0_ctrl_main.osd0_hdr_ready_flag_en	= 0x1;
#endif
            g_fbdev_h14_reg->osd0_ctrl_main.osd0_hdr_src_sel		= hdr_src_sel;
            g_fbdev_h14_reg->osd0_ctrl_base_addr.osd0_base_addr 	= osd_hdr_addr;	/* set header data address */

			g_fbdev_h14_reg->osd0_ctrl_scaler.osd0_sc_sampling_mode	= 0x1;			/* set scaler sample mode to normal (same as GFX) */
        break;

        case LX_FBDEV_ID_OSD1:
#if (CONFIG_LX_BOOT_LOGO==1 && CONFIG_LX_BOOT_LOGO_FBDEV==1)
            g_fbdev_h14_reg->osd1_ctrl_main.osd1_en 				= 1;			/* keep logo */
#else
            g_fbdev_h14_reg->osd1_ctrl_main.osd1_en 				= 0;			/* disable OSD by default */
#endif
#ifdef FBDEV_USE_HDR_RDY_REG
			g_fbdev_h14_reg->osd1_ctrl_main.osd1_hdr_ready_flag_en	= 0x1;
#endif
            g_fbdev_h14_reg->osd1_ctrl_main.osd1_hdr_src_sel		= hdr_src_sel;
            g_fbdev_h14_reg->osd1_ctrl_base_addr.osd1_base_addr 	= osd_hdr_addr; /* set header data address */

			g_fbdev_h14_reg->osd1_ctrl_scaler.osd1_sc_sampling_mode	= 0x1;			/* set scaler sample mode to normal (same as GFX) */
        break;

        case LX_FBDEV_ID_OSD2:
            g_fbdev_h14_reg->osd2_ctrl_main.osd2_en 				= 0;			/* disable OSD by default */
#ifdef FBDEV_USE_HDR_RDY_REG
           	g_fbdev_h14_reg->osd2_ctrl_main.osd2_hdr_ready_flag_en	= 0x1;
#endif
            g_fbdev_h14_reg->osd2_ctrl_main.osd2_hdr_src_sel 		= hdr_src_sel;
            g_fbdev_h14_reg->osd2_ctrl_base_addr.osd2_base_addr		= osd_hdr_addr;	/* set header data address */

			g_fbdev_h14_reg->osd1_ctrl_scaler.osd1_sc_sampling_mode	= 0x1;			/* set scaler sample mode to normal (same as GFX) */
        break;

        case LX_FBDEV_ID_CSR0:
            g_fbdev_h14_reg->osd3_ctrl_main.osd3_en 				= 0;			/* disable OSD by default */
#ifdef FBDEV_USE_HDR_RDY_REG
           	g_fbdev_h14_reg->osd3_ctrl_main.osd3_hdr_ready_flag_en	= 0x1;
#endif
            g_fbdev_h14_reg->osd3_ctrl_main.osd3_hdr_src_sel 		= hdr_src_sel;
            g_fbdev_h14_reg->osd3_ctrl_base_addr.osd3_base_addr 	= osd_hdr_addr; 	/* set header data address */

			g_fbdev_h14_reg->osd1_ctrl_scaler.osd1_sc_sampling_mode  = 0x1;			/* set scaler sample mode to normal (same as GFX) */
        break;

        default:
            FBDEV_ERROR("invalid osd %d\n", layer ); goto func_exit;
        break;
    }

    ret = RET_OK;   /* all work done */
func_exit:

    return ret;
}
/*========================================================================================
    Implementation Group (OSD Hdr Processing)
========================================================================================*/
/** write real header data to register
 *
 *
 *
 */
void	FBDEV_H14CMN_WriteHdrRegs	( int fb_dev_id, FBDEV_H14_OSD_HDR_T* osd_hdr, FBDEV_H14_OSD_EXHDR_T* osd_exhdr )
{
	ULONG   flags;
	struct scanvas* hCanvas = NULL;
	FBDEV_WIN_CTX_T* wctx	= NULL;

	hCanvas = FBDEV_GetWinCanvas( fb_dev_id );
	FBDEV_CHECK_CODE(NULL==hCanvas, return, "[fb] invalid layer %d\n", fb_dev_id );

	wctx = FBDEV_GetWinCtx(fb_dev_id);
	FBDEV_CHECK_CODE(NULL==wctx, return, "fb(%d) - null winctx\n", fb_dev_id );

	spin_lock_irqsave(&g_osd_hdr_spinlock, flags);

	if ( g_reg_io_ctx->hdr_op_method == FBDEV_HDR_OP_MMIO )
	{
		memcpy( (void*)hCanvas->osd_hdr_addr, (void*)osd_hdr, sizeof(FBDEV_H14_OSD_HDR_T) );
	}
	else
	{
		UINT32* p_osd_hdr_data = (UINT32 *)osd_hdr;

		/* raxis.lim (2013/12/18) - notify osd freeze for debug.
		 */
		if ( !wctx->status.b_osd_hdr_flush )
		{
			FBDEV_INFO("fb(%d) fbosd_hdr_flush disabled. \n", fb_dev_id );
		}

		switch( fb_dev_id )
		{
			case 0:
				FBDEV_REG_MEMCPY( &g_fbdev_h14_reg->osd0_ctrl_hdr0, p_osd_hdr_data, 7);
#ifdef FBDEV_USE_HDR_RDY_REG
				if ( wctx->status.b_osd_hdr_flush )
				g_fbdev_h14_reg->osd0_ctrl_main.osd0_hdr_ready_flag = 0x1;
#endif
			break;
			case 1:
				FBDEV_REG_MEMCPY( &g_fbdev_h14_reg->osd1_ctrl_hdr0, p_osd_hdr_data, 7);
#ifdef FBDEV_USE_HDR_RDY_REG
				if ( wctx->status.b_osd_hdr_flush )
				g_fbdev_h14_reg->osd1_ctrl_main.osd1_hdr_ready_flag = 0x1;
#endif
			break;
			case 2:
				FBDEV_REG_MEMCPY( &g_fbdev_h14_reg->osd2_ctrl_hdr0, p_osd_hdr_data, 7);
#ifdef FBDEV_USE_HDR_RDY_REG
				if ( wctx->status.b_osd_hdr_flush )
				g_fbdev_h14_reg->osd2_ctrl_main.osd2_hdr_ready_flag = 0x1;
#endif
			break;
			case 3:
				FBDEV_REG_MEMCPY( &g_fbdev_h14_reg->osd3_ctrl_hdr0, p_osd_hdr_data, 7);
#ifdef FBDEV_USE_HDR_RDY_REG
				if ( wctx->status.b_osd_hdr_flush )
				g_fbdev_h14_reg->osd3_ctrl_main.osd3_hdr_ready_flag = 0x1;
#endif
			break;
		}
	}

	/* extened osd header for 3DOSD
	 * 3DOSD is used only for FHD model. normally 3DOSD is not processed in H14 SOC in UHD model.
	 */
	if ( lx_chip_plt() == LX_CHIP_PLT_FHD )
	{
		UINT32	val0, val1;

		FBDEV_HW_CTX_T* hwctx = FBDEV_GetHWCtx();
		LX_FBDEV_TRID_CTRL_T* trid_ctrl = &(hwctx->fb[fb_dev_id].trid_ctrl);

		UINT32	disp_val_l	= 0x100;
		UINT32	disp_val_r	= 0x100;
		UINT32	disparity	= trid_ctrl->disparity;

		UINT32	b_s3d_en	= (trid_ctrl->mode & (LX_FBDEV_TRID_MODE_SS|LX_FBDEV_TRID_MODE_TB))? 0x1: 0x0;
		UINT32	b_2d3d_en	= (trid_ctrl->mode & LX_FBDEV_TRID_MODE_2D_3D)? 0x1: 0x0;
		BOOLEAN b_reverse_en= FBDEV_H14CMN_IsReverseOSD(fb_dev_id);

		UINT32	l_bmp_addr	= osd_hdr->osd_hdr_ptr_bmp;
		UINT32	r_bmp_addr	= l_bmp_addr;

		/* raxis.lim (2012/07/12) -- setup disparity
		 * maximum (effective) available disparity is between -0x40 and +0x40 in H14.
		 * this is not H/W restriction, just optimal max value for all type of panel.
		 */
		if ( trid_ctrl->mode != LX_FBDEV_TRID_MODE_2D )
		{
			if ( disparity > 0x40 ) disparity = 0x40;

			if ( trid_ctrl->mode & LX_FBDEV_TRID_MODE_EXT_DOWN )
			{
				disp_val_l	= 0x100 + disparity;	/* move L to right direction */
				disp_val_r	= 0x100 - disparity;	/* move R to left  direction */
			}
			else
			{
				disp_val_l	= 0x100 - disparity;	/* move L to left  direction */
				disp_val_r	= 0x100 + disparity;	/* move R to right direction */
			}
		}

		/* if S3D is enabled, setup right fbmem addr points rigth half of screen.
		 *
		 * raxis.lim (2012/12/18) HW workaround in reverse mode.
	 	 * because MIF HW reads the invalid base address in reverse mode, we should adjust address for MIF to read the right data
		 * see internal docs for more information
		 *
		 * SS: we should move L/R bmp address by -(minus) a half of bmp_lenth.
		 * TB: we should move L/R bmp address by -(minus) a half of bmp_lenth (FIXME: check it)
		 */
		if ( b_s3d_en )
		{
			if (trid_ctrl->mode & LX_FBDEV_TRID_MODE_SS ) /* S/S */
			{
				r_bmp_addr += ((osd_hdr->osd_hdr_w_mem * osd_exhdr->bits_per_pixel)>>3);

				if ( b_reverse_en )
				{
					UINT32 bmp_len = (osd_hdr->osd_hdr_wpl<<3) * osd_hdr->osd_hdr_h_mem;
					l_bmp_addr -= (bmp_len>>1); r_bmp_addr -= (bmp_len>>1);
				}
			}
			else /* T/B */
			{
				r_bmp_addr += ((osd_hdr->osd_hdr_w_mem * (osd_hdr->osd_hdr_h_mem>>1) * osd_exhdr->bits_per_pixel)>>3);

				if ( b_reverse_en )
				{
					UINT32 bmp_len = (osd_hdr->osd_hdr_wpl<<3) * osd_hdr->osd_hdr_h_mem;
					l_bmp_addr -= (bmp_len); r_bmp_addr -= (bmp_len);
				}
			}
		}

		FBDEV_DEBUG("fb(%d) exhdr info. s3d[%d] 2d3d[%d] rev[%d] disp L[0x%x] R[0x%x], mif L[0x%08x] R[0x%08x]\n",
			fb_dev_id, b_s3d_en, b_2d3d_en, b_reverse_en, disp_val_l, disp_val_r, l_bmp_addr, r_bmp_addr );

		switch( fb_dev_id )
		{
			case 0:
				OSD_H14_RdFL( osd0_ctrl_3d_disparity );
				OSD_H14_Rd02( osd0_ctrl_3d_disparity,   osd0_l_3d_disparity_value,  val0,
														osd0_r_3d_disparity_value,  val1);
				if ( val0 != disp_val_l || val1 != disp_val_r )
				{
					OSD_H14_Wr02( osd0_ctrl_3d_disparity,   osd0_l_3d_disparity_value,  disp_val_l,
															osd0_r_3d_disparity_value,  disp_val_r );
					OSD_H14_WrFL( osd0_ctrl_3d_disparity );
				}

				OSD_H14_RdFL( osd0_ctrl_scaler );
				if ( b_2d3d_en != OSD_H14_Rd00(osd0_ctrl_scaler,osd0_sc_vdouble) )
				{
					OSD_H14_Wr01( osd0_ctrl_scaler, osd0_sc_vdouble, b_2d3d_en );
					OSD_H14_WrFL( osd0_ctrl_scaler );
				}

				OSD_H14_RdFL( osd0_ctrl_main );
				if ( b_s3d_en != OSD_H14_Rd00(osd0_ctrl_main,osd0_s3d_en) )
				{
					OSD_H14_Wr01( osd0_ctrl_main, osd0_s3d_en, b_s3d_en );
					OSD_H14_WrFL( osd0_ctrl_main );
				}

				if ( b_s3d_en )
				{
					MIF_OSD_H14_Wr01( mifo_saddr_osd_0_l, cmd_rd_saddr_osd_0_l, l_bmp_addr );
					MIF_OSD_H14_Wr01( mifo_saddr_osd_0_r, cmd_rd_saddr_osd_0_r, r_bmp_addr );
					MIF_OSD_H14_WrFL( mifo_saddr_osd_0_l );
					MIF_OSD_H14_WrFL( mifo_saddr_osd_0_r );

					MIF_OSD_H14_RdFL( mifo_rd_ctrl );
					MIF_OSD_H14_Wr01( mifo_rd_ctrl, load_rd_cmd_saddr_v, 0x1 );
					MIF_OSD_H14_WrFL( mifo_rd_ctrl );
				}
			break;

			case 1:
				OSD_H14_RdFL( osd1_ctrl_3d_disparity );
				OSD_H14_Rd02( osd1_ctrl_3d_disparity,	osd1_l_3d_disparity_value,  val0,
														osd1_r_3d_disparity_value,	val1 );
				if ( val0 != disp_val_l || val1 != disp_val_r )
				{
					OSD_H14_Wr02( osd1_ctrl_3d_disparity,	osd1_l_3d_disparity_value,  disp_val_l,
															osd1_r_3d_disparity_value,	disp_val_r );
					OSD_H14_WrFL( osd1_ctrl_3d_disparity );
				}

				OSD_H14_RdFL( osd1_ctrl_scaler );
				if ( b_2d3d_en != OSD_H14_Rd00(osd1_ctrl_scaler, osd1_sc_vdouble) )
				{
					OSD_H14_Wr01( osd1_ctrl_scaler, osd1_sc_vdouble, b_2d3d_en );
					OSD_H14_WrFL( osd1_ctrl_scaler );
				}

				OSD_H14_RdFL( osd1_ctrl_main );
				if ( b_s3d_en != OSD_H14_Rd00( osd1_ctrl_main, osd1_s3d_en) )
				{
					OSD_H14_Wr01( osd1_ctrl_main, osd1_s3d_en, b_s3d_en );
					OSD_H14_WrFL( osd1_ctrl_main );
				}

				if ( b_s3d_en )
				{
					MIF_OSD_H14_Wr01( mifo_saddr_osd_1_l, cmd_rd_saddr_osd_1_l, l_bmp_addr );
					MIF_OSD_H14_Wr01( mifo_saddr_osd_1_r, cmd_rd_saddr_osd_1_r, r_bmp_addr );
					MIF_OSD_H14_WrFL( mifo_saddr_osd_1_l );
					MIF_OSD_H14_WrFL( mifo_saddr_osd_1_r );

					MIF_OSD_H14_RdFL( mifo_rd_ctrl );
					MIF_OSD_H14_Wr01( mifo_rd_ctrl, load_rd_cmd_saddr_v, 0x1 );
					MIF_OSD_H14_WrFL( mifo_rd_ctrl );
				}
			break;

			case 2: case 3: default:
				/* do nothing */
			break;
		}
	}
	spin_unlock_irqrestore(&g_osd_hdr_spinlock, flags);
}

/*========================================================================================
    Implementation Group (FRC)
========================================================================================*/
void FBDEV_H14CMN_WriteFRCHeader( void )
{
	/* make the initial FRC header. this is processed only once at the first call */
	if ( g_fbdev_h14_hw_ctx.frc_if_ctrl.b_hdr_ready == FALSE )
	{
#ifdef FBDEV_USE_FRC_IF_CTRL
		UINT32				val;
	    FBDEV_H14_FRC_HDR_0  osd_frc_hdr_0;   // 0x1ba4 : ''
		FBDEV_H14_FRC_HDR_1  osd_frc_hdr_1;   // 0x1ba8 : ''
		FBDEV_H14_FRC_HDR_2  osd_frc_hdr_2;   // 0x1bac : ''
		FBDEV_H14_FRC_HDR_3  osd_frc_hdr_3;   // 0x1bb0 : ''
		FBDEV_H14_FRC_HDR_4  osd_frc_hdr_4;   // 0x1bb4 : ''
		FBDEV_H14_FRC_HDR_5  osd_frc_hdr_5;   // 0x1bb8 : ''

		/* enable the 2nd routine if you want to use the cleared memory before setting frc header */
		OSD_H14_RdFL(osd_common_sep_osd_hdr0); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_0,	&val, sizeof(UINT32));
		OSD_H14_RdFL(osd_common_sep_osd_hdr1); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_1,	&val, sizeof(UINT32));
		OSD_H14_RdFL(osd_common_sep_osd_hdr2); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_2,	&val, sizeof(UINT32));
		OSD_H14_RdFL(osd_common_sep_osd_hdr3); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_3,	&val, sizeof(UINT32));
		OSD_H14_RdFL(osd_common_sep_osd_hdr4); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_4,	&val, sizeof(UINT32));
		OSD_H14_RdFL(osd_common_sep_osd_hdr5); val = OSD_H14_Rd(osd_common_sep_osd_hdr0); memcpy( &osd_frc_hdr_5,	&val, sizeof(UINT32));

		/* osd_frc_hdr_0.osdf_frc_hdr_frame_number should be increased at vsync interrupt
	 	 *
		 * hdr0.format
		 *  0001 (AYUV6888), 0010 (ARGB6888), 0101 (AYUV8888), 0100 (ARGB8888)
		 *
		 * hdr0.po (pixel order)
		 *	0001 (AGBR/YUV), 0010 (BGRA/VUYA), 0001 (ABGR/AVUY, 0000: (ARGB/AYUV)
		 *
		 * hdr0.lr (Left/Right)
		 *  1 (R), 0 (L)
		 *
		 * hdr0.3d (3D bitmap indicator) - this value should be set for 2D mode or 3D mode with the same L/R image
		 *  1 (3D), 0 (2d)
		 *
		 * hdr0.header_prefix
		 *  1010 (valid OSD), 0000
		 */
		osd_frc_hdr_0.osd_frc_hdr_format		= 0x4; /* ARGB8888 */
		osd_frc_hdr_0.osd_frc_hdr_po			= 0x0; /* ARGB/AYUV */
		osd_frc_hdr_0.osd_frc_lr				= 0x0;
		osd_frc_hdr_0.osd_frc_hdr_prefix		= 0xA;
		osd_frc_hdr_0.osd_frc_3d				= 0x0; //( g_fbdev_frc_info.trid_ctrl[layer].mode )? 1:0;
		osd_frc_hdr_0.osd_frc_hdr_frame_number	= 0x0; /* fixed */

		osd_frc_hdr_1.osd_frc_hdr_bmp_width		= 1920;
		osd_frc_hdr_1.osd_frc_hdr_bmp_height	= 1080;

#if 0//def	FBDEV_L9B1_VCOM_TEST_SUPPORT
		{
			struct scanvas* hCanvas = &g_dm_ctx->wctx[LX_FBDEV_ID_OSD1]->pixel;
			if ( hCanvas->back_image_en )
			{
               	osd_frc_hdr_1.osd_frc_hdr_bmp_width = 1920;
			}
		}
#endif
		osd_frc_hdr_2.osd_frc_hdr_global_alpha	= 0xff;
		osd_frc_hdr_2.osd_frc_hdr_pe			= 0x1;
		osd_frc_hdr_2.osd_frc_hdr_ge			= 0x0;
		osd_frc_hdr_2.osd_frc_hdr_ce			= 0x0;
		osd_frc_hdr_2.osd_frc_hdr_ou			= 0x1;

		osd_frc_hdr_3.osd_frc_hdr_ck_r			= 0x0;
		osd_frc_hdr_3.osd_frc_hdr_ck_g			= 0x0;
		osd_frc_hdr_3.osd_frc_hdr_ck_b			= 0x0;

		osd_frc_hdr_4.osd_frc_hdr_xoffset		= 0x0;
		osd_frc_hdr_4.osd_frc_hdr_yoffset		= 0x0;

		osd_frc_hdr_5.osd_frc_hdr_disp_width	= 1920;
		osd_frc_hdr_5.osd_frc_hdr_disp_height	= 1080;

		/* save FRC header */
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[0] = *((UINT32*)&osd_frc_hdr_0);
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[1] = *((UINT32*)&osd_frc_hdr_1);
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[2] = *((UINT32*)&osd_frc_hdr_2);
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[3] = *((UINT32*)&osd_frc_hdr_3);
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[4] = *((UINT32*)&osd_frc_hdr_4);
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data[5] = *((UINT32*)&osd_frc_hdr_5);

		/* save remapped header */
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[0] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_0) );
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[1] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_1) );
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[2] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_2) );
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[3] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_3) );
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[4] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_4) );
		g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[5] = FBDEV_H14CMN_RemapFRCHeader( *((UINT32*)&osd_frc_hdr_5) );

		/* TODO: dump header if you need */

		/* remap(swap bit order) should be done before writing register */
		OSD_H14_Wr( osd_common_sep_osd_hdr0, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[0] );
		OSD_H14_Wr( osd_common_sep_osd_hdr1, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[1] );
		OSD_H14_Wr( osd_common_sep_osd_hdr2, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[2] );
		OSD_H14_Wr( osd_common_sep_osd_hdr3, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[3] );
		OSD_H14_Wr( osd_common_sep_osd_hdr4, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[4] );
		OSD_H14_Wr( osd_common_sep_osd_hdr5, g_fbdev_h14_hw_ctx.frc_if_ctrl.hdr_data_remap[5] );

		OSD_H14_WrFL( osd_common_sep_osd_hdr0 );
		OSD_H14_WrFL( osd_common_sep_osd_hdr1 );
		OSD_H14_WrFL( osd_common_sep_osd_hdr2 );
		OSD_H14_WrFL( osd_common_sep_osd_hdr3 );
		OSD_H14_WrFL( osd_common_sep_osd_hdr4 );
		OSD_H14_WrFL( osd_common_sep_osd_hdr5 );
#else
		/* raxis.lim (2012/06/06) -- Not Ready. check if FRC header should be generated or not !!! */
		FBDEV_INFO("FRC IF disabled\n");
#endif
		g_fbdev_h14_hw_ctx.frc_if_ctrl.b_hdr_ready = TRUE;
	}
}

/** remapp FRC header data format
 *
 */
static UINT32	FBDEV_H14CMN_RemapFRCHeader ( UINT32 in )
{
	typedef struct
	{
		UINT32		blk00:8,
					__r00:2,
					blk10:6,
					blk11:2,
					__r01:2,
					blk20:4,
					blk21:4,
					__r02:2,
					__r03:2;
	}
	REMAP_BEFORE_T;

	typedef struct
	{
		UINT32		blk00:8,
					blk10:6,
					blk11:2,
					blk20:4,
					blk21:4,

					__r03:2,
					__r00:2,
					__r01:2,
					__r02:2;
	}
	REMAP_AFTER_T;

	UINT32	out;
	REMAP_BEFORE_T	remap0;
	REMAP_AFTER_T	remap1;

	memcpy(&remap0, &in, 4 );

	remap1.blk00 = remap0.blk00;
	remap1.blk10 = remap0.blk10;
	remap1.blk11 = remap0.blk11;
	remap1.blk20 = remap0.blk20;
	remap1.blk21 = remap0.blk21;
	remap1.__r00 = remap0.__r00;
	remap1.__r01 = remap0.__r01;
	remap1.__r02 = remap0.__r02;
	remap1.__r03 = remap0.__r03;

	memcpy(&out, &remap1, 4 );

	return out;
}

/** OSD HW Layer enable/disable
 *
 * @param	int layer : osd layer ,
 *		enable : 0 : display off , 1 : display on
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_H14CMN_SetOSDEnable(int layer, int enable)
{
	UINT32 val = (enable)? 0x1: 0x0;
	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

	FBDEV_CHECK_CODE( layer<0 || layer>=LX_FBDEV_ID_MAX, return RET_ERROR, "invalid layer %d\n", layer );

	FBDEV_DEBUG("fb(%d) <%s>\n", layer, (enable)?"on":"off");
	FBDEV_CHECK_CODE( g_fbdev_cfg->uhd_disp_mode && layer==LX_FBDEV_ID_OSD1, goto func_exit, "UHD mode: POSD1 disabled\n");

	/* save the last on/off status for osd sw compoitor */
	comp_ctrl->osd_view_status[layer] = val;

	/* if osd compositor enabled, DO NOT chnage HW settings.
	 * the real composition is done in FBDEV_H14_SetOSDEnable.
	 */
	FBDEV_CHECK_DEBUG( comp_ctrl->comp_opt != FB_COMP_OPT_NONE, return RET_OK, "osd merge enabled.\n");

	switch(layer)
	{
		case LX_FBDEV_ID_OSD0:	g_fbdev_h14_reg->osd0_ctrl_main.osd0_en = val; break;
		case LX_FBDEV_ID_OSD1:	g_fbdev_h14_reg->osd1_ctrl_main.osd1_en = val; break;
		case LX_FBDEV_ID_OSD2:	g_fbdev_h14_reg->osd2_ctrl_main.osd2_en = val; break;
		case LX_FBDEV_ID_CSR0:	g_fbdev_h14_reg->osd3_ctrl_main.osd3_en = val; break;
	}

func_exit:
	return RET_OK;
}

/** OSD HW Layer status
 *
 * @param	void
 * @return	int 1 : ON , 0 : OFF
 *
 */
int FBDEV_H14CMN_GetOSDEnable(int layer)
{
	int ret = 0;
	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

	/* if osd compositor enabled, just return saved view status */
	FBDEV_CHECK_DEBUG( comp_ctrl->comp_opt != FB_COMP_OPT_NONE, return comp_ctrl->osd_view_status[layer], "osd merge enabled\n");

	switch(layer)
	{
		case LX_FBDEV_ID_OSD0:	ret = g_fbdev_h14_reg->osd0_ctrl_main.osd0_en;	break;
		case LX_FBDEV_ID_OSD1:	ret = g_fbdev_h14_reg->osd1_ctrl_main.osd1_en; break;
		case LX_FBDEV_ID_OSD2:	ret = g_fbdev_h14_reg->osd2_ctrl_main.osd2_en; break;
		case LX_FBDEV_ID_CSR0:	ret = g_fbdev_h14_reg->osd3_ctrl_main.osd3_en; break;
		default: 				FBDEV_WARN("invalid OSD ID %d\n", layer ); ret = -1; break;
	}

	return ret;
}

int FBDEV_H14CMN_SetTridCtrl( int fb_dev_id, LX_FBDEV_TRID_CTRL_T* ctrl )
{
	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(fb_dev_id);
	FBDEV_WriteOSDHeader( &wctx->pixel );
	return RET_OK;
}

int		FBDEV_H14CMN_SetViewCtrl ( LX_FBDEV_VIEW_CTRL_T* ctrl )
{
    int i;
    ULONG flags;

	/* we can't do spinlock when pivot mode is enabled because sleep ocurrs during GFX merge */
	spin_lock_irqsave(&g_osd_hdr_spinlock, flags);

    for (i=0; i<LX_FBDEV_ID_MAX; i++ )
    {
		if ( ctrl->view_status[i] == 0x0 || ctrl->view_status[i] == 0x1 )
			FBDEV_H14CMN_SetOSDEnable( i, ctrl->view_status[i] );
    }

	spin_unlock_irqrestore(&g_osd_hdr_spinlock, flags);

    return RET_OK;
}

/** set OSD out connection ctrl
 *
 *	@param pConnCtrl [IN] connection control data
 *	@return RET_OK if success, RET_ERROR otherwise
 *
 */
int     FBDEV_H14CMN_SetConnCtrl           (LX_FBDEV_CONN_CTRL_T* pConnCtrl)
{
	int		i;
	BOOLEAN	osd_view_status[LX_FBDEV_ID_MAX];

	FBDEV_WARN("OSD conn setup might make system unstable. conn_type 0x%x\n",	pConnCtrl->conn_type );

	/* diable OSD before changing OSD config */
	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		osd_view_status[i] = FBDEV_H14_GetOSDEnable(i);
		FBDEV_H14_SetOSDEnable( i, FALSE );
	}
#if 0
    /* stop DE_TOP::DED_CTRL::DED_PIC_INIT */
	OS_WrReg ( 0xc0035000, 0x00000000 );
#endif

	/* sleep until OSD is stablized */
	OS_MsecSleep(50);

	if ( pConnCtrl->conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	{
		UINT32	val;

		FBDEV_DEBUG("H14::UD_OSD::BLEND mode\n");
#if 0
		OS_WrReg( H14_CTOP_REG_BASE+0x5c, (1<<19) );
		OS_MsecSleep(1000);
#endif
		//OS_WrReg( H14_CTOP_REG_BASE+0x60 /* 0xc001c060 */, 0x00000003 ); /* change CTOP clock */
		OS_WrReg( H14_CTOP_REG_BASE+0x60 /* 0xc001c060 */, 0xe0000000 ); /* change CTOP clock */

		OS_MsecSleep(100);
#if 0
		OS_WrReg( H14_CTOP_REG_BASE+0x5c, 0x00000000 );
#endif

#if 1
		OS_WrReg( H14_CTOP_REG_BASE+0x1bc, 0x00ffffff );/* LVDS power On */
		FBDEV_DEBUG("H14::UD_OSD::BLEND - LVDS POWER ON\n");

		OS_WrReg( 0xc0049000+0x00, 0x01780004 );		/* TCON setting */
		FBDEV_DEBUG("H14::UD_OSD::BLEND - TCON SET\n");

//		OS_WrReg( 0xc0049000+0x04, 0x00780004 );		/* TCON setting */
//		FBDEV_DEBUG("H14::UD_OSD::BLEND - TCON SET\n");

		//OS_WrReg( 0xc0049000+0x18, 0x0000b004 );		/* LVDS CH setting : Ch.A=Video, Ch.B=OSD */
		OS_WrReg( 0xc0049000+0x18, 0x0000b204 );		/* LVDS CH setting : Ch.A=Video, Ch.B=OSD */
		FBDEV_DEBUG("H14::UD_OSD::BLEND - LVDS CH SET\n");
#endif

		OS_WrReg( 0xc0035000+0x0c, 0x00000000 );		/* DPPA data path ctrl */
		OS_WrReg( 0xc0035000+0x10, 0x00000000 );		/* DPPA sync path ctrl */
		FBDEV_DEBUG("H14::UD_OSD::BLEND - DPPA DATA SET\n");

		OS_RdReg( 0xc0035000+0xd8, &val );				/* OSD SYNC GEN ctrl */
		val &= ~(1<<16);
		OS_WrReg( 0xc0035000+0xd8, val );
		FBDEV_DEBUG("H14::UD_OSD::BLEND - OSD SYNC GEN\n");

		OSD_H14_RdFL( osd_common_sep_osd_ctrl );		/* OSD S.OSD HDR disable */
		OSD_H14_Wr01( osd_common_sep_osd_ctrl, reg_common_sep_osd_hdr_en, 0x0 );
		OSD_H14_WrFL( osd_common_sep_osd_ctrl );
		FBDEV_DEBUG("H14::UD_OSD::BLEND - S.OSD HDR OFF\n");

		OSD_H14_RdFL( osd_mixer_ctrl_blend );			/* OSD MIXER PATH ctrl */
		OSD_H14_Wr( osd_mixer_ctrl_blend, 0x00000000 );
		OSD_H14_WrFL( osd_mixer_ctrl_blend );
		FBDEV_DEBUG("H14::UD_OSD::BLEND - OSD MIXER SET\n");
	}
	else
	{
		FBDEV_WARN("invalid connection 0x%x\n", pConnCtrl->conn_type );
		goto func_exit;
	}

	/* rereiniailize OSD HW. DE::PIC_INIT is also restarted here */
	FBDEV_H14_InitHW();
	FBDEV_SetInterruptEnable(MIXER_INTR,1);

	FBDEV_H14_InitOSDLayer(0);
	FBDEV_H14_InitOSDLayer(1);
	FBDEV_H14_InitOSDLayer(3);

	FBDEV_SetOSDImageEndian( 0, g_dm_ctx->wctx[0]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 1, g_dm_ctx->wctx[1]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 3, g_dm_ctx->wctx[3]->pixel.bits_per_pixel );

	OSD_H14_RdFL( osd_common_disp_size );
   	OSD_H14_Wr02( osd_common_disp_size, reg_common_disp_size_w, 1920,   /* 1920 */
                                   		reg_common_disp_size_h, 1080 ); /* 1080 */
	OSD_H14_WrFL( osd_common_disp_size );

//	FBDEV_WriteOSDHeader( &g_dm_ctx->wctx[0]->pixel );
//	FBDEV_WriteOSDHeader( &g_dm_ctx->wctx[1]->pixel );
//	FBDEV_WriteOSDHeader( &g_dm_ctx->wctx[3]->pixel );

func_exit:
	/* diable OSD before changing OSD config */
	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		FBDEV_H14_SetOSDEnable( i, osd_view_status[i] );
	}

	/* sleep until OSD is stablized */
	return RET_OK;
}

/*========================================================================================
    Implementation Group (OSD merge ctrl)
========================================================================================*/
static void FBDEV_H14CMN_ExitOSDMerge (void)
{
	int	i;
	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

	FBDEV_CHECK_CODE( comp_ctrl->comp_opt == FB_COMP_OPT_NONE, return, "ignore dup calls\n");

	GFX_LockDevice();

	if (comp_ctrl->merge_surf >=0 ) {  GFX_FreeSurface(comp_ctrl->merge_surf); comp_ctrl->merge_surf = -1; }

	for (i=0; i<FB_COMP_OUT_FB_NUM; i++)
	{
		if (comp_ctrl->out_fb_surf[i] >= 0)
		{
			GFX_FreeSurface(comp_ctrl->out_fb_surf[i]);
			comp_ctrl->out_fb_surf[i] = -1;
			comp_ctrl->out_fb_mem[i] = 0x0;
		}
	}

	GFX_UnlockDevice();
}

static void FBDEV_H14CMN_InitOSDMerge (void)
{
	int	i;
	int ret=RET_ERROR;
	LX_GFX_SURFACE_SETTING_T surf_info;
	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

	if (comp_ctrl->comp_opt==FB_COMP_OPT_ROTATE_90 || comp_ctrl->comp_opt==FB_COMP_OPT_ROTATE_270)
	{
		comp_ctrl->width = 1080;	/* FIXME */
		comp_ctrl->height= 608;
	}
	else
	{
		comp_ctrl->width = 1920;	/* FIXME */
		comp_ctrl->height= 1080;
	}

	comp_ctrl->out_width = g_fbdev_cfg->display_width;	/* 1920 */
	comp_ctrl->out_height= g_fbdev_cfg->display_height;	/* 1080 */
	comp_ctrl->my_vsync  = -1;
	comp_ctrl->fb_dev_id = LX_FBDEV_ID_OSD0;			/* FIXME : is it necessary to config fb_dev_id */
	comp_ctrl->view_idx  = 0;

	GFX_LockDevice();

	{
		int surf_fd = GFX_AllocARGBSurface(comp_ctrl->width, comp_ctrl->height, 0x0);
		FBDEV_CHECK_CODE(surf_fd<0, goto func_exit, "can't alloc %dx%d surf\n", comp_ctrl->width, comp_ctrl->height );

		comp_ctrl->merge_surf = surf_fd;
	}

	for (i=0; i<FB_COMP_OUT_FB_NUM; i++)
	{
		if (comp_ctrl->out_fb_surf[i] < 0)
		{
			int surf_fd = -1;

			surf_fd = GFX_AllocARGBSurface(comp_ctrl->out_width, comp_ctrl->out_height, 0x0); /* pivot mode !! */
			FBDEV_CHECK_CODE(surf_fd<0, goto func_exit, "can't alloc %dx%d surf\n", comp_ctrl->out_width, comp_ctrl->out_height );

			GFX_GetSurfaceInfo(surf_fd, &surf_info);

			comp_ctrl->out_fb_surf[i] = surf_fd;
			comp_ctrl->out_fb_mem[i]  = (UINT32)surf_info.phys_addr;

			GFX_ClearSurface( surf_fd, comp_ctrl->out_width, comp_ctrl->out_height );

			FBDEV_DEBUG("osd_fb_surf[%d] created. fd=0x%x,sz=%dx%d,mem=0x%x\n", i, surf_fd, comp_ctrl->width, comp_ctrl->height, (UINT32)surf_info.phys_addr );
		}
	}

	FBDEV_DEBUG("comp_ctrl.fb_dev_id = %d\n", comp_ctrl->fb_dev_id);
	FBDEV_DEBUG("comp_ctrl.width,height= %dx%d\n", comp_ctrl->width, comp_ctrl->height);
	FBDEV_DEBUG("comp_ctrl.merge_surf  = 0x%x\n", comp_ctrl->merge_surf);
	FBDEV_DEBUG("comp_ctrl.out_fb_surf0= 0x%x\n", comp_ctrl->out_fb_surf[0] );
	FBDEV_DEBUG("comp_ctrl.out_fb_surf1= 0x%x\n", comp_ctrl->out_fb_surf[1] );

	ret = RET_OK; /* all work done */
func_exit:
	GFX_UnlockDevice();

	if (ret!=RET_OK)
	{
		FBDEV_H14CMN_ExitOSDMerge( );
	}
}


/** SW(GFX) based osd compositor
 *
 */
void FBDEV_H14CMN_DoOSDMerge (int fb_dev_id, BOOLEAN b_flip)
{
	int	i;

	int	view_idx;
	UINT32 osd_view_mask = 0x0;

	struct scanvas 		osd_s_canvas[LX_FBDEV_ID_MAX];
	FBDEV_H14_OSD_HDR_T osd_s_hdr[LX_FBDEV_ID_MAX];
	int					osd_s_surf[LX_FBDEV_ID_MAX] = { -1, -1, -1, -1 };	/* surface for app POSD */
	FBDEV_WIN_CTX_T* 	wctx[LX_FBDEV_ID_MAX];

	FBDEV_H14_OSD_HDR_T out_fb_hdr;
	FBDEV_H14_OSD_EXHDR_T out_fb_exhdr;
	int 	out_fb_surf= -1;
	UINT32	out_fb_mem = 0x0;

	int		osd_t_surf = -1;	/* temp surface */

	int		merge_crsr_surf = -1;	/* cursor */
	int		cx,cy,cw,ch;			/* cursor */
	int		dx,dy,dw,dh;			/* cursor */

	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

#if 0
	/* DO NOT draw cursor, if someone already equested osd merge */
	FBDEV_CHECK_CODE( fb_dev_id == LX_FBDEV_ID_CSR0 && comp_ctrl->my_vsync >= 0, return, "crsr..device busy...\n");
#endif
	OS_LockMutex(&comp_ctrl->mtx);
	GFX_LockDevice();

	FBDEV_DEBUG("req_fb(%d) OSD compisotor. opt=0x%x\n", fb_dev_id, comp_ctrl->comp_opt );

	if (b_flip) comp_ctrl->my_vsync = -1;	/* init my vsync for osd swap */

	view_idx = comp_ctrl->view_idx;

	for (i=0; i<LX_FBDEV_ID_MAX; i++)
	{
		wctx[i] = FBDEV_GetWinCtx(i);

		osd_s_canvas[i] = wctx[i]->pixel;
		osd_view_mask |= (comp_ctrl->osd_view_status[i]) ? (1<<i) : 0x0;

		memcpy( &osd_s_hdr[i], &g_fbdev_h14_hw_ctx.osd_hdr_sav[i], sizeof(FBDEV_H14_OSD_HDR_T));
	}

#if 0
	/* abort if render target is not visible */
	if (fb_dev_id==0 && comp_ctrl->osd_view_status[0]==FALSE) { FBDEV_DEBUG("abort osd merge. fb(%d) hidden\n", fb_dev_id); goto func_exit; }
	if (fb_dev_id==1 && comp_ctrl->osd_view_status[1]==FALSE) { FBDEV_DEBUG("abort osd merge. fb(%d) hidden\n", fb_dev_id); goto func_exit; }
	if (fb_dev_id==3 && comp_ctrl->osd_view_status[3]==FALSE) { FBDEV_DEBUG("abort osd merge. fb(%d) hidden\n", fb_dev_id); goto func_exit; }
#endif
	memcpy(&out_fb_hdr, &g_fbdev_h14_hw_ctx.osd_hdr_sav[ comp_ctrl->fb_dev_id ], sizeof(FBDEV_H14_OSD_HDR_T));
	memset(&out_fb_hdr, 0x0, sizeof(FBDEV_H14_OSD_EXHDR_T));

	/* select next active frame buffer */
	out_fb_surf = comp_ctrl->out_fb_surf[view_idx];
	out_fb_mem  = comp_ctrl->out_fb_mem[view_idx];

	for (i=0;i<LX_FBDEV_ID_MAX;i++)
	{
		osd_s_surf[i] = -1;
		if( FALSE==g_fbdev_cfg->fb[fb_dev_id].b_active ) continue;
		if( FALSE==comp_ctrl->osd_view_status[i]) continue;
		if( 0==osd_s_hdr[i].osd_hdr_w_mem || 0==osd_s_hdr[i].osd_hdr_h_mem ) continue;

		osd_s_surf[i] = GFX_AllocARGBSurfaceEx( osd_s_hdr[i].osd_hdr_w_mem, osd_s_hdr[i].osd_hdr_h_mem, osd_s_hdr[i].osd_hdr_ptr_bmp, osd_s_hdr[i].osd_hdr_wpl<<3 );
		FBDEV_CHECK_CODE( osd_s_surf[i]<0, goto func_exit, "can't alloc osd src surf %dx%d paddr 0x%x\n",
			osd_s_hdr[i].osd_hdr_w_mem, osd_s_hdr[i].osd_hdr_h_mem, osd_s_hdr[i].osd_hdr_ptr_bmp );

		FBDEV_DEBUG("osd_s_surf[%d] 0x%x, %dx%d, paddr 0x%x\n", i, osd_s_surf[i], osd_s_hdr[i].osd_hdr_w_mem, osd_s_hdr[i].osd_hdr_h_mem, osd_s_hdr[i].osd_hdr_ptr_bmp );
	}

	/* process cursor first to avoid the unnecessary cursor update */
	if ( osd_view_mask & 0x8 /* 1<<3 */ )
	{
		BOOLEAN	b_crsr_ok = FALSE;

		cx = osd_s_hdr[3].osd_hdr_xpos;
		cy = osd_s_hdr[3].osd_hdr_ypos;
		cw = osd_s_hdr[3].osd_hdr_w_mem;
		ch = osd_s_hdr[3].osd_hdr_h_mem;

		dx = cx * comp_ctrl->width / 1920;	/* FIXME */
		dy = cy * comp_ctrl->height/ 1080;	/* FIXME */
		dw = cw * comp_ctrl->width / 1920;	/* FIXME */
		dh = ch * comp_ctrl->height/ 1080;	/* FIXME */

		FBDEV_CHECK_CODE( cw==0 || ch==0, goto crsr_render_done, "crsr image not ready\n");
		FBDEV_DEBUG("cxywh = %d,%d,%d,%d, dxywh = %d,%d,%d,%d\n", cx, cy, cw, ch, dx, dy, dw, dh);

		merge_crsr_surf = GFX_AllocARGBSurface( cw, ch, 0x0 );
		FBDEV_CHECK_CODE(merge_crsr_surf<0, goto func_exit, "can't alloc crsr surf %dx%d\n", cw, ch );
		GFX_StretchSurface(osd_s_surf[3], merge_crsr_surf, 0, 0, cw, ch, 0, 0, dw, dh);

		if ((dx+dw) > comp_ctrl->width)  dw = comp_ctrl->width - dx;
		if ((dy+dh) > comp_ctrl->height) dh = comp_ctrl->height- dy;

		/* if cursor position too far at right/bottom cornder, ignore update becuase cursor should not be visible */
		FBDEV_CHECK_DEBUG( dw<2 || dh<2, goto crsr_render_done, "tgt crsr image is at right/bottom corder. ignore update..\n");

		b_crsr_ok = TRUE;	/* cursor render ok */
crsr_render_done:
		FBDEV_CHECK_CODE( b_crsr_ok != TRUE && fb_dev_id == LX_FBDEV_ID_CSR0, goto func_exit, "abort osd merge by crsor\n");
	}

	/* create temporary surface to merge */
	osd_t_surf = GFX_AllocARGBSurface( comp_ctrl->width, comp_ctrl->height, 0x0 );
	FBDEV_CHECK_CODE( osd_t_surf<0, goto func_exit, "can't alloc temp surf %dx%d\n", comp_ctrl->width, comp_ctrl->height );

	/* make merged image based on OSD0&1 */
	switch( osd_view_mask & 0x3 )
	{
		case 0x1: /* OSD0 only */
			FBDEV_DEBUG("POSD0 only merge\n");
			GFX_StretchSurface( osd_s_surf[0], comp_ctrl->merge_surf, 0, 0, osd_s_hdr[0].osd_hdr_w_mem, osd_s_hdr[0].osd_hdr_h_mem, 0, 0, comp_ctrl->width, comp_ctrl->height );
			break;

		case 0x2: /* OSD1 only */
			FBDEV_DEBUG("POSD1 only merge\n");
			GFX_StretchSurface( osd_s_surf[1], comp_ctrl->merge_surf, 0, 0, osd_s_hdr[1].osd_hdr_w_mem, osd_s_hdr[1].osd_hdr_h_mem, 0, 0, comp_ctrl->width, comp_ctrl->height );
			break;

		case 0x3: /* OSD1 & OSD2 */
			FBDEV_DEBUG("POSD0 & POSD1 merge\n");
			GFX_StretchSurface( osd_s_surf[1], comp_ctrl->merge_surf, 0, 0, osd_s_hdr[1].osd_hdr_w_mem, osd_s_hdr[1].osd_hdr_h_mem, 0, 0, comp_ctrl->width, comp_ctrl->height );
			GFX_StretchSurface( osd_s_surf[0], osd_t_surf, 0, 0, osd_s_hdr[0].osd_hdr_w_mem, osd_s_hdr[0].osd_hdr_h_mem, 0, 0, comp_ctrl->width, comp_ctrl->height );
			GFX_BlitSurface( osd_t_surf, comp_ctrl->merge_surf, comp_ctrl->merge_surf, 0, 0, comp_ctrl->width, comp_ctrl->height, 0, 0, TRUE );
			break;

		default:  /* clear surface */
			FBDEV_DEBUG("CRSR only merge\n");
			GFX_ClearSurface( comp_ctrl->merge_surf, comp_ctrl->width, comp_ctrl->height );
			break;
	}
	FBDEV_DEBUG("GFX OSD merge done..\n");

	/* blit cursor if cursor is shown */
	if ( osd_view_mask & 0x8 /* 1<<3 */ )
	{
		if( merge_crsr_surf>=0) GFX_BlitSurface(merge_crsr_surf, comp_ctrl->merge_surf, comp_ctrl->merge_surf, 0, 0, dw, dh, dx, dy, TRUE);
	}

	FBDEV_DEBUG("rotate src:0x%x, dst:0x%x\n", comp_ctrl->merge_surf, out_fb_surf );
	GFX_RotateSurface( comp_ctrl->merge_surf, out_fb_surf, 0, 0, comp_ctrl->width, comp_ctrl->height,
						(comp_ctrl->out_width - comp_ctrl->height)/2,
						(comp_ctrl->out_height- comp_ctrl->width)/2,
						(comp_ctrl->comp_opt & FB_COMP_OPT_ROTATE_270) ? 270:
						(comp_ctrl->comp_opt & FB_COMP_OPT_ROTATE_90 ) ?  90: 0 );


	out_fb_hdr.osd_hdr_w_out= comp_ctrl->out_width;
	out_fb_hdr.osd_hdr_h_out= comp_ctrl->out_height;
	out_fb_hdr.osd_hdr_w_out= comp_ctrl->out_width;
	out_fb_hdr.osd_hdr_h_out= comp_ctrl->out_height;
	out_fb_hdr.osd_hdr_xpos = 0;
	out_fb_hdr.osd_hdr_ypos	= 0;
	out_fb_hdr.osd_hdr_wpl	= (comp_ctrl->out_width<<2)>>3;
	out_fb_hdr.osd_hdr_ptr_bmp = (UINT32)out_fb_mem;

	out_fb_exhdr.bits_per_pixel = 32; /* 32bpp fixed */

	FBDEV_DEBUG("fb(%d) writing OSD hdr %dx%d\n", comp_ctrl->fb_dev_id, out_fb_hdr.osd_hdr_w_out,out_fb_hdr.osd_hdr_h_out );
	FBDEV_H14CMN_WriteHdrRegs	( comp_ctrl->fb_dev_id, &out_fb_hdr, &out_fb_exhdr );

	if (b_flip)
	{
		comp_ctrl->my_vsync = g_fbdev_vsync_cnt;
		comp_ctrl->view_idx  = (view_idx+1)%FB_COMP_OUT_FB_NUM;
	}

func_exit:
	for (i=0; i<LX_FBDEV_ID_MAX; i++)
	{
		if (osd_s_surf[i]>=0) GFX_FreeSurface(osd_s_surf[i]);
	}

	if (osd_t_surf>=0) GFX_FreeSurface(osd_t_surf);
	if(merge_crsr_surf>=0) GFX_FreeSurface(merge_crsr_surf);

	GFX_UnlockDevice();
	OS_UnlockMutex(&comp_ctrl->mtx);
}

/*========================================================================================
    Implementation Group (PIVOT emulation)
========================================================================================*/
int		FBDEV_H14CMN_SetPivotCtrl	( LX_FBDEV_PIVOT_CTRL_T* ctrl )
{
	FBDEV_WIN_CTX_T* wctx0 = FBDEV_GetWinCtx(0);
	FBDEV_WIN_CTX_T* wctx1 = FBDEV_GetWinCtx(1);
	FBDEV_H14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_h14_hw_ctx.comp_ctrl;

	FBDEV_DEBUG("pivot ctrl. mode %d\n", ctrl->mode );

	comp_ctrl->comp_opt = (ctrl->mode==LX_FBDEV_PIVOT_MODE_90_CW)? FB_COMP_OPT_ROTATE_90:
						  (ctrl->mode==LX_FBDEV_PIVOT_MODE_270_CW)? FB_COMP_OPT_ROTATE_270:LX_FBDEV_PIVOT_MODE_NONE;

	/* for simple implementation, clear all compositor resources for pivot state change */
	FBDEV_H14CMN_ExitOSDMerge( );

	if ( ctrl->mode != LX_FBDEV_PIVOT_MODE_NONE )
	{
		FBDEV_H14CMN_InitOSDMerge( );

		/* enable only osd merged layer */
		g_fbdev_h14_reg->osd0_ctrl_main.osd0_en = (comp_ctrl->fb_dev_id==0)?0x1:0x0;
		g_fbdev_h14_reg->osd1_ctrl_main.osd1_en = (comp_ctrl->fb_dev_id==1)?0x1:0x0;
		g_fbdev_h14_reg->osd3_ctrl_main.osd3_en = 0;
	}
	else
	{
		/* restore osd on/off status */
		g_fbdev_h14_reg->osd0_ctrl_main.osd0_en = comp_ctrl->osd_view_status[0] ? 0x1: 0x0;
		g_fbdev_h14_reg->osd1_ctrl_main.osd1_en = comp_ctrl->osd_view_status[1] ? 0x1: 0x0;
		g_fbdev_h14_reg->osd3_ctrl_main.osd3_en = comp_ctrl->osd_view_status[3] ? 0x1: 0x0;
	}

	/* rewrite osd header with the real cfg */
	/* FIXME : osd merge is done twice when OSD merge is enabled ? */
	FBDEV_WriteOSDHeader( &wctx0->pixel );
	FBDEV_WriteOSDHeader( &wctx1->pixel );

	return RET_OK;
}

/*========================================================================================
    Implementation Group (VOSD emulation)
========================================================================================*/
void	FBDEV_H14CMN_HandleCLUT ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H14_OSD_HDR_T* osd_hdr )
{
#ifdef FBDEV_SUPPORT_8BPP_EMUL
	FBDEV_H14_CLUT_VOSD_CTRL_T* vosd_ctrl = &g_fbdev_h14_hw_ctx.clut_vosd_ctrl[fb_dev_id];

	if ( hCanvas->bits_per_pixel == 8 )
	{
		LX_GFX_SURFACE_SETTING_T surf_info;

		GFX_LockDevice();

		/* create 8bpp clut surface if needed */
		if ( vosd_ctrl->osd_surf[0] < 0 )
		{
			vosd_ctrl->osd_surf[0] = GFX_AllocARGBSurface( FBDEV_H14_CLUT_VOSD_WIDTH, FBDEV_H14_CLUT_VOSD_HEIGHT, 0x0 );
			FBDEV_CHECK_CODE( vosd_ctrl->osd_surf[0] < 0, goto clut_proc_exit, "can't clut vosd\n");

			GFX_GetSurfaceInfo ( vosd_ctrl->osd_surf[0], &surf_info );
			vosd_ctrl->fb_mem_ptr[0] = (UINT32)surf_info.phys_addr;
		}
		if ( vosd_ctrl->osd_surf[1] < 0 )
		{
			vosd_ctrl->osd_surf[1] = GFX_AllocARGBSurface( FBDEV_H14_CLUT_VOSD_WIDTH, FBDEV_H14_CLUT_VOSD_HEIGHT, 0x0 );
			FBDEV_CHECK_CODE( vosd_ctrl->osd_surf[1] < 0, goto clut_proc_exit, "can't alloc VOSD surf\n");

			GFX_GetSurfaceInfo ( vosd_ctrl->osd_surf[1], &surf_info );
			vosd_ctrl->fb_mem_ptr[1] = (UINT32)surf_info.phys_addr;
		}

		/* convert clut to 32bpp surface */
		{
			int 	fb_width	= (hCanvas->stride << 3)/ hCanvas->bits_per_pixel;
			int		fb_height	= (hCanvas->input_win.h);
			int		vosd_idx	= vosd_ctrl->osd_idx;
			int		vosd_surf	= vosd_ctrl->osd_surf[ vosd_idx ];
			int		vosd_mem_ptr= vosd_ctrl->fb_mem_ptr[ vosd_idx ];
			int		posd_surf	= -1;

			/* raxis.lim (2012/12/11) -- OSD input window can't be larger than physical OSD width (?) */
			if ( hCanvas->input_win.w > fb_width  ) hCanvas->input_win.w = fb_width;
			if ( hCanvas->input_win.h > fb_height ) hCanvas->input_win.h = fb_height;

			posd_surf = GFX_Alloc8BppSurfaceEx( fb_width, fb_height, hCanvas->osd_bm_paddr, hCanvas->stride );
			FBDEV_CHECK_CODE( posd_surf<0, goto clut_proc_exit, "can't create temp FB surf for CLUT\n");

			GFX_SetSurfacePalette( posd_surf, 256, hCanvas->palette );

			/* if real POSD is less than clut vosd region, just blit */
			if ( hCanvas->input_win.w <= FBDEV_H14_CLUT_VOSD_WIDTH && hCanvas->input_win.h <= FBDEV_H14_CLUT_VOSD_HEIGHT )
			{
				GFX_BlitSurface( posd_surf, -1, vosd_surf, 0, 0, hCanvas->input_win.w, hCanvas->input_win.h, 0, 0, FALSE );
			}
			/* if real POSD is larger than clut vosd region, scale down it */
			else
			{
				GFX_StretchSurface( posd_surf, vosd_surf, 0, 0, hCanvas->input_win.w, hCanvas->input_win.h, 0, 0, FBDEV_H14_CLUT_VOSD_WIDTH, FBDEV_H14_CLUT_VOSD_HEIGHT );

				/* scale input dimension */
				if ( hCanvas->input_win.w > FBDEV_H14_CLUT_VOSD_WIDTH )
				{
					hCanvas->input_win.x = (hCanvas->input_win.x*FBDEV_H14_CLUT_VOSD_WIDTH)/fb_width;
					hCanvas->input_win.w = (hCanvas->input_win.w*FBDEV_H14_CLUT_VOSD_WIDTH)/fb_width;
				}
				if ( hCanvas->input_win.h > FBDEV_H14_CLUT_VOSD_HEIGHT )
				{
					hCanvas->input_win.y = (hCanvas->input_win.y*FBDEV_H14_CLUT_VOSD_HEIGHT)/fb_height;
					hCanvas->input_win.h = (hCanvas->input_win.h*FBDEV_H14_CLUT_VOSD_HEIGHT)/fb_height;
				}
			}

			/* change canvas data to hold 32bpp clut vosd */
			hCanvas->stride				= FBDEV_H14_CLUT_VOSD_WIDTH * 4;
			hCanvas->osd_bm_paddr 	= vosd_mem_ptr;
			hCanvas->bits_per_pixel		= 32;

			if ( posd_surf>=0 ) GFX_FreeSurface( posd_surf );

			/* request vsync monitor & swap osd index */
			vosd_ctrl->my_vsync = g_fbdev_vsync_cnt;
#ifndef UPDATE_VOSD_IDX_INSIDE_ISR
			vosd_ctrl->osd_idx ^= 1;
#endif
		}

clut_proc_exit:
		GFX_UnlockDevice();
	}
	else
	{
		if ( vosd_ctrl->osd_surf[0] >= 0 || vosd_ctrl->osd_surf[1] >= 0 )
		{
			GFX_LockDevice();
			if ( vosd_ctrl->osd_surf[0] >= 0 ) { GFX_FreeSurface( vosd_ctrl->osd_surf[0] ); vosd_ctrl->osd_surf[0] = -1; }
			if ( vosd_ctrl->osd_surf[1] >= 0 ) { GFX_FreeSurface( vosd_ctrl->osd_surf[1] ); vosd_ctrl->osd_surf[1] = -1; }
			GFX_UnlockDevice();

			vosd_ctrl->osd_idx  = 0;
			vosd_ctrl->my_vsync = -1;
		}
	}

#endif
}

/** initialize virtual OSD 
 *
 */
static void	FBDEV_H14CMN_InitVOSD	( int fb_dev_id, UINT32 opmask )
{
	FBDEV_H14_VOSD_CTRL_T* vosd_ctrl = &g_fbdev_h14_hw_ctx.vosd_ctrl[fb_dev_id];

	if ( opmask == FBDEV_H14_VOSD_OPMASK_NONE ) return;

	if ( vosd_ctrl->opmask == FBDEV_H14_VOSD_OPMASK_NONE ) 
	{
		LX_GFX_SURFACE_SETTING_T surf_info;
		int width  = g_fbdev_cfg->display_width;	/* maximum width	*/
		int height = g_fbdev_cfg->display_height;	/* maximum height	*/

		vosd_ctrl->osd_surf[0] = GFX_AllocARGBSurface( width, height, 0x0 );
		FBDEV_CHECK_CODE( vosd_ctrl->osd_surf[0] < 0, return, "can't alloc VOSD surf\n");
		vosd_ctrl->osd_surf[1] = GFX_AllocARGBSurface( width, height, 0x0 );
		FBDEV_CHECK_CODE( vosd_ctrl->osd_surf[1] < 0, return, "can't alloc VOSD surf\n");

		GFX_GetSurfaceInfo ( vosd_ctrl->osd_surf[0], &surf_info );
		vosd_ctrl->fb_mem_ptr[0] = (UINT32)surf_info.phys_addr;
		GFX_GetSurfaceInfo ( vosd_ctrl->osd_surf[1], &surf_info );
		vosd_ctrl->fb_mem_ptr[1] = (UINT32)surf_info.phys_addr;

		vosd_ctrl->osd_idx = 0x0;
		memset( &vosd_ctrl->src_rect, 0x0, sizeof(LX_RECT_T));
		memset( &vosd_ctrl->dst_rect, 0x0, sizeof(LX_RECT_T));
	}

	vosd_ctrl->opmask |= opmask;
}

/** shutdown virtual OSD
 *
 */
static void	FBDEV_H14CMN_ExitVOSD	( int fb_dev_id )
{
	FBDEV_H14_VOSD_CTRL_T* vosd_ctrl = &g_fbdev_h14_hw_ctx.vosd_ctrl[fb_dev_id];

	if ( vosd_ctrl->opmask == FBDEV_H14_VOSD_OPMASK_NONE ) return; /* ignore multiple shutdown */

	vosd_ctrl->opmask = FBDEV_H14_VOSD_OPMASK_NONE;

	if ( vosd_ctrl->osd_surf[0] >= 0 ) GFX_FreeSurface( vosd_ctrl->osd_surf[0] );
	if ( vosd_ctrl->osd_surf[1] >= 0 ) GFX_FreeSurface( vosd_ctrl->osd_surf[1] );
}

/** make virtual OSD screen
 *
 *	@param fb_dev_id [IN] OSD id
 *	@param hCanvas [IN] canvas handle
 *	@param osd_hdr [IN/OUT] OSD header 
 */
void	FBDEV_H14CMN_HandleVOSD ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H14_OSD_HDR_T* osd_hdr )
{
	UINT32	opmask = FBDEV_H14_VOSD_OPMASK_NONE;

	/* check if application want to scale down POSD or fade in/out */
	if ( hCanvas->output_win.w < hCanvas->input_win.w || hCanvas->output_win.h < hCanvas->input_win.h )
		opmask |= FBDEV_H14_VOSD_OPMASK_DOWN_SCALE;
	if ( hCanvas->blend_alpha_en && hCanvas->blend_alpha < 0xff )
		opmask |= FBDEV_H14_VOSD_OPMASK_BLEND_ALPHA;

	if ( opmask == FBDEV_H14_VOSD_OPMASK_NONE) 
	{
		FBDEV_H14CMN_ExitVOSD( fb_dev_id );
		return;
	}

	GFX_LockDevice();

	/* initialize VOSD struct based on opmask */
	FBDEV_H14CMN_InitVOSD( fb_dev_id, opmask );

	/* make virtual OSD screen */
	{
		UINT32	vosd_idx;
		UINT32	vosd_mem_ptr;
		int		vosd_surf = -1;
		int		posd_surf = -1;

		FBDEV_H14_VOSD_CTRL_T* vosd_ctrl = &g_fbdev_h14_hw_ctx.vosd_ctrl[fb_dev_id];

		int 		fb_width	= (hCanvas->stride << 3)/ hCanvas->bits_per_pixel;
		int			fb_height	= (hCanvas->input_win.h);
		LX_RECT_T	src_rect	= { 0, 0, hCanvas->input_win.w, hCanvas->input_win.h };
		LX_RECT_T	dst_rect	= { hCanvas->output_win.x, hCanvas->output_win.y, hCanvas->output_win.w, hCanvas->output_win.h };

		vosd_idx	= vosd_ctrl->osd_idx;
		vosd_surf	= vosd_ctrl->osd_surf[ vosd_idx ];
		vosd_mem_ptr= vosd_ctrl->fb_mem_ptr[ vosd_idx ];

		/* create current (input) POSD surface */
		posd_surf= GFX_AllocARGBSurface( fb_width, fb_height, osd_hdr->osd_hdr_ptr_bmp );
		FBDEV_CHECK_CODE( posd_surf<0, goto vosd_exit, "can't create temp FB surf\n");

		osd_hdr->osd_hdr_wpl = ( g_fbdev_cfg->display_width << 2 /* stride */ ) >> 3;

		switch( vosd_ctrl->opmask )
		{
			case FBDEV_H14_VOSD_OPMASK_BLEND_ALPHA:
			{
				GFX_FadeSurface( posd_surf, vosd_surf, dst_rect.w, dst_rect.h, hCanvas->blend_alpha );

				osd_hdr->osd_hdr_w_mem	= dst_rect.w;
				osd_hdr->osd_hdr_h_mem	= dst_rect.h;
			}
			break;

			case FBDEV_H14_VOSD_OPMASK_DOWN_SCALE:
			{
#if 0
				/* accoding to down-scale test, ther seems 2 line screen garbage at the right side in vertical lines.
				 * below codes will be remove these garbage [TODO] check H14
				 */
				osd_hdr->osd_hdr_w_mem  -= 2;   /* hidde right 2 lines in vertical */
				osd_hdr->osd_hdr_w_out  -= 2;   /* hidde right 2 lines in vertical */
#endif
				GFX_StretchSurface( posd_surf, vosd_surf, 0, 0, src_rect.w, src_rect.h, 0, 0, dst_rect.w, dst_rect.h );

				osd_hdr->osd_hdr_w_mem	= dst_rect.w;
				osd_hdr->osd_hdr_h_mem	= dst_rect.h;
			}
			break;

			case FBDEV_H14_VOSD_OPMASK_DOWN_SCALE|FBDEV_H14_VOSD_OPMASK_BLEND_ALPHA:
			default:
			{
#if 0
				/* accoding to down-scale test, ther seems 2 line screen garbage at the right side in vertical lines.
				 * below codes will be remove these garbage [TODO] check H14
				 */
				osd_hdr->osd_hdr_w_mem  -= 2;   /* hidde right 2 lines in vertical */
				osd_hdr->osd_hdr_w_out  -= 2;   /* hidde right 2 lines in vertical */
#endif
				GFX_StretchSurface( posd_surf, vosd_surf, 0, 0, src_rect.w, src_rect.h, 0, 0, dst_rect.w, dst_rect.h );
				GFX_FadeSurface   ( vosd_surf, vosd_surf, dst_rect.w, dst_rect.h, hCanvas->blend_alpha );

				osd_hdr->osd_hdr_w_mem	= dst_rect.w;
				osd_hdr->osd_hdr_h_mem	= dst_rect.h;
			}
			break;
		}

		if ( posd_surf>=0 ) GFX_FreeSurface( posd_surf );

		/* replace old bm to new bm */
		osd_hdr->osd_hdr_ptr_bmp = vosd_mem_ptr;

		/* request vsync monitor & swap osd index */
		vosd_ctrl->my_vsync = g_fbdev_vsync_cnt;
#ifndef UPDATE_VOSD_IDX_INSIDE_ISR
		vosd_ctrl->osd_idx ^= 1;
#endif

#if 0
printk("[fb] %s : POSD info = (0,0,%d,%d) @ %dx%d\n", __F__, src_rect.w, src_rect.h, fb_width, fb_height );
printk("[fb] %s : VOSD info = (%d,%d,%d,%d) \n", __F__, dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h );
printk("[fb] %s : HDR  info = %d,%d,%d,%d -> %d,%d,%d,%d\n", __F__, 0, 0, osd_hdr->osd_hdr_w_mem, osd_hdr->osd_hdr_h_mem,
		            osd_hdr->osd_hdr_xpos, osd_hdr->osd_hdr_ypos ,
           		 	osd_hdr->osd_hdr_w_out, osd_hdr->osd_hdr_h_out );
#endif
	}
vosd_exit:
	GFX_UnlockDevice();
}

/* update S3D status if necessary */
void	FBDEV_H14CMN_Handle3DOSD ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H14_OSD_HDR_T* osd_hdr )
{
	FBDEV_CHECK_DEBUG( lx_chip_plt() != LX_CHIP_PLT_FHD, return, "fb(%d) H14:UHD_OSD - ignore 3D handler\n", fb_dev_id );
#ifdef FBDEV_USE_H14_UHD_DEMO
	return;
#endif

	if ( g_fbdev_hw_ctx.fb[fb_dev_id].trid_ctrl.mode & LX_FBDEV_TRID_MODE_SS )
	{
		/* raxis.lim (2012/12/07)
		 * HW requires osd input height should be less than half of output height.
		 * for example, if output height is 1080, input height should be less than 540 (half of 1080) !!!
		 * H/W makes full OSD screen using two half-height left & right screen.
		 *
		 * So Application should knows this H/W restrtion.
		 */
		if ( osd_hdr->osd_hdr_h_mem > (osd_hdr->osd_hdr_h_out>>1) )
		{
			osd_hdr->osd_hdr_h_mem = osd_hdr->osd_hdr_h_out>>1;
		}

		/* raxis.lim (2012/12/07)
		 * OSD H/W mixes left picture & right picture when S3D(S/S) enabled.
	 	 * OSD H/W requires half of width & double of height.
		 * for example, if you want to enable S3D with 1280x540 POSD, you should make OSD header with input_w 640, input_h 1080.
		 */
		osd_hdr->osd_hdr_w_mem >>= 1;	/* h/2 */
		osd_hdr->osd_hdr_h_mem <<= 1;	/* V*2 */

		FBDEV_PRINT("fb(%d) S3D(S/S) osd_hdr w %d, h %d \n", fb_dev_id, osd_hdr->osd_hdr_w_mem, osd_hdr->osd_hdr_h_mem );
	}
}

/** handle osd scaler workaround during reverse screen
 *
 *	this workaround makes OSD scaler output to be similar to GFX.
 *	so this workaround code should run only when framebuffer is scaled up !!
 *
 */
void	FBDEV_H14CMN_HandleReverseOSD ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H14_OSD_HDR_T* osd_hdr )
{
	BOOLEAN	b_reverse_en;

	/* raxis.lim (2013/10/31) change reverse_screen detection code.
 	 * HW guy (woonghyun.kim) recommended to use mixer_ctrl_sync rather than osd_ctrl_main.
	 * Actually it is normal mixer_ctrl_sync.reg_sync_v_reverse is the same as osd_ctrl_main.osd_v_reverse_en.
	 */
	b_reverse_en = FBDEV_H14CMN_IsReverseOSD(fb_dev_id);

	if (FALSE==b_reverse_en) { FBDEV_PRINT("reverse mode off. return\n"); return; }

	if ( osd_hdr->osd_hdr_h_mem < osd_hdr->osd_hdr_h_out )
	{
		UINT32	v_phase_off;
		FBDEV_PRINT("fb(%d) tune vphase_off 0x%x at reverse screen\n", fb_dev_id, _g_fbdev_h14_v_phase_offset );

	switch ( fb_dev_id )
	{
		case 0:
			OSD_H14_RdFL( osd0_ctrl_scaler );
				OSD_H14_Rd01( osd0_ctrl_scaler, osd0_sc_v_phase_off, v_phase_off );

				if ( v_phase_off != _g_fbdev_h14_v_phase_offset )
				{
			OSD_H14_Wr01( osd0_ctrl_scaler, osd0_sc_v_phase_off, _g_fbdev_h14_v_phase_offset );	
			OSD_H14_WrFL( osd0_ctrl_scaler );
				}
			break;
		case 1:
			OSD_H14_RdFL( osd1_ctrl_scaler );
				OSD_H14_Rd01( osd1_ctrl_scaler, osd1_sc_v_phase_off, v_phase_off );

				if ( v_phase_off != _g_fbdev_h14_v_phase_offset )
				{
			OSD_H14_Wr01( osd1_ctrl_scaler, osd1_sc_v_phase_off, _g_fbdev_h14_v_phase_offset );	
			OSD_H14_WrFL( osd1_ctrl_scaler );
				}
			break;

		case 2:	/* not supported */ break;
		case 3: /* don't do workaround */ break;
	}
}
}

/** VOSD ISR handler 
 *
 *
 */
void     FBDEV_H14CMN_HandleISR ( void )
{
	int		i;

	/* wirte MIF::mrb_bank_mode */
	if ( _g_fbdev_h14_mrb_bank_mode != _g_fbdev_h14_mrb_bank_mode_req )
	{
		printk("[fb] mrb_bank_mode changed from 0x%x to 0x%x\n", _g_fbdev_h14_mrb_bank_mode, _g_fbdev_h14_mrb_bank_mode_req );
		_g_fbdev_h14_mrb_bank_mode = _g_fbdev_h14_mrb_bank_mode_req;

		MIF_OSD_H14_RdFL( mifo_rd_ctrl );
		MIF_OSD_H14_Wr01( mifo_rd_ctrl, mrb_bank_mode, _g_fbdev_h14_mrb_bank_mode );
		MIF_OSD_H14_WrFL( mifo_rd_ctrl );
	}

	for ( i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		FBDEV_H14_VOSD_CTRL_T* vosd_ctrl = &g_fbdev_h14_hw_ctx.vosd_ctrl[i];
		FBDEV_H14_CLUT_VOSD_CTRL_T* clut_vosd_ctrl = &g_fbdev_h14_hw_ctx.clut_vosd_ctrl[i];

		if ( vosd_ctrl->opmask && vosd_ctrl->my_vsync >= 0 && vosd_ctrl->my_vsync != g_fbdev_vsync_cnt )
		{
#ifdef UPDATE_VOSD_IDX_INSIDE_ISR
			vosd_ctrl->osd_idx ^= 1;	/* swap osd_idx */
#endif
			vosd_ctrl->my_vsync = -1;	/* disable vsync monitoring */
		}
		if ( clut_vosd_ctrl->my_vsync >= 0 && clut_vosd_ctrl->my_vsync != g_fbdev_vsync_cnt )
		{
#ifdef UPDATE_VOSD_IDX_INSIDE_ISR
			clut_vosd_ctrl->osd_idx ^= 1;	/* swap osd_idx */
#endif
			clut_vosd_ctrl->my_vsync = -1;	/* disable vsync monitoring */
		}
	}

	if ( g_fbdev_h14_hw_ctx.comp_ctrl.comp_opt != FB_COMP_OPT_NONE )
	{
		if ( g_fbdev_h14_hw_ctx.comp_ctrl.my_vsync >= 0 && g_fbdev_h14_hw_ctx.comp_ctrl.my_vsync != g_fbdev_vsync_cnt )
		{
			g_fbdev_h14_hw_ctx.comp_ctrl.my_vsync = -1;
		}
	}
}

/*========================================================================================
    Implementation Group (Suspend/Resume)
========================================================================================*/
/** common resume/suspend handler 
 *
 */
int	FBDEV_H14CMN_RunResume	( void )
{
	FBDEV_H14_InitHW();
	FBDEV_SetInterruptEnable(MIXER_INTR,1);

	FBDEV_H14_InitOSDLayer(0);
	FBDEV_H14_InitOSDLayer(1);
	FBDEV_H14_InitOSDLayer(3);

	FBDEV_SetOSDImageEndian( 0, g_dm_ctx->wctx[0]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 1, g_dm_ctx->wctx[1]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 3, g_dm_ctx->wctx[3]->pixel.bits_per_pixel );

	OSD_H14_RdFL( osd_common_disp_size );
	OSD_H14_Wr02( osd_common_disp_size, reg_common_disp_size_w, LX_RES_GET_WIDTH(lx_chip_graphic_res()),	/* 1920 */
										reg_common_disp_size_h, LX_RES_GET_HEIGHT(lx_chip_graphic_res()) );	/* 1080 */

	OSD_H14_WrFL( osd_common_disp_size );

	FBDEV_H14CMN_RunBootLogoTimer( 1500, TRUE );

	return RET_OK;
}

int	FBDEV_H14CMN_RunSuspend	( void )
{
	/* do nothing */
	return RET_OK;
}

