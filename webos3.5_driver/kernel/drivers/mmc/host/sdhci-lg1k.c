/*
 * drivers/mmc/host/sdhci-lg1k.c
 *
 * Support of SDHCI platform devices for lg115x
 *
 * Copyright (C) 2013 LG Electronics
 *
 * Author: Chanho Min <chanho.min@lge.com>
 *             Hankyung Yu <hankyung.yu@lge.com>
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

#include "sdhci-pltfm.h"

#define EMMC_HOST_V1			0
#define EMMC_HOST_V2			1
#define EMMC_HOST_V3			2

#define EMMC_CLOCK_GATING		0x78
#define EMMC_DLL_SELECT			0x7C

struct sdhci_lg1k_drv_data {
	void __iomem 	*top_reg;
	unsigned int	host_rev;
	unsigned int	offset_tab;
	unsigned int	offset_dll;
	unsigned int	offset_rdy;
	unsigned int	offset_delay;
	unsigned int	ddr50_out;
	unsigned int	ddr50_in;
	unsigned int	hs200_out;
	unsigned int	hs200_in;
	unsigned int	hs400_out;
	unsigned int	hs400_in;
	unsigned int	dll_iff;
	unsigned int	trm_icp;
	unsigned int	strb;
	unsigned int	host_ds;		// host driver strength
	unsigned int	device_ds;		// device driver strength
	unsigned int	pm_enable;
	unsigned int	inited;
};

static struct sdhci_lg1k_drv_data lg1k_441_sdhci_drv_data = {
	.pm_enable = 0,
};

static struct sdhci_lg1k_drv_data lg1k_451_sdhci_drv_data = {
	.pm_enable = 0,
};

static struct sdhci_lg1k_drv_data lg1k_451_sd_sdhci_drv_data = {
	.pm_enable = 1,
};

static struct sdhci_lg1k_drv_data lg1k_50_sdhci_drv_data = {
	.host_rev = EMMC_HOST_V1,
	.offset_tab = 0x0,
	.offset_dll = 0x28,
	.offset_rdy = 0x3C,
	.pm_enable = 1,
};

static struct sdhci_lg1k_drv_data lg1k_50_sdhci_v2_drv_data = {
	.host_rev = EMMC_HOST_V2,
	.offset_tab = 0x0,
	.offset_dll = 0x24,
	.offset_rdy = 0x48,
	.offset_delay = 0x30,
	.dll_iff = 0x0,
	.pm_enable = 1,
};

static struct sdhci_lg1k_drv_data lg1k_50_sdhci_v3_drv_data = {
	.host_rev = EMMC_HOST_V3,
	.offset_tab = 0x10,
	.offset_dll = 0x60,
	.offset_rdy = 0x4C,
	.offset_delay = 0x6C,
	.dll_iff = 0x0,
	.pm_enable = 1,
	.device_ds = 0,
};

static const struct of_device_id sdhci_lg1k_dt_ids[];

static inline struct sdhci_lg1k_drv_data *sdhci_lg1k_get_driver_data(
			struct platform_device *pdev)
{
#ifdef CONFIG_OF
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(sdhci_lg1k_dt_ids, pdev->dev.of_node);
		return (struct sdhci_lg1k_drv_data *)match->data;
	}
#endif
	return (struct sdhci_lg1k_drv_data *)
			platform_get_device_id(pdev)->driver_data;
}

#ifdef CONFIG_OF
void lg1k_get_reg(struct sdhci_host *host)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	unsigned int i;

#define REG_DUMP_SIZE     64

	pr_info("shdci reg dump CURRENT (in/out tab : 0x%08X)\n",
			readl(drv_data->top_reg + drv_data->offset_tab));
	for (i = 0; i < REG_DUMP_SIZE; i += 16) {
		pr_info("0x%02X : 0x%08X 0x%08X 0x%08X 0x%08X\n",
			i,
			readl(host->ioaddr + i),
			readl(host->ioaddr + i + 4),
			readl(host->ioaddr + i + 8),
			readl(host->ioaddr + i + 12));
	}
}

static void lg1k_set_inited(struct sdhci_host *host, unsigned int val)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	drv_data->inited = val;
}

static unsigned int lg1k_get_inited(struct sdhci_host *host)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	return drv_data->inited;
}

#define LG1K_ALIGN32(host, addr)	(host->ioaddr + (addr & 0xFFFFFFFC))
#define LG1K_SHIFT(addr)			((addr & 0x3) * 8)
#define LG1K_MASK8(addr)			(~(0xff << ((addr & 0x3) * 8)))
#define LG1K_MASK16(addr)			(~(0xffff << ((addr & 0x3) * 8)))

static u32 lg1k_readl(struct sdhci_host *host, int reg)
{
	u32 read_reg;

	read_reg = readl(host->ioaddr + reg);
	if ((reg == SDHCI_INT_STATUS) && (read_reg & 0xffff0000)) {
		if (lg1k_get_inited(host)) {
			lg1k_get_reg(host);
		}
	}

	return read_reg;
}

static void lg1k_writew(struct sdhci_host *host, u16 val, int reg)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);

	if (reg == SDHCI_TRANSFER_MODE) {
		pltfm_host->xfer_mode_shadow = val;
		return;
	} else if (reg == SDHCI_COMMAND) {
		writel((val << LG1K_SHIFT(reg)) | pltfm_host->xfer_mode_shadow, \
			LG1K_ALIGN32(host, reg));
		return;
	}

	writel((readl(LG1K_ALIGN32(host, reg)) & LG1K_MASK16(reg)) | \
		(val << LG1K_SHIFT(reg)), LG1K_ALIGN32(host, reg));
}

static void lg1k_writeb(struct sdhci_host *host, u8 val, int reg)
{
	writel((readl(LG1K_ALIGN32(host, reg)) & LG1K_MASK8(reg)) | \
		(val << LG1K_SHIFT(reg)), LG1K_ALIGN32(host, reg));
}

static u16 lg1k_readw(struct sdhci_host *host, int reg)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);

	if (reg == SDHCI_TRANSFER_MODE)
		return pltfm_host->xfer_mode_shadow;

	return (u16)((readl(LG1K_ALIGN32(host, reg)) >> LG1K_SHIFT(reg)) & 0xffff);
}

static u8 lg1k_readb(struct sdhci_host *host, int reg)
{
	return (u8)((readl(LG1K_ALIGN32(host, reg)) >> LG1K_SHIFT(reg)) & 0xff);
}

static void lg1k_451_platform_init(struct sdhci_host *host)
{
	host->mmc->caps |= MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA;
	host->mmc->caps2 |= MMC_CAP2_HS200;
	host->quirks2 |= SDHCI_QUIRK2_BROKEN_1_8V;
}

static void lg1k_50_platform_init(struct sdhci_host *host)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct device_node *np = pdev->dev.of_node;
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);
	u32 tab_delay = 0;
	u32 device_ds = 1;
	u32 reg = 0;

	host->mmc->caps |= MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA | MMC_CAP_UHS_DDR50;
	host->mmc->caps2 |= MMC_CAP2_HS400_1_8V | MMC_CAP2_HS200_1_8V_SDR | MMC_CAP2_FULL_PWR_CYCLE;
	drv_data->top_reg = of_iomap(np, 1);
	host->quirks2 |= SDHCI_QUIRK2_BROKEN_1_8V;

	if (!drv_data->top_reg) {
		dev_err(&pdev->dev, "Failed to map IO space\n");
		goto fail;
	}

	if (of_property_read_u32(np, "tab-delay", &tab_delay) < 0) {
		goto fail;
	}

	if (of_property_read_u32(np, "device-ds", &device_ds) < 0)
		drv_data->device_ds = 1;
	else
		drv_data->device_ds = device_ds;

	drv_data->ddr50_out = tab_delay & 0xF;
	drv_data->ddr50_in = (tab_delay >> 4) & 0x3F;
	drv_data->hs200_out = (tab_delay >> 10) & 0xF;
	drv_data->hs200_in = (tab_delay >> 14) & 0x3F;
	drv_data->hs400_out = (tab_delay >> 20) & 0xF;
	drv_data->hs400_in = (tab_delay >> 24) & 0x3F;

	if (drv_data->host_rev == EMMC_HOST_V1) {
		reg = readl(drv_data->top_reg + drv_data->offset_dll);
		drv_data->dll_iff = (reg >> 8) & 0x7;
		drv_data->trm_icp = reg & 0xF;
		drv_data->strb = (reg >> 4) & 0xF;
		drv_data->host_ds = (reg >> 20) & 0x7;
	} else if ((drv_data->host_rev == EMMC_HOST_V2) || \
				(drv_data->host_rev == EMMC_HOST_V3)) {
		reg = readl(drv_data->top_reg + drv_data->offset_delay);
		drv_data->strb = reg & 0xF;
		reg = readl(drv_data->top_reg + drv_data->offset_dll);
		drv_data->trm_icp = reg & 0xF;
		drv_data->host_ds = (reg >> 20) & 0x7;
	}

	return;

fail:
	host->quirks2 |= SDHCI_QUIRK2_BROKEN_HS200|SDHCI_QUIRK2_BROKEN_DDR50;
	iounmap(drv_data->top_reg);
}
#endif

static void lg1k_441_platform_init(struct sdhci_host *host)
{
	host->quirks2 |= SDHCI_QUIRK2_PRESET_VALUE_BROKEN;
	host->mmc->caps |= MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA
				| MMC_CAP_UHS_DDR50 | MMC_CAP_1_8V_DDR;
}

static void lg1k_set_uhs_signaling(struct sdhci_host *host, unsigned uhs)
{
	u16 ctrl_2;

	ctrl_2 = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	/* Select Bus Speed Mode for host */
	ctrl_2 &= ~SDHCI_CTRL_UHS_MASK;
	ctrl_2 &= ~SDHCI_CTRL_VDD_180;

	if (uhs == MMC_TIMING_MMC_HS200)
		ctrl_2 |= (SDHCI_CTRL_UHS_SDR104 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_UHS_SDR12)
		ctrl_2 |= SDHCI_CTRL_UHS_SDR12;
	else if (uhs == MMC_TIMING_UHS_SDR25)
		ctrl_2 |= SDHCI_CTRL_UHS_SDR25;
	else if (uhs == MMC_TIMING_UHS_SDR50)
		ctrl_2 |= (SDHCI_CTRL_UHS_SDR50 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_UHS_SDR104)
		ctrl_2 |= (SDHCI_CTRL_UHS_SDR104 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_MMC_DDR52)
		ctrl_2 |= (SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_UHS_DDR50)
		ctrl_2 |= (SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180);

	sdhci_writew(host, ctrl_2, SDHCI_HOST_CONTROL2);

	dev_dbg(mmc_dev(host->mmc),
		"%s uhs = %d, ctrl_2 = %04X\n",
		__func__, uhs, ctrl_2);

}

