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
#include "ves/ves_drv.h"
#include "log.h"
#include "vdec_drv.h"
#include "vdc_drv.h"

static int feeddump_count;
module_param(feeddump_count, int, 0644);

logm_define (vdc_proc, log_level_warning);

// extern used at vdc
int feeddump_dumped_count[VDEC_NUM_OF_CHANNEL];
feeddump_mode_t feeddump_mode = feeddump_feed;
bool feeddump_header_found = FALSE;

static int feeddump_dumpcmd = 1;
static struct proc_dir_entry *feeddump_proc_entry;
static struct proc_dir_entry *vdc_proc_entry;
static struct proc_dir_entry *status_proc_entry;
static struct proc_dir_entry *vdc_ctrl_entry;
static unsigned char *feeddump_buf;
static int feeddump_buf_size = 1024*1024;
static int feeddump_buf_head, feeddump_buf_tail;
static spinlock_t feeddump_lock;
static DECLARE_WAIT_QUEUE_HEAD (feeddump_wq);

static int _feeddump_dump (unsigned char *data, int size, int head)
{
	int count1, count2;

	logm_trace (vdc_proc, "head %d, size %d\n",
			head, size);
	if (head + size > feeddump_buf_size)
	{
		/* wraped */
		count1 = feeddump_buf_size - head;
		count2 = head+size - feeddump_buf_size;
	}
	else
	{
		count1 = size;
		count2 = 0;
	}

	logm_trace (vdc_proc, "count1 %d, count2 %d\n",
			count1, count2);

	if (count1)
		memcpy (feeddump_buf + head, data, count1);
	if (count2)
		memcpy (feeddump_buf, data+count1, count2);

	return 0;
}

int feeddump_dump (int ves, int vdc, unsigned int cmd,
		int addr,unsigned char *data, int size)
{
	unsigned long flags;
	unsigned long cpb_base;
	unsigned char *cpb_base_virt;
	int cpb_size;
	unsigned char *data1, *data2;
	int head, tail;
	int free;
	int size1, size2;
	int ret = 0;
	int offset;

	spin_lock_irqsave (&feeddump_lock, flags);
	if (!feeddump_buf)
		goto done;

	if (( ( cmd == FEED_DATA || cmd == FEED_TIMESTAMP ) && feeddump_count > 0
				&& feeddump_count <= feeddump_dumped_count[vdc]))
		goto done;

	if ( feeddump_dumpcmd == 0 && cmd != FEED_DATA )
		goto done;

	/* get CPB information */
	cpb_base = VES_CPB_GetBufferBaseAddr (ves);
	cpb_size = VES_CPB_GetBufferSize (ves);
	cpb_base_virt = VES_CPB_TranslatePhyWrPtr (ves, cpb_base);
	if (!cpb_base || !cpb_base_virt)
	{
		logm_warning (vdc_proc, "Oops\n");
		ret = -1;
		goto done;
	}

	if (addr)
	{
		offset = addr - cpb_base;
		if (size+offset > cpb_size)
		{
			size1 = cpb_size-offset;
			size2 = size-size1;
		}
		else
		{
			size1 = size;
			size2 = 0;
		}
		data1 = cpb_base_virt + offset;
		data2 = cpb_base_virt;
		logm_trace (vdc_proc,
				"size1 0x%X, size2 0x%X from "
				"offset 0x%X(0x%lx)\n",
				size1, size2, offset, cpb_base);
	}
	else if (data)
	{
		data1 = data;
		size1 = size;

		data2 = NULL;
		size2 = 0;

		logm_trace (vdc_proc,
				"data %p, size %d from data\n",
				data, size);
	}
	else
	{
		data1 = data2 = NULL;
		size1 = size2 = 0;
	}

	/* get local temporal buffer information */
	head = feeddump_buf_head;
	tail = feeddump_buf_tail;

	logm_trace (vdc_proc, "before copy tail %d, head %d, size %d\n",
			tail, head, size);

	/* check we have enough free space */
	free = tail-head;
	if (free <= 0)
		free += feeddump_buf_size;
	logm_trace (vdc_proc, "free %d\n", free);

	if (free <= size+12)
	{
		logm_warning (vdc_proc, "queue full\n");
		ret = -1;
		goto done;
	}

	if (feeddump_dumpcmd)
	{

		/* put stream length */
		_feeddump_dump ((unsigned char *)&size, 4, head);
		head += 4;
		head %= feeddump_buf_size;

		/* put vdc ch */
		_feeddump_dump((unsigned char *)&vdc, 4, head);
		head += 4;
		head %= feeddump_buf_size;

		/* put command */
		_feeddump_dump ((unsigned char *)&cmd, 4, head);
		head += 4;
		head %= feeddump_buf_size;

	}

	/* puth stream or argument */
	if (size1)
	{
		_feeddump_dump (data1, size1, head);
		head += size1;
		head %= feeddump_buf_size;
	}

	if (size2)
	{
		_feeddump_dump (data2, size2, head);
		head += size2;
		head %= feeddump_buf_size;
	}

	/* update head pointer */
	feeddump_buf_head = head;

	if( cmd == FEED_DATA && ( feeddump_mode == feeddump_feed
			       || (feeddump_mode==feeddump_header
			  && feeddump_header_found) ) )
		feeddump_dumped_count[vdc]++;

	wake_up (&feeddump_wq);
	logm_debug (vdc_proc, "after  copy tail %d, head %d, written %d\n",
			tail, head, size);

done:
	spin_unlock_irqrestore (&feeddump_lock, flags);

	return ret;
}

