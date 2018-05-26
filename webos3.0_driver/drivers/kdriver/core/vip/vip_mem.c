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

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
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
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
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
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
#define __VIP_PHYS_DMA_MAP			1

#define __VIP_PHYS_PAGE_TO_PHYS		1
#define __VIP_PHYS_DMA_MAP_PAGE		1

#define __VIP_PHYS_DMA_COHERENT		1

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

//#define VIP_USE_BUF_CPY_LOCK

#if defined(VIP_USE_BUF_CPY_LOCK)
static DEFINE_SPINLOCK(vip_buf_cpy_lock);
#endif

/*
 *
 */
static int _VIP_Copy(void *dst, void *src, unsigned int size,
						int user, int opt, int io)
{
	char *fname;
	int from = (opt == VIP_COPY_FROM);
	int no_msg = io & VIPM_NO_MSG_FLAG;
	int ret = 0;

	if ((dst == NULL) || (src == NULL) || !size) {
		vipm_err("NULL : dst[0x%p], src[0x%p], size[0x%8x]\n",
				dst, src, size);
		return -EFAULT;
	}

	io = !!(io & VIPM_NO_MSG_MASK);

	if (user & 0x01) {
		ret = (from)? copy_from_user(dst, (void __user *)src, size) :	\
					copy_to_user((void __user *)dst, src, size);
		fname = (from)? "copy_from_user" : "copy_to_user";
	} else {
		if (io & 0x01) {
			/*
			 * The operation time of memcpy_xxxio is at least 200 times bigger than memcpy
			 */
			if (from) {
				memcpy_fromio(dst, src, size);
				fname = "memcpy_fromio";
			} else {
				memcpy_toio(dst, src, size);
				fname = "memcpy_toio";
			}
		} else {
			memcpy(dst, src, size);
			fname = "memcpy";
		}
	}

	if (ret) {
		vipm_err("%s failed(%d) : dst[0x%p] <- src[0x%p], size[0x%x]\n",
				fname, ret, dst, src, size);
	} else {
		if (!no_msg) {
			vipm_debug_mem("%s done : dst[0x%p] <- src[0x%p], size[0x%x]\n",
							fname, dst, src, size);
		}
	}
	return ret;
}

int VIP_Copy_From(void *dst, void *src, unsigned int size,
						int user, int io)
{
	return _VIP_Copy(dst, src, size, user, VIP_COPY_FROM, io);
}

int VIP_Copy_To(void *dst, void *src, unsigned int size,
						int user, int io)
{
	return _VIP_Copy(dst, src, size, user, VIP_COPY_TO, io);
}

/*
 *
 */
static int VIP_Chk_Contig_Vaddr(void *buf_priv)
{
	struct vip_buf *buf = buf_priv;
	unsigned long vaddr, vaddr_page;
	unsigned long phys, phys_old;
	unsigned long size;
	unsigned long first, last;
	int contig_mem = 1;
	int n_pages;
	int i;

	vaddr = (unsigned long)buf->vaddr;
	size = buf->size;

	first = vaddr >> PAGE_SHIFT;
	last = (vaddr + size - 1) >> PAGE_SHIFT;
	n_pages = last - first + 1;

	vaddr_page = vaddr & ~PAGE_MASK;

	for (i=0 ; i<n_pages ; i++) {
		phys = virt_to_phys((void *)vaddr_page);
	#if 0
		vipm_debug_mem("phys[0x%lx] <-> vaddr_page[0x%lx]\n",
						phys, vaddr_page);
	#endif
		if (i && ((phys_old + PAGE_SIZE) != phys))  {
			vipm_debug_mem("phys[0x%lx] != phys_old[0x%lx] + PAGE_SIZE[0x%lx]\n",
						phys, phys_old, PAGE_SIZE);
			vip_set_bit(VIP_BUF_FLAG_NOT_CONTIG, &buf->flags);
			contig_mem = 0;
			break;
		}
		phys_old = phys;
		vaddr_page += PAGE_SIZE;
	}
	if (contig_mem) {
		vipm_debug_mem("Contiguous memory\n");
	}
	return contig_mem;
}

static int __used VIP_Chk_Contig_Pages(void *buf_priv)
{
	struct vip_buf *buf = buf_priv;
	unsigned long phys, phys_old;
	int contig_mem = 1;
	int i;

	for (i=0 ; i<buf->n_pages ; i++) {
		phys = page_to_phys(buf->pages[i]);
	#if 0
		vipm_debug_mem("phys[0x%lx] <-> pages[0x%p]\n",
						phys, buf->pages[i]);
	#endif
		if (i && ((phys_old + PAGE_SIZE) != phys))  {
			vipm_debug_mem("phys[0x%lx] != phys_old[0x%lx] + PAGE_SIZE[0x%lx]\n",
						phys, phys_old, PAGE_SIZE);
			vip_set_bit(VIP_BUF_FLAG_NOT_CONTIG, &buf->flags);
			contig_mem = 0;
			break;
		}
		phys_old = phys;
	}
	if (contig_mem) {
		vipm_debug_mem("Contiguous memory\n");
	}
	return contig_mem;
}

/*
 *
 */
#if (__VIP_PHYS_DMA_MAP > 0)
static void VIP_Put_Phys(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = buf_priv;

	if (buf->paddr) {
		int dir = (buf->write)? DMA_FROM_DEVICE : DMA_TO_DEVICE;
		dma_unmap_single(vip->vtop->parent,
						(dma_addr_t)buf->paddr,
						buf->size,
						dir);
		vipm_debug_mem("dma_unmap_single[%s] : 0x%p[0x%lx]\n",
				(dir == DMA_FROM_DEVICE)? "w" : "r",
				 buf->paddr, buf->size);
	}
}

static void *VIP_Get_Phys(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = buf_priv;
	void *paddr;
	int dir = (buf->write)? DMA_FROM_DEVICE : DMA_TO_DEVICE;

	paddr = (void *)dma_map_single(vip->vtop->parent,
								buf->vaddr,
								buf->size,
								dir);
	if (dma_mapping_error(NULL, (dma_addr_t)paddr)) {
		paddr = NULL;
	}
	vipm_debug_mem("dma_map_single %s[%s] : 0x%p[0x%lx] <- 0x%p\n",
				(paddr)? "done" : "error",
				(dir == DMA_FROM_DEVICE)? "w" : "r",
				paddr, buf->size, buf->vaddr);
	if (paddr && (dir == DMA_TO_DEVICE)) {
		dma_sync_single_for_device(vip->vtop->parent,
								(dma_addr_t)paddr,
								(size_t)buf->size,
								dir);
		vipm_debug_mem("dma_sync_single_for_device\n");
	}
	return paddr;
}
#else
#define VIP_Put_Phys(_vip, _buf)	{	}
#define VIP_Get_Phys(_vip, _buf)	(void *)virt_to_phys((_buf)->vaddr);
#endif

/*
 *
 */
