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
#define SUPPORT_VERIFICATION_SOFTWARE

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< lgbus_debug() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */

#include <linux/interrupt.h>/** irqreturn_t */
#include <linux/irq.h>		/** platform interrupt */
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/sched.h>

#include <linux/delay.h>

#include "os_util.h"
#include "sys_regs.h"

#include "lgbus_drv.h"
#include "lgbus_hw.h"
#include "lgbus_h15.h"

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
#define lgbus_debug(fmt, args...)		lgbus_debug(fmt, ##args)
#else
#define lgbus_debug(fmt, args...)		do{}while(0)
#endif

#define lgbus_error(fmt, args...)		lgbus_debug(KERN_ERR"\x1b[31m"fmt"\x1b[0m\n", ##args)


#define lgbus_reg_rd(addr)			__raw_readl((void*)(addr))
#define lgbus_reg_wr(val,addr)		__raw_writel(val, (void*)(addr))

#define lgbus_blk_reg_wr(idx, val, reg)	lgbus_reg_wr(val, lgbus_ctrl.mnt_blk[idx].reg_base + (reg))
#define lgbus_blk_reg_rd(idx, reg)		lgbus_reg_rd(lgbus_ctrl.mnt_blk[idx].reg_base + (reg))

#define lgbus_access_start()		do{}while(0)
#define lgbus_access_stop()			do{}while(0)

/* memory protector */

#define MPRO_ISR_ENABLE_OFFSET 		(0x4)
#define MPRO_ISR_STATUS_OFFSET 		(0x8)
#define MPRO_ISR_CLEAR_OFFSET 		(0xc)
#define MPRO_VIO_WR_ADDR 			(0x70)
#define MPRO_VIO_RD_ADDR 			(0x78)


#define MPRO_ISR_ENABLE 			(0xc0000)
#define MPRO_ISR_CLEAR 				(0xc0000)
#define MPRO_ISR_STATUS 			(0xc0000)
#define MPRO_ISR_STATUS_WR 			(0x40000)
#define MPRO_ISR_STATUS_RD 			(0x80000)

#define mpro_reg_read(addr)				__raw_readl((void*)(addr))
#define mpro_reg_write(val,addr)		__raw_writel(val, (void*)(addr))

#define error 		LGBUS_ERROR

#define CHECK_RET(x, __if_action) { \
	__CHECK_IF_ERROR(x != 0, error, __if_action , "[%s] Error! [E = %d]\n", __FUNCTION__, x);\
}

#define COLOR_NONE		"\x1b[0m"
#define COLOR_RED		"\x1b[1;31m"



/*----------------------------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	const char  *name;
	int			bus_idx;
	UINT32      clock;
}
LGBUS_H15_BLK_INFO_T;

typedef struct  {
    volatile unsigned int   bus_reg_base[QOS_MAX_IDX];
    volatile unsigned int   cmn_reg_base[QOS_MAX_IDX];
    volatile unsigned int   mtop_reg_base[MEM_MAX_IDX];
    volatile unsigned int   cci_mtop_reg_base;
    volatile unsigned int   de_smx_base;                    //SMX
    volatile unsigned int   de_reg_base;                    //DE_PATH
    volatile unsigned int   vcp_reg_base;                   //VCP
    volatile unsigned int   osd_reg_base;                   //OSD
    volatile unsigned int   apb_reg_base;                   //APB
    int                     irq[QOS_MAX_IDX];
    lgbus_ops_t             func;
    struct semaphore        sem;

#if 0
	struct completion       completion;
#endif
	spinlock_t				lock;
	LX_LGBUS_MON_CFG_T      mon_cfg;
	int						mon_status;

	LGBUS_H15_BLK_INFO_T    *mnt_blk;

	UINT64					max_bw;
	UINT64					ddr_clk[LGBUS_MAX_DDR_BLK];
	UINT64					ddr_max_bw[LGBUS_MAX_DDR_BLK];
}
lgbus_ctrl_t, LGBUS_H15_CTRL_T;

/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
#ifdef SUPPORT_VERIFICATION_SOFTWARE
extern int busmon_module_init(void);
#endif

/*----------------------------------------------------------------------------------------
  External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int lgbus_init(void);
static int lgbus_resume(void);
static int lgbus_suspend(void);
static void lgbus_settmr(void);
static int lgbus_mpro_init(void);
static int lgbus_mpro_cleanup(void);
static int lgbus_mpro_request_irq(void);
static int lgbus_mpro_free_irq(void);
static int lgbus_mpro_isr_enable(void);
static int lgbus_mpro_isr_disable(void);
static int lgbus_mpro_ioremap(void);
static int lgbus_mpro_iounmap(void);
static irqreturn_t lgbus_mpro_isr(int irq, void *dev);

/*----------------------------------------------------------------------------------------
  Static Variables
----------------------------------------------------------------------------------------*/
static lgbus_ctrl_t		lgbus_ctrl;
static bw_reply_t		bw_reply;
static port_t 			p_reply;

