/*
 * Contiguous Memory Allocator for DMA mapping framework
 * Copyright (c) 2010-2011 by Samsung Electronics.
 * Written by:
 *	Marek Szyprowski <m.szyprowski@samsung.com>
 *	Michal Nazarewicz <mina86@mina86.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

#define pr_fmt(fmt) "cma: " fmt

#ifdef CONFIG_CMA_DEBUG
#ifndef DEBUG
#  define DEBUG
#endif
#endif

#include <asm/page.h>
#include <asm/dma-contiguous.h>

#include <linux/memblock.h>
#include <linux/err.h>
#include <linux/sizes.h>
#include <linux/dma-contiguous.h>
#include <linux/cma.h>
#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
#include "mdrv_types.h"
#include "mdrv_system.h"
#endif

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
#define CMA_HEAP_MIUOFFSET_NOCARE (-1UL)
#endif

#ifdef CONFIG_CMA_SIZE_MBYTES
#define CMA_SIZE_MBYTES CONFIG_CMA_SIZE_MBYTES
#else
#define CMA_SIZE_MBYTES 0
#endif

struct cma *dma_contiguous_default_area;

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
phys_addr_t lowmem_limit = 0;
#if defined(CONFIG_ARM)
extern phys_addr_t arm_lowmem_limit;
#else
#error "Error, the selected processor type is unknown"
#endif
#endif /* CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER */

/*
 * Default global CMA area size can be defined in kernel's .config.
 * This is useful mainly for distro maintainers to create a kernel
 * that works correctly for most supported systems.
 * The size can be set in bytes or as a percentage of the total memory
 * in the system.
 *
 * Users, who want to set the size of global CMA area for their system
 * should use cma= kernel parameter.
 */
static const phys_addr_t size_bytes = (phys_addr_t)CMA_SIZE_MBYTES * SZ_1M;
static phys_addr_t size_cmdline = -1;

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
/* To store every mstar driver cma_buffer size, including default cma_buffer with index is 0 */
struct CMA_BootArgs_Config cma_config[MAX_CMA_AREAS];
struct device mstar_cma_device[MAX_CMA_AREAS];
int mstar_driver_boot_cma_buffer_num = 0;


EXPORT_SYMBOL(cma_config);
EXPORT_SYMBOL(mstar_cma_device);
EXPORT_SYMBOL(mstar_driver_boot_cma_buffer_num);

/*
  * if  start address has been specified, only convert it to cpu bus address;
  * else, find start address
  */
static bool GetReservedPhysicalAddr(unsigned char miu, unsigned long *start,
    unsigned long *size)

{
    unsigned long alignstart = 0, alignfactor = pageblock_nr_pages*PAGE_SIZE;

    if(miu < 0 || miu > 3 || *size == 0)
        goto GetReservedPhysicalAddr_Fail;

    if(*start != CMA_HEAP_MIUOFFSET_NOCARE)
    {
        if(!IS_ALIGNED(*start, alignfactor))
            goto GetReservedPhysicalAddr_Fail;
    }


    if(!IS_ALIGNED(*size, alignfactor))
        goto GetReservedPhysicalAddr_Fail;

    if(*start == CMA_HEAP_MIUOFFSET_NOCARE)
        return true;

    switch (miu)
    {
        case 0:
        {
            alignstart = *start + MIU0_BUS_BASE;
            break;
        }
        case 1:
        {
            alignstart = *start + MIU1_BUS_BASE;
            break;
        }
        case 2:
        {
            alignstart = *start + MIU2_BUS_BASE;
            break;
        }
        case 3:
        {
            alignstart = *start + MIU3_BUS_BASE;
            break;
        }
        default:
            goto GetReservedPhysicalAddr_Fail;
    }

    *start = alignstart;
    return true;

GetReservedPhysicalAddr_Fail:
    printk(KERN_ERR "error: invalid parameters\n");
    *start = 0;
    *size = 0;
    return false;
}

