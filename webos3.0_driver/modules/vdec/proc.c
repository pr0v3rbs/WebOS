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

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

#include "vdc/vdc_drv.h"
#include "ves/ves_cpb.h"
#include "ves/ves_auib.h"

#include "hal/vdec_base.h"

#include "decoder.h"
#include "log.h"

logm_define (vdec_proc, log_level_warning);
logm_define (vdec_dpbdump, log_level_warning);
logm_define (vdec_cpbdump, log_level_warning);

#if !defined(LOGFUNNEL)
#define LOG_MASK
#endif

#ifdef LOG_MASK
typedef struct
{
	char *name;
	void (*set_level)(int);
} LOG_CMD_T;

static LOG_CMD_T log_cmd[32];

#define _reg_log_cmd(logName, cmdNum)		\
do{		\
	extern void _vdec_set_log_level_ ##logName(int);		\
	log_cmd[cmdNum].name = #logName;				\
	log_cmd[cmdNum].set_level = _vdec_set_log_level_ ##logName;	\
}while(0)

static void set_log_level (unsigned int mask, int level)
{
	int i;

	for (i=0;i<32;i++)
	{
		if (mask & (1<<i))
		{
			if( log_cmd[i].set_level != NULL )
				log_cmd[i].set_level(level);
		}
	}
}


static ssize_t logmask_write (struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int level;
	unsigned int mask, n=0;
	char tmp[64], *t, *param[10];

	if (size == 0)
		return 0;

	/* we only allow single write */
	if (*off != 0)
		return -EINVAL;

	if (size > sizeof(tmp)-1)
	{
		logm_warning (vdec_proc, "too long string.\n");
		return -EINVAL;
	}
	if (copy_from_user (tmp, buf, size) > 0)
		return -EFAULT;

	tmp[size-1] = 0;
	t = tmp;
	while( t != NULL && n < ARRAY_SIZE(param) )
		param[n++] = strsep(&t, " \t");

	if( param[0][0] == '0' && param[0][1] == 'x' )
	{
		mask = simple_strtoul(param[0], NULL, 0);
		if(n > 1)
			level = simple_strtol(param[1], NULL, 0);
		else
			level = 9;
	}
	else
	{
		if(n > 1)
		{
			mask = 1 << simple_strtoul(param[0], NULL, 0);
			level = simple_strtol(param[1], NULL, 0);
		}
		else
		{
			mask = 0xFFFFFFFF;
			level = simple_strtoul(param[0], NULL, 0);
		}
	}

	logm_debug (vdec_proc, "vdec set 0x%X mask's level to %d\n",
			mask, level);
	set_log_level (mask, level);

	return size;
}

static int logmask_show (struct seq_file *m, void *data)
{
	int i;

	for (i=0; i<32; i++)
		if( log_cmd[i].set_level != NULL )
			seq_printf (m, "%2d : %s\n", i, log_cmd[i].name );

	return 0;
}

static int logmask_open (struct inode *inode, struct file *file)
{
	return single_open (file, logmask_show, NULL);;
}

static int logmask_release (struct inode *inode, struct file *file)
{
	return single_release (inode, file);
}

static struct file_operations logmask_fops =
{
	.open = logmask_open,
	.read = seq_read,
	.write = logmask_write,
	.llseek = seq_lseek,
	.release = logmask_release,
};
#endif


static volatile unsigned int *power_cpu, *power_ctop;

static int power_open (struct inode *inode, struct file *file)
{
	if (!power_cpu)
		power_cpu = ioremap (0xfd300000, 0x200);
	if (!power_ctop)
		power_ctop = ioremap (0xc001b000, 0x100);

	if (!power_cpu || !power_ctop)
	{
		logm_warning (vdec, "ioremap failed. %p, %p\n",
				power_cpu, power_ctop);
		return -EINVAL;
	}

	return 0;
}

static int power_release (struct inode *inode, struct file *file)
{
	return 0;
}

static int power_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
	return 0;
}

static int power_status = 1;
static unsigned int CTR05_backup, CTR31_backup, CTR04_backup, CTR29_backup;
//extern int VPU_SleepWake(unsigned int coreIdx, int iSleepWake);

#define CTR05		(power_ctop[0x14/4])
#define CTR05_mask	(7<<14)
#define CTR31		(power_ctop[0x7c/4])
#define CTR31_mask	(7<<17)
#define CTR04		(power_ctop[0x10/4])
#define CTR04_mask	(1<<22)
#define CTR29		(power_ctop[0x74/4])
#define CTR29_mask	(1<<17)

