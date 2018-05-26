/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
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

/**
 * @file
 *
 *  main driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#ifndef _VIP_DRV_H_
#define _VIP_DRV_H_

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>

#include "vip_debug.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

//#define	__VIP_DEVICE_CREATE

struct vip_top;
struct vip_req;
struct vip_dev;

/*
 * VIP Register configuration.
 */
enum {
	VIP_CHIP_H15_A0 = 0,
	VIP_CHIP_MAX,
};
struct vip_reg_cfg
{
	char			*chip_name;		//Chip Name
	unsigned int	reg_base;		//VIP Register Base Address for Real Chip.
	unsigned int	reg_size;		//VIP Register Size for Real Chip.
	unsigned int	irq_num;		//VIP IRQ 0 Number for Real Chip.
};

struct vip_buf_handler {
//	atomic_t		*refcount;
	void			(*put)(void *vip, void *arg);
};

struct vip_buf {
	struct list_head		list;
	/* */
	unsigned int			func;
	unsigned int			index;
	/* */
	void					*bsrc;
	/* */
	void					*vaddr;
	void					*paddr;
	unsigned long			user_ptr;
	struct page				**pages;
	struct vm_area_struct	*vma;
	int						write;
	unsigned long			size;
	unsigned int			n_pages;
	atomic_t				refcount;
	gfp_t					gfp;
	/* */
	struct vip_buf_handler	handler;
	/* */
	void					*priv;
	/* */
	void					*areq;
	/* */
	unsigned int			time_us;
	/* */
	unsigned long			flags;
};

enum {
	VIP_BUF_UPTR_STATE_NOT_CONTIG = 0,
	VIP_BUF_UPTR_STATE_NOT_ALIGNED,
	VIP_BUF_UPTR_STATE_MAX,
};

#define	VIP_BUF_UPTR_STATE_MASK_NOT_CONTIG		BIT(VIP_BUF_UPTR_STATE_NOT_CONTIG)
#define	VIP_BUF_UPTR_STATE_MASK_NOT_ALIGNED		BIT(VIP_BUF_UPTR_STATE_NOT_ALIGNED)

enum {
	VIP_BUF_FLAG_PADDR = 0,
	VIP_BUF_FLAG_VADDR,
	VIP_BUF_FLAG_KMALLOC,
	VIP_BUF_FLAG_VMALLOC,
	VIP_BUF_FLAG_COHERENT,
	VIP_BUF_FLAG_CACHE,
	VIP_BUF_FLAG_IOREMAP,
	VIP_BUF_FLAG_USERPTR,
	/* */
	VIP_BUF_FLAG_IO			= 8,
	/* */
	VIP_BUF_FLAG_NOT_CONTIG = 16,
	VIP_BUF_FLAG_NOT_ALIGNED,
	/* */
	VIP_BUF_FLAG_NO_CONV = 28,
	VIP_BUF_FLAG_MAX,
};

#define	VIP_BUF_FLAG_MASK_PADDR			BIT(VIP_BUF_FLAG_PADDR)
#define	VIP_BUF_FLAG_MASK_VADDR			BIT(VIP_BUF_FLAG_VADDR)
#define	VIP_BUF_FLAG_MASK_KMALLOC		BIT(VIP_BUF_FLAG_KMALLOC)
#define	VIP_BUF_FLAG_MASK_VMALLOC		BIT(VIP_BUF_FLAG_VMALLOC)
#define VIP_BUF_FLAG_MASK_COHERENT		BIT(VIP_BUF_FLAG_COHERENT)
#define	VIP_BUF_FLAG_MASK_CACHE			BIT(VIP_BUF_FLAG_CACHE)
#define	VIP_BUF_FLAG_MASK_IOREMAP		BIT(VIP_BUF_FLAG_IOREMAP)
#define	VIP_BUF_FLAG_MASK_USERPTR		BIT(VIP_BUF_FLAG_USERPTR)

#define VIP_BUF_FLAG_MASK_IO			BIT(VIP_BUF_FLAG_IO)

#define VIP_BUF_FLAG_MASK_NOT_CONTIG	BIT(VIP_BUF_FLAG_NOT_CONTIG)
#define VIP_BUF_FLAG_MASK_NOT_ALIGNED	BIT(VIP_BUF_FLAG_NOT_ALIGNED)

#define VIP_BUF_FLAG_MASK_NO_CONV		BIT(VIP_BUF_FLAG_NO_CONV)

