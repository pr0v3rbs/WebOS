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
#include "lgbus_m14a0.h"



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

#define lgbus_blk_reg_write(idx, val, reg)	lgbus_reg_write(val, lgbus_ctrl.mnt_blk[idx].reg_base + reg)
#define lgbus_blk_reg_read(idx, reg)		lgbus_reg_read(lgbus_ctrl.mnt_blk[idx].reg_base + reg)

#define lgbus_top_reg_read(type, offset)		lgbus_reg_read(lgbus_ctrl.reg_base[type] + offset)
#define lgbus_top_reg_write(val, type, offset)	lgbus_reg_write(val, lgbus_ctrl.reg_base[type] + offset)

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
	UINT8	addr0[2];	/* {base, end} */
	UINT8	addr1[2];	/* {base, end} */
} LGBUS_MEM_PROT_RANGE_T;

typedef struct
{
	const char	*name;
	int			reg_type;
	UINT32		reg_offset;
	int			bus_type;
	UINT32		clock;
	LGBUS_MEM_PROT_RANGE_T	*mprot;

	UINT32		reg_base;
	int			port[LGBUS_TYPE_MAX];

#ifdef LGBUS_IRQ_DEBUG
	UINT32		irq_elapsed;
#endif

} LGBUS_M14_A0_BLK_INFO_T;

typedef struct
{
	const char	*name;
	UINT32		reg_offset;
	UINT32		reg_mask;
} LGBUS_M14_A0_REG_INFO_T;