#define backup_and_set(r)	do{r##_backup = r; r = r##_backup | r##_mask;}while(0)
#define restore(r)		do{r = (r & ~r##_mask) | (r##_backup & r##_mask);}while(0)

static int power_off (void)
{
	int ret;

	logm_noti (vdec, "power off...\n");

	if (!power_status)
	{
		logm_warning (vdec, "already off. do nothing.\n");
		return -1;
	}

	/* turn off the vpu */
	//ret = VPU_SleepWake (0, 1);
	ret = 0;
	logm_debug (vdec, "VPU_SleepWake %d\n", ret);

	/* flush */
	logm_debug (vdec, "flush set\n");
	power_cpu[0x15c/4] = power_cpu[0x15c/4] | 0x4;

	logm_debug (vdec, "flush wait...\n");
	while (!(power_cpu[0x158/4]&0x4));
	logm_debug (vdec, "flush got wait.\n");

	/* reset */
	logm_debug (vdec, "reset set...\n");
	backup_and_set (CTR04);
	backup_and_set (CTR29);
	logm_debug (vdec, "reset done.\n");

	/* turn off the clock */
	logm_debug (vdec, "clock off...\n");
	backup_and_set (CTR05);
	backup_and_set (CTR31);
	logm_debug (vdec, "clock off done.\n");

	power_status = 0;

	return 0;
}

static int power_on (void)
{
	int ret;

	logm_noti (vdec, "power on...\n");

	if (power_status)
	{
		logm_warning (vdec, "already on. do nothing.\n");
		return -1;
	}

	/* flush */
	logm_debug (vdec, "flush set\n");
	power_cpu[0x15c/4] = power_cpu[0x15c/4] & ~0x4;

	/* turn on the clock */
	logm_debug (vdec, "clock off...\n");
	restore (CTR05);
	restore (CTR31);
	logm_debug (vdec, "clock off done.\n");

	/* reset */
	logm_debug (vdec, "reset set...\n");
	restore (CTR04);
	restore (CTR29);
	logm_debug (vdec, "reset done.\n");

	/* turn on the vpu */
	//ret = VPU_SleepWake (0, 0);
	ret = 0;
	logm_debug (vdec, "VPU_SleepWake %d\n", ret);

	power_status = 1;

	return 0;
}

static int power_write (struct file *file, const char __user *buf, size_t size, loff_t *off)
{
	if (size <= 0)
		return -EINVAL;

	if (*off)
		return size;

	if (buf[0] == '0')
		power_off ();
	else
		power_on ();

	logm_noti (vdec, "done.\n");

	return size;
}

static struct file_operations power_fops =
{
	.open = power_open,
	.read = power_read,
	.write = power_write,
	.release = power_release,
};



