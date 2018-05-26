#ifndef _FWLOAD_H
#define _FWLOAD_H

#include <linux/ioctl.h>

enum fwload_image_type
{
	fwload_image_type_rom,
	fwload_image_type_ram,
};

struct fwload_image
{
	enum fwload_image_type type;
	const unsigned char *data;	/* image data */
	unsigned int size;		/* image size */
	unsigned int total_size;	/* memory size */
};

#define FWLOAD_IOCTL_TYPE	'F'
#define FWLOAD_IOCTL_RESET	_IO  (FWLOAD_IOCTL_TYPE, 1)
#define FWLOAD_IOCTL_HALT	_IO  (FWLOAD_IOCTL_TYPE, 2)
#define FWLOAD_IOCTL_LOAD	_IOW (FWLOAD_IOCTL_TYPE, 3, struct fwload_image)


#ifdef __KERNEL__
typedef enum
{
	fwload_event_halt_hold,
	fwload_event_halt_release,
	fwload_event_reset,
} fwload_event_t;

typedef void (*fwload_callback_t) (const char *name, fwload_event_t event);

int fwload_register_callback (const char *name, fwload_callback_t cb);
int fwload_reset (const char *name);
#endif

#endif
