#ifndef _IPC_VDEC_H_
#define _IPC_VDEC_H_

#include <linux/kernel.h>

#include "ipc.h"

extern struct ipc *ipc_vdec_init (ipc_handler_t ipc_handler,
		ipc_handler_irq_t ipc_handler_irq,
		ipc_handler_ioctl_t ipc_handler_ioctl,
		ipc_ioctl_t ioctl);
extern void ipc_vdec_exit (void);
extern phys_addr_t ipc_vdec_to_phys (struct ipc *ipc, uint32_t addr);


struct ipc_vdec_priv
{
#define IPC_FLAGS_VDEC_POLLIN	(1<<0)
#define IPC_FLAGS_VDEC_POLLOUT	(1<<1)
#define IPC_FLAGS_VDEC_POLLERR	(1<<2)
#define IPC_FLAGS_VO_POLLIN	(1<<8)
#define IPC_FLAGS_VO_POLLOUT	(1<<9)
#define IPC_FLAGS_VO_POLLERR	(1<<10)
	volatile uint32_t *flags;
};


enum ipc_vdec_type
{
	/* xtensa to ARM */
	ipc_vdec_type_poll_vdec = 0x10000000,
	ipc_vdec_type_poll_vo,
	ipc_vdec_type_pong,
	ipc_vdec_type_audioinf_set_basetime_stc,
	ipc_vdec_type_audioinf_set_basetime_pts,
	ipc_vdec_type_audioinf_playto,

	/* ARM to xtensa */
	ipc_vdec_type_suspend = 0x10001000,
	ipc_vdec_type_ping,
};

#endif

