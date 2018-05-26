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
#ifndef _OUTPUT_H
#define _OUTPUT_H

enum output_pts_match
{
	output_pts_enable,
	output_pts_freerun_based_sync,
	output_pts_freerun_ignore_sync,
	output_pts_adaptive,
};

enum output_clock_source
{
	output_clock_stc0,
	output_clock_stc1,
	output_clock_stc2,
	output_clock_stc3,
	output_clock_gstc,
};

enum output_cmds
{
	output_cmd_basetime_stc,
	output_cmd_basetime_pts,	// should be after _stc

	output_cmd_speed,
	output_cmd_start,
	output_cmd_stop,
	output_cmd_freeze,
	output_cmd_reset,
	output_cmd_flush,

	output_cmd_framerate_num,
	output_cmd_framerate_den,	// should be after _num

	output_cmd_get_framerate_num,	// should be first
	output_cmd_get_framerate_den,

	output_cmd_display_queued,
	output_cmd_set_adaptive_queue,
	output_cmd_stc_discontinue,
	output_cmd_display_offset,
	output_cmd_pts_match_mode,
	output_cmd_waitqueue,
	output_cmd_set_clocksource,

	output_cmd_unref_callback,
	output_cmd_unref_callback_arg,

	output_cmd_display,
};

enum output_dual_t
{
	output_dual_none,
	output_dual_left,
	output_dual_right,
};

int output_init (void);
int output_cleanup (void);
int output_open (int deid, enum output_clock_source clock_src,
		int queue_size, int dual, void *status_cb,
		enum output_dual_t duallr);
int output_close (int o);
int output_bind (int o, int b);
int output_set_output (int deid, int port);
int output_opened (int deid);
int output_set_mirror (int deid, int on);
long output_cmd (int o, enum output_cmds cmd, unsigned long arg);

#endif
