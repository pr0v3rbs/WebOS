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

#ifndef _VIP_QUEUE_H_
#define _VIP_QUEUE_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#define vip_to_vq(_v)				((_v)->vqueue)
#define areq_to_vq(_a)				((_a)->vip->vqueue)

#define areq_to_vq_q_head(_a)		((_a)->vip->vqueue.queue)
#define areq_to_vq_run_head(_a)		((_a)->vip->vqueue.queue_run)
#define	areq_to_vq_done_head(_a)	((_a)->vip->vqueue.queue_done)

#define vq_q_head(_vq)				((_vq)->queue)
#define vq_run_head(_vq)			((_vq)->queue_run)
#define vq_done_head(_vq)			((_vq)->queue_done)

#define areq_q(_a)					((_a)->queue)
#define areq_op(_a)					((_a)->queue_op)

#define vq_lock(_vq)				((_vq)->lock)

/*
 *
 */
extern void VIP_Init_Queue(struct vip_dev *vip);

extern struct vip_req *VIP_Find_Queue(struct vip_dev *vip,
											int func, int index);
extern struct vip_req *VIP_Gen_Queue(struct vip_dev *vip,
											int func, int index);

extern void VIP_Add_Queue(struct vip_req *areq);
extern void VIP_Add_Queue_Run(struct vip_req *areq);
extern void VIP_Add_Queue_Done(struct vip_req *areq);

extern struct vip_req *VIP_Get_Queue_Run(struct vip_dev *vip);
extern struct vip_req *VIP_Chk_Queue_Run(struct vip_dev *vip);

extern struct vip_req *VIP_Find_Queue_Run(struct vip_dev *vip,
											int func, int index);

extern struct vip_req *VIP_Find_Queue_Done(struct vip_dev *vip,
											int func, int index);

extern void VIP_Free_Queue(struct vip_dev *vip, int func, int index);
extern void VIP_Abort_Queue_Run(struct vip_dev *vip, int func, int index);
extern void VIP_Abort_And_Free_Queue(struct vip_dev *vip,
												int func, int index,
												int remove);

extern void VIP_Abort_And_Free_Queue_All(struct vip_dev *vip);

extern void VIP_Set_Fmt_On_Queue(struct vip_dev *vip,
											struct vip_fmt_list *fmt_list,
											int func, int index);
extern void VIP_Set_Buf_On_Queue(struct vip_dev *vip,
											struct vip_buf *buf,
											int func, int index,
											int input);

extern int VIP_Get_Request_Sub_Fmt(struct vip_req *areq,
								int func, int index);

extern int VIP_Get_Request_Sub_Buf(struct vip_req *areq,
								int func, int index,
								int input);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _VIP_QUEUE_H_ */