static void VIP_Mem_Handler(struct vip_buf *buf, void *handler)
{
//	buf->handler.refcount = &buf->refcount;
	buf->handler.put = handler;
}

static void VIP_Free_Mem(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = (struct vip_buf *)buf_priv;

	if (buf){
		/*
		if (!atomic_dec_and_test(&buf->refcount)) {
			return;
		}
		*/
		if (vip_test_bit(VIP_BUF_FLAG_COHERENT, &buf->flags)) {
			dma_addr_t dma_addr = (dma_addr_t)buf->paddr;
			dma_free_coherent(&vip->pdev->dev, buf->size, buf->vaddr, dma_addr);
			return;
		}

		VIP_Put_Phys(vip_priv, buf);

		if (vip_test_bit(VIP_BUF_FLAG_VMALLOC, &buf->flags)) {
			vfree(buf->vaddr);
		}
		else if (vip_test_bit(VIP_BUF_FLAG_KMALLOC, &buf->flags)) {
			kfree(buf->vaddr);
		}
		kfree(buf);
	}
}

static void *VIP_Alloc_Mem(void *vip_priv, size_t s, gfp_t gfp)
{
	if (s) {
		struct vip_dev *vip = vip_priv;
		struct vip_buf *buf;
		dma_addr_t dma_addr;
		int sel_vm;

		buf = kzalloc(sizeof(*buf), GFP_KERNEL);
		if (buf == NULL) {
			vipm_err("NULL vip_buf\n");
			return NULL;
		}
		buf->size = s;
		buf->gfp = GFP_KERNEL | GFP_DMA | gfp;

		sel_vm = !!(s > KMALLOC_MAX_SIZE);
	#if (__VIP_PHYS_DMA_COHERENT > 0)
		vipm_debug_mem("dma_alloc_coherent selected\n");
		buf->vaddr = dma_alloc_coherent(&vip->pdev->dev,
									  s, &dma_addr, GFP_KERNEL);
		if ((buf->vaddr == NULL) || !dma_addr) {
			vipm_err("NULL dma_alloc_coherent\n");
			kfree(buf);
			return NULL;
		}
		buf->paddr = (void *)dma_addr;
		vip_set_bit(VIP_BUF_FLAG_COHERENT, &buf->flags);
	#else
		vipm_debug_mem("%s selected\n",
					(sel_vm)? "vmalloc" : "kmalloc");
		buf->vaddr = (sel_vm)? vmalloc(s) :	\
						kmalloc(s, buf->gfp);
		if (buf->vaddr == NULL) {
			vipm_err("NULL %s\n", (sel_vm)? "vmalloc" : "kmalloc");
			kfree(buf);
			return NULL;
		}
		buf->paddr = VIP_Get_Phys(vip_priv, buf);
		if (buf->paddr == NULL) {
			if (sel_vm) {
				vfree(buf->vaddr);
			} else {
				kfree(buf->vaddr);
			}
			kfree(buf);
			return NULL;
		}
		vip_set_bit((sel_vm)? VIP_BUF_FLAG_VMALLOC : VIP_BUF_FLAG_KMALLOC,
					&buf->flags);
	#endif
		vipm_debug_mem(" vaddr[0x%p] paddr[0x%p]\n",
					buf->vaddr, buf->paddr);
		VIP_Mem_Handler(buf, VIP_Free_Mem);
	//	atomic_inc(&buf->refcount);
		return buf;
	}
	return NULL;
}

static void *VIP_Srch_Klist(void *vip_priv, void *ptr)
{
	struct vip_dev *vip = vip_priv;
	struct vip_kcache_list *klist;

	list_for_each_entry(klist, &vip->klist_head.list, list) {
		if (klist->vaddr == ptr) {
			return klist;
		}
	}
	vipm_err("No entry in vip klist!!! : 0x%p\n", ptr);
	return NULL;
}

static void VIP_Del_Klist(void *vip_priv, void *dlist)
{
	struct vip_dev *vip = vip_priv;
	struct vip_kcache_list *klist = dlist;

	list_del(&klist->list);
	kmem_cache_free(vip->klist_cache, klist);
	vipm_debug_mem("klist deleted : 0x%p\n", klist);
}

static struct vip_kcache_list *VIP_Add_Klist(void *vip_priv, void *ptr)
{
	struct vip_dev *vip = vip_priv;
	struct vip_kcache_list *klist_head = &vip->klist_head;
	struct vip_kcache_list *klist;

	klist = (struct vip_kcache_list *)kmem_cache_zalloc(vip->klist_cache, GFP_KERNEL);
	if (klist == NULL) {
		vipm_err("NULL klist\n");
		return NULL;
	}

	klist->vaddr = ptr;
	INIT_LIST_HEAD(&klist->list);
	list_add_tail(&klist->list, &klist_head->list);
	vipm_debug_mem("klist created : 0x%p\n", klist);
	return klist;
}

static void VIP_Destroy_Cache(void *vip_priv)
{
	struct vip_dev *vip = vip_priv;

	if (vip && (vip->kcache != NULL)) {
		struct vip_kcache_list *klist, *t;
		int num = 0;

		list_for_each_entry_safe_reverse(klist, t,
										&vip->klist_head.list,
										list) {
			kmem_cache_free(vip->kcache, klist->vaddr);
		//	list_del(&klist->list);
			kmem_cache_free(vip->klist_cache, klist);
			num++;
		}
		INIT_LIST_HEAD(&vip->klist_head.list);
		vipm_debug_mem("klist[%d] deleted\n", num);

		kmem_cache_destroy(vip->klist_cache);
		vipm_debug_mem("klist_cache destroyed : 0x%p\n", vip->kcache);
		kmem_cache_destroy(vip->kcache);
		vipm_debug_mem("kcache destroyed : 0x%p\n", vip->kcache);
		vip->klist_cache = NULL;
		vip->kcache = NULL;
	}
}

static int VIP_Create_Cache(void *vip_priv)
{
	struct vip_dev *vip = vip_priv;
	struct kmem_cache *cache;

	if (vip->kcache == NULL) {
		/* Step 1 */
		INIT_LIST_HEAD(&vip->klist_head.list);

		/* Step 2 */
		cache = kmem_cache_create("vip_klist_cache",
					    		sizeof(struct vip_kcache_list),
					    		0,
					    		SLAB_HWCACHE_ALIGN,
					    		NULL);
		if (cache == NULL) {
			vipm_err("klist_cache not created\n");
			goto out;
		}
		vip->klist_cache = cache;
		vipm_debug_mem("klist_cache created : 0x%p\n", cache);

		/* Step 3 */
		cache = kmem_cache_create("vip_kmem_cache",
					    		vip->buf_max,
					    		vip->buf_align,
					    		SLAB_HWCACHE_ALIGN,
					    		NULL);
		if (cache == NULL) {
			vipm_err("kcache not created\n");
			goto out;
		}
		vip->kcache = cache;
		vipm_debug_mem("kcache created : 0x%p\n", cache);
		return 0;
	}

out:
	return -ENOMEM;
}

