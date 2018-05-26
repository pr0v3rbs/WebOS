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
#include "fbdev_hw_h13.h"
#include "fbdev_reg_h13.h"

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
extern	volatile OSD_H13_REG_T*		g_fbdev_h13_reg;
extern	OSD_H13_REG_T*				g_fbdev_h13_reg_cached;

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

/** initialize H13(A1) HW Ctx, Cfg and all chip dependent variables.
 * 
 */
void	FBDEV_H13A0_InitHW ( void )
{
	g_fbdev_frc_ctrl.b_frc_sep_enable	= FALSE;				/* Blending Mode by default */
	g_fbdev_frc_ctrl.b_frc_UD_enable	= FALSE;				/* Standard Mode by default */
	g_fbdev_frc_ctrl.b_frc_hdr_emul		= FALSE;				/* Disable FRC HDR emulation */
	g_fbdev_frc_ctrl.b_frc_osd_emul		= TRUE;
	g_fbdev_frc_ctrl.frc_hdr_emul_dev	= -1;
}

/** Set Header information
 *
 * @param	SCanvas : pointer to structure bearing osd header information
 * @return	void
 *
 * @ingroup osd_ddi_func
 */
int FBDEV_H13A0_WriteOSDHeader( struct scanvas *pCanvas )
{
	int				ret;
	int				fb_dev_id;
	UINT32			bitmap_ptr;
	FBDEV_H13_OSD_HDR_T osd_hdr;
	FBDEV_H13_OSD_EXHDR_T osd_exhdr;
	struct scanvas	canvas;

	/* clear osd_hdr before writing */
	memset( &osd_hdr, 0x0, sizeof(FBDEV_H13_OSD_HDR_T) );
	memset( &osd_exhdr, 0x0, sizeof(FBDEV_H13_OSD_EXHDR_T) );
	memcpy( &canvas, pCanvas, sizeof(struct scanvas) );

	ret = 0; //down_interruptible(&fbdev_sem_h13);

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
            if ( csr_x_size< 12)			csr_x_size = 12;		/* for preventing OSD H/W crash or garbage */
			if ( csr_x_size> csr_x_max )	csr_x_size = csr_x_max;	/* for preventing OSD garbage */

			/* raxis.lim (2014/05/07)
             * if cursor is located at the right corner, cursor x_pos should be multiple of 4.
			 * But accodring to board test, cursor x_pos can be multiple of 2 if cursor x_pos is smaller than -12 pixel of right corner.
			 * OSD HW crops the cursor width automatically during mixing osd layers.
             */
            if ( csr_x_pos + csr_x_size > g_fbdev_cfg->display_width )
			{ 
				if (csr_x_pos > g_fbdev_cfg->display_width-12) {csr_x_pos = g_fbdev_cfg->display_width-12; }
				csr_x_pos = LX_CALC_ALIGNED_VALUE(csr_x_pos,1);
				// csr_x_size= g_fbdev_cfg->display_width - csr_x_pos;
			}

			osd_hdr.osd_hdr_xpos			= csr_x_pos;	/* output x (csr x pos) */
			osd_hdr.osd_hdr_ypos			= csr_y_pos;	/* output y (csr y pos) */
			osd_hdr.osd_hdr_w_mem			= csr_x_size;	/* viewing cursor width */
			osd_hdr.osd_hdr_h_mem			= csr_y_size;	/* viewing cursor height */
			osd_hdr.osd_hdr_w_out			= csr_x_size;	/* output w */
			osd_hdr.osd_hdr_h_out			= csr_y_size;	/* output h */

			/* gMAU requests 16byte aligned address */
			bitmap_ptr	= canvas.osd_bm_paddr;
            bitmap_ptr += ( csr_y_off * canvas.stride ) + LX_CALC_ALIGNED_VALUE((csr_x_off * (canvas.bits_per_pixel>>3)),4);

			osd_hdr.osd_hdr_pixel_order	= canvas.pixel_order_en;
			osd_hdr.osd_hdr_wpl 		= canvas.stride>>3;

			FBDEV_DEBUG("[fb#1] pos %d,%d, sz %d,%d, input( pos %d,%d, sz %d,%d, hot %d,%d )\n\n", 
						csr_x_pos, csr_y_pos, csr_x_size, csr_y_size, 
						canvas.output_win.x, canvas.output_win.y, canvas.csr_size.w, canvas.csr_size.h, canvas.csr_hotspot.x, canvas.csr_hotspot.y ); 
		}
		break;

		case LX_FBDEV_ID_OSD0:
		case LX_FBDEV_ID_OSD1:
		default:
		{
			UINT32	max_disp_width = LX_RES_GET_WIDTH(lx_chip_graphic_res());
			UINT32	max_disp_height= LX_RES_GET_HEIGHT(lx_chip_graphic_res());

			FBDEV_CHECK_ERROR( g_fbdev_cfg->uhd_disp_mode && fb_dev_id != LX_FBDEV_ID_OSD0, return RET_ERROR,
					"invalid fb(%d) for UHD mode\n", fb_dev_id );

			FBDEV_DEBUG("canvas info (%4d,%4d) -> (%4d,%4d,%4d,%4d) disp max(%4d,%4d)\n", 
				canvas.input_win.w, canvas.input_win.h, 
				canvas.output_win.x, canvas.output_win.y, canvas.output_win.w, canvas.output_win.h,
				max_disp_width, max_disp_height );

			if ( canvas.output_win.w > max_disp_width ) canvas.output_win.w = max_disp_width;
			if ( canvas.output_win.h > max_disp_height) canvas.output_win.h = max_disp_height;

			/* raxis.lim (2012/11/16) -- handle CLUT
			 * if POSD is configured to 8bpp, this function will convert it to 32bpp image.
			 * DO NOT forget that this function will replace some canvas variable.
			 */
			FBDEV_H13CMN_HandleCLUT( fb_dev_id, &canvas, &osd_hdr );

			osd_hdr.osd_hdr_color_key_en= canvas.color_key_en;

			osd_hdr.osd_hdr_w_out		= canvas.output_win.w;	/* output w */
			osd_hdr.osd_hdr_h_out		= canvas.output_win.h;	/* output h */
			osd_hdr.osd_hdr_xpos	 	= canvas.output_win.x;	/* output x */
			osd_hdr.osd_hdr_ypos 		= canvas.output_win.y;	/* output y */
			osd_hdr.osd_hdr_w_mem 		= canvas.input_win.w;	/* input w  */
			osd_hdr.osd_hdr_h_mem 		= canvas.input_win.h;	/* input h  */

			bitmap_ptr					= canvas.osd_bm_paddr;

			/* modify bitmap ptr for input position is provided */
			if ( canvas.input_win.x > 0 || canvas.input_win.y > 0 )
			{
				/* gMAU requests 16byte aligned address */
				bitmap_ptr += ( canvas.input_win.y ) * canvas.stride;
				bitmap_ptr += LX_CALC_ALIGNED_VALUE( ((canvas.input_win.x*canvas.bits_per_pixel)>>3), 4);
			}
			FBDEV_DEBUG("fb(%d) bitmap param in (%d,%d,%d,%d) out (%d,%d) stride %d ptr 0x%08x\n", fb_dev_id,
					canvas.input_win.x, canvas.input_win.y,
					canvas.input_win.w, canvas.input_win.h,
					canvas.output_win.w, canvas.output_win.h, 
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
	FBDEV_H13CMN_HandleVOSD( fb_dev_id, &canvas, &osd_hdr );

	/* handle S3D if requestd */
	FBDEV_H13CMN_Handle3DOSD( fb_dev_id, &canvas, &osd_hdr );

	/* handle reverse OSD if requested */
	FBDEV_H13CMN_HandleReverseOSD( fb_dev_id, &canvas, &osd_hdr );

	FBDEV_DEBUG("fb(%d) bitmap param (%d,%d,%d,%d) stride %d ptr 0x%08x (final)\n", fb_dev_id,
			canvas.input_win.x, canvas.input_win.y,
			canvas.input_win.w, canvas.input_win.h,
			canvas.stride, bitmap_ptr );

	/* save osd header */
	memcpy( &g_fbdev_h13_hw_ctx.osd_hdr_sav[fb_dev_id], &osd_hdr, sizeof(FBDEV_H13_OSD_HDR_T));

	if ( g_fbdev_h13_hw_ctx.comp_ctrl.comp_opt != FB_COMP_OPT_NONE )
	{
		FBDEV_DEBUG("osd merge (case3:hdr-update) from %s\n", __F__ );
		FBDEV_H13CMN_DoOSDMerge(fb_dev_id, TRUE);
	}
	else
	{
		osd_exhdr.bits_per_pixel = canvas.bits_per_pixel;
		FBDEV_H13CMN_WriteHdrRegs(fb_dev_id, &osd_hdr, &osd_exhdr );
	}

	/* update FRC header if FRC SEP path enabled */
	FBDEV_H13CMN_WriteFRCHeader( );
//	up(&fbdev_sem_h13);
	return RET_OK;
}

/*========================================================================================
	Implementation Group (ISR)
========================================================================================*/
void FBDEV_H13A0_ISRTickHandler ( void )
{
#ifdef FBDEV_USE_OSD_STATUS_MON	/* debug only */
	UINT32* a0 = (UINT32*) &g_fbdev_h13_reg->osd0_ctrl_main;
	UINT32* a1 = (UINT32*) &g_fbdev_h13_reg->osd0_ctrl_base_addr;
	UINT32* a2 = (UINT32*) &g_fbdev_h13_reg->osd0_ctrl_hdr6;
	UINT32* b0 = (UINT32*) &g_fbdev_h13_reg->osd1_ctrl_main;
	UINT32* b1 = (UINT32*) &g_fbdev_h13_reg->osd1_ctrl_base_addr;
	UINT32* b2 = (UINT32*) &g_fbdev_h13_reg->osd1_ctrl_hdr6;

	printk("POSD0 0x%08x, 0x%08x, 0x%08x  POSD1 0x%08x, 0x%08x, 0x%08x\n", *a0, *a1, *a2, *b0, *b1, *b2 );
#endif

	FBDEV_H13CMN_HandleISR ( );
}

/*========================================================================================
    chip revision handler
=======================================================================================*/
FBDEV_H13_HW_CTX_T  g_fbdev_h13a0_hw_ctx =
{
	/* variable is initialize at fbdev_drv_h13.c */

    .H13_InitHW			 = FBDEV_H13A0_InitHW,

	.H13_SetViewCtrl	 = FBDEV_H13CMN_SetViewCtrl,
    .H13_SetConnCtrl     = FBDEV_H13CMN_SetConnCtrl,
	.H13_SetPivotCtrl	 = FBDEV_H13CMN_SetPivotCtrl,

    .H13_SetTridCtrl     = FBDEV_H13CMN_SetTridCtrl,
    .H13_WriteOSDHeader  = FBDEV_H13A0_WriteOSDHeader,
    .H13_SetOSDEnable    = FBDEV_H13CMN_SetOSDEnable,
    .H13_GetOSDEnable    = FBDEV_H13CMN_GetOSDEnable,
    .H13_ISRTickHandler  = FBDEV_H13A0_ISRTickHandler,
};

