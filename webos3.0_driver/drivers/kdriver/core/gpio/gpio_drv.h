/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 2013 by LG Electronics Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

/** @file
 *
 *  driver interface header for gpio device. ( used only within kdriver )
 *	gpio device will teach you how to make device driver with new platform.
 *
 *  @author		ingyu.yang (ingyu.yang@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_gpio
 *	@{
 */

#ifndef	_GPIO_DRV_H_
#define	_GPIO_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "debug_util.h"
#include "gpio_cfg.h"
#include "gpio_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	GPIO_PRINT(format, args...)		DBG_PRINT(  g_gpio_debug_fd, GPIO_MSG_TRACE, format, ##args)
#define	GPIO_INFO(format, args...)		DBG_PRINT( g_gpio_debug_fd, GPIO_MSG_INFO, format, ##args)
#define	GPIO_DEBUG(format, args...)		DBG_PRINT( g_gpio_debug_fd, GPIO_MSG_DEBUG, format, ##args)
#define	GPIO_ERROR(format, args...)		DBG_PRINT( g_gpio_debug_fd, GPIO_MSG_ERROR, format, ##args)
#define	GPIO_TRACE(format, args...)		DBG_PRINT( g_gpio_debug_fd, GPIO_MSG_TRACE, format, ##args)
#define	GPIO_NOTI(format, args...)		DBG_PRINT( g_gpio_debug_fd, GPIO_MSG_NOTI, format, ##args)

#define GPIO_CHECK_CODE(__checker,__if_action,fmt,args...)   \
			 __CHECK_IF_ERROR(__checker, GPIO_ERROR, __if_action , fmt, ##args )

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	GPIO_MSG_ERROR = LX_LOGM_LEVEL_ERROR,
	GPIO_MSG_WARN = LX_LOGM_LEVEL_WARNING,
	GPIO_MSG_NOTI = LX_LOGM_LEVEL_NOTI,
	GPIO_MSG_INFO = LX_LOGM_LEVEL_INFO,
	GPIO_MSG_DEBUG = LX_LOGM_LEVEL_DEBUG,
	GPIO_MSG_TRACE = LX_LOGM_LEVEL_TRACE,
	GPIO_MSG_NORMAL = (LX_LOGM_LEVEL_TRACE + 1),	// print
	GPIO_MSG_VSYNC = (LX_LOGM_LEVEL_TRACE + 2),		// vsync
	GPIO_MSG_TIMER = (LX_LOGM_LEVEL_TRACE + 3),		// timer
	GPIO_MSG_INTER = (LX_LOGM_LEVEL_TRACE + 4),		// interrupt

	GPIO_MSG_INDEX_COUNT
} GPIO_MSG_TYPE_T;
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void     GPIO_PreInit(void);
extern	int      GPIO_Init(void);
extern	void     GPIO_Cleanup(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_gpio_debug_fd;


extern int gpio_intr_pin;
extern int gpio_intr_pin_value;
extern UINT32	g_poll_intr_idx;
extern UINT32 gpio_intrarray_used[GPIO_IRQ_NUM_NR];
extern UINT32 gpio_pollarray_context[GPIO_IRQ_NUM_NR];
extern GPIO_INTR_INFO_T	 gpio_isr_info[GPIO_PIN_MAX];

extern spinlock_t gpioPoll_lock;
extern wait_queue_head_t gGPIOPollWaitQueue;

extern wait_queue_head_t gGPIOPollWaitQueue_00_02;
extern wait_queue_head_t gGPIOPollWaitQueue_03_05;
extern wait_queue_head_t gGPIOPollWaitQueue_06_08;
extern wait_queue_head_t gGPIOPollWaitQueue_09_11;
extern wait_queue_head_t gGPIOPollWaitQueue_12_14;
extern wait_queue_head_t gGPIOPollWaitQueue_15_17;

extern wait_queue_head_t* gGPIOPollWaitQueueT[GPIO_IRQ_NUM_NR];
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GPIO_DRV_H_ */

/** @} */
