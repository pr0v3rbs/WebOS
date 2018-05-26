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
 *  main configuration file for fbdev device
 *	fbdev device will teach you how to make device driver with new platform.
 *
 *  author		raxis
 *  version		1.0
 *  date		2010.02.01
 *  note		Additional information.
 *
 *  @addtogroup lg1150_fbdev
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include <linux/mm.h>
#include <linux/kernel.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define LCD_PANEL_CLOCK				180000   // not be used

#define	MINIMUM_FB_XRES				1280	// when FB doesn't have internal FB memory, use this value
#define	MINIMUM_FB_YRES				4		// when FB doesn't have internal FB memory, use this value

#if CONFIG_LX_GRAPHIC_RES>0
    #define lx_chip_graphic_res()   CONFIG_LX_GRAPHIC_RES
#else
    extern UINT32 lx_chip_graphic_res(void);
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
/** OSD header size
 *
 */
#define OSD_HEADER_SIZE             32		// same as sizeof(LG1150_OSDHEADER_T))

/** OSD header memory address
 *
 */
#define OSD_BASE_ADDR				(g_reg_io_ctx->frame_base_addr)
#define OSD_HDR_SIZE				LX_CALC_ALIGNED_VALUE(32,4)
#define OSD_PAL_SIZE				(sizeof(UINT32)*256)

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
LX_FBDEV_DEV_MEM_CFG_T gMemCfgFBDev[4] = {
{
	.mem_palette_name	= "fb0_palette",
	.mem_palette_base	= 0x0,
	.mem_palette_size	= 0x0,
	.mem_frame_name		= "fb0_frame", .mem_frame_base		= 0x0, .mem_frame_size		= 0x0,
},
{
	.mem_palette_name	= "fb1_palette", .mem_palette_base	= 0x0, .mem_palette_size	= 0x0,
	.mem_frame_name		= "fb1_frame", .mem_frame_base		= 0x0, .mem_frame_size		= 0x0,
},
{
	.mem_palette_name	= "fb2_palette", .mem_palette_base	= 0x0, .mem_palette_size	= 0x0,
	.mem_frame_name		= "fb2_frame", .mem_frame_base		= 0x0, .mem_frame_size		= 0x0,
},
{
	.mem_palette_name	= "fb3_palette", .mem_palette_base	= 0x0, .mem_palette_size	= 0x0,
	.mem_frame_name		= "fb3_frame", .mem_frame_base		= 0x0, .mem_frame_size		= 0x0,
}
};

LX_FBDEV_HDR_MEM_CFG_T gMemCfgFBHdr[4] = {
{
	.mem_header_name	= "fb0_header", .mem_header_base	= 0x0, .mem_header_size	= 0x0,
},
{
	.mem_header_name	= "fb1_header", .mem_header_base	= 0x0, .mem_header_size	= 0x0,
},
{
	.mem_header_name	= "fb2_header", .mem_header_base	= 0x0, .mem_header_size	= 0x0,
},
{
	.mem_header_name	= "fb3_header", .mem_header_base	= 0x0, .mem_header_size	= 0x0,
}
};


/** chip register configuration
 *
 */