static LGBUS_H15_BLK_INFO_T mnt_blk_h15[] =
{
    { "GPU0",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "GFX0",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "TE",				LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "ICOD",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "VENC0",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "AUD",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},
    { "VIP",			LGBUS_TYPE_L,	(LGEBUS_FREQ*MHZ),	},

    { "GPU1",			LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "GFX1",			LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "VDEC0_M0",		LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "VDEC0_M1",		LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "VDEC1_M0",		LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "VDEC2_M0",		LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},
    { "OSD0",			LGBUS_TYPE_G,	(LGEBUS_FREQ*MHZ),	},

    { "VDEC1_M1",		LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "VDEC2_M1",		LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "OSD1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "VDM0",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "VDM1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "SMX0",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "SMX1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "MCU",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "CVD",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "ND0",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "ND1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "GSC0",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "GSC1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "VCP0",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "VCP1",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
    { "SUB",			LGBUS_TYPE_E,	(LGEBUS_FREQ*MHZ),	},
};

typedef struct
{
	char  	*name;
	ulong 	base_addr_phy;
	ulong 	base_addr;
	u32 	isr_cnt;
}lgbus_mpro_t;

static lgbus_mpro_t mpro_ctrl[] =
{
    { "GPU0",		0xC880A000	  	},
    { "GFX0",		0xC880A200	  	},
    { "TE",			0xC880A400	   	},
    { "ICOD",		0xC880A600	  	},
    { "VENC0",		0xC880A800	  	},
    { "AUD",		0xC880AA00	  	},
    { "VIP",		0xC880AC00	  	},
    { "GPU1",		0xC8455000	  	},
    { "GFX1",		0xC8455200	  	},
    { "VDEC0_M0",	0xC8455400	   	},
    { "VDEC0_M1",	0xC8455600	   	},
    { "VDEC1_M0",	0xC8455800	   	},
    { "VDEC2_M0",	0xC8455A00	   	},
    { "OSD0",		0xC8455C00	  	},
    { "VDEC1_M1",	0xC8213000	   	},
    { "VDEC2_M1",	0xC8213200	   	},
    { "OSD1",		0xC8213400	  	},
    { "VDM0",		0xC8213600	  	},
    { "VDM1",		0xC8213800	  	},
    { "SMX0",		0xC8213A00	  	},
    { "SMX1",		0xC8213C00	  	},
    { "MCU",		0xC8213E00	  	},
    { "CVD",		0xC8214000	  	},
    { "ND0",		0xC8214200	  	},
    { "ND1",		0xC8214400	  	},
    { "GSC0",		0xC8214600	  	},
    { "GSC1",		0xC8214800	  	},
    { "VCP0",		0xC8214A00	  	},
    { "VCP1",		0xC8214C00	  	},
    { "SUB",		0xC8214E00	  	},
};


/*========================================================================================
    Implementation Group
========================================================================================*/
static void	lgbus_module_perf_init(void)
{
    unsigned int reg;

	/* DE_PERF_INIT BEGIN */
    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xc20);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xc20);           //SMX0_R_MIF_Y(0xc8003c20)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xc40);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xc40);           //SMX0_R_MIF_C(0xc8003c40)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xcc0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xcc0);           //SMX0_W_MIF_Y(0xc8003cc0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xce0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xce0);           //SMX0_W_MIF_C(0xc8003ce0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xe20);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xe20);           //SMX1_R_MIF_Y(0xc8003e20)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xe40);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xe40);           //SMX1_R_MIF_C(0xc8003e40)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xec0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xec0);           //SMX1_W_MIF_Y(0xc8003ec0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0xee0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_smx_base+0xee0);           //SMX1_W_MIF_C(0xc8003ee0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0xe60);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0xe60);           //ND0_R_MIF_Y(0xc8004e60)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0xe80);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0xe80);           //ND0_R_MIF_C(0xc8004e80)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0xf40);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0xf40);           //ND0_W_MIF_Y(0xc8004f40)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0xf60);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0xf60);           //ND0_W_MIF_C(0xc8004f60)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1e60);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x1e60);          //ND1_R_MIF_Y(0xc8005e60)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1e80);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x1e80);          //ND1_R_MIF_C(0xc8005e80)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1f40);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x1f40);          //ND1_W_MIF_Y(0xc8005f40)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1f60);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x1f60);          //ND1_W_MIF_C(0xc8005f60)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3cc0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3cc0);          //GSC0_PE0_W_MIF_Y(0xc8007cc0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3ce0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3ce0);          //GSC0_PE0_W_MIF_C(0xc8007ce0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3d00);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3d00);          //GSC0_SM_W_MIF_Y(0xc8007d00)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3d20);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3d20);          //GSC0_SM_W_MIF_Y(0xc8007d20)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3ec0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3ec0);          //GSC1_PE0_W_MIF_Y(0xc8007ec0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3ee0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3ee0);          //GSC1_PE0_W_MIF_C(0xc8007ee0)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3f00);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3f00);          //GSC1_SM_W_MIF_Y(0xc8007f00)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3f20);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x3f20);          //GSC1_SM_W_MIF_C(0xc8007f20)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x2f80);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x2f80);          //SUB_SS_W_MIF_Y(0xc8006f80)

    reg = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x2fa0);
    reg |= 0x40000000;
    lgbus_reg_wr(reg, lgbus_ctrl.de_reg_base+0x2fa0);          //SUB_SS_W_MIF_C(0xc8006fa0)

	/* DE_PERF_INIT END */
}

/** LGBUS ISR handler
 *
 *
 */
static irqreturn_t lgbus_isr(int irq, void *dev)
{
	//unsigned int intr_status = 0;

	if(down_trylock(&(lgbus_ctrl.sem))!=0)
	{
        //if(!(intr_status & ABNORMAL_INTR_MASK))   //nalm
        {
			lgbus_debug("+++lgbus isr\n");
            up(&(lgbus_ctrl.sem));
        }
    }

#if 0
	complete(&lgbus_ctrl.completion);
#endif
    return IRQ_HANDLED;
}

