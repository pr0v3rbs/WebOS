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
 *  @date		2014-03-01
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
#include "fbdev_hw_h15.h"
#include "fbdev_reg_h15.h"

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
extern  volatile OSD_H15_REG_T*     g_osd_h15_reg;
extern  OSD_H15_REG_T*              g_osd_h15_reg_cached;
#if FBDEV_H15_NOT_IMPL("not implemented now. MIF IF")
extern  volatile MIF_OSD_H15_REG_T* g_mif_osd_h15_reg;
extern  MIF_OSD_H15_REG_T*          g_mif_osd_h15_reg_cached;
#endif

extern	int							g_fbdev_use_isr_mon;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#ifdef FBDEV_USE_FRC_IF_CTRL
static UINT32	FBDEV_H15CMN_RemapFRCHeader ( UINT32 in );
#endif

static void		FBDEV_H15CMN_ReInitHW		(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DEFINE_SPINLOCK(g_osd_hdr_spinlock);

static int 	_g_fbdev_h15_v_phase_offset		= 0x27;	/* determined by test. used when reverse mode enabled */
module_param_named( fbdev_h15_v_phase_offset, _g_fbdev_h15_v_phase_offset, int, 0644 );

#if FBDEV_H15_NOT_IMPL("not implemented now. MIF IF")
static int 	_g_fbdev_h15_mrb_bank_mode		= 0x0;	/* H15 H/W default. TODO: read the default value from H/W */
static int 	_g_fbdev_h15_mrb_bank_mode_req	= 0x0;
module_param_named( fbdev_h15_mrb_bank_mode, _g_fbdev_h15_mrb_bank_mode_req, int, 0644 );
#endif

/*----------------------------------------------------------------------------------------
	Static Functions
----------------------------------------------------------------------------------------*/
int	FBDEV_H15CMN_GetHWXList( int fbdev_id, int xlst[2] )
{
	int	num_xlst = 0;

	if( g_fbdev_cfg->uhd_disp_mode )
	{
	    switch( fbdev_id )
		{
			case LX_FBDEV_ID_OSD0:	num_xlst=2; xlst[0] = 0; xlst[1] = 1; break;
			case LX_FBDEV_ID_OSD1:  num_xlst=2; xlst[0] = 2; xlst[1] = 3; break;
			case LX_FBDEV_ID_CSR0:  num_xlst=1; xlst[0] = 4; xlst[1] =-1; break;
			default:				num_xlst=0; xlst[0] =-1; xlst[1] =-1; break;
		}
	}
	else
	{
	    switch( fbdev_id )
		{
			case LX_FBDEV_ID_OSD0:	num_xlst=1; xlst[0] = 0; xlst[1] = -1; break;
			case LX_FBDEV_ID_OSD1:  num_xlst=1; xlst[0] = 1; xlst[1] = -1; break;
			case LX_FBDEV_ID_CSR0:  num_xlst=1; xlst[0] = 3; xlst[1] = -1; break;
			default:				num_xlst=0; xlst[0] =-1; xlst[1] = -1; break;
		}
	}

	return num_xlst;
}

/** return X position value based on current video path & platform type.
 *	this function is only valid when current platform is set to UHD mode.
 *
 *	caculation algorithm
 *	- L picture : always 0
 *	- R picture : disp_width/4 if BLEND path, disp_width/2 if SEP path.
 *
 * @note disp_width/4 is not 100% corrent !!!
 *
 */
static int		FBDEV_H15CMN_GetXPosRefValue	( int fb_dev_id, UINT32 screen_no, UINT32 disp_width )
{
	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

	FBDEV_CHECK_CODE(!g_fbdev_cfg->uhd_disp_mode, return 0, "not UHD disp(0x%x)\n", g_fbdev_cfg->uhd_disp_mode);

	#if 0
	if( hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	#else
	if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	#endif
	{
		/* when disp_width 3840, return L(0), R(960) */
		return (disp_width>>2)*screen_no;
	}
	else /* SEP */
	{
		/* when disp_width 3840, return L(0), R(1920) */
		return (disp_width>>1)*screen_no;
	}
}

static BOOLEAN	FBDEV_H15CMN_IsReverseOSD	( int fb_dev_id )
{
	/* @note how to check reverse OSD mode ? */
	return FALSE;
}

/** clear OSD region at the initialization stage.
 *
 */
#if FBDEV_H15_NOT_IMPL("not implemented now. 3D & MIF IF")
static void FBDEV_H15CMN_FillOSDRegion( int fb_dev_id, UINT32 color )
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
#endif

/** enable OSD hdr flush
 *
 */
static void FBDEV_H15CMN_EnableOSDHdrFlush( ULONG data )
{
    int i;
#if (CONFIG_LX_BOOT_LOGO==1)
	int	j;
	int	xidx;
	int xnum, xlst[2];

	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

    FBDEV_WARN("enable osd_hdr_flush & stop OSD freeze stage\n");

	/* enable osd_hdr_flush to unfreeze OSD */
    for (i=0; i<FBDEV_MAX_DEVICE; i++)
    {
        FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(i);

		FBDEV_CHECK_CODE(NULL==wctx, continue, "invalid fb(%d) ctx\n", i );

		/* if freeize is already disabled, ignore osd hdr flush */
		FBDEV_DEBUG("fb(%d) osd_hdr_flush status %d\n", i, wctx->status.b_osd_hdr_flush );
		if(wctx->status.b_osd_hdr_flush) continue;

		wctx->status.b_osd_hdr_flush = TRUE;
#ifdef FBDEV_H15_KEEP_LOGO_LAYER
		if (i==CONFIG_LX_BOOT_LOGO_FBDEV) { FBDEV_WARN("fb(%d) keep current status (LOGO)\n", i ); continue; }
#endif
		/* write OSD hdr again to update screen */
		xnum = FBDEV_H15CMN_GetHWXList(i, xlst);
		if(INVALID_XNUM(xnum)) continue;

		FBDEV_WARN("fb(%d) flush OSD header\n", i);

		for(j=0;j<xnum;j++)
		{
			xidx = xlst[j];

#ifdef FBDEV_H15_USE_HDR_RDY_REG
			g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_main.osd_hdr_ready_flag = 0x1;
#elif defined FBDEV_H15_SW_HDR_RDY_CTRL
			atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[xidx].osd_hdr_ready, 0x1);
#endif
		}
    }
#endif /* CONFIG_LX_BOOT_LOGO */
}

/**
 *
 */
void FBDEV_H15CMN_RunBootLogoTimer( UINT32 ms, BOOLEAN bForce )
{
#if (CONFIG_LX_BOOT_LOGO==1)
	static BOOLEAN		b_timer_req = TRUE;
	static OS_TIMER_T	osd_en_timer;

	if ( b_timer_req || bForce )
	{
		b_timer_req = FALSE;
		FBDEV_WARN("start boot timer %d ms (%d)\n", ms, bForce );

		OS_InitTimer ( &osd_en_timer );
		OS_StartTimer( &osd_en_timer, FBDEV_H15CMN_EnableOSDHdrFlush, OS_TIMER_TIMEOUT, ms, 0x0 );
	}
#else
	FBDEV_H15CMN_EnableOSDHdrFlush(0x0);
#endif
}

/*========================================================================================
    Implementation Group (OSD HW Initialization)
========================================================================================*/

/** initialize HW SOC
 *
 *
 */
int	FBDEV_H15CMN_InitHW ( void )
{
	int	i;
	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

	/* fetch the current HW disp size */
	UINT32	disp_width = hw_ctx->hw_disp_width;
	UINT32	disp_height= hw_ctx->hw_disp_height;

	FBDEV_DEBUG("(Re)Init OSD(H15) HW.. disp_size %dx%d\n", disp_width, disp_height);

	/* initialize S.OSD & B.OSD mixer disp size */
	OSDX_H15_RdFL(osdx_sep_osd_mixer[0], ctrl_sosd_mixer_disp_size);
	OSDX_H15_Wr02(osdx_sep_osd_mixer[0], ctrl_sosd_mixer_disp_size,		reg_sosd_disp_size_w, hw_ctx->sosd_disp.width,
																		reg_sosd_disp_size_h, hw_ctx->sosd_disp.height);
	OSDX_H15_WrFL(osdx_sep_osd_mixer[0], ctrl_sosd_mixer_disp_size);

	OSDX_H15_RdFL(osdx_blend_osd_mixer[0], ctrl_bosd_mixer_disp_size);
	OSDX_H15_Wr02(osdx_blend_osd_mixer[0], ctrl_bosd_mixer_disp_size,	reg_sosd_disp_size_w, hw_ctx->bosd_disp.width,
																		reg_sosd_disp_size_h, hw_ctx->bosd_disp.height);
	OSDX_H15_WrFL(osdx_blend_osd_mixer[0], ctrl_bosd_mixer_disp_size);

	OSDX_H15_RdFL(osdx_blend_osd_mixer[1], ctrl_bosd_mixer_disp_size);
	OSDX_H15_Wr02(osdx_blend_osd_mixer[1], ctrl_bosd_mixer_disp_size,	reg_sosd_disp_size_w, hw_ctx->bosd_disp.width,
																		reg_sosd_disp_size_h, hw_ctx->bosd_disp.height);
	OSDX_H15_WrFL(osdx_blend_osd_mixer[1], ctrl_bosd_mixer_disp_size);

	/* setup all OSD common disp size */
	for (i=0; i< FBDEV_H15_MAX_OSD_NUM /*8*/; i++)
	{
		OSD_H15_RdFL(osdx_common_disp_size[i]);
		OSD_H15_Wr02(osdx_common_disp_size[i], 	reg_common_disp_size_w, disp_width,
								  				reg_common_disp_size_h, disp_height );
		OSD_H15_WrFL(osdx_common_disp_size[i]);
	}

    /* setup the default layer mux. layer order = <top> 7, 6, 5, OSD4(crsr old), OSD3(crsor new)OSD0, OSD1, OSD2 <bottom> */
	{
		UINT32 mixer_mux_value = 0x76543012;
		FBDEV_WARN("(Re)Init OSD(H15) mixer mux order = 0x%08x\n", mixer_mux_value );

		OSDX_H15_Wr(osdx_sep_osd_mixer[0], 		ctrl_sosd_mixer_mux1, mixer_mux_value);
		OSDX_H15_WrFL(osdx_sep_osd_mixer[0],	ctrl_sosd_mixer_mux1);

	    OSDX_H15_Wr(osdx_blend_osd_mixer[0],	ctrl_bosd_mixer_mux1, mixer_mux_value);
		OSDX_H15_WrFL(osdx_blend_osd_mixer[0],	ctrl_bosd_mixer_mux1);

		OSDX_H15_Wr(osdx_blend_osd_mixer[1],	ctrl_bosd_mixer_mux1, mixer_mux_value);
		OSDX_H15_WrFL(osdx_blend_osd_mixer[1],	ctrl_bosd_mixer_mux1);
	}

	/* raxis.lim (2014/11/11) tune osd sync pulse timer
	 * CCO_CTRL.CTRL_SOSD_PLUS.reg_disp_sosd_timer controls the delay the OSD mix start time in cycle unit.
	 * OSD HW engineer recommend to use 0x300 (almost 10 line delay) can fix OSD sync & isr mismatch issue.
	 * TODO: more analysis is required
	 */
	OS_WrReg(0xc8024034, 0x3000);

#ifdef FBDEV_H15_CPB_BW_TEST
	FBDEV_H15TEST_InitCPBTest();
#endif

	return RET_OK;
}

/** make OSD to be initial state
 *	make the basic configuration for each OSD layer.
 *
 *	@param layerId
 */
int FBDEV_H15CMN_InitOSDLayer ( int fb_dev_id )
{
	int	i;
	int	xidx;
	int xnum, xlst[2];
	int ret = RET_ERROR;

	/* please refer to OSD manual OSD?_CFG field */
	UINT32  hdr_src_sel;
	UINT32  osd_memhdr_addr;
	BOOLEAN	osd_en = 0x0;			/* off by default */
	BOOLEAN	hdr_rdy_flag = 0x0;		/* OFF by default */

	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

	/* fetch the current output disp size */
	UINT32	disp_width = hw_ctx->hw_disp_width;
	UINT32	disp_height= hw_ctx->hw_disp_height;

	xnum = FBDEV_H15CMN_GetHWXList(fb_dev_id, xlst);
	FBDEV_CHECK_CODE(INVALID_XNUM(xnum), return RET_ERROR, "invalid fb(%d)\n", fb_dev_id );

	/* hdr_src_sel 0x0: DDR, 0x1: REG */
	hdr_src_sel	= (g_reg_io_ctx->hdr_op_method == FBDEV_HDR_OP_MMIO)? 0x0: 0x1;
	osd_memhdr_addr= (gMemCfgFBHdr[fb_dev_id].mem_header_base);

	/* choose the initial osd visibility based on boot logo configuration */
	switch(fb_dev_id)
	{
#if (CONFIG_LX_BOOT_LOGO==1 && CONFIG_LX_BOOT_LOGO_FBDEV==0)
		case LX_FBDEV_ID_OSD0:	osd_en = 0x1; break;
#endif
#if (CONFIG_LX_BOOT_LOGO==1 && CONFIG_LX_BOOT_LOGO_FBDEV==1)
		case LX_FBDEV_ID_OSD1:	osd_en = 0x1; break;
#endif
		default:				osd_en = 0x0; break;
	}
#ifdef FBDEV_H15_USE_HDR_RDY_REG
	hdr_rdy_flag = 0x1;
#endif

	for (i=0;i<xnum;i++)
	{
		UINT32	disp_width_sync;

		if (INVALID_XIDX(xlst[i])) continue; /* ignore processingif xidx is invalid */

		xidx = xlst[i];

		/* raxis.lim (2014/06)
		 * if UHD(more than 3840x2160) disp mode, each OSD layer should has a half of real display size.
		 * this restruction is also applied to the cursor layer.
		 * BLEND path should be treated as UHD disp mode becuase video layer is fixed to UHD.
		 */
		#if 0
		disp_width_sync = ( hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )? disp_width/2 : disp_width;
		#else
		disp_width_sync = ( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )? disp_width/2 : disp_width;
		#endif

		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_main);
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_pdec);
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_base_addr);
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_sync0);
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_scaler);

		/* OSD HW requests to write both osd_en & osd_sync_enable to control osd visibility */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main, 	osd_en,			osd_en);
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync0,	osd_sync_enable,osd_en);

		/* raxis.lim (2014/06)
		 * if UHD disp mode, each OSD layer should has a half of real display size (S.OSD only?)
	 	 * this restriction is also applied to CRSR layer.
		 */
		#if 0
		if( hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
		#else
		if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
		#endif
		{
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync0, osd_sync_hdouble_en,	0x0 );
		}
		else
		{
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync0, osd_sync_hdouble_en,	0x1 );
		}

		/* use osd_hdr_flag to avoid the possible OSD mis-configuration when OSD header is set across vsync */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main, osd_hdr_ready_flag_en,	hdr_rdy_flag );

		/* normally osd_hdr_src_sel is fixed to "read from hw register" not "DDR contains osd_hdr data" */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main, osd_hdr_src_sel,		hdr_src_sel );

		/* we should set DDR memory to hold osd_hdr data, but NOT used in normal case */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_base_addr, 	osd_base_addr,		osd_memhdr_addr );

		#if 0 /* KEEP HW DEFAULT */
		/* set scaler sample mode to the same behavior as that of GFX */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_scaler,osd_sc_sampling_mode,	0x1 );
		#endif

		#if 0 /* KEEP HW DEFAULT */
		/* MIF feeder cfg : "10 (0x2)" L to R by default.
		 * this value has ben fixed to "10" inside OSD HW since older chip.
		 * In H15, this value can be configurable by SW.
		 *
		 * @note : MIF should be also changed ?
		 * @note : I think kdriver doesn't need to know this functionalities :)
		 */
		FBDEV_H15_NOTE("I think this value is fixed to 0x2 forever")
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main,	osd_manual_dlr,		0x2 );
		#endif

		/* setup PDEC (each pixel component (A,R,G,B) can be configured to make any pixel order combination.
		 * for example, ARGB can be BGRA :)
		 * by default, input pixel order is bypassed to output.
		 *
		 * @note : I think kdriver doesn't need to know this functionalities :)
		 */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_ch_alpha,			0x0 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_ch_red,				0x1 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_ch_green,			0x2 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_ch_blue,			0x3 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_round,				0x0 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_pure_ck,			0x0 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_pdec,	osd_lfsr_seed,			0x0 );

		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_main );
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_pdec );
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_base_addr );
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_scaler );
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_sync0 );

		FBDEV_H15_NOTE("Not enough Info.. might work only in Blend Mode?")
		/* setup basic path */

		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_path );

		#if 0
		if( hw_ctx->conn_ctrl[fb_dev_id].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
		#else
		if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
		#endif
		{
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_direc,		0x0 );
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_eo2s,		0x0 );
		}
		else
		{
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_direc,		0x1 );
			OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_eo2s,		0x1 );
		}

		/* setup each display size(?).
		 * keep in mind that width value osd_ctrl_path, osd_ctrl_sync1 is NOT real disp_width.
	 	 * they should be set a half when B.OSD is activated.
		 * these field is used to control how OSD fetch & process OSD frame data.
		 *
		 */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_path_width,		disp_width_sync );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_bosd_pos,		0x0 );	/* fixed to 0x0(BOSD1) */
		#if 0 /* KEEP HW DEFAULT */
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_path, osd_ctrl_eo2eeoo,			0x0 );	/* fixed to 0x0 */
		#endif
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_path );

		FBDEV_WARN("check osd_ctrl_sync1.osd_sync_disp_w !\n");
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_sync1 );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync1, osd_sync_disp_w,		disp_width_sync);
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync1, osd_sync_disp_h,		disp_height);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_sync1 );
	}

    ret = RET_OK;   /* all work done */

	return ret;
}

