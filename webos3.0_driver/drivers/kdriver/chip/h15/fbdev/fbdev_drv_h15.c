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

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_hw.h"
#include "fbdev_hw_h15.h"
#include "fbdev_hal.h"
#include "fbdev_reg_h15.h"
#include "osd_afbc_reg_h15.h"
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "../sys/ctop_regs.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	CCO_CTRL_BASE		0xc8024000
#define OSD_AFBC0_BASE		0xc8024700
#define OSD_AFBC1_BASE		0xc8024800
#define OSD_AFBC_LEN		0x100

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void FBDEV_H15_EventCallback		(FBDEV_HW_EVENT_T ev, FBDEV_HW_EVENT_DATA_T* data );

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern FBDEV_H15_HW_FUNC_T		g_fbdev_h15a0_hw_func;	/* H15A0 HW func */

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
void FBDEV_H15_InitOSDZOrder ( void );
LX_FBDEV_CFG_T*	FBDEV_H15_GetCfg(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile OSD_AFBC_H15_REG_T* 	g_osd_afbc_h15_reg[2] = {NULL,NULL};
		 OSD_AFBC_H15_REG_T* 	g_osd_afbc_h15_reg_cached[2] = {NULL,NULL};

volatile OSD_H15_REG_T*			g_osd_h15_reg = NULL;
		 OSD_H15_REG_T*			g_osd_h15_reg_cached = NULL;
#ifdef FBDEV_USE_MIF_CTRL
volatile MIF_OSD_H15_REG_T*		g_osd_mif_h15_reg = NULL;
		 MIF_OSD_H15_REG_T*		g_osd_mif_h15_reg_cached = NULL;
#endif

static FBDEV_H15_HW_CTX_T	g_fbdev_h15_hw_ctx;			/* H15 OSD H/W ctrl */

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void FBDEV_H15_AutoDetectOSDHWValue (void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static	FBDEV_HW_CTX_T	_g_fbdev_hw_ctx =
{
	.GetCfg				= FBDEV_H15_GetCfg,
	.RunSuspend			= FBDEV_H15CMN_RunSuspend,
	.RunResume			= FBDEV_H15CMN_RunResume,

	.InitHW				= FBDEV_H15_InitHW,
	.ShutdownHW			= FBDEV_H15_ShutdownHW,

	.SetOSDEnable		= FBDEV_H15_SetOSDEnable,
	.GetOSDEnable		= FBDEV_H15_GetOSDEnable,

	.InitOSDLayer		= FBDEV_H15_InitOSDLayer,
	.WriteOSDHeader		= FBDEV_H15_WriteOSDHeader,

	.GetZList			= FBDEV_H15_GetZList,
	.SetZList			= FBDEV_H15_SetZList,
    .SetVideoMixerCtrl  = FBDEV_H15_SetVideoMixerCtrl,

	.WriteOSDEndian		= FBDEV_H15_WriteOSDEndian,
	.WriteOSDCSC		= FBDEV_H15_WriteOSDCSC,

	.GetInterruptStatus	= FBDEV_H15_GetInterruptStatus,
	.SetInterruptClear	= FBDEV_H15_SetInterruptClear,
	.SetInterruptEnable	= FBDEV_H15_SetInterruptEnable,

	.SetViewCtrl		= FBDEV_H15_SetViewCtrl,
	.SetTridCtrl		= FBDEV_H15_SetTridCtrl,
	.SetConnCtrl		= FBDEV_H15_SetConnCtrl,
	.SetPivotCtrl		= FBDEV_H15_SetPivotCtrl,

	.EventCallback		= FBDEV_H15_EventCallback,
};

/*========================================================================================
    Implementation Group
========================================================================================*/

/** get H15 specific configuration
 *
 *  @return LX_FBDEV_CFG_T
 */
LX_FBDEV_CFG_T*   FBDEV_H15_GetCfg(void)
{
    return NULL;
}

/** initialize H15 specific HW context variables.
 *
 *	@note this function is called before at H15 configure stage NOT at HW initialization stage.
 */
void	FBDEV_H15_InitHWCtx( FBDEV_HW_CTX_T* halobj )
{
	int	i;

	/* select the active H15 HW function handler */
	switch( lx_chip_rev() )
	{
		case LX_CHIP_REV(H15,A0):
		default:
			memcpy( &g_fbdev_h15_hw_ctx.func, &g_fbdev_h15a0_hw_func, sizeof(FBDEV_H15_HW_FUNC_T)); break;
	}

	/* initialize HW context variable */
	memset( &g_fbdev_h15_hw_ctx.frc_if_ctrl, 0x0, sizeof(FBDEV_H15_FRC_IF_CTRL_T));

	/* auto detect the default OSD connection and display size etc */
	FBDEV_H15_AutoDetectOSDHWValue( );

    for( i=0; i<LX_FBDEV_ID_MAX; i++ )
    {
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].opmask       	= FBDEV_H15_VOSD_OPMASK_NONE;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].osd_idx      	=  0;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].my_vsync     	= -1;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].osd_surf[0]  	= -1;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].osd_surf[1] 	= -1;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].fb_mem_ptr[0]	= 0x0;
        g_fbdev_h15_hw_ctx.vosd_ctrl[i].fb_mem_ptr[1]	= 0x0;

		g_fbdev_h15_hw_ctx.conn_ctrl[i].conn_type		= g_fbdev_h15_hw_ctx.hw_conn_type;
	}

	g_fbdev_h15_hw_ctx.comp_ctrl.comp_opt 		= FB_COMP_OPT_NONE;
	g_fbdev_h15_hw_ctx.comp_ctrl.fb_dev_id		= 0;
	g_fbdev_h15_hw_ctx.comp_ctrl.my_vsync 		= -1;
	g_fbdev_h15_hw_ctx.comp_ctrl.merge_surf 	= -1;
	g_fbdev_h15_hw_ctx.comp_ctrl.out_fb_surf[0] = -1;
	g_fbdev_h15_hw_ctx.comp_ctrl.out_fb_surf[1] = -1;

	OS_InitMutex( &g_fbdev_h15_hw_ctx.comp_ctrl.mtx, OS_SEM_ATTR_DEFAULT );