struct vip_int_buf {
	unsigned char	*imgv;		/* Internal Image Buffer Address (Virtual) */
	unsigned char	*imgb;		/* Internal Image Buffer Address (Physical) */
	atomic_t		refcount;
};

struct vip_mem_ops {
	/* */
	void 			(*destroy_cache)(void *vip_priv);
	int 			(*create_cache)(void *vip_priv);
	/* */
	void 			*(*alloc)(void *vip_priv, size_t s, gfp_t gfp);
	/* */
	void 			*(*alloc_cache)(void *vip_priv,
									size_t s,
									gfp_t gfp);
	/* */
	void			*(*iomap)(void *vip_priv,
							unsigned long paddr,
							size_t s,
							int io);
	/* */
	void			*(*userptr_get)(void *vip_priv,
									unsigned long vaddr,
									unsigned long size,
									int write,
									int io);
	/* */
	void			*(*get_uptr)(void *vip_priv,
								void *buf_priv);
	void			*(*get_vaddr)(void *vip_priv,
								void *buf_priv);
	void			*(*get_paddr)(void *vip_priv,
								void *buf_priv);
	unsigned long	(*get_vsize)(void *vip_priv,
								void *buf_priv);
	/* */
	void			(*free)(void *vip_priv,
							void *ptr);
	/* */
	void			*(*alloc_buf)(void *vip_priv,
								void *buffer_priv,
								int input);
	void 			(*free_buf)(void *vip_priv,
								void *buf_priv);
};

/*
 * VIP HAL operations
 */
struct vip_hal_cfg {
	unsigned int base_addr;
	unsigned int base_size;
	/* */
	void	*firmware;
	int		load_fw;
	int		load_ldb;
	/* */
	void	*ipc_imgb;
	void	*imgb;
};

struct vip_hal_ops {
	char			*name;
	int 			(*init)(struct vip_dev *);
	int 			(*exit)(struct vip_dev *);
	int 			(*reset)(struct vip_dev *);
	/* */
	int				(*sleep)(struct vip_dev *);
	int				(*resume)(struct vip_dev *);
	int				(*scene_change)(struct vip_dev *);
	/* */
	int				(*setmode)(struct vip_dev *);
	int				(*setres)(struct vip_dev *);
	/* */
	int 			(*run)(struct vip_req *);
	int				(*msg)(struct vip_req *);
	int 			(*poll)(struct vip_req *);
	/* */
	void			(*top_writel)(struct vip_dev *, unsigned int v, unsigned int i);
	unsigned int 	(*top_readl)(struct vip_dev *, unsigned int i);
	void			(*ipc_writel)(struct vip_dev *, unsigned int v, unsigned int i);
	unsigned int 	(*ipc_readl)(struct vip_dev *, unsigned int i);
	/* */
	/* */
	struct vip_hal_cfg	cfg;
};

struct vip_fmt_list {
	struct list_head		list;
	struct vip_io_ifmt		format;
	atomic_t				refcount;
	void					*areq;
};

/**
 * VIP Request.
 */
struct vip_req
{
	struct list_head		queue;
	struct list_head		queue_op;
	/* */
	struct vip_dev			*vip;
	unsigned int			func;
	unsigned int			index;
	unsigned int			from_user;
	unsigned char			*ipc_imgb;
	/* */
	struct vip_fmt_list		*ifmt_list;
	struct vip_buf			*ibuf;
	struct vip_buf			*obuf;
	/* */
	unsigned char			*odata;
	unsigned int			osize;
	unsigned int			onum;
	/* */
	struct vip_msg_gesture	gesture;	//__VIP_GESTURE_ON
	struct vip_msg_crop		crop;
	/* */
	unsigned int			set_mode;
	/* */
	int						fault;
	int						error;
	unsigned long			timeout;
	/* */
	ktime_t					add_t;
	ktime_t					run_t;
	ktime_t					end_t;
	unsigned int			elapsed;
	/* */
	spinlock_t				*lock;
	struct completion		completion;
	/* */
	struct vip_hal_ops		*vhal_ops;
	/* */
	int 					(*post)(struct vip_req *);
	/* */
	unsigned long			aflags;
	/* */
	unsigned long private[0] ____cacheline_aligned;
};

enum vip_req_flags {
	VIP_REQ_FLAG_RUN = 0,
	VIP_REQ_FLAG_STOP,
	/* */
	VIP_REQ_FLAG_RESET = 8,
	VIP_REQ_FLAG_SCENE_CHANGE,
	VIP_REQ_FLAG_MAX,
};

