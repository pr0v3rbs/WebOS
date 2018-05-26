/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  main configuration file for lgbus device
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  author		hwanwook.lee (hwanwook.lee@lge.com)
 *  author		ks.hyun (ks.hyun@lge.com) - modifier
 *  version		1.0
 *  date		2012.07.14
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */


/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */

#include <linux/interrupt.h>	/** irqreturn_t */
#include <linux/irq.h>		/** platform interrupt */

#include <linux/delay.h>

#include "os_util.h"
#include "sys_regs.h"

#include "lgbus_drv.h"
#include "lgbus_hw.h"
#include "lgbus_h13.h"



/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#if 0
#define lgbus_debug(fmt, args...)		printk(fmt, ##args)
#else
#define lgbus_debug(fmt, args...)		do{}while(0)
#endif

#if 0
#define lgbus_trace()					printk("%s:%d\n", __func__, __LINE__);
#else
#define lgbus_trace()					do{}while(0)
#endif


#define lgbus_reg_read(addr)			__raw_readl((void*)(addr))
#define lgbus_reg_write(val,addr)		__raw_writel(val, (void*)(addr))

#define lgbus_blk_reg_write(idx, val, reg)	lgbus_reg_write(val, lgbus_ctrl.mnt_blk[idx].reg_base + reg)
#define lgbus_blk_reg_read(idx, reg)		lgbus_reg_read(lgbus_ctrl.mnt_blk[idx].reg_base + reg)

/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/
typedef struct
{
	spinlock_t	lock;
	int			refcnt;
	void		(*start)(void);
	void		(*stop)(void);
} LGBUS_ACCESS_CTRL_T;

typedef struct
{
	UINT32					reg_base[LGBUS_TYPE_MAX];
	LGBUS_MNT_BLK_INFO_T	*mnt_blk;

	UINT64					max_bw;
	struct completion		completion;

	LX_LGBUS_MON_CFG_T		mon_cfg;

//	unsigned long long		start;
} LGBUS_CTRL_T;


/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/
static const char *blk_name[] =
{
	[LGBUS_CPU_MAIN]	= "cpu_main",
	[LGBUS_CPU_SUB]		= "cpu_sub",
	[LGBUS_GPU0]		= "gpu0",
	[LGBUS_GPU1]		= "gpu1",
	[LGBUS_GFX]			= "gfx",
	[LGBUS_VDEC]		= "vdec",
	[LGBUS_VDO]			= "vdo",
	[LGBUS_AUD]			= "aud",
	[LGBUS_ICOD]		= "icod",
	[LGBUS_TE]			= "te",
	[LGBUS_VENC]		= "venc",
	[LGBUS_TCON]		= "tcon",
	[LGBUS_DPPA]		= "dpp_a",
	[LGBUS_DE_A]		= "de_a",
	[LGBUS_DE_B]		= "de_b",
	[LGBUS_DE_C]		= "de_c",
	[LGBUS_DE_D]		= "de_d",
	[LGBUS_DE_E_CVD]	= "de_e_cvd",
	[LGBUS_DE_E_MCU]	= "de_e_mcu",
	[LGBUS_DVI]			= "dvi",
	[LGBUS_DVO]			= "dvo",
	[LGBUS_SRE]			= "sre",
	[LGBUS_MEP_A]		= "mep_a",
	[LGBUS_MEP_B]		= "mep_b",
	[LGBUS_BVE_MCU]		= "bve_mcu",
	[LGBUS_MC]			= "mc",
};

