/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/** @file
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author		juhee.park
 *  @version	1.0
 *  @date		2011-04-01
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "vport_i2c_l9a0.h"
#include "sys_regs.h"
#include <linux/kernel.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

L9A_VERSION_0_T gl9a_version_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x00,
};

HOSTIF_CONTROL_0_T ghostif_control_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x01,
};

L9A_HOSTIF_INTR_0_T gl9a_hostif_intr_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x02,
};

L9A_HOSTIF_INTR_EN_0_T gl9a_hostif_intr_en_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x03,
};

TOP_CONTROL_0_T gtop_control_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x04,
};

TOP_CONTROL_1_T gtop_control_1 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x05,
};

TOP_CONTROL_2_T gtop_control_2 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x06,
};

SOFT_RESET_0_T gsoft_reset_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x07,
};

SOFT_RESET_1_T gsoft_reset_1 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x08,
};

SOFT_RESET_2_T gsoft_reset_2 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x09,
};

SOFT_RESET_3_T gsoft_reset_3 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0A,
};

SOFT_RESET_4_T gsoft_reset_4 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0B,
};

SOFT_RESET_5_T gsoft_reset_5 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0C,
};

SOFT_RESET_6_T gsoft_reset_6 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0D,
};

SOFT_RESET_7_T gsoft_reset_7 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0E,
};

SOFT_RESET_8_T gsoft_reset_8 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0F,
};

SOFT_RESET_9_T gsoft_reset_9 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x10,
};

SOFT_RESET_10_T gsoft_reset_10 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x11,
};

SOFT_RESET_11_T gsoft_reset_11 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x12,
};

SOFT_RESET_12_T gsoft_reset_12 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x13,
};

SOFT_RESET_13_T gsoft_reset_13 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x14,
};

SOFT_RESET_14_T gsoft_reset_14 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x15,
};

SOFT_RESET_15_T gsoft_reset_15 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x16,
};

SOFT_RESET_16_T gsoft_reset_16 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x17,
};

SOFT_RESET_17_T gsoft_reset_17 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x18,
};

CLOCK_CONTROL_0_T gclock_control_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x19,
};

CLOCK_CONTROL_1_T gclock_control_1 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1A,
};

CLOCK_CONTROL_2_T gclock_control_2 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1B,
};

AUDIO_DTO_0_T gaudio_dto_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1C,
};

AUDIO_DTO_1_T gaudio_dto_1 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1D,
};

AUDIO_DTO_2_T gaudio_dto_2 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1E,
};

AUDIO_DTO_3_T gaudio_dto_3 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1F,
};

AUDIO_DTO_4_T gaudio_dto_4 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x20,
};

AUDIO_DTO_5_T gaudio_dto_5 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x21,
};

AUDIO_DTO_6_T gaudio_dto_6 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x22,
};

AUDIO_DTO_7_T gaudio_dto_7 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x23,
};

AUDIO_DTO_8_T gaudio_dto_8 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x24,
};

DATA_BRIDGE_0_T gdata_bridge_0 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x25,
};

DATA_BRIDGE_1_T gdata_bridge_1 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x26,
};

DATA_BRIDGE_2_T gdata_bridge_2 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x27,
};

DATA_BRIDGE_3_T gdata_bridge_3 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x28,
};

DATA_BRIDGE_4_T gdata_bridge_4 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x29,
};

DATA_BRIDGE_5_T gdata_bridge_5 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2A,
};

DATA_BRIDGE_6_T gdata_bridge_6 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2B,
};

DATA_BRIDGE_7_T gdata_bridge_7 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2C,
};

DATA_BRIDGE_8_T gdata_bridge_8 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2D,
};

DATA_BRIDGE_9_T gdata_bridge_9 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2E,
};

DATA_BRIDGE_10_T gdata_bridge_10 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2F,
};

DATA_BRIDGE_11_T gdata_bridge_11 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x30,
};

DATA_BRIDGE_12_T gdata_bridge_12 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x31,
};

DATA_BRIDGE_13_T gdata_bridge_13 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x32,
};

DATA_BRIDGE_14_T gdata_bridge_14 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x33,
};

DATA_BRIDGE_15_T gdata_bridge_15 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x34,
};

DATA_BRIDGE_16_T gdata_bridge_16 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x35,
};

DATA_BRIDGE_17_T gdata_bridge_17 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x36,
};

DATA_BRIDGE_18_T gdata_bridge_18 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x37,
};

DATA_BRIDGE_19_T gdata_bridge_19 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x38,
};

DATA_BRIDGE_20_T gdata_bridge_20 =
{
	.slaveAddr = 0x10,
	.regAddr = 0x39,
};

HSLVDSTX0_0_T ghslvdstx0_0 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x00,
};

HSLVDSTX0_1_T ghslvdstx0_1 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x01,
};

HSLVDSTX0_2_T ghslvdstx0_2 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x02,
};

HSLVDSTX0_3_T ghslvdstx0_3 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x03,
};

HSLVDSTX0_4_T ghslvdstx0_4 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x04,
};

HSLVDSTX0_5_T ghslvdstx0_5 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x05,
};

HSLVDSTX0_6_T ghslvdstx0_6 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x06,
};

HSLVDSTX1_0_T ghslvdstx1_0 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x07,
};

HSLVDSTX1_1_T ghslvdstx1_1 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x08,
};

HSLVDSTX1_2_T ghslvdstx1_2 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x09,
};

HSLVDSTX1_3_T ghslvdstx1_3 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0A,
};

HSLVDSTX1_4_T ghslvdstx1_4 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0B,
};

HSLVDSTX1_5_T ghslvdstx1_5 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0C,
};

HSLVDSTX1_6_T ghslvdstx1_6 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0D,
};

HSLVDSTX2_0_T ghslvdstx2_0 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0E,
};

HSLVDSTX2_1_T ghslvdstx2_1 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0F,
};

HSLVDSTX2_2_T ghslvdstx2_2 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x10,
};

HSLVDSTX2_3_T ghslvdstx2_3 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x11,
};

HSLVDSTX2_4_T ghslvdstx2_4 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x12,
};

HSLVDSTX2_5_T ghslvdstx2_5 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x13,
};

HSLVDSTX2_6_T ghslvdstx2_6 =
{
	.slaveAddr = 0x11,
	.regAddr = 0x14,
};

GBB_AFE_0_T ggbb_afe_0 =
{
	.slaveAddr = 0x12,
	.regAddr = 0x00,
};

GBB_0_T ggbb_0 =
{
	.slaveAddr = 0x12,
	.regAddr = 0x01,
};

AFE_CVBS_0_T gafe_cvbs_0 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x00,
};

AFE_CVBS_1_T gafe_cvbs_1 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x01,
};

AFE_CVBS_2_T gafe_cvbs_2 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x02,
};

AFE_CVBS_3_T gafe_cvbs_3 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x03,
};

AFE_VBUF_0_T gafe_vbuf_0 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x04,
};

AFE_VBUF_1_T gafe_vbuf_1 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x05,
};

AFE_VDAC_0_T gafe_vdac_0 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x06,
};

AFE_VDAC_1_T gafe_vdac_1 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x07,
};

MAIN_PLL_0_T gmain_pll_0 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x08,
};

MAIN_PLL_1_T gmain_pll_1 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x09,
};

MAIN_PLL_2_T gmain_pll_2 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0A,
};

MAIN_PLL_3_T gmain_pll_3 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0B,
};

MAIN_PLL_4_T gmain_pll_4 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0C,
};

AFE_CB_0_T gafe_cb_0 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0D,
};

AFE_CB_1_T gafe_cb_1 =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0E,
};

HPD_OUT_CONTROL_0_T ghpd_out_control_0 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x00,
};

HPD_OUT_CONTROL_1_T ghpd_out_control_1 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x01,
};

HPD_OUT_CONTROL_2_T ghpd_out_control_2 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x02,
};

HDMI_PHY0_0_T ghdmi_phy0_0 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x03,
};

HDMI_PHY0_1_T ghdmi_phy0_1 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x04,
};

HDMI_PHY0_2_T ghdmi_phy0_2 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x05,
};

HDMI_PHY1_0_T ghdmi_phy1_0 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x06,
};

HDMI_PHY1_1_T ghdmi_phy1_1 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x07,
};

HDMI_PHY1_2_T ghdmi_phy1_2 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x08,
};

HDMI_PHY1_3_T ghdmi_phy1_3 =
{
	.slaveAddr = 0x14,
	.regAddr = 0x09,
};

L9A_HDMI0_INTR_0_T gl9a_hdmi0_intr_0 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x00,
};

L9A_HDMI0_INTR_1_T gl9a_hdmi0_intr_1 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x01,
};

L9A_HDMI0_INTR_2_T gl9a_hdmi0_intr_2 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x02,
};

L9A_HDMI0_INTR_3_T gl9a_hdmi0_intr_3 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x03,
};

L9A_HDMI0_INTR_4_T gl9a_hdmi0_intr_4 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x04,
};

L9A_HDMI0_INTR_5_T gl9a_hdmi0_intr_5 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x05,
};

L9A_HDMI0_INTR_EN_0_T gl9a_hdmi0_intr_en_0 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x06,
};

L9A_HDMI0_INTR_EN_1_T gl9a_hdmi0_intr_en_1 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x07,
};

L9A_HDMI0_INTR_EN_2_T gl9a_hdmi0_intr_en_2 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x08,
};

L9A_HDMI0_INTR_EN_3_T gl9a_hdmi0_intr_en_3 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x09,
};

L9A_HDMI0_INTR_EN_4_T gl9a_hdmi0_intr_en_4 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0A,
};

L9A_HDMI0_INTR_EN_5_T gl9a_hdmi0_intr_en_5 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0B,
};

L9A_HDMI1_INTR_0_T gl9a_hdmi1_intr_0 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0C,
};

L9A_HDMI1_INTR_1_T gl9a_hdmi1_intr_1 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0D,
};

L9A_HDMI1_INTR_2_T gl9a_hdmi1_intr_2 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0E,
};

L9A_HDMI1_INTR_3_T gl9a_hdmi1_intr_3 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0F,
};

L9A_HDMI1_INTR_4_T gl9a_hdmi1_intr_4 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x10,
};

L9A_HDMI1_INTR_5_T gl9a_hdmi1_intr_5 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x11,
};

L9A_HDMI1_INTR_EN_0_T gl9a_hdmi1_intr_en_0 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x12,
};

L9A_HDMI1_INTR_EN_1_T gl9a_hdmi1_intr_en_1 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x13,
};

L9A_HDMI1_INTR_EN_2_T gl9a_hdmi1_intr_en_2 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x14,
};

L9A_HDMI1_INTR_EN_3_T gl9a_hdmi1_intr_en_3 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x15,
};

L9A_HDMI1_INTR_EN_4_T gl9a_hdmi1_intr_en_4 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x16,
};

L9A_HDMI1_INTR_EN_5_T gl9a_hdmi1_intr_en_5 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x17,
};

LINK_0_T glink_0 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x18,
};

LINK_1_T glink_1 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x19,
};

LINK_2_T glink_2 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1A,
};

LINK_3_T glink_3 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1B,
};

LINK_4_T glink_4 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1C,
};

LINK_5_T glink_5 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1D,
};

LINK_6_T glink_6 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1E,
};

LINK_7_T glink_7 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1F,
};

LINK_8_T glink_8 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x20,
};

LINK_9_T glink_9 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x21,
};

LINK_10_T glink_10 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x22,
};

LINK_11_T glink_11 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x23,
};

LINK_12_T glink_12 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x24,
};

LINK_13_T glink_13 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x25,
};

LINK_14_T glink_14 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x26,
};

LINK_15_T glink_15 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x27,
};

LINK_16_T glink_16 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x28,
};

LINK_17_T glink_17 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x29,
};

LINK_18_T glink_18 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2A,
};

LINK_19_T glink_19 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2B,
};

LINK_20_T glink_20 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2C,
};

LINK_21_T glink_21 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2D,
};

LINK_22_T glink_22 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2E,
};

LINK_23_T glink_23 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2F,
};

LINK_24_T glink_24 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x30,
};

LINK_25_T glink_25 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x31,
};

LINK_26_T glink_26 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x32,
};

LINK_27_T glink_27 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x33,
};

LINK_28_T glink_28 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x34,
};

LINK_29_T glink_29 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x35,
};

LINK_30_T glink_30 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x36,
};

LINK_31_T glink_31 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x37,
};

LINK_32_T glink_32 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x38,
};

LINK_33_T glink_33 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x39,
};

LINK_34_T glink_34 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3A,
};

LINK_35_T glink_35 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3B,
};

LINK_36_T glink_36 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3C,
};

LINK_37_T glink_37 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3D,
};

LINK_38_T glink_38 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3E,
};

LINK_39_T glink_39 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3F,
};

LINK_40_T glink_40 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x40,
};

LINK_41_T glink_41 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x41,
};

LINK_42_T glink_42 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x42,
};

LINK_43_T glink_43 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x43,
};

LINK_44_T glink_44 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x44,
};

LINK_45_T glink_45 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x45,
};

LINK_46_T glink_46 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x46,
};

LINK_47_T glink_47 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x47,
};

LINK_48_T glink_48 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x48,
};

LINK_49_T glink_49 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x49,
};

LINK_50_T glink_50 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4A,
};

LINK_51_T glink_51 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4B,
};

LINK_52_T glink_52 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4C,
};

LINK_53_T glink_53 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4D,
};

LINK_54_T glink_54 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4E,
};

LINK_55_T glink_55 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4F,
};

LINK_56_T glink_56 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x50,
};

LINK_57_T glink_57 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x51,
};

LINK_58_T glink_58 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x52,
};

LINK_59_T glink_59 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x53,
};

LINK_60_T glink_60 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x54,
};

LINK_61_T glink_61 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x55,
};

LINK_62_T glink_62 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x56,
};

LINK_63_T glink_63 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x57,
};

LINK_64_T glink_64 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x58,
};

LINK_65_T glink_65 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x59,
};

LINK_66_T glink_66 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5A,
};

LINK_67_T glink_67 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5B,
};

LINK_68_T glink_68 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5C,
};

LINK_69_T glink_69 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5D,
};

LINK_70_T glink_70 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5E,
};

LINK_71_T glink_71 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5F,
};

LINK_72_T glink_72 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x60,
};

LINK_73_T glink_73 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x61,
};

LINK_74_T glink_74 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x62,
};

LINK_75_T glink_75 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x63,
};

LINK_76_T glink_76 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x64,
};

LINK_77_T glink_77 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x65,
};

LINK_78_T glink_78 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x66,
};

LINK_79_T glink_79 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x67,
};

LINK_80_T glink_80 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x68,
};

LINK_81_T glink_81 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x69,
};

LINK_82_T glink_82 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6A,
};

LINK_83_T glink_83 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6B,
};

LINK_84_T glink_84 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6C,
};

LINK_85_T glink_85 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6D,
};

LINK_86_T glink_86 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6E,
};

LINK_87_T glink_87 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6F,
};

LINK_88_T glink_88 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x70,
};

LINK_89_T glink_89 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x71,
};

LINK_90_T glink_90 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x72,
};

LINK_91_T glink_91 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x73,
};

LINK_92_T glink_92 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x74,
};

LINK_93_T glink_93 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x75,
};

LINK_94_T glink_94 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x76,
};

LINK_95_T glink_95 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x77,
};

LINK_96_T glink_96 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x78,
};

LINK_97_T glink_97 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x79,
};

LINK_98_T glink_98 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7A,
};

LINK_99_T glink_99 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7B,
};

LINK_100_T glink_100 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7C,
};

LINK_101_T glink_101 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7D,
};

LINK_102_T glink_102 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7E,
};

LINK_103_T glink_103 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7F,
};

LINK_104_T glink_104 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x80,
};

LINK_105_T glink_105 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x81,
};

LINK_106_T glink_106 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x82,
};

LINK_107_T glink_107 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x83,
};

LINK_108_T glink_108 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x84,
};

LINK_109_T glink_109 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x85,
};

LINK_110_T glink_110 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x86,
};

LINK_111_T glink_111 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x87,
};

LINK_112_T glink_112 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x88,
};

LINK_113_T glink_113 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x89,
};

LINK_114_T glink_114 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8A,
};

LINK_115_T glink_115 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8B,
};

LINK_116_T glink_116 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8C,
};

LINK_117_T glink_117 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8D,
};

LINK_118_T glink_118 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8E,
};

LINK_119_T glink_119 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8F,
};

LINK_120_T glink_120 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x90,
};

LINK_121_T glink_121 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x91,
};

LINK_122_T glink_122 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x92,
};

LINK_123_T glink_123 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x93,
};

LINK_124_T glink_124 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x94,
};

LINK_125_T glink_125 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x95,
};

LINK_126_T glink_126 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x96,
};

LINK_127_T glink_127 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x97,
};

LINK_128_T glink_128 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x98,
};

LINK_129_T glink_129 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x99,
};

LINK_130_T glink_130 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9A,
};

LINK_131_T glink_131 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9B,
};

LINK_132_T glink_132 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9C,
};

LINK_133_T glink_133 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9D,
};

LINK_134_T glink_134 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9E,
};

LINK_135_T glink_135 =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9F,
};

LINK_136_T glink_136 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA0,
};

LINK_137_T glink_137 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA1,
};

LINK_138_T glink_138 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA2,
};

LINK_139_T glink_139 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA3,
};

LINK_140_T glink_140 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA4,
};

LINK_141_T glink_141 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA5,
};

LINK_142_T glink_142 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA6,
};

LINK_143_T glink_143 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA7,
};

LINK_144_T glink_144 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA8,
};

LINK_145_T glink_145 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA9,
};

LINK_146_T glink_146 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAA,
};

LINK_147_T glink_147 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAB,
};

LINK_148_T glink_148 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAC,
};

LINK_149_T glink_149 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAD,
};

LINK_150_T glink_150 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAE,
};

LINK_151_T glink_151 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAF,
};

LINK_152_T glink_152 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB0,
};

LINK_153_T glink_153 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB1,
};

LINK_154_T glink_154 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB2,
};

LINK_155_T glink_155 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB3,
};

LINK_156_T glink_156 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB4,
};

LINK_157_T glink_157 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB5,
};

LINK_158_T glink_158 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB6,
};

LINK_159_T glink_159 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB7,
};

LINK_160_T glink_160 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB8,
};

LINK_161_T glink_161 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB9,
};

LINK_162_T glink_162 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBA,
};

LINK_163_T glink_163 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBB,
};

LINK_164_T glink_164 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBC,
};

LINK_165_T glink_165 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBD,
};

LINK_166_T glink_166 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBE,
};

LINK_167_T glink_167 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBF,
};

LINK_168_T glink_168 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC0,
};

LINK_169_T glink_169 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC1,
};

LINK_170_T glink_170 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC2,
};

LINK_171_T glink_171 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC3,
};

LINK_172_T glink_172 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC4,
};

LINK_173_T glink_173 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC5,
};

LINK_174_T glink_174 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC6,
};

LINK_175_T glink_175 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC7,
};

LINK_176_T glink_176 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC8,
};

LINK_177_T glink_177 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC9,
};

LINK_178_T glink_178 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCA,
};

LINK_179_T glink_179 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCB,
};

LINK_180_T glink_180 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCC,
};

LINK_181_T glink_181 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCD,
};

LINK_182_T glink_182 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCE,
};

LINK_183_T glink_183 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCF,
};

LINK_184_T glink_184 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD0,
};

LINK_185_T glink_185 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD1,
};

LINK_186_T glink_186 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD2,
};

LINK_187_T glink_187 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD3,
};

LINK_188_T glink_188 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD4,
};

LINK_189_T glink_189 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD5,
};

LINK_190_T glink_190 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD6,
};

LINK_191_T glink_191 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD7,
};

LINK_192_T glink_192 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD8,
};

LINK_193_T glink_193 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD9,
};

LINK_194_T glink_194 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDA,
};

LINK_195_T glink_195 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDB,
};

LINK_196_T glink_196 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDC,
};

LINK_197_T glink_197 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDD,
};

LINK_198_T glink_198 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDE,
};

LINK_199_T glink_199 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDF,
};

LINK_200_T glink_200 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE0,
};

LINK_201_T glink_201 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE1,
};

LINK_202_T glink_202 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE2,
};

LINK_203_T glink_203 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE3,
};

LINK_204_T glink_204 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE4,
};

LINK_205_T glink_205 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE5,
};

LINK_206_T glink_206 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE6,
};

LINK_207_T glink_207 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE7,
};

LINK_208_T glink_208 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE8,
};

LINK_209_T glink_209 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE9,
};

LINK_210_T glink_210 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEA,
};

LINK_211_T glink_211 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEB,
};

LINK_212_T glink_212 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEC,
};

LINK_213_T glink_213 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xED,
};

LINK_214_T glink_214 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEE,
};

LINK_215_T glink_215 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEF,
};

LINK_216_T glink_216 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF0,
};

LINK_217_T glink_217 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF1,
};

LINK_218_T glink_218 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF2,
};

LINK_219_T glink_219 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF3,
};

LINK_220_T glink_220 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF4,
};

LINK_221_T glink_221 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF5,
};

LINK_222_T glink_222 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF6,
};

LINK_223_T glink_223 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF7,
};

LINK_224_T glink_224 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF8,
};

LINK_225_T glink_225 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF9,
};

LINK_226_T glink_226 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFA,
};

LINK_227_T glink_227 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFB,
};

LINK_228_T glink_228 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFC,
};

LINK_229_T glink_229 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFD,
};

LINK_230_T glink_230 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFE,
};

LINK_231_T glink_231 =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFF,
};

LINK_232_T glink_232 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x00,
};

LINK_233_T glink_233 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x01,
};

LINK_234_T glink_234 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x02,
};

LINK_235_T glink_235 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x03,
};

LINK_236_T glink_236 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x04,
};

LINK_237_T glink_237 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x05,
};

LINK_238_T glink_238 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x06,
};

LINK_239_T glink_239 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x07,
};

LINK_240_T glink_240 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x08,
};

LINK_241_T glink_241 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x09,
};

LINK_242_T glink_242 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0A,
};

LINK_243_T glink_243 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0B,
};

LINK_244_T glink_244 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0C,
};

LINK_245_T glink_245 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0D,
};

LINK_246_T glink_246 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0E,
};

LINK_247_T glink_247 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0F,
};

LINK_248_T glink_248 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x10,
};

LINK_249_T glink_249 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x11,
};

LINK_250_T glink_250 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x12,
};

LINK_251_T glink_251 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x13,
};

LINK_252_T glink_252 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x14,
};

LINK_253_T glink_253 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x15,
};

LINK_254_T glink_254 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x16,
};

LINK_255_T glink_255 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x17,
};

LINK_256_T glink_256 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x18,
};

LINK_257_T glink_257 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x19,
};

LINK_258_T glink_258 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1A,
};

LINK_259_T glink_259 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1B,
};

LINK_260_T glink_260 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1C,
};

LINK_261_T glink_261 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1D,
};

LINK_262_T glink_262 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1E,
};

LINK_263_T glink_263 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1F,
};

LINK_264_T glink_264 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x20,
};

LINK_265_T glink_265 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x21,
};

LINK_266_T glink_266 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x22,
};

LINK_267_T glink_267 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x23,
};

LINK_268_T glink_268 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x24,
};

LINK_269_T glink_269 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x25,
};

LINK_270_T glink_270 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x26,
};

LINK_271_T glink_271 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x27,
};

LINK_272_T glink_272 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x28,
};

LINK_273_T glink_273 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x29,
};

LINK_274_T glink_274 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2A,
};

LINK_275_T glink_275 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2B,
};

LINK_276_T glink_276 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2C,
};

LINK_277_T glink_277 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2D,
};

LINK_278_T glink_278 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2E,
};

LINK_279_T glink_279 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2F,
};

LINK_280_T glink_280 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x30,
};

LINK_281_T glink_281 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x31,
};

LINK_282_T glink_282 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x32,
};

LINK_283_T glink_283 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x33,
};

LINK_284_T glink_284 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x34,
};

LINK_285_T glink_285 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x35,
};

LINK_286_T glink_286 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x36,
};

LINK_287_T glink_287 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x37,
};

LINK_288_T glink_288 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x38,
};

LINK_289_T glink_289 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x39,
};

LINK_290_T glink_290 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3A,
};

LINK_291_T glink_291 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3B,
};

LINK_292_T glink_292 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3C,
};

LINK_293_T glink_293 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3D,
};

LINK_294_T glink_294 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3E,
};

LINK_295_T glink_295 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3F,
};

LINK_296_T glink_296 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x40,
};

LINK_297_T glink_297 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x41,
};

LINK_298_T glink_298 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x42,
};

LINK_299_T glink_299 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x43,
};

LINK_300_T glink_300 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x44,
};

LINK_301_T glink_301 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x45,
};

LINK_302_T glink_302 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x46,
};

LINK_303_T glink_303 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x47,
};

LINK_304_T glink_304 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x48,
};

LINK_305_T glink_305 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x49,
};

LINK_306_T glink_306 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4A,
};

LINK_307_T glink_307 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4B,
};

LINK_308_T glink_308 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4C,
};

LINK_309_T glink_309 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4D,
};

LINK_310_T glink_310 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4E,
};

LINK_311_T glink_311 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4F,
};

LINK_312_T glink_312 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x50,
};

LINK_313_T glink_313 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x51,
};

LINK_314_T glink_314 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x52,
};

LINK_315_T glink_315 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x53,
};

LINK_316_T glink_316 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x54,
};

LINK_317_T glink_317 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x55,
};

LINK_318_T glink_318 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x56,
};

LINK_319_T glink_319 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x57,
};

LINK_320_T glink_320 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x58,
};

LINK_321_T glink_321 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x59,
};

LINK_322_T glink_322 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5A,
};

LINK_323_T glink_323 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5B,
};

LINK_324_T glink_324 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5C,
};

LINK_325_T glink_325 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5D,
};

LINK_326_T glink_326 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5E,
};

LINK_327_T glink_327 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5F,
};

LINK_328_T glink_328 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x60,
};

LINK_329_T glink_329 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x61,
};

LINK_330_T glink_330 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x62,
};

LINK_331_T glink_331 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x63,
};

LINK_332_T glink_332 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x64,
};

LINK_333_T glink_333 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x65,
};

LINK_334_T glink_334 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x66,
};

LINK_335_T glink_335 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x67,
};

LINK_336_T glink_336 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x68,
};

LINK_337_T glink_337 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x69,
};

LINK_338_T glink_338 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6A,
};

LINK_339_T glink_339 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6B,
};

LINK_340_T glink_340 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6C,
};

LINK_341_T glink_341 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6D,
};

LINK_342_T glink_342 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6E,
};

LINK_343_T glink_343 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6F,
};

LINK_344_T glink_344 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x70,
};

LINK_345_T glink_345 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x71,
};

LINK_346_T glink_346 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x72,
};

LINK_347_T glink_347 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x73,
};

LINK_348_T glink_348 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x74,
};

LINK_349_T glink_349 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x75,
};

LINK_350_T glink_350 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x76,
};

LINK_351_T glink_351 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x77,
};

LINK_352_T glink_352 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x78,
};

LINK_353_T glink_353 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x79,
};

LINK_354_T glink_354 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7A,
};

LINK_355_T glink_355 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7B,
};

LINK_356_T glink_356 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7C,
};

LINK_357_T glink_357 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7D,
};

LINK_358_T glink_358 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7E,
};

LINK_359_T glink_359 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7F,
};

LINK_360_T glink_360 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x80,
};

LINK_361_T glink_361 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x81,
};

LINK_362_T glink_362 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x82,
};

LINK_363_T glink_363 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x83,
};

LINK_364_T glink_364 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x84,
};

LINK_365_T glink_365 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x85,
};

LINK_366_T glink_366 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x86,
};

LINK_367_T glink_367 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x87,
};

LINK_368_T glink_368 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x88,
};

LINK_369_T glink_369 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x89,
};

LINK_370_T glink_370 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8A,
};

LINK_371_T glink_371 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8B,
};

LINK_372_T glink_372 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8C,
};

LINK_373_T glink_373 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8D,
};

LINK_374_T glink_374 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8E,
};

LINK_375_T glink_375 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8F,
};

LINK_376_T glink_376 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x90,
};

LINK_377_T glink_377 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x91,
};

LINK_378_T glink_378 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x92,
};

LINK_379_T glink_379 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x93,
};

LINK_380_T glink_380 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x94,
};

LINK_381_T glink_381 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x95,
};

LINK_382_T glink_382 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x96,
};

LINK_383_T glink_383 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x97,
};

LINK_384_T glink_384 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x98,
};

LINK_385_T glink_385 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x99,
};

LINK_386_T glink_386 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9A,
};

LINK_387_T glink_387 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9B,
};

LINK_388_T glink_388 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9C,
};

LINK_389_T glink_389 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9D,
};

LINK_390_T glink_390 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9E,
};

LINK_391_T glink_391 =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9F,
};

LINK_392_T glink_392 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA0,
};

LINK_393_T glink_393 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA1,
};

LINK_394_T glink_394 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA2,
};

LINK_395_T glink_395 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA3,
};

LINK_396_T glink_396 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA4,
};

LINK_397_T glink_397 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA5,
};

LINK_398_T glink_398 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA6,
};

LINK_399_T glink_399 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA7,
};

LINK_400_T glink_400 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA8,
};

LINK_401_T glink_401 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA9,
};

LINK_402_T glink_402 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAA,
};

LINK_403_T glink_403 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAB,
};

LINK_404_T glink_404 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAC,
};

LINK_405_T glink_405 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAD,
};

LINK_406_T glink_406 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAE,
};

LINK_407_T glink_407 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAF,
};

LINK_408_T glink_408 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB0,
};

LINK_409_T glink_409 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB1,
};

LINK_410_T glink_410 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB2,
};

LINK_411_T glink_411 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB3,
};

LINK_412_T glink_412 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB4,
};

LINK_413_T glink_413 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB5,
};

LINK_414_T glink_414 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB6,
};

LINK_415_T glink_415 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB7,
};

LINK_416_T glink_416 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB8,
};

LINK_417_T glink_417 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xB9,
};

LINK_418_T glink_418 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBA,
};

LINK_419_T glink_419 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBB,
};

LINK_420_T glink_420 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBC,
};

LINK_421_T glink_421 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBD,
};

LINK_422_T glink_422 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBE,
};

LINK_423_T glink_423 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xBF,
};

LINK_424_T glink_424 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC0,
};

LINK_425_T glink_425 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC1,
};

LINK_426_T glink_426 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC2,
};

LINK_427_T glink_427 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC3,
};

LINK_428_T glink_428 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC4,
};

LINK_429_T glink_429 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC5,
};

LINK_430_T glink_430 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC6,
};

LINK_431_T glink_431 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC7,
};

LINK_432_T glink_432 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC8,
};

LINK_433_T glink_433 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xC9,
};

LINK_434_T glink_434 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCA,
};

LINK_435_T glink_435 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCB,
};

LINK_436_T glink_436 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCC,
};

LINK_437_T glink_437 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCD,
};

LINK_438_T glink_438 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCE,
};

LINK_439_T glink_439 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xCF,
};

LINK_440_T glink_440 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD0,
};

LINK_441_T glink_441 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD1,
};

LINK_442_T glink_442 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD2,
};

LINK_443_T glink_443 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD3,
};

LINK_444_T glink_444 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD4,
};

LINK_445_T glink_445 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD5,
};

LINK_446_T glink_446 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD6,
};

LINK_447_T glink_447 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD7,
};

LINK_448_T glink_448 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD8,
};

LINK_449_T glink_449 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xD9,
};

LINK_450_T glink_450 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDA,
};

LINK_451_T glink_451 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDB,
};

LINK_452_T glink_452 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDC,
};

LINK_453_T glink_453 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDD,
};

LINK_454_T glink_454 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDE,
};

LINK_455_T glink_455 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xDF,
};

LINK_456_T glink_456 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE0,
};

LINK_457_T glink_457 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE1,
};

LINK_458_T glink_458 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE2,
};

LINK_459_T glink_459 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE3,
};

LINK_460_T glink_460 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE4,
};

LINK_461_T glink_461 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE5,
};

LINK_462_T glink_462 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE6,
};

LINK_463_T glink_463 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE7,
};

LINK_464_T glink_464 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE8,
};

LINK_465_T glink_465 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xE9,
};

LINK_466_T glink_466 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xEA,
};

LINK_467_T glink_467 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xEB,
};

LINK_468_T glink_468 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xEC,
};

LINK_469_T glink_469 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xED,
};

LINK_470_T glink_470 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xEE,
};

LINK_471_T glink_471 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xEF,
};

LINK_472_T glink_472 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF0,
};

LINK_473_T glink_473 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF1,
};

LINK_474_T glink_474 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF2,
};

LINK_475_T glink_475 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF3,
};

LINK_476_T glink_476 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF4,
};

LINK_477_T glink_477 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF5,
};

LINK_478_T glink_478 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF6,
};

LINK_479_T glink_479 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF7,
};

LINK_480_T glink_480 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF8,
};

LINK_481_T glink_481 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xF9,
};

LINK_482_T glink_482 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFA,
};

LINK_483_T glink_483 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFB,
};

LINK_484_T glink_484 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFC,
};

LINK_485_T glink_485 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFD,
};

LINK_486_T glink_486 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFE,
};

LINK_487_T glink_487 =
{
	.slaveAddr = 0x16,
	.regAddr = 0xFF,
};

LINK_488_T glink_488 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x00,
};

LINK_489_T glink_489 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x01,
};

LINK_490_T glink_490 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x02,
};

LINK_491_T glink_491 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x03,
};

LINK_492_T glink_492 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x04,
};

LINK_493_T glink_493 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x05,
};

LINK_494_T glink_494 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x06,
};

LINK_495_T glink_495 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x07,
};

LINK_496_T glink_496 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x08,
};

LINK_497_T glink_497 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x09,
};

LINK_498_T glink_498 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0A,
};

LINK_499_T glink_499 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0B,
};

LINK_500_T glink_500 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0C,
};

LINK_501_T glink_501 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0D,
};

LINK_502_T glink_502 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0E,
};

LINK_503_T glink_503 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x0F,
};

LINK_504_T glink_504 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x10,
};

LINK_505_T glink_505 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x11,
};

LINK_506_T glink_506 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x12,
};

LINK_507_T glink_507 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x13,
};

LINK_508_T glink_508 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x14,
};

LINK_509_T glink_509 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x15,
};

LINK_510_T glink_510 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x16,
};

LINK_511_T glink_511 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x17,
};

LINK_512_T glink_512 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x18,
};

LINK_513_T glink_513 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x19,
};

LINK_514_T glink_514 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1A,
};

LINK_515_T glink_515 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1B,
};

LINK_516_T glink_516 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1C,
};

LINK_517_T glink_517 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1D,
};

LINK_518_T glink_518 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1E,
};

LINK_519_T glink_519 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x1F,
};

LINK_520_T glink_520 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x20,
};

LINK_521_T glink_521 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x21,
};

LINK_522_T glink_522 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x22,
};

LINK_523_T glink_523 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x23,
};

LINK_524_T glink_524 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x24,
};

LINK_525_T glink_525 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x25,
};

LINK_526_T glink_526 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x26,
};

LINK_527_T glink_527 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x27,
};

LINK_528_T glink_528 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x28,
};

LINK_529_T glink_529 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x29,
};

LINK_530_T glink_530 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2A,
};

LINK_531_T glink_531 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2B,
};

LINK_532_T glink_532 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2C,
};

LINK_533_T glink_533 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2D,
};

LINK_534_T glink_534 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2E,
};

LINK_535_T glink_535 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x2F,
};

LINK_536_T glink_536 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x30,
};

LINK_537_T glink_537 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x31,
};

LINK_538_T glink_538 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x32,
};

LINK_539_T glink_539 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x33,
};

LINK_540_T glink_540 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x34,
};

LINK_541_T glink_541 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x35,
};

LINK_542_T glink_542 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x36,
};

LINK_543_T glink_543 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x37,
};

LINK_544_T glink_544 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x38,
};

LINK_545_T glink_545 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x39,
};

LINK_546_T glink_546 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3A,
};

LINK_547_T glink_547 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3B,
};

LINK_548_T glink_548 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3C,
};

LINK_549_T glink_549 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3D,
};

LINK_550_T glink_550 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3E,
};

LINK_551_T glink_551 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x3F,
};

LINK_552_T glink_552 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x40,
};

LINK_553_T glink_553 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x41,
};

LINK_554_T glink_554 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x42,
};

LINK_555_T glink_555 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x43,
};

LINK_556_T glink_556 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x44,
};

LINK_557_T glink_557 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x45,
};

LINK_558_T glink_558 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x46,
};

LINK_559_T glink_559 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x47,
};

LINK_560_T glink_560 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x48,
};

LINK_561_T glink_561 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x49,
};

LINK_562_T glink_562 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4A,
};

LINK_563_T glink_563 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4B,
};

LINK_564_T glink_564 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4C,
};

LINK_565_T glink_565 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4D,
};

LINK_566_T glink_566 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4E,
};

LINK_567_T glink_567 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x4F,
};

LINK_568_T glink_568 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x50,
};

LINK_569_T glink_569 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x51,
};

LINK_570_T glink_570 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x52,
};

LINK_571_T glink_571 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x53,
};

LINK_572_T glink_572 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x54,
};

LINK_573_T glink_573 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x55,
};

LINK_574_T glink_574 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x56,
};

LINK_575_T glink_575 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x57,
};

LINK_576_T glink_576 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x58,
};

LINK_577_T glink_577 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x59,
};

LINK_578_T glink_578 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5A,
};

LINK_579_T glink_579 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5B,
};

LINK_580_T glink_580 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5C,
};

LINK_581_T glink_581 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5D,
};

LINK_582_T glink_582 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5E,
};

LINK_583_T glink_583 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x5F,
};

LINK_584_T glink_584 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x60,
};

LINK_585_T glink_585 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x61,
};

LINK_586_T glink_586 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x62,
};

LINK_587_T glink_587 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x63,
};

LINK_588_T glink_588 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x64,
};

LINK_589_T glink_589 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x65,
};

LINK_590_T glink_590 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x66,
};

LINK_591_T glink_591 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x67,
};

LINK_592_T glink_592 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x68,
};

LINK_593_T glink_593 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x69,
};

LINK_594_T glink_594 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6A,
};

LINK_595_T glink_595 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6B,
};

LINK_596_T glink_596 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6C,
};

LINK_597_T glink_597 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6D,
};

LINK_598_T glink_598 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6E,
};

LINK_599_T glink_599 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x6F,
};

LINK_600_T glink_600 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x70,
};

LINK_601_T glink_601 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x71,
};

LINK_602_T glink_602 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x72,
};

LINK_603_T glink_603 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x73,
};

LINK_604_T glink_604 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x74,
};

LINK_605_T glink_605 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x75,
};

LINK_606_T glink_606 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x76,
};

LINK_607_T glink_607 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x77,
};

LINK_608_T glink_608 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x78,
};

LINK_609_T glink_609 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x79,
};

LINK_610_T glink_610 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7A,
};

LINK_611_T glink_611 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7B,
};

LINK_612_T glink_612 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7C,
};

LINK_613_T glink_613 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7D,
};

LINK_614_T glink_614 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7E,
};

LINK_615_T glink_615 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x7F,
};

LINK_616_T glink_616 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x80,
};

LINK_617_T glink_617 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x81,
};

LINK_618_T glink_618 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x82,
};

LINK_619_T glink_619 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x83,
};

LINK_620_T glink_620 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x84,
};

LINK_621_T glink_621 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x85,
};

LINK_622_T glink_622 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x86,
};

LINK_623_T glink_623 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x87,
};

LINK_624_T glink_624 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x88,
};

LINK_625_T glink_625 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x89,
};

LINK_626_T glink_626 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8A,
};

LINK_627_T glink_627 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8B,
};

LINK_628_T glink_628 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8C,
};

LINK_629_T glink_629 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8D,
};

LINK_630_T glink_630 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8E,
};

LINK_631_T glink_631 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x8F,
};

LINK_632_T glink_632 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x90,
};

LINK_633_T glink_633 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x91,
};

LINK_634_T glink_634 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x92,
};

LINK_635_T glink_635 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x93,
};

LINK_636_T glink_636 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x94,
};

LINK_637_T glink_637 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x95,
};

LINK_638_T glink_638 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x96,
};

LINK_639_T glink_639 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x97,
};

LINK_640_T glink_640 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x98,
};

LINK_641_T glink_641 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x99,
};

LINK_642_T glink_642 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9A,
};

LINK_643_T glink_643 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9B,
};

LINK_644_T glink_644 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9C,
};

LINK_645_T glink_645 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9D,
};

LINK_646_T glink_646 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9E,
};

LINK_647_T glink_647 =
{
	.slaveAddr = 0x17,
	.regAddr = 0x9F,
};

LINK_648_T glink_648 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA0,
};

LINK_649_T glink_649 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA1,
};

LINK_650_T glink_650 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA2,
};

LINK_651_T glink_651 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA3,
};

LINK_652_T glink_652 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA4,
};

LINK_653_T glink_653 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA5,
};

LINK_654_T glink_654 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA6,
};

LINK_655_T glink_655 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA7,
};

LINK_656_T glink_656 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA8,
};

LINK_657_T glink_657 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xA9,
};

LINK_658_T glink_658 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAA,
};

LINK_659_T glink_659 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAB,
};

LINK_660_T glink_660 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAC,
};

LINK_661_T glink_661 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAD,
};

LINK_662_T glink_662 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAE,
};

LINK_663_T glink_663 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xAF,
};

LINK_664_T glink_664 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB0,
};

LINK_665_T glink_665 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB1,
};

LINK_666_T glink_666 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB2,
};

LINK_667_T glink_667 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB3,
};

LINK_668_T glink_668 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB4,
};

LINK_669_T glink_669 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB5,
};

LINK_670_T glink_670 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB6,
};

LINK_671_T glink_671 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB7,
};

LINK_672_T glink_672 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB8,
};

LINK_673_T glink_673 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xB9,
};

LINK_674_T glink_674 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBA,
};

LINK_675_T glink_675 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBB,
};

LINK_676_T glink_676 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBC,
};

LINK_677_T glink_677 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBD,
};

LINK_678_T glink_678 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBE,
};

LINK_679_T glink_679 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xBF,
};

LINK_680_T glink_680 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC0,
};

LINK_681_T glink_681 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC1,
};

LINK_682_T glink_682 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC2,
};

LINK_683_T glink_683 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC3,
};

LINK_684_T glink_684 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC4,
};

LINK_685_T glink_685 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC5,
};

LINK_686_T glink_686 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC6,
};

LINK_687_T glink_687 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC7,
};

LINK_688_T glink_688 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC8,
};

LINK_689_T glink_689 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xC9,
};

LINK_690_T glink_690 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCA,
};

LINK_691_T glink_691 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCB,
};

LINK_692_T glink_692 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCC,
};

LINK_693_T glink_693 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCD,
};

LINK_694_T glink_694 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCE,
};

LINK_695_T glink_695 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xCF,
};

LINK_696_T glink_696 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD0,
};

LINK_697_T glink_697 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD1,
};

LINK_698_T glink_698 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD2,
};

LINK_699_T glink_699 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD3,
};

LINK_700_T glink_700 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD4,
};

LINK_701_T glink_701 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD5,
};

LINK_702_T glink_702 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD6,
};

LINK_703_T glink_703 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD7,
};

LINK_704_T glink_704 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD8,
};

LINK_705_T glink_705 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xD9,
};

LINK_706_T glink_706 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDA,
};

LINK_707_T glink_707 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDB,
};

LINK_708_T glink_708 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDC,
};

LINK_709_T glink_709 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDD,
};

LINK_710_T glink_710 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDE,
};

LINK_711_T glink_711 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xDF,
};

LINK_712_T glink_712 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE0,
};

LINK_713_T glink_713 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE1,
};

LINK_714_T glink_714 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE2,
};

LINK_715_T glink_715 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE3,
};

LINK_716_T glink_716 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE4,
};

LINK_717_T glink_717 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE5,
};

LINK_718_T glink_718 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE6,
};

LINK_719_T glink_719 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE7,
};

LINK_720_T glink_720 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE8,
};

LINK_721_T glink_721 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xE9,
};

LINK_722_T glink_722 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xEA,
};

LINK_723_T glink_723 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xEB,
};

LINK_724_T glink_724 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xEC,
};

LINK_725_T glink_725 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xED,
};

LINK_726_T glink_726 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xEE,
};

LINK_727_T glink_727 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xEF,
};

LINK_728_T glink_728 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF0,
};

LINK_729_T glink_729 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF1,
};

LINK_730_T glink_730 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF2,
};

LINK_731_T glink_731 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF3,
};

LINK_732_T glink_732 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF4,
};

LINK_733_T glink_733 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF5,
};

LINK_734_T glink_734 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF6,
};

LINK_735_T glink_735 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF7,
};

LINK_736_T glink_736 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF8,
};

LINK_737_T glink_737 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xF9,
};

LINK_738_T glink_738 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFA,
};

LINK_739_T glink_739 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFB,
};

LINK_740_T glink_740 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFC,
};

LINK_741_T glink_741 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFD,
};

LINK_742_T glink_742 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFE,
};

LINK_743_T glink_743 =
{
	.slaveAddr = 0x17,
	.regAddr = 0xFF,
};

LINK_744_T glink_744 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x00,
};

LINK_745_T glink_745 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x01,
};

LINK_746_T glink_746 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x02,
};

LINK_747_T glink_747 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x03,
};

LINK_748_T glink_748 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x04,
};

LINK_749_T glink_749 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x05,
};

LINK_750_T glink_750 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x06,
};

LINK_751_T glink_751 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x07,
};

LINK_752_T glink_752 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x08,
};

LINK_753_T glink_753 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x09,
};

LINK_754_T glink_754 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0A,
};

LINK_755_T glink_755 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0B,
};

LINK_756_T glink_756 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0C,
};

LINK_757_T glink_757 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0D,
};

LINK_758_T glink_758 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0E,
};

LINK_759_T glink_759 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x0F,
};

LINK_760_T glink_760 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x10,
};

LINK_761_T glink_761 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x11,
};

LINK_762_T glink_762 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x12,
};

LINK_763_T glink_763 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x13,
};

LINK_764_T glink_764 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x14,
};

LINK_765_T glink_765 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x15,
};

LINK_766_T glink_766 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x16,
};

LINK_767_T glink_767 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x17,
};

LINK_768_T glink_768 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x18,
};

LINK_769_T glink_769 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x19,
};

LINK_770_T glink_770 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1A,
};

LINK_771_T glink_771 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1B,
};

LINK_772_T glink_772 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1C,
};

LINK_773_T glink_773 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1D,
};

LINK_774_T glink_774 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1E,
};

LINK_775_T glink_775 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x1F,
};

LINK_776_T glink_776 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x20,
};

LINK_777_T glink_777 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x21,
};

LINK_778_T glink_778 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x22,
};

LINK_779_T glink_779 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x23,
};

LINK_780_T glink_780 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x24,
};

LINK_781_T glink_781 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x25,
};

LINK_782_T glink_782 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x26,
};

LINK_783_T glink_783 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x27,
};

LINK_784_T glink_784 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x28,
};

LINK_785_T glink_785 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x29,
};

LINK_786_T glink_786 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2A,
};

LINK_787_T glink_787 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2B,
};

LINK_788_T glink_788 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2C,
};

LINK_789_T glink_789 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2D,
};

LINK_790_T glink_790 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2E,
};

LINK_791_T glink_791 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x2F,
};

LINK_792_T glink_792 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x30,
};

LINK_793_T glink_793 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x31,
};

LINK_794_T glink_794 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x32,
};

LINK_795_T glink_795 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x33,
};

LINK_796_T glink_796 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x34,
};

LINK_797_T glink_797 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x35,
};

LINK_798_T glink_798 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x36,
};

LINK_799_T glink_799 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x37,
};

LINK_800_T glink_800 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x38,
};

LINK_801_T glink_801 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x39,
};

LINK_802_T glink_802 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3A,
};

LINK_803_T glink_803 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3B,
};

LINK_804_T glink_804 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3C,
};

LINK_805_T glink_805 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3D,
};

LINK_806_T glink_806 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3E,
};

LINK_807_T glink_807 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x3F,
};

LINK_808_T glink_808 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x40,
};

LINK_809_T glink_809 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x41,
};

LINK_810_T glink_810 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x42,
};

LINK_811_T glink_811 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x43,
};

LINK_812_T glink_812 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x44,
};

LINK_813_T glink_813 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x45,
};

LINK_814_T glink_814 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x46,
};

LINK_815_T glink_815 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x47,
};

LINK_816_T glink_816 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x48,
};

LINK_817_T glink_817 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x49,
};

LINK_818_T glink_818 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4A,
};

LINK_819_T glink_819 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4B,
};

LINK_820_T glink_820 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4C,
};

LINK_821_T glink_821 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4D,
};

LINK_822_T glink_822 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4E,
};

LINK_823_T glink_823 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4F,
};

LINK_824_T glink_824 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x50,
};

LINK_825_T glink_825 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x51,
};

LINK_826_T glink_826 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x52,
};

LINK_827_T glink_827 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x53,
};

LINK_828_T glink_828 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x54,
};

LINK_829_T glink_829 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x55,
};

LINK_830_T glink_830 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x56,
};

LINK_831_T glink_831 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x57,
};

LINK_832_T glink_832 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x58,
};

LINK_833_T glink_833 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x59,
};

LINK_834_T glink_834 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5A,
};

LINK_835_T glink_835 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5B,
};

LINK_836_T glink_836 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5C,
};

LINK_837_T glink_837 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5D,
};

LINK_838_T glink_838 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5E,
};

LINK_839_T glink_839 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5F,
};

LINK_840_T glink_840 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x60,
};

LINK_841_T glink_841 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x61,
};

LINK_842_T glink_842 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x62,
};

LINK_843_T glink_843 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x63,
};

LINK_844_T glink_844 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x64,
};

LINK_845_T glink_845 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x65,
};

LINK_846_T glink_846 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x66,
};

LINK_847_T glink_847 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x67,
};

LINK_848_T glink_848 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x68,
};

LINK_849_T glink_849 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x69,
};

LINK_850_T glink_850 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6A,
};

LINK_851_T glink_851 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6B,
};

LINK_852_T glink_852 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6C,
};

LINK_853_T glink_853 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6D,
};

LINK_854_T glink_854 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6E,
};

LINK_855_T glink_855 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6F,
};

LINK_856_T glink_856 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x70,
};

LINK_857_T glink_857 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x71,
};

LINK_858_T glink_858 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x72,
};

LINK_859_T glink_859 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x73,
};

LINK_860_T glink_860 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x74,
};

LINK_861_T glink_861 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x75,
};

LINK_862_T glink_862 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x76,
};

LINK_863_T glink_863 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x77,
};

LINK_864_T glink_864 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x78,
};

LINK_865_T glink_865 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x79,
};

LINK_866_T glink_866 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7A,
};

LINK_867_T glink_867 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7B,
};

LINK_868_T glink_868 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7C,
};

LINK_869_T glink_869 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7D,
};

LINK_870_T glink_870 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7E,
};

LINK_871_T glink_871 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7F,
};

LINK_872_T glink_872 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x80,
};

LINK_873_T glink_873 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x81,
};

LINK_874_T glink_874 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x82,
};

LINK_875_T glink_875 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x83,
};

LINK_876_T glink_876 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x84,
};

LINK_877_T glink_877 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x85,
};

LINK_878_T glink_878 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x86,
};

LINK_879_T glink_879 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x87,
};

LINK_880_T glink_880 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x88,
};

LINK_881_T glink_881 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x89,
};

LINK_882_T glink_882 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8A,
};

LINK_883_T glink_883 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8B,
};

LINK_884_T glink_884 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8C,
};

LINK_885_T glink_885 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8D,
};

LINK_886_T glink_886 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8E,
};

LINK_887_T glink_887 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8F,
};

LINK_888_T glink_888 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x90,
};

LINK_889_T glink_889 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x91,
};

LINK_890_T glink_890 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x92,
};

LINK_891_T glink_891 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x93,
};

LINK_892_T glink_892 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x94,
};

LINK_893_T glink_893 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x95,
};

LINK_894_T glink_894 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x96,
};

LINK_895_T glink_895 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x97,
};

LINK_896_T glink_896 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x98,
};

LINK_897_T glink_897 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x99,
};

LINK_898_T glink_898 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9A,
};

LINK_899_T glink_899 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9B,
};

LINK_900_T glink_900 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9C,
};

LINK_901_T glink_901 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9D,
};

LINK_902_T glink_902 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9E,
};

LINK_903_T glink_903 =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9F,
};

LINK_904_T glink_904 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA0,
};

LINK_905_T glink_905 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA1,
};

LINK_906_T glink_906 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA2,
};

LINK_907_T glink_907 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA3,
};

LINK_908_T glink_908 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA4,
};

LINK_909_T glink_909 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA5,
};

LINK_910_T glink_910 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA6,
};

LINK_911_T glink_911 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA7,
};

LINK_912_T glink_912 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA8,
};

LINK_913_T glink_913 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA9,
};

LINK_914_T glink_914 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAA,
};

LINK_915_T glink_915 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAB,
};

LINK_916_T glink_916 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAC,
};

LINK_917_T glink_917 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAD,
};

LINK_918_T glink_918 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAE,
};

LINK_919_T glink_919 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAF,
};

LINK_920_T glink_920 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB0,
};

LINK_921_T glink_921 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB1,
};

LINK_922_T glink_922 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB2,
};

LINK_923_T glink_923 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB3,
};

LINK_924_T glink_924 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB4,
};

LINK_925_T glink_925 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB5,
};

LINK_926_T glink_926 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB6,
};

LINK_927_T glink_927 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB7,
};

LINK_928_T glink_928 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB8,
};

LINK_929_T glink_929 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB9,
};

LINK_930_T glink_930 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBA,
};

LINK_931_T glink_931 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBB,
};

LINK_932_T glink_932 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBC,
};

LINK_933_T glink_933 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBD,
};

LINK_934_T glink_934 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBE,
};

LINK_935_T glink_935 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBF,
};

LINK_936_T glink_936 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC0,
};

LINK_937_T glink_937 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC1,
};

LINK_938_T glink_938 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC2,
};

LINK_939_T glink_939 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC3,
};

LINK_940_T glink_940 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC4,
};

LINK_941_T glink_941 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC5,
};

LINK_942_T glink_942 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC6,
};

LINK_943_T glink_943 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC7,
};

LINK_944_T glink_944 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC8,
};

LINK_945_T glink_945 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC9,
};

LINK_946_T glink_946 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCA,
};

LINK_947_T glink_947 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCB,
};

LINK_948_T glink_948 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCC,
};

LINK_949_T glink_949 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCD,
};

LINK_950_T glink_950 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCE,
};

LINK_951_T glink_951 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCF,
};

LINK_952_T glink_952 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD0,
};

LINK_953_T glink_953 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD1,
};

LINK_954_T glink_954 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD2,
};

LINK_955_T glink_955 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD3,
};

LINK_956_T glink_956 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD4,
};

LINK_957_T glink_957 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD5,
};

LINK_958_T glink_958 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD6,
};

LINK_959_T glink_959 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD7,
};

LINK_960_T glink_960 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD8,
};

LINK_961_T glink_961 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD9,
};

LINK_962_T glink_962 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDA,
};

LINK_963_T glink_963 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDB,
};

LINK_964_T glink_964 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDC,
};

LINK_965_T glink_965 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDD,
};

LINK_966_T glink_966 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDE,
};

LINK_967_T glink_967 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDF,
};

LINK_968_T glink_968 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE0,
};

LINK_969_T glink_969 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE1,
};

LINK_970_T glink_970 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE2,
};

LINK_971_T glink_971 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE3,
};

LINK_972_T glink_972 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE4,
};

LINK_973_T glink_973 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE5,
};

LINK_974_T glink_974 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE6,
};

LINK_975_T glink_975 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE7,
};

LINK_976_T glink_976 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE8,
};

LINK_977_T glink_977 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE9,
};

LINK_978_T glink_978 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEA,
};

LINK_979_T glink_979 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEB,
};

LINK_980_T glink_980 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEC,
};

LINK_981_T glink_981 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xED,
};

LINK_982_T glink_982 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEE,
};

LINK_983_T glink_983 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEF,
};

LINK_984_T glink_984 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF0,
};

LINK_985_T glink_985 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF1,
};

LINK_986_T glink_986 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF2,
};

LINK_987_T glink_987 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF3,
};

LINK_988_T glink_988 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF4,
};

LINK_989_T glink_989 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF5,
};

LINK_990_T glink_990 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF6,
};

LINK_991_T glink_991 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF7,
};

LINK_992_T glink_992 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF8,
};

LINK_993_T glink_993 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF9,
};

LINK_994_T glink_994 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFA,
};

LINK_995_T glink_995 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFB,
};

LINK_996_T glink_996 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFC,
};

LINK_997_T glink_997 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFD,
};

LINK_998_T glink_998 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFE,
};

LINK_999_T glink_999 =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFF,
};

LINK_1000_T glink_1000 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x00,
};

LINK_1001_T glink_1001 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x01,
};

LINK_1002_T glink_1002 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x02,
};

LINK_1003_T glink_1003 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x03,
};

LINK_1004_T glink_1004 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x04,
};

LINK_1005_T glink_1005 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x05,
};

LINK_1006_T glink_1006 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x06,
};

LINK_1007_T glink_1007 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x07,
};

LINK_1008_T glink_1008 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x08,
};

LINK_1009_T glink_1009 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x09,
};

LINK_1010_T glink_1010 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0A,
};

LINK_1011_T glink_1011 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0B,
};

LINK_1012_T glink_1012 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0C,
};

LINK_1013_T glink_1013 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0D,
};

LINK_1014_T glink_1014 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0E,
};

LINK_1015_T glink_1015 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0F,
};

LINK_1016_T glink_1016 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x10,
};

LINK_1017_T glink_1017 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x11,
};

LINK_1018_T glink_1018 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x12,
};

LINK_1019_T glink_1019 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x13,
};

LINK_1020_T glink_1020 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x14,
};

LINK_1021_T glink_1021 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x15,
};

LINK_1022_T glink_1022 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x16,
};

LINK_1023_T glink_1023 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x17,
};

LINK_1024_T glink_1024 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x18,
};

LINK_1025_T glink_1025 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x19,
};

LINK_1026_T glink_1026 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1A,
};

LINK_1027_T glink_1027 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1B,
};

LINK_1028_T glink_1028 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1C,
};

LINK_1029_T glink_1029 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1D,
};

LINK_1030_T glink_1030 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1E,
};

LINK_1031_T glink_1031 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1F,
};

LINK_1032_T glink_1032 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x20,
};

LINK_1033_T glink_1033 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x21,
};

LINK_1034_T glink_1034 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x22,
};

LINK_1035_T glink_1035 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x23,
};

LINK_1036_T glink_1036 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x24,
};

LINK_1037_T glink_1037 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x25,
};

LINK_1038_T glink_1038 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x26,
};

LINK_1039_T glink_1039 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x27,
};

LINK_1040_T glink_1040 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x28,
};

LINK_1041_T glink_1041 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x29,
};

LINK_1042_T glink_1042 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2A,
};

LINK_1043_T glink_1043 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2B,
};

LINK_1044_T glink_1044 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2C,
};

LINK_1045_T glink_1045 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2D,
};

LINK_1046_T glink_1046 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2E,
};

LINK_1047_T glink_1047 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2F,
};

LINK_1048_T glink_1048 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x30,
};

LINK_1049_T glink_1049 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x31,
};

LINK_1050_T glink_1050 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x32,
};

LINK_1051_T glink_1051 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x33,
};

LINK_1052_T glink_1052 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x34,
};

LINK_1053_T glink_1053 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x35,
};

LINK_1054_T glink_1054 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x36,
};

LINK_1055_T glink_1055 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x37,
};

LINK_1056_T glink_1056 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x38,
};

LINK_1057_T glink_1057 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x39,
};

LINK_1058_T glink_1058 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3A,
};

LINK_1059_T glink_1059 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3B,
};

LINK_1060_T glink_1060 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3C,
};

LINK_1061_T glink_1061 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3D,
};

LINK_1062_T glink_1062 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3E,
};

LINK_1063_T glink_1063 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3F,
};

LINK_1064_T glink_1064 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x40,
};

LINK_1065_T glink_1065 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x41,
};

LINK_1066_T glink_1066 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x42,
};

LINK_1067_T glink_1067 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x43,
};

LINK_1068_T glink_1068 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x44,
};

LINK_1069_T glink_1069 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x45,
};

LINK_1070_T glink_1070 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x46,
};

LINK_1071_T glink_1071 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x47,
};

LINK_1072_T glink_1072 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x48,
};

LINK_1073_T glink_1073 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x49,
};

LINK_1074_T glink_1074 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4A,
};

LINK_1075_T glink_1075 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4B,
};

LINK_1076_T glink_1076 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4C,
};

LINK_1077_T glink_1077 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4D,
};

LINK_1078_T glink_1078 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4E,
};

LINK_1079_T glink_1079 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4F,
};

LINK_1080_T glink_1080 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x50,
};

LINK_1081_T glink_1081 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x51,
};

LINK_1082_T glink_1082 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x52,
};

LINK_1083_T glink_1083 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x53,
};

LINK_1084_T glink_1084 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x54,
};

LINK_1085_T glink_1085 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x55,
};

LINK_1086_T glink_1086 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x56,
};

LINK_1087_T glink_1087 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x57,
};

LINK_1088_T glink_1088 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x58,
};

LINK_1089_T glink_1089 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x59,
};

LINK_1090_T glink_1090 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5A,
};

LINK_1091_T glink_1091 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5B,
};

LINK_1092_T glink_1092 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5C,
};

LINK_1093_T glink_1093 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5D,
};

LINK_1094_T glink_1094 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5E,
};

LINK_1095_T glink_1095 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5F,
};

LINK_1096_T glink_1096 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x60,
};

LINK_1097_T glink_1097 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x61,
};

LINK_1098_T glink_1098 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x62,
};

LINK_1099_T glink_1099 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x63,
};

LINK_1100_T glink_1100 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x64,
};

LINK_1101_T glink_1101 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x65,
};

LINK_1102_T glink_1102 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x66,
};

LINK_1103_T glink_1103 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x67,
};

LINK_1104_T glink_1104 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x68,
};

LINK_1105_T glink_1105 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x69,
};

LINK_1106_T glink_1106 =
{
	.slaveAddr = 0x19,
	.regAddr = 0x6A,
};

L9A_3CH_INTR_0_T gl9a_3ch_intr_0 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x00,
};

L9A_3CH_INTR_1_T gl9a_3ch_intr_1 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x01,
};

L9A_3CH_INTR_EN_0_T gl9a_3ch_intr_en_0 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x02,
};

L9A_3CH_INTR_EN_1_T gl9a_3ch_intr_en_1 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x03,
};

LLPLL_0_T gllpll_0 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x04,
};

LLPLL_1_T gllpll_1 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x05,
};

LLPLL_2_T gllpll_2 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x06,
};

LLPLL_3_T gllpll_3 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x07,
};

LLPLL_4_T gllpll_4 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x08,
};

LLPLL_5_T gllpll_5 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x09,
};

LLPLL_6_T gllpll_6 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0A,
};

LLPLL_7_T gllpll_7 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0B,
};

LLPLL_8_T gllpll_8 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0C,
};

LLPLL_9_T gllpll_9 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0D,
};

LLPLL_10_T gllpll_10 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0E,
};

LLPLL_11_T gllpll_11 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0F,
};

LLPLL_12_T gllpll_12 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x10,
};

LLPLL_13_T gllpll_13 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x11,
};

LLPLL_14_T gllpll_14 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x12,
};

LLPLL_15_T gllpll_15 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x13,
};

LLPLL_16_T gllpll_16 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x14,
};

LLPLL_17_T gllpll_17 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x15,
};

LLPLL_18_T gllpll_18 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x16,
};

LLPLL_19_T gllpll_19 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x17,
};

LLPLL_20_T gllpll_20 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x18,
};

LLPLL_21_T gllpll_21 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x19,
};

LLPLL_22_T gllpll_22 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1A,
};

LLPLL_23_T gllpll_23 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1B,
};

LLPLL_24_T gllpll_24 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1C,
};

LLPLL_25_T gllpll_25 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1D,
};

LLPLL_26_T gllpll_26 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1E,
};

LLPLL_27_T gllpll_27 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1F,
};

AFE_3CH_0_T gafe_3ch_0 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x20,
};

AFE_3CH_1_T gafe_3ch_1 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x21,
};

AFE_3CH_2_T gafe_3ch_2 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x22,
};

AFE_3CH_3_T gafe_3ch_3 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x23,
};

AFE_3CH_4_T gafe_3ch_4 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x24,
};

AFE_3CH_5_T gafe_3ch_5 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x25,
};

AFE_3CH_6_T gafe_3ch_6 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x26,
};

AFE_3CH_7_T gafe_3ch_7 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x27,
};

ADC_DIG_0_T gadc_dig_0 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x28,
};

ADC_DIG_1_T gadc_dig_1 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x29,
};

ADC_DIG_2_T gadc_dig_2 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2A,
};

ADC_DIG_3_T gadc_dig_3 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2B,
};

ADC_DIG_4_T gadc_dig_4 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2C,
};

ADC_DIG_5_T gadc_dig_5 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2D,
};

ADC_DIG_6_T gadc_dig_6 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2E,
};

ADC_DIG_7_T gadc_dig_7 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2F,
};

ADC_DIG_8_T gadc_dig_8 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x30,
};

ADC_DIG_9_T gadc_dig_9 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x31,
};

ADC_DIG_10_T gadc_dig_10 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x32,
};

ADC_DIG_11_T gadc_dig_11 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x33,
};

ADC_DIG_12_T gadc_dig_12 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x34,
};

ADC_DIG_13_T gadc_dig_13 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x35,
};

ADC_DIG_14_T gadc_dig_14 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x36,
};

ADC_DIG_15_T gadc_dig_15 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x37,
};

ADC_DIG_16_T gadc_dig_16 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x38,
};

ADC_DIG_17_T gadc_dig_17 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x39,
};

ADC_DIG_18_T gadc_dig_18 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3A,
};

ADC_DIG_19_T gadc_dig_19 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3B,
};

ADC_DIG_20_T gadc_dig_20 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3C,
};

ADC_DIG_21_T gadc_dig_21 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3D,
};

ADC_DIG_22_T gadc_dig_22 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3E,
};

ADC_DIG_23_T gadc_dig_23 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3F,
};

ADC_DIG_24_T gadc_dig_24 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x40,
};

ADC_DIG_25_T gadc_dig_25 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x41,
};

ADC_DIG_26_T gadc_dig_26 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x42,
};

ADC_DIG_27_T gadc_dig_27 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x43,
};

ADC_DIG_28_T gadc_dig_28 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x44,
};

ADC_DIG_29_T gadc_dig_29 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x45,
};

ADC_DIG_30_T gadc_dig_30 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x46,
};

ADC_DIG_31_T gadc_dig_31 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x47,
};

ADC_DIG_32_T gadc_dig_32 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x48,
};

ADC_DIG_33_T gadc_dig_33 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x49,
};

ADC_DIG_34_T gadc_dig_34 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4A,
};

ADC_DIG_35_T gadc_dig_35 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4B,
};

ADC_DIG_36_T gadc_dig_36 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4C,
};

ADC_DIG_37_T gadc_dig_37 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4D,
};

ADC_DIG_38_T gadc_dig_38 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4E,
};

ADC_DIG_39_T gadc_dig_39 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4F,
};

ADC_DIG_40_T gadc_dig_40 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x50,
};

ADC_DIG_41_T gadc_dig_41 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x51,
};

ADC_DIG_42_T gadc_dig_42 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x52,
};

ADC_DIG_43_T gadc_dig_43 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x53,
};

ADC_DIG_44_T gadc_dig_44 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x54,
};

ADC_DIG_45_T gadc_dig_45 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x55,
};

ADC_DIG_46_T gadc_dig_46 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x56,
};

ADC_DIG_47_T gadc_dig_47 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x57,
};

ADC_DIG_48_T gadc_dig_48 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x58,
};

ADC_DIG_49_T gadc_dig_49 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x59,
};

ADC_DIG_50_T gadc_dig_50 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5A,
};

ADC_DIG_51_T gadc_dig_51 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5B,
};

ADC_DIG_52_T gadc_dig_52 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5C,
};

ADC_DIG_53_T gadc_dig_53 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5D,
};

ADC_DIG_54_T gadc_dig_54 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5E,
};

ADC_DIG_55_T gadc_dig_55 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5F,
};

ADC_DIG_56_T gadc_dig_56 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x60,
};

ADC_DIG_57_T gadc_dig_57 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x61,
};

ADC_DIG_58_T gadc_dig_58 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x62,
};

ADC_DIG_59_T gadc_dig_59 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x63,
};

ADC_DIG_60_T gadc_dig_60 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x64,
};

ADC_DIG_61_T gadc_dig_61 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x65,
};

ADC_DIG_62_T gadc_dig_62 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x66,
};

ADC_DIG_63_T gadc_dig_63 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x67,
};

ADC_DIG_64_T gadc_dig_64 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x68,
};

ADC_DIG_65_T gadc_dig_65 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x69,
};

ADC_DIG_66_T gadc_dig_66 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6A,
};

ADC_DIG_67_T gadc_dig_67 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6B,
};

ADC_DIG_68_T gadc_dig_68 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6C,
};

ADC_DIG_69_T gadc_dig_69 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6D,
};

ADC_DIG_70_T gadc_dig_70 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6E,
};

ADC_DIG_71_T gadc_dig_71 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6F,
};

ADC_DIG_72_T gadc_dig_72 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x70,
};

ADC_DIG_73_T gadc_dig_73 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x71,
};

ADC_DIG_74_T gadc_dig_74 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x72,
};

ADC_DIG_75_T gadc_dig_75 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x73,
};

ADC_DIG_76_T gadc_dig_76 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x74,
};

ADC_DIG_77_T gadc_dig_77 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x75,
};

ADC_DIG_78_T gadc_dig_78 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x76,
};

ADC_DIG_79_T gadc_dig_79 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x77,
};

ADC_DIG_80_T gadc_dig_80 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x78,
};

ADC_DIG_81_T gadc_dig_81 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x79,
};

ADC_DIG_82_T gadc_dig_82 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7A,
};

ADC_DIG_83_T gadc_dig_83 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7B,
};

ADC_DIG_84_T gadc_dig_84 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7C,
};

ADC_DIG_85_T gadc_dig_85 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7D,
};

ADC_DIG_86_T gadc_dig_86 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7E,
};

ADC_DIG_87_T gadc_dig_87 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7F,
};

ADC_DIG_88_T gadc_dig_88 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x80,
};

ADC_DIG_89_T gadc_dig_89 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x81,
};

ADC_DIG_90_T gadc_dig_90 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x82,
};

ADC_DIG_91_T gadc_dig_91 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x83,
};

ADC_DIG_92_T gadc_dig_92 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x84,
};

ADC_DIG_93_T gadc_dig_93 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x85,
};

ADC_DIG_94_T gadc_dig_94 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x86,
};

ADC_DIG_95_T gadc_dig_95 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x87,
};

ADC_DIG_96_T gadc_dig_96 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x88,
};

ADC_DIG_97_T gadc_dig_97 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x89,
};

ADC_DIG_98_T gadc_dig_98 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8A,
};

ADC_DIG_99_T gadc_dig_99 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8B,
};

ADC_DIG_100_T gadc_dig_100 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8C,
};

ADC_DIG_101_T gadc_dig_101 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8D,
};

ADC_DIG_102_T gadc_dig_102 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8E,
};

ADC_DIG_103_T gadc_dig_103 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8F,
};

ADC_DIG_104_T gadc_dig_104 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x90,
};

ADC_DIG_105_T gadc_dig_105 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x91,
};

ADC_DIG_106_T gadc_dig_106 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x92,
};

ADC_DIG_107_T gadc_dig_107 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x93,
};

ADC_DIG_108_T gadc_dig_108 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x94,
};

ADC_DIG_109_T gadc_dig_109 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x95,
};

ADC_DIG_110_T gadc_dig_110 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x96,
};

ADC_DIG_111_T gadc_dig_111 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x97,
};

ADC_DIG_112_T gadc_dig_112 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x98,
};

ADC_DIG_113_T gadc_dig_113 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x99,
};

ADC_DIG_114_T gadc_dig_114 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9A,
};

ADC_DIG_115_T gadc_dig_115 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9B,
};

ADC_DIG_116_T gadc_dig_116 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9C,
};

ADC_DIG_117_T gadc_dig_117 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9D,
};

ADC_DIG_118_T gadc_dig_118 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9E,
};

ADC_DIG_119_T gadc_dig_119 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9F,
};

ADC_DIG_120_T gadc_dig_120 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA0,
};

ADC_DIG_121_T gadc_dig_121 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA1,
};

ADC_DIG_122_T gadc_dig_122 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA2,
};

ADC_DIG_123_T gadc_dig_123 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA3,
};

ADC_DIG_124_T gadc_dig_124 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA4,
};

ADC_DIG_125_T gadc_dig_125 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA5,
};

ADC_DIG_126_T gadc_dig_126 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA6,
};

ADC_DIG_127_T gadc_dig_127 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA7,
};

ADC_DIG_128_T gadc_dig_128 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA8,
};

ADC_DIG_129_T gadc_dig_129 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA9,
};

ADC_DIG_130_T gadc_dig_130 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAA,
};

ADC_DIG_131_T gadc_dig_131 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAB,
};

ADC_DIG_132_T gadc_dig_132 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAC,
};

ADC_DIG_133_T gadc_dig_133 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAD,
};

ADC_DIG_134_T gadc_dig_134 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAE,
};

ADC_DIG_135_T gadc_dig_135 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAF,
};

ADC_DIG_136_T gadc_dig_136 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB0,
};

ADC_DIG_137_T gadc_dig_137 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB1,
};

ADC_DIG_138_T gadc_dig_138 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB2,
};

ADC_DIG_139_T gadc_dig_139 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB3,
};

ADC_DIG_140_T gadc_dig_140 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB4,
};

ADC_DIG_141_T gadc_dig_141 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB5,
};

ADC_DIG_142_T gadc_dig_142 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB6,
};

ADC_DIG_143_T gadc_dig_143 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB7,
};

ADC_DIG_144_T gadc_dig_144 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB8,
};

ADC_DIG_145_T gadc_dig_145 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB9,
};

ADC_DIG_146_T gadc_dig_146 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBA,
};

ADC_DIG_147_T gadc_dig_147 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBB,
};

ADC_DIG_148_T gadc_dig_148 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBC,
};

ADC_DIG_149_T gadc_dig_149 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBD,
};

ADC_DIG_150_T gadc_dig_150 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBE,
};

ADC_DIG_151_T gadc_dig_151 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBF,
};

ADC_DIG_152_T gadc_dig_152 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC0,
};

ADC_DIG_153_T gadc_dig_153 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC1,
};

ADC_DIG_154_T gadc_dig_154 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC2,
};

ADC_DIG_155_T gadc_dig_155 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC3,
};

ADC_DIG_156_T gadc_dig_156 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC4,
};

ADC_DIG_157_T gadc_dig_157 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC5,
};

ADC_DIG_158_T gadc_dig_158 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC6,
};

ADC_DIG_159_T gadc_dig_159 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC7,
};

ADC_DIG_160_T gadc_dig_160 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC8,
};

ADC_DIG_161_T gadc_dig_161 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC9,
};

ADC_DIG_162_T gadc_dig_162 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCA,
};

ADC_DIG_163_T gadc_dig_163 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCB,
};

ADC_DIG_164_T gadc_dig_164 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCC,
};

ADC_DIG_165_T gadc_dig_165 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCD,
};

ADC_DIG_166_T gadc_dig_166 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCE,
};

ADC_DIG_167_T gadc_dig_167 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCF,
};

ADC_DIG_168_T gadc_dig_168 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD0,
};

ADC_DIG_169_T gadc_dig_169 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD1,
};

ADC_DIG_170_T gadc_dig_170 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD2,
};

ADC_DIG_171_T gadc_dig_171 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD3,
};

ADC_DIG_172_T gadc_dig_172 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD4,
};

ADC_DIG_173_T gadc_dig_173 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD5,
};

ADC_DIG_174_T gadc_dig_174 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD6,
};

ADC_DIG_175_T gadc_dig_175 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD7,
};

ADC_DIG_176_T gadc_dig_176 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD8,
};

ADC_DIG_177_T gadc_dig_177 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD9,
};

ADC_DIG_178_T gadc_dig_178 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDA,
};

ADC_DIG_179_T gadc_dig_179 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDB,
};

ADC_DIG_180_T gadc_dig_180 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDC,
};

ADC_DIG_181_T gadc_dig_181 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDD,
};

ADC_DIG_182_T gadc_dig_182 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDE,
};

ADC_DIG_183_T gadc_dig_183 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDF,
};

ADC_DIG_184_T gadc_dig_184 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE0,
};

ADC_DIG_185_T gadc_dig_185 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE1,
};

ADC_DIG_186_T gadc_dig_186 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE2,
};

ADC_DIG_187_T gadc_dig_187 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE3,
};

ADC_DIG_188_T gadc_dig_188 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE4,
};

ADC_DIG_189_T gadc_dig_189 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE5,
};

ADC_DIG_190_T gadc_dig_190 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE6,
};

ADC_DIG_191_T gadc_dig_191 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE7,
};

ADC_DIG_192_T gadc_dig_192 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE8,
};

ADC_DIG_193_T gadc_dig_193 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE9,
};

ADC_DIG_194_T gadc_dig_194 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEA,
};

ADC_DIG_195_T gadc_dig_195 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEB,
};

ADC_DIG_196_T gadc_dig_196 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEC,
};

ADC_DIG_197_T gadc_dig_197 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xED,
};

ADC_DIG_198_T gadc_dig_198 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEE,
};

ADC_DIG_199_T gadc_dig_199 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEF,
};

ADC_DIG_200_T gadc_dig_200 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF0,
};

ADC_DIG_201_T gadc_dig_201 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF1,
};

ADC_DIG_202_T gadc_dig_202 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF2,
};

ADC_DIG_203_T gadc_dig_203 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF3,
};

ADC_DIG_204_T gadc_dig_204 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF4,
};

ADC_DIG_205_T gadc_dig_205 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF5,
};

ADC_DIG_206_T gadc_dig_206 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF6,
};

ADC_DIG_207_T gadc_dig_207 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF7,
};

ADC_DIG_208_T gadc_dig_208 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF8,
};

ADC_DIG_209_T gadc_dig_209 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF9,
};

ADC_DIG_210_T gadc_dig_210 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFA,
};

ADC_DIG_211_T gadc_dig_211 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFB,
};

ADC_DIG_212_T gadc_dig_212 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFC,
};

ADC_DIG_213_T gadc_dig_213 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFD,
};

ADC_DIG_214_T gadc_dig_214 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFE,
};

ADC_DIG_215_T gadc_dig_215 =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFF,
};

ADC_DIG_216_T gadc_dig_216 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x00,
};

ADC_DIG_217_T gadc_dig_217 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x01,
};

ADC_DIG_218_T gadc_dig_218 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x02,
};

ADC_DIG_219_T gadc_dig_219 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x03,
};

ADC_DIG_220_T gadc_dig_220 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x04,
};

ADC_DIG_221_T gadc_dig_221 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x05,
};

ADC_DIG_222_T gadc_dig_222 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x06,
};

ADC_DIG_223_T gadc_dig_223 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x07,
};

ADC_DIG_224_T gadc_dig_224 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x08,
};

ADC_DIG_225_T gadc_dig_225 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x09,
};

ADC_DIG_226_T gadc_dig_226 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0A,
};

ADC_DIG_227_T gadc_dig_227 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0B,
};

ADC_DIG_228_T gadc_dig_228 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0C,
};

ADC_DIG_229_T gadc_dig_229 =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0D,
};

AUDIO_PLL_0_T gaudio_pll_0 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x00,
};

AUDIO_PLL_1_T gaudio_pll_1 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x01,
};

AUDIO_PLL_2_T gaudio_pll_2 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x02,
};

AUDIO_PLL_3_T gaudio_pll_3 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x03,
};

AUDIO_PLL_4_T gaudio_pll_4 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x04,
};

AUDIO_PLL_5_T gaudio_pll_5 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x05,
};

AUDIO_PLL_6_T gaudio_pll_6 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x06,
};

AFE_AAD_0_T gafe_aad_0 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x07,
};

AFE_AAD_1_T gafe_aad_1 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x08,
};

AFE_AAD_2_T gafe_aad_2 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x09,
};

AFE_AAD_3_T gafe_aad_3 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0A,
};

AFE_AAD_4_T gafe_aad_4 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0B,
};

AFE_AAD_5_T gafe_aad_5 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0C,
};

AFE_AAD_6_T gafe_aad_6 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0D,
};

AFE_AAD_7_T gafe_aad_7 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0E,
};

AFE_AAD_8_T gafe_aad_8 =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0F,
};

AFE_ACODEC_0_T gafe_acodec_0 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x00,
};

AFE_ACODEC_1_T gafe_acodec_1 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x01,
};

AFE_ACODEC_2_T gafe_acodec_2 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x02,
};

AFE_ACODEC_3_T gafe_acodec_3 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x03,
};

AFE_ACODEC_4_T gafe_acodec_4 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x04,
};

AFE_ACODEC_5_T gafe_acodec_5 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x05,
};

AFE_HP_DRV_0_T gafe_hp_drv_0 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x06,
};

AFE_HP_DRV_1_T gafe_hp_drv_1 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x07,
};

AFE_HP_DRV_2_T gafe_hp_drv_2 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x08,
};

AFE_HP_DRV_3_T gafe_hp_drv_3 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x09,
};

AFE_HP_DRV_4_T gafe_hp_drv_4 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0A,
};

ACODEC_0_T gacodec_0 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0B,
};

ACODEC_1_T gacodec_1 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0C,
};

ACODEC_2_T gacodec_2 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0D,
};

ACODEC_3_T gacodec_3 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0E,
};

ACODEC_4_T gacodec_4 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0F,
};

ACODEC_5_T gacodec_5 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x10,
};

ACODEC_6_T gacodec_6 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x11,
};

ACODEC_7_T gacodec_7 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x12,
};

ACODEC_8_T gacodec_8 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x13,
};

ACODEC_9_T gacodec_9 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x14,
};

ACODEC_10_T gacodec_10 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x15,
};

ACODEC_11_T gacodec_11 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x16,
};

ACODEC_12_T gacodec_12 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x17,
};

ACODEC_13_T gacodec_13 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x18,
};

ACODEC_14_T gacodec_14 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x19,
};

ACODEC_15_T gacodec_15 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1A,
};

ACODEC_16_T gacodec_16 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1B,
};

ACODEC_17_T gacodec_17 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1C,
};

ACODEC_18_T gacodec_18 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1D,
};

ACODEC_19_T gacodec_19 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1E,
};

ACODEC_20_T gacodec_20 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1F,
};

ACODEC_21_T gacodec_21 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x20,
};

ACODEC_22_T gacodec_22 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x21,
};

ACODEC_23_T gacodec_23 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x22,
};

ACODEC_24_T gacodec_24 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x23,
};

ACODEC_25_T gacodec_25 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x24,
};

ACODEC_26_T gacodec_26 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x25,
};

ACODEC_27_T gacodec_27 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x26,
};

ACODEC_28_T gacodec_28 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x27,
};

ACODEC_29_T gacodec_29 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x28,
};

ACODEC_30_T gacodec_30 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x29,
};

ACODEC_31_T gacodec_31 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2A,
};

ACODEC_32_T gacodec_32 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2B,
};

ACODEC_33_T gacodec_33 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2C,
};

ACODEC_34_T gacodec_34 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2D,
};

ACODEC_35_T gacodec_35 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2E,
};

ACODEC_36_T gacodec_36 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2F,
};

ACODEC_37_T gacodec_37 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x30,
};

ACODEC_38_T gacodec_38 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x31,
};

ACODEC_39_T gacodec_39 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x32,
};

ACODEC_40_T gacodec_40 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x33,
};

ACODEC_41_T gacodec_41 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x34,
};

ACODEC_42_T gacodec_42 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x35,
};

ACODEC_43_T gacodec_43 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x36,
};

ACODEC_44_T gacodec_44 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x37,
};

ACODEC_45_T gacodec_45 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x38,
};

ACODEC_46_T gacodec_46 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x39,
};

ACODEC_47_T gacodec_47 =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x3A,
};

HDMI_PHY_N_01_T ghdmi_phy_n_01 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x01,
};

HDMI_PHY_N_02_T ghdmi_phy_n_02 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x02,
};

HDMI_PHY_N_03_T ghdmi_phy_n_03 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x03,
};

HDMI_PHY_N_04_T ghdmi_phy_n_04 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x04,
};

HDMI_PHY_N_05_T ghdmi_phy_n_05 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x05,
};

HDMI_PHY_N_06_T ghdmi_phy_n_06 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x06,
};

HDMI_PHY_N_07_T ghdmi_phy_n_07 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x07,
};

HDMI_PHY_N_08_T ghdmi_phy_n_08 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x08,
};

HDMI_PHY_N_09_T ghdmi_phy_n_09 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x09,
};

HDMI_PHY_N_0A_T ghdmi_phy_n_0a =
{
	.slaveAddr = 0x38,
	.regAddr = 0x0A,
};

HDMI_PHY_N_10_T ghdmi_phy_n_10 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x10,
};

HDMI_PHY_N_11_T ghdmi_phy_n_11 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x11,
};

HDMI_PHY_N_12_T ghdmi_phy_n_12 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x12,
};

HDMI_PHY_N_13_T ghdmi_phy_n_13 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x13,
};

HDMI_PHY_N_14_T ghdmi_phy_n_14 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x14,
};

HDMI_PHY_N_15_T ghdmi_phy_n_15 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x15,
};

HDMI_PHY_N_16_T ghdmi_phy_n_16 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x16,
};

HDMI_PHY_N_17_T ghdmi_phy_n_17 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x17,
};

HDMI_PHY_N_18_T ghdmi_phy_n_18 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x18,
};

HDMI_PHY_N_19_T ghdmi_phy_n_19 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x19,
};

HDMI_PHY_N_1A_T ghdmi_phy_n_1a =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1A,
};

HDMI_PHY_N_1B_T ghdmi_phy_n_1b =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1B,
};

HDMI_PHY_N_1C_T ghdmi_phy_n_1c =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1C,
};

HDMI_PHY_N_1D_T ghdmi_phy_n_1d =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1D,
};

HDMI_PHY_N_1E_T ghdmi_phy_n_1e =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1E,
};

HDMI_PHY_N_1F_T ghdmi_phy_n_1f =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1F,
};

HDMI_PHY_N_20_T ghdmi_phy_n_20 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x20,
};

HDMI_PHY_N_21_T ghdmi_phy_n_21 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x21,
};

HDMI_PHY_N_22_T ghdmi_phy_n_22 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x22,
};

HDMI_PHY_N_23_T ghdmi_phy_n_23 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x23,
};

HDMI_PHY_N_24_T ghdmi_phy_n_24 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x24,
};

HDMI_PHY_N_25_T ghdmi_phy_n_25 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x25,
};

HDMI_PHY_N_26_T ghdmi_phy_n_26 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x26,
};

HDMI_PHY_N_30_T ghdmi_phy_n_30 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x30,
};

HDMI_PHY_N_31_T ghdmi_phy_n_31 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x31,
};

HDMI_PHY_N_32_T ghdmi_phy_n_32 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x32,
};

HDMI_PHY_N_33_T ghdmi_phy_n_33 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x33,
};

HDMI_PHY_N_34_T ghdmi_phy_n_34 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x34,
};

HDMI_PHY_N_35_T ghdmi_phy_n_35 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x35,
};

HDMI_PHY_N_36_T ghdmi_phy_n_36 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x36,
};

HDMI_PHY_N_40_T ghdmi_phy_n_40 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x40,
};

HDMI_PHY_N_41_T ghdmi_phy_n_41 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x41,
};

HDMI_PHY_N_42_T ghdmi_phy_n_42 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x42,
};

HDMI_PHY_N_43_T ghdmi_phy_n_43 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x43,
};

HDMI_PHY_N_44_T ghdmi_phy_n_44 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x44,
};

HDMI_PHY_N_45_T ghdmi_phy_n_45 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x45,
};

HDMI_PHY_N_46_T ghdmi_phy_n_46 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x46,
};

HDMI_PHY_N_47_T ghdmi_phy_n_47 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x47,
};

HDMI_PHY_N_48_T ghdmi_phy_n_48 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x48,
};

HDMI_PHY_N_49_T ghdmi_phy_n_49 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x49,
};

HDMI_PHY_N_4A_T ghdmi_phy_n_4a =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4A,
};

HDMI_PHY_N_4B_T ghdmi_phy_n_4b =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4B,
};

HDMI_PHY_N_4C_T ghdmi_phy_n_4c =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4C,
};

HDMI_PHY_N_4D_T ghdmi_phy_n_4d =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4D,
};

HDMI_PHY_N_4E_T ghdmi_phy_n_4e =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4E,
};

HDMI_PHY_N_4F_T ghdmi_phy_n_4f =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4F,
};

HDMI_PHY_N_50_T ghdmi_phy_n_50 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x50,
};

HDMI_PHY_N_51_T ghdmi_phy_n_51 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x51,
};

HDMI_PHY_N_52_T ghdmi_phy_n_52 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x52,
};

HDMI_PHY_N_53_T ghdmi_phy_n_53 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x53,
};

HDMI_PHY_N_54_T ghdmi_phy_n_54 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x54,
};

HDMI_PHY_N_80_T ghdmi_phy_n_80 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x80,
};

HDMI_PHY_N_81_T ghdmi_phy_n_81 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x81,
};

HDMI_PHY_N_82_T ghdmi_phy_n_82 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x82,
};

HDMI_PHY_N_83_T ghdmi_phy_n_83 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x83,
};

HDMI_PHY_N_84_T ghdmi_phy_n_84 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x84,
};

HDMI_PHY_N_85_T ghdmi_phy_n_85 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x85,
};

HDMI_PHY_N_86_T ghdmi_phy_n_86 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x86,
};

HDMI_PHY_N_87_T ghdmi_phy_n_87 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x87,
};

HDMI_PHY_N_88_T ghdmi_phy_n_88 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x88,
};

HDMI_PHY_N_89_T ghdmi_phy_n_89 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x89,
};

HDMI_PHY_N_8A_T ghdmi_phy_n_8a =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8A,
};

HDMI_PHY_N_8B_T ghdmi_phy_n_8b =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8B,
};

HDMI_PHY_N_8C_T ghdmi_phy_n_8c =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8C,
};

HDMI_PHY_N_8D_T ghdmi_phy_n_8d =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8D,
};

HDMI_PHY_N_8E_T ghdmi_phy_n_8e =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8E,
};

HDMI_PHY_N_8F_T ghdmi_phy_n_8f =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8F,
};

HDMI_PHY_N_90_T ghdmi_phy_n_90 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x90,
};

HDMI_PHY_N_91_T ghdmi_phy_n_91 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x91,
};

HDMI_PHY_N_92_T ghdmi_phy_n_92 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x92,
};

HDMI_PHY_N_93_T ghdmi_phy_n_93 =
{
	.slaveAddr = 0x38,
	.regAddr = 0x93,
};

HDMI_PHY_N_A0_T ghdmi_phy_n_a0 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA0,
};

HDMI_PHY_N_A1_T ghdmi_phy_n_a1 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA1,
};

HDMI_PHY_N_A2_T ghdmi_phy_n_a2 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA2,
};

HDMI_PHY_N_A3_T ghdmi_phy_n_a3 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA3,
};

HDMI_PHY_N_A4_T ghdmi_phy_n_a4 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA4,
};

HDMI_PHY_N_A5_T ghdmi_phy_n_a5 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA5,
};

HDMI_PHY_N_A6_T ghdmi_phy_n_a6 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA6,
};

HDMI_PHY_N_A7_T ghdmi_phy_n_a7 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA7,
};

HDMI_PHY_N_A8_T ghdmi_phy_n_a8 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA8,
};

HDMI_PHY_N_A9_T ghdmi_phy_n_a9 =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA9,
};

HDMI_PHY_N_AA_T ghdmi_phy_n_aa =
{
	.slaveAddr = 0x38,
	.regAddr = 0xAA,
};

UINT32 VPORT_I2C_Read(UINT32* reg)
{
	VPORT_I2C_DATA_T* data = (VPORT_I2C_DATA_T*)reg;
	UINT8 tmpData = 0;

	ACE_RegRead((data->slaveAddr)<<1, data->regAddr, &tmpData);
	data->data = tmpData;

	return 0;
}

UINT32 VPORT_I2C_Write(UINT32* reg)
{
	VPORT_I2C_DATA_T* data = (VPORT_I2C_DATA_T*)reg;

#if 0	//jongsang.oh : 2011.06.29
	if(data->slaveAddr == 0x10)
		printk("WR addr = 0x%2X reg = 0x%2X data = 0x%2X\n",data->slaveAddr,data->regAddr, data->data);

	if(data->slaveAddr == 0x1D)
		printk("WR addr = 0x%2X reg = 0x%2X data = 0x%2X\n",data->slaveAddr,data->regAddr, data->data);
#endif

	ACE_RegWrite((data->slaveAddr)<<1, data->regAddr, data->data);

#if 0	//jongsang.oh : 2011.06.29

	UINT8 tmpData = 0;

	I2C_DevRegRead((data->slaveAddr)<<1, data->regAddr, &tmpData, 3);
	data->data = tmpData;

	if(data->slaveAddr == 0x10)
		printk("RD addr = 0x%2X reg = 0x%2X data = 0x%2X\n",data->slaveAddr,data->regAddr, data->data);

	if(data->slaveAddr == 0x1D)
		printk("RD addr = 0x%2X reg = 0x%2X data = 0x%2X\n",data->slaveAddr,data->regAddr, data->data);
#endif

	return 0;
}