static void VIP_Free_Cache(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = (struct vip_buf *)buf_priv;

	if (buf) {
		/*
		if (!atomic_dec_and_test(&buf->refcount)) {
			return;
		}
		*/

		if (buf->vaddr != NULL) {
			struct vip_kcache_list *klist;

			VIP_Put_Phys(vip_priv, buf);

			klist = VIP_Srch_Klist(vip, buf->vaddr);
			if (klist == NULL) {
				vipm_err("kmem_cache_free failed : 0x%p\n", buf->vaddr);
			} else {
				kmem_cache_free(vip->kcache, buf->vaddr);
				VIP_Del_Klist(vip, klist);
				vipm_debug_mem("kmem_cache_free done : 0x%p\n", buf->vaddr);
			}
		}
		kfree(buf);
	}
}

static void *VIP_Alloc_Cache(void *vip_priv, size_t s, gfp_t gfp)
{
	if (s) {
		struct vip_dev *vip = vip_priv;
		struct vip_buf *buf;
		struct vip_kcache_list *klist;
		void *ptr;

		if (vip->kcache == NULL) {
			vipm_err("No cache created\n");
			goto out;
		}

		if (s > VIP_MAX_IMG_SIZE) {
			vipm_err("Too big size : s[%d] > max[%d]\n",
					s, VIP_MAX_IMG_SIZE);
			goto out;
		}

		buf = kzalloc(sizeof(*buf), GFP_KERNEL);
		if (buf == NULL) {
			vipm_err("NULL vip_buf\n");
			return NULL;
		}
		buf->size = s;
		buf->gfp = GFP_KERNEL | GFP_DMA | gfp;

		ptr = kmem_cache_alloc(vip->kcache, buf->gfp);
		if (ptr == NULL) {
			kfree(buf);
			vipm_err("kmem_cache_alloc failed\n");
			goto out;
		}
		vipm_debug_mem("kmem_cache_alloc done : 0x%p\n", ptr);

		klist = VIP_Add_Klist(vip_priv, ptr);
		if (klist == NULL) {
			kmem_cache_free(vip->kcache, ptr);
			kfree(buf);
			goto out;
		}

		buf->vaddr = ptr;
		buf->paddr = VIP_Get_Phys(vip_priv, buf);
		if (buf->paddr == NULL) {
			kmem_cache_free(vip->kcache, ptr);
			VIP_Del_Klist(vip_priv, klist);
			kfree(buf);
			goto out;
		}
		vip_set_bit(VIP_BUF_FLAG_CACHE, &buf->flags);

		VIP_Mem_Handler(buf, VIP_Free_Cache);
	//	atomic_inc(&buf->refcount);
		return buf;
	}

out:
	return NULL;
}

static void VIP_Mem_Put_Int_Buf(struct vip_dev *vip)
{
	struct vip_int_buf *vint_buf = &vip->vint_buf;
	if (atomic_dec_and_test(&vint_buf->refcount)) {
		iounmap(vint_buf->imgv);
		vint_buf->imgv = NULL;
	}
}

static void *VIP_Mem_Get_Int_Buf(struct vip_dev *vip,
										unsigned long size)
{
	struct vip_int_buf *vint_buf = &vip->vint_buf;

	if (vint_buf->imgv == NULL) {
		vint_buf->imgv = (void *)ioremap_nocache((phys_addr_t)vint_buf->imgb, size);
		if (vint_buf->imgv == NULL) {
			goto out;
		}
		atomic_set(&vint_buf->refcount, 0);
	}
	atomic_inc(&vint_buf->refcount);

out:
	return (void *)vint_buf->imgv;
}

static void VIP_IOunmap(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = (struct vip_buf *)buf_priv;

	if (buf) {
		/*
		if (!atomic_dec_and_test(&buf->refcount)) {
			return;
		}
		*/
		if (buf->paddr == (void *)vip->vint_buf.imgb) {
			VIP_Mem_Put_Int_Buf(vip);
		} else {
			iounmap(buf->vaddr);
		}
		vipm_debug_mem("iounmap : vaddr[0x%p]\n", buf->vaddr);
		kfree(buf);
	}
}

static void *VIP_IOremap(void *vip_priv,
						unsigned long paddr,
						size_t s,
						int io)
{
	if (s) {
		struct vip_dev *vip = vip_priv;
		struct vip_buf *buf;

		io = !!io;

		buf = kzalloc(sizeof(*buf), GFP_KERNEL);
		if (buf == NULL) {
			vipm_err("NULL vip_buf\n");
			return NULL;
		}
		buf->size = s;
		buf->paddr = (void *)paddr;

		if (paddr == (unsigned long)vip->vint_buf.imgb) {
			buf->vaddr = VIP_Mem_Get_Int_Buf(vip, VIP_MAX_IMG_SIZE);
			buf->size = VIP_MAX_IMG_SIZE;
		} else {
			buf->vaddr = (void *)ioremap_nocache((phys_addr_t)paddr, s);
			buf->size = s;
		}
		if (buf->vaddr == NULL) {
			vipm_err("NULL ioremap\n");
			kfree(buf);
			return NULL;
		}
		vipm_debug_mem("ioremap : vaddr[0x%p] paddr[0x%p]\n",
					buf->vaddr, buf->paddr);

		vip_set_bit(VIP_BUF_FLAG_IOREMAP, &buf->flags);

		VIP_Mem_Handler(buf, VIP_IOunmap);
	//	atomic_inc(&buf->refcount);
		return buf;
	}
	return NULL;
}

static struct vm_area_struct *VIP_Vma_Get(struct vm_area_struct *vma)
{
	struct vm_area_struct *vma_copy;

	vma_copy = kmalloc(sizeof(*vma_copy), GFP_KERNEL);
	if (vma_copy == NULL) {
		return NULL;
	}

	if ((vma->vm_ops != NULL) &&
		(vma->vm_ops->open != NULL)) {
		vma->vm_ops->open(vma);
	}

	if (vma->vm_file != NULL) {
		get_file(vma->vm_file);
	}

	memcpy(vma_copy, vma, sizeof(*vma));

	vma_copy->vm_mm = NULL;
	vma_copy->vm_next = NULL;
	vma_copy->vm_prev = NULL;

	return vma_copy;
}

static void VIP_Vma_Put(struct vm_area_struct *vma)
{
	if (vma == NULL)
		return;

	if ((vma->vm_ops != NULL) &&
		(vma->vm_ops->close != NULL)) {
		vma->vm_ops->close(vma);
	}

	if (vma->vm_file != NULL) {
		fput(vma->vm_file);
	}

	kfree(vma);
}

