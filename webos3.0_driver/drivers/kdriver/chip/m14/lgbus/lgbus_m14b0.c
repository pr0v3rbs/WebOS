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
 *  author		youngjun.jang (youngjun.jang@lge.com)
 *  version		1.0
 *  date		2013.10.14
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
#include "lgbus_m14b0.h"



/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#define LGBUS_ENABLE_MEM_PROTECT
//#define LGBUS_SETUP_MEM_PROTECT

//#define LGBUS_TOOL_INFORMATION
//#define LGBUS_IRQ_DEBUG


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

#define lgbus_error(fmt, args...)		printk(KERN_ERR"\x1b[31m"fmt"\x1b[0m\n", ##args)


#define lgbus_reg_read(addr)			__raw_readl((void*)(addr))
#define lgbus_reg_write(val,addr)		__raw_writel(val, (void*)(addr))

#define lgbus_blk_reg_write(idx, val, reg)	lgbus_reg_write(val, lgbus_ctrl.mnt_blk[idx].reg_base + (reg))
#define lgbus_blk_reg_read(idx, reg)		lgbus_reg_read(lgbus_ctrl.mnt_blk[idx].reg_base + (reg))

#define lgbus_blk_wtk_write(idx, val, reg)	lgbus_reg_write(val, lgbus_ctrl.mnt_blk[idx].reg_base + M14_B0_WR_TOOLKIT_OFFSET + (reg))
#define lgbus_blk_wtk_read(idx, reg)		lgbus_reg_read(lgbus_ctrl.mnt_blk[idx].reg_base + M14_B0_WR_TOOLKIT_OFFSET + (reg))

#define lgbus_top_reg_read(type, offset)		lgbus_reg_read(lgbus_ctrl.reg_base[type] + (offset))
#define lgbus_top_reg_write(val, type, offset)	lgbus_reg_write(val, lgbus_ctrl.reg_base[type] + (offset))

/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	addr0[2];	/* {base, end} */
	UINT32	addr1[2];	/* {base, end} */
} LGBUS_MEM_PROT_RANGE_T;


typedef struct
{
	const char	*name;
	int			reg_type;
	UINT32		reg_offset;
	int			bus_type;
	UINT32		intr_status;
	LGBUS_MEM_PROT_RANGE_T	*mprot;


	UINT32		reg_base;
	int			port[LGBUS_TYPE_MAX][LGBUS_RW_MAX];

#ifdef LGBUS_IRQ_DEBUG
	UINT32		irq_elapsed;
#endif

} LGBUS_M14_B0_BLK_INFO_T;

typedef struct
{
	const char	*name;
	UINT32		reg_offset;
	UINT32		reg_mask;
} LGBUS_M14_B0_REG_INFO_T;

