/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
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

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
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

#include "../sys/sys_regs.h"	//for CTOP CTRL Reg. map

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "logm_kapi.h"

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_usb.h"
#include "vip_queue.h"
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

int _vip_debug = 0;

/*
 *
 */
int g_vip_debug_fd = -1;

static int g_vip_top_inited = 0;
static int g_vip_top_probed = 0;
static int g_vip_max_dev = LX_VIP_DEV_MAX;

static int g_vip_major = VIP_DEV_MAJOR;

static void __iomem *g_vip_ctop_base = NULL;

#if defined(__VIP_DEVICE_CREATE)
module_param_named(vip_major, g_vip_major, int, 0644);
MODULE_PARM_DESC(vip_major, "VIP major");
module_param_named(vip_debug, _vip_debug, int, 0644);
MODULE_PARM_DESC(vip_debug, "VIP debug level");
#endif

static struct vip_top *_g_vip_top[LX_VIP_DEV_MAX] = { NULL, };

/*
 * VIP Reg Map configuration.
 */
#ifdef USE_KDRV_CODES_FOR_H15
#include "./h15/vip_reg_h15_a0.h"
#endif

#ifdef USE_KDRV_CODES_FOR_H15
/* Core regsiter area */
#define	H15_VIP_REG_BASE		VIP_REG_CORE_BASE
#define	H15_VIP_REG_SIZE		VIP_REG_CORE_SIZE
#endif	/* USE_KDRV_CODES_FOR_H15 */

struct vip_reg_cfg g_vip_reg_cfg[VIP_CHIP_MAX][LX_VIP_DEV_MAX] =
{
#ifdef USE_KDRV_CODES_FOR_H15
	[VIP_CHIP_H15_A0] = {
		[LX_VIP_DEV_IMG] = {	/* LX_VIP_DEV_IMG */
			.chip_name		= "VIP H15 A0",
			.reg_base		= H15_VIP_REG_BASE,
			.reg_size		= H15_VIP_REG_SIZE,
		#if (__VIP_USE_INTERRUPT > 0)
			.irq_num		= H15_IRQ_VIP,
		#else
			.irq_num		= 0
		#endif
		},
	}
#endif
};
static struct vip_reg_cfg *_g_vip_reg_cfg_grp = NULL;

/*
 *
 */
static void *VIP_Drv_Get_FOPS(int index)
{
	void *fops = NULL;

	switch(index) {
		case LX_VIP_DEV_IMG :
			fops = VIP_IMG_Get_FOPS();
			break;
		default :
			break;
	}
	return fops;
}

void VIP_Drv_Set_Top(void *vtop_priv, unsigned int idx)
{
	if (idx < g_vip_max_dev) {
		_g_vip_top[idx] = (struct vip_top *)vtop_priv;
		vipm_debug_drv("Set : _g_vip_top[%d] = 0x%p\n", idx, vtop_priv);
	}
}

void VIP_Drv_Del_Top(void *vtop_priv)
{
	int i;

	for (i=0 ; i<g_vip_max_dev ; i++) {
		if (_g_vip_top[i] == (struct vip_top *)vtop_priv) {
			vipm_debug_drv("Del : _g_vip_top[%d] = 0x%p\n", i, vtop_priv);
			_g_vip_top[i] = NULL;
			return;
		}
	}
}

void *VIP_Drv_Get_Top(int idx)
{
	if (idx < g_vip_max_dev) {
		vipm_debug_drv("Get : _g_vip_top[%d] = 0x%p\n", idx, _g_vip_top[idx]);
		return (void *)_g_vip_top[idx];
	}
	return NULL;
}

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*
 *
 */
#if defined(__VIP_DEVICE_CREATE)
static ssize_t show_debug(struct device *cd,
			 struct device_attribute *attr, char *buf)
{
//	struct vip_top *vtop = to_vip_top_dev(cd);
//	struct vip_top *vtop = dev_get_drvdata(cd);
//	struct vip_dev *vip = vip_top_priv(vtop);

	return sprintf(buf, "%i\n", _vip_debug);
}

static ssize_t set_debug(struct device *cd, struct device_attribute *attr,
		   const char *buf, size_t len)
{
//	struct vip_top *vtop = to_vip_top_dev(cd);
	struct vip_top *vtop = dev_get_drvdata(cd);
	struct vip_dev *vip = vip_top_priv(vtop);
	int res = 0;
	u16 value;

	res = kstrtou16(buf, 0, &value);
	if (res)
		return res;

	spin_lock(vip->lock);
	_vip_debug = value;
	spin_unlock(vip->lock);
	return len;
}