/* vdec status proc */
static int status_show (struct seq_file *m, void *data)
{
	decoded_buffer_t current_decoded;
	vdec_t *vdec = m->private;
	unsigned long flag;
	int cpb_read, cpb_write, cpb_size;
	int auib_depth;
	char *tmp;
	struct decoded_info *now;
	int a;
	int tmp_len;

	spin_lock_irqsave (&vdec->decoded_buffers_lock, flag);
	if (vdec->decoded_infos == NULL)
		memset (&current_decoded, 0, sizeof (current_decoded));
	else
		current_decoded = vdec->decoded_infos->buffer;
	spin_unlock_irqrestore (&vdec->decoded_buffers_lock, flag);

#define seq_print_vdec(v,f)	seq_printf (m, #v"="f"\n", vdec->v)
#define seq_print_vdect(v,f,t)	seq_printf (m, #v"="f"\n", (t)vdec->v)
	seq_print_vdec (proc_name, "%s");
	seq_print_vdec (ves, "%d");
	seq_print_vdec (vdc, "%d");
	seq_print_vdec (sync, "%d");
	seq_printf (m, "src=%s(%d)\n", src_name (vdec->src), vdec->src);
	seq_print_vdec (dest, "%d");
	seq_print_vdec (opmode, "%d");
	seq_printf (m, "codec=%s(%d)\n", codec_name (vdec->codec), vdec->codec);
	seq_printf (m, "preferred_dec=%x(%c%c%c%c)\n", vdec->preferred_dec,
			(vdec->preferred_dec>>0 )&0xff?(vdec->preferred_dec>>0 )&0xff:'_',
			(vdec->preferred_dec>>8 )&0xff?(vdec->preferred_dec>>8 )&0xff:'_',
			(vdec->preferred_dec>>16)&0xff?(vdec->preferred_dec>>16)&0xff:'_',
			(vdec->preferred_dec>>24)&0xff?(vdec->preferred_dec>>24)&0xff:'_'
			);
	seq_print_vdec (scan_mode, "%d");
	seq_print_vdec (trid_type, "%d");
	seq_print_vdec (userdata_thief, "%d");
	seq_print_vdec (framerate_num, "%d");
	seq_print_vdec (framerate_den, "%d");
	seq_print_vdec (display_offset, "%d");
	seq_print_vdec (use_gstc, "%d");
	seq_print_vdec (base_stc, "%08x");
	seq_print_vdec (base_pts, "%08x");
	seq_print_vdec (adec_base_stc, "%08x");
	seq_print_vdec (adec_base_pts, "%08x");
	seq_print_vdec (match_mode, "%d");
	seq_print_vdec (kick_decoder_count, "%d");
	seq_print_vdec (frame_requested, "%d");
	seq_print_vdec (last_display_pts, "%u");
	seq_print_vdec (last_displayed_pts, "%u");
	seq_print_vdec (id, "%ld");
	seq_print_vdec (port, "%d");
	seq_print_vdec (port_adec, "%d");
	seq_print_vdec (output_memory_format, "%d");
	seq_print_vdec (noti.tail, "%d");
	seq_print_vdec (noti.head, "%d");
	seq_print_vdec (noti.data, "%p");
	seq_print_vdec (decoding_queue_slots, "%d");
	seq_print_vdect(cpb_addr, "%08llx", unsigned long long);
	seq_print_vdec (cpb_vaddr, "%lx");
	seq_print_vdect(cpb_daddr, "%llx", u64);
	seq_print_vdec (cpb_size, "%d");
	seq_print_vdec (frame_decoded, "%d");
	seq_print_vdec (frame_dropped, "%d");
	seq_print_vdec (frame_displayed, "%d");
	seq_printf (m, "flags=%s\n", vdec_flag_name (vdec->flags));
#undef seq_print_vdec

	if (vdec->ves != 0xff)
	{
		cpb_read = VES_CPB_GetPhyRdPtr(vdec->ves)-vdec->cpb_addr;
		cpb_write = VES_CPB_GetPhyWrPtr(vdec->ves)-vdec->cpb_addr;
		auib_depth = VES_AUIB_NumActive(vdec->ves);
	}
	else
		cpb_read = cpb_write = auib_depth = 0;

	cpb_size = cpb_write - cpb_read;
	if (cpb_size < 0)
		cpb_size += vdec->cpb_size;

	seq_printf (m, "cpb_read=%08x\n", cpb_read);
	seq_printf (m, "cpb_write=%08x\n", cpb_write);
	seq_printf (m, "cpb_data=%d bytes\n", cpb_size);
	seq_printf (m, "auib_depth=%d\n", auib_depth);

	spin_lock_irqsave (&vdec->decoded_buffers_lock, flag);
	for (
			tmp_len=1, now=vdec->decoded_infos;
			now;
			now=now->next, tmp_len++
	    );
	tmp = kmalloc (tmp_len, GFP_ATOMIC);
	if (tmp)
	{
		for (
				a=0, now=vdec->decoded_infos;
				now;
				a++, now=now->next
		    )
		{
			if (now->buffer.buffer_index < 10)
				tmp[a] = now->buffer.buffer_index + '0';
			else
				tmp[a] = now->buffer.buffer_index - 10 + 'a';
		}
		tmp[a] = 0;
	}
	spin_unlock_irqrestore (&vdec->decoded_buffers_lock, flag);

	if (tmp)
	{
		seq_printf (m, "decoded_buffer_indexes=%s\n", tmp);
		kfree (tmp);
	}
	else
		seq_printf (m, "decoded_buffer_indexes=<nomem>");


#define seq_print_buf(v,f)	seq_printf (m, #v"="f"\n", v)
	seq_print_buf (current_decoded.decoder_priv, "%p");
	seq_print_buf (current_decoded.addr_y, "%07lx");
	seq_print_buf (current_decoded.addr_cb, "%07lx");
	seq_print_buf (current_decoded.addr_cr, "%07lx");
	seq_print_buf (current_decoded.buffer_index, "%d");
	seq_print_buf (current_decoded.error_blocks, "%d");
	seq_printf (m, "current_decoded.report=%d(", current_decoded.report);
	if (current_decoded.report & DECODED_REPORT_LOW_DELAY)
		seq_printf (m, "low_delay,");
	if (current_decoded.report & DECODED_REPORT_HW_RESET)
		seq_printf (m, "hw_reset,");
	if (current_decoded.report & DECODED_REPORT_EOS)
		seq_printf (m, "eos,");
	if (current_decoded.report & DECODED_REPORT_REQUEST_DPB)
		seq_printf (m, "request_dpb,");
	if (current_decoded.report & DECODED_REPORT_SEQUENCE_INIT_FAILED)
		seq_printf (m, "sequence_init_failed,");
	if (current_decoded.report & DECODED_REPORT_DECODE_FAILED)
		seq_printf (m, "decode_failed,");
	if (current_decoded.report & DECODED_REPORT_NOT_SUPPORTED_STREAM)
		seq_printf (m, "not_supported_stream,");
	if (current_decoded.report & DECODED_REPORT_RESOURCE_BUSY)
		seq_printf (m, "resource_busy,");
	if (current_decoded.report & DISPLAY_REPORT_DROPPED)
		seq_printf (m, "display_dropped,");
	seq_printf (m, ")\n");

	seq_print_buf (current_decoded.framerate_num, "%d");
	seq_print_buf (current_decoded.framerate_den, "%d");
	seq_print_buf (current_decoded.crop_left, "%d");
	seq_print_buf (current_decoded.crop_right, "%d");
	seq_print_buf (current_decoded.crop_top, "%d");
	seq_print_buf (current_decoded.crop_bottom, "%d");
	seq_print_buf (current_decoded.stride, "%d");
	seq_print_buf (current_decoded.width, "%d");
	seq_print_buf (current_decoded.height, "%d");
	seq_print_buf (current_decoded.display_width, "%d");
	seq_print_buf (current_decoded.display_height, "%d");
	{
		char *i;

		switch (current_decoded.interlace)
		{
		case decoded_buffer_interlace_top_first:
			i="top_first";
			break;
		case decoded_buffer_interlace_bottom_first:
			i="bottom_first";
			break;
		case decoded_buffer_interlace_none:
			i="progressive";
			break;
		default:
			i="unknown";
			break;
		}

		seq_printf (m, "current_decoded.interlace=%s(%d)\n",
				i, current_decoded.interlace);
	}
	seq_print_buf (current_decoded.picture_type, "%d");
	seq_printf (m, "current_decoded.display_period=%d(%d)\n",
			current_decoded.display_period,
			current_decoded.display_period&0x7);
	seq_print_buf (current_decoded.active_format, "%d");
	seq_print_buf (current_decoded.par_w, "%d");
	seq_print_buf (current_decoded.par_h, "%d");
	seq_print_buf (current_decoded.frame_packing_arrangement, "%d");
	seq_print_buf (current_decoded.dts, "%u");
	seq_print_buf (current_decoded.pts, "%u");
	seq_print_buf (current_decoded.timestamp, "%lld");
	seq_print_buf (current_decoded.multi_picture, "%d");
	seq_print_buf (current_decoded.uid, "%ld");
#undef seq_print_buf

	seq_printf (m, "\n");

	return 0;
}