static void FBDEV_H15CMN_ReInitHW (void)
{
	FBDEV_H15_InitHW();
	FBDEV_SetInterruptEnable(MIXER_INTR,1);

	FBDEV_H15_InitOSDLayer(0);
	FBDEV_H15_InitOSDLayer(1);
	FBDEV_H15_InitOSDLayer(3);

	FBDEV_SetOSDImageEndian( 0, g_dm_ctx->wctx[0]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 1, g_dm_ctx->wctx[1]->pixel.bits_per_pixel );
	FBDEV_SetOSDImageEndian( 3, g_dm_ctx->wctx[3]->pixel.bits_per_pixel );
}

/*========================================================================================
    Implementation Group (OSD Hdr Processing)
========================================================================================*/
static void	FBDEV_H15CMN_WriteHdrRegsPostHandler ( int fb_dev_id, FBDEV_H15_OSD_HDR_T* osd_hdr, FBDEV_H15_OSD_EXHDR_T* osd_exhdr )
{
	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

	//UINT32	disp_width = hw_ctx->hw_disp_width;
	//UINT32	disp_height= hw_ctx->hw_disp_height;

	if (!hw_ctx->b_support_3dosd)
	{
		FBDEV_DEBUG("fb(%d) 3D not supported.\n", fb_dev_id ); return;
	}

	if (fb_dev_id != LX_FBDEV_ID_OSD0)
	{
		FBDEV_DEBUG("fb(%d) 3D not supported.\n", fb_dev_id ); return;
	}

	/* 3D HANDLER */
	{
		FBDEV_HW_CTX_T* hw_cmn_ctx = FBDEV_GetHWCtx();
		LX_FBDEV_TRID_CTRL_T* trid_ctrl = &(hw_cmn_ctx->fb[fb_dev_id].trid_ctrl);

		UINT32	disp_val_l	= 0x100;
		UINT32	disp_val_r	= 0x100;
		UINT32	disparity	= trid_ctrl->disparity;

		UINT32	b_s3d_en	= (trid_ctrl->mode & (LX_FBDEV_TRID_MODE_SS|LX_FBDEV_TRID_MODE_TB))? 0x1: 0x0;
		//UINT32	b_2d3d_en	= (trid_ctrl->mode & LX_FBDEV_TRID_MODE_2D_3D)? 0x1: 0x0;
		//BOOLEAN b_reverse_en= FBDEV_H15CMN_IsReverseOSD(fb_dev_id);

		UINT32	l_bmp_addr	= osd_hdr->osd_hdr_ptr_bmp;
		UINT32	r_bmp_addr	= l_bmp_addr;

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

		if (b_s3d_en)
		{
			if (trid_ctrl->mode & LX_FBDEV_TRID_MODE_SS ) /* S/S */
			{
				r_bmp_addr += ((osd_hdr->osd_hdr_w_mem * osd_exhdr->bits_per_pixel)>>3);

				#if 0
				if ( b_reverse_en )
				{
					UINT32 bmp_len = (osd_hdr->osd_hdr_wpl<<3) * osd_hdr->osd_hdr_h_mem;
					l_bmp_addr -= (bmp_len>>1); r_bmp_addr -= (bmp_len>>1);
				}
				#endif

				FBDEV_DEBUG("fb(%d) s3d L:0x%08x, R:0x%08x\n", fb_dev_id, l_bmp_addr, r_bmp_addr );

			}
			else /* T/B */
			{
				r_bmp_addr += ((osd_hdr->osd_hdr_w_mem * (osd_hdr->osd_hdr_h_mem>>1) * osd_exhdr->bits_per_pixel)>>3);

				#if 0
				if ( b_reverse_en )
				{
					UINT32 bmp_len = (osd_hdr->osd_hdr_wpl<<3) * osd_hdr->osd_hdr_h_mem;
					l_bmp_addr -= (bmp_len); r_bmp_addr -= (bmp_len);
				}
				#endif
			}
		}

		/* FIXME: only single POSD per fb_dev_id is supported */
		switch (fb_dev_id)
		{
			case LX_FBDEV_ID_OSD0: /* OSD0 */
			{
				/* implement disparity config if needed */

				OSDX_H15_RdFL(osdx_ctrl[0], osd_ctrl_main);

				if ( b_s3d_en != OSDX_H15_Rd00(osdx_ctrl[0], osd_ctrl_main, osd_s3d_en) )
				{
					FBDEV_DEBUG("fb(%d), s3d status change %d\n", fb_dev_id, b_s3d_en );

					OSDX_H15_Wr01(osdx_ctrl[0], osd_ctrl_main, osd_s3d_en, b_s3d_en);
					OSDX_H15_WrFL(osdx_ctrl[0], osd_ctrl_main );
				}

				if (b_s3d_en)
				{
					OS_WrReg(0xc8024c00 + 0x2c, l_bmp_addr );
					OS_WrReg(0xc8024c00 + 0x34, r_bmp_addr );
#if 0
					hw_ctx->sw_hdr_rdy_ctrl[0].mif_hdr[0] = l_bmp_addr;
					hw_ctx->sw_hdr_rdy_ctrl[0].mif_hdr[1] = r_bmp_addr;

					atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[0].mif_hdr_ready, 0x1);
#endif
				}
			}
			break;

			case LX_FBDEV_ID_OSD1:
			{
				/* implement disparity config if needed */

				OSDX_H15_RdFL(osdx_ctrl[1], osd_ctrl_main);

				if ( b_s3d_en != OSDX_H15_Rd00(osdx_ctrl[1], osd_ctrl_main, osd_s3d_en) )
				{
					FBDEV_DEBUG("fb(%d), s3d status change %d\n", fb_dev_id, b_s3d_en );

					OSDX_H15_Wr01(osdx_ctrl[1], osd_ctrl_main, osd_s3d_en, b_s3d_en);
					OSDX_H15_WrFL(osdx_ctrl[1], osd_ctrl_main );
				}

				if (b_s3d_en)
				{
					OS_WrReg(0xc8024c00 + 0x4c, l_bmp_addr );
					OS_WrReg(0xc8024c00 + 0x54, r_bmp_addr );
#if 0
					hw_ctx->sw_hdr_rdy_ctrl[1].mif_hdr[0] = l_bmp_addr;
					hw_ctx->sw_hdr_rdy_ctrl[1].mif_hdr[1] = r_bmp_addr;

					atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[1].mif_hdr_ready, 0x1);
#endif
				}
			}
			break;

			default:
			{
				/* not supported */
			}
			break;
		}
	}
}