static int lgbus_open(void)
{
	int i;

	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

	/* initialize(detect) DDR clock */
	for (i=0; i<LGBUS_MAX_DDR_BLK; i++)
	{
		lgbus_ctrl.ddr_clk[i]		= 792*MHZ;
		lgbus_ctrl.ddr_max_bw[i]	= lgbus_ctrl.ddr_clk[i] * 2 * 4;	/* clk * 2ch * 4byte(32bit) */
	}

	lgbus_ctrl.max_bw = lgbus_ctrl.ddr_max_bw[0];
	lgbus_ctrl.mnt_blk = mnt_blk_h15;

#if 0
	/*
	   ; initialize completion
	   ; this completion operate with lbus timer done interrupt handler.
	 */
	init_completion(&lgbus_ctrl.completion);
#endif

	spin_lock_init(&lgbus_ctrl.lock);
	lgbus_ctrl.mon_status = 0;

    lgbus_ctrl.bus_reg_base[L_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_LBUS_REG_BUS_GPU, 0x3000);
    lgbus_ctrl.bus_reg_base[G_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_GBUS_REG_BUS_GPU, 0x3000);
    lgbus_ctrl.bus_reg_base[E_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_EBUS_REG_BUS_VDEC1, 0x5000);
    lgbus_ctrl.bus_reg_base[CCI_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_CCI_REG_BUS_GPU, 0x1000);

    lgbus_ctrl.mtop_reg_base[L_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_LBUS_MTOP_BUS_M00, 0x500);
    lgbus_ctrl.mtop_reg_base[G_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_GBUS_MTOP_BUS_M00, 0x500);
    lgbus_ctrl.mtop_reg_base[E_QOS_IDX] = (unsigned long)ioremap((unsigned long)LG1210_EBUS_MTOP_BUS_M00, 0x500);

    lgbus_ctrl.cci_mtop_reg_base = (unsigned long)ioremap((unsigned long)LG1210_CCI_MTOP_BUS_M00, 0x500);

    lgbus_ctrl.de_smx_base = (unsigned long)ioremap((unsigned long)DE_REG_SMX_BASE, 0x1000);        //DE_PATH
    lgbus_ctrl.de_reg_base = (unsigned long)ioremap((unsigned long)DE_REG_BASE, 0x4000);        	//DE_PATH
    lgbus_ctrl.vcp_reg_base = (unsigned long)ioremap((unsigned long)VCP_REG_BUS_U1Y, 0x200);        //VCP
    lgbus_ctrl.osd_reg_base = (unsigned long)ioremap((unsigned long)OSD_REG_BUS_0, 0x500);      	//OSD
    lgbus_ctrl.apb_reg_base = (unsigned long)ioremap((unsigned long)APB_REG_BASE, 0x100);   		//APB

    lgbus_ctrl.cmn_reg_base[L_QOS_IDX] = lgbus_ctrl.bus_reg_base[L_QOS_IDX] + 0x2000;
    lgbus_ctrl.cmn_reg_base[G_QOS_IDX] = lgbus_ctrl.bus_reg_base[G_QOS_IDX] + 0x2000;
    lgbus_ctrl.cmn_reg_base[E_QOS_IDX] = lgbus_ctrl.bus_reg_base[E_QOS_IDX] + 0x4000;
    lgbus_ctrl.cmn_reg_base[CCI_QOS_IDX] = lgbus_ctrl.bus_reg_base[CCI_QOS_IDX] + 0x3c0;

    lgbus_debug("busmon_init, LQOS vaddr[0x%x]\n", lgbus_ctrl.bus_reg_base[L_QOS_IDX]);
    lgbus_debug("busmon_init, GQOS vaddr[0x%x]\n", lgbus_ctrl.bus_reg_base[G_QOS_IDX]);
    lgbus_debug("busmon_init, EQOS vaddr[0x%x]\n", lgbus_ctrl.bus_reg_base[E_QOS_IDX]);
    lgbus_debug("busmon_init, CCI_QOS vaddr[0x%x]\n", lgbus_ctrl.bus_reg_base[CCI_QOS_IDX]);
    lgbus_debug("busmon_init, L_MTOP vaddr[0x%x]\n", lgbus_ctrl.mtop_reg_base[L_QOS_IDX]);
    lgbus_debug("busmon_init, G_MTOP vaddr[0x%x]\n", lgbus_ctrl.mtop_reg_base[G_QOS_IDX]);
    lgbus_debug("busmon_init, E_MTOP vaddr[0x%x]\n", lgbus_ctrl.mtop_reg_base[E_QOS_IDX]);
    lgbus_debug("busmon_init, CCI_MTOP vaddr[0x%x]\n", lgbus_ctrl.cci_mtop_reg_base);

    lgbus_debug("busmon_init, DE SMX vaddr[0x%x]\n", lgbus_ctrl.de_smx_base);    //SMX
    lgbus_debug("busmon_init, DE vaddr[0x%x]\n", lgbus_ctrl.de_reg_base);        //DE_PATH
    lgbus_debug("busmon_init, VCP vaddr[0x%x]\n", lgbus_ctrl.vcp_reg_base);      //VCP
    lgbus_debug("busmon_init, OSD vaddr[0x%x]\n", lgbus_ctrl.osd_reg_base);      //OSD
    lgbus_debug("busmon_init, APB vaddr[0x%x]\n", lgbus_ctrl.apb_reg_base);      //APB

    lgbus_settmr();
    lgbus_reg_wr(0x00000077, lgbus_ctrl.apb_reg_base + MNT_APB_FAST_PREADY);    //APB Fast Pready register setting

	lgbus_module_perf_init();

    //lgbus_reg_wr(0x64B540, lgbus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_WINDOW);   //GFX BW limiter WINDOW 396MHz/60 (0xc880A2B0)
    //lgbus_reg_wr(0x1312D00, lgbus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_WR);  	//GFX BW limiter WR 1.2G/60 (0xc880A240)
    //lgbus_reg_wr(0x22F7D2A, lgbus_ctrl.bus_reg_base[L_QOS_IDX] + 0x200 + MNT_BW_LIMITER_RD);  	//GFX BW limiter RD 2.2G/60 (0xc880A244)

    sema_init(&(lgbus_ctrl.sem), 1);

	if( request_irq(122, lgbus_isr, IRQF_SHARED | IRQF_TRIGGER_RISING, "lgbus_isr", (void*)&lgbus_ctrl))
	{
		lgbus_error("can't register lgbus irq\n");
	}

    down(&(lgbus_ctrl.sem));

    lgbus_ctrl.irq[L_QOS_IDX]	= IRQ_LBUS_TIMER;
    lgbus_ctrl.irq[G_QOS_IDX]	= IRQ_GBUS_TIMER;
    lgbus_ctrl.irq[E_QOS_IDX]	= IRQ_EBUS_TIMER;
    lgbus_ctrl.irq[CCI_QOS_IDX]	= IRQ_CCI_TIMER;

	return RET_OK;
}

