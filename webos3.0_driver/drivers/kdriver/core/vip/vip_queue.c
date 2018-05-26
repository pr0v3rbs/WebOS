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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <linux/file.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_queue.h"
#include "vip_drv_hal.h"
#include "vip_drv_img.h"
#include "vip_mem.h"
#include "vip_proc.h"

static void _VIP_Init_Queue_Head(struct vip_req_queue *vq)
{
	INIT_LIST_HEAD(&vq_q_head(vq));
	INIT_LIST_HEAD(&vq_run_head(vq));
	INIT_LIST_HEAD(&vq_done_head(vq));
}

void VIP_Init_Queue(struct vip_dev *vip)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);

	spin_lock_init(&vq_lock(vq));
	_VIP_Init_Queue_Head(vq);
}

static inline int _VIP_Chk_Queue_Link(struct list_head *queue)
{
	return (queue->prev != queue);
}

static inline void _VIP_Del_Queue_Op(struct vip_req *areq)
{
	struct list_head *queue = &areq_op(areq);
	if (_VIP_Chk_Queue_Link(queue)) {
		list_del_init(queue);
		VIP_Rst_Areq_Flags(areq);
		vipm_debug_queue("areq[0x%p] deleted from queue_op\n", areq);
	}
}

static inline void _VIP_Del_Queue(struct vip_req *areq)
{
	struct list_head *queue = &areq_q(areq);
	if (_VIP_Chk_Queue_Link(queue)) {
		list_del_init(queue);
		vipm_debug_queue("areq[0x%p] deleted from queue\n", areq);
	}
}

static inline void _VIP_Add_Queue(struct vip_req *areq)
{
	list_add_tail(&areq_q(areq), &areq_to_vq_q_head(areq));
	vipm_debug_queue("New areq[0x%p] added on to vqueue\n", areq);
}

static void _VIP_Move_Queue_Run_To_Done(struct vip_req *areq)
{
	list_move_tail(&areq_op(areq), &areq_to_vq_done_head(areq));
	VIP_Clr_Areq_Flags(areq, VIP_REQ_FLAG_RUN);
	VIP_Clr_Areq_Flags(areq, VIP_REQ_FLAG_STOP);
	vipm_debug_queue("areq[0x%p] moved from queue_run to queue_done\n", areq);
}

static void _VIP_Move_Queue_Run_To_Done_By_Stop(struct vip_req *areq)
{
	VIP_Set_Completion(areq, VIP_FAULT_COMPLETION_ABORTED);

	VIP_Dec_Fmt_Usage(areq->ifmt_list);
	VIP_Inc_Buf_Usage(areq->ibuf);
	VIP_Inc_Buf_Usage(areq->obuf);

	_VIP_Move_Queue_Run_To_Done(areq);
	areq->error = -EFAULT;
	vipm_debug_queue("areq[0x%p] is aborted\n", areq);
}

static void _VIP_Move_Queue_Done_To_Run(struct vip_req *areq)
{
	struct list_head *queue = &areq_op(areq);
	struct list_head *head = &areq_to_vq_run_head(areq);

	if (_VIP_Chk_Queue_Link(queue)) {
		list_move_tail(queue, head);
		vipm_debug_queue("areq[0x%p] moved from queue_done to queue_run\n", areq);
	} else {
		list_add_tail(queue, head);
		vipm_debug_queue("areq[0x%p] added on to queue_run\n", areq);
	}
}

static struct vip_req *_VIP_Get_Queue_Run(struct vip_dev *vip)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq = NULL;

	areq = list_first_entry_or_null(&vq_run_head(vq),
									struct vip_req,
									queue_op);
	if (areq) {
		vipm_debug_queue("First entry of queue_run : areq[0x%p]\n", areq);
	}
	return areq;
}

