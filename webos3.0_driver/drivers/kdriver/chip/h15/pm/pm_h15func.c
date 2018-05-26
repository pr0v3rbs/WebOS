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
/** @file
 *
 *  main driver implementation for pm device.
 *	pm device will teach you how to make device driver with new platform.
 *
 *  author		hankyung.yu (hankyung.yu@lge.com)
 *  version		1.0
 *  date			2014.07.18
 *  note			Additional information.
 *
 *  @addtogroup lg1210_pm
 */
#ifdef KDRV_CONFIG_PM
/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include <asm/uaccess.h>
#include <linux/poll.h>
#include "os_util.h"
#include "base_device.h"
#include "pm_drv.h"
//#include "pm_reg.h"
#include "pm_core.h"
//#include "pm_cfg.h"
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/cpu.h>
#include <linux/io.h>

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/suspend.h>


#include <linux/tick.h>
#include <trace/events/power.h>

#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>

#include <generated/uapi/linux/version.h>

#include <linux/lg1k/pms.h>

#include "pm_h15common.h"
#include <linux/cpufreq.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PM_CPU_HOTPLUG
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0))
#define PM_USE_KERNEL_FUNC
#endif

#define PM_CM3_RECOVERY
//#define PM_ALLON_RESUME

#undef PM_DEBUG_PRINT_ENABLE
#ifdef PM_DEBUG_PRINT_ENABLE
#define PM_DEBUG_PRINT(fmt, args...)		do {printk(fmt, ##args);} while (0)
#else
#define PM_DEBUG_PRINT(fmt, args...)		do{}while(0)
#endif

#define CM3FW_BASE_ADDRESS	0xC1000000
#define CM3FW_RUN_STOP		0xC103F000	//  0:STALL, 1:RUN
#define CM3FW_UART_MUX		0xc8601084	// 0x22508000
#define CM3FW_PLL_BASE		0xC8AFF000	// 0x22508000

#define ERROR_MASK				0xffff
#ifdef PM_CM3_RECOVERY
#define CM3_INFO_UPDATE_TIME	(10)		//10(tick)means 100ms
#define CTOP_BASE_FOR_CM3		0xfd300000	/* pms flush en 0xfd30015c 2bit       fd300158 flush done 0xfd300158 2bit */
#define CM3_SYSTEM_RESET		0xC8aff000	/* pms system reset		0xc8aff01c   13bit */
#define H15_CPU_NUM				4

#define FLUSH_EN 		0x4
#define FLUSH_DONE 	0x4
#define PMS_SYSTEM_RESET 0x2000
#define DEAD_CONDITON	7	// no live update 7 times
#endif


/* CM3 Firmware control */
#define CM3_IRQ_BASE		0xC1020110
#define CM3_IPC_BASE		0xC1013E00
#define CA15_CMD			CM3_IPC_BASE
#define CA15_RESP			(CM3_IPC_BASE + 0x20)
#define TZ_CMD				(CM3_IPC_BASE + 0x40)
#define TZ_RESP				(CM3_IPC_BASE + 0x60)
#define GPU_CMD				(CM3_IPC_BASE + 0x80)
#define GPU_RESP			(CM3_IPC_BASE + 0xA0)

#define PMS_INFO_BASE		(CM3_IPC_BASE + 0xC0)

#define CPU_COMMAND			0xaa000000
#define GPU_COMMAND			0x55000000

#define CPU_FS				0xaa000001
#define CPU_VS				0xaa000002
#define CPU_PWR				0xaa000004

#define PMS_RES				0xaa000100

#define GPU_FS				0x55000001
#define GPU_VS				0x55000002
#define GPU_PMS_RES			0x55000005


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
#define FREQ_SAME				0
#define FREQ_UP					1
#define FREQ_DOWN				2
#define FREQ_ONLY				3
#define VOL_ONLY				4

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
static void h15_monitor_func(void);
static void h15_test_func(void);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
---------------------------------------------------------------------------------------*/

static int h15_pms_enable = 1;
module_param(h15_pms_enable, int, S_IRUGO);
MODULE_PARM_DESC(h15_pms_enable, "h15 pms disable");

static int h15_pms_tfreq = 0;
module_param(h15_pms_tfreq, int, S_IRUGO);
MODULE_PARM_DESC(h15_pms_tfreq, "h15 cpu target freq");

static int h15_pms_tvol = 0;
module_param(h15_pms_tvol, int, S_IRUGO);
MODULE_PARM_DESC(h15_pms_tvol, "h15 cpu target vol");

int h15_pm_inited = 0;

// CM3 Firmware
static unsigned char *cm3_fw_backcup = NULL;
static unsigned int cm3_fw_copied = 0;

// Clock Gating
LX_PM_CLOCKGATING_T pms_clockgating;
LX_PM_TEST_FACTOR_T pms_test_factor;

typedef struct PM_CGINFO_t
{
	unsigned int module;
	unsigned int sub;
	unsigned int sub_max;
	unsigned int base;
	unsigned int clkbit;
	unsigned int mem_offset;
	unsigned int mem_bitstart;
	unsigned int mem_bitsize;
}
PM_CGINFO_T;

// CPU Power Management
static void __iomem *pms_status_base = NULL;
static void __iomem *pms_ipc_base = NULL;
static void __iomem *pms_irq_base = NULL;

static void __iomem *pms_cm3_run = NULL;
static void __iomem *pms_cm3_pll = NULL;

#ifdef PM_CM3_RECOVERY
static void __iomem *pms_cm3_base = NULL;
static void __iomem *flushen_base = NULL;
static void __iomem *pms_systemreset_base= NULL;

static unsigned int g_h15_cm3recovery = 0;
static unsigned int g_h15_cm3recovery_test = 0;
static unsigned int dead_count = 0;
static unsigned int recovery_check = 0;
static unsigned int make_recovery = 0;
static unsigned int live_backup = 0;
#endif

static int g_h15_pm_task_disable = 0;

#ifdef PM_CPU_HOTPLUG
static struct task_struct *cpu_hotplug_pm_task;
static struct completion   cpu_hotplug_completion;
#endif

#ifdef PM_USE_KERNEL_FUNC
static unsigned int g_h15_kernel_func = 0;
#endif

static unsigned int g_h15_sampling_rate = ((HZ/ 100) * 3); // 30ms
static unsigned int g_core_n = 0x0;
static unsigned int g_core_onoff = 0x0;

static unsigned int g_h15_do_hotplug = 0;
static unsigned int g_h15_do_dvfs = 0;
static unsigned int g_h15_freq = 0;
static unsigned int g_h15_vol = 0;
static unsigned int g_h15_err = 0;

struct timer_list pm_timer;
static unsigned int timer_work = 0;

// CPU Governor
static pms_h15gov_t* pm_h15gov = NULL;
static unsigned int pm_status_backup;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
//static unsigned int PM_Poll(struct file *filp, poll_table *wait);
static unsigned int h15_GetCPUOn(void);
#ifdef PM_CM3_RECOVERY
static int h15_SendIPC(unsigned int cmd, unsigned int arg, unsigned int flag);
static int h15_SetCPUFreq(unsigned  int freq);
static int h15_SetCPUVol(unsigned  int vol);
extern int lg1k_pms_set_hotplug(int cpu, int on);
extern void cpu_maps_update_begin(void);
extern void cpu_maps_update_done(void);
#endif
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
static unsigned int h15_get_sampling_rate(void)
{
	return g_h15_sampling_rate;
}