static ssize_t show_name(struct device *cd,
			 struct device_attribute *attr, char *buf)
{
//	struct vip_top *vtop = to_vip_top_dev(cd);
	struct vip_top *vtop = dev_get_drvdata(cd);
	struct vip_dev *vip = vip_top_priv(vtop);

	return sprintf(buf, "%.*s\n", (int)sizeof(vip_dev_name(vip)), vip->name);
}

const struct device_attribute vip_device_attrs[] = {
	__ATTR(name, S_IRUGO, show_name, NULL),
	__ATTR(debug, 0644, show_debug, set_debug),
	__ATTR_NULL
};

static void VIP_Class_Release(struct device *dev)
{
	vipm_debug_drv("\n");
}

static char *kdrv_vip_node_name( struct device* dev, umode_t* mode)
{
	return kasprintf(GFP_KERNEL, "lg/%s", dev_name(dev));
}

static const struct class kdrv_vip_class = {
	.name			= "lg",
	.devnode 		= kdrv_vip_node_name,
	.dev_attrs		= (struct device_attribute *)vip_device_attrs,
	.dev_release	= VIP_Class_Release,
};

void *VIP_KDRV_Class(void)
{
	return (void *)&kdrv_vip_class;
}

#endif	/* __VIP_DEVICE_CREATE */

/*========================================================================================
	Implementation Group
========================================================================================*/
#if defined(VIP_KDRV_CONFIG_PM)
/*
 *	module platform driver structure
 */
static const struct platform_device_id vip_platform_ids[] = {
	{ VIP_MODULE, 0 },
	{ }
};
MODULE_DEVICE_TABLE(platform, vip_platform_ids);

static struct platform_device *_g_plt_device[LX_VIP_DEV_MAX] = { NULL, };

static void VIP_Drv_Set_Plt(void *plt_priv, unsigned int idx)
{
	if (idx < g_vip_max_dev) {
		_g_plt_device[idx] = (struct platform_device *)plt_priv;
		vipm_debug_drv("Set : _g_plt_device[%d] = 0x%p\n", idx, plt_priv);
		return;
	}
	vipm_err("Index err : %d\n", idx);
}

static void VIP_Drv_Del_Plt(void *plt_priv)
{
	int i;

	for (i=0 ; i<g_vip_max_dev ; i++) {
		if (_g_plt_device[i] == (struct platform_device *)plt_priv) {
			vipm_debug_drv("Del : _g_plt_device[%d] = 0x%p\n", i, plt_priv);
			_g_plt_device[i] = NULL;
			return;
		}
	}
}

static void *VIP_Drv_Get_Plt(int idx)
{
	if (idx < g_vip_max_dev) {
		vipm_debug_drv("Get : _g_plt_device[%d] = 0x%p\n", idx, _g_plt_device[idx]);
		return (void *)_g_plt_device[idx];
	}
	return NULL;
}

static int VIP_Drv_Get_Plt_Index(void *plt_priv)
{
	int i;

	for (i=0 ; i<g_vip_max_dev ; i++) {
		if (_g_plt_device[i] == (struct platform_device *)plt_priv) {
			vipm_debug_drv("Get : _g_plt_device[%d] = 0x%p\n", i, plt_priv);
			return i;
		}
	}
#if 1
	vipm_debug_drv("Err : plt_priv = 0x%p\n", plt_priv);
	for (i=0 ; i<g_vip_max_dev ; i++) {
		vipm_debug_drv("    : _g_plt_device[%d] = 0x%p\n", i, _g_plt_device[i]);
	}
#endif
	return -EINVAL;
}

static int VIP_Plt_Device_Register(int idx)
{
	struct platform_device *plt_device = NULL;
	int ret = 0;

	if (idx == -1) {
		int i;
		for (i=0 ; i<g_vip_max_dev ; i++) {
			if (_g_plt_device[i] == NULL) {
				idx = i;
				break;
			}
		}
		if (idx == -1) {
			vipm_err("Full platform device\n");
			ret = -ENOMEM;
			goto out;
		}
	}

	plt_device = platform_device_alloc(VIP_MODULE, idx);
	if (plt_device == NULL) {
		vipm_err("NULL platform device[%d]\n", idx);
		ret = -ENOMEM;
		goto out;
	}
	vipm_debug_drv("plt_device = %p\n", plt_device);
	VIP_Drv_Set_Plt(plt_device, idx);

	ret = platform_device_add(plt_device);
	if (ret) {
		platform_device_put(plt_device);
		VIP_Drv_Del_Plt(plt_device);
		vipm_err("Unable to add platform device for VIP\n");
		goto out;
	}

out:
	return ret;
}