struct vip_req *_VIP_Find_Queue(struct vip_dev *vip,
									int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq_found = NULL;
	struct vip_req *areq;

	list_for_each_entry(areq, &vq_q_head(vq), queue) {
		if ((areq->func == func) &&
			(areq->index == index)) {
			areq_found = areq;
			vipm_debug_queue("areq[0x%p](func[%d], index[%d]) found in queue\n",
							areq, func, index);
			break;
		}
	}
	return areq_found;
}

struct vip_req *_VIP_Find_Queue_Op(struct vip_dev *vip,
										int func, int index,
										int run_head)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq_found = NULL;
	struct vip_req *areq;
	struct list_head *head;
	int no_msg = index & VIPM_NO_MSG_FLAG;

	index &= VIPM_NO_MSG_MASK;

	head = (run_head)? &vq_run_head(vq) : &vq_done_head(vq);

	list_for_each_entry(areq, head, queue_op) {
		if ((areq->func == func) &&
			(areq->index == index)) {
			areq_found = areq;
			if (!no_msg) {
				vipm_debug_queue("areq[0x%p](func[%d], index[%d]) found in %s\n",
								areq, func, index,
								(run_head)? "queue_run" : "queue_done");
			}
			break;
		}
	}
	return areq_found;
}

struct vip_req *_VIP_Find_Queue_Run(struct vip_dev *vip,
											int func, int index)
{
	return _VIP_Find_Queue_Op(vip, func, index, 1);
}

struct vip_req *_VIP_Find_Queue_Done(struct vip_dev *vip,
											int func, int index)
{
	return _VIP_Find_Queue_Op(vip, func, index, 0);
}

struct vip_req *VIP_Find_Queue(struct vip_dev *vip,
									int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue(vip, func, index);
	spin_unlock(&vq_lock(vq));
	return areq;
}

struct vip_req *VIP_Gen_Queue(struct vip_dev *vip,
									int func, int index)
{
	struct vip_req *areq = NULL;

	if ((vip->vhal_ops == NULL) ||
		(vip->lock == NULL)) {
		vipm_err("NULL : vip->hal_ops[0x%p], vip->lock[0x%p]\n",
				vip->vhal_ops, vip->lock);
		goto out;
	}

	areq = (struct vip_req *)kzalloc(sizeof(struct vip_req), GFP_KERNEL);
	if (areq == NULL) {
		vipm_err("NULL areq\n");
		goto out;
	}

	areq->vip = vip;
	areq->func = func;
	areq->index = index;
	areq->vhal_ops = vip->vhal_ops;
	areq->lock = vip->lock;
	areq->ipc_imgb = vip->ipc_imgb;

	vipm_debug_queue("areq        : 0x%p\n", areq);
	vipm_debug_queue(" ->vip      : 0x%p\n", areq->vip);
	vipm_debug_queue(" ->func     : %d\n", areq->func);
	vipm_debug_queue(" ->index    : %d\n", areq->index);
	vipm_debug_queue(" ->vhal_ops : 0x%p\n", areq->vhal_ops);
	vipm_debug_queue(" ->lock     : 0x%p\n", areq->lock);
	vipm_debug_queue(" ->ipc_imgb : 0x%p\n", areq->ipc_imgb);

	INIT_LIST_HEAD(&areq_q(areq));
	INIT_LIST_HEAD(&areq_op(areq));
	VIP_Rst_Areq_Flags(areq);

out:
	return areq;
}

void VIP_Add_Queue(struct vip_req *areq)
{
	struct vip_req_queue *vq = &areq_to_vq(areq);

	spin_lock(&vq_lock(vq));
	_VIP_Add_Queue(areq);
	spin_unlock(&vq_lock(vq));
}

void VIP_Add_Queue_Run(struct vip_req *areq)
{
	struct vip_req_queue *vq = &areq_to_vq(areq);

	spin_lock(&vq_lock(vq));
	_VIP_Move_Queue_Done_To_Run(areq);
	spin_unlock(&vq_lock(vq));
}

