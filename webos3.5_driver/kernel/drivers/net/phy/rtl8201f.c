#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/gpio.h>
#include <linux/netdevice.h>
#include <linux/phy.h>

#define RTL8201F_INER		0x13
#define RTL8201F_INER_MASK	0x3800
#define RTL8201F_INSR		0x1e
#define RTL8201F_PGSR		0x1f

static int rtl8201f_soft_reset(struct phy_device *phydev)
{
	struct gpio_desc *gpio = gpiod_get(&phydev->dev, "reset", GPIOD_ASIS);

	if (IS_ERR(gpio))
		goto _soft_reset_;

	pr_info("hard-reset by GPIO\n");
	gpiod_direction_output(gpio, 0);
	gpiod_set_value(gpio, 1);
	msleep(10);
	gpiod_set_value(gpio, 0);
	msleep(150);

	gpiod_put(gpio);

	return 0;

_soft_reset_:
	pr_info("soft-reset by MDIO\n");
	return genphy_soft_reset(phydev);
}

static int rtl8201f_config_init(struct phy_device *phydev)
{
	/*
	 * EEE implemented in RTL8201F looks incompatible with
	 * 802.3az standard. Disabled it by default for better
	 * compatibility.
	 */
	phy_write(phydev, RTL8201F_PGSR, 0x04);
	phy_write(phydev, 0x10, 0x4077);
	phy_write(phydev, RTL8201F_PGSR, 0x00);
	phy_write(phydev, 0x0d, 0x0007);
	phy_write(phydev, 0x0e, 0x003c);
	phy_write(phydev, 0x0d, 0x4007);
	phy_write(phydev, 0x0e, 0x0000);

	/* disable all WOL events */
	phy_write(phydev, RTL8201F_PGSR, 0x11);
	phy_write(phydev, 0x10, 0x0000);

	phy_write(phydev, RTL8201F_PGSR, 0x00);

	return 0;
}

static int rtl8201f_resume(struct phy_device *phydev)
{
	return phy_init_hw(phydev);
}

static int rtl8201f_ack_interrupt(struct phy_device *phydev)
{
	phy_read(phydev, RTL8201F_INSR);
	return 0;
}

static int rtl8201f_config_intr(struct phy_device *phydev)
{
	int iner;

	phy_write(phydev, RTL8201F_PGSR, 0x07);
	iner = phy_read(phydev, RTL8201F_INER);

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		iner |= RTL8201F_INER_MASK;
	else
		iner &= ~RTL8201F_INER_MASK;

	phy_write(phydev, RTL8201F_INER, iner);
	phy_write(phydev, RTL8201F_PGSR, 0x00);

	return 0;
}

static void
rtl8201f_get_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	u16 mac_addr[3];
	int wol_event;

	phy_write(phydev, RTL8201F_PGSR, 0x11);
	wol_event = phy_read(phydev, 0x10);

	/*
	 * RTL8201F supports WAKE_{PHY,MAGIC,UCAST,MCAST,BCAST} actually.
	 * But, only WAKE_MAGIC is implemented at this time.
	 */
	wol->supported = WAKE_MAGIC;
	wol->wolopts = 0;

	if (wol_event & 0x1000) {
		wol->wolopts |= WAKE_MAGIC;

		phy_write(phydev, RTL8201F_PGSR, 0x12);
		mac_addr[0] = phy_read(phydev, 0x10);
		mac_addr[1] = phy_read(phydev, 0x11);
		mac_addr[2] = phy_read(phydev, 0x12);

		if (memcmp(mac_addr, netdev->dev_addr, ETH_ALEN))
			pr_warn("WOL by Magic-Packet without MAC address\n");
	}

	phy_write(phydev, RTL8201F_PGSR, 0x00);
}

static int
rtl8201f_set_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	u16 const *mac_addr = (u16 *)netdev->dev_addr;
	int wol_event = 0;

	if ((wol->wolopts & wol->supported) != wol->wolopts)
		return -ENOTSUPP;

	/* set max packet length */
	phy_write(phydev, RTL8201F_PGSR, 0x11);
	phy_write(phydev, 0x11, 0x1fff);

	if (wol->wolopts & WAKE_MAGIC) {
		wol_event |= 0x1000;

		/* set MAC address */
		phy_write(phydev, RTL8201F_PGSR, 0x12);
		phy_write(phydev, 0x10, mac_addr[0]);
		phy_write(phydev, 0x11, mac_addr[1]);
		phy_write(phydev, 0x12, mac_addr[2]);
	}

	/* set WOL event(s) */
	phy_write(phydev, RTL8201F_PGSR, 0x11);
	phy_write(phydev, 0x10, wol_event);

	phy_write(phydev, RTL8201F_PGSR, 0x00);

	return 0;
}

static struct phy_driver rtl8201f_drvs[] = {
	{
		.phy_id		= 0x001cc816,
		.name		= "RTL8201F 10/100Mbps Ethernet",
		.phy_id_mask	= 0xffffffff,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.soft_reset	= rtl8201f_soft_reset,
		.config_init	= rtl8201f_config_init,
		.resume		= rtl8201f_resume,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.ack_interrupt	= rtl8201f_ack_interrupt,
		.config_intr	= rtl8201f_config_intr,
		.get_wol	= rtl8201f_get_wol,
		.set_wol	= rtl8201f_set_wol,
		.driver.owner	= THIS_MODULE,
	},
};

#ifndef module_phy_driver

static int __init rtl8201f_init(void)
{
	return phy_drivers_register(rtl8201f_drvs, ARRAY_SIZE(rtl8201f_drvs));
}
module_init(rtl8201f_init);

static void __exit rtl8201f_exit(void)
{
	phy_drivers_unregister(rtl8201f_drvs, ARRAY_SIZE(rtl8201f_drvs));
}
module_exit(rtl8201f_exit);

#else	/* module_phy_driver */

module_phy_driver(rtl8201f_drvs);

#endif	/* module_phy_driver */

static struct mdio_device_id rtl8201f_ids[] __maybe_unused = {
	{ 0x001cc816, 0xffffffff },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(mdio, rtl8201f_ids);

MODULE_AUTHOR("Jongsung Kim <neidhard.kim@lge.com>");
MODULE_DESCRIPTION("Realtek RTL8201F 10/100Mbps Ethernet PHY driver");
MODULE_LICENSE("GPL");
