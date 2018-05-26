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

#ifndef	_VIP_MEM_H_
#define	_VIP_MEM_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "vip_debug.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#define _VIP_MEM_USE_COPY_USER

struct vip_buf;

enum {
	VIP_COPY_TO		= 0,
	VIP_COPY_FROM	= 1,
};

extern int VIP_Copy_From(void *dst, void *src, unsigned int size,
							int from_user, int io);
extern int VIP_Copy_To(void *dst, void *src, unsigned int size,
							int from_user, int io);

#define vip_call_memop(v, op, args...)	\
				(((v)->vmem_ops->op)?	((v)->vmem_ops->op(v, args)) : 0)	\

static inline void VIP_Mem_Destroy_Cache(struct vip_dev *vip)
{
	if (vip->vmem_ops && vip->vmem_ops->destroy_cache) {
		(vip->vmem_ops->destroy_cache)(vip);
	}
}

static inline int VIP_Mem_Create_Cache(struct vip_dev *vip)
{
	if (vip->vmem_ops && vip->vmem_ops->create_cache) {
		return (vip->vmem_ops->create_cache)(vip);
	}
	return -EFAULT;
}

static inline void *VIP_Mem_Alloc(struct vip_dev *vip, size_t s, gfp_t gfp, int cache)
{
	if (cache) {
		return vip_call_memop(vip, alloc_cache, s, gfp);
	}
	return vip_call_memop(vip, alloc, s, gfp);
}

static inline void *VIP_Mem_IOremap(struct vip_dev *vip,
									unsigned long paddr,
									size_t s,
									int io)
{
	return vip_call_memop(vip, iomap, paddr, s, io);
}

static inline void *VIP_Mem_Userptr_Get(struct vip_dev *vip,
										unsigned long vaddr,
										unsigned long size,
										int write,
										int io)
{
	return vip_call_memop(vip, userptr_get, vaddr, size, write, io);
}

static inline void *VIP_Mem_Get_Uptr(struct vip_dev *vip,
												void *buf_priv)
{
	return vip_call_memop(vip, get_uptr, buf_priv);
}

static inline void *VIP_Mem_Get_Vaddr(struct vip_dev *vip,
												void *buf_priv)
{
	return vip_call_memop(vip, get_vaddr, buf_priv);
}

static inline void *VIP_Mem_Get_Paddr(struct vip_dev *vip,
												void *buf_priv)
{
	return vip_call_memop(vip, get_paddr, buf_priv);
}

static inline unsigned long VIP_Mem_Get_Vsize(struct vip_dev *vip,
												void *buf_priv)
{
	return vip_call_memop(vip, get_vsize, buf_priv);
}

static inline void VIP_Mem_Free(struct vip_dev *vip, void *ptr)
{
	vip_call_memop(vip, free, ptr);
}

static inline void *VIP_Mem_Alloc_Buf(struct vip_dev *vip,
									void *buffer_priv,
									int input)
{
	return vip_call_memop(vip, alloc_buf, buffer_priv, input);
}

static inline void VIP_Mem_Free_Buf(struct vip_dev *vip,
									void *buf_priv)
{
	vip_call_memop(vip, free_buf, buf_priv);
}

static inline void VIP_Rst_Fmt_Usage(struct vip_fmt_list *fmt_list)
{
	if (fmt_list == NULL)
		return;

	atomic_set(&fmt_list->refcount, 0);
}

static inline int VIP_Chk_Fmt_Usage(struct vip_fmt_list *fmt_list)
{
	if (fmt_list == NULL)
		return 0;

	return atomic_read(&fmt_list->refcount);
}

static inline int VIP_Inc_Fmt_Usage(struct vip_fmt_list *fmt_list)
{
	if (fmt_list == NULL)
		return 0;

	return atomic_inc_return(&fmt_list->refcount);
}

static inline int VIP_Dec_Fmt_Usage(struct vip_fmt_list *fmt_list)
{
	if (fmt_list == NULL)
		return 0;

	if (!VIP_Chk_Fmt_Usage(fmt_list)) {
		vipm_err("Zero refcount\n");
		return -EFAULT;
	}
	return atomic_dec_return(&fmt_list->refcount);
}

extern void VIP_Get_Mem_Ops(struct vip_dev *vip);

extern void VIP_Buf_Flush(unsigned long vaddr, unsigned long size);

extern struct vip_fmt_list *VIP_Find_Fmt_List(struct vip_dev *vip,
													int func, int index);
extern void VIP_Clr_Fmt_List(struct vip_fmt_list *fmt_list);

extern struct vip_fmt_list *VIP_Add_Fmt_List(struct vip_dev *vip);


enum {
	VIP_BUF_OUTPUT	= 0,
	VIP_BUF_INPUT	= 1,
};

static inline void VIP_Rst_Buf_Usage(struct vip_buf *buf)
{
	if (buf == NULL)
		return;

	atomic_set(&buf->refcount, 0);
}

static inline int VIP_Chk_Buf_Usage(struct vip_buf *buf)
{
	if (buf == NULL)
		return 0;

	return atomic_read(&buf->refcount);
}

static inline int VIP_Inc_Buf_Usage(struct vip_buf *buf)
{
	if (buf == NULL)
		return 0;

	return atomic_inc_return(&buf->refcount);
}

static inline int VIP_Dec_Buf_Usage(struct vip_buf *buf)
{
	if (buf == NULL)
		return 0;

	if (!VIP_Chk_Buf_Usage(buf)) {
		vipm_err("Zero refcount\n");
		return -EFAULT;
	}
	return atomic_dec_return(&buf->refcount);
}

extern struct vip_buf *VIP_Find_Buf(struct vip_dev *vip,
										int func, int index,
										int input);

extern void VIP_Clr_Buf(struct vip_dev *vip,
							struct vip_buf *buf,
							int input);

extern struct vip_buf *VIP_Add_Buf(struct vip_dev *vip,
										struct vip_io_buffer *buffer,
										int input);

extern int VIP_Chk_Buf(struct vip_buf *buf, int input);
extern int VIP_Copy_Input_Buf(struct vip_req *areq);

extern void VIP_Clr_List_And_Buf(struct vip_dev *vip,
										int func, int index);

extern void VIP_Init_List(struct vip_dev *vip);

extern void VIP_Init_Mem_Test(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _VIP_MEM_H_ */


