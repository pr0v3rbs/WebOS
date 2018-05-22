/*
 *  include/linux/fault_notifier.h
 *
 *  Copyright (C) 2016 Sangseok Lee <sangseok.lee@lge.com>
 *
 */

#ifndef _LINUX_FAULTNOTIFIER_H
#define _LINUX_FAULTNOTIFIER_H

#define	FN_TYPE_EXT4_CRASH		0x1
#define	FN_TYPE_ALLOC_FAIL		0x10
#define	FN_TYPE_TEST			0x80000000
#define	FN_TYPE_ALL			0xffffffff

/**
 * fn_kernel_notify - notify kernel fault to Fault Manager
 * @type: fault type
 * @fmt: format string as printk
 * @...: variable args as printk
 * Returns: if success 0, else
 *		-EINVAL : reason is NULL
 *		-ENOMEM : memory access fail
 *		-ENDEV  : Fault Manager not registered
 *
 * Before Fault Manager register path, Only one fault reason can be saved.
 * The saved fault reason fire as soon as Fault Manager register path.
 * If there are more than two fault before Fault Manager registere path,
 * return -ENODEV
 */

#ifdef CONFIG_FAULT_NOTIFIER
extern int fn_kernel_notify(int type, const char *fmt, ...);
#else
static inline int fn_kernel_notify(int type, const char *fmt, ...) { return 0; };
#endif

#endif /* _LINUX_FAULTNOTIFIER_H */
