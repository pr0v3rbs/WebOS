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

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "fbdev_impl.h"
#include "fbdev_hw.h"
#include "fbdev_hw_m14.h"
#include "fbdev_reg_m14.h"
#include "mif_osd_reg_m14.h"
#include "fbdev_hal.h"
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "../sys/ctop_regs.h"

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

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern FBDEV_M14_HW_CTX_T		g_fbdev_m14a0_hw_ctx;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
void FBDEV_M14_InitOSDZOrder ( void );
LX_FBDEV_CFG_T*	FBDEV_M14_GetCfg(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile OSD_M14_REG_T*			g_fbdev_m14_reg = NULL;
OSD_M14_REG_T*					g_fbdev_m14_reg_cached = NULL;
volatile MIF_OSD_M14_REG_T*		g_mif_osd_m14_reg = NULL;
MIF_OSD_M14_REG_T*				g_mif_osd_m14_reg_cached = NULL;

FBDEV_M14_HW_CTX_T				g_fbdev_m14_hw_ctx;			/* M14 OSD H/W ctrl */

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static	FBDEV_HW_CTX_T		g_fbdev_hw_ctx_m14 =
{
	.GetCfg				= FBDEV_M14_GetCfg,
	.RunSuspend			= FBDEV_M14CMN_RunSuspend,
	.RunResume			= FBDEV_M14CMN_RunResume,

	.InitHW				= FBDEV_M14_InitHW,
	.ShutdownHW			= FBDEV_M14_ShutdownHW,

	.SetOSDEnable		= FBDEV_M14_SetOSDEnable,
	.GetOSDEnable		= FBDEV_M14_GetOSDEnable,

	.InitOSDLayer		= FBDEV_M14_InitOSDLayer,
	.WriteOSDHeader		= FBDEV_M14_WriteOSDHeader,

	.GetZList			= FBDEV_M14_GetZList,
	.SetZList			= FBDEV_M14_SetZList,
    .SetVideoMixerCtrl  = FBDEV_M14_SetVideoMixerCtrl,

	.WriteOSDEndian		= FBDEV_M14_WriteOSDEndian,
	.WriteOSDCSC		= FBDEV_M14_WriteOSDCSC,

	.GetInterruptStatus	= FBDEV_M14_GetInterruptStatus,
	.SetInterruptClear	= FBDEV_M14_SetInterruptClear,
	.SetInterruptEnable	= FBDEV_M14_SetInterruptEnable,

	.SetViewCtrl		= FBDEV_M14_SetViewCtrl,
	.SetTridCtrl		= FBDEV_M14_SetTridCtrl,
	.SetConnCtrl		= FBDEV_M14_SetConnCtrl,
	.SetPivotCtrl		= FBDEV_M14_SetPivotCtrl,

	.EventCallback		= FBDEV_M14CMN_EventCallback,
};

/*========================================================================================
    Implementation Group
========================================================================================*/

/** get M14 specific configuration
 *
 *  @return LX_FBDEV_CFG_T
 */
LX_FBDEV_CFG_T*   FBDEV_M14_GetCfg(void)
{
    return NULL;
}

/** initialize M14 specific HW Ctx.
 *
 *	@note this function is called before any M14 configuration
 */
void	FBDEV_M14_InitHWCtx( FBDEV_HW_CTX_T*	hal )
{
	int	i;

	/* select the active M14 HW function handler */
	switch( lx_chip_rev() )
	{
		case LX_CHIP_REV(M14,A0):
		default:
			memcpy( &g_fbdev_m14_hw_ctx, &g_fbdev_m14a0_hw_ctx, sizeof(FBDEV_M14_HW_CTX_T)); break;

	}

	/* initialize variable such as FRC ctrl, vosd ctrl etc */
    for( i=0; i<LX_FBDEV_ID_MAX; i++ )
    {
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].opmask       = FBDEV_M14_VOSD_OPMASK_NONE;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].osd_idx      =  0;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].my_vsync     = -1;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].osd_surf[0]  = -1;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].osd_surf[1]  = -1;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].fb_mem_ptr[0]= 0x0;
        g_fbdev_m14_hw_ctx.vosd_ctrl[i].fb_mem_ptr[1]= 0x0;

        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].osd_idx      =  0;
        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].my_vsync     = -1;
        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].osd_surf[0]  = -1;
        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].osd_surf[1]  = -1;
        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].fb_mem_ptr[0]= 0x0;
        g_fbdev_m14_hw_ctx.clut_vosd_ctrl[i].fb_mem_ptr[1]= 0x0;
	}

	g_fbdev_m14_hw_ctx.comp_ctrl.comp_opt = FB_COMP_OPT_NONE;
	g_fbdev_m14_hw_ctx.comp_ctrl.fb_dev_id= 0;
	g_fbdev_m14_hw_ctx.comp_ctrl.my_vsync 	= -1;
	g_fbdev_m14_hw_ctx.comp_ctrl.merge_surf = -1;
	g_fbdev_m14_hw_ctx.comp_ctrl.out_fb_surf[0] = -1;
	g_fbdev_m14_hw_ctx.comp_ctrl.out_fb_surf[1] = -1;

	OS_InitMutex( &g_fbdev_m14_hw_ctx.comp_ctrl.mtx, OS_SEM_ATTR_DEFAULT );