static int lgbus_close(void)
{
    free_irq(122,(void*)&lgbus_ctrl);
    iounmap((volatile void *)lgbus_ctrl.bus_reg_base[L_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.bus_reg_base[G_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.bus_reg_base[E_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.bus_reg_base[CCI_QOS_IDX]);

    iounmap((volatile void *)lgbus_ctrl.mtop_reg_base[L_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.mtop_reg_base[G_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.mtop_reg_base[E_QOS_IDX]);
    iounmap((volatile void *)lgbus_ctrl.cci_mtop_reg_base);

    iounmap((volatile void *)lgbus_ctrl.de_smx_base);  //SMX
    iounmap((volatile void *)lgbus_ctrl.de_reg_base);  //DE_PATH
    iounmap((volatile void *)lgbus_ctrl.vcp_reg_base); //VCP
    iounmap((volatile void *)lgbus_ctrl.osd_reg_base); //OSD
    iounmap((volatile void *)lgbus_ctrl.apb_reg_base); //APB

	return RET_OK;
}

/** get the basic LGBUS struct
 *
 */
static int lgbus_mon_getinfo(LX_LGBUS_MON_INFO_T *info)
{
	int i;

	if(info == NULL) return -1;

	memset(info, 0, sizeof(LX_LGBUS_MON_INFO_T));
	info->max_bw 	= lgbus_ctrl.max_bw;
	info->blk_count = LGBUS_MAX_BLK;

	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

	for(i = 0; i<LGBUS_MAX_BLK; i++)
	{
		strlcpy(info->name[i], lgbus_ctrl.mnt_blk[i].name, LGBUS_MAX_BLKNAME);
		info->clock[i] = lgbus_ctrl.mnt_blk[i].clock;
	}
	return 0;
}

static int lgbus_mon_start(LX_LGBUS_MON_CFG_T *cfg)
{
	unsigned long flags;

	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	if(lgbus_ctrl.mon_status != 0)
	{
		spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);
		return -1;
	}
	lgbus_ctrl.mon_status = 1;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);

	lgbus_ctrl.mon_cfg = *cfg;

	return RET_OK;
}

static int lgbus_mon_stop(void)
{
	unsigned long flags;

	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

	if(lgbus_ctrl.mon_status != 1)
		return -1;

	spin_lock_irqsave(&lgbus_ctrl.lock, flags);
	lgbus_ctrl.mon_status = 0;
	spin_unlock_irqrestore(&lgbus_ctrl.lock, flags);

	return RET_OK;
}

/** read H15 LGBUS system to bw_reply variable
 *
 *
 */
static void lgbus_mon_read_raw_data(void)
{
    int i, j,idx;
    int k,bus_offset;
    idx =0;

lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

    for(i=0; i<MEM_MAX_IDX; i++)
    {
        for(j=0; j<MNT_BTOP_MAX_IDX; j++)
        {
            idx = j/7;
            if(idx >2) idx=2;
            bw_reply.btop.result[i].raw[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + BASE_PREV_W+(i*0x10));
            bw_reply.btop.result[i].raw[j].rd_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + BASE_PREV_R+(i*0x10));

lgbus_debug("RAW BLK(%d) mem(%d) wr(%d), rd(%d)\n", j, i, bw_reply.btop.result[i].raw[j].wr_bw, bw_reply.btop.result[i].raw[j].rd_bw );

            if(!i) bw_reply.btop.praw[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((j-(7*idx))*0x200) + MNT_BTOP_PENDING_MAX);     //btop temp debug
        }

        for(j=0; j<MNT_CCI_MAX_IDX; j++)
        {
            bw_reply.btop.result[i].raw_cci[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + BASE_PREV_W+(i*0x10));
            bw_reply.btop.result[i].raw_cci[j].rd_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + BASE_PREV_R+(i*0x10));

            if(!i) bw_reply.btop.praw_cci[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[CCI_QOS_IDX]+(j*0x100) + MNT_BTOP_PENDING_MAX);       //ctop = btop cci temp debug
        }

        for(j=0; j<MNT_MTOP_MAX_IDX; j++)
        {
            if(i==2)    //jaehyun.jeong 2014.01.21 add below code because of bus rtl bug
            {
                bw_reply.mtop.result[i].raw[j].wr_bw = 0;
                for(k=0; k<MEM_MAX_IDX; k++)
                {
                    bw_reply.mtop.result[i].raw[j].wr_bw += lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_W + (k*0x10));
                }
            }
            else
            {
                bw_reply.mtop.result[i].raw[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_W + (i*0x10));
            }
            bw_reply.mtop.result[i].raw[j].rd_bw = lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(j*0x40) + BASE_MTOP_PREV_R + (i*0x10));

            bw_reply.mtop.result[i].praw[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(j*0x40) + MNT_MTOP_PENDING_MAX);                  //mtop temp debug
        }
        for(j=0; j<MNT_CCI_MTOP_MAX_IDX; j++)
        {
            bw_reply.mtop.result[i].raw_cci[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.cci_mtop_reg_base+(j*0x40) + BASE_MTOP_PREV_W + (i*0x10));
            bw_reply.mtop.result[i].raw_cci[j].rd_bw = lgbus_reg_rd(lgbus_ctrl.cci_mtop_reg_base+(j*0x40) + BASE_MTOP_PREV_R + (i*0x10));

            if(!i) bw_reply.mtop.praw_cci[j].wr_bw = lgbus_reg_rd(lgbus_ctrl.cci_mtop_reg_base+(j*0x40) + MNT_MTOP_PENDING_MAX);                //mtop cci temp debug
        }
        bus_offset = 0;
        for(j=0; j<MEM_MAX_IDX; j++) // To (L/G/E)BUS
        {
            bw_reply.mtop.result[i].to_bus[j].wr_bw = 0;
            bw_reply.mtop.result[i].to_bus[j].rd_bw = 0;
            if ( i == j ) continue;
            for(k=0; k<MEM_MAX_IDX; k++)
            {
                bw_reply.mtop.result[i].to_bus[j].wr_bw += lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(bus_offset+MNT_MTOP_MAX_IDX)*0x40 + BASE_MTOP_PREV_W + (k*0x10));
                bw_reply.mtop.result[i].to_bus[j].rd_bw += lgbus_reg_rd(lgbus_ctrl.mtop_reg_base[i]+(bus_offset+MNT_MTOP_MAX_IDX)*0x40 + BASE_MTOP_PREV_R + (k*0x10));
            }
            bus_offset++;
        }
    }

    //Latency
    for(i=0;i<MNT_BTOP_MAX_IDX;i++)
    {
        idx = i/7;
        if(idx >2) idx=2;
        bw_reply.btop.ltn_raw[i].wr_peak_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_PEAK);
        bw_reply.btop.ltn_raw[i].wr_cmd_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_CMD);
        bw_reply.btop.ltn_raw[i].wr_sum0_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_SUM0);
        bw_reply.btop.ltn_raw[i].wr_sum1_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_WR_LATENCY_SUM1);

        bw_reply.btop.ltn_raw[i].rd_peak_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_PEAK);
        bw_reply.btop.ltn_raw[i].rd_cmd_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_CMD);
        bw_reply.btop.ltn_raw[i].rd_sum0_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_SUM0);
        bw_reply.btop.ltn_raw[i].rd_sum1_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_RD_LATENCY_SUM1);
    }

    idx = CCI_QOS_IDX;      //idx = 3
    for(i=0; i<MNT_CCI_MAX_IDX; i++)
    {
        bw_reply.btop.ltn_raw_cci[i].wr_peak_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_PEAK);
        bw_reply.btop.ltn_raw_cci[i].wr_cmd_ltn  = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_CMD);
        bw_reply.btop.ltn_raw_cci[i].wr_sum0_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_SUM0);
        bw_reply.btop.ltn_raw_cci[i].wr_sum1_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_WR_LATENCY_SUM1);

        bw_reply.btop.ltn_raw_cci[i].rd_peak_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_PEAK);
        bw_reply.btop.ltn_raw_cci[i].rd_cmd_ltn  = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_CMD);
        bw_reply.btop.ltn_raw_cci[i].rd_sum0_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_SUM0);
        bw_reply.btop.ltn_raw_cci[i].rd_sum1_ltn = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+(i*0x100) + MNT_RD_LATENCY_SUM1);
    }

    //DE_PATH
    for(i=0;i<4;i++)
    {
        bw_reply.de[i].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x10C+(i*0x1000));
    }
    bw_reply.de[i].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+((i-1)*0x1000 + 0x200));
    // ND Performance measure
    for(i=0;i<3;i++)
    {
        ((bw_reply.de)+5)[i].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x108+(i*0x1000));
    }
    bw_reply.de[8].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3CB0);        //GSC0_PE0_W_MIF
    bw_reply.de[9].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3EB0);        //GSC1_PE0_W_MIF
    bw_reply.de[10].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x2F34);       //SUB_MIF

    //SMX
    bw_reply.de[11].bw = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0x0C10);       //SMX0_R_MIF
    bw_reply.de[12].bw = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0x0CB0);       //SMX0_W_MIF
    bw_reply.de[13].bw = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0x0E10);       //SMX1_R_MIF
    bw_reply.de[14].bw = lgbus_reg_rd(lgbus_ctrl.de_smx_base+0x0EB0);       //SMX1_W_MIF
    bw_reply.de[15].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x0F30);       //ND0_W_MIF
    bw_reply.de[16].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1F30);       //ND1_W_MIF
    bw_reply.de[17].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x0e14);       //ND0_R_MIF
    bw_reply.de[18].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x1e14);       //ND1_R_MIF
    bw_reply.de[19].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3CB4);       //GSC0_SM_W_MIF
    bw_reply.de[20].bw = lgbus_reg_rd(lgbus_ctrl.de_reg_base+0x3EB4);       //GSC1_SM_W_MIF

    //VCP
    i = 0;
    bw_reply.vcp[i++].bw = lgbus_reg_rd(lgbus_ctrl.vcp_reg_base);
    bw_reply.vcp[i++].bw = lgbus_reg_rd(lgbus_ctrl.vcp_reg_base + 0xA0);
    bw_reply.vcp[i++].bw = lgbus_reg_rd(lgbus_ctrl.vcp_reg_base + 0x13C);
    bw_reply.vcp[i++].bw = lgbus_reg_rd(lgbus_ctrl.vcp_reg_base + 0x140);

    //OSD
    i = 0;
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x80 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x100 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x100 + 0x80 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x200 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x200 + 0x80 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x300 + 0x14);
    bw_reply.osd[i++].bw = lgbus_reg_rd(lgbus_ctrl.osd_reg_base + 0x300 + 0x80 + 0x14);

	/* read port */
    for(i=0; i<MNT_BTOP_MAX_IDX; i++)
    {
        idx = i/7;
        if(idx >2) idx=2;
        p_reply.raw[i].wr_enable = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_ENABLE_W );
        p_reply.raw[i].wr_num = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_NUM_W );
        p_reply.raw[i].rd_enable = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_ENABLE_R );
        p_reply.raw[i].rd_num = lgbus_reg_rd(lgbus_ctrl.bus_reg_base[idx]+((i-(7*idx))*0x200) + MNT_PORT_NUM_R );
    }
}

