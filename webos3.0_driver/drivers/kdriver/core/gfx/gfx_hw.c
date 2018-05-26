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
 *	GFX drawing function control Drivers linux module
 *
 *  author      sinchul.jung (sinchul.jung@lge.com)
 *				raxis.lim    (raxis.lim@lge.com)
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
#define	GFX_IGNORE_DUPLICATE_PALTTE

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/delay.h>
#include <linux/workqueue.h>/**< For working queue */
#include <linux/interrupt.h>
#include <linux/sched.h>	/* for schedule_timeout */
#include <linux/dma-mapping.h>
#include <linux/version.h>

#include "gfx_impl.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#if 0
#define GFX_PORT_BASE_ADDR( surface, x, y )    \
        (ULONG)((surface)->surf.phys_addr) + (y) * (surface)->surf.stride + (x)*(surface)->surf.stride / (surface)->surf.width
#else
#define GFX_PORT_BASE_ADDR( surface, x, y )    \
        (ULONG)((surface)->surf.phys_addr) + (y) * (surface)->surf.stride + (((x)<=0)? 0 : ((x)%2)? (x)+1:(x)) * (surface)->surf.stride / (surface)->surf.width
#endif
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

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_GFX_BATCH_RUN_MODE_T 		g_gfx_run_mode = LX_GFX_BATCH_RUN_MODE_AUTO;
static LX_GFX_GRAPHIC_SYNC_MODE_T	g_gfx_sync_mode= LX_GFX_GRAPHIC_SYNC_MODE_AUTO;
static DECLARE_WAIT_QUEUE_HEAD(g_gfx_wq_done);

static int          g_gfx_hw_op_log = 0;
module_param_named( gfx_hw_op_log, g_gfx_hw_op_log, int, 0644 );

static int          g_gfx_cmd_delay_test_value = -1;
module_param_named( gfx_cmd_delay_test_val, g_gfx_cmd_delay_test_value, int, 0644 );

#ifdef GFX_IGNORE_DUPLICATE_PALTTE
static 	BOOLEAN	g_pal_init_0 = FALSE;	/* palette_0 HW initialized ? */
static 	BOOLEAN	g_pal_init_1 = FALSE;	/* palette_1 HW initialized ? */
static	UINT32	g_pal_data_0[256] = {[255]=0x00020202};
static	UINT32	g_pal_data_1[256] = {[255]=0x00020202};
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * register the pallete date to HW IP
 *
 * @param	int port_num , int size , UINT32 *data
 *	port_num : input port#(0~1)
 *	size : palette data size
 *	data : palette data
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_DownloadPalette(int port_num , int size , UINT32 *data)
{
	int ret = RET_OK;

	GFX_CMD_OP_T cmd_op;

	GFX_PRINT("%s:%d -- downloading palette port %d, palSize %d, palData %p\n", __F__, __L__, port_num, size, data );
	GFX_TRACE_BEGIN();

	memset( &cmd_op, 0x0, sizeof(GFX_CMD_OP_T));

	switch(port_num)
	{
		case 0:
		{
#ifdef GFX_IGNORE_DUPLICATE_PALTTE
			if ( g_pal_init_0 && !memcmp( g_pal_data_0, data, sizeof(UINT32)*size ) )
			{
				GFX_PRINT("<gfx> same palette 0 .. ignore\n"); goto func_exit;
			}
			memcpy( g_pal_data_0, data, sizeof(UINT32)*size );
			g_pal_init_0 = TRUE;
#endif
			cmd_op.sPort = eReadOnePort;
			cmd_op.bUpdateCLUT = 0x0;		/* raxis.lim (2010/07/01) -- bugf fix . write should be zero */
		}
		break;

		case 1:
		{
#ifdef GFX_IGNORE_DUPLICATE_PALTTE
			if ( g_pal_init_1 && !memcmp( g_pal_data_1, data, sizeof(UINT32)*size ) )
			{
				GFX_PRINT("<gfx> same palette 1 .. ignore\n"); goto func_exit;
			}
			memcpy( g_pal_data_1, data, sizeof(UINT32)*size );
			g_pal_init_1 = TRUE;
#endif
			cmd_op.sPort = eReadTwoPorts;
			cmd_op.bUpdateCLUT = 0x0;		/* raxis.lim (2010/07/01) -- bugf fix . write should be zero */
		}
		break;

		default:
			ret = RET_ERROR; goto func_exit;
		break;
	}

	GFX_SetOperationStatus(&cmd_op);

	if(GFX_SetCLUT(port_num , size , data) < 0)
	{
		GFX_ERROR("GFX_SetCLUT error\n"); ret = RET_ERROR; goto func_exit;
	}

func_exit:
	GFX_TRACE_END();
	return ret;
}

/** draw image on memory by application layer's setting
 *
 * @param	LX_GFX_MANUAL_BLEND_CTRL_PARAM_T
 * @return	int	0 : OK , -1 : NOT OK
 *
 *	raxis.lim (2010/05/15)
 *	--	bug fix in determine port->ulAddr
 *	--	port->ulAddr should be calculated by (base_addr + stride * y + byets_per_pixel * x )
 */