#ifdef FBDEV_H15_SW_HDR_RDY_CTRL
	for (i=0; i<FBDEV_H15_MAX_OSD_NUM; i++)
	{
		atomic_set( &g_fbdev_h15_hw_ctx.sw_hdr_rdy_ctrl[i].osd_hdr_ready, 0 );
		atomic_set( &g_fbdev_h15_hw_ctx.sw_hdr_rdy_ctrl[i].mif_hdr_ready, 0 );
	}
#endif

#if (CONFIG_LX_BOOT_LOGO==1)
	/* FIXME */
#endif

	/* initialize hal object
	 * after this function, all fbdev kdriver can get g_fbdev_h15_hw_ctx via g_fbdev_hw_ctx->chip_priv_ctx
	 */
	memcpy(halobj, &_g_fbdev_hw_ctx, sizeof(FBDEV_HW_CTX_T));
	halobj->chip_priv_ctx = &g_fbdev_h15_hw_ctx;
}

/*========================================================================================
	Implementation Group (INIT/EXIT)
========================================================================================*/
/** initialize OSD IP HW and register
 *
 * @param
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK]
 */
int FBDEV_H15_InitHW (void)
{
	int	i;
	int ret = RET_ERROR;
	LX_MEM_BUF_T afbc_reg_desc[2] = { { OSD_AFBC0_BASE, OSD_AFBC_LEN }, { OSD_AFBC1_BASE, OSD_AFBC_LEN } };

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	/* ioremap OSD regs */
	if (NULL==g_osd_h15_reg)
	{
		g_osd_h15_reg =(OSD_H15_REG_T*)ioremap(g_reg_io_ctx->reg_base_addr, g_reg_io_ctx->reg_size);
	}
	FBDEV_CHECK_CODE( NULL==g_osd_h15_reg, goto func_exit, "out of memory\n");

	if (NULL==g_osd_h15_reg_cached)
	{
		g_osd_h15_reg_cached = (OSD_H15_REG_T*)OS_Malloc(g_reg_io_ctx->reg_size);
	}
	FBDEV_CHECK_CODE( NULL==g_osd_h15_reg_cached, return RET_ERROR, "out of memory\n");

	/* ioremap OSD_AFGC regs */
	for (i=0;i<2;i++)
	{
		if (NULL==g_osd_afbc_h15_reg[i])
		{
			g_osd_afbc_h15_reg[i] =(OSD_AFBC_H15_REG_T*)ioremap( afbc_reg_desc[i].addr, afbc_reg_desc[i].length );
		}
		FBDEV_CHECK_CODE( NULL==g_osd_afbc_h15_reg[i], goto func_exit, "out of memory\n");

		if (NULL==g_osd_afbc_h15_reg_cached[i])
		{
			g_osd_afbc_h15_reg_cached[i] = (OSD_AFBC_H15_REG_T*)OS_Malloc( afbc_reg_desc[i].length );
		}
		FBDEV_CHECK_CODE( NULL==g_osd_afbc_h15_reg_cached[i], return RET_ERROR, "out of memory\n");
	}

#ifdef FBDEV_USE_MIF_CTRL
	/* access to mif register */
	if (NULL==g_osd_mif_h15_reg)
	{
		g_osd_mif_h15_reg =(MIF_OSD_H15_REG_T*)ioremap( 0xc0035900, sizeof(MIF_OSD_H15_REG_T));
	}
	FBDEV_CHECK_CODE( NULL==g_osd_mif_h15_reg, goto func_exit, "out of memory\n");

	if (NULL==g_osd_mif_h15_reg_cached)
	{
		g_osd_mif_h15_reg_cached = (MIF_OSD_H15_REG_T*)OS_Malloc( sizeof(MIF_OSD_H15_REG_T));
	}
	FBDEV_CHECK_CODE( NULL==g_osd_mif_h15_reg_cached, return RET_ERROR, "out of memory\n");
#endif

	/* raxis.lim (2014/06/18)
	 * CCO init param is too difficult to understand :(
	 * ask woonghyun.kim if you have any question. he knows everything !!
	 *
	 * let's summarize all that I know
	 * CCO_CTRL_BASE+0x08 controls osd pluse source selections
	 * 0x5 : used for BLEND path (non-compressed)
	 * 0x7 : used for SEP path   (non-compressed)
	 *
	 */
	#if 0
	if( hw_ctx->conn_ctrl[0].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	#else
	if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	#endif
	{
		OS_WrReg ( CCO_CTRL_BASE + 0x08, 0x55555555 );	/* CC0 CTRL_AUTO_INIT */
	}
	else /* SEP */
	{
		OS_WrReg ( CCO_CTRL_BASE + 0x08, 0x77777777 );	/* CC0 CTRL_AUTO_INIT */
	}

	OS_WrReg ( CCO_CTRL_BASE + 0x00, 0x00ff0000 );	/* CCO AUTO_INIT */

#if 0
	/* OSD SCALER 2*LineMemory Enable.
	 * [TODO] rewrite the below bad code :(
	 */
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_osd_h15_reg) + 0x110));
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_osd_h15_reg) + 0x150));
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_osd_h15_reg) + 0x1D0));
#endif

	/* HW revision specific initialization */
	FBDEV_H15_InitOSDZOrder();
	hw_ctx->func.H15_InitHW();

	ret = RET_OK; /* all work done */
