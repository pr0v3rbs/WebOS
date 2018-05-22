/*
 * kernel/power/suspend.c - Suspend to RAM and standby functionality.
 *
 * Copyright (c) 2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
 * Copyright (c) 2009 Rafael J. Wysocki <rjw@sisk.pl>, Novell Inc.
 *
 * This file is released under the GPLv2.
 */

#include <linux/string.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/cpu.h>
#include <linux/cpuidle.h>
#include <linux/syscalls.h>
#include <linux/gfp.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/ftrace.h>
#include <trace/events/power.h>
#include <linux/compiler.h>
#include <linux/moduleparam.h>
#define SECURER2_STR 1

#if SECURER2_STR
#include "power.h"

#include "../../drivers/mstar2/include/mdrv_types.h"
#include "../../drivers/mstar2/include/mdrv_mstypes.h"
#include "../../drivers/mstar2/drv/mbx/mdrv_mbx.h"
#include "../../drivers/mstar2/drv/mbx/mapi_mbx.h"
#endif //SECURER2_STR

const char *pm_labels[] = { "mem", "standby", "freeze", NULL };
const char *pm_states[PM_SUSPEND_MAX];

unsigned int pm_suspend_global_flags;
EXPORT_SYMBOL_GPL(pm_suspend_global_flags);

static const struct platform_suspend_ops *suspend_ops;
static const struct platform_freeze_ops *freeze_ops;
static DECLARE_WAIT_QUEUE_HEAD(suspend_freeze_wait_head);

enum freeze_state __read_mostly suspend_freeze_state;
static DEFINE_SPINLOCK(suspend_freeze_lock);

#if defined(CONFIG_STR_RESUME_KTHREAD)
#define STR_RESUME_KTHREAD_COMPLETE_1 0x01
#define STR_RESUME_KTHREAD_COMPLETE_2 0x02
#define STR_RESUME_KTHREAD_COMPLETE_3 0x04

#define STR_RESUME_KTHREAD_TIMEOUT (5 * HZ)

static DECLARE_WAIT_QUEUE_HEAD(str_resume_kthread_waitq);
unsigned int str_resume_kthread_complete = 0;
EXPORT_SYMBOL(str_resume_kthread_complete);

void str_resume_kthread_wakeup(void)
{
    wake_up(&str_resume_kthread_waitq);
}
EXPORT_SYMBOL(str_resume_kthread_wakeup);
#endif /* CONFIG_STR_RESUME_KTHREAD */

#if defined(CONFIG_MP_USB_STR_PATCH)
typedef enum
{
    E_STR_NONE,
    E_STR_IN_SUSPEND,
    E_STR_IN_RESUME
}EN_STR_STATUS;

static EN_STR_STATUS enStrStatus=E_STR_NONE;

bool is_suspending(void)
{
    return (enStrStatus == E_STR_IN_SUSPEND);
}
EXPORT_SYMBOL_GPL(is_suspending);
#endif /* CONFIG_MP_USB_STR_PATCH */

void freeze_set_ops(const struct platform_freeze_ops *ops)
{
    lock_system_sleep();
    freeze_ops = ops;
    unlock_system_sleep();
}

static void freeze_begin(void)
{
    suspend_freeze_state = FREEZE_STATE_NONE;
}

static void freeze_enter(void)
{
    spin_lock_irq(&suspend_freeze_lock);
    if (pm_wakeup_pending())
        goto out;

    suspend_freeze_state = FREEZE_STATE_ENTER;
    spin_unlock_irq(&suspend_freeze_lock);

    get_online_cpus();
    cpuidle_resume();

    /* Push all the CPUs into the idle loop. */
    wake_up_all_idle_cpus();
    pr_debug("PM: suspend-to-idle\n");
    /* Make the current CPU wait so it can enter the idle loop too. */
    wait_event(suspend_freeze_wait_head,
           suspend_freeze_state == FREEZE_STATE_WAKE);
    pr_debug("PM: resume from suspend-to-idle\n");

    cpuidle_pause();
    put_online_cpus();

    spin_lock_irq(&suspend_freeze_lock);

 out:
    suspend_freeze_state = FREEZE_STATE_NONE;
    spin_unlock_irq(&suspend_freeze_lock);
}

