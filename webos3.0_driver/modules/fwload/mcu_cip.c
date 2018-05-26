
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "hma_alloc.h"
#include "fwload_drv.h"


#if defined(CHIP_NAME_h15)
typedef unsigned int UINT32;
#include "chip/h15/sys/ctop_ctrl_reg_h15.h"
#elif defined(CHIP_NAME_m14)
#include "core/base/os_util.h"

typedef unsigned int UINT32;
#include "chip/m14/sys/ctop_ctrl_reg_m14.h"
#endif


#define debug(fmt,args...)	do{ if (debug_level > 0) printk(KERN_DEBUG"%16s.%d: "fmt,__func__,__LINE__,##args); }while(0)
#define info(fmt,args...)	printk(KERN_INFO"%16s.%d: "fmt,__func__,__LINE__,##args)
#define error(fmt,args...)	printk(KERN_ERR"%16s.%d: "fmt,__func__,__LINE__,##args)

static int debug_level = 1;
module_param_named (debug_cip, debug_level, int, 0640);


#if defined(CHIP_NAME_h15)
#define MCU_REG_BASE	0xc8833800
#define MCU_REG_SIZE	0x200
#define CTOP_REG_BASE	0xc001b000
#else
#error unknown chip name
#endif

#define PROC_CTRL	(0x00/4)	// MCU Processor Control
#define PROC_ID		(0x04/4)	// MCU Processor ID
#define PDBG_DATA	(0x08/4)	// MCU Processor Debug Data
#define PDBG_PC		(0x0C/4)	// MCU Processor Debug PC
#define SROM_OFFSET	(0x10/4)	// MCU System ROM OFFSET Address
#define SRAM_OFFSET_0	(0x14/4)	// MCU System RAM OFFSET 0 Address
#define SRAM_OFFSET_1	(0x18/4)	// MCU System RAM OFFSET 1 Address
#define SRAM_OFFSET_2	(0x1C/4)	// MCU System RAM OFFSET 2 Address
#define E_INTR_EN	(0x30/4)	// MCU External Interrupt Enable
#define E_INTR_ST	(0x34/4)	// MCU External Interrupt Status
#define E_INTR_CL	(0x38/4)	// MCU External Interrupt Clear
#define E_TO_I_EV	(0x3C/4)	// MCU External to Internal Interrupt Event
#define I_INTR_EN	(0x40/4)	// MCU Internal Interrupt Enable
#define I_INTR_ST	(0x44/4)	// MCU Internal Interrupt Status
#define I_INTR_CL	(0x48/4)	// MCU Internal Interrupt Clear
#define I_TO_E_EV	(0x4C/4)	// MCU Internal to External Interrupt Event


static volatile unsigned int *mcu_reg;

#define mcu_write(r,d)		_mcu_write (#r,r,d)
#define reg_read(r)		_reg_read(#r,r)
#define mcu_write_mask(r,d,m)	_mcu_write_mask(#r,r,d,m)
static inline void _mcu_write (const char *name, int addr, unsigned int data)
{
	debug ("%14s(0x%03x) <- %08x\n", name, addr*4, data);
	mcu_reg[addr] = data;
}

static inline unsigned int _reg_read (const char *name, int addr)
{
	unsigned int ret;

	ret = mcu_reg[addr];
	debug ("%14s(0x%03x) -> %08x\n", name, addr*4, ret);

	return ret;
}

static inline void _mcu_write_mask (const char *name, int addr, unsigned int data, unsigned int mask)
{
	unsigned int reg;

	reg = _reg_read (name, addr);
	reg = (reg & ~mask) | data;
	_mcu_write (name, addr, reg);
}


static int reset (int on);
static int halt (int on);
static int uart (int port);
static int jtag (int port);
static struct mcu mcu =
{
	.name	= "cip",
	.reset	= reset,
	.halt	= halt,
	.uart	= uart,
	.jtag	= jtag,
};

static int reset (int on)
{
	if (on)
	{
		mcu_write_mask (PROC_CTRL, 1<<28 | 1<<8, 1<<28 | 1<<8);
	}
	else
	{
		mcu_write (SROM_OFFSET, mcu.rom.addr);

		mcu_write_mask (PROC_CTRL, 0, 1<<28 | 1<<8);
	}

	return 0;
}

static int halt (int on)
{
	if (on)
	{
		mcu_write_mask (PROC_CTRL, 1<<0, 1<<0);
	}
	else
	{
		/* enable debug */
		mcu_write_mask (PROC_CTRL, 1<<2, 1<<2);

		/* release halt */
		mcu_write_mask (PROC_CTRL, 0, 1<<0);
	}

	return 0;
}

static int uart (int port)
{
#if defined(CHIP_NAME_h15)
	switch (port)
	{
	default:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 2);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_ti, 0);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 0:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 1);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_ti, 0);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 1:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart1_sel, 1);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_ti, 1);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 2:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart2_sel, 1);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_ti, 2);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	}
#endif
	return 0;
}

static int jtag (int port)
{
	return 0;
}


struct mcu *init_cip (void)
{
	/* get register memory */
	mcu_reg = ioremap (MCU_REG_BASE, MCU_REG_SIZE);
	if (!mcu_reg)
	{
		error ("ioremap failed\n");
		goto error_0;
	}


	mcu.owner = THIS_MODULE;
	mcu.control_register = mcu_reg;
	mcu.control_register_size = MCU_REG_SIZE;

	return &mcu;

error_0:
	return NULL;
}

void cleanup_cip (void)
{
	/* halt */
	mcu_write_mask (0, 1<<0, 1<<0);


	if (mcu.rom.addr)
		free_image (&mcu.rom);
	if (mcu.ram.addr)
		free_image (&mcu.ram);
	iounmap (mcu_reg);
}