#ifdef CONFIG_OF
static void lg1k_50_set_tab_strobe(struct sdhci_host *host)
{
	unsigned int reg;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	if (drv_data->host_rev == EMMC_HOST_V1) {
		reg = readl(drv_data->top_reg + drv_data->offset_dll);

		if (((reg >> 4) & 0xF) == drv_data->strb)
			return;

		reg &= 0xFFFFFF0F;
		reg |= (drv_data->strb << 4);
		writel(reg, drv_data->top_reg + drv_data->offset_dll);

		mdelay(1);
	} else if ((drv_data->host_rev == EMMC_HOST_V2) || \
				(drv_data->host_rev == EMMC_HOST_V3)) {

		reg = readl(drv_data->top_reg + drv_data->offset_delay);

		if ((reg & 0xF) == drv_data->strb)
			return;

		reg &= 0xFFFFFFF0;
		reg |= drv_data->strb;
		writel(reg, drv_data->top_reg + drv_data->offset_delay);

		mdelay(1);
	}
}

static void lg1k_50_set_trm_n_dll(struct sdhci_host *host)
{
	unsigned int reg;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	reg = readl(drv_data->top_reg + drv_data->offset_dll);

	if (drv_data->host_rev == EMMC_HOST_V1) {
		// Set DLL IFF & TRM ICP
		if (((reg & 0xF) == drv_data->trm_icp) && (((reg >> 8) & 0x7) == drv_data->dll_iff))
			return;

		reg &= 0xFFFFF8F0;
		reg |= drv_data->trm_icp & 0xF;
		reg |= (drv_data->dll_iff << 8);
	} else if ((drv_data->host_rev == EMMC_HOST_V2) || \
				(drv_data->host_rev == EMMC_HOST_V3)) {
		if ((reg & 0xF) == drv_data->trm_icp)
			return;

		reg &= 0xFFFFFFF0;
		reg |= drv_data->trm_icp & 0xF;
	}

	writel(reg, drv_data->top_reg + drv_data->offset_dll);

	mdelay(1);
}