func_exit:
	return ret;
}

/** de-initial OSD IP HW and register
 *
 * @param
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK]
 */
int FBDEV_H15_ShutdownHW	(void)
{
	int ret = RET_OK;

	/* TODO: implement it */

	return ret;
}

/** make initial OSD zorder.
 *
 *	@return none
 *	@note this function controls osd_layer_mux register
 */
void FBDEV_H15_InitOSDZOrder ( void )
{
    LX_FBDEV_ZLIST_T    zlist;

    zlist.path      = LX_FBDEV_PATH_ALL;
    zlist.order[0]  = LX_FBDEV_ZLIST_OSD0;  /* top most . H/W default */
    zlist.order[1]  = LX_FBDEV_ZLIST_OSD1;  /* center   . H/W default */
    zlist.order[2]  = LX_FBDEV_ZLIST_VIDEO; /* bottom   . H/W default */

	FBDEV_H15_SetZList( &zlist );

	return;
}

/*========================================================================================
	Implementation Group
========================================================================================*/
/** set endianess on each layer
 *
 */
int FBDEV_H15_WriteOSDEndian(int fb_dev_id, ENDIAN_INFORM_T *endian)
{

	int	i;
	int xidx;
	int xnum, xlst[2];
    UINT32 val;

	xnum = FBDEV_H15CMN_GetHWXList(fb_dev_id, xlst);
	FBDEV_CHECK_CODE(INVALID_XNUM(xnum), return RET_ERROR, "invalid fb(%d)\n", fb_dev_id );

    val = (UINT32)((endian->header << 8) | (endian->palette << 4) | (endian->image << 0) );

    for (i=0;i<xnum;i++)
    {
		xidx = xlst[i];

		if (INVALID_XIDX(xidx)) continue; /* ignore processingif xidx is invalid */

		OSDX_H15_RdFL(osdx_ctrl[xidx], osd_ctrl_swap);
		OSDX_H15_Wr  (osdx_ctrl[xidx], osd_ctrl_swap, val);
		OSDX_H15_WrFL(osdx_ctrl[xidx], osd_ctrl_swap);
	}

    return RET_OK;
}

