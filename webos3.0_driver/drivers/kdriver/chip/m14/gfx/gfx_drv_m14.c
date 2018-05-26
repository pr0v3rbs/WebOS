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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "gfx_impl.h"
#include "gfx_reg_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	addr_sw_de_sav;
	UINT32	addr_sw_cpu_gpu;
	UINT32	addr_sw_cpu_shadow;
}
GFX_M14_MEM_MAP_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void		GFX_M14_stop(void);
extern int 		GFX_M14_RunFlushCommand(void);
extern int 		GFX_M14_RunStartCommand(void);
extern int 		GFX_M14_PauseCommand(void);
extern int		GFX_M14_ResumeCommand(void);
extern int		GFX_M14_StopCommand(void);
extern int		GFX_M14_SwResetCommand(void);
extern void		GFX_M14_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue);
extern BOOLEAN	GFX_M14_IsGfxIdle(void);
extern void		GFX_M14_GetOperationStatus(GFX_CMD_OP_T *operate);
extern void		GFX_M14_SetOperationStatus(GFX_CMD_OP_T *operate);
extern void		GFX_M14_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port);
extern void		GFX_M14_SetInputConfigure(GFX_PORT_CONFIGURE_T *port);
extern void		GFX_M14_SetBlendingOut(GFX_ENUM_OUT_T type);
extern void		GFX_M14_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
extern void		GFX_M14_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
extern void		GFX_M14_GetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
extern void		GFX_M14_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
extern void		GFX_M14_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler);
extern void		GFX_M14_SetInterruptMode(UINT32 uiMode);
extern void		GFX_M14_SetAckMode(UINT32 *mode);
extern void		GFX_M14_GetAckMode(UINT32 *mode);
extern int		GFX_M14_SetCLUT(int port, int size , UINT32 *data);
extern void		GFX_M14_SetCommandDelayReg(UINT16 delay);
extern UINT16 	GFX_M14_GetCommandDelayReg(void);
extern void		GFX_M14_DumpRegister(void);
extern int 		GFX_M14_RunSuspend(void);
extern int 		GFX_M14_RunResume(void);

extern void		GFX_M14_InitScaler(void);