static int lgbus_mon_read(LX_LGBUS_MON_DATA_T *data)
{
	int i, j;
	int bus_idx, port, port_other;

	unsigned int	rd_bw, wr_bw;

	if(lgbus_ctrl.mon_status != 1)
		return -1;

	if(data == NULL) return -1;

	/* wait for read */
	down(&(lgbus_ctrl.sem));

#if 0
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
			lgbus_debug("timeout !!!\n");
			return -1;
		}
	}
#endif

	memset(data, 0, sizeof(LX_LGBUS_MON_DATA_T));

	/* read RAW data */
	lgbus_mon_read_raw_data();

	data->max_bus_num	= 3; /* L/G/E */
	data->hw.max_bw		= lgbus_ctrl.max_bw;
	data->hw.blk_count	= LGBUS_MAX_BLK;
	data->cfg 			= lgbus_ctrl.mon_cfg;

lgbus_debug("max_bw %d, cnt %d \n", (int)(data->hw.max_bw>>20), data->hw.blk_count );

	for(i=0; i<LGBUS_MAX_BLK; i++)
	{
		bus_idx 	= lgbus_ctrl.mnt_blk[i].bus_idx;
		port 		= 0;
		port_other 	= 0;

		data->raw[i].bus		= bus_idx;
		data->raw[i].port		= 0;
		data->raw[i].port_other = 0;

		rd_bw = 0;
		wr_bw = 0;

		for(j=0; j<MEM_MAX_IDX; j++)
		{
			rd_bw += bw_reply.btop.result[j].raw[i].rd_bw;
			wr_bw += bw_reply.btop.result[j].raw[i].wr_bw;
		}

		lgbus_debug("blk(%d) RD %d, WR %d\n", i, rd_bw, wr_bw );

		data->raw[i].rw[LGBUS_WRITE].bw			= wr_bw;
		data->raw[i].rw[LGBUS_WRITE].other		= 0;

		data->raw[i].rw[LGBUS_WRITE].req_val 	= 0;
		data->raw[i].rw[LGBUS_WRITE].sum_ltn 	= 0;
		data->raw[i].rw[LGBUS_WRITE].max_ltn 	= 0;
		data->raw[i].rw[LGBUS_WRITE].ltn_bud	= 0;

		/*read bandwidth*/
		data->raw[i].rw[LGBUS_READ].bw			= rd_bw;
		data->raw[i].rw[LGBUS_READ].other		= 0;

		data->raw[i].rw[LGBUS_READ].req_val 	= 0;
		data->raw[i].rw[LGBUS_READ].sum_ltn 	= 0;
		data->raw[i].rw[LGBUS_READ].max_ltn 	= 0;
		data->raw[i].rw[LGBUS_READ].ltn_bud 	= 0;

		/* not supported yet */
#if 0
		/* calucate bw data for each block */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			if(data->raw[i].rw[j].req_val == 0)
				data->blk[i].rw[j].avg_ltn = 0;
			else
				data->blk[i].rw[j].avg_ltn = data->raw[i].rw[j].sum_ltn / data->raw[i].rw[j].req_val;
		}