static int status_open (struct inode *inode, struct file *file)
{
	vdec_t *vdec;
	int ret;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	vdec = dp->data;

#else
	vdec = PDE_DATA(inode);
#endif
	ret = single_open (file, status_show, vdec);
	if (ret < 0)
		return ret;

	mutex_lock (&vdec->submodule_lock);

	return ret;
}

static int status_release (struct inode *inode, struct file *file)
{
	int ret;
	vdec_t *vdec;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
        struct proc_dir_entry *dp;
        dp = PDE (inode);
        vdec = dp->data;

#else
	vdec = PDE_DATA (inode);
#endif
	mutex_unlock (&vdec->submodule_lock);

	ret = single_release (inode, file);

	return ret;
}

static struct file_operations proc_status_fops =
{
	.open = status_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = status_release,
};


static DEFINE_MUTEX (cpbdump_lock);
static int cpbdump_init (vdec_t *vdec, int (*cpb_read)(vdec_t *))
{
	logm_debug (vdec_cpbdump, "vdec:%p, ...\n", vdec);

	if (vdec->cpb_addr == 0 || vdec->cpb_size == 0)
	{
		logm_warning (vdec_cpbdump, "vdec:%p, no cpb address\n", vdec);
		return -EINVAL;
	}

	if (vdec->cpbdump.addr == NULL)
	{
		init_waitqueue_head (&vdec->cpbdump.wq);
		vdec->cpbdump.now = cpb_read (vdec);
		logm_debug (vdec_cpbdump, "vdec:%p, current read %d\n", vdec,
				vdec->cpbdump.now);
		if (vdec->cpbdump.now < 0)
		{
			logm_warning (vdec_cpbdump, "vdec:%p, wrong cpb read\n",
					vdec);
			return -EINVAL;
		}
		vdec->cpbdump.readed = vdec->cpbdump.now;

		vdec->cpbdump.addr = vdec_remap (vdec->cpb_addr, vdec->cpb_size);
		if (vdec->cpbdump.addr == NULL)
		{
			logm_warning (vdec_cpbdump, "vdec:%p, vdec_remap failed\n", vdec);
			return -EINVAL;
		}
	}
	else
	{
		logm_warning (vdec_cpbdump, "vdec:%p, already opened?\n", vdec);
		return -EBUSY;
	}

	vdec->cpbdump.cpb_read = cpb_read;

	return 0;
}


