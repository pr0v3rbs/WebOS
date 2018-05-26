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
#include "gfx_reg_h13.h"

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
GFX_H13_MEM_MAP_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void		GFX_H13_stop(void);
extern int 		GFX_H13_RunFlushCommand(void);
extern int 		GFX_H13_RunStartCommand(void);
extern int 		GFX_H13_PauseCommand(void);
extern int		GFX_H13_ResumeCommand(void);
extern int		GFX_H13_StopCommand(void);
extern int		GFX_H13_SwResetCommand(void);
extern void		GFX_H13_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue);
extern BOOLEAN	GFX_H13_IsGfxIdle(void);
extern void		GFX_H13_GetOperationStatus(GFX_CMD_OP_T *operate);
extern void		GFX_H13_SetOperationStatus(GFX_CMD_OP_T *operate);
extern void		GFX_H13_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port);
extern void		GFX_H13_SetInputConfigure(GFX_PORT_CONFIGURE_T *port);
extern void		GFX_H13_SetBlendingOut(GFX_ENUM_OUT_T type);
extern void		GFX_H13_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
extern void		GFX_H13_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
extern void		GFX_H13_GetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
extern void		GFX_H13_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
extern void		GFX_H13_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler);
extern void		GFX_H13_SetInterruptMode(UINT32 uiMode);
extern void		GFX_H13_SetAckMode(UINT32 *mode);
extern void		GFX_H13_GetAckMode(UINT32 *mode);
extern int		GFX_H13_SetCLUT(int port, int size , UINT32 *data);
extern void		GFX_H13_SetCommandDelayReg(UINT16 delay);
extern UINT16 	GFX_H13_GetCommandDelayReg(void);
extern void		GFX_H13_DumpRegister(void);
extern int 		GFX_H13_RunSuspend(void);
extern int 		GFX_H13_RunResume(void);

extern void		GFX_H13_InitScaler(void);