static int h15_set_sampling_rate(unsigned int ms)
{
	g_h15_sampling_rate = ((HZ/ 100) * (ms / 10));

	return 0;
}

static void h15_TimerHandler( unsigned long arg )
{
//	#define TIMER_50MS ((HZ / 100) * 5)
//	mod_timer(&pm_timer, jiffies + TIMER_50MS); // // 50ms

	mod_timer(&pm_timer, jiffies + h15_get_sampling_rate());

	h15_monitor_func();
	h15_test_func();
	pm_h15gov->check_cpu();

	return;
}

static void h15_TimerInit(void)
{
#define DEFAULT_TIMER ((HZ / 100) * 5) // 50ms

	init_timer( &pm_timer );

	if ( pm_timer.function == NULL )
	{
		pm_timer.function = h15_TimerHandler;
	}

	pm_timer.expires = jiffies + DEFAULT_TIMER;	// 50ms
}

static void h15_TimerStart(void)
{
	if (timer_work == 0)
	{
		timer_work = 1;
		add_timer( &pm_timer );
	}
}

static void h15_TimerStop(void)
{
	if (timer_work)
	{
		timer_work = 0;
		del_timer( &pm_timer );
	}
}

static unsigned int h15_UartToCM3(void)
{
	static void __iomem *pms_uart = NULL;
	volatile unsigned int reg;

	pms_uart = ioremap(0xc8601084, 4);
	reg = readl(pms_uart);
	reg &= 0x0FFE7FFF;
	reg |= 0x50000000;
	writel(reg, pms_uart);
	iounmap(pms_uart);

	return 0;
}

#ifdef PM_CM3_RECOVERY
static void h15_RecoveryCM3(void)
{
	unsigned int i = 0,ret = 0;
	unsigned int *data	= NULL;
	unsigned int reg = 0;

	PM_DEBUG("CM3 RECOVERRY START\n");

	if (cm3_fw_backcup == NULL)
	{
		PM_ERROR("pms f/w don't download\n");
		return;
	}

	h15_TimerStop();

	reinit_completion(&cpu_hotplug_completion);

	cpu_maps_update_begin();
	cpu_maps_update_done();


	//writel(0, pms_cm3_run);								  			 //0. cm3 reset de-assert (0xC103FF00 1을 0으로) -> recommand to remove by GIL C
	reg = readl(flushen_base + 0x15c);
	writel(reg | FLUSH_EN , flushen_base+0x15c); 					//1.	Flush Control (set PMS 0x04, complete확인)
	do{
	 	mdelay(1);
	}while( (readl(flushen_base + 0x158) & FLUSH_DONE)==0);

	reg = readl(pms_systemreset_base + 0x1c);
	writel(reg | PMS_SYSTEM_RESET, pms_systemreset_base+0x1c); 		//2.	Swrst_pms_system set(13번 Bit 0x2000)
	reg = readl(flushen_base + 0x15c);
	writel( reg & (~FLUSH_EN) , flushen_base+0x15c);				//3.	Flush control (set 0x0)
	reg = readl(pms_systemreset_base + 0x1c);
	writel( reg &(~PMS_SYSTEM_RESET), pms_systemreset_base+0x1c);	//4.	Swrst pms system 0

	mdelay(1);

	data = (unsigned int*)cm3_fw_backcup;

	memset((unsigned char *)pms_cm3_base,0x0,PMS_PARTITION_FW_SIZE);
	for(i=0; i<(PMS_PARTITION_FW_SIZE/4); i++)
		writel(data[i], pms_cm3_base + (i*4));						//5. CM3 download
	writel_relaxed(0x00, pms_irq_base);
	PM_DEBUG("Before CM3 RERUN pms_irq_base [%x]\n",readl_relaxed(pms_irq_base));
	PM_DEBUG("Before CM3 RERUN PLL [%x][%x][%x][%x][%x]\n",readl(pms_cm3_pll+0x80),readl(pms_cm3_pll+0x84),readl(pms_cm3_pll+0x88),readl(pms_cm3_pll+0x8c) ,readl(pms_cm3_pll+0x3c));
	//h15_UartToCM3();
	writel(1, pms_cm3_run);											//6.	cm3 reset de-assert (0xC103FF00 1을 0으로)

	msleep(500);													//7. give 500ms delay after CM3 Booting

	//h15_UartToA57();
	PM_DEBUG("After CM3 RERUN PLL [%x][%x][%x][%x][%x]\n",readl(pms_cm3_pll+0x80),readl(pms_cm3_pll+0x84),readl(pms_cm3_pll+0x88),readl(pms_cm3_pll+0x8c) ,readl(pms_cm3_pll+0x3c));

	// consider cpu_down fail case
	if( ((g_h15_err & 0xf0000000)  == 0xD0000000) && ((g_h15_err & 0x000f0000) == 0) )
	{
		for(i = 1;i < H15_CPU_NUM ; i++)
		{
			PM_DEBUG("EXXA[%x]CPU[%x] \n",g_h15_err,h15_GetCPUOn());
			if( cpu_online(i) == 0 && ( h15_GetCPUOn() & (1<<i)) )
			{
				ret= lg1k_pms_set_hotplug(i,0);
				PM_DEBUG("CORE[%d]ON[%d][%x]RET[%d] \n",i,cpu_online(i),h15_GetCPUOn(),ret);
			}
		}
	}

	h15_SetCPUFreq(CPU_FREQ_UD);
	h15_SetCPUVol(CPU_VOL_UD);

	g_h15_err = 0;
	dead_count = 0;
	PM_DEBUG("CM3 RECOVERRY END[%x] VER[%x]",h15_GetCPUOn(),readl(pms_ipc_base + 0xc0) );
	h15_TimerStart();

}
#endif

static int h15_GetCGInfo(PM_CGINFO_T *cginfo)
{
	#define PM_VENC_BASE	0xC898E000
	#define PM_VDEC0_BASE	0xC8714004
	#define PM_VDEC1_BASE	0xC8744000
	#define PM_VDEC2_BASE	0xC8750000
	#define PM_VIP_BASE		0xC898E400

	unsigned int ret = -1;

	if (cginfo->module == PM_VENC) {
		if (cginfo->sub == PM_VENC_0) {
			cginfo->base = PM_VENC_BASE;
			cginfo->clkbit = 9;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 0;
			cginfo->mem_bitsize = 3;
			ret = 0;
		}
 		else if (cginfo->sub == PM_VENC_1) {
			cginfo->base = PM_VENC_BASE;
			cginfo->clkbit = 10;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 3;
			cginfo->mem_bitsize = 3;
			ret = 0;
		}
	}
	else if (cginfo->module == PM_VDEC)
	{
		if (cginfo->sub == PM_VDEC0_G2) {
			cginfo->base = PM_VDEC0_BASE;
			cginfo->clkbit = 7;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 0;
			cginfo->mem_bitsize = 4;
			ret = 0;
		}
		else if (cginfo->sub == PM_VDEC1_HEVC) {
			cginfo->base = PM_VDEC1_BASE;
			cginfo->clkbit = 13;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 0;
			cginfo->mem_bitsize = 5;
			ret = 0;
		}
		else if (cginfo->sub == PM_VDEC1_G1) {
			cginfo->base = PM_VDEC1_BASE;
			cginfo->clkbit = 14;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 8;
			cginfo->mem_bitsize = 1;
			ret = 0;
		}
		else if (cginfo->sub == PM_VDEC2_HEVC) {
			cginfo->base = PM_VDEC2_BASE;
			cginfo->clkbit = 13;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 0;
			cginfo->mem_bitsize = 5;
			ret = 0;
		}
		else if (cginfo->sub == PM_VDEC2_G1) {
			cginfo->base = PM_VDEC2_BASE;
			cginfo->clkbit = 14;
			cginfo->mem_offset = 24*4;
			cginfo->mem_bitstart = 8;
			cginfo->mem_bitsize = 1;
			ret = 0;
		}
	}
	else if (cginfo->module == PM_VIP) {
		cginfo->base = PM_VIP_BASE;
		cginfo->clkbit = 5;
		cginfo->mem_offset = 24*4;
		cginfo->mem_bitstart = 0;
		cginfo->mem_bitsize = 3;
		ret = 0;
	}

	return ret;
}

