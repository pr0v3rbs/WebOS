/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/
/*
    @file	lgbus_drv.c

    @author	yw.kim@lge.com

    @date	2012/09/27

    @brief
    		h14 lgbus bandwidth monitoring driver
*/

#ifndef _BUS_BW_DRV_C_
#define _BUS_BW_DRV_C_


/*=========================================================================================*
| Header files
*==========================================================================================*/

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
//#include <asm/gic.h>		//nalm
#include <linux/interrupt.h>	/** irqreturn_t */
#include <linux/irq.h>		/** platform interrupt */

#include <linux/delay.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/sched.h>
//#include <asm/sched_clock.h>

#include "veri_sw_h15.h"


/*=========================================================================================*
| Symbolic Constant
*==========================================================================================*/

MODULE_LICENSE("Dual BSD/GPL");
#define	MODULE_NAME		"LGBUS"

//#define	DBG_LGBUS printk
//#define 	DBG_LGBUS //

#define LGBUS_DEBUG				0
#define DBG_LGBUS(fmt, args...) \
	do { if(LGBUS_DEBUG) printk(fmt,## args); } while(0)


#define lgbus_reg_rd(addr)          __raw_readl((void*)(addr))
#define lgbus_reg_wr(val,addr)      __raw_writel(val, (void*)(addr))

//#define LGBUS_TMR_AUTO_START

/*=========================================================================================*
| Type definitions
*==========================================================================================*/
typedef struct	{
	volatile unsigned int 	bus_reg_base[QOS_MAX_IDX];
	volatile unsigned int 	cmn_reg_base[QOS_MAX_IDX];
	volatile unsigned int	mtop_reg_base[MEM_MAX_IDX];
	volatile unsigned int	cci_mtop_reg_base;
	volatile unsigned int	de_smx_base;					//SMX
	volatile unsigned int	de_reg_base;					//DE_PATH
	volatile unsigned int	vcp_reg_base;					//VCP
	volatile unsigned int	osd_reg_base;					//OSD
	volatile unsigned int	apb_reg_base;					//APB
	int 					irq[QOS_MAX_IDX];
	lgbus_ops_t				func;
	struct semaphore		sem;
} lgebus_ctrl_t;

struct  timer_list timer;

/*=========================================================================================*
| Function Prototype
*==========================================================================================*/

static int lgbus_open(struct inode *i_psInode, struct file *i_psFilePtr);
static int lgbus_release(struct inode * i_psInode, struct file * i_psFilePtr);
static long lgbus_ioctl(struct file *file, unsigned int i_uiCMD, unsigned long i_uiArg);

static int lgbus_init(void);
static void lgbus_settmr(void);
static void lgbus_stop(void);
static void lgbus_read(void);
static void lgbus_port(void);
static int lbus_isr(void);


static void DE_Performance_Init(void);

/*=========================================================================================*
| Global variables
*==========================================================================================*/

lgbus_ops_t lgbus_func =
{
	.init = lgbus_init,
	.read = lgbus_read,
	.port = lgbus_port,
	.stop = lgbus_stop,
	.isr = lbus_isr,
};

static	int	bus_mon_major;
lgebus_ctrl_t			lgebus_ctrl;
bw_reply_t				bw_reply;
port_t					p_reply;

struct file_operations bus_mon_fops =
{
	.open = lgbus_open,
	.release = lgbus_release,
	.unlocked_ioctl = lgbus_ioctl,
	.mmap	= NULL,
	.read	= NULL,
	.write	= NULL,
};


/*=========================================================================================*
| Function Definitions
*==========================================================================================*/
int busmon_module_init(void)
{
	//printk("___________________________ %s ____________________________________ \n",__func__);
	/* Registering device */
	bus_mon_major
		= register_chrdev(BUS_MON_MAJOR, BUS_MON_NODE_NAME, &bus_mon_fops);

	if (bus_mon_major < 0)
	{
		printk("<1>%s: cannot obtain major number %d\n"
			, MODULE_NAME, bus_mon_major);
		return bus_mon_major;
	}
	else
	{
		bus_mon_major = BUS_MON_MAJOR;
		printk("<1>%s: Major number %d assigned\n"
			, MODULE_NAME, bus_mon_major);
	}


	printk("INFO(busmon_module_init)-  Inserting module:%s\n", MODULE_NAME);
	return 0;
}

void busmon_module_exit(void)
{
	//printk("___________________________ %s ____________________________________ \n",__func__);
	unregister_chrdev(BUS_MON_MAJOR, BUS_MON_NODE_NAME);
}


//module_init(busmon_module_init);
//module_exit(busmon_module_exit);


static int lbus_isr(void)
{
	//unsigned int intr_status = 0;
	if(down_trylock(&(lgebus_ctrl.sem))!=0)
	{

		//if(!(intr_status & ABNORMAL_INTR_MASK))	//nalm
		{
			up(&(lgebus_ctrl.sem));
		}
	}
	return 1;
}

static irqreturn_t lgbus_isr(int irq, void *dev)
{
	//	printk("___________________________ %s ____________________________________ \n",__func__);

	lgebus_ctrl.func.isr();
	return IRQ_HANDLED;
}


/*
; file operation function
*/
static int lgbus_open(struct inode *i_psInode, struct file *i_psFilePtr)
{
	DBG_LGBUS(KERN_INFO "lgbus_open()\n");
	//printk("___________________________ %s ____________________________________ \n",__func__);

	lgebus_ctrl.irq[L_QOS_IDX] = IRQ_LBUS_TIMER;
	lgebus_ctrl.irq[G_QOS_IDX] = IRQ_GBUS_TIMER;
	lgebus_ctrl.irq[E_QOS_IDX] = IRQ_EBUS_TIMER;
	lgebus_ctrl.irq[CCI_QOS_IDX] = IRQ_CCI_TIMER;
	lgebus_ctrl.func = lgbus_func;
	return 0;
}



static int lgbus_release(struct inode * i_psInode, struct file * i_psFilePtr)
{
	//printk("___________________________ %s ____________________________________ \n",__func__);

	return 0;
}

static void lgbus_settmr(void)	//nalm interrupt enable trigger???
{
	unsigned int i;
	unsigned int tmr_val;
	unsigned int tmr_val_cci;

#ifdef FPGA
    tmr_val = (unsigned int)((LGEBUS_FREQ*MHz*4)/3);
    tmr_val_cci = (unsigned int)((CCI_FREQ*MHz*4)/3);
#else
    tmr_val = (unsigned int)(LGEBUS_FREQ*MHz);
    tmr_val_cci = (unsigned int)(CCI_FREQ*MHz);
#endif

	for(i=0;i<QOS_MAX_IDX - 1;i++)
	{
		lgbus_reg_wr(tmr_val, lgebus_ctrl.cmn_reg_base[i]+MNT_RF_TIMER_INIT_VALUE);	// set timer value for L, G, E
	}
	lgbus_reg_wr(tmr_val_cci, lgebus_ctrl.cmn_reg_base[CCI_QOS_IDX]+MNT_RF_TIMER_INIT_VALUE);	// set timer value for CCI

}

static int lgbus_init(void)		//nalm
{
	//unsigned int i;
	//unsigned int r_val=0;

	//printk("___________________________ %s ____________________________________ \n",__func__);
	lgebus_ctrl.bus_reg_base[L_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_LBUS_REG_BUS_GPU, 0x3000);
	lgebus_ctrl.bus_reg_base[G_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_GBUS_REG_BUS_GPU, 0x3000);
	lgebus_ctrl.bus_reg_base[E_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_EBUS_REG_BUS_VDEC1, 0x5000);
	lgebus_ctrl.bus_reg_base[CCI_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_CCI_REG_BUS_GPU, 0x1000);

	lgebus_ctrl.mtop_reg_base[L_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_LBUS_MTOP_BUS_M00, 0x500);
	lgebus_ctrl.mtop_reg_base[G_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_GBUS_MTOP_BUS_M00, 0x500);
	lgebus_ctrl.mtop_reg_base[E_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_EBUS_MTOP_BUS_M00, 0x500);

	lgebus_ctrl.cci_mtop_reg_base = (unsigned long)ioremap((unsigned long)LG1210_CCI_MTOP_BUS_M00, 0x500);

	lgebus_ctrl.de_smx_base = (unsigned long)ioremap((unsigned long)DE_REG_SMX_BASE, 0x1000);		//DE_PATH
	lgebus_ctrl.de_reg_base = (unsigned long)ioremap((unsigned long)DE_REG_BASE, 0x4000);		//DE_PATH
	lgebus_ctrl.vcp_reg_base = (unsigned long)ioremap((unsigned long)VCP_REG_BUS_U1Y, 0x200);		//VCP
	lgebus_ctrl.osd_reg_base = (unsigned long)ioremap((unsigned long)OSD_REG_BUS_0, 0x500);		//OSD
	lgebus_ctrl.apb_reg_base = (unsigned long)ioremap((unsigned long)APB_REG_BASE, 0x100);	//APB

	lgebus_ctrl.cmn_reg_base[L_QOS_IDX] = lgebus_ctrl.bus_reg_base[L_QOS_IDX] + 0x2000;
	lgebus_ctrl.cmn_reg_base[G_QOS_IDX] = lgebus_ctrl.bus_reg_base[G_QOS_IDX] + 0x2000;
	lgebus_ctrl.cmn_reg_base[E_QOS_IDX] = lgebus_ctrl.bus_reg_base[E_QOS_IDX] + 0x4000;
	lgebus_ctrl.cmn_reg_base[CCI_QOS_IDX] = lgebus_ctrl.bus_reg_base[CCI_QOS_IDX] + 0x3c0;

	printk("busmon_init, LQOS vaddr[0x%x]\n", lgebus_ctrl.bus_reg_base[L_QOS_IDX]);
	printk("busmon_init, GQOS vaddr[0x%x]\n", lgebus_ctrl.bus_reg_base[G_QOS_IDX]);
	printk("busmon_init, EQOS vaddr[0x%x]\n", lgebus_ctrl.bus_reg_base[E_QOS_IDX]);
	printk("busmon_init, CCI_QOS vaddr[0x%x]\n", lgebus_ctrl.bus_reg_base[CCI_QOS_IDX]);
	printk("busmon_init, L_MTOP vaddr[0x%x]\n", lgebus_ctrl.mtop_reg_base[L_QOS_IDX]);
	printk("busmon_init, G_MTOP vaddr[0x%x]\n", lgebus_ctrl.mtop_reg_base[G_QOS_IDX]);
	printk("busmon_init, E_MTOP vaddr[0x%x]\n", lgebus_ctrl.mtop_reg_base[E_QOS_IDX]);
	printk("busmon_init, CCI_MTOP vaddr[0x%x]\n", lgebus_ctrl.cci_mtop_reg_base);

	printk("busmon_init, DE SMX vaddr[0x%x]\n", lgebus_ctrl.de_smx_base);		//SMX
	printk("busmon_init, DE vaddr[0x%x]\n", lgebus_ctrl.de_reg_base);		//DE_PATH
	printk("busmon_init, VCP vaddr[0x%x]\n", lgebus_ctrl.vcp_reg_base);		//VCP
	printk("busmon_init, OSD vaddr[0x%x]\n", lgebus_ctrl.osd_reg_base);		//OSD
	printk("busmon_init, APB vaddr[0x%x]\n", lgebus_ctrl.apb_reg_base);		//APB

	lgbus_settmr();

	lgbus_reg_wr(0x00000077, lgebus_ctrl.apb_reg_base + MNT_APB_FAST_PREADY);	//APB Fast Pready register setting
	DE_Performance_Init();

	//lgbus_reg_wr(0x64B540, lgebus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_WINDOW);	//GFX BW limiter WINDOW 396MHz/60 (0xc880A2B0)
	//lgbus_reg_wr(0x1312D00, lgebus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_WR);	//GFX BW limiter WR 1.2G/60 (0xc880A240)
	//lgbus_reg_wr(0x22F7D2A, lgebus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_RD);	//GFX BW limiter RD 2.2G/60 (0xc880A244)


	sema_init(&(lgebus_ctrl.sem), 1);
	if (request_irq(122, lgbus_isr, IRQF_SHARED | IRQF_TRIGGER_RISING,"CCI", (void*)&lgebus_ctrl)) printk("request_irq CCI in %s is failed\n", __FUNCTION__);

	down(&(lgebus_ctrl.sem));

	return 0;

}

static void DE_Performance_Init(void)
{
	unsigned int reg;
	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xc20);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xc20);			//SMX0_R_MIF_Y(0xc8003c20)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xc40);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xc40);			//SMX0_R_MIF_C(0xc8003c40)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xcc0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xcc0);			//SMX0_W_MIF_Y(0xc8003cc0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xce0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xce0);			//SMX0_W_MIF_C(0xc8003ce0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xe20);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xe20);			//SMX1_R_MIF_Y(0xc8003e20)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xe40);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xe40);			//SMX1_R_MIF_C(0xc8003e40)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xec0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xec0);			//SMX1_W_MIF_Y(0xc8003ec0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0xee0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_smx_base+0xee0);			//SMX1_W_MIF_C(0xc8003ee0)


	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0xe60);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0xe60);			//ND0_R_MIF_Y(0xc8004e60)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0xe80);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0xe80);			//ND0_R_MIF_C(0xc8004e80)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0xf40);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0xf40);			//ND0_W_MIF_Y(0xc8004f40)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0xf60);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0xf60);			//ND0_W_MIF_C(0xc8004f60)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1e60);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x1e60);			//ND1_R_MIF_Y(0xc8005e60)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1e80);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x1e80);			//ND1_R_MIF_C(0xc8005e80)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1f40);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x1f40);			//ND1_W_MIF_Y(0xc8005f40)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1f60);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x1f60);			//ND1_W_MIF_C(0xc8005f60)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3cc0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3cc0);			//GSC0_PE0_W_MIF_Y(0xc8007cc0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3ce0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3ce0);			//GSC0_PE0_W_MIF_C(0xc8007ce0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3d00);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3d00);			//GSC0_SM_W_MIF_Y(0xc8007d00)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3d20);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3d20);			//GSC0_SM_W_MIF_Y(0xc8007d20)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3ec0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3ec0);			//GSC1_PE0_W_MIF_Y(0xc8007ec0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3ee0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3ee0);			//GSC1_PE0_W_MIF_C(0xc8007ee0)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3f00);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3f00);			//GSC1_SM_W_MIF_Y(0xc8007f00)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3f20);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x3f20);			//GSC1_SM_W_MIF_C(0xc8007f20)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x2f80);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x2f80);			//SUB_SS_W_MIF_Y(0xc8006f80)

	reg = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x2fa0);
	reg |= 0x40000000;
	lgbus_reg_wr(reg, lgebus_ctrl.de_reg_base+0x2fa0);			//SUB_SS_W_MIF_C(0xc8006fa0)
}


