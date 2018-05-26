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

#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#include "vds/vdisp_drv.h"
#include "log.h"

logm_define (vds_proc, log_level_warning);

extern void VDISP_PrintDebug(struct seq_file *m);
static int vdisp_show (struct seq_file *m, void *data)
{
	VDISP_PrintDebug(m);

	return 0;
}

static int vdisp_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, vdisp_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int vdisp_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_vdisp_fops =
{
	.open = vdisp_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = vdisp_release,
};

extern void RATE_PrintDebug(struct seq_file *m);
static int rate_show (struct seq_file *m, void *data)
{
	RATE_PrintDebug(m);

	return 0;
}

static int rate_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, rate_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int rate_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_rate_fops =
{
	.open = rate_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = rate_release,
};

extern void VSYNC_PrintDebug(struct seq_file *m);
static int vsync_show (struct seq_file *m, void *data)
{
	VSYNC_PrintDebug(m);

	return 0;
}

static int vsync_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, vsync_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int vsync_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_vsync_fops =
{
	.open = vsync_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = vsync_release,
};

extern void SYNC_PrintDebug(struct seq_file *m);
static int sync_show (struct seq_file *m, void *data)
{
	SYNC_PrintDebug(m);

	return 0;
}

static int sync_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, sync_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int sync_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_sync_fops =
{
	.open = sync_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = sync_release,
};

extern void DEIF_PrintDebug(struct seq_file *m);
static int deif_show (struct seq_file *m, void *data)
{
	DEIF_PrintDebug(m);

	return 0;
}

static int deif_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, deif_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int deif_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_deif_fops =
{
	.open = deif_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = deif_release,
};

extern void VDISP_PrintDebugCmd(struct seq_file *m);
static int vds_dbg_show (struct seq_file *m, void *data)
{
	VDISP_PrintDebugCmd(m);

	return 0;
}

static int vds_dbg_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, vds_dbg_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int vds_dbg_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

extern void VDISP_DebugCmd(UINT32 u32nParam, CHAR *apParams[]);
static ssize_t vds_dbg_write (struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	unsigned int n=0;
	char tmp[64], *t, *param[10], *tok;

	if (size == 0)
		return 0;

	/* we only allow single write */
	if (*off != 0)
		return -EINVAL;

	if (size > sizeof(tmp)-1)
	{
		logm_warning (vds_proc, "too long string.\n");
		return -EINVAL;
	}

	if (copy_from_user (tmp, buf, size) > 0)
		return -EFAULT;

	if (tmp[size-1] != '\0')
		tmp[size-1] = '\0';

	t = tmp;
	while( t != NULL && n < ARRAY_SIZE(param) )
	{
		tok = strsep(&t, " \t");
		if (tok != NULL)
			param[n++] = tok;
	}

	VDISP_DebugCmd(n, param);

	return size;
}

static struct file_operations proc_vds_dbg_fops =
{
	.open = vds_dbg_open,
	.read = seq_read,
	.write = vds_dbg_write,
	.llseek = seq_lseek,
	.release = vds_dbg_release,
};


extern struct proc_dir_entry *vo_proc_root_entry;
static struct proc_dir_entry *proc_entry;
static struct proc_dir_entry *proc_vdisp;
static struct proc_dir_entry *proc_rate;
static struct proc_dir_entry *proc_vsync;
static struct proc_dir_entry *proc_sync;
static struct proc_dir_entry *proc_deif;
static struct proc_dir_entry *proc_vds_dbg;

int VDISP_proc_make_instance(void)
{
	proc_entry = proc_mkdir ("vdisp", vo_proc_root_entry);
	proc_vdisp = proc_create_data ("vdisp", 0440,
			proc_entry, &proc_vdisp_fops, NULL);
	proc_rate = proc_create_data ("rate", 0440,
			proc_entry, &proc_rate_fops, NULL);
	proc_vsync = proc_create_data ("vsync", 0440,
			proc_entry, &proc_vsync_fops, NULL);
	proc_sync = proc_create_data ("sync", 0440,
			proc_entry, &proc_sync_fops, NULL);
	proc_deif = proc_create_data ("deif", 0440,
			proc_entry, &proc_deif_fops, NULL);
	proc_vds_dbg = proc_create_data ("debug", 0640,
			proc_entry, &proc_vds_dbg_fops, NULL);

	return 0;
}