typedef struct
{
	UINT32					reg_base[LGBUS_TYPE_MAX];
	LGBUS_M14_A0_BLK_INFO_T	*mnt_blk;
	LGBUS_M14_A0_REG_INFO_T	*mnt_blk_reg;

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

static LGBUS_MEM_PROT_RANGE_T default_mprot_range =
{
	{0x00, 0x5C},	/* 0x00000000-0x5CFFFFFF */
	{0x80, 0xAF}	/* 0x80000000-0xAFFFFFFF */
};
#define MPROT	&default_mprot_range

#if 0
static LGBUS_MEM_PROT_RANGE_T xxx_mprot_range =
{
	{0x00, 0x6A},	/* 0x00000000-0x6AFFFFFF */
	{0x6C, 0xAF}	/* 0x6D000000-0xAFFFFFFF */
};
#endif


#define M14_A0_BLK_INFO(name, reg, bus, clock, mprot) \
	[M14_A0_##name] = {#name, M14_A0_REG_TYPE_##reg, M14_A0_##name##_OFFSET, LGBUS_TYPE_##bus, clock, mprot, }

static LGBUS_M14_A0_BLK_INFO_T mnt_blk_info_m14a0[] =
{
	/*				name,		reg,	bus,	clock,		mprot	*/
	M14_A0_BLK_INFO(CPU_MAIN,	LBUS,	LBUS,	396*MHZ,	NULL),
	M14_A0_BLK_INFO(CPU_SUB,	GBUS,	GBUS,	396*MHZ,	NULL),
	M14_A0_BLK_INFO(GPU0,		LBUS,	LBUS,	297*MHZ,	NULL),
	M14_A0_BLK_INFO(GPU1,		LBUS,	LBUS,	297*MHZ,	NULL),
	M14_A0_BLK_INFO(GFX,		LBUS,	LBUS,	396*MHZ,	MPROT),
	M14_A0_BLK_INFO(VDEC,		LBUS,	LBUS,	320*MHZ,	MPROT),
	M14_A0_BLK_INFO(VDO,		LBUS,	LBUS,	320*MHZ,	MPROT),
	M14_A0_BLK_INFO(AUD,		LBUS,	LBUS,	320*MHZ,	MPROT),
	M14_A0_BLK_INFO(ICOD,		LBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(TE,			LBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(VENC,		LBUS,	LBUS,	320*MHZ,	MPROT),
	M14_A0_BLK_INFO(TCON,		GBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DPPA,		GBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_A,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_B,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_C,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_D,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_E_CVD,	LBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DE_E_MCU,	LBUS,	LBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DVI,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(DVO,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(SRE,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(MEPA,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(MEPB,		GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(BVE_MCU,	GBUS,	GBUS,	198*MHZ,	MPROT),
	M14_A0_BLK_INFO(MC,			GBUS,	GBUS,	198*MHZ,	MPROT),
};

#define M14_A0_REG_INFO(name) \
	[M14_A0_REG_##name] = {#name, M14_A0_REG_##name##_OFFSET, M14_A0_REG_##name##_MASK}

static LGBUS_M14_A0_REG_INFO_T mnt_reg_info_m14a0[] =
{
	M14_A0_REG_INFO(PRIORITY),
	M14_A0_REG_INFO(BANDWIDTH),
};

static LGBUS_CTRL_T lgbus_ctrl;
static LGBUS_ACCESS_CTRL_T access_ctrl;


/* Workaround for arbitration H/W bug */
static void reg_access_start_a0(void)
{
	unsigned long flags;

	spin_lock_irqsave(&access_ctrl.lock, flags);
	if(access_ctrl.refcnt == 0)
	{
		lgbus_debug("Set APB Clock to 50MHz\n");

		// set APB clk select to 50MHz(1), 25MHz(2), doesn't work at 100MHz(0) async arbitration H/W bug
		CTOP_CTRL_M14A0_RdFL(ctr01);
		CTOP_CTRL_M14A0_Wr01(ctr01, apb_clk_sel, 1);
		CTOP_CTRL_M14A0_WrFL(ctr01);
	}
	access_ctrl.refcnt++;
	spin_unlock_irqrestore(&access_ctrl.lock, flags);
}

static void reg_access_stop_a0(void)
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
			CTOP_CTRL_M14A0_RdFL(ctr01);
			CTOP_CTRL_M14A0_Wr01(ctr01, apb_clk_sel, 0);
			CTOP_CTRL_M14A0_WrFL(ctr01);
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

static irqreturn_t lgbus_isr_a0(int irq, void *dev)
{
	int i, type;
	UINT32 reg_val, intr_status;
	LGBUS_M14_A0_BLK_INFO_T *mnt_blk;
#ifdef LGBUS_IRQ_DEBUG
	unsigned long long now = sched_clock();
#endif

	if(irq == M14_A0_IRQ_LBUS) type = M14_A0_REG_TYPE_LBUS;
	else type = M14_A0_REG_TYPE_GBUS;

	lgbus_access_start();
	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		mnt_blk = &lgbus_ctrl.mnt_blk[i];
		if(mnt_blk->reg_type != type) continue;

		reg_val = lgbus_reg_read(mnt_blk->reg_base + M14_A0_INTR_CTRL);
		intr_status = reg_val & M14_A0_INTR_STATUS_MASK;

		if(!intr_status) continue;

		lgbus_debug("%s irq status : 0x%08x\n", mnt_blk->name, reg_val);

		if(intr_status & M14_A0_TMR_DONE_INTR_STATUS)
		{
			lgbus_ctrl.irq_status |= (0x1 << i);

		#ifdef LGBUS_IRQ_DEBUG
			lgbus_ctrl.mnt_blk[i].irq_elapsed = (UINT32)((int)(now-lgbus_ctrl.start)/1000);
		#endif

			if(i == M14_A0_CPU_MAIN)
			{
				//unsigned long long now = sched_clock();
				//printk("CPU Elapsed : %dus\n", ((int)(now-lgbus_ctrl.start))/1000);
				/*
				 * Send signal to process
				 */
				//complete(&lgbus_ctrl.completion);
			}
			else if(i == M14_A0_AUD)
			{
			// TODO: Interrupt delayed 10ms in 1HZ mode. So event triggerd based on audio irq
				//unsigned long long now = sched_clock();
				//printk("AUD Elapsed : %dus\n", ((int)(now-lgbus_ctrl.start))/1000);
				complete(&lgbus_ctrl.completion);
			}
		}

		if(intr_status & M14_A0_MEM_PROTECT_INTR_STATUS)  // protected memory access
		{
			int count = 20;	/* display 20 times */
			UINT32 violated_addr, violated_id, protected_base, protected_end;

			violated_addr = lgbus_reg_read(mnt_blk->reg_base + M14_A0_MEM_PROTECTION_STATUS0);
			violated_id = lgbus_reg_read(mnt_blk->reg_base + M14_A0_MEM_PROTECTION_STATUS1);

			protected_base = lgbus_reg_read(mnt_blk->reg_base + M14_A0_MEM_PROTECTION_BASE);
			protected_end = lgbus_reg_read(mnt_blk->reg_base + M14_A0_MEM_PROTECTION_END);

			while(--count)
			{
				lgbus_error("FATAL : [%s] accessed protected memory[0x%08X] - id[%d]\n", mnt_blk->name, violated_addr, violated_id);
				LGBUS_ERROR("FATAL : [%s] accessed protected memory[0x%08X] - id[%d]\n", mnt_blk->name, violated_addr, violated_id);
				LGBUS_ERROR("protected area1:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base & 0xFF, protected_end & 0xFF);
				LGBUS_ERROR("protected area2:[0x%02X000000]~[0x%02XFFFFFF]\n", protected_base >> 8, protected_end >> 8);
				LGBUS_ERROR("\n\n\n");
			}

			reg_val &= ~M14_A0_MEM_PROTECT_INTR_EN; // disable protected memory access intr
		}

		/*
		   ; clear interrupt
		 */
		reg_val |= (intr_status << 8);
		lgbus_reg_write(reg_val, mnt_blk->reg_base + M14_A0_INTR_CTRL);
	}
	lgbus_access_stop();

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
	info->blk_count = M14_A0_MAX_BLK;
	info->reg_count = M14_A0_MAX_REG;

	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		strlcpy(info->name[i], lgbus_ctrl.mnt_blk[i].name, LGBUS_MAX_BLKNAME);
		info->clock[i] = lgbus_ctrl.mnt_blk[i].clock;
	}
	for(i = 0; i < M14_A0_MAX_REG; i++)
	{
		strlcpy(info->reg_name[i], lgbus_ctrl.mnt_blk_reg[i].name, LGBUS_MAX_BLK_REGNAME);
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
	lgbus_ctrl.irq_status = 0;

	lgbus_access_start();

	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
#ifdef LGBUS_IRQ_DEBUG
		lgbus_ctrl.mnt_blk[i].irq_elapsed = 0;
#endif
		reg_val = lgbus_blk_reg_read(i, M14_A0_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val | M14_A0_TMR_ENABLE), M14_A0_INTR_CTRL);
	}

#ifdef LGBUS_IRQ_DEBUG
	lgbus_ctrl.start = sched_clock();
#endif

	lgbus_access_stop();
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
	/*
	 * enable monitoring timer done interrupt.
	 */
	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, M14_A0_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val| M14_A0_TMR_DONE_INTR_EN), M14_A0_INTR_CTRL);
	}

	/*
	 * set timer init value
	 */
	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		reg_val = lgbus_ctrl.mnt_blk[i].clock / cfg->freq;
		lgbus_blk_reg_write(i, reg_val, M14_A0_TIMER_SETTING);
	}

	/* get port value */
	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		int j;
		UINT8 start, offset, port;
		int mport[2] = {-1, -1};

		/* CPU_MAIN and CPU_SUB : address switch is not activated */
		if(i == M14_A0_CPU_MAIN || i == M14_A0_CPU_SUB)
		{
			lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_LBUS] = 0;
			lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_GBUS] = 0;
			continue;
		}

		for(j=0; j<8; j++)
		{
			reg_val = lgbus_blk_reg_read(i, M14_A0_ADDR_SWITCH0 + j*4);

			start	= (reg_val >>  0) & 0xFF;
			offset	= (reg_val >> 16) & 0xFF;
			port	= (reg_val >> 24) & 0x03;

			if((UINT8)(start + offset) < 0x80)
			{
				if(mport[LGBUS_TYPE_LBUS] == -1) mport[LGBUS_TYPE_LBUS] = port;
			}
			else
			{
				if(mport[LGBUS_TYPE_GBUS] == -1) mport[LGBUS_TYPE_GBUS] = port;
			}

			if(mport[LGBUS_TYPE_LBUS] != -1 && mport[LGBUS_TYPE_GBUS] != -1)
				break;
		}
		lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_LBUS] = mport[LGBUS_TYPE_LBUS];
		lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_GBUS] = mport[LGBUS_TYPE_GBUS];

		lgbus_debug("%s : m0p%d, m1p%d\n", lgbus_ctrl.mnt_blk[i].name,
				lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_LBUS],
				lgbus_ctrl.mnt_blk[i].port[LGBUS_TYPE_GBUS]);
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

	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, M14_A0_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val & ~M14_A0_TMR_ENABLE), M14_A0_INTR_CTRL);
	}
	/*
	 * disable monitoring timer done interrupt.
	 */
	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		reg_val = lgbus_blk_reg_read(i, M14_A0_INTR_CTRL);
		lgbus_blk_reg_write(i, (reg_val & ~M14_A0_TMR_DONE_INTR_EN), M14_A0_INTR_CTRL);
	}

	lgbus_access_stop();

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	lgbus_ctrl.mon_status = 0;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);

	return 0;
}