extern BOOLEAN	GFX_M14_ClkCheck(void);
extern int		GFX_M14_RunClkOnCommand(void);
extern int		GFX_M14_RunClkOffCommand(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile GFX_M14_REG_T*	g_gfx_reg_m14;
GFX_M14_REG_T*				g_gfx_reg_m14_cache;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
const LX_GFX_CFG_T*	GFX_M14_GetCfg		(void);
int 				GFX_M14_InitHW		(void);
int 				GFX_M14_ShutdownHW	(void);
irqreturn_t 		GFX_M14_ISRHandler	(int irq, void *dev_id, struct pt_regs *regs);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
const static	GFX_HAL_T		g_gfx_hal_m14 =
{
	.GetCfg					= GFX_M14_GetCfg,
	.InitHW					= GFX_M14_InitHW,
	.ShutdownHW				= GFX_M14_ShutdownHW,
	.RunSuspend				= GFX_M14_RunSuspend,
	.RunResume				= GFX_M14_RunResume,

	.RunFlushCommand		= GFX_M14_RunFlushCommand,
	.RunStartCommand		= GFX_M14_RunStartCommand,
	.RunPauseCommand		= GFX_M14_PauseCommand,
	.RunResumeCommand		= GFX_M14_ResumeCommand,
	.RunStopCommand			= GFX_M14_StopCommand,
	.RunSwResetCommand		= GFX_M14_SwResetCommand,

	.GetComQueueStatus		= GFX_M14_GetComQueueStatus,

	.GetOperationStatus		= GFX_M14_GetOperationStatus,
	.SetOperationStatus		= GFX_M14_SetOperationStatus,
	.GetInputConfigure		= GFX_M14_GetInputConfigure,
	.SetInputConfigure		= GFX_M14_SetInputConfigure,
	.GetBlendConfigure		= GFX_M14_GetBlendConfigure,
	.SetBlendConfigure		= GFX_M14_SetBlendConfigure,
	.SetBlendingOut			= GFX_M14_SetBlendingOut,
	.GetOutputConfigure		= GFX_M14_GetOutputConfigure,
	.SetOutputConfigure		= GFX_M14_SetOutputConfigure,
	.SetScalerConfigure		= GFX_M14_SetScalerConfigure,
	.SetCLUT				= GFX_M14_SetCLUT,
	.SetCommandDelayReg		= GFX_M14_SetCommandDelayReg,
	.GetCommandDelayReg		= GFX_M14_GetCommandDelayReg,

	.IsGfxIdle				= GFX_M14_IsGfxIdle,

	.DumpRegister			= GFX_M14_DumpRegister,

	.RunClkOnCommand		= GFX_M14_RunClkOnCommand,
	.RunClkOffCommand		= GFX_M14_RunClkOffCommand,
	.ClkCheck				= GFX_M14_ClkCheck,
};

static	LX_GFX_CFG_T 		g_gfx_cfg_m14 =
{
    .b_hw_scaler 			= TRUE,
    .surface_blit_cmd_delay = 0xffff,		// normal gfx operation by APP.
    .screen_blit_cmd_delay  = 0xffff,		// internal gfx operation by KDRV.
#if CONFIG_LX_BOARD_FPGA
	.sync_wait_timeout		= 4000,			/* 4 sec for FPGA */
#else
	.sync_wait_timeout		= 60,			/* 40 ms */
#endif
	.sync_fail_retry_count	= 2,

    .hw_limit = {
        .max_surface_width      = 8191,     /* L9 has 13 bit width field  */
        .max_surface_stride     = 32767,    /* L9 has 15 bit stride field */
        .min_scaler_input_width = 12,       /* L9 doesn't strech below input width <= 12 */
    },
};

/*========================================================================================
    Implementation Group
========================================================================================*/

/** get M14 specific configuration
 *
 *  @return LX_GFX_CFG_T
 */
const LX_GFX_CFG_T*   GFX_M14_GetCfg(void)
{
    return &g_gfx_cfg_m14;
}

void	GFX_M14_InitHAL( GFX_HAL_T*	hal )
{
	memcpy( hal, &g_gfx_hal_m14, sizeof(GFX_HAL_T));

    switch( lx_chip_rev() )
    {
        case LX_CHIP_REV(M14,A0):
        {
            g_gfx_cfg_m14.workaround.bad_dst_addr_stuck      = 0;
            g_gfx_cfg_m14.workaround.scaler_read_buf_stuck   = 0;
            g_gfx_cfg_m14.workaround.srcblt_op_stuck         = 0;
            g_gfx_cfg_m14.workaround.write_op_stuck          = 0;
        }
        break;

        default:
        {
            g_gfx_cfg_m14.workaround.bad_dst_addr_stuck      = 0;
            g_gfx_cfg_m14.workaround.scaler_read_buf_stuck   = 0;
            g_gfx_cfg_m14.workaround.srcblt_op_stuck         = 0;
            g_gfx_cfg_m14.workaround.write_op_stuck          = 0;
        }
        break;
    }
}

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/

/** GFX interrupt handler
 *
 */
irqreturn_t GFX_M14_ISRHandler(int irq, void *dev_id, struct pt_regs *regs)
{
//	if ( GFX_IsGfxIdle() )
	{
//		GFX_PRINT("#### GFX ISR - IDLE OK ####\n");
		GFX_WakeupWaitSync();
	}

	g_gfx_reg_m14->gfx_intr_clear.gfx_intr_clear = 0x1;
	GFX_PRINT("#### GFX ISR ####\n");

	return IRQ_HANDLED;
}

/** initialize M14 hardware
*
* @return RET_OK when success, RET_ERROR otherwise
*/

int GFX_M14_InitHW(void)
{
	static BOOLEAN b_os_init = FALSE;

	if ( b_os_init == FALSE )
	{
		/* do ioremap */
		g_gfx_reg_m14 =(GFX_M14_REG_T *)ioremap( gpGfxRegCfg->reg_base_addr, gpGfxRegCfg->reg_size);
		GFX_CHECK_ERROR( NULL == g_gfx_reg_m14, return RET_ERROR, "out of memory. can't ioremap\n");

		g_gfx_reg_m14_cache = (GFX_M14_REG_T *)OS_Malloc( gpGfxRegCfg->reg_size );
		GFX_CHECK_ERROR( g_gfx_reg_m14_cache == NULL, return RET_ERROR, "out of memory\n");

		memset( g_gfx_reg_m14_cache, 0x0, gpGfxRegCfg->reg_size );

		/* register GFX interrupt handler */
		GFX_CHECK_ERROR( request_irq( gpGfxRegCfg->irq_num, (irq_handler_t)GFX_M14_ISRHandler, 0, "gfx_irq", NULL), /* nop */, "request_irq failed\n" );

		b_os_init = TRUE;
	}

	/* Reset GFX H/W */	
	GFX_M14_SwResetCommand( );


	g_gfx_reg_m14->gfx_intr_ctrl.intr_gen_mode = 0;	// batch command finish
	g_gfx_reg_m14->gfx_intr_ctrl.intr_en = 1;		// interrupt enable

	GFX_M14_InitScaler( );

	return 0;
}

/** shutdown M14 hardware 
 *
 * @return RET_OK when success, RET_ERROR otherwise
 */
int 			GFX_M14_ShutdownHW	(void)
{
	g_gfx_reg_m14->gfx_intr_ctrl.intr_en = 0;	// interrupt disable

	/* [TODO] more cleanup !!! */

    iounmap((void *)g_gfx_reg_m14);

	return 0;
}

/** @} */