int GFX_RunBlendOp(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T *param, UINT32 cmd_delay )
{
	BOOLEAN					b_use_iport1 = FALSE, b_use_iport2 = FALSE;
	GFX_PORT_CONFIGURE_T 	*iport0 = NULL, *iport1 = NULL, *iport2 = NULL;
	GFX_OUT_CONFIGURE_T  	*oport;
	GFX_BLEND_CONFIGURE_T 	*blend;
	GFX_CMD_OP_T 			*operate;
	GFX_SCALER_CONFIGURE_T	*scaler;

	int ret=RET_OK;

	GFX_SURFACE_OBJ_T	*src0_surface;
	GFX_SURFACE_OBJ_T	*src1_surface;
	GFX_SURFACE_OBJ_T	*src2_surface;
	GFX_SURFACE_OBJ_T	*dst_surface;

	/* initialize premultiply/demultiply flag to be FALSE by default */
	BOOLEAN	fSrc0Premultiply = FALSE;
	BOOLEAN	fSrc1Premultiply = FALSE;
	BOOLEAN	fDstDemultiply	 = FALSE;

#if 0
	/* custom blend doesn't support LX_GFX_OP_CMD_WRITE */
	if ( param->op_mode == LX_GFX_OP_MODE_WRITE )
	{
		GFX_WARN("%s:%d -- WRITE mode is supported in GFX_IOW_BLEND\n", __F__, __L__ );
		return RET_ERROR;
	}

	/* custom blend doesn't support LX_GFX_OP_ONE_SRC_PORT */
	if ( param->op_mode == LX_GFX_OP_MODE_ONE_SRC_PORT )
	{
		GFX_WARN("%s:%d -- SCRBLT mode is supported in GFX_IOW_BLEND\n", __F__, __L__ );
		return RET_ERROR;
	}
#endif

#if 0
	/* get the dummy spin lock to sync memory */
	ULONG flags;
	spin_lock_irqsave(&_g_draw_spinlock, flags);
	spin_unlock_irqrestore(&_g_draw_spinlock, flags);
#endif

	GFX_TRACE_BEGIN();

	/* get surface handle for each GFX port */
	src0_surface = &(g_gfx_surf_list[param->src0.surface_fd]);
	src1_surface = &(g_gfx_surf_list[param->src1.surface_fd]);
	src2_surface = &(g_gfx_surf_list[param->src2.surface_fd]);
	dst_surface  = &(g_gfx_surf_list[param->dst.surface_fd]);

#ifdef GFX_HEX_DUMP
	OS_HexDump((UINT32)param, param, sizeof(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T));
#endif

	iport0	= (GFX_PORT_CONFIGURE_T*) OS_Malloc(sizeof(GFX_PORT_CONFIGURE_T));
	iport1	= (GFX_PORT_CONFIGURE_T*) OS_Malloc(sizeof(GFX_PORT_CONFIGURE_T));
	iport2	= (GFX_PORT_CONFIGURE_T *)OS_Malloc(sizeof(GFX_PORT_CONFIGURE_T));
	oport 	= (GFX_OUT_CONFIGURE_T*)  OS_Malloc(sizeof(GFX_OUT_CONFIGURE_T));
	blend 	= (GFX_BLEND_CONFIGURE_T*)OS_Malloc(sizeof(GFX_BLEND_CONFIGURE_T));
	scaler 	= (GFX_SCALER_CONFIGURE_T*)OS_Malloc(sizeof(GFX_SCALER_CONFIGURE_T));
	operate = (GFX_CMD_OP_T *)OS_Malloc(sizeof(GFX_CMD_OP_T));

	GFX_CHECK_ERROR( iport0 == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( iport1 == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( iport2 == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( oport  == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( blend  == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( scaler == NULL, goto func_exit, "out of memory\n");
	GFX_CHECK_ERROR( operate== NULL, goto func_exit, "out of memory\n");

	memset( iport0, 0x0, sizeof(GFX_PORT_CONFIGURE_T));
	memset( iport1, 0x0, sizeof(GFX_PORT_CONFIGURE_T));
	memset( iport2, 0x0, sizeof(GFX_PORT_CONFIGURE_T));
	memset( oport , 0x0, sizeof(GFX_OUT_CONFIGURE_T));
	memset( blend , 0x0, sizeof(GFX_BLEND_CONFIGURE_T));
	memset( operate,0x0, sizeof(GFX_CMD_OP_T));

	/* raxis.lim (2012/11/02)
	 * reset scaler H/W block for prevent some GFX H/W bugs. it's platform dependent.
	 * in H13B0, scaler block should be reset. otherwise output image is shifted to right by 10-20 pixels.
	 */
	memset( scaler, 0x0, sizeof(GFX_SCALER_CONFIGURE_T));
	scaler->cmd = GFX_SCALER_CMD_SOFT_RESET;
	GFX_SetScalerConfigure( scaler );

	/* raxis.lim (2011/05/19)
	 * when scaler enabled, we should modify dst rect to be the same dimension as src rect.
	 * GFX blender block requires the same dimension in both src and dst port.
	 * the real scaling dimension should be set after blender block
	 *
	 * since we are using only very simple scaler from src0 to dst port, we can refer to src0 dimension without any check code.
	 */
	if ( param->scaler.mode )
	{
		param->dst.rect.w = param->src0.rect.w;
		param->dst.rect.h = param->src0.rect.h;
	}

	GFX_DumpHWOpParam(param, FALSE);

	/*	parse src0 port configuration
	 *	-- alomost GFX operations needs read port0 (src0) except OP_MODE_WRITE and OP_MODE_PACK_Cb_Cr
	 *
	*	raxis.lim (2010/05/28)
	 *	[TODO] need some code to convert 8bpp or 16bpp surface to 32bpp ????
	 */
	{
		iport0->sPort 	= ePort0;
		iport0->sEndian	= eLITTLE;

		if ( param->src0.surface_type == LX_GFX_SURFACE_TYPE_COLOR )
		{
			iport0->usStride		= 0x0;
			iport0->ulAddr			= 0x0;
			iport0->sFmt			= LX_GFX_PIXEL_FORMAT_INDEX_0;
			iport0->uiGlobalAlpha 	= param->src0.surface_color.pixel_value;
		}
		else
		{
			iport0->usStride		= (UINT16)src0_surface->surf.stride;
			iport0->ulAddr			= GFX_GetPortBaseAddr( src0_surface, &param->src0.rect );
			iport0->sFmt 			= src0_surface->surf.pixel_format;
			iport0->uiGlobalAlpha 	= 0xffffffff;

			/* download palette if surface is index color format */
			if( src0_surface->surf.pixel_format <= LX_GFX_PIXEL_FORMAT_INDEX_8)
			{
				if( src0_surface->pal )
				{
					GFX_DownloadPalette( 0 , src0_surface->PalSize, src0_surface->pal);
				}
			}
		}

		iport0->bBitMaskEnable 	= FALSE;
		iport0->bCOCEnable 		= FALSE;
		iport0->bCSCEnable 		= FALSE;
		iport0->bColorKeyEnable = FALSE;

		/* enable/disable premulitication of source. */
		if ( param->src0.port_flag & LX_GFX_PORT_FLAG_PREMULTIPLY )
		{
			fSrc0Premultiply = TRUE;
		}

		/* enable/disable colorkey when input color is in inside color key range. */
		if ( param->src0.port_flag & LX_GFX_PORT_FLAG_COLORKEY_INSIDE )
		{
			iport0->bColorKeyEnable = 0x1;
			iport0->bColorKeyMode 	= 0x0;
		}
		/* enable/disable colorkey when input color is outside color key range. */
		else if ( param->src0.port_flag & LX_GFX_PORT_FLAG_COLORKEY_OUTSIDE )
		{
			iport0->bColorKeyEnable = 0x1;
			iport0->bColorKeyMode	= 0x1;
		}

		/* enable/disable bitmask color */
		if ( param->src0.port_flag & LX_GFX_PORT_FLAG_BITMASK )
		{
			iport0->bBitMaskEnable 	= 0x1;
			iport0->ulBitMask 		= param->src0.bitmask;
		}

		if ( param->src0.port_flag & LX_GFX_PORT_FLAG_COC_CTRL )
		{
			iport0->bCOCEnable		= 0x1;
			memcpy( &iport0->ulCOCCtrl, &param->src0.coc_ctrl, 0x4 );
		}

		if ( param->src0.port_flag & LX_GFX_PORT_FLAG_CSC_CTRL )
		{
			iport0->bCSCEnable		= 0x1;
			iport0->sCSCSel			= param->src0.csc_ctrl;
		}

		iport0->ulReplaceColor 	= param->src0.colorkey_tgt_color;
		iport0->ulKeyLow 		= param->src0.colorkey_lower_limit;
		iport0->ulKeyHigh 		= param->src0.colorkey_upper_limit;
	}

	/*	parse src1 port configuration
	 *	-- alomost GFX operations needs read port0 (src1) except OP_MODE_ONE_SRC_PORT (SRCBLT) mode.
	 *
	*	raxis.lim (2010/05/28)
	 *	[TODO] need some code to convert 8bpp or 16bpp surface to 32bpp ????
	 */
	if ( param->op_mode == LX_GFX_OP_MODE_TWO_SRC_PORT ||
		 param->op_mode == LX_GFX_OP_MODE_THREE_SRC_PORT ||
		 param->op_mode == LX_GFX_OP_MODE_PACK_Y_CbCr ||
		 param->op_mode == LX_GFX_OP_MODE_PACK_Y_Cb_Cr ||
		 param->op_mode == LX_GFX_OP_MODE_PACK_Cb_Cr )
	{
		b_use_iport1 = TRUE;

		iport1->sPort 	= ePort1;
		iport1->sEndian	= eLITTLE;

		if ( param->src1.surface_type == LX_GFX_SURFACE_TYPE_COLOR )
		{
			iport1->usStride		= 0x0;
			iport1->ulAddr			= 0x0;
			iport1->sFmt			= LX_GFX_PIXEL_FORMAT_INDEX_0;
			iport1->uiGlobalAlpha 	= param->src1.surface_color.pixel_value;
		}
		else
		{
			iport1->usStride		= (UINT16)src1_surface->surf.stride;
			iport1->ulAddr			= GFX_GetPortBaseAddr( src1_surface, &param->src1.rect );
			iport1->sFmt 			= src1_surface->surf.pixel_format;
			iport1->uiGlobalAlpha 	= 0xffffffff;

			/* download palette if surface is index color format */
			if( src1_surface->surf.pixel_format <= LX_GFX_PIXEL_FORMAT_INDEX_8)
			{
				if( src1_surface->pal )
				{
					GFX_DownloadPalette( 1 , src1_surface->PalSize, src1_surface->pal);
				}
			}
		}

		iport1->bBitMaskEnable 	= FALSE;
		iport1->bCOCEnable 		= FALSE;
		iport1->bColorKeyEnable = FALSE;
		iport1->bCSCEnable 		= FALSE;

		/* enable/disable premulitication of source. */
		if ( param->src1.port_flag & LX_GFX_PORT_FLAG_PREMULTIPLY )
		{
			fSrc1Premultiply = TRUE;
		}

		/* enable/disable colorkey when input color is in inside color key range. */
		if ( param->src1.port_flag & LX_GFX_PORT_FLAG_COLORKEY_INSIDE )
		{
			iport1->bColorKeyEnable = 0x1;
			iport1->bColorKeyMode 	= 0x0;
		}
		/* enable/disable colorkey when input color is outside color key range. */
		else if ( param->src1.port_flag & LX_GFX_PORT_FLAG_COLORKEY_OUTSIDE )
		{
			iport1->bColorKeyEnable = 0x1;
			iport1->bColorKeyMode	= 0x1;
		}

		/* enable/disable bitmask color */
		if ( param->src1.port_flag & LX_GFX_PORT_FLAG_BITMASK )
		{
			iport1->bBitMaskEnable 	= 0x1;
			iport1->ulBitMask 		= param->src1.bitmask;
		}

		if ( param->src1.port_flag & LX_GFX_PORT_FLAG_COC_CTRL )
		{
			iport1->bCOCEnable		= 0x1;
			memcpy( &iport1->ulCOCCtrl, &param->src1.coc_ctrl, 0x4 );
		}

		if ( param->src1.port_flag & LX_GFX_PORT_FLAG_CSC_CTRL )
		{
			iport1->bCSCEnable		= 0x1;
			iport1->sCSCSel			= param->src1.csc_ctrl;
		}

		iport1->ulReplaceColor 	= param->src1.colorkey_tgt_color;
		iport1->ulKeyLow 		= param->src1.colorkey_lower_limit;
		iport1->ulKeyHigh 		= param->src1.colorkey_upper_limit;
	}

	/*	parse src2 port configuration
	 *	-- 	alomost GFX operations doesn't needs read2 port (src2) except OP_MODE_THREE_SRC_PORT, OP_MODE_PACK_Y_Cb_Cr
	 *		and OP_MODE_PACK_Cb_Cr
	 *
	 *	raxis.lim (2010/05/28)
	 *	[TODO] need some code to convert 8bpp or 16bpp surface to 32bpp ????
	 *
	 */
	if ( param->op_mode == LX_GFX_OP_MODE_THREE_SRC_PORT ||
		 param->op_mode == LX_GFX_OP_MODE_PACK_Y_Cb_Cr ||
		 param->op_mode == LX_GFX_OP_MODE_PACK_Cb_Cr )
	{
		b_use_iport2 = TRUE;

		iport2->sPort 	= ePort2;
		iport2->sEndian	= eLITTLE;

		if ( param->src2.surface_type == LX_GFX_SURFACE_TYPE_COLOR )
		{
			iport2->usStride		= 0x0;
			iport2->ulAddr			= 0x0;
			iport2->sFmt			= LX_GFX_PIXEL_FORMAT_INDEX_0;
			iport2->uiGlobalAlpha 	= param->src2.surface_color.pixel_value;
		}
		else
		{
			iport2->usStride		= (UINT16)src2_surface->surf.stride;
			iport2->ulAddr			= GFX_GetPortBaseAddr( src2_surface, &param->src2.rect );
			iport2->sFmt 			= src2_surface->surf.pixel_format;
			iport2->uiGlobalAlpha 	= 0xffffffff;
		}
	}

#if 0
	/* get current dst port configuration */
	GFX_GetOutputConfigure(oport);
#endif

	/*	parse dst port configuration
	 *
	 *	-- dst port should always be surface !!
	 **/
	{
		if ( param->dst.port_flag & LX_GFX_PORT_FLAG_DEMULTIPLY )
		{
			fDstDemultiply = TRUE;
		}

		if ( param->dst.port_flag & LX_GFX_PORT_FLAG_COC_CTRL )
		{
			oport->bCOCEnable = 0x1;
			memcpy( &oport->ulCOCCtrl, &param->dst.coc_ctrl, 0x4 );
		}

		if ( param->dst.port_flag  & LX_GFX_PORT_FLAG_CSC_CTRL )
		{
			oport->bCSCEnable		= 0x1;

			/* coefficient for YUV -> RGB */
			if ( param->dst.csc_ctrl == LX_CSC_COEF_SEL_0 || param->dst.csc_ctrl == LX_CSC_COEF_SEL_1 )
			{
				oport->uiCSCCoef[0] = 0x129FFC98;
				oport->uiCSCCoef[1] = 0xF775129F;
				oport->uiCSCCoef[2] = 0x21D70000;
				oport->uiCSCCoef[3] = 0x129F0000;
				oport->uiCSCCoef[4] = 0x1CB00000;
				oport->uiCSCCoef[5] = 0xFFF0FF80;
				oport->uiCSCCoef[6] = 0xFF800000;
				oport->uiCSCCoef[7] = 0x00000000;
			}
			/* coefficient for RGB -> YUV */
			else
			{
				oport->uiCSCCoef[0] = 0x09D200FD;
				oport->uiCSCCoef[1] = 0x02EDFA98;
				oport->uiCSCCoef[2] = 0x0706FE63;
				oport->uiCSCCoef[3] = 0xF99EFF5D;
				oport->uiCSCCoef[4] = 0x07060000;
				oport->uiCSCCoef[5] = 0x00000000;
				oport->uiCSCCoef[6] = 0x00000010;
				oport->uiCSCCoef[7] = 0x00800080;
			}
		}
		else /* no CSC. bypass ? */
		{
			oport->uiCSCCoef[0]		= 0x10000000;
			oport->uiCSCCoef[1]		= 0x00000000;
			oport->uiCSCCoef[2]		= 0x10000000;
			oport->uiCSCCoef[3]		= 0x00000000;
			oport->uiCSCCoef[4]		= 0x10000000;
			oport->uiCSCCoef[5]		= 0x00000000;
			oport->uiCSCCoef[6]		= 0x00000000;
			oport->uiCSCCoef[7]		= 0x00000000;
		}

 		/* [TODO] need to verify oport configuration */
		oport->usStride = (UINT16)dst_surface->surf.stride;

		oport->ulAddr	= GFX_GetPortBaseAddr( dst_surface, &param->dst.rect );
		oport->usHSize 	= param->dst.rect.w;		// sub region display
		oport->usVSize 	= param->dst.rect.h;

		if ( dst_surface->surf.pixel_format == LX_GFX_PIXEL_FORMAT_CbCr_420 )
		{
			oport->usHSize 	*= 2;
			oport->usVSize 	*= 2;
		}
		if ( dst_surface->surf.pixel_format == LX_GFX_PIXEL_FORMAT_CbCr_422 )
		{
			oport->usHSize 	*= 2;
		}

		/* if current OP mode is 'WRITE ONLY', set pixel format and global alpha based on src0 port */
		if ( param->op_mode == LX_GFX_OP_MODE_WRITE )
		{
			oport->sFmt 	= LX_GFX_PIXEL_FORMAT_INDEX_0;
			oport->ulGAlpha = param->src0.surface_color.pixel_value;
		}
		else
		{
			oport->sFmt 	= dst_surface->surf.pixel_format;
			oport->ulGAlpha = 0xFFFFFFFF;
		}

		oport->sEndian	= eLITTLE;
	}

	/* configure operation */
#if 0
	GFX_GetOperationStatus(operate);
#endif
	operate->sPort = param->op_mode;
	operate->usCoef[0] = 0x080;		/* test value ? */
	operate->usCoef[1] = 0x0C0;		/* test value ? */
	operate->usCoef[2] = 0x100;		/* test value ? */
	operate->usCoef[3] = 0x040;		/* test value ? */

	/* configure blend parameter */
#if 0
	GFX_GetBlendConfigure(blend);
#endif

	/* set blend param only for BLEND request */
	blend->sOut 		= param->out_sel;
	blend->sRaster 		= param->rop;

	/* blend factor is valid only for BLEND mode */
	if ( param->out_sel == LX_GFX_OUT_SEL_BLEND )
	{
		blend->bPMA0Enable	= fSrc0Premultiply;		/* src0 port should be premultiplied ? */
		blend->bPMA1Enable	= fSrc1Premultiply;		/* src1 port should be premultiplied ? */
		blend->bXOR0Enable	= param->xor_color_en;
		blend->bXOR1Enable	= param->xor_alpha_en;
		blend->bDIVEnable	=  fDstDemultiply;		/* dst port should be demultiplied ? */
		blend->usAlpha_m0	= param->mux_alpha_m0;
		blend->usC_m0		= param->mux_color_m0;
		blend->sAlphaBlend	= param->comp_rule_alpha;
		blend->sRBlend		= param->comp_rule_color;
		blend->sGBlend		= param->comp_rule_color;
		blend->sBBlend		= param->comp_rule_color;

		blend->usA0_Alpha 	= param->mux_factor0_alpha;
		blend->usB0_Alpha 	= param->mux_factor1_alpha;
		blend->usA1_R		= param->mux_factor0_color;
		blend->usA2_G		= param->mux_factor0_color;
		blend->usA3_B		= param->mux_factor0_color;
		blend->usB1_R		= param->mux_factor1_color;
		blend->usB2_G 		= param->mux_factor1_color;
		blend->usB3_B 		= param->mux_factor1_color;

		blend->ulBlendConstant = param->constant_color;
	}

	GFX_SetInputConfigure(iport0);
	if (b_use_iport1) GFX_SetInputConfigure(iport1);
	if (b_use_iport2) GFX_SetInputConfigure(iport2);

	GFX_SetOutputConfigure(oport);
	GFX_SetBlendConfigure(blend);
	GFX_SetOperationStatus(operate);

	memset( scaler, 0x0, sizeof(GFX_SCALER_CONFIGURE_T));

	/* make scaler command after the normal GFX command set */
	if ( param->scaler.mode == LX_GFX_SCALER_MODE_NONE )
	{
		scaler->cmd = GFX_SCALER_CMD_BYPASS;
		GFX_SetScalerConfigure( scaler );
	}
	else
	{
		param->dst.rect.w	= param->src0.rect.w;
		param->dst.rect.h	= param->src0.rect.h;

		scaler->cmd 		= GFX_SCALER_CMD_START;
		scaler->filter 		= GFX_SCALER_FILTER_CUSTOM;
		scaler->in_dim.w	= param->scaler.input.w;
		scaler->in_dim.h	= param->scaler.input.h;
		scaler->out_dim.w	= param->scaler.output.w;
		scaler->out_dim.h	= param->scaler.output.h;

		GFX_SetScalerConfigure( scaler );
	}

	/* set command delay for every gfx START command */
	GFX_SetCommandDelay( cmd_delay );

	/* raxis.lim (2010/05/25)
	 * GFX START command should be issued regardless of batch run mode.
	 * Application controls only "Batch Run" (flush) command.
	 */
	GFX_RunStartCommand();

func_exit:
	if(iport0)	OS_Free(iport0);
	if(iport1)	OS_Free(iport1);
	if(iport2)	OS_Free(iport2);
	if(oport)	OS_Free(oport);
	if(operate)	OS_Free(operate);
	if(scaler)	OS_Free(scaler);
	if(blend)	OS_Free(blend);

	GFX_TRACE_END();

	return ret;
}

/** dump blend param for debug
 *
 * @param param [IN] GFX blend param to request
 *
 */
void	GFX_DumpHWOpParam(LX_GFX_MANUAL_BLEND_CTRL_PARAM_T *param, BOOLEAN bForce )
{
	if ( bForce ||  g_gfx_hw_op_log>0 )
	{
		GFX_ERROR( "[r0] 0x%08x,0x%x,0x%04x,0x%08x (%4d,%4d,%4d,%4d) [r1] 0x%08x,0x%x,0x%04x,0x%08x (%4d,%4d,%4d,%4d) "
				   "[wr] 0x%08x,0x%x,0x%04x,0x%08x (%4d,%4d,%4d,%4d) [op] %d %d 0x%08x [sc] %d (%4d,%4d) (%4d,%4d)\n",
			param->src0.port_flag,param->src0.surface_type, param->src0.surface_fd,   param->src0.surface_color.pixel_value,
			param->src0.rect.x,   param->src0.rect.y,       param->src0.rect.w,       param->src0.rect.h,
			param->src1.port_flag,param->src1.surface_type, param->src1.surface_fd,   param->src1.surface_color.pixel_value,
			param->src1.rect.x,   param->src1.rect.y,       param->src1.rect.w,       param->src1.rect.h,
			param->dst.port_flag, param->dst.surface_type,  param->dst.surface_fd,    param->dst.surface_color.pixel_value,
			param->dst.rect.x,    param->dst.rect.y,        param->dst.rect.w,        param->dst.rect.h,
			param->op_mode, param->out_sel, param->constant_color, param->scaler.mode,
			param->scaler.input.w, param->scaler.input.h, param->scaler.output.w,param->scaler.output.h );
	}
}

/**
 *
 * color space setting on out port
 *
 * @param	UINT8 *coef
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_SetColorSpace(UINT32 *coef)
{
	GFX_OUT_CONFIGURE_T *oport;
	int ret = 0;
	int i;

	GFX_TRACE_BEGIN();
	oport = (GFX_OUT_CONFIGURE_T *)kmalloc(sizeof(GFX_OUT_CONFIGURE_T), GFP_KERNEL);

	GFX_GetOutputConfigure(oport);

	for(i=0 ; i<8 ;i++)
		oport->uiCSCCoef[i] = *coef++;

	GFX_SetOutputConfigure(oport);
	GFX_TRACE_END();

	kfree(oport);

	return ret;
}

/**
 *
 * get command delay parameter which mean the execution time gap between two commands
 *
 * @param	UINT32 *cmd_delay
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_GetCommandDelay(UINT32 *cmd_delay)
{
	int ret = 0;

	GFX_TRACE_BEGIN();
	*cmd_delay = GFX_GetCommandDelayReg();
	GFX_TRACE_END();

	return ret;
}


/**
 *
 * set command delay parameter which mean the execution time gap between two commands
 *
 * @param	UINT32 cmd_delay
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_SetCommandDelay(UINT32 cmd_delay)
{
	int ret = 0;

	/* override the request command delay value for test */
	if(g_gfx_cmd_delay_test_value >= 0x0)
	{
		cmd_delay = g_gfx_cmd_delay_test_value;
	}

	GFX_SetCommandDelayReg((UINT16)(cmd_delay));

	return ret;
}


/** set command run type
 *
 * @param	UINT32 LX_GFX_BATCH_RUN_MODE_T
 * @return	int	0 : OK , -1 : NOT OK
 *
 */
int GFX_SetRunCommand(LX_GFX_BATCH_RUN_MODE_T *cmd)
{
	int ret = RET_OK;

	GFX_TRACE_BEGIN();
	g_gfx_run_mode = *cmd;
	GFX_TRACE_END();

	return ret;
}

int	GFX_SetGraphicSyncMode( LX_GFX_GRAPHIC_SYNC_MODE_T	mode)
{
	int ret = RET_OK;
	GFX_TRACE_BEGIN();
	g_gfx_sync_mode = mode;
	GFX_TRACE_END();

	return ret;
}

LX_GFX_GRAPHIC_SYNC_MODE_T	GFX_GetGraphicSyncMode(void)
{
	return g_gfx_sync_mode;
}

/** get command run type
 *
 * @param	void
 * @return	LX_GFX_BATCH_RUN_MODE_T
 *
 */
LX_GFX_BATCH_RUN_MODE_T GFX_GetRunCommand(void)
{
	return( g_gfx_run_mode );
}

/** calculate GFX sync timeout with very heuristic method.
 *
 *	default timeout value will be enough for almost GFX operation.
 *	but some big image requires more timeout value.
 */
UINT32		GFX_CalcSyncTimeout         ( LX_GFX_MANUAL_BLEND_CTRL_PARAM_T* blend_ctrl, UINT32 def_tm_out )
{
	SINT16	src_w, src_h, dst_w, dst_h;
	UINT32	src_area, dst_area;

	if ( blend_ctrl->scaler.mode == LX_GFX_SCALER_MODE_NONE )
	{
		src_w = 0;
		src_h = 0;
		dst_w = blend_ctrl->dst.rect.w;
		dst_h = blend_ctrl->dst.rect.h;
	}
	else
	{
		src_w = blend_ctrl->scaler.input.w;
		src_h = blend_ctrl->scaler.input.h;
		dst_w = blend_ctrl->scaler.output.w;
		dst_h = blend_ctrl->scaler.output.h;
	}

	src_area = src_w * src_h;
	dst_area = dst_w * dst_h;

#if 0
	if ( src_area <= 0xe1000 && dst_area <= 0xe1000 )	{ /* do noting */ }
	else												{ printk("[gfx] tm calc : 1 - %dx%d - %dx%d\n", src_w, src_h, dst_w, dst_h ); }
#endif

	/* 0xe1000 means 1280*720 */
	if ( src_area <= 0xe1000 && dst_area <= 0xe1000 )	return def_tm_out;
	else												return (def_tm_out<<2);
}

/**	wait until gfx core completes all CQs ( action handler for GFX_IOW_WAIT_FOR_SYNC )
 *	-- 	this function will be invoked by application since application shouild wait for
 *		GFX sync to access surface memory by CPU operation.
 *	--	application can call this function multiple times regardless of the real request count.
 *	--	driver should return this function when GFX core is idle ( all CQs are processed ),
 *		otherwise driver should block application for msec_tm interval.
 *
 *	raxis.lim (2010/05/22) -- bug fix
 *	--	the objective of this function is not holding/resuming GFX operation
 *		function code should be re-implemented !!
 *
 *	raxis.lim (2010/05/27) -- more revision
 *	--	I decided not to use giGfxIntDone, instead read GFX register.
 *		register itself is the exact status information.
 *	--	GFX_IsGfxIdle() function created to hide the decision algorithm.
 *	--	more analysis is needed if decision algorithm is 100% correct or not.
 *	-- wait_event_xxx function may return RET_ERROR or RET_TIMEOUT
 *
 * @param	void
 * @return	int	0 : OK , -1 : NOT OK
 */
int GFX_WaitSyncCommand(UINT32 msec_tm )
{
	int	rc;
//	UINT32	tk0 = OS_GetHighResTicks();
	rc = wait_event_interruptible_timeout(g_gfx_wq_done, GFX_IsGfxIdle(), msecs_to_jiffies(msec_tm) );
	GFX_ASSERT( rc > 0 );
//	printk("<gfx> %s : rc = %d. us = %d\n", __F__, rc, (UINT32)OS_GetHighResTimeElapsed(tk0)/1000 );

	/* print warning message when timeout detected. 
	 * it's very critical error for GFX h/w and 'reset' recovery should be executed.
	 *
	 * raxis.lim (2012/12/27)
	 * Reset GFX HW after every GFX operation, this will prevent the possible GFX invalid output.
	 * this is very rare case, but it's very critical cuz all GFX output image will be wrong.
	 */
	if ( rc <= 0 )
	{
		int	i;
		GFX_CMD_QUEUE_CTRL_T st;
		GFX_GetComQueueStatus(&st);
		GFX_ERROR("gfx sync timeout. status=%d-%d-%d\n", st.bStatus, st.bCmdQStatus, st.bFull );

		GFX_StopCommand( );

		for ( i=100; i>0; i-- )
		{
			if ( GFX_IsGfxIdle() ) break;
			OS_MsecSleep(1); 
		}
		if ( i == 0 ) GFX_ERROR("crtical error !! GFX recovery failed !!\n"); 
	}
	else
	{
		GFX_StopCommand( );
	}	

	/* the caller should check return value and do some recovery based on return value.
	 * for example, if return value is RET_ERROR, the caller should retry rendering again
	 */
	return (rc>0)? RET_OK: RET_ERROR;
}

/** wake up fucntion
 *
 * @param	void
 * @return	int	0 : OK , -1 : NOT OK
 */
int GFX_WakeupWaitSync(void)
{
	if ( GFX_IsGfxIdle() ) wake_up_interruptible(&g_gfx_wq_done);
	return RET_OK;
}

/** get bits per pixel information from GFX pixel format
 *
 *	@param pxl_fmt [IN] GFX pixel format 
 *	return bits per pixel 
 */
UINT32	GFX_PxlFmt2Bpp( LX_GFX_PIXEL_FORMAT_T pxl_fmt )
{
	switch( pxl_fmt )
	{
//		case LX_GFX_PIXEL_FORMAT_INDEX_0:

		case LX_GFX_PIXEL_FORMAT_INDEX_1: 		return 1;
		case LX_GFX_PIXEL_FORMAT_INDEX_2: 		return 2;
		case LX_GFX_PIXEL_FORMAT_INDEX_4: 		return 4;
		case LX_GFX_PIXEL_FORMAT_INDEX_8:
    	case LX_GFX_PIXEL_FORMAT_ALPHA_8:

   		case LX_GFX_PIXEL_FORMAT_Cb8_420__Cr8_420:	
    	case LX_GFX_PIXEL_FORMAT_Cb8_422__Cr8_422:
    	case LX_GFX_PIXEL_FORMAT_Y8__Cb8_444__Cr8_444:
												return 8;

		case LX_GFX_PIXEL_FORMAT_CbCr_420:
		case LX_GFX_PIXEL_FORMAT_CbCr_422:
		case LX_GFX_PIXEL_FORMAT_CbCr_444:

    	case LX_GFX_PIXEL_FORMAT_YCbCr655:
		case LX_GFX_PIXEL_FORMAT_AYCbCr2644:
		case LX_GFX_PIXEL_FORMAT_AYCbCr4633:
		case LX_GFX_PIXEL_FORMAT_AYCbCr6433:

		case LX_GFX_PIXEL_FORMAT_RGB565:
		case LX_GFX_PIXEL_FORMAT_ARGB1555:
		case LX_GFX_PIXEL_FORMAT_ARGB4444:
		case LX_GFX_PIXEL_FORMAT_ARGB6343:	return 16;

		case LX_GFX_PIXEL_FORMAT_Y0Cb0Y1Cr0_422:
		case LX_GFX_PIXEL_FORMAT_AYCbCr8888:
		case LX_GFX_PIXEL_FORMAT_ARGB8888:
		default:						 	return 32;
	}
}

/** get the physical base address for GFX operation.
 *
 *	@param surface [IN] GFX surface handle
 *	@param rect [IN] rendering area 
 *	@return physical address for GFX operation
 */
UINT32	GFX_GetPortBaseAddr( GFX_SURFACE_OBJ_T* surface, LX_RECT_T* rect )
{
	UINT32	base_addr;
	int		x = rect->x;
	int		y = rect->y;

	base_addr  = (UINT32)surface->surf.phys_addr + y * surface->surf.stride;
	base_addr += x * ( GFX_PxlFmt2Bpp( surface->surf.pixel_format ) >> 3 );

	return base_addr;
}

/*========================================================================================
	Implementation Group (HW HAL)
========================================================================================*/
int GFX_InitHW  (void)
{
    return g_gfx_hal.InitHW( );
}

int GFX_ShutdownHW ( void )
{
	return g_gfx_hal.ShutdownHW( );
}

int GFX_RunFlushCommand(void)
{
	return g_gfx_hal.RunFlushCommand( );
}

int GFX_RunStartCommand(void)
{
	return g_gfx_hal.RunStartCommand( );
}

int GFX_PauseCommand(void)
{
	return g_gfx_hal.RunPauseCommand( );
}

int GFX_ResumeCommand(void)
{
	return g_gfx_hal.RunResumeCommand( );
}

int GFX_StopCommand(void)
{
	return g_gfx_hal.RunStopCommand( );
}

int GFX_SwResetCommand(void)
{
	return g_gfx_hal.RunSwResetCommand();
}

int GFX_RunClkOnCommand(void)
{
	GFX_CHECK_CODE( GFX_ClkCheck() == TRUE, return RET_OK, "ignore multiple CLK_ON req\n");

	return g_gfx_hal.RunClkOnCommand();
}

int GFX_RunClkOffCommand(void)
{
	GFX_CHECK_CODE( GFX_ClkCheck() == FALSE, return RET_OK, "ignore multiple CLK_OFF req\n");
	return g_gfx_hal.RunClkOffCommand();
}

BOOLEAN	GFX_ClkCheck(void)
{
	return g_gfx_hal.ClkCheck( );
}

void GFX_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue)
{
	g_gfx_hal.GetComQueueStatus(queue);
}

BOOLEAN GFX_IsGfxIdle(void)
{
	return g_gfx_hal.IsGfxIdle( );
}

void GFX_GetOperationStatus(GFX_CMD_OP_T *operate)
{
	g_gfx_hal.GetOperationStatus(operate);
}

void GFX_SetOperationStatus(GFX_CMD_OP_T *operate)
{
	g_gfx_hal.SetOperationStatus(operate); 
}

void GFX_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port)
{
	g_gfx_hal.GetInputConfigure( iPort, port );
}

void GFX_SetInputConfigure(GFX_PORT_CONFIGURE_T *port)
{
	g_gfx_hal.SetInputConfigure(port);
}

void GFX_SetBlendingOut(GFX_ENUM_OUT_T type)
{
	g_gfx_hal.SetBlendingOut(type);
}

void GFX_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{
	g_gfx_hal.GetBlendConfigure(blend);
}

void GFX_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{
	g_gfx_hal.SetBlendConfigure(blend);
}

void GFX_GetOutputConfigure( GFX_OUT_CONFIGURE_T *port)
{
	g_gfx_hal.GetOutputConfigure(port);
}

void GFX_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port)
{
	g_gfx_hal.SetOutputConfigure(port);
}

void GFX_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler)
{
	if ( g_gfx_hal.SetScalerConfigure ) g_gfx_hal.SetScalerConfigure(scaler);
}

int GFX_SetCLUT(int port, int size , UINT32 *data)
{
	return g_gfx_hal.SetCLUT(port,size,data);
}

void GFX_SetCommandDelayReg(UINT16 delay)
{
	g_gfx_hal.SetCommandDelayReg(delay);
}

UINT16 GFX_GetCommandDelayReg(void)
{
	return g_gfx_hal.GetCommandDelayReg( );
}

void GFX_DumpRegister(void)
{
	return g_gfx_hal.DumpRegister( );
}

int GFX_RunSuspend(void)
{
	return g_gfx_hal.RunSuspend( );
}

int GFX_RunResume(void)
{
#ifdef GFX_IGNORE_DUPLICATE_PALTTE
	g_pal_init_0 = FALSE;
	g_pal_init_1 = FALSE;
	memset( g_pal_data_0, 0x0, sizeof(UINT32)* 256 );
	memset( g_pal_data_1, 0x0, sizeof(UINT32)* 256 );
#endif
	return g_gfx_hal.RunResume( );
}

/** @} */