static int VIP_Plt_Device_Unregister(int idx)
{
	struct platform_device *plt_device = VIP_Drv_Get_Plt(idx);
//	char *plt_name;

	if (plt_device) {
		platform_device_unregister(plt_device);

		VIP_Drv_Del_Plt(plt_device);
	}
	return 0;
}

static int VIP_SuspendTask(struct vip_dev *vip)
{
	/* Abort all run queue */
	VIP_Abort_Queue_Run(vip, -1, 0);

	VIP_Clr_HW_Inited(vip);

	return 0;
}

static int VIP_ResumeTask(struct vip_dev *vip)
{
	int ret;

	ret = VIP_Core_HW_Init(vip);
	if (ret) {
		goto out;
	}

out:
	return ret;
}

#define VIPM_TEST_LOG_ON

#if defined(VIPM_TEST_LOG_ON)
#define vipm_pm_test_log(fmt, args...)		printk(fmt, ##args)
#else
#define vipm_pm_test_log(fmt, args...)		do{	}while(0)
#endif

static int VIP_Suspend(struct vip_dev *vip)
{
	int ret = 0;
#ifdef KDRV_CONFIG_PM_VIP
	struct vip_drvdata *drv_data = vip_dev_priv(vip);

	vipm_pm_test_log("VIP - suspend BEGIN\n");

	if (!VIP_Is_Registered(vip)) {
		vipm_warn("VIP not ready\n");
		goto out;
	}
	if (!VIP_Is_Opened(vip)) {
		vipm_warn("VIP not opened\n");
		goto out;
	}

	if (drv_data->is_suspended) {
		vipm_warn("VIP already suspended\n");
		goto out;
	}

	if (VIP_Is_Busy(vip)) {
		vipm_err("VIP Busy...");
		ret = -EBUSY;
		goto out;
	}

	VIP_SuspendTask(vip);

	ret = VIP_Slumber(vip, 1);
	if (ret) {
		vipm_err("slumber err\n");
		goto out;
	}

	drv_data->is_suspended = 1;

out:
	vipm_pm_test_log("VIP - suspend %s\n", (ret)? "FAIL" : "OK");
	if (!ret) {
	#if !defined(VIPM_TEST_LOG_ON)
		vipm_info("VIP suspended\n");
	#endif
	}
#endif

	return ret;
}

static int VIP_Resume(struct vip_dev *vip)
{
	int ret = 0;
#ifdef KDRV_CONFIG_PM_VIP
	struct vip_drvdata *drv_data = vip_dev_priv(vip);

	vipm_pm_test_log("VIP - resume BEGIN\n");

	if (!VIP_Is_Registered(vip)) {
		vipm_warn("VIP not ready\n");
		goto out;
	}
	if (!VIP_Is_Opened(vip)) {
		vipm_warn("VIP not opened\n");
		goto out;
	}

	if (!drv_data->is_suspended) {
		vipm_warn("VIP already resumed\n");
		goto out;
	}

	ret = VIP_Wakeup(vip, 1);
	if (ret) {
		vipm_err("wakeup err\n");
		goto out;
	}

	VIP_ResumeTask(vip);

	drv_data->is_suspended = 0;

out:
	vipm_pm_test_log("VIP - resume %s\n", (ret)? "FAIL" : "OK");
	if (!ret) {
	#if !defined(VIPM_TEST_LOG_ON)
		vipm_info("VIP resumed\n");
	#endif
	}
#endif

	return ret;
}

/**
 *
 * suspending module.
 *
 * @param	struct platform_device *pdev pm_message_t state
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int VIP_Plt_Suspend(struct platform_device *pdev, pm_message_t state)
{
	struct vip_top *vtop = platform_get_drvdata(pdev);
	struct vip_dev *vip = vip_top_priv(vtop);
//	int idx;
	int ret;

	// run to suspend mode
	ret = VIP_Suspend(vip);
	if (!ret) {
		/* Update Internal Status Value */
	//	VIP_HAL_DRV_InitDone_Flag(0);
	}
	vipm_debug_drv("suspend done\n");
	return ret;
}

