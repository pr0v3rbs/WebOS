#include <linux/kernel.h>

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <asm/memblock.h>

static phys_addr_t cmem;

static int __init
dt_scan_compat(unsigned long node, char const *uname, int depth, void *data)
{
	return of_flat_dt_is_compatible(node, data);
}

void __init lg115x_reserve_eth(void)
{
	if (of_scan_flat_dt(dt_scan_compat, "arasan,emac")) {
		cmem = arm_memblock_steal(SZ_1M, SZ_1M);
	}
}

/* Currently, this function assumes the platform has only single MDIO-bus. */
static int rtl8201f_fixup_run(struct phy_device *phydev)
{
	struct device_node *np;
	struct gpio_desc *reset;
	u32 addr;

	for_each_compatible_node(np, NULL, "realtek,rtl8201f") {
		if (of_property_read_u32(np, "reg", &addr) < 0) {
			pr_warn("%s lacks property reg\n", np->name);
			continue;
		} else if (addr != phydev->addr) {
			continue;
		}

		reset = gpiod_get(&phydev->dev, "reset");
		if (IS_ERR(reset)) {
			pr_warn("%s lacks property reset-gpios\n", np->name);
			continue;
		}

		gpiod_direction_output(reset, 0);

		gpiod_set_value(reset, 1);
		msleep(10);
		gpiod_set_value(reset, 0);
		msleep(150);

		gpiod_put(reset);
	}
	/*
	 * EEE implemented in RTL8201F looks incompatible with
	 * 802.3az standard. Disabled it by default for better
	 * compatibility.
	 */
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0004);
	mdiobus_write(phydev->bus, phydev->addr, 0x10, 0x4077);
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0000);
	mdiobus_write(phydev->bus, phydev->addr, 0x0d, 0x0007);
	mdiobus_write(phydev->bus, phydev->addr, 0x0e, 0x003c);
	mdiobus_write(phydev->bus, phydev->addr, 0x0d, 0x4007);
	mdiobus_write(phydev->bus, phydev->addr, 0x0e, 0x0000);

	/* adjust RMII mode setting */
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0007);
	mdiobus_write(phydev->bus, phydev->addr, 0x10, 0x077a);

	/* return to page 0 */
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0000);

	return 0;
}

static struct of_device_id const eth_ids[] = {
	{ .compatible = "arasan,emac", },
	{ },
};

static int lg115x_init_eth(void)
{
	struct device_node *np;
	struct gpio_desc *reset;

	phy_register_fixup_for_uid(0x001cc816, 0xfffffff0, rtl8201f_fixup_run);

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

	for_each_matching_node(np, eth_ids) {
		struct platform_device *pdev;
		struct resource *res;

		res = kzalloc(sizeof(struct resource) * 2, GFP_KERNEL);
		of_address_to_resource(np, 0, &res[0]);
		of_irq_to_resource(np, 0, &res[1]);

		pdev = kzalloc(sizeof(struct platform_device), GFP_KERNEL);
		pdev->name = np->name;
		pdev->id = 0;
		pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
		pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
		pdev->dev.of_node = np;
		pdev->num_resources = 2;
		pdev->resource = res;

		if (cmem) {
			dma_declare_coherent_memory(&pdev->dev, cmem, cmem,
			                            SZ_1M, DMA_MEMORY_IO);
		}

		platform_device_register(pdev);
	}

	return 0;
}
#ifndef	CONFIG_USER_INITCALL_NET
device_initcall(lg115x_init_eth);
#else	/* CONFIG_USER_INITCALL_NET */
user_initcall_grp("NET", lg115x_init_eth);
#endif	/* CONFIG_USER_INITCALL_NET */
