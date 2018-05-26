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
 *  vip usb layer implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg115x_vip
 * @{
 */

#ifdef CONFIG_USB

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
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <linux/notifier.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#ifdef SUPPORT_ION
#include <linux/ion.h>
#endif

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_usb.h"
#include "vip_queue.h"
#include "vip_mem.h"
#include "vip_proc.h"

//#define __VIP_USB_MON

#endif	//CONFIG_USB

#if defined(__VIP_USB_MON)

/*
 *
 */
static DEFINE_MUTEX(vip_usb_mutex);
void vip_usb_lock(void)
{
//	vipm_debug_usb("\n");
	mutex_lock(&vip_usb_mutex);
}
void vip_usb_unlock(void)
{
//	vipm_debug_usb("\n");
	mutex_unlock(&vip_usb_mutex);
}

/*
 *
 */
static LIST_HEAD(vip_usb_buses);

struct vip_usb_bus {
	struct list_head bus_link;
	spinlock_t lock;
	struct usb_bus *u_bus;
};

static void VIP_USB_Bus_Clr(void)
{
	struct vip_usb_bus *vubus;
	struct list_head *p;

	vip_usb_lock();

	while (!list_empty(&vip_usb_buses)) {
		p = vip_usb_buses.next;
		vubus = list_entry(p, struct vip_usb_bus, bus_link);
		list_del(p);

		/* usage for vubus */

		/* */
	}

	vip_usb_unlock();
}

static void VIP_USB_Bus_Del(struct usb_bus *ubus)
{
	struct vip_usb_bus *vubus;
	struct list_head *p;

	vip_usb_lock();

	list_for_each (p, &vip_usb_buses) {
		vubus = list_entry(p, struct vip_usb_bus, bus_link);
		if (vubus->u_bus == ubus) {
			list_del(p);
			return;
		}
	}

	vip_usb_unlock();
}

static int VIP_USB_Bus_Gen(struct usb_bus *ubus)
{
	struct vip_usb_bus *vubus;
	int ret = 0;

	vipm_debug_usb("usb bus[0x%p] added\n", ubus);
	vipm_debug_usb(" ->root_hub           : 0x%p\n", ubus->root_hub);

	if ((vubus = kzalloc(sizeof(struct vip_usb_bus), GFP_KERNEL)) == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	spin_lock_init(&vubus->lock);
	INIT_LIST_HEAD(&vubus->bus_link);

	vubus->u_bus = ubus;

	vip_usb_lock();
	list_add_tail(&vubus->bus_link, &vip_usb_buses);
	vip_usb_unlock();

out:
	return ret;
}

/*
 *
 */
static void VIP_USB_Prt_Iad(struct usb_interface_assoc_descriptor *iad)
{
	vipm_debug_usb(" ->bFirstInterface    : 0x%02x\n", iad->bFirstInterface);
	vipm_debug_usb(" ->bInterfaceCount    : 0x%02x\n", iad->bInterfaceCount);
	vipm_debug_usb(" ->bFunctionClass     : 0x%02x\n", iad->bFunctionClass);
	vipm_debug_usb(" ->bFunctionSubClass  : 0x%02x\n", iad->bFunctionSubClass);
	vipm_debug_usb(" ->bFunctionProtocol  : 0x%02x\n", iad->bFunctionProtocol);
}

static inline int _usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}

static inline int _usb_endpoint_type(const struct usb_endpoint_descriptor *epd)
{
	return epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
}

static void VIP_USB_Prt_Ep(struct usb_endpoint_descriptor *ep_desc)
{
	char *type = "";
	char dir;

	dir = _usb_endpoint_dir_in(ep_desc) ? 'I' : 'O';

	/* this isn't checking for illegal values */
	switch (_usb_endpoint_type(ep_desc)) {
	case USB_ENDPOINT_XFER_CONTROL:
		type = "Ctrl";
		dir = 'B';			/* ctrl is bidirectional */
		break;
	case USB_ENDPOINT_XFER_ISOC:
		type = "Isoc";
		break;
	case USB_ENDPOINT_XFER_BULK:
		type = "Bulk";
		break;
	case USB_ENDPOINT_XFER_INT:
		type = "Int.";
		break;
	}

	vipm_debug_usb("[end point descriptor] = 0x%p\n", ep_desc);
	vipm_debug_usb(" ->bEndpointAddress   : 0x%02x(%c)\n", ep_desc->bEndpointAddress, dir);
	vipm_debug_usb(" ->bmAttributes       : 0x%02x(%-4s)\n", ep_desc->bmAttributes, type);
}

static void VIP_USB_Prt_If(struct usb_interface_cache *intfc,
							struct usb_interface *iface,
							int setno)
{
	const struct usb_host_interface *hiface = &intfc->altsetting[setno];
	const struct usb_interface_descriptor *idesc = &intfc->altsetting[setno].desc;
	const char *driver_name = "";
	int active;
	int i;

	if (iface) {
		driver_name = (iface->dev.driver
				? iface->dev.driver->name
				: "(none)");
		active = (idesc == &iface->cur_altsetting->desc);
	}
	vipm_debug_usb("[interface descriptor] = 0x%p\n", idesc);
	vipm_debug_usb(" ->bInterfaceNumber   : 0x%02x\n", idesc->bInterfaceNumber);
	vipm_debug_usb(" ->bAlternateSetting  : 0x%02x\n", idesc->bAlternateSetting);
	vipm_debug_usb(" ->bNumEndpoints      : 0x%02x\n", idesc->bNumEndpoints);
	vipm_debug_usb(" ->bInterfaceClass    : 0x%02x\n", idesc->bInterfaceClass);
	vipm_debug_usb(" ->bInterfaceSubClass : 0x%02x\n", idesc->bInterfaceSubClass);
	vipm_debug_usb(" ->bInterfaceProtocol : 0x%02x\n", idesc->bInterfaceProtocol);
	vipm_debug_usb(" Driver               : %s\n", driver_name);

	for (i = 0; i < hiface->desc.bNumEndpoints; i++) {
		VIP_USB_Prt_Ep(&hiface->endpoint[i].desc);
	}
}

static void VIP_USB_Prt_Config(struct usb_host_config *config,
								int index,
								int active)
{
	struct usb_config_descriptor *config_desc = &config->desc;
	struct usb_interface_cache *intfc;
	struct usb_interface *iface;
	int i, j;

	vipm_debug_usb("udev->config[%d] = 0x%p(%s)\n",
					index, config, (active)?"active":"inactive");

	vipm_debug_usb("udev->config[%d].desc = 0x%p\n", index, config_desc);
	vipm_debug_usb(" ->bLength            : 0x%02x\n", config_desc->bLength);
	vipm_debug_usb(" ->bDescriptorType    : 0x%02x\n", config_desc->bDescriptorType);
	vipm_debug_usb(" ->wTotalLength       : 0x%04x\n", le16_to_cpu(config_desc->wTotalLength));
	vipm_debug_usb(" ->bNumInterfaces     : 0x%02x\n", config_desc->bNumInterfaces);
	vipm_debug_usb(" ->bConfigurationValue : 0x%02x\n", config_desc->bConfigurationValue);
	vipm_debug_usb(" ->bmAttributes       : 0x%02x\n", config_desc->bmAttributes);

	for (i=0; i<USB_MAXIADS ; i++) {
		if (config->intf_assoc[i] == NULL)
			break;
		vipm_debug_usb("udev->config[%d].iad[%d] = 0x%p\n",
					index, i, config->intf_assoc[i]);
		VIP_USB_Prt_Iad(config->intf_assoc[i]);
	}

	for (i=0; i<config_desc->bNumInterfaces ; i++) {
		intfc = config->intf_cache[i];
		iface = config->interface[i];
		for (j = 0; j < intfc->num_altsetting; j++) {
			VIP_USB_Prt_If(intfc, iface, j);
		}
	}
}

static void VIP_USB_Prt_Descriptor(struct usb_device_descriptor *desc)
{
	vipm_debug_usb("udev->descriptor = 0x%p\n", desc);
	vipm_debug_usb(" ->bLength            : 0x%02x\n", desc->bLength);
	vipm_debug_usb(" ->bDescriptorType    : 0x%02x\n", desc->bDescriptorType);
	vipm_debug_usb(" ->bcdUSB             : 0x%04x\n", le16_to_cpu(desc->bcdUSB));
	vipm_debug_usb(" ->bDeviceClass       : 0x%02x\n", desc->bDeviceClass);
	vipm_debug_usb(" ->bDeviceSubClass    : 0x%02x\n", desc->bDeviceSubClass);
	vipm_debug_usb(" ->bDeviceProtocol    : 0x%02x\n", desc->bDeviceProtocol);
	vipm_debug_usb(" ->bMaxPacketSize0    : 0x%02x\n", desc->bMaxPacketSize0);
	vipm_debug_usb(" ->idVendor           : 0x%04x\n", le16_to_cpu(desc->idVendor));
	vipm_debug_usb(" ->idProduct          : 0x%04x\n", le16_to_cpu(desc->idProduct));
	vipm_debug_usb(" ->bcdDevice          : 0x%04x\n", le16_to_cpu(desc->bcdDevice));
	vipm_debug_usb(" ->iManufacturer      : 0x%02x\n", desc->iManufacturer);
	vipm_debug_usb(" ->iProduct           : 0x%02x\n", desc->iProduct);
	vipm_debug_usb(" ->iSerialNumber      : 0x%02x\n", desc->iSerialNumber);
	vipm_debug_usb(" ->bNumConfigurations : 0x%02x\n", desc->bNumConfigurations);
}

static void VIP_USB_Prt_Dev(struct device *dev)
{
	vipm_debug_usb("usb->dev = 0x%p\n", dev);
	vipm_debug_usb("->devt                : 0x%x\n", dev->devt);
}

static void VIP_USB_Prt_Udev(struct usb_device *udev)
{
	int i;

	vipm_debug_usb("usb_device = 0x%p\n", udev);
	vipm_debug_usb(" ->devnum             : 0x%d\n", udev->devnum);
	vipm_debug_usb(" ->speed              : 0x%d\n", udev->speed);
	vipm_debug_usb(" ->parent             : 0x%p\n", udev->parent);
	vipm_debug_usb(" ->bus                : 0x%p\n", udev->bus);
	vipm_debug_usb(" ->product            : 0x%p\n", udev->product);
	vipm_debug_usb(" ->manufacturer       : 0x%p\n", udev->manufacturer);
	vipm_debug_usb(" ->serial             : 0x%p\n", udev->serial);

	if (udev->bus) {
		vipm_debug_usb("udev->bus->busnum = %d\n", udev->bus->busnum);
	}

	VIP_USB_Prt_Dev(&udev->dev);

	VIP_USB_Prt_Descriptor(&udev->descriptor);

	for (i=0; i<udev->descriptor.bNumConfigurations ; i++) {
		VIP_USB_Prt_Config(udev->config + i, i,
						(udev->actconfig == (udev->config + i)));
	}
}

static void VIP_USB_Do_Prt_All(struct usb_device *udev,
							struct usb_bus *bus,
							int level, int index)
{
	int chix;
	struct usb_device *childdev = NULL;

//	vipm_debug_usb("level[%d] index[%d]\n");
//	vipm_debug_usb("", ubus->busnum);

	VIP_USB_Prt_Udev(udev);

	/* Now look at all of this device's children. */
	usb_hub_for_each_child(udev, chix, childdev) {
		usb_lock_device(childdev);
		VIP_USB_Do_Prt_All(childdev, bus, level + 1, chix - 1);
		usb_unlock_device(childdev);
	}
}

static int VIP_USB_Prt_All(void)
{
	struct usb_bus *ubus;
	int ret = 0;

	mutex_lock(&usb_bus_list_lock);
	/* print devices for all busses */
	list_for_each_entry(ubus, &usb_bus_list, bus_list) {
		/* recurse through all children of the root hub */
		if (!bus_to_hcd(ubus)->rh_registered)
			continue;
		usb_lock_device(ubus->root_hub);
		VIP_USB_Do_Prt_All(ubus->root_hub, ubus, 0, 0);
		usb_unlock_device(ubus->root_hub);
	}
	mutex_unlock(&usb_bus_list_lock);

	return ret;
}

static void VIP_USB_Device_Add(struct usb_device *udev)
{
//	int i;

	vip_usb_lock();

	VIP_USB_Prt_Udev(udev);

	vip_usb_unlock();
}

static void VIP_USB_Device_Remove(struct usb_device *udev)
{
//	const struct usb_device_descriptor *desc = &udev->descriptor;

	vip_usb_lock();
	vipm_debug_usb("usb device[0x%p] removed\n", udev);
	vip_usb_unlock();
}

static void VIP_USB_Bus_Add(struct usb_bus *ubus)
{
	VIP_USB_Bus_Gen(ubus);
	vip_usb_lock();
	vip_usb_unlock();
}

static void VIP_USB_Bus_Remove(struct usb_bus *ubus)
{
	VIP_USB_Bus_Del(ubus);
	vip_usb_lock();
	vipm_debug_usb("usb bus[0x%p] removed\n", ubus);
	vip_usb_unlock();
}

static int VIP_USB_Notify(struct notifier_block *self,
					unsigned long action,
					void *dev)
{
	switch (action) {
	case USB_DEVICE_ADD :
		VIP_USB_Device_Add(dev);
		break;
	case USB_DEVICE_REMOVE :
		VIP_USB_Device_Remove(dev);
		break;
	case USB_BUS_ADD:
		VIP_USB_Bus_Add(dev);
		break;
	case USB_BUS_REMOVE:
		VIP_USB_Bus_Remove(dev);
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block vip_usb_nb = {
	.notifier_call = 	VIP_USB_Notify,
};

void VIP_USB_Cleanup(void)
{
	VIP_USB_Bus_Clr();
	usb_unregister_notify(&vip_usb_nb);
}

int VIP_USB_Init(void)
{
	struct usb_bus *ubus;
	int ret = 0;

	mutex_lock(&usb_bus_list_lock);
	list_for_each_entry (ubus, &usb_bus_list, bus_list) {
		ret = VIP_USB_Bus_Gen(ubus);
		if (ret) {
			VIP_USB_Bus_Clr();
			goto out;
		}
	}
	usb_register_notify(&vip_usb_nb);

out:
	mutex_unlock(&usb_bus_list_lock);
	return ret;
}

#else	/* __VIP_USB_MON */

void VIP_USB_Cleanup(void)
{

}

int VIP_USB_Init(void)
{
	return 0;
}

#endif	/* __VIP_USB_MON */