extern int		GFX_H13_RunClkOnCommand(void);
extern int		GFX_H13_RunClkOffCommand(void);
extern BOOLEAN	GFX_H13_ClkCheck(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile GFX_H13_REG_T*	g_gfx_reg_h13;
GFX_H13_REG_T*				g_gfx_reg_h13_cache;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
const LX_GFX_CFG_T*	GFX_H13_GetCfg		(void);
int 				GFX_H13_InitHW		(void);
int 				GFX_H13_ShutdownHW	(void);
irqreturn_t 		GFX_H13_ISRHandler	(int irq, void *dev_id, struct pt_regs *regs);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
const static	GFX_HAL_T		g_gfx_hal_h13 =
{
	.GetCfg					= GFX_H13_GetCfg,
	.InitHW					= GFX_H13_InitHW,
	.ShutdownHW				= GFX_H13_ShutdownHW,
	.RunSuspend				= GFX_H13_RunSuspend,
	.RunResume				= GFX_H13_RunResume,

	.RunFlushCommand		= GFX_H13_RunFlushCommand,
	.RunStartCommand		= GFX_H13_RunStartCommand,
	.RunPauseCommand		= GFX_H13_PauseCommand,
	.RunResumeCommand		= GFX_H13_ResumeCommand,
	.RunStopCommand			= GFX_H13_StopCommand,
	.RunSwResetCommand		= GFX_H13_SwResetCommand,

	.GetComQueueStatus		= GFX_H13_GetComQueueStatus,

	.GetOperationStatus		= GFX_H13_GetOperationStatus,
	.SetOperationStatus		= GFX_H13_SetOperationStatus,
	.GetInputConfigure		= GFX_H13_GetInputConfigure,
	.SetInputConfigure		= GFX_H13_SetInputConfigure,
	.GetBlendConfigure		= GFX_H13_GetBlendConfigure,
	.SetBlendConfigure		= GFX_H13_SetBlendConfigure,
	.SetBlendingOut			= GFX_H13_SetBlendingOut,
	.GetOutputConfigure		= GFX_H13_GetOutputConfigure,
	.SetOutputConfigure		= GFX_H13_SetOutputConfigure,
	.SetScalerConfigure		= GFX_H13_SetScalerConfigure,
	.SetCLUT				= GFX_H13_SetCLUT,
	.SetCommandDelayReg		= GFX_H13_SetCommandDelayReg,
	.GetCommandDelayReg		= GFX_H13_GetCommandDelayReg,

	.IsGfxIdle				= GFX_H13_IsGfxIdle,

	.DumpRegister			= GFX_H13_DumpRegister,

	.RunClkOnCommand		= GFX_H13_RunClkOnCommand,
	.RunClkOffCommand		= GFX_H13_RunClkOffCommand,
	.ClkCheck				= GFX_H13_ClkCheck
};

static	LX_GFX_CFG_T 		g_gfx_cfg_h13 =
{
    .b_hw_scaler 			= TRUE,
    .surface_blit_cmd_delay = 0xffff,		// normal gfx operation by APP.
    .screen_blit_cmd_delay  = 0xffff,		// internal gfx operation by KDRV.
	.sync_wait_timeout		= 60,			//20, /* for FPGA verification */
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

/** get H13 specific configuration
 *
 *  @return LX_GFX_CFG_T
 */
const LX_GFX_CFG_T*   GFX_H13_GetCfg(void)
{
    return &g_gfx_cfg_h13;
}

void	GFX_H13_InitHAL( GFX_HAL_T*	hal )
{
	memcpy( hal, &g_gfx_hal_h13, sizeof(GFX_HAL_T));

    switch( lx_chip_rev() )
    {
        case LX_CHIP_REV(H13,A0):
        {
            g_gfx_cfg_h13.workaround.bad_dst_addr_stuck      = 0;
            g_gfx_cfg_h13.workaround.scaler_read_buf_stuck   = 0;
            g_gfx_cfg_h13.workaround.srcblt_op_stuck         = 0;
            g_gfx_cfg_h13.workaround.write_op_stuck          = 0;
        }
        break;

        default:
        {
            g_gfx_cfg_h13.workaround.bad_dst_addr_stuck      = 0;
            g_gfx_cfg_h13.workaround.scaler_read_buf_stuck   = 0;
            g_gfx_cfg_h13.workaround.srcblt_op_stuck         = 0;
            g_gfx_cfg_h13.workaround.write_op_stuck          = 0;
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
irqreturn_t GFX_H13_ISRHandler(int irq, void *dev_id, struct pt_regs *regs)
{
//	if ( GFX_IsGfxIdle() )
	{
//		GFX_PRINT("#### GFX ISR - IDLE OK ####\n");
		GFX_WakeupWaitSync();
	}

	g_gfx_reg_h13->gfx_intr_clear.gfx_intr_clear = 0x1;
	GFX_PRINT("#### GFX ISR ####\n");

	return IRQ_HANDLED;
}

/** initialize H13 hardware
*
* @return RET_OK when success, RET_ERROR otherwise
*/

int GFX_H13_InitHW(void)
{
	static BOOLEAN b_os_init = FALSE;

	if ( b_os_init == FALSE )
	{
		/* do ioremap */
		g_gfx_reg_h13 =(GFX_H13_REG_T *)ioremap( gpGfxRegCfg->reg_base_addr, gpGfxRegCfg->reg_size);
		GFX_CHECK_ERROR( NULL == g_gfx_reg_h13, return RET_ERROR, "out of memory. can't ioremap\n");

		g_gfx_reg_h13_cache = (GFX_H13_REG_T *)OS_Malloc( gpGfxRegCfg->reg_size );
		GFX_CHECK_ERROR( g_gfx_reg_h13_cache == NULL, return RET_ERROR, "out of memory\n");

		memset( g_gfx_reg_h13_cache, 0x0, gpGfxRegCfg->reg_size );

		/* register GFX interrupt handler */
		GFX_CHECK_ERROR( request_irq( gpGfxRegCfg->irq_num, (irq_handler_t)GFX_H13_ISRHandler, 0, "gfx_irq", NULL), /* nop */, "request_irq failed\n" );

		b_os_init = TRUE;
	}

	/* Reset GFX H/W */	
	GFX_H13_SwResetCommand( );


	g_gfx_reg_h13->gfx_intr_ctrl.intr_gen_mode = 0;		// batch command finish
	g_gfx_reg_h13->gfx_intr_ctrl.intr_en		= 1;	// interrupt enable

	GFX_H13_InitScaler( );

	return 0;
}

/** shutdown H13 hardware 
 *
 * @return RET_OK when success, RET_ERROR otherwise
 */
int 			GFX_H13_ShutdownHW	(void)
{
	/* clear interrupt */	
	g_gfx_reg_h13->gfx_intr_ctrl.intr_en = 0;
	free_irq( gpGfxRegCfg->irq_num, NULL );

//	iounmap((void *)g_gfx_reg_h13);

	return 0;
}

/** @} */

