
#ifndef _IPC_H
#define _IPC_H

#include <linux/types.h>
#include <linux/mutex.h>

struct ipc;

typedef int (*ipc_handler_t) (struct ipc *ipc,
		void *data, int size);
typedef int (*ipc_handler_irq_t) (struct ipc *, uint32_t type,
		uint32_t target_number,
		uint32_t offs, uint32_t size);
typedef int (*ipc_ioctl_t) (unsigned int cmd, unsigned long arg);
typedef int (*ipc_handler_ioctl_t) (struct ipc *ipc,
		unsigned int cmd, unsigned long arg);

struct ipc
{
	/* ipc register base address */
	void *ipc_base;

	/* base address of shared DDR memory with ARM and xtensa. send_queue
	 * and receive_queue will lay on the shared memory.
	 */
	void *queue_base;
	int queue_size;

	const char *name;
	int irq_num;
	void (*irq_send) (struct ipc *);
	int (*irq_status) (struct ipc *);
	void (*irq_clear) (struct ipc *);

	ipc_handler_t handler;
	ipc_handler_irq_t handler_irq;
	ipc_handler_ioctl_t handler_ioctl;
	ipc_ioctl_t ioctl;

	phys_addr_t arg_base_phys;
	void *arg_base;
	int arg_size;

	void *priv;
};


/* structure that will be stored in parameter area */
struct ipc_param_arg
{
	uint32_t call;
	uint32_t priv;
	uint32_t data_size;
};


void ipc__dump (unsigned char *data, int len );

//int ipc_call (struct ipc *ipc, void *data, int size);
int ipc_send_noirq (struct ipc *ipc, uint32_t type,
		uint32_t target_number,
		uint32_t offs, uint32_t size,
		uint32_t *ipc_number);
int ipc_send (struct ipc *ipc, uint32_t type,
		uint32_t target_number,
		uint32_t offs, uint32_t size,
		uint32_t *ipc_number);
int ipc_send_ioctl (struct ipc *ipc, unsigned int cmd,
		unsigned long arg);
int ipc_call2 (struct ipc *ipc,
		uint32_t priv, const char *call,
		void *data, int size,
		int data_dir, int *ret);
#define IPC_CALL_DATA_DIR_WR	1
#define IPC_CALL_DATA_DIR_RD	2
int ipc_reset (struct ipc *ipc);



struct ipc* ipc_new (void);
int ipc_init (struct ipc *ipc);
void ipc_del (struct ipc *ipc);
int ipc_user_command (struct ipc *ipc, const char *str);

int ipc_core_init (void);
void ipc_core_exit (void);

#endif