enum vip_fault_completion {
	VIP_FAULT_COMPLETION_NONE = 0,
	VIP_FAULT_COMPLETION_PRE,
	VIP_FAULT_COMPLETION_RUN,
	VIP_FAULT_COMPLETION_POLLING,
	VIP_FAULT_COMPLETION_WAITING,
	VIP_FAULT_COMPLETION_ABORTED,
	VIP_FAULT_COMPLETION_MAX,
};

struct vip_req_queue {
	struct vip_dev		*vip;
	/* */
	spinlock_t			lock;
	struct list_head	queue;
	struct list_head	queue_run;
	struct list_head	queue_done;
	/* */
	unsigned long		flags;
};

enum vip_req_queue_flag {
	VIP_REQ_QUEUE_FLAG_DISCONNECTED = 0,
	VIP_REQ_QUEUE_FLAG_MAX,
};

#define VIP_REQ_QUEUE_FLAG_MASK_DISCONNECTED	BIT(VIP_REQ_QUEUE_FLAG_DISCONNECTED)

#define VIP_REQ_QUEUE_DISCONNECTED	(1 << 0)

#define VIP_REQ_SRCH_MASK_IBUF		BIT(0)
#define	VIP_REQ_SRCH_MASK_OBUF		BIT(1)

struct vip_drvdata
{
	struct vip_top		*vtop;
	int					is_suspended;
	/* */
	unsigned long private[0] ____cacheline_aligned;
};
typedef struct vip_drvdata LX_VIP_DRVDATA_T;

struct vip_kcache_list {
	struct list_head 		list;
	void 					*vaddr;
	int						index;
};

/*
 *
 */
struct vip_init_link {
	unsigned int	buf_max;
	unsigned int	buf_align;
	/* */
	int				max_func;
	/* */
	void			(*get_hal_ops)(struct vip_dev *vip);
	void			(*get_mem_ops)(struct vip_dev *vip);
	/* */
	void			*thread_handler;
	void			*irq_handler;
};

/*
 * VIP device
 */
struct vip_dev
{
	struct platform_device	*pdev;	/* platform_device */
	struct vip_top			*vtop;	/* vip_top */
	/* */
	char					name[VIP_MAX_NAME];
	/* */
#if (__VIP_USE_INPUT_DEVICE > 0)
	struct input_dev		*input;
#endif
	/* */
	struct clk				*clk;
	int						irq;
	irqreturn_t				(*irq_handler)(int irq, void *dev_id);
	/* */
	struct vip_hal_ops		*vhal_ops;
	struct vip_mem_ops		*vmem_ops;
	struct vip_init_link	*vinit_link;
	/* */
	int						cur_func;
	int						max_func;
	int						cur_index;
	struct vip_req			*areq;
	struct vip_run_err		vrun_err;
	/* */
	struct vip_req_queue	vqueue;
	/* */
	struct list_head		ifmt_list_head;	/* List head : Input format */
	struct list_head		ibuf_head;		/* List head : Input buffer */
	struct list_head		obuf_head;		/* List head : Output buffer */
	/* */
	struct vip_reg_cfg		*reg_cfg;
	unsigned char			*regb;
	unsigned char			*ipcb;
	/* */
	unsigned int			firm_base;
	unsigned int			lndb_base;
	unsigned char			*firm_addr;
	unsigned char			*lndb_addr;
	unsigned char			*ipc_imgb;
	struct vip_firm_down	firm_ctrl;	//__VIP_FIRM_DOWN_FROM_KADAP
	/* */
	struct vip_int_buf		vint_buf;
	/* */
	struct vip_kcache_list	klist_head;
	struct kmem_cache 		*klist_cache;
	struct kmem_cache 		*kcache;
	/* */
	unsigned int			buf_max;
	unsigned int			buf_align;
	/* */
	unsigned long			lib_ver;
	unsigned long			set_mode_val;
	unsigned int			set_mode;
	unsigned long			set_res;
	/* */
//	struct mutex			mutex;
	spinlock_t				_lock;
	spinlock_t				*lock;
	/* */
	wait_queue_head_t		areq_wait;
	/* */
	struct task_struct		*thread;
	struct semaphore		thread_sem;
	int						(*thread_handler)(void *d);
	/* */
	struct timer_list		timer;
	/* */
	struct notifier_block	pm_notify;
	unsigned int			pm_state;	// VIP_PM_GATING

