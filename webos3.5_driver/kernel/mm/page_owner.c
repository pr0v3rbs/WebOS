#include <linux/debugfs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/bootmem.h>
#include <linux/stacktrace.h>
#include <linux/page_owner.h>
#include <linux/jump_label.h>
#include <linux/stackdepot.h>

#include "internal.h"

/*
 * TODO: teach PAGE_OWNER_STACK_DEPTH (__dump_page_owner and save_stack)
 * to use off stack temporal storage
 */
#define PAGE_OWNER_STACK_DEPTH (16)

static const char *page_owner_handles_names[PAGE_OWNER_HANDLE_MAX] = {
	"page allocated",
	"page freed, was allocated",
};

static bool page_owner_disabled = true;
DEFINE_STATIC_KEY_FALSE(page_owner_inited);

static depot_stack_handle_t dummy_handle;
static depot_stack_handle_t failure_handle;

static void init_early_allocated_pages(void);

static int early_page_owner_param(char *buf)
{
	if (!buf)
		return -EINVAL;

	if (strcmp(buf, "on") == 0)
		page_owner_disabled = false;

	return 0;
}
early_param("page_owner", early_page_owner_param);

static bool need_page_owner(void)
{
	if (page_owner_disabled)
		return false;

	return true;
}

static noinline void register_dummy_stack(void)
{
	unsigned long entries[4];
	struct stack_trace dummy;

	dummy.nr_entries = 0;
	dummy.max_entries = ARRAY_SIZE(entries);
	dummy.entries = &entries[0];
	dummy.skip = 0;

	save_stack_trace(&dummy);
	dummy_handle = depot_save_stack(&dummy, GFP_KERNEL);
}

static noinline void register_failure_stack(void)
{
	unsigned long entries[4];
	struct stack_trace failure;

	failure.nr_entries = 0;
	failure.max_entries = ARRAY_SIZE(entries);
	failure.entries = &entries[0];
	failure.skip = 0;

	save_stack_trace(&failure);
	failure_handle = depot_save_stack(&failure, GFP_KERNEL);
}

static void init_page_owner(void)
{
	if (page_owner_disabled)
		return;

	register_dummy_stack();
	register_failure_stack();
	static_branch_enable(&page_owner_inited);
	init_early_allocated_pages();
}

struct page_ext_operations page_owner_ops = {
	.need = need_page_owner,
	.init = init_page_owner,
};

static inline bool check_recursive_alloc(struct stack_trace *trace,
					unsigned long ip)
{
	int i, count;

	if (!trace->nr_entries)
		return false;

	for (i = 0, count = 0; i < trace->nr_entries; i++) {
		if (trace->entries[i] == ip && ++count == 2)
			return true;
	}

	return false;
}

static noinline depot_stack_handle_t save_stack(gfp_t flags)
{
	unsigned long entries[PAGE_OWNER_STACK_DEPTH];
	struct stack_trace trace = {
		.nr_entries = 0,
		.entries = entries,
		.max_entries = PAGE_OWNER_STACK_DEPTH,
		.skip = 0
	};
	depot_stack_handle_t handle;

	save_stack_trace(&trace);
	if (trace.nr_entries != 0 &&
	    trace.entries[trace.nr_entries-1] == ULONG_MAX)
		trace.nr_entries--;

	/*
	 * We need to check recursion here because our request to stackdepot
	 * could trigger memory allocation to save new entry. New memory
	 * allocation would reach here and call depot_save_stack() again
	 * if we don't catch it. There is still not enough memory in stackdepot
	 * so it would try to allocate memory again and loop forever.
	 */
	if (check_recursive_alloc(&trace, _RET_IP_))
		return dummy_handle;

	handle = depot_save_stack(&trace, flags);
	if (!handle)
		handle = failure_handle;

	return handle;
}

void __page_owner_free_pages(struct page *page, unsigned int order)
{
	int i;
	depot_stack_handle_t handle = save_stack(0);

	for (i = 0; i < (1 << order); i++) {
		struct page_ext *page_ext = lookup_page_ext(page + i);

		if (unlikely(!page_ext))
			continue;

		page_ext->handles[PAGE_OWNER_HANDLE_FREE] = handle;
		__set_bit(PAGE_EXT_OWNER_FREE, &page_ext->flags);
		__clear_bit(PAGE_EXT_OWNER_ALLOC, &page_ext->flags);
	}
}

noinline void __page_owner_alloc_pages(struct page *page, unsigned int order,
					gfp_t gfp_mask)
{
	struct page_ext *page_ext = lookup_page_ext(page);

	if (unlikely(!page_ext))
		return;

	page_ext->handles[PAGE_OWNER_HANDLE_ALLOC] = save_stack(gfp_mask);
	page_ext->order = order;
	page_ext->gfp_mask = gfp_mask;

	__set_bit(PAGE_EXT_OWNER_ALLOC, &page_ext->flags);
}

void __split_page_owner(struct page *page, unsigned int order)
{
	int i;
	struct page_ext *page_ext = lookup_page_ext(page);

	if (unlikely(!page_ext))
		return;

	page_ext->order = 0;
	for (i = 1; i < (1 << order); i++)
		__copy_page_owner(page, page + i);
}

