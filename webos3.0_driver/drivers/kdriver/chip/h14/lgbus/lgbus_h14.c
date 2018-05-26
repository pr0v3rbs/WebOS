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
#include "lgbus_h14.h"



/*----------------------------------------------------------------------------------------
  Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
----------------------------------------------------------------------------------------*/
#define LGBUS_ENABLE_MEM_PROTECT	1
//#define LGBUS_TOOL_INFORMATION		1

#define LGBUS_IRQ_DEBUG		0

#if 0
#define lgbus_debug(fmt, args...)		printk(fmt, ##args)
#else
#define lgbus_debug(fmt, args...)		do{}while(0)
#endif

#if 0
#define lgbus_trace()					printk("%s:%d\n", __func__, __LINE__)
#else
#define lgbus_trace()					do{}while(0)
#endif

#define lgbus_error(fmt, args...)		printk(KERN_ERR"\x1b[31m"fmt"\x1b[0m\n", ##args)


#define lgbus_reg_read(addr)			__raw_readl((void*)(addr))
#define lgbus_reg_write(val,addr)		__raw_writel(val, (void*)(addr))

#define lgbus_blk_reg_write(idx, val, reg)	lgbus_reg_write(val, lgbus_ctrl.mnt_blk[idx].reg_base + (reg))
#define lgbus_blk_reg_read(idx, reg)		lgbus_reg_read(lgbus_ctrl.mnt_blk[idx].reg_base + (reg))