static LGBUS_MNT_BLK_INFO_T mnt_blk_info_h13a0[] =
{
	{	.idx = LGBUS_CPU_MAIN,	.reg_offset = 0x0B00, .bus_idx = LGBUS_TYPE_L, .clock =	(400*MHZ) },
	{	.idx = LGBUS_CPU_SUB,	.reg_offset = 0x1800, .bus_idx = LGBUS_TYPE_G, .clock =	(400*MHZ) },
	{	.idx = LGBUS_GPU0,		.reg_offset = 0x0200, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_GPU1,		.reg_offset = 0x0300, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_GFX,		.reg_offset = 0x0400, .bus_idx = LGBUS_TYPE_L, .clock =	(400*MHZ) },
	{	.idx = LGBUS_VDEC,		.reg_offset = 0x0700, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_VDO,		.reg_offset = 0x0800, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_AUD,		.reg_offset = 0x0100, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_ICOD,		.reg_offset = 0x0500, .bus_idx = LGBUS_TYPE_L, .clock =	(200*MHZ) },
	{	.idx = LGBUS_TE,		.reg_offset = 0x0600, .bus_idx = LGBUS_TYPE_L, .clock =	(200*MHZ) },
	{	.idx = LGBUS_VENC,		.reg_offset = 0x0900, .bus_idx = LGBUS_TYPE_L, .clock =	(320*MHZ) },
	{	.idx = LGBUS_TCON,		.reg_offset = 0x1A00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DPPA,		.reg_offset = 0x1000, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_A,		.reg_offset = 0x0A00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_B,		.reg_offset = 0x0B00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_C,		.reg_offset = 0x0C00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_D,		.reg_offset = 0x0D00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_E_CVD,	.reg_offset = 0x0E00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DE_E_MCU,	.reg_offset = 0x0F00, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DVI,		.reg_offset = 0x1100, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_DVO,		.reg_offset = 0x1200, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_SRE,		.reg_offset = 0x1300, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_MEP_A,		.reg_offset = 0x1400, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_MEP_B,		.reg_offset = 0x1500, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_BVE_MCU,	.reg_offset = 0x1600, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
	{	.idx = LGBUS_MC,		.reg_offset = 0x1700, .bus_idx = LGBUS_TYPE_G, .clock =	(200*MHZ) },
};

static LGBUS_MNT_BLK_INFO_T mnt_blk_info_h13b0[] =
{
	{	.idx = LGBUS_CPU_MAIN,	.reg_offset = 0x1C00, .bus_idx = LGBUS_TYPE_L, .clock = (396*MHZ) },
	{	.idx = LGBUS_CPU_SUB,	.reg_offset = 0x1C00, .bus_idx = LGBUS_TYPE_G, .clock = (396*MHZ) },
	{	.idx = LGBUS_GPU0,		.reg_offset = 0x0200, .bus_idx = LGBUS_TYPE_L, .clock = (297*MHZ) },
	{	.idx = LGBUS_GPU1,		.reg_offset = 0x0300, .bus_idx = LGBUS_TYPE_L, .clock = (297*MHZ) },
	{	.idx = LGBUS_GFX,		.reg_offset = 0x0400, .bus_idx = LGBUS_TYPE_L, .clock = (396*MHZ) },
	{	.idx = LGBUS_VDEC,		.reg_offset = 0x0700, .bus_idx = LGBUS_TYPE_L, .clock = (320*MHZ) },
	{	.idx = LGBUS_VDO,		.reg_offset = 0x0800, .bus_idx = LGBUS_TYPE_L, .clock = (320*MHZ) },
	{	.idx = LGBUS_AUD,		.reg_offset = 0x0100, .bus_idx = LGBUS_TYPE_L, .clock = (320*MHZ) },
	{	.idx = LGBUS_ICOD,		.reg_offset = 0x0500, .bus_idx = LGBUS_TYPE_L, .clock = (198*MHZ) },
	{	.idx = LGBUS_TE,		.reg_offset = 0x0600, .bus_idx = LGBUS_TYPE_L, .clock = (198*MHZ) },
	{	.idx = LGBUS_VENC,		.reg_offset = 0x0900, .bus_idx = LGBUS_TYPE_L, .clock = (320*MHZ) },
	{	.idx = LGBUS_TCON,		.reg_offset = 0x1A00, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DPPA,		.reg_offset = 0x1000, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_A,		.reg_offset = 0x0A00, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_B,		.reg_offset = 0x0B00, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_C,		.reg_offset = 0x0C00, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_D,		.reg_offset = 0x0D00, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_E_CVD,	.reg_offset = 0x0E00, .bus_idx = LGBUS_TYPE_L, .clock = (198*MHZ) },
	{	.idx = LGBUS_DE_E_MCU,	.reg_offset = 0x0F00, .bus_idx = LGBUS_TYPE_L, .clock = (198*MHZ) },
	{	.idx = LGBUS_DVI,		.reg_offset = 0x1100, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_DVO,		.reg_offset = 0x1200, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_SRE,		.reg_offset = 0x1300, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_MEP_A,		.reg_offset = 0x1400, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_MEP_B,		.reg_offset = 0x1500, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_BVE_MCU,	.reg_offset = 0x1600, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
	{	.idx = LGBUS_MC,		.reg_offset = 0x1700, .bus_idx = LGBUS_TYPE_G, .clock = (198*MHZ) },
};


static LGBUS_CTRL_T lgbus_ctrl;
static LGBUS_ACCESS_CTRL_T access_ctrl;


/* Workaround for arbitration H/W bug */
static void reg_access_start_b0(void)
{
	unsigned long flags;

	spin_lock_irqsave(&access_ctrl.lock, flags);
	if(access_ctrl.refcnt == 0)
	{
		lgbus_debug("Set APB Clock to 50MHz\n");

		// set APB clk select to 50MHz(1), 25MHz(2), doesn't work at 100MHz(0) async arbitration H/W bug
		CTOP_CTRL_H13B0_RdFL(ctr01);
		CTOP_CTRL_H13B0_Wr01(ctr01, apb_clk_sel, 1);
		CTOP_CTRL_H13B0_WrFL(ctr01);
	}
	access_ctrl.refcnt++;
	spin_unlock_irqrestore(&access_ctrl.lock, flags);
}

static void reg_access_stop_b0(void)
{
	unsigned long flags;

	spin_lock_irqsave(&access_ctrl.lock, flags);
	if(access_ctrl.refcnt > 0)
	{
		access_ctrl.refcnt--;
		if(access_ctrl.refcnt == 0)
		{
			lgbus_debug("Set APB Clock to 100MHz\n");

			// set APB clk select to 100MHz(0) for normal operation, It's workaround of async arbitration H/W bug
			CTOP_CTRL_H13B0_RdFL(ctr01);
			CTOP_CTRL_H13B0_Wr01(ctr01, apb_clk_sel, 0);
			CTOP_CTRL_H13B0_WrFL(ctr01);
		}
	}
	spin_unlock_irqrestore(&access_ctrl.lock, flags);
}


static void lgbus_access_start(void)
{
	if(access_ctrl.start) access_ctrl.start();
}

static void lgbus_access_stop(void)
{
	if(access_ctrl.stop) access_ctrl.stop();
}


static irqreturn_t lgbus_isr(int irq, void *dev)
{
	int i, bus_idx;
	UINT32 reg_val, intr_status;

	lgbus_debug("lgbus_isr, irq number[%d]\n", irq);

	if(irq == H13_IRQ_LBUS) bus_idx = LGBUS_TYPE_L;
	else bus_idx = LGBUS_TYPE_G;

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		if(lgbus_ctrl.mnt_blk[i].bus_idx != bus_idx) continue;

		reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		intr_status = reg_val & MNT_INTR_STATUS_MASK;

		if(!intr_status) continue;

		lgbus_debug("%s irq status : 0x%08x\n", blk_name[i], reg_val);

		if(intr_status & TMR_DONE_INTR_STATUS)
		{
			if(i == LGBUS_CPU_MAIN)
			{
				//unsigned long long now = sched_clock();
				//printk("CPU Elapsed : %dus\n", ((int)(now-lgbus_ctrl.start))/1000);
				/*
				 * Send signal to process
				 */
				//complete(&lgbus_ctrl.completion);
			}
			else if(i == LGBUS_AUD)
			{
			// TODO: Interrupt delayed 10ms in 1HZ mode. So event triggerd based on audio irq
				//unsigned long long now = sched_clock();
				//printk("AUD Elapsed : %dus\n", ((int)(now-lgbus_ctrl.start))/1000);
				complete(&lgbus_ctrl.completion);
			}
		}

		if(intr_status & MEM_PROTECT_INTR_STATUS)  // protected memory access
		{
			UINT32 violated_addr, protected_base, protected_end;
			violated_addr = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_MEM_PROTECTION_STATUS0);
			printk("Check the Protected Memory Access of Blk[%s]!! violation address:[0x%08X]\n", blk_name[i], violated_addr);

			protected_base = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_MEM_PROTECTION_BASE);
			protected_end = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_MEM_PROTECTION_END);
			printk("protected area1:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base & 0xFF, protected_end & 0xFF);
			printk("protected area2:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base >> 8, protected_end >> 8);
			//printk("Check the Protected Memory Access of Blk[%s]!! reg:[0x%08X]\n", lgbus_ctrl.mnt_blk[i].name, reg_val);
			reg_val &= ~MEM_PROTECT_INTR_EN; // disable protected memory access intr
		}

		/*
		   ; clear interrupt
		 */
		reg_val |= (intr_status << 8);
		lgbus_reg_write(reg_val, lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}
	lgbus_access_stop();

	return IRQ_HANDLED;
}


/*
   ; file operation function
 */
static int lgbus_open(void)
{
	int i, rc;
	int bus_idx;

	lgbus_trace();

	lgbus_ctrl.reg_base[LGBUS_TYPE_L]	= (unsigned int)ioremap_nocache(LG1154_LBUS_REG_BASE, 0x2000);
	lgbus_ctrl.reg_base[LGBUS_TYPE_G]	= (unsigned int)ioremap_nocache(LG1154_GBUS_REG_BASE, 0x2000);

	lgbus_debug("lgbus_init, Lbus vaddr[0x%8x], Lbus vaddr[0x%8x]\n", lgbus_ctrl.reg_base[LGBUS_TYPE_L], lgbus_ctrl.reg_base[LGBUS_TYPE_G]);

	if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		lgbus_ctrl.mnt_blk = mnt_blk_info_h13b0;
	}
	else
	{
		lgbus_ctrl.mnt_blk = mnt_blk_info_h13a0;
	}

	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		bus_idx = lgbus_ctrl.mnt_blk[i].bus_idx;
		lgbus_ctrl.mnt_blk[i].reg_base = (UINT32)(lgbus_ctrl.reg_base[bus_idx]) + lgbus_ctrl.mnt_blk[i].reg_offset;
		//lgbus_debug("[%d] [%d]BUS base addr : 0x%X\n", i, bus_idx, lgbus_ctrl.mnt_blk[i].reg_base);
	}

	lgbus_ctrl.max_bw = LGBUS_DDR_TOTAL_BW;

	/*
	   ; initialize completion
	   ; this completion operate with lbus timer done interrupt handler.
	 */
	init_completion(&lgbus_ctrl.completion);


	/*
	   ; install interrupt handler.
	 */
	if ((rc = request_irq(H13_IRQ_LBUS, lgbus_isr, IRQF_SHARED, "LGBUS_L", (void*)&lgbus_ctrl)))
	{
		LGBUS_ERROR("request_irq failed. rc=%d\n", rc);
	}

	if ((rc = request_irq(H13_IRQ_GBUS, lgbus_isr, IRQF_SHARED, "LGBUS_G", (void*)&lgbus_ctrl)))
	{
		LGBUS_ERROR("request_irq failed. rc=%d\n", rc);
	}

	/* Success */
	return 0;
}

