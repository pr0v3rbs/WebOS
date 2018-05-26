#ifndef _FWLOAD_DRV_H
#define _FWLOAD_DRV_H

#include <linux/module.h>

struct image
{
	unsigned long addr;
	unsigned char *vaddr;
	unsigned int size;

	/* image backup for hivernation */
	unsigned char *backup;
	unsigned int backup_size;
};

struct mcu_control;
struct mcu
{
	struct module *owner;
	const char *name;

	int (*reset) (int);
	int (*halt) (int);
	int (*uart) (int);
	int (*jtag) (int);
	int (*suspend) (void);
	int (*resume) (void);
	int (*freeze) (void);
	int (*thaw) (void);
	int (*restore) (void);

	volatile unsigned int *control_register;
	int control_register_size;

	struct image rom, ram;

	struct mcu_control *control;
};

void free_image (struct image *r);
int copy_to_ddr (struct image *r);

#endif