#define lgbus_access_start()		do{}while(0)
#define lgbus_access_stop()			do{}while(0)
/*----------------------------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	const char	*name;
	int			type;
	UINT32		reg_offset;
	int			bus_idx;
	UINT32		clock;

	UINT32		reg_base;
	int			port[LGBUS_MEM_TYPE_MAX];

#if LGBUS_IRQ_DEBUG
	UINT32		irq_elapsed;
#endif
} LGBUS_H14_BLK_INFO_T;


typedef struct
{
	UINT32					reg_base[LGBUS_REG_MAX];
	LGBUS_H14_BLK_INFO_T	*mnt_blk;

	UINT64					max_bw;
	struct completion		completion;

	spinlock_t				lock;

	LX_LGBUS_MON_CFG_T		mon_cfg;
	int						mon_status;

	UINT32					irq_status;
#if LGBUS_IRQ_DEBUG
	unsigned long long		start;
#endif
} LGBUS_H14_CTRL_T;


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
static LGBUS_H14_BLK_INFO_T mnt_blk_h14a0[] =
{
/*	{name,			type,					reg_offset,				bus_idx,	  clock,	 } */
	{"CPU_MAIN",	LGBUS_REG_TYPE_CCI,		MNT_CPU_MAIN_OFFSET,	LGBUS_TYPE_L, (400*MHZ), }, /* CPU_MAIN */
	{"CPU_PERI",	LGBUS_REG_TYPE_CCI,		MNT_CPU_PERI_OFFSET,	LGBUS_TYPE_L, (400*MHZ), }, /* CPU_PERI */
	{"GPU_CCI",		LGBUS_REG_TYPE_CCI,		MNT_GPU_CCI_OFFSET,		LGBUS_TYPE_L, (400*MHZ), }, /* GPU_CCI */

	{"AUD",			LGBUS_REG_TYPE_NOCL,	MNT_AUD_OFFSET,			LGBUS_TYPE_L, (400*MHZ), },	/* AUD */	/* 3 */
	{"GFX",			LGBUS_REG_TYPE_NOCL,	MNT_GFX_OFFSET,			LGBUS_TYPE_L, (400*MHZ), }, /* GFX */
	{"GPU",			LGBUS_REG_TYPE_NOCL,	MNT_GPU_OFFSET,			LGBUS_TYPE_L, (400*MHZ), }, /* GPU */
	{"ICOD",		LGBUS_REG_TYPE_NOCL,	MNT_ICOD_OFFSET,		LGBUS_TYPE_L, (200*MHZ), }, /* ICOD */
	{"TCON",		LGBUS_REG_TYPE_NOCL,	MNT_TCON_OFFSET,		LGBUS_TYPE_L, (200*MHZ), }, /* TCON */
	{"TE",			LGBUS_REG_TYPE_NOCL,	MNT_TE_OFFSET,			LGBUS_TYPE_L, (200*MHZ), },	/* TE */
	{"VDEC",		LGBUS_REG_TYPE_NOCL,	MNT_VDEC_OFFSET,		LGBUS_TYPE_L, (400*MHZ), },	/* VDEC */
	{"VDO",			LGBUS_REG_TYPE_NOCL,	MNT_VDO_OFFSET,			LGBUS_TYPE_L, (400*MHZ), },	/* VDO */
	{"VENC",		LGBUS_REG_TYPE_NOCL,	MNT_VENC_OFFSET,		LGBUS_TYPE_L, (320*MHZ), },	/* VENC */

	{"BVE_MCU",		LGBUS_REG_TYPE_NOCG,	MNT_BVE_MCU_OFFSET,		LGBUS_TYPE_G, (200*MHZ), }, /* BVE_MCU */	/* 12 */
	{"DE_A",		LGBUS_REG_TYPE_NOCG,	MNT_DE_A_OFFSET,		LGBUS_TYPE_G, (200*MHZ), }, /* DE_A */
	{"DE_B",		LGBUS_REG_TYPE_NOCG,	MNT_DE_B_OFFSET,		LGBUS_TYPE_G, (200*MHZ), }, /* DE_B */
	{"DE_C",		LGBUS_REG_TYPE_NOCG,	MNT_DE_C_OFFSET,		LGBUS_TYPE_G, (200*MHZ), }, /* DE_C */
	{"DE_D",		LGBUS_REG_TYPE_NOCG,	MNT_DE_D_OFFSET,		LGBUS_TYPE_G, (200*MHZ), }, /* DE_D */
	{"DE_E_CVD",	LGBUS_REG_TYPE_NOCG,	MNT_DE_E_CVD_OFFSET,	LGBUS_TYPE_G, (200*MHZ), }, /* DE_E_CVD */
	{"DE_E_MCU",	LGBUS_REG_TYPE_NOCG,	MNT_DE_E_MCU_OFFSET,	LGBUS_TYPE_G, (200*MHZ), }, /* DE_E_MCU */
	{"DPPA",		LGBUS_REG_TYPE_NOCG,	MNT_DPPA_OFFSET,		LGBUS_TYPE_G, (400*MHZ), }, /* DPPA */
	{"DVI",			LGBUS_REG_TYPE_NOCG,	MNT_DVI_OFFSET,			LGBUS_TYPE_G, (200*MHZ), }, /* DVI */
	{"DVO",			LGBUS_REG_TYPE_NOCG,	MNT_DVO_OFFSET,			LGBUS_TYPE_G, (300*MHZ), }, /* DVO */
	{"MC",			LGBUS_REG_TYPE_NOCG,	MNT_MC_OFFSET,			LGBUS_TYPE_G, (226*MHZ), }, /* MC */
	{"MEP_A",		LGBUS_REG_TYPE_NOCG,	MNT_MEPA_OFFSET,		LGBUS_TYPE_G, (264*MHZ), }, /* MEP_A */
	{"MEP_B",		LGBUS_REG_TYPE_NOCG,	MNT_MEPB_OFFSET,		LGBUS_TYPE_G, (264*MHZ), }, /* MEP_B */
	{"SRE",			LGBUS_REG_TYPE_NOCG,	MNT_SRE_OFFSET,			LGBUS_TYPE_G, (200*MHZ), }, /* SRE */
};


static LGBUS_H14_CTRL_T lgbus_ctrl;


