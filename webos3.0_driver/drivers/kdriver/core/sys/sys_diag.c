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
 *  diagnosis system driver implementation
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2010.12.13
 *  note		Additional information.
 *
 *  @addtogroup lg115x_sys
 *	@{
 */


/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "sys_drv.h"
#include "sys_diag.h"


/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DIAG_CHECKER_MAX	128

#define CHECK_MODULE_PARAM(module, action) \
	do { \
		if(module < 0 || module >= LX_DIAG_MODULE_MAX) { \
			action; \
		} \
	} while(0)

#define CHECK_TYPE_PARAM(type, action) \
	do { \
		if(type < 0 || type >= LX_DIAG_CHECKER_TYPE_MAX) { \
			action; \
		} \
	} while(0)


/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
struct diag_checker_normal
{
	int (*func)(int type, void *arg);
	void *arg;
};

struct diag_checker_rang
{
	int		min;
	int		max;
	void	*var;
};

struct diag_checker_irq
{
	int min_freq;
	int max_freq;
	int max_duration;

	unsigned long time_enter;
	unsigned long stats_min_freq, stats_max_freq, stats_max_duration;
};

struct diag_checker
{
	int module;
	int type;

	int index;
	atomic_t used;

	union {
		struct diag_checker_normal	normal;
		struct diag_checker_irq		irq;
		struct diag_checker_rang	range;
	};

	struct diag_checker *next;
};

struct diag_checker_head {
	spinlock_t lock;
	struct diag_checker *head;
};

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 global Variables
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *	 Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
static int diag_panic_event(struct notifier_block *nb, unsigned long val, void *v);

/*----------------------------------------------------------------------------------------
 *	 Static Variables
 *---------------------------------------------------------------------------------------*/
static struct notifier_block diag_panic_nb = {
	.notifier_call	= diag_panic_event,
	.priority		= 1,
};


static struct diag_checker _diag_checker[DIAG_CHECKER_MAX];
static struct diag_checker_head diag_checker_list[LX_DIAG_MODULE_MAX];

static const char *diag_module_name[] =
{
	[LX_DIAG_AAD]	= "AAD",
	[LX_DIAG_AUDIO]	= "AUDIO",
	[LX_DIAG_AFE]	= "AFE",
	[LX_DIAG_CI]	= "CI",
	[LX_DIAG_DE]	= "DE",
	[LX_DIAG_DEMOD]	= "DEMOD",
	[LX_DIAG_DENC]	= "DENC",
	[LX_DIAG_DIMM]	= "DIMM",
	[LX_DIAG_FBDEV]	= "FBDEV",
	[LX_DIAG_FMT3D]	= "FMT3D",
	[LX_DIAG_GFX]	= "GFX",
	[LX_DIAG_GPU]	= "GPU",
	[LX_DIAG_GPIO]	= "GPIO",
	[LX_DIAG_HDMI]	= "HDMI",
	[LX_DIAG_I2C]	= "I2C",
	[LX_DIAG_SPI]	= "SPI",
	[LX_DIAG_PE]	= "PE",
	[LX_DIAG_PVR]	= "PVR",
	[LX_DIAG_SDEC]	= "SDEC",
	[LX_DIAG_VBI]	= "VBI",
	[LX_DIAG_VENC]	= "VENC",
	[LX_DIAG_PNG]	= "PNG",
	[LX_DIAG_SE]	= "SE",
	[LX_DIAG_SYS]	= "SYS",
	[LX_DIAG_SCI]	= "SCI",
	[LX_DIAG_CLK]	= "CLK",
	[LX_DIAG_MONITOR]	= "MONITOR",
	[LX_DIAG_MJPEG]	= "MJPEG",
	[LX_DIAG_VDEC]	= "VDEC",
	[LX_DIAG_APR]	= "APR",
	[LX_DIAG_BE]	= "BE",
	[LX_DIAG_OVI]	= "OVI",
};

static const char* get_module_name(int module)
{
	const char *name = NULL;
	if(module >= 0 && module < LX_DIAG_MODULE_MAX)
		name = diag_module_name[module];

	if(name == NULL) return "Unknown";
	return name;
}

static struct diag_checker* get_checker(int module, int type)
{
	int i;
	struct diag_checker *checker = NULL;

	for(i=0; i<DIAG_CHECKER_MAX; i++)
	{
		if(atomic_cmpxchg(&_diag_checker[i].used, 0, 1) == 0)
		{
			checker = &_diag_checker[i];
			break;
		}
	}
	if(checker == NULL) return NULL;

	checker->module = module;
	checker->type = type;

	return checker;
}

static int add_checker(struct diag_checker* checker)
{
	int module;
	int type;
	struct diag_checker_head *list;
	struct diag_checker *head;

	module = checker->module;
	type = checker->type;
	list = &diag_checker_list[module];

	spin_lock(&list->lock);
	checker->index = 0;
	head = list->head;
	if(head == NULL)
	{
		list->head = checker;
		spin_unlock(&list->lock);
		return 0;
	}

	while(1)
	{
		if(head->type == type) checker->index++;
		if(head->next == NULL) break;
		head = head->next;
	}
	head->next = checker;
	spin_unlock(&list->lock);

	return 0;
}

int diag_init(void)
{
	int i;

	for(i=0; i<LX_DIAG_MODULE_MAX; i++)
	{
		spin_lock_init(&diag_checker_list[i].lock);
	}

	atomic_notifier_chain_register(&panic_notifier_list, &diag_panic_nb);

	return 0;
}

struct diag_checker* diag_register(int module, int type, int (*func)(int type, void *arg), void *arg)
{
	struct diag_checker* checker;

	CHECK_MODULE_PARAM(module, return NULL);
	CHECK_TYPE_PARAM(type, return NULL);

	checker = get_checker(module, type);
	if(checker == NULL) return NULL;

	checker->normal.func = func;
	checker->normal.arg = arg;

	add_checker(checker);

	return checker;
}


/* Range Checker */
struct diag_checker* diag_register_range(int module, void *var, int min, int max)
{
	struct diag_checker* checker;

	CHECK_MODULE_PARAM(module, return NULL);

	checker = get_checker(module, LX_DIAG_CHECKER_RANGE);
	if(checker == NULL) return NULL;

	checker->range.var = var;
	checker->range.min = min;
	checker->range.max = max;

	add_checker(checker);

	return checker;
}

int diag_specify_range(struct diag_checker *checker, int min, int max)
{
	if(checker == NULL || checker->type != LX_DIAG_CHECKER_RANGE)
		return -1;

	checker->range.min = min;
	checker->range.max = max;

	return 0;
}

/* Interrupt Handler Diagnosis */
struct diag_checker* diag_register_irq(int module, int min_freq, int max_freq, int max_duration)
{
	struct diag_checker* checker;

	CHECK_MODULE_PARAM(module, return NULL);

	checker = get_checker(module, LX_DIAG_CHECKER_IRQ);
	if(checker == NULL) return NULL;

	checker->irq.min_freq = min_freq;
	checker->irq.max_freq = max_freq;
	checker->irq.max_duration = max_duration;

	// initialize internal data
	checker->irq.stats_min_freq = (unsigned long)-1;
	checker->irq.stats_max_freq = 0;
	checker->irq.stats_max_duration = 0;

	add_checker(checker);

	return checker;
}

int diag_specify_irq(struct diag_checker *checker, int min_freq, int max_freq, int max_duration)
{
	if(checker == NULL || checker->type != LX_DIAG_CHECKER_IRQ)
		return -1;

	checker->irq.min_freq = min_freq;
	checker->irq.max_freq = max_freq;
	checker->irq.max_duration = max_duration;

	return 0;
}

int diag_irq_enter(struct diag_checker* checker)
{
	unsigned long clock;
	if(checker == NULL || checker->type != LX_DIAG_CHECKER_IRQ)
		return -1;

	clock = sched_clock();
#if 0
	if((checker->irq.max_freq > 0 || checker->irq.min_freq > 0) &&
		checker->irq.time_enter != 0 )
#endif
	if(checker->irq.time_enter != 0)
	{
		unsigned long diff = (unsigned long)(clock - checker->irq.time_enter) / 1000;	// in usec unit

	#if 0	// report or just save
		if(checker->irq.max_freq > 0 && diff > checker->irq.max_freq)
		{

		}
		else if(checker->irq.min_freq > 0 && diff < checker->irq.min_freq)
		{

		}
	#else
		if(diff < checker->irq.stats_min_freq)
			checker->irq.stats_min_freq = diff;
		if(diff > checker->irq.stats_max_freq)
			checker->irq.stats_max_freq = diff;
	#endif
	}
	checker->irq.time_enter = clock;

	return 0;
}

int diag_irq_exit(struct diag_checker* checker)
{
	if(checker == NULL || checker->type != LX_DIAG_CHECKER_IRQ)
		return -1;

	if(1)	//checker->irq.max_duration)
	{
		unsigned long clock = sched_clock();
		unsigned long diff = (unsigned long)(clock - checker->irq.time_enter) / 1000;	// in usec unit

	#if 0	// report or just save
		if(diff > checker->irq.max_duration)
		{

		}
	#else
		if(diff > checker->irq.stats_max_duration)
			checker->irq.stats_max_duration = diff;
	#endif
	}

	return 0;
}
/* End of Interrupt Handler Diagnosis */



/* Diagnosis Reporting */
#define DIAG_REPORT_BUFFER_SIZE		(1024*16)
static char diag_log_buf[DIAG_REPORT_BUFFER_SIZE];	// 16KB
static int diag_log_buf_current;
int diag_printf(const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int len, size;

	size = DIAG_REPORT_BUFFER_SIZE - diag_log_buf_current;
	if(size <= 0) return 0;

	buf = diag_log_buf + diag_log_buf_current;
	va_start(ap, fmt);
	len = vscnprintf(buf, size, fmt, ap);
	va_end(ap);
	diag_log_buf_current += len;

	return len;
}

static int invoke(struct diag_checker *checker)
{
	if(checker->type == LX_DIAG_CHECKER_PANIC ||
		checker->type == LX_DIAG_CHECKER_STATUS)
	{
		checker->normal.func(checker->type, checker->normal.arg);
	}
	else if(checker->type == LX_DIAG_CHECKER_IRQ)
	{
		diag_printf("IRQ - %s[%d]\n", get_module_name(checker->module), checker->index);
		diag_printf("MIN:%d, MAX:%d, DURATION:%d\n",
			checker->irq.min_freq, checker->irq.max_freq, checker->irq.max_duration);
		diag_printf("Statistic. MIN:%d, MAX:%d, DURATION:%d\n",
			checker->irq.stats_min_freq, checker->irq.stats_max_freq, checker->irq.stats_max_duration);
	}

	return 0;
}

int diag_invoke_type(int type)
{
	int i;
	struct diag_checker *checker;
	struct diag_checker_head *list;

	for(i=0; i<LX_DIAG_MODULE_MAX; i++)
	{
		list = &diag_checker_list[i];
		checker = list->head;
		while(checker)
		{
			if(checker->type == type)
				invoke(checker);
			checker = checker->next;
		}
	}
	return 0;
}

int diag_invoke_module(int module)
{
	struct diag_checker *checker;
	struct diag_checker_head *list;

	CHECK_MODULE_PARAM(module, return -1);

	list = &diag_checker_list[module];
	checker = list->head;
	while(checker)
	{
		invoke(checker);
		checker = checker->next;
	}
	return 0;
}

int diag_invoke_all(void)
{
	int i;

	for(i=0; i<LX_DIAG_MODULE_MAX; i++)
	{
		diag_invoke_module(i);
	}
	return 0;
}

int diag_invoke_reset(void)
{
	diag_log_buf_current = 0;
	return 0;
}

int diag_report(int type)
{
	if(type == LX_DIAG_REPORT_TYPE_PRINT)
	{
		static char buf[512];
		int i = 0;

		while(i<diag_log_buf_current)
		{
			int size = min(diag_log_buf_current-i, 511);
			memcpy(buf, diag_log_buf+i, size);
			buf[size] = '\0';
			printk(buf);
			i += size;
		}
	}
	else if(type == LX_DIAG_REPORT_TYPE_STORAGE)
	{
		printk("-- report to storage --\n");
	}
	return 0;
}

static int diag_panic_event(struct notifier_block *nb, unsigned long val, void *v)
{
	diag_invoke_reset();

	diag_printf("\n--- diagnosis panic event ----\n");
	diag_invoke_type(LX_DIAG_CHECKER_PANIC);
	diag_report(LX_DIAG_REPORT_TYPE_PRINT);
	return NOTIFY_DONE;
}