int PM_SetClockGating(u32 Module, u32 SubModule, u32 state)
{
	void __iomem *pms_clock_gating_base = NULL;
	PM_CGINFO_T cginfo;
	volatile unsigned int reg;
	unsigned int i;
	int ret;

	if (( lx_chip_rev() & 0xFFFFFF00) != (LX_CHIP_REV(H15,A0) & 0xFFFFFF00))
		return 0;

	if ((Module == PM_NULL) || (Module > PM_MAX))
		return -1;

	PM_DEBUG_PRINT("Set Module %u\n",Module);
	PM_DEBUG_PRINT("Set Sub 0x%X\n",SubModule);

	cginfo.module = Module;
	cginfo.sub = SubModule;
	ret = h15_GetCGInfo(&cginfo);
	if (ret < 0)
		return -2;

	pms_clock_gating_base = ioremap(cginfo.base, 4096);

	PM_DEBUG_PRINT("Set reg 0x%X, cginfo.clkbit %u\n", cginfo.base, cginfo.clkbit);

	if (state == PM_CLK_ON) // POWER ON
	{
		for (i=0; i<cginfo.mem_bitsize; i++)
		{
			reg = readl(pms_clock_gating_base + cginfo.mem_offset);

			reg = reg & (~(1 << (cginfo.mem_bitstart + cginfo.mem_bitsize - 1 - i)));

			writel(reg, pms_clock_gating_base + cginfo.mem_offset );
			PM_DEBUG_PRINT("Set mem reg 0x%X - 0x%X\n",cginfo.mem_offset, readl(pms_clock_gating_base + cginfo.mem_offset));
		}

		if (Module == PM_VIP)
		{
			reg = readl(pms_clock_gating_base + cginfo.mem_offset);
			reg = reg & ~(1 << 15);
			writel(reg, pms_clock_gating_base + cginfo.mem_offset);
		}

		reg = readl(pms_clock_gating_base);
		reg = reg & ~(1 << cginfo.clkbit);
		writel(reg, pms_clock_gating_base);
		PM_DEBUG_PRINT("Set mem clk 0x%X - 0x%X\n",cginfo.base, readl(pms_clock_gating_base));
	}
	else // POWER DOWN
	{
		reg = readl(pms_clock_gating_base);
		reg = reg | (1 << cginfo.clkbit);
		writel(reg, pms_clock_gating_base);
		PM_DEBUG_PRINT("Set mem clk 0x%X - 0x%X\n",cginfo.base, readl(pms_clock_gating_base));

		if (Module == PM_VIP)
		{
			reg = readl(pms_clock_gating_base + cginfo.mem_offset);
			reg = reg | (1 << 15);
			writel(reg, pms_clock_gating_base + cginfo.mem_offset);
		}

		for (i=0; i<cginfo.mem_bitsize; i++)
		{
			reg = readl(pms_clock_gating_base + cginfo.mem_offset);
			reg = reg | (1 << (cginfo.mem_bitstart + i));
			writel(reg, pms_clock_gating_base + cginfo.mem_offset);
			PM_DEBUG_PRINT("Set mem reg 0x%X - 0x%X\n",cginfo.mem_offset, readl(pms_clock_gating_base + cginfo.mem_offset));
		}
	}

	iounmap(pms_clock_gating_base);


	return 0;
}
EXPORT_SYMBOL(PM_SetClockGating);

int PM_GetClockGating(u32 Module, u32 SubModule, u32 *state)
			{
	void __iomem *pms_clock_gating_base = NULL;
	PM_CGINFO_T cginfo;
	volatile unsigned int reg;
	int ret;
	unsigned int st = 0;

	if (( lx_chip_rev() & 0xFFFFFF00) != (LX_CHIP_REV(H15,A0) & 0xFFFFFF00))
		return 0;

	if ((Module == PM_NULL) || (Module > PM_MAX))
		return -1;

	PM_DEBUG_PRINT("Get Module %u\n",Module);
	PM_DEBUG_PRINT("Get SubModule %u\n",SubModule);

	cginfo.module = Module;
	cginfo.sub = SubModule;

	ret = h15_GetCGInfo(&cginfo);
	if (ret < 0)
		return -1;

	pms_clock_gating_base = ioremap(cginfo.base, 4096);

	reg = readl(pms_clock_gating_base);

	PM_DEBUG_PRINT("Get reg 0x%X - 0x%X, cginfo.clkbit %u\n",cginfo.base, reg, cginfo.clkbit);

	if (reg & (1 << cginfo.clkbit))
		st = 1;

	iounmap(pms_clock_gating_base);

	PM_DEBUG_PRINT("Get st 0x%X\n",st);

	*state = st;

	return 0;

}
EXPORT_SYMBOL(PM_GetClockGating);

