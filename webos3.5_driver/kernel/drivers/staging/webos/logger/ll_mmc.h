#ifndef _LINUX_MMCOOPS_H
#define _LINUX_MMCOOPS_H

#include <linux/types.h>

struct ll_mmc_ops {
	const char *name;
	s32 (*ll_mmc_init)(void);
	s32 (*ll_mmc_write)(u8 *buffer, u64 address, u32 size);
	s32 (*ll_mmc_read)(u8 *buffer, u64 address, u32 size);
	void (*ll_mmc_exit)(void);
	bool inited;
};

s32 ll_mmc_init(void);
bool ll_mmc_ready(void);
s32 ll_mmc_write(u8 *buffer, u64 address, u32 size);
s32 ll_mmc_read(u8 *buffer, u64 address, u32 size);
void ll_mmc_exit(void);

void register_ll_mmc_ops(struct ll_mmc_ops *ops);
#endif
