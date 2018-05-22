#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>

#include "mst_str.h"

//#define STR_PLATFORM_DRV
//#define STR_RESUME_END
#define STR_RESUME_KTHREAD

extern int (*mst_pm_begin_cb)(suspend_state_t state);
extern int (*mst_pm_prepare_cb)(void);
extern int (*mst_pm_prepare_late_cb)(void);
extern void (*mst_pm_wake_cb)(void);
extern void (*mst_pm_finish_cb)(void);
extern bool (*mst_pm_suspend_again_cb)(void);
extern void (*mst_pm_end_cb)(void);
extern void (*mst_pm_recover_cb)(void);

// init drivers
extern int init_tsp(void);
extern int init_vdec(void);
extern int init_hdmi(void);
// exit drivers
extern void exit_tsp(void);
extern void exit_vdec(void);
extern void exit_hdmi(void);
// suspend drivers
extern int suspend_sys(void);
extern int suspend_gpio(void);
extern int suspend_i2c(void);
extern int suspend_gic(void);
extern int suspend_vdec(void);
extern int suspend_mfe(void);
extern int suspend_sc(void);
extern int suspend_gop(void);
extern int suspend_gfx(void);
extern int suspend_tsp(void);
extern int suspend_pcmcia(void);
extern int suspend_dscmb(void);
extern int suspend_vd(void);
extern int suspend_audio(void);
extern int suspend_vbi(void);
extern int suspend_demod(void);
extern int suspend_demod_ext(void);
extern int suspend_ADC(void);   //hankw add

// resume drivers
extern int resume_sys(void);
extern int resume_gpio(void);
extern int resume_i2c(void);
extern int resume_hw_preinit(void);
extern int resume_gic(void);
extern int resume_vdec(void);
extern int resume_mfe(void);
extern int resume_sc(void);
extern int resume_gop(void);
extern int resume_gfx(void);
extern int resume_tsp(void);
extern int resume_pcmcia(void);
extern int resume_dscmb(void);
extern int resume_vd(void);
extern int resume_audio_part1(void);
extern int resume_audio_part2(void);
extern int resume_vbi(void);
extern int resume_demod(void);
extern int resume_demod_ext(void);
extern int resume_ADC(void);

#ifdef STR_PLATFORM_DRV
static struct platform_device *suspend_mst_dev;
#endif

#ifdef STR_RESUME_KTHREAD
#define STR_RESUME_KTHREAD_COMPLETE_1 0x01
#define STR_RESUME_KTHREAD_COMPLETE_2 0x02
#define STR_RESUME_KTHREAD_COMPLETE_3 0x04

extern unsigned int str_resume_kthread_complete;
extern void str_resume_kthread_wakeup(void);
#endif

#if 0
static int resume_platform_msleep(void)
{
    msleep(10);
    return 0;
}
#endif

static int (*suspend_mst_drivers_array[])(void) = {
    suspend_tsp,
    suspend_pcmcia,
    suspend_dscmb,
    suspend_vd,
    suspend_ADC,
    suspend_vdec,
    suspend_gop,
    suspend_gfx,
    suspend_sc,
    suspend_mfe,
    suspend_audio,
    suspend_vbi,
    suspend_demod_ext,
    suspend_demod,
    suspend_i2c,
    suspend_gpio, // should be last 2
    suspend_sys, // should be last 1
};

static int (*resume_mst_drivers_array[])(void) = {
    resume_sys, // should be first 1
    resume_gpio, // should be first 2
    resume_hw_preinit, // should be first 3
    resume_i2c,
};

static int (*resume_platform_mst_drivers_array_1[])(void) = {
    resume_audio_part1,
    resume_ADC,
    resume_vd,
    resume_sc,
    resume_gfx,
    resume_gop,
    resume_audio_part2,
};

static int (*resume_platform_mst_drivers_array_2[])(void) = {
    resume_demod_ext,
    resume_tsp,
    resume_dscmb,
    resume_vdec,
    resume_mfe,
    resume_vbi,
};

static int (*resume_platform_mst_drivers_array_3[])(void) = {
    resume_pcmcia,
    resume_demod,
};

#ifdef STR_RESUME_END
static int (*resume_mst_drivers_end_array[])(void) = {
};
#endif

static int __initdata (*init_mst_drivers_array[])(void) = {
//  init_tsp,
//  init_vdec,
    init_hdmi,
};

static void __exitdata (*exit_mst_drivers_array[])(void) = {
//  exit_tsp,
//  exit_vdec,
    exit_hdmi,
};


static int suspend_error;

