
/**
 * \defgroup hma_drv HMA user level interface driver
 *
 * 사용자 어플리케이션에서 HMA 메모리를 할당하고 반납하도록 도와주는 드라이버이다.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <asm/pgtable.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/version.h>

#include "hma_drv_ext.h"
#include "hma_alloc.h"
#include "hma_drv.h"


#define debug(fmt,args...)	do{ if (debug_level > 0) printk(KERN_DEBUG"%s.%d: "fmt,__func__,__LINE__,##args); }while(0)
#define info(fmt,args...)	printk(KERN_INFO"%s.%d: "fmt,__func__,__LINE__,##args)
#define error(fmt,args...)	printk(KERN_ERR"%s.%d: "fmt,__func__,__LINE__,##args)


static int hma_drv_major = 1819;

static int debug_level;
module_param_named (debug_drv, debug_level, int, 0640);


hma_drv_hook_open_t hma_drv_hook_open;
hma_drv_hook_ioctl_t hma_drv_hook_ioctl;
hma_drv_hook_release_t hma_drv_hook_release;
int hma_drv_priv_size = sizeof(struct hma_drv_mem_desc);
EXPORT_SYMBOL (hma_drv_hook_open);
EXPORT_SYMBOL (hma_drv_hook_ioctl);
EXPORT_SYMBOL (hma_drv_hook_release);
EXPORT_SYMBOL (hma_drv_priv_size);

static int hma_drv_open (struct inode *inode, struct file *file)
{
	struct hma_drv_mem_desc *desc;

	file->private_data = desc = kcalloc (1, hma_drv_priv_size, GFP_KERNEL);

	if (hma_drv_hook_open)
		hma_drv_hook_open (inode, file);

	debug ("open.. desc %p\n", desc);

	return 0;
}

static int hma_drv_release (struct inode *inode, struct file *file)
{
	struct hma_drv_mem_desc *desc = file->private_data;

	debug ("close.. desc %p\n", desc);

	if (hma_drv_hook_release)
		hma_drv_hook_release (inode, file);

	if (desc->paddr)
	{
		debug ("paddr %lx\n", desc->paddr);
		hma_free (desc->poolname[0]?desc->poolname:NULL, desc->paddr);
	}

	kfree (desc);

	return 0;
}

static int hma_drv_mmap (struct file *file, struct vm_area_struct *vm)
{
        struct hma_drv_mem_desc *desc;
        int ret = -EINVAL;
        int size = (int)(vm->vm_end - vm->vm_start);

        desc = file->private_data;

	/* 메모리가 두개의 page 사이에 걸쳐있을 수 있다 */
        if (size > desc->size + PAGE_SIZE*2)
                return -EINVAL;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0))
#ifdef VM_RESERVED
	vm->vm_flags |= VM_RESERVED;//to prevent swapout
#endif
#else
	vm->vm_flags |= VM_IO|VM_DONTEXPAND|VM_DONTDUMP;//to prevent swapout
#endif

        if (!desc->cached)
        {
#ifdef CONFIG_MMU
            vm->vm_page_prot = pgprot_noncached(vm->vm_page_prot);
#endif
            debug ("hma alloc noncached\n");
        }
        else
            debug ("hma alloc cached\n");

        ret = remap_pfn_range (vm, vm->vm_start,
                        desc->paddr >> PAGE_SHIFT,
                        size,
                        vm->vm_page_prot
                        );
        if (ret == 0)
                desc->vma = vm;

        return ret;
}

static long hma_drv_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct hma_ioctl_alloc alloc;
	struct hma_drv_mem_desc *desc = file->private_data;

	if (hma_drv_hook_ioctl)
	{
		long ret;

		ret = hma_drv_hook_ioctl (file, cmd, arg);
		if (ret >= 0)
			return ret;
	}

	debug ("cmd %08x\n", cmd);
	switch (cmd)
	{
	case HMA_IOCTL_ALLOC:
		debug ("alloc...\n");
		if (copy_from_user (&alloc, (void*)arg, sizeof(alloc)) != 0)
			return -EINVAL;

		alloc.paddr = hma_alloc (alloc.pool[0]?alloc.pool:NULL, alloc.size, alloc.align);
		if (alloc.paddr == 0)
			return -ENOMEM;

		desc->paddr = alloc.paddr;
		desc->size = alloc.size;
		memcpy (desc->poolname, alloc.pool, sizeof (alloc.pool));

		put_user (alloc.paddr, (typeof(alloc.paddr)*)(arg+offsetof(typeof(alloc),paddr)));
		debug ("alloc... paddr %lx\n", alloc.paddr);
		break;

	case HMA_IOCTL_CACHED:
		return get_user (desc->cached, (int*)arg);

	case HMA_IOCTL_CACHE_CTL:
#ifdef CONFIG_ARM
		{
			struct hma_ioctl_cache_ctl cache_ctl;

			if(copy_from_user(&cache_ctl, (void*)arg, sizeof(cache_ctl)) != 0)
				return -EINVAL;

			if(cache_ctl.operation == HMA_CACHE_INV)//invalidate cache
			{
#ifdef __aarch64__
				dma_map_single(NULL,cache_ctl.vaddr, cache_ctl.size, DMA_FROM_DEVICE);
#else
				dmac_map_area(cache_ctl.vaddr, cache_ctl.size, DMA_FROM_DEVICE);//v7_dma_inv_range
				outer_inv_range((phys_addr_t)cache_ctl.paddr, (phys_addr_t)cache_ctl.paddr + cache_ctl.size);
#endif
			}
			else if(cache_ctl.operation == HMA_CACHE_CLEAN)//clean cache
			{
#ifdef __aarch64__
				dma_map_single(NULL,cache_ctl.vaddr, cache_ctl.size, DMA_TO_DEVICE);
#else
				dmac_map_area(cache_ctl.vaddr, cache_ctl.size, DMA_TO_DEVICE);//v7_dma_clean_range
				outer_clean_range((phys_addr_t)cache_ctl.paddr, (phys_addr_t)cache_ctl.paddr + cache_ctl.size);
#endif
			}
			else//FIXME : flush 추가
			{
				error ("invalid cache operation\n");
				return -EINVAL;
			}

			break;
		}
#endif

	default:
		return -EINVAL;
	}

	return 0;
}

static struct file_operations hma_drv_ops =
{
	.open = hma_drv_open,
	.unlocked_ioctl = hma_drv_ioctl,
	.mmap = hma_drv_mmap,
	.release = hma_drv_release,
};

static struct class *hma_class;
static struct device *hma_device;

static int hma_drv_init (void)
{
	int ret;

	ret = hma_init ();
	if (ret < 0)
		return ret;

	ret = register_chrdev (hma_drv_major, "hma_drv", &hma_drv_ops);
	if (ret < 0)
		return ret;
	if (hma_drv_major == 0)
		hma_drv_major = ret;
	printk ("hma registered on %d\n", hma_drv_major);

	hma_class = class_create (THIS_MODULE, "hma");
	hma_device = device_create (hma_class, NULL, MKDEV (hma_drv_major,0), NULL, "hma");

	return 0;
}

static void hma_drv_exit (void)
{
	device_destroy (hma_class, MKDEV (hma_drv_major,0));
	class_destroy (hma_class);

	unregister_chrdev (hma_drv_major, "hma_drv");
	hma_exit ();
}

MODULE_LICENSE ("GPL");
module_param_named (major, hma_drv_major, int, 0444);

#ifdef MODULE
module_init (hma_drv_init);
module_exit (hma_drv_exit);
#else
subsys_initcall (hma_drv_init);
#endif
