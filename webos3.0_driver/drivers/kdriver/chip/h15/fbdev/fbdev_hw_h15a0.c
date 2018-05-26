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

/* @file name : fbdev_hw.c
 * @description : fbdev hardware register control functions
 */
/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_drv.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"
#include "fbdev_hw_h15.h"
#include "fbdev_reg_h15.h"

#include "../sys/ctop_regs.h"
#include <linux/version.h>


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PRINTK(fmt,args...)						//printk(##fmt,args)

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
//extern	volatile OSD_H15_REG_T*		g_fbdev_h15_reg;
//extern	OSD_H15_REG_T*				g_fbdev_h15_reg_cached;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static int		g_osd_csr_crash_fix = 0;	/* H15Ax : cursor HW is lockup up when cursor viewing width is changed */

/*========================================================================================
	Implementation Group
========================================================================================*/

/** initialize H15(A1) HW Ctx, Cfg and all chip dependent variables.
 *
 */
int	FBDEV_H15A0_InitHW ( void )
{
	g_fbdev_frc_ctrl.b_frc_sep_enable	= FALSE;	/* Blending Mode by default */
	g_fbdev_frc_ctrl.b_frc_UD_enable	= FALSE;	/* Standard Mode by default */
	g_fbdev_frc_ctrl.b_frc_hdr_emul		= FALSE;	/* Disable FRC HDR emulation */
	g_fbdev_frc_ctrl.b_frc_osd_emul		= TRUE;
	g_fbdev_frc_ctrl.frc_hdr_emul_dev	= -1;

	/* raxis.lim (2014/09/04) -- H15Ax OSD crashes when cursor is located at the right corder */
	if (lx_chip_rev() < LX_CHIP_REV(H15,B0))
	{
		g_osd_csr_crash_fix = 1;
	}

	return FBDEV_H15CMN_InitHW();
}

/** Set Header information
 *
 * @param	SCanvas : pointer to structure bearing osd header information
 * @return	void
 *
 * @ingroup osd_ddi_func
 */