/** set color space conversion on each layer
 *
 *	@note H15 doesn't support CSC for each layer
 *
 */
int FBDEV_H15_WriteOSDCSC(int layer , CSC_INFORM_T *csc)
{
#if FBDEV_H15_NOT_IMPL("not implemented now. I don't know")
    int ret = 0;

    switch(layer){
        case LX_FBDEV_ID_OSD0:
        case LX_FBDEV_ID_OSD1:
        case LX_FBDEV_ID_OSD2:
        case LX_FBDEV_ID_CSR0:
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_datai 	= csc->data;
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_rw_n 	= csc->rw_n;
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_bypass	= csc->bypass;
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_sel 	= csc->sel;
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_out_ctrl= csc->out_ctrl;
            g_osd_h15_reg->osd_common_csc_coef0.reg_common_csc0_in_ctrl	= csc->in_ctrl;
        break;

        default:
            FBDEV_PRINT("WARNING : invalid int type %s %d\n" , __func__ , __LINE__);
            return -1;
        break;

    }
    return ret;
#else
	return RET_OK;
#endif
}

/** Get interrupt status
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK?]
 */
int FBDEV_H15_GetInterruptStatus(INTERRUPT_INFORM_T intr)
{
	UINT32	status = 0;

	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	OSD_H15_RdFL(osd_common_ctrl_irq);

	if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
	{
		UINT32 bosd_mixer;
		OSD_H15_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_bosd, bosd_mixer );

		if(bosd_mixer)	status |= (1<<MIXER_INTR);
		else			printk("OSD ISR - BOSD MIXER INTR error\n");
	}
	else
	{
		UINT32 sosd_mixer;
		OSD_H15_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_sosd, sosd_mixer );

		if(sosd_mixer)	status |= (1<<MIXER_INTR);
		else			printk("OSD ISR - SOSD MIXER INTR error\n");
	}

	FBDEV_ISR_DEBUG("irq reg info = 0x%08x\n", status );

	return status;
}

/** Clear interrupt status
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK]
 */
int FBDEV_H15_SetInterruptClear(INTERRUPT_INFORM_T intr)
{
	OSD_H15_RdFL(osd_common_ctrl_irq);

	/* clear both bosd, sosd mixer intr for the safe operation */
	OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_bosd, 0x1 );
	OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_sosd, 0x1 );

	OSD_H15_WrFL(osd_common_ctrl_irq);

	return RET_OK;
}

/** Set interrupt enable/disable
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T , int : enable/disable
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK]
 */
int FBDEV_H15_SetInterruptEnable(INTERRUPT_INFORM_T intr , int enable)
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

    switch(intr)
    {
		default:
        case MIXER_INTR:
		{
			OSD_H15_RdFL(osd_common_ctrl_irq);
			#if 0
			if( hw_ctx->conn_ctrl[0].conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#else
			if( hw_ctx->hw_conn_type & LX_FBDEV_CONN_OSD_VIDEO_BLEND )
			#endif
			{
				FBDEV_DEBUG("INTR <%s> for B.OSD\n", enable? "ON":"OFF" );
				OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_en_mixer_bos, enable );
				OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_en_mixer_sos, 0x0 );
			}
			else
			{
				FBDEV_DEBUG("INTR <%s> for S.OSD\n", enable? "ON":"OFF" );
				OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_en_mixer_bos, 0x0 );
				OSD_H15_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_en_mixer_sos, enable );
			}
			OSD_H15_WrFL(osd_common_ctrl_irq);
		}
		break;
    }

    return RET_OK;
}

