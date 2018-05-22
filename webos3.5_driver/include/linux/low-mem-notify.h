#ifndef _LINUX_LOW_MEM_NOTIFY_H
#define _LINUX_LOW_MEM_NOTIFY_H

struct xreclaimable;

#ifdef CONFIG_LOW_MEM_NOTIFY

extern void low_mem_notify(void);
extern struct xreclaimable * low_mem_notify_add_xreclaimable(char *name);
extern void low_mem_notify_remove_xreclaimable(struct xreclaimable * xr);
extern void low_mem_notify_set_xreclaimable(unsigned long size);
extern void low_mem_notify_set_xreclaimable_size(struct xreclaimable * xr, unsigned long size);
extern void low_mem_notify_set_freeze(unsigned long flag);

#else /* !CONFIG_LOW_MEM_NOTIFY */

static inline void low_mem_notify(void) {}
static inline struct xreclaimable * low_mem_notify_add_xreclaimable(char *name) { return NULL; }
static inline void low_mem_notify_remove_xreclaimable(struct xreclaimable * xr) {}
static inline void low_mem_notify_set_xreclaimable(unsigned long size) {}
static inline void low_mem_notify_set_xreclaimable_size(struct xreclaimable * xr, unsigned long size) {}
static inline void low_mem_notify_set_freeze(unsigned long flag) {}

#endif

#endif
