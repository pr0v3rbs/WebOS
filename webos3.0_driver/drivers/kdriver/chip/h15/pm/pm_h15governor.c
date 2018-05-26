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
#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
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

#include "pm_h15common.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

//#define PM_DEBUG_PRINT
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define HARD_UP_THRESHOLD		90
#define EASY_UP_THRESHOLD		75
#define PM_MP_CONTROL

/* h15_load_dump related*/
enum {
	UD_CNT = 0,
	ND_CNT,
	OD_CNT,
	UD_PER ,
	ND_PER ,
	OD_PER ,
};


/* h15_cpu_load related */
enum {
	LOAD_CPU0 = 0,
	LOAD_CPU1,
	LOAD_CPU2,
	LOAD_CPU3,
	LOAD_MAX,
	LOAD_AVG
 };

#define	MP_TASK_GO		0
#define	MP_TASK_NONEED	1

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
static unsigned int h15_startup_timer = 60;
static unsigned int h15_gov_status = PMS_GOV_DISABLE;
static unsigned int h15_gov_up_threshold = 0;
static unsigned int h15_gov_hard_up_threshold = HARD_UP_THRESHOLD;
static unsigned int h15_gov_easy_up_threshold = EASY_UP_THRESHOLD;
static unsigned int h15_gov_easy_count = 48;
static unsigned int h15_gov_od_freqdown_count = 24;
static unsigned int h15_gov_nd_freqdown_count = 24;
static unsigned int h15_gov_mpd_count = 100 + (100 << 8) +  (1 << 16);
static unsigned int h15_gov_min_freq = CPU_FREQ_UD;
static unsigned int h15_gov_max_freq = CPU_FREQ_OD;

static pms_h15func_t* pm_h15func = NULL;

static unsigned int h15_mp_enable = 1;

static unsigned int h15_cpu_load[6] = {0,0,0,0,0,0};
static unsigned int h15_load_dump[6] = {0,0,0,0,0,0};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
//static unsigned int PM_Poll(struct file *filp, poll_table *wait);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
#ifdef KDRV_CONFIG_PM
static void PM_print_cpuload(unsigned int max_load, unsigned int *cpu_load, unsigned int *load_data, unsigned int freq)
{
	if (freq == CPU_FREQ_OD)		printk("O:");
	else if (freq == CPU_FREQ_ND)	printk("N:");
	else if (freq == CPU_FREQ_UD)	printk("U:");

	printk("%3u:",cpu_load[LOAD_CPU0]);
	if (cpu_online(1))		printk("%3u:",cpu_load[LOAD_CPU1]);
	else					printk(" x :");
	if (cpu_online(2))		printk("%3u:",cpu_load[LOAD_CPU2]);
	else					printk(" x :");
	if (cpu_online(3))		printk("%3u: ",cpu_load[LOAD_CPU3]);
	else					printk(" x :");

	printk("%3u:%3u:",max_load,h15_cpu_load[LOAD_AVG]);
	printk("u%u:", h15_gov_up_threshold);

 	printk("%u:%u:%u\n",load_data[UD_PER],load_data[ND_PER],load_data[OD_PER]);
}

static inline u64 PM_get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

	busy_time = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

	idle_time = cur_wall_time - busy_time;
	if (wall)
		*wall = cputime_to_usecs(cur_wall_time);

	return cputime_to_usecs(idle_time);
}

static u64 PM_get_cpu_idle_time(unsigned int cpu, u64 *wall, int io_busy)
{
extern u64 get_cpu_idle_time_us(int cpu, u64 *last_update_time);
extern u64 get_cpu_iowait_time_us(int cpu, u64 *last_update_time);

	u64 idle_time = get_cpu_idle_time_us(cpu, io_busy ? wall : NULL);

	if (idle_time == -1ULL)
		return PM_get_cpu_idle_time_jiffy(cpu, wall);
	else if (!io_busy)
		idle_time += get_cpu_iowait_time_us(cpu, wall);

	return idle_time;
}

