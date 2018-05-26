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

/**
 * @file
 *
 *  main driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <linux/file.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_queue.h"
#include "vip_drv_hal.h"
#include "vip_drv_img.h"
#include "vip_mem.h"
#include "vip_proc.h"

#if defined(VIP_PM_GATING)
#include "pm_kapi.h"
#endif

#ifdef USE_KDRV_CODES_FOR_H15
#include "./h15/vip_reg_h15_a0.h"
#endif

/*
 *
 */
#define	VIP_RUN_WAIT_TIMEOUT	(1000*1000)	// 1 sec
#define VIP_POLLING_INTERVAL	(1000)		// 1 msec
static const char *_vip_fault_name[VIP_FAULT_COMPLETION_MAX] = {
	[VIP_FAULT_COMPLETION_NONE]		= "No fault",
	[VIP_FAULT_COMPLETION_POLLING]	= "Polling error",
	[VIP_FAULT_COMPLETION_WAITING]	= "Waiting error",
	[VIP_FAULT_COMPLETION_ABORTED]	= "Aborted by user",
	[VIP_FAULT_COMPLETION_RUN]		= "Run response error",
};

/*
 *
 */
#if 1
static DEFINE_MUTEX(vip_io_mutex);
int vip_io_lock(void)
{
	int ret = 0;
#if 0
	ret = mutex_lock_interruptible(&vip_io_mutex);
	if (ret) {
		vipm_err("mutex_lock_interruptible failed, %d\n", ret);
	}
#else
	mutex_lock(&vip_io_mutex);
#endif
	return ret;
}

void vip_io_unlock(void)
{
	mutex_unlock(&vip_io_mutex);
}
#else
int vip_io_lock(void)
{
	return 0;
}
void vip_io_unlock(void)
{

}
#endif

/*
 *
 */
#define	__vip_sel_hal_ops(v, f, p)			((v && ((v)->f))? (v)->f(p) : -ESRCH)
#define	__vip_sel_hal_ops_no_err(v, f, p)	((v && ((v)->f))? (v)->f(p) : 0)
#define	__vip_prt_err(_ret)								\
		do {											\
			if (_ret) vipm_err("failed : %d\n", _ret);	\
		} while(0)