static int cpbdump_cleanup (vdec_t *vdec)
{
	vdec_unmap (vdec->cpbdump.addr);
	vdec->cpbdump.addr = NULL;

	return 0;
}

static ssize_t cpbdump_read( struct file *file,
			char __user *data,
			size_t size,
			loff_t *off  )
{
	vdec_t *vdec = file->private_data;
	struct cpbdump *d = &vdec->cpbdump;
	int count1, count2;
	int ret;

	if (d->now == d->readed)
	{
		int got;

		ret = wait_event_interruptible (d->wq,
				(vdec->ves != 0xff &&
				 d->now != d->cpb_read (vdec)));
		if (ret < 0)
			return ret;

		d->now = d->cpb_read (vdec);
		if (d->now < 0)
		{
			logm_warning (vdec_cpbdump, "vdec:%p, wrong cpb read\n",
					vdec);
			return -EINVAL;
		}

		got = d->now - d->readed;
		if (got < 0)
			got += vdec->cpb_size;

		logm_debug (vdec_cpbdump, "vdec:%p, readed %d, now %d, got %d bytes\n",
				vdec, d->readed, d->now, got);
	}

	count1 = d->now - d->readed;
	count2 = 0;
	if (count1 < 0)
	{
		count1 = vdec->cpb_size - d->readed;
		count2 = d->now;
	}

	if (count1 > size)
	{
		count1 = size;
		count2 = 0;
	}
	if (count1+count2 > size)
		count2 = size-count1;
	logm_debug (vdec_cpbdump, "vdec:%p, copy %d, %d bytes\n", vdec,
			count1, count2);

	ret = copy_to_user (data, d->addr+d->readed, count1);
	if (ret > 0)
	{
		logm_warning (vdec_cpbdump, "vdec:%p, copy failed\n", vdec);
		return -EINVAL;
	}
	if (count2 > 0)
	{
		ret = copy_to_user (data+count1, d->addr, count2);
		if (ret > 0)
		{
			logm_warning (vdec_cpbdump, "vdec:%p, copy failed\n", vdec);
			return -EINVAL;
		}
	}

	d->readed += count1 + count2;
	d->readed %= vdec->cpb_size;

	return count1+count2;
}

/* vdec cpb dump proc */
static int cpb_write_addr (vdec_t *vdec)
{
	int read;
	int offset;

	read = VES_CPB_GetPhyWrPtr(vdec->ves);
	offset = read - vdec->cpb_addr;

	if (offset < 0 || offset >= vdec->cpb_size)
	{
		logm_warning (vdec_cpbdump,
				"vdec:%p, wrong cpb read pointer. "
				"%08x, %08llx\n", vdec,
				read, (unsigned long long)vdec->cpb_addr);
		offset = -1;
	}

	return offset;
}

static int cpbdump_writer_open (struct inode *inode, struct file *file)
{
	vdec_t *vdec;
	int ret = 0;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;
	dp = PDE (inode);
	vdec = dp->data;

#else
	vdec =  PDE_DATA (inode);
#endif
	file->private_data = vdec;

	if (vdec->file && !atomic_long_inc_not_zero (&vdec->file->f_count))
	{
		logm_warning (vdec_cpbdump, "vdec:%p, already closed?\n", vdec);
		return -EINVAL;
	}

	mutex_lock (&cpbdump_lock);
	ret = cpbdump_init (vdec, cpb_write_addr);
	mutex_unlock (&cpbdump_lock);

	if (vdec->file && ret < 0)
		fput (vdec->file);

	return ret;
}

static int cpbdump_writer_release (struct inode *inode, struct file *file)
{
	vdec_t *vdec = file->private_data;

	logm_debug (vdec_cpbdump, "vdec:%p, close...\n", vdec);

	cpbdump_cleanup (vdec);

	if (vdec->file)
		fput (vdec->file);

	return 0;
}

static struct file_operations proc_cpbdump_writer_fops =
{
	.open = cpbdump_writer_open,
	.read = cpbdump_read,
	.release = cpbdump_writer_release,
};