unsigned int h15_get_cpuload(void)
{
	unsigned int max_load = 0;
	unsigned int min_load = 0;
	unsigned int j;

	static u64 prev_cpu_wall[4] = {0,0,0,0}, prev_cpu_idle[4] = {0,0,0,0};
	unsigned int cpu_load[4];
	static unsigned int cpu_on_backup[4] = {0,0,0,0};
	unsigned int count_on;

	/* Get CPU Load */
	for(j=0,count_on=0; j<NR_CPUS; j++) {
		u64 cur_wall_time, cur_idle_time;
		unsigned int idle_time, wall_time;
		unsigned int load;
		int io_busy = 0;

		if (!cpu_online(j))
		{
			cpu_on_backup[j] = 0;
			cpu_load[j] = 0;
			h15_cpu_load[j] = 0;
			continue;
		}
		count_on++;

		cur_idle_time = PM_get_cpu_idle_time(j, &cur_wall_time, io_busy);
		wall_time = (unsigned int)
			(cur_wall_time - prev_cpu_wall[j]);
		prev_cpu_wall[j] = cur_wall_time;

		idle_time = (unsigned int)
			(cur_idle_time - prev_cpu_idle[j]);
		prev_cpu_idle[j] = cur_idle_time;

		if (unlikely(!wall_time || wall_time < idle_time))
		{
			cpu_on_backup[j] = 1;
			cpu_load[j] = 0;
			h15_cpu_load[j] = 0;
			continue;
		}

		if (cpu_on_backup[j])
		{
			load = 100 * (wall_time - idle_time) / wall_time;
			if (load > 100)
				load = 100;
		}
		else
		{
			load = 0;
		}

		cpu_on_backup[j] = 1;

		cpu_load[j] = load;
		h15_cpu_load[j] =  load;

		if (load > max_load)
			max_load = load;

		if (load < min_load)
			min_load = load;

		h15_cpu_load[LOAD_MAX] = max_load;

	}

	h15_cpu_load[LOAD_AVG] = 0;

	for(j=0 ; j< count_on; j++)
	{
		h15_cpu_load[LOAD_AVG] += h15_cpu_load[j];
	}
	if(count_on)
		h15_cpu_load[LOAD_AVG] = h15_cpu_load[LOAD_AVG] / count_on ;

	return 0;
}

unsigned int h15_freq_governor(unsigned int load, unsigned int current_clock)
{
	unsigned int target;
	unsigned int target_clock = 0;
	static unsigned int freqdown_count = 0;
	static unsigned int easy_up_count = 0;

	// CPU Freq Up Condition
	if (h15_gov_up_threshold == 0)
	{
		h15_gov_up_threshold = h15_gov_hard_up_threshold;
		easy_up_count = 0;
	}
	else if (h15_gov_up_threshold == h15_gov_easy_up_threshold)
	{
		easy_up_count++;
		if (easy_up_count >= h15_gov_easy_count)
		{
			h15_gov_up_threshold = h15_gov_hard_up_threshold;
			easy_up_count = 0;
		}

		if (current_clock == CPU_FREQ_OD)
			easy_up_count = 0;
	}
	else
	{
		easy_up_count = 0;
	}

	if (load > h15_gov_up_threshold)
	{
		target_clock = CPU_FREQ_OD;
		freqdown_count = 0;

		h15_gov_up_threshold = h15_gov_easy_up_threshold;
	}
	else
	{
		freqdown_count++;
		if (current_clock == CPU_FREQ_OD)
		{
			if (freqdown_count >= h15_gov_od_freqdown_count)
			{
				freqdown_count = 0;
				target = ((load * CPU_FREQ_OD) * 120) / 10000;
				if (target <= CPU_FREQ_ND)
				{
					target_clock = CPU_FREQ_ND;
				}
			}
		}
		else if (current_clock == CPU_FREQ_ND)
		{
			if (freqdown_count >= h15_gov_nd_freqdown_count)
			{
				freqdown_count = 0;
				target = ((load * CPU_FREQ_ND) * 120) / 10000;
				if (target < CPU_FREQ_UD)
				{
					target_clock = CPU_FREQ_UD;
				}
			}
		}
		else
		{
			freqdown_count = 0;
		}
	}

	if (target_clock)
	{
		if (target_clock < h15_gov_min_freq)
			target_clock = h15_gov_min_freq;

		if (target_clock > h15_gov_max_freq)
			target_clock = h15_gov_max_freq;

		if (target_clock > current_clock)
			pm_h15func->frequp(target_clock);
		else if (target_clock < current_clock)
			pm_h15func->freqdown(target_clock);
	}

	if (target_clock == 0)
		target_clock = current_clock;

	return target_clock;
}