LX_FBDEV_REG_IO_CTX_T    g_fbdev_reg_cfg[] =
{
    /* for L9A0, L9A1 -- 2013/09/03 L9 drv is not managed anymore */
    [0] = {
        .chip_name          = "FBDEV-L9Ax",
    },
    /* for L9B0,B1,B2 -- 2013/09/03 L9 drv is not managed anymore */
    [1] = {
        .chip_name          = "FBDEV-L9Bx",
    },

    // for H13
    [2] = {
		.chip_name          = "FBDEV-H13B0 & later",
		.reg_base_addr      = 0xC0035100,
		.reg_size           = 0x200,
		.irq_num            = (32)+64,//H13_IRQ_OSD_BCPU,

		.slkrd_prefix_mask  = 0x00000000,
		.slkrd_mmap_mask    = 0x00000000,
		.hdr_op_method		= FBDEV_HDR_OP_REG,	/* hdr operation */
		.frame_base_addr    = 0x00000000,		/* set base_addr to 0x0 for dynamic allocation */
		.frame_base_size    = 0x02000000,		/* 32MB */
    },

    // for M14
    [3] = {
		.chip_name          = "FBDEV-M14A0",
		.reg_base_addr      = 0xC0035100,		/* M14A0 = 0xc003_5100 */
		.reg_size           = 0x200,
		.irq_num            = (32)+64,			//H13_IRQ_OSD_BCPU,

		.slkrd_prefix_mask  = 0x00000000,
		.slkrd_mmap_mask    = 0x00000000,
		.hdr_op_method		= FBDEV_HDR_OP_REG,	/* hdr operation */
		.frame_base_addr    = 0x00000000,		/* set base_addr to 0x0 for dynamic allocation */
		.frame_base_size    = 0x02000000,		/* 32MB */
    },
    [4] = {
		.chip_name          = "FBDEV-M14B0 & later",
		.reg_base_addr      = 0xC0024100,		/* M14B0 = 0xc002_4100 */
		.reg_size           = 0x200,
		.irq_num            = (32)+64,			//H13_IRQ_OSD_BCPU,

		.slkrd_prefix_mask  = 0x00000000,
		.slkrd_mmap_mask    = 0x00000000,
		.hdr_op_method		= FBDEV_HDR_OP_REG,	/* hdr operation */
		.frame_base_addr    = 0x00000000,		/* set base_addr to 0x0 for dynamic allocation */
		.frame_base_size    = 0x02000000,		/* 32MB */
    },

    // for H14
    [5] = {
		.chip_name          = "FBDEV-H14",
		.reg_base_addr      = 0xC0035100,
		.reg_size           = 0x200,
		.irq_num            = (32)+64,//H13_IRQ_OSD_BCPU,

		.slkrd_prefix_mask  = 0x00000000,
		.slkrd_mmap_mask    = 0x00000000,
		.hdr_op_method		= FBDEV_HDR_OP_REG,	/* hdr operation */
		.frame_base_addr    = 0x00000000,		/* set base_addr to 0x0 for dynamic allocation */
		.frame_base_size    = 0x02000000,		/* 32MB */
    },

    // for H15
    [6] = {
		.chip_name          = "FBDEV-H15",
		.reg_base_addr      = 0xC8024100,		/* confirmed by woonghyeon.kim */
		.reg_size           = 0x600,
		.irq_num            = (32)+61,			/* IRQS[61] = intr_osd, confirmed by woonghyeon.kim */

		.slkrd_prefix_mask  = 0x00000000,
		.slkrd_mmap_mask    = 0x00000000,
		.hdr_op_method		= FBDEV_HDR_OP_REG,	/* hdr operation */
		.frame_base_addr    = 0x00000000,		/* set base_addr to 0x0 for dynamic allocation */
		.frame_base_size    = 0x02000000,		/* 32MB */
	},
};