void freeze_wake(void)
{
    unsigned long flags;

    spin_lock_irqsave(&suspend_freeze_lock, flags);
    if (suspend_freeze_state > FREEZE_STATE_NONE) {
        suspend_freeze_state = FREEZE_STATE_WAKE;
        wake_up(&suspend_freeze_wait_head);
    }
    spin_unlock_irqrestore(&suspend_freeze_lock, flags);
}
EXPORT_SYMBOL_GPL(freeze_wake);

static bool valid_state(suspend_state_t state)
{
    /*
     * PM_SUSPEND_STANDBY and PM_SUSPEND_MEM states need low level
     * support and need to be valid to the low level
     * implementation, no valid callback implies that none are valid.
     */
    return suspend_ops && suspend_ops->valid && suspend_ops->valid(state);
}

/*
 * If this is set, the "mem" label always corresponds to the deepest sleep state
 * available, the "standby" label corresponds to the second deepest sleep state
 * available (if any), and the "freeze" label corresponds to the remaining
 * available sleep state (if there is one).
 */
static bool relative_states;

static int __init sleep_states_setup(char *str)
{
    relative_states = !strncmp(str, "1", 1);
    pm_states[PM_SUSPEND_FREEZE] = pm_labels[relative_states ? 0 : 2];
    return 1;
}

__setup("relative_sleep_states=", sleep_states_setup);

/**
 * suspend_set_ops - Set the global suspend method table.
 * @ops: Suspend operations to use.
 */
void suspend_set_ops(const struct platform_suspend_ops *ops)
{
    suspend_state_t i;
    int j = 0;

    lock_system_sleep();

    suspend_ops = ops;
    for (i = PM_SUSPEND_MEM; i >= PM_SUSPEND_STANDBY; i--)
        if (valid_state(i)) {
            pm_states[i] = pm_labels[j++];
        } else if (!relative_states) {
            pm_states[i] = NULL;
            j++;
        }

    pm_states[PM_SUSPEND_FREEZE] = pm_labels[j];

    unlock_system_sleep();
}
EXPORT_SYMBOL_GPL(suspend_set_ops);

/**
 * suspend_valid_only_mem - Generic memory-only valid callback.
 *
 * Platform drivers that implement mem suspend only and only need to check for
 * that in their .valid() callback can use this instead of rolling their own
 * .valid() callback.
 */
int suspend_valid_only_mem(suspend_state_t state)
{
    return state == PM_SUSPEND_MEM;
}
EXPORT_SYMBOL_GPL(suspend_valid_only_mem);

static bool sleep_state_supported(suspend_state_t state)
{
    return state == PM_SUSPEND_FREEZE || (suspend_ops && suspend_ops->enter);
}

static int platform_suspend_prepare(suspend_state_t state)
{
    return state != PM_SUSPEND_FREEZE && suspend_ops->prepare ?
        suspend_ops->prepare() : 0;
}

static int platform_suspend_prepare_late(suspend_state_t state)
{
    return state == PM_SUSPEND_FREEZE && freeze_ops && freeze_ops->prepare ?
        freeze_ops->prepare() : 0;
}

static int platform_suspend_prepare_noirq(suspend_state_t state)
{
    return state != PM_SUSPEND_FREEZE && suspend_ops->prepare_late ?
        suspend_ops->prepare_late() : 0;
}

static void platform_resume_noirq(suspend_state_t state)
{
    if (state != PM_SUSPEND_FREEZE && suspend_ops->wake)
        suspend_ops->wake();
}

static void platform_resume_early(suspend_state_t state)
{
    if (state == PM_SUSPEND_FREEZE && freeze_ops && freeze_ops->restore)
        freeze_ops->restore();
}

static void platform_resume_finish(suspend_state_t state)
{
    if (state != PM_SUSPEND_FREEZE && suspend_ops->finish)
        suspend_ops->finish();
}

static int platform_suspend_begin(suspend_state_t state)
{
    if (state == PM_SUSPEND_FREEZE && freeze_ops && freeze_ops->begin)
        return freeze_ops->begin();
    else if (suspend_ops->begin)
        return suspend_ops->begin(state);
    else
        return 0;
}

static void platform_resume_end(suspend_state_t state)
{
    if (state == PM_SUSPEND_FREEZE && freeze_ops && freeze_ops->end)
        freeze_ops->end();
    else if (suspend_ops->end)
        suspend_ops->end();
}

