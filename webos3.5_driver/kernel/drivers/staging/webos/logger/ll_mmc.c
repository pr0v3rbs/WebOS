/*
 * Low Level MMC driver for Logger
 */

#include <linux/types.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include "ll_mmc.h"

static struct ll_mmc_ops *mmc_ops;

void register_ll_mmc_ops(struct ll_mmc_ops *ops)
{
	if (mmc_ops)
		pr_warn("Overriding previously set MMCOOPS ops\n");

	mmc_ops = ops;
	pr_info("%s: Register low level MMC driver for logger", mmc_ops->name);
}

s32 ll_mmc_init(void)
{
	s32 ret = -EINVAL;

	if (mmc_ops && mmc_ops->inited)
		return 0;

	if (mmc_ops && mmc_ops->ll_mmc_init)
		ret = mmc_ops->ll_mmc_init();

	if (ret == 0)
		mmc_ops->inited = true;

	return ret;
}

bool ll_mmc_ready(void)
{
	return (mmc_ops && mmc_ops->ll_mmc_init &&
			mmc_ops->ll_mmc_write && mmc_ops->ll_mmc_read);
}

s32 ll_mmc_write(u8 *buffer, u64 address, u32 size)
{
	if (mmc_ops->ll_mmc_write)
		return mmc_ops->ll_mmc_write(buffer, address, size);

	return -EINVAL;
}

s32 ll_mmc_read(u8 *buffer, u64 address, u32 size)
{
	if (mmc_ops->ll_mmc_read)
		return mmc_ops->ll_mmc_read(buffer, address, size);

	return -EINVAL;
}

void ll_mmc_exit(void)
{
	if (mmc_ops->inited && mmc_ops->ll_mmc_exit)
		mmc_ops->ll_mmc_exit();

	mmc_ops->inited = false;
}