LX_FBDEV_REG_IO_CTX_T*	g_reg_io_ctx = NULL;
LX_FBDEV_CFG_T*			g_fbdev_cfg = NULL;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_FBDEV_CFG_T	g_fbdev_cfg_default =
{
	.b_use_irq				= 0x1,
	.b_gpu_clock_enable		= 0x1,		// GP platform donesn't use GPU !!
	.cursor_layer			= 0x3,
	.uhd_disp_mode			= 0,		// disable UHD display mode by default
	.display_width			= 0,		// default dispaly width
	.display_height			= 0,		// default display height

	.fb[0] = {
		.b_active			= 0x1,
		.b_little_endian	= 0x1,		// OSD endian is fixed to the littlen. DO NOT try big endian. it's never tested.
		.b_show_on_startup	= 0x0,
		.b_use_vsync		= 0x1,
		.b_use_global_alpha	= 0x0,
		.max_buf_num		= 2,
		.bits_per_pixel		= 32,
		.pixel_format		= LX_FBDEV_PIXEL_FORMAT_RGB_8888,
		.max_xres			= CONFIG_LX_FB0_WIDTH,
		.max_yres			= CONFIG_LX_FB0_HEIGHT,

		.b_gfx_lock_req		= 0x0,
		.b_use_trid_hw		= 0x0,
		.b_use_builtin_fbmem= CONFIG_LX_BUILTIN_FBMEM,
	},

	.fb[1] = {
		.b_active			= 0x1,
		.b_little_endian	= 0x1,	// OSD endian is fixed to the littlen. DO NOT try big endian. it's never tested.
		.b_show_on_startup	= 0x0,
		.b_use_vsync		= 0x1,
		.b_use_global_alpha	= 0x0,
		.max_buf_num		= 2,
		.bits_per_pixel		= 32,
		.pixel_format		= LX_FBDEV_PIXEL_FORMAT_RGB_8888,
#ifdef USE_WORST_BW_TEST_SUPPORT
		.max_xres			= 3840,
		.max_yres			= 2160,
#else
		.max_xres			= CONFIG_LX_FB1_WIDTH,
		.max_yres			= CONFIG_LX_FB1_HEIGHT,
#endif
		.b_gfx_lock_req		= 0x0,
		.b_use_trid_hw		= 0x0,
#ifdef USE_WORST_BW_TEST_SUPPORT
		.b_use_builtin_fbmem= 0x1,
#else
		.b_use_builtin_fbmem= CONFIG_LX_BUILTIN_FBMEM,
#endif
	},


	/* raxis.lim (2011/11/19) -- L9 doesn't support POSD2 since it is used as alpha blender metal revison */
	.fb[2] = {
		.b_active			= 0x0,
		.b_little_endian	= 0x1,	// OSD endian is fixed to the littlen. DO NOT try big endian. it's never tested.
		.b_show_on_startup	= 0x0,
		.b_use_vsync		= 0x1,	// UI screen sometimes needs animation synced with vsync isr.

		.b_use_global_alpha	= 0x0,
		.max_buf_num		= 2,
		.bits_per_pixel		= 32,
		.pixel_format		= LX_FBDEV_PIXEL_FORMAT_RGB_8888,
		.max_xres			= 128,
		.max_yres			= 2,

		.b_gfx_lock_req		= 0x0,
		.b_use_trid_hw		= 0x0,
		.b_use_builtin_fbmem= CONFIG_LX_BUILTIN_FBMEM,
	},

	.fb[3] = {
		.b_active			= 0x1,
		.b_little_endian	= 0x1,	// OSD endian is fixed to the littlen. DO NOT try big endian. it's never tested.
		.b_show_on_startup	= 0x0,
		.b_use_vsync		= 0x1,

		.b_use_global_alpha	= 0x0,
		.max_buf_num		= 2,
		.bits_per_pixel		= 32,
		.pixel_format		= LX_FBDEV_PIXEL_FORMAT_RGB_8888,
		.max_xres			= 128,
		.max_yres			= 128,

		.b_gfx_lock_req		= 0x0,
		.b_use_trid_hw		= 0x0,
		.b_use_builtin_fbmem= CONFIG_LX_BUILTIN_FBMEM,	/* fixed to TRUE, DO NOT change value */
	},
};

