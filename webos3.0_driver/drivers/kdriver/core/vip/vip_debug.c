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
 *  debug implementation for vision processor device.
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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/compiler.h>
#include <linux/debugfs.h>
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
#include "vip_usb.h"
#include "vip_queue.h"
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

int VIP_EnableLog(unsigned int flags)
{
	int idx;

	vipm_info("VIP_EnableLog : 0x%08x\n", flags);

	for (idx=0 ; idx<LX_MAX_MODULE_DEBUG_NUM ; idx++) {
		if (flags & (1<<idx)) {
			OS_DEBUG_EnableModuleByIndex(g_vip_debug_fd, idx, DBG_COLOR_NONE);
		} else {
			OS_DEBUG_DisableModuleByIndex(g_vip_debug_fd, idx);
		}
	}
#if (__VIP_USE_LOGM_FLAG == 1)
	_vip_debug = flags;
#endif

	return 0;
}

#define VIP_DEBUG_FORMAT_QUEUE_BUF_NONE	\
		"cp[------- us][----------][----------]"
#define VIP_DEBUG_FORMAT_QUEUE_BUF_ZERO	\
		"cp[zerocpy us][----------][0x%08x]"
#define VIP_DEBUG_FORMAT_QUEUE_BUF_CPY	\
		"cp[%7d us][0x%08x][0x%08x]"

#define VIP_DEBUG_FORMAT_QUEUE_RUN		\
		"op[--- ms]"
#define VIP_DEBUG_FORMAT_QUEUE_DONE		\
		"op[%3d ms]"

#define VIP_DEBUG_FORMAT_QUEUE_TOTAL	\
		" fn[%d] id[0x%04x] flt[%d] err[%d] %s %s\n"

#define vip_dbg_queue_prt(_s, fmt, args...)	\
	do {									\
		if (_s == NULL) {					\
			vipm_noti(fmt, ##args);			\
		} else {							\
			seq_printf(_s, fmt, ##args);	\
		}									\
	} while(0)

int VIP_Debug_Show_Queue(void *priv, void *data, int opt)
{
	struct seq_file *s = NULL;
	struct vip_dev *vip;
	struct vip_req_queue *vq;
	struct vip_req *areq;
	struct vip_buf *buf_kern;
	struct vip_buf *buf_user;
	static int buf_state;
	unsigned int time_us;
	char dbg_buf_info[64];
	char dbg_vip_info[64];
	int no_queue = 0;
//	int i;

	if (priv == NULL) {
		vipm_err("NULL priv\n");
		return -EINVAL;
	}

	if (opt) {
		s = (struct seq_file *)priv;
		vip = s->private;
	} else {
		vip = (struct vip_dev *)priv;
	}

	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -EINVAL;
	}

	if (!VIP_Is_Inited(vip) || !VIP_Is_HW_Inited(vip)) {
		no_queue = 1;
		goto out_no_queue;
	}

	vq = &vip_to_vq(vip);
	if (vq == NULL) {
		no_queue = 2;
		goto out_no_queue;
	}

	spin_lock(&vq_lock(vq));
	areq = list_first_entry_or_null(&vq_q_head(vq),
									struct vip_req,
									queue);
	spin_unlock(&vq_lock(vq));
	if (areq == NULL) {
		no_queue = 3;
		goto out_no_queue;
	}

out_no_queue:
	if (no_queue) {
		vip_dbg_queue_prt(s, "[No queue] (%d)\n", no_queue);
		goto out;
	}

	vip_dbg_queue_prt(s, "[Current queue list]\n");

	spin_lock(&vq_lock(vq));
	list_for_each_entry(areq, &vq_q_head(vq), queue) {
		buf_state = 0;
		buf_kern = areq->ibuf;
		do {
			if (buf_kern == NULL) {
				sprintf(dbg_buf_info, VIP_DEBUG_FORMAT_QUEUE_BUF_NONE);
				break;
			}

			buf_user = buf_kern->bsrc;
			if (buf_user == NULL) {
				sprintf(dbg_buf_info, VIP_DEBUG_FORMAT_QUEUE_BUF_ZERO,
				(unsigned int)buf_kern->flags);
				break;
			}

			time_us = buf_kern->time_us;
			sprintf(dbg_buf_info, VIP_DEBUG_FORMAT_QUEUE_BUF_CPY,
					time_us,
					(unsigned int)buf_kern->flags,
					(unsigned int)buf_user->flags);
		} while(0);

		if (VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_RUN)) {
			sprintf(dbg_vip_info, VIP_DEBUG_FORMAT_QUEUE_RUN);
		} else {
			sprintf(dbg_vip_info, VIP_DEBUG_FORMAT_QUEUE_DONE,
					(unsigned int)areq->elapsed/1000);
		}

		vip_dbg_queue_prt(s, VIP_DEBUG_FORMAT_QUEUE_TOTAL,
					areq->func, areq->index,
					areq->fault, areq->error,
					dbg_buf_info, dbg_vip_info);
	}
	spin_unlock(&vq_lock(vq));

out:
	return 0;
}

#if	defined(CONFIG_DEBUG_FS)