static unsigned long _find_in_range(phys_addr_t start, phys_addr_t end, phys_addr_t size, phys_addr_t alignfactor)
{
    phys_addr_t ret = 0;

    if((lowmem_limit > start) && (lowmem_limit < end))
    {
		/* find from lowmem first*/
        ret = memblock_find_in_range(start, lowmem_limit, size, alignfactor);
        if(ret > 0)
		{
			//printk("\033[35mFunction = %s, Line = %d, find cma_buffer range from 0x%X to 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, start, lowmem_limit);
            return ret;
		}

		/* find from highmem */
        ret = memblock_find_in_range(lowmem_limit, end, size, alignfactor);
        if(ret > 0)
		{
			//printk("\033[35mFunction = %s, Line = %d, find cma_buffer range from 0x%X to 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, lowmem_limit, end);
            return ret;
		}
    }
    else if(end > start)
    {
        ret = memblock_find_in_range(start, end, size, alignfactor);
        if(ret > 0)
		{
			//printk("\033[35mFunction = %s, Line = %d, find cma_buffer range from 0x%X to 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, start, end);
            return ret;
		}
    }

	printk("\033[35mFunction = %s, Line = %d, ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    return ret;
}

static unsigned long find_start_addr(unsigned char miu, unsigned long size)
{
    unsigned long ret = 0;
    unsigned long alignfactor = pageblock_nr_pages*PAGE_SIZE;

    if(miu < 0 || miu > 3)
        return 0;

    switch (miu)
    {
        case 0:
        {
            ret = _find_in_range(MIU0_BUS_BASE, MIU1_BUS_BASE, size, alignfactor);
            break;
        }
        case 1:
        {
            ret = _find_in_range(MIU1_BUS_BASE, MIU2_BUS_BASE, size, alignfactor);
            break;
        }
        case 2:
        {
            ret = _find_in_range(MIU2_BUS_BASE, MIU3_BUS_BASE, size, alignfactor);
            break;
        }
        case 3:
        {
            ret = _find_in_range(MIU3_BUS_BASE, CMA_HEAP_MIUOFFSET_NOCARE, size, alignfactor);
            break;
        }
        default:
            return 0;
    }

    return ret;
}

static bool parse_heap_config(char *cmdline, struct CMA_BootArgs_Config * heapconfig)
{
    char * option;
    int leng = 0;
    bool has_start = false;

    if(cmdline == NULL)
        goto INVALID_HEAP_CONFIG;

    //printk(KERN_ERR "cmdline: %s\n", cmdline);

    option = strstr(cmdline, ",");
    leng = (int)(option - cmdline);
    if(leng > (CMA_HEAP_NAME_LENG-1))
        leng = CMA_HEAP_NAME_LENG -1;

    strncpy(heapconfig->name, cmdline, leng);
    heapconfig->name[leng] = '\0';

    option = strstr(cmdline, "st=");
    if(option != NULL)
        has_start = true;

    option = strstr(cmdline, "sz=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;

    option = strstr(cmdline, "hid=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;


    option = strstr(cmdline, "miu=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;

    if(has_start)
    {
        sscanf(option, "miu=%d,hid=%d,sz=%lx,st=%lx", &heapconfig->miu,
        &heapconfig->pool_id, &heapconfig->size, &heapconfig->start);
    }
    else
    {
        sscanf(option, "miu=%d,hid=%d,sz=%lx", &heapconfig->miu,
        &heapconfig->pool_id, &heapconfig->size);

        heapconfig->start = CMA_HEAP_MIUOFFSET_NOCARE;
    }

    if(!GetReservedPhysicalAddr(heapconfig->miu, &heapconfig->start, &heapconfig->size))
        goto INVALID_HEAP_CONFIG;

    //printk(KERN_ERR "miu %d, pool id %d, size %lu, start %lu\n", heapconfig->miu,
    //heapconfig->pool_id, heapconfig->size, heapconfig->miu_offset);

    return true;

INVALID_HEAP_CONFIG:
	heapconfig->size = 0;
	return false;
}

int __init setup_cma0_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma0 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma1_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma1 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma2_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma2 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma3_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma3 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma4_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma4 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma5_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))
        printk(KERN_ERR "error: cma5 args invalid\n");
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

early_param("CMA0", setup_cma0_info);
early_param("CMA1", setup_cma1_info);
early_param("CMA2", setup_cma2_info);
early_param("CMA3", setup_cma3_info);
early_param("CMA4", setup_cma4_info);
early_param("CMA5", setup_cma5_info);
#endif /* CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER */

static phys_addr_t base_cmdline;
static phys_addr_t limit_cmdline;

static int __init early_cma(char *p)
{
	pr_debug("%s(%s)\n", __func__, p);
	size_cmdline = memparse(p, &p);
	if (*p != '@')
		return 0;
	base_cmdline = memparse(p + 1, &p);
	if (*p != '-') {
		limit_cmdline = base_cmdline + size_cmdline;
		return 0;
	}
	limit_cmdline = memparse(p + 1, &p);

	return 0;
}
early_param("cma", early_cma);

#ifdef CONFIG_CMA_SIZE_PERCENTAGE

static phys_addr_t __init __maybe_unused cma_early_percent_memory(void)
{
	struct memblock_region *reg;
	unsigned long total_pages = 0;

	/*
	 * We cannot use memblock_phys_mem_size() here, because
	 * memblock_analyze() has not been called yet.
	 */
	for_each_memblock(memory, reg)
		total_pages += memblock_region_memory_end_pfn(reg) -
			       memblock_region_memory_base_pfn(reg);

	return (total_pages * CONFIG_CMA_SIZE_PERCENTAGE / 100) << PAGE_SHIFT;
}

#else

static inline __maybe_unused phys_addr_t cma_early_percent_memory(void)
{
	return 0;
}

#endif

/**
 * dma_contiguous_reserve() - reserve area(s) for contiguous memory handling
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory from early allocator. It should be
 * called by arch specific code once the early allocator (memblock or bootmem)
 * has been activated and all other subsystems have already allocated/reserved
 * memory.
 */
void __init dma_contiguous_reserve(phys_addr_t limit)
{
	phys_addr_t selected_size = 0;
	phys_addr_t selected_base = 0;
	phys_addr_t selected_limit = limit;
	bool fixed = false;
#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	int dma_declare_index = 0;
	int ret = 0;

    /*
     *  mstar_cma_device store the device with successfully parsed cma buffer info
     */
	struct device *declare_mstar_cma_device = &mstar_cma_device[0];
#endif /* CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER */

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
#if defined(CONFIG_ARM)
	lowmem_limit = arm_lowmem_limit;
#else
#error "Error, the selected processor type is unknown"
#endif
#endif /* CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER */

	pr_debug("%s(limit %08lx)\n", __func__, (unsigned long)limit);

	if (size_cmdline != -1) {
		selected_size = size_cmdline;
		selected_base = base_cmdline;
		selected_limit = min_not_zero(limit_cmdline, limit);
		if (base_cmdline + size_cmdline == limit_cmdline)
			fixed = true;
	} else {
#ifdef CONFIG_CMA_SIZE_SEL_MBYTES
		selected_size = size_bytes;
#elif defined(CONFIG_CMA_SIZE_SEL_PERCENTAGE)
		selected_size = cma_early_percent_memory();
#elif defined(CONFIG_CMA_SIZE_SEL_MIN)
		selected_size = min(size_bytes, cma_early_percent_memory());
#elif defined(CONFIG_CMA_SIZE_SEL_MAX)
		selected_size = max(size_bytes, cma_early_percent_memory());
#endif
	}

	if (selected_size && !dma_contiguous_default_area) {
		pr_debug("%s: reserving %ld MiB for global area\n", __func__,
			 (unsigned long)selected_size / SZ_1M);
#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
		ret = dma_contiguous_reserve_area(selected_size, selected_base,
					    selected_limit,
					    &dma_contiguous_default_area,
					    fixed);
		if (ret)
	{
		printk(KERN_ERR "error: declare default cma config fail\n");
		BUG_ON(ret);
	}
#else
		dma_contiguous_reserve_area(selected_size, selected_base,
					    selected_limit,
					    &dma_contiguous_default_area,
					    fixed);
#endif
	}

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	/*
	 * add cma buffer from bootargs, and assigne it to the specific device
	 * cma buffer is not limited in low memory, also can locate in high memory
	 */

	while(dma_declare_index < mstar_driver_boot_cma_buffer_num)
	{
		pr_info("\033[35mreserving %ld MiB for mstar_driver\033[m\n", cma_config[dma_declare_index].size / SZ_1M);

        BUG_ON(cma_config[dma_declare_index].size == 0);
        if(CMA_HEAP_MIUOFFSET_NOCARE == cma_config[dma_declare_index].start)
        {
			pr_info("find cma_buffer start addr(@miu%d) for %s\033[m\n", cma_config[dma_declare_index].miu, cma_config[dma_declare_index].name);
			cma_config[dma_declare_index].start = find_start_addr(cma_config[dma_declare_index].miu, cma_config[dma_declare_index].size);

			BUG_ON(cma_config[dma_declare_index].start == 0);
        }

//        printk(KERN_ERR "index %d, name %s, miu %d, start 0x%X, size 0x%X, pool id %d\n",dma_declare_index,
//            cma_config[dma_declare_index].name, (int)cma_config[dma_declare_index].miu,
//            cma_config[dma_declare_index].start, cma_config[dma_declare_index].size,
//            cma_config[dma_declare_index].pool_id);

        //check if the reserved memory allocated across 2 memory zones: a part of it in normal zone, the other in high memory
        if(cma_config[dma_declare_index].start > 0)
        {
            if((cma_config[dma_declare_index].start < lowmem_limit)
                && (cma_config[dma_declare_index].start + cma_config[dma_declare_index].size > lowmem_limit))
            {
                printk(KERN_ERR "Warning: reserved memory allocated across 2 memory zones!!!=========\n");
            }
        }

        ret = dma_declare_contiguous(declare_mstar_cma_device, cma_config[dma_declare_index].size, cma_config[dma_declare_index].start, limit);
		declare_mstar_cma_device->coherent_dma_mask = ~0;	// not sure, this mask will be used in __dma_alloc while doing cma_alloc, 0xFFFFFFFF is for NULL device

        if(ret)
        {
            printk(KERN_ERR "error: reserve memory fail, start %lu size %lu\n",
            cma_config[dma_declare_index].start, cma_config[dma_declare_index].size);
            BUG_ON(ret);
        }

		declare_mstar_cma_device->init_name = cma_config[dma_declare_index].name;
		dma_declare_index++;
        declare_mstar_cma_device++;
	}
#endif /* CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER */
}

/**
 * dma_contiguous_reserve_area() - reserve custom contiguous area
 * @size: Size of the reserved area (in bytes),
 * @base: Base address of the reserved area optional, use 0 for any
 * @limit: End address of the reserved memory (optional, 0 for any).
 * @res_cma: Pointer to store the created cma region.
 * @fixed: hint about where to place the reserved area
 *
 * This function reserves memory from early allocator. It should be
 * called by arch specific code once the early allocator (memblock or bootmem)
 * has been activated and all other subsystems have already allocated/reserved
 * memory. This function allows to create custom reserved areas for specific
 * devices.
 *
 * If @fixed is true, reserve contiguous area at exactly @base.  If false,
 * reserve in range from @base to @limit.
 */
int __init dma_contiguous_reserve_area(phys_addr_t size, phys_addr_t base,
				       phys_addr_t limit, struct cma **res_cma,
				       bool fixed)
{
	int ret;

	ret = cma_declare_contiguous(base, size, limit, 0, 0, fixed, res_cma);

	if (ret)
		return ret;

	/* Architecture specific contiguous memory fixup. */
	dma_contiguous_early_fixup(cma_get_base(*res_cma),
				cma_get_size(*res_cma));

	return 0;
}

/**
 * dma_alloc_from_contiguous() - allocate pages from contiguous area
 * @dev:   Pointer to device for which the allocation is performed.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).
 *
 * This function allocates memory buffer for specified device. It uses
 * device specific contiguous memory area if available or the default
 * global one. Requires architecture specific dev_get_cma_area() helper
 * function.
 */
struct page *dma_alloc_from_contiguous(struct device *dev, size_t count,
				       unsigned int align)
{
	if (align > CONFIG_CMA_ALIGNMENT)
		align = CONFIG_CMA_ALIGNMENT;

	return cma_alloc(dev_get_cma_area(dev), count, align);
}

/**
 * dma_release_from_contiguous() - release allocated pages
 * @dev:   Pointer to device for which the pages were allocated.
 * @pages: Allocated pages.
 * @count: Number of allocated pages.
 *
 * This function releases memory allocated by dma_alloc_from_contiguous().
 * It returns false when provided pages do not belong to contiguous area and
 * true otherwise.
 */
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count)
{
	return cma_release(dev_get_cma_area(dev), pages, count);
}

/*
 * Support for reserved memory regions defined in device tree
 */
#ifdef CONFIG_OF_RESERVED_MEM
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>

#undef pr_fmt
#define pr_fmt(fmt) fmt

static int rmem_cma_device_init(struct reserved_mem *rmem, struct device *dev)
{
	dev_set_cma_area(dev, rmem->priv);
	return 0;
}

static void rmem_cma_device_release(struct reserved_mem *rmem,
				    struct device *dev)
{
	dev_set_cma_area(dev, NULL);
}

static const struct reserved_mem_ops rmem_cma_ops = {
	.device_init	= rmem_cma_device_init,
	.device_release = rmem_cma_device_release,
};

static int __init rmem_cma_setup(struct reserved_mem *rmem)
{
	phys_addr_t align = PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order);
	phys_addr_t mask = align - 1;
	unsigned long node = rmem->fdt_node;
	struct cma *cma;
	int err;

	if (!of_get_flat_dt_prop(node, "reusable", NULL) ||
	    of_get_flat_dt_prop(node, "no-map", NULL))
		return -EINVAL;

	if ((rmem->base & mask) || (rmem->size & mask)) {
		pr_err("Reserved memory: incorrect alignment of CMA region\n");
		return -EINVAL;
	}

	err = cma_init_reserved_mem(rmem->base, rmem->size, 0, &cma);
	if (err) {
		pr_err("Reserved memory: unable to setup CMA region\n");
		return err;
	}
	/* Architecture specific contiguous memory fixup. */
	dma_contiguous_early_fixup(rmem->base, rmem->size);

	if (of_get_flat_dt_prop(node, "linux,cma-default", NULL))
		dma_contiguous_set_default(cma);

	rmem->ops = &rmem_cma_ops;
	rmem->priv = cma;

	pr_info("Reserved memory: created CMA memory pool at %pa, size %ld MiB\n",
		&rmem->base, (unsigned long)rmem->size / SZ_1M);

	return 0;
}
RESERVEDMEM_OF_DECLARE(cma, "shared-dma-pool", rmem_cma_setup);

#endif

#ifdef CONFIG_MP_ION_PATCH_MSTAR
struct page *dma_alloc_from_contiguous_direct(struct device *dev, size_t count,
				       unsigned int align, long *retlen)
{
	return cma_alloc_direct(dev_get_cma_area(dev), count, align, retlen);
}

/**
 * dma_alloc_from_contiguous_addr() - allocate pages from contiguous area from specified start address
 * @dev:   Pointer to device for which the allocation is performed.
 * @start: Start bus address to allocate.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).	// request 2^align pages
 *
 * This function allocates memory buffer for specified device. It uses
 * device specific contiguous memory area if available or the default
 * global one. Requires architecture specific get_dev_cma_area() helper
 * function.
 */
void *dma_alloc_from_contiguous_addr(struct device *dev, unsigned long start,
		size_t count, unsigned int align)
{
	return cma_alloc_addr(dev_get_cma_area(dev), start, count, align);
}
#endif /* CONFIG_MP_ION_PATCH_MSTAR */
