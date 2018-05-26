
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


#define debug(fmt,args...)	do{ if (debug_vdec > 0) printk(KERN_DEBUG"%16s.%d: "fmt,__func__,__LINE__,##args); }while(0)
#define info(fmt,args...)	printk(KERN_INFO"%16s.%d: "fmt,__func__,__LINE__,##args)
#define error(fmt,args...)	printk(KERN_ERR"%16s.%d: "fmt,__func__,__LINE__,##args)

static int debug_vdec = 1;
module_param (debug_vdec, int, 0640);


#if defined(CHIP_NAME_h13)
#define MCU_REG_BASE	0xc0004600
#define MCU_REG_SIZE	0x200
#define CTOP_REG_BASE	0xc001b000
#elif defined(CHIP_NAME_m14)
#define MCU_REG_BASE	0xc0004700
#define MCU_REG_SIZE	0x200
#define CTOP_REG_BASE	0xc001b000
#elif defined(CHIP_NAME_h15)
#define MCU_REG_BASE	0xc8450700
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
#define DEC_MCU_VER	(0x70/4)	// MCU Version


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


static char cmd_line[256];
static unsigned char bootparam[512];

static int reset (int on);
static int halt (int on);
static int uart (int port);
static int jtag (int port);
static int suspend (void);
static int resume (void);
static int freeze (void);
static int thaw (void);
static int restore (void);
static struct mcu mcu =
{
	.name	= "vdec",
	.reset	= reset,
	.halt	= halt,
	.uart	= uart,
	.jtag	= jtag,
	.suspend = suspend,
	.resume	= resume,
	.freeze	= freeze,
	.thaw	= thaw,
	.restore = restore,
};

static int reset (int on)
{
	if (on)
	{
		mcu_write_mask (PROC_CTRL, 1<<28, 1<<28);
	}
	else
	{
		unsigned char *bp;

		mcu_write (SROM_OFFSET, mcu.rom.addr);

		bp = mcu.rom.vaddr + mcu.rom.size - sizeof (bootparam);
		memcpy (bp, bootparam, sizeof (bootparam));
		mcu_reg[0x100/4] = 0;
		mcu_reg[0x104/4] = 0x50000000 + mcu.rom.size - sizeof (bootparam);
		mcu_reg[0x180/4] = 0;

		/* disable all vdec IRQ for live vdec MCU reset */
		{
			unsigned int *irqen;

			//0xc8451540 = 0;
			//0xc8451550 = 0;
			irqen = ioremap (0xc8451500, 0x100);
			if (irqen)
			{
				irqen[0x40/4] = 0;
				irqen[0x50/4] = 0;

				iounmap (irqen);
			}
			else
				error ("cannot remap irq enable register\n");
		}

		mcu_write_mask (PROC_CTRL, 0, 1<<28);
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
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_vdec, 0);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 0:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart0_sel, 4);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_vdec, 0);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 1:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart1_sel, 4);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_vdec, 1);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	case 2:
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, uart2_sel, 4);
		CTOP_CTRL_H15A0_Wr01(ND0, ctr33, rx_sel_vdec, 2);
		CTOP_CTRL_H15A0_WrFL(ND0, ctr33);
		break;
	}