/* vdec cpb dump proc */
static int cpb_read_addr (vdec_t *vdec)
{
	int read;
	int offset;

	read = VES_CPB_GetPhyRdPtr(vdec->ves);
	offset = read - vdec->cpb_addr;

	if (offset < 0 || offset >= vdec->cpb_size)
	{
		logm_warning (vdec_cpbdump,
				"vdec:%p, wrong cpb read pointer. "
				"%08x, %08llx\n", vdec,
				read, (unsigned long long)vdec->cpb_addr);
		offset = -1;
	}

	return offset;
}

static int cpbdump_reader_open (struct inode *inode, struct file *file)
{
	vdec_t *vdec;
        int ret = 0;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
        struct proc_dir_entry *dp;

        dp = PDE (inode);
        vdec = dp->data;
#else
	vdec =  PDE_DATA (inode);
#endif
	file->private_data = vdec;

	if (vdec->file && !atomic_long_inc_not_zero (&vdec->file->f_count))
	{
		logm_warning (vdec_cpbdump, "vdec:%p, already closed?\n", vdec);
		return -EINVAL;
	}

	mutex_lock (&cpbdump_lock);
	ret = cpbdump_init (vdec, cpb_read_addr);
	mutex_unlock (&cpbdump_lock);

	if (vdec->file && ret < 0)
		fput (vdec->file);

	return ret;
}

static int cpbdump_reader_release (struct inode *inode, struct file *file)
{
	vdec_t *vdec = file->private_data;

	logm_debug (vdec_cpbdump, "vdec:%p, close...\n", vdec);

	cpbdump_cleanup (vdec);

	if (vdec->file)
		fput (vdec->file);

	return 0;
}


static struct file_operations proc_cpbdump_reader_fops =
{
	.open = cpbdump_reader_open,
	.read = cpbdump_read,
	.release = cpbdump_reader_release,
};


/* vdec dpb dump proc */
static int dpbdump_open (struct inode *inode, struct file *file)
{
	vdec_t *vdec;
	int ret = 0;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;
	dp = PDE (inode);
	vdec = dp->data;
#else
	vdec =  PDE_DATA (inode);
#endif
	file->private_data = vdec;

	if (vdec->file && !atomic_long_inc_not_zero (&vdec->file->f_count))
	{
		logm_warning (vdec_dpbdump, "vdec:%p, already closed?\n", vdec);
		return -EINVAL;
	}

	if (vdec->dpbdump.data == NULL)
	{
		int size;

		memset (&vdec->dpbdump, 0, sizeof (vdec->dpbdump));

		init_waitqueue_head (&vdec->dpbdump.wq);
		vdec->dpbdump.size = 64;
		size = vdec->dpbdump.size * sizeof (decoded_buffer_t *);

		vdec->dpbdump.data = kmalloc (size, GFP_KERNEL);
		if (vdec->dpbdump.data == NULL)
		{
			logm_warning (vdec_dpbdump, "vdec:%p, no memory\n", vdec);
			ret = -ENOMEM;
		}
	}
	else
	{
		logm_warning (vdec_dpbdump, "vdec:%p, dpbdump already opened\n",
				vdec);
		ret = -EBUSY;
	}

	return ret;
}

static int dpbdump_release (struct inode *inode, struct file *file)
{
	vdec_t *vdec = file->private_data;
	unsigned long flag;
	decoded_buffer_t **data;

	spin_lock_irqsave (&vdec->dpbdump_lock, flag);
	data = vdec->dpbdump.data;
	vdec->dpbdump.data = NULL;
	spin_unlock_irqrestore (&vdec->dpbdump_lock, flag);

	/* free remaining decoded buffer */
	mutex_lock (&vdec->submodule_lock);
	if (vdec->dpbdump.cur)
		unref_frame_and_kick_decoder_locked (vdec,
				vdec->dpbdump.cur->addr_y);
	while (vdec->dpbdump.head != vdec->dpbdump.tail)
	{
		decoded_buffer_t *buf;

		buf = data[vdec->dpbdump.tail];
		vdec->dpbdump.tail ++;
		vdec->dpbdump.tail %= vdec->dpbdump.size;

		unref_frame_and_kick_decoder_locked (vdec, buf->addr_y);
	}
	mutex_unlock (&vdec->submodule_lock);

	/* free framebuffer or iounmap */
	if (vdec->dpbdump.fb)
		vfree (vdec->dpbdump.fb);
	if (vdec->dpbdump.fb_y)
	{
		vdec_unmap (vdec->dpbdump.fb_y);
		vdec_unmap (vdec->dpbdump.fb_c);
	}

	/* free queue data buffer */
	kfree (data);

	if (vdec->file)
		fput (vdec->file);

	return 0;
}