typedef struct
{
	UINT32					reg_base[LGBUS_TYPE_MAX];
	LGBUS_M14_B0_BLK_INFO_T	*mnt_blk;
	LGBUS_M14_B0_REG_INFO_T	*mnt_blk_reg;

	UINT64					max_bw;
	struct completion		completion;

	spinlock_t				lock;

	LX_LGBUS_MON_CFG_T		mon_cfg;
	int 					mon_status;

	UINT32					irq_status;
#ifdef LGBUS_IRQ_DEBUG
	unsigned long long		start;
#endif

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

/*-----------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/* LSB 12bits are ignored. Valid bits are [31:12] */
static LGBUS_MEM_PROT_RANGE_T default_mprot_range =
{
	{0x00000000, 0x66FFF000},	/* 0x00000000-0x66FFFFFF */
	{0x80000000, 0xB4FFF000}	/* 0x80000000-0xB4FFFFFF */
};
#define MPROT	&default_mprot_range

#if 0
static LGBUS_MEM_PROT_RANGE_T xxx_mprot_range =
{
	{0x00000000, 0x76FFF000},	/* 0x00000000-0x76FFFFFF */
	{0x80000000, 0xB4FFF000}	/* 0x80000000-0xB4FFFFFF */
};
#endif

#define M14_B0_BLK_INFO(name, reg, mprot) \
	[M14_B0_##name] = {#name, M14_B0_REG_TYPE_##reg, M14_B0_##name##_OFFSET, M14_B0_REG_TYPE_##reg, M14_B0_INTR_STATUS_##name, mprot,}

static LGBUS_M14_B0_BLK_INFO_T mnt_blk_info_m14b0[] =
{
	/*				name,		reg,	mprot	*/
	M14_B0_BLK_INFO(CPU0,		ATOP,	NULL),
	M14_B0_BLK_INFO(CPU1,		ATOP,	NULL),
	M14_B0_BLK_INFO(CPU_PERI,	ATOP,	NULL),
	M14_B0_BLK_INFO(USB,		ATOP,	NULL),
	M14_B0_BLK_INFO(GPU0,		ATOP,	NULL),
	M14_B0_BLK_INFO(GPU1,		ATOP,	NULL),
	M14_B0_BLK_INFO(VDEC,		ATOP,	MPROT),
	M14_B0_BLK_INFO(VDEC_MCU,	ATOP,	MPROT),
	M14_B0_BLK_INFO(IDS_MCU,	ATOP,	MPROT),
	M14_B0_BLK_INFO(HEVC0,		ATOP,	MPROT),
	M14_B0_BLK_INFO(HEVC1,		ATOP,	MPROT),
	M14_B0_BLK_INFO(DPE,		BTOP,	MPROT),
	M14_B0_BLK_INFO(DPP,		BTOP,	MPROT),
	M14_B0_BLK_INFO(FDA,		BTOP,	MPROT),
	M14_B0_BLK_INFO(DEO,		BTOP,	MPROT),
	M14_B0_BLK_INFO(DEM,		BTOP,	MPROT),
	M14_B0_BLK_INFO(DEI,		BTOP,	MPROT),
	M14_B0_BLK_INFO(CVD,		BTOP,	MPROT),
	M14_B0_BLK_INFO(TE,			BTOP,	MPROT),
	M14_B0_BLK_INFO(FDD0,		BTOP,	NULL),		// FDD0 is not exist write toolkit
	M14_B0_BLK_INFO(FDD1,		BTOP,	MPROT),
	M14_B0_BLK_INFO(FDB,		BTOP,	MPROT),
	M14_B0_BLK_INFO(FDC,		BTOP,	MPROT),
	M14_B0_BLK_INFO(VENC,		BTOP,	MPROT),
	M14_B0_BLK_INFO(GFX,		BTOP,	MPROT),
	M14_B0_BLK_INFO(AUD,		BTOP,	MPROT),
};

#define M14_B0_REG_INFO(name) \
	[M14_B0_REG_##name] = {#name, M14_B0_REG_##name##_OFFSET, M14_B0_REG_##name##_MASK}

static LGBUS_M14_B0_REG_INFO_T mnt_reg_info_m14b0[] =
{
	M14_B0_REG_INFO(PRIORITY),
	M14_B0_REG_INFO(BANDWIDTH),
};

static LGBUS_CTRL_T lgbus_ctrl;


static irqreturn_t lgbus_isr_b0(int irq, void *dev)
{
	int i, type;
	UINT32 reg_val, intr_status;
	LGBUS_M14_B0_BLK_INFO_T *mnt_blk;

#ifdef LGBUS_IRQ_DEBUG
	unsigned long long now = sched_clock();
#endif

	if(irq == M14_B0_IRQ_LBUS)
	{
		reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_ATOP, M14_B0_INTR_STATUS);
		intr_status = reg_val & M14_B0_INTR_STATUS_ATOP_MASK;
		type = M14_B0_REG_TYPE_ATOP;
	}
	else
	{
		reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_BTOP, M14_B0_INTR_STATUS);
		intr_status = reg_val & M14_B0_INTR_STATUS_BTOP_MASK;
		type = M14_B0_REG_TYPE_BTOP;
	}

	if(intr_status & M14_B0_TMR_DONE_INTR_STATUS)
	{
#ifdef LGBUS_IRQ_DEBUG
		if(irq == M14_B0_IRQ_LBUS) printk("ATOP Toolkit elapsed :");
		else printk("BTOP Toolkit elapsed:");
		printk("%dus\n", ((int)(now-lgbus_ctrl.start))/1000);
#endif

		lgbus_ctrl.irq_status |= (0x1 << type);
		if(lgbus_ctrl.irq_status == ((0x1 << M14_B0_REG_TYPE_ATOP) | (0x1 << M14_B0_REG_TYPE_BTOP)))
		{
			complete(&lgbus_ctrl.completion);
#ifdef LGBUS_IRQ_DEBUG
			lgbus_ctrl.start = now;		/* M14B0 timer is always automatically restarted... */
#endif

			/* Have to disable the timer interrupt to synchonize with the main function */
			reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);
			lgbus_top_reg_write((reg_val & ~M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);

			reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
			lgbus_top_reg_write((reg_val & ~M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
		}
	}
	else
	{
		int count;
		UINT32 violated_addr, violated_id, protected_base[2], protected_end[2];
		UINT32 reg_base;

		for(i = 0; i < M14_B0_MAX_BLK; i++)
		{
			mnt_blk = &lgbus_ctrl.mnt_blk[i];

			if(type != mnt_blk->reg_type) continue;

			if(!(intr_status&mnt_blk->intr_status)) continue;

			reg_base = mnt_blk->reg_base + M14_B0_WR_TOOLKIT_OFFSET;

			violated_addr = lgbus_reg_read(reg_base + M14_B0_WR_VIOLATED_ADDR);
			violated_id = lgbus_reg_read(reg_base + M14_B0_WR_VIOLATED_ID);

			protected_base[0]	= lgbus_reg_read(reg_base + M14_B0_WR_PROTECTION_BASE0) & M14_B0_WR_PROTECTION_ADDR_MASK;
			protected_end[0]	= (lgbus_reg_read(reg_base + M14_B0_WR_PROTECTION_END0) & M14_B0_WR_PROTECTION_ADDR_MASK) | 0xFFF;
			protected_base[1]	= lgbus_reg_read(reg_base + M14_B0_WR_PROTECTION_BASE1) & M14_B0_WR_PROTECTION_ADDR_MASK;
			protected_end[1]	= (lgbus_reg_read(reg_base + M14_B0_WR_PROTECTION_END1) & M14_B0_WR_PROTECTION_ADDR_MASK) | 0xFFF;

			count = 20;	/* display 20 times */
			while(--count)
			{
				lgbus_error("FATAL : [%s] accessed protected memory[0x%08X] - id[%d]", mnt_blk->name, violated_addr, violated_id);
				LGBUS_ERROR("FATAL : [%s] accessed protected memory[0x%08X] - id[%d]", mnt_blk->name, violated_addr, violated_id);
				LGBUS_ERROR("protected area1:[0x%08X]~[0x%08X]\n", protected_base[0], protected_end[0]);
				LGBUS_ERROR("protected area2:[0x%08X]~[0x%08X]\n", protected_base[1], protected_end[1]);
				LGBUS_ERROR("\n\n\n");
			}
		}
		lgbus_top_reg_write(0, type, M14_B0_INTR_ENABLE);	// disable interrupts to protect the infinite call
	}

	// Interrupt Clear
	lgbus_top_reg_write(intr_status, type, M14_B0_INTR_CLEAR);

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
	info->blk_count = M14_B0_MAX_BLK;
	info->reg_count = M14_B0_MAX_REG;

	for(i = 0; i < M14_B0_MAX_BLK; i++)
	{
		strlcpy(info->name[i], lgbus_ctrl.mnt_blk[i].name, LGBUS_MAX_BLKNAME);
	}
	for(i = 0; i < M14_B0_MAX_REG; i++)
	{
		strlcpy(info->reg_name[i], lgbus_ctrl.mnt_blk_reg[i].name, LGBUS_MAX_BLK_REGNAME);
	}
	return 0;
}

static void start_mon_timer(void)
{
	UINT32 reg_val;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
	reinit_completion(&lgbus_ctrl.completion);
#else
	INIT_COMPLETION(lgbus_ctrl.completion);
#endif
	lgbus_ctrl.irq_status = 0;

	reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);
	lgbus_top_reg_write((reg_val | M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);

	reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
	lgbus_top_reg_write((reg_val | M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
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

	// Set timer init value (M14_B0 Common Toolkit Clock == 396MHz)
	reg_val = M14_B0_DEFAULT_TIMER_VALUE / cfg->freq;
	lgbus_top_reg_write(reg_val, M14_B0_REG_TYPE_ATOP, M14_B0_TIMER_INIT_VALUE);
	lgbus_top_reg_write(reg_val, M14_B0_REG_TYPE_BTOP, M14_B0_TIMER_INIT_VALUE);

	// Get port value
	for(i = 0; i < M14_B0_MAX_BLK; i++)
	{
		int j;
		UINT8 lm_port[LGBUS_RW_MAX] = {0};
		UINT8 gm_port[LGBUS_RW_MAX] = {0};

		// Read Toolkit port value
		lm_port[LGBUS_READ] = lgbus_blk_reg_read(i, M14_B0_RTE_PATH0) & 0x03;
		gm_port[LGBUS_READ] = lgbus_blk_reg_read(i, M14_B0_RTE_PATH1) & 0x03;

		// Write Toolkit port value
		lm_port[LGBUS_WRITE] = lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_RTE_PATH0) & 0x03;
		gm_port[LGBUS_WRITE] = lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_RTE_PATH1) & 0x03;

		for(j = 0; j < LGBUS_RW_MAX; j++)
		{
			lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_LBUS][j] = lm_port[j];
			lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_GBUS][j] = gm_port[j];
		}
		//printk("[%s] LM_R: %d, LM_W: %d, GM_R: %d, GM_W: %d\n", lgbus_ctrl.mnt_blk[i].name, lm_port[LGBUS_READ], lm_port[LGBUS_WRITE], gm_port[LGBUS_READ], gm_port[LGBUS_WRITE]);
	}

	// To synchonize the timer, throw away the previous data
	start_mon_timer();
#ifdef LGBUS_IRQ_DEBUG
	lgbus_ctrl.start = sched_clock();
#endif
	if(wait_for_completion_timeout(&lgbus_ctrl.completion, msecs_to_jiffies(2000)) == 0)
	{
		printk("Timeout. Can't start the lgbus monitor !!!\n");
		return -1;
	}

	if(cfg->mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}

	return 0;
}

static int lgbus_mon_stop(void)
{
	UINT32 reg_val;
	unsigned long flags;

	lgbus_trace();

	if(lgbus_ctrl.mon_status != 1)
		return -1;

	reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);
	lgbus_top_reg_write((reg_val & ~M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);

	reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
	lgbus_top_reg_write((reg_val & ~M14_B0_TMR_ENABLE), M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	lgbus_ctrl.mon_status = 0;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);

	return 0;
}

#define invert_bux_type(i)	((i==LGBUS_TYPE_LBUS) ? LGBUS_TYPE_GBUS : LGBUS_TYPE_LBUS)
static int lgbus_mon_read(LX_LGBUS_MON_DATA_T *data)
{
	int i, j;
	int bus_type;
	UINT8 lm_port[LGBUS_RW_MAX] = {0};
	UINT8 gm_port[LGBUS_RW_MAX] = {0};
	UINT32 lm_bw[LGBUS_RW_MAX] = {0};
	UINT32 gm_bw[LGBUS_RW_MAX] = {0};

	lgbus_trace();

	if(lgbus_ctrl.mon_status != 1) return -1;

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
	data->hw.blk_count = M14_B0_MAX_BLK;
	data->cfg = lgbus_ctrl.mon_cfg;

	for(i = 0; i < M14_B0_MAX_BLK; i++)
	{
		bus_type = lgbus_ctrl.mnt_blk[i].bus_type;

		for(j = 0; j < LGBUS_RW_MAX; j++)
		{
			lm_port[j] = lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_LBUS][j];
			gm_port[j] = lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_GBUS][j];
		}

		data->raw[i].bus		= bus_type;
		data->raw[i].port		= (bus_type == M14_B0_REG_TYPE_ATOP) ? lm_port[LGBUS_TYPE_LBUS] : lm_port[LGBUS_TYPE_GBUS];
		data->raw[i].port_other	= (bus_type == M14_B0_REG_TYPE_ATOP) ? lm_port[LGBUS_TYPE_GBUS] : lm_port[LGBUS_TYPE_LBUS];

		/*write bandwidth*/
		lm_bw[LGBUS_WRITE] = lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_LM_BW_VALUE);
		gm_bw[LGBUS_WRITE] = lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_GM_BW_VALUE);

		data->raw[i].rw[LGBUS_WRITE].bw			= lm_bw[LGBUS_WRITE] + gm_bw[LGBUS_WRITE];
		data->raw[i].rw[LGBUS_WRITE].req_val 	= lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_REQUEST_NUM);
		data->raw[i].rw[LGBUS_WRITE].sum_ltn 	= lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_LATENCY);
		data->raw[i].rw[LGBUS_WRITE].max_ltn 	= lgbus_blk_reg_read(i, M14_B0_WR_TOOLKIT_OFFSET + M14_B0_PEAK_LATENCY);
		data->raw[i].rw[LGBUS_WRITE].other		= (bus_type == M14_B0_REG_TYPE_ATOP) ? gm_bw[LGBUS_WRITE] : lm_bw[LGBUS_WRITE];

		/*read bandwidth*/
		lm_bw[LGBUS_READ] = lgbus_blk_reg_read(i, M14_B0_LM_BW_VALUE);
		gm_bw[LGBUS_READ] = lgbus_blk_reg_read(i, M14_B0_GM_BW_VALUE);

		data->raw[i].rw[LGBUS_READ].bw			= lm_bw[LGBUS_READ] + gm_bw[LGBUS_READ];
		data->raw[i].rw[LGBUS_READ].req_val 	= lgbus_blk_reg_read(i, M14_B0_REQUEST_NUM);
		data->raw[i].rw[LGBUS_READ].sum_ltn 	= lgbus_blk_reg_read(i, M14_B0_LATENCY);
		data->raw[i].rw[LGBUS_READ].max_ltn 	= lgbus_blk_reg_read(i, M14_B0_PEAK_LATENCY);
		data->raw[i].rw[LGBUS_READ].other		= (bus_type == M14_B0_REG_TYPE_ATOP) ? gm_bw[LGBUS_READ] : lm_bw[LGBUS_READ];

		/*
		   printk("[%s] wr_bw: %d, wr_bw_other: %d, rd_bw: %d, rd_bw_other: %d\n",
		   lgbus_ctrl.mnt_blk[i].name,
		   data->raw[i].rw[LGBUS_WRITE].bw,
		   data->raw[i].rw[LGBUS_WRITE].other,
		   data->raw[i].rw[LGBUS_READ].bw,
		   data->raw[i].rw[LGBUS_READ].other);
		 */

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
			data->bus[LGBUS_TYPE_LBUS].rw[j].port_bw[lm_port[j]] += lm_bw[j];
			data->bus[LGBUS_TYPE_GBUS].rw[j].port_bw[gm_port[j]] += gm_bw[j];
			data->bus[bus_type].rw[j].score += data->raw[i].rw[j].sum_ltn ;
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

	if(lgbus_ctrl.mon_cfg.mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}

	return 0;
}