#if (CONFIG_LX_BOOT_LOGO==1)
	printk("[fb] disable video mix\n");
	g_fbdev_m14_hw_ctx.video_mixer_ctrl.b_video_mix = FALSE;
#else
	g_fbdev_m14_hw_ctx.video_mixer_ctrl.b_video_mix = TRUE;
#endif

	/* fill hal value */
	memcpy( hal, &g_fbdev_hw_ctx_m14, sizeof(FBDEV_HW_CTX_T));
	hal->chip_priv_ctx = &g_fbdev_m14_hw_ctx;
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
int FBDEV_M14_InitHW		(void)
{
	int					ret = RET_ERROR;

	/* access to register */
	g_fbdev_m14_reg =(OSD_M14_REG_T *)ioremap( g_reg_io_ctx->reg_base_addr, g_reg_io_ctx->reg_size );
	FBDEV_CHECK_CODE( g_fbdev_m14_reg == NULL, goto func_exit, "out of memory\n");

	g_fbdev_m14_reg_cached = (OSD_M14_REG_T *)OS_Malloc( g_reg_io_ctx->reg_size );
	FBDEV_CHECK_CODE( g_fbdev_m14_reg_cached == NULL, return RET_ERROR, "out of memory\n");

	/* access to mif register */
	if ( lx_chip_rev() < LX_CHIP_REV(M14,B0) ) /* A0 */
	{
		g_mif_osd_m14_reg =(MIF_OSD_M14_REG_T*)ioremap( 0xc0035900, sizeof(MIF_OSD_M14_REG_T));
	}
	else /* B0 or later */
	{
		g_mif_osd_m14_reg =(MIF_OSD_M14_REG_T*)ioremap( 0xc0024e00, sizeof(MIF_OSD_M14_REG_T));
	}
	FBDEV_CHECK_CODE( g_mif_osd_m14_reg == NULL, goto func_exit, "out of memory\n");

	g_mif_osd_m14_reg_cached = (MIF_OSD_M14_REG_T*)OS_Malloc( sizeof(MIF_OSD_M14_REG_T));
	FBDEV_CHECK_CODE( g_mif_osd_m14_reg_cached == NULL, return RET_ERROR, "out of memory\n");

	/* DE_TOP::DED_PIC_INIT */
	if ( lx_chip_rev() < LX_CHIP_REV(M14,B0) ) /* A0 */
	{
		OS_WrReg ( 0xc0035000, 0x08000000 );
	}
	else /* B0 or later */
	{
		OS_WrReg ( 0xc0024000, 0x01000000 );
	}

	/* OSD SCALER 2*LineMemory Enable.
	 * [TODO] rewrite the below bad code :(
	 */
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_fbdev_m14_reg) + 0x110));
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_fbdev_m14_reg) + 0x150));
	__raw_writel(0x80001000 , (volatile void*)((ULONG)(g_fbdev_m14_reg) + 0x1D0));

	/* make POSD order to be default */
	FBDEV_M14_InitOSDZOrder( );

	/* call revision specific initialization */
	g_fbdev_m14_hw_ctx.M14_InitHW( );

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
int FBDEV_M14_ShutdownHW	(void)
{
	int ret = RET_OK;

	iounmap((void*)g_fbdev_m14_reg);

	return ret;
}