#ifdef PM_MP_CONTROL
unsigned int h15_mp_governor(unsigned int current_clock, unsigned int *cpu_load)
{
	static unsigned int current_clock_backup = 0;
	static unsigned int hotplug_count = 0;
	static unsigned int cpu_on_target = 4;
	unsigned int cpu_on_count;
	unsigned int hotplug_condition = 0;

	unsigned int second_cpu_n = 0;
	unsigned int third_cpu_n = 0;

	unsigned int i;

#define CPU_OFF				0
#define CPU_ON				1

	// CPU의 클럭에 연동된 MP제어
	for (i=1, cpu_on_count=1, second_cpu_n=0; i<4; i++)
	{
		if (cpu_online(i))
		{
			if (second_cpu_n > 0)
				third_cpu_n = i;

			if (second_cpu_n == 0)
				second_cpu_n = i;

			cpu_on_count++;
		}
	}

	if (current_clock != current_clock_backup)
	{
		if ((current_clock_backup != CPU_FREQ_ND) && (current_clock_backup != CPU_FREQ_UD))
			hotplug_count = 0;
		current_clock_backup = current_clock;
	}

	if (current_clock == CPU_FREQ_OD)
	{
		hotplug_condition = ((h15_gov_mpd_count >> 16) & 0xFF);
		if (cpu_on_count <= 2)
		{
			if ((cpu_load[0]  + cpu_load[second_cpu_n]) >= 120) /* if avg load is higher than 60%  up cpu */
			{
				hotplug_count++;
				if (hotplug_count >= hotplug_condition)
				{
					cpu_on_target = 3;
					hotplug_count = 0;
				}
			}
			else
			{
				hotplug_count = 0;
			}
		}
		else if (cpu_on_count == 3)
		{


			if ((cpu_load[0] + cpu_load[second_cpu_n]  + cpu_load[third_cpu_n]) >= 180) /* if avg load is higher than 60%  up cpu */
			{
				hotplug_count++;
				if (hotplug_count >= hotplug_condition)
				{
					cpu_on_target = 4;
					hotplug_count = 0;
				}
			}
			else
			{
				hotplug_count = 0;
			}
		}
		else
		{
			hotplug_count = 0;
		}
	}
	else if (current_clock == CPU_FREQ_ND)
	{
		hotplug_condition = ((h15_gov_mpd_count >> 8) & 0xFF);
		hotplug_count++;
		if (hotplug_count >= hotplug_condition)
		{
			if (cpu_on_count == 4)
			{
				cpu_on_target = 3;
				hotplug_count = 0;
			}
			else if (cpu_on_count == 3)
			{
				cpu_on_target = 2;
				hotplug_count = 0;
			}
		}
	}
	else // if (current_clock == CPU_FREQ_UD)
	{
		hotplug_condition = h15_gov_mpd_count & 0xFF;
		hotplug_count++;
		if (hotplug_count >= hotplug_condition)
		{
			if (cpu_on_count == 4)
			{
				cpu_on_target = 3;
				hotplug_count = 0;
			}
			else if (cpu_on_count == 3)
			{
				cpu_on_target = 2;
				hotplug_count = 0;
			}
		}
	}

	if (cpu_on_count < cpu_on_target)
	{
		for(i=1; i<4; i++)
		{
			if (cpu_online(i) == 0)
			{
				pm_h15func->set_cpu_onoff(i, CPU_ON);
				break;
			}
		}
	}
	else if (cpu_on_count > cpu_on_target)
	{
		for(i=3; i>0; i--)
		{
			if (cpu_online(i))
			{
				pm_h15func->set_cpu_onoff(i, CPU_OFF);
				break;
			}
		}
	}

	if (cpu_on_count != cpu_on_target)
		return MP_TASK_GO;
	else
		return MP_TASK_NONEED;
}
#endif

