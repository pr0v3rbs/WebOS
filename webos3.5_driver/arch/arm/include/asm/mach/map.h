/*
 *  arch/arm/include/asm/map.h
 *
 *  Copyright (C) 1999-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Page table mapping constructs and function prototypes
 */
#ifndef __ASM_MACH_MAP_H
#define __ASM_MACH_MAP_H

#include <asm/io.h>

struct map_desc {
	unsigned long virtual;
	unsigned long pfn;
	unsigned long length;
	unsigned int type;
};

/* types 0-3 are defined in asm/io.h */
enum {
	MT_UNCACHED = 4,
	MT_CACHECLEAN,
	MT_MINICLEAN,
	MT_LOW_VECTORS,
	MT_HIGH_VECTORS,
	MT_MEMORY_RWX,
	MT_MEMORY_RW,
	MT_ROM,
	MT_MEMORY_RWX_NONCACHED,
	MT_MEMORY_RW_DTCM,
	MT_MEMORY_RWX_ITCM,
	MT_MEMORY_RW_SO,
	MT_MEMORY_DMA_READY,
};

#ifdef CONFIG_MMU
extern void iotable_init(struct map_desc *, int);
extern void vm_reserve_area_early(unsigned long addr, unsigned long size,
				  void *caller);

#ifdef CONFIG_DEBUG_LL
extern void debug_ll_addr(unsigned long *paddr, unsigned long *vaddr);
extern void debug_ll_io_init(void);
#else
static inline void debug_ll_io_init(void) {}
#endif

struct mem_type;
extern const struct mem_type *get_mem_type(unsigned int type);

#ifdef CONFIG_MP_PLATFORM_UTOPIA2K_EXPORT_SYMBOL
#ifdef MSTAR_PORTING_KERNEL_4_4_3
/* fix warning: section mismatch */
extern void __init map_driver_mem(unsigned long drv_ba, unsigned long len, unsigned long va, bool non_cache);
#else
extern void map_driver_mem(unsigned long drv_ba, unsigned long len, unsigned long va, bool non_cache);
#endif /* MSTAR_PORTING_KERNEL_4_4_3 */
extern int clear_driver_map(unsigned long drv_va, unsigned long len);
#endif /* CONFIG_MP_PLATFORM_UTOPIA2K_EXPORT_SYMBOL */

/*
 * external interface to remap single page with appropriate type
 */
extern int ioremap_page(unsigned long virt, unsigned long phys,
			const struct mem_type *mtype);
#else
#define iotable_init(map,num)	do { } while (0)
#define vm_reserve_area_early(a,s,c)	do { } while (0)
#endif

#endif