/** write real header data to register
 *
 *
 *
 */
void	FBDEV_H15CMN_WriteHdrRegs	( int fb_dev_id, FBDEV_H15_OSD_HDR_T* osd_hdr, FBDEV_H15_OSD_EXHDR_T* osd_exhdr )
{
	int		xidx;
	int		xnum;
	int		xlst[2];

	ULONG   flags;
	struct scanvas* hCanvas = NULL;
	FBDEV_WIN_CTX_T* wctx	= NULL;

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	UINT32	disp_width = hw_ctx->hw_disp_width;
	UINT32	disp_height= hw_ctx->hw_disp_height;

	hCanvas = FBDEV_GetWinCanvas( fb_dev_id );
	FBDEV_CHECK_CODE(NULL==hCanvas, return, "invalid canvas. fb(%d)\n", fb_dev_id );

	wctx = FBDEV_GetWinCtx(fb_dev_id);
	FBDEV_CHECK_CODE(NULL==wctx, return, "invalid winctx. fb(%d)\n", fb_dev_id );

	xnum = FBDEV_H15CMN_GetHWXList(fb_dev_id, xlst);
	FBDEV_CHECK_CODE( INVALID_XNUM(xnum), return, "invalid fb(%d)\n", fb_dev_id );

	spin_lock_irqsave(&g_osd_hdr_spinlock, flags);

#if 0 /* obsolete */
	if ( g_reg_io_ctx->hdr_op_method == FBDEV_HDR_OP_MMIO )
	{
		memcpy( (void*)hCanvas->osd_hdr_addr, (void*)osd_hdr, sizeof(FBDEV_H15_OSD_HDR_T) );
	}
	/* raxis.lim (2014/02/04) -- I'll support only direct register access */
#endif

	/* notify osd freeze for debug */
	if ( !wctx->status.b_osd_hdr_flush ) FBDEV_WARN("fb(%d) freezed. osd_hdr_flush disabled.  \n", fb_dev_id );

	/* xnum==1 : it is natural that each fb_dev_id is connected to only single physical OSD HW. */
	if (xnum==1)
	{
		UINT32* hdr= (UINT32*)osd_hdr;

		xidx = xlst[0];

#ifdef FBDEV_H15_USE_HDR_RDY_REG
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr0, hdr[0]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr1, hdr[1]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr2, hdr[2]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr3, hdr[3]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr4, hdr[4]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr5, hdr[5]);
		OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr6, hdr[6]);

		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr0);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr1);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr2);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr3);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr4);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr5);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr6);

		if (wctx->status.b_osd_hdr_flush)
		{
			FBDEV_DEBUG("fb(%d). xidx %d. hdr_ready = 0x1\n", fb_dev_id, xidx );
			g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_main.osd_hdr_ready_flag = 0x1;
		}