#elif defined(CHIP_NAME_m14)
	if (lx_chip_rev () < LX_CHIP_REV(M14, B0))
	{
		switch (port)
		{
		default:
			CTOP_CTRL_M14A0_Wr01(ctr58, uart0_sel, 2);
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_vdec, 0);
			CTOP_CTRL_M14A0_WrFL(ctr58);
			break;
		case 0:
			CTOP_CTRL_M14A0_Wr01(ctr58, uart0_sel, 4);
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_vdec, 0);
			CTOP_CTRL_M14A0_WrFL(ctr58);
			break;
		case 1:
			CTOP_CTRL_M14A0_Wr01(ctr58, uart1_sel, 4);
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_vdec, 1);
			CTOP_CTRL_M14A0_WrFL(ctr58);
			break;
		case 2:
			CTOP_CTRL_M14A0_Wr01(ctr58, uart2_sel, 4);
			CTOP_CTRL_M14A0_Wr01(ctr58, rx_sel_vdec, 2);
			CTOP_CTRL_M14A0_WrFL(ctr58);
			break;
		}
	}
	else
	{
		switch (port)
		{
		default:
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, uart0_sel, 2);
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, rx_sel_vdec, 0);
			CTOP_CTRL_M14B0_WrFL(LEFT, ctr96);
			break;
		case 0:
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, uart0_sel, 4);
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, rx_sel_vdec, 0);
			CTOP_CTRL_M14B0_WrFL(LEFT, ctr96);
			break;
		case 1:
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, uart1_sel, 4);
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, rx_sel_vdec, 1);
			CTOP_CTRL_M14B0_WrFL(LEFT, ctr96);
			break;
		case 2:
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, uart2_sel, 4);
			CTOP_CTRL_M14B0_Wr01(LEFT, ctr96, rx_sel_vdec, 2);
			CTOP_CTRL_M14B0_WrFL(LEFT, ctr96);
			break;
		}
	}
#endif

	return 0;
}

static int jtag (int port)
{
#if defined(CHIP_NAME_h15)
	switch (port)
	{
	default:
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 0);
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, cpu_jtag, 0);
		CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
		break;
	case 0:
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag0_sel, 6);
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, vdec_jtag, 1);
		CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
		break;
	case 1:
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, jtag1_sel, 6);
		CTOP_CTRL_H15A0_Wr01(CPU, ctr39, vdec_jtag, 0);
		CTOP_CTRL_H15A0_WrFL(CPU, ctr39);
		break;
	}
#else
#warning "FIXME: not implemented"
#endif

	return 0;
}


/* suspend to ram */
static uint32_t *suspend_backup;
static int suspend (void)
{
	printk ("mcu_vdec: suspend...\n");

	suspend_backup = kmalloc (6*sizeof(uint32_t), GFP_ATOMIC);
	if (!suspend_backup)
	{
		printk (KERN_ERR"mcu_vdec: no memory. failed.\n");
		return -ENOMEM;
	}

	suspend_backup[0] = reg_read (SROM_OFFSET);
	suspend_backup[1] = reg_read (SRAM_OFFSET_0);
	suspend_backup[2] = reg_read (SRAM_OFFSET_1);
	suspend_backup[3] = reg_read (SRAM_OFFSET_2);
	suspend_backup[4] = reg_read (E_INTR_EN);
	suspend_backup[5] = reg_read (I_INTR_EN);

	printk ("mcu_vdec: suspend done.\n");

	return 0;
}


static int resume (void)
{
	printk ("mcu_vdec: resume...\n");

	if (!suspend_backup)
	{
		printk (KERN_ERR"mcu_vdec: Oops.\n");
		return -1;
	}

	mcu_write (SROM_OFFSET,   suspend_backup[0]);
	mcu_write (SRAM_OFFSET_0, suspend_backup[1]);
	mcu_write (SRAM_OFFSET_1, suspend_backup[2]);
	mcu_write (SRAM_OFFSET_2, suspend_backup[3]);
	mcu_write (E_INTR_EN,     suspend_backup[4]);
	mcu_write (I_INTR_EN,     suspend_backup[5]);
	kfree (suspend_backup);

	printk ("mcu_vdec: resume xtensa from 0x%x\n",
			mcu_reg[0x180/4]);

	mcu_write_mask (PROC_CTRL, 1<<0, 1<<0);		/* hold halt */
	mcu_write_mask (PROC_CTRL, 1<<28, 1<<28);	/* reset */
	mcu_write_mask (PROC_CTRL, 1<<2, 1<<2);		/* enable debug */
	mcu_write_mask (PROC_CTRL, 0, 1<<0);		/* release halt */

	printk ("mcu_vdec: resume done.\n");

	return 0;
}