static irqreturn_t lgbus_isr(int irq, void *dev)
{
	int i, type;
	UINT32 reg_val, intr_status;
	LGBUS_H14_BLK_INFO_T *mnt_blk;
#if LGBUS_IRQ_DEBUG
	unsigned long long now = sched_clock();
#endif

//	printk("lgbus_isr, irq number[%d]\n", irq);

	if(irq == H14_IRQ_LBUS) type = LGBUS_REG_TYPE_NOCL;
	else if(irq == H14_IRQ_GBUS) type = LGBUS_REG_TYPE_NOCG;
	else type = LGBUS_REG_TYPE_CCI;

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		mnt_blk = &lgbus_ctrl.mnt_blk[i];
		if(mnt_blk->type != type && mnt_blk->type != LGBUS_REG_TYPE_CCI) continue;

		reg_val = lgbus_reg_read(mnt_blk->reg_base + MNT_INTR_CTRL);
		intr_status = reg_val & MNT_INTR_STATUS_MASK;

		if(!intr_status) continue;

		lgbus_debug("%s irq status : 0x%08x\n", mnt_blk->name, reg_val);

		if(intr_status & TMR_DONE_INTR_STATUS)
		{
			lgbus_ctrl.irq_status |= (0x1 << i);

		#if LGBUS_IRQ_DEBUG
			lgbus_ctrl.mnt_blk[i].irq_elapsed = (UINT32)((int)(now-lgbus_ctrl.start)/1000);
		#endif

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
				//complete(&lgbus_ctrl.completion);

			}
		}

		if(intr_status & MEM_PROTECT_INTR_STATUS)  // protected memory access
		{
			UINT32 violated_addr, violated_id, protected_base, protected_end;
			violated_addr = lgbus_reg_read(mnt_blk->reg_base + MNT_MEM_PROTECTION_STATUS0);
			violated_id = lgbus_reg_read(mnt_blk->reg_base + MNT_MEM_PROTECTION_STATUS1);

			lgbus_error("\n\nERROR : [%s] accessed protected memory[0x%08X] - id[%d]", mnt_blk->name, violated_addr, violated_id);

			protected_base = lgbus_reg_read(mnt_blk->reg_base + MNT_MEM_PROTECTION_BASE);
			protected_end = lgbus_reg_read(mnt_blk->reg_base + MNT_MEM_PROTECTION_END);
			printk("protected area1:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base & 0xFF, protected_end & 0xFF);
			printk("protected area2:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base >> 8, protected_end >> 8);
			printk("\n\n\n");
			reg_val &= ~MEM_PROTECT_INTR_EN; // disable protected memory access intr
		}

		/*
		   ; clear interrupt
		 */
		reg_val |= (intr_status << 8);
		lgbus_reg_write(reg_val, mnt_blk->reg_base + MNT_INTR_CTRL);
	}
	lgbus_access_stop();


	do {
		const UINT32 cci_mask = (0x1<<LGBUS_CPU_MAIN | 0x1<<LGBUS_CPU_PERI | 0x1<<LGBUS_GPU_CCI);
		const UINT32 irq_mask = (((1 << LGBUS_MAX_BLK) - 1) & ~cci_mask);

		if((lgbus_ctrl.irq_status&~cci_mask) == irq_mask)
		{
#if LGBUS_IRQ_DEBUG
			printk("LGBUS : IRQ Elapsed Time in %dHZ\n", lgbus_ctrl.mon_cfg.freq);
			for(i=0; i<LGBUS_MAX_BLK; i++)
			{
				printk("%-8s] %dms(%dus)\n", lgbus_ctrl.mnt_blk[i].name,
						lgbus_ctrl.mnt_blk[i].irq_elapsed/1000, lgbus_ctrl.mnt_blk[i].irq_elapsed);
			}
#endif
			complete(&lgbus_ctrl.completion);
		}
	}while(0);

	return IRQ_HANDLED;
}


/*
   ; file operation function
 */
static int lgbus_open(void)
{
	lgbus_trace();
	return 0;
}