#elif defined FBDEV_H15_SW_HDR_RDY_CTRL
		memcpy(hw_ctx->sw_hdr_rdy_ctrl[xidx].osd_hdr, hdr, sizeof(UINT32)*7);
		if (wctx->status.b_osd_hdr_flush)
		{
			atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[xidx].osd_hdr_ready, 0x1);
		}
#else
		FBDEV_REG_MEMCPY( &g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_hdr0, hdr, 7 );
#endif
	}
	/* xnum==2 : HW requires two OSD layer to display single framebuffer if framebuffer is UHD resolution
	 *
	 * OSD#0 points the left half screen.
	 * OSD#1 points the right half screen.
	 *
	 * if framebuffer is 3840x2160, each filed of osd_hdr should be set as following :
	 *
	 * - osd_hdr_xpos	: L - 0, R - 960 (if BLEND path. L-0, R-1920 if SEP Path. request by HW guy ).
	 * - osd_hdr_w_out	: L/R - 1920
	 * - osd_hdr_w_mem	: L/R - 1920
	 * - osd_hdr_ptr_bmp: L - same as bmp, R - bmp + (stride/2)
	 *
	 * below 4 field is not changed !
	 *
	 * - stride (osd_hdr_wpl)
	 * - pxlfmt(osd_hdr_format)
	 * - pxldepth(osd_hdr_depth)
	 * - gralpha(osd_hdr_global_alpha)
	 */
	else
	{
		int	i;
		FBDEV_H15_OSD_HDR_T	xosd_hdr[2];

		int fb_stride	= osd_hdr->osd_hdr_wpl<<3;

		FBDEV_CHECK_CODE( fb_stride % 16, goto hdr_write_exit, "mis-aligned stride %d (should be 16x)\n", fb_stride );
		FBDEV_CHECK_CODE( osd_hdr->osd_hdr_w_mem % 16, goto hdr_write_exit, "mis-aligned width %d (should be 8x)\n", osd_hdr->osd_hdr_w_mem );

		/* construct L/R header data */
		for (i=0;i<2;i++)
		{
			memcpy(&xosd_hdr[i], osd_hdr, sizeof(FBDEV_H15_OSD_HDR_T));

			xosd_hdr[i].osd_hdr_w_out	= disp_width>>1;			/* 1920 = 3840/2*/
			xosd_hdr[i].osd_hdr_h_out	= disp_height;				/* 2160 */
			xosd_hdr[i].osd_hdr_xpos	= FBDEV_H15CMN_GetXPosRefValue(fb_dev_id, i, disp_width );
			xosd_hdr[i].osd_hdr_ypos	= 0;						/* 0 */
			xosd_hdr[i].osd_hdr_w_mem	= osd_hdr->osd_hdr_w_mem>>1;/* 1920 = 3840/2 */
			xosd_hdr[i].osd_hdr_h_mem	= osd_hdr->osd_hdr_h_mem;	/* 1080 */

			xosd_hdr[i].osd_hdr_ptr_bmp	= osd_hdr->osd_hdr_ptr_bmp + i*((osd_hdr->osd_hdr_w_mem>>1)<<2);  /* bitmap_ptr + i*7680 (1920*4) */

			FBDEV_HW_DEBUG("UHD hdr[%d] in %d,%d, xy %d,%d, out %d,%d, wpl %d, fmt %d, depth %d, bmp 0x%x\n", i,
				xosd_hdr[i].osd_hdr_w_mem, xosd_hdr[i].osd_hdr_h_mem,
				xosd_hdr[i].osd_hdr_xpos , xosd_hdr[i].osd_hdr_ypos,
				xosd_hdr[i].osd_hdr_w_out, xosd_hdr[i].osd_hdr_h_out,
				xosd_hdr[i].osd_hdr_wpl,   xosd_hdr[i].osd_hdr_format, xosd_hdr[i].osd_hdr_depth, xosd_hdr[i].osd_hdr_ptr_bmp );
		}

		/* write L/R header simultaneously */
		for (i=0;i<2;i++)
		{
			UINT32* hdr;
			xidx = xlst[i];

			hdr = (UINT32*)&xosd_hdr[i];

			FBDEV_HW_DEBUG("xidx(%d) reg(%d) off 0x%04x %08x %08x %08x %08x %08x %08x %08x\n", xidx, i,
						((UINT32)&g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_hdr0) - (UINT32)g_osd_h15_reg,
						hdr[0],hdr[1],hdr[2],hdr[3],hdr[4],hdr[5],hdr[6] );

#ifdef FBDEV_H15_USE_HDR_RDY_REG
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr0, hdr[0]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr1, hdr[1]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr2, hdr[2]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr3, hdr[3]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr4, hdr[4]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr5, hdr[5]);
			OSDX_H15_Wr(osdx_ctrl[xidx], osd_ctrl_hdr6, hdr[6]);

			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr0);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr1);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr2);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr3);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr4);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr5);
			OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_hdr6);

			if (wctx->status.b_osd_hdr_flush)
			{
				FBDEV_DEBUG("fb(%d). xidx %d. hdr_ready = 0x1\n", fb_dev_id, xidx );
			#if 1
				g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_main.osd_hdr_ready_flag = 0x1;
			#else
				OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_main);
				OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main, osd_hdr_ready_flag, 0x1);
				OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_main);
			#endif
			}