static void platform_recover(suspend_state_t state)
{
    if (state != PM_SUSPEND_FREEZE && suspend_ops->recover)
        suspend_ops->recover();
}

static bool platform_suspend_again(suspend_state_t state)
{
    return state != PM_SUSPEND_FREEZE && suspend_ops->suspend_again ?
        suspend_ops->suspend_again() : false;
}

#ifdef CONFIG_PM_DEBUG
static unsigned int pm_test_delay = 5;
module_param(pm_test_delay, uint, 0644);
MODULE_PARM_DESC(pm_test_delay,
         "Number of seconds to wait before resuming from suspend test");
#endif

static int suspend_test(int level)
{
#ifdef CONFIG_PM_DEBUG
    if (pm_test_level == level) {
        printk(KERN_INFO "suspend debug: Waiting for %d second(s).\n",
                pm_test_delay);
        mdelay(pm_test_delay * 1000);
        return 1;
    }
#endif /* !CONFIG_PM_DEBUG */
    return 0;
}

/**
 * suspend_prepare - Prepare for entering system sleep state.
 *
 * Common code run for every system sleep state that can be entered (except for
 * hibernation).  Run suspend notifiers, allocate the "suspend" console and
 * freeze processes.
 */
static int suspend_prepare(suspend_state_t state)
{
    int error;

    if (!sleep_state_supported(state))
        return -EPERM;

    pm_prepare_console();

    error = pm_notifier_call_chain(PM_SUSPEND_PREPARE);
    if (error)
        goto Finish;

    trace_suspend_resume(TPS("freeze_processes"), 0, true);
    error = suspend_freeze_processes();
    trace_suspend_resume(TPS("freeze_processes"), 0, false);
    if (!error)
        return 0;

    suspend_stats.failed_freeze++;
    dpm_save_failed_step(SUSPEND_FREEZE);
 Finish:
    pm_notifier_call_chain(PM_POST_SUSPEND);
    pm_restore_console();
    return error;
}

/* default implementation */
void __weak arch_suspend_disable_irqs(void)
{
    local_irq_disable();
}

/* default implementation */
void __weak arch_suspend_enable_irqs(void)
{
    local_irq_enable();
}

extern bool console_suspend_lately_disabled;

/**
 * suspend_enter - Make the system enter the given sleep state.
 * @state: System sleep state to enter.
 * @wakeup: Returns information that the sleep state should not be re-entered.
 *
 * This function should be called after devices have been suspended.
 */
static int suspend_enter(suspend_state_t state, bool *wakeup)
{
    int error;

    error = platform_suspend_prepare(state);
    if (error)
        goto Platform_finish;

    error = dpm_suspend_late(PMSG_SUSPEND);
    if (error) {
        printk(KERN_ERR "PM: late suspend of devices failed\n");
        goto Platform_finish;
    }
    error = platform_suspend_prepare_late(state);
    if (error)
        goto Devices_early_resume;

    error = dpm_suspend_noirq(PMSG_SUSPEND);
    if (error) {
        printk(KERN_ERR "PM: noirq suspend of devices failed\n");
        goto Platform_early_resume;
    }
    error = platform_suspend_prepare_noirq(state);
    if (error)
        goto Platform_wake;

    if (suspend_test(TEST_PLATFORM))
        goto Platform_wake;

    /*
     * PM_SUSPEND_FREEZE equals
     * frozen processes + suspended devices + idle processors.
     * Thus we should invoke freeze_enter() soon after
     * all the devices are suspended.
     */
    if (state == PM_SUSPEND_FREEZE) {
        trace_suspend_resume(TPS("machine_suspend"), state, true);
        freeze_enter();
        trace_suspend_resume(TPS("machine_suspend"), state, false);
        goto Platform_wake;
    }

    if (!console_suspend_lately_disabled)
        suspend_console();

    error = disable_nonboot_cpus();
    if (error || suspend_test(TEST_CPUS))
        goto Enable_cpus;

    arch_suspend_disable_irqs();
    BUG_ON(!irqs_disabled());

    error = syscore_suspend();
    if (!error) {
        *wakeup = pm_wakeup_pending();
        if (!(suspend_test(TEST_CORE) || *wakeup)) {
            trace_suspend_resume(TPS("machine_suspend"),
                state, true);
            error = suspend_ops->enter(state);
            trace_suspend_resume(TPS("machine_suspend"),
                state, false);
            events_check_enabled = false;
#if defined(CONFIG_MP_MSTAR_STR_BASE)
            set_state_value(STENT_RESUME_FROM_SUSPEND);
#endif /* CONFIG_MP_MSTAR_STR_BASE */
        } else if (*wakeup) {
            error = -EBUSY;
        }
#if defined(CONFIG_MP_USB_STR_PATCH)
        enStrStatus=E_STR_IN_RESUME;
#endif /* CONFIG_MP_USB_STR_PATCH */
        syscore_resume();
    }

    arch_suspend_enable_irqs();
    BUG_ON(irqs_disabled());

 Enable_cpus:
    enable_nonboot_cpus();

 Platform_wake:
    platform_resume_noirq(state);
    dpm_resume_noirq(PMSG_RESUME);

 Platform_early_resume:
    platform_resume_early(state);

 Devices_early_resume:
    dpm_resume_early(PMSG_RESUME);

 Platform_finish:
    platform_resume_finish(state);
    return error;
}