#define invert_bux_type(i)	((i==LGBUS_TYPE_LBUS) ? LGBUS_TYPE_GBUS : LGBUS_TYPE_LBUS)
static int lgbus_mon_read(LX_LGBUS_MON_DATA_T *data)
{
	int i, j;
	int bus_type, port, port_other;

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
	data->hw.blk_count = M14_A0_MAX_BLK;
	data->cfg = lgbus_ctrl.mon_cfg;

	lgbus_access_start();

	for(i = 0; i < M14_A0_MAX_BLK; i++)
	{
		bus_type = lgbus_ctrl.mnt_blk[i].bus_type;

		port = lgbus_ctrl.mnt_blk[i].port[bus_type];
		port_other = lgbus_ctrl.mnt_blk[i].port[invert_bux_type(bus_type)];

		data->raw[i].bus		= bus_type;
		data->raw[i].port		= port;
		data->raw[i].port_other	= port_other;

		/*write bandwidth*/
		data->raw[i].rw[LGBUS_WRITE].bw			= lgbus_blk_reg_read(i, M14_A0_WR_BW_VALUE);
		//		data->raw[i].rw[LGBUS_WRITE].bw_bud  	= lgbus_blk_reg_read(i, M14_A0_WR_BW_BUDGET);
		data->raw[i].rw[LGBUS_WRITE].req_val 	= lgbus_blk_reg_read(i, M14_A0_WR_REQ_VALUE);
		data->raw[i].rw[LGBUS_WRITE].sum_ltn 	= lgbus_blk_reg_read(i, M14_A0_WR_LATENCY_SUM);
		data->raw[i].rw[LGBUS_WRITE].max_ltn 	= lgbus_blk_reg_read(i, M14_A0_WR_LATENCY_MAX);
		data->raw[i].rw[LGBUS_WRITE].ltn_bud	= lgbus_blk_reg_read(i, M14_A0_WR_LATENCY_BUDGET);
		data->raw[i].rw[LGBUS_WRITE].other		= lgbus_blk_reg_read(i, M14_A0_WR_OTHER_BUS_BW);

		/*read bandwidth*/
		data->raw[i].rw[LGBUS_READ].bw 			= lgbus_blk_reg_read(i, M14_A0_RD_BW_VALUE);
		//		data->raw[i].rw[LGBUS_READ].bw_bud 		= lgbus_blk_reg_read(i, M14_A0_RD_BW_BUDGET);
		data->raw[i].rw[LGBUS_READ].req_val 	= lgbus_blk_reg_read(i, M14_A0_RD_REQ_VALUE);
		data->raw[i].rw[LGBUS_READ].sum_ltn 	= lgbus_blk_reg_read(i, M14_A0_RD_LATENCY_SUM);
		data->raw[i].rw[LGBUS_READ].max_ltn 	= lgbus_blk_reg_read(i, M14_A0_RD_LATENCY_MAX);
		data->raw[i].rw[LGBUS_READ].ltn_bud 	= lgbus_blk_reg_read(i, M14_A0_RD_LATENCY_BUDGET);
		data->raw[i].rw[LGBUS_READ].other		= lgbus_blk_reg_read(i, M14_A0_RD_OTHER_BUS_BW);


		/* calucate bw data for each block */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			if(data->raw[i].rw[j].req_val == 0)
				data->blk[i].rw[j].avg_ltn = 0;
			else
				data->blk[i].rw[j].avg_ltn = data->raw[i].rw[j].sum_ltn / data->raw[i].rw[j].req_val;
		}


		// work around of H/W problem
		if (i == M14_A0_CPU_MAIN || i == M14_A0_CPU_SUB)
		{
			data->raw[i].rw[LGBUS_WRITE].other = 0;
			data->raw[i].rw[LGBUS_READ].other = 0;
		}

		/* calcuate bw for each bus */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[bus_type].rw[j].port_bw[port] += (data->raw[i].rw[j].bw - data->raw[i].rw[j].other);
			data->bus[invert_bux_type(bus_type)].rw[j].port_bw[port_other] += data->raw[i].rw[j].other;

			//data->bus[bus_type].rw[j].other_bw += data->raw[i].rw[j].other;
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

	lgbus_access_stop();

	if(lgbus_ctrl.mon_cfg.mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}

	return 0;
}