#endif
		/* calcuate bw for each bus */
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[bus_idx].rw[j].port_bw[port] += (data->raw[i].rw[j].bw - data->raw[i].rw[j].other);

			data->bus[bus_idx].rw[j].other_bw	+= data->raw[i].rw[j].other;
			data->bus[bus_idx].rw[j].score 		+= data->raw[i].rw[j].sum_ltn ;
		}
	}

	/* calculate total bandwidth */
	for(i=0; i<LGBUS_MAX_DDR_PORT; i++)
	{
		for(j=0; j<LGBUS_RW_MAX; j++)
		{
			data->bus[LGBUS_TYPE_L].rw[j].total_bw += data->bus[LGBUS_TYPE_L].rw[j].port_bw[i];
			data->bus[LGBUS_TYPE_G].rw[j].total_bw += data->bus[LGBUS_TYPE_G].rw[j].port_bw[i];
			data->bus[LGBUS_TYPE_E].rw[j].total_bw += data->bus[LGBUS_TYPE_E].rw[j].port_bw[i];
		}
	}

	for(j=0; j<LGBUS_RW_MAX; j++)
	{
		data->bus[LGBUS_TYPE_L].total_bw += data->bus[LGBUS_TYPE_L].rw[j].total_bw;
		data->bus[LGBUS_TYPE_G].total_bw += data->bus[LGBUS_TYPE_G].rw[j].total_bw;
		data->bus[LGBUS_TYPE_E].total_bw += data->bus[LGBUS_TYPE_E].rw[j].total_bw;
	}

	lgbus_access_stop();

