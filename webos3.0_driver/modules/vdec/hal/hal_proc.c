
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
 * Video decoding driver implementation for VDEC device.
 * VDEC device will teach you how to make device driver with lg115x platform.
 *
 * @author     Junghyun Son(junghyun.son@lge.com)
 * @version    1.0
 * @date       2013.11.20
 * @note       Additional information.
 *
 */

#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h> // copy_to_user
#include <linux/seq_file.h>

#include "hal/vdec_base.h"
#include "log.h"
#include "vdec_drv.h"

logm_define (hal_proc, log_level_warning);

typedef enum {
	BODA,
	TOP,
	SYNC,
	PDEC,
	DEIPC,
} REG_T;

static struct proc_dir_entry *hal_proc_entry;
static struct proc_dir_entry *top_reg_entry;
static struct proc_dir_entry *pdec_reg_entry;
static struct proc_dir_entry *vsync_reg_entry;
static struct proc_dir_entry *deipc_reg_entry;
static struct proc_dir_entry *boda_reg_entry;
static struct proc_dir_entry *hevc_reg_entry;
static struct proc_dir_entry *all_reg_entry;

static void reg_dump(struct seq_file *file, unsigned int addr, unsigned int size)
{
	int i;
	unsigned int *pReg;

	pReg = (unsigned int *)ioremap(addr, size);

	for( i=0; i<size/4; )
	{
		if( i%4 == 0 )
			seq_printf(file, "%08X: ", i*4+addr);
		seq_printf(file, " %08X ",pReg[i]);
		i++;
		if( i%4 == 0 )
			seq_printf(file, "\n");
	}

	if( pReg ) iounmap(pReg);
}

static void reg_write(unsigned int addr, unsigned int value, unsigned int base, unsigned int size)
{
	unsigned int *pReg;

	pReg = (unsigned int *)ioremap(base, size);

	if(pReg)
	{
		pReg[addr-base] = value;
		iounmap(pReg);
	}
}

static ssize_t parse_command(const char __user *buf, loff_t *off, size_t size, unsigned int *addr, unsigned int *val)
{
	unsigned int n = 0;
	char tmp[64], *t, *param[10];

	if (size == 0)
		return 0;

	/* we only allow single write */
	if (*off != 0)
		return -EINVAL;

	if (size > sizeof(tmp)-1)
	{
		logm_warning(hal_proc, "too long string \n");
		return -EINVAL;
	}
	if (copy_from_user (tmp, buf, size) > 0)
		return -EFAULT;
	tmp[size-1] = 0;
	t = tmp;
	while( t != NULL && n < ARRAY_SIZE(param) )
		param[n++] = strsep(&t, " \t");

	if( n > 1 )
	{
		*addr = simple_strtoul(param[0],NULL,0);
		*val = simple_strtoul(param[1],NULL,0);
	}

	return size;
}

static void hevc_reg_show(struct seq_file *file)
{
	int i;
	for( i=0; i<HEVC_NUM_OF_CORE; i++)
	{
		seq_printf(file, "\nhevc%d \n",i);
		reg_dump(file, HEVC_REG_BASE(i), HEVC__REG_SIZE);
	}
}

static int hevc_reg_proc_show(struct seq_file *file, void *data)
{
	hevc_reg_show(file);
	return 0;
}

static int hevc_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, hevc_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t hevc_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int i;
	unsigned int addr, val;

	size = parse_command(buf, off, size, &addr, &val);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	for( i=0; i<HEVC_NUM_OF_CORE; i++)
	{
		if( addr >= HEVC_REG_BASE(i) &&
				addr <= HEVC_REG_BASE(i)+(HEVC__REG_SIZE/4) )
		{
			reg_write(addr, val, HEVC_REG_BASE(i), HEVC__REG_SIZE);
		}
	}
	return size;
}

static int hevc_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations hevc_reg_proc_fops =
{
	.open = hevc_reg_proc_open,
	.read = seq_read,
	.write = hevc_reg_proc_write,
	.llseek = seq_lseek,
	.release = hevc_reg_proc_release,
};