static int lgbus_close(void)
{
	lgbus_trace();
	return 0;
}

static int lgbus_mon_getinfo(LX_LGBUS_MON_INFO_T *info)
{
	int i;

	if(info == NULL) return -1;

	memset(info, 0, sizeof(LX_LGBUS_MON_INFO_T));
	info->max_bw = lgbus_ctrl.max_bw;
	info->blk_count = LGBUS_MAX_BLK;
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		strlcpy(info->name[i], lgbus_ctrl.mnt_blk[i].name, LGBUS_MAX_BLKNAME);
		info->clock[i] = lgbus_ctrl.mnt_blk[i].clock;
	}
	return 0;
}

static void start_mon_timer(void)
{
	int i;
	UINT32 reg_val;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
	reinit_completion(&lgbus_ctrl.completion);
#else
	INIT_COMPLETION(lgbus_ctrl.completion);
#endif
	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
#if LGBUS_IRQ_DEBUG
		lgbus_ctrl.mnt_blk[i].irq_elapsed = 0;
#endif
		reg_val = lgbus_blk_reg_read(i, MNT_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val | TMR_ENABLE), MNT_INTR_CTRL);
	}
	lgbus_access_stop();

#if LGBUS_IRQ_DEBUG
	lgbus_ctrl.start = sched_clock();
#endif
	lgbus_ctrl.irq_status = 0;
}


static int lgbus_mon_start(LX_LGBUS_MON_CFG_T *cfg)
{
	int i;
	UINT32 reg_val;
	unsigned long flags;

	lgbus_trace();

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	if(lgbus_ctrl.mon_status != 0)
	{
		spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);
		return -1;
	}
	lgbus_ctrl.mon_status = 1;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);


	lgbus_ctrl.mon_cfg = *cfg;

	lgbus_access_start();

#if 0
	/*
	 * enable cpu main monitoring timer done interrupt.
	 */
	reg_val = lgbus_blk_reg_read(LGBUS_CPU_MAIN, MNT_INTR_CTRL);
	lgbus_reg_write(LGBUS_CPU_MAIN, (reg_val| TMR_DONE_INTR_EN), MNT_INTR_CTRL);
#else
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, MNT_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val| TMR_DONE_INTR_EN), MNT_INTR_CTRL);
	}
#endif
	/*
	 * set timer init value
	 */
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_ctrl.mnt_blk[i].clock / cfg->freq;
		lgbus_blk_reg_write(i, reg_val, MNT_TIMER_SETTING);
	}

	/* get port value */
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		int j;
		UINT8 start, offset, port;
		int mport[2] = {-1, -1};

		if(lgbus_ctrl.mnt_blk[i].type == LGBUS_REG_TYPE_CCI)
		{
			lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_0] = 0;
			lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_1] = 0;
			continue;
		}

		for(j=0; j<8; j++)
		{
			reg_val = lgbus_blk_reg_read(i, MNT_ADDR_SWITCH0 + j*4);

			start	= (reg_val >>  0) & 0xFF;
			offset	= (reg_val >> 16) & 0xFF;
			port	= (reg_val >> 24) & 0x03;

			if((UINT8)(start + offset) < 0x80)
			{
				if(mport[LGBUS_MEM_TYPE_0] == -1) mport[LGBUS_MEM_TYPE_0] = port;
			}
			else
			{
				if(mport[LGBUS_MEM_TYPE_1] == -1) mport[LGBUS_MEM_TYPE_1] = port;
			}

			if(mport[LGBUS_MEM_TYPE_0] != -1 && mport[LGBUS_MEM_TYPE_1] != -1)
				break;
		}
		lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_0] = mport[LGBUS_MEM_TYPE_0];
		lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_1] = mport[LGBUS_MEM_TYPE_1];

		lgbus_debug("%s : m0p%d, m1p%d\n", lgbus_ctrl.mnt_blk[i].name,
						lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_0],
						lgbus_ctrl.mnt_blk[i].port[LGBUS_MEM_TYPE_1]);
	}
	lgbus_access_stop();

	if(cfg->mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}

	return 0;
}