	/* */
//	struct dentry			*debugfs_root;
	void					*debugfs_root;
	/* */
	unsigned int			poll_interval;	//Unit : usec
	/* */
	dev_t					devt;
	/* */
	int						open_count;
	/* */
	unsigned int			priv_data;
	/* */
	unsigned long			flags;
	/* */
	unsigned long private[0] ____cacheline_aligned;
};

#define VIP_FLAG_DSP			0
#define	VIP_FLAG_RUN			1
#define	VIP_FLAG_RSVD_02		2
#define	VIP_FLAG_RSVD_03		3
#define	VIP_FLAG_RSVD_04		4
#define	VIP_FLAG_RSVD_05		5
#define	VIP_FLAG_RESET			6
#define	VIP_FLAG_INIT_D			7	/* Init detector */

#define	VIP_FLAG_REGISTERED		8
#define	VIP_FLAG_INITED			9
#define	VIP_FLAG_OPENED			10
#define	VIP_FLAG_HW_INITED		11
#define	VIP_FLAG_RSVD_0C		12
#define	VIP_FLAG_RSVD_0D		13
#define	VIP_FLAG_CLK_GET		14
#define	VIP_FLAG_SLEEP			15

#define	VIP_FLAG_FAULT_RST		16
#define	VIP_FLAG_RSVD_11		17
#define	VIP_FLAG_RSVD_12		18
#define	VIP_FLAG_RSVD_13		19
#define	VIP_FLAG_RSVD_14		20
#define	VIP_FLAG_RSVD_15		21
#define	VIP_FLAG_RSVD_16		22
#define	VIP_FLAG_RSVD_17		23

#define	VIP_FLAG_RSVD_18		24
#define	VIP_FLAG_RSVD_19		25
#define	VIP_FLAG_RSVD_1A		26
#define	VIP_FLAG_RSVD_1B		27
#define	VIP_FLAG_RSVD_1C		28
#define	VIP_FLAG_RSVD_1D		29
#define	VIP_FLAG_RSVD_1E		30
#define	VIP_FLAG_RSVD_1F		31

#define VIP_FLAG_MASK_OPERATION			0xFF

#define	VIP_FLAG_MASK_DSP				BIT(VIP_FLAG_DSP)
#define	VIP_FLAG_MASK_RUN				BIT(VIP_FLAG_RUN)

#define	VIP_FLAG_MASK_RESET				BIT(VIP_FLAG_RESET)
#define	VIP_FLAG_MASK_INIT_D			BIT(VIP_FLAG_INIT_D)

#define	VIP_FLAG_MASK_REGISTERD			BIT(VIP_FLAG_REGISTERED)

#define	VIP_FLAG_MASK_INITED			BIT(VIP_FLAG_INITED)
#define	VIP_FLAG_MASK_OPENED			BIT(VIP_FLAG_OPENED)
#define	VIP_FLAG_MASK_HW_INITED			BIT(VIP_FLAG_HW_INITED)

#define VIP_FLAG_MASK_CLK_GET			BIT(VIP_FLAG_CLK_GET)
#define VIP_FLAG_MASK_SLEEP				BIT(VIP_FLAG_SLEEP)

#define VIP_FLAG_MASK_FAULT_RST			BIT(VIP_FLAG_FAULT_RST)

#define vip_dev_name(vip)			(vip->name)

static inline void *vip_dev_priv(struct vip_dev *vip)
{
	return (void *)vip->private;
}

/*
 * VIP Top Information.
 */
struct vip_top
{
//	int						dev_open_count;
	dev_t					devNo;		/* device number */
	struct cdev				cdev;		/* charactor device struct */
	/* */
#if defined(__VIP_DEVICE_CREATE)
	struct device			class_dev;
	struct device			*dev;
#endif
#if defined(VIP_INPUT_DEVICE_ENABLE)
	struct input_dev		*input;
#endif
	/* */
	// VIP_KDRV_CONFIG_PM
	struct platform_device	*pdev;
	struct device			*parent;
	/* */
	int						idx;
	/* */
	unsigned long private[0] ____cacheline_aligned;
};

#define to_vip_top_dev(cd)			container_of(cd, struct vip_top, class_dev)
#define	to_vip_top_cdev(cdev)		container_of(cdev, struct vip_top, cdev)

/**
 * for vip_dev
 */
static inline void *vip_top_priv(struct vip_top *vtop)
{
	return (void *)vtop->private;
}

extern void VIP_PreInit(void);
extern int VIP_Init(void);
extern void VIP_Cleanup(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */


#endif	/* _VIP_DRV_H_ */