#elif defined FBDEV_H15_SW_HDR_RDY_CTRL
			memcpy(hw_ctx->sw_hdr_rdy_ctrl[xidx].osd_hdr, hdr, sizeof(UINT32)*7);
			if (wctx->status.b_osd_hdr_flush)
			{
				atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[xidx].osd_hdr_ready, 0x1);
			}
#else
			FBDEV_REG_MEMCPY( &g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_hdr0, hdr, 7 );
#endif
		}
	}

	FBDEV_H15CMN_WriteHdrRegsPostHandler( fb_dev_id, osd_hdr, osd_exhdr );

hdr_write_exit:
	spin_unlock_irqrestore(&g_osd_hdr_spinlock, flags);
}

/** OSD HW Layer enable/disable
 *
 * @param	int fb_dev_id : osd layer ,
 *			enable : 0 : display off , 1 : display on
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
int FBDEV_H15CMN_SetOSDEnable(int fb_dev_id, int enable)
{
	int		i;
	int		xidx;
	int		xnum, xlst[2];
	UINT32	val = 0x0;

	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_H15_FB_COMPOSITOR_T* comp_ctrl = &hw_ctx->comp_ctrl;

	xnum = FBDEV_H15CMN_GetHWXList(fb_dev_id, xlst);
	FBDEV_CHECK_CODE( INVALID_XNUM(xnum), return RET_ERROR, "invalid fb(%d)\n", fb_dev_id );

	val = (enable)? 0x1: 0x0;
	FBDEV_DEBUG("fb(%d) view <%s>\n", fb_dev_id, (enable)?"on":"off");

	/* save the last on/off status for osd sw compoitor */
	comp_ctrl->osd_view_status[fb_dev_id] = val;