/**
 * suspend_devices_and_enter - Suspend devices and enter system sleep state.
 * @state: System sleep state to enter.
 */
int suspend_devices_and_enter(suspend_state_t state)
{
    int error;
    bool wakeup = false;

    if (!sleep_state_supported(state))
        return -ENOSYS;

    error = platform_suspend_begin(state);
    if (error)
        goto Close;

    if (console_suspend_lately_disabled)
        suspend_console();
    suspend_test_start();
    error = dpm_suspend_start(PMSG_SUSPEND);
    if (error) {
        pr_err("PM: Some devices failed to suspend, or early wake event detected\n");
        goto Recover_platform;
    }
    suspend_test_finish("suspend devices");
    if (suspend_test(TEST_DEVICES))
        goto Recover_platform;

    do {
        error = suspend_enter(state, &wakeup);
    } while (!error && !wakeup && platform_suspend_again(state));

 Resume_devices:
    if (!console_suspend_lately_disabled)
        resume_console();
    suspend_test_start();
    dpm_resume_end(PMSG_RESUME);
    suspend_test_finish("resume devices");
    trace_suspend_resume(TPS("resume_console"), state, true);
    if (console_suspend_lately_disabled)
        resume_console();
    trace_suspend_resume(TPS("resume_console"), state, false);

 Close:
    platform_resume_end(state);
    return error;

 Recover_platform:
    platform_recover(state);
    goto Resume_devices;
}

/**
 * suspend_finish - Clean up before finishing the suspend sequence.
 *
 * Call platform code to clean up, restart processes, and free the console that
 * we've allocated. This routine is not called for hibernation.
 */
static void suspend_finish(void)
{
    suspend_thaw_processes();
    pm_notifier_call_chain(PM_POST_SUSPEND);
    pm_restore_console();
}

/**
 * enter_state - Do common work needed to enter system sleep state.
 * @state: System sleep state to enter.
 *
 * Make sure that no one else is trying to put the system into a sleep state.
 * Fail if that's not the case.  Otherwise, prepare for system suspend, make the
 * system enter the given sleep state and clean up after wakeup.
 */
u32 kernel_read_phys(u32 phys_addr)
{
    u32 phys_addr_page = phys_addr & 0xFFFFE000;
    u32 phys_offset    = phys_addr & 0x00001FFF;
    u32 map_size       = phys_offset + sizeof(u32);
    u32 ret = 0xDEADBEEF;
    void *mem_mapped = ioremap_nocache(phys_addr_page, map_size);
    if (NULL != mem_mapped) {
        ret = (u32)ioread32(((u8*)mem_mapped) + phys_offset);
        iounmap(mem_mapped);
    }

    return ret;
}

#include <asm/setup.h>
#define MMAP_INDEX_SYS_MMAP_AES          0x5
#define MMAP_INDEX_MEMADDRESS            0x1
#if SECURER2_STR
    //1. Setup Suspend Flag to 0