/**
 *
 * resuming module.
 *
 * @param	struct platform_device *
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int VIP_Plt_Resume(struct platform_device *pdev)
{
	struct vip_top *vtop = platform_get_drvdata(pdev);
	struct vip_dev *vip = vip_top_priv(vtop);
//	int idx;
	int ret;

	// run to resume mode
	ret = VIP_Resume(vip);
	if (!ret) {
		/* Update Internal Status Value */
	//	VIP_HAL_DRV_InitDone_Flag(1);
	}

	vipm_debug_drv("resume done\n");
	return ret;
}

/**
 *
 * probing module.
 *
 * @param	struct platform_device *pdev
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int VIP_Plt_Probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct vip_top *vtop;
	int idx;

	if (!dev->dma_mask) {
		dev->dma_mask = &dev->coherent_dma_mask;
		dev->coherent_dma_mask = DMA_BIT_MASK(32);
	}

	idx = VIP_Drv_Get_Plt_Index(pdev);
	if (idx < 0) {
		vipm_err("No platofrm device\n");
		return -ENODEV;
	}

	vtop = (struct vip_top *)VIP_Drv_Get_Top(idx);
	if (vtop == NULL) {
		vipm_err("No vip device\n");
		return -ENODEV;
	}
	vtop->parent = &pdev->dev;
	vtop->pdev = pdev;
	platform_set_drvdata(pdev, vtop);

	vipm_debug_drv("probe done\n");

	return 0;
}


/**
 *
 * module remove function. this function will be called in rmmod fbdev module.
 *
 * @param	struct platform_device
 * @return	int 0 : OK , -1 : NOT OK
 *
 */
static int VIP_Plt_Remove(struct platform_device *pdev)
{
	platform_set_drvdata(pdev, NULL);

	vipm_debug_drv("remove done\n");

	return 0;
}

static struct platform_driver vip_platform_driver =
{
	.id_table	= vip_platform_ids,
	.probe		= VIP_Plt_Probe,
	.remove		= VIP_Plt_Remove,
	.suspend	= VIP_Plt_Suspend,
	.resume		= VIP_Plt_Resume,
	.driver = {
		.name   = VIP_MODULE,
	},
};

static int VIP_Plt_Driver_Register(void)
{
	int ret;

	ret = platform_driver_register(&vip_platform_driver);
	if (ret) {
		vipm_err("platform driver register failed\n");
		goto out;
	}
	vipm_debug_drv("platform driver register done\n");

out:
	return ret;
}

static void VIP_Plt_Driver_Unregister(void)
{
	platform_driver_unregister(&vip_platform_driver);
}


#else	/* VIP_KDRV_CONFIG_PM */
static void *VIP_Drv_Get_Plt(int idx)
{
	return NULL;
}

static int VIP_Plt_Device_Noop(void)
{
	return 0;
}
#define VIP_Plt_Driver_Register()		VIP_Plt_Device_Noop()
#define VIP_Plt_Driver_Unregister()		VIP_Plt_Device_Noop()
#define	VIP_Plt_Device_Register(d)		VIP_Plt_Device_Noop()
#define	VIP_Plt_Device_Unregister(d)	VIP_Plt_Device_Noop()
#endif	/* VIP_KDRV_CONFIG_PM */

static int VIP_Drv_Config(void)
{
	//Initialize Audio Decoder(DSP) register base
	/* Check LX Chip Revision Number */
	if(lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		vipm_debug_drv("LX_CHIP_REV(H15, A0)(0x%X)\n", lx_chip_rev());

		_g_vip_reg_cfg_grp = g_vip_reg_cfg[VIP_CHIP_H15_A0];
	}
	else
	{
		vipm_err("LX_CHIP_REV => Unknown(0x%X) : ERROR\n", lx_chip_rev());
		return -EIO;
	}
	return 0;
}

static void VIP_Drv_Clr_Device(struct vip_top *vtop)
{

}