static int VIP_Get_Cont_Userptr(struct vm_area_struct **r_vma,
								dma_addr_t *r_pa,
								unsigned long vaddr,
								unsigned long size)
{
	struct mm_struct *vip_mm = current->mm;
	struct vm_area_struct *vip_vma;
	dma_addr_t vip_pa = 0;
	unsigned long pfn_this, pfn_prev;
	unsigned long m_offset, m_start, m_end;
	int ret = 0;

	m_start = vaddr;
	m_offset = m_start & ~PAGE_MASK;
	m_end = m_start + size;

	vip_vma = find_vma(vip_mm, m_start);
	if ((vip_vma == NULL) || (vip_vma->vm_end < m_end)) {
		if (vip_vma != NULL) {
			vipm_err("vma[0x%p] : vma->end[0x%lx] < end[0x%lx]\n",
					vip_vma, vip_vma->vm_end, m_end);
		} else {
			vipm_err("NULL vma\n");
		}
		return -EFAULT;
	}

	for (pfn_prev = 0; m_start < m_end; m_start += PAGE_SIZE) {
		ret = follow_pfn(vip_vma, m_start, &pfn_this);
		if (ret) {
			return ret;
		}

		if (pfn_prev == 0) {
			vip_pa = pfn_this << PAGE_SHIFT;
		} else if (pfn_this != (pfn_prev + 1)) {
			return -EFAULT;
		}

		pfn_prev = pfn_this;
	}

	if (r_vma) {
		*r_vma = VIP_Vma_Get(vip_vma);
		if ((*r_vma) == NULL)
			return -ENOMEM;
	}

	*r_pa = vip_pa + m_offset;
	return 0;
}

#if (__VIP_PHYS_PAGE_TO_PHYS > 0)
static void VIP_Userptr_Put_Phys(void *vip_priv, void *buf_priv)
{

}

static void *VIP_Userptr_Get_Phys(void *vip_priv, void *buf_priv)
{
//	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = buf_priv;
	void *paddr = NULL;
	VIP_Chk_Contig_Pages(buf);
	paddr = (void *)page_to_phys(buf->pages[0]);
	return paddr;
}
#elif (__VIP_PHYS_DMA_MAP_PAGE > 0)
static void VIP_Userptr_Put_Phys(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (buf->paddr != NULL) {
		struct vip_dev *vip = vip_priv;
		int dir = (buf->write)? DMA_FROM_DEVICE : DMA_TO_DEVICE;
		dma_addr_t *phys = (dma_addr_t *)buf->priv;
		int i;

		for (i=0 ; i<buf->n_pages ; i++) {
			dma_unmap_page(vip->parent,
							phys[i],
							PAGE_SIZE,
							dir);
		}
		kfree(phys);
		buf->priv = NULL;
	}
}

static void *VIP_Userptr_Get_Phys(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = buf_priv;
	int dir = (buf->write)? DMA_FROM_DEVICE : DMA_TO_DEVICE;
	dma_addr_t *phys;
	void *paddr = NULL;
	int i;

	phys = kzalloc(sizeof(dma_addr_t) * buf->n_pages, GFP_KERNEL);
	if (phys != NULL) {
		for (i=0 ; i<buf->n_pages ; i++) {
			phys[i] = dma_map_page(vip->parent,
								buf->pages[i],
								0,
								PAGE_SIZE,
								dir);
			if (i && ((phys[i-1] + PAGE_SIZE) != phys[i]))  {
				vip_set_bit(VIP_BUF_FLAG_NOT_CONTIG, &buf->flags);
			}
		#if 0
			vipm_debug_mem("dma_addr[0x%08x] <- page[0x%p]\n",
						phys[i],
						buf->pages[i]);
		#endif
		}
		paddr = (void *)phys[0];
		if (paddr == NULL) {
			kfree(phys);
			return NULL;
		}
	}
	buf->priv = phys;
	return paddr;
}
#else
#define VIP_Userptr_Put_Phys	VIP_Put_Phys
#define VIP_Userptr_Get_Phys	VIP_Get_Phys
#endif

static void VIP_Userptr_Put(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = (struct vip_buf *)buf_priv;
	unsigned long vaddr;
	unsigned int i;

	if (buf == NULL) {
		return;
	}

	/*
	if (!atomic_dec_and_test(&buf->refcount)) {
		return;
	}
	*/

	down_read(&current->mm->mmap_sem);

	buf = (struct vip_buf *)buf_priv;
	vaddr = (unsigned long)buf->vaddr & PAGE_MASK;

	if (buf->pages != NULL) {
		VIP_Userptr_Put_Phys(vip_priv, buf);

		if (vaddr) {
			vm_unmap_ram((void *)vaddr, buf->n_pages);
		}
		for (i = 0; i < buf->n_pages; ++i) {
			if (buf->write) {
				set_page_dirty_lock(buf->pages[i]);
			}
			put_page(buf->pages[i]);
		}
		kfree(buf->pages);
	} else {
		if (buf->vma != NULL) {
			VIP_Vma_Put(buf->vma);
		}
		iounmap(buf->vaddr);
	}
	kfree(buf);

	up_read(&current->mm->mmap_sem);
}