/** make initial OSD zorder.
 *
 *	@return none
 *	@note this function controls osd_layer_mux register
 *	@note M14 doesn't support POSD2
 */
void FBDEV_M14_InitOSDZOrder ( void )
{
    LX_FBDEV_ZLIST_T    zlist;

    zlist.path      = LX_FBDEV_PATH_ALL;
    zlist.order[0]  = LX_FBDEV_ZLIST_OSD0;  /* top most . H/W default */
    zlist.order[1]  = LX_FBDEV_ZLIST_OSD1;  /* center   . H/W default */
    zlist.order[2]  = LX_FBDEV_ZLIST_VIDEO; /* bottom   . H/W default */

	FBDEV_M14_SetZList( &zlist );

	return;
}

/*========================================================================================
	Implementation Group
========================================================================================*/
static const int 		pixel_depth_2_bpp[PIX_DEPTH_MAX] = { 1, 2, 4, 8, 16, 24, 32  };

/*========================================================================================
	Implementation Group
========================================================================================*/
/** set endianess on each layer
 *
 */
int FBDEV_M14_WriteOSDEndian(int layer , ENDIAN_INFORM_T *endian)
{
    int ret = RET_OK;

    unsigned int temp;

    temp = (unsigned int)((endian->header << 8) | (endian->palette << 4) | (endian->image << 0) );

    switch(layer){
        case LX_FBDEV_ID_OSD0:
            memcpy( (void *)&g_fbdev_m14_reg->osd0_ctrl_swap, (void *)&temp , sizeof(int) );
        break;

        case LX_FBDEV_ID_OSD1:
            memcpy( (void *)&g_fbdev_m14_reg->osd1_ctrl_swap ,(void *) &temp , sizeof(int) );
        break;

        case LX_FBDEV_ID_OSD2:
            memcpy( (void *)&g_fbdev_m14_reg->osd2_ctrl_swap, (void *)&temp , sizeof(int) );
        break;

        case LX_FBDEV_ID_CSR0:
            memcpy( (void *)&g_fbdev_m14_reg->osd3_ctrl_swap, (void *) &temp , sizeof(int) );
        break;
        default:
            FBDEV_PRINT("WARNING : invalid int type %s %d\n" , __func__ , __LINE__);
            ret = RET_ERROR;
        break;
    }

    return ret;
}

/** set color space conversion on each layer
 *
 *	@note M14 doesn't support CSC for each layer
 *
 */
int FBDEV_M14_WriteOSDCSC(int layer , CSC_INFORM_T *csc)
{
    int ret = 0;

    switch(layer){
        case LX_FBDEV_ID_OSD0:
        case LX_FBDEV_ID_OSD1:
        case LX_FBDEV_ID_OSD2:
        case LX_FBDEV_ID_CSR0:
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_datai 	= csc->data;
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_rw_n 		= csc->rw_n;
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_bypass	= csc->bypass;
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_sel 		= csc->sel;
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_out_ctrl	= csc->out_ctrl;
            g_fbdev_m14_reg->osd_common_csc_coef0.reg_common_csc0_in_ctrl	= csc->in_ctrl;
        break;

        default:
            FBDEV_PRINT("WARNING : invalid int type %s %d\n" , __func__ , __LINE__);
            return -1;
        break;

    }

    return ret;
}