#if 0 /* NOT IMPL */
	/* if osd compositor enabled, DO NOT chnage HW settings.
	 * the real composition is done in FBDEV_H15_SetOSDEnable.
	 */
	FBDEV_CHECK_DEBUG( comp_ctrl->comp_opt != FB_COMP_OPT_NONE, ret_val=RET_OK; goto func_exit, "osd merge enabled.\n");
#endif

	for (i=0;i<2;i++)
	{
		if(INVALID_XIDX(xlst[i])) continue;

		xidx = xlst[i];

		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_main);
		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_sync0);

		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_main,  osd_en, val );
		OSDX_H15_Wr01(osdx_ctrl[xidx], osd_ctrl_sync0, osd_sync_enable, val );
	}

	/* try to write all OSD regs simultaneously */
	for (i=0;i<2;i++)
	{
		if(INVALID_XIDX(xlst[i])) continue;

		xidx = xlst[i];

		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_main);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_sync0);
	}

	return RET_OK;
}

/** OSD HW Layer status
 *
 * @param	void
 * @return	int 1 : ON , 0 : OFF
 *
 */
int FBDEV_H15CMN_GetOSDEnable(int fb_dev_id)
{
	int	xidx;
	int	xnum, xlst[2];

	//FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	//FBDEV_H15_FB_COMPOSITOR_T* comp_ctrl = &hw_ctx->comp_ctrl;

	/* check if OSD is valid or not */
	xnum = FBDEV_H15CMN_GetHWXList(fb_dev_id, xlst);
	FBDEV_CHECK_CODE(INVALID_XNUM(xnum), return FALSE, "invalid fb(%d)\n", fb_dev_id );

#if FBDEV_H15_NOT_IMPL("not implemented now")
	/* if osd compositor enabled, just return saved view status */
	FBDEV_CHECK_DEBUG( comp_ctrl->comp_opt != FB_COMP_OPT_NONE, return comp_ctrl->osd_view_status[fb_dev_id], "osd merge enabled\n");
#endif

	/* check if the first(main) xidx is valid */
	xidx = xlst[0];
	FBDEV_CHECK_CODE(INVALID_XIDX(xlst[0]), return FALSE, "fb(%d) invalid xidx %d\n", fb_dev_id, xlst[0]);

	return g_osd_h15_reg->osdx_ctrl[xidx].osd_ctrl_main.osd_en;
}

int FBDEV_H15CMN_SetTridCtrl( int fb_dev_id, LX_FBDEV_TRID_CTRL_T* ctrl )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	FBDEV_WIN_CTX_T* wctx = FBDEV_GetWinCtx(fb_dev_id);
	FBDEV_CHECK_CODE(NULL==wctx, return RET_ERROR, "invalid fb(%d)\n", fb_dev_id );

    FBDEV_DEBUG("%s 3D REQ. mode 0x%x, disp 0x%x\n", (lx_chip_plt()==LX_CHIP_PLT_FHD)? "FHD":"UHD", ctrl->mode, ctrl->disparity );
#ifdef FBDEV_H15_USE_UHD_S3D_AUTO_CONN_PATH
	if ( !hw_ctx->b_keep_hw_conn_type && lx_chip_plt() != LX_CHIP_PLT_FHD )
	{
		LX_FBDEV_CONN_CTRL_T conn_ctrl;

        memset(&conn_ctrl, 0x0, sizeof(LX_FBDEV_CONN_CTRL_T));

        if (ctrl->mode & (LX_FBDEV_TRID_MODE_SS|LX_FBDEV_TRID_MODE_TB|LX_FBDEV_TRID_MODE_2D_3D))
        {
	    	FBDEV_DEBUG("<%s> 3D ON. goto B.OSD path\n", (lx_chip_plt()==LX_CHIP_PLT_FHD)? "FHD":"UHD" );
            conn_ctrl.conn_type = LX_FBDEV_CONN_OSD_VIDEO_BLEND;
        }
        else
        {
	    	FBDEV_DEBUG("<%s> 3D OFF. goto S.OSD path\n", (lx_chip_plt()==LX_CHIP_PLT_FHD)? "FHD":"UHD" );
            conn_ctrl.conn_type = LX_FBDEV_CONN_OSD_VIDEO_SEPARATE;
        }
        FBDEV_H15CMN_SetConnCtrl(&conn_ctrl);
    }
	else
#endif
	{
	    FBDEV_DEBUG("<%s> 3D REQ. keep OSD path\n", (lx_chip_plt()==LX_CHIP_PLT_FHD)? "FHD":"UHD");
		FBDEV_WriteOSDHeader( &wctx->pixel );
	}

	return RET_OK;
}