/** Update OSD ZList
 *
 * @param zList [IN] pointer to POSD List config.
 * @return RET_OK
 *
 * @note layer id definition is 03: video, 00: OSD0, 01: OSD1, 02: OSD2 ( refer to OSD register manual )
 * @note I will not use osd_mixer_ctrl_mux0.layer_mux_3 ( it's default value is POSD2 )
 *
 * [OK]
 */
int FBDEV_H15_SetZList ( LX_FBDEV_ZLIST_T* zList )
{
#if FBDEV_H15_NOT_IMPL("Can't implement until OSD AppNote released")
	UINT32	video_layer_off;
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	/* raxis.lim (2012/12/10) -- I should control video mix flag to prevent screen garbage during bootlogo */
	video_layer_off = ( hw_ctx->video_mixer_ctrl.b_video_mix )? FALSE: TRUE;

	OSD_H15_RdFL(osd_mixer_ctrl_mux0);

	OSD_H15_Wr06(osd_mixer_ctrl_mux0, 	layer_mux_3,        0x2,
                                        layer_mux_2,        zList->order[0],
                                        layer_mux_1,        zList->order[1],
                                        layer_mux_0,        zList->order[2],
										video_layer_off,	video_layer_off,
                                        video_layer_alpha,  0xff );
    OSD_H15_WrFL(osd_mixer_ctrl_mux0);
#endif
	return RET_OK;
}

/** Get OSD ZList
 *
 * @param zList [OUT] pointer to POSD List config.
 * @return RET_OK
 *
 * @note layer id definition is 00: video, 01: OSD0, 02: OSD1, 03: OSD2
 * @note H15 doesn't support L&R zlist.
 * @note I will not use osd_mixer_ctrl_mux0.layer_mux_3 ( it's default value is POSD2 )
 *
 * [OK]
 */
int FBDEV_H15_GetZList( LX_FBDEV_ZLIST_T* zList )
{
#if FBDEV_H15_NOT_IMPL("Can't implement until OSD AppNote released")
    OSD_H15_RdFL(osd_mixer_ctrl_mux0);

	zList->order[0] = g_osd_h15_reg_cached->osd_mixer_ctrl_mux0.layer_mux_2;	/* top most */
	zList->order[1] = g_osd_h15_reg_cached->osd_mixer_ctrl_mux0.layer_mux_1;	/* midle */
	zList->order[2] = g_osd_h15_reg_cached->osd_mixer_ctrl_mux0.layer_mux_0;	/* bottom */
#else
	zList->order[0] = 0;
	zList->order[1] = 1;
	zList->order[2] = 2;
#endif
	return RET_OK;
}

/** control video mixer region
 *
 *	@note window_ctrl.window_alpha value is initialized as 0xff in OSD H/W.
 *		  actually, we don't need to set window_ctrl.window_alpha since it is always set to 0xff.
 *		  but I think that it is set to the right value explictely
 *
 *	[OK?]
 */