/* hivernation */
static int freeze (void)
{
	printk ("mcu_vdec: freeze...\n");

	suspend_backup = kmalloc (0x100+4*sizeof(uint32_t), GFP_ATOMIC);
	if (!suspend_backup)
	{
		printk (KERN_ERR"mcu_vdec: no memory. failed.\n");
		return -1;
	}

	/* backup all IPC registers */
	memcpy (suspend_backup, (void*)&mcu_reg[0x100/4], 0x100);
	suspend_backup[0x100/4 + 0] = reg_read (E_INTR_EN);
	suspend_backup[0x100/4 + 1] = reg_read (SRAM_OFFSET_0);
	suspend_backup[0x100/4 + 2] = reg_read (SRAM_OFFSET_1);
	suspend_backup[0x100/4 + 3] = reg_read (SRAM_OFFSET_2);

	return 0;
}


static int thaw (void)
{
	printk ("mcu_vdec: thaw...\n");

	if (!suspend_backup)
	{
		printk (KERN_ERR"mcu_vdec: Oops\n");
		return -1;
	}

	kfree (suspend_backup);
	suspend_backup = NULL;

	return 0;
}


static int restore (void)
{
	printk ("mcu_vdec: restore...\n");

	if (!suspend_backup)
	{
		printk (KERN_ERR"mcu_vdec: Oops\n");
		return -1;
	}

	/* restore all IPC registers */
	memcpy ((void*)&mcu_reg[0x100/4], suspend_backup, 0x100);
	mcu_write (E_INTR_EN,     suspend_backup[0x100/4 + 0]);
	mcu_write (SRAM_OFFSET_0, suspend_backup[0x100/4 + 1]);
	mcu_write (SRAM_OFFSET_1, suspend_backup[0x100/4 + 2]);
	mcu_write (SRAM_OFFSET_2, suspend_backup[0x100/4 + 3]);
	kfree (suspend_backup);
	suspend_backup = NULL;

	if (!mcu.rom.vaddr || !mcu.rom.backup || !mcu.rom.backup_size)
	{
		/* clear mcu boot indicator register */
		mcu_reg[0x100/4] = 0;
		mcu_reg[0x104/4] = 0;

		/* suspend indicator register */
		mcu_reg[0x180/4] = 0;

		printk (KERN_ERR"mcu_vdec: no restoring firmware image yet\n");
		return 0;
	}

	/* copy firmware and boot */
	halt (1);
	copy_to_ddr (&mcu.rom);

	reset (1);
	reset (0);
	halt (0);

	return 0;
}


#if defined(CHIP_NAME_h15)
static int low_clock = 0;
module_param (low_clock, int, 0640);

static const char *init_mem (void)
{
	struct area
	{
		const char *from;
		const char *name;
		int size;
		phys_addr_t start;	/* physical address */
	} areas[] =
	{
		{ "ddr0", "dpb",	416*1024*1024 },
		{ "ddr0", "vdec",	144*1024*1024-0x8000 },
		{ "ddr1", "ddr1",	208*1024*1024 },
	};
	int i;
	static char cmdline[128];
	phys_addr_t param_base;


	/* allocate memory pool for MCU */
	for (i=0; i<ARRAY_SIZE(areas); i++)
	{
		struct area *a = areas+i;

		debug ("allocate %s from %s, size %d\n",
				a->name, a->from, a->size);
		a->start = hma_alloc_user (a->from, a->size, 4096,
				a->name);
		if (!a->start)
		{
			error ("no mem for \"%s\"\n", a->name);
			break;
		}
	}
	if (i != ARRAY_SIZE(areas))
	{
		for (i--; i >= 0; i--)
		{
			struct area *a = areas+i;

			debug ("free %s, %llx\n", a->name,
					(unsigned long long)a->start);
			hma_free (a->from, a->start);
		}

		return NULL;
	}


	/* register "dpb", "vdec" pool */
	info ("dpb  pool base 0x%llx(0x%x)\n",
			(unsigned long long)areas[0].start,
			areas[0].size);
	hma_pool_register ("dpb",  areas[0].start, areas[0].size);

	info ("vdec pool base 0x%llx(0x%x)\n",
			(unsigned long long)areas[1].start,
			areas[1].size);
	hma_pool_register ("vdec", areas[1].start, areas[1].size);

	/* allocate IPC parameters */
	param_base = hma_alloc_user ("vdec",
			0x1000+0x100000, 4096, "vdec_ipc");
	info ("parameter base %llx\n", (unsigned long long)param_base);
	mcu_reg[0x108/4] = param_base;

	mcu_write (SRAM_OFFSET_0, 0x40000000);	/* 0x60000000 */
	mcu_write (SRAM_OFFSET_1, 0x60000000);	/* 0x80000000 */
	mcu_write (SRAM_OFFSET_2, 0x90000000);	/* 0xa0000000 */


	/* make additional command line */
	sprintf (cmdline, " hma.pool=ddr1:0x%llx:0x%x "
			"vdec.pool_size=0 vdec.dpb_pool_size=0 vdec.dpb_base=0x%llx" ,
			(unsigned long long)(areas[2].start-0x90000000+0xa0000000), areas[2].size,
			(unsigned long long)areas[0].start
			);

	if (low_clock)
	{
		unsigned int *ctop_ctrl_vdec0;

		info ("speed down vdec MCU...\n");

		ctop_ctrl_vdec0 = ioremap (0xc8714000, 0x10);
		if (!ctop_ctrl_vdec0)
		{
			error ("ioremap failed for ctop_ctrl_vdec0.\n");
		}
		else
		{
			unsigned int val;

			val = ctop_ctrl_vdec0[0];
			val |= 1<<29;
			ctop_ctrl_vdec0[0] = val;
			debug ("CTOP_CTRL_VDEC0 = 0x%08x\n", val);

			iounmap (ctop_ctrl_vdec0);
		}
	}

	return cmdline;
}