static ssize_t dpbdump_read( struct file *file,
			char __user *data,
			size_t size,
			loff_t *off  )
{
	vdec_t *vdec = file->private_data;
	int ret = -1;
	decoded_buffer_t *buf;
	int copied = 0;

	if (vdec->dpbdump.fb_size <= vdec->dpbdump.fb_readed)
	{
		logm_debug (vdec_dpbdump, "vdec:%p, get new buffer\n", vdec);

		/* get new buffer */
		ret = wait_event_interruptible (vdec->dpbdump.wq,
				(vdec->dpbdump.head != vdec->dpbdump.tail));
		if (ret < 0)
			return ret;

		buf = vdec->dpbdump.data[vdec->dpbdump.tail];
		vdec->dpbdump.tail ++;
		vdec->dpbdump.tail %= vdec->dpbdump.size;

		/* unref previous frame */
		if (vdec->dpbdump.cur)
		{
			mutex_lock (&vdec->submodule_lock);
			unref_frame_and_kick_decoder_locked (vdec,
					vdec->dpbdump.cur->addr_y);
			mutex_unlock (&vdec->submodule_lock);
		}
		vdec->dpbdump.cur = buf;

		logm_debug (vdec_dpbdump, "vdec:%p, get %dx%d(%d), "
				"y %07lx, c %07lx\n",
				vdec, buf->width, buf->height, buf->stride,
				buf->addr_y, buf->addr_cb);

		if (vdec->dpbdump.fb_y)
		{
			vdec_unmap (vdec->dpbdump.fb_y);
			vdec_unmap (vdec->dpbdump.fb_c);
			vdec->dpbdump.fb_y = NULL;
			vdec->dpbdump.fb_c = NULL;
		}

		if (vdec->output_memory_format == LX_VDEC_MEMORY_FORMAT_RASTER)
		{
			/* ioremap for new framebuffer */
			logm_debug (vdec_dpbdump, "vdec:%p, raster memory,\n", vdec);

			vdec->dpbdump.fb_size_y = buf->stride*buf->height;
			vdec->dpbdump.fb_size_c = vdec->dpbdump.fb_size_y/2;
			vdec->dpbdump.fb_size = vdec->dpbdump.fb_size_y + vdec->dpbdump.fb_size_c;
			logm_debug (vdec_dpbdump, "vdec:%p, size_y %d, size_c %d\n", vdec,
					vdec->dpbdump.fb_size_y,
					vdec->dpbdump.fb_size_c);

			vdec->dpbdump.fb_y = vdec_remap (buf->addr_y, vdec->dpbdump.fb_size_y);
			vdec->dpbdump.fb_c = vdec_remap (buf->addr_cb, vdec->dpbdump.fb_size_c);
		}
		else
		{
			if (vdec->dpbdump.fb == NULL)
			{
				vdec->dpbdump.fb_size = buf->stride * buf->height * 3 / 2;
				logm_debug (vdec_dpbdump, "vdec:%p, allocate fb %d bytes\n",
						vdec, vdec->dpbdump.fb_size);

				vdec->dpbdump.fb = vmalloc (vdec->dpbdump.fb_size);
			}

			/* convert tiled memory to raster, NV12 ?? */
			VDC_GetFrame (vdec->vdc,
					buf->buffer_index, vdec->dpbdump.fb);
		}

		/* reset read pointer */
		vdec->dpbdump.fb_readed = 0;
	}

	/* copy real framebuffer to user space */
	if (vdec->output_memory_format == LX_VDEC_MEMORY_FORMAT_RASTER)
	{
		int count;

		if (vdec->dpbdump.fb_size_y > vdec->dpbdump.fb_readed)
		{
			count = vdec->dpbdump.fb_size_y - vdec->dpbdump.fb_readed;
			if (count > size)
				count = size;

			logm_debug (vdec_dpbdump, "vdec:%p, copy y %d bytes\n", vdec,
					count);
			ret = copy_to_user (data,
					vdec->dpbdump.fb_y+vdec->dpbdump.fb_readed,
					count);
			if (ret)
				ret = -EFAULT;
			else
			{
				copied += count;
				vdec->dpbdump.fb_readed += count;
			}
		}

		if (vdec->dpbdump.fb_size_y <= vdec->dpbdump.fb_readed)
		{
			int coffset;
			coffset = vdec->dpbdump.fb_readed - vdec->dpbdump.fb_size_y;
			count = vdec->dpbdump.fb_size_c - coffset;
			if (count > size-copied)
				count = size-copied;

			logm_debug (vdec_dpbdump, "vdec:%p, copy y %d bytes\n", vdec,
					count);
			ret = copy_to_user (data,
					vdec->dpbdump.fb_c+coffset,
					count);
			if (ret)
				ret = -EFAULT;
			else
			{
				copied += count;
				vdec->dpbdump.fb_readed += count;
			}
		}
	}
	else
	{
		int count;

		count = vdec->dpbdump.fb_size - vdec->dpbdump.fb_readed;
		if (count > size)
			count = size;

		logm_debug (vdec_dpbdump, "vdec:%p, copy %d bytes\n", vdec,
				count);
		if (count > 0)
		{
			ret = copy_to_user (data,
					vdec->dpbdump.fb+vdec->dpbdump.fb_readed,
					count);
			if (ret)
				ret = -EFAULT;
			else
			{
				copied += count;
				vdec->dpbdump.fb_readed += count;
			}
		}
	}

	logm_debug (vdec_dpbdump, "vdec:%p, copied %d, readed %d\n",
			vdec, copied,
			vdec->dpbdump.fb_readed);

	if (ret < 0)
	{
		logm_warning (vdec_dpbdump, "vdec:%p, failed\n", vdec);
		return ret;
	}

	return copied;
}