int FBDEV_H15A0_WriteOSDHeader( struct scanvas *pCanvas )
{
	int				ret;
	int				fb_dev_id;
	UINT32			bitmap_ptr;
	FBDEV_H15_OSD_HDR_T osd_hdr;
	FBDEV_H15_OSD_EXHDR_T osd_exhdr;
	struct scanvas	canvas;

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	UINT32  disp_width = hw_ctx->hw_disp_width;
	UINT32  disp_height= hw_ctx->hw_disp_height;

	/* clear osd_hdr before writing */
	memset( &osd_hdr, 0x0, sizeof(FBDEV_H15_OSD_HDR_T) );
	memset( &osd_exhdr, 0x0, sizeof(FBDEV_H15_OSD_EXHDR_T) );
	memcpy( &canvas, pCanvas, sizeof(struct scanvas) );

	ret = 0; //down_interruptible(&fbdev_sem_h15);

	fb_dev_id	= canvas.fb_dev_id;

	switch( fb_dev_id )
	{
		case LX_FBDEV_ID_CSR0:
		{
			int	csr_x_max;
			int csr_x_off, csr_y_off, csr_x_pos, csr_y_pos, csr_x_size, csr_y_size;

			/* raxis.lim (2014/06/16) -- auto adjust cursor_x,y when cursor_x,y are feeded inside FHD range */
			#if 0
			if( g_fbdev_cfg->uhd_disp_mode || hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#else
			if( g_fbdev_cfg->uhd_disp_mode || hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#endif
			{
				canvas.output_win.y = canvas.output_win.y*2;
			}

            /* if cursor csr_hotspot is active then adjust (x,y) position of cursor layer.
             * if cursor position is negative, I should move the bitmap address ( H/W doesn't accept negative position )
             *
             * IMPORTATNT !! if you change the base address, its alignment should be multiplt of 16 byte ( not 8 byte )
             * woonghyeon just said that it may be H/W bug.
             * since we are using ARGB8888 pixel format for cursor OSD. cursor offset should be multiple of 4 !!
			 */
			csr_x_max = canvas.stride / (canvas.bits_per_pixel>>3);

			csr_x_off = 0;
			csr_y_off = 0;
			csr_x_pos = canvas.output_win.x - canvas.csr_hotspot.x;
			csr_y_pos = canvas.output_win.y - canvas.csr_hotspot.y;

			/* raxis.lim (2014/06/16)
			 * H15 cursor HW seems to have a image corruption bug when cursor width is less than stride
			 */
			if( g_osd_csr_crash_fix )
			{
				csr_x_size= canvas.input_win.w;
				csr_y_size= canvas.input_win.h;
			}
			else
			{
				csr_x_size= (canvas.csr_size.w)? LX_CALC_ALIGNED_VALUE(canvas.csr_size.w,2): canvas.input_win.w;
				csr_y_size= (canvas.csr_size.h)? canvas.csr_size.h : canvas.input_win.h;
			}

			FBDEV_DEBUG("[fb#0] pos %dx%d csr_sz %d,%d info( %dx%d h %d,%d ) max_x %d disp_x %d\n", csr_x_pos, csr_y_pos, csr_x_size, csr_y_size,
						canvas.output_win.x, canvas.output_win.y, canvas.csr_hotspot.x, canvas.csr_hotspot.y, csr_x_max,  g_fbdev_cfg->display_width );

			/* if cursor is located at the left corner */
			if( g_osd_csr_crash_fix )
			{
	            if ( csr_x_pos < 0 ) { csr_x_pos = 0; }
				if ( csr_y_pos < 0 ) { csr_y_pos = 0; }
			}
			else
			{
    	        if ( csr_x_pos < 0 ) { csr_x_off = -csr_x_pos; csr_x_off = LX_CALC_ALIGNED_VALUE(csr_x_off,2); csr_x_pos = 0; csr_x_size -= csr_x_off; }
				if ( csr_y_pos < 0 ) { csr_y_off = -csr_y_pos; csr_y_pos = 0; csr_y_size -= csr_y_off; }
			}

			/* prevent OSD garbage */
			if ( csr_x_size > csr_x_max )	csr_x_size = csr_x_max;

			/* if cursor is located at the right corner */
            if ( csr_x_pos + csr_x_size > g_fbdev_cfg->display_width )
            {
                if (csr_x_pos > g_fbdev_cfg->display_width-12)
				{
					csr_x_pos = g_fbdev_cfg->display_width-12;
				}
				csr_x_pos = LX_CALC_ALIGNED_VALUE(csr_x_pos,1);

				if(g_osd_csr_crash_fix)
				{
					/* keep the current csr_x_size (view width) otherwise OSD HW get locked up */
				}
				else
				{
					csr_x_size= g_fbdev_cfg->display_width-csr_x_pos;
				}
            }

			osd_hdr.osd_hdr_xpos			= csr_x_pos;	/* output x (csr x pos) */
			osd_hdr.osd_hdr_ypos			= csr_y_pos;	/* output y (csr y pos) */
			osd_hdr.osd_hdr_w_mem			= csr_x_size;	/* viewing cursor width */
			osd_hdr.osd_hdr_h_mem			= csr_y_size;
			osd_hdr.osd_hdr_w_out			= csr_x_size;	/* output w */
			osd_hdr.osd_hdr_h_out			= csr_y_size;	/* output h */

			if( g_fbdev_cfg->uhd_disp_mode || hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			{
				osd_hdr.osd_hdr_w_out		*= 2;
				osd_hdr.osd_hdr_h_out		*= 2;
			}

			/* gMAU requests 16byte aligned address */
			bitmap_ptr	= canvas.osd_bm_paddr;
            bitmap_ptr += ( csr_y_off * canvas.stride ) + LX_CALC_ALIGNED_VALUE((csr_x_off * (canvas.bits_per_pixel>>3)),4);

			osd_hdr.osd_hdr_pixel_order	= canvas.pixel_order_en;
			osd_hdr.osd_hdr_wpl 			= canvas.stride>>3;

			FBDEV_DEBUG("[fb#1] pos %dx%d csr_sz %d,%d info( %dx%d h %d,%d ) \n\n", csr_x_pos, csr_y_pos, csr_x_size, csr_y_size,
						canvas.output_win.x, canvas.output_win.y, canvas.csr_hotspot.x, canvas.csr_hotspot.y );
		}
		break;

		case LX_FBDEV_ID_OSD0:
		case LX_FBDEV_ID_OSD1:
		default:
		{
			/* FIXME: in B.OSD path, OSD should be scale up to the video plane size (3840x2160 UHD)*/
			#if 0
			if ( hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#else
			if ( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#endif
			{
				canvas.output_win.w = disp_width;
				canvas.output_win.h = disp_height;
			}

			if ( canvas.input_win.w > disp_width ) canvas.input_win.w = disp_width;
			if ( canvas.input_win.h > disp_height) canvas.input_win.h = disp_height;

			if ( canvas.output_win.w > disp_width ) canvas.output_win.w = disp_width;
			if ( canvas.output_win.h > disp_height) canvas.output_win.h = disp_height;

#if 0 /* CLUT processing will be moved to user driver */
			/* raxis.lim (2012/11/16) -- handle CLUT
			 * if POSD is configured to 8bpp, this function will convert it to 32bpp image.
			 * DO NOT forget that this function will replace some canvas variable.
			 */
			FBDEV_H15CMN_HandleCLUT( fb_dev_id, &canvas, &osd_hdr );
#endif
			osd_hdr.osd_hdr_color_key_en= canvas.color_key_en;

			osd_hdr.osd_hdr_w_out		= canvas.output_win.w;	/* output w */
			osd_hdr.osd_hdr_h_out		= canvas.output_win.h;	/* output h */
			osd_hdr.osd_hdr_xpos	 	= canvas.output_win.x;	/* output x */
			osd_hdr.osd_hdr_ypos 		= canvas.output_win.y;	/* output y */
			osd_hdr.osd_hdr_w_mem 		= canvas.input_win.w;	/* input w  */
			osd_hdr.osd_hdr_h_mem 		= canvas.input_win.h;	/* input h  */

			bitmap_ptr	= canvas.osd_bm_paddr;

			/* modify bitmap ptr for input position is provided */
			if ( canvas.input_win.x > 0 || canvas.input_win.y > 0 )
			{
				FBDEV_DEBUG("fb(%d) bitmap based moved ! by %d,%d\n", fb_dev_id, canvas.input_win.x, canvas.input_win.y );

				/* gMAU requests 16byte aligned address */
				bitmap_ptr += ( canvas.input_win.y ) * canvas.stride;
				bitmap_ptr += LX_CALC_ALIGNED_VALUE( ((canvas.input_win.x*canvas.bits_per_pixel)>>3), 4);
			}
			FBDEV_DEBUG("fb(%d) bitmap param (%d,%d,%d,%d) stride %d ptr 0x%08x\n", fb_dev_id,
					canvas.input_win.x, canvas.input_win.y,
					canvas.input_win.w, canvas.input_win.h,
					canvas.stride, bitmap_ptr );

			osd_hdr.osd_hdr_pixel_order	= canvas.pixel_order_en;
			osd_hdr.osd_hdr_wpl 		= canvas.stride>>3;

		}
		break;
	}

	/* copy common attribute */
    osd_hdr.osd_hdr_format          = FBDEV_GetOSDPxlFmt( canvas.bits_per_pixel );
    osd_hdr.osd_hdr_depth           = FBDEV_GetOSDPxlDepth( canvas.bits_per_pixel );

	osd_hdr.osd_hdr_global_alpha_en	= canvas.global_alpha_en;
	osd_hdr.osd_hdr_global_alpha	= (canvas.global_alpha_en)? canvas.global_alpha : 0xff;
	osd_hdr.osd_hdr_color_key		= canvas.color_key;

	if ( g_reg_io_ctx->slkrd_prefix_mask )
	{
		osd_hdr.osd_hdr_ptr_bmp		= ((bitmap_ptr & 0x0FFFFFFF)| g_reg_io_ctx->slkrd_prefix_mask );
		osd_hdr.osd_hdr_ptr_plte	= ((canvas.osd_plte_paddr&0x0FFFFFFF)| g_reg_io_ctx->slkrd_prefix_mask );
	}
	else
	{
		osd_hdr.osd_hdr_ptr_bmp		= bitmap_ptr;
		osd_hdr.osd_hdr_ptr_plte	= canvas.osd_plte_paddr;
	}

#if 0
	/* handle virtual OSD if requested */
	FBDEV_H15CMN_HandleVOSD( fb_dev_id, &canvas, &osd_hdr );
#endif
	/* handle S3D if requestd */
	FBDEV_H15CMN_Handle3DOSD( fb_dev_id, &canvas, &osd_hdr );

	/* handle reverse OSD if requested */
	FBDEV_H15CMN_HandleReverseOSD( fb_dev_id, &canvas, &osd_hdr );

	FBDEV_DEBUG("fb(%d) bitmap param (%d,%d,%d,%d) stride %d ptr 0x%08x (final)\n", fb_dev_id,
			canvas.input_win.x, canvas.input_win.y,
			canvas.input_win.w, canvas.input_win.h,
			canvas.stride, bitmap_ptr );

	/* save osd header */
	memcpy( &hw_ctx->osd_hdr_sav[fb_dev_id], &osd_hdr, sizeof(FBDEV_H15_OSD_HDR_T));

#ifdef FBDEV_H15_CPB_BW_TEST
	if ( FBDEV_H15TEST_IsCPBTestMode() )
	{
		/* nop */ return RET_OK;
	}
#endif
	if ( hw_ctx->comp_ctrl.comp_opt != FB_COMP_OPT_NONE )
	{
		FBDEV_DEBUG("osd merge (case3:hdr-update) from %s\n", __F__ );
		FBDEV_H15CMN_DoOSDMerge(fb_dev_id, TRUE);
	}
	else
	{
		osd_exhdr.bits_per_pixel = canvas.bits_per_pixel;
		FBDEV_H15CMN_WriteHdrRegs(fb_dev_id, &osd_hdr, &osd_exhdr);
	}

	/* update FRC header if FRC SEP path enabled */
	FBDEV_H15CMN_WriteFRCHeader( );
//	up(&fbdev_sem_h15);
	return RET_OK;
}

/*========================================================================================
    chip revision handler
=======================================================================================*/
FBDEV_H15_HW_FUNC_T  g_fbdev_h15a0_hw_func =
{
	/* variable is initialize at fbdev_drv_h15.c */

    .H15_InitHW			 = FBDEV_H15A0_InitHW,

	.H15_SetViewCtrl	 = FBDEV_H15CMN_SetViewCtrl,
    .H15_SetConnCtrl     = FBDEV_H15CMN_SetConnCtrl,
	.H15_SetPivotCtrl	 = FBDEV_H15CMN_SetPivotCtrl,

    .H15_SetTridCtrl     = FBDEV_H15CMN_SetTridCtrl,
    .H15_WriteOSDHeader  = FBDEV_H15A0_WriteOSDHeader,
    .H15_SetOSDEnable    = FBDEV_H15CMN_SetOSDEnable,
    .H15_GetOSDEnable    = FBDEV_H15CMN_GetOSDEnable,
    .H15_ISRTickHandler  = FBDEV_H15CMN_ISRHook,
};

