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
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#include "vds/vdisp_drv.h"

#include "output.h"

#include "log.h"

logm_define (vout, log_level_warning);


static int output_bind_pts_shift;
module_param (output_bind_pts_shift, int, 0644);

static struct
{
	int opened;

	int o;
	int port;
	int mirror;
	int bind;

	unsigned long stc;
	unsigned int fr_num;
	unsigned int fr_den;

	void (*unref) (void*, S_DISPQ_BUF_T *);
	void *unref_arg;
} p[
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
6
#else
4
#endif
];

static DEFINE_MUTEX (lock);

static struct proc_dir_entry *output_entry;


static int proc_output_show (struct seq_file *m, void *data)
{
	int a;

	for (a=0; a<ARRAY_SIZE (p); a++)
	{
#define seq_print_p(v,f)	seq_printf (m, "p[%d]."#v"="f"\n", a, p[a].v)
		seq_print_p (opened, "%d");
		seq_print_p (o, "%d");
		seq_print_p (port, "%d");
		seq_print_p (mirror, "%d");
		seq_print_p (bind, "%d");
	}

	return 0;
}

static int proc_output_open (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_open (file, proc_output_show, NULL);
	if (ret < 0)
		return ret;

	return ret;
}

static int proc_output_release (struct inode *inode, struct file *file)
{
	int ret;

	ret = single_release (inode, file);

	return ret;
}

static ssize_t proc_output_write (struct file *file,
		const char __user *buf, size_t size, loff_t *off)
{
	int n;
	char tmp[64], *t, *param[10];

	if (size == 0)
		return 0;

	/* we only allow single write */
	if (*off != 0)
		return -EINVAL;

	if (size > sizeof(tmp)-1)
	{
		logm_warning (vout, "too long string.\n");
		return -EINVAL;
	}
	if (copy_from_user (tmp, buf, size) > 0)
		return -EFAULT;

	tmp[size-1] = 0;
	t = tmp;
	n = 0;
	while( t != NULL && n < ARRAY_SIZE(param) )
		param[n++] = strsep(&t, " 	");
	logm_debug (vout, "n:%d\n", n);

	if (n >= 3 && !strcmp(param[0], "switch"))
	{
		int deid, port;

		deid = simple_strtol (param[1], NULL, 0);
		port = simple_strtol (param[2], NULL, 0);

		logm_noti (vout, "set deid%d to port%d\n", deid, port);
		output_set_output (deid, port);
	}
	else
		return -1;

	return size;
}

static struct file_operations proc_output_fops =
{
	.open = proc_output_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = proc_output_write,
	.release = proc_output_release,
};

extern struct proc_dir_entry *vo_proc_root_entry;
int output_init (void)
{

	output_entry = proc_create ("output", 0440,
			vo_proc_root_entry, &proc_output_fops);
	return 0;
}

int output_cleanup (void)
{
	remove_proc_entry ("status", vo_proc_root_entry);

	return 0;
}

int output_open (int deid, enum output_clock_source clock_src,
		int queue_size, int dual, void *status_cb,
		enum output_dual_t duallr)
{
	int o;
	E_VDISP_DST_T dst;
	int port;
	VDISP_CLK_SRC src;
	int a;
	VDISP_3D_DUAL_TYPE_E dualtype;

	logm_info (vout, "deid %d, src %d, qsize %d, dual %d, duallr %d\n",
			deid, clock_src, queue_size, dual, duallr);

	if (deid < 0 || deid >= ARRAY_SIZE(p))
	{
		logm_warning (vout, "unknown deid. %d\n", deid);
		goto failed;
	}

	mutex_lock (&lock);

	if (p[deid].opened)
	{
		logm_warning (vout, "deid%d, already opened\n", deid);
		goto failed;
	}

	if (p[deid].port != 0)
		port = p[deid].port;
	else
	{
		port = deid+1;

		logm_debug (vout, "deid%d, use default switch setting %d\n",
				deid, port);
	}

