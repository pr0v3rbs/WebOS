/*
 *  include/linux/dump_around_register.h
 *
 *  Copyright (C) 2016 Sangseok Lee <sangseok.lee@lge.com>
 *
 */

#ifndef _LINUX_DUMP_AROUND_REGISTER_H
#define _LINUX_DUMP_AROUND_REGISTER_H

/*
 * address_checker(): check address that attempt to read
 * @addr: start address
 * @user: kerenl cotext or user context
 *
 * return 1 if address @addr can be read in @user context,
 * else return 0.
 *
 *
 * probe_read(): safely attempt to read from a location
 * @dst: pointer to the buffer that shall take the data
 * @src: address to read from
 * @size: size of the data chunk
 *
 * Safely read from address @src to the buffer at @dst.  If a kernel fault
 * happens, handle that and return -EFAULT.
 */
struct address_check_ops {
	int (*address_checker)(unsigned long addr, int user);
	long (*probe_read)(void *dst, const void *src, size_t size);
};

int register_address_check_ops(struct address_check_ops *ops);

#endif /* _LINUX_DUMP_AROUND_REGISTER_H */