static int lgbus_mon_stop(void)
{
	UINT32 reg_val, i;
	unsigned long flags;

	lgbus_trace();

	if(lgbus_ctrl.mon_status != 1)
		return -1;

	lgbus_access_start();
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, MNT_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val & ~TMR_ENABLE), MNT_INTR_CTRL);
	}

	/*
	 * disable cpu main monitoring timer done interrupt.
	 */
#if 0
	reg_val = lgbus_blk_reg_read(LGBUS_CPU_MAIN, MNT_INTR_CTRL);
	lgbus_reg_write(LGBUS_CPU_MAIN, (reg_val & ~TMR_DONE_INTR_EN), MNT_INTR_CTRL);
#else
	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, MNT_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val & ~TMR_DONE_INTR_EN), MNT_INTR_CTRL);
	}
#endif

	lgbus_access_stop();

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	lgbus_ctrl.mon_status = 0;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);

	return 0;
}


#define invert_bux_idx(i)	((i==LGBUS_TYPE_L) ? LGBUS_TYPE_G : LGBUS_TYPE_L)
static int lgbus_mon_read(LX_LGBUS_MON_DATA_T *data)
{
	int i, j;
	int bus_idx, port, port_other;

	lgbus_trace();

	if(lgbus_ctrl.mon_status != 1)
		return -1;

	if(data == NULL) return -1;

	if(lgbus_ctrl.mon_cfg.mode == LGBUS_MON_MODE_AUTO)
	{
		if(try_wait_for_completion(&lgbus_ctrl.completion) == 0)
			return -EBUSY;
	}
	else
	{
		int timeout = 2000/lgbus_ctrl.mon_cfg.freq;
		start_mon_timer();
		if(wait_for_completion_timeout(&lgbus_ctrl.completion, msecs_to_jiffies(timeout)) == 0)
		{
			printk("timeout !!!\n");
			return -1;
		}
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

		port = lgbus_ctrl.mnt_blk[i].port[bus_idx];
		port_other = lgbus_ctrl.mnt_blk[i].port[invert_bux_idx(bus_idx)];

		data->raw[i].bus	= bus_idx;
		data->raw[i].port	= port;
		data->raw[i].port_other = port_other;

		/*write bandwidth*/
	#if 0	/* it's different from the previous chip */
		data->raw[i].rw[LGBUS_WRITE].bw			= lgbus_blk_reg_read(i, MNT_WR_BW_VALUE);
		data->raw[i].rw[LGBUS_WRITE].other		= lgbus_blk_reg_read(i, MNT_WR_OTHER_BUS_BW);
	#else
		data->raw[i].rw[LGBUS_WRITE].bw			= lgbus_blk_reg_read(i, MNT_WR_BW_VALUE);
		data->raw[i].rw[LGBUS_WRITE].other		= data->raw[i].rw[LGBUS_WRITE].bw - lgbus_blk_reg_read(i, MNT_WR_OTHER_BUS_BW);
	#endif

		data->raw[i].rw[LGBUS_WRITE].req_val 	= lgbus_blk_reg_read(i, MNT_WR_REQ_VALUE);
		data->raw[i].rw[LGBUS_WRITE].sum_ltn 	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_SUM);
		data->raw[i].rw[LGBUS_WRITE].max_ltn 	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_MAX);
		data->raw[i].rw[LGBUS_WRITE].ltn_bud	= lgbus_blk_reg_read(i, MNT_WR_LATENCY_BUDGET);

		/*read bandwidth*/
	#if 0
		data->raw[i].rw[LGBUS_READ].bw 			= lgbus_blk_reg_read(i, MNT_RD_BW_VALUE);
		data->raw[i].rw[LGBUS_READ].other		= lgbus_blk_reg_read(i, MNT_RD_OTHER_BUS_BW);
	#else
		data->raw[i].rw[LGBUS_READ].bw			= lgbus_blk_reg_read(i, MNT_RD_BW_VALUE);
		data->raw[i].rw[LGBUS_READ].other		= data->raw[i].rw[LGBUS_READ].bw - lgbus_blk_reg_read(i, MNT_RD_OTHER_BUS_BW);
	#endif
		data->raw[i].rw[LGBUS_READ].req_val 	= lgbus_blk_reg_read(i, MNT_RD_REQ_VALUE);
		data->raw[i].rw[LGBUS_READ].sum_ltn 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_SUM);
		data->raw[i].rw[LGBUS_READ].max_ltn 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_MAX);
		data->raw[i].rw[LGBUS_READ].ltn_bud 	= lgbus_blk_reg_read(i, MNT_RD_LATENCY_BUDGET);

		/* calucate bw data for each block */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			if(data->raw[i].rw[j].req_val == 0)
				data->blk[i].rw[j].avg_ltn = 0;
			else
				data->blk[i].rw[j].avg_ltn = data->raw[i].rw[j].sum_ltn / data->raw[i].rw[j].req_val;
		}

		/* calcuate bw for each bus */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[bus_idx].rw[j].port_bw[port] += (data->raw[i].rw[j].bw - data->raw[i].rw[j].other);

		//	if(data->raw[i].rw[j].bw == data->raw[i].rw[j].other)
			data->bus[invert_bux_idx(bus_idx)].rw[j].port_bw[port_other] += data->raw[i].rw[j].other;

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

	if(lgbus_ctrl.mon_cfg.mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}

	return 0;
}