static int lgbus_close(void)
{
	lgbus_trace();

	free_irq (H13_IRQ_LBUS, (void*)&lgbus_ctrl);
	free_irq (H13_IRQ_GBUS, (void*)&lgbus_ctrl);

	iounmap((void*)lgbus_ctrl.reg_base[LGBUS_TYPE_L]);
	iounmap((void*)lgbus_ctrl.reg_base[LGBUS_TYPE_G]);

	return 0;
}


#if 0
static void lgbus_enable_intr(bus_intr_type_e i_type)
{
	int i;
	UINT32 reg_val;

	lgbus_trace();

	lgbus_access_start();
#if 0
	reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_reg_write((reg_val)|(1 << i_type), lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
#else
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		lgbus_reg_write((reg_val)|(1 << i_type), lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}
#endif
	lgbus_access_stop();

	lgbus_debug("cpu timer intr is enabled!!\n");
}

static void lgbus_disable_intr(bus_intr_type_e i_type)
{
	UINT32 reg_val;

	lgbus_trace();

	lgbus_access_start();
	reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_reg_write((reg_val)&(~(1 << i_type)), lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_access_stop();
}

void lgbus_auto_start(void)
{
	unsigned int r_val, i;

	lgbus_trace();

	/*
	   ; check requested monitoring blks.
	 */
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		r_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		lgbus_reg_write((r_val | TMR_AUTO_ENABLE), lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}
}
#endif

static int lgbus_mon_getinfo(LX_LGBUS_MON_INFO_T *info)
{
	int i;

	if(info == NULL) return -1;

	memset(info, 0, sizeof(LX_LGBUS_MON_INFO_T));
	info->max_bw = lgbus_ctrl.max_bw;
	info->blk_count = LGBUS_MAX_BLK;
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		strlcpy(info->name[i], blk_name[i], LGBUS_MAX_BLKNAME);
	}
	return 0;
}

static int lgbus_mon_start(LX_LGBUS_MON_CFG_T *cfg)
{
	int i;
	UINT32 reg_val;

	lgbus_trace();

	lgbus_ctrl.mon_cfg = *cfg;

	lgbus_access_start();
	/*
	 * enable cpu main monitoring timer done interrupt.
	 */
#if 0
	reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_reg_write((reg_val| TMR_DONE_INTR_EN), lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
#else
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		lgbus_reg_write((reg_val| TMR_DONE_INTR_EN), lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}
#endif
	/*
	 * set timer init value
	 */
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_ctrl.mnt_blk[i].clock / cfg->freq;
		lgbus_reg_write(reg_val, lgbus_ctrl.mnt_blk[i].reg_base + MNT_TIMER_SETTING);
	}
	lgbus_access_stop();

	return 0;
}

static int lgbus_mon_stop(void)
{
	unsigned int reg_val, i;

	lgbus_trace();

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		lgbus_reg_write((reg_val & ~TMR_ENABLE), lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}

	/*
	 * disable cpu main monitoring timer done interrupt.
	 */
	reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_reg_write((reg_val & ~TMR_DONE_INTR_EN), lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);

	lgbus_access_stop();

	return 0;
}


#define invert_bux_idx(i)	((i==LGBUS_TYPE_L) ? LGBUS_TYPE_G : LGBUS_TYPE_L)
static int lgbus_mon_read(LX_LGBUS_MON_DATA_T *data)
{
	int i, j;
	int bus_idx, port = 0;
	UINT32 reg_val;

	lgbus_trace();

	if(data == NULL) return -1;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
	reinit_completion(&lgbus_ctrl.completion);
#else
	INIT_COMPLETION(lgbus_ctrl.completion);
#endif

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		if(i == LGBUS_CPU_MAIN) continue;
		reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
		lgbus_reg_write((reg_val | TMR_ENABLE), lgbus_ctrl.mnt_blk[i].reg_base + MNT_INTR_CTRL);
	}

	/* Start the CPU_MAIN timer at the end to wait for the others becausse we use only this interrupt */
	reg_val = lgbus_reg_read(lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
	lgbus_reg_write((reg_val | TMR_ENABLE), lgbus_ctrl.mnt_blk[LGBUS_CPU_MAIN].reg_base + MNT_INTR_CTRL);
//	lgbus_ctrl.start = sched_clock();

	lgbus_access_stop();

	if(wait_for_completion_timeout(&lgbus_ctrl.completion, msecs_to_jiffies(2000)) == 0)
	{
		printk("timeout !!!\n");
		return -1;
	}

	memset(data, 0, sizeof(LX_LGBUS_MON_DATA_T));
	data->max_bus_num = 2; /* L/G */
	data->hw.max_bw = lgbus_ctrl.max_bw;
	data->hw.blk_count = LGBUS_MAX_BLK;
	data->cfg = lgbus_ctrl.mon_cfg;

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		bus_idx = lgbus_ctrl.mnt_blk[i].bus_idx;

		// TODO: get the port configuration in lgbus_open
		port = (lgbus_reg_read(lgbus_ctrl.mnt_blk[i].reg_base + MNT_ADDR_SWITCH0) >> 24) & 0x3;

		data->raw[i].bus	= bus_idx;
		data->raw[i].port	= port;

		/*write bandwidth*/
		data->raw[i].rw[LGBUS_WRITE].bw			= lgbus_blk_reg_read(i, MNT_WR_BW_VALUE);
//		data->raw[i].rw[LGBUS_WRITE].bw_bud  	= lgbus_blk_reg_read(i, MNT_WR_BW_BUDGET);
		data->raw[i].rw[LGBUS_WRITE].req_val 	= lgbus_blk_reg_read(i, MNT_WR_REQ_VALUE);
		data->raw[i].rw[LGBUS_WRITE].sum_ltn 	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_SUM);
		data->raw[i].rw[LGBUS_WRITE].max_ltn 	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_MAX);
		data->raw[i].rw[LGBUS_WRITE].ltn_bud	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_BUDGET);
		data->raw[i].rw[LGBUS_WRITE].other		= lgbus_blk_reg_read(i, MNT_WR_OTHER_BUS_BW);

		/*read bandwidth*/
		data->raw[i].rw[LGBUS_READ].bw 			= lgbus_blk_reg_read(i, MNT_RD_BW_VALUE);
