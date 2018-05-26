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
#ifndef _VO_KAPI_H
#define _VO_KAPI_H

#include <linux/ioctl.h>

#define VO_TYPE	'v'

#define VO_GET_STC			_IOR(VO_TYPE,1, struct vo_stc)
#define VO_SET_DISPLAY_POSITION		_IO (VO_TYPE,2)
#define VO_SET_FRAMEBUFFER_INFO		_IOW(VO_TYPE,3, struct vo_framebuffer_info)
#define VO_SET_INTERLACE		_IO (VO_TYPE,4)
#define VO_SET_FRAMERATE_ES		_IO (VO_TYPE,5)
#define VO_SET_QUEUE_SIZE		_IO (VO_TYPE,6)
#define VO_SET_3D			_IO (VO_TYPE,7)
#define VO_SET_TILE_BASE		_IOW(VO_TYPE,8, unsigned long)
#define VO_GET_DISPLAYING_QUEUE_SIZE	_IOR(VO_TYPE,9, int)
#define VO_GET_DISPLAYED_QUEUE_SIZE	_IOR(VO_TYPE,10, int)
#define VO_SET_COLOR_SUBSAMPLE		_IO (VO_TYPE,11)
#define VO_SET_SPEED			_IO (VO_TYPE,12)
#define VO_SET_BASETIME			_IOW(VO_TYPE,13, struct vo_basetime)
#define VO_FLUSH			_IO (VO_TYPE,14)
#define VO_SET_FRAMERATE_CONTAINER	_IO (VO_TYPE,15)
#define VO_SET_DE_ID			_IO (VO_TYPE,16)
#define VO_GET_OUTPUT_INFO		_IOR(VO_TYPE,17, struct vo_output_info)
#define VO_SET_DISPLAY_OFFSET		_IO (VO_TYPE,18)
#define VO_SET_PAR			_IO (VO_TYPE,19)
#define VO_SET_LOW_LATENCY		_IO (VO_TYPE,20)
#define VO_SET_MIRROR_MODE		_IO (VO_TYPE,21)
#define VO_REPEAT_FRAME			_IO (VO_TYPE,22)
#define VO_GET_OUTPUT_INFO2		_IOR(VO_TYPE,23, struct vo_output_info)
#define VO_SET_CONSTANT_DELAY		_IO (VO_TYPE,24)
#define VO_SET_UHD			_IO (VO_TYPE,25)
#define VO_SET_OUTPUT_PORT		_IO (VO_TYPE,26)
#define VO_SET_FREERUN			_IO (VO_TYPE,27)
#define VO_WRITE_PICTURE		_IOW(VO_TYPE,28, struct vo_write)
#define VO_GET_INFO			_IOR(VO_TYPE,29, struct vo_displayed_info)

#define VO_SET_FRAMERATE		VO_SET_FRAMERATE_ES

enum vo_interlace
{
	vo_interlace_progressive,
	vo_interlace_top_first,
	vo_interlace_bottom_first,
};

/* contents of write() system call */
struct vo_write
{
	/* user layer private data to identify the buffer */
	void *priv;

	/* PTS. -1 for unavailable */
	long long pts;

	/* can be droped if queue full or old PTS */
	/* deprecated */
	int droppable;

	/* crop screen */
	int crop_left, crop_right;
	int crop_top, crop_bottom;

	enum vo_interlace interlace;

	/*
	 * progressive : 1
	 * interlace   : 2
	 * interlace(repeat first field) : 3
	 */
	int display_period;

	/* picture index for multi picture. ex. 3D image
	 *
	 * -1: 2d image
	 * 0 : left image
	 * 1 : right image
	 */
	int multi_picture_index;

	/* framebuffer address */
	unsigned long addr_y;
	unsigned long addr_cb;
	unsigned long addr_cr;

	void *decoder_priv;
};

/* result of read() system call */
#define VO_FCC(a,b,c,d)		(((a)<<24)|((b)<<16)|((c)<<8)|((d)<<0))
struct vo_displayed_info
{
	unsigned int id;
	unsigned long priv;
};

struct vo_framebuffer_info
{
	int width;
	int height;
	int stride;
};

struct vo_basetime
{
	unsigned int base_stc;
	unsigned int base_pts;
};

struct vo_stc
{
	unsigned int stc;
	unsigned int mask;
};

enum vo_3d_type
{
	/* from Frame Packing Arrangement(FPA) */
	vo_3d_type_checkerboard = 0,	// pixels are alternatively from L and R
	vo_3d_type_column_alternation,	// L and R are interlaced by column
	vo_3d_type_row_alternation,	// L and R are interlaced by row
	vo_3d_type_side_by_side,	// L is on the left, R on the right
	vo_3d_type_top_bottom,		// L is on top, R on bottom
	vo_3d_type_frame_alternation,	// one view per frame
	vo_3d_type_fpa_end = vo_3d_type_frame_alternation,

	/* user defined */
	vo_3d_type_dual,
	vo_3d_type_side_by_side_rl,	// L is on the right, R on the left
	vo_3d_type_bottom_top,		// L is on bottom, R on top

	vo_3d_type_none,
};


struct vo_output_info
{
	int width, height;

	int par_w, par_h;

	int framerate_num;
	int framerate_den;

	int progressive;

	enum vo_3d_type trid_type;
};

#ifdef __KERNEL__
int vo_set_output_info (int de_id, int width, int height,
		int framerate_num, int framerate_den,
		int par_w, int par_h,
		int progressive,
		enum vo_3d_type trid_type);
#endif


#endif
