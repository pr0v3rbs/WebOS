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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.02.17
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
//#define USE_IPC_FOR_MCU1

/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>

#include "base_types.h"
#include "os_util.h"

#include "de_ver_def.h"

#include "de_kapi.h"
#include "de_def.h"

#include "de_int_def.h"
#include "de_ipc_def.h"
#include "de_ipc.h"
#include "de_int.h"
#include "hal/de_hal_def.h"
#include "hal/ipc_phys_hal.h"

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef struct {
	int (*cb_func)(void);
} DE_VSYNC_CB;
DE_VSYNC_CB vsync_cb;
DE_VSYNC_CB seamless_cb;

/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   External Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *----------------------------------------------------------------------------*/
static void VIDEO_CPU_VSYNC_Tasklet(unsigned long temp);
static void DE_IPC_Handler(void);
static void DE_VSYNC_Handler(void);
static void DE_SEAMLESS_Callback(unsigned long temp);
static irqreturn_t DE_IPC_interrupt(	int irq,      \
										void *dev_id, \
										struct pt_regs *regs);
#ifdef USE_IPC_FOR_MCU1
static irqreturn_t BE_IPC_interrupt( 	int irq,      \
										void *dev_id, \
										struct pt_regs *regs);
#endif
static irqreturn_t DE_VSYNC_interrupt(	int irq,      \
										void *dev_id, \
										struct pt_regs *regs);

/*------------------------------------------------------------------------------
 *   Static Variables
 *----------------------------------------------------------------------------*/
// mcu 0
static DECLARE_TASKLET(Video_WDG_tasklet, VIDEO_CPU_WDG_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_IPC_tasklet, VIDEO_CPU_ISR_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_DMA_tasklet, VIDEO_CPU_DMA_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_JPG_tasklet, VIDEO_CPU_JPG_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_USB_tasklet, VIDEO_CPU_USB_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_WEL_tasklet, VIDEO_CPU_WEL_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_WER_tasklet, VIDEO_CPU_WER_FROM_MCU,  0UL);
static DECLARE_TASKLET(Video_AMZ_tasklet, DE_SEAMLESS_Callback,  0UL);
static DECLARE_TASKLET(Video_VSYNC_tasklet, VIDEO_CPU_VSYNC_Tasklet,  0UL);

// mcu 1
#ifdef USE_IPC_FOR_MCU1
static DECLARE_TASKLET(Video_WDG_1_tasklet, VIDEO_CPU_WDG_FROM_MCU_1,  0UL);
static DECLARE_TASKLET(Video_IPC_1_tasklet, VIDEO_CPU_ISR_FROM_MCU_1,  0UL);
static DECLARE_TASKLET(Video_ACK_1_tasklet, VIDEO_CPU_ACK_FROM_MCU_1,  0UL);
#endif

static DECLARE_WAIT_QUEUE_HEAD(wqVsync);
static int de_vsync_event_wakeup;

/*==============================================================================
 *   Implementation Group
 *============================================================================*/

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt request
 */