	for (a=0; a<ARRAY_SIZE (p); a++)
		if (p[a].opened && a != deid && port >= 0 && p[a].port == port)
			break;
	if (a != ARRAY_SIZE (p))
	{
		logm_warning (vout, "deid%d, port %d busy\n", deid, port);
		for (a=0; a<ARRAY_SIZE(p); a++)
			logm_warning (vout, "deid%d, "
					"opened%d, o%d, port%d\n",
					a,
					p[a].opened, p[a].o, p[a].port);

		goto failed;
	}

	switch (port)
	{
	case 1: dst = DE_IF_DST_DE_IF0; break;
	case 2: dst = DE_IF_DST_DE_IF1; break;
	case 3: dst = DE_IF_DST_DE_IF2; break;
	case 4: dst = DE_IF_DST_DE_IF3; break;
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
	case 5: dst = DE_IF_DST_DE_IF4; break;
	case 6: dst = DE_IF_DST_DE_IF5; break;
#endif
	case -1:dst = DE_IF_DST_NONE; break;
	default:
		logm_warning (vout, "deid%d, unknown port, %d.\n",
				deid, port);
		goto failed;
	}
	logm_info (vout, "deid%d, port %d, dst %d\n", deid, port, dst);

	switch (clock_src)
	{
	default:
		logm_warning (vout, "deid%d, unknown clock source %d\n",
				deid, clock_src);
		// fallthrough
	case output_clock_gstc: src = VDISP_CLK_GSTC; break;
	case output_clock_stc0: src = VDISP_CLK_PDEC0; break;
	case output_clock_stc1: src = VDISP_CLK_PDEC1; break;
	case output_clock_stc2: src = VDISP_CLK_PDEC2; break;
	case output_clock_stc3: src = VDISP_CLK_PDEC3; break;
	}

	logm_info (vout, "deid%d, clocksource %d\n",
			deid, src);

	switch (duallr)
	{
	default:
		dualtype = VDISP_3D_DUAL_NONE;
		break;
	case output_dual_left:
		dualtype = VDISP_3D_DUAL_LEFT;
		break;
	case output_dual_right:
		dualtype = VDISP_3D_DUAL_RIGHT;
		break;
	}
	logm_info (vout, "deid%d, dualtype%d\n", deid, dualtype);
	o = VDISP_Open (dst, src, 0, queue_size, dual,
			FALSE, status_cb, dualtype);
	if (o == 0xff)
	{
		logm_warning (vout, "deid%d, VDISP_Open() falied\n",
				deid);
		goto failed;
	}

	p[deid].opened = 1;
	p[deid].o = o;
	p[deid].port = port;
	p[deid].bind = -1;

	logm_info (vout, "opened %d\n", o);

	logm_info (vout, "deid%d, o%d mirror %d\n",
			deid, p[deid].o, p[deid].mirror);
	VDISP_SetDualOutput (p[deid].o, p[deid].mirror);

	mutex_unlock (&lock);

	return o;

failed:
	mutex_unlock (&lock);
	return -1;
}

int output_close (int o)
{
	int deid;

	logm_info (vout, "close %d\n", o);

	for (deid=0; deid<ARRAY_SIZE(p); deid++)
		if (p[deid].opened && p[deid].o == o)
			break;
	if (deid == ARRAY_SIZE(p))
	{
		logm_warning (vout, "unknown o%d\n", o);
		return -1;
	}

	mutex_lock (&lock);

	VDISP_Close (o);
	p[deid].opened = 0;

	mutex_unlock (&lock);

	if (p[deid].bind >= 0 && output_close (p[deid].bind) < 0)
		logm_warning (vout, "Oops\n");

	return 0;
}