static void VIP_Drv_Set_Device(struct vip_top *vtop)
{
	struct vip_dev *vip;
	int idx = vtop->idx;

	vip = vip_top_priv(vtop);

	vip->reg_cfg = (struct vip_reg_cfg *)&_g_vip_reg_cfg_grp[idx];
	vip->regb = NULL;
	vip->ipcb = NULL;
	vip->firm_addr = NULL;
	vip->lndb_addr = NULL;
	vip->ipc_imgb = NULL;

	sprintf(vip->name, "%s%d", VIP_MODULE, idx);
	vip->irq = vip->reg_cfg->irq_num;
	vip->irq_handler = NULL;

	vip->thread_handler = NULL;

	vip->poll_interval = 0;

//	vip_set_bit(VIP_FLAG_DBG_ON, &vip->flags);
}

static int VIP_Drv_Device_Free(struct vip_top *vtop)
{
	if (vtop) {
	//	int idx = vtop->idx;

		struct vip_dev *vip = vip_top_priv(vtop);

		VIP_HAL_Free_Device(vip);

		VIP_Free_Device(vtop);

		VIP_Drv_Del_Top(vtop);
	}
	return 0;
}

static struct vip_top *VIP_Drv_Device_Alloc(int idx)
{
	struct vip_top *vtop = NULL;
	int ret = 0;

	vtop = VIP_Alloc_Device(idx);
	if (vtop == NULL) {
		vipm_err("VIP_Alloc_Device failed\n" );
		goto out;
	}
	VIP_Drv_Set_Device(vtop);

	ret = VIP_HAL_Alloc_Device(vip_top_priv(vtop));
	if (ret) {
		VIP_Free_Device(vtop);
		vtop = NULL;
		goto out;
	}

	VIP_Drv_Set_Top(vtop, idx);

out:
	return vtop;
}

static int VIP_Drv_Device_Remove(struct vip_top *vtop)
{
	if (vtop != NULL) {
		struct vip_dev *vip = vip_top_priv(vtop);
		int idx = vtop->idx;

		VIP_Clr_Registered(vip);

		VIP_PROC_Cleanup(vip_dev_name(vip), MINOR(vtop->devNo));

		VIP_Core_Cleanup(vip);

		VIP_Unregister_Device(vtop);

		VIP_Drv_Clr_Device(vtop);

		VIP_Plt_Device_Unregister(idx);
	}

	return 0;
}

static int VIP_Drv_Device_Probe(struct vip_top *vtop, void *fops)
{
	struct vip_dev *vip;
	int idx;
	int ret;

	if (vtop == NULL) {
		vipm_err("%s : NULL vtop\n", __F__);
		return -ENOMEM;
	}
	idx = vtop->idx;
	vip = vip_top_priv(vtop);

	ret = VIP_Plt_Device_Register(idx);
	if (ret) {
		goto out;
	}

	vip->pdev = VIP_Drv_Get_Plt(idx);

	ret = VIP_Register_Device(vtop, fops, MKDEV(g_vip_major, idx));
	if (ret) {
		goto out_plt_dev;
	}

	/* initialize proc system */
	VIP_PROC_Init(vip_dev_name(vip), MINOR(vtop->devNo));

	VIP_Set_Registered(vip);

	return 0;

out_plt_dev:
	VIP_Plt_Device_Unregister(idx);

out:
	return ret;
}


static void VIP_Drv_Remove(int idx)
{
	struct vip_top *vtop = (struct vip_top *)VIP_Drv_Get_Top(idx);
	if (vtop != NULL) {
		VIP_Drv_Device_Remove(vtop);
		VIP_Drv_Device_Free(vtop);
	}
}

static int VIP_Drv_Probe(int idx)
{
	struct vip_top *vtop;
	int ret = 0;

	vtop = VIP_Drv_Device_Alloc(idx);
	if (vtop == NULL) {
		goto out;
	}

	ret = VIP_Drv_Device_Probe(vtop, VIP_Drv_Get_FOPS(vtop->idx));
	if (ret) {
		VIP_Drv_Device_Free(vtop);
		goto out;
	}

out:
	return ret;
}

static void VIP_Drv_Cleanup(void)
{
	int idx;

	for (idx=0 ; idx<g_vip_max_dev ; idx++) {
		VIP_Drv_Remove(idx);
	}
	g_vip_top_probed = 0;
}