int DE_INT_Init(void)
{
	int ret = RET_OK;
	int i;
	int num_de_ipc_irq = 0, num_de_irq = 0;

	for(i=SOC_MCU_0;i<SOC_MCU_MAX;i++)
	{
		if(ipc_phys_hal_GetIrqNum(i, &num_de_ipc_irq, &num_de_irq)) {
			DE_ERROR("get irq num in %s is failed\n", __FUNCTION__);
			return -1;
		}
		switch(i)
		{
			case SOC_MCU_0:
				if (request_irq(num_de_ipc_irq,             \
							(irq_handler_t)DE_IPC_interrupt,\
							0,"DE2IPC", NULL)) {
					DE_ERROR("request_irq in %s is failed\n", __FUNCTION__);
					return -1;
				}

				if (request_irq(num_de_irq,                   \
							(irq_handler_t)DE_VSYNC_interrupt,\
							0,"DE2VSYNC", NULL)) {
					DE_ERROR("request_irq in %s is failed\n", __FUNCTION__);
					return -1;
				}
				break;
			case SOC_MCU_1:
#ifdef USE_IPC_FOR_MCU1
				if (request_irq(num_de_ipc_irq,             \
							(irq_handler_t)BE_IPC_interrupt,\
							0,"BE2IPC", NULL)) {
					DE_ERROR("request_irq in %s is failed\n", __FUNCTION__);
					return -1;
				}
#else
				DE_NOTI("Currently not supported mcu[%d]\n", i);
#endif
				break;
		}

	}
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt free
 */
int DE_INT_Cleanup(void)
{
	int ret = RET_OK;
	int num_de_irq = 0, num_de_ipc_irq = 0;

	if(ipc_phys_hal_GetIrqNum(0, &num_de_ipc_irq, &num_de_irq)) {
		DE_ERROR("get irq num in %s is failed\n", __FUNCTION__);
		return -1;
	}

	free_irq(num_de_ipc_irq, NULL);
	free_irq(num_de_irq, NULL);

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief register call-back function for vsync
 */
void DE_VSYNC_Register(char *str, int (*func)(void))
{
	int intr = 0;
	if(!strcmp(str, "vsync"))
		intr = 0;
	else if(!strcmp(str, "seamless"))
		intr = 1;
	else
		intr = -1;
	switch(intr)
	{
		case 0:
			vsync_cb.cb_func = func;
			break;
		case 1:
			seamless_cb.cb_func = func;
			break;
		default:
			printk("%s : undefined handler!\n", __F__);
			break;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief initialize vsync
 */
int DE_VSYNC_Init(void)
{
	int ret = RET_OK;
	ret = ipc_phys_hal_InitInterrupt(0, TRUE);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief free vsync
 */
int DE_VSYNC_Free(void)
{
	int ret = RET_OK;
	ret = ipc_phys_hal_InitInterrupt(0, FALSE);
	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief wait vsync with count-param
 */
int DE_VSYNC_WaitTimeOut(int timeout)
{
	if (timeout < 0) {
		de_vsync_event_wakeup = timeout;
		wait_event_interruptible(wqVsync, !de_vsync_event_wakeup);
	} else {
		de_vsync_event_wakeup = -1;
		wait_event_interruptible_timeout(wqVsync,              \
										!de_vsync_event_wakeup,\
										(long)timeout);
	}
	return RET_OK;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief interrupt handler which is comming from MCU
 */
static void DE_IPC_Handler(void)
{
	UINT32 ipcStatus;

	ipc_phys_hal_GetIntrStatus(0, VIDEO_IPC_CPU, &ipcStatus);
	if (GET_CM(ipcStatus, VIDEO_WDG_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearWDGStatus(0);
		tasklet_schedule(&Video_WDG_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_IPC_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_IPC_CPU, &ipcStatus);
		tasklet_schedule(&Video_IPC_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_DMA_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_DMA_CPU, &ipcStatus);
		tasklet_schedule(&Video_DMA_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_JPG_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_JPG_CPU, &ipcStatus);
		tasklet_schedule(&Video_JPG_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_USB_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_USB_CPU, &ipcStatus);
		tasklet_schedule(&Video_USB_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_WEL_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_WEL_CPU, &ipcStatus);
		tasklet_schedule(&Video_WEL_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_WER_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_WER_CPU, &ipcStatus);
		tasklet_schedule(&Video_WER_tasklet);
	}
	if (GET_CM(ipcStatus, VIDEO_AMZ_INTERRUPT_ARM_MASK)) {
		ipc_phys_hal_ClearIPCStatus(0, VIDEO_AMZ_CPU, &ipcStatus);
		tasklet_schedule(&Video_AMZ_tasklet);
	}	
}

#ifdef USE_IPC_FOR_MCU1
static void BE_IPC_Handler(void)
{
	int ret;
	UINT32 intrStatus;

	printk("%s : \n", __F__);

	ret = ipc_phys_hal_GetIntrStatus(1, VIDEO_INT_SWI, &intrStatus);
	if(intrStatus & MCU1_SWI_IPC)
	{	
		tasklet_schedule(&Video_IPC_1_tasklet);
	}
	if(intrStatus & MCU1_SWI_ACK)
	{
		tasklet_schedule(&Video_ACK_1_tasklet);
	}
	// clear interrupt
	if(intrStatus) ipc_phys_hal_ClearIPCStatus(1, VIDEO_INT_SWI, &intrStatus);

	ret = ipc_phys_hal_GetIntrStatus(1, VIDEO_INT_HWI, &intrStatus);

	if(intrStatus & (1 << MCU1_CPU_INT_WDG))
	{
		tasklet_schedule(&Video_WDG_1_tasklet);
	}
	if(intrStatus & (1 << MCU1_CPU_INT_DMA))
	{ }
	// clear interrupt
	if(intrStatus) ipc_phys_hal_ClearIPCStatus(1, VIDEO_INT_HWI, &intrStatus);
}
#endif

static void DE_VSYNC_Handler(void)
{
	do {
		if (!ipc_phys_hal_CheckIrq4Vsync(0)) break;
		tasklet_schedule(&Video_VSYNC_tasklet);
	} while (0);
}

static void DE_SEAMLESS_Callback(unsigned long temp)
{
	seamless_cb.cb_func();
}

static void DE_VSYNC_Callback(void)
{
	vsync_cb.cb_func();
}

static void DE_VSYNC_WakeUp(void)
{
	if (de_vsync_event_wakeup < 0) de_vsync_event_wakeup++;
	wake_up_interruptible(&wqVsync);
}

static void VIDEO_CPU_VSYNC_Tasklet(unsigned long temp)
{
	DE_VSYNC_WakeUp();
	DE_VSYNC_Callback();
}

static irqreturn_t DE_IPC_interrupt( 	int irq,      \
										void *dev_id, \
										struct pt_regs *regs)
{
    DE_IPC_Handler();

	return IRQ_HANDLED;
}

#ifdef USE_IPC_FOR_MCU1
static irqreturn_t BE_IPC_interrupt( 	int irq,      \
										void *dev_id, \
										struct pt_regs *regs)
{
    BE_IPC_Handler();

	return IRQ_HANDLED;
}
#endif

static irqreturn_t DE_VSYNC_interrupt(	int irq,      \
										void *dev_id, \
										struct pt_regs *regs)
{
    DE_VSYNC_Handler();

	return IRQ_HANDLED;
}

/** @} */