/** Get interrupt status
 * [note] If you don't need any interrupt service of FB device, DO NOT call this function
 *
 * @param	INTERRUPT_INFORM_T
 * @return	int 0 : OK , -1 : NOT OK
 *
 * [OK?]
 */
int FBDEV_M14_GetInterruptStatus(INTERRUPT_INFORM_T intr)
{
	UINT32  status = 0x0;

	OSD_M14_RdFL(osd_common_ctrl_irq);

	switch(intr)
	{
		case CRSR_INTR:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd3, status);
		break;
		case OSD_0_INTR:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd0, status);
		break;
		case OSD_1_INTR:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd1, status);
		break;
		case OSD_2_INTR:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd2, status);
		break;
		case MIXER_INTR:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer, status);
		break;
		case MIXER_INTR_EXT:
			OSD_M14_Rd01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer_60hz, status);
		break;
			case ALL_INTR:
		default:            status = (OSD_M14_Rd(osd_common_ctrl_irq)) & 0x3f;    break;
	}

    //FBDEV_PRINT("intr(%d) status %x\n", intr, status );
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
int FBDEV_M14_SetInterruptClear(INTERRUPT_INFORM_T intr)
{
	OSD_M14_RdFL(osd_common_ctrl_irq);

	switch(intr)
	{
    	case OSD_0_INTR:	OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd0, 1);		break;
    	case OSD_1_INTR:	OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd1, 1);		break;
		case OSD_2_INTR:	OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd2, 1);		break;
    	case CRSR_INTR:		OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd3, 1);		break;
		case MIXER_INTR:	OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer,1);		break;
		case MIXER_INTR_EXT:OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer_60hz,1);	break;
		case ALL_INTR:
		default:			OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd0, 1);
							OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd1, 1);
							OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd2, 1);
							OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_osd3, 1);
							OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer,1);
							OSD_M14_Wr01(osd_common_ctrl_irq, reg_common_ctrl_irq_clear_mixer_60hz,1);	break;
	}

	OSD_M14_WrFL(osd_common_ctrl_irq);

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
int FBDEV_M14_SetInterruptEnable(INTERRUPT_INFORM_T intr , int enable)
{
    switch(intr)
    {
        case OSD_0_INTR:    g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd0 = enable; break;
        case OSD_1_INTR:    g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd1 = enable; break;
        case OSD_2_INTR:    g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd2 = enable; break;
        case CRSR_INTR:     g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd3 = enable; break;
        case MIXER_INTR:    g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_mixer= enable; break;
	    case MIXER_INTR_EXT:g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_mixer_60hz = enable; break;
        case ALL_INTR:
		default: 			g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd0 = enable;
							g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd1 = enable;
							g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd2 = enable;
							g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_osd3 = enable;
							g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_mixer= enable;
							g_fbdev_m14_reg->osd_common_ctrl_irq.reg_common_ctrl_irq_en_mixer_60hz = enable; break;
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
int FBDEV_M14_SetZList ( LX_FBDEV_ZLIST_T* zList )
{
	UINT32	video_layer_off;

	/* raxis.lim (2012/12/10) -- I should control video mix flag to prevent screen garbage during bootlogo */
	video_layer_off = ( g_fbdev_m14_hw_ctx.video_mixer_ctrl.b_video_mix )? FALSE: TRUE;

	OSD_M14_RdFL(osd_mixer_ctrl_mux0);

	OSD_M14_Wr06(osd_mixer_ctrl_mux0, 	layer_mux_3,        0x2,
                                        layer_mux_2,        zList->order[0],
                                        layer_mux_1,        zList->order[1],
                                        layer_mux_0,        zList->order[2],
										video_layer_off,	video_layer_off,
                                        video_layer_alpha,  0xff );
    OSD_M14_WrFL(osd_mixer_ctrl_mux0);

	return RET_OK;
}

/** Get OSD ZList
 *
 * @param zList [OUT] pointer to POSD List config.
 * @return RET_OK
 *
 * @note layer id definition is 00: video, 01: OSD0, 02: OSD1, 03: OSD2
 * @note M14 doesn't support L&R zlist.
 * @note I will not use osd_mixer_ctrl_mux0.layer_mux_3 ( it's default value is POSD2 )
 *
 * [OK]
 */
int FBDEV_M14_GetZList( LX_FBDEV_ZLIST_T* zList )
{
    OSD_M14_RdFL(osd_mixer_ctrl_mux0);

	zList->order[0] = g_fbdev_m14_reg_cached->osd_mixer_ctrl_mux0.layer_mux_2;	/* top most */
	zList->order[1] = g_fbdev_m14_reg_cached->osd_mixer_ctrl_mux0.layer_mux_1;	/* midle */
	zList->order[2] = g_fbdev_m14_reg_cached->osd_mixer_ctrl_mux0.layer_mux_0;	/* bottom */

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
int FBDEV_M14_SetVideoMixerCtrl   (LX_FBDEV_VIDEO_MIXER_CTRL_T* ctrl)
{
	int	ret = RET_ERROR;
	int b_enable = (ctrl->b_enable)? 0x1: 0x0;

	FBDEV_INFO("path = 0x%x, mixId = 0x%x\n", ctrl->path, ctrl->mix_id );

	switch( ctrl->mix_id )
	{
		case 0:
		{
			OSD_M14_RdFL( osd_mixer_ctrl_mux1 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux2 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux3 );

			OSD_M14_Wr02( osd_mixer_ctrl_mux1, reg_mixer_window0_en, 		b_enable,
												reg_mixer_window0_alpha,	0xff );
			OSD_M14_Wr02( osd_mixer_ctrl_mux2, reg_mixer_window0_xpos, 		ctrl->mix_win.x,
												reg_mixer_window0_ypos, 	ctrl->mix_win.y );
			OSD_M14_Wr02( osd_mixer_ctrl_mux3, reg_mixer_window0_w, 		ctrl->mix_win.w,
												reg_mixer_window0_h, 		ctrl->mix_win.h );
			OSD_M14_WrFL( osd_mixer_ctrl_mux1 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux2 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux3 );
		}
		break;

		case 1:
		{
			OSD_M14_RdFL( osd_mixer_ctrl_mux4 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux5 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux6 );

			OSD_M14_Wr02( osd_mixer_ctrl_mux4, reg_mixer_window1_en, 		b_enable,
												reg_mixer_window1_alpha,	0xff );
			OSD_M14_Wr02( osd_mixer_ctrl_mux5, reg_mixer_window1_xpos, 		ctrl->mix_win.x,
												reg_mixer_window1_ypos, 	ctrl->mix_win.y );
			OSD_M14_Wr02( osd_mixer_ctrl_mux6, reg_mixer_window1_w, 		ctrl->mix_win.w,
												reg_mixer_window1_h, 		ctrl->mix_win.h );
			OSD_M14_WrFL( osd_mixer_ctrl_mux4 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux5 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux6 );
		}
		break;

		case 2:
		{
			OSD_M14_RdFL( osd_mixer_ctrl_mux7 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux8 );
			OSD_M14_RdFL( osd_mixer_ctrl_mux9 );

			OSD_M14_Wr02( osd_mixer_ctrl_mux7, reg_mixer_window2_en, 		b_enable,
												reg_mixer_window2_alpha,	0xff );
			OSD_M14_Wr02( osd_mixer_ctrl_mux8, reg_mixer_window2_xpos, 		ctrl->mix_win.x,
												reg_mixer_window2_ypos, 	ctrl->mix_win.y );
			OSD_M14_Wr02( osd_mixer_ctrl_mux9, reg_mixer_window2_w, 		ctrl->mix_win.w,
												reg_mixer_window2_h, 		ctrl->mix_win.h );
			OSD_M14_WrFL( osd_mixer_ctrl_mux7 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux8 );
			OSD_M14_WrFL( osd_mixer_ctrl_mux9 );
		}
		break;

		default: FBDEV_ERROR("invalid mix id %d\n", ctrl->mix_id ); goto func_exit;
	}

	ret = RET_OK; /* all work done */
func_exit:
	return ret;
}

/*========================================================================================
	Implementation Group
========================================================================================*/
/** some layer depdendent HW initialization
 *
 */
int	FBDEV_M14_InitOSDLayer	(int layer)
{
	return FBDEV_M14CMN_InitOSDLayer(layer);
}


/*========================================================================================
	Implementation Group (REV dependent)
========================================================================================*/
int		FBDEV_M14_SetViewCtrl(LX_FBDEV_VIEW_CTRL_T* ctrl )
{
	int	ret = RET_ERROR;
	FBDEV_M14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_m14_hw_ctx.comp_ctrl;

	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_SetViewCtrl, return RET_ERROR, "NULL handler\n");

	ret = g_fbdev_m14_hw_ctx.M14_SetViewCtrl( ctrl );

	/* if composition enabled, start OSD merge */
	if(ret==RET_OK && comp_ctrl->comp_opt != FB_COMP_OPT_NONE)
	{
		FBDEV_DEBUG("osd merge (case1:viewctrl) from %s\n", __F__ );
		FBDEV_M14CMN_DoOSDMerge(0, TRUE);
	}

	return ret;
}

/** set OSD output ctrl
 *
 */
int     FBDEV_M14_SetConnCtrl(LX_FBDEV_CONN_CTRL_T* ctrl)
{
	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_SetConnCtrl, return RET_ERROR, "NULL handler\n");

	return g_fbdev_m14_hw_ctx.M14_SetConnCtrl( ctrl );
}

/** update 3D OSD mode
 *
 */
int FBDEV_M14_SetTridCtrl( int layer, LX_FBDEV_TRID_CTRL_T* ctrl )
{
	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_SetTridCtrl, return RET_ERROR, "NULL handler\n");

	return g_fbdev_m14_hw_ctx.M14_SetTridCtrl( layer, ctrl );
}

