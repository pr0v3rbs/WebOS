#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <asm/io.h>

static void *_devmap(phys_addr_t addr, unsigned long size, pgprot_t pgprot)
{
	unsigned int n_pages;
	void *vaddr;
	size_t page_offset = 0;
	struct page **pages;
	unsigned int start_pfn, i;

	if (!IS_ALIGNED((unsigned long)addr, PAGE_SIZE)) {
		page_offset = addr & ~PAGE_MASK;
		pr_debug("vmap: addr is not-page-aligned %lx\n",
				(unsigned long)addr);
	}

	start_pfn = __phys_to_pfn(addr & PAGE_MASK);

	n_pages = PAGE_ALIGN(size) >> PAGE_SHIFT;
	pages =  vmalloc(sizeof(struct page *) * n_pages);

	for (i = 0; i < n_pages; i++)
		pages[i] = pfn_to_page(start_pfn + i);

	vaddr = vmap(pages, n_pages, VM_MAP, pgprot) + page_offset;

	vfree(pages);
	return vaddr;
}

void *devmap(phys_addr_t addr, unsigned long size)
{
	unsigned int pfn = __phys_to_pfn(addr & PAGE_MASK);

	if (!pfn_valid(pfn)) {
		pr_warn("devmap: 0x%x is not valid pfn, use ioremap\n", addr);
		return ioremap(addr, size);
	}

	return _devmap(addr, size, pgprot_writecombine(PAGE_KERNEL));
}
EXPORT_SYMBOL(devmap);

void *devmap_cache(phys_addr_t addr, unsigned long size)
{
	unsigned int pfn = __phys_to_pfn(addr & PAGE_MASK);

	if (pfn_valid(pfn))
		return _devmap(addr, size, PAGE_KERNEL);
	else
		return ioremap_cache(addr, size);
}
EXPORT_SYMBOL(devmap_cache);

void devunmap(void *addr)
{
	void *vaddr;

	if (!IS_ALIGNED((unsigned long)addr, PAGE_SIZE))
		pr_debug("vunmap: addr is not-page-aligned vaddr:%p\n", addr);

	vaddr = (void *)((unsigned long)addr & PAGE_MASK);

	/*
	 * We consider the addr is mapped by vmap basically.
	 * So, If addr is ioremaped by fallback, The exceptional
	 * condition in iounmap() won't be not checked.
	 * (eg, static mapping, section based mapping)
	 */
	vunmap(vaddr);
}
EXPORT_SYMBOL(devunmap);