static int lgbus_init(void)
{
	int i, rc;
	int type;

	lgbus_trace();

	lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL] = (UINT32)ioremap_nocache(H14_LBUS_REG_BASE, 0x4000);
	lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG] = (UINT32)ioremap_nocache(H14_GBUS_REG_BASE, 0x4000);
	lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI] = (UINT32)ioremap_nocache(H14_CCI_REG_BASE, 0x1000);
	if(lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL] == 0 ||
		lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG] == 0 ||
		lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI] == 0)
	{
		LGBUS_ERROR("can't ioremap !!!\n");
		goto exit;
	}

	lgbus_debug("lgbus_init, L[0x%8x],G[0x%8x],CCI[0x%08x]\n",
		lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL],
		lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG],
		lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI]);

	lgbus_ctrl.mnt_blk = mnt_blk_h14a0;

	for(i = 0; i < LGBUS_MAX_BLK; i++)
	{
		type = lgbus_ctrl.mnt_blk[i].type;
		lgbus_ctrl.mnt_blk[i].reg_base = (UINT32)(lgbus_ctrl.reg_base[type]) + lgbus_ctrl.mnt_blk[i].reg_offset;
		//lgbus_debug("[%d] [%d]BUS base addr : 0x%X\n", i, bus_idx, lgbus_ctrl.mnt_blk[i].reg_base);
	}
	lgbus_ctrl.max_bw = H14_SOC_DDR_TOTAL_BW;

	/*
	   ; initialize completion
	   ; this completion operate with lbus timer done interrupt handler.
	 */
	init_completion(&lgbus_ctrl.completion);

	spin_lock_init(&lgbus_ctrl.lock);
	lgbus_ctrl.mon_status = 0;

	/*
	   ; install interrupt handler.
	 */
	if((rc = request_irq(H14_IRQ_LBUS, lgbus_isr, IRQF_SHARED, "LGBUS_L", (void*)&lgbus_ctrl)))
		goto err_lbus;

	if((rc = request_irq(H14_IRQ_GBUS, lgbus_isr, IRQF_SHARED, "LGBUS_G", (void*)&lgbus_ctrl)))
		goto err_gbus;

	if((rc = request_irq(H14_IRQ_CCI_MON, lgbus_isr, IRQF_SHARED, "LGBUS_CCI", (void*)&lgbus_ctrl)))
		goto err_cci;

