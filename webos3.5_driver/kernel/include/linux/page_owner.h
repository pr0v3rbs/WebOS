#ifndef __LINUX_PAGE_OWNER_H
#define __LINUX_PAGE_OWNER_H

#include <linux/jump_label.h>

#ifdef CONFIG_PAGE_OWNER
extern struct static_key_false page_owner_inited;
extern struct page_ext_operations page_owner_ops;

extern void __page_owner_free_pages(struct page *page, unsigned int order);
extern void __page_owner_alloc_pages(struct page *page,
			unsigned int order, gfp_t gfp_mask);
extern void __split_page_owner(struct page *page, unsigned int order);
extern void __copy_page_owner(struct page *oldpage, struct page *newpage);
extern void __dump_page_owner(struct page *page);

static inline void page_owner_free_pages(struct page *page, unsigned int order)
{
	if (static_branch_unlikely(&page_owner_inited))
		__page_owner_free_pages(page, order);
}

static inline void page_owner_alloc_pages(struct page *page,
			unsigned int order, gfp_t gfp_mask)
{
	if (static_branch_unlikely(&page_owner_inited))
		__page_owner_alloc_pages(page, order, gfp_mask);
}

static inline void split_page_owner(struct page *page, unsigned int order)
{
	if (static_branch_unlikely(&page_owner_inited))
		__split_page_owner(page, order);
}
static inline void copy_page_owner(struct page *oldpage, struct page *newpage)
{
	if (static_branch_unlikely(&page_owner_inited))
		__copy_page_owner(oldpage, newpage);
}
static inline void dump_page_owner(struct page *page)
{
	if (static_branch_unlikely(&page_owner_inited))
		__dump_page_owner(page);
}
#else
static inline void page_owner_free_pages(struct page *page, unsigned int order)
{
}
static inline void page_owner_alloc_pages(struct page *page,
			unsigned int order, gfp_t gfp_mask)
{
}
static inline void split_page_owner(struct page *page,
			unsigned int order)
{
}
static inline void copy_page_owner(struct page *oldpage, struct page *newpage)
{
}
static inline void dump_page_owner(struct page *page)
{
}
#endif /* CONFIG_PAGE_OWNER */
#endif /* __LINUX_PAGE_OWNER_H */