static struct fb_var_screeninfo default_fb_var_screeninfo[LX_FBDEV_ID_MAX] = {

/* OSD0 default value */
{
	/* xres, yres, xres_virtual, yres_virtual is initialized at FBDEV_InitCfg */
	/* bits_per_pixel, red, green, blue, transp is initialized at FBDEV_InitCfg */
	/* hsync_len ( pixel format ), vsync_len ( pixel depth ) is initialized at FBDEV_InitCfg */

	.xoffset 		= 0,
	.yoffset 		= 0,
	.grayscale		= 0,

	.nonstd			= 0,
	.activate		= FB_ACTIVATE_FORCE,
	.height			= -1,
	.width			= -1,
	.accel_flags	= 0,
	.pixclock		= LCD_PANEL_CLOCK,	/* picoseconds */
	.left_margin	= 40,				/* pixclocks */
	.right_margin	= 4,				/* pixclocks */
	.upper_margin	= 8,				/* line clocks */
	.lower_margin	= 2,				/* line clocks */

	.sync 			= 0,
	.vmode 			= FB_VMODE_NONINTERLACED,
},

/* OSD1 default value */
{
	/* xres, yres, xres_virtual, yres_virtual is initialized at FBDEV_InitCfg */
	/* bits_per_pixel, red, green, blue, transp is initialized at FBDEV_InitCfg */
	/* hsync_len ( pixel format ), vsync_len ( pixel depth ) is initialized at FBDEV_InitCfg */

	.xoffset = 0,
	.yoffset = 0,
	.grayscale = 0,

	.nonstd = 0,
	.activate = FB_ACTIVATE_FORCE,
	.height = -1,
	.width = -1,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK, /* picoseconds */
	.left_margin = 40,			 /* pixclocks */
	.right_margin = 4,			 /* pixclocks */
	.upper_margin = 8,			 /* line clocks */
	.lower_margin = 2,			 /* line clocks */

	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
},

/* OSD2 default value */
{
	/* xres, yres, xres_virtual, yres_virtual is initialized at FBDEV_InitCfg */
	/* bits_per_pixel, red, green, blue, transp is initialized at FBDEV_InitCfg */
	/* hsync_len ( pixel format ), vsync_len ( pixel depth ) is initialized at FBDEV_InitCfg */

	.xoffset = 0,
	.yoffset = 0,
	.grayscale = 0,

	.nonstd = 0,
	.activate = FB_ACTIVATE_FORCE,
	.height = -1,
	.width = -1,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK, /* picoseconds */
	.left_margin = 40,			 /* pixclocks */
	.right_margin = 4,			 /* pixclocks */
	.upper_margin = 8,			 /* line clocks */
	.lower_margin = 2,			 /* line clocks */

	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
},

/* OSD3 default value */
{
	/* xres, yres, xres_virtual, yres_virtual is initialized at FBDEV_InitCfg */
	/* bits_per_pixel, red, green, blue, transp is initialized at FBDEV_InitCfg */
	/* hsync_len ( pixel format ), vsync_len ( pixel depth ) is initialized at FBDEV_InitCfg */

	.xoffset = 0,
	.yoffset = 0,
	.grayscale = 0,

	.nonstd = 0,
	.activate = FB_ACTIVATE_FORCE,
	.height = -1,
	.width = -1,
	.accel_flags = 0,
	.pixclock = LCD_PANEL_CLOCK, /* picoseconds */
	.left_margin = 40,			 /* pixclocks */
	.right_margin = 4,			 /* pixclocks */
	.upper_margin = 8,			 /* line clocks */
	.lower_margin = 2,			 /* line clocks */

	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
},
};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/
/** get preinitialized fb_var_screeninfo object
 *
 *
 */
struct fb_var_screeninfo*   FBDEV_GetVarScreenObject ( int idx )
{
	FBDEV_CHECK_ERROR( idx >= LX_FBDEV_ID_MAX, return NULL, "out of range (%d)\n", idx );

	return &default_fb_var_screeninfo[idx];
}

/** initialize FBDEV configuration
 *
 *
 */