static void msvc_reg_show(struct seq_file *file)
{
	int i;
	for( i=0; i<MSVC_NUM_OF_CORE; i++)
	{
		seq_printf(file, "\nboda%d \n",i);
		reg_dump(file, CNM_BASE(i), 0x200);
	}
}

static int boda_reg_proc_show(struct seq_file *file, void *data)
{
	msvc_reg_show(file);
	return 0;
}

static int boda_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, boda_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t boda_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int i;
	unsigned int addr, val;

	size = parse_command(buf, off, size, &addr, &val);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	for( i=0; i<MSVC_NUM_OF_CORE; i++)
	{
		if( addr >= CNM_BASE(i) &&
				addr <= CNM_BASE(i)+128 )
		{
			reg_write(addr, val, CNM_BASE(i), 0x200);
		}
	}
	return size;
}

static int boda_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations boda_reg_proc_fops =
{
	.open = boda_reg_proc_open,
	.read = seq_read,
	.write = boda_reg_proc_write,
	.llseek = seq_lseek,
	.release = boda_reg_proc_release,
};

static void deipc_reg_show(struct seq_file *file)
{
	int i;
	unsigned int chip, rev, base;

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

	for( i=0; i<DE_IPC_NUM_OF_REG; i++)
	{
		VDEC_GET_DE_IF_BASE(rev, i, base);
		seq_printf(file, "\ndeipc%d \n",i);
		reg_dump(file, base, 0x40);
	}
}

static int deipc_reg_proc_show(struct seq_file *file, void *data)
{
	deipc_reg_show(file);
	return 0;
}

static int deipc_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, deipc_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t deipc_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int i;
	unsigned int addr, val, chip, rev, base;

	size = parse_command(buf, off, size, &addr, &val);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

	for( i=0; i<DE_IPC_NUM_OF_REG; i++)
	{
		VDEC_GET_DE_IF_BASE(rev, i, base);
		if( addr >= base &&
				addr <= base+16 )
		{
			reg_write(addr, val, base, 0x40);
		}

	}
	return size;
}

static int deipc_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations deipc_reg_proc_fops =
{
	.open = deipc_reg_proc_open,
	.read = seq_read,
	.write = deipc_reg_proc_write,
	.llseek = seq_lseek,
	.release = deipc_reg_proc_release,
};

static void vsync_reg_show(struct seq_file *file)
{
	unsigned int chip, rev, base;

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

	VDEC_GET_VSYNC_BASE(rev, base);

	seq_printf(file, "\nsync \n");
	reg_dump(file, base, 0x100);
}

static int vsync_reg_proc_show(struct seq_file *file, void *data)
{
	vsync_reg_show(file);
	return 0;
}

static int vsync_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, vsync_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t vsync_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	unsigned int addr, val, chip, rev, base;

	size = parse_command(buf, off, size, &addr, &val);

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

	VDEC_GET_VSYNC_BASE(rev, base);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	if( addr >= base &&
			addr <= base+64)
	{
		reg_write(addr, val, base, 0x100);
	}

	return size;
}

static int vsync_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations vsync_reg_proc_fops =
{
	.open = vsync_reg_proc_open,
	.read = seq_read,
	.write = vsync_reg_proc_write,
	.llseek = seq_lseek,
	.release = vsync_reg_proc_release,
};

static void pdec_reg_show(struct seq_file *file)
{
	int i;
	for( i=0; i<PDEC_NUM_OF_CHANNEL; i++)
	{
		seq_printf(file, "\npdec%d \n",i);
		reg_dump(file, PDEC_BASE(i), 0x100);
	}
}

static int pdec_reg_proc_show(struct seq_file *file, void *data)
{
	pdec_reg_show(file);
	return 0;
}

static int pdec_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, pdec_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t pdec_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int i;
	unsigned int addr, val;

	size = parse_command(buf, off, size, &addr, &val);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	for( i=0; i<PDEC_NUM_OF_CHANNEL; i++)
	{
		if( addr >= PDEC_BASE(i) &&
				addr <= PDEC_BASE(i)+64 )
		{
			reg_write(addr, val, PDEC_BASE(i), 0x100);
		}
	}

	return size;
}