int		FBDEV_H15CMN_SetViewCtrl ( LX_FBDEV_VIEW_CTRL_T* ctrl )
{
    int i;
    ULONG flags;

	/* we can't do spinlock when pivot mode is enabled because sleep ocurrs during GFX merge */
	spin_lock_irqsave(&g_osd_hdr_spinlock, flags);

    for (i=0; i<LX_FBDEV_ID_MAX; i++ )
    {
		if ( ctrl->view_status[i] == 0x0 || ctrl->view_status[i] == 0x1 )
			FBDEV_H15CMN_SetOSDEnable(i, ctrl->view_status[i] );
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
int     FBDEV_H15CMN_SetConnCtrl           (LX_FBDEV_CONN_CTRL_T* pConnCtrl)
{
	int     i;
	BOOLEAN osd_view_status[LX_FBDEV_ID_MAX];

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	FBDEV_WARN("OSD conn change. conn_type 0x%x (%s disp)\n", pConnCtrl->conn_type, (g_fbdev_cfg->uhd_disp_mode)? "UHD":"FHD" );

	FBDEV_CHECK_DEBUG( hw_ctx->hw_conn_type == pConnCtrl->conn_type, return RET_OK, "ignore the dup calls (0x%x)\n", pConnCtrl->conn_type );


#if 1
	/* save current config. diable OSD before changing OSD config */
	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
    {
		osd_view_status[i] = FBDEV_H15_GetOSDEnable(i);
		FBDEV_H15_SetOSDEnable( i, FALSE );
	}
#endif

	/* disable ISR to change ISR source */
	FBDEV_H15_SetInterruptEnable(MIXER_INTR, FALSE);

	/* sleep until OSD stablized. not necessary to sleep in H15 */

	if (pConnCtrl->conn_type & LX_FBDEV_CONN_OSD_VIDEO_SEPARATE)
	{
		FBDEV_DEBUG("H15::S.OSD\n");

		hw_ctx->hw_disp_width	= hw_ctx->sosd_disp.width;
		hw_ctx->hw_disp_height	= hw_ctx->sosd_disp.height;
	}
	else if(pConnCtrl->conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND)
	{
		FBDEV_DEBUG("H15::B.OSD\n");

		hw_ctx->hw_disp_width	= hw_ctx->bosd_disp.width;
		hw_ctx->hw_disp_height	= hw_ctx->bosd_disp.height;
	}
	else
	{
		FBDEV_WARN("invalid connection 0x%x\n", pConnCtrl->conn_type );
		goto func_exit;
	}

	/* save the current connection */
	hw_ctx->hw_conn_type = pConnCtrl->conn_type;

	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		hw_ctx->conn_ctrl[i].conn_type = hw_ctx->hw_conn_type;
	}

	/* reinit HW. OSD might be off during re-initialization */
	FBDEV_H15CMN_ReInitHW();

	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
	    FBDEV_WriteOSDHeader( &g_dm_ctx->wctx[i]->pixel );
	}
func_exit:
#if 1 /* recover OSD view stauts */
	for (i=0; i<LX_FBDEV_ID_MAX; i++ )
	{
		FBDEV_H15_SetOSDEnable( i, osd_view_status[i] );
	}
#endif

	return RET_OK;
}

/*========================================================================================
    Implementation Group (OSD merge ctrl)
========================================================================================*/
void FBDEV_H15CMN_DoOSDMerge (int fb_dev_id, BOOLEAN b_flip)
{
	/* @note virtual osd compositor can't be supported becuase I can't use H15 GFX inside kdriver. */
	FBDEV_ERROR("not implemented\n");
}

/*========================================================================================
    Implementation Group (PIVOT emulation)
========================================================================================*/
int		FBDEV_H15CMN_SetPivotCtrl	( LX_FBDEV_PIVOT_CTRL_T* ctrl )
{
	/* @note pivot ctrl can't be supported now because I can't use H15 GFX inside kdriver. */
	FBDEV_ERROR("not implemented\n");

	return RET_OK;
}

/*========================================================================================
    Implementation Group (VOSD emulation)
========================================================================================*/

/** handle 3D feature workaround
 *
 */
void	FBDEV_H15CMN_Handle3DOSD ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H15_OSD_HDR_T* osd_hdr )
{
	FBDEV_H15_HW_CTX_T*	hw_ctx = FBDEV_H15_GET_HW_CTX();

	UINT32	disp_width = hw_ctx->hw_disp_width;
	UINT32	disp_height= hw_ctx->hw_disp_height;

	FBDEV_HW_CTX_T* hw_cmn_ctx = FBDEV_GetHWCtx();
	LX_FBDEV_TRID_CTRL_T* trid_ctrl = &(hw_cmn_ctx->fb[fb_dev_id].trid_ctrl);

	/* FIXME : only OSD0 S3D is supported */
	if (hw_ctx->b_support_3dosd && (fb_dev_id==LX_FBDEV_ID_OSD0) && (trid_ctrl->mode & LX_FBDEV_TRID_MODE_SS) )
	{
        /* raxis.lim (2014/10/30)
         * HW requires osd input height should be less than half of output(display) height.
         * for example, if output height is 1080, input height should be less than 540 (half of 1080) !!
         * for example, if output height is 2160, input height should be less than 1920 (half of 2160) !!
         * H/W makes full OSD screen using two half-height left & right screen.
         *
         * So Application should knows this H/W restrtion.
         */
		if (osd_hdr->osd_hdr_h_mem > (disp_height>>1) )
		{
			osd_hdr->osd_hdr_h_mem = (osd_hdr->osd_hdr_h_out>>1);
		}

        /* raxis.lim (2014/10/30)
         * OSD H/W mixes left picture & right picture when S3D(S/S) enabled.
         * OSD H/W requires half of width & double of height.
         * for example, if you want to enable S3D with 1280x540 POSD, you should make OSD header with input_w 640, input_h 1080.
         * for example, if you want to enable S3D with 1920x1080 POSD, you should make OSD header with input_w 960, input_h 2160.
         */
		osd_hdr->osd_hdr_w_mem >>= 1;   /* h/2 */
		osd_hdr->osd_hdr_h_mem <<= 1;   /* V*2 */

		FBDEV_DEBUG("fb(%d) S3D(S/S) osd_hdr %d,%d disp %d,%d\n", fb_dev_id,
			osd_hdr->osd_hdr_w_mem, osd_hdr->osd_hdr_h_mem, disp_width, disp_height );
	}
}

/** handle osd scaler workaround during reverse screen
 *
 *	this workaround makes OSD scaler output to be similar to GFX.
 *	so this workaround code should run only when framebuffer is scaled up !!
 *
 */