void VIP_Add_Queue_Done(struct vip_req *areq)
{
	struct vip_req_queue *vq = &areq_to_vq(areq);

	spin_lock(&vq_lock(vq));
	_VIP_Move_Queue_Run_To_Done(areq);
	spin_unlock(&vq_lock(vq));
}

struct vip_req *VIP_Get_Queue_Run(struct vip_dev *vip)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq = NULL;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Get_Queue_Run(vip);
	spin_unlock(&vq_lock(vq));
	return areq;
}

struct vip_req *VIP_Chk_Queue_Run(struct vip_dev *vip)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq = NULL;

	spin_lock(&vq_lock(vq));

	vipm_debug_queue("\n");

	if (VIP_Is_Inited(vip) && VIP_Is_HW_Inited(vip)) {
		areq = _VIP_Get_Queue_Run(vip);
	}
	if (areq != NULL) {
		if (VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_STOP)) {
			_VIP_Move_Queue_Run_To_Done_By_Stop(areq);
			areq = NULL;
		} else {
			VIP_Set_Areq_Flags(areq, VIP_REQ_FLAG_RUN);
			vipm_debug_queue("areq[0x%p] -> vip_run_requet\n", areq);
		}
	}

	if (areq == NULL) {
		vipm_debug_queue("No areq\n");
	}
	spin_unlock(&vq_lock(vq));
	return areq;
}

struct vip_req *VIP_Find_Queue_Run(struct vip_dev *vip,
											int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue_Run(vip, func, index);
	spin_unlock(&vq_lock(vq));
	return areq;
}

struct vip_req *VIP_Find_Queue_Done(struct vip_dev *vip,
											int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue_Done(vip, func, index);
	spin_unlock(&vq_lock(vq));
	return areq;
}

static void VIP_Free_Queue_All(struct vip_dev *vip)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq, *t;

	spin_lock(&vq_lock(vq));
	list_for_each_entry_safe_reverse(areq, t, &vq_q_head(vq), queue) {
		vipm_debug_queue("free areq[0x%p]\n", areq);
		kfree(areq);
	}
	_VIP_Init_Queue_Head(vq);
	spin_unlock(&vq_lock(vq));
}

void VIP_Free_Queue(struct vip_dev *vip,
					int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	if (func == -1) {
		VIP_Free_Queue_All(vip);
		return;
	}

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue(vip, func, index);
	if (areq) {
		_VIP_Del_Queue_Op(areq);
		_VIP_Del_Queue(areq);
		vipm_debug_queue("free areq[0x%p]\n", areq);
		kfree(areq);
	}
	spin_unlock(&vq_lock(vq));
}

static void VIP_Set_Abort_Queue_Run(struct vip_dev *vip,
									int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq_first = NULL;
	struct vip_req *areq = NULL;

	spin_lock(&vq_lock(vq));
	areq_first = _VIP_Get_Queue_Run(vip);
	if (areq_first) {
		if (func == -1) {
			list_for_each_entry(areq,
								&vq_run_head(vq),
								queue_op) {
				if (areq == areq_first) {
					if (!VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_RUN)) {
						VIP_Set_Areq_Flags(areq, VIP_REQ_FLAG_STOP);
					}
				} else {
					_VIP_Move_Queue_Run_To_Done_By_Stop(areq);
				}
			}
			vipm_debug_queue("Trying to stop all areq on queue_run\n");
		} else {
			areq = _VIP_Find_Queue_Run(vip, func, index);
			if (areq) {
				if (areq == areq_first) {
					if (!VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_RUN)) {
						VIP_Set_Areq_Flags(areq, VIP_REQ_FLAG_STOP);
					}
				} else {
					_VIP_Move_Queue_Run_To_Done_By_Stop(areq);
				}

				vipm_debug_queue("Trying to abort areq[0x%p] on queue_run\n", areq);
				if (areq == areq_first) {
					vipm_warn("Target request is running...\n");
				}
			}
		}
	}
	spin_unlock(&vq_lock(vq));

	if (areq_first) {
		wake_up_process(vip->thread);
	}
}