static int lgbus_mon_readreg(LX_LGBUS_MON_REG_T *reg_data)

{
	UINT32 reg_val, reg_addr;

	if(reg_data->blk_index >= M14_B0_MAX_BLK || reg_data->reg_type >= M14_B0_MAX_REG)
	{
		return -1;
	}

	lgbus_trace();

	reg_addr = lgbus_ctrl.mnt_blk[reg_data->blk_index].reg_base + lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_offset;

	reg_val = lgbus_reg_read(reg_addr);

	reg_data->reg_value = reg_val & lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_mask;

	return 0;
}

static int lgbus_mon_writereg(LX_LGBUS_MON_REG_T *reg_data)
{
	UINT32 reg_val, reg_addr;

	if(reg_data->blk_index >= M14_B0_MAX_BLK || reg_data->reg_type >= M14_B0_MAX_REG)
	{
		return -1;
	}

	lgbus_trace();

	reg_val = reg_data->reg_value & lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_mask;

	reg_addr = lgbus_ctrl.mnt_blk[reg_data->blk_index].reg_base + lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_offset;

	lgbus_reg_write(reg_val, reg_addr);

	return 0;
}

static int lgbus_init(void)
{
	int i, rc;
	int bus_idx;

	lgbus_trace();

	lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP] = (UINT32)ioremap_nocache(LG1311_B0_ATOP_REG_BASE, LG1311_B0_TOP_REG_RGN_SIZE);
	lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP] = (UINT32)ioremap_nocache(LG1311_B0_BTOP_REG_BASE, LG1311_B0_TOP_REG_RGN_SIZE);
	if(lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP] == 0 ||
			lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP] == 0)
	{
		LGBUS_ERROR("Can't ioremap !!!\n");
		goto exit;
	}
	lgbus_debug("lgbus_init. L[0x%8x],G[0x%8x]\n", lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP], lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP]);
	lgbus_ctrl.mnt_blk = mnt_blk_info_m14b0;
	lgbus_ctrl.mnt_blk_reg = mnt_reg_info_m14b0;

	lgbus_ctrl.max_bw = M14_B0_DDR_TOTAL_BW;

	for(i = 0; i < M14_B0_MAX_BLK; i++)
	{
		bus_idx = lgbus_ctrl.mnt_blk[i].reg_type;
		lgbus_ctrl.mnt_blk[i].reg_base = (UINT32)(lgbus_ctrl.reg_base[bus_idx]) + lgbus_ctrl.mnt_blk[i].reg_offset;
		//lgbus_debug("[%d] [%d]BUS base addr : 0x%X\n", i, bus_idx, lgbus_ctrl.mnt_blk[i].reg_base);
	}

	/*
	   ; initialize completion
	   ; this completion operate with lbus timer done interrupt handler.
	 */
	init_completion(&lgbus_ctrl.completion);

	spin_lock_init(&lgbus_ctrl.lock);
	lgbus_ctrl.mon_status = 0;