#if 0
	if(lgbus_ctrl.mon_cfg.mode == LGBUS_MON_MODE_AUTO)
	{
		start_mon_timer();
	}
#endif

	return 0;
}

/** initialize LGBUS system
 *
 */
static void lgbus_settmr(void)  //nalm interrupt enable trigger???
{
    unsigned int i;
    unsigned int tmrc;
    unsigned int tmrc_cci;

#ifdef FPGA
    tmrc		= (unsigned int)((LGEBUS_FREQ*MHZ*4)/3);
    tmrc_cci	= (unsigned int)((CCI_FREQ*MHZ*4)/3);
#else
    tmrc 		= (unsigned int)(LGEBUS_FREQ*MHZ);
    tmrc_cci	= (unsigned int)(CCI_FREQ*MHZ);
#endif

    for(i=0;i< QOS_MAX_IDX - 1;i++)
    {
        lgbus_reg_wr(tmrc, lgbus_ctrl.cmn_reg_base[i]+MNT_RF_TIMER_INIT_VALUE); // set timer value for L, G, E
    }
    lgbus_reg_wr(tmrc_cci, lgbus_ctrl.cmn_reg_base[CCI_QOS_IDX]+MNT_RF_TIMER_INIT_VALUE);   // set timer value for CCI
}

////////////////////////////////////////////////////////
///// memory protector /////////////////////////////////
////////////////////////////////////////////////////////
static int lgbus_mpro_init(void)
{
	int ret;

	ret = lgbus_mpro_ioremap();
	CHECK_RET(ret, goto func_exit);

	ret = lgbus_mpro_request_irq();
	CHECK_RET(ret, goto func_exit);

	ret = lgbus_mpro_isr_enable();
	CHECK_RET(ret, goto func_exit);

	return 0;

func_exit: /* control error */

	return ret;
}

static int lgbus_mpro_cleanup(void)
{
	lgbus_mpro_isr_disable();
	lgbus_mpro_free_irq();
	lgbus_mpro_iounmap();
	return 0;
}

static int lgbus_mpro_request_irq(void)
{
	int ret;

	ret = request_irq(H15_IRQ_LBUS, lgbus_mpro_isr, IRQF_SHARED, "LBUS_MPRO", (void*)&mpro_ctrl);
	CHECK_RET(ret, goto func_exit);

	ret = request_irq(H15_IRQ_GBUS, lgbus_mpro_isr, IRQF_SHARED, "GBUS_MPRO", (void*)&mpro_ctrl);
	CHECK_RET(ret, goto func_exit);

	ret = request_irq(H15_IRQ_EBUS, lgbus_mpro_isr, IRQF_SHARED, "EBUS_MPRO", (void*)&mpro_ctrl);
	CHECK_RET(ret, goto func_exit);

	return 0;

func_exit:
	lgbus_mpro_free_irq();

	return ret;
}

static int lgbus_mpro_free_irq(void)
{
	free_irq(H15_IRQ_LBUS, (void*)&mpro_ctrl);
	free_irq(H15_IRQ_GBUS, (void*)&mpro_ctrl);
	free_irq(H15_IRQ_EBUS, (void*)&mpro_ctrl);
	return 0;
}

static int lgbus_mpro_isr_enable(void)
{
	int i;
	/* isr enable to active memory protector */
	for(i = 0; i < ARRAY_SIZE(mpro_ctrl); i++)
	{
		ulong addr = mpro_ctrl[i].base_addr + MPRO_ISR_ENABLE_OFFSET;
		uint32_t val;

		if(unlikely(!mpro_ctrl[i].base_addr))
			return -1;

		val = mpro_reg_read(addr);
		val |= MPRO_ISR_ENABLE;
		mpro_reg_write(val, addr);
		LGBUS_PRINT("addr : 0x%08lx, val : 0x%x\n", addr, val);
	}
	return 0;
}

static int lgbus_mpro_isr_disable(void)
{
	int i;
	/* isr disable to active memory protector */
	for(i = 0; i < ARRAY_SIZE(mpro_ctrl); i++)
	{
		ulong addr = mpro_ctrl[i].base_addr + MPRO_ISR_ENABLE_OFFSET;
		uint32_t val;

		if(unlikely(!mpro_ctrl[i].base_addr))
			return -1;

		val = mpro_reg_read(addr);
		val &= (~((u32)MPRO_ISR_ENABLE));
		mpro_reg_write(val, addr);
		LGBUS_PRINT("addr : 0x%08lx, val : 0x%x\n", addr, val);
	}
	return 0;
}

static int lgbus_mpro_isr_disable_module(int i)
{
	ulong addr;
	uint32_t val;

	/* isr disable to active memory protector */
	if(unlikely(i >= ARRAY_SIZE(mpro_ctrl)))
	{
		LGBUS_ERROR("wrong param\n");
		return -1;
	}

	if(unlikely(!mpro_ctrl[i].base_addr))
	{
		LGBUS_ERROR("base_addr:NULL\n");
		return -1;
	}

	addr = mpro_ctrl[i].base_addr + MPRO_ISR_ENABLE_OFFSET;
	val = mpro_reg_read(addr);
	val &= (~((u32)MPRO_ISR_ENABLE));
	mpro_reg_write(val, addr);
	LGBUS_PRINT("addr : 0x%08lx, val : 0x%x\n", addr, val);

	return 0;
}