static void *VIP_Userptr_Get(void *vip_priv,
									unsigned long vaddr,
								  	unsigned long size,
									int write,
									int io)
{
	struct vip_buf *buf;
	unsigned long first, last;
	int n_pages, offset;
	struct vm_area_struct *vma;
	dma_addr_t physp;

	if (!size) {
		return NULL;
	}

	io = !!io;

	down_read(&current->mm->mmap_sem);

	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	if (buf == NULL) {
		goto out;
	}

	buf->user_ptr = vaddr;

	buf->write = write;
	buf->size = size;

	offset = vaddr & ~PAGE_MASK;

	vma = find_vma(current->mm, vaddr);
	if (vma && (vma->vm_flags & VM_PFNMAP) && (vma->vm_pgoff)) {
		vipm_debug_mem("use vip_get_contig_userptr\n");
		if (VIP_Get_Cont_Userptr(&vma, &physp, vaddr, size)) {
			goto out_pages_array_alloc;
		}
		buf->vma = vma;
		buf->vaddr = ioremap_nocache(physp, size);
		if (buf->vaddr == NULL) {
			goto out_pages_array_alloc;
		}
		if (!io) {
			buf->paddr = (void *)physp;
			buf->paddr += offset;
		}
	} else {
		vipm_debug_mem("use get_user_pages\n");
		first = vaddr >> PAGE_SHIFT;
		last = (vaddr + size - 1) >> PAGE_SHIFT;

		buf->n_pages = last - first + 1;
		buf->pages = kzalloc(buf->n_pages * sizeof(struct page *),
							GFP_KERNEL);
		if (buf->pages == NULL) {
			goto out_pages_array_alloc;
		}

		n_pages = get_user_pages(current, current->mm,
								vaddr & PAGE_MASK,
								buf->n_pages,
								write,
								1,	/* do force */
								buf->pages,
								NULL);

		if (n_pages != buf->n_pages) {
			goto out_get_user_pages;
		}
		buf->vaddr = vm_map_ram(buf->pages, buf->n_pages, -1,
								PAGE_KERNEL);
		if (buf->vaddr == NULL) {
			goto out_get_user_pages;
		}
		if (!io) {
			buf->paddr = VIP_Userptr_Get_Phys(vip_priv, buf);
			if (buf->paddr == NULL) {
				vm_unmap_ram((void *)(vaddr & PAGE_MASK), buf->n_pages);
				buf->vaddr = NULL;
				goto out_get_user_pages;
			}
			buf->paddr += offset;
		}
	}
	buf->vaddr += offset;

	vip_set_bit(VIP_BUF_FLAG_USERPTR, &buf->flags);

	VIP_Mem_Handler(buf, VIP_Userptr_Put);
//	atomic_inc(&buf->refcount);

	up_read(&current->mm->mmap_sem);

	vipm_debug_mem("vma       : 0x%p\n", vma);
	if (vma) {
		vipm_debug_mem(" vm_start      : 0x%lx\n", vma->vm_start);
		vipm_debug_mem(" vm_end        : 0x%lx\n", vma->vm_end);
		vipm_debug_mem(" vm_next       : 0x%p\n", vma->vm_next);
		vipm_debug_mem(" vm_prev       : 0x%p\n", vma->vm_prev);
		vipm_debug_mem(" vm_page_prot  : 0x%lx\n", (unsigned long)vma->vm_page_prot);
		vipm_debug_mem(" vm_flags      : 0x%lx\n", vma->vm_flags);
		vipm_debug_mem(" anon_vma      : 0x%p\n", vma->anon_vma);
	}

	vipm_debug_mem("buf       : 0x%p\n", buf);
	vipm_debug_mem(" vaddr    : 0x%p\n", buf->vaddr);
	vipm_debug_mem(" paddr    : 0x%p\n", buf->paddr);
	vipm_debug_mem(" user_ptr : 0x%lx\n", buf->user_ptr);
	vipm_debug_mem(" pages    : 0x%p\n", buf->pages);
	vipm_debug_mem(" write    : %d\n", buf->write);
	vipm_debug_mem(" size     : %ld\n", buf->size);
	vipm_debug_mem(" n_pages  : %d\n", buf->n_pages);
	vipm_debug_mem(" flags    : 0x%lx\n", buf->flags);

	return buf;

out_get_user_pages:
	vipm_debug_mem("get_user_pages requested/got: %d/%d]\n", n_pages,
					buf->n_pages);
	while (--n_pages >= 0) {
		put_page(buf->pages[n_pages]);
	}
	kfree(buf->pages);

out_pages_array_alloc:
	kfree(buf);

out:
	up_read(&current->mm->mmap_sem);
	return NULL;
}

static void VIP_Free_Top(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (buf) {
		struct vip_buf_handler *handler = &buf->handler;
		if (VIP_Chk_Buf_Usage(buf)) {
			vipm_err("Can't release : buf[0x%p] is used!, flags[0x%lx]\n",
					buf, buf->flags);
			return;
		}
		if (handler->put) {
			handler->put(vip_priv, buf);
		}
	}
}

static void *VIP_Get_Uptr(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (buf->user_ptr == 0) {
		vipm_err("Zero user_ptr\n");
	}
	return (void *)buf->user_ptr;
}

static void *VIP_Get_Vaddr(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (buf->vaddr == NULL) {
		vipm_err("NULL vaddr\n");
	}
	return buf->vaddr;
}

static void *VIP_Get_Paddr(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (buf->paddr == NULL) {
		vipm_err("NULL paddr\n");
	}
	return buf->paddr;
}

static unsigned long VIP_Get_Vsize(void *vip_priv, void *buf_priv)
{
	struct vip_buf *buf = buf_priv;

	if (!buf->size) {
		vipm_err("Zero size\n");
	}
	return buf->size;
}

static void VIP_Free_Buf(void *vip_priv, void *buf_priv)
{
	struct vip_dev *vip = vip_priv;
	struct vip_buf *buf = buf_priv;
	VIP_Mem_Free(vip, buf->bsrc);
	VIP_Mem_Free(vip, buf);
}

static struct vip_buf *VIP_Alloc_Buf_Kern(struct vip_dev *vip,
										unsigned long size,
										unsigned long flags)
{
	struct vip_buf *buf_kern = NULL;

	if (flags & VIP_IO_FLAG_MASK_INT_FIX) {
		buf_kern = VIP_Mem_IOremap(vip,
								(unsigned long)vip->vint_buf.imgb,
								size,
								0);
		if (buf_kern == NULL) {
			vipm_err("(kern) vip_mem_ioremap error\n");
		}
	}
	else {
		int cache = !!(flags & VIP_IO_FLAG_MASK_INT_CACHE);
		buf_kern = VIP_Mem_Alloc(vip, size, 0, cache);
		if (buf_kern == NULL) {
			vipm_err("(kern) vip_mem_alloc error\n");
		}
	}
	return buf_kern;
}

static int VIP_Buf_Kern_Required(struct vip_dev *vip,
								struct vip_buf *buf)
{
	unsigned int offset;
	int ret = 0;

	if (vip_test_bit(VIP_BUF_FLAG_NOT_CONTIG, &buf->flags)) {
		vipm_debug_mem("(user) non-contig addr\n");
		vipm_debug_mem("       kernel buffer required\n");
		ret |= VIP_BUF_UPTR_STATE_MASK_NOT_CONTIG;
	}
	offset = (unsigned int)buf->vaddr & (vip->buf_align-1);
	if (offset) {
		vip_set_bit(VIP_BUF_FLAG_NOT_ALIGNED, &buf->flags);
		vipm_debug_mem("(user) addr offset[0x%08x]\n", offset);
		vipm_debug_mem("       kernel buffer required\n");
		ret |= VIP_BUF_UPTR_STATE_MASK_NOT_ALIGNED;
	}
	return ret;
}

static struct vip_buf *VIP_Buf_No_Conv(struct vip_dev *vip,
										struct vip_io_buffer *buffer)
{
	struct vip_buf *buf;

	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	if (buf == NULL) {
		goto out;
	}
	buf->user_ptr = buffer->addr;
	buf->size = buffer->size;
	buf->write = !!(buffer->flags & VIP_IO_FLAG_MASK_WRITE);

	vip_set_bit(VIP_BUF_FLAG_NO_CONV, &buf->flags);

out:
	return buf;
}

