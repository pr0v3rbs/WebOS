/**
 * @file hdmi_rx_phy.h
 * @brief HDMI RX PHY driver
 *
 * Copyright (C) 2010 Synopsys, Inc. All rights reserved.
 *
 * @version 1.0 first release
 */
#ifndef HDMI_RX_PHY_H_
#define HDMI_RX_PHY_H_

#include <linux/errno.h>
//#include <stdbool.h>
//#include <stdint.h>
#include <linux/types.h>
/** The following define is for SYNOPSYS testchip
 * Please comment if this driver is to work with a SoC and/or
 * the PHY hard macro straight through the DWC_HDMI_RX_CTRL registers */
//#define TESTCHIP
#define PHY_GEN3

/**
 * @short HDMI RX PHY context information
 *
 * Initialize @b user fields (set status to zero).
 * After opening this data is for internal use only.
 */
struct hdmi_rx_phy
{
	int version;
	int generation;
	/** (@b user) Context status: closed (0), opened (<0) and configured (>0) */
	int status;
	/** (@b user) Configuration clock frequency [kHz], valid range 10MHz to 160MHz */
	unsigned long cfg_clk;
	/** Peaking configuration */
	uint16_t peaking;
	/** PLL configuration */
	uint32_t pll_cfg;
	/** (@b user) Log errors */
	void (*log_error)(const char *file, int line, const char *fmt, ...);
	/** (@b user) Log information */
	void (*log_info)(const char *fmt, ...);
	/** (@b user) Board support package, reset device - external */
	void (*bsp_reset)(bool enable);
#ifdef PHY_GEN3
	/** (@b user) Board support package, control PDDQ signal - external/controller */
	void (*bsp_pddq)(int enable);
	/** (@b user) Board support package, control SVS mode signal - external/controller */
	void (*bsp_svsmode)(int enable);
	/** (@b user) Board support package, control zcal reset signal - external/controller */
	void (*bsp_zcal_reset)(int enable);
	/** (@b user) Board support package, zcal done signal status - external/controller */
	uint16_t (*bsp_zcal_done)(void);
	/** (@b user) Board support package, configuration callback to prepare
	 * infrastructure/board for PHY configuration if needed */
	int (*bsp_config)(unsigned long tmds);
#endif
#ifdef TESTCHIP
	/** (@b user) Board support package, register read access */
	uint16_t (*bsp_read)(uint8_t addr);
	/** (@b user) Board support package, register write access */
	void (*bsp_write)(uint8_t addr, uint16_t data);
#else
	/** (@b user) Board support package, register read access */
	uint32_t (*bsp_read)(uint16_t addr);
	/** (@b user) Board support package, register write access */
	void (*bsp_write)(uint16_t addr, uint32_t data);
#endif

};

int hdmi_rx_phy_open(struct hdmi_rx_phy *phy);
int hdmi_rx_phy_config(struct hdmi_rx_phy *ctx, unsigned long tclk, unsigned dcm);
int hdmi_rx_phy_config_and_reset(struct hdmi_rx_phy *ctx, unsigned port, unsigned long tclk, unsigned dcm);
int hdmi_rx_phy_debug(struct hdmi_rx_phy *phy);
void hdmi_rx_phy_fast_switching(struct hdmi_rx_phy *ctx, int enable);
int hdmi_rx_phy_close(struct hdmi_rx_phy *phy);
int hdmi_rx_phy_try_force_close(struct hdmi_rx_phy *phy);

#endif /* HDMI_RX_PHY_H_ */