static void lg1k_50_disable_dll(struct sdhci_host *host)
{
	unsigned int reg;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	reg = readl(drv_data->top_reg + drv_data->offset_dll);

	if (drv_data->host_rev == EMMC_HOST_V1)
		reg &= 0xFFFFF7FF;
	else if ((drv_data->host_rev == EMMC_HOST_V2) || \
			(drv_data->host_rev == EMMC_HOST_V3))
		reg &= 0xFFFFFF7F;

	writel(reg, drv_data->top_reg + drv_data->offset_dll);
	mdelay(1);
}

static void lg1k_50_enable_dll(struct sdhci_host *host)
{
	unsigned int reg;
	unsigned int timeout = 10;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	lg1k_50_disable_dll(host);
	reg = readl(drv_data->top_reg + drv_data->offset_dll);

	if (drv_data->host_rev == EMMC_HOST_V1)
		reg |= 0x00000800;
	else if ((drv_data->host_rev == EMMC_HOST_V2) || \
			(drv_data->host_rev == EMMC_HOST_V3))
		reg |= 0x00000080;

	writel(reg, drv_data->top_reg + drv_data->offset_dll);

	while (timeout--) {
		if (readl(drv_data->top_reg + drv_data->offset_rdy) & 1)
			break;
		mdelay(1);
	}
}

static void lg1k_50_set_ds(struct sdhci_host *host, unsigned int ds)
{
	unsigned int reg;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	reg = readl(drv_data->top_reg + drv_data->offset_dll);
	if ((reg & 0x700000) == (ds << 20))
		return;

	reg &= 0xFF8FFFFF;
	reg |= (ds << 20);
	writel(reg, drv_data->top_reg + drv_data->offset_dll);
}


