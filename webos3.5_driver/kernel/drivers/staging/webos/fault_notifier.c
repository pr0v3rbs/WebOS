/*
 * Fault Notifier
 *
 * Copyright (C) 2016 LGE, Inc.
 *
 * Sangseok Lee <sangseok.lee@lge.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "fault noti: " fmt

#include <linux/init.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/fault_notifier.h>
#include <linux/uaccess.h>

enum fn_type {
	FN_KERNEL,
	FN_DBGFRWK,
	MAX_FN,
};

static DEFINE_SPINLOCK(fn_lock);
static DEFINE_SPINLOCK(cold_lock);
static char fn_path[MAX_FN][PATH_MAX];
static char *cold_reason;
static int cold_type;
static int path_registered;
static int fn_enable = FN_TYPE_ALL;

static int fn_proc_show(struct seq_file *m, void *v)
{
	int type = (int)m->private;

	if (type >= MAX_FN)
		return -EINVAL;

	seq_printf(m, "%s\n", fn_path[type]);

	return 0;
}

static int fn_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fn_proc_show, PDE_DATA(inode));
}

static ssize_t fn_proc_write(struct file *file,
			     const char __user *buffer, size_t count,
			     loff_t *pos)
{
	int ret;
	int type = (int)(long)PDE_DATA(file_inode(file));

	if (type >= MAX_FN)
		return -EINVAL;

	if (count >= PATH_MAX - NAME_MAX)
		return -ENOSPC;

	spin_lock(&fn_lock);
	ret = copy_from_user(fn_path[type], buffer, count);
	if (ret) {
		spin_unlock(&fn_lock);
		return -EFAULT;
	}

	fn_path[type][count - 1] = '\0';
	if(type == FN_KERNEL) {
		if(!strncmp(fn_path[FN_KERNEL], "0", 2)) {
			/* clear /proc/fault_noti/kernel */
			path_registered = 0;
			fn_path[FN_KERNEL][0] = '\0';
		} else {
			path_registered = 1;
		}
	}
	spin_unlock(&fn_lock);

	spin_lock(&cold_lock);
	if (cold_reason && type == FN_KERNEL) {
		fn_kernel_notify(cold_type, cold_reason);
		kfree(cold_reason);
		cold_reason = NULL;
	}
	spin_unlock(&cold_lock);

	return count;
}

static const struct file_operations fn_proc_fops = {
	.open		= fn_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= fn_proc_write,
};

static ssize_t fn_test_proc_write(struct file *file,
				  const char __user *buffer, size_t count,
				  loff_t *pos)
{
	char noti_reason[NAME_MAX];

	if (count >= NAME_MAX)
		return -ENOSPC;

	if (copy_from_user(noti_reason, buffer, count))
		return -EFAULT;

	noti_reason[count - 1] = '\0';

	fn_kernel_notify(FN_TYPE_TEST, noti_reason);

	return count;
}

static const struct file_operations fn_test_proc_fops = {
	.write		= fn_test_proc_write,
};

static int __init fault_noti_init(void)
{
	struct proc_dir_entry *proc_fn;

	proc_fn = proc_mkdir("fault_noti", NULL);
	if (!proc_fn)
		return -EEXIST;

	proc_create_data("kernel", 0644, proc_fn, &fn_proc_fops,
			 (void *)FN_KERNEL);
	proc_create_data("dbgfrwk", 0644, proc_fn, &fn_proc_fops,
			 (void *)FN_DBGFRWK);
	proc_create("fn_test", 0200, proc_fn, &fn_test_proc_fops);

	return 0;
}
device_initcall(fault_noti_init);

static char path[PATH_MAX];
int fn_kernel_notify(int type, const char *fmt, ...)
{
	int err;
	size_t len;
	mm_segment_t oldfs;
	struct file *filp;
	va_list args;

	if(!(fn_enable & type))
		return -EACCES;

	if (!fmt)
		return -EINVAL;

	spin_lock(&fn_lock);

	if (path_registered != 1) {
		spin_lock(&cold_lock);
		if (!cold_reason) {
			cold_reason = kmalloc(NAME_MAX, GFP_ATOMIC);
			if(!cold_reason) {
				err = -ENOMEM;
			} else {
				cold_type = type;
				va_start(args, fmt);
				vsnprintf(cold_reason, NAME_MAX, fmt, args);
				va_end(args);

				err = 0;
			}
		} else {
			err = -ENODEV;
		}
		spin_unlock(&cold_lock);
		goto out_err;
	}

	len = snprintf(path, PATH_MAX - NAME_MAX, "%s", fn_path[FN_KERNEL]);

	va_start(args, fmt);
	vsnprintf(&path[len], NAME_MAX, fmt, args);
	va_end(args);

	oldfs = get_fs();
	set_fs(get_ds());

	filp = filp_open(path, O_WRONLY | O_CREAT, 0644);
	if (IS_ERR(filp)) {
		err = PTR_ERR(filp);
		pr_err("open %s error : %d\n", path, err);
		goto out_open;
	}
	spin_unlock(&fn_lock);

	filp_close(filp, NULL);
	set_fs(oldfs);

	return 0;

out_open:
	set_fs(oldfs);
out_err:
	spin_unlock(&fn_lock);

	return err;
}
EXPORT_SYMBOL(fn_kernel_notify);
