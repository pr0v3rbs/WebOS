#include <linux/kernel.h>

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
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


static int omniphy_fixup_run(struct phy_device *phydev)
{
	static void __iomem *base;

	if (!base)
		base = ioremap(0xc930a000, 0x1000);

	if (readl_relaxed(base + 0x04b4) == 0xb0001313) {
		/* enable access to Analog and DSP register bank */
		phy_write(phydev, 0x14, 0x0400);
		phy_write(phydev, 0x14, 0x0000);
		phy_write(phydev, 0x14, 0x0400);

		/* apply some Tx-tuning values */
		phy_write(phydev, 0x17, 0x804e);
		phy_write(phydev, 0x14, 0x4416);
		phy_write(phydev, 0x17, 0x3c00);
		phy_write(phydev, 0x14, 0x4417);
		phy_write(phydev, 0x17, 0x0036);
		phy_write(phydev, 0x14, 0x4418);
		phy_write(phydev, 0x17, 0x1000);
		phy_write(phydev, 0x14, 0x441c);

		dev_info(&phydev->dev, "Tx-tuning applied\n");
	}

	return 0;
}

static int rtl8201f_fixup_run(struct phy_device *phydev)
{
	static u16 rmii_mode;

	if (unlikely(!rmii_mode)) {
		if (of_machine_is_compatible("lge,lg1312"))
			rmii_mode = 0x066a;
		else
			rmii_mode = 0x077a;
	}

	/* adjust RMII mode setting */
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0007);
	mdiobus_write(phydev->bus, phydev->addr, 0x10, rmii_mode);

	/* return to page 0 */
	mdiobus_write(phydev->bus, phydev->addr, 0x1f, 0x0000);

	return 0;
}

static int init_emac(struct device_node *np)
{
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
		dma_declare_coherent_memory(&pdev->dev, cmem, cmem, SZ_1M,
		                            DMA_MEMORY_IO);
	}

	return platform_device_register(pdev);
}

void reset_omniphy(struct device_node *np)
{
	static void __iomem *base[3];
	u32 v;

	if (!base[0])
		base[0] = ioremap(0xc3501000, 0x1000);
	if (!base[1])
		base[1] = ioremap(0xc9305000, 0x1000);
	if (!base[2])
		base[2] = ioremap(0xfd440000, 0x1000);

	/* assert reset */
	v = readl_relaxed(base[0] + 0x0004) | 0x00000404;
	writel_relaxed(v, base[0] + 0x0004);

	mdelay(1);

	/* adjust pinmux to use GPIO35 */
	v = readl_relaxed(base[1] + 0x0410) | 0x08000000;
	writel_relaxed(v, base[1] + 0x0410);
	v = readl_relaxed(base[1] + 0x0424) | 0x08000000;
	writel_relaxed(v, base[1] + 0x0424);

	mdelay(1);

	/* release power isolation using GPIO35 */
	v = readb_relaxed(base[2] + 0x0400) | 0x08;
	writeb_relaxed(v, base[2] + 0x0400);
	writeb_relaxed(0x08, base[2] + 0x0020);

	mdelay(1);

	/* deassert reset */
	v = readl_relaxed(base[0] + 0x0004) ^ 0x00000404;
	writel_relaxed(v, base[0] + 0x0004);

	mdelay(1);
}

static int init_macb(struct device_node *np)
{
	if (of_machine_is_compatible("lge,lg1313")) {
		struct device_node *phy_node = of_get_next_child(np, NULL);
		reset_omniphy(phy_node);
	} else {
		/* MACB/GEM: enable half-duplex mode */
		void __iomem *base = ioremap(0xfd3003d4, 4);
		writel_relaxed(0x00000000, base);
		iounmap(base);
	}

	return 0;
}

static struct of_device_id const eth_ids[] = {
	{ .compatible = "arasan,emac", .data = init_emac, },
	{ .compatible = "cdns,gem",    .data = init_macb, },
	{ },
};

static int lg115x_init_eth(void)
{
	struct device_node *np;
	struct of_device_id const *match;
	int err;

	phy_register_fixup_for_uid(0x01814400, 0xfffffc00, omniphy_fixup_run);
	phy_register_fixup_for_uid(0x001cc816, 0xfffffff0, rtl8201f_fixup_run);

	for_each_matching_node_and_match(np, eth_ids, &match) {
		if (match->data) {
			err = ((int (*)(struct device_node *))match->data)(np);
			if (err < 0)
				return err;
		}
	}

	return 0;
}
#ifndef	CONFIG_USER_INITCALL_NET
device_initcall(lg115x_init_eth);
#else	/* CONFIG_USER_INITCALL_NET */
user_initcall_grp("NET", lg115x_init_eth);
#endif	/* CONFIG_USER_INITCALL_NET */