static void VIP_Wait_Abort_Queue_Run(struct vip_dev *vip,
												int func)
{
	if (func == -1) {
		vipm_debug_queue("Wait until queue_run becomes empty state\n");
		while(1) {
			if (VIP_Get_Queue_Run(vip) == NULL) {
				break;
			}
			wake_up_process(vip->thread);
			VIP_Delay_US(100);
		}
	}
}

void VIP_Abort_Queue_Run(struct vip_dev *vip,
						int func, int index)
{
	VIP_Set_Abort_Queue_Run(vip, func, index);
	VIP_Wait_Abort_Queue_Run(vip, func);
}

void VIP_Abort_And_Free_Queue(struct vip_dev *vip,
							int func, int index,
							int free)
{
	VIP_Abort_Queue_Run(vip, func, index);
	if (free) {
		VIP_Free_Queue(vip, func, index);
		VIP_Clr_List_And_Buf(vip, func, index);
	}
}

void VIP_Abort_And_Free_Queue_All(struct vip_dev *vip)
{
	VIP_Abort_And_Free_Queue(vip, -1, 0, 1);
}

/*
 *
 */
#define __vip_set_areq(v, a)				\
		do {								\
			if (v != NULL) (v)->areq = (a);	\
		} while(0)

void VIP_Set_Fmt_On_Queue(struct vip_dev *vip,
							struct vip_fmt_list *fmt_list,
							int func, int index)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue(vip, func, index);
	if (areq != NULL) {
		areq->ifmt_list = fmt_list;
		__vip_set_areq(fmt_list, areq);
	}
	spin_unlock(&vq_lock(vq));
}

void VIP_Set_Buf_On_Queue(struct vip_dev *vip,
									struct vip_buf *buf,
									int func, int index,
									int input)
{
	struct vip_req_queue *vq = &vip_to_vq(vip);
	struct vip_req *areq;

	spin_lock(&vq_lock(vq));
	areq = _VIP_Find_Queue(vip, func, index);
	if (areq != NULL) {
		if (input) {
			areq->ibuf = buf;
		} else {
			areq->obuf = buf;
		}
		__vip_set_areq(buf, areq);
	}
	spin_unlock(&vq_lock(vq));
}

int VIP_Get_Request_Sub_Fmt(struct vip_req *areq,
								int func, int index)
{
	struct vip_dev *vip = areq->vip;
	struct vip_fmt_list *fmt_list = areq->ifmt_list;

	if (fmt_list == NULL) {
		fmt_list = VIP_Find_Fmt_List(vip, func, index);
		if (fmt_list == NULL) {
			vipm_err("Format entry not found\n");
			return -ENOMEM;
		}
		if (VIP_Chk_Fmt(&fmt_list->format)) {
			vipm_err("Image format error\n");
			return -EINVAL;
		}
		areq->ifmt_list = fmt_list;
		__vip_set_areq(fmt_list, areq);
	}
	vipm_debug_queue(" areq->imft_list : 0x%p\n", fmt_list);
	return 0;
}

int VIP_Get_Request_Sub_Buf(struct vip_req *areq,
								int func, int index,
								int input)
{
	struct vip_dev *vip = areq->vip;
	struct vip_buf *buf = (input)? areq->ibuf : areq->obuf;

	if (buf == NULL) {
		buf = VIP_Find_Buf(vip, func, index, input);
		if (buf == NULL) {
			vipm_err("%s buffer entry not found\n",
					(input)? "Input" : "Output");
			return -ENOMEM;
		}
		if (input) {
			areq->ibuf = buf;
		} else {
			areq->obuf = buf;
		}
		__vip_set_areq(buf, areq);
	}
	vipm_debug_queue(" areq->%s      : 0x%p\n",
					(input)? "ibuf" : "obuf",
					buf);
	return 0;
}