#ifdef LGBUS_TOOL_INFORMATION
	if(1)
	{
	#define TOOL_NUM	15
		UINT32 reg_val;
		int bit, x, y;
		const char *tool_name[TOOL_NUM] =
		{
			"TIMER", "ADDR_SWITCH", "MEM_PROTECT", "WBW_MONITOR", "RBW_MONITOR",
			"WLATENCY_MONITOR", "RLATENCY_MONITOR", "USER_PRI_CTRL", "CMD_DISTRIBUTE", "SMART_BANKING_CTRL",
			"SLAVE_BUSY_MONITOR", "PENDING_CTRL", "LATENCY_CTRL", "RORDER_CTRL", "WORDER_CTRL",
		};

		printk("==============================================================================\n");
		printk("LGBUS MONITOR TOOL INFORMATION\n");
		printk("==============================================================================\n");

		for(bit=0; bit<TOOL_NUM; bit++)
		{
			printk("%23s", "");
			for(y=0; y<bit; y++) printk("| ");
			printk("+-");
			for(x=(TOOL_NUM-1);x>=bit;x--) printk("--");
			printk(" %s\n", tool_name[bit]);
		}

		for(i = 0; i < LGBUS_MAX_BLK; i++)
		{
			reg_val = lgbus_blk_reg_read(i, M14_A0_TOOL_INFORMATION);
			printk("%10s : %s%d %02x", lgbus_ctrl.mnt_blk[i].name,
				((reg_val >> 16) & 0x01) ? "GPORT" : "LPORT",
				(reg_val >> 17) & 0x03,
				(reg_val >> 24) & 0xFF);
			for(bit=0; bit<TOOL_NUM; bit++)
			{
				printk(" %c", ((reg_val >>	bit) & 0x01) ? 'O' : 'X');
			}
			printk("\n");
		}
		printk("==============================================================================\n");
	}