static void lg1k_50_autotune_disable(struct sdhci_host *host)
{
	unsigned short reg = 0;

	reg = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	reg &= 0xFF7F;
	sdhci_writew(host, reg, SDHCI_HOST_CONTROL2);
}

static void lg1k_50_set_intab(struct sdhci_host *host, unsigned int in,
				 unsigned int enable)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

#define IN_TAB_ENABLE			0x00000100
#define IN_TAB_CHANGE_WINDOW	0x00000200
#define IN_TAB_DISABLE			0xFFFFFE00
#define IN_TAB_SHIFT			0
#define IN_TAB_MASK				0xFFFFFF00

	unsigned int tab = 0;

	tab = readl(drv_data->top_reg + drv_data->offset_tab);
	tab &= IN_TAB_DISABLE;
	writel(tab, drv_data->top_reg + drv_data->offset_tab);

	if (enable) {
		tab |= IN_TAB_CHANGE_WINDOW;
		writel(tab, drv_data->top_reg + drv_data->offset_tab);

		tab &= IN_TAB_MASK;
		tab |= (in << IN_TAB_SHIFT);
		writel(tab, drv_data->top_reg + drv_data->offset_tab);

		tab &= ~IN_TAB_CHANGE_WINDOW;
		writel(tab, drv_data->top_reg + drv_data->offset_tab);

		tab |= IN_TAB_ENABLE;
		writel(tab, drv_data->top_reg + drv_data->offset_tab);
	}
}

static void lg1k_50_set_outtab(struct sdhci_host *host, unsigned int out,
				unsigned int enable)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

#define OUT_TAB_ENABLE		0x00100000
#define OUT_TAB_DISABLE		0xFFEFFFFF
#define OUT_TAB_SHIFT		16
#define OUT_TAB_MASK		0xFFF0FFFF

	unsigned int tab = 0;

	tab = readl(drv_data->top_reg + drv_data->offset_tab);

	if (enable) {
		tab &= OUT_TAB_MASK;
		tab |= (OUT_TAB_ENABLE | (out << OUT_TAB_SHIFT));
	} else {
		tab &= OUT_TAB_DISABLE;
	}

	writel(tab, drv_data->top_reg + drv_data->offset_tab);
}

static void lg1k_50_set_tab(struct sdhci_host *host)
{
	u16 ctrl2;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	ctrl2 = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	switch (ctrl2 & SDHCI_CTRL_UHS_MASK)
	{
		case SDHCI_CTRL_HS400:
			lg1k_50_set_outtab(host, drv_data->hs400_out, 1);
			if (drv_data->hs200_in == drv_data->hs400_in)
				lg1k_50_set_intab(host, 0, 0);
			else {
				lg1k_50_autotune_disable(host);
				lg1k_50_set_intab(host, drv_data->hs400_in, 1);
			}
			break;
		case SDHCI_CTRL_UHS_SDR104:
			if (host->clock == 52000000) {
				lg1k_50_set_outtab(host, drv_data->ddr50_out, 1);
				lg1k_50_set_intab(host, drv_data->ddr50_in, 1);
			} else {
				lg1k_50_set_outtab(host, drv_data->hs200_out, 1);
				lg1k_50_set_intab(host, 0, 0);
			}
			break;
		case SDHCI_CTRL_UHS_DDR50:
			lg1k_50_set_outtab(host, drv_data->ddr50_out, 1);
			lg1k_50_set_intab(host, drv_data->ddr50_in, 1);
			break;
		case SDHCI_CTRL_UHS_SDR25:
			if ((drv_data->host_rev == EMMC_HOST_V2) || \
				(drv_data->host_rev == EMMC_HOST_V3)) {
				lg1k_50_set_outtab(host, drv_data->ddr50_out, 1);
				lg1k_50_set_intab(host, drv_data->ddr50_in, 1);
			}
			break;
		default :
			break;
	}
}

static void lg1k_50_unset_tab(struct sdhci_host *host)
{
	lg1k_50_set_outtab(host, 0, 0);
	lg1k_50_set_intab(host, 0, 0);
}