int FBDEV_H15_SetVideoMixerCtrl   (LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl)
{
#if FBDEV_H15_NOT_IMPL("Can't implement until OSD AppNote released")
	int	ret = RET_ERROR;
	int b_enable = (ctrl->b_enable)? 0x1: 0x0;

	FBDEV_INFO("path = 0x%x, mixId = 0x%x\n", ctrl->path, ctrl->mix_id );

	switch( ctrl->mix_id )
	{
		case 0:
		{
			OSD_H15_RdFL( osd_mixer_ctrl_mux1 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux2 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux3 );

			OSD_H15_Wr02( osd_mixer_ctrl_mux1, reg_mixer_window0_en, 		b_enable,
												reg_mixer_window0_alpha,	0xff );
			OSD_H15_Wr02( osd_mixer_ctrl_mux2, reg_mixer_window0_xpos, 		ctrl->mix_win.x,
												reg_mixer_window0_ypos, 	ctrl->mix_win.y );
			OSD_H15_Wr02( osd_mixer_ctrl_mux3, reg_mixer_window0_w, 		ctrl->mix_win.w,
												reg_mixer_window0_h, 		ctrl->mix_win.h );
			OSD_H15_WrFL( osd_mixer_ctrl_mux1 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux2 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux3 );
		}
		break;

		case 1:
		{
			OSD_H15_RdFL( osd_mixer_ctrl_mux4 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux5 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux6 );

			OSD_H15_Wr02( osd_mixer_ctrl_mux4, reg_mixer_window1_en, 		b_enable,
												reg_mixer_window1_alpha,	0xff );
			OSD_H15_Wr02( osd_mixer_ctrl_mux5, reg_mixer_window1_xpos, 		ctrl->mix_win.x,
												reg_mixer_window1_ypos, 	ctrl->mix_win.y );
			OSD_H15_Wr02( osd_mixer_ctrl_mux6, reg_mixer_window1_w, 		ctrl->mix_win.w,
												reg_mixer_window1_h, 		ctrl->mix_win.h );
			OSD_H15_WrFL( osd_mixer_ctrl_mux4 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux5 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux6 );
		}
		break;

		case 2:
		{
			OSD_H15_RdFL( osd_mixer_ctrl_mux7 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux8 );
			OSD_H15_RdFL( osd_mixer_ctrl_mux9 );

			OSD_H15_Wr02( osd_mixer_ctrl_mux7, reg_mixer_window2_en, 		b_enable,
												reg_mixer_window2_alpha,	0xff );
			OSD_H15_Wr02( osd_mixer_ctrl_mux8, reg_mixer_window2_xpos, 		ctrl->mix_win.x,
												reg_mixer_window2_ypos, 	ctrl->mix_win.y );
			OSD_H15_Wr02( osd_mixer_ctrl_mux9, reg_mixer_window2_w, 		ctrl->mix_win.w,
												reg_mixer_window2_h, 		ctrl->mix_win.h );
			OSD_H15_WrFL( osd_mixer_ctrl_mux7 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux8 );
			OSD_H15_WrFL( osd_mixer_ctrl_mux9 );
		}
		break;

		default: FBDEV_ERROR("invalid mix id %d\n", ctrl->mix_id ); goto func_exit;
	}

	ret = RET_OK; /* all work done */
func_exit:
	return ret;
#else
	return RET_OK;
#endif
}

/*========================================================================================
	Implementation Group
========================================================================================*/
/** some layer depdendent HW initialization
 *
 */
int	FBDEV_H15_InitOSDLayer	(int layer)
{
	return FBDEV_H15CMN_InitOSDLayer(layer);
}

/*========================================================================================
	Implementation Group (Event)
========================================================================================*/
void FBDEV_H15_EventCallback		(FBDEV_HW_EVENT_T ev, FBDEV_HW_EVENT_DATA_T* data )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();

	switch(ev)
	{
		/* H15 support only 1 8bpp OSD layer (it's selectable)
	 	 * before writing OSD header (before updating 8bpp OSD plane), selection register should be set !!
		 *
		 */
		case FBDEV_HW_EVENT_PXL_FMT_CHANGE:
		{
//			UINT32	fb_dev_id = data->pxl_fmt_change.fb_dev_id;
//			UINT32	bpp		  = data->pxl_fmt_change.bits_per_pixel;

#if FBDEV_H15_NOT_IMPL("Can't implement until OSD AppNote released")
		#ifdef FBDEV_SUPPORT_8BPP_EMUL
			/* do nothing */
		#else
			if ( bpp == 8 )
			{
				OSD_H15_RdFL(osd_common_plte_sel);
				OSD_H15_Wr01(osd_common_plte_sel, reg_common_plte_sel, fb_dev_id );
				OSD_H15_WrFL(osd_common_plte_sel);
				FBDEV_NOTI("layer %d, 8bpp OSD selected\n", fb_dev_id );
			}
		#endif
#endif
		}
		break;

		case FBDEV_HW_EVENT_REVERSE_SCREEN_CHANGE:
		{
			UINT32	fb_dev_id = data->reverse_screen_change.fb_dev_id;
			UINT32	enable	= data->reverse_screen_change.enable;
#if FBDEV_H15_NOT_IMPL("Can't implement until OSD AppNote released")
	        switch(fb_dev_id)
			{
				case LX_FBDEV_ID_OSD0:  g_osd_h15_reg->osd0_ctrl_main.osd0_v_reverse_en = enable; break;
				case LX_FBDEV_ID_OSD1:  g_osd_h15_reg->osd1_ctrl_main.osd1_v_reverse_en = enable; break;
				case LX_FBDEV_ID_OSD2:  g_osd_h15_reg->osd2_ctrl_main.osd2_v_reverse_en = enable; break;
				case LX_FBDEV_ID_CSR0:  g_osd_h15_reg->osd3_ctrl_main.osd3_v_reverse_en = enable; break;
				default:                FBDEV_WARN("invalid OSD ID %d\n", fb_dev_id ); break;
	        }
#else
			FBDEV_INFO("fb(%d). reverse changed to <%s>\n", fb_dev_id, (enable)? "ON":"OFF");
#endif
		}
		break;

		case FBDEV_HW_EVENT_VSYNC:
		{
			if ( hw_ctx->func.H15_ISRTickHandler )
			{
				hw_ctx->func.H15_ISRTickHandler();
			}
		}
		break;

		case FBDEV_HW_EVENT_POSD_OPEN:
		{
			/* raxis.lim (2014/10/23)
			 * H15 is so fast, I decrased logo timer to 300ms after cold boot test
			 * logo freeze timer will keep the boot logo until app request the OSD update even though timer is expired !
			 */
			FBDEV_H15CMN_RunBootLogoTimer(300, FALSE );
		}
		break;

		default:
		{

		}
		break;
	}
}