int output_bind (int o, int b)
{
	int deid;

	for (deid=0; deid<ARRAY_SIZE(p); deid++)
		if (p[deid].opened && p[deid].o == o)
			break;
	if (deid == ARRAY_SIZE(p))
	{
		logm_warning (vout, "unknown o%d\n", o);
		return -1;
	}

	p[deid].bind = b;

	return 0;
}

int output_set_output (int deid, int port)
{
	int a;
	E_VDISP_DST_T dst;

	mutex_lock (&lock);
	if (deid < 0 || deid >= ARRAY_SIZE(p))
	{
		logm_warning (vout, "deid%d, unknown deid...\n", deid);
		goto failed;
	}

	if (port >= 0)
		port ++;

	logm_info (vout, "deid%d, connect to port%d\n",
			deid, port);

	switch (port)
	{
	default:
		logm_warning (vout,
				"deid%d, unknown port, %d\n",
				deid, port);
		goto failed;

	case -1:dst = DE_IF_DST_NONE; break;
	case 1: dst = DE_IF_DST_DE_IF0; break;
	case 2: dst = DE_IF_DST_DE_IF1; break;
	case 3: dst = DE_IF_DST_DE_IF2; break;
	case 4: dst = DE_IF_DST_DE_IF3; break;
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
	case 5: dst = DE_IF_DST_DE_IF4; break;
	case 6: dst = DE_IF_DST_DE_IF5; break;
#endif
	}

	for (a=0; a<ARRAY_SIZE (p); a++)
		if (p[a].opened && a != deid &&
				port >= 0 && p[a].port == port)
			break;
	if (a != ARRAY_SIZE (p))
	{
		logm_warning (vout, "deid%d, port %d busy\n", deid, port);
		for (a=0; a<ARRAY_SIZE(p); a++)
			logm_warning (vout, "deid%d, "
					"opened%d, o%d, port%d\n",
					a, p[a].opened, p[a].o, p[a].port);

		goto failed;
	}

	p[deid].port = port;

	if (p[deid].opened)
		VDISP_SetOutPort (p[deid].o, dst);

	mutex_unlock (&lock);

	return 0;

failed:
	mutex_unlock (&lock);
	return -1;
}

int output_opened (int deid)
{
	int ret;

	if (deid < 0 || deid >= ARRAY_SIZE(p))
	{
		logm_warning (vout, "deid%d, unknown deid...\n", deid);
		return 0;
	}

	ret = p[deid].opened;

	return ret;
}

int output_set_mirror (int deid, int on)
{
	logm_info (vout, "deid%d, mirror %d\n",
			deid, on);

	if (deid < 0 || deid >= ARRAY_SIZE(p))
	{
		logm_warning (vout, "deid%d, wrong deid.\n",
				deid);
		return -1;
	}

	mutex_lock (&lock);
	p[deid].mirror = on;

	if (p[deid].opened)
	{
		logm_info (vout, "deid%d, o%d mirror %d\n",
				deid, p[deid].o, on);

		VDISP_SetDualOutput (p[deid].o, on);
	}
	mutex_unlock (&lock);

	return 0;
}

