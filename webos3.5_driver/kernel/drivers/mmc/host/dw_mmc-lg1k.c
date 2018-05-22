/*
 * drivers/mmc/host/dwmmc-lg1k.c
 *
 * Support of DW_MMC platform devices for lg115x
 *
 * Copyright (C) 2016 LG Electronics
 *
 * Author: Hankyung Yu <hankyung.yu@lge.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/delay.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/host.h>
#include <linux/mmc/dw_mmc.h>
#include <linux/platform_device.h>

#include "dw_mmc.h"
#include "dw_mmc-pltfm.h"

static void dwmmc_lg1k_prepare_command(struct dw_mci *host, u32 *cmdr)
{
	*cmdr |= SDMMC_CMD_USE_HOLD_REG;
}

/* Common capabilities of Exynos4/Exynos5 SoC */
static unsigned long lg1k_dwmmc_caps[4] = {
	MMC_CAP_1_8V_DDR | MMC_CAP_8_BIT_DATA | MMC_CAP_CMD23,
	MMC_CAP_CMD23,
	MMC_CAP_CMD23,
	MMC_CAP_CMD23,
};

static const struct dw_mci_drv_data lg1k_dwmmc_drv_data = {
	.caps				= lg1k_dwmmc_caps,
	.init				= NULL,
	.setup_clock			= NULL,
	.prepare_command		= dwmmc_lg1k_prepare_command,
	.set_ios			= NULL,
	.parse_dt			= NULL,
	.execute_tuning			= NULL,
};

static struct platform_device_id dwmmc_lg1k_driver_ids[] = {
	{
		.name		= "dwmmc-lg1k",
		.driver_data	= (kernel_ulong_t)&lg1k_dwmmc_drv_data,
	},
	{}
};
MODULE_DEVICE_TABLE(platform, dwmmc_lg1k_driver_ids);

static const struct of_device_id dwmmc_lg1k_dt_ids[] = {
	{ .compatible = "lge,lg1k-dwmmc",
		.data = (void *)&lg1k_dwmmc_drv_data },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, dwmmc_lg1k_dt_ids);

static int dwmmc_lg1k_probe(struct platform_device *pdev)
{
	const struct dw_mci_drv_data *drv_data;
	const struct of_device_id *match;

	match = of_match_node(dwmmc_lg1k_dt_ids, pdev->dev.of_node);
	drv_data = match->data;

	return dw_mci_pltfm_register(pdev, drv_data);
}

static struct platform_driver dwmmc_driver = {
	.id_table	= dwmmc_lg1k_driver_ids,
	.driver = {
		.name	= "dwmmc-lg1k",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(dwmmc_lg1k_dt_ids),
		.pm = &dw_mci_pltfm_pmops,
	},
	.probe		= dwmmc_lg1k_probe,
	.remove		= __exit_p(dw_mci_pltfm_remove),
};
module_platform_driver(dwmmc_driver);

MODULE_DESCRIPTION("LG1XXX Secure Digital Host Controller Interface driver");
MODULE_AUTHOR("Hankyung Yu <hankyung.yu@lge.com>");
MODULE_LICENSE("GPL v2");