void __copy_page_owner(struct page *oldpage, struct page *newpage)
{
	struct page_ext *old_ext = lookup_page_ext(oldpage);
	struct page_ext *new_ext = lookup_page_ext(newpage);
	int i;

	if (unlikely(!old_ext || !new_ext))
		return;

	new_ext->order = old_ext->order;
	new_ext->gfp_mask = old_ext->gfp_mask;
	for (i = 0; i < PAGE_OWNER_HANDLE_MAX; i++)
		new_ext->handles[i] = old_ext->handles[i];
	/*
	 * We don't clear the bit on the oldpage as it's going to be freed
	 * after migration. Until then, the info can be useful in case of
	 * a bug, and the overal stats will be off a bit only temporarily.
	 * Also, migrate_misplaced_transhuge_page() can still fail the
	 * migration and then we want the oldpage to retain the info. But
	 * in that case we also don't need to explicitly clear the info from
	 * the new page, which will be freed.
	 */
	__set_bit(PAGE_EXT_OWNER_ALLOC, &new_ext->flags);
}

static ssize_t
print_page_owner(char __user *buf, size_t count, unsigned long pfn,
		struct page *page, struct page_ext *page_ext,
		depot_stack_handle_t handle)
{
	int ret;
	int pageblock_mt, page_mt;
	char *kbuf;
	unsigned long entries[PAGE_OWNER_STACK_DEPTH];
	struct stack_trace trace = {
		.nr_entries = 0,
		.entries = entries,
		.max_entries = PAGE_OWNER_STACK_DEPTH,
		.skip = 0
	};

	kbuf = kmalloc(count, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	ret = snprintf(kbuf, count,
			"Page allocated via order %u, mask 0x%x\n",
			page_ext->order, page_ext->gfp_mask);

	if (ret >= count)
		goto err;

	/* Print information relevant to grouping pages by mobility */
	pageblock_mt = get_pfnblock_migratetype(page, pfn);
	page_mt  = gfpflags_to_migratetype(page_ext->gfp_mask);
	ret += snprintf(kbuf + ret, count - ret,
			"PFN %lu Block %lu type %d %s Flags %s%s%s%s%s%s%s%s%s%s%s%s\n",
			pfn,
			pfn >> pageblock_order,
			pageblock_mt,
			pageblock_mt != page_mt ? "Fallback" : "        ",
			PageLocked(page)	? "K" : " ",
			PageError(page)		? "E" : " ",
			PageReferenced(page)	? "R" : " ",
			PageUptodate(page)	? "U" : " ",
			PageDirty(page)		? "D" : " ",
			PageLRU(page)		? "L" : " ",
			PageActive(page)	? "A" : " ",
			PageSlab(page)		? "S" : " ",
			PageWriteback(page)	? "W" : " ",
			PageCompound(page)	? "C" : " ",
			PageSwapCache(page)	? "B" : " ",
			PageMappedToDisk(page)	? "M" : " ");

	if (ret >= count)
		goto err;

	depot_fetch_stack(handle, &trace);
	ret += snprint_stack_trace(kbuf + ret, count - ret, &trace, 0);
	if (ret >= count)
		goto err;

	ret += snprintf(kbuf + ret, count - ret, "\n");
	if (ret >= count)
		goto err;

	if (copy_to_user(buf, kbuf, ret))
		ret = -EFAULT;

	kfree(kbuf);
	return ret;

err:
	kfree(kbuf);
	return -ENOMEM;
}

void __dump_page_owner(struct page *page)
{
	struct page_ext *page_ext = lookup_page_ext(page);
	unsigned long entries[PAGE_OWNER_STACK_DEPTH];
	struct stack_trace trace = {
		.nr_entries = 0,
		.entries = entries,
		.max_entries = PAGE_OWNER_STACK_DEPTH,
		.skip = 0
	};
	depot_stack_handle_t handle;
	gfp_t gfp_mask = page_ext->gfp_mask;
	int mt = gfpflags_to_migratetype(gfp_mask);
	int i;

	if (!test_bit(PAGE_EXT_OWNER_ALLOC, &page_ext->flags) &&
		!test_bit(PAGE_EXT_OWNER_FREE, &page_ext->flags)) {
		pr_alert("page_owner info is not active (free page?)\n");
		return;
	}

	for (i = 0; i < PAGE_OWNER_HANDLE_MAX; i++) {
		handle = READ_ONCE(page_ext->handles[i]);
		if (!handle) {
			pr_alert("page_owner info is not active for `%s'\n",
					page_owner_handles_names[i]);
			continue;
		}

		depot_fetch_stack(handle, &trace);
		pr_alert("%s via order %u, migratetype %d, gfp_mask %#x(%pGg)\n",
				page_owner_handles_names[i], page_ext->order,
				mt, gfp_mask, &gfp_mask);
		print_stack_trace(&trace, 0);
	}
}

static ssize_t
read_page_owner(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	unsigned long pfn;
	struct page *page;
	struct page_ext *page_ext;
	depot_stack_handle_t handle;

	if (!static_branch_unlikely(&page_owner_inited))
		return -EINVAL;

	page = NULL;
	pfn = min_low_pfn + *ppos;

	/* Find a valid PFN or the start of a MAX_ORDER_NR_PAGES area */
	while (!pfn_valid(pfn) && (pfn & (MAX_ORDER_NR_PAGES - 1)) != 0)
		pfn++;

	drain_all_pages(NULL);

	/* Find an allocated page */
	for (; pfn < max_pfn; pfn++) {
		/*
		 * If the new page is in a new MAX_ORDER_NR_PAGES area,
		 * validate the area as existing, skip it if not
		 */
		if ((pfn & (MAX_ORDER_NR_PAGES - 1)) == 0 && !pfn_valid(pfn)) {
			pfn += MAX_ORDER_NR_PAGES - 1;
			continue;
		}

		/* Check for holes within a MAX_ORDER area */
		if (!pfn_valid_within(pfn))
			continue;

		page = pfn_to_page(pfn);
		if (PageBuddy(page)) {
			unsigned long freepage_order = page_order_unsafe(page);

			if (freepage_order < MAX_ORDER)
				pfn += (1UL << freepage_order) - 1;
			continue;
		}

		page_ext = lookup_page_ext(page);
		if (unlikely(!page_ext))
			continue;

		/*
		 * Some pages could be missed by concurrent allocation or free,
		 * because we don't hold the zone lock.
		 */
		if (!test_bit(PAGE_EXT_OWNER_ALLOC, &page_ext->flags))
			continue;

		/*
		 * Access to page_ext->handle isn't synchronous so we should
		 * be careful to access it.
		 */
		handle = READ_ONCE(page_ext->handles[PAGE_OWNER_HANDLE_ALLOC]);
		if (!handle)
			continue;

		/* Record the next PFN to read in the file offset */
		*ppos = (pfn - min_low_pfn) + 1;

		return print_page_owner(buf, count, pfn, page,
				page_ext, handle);
	}

	return 0;
}

static void init_pages_in_zone(pg_data_t *pgdat, struct zone *zone)
{
	struct page *page;
	struct page_ext *page_ext;
	unsigned long pfn = zone->zone_start_pfn, block_end_pfn;
	unsigned long end_pfn = pfn + zone->spanned_pages;
	unsigned long count = 0;

	/* Scan block by block. First and last block may be incomplete */
	pfn = zone->zone_start_pfn;

	/*
	 * Walk the zone in pageblock_nr_pages steps. If a page block spans
	 * a zone boundary, it will be double counted between zones. This does
	 * not matter as the mixed block count will still be correct
	 */
	for (; pfn < end_pfn; ) {
		if (!pfn_valid(pfn)) {
			pfn = ALIGN(pfn + 1, MAX_ORDER_NR_PAGES);
			continue;
		}

		block_end_pfn = ALIGN(pfn + 1, pageblock_nr_pages);
		block_end_pfn = min(block_end_pfn, end_pfn);

		page = pfn_to_page(pfn);

		for (; pfn < block_end_pfn; pfn++) {
			if (!pfn_valid_within(pfn))
				continue;

			page = pfn_to_page(pfn);

			if (page_zone(page) != zone)
				continue;

			/*
			 * We are safe to check buddy flag and order, because
			 * this is init stage and only single thread runs.
			 */
			if (PageBuddy(page)) {
				pfn += (1UL << page_order(page)) - 1;
				continue;
			}

			if (PageReserved(page))
				continue;

			page_ext = lookup_page_ext(page);
			if (unlikely(!page_ext))
				continue;

			/* Maybe overraping zone */
			if (test_bit(PAGE_EXT_OWNER_ALLOC, &page_ext->flags))
				continue;

			/* Found early allocated page */
			__page_owner_alloc_pages(page, 0, 0);
			count++;
		}
	}

	pr_info("Node %d, zone %8s: page owner found early allocated %lu pages\n",
		pgdat->node_id, zone->name, count);
}

static void init_zones_in_node(pg_data_t *pgdat)
{
	struct zone *zone;
	struct zone *node_zones = pgdat->node_zones;
	unsigned long flags;

	for (zone = node_zones; zone - node_zones < MAX_NR_ZONES; ++zone) {
		if (!populated_zone(zone))
			continue;

		spin_lock_irqsave(&zone->lock, flags);
		init_pages_in_zone(pgdat, zone);
		spin_unlock_irqrestore(&zone->lock, flags);
	}
}

static void init_early_allocated_pages(void)
{
	pg_data_t *pgdat;

	drain_all_pages(NULL);
	for_each_online_pgdat(pgdat)
		init_zones_in_node(pgdat);
}

static const struct file_operations proc_page_owner_operations = {
	.read		= read_page_owner,
};

static int __init pageowner_init(void)
{
	struct dentry *dentry;

	if (!static_branch_unlikely(&page_owner_inited)) {
		pr_info("page_owner is disabled\n");
		return 0;
	}

	dentry = debugfs_create_file("page_owner", S_IRUSR, NULL,
			NULL, &proc_page_owner_operations);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	return 0;
}
late_initcall(pageowner_init)