/*========================================================================================
	Implementation Group (REV dependent)
========================================================================================*/
int		FBDEV_H15_SetViewCtrl(LX_FBDEV_VIEW_CTRL_T* ctrl )
{
	int	ret = RET_ERROR;
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_H15_FB_COMPOSITOR_T* comp_ctrl = &hw_ctx->comp_ctrl;

	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_SetViewCtrl, return RET_ERROR, "NULL handler\n");

	ret = hw_ctx->func.H15_SetViewCtrl( ctrl );

	/* if composition enabled, start OSD merge */
	if(ret==RET_OK && comp_ctrl->comp_opt != FB_COMP_OPT_NONE)
	{
		FBDEV_DEBUG("osd merge (case1:viewctrl) from %s\n", __F__ );
		FBDEV_H15CMN_DoOSDMerge(0, TRUE);
	}

	return ret;
}

/** set OSD output ctrl
 *
 */
int     FBDEV_H15_SetConnCtrl(LX_FBDEV_CONN_CTRL_T* ctrl)
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_SetConnCtrl, return RET_ERROR, "NULL handler\n");

	return hw_ctx->func.H15_SetConnCtrl( ctrl );
}

/** update 3D OSD mode
 *
 */
int FBDEV_H15_SetTridCtrl( int layer, LX_FBDEV_TRID_CTRL_T* ctrl )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_SetTridCtrl, return RET_ERROR, "NULL handler\n");

	return hw_ctx->func.H15_SetTridCtrl( layer, ctrl );
	}

int FBDEV_H15_SetPivotCtrl( LX_FBDEV_PIVOT_CTRL_T* ctrl )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_SetPivotCtrl, return RET_ERROR, "NULL handler\n");

	return hw_ctx->func.H15_SetPivotCtrl(ctrl);
}

/** Set Header information
 *
 */
int FBDEV_H15_WriteOSDHeader( struct scanvas *hCanvas )
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_WriteOSDHeader, return RET_ERROR, "NULL handler\n");

	return hw_ctx->func.H15_WriteOSDHeader( hCanvas );
}

/** OSD HW Layer enable/disable
 *
 */
int FBDEV_H15_SetOSDEnable(int layer , int enable)
{
	int	ret = RET_ERROR;
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_H15_FB_COMPOSITOR_T* comp_ctrl = &hw_ctx->comp_ctrl;

	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_SetOSDEnable, return RET_ERROR, "NULL handler\n");

	ret = hw_ctx->func.H15_SetOSDEnable( layer, enable );

	/* if composition enabled, start OSD merge */
	if(ret==RET_OK && comp_ctrl->comp_opt != FB_COMP_OPT_NONE)
	{
		FBDEV_DEBUG("osd merge (case2:osd-onoff) from %s\n", __F__ );
		FBDEV_H15CMN_DoOSDMerge(layer, TRUE);
	}

	return ret;
}

/** OSD HW Layer status
 *
 */
int FBDEV_H15_GetOSDEnable(int layer)
{
	FBDEV_H15_HW_CTX_T* hw_ctx = FBDEV_H15_GET_HW_CTX();
	FBDEV_CHECK_CODE(NULL==hw_ctx->func.H15_GetOSDEnable, return RET_ERROR, "NULL handler\n");

	return hw_ctx->func.H15_GetOSDEnable( layer );
}