static void h15_check_cpu(void)
{
	static unsigned int timer_count = 0;

	unsigned int current_clock = 0;
	unsigned int target_clock = 0;
	unsigned int mode_cnt_sum = 0;
	unsigned int task_run = 0;
	unsigned int task_mp = 0;

	if (h15_gov_status == PMS_GOV_DISABLE)
		return;

	if (h15_gov_status & PMS_GOV_TOSUSPEND_FLAG)
		return;

	if (h15_startup_timer > 0)
	{
		timer_count++;
		if (timer_count >= (HZ/pm_h15func->get_sampling()))
		{
			timer_count = 0;
			h15_startup_timer--;
		}
		return;
	}
	else
	{
		if (timer_count == 0)
		{
			printk("H15 CPU Governor Start!\n");
			timer_count = 1;
		}
	}

	// calculate CPU Load
	h15_get_cpuload();

	current_clock = pm_h15func->getfreq();
	if (current_clock == CPU_FREQ_UD)
	{
		h15_load_dump[UD_CNT]++;

	}
	else if (current_clock == CPU_FREQ_ND)
	{
		h15_load_dump[ND_CNT]++;
	}
	else if (current_clock == CPU_FREQ_OD)
	{
		h15_load_dump[OD_CNT]++;
	}

	if ((h15_load_dump[UD_CNT] == 0) || (h15_load_dump[ND_CNT] == 0) || (h15_load_dump[OD_CNT] == 0))
	{
		h15_load_dump[UD_CNT] = 1;
		h15_load_dump[ND_CNT] = 1;
		h15_load_dump[OD_CNT] = 1;
	}


	/* */
	mode_cnt_sum= h15_load_dump[UD_CNT] + h15_load_dump[ND_CNT] + h15_load_dump[OD_CNT];

	h15_load_dump[UD_PER]= h15_load_dump[UD_CNT] * 100 /  mode_cnt_sum;
	h15_load_dump[ND_PER]= h15_load_dump[ND_CNT] * 100 /  mode_cnt_sum;
	h15_load_dump[OD_PER]= h15_load_dump[OD_CNT] * 100 /  mode_cnt_sum;

	if (h15_gov_status == PMS_GOV_MONITOR)
	{
		PM_print_cpuload(h15_cpu_load[LOAD_MAX], h15_cpu_load, h15_load_dump, current_clock);
		return;
	}

	if (current_clock == 0)
	{
		pm_h15func->frequp(CPU_FREQ_ND);
		pm_h15func->run();
		return;
	}

	// call CPU Freq Governor
	target_clock = h15_freq_governor(h15_cpu_load[LOAD_MAX], current_clock);
	if (current_clock != target_clock)
		task_run = 1;

#ifdef PM_MP_CONTROL
	// call MP Decision Governor
	if (h15_mp_enable)
	{
		task_mp = h15_mp_governor(target_clock ? target_clock : current_clock, h15_cpu_load);
		if (task_mp == MP_TASK_GO)
			task_run = 1;
	}
#endif

	if (h15_gov_status == PMS_GOV_EN_N_MON)
		PM_print_cpuload(h15_cpu_load[LOAD_MAX], h15_cpu_load, h15_load_dump,target_clock ? target_clock : current_clock);

	if (task_run)
	pm_h15func->run();
}

static unsigned int h15_pm_get_gov(void)
{
	return h15_gov_status;
}

static void h15_pm_reset_load_monitor(void)
{
	h15_load_dump[UD_CNT] = 0;
	h15_load_dump[ND_CNT] = 0;
	h15_load_dump[OD_CNT] = 0;
}
static int h15_pm_set_gov(unsigned int value)
{
	h15_gov_status =  value;
	if( value == 0 )
		h15_pm_reset_load_monitor();
	return 0;
}

static unsigned int h15_pm_get_hard_upthreshold(void)
{
	return h15_gov_hard_up_threshold;
}

static int h15_pm_set_hard_upthreshold(unsigned int up_threshold)
{
	if (h15_gov_up_threshold == h15_gov_hard_up_threshold)
		h15_gov_up_threshold = up_threshold;

	h15_gov_hard_up_threshold = up_threshold;

	return 0;
}

static unsigned int h15_pm_get_easy_upthreshold(void)
{
	return h15_gov_easy_up_threshold;
}