int FBDEV_M14_SetPivotCtrl( LX_FBDEV_PIVOT_CTRL_T* ctrl )
{
	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_SetPivotCtrl, return RET_ERROR, "NULL handler\n");

	return g_fbdev_m14_hw_ctx.M14_SetPivotCtrl(ctrl);
}

/** Set Header information
 *
 */
int FBDEV_M14_WriteOSDHeader( struct scanvas *hCanvas )
{
	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_WriteOSDHeader, return RET_ERROR, "NULL handler\n");
	return g_fbdev_m14_hw_ctx.M14_WriteOSDHeader( hCanvas );
}

/** OSD HW Layer enable/disable
 *
 */
int FBDEV_M14_SetOSDEnable(int layer , int enable)
{
	int	ret = RET_ERROR;
	FBDEV_M14_FB_COMPOSITOR_T* comp_ctrl = &g_fbdev_m14_hw_ctx.comp_ctrl;

	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_SetOSDEnable, return RET_ERROR, "NULL handler\n");

	ret = g_fbdev_m14_hw_ctx.M14_SetOSDEnable( layer, enable );

	/* if composition enabled, start OSD merge */
	if(ret==RET_OK && comp_ctrl->comp_opt != FB_COMP_OPT_NONE)
	{
		FBDEV_DEBUG("osd merge (case2:osd-onoff) from %s\n", __F__ );
		FBDEV_M14CMN_DoOSDMerge(layer, TRUE);
	}

	return ret;
}

/** OSD HW Layer status
 *
 */
int FBDEV_M14_GetOSDEnable(int layer)
{
	FBDEV_CHECK_CODE(NULL==g_fbdev_m14_hw_ctx.M14_GetOSDEnable, return RET_ERROR, "NULL handler\n");

	return g_fbdev_m14_hw_ctx.M14_GetOSDEnable( layer );
}