static void *VIP_Alloc_Buf(void *vip_priv,
									void *buffer_priv,
									int input)
{
	struct vip_dev *vip = vip_priv;
	struct vip_io_buffer *buffer = buffer_priv;
	struct vip_buf *buf_kern = NULL;
	struct vip_buf *buf_user = NULL;
	unsigned long buff_flag = buffer->flags;
	int buf_kern_required = 0;
	int write, io;

	spin_lock(vip->lock);

	if (!buffer->addr || !buffer->size) {
		vipm_err("Invalid param : addr[0x%lx], size[%ld]\n",
				buffer->addr, buffer->size);
		goto out;
	}

	write = !!(buff_flag & VIP_IO_FLAG_MASK_WRITE);
	io = !!(buff_flag & VIP_IO_FLAG_MASK_IO);

	/* use internal buffer */
	if (buff_flag & VIP_IO_FLAG_MASK_USER) {
		if (buff_flag & VIP_IO_FLAG_MASK_NO_CONV) {
			buff_flag &= ~VIP_IO_FLAG_MASK_INT_FIX;
			buff_flag |= VIP_IO_FLAG_MASK_INT_NO;
			buffer->flags = buff_flag;
		}
	}
	if (buff_flag & VIP_IO_FLAG_MASK_INT_FIX) {
		if (vip->vint_buf.imgb == NULL) {
			buff_flag &= ~VIP_IO_FLAG_MASK_INT_FIX;
			buffer->flags = buff_flag;
		}
	}

	if (buff_flag & VIP_IO_FLAG_MASK_INT_NO) {
		buff_flag &= ~VIP_IO_FLAG_MASK_INT;
		buffer->flags = buff_flag;
	} else {
		if ((buff_flag & VIP_IO_FLAG_MASK_IO) ||
			(buff_flag & VIP_IO_FLAG_MASK_INT_FIX) ||
			(buff_flag & VIP_IO_FLAG_MASK_INT_CACHE)) {
			/* If source buffer is io, internal copy required */
			buff_flag |= VIP_IO_FLAG_MASK_INT;
			buffer->flags = buff_flag;
		}
	}

	if (buff_flag & VIP_IO_FLAG_MASK_USER) {
		if (buff_flag & VIP_IO_FLAG_MASK_NO_CONV) {
			buf_user = VIP_Buf_No_Conv(vip, buffer);
			if (buf_user == NULL) {
				vipm_err("(user) VIP_Buf_No_Conv error\n");
				goto out;
			}
			goto skip_conv;
		}

		buf_user = VIP_Mem_Userptr_Get(vip,
								buffer->addr,
								buffer->size,
								write,	/* write(1)/read(0) */
								io);	/* io(1)/buffer(0) */
		if (buf_user == NULL) {
			vipm_err("(user) vip_mem_userptr_get error\n");
			goto out;
		}
		buf_kern_required = VIP_Buf_Kern_Required(vip, buf_user);
	}
	else if (buff_flag & VIP_IO_FLAG_MASK_KERN_ADDR) {
		if (buff_flag & VIP_IO_FLAG_MASK_PADDR) {
			buf_user = VIP_Mem_IOremap(vip,
									(unsigned long)buffer->addr,
									buffer->size,
									io);
			if (buf_user == NULL) {
				vipm_err("(user) vip_mem_ioremap error\n");
				goto out;
			}
			buf_user->write = write;
		//	vip_set_bit(VIP_BUF_FLAG_PADDR, &buf_user->flags);
			buf_kern_required = VIP_Buf_Kern_Required(vip, buf_user);
		} else {
			buf_user = kzalloc(sizeof(struct vip_buf), GFP_KERNEL);
			if (buf_user == NULL) {
				vipm_err("(user) kzalloc error\n");
				goto out;
			}
			buf_user->write = write;
			buf_user->size = buffer->size;
			buf_user->vaddr = (void *)buffer->addr;
			if (!io) {
				VIP_Chk_Contig_Vaddr(buf_user);

				buf_kern_required = VIP_Buf_Kern_Required(vip, buf_user);
				if (!buf_kern_required) {
					buf_user->paddr = VIP_Get_Phys(vip, buf_user);
					if (buf_user->paddr == NULL) {
						VIP_Mem_Free(vip, buf_user);
						goto out;
					}
				}
			}
			vip_set_bit(VIP_BUF_FLAG_VADDR, &buf_user->flags);
			VIP_Mem_Handler(buf_user, VIP_Free_Mem);
		}
	}
	else {
		vipm_err("Unknown flag : buffer->flags[0x%lx]\n",
				buff_flag);
		goto out;
	}

skip_conv:
	if (io) {
		vip_set_bit(VIP_BUF_FLAG_IO, &buf_user->flags);
	}

	if (!(buff_flag & VIP_IO_FLAG_MASK_INT_NO)) {
		if (buf_kern_required) {
			if (!(buff_flag & VIP_IO_FLAG_MASK_INT)) {
				if (vip->vint_buf.imgb != NULL) {
					buff_flag |= VIP_IO_FLAG_MASK_FIX;
				} else {
				//	buff_flag |= VIP_IO_FLAG_MASK_CACHE;
					buff_flag |= VIP_IO_FLAG_MASK_INT;
				}
				buffer->flags = buff_flag;
			}
		}

		if (buff_flag & VIP_IO_FLAG_MASK_INT) {
			buf_kern = VIP_Alloc_Buf_Kern(vip, buffer->size, buff_flag);
			if (buf_kern == NULL) {
				VIP_Mem_Free(vip, buf_user);
				goto out;
			}
		}
	}

	if (buf_kern) {
		buf_kern->bsrc = (void *)buf_user;
		VIP_Rst_Buf_Usage(buf_user);
	} else {
		buf_kern = buf_user;
		buf_kern->bsrc = NULL;
	}
	VIP_Rst_Buf_Usage(buf_kern);
	vipm_debug_mem("buf_kern[0x%p] buf_kern->bsrc[0x%p]\n",
				buf_kern, buf_kern->bsrc);

out:
	spin_unlock(vip->lock);
	return buf_kern;
}

static const struct vip_mem_ops _g_vip_memops = {
	.destroy_cache	= VIP_Destroy_Cache,
	.create_cache	= VIP_Create_Cache,
	/* */
	.alloc			= VIP_Alloc_Mem,
	.alloc_cache	= VIP_Alloc_Cache,
	/* */
	.iomap			= VIP_IOremap,
	/* */
	.userptr_get	= VIP_Userptr_Get,
	/* */
	.get_uptr		= VIP_Get_Uptr,
	.get_vaddr		= VIP_Get_Vaddr,
	.get_paddr		= VIP_Get_Paddr,
	.get_vsize		= VIP_Get_Vsize,
	/* */
	.free			= VIP_Free_Top,
	/* */
	.alloc_buf		= VIP_Alloc_Buf,
	.free_buf		= VIP_Free_Buf,
};

void VIP_Get_Mem_Ops(struct vip_dev *vip)
{
	vip->vmem_ops = (struct vip_mem_ops *)&_g_vip_memops;
}

#if (__VIP_BUFFER_FLUSH_ON > 0)
void VIP_Buf_Flush(unsigned long vaddr, unsigned long size)
{
#if 0
	unsigned long vaddr_aligned;
	unsigned long first, last;
	unsigned long n_pages;
	int i;

	if (!vaddr) {
		return;
	}

	vaddr_aligned = vaddr & PAGE_MASK;
	first = vaddr >> PAGE_SHIFT;
	last = (vaddr + size - 1) >> PAGE_SHIFT;

	n_pages = last - first + 1;

	for (i=0 ; i<n_pages ; i++) {
		flush_kernel_dcache_page(virt_to_page(vaddr_aligned));
		vaddr_aligned += PAGE_SIZE;
	}
#else
	flush_kernel_vmap_range((void *)vaddr, (int)size);
#endif
	vipm_debug_mem("Flush vaddr : 0x%lx[0x%lx]\n",
				vaddr, size);
}
#else
void VIP_Buf_Flush(unsigned long vaddr, unsigned long size)
{

}
#endif