static int VIP_Drv_Init(void)
{
	int idx;
	int probe_try = 0;
	int ret = 0;

	g_vip_top_probed = 0;
	for (idx=0 ; idx<g_vip_max_dev ; idx++) {
		if (!VIP_Drv_Probe(idx)) {
			g_vip_top_probed |= (1<<idx);
		}
		probe_try |= (1<<idx);
	}
	if (probe_try != g_vip_top_probed) {
		vipm_warn("try[0x%x] : success[0x%x]\n",
					probe_try, g_vip_top_probed);
	}
	if (!g_vip_top_probed) {
		ret = -ENODEV;
	}
	return ret;
}

static int VIP_Drv_Clk_Put(void)
{
	struct vip_top *vtop;
	struct vip_dev *vip;
	int idx;
	int result;
	int ret = 0;

	for (idx=0 ; idx<g_vip_max_dev ; idx++) {
		if (g_vip_top_probed & (1<<idx)) {
			vtop = VIP_Drv_Get_Top(idx);
			if (vtop == NULL) {
				vipm_warn("NULL vtop[%d]\n", idx);
				continue;
			}
			vip = vip_top_priv(vtop);
			if (vip == NULL) {
				vipm_warn("NULL vip[%d]\n", idx);
				continue;
			}

			vip->pm_state = ~0;

			result = VIP_Clk_Get(vip);
			if (result) {
				vipm_err("VIP_Clk_Get[%d] failed : %d\n",
						idx, result);
				continue;
			}

			result = VIP_Clk_Put(vip);
			if (result) {
				vipm_err("VIP_Clk_Put[%d] failed : %d\n",
						idx, result);
				continue;
			}
		}
	}

	return ret;
}

#if 0
static int _VIP_CTOP_Init(int ctop_init)
{
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
		/*
		 * CTR16(0xC898E440) [01:01] bus_gating_en_W
		 *                   [01:01] bus_gating_en_R
		 */
		UINT32 reg_val;
		reg_val = CTOP_CTRL_H15A0_READ(VIP, 0x40);
		if (ctop_init) {
			reg_val |= 0x03;	// bus_gating_en_W/R : bypass
		} else {
			reg_val &= ~0x03;
		}
		CTOP_CTRL_H15A0_WRITE(VIP, 0x40, reg_val);

		vipm_info("CTOP %s\n", (ctop_init)?"Initialized":"Uninitialized");
	}
}
#endif

static int VIP_CTOP_Uninit(void)
{
#if defined(USE_CTOP_CODES_FOR_H15)
	if (g_vip_ctop_base != NULL) {
		UINT32 reg_val;

		if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
			reg_val = vip_readl((volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR16));
			reg_val &= ~0x03;
			vip_writel(reg_val, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR16));
		}
		iounmap(g_vip_ctop_base);
		g_vip_ctop_base = NULL;

		vipm_info("CTOP Uninitialized\n");
	}
#endif
	return 0;
}

static int VIP_CTOP_Init(void)
{
#if !defined(VIP_PM_GATING)
	if (g_vip_ctop_base == NULL) {
		UINT32 reg_val;

		if (lx_chip() == LX_CHIP_H15) {
			if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
				if (g_vip_ctop_base == NULL) {
					g_vip_ctop_base = ioremap(VIP_CTOP_REG_BASE, VIP_CTOP_REG_SIZE);
					if (g_vip_ctop_base == NULL) {
						vipm_err("NULL g_vip_ctop_base\n");
						return -ENOMEM;
					}
				}
				vip_writel(0, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR00));
				reg_val = vip_readl((volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR16));
				reg_val |= 0x03;	// bus_gating_en_W/R : bypass
				vip_writel(reg_val, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR16));
				vip_writel(0, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR19));
				vip_writel(0, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR24));
				vip_writel(0, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR30));
				vip_writel(0, (volatile void *)(g_vip_ctop_base + VIP_CTOP_REG_CTR31));

				vipm_info("CTOP Initialized\n");
			}
		}
	}
#endif

	return 0;
}

static int __used _VIP_Init(void)
{
	char *drv_name = VIP_MODULE;
	int count = VIP_NUM_MINORS;
	dev_t devt;
	int ret = 0;

	if (g_vip_top_inited) {
		return 0;
	}

	g_vip_debug_fd = DBG_OPEN(VIP_MODULE);
	if (g_vip_debug_fd < 0) {
		DBG_PRINT_ERROR("debug system shutdowned : %d\n", g_vip_debug_fd);
		goto out;
	}

	OS_DEBUG_EnableModule(g_vip_debug_fd);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_ERR, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_NOTI, DBG_COLOR_NONE);

