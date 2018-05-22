#include <linux/of_gpio.h>
#include <linux/delay.h>
#include "wdt_log.h"

#define DRV_NAME "wdt-gpio-mstar"

/*
 * drivers/staging/webos/logger/wdt_detect_lg1k.c
 */

#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/of_gpio.h>
#include "wdt_log.h"

#define MOD_NAME "wdt-detect-mstar"

static int mstar_wdt_check(void)
{
#if 1// CCC fail for some boards
    int det = 8, opt = 43; //PM_OPT1
    int val;

    if(gpio_get_value(opt)) {

        if (gpio_direction_input(det) < 0) {
            return -1;
        }

        val = gpio_get_value(det);

    }else {
        val = readw((void *)0xFD001D48) & 0x8000 ? 1 : 0;
    }

    return val;
#else
    return 0;
#endif
}

static int mstar_wdt_detect_thread(void *data)
{
	int once_do = 0;
	int timeout;

	timeout = mstar_wdt_check();

	if (timeout > 0) {
		printk("Not supported WDT\n");
                goto EXIT;
	}

	while (1) {

		timeout = mstar_wdt_check();

		if (timeout < 0)
			goto EXIT;

		if (timeout && !once_do) {
			once_do = 1;
			wdt_log_save();
		}

		msleep(500);
	}

	while (1)
		;
EXIT:
	return 0;
}

static int __init mstar_init_wdt_detect(void)
{
	struct task_struct *log_task;

	if (!wdt_log_ready())
		return 0;

	log_task = kthread_run(mstar_wdt_detect_thread, NULL, "mstar_wdt_detect_thread");
	if (!IS_ERR(log_task)) {
		struct sched_param param = { .sched_priority = 99 };

		sched_setscheduler_nocheck(log_task, SCHED_FIFO,
							&param);
	}
	return 0;
}
late_initcall(mstar_init_wdt_detect);