#endif

	/*
	   ; install interrupt handler.
	 */
	if ((rc = request_irq(M14_IRQ_LBUS, lgbus_isr_b0, IRQF_SHARED, "BUS_ATOP", (void*)&lgbus_ctrl)))
		goto err_lbus;

	if ((rc = request_irq(M14_IRQ_GBUS, lgbus_isr_b0, IRQF_SHARED, "BUS_BTOP", (void*)&lgbus_ctrl)))
		goto err_gbus;

#ifdef LGBUS_SETUP_MEM_PROTECT
	if(1)
	{
		int j;
		LGBUS_MEM_PROT_RANGE_T *mprot;

		for(i=0; i<M14_B0_MAX_BLK; i++)
		{
			mprot = lgbus_ctrl.mnt_blk[i].mprot;
			if(mprot != NULL)
			{
			printk("MPROT : %s\n", lgbus_ctrl.mnt_blk[i].name);
				for(j=0; j<2; j++)
				{
					lgbus_blk_wtk_write(i, (mprot->addr0[j] & M14_B0_WR_PROTECTION_ADDR_MASK), M14_B0_WR_PROTECTION_BASE0 + j*4);
					lgbus_blk_wtk_write(i, (mprot->addr1[j] & M14_B0_WR_PROTECTION_ADDR_MASK), M14_B0_WR_PROTECTION_BASE1 + j*4);
				}
			}
		}
	}
