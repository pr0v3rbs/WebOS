
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/io.h>

#include "hal/vdec_base.h"
#include "ipc.h"
#include "ipc_vdec.h"

//#define LOG_PRINTK
#define LOG_NAME ipc_vdec
#define LOG_LEVEL log_level_warning
#include "log.h"


static uint32_t *mcurc;
static struct ipc *ipc;

static void *arg_base_virt;


#ifdef __XTENSA__
#define MCURC_IRQ(c)	mcurc[(0x40+(c))/4]
#else
#define MCURC_IRQ(c)	mcurc[(0x30+(c))/4]
#endif
static void ipc_irq_send (struct ipc *ipc)
{
	log_debug ("send irq...\n");
	MCURC_IRQ(0xc) = 0x1;
}

static int ipc_irq_status (struct ipc *ipc)
{
	uint32_t status;

	status = MCURC_IRQ(0x4);
	log_debug ("irq status.. %x\n", status);

	return !!(status&0x400);
}

static void ipc_irq_clear (struct ipc *ipc)
{
	log_debug ("clear irq\n");

	MCURC_IRQ(0x8) = 0x400;
}

phys_addr_t ipc_vdec_to_phys (struct ipc *ipc, uint32_t addr)
{
	if (addr < 0x50000000)
	{
		log_warning ("unknown address. %x\n", addr);
		return 0;
	}

	if (addr < 0x60000000)
		return addr-0x50000000+mcurc[0x10/4];

	if (addr < 0x80000000)
		return addr-0x60000000+mcurc[0x14/4];

	if (addr < 0xa0000000)
		return addr-0x80000000+mcurc[0x18/4];

	if (addr < 0xc0000000)
		return addr-0xa0000000+mcurc[0x1c/4];

	log_warning ("unknown address. %x\n", addr);
	return 0;
}

struct ipc *ipc_vdec_init (ipc_handler_t ipc_handler,
		ipc_handler_irq_t ipc_handler_irq,
		ipc_handler_ioctl_t ipc_handler_ioctl,
		ipc_ioctl_t ipc_ioctl)
{
	int ret;
	int arg_size;
	phys_addr_t arg_base_phys;
	phys_addr_t mem_base;
	uint32_t *ipc_base;

	printk (KERN_INFO "ipc driver\n");

	mcurc = ioremap (VDEC_REG_MCU_RC, 0x100);

	/* enable IPC irq */
	MCURC_IRQ(0) = 0x400;

	ret = ipc_core_init ();
	if (ret < 0)
	{
		log_error ("ipc_core_init failed. %d\n", ret);
		return NULL;
	}

	ipc = ipc_new ();
	if (!ipc)
	{
		log_error ("no ipc.\n");
		return NULL;
	}

	ipc->ipc_base = ioremap (VDEC_REG_MCU_IPC, 0x100);
	if (!ipc->ipc_base)
	{
		log_error ("ioremap failed\n");
		ipc_del (ipc);
		return NULL;
	}
	ipc_base = ipc->ipc_base;

	ipc->name = "vdec";
	ipc->irq_num = VDEC_IRQ_NUM_IPC;
	ipc->handler = ipc_handler;
	ipc->handler_irq = ipc_handler_irq;
	ipc->handler_ioctl = ipc_handler_ioctl;
	ipc->ioctl = ipc_ioctl;
	ipc->irq_send = ipc_irq_send;
	ipc->irq_status = ipc_irq_status;
	ipc->irq_clear = ipc_irq_clear;

	mem_base = ipc_base[0x8/4];
#ifdef __XTENSA__
	mem_base = mem_base - 0x60000000 + 0x80000000;
#else
	if (!mem_base)
	{
		log_error ("no base address of IPC\n");
		goto failed;
	}
#endif
	log_info ("ipc parameter base %x\n", mem_base);

	ipc->queue_size = 0x1000;
	ipc->queue_base = ioremap (mem_base, ipc->queue_size);
	log_info ("mem_base %x, shared %p(%d)\n",
			mem_base, ipc->queue_base, ipc->queue_size);

	arg_size = 0x100000;
	arg_base_phys = mem_base + ipc->queue_size;

	arg_base_virt = ioremap (arg_base_phys, arg_size);
	ipc->arg_base_phys = arg_base_phys;
	ipc->arg_base = arg_base_virt;
	ipc->arg_size = arg_size;

	ret = ipc_init (ipc);
	if (ret < 0)
	{
		log_error ("ipc_init failed. %d\n", ret);
		goto failed;
	}

	return ipc;

failed:
	if (ipc)
	{
		if (ipc->queue_base)
			iounmap (ipc->queue_base);
		if (ipc->ipc_base)
			iounmap (ipc->ipc_base);
		ipc_del (ipc);
	}

	ipc_core_exit ();

	return NULL;
}

void ipc_vdec_exit (void)
{
	printk (KERN_INFO "ipc driver exit\n");

	iounmap (arg_base_virt);

	iounmap (ipc->queue_base);

	iounmap (ipc->ipc_base);

	ipc_del (ipc);

	ipc_core_exit ();

	iounmap (mcurc);
}