static void VIP_Gen_Ver_String(char *vip_ver, unsigned int lib_ver)
{
	int year, mon, day, hour, min;
	int idx;

	year = (lib_ver>>28) & 0x0F;
	mon = (lib_ver>>24) & 0x0F;
	day = (lib_ver>>16) & 0xFF;
	hour = (lib_ver>>8) & 0xFF;
	min = lib_ver & 0xFF;

	idx = 0;
	vip_ver[idx++] = '2';
	vip_ver[idx++] = '0';
	vip_ver[idx++] = '1';
	vip_ver[idx++] = '0' + year;
	vip_ver[idx++] = '.';
	vip_ver[idx++] = '0' + (mon / 10);
	vip_ver[idx++] = '0' + (mon % 10);
	vip_ver[idx++] = '.';
	vip_ver[idx++] = '0' + (day >> 4);
	vip_ver[idx++] = '0' + (day & 0xF);
	vip_ver[idx++] = ' ';
	vip_ver[idx++] = '0' + (hour >> 4);
	vip_ver[idx++] = '0' + (hour & 0xF);
	vip_ver[idx++] = ':';
	vip_ver[idx++] = '0' + (min >> 4);
	vip_ver[idx++] = '0' + (min & 0xF);
	vip_ver[idx++] = '\0';
}

#define VIP_DEBUG_FORMAT_INFO_MEM		"Mem info\n"
#define VIP_DEBUG_FORMAT_INFO_MEM_BASE	"    base : 0x%08x\n"
#define VIP_DEBUG_FORMAT_INFO_MEM_SIZE	"    size : 0x%08x\n"

#define VIP_DEBUG_FORMAT_INFO_FW		"FW  info\n"
#define VIP_DEBUG_FORMAT_INFO_FW_VER	"    ver  : %s\n"
#define VIP_DEBUG_FORMAT_INFO_FW_BASE	"    base : 0x%08x\n"
#define VIP_DEBUG_FORMAT_INFO_FW_ADDR	"    addr : 0x%08x\n"
#define VIP_DEBUG_FORMAT_INFO_FW_SIZE	"    size : 0x%08x\n"

static int VIP_Info_Show(struct seq_file *s, void *data)
{
	struct vip_dev *vip = s->private;
	char vip_ver[20] = { '\0', };

	spin_lock(vip->lock);

	seq_printf(s, VIP_DEBUG_FORMAT_INFO_MEM);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_MEM_BASE,
			(unsigned int)vip->vhal_ops->cfg.base_addr);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_MEM_SIZE,
			(unsigned int)vip->vhal_ops->cfg.base_size);

	VIP_Gen_Ver_String(vip_ver, vip->lib_ver);

	seq_printf(s, VIP_DEBUG_FORMAT_INFO_FW);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_FW_VER,
			vip_ver);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_FW_BASE,
			(unsigned int)vip->firm_base);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_FW_ADDR,
			(unsigned int)vip->firm_ctrl.addr);
	seq_printf(s, VIP_DEBUG_FORMAT_INFO_FW_SIZE,
			(unsigned int)vip->firm_ctrl.size);

	spin_unlock(vip->lock);

	return 0;
}

static int VIP_Queue_Show(struct seq_file *s, void *data)
{
	return VIP_Debug_Show_Queue(s, data, 1);
}

static int VIP_Info_Open(struct inode *inode, struct file *file)
{
	return single_open(file, VIP_Info_Show, inode->i_private);
}

static int VIP_Queue_Open(struct inode *inode, struct file *file)
{
	return single_open(file, VIP_Queue_Show, inode->i_private);
}

static const struct file_operations gVIP_Info_FOPS = {
	.open		= VIP_Info_Open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations gVIP_Queue_FOPS = {
	.open		= VIP_Queue_Open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int VIP_Debug_Opt_Get(void *data, u64 *val)
{
//	struct vip_dev *vip = data;
	int ret;

	ret = vip_io_lock();
	if (ret) {
		return ret;
	}

	*val = _vip_debug;

	vip_io_unlock();

	return 0;
}

static int VIP_Debug_Opt_Set(void *data, u64 val)
{
//	struct vip_dev *vip = data;
	int ret;

	ret = vip_io_lock();
	if (ret) {
		return ret;
	}

	_vip_debug = val;

	vip_io_unlock();

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(gVIP_Debug_FOPS, VIP_Debug_Opt_Get, VIP_Debug_Opt_Set,
	"%llu\n");

int VIP_Add_Debugfs(void *vip_priv)
{
	struct vip_dev *vip = vip_priv;
	struct dentry *root;

	root = debugfs_create_dir(vip_dev_name(vip), NULL);
	if (IS_ERR(root)) {
		/* Don't complain -- debugfs just isn't enabled */
		return -1;
	}
	if (!root) {
		/* Complain -- debugfs is enabled, but it failed to
		 * create the directory. */
		return -1;
	}

	vip->debugfs_root = root;

	if (!debugfs_create_file("debug", S_IRUSR | S_IWUSR, root, vip, &gVIP_Debug_FOPS))
		goto err_node;

	if (!debugfs_create_file("info", S_IRUSR, root, vip, &gVIP_Info_FOPS))
		goto err_node;

	if (!debugfs_create_file("queue", S_IRUSR, root, vip, &gVIP_Queue_FOPS))
		goto err_node;

	return 0;

err_node:
	debugfs_remove_recursive(root);
	vip->debugfs_root = NULL;
	return -1;
}

void VIP_Remove_Debugfs(void *vip_priv)
{
	struct vip_dev *vip = vip_priv;
	struct dentry *root = vip->debugfs_root;

	if (root) {
		debugfs_remove_recursive(root);
		vip->debugfs_root = NULL;
	}
}

#else	//CONFIG_DEBUG_FS

int VIP_Add_Debugfs(void *vip_priv)
{
	return 0;
}

void VIP_Remove_Debugfs(void *vip_priv)
{

}

#endif	//CONFIG_DEBUG_FS

