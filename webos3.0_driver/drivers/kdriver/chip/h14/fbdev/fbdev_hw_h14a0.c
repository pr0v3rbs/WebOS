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
#undef	FBDEV_USE_OSD_STATUS_MON

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_drv.h"
#include "fbdev_hw.h"
#include "fbdev_hal.h"
#include "fbdev_hw_h14.h"
#include "fbdev_reg_h14.h"

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
extern	volatile OSD_H14_REG_T*		g_fbdev_h14_reg;
extern	OSD_H14_REG_T*				g_fbdev_h14_reg_cached;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static const int 					pixel_depth_2_bpp[PIX_DEPTH_MAX] = { 1, 2, 4, 8, 16, 24, 32  };

/*========================================================================================
	Implementation Group
========================================================================================*/

/** initialize H14(A1) HW Ctx, Cfg and all chip dependent variables.
 * 
 */
void	FBDEV_H14A0_InitHW ( void )
{
	g_fbdev_frc_ctrl.b_frc_sep_enable	= FALSE;				/* Blending Mode by default */
	g_fbdev_frc_ctrl.b_frc_UD_enable	= FALSE;				/* Standard Mode by default */
	g_fbdev_frc_ctrl.b_frc_hdr_emul		= FALSE;				/* Disable FRC HDR emulation */
	g_fbdev_frc_ctrl.b_frc_osd_emul		= TRUE;
	g_fbdev_frc_ctrl.frc_hdr_emul_dev	= -1;
}

int	FBDEV_H14A0_WriteUHDOSDHeader( int fb_dev_id, struct scanvas* fb_canvas )
{
	int    i;
	FBDEV_H14_OSD_HDR_T osd_hdr[2];
	FBDEV_H14_OSD_EXHDR_T osd_exhdr;
	UINT32 bitmap_ptr = fb_canvas->osd_bm_paddr;

	int		disp_width = g_fbdev_cfg->display_width;
	int		disp_stride= disp_width<<2;
	int		disp_height= g_fbdev_cfg->display_height;

	/* this function is valid for only UHD mode.
	 * in UHD mode, only POSD0 is supported for application.
	 * in UHD mode, all POSD dimension is the same as display dimension 
	 */
	FBDEV_CHECK_CODE( FALSE==g_fbdev_cfg->uhd_disp_mode, return RET_ERROR, "invalid call. not UHD mode\n");
	FBDEV_CHECK_CODE( fb_dev_id != LX_FBDEV_ID_OSD0, return RET_ERROR, "invalid fb(%d) for UHD mode\n", fb_dev_id );
	FBDEV_CHECK_CODE( disp_width != fb_canvas->input_win.w, return RET_ERROR, "in.w %d diff from %d\n", fb_canvas->input_win.w, disp_width );
	FBDEV_CHECK_CODE( disp_width != fb_canvas->output_win.w, return RET_ERROR, "out.w %d diff from %d\n", fb_canvas->input_win.w, disp_width );
	FBDEV_CHECK_CODE( disp_stride!= fb_canvas->stride, return RET_ERROR, "input.pitch %d diff from %d\n", fb_canvas->stride, disp_stride );

	/* all osd hdr is made based on disp dimension.
	 * DO NOT forget that osd frame buffer should be the same as the display dimension in UHD mode
	 */
	for (i=0;i<2;i++)
	{
		memset( &osd_hdr[i], 0x0, sizeof(FBDEV_H14_OSD_HDR_T));

		osd_hdr[i].osd_hdr_w_out	= disp_width>>1;						/* 1920 */
		osd_hdr[i].osd_hdr_h_out	= disp_height;							/* 2160 */
		osd_hdr[i].osd_hdr_xpos	= (disp_width>>1)*i;					/* 1920*i */
		osd_hdr[i].osd_hdr_ypos	= 0;
		osd_hdr[i].osd_hdr_w_mem	= (disp_width>>1);						/* 1920 */
		osd_hdr[i].osd_hdr_h_mem	= disp_height;							/* 2160 */

		osd_hdr[i].osd_hdr_wpl				= (disp_width<<2)>>3;					/* stride/8 */
		osd_hdr[i].osd_hdr_format			= FBDEV_GetOSDPxlFmt(32);
		osd_hdr[i].osd_hdr_depth			= FBDEV_GetOSDPxlDepth(32);
		osd_hdr[i].osd_hdr_global_alpha		= 0xff;

		osd_hdr[i].osd_hdr_ptr_bmp			= bitmap_ptr + i*((disp_width>>1)<<2);	/* bitmap_ptr + i*7680 (1920*4) */

		FBDEV_HW_DEBUG("UHD hdr[%d] in %d,%d, xy %d,%d, out %d,%d, wpl %d, fmt %d, dpth %d, bmp 0x%x\n", i,
			osd_hdr[i].osd_hdr_w_mem, osd_hdr[i].osd_hdr_h_mem,
			osd_hdr[i].osd_hdr_xpos , osd_hdr[i].osd_hdr_ypos,
			osd_hdr[i].osd_hdr_w_out, osd_hdr[i].osd_hdr_h_out,
			osd_hdr[i].osd_hdr_wpl, osd_hdr[i].osd_hdr_format, osd_hdr[i].osd_hdr_depth, osd_hdr[i].osd_hdr_ptr_bmp );
	}

	/* set disp size */
	OSD_H14_RdFL( osd_common_disp_size );
   	OSD_H14_Wr02( osd_common_disp_size, reg_common_disp_size_w, disp_width,		/* 3840 */
                                   		reg_common_disp_size_h, disp_height);	/* 2160 */
	OSD_H14_WrFL( osd_common_disp_size );

	osd_exhdr.bits_per_pixel = fb_canvas->bits_per_pixel;

	/* write osd header */
	FBDEV_H14CMN_WriteHdrRegs(0, &osd_hdr[0], &osd_exhdr);
	FBDEV_H14CMN_WriteHdrRegs(1, &osd_hdr[1], &osd_exhdr);

	return RET_OK;
}
/** Set Header information
 *
 * @param	SCanvas : pointer to structure bearing osd header information
 * @return	void
 *
 * @ingroup osd_ddi_func
 */