/** detect the default OSD connection
 *	This function will return the default(best) OSD connection setup based on SW, HW info
 *
 *	@note In H15, video path is fixed to UHD display. OSD will use this HW assumption.
 */
static void FBDEV_H15_AutoDetectOSDHWValue (void)
{
	if(lx_chip_plt() != LX_CHIP_PLT_FHD)
    {
		UINT32 	osd_conn_val = LX_FBDEV_CONN_OSD_VIDEO_SEPARATE; /* S.OSD by default */
		BOOLEAN	b_support_3dosd = FALSE;

		#if 1
		{
			/* detect blend configuration. "bosd=1" is just TEST purpose */
			char    bosd_opt[8];
			char	hw_opt[24];

	        if(RET_OK==OS_ScanKernelCmdline("bosd=%s", bosd_opt))
			{
				switch(bosd_opt[0]) {
					case '1': osd_conn_val = LX_FBDEV_CONN_OSD_VIDEO_BLEND; break;
					default:  osd_conn_val = LX_FBDEV_CONN_OSD_VIDEO_SEPARATE; break;
	   	     	}
			}

			/* hwopt[17] == 0x0 : OSD is set to B.OSD due to NO_FRC */
	        if(RET_OK==OS_ScanKernelCmdline("hwopt=%s", hw_opt))
			{
				switch(hw_opt[17]) {
					case '0': osd_conn_val = LX_FBDEV_CONN_OSD_VIDEO_BLEND; 	b_support_3dosd = TRUE;  break;
					default:  osd_conn_val = LX_FBDEV_CONN_OSD_VIDEO_SEPARATE; 	b_support_3dosd = FALSE; break;
				}
			}

			g_fbdev_h15_hw_ctx.hw_conn_type = osd_conn_val;
		}
		#endif

		g_fbdev_h15_hw_ctx.b_support_3dosd = b_support_3dosd;	/* support 3D OSD if there is no backend chip */

		g_fbdev_h15_hw_ctx.sosd_disp.width = 1920;	/* S.OSD size is fixed to 1920x1080 */
		g_fbdev_h15_hw_ctx.sosd_disp.height= 1080;

		g_fbdev_h15_hw_ctx.bosd_disp.width = 3840;	/* B.OSD size might be changed based on DTV config */
		g_fbdev_h15_hw_ctx.bosd_disp.height= 2160;

		if (osd_conn_val & LX_FBDEV_CONN_OSD_VIDEO_BLEND)
		{
			g_fbdev_h15_hw_ctx.b_keep_hw_conn_type = TRUE;
			g_fbdev_h15_hw_ctx.hw_disp_width	= g_fbdev_h15_hw_ctx.bosd_disp.width;
			g_fbdev_h15_hw_ctx.hw_disp_height	= g_fbdev_h15_hw_ctx.bosd_disp.height;
		}
		else
		{
			g_fbdev_h15_hw_ctx.b_keep_hw_conn_type = FALSE;
			g_fbdev_h15_hw_ctx.hw_disp_width 	= g_fbdev_h15_hw_ctx.sosd_disp.width;
			g_fbdev_h15_hw_ctx.hw_disp_height	= g_fbdev_h15_hw_ctx.sosd_disp.height;
		}
    }
	else
	{
		g_fbdev_h15_hw_ctx.hw_conn_type		= -1;
		g_fbdev_h15_hw_ctx.b_keep_hw_conn_type = FALSE;

		g_fbdev_h15_hw_ctx.sosd_disp.width	= 1920;
		g_fbdev_h15_hw_ctx.sosd_disp.height	= 1080;

		g_fbdev_h15_hw_ctx.bosd_disp.width	= 1920;
		g_fbdev_h15_hw_ctx.bosd_disp.height	= 1080;

		g_fbdev_h15_hw_ctx.hw_disp_width 	= 1920;
		g_fbdev_h15_hw_ctx.hw_disp_height	= 1080;
	}

	printk("OSD HW default osd_conn = 0x%x, disp_size = %dx%d\n",
			g_fbdev_h15_hw_ctx.hw_conn_type, g_fbdev_h15_hw_ctx.hw_disp_width, g_fbdev_h15_hw_ctx.hw_disp_height );
}