static void lg1k_50_set_uhs_signaling(struct sdhci_host *host, unsigned int uhs)
{
	u16 ctrl2;

	ctrl2 = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	/* Select Bus Speed Mode for host */
	ctrl2 &= ~SDHCI_CTRL_UHS_MASK;
	ctrl2 &= ~SDHCI_CTRL_VDD_180;

	if (uhs == MMC_TIMING_MMC_HS400)
		ctrl2 |= (SDHCI_CTRL_HS400 | SDHCI_CTRL_VDD_180);
	else if ((uhs == MMC_TIMING_MMC_HS200) ||
		(uhs == MMC_TIMING_UHS_SDR104))
		ctrl2 |= (SDHCI_CTRL_UHS_SDR104 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_UHS_SDR12)
		ctrl2 |= SDHCI_CTRL_UHS_SDR12;
	else if (uhs == MMC_TIMING_UHS_SDR25)
		ctrl2 |= SDHCI_CTRL_UHS_SDR25;
	else if (uhs == MMC_TIMING_UHS_SDR50)
		ctrl2 |= (SDHCI_CTRL_UHS_SDR50 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_MMC_DDR52)
		ctrl2 |= (SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_UHS_DDR50)
		ctrl2 |= (SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180);
	else if (uhs == MMC_TIMING_MMC_HS)
		ctrl2 |= SDHCI_CTRL_UHS_SDR25;

	sdhci_writew(host, ctrl2, SDHCI_HOST_CONTROL2);

	ctrl2 = sdhci_readw(host, SDHCI_HOST_CONTROL2) & SDHCI_CTRL_UHS_MASK;

	if (ctrl2 == SDHCI_CTRL_HS400) {
		lg1k_50_set_tab_strobe(host);
		lg1k_set_inited(host, 1);
	}

	if ((ctrl2 == SDHCI_CTRL_UHS_SDR25) || (ctrl2 == SDHCI_CTRL_UHS_SDR104) ||
		(ctrl2 == SDHCI_CTRL_UHS_DDR50) || (ctrl2 == SDHCI_CTRL_HS400)) {
		lg1k_50_enable_dll(host);
		lg1k_50_set_tab(host);
	} else {
		lg1k_50_disable_dll(host);
		lg1k_50_unset_tab(host);
	}

	return;
}

static unsigned int lg1k_451_get_min_clock(struct sdhci_host *host)
{
	return 398000;
}

static unsigned int lg1k_451_get_max_clock(struct sdhci_host *host)
{
	return 198000000;
}

static unsigned int lg1k_50_get_max_clock(struct sdhci_host *host)
{
	unsigned int clk;
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);

	clk = pltfm_host->clock;

	if (clk)
		return clk;
	else
		return 198000000;
}

static void lg1k_451_set_clock(struct sdhci_host *host, unsigned int clock)
{
	int div = 0; /* Initialized for compiler warning */
	u16 clk = 0;
	unsigned long timeout;

	host->mmc->actual_clock = 0;

	sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		return;

	if (clock > (host->max_clk / 2))
		div = 0;
	else {
		for (div = 1; div <= 0x3FF; div *= 2)
			if ((host->max_clk / (div * 2)) <= clock)
				break;
	}

	clk |= (div & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
	clk |= ((div & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN)
		<< SDHCI_DIVIDER_HI_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	timeout = 20;
	while (!((clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			pr_err("%s: Internal clock never "
				"stabilised.\n", mmc_hostname(host->mmc));
			return;
		}
		timeout--;
		mdelay(1);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void lg1k_50_select_standardcell(struct sdhci_host *host)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);
	unsigned short reg;

	reg = readl(drv_data->top_reg + drv_data->offset_delay);
	reg &= 0xffffffcf;

	if (drv_data->host_rev == EMMC_HOST_V3) {
		writel(0x1, drv_data->top_reg + EMMC_DLL_SELECT);
		reg |= 0x10;
	} else
		reg |= 0x30;

	writel(reg, drv_data->top_reg + drv_data->offset_delay);

	return;
}

static void lg1k_50_select_dll(struct sdhci_host *host)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);
	unsigned short reg;

	if (drv_data->host_rev == EMMC_HOST_V3)
		writel(0x0, drv_data->top_reg + EMMC_DLL_SELECT);

	reg = readl(drv_data->top_reg + drv_data->offset_delay);
	reg &= 0xffffffcf;
	writel(reg, drv_data->top_reg + drv_data->offset_delay);

	return;
}

static void lg1k_50_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);
	unsigned short reg;

	lg1k_50_set_trm_n_dll(host);

	lg1k_451_set_clock(host, clock);

	reg = sdhci_readw(host, SDHCI_HOST_CONTROL2) & SDHCI_CTRL_UHS_MASK;

	if (reg == SDHCI_CTRL_HS400)
		lg1k_50_set_tab_strobe(host);

	if ((reg == SDHCI_CTRL_UHS_SDR25) || (reg == SDHCI_CTRL_UHS_SDR104) ||
		(reg == SDHCI_CTRL_UHS_DDR50) || (reg == SDHCI_CTRL_HS400)) {
		lg1k_50_enable_dll(host);
		lg1k_50_set_tab(host);
	} else {
		if ((drv_data->host_rev == EMMC_HOST_V2) || \
			(drv_data->host_rev == EMMC_HOST_V3)) {
			lg1k_50_set_outtab(host, 15, 1);
			lg1k_50_set_intab(host, 0, 0);
		} else {
			lg1k_50_unset_tab(host);
		}
		lg1k_50_disable_dll(host);
	}

	if ((drv_data->host_rev == EMMC_HOST_V2) || \
		(drv_data->host_rev == EMMC_HOST_V3)) {
		if (clock <= 52000000) {
			lg1k_50_select_standardcell(host);
		} else {
			lg1k_50_select_dll(host);
		}
	}

	if (clock >= 52000000)
		lg1k_50_set_ds(host, drv_data->host_ds);
	else
		lg1k_50_set_ds(host, 0);
}