struct vip_fmt_list *VIP_Find_Fmt_List(struct vip_dev *vip,
											int func, int index)
{
	struct vip_fmt_list *fmt_list = NULL;

	list_for_each_entry(fmt_list, &vip->ifmt_list_head, list) {
		if ((fmt_list->format.func == func) &&
			(fmt_list->format.index == index)) {
			return fmt_list;
		}
	}
	return NULL;
}

static void VIP_Free_Fmt_List(struct vip_fmt_list *fmt_list)
{
	if (fmt_list) {
		vipm_debug_mem("Format entry deleted : 0x%p\n", fmt_list);
		kfree(fmt_list);
	}
}

void VIP_Clr_Fmt_List(struct vip_fmt_list *fmt_list)
{
	if (fmt_list) {
		list_del(&fmt_list->list);
		VIP_Free_Fmt_List(fmt_list);
	}
}

struct vip_fmt_list *VIP_Add_Fmt_List(struct vip_dev *vip)
{
	struct vip_fmt_list *fmt_list;

	fmt_list = (struct vip_fmt_list *)kzalloc(sizeof(struct vip_fmt_list), GFP_KERNEL);
	if (fmt_list == NULL) {
		vipm_err("NULL fmt_list\n");
		return NULL;
	}
	INIT_LIST_HEAD(&fmt_list->list);
	list_add_tail(&fmt_list->list, &vip->ifmt_list_head);
	VIP_Rst_Fmt_Usage(fmt_list);
	vipm_debug_mem("Format entry created : 0x%p\n", fmt_list);

	return fmt_list;
}

static const char *g_vip_mem_buf_name[2] = {
	[VIP_BUF_OUTPUT]	= "output",
	[VIP_BUF_INPUT]		= "input",
};

static void VIP_Clr_Buf_List(struct vip_buf *buf,
							int input)
{
	if (buf) {
		list_del(&buf->list);
		vipm_debug_mem("%s entry deleted : 0x%p\n",
				prt_names(input, g_vip_mem_buf_name),
				buf);
	}
}

static void VIP_Add_Buf_List(struct vip_dev *vip,
									struct vip_buf *buf,
									int input)
{
	struct list_head *buf_head;

	buf_head = (input == VIP_BUF_INPUT)?	\
				&vip->ibuf_head : &vip->obuf_head;

	INIT_LIST_HEAD(&buf->list);
	list_add_tail(&buf->list, buf_head);
	vipm_debug_mem("%s entry created\n",
			prt_names(input, g_vip_mem_buf_name));
}

struct vip_buf *VIP_Find_Buf(struct vip_dev *vip,
									int func, int index,
									int input)
{
	struct list_head *buf_head;
	struct vip_buf *buf;

	buf_head = (input == VIP_BUF_INPUT)?	\
				&vip->ibuf_head : &vip->obuf_head;

	list_for_each_entry(buf, buf_head, list) {
		if ((buf->func == func) &&
			(buf->index == index)) {
			return buf;
		}
	}
	return NULL;
}

void VIP_Clr_Buf(struct vip_dev *vip,
					struct vip_buf *buf,
					int input)
{
	if (buf) {
		vipm_debug_mem("Free %s buf[0x%p]\n",
				prt_names(input, g_vip_mem_buf_name),
				buf);
		VIP_Clr_Buf_List(buf, input);
		VIP_Mem_Free_Buf(vip, buf);
	}
}

struct vip_buf *VIP_Add_Buf(struct vip_dev *vip,
								struct vip_io_buffer *buffer,
								int input)
{
	struct vip_buf *buf;

	buf = VIP_Mem_Alloc_Buf(vip, buffer, input);
	if (buf == NULL) {
		return NULL;
	}
	buf->func = buffer->func;
	buf->index = buffer->index;
	vipm_debug_mem("buf func[%d] index[%d]\n",
					buf->func, buf->index);

	VIP_Add_Buf_List(vip, buf, input);

	{
		vipm_debug_mem("[%s info]\n", prt_names(input, g_vip_mem_buf_name));
		vipm_debug_mem("  list[0x%p] prev[0x%p] next[0x%p]\n",
				&buf->list, buf->list.prev, buf->list.next);
	}
	return buf;
}

int VIP_Chk_Buf(struct vip_buf *buf, int input)
{
	int ret = 0;

	if (buf->user_ptr && buf->size) {
		int type = (input == VIP_BUF_OUTPUT)? VERIFY_WRITE : VERIFY_READ;

		ret = !access_ok(type, (void __user *)buf->user_ptr, buf->size);
		if (ret) {
			vipm_err("Can't access buf->user_ptr[0x%lx], type[%d]\n",
					buf->user_ptr, type);
		}
	}
	return ret;
}

#if 0
#define	__vipm_debug_time_mem(_flag, _idx, _size, _start, _end)		\
	do {														\
		vipm_err("[%d] size[0x%x] elapsed[%lld us (%lld ms)] (%lld ms ~ %lld ms)\n",	\
					_idx, _size,								\
					ktime_to_us(ktime_sub(_end, _start)),		\
					ktime_to_ms(ktime_sub(_end, _start)),		\
					ktime_to_ms(_start), ktime_to_ms(_end));	\
	} while(0)
#else
#define	__vipm_debug_time_mem(_flag, _idx, _size, _start, _end)		\
	do {														\
		vipm_debug(_flag, "[%d] size[0x%x] elapsed[%lld us (%lld ms)] (%lld ms ~ %lld ms)\n",	\
					_idx, _size,								\
					ktime_to_us(ktime_sub(_end, _start)),		\
					ktime_to_ms(ktime_sub(_end, _start)),		\
					ktime_to_ms(_start), ktime_to_ms(_end));	\
	} while(0)
#endif

