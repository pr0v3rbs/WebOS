/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/gpio.h>

#include "phy-lg1k-usb.h"

static int lg1k_usb_gpio_set_pin_mux(void __iomem *addr,
				unsigned int mask, unsigned int val,
				int enable)
{
	unsigned int set_val;
	unsigned int masked_val = mask | val;

	if (!addr)
		return -EFAULT;

	set_val = readl_relaxed(addr);
	set_val = enable ? (set_val | masked_val):(set_val & (~masked_val));
	writel_relaxed(set_val, addr);

	return 0;
}

int lg1k_usbphy_parse_dt(struct lg1k_usbphy *lgphy)
{
	unsigned int regs[2];
	int ret;
	int i;
	struct device_node *np = lgphy->dev->of_node;

	if (!np) {
		dev_err(lgphy->dev, "device node is null\n");
		return -ENODEV;
	}

	i = of_property_match_string(np, "reg-names", "base");
	lgphy->base_addr = of_iomap(np, i);
	if (!lgphy->base_addr) {
		dev_err(lgphy->dev, "error base addr\n");
		ret = -EFAULT;
		goto cleanup;
	}

	i = of_property_match_string(np, "reg-names", "host");
	lgphy->host_reg = of_iomap(np, i);
	if (!lgphy->host_reg) {
		dev_err(lgphy->dev, "error host reg\n");
		ret = -EFAULT;
		goto cleanup;
	}

	i = of_property_match_string(np, "reg-names", "phy");
	lgphy->phy_reg = of_iomap(np, i);
	if (!lgphy->phy_reg) {
		dev_err(lgphy->dev, "error phy reg\n");
		ret = -EFAULT;
		goto cleanup;
	}
	i = of_property_match_string(np, "reg-names", "pinmux");
	lgphy->pin_mux = of_iomap(np, i);

	lgphy->ctl_gpio = lgphy->ocd_gpio = lgphy->rst_gpio = -1;
	lgphy->ctl_gpio = of_get_named_gpio(np,"ctrl-gpios", 0);
	lgphy->ocd_gpio = of_get_named_gpio(np,"ocd-gpios", 0);
	lgphy->rst_gpio = of_get_named_gpio(np,"rst-gpios", 0);
	lgphy->hw_initialized = 0;

	/* For USB3.0 only */
	i = of_property_match_string(np, "reg-names", "gbl");
	lgphy->gbl_reg = of_iomap(np, i);

	ret = of_property_read_u32(np, "rx_eq_val", &regs[0]);
	if (ret)
		lgphy->rx_eq_val = 0x0;
	else
		lgphy->rx_eq_val = regs[0];

	return 0;

cleanup:
	if (lgphy->base_addr)
		iounmap(lgphy->base_addr);
	if (lgphy->host_reg)
		iounmap(lgphy->host_reg);
	if (lgphy->phy_reg)
		iounmap(lgphy->phy_reg);
	if (lgphy->pin_mux)
		iounmap(lgphy->pin_mux);
	if (lgphy->gbl_reg)
		iounmap(lgphy->gbl_reg);
	return ret;
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_parse_dt);

int lg1k_usbphy_set_type(struct usb_phy *phy, enum usb_phy_type type)
{
	phy->type = type;

	return 0;
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_set_type);

int lg1k_usbphy_set_vbus(struct usb_phy *phy, int on)
{
	struct device *dev;
	struct lg1k_usbphy *lgphy;

	dev = phy->dev;
	lgphy = phy_to_lgphy(phy);

	if (lgphy->pin_mux) {
		unsigned int regs[2];
		int ret;
		ret = of_property_read_u32_array(lgphy->dev->of_node,
					"gpio-pinmux", regs, 2);
		if (ret) {
			dev_warn( lgphy->dev,
				"pinmux register is exist,but won't be set\n");
		} else {
			ret = lg1k_usb_gpio_set_pin_mux(lgphy->pin_mux, regs[0],
						regs[1], 1);
			if (ret) {
				dev_err(lgphy->dev, "err pin mux\n");
				return ret;
			}
		}
        }
	if (lgphy->ctl_gpio >= 0) {
		gpio_request(lgphy->ctl_gpio, "usbphy_ctl");
		gpio_direction_output(lgphy->ctl_gpio, on);
		gpio_set_value(lgphy->ctl_gpio, on);

		dev_info(dev, "vbus turn %s\n", on ? "on" : "off");
		gpio_free(lgphy->ctl_gpio);
	}

	if (on && (lgphy->ocd_gpio >= 0)) {
		gpio_request(lgphy->ocd_gpio, "usbphy_ocd");
		gpio_direction_input(lgphy->ocd_gpio);
		gpio_free(lgphy->ocd_gpio);
	}

	if (on && (lgphy->rst_gpio >= 0)) {
		gpio_request(lgphy->rst_gpio, "usbphy_rst");
		gpio_direction_output(lgphy->rst_gpio, 0);
		gpio_set_value(lgphy->rst_gpio, 0);
		mdelay(10);
		gpio_set_value(lgphy->rst_gpio, 1);
		gpio_free(lgphy->rst_gpio);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_set_vbus);

void lg1k_usbphy_shutdown(struct usb_phy *phy)
{
	struct device *dev;
	struct lg1k_usbphy *lgphy;

	dev = phy->dev;
	lgphy = phy_to_lgphy(phy);

	dev_warn(dev, "Not properly implemeted yet..\n");
	lg1k_usbphy_set_vbus(phy, false);
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_shutdown);

int lg1k_usbphy_set_wakeup(struct usb_phy *phy, bool enabled)
{
	struct device *dev;
	struct lg1k_usbphy *lgphy;

	dev = phy->dev;
	lgphy = phy_to_lgphy(phy);

	dev_warn(dev, "Wake up setting is not suppored..\n");

	return 0;
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_set_wakeup);

int lg1k_usbphy_set_suspend(struct usb_phy *phy, int suspend)
{
	struct device *dev;
	struct lg1k_usbphy *lgphy;

	dev = phy->dev;
	lgphy = phy_to_lgphy(phy);

	dev_warn(dev, "Not properly implemeted yet..\n");

	return 0;
}
EXPORT_SYMBOL_GPL(lg1k_usbphy_set_suspend);

MODULE_DESCRIPTION("LG DTV USB phy controller");
MODULE_AUTHOR("Shinhoe Kim <shinhoe.kim@lge.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:lg1k-usbphy");