#define MIU0_BASE               0x20000000
#define TEESTORESIZE            0X100000
#define NUTTX_BOOT_CONFIG_SIZE  0x1000
#define NFDRM_HW_AES_BUF_ADR    customTAG.mmapger[MMAP_INDEX_SYS_MMAP_AES][MMAP_INDEX_MEMADDRESS] //0x001F700000
#define STR_FLAG_SUSPEND_FINISH 0xFFFF8888
#endif /* SECURER2_STR */

extern struct tag_custom customTAG;
static int enter_state(suspend_state_t state)
{
    int error;
#if defined(CONFIG_MP_MSTAR_STR_BASE)
    int bresumefromsuspend=0;
#endif /* CONFIG_MP_MSTAR_STR_BASE */
#if SECURER2_STR
//    U32 u32TEEStoreSize = 0x100000;
    U32* u32TEESTRBOOTFLAG = NULL;
    U32 u32BootConfigSize = NUTTX_BOOT_CONFIG_SIZE;
    U32 u32PrevFlag = 0,u32CurrentFlag = 0;;
    u32TEESTRBOOTFLAG = (U32*)(NFDRM_HW_AES_BUF_ADR + MIU0_BASE - TEESTORESIZE - u32BootConfigSize);
#endif /* SECURER2_STR */

    trace_suspend_resume(TPS("suspend_enter"), state, true);
    if (state == PM_SUSPEND_FREEZE) {
#ifdef CONFIG_PM_DEBUG
        if (pm_test_level != TEST_NONE && pm_test_level <= TEST_CPUS) {
            pr_warning("PM: Unsupported test mode for suspend to idle,"
                   "please choose none/freezer/devices/platform.\n");
            return -EAGAIN;
        }
#endif
    } else if (!valid_state(state)) {
        return -EINVAL;
    }
    if (!mutex_trylock(&pm_mutex))
        return -EBUSY;

    printk(KERN_INFO "PM: Send MBX to TEE for STR_Suspend  ... \n");
#if SECURER2_STR
    //1. Setup Suspend Flag to 0
    printk(KERN_INFO "PM: NFDRM_HW_AES_BUF_ADR => Addr = 0x%x  (should be 0x001F700000) !!!!\n", NFDRM_HW_AES_BUF_ADR );
    printk(KERN_INFO "PM: u32TEESTRBOOTFLAG => Addr = 0x%p  !!!!\n", u32TEESTRBOOTFLAG );
    printk(KERN_INFO "PM: u32TEESTRBOOTFLAG => Value = 0x%x !!!! \n",kernel_read_phys((U32)u32TEESTRBOOTFLAG));

    //2. Send Mailbox to TEE
    MApi_MBX_NotifyTeetoSuspend();
#endif

#if SECURER2_STR
    //3. Waiting TEE to finish susepnd jobs
    do
    {
        u32CurrentFlag = kernel_read_phys((U32)u32TEESTRBOOTFLAG);
        if (u32PrevFlag != u32CurrentFlag )
        {
            u32PrevFlag =  u32CurrentFlag;
            printk(KERN_INFO "PM: Waiting TEE suspend done signal!!! 0x%x \n", u32PrevFlag);
        }
        mdelay(1);
    }while( u32CurrentFlag != STR_FLAG_SUSPEND_FINISH );
#endif
#if defined(CONFIG_MP_MSTAR_STR_BASE)
    set_state_entering();
#if defined(CONFIG_MP_USB_STR_PATCH)
    enStrStatus=E_STR_IN_SUSPEND;
#endif /* CONFIG_MP_USB_STR_PATCH */
#endif /* CONFIG_MP_MSTAR_STR_BASE */

#if defined(CONFIG_MP_MSTAR_STR_BASE)
try_again:
#endif /* CONFIG_MP_MSTAR_STR_BASE */
    if (state == PM_SUSPEND_FREEZE)
        freeze_begin();

#ifndef CONFIG_SUSPEND_SKIP_SYNC
    trace_suspend_resume(TPS("sync_filesystems"), 0, true);
    printk(KERN_INFO "PM: Syncing filesystems ... ");
    sys_sync();
    printk("done.\n");
    trace_suspend_resume(TPS("sync_filesystems"), 0, false);
#endif

    pr_debug("PM: Preparing system for sleep (%s)\n", pm_states[state]);
    pm_suspend_clear_flags();
    error = suspend_prepare(state);
    if (error)
        goto Unlock;

    if (suspend_test(TEST_FREEZER))
        goto Finish;

    trace_suspend_resume(TPS("suspend_enter"), state, false);
    pr_debug("PM: Suspending system (%s)\n", pm_states[state]);
    pm_restrict_gfp_mask();
    error = suspend_devices_and_enter(state);
    pm_restore_gfp_mask();

 Finish:
    pr_debug("PM: Finishing wakeup.\n");
#if defined(CONFIG_MP_MSTAR_STR_BASE)
    if(STENT_RESUME_FROM_SUSPEND == get_state_value()){
        clear_state_entering();
        bresumefromsuspend=1;
    }
#endif /* CONFIG_MP_MSTAR_STR_BASE */

#if defined(CONFIG_STR_RESUME_KTHREAD)
{
#ifdef CONFIG_MSTAR_STR_TIME
    ktime_t starttime, endtime;
    u64 usecs64;
    int usecs;
#endif /* CONFIG_MSTAR_STR_TIME */
    long ret;
    printk("wait for STR resume kthread complete ...\n");
#ifdef CONFIG_MSTAR_STR_TIME
    starttime = ktime_get();
#endif /* CONFIG_MSTAR_STR_TIME */
    ret = wait_event_timeout(str_resume_kthread_waitq,
        (STR_RESUME_KTHREAD_COMPLETE_1 | STR_RESUME_KTHREAD_COMPLETE_2 | STR_RESUME_KTHREAD_COMPLETE_3) == str_resume_kthread_complete,
        STR_RESUME_KTHREAD_TIMEOUT);
#ifdef CONFIG_MSTAR_STR_TIME
    endtime = ktime_get();
    usecs64 = ktime_to_ns(ktime_sub(endtime, starttime));
    do_div(usecs64, NSEC_PER_USEC);
    usecs = usecs64;
    printk("\033[0;31m[time] resume kthread wait time takes %ld.%03ldms \033[0m\n",
        usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
#endif /* CONFIG_MSTAR_STR_TIME */
    printk("resume kthread finish!\n");
//  printk("cyli %s ret = %ld, str_resume_kthread_complete=0x%x\n", __func__, ret, str_resume_kthread_complete);
    if (!ret)
        printk(KERN_ERR "%s(): Timeout!! str_resume_kthread_complete=0x%x\n", __func__, str_resume_kthread_complete);
}
#endif /* STR_RESUME_KTHREAD */

    suspend_finish();
 Unlock:
#if defined(CONFIG_MP_MSTAR_STR_BASE)
#if defined(CONFIG_MSTAR_STR_ACOFF_ON_ERR)
    if(error)
    {
        extern void mstar_str_notifypmerror_off(void);
        mstar_str_notifypmerror_off(); //it won't return, wait pm to power off
    }
#endif /* CONFIG_MSTAR_STR_ACOFF_ON_ERR */

    if(is_mstar_str() && bresumefromsuspend==0){
        schedule_timeout_interruptible(HZ);
        goto try_again;
    }
#if defined(CONFIG_MP_USB_STR_PATCH)
    enStrStatus=E_STR_NONE;
#endif /* CONFIG_MP_USB_STR_PATCH */
#endif /* CONFIG_MP_MSTAR_STR_BASE */
#if defined(CONFIG_CACHE_PL310) && defined(CONFIG_MP_PLATFORM_PM)
{
    extern void Check_PL310_Enable(void);
    Check_PL310_Enable();
}
#endif /* CONFIG_CACHE_PL310 && CONFIG_MP_PLATFORM_PM */
    mutex_unlock(&pm_mutex);
    return error;
}

/**
 * pm_suspend - Externally visible function for suspending the system.
 * @state: System sleep state to enter.
 *
 * Check if the value of @state represents one of the supported states,
 * execute enter_state() and update system suspend statistics.
 */
int pm_suspend(suspend_state_t state)
{
    int error;

    if (state <= PM_SUSPEND_ON || state >= PM_SUSPEND_MAX)
        return -EINVAL;

    error = enter_state(state);
    if (error) {
        suspend_stats.fail++;
        dpm_save_failed_errno(error);
    } else {
        suspend_stats.success++;
    }
    return error;
}
EXPORT_SYMBOL(pm_suspend);