static void lg1k_50_forced_fail_do(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_HOST_CONTROL);
	reg &= 0xFFFFFFDD;
	reg |= 0x02;
	sdhci_writel(host,reg,SDHCI_HOST_CONTROL);
}

static void lg1k_50_forced_fail_cancel(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_HOST_CONTROL);
	reg &= 0xFFFFFFDD;
	reg |= 0x20;
	sdhci_writel(host,reg,SDHCI_HOST_CONTROL);
}

static int lg1k_50_execute_tuning(struct sdhci_host *host, unsigned int opcode)
{
	unsigned short ctrl;
	unsigned int ier;
	unsigned long timeout;
	int err = 0;
	unsigned int reg;
	unsigned int i;
	unsigned int data_null;
	unsigned int execute_loop = 0;
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);

	lg1k_set_inited(host, 0);

	lg1k_50_set_outtab(host, drv_data->hs200_out, 1);
	lg1k_50_set_intab(host, 0, 0);

	ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	ctrl |= SDHCI_CTRL_EXEC_TUNING;
	sdhci_writew(host, ctrl, SDHCI_HOST_CONTROL2);

	/*
	 * As per the Host Controller spec v3.00, tuning command
	 * generates Buffer Read Ready interrupt, so enable that.
	 *
	 * Note: The spec clearly says that when tuning sequence
	 * is being performed, the controller does not generate
	 * interrupts other than Buffer Read Ready interrupt. But
	 * to make sure we don't hit a controller bug, we _only_
	 * enable Buffer Read Ready interrupt here.
	 */
	ier = sdhci_readl(host, SDHCI_INT_ENABLE);
	sdhci_writel(host, SDHCI_INT_DATA_AVAIL, SDHCI_INT_ENABLE);
	sdhci_writel(host, SDHCI_INT_DATA_AVAIL, SDHCI_SIGNAL_ENABLE);

	/*
	 * Issue CMD19 repeatedly till Execute Tuning is set to 0 or the number
	 * of loops reaches 40 times or a timeout of 150ms occurs.
	 */
	timeout = 150;
	do {
		struct mmc_command cmd = {0};
		struct mmc_request mrq = {NULL};

		execute_loop++;

		if (execute_loop > 40) {
			pr_err("%s:execute tune loop over 40!\n", mmc_hostname(host->mmc));
			break;
		}

		// must to fail (32~39)
		if (execute_loop > 32)
			lg1k_50_forced_fail_do(host);

		cmd.opcode = opcode;
		cmd.arg = 0;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd.retries = 0;
		cmd.data = NULL;
		cmd.error = 0;

		mrq.cmd = &cmd;
		host->mrq = &mrq;

		/*
		 * In response to CMD19, the card sends 64 bytes of tuning
		 * block to the Host Controller. So we set the block size
		 * to 64 here.
		 */
		if (cmd.opcode == MMC_SEND_TUNING_BLOCK_HS200) {
			sdhci_writew(host, SDHCI_MAKE_BLKSZ(7, 128),
				SDHCI_BLOCK_SIZE);
		} else {
			sdhci_writew(host, SDHCI_MAKE_BLKSZ(7, 64),
				SDHCI_BLOCK_SIZE);
		}

		/*
		 * The tuning block is sent by the card to the host controller.
		 * So we set the TRNS_READ bit in the Transfer Mode register.
		 * This also takes care of setting DMA Enable and Multi Block
		 * Select in the same register to 0.
		 */
		sdhci_writew(host, SDHCI_TRNS_READ, SDHCI_TRANSFER_MODE);

		sdhci_send_command(host, &cmd);

		host->cmd = NULL;
		host->mrq = NULL;

		spin_unlock_irqrestore(&host->lock, flags);
		/* Wait for Buffer Read Ready interrupt */
		wait_event_interruptible_timeout(host->buf_ready_int,
					(host->tuning_done == 1),
					msecs_to_jiffies(50));
		spin_lock_irqsave(&host->lock, flags);

		if (!host->tuning_done) {
			pr_err("%s : Timeout waiting for "
				"Buffer Read Ready interrupt during tuning "
				"procedure, falling back to fixed sampling "
				"clock\n", mmc_hostname(host->mmc));

			ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
			ctrl &= ~SDHCI_CTRL_TUNED_CLK;
			ctrl &= ~SDHCI_CTRL_EXEC_TUNING;
			sdhci_writew(host, ctrl, SDHCI_HOST_CONTROL2);

			err = -EIO;
			goto out;
		}

		host->tuning_done = 0;

		for (i = 0; i < (128/4); i++) {
			data_null = sdhci_readl(host, SDHCI_BUFFER);
		}

		ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
		timeout--;
	} while (ctrl & SDHCI_CTRL_EXEC_TUNING);

	lg1k_50_forced_fail_cancel(host);

	if ((ctrl & SDHCI_CTRL_TUNED_CLK) == 0)
		err = -EIO;

	sdhci_writeb(host, SDHCI_RESET_DATA, SDHCI_SOFTWARE_RESET);
	sdhci_writeb(host, SDHCI_RESET_CMD, SDHCI_SOFTWARE_RESET);
	sdhci_writel(host, SDHCI_INT_DATA_AVAIL, SDHCI_INT_STATUS);

	sdhci_writel(host, ier, SDHCI_INT_ENABLE);
	sdhci_writel(host, ier, SDHCI_SIGNAL_ENABLE);

	reg = readl(drv_data->top_reg + drv_data->offset_rdy);
	if (drv_data->host_rev == EMMC_HOST_V1)
		pr_notice("sdhci-lg1k : tuned intab value : %u\n", (reg >> 8) & 0x1F);
	else if ((drv_data->host_rev == EMMC_HOST_V2) || \
			(drv_data->host_rev == EMMC_HOST_V3))
		pr_notice("sdhci-lg1k : tuned intab value : %u\n", (reg >> 12) & 0x1F);

	if (drv_data->host_rev == EMMC_HOST_V3)
		sdhci_writel(host, 0x1, EMMC_CLOCK_GATING);