static int resume_platform_mst_drivers_1(void *data)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs, usecs_total = 0;
#endif

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(resume_platform_mst_drivers_array_1); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*resume_platform_mst_drivers_array_1[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        usecs_total += usecs;
        printk("[part-2_1] resume time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }
#ifdef STR_MEASURE_TIME
    printk("\033[0;31m[time] resume drivers part-2_1 takes %ld.%03ldms \033[0m\n",
        usecs_total / USEC_PER_MSEC, usecs_total % USEC_PER_MSEC);
#endif
#ifdef STR_RESUME_KTHREAD
    str_resume_kthread_complete |= STR_RESUME_KTHREAD_COMPLETE_1;
    str_resume_kthread_wakeup();
#endif
    STR_PRINT("exit\n");
    return ret;
}

static int resume_platform_mst_drivers_2(void *data)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs, usecs_total = 0;
#endif

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(resume_platform_mst_drivers_array_2); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*resume_platform_mst_drivers_array_2[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        usecs_total += usecs;
        printk("[part-2_2] resume time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }
#ifdef STR_MEASURE_TIME
    printk("\033[0;31m[time] resume drivers part-2_2 takes %ld.%03ldms \033[0m\n",
        usecs_total / USEC_PER_MSEC, usecs_total % USEC_PER_MSEC);
#endif
#ifdef STR_RESUME_KTHREAD
    str_resume_kthread_complete |= STR_RESUME_KTHREAD_COMPLETE_2;
    str_resume_kthread_wakeup();
#endif
    STR_PRINT("exit\n");
    return ret;
}

static int resume_platform_mst_drivers_3(void *data)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs, usecs_total = 0;
#endif

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(resume_platform_mst_drivers_array_3); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*resume_platform_mst_drivers_array_3[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        usecs_total += usecs;
        printk("[part-2_3] resume time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }
#ifdef STR_MEASURE_TIME
    printk("\033[0;31m[time] resume drivers part-2_3 takes %ld.%03ldms \033[0m\n",
        usecs_total / USEC_PER_MSEC, usecs_total % USEC_PER_MSEC);
#endif
#ifdef STR_RESUME_KTHREAD
    str_resume_kthread_complete |= STR_RESUME_KTHREAD_COMPLETE_3;
    str_resume_kthread_wakeup();
#endif
    STR_PRINT("exit\n");
    return ret;
}

#ifdef STR_PLATFORM_DRV
static int suspend_mst_resume(struct platform_device *dev)
#else
static void suspend_mst_resume(void)
#endif
{
    int ret = 0;
#ifdef STR_RESUME_KTHREAD
    struct task_struct *kthread_1, *kthread_2, *kthread_3;
#endif

    printk("[%s] platform driver resume...\n", __func__);
    if (suspend_error)
#ifdef STR_PLATFORM_DRV
        return 0;
#else
        return;
#endif

    STR_PRINT("enter\n");
#ifdef STR_RESUME_KTHREAD
    kthread_1 = kthread_run(resume_platform_mst_drivers_1, NULL, "resume_platform_mst_drivers_1");
    if (IS_ERR(kthread_1)) {
        printk(KERN_ERR "Fail to create a kthread for resume_platform_mst_drivers_1 !!\n");
        ret |= PTR_ERR(kthread_1);
    }

    kthread_2 = kthread_run(resume_platform_mst_drivers_2, NULL, "resume_platform_mst_drivers_2");
    if (IS_ERR(kthread_2)) {
        printk(KERN_ERR "Fail to create a kthread for resume_platform_mst_drivers_2 !!\n");
        ret |= PTR_ERR(kthread_2);
    }

    kthread_3 = kthread_run(resume_platform_mst_drivers_3, NULL, "resume_platform_mst_drivers_3");
    if (IS_ERR(kthread_3)) {
        printk(KERN_ERR "Fail to create a kthread for resume_platform_mst_drivers_3 !!\n");
        ret |= PTR_ERR(kthread_3);
    }

#else
    ret |= resume_platform_mst_drivers_1(NULL);
    ret |= resume_platform_mst_drivers_2(NULL);
    ret |= resume_platform_mst_drivers_3(NULL);
#endif
    STR_PRINT("exit\n");

#ifdef STR_PLATFORM_DRV
    return ret;
#endif
}

static int suspend_mst_drivers(void)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs;
#endif
    printk("[%s] driver suspend...\n", __func__);

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(suspend_mst_drivers_array); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*suspend_mst_drivers_array[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        printk("suspend time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }

    suspend_error = ret;
    if (suspend_error)
        printk("[%s] suspend fail!\n", __func__);

#ifdef STR_RESUME_KTHREAD
    str_resume_kthread_complete = 0;
#endif
    STR_PRINT("exit\n");
    return ret;
}

static void resume_mst_drivers(void)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs, usecs_total = 0;
    unsigned int u32Timer_now, u32Timer_a, u32Timer_b;

    u32Timer_now = (*((volatile unsigned int *)0xFD006090)) | ((*((volatile unsigned int *)0xFD006094)) << 16);
    u32Timer_a = (*((volatile unsigned int *)0xFD002440)) | ((*((volatile unsigned int *)0xFD002444)) << 16);
    u32Timer_b = (*((volatile unsigned int *)0xFD002640)) | ((*((volatile unsigned int *)0xFD002640)) << 16);
    printk("\033[0;31m[time] kernel resume takes %d.%03dms \033[0m\n",
        (u32Timer_now - u32Timer_a + u32Timer_b)/12000, ((u32Timer_now - u32Timer_a + u32Timer_b)/12)%1000);
#endif

    printk("[%s] driver resume...\n", __func__);
    if (suspend_error)
        return;

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(resume_mst_drivers_array); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*resume_mst_drivers_array[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        usecs_total += usecs;
        printk("[part-1] resume time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }

#ifdef STR_MEASURE_TIME
    printk("\033[0;31m[time] resume drivers part-1 takes %ld.%03ldms \033[0m\n",
        usecs_total / USEC_PER_MSEC, usecs_total % USEC_PER_MSEC);
#endif

    STR_PRINT("exit\n");
}

#ifdef STR_RESUME_END
static void resume_mst_drivers_end(void)
{
    int ret = 0, i;
#ifdef STR_MEASURE_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs, usecs_total = 0;
#endif

    printk("[%s] driver resume end...\n", __func__);
    if (suspend_error)
        return;

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(resume_mst_drivers_end_array); i++) {
#ifdef STR_MEASURE_TIME
        starttime = ktime_get();
#endif
        ret = (*resume_mst_drivers_end_array[i])();
#ifdef STR_MEASURE_TIME
        endtime = ktime_get();
        usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
        do_div(usecs64, NSEC_PER_USEC);
        usecs = usecs64;
        usecs_total += usecs;
        printk("[part-3] resume time [%02d] : %ld.%03ldms\n\n", i,
            usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif
        if (ret)
            break;
    }

#ifdef STR_MEASURE_TIME
    printk("\033[0;31m[time] resume drivers part-3 takes %ld.%03ldms \033[0m\n",
        usecs_total / USEC_PER_MSEC, usecs_total % USEC_PER_MSEC);
#endif

    STR_PRINT("exit\n");
}
#endif //STR_RESUME_END

static int mst_pm_begin(suspend_state_t state)
{
    // use this function to check if the driver is ready or not, do not remove it!
    return 0;
}

#ifdef STR_PLATFORM_DRV
static struct platform_driver suspend_mst_drv = {
//  .suspend    = suspend_mst_suspend,
    .resume     = suspend_mst_resume,
    .driver = {
        .name   = "suspend_mst",
        .owner  = THIS_MODULE,
    }
};
#endif

static int __init init_mst_drivers(void)
{
    int ret = 0, i;

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(init_mst_drivers_array); i++) {
        ret = (*init_mst_drivers_array[i])();
        if (ret)
            break;
    }
    STR_PRINT("exit\n");
    return ret;
}

static void __exit exit_mst_drivers(void)
{
    int i;

    STR_PRINT("enter\n");
    for (i = 0; i < ARRAY_SIZE(exit_mst_drivers_array); i++) {
        (*exit_mst_drivers_array[i])();
    }
    STR_PRINT("exit\n");
}

extern int __init mst_str_init(void)
{
    int ret = 0;

    STR_PRINT("enter\n");

    ret = init_mst_drivers();
    if (ret) {
        exit_mst_drivers();
        ret = -ENOMEM;
        goto out;
    }

    mst_pm_begin_cb = mst_pm_begin;
    mst_pm_prepare_late_cb = suspend_mst_drivers;
#ifdef STR_PLATFORM_DRV
    mst_pm_finish_cb = resume_mst_drivers;
#else
    mst_pm_wake_cb = resume_mst_drivers;
    mst_pm_finish_cb = suspend_mst_resume;
#endif
#ifdef STR_RESUME_END
    mst_pm_end_cb = resume_mst_drivers_end;
#endif

#ifdef STR_PLATFORM_DRV
    suspend_mst_dev = platform_device_alloc("suspend_mst", -1);
    platform_device_add(suspend_mst_dev);
    ret = platform_driver_register(&suspend_mst_drv);
#endif

out:
    STR_PRINT("exit\n");
    return ret;
}

extern void __exit mst_str_exit(void)
{
    STR_PRINT("enter\n");

    exit_mst_drivers();

#ifdef STR_PLATFORM_DRV
    platform_driver_unregister(&suspend_mst_drv);
    platform_device_unregister(suspend_mst_dev);
#endif

    mst_pm_begin_cb = NULL;
    mst_pm_prepare_cb = NULL;
    mst_pm_prepare_late_cb = NULL;
    mst_pm_wake_cb = NULL;
    mst_pm_finish_cb = NULL;
    mst_pm_suspend_again_cb = NULL;
    mst_pm_end_cb = NULL;
    mst_pm_recover_cb = NULL;

    STR_PRINT("exit\n");
}

#if 0
module_init(mst_str_init);
module_exit(mst_str_exit);

MODULE_AUTHOR("MSTAR (cy.li@mstarsemi.com)");
MODULE_DESCRIPTION("Mstar STR Kdriver");
MODULE_LICENSE("GPL");
#endif