static int VIP_HW_Init(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops(vip->vhal_ops, init, vip);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Exit(struct vip_dev *vip)
{
	int ret =__vip_sel_hal_ops(vip->vhal_ops, exit, vip);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Reset(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops(vip->vhal_ops, reset, vip);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Sleep(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops_no_err(vip->vhal_ops, sleep, vip);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Resume(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops_no_err(vip->vhal_ops, resume, vip);
	__vip_prt_err(ret);
	return ret;
}

static int __used VIP_HW_Scene_Change(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops_no_err(vip->vhal_ops, scene_change, vip);
	__vip_prt_err(ret);
	return ret;
}

static int __used VIP_HW_Set_Mode(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops_no_err(vip->vhal_ops, setmode, vip);
	__vip_prt_err(ret);
	return ret;
}

static int __used VIP_HW_Set_Res(struct vip_dev *vip)
{
	int ret = __vip_sel_hal_ops_no_err(vip->vhal_ops, setres, vip);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Run(struct vip_req *areq)
{
	int ret = __vip_sel_hal_ops(areq->vhal_ops, run, areq);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Msg(struct vip_req *areq)
{
	int ret = __vip_sel_hal_ops(areq->vhal_ops, msg, areq);
	__vip_prt_err(ret);
	return ret;
}

static int VIP_HW_Poll(struct vip_req *areq)
{
	int ret = __vip_sel_hal_ops(areq->vhal_ops, poll, areq);
//	__vip_prt_err(ret);
	return ret;
}

/*
 *
 */
int VIP_Core_HW_Init(struct vip_dev *vip)
{
	int ret;

#if defined(VIP_FIRM_DOWN_FROM_KADAP)
	if (!vip->firm_ctrl.addr ||
		!vip->firm_ctrl.size ||
		vip->firm_ctrl.magic != LX_VIP_IMG_MAGIC)
	{
		return -EINVAL;
	}
#endif

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_HW_Init(vip);
	if (!ret) {
		VIP_Set_HW_Inited(vip);
	}

	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

int VIP_Core_Reset(struct vip_dev *vip)
{
	int ret;

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_HW_Reset(vip);

	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

int VIP_Core_Reset_Check(struct vip_dev *vip)
{
	int ret = 0;
	if (vip_test_bit(VIP_FLAG_RESET, &vip->flags)) {
		ret = VIP_Core_Reset(vip);
		vip->priv_data = ret;
		vip_clear_bit(VIP_FLAG_RESET, &vip->flags);
	}
	return ret;
}

#define VIP_CORE_RESET_WAIT_MAX			(10 * 1000 * 1000)	// 10 sec
#define VIP_CORE_RESET_WAIT_INTERVAL	100					// 100 usec
static int VIP_Core_Reset_Wait(struct vip_dev *vip)
{
	unsigned long time_limit;
	int ret;

	time_limit = VIP_CORE_RESET_WAIT_MAX / VIP_CORE_RESET_WAIT_INTERVAL;

	while (1) {
		if (!vip_test_bit(VIP_FLAG_RESET, &vip->flags)) {
			break;
		}
		time_limit--;
		if (!time_limit) {
			vipm_debug_core("Timeout\n");
			return -ETIMEDOUT;
		}
		VIP_Delay_US(VIP_CORE_RESET_WAIT_INTERVAL);
	}
	ret = vip->priv_data;

	return ret;
}

int VIP_Core_Reset_Request(struct vip_dev *vip)
{
	int ret = 0;

	if (!VIP_Is_Inited(vip) || !VIP_Is_HW_Inited(vip)) {
		return 0;
	}
	if (vip->thread_handler == NULL) {
		return 0;
	}

	spin_lock(vip->lock);
	vip_set_bit(VIP_FLAG_RESET, &vip->flags);
	spin_unlock(vip->lock);

	ret = VIP_Core_Reset_Wait(vip);

	return ret;
}

int VIP_Core_Sleep(struct vip_dev *vip)
{
	int ret = 0;

	spin_lock(vip->lock);
	if (!vip_test_bit(VIP_FLAG_SLEEP, &vip->flags)) {
		ret = VIP_HW_Sleep(vip);
		if (!ret) {
			vip_set_bit(VIP_FLAG_SLEEP, &vip->flags);
		//	vipm_debug_core("\n");
		}
	}
	spin_unlock(vip->lock);

	return ret;
}

int VIP_Core_Resume(struct vip_dev *vip)
{
	int ret = 0;

	spin_lock(vip->lock);
	if (vip_test_bit(VIP_FLAG_SLEEP, &vip->flags)) {
		ret = VIP_HW_Resume(vip);
		if (!ret) {
			vip_clear_bit(VIP_FLAG_SLEEP, &vip->flags);
		//	vipm_debug_core("\n");
		}
	}
	spin_unlock(vip->lock);

	return ret;
}

int VIP_Core_Scene_Change(struct vip_dev *vip)
{
	int ret;

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_HW_Scene_Change(vip);

	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

int VIP_Core_Set_Mode(struct vip_dev *vip)
{
	int ret;

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_HW_Set_Mode(vip);

	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

int VIP_Core_Set_Res(struct vip_dev *vip)
{
	int ret;

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_HW_Set_Res(vip);

	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

/*
 *
 */
#if defined(VIP_PM_GATING)
extern int PM_SetClockGating(u32 Module, u32 SubModule, u32 state);
extern int PM_GetClockGating(u32 Module, u32 SubModule, u32 *state);
#endif

static int VIP_PM_GetClockGating(struct vip_dev *vip)
{
	int ret = 0;

	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

#if defined(VIP_PM_GATING)
//	spin_lock(vip->lock);

	vipm_debug_core("vip            = 0x%p\n", vip);
	vipm_debug_core("vip->vtop->idx = %d\n", vip->vtop->idx);
	ret = PM_GetClockGating(PM_VIP, PM_VIP_0 + vip->vtop->idx, &vip->pm_state);
	vipm_debug_core("vip->pm_state  = %d\n", vip->pm_state);
	if (ret) {
		vipm_err("PM_GetClockGating failed(%d)\n", ret);
		goto out;
	}

	vipm_debug_core("Get vip->pm_state[%d]\n", vip->pm_state);

out:
//	spin_unlock(vip->lock);
#endif

	return ret;
}

static int VIP_PM_SetClockGating(struct vip_dev *vip, int pm_on)
{
	int ret = 0;
	unsigned int pm_state = 0;

	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

#if defined(VIP_PM_GATING)
//	spin_lock(vip->lock);

	if (vip->pm_state == PM_CLK_ON) {
		if (pm_on == PM_CLK_ON) {
			goto out;
		}
	} else {
		if (pm_on == PM_CLK_OFF) {
			goto out;
		}
	}

	pm_state = (pm_on == PM_CLK_ON)? PM_CLK_ON : PM_CLK_OFF;
	vipm_debug_core("vip            = 0x%p\n", vip);
	vipm_debug_core("vip->vtop->idx = %d\n", vip->vtop->idx);
	ret = PM_SetClockGating(PM_VIP, PM_VIP_0 + vip->vtop->idx, pm_state);
	vipm_debug_core("pm_state       = %d\n", pm_state);
	if (ret) {
		vipm_err("PM_SetClockGating failed(%d)\n", ret);
		goto out;
	}

	ret = PM_GetClockGating(PM_VIP, PM_VIP_0 + vip->vtop->idx, &vip->pm_state);
	vipm_debug_core("vip->pm_state  = %d\n", vip->pm_state);
	if (ret) {
		vipm_err("PM_GetClockGating failed(%d)\n", ret);
		goto out;
	}

	if (vip->pm_state != pm_state) {
		vipm_err("Err : vip->pm_state[%d] != pm_statep[%d]n",
				vip->pm_state, pm_state);
		goto out;
	}

	vipm_debug_core("Set vip->pm_state[%d]\n", vip->pm_state);

out:
//	spin_unlock(vip->lock);
#endif

	return ret;
}

/*
 *
 */
int VIP_Clk_Put(struct vip_dev *vip)
{
	int ret = 0;

	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

	spin_lock(vip->lock);
	if (vip_test_bit(VIP_FLAG_CLK_GET, &vip->flags)) {
		ret = VIP_PM_SetClockGating(vip, PM_CLK_OFF);
		if (ret) {
			goto out;
		}
		vip_clear_bit(VIP_FLAG_CLK_GET, &vip->flags);

		vipm_debug_core("clk put done\n");
	//	vipm_noti("clk put done\n");
	}
out:
	spin_unlock(vip->lock);

	return ret;
}

int VIP_Clk_Get(struct vip_dev *vip)
{
	int ret = 0;

	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

	spin_lock(vip->lock);
	if (!vip_test_bit(VIP_FLAG_CLK_GET, &vip->flags)) {
		ret = VIP_PM_GetClockGating(vip);
		if (ret) {
			goto out;
		}
		ret = VIP_PM_SetClockGating(vip, PM_CLK_ON);
		if (ret) {
			goto out;
		}
		vip_set_bit(VIP_FLAG_CLK_GET, &vip->flags);

		vipm_debug_core("clk get done\n");
	//	vipm_noti("clk get done\n");
	}
out:
	spin_unlock(vip->lock);

	return ret;
}

int VIP_Disable_IRQ(struct vip_dev *vip)
{
	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

#if (__VIP_USE_INT_ENABLE > 0)
	if (vip->irq_handler) {
		struct irq_desc *desc = irq_to_desc(vip->irq);
		if (desc && !desc->depth) {
			disable_irq(vip->irq);
			if (desc->depth) {
				vipm_debug_core("irq[%d] disabled\n", vip->irq);
			}
		}
	}
#endif
	return 0;
}

int VIP_Enable_IRQ(struct vip_dev *vip)
{
	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

#if (__VIP_USE_INT_ENABLE > 0)
	if (vip->irq_handler) {
		struct irq_desc *desc = irq_to_desc(vip->irq);
		if (desc && desc->depth) {
			enable_irq(vip->irq);
			if (!desc->depth) {
				vipm_debug_core("irq[%d] enabled\n", vip->irq);
			}
		}
	}
#endif
	return 0;
}

int VIP_Slumber(struct vip_dev *vip, int opt)
{
	int ret;

	ret = VIP_Core_Sleep(vip);
	if (ret) {
		vipm_err("core sleep err\n");
		goto out;
	}
	if (opt) {
		VIP_Disable_IRQ(vip);
	}
	ret = VIP_Clk_Put(vip);
	if (ret) {
		vipm_err("clk put err\n");
		goto out;
	}

out:
	return ret;
}

int VIP_Wakeup(struct vip_dev *vip, int opt)
{
	int ret;

	ret = VIP_Clk_Get(vip);
	if (ret) {
		vipm_err("clk get err\n");
		goto out;
	}
	if (opt) {
		VIP_Enable_IRQ(vip);
	}
	ret = VIP_Core_Resume(vip);
	if (ret) {
		vipm_err("core resume err\n");
		goto out;
	}

out:
	return ret;
}

static int VIP_Release_IRQ(struct vip_dev *vip)
{
	if (vip && vip->irq_handler) {
		free_irq(vip->irq, vip);
		vipm_debug_core("vip[0x%p] irq[%d] released\n", vip, vip->irq);
		return 0;
	}
	return -ENODEV;
}

static int _VIP_Request_IRQ(struct vip_dev *vip, void *irq_handler)
{
	int ret = 0;

	if (vip && vip->irq) {
		if (irq_handler == NULL) {
			vipm_err("NULL irq_handler\n");
			return -EINVAL;
		}

		ret = request_irq(vip->irq,
						(irq_handler_t)irq_handler,
						IRQF_SHARED | IRQF_TRIGGER_HIGH,
					//	IRQF_SHARED | IRQF_TRIGGER_RISING,
						vip_dev_name(vip), vip);
		vipm_info("request_irq[%d] %s, %d\n",
				vip->irq, (ret)?"failed":"done", ret);
		if (!ret) {
			vip->irq_handler = irq_handler;
		}
	}
	return ret;
}

/*
 *
 */
#if (__VIP_USE_INPUT_DEVICE > 0)
static int VIP_Unregister_Input(struct vip_dev *vip)
{
	if (vip->input) {
		input_unregister_device(vip->input);
		vipm_debug_core("vip->input unregisered\n");
		vip->input = NULL;
	}
	return 0;
}

static int VIP_Register_Input(struct vip_dev *vip)
{
	struct input_dev *input;
	int ret = 0;
	input = input_allocate_device();
	if (input == NULL) {
		ret = -ENOMEM;
		goto out;
	}
	input->name = vip_dev_name(vip);
	input->id.bustype = BUS_HOST;
	if (vip->parent) {
		input->dev.parent = vip->parent;
	}

#if	0
//	input->evbit[0] = BIT_MASK(EV_REL);
//	input->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	__set_bit(EV_REL, input->evbit);
	__set_bit(REL_X, input->relbit);
	__set_bit(REL_Y, input->relbit);
#else
	__set_bit(EV_ABS, input->evbit);
	__set_bit(ABS_X, input->relbit);
	__set_bit(ABS_Y, input->relbit);
#endif

	/* Register the input device */
	ret = input_register_device(input);
	if (ret) {
		vipm_err("input_register_device failed\n");
		input_free_device(input);
		goto out;
	}
	vip->input = input;
	input_set_drvdata(input, vip);

out:
	return ret;
}
#else
static int VIP_Unregister_Input(struct vip_dev *vip)
{
	return 0;
}

static int VIP_Register_Input(struct vip_dev *vip)
{
	return 0;
}
#endif

/*
 *
 */
int VIP_Unregister_Device(struct vip_top *vtop)
{
	if (vtop) {
		struct vip_dev *vip = vip_top_priv(vtop);
		dev_t devNo = vtop->devNo;

		VIP_Unregister_Input(vip);

#if	defined(__VIP_DEVICE_CREATE)
		device_del(vtop->dev);
#else
		OS_DestroyDeviceClass(devNo);
#endif
		cdev_del(&vtop->cdev);
	}
	return 0;
}

#if	defined(__VIP_DEVICE_CREATE)
static void VIP_Dev_Release(struct device *dev)
{
	vipm_debug_core("\n");
}
#endif

int VIP_Register_Device(struct vip_top *vtop,
						struct file_operations *fops,
						dev_t devNo)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct cdev *cdev;
#if	defined(__VIP_DEVICE_CREATE)
	struct device *dev;
#endif
	int ret;

	cdev = &vtop->cdev;
	cdev_init(cdev, fops);
	cdev->owner = THIS_MODULE;
	ret = cdev_add(cdev, devNo, 1);
	if (ret) {
		vipm_err("error (%d) while adding vip device (%d.%d)\n",
				ret, MAJOR(devNo), MINOR(devNo));
		goto out_cdev;
	}

#if	defined(__VIP_DEVICE_CREATE)
	/* register the device with sysfs */
	dev = &vtop->class_dev;
	dev->class = (struct class *)&VIP_KDRV_Class();
	dev->devt = devNo;
	if (vip->pdev != NULL) {
		dev->parent = &((struct platform_device *)vip->pdev)->dev;
	}
	dev->release = VIP_Dev_Release;
	dev_set_name(dev, "%s", vip_dev_name(vip));
	dev_set_drvdata(dev, vtop);
	ret = device_register(dev);
	if (ret) {
		vipm_err("device_register failed\n");
		goto out_cdev;
	}
	vtop->dev = dev;
#else
#ifndef OS_CreateDeviceClass
	OS_CreateDeviceClass(devNo, "%s", vip_dev_name(vip));
#endif
#endif

	vip->devt = devNo;
	vtop->devNo = devNo;

	ret = VIP_Register_Input(vip);
	if (ret) {
		goto out_device;
	}

	return 0;

out_device:
	vip->devt = 0;
	vtop->devNo = 0;

#if	defined(__VIP_DEVICE_CREATE)
	device_del(vip->dev);
	vtop->dev = NULL;
#else
	OS_DestroyDeviceClass(devNo);
#endif

out_cdev:
	cdev_del(cdev);

	return ret;
}

void VIP_Chrdev_Free(dev_t devt, int count)
{
	unregister_chrdev_region(devt, count);
}

int VIP_Chrdev_Region(dev_t *devt, int *major,
									int count, char *name)
{
	dev_t dev_num;
	int ret = 0;

	if (!devt || !major) {
		return -EINVAL;
	}

	if ((*major)) {
		dev_num = MKDEV((*major), 0);
		ret = register_chrdev_region(dev_num, count, name);
		if (ret < 0) {
			goto out;
		}
	} else {
		ret = alloc_chrdev_region(&dev_num, 0, count, name);
		if (ret < 0) {
			goto out;
		}
		(*major) = MAJOR(dev_num);
	}
	(*devt) = dev_num;

out:
	if (ret < 0) {
		vipm_err("%s failed, %d\n", __F__, ret);
		return -EIO;
	}
	return ret;

}

#if 0
void VIP_Bus_Unregister(void *bus)
{
	if (bus) {
		bus_unregister((struct bus_type *)bus);
	}
}

int VIP_Bus_Register(void *bus)
{
	if (bus) {
		return bus_register((struct bus_type *)bus);
	}
	return 0;
}
#endif

static void VIP_Del_Dev(struct vip_top *vtop)
{
	if (vtop) {
		kfree(vtop);
	}
}

static struct vip_top *VIP_Gen_Dev(int extra_size)
{
	struct vip_top *vtop = NULL;
	struct vip_dev *vip;
	unsigned int alloc_size = sizeof(struct vip_top) + sizeof(struct vip_dev) + extra_size;

	vtop = (struct vip_top *)kzalloc(alloc_size, GFP_KERNEL);
	if (vtop == NULL) {
		vipm_err("can't allocate vip device\n" );
		return NULL;
	}
//	memset(vtop, 0x0, alloc_size);

	vip = vip_top_priv(vtop);

	vip->vtop = vtop;

//	mutex_init(&vip->mutex);
	vip->lock = &vip->_lock;
	spin_lock_init(vip->lock);

	return vtop;
}

void VIP_Free_Device(struct vip_top *vtop)
{
	VIP_Del_Dev(vtop);
}

struct vip_top *VIP_Alloc_Device(int idx)
{
	struct vip_top *vtop = NULL;
//	struct vip_dev *vip = NULL;
	struct vip_drvdata *drv_data;

	vtop = VIP_Gen_Dev(sizeof(struct vip_drvdata));
	if (vtop == NULL) {
		return NULL;
	}
	drv_data = (struct vip_drvdata *)vip_dev_priv(vip_top_priv(vtop));
	drv_data->vtop = vtop;
	drv_data->is_suspended = 0;

	vtop->idx = idx;

	return vtop;
}


/*
 *
 */
void VIP_Thread_Stop(struct vip_dev *vip)
{
	if (vip->thread) {
		kthread_stop(vip->thread);
		vip->thread = NULL;
	}
}

int VIP_Thread_Init(struct vip_dev *vip, void *thread_handler)
{
	int ret = 0;

	sema_init(&vip->thread_sem, 1);
	vip->thread = kthread_run(thread_handler,
							vip, "vipthd%d",
							MINOR(vip->devt));
	if (IS_ERR(vip->thread)) {
		ret = PTR_ERR(vip->thread);
	}
	vipm_info("kthread_run[0x%p] %s, %d\n",
				thread_handler, (ret)?"failed":"done", ret);
	if (!ret) {
		vip->thread_handler = thread_handler;
	}
	return ret;
}

/*
 *
 */
int VIP_Run_Thread(void *d)
{
	struct vip_dev *vip = (struct vip_dev *)d;
	struct vip_req *areq = NULL;

	current->flags |= PF_MEMALLOC;

	down(&vip->thread_sem);
	do {
		VIP_Core_Reset_Check(vip);

		spin_lock_irq(vip->lock);
		set_current_state(TASK_INTERRUPTIBLE);
		areq = VIP_Chk_Queue_Run(vip);
		spin_unlock_irq(vip->lock);

		if (areq) {
			set_current_state(TASK_RUNNING);

			vipm_debug_core("Run request\n");

			VIP_Run_Request(areq);

			areq = NULL;
		} else {
			if (kthread_should_stop()) {
				vipm_debug_core("Stopping thread\n");
				set_current_state(TASK_RUNNING);
				break;
			}
			up(&vip->thread_sem);
			vipm_debug_core("Schedule in thread\n");
			schedule();
			down(&vip->thread_sem);
		}
	} while (1);
	up(&vip->thread_sem);

	return 0;
}

void VIP_Wakeup_Thread(struct vip_dev *vip)
{
	wake_up_process(vip->thread);
	vipm_debug_core("Wake up thread[0x%p]\n", vip->thread);
}

/*
 *
 */
int VIP_IRQ_Status(struct vip_dev *vip)
{
	return VIP_HW_Top_Readl(vip, TOP_INTR_CPU_ST);
}

void VIP_IRQ_Clear(struct vip_dev *vip, unsigned int intmask)
{
	VIP_HW_Top_Writel(vip, intmask, TOP_INTR_CPU_CLR);
}

int VIP_IRQ_For_Request(struct vip_req *areq)
{
	int ret = 0;

	if ((areq == NULL) ||
		!VIP_Is_Inited(areq->vip) ||
		!VIP_Is_HW_Inited(areq->vip)) {
		goto out;
	}

	VIP_Finish_Request(areq);

out:
	return ret;
}

irqreturn_t VIP_IRQ_Handler(int irq, void *dev_id)
{
	struct vip_dev *vip = dev_id;
	struct vip_req *areq = vip->areq;
	unsigned int intstatus = 0;
	unsigned int intid = 0;
	unsigned int intmask = 0;
	irqreturn_t result = IRQ_HANDLED;
//	int ret;

//	spin_lock(vip->lock);

	intstatus = VIP_IRQ_Status(vip);
	intid = intstatus>>VIP_INTR_CPU_ST_ID_OFFSET;
	intmask = intstatus&VIP_INTR_CPU_ST_MASK;
	vipm_debug_core("Intr. mask = 0x%08x\n", intmask);
	if (!intmask) {
		result = IRQ_NONE;
		goto out;
	}

	if (vip_test_bit(VIP_FLAG_INIT_D, &vip->flags)) {
		vip_clear_bit(VIP_FLAG_INIT_D, &vip->flags);
	} else {
		VIP_IRQ_For_Request(areq);
	}

	VIP_IRQ_Clear(vip, intstatus);
	intstatus = VIP_IRQ_Status(vip);
	if (intstatus & VIP_INTR_CPU_ST_MASK) {
		vipm_debug_core("Intr status isn't cleared : 0x%08x\n", intstatus);
	}

out:
//	spin_unlock(vip->lock);

	return result;
}

int VIP_Core_FW_Cleanup(struct vip_dev *vip)
{
	if (vip->firm_ctrl.addr) {
		kfree((void *)(vip->firm_ctrl.addr));
		vip->firm_ctrl.addr = 0;
	}
	return 0;
}

int VIP_Core_FW_Init(struct vip_dev *vip, void *addr, unsigned int size)
{
	VIP_Core_FW_Cleanup(vip);
	vip->firm_ctrl.addr = (unsigned long)addr;
	vip->firm_ctrl.size = VIP_FIRM_IOREMAP_SIZE(size);
	vip->firm_ctrl.magic = LX_VIP_IMG_MAGIC;
	vipm_debug_core("vip->firm_ctrl.addr = 0x%08x\n", (unsigned int)vip->firm_ctrl.addr);
	vipm_debug_core("               addr = 0x%08x\n", vip->firm_ctrl.size);
	return 0;
}

void VIP_Core_Cleanup(struct vip_dev *vip)
{
	if (vip) {
		if (vip_io_lock()) {
			return;
		}

		if (VIP_Is_Inited(vip)) {
			VIP_Clr_Inited(vip);

			VIP_Clk_Get(vip);

			VIP_Abort_And_Free_Queue_All(vip);

			VIP_Remove_Debugfs(vip);

			VIP_HW_Exit(vip);

			VIP_Release_IRQ(vip);

			VIP_Thread_Stop(vip);

			VIP_Mem_Destroy_Cache(vip);

			VIP_Core_FW_Cleanup(vip);

			VIP_Clk_Put(vip);
		}

		vip_io_unlock();
	}
}

int VIP_Core_Init(struct vip_dev *vip,
						struct vip_init_link *link)
{
	int ret = 0;

	ret = vip_io_lock();
	if (ret) {
		return ret;
	}

	if (VIP_Is_Inited(vip)) {
		ret = VIP_Clk_Get(vip);
		vip_io_unlock();
		return ret;
	}

	vipm_debug_core("\n");

	if (link == NULL) {
		vipm_err("NULL link\n");
		ret = -EINVAL;
		goto out;
	}
	if (!link->max_func) {
		vipm_err("Zero : max_func\n");
		ret = -EINVAL;
		goto out;
	}
	if ((link->get_hal_ops == NULL) || (link->get_mem_ops == NULL)) {
		vipm_err("NULL : get_hal_ops[0x%p] get_mem_ops[0x%p]\n",
				link->get_hal_ops, link->get_mem_ops);
		ret = -EINVAL;
		goto out;
	}
	if ((link->thread_handler == NULL) || (link->irq_handler == NULL)) {
		vipm_err("NULL : thread_handler[0x%p] irq_handler[0x%p]\n",
				link->thread_handler, link->irq_handler);
		ret = -EINVAL;
		goto out;
	}

	vip->vinit_link = link;

	vip->buf_max = link->buf_max;
	vip->buf_align = link->buf_align;

	vip->max_func = link->max_func;

	(link->get_hal_ops)(vip);
	(link->get_mem_ops)(vip);
	if ((vip->vhal_ops == NULL) || (vip->vmem_ops == NULL)) {
		vipm_err("NULL : vhal_ops[0x%p] vmem_ops[0x%p]\n",
				vip->vhal_ops, vip->vmem_ops);
		ret = -EINVAL;
		goto out;
	}

	vip->ipc_imgb = (unsigned char *)vip->vhal_ops->cfg.ipc_imgb;
	vip->vint_buf.imgb = (unsigned char *)vip->vhal_ops->cfg.imgb;

	ret = VIP_Clk_Get(vip);
	if (ret) {
		goto out;
	}

	VIP_Init_List(vip);
	VIP_Init_Queue(vip);

	vipm_info("vip dsp name = %s\n",
			(vip->vhal_ops)? prt_string(vip->vhal_ops->name) : "unknonwn");

	ret = VIP_Mem_Create_Cache(vip);
	if (ret) {
		goto out_clk;
	}

	init_waitqueue_head(&vip->areq_wait);

	ret = VIP_Thread_Init(vip, link->thread_handler);
	if (ret) {
		goto out_cache;
	}

	ret = _VIP_Request_IRQ(vip, link->irq_handler);
	if (ret) {
		goto out_thread;
	}

#if !defined(VIP_FIRM_DOWN_FROM_KADAP)
	ret = VIP_HW_Init(vip);
	if (ret) {
		goto out_irq;
	}
#endif

	ret = VIP_Add_Debugfs(vip);
	if (ret) {
		goto out_hw;
	}

	VIP_Set_Inited(vip);

	vipm_debug_core("vip         : 0x%p\n", vip);
	vipm_debug_core(" name       : %s\n", prt_string(vip_dev_name(vip)));
	vipm_debug_core(" parent     : 0x%p\n", vip->vtop->parent);
	vipm_debug_core(" clk        : 0x%p\n", vip->clk);
	vipm_debug_core(" vhal_ops   : 0x%p\n", vip->vhal_ops);
	vipm_debug_core(" regb       : 0x%p\n", vip->regb);
	vipm_debug_core(" imgb       : 0x%p\n", vip->vint_buf.imgb);
	vipm_debug_core(" devt       : %d\n", vip->devt);
	vipm_debug_core(" irq        : %d\n", vip->irq);

	vip_io_unlock();
	return 0;

out_hw:
	VIP_HW_Exit(vip);

#if !defined(VIP_FIRM_DOWN_FROM_KADAP)
out_irq:
	VIP_Release_IRQ(vip);
#endif

out_thread:
	VIP_Thread_Stop(vip);

out_cache:
	VIP_Mem_Destroy_Cache(vip);

out_clk:
	VIP_Clk_Put(vip);

out:
	vip->vhal_ops = NULL;
	vip->vmem_ops = NULL;

	vip_io_unlock();
	return ret;
}

/*
 *
 */
int VIP_Set_Completion(struct vip_req *areq, int fault)
{
	int ret = 0;

	if (VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_RUN) || fault) {
		complete(&areq->completion);
		areq->fault = fault;
		if (fault) {
		//	struct vip_dev *vip = areq->vip;
			vipm_debug_core("abnormal completion : %s\n",
						prt_names(fault, _vip_fault_name));
			/*
			if (vip_test_bit(VIP_FLAG_FAULT_RST, &vip->flags)) {
				VIP_HW_Reset(areq->vip);
			}
			*/
		}
	}

	return ret;
}


int VIP_Done_Completion(struct vip_req *areq, int fault)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(areq->lock, flags);	//called in interrupt
	ret = VIP_Set_Completion(areq, fault);
	spin_unlock_irqrestore(areq->lock, flags);
	return ret;
}

void VIP_Finish_Request(struct vip_req *areq)
{
	VIP_Set_Completion(areq, VIP_FAULT_COMPLETION_NONE);
	wake_up_interruptible(&areq->vip->areq_wait);
}

static int VIP_Wait_Polling(struct vip_req *areq)
{
	struct vip_dev *vip;
//	ktime_t start_t, end_t;
	unsigned long time_count;
	unsigned int time_limited;
	unsigned int interval_us;
	int fault = VIP_FAULT_COMPLETION_NONE;
	int ret = 0;

	vip = areq->vip;
	if (!VIP_Is_Run(vip)) {
		areq->error = -EFAULT;
		goto out;
	}

	vipm_debug_core("Polling start...\n");

	interval_us = vip->poll_interval;
	interval_us = (interval_us) ? interval_us : VIP_POLLING_INTERVAL;
	vipm_debug_core("interval_us : %d\n", interval_us);

	time_limited = !!(areq->timeout);
	time_count = (areq->timeout/interval_us)? areq->timeout : VIP_RUN_WAIT_TIMEOUT;
	time_count /= interval_us;

//	start_t = ktime_get();
	ret = -EBUSY;
	while (1) {
		ret = VIP_HW_Poll(areq);
		if (ret != -EBUSY) {
			break;
		}
		VIP_Delay_US(interval_us);

		if (time_limited) {
			if (!time_count) {
				fault = VIP_FAULT_COMPLETION_WAITING;
				ret = -ETIMEDOUT;
				break;
			}
			time_count--;
		}
	}
//	end_t = ktime_get();
//	__vipm_debug_time(VIPM_DBG_SET_CORE, start_t, end_t);

	fault = (!fault && (ret < 0))? VIP_FAULT_COMPLETION_POLLING : fault;
	VIP_Done_Completion(areq, fault);

out:
	return ret;
}

static int VIP_Wait_Event(struct vip_req *areq)
{
	struct vip_dev *vip;
//	ktime_t start_t, end_t;
	unsigned long timeout;
	long result, ret;

	vip = areq->vip;
	if (!VIP_Is_Run(vip)) {
		ret = -EFAULT;
		goto out;
	}

	vipm_debug_core("Waiting interrupt...\n");

	timeout = (areq->timeout)? msecs_to_jiffies(areq->timeout) :
							MAX_SCHEDULE_TIMEOUT;
	vipm_debug_core("timeout = 0x%lx\n", timeout);

//	start_t = ktime_get();
#if	1
	result = wait_event_interruptible_timeout(vip->areq_wait,
									completion_done(&areq->completion),
									timeout);
#else
	result = wait_for_completion_timeout(areq->completion, timeout);
#endif
//	end_t = ktime_get();
//	__vipm_debug_time(VIPM_DBG_SET_CORE, start_t, end_t);

	ret = (!result)? -ETIMEDOUT : (int)result;
	if (ret < 0) {
		VIP_Done_Completion(areq, VIP_FAULT_COMPLETION_WAITING);
	}

out:
	return ret;
}

static int VIP_Wait_Completion(struct vip_req *areq)
{
	int ret;

	ret = (areq->vip->irq)? VIP_Wait_Event(areq) :	\
							VIP_Wait_Polling(areq);

	if (ret < 0) {
		if (ret == -ETIMEDOUT) {
			vipm_err("%s : Time out\n", __F__);
		} else {
			vipm_err("%s : Completion failed\n", __F__);
		}
		return ret;
	}

	//Positive value of ret doesn't mean error
	return 0;
}

void VIP_Reset_Gesture_Data(struct vip_msg_gesture *gesture)
{
	if (gesture != NULL) {
		gesture->posx = -1;
		gesture->posy = -1;
		gesture->handposx = -1;
		gesture->handposy = -1;
		gesture->osd_button_num = -1;
		gesture->click = 0;
		gesture->event_result = 0;
		gesture->gesture_alert = 0;
	}
}

void VIP_Reset_Crop_Data(struct vip_msg_crop *crop)
{
	if (crop != NULL) {
		crop->enable = 0;
		crop->offsetx = 0;
		crop->offsety = 0;
		crop->width = 0;
		crop->height = 0;
	}
}

void VIP_Run_Error(struct vip_dev *vip,
						int func,
						int index,
						int result)
{
	struct vip_run_err *err = &vip->vrun_err;
	err->func = func;
	err->index = index;
	err->result = result;
}

void VIP_Run_Err_Rst(struct vip_dev *vip)
{
	memset(&vip->vrun_err, 0, sizeof(vip->vrun_err));
}

static void VIP_Run_Init(struct vip_req *areq)
{
	struct vip_dev *vip = areq->vip;

	spin_lock(vip->lock);
	VIP_Run_Err_Rst(vip);
	VIP_Set_Run(vip);

	vip->cur_func = areq->func;
	vip->cur_index = areq->index;

	areq->vip->areq = areq;
	spin_unlock(vip->lock);
}

static int VIP_Run_Done(struct vip_req *areq, int result)
{
	struct vip_dev *vip = areq->vip;

	spin_lock(vip->lock);

	VIP_Dec_Fmt_Usage(areq->ifmt_list);
	VIP_Dec_Buf_Usage(areq->ibuf);
	VIP_Dec_Buf_Usage(areq->obuf);

//	areq->post = NULL;

	areq->error = result;
	VIP_Run_Error(vip, areq->func, areq->index, result);
	VIP_Add_Queue_Done(areq);

	VIP_Clr_Run(vip);

	vip->areq = NULL;

	spin_unlock(vip->lock);
	return 0;
}

int VIP_Run_Request(struct vip_req *areq)
{
	int ret;

	areq->run_t = ktime_get();

	vipm_debug_core("f[%d] i[%d] : Run Start\n", areq->func, areq->index);
//	vipm_err("f[%d] i[%d] : Run Start\n", areq->func, areq->index);

	VIP_Run_Init(areq);

	ret = VIP_HW_Run(areq);
	if (ret) {
		VIP_Done_Completion(areq, VIP_FAULT_COMPLETION_RUN);
		goto out;
	}

	ret = VIP_Wait_Completion(areq);
	if (ret < 0) {	/* No 'if (ret)' */
		goto out;
	}

#if (__VIP_POST_AT_STATE == 0)
	if (areq->post) {
		ret = areq->post(areq);
	}
#endif

	vipm_debug_core("f[%d] i[%d]] : Run Done\n", areq->func, areq->index);
//	vipm_err("f[%d] i[%d] : Run Done\n", areq->func, areq->index);

out:
	VIP_Run_Done(areq, ret);
	areq->end_t = ktime_get();
	areq->elapsed = (unsigned int)ktime_to_us(ktime_sub(areq->end_t, areq->add_t));
	{
		ktime_t _s_t, _e_t;
		_e_t = areq->end_t;
	#if 1
		_s_t = areq->add_t;
		__vipm_debug_time(VIPM_DBG_SET_CORE, _s_t, _e_t);
	//	_s_t = areq->run_t;
	//	__vipm_debug_time(VIPM_DBG_SET_CORE, _s_t, _e_t);
	#else
		_s_t = areq->add_t;
		__vipm_debug_time_noti(_s_t, _e_t);
	//	_s_t = areq->run_t;
	//	__vipm_debug_time_noti(_s_t, _e_t);
	#endif
	}
	return ret;
}

void VIP_Add_Request(struct vip_req *areq,
					unsigned long timeout,
					int (*post)(struct vip_req *),
					int from_user)
{
	vipm_debug_core("\n");

	init_completion(&areq->completion);

	areq->from_user = from_user;
	areq->post = post;
	areq->odata = NULL;
	areq->osize = 0;
	areq->onum = 0;

	//__VIP_GESTURE_ON
	VIP_Reset_Gesture_Data(&areq->gesture);
	VIP_Reset_Crop_Data(&areq->crop);
	areq->set_mode = areq->vip->set_mode;

	areq->fault = 0;
	areq->error = 0;
	areq->timeout = timeout;
	areq->aflags = 0;

	areq->end_t = ktime_get();
	areq->add_t = areq->end_t;
	areq->run_t = areq->end_t;
	areq->elapsed = 0;

	VIP_Inc_Fmt_Usage(areq->ifmt_list);
	VIP_Inc_Buf_Usage(areq->ibuf);
	VIP_Inc_Buf_Usage(areq->obuf);

	VIP_Add_Queue_Run(areq);

	vipm_debug_core("vip_add_request for areq[0x%p]\n", areq);
}

/*
 *
 */
int VIP_Chk_Fmt(struct vip_io_ifmt *format)
{
	if (!format->width || (format->width > VIP_MAX_WIDTH)) {
		vipm_err("Out of range : width[%d]\n", format->width);
		return -EINVAL;
	}
	if (!format->height || (format->height > VIP_MAX_HEIGHT)) {
		vipm_err("Out of range : height[%d]\n", format->height);
		return -EINVAL;
	}

	vipm_debug_core("format->width  : %d\n", format->width);
	vipm_debug_core("format->height : %d\n", format->height);
	vipm_debug_core("format->stride : %d\n", format->stride);
	vipm_debug_core("format->format : %d\n", format->format);
	return 0;
}

struct vip_req *VIP_Get_Request(struct vip_dev *vip,
										int func, int index,
										int *result)
{
	struct vip_req *areq = NULL;
	int ret = 0;

	if (func >= vip->max_func) {
		vipm_err("Invalid func : func[%d] > max[%d]\n",
				func, vip->max_func);
		ret = -EINVAL;
		goto out;
	}

	if (VIP_Find_Queue_Run(vip, func, index)) {
		ret = -EBUSY;
		goto out;
	}

	areq = VIP_Find_Queue(vip, func, index);
	if (areq == NULL) {
		areq = VIP_Gen_Queue(vip, func, index);
		if (areq == NULL) {
			ret = -ENOMEM;
			goto out;
		}
		VIP_Add_Queue(areq);
	}

	ret = VIP_Get_Request_Sub_Fmt(areq, func, index);
	if (ret) {
		goto out;
	}

	ret = VIP_Get_Request_Sub_Buf(areq, func, index, VIP_BUF_INPUT);
	if (ret) {
		goto out;
	}

	ret = VIP_Get_Request_Sub_Buf(areq, func, index, VIP_BUF_OUTPUT);
	if (ret) {
		goto out;
	}

	if (result) {
		(*result) = 0;
	}
	return areq;

out:
	if (result) {
		(*result) = ret;
	}
	return NULL;
}

int VIP_Get_Obj_Info(struct vip_req *areq)
{
	return VIP_HW_Msg(areq);
}



