/*
 * drivers/net/phy/realtek.c
 *
 * Driver for Realtek PHYs
 *
 * Author: Johnson Leung <r58129@freescale.com>
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/phy.h>
#include <linux/module.h>
#include <linux/netdevice.h>

#define RTL8201F_INER		0x13
#define RTL8201F_INER_MASK	0x3800
#define RTL8201F_INSR		0x1e
#define RTL8201F_PGSR		0x1f

#define RTL821x_PHYSR		0x11
#define RTL821x_PHYSR_DUPLEX	0x2000
#define RTL821x_PHYSR_SPEED	0xc000
#define RTL821x_INER		0x12
#define RTL821x_INER_INIT	0x6400
#define RTL821x_INSR		0x13

#define	RTL8211E_INER_LINK_STATUS	0x400

MODULE_DESCRIPTION("Realtek PHY driver");
MODULE_AUTHOR("Johnson Leung");
MODULE_LICENSE("GPL");

static void rtl8201f_select_page(struct phy_device *phydev, int page)
{
	phy_write(phydev, RTL8201F_PGSR, page);
}

static int rtl8201f_soft_reset(struct phy_device *phydev)
{
	/*
	 * Leave this function empty to avoid the meaningless
	 * soft-reset immediately followed by hard-reset from
	 * fixup->run.
	 */
	return 0;
}

static int rtl8201f_config_init(struct phy_device *phydev)
{
	/* disable all WOL events */
	rtl8201f_select_page(phydev, 0x11);
	phy_write(phydev, 0x10, 0x0000);

	rtl8201f_select_page(phydev, 0x00);

	return 0;
}

static int rtl8201f_ack_interrupt(struct phy_device *phydev)
{
	int err = phy_read(phydev, RTL8201F_INSR);
	return (err < 0) ? err : 0;
}

static int rtl8201f_config_intr(struct phy_device *phydev)
{
	int err;

	rtl8201f_select_page(phydev, 7);

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL8201F_INER, RTL8201F_INER_MASK |
				phy_read(phydev, RTL8201F_INER));
	else
		err = phy_write(phydev, RTL8201F_INER, ~RTL8201F_INER_MASK &
				phy_read(phydev, RTL8201F_INER));

	rtl8201f_select_page(phydev, 0);

	return err;
}

static void rtl8201f_get_wol(struct phy_device *phydev,
                             struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	u16 mac_addr[3];
	int wol_event;

	rtl8201f_select_page(phydev, 0x11);
	wol_event = phy_read(phydev, 0x10);

	/*
	 * RTL8201F supports WAKE_{PHY,MAGIC,UCAST,MCAST,BCAST} actually.
	 * But, only WAKE_MAGIC is implemented at this time.
	 */
	wol->supported = WAKE_MAGIC;
	wol->wolopts = 0;

	if (wol_event & 0x1000) {
		wol->wolopts |= WAKE_MAGIC;

		rtl8201f_select_page(phydev, 0x12);
		mac_addr[0] = phy_read(phydev, 0x10);
		mac_addr[1] = phy_read(phydev, 0x11);
		mac_addr[2] = phy_read(phydev, 0x12);

		if (memcmp(mac_addr, netdev->dev_addr, ETH_ALEN))
			pr_warn("WOL by Magic-Packet without MAC address\n");
	}

	rtl8201f_select_page(phydev, 0x00);
}

static int rtl8201f_set_wol(struct phy_device *phydev,
                            struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	u16 const *mac_addr = (u16 *)netdev->dev_addr;
	int wol_event = 0;

	if ((wol->wolopts & wol->supported) != wol->wolopts)
		return -ENOTSUPP;

	/* set max packet length */
	rtl8201f_select_page(phydev, 0x11);
	phy_write(phydev, 0x11, 0x1fff);

	if (wol->wolopts & WAKE_MAGIC) {
		wol_event |= 0x1000;

		/* set MAC address */
		rtl8201f_select_page(phydev, 0x12);
		phy_write(phydev, 0x10, mac_addr[0]);
		phy_write(phydev, 0x11, mac_addr[1]);
		phy_write(phydev, 0x12, mac_addr[2]);
	}

	/* set WOL event(s) */
	rtl8201f_select_page(phydev, 0x11);
	phy_write(phydev, 0x10, wol_event);

	rtl8201f_select_page(phydev, 0x00);

	return 0;
}

static int rtl821x_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, RTL821x_INSR);

	return (err < 0) ? err : 0;
}

static int rtl8211b_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL821x_INER,
				RTL821x_INER_INIT);
	else
		err = phy_write(phydev, RTL821x_INER, 0);

	return err;
}

static int rtl8211e_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, RTL821x_INER,
				RTL8211E_INER_LINK_STATUS);
	else
		err = phy_write(phydev, RTL821x_INER, 0);

	return err;
}

static struct phy_driver realtek_drvs[] = {
	{	/* RTL8201F */
		.phy_id		= 0x001cc816,
		.name		= "RTL8201F 10/100Mbps Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.soft_reset	= rtl8201f_soft_reset,
		.config_init	= rtl8201f_config_init,
		.config_aneg	= genphy_config_aneg,
		.read_status	= genphy_read_status,
		.ack_interrupt	= rtl8201f_ack_interrupt,
		.config_intr	= rtl8201f_config_intr,
		.get_wol	= rtl8201f_get_wol,
		.set_wol	= rtl8201f_set_wol,
		.driver		= { .owner = THIS_MODULE,},
	},

	{
		.phy_id         = 0x00008201,
		.name           = "RTL8201CP Ethernet",
		.phy_id_mask    = 0x0000ffff,
		.features       = PHY_BASIC_FEATURES,
		.flags          = PHY_HAS_INTERRUPT,
		.config_aneg    = &genphy_config_aneg,
		.read_status    = &genphy_read_status,
		.driver         = { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc912,
		.name		= "RTL8211B Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &genphy_config_aneg,
		.read_status	= &genphy_read_status,
		.ack_interrupt	= &rtl821x_ack_interrupt,
		.config_intr	= &rtl8211b_config_intr,
		.driver		= { .owner = THIS_MODULE,},
	}, {
		.phy_id		= 0x001cc915,
		.name		= "RTL8211E Gigabit Ethernet",
		.phy_id_mask	= 0x001fffff,
		.features	= PHY_GBIT_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.config_aneg	= &genphy_config_aneg,
		.read_status	= &genphy_read_status,
		.ack_interrupt	= &rtl821x_ack_interrupt,
		.config_intr	= &rtl8211e_config_intr,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.driver		= { .owner = THIS_MODULE,},
	},
};

static int __init realtek_init(void)
{
	return phy_drivers_register(realtek_drvs, ARRAY_SIZE(realtek_drvs));
}

static void __exit realtek_exit(void)
{
	phy_drivers_unregister(realtek_drvs, ARRAY_SIZE(realtek_drvs));
}

module_init(realtek_init);
module_exit(realtek_exit);

static struct mdio_device_id __maybe_unused realtek_tbl[] = {
	{ 0x001cc816, 0x001fffff },
	{ 0x001cc912, 0x001fffff },
	{ 0x001cc915, 0x001fffff },
	{ }
};

MODULE_DEVICE_TABLE(mdio, realtek_tbl);