long output_cmd (int o, enum output_cmds cmd, unsigned long arg)
{
	int deid;
	long ret = 0;
	int no_bind = 0;
	VDISP_CLK_SRC clocksrc;

	for (deid=0; deid<ARRAY_SIZE(p); deid++)
		if (p[deid].opened && p[deid].o == o)
			break;
	if (deid == ARRAY_SIZE(p))
	{
		logm_warning (vout, "unknown output %d, cmd %d, arg %lx\n",
				o, cmd, arg);
		return -1;
	}

	switch (cmd)
	{
	case output_cmd_basetime_stc:
		p[deid].stc = arg;
		break;

	case output_cmd_basetime_pts:
		VDISP_SetBaseTime (o, p[deid].stc, arg);
		break;

	case output_cmd_speed:
		VDISP_SetSpeed (o, arg);
		break;

	case output_cmd_start:
		VDISP_Start (o, arg);
		break;

	case output_cmd_stop:
		VDISP_Stop (o);
		break;

	case output_cmd_freeze:
		VDISP_Freeze (o, arg);
		break;

	case output_cmd_reset:
		VDISP_Reset (o);
		break;

	case output_cmd_flush:
		VDISP_Flush (o);
		break;

	case output_cmd_framerate_num:
		p[deid].fr_num = (int)arg;
		break;

	case output_cmd_framerate_den:
		VDISP_SetFrameRate (o, p[deid].fr_num, arg);
		break;

	case output_cmd_get_framerate_num:
		VDISP_GetRateInfo (o,
				&p[deid].fr_num, &p[deid].fr_den);
		ret = (long)p[deid].fr_num;
		no_bind = 1;
		break;

	case output_cmd_get_framerate_den:
		ret = p[deid].fr_den;
		no_bind = 1;
		break;

	case output_cmd_display_queued:
		ret = VDISP_GetDispQLen (o);
		no_bind = 1;
		break;

	case output_cmd_set_adaptive_queue:
		VDISP_SetMaxQlen (o, arg);
		break;

	case output_cmd_stc_discontinue:
		VDISP_SetStcDiscont (o, arg);
		break;

	case output_cmd_display_offset:
		VDISP_SetDisplayOffset (o, arg);
		break;

	case output_cmd_pts_match_mode:
		switch (arg)
		{
		default:
			// fallthrough
		case output_pts_enable:
			VDISP_SetMatchMode (o, PTS_MATCH_ENABLE);
			break;

		case output_pts_freerun_based_sync:
			VDISP_SetMatchMode (o, PTS_MATCH_FREERUN_BASED_SYNC);
			break;

		case output_pts_freerun_ignore_sync:
			VDISP_SetMatchMode (o, PTS_MATCH_FREERUN_IGNORE_SYNC);
			break;

		case output_pts_adaptive:
			VDISP_SetMatchMode (o, PTS_MATCH_ADAPTIVE_FREERUN);
			break;
		}
		break;

	case output_cmd_waitqueue:
		ret = (long)DISP_Q_GetWait (o);
		no_bind = 1;
		break;

	case output_cmd_unref_callback:
		p[deid].unref = (void*)arg;
		break;

	case output_cmd_unref_callback_arg:
		p[deid].unref_arg = (void*)arg;
		break;

	case output_cmd_display:
		VDISP_SendNewFrame (o, (void*)arg,
				p[deid].unref, p[deid].unref_arg);
		break;

	case output_cmd_set_clocksource:
		switch (arg)
		{
		default:
			logm_warning (vout, "o%d, unknown clock source %ld\n",
					o, arg);
			// fallthrough
		case output_clock_gstc: clocksrc = VDISP_CLK_GSTC; break;
		case output_clock_stc0: clocksrc = VDISP_CLK_PDEC0; break;
		case output_clock_stc1: clocksrc = VDISP_CLK_PDEC1; break;
		case output_clock_stc2: clocksrc = VDISP_CLK_PDEC2; break;
		case output_clock_stc3: clocksrc = VDISP_CLK_PDEC3; break;
		}

		VDISP_SetClkSrc (o, clocksrc);
		break;

	default:
		logm_warning (vout, "unknown command, %d, %ld\n", cmd, arg);
		return -1;
	}

	if (unlikely (p[deid].bind >= 0) && !no_bind)
	{
		logm_debug (vout, "do bind %d\n", p[deid].bind);

		if (cmd == output_cmd_display)
		{
			S_DISPQ_BUF_T *b = (S_DISPQ_BUF_T*)arg;

			logm_debug (vout, "shift pts %d\n", output_bind_pts_shift);
			b->ui32DTS += output_bind_pts_shift;
			if (b->ui32PTS != VDEC_UNKNOWN_PTS)
				b->ui32PTS += output_bind_pts_shift;
		}

		output_cmd (p[deid].bind, cmd, arg);
	}

	return ret;
}

