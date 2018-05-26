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
#ifndef _DECODER_H
#define _DECODER_H

#include <linux/spinlock.h>
#include <linux/mutex.h>

#include "include/vdec_kapi.h"
#include "include/vo_kapi.h"

#include "vds/disp_clear_q.h"

#include "decoded_q.h"
#include "output.h"

typedef struct
{
	wait_queue_head_t wq;

	unsigned int mask;
	int size;
	volatile int head;
	volatile int tail;

	void *data;
#if defined(CHIP_NAME_h15) && defined(__XTENSA__)
	phys_addr_t data_phy;
#endif

	spinlock_t write_lock;
	struct mutex read_lock;
} noti_t;


struct dpbdump
{
	wait_queue_head_t wq;

	decoded_buffer_t *cur;

	/* for tiled */
	unsigned char *fb;
	int fb_size;

	/* for raster */
	unsigned char *fb_y, *fb_c;
	int fb_size_y, fb_size_c;

	int fb_readed;

	decoded_buffer_t **data;
	int size;
	int head;
	int tail;
};

struct vdec;
struct cpbdump
{
	wait_queue_head_t wq;

	unsigned char *addr;

	int (*cpb_read) (struct vdec *);
	int now, readed;
};

struct decoded_info
{
	int user;

	decoded_buffer_t buffer;

	struct decoded_info *next;
};


struct dpb
{
	LX_VDEC_FB_T fb;
	struct dpb *next;
};

typedef struct vdec vdec_t;
struct vdec
{
	noti_t noti;

	struct file *file;

	LX_VDEC_SRC_T src;
	LX_VDEC_DST_T dest;
	unsigned int preferred_dec;

	LX_VDEC_OPMODE_T opmode;
	LX_VDEC_CODEC_T codec;
	LX_VDEC_PIC_SCAN_T scan_mode;

	/* flags defined at lx_vdec_flags_t or FLAGS_INTER_*
	 *  0~15: lx_vdec_flags_t. lxvdec maintains the flags.
	 * 16~31: FLAGS_INTER_*. internal flags. driver will manage.
	 */
	unsigned int flags;
	spinlock_t flags_lock;

	struct mutex submodule_lock;
	spinlock_t submodule_spin_lock;
	spinlock_t submodule_vdc_spin_lock;

	enum vo_3d_type trid_type;

	int userdata_thief;
	unsigned long id;

	int framerate_num;
	int framerate_den;
	LX_VDEC_MEMORY_FORMAT_T output_memory_format;

	struct ion_client *ion_client;
	struct dpb *dpb;
	spinlock_t dpb_lock;
	decoded_buffer_t *wait_dpb;

	int port;
	int port_adec;



	/* instance of submodules */
	int ves;
	int vdc;
	int sync;

	/* codec informations */
	phys_addr_t cpb_addr;
	unsigned long cpb_vaddr;
	dma_addr_t cpb_daddr;
	int cpb_size;
	int cpb_allocated;
	wait_queue_head_t codec_state_changed;

	/* decoded queue */
	decoded_q_t decoded_q;
	struct decoded_info *decoded_infos;
	spinlock_t decoded_buffers_lock;
	int frame_decoded;
	int frame_decoded_all;
	int frame_requested;
	int last_display_diff;
	unsigned int last_display_pts;

	/* decoding queue */
	int decoding_queue_size;
	int decoding_queue_slots;
	int flushed;
	int cpb_full_count;

	LX_VDEC_D14_UHDSEAMLESS d14_seamless;



	/* display informations */
	unsigned long speed;
	spinlock_t speed_change;

	/* display queue */
	int display_offset;
	int display_q_maxsize;
	int use_gstc;
	int use_gstc_have_cut;

	unsigned int base_stc;
	unsigned int base_pts;

	unsigned int adec_base_stc;
	unsigned int adec_base_pts;

	/* clear queue */
	clear_q_t *clear_q;
	int kick_decoder_count;
	int frame_displayed;
	int frame_dropped;
	unsigned int last_displayed_pts;

	enum output_pts_match match_mode;


	/* dpb/cpb dump, debug */
	spinlock_t dpbdump_lock;
	struct dpbdump dpbdump;
	struct cpbdump cpbdump;

	char proc_name[64];
	struct proc_dir_entry *proc_entry;
	struct proc_dir_entry *proc_status;
	struct proc_dir_entry *proc_cpb_reader;
	struct proc_dir_entry *proc_cpb_writer;
	struct proc_dir_entry *proc_dpb;

	vdec_t *next;
};


vdec_t *vdec_open(struct file *filp);
int vdec_close(vdec_t *vdec);
long vdec_ioctl (vdec_t *vdec,
		unsigned int cmd, unsigned long arg,
		int from_user);
const char *vdec_flag_name (unsigned int flags);

int unref_frame_and_kick_decoder_locked (vdec_t *vdec,
		unsigned long y);
const char *src_name (LX_VDEC_SRC_T s);
const char *codec_name (LX_VDEC_CODEC_T c);


#endif
