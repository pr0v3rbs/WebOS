#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/jiffies.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <linux/timer.h>

#define DSP			0x00
#define WOL			0x01
#define BIST			0x03
#define VMDAC			0x07

#define TSTCNTL_R(RB,AD)	(0x8000 | ((RB) << 11) | ((AD) << 5))
#define TSTCNTL_W(RB,AD)	(0x4000 | ((RB) << 11) | ((AD) << 0))
#define TSTCNTL(RW,RB,AD)	TSTCNTL_##RW(RB,AD)

#define OMNIPHY_MCSR		0x11
#define OMNIPHY_TST_CNTL	0x14
#define OMNIPHY_TST_READ1	0x15
#define OMNIPHY_TST_READ2	0x16
#define OMNIPHY_TST_WRITE	0x17
#define OMNIPHY_SCSI		0x1b
#define OMNIPHY_ISF		0x1d
#define OMNIPHY_IMR		0x1e
#define OMNIPHY_SCSR		0x1f

#define MEDIA_NONE		0x00
#define MEDIA_MDI		0x01
#define MEDIA_MDIX		0x02

#define MAX_AUTONEG_RETRY	4

struct omniphy_priv {
	bool saved;

	/* saved link-mode */
	int autoneg;
	int speed;
	int duplex;
};

#ifdef CONFIG_ARCH_LG1K

extern void reset_omniphy(struct device_node *);

static void omniphy_reset(struct phy_device *phydev)
{
	reset_omniphy(phydev->dev.of_node);
}

#else	/* CONFIG_ARCH_LG1K */

static inline void omniphy_reset(struct phy_device *phydev) { }

#endif

static int omniphy_soft_reset(struct phy_device *phydev)
{
	int v;

	omniphy_reset(phydev);

	/*
	 * Address 27: Special Control/Status Indication
	 * Bit 12: SWRST_FAST
	 * 0 - default
	 * 1 - accelerate SW reset counter from 256us to 10us
	 *     for production testing.
	 */
	v = phy_read(phydev, OMNIPHY_SCSI);
	phy_write(phydev, OMNIPHY_SCSI, v | 0x1000);

	dev_info(&phydev->dev, "soft-reset by MDIO\n");
	return genphy_soft_reset(phydev);
}

static int omniphy_config_init(struct phy_device *phydev)
{
	struct omniphy_priv *priv = phydev->priv;

	if (priv->saved) {
		/* restore link-mode */
		phydev->autoneg = priv->autoneg;
		phydev->speed = priv->speed;
		phydev->duplex = priv->duplex;

		priv->saved = false;
	}

	/* medium type might be changed by reset */
	phydev->link = 0;

	return 0;
}

static int omniphy_probe(struct phy_device *phydev)
{
	struct omniphy_priv *priv;

	priv = devm_kzalloc(&phydev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->saved = false;

	phydev->priv = priv;

	return 0;
}

static int omniphy_resume(struct phy_device *phydev)
{
	return phy_init_hw(phydev);
}

static void omniphy_switch_media(struct phy_device *phydev, int media)
{
	switch (media) {
	case MEDIA_MDI:
		dev_info(&phydev->dev, "MDI\n");

		phy_write(phydev, OMNIPHY_TST_WRITE, 0x8003);
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, VMDAC, 0x02));
		phy_write(phydev, OMNIPHY_TST_WRITE, 0x707c);
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, VMDAC, 0x03));
		break;

	case MEDIA_MDIX:
		dev_info(&phydev->dev, "MDI-X\n");

		phy_write(phydev, OMNIPHY_TST_WRITE, 0xf07b);
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, VMDAC, 0x02));
		phy_write(phydev, OMNIPHY_TST_WRITE, 0x0004);
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, VMDAC, 0x03));
		break;

	default:
		dev_info(&phydev->dev, "invalid media-type\n");
		break;
	}
}

