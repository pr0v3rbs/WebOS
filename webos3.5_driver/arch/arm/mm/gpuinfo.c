/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/spinlock_types.h>
#include <linux/rwlock.h>
#include <linux/seq_file.h>

#include <asm/gpuinfo.h>

static DEFINE_RWLOCK(gpu_info_driver_lock);

struct gpu_info {
	__kernel_ulong_t total;		/* Total pages used by gpu */
	__kernel_ulong_t cached;	/* Cached pages for gpu acceleration */
	__kernel_ulong_t reclaimable;	/* reclaimable pages from cache */
};

static struct gpu_info_driver *gpuinfo_driver = NULL;

int gpu_info_register(struct gpu_info_driver *driver)
{
	unsigned long flags;
	int ret = 0;

	write_lock_irqsave(&gpu_info_driver_lock, flags);

	if(gpuinfo_driver) {
		ret = -EEXIST;
		goto out;
	}

	gpuinfo_driver = driver;

	printk("driver : %p, gpuinfo_driver : %p\n", driver, gpuinfo_driver);

out:
	write_unlock_irqrestore(&gpu_info_driver_lock, flags);
	return ret;
}
EXPORT_SYMBOL(gpu_info_register);

int gpu_info_unregister(struct gpu_info_driver *driver)
{
	unsigned long flags;
	int ret = 0;

	write_lock_irqsave(&gpu_info_driver_lock, flags);
	if(gpuinfo_driver != driver) {
		ret = -EINVAL;
		goto out;
	}

	gpuinfo_driver = NULL;

out:
	write_unlock_irqrestore(&gpu_info_driver_lock, flags);
	return ret;
}
EXPORT_SYMBOL(gpu_info_unregister);

static int gpu_page_state(struct gpu_info *info)
{
	unsigned long flags;

	if(!gpuinfo_driver)
		return -ENODEV;

	read_lock_irqsave(&gpu_info_driver_lock, flags);

	info->total = gpuinfo_driver->getTotal();
	info->cached = gpuinfo_driver->getCached();
	info->reclaimable = gpuinfo_driver->getReclaimable();

	read_unlock_irqrestore(&gpu_info_driver_lock, flags);

	return 0;
}

void arch_report_meminfo(struct seq_file *m)
{
	int ret;
	struct gpu_info gpu;

	ret = gpu_page_state(&gpu);
	if(ret)
		return;

	seq_printf(m, "GPUTotal:       %8lu kB\n",
			gpu.total << 2);
	seq_printf(m, "GPUCached:      %8lu kB\n",
			gpu.cached << 2);
	seq_printf(m, "GPUReclaimable: %8lu kB\n",
			gpu.reclaimable << 2);
}