int VIP_Copy_Input_Buf(struct vip_req *areq)
{
	ktime_t start_t, end_t;
	struct vip_dev *vip = areq->vip;
	struct vip_buf *buf_kern;
	struct vip_buf *buf_user;
	void *addr_dst, *addr_src;
	unsigned int size_dst, size_src;
	int ret = 0;

	buf_kern = areq->ibuf;
	buf_user = buf_kern->bsrc;

	size_dst = VIP_Mem_Get_Vsize(vip, buf_kern);
	addr_dst = VIP_Mem_Get_Vaddr(vip, buf_kern);

	if (buf_user != NULL) {
		unsigned int size;
		int io;
		int from_user;

		io = !!vip_test_bit(VIP_BUF_FLAG_IO, &buf_user->flags);
	#if defined(_VIP_MEM_USE_COPY_USER)
		from_user = 1;
	#else
		from_user = !!vip_test_bit(VIP_BUF_FLAG_NO_CONV, &buf_user->flags);
	#endif

		size_src = VIP_Mem_Get_Vsize(vip, buf_user);
		if (from_user) {
			addr_src = VIP_Mem_Get_Uptr(vip, buf_user);
		} else {
			addr_src = VIP_Mem_Get_Vaddr(vip, buf_user);
			VIP_Buf_Flush((unsigned long)addr_src, (unsigned long)size_src);
		}
		size = min(size_dst, size_src);

		start_t = ktime_get();
	#if defined(VIP_USE_BUF_CPY_LOCK)
		{
			unsigned long flags;
			spin_lock_irqsave(&vip_buf_cpy_lock, flags);
			ret = VIP_Copy_From(addr_dst, addr_src, size, from_user, VIPM_NO_MSG_FLAG | io);
			spin_unlock_irqrestore(&vip_buf_cpy_lock, flags);
		}
	#else
		ret = VIP_Copy_From(addr_dst, addr_src, size, from_user, VIPM_NO_MSG_FLAG | io);
	#endif
		end_t = ktime_get();
		buf_kern->time_us = (unsigned int)ktime_to_us(ktime_sub(end_t, start_t));
		__vipm_debug_time_mem(VIPM_DBG_SET_MEM, areq->index, size, start_t, end_t);
	#if 0
		{
			unsigned int time_us = buf_kern->time_us;
			unsigned int s_us = ktime_to_us(start_t);
			unsigned int e_us = ktime_to_us(end_t);
			vipm_noti("  [%d] cp[%4ld.%06ld ~ %4ld.%06ld : %3ld.%03ld us] %s\n",
					areq->index,
					s_us/USEC_PER_SEC, s_us%USEC_PER_SEC,
					e_us/USEC_PER_SEC, e_us%USEC_PER_SEC,
					time_us/USEC_PER_MSEC, time_us%USEC_PER_MSEC,
					(time_us/USEC_PER_MSEC)? "*" : " ");
		}
	#endif
		if (ret) {
			goto out;
		}
	} else {
		vipm_debug_mem("[%d] Zero-copy[0x%p] area\n", areq->index, (void *)VIP_Mem_Get_Vsize(vip, buf_kern));
		buf_kern->time_us = 0;
	}

	VIP_Buf_Flush((unsigned long)addr_dst, (unsigned long)size_dst);

out:
	return ret;
}

static void VIP_Clr_Fmt_List_All(struct vip_dev *vip)
{
	struct list_head *fmt_head;
	struct vip_fmt_list *fmt_list, *t;
	int num = 0;

	fmt_head = &vip->ifmt_list_head;
	list_for_each_entry_safe_reverse(fmt_list, t, fmt_head, list) {
		VIP_Free_Fmt_List(fmt_list);
		num++;
	}
	INIT_LIST_HEAD(fmt_head);
	vipm_debug_mem("ifmt list[%d] cleared\n", num);
}

static void VIP_Clr_Buf_All(struct vip_dev *vip,
									int input)
{
	struct list_head *buf_head;
	struct vip_buf *buf, *t;
	char *name = (char *)prt_names(input, g_vip_mem_buf_name);
	int num = 0;

	buf_head = (input == VIP_BUF_INPUT)?	\
				&vip->ibuf_head : &vip->obuf_head;

	list_for_each_entry_safe_reverse(buf, t, buf_head, list) {
		vipm_debug_mem("Free %s buf[0x%p]\n", name, buf);
		VIP_Mem_Free_Buf(vip, buf);
		num++;
	}
	INIT_LIST_HEAD(buf_head);
	vipm_debug_mem("%s buf list[%d] cleared\n", name, num);
}

void VIP_Clr_List_And_Buf_All(struct vip_dev *vip)
{
	VIP_Clr_Fmt_List_All(vip);

	VIP_Clr_Buf_All(vip, VIP_BUF_INPUT);
	VIP_Clr_Buf_All(vip, VIP_BUF_OUTPUT);
}

void VIP_Clr_List_And_Buf(struct vip_dev *vip,
									int func, int index)
{
	struct vip_fmt_list *ifmt_list = NULL;
	struct vip_buf *ibuf = NULL;
	struct vip_buf *obuf = NULL;

	if (func == -1) {
		VIP_Clr_List_And_Buf_All(vip);
		return;
	}

	ifmt_list = VIP_Find_Fmt_List(vip, func, index);
	if (ifmt_list != NULL) {
		VIP_Clr_Fmt_List(ifmt_list);
	}

	ibuf = VIP_Find_Buf(vip, func, index, VIP_BUF_INPUT);
	if (ibuf != NULL) {
		VIP_Clr_Buf(vip, ibuf, VIP_BUF_INPUT);
	}

	obuf = VIP_Find_Buf(vip, func, index, VIP_BUF_OUTPUT);
	if (obuf != NULL) {
		VIP_Clr_Buf(vip, obuf, VIP_BUF_OUTPUT);
	}
}

void VIP_Init_List(struct vip_dev *vip)
{
	INIT_LIST_HEAD(&vip->ifmt_list_head);
	INIT_LIST_HEAD(&vip->ibuf_head);
	INIT_LIST_HEAD(&vip->obuf_head);
}

static void __used VIP_Init_Mem_Copy_Test(unsigned int size, int repeat, int opt)
{
	ktime_t start_t, end_t;
	void *dst = NULL, *src = NULL;
	int i;

	dst = (opt)? (void *)vmalloc(size) :
				(void *)kmalloc(size, GFP_KERNEL);
	if (dst == NULL) {
		vipm_err("NULL dst\n");
		return;
	}

	src = (opt)? (void *)vmalloc(size) :
				(void *)kmalloc(size, GFP_KERNEL);
	if (src == NULL) {
		if (opt) vfree(dst);
		else kfree(dst);
		vipm_err("NULL src\n");
		return;
	}

	vipm_err("memory copy test : %s, size 0x%08x, repeat %d\n",
			(opt)? "vmalloc" : "kmalloc", size, repeat);
	memset(dst, 0x5A, size);
	for (i=0 ; i<repeat ; i++) {
		memset(src, 0xA5, size);
		start_t = ktime_get();
		memcpy(src, dst, size);
		end_t = ktime_get();
		__vipm_debug_time(VIPM_DBG_SET_MEM, start_t, end_t);
	}

	if (opt) {
		vfree(src);
		vfree(dst);
	} else {
		kfree(src);
		kfree(dst);
	}
}

void VIP_Init_Mem_Test(void)
{
#if 0
	VIP_Init_Mem_Copy_Test(640*480*2, 10, 0);
	VIP_Init_Mem_Copy_Test(1920*1080*2, 10, 0);
	VIP_Init_Mem_Copy_Test(640*480*2, 10, 1);
	VIP_Init_Mem_Copy_Test(1920*1080*2, 10, 1);
#endif
}
