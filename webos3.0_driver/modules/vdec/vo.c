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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <linux/version.h>
#include <linux/platform_device.h>

#include "include/vo_kapi.h"

#include "hal/vdec_base.h"
#include "hal/top_hal_api.h"
#include "hal/vsync_hal_api.h"
#include "vds/vdisp_drv.h"
#include "vds/disp_q.h"

#include "vo.h"
#include "output.h"

#include "log.h"

logm_define (vo, log_level_warning);
logm_define (vo_mon, log_level_warning);

#define MAX_QUEUE_SIZE	32

static int major = 1818;
static int force_2d;
module_param_named (vo_major, major, int, 0444);
module_param_named (vo_force_2d, force_2d, int, 0644);

static int force_framerate_num;
static int force_framerate_den;
module_param_named (vo_force_framerate_num, force_framerate_num, int, 0644);
module_param_named (vo_force_framerate_den, force_framerate_den, int, 0644);

static int force_par_w;
static int force_par_h;
module_param_named (vo_force_par_w, force_par_w, int, 0644);
module_param_named (vo_force_par_h, force_par_h, int, 0644);

static int donot_update_info;
module_param_named (vo_donot_update_info, donot_update_info, int, 0644);

struct proc_dir_entry *vo_proc_root_entry;


static DEFINE_MUTEX (initialize_lock);
static struct vo *device_list;

static int mirror_mode;
static DEFINE_SPINLOCK (mirror_mode_lock);
static int mirror_de_id = -1;

struct vo_output_info output_info[
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
6
#else
4
#endif
];
static DEFINE_SPINLOCK (output_info_lock);

static int UHD_output;

static int vo_set_output_info_locked (int de_id, int width, int height,
		int framerate_num, int framerate_den,
		int par_w, int par_h,
		int progressive,
		enum vo_3d_type trid_type)
{
	struct vo_output_info *info;
	unsigned long flag;

	if (
			de_id < 0 ||
			de_id >= ARRAY_SIZE (output_info)
	   )
	{
		logm_warning (vo, "wrong de index %d\n", de_id);
		return -EINVAL;
	}

	if (unlikely(donot_update_info))
		return 0;

	if (framerate_den == 0)
	{
		logm_debug (vo, "wrong framerate %d/%d\n",
				framerate_num, framerate_den);
		framerate_num = 0;
		framerate_den = 1;
	}

	/* when par_w and par_h == -1, */
	if (par_w == par_h)
		par_w = par_h = 1;

	logm_debug (vo_mon, "deid%d, %dx%d, fr:%d/%d, par%dx%d, prog%d, 3d:%d\n",
			de_id, width, height,
			framerate_num, framerate_den,
			par_w, par_h, progressive,
			trid_type);

	info = output_info+de_id;

	info->width = width;
	info->height = height;
	info->framerate_num = framerate_num;
	info->framerate_den = framerate_den;
	info->progressive = progressive;
	info->par_w = par_w;
	info->par_h = par_h;
	info->trid_type = trid_type;

	spin_lock_irqsave (&mirror_mode_lock, flag);
	if (mirror_mode)
	{
		if (de_id == 0)
			mirror_de_id = 1;
		else
			mirror_de_id = 0;

		if (!output_opened (mirror_de_id))
		{
			logm_debug (vo_mon, "mirror to deid%d\n",
					mirror_de_id);

			info = output_info+mirror_de_id;

			info->width = width;
			info->height = height;
			info->framerate_num = framerate_num;
			info->framerate_den = framerate_den;
			info->progressive = progressive;
			info->par_w = par_w;
			info->par_h = par_h;
			info->trid_type = trid_type;
		}
		else
			logm_debug (vo_mon, "failed to mirror deid%d\n",
					mirror_de_id);
	}
	spin_unlock_irqrestore (&mirror_mode_lock, flag);

	return 0;
}

int vo_set_output_info (int de_id, int width, int height,
		int framerate_num, int framerate_den,
		int par_w, int par_h,
		int progressive,
		enum vo_3d_type trid_type)
{
	int ret;
	unsigned long flag;

	spin_lock_irqsave (&output_info_lock, flag);
	ret = vo_set_output_info_locked (de_id, width, height,
			framerate_num, framerate_den,
			par_w, par_h, progressive, trid_type);
	spin_unlock_irqrestore (&output_info_lock, flag);

	return ret;
}

static int running_info (struct vo *vo,
		int *framerate_num, int *framerate_den,
		int *width, int *height)
{
	S_VDISP_DISP_INFO_T info;

	memset (&info, 0, sizeof(info));

	VDISP_GetCurrentInfo (vo->de_id, &info);

	*width  = info.width;
	*height = info.height;

	*framerate_num = info.fr_num;
	*framerate_den = info.fr_den;

	return 0;
}

/* vo status proc */
static int status_show (struct seq_file *m, void *data)
{
	struct vo *vo = m->private;
	int q_size, q_head, q_tail;
	unsigned int q_last_written;
	unsigned long flag;
	int queued;

#define seq_print_vo(v,f)	seq_printf (m, #v"="f"\n", vo->v)
	seq_print_vo (proc_name, "%s");
	seq_print_vo (sync, "%d");
	seq_print_vo (sync2, "%d");
	seq_print_vo (open_dual, "%d");
	seq_print_vo (flushed, "%d");
	seq_print_vo (de_id, "%d");
	seq_print_vo (width, "%d");
	seq_print_vo (height, "%d");
	seq_print_vo (stride, "%d");
	seq_print_vo (framerate_es_num, "%d");
	seq_print_vo (framerate_es_den, "%d");
	seq_print_vo (framerate_container_num, "%d");
	seq_print_vo (framerate_container_den, "%d");
	seq_print_vo (force_framerate_num, "%d");
	seq_print_vo (force_framerate_den, "%d");
	seq_print_vo (frame_format, "%d");
	seq_print_vo (queue_size, "%d");
	seq_print_vo (inited, "%d");
	seq_print_vo (trid_type, "%d");
	seq_print_vo (speed, "%d");
	seq_print_vo (basetime.base_stc, "%d");
	seq_print_vo (basetime.base_pts, "%d");
	seq_print_vo (display_offset, "%d");
	seq_print_vo (keeping_sync, "%d");
	seq_print_vo (keeping_deid, "%d");
	seq_print_vo (flags, "0x%x");
	seq_print_vo (constant_delay, "%d");
	seq_print_vo (par_w, "%d");
	seq_print_vo (par_h, "%d");
	seq_print_vo (frames_written, "%d");
	seq_print_vo (frames_displayed, "%d");
	seq_print_vo (frames_dropped, "%d");

	spin_lock_irqsave (&vo->displayed_queue_lock, flag);
	q_size = vo->displayed_queue_size;
	q_head = vo->displayed_queue_head;
	q_tail = vo->displayed_queue_tail;
	q_last_written = vo->displayed_queue_last_written_id;
	spin_unlock_irqrestore (&vo->displayed_queue_lock, flag);

	seq_printf (m, "force_framerate_num=%d\n", force_framerate_num);
	seq_printf (m, "force_framerate_den=%d\n", force_framerate_den);

	if (vo->sync >= 0)
		queued = output_cmd (vo->sync, output_cmd_display_queued, 0);
	else
		queued = -1;
	seq_printf (m, "queued=%d\n", queued);
	seq_printf (m, "displayed_queue_size=%d\n", q_size);
	seq_printf (m, "displayed_queue_head=%d\n", q_head);
	seq_printf (m, "displayed_queue_tail=%d\n", q_tail);
	seq_printf (m, "displayed_queue_last_written_id=%d\n", q_last_written);

	seq_printf (m, "calling_read=%d\n", atomic_read (&vo->calling_read));
	seq_printf (m, "calling_ioctl=%d\n", atomic_read (&vo->calling_ioctl));
	seq_print_vo (last_ioctl_cmd, "0x%08x");
#undef seq_print_vo

	seq_printf (m, "\n");

	return 0;
}