static void h15_PrintIPC(const char *string, unsigned int cmd, unsigned int arg)
{
#if 0
	printk("%s\n",string);
	printk("cmd 0x%X\n",cmd);
	printk("arg %u\n",arg);
	if ((cmd == CPU_FS) || (cmd == CPU_VS))
	{
		printk("0xC1013E00 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x0),readl(pms_ipc_base+0x4),readl(pms_ipc_base+0x8),readl(pms_ipc_base+0xC));
		printk("0xC1013E10 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x10),readl(pms_ipc_base+0x14),readl(pms_ipc_base+0x18),readl(pms_ipc_base+0x1C));
		printk("0xC1013E20 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x20),readl(pms_ipc_base+0x24),readl(pms_ipc_base+0x28),readl(pms_ipc_base+0x2C));
		printk("0xC1013E30 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x30),readl(pms_ipc_base+0x34),readl(pms_ipc_base+0x38),readl(pms_ipc_base+0x3C));
	}
	else
	{
		printk("0xC1013E80 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x80),readl(pms_ipc_base+0x84),readl(pms_ipc_base+0x88),readl(pms_ipc_base+0x8C));
		printk("0xC1013E90 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0x90),readl(pms_ipc_base+0x94),readl(pms_ipc_base+0x98),readl(pms_ipc_base+0x9C));
		printk("0xC1013EA0 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0xA0),readl(pms_ipc_base+0xA4),readl(pms_ipc_base+0xA8),readl(pms_ipc_base+0xAC));
		printk("0xC1013EB0 : 0x%08X 0x%08X 0x%08X 0x%08X\n",readl(pms_ipc_base+0xB0),readl(pms_ipc_base+0xB4),readl(pms_ipc_base+0xB8),readl(pms_ipc_base+0xBC));
	}
#endif

	return ;
}

static void h15_ClearIPCKey(void)
{
	writel(0x0, pms_ipc_base + 0x20 + 0x1C);
}

static int h15_SendIPC(unsigned int cmd, unsigned int arg, unsigned int flag)
{
	unsigned int ack;
	unsigned int offset;
	unsigned int response_hdr;
	volatile unsigned int reg;
	volatile unsigned int send_key;
	volatile unsigned int read_key;
	unsigned long long start;
	unsigned long timeout = 100 * NSEC_PER_MSEC;

	if ((cmd == CPU_FS) || (cmd == CPU_VS))
	{
		ack = 0x1;
		offset = 0x0;
		response_hdr = 0xAA000100;
		arg *= 1000;
	}
	else if ((cmd == GPU_FS) || (cmd == GPU_VS))
	{
		ack = 0x2;
		offset = 0x80;
		response_hdr = 0x55000000;
		arg *= 1000;
	}
	else if (cmd == CPU_PWR)
	{
		ack = 0x1;
		offset = 0x0;
		response_hdr = 0xAA000100;
	}
	else
		return -1;

	writel(cmd, pms_ipc_base + offset);

	writel(0, pms_ipc_base + offset + 0x4);		// freq;
	writel(0, pms_ipc_base + offset + 0x8);		// vol;
	writel(0, pms_ipc_base + offset + 0xC);		// cpu online

	if ((cmd == CPU_FS) || (cmd == GPU_FS))
		writel(arg, pms_ipc_base + offset + 0x4);	// freq;
	else if ((cmd == CPU_VS) || (cmd == GPU_VS))
		writel(arg, pms_ipc_base + offset + 0x8);	// vol;
	else if (cmd == CPU_PWR)
		writel(arg, pms_ipc_base + offset + 0xC);	// power;
	writel(0x0, pms_ipc_base + offset + 0x10);	// reserved
	writel(flag, pms_ipc_base + offset + 0x14);	// flag
	writel(0x0, pms_ipc_base + offset + 0x18);	// reserved

	send_key = readl(pms_ipc_base + offset + 0x1C) + 1;
	writel(send_key, pms_ipc_base + offset + 0x1C);	// key

	h15_PrintIPC("REQ", cmd, arg);

	writel(0, pms_irq_base);
	writel(ack, pms_irq_base);

	start = sched_clock();
	while(1)
	{
		reg = readl(pms_ipc_base + offset + 0x20);
		read_key = readl(pms_ipc_base + offset + 0x20 + 0x1C);

		if ((reg & response_hdr) && (read_key == send_key))
			break;

		if ((sched_clock() - start) > timeout)
		{
			PM_ERROR("CM3 IPC TIMEOUT ERROR\n");
			h15_PrintIPC("REQUEST", cmd, arg);
			return 1;
		}
	}

	h15_PrintIPC("REQUEST", cmd, arg);

	return 0;
}

static unsigned int h15_GetCPUOn(void)
{
	return readl(pms_status_base + 0xC);
}

#ifdef PM_CM3_RECOVERY
static unsigned int h15_GetCM3Live(void)
{
	return readl(pms_status_base + 0x3C);
}
#endif

static unsigned int h15_GetCPUFreq(void)
{
	return (readl(pms_ipc_base + 0x20 + 0x4) / 1000);
}

static int h15_SetCPUFreq(unsigned  int freq)
{
	unsigned long flags;
	int ret = 0;

	local_irq_save(flags);
	preempt_disable();

	h15_ClearIPCKey();

#ifdef PM_USE_KERNEL_FUNC
	if (g_h15_kernel_func == 0)
		ret = h15_SendIPC(CPU_FS, freq, 0x0);
	else
		ret = lg1k_pms_set_cpufreq(0, freq * 1000);
#else
	ret = h15_SendIPC(CPU_FS, freq, 0x0);
#endif

	h15_ClearIPCKey();

	preempt_enable();
	local_irq_restore(flags);

	return ret;
}

static unsigned int h15_GetCPUVol(void)
{
	return (readl(pms_ipc_base + 0x20 + 0x8) / 1000);
}

static int h15_SetCPUVol(unsigned int vol)
{
	int ret = 0;

	h15_ClearIPCKey();

#ifdef PM_USE_KERNEL_FUNC
	if (g_h15_kernel_func == 0)
		ret = h15_SendIPC(CPU_VS, vol, 0x0);
	else
		ret = lg1k_pms_set_cpuvolt(0, vol * 1000);
#else
	ret = h15_SendIPC(CPU_VS, vol, 0x0);
#endif

	h15_ClearIPCKey();

	return ret;
}

static unsigned int h15_freq_vol(unsigned int freq)
{
	unsigned int vol;

	switch(freq)
	{
		case CPU_FREQ_OD:
			vol = CPU_VOL_OD;
			break;

		case CPU_FREQ_ND:
			vol = CPU_VOL_ND;
			break;

		case CPU_FREQ_UD:
			vol = CPU_VOL_UD;
			break;

		default:
			vol = CPU_VOL_OD;
	}

	return vol;
}

static int h15_SetCPUFreqDown(unsigned int target_freq)
{
	int ret = 0;
	PM_DEBUG_PRINT("CPUFreqDown %u -> %u\n",h15_GetCPUFreq(),target_freq);

	g_h15_freq = target_freq;
	g_h15_vol = h15_freq_vol(target_freq);
	g_h15_do_dvfs = 2; // 0, disable, 1: up, 2 : down, 3:freq_only, 4:vol_only

	return ret;
}

static int h15_SetCPUFreqUp(unsigned int target_freq)
{
	int ret = 0;
	PM_DEBUG_PRINT("CPUFreqUp %u -> %u\n",h15_GetCPUFreq(),target_freq);

	g_h15_freq = target_freq;
	g_h15_vol = h15_freq_vol(target_freq);
	g_h15_do_dvfs = 1; // 0, disable, 1: up, 2 : down, 3:freq_only, 4:vol_only

	return ret;
}

static int h15_SetCPUFreqOnly(unsigned int target_freq)
{
	int ret = 0;

	g_h15_freq = target_freq;
	g_h15_vol = 0;
	g_h15_do_dvfs = 3; // 0, disable, 1: up, 2 : down, 3:freq_only, 4:vol_only

	return ret;
}

static int h15_SetCPUVolOnly(unsigned int target_vol)
{
	int ret = 0;

	g_h15_freq = 0;
	g_h15_vol = target_vol;
	g_h15_do_dvfs = 4; // 0, disable, 1: up, 2 : down, 3:freq_only, 4:vol_only

	return ret;
}

unsigned int h15_GetGPUFreq(void)
{
	return  readl(pms_status_base + 0x10);
}

unsigned int h15_GetGPUVol(void)
{
	return readl(pms_status_base + 0x14);
}

unsigned int h15_GetGPUOn(void)
{
	return readl(pms_status_base + 0x18);
}

static int h15_GetStatus(LX_PM_STATUS_T *pms_status)
{
	pms_status->cpu_freq = h15_GetCPUFreq();
	pms_status->cpu_vol = h15_GetCPUVol();
	pms_status->cpu_on = h15_GetCPUOn();
	pms_status->gpu_freq = h15_GetGPUFreq();
	pms_status->gpu_vol = h15_GetGPUVol();
	pms_status->gpu_on = h15_GetGPUOn();
	pms_status->pm_inited = h15_pm_inited;
	return 0;
}

static int h15_SetStatus(LX_PM_STATUS_T *pms_status)
{
	unsigned int cmd = 0;
	unsigned int arg = 0;
	unsigned int flag = 0;

	if (pms_status->sel == PMS_CMD_CPU_FREQ) {
		cmd = CPU_FS;
		arg = pms_status->cpu_freq;
	}
	else if (pms_status->sel == PMS_CMD_CPU_VOL) {
		cmd = CPU_VS;
		arg = pms_status->cpu_vol;
	}
	else if (pms_status->sel == PMS_CMD_GPU_FREQ) {
		cmd = GPU_FS;
		arg = pms_status->gpu_freq;
	}
	else if (pms_status->sel == PMS_CMD_GPU_VOL) {
		cmd = GPU_VS;
		arg = pms_status->gpu_vol;
	}
	else if( pms_status->sel == PMS_CMD_CHECK_INIT) {
		h15_pm_inited = pms_status->pm_inited;
	}
	else
		return -1;

	if(pms_status->sel != PMS_CMD_CHECK_INIT)
		h15_SendIPC(cmd, arg, flag);

	return 0;
}

static void h15_lgdtv_pm_task_call(void)
{
	complete(&cpu_hotplug_completion);
}

static int h15_lgdtv_pm_task(void *pParam)
{
	unsigned int freq = 0;
	unsigned int vol = 0;
	unsigned int do_dvfs = 0;
	unsigned int core_n = 0;
	unsigned int core_onoff = 1;
	unsigned int do_hotplug = 0;
	volatile int ret = 0;
	struct device * pCpudev = NULL;

	PM_NOTI("h15 lgdtv_pm_task is created\n");

	do {
		/* Check stop condition when device is closed. */
		if (kthread_should_stop()) {
			PM_NOTI("h15 lgdtv_pm_task - exit!\n");
			break;
		}

		reinit_completion(&cpu_hotplug_completion);
		wait_for_completion(&cpu_hotplug_completion);

		PM_DEBUG_PRINT("PM TASK START\n");
		PM_DEBUG_PRINT("task run in cpu%d\n", smp_processor_id());

		if (g_h15_pm_task_disable == 1)
			continue;

#ifdef PM_CM3_RECOVERY
		if (g_h15_err == 0)
		{
#endif
			if (g_h15_do_dvfs)
			{
				freq = g_h15_freq;
				vol = g_h15_vol;
				do_dvfs = g_h15_do_dvfs;
				g_h15_do_dvfs = 0;

			}

			if (g_h15_do_hotplug)
			{
				core_n = g_core_n;
				core_onoff = g_core_onoff;
				do_hotplug = g_h15_do_hotplug;
				g_h15_do_hotplug = 0;
			}
#ifdef PM_CM3_RECOVERY
		}
		else
		{
			do_dvfs = 0;
			do_hotplug = 0;
		}
#endif
		if (do_dvfs == 1)
		{
			do_dvfs = 0;
			PM_DEBUG_PRINT("FREQ UP %ukHz:%uuV\n",freq,vol);
#ifdef CONFIG_CPU_FREQ
			ret = cpufreq_driver_target(cpufreq_cpu_get(0), freq*1000, CPUFREQ_RELATION_H);
			if(ret != 0)
				g_h15_err = 0xF << 28 | freq << 16 | (ERROR_MASK & ret);
#else
			ret = h15_SetCPUVol(vol);
			if(ret != 0)
			{
				g_h15_err = 0xE << 28 | vol << 16  | (ERROR_MASK & ret);
			}
			else
			{
				ret = h15_SetCPUFreq(freq);
				if(ret != 0)
				{
					g_h15_err = 0xF << 28 | freq << 16 | (ERROR_MASK & ret);
				}
			}
#endif
		}
		else if (do_dvfs == 2)
		{
			do_dvfs = 0;
			PM_DEBUG_PRINT("FREQ DN %ukHz:%uuV\n",freq,vol);
#ifdef CONFIG_CPU_FREQ
			ret = cpufreq_driver_target(cpufreq_cpu_get(0), freq*1000 ,CPUFREQ_RELATION_H);
			if(ret != 0)
				g_h15_err = 0xF << 28 | freq << 16 | (ERROR_MASK & ret);
#else
			ret = h15_SetCPUFreq(freq);
			if(ret != 0)
			{
				g_h15_err = 0xF << 28 | freq << 16 | (ERROR_MASK & ret);
			}
			else
			{
				ret = h15_SetCPUVol(vol);
				if(ret != 0)
				{
					g_h15_err = 0xE << 28 | vol << 16 | (ERROR_MASK & ret);
				}
			}
#endif
		}
		else if (do_dvfs == 3)
		{
			do_dvfs = 0;
			PM_DEBUG_PRINT("FREQ %ukHz\n",freq);
			ret = h15_SetCPUFreq(freq);
			if(ret != 0)
			{
				g_h15_err = 0xF << 28 | freq << 16 | (ERROR_MASK & ret);
			}
		}
		else if (do_dvfs == 4)
		{
			do_dvfs = 0;
			PM_DEBUG_PRINT("VOL %uuV\n",vol);
			ret = h15_SetCPUVol(vol);
			if(ret != 0)
			{
				g_h15_err = 0xE << 28 | vol << 16 | (ERROR_MASK & ret);
			}
		}

#ifdef PM_CM3_RECOVERY
		if (do_hotplug && (g_h15_err == 0))
#else
		if (do_hotplug)
#endif
		{
			do_hotplug = 0;

			PM_DEBUG_PRINT("HOTPLUG DO cpu%u:%u\n",core_n,core_onoff);

			if (core_onoff == 0)
			{
				if (cpu_online(core_n))
				{
					ret = cpu_down(core_n);
					if(ret != 0)
					{
						if (ret != -EBUSY)
							g_h15_err = 0xD << 28 | core_n << 24 | core_onoff << 16 | (ERROR_MASK & ret);
					}
					else
					{
						pCpudev = get_cpu_device(core_n);
						device_lock(pCpudev);
						pCpudev->offline = 1;
						device_unlock(pCpudev);

						if (h15_GetCPUOn() & (1 << core_n))
						{
							PM_ERROR("CPU%u POWER do not DOWN\n",core_n);
							g_h15_err = 0xD << 28 | core_n << 24 | core_onoff << 16 | (ERROR_MASK & ret);
						}
					}
				}
			}
			else
			{
				if (!cpu_online(core_n))
				{
					ret = cpu_up(core_n);
					if(ret != 0)
					{
						if (ret != -EBUSY)
							g_h15_err = 0xD << 28 | core_n << 24 | core_onoff << 16 | (ERROR_MASK & ret);
					}
					else
					{
						pCpudev = get_cpu_device(core_n);
						device_lock(pCpudev);
						pCpudev->offline = 0;
						device_unlock(pCpudev);
					}
				}
			}
		}

#ifdef PM_CM3_RECOVERY
		if(g_h15_err != 0)
		{
			PM_DEBUG("GOV[%x]\n",g_h15_err);
			if (cm3_fw_copied == 0)
				g_h15_pm_task_disable = 1;
		}

		if (g_h15_cm3recovery == 1)
		{
			h15_RecoveryCM3();
			g_h15_cm3recovery = 0;
		}
#endif

		PM_DEBUG_PRINT("PM TASK END\n");
	} while (1);

	return 0;
}

static void h15_SetCPUOnOff(unsigned int core_n, unsigned int onoff)
{
	if (cpu_online(core_n) == onoff)
		return;

	g_core_n = core_n;
	g_core_onoff = onoff;

	g_h15_do_hotplug = 1;
}

static void h15_SetMaxPerformance(void)
{
	int i;
	#define CPU_OFF				0
	#define CPU_ON				1


	reinit_completion(&cpu_hotplug_completion);

	cpu_maps_update_begin();
	cpu_maps_update_done();


	for(i=1; i< 4; i++)
	{
		if (cpu_online(i) == 0)
		{
			h15_SetCPUOnOff(i, CPU_ON);
			h15_lgdtv_pm_task_call();
			msleep(40);		/* max cpu on time */
		}
	}
	h15_SetCPUFreqUp(CPU_FREQ_OD);
	h15_lgdtv_pm_task_call();
}

static int h15_test_set(unsigned int testmode)
{
	pms_test_factor.testmode = testmode;
	if (testmode == 0)
	{
		h15_TimerStop();
	}
	else
	{
		h15_TimerStart();
	}
	return 0;
}


static void h15_monitor_func(void)
{
#ifdef PM_CM3_RECOVERY
	static unsigned int monitor_count = 0;
	unsigned int live;

	monitor_count =  (monitor_count + 1) % 0xfffffff;

	//recovery depand on cm3 live
	if ((g_h15_cm3recovery == 0) && (cm3_fw_copied == 1))
	{
		if ( (monitor_count % recovery_check) == 0 )
		{
			live = h15_GetCM3Live();

			if (live !=0 && live_backup == live)
			{
				//PM_DEBUG("[%x]=[%x]\n",live,live_backup);
				dead_count++;
				live_backup = 0;
			}
			else
				dead_count = 0;

			if (dead_count >= DEAD_CONDITON)
			{
				PM_DEBUG("\nDoing CM3 RRECOVERY!![%d][%d][%d][%d]\n",g_h15_cm3recovery, recovery_check,monitor_count,dead_count);
				dead_count = 0;

				g_h15_cm3recovery = 1;
				h15_lgdtv_pm_task_call();
				return;
			}
			live_backup = h15_GetCM3Live();
		}

	}
	else
	{
		dead_count = 0;
		return;
	}
#endif

	return;
}


static void h15_test_func(void)
{
	static unsigned int interval = 0;
	static unsigned int count1s = 0;

	unsigned int cmd;
	unsigned int vol;
	unsigned int freq;
	unsigned int new_freq, new_vol;
	unsigned long long random;
#ifdef PM_CPU_HOTPLUG
	unsigned int core_n;
	unsigned int core_onoff;
	static unsigned int core_stats[4] = {1,1,1,1};
	static unsigned int hotplug_phase = 0;
#endif
	static unsigned int test_count = 0;

	if (pms_test_factor.testmode == 0)
	{
		return;
	}
	else if ((pms_test_factor.testmode <= GPU_FREQ_UP) && (pms_test_factor.testmode > 0))
	{
		count1s++;
		if (count1s >= 10)
		{
			count1s = 0;

			interval++;
			if (interval >= pms_test_factor.interval)
			{
				interval = 0;
				if (pms_test_factor.testmode == 1) {
					cmd = CPU_VS;
					freq = h15_GetCPUFreq();
					vol = h15_GetCPUVol();
				}
				else if (pms_test_factor.testmode == 2) {
					cmd = GPU_VS;
					freq = h15_GetGPUFreq();
					vol = h15_GetGPUVol();
				}
				else if (pms_test_factor.testmode == 3) {
					cmd = CPU_FS;
					freq = h15_GetCPUFreq();
					vol = h15_GetCPUVol();
				}
				else if (pms_test_factor.testmode == 4) {
					cmd = GPU_FS;
					freq = h15_GetGPUFreq();
					vol = h15_GetGPUVol();
				}
				else
					return;

				if (pms_test_factor.testmode & 1)
					printk("CPU ");
				else
					printk("GPU ");

				if ((pms_test_factor.testmode == 1) || (pms_test_factor.testmode == 2))
				{
					printk("Current Freq %uMhz, Vol %umv -> %umv\n",freq,vol,vol - 10);
					vol = vol - 10; // current voltage - 10mv;
					h15_SendIPC(cmd, vol, 0x0);
				}
				else if ((pms_test_factor.testmode == 3) || (pms_test_factor.testmode == 4))
				{
					printk("Current Vol %umv, Freq %uMhz -> %uMhz\n",vol,freq,freq + 24);
					freq = freq + 24;
					h15_SendIPC(cmd, freq, 0x0);
				}

			}
		}
	}
	else if (pms_test_factor.testmode >= CPU_FULL_RANDOM)
	{
		random = sched_clock();

#ifdef PM_CM3_RECOVERY
		if (g_h15_cm3recovery_test)
			if ((random & 0xF) == 0xF)
				if (g_h15_cm3recovery == 0)
					writel(0, pms_cm3_run);
#endif

		if ((pms_test_factor.testmode == CPU_FULL_RANDOM) || (pms_test_factor.testmode == CPU_HOTPLUG_STRESS) || (pms_test_factor.testmode == CPU_DVFS_RANDOM) || (pms_test_factor.testmode == CPU_FREQ_RANDOM) || (pms_test_factor.testmode == CPU_VOL_RANDOM))
		{
			unsigned int freq_t[2];
			unsigned int vol_t[2];

			freq = h15_GetCPUFreq();
			vol = h15_GetCPUVol();

			if (freq == CPU_FREQ_OD)
			{
				freq_t[0] = CPU_FREQ_ND;	vol_t[0] = CPU_VOL_ND;
				freq_t[1] = CPU_FREQ_UD;	vol_t[1] = CPU_VOL_UD;
			}
			else if (freq == CPU_FREQ_ND)
			{
				freq_t[0] = CPU_FREQ_OD;	vol_t[0] = CPU_VOL_OD;
				freq_t[1] = CPU_FREQ_UD;	vol_t[1] = CPU_VOL_UD;
			}
			else
			{
				freq_t[0] = CPU_FREQ_OD;	vol_t[0] = CPU_VOL_OD;
				freq_t[1] = CPU_FREQ_ND;	vol_t[1] = CPU_VOL_ND;
			}

			if ((random & 0x1) == 0)
			{
				new_freq = freq_t[0];
				new_vol = vol_t[0];
			}
			else
			{
				new_freq = freq_t[1];
				new_vol = vol_t[1];
			}

			test_count++;
			if ((pms_test_factor.testmode == CPU_FULL_RANDOM) || (pms_test_factor.testmode == CPU_DVFS_RANDOM))
			{
				printk("[%u]CPU DVFS F%u, V%u -> F%u, V%u\n",test_count,freq,vol,new_freq,new_vol);

				if (new_freq > freq)
				{
					h15_SetCPUFreqUp(new_freq);
				}
				else if (new_freq < freq)
				{
					h15_SetCPUFreqDown(new_freq);
				}
			}
			else if (pms_test_factor.testmode == CPU_FREQ_RANDOM)
			{
				printk("CPU FS F%u, V%u -> F%u, V%u\n",freq,vol,new_freq,vol);

				h15_SetCPUFreqOnly(new_freq);
			}
			else if (pms_test_factor.testmode == CPU_FREQ_RANDOM)
			{
				printk("CPU VS F%u, V%u -> F%u, V%u\n",freq,vol,freq,new_vol);
				h15_SetCPUVolOnly(new_vol);
			}
#ifdef PM_CPU_HOTPLUG
			hotplug_phase++;
			if (hotplug_phase & 1)
			{
				if ((pms_test_factor.testmode == CPU_FULL_RANDOM) || (pms_test_factor.testmode == CPU_HOTPLUG_STRESS))
				{
					core_n = ((unsigned int)(random) % 3) + 1;
					if (core_stats[core_n] == 1)
						core_onoff = 0;
					else
						core_onoff = 1;

					core_stats[core_n] =  core_onoff;

					h15_SetCPUOnOff(core_n, core_onoff);
				}
#endif
			}

			h15_lgdtv_pm_task_call();

		}
		else if ((pms_test_factor.testmode == GPU_DVFS_RANDOM) || (pms_test_factor.testmode == GPU_FREQ_RANDOM) || (pms_test_factor.testmode == GPU_VOL_RANDOM))
		{
			freq = h15_GetGPUFreq();
			vol = h15_GetGPUVol();

			if (random & 0x1)
			{
				new_freq = GPU_FREQ_OD;
				new_vol = GPU_VOL_OD;
			}
			else
			{
				new_freq = GPU_FREQ_ND;
				new_vol = GPU_VOL_ND;
			}

			if (pms_test_factor.testmode == GPU_DVFS_RANDOM)
			{
				printk("GPU DVFS F%u, V%u -> F%u, V%u\n",freq,vol,new_freq,new_vol);
				if (new_freq > freq)
				{
					h15_SendIPC(GPU_VS, new_vol, 0x0);
					h15_SendIPC(GPU_FS, new_freq, 0x0);
				}
				else if (new_freq < freq)
				{
					h15_SendIPC(GPU_VS, new_vol, 0x0);
					h15_SendIPC(GPU_FS, new_freq, 0x0);
				}
			}
			else if (pms_test_factor.testmode == GPU_FREQ_RANDOM)
			{
					printk("GPU FS F%u, V%u -> F%u, V%u\n",freq,vol,new_freq,vol);
					h15_SendIPC(GPU_FS, new_freq, 0x0);
			}
			else if (pms_test_factor.testmode == GPU_FREQ_RANDOM)
			{
					printk("GPU VS F%u, V%u -> F%u, V%u\n",freq,vol,freq,new_vol);
					h15_SendIPC(GPU_VS, new_vol, 0x0);
			}
		}
	}

	return;
}

static void h15_pm_init(void)
{
#ifdef PM_CPU_HOTPLUG
	int cpu;
	unsigned long targetcpu = 0;
#endif
#ifdef PM_USE_KERNEL_FUNC
	int ret;
#endif

	/* initialize pms control base */
	pms_status_base = ioremap(PMS_INFO_BASE, 64);
	pms_ipc_base = ioremap(CM3_IPC_BASE, 512);
	pms_irq_base = ioremap(CM3_IRQ_BASE, 512);
#ifdef PM_CM3_RECOVERY
	pms_cm3_base = ioremap(CM3FW_BASE_ADDRESS, PMS_PARTITION_FW_SIZE);
	pms_cm3_run = ioremap(CM3FW_RUN_STOP, 0x1000);
	pms_cm3_pll = ioremap(CM3FW_PLL_BASE, 0x100);
	flushen_base  = ioremap(CTOP_BASE_FOR_CM3, 0x200);
	pms_systemreset_base = ioremap(CM3_SYSTEM_RESET, 256);
#endif

#ifdef PM_USE_KERNEL_FUNC
	ret = lg1k_pms_get_cpufreq(0);
	if (ret == -ENOSYS)
	{
		PM_NOTI("kernel pms function not implemented\n");
		g_h15_kernel_func = 0;
	}
	else
	{
		PM_NOTI("kernel pms function is implemented\n");
#ifdef CONFIG_CPU_FREQ
		PM_NOTI("cpufreq function is implemented\n");
#endif
		g_h15_kernel_func = 1;
	}
#endif

#ifdef PM_CM3_RECOVERY
	recovery_check = (h15_get_sampling_rate() > CM3_INFO_UPDATE_TIME) ? 1: (CM3_INFO_UPDATE_TIME / h15_get_sampling_rate() +1);
	make_recovery = 20 * recovery_check;
#endif

#ifdef PM_CPU_HOTPLUG
	cpu = 0;	// fix cpu 0

	// pmdrv_task_init
	if (cpu_hotplug_pm_task == NULL) {
		init_completion(&cpu_hotplug_completion);
		cpu_hotplug_pm_task = kthread_create_on_node(h15_lgdtv_pm_task,
					NULL,
					cpu,
					"LGDTV-PMDRV-TASK/%lu", targetcpu);

		if (likely(!IS_ERR(cpu_hotplug_pm_task))) {
			kthread_bind(cpu_hotplug_pm_task, targetcpu);
			wake_up_process(cpu_hotplug_pm_task);
			PM_NOTI("LGDTV-PM-TASK create successed\n");
		} else
			PM_NOTI("LGDTV-PM-TASK create failed\n");
	}
#endif

	pm_h15gov = get_h15pm_gov();
	pm_h15gov->init();
	h15_TimerInit();

	if (h15_pms_tfreq && h15_pms_tvol)
	{
		PM_NOTI("H15 CPU PMS Governor Fixed CLOCK MODE Start - %uMhz/%umV\n",h15_pms_tfreq,h15_pms_tvol);
		if (h15_pms_tfreq > h15_GetCPUFreq())
		{
			h15_SetCPUVol(h15_pms_tvol);
			h15_SetCPUFreq(h15_pms_tfreq);
		}
		else
		{
			h15_SetCPUFreq(h15_pms_tfreq);
			h15_SetCPUVol(h15_pms_tvol);
		}
	}
	else
	{
		if (h15_pms_enable)
		{
			/* CPU Governor Start */
			pm_h15gov->set_startup_timer(60);
			pm_h15gov->set_gov(PMS_GOV_ENABLE);
			h15_TimerStart();
		}
		else
		{
			PM_NOTI("H15 CPU PMS Governor Don't Start\n");
		}
	}

	return;
}

/**
 * ioctl handler for pm device.
 *
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static long h15_pm_ioctl (unsigned int cmd, unsigned long arg)
{
	LX_PM_STATUS_T 		pms_status;
	LX_PM_TEST_FACTOR_T test_factor;
	LX_PM_CM3_INFO_T		cm3_info;
	unsigned int 		status;
	unsigned int 		s_rate;

	int ret = 0;

	if ((_IOC_TYPE(cmd) != PM_DEV_MAJOR) || (_IOC_NR(cmd) != 'L'))
		return -EINVAL;

	switch (_IOC_SIZE(cmd))
	{
		case PM_CPU_GOV_WORK:		// 0 : Disable, 1 : Enable, 2 : Monitor
			h15_test_set(0);
			if (arg <= 3)
			{
				pm_h15gov->set_gov(arg);
				pm_h15gov->set_startup_timer(0);

				if (arg)
					h15_TimerStart();
				else
				{
					h15_TimerStop();
					h15_SetMaxPerformance();
				}
			}
			ret = 0;
			break;
		case PM_GET_CPU_GOV_STATUS:	// 0 : Disable, 1 : Enable, 2: Monitor, 3: Enable & Monutor
			status = pm_h15gov->get_gov();
			ret = copy_to_user((void __user*)(arg), (void *)(&status), sizeof(status));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			break;
		case PM_GET_STATUS:
			ret = copy_from_user((void *)&pms_status, (void __user *)arg, sizeof(pms_status));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			h15_GetStatus(&pms_status);
			ret = copy_to_user((void __user*)(arg), (void *)(&pms_status), sizeof(pms_status));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			break;
		case PM_SET_STATUS:
			ret = copy_from_user((void *)&pms_status, (void __user *)arg, sizeof(pms_status));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			h15_SetStatus(&pms_status);
			ret = 0;
			break;
		case PM_SET_CLOCK_GATING:
			ret = copy_from_user((void *)&pms_clockgating, (void __user *)arg, sizeof(pms_clockgating));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");

			PM_SetClockGating(pms_clockgating.module, pms_clockgating.sub, pms_clockgating.state);
			break;
		case PM_GET_CLOCK_GATING:
			ret = copy_from_user((void *)&pms_clockgating, (void __user *)arg, sizeof(pms_clockgating));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");

			PM_GetClockGating(pms_clockgating.module, pms_clockgating.sub, &pms_clockgating.state);

			ret = copy_to_user((void __user*)(arg), (void *)(&pms_clockgating), sizeof(pms_clockgating));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			break;
		case PM_GET_TEST_FACTOR:
			ret = copy_from_user((void *)&test_factor, (void __user *)arg, sizeof(test_factor));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");

			test_factor.interval = pms_test_factor.interval;

			ret = copy_to_user((void __user*)(arg), (void *)(&test_factor), sizeof(test_factor));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");
			break;
		case PM_SET_TEST_FACTOR:
			ret = copy_from_user((void *)&test_factor, (void __user *)arg, sizeof(test_factor));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");

			pms_test_factor.interval =  test_factor.interval;
			break;
		case PM_GO_TEST:
			ret = copy_from_user((void *)&test_factor, (void __user *)arg, sizeof(test_factor));
			if (ret)
				PM_ERROR("CONFIG DATA COPY FAIL\n");

			//h15_gov_set(0);
			h15_test_set(test_factor.testmode);
			break;
		case PM_STOP_TEST:
#ifdef PM_CM3_RECOVERY
			g_h15_cm3recovery_test = 0;
#endif
			h15_test_set(0);
			break;
		case PM_UART_TO_CM3:
			h15_UartToCM3();
			break;
		case PM_GET_SAMPLING_RATE:
			s_rate = h15_get_sampling_rate() * 10;

			ret = copy_to_user((void __user*)(arg), (void *)(&s_rate), sizeof(s_rate));
			if (ret)
				PM_ERROR("DATA COPY FAIL\n");
			break;
		case PM_SET_SAMPLING_RATE:
			h15_set_sampling_rate(arg);
			break;
		case PM_RECOVERY_CM3:
			#ifdef PM_CM3_RECOVERY
			h15_RecoveryCM3();
			#endif
			PM_ERROR("CM3 RECOVERY\n");
			break;
		case PM_RECOVERY_CM3_TEST:
			#ifdef PM_CM3_RECOVERY
			g_h15_cm3recovery_test = 1;
			#endif
			PM_ERROR("CM3 RECOVERY\n");
			break;
		case PM_SET_CM3_STOP:
			writel(0, pms_cm3_run);
			PM_ERROR("CM3 STOP\n");
			break;
		case PM_SET_CM3_START:
			writel(1, pms_cm3_run);
			PM_ERROR("CM3 START\n");
			break;
		case PM_STORE_CM3_FW:
			ret = copy_from_user((void *)&cm3_info, (void __user *)arg, sizeof(LX_PM_CM3_INFO_T));
			if (ret)
				PM_ERROR("CM3 INFO COPY FAIL\n");
			else
			{
				if (cm3_info.fwsize <= 0x10000)
				{
					if (cm3_fw_backcup == NULL)
 						cm3_fw_backcup = kmalloc(0x10000, GFP_ATOMIC);

					if (cm3_fw_backcup != NULL)
					{
						ret = copy_from_user((void *)cm3_fw_backcup, (void __user *)cm3_info.src, cm3_info.fwsize);
						cm3_fw_copied = 1;

//						PM_DEBUG("pms_cm3_base [%x] cm3_fw_backcup[%x] cm3_fw_header[%x] \n",
//							(unsigned int)pms_cm3_base, (unsigned int)cm3_fw_backcup, (unsigned int)cm3_fw_header);
						if (ret)
							PM_ERROR("CM3 FW COPY FAIL\n");
					}
					else
						PM_ERROR("CM3 FW BUFFER MALLOC FAIL\n");
				}
			}
			PM_DEBUG("CM3 STORE IN KDRV !!!\n");

			break;
		default:
			ret = -EINVAL;
			break;
	}

    return ret;
}

static int h15_pm_suspend(void)
{
	pm_status_backup = pm_h15gov->get_gov();

	pm_h15gov->set_gov(PMS_GOV_DISABLE);
	pm_h15gov->set_startup_timer(0);
	h15_TimerStop();

	return 0;
}

static int h15_pm_resume(void)
{
#ifdef PM_ALLON_RESUME // TZFW 업데이트 이후 동작시킴
	struct device * pCpudev = NULL;
	unsigned int i;
	int	ret;

	g_h15_err = 0;
	g_h15_pm_task_disable = 0;

	for (i=1; i<4; i++)
	{
		if (cpu_online(i) == 0)
		{
			ret = cpu_up(i);
			if(ret == 0)
			{
				pCpudev = get_cpu_device(i);
				device_lock(pCpudev);
				pCpudev->offline = 0;
				device_unlock(pCpudev);
			}
		}
	}
#else
	g_h15_err = 0;
	g_h15_pm_task_disable = 0;
#endif

	if ((pm_status_backup & PMS_GOV_MASK) == PMS_GOV_DISABLE)
	{
		pm_h15gov->set_gov(PMS_GOV_DISABLE);
		pm_h15gov->set_startup_timer(0);
		h15_TimerStop();
		pm_status_backup = PMS_GOV_DISABLE;

		printk("H15 CPU Governor Disable!\n");
		return 0;
	}

	if ((pm_status_backup & PMS_GOV_TOSUSPEND_FLAG) == PMS_GOV_TOSUSPEND_FLAG)
		pm_status_backup = pm_status_backup & PMS_GOV_MASK;

	pm_h15gov->set_gov(pm_status_backup);
	pm_h15gov->set_startup_timer(60);
	h15_TimerStart();

	return 0;
}

static pms_driver_t h15_pms_driver =
{
	.init = h15_pm_init,
	.ioctl = h15_pm_ioctl,
	.suspend = h15_pm_suspend,
	.resume = h15_pm_resume,
};

pms_driver_t* get_h15pm_driver(void)
{
	return &h15_pms_driver;
}

static pms_h15func_t h15_pm_func =
{
	.getfreq = h15_GetCPUFreq,
	.getvol = h15_GetCPUVol,
	.frequp = h15_SetCPUFreqUp,
	.freqdown = h15_SetCPUFreqDown,
	.set_cpu_onoff = h15_SetCPUOnOff,
	.get_cpu_onoff = h15_GetCPUOn,
	.run = h15_lgdtv_pm_task_call,
	.get_sampling = h15_get_sampling_rate,
	.set_sampling = h15_set_sampling_rate,
	.start_timer = h15_TimerStart,
	.stop_timer = h15_TimerStop,
	.set_max_performance = h15_SetMaxPerformance,
};

pms_h15func_t* get_h15pm_func(void)
{
	return &h15_pm_func;
}
#endif