static int feeddump_open (struct inode *inode, struct file *file)
{
	unsigned long flags;
	unsigned char *buf;
	int ret = 0;

	logm_debug (vdc_proc, "open..\n");

	/* vmalloc() can be scheduled */
	buf = vmalloc (feeddump_buf_size);
	if (!buf)
	{
		logm_warning (vdc_proc, "no memory\n");
		return -ENOMEM;
	}

	spin_lock_irqsave (&feeddump_lock, flags);
	if (feeddump_buf)
		ret = -EBUSY;
	else
	{
		feeddump_buf = buf;
		feeddump_buf_head = feeddump_buf_tail = 0;
	}
	spin_unlock_irqrestore (&feeddump_lock, flags);

	if (ret < 0)
	{
		logm_warning (vdc_proc, "feeddump busy\n");
		vfree (buf);
		return ret;
	}

	return 0;
}

static ssize_t feeddump_read (struct file *file, char __user *data, size_t size, loff_t *off)
{
	int count1, count2;
	int ret;
	int head, tail;

	logm_trace (vdc_proc, "reading %d bytes\n", size);

	ret = wait_event_interruptible  (feeddump_wq,
			feeddump_buf_head != feeddump_buf_tail);
	if (ret < 0)
		return ret;

	head = feeddump_buf_head;
	tail = feeddump_buf_tail;

	logm_trace (vdc_proc, "before copy tail %d, head %d\n", tail, head);

	count1 = head-tail;
	if (count1 < 0)
		count1 += feeddump_buf_size;
	if (count1 > size)
		count1 = size;

	if (tail+count1 > feeddump_buf_size)
	{
		count2 = tail+count1-feeddump_buf_size;
		count1 = feeddump_buf_size - tail;
	}
	else
		count2 = 0;

	if (count1)
	{
		ret = copy_to_user (data, feeddump_buf + tail, count1);
		if (ret)
			return -EFAULT;
	}
	if (count2)
	{
		ret = copy_to_user (data+count1, feeddump_buf, count2);
		if (ret)
			return -EFAULT;
	}

	tail += count1+count2;
	tail %= feeddump_buf_size;
	feeddump_buf_tail = tail;

	logm_debug (vdc_proc, "after  copy tail %d, head %d, readed %d\n",
			tail, head, count1+count2);

	return count1+count2;
}

static int feeddump_release (struct inode *inode, struct file *file)
{
	unsigned long flags;

	logm_debug (vdc_proc, "close feeddump\n");
	spin_lock_irqsave (&feeddump_lock, flags);
	vfree (feeddump_buf);
	feeddump_buf = NULL;
	feeddump_header_found = FALSE;
	spin_unlock_irqrestore (&feeddump_lock, flags);

	return 0;
}

typedef enum
{
	feed_cmd_idle,
	feed_cmd_dpcmd,
	feed_cmd_nod,
	feed_cmd_mode,
} feed_cmd_state;