static int status_open (struct inode *inode, struct file *file)
{
	struct vo *vo;
	int ret;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
	struct proc_dir_entry *dp;

	dp = PDE (inode);
	vo = dp->data;

#else
	vo = PDE_DATA(inode);
#endif
	ret = single_open (file, status_show, vo);
	if (ret < 0)
		return ret;

	return ret;
}

static int status_release (struct inode *inode, struct file *file)
{
	int ret;

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

static struct proc_dir_entry *last_created;
static int open_count;


vo_t *vo_open (void)
{
	vo_t *vo;

	vo = kcalloc (1, sizeof(*vo), GFP_KERNEL);;
	if (!vo)
		return NULL;

	/* set default parameter */
	vo->width = 1920;
	vo->height = 1080;
	vo->stride = 2048;
	vo->framerate_es_num = 0;
	vo->framerate_es_den = 1;
	vo->framerate_container_num = vo->framerate_es_num;
	vo->framerate_container_den = vo->framerate_es_den;
	vo->frame_format = DISPQ_FRAME_FORMAT_420;
	vo->queue_size = 9;
	vo->inited = 0;
	vo->sync = -1;
	vo->sync2 = -1;
	vo->open_dual = 0;
	vo->trid_type = vo_3d_type_none;
	vo->speed = 0x10000;
	vo->par_w = vo->par_h = 1;

	/* displayed queue */
	spin_lock_init (&vo->displayed_queue_lock);
	init_waitqueue_head (&vo->displayed_wait);
	vo->displayed_queue_head = 0;
	vo->displayed_queue_tail = 0;
	vo->displayed_queue_size = 64;
	vo->displayed_queue = kmalloc (
			sizeof(vo->displayed_queue[0])*vo->displayed_queue_size,
			GFP_KERNEL);

	/* make /proc entry */
	sprintf (vo->proc_name, "%08d_%p", open_count ++, vo);
	logm_noti (vo, "vo:%p, proc dir name %s\n",
			vo, vo->proc_name);
	vo->proc_entry = proc_mkdir (vo->proc_name, vo_proc_root_entry);
	vo->proc_status = proc_create_data ("status", 0440,
			vo->proc_entry, &proc_status_fops, vo);

	mutex_lock (&initialize_lock);
	if (last_created)
		remove_proc_entry ("last_created", vo_proc_root_entry);
	last_created = proc_symlink ("last_created", vo_proc_root_entry, vo->proc_name);

	/* register at the device list */
	vo->next = device_list;
	device_list = vo;
	mutex_unlock (&initialize_lock);

	logm_noti (vo, "vo:%p, opened\n", vo);

	return vo;
}
static int open (struct inode *inode, struct file *file)
{
	vo_t *vo;

	vo = vo_open ();
	if (!vo)
	{
		logm_warning (vo, "Oops\n");
		return -EIO;
	}

	file->private_data = vo;

	return 0;
}

int vo_release (vo_t *vo)
{
	logm_noti (vo, "vo:%p, releasing\n", vo);

	mutex_lock (&initialize_lock);

	/* remove /proc entry */
	remove_proc_entry ("status", vo->proc_entry );
	remove_proc_entry (vo->proc_name, vo_proc_root_entry );

	if (vo->sync >= 0)
	{
		logm_noti (vo, "vo:%p, close sync %d\n",
				vo, vo->sync);
		output_close (vo->sync);

		memset (&output_info[vo->de_id], 0,
				sizeof (struct vo_output_info));

		vo_set_output_info (vo->de_id, 0, 0, 0, 0,
				0, 0, 0, vo_3d_type_none);

		if (vo->flags & FLAG_UHD)
			UHD_output = 0;
	}
	if (vo->sync2 >= 0)
	{
		logm_noti (vo, "vo:%p, close sync2 %d\n",
				vo, vo->sync);
		output_close (vo->sync2);

		memset (&output_info[vo->de_id + 1], 0,
				sizeof (struct vo_output_info));

		vo_set_output_info (1, 0, 0, 0, 0,
				0, 0, 0, vo_3d_type_none);
	}

	/* remove from device list */
	if (device_list == vo)
		device_list = vo->next;
	else
	{
		struct vo *now;
		now = device_list;

		while (now->next != NULL && now->next != vo)
			now = now->next;

		if (now->next == vo)
			now->next = vo->next;
		else
			logm_error (vo, "vo:%p, Oops\n", vo);
	}

	kfree (vo->displayed_queue);

	logm_noti (vo, "vo:%p, released\n", vo);

	kfree (vo);
	mutex_unlock (&initialize_lock);

	return 0;
}
static int release (struct inode *inode, struct file *file)
{
	vo_t *vo = file->private_data;

	return vo_release (vo);
}


static int get_info (struct vo *vo, struct vo_displayed_info *info)
{
	int ret;

	ret = wait_event_interruptible (vo->displayed_wait,
			vo->displayed_queue_head != vo->displayed_queue_tail);
	if (ret < 0)
	{
		logm_debug (vo, "vo:%p, signaled\n", vo);
		return ret;
	}

	*info = vo->displayed_queue[vo->displayed_queue_tail];
	vo->displayed_queue_tail ++;
	vo->displayed_queue_tail %= vo->displayed_queue_size;
	logm_debug (vo, "vo:%p, %c%c%c%c, %lx. (%d %d)\n", vo,
			(info->id>>24)&0xff,
			(info->id>>16)&0xff,
			(info->id>>8)&0xff,
			(info->id>>0)&0xff,
			info->priv,
			vo->displayed_queue_tail,
			vo->displayed_queue_head);

	return 0;
}


static ssize_t vo_read (struct file *file, char __user *buf, size_t len, loff_t *off)
{
	struct vo *vo = (struct vo*)file->private_data;
	int ret = 0;
	int written = 0;
	int unit_size = sizeof (vo->displayed_queue[0]);
	struct vo_displayed_info info;

	logm_debug (vo, "vo:%p, len %d, head %d, tail %d\n",
			vo,
			len,
			vo->displayed_queue_head,
			vo->displayed_queue_tail
			);
	atomic_inc (&vo->calling_read);

	while (written+unit_size <= len)
	{
		ret = get_info (vo, &info);
		if (ret < 0)
			break;

		ret = copy_to_user (buf+written, &info,
				unit_size);
		if (ret)
		{
			ret = -EFAULT;
			logm_warning (vo, "vo:%p, oops\n", vo);
			break;
		}
		written += unit_size;

		logm_debug (vo, "vo:%p, written %d, ret %d\n", vo, written, ret);
	}

	atomic_dec (&vo->calling_read);

	logm_trace (vo, "vo:%p, written %d, ret %d\n", vo, written, ret);

	if (written == 0)
		return ret;

	return written;
}

static void push_info (struct vo *vo,
		unsigned int id, unsigned long priv)
{
	int filled;
	int head, tail;
	struct vo_displayed_info info;
	unsigned int size = sizeof (info);
	unsigned long flag;

	info.id = id;
	info.priv = priv;

	spin_lock_irqsave (&vo->displayed_queue_lock, flag);

	head = vo->displayed_queue_head;
	tail = vo->displayed_queue_tail;

	logm_debug (vo, "vo:%p, %c%c%c%c, %d bytes, %d %d\n", vo,
			(info.id>>24)&0xff,
			(info.id>>16)&0xff,
			(info.id>>8)&0xff,
			(info.id>>0)&0xff,
			size, tail, head);

	/* check buffer overflow */
	filled = head - tail;
	if (filled < 0)
		filled += vo->displayed_queue_size;

	if (filled + 1 >= vo->displayed_queue_size)
	{
		logm_noti (vo, "vo:%p, displayed queue full %d, %d, "
				"size %d, %c%c%c%c\n",
				vo, tail, head,
				size,
				(info.id>>24)&0xff,
				(info.id>>16)&0xff,
				(info.id>>8)&0xff,
				(info.id>>0)&0xff
				);
		goto failed;
	}

	if (info.id == VO_FCC('E','M','P','T') &&
			vo->displayed_queue_last_written_id == info.id)
	{
		logm_debug (vo, "vo:%p, skip empty info\n", vo);
		goto skip;
	}

	vo->displayed_queue_last_written_id = info.id;

	/* put data */
	memcpy (vo->displayed_queue+head, &info, size);
	head += 1;
	head %= vo->displayed_queue_size;
	vo->displayed_queue_head = head;

	wake_up (&vo->displayed_wait);

#if defined(CHIP_NAME_h15) && defined(__XTENSA__)
	{
		struct handle;
		extern int ipc_vo_update_poll (struct handle *h, vo_t *o,
				int update_in, int update_out);

		ipc_vo_update_poll (NULL, vo, 1, 1);
	}
#endif

failed:

skip:

	spin_unlock_irqrestore (&vo->displayed_queue_lock, flag);
	return;
}

static void displayed_callback (void *arg, S_DISPQ_BUF_T *buffer)
{
	struct vo *vo = (struct vo*)arg;

	logm_debug (vo, "vo:%p, %s y %07x, priv %08x, %dx%d, "
			"queue tail %d, queue head %d\n",
			vo,
			buffer->bDispResult?"displayed":"dropped  ",
			buffer->ui32Y_FrameBaseAddr,
			buffer->ui32UId,
			buffer->ui32PicWidth,
			buffer->ui32PicHeight,
			vo->displayed_queue_tail,
			vo->displayed_queue_head
		   );

	if (buffer->bDispResult)
	{
		push_info (vo, VO_FCC ('D','I','S','P'), buffer->ui32UId);
		vo->frames_displayed ++;
	}
	else
	{
		push_info (vo, VO_FCC ('D','R','O','P'), buffer->ui32UId);
		vo->frames_dropped ++;
	}
}

static void status_callback (unsigned char sync,
		VDISP_STATUS_TYPE type, unsigned int data)
{
	struct vo *vo;

	logm_debug (vo, "vo:<xxxxxx>, sync %d, type %d, data %08x\n",
			sync, type, data);
	vo = device_list;
	while (vo)
	{
		if (vo->sync == sync || vo->sync2 == sync)
			break;

		vo = vo->next;
	}

	if (vo == NULL)
	{
		logm_error (vo, "vo:<null>, Oops? unknown sync %d\n",
				sync);

		return;
	}

	logm_debug (vo, "vo:%p, status %d\n", vo, type);

	switch (type)
	{
	case VDISP_ST_EMPTY:
		if (!(vo->flags & FLAG_LOW_DELAY))
			push_info (vo, VO_FCC ('E','M','P','T'), 0);
		break;
	default:
		logm_trace (vo, "vo:%p, unhandled status type %d\n",
				vo, type);
		break;
	}
}

static int initialize_locked (struct vo *vo, int dual, int open_dual)
{
	enum output_pts_match match_mode;

	if (vo->flags&FLAG_LOW_DELAY)
	{
		logm_info (vo, "vo:%p, setting low delay\n", vo);
		match_mode = output_pts_freerun_ignore_sync;
	}
	else if (vo->flags&FLAG_FREERUN)
	{
		logm_info (vo, "vo:%p, setting freerun\n", vo);
		match_mode = output_pts_freerun_based_sync;
	}
	else if (vo->constant_delay > 0)
	{
		logm_info (vo, "vo:%p, set adaptive freerun\n", vo);
		match_mode = output_pts_adaptive;
	}
	else
	{
		match_mode = output_pts_enable;
	}
	vo->sync = output_open (vo->de_id, output_clock_gstc,
			MAX_QUEUE_SIZE, dual, status_callback,
			output_dual_none);
	if (vo->sync < 0)
	{
		logm_warning (vo, "vo:%p, sync open failed\n", vo);
		return -EINVAL;
	}
	output_cmd (vo->sync, output_cmd_pts_match_mode,
			match_mode);
	output_cmd (vo->sync, output_cmd_display_offset,
			vo->display_offset);
	output_cmd (vo->sync, output_cmd_unref_callback,
			(long)displayed_callback);
	output_cmd (vo->sync, output_cmd_unref_callback_arg,
			(long)vo);

	if (vo->force_framerate_num && vo->force_framerate_den)
	{
		output_cmd (vo->sync, output_cmd_framerate_num,
				vo->force_framerate_num);
		output_cmd (vo->sync, output_cmd_framerate_den,
				vo->force_framerate_den);
	}
	else if (force_framerate_num && force_framerate_den)
	{
		output_cmd (vo->sync, output_cmd_framerate_num,
				force_framerate_num);
		output_cmd (vo->sync, output_cmd_framerate_den,
				force_framerate_den);
	}
	else
	{
		output_cmd (vo->sync, output_cmd_framerate_num,
				vo->framerate_container_num);
		output_cmd (vo->sync, output_cmd_framerate_den,
				vo->framerate_container_den);
	}

	output_cmd (vo->sync, output_cmd_speed,
			vo->speed*1000/0x10000);
	output_cmd (vo->sync, output_cmd_basetime_stc,
			vo->basetime.base_stc);
	output_cmd (vo->sync, output_cmd_basetime_pts,
			vo->basetime.base_pts);
	output_cmd (vo->sync, output_cmd_start,
			FALSE);
	if (match_mode == output_pts_adaptive)
		output_cmd (vo->sync, output_cmd_set_adaptive_queue,
				vo->constant_delay);

	if (open_dual)
	{
		vo->sync2 = output_open (vo->de_id + 1, output_clock_gstc,
				MAX_QUEUE_SIZE, dual, status_callback,
				output_dual_none);
		if (vo->sync2 < 0)
		{
			logm_warning (vo, "vo:%p, sync2 open failed\n", vo);

			output_close (vo->sync);
			vo->sync = -1;
			return -EINVAL;
		}
		output_cmd (vo->sync2, output_cmd_pts_match_mode,
				match_mode);
		output_cmd (vo->sync2, output_cmd_display_offset,
				vo->display_offset);
		output_cmd (vo->sync2, output_cmd_unref_callback,
				(long)displayed_callback);
		output_cmd (vo->sync2, output_cmd_unref_callback_arg,
				(long)vo);

		if (vo->force_framerate_num && vo->force_framerate_den)
		{
			output_cmd (vo->sync2, output_cmd_framerate_num,
					vo->force_framerate_num);
			output_cmd (vo->sync2, output_cmd_framerate_den,
					vo->force_framerate_den);
		}
		else if (force_framerate_num && force_framerate_den)
		{
			output_cmd (vo->sync2, output_cmd_framerate_num,
					force_framerate_num);
			output_cmd (vo->sync2, output_cmd_framerate_den,
					force_framerate_den);
		}
		else
		{
			output_cmd (vo->sync2, output_cmd_framerate_num,
					vo->framerate_container_num);
			output_cmd (vo->sync2, output_cmd_framerate_den,
					vo->framerate_container_den);
		}

		output_cmd (vo->sync2, output_cmd_speed,
				vo->speed*1000/0x10000);
		output_cmd (vo->sync2, output_cmd_basetime_stc,
				vo->basetime.base_stc);
		output_cmd (vo->sync2, output_cmd_basetime_pts,
				vo->basetime.base_pts);
		output_cmd (vo->sync2, output_cmd_start,
				FALSE);
		if (match_mode == output_pts_adaptive)
			output_cmd (vo->sync2, output_cmd_set_adaptive_queue,
					vo->constant_delay);

		logm_info (vo, "vo:%p, sync: %d, sync2: %d\n",
				vo,
				vo->sync, vo->sync2);
	}

	return 0;
}


static int write_picture (struct vo *vo, struct vo_write *write)
{
	S_DISPQ_BUF_T picture, *pic;
	int dual = 0;
	wait_queue_head_t *wait;
	int sync, de_id;
	int ret;
	int keep_this_frame, update_keeping_frame;
	unsigned long flag;
	long long pts;

	/* 3D type 이 dual 인 경우 두가지 vo 드라이버 제어방법이 있을
	 * 수 있다.
	 *
	 * - 첫번째로 두개의 vo 드라이버를 열어 각각 de_id 를 다르게
	 *   설정 한 다음 picture를 쓰는 방법과
	 * - 하나의 vo 드라이버를 열어 multi_picture_index를 0 혹은
	 *   1로 설정해 왼쪽 혹은 오른쪽 그림을 구분하는 방법이다.
	 *
	 * 첫번째 방법은 3D type은 dual 이지만 하나의 sync module을
	 * 열어야 하며, 두번째 방법은 하나의 vo 드라이버에서 두개의
	 * sync module을 열어 한번에 제어해야 한다.
	 */
	if (vo->trid_type == vo_3d_type_dual ||
			vo->flags & FLAG_UHD)
		dual = 1;

	if (unlikely (!vo->inited))
	{
		int ret;

		if (dual && write->multi_picture_index >= 0)
			vo->open_dual = 1;

		if (vo->open_dual && (vo->de_id & 1))
		{
			logm_warning (vo, "vo:%p, "
					"dual open while de_id %d\n",
					vo,
					vo->de_id);
			vo->de_id = 0;
		}

		mutex_lock (&initialize_lock);
		ret = initialize_locked (vo, dual, vo->open_dual);
		mutex_unlock (&initialize_lock);

		if (ret < 0)
		{
			logm_warning (vo, "vo:%p, initialize failed\n",
					vo);
			return ret;
		}

		vo->inited = 1;

		if (vo->flags & FLAG_UHD)
			UHD_output = 1;
	}

	/* select writing sync module for dual 3D */
	if (!vo->open_dual || write->multi_picture_index <= 0)
	{
		sync = vo->sync;
		de_id = vo->de_id;

		logm_debug (vo, "vo:%p, de_id %d, 1st sync selected\n",
				vo, de_id);
	}
	else
	{
		sync = vo->sync2;
		de_id = 1;

		logm_debug (vo, "vo:%p, de_id %d, 2nd sync selected\n",
				vo, de_id);
	}

	/*
	 * frame sequencial이거나 dual stream(하나의 vo에서 두개의
	 * sync를 사용하는 경우)인 경우 왼쪽 프레임을 갓고 있다가
	 * 오른쪽 프레임이 들어오면 한번에 출력 대기 큐로 넘겨주도록
	 * 한다.
	 *
	 * 3D type이 frame sequencial이면 한 프레임의 레이턴시가
	 * 증가하므로 일단 예전의 동작과 같이 동작하도록 한다. 3D
	 * dual일 경우에만 적용하도록 한다.
	 */
	keep_this_frame = 0;
	update_keeping_frame = 0;
	pic = &picture;
	if (
			vo->open_dual
			/* || vo->trid_type == vo_3d_type_frame_alternation */
	   )
	{
		if (write->multi_picture_index == 0)
		{
			vo->keeping_sync = sync;
			vo->keeping_deid = de_id;
			pic = &vo->keeping_picture;
			keep_this_frame = 1;
			logm_debug (vo, "vo:%p, keep this frame.\n", vo);
		}
		else
		{
			update_keeping_frame = 1;
			logm_debug (vo, "vo:%p, update keeping frame with this one\n", vo);
		}
	}
	memset (pic, 0, sizeof (*pic));

	wait = (void*)output_cmd (sync, output_cmd_waitqueue, 0);

	if (vo->flushed)
		vo->flushed = 0;
	logm_debug (vo, "vo:%p, active number %ld\n", vo,
			output_cmd (sync,
				output_cmd_display_queued, 0));
	ret = wait_event_interruptible (*wait,
			output_cmd (sync,
				output_cmd_display_queued, 0)
			<vo->queue_size);
	if (ret)
	{
		logm_debug (vo, "vo:%p, signaled\n", vo);
		return ret;
	}
	if (vo->flushed)
	{
		logm_debug (vo, "vo:%p, flushed.\n", vo);
		push_info (vo, VO_FCC ('D','R','O','P'), (unsigned long)write->priv);
		vo->flushed = 0;
		return 0;
	}

	pic->pBufferPriv = vo->decoder_priv;//write->decoder_priv;
	pic->ui32Y_FrameBaseAddr = write->addr_y;
	pic->ui32C_FrameBaseAddr = write->addr_cb;
	pic->ui32Y_FrameBaseAddrBot = write->addr_y;
	pic->ui32C_FrameBaseAddrBot = write->addr_cb;
	pic->ui32Stride = vo->stride;
	pic->FrameFormat = vo->frame_format;

	if (vo->force_framerate_num && vo->force_framerate_den)
	{
		pic->ui32FrameRateRes = vo->force_framerate_num;
		pic->ui32FrameRateDiv = vo->force_framerate_den;
	}
	else if (force_framerate_num && force_framerate_den)
	{
		pic->ui32FrameRateRes = force_framerate_num;
		pic->ui32FrameRateDiv = force_framerate_den;
	}
	else
	{
		pic->ui32FrameRateRes = vo->framerate_es_num;
		pic->ui32FrameRateDiv = vo->framerate_es_den;
	}
	//pic->bVariableFrameRate;	// not used

	pic->ui32FrameIdx = 0;
	pic->ui32AspectRatio = 3;
	pic->ui32PicWidth = vo->width;
	pic->ui32PicHeight = vo->height;
	pic->ui32H_Offset =
		write->crop_left<<16 | write->crop_right;
	pic->ui32V_Offset =
		write->crop_top<<16 | write->crop_bottom;
	switch (write->interlace)
	{
	case vo_interlace_top_first:
		pic->ui32DisplayInfo =
			DISPQ_SCAN_TFF;
		break;
	case vo_interlace_bottom_first:
		pic->ui32DisplayInfo =
			DISPQ_SCAN_BFF;
		break;
	default:
	case vo_interlace_progressive:
		pic->ui32DisplayInfo =
			DISPQ_SCAN_PROG;
		break;
	}
	pts = write->pts;
	if (pts != -1)
	{
		pts *= 9;
		do_div (pts, 100000);
		pic->ui32PTS = pts;
	}
	else
		pic->ui32PTS = VDEC_UNKNOWN_PTS;
	//pic->ui32DTS;
	//pic->ui64TimeStamp;
	pic->ui32UId = (unsigned long)write->priv;
	pic->ui32DisplayPeriod = write->display_period;
	if (vo_3d_type_checkerboard <= vo->trid_type &&
			vo->trid_type <= vo_3d_type_fpa_end)
		pic->i32FramePackArrange = vo->trid_type;
	else
		pic->i32FramePackArrange = -2;
	if (dual)
		pic->ui32LR_Order = DISPQ_3D_FRAME_NONE;
	else
	{
		if (write->multi_picture_index < 0)
			pic->ui32LR_Order = DISPQ_3D_FRAME_NONE;
		else if (write->multi_picture_index == 0)
			pic->ui32LR_Order = DISPQ_3D_FRAME_LEFT;
		else
			pic->ui32LR_Order = DISPQ_3D_FRAME_RIGHT;
	}
	pic->ui16ParW = vo->par_w;
	pic->ui16ParH = vo->par_h;

	logm_debug (vo,  "vo:%p, "
			"bpriv:%p,y:%07lx,c:%07lx, "
			"%dx%d(%d),ts:%lld,pts:%08x,%d/%dfps, "
			"par:%ux%u,3d:%d,lr:%d,interlace:%d,priv:%p\n", vo,
			vo->decoder_priv,//write->decoder_priv,
			write->addr_y, write->addr_cb,

			vo->width, vo->height,
			vo->stride,
			write->pts, pic->ui32PTS,
			pic->ui32FrameRateRes, pic->ui32FrameRateDiv,

			vo->par_w, vo->par_h,
			vo->trid_type, write->multi_picture_index,
			write->interlace,
			write->priv
		   );
	logm_debug (vo, "vo:%p, crop(lrtb):%d,%d,%d,%d\n", vo,
			write->crop_left, write->crop_right,
			write->crop_top, write->crop_bottom);

	spin_lock_irqsave (&output_info_lock, flag);
	spin_lock (&VSync_HAL_IRQLock);
	if (update_keeping_frame)
	{
		logm_debug (vo, "vo:%p, update keeping frame\n", vo);

		if (vo->keeping_sync < 0 || vo->keeping_deid < 0)
		{
			logm_warning (vo, "vo:%p, no keeping frame. "
					"wrong multipicture index?\n",
					vo);
		}
		else
		{
			output_cmd (vo->keeping_sync, output_cmd_display,
					(long)&vo->keeping_picture);

			vo_set_output_info_locked (vo->keeping_deid,
					vo->width-write->crop_left-write->crop_right,
					vo->height-write->crop_top-write->crop_bottom,
					pic->ui32FrameRateRes,
					pic->ui32FrameRateDiv,
					vo->keeping_picture.ui16ParW,
					vo->keeping_picture.ui16ParH,
					write->interlace == vo_interlace_progressive,
					vo->trid_type);

			vo->keeping_sync = -1;
			vo->keeping_deid = -1;

			vo->frames_written ++;
		}
	}

	if (!keep_this_frame)
	{
		output_cmd (sync, output_cmd_display, (long)pic);

		vo_set_output_info_locked (de_id,
				vo->width-write->crop_left-write->crop_right,
				vo->height-write->crop_top-write->crop_bottom,
				pic->ui32FrameRateRes,
				pic->ui32FrameRateDiv,
				pic->ui16ParW,
				pic->ui16ParH,
				write->interlace == vo_interlace_progressive,
				vo->trid_type);

		vo->frames_written ++;
	}
	else
		logm_debug (vo, "vo:%p, keep this one\n", vo);
	spin_unlock (&VSync_HAL_IRQLock);
	spin_unlock_irqrestore (&output_info_lock, flag);

	return 0;
}

static ssize_t vo_write (struct file *file,
		const char __user *buf, size_t len, loff_t *off)
{
	struct vo *vo = file->private_data;
	struct vo_write write;
	int ret;

	logm_trace (vo, "vo:%p, size %d\n", vo, len);

	if (len != sizeof (struct vo_write))
	{
		logm_warning (vo, "vo:%p, wrong size. %d\n",
				vo, len);
		return -EINVAL;
	}

	ret = copy_from_user (&write, buf, sizeof(write));
	if (ret)
	{
		logm_error (vo, "vo:%p, copy_from_user failed\n", vo);
		return -EFAULT;
	}

	write_picture (vo, &write);

	logm_trace (vo, "vo:%p, written %d\n", vo, len);
	return len;
}

static unsigned int vo_poll (struct file *file, struct poll_table_struct *pt)
{
	struct vo *vo = file->private_data;
	wait_queue_head_t *wait;
	unsigned int ret = 0;
	int display_queue_num;

	logm_trace (vo, "vo:%p, poll..\n", vo);

	if (unlikely (vo->sync < 0))
	{
		logm_trace (vo, "vo:%p, no sync\n", vo);
		return POLLERR;
	}

	wait = (void*)output_cmd (vo->sync, output_cmd_waitqueue, 0);
	display_queue_num = output_cmd (vo->sync,
			output_cmd_display_queued, 0);

	poll_wait (file, &vo->displayed_wait, pt);
	poll_wait (file, wait, pt);

	if (vo->displayed_queue_head != vo->displayed_queue_tail)
		ret |= POLLIN | POLLRDNORM;

	if (display_queue_num < vo->queue_size)
		ret |= POLLOUT | POLLWRNORM;

	if (
			vo->displayed_queue_head == vo->displayed_queue_tail &&
			display_queue_num == 0
	   )
		ret |= POLLERR;

	logm_trace (vo, "vo:%p, poll.. %x\n", vo, ret);
	return ret;
}

long vo_ioctl (struct vo *vo,
		unsigned int cmd, unsigned long arg,
		int from_user)
{
	union
	{
		struct vo_framebuffer_info fb_info;
		struct vo_basetime basetime;
		struct vo_stc stc;
		unsigned long tile_base;
		int queue_size;
		struct vo_output_info output_info;
		struct vo_write write;
		struct vo_displayed_info info;
	} a;
	long ret = -ENOIOCTLCMD;
	wait_queue_head_t *wait;

	if (cmd != VO_GET_OUTPUT_INFO &&
			cmd != VO_GET_OUTPUT_INFO2)
		logm_trace (vo, "vo:%p, cmd = %08x (cmd_idx=%d)\n", vo, cmd, _IOC_NR(cmd));

	if (_IOC_TYPE(cmd) != VO_TYPE)
	{
		logm_warning (vo, "vo:%p, invalid magic. magic=0x%02x\n", vo,
				_IOC_TYPE(cmd) );
		return -ENOIOCTLCMD;
	}
	if (_IOC_DIR(cmd) & _IOC_WRITE)
	{
		if (from_user)
		{
			int r;

			r = copy_from_user (&a, (void*)arg, _IOC_SIZE(cmd));
			if (r)
			{
				logm_error (vo, "vo:%p, copy_from_user failed\n", vo);
				return -EFAULT;
			}
		}
		else
			memcpy (&a, (void*)arg, _IOC_SIZE(cmd));
	}

	atomic_inc (&vo->calling_ioctl);
	vo->last_ioctl_cmd = cmd;

	switch (cmd)
	{
	case VO_SET_DE_ID:
		if (arg >= ARRAY_SIZE (output_info))
		{
			logm_warning (vo, "vo:%p, unknown de id %ld\n", vo, arg);
			ret = -EINVAL;
			break;
		}

		vo->de_id = arg;

		logm_noti (vo, "vo:%p, set de id %d\n", vo, vo->de_id);
		ret = 0;
		break;

	case VO_FLUSH:
		if (vo->sync < 0)
		{
			logm_debug (vo, "vo:%p, no device while flushing\n", vo);
			ret = -EINVAL;
			break;
		}

		logm_noti (vo, "vo:%p, flush\n", vo);
		output_cmd (vo->sync, output_cmd_flush, 0);
		vo->flushed = 1;
		wait = (void*)output_cmd (vo->sync, output_cmd_waitqueue, 0);

		wake_up (&vo->displayed_wait);
		wake_up (wait);

		ret = 0;
		break;

	case VO_GET_STC:
		a.stc.stc = TOP_HAL_GetGSTCC ();
		a.stc.mask = 0xffffffff;

		logm_debug (vo, "vo:%p, stc %08x\n", vo, a.stc.stc);
		ret = 0;
		break;

	case VO_SET_BASETIME:
		logm_noti (vo, "vo:%p, set base stc %08x, pts %08x. STC diff %d(ms)\n", vo,
				a.basetime.base_stc,
				a.basetime.base_pts,
				((int)a.basetime.base_stc-(int)vo->basetime.base_stc)/90);
		vo->basetime = a.basetime;

		if (vo->sync >= 0)
		{
			output_cmd (vo->sync, output_cmd_basetime_stc,
					vo->basetime.base_stc);
			output_cmd (vo->sync, output_cmd_basetime_pts,
					vo->basetime.base_pts);
		}
		ret = 0;
		break;

	case VO_SET_DISPLAY_POSITION:
		break;

	case VO_SET_FRAMEBUFFER_INFO:
		vo->width  = a.fb_info.width;
		vo->height = a.fb_info.height;
		vo->stride = a.fb_info.stride;

		logm_debug (vo, "vo:%p, set framebuffer info %dx%d %d\n", vo,
				vo->width, vo->height, vo->stride);
		ret = 0;
		break;

	case VO_SET_INTERLACE:
		logm_warning (vo, "vo:%p, not supported anymore\n", vo);
		break;

	case VO_SET_COLOR_SUBSAMPLE:
		if (arg == 0x00020002)
			vo->frame_format = DISPQ_FRAME_FORMAT_420;
		else if (arg == 0x00010002)
			vo->frame_format = DISPQ_FRAME_FORMAT_422;
		else
		{
			logm_warning (vo, "vo:%p, not supported subsampling. %ldx%ld\n", vo,
					arg & 0xffff,
					arg>>16);
			break;
		}

		logm_debug (vo, "vo:%p, set color subsample %ldx%ld\n", vo,
				arg & 0xffff,
				arg >> 16);
		ret = 0;
		break;

	case VO_SET_FRAMERATE_ES:
		vo->framerate_es_num = arg >> 16;
		vo->framerate_es_den = arg & 0xffff;

		logm_debug (vo, "vo:%p, set es framerate %d/%d\n", vo,
				vo->framerate_es_num, vo->framerate_es_den);

		ret = 0;
		break;

	case VO_SET_FRAMERATE_CONTAINER:
		vo->framerate_container_num = arg >> 16;
		vo->framerate_container_den = arg & 0xffff;

		logm_debug (vo, "vo:%p, set container framerate %d/%d\n", vo,
				vo->framerate_container_num,
				vo->framerate_container_den);

		ret = 0;
		break;

	case VO_SET_QUEUE_SIZE:
		if (arg < MAX_QUEUE_SIZE)
		{
			vo->queue_size = arg;
			logm_debug (vo, "vo:%p, set queue size %d\n", vo,
					vo->queue_size);
			ret = 0;
		}
		else
		{
			logm_warning (vo, "vo:%p, too big queue size %d\n", vo,
					(int)arg);
			ret = -EINVAL;
		}

		break;

	case VO_GET_DISPLAYING_QUEUE_SIZE:
		if (vo->sync >= 0)
			a.queue_size = output_cmd (vo->sync,
					output_cmd_display_queued, 0);
		else
			a.queue_size = 0;
		logm_trace (vo, "vo:%p, get displaying queue size %d\n", vo,
				a.queue_size);
		ret = 0;
		break;

	case VO_GET_DISPLAYED_QUEUE_SIZE:
		{
			int head, tail;

			head = vo->displayed_queue_head;
			tail = vo->displayed_queue_tail;

			if (head >= tail)
				a.queue_size = head-tail;
			else
				a.queue_size = vo->displayed_queue_size
					- tail + head;

			logm_trace (vo, "vo:%p, get displayed queue size %d\n",
					vo, a.queue_size);

			ret = 0;
		}
		break;

	case VO_SET_3D:
		logm_debug (vo, "vo:%p, set 3d type %ld\n", vo, arg);
		if (arg > vo_3d_type_none)
		{
			logm_warning (vo, "vo:%p, unknown 3d type %08lx\n", vo, arg);
			ret = -EINVAL;
			break;
		}
		if (force_2d)
		{
			logm_debug (vo, "vo:%p, force set 2D\n", vo);
			vo->trid_type = vo_3d_type_none;
		}
		else
			vo->trid_type = arg;

		ret = 0;
		break;

	case VO_SET_TILE_BASE:
		vo->decoder_priv = (void*)a.tile_base;

		logm_debug (vo, "vo:%p, buffer priv %p\n", vo,
				vo->decoder_priv);

		ret = 0;
		break;

	case VO_SET_SPEED:
		vo->speed = arg;
		logm_noti (vo, "vo:%p, set speed %lx\n", vo, arg);
		if (vo->sync >= 0)
			output_cmd (vo->sync, output_cmd_speed,
					vo->speed*1000/0x10000);
		ret = 0;
		break;

	case VO_GET_OUTPUT_INFO:
	case VO_GET_OUTPUT_INFO2:
		{
			int fr_num, fr_den;
			int width, height;
			unsigned long flag;

			spin_lock_irqsave (&output_info_lock, flag);
			a.output_info = output_info[vo->de_id];
			spin_unlock_irqrestore (&output_info_lock, flag);

			running_info (vo, &fr_num, &fr_den,
					&width, &height);

			if (!donot_update_info)
			{
				if (fr_den)
				{
					a.output_info.framerate_num = fr_num;
					a.output_info.framerate_den = fr_den;
				}

				if (cmd == VO_GET_OUTPUT_INFO2 &&
						width >= 0 && height >= 0)
				{
					a.output_info.width  = width;
					a.output_info.height = height;
				}
			}

			if (UHD_output)
				a.output_info.width *= 2;

			logm_debug (vo_mon, "output info %dx%d(%dx%d) %d/%d, prog%d, 3d:%d\n",
					a.output_info.width,
					a.output_info.height,
					a.output_info.par_w,
					a.output_info.par_h,
					a.output_info.framerate_num,
					a.output_info.framerate_den,
					a.output_info.progressive,
					a.output_info.trid_type);

			ret = 0;
		}
		break;

	case VO_SET_DISPLAY_OFFSET:
		if (vo->sync >= 0)
		{
			logm_warning (vo, "vo:%p, set display offset "
					"after initialization\n", vo);
			ret = -EBUSY;
			break;
		}

		vo->display_offset = arg;
		logm_noti (vo, "vo:%p, set display offset %d\n",
				vo, vo->display_offset);
		ret = 0;
		break;

	case VO_SET_PAR:
		vo->par_w = arg>>16;
		vo->par_h = arg&0xffff;
		logm_noti (vo, "vo:%p, set par %dx%d\n", vo,
				vo->par_w, vo->par_h);

		if (force_par_w > 0 && force_par_h > 0)
		{
			vo->par_w = force_par_w;
			vo->par_h = force_par_h;
			logm_noti (vo, "vo:%p, fixup par %dx%d\n", vo,
					vo->par_w, vo->par_h);
		}

		if (vo->par_w <= 0 || vo->par_h <= 0)
		{
			logm_warning (vo, "vo:%p, wrong par %dx%d\n", vo,
					vo->par_w, vo->par_h);
			vo->par_w = vo->par_h = 1;
		}

		ret = 0;
		break;

	case VO_SET_LOW_LATENCY:
		if (vo->sync >= 0)
		{
			logm_warning (vo, "vo:%p, we have sync already\n", vo);
			ret = -EINVAL;
			break;
		}
		if (!!arg)
			vo->flags |= FLAG_LOW_DELAY;
		else
			vo->flags &= ~FLAG_LOW_DELAY;

		if (vo->height <= 720)
		{
			logm_debug (vo, "vo:%p, set framerate 60\n", vo);
			vo->force_framerate_num = 60;
			vo->force_framerate_den = 1;
		}
		logm_debug (vo, "vo:%p, set low delay %ld, flags %08x\n",
				vo, arg, vo->flags);

		ret = 0;
		break;

	case VO_SET_UHD:
		if (vo->sync >= 0)
		{
			logm_warning (vo, "vo:%p, we have sync already\n", vo);
			ret = -EINVAL;
			break;
		}
		if (!!arg)
			vo->flags |= FLAG_UHD;
		else
			vo->flags &= ~FLAG_UHD;

		logm_debug (vo, "vo:%p, set UHD %ld, flags %08x\n",
				vo, arg, vo->flags);

		ret = 0;
		break;

	case VO_SET_FREERUN:
		if (!!arg)
			vo->flags |= FLAG_FREERUN;
		else
			vo->flags &= ~FLAG_FREERUN;

		logm_info (vo, "vo:%p, set freerun %ld, flags %08x\n",
				vo, arg, vo->flags);
		if (vo->sync >= 0)
		{
			enum output_pts_match match_mode;

			if (!!arg)
				match_mode = output_pts_freerun_based_sync;
			else
				/* change to PTS_ENABLE??? */
				match_mode = output_pts_enable;
			logm_info (vo, "vo:%p, change output config. %d\n",
					vo, match_mode);

			output_cmd (vo->sync, output_cmd_pts_match_mode,
					match_mode);
			if (vo->sync2 >= 0)
				output_cmd (vo->sync2, output_cmd_pts_match_mode,
						match_mode);
		}

		ret = 0;
		break;

	case VO_SET_CONSTANT_DELAY:
		if (arg > adaptive_queue_max)
		{
			logm_warning (vo, "vo:%p, too big adaptive queue size, %ld. limit to %d\n",
					vo, arg, adaptive_queue_max);
			arg = adaptive_queue_max;
		}

		vo->constant_delay = arg;
		logm_info (vo, "vo:%p, set constant delay %d\n", vo,
				vo->constant_delay);

		if (vo->sync >= 0)
			ret = output_cmd (vo->sync,
					output_cmd_set_adaptive_queue, vo->constant_delay);
		else
		{
			logm_info (vo, "vo:%p, no sync.\n", vo);
			ret = 0;
		}

		break;

	case VO_SET_MIRROR_MODE:
		logm_noti (vo, "vo:%p, set mirror mode %d\n",
				vo, !!arg);

		output_set_mirror (vo->de_id, !!arg);

		{
			unsigned long flag;
			int m_deid;

			spin_lock_irqsave (&mirror_mode_lock, flag);
			mirror_mode = !!arg;
			m_deid = mirror_de_id;
			if (!mirror_mode)
				mirror_de_id = -1;
			spin_unlock_irqrestore (&mirror_mode_lock, flag);

			/* cleanup mirror */
			if (!mirror_mode && m_deid >= 0)
				vo_set_output_info (m_deid, 0, 0, 0, 0,
						0, 0, 0, vo_3d_type_none);
		}
		ret = 0;
		break;

	case VO_REPEAT_FRAME:
		/* repeat frames for paused output */
		logm_info (vo, "vo:%p, repeat frames\n", vo);

		VDISP_RepeatFrame (0, (int)arg);
		VDISP_RepeatFrame (1, (int)arg);
		ret = 0;
		break;

	case VO_SET_OUTPUT_PORT:
		logm_info (vo, "vo:%p, set output port, %ld\n", vo, arg);
		ret = output_set_output (vo->de_id, arg);
		break;

	case VO_WRITE_PICTURE:
		ret = write_picture (vo, &a.write);
		break;

	case VO_GET_INFO:
		ret = get_info (vo, &a.info);
		break;

	default:
		logm_warning (vo, "vo:%p, unknown ioctl %08x\n", vo, cmd);
		break;
	}

	if(ret < 0)
	{
		logm_warning (vo, "vo:%p, ioctl failed (cmd:%d, ret:%ld)\n",
				vo,
				_IOC_NR(cmd),
				ret);
	}

	atomic_dec (&vo->calling_ioctl);

	if (ret >= 0 && _IOC_DIR(cmd) & _IOC_READ)
	{
		if (from_user)
		{
			int r;

			r = copy_to_user ((void*)arg, &a, _IOC_SIZE(cmd));
			if (r)
			{
				logm_error (vo, "vo:%p, copy_to_user failed\n", vo);
				return -EFAULT;
			}
		}
		else
			memcpy ((void*)arg, &a, _IOC_SIZE(cmd));
	}

	if (cmd != VO_GET_OUTPUT_INFO &&
			cmd != VO_GET_OUTPUT_INFO2)
		logm_trace (vo, "vo:%p, cmd = %08x (cmd_idx=%d)\n",
				vo, cmd, _IOC_NR(cmd));
	return ret;
}
static long ioctl (struct file *file,
		unsigned int cmd, unsigned long arg)
{
	vo_t *vo = file->private_data;

	return vo_ioctl (vo, cmd, arg, 1);
}


static struct file_operations vo_fops =
{
	.open		= open,
	.release	= release,
	.read		= vo_read,
	.write		= vo_write,
	.unlocked_ioctl	= ioctl,
	.poll		= vo_poll,
};

static int probe (struct platform_device *dev)
{
	int ret;

	logm_info (vo, "probe...\n");

	ret = register_chrdev (major, "vo", &vo_fops);
	if (ret < 0)
	{
		logm_error (vo, "register_chrdev failed\n");
		return ret;
	}
	if (major == 0)
	{
		major = ret;
		logm_debug (vo, "major %d\n", major);
	}

	vo_proc_root_entry = proc_mkdir ("lxvo", NULL);

	DISP_Q_Init ();
	VDISP_Init ();

	output_init ();

	return 0;
}

static int remove (struct platform_device *dev)
{
	logm_info (vo, "remove...\n");

	remove_proc_entry ("lxvo", NULL);

	free_irq (VDEC_IRQ_NUM_SYNC, dev);

	unregister_chrdev (major, "vo");

	return 0;
}

static struct platform_driver platform_driver =
{
	.probe = probe,
	.remove = remove,
	.driver =
	{
		.name = "vo",
	},
};

static struct platform_device platform_device =
{
	.name = "vo",
	.id = -1,
};

int vo_init (void)
{
	int ret;
	static struct class* class = NULL;

	logm_info (vo, "init..\n");

	class = class_create(THIS_MODULE, "vo" );
	device_create (class, NULL, MKDEV(major, 0), NULL, "vo");

	ret = platform_device_register (&platform_device);
	if (ret)
	{
		logm_error (vo, "platform_device_register failed, %d\n", ret);
		return ret;
	}

	ret = platform_driver_register (&platform_driver);
	if (ret)
	{
		logm_error (vo, "platform_driver_register failed, %d\n", ret);
		return ret;
	}

	return 0;
}

void vo_exit (void)
{
	logm_info (vo, "exit..\n");

	platform_device_unregister (&platform_device);
	platform_driver_unregister (&platform_driver);
}

MODULE_LICENSE ("GPL");
//module_init (vo_init);
//module_exit (vo_exit);