void	FBDEV_InitCfg	( void )
{
	int		i;

	/*-----------------------------------------------------------------------------------
	 * [H15] configuration
	 *----------------------------------------------------------------------------------*/
	if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H15) )
	{
		g_reg_io_ctx 	= &g_fbdev_reg_cfg[6];
		g_fbdev_cfg 	= &g_fbdev_cfg_default;
	}
	/*-----------------------------------------------------------------------------------
	 * [H14] configuration
	 *----------------------------------------------------------------------------------*/
	else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		g_reg_io_ctx 	= &g_fbdev_reg_cfg[5];
		g_fbdev_cfg 	= &g_fbdev_cfg_default;
	}
	/*-----------------------------------------------------------------------------------
	 * [M14] configuration
	 *----------------------------------------------------------------------------------*/
	else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14) )
	{
		switch(lx_chip_rev() & 0xfff0 )
		{
			case LX_CHIP_REV(M14,A0): 	g_reg_io_ctx = &g_fbdev_reg_cfg[3]; break;
			case LX_CHIP_REV(M14,B0):
			default:					g_reg_io_ctx = &g_fbdev_reg_cfg[4]; break;
		}
		g_fbdev_cfg 	= &g_fbdev_cfg_default;
	}
	/*-----------------------------------------------------------------------------------
	 * [H13] configuration
	 *----------------------------------------------------------------------------------*/
	else if( !LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H13) )
	{
		/* select reg cfg based on chip revision */
		g_reg_io_ctx = &g_fbdev_reg_cfg[2];
		g_fbdev_cfg 	= &g_fbdev_cfg_default;
	}
	/*-----------------------------------------------------------------------------------
 	 * unknown/not supported chips
	 *----------------------------------------------------------------------------------*/
	else
	{
        DBG_PRINT_ERROR("not supported chip 0x%x\n", lx_chip_rev() );
	}

	/*-----------------------------------------------------------------------------------
 	 * determine max disp_width, height, uhd_disp_mode
	 *
	 * UHD mode = 2560x1080, 3840x2160, 5120x2160
	 *----------------------------------------------------------------------------------*/
	g_fbdev_cfg->display_width = LX_RES_GET_WIDTH( lx_chip_graphic_res() );
	g_fbdev_cfg->display_height= LX_RES_GET_HEIGHT( lx_chip_graphic_res() );

	g_fbdev_cfg->uhd_disp_mode = (g_fbdev_cfg->display_width > 1920)? TRUE: FALSE;

	printk("[fb] osd graphic_res=%dx%d, uhd_disp_mode=%s\n",
					g_fbdev_cfg->display_width, g_fbdev_cfg->display_height, g_fbdev_cfg->uhd_disp_mode? "on":"off" );

#define	OSD_BUF_NUM(id)			g_fbdev_cfg->fb[(id)].max_buf_num
#define OSD0_XRES               g_fbdev_cfg->fb[0].max_xres
#define OSD0_YRES               g_fbdev_cfg->fb[0].max_yres
#define OSD0_BYTES_PER_PIXEL	( g_fbdev_cfg->fb[0].bits_per_pixel >> 3 )
#define OSD1_XRES               g_fbdev_cfg->fb[1].max_xres
#define OSD1_YRES               g_fbdev_cfg->fb[1].max_yres
#define OSD1_BYTES_PER_PIXEL	( g_fbdev_cfg->fb[1].bits_per_pixel >> 3 )
#define OSD2_XRES               g_fbdev_cfg->fb[2].max_xres
#define OSD2_YRES               g_fbdev_cfg->fb[2].max_yres
#define OSD2_BYTES_PER_PIXEL	( g_fbdev_cfg->fb[2].bits_per_pixel >> 3 )
#define OSD3_XRES               g_fbdev_cfg->fb[3].max_xres
#define OSD3_YRES               g_fbdev_cfg->fb[3].max_yres
#define OSD3_BYTES_PER_PIXEL	( g_fbdev_cfg->fb[3].bits_per_pixel >> 3 )