static void lgbus_stop(void)
{
	free_irq(122,(void*)&lgebus_ctrl);
	iounmap((volatile void *)lgebus_ctrl.bus_reg_base[L_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.bus_reg_base[G_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.bus_reg_base[E_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.bus_reg_base[CCI_QOS_IDX]);

	iounmap((volatile void *)lgebus_ctrl.mtop_reg_base[L_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.mtop_reg_base[G_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.mtop_reg_base[E_QOS_IDX]);
	iounmap((volatile void *)lgebus_ctrl.cci_mtop_reg_base);

	iounmap((volatile void *)lgebus_ctrl.de_smx_base);	//SMX
	iounmap((volatile void *)lgebus_ctrl.de_reg_base);	//DE_PATH
	iounmap((volatile void *)lgebus_ctrl.vcp_reg_base);	//VCP
	iounmap((volatile void *)lgebus_ctrl.osd_reg_base);	//OSD
	iounmap((volatile void *)lgebus_ctrl.apb_reg_base);	//APB
}

static void lgbus_read(void) //Hyun's
{
	int i, j,idx;
	int k,bus_offset;
	idx =0;

	for(i=0; i<MEM_MAX_IDX; i++)
	{
		for(j=0; j<MNT_BTOP_MAX_IDX; j++)
		{
			idx = j/7;
			if(idx >2) idx=2;
			bw_reply.btop.result[i].raw[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + BASE_PREV_W+(i*0x10));
			bw_reply.btop.result[i].raw[j].rd_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + BASE_PREV_R+(i*0x10));

			if(!i) bw_reply.btop.praw[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + MNT_BTOP_PENDING_MAX);		//btop temp debug
		}

		for(j=0; j<MNT_CCI_MAX_IDX; j++)
		{
			bw_reply.btop.result[i].raw_cci[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + BASE_PREV_W+(i*0x10));
			bw_reply.btop.result[i].raw_cci[j].rd_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + BASE_PREV_R+(i*0x10));

			if(!i) bw_reply.btop.praw_cci[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + MNT_BTOP_PENDING_MAX);		//ctop = btop cci temp debug
		}

		for(j=0; j<MNT_MTOP_MAX_IDX; j++)
		{
			if(i==2)	//jaehyun.jeong 2014.01.21 add below code because of bus rtl bug
			{
				bw_reply.mtop.result[i].raw[j].wr_bw = 0;
				for(k=0; k<MEM_MAX_IDX; k++)
				{
					bw_reply.mtop.result[i].raw[j].wr_bw += lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_W + (k*0x10));
				}
			}
			else
			{
				bw_reply.mtop.result[i].raw[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_W + (i*0x10));
			}
			bw_reply.mtop.result[i].raw[j].rd_bw = lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_R + (i*0x10));

			bw_reply.mtop.result[i].praw[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(j*0x40) + MNT_MTOP_PENDING_MAX);					//mtop temp debug
		}
		for(j=0; j<MNT_CCI_MTOP_MAX_IDX; j++)
		{
			bw_reply.mtop.result[i].raw_cci[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.cci_mtop_reg_base+(j*0x40) + BASE_MTOP_PREV_W + (i*0x10));
			bw_reply.mtop.result[i].raw_cci[j].rd_bw = lgbus_reg_rd(lgebus_ctrl.cci_mtop_reg_base+(j*0x40) + BASE_MTOP_PREV_R + (i*0x10));

			if(!i) bw_reply.mtop.praw_cci[j].wr_bw = lgbus_reg_rd(lgebus_ctrl.cci_mtop_reg_base+(j*0x40) + MNT_MTOP_PENDING_MAX);				//mtop cci temp debug
		}

		bus_offset = 0;
		for(j=0; j<MEM_MAX_IDX; j++) // To (L/G/E)BUS
		{
			bw_reply.mtop.result[i].to_bus[j].wr_bw = 0;
			bw_reply.mtop.result[i].to_bus[j].rd_bw = 0;
			if ( i == j ) continue;
			for(k=0; k<MEM_MAX_IDX; k++)
			{
				bw_reply.mtop.result[i].to_bus[j].wr_bw += lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(bus_offset+MNT_MTOP_MAX_IDX)*0x40 + BASE_MTOP_PREV_W + (k*0x10));
				bw_reply.mtop.result[i].to_bus[j].rd_bw += lgbus_reg_rd(lgebus_ctrl.mtop_reg_base[i]+(bus_offset+MNT_MTOP_MAX_IDX)*0x40 + BASE_MTOP_PREV_R + (k*0x10));
			}
			bus_offset++;
		}
	}

	//Latency
	for(i=0;i<MNT_BTOP_MAX_IDX;i++)
	{
		idx = i/7;
		if(idx >2) idx=2;
		bw_reply.btop.ltn_raw[i].wr_peak_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_PEAK);
		bw_reply.btop.ltn_raw[i].wr_cmd_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_CMD);
		bw_reply.btop.ltn_raw[i].wr_sum0_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_SUM0);
		bw_reply.btop.ltn_raw[i].wr_sum1_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_SUM1);

		bw_reply.btop.ltn_raw[i].rd_peak_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_PEAK);
		bw_reply.btop.ltn_raw[i].rd_cmd_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_CMD);
		bw_reply.btop.ltn_raw[i].rd_sum0_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_SUM0);
		bw_reply.btop.ltn_raw[i].rd_sum1_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_SUM1);
	}

	idx = CCI_QOS_IDX;		//idx = 3
	for(i=0; i<MNT_CCI_MAX_IDX; i++)
	{
		bw_reply.btop.ltn_raw_cci[i].wr_peak_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_PEAK);
		bw_reply.btop.ltn_raw_cci[i].wr_cmd_ltn  = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_CMD);
		bw_reply.btop.ltn_raw_cci[i].wr_sum0_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_SUM0);
		bw_reply.btop.ltn_raw_cci[i].wr_sum1_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_SUM1);

		bw_reply.btop.ltn_raw_cci[i].rd_peak_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_PEAK);
		bw_reply.btop.ltn_raw_cci[i].rd_cmd_ltn  = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_CMD);
		bw_reply.btop.ltn_raw_cci[i].rd_sum0_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_SUM0);
		bw_reply.btop.ltn_raw_cci[i].rd_sum1_ltn = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_SUM1);
	}

	//DE_PATH
	for(i=0;i<4;i++)
	{
		bw_reply.de[i].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x10C+(i*0x1000));
	}
	bw_reply.de[i].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x10C+((i-1)*0x1000 + 0x200));
    // ND Performance measure
    for(i=0;i<3;i++)
	{
		((bw_reply.de)+5)[i].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x108+(i*0x1000));
	}

	bw_reply.de[8].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3CB0);		//GSC0_PE0_W_MIF
	bw_reply.de[9].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3EB0);		//GSC1_PE0_W_MIF
	bw_reply.de[10].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x2F34);		//SUB_MIF
	//SMX
	bw_reply.de[11].bw = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0x0C10);		//SMX0_R_MIF
	bw_reply.de[12].bw = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0x0CB0);		//SMX0_W_MIF
	bw_reply.de[13].bw = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0x0E10);		//SMX1_R_MIF
	bw_reply.de[14].bw = lgbus_reg_rd(lgebus_ctrl.de_smx_base+0x0EB0);		//SMX1_W_MIF
	bw_reply.de[15].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x0F30);		//ND0_W_MIF
	bw_reply.de[16].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1F30);		//ND1_W_MIF
	bw_reply.de[17].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x0e14);		//ND0_R_MIF
	bw_reply.de[18].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x1e14);		//ND1_R_MIF
	bw_reply.de[19].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3CB4);		//GSC0_SM_W_MIF
	bw_reply.de[20].bw = lgbus_reg_rd(lgebus_ctrl.de_reg_base+0x3EB4);		//GSC1_SM_W_MIF



	//VCP
	i = 0;
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0xA0);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0xF0);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0xF4);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0x13C);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0x140);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0x18C);
	bw_reply.vcp[i++].bw = lgbus_reg_rd(lgebus_ctrl.vcp_reg_base + 0x190);

	//OSD
	i = 0;
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x80 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x100 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x100 + 0x80 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x200 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x200 + 0x80 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x300 + 0x14);
	bw_reply.osd[i++].bw = lgbus_reg_rd(lgebus_ctrl.osd_reg_base + 0x300 + 0x80 + 0x14);

}