static int pdec_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations pdec_reg_proc_fops =
{
	.open = pdec_reg_proc_open,
	.read = seq_read,
	.write = pdec_reg_proc_write,
	.llseek = seq_lseek,
	.release = pdec_reg_proc_release,
};

static void top_reg_show(struct seq_file *file)
{
	seq_printf(file, "\nrc0 \n");
	reg_dump(file, VDEC_REG_RC0_BASE, 0x100);
	seq_printf(file, "\nrc1 \n");
	reg_dump(file, VDEC_REG_RC1_BASE, 0x60);
}

static int top_reg_proc_show(struct seq_file *file, void *data)
{
	top_reg_show(file);
	return 0;
}

static int top_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, top_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static ssize_t top_reg_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	unsigned int addr, val;

	size = parse_command(buf, off, size, &addr, &val);

	logm_noti(hal_proc, "addr:%x value:%x \n",addr, val);

	if( addr >= VDEC_REG_RC0_BASE &&
			addr <= VDEC_REG_RC0_BASE+64 )
	{
		reg_write(addr, val, VDEC_REG_RC0_BASE, 0x100);
	}
	else if( addr >= VDEC_REG_RC1_BASE &&
			addr <= VDEC_REG_RC1_BASE+24 )
	{
		reg_write(addr, val, VDEC_REG_RC1_BASE, 0x60);
	}

	return size;
}

static int top_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations top_reg_proc_fops =
{
	.open = top_reg_proc_open,
	.read = seq_read,
	.write = top_reg_proc_write,
	.llseek = seq_lseek,
	.release = top_reg_proc_release,
};

static int all_reg_proc_show(struct seq_file *file, void *data)
{
	msvc_reg_show(file);
	top_reg_show(file);
	vsync_reg_show(file);
	pdec_reg_show(file);
	deipc_reg_show(file);
	return 0;
}

static int all_reg_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, all_reg_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static int all_reg_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations all_reg_proc_fops =
{
	.open = all_reg_proc_open,
	.read = seq_read,
	.release = all_reg_proc_release,
};

void hal_proc_init(void)
{
	hal_proc_entry = proc_mkdir ("reg", vdec_proc_root_entry);
	top_reg_entry = proc_create ("top", 0640,
			hal_proc_entry, &top_reg_proc_fops);
	if (!top_reg_entry)
		logm_warning(hal_proc, "top_reg create failed\n");
	pdec_reg_entry = proc_create ("pdec", 0640,
			hal_proc_entry, &pdec_reg_proc_fops);
	if (!pdec_reg_entry)
		logm_warning(hal_proc, "pdec_reg create failed\n");
	vsync_reg_entry = proc_create ("vsync", 0640,
			hal_proc_entry, &vsync_reg_proc_fops);
	if (!vsync_reg_entry)
		logm_warning(hal_proc, "vsync_reg create failed\n");
	deipc_reg_entry = proc_create ("deipc", 0640,
			hal_proc_entry, &deipc_reg_proc_fops);
	if (!deipc_reg_entry)
		logm_warning(hal_proc, "deipc_reg create failed\n");
	boda_reg_entry = proc_create ("boda", 0640,
			hal_proc_entry, &boda_reg_proc_fops);
	if (!boda_reg_entry)
		logm_warning(hal_proc, "boda_reg create failed\n");
	hevc_reg_entry = proc_create ("hevc", 0640,
			hal_proc_entry, &hevc_reg_proc_fops);
	if (!hevc_reg_entry)
		logm_warning(hal_proc, "hevc_reg create failed\n");
	all_reg_entry = proc_create ("all", 0440,
			hal_proc_entry, &all_reg_proc_fops);
	if (!all_reg_entry)
		logm_warning(hal_proc, "all_reg create failed\n");
}

void hal_proc_cleanup(void)
{
	remove_proc_entry ("all", hal_proc_entry);
	remove_proc_entry ("hevc", hal_proc_entry);
	remove_proc_entry ("boda", hal_proc_entry);
	remove_proc_entry ("deipc", hal_proc_entry);
	remove_proc_entry ("vsync", hal_proc_entry);
	remove_proc_entry ("pdec", hal_proc_entry);
	remove_proc_entry ("top", hal_proc_entry);
	remove_proc_entry ("reg", vdec_proc_root_entry);
}