out:
	spin_unlock_irqrestore(&host->lock, flags);

	lg1k_set_inited(host, 1);

	return err;
}

int lg1k_50_select_drive_strength(struct sdhci_host *host,
				 struct mmc_card *card,
				 unsigned int max_dtr, int host_drv,
				 int card_drv, int *drv_type)
{
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	return drv_data->device_ds;
}

#endif

static struct sdhci_ops lg1k_441_sdhci_ops = {
	.platform_init = lg1k_441_platform_init,
	.set_clock = sdhci_set_clock,
	.reset = sdhci_reset,
	.set_uhs_signaling = lg1k_set_uhs_signaling,
	.set_bus_width = sdhci_set_bus_width,
};

#ifdef CONFIG_OF
static struct sdhci_ops lg1k_451_sdhci_ops = {
	.platform_init = lg1k_451_platform_init,
	.set_clock = lg1k_451_set_clock,
	.reset = sdhci_reset,
	.set_uhs_signaling = lg1k_set_uhs_signaling,
	.set_bus_width = sdhci_set_bus_width,
	.get_min_clock = lg1k_451_get_min_clock,
	.get_max_clock = lg1k_451_get_max_clock,
};

static struct sdhci_ops lg1k_50_sdhci_ops = {
	.platform_init = lg1k_50_platform_init,
	.set_clock = lg1k_50_set_clock,
	.reset = sdhci_reset,
	.set_bus_width = sdhci_set_bus_width,
	.get_min_clock = lg1k_451_get_min_clock,
	.get_max_clock = lg1k_50_get_max_clock,
	.set_uhs_signaling = lg1k_50_set_uhs_signaling,
	.platform_execute_tuning = lg1k_50_execute_tuning,
};

static struct sdhci_ops lg1k_50_sdhci_ops_v3 = {
	.platform_init = lg1k_50_platform_init,
	.set_clock = lg1k_50_set_clock,
	.reset = sdhci_reset,
	.set_bus_width = sdhci_set_bus_width,
	.get_min_clock = lg1k_451_get_min_clock,
	.get_max_clock = lg1k_50_get_max_clock,
	.set_uhs_signaling = lg1k_50_set_uhs_signaling,
	.platform_execute_tuning = lg1k_50_execute_tuning,
	.select_drive_strength = lg1k_50_select_drive_strength,
	.read_l = lg1k_readl,
	.read_w = lg1k_readw,
	.read_b = lg1k_readb,
	.write_w = lg1k_writew,
	.write_b = lg1k_writeb,
};

#endif

static struct sdhci_pltfm_data sdhci_lg1k_441_pdata = {
	.ops  = &lg1k_441_sdhci_ops,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL,
};

#ifdef CONFIG_OF
static struct sdhci_pltfm_data sdhci_lg1k_451_pdata = {
	.ops  = &lg1k_451_sdhci_ops,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL
			| SDHCI_QUIRK_FORCE_BLK_SZ_2048
			| SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN
			| SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK,
};

static struct sdhci_pltfm_data sdhci_lg1k_451_sd_pdata = {
	.ops  = &lg1k_451_sdhci_ops,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL
			| SDHCI_QUIRK_FORCE_BLK_SZ_2048
			| SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12
			| SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN
			| SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK,
	.quirks2 = SDHCI_QUIRK2_NO_1_8_V,
};

static struct sdhci_pltfm_data sdhci_lg1k_50_pdata = {
	.ops  = &lg1k_50_sdhci_ops,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL
			| SDHCI_QUIRK_FORCE_BLK_SZ_2048
			| SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN
			| SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK,
};