static int lgbus_mpro_ioremap(void)
{
	int i;
	/* ioremap : get virtual address */
	for(i = 0; i < ARRAY_SIZE(mpro_ctrl); i++)
	{
		/* at arm, ioremap == ioremap_nocache */
		mpro_ctrl[i].base_addr = (ulong)ioremap_nocache(mpro_ctrl[i].base_addr_phy, 0x200);

		LGBUS_PRINT("[%-10s] ioremap phy : 0x%08lx, addr : 0x%08lx\n", mpro_ctrl[i].name
										   , mpro_ctrl[i].base_addr_phy
										   , mpro_ctrl[i].base_addr);

		if(!mpro_ctrl[i].base_addr)
		{
			LGBUS_ERROR("fail ioremap\n");
			lgbus_mpro_iounmap();
			return -1;
		}
	}
	return 0;
}

static int lgbus_mpro_iounmap(void)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(mpro_ctrl); i++)
	{
		if(mpro_ctrl[i].base_addr)
		{
			iounmap((void*)mpro_ctrl[i].base_addr);
			mpro_ctrl[i].base_addr = 0;

			LGBUS_PRINT("[%-10s] iounmap phy : 0x%08lx, addr : 0x%08lx\n", mpro_ctrl[i].name
																 	   , mpro_ctrl[i].base_addr_phy
																	   , mpro_ctrl[i].base_addr);
		}
	}
	return 0;
}

static irqreturn_t lgbus_mpro_isr(int irq, void *dev)
{
	int i;

	if(irq == H15_IRQ_LBUS ||
		irq == H15_IRQ_GBUS ||
		irq == H15_IRQ_EBUS)
	{

		for(i = 0; i < ARRAY_SIZE(mpro_ctrl); i++)
		{
			ulong 	irq_status_addr = mpro_ctrl[i].base_addr + MPRO_ISR_STATUS_OFFSET;
			ulong 	irq_clear_addr = mpro_ctrl[i].base_addr + MPRO_ISR_CLEAR_OFFSET;
			ulong 	wr_vio_reg_addr, rd_vio_reg_addr;
			u64 	wr_detect, rd_detect;
			u32 	irq_status;
			u32 	irq_clear;

			if(unlikely(!mpro_ctrl[i].base_addr))
				return -1;

			irq_status = mpro_reg_read(irq_status_addr);
			if(irq_status & MPRO_ISR_STATUS)
			{
				wr_vio_reg_addr = mpro_ctrl[i].base_addr + MPRO_VIO_WR_ADDR;
				rd_vio_reg_addr = mpro_ctrl[i].base_addr + MPRO_VIO_RD_ADDR;

				/* violation address (36bit)
				 * 
				 * mpro wr/rd id(lsb 4bit) | mpro wr/rd address(32bit)
				 */

				wr_detect = mpro_reg_read(wr_vio_reg_addr) | (((u64)(mpro_reg_read(wr_vio_reg_addr+4) & 0xf)) << 32);
				rd_detect = mpro_reg_read(rd_vio_reg_addr) | (((u64)(mpro_reg_read(rd_vio_reg_addr+4) & 0xf)) << 32);

				if(irq_status & MPRO_ISR_STATUS_WR)
				{
					LGBUS_ERROR(COLOR_RED"[%-10s] Detect write violation!. ddr phy 0x%09llx"COLOR_NONE"\n"
											, mpro_ctrl[i].name, wr_detect);
				}

				if(irq_status & MPRO_ISR_STATUS_RD)
				{
					LGBUS_ERROR(COLOR_RED"[%-10s] Detect read violation!. ddr phy 0x%09llx"COLOR_NONE"\n"
											, mpro_ctrl[i].name, rd_detect);
				}

				if(++mpro_ctrl[i].isr_cnt >= 3)
					lgbus_mpro_isr_disable_module(i);

				irq_clear = mpro_reg_read(irq_clear_addr);
				irq_clear |= MPRO_ISR_CLEAR;
				mpro_reg_write(irq_clear, irq_clear_addr);
				irq_clear &= (~((u32)MPRO_ISR_CLEAR));
				mpro_reg_write(irq_clear, irq_clear_addr);
			}
		}


	}

	return IRQ_HANDLED;
}




static int lgbus_init(void)
{
	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);

#ifdef SUPPORT_VERIFICATION_SOFTWARE
	busmon_module_init();
#endif

	lgbus_mpro_init();

	LGBUS_INFO("LGBUS H15 Init OK\n");

	return RET_OK;
}

/** cleanup LGBUS system
 *
 */
static void lgbus_cleanup(void)
{
	lgbus_debug("%s:%d\n", __FUNCTION__,__LINE__);
	lgbus_mpro_cleanup();
}

static int lgbus_resume(void)
{
	int ret;

	ret = lgbus_mpro_isr_enable();
	CHECK_RET(ret, goto func_exit);

	LGBUS_INFO("LGBUS H15 Resume OK\n");

	return 0;

func_exit: /* control error */
	return ret;
}

static int lgbus_suspend(void)
{
	return 0;
}

/*========================================================================================
    Implementation Group (LGBUS H15 DESC)
========================================================================================*/
static LGBUS_HW_FUNC_T hw_func =
{
	.init			= lgbus_init,
	.cleanup		= lgbus_cleanup,
	.open			= lgbus_open,
	.close			= lgbus_close,
	.resume 		= lgbus_resume,
	.suspend 		= lgbus_suspend,

	.mon_getinfo	= lgbus_mon_getinfo,
	.mon_start		= lgbus_mon_start,
	.mon_read		= lgbus_mon_read,
	.mon_stop		= lgbus_mon_stop,
};

LGBUS_HW_FUNC_T* LGBUS_H15_Init(void)
{
	return &hw_func;
}

