
#ifndef _VO_H
#define _VO_H

#include <linux/kernel.h>

#include "include/vo_kapi.h"
#include "vds/disp_q.h"


struct vo
{
	int sync;
	int sync2;

	int open_dual;
	int flushed;

	int de_id;

	int width;
	int height;
	int stride;

	void *decoder_priv;

	int framerate_es_num;
	int framerate_es_den;
	int framerate_container_num;
	int framerate_container_den;
	int force_framerate_num;
	int force_framerate_den;
	enum DISPQ_FRAME_FORMAT frame_format;
	int queue_size;
	int inited;
	enum vo_3d_type trid_type;
	int speed;
	struct vo_basetime basetime;
	int display_offset;
	S_DISPQ_BUF_T keeping_picture;
	int keeping_sync;
	int keeping_deid;

	unsigned int flags;
#define FLAG_LOW_DELAY		(1<<0)
#define FLAG_UHD		(1<<1)
#define FLAG_FREERUN		(1<<2)

	int frames_written;
	int frames_displayed;
	int frames_dropped;

#define adaptive_queue_max	8
	int constant_delay;

	int par_w, par_h;

	/* displayed queue */
	spinlock_t displayed_queue_lock;
	wait_queue_head_t displayed_wait;
	struct vo_displayed_info *displayed_queue;
	int displayed_queue_size;
	volatile int displayed_queue_head;
	volatile int displayed_queue_tail;
	unsigned int displayed_queue_last_written_id;

	char proc_name[64];
	struct proc_dir_entry *proc_entry;
	struct proc_dir_entry *proc_status;
	atomic_t calling_read;
	atomic_t calling_ioctl;
	unsigned int last_ioctl_cmd;

	struct vo *next;
};

typedef struct vo vo_t;


int vo_set_output_info (int de_id, int width, int height,
		int framerate_num, int framerate_den,
		int par_w, int par_h,
		int progressive,
		enum vo_3d_type trid_type);

vo_t *vo_open (void);
int vo_release (vo_t *vo);
long vo_ioctl (vo_t *vo,
		unsigned int cmd, unsigned long arg,
		int from_user);


#endif

