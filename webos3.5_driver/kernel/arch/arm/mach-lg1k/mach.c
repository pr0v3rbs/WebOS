#include <linux/kernel.h>

#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/clocksource.h>
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/irqdomain.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>
#include <linux/memblock.h>
#include <linux/amba/pl08x.h>
#include <linux/amba/pl080.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/pgtable.h>

#include <mach/ion.h>

#include <dt-bindings/interrupt-controller/arm-gic.h>

extern bool __init lg1k_smp_init_ops(void);
extern struct amba_pl011_data uart1_data;
extern struct pl022_ssp_controller spi0_data;
extern struct pl022_ssp_controller spi1_data;
extern struct pl08x_platform_data dmac_data;

static void __init lg115x_reserve(void)
{
	extern void __init lg115x_reserve_eth(void);
	extern void __init lg1k_reserve_smp(void);
	extern void __init lg1k_reserve_cma(void);

	/* reserved for special purpose, (trustzone etc.) */
	memblock_reserve(0x0, SZ_4K);

	lg115x_reserve_eth();
	lg1k_reserve_smp();
	lg1k_reserve_cma();
}

#define	MAP_DESC_MAX	16

static struct map_desc map_desc[MAP_DESC_MAX] __initdata;
static size_t map_desc_size __initdata;

static unsigned long map_addr __initdata = VMALLOC_END;

static int __init build_map_desc(unsigned long node, char const *uname,
                                 int depth, void *data)
{
	unsigned long phys, size, virt;
	__be32 const *prop;

	if (!of_get_flat_dt_prop(node, "static-map", NULL))
		return 0;

	prop = of_get_flat_dt_prop(node, "reg", NULL);
	phys = of_read_number(prop++, 1);
	size = of_read_number(prop, 1);

	if (!of_get_flat_dt_prop(node, "static-map-virt", NULL)) {
		map_addr -= size;
		map_addr &= ~(size - 1);
		virt = map_addr;
	} else {
		virt = phys;
	}

	map_desc[map_desc_size].virtual	= virt;
	map_desc[map_desc_size].pfn	= __phys_to_pfn(phys);
	map_desc[map_desc_size].length	= size;
	map_desc[map_desc_size++].type	= MT_DEVICE;

	return 0;
}

static void __init lg115x_map_io(void)
{
	of_scan_flat_dt(build_map_desc, NULL);
	iotable_init(map_desc, map_desc_size);
}

static void __init lg115x_init_early(void)
{
	extern void __init lg115x_arm_firmware_init(void);
	extern void __init lg115x_init_l2cc(void);

	lg115x_arm_firmware_init();
	lg115x_init_l2cc();
}

static void __init lg115x_init_time(void)
{
	of_clk_init(NULL);
	clocksource_probe();
}

static void __init lg115x_init_irq(void)
{
	struct device_node *dn;
	struct irq_domain *host;
	struct irq_fwspec fwspec;
	int rc;

	irqchip_init();

	dn = of_find_node_with_property(NULL, "interrupt-controller");
	if (!dn) {
		pr_err("of_find_node_with_property failed\n");
		return;
	}

	host = irq_find_host(dn);
	if (!host) {
		pr_err("irq_find_host failed\n");
		return;
	}

	/*
	 * Pre-create identical(hwirq == virq) IRQ mappings for all
	 * possible SPIs to support IRQs to out-of-DT devices
	 */
	rc = irq_create_strict_mappings(host, 32, 32, host->hwirq_max - 32);
	if (rc < 0) {
		pr_err("irq_create_strict_mappings failed: %d\n", rc);
		return;
	}

	fwspec.fwnode = host->fwnode;
	fwspec.param_count = 3;
	fwspec.param[0] = GIC_SPI;
	fwspec.param[1] = 0;
	fwspec.param[2] = IRQ_TYPE_LEVEL_HIGH;

	rc = __irq_domain_alloc_irqs(host, 32, host->hwirq_max - 32,
	                             NUMA_NO_NODE, &fwspec, true);
	if (rc < 0)
		pr_err("__irq_domain_alloc_irqs failed: %d\n", rc);
}

static struct of_dev_auxdata lg115x_auxdata_lookup[] __initdata = {
	OF_DEV_AUXDATA("arm,pl011", 0xfe100000, "uart1", NULL),
	OF_DEV_AUXDATA("arm,pl022", 0xfe800000, "spi0", &spi0_data),
	OF_DEV_AUXDATA("arm,pl022", 0xfe900000, "spi1", &spi1_data),
	OF_DEV_AUXDATA("arm,pl080", 0xff200000, "dmac", &dmac_data),
	{ },
};

static void __init lg115x_init_machine(void)
{
	extern void __init lg1k_of_init_sram(void);

	if (of_machine_is_compatible("lge,lg1154"))
		lg115x_auxdata_lookup[0].platform_data = &uart1_data;

	of_platform_populate(NULL, of_default_bus_match_table,
				lg115x_auxdata_lookup, NULL);

	if (!of_machine_is_compatible("lge,lg1156"))
		lg115x_register_ion();

	lg1k_of_init_sram();
}

static void __init lg115x_init_late(void)
{
	struct platform_device_info cpufreq_info = { .name = "cpufreq-dt", };
	extern void __init lg115x_init_spi(void);

	platform_device_register_full(&cpufreq_info);
	lg115x_init_spi();
}

static void lg115x_restart(enum reboot_mode mode, char const *cmd)
{
	struct device_node *np;
	void __iomem *base;

	if (!(np = of_find_compatible_node(NULL, NULL, "arm,sp805"))) {
		printk("%s: failed to find SP805 device\n", __func__);
		return;
	}
	if (!(base = of_iomap(np, 0))) {
		printk("%s: failed to find SP805 address\n", __func__);
		return;
	}

	writel_relaxed(0x00, base + 0x0008);
	writel_relaxed(0x00, base + 0x0000);
	writel_relaxed(0x03, base + 0x0008);
}

static char const *const lg115x_dt_compat[] __initconst = {
	"lge,lg1156",
	"lge,lg1154",
	"lge,lg1311",
	"lge,lg1210",
	"lge,lg1312",
	"lge,lg1313",
	NULL
};

DT_MACHINE_START(LG115X, "LG Electronics DTV SoC")
	.atag_offset	= 0x0100,
	.dt_compat	= lg115x_dt_compat,
	.smp_init       = smp_init_ops(lg1k_smp_init_ops),
	.reserve	= lg115x_reserve,
	.map_io		= lg115x_map_io,
	.init_early	= lg115x_init_early,
	.init_irq	= lg115x_init_irq,
	.init_time	= lg115x_init_time,
	.init_machine	= lg115x_init_machine,
	.init_late	= lg115x_init_late,
	.restart	= lg115x_restart,
MACHINE_END
