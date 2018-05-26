#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/syscore_ops.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/suspend.h>
#include <linux/cpu.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/cpu_pm.h>

#include <asm/smp_scu.h>
#include <asm/memory.h>
#include <asm/suspend.h>
#include <asm/psci.h>
#include <asm/idmap.h>

#include <mach/lg1k.h>
#include "sram.h"
#include "pm.h"

extern void lg1k_smp_resume(void);

static void (*lg1k_pm_ddr_refresh_enter)(void);

static int lg115x_pm_suspend(void)
{
	return 0;
}

static void lg1210_pm_resume(void)
{
	struct device_node *np;
	struct gpio_desc *reset;
	void __iomem *base;

	/* UART mux setting */
	base = ioremap(0xC8601084,4);
	writel_relaxed(0x22508000, base);
	iounmap(base);

	/* H15: 50MHz PHY-interface clock */
	base = ioremap(0xc8aff014, 4);
	writel_relaxed(0x00003000, base);
	iounmap(base);

	/* H15: RMII PHY-interface */
	base = ioremap(0xfd3003d4, 4);
	writel_relaxed(0x00000001, base);
	iounmap(base);

	for_each_compatible_node(np, NULL, "realtek,rtl8201f") {
		reset = gpiod_get(NULL, "reset");
		if (IS_ERR(reset)) {
			pr_warn("%s lacks property reset-gpios\n", np->name);
			continue;
		}

		/* reset-gpio should be initialized as output/inactive */
		gpiod_direction_output(reset, 0);
		gpiod_put(reset);
	}
}

static struct syscore_ops lg1210_pm_syscore_ops = {
	.resume         = lg1210_pm_resume,
};

static void lg115x_pm_resume(void)
{
	lg1k_smp_resume();
}

static struct syscore_ops lg115x_pm_syscore_ops = {
	.suspend        = lg115x_pm_suspend,
	.resume         = lg115x_pm_resume,
};

/*
 *      lg1k_pm_prepare - Do preliminary suspend work.
 *
 */
static int lg1k_pm_prepare(void)
{
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "lg1210-ddr-ctrl");
	if (np) {
		lg1k_pm_ddr_refresh_enter = (void *)lg1k_sram_phys(
			(void *)lg1k_sram_push(_lg1210_pm_ddr_refresh_enter,
				(ulong)_end_lg1210_pm_ddr_refresh_enter
				- (ulong)_lg1210_pm_ddr_refresh_enter));
	}

	return 0;
}

static int lg1k_cpu_suspend_finish(unsigned long arg)
{
	void (*phys_reset)(unsigned long);

	const struct psci_power_state ps = {
		.type = PSCI_POWER_STATE_TYPE_POWER_DOWN,
		.affinity_level = 1,
	};
	if (psci_ops.cpu_suspend)
		psci_ops.cpu_suspend(ps, __pa(cpu_resume));

	if (!lg1k_pm_ddr_refresh_enter)
		return 0;

	/* Take out a flat memory mapping. */
	setup_mm_for_reboot();

	/* Clean and invalidate caches */
	flush_cache_all();

	/* Switch to the identity mapping. */
	phys_reset = (void *)(unsigned long)__pa(cpu_reset);
	phys_reset((unsigned long)lg1k_pm_ddr_refresh_enter);

	/* never get here */
	BUG();
	return 0;
}

/*
 *      lg1k_pm_enter - Actually enter a sleep state.
 *      @state:         State we're entering.
 *
 */
static int lg1k_pm_enter(suspend_state_t state)
{
	int ret;

	cpu_pm_enter();
	cpu_cluster_pm_enter();

	ret = cpu_suspend(0, lg1k_cpu_suspend_finish);

	cpu_cluster_pm_exit();
	cpu_pm_exit();

	return ret;
}

/**
 *      lg1k_pm_finish - Finish up suspend sequence.
 *
 *      This is called after we wake back up (or if entering the sleep state
 *      failed).
 */
static void lg1k_pm_finish(void)
{
	lg1k_sram_reset();
}

static const struct platform_suspend_ops lg1k_pm_ops = {
	.prepare	= lg1k_pm_prepare,
	.enter		= lg1k_pm_enter,
	.finish		= lg1k_pm_finish,
	.valid		= suspend_valid_only_mem,
};


static __init int lg115x_pm_syscore_init(void)
{
	register_syscore_ops(&lg115x_pm_syscore_ops);
	if (of_machine_is_compatible("lge,lg1210"))
		register_syscore_ops(&lg1210_pm_syscore_ops);

	suspend_set_ops(&lg1k_pm_ops);
	return 0;
}
arch_initcall(lg115x_pm_syscore_init);