static int omniphy_update_link(struct phy_device *phydev)
{
	int v;

	/* do a fake read */
	v = phy_read(phydev, MII_BMSR);
	if (v < 0)
		return v;

	v = phy_read(phydev, MII_BMSR);
	if (v < 0)
		return v;

	if (v & BMSR_LSTATUS) {
		if (!phydev->link) {
			v = phy_read(phydev, OMNIPHY_MCSR);
			if (v & 0x0040)
				omniphy_switch_media(phydev, MEDIA_MDIX);
			else
				omniphy_switch_media(phydev, MEDIA_MDI);
		}

		phydev->link = 1;
	} else
		phydev->link = 0;

	return 0;
}

static int omniphy_read_status(struct phy_device *phydev)
{
	unsigned int retry = 0;
	int v;

__retry__:
	v = omniphy_update_link(phydev);
	if (v < 0)
		return v;

	phydev->speed = SPEED_10;
	phydev->duplex = DUPLEX_HALF;
	phydev->pause = 0;
	phydev->asym_pause = 0;
	phydev->lp_advertising = 0;

	if (!phydev->link)
		return 0;

	if (phydev->autoneg) {
		v = phy_read(phydev, MII_LPA);
		if (v < 0)
			return v;

		phydev->lp_advertising = mii_lpa_to_ethtool_lpa_t(v);

		v = phy_read(phydev, OMNIPHY_SCSR);
		if (v < 0)
			return v;

		if ((v & 0x000c) == 0x0000 || (v & 0x000c) == 0x000c) {
			/* reset and retry upon illegal link-mode */
			if (retry++ < MAX_AUTONEG_RETRY) {
				phy_init_hw(phydev);
				goto __retry__;
			}

			dev_warn(&phydev->dev, "auto-negotiation failed\n");

			return -EIO;
		}

		if (v & 0x0008)
			phydev->speed = SPEED_100;
		if (v & 0x0010) {
			phydev->duplex = DUPLEX_FULL;

			if (phydev->lp_advertising & ADVERTISED_Pause)
				phydev->pause = 1;
			if (phydev->lp_advertising & ADVERTISED_Asym_Pause)
				phydev->asym_pause = 1;
		}
	} else {
		v = phy_read(phydev, MII_BMCR);
		if (v < 0)
			return v;

		if (v & BMCR_FULLDPLX)
			phydev->duplex = DUPLEX_FULL;
		if (v & BMCR_SPEED100)
			phydev->speed = SPEED_100;
	}

	return 0;
}

static int omniphy_ack_interrupt(struct phy_device *phydev)
{
	phy_read(phydev, OMNIPHY_ISF);
	return 0;
}

static int omniphy_config_intr(struct phy_device *phydev)
{
	/* keep WOL interrupts enabled */
	phy_write(phydev, OMNIPHY_IMR, phydev->interrupts ? 0xffff : 0x0e00);
	return 0;
}

static void
omniphy_get_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	u16 mac_addr[3];
	int wol_event;

	phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(R, WOL, 0x03));
	wol_event = phy_read(phydev, OMNIPHY_TST_READ1);

	wol->supported = WAKE_MAGIC;
	wol->wolopts = 0;

	if ((wol_event & 0x0007) == 0x0007) {
		wol->wolopts |= WAKE_MAGIC;

		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(R, WOL, 0x00));
		mac_addr[2] = ntohs(phy_read(phydev, OMNIPHY_TST_READ1));
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(R, WOL, 0x01));
		mac_addr[1] = ntohs(phy_read(phydev, OMNIPHY_TST_READ1));
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(R, WOL, 0x02));
		mac_addr[0] = ntohs(phy_read(phydev, OMNIPHY_TST_READ1));

		if (memcmp(mac_addr, netdev->dev_addr, ETH_ALEN)) {
			dev_warn(&phydev->dev, "invalid MAC address: %pM\n",
			         mac_addr);
		}
	}
}

#ifdef CONFIG_ARCH_LG1K

static void omniphy_isolate(unsigned long data)
{
	void __iomem *base = (void *)data;
	/* power-isolation using GPIO35 */
	writeb_relaxed(0x00, base + 0x0020);
}

#endif

static int
omniphy_set_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	struct net_device *netdev = phydev->attached_dev;
	struct omniphy_priv *priv = phydev->priv;
	u16 mac_addr[3];
	int wol_event = 0;
	int v;

	if ((wol->wolopts & wol->supported) != wol->wolopts)
		return -ENOTSUPP;

	if (wol->wolopts & WAKE_MAGIC)
		wol_event |= 0x0007;