static struct sdhci_pltfm_data sdhci_lg1k_50_pdata_v3 = {
	.ops  = &lg1k_50_sdhci_ops_v3,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL
			| SDHCI_QUIRK_FORCE_BLK_SZ_2048
			| SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN
			| SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK,
	.quirks2 = SDHCI_QUIRK2_PRESET_VALUE_BROKEN,
};

#endif

static int sdhci_lg1k_probe(struct platform_device *pdev)
{
#ifdef CONFIG_OF
	int ret = -1;
	struct device_node *np = pdev->dev.of_node;
	if (of_device_is_compatible(np, "lge,lg1k-sdhci-5.0"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_50_pdata, 0);
	else if (of_device_is_compatible(np, "lge,lg1k-sdhci-5.0-v2"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_50_pdata, 0);
	else if (of_device_is_compatible(np, "lge,lg1k-sdhci-5.0-v3"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_50_pdata_v3, 0);
	else if (of_device_is_compatible(np, "lge,lg1154-sdhci")
		|| of_device_is_compatible(np, "lge,lg1k-sdhci-4.41"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_441_pdata, 0);
	else if (of_device_is_compatible(np, "lge,lg1156-sdhci")
		|| of_device_is_compatible(np, "lge,lg1k-sdhci-4.51"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_451_pdata, 0);
	else if (of_device_is_compatible(np, "lge,lg1k-sdhci-4.51-sd"))
		ret = sdhci_pltfm_register(pdev, &sdhci_lg1k_451_sd_pdata, 0);
	else
		dev_err(&pdev->dev, "Can't find compatible device \n");

	return ret;
#else
	return sdhci_pltfm_register(pdev, &sdhci_lg1k_441_pdata);
#endif
}

static int sdhci_lg1k_remove(struct platform_device *pdev)
{
	return sdhci_pltfm_unregister(pdev);
}

static struct platform_device_id sdhci_lg1k_driver_ids[] = {
	{
		.name		= "sdhci-lg1k",
		.driver_data	= (kernel_ulong_t)&lg1k_441_sdhci_drv_data,
	},
	{}
};
MODULE_DEVICE_TABLE(platform, sdhci_lg1k_driver_ids);

static const struct of_device_id sdhci_lg1k_dt_ids[] = {
	{ .compatible = "lge,lg1154-sdhci",
		.data = (void *)&lg1k_441_sdhci_drv_data },
	{ .compatible = "lge,lg1156-sdhci",
		.data = (void *)&lg1k_451_sdhci_drv_data },
	{ .compatible = "lge,lg1k-sdhci-4.51",
		.data = (void *)&lg1k_451_sdhci_drv_data },
	{ .compatible = "lge,lg1k-sdhci-4.51-sd",
		.data = (void *)&lg1k_451_sd_sdhci_drv_data },
	{ .compatible = "lge,lg1k-sdhci-4.41",
		.data = (void *)&lg1k_441_sdhci_drv_data },
	{ .compatible = "lge,lg1k-sdhci-5.0",
		.data = (void *)&lg1k_50_sdhci_drv_data },
	{ .compatible = "lge,lg1k-sdhci-5.0-v2",
		.data = (void *)&lg1k_50_sdhci_v2_drv_data },
	{ .compatible = "lge,lg1k-sdhci-5.0-v3",
		.data = (void *)&lg1k_50_sdhci_v3_drv_data },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sdhci_lg1k_dt_ids);

static int lg1k_sdhci_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	if (drv_data->pm_enable) {
		lg1k_set_inited(host, 0);
		return sdhci_suspend_host(host);
	} else
		return 0;
}

static int lg1k_sdhci_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(host->mmc->parent);
	struct sdhci_lg1k_drv_data *drv_data = sdhci_lg1k_get_driver_data(pdev);

	if (drv_data->pm_enable) {
		lg1k_set_inited(host, 0);
		return sdhci_resume_host(host);
	}
	else
		return 0;
}

SIMPLE_DEV_PM_OPS(lg1k_sdhci_pm_ops, lg1k_sdhci_suspend, lg1k_sdhci_resume);


static struct platform_driver sdhci_driver = {
	.id_table	= sdhci_lg1k_driver_ids,
	.driver = {
		.name	= "sdhci-lg1k",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(sdhci_lg1k_dt_ids),
		.pm = &lg1k_sdhci_pm_ops,
	},
	.probe		= sdhci_lg1k_probe,
	.remove		= sdhci_lg1k_remove,
};

module_platform_driver(sdhci_driver);

MODULE_DESCRIPTION("LG1XXX Secure Digital Host Controller Interface driver");
MODULE_AUTHOR("Chanho Min <chanho.min@lge.com>, "
		"Hankyung Yu <hankyung.yu@lge.com>");
MODULE_LICENSE("GPL v2");