static int lgbus_mon_readreg(LX_LGBUS_MON_REG_T *reg_data)

{
	UINT32 reg_val, reg_addr;

	if(reg_data->blk_index >= M14_A0_MAX_BLK || reg_data->reg_type >= M14_A0_MAX_REG)
	{
		return -1;
	}

	lgbus_trace();

	lgbus_access_start();

	reg_addr = lgbus_ctrl.mnt_blk[reg_data->blk_index].reg_base + lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_offset;

	reg_val = lgbus_reg_read(reg_addr);

	reg_data->reg_value = reg_val & lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_mask;

	lgbus_access_stop();

	return 0;
}

static int lgbus_mon_writereg(LX_LGBUS_MON_REG_T *reg_data)
{
	UINT32 reg_val, reg_addr;

	if(reg_data->blk_index >= M14_A0_MAX_BLK || reg_data->reg_type >= M14_A0_MAX_REG)
	{
		return -1;
	}

	lgbus_trace();

	lgbus_access_start();

	reg_val = reg_data->reg_value & lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_mask;

	reg_addr = lgbus_ctrl.mnt_blk[reg_data->blk_index].reg_base + lgbus_ctrl.mnt_blk_reg[reg_data->reg_type].reg_offset;

	lgbus_reg_write(reg_val, reg_addr);

	lgbus_access_stop();

	return 0;
}

