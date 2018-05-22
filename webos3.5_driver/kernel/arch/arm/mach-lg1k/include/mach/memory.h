#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H __FILE__

/*
 * SECTION_SIZE_BITS defines how much size we assign to each section.
 * ('28' means 2^8 MB = 256 MB)
 */
#define MAX_PHYSMEM_BITS        32
#define SECTION_SIZE_BITS       28

#ifndef __ASSEMBLY__
#include <asm/page.h>

extern void v7_dma_map_area(const void *, size_t, int);
extern void v7_dma_unmap_area(const void *, size_t, int);
extern void v7_dma_flush_range(const void *, const void *);

#define __dma_map_area v7_dma_map_area
#define __dma_unmap_area v7_dma_unmap_area
#define __dma_flush_range v7_dma_flush_range

extern void *devmap(phys_addr_t addr, unsigned long size);
extern void devunmap(void *addr);

#endif
#endif /* __ASM_ARCH_MEMORY_H */