#define OSD0_MAX_SIZE			g_fbdev_cfg->fb[0].max_size
#define OSD1_MAX_SIZE			g_fbdev_cfg->fb[1].max_size
#define OSD2_MAX_SIZE			g_fbdev_cfg->fb[2].max_size
#define OSD3_MAX_SIZE			g_fbdev_cfg->fb[3].max_size

	if ( OSD0_XRES == 0 || OSD0_YRES == 0 /* auto detect */ )
	{
		OSD0_XRES = LX_RES_GET_WIDTH( lx_chip_graphic_res() );
		OSD0_YRES = LX_RES_GET_HEIGHT( lx_chip_graphic_res() );
	}

	if ( OSD1_XRES == 0 || OSD1_YRES == 0 /* auto detect */ )
	{
		OSD1_XRES = LX_RES_GET_WIDTH( lx_chip_graphic_res() );
		OSD1_YRES = LX_RES_GET_HEIGHT( lx_chip_graphic_res() );
	}

	OSD0_MAX_SIZE = PAGE_ALIGN( OSD0_XRES * OSD0_YRES * OSD0_BYTES_PER_PIXEL * OSD_BUF_NUM(0) );
	OSD1_MAX_SIZE = PAGE_ALIGN( OSD1_XRES * OSD1_YRES * OSD1_BYTES_PER_PIXEL * OSD_BUF_NUM(1) );
	OSD2_MAX_SIZE = PAGE_ALIGN( OSD2_XRES * OSD2_YRES * OSD2_BYTES_PER_PIXEL * OSD_BUF_NUM(2) );
	OSD3_MAX_SIZE = PAGE_ALIGN( OSD3_XRES * OSD3_YRES * OSD3_BYTES_PER_PIXEL * OSD_BUF_NUM(3) );

	/* construct default_fb_var_screeninfo array */
	for ( i=0; i< LX_FBDEV_ID_MAX; i++ )
	{
		struct fb_var_screeninfo* fb_info	= &default_fb_var_screeninfo[i];

		fb_info->xres			= g_fbdev_cfg->fb[i].max_xres;
		fb_info->yres			= g_fbdev_cfg->fb[i].max_yres;
		fb_info->xres_virtual	= g_fbdev_cfg->fb[i].max_xres;
		fb_info->yres_virtual	= g_fbdev_cfg->fb[i].max_yres * OSD_BUF_NUM(i);
		fb_info->bits_per_pixel	= g_fbdev_cfg->fb[i].bits_per_pixel;

		/* raxis.lim (2010/11/21) -- I dont't refer to pixel depth because bitfield definition is independent of pixel depth */
		switch( g_fbdev_cfg->fb[i].bits_per_pixel )
		{
			case 8: case 32:
			{
				struct fb_bitfield a = { 24, 8, 0 };
				struct fb_bitfield r = { 16, 8, 0 };
				struct fb_bitfield g = {  8, 8, 0 };
				struct fb_bitfield b = {  0, 8, 0 };

				fb_info->transp = a, fb_info->red = r, fb_info->green = g, fb_info->blue = b;
			}
			break;
			case 16:
			{
				if ( g_fbdev_cfg->fb[i].pixel_format == LX_FBDEV_PIXEL_FORMAT_RGB_565 )
				{
					struct fb_bitfield a = {  0, 0, 0 };
					struct fb_bitfield r = { 11, 5, 0 };
					struct fb_bitfield g = {  5, 6, 0 };
					struct fb_bitfield b = {  0, 5, 0 };
					fb_info->transp = a, fb_info->red = r, fb_info->green = g, fb_info->blue = b;
				}
				else if ( g_fbdev_cfg->fb[i].pixel_format == LX_FBDEV_PIXEL_FORMAT_RGB_4444 )
				{
					struct fb_bitfield a = { 12, 4, 0 };
					struct fb_bitfield r = {  8, 4, 0 };
					struct fb_bitfield g = {  4, 4, 0 };
					struct fb_bitfield b = {  0, 4, 0 };
					fb_info->transp = a, fb_info->red = r, fb_info->green = g, fb_info->blue = b;
				}
				else
				{
					DBG_PRINT_ERROR("sorry. not ready pxlfmt %d.\n", g_fbdev_cfg->fb[i].pixel_format );
				}
			}
			break;

			default:
			{
				DBG_PRINT_ERROR("sorry. not supported or not tested bpp %d.\n", g_fbdev_cfg->fb[i].bits_per_pixel );
			}
		}
	}