#ifdef CONFIG_ARCH_LG1K
	if (wol_event) {
		static void __iomem *base;
		static struct timer_list timer;

		if (!base) {
			base = ioremap(0xfd440000, 0x1000);
			if (!base)
				return -EIO;
		}

		setup_timer(&timer, omniphy_isolate, (unsigned long)base);
		mod_timer(&timer, jiffies + msecs_to_jiffies(100));
	}
#endif

	/* set WOL event(s) */
	phy_write(phydev, OMNIPHY_TST_WRITE, wol_event);
	phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, WOL, 0x03));

	if (!wol_event)
		return 0;

	if (wol->wolopts & WAKE_MAGIC) {
		memcpy(mac_addr, netdev->dev_addr, ETH_ALEN);

		/* set MAC address */
		phy_write(phydev, OMNIPHY_TST_WRITE, htons(mac_addr[2]));
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, WOL, 0x00));
		phy_write(phydev, OMNIPHY_TST_WRITE, htons(mac_addr[1]));
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, WOL, 0x01));
		phy_write(phydev, OMNIPHY_TST_WRITE, htons(mac_addr[0]));
		phy_write(phydev, OMNIPHY_TST_CNTL, TSTCNTL(W, WOL, 0x02));
	}

	/* enable WOL interrupts */
	phy_write(phydev, OMNIPHY_IMR, 0x0e00);

	/*
	 * disable MDI/MDI-X auto-detection and retain current mode
	 * while WOL-standby -- SW workaround cannot handle MDI/MDI-X
	 * mode changes while WOL-standby, because CPUs are powered-
	 * off. Disable MDI/MDI-X auto-detection and retain current
	 * mode to prevent random WOL-malfunction.
	 */
	v = phy_read(phydev, OMNIPHY_MCSR) & ~0x0080;
	phy_write(phydev, OMNIPHY_MCSR, v);

	/* save link-mode */
	priv->autoneg = phydev->autoneg;
	priv->speed = phydev->speed;
	priv->duplex = phydev->duplex;
	priv->saved = true;

	/* 10Mbps for lower power consumption */
	phydev->autoneg = AUTONEG_DISABLE;
	phydev->speed = SPEED_10;
	genphy_setup_forced(phydev);

	return 0;
}

static struct phy_driver omniphy_drvs[] = {
	{
		.phy_id		= 0x01814400,
		.name		= "omniphy",
		.phy_id_mask	= 0xfffffc00,
		.features	= PHY_BASIC_FEATURES,
		.flags		= PHY_HAS_INTERRUPT,
		.soft_reset	= omniphy_soft_reset,
		.config_init	= omniphy_config_init,
		.probe		= omniphy_probe,
		.resume		= omniphy_resume,
		.config_aneg	= genphy_config_aneg,
		.read_status	= omniphy_read_status,
		.ack_interrupt	= omniphy_ack_interrupt,
		.config_intr	= omniphy_config_intr,
		.get_wol	= omniphy_get_wol,
		.set_wol	= omniphy_set_wol,
		.driver.owner	= THIS_MODULE,
	},
};

#ifndef module_phy_driver

static int __init omniphy_init(void)
{
	return phy_drivers_register(omniphy_drvs, ARRAY_SIZE(omniphy_drvs));
}
module_init(omniphy_init);

static void __exit omniphy_exit(void)
{
	phy_drivers_unregister(omniphy_drvs, ARRAY_SIZE(omniphy_drvs));
}
module_exit(omniphy_exit);

#else	/* module_phy_driver */

module_phy_driver(omniphy_drvs);

#endif	/* module_phy_driver */

static struct mdio_device_id omniphy_ids[] __maybe_unused = {
	{ 0x01814400, 0xfffffc00 },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(mdio, omniphy_ids);

MODULE_AUTHOR("Jongsung Kim <neidhard.kim@lge.com>");
MODULE_DESCRIPTION("OmniPhy 10/100Mbps Ethernet PHY driver");
MODULE_LICENSE("GPL");