static int h15_pm_set_easy_upthreshold(unsigned int up_threshold)
{
	if (h15_gov_up_threshold == h15_gov_easy_up_threshold)
		h15_gov_up_threshold = up_threshold;

	h15_gov_easy_up_threshold = up_threshold;

	return 0;
}

static unsigned int h15_pm_get_easy_count(void)
{
	return h15_gov_easy_count;
}

static int h15_pm_set_easy_count(unsigned int count)
{
	h15_gov_easy_count = count;

	return 0;
}

static unsigned int h15_pm_get_startup_timer(void)
{
	// return value unit is second
	return h15_startup_timer;
}

static int h15_pm_set_startup_timer(unsigned int startup_timer)
{
	// input value unit is second
	h15_startup_timer = startup_timer;

	return 0;
}

static void h15_gov_init(void)
{
	pm_h15func = get_h15pm_func();
}



static unsigned int h15_pm_get_od_freqdown_count(void)
{
	return h15_gov_od_freqdown_count;
}

static int h15_pm_set_od_freqdown_count(unsigned int value)
{
	h15_gov_od_freqdown_count = value;

	return 0;
}
static unsigned int h15_pm_get_nd_freqdown_count(void)
{
	return h15_gov_nd_freqdown_count;
}

static int h15_pm_set_nd_freqdown_count(unsigned int value)
{
	h15_gov_nd_freqdown_count = value;

	return 0;
}

static unsigned int h15_pm_get_mpd_count(void)
{
	return h15_gov_mpd_count;
}

static int h15_pm_set_mpd_count(unsigned int value)
{
	h15_gov_mpd_count = value;

	return 0;
}

static unsigned int h15_pm_get_min_freq(void)
{
	return h15_gov_min_freq;
}

static int h15_pm_set_min_freq(unsigned int value)
{
	h15_gov_min_freq = value;

	return 0;
}

static unsigned int h15_pm_get_max_freq(void)
{
	return h15_gov_max_freq;
}

static int h15_pm_set_max_freq(unsigned int value)
{
	h15_gov_max_freq = value;

	return 0;
}

static unsigned int h15_pm_get_load(unsigned int cpu)
{
	return h15_cpu_load[cpu];
}

static int h15_pm_set_mp_enable(unsigned int value)
{
	h15_mp_enable = value;

	return 0;
}

static unsigned int h15_pm_get_mp_enable(void)
{
	return h15_mp_enable;
}

static pms_h15gov_t h15_pm_gov =
{
	.init = h15_gov_init,
	.check_cpu = h15_check_cpu,
	.get_gov = h15_pm_get_gov,
	.set_gov = h15_pm_set_gov,
	.get_hard_up_threshold = h15_pm_get_hard_upthreshold,
	.set_hard_up_threshold = h15_pm_set_hard_upthreshold,
	.get_easy_up_threshold = h15_pm_get_easy_upthreshold,
	.set_easy_up_threshold = h15_pm_set_easy_upthreshold,
	.get_easy_count = h15_pm_get_easy_count,
	.set_easy_count = h15_pm_set_easy_count,
	.get_startup_timer = h15_pm_get_startup_timer,
	.set_startup_timer = h15_pm_set_startup_timer,
	.reset_load_monitor = h15_pm_reset_load_monitor,
	.get_od_freqdown_count = h15_pm_get_od_freqdown_count,
	.set_od_freqdown_count = h15_pm_set_od_freqdown_count,
	.get_nd_freqdown_count = h15_pm_get_nd_freqdown_count,
	.set_nd_freqdown_count = h15_pm_set_nd_freqdown_count,
	.get_mpd_count = h15_pm_get_mpd_count,
	.set_mpd_count = h15_pm_set_mpd_count,
	.get_min_freq = h15_pm_get_min_freq,
	.set_min_freq = h15_pm_set_min_freq,
	.get_max_freq = h15_pm_get_max_freq,
	.set_max_freq = h15_pm_set_max_freq,
	.get_mp_enable = h15_pm_get_mp_enable,
	.set_mp_enable = h15_pm_set_mp_enable,
	.get_load = h15_pm_get_load,
};

pms_h15gov_t* get_h15pm_gov(void)
{
	return &h15_pm_gov;
}

#endif