int FBDEV_H14A0_WriteOSDHeader( struct scanvas *pCanvas )
{
	int				ret;
	int				fb_dev_id;
	UINT32			bitmap_ptr;
	FBDEV_H14_OSD_HDR_T osd_hdr;
	FBDEV_H14_OSD_EXHDR_T osd_exhdr;
	struct scanvas	canvas;

	/* clear osd_hdr before writing */
	memset( &osd_hdr, 0x0, sizeof(FBDEV_H14_OSD_HDR_T) );
	memset( &osd_exhdr, 0x0, sizeof(FBDEV_H14_OSD_EXHDR_T) );
	memcpy( &canvas, pCanvas, sizeof(struct scanvas) );

	ret = 0; //down_interruptible(&fbdev_sem_h14);

	fb_dev_id	= canvas.fb_dev_id;

	switch( fb_dev_id )
	{
		case LX_FBDEV_ID_CSR0:
		{
            /* if cursor csr_hotspot is active then adjust (x,y) position of cursor layer.
             * if cursor position is negative, I should move the bitmap address ( H/W doesn't accept negative position )
             *
             * IMPORTATNT !! if you change the base address, its alignment should be multiplt of 16 byte ( not 8 byte )
             * woonghyeon just said that it may be H/W bug.
             * since we are using ARGB8888 pixel format for cursor OSD. cursor offset should be multiple of 4 !!
			 */
			int csr_x_max = canvas.stride / (canvas.bits_per_pixel>>3);

			int csr_x_off = 0;
			int csr_y_off = 0;
			int csr_x_pos = canvas.output_win.x - canvas.csr_hotspot.x;
			int csr_y_pos = canvas.output_win.y - canvas.csr_hotspot.y;
			int csr_x_size= (canvas.csr_size.w)? LX_CALC_ALIGNED_VALUE(canvas.csr_size.w,2) : canvas.input_win.w;
			int csr_y_size= (canvas.csr_size.h)? canvas.csr_size.h : canvas.input_win.h;

			FBDEV_DEBUG("[fb#0] pos %dx%d csr_sz %d,%d info( %dx%d h %d,%d ) max_x %d\n", csr_x_pos, csr_y_pos, csr_x_size, csr_y_size, 
						canvas.output_win.x, canvas.output_win.y, canvas.csr_hotspot.x, canvas.csr_hotspot.y, csr_x_max );

			/* too small cursor with may lead to OSD corruption. According to system test, width should be greater than a half of OSD width */
            if ( csr_x_pos < 0 ) { csr_x_off = -csr_x_pos; csr_x_off = LX_CALC_ALIGNED_VALUE(csr_x_off,2); csr_x_pos = 0; csr_x_size -= csr_x_off; }
            if ( csr_y_pos < 0 ) { csr_y_off = -csr_y_pos; csr_y_pos = 0; csr_y_size -= csr_y_off; }
            if ( csr_x_size < 32 ) 			csr_x_size = 32;		/* for preventing OSD H/W crash */
			if ( csr_x_size > csr_x_max )	csr_x_size = csr_x_max;	/* for preventing OSD garbage */

            /* raxis.lim@lge.com (2011/09/23)
             * L9 donesn't support odd numbered x position when cursor is located to the far right side of the screen
             */
            if ( csr_x_pos + csr_x_size > g_fbdev_cfg->display_width ) csr_x_pos = LX_CALC_ALIGNED_VALUE(csr_x_pos,1);

			osd_hdr.osd_hdr_xpos			= csr_x_pos;	/* output x (csr x pos) */
			osd_hdr.osd_hdr_ypos			= csr_y_pos;	/* output y (csr y pos) */
			osd_hdr.osd_hdr_w_mem			= csr_x_size;	/* viewing cursor width */
			osd_hdr.osd_hdr_h_mem			= csr_y_size;
			osd_hdr.osd_hdr_w_out			= csr_x_size;	/* output w */
			osd_hdr.osd_hdr_h_out			= csr_y_size;	/* output h */

			/* gMAU requests 16byte aligned address */
			bitmap_ptr	= canvas.osd_bm_paddr;
            bitmap_ptr += ( csr_y_off * canvas.stride ) + LX_CALC_ALIGNED_VALUE((csr_x_off * (canvas.bits_per_pixel>>3)),4);

			osd_hdr.osd_hdr_pixel_order	= canvas.pixel_order_en;
			osd_hdr.osd_hdr_wpl 					= canvas.stride>>3;

			FBDEV_DEBUG("[fb#1] pos %dx%d csr_sz %d,%d info( %dx%d h %d,%d ) \n\n", csr_x_pos, csr_y_pos, csr_x_size, csr_y_size, 
						canvas.output_win.x, canvas.output_win.y, canvas.csr_hotspot.x, canvas.csr_hotspot.y ); 
		}
		break;

		case LX_FBDEV_ID_OSD0:
		case LX_FBDEV_ID_OSD1:
		default:
		{
#ifdef FBDEV_USE_H14_UHD_DEMO
			if (canvas.stride == 15360 && canvas.bits_per_pixel == 32)
			{			
				static BOOLEAN	b_video_blend_mode = FALSE;

				g_fbdev_cfg->uhd_disp_mode = TRUE;
				g_fbdev_cfg->display_width = 3840;
				g_fbdev_cfg->display_height= 2160;

				if (!b_video_blend_mode)
				{
					LX_FBDEV_CONN_CTRL_T conn;
					memset(&conn, 0x0, sizeof(LX_FBDEV_CONN_CTRL_T));
					conn.conn_type = LX_FBDEV_CONN_OSD_VIDEO_BLEND;
					FBDEV_INFO("H14 UHD DEMOD::BLEND Path !!\n");
					FBDEV_H14CMN_SetConnCtrl(&conn);

					b_video_blend_mode = TRUE;
				}
			}
#endif
			FBDEV_CHECK_ERROR( g_fbdev_cfg->uhd_disp_mode && fb_dev_id != LX_FBDEV_ID_OSD0, return RET_ERROR,
					"invalid fb(%d) for UHD mode\n", fb_dev_id );
			if ( g_fbdev_cfg->uhd_disp_mode )
			{
				FBDEV_HW_DEBUG("writing osd hdr for UHD mode !\n"); goto uhd_disp_mode;
			}

			if ( canvas.output_win.w > 1920 ) canvas.output_win.w = 1920;
			if ( canvas.output_win.h > 1080 ) canvas.output_win.h = 1080;

			/* raxis.lim (2012/11/16) -- handle CLUT
			 * if POSD is configured to 8bpp, this function will convert it to 32bpp image.
			 * DO NOT forget that this function will replace some canvas variable.
			 */
			FBDEV_H14CMN_HandleCLUT( fb_dev_id, &canvas, &osd_hdr );

			osd_hdr.osd_hdr_color_key_en= canvas.color_key_en;

#if 1 /* max size checker */
			if( canvas.output_win.w > 1920 )	canvas.output_win.w = 1920;
			if( canvas.output_win.h > 1080 )	canvas.output_win.h = 1080;
#endif
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
				/* gMAU requests 16byte aligned address */
				bitmap_ptr += ( canvas.input_win.y ) * canvas.stride;
				bitmap_ptr += LX_CALC_ALIGNED_VALUE( ((canvas.input_win.x*canvas.bits_per_pixel)>>3), 4);
			}
			FBDEV_DEBUG("fb(%d) bitmap param (%d,%d,%d,%d) stride %d ptr 0x%08x\n", fb_dev_id,
					canvas.input_win.x, canvas.input_win.y,
					canvas.input_win.w, canvas.input_win.h,
					canvas.stride, bitmap_ptr );

			osd_hdr.osd_hdr_pixel_order	= canvas.pixel_order_en;
			osd_hdr.osd_hdr_wpl 				= canvas.stride>>3;

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

	/* handle virtual OSD if requested */
	FBDEV_H14CMN_HandleVOSD( fb_dev_id, &canvas, &osd_hdr );

	/* handle S3D if requestd */
	FBDEV_H14CMN_Handle3DOSD( fb_dev_id, &canvas, &osd_hdr );

	/* handle reverse OSD if requested */
	FBDEV_H14CMN_HandleReverseOSD( fb_dev_id, &canvas, &osd_hdr );

	FBDEV_DEBUG("fb(%d) bitmap param (%d,%d,%d,%d) stride %d ptr 0x%08x (final)\n", fb_dev_id,
			canvas.input_win.x, canvas.input_win.y,
			canvas.input_win.w, canvas.input_win.h,
			canvas.stride, bitmap_ptr );

	/* save osd header */
	memcpy( &g_fbdev_h14_hw_ctx.osd_hdr_sav[fb_dev_id], &osd_hdr, sizeof(FBDEV_H14_OSD_HDR_T));

	if ( g_fbdev_h14_hw_ctx.comp_ctrl.comp_opt != FB_COMP_OPT_NONE )
	{
		FBDEV_DEBUG("osd merge (case3:hdr-update) from %s\n", __F__ );
		FBDEV_H14CMN_DoOSDMerge(fb_dev_id, TRUE);
	}
	else
	{
		osd_exhdr.bits_per_pixel = canvas.bits_per_pixel;

		FBDEV_H14CMN_WriteHdrRegs	( fb_dev_id, &osd_hdr, &osd_exhdr );
	}

	/* update FRC header if FRC SEP path enabled */
	FBDEV_H14CMN_WriteFRCHeader( );
//	up(&fbdev_sem_h14);
	return RET_OK;

uhd_disp_mode:
	return FBDEV_H14A0_WriteUHDOSDHeader(fb_dev_id, &canvas);
}

/*========================================================================================
	Implementation Group (ISR)
========================================================================================*/
void FBDEV_H14A0_ISRTickHandler ( void )
{
#ifdef FBDEV_USE_OSD_STATUS_MON	/* debug only */
	UINT32* a0 = (UINT32*) &g_fbdev_h14_reg->osd0_ctrl_main;
	UINT32* a1 = (UINT32*) &g_fbdev_h14_reg->osd0_ctrl_base_addr;
	UINT32* a2 = (UINT32*) &g_fbdev_h14_reg->osd0_ctrl_hdr6;
	UINT32* b0 = (UINT32*) &g_fbdev_h14_reg->osd1_ctrl_main;
	UINT32* b1 = (UINT32*) &g_fbdev_h14_reg->osd1_ctrl_base_addr;
	UINT32* b2 = (UINT32*) &g_fbdev_h14_reg->osd1_ctrl_hdr6;

	printk("POSD0 0x%08x, 0x%08x, 0x%08x  POSD1 0x%08x, 0x%08x, 0x%08x\n", *a0, *a1, *a2, *b0, *b1, *b2 );
#endif

	FBDEV_H14CMN_HandleISR ( );
}

/*========================================================================================
    chip revision handler
=======================================================================================*/
FBDEV_H14_HW_CTX_T  g_fbdev_h14a0_hw_ctx =
{
	/* variable is initialize at fbdev_drv_h14.c */

    .H14_InitHW			 = FBDEV_H14A0_InitHW,

	.H14_SetViewCtrl	 = FBDEV_H14CMN_SetViewCtrl,
    .H14_SetConnCtrl     = FBDEV_H14CMN_SetConnCtrl,
	.H14_SetPivotCtrl	 = FBDEV_H14CMN_SetPivotCtrl,

    .H14_SetTridCtrl     = FBDEV_H14CMN_SetTridCtrl,
    .H14_WriteOSDHeader  = FBDEV_H14A0_WriteOSDHeader,
    .H14_SetOSDEnable    = FBDEV_H14CMN_SetOSDEnable,
    .H14_GetOSDEnable    = FBDEV_H14CMN_GetOSDEnable,
    .H14_ISRTickHandler  = FBDEV_H14A0_ISRTickHandler,
};