#endif

#ifdef LGBUS_ENABLE_MEM_PROTECT
	if(1)
	{
		UINT32 reg_val;
		reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);
		reg_val = (reg_val | M14_B0_INTR_STATUS_ATOP_MASK) & ~M14_B0_TMR_DONE_INTR_STATUS;
		lgbus_top_reg_write(reg_val, M14_B0_REG_TYPE_ATOP, M14_B0_INTR_ENABLE);

		reg_val = lgbus_top_reg_read(M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
		reg_val = (reg_val | M14_B0_INTR_STATUS_BTOP_MASK) & ~M14_B0_TMR_DONE_INTR_STATUS;
		lgbus_top_reg_write(reg_val, M14_B0_REG_TYPE_BTOP, M14_B0_INTR_ENABLE);
	}
#endif

	LGBUS_INFO("LGBUS M14 Initialized\n");

	/* Success */
	return 0;

err_gbus:
	free_irq (M14_IRQ_LBUS, (void*)&lgbus_ctrl);
err_lbus:
	LGBUS_ERROR("request_irq failed. rc=%d\n", rc);
exit:
	if(lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP]) iounmap((void*)lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP]);
	if(lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP]) iounmap((void*)lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP]);

	return -1;
}

static void lgbus_cleanup(void)
{
	lgbus_trace();

	free_irq (M14_IRQ_LBUS, (void*)&lgbus_ctrl);
	free_irq (M14_IRQ_GBUS, (void*)&lgbus_ctrl);

	iounmap((void*)lgbus_ctrl.reg_base[M14_B0_REG_TYPE_ATOP]);
	iounmap((void*)lgbus_ctrl.reg_base[M14_B0_REG_TYPE_BTOP]);
}

static LGBUS_HW_FUNC_T hw_func =
{
	.init			= lgbus_init,
	.cleanup		= lgbus_cleanup,
	.open			= lgbus_open,
	.close			= lgbus_close,
	.resume 		= NULL,
	.suspend 		= NULL,

	.mon_getinfo	= lgbus_mon_getinfo,
	.mon_start		= lgbus_mon_start,
	.mon_read		= lgbus_mon_read,
	.mon_stop		= lgbus_mon_stop,
	.mon_readreg	= lgbus_mon_readreg,
	.mon_writereg	= lgbus_mon_writereg
};

LGBUS_HW_FUNC_T* LGBUS_M14_B0_Init(void)
{
	return &hw_func;
}