static struct file_operations proc_dpbdump_fops =
{
	.open = dpbdump_open,
	.read = dpbdump_read,
	.release = dpbdump_release,
};

struct proc_dir_entry *vdec_proc_root_entry;

static DEFINE_MUTEX (init_lock);
static struct proc_dir_entry *last_created;

int proc_make_instance (vdec_t *vdec)
{
	vdec->proc_entry = proc_mkdir (vdec->proc_name, vdec_proc_root_entry);
	vdec->proc_status = proc_create_data ("status", 0440,
			vdec->proc_entry, &proc_status_fops, vdec);
	vdec->proc_cpb_reader = proc_create_data ("cpb_reader", 0440,
			vdec->proc_entry, &proc_cpbdump_reader_fops, vdec);
	vdec->proc_cpb_writer = proc_create_data ("cpb_writer", 0440,
			vdec->proc_entry, &proc_cpbdump_writer_fops, vdec);
	vdec->proc_dpb = proc_create_data ("dpb", 0440,
			vdec->proc_entry, &proc_dpbdump_fops, vdec);

	mutex_lock (&init_lock);
	if (last_created)
		remove_proc_entry ("last_created", vdec_proc_root_entry);
	last_created = proc_symlink ("last_created", vdec_proc_root_entry, vdec->proc_name);
	mutex_unlock (&init_lock);

	return 0;
}

void proc_rm_instance (vdec_t *vdec)
{
	remove_proc_entry ("dpb", vdec->proc_entry);
	remove_proc_entry ("cpb_writer", vdec->proc_entry);
	remove_proc_entry ("cpb_reader", vdec->proc_entry);
	remove_proc_entry ("status", vdec->proc_entry);
	remove_proc_entry (vdec->proc_name, vdec_proc_root_entry);
}


int proc_init(void)
{
	vdec_proc_root_entry = proc_mkdir ("lxvdec", NULL);

	proc_create ("power", 0640, vdec_proc_root_entry, &power_fops);

#ifdef LOG_MASK
	proc_create ("logmask", 0640, vdec_proc_root_entry, &logmask_fops);

	// TOP
	_reg_log_cmd(vdec, 0);
	_reg_log_cmd(vdec_hal_top, 1);

	// PDEC Related
	_reg_log_cmd(vdec_hal_pdec, 3);
	_reg_log_cmd(vdec_ves, 4);

	// Feeder&VDC Related
	_reg_log_cmd(vdec_vdc, 5);
	_reg_log_cmd(vdc_proc, 6);

	// BODA
	_reg_log_cmd(vdec_vdu, 8);

	// GX
	_reg_log_cmd(vdec_gxref, 9);

	// VO
	_reg_log_cmd(vo_dispq, 12);
	_reg_log_cmd(vdec_cq, 13);
	_reg_log_cmd(vo, 14);

	// SYNC
	_reg_log_cmd(vdec_vdisp, 16);
	_reg_log_cmd(vdec_pts, 17);
	_reg_log_cmd(vdec_hal_deipc, 18);
	_reg_log_cmd(vdec_hal_vdo, 19);
	_reg_log_cmd(vdec_de_if, 20);
	_reg_log_cmd(vdec_rate, 21);
	_reg_log_cmd(vdec_vsync, 22);
	_reg_log_cmd(vdec_hal_vsync, 23);
	_reg_log_cmd(dif_hal, 24);
#endif

	return 0;
}

void proc_cleanup (void)
{
#ifdef LOG_MASK
	remove_proc_entry ("logmask", vdec_proc_root_entry);
#endif

	remove_proc_entry ("lxvdec", NULL);
}