#ifdef LGBUS_TOOL_INFORMATION
	do {
		UINT32 reg_val;
		int bit, x, y;
		const char *tool_name[16] =
		{
			"TIMER", "ADDR_SWITCH", "MEM_PROTECT", "WBW_MONITOR", "RBW_MONITOR",
			"WLATENCY_MONITOR", "RLATENCY_MONITOR", "USER_PRI_CTRL", "CMD_DISTRIBUTE", "ADDR_REMAP",
			"SLAVE_BUSY_MONITOR", "PENDING_CTRL", "LATENCY_CTRL", "RORDER_CTRL", "WORDER_CTRL",
			"MMU"
		};

		printk("==============================================================================\n");
		printk("LGBUS MONITOR TOOL INFORMATION\n");
		printk("==============================================================================\n");

		for(bit=0; bit<16; bit++)
		{
			printk("%23s", "");
			for(y=0; y<bit; y++) printk("| ");
			printk("+-");
			for(x=15;x>=bit;x--) printk("--");
			printk(" %s\n", tool_name[bit]);
		}

		for(i = 0; i < LGBUS_MAX_BLK; i++)
		{
			reg_val = lgbus_blk_reg_read(i, MNT_TOOL_INFORMATION);
			printk("%10s : %s%d %02x", lgbus_ctrl.mnt_blk[i].name,
				((reg_val >> 16) & 0x01) ? "GPORT" : "LPORT",
				(reg_val >> 17) & 0x03,
				(reg_val >> 24) & 0xFF);
			for(bit=0; bit<16; bit++)
			{
				printk(" %c", ((reg_val >>  bit) & 0x01) ? 'O' : 'X');
			}
			printk("\n");
		}
		printk("==============================================================================\n");
	} while(0);
#endif

#ifdef LGBUS_ENABLE_MEM_PROTECT
	do {
		UINT32 reg_val;

		for(i = 0; i < LGBUS_MAX_BLK; i++)
		{
			if(lgbus_ctrl.mnt_blk[i].type == LGBUS_REG_TYPE_CCI) continue;

			reg_val = lgbus_blk_reg_read(i, MNT_INTR_CTRL);
			lgbus_blk_reg_write(i, (reg_val| MEM_PROTECT_INTR_EN), MNT_INTR_CTRL);
		}
	} while(0);
#endif

	LGBUS_INFO("LGBUS H14 Initialized\n");

	/* Success */
	return 0;

err_cci:
	free_irq (H14_IRQ_GBUS, (void*)&lgbus_ctrl);
err_gbus:
	free_irq (H14_IRQ_LBUS, (void*)&lgbus_ctrl);
err_lbus:
	LGBUS_ERROR("request_irq failed. rc=%d\n", rc);
exit:
	if(lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL]) iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL]);
	if(lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG]) iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG]);
	if(lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI]) iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI]);

	return -1;

}

static void lgbus_cleanup(void)
{
	lgbus_trace();

	free_irq (H14_IRQ_LBUS,	(void*)&lgbus_ctrl);
	free_irq (H14_IRQ_GBUS,	(void*)&lgbus_ctrl);
	free_irq (H14_IRQ_CCI_MON, (void*)&lgbus_ctrl);

	iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCL]);
	iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_NOCG]);
	iounmap((void*)lgbus_ctrl.reg_base[LGBUS_REG_TYPE_CCI]);
}

static LGBUS_HW_FUNC_T hw_func =
{
	.init		= lgbus_init,
	.cleanup	= lgbus_cleanup,
	.open		= lgbus_open,
	.close		= lgbus_close,

	.mon_getinfo	= lgbus_mon_getinfo,
	.mon_start		= lgbus_mon_start,
	.mon_read		= lgbus_mon_read,
	.mon_stop		= lgbus_mon_stop,
};


LGBUS_HW_FUNC_T* LGBUS_H14_Init(void)
{
	return &hw_func;
}