void	FBDEV_H15CMN_HandleReverseOSD ( int fb_dev_id, struct scanvas* hCanvas, FBDEV_H15_OSD_HDR_T* osd_hdr )
{
	BOOLEAN	b_reverse_en;

	/* raxis.lim (2013/10/31) change reverse_screen detection code.
 	 * HW guy (woonghyun.kim) recommended to use mixer_ctrl_sync rather than osd_ctrl_main.
	 * Actually it is normal mixer_ctrl_sync.reg_sync_v_reverse is the same as osd_ctrl_main.osd_v_reverse_en.
	 */
	b_reverse_en = FBDEV_H15CMN_IsReverseOSD(fb_dev_id);

#if FBDEV_H15_NOT_IMPL("not implemented now. reverse handler")
	if (FALSE==b_reverse_en) { FBDEV_PRINT("reverse mode off. return\n"); return; }

	if ( osd_hdr->osd_hdr_h_mem < osd_hdr->osd_hdr_h_out )
	{
		UINT32	v_phase_off;
		FBDEV_PRINT("fb(%d) tune vphase_off 0x%x at reverse screen\n", fb_dev_id, _g_fbdev_h15_v_phase_offset );

		switch ( fb_dev_id )
		{
			case 0:
				OSD_H15_RdFL( osd0_ctrl_scaler );
				OSD_H15_Rd01( osd0_ctrl_scaler, osd0_sc_v_phase_off, v_phase_off );

				if ( v_phase_off != _g_fbdev_h15_v_phase_offset )
				{
					OSD_H15_Wr01( osd0_ctrl_scaler, osd0_sc_v_phase_off, _g_fbdev_h15_v_phase_offset );
					OSD_H15_WrFL( osd0_ctrl_scaler );
				}
			break;

			case 1:
				OSD_H15_RdFL( osd1_ctrl_scaler );
				OSD_H15_Rd01( osd1_ctrl_scaler, osd1_sc_v_phase_off, v_phase_off );

				if ( v_phase_off != _g_fbdev_h15_v_phase_offset )
				{
					OSD_H15_Wr01( osd1_ctrl_scaler, osd1_sc_v_phase_off, _g_fbdev_h15_v_phase_offset );
					OSD_H15_WrFL( osd1_ctrl_scaler );
				}
			break;

			case 2:	/* not supported */ break;
			case 3: /* don't do workaround */ break;
		}
	}
#endif
}

/** VOSD ISR handler
 *
 *
 */
void     FBDEV_H15CMN_ISRHook ( void )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	//FBDEV_H15_FB_COMPOSITOR_T* comp_ctrl = &hw_ctx->comp_ctrl;

#if FBDEV_H15_NOT_IMPL("not implemented now. MIF IF")
	/* wirte MIF::mrb_bank_mode */
	if ( _g_fbdev_h15_mrb_bank_mode != _g_fbdev_h15_mrb_bank_mode_req )
	{
		printk("[fb] mrb_bank_mode changed from 0x%x to 0x%x\n", _g_fbdev_h15_mrb_bank_mode, _g_fbdev_h15_mrb_bank_mode_req );
		_g_fbdev_h15_mrb_bank_mode = _g_fbdev_h15_mrb_bank_mode_req;

		MIF_OSD_H15_RdFL( mifo_rd_ctrl );
		MIF_OSD_H15_Wr01( mifo_rd_ctrl, mrb_bank_mode, _g_fbdev_h15_mrb_bank_mode );
		MIF_OSD_H15_WrFL( mifo_rd_ctrl );
	}
#endif

#if FBDEV_H15_NOT_IMPL("not implemented now. OSD compositor")

	if ( g_fbdev_h15_hw_ctx.comp_ctrl.comp_opt != FB_COMP_OPT_NONE )
	{
		if ( g_fbdev_h15_hw_ctx.comp_ctrl.my_vsync >= 0 && g_fbdev_h15_hw_ctx.comp_ctrl.my_vsync != g_fbdev_vsync_cnt )
		{
			g_fbdev_h15_hw_ctx.comp_ctrl.my_vsync = -1;
		}
	}
#endif

#ifdef FBDEV_H15_SW_HDR_RDY_CTRL
	{
		int	i,j;

		for (i=0;i<FBDEV_H15_MAX_OSD_NUM;i++)
		{
			if( atomic_read(&hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr_ready) )
			{
				UINT32* hdr = hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr;

				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr0, hdr[0] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr1, hdr[1] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr2, hdr[2] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr3, hdr[3] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr4, hdr[4] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr5, hdr[5] );
				OSDX_H15_Wr(osdx_ctrl[i], osd_ctrl_hdr6, hdr[6] );

				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr0);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr1);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr2);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr3);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr4);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr5);
				OSDX_H15_WrFL(osdx_ctrl[i], osd_ctrl_hdr6);

				if(g_fbdev_use_isr_mon)
				{
					printk("osd H15 ISR - osd(%d)  osd_hdr_sync. 0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n", i,
							hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[0], hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[1],
							hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[2], hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[3],
							hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[4], hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[5],
							hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr[6] );
				}

				atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[i].osd_hdr_ready, 0x0);
			}

			if( atomic_read(&hw_ctx->sw_hdr_rdy_ctrl[i].mif_hdr_ready) )
			{
				switch(i)
				{
					case 0:
						OS_WrReg(0xc8024c00 + 0x2c, hw_ctx->sw_hdr_rdy_ctrl[0].mif_hdr[0] );
						OS_WrReg(0xc8024c00 + 0x34, hw_ctx->sw_hdr_rdy_ctrl[0].mif_hdr[1] );
						break;

					case 1:
						OS_WrReg(0xc8024c00 + 0x4c, hw_ctx->sw_hdr_rdy_ctrl[1].mif_hdr[0] );
						OS_WrReg(0xc8024c00 + 0x54, hw_ctx->sw_hdr_rdy_ctrl[1].mif_hdr[1] );
						break;

					default: /* do nothing */ break;
				}

				atomic_set(&hw_ctx->sw_hdr_rdy_ctrl[i].mif_hdr_ready, 0x0);
			}
		}
	}
#endif


#ifdef FBDEV_H15_CPB_BW_TEST
	FBDEV_H15TEST_ISRHandler();
#endif
}

/*========================================================================================
    Implementation Group (Suspend/Resume)
========================================================================================*/
/** common resume/suspend handler
 *
 */
int	FBDEV_H15CMN_RunResume	( void )
{
	FBDEV_H15CMN_ReInitHW( );

	/* OSD freeze will be stopped after 500 ms */
	FBDEV_H15CMN_RunBootLogoTimer(500, TRUE);
	return RET_OK;
}

int	FBDEV_H15CMN_RunSuspend	( void )
{
	/* do nothing */
	return RET_OK;
}

/*========================================================================================
    Implementation Group (Misc)
========================================================================================*/

/*========================================================================================
    Implementation Group (OBSOLETE)
========================================================================================*/
void FBDEV_H15CMN_WriteFRCHeader( void )
{
	/* TODO: implement this function when FRC interface is necessary */
}