static int make_bootparam (void)
{
	unsigned int len;
	int a;

	len = strlen (cmd_line)+1;
	len = (len+7)&~7;

#define add_bp(d)	bootparam[a++] = (d)

	a = 0;

	/* BP_TAG_FIRST */
	add_bp (0x0b);
	add_bp (0x7b);
	add_bp (0x00);
	add_bp (0x00);

	/* BP_TAG_COMMAND_LINE */
	add_bp (0x01);
	add_bp (0x10);
	add_bp (len&0xff);
	add_bp (len>>8);
	memcpy (bootparam+a, cmd_line, len);
	a += len;

	/* BP_TAG_LAST */
	add_bp (0x0b);
	add_bp (0x7e);
	add_bp (0x00);
	add_bp (0x00);

	return 0;
}

#else
static const char *init_mem (void)
{
	error ("not implemented\n");
	return NULL;
}

static int make_bootparam (void)
{
	error ("not implemented\n");
	return NULL;
}

#endif

static int init = 1;
module_param (init, int, 0640);

static char *vdec_bootopts;
module_param (vdec_bootopts, charp, 0640);

struct mcu *init_vdec (void)
{
	const char *cmd_mem;

	if (!init)
	{
		error ("no init flag. do not init.\n");
		return NULL;
	}

	/* get register memory */
	mcu_reg = ioremap (MCU_REG_BASE, MCU_REG_SIZE);
	if (!mcu_reg)
	{
		error ("ioremap failed\n");
		goto error_0;
	}
	memset ((void*)(mcu_reg+0x100/4), 0, MCU_REG_SIZE-0x100);

	/* initialize memory */
	cmd_mem = init_mem ();
	if (!cmd_mem)
	{
		error ("cannot initialize memory\n");
		return NULL;
	}

	sprintf (cmd_line, "console=ttyAMA0,115200n81 root=mtd1 quiet%s %s",
			cmd_mem,
			(vdec_bootopts&&vdec_bootopts[0])?vdec_bootopts:""
			);
	info ("use mcu command line : \"%s\"\n",
			cmd_line);

	make_bootparam ();


	mcu.owner = THIS_MODULE;
	mcu.control_register = mcu_reg;
	mcu.control_register_size = MCU_REG_SIZE;

	return &mcu;

error_0:
	return NULL;
}

void cleanup_vdec (void)
{
	/* halt */
	mcu_write_mask (0, 1<<0, 1<<0);


	if (mcu.rom.addr)
		free_image (&mcu.rom);
	if (mcu.ram.addr)
		free_image (&mcu.ram);
	iounmap (mcu_reg);
}