#if 0
	/* raxis.lim (2011/11/08) -- set FB memory info. this information is passed to the user application */
	g_fbdev_cfg->fb_mem_base = g_reg_io_ctx->frame_base_addr;
	g_fbdev_cfg->fb_mem_size = g_reg_io_ctx->frame_base_size;
#endif

	/* intialize FBDEV memory layout.
	 *
	 * if fbdev is configured not to have its own frame buffer, we should not allocate frame buffer !!
	 * so memcfg.mem_frame_size might be set as very small value such as 0x1000
	 * since fbdev doesn't have enough memory to offer LinuxFB functionalites, fbdev driver should return error for all fbdev calls
	 * For example, flip display and mmap always return error
	 */
	gMemCfgFBDev[0].mem_frame_base	= OSD_BASE_ADDR;
	gMemCfgFBDev[0].mem_frame_size	= ( g_fbdev_cfg->fb[0].b_use_builtin_fbmem)? OSD0_MAX_SIZE : 0x1000;
	gMemCfgFBDev[0].mem_palette_base= 0x00000000;
	gMemCfgFBDev[0].mem_palette_size= OSD_PAL_SIZE;

	gMemCfgFBDev[1].mem_frame_base	= 0x00000000;
	gMemCfgFBDev[1].mem_frame_size	= ( g_fbdev_cfg->fb[1].b_use_builtin_fbmem)? OSD1_MAX_SIZE : 0x1000;
	gMemCfgFBDev[1].mem_palette_base= 0x00000000;
	gMemCfgFBDev[1].mem_palette_size= OSD_PAL_SIZE;

	gMemCfgFBDev[2].mem_frame_base	= 0x00000000;
	gMemCfgFBDev[2].mem_frame_size	= ( g_fbdev_cfg->fb[2].b_use_builtin_fbmem)? OSD2_MAX_SIZE : 0x1000;
	gMemCfgFBDev[2].mem_palette_base= 0x00000000;
	gMemCfgFBDev[2].mem_palette_size= OSD_PAL_SIZE;

	gMemCfgFBDev[3].mem_frame_base	= 0x00000000;
	gMemCfgFBDev[3].mem_frame_size	= ( g_fbdev_cfg->fb[3].b_use_builtin_fbmem)? OSD3_MAX_SIZE : 0x1000;
	gMemCfgFBDev[3].mem_palette_base= 0x00000000;
	gMemCfgFBDev[3].mem_palette_size= OSD_PAL_SIZE;

	gMemCfgFBHdr[0].mem_header_base	= 0x0000000;
	gMemCfgFBHdr[0].mem_header_size	= OSD_HDR_SIZE;
	gMemCfgFBHdr[1].mem_header_base	= 0x00000000;
	gMemCfgFBHdr[1].mem_header_size	= OSD_HDR_SIZE;
	gMemCfgFBHdr[2].mem_header_base	= 0x00000000;
	gMemCfgFBHdr[2].mem_header_size	= OSD_HDR_SIZE;
	gMemCfgFBHdr[3].mem_header_base	= 0x00000000;
	gMemCfgFBHdr[3].mem_header_size	= OSD_HDR_SIZE;

	/* raxis.lim (2011/09/16)
	 * [L9] POSD3(cursor) will bs DDR2 to avoid the burst band width problem. DE allowed for POSD3 to use the 1MB in DDR2
	 */
	if ( !LX_COMP_CHIP( lx_chip_rev(), LX_CHIP_L9 ) )
	{
		gMemCfgFBDev[3].mem_palette_base	= 0x88000000 - (1<<20);	/* static alloc */
		gMemCfgFBDev[3].mem_frame_base		= 0x00000000;			/* auto alloc */
		gMemCfgFBHdr[3].mem_header_base		= 0x00000000;			/* auto alloc */
	}
}

void    FBDEV_GetMemCfg ( UINT32* mem_base, UINT32* mem_size )
{
	*mem_base = g_reg_io_ctx->frame_base_addr;
	*mem_size = g_reg_io_ctx->frame_base_size;
}

/** @} */

