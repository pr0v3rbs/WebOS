
#ifndef _LXIPC_H_
#define _LXIPC_H_

#include <linux/ioctl.h>

#define	LXIPC_IOC_MAGIC		'v'

#define LXIPC_IO_UART		_IO  (LXIPC_IOC_MAGIC, 0)
#define LXIPC_IO_JTAG		_IO  (LXIPC_IOC_MAGIC, 1)

#endif