static int lgbus_init(void)
{
	int i, rc;
	int bus_idx;

	lgbus_trace();

	lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS] = (UINT32)ioremap_nocache(LG1311_A0_LBUS_REG_BASE, LG1311_A0_LGBUS_REG_RGN_SIZE);
	lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS] = (UINT32)ioremap_nocache(LG1311_A0_GBUS_REG_BASE, LG1311_A0_LGBUS_REG_RGN_SIZE);
	if(lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS] == 0 ||
			lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS] == 0)
	{
		LGBUS_ERROR("Can't ioremap !!!\n");
		goto exit;
	}
	lgbus_debug("lgbus_init. L[0x%8x],G[0x%8x]\n", lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS], lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS]);
	lgbus_ctrl.mnt_blk = mnt_blk_info_m14a0;
	lgbus_ctrl.mnt_blk_reg = mnt_reg_info_m14a0;

	lgbus_ctrl.max_bw = M14_A0_DDR_TOTAL_BW;

	access_ctrl.refcnt = 0;
	access_ctrl.start	= reg_access_start_a0;
	access_ctrl.stop	= reg_access_stop_a0;

	for(i = 0; i < M14_A0_MAX_BLK; i++)
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

	spin_lock_init(&access_ctrl.lock);


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

		for(i = 0; i < M14_A0_MAX_BLK; i++)
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

	if ((rc = request_irq(M14_IRQ_LBUS, lgbus_isr_a0, IRQF_SHARED, "LGBUS_L", (void*)&lgbus_ctrl)))
		goto err_lbus;

	if ((rc = request_irq(M14_IRQ_GBUS, lgbus_isr_a0, IRQF_SHARED, "LGBUS_G", (void*)&lgbus_ctrl)))
		goto err_gbus;

#ifdef LGBUS_SETUP_MEM_PROTECT
	if(1)
	{
		LGBUS_MEM_PROT_RANGE_T *mprot;

		lgbus_access_start();
		for(i = 0; i < M14_A0_MAX_BLK; i++)
		{
			mprot = lgbus_ctrl.mnt_blk[i].mprot;
			if(mprot != NULL)
			{
				lgbus_blk_reg_write(i, mprot->addr0[0] | (mprot->addr1[0] << 8),M14_A0_MEM_PROTECTION_BASE);
				lgbus_blk_reg_write(i, mprot->addr0[1] | (mprot->addr1[1] << 8),M14_A0_MEM_PROTECTION_END);
			}
		}
		lgbus_access_stop();
	}
#endif

#ifdef LGBUS_ENABLE_MEM_PROTECT
	if(1)
	{
		UINT32 reg_val;

		for(i = 0; i < M14_A0_MAX_BLK; i++)
		{
			reg_val = lgbus_blk_reg_read(i, M14_A0_INTR_CTRL);
			lgbus_blk_reg_write(i, (reg_val| M14_A0_MEM_PROTECT_INTR_EN), M14_A0_INTR_CTRL);
		}
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
	if(lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS]) iounmap((void*)lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS]);
	if(lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS]) iounmap((void*)lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS]);

	return -1;
}

static void lgbus_cleanup(void)
{
	lgbus_trace();

	free_irq (M14_IRQ_LBUS, (void*)&lgbus_ctrl);
	free_irq (M14_IRQ_GBUS, (void*)&lgbus_ctrl);

	iounmap((void*)lgbus_ctrl.reg_base[M14_A0_REG_TYPE_LBUS]);
	iounmap((void*)lgbus_ctrl.reg_base[M14_A0_REG_TYPE_GBUS]);
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


LGBUS_HW_FUNC_T* LGBUS_M14_A0_Init(void)
{
	return &hw_func;
}