#if (__VIP_USE_LOGM_FLAG == 1)
	_vip_debug = VIP_ERR | VIP_NOTI;
#else
	_vip_debug = 0 |
				VIPM_DBG_SET_DRV |
				VIPM_DBG_SET_CORE |
				VIPM_DBG_SET_HAL |
			//	VIPM_DBG_SET_MEM |
				VIPM_DBG_SET_QUEUE |
				VIPM_DBG_SET_PM |
				VIPM_DBG_SET_PROC |
				VIPM_DBG_SET_USB |
				VIPM_DBG_SET_DRV_IMG |
				0;
#endif

#if defined(VIP_DBG_PRINT_ON)
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_INFO, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO, DBG_COLOR_NONE);

#if (__VIP_USE_LOGM_FLAG == 1)
	_vip_debug |= VIP_INFO | VIP_DBG_INFO;

	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_DRV, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_CORE, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_HAL, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_MEM, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_QUEUE, DBG_COLOR_NONE);
	OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, VIP_DBG_INFO_DRV_IMG, DBG_COLOR_NONE);
	_vip_debug |= 0 |	\
				VIP_DBG_INFO_DRV |		\
				VIP_DBG_INFO_CORE |		\
				VIP_DBG_INFO_HAL |		\
				VIP_DBG_INFO_MEM |		\
				VIP_DBG_INFO_QUEUE |	\
				VIP_DBG_INFO_DRV_IMG |	\
				0;
#endif
#endif

#if 0
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO);		/* 6 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_2);		/* 7 */

#if (__VIP_USE_LOGM_FLAG == 1)
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_DRV);		/* 16 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_CORE);		/* 17 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_HAL);		/* 18 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_MEM);		/* 19 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_QUEUE);		/* 20 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_PM);			/* 21 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_PROC);		/* 22 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_USB);		/* 23 */
	LOGM_REG_CUS_BIT_NAME(g_vip_debug_fd, VIP_DBG_INFO_DRV_IMG);	/* 24 */
#endif
#endif

	ret = VIP_CTOP_Init();
	if (ret) {
		goto out;
	}

	ret = VIP_Drv_Config();
	if (ret) {
		goto out;
	}

	ret = VIP_Plt_Driver_Register();
	if (ret) {
		goto out;
	}

	ret = VIP_Chrdev_Region(&devt, &g_vip_major, count, drv_name);
	if (ret) {
		goto out_platform;
	}

	ret = VIP_Drv_Init();
	if (ret) {
		goto out_chrdev;
	}

	VIP_Drv_Clk_Put();

	ret = VIP_USB_Init();
	if (ret) {
		goto out_init;
	}

	g_vip_top_inited = 1;

	vipm_noti("VIP Driver Initialized\n");

	return 0;

out_init:
	VIP_Drv_Cleanup();

out_chrdev:
	VIP_Chrdev_Free(devt, count);

out_platform:
	VIP_Plt_Driver_Unregister();

out:
	return ret;
}

static void __used _VIP_Cleanup(void)
{
	VIP_USB_Cleanup();

	VIP_Drv_Cleanup();

	VIP_Chrdev_Free(g_vip_major, VIP_NUM_MINORS);

#if defined(VIP_KDRV_CONFIG_PM)
	VIP_Plt_Driver_Unregister();
#endif

	VIP_CTOP_Uninit();

	g_vip_top_probed = 0;
	g_vip_top_inited = 0;
}

static int __used _VIP_Init_Noop(void)
{
	printk("VIP_Init: VIP not available, disabled\n");
	return 0;
}

static void __used _VIP_Cleanup_Noop(void)
{
	printk("VIP_Cleanup\n");
}

void VIP_PreInit(void)
{
    /* TODO: do something */
}

int VIP_Init(void)
{
#ifdef __VIP_ENABLED
	return _VIP_Init();
#else
	return _VIP_Init_Noop();
#endif
}

void VIP_Cleanup(void)
{
#ifdef __VIP_ENABLED
	_VIP_Cleanup();
#else
	_VIP_Cleanup_Noop();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef KDRV_GLOBAL_LINK
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv", VIP_Init);
#else
module_init(VIP_Init);
#endif
module_exit(VIP_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("VIP base driver");
MODULE_LICENSE("GPL");
#endif

/** @} */