//		data->raw[i].rw[LGBUS_READ].bw_bud 		= lgbus_blk_reg_read(i, MNT_RD_BW_BUDGET);
		data->raw[i].rw[LGBUS_READ].req_val 	= lgbus_blk_reg_read(i, MNT_RD_REQ_VALUE);
		data->raw[i].rw[LGBUS_READ].sum_ltn 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_SUM);
		data->raw[i].rw[LGBUS_READ].max_ltn 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_MAX);
		data->raw[i].rw[LGBUS_READ].ltn_bud 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_BUDGET);
		data->raw[i].rw[LGBUS_READ].other		= lgbus_blk_reg_read(i, MNT_RD_OTHER_BUS_BW);


		/* calucate bw data for each block */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			if(data->raw[i].rw[j].req_val == 0)
				data->blk[i].rw[j].avg_ltn = 0;
			else
				data->blk[i].rw[j].avg_ltn = data->raw[i].rw[j].sum_ltn / data->raw[i].rw[j].req_val;
		}


		// work around of H/W problem
		if (lgbus_ctrl.mnt_blk[i].idx == LGBUS_CPU_MAIN ||
			lgbus_ctrl.mnt_blk[i].idx == LGBUS_CPU_SUB ||
			lgbus_ctrl.mnt_blk[i].idx == LGBUS_DE_E_CVD)
		{
			data->raw[i].rw[LGBUS_WRITE].other = 0;
			data->raw[i].rw[LGBUS_READ].other = 0;
		}
		// work around of H/W problem
		else if(lgbus_ctrl.mnt_blk[i].idx == LGBUS_DPPA)
		{
			bus_idx = LGBUS_TYPE_L;
			data->raw[i].bus = bus_idx;
		}
		// Move TCON to LBUS statically
		else if(lgbus_ctrl.mnt_blk[i].idx == LGBUS_TCON)
		{
			bus_idx = LGBUS_TYPE_L;
			data->raw[i].bus = bus_idx;

			data->raw[i].rw[LGBUS_WRITE].other = 0;
			data->raw[i].rw[LGBUS_READ].other = 0;
		}


		/* calcuate bw for each bus */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[bus_idx].rw[j].port_bw[port] += (data->raw[i].rw[j].bw - data->raw[i].rw[j].other);

			// TODO: port other bus is not same current
			if(data->raw[i].rw[j].bw == data->raw[i].rw[j].other)
				data->bus[invert_bux_idx(bus_idx)].rw[j].port_bw[port] += data->raw[i].rw[j].other;

			data->bus[bus_idx].rw[j].other_bw += data->raw[i].rw[j].other;
			data->bus[bus_idx].rw[j].score += data->raw[i].rw[j].sum_ltn ;
		}
	}

	for(i=0; i<LGBUS_MAX_DDR_PORT; i++)
	{
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[LGBUS_TYPE_L].rw[j].total_bw += data->bus[LGBUS_TYPE_L].rw[j].port_bw[i];
			data->bus[LGBUS_TYPE_G].rw[j].total_bw += data->bus[LGBUS_TYPE_G].rw[j].port_bw[i];
		}
	}

	for(j=0; j<LGBUS_RW_MAX; j++)
	{
		data->bus[LGBUS_TYPE_L].total_bw += data->bus[LGBUS_TYPE_L].rw[j].total_bw;
		data->bus[LGBUS_TYPE_G].total_bw += data->bus[LGBUS_TYPE_G].rw[j].total_bw;
	}

	lgbus_access_stop();

	return 0;
}

static int lgbus_init(void)
{
	if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		spin_lock_init(&access_ctrl.lock);
		access_ctrl.refcnt = 0;
		access_ctrl.start	= reg_access_start_b0;
		access_ctrl.stop	= reg_access_stop_b0;
	}

	return 0;
}

static LGBUS_HW_FUNC_T hw_func =
{
	.init	= lgbus_init,
	.open	= lgbus_open,
	.close	= lgbus_close,

	.mon_getinfo= lgbus_mon_getinfo,
	.mon_start	= lgbus_mon_start,
	.mon_read	= lgbus_mon_read,
	.mon_stop	= lgbus_mon_stop,
};


LGBUS_HW_FUNC_T* LGBUS_H13_Init(void)
{
	return &hw_func;
}