static void lgbus_port(void) //Hyun's
{
	int i,idx;
	for(i=0; i<MNT_BTOP_MAX_IDX; i++)
	{
		idx = i/7;
		if(idx >2) idx=2;
		p_reply.raw[i].wr_enable = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_ENABLE_W );
		p_reply.raw[i].wr_num = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_NUM_W );
		p_reply.raw[i].rd_enable = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_ENABLE_R );
		p_reply.raw[i].rd_num = lgbus_reg_rd(lgebus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_NUM_R );
	}
}

static long lgbus_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	//unsigned int r_val=0;

	//printk("___________________________ %s ____________________________________ \n",__func__);

	DBG_LGBUS("cmd[%08X] cmd_idx[%d], size[0x%x]\n", cmd, _IOC_NR(cmd),_IOC_SIZE(cmd));

	switch(cmd)
	{

		case IOCTL_LGBUS_MON_STOP:
			{
				lgebus_ctrl.func.stop();
			}
			break;
		case IOCTL_LGBUS_MON_INIT:
			{
				lgebus_ctrl.func.init();
				//lgebus_ctrl.func.settmr();
			}
			break;

		case IOCTL_LGBUS_MON_PORT:
			{
				lgebus_ctrl.func.port();
				ret = copy_to_user((void __user *)arg, (void *)(&p_reply), sizeof(port_t));
			}
			break;
		case IOCTL_LGBUS_MON_READ:
			{
				down(&(lgebus_ctrl.sem));
				lgebus_ctrl.func.read();
				ret = copy_to_user((void __user *)arg, (void *)(&bw_reply), sizeof(bw_reply_t));

			}
			break;

		default:
			{
				DBG_LGBUS("ERROR(lgbus_ioctl)-Not Supported:%d\n",cmd);
			}
	}
	return ret;
}
#endif /*_BUS_BW_DRV_C_*/