static void feeddump_debug(UINT32 ui32Param,CHAR *apParams[])
{
	UINT32 i;
	UINT32 ui32Params[10];
	feed_cmd_state cur_state = feed_cmd_idle;

	for(i=0;i<10 && i<ui32Param;i++) {
		ui32Params[i] = simple_strtoul(apParams[i], NULL, 0);
		switch (cur_state)
		{
			case feed_cmd_idle:
				{
					switch (*apParams[i])
					{
						case 'c':
						case 'C':
							logm_debug(vdc_proc, "cmd dump \n");
							cur_state = feed_cmd_dpcmd;
							break;
						case 'n':
						case 'N':
							logm_debug(vdc_proc, "number of dump \n");
							cur_state = feed_cmd_nod;
							break;
						case 'm':
						case 'M':
							logm_debug(vdc_proc, "dec dump count \n");
							cur_state = feed_cmd_mode;
							break;
						default:
							break;
					}
				}
				break;
			case feed_cmd_dpcmd:
				{
					logm_debug(vdc_proc,"dump cmd? %d\n"
							,ui32Params[i]);
					feeddump_dumpcmd = ui32Params[i];
					cur_state = feed_cmd_idle;
				}
				break;
			case feed_cmd_nod:
				{
					logm_debug(vdc_proc,"num of dump? %d\n"
							,ui32Params[i]);
					feeddump_count = ui32Params[i];
					feeddump_mode = feeddump_feed;
					cur_state = feed_cmd_idle;
				}
				break;
			case feed_cmd_mode:
				{
					logm_debug(vdc_proc,"dump mode?%c\n"
							,*apParams[i]);
					switch (*apParams[i])
					{
						case 'f':
						case 'F':
							feeddump_mode = feeddump_feed;
							break;
						case 'd':
						case 'D':
							feeddump_mode = feeddump_decdone;
							break;
						case 'h':
						case 'H':
							feeddump_mode = feeddump_header;
							break;
					}
					cur_state = feed_cmd_idle;
				}
				break;
			default:
				logm_warning(vdc_proc,"%d: state error? %d \n",i,cur_state);
				break;
		}
	}

}

static struct file_operations feeddump_fops =
{
	.open = feeddump_open,
	.read = feeddump_read,
	.release = feeddump_release,
};

extern void VDC_Print_Stat(struct seq_file *file);
static int vdc_proc_show(struct seq_file *file, void *data)
{
	VDC_Print_Stat(file);

	return 0;
}

static int vdc_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, vdc_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

static int vdc_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations vdc_proc_fops =
{
	.open = vdc_proc_open,
	.read = seq_read,
	.release = vdc_proc_release,
};

extern void VDC_Print_Cmd(struct seq_file *file);
static int vdc_ctrl_proc_show(struct seq_file *file, void *data)
{
	VDC_Print_Cmd(file);

	return 0;
}

static int vdc_ctrl_proc_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open(file, vdc_ctrl_proc_show, NULL);
	if (ret < 0)
		return ret;
	return ret;
}

extern void VDC_DebugCmd(UINT32 ui32Param, CHAR *apParams[]);
void VDC_Proc_ParseCmd(UINT32 ui32Param, CHAR *apParams[])
{
	UINT32 ui32CmdType;

	ui32CmdType = simple_strtol(apParams[0], NULL, 0);

	switch (ui32CmdType)
	{
		case 0:
			feeddump_debug(ui32Param,apParams);
			break;
		case 1:
			VDC_DebugCmd(ui32Param,apParams);
			break;
		default:
			logm_warning(vdc_proc, "unknown command \n");
			break;
	}
}

static ssize_t vdc_ctrl_proc_write(struct file *file,
		const char __user *buf, size_t size, loff_t *off)
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
		logm_warning(vdc_proc, "too long string \n");
		return -EINVAL;
	}
	if (copy_from_user (tmp, buf, size) > 0)
		return -EFAULT;
	tmp[size-1] = 0;
	t = tmp;
	while( t != NULL && n < ARRAY_SIZE(param) )
		param[n++] = strsep(&t, " \t");

	if( n > 2 )
		VDC_Proc_ParseCmd(n, param);

	return size;
}

static int vdc_ctrl_proc_release(struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release(inode, file);

	return ret;
}

static struct file_operations vdc_ctrl_fops =
{
	.open = vdc_ctrl_proc_open,
	.read = seq_read,
	.write = vdc_ctrl_proc_write,
	.llseek = seq_lseek,
	.release = vdc_ctrl_proc_release,
};

void VDC_ProcInit(void)
{
	vdc_proc_entry = proc_mkdir ("vdc", vdec_proc_root_entry);
	feeddump_proc_entry = proc_create ("feeddump", 0440,
			vdec_proc_root_entry, &feeddump_fops);
	if (!feeddump_proc_entry)
		logm_warning(vdc_proc, "feeddump create failed\n");
	vdc_ctrl_entry = proc_create ("debug_vdc", 0640,
			vdc_proc_entry, &vdc_ctrl_fops);
	if( !vdc_ctrl_entry)
		logm_warning(vdc_proc, "debug_vdc create failed\n");
	status_proc_entry = proc_create ("status", 0440,
			vdc_proc_entry, &vdc_proc_fops);
	if (!status_proc_entry)
		logm_warning(vdc_proc, "status_proc create failed\n");
}

void VDC_ProcCleanup(void)
{
	remove_proc_entry ("debug_vdc", vdc_proc_entry);
	remove_proc_entry ("status", vdc_proc_entry);
	remove_proc_entry ("vdc", vdec_proc_root_entry);
	remove_proc_entry ("feeddump", vdec_proc_root_entry);
}
