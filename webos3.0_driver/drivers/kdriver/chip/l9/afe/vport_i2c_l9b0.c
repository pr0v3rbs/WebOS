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
 *  @author		won.hur
 *  @version	1.0
 *  @date		2011-08-16
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "vport_i2c_l9b0.h"
#include "i2c_core.h"
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


L9A_VERSION_0_Bx_T gl9a_version_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x00,
};
HOSTIF_CONTROL_0_Bx_T ghostif_control_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x01,
};
L9A_HOSTIF_INTR_0_Bx_T gl9a_hostif_intr_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x02,
};
L9A_HOSTIF_INTR_EN_0_Bx_T gl9a_hostif_intr_en_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x03,
};
TOP_CONTROL_0_Bx_T gtop_control_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x04,
};
TOP_CONTROL_1_Bx_T gtop_control_1_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x05,
};
TOP_CONTROL_2_Bx_T gtop_control_2_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x06,
};
SOFT_RESET_0_Bx_T gsoft_reset_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x07,
};
SOFT_RESET_1_Bx_T gsoft_reset_1_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x08,
};
SOFT_RESET_2_Bx_T gsoft_reset_2_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x09,
};
SOFT_RESET_3_Bx_T gsoft_reset_3_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0A,
};
SOFT_RESET_4_Bx_T gsoft_reset_4_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0B,
};
SOFT_RESET_5_Bx_T gsoft_reset_5_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0C,
};
SOFT_RESET_6_Bx_T gsoft_reset_6_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0D,
};
SOFT_RESET_7_Bx_T gsoft_reset_7_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0E,
};
SOFT_RESET_8_Bx_T gsoft_reset_8_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x0F,
};
SOFT_RESET_9_Bx_T gsoft_reset_9_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x10,
};
SOFT_RESET_10_Bx_T gsoft_reset_10_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x11,
};
SOFT_RESET_11_Bx_T gsoft_reset_11_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x12,
};
SOFT_RESET_12_Bx_T gsoft_reset_12_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x13,
};
SOFT_RESET_13_Bx_T gsoft_reset_13_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x14,
};
SOFT_RESET_14_Bx_T gsoft_reset_14_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x15,
};
SOFT_RESET_15_Bx_T gsoft_reset_15_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x16,
};
SOFT_RESET_16_Bx_T gsoft_reset_16_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x17,
};
SOFT_RESET_17_Bx_T gsoft_reset_17_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x18,
};
CLOCK_CONTROL_0_Bx_T gclock_control_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x19,
};
CLOCK_CONTROL_1_Bx_T gclock_control_1_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1A,
};
CLOCK_CONTROL_2_Bx_T gclock_control_2_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1B,
};
AUDIO_DTO_0_Bx_T gaudio_dto_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1C,
};
AUDIO_DTO_1_Bx_T gaudio_dto_1_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1D,
};
AUDIO_DTO_2_Bx_T gaudio_dto_2_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1E,
};
AUDIO_DTO_3_Bx_T gaudio_dto_3_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x1F,
};
AUDIO_DTO_4_Bx_T gaudio_dto_4_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x20,
};
AUDIO_DTO_5_Bx_T gaudio_dto_5_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x21,
};
AUDIO_DTO_6_Bx_T gaudio_dto_6_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x22,
};
AUDIO_DTO_7_Bx_T gaudio_dto_7_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x23,
};
AUDIO_DTO_8_Bx_T gaudio_dto_8_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x24,
};
DATA_BRIDGE_0_Bx_T gdata_bridge_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x25,
};
DATA_BRIDGE_1_Bx_T gdata_bridge_1_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x26,
};
DATA_BRIDGE_2_Bx_T gdata_bridge_2_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x27,
};
DATA_BRIDGE_3_Bx_T gdata_bridge_3_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x28,
};
DATA_BRIDGE_4_Bx_T gdata_bridge_4_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x29,
};
DATA_BRIDGE_5_Bx_T gdata_bridge_5_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2A,
};
DATA_BRIDGE_6_Bx_T gdata_bridge_6_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2B,
};
DATA_BRIDGE_7_Bx_T gdata_bridge_7_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2C,
};
DATA_BRIDGE_8_Bx_T gdata_bridge_8_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2D,
};
DATA_BRIDGE_9_Bx_T gdata_bridge_9_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2E,
};
DATA_BRIDGE_10_Bx_T gdata_bridge_10_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x2F,
};
DATA_BRIDGE_11_Bx_T gdata_bridge_11_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x30,
};
DATA_BRIDGE_12_Bx_T gdata_bridge_12_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x31,
};
DATA_BRIDGE_13_Bx_T gdata_bridge_13_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x32,
};
DATA_BRIDGE_14_Bx_T gdata_bridge_14_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x33,
};
DATA_BRIDGE_15_Bx_T gdata_bridge_15_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x34,
};
DATA_BRIDGE_16_Bx_T gdata_bridge_16_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x35,
};
DATA_BRIDGE_17_Bx_T gdata_bridge_17_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x36,
};
DATA_BRIDGE_18_Bx_T gdata_bridge_18_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x37,
};
DATA_BRIDGE_19_Bx_T gdata_bridge_19_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x38,
};
DATA_BRIDGE_20_Bx_T gdata_bridge_20_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x39,
};
SOFT_RESET_18_Bx_T gsoft_reset_18_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3A,
};
SOFT_RESET_19_Bx_T gsoft_reset_19_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3B,
};
DATA_BRIDGE_21_Bx_T gdata_bridge_21_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3C,
};
DATA_BRIDGE_22_Bx_T gdata_bridge_22_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3D,
};
DATA_BRIDGE_23_Bx_T gdata_bridge_23_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3E,
};
DATA_BRIDGE_24_Bx_T gdata_bridge_24_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x3F,
};
DATA_BRIDGE_25_Bx_T gdata_bridge_25_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x40,
};
DATA_BRIDGE_26_Bx_T gdata_bridge_26_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x41,
};
DATA_BRIDGE_27_Bx_T gdata_bridge_27_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x42,
};
DATA_BRIDGE_28_Bx_T gdata_bridge_28_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x43,
};
DATA_BRIDGE_29_Bx_T gdata_bridge_29_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x44,
};
DATA_BRIDGE_30_Bx_T gdata_bridge_30_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x45,
};
DATA_BRIDGE_31_Bx_T gdata_bridge_31_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x46,
};
DATA_BRIDGE_32_Bx_T gdata_bridge_32_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x47,
};
DATA_BRIDGE_33_Bx_T gdata_bridge_33_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x48,
};
DATA_BRIDGE_34_Bx_T gdata_bridge_34_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x49,
};
DATA_BRIDGE_35_Bx_T gdata_bridge_35_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4A,
};
DATA_BRIDGE_36_Bx_T gdata_bridge_36_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4B,
};
DATA_BRIDGE_37_Bx_T gdata_bridge_37_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4C,
};
DATA_BRIDGE_38_Bx_T gdata_bridge_38_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4D,
};
DATA_BRIDGE_39_Bx_T gdata_bridge_39_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4E,
};
DATA_BRIDGE_40_Bx_T gdata_bridge_40_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x4F,
};
DATA_BRIDGE_41_Bx_T gdata_bridge_41_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x50,
};
DATA_BRIDGE_42_Bx_T gdata_bridge_42_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x51,
};
DATA_BRIDGE_43_Bx_T gdata_bridge_43_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x52,
};
DATA_BRIDGE_44_Bx_T gdata_bridge_44_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x53,
};
DATA_BRIDGE_45_Bx_T gdata_bridge_45_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x54,
};
DATA_BRIDGE_46_Bx_T gdata_bridge_46_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x55,
};
DATA_BRIDGE_47_Bx_T gdata_bridge_47_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x56,
};
DATA_BRIDGE_48_Bx_T gdata_bridge_48_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x57,
};
DATA_BRIDGE_49_Bx_T gdata_bridge_49_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x58,
};
DATA_BRIDGE_50_Bx_T gdata_bridge_50_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x59,
};
DATA_BRIDGE_51_Bx_T gdata_bridge_51_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0x5A,
};
AUD_CLOCK_INV_0_Bx_T gaud_clock_inv_0_bx =
{
	.slaveAddr = 0x10,
	.regAddr = 0xFF,
};
HSLVDSTX0_0_Bx_T ghslvdstx0_0_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x00,
};
HSLVDSTX0_1_Bx_T ghslvdstx0_1_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x01,
};
HSLVDSTX0_2_Bx_T ghslvdstx0_2_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x02,
};
HSLVDSTX0_3_Bx_T ghslvdstx0_3_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x03,
};
HSLVDSTX0_4_Bx_T ghslvdstx0_4_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x04,
};
HSLVDSTX0_5_Bx_T ghslvdstx0_5_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x05,
};
HSLVDSTX0_6_Bx_T ghslvdstx0_6_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x06,
};
HSLVDSTX1_0_Bx_T ghslvdstx1_0_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x07,
};
HSLVDSTX1_1_Bx_T ghslvdstx1_1_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x08,
};
HSLVDSTX1_2_Bx_T ghslvdstx1_2_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x09,
};
HSLVDSTX1_3_Bx_T ghslvdstx1_3_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0A,
};
HSLVDSTX1_4_Bx_T ghslvdstx1_4_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0B,
};
HSLVDSTX1_5_Bx_T ghslvdstx1_5_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0C,
};
HSLVDSTX1_6_Bx_T ghslvdstx1_6_bx =
{
	.slaveAddr = 0x11,
	.regAddr = 0x0D,
};
GBB_AFE_0_Bx_T ggbb_afe_0_bx =
{
	.slaveAddr = 0x12,
	.regAddr = 0x00,
};
GBB_0_Bx_T ggbb_0_bx =
{
	.slaveAddr = 0x12,
	.regAddr = 0x01,
};
AFE_CVBS_0_Bx_T gafe_cvbs_0_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x00,
};
AFE_CVBS_1_Bx_T gafe_cvbs_1_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x01,
};
AFE_CVBS_2_Bx_T gafe_cvbs_2_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x02,
};
AFE_CVBS_3_Bx_T gafe_cvbs_3_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x03,
};
AFE_VBUF_0_Bx_T gafe_vbuf_0_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x04,
};
AFE_VBUF_1_Bx_T gafe_vbuf_1_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x05,
};
AFE_VDAC_0_Bx_T gafe_vdac_0_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x06,
};
AFE_VDAC_1_Bx_T gafe_vdac_1_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x07,
};
MAIN_PLL_0_Bx_T gmain_pll_0_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x08,
};
MAIN_PLL_1_Bx_T gmain_pll_1_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x09,
};
MAIN_PLL_2_Bx_T gmain_pll_2_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0A,
};
MAIN_PLL_3_Bx_T gmain_pll_3_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0B,
};
MAIN_PLL_4_Bx_T gmain_pll_4_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0C,
};
AFE_CB_0_Bx_T gafe_cb_0_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0D,
};
AFE_CB_1_Bx_T gafe_cb_1_bx =
{
	.slaveAddr = 0x13,
	.regAddr = 0x0E,
};
HPD_OUT_CONTROL_0_Bx_T ghpd_out_control_0_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x00,
};
HPD_OUT_CONTROL_1_Bx_T ghpd_out_control_1_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x01,
};
HPD_OUT_CONTROL_2_Bx_T ghpd_out_control_2_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x02,
};
HDMI_PHY0_0_Bx_T ghdmi_phy0_0_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x03,
};
HDMI_PHY0_1_Bx_T ghdmi_phy0_1_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x04,
};
HDMI_PHY0_2_Bx_T ghdmi_phy0_2_bx =
{
	.slaveAddr = 0x14,
	.regAddr = 0x05,
};
L9A_HDMI0_INTR_0_Bx_T gl9a_hdmi0_intr_0_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x00,
};
L9A_HDMI0_INTR_1_Bx_T gl9a_hdmi0_intr_1_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x01,
};
L9A_HDMI0_INTR_2_Bx_T gl9a_hdmi0_intr_2_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x02,
};
L9A_HDMI0_INTR_3_Bx_T gl9a_hdmi0_intr_3_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x03,
};
L9A_HDMI0_INTR_4_Bx_T gl9a_hdmi0_intr_4_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x04,
};
L9A_HDMI0_INTR_5_Bx_T gl9a_hdmi0_intr_5_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x05,
};
L9A_HDMI0_INTR_6_Bx_T gl9a_hdmi0_intr_6_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x06,
};
L9A_HDMI0_INTR_EN_0_Bx_T gl9a_hdmi0_intr_en_0_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x07,
};
L9A_HDMI0_INTR_EN_1_Bx_T gl9a_hdmi0_intr_en_1_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x08,
};
L9A_HDMI0_INTR_EN_2_Bx_T gl9a_hdmi0_intr_en_2_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x09,
};
L9A_HDMI0_INTR_EN_3_Bx_T gl9a_hdmi0_intr_en_3_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0A,
};
L9A_HDMI0_INTR_EN_4_Bx_T gl9a_hdmi0_intr_en_4_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0B,
};
L9A_HDMI0_INTR_EN_5_Bx_T gl9a_hdmi0_intr_en_5_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0C,
};
L9A_HDMI0_INTR_EN_6_Bx_T gl9a_hdmi0_intr_en_6_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x0D,
};
LINK_0_Bx_T glink_0_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x18,
};
LINK_1_Bx_T glink_1_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x19,
};
LINK_2_Bx_T glink_2_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1A,
};
LINK_3_Bx_T glink_3_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1B,
};
LINK_4_Bx_T glink_4_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1C,
};
LINK_5_Bx_T glink_5_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1D,
};
LINK_6_Bx_T glink_6_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1E,
};
LINK_7_Bx_T glink_7_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x1F,
};
LINK_8_Bx_T glink_8_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x20,
};
LINK_9_Bx_T glink_9_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x21,
};
LINK_10_Bx_T glink_10_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x22,
};
LINK_11_Bx_T glink_11_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x23,
};
LINK_12_Bx_T glink_12_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x24,
};
LINK_13_Bx_T glink_13_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x25,
};
LINK_14_Bx_T glink_14_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x26,
};
LINK_15_Bx_T glink_15_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x27,
};
LINK_16_Bx_T glink_16_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x28,
};
LINK_17_Bx_T glink_17_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x29,
};
LINK_18_Bx_T glink_18_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2A,
};
LINK_19_Bx_T glink_19_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2B,
};
LINK_20_Bx_T glink_20_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2C,
};
LINK_21_Bx_T glink_21_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2D,
};
LINK_22_Bx_T glink_22_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2E,
};
LINK_23_Bx_T glink_23_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x2F,
};
LINK_24_Bx_T glink_24_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x30,
};
LINK_25_Bx_T glink_25_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x31,
};
LINK_26_Bx_T glink_26_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x32,
};
LINK_27_Bx_T glink_27_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x33,
};
LINK_28_Bx_T glink_28_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x34,
};
LINK_29_Bx_T glink_29_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x35,
};
LINK_30_Bx_T glink_30_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x36,
};
LINK_31_Bx_T glink_31_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x37,
};
LINK_32_Bx_T glink_32_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x38,
};
LINK_33_Bx_T glink_33_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x39,
};
LINK_34_Bx_T glink_34_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3A,
};
LINK_35_Bx_T glink_35_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3B,
};
LINK_36_Bx_T glink_36_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3C,
};
LINK_37_Bx_T glink_37_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3D,
};
LINK_38_Bx_T glink_38_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3E,
};
LINK_39_Bx_T glink_39_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x3F,
};
LINK_40_Bx_T glink_40_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x40,
};
LINK_41_Bx_T glink_41_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x41,
};
LINK_42_Bx_T glink_42_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x42,
};
LINK_43_Bx_T glink_43_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x43,
};
LINK_44_Bx_T glink_44_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x44,
};
LINK_45_Bx_T glink_45_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x45,
};
LINK_46_Bx_T glink_46_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x46,
};
LINK_47_Bx_T glink_47_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x47,
};
LINK_48_Bx_T glink_48_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x48,
};
LINK_49_Bx_T glink_49_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x49,
};
LINK_50_Bx_T glink_50_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4A,
};
LINK_51_Bx_T glink_51_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4B,
};
LINK_52_Bx_T glink_52_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4C,
};
LINK_53_Bx_T glink_53_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4D,
};
LINK_54_Bx_T glink_54_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4E,
};
LINK_55_Bx_T glink_55_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x4F,
};
LINK_56_Bx_T glink_56_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x50,
};
LINK_57_Bx_T glink_57_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x51,
};
LINK_58_Bx_T glink_58_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x52,
};
LINK_59_Bx_T glink_59_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x53,
};
LINK_60_Bx_T glink_60_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x54,
};
LINK_61_Bx_T glink_61_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x55,
};
LINK_62_Bx_T glink_62_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x56,
};
LINK_63_Bx_T glink_63_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x57,
};
LINK_64_Bx_T glink_64_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x58,
};
LINK_65_Bx_T glink_65_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x59,
};
LINK_66_Bx_T glink_66_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5A,
};
LINK_67_Bx_T glink_67_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5B,
};
LINK_68_Bx_T glink_68_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5C,
};
LINK_69_Bx_T glink_69_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5D,
};
LINK_70_Bx_T glink_70_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5E,
};
LINK_71_Bx_T glink_71_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x5F,
};
LINK_72_Bx_T glink_72_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x60,
};
LINK_73_Bx_T glink_73_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x61,
};
LINK_74_Bx_T glink_74_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x62,
};
LINK_75_Bx_T glink_75_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x63,
};
LINK_76_Bx_T glink_76_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x64,
};
LINK_77_Bx_T glink_77_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x65,
};
LINK_78_Bx_T glink_78_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x66,
};
LINK_79_Bx_T glink_79_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x67,
};
LINK_80_Bx_T glink_80_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x68,
};
LINK_81_Bx_T glink_81_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x69,
};
LINK_82_Bx_T glink_82_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6A,
};
LINK_83_Bx_T glink_83_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6B,
};
LINK_84_Bx_T glink_84_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6C,
};
LINK_85_Bx_T glink_85_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6D,
};
LINK_86_Bx_T glink_86_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6E,
};
LINK_87_Bx_T glink_87_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x6F,
};
LINK_88_Bx_T glink_88_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x70,
};
LINK_89_Bx_T glink_89_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x71,
};
LINK_90_Bx_T glink_90_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x72,
};
LINK_91_Bx_T glink_91_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x73,
};
LINK_92_Bx_T glink_92_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x74,
};
LINK_93_Bx_T glink_93_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x75,
};
LINK_94_Bx_T glink_94_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x76,
};
LINK_95_Bx_T glink_95_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x77,
};
LINK_96_Bx_T glink_96_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x78,
};
LINK_97_Bx_T glink_97_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x79,
};
LINK_98_Bx_T glink_98_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7A,
};
LINK_99_Bx_T glink_99_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7B,
};
LINK_100_Bx_T glink_100_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7C,
};
LINK_101_Bx_T glink_101_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7D,
};
LINK_102_Bx_T glink_102_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7E,
};
LINK_103_Bx_T glink_103_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x7F,
};
LINK_104_Bx_T glink_104_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x80,
};
LINK_105_Bx_T glink_105_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x81,
};
LINK_106_Bx_T glink_106_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x82,
};
LINK_107_Bx_T glink_107_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x83,
};
LINK_108_Bx_T glink_108_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x84,
};
LINK_109_Bx_T glink_109_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x85,
};
LINK_110_Bx_T glink_110_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x86,
};
LINK_111_Bx_T glink_111_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x87,
};
LINK_112_Bx_T glink_112_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x88,
};
LINK_113_Bx_T glink_113_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x89,
};
LINK_114_Bx_T glink_114_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8A,
};
LINK_115_Bx_T glink_115_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8B,
};
LINK_116_Bx_T glink_116_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8C,
};
LINK_117_Bx_T glink_117_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8D,
};
LINK_118_Bx_T glink_118_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8E,
};
LINK_119_Bx_T glink_119_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x8F,
};
LINK_120_Bx_T glink_120_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x90,
};
LINK_121_Bx_T glink_121_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x91,
};
LINK_122_Bx_T glink_122_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x92,
};
LINK_123_Bx_T glink_123_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x93,
};
LINK_124_Bx_T glink_124_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x94,
};
LINK_125_Bx_T glink_125_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x95,
};
LINK_126_Bx_T glink_126_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x96,
};
LINK_127_Bx_T glink_127_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x97,
};
LINK_128_Bx_T glink_128_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x98,
};
LINK_129_Bx_T glink_129_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x99,
};
LINK_130_Bx_T glink_130_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9A,
};
LINK_131_Bx_T glink_131_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9B,
};
LINK_132_Bx_T glink_132_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9C,
};
LINK_133_Bx_T glink_133_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9D,
};
LINK_134_Bx_T glink_134_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9E,
};
LINK_135_Bx_T glink_135_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0x9F,
};
LINK_136_Bx_T glink_136_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA0,
};
LINK_137_Bx_T glink_137_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA1,
};
LINK_138_Bx_T glink_138_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA2,
};
LINK_139_Bx_T glink_139_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA3,
};
LINK_140_Bx_T glink_140_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA4,
};
LINK_141_Bx_T glink_141_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA5,
};
LINK_142_Bx_T glink_142_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA6,
};
LINK_143_Bx_T glink_143_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA7,
};
LINK_144_Bx_T glink_144_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA8,
};
LINK_145_Bx_T glink_145_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xA9,
};
LINK_146_Bx_T glink_146_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAA,
};
LINK_147_Bx_T glink_147_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAB,
};
LINK_148_Bx_T glink_148_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAC,
};
LINK_149_Bx_T glink_149_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAD,
};
LINK_150_Bx_T glink_150_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAE,
};
LINK_151_Bx_T glink_151_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xAF,
};
LINK_152_Bx_T glink_152_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB0,
};
LINK_153_Bx_T glink_153_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB1,
};
LINK_154_Bx_T glink_154_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB2,
};
LINK_155_Bx_T glink_155_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB3,
};
LINK_156_Bx_T glink_156_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB4,
};
LINK_157_Bx_T glink_157_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB5,
};
LINK_158_Bx_T glink_158_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB6,
};
LINK_159_Bx_T glink_159_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB7,
};
LINK_160_Bx_T glink_160_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB8,
};
LINK_161_Bx_T glink_161_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xB9,
};
LINK_162_Bx_T glink_162_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBA,
};
LINK_163_Bx_T glink_163_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBB,
};
LINK_164_Bx_T glink_164_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBC,
};
LINK_165_Bx_T glink_165_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBD,
};
LINK_166_Bx_T glink_166_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBE,
};
LINK_167_Bx_T glink_167_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xBF,
};
LINK_168_Bx_T glink_168_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC0,
};
LINK_169_Bx_T glink_169_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC1,
};
LINK_170_Bx_T glink_170_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC2,
};
LINK_171_Bx_T glink_171_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC3,
};
LINK_172_Bx_T glink_172_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC4,
};
LINK_173_Bx_T glink_173_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC5,
};
LINK_174_Bx_T glink_174_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC6,
};
LINK_175_Bx_T glink_175_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC7,
};
LINK_176_Bx_T glink_176_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC8,
};
LINK_177_Bx_T glink_177_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xC9,
};
LINK_178_Bx_T glink_178_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCA,
};
LINK_179_Bx_T glink_179_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCB,
};
LINK_180_Bx_T glink_180_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCC,
};
LINK_181_Bx_T glink_181_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCD,
};
LINK_182_Bx_T glink_182_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCE,
};
LINK_183_Bx_T glink_183_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xCF,
};
LINK_184_Bx_T glink_184_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD0,
};
LINK_185_Bx_T glink_185_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD1,
};
LINK_186_Bx_T glink_186_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD2,
};
LINK_187_Bx_T glink_187_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD3,
};
LINK_188_Bx_T glink_188_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD4,
};
LINK_189_Bx_T glink_189_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD5,
};
LINK_190_Bx_T glink_190_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD6,
};
LINK_191_Bx_T glink_191_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD7,
};
LINK_192_Bx_T glink_192_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD8,
};
LINK_193_Bx_T glink_193_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xD9,
};
LINK_194_Bx_T glink_194_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDA,
};
LINK_195_Bx_T glink_195_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDB,
};
LINK_196_Bx_T glink_196_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDC,
};
LINK_197_Bx_T glink_197_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDD,
};
LINK_198_Bx_T glink_198_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDE,
};
LINK_199_Bx_T glink_199_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xDF,
};
LINK_200_Bx_T glink_200_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE0,
};
LINK_201_Bx_T glink_201_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE1,
};
LINK_202_Bx_T glink_202_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE2,
};
LINK_203_Bx_T glink_203_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE3,
};
LINK_204_Bx_T glink_204_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE4,
};
LINK_205_Bx_T glink_205_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE5,
};
LINK_206_Bx_T glink_206_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE6,
};
LINK_207_Bx_T glink_207_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE7,
};
LINK_208_Bx_T glink_208_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE8,
};
LINK_209_Bx_T glink_209_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xE9,
};
LINK_210_Bx_T glink_210_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEA,
};
LINK_211_Bx_T glink_211_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEB,
};
LINK_212_Bx_T glink_212_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEC,
};
LINK_213_Bx_T glink_213_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xED,
};
LINK_214_Bx_T glink_214_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEE,
};
LINK_215_Bx_T glink_215_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xEF,
};
LINK_216_Bx_T glink_216_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF0,
};
LINK_217_Bx_T glink_217_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF1,
};
LINK_218_Bx_T glink_218_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF2,
};
LINK_219_Bx_T glink_219_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF3,
};
LINK_220_Bx_T glink_220_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF4,
};
LINK_221_Bx_T glink_221_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF5,
};
LINK_222_Bx_T glink_222_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF6,
};
LINK_223_Bx_T glink_223_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF7,
};
LINK_224_Bx_T glink_224_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF8,
};
LINK_225_Bx_T glink_225_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xF9,
};
LINK_226_Bx_T glink_226_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFA,
};
LINK_227_Bx_T glink_227_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFB,
};
LINK_228_Bx_T glink_228_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFC,
};
LINK_229_Bx_T glink_229_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFD,
};
LINK_230_Bx_T glink_230_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFE,
};
LINK_231_Bx_T glink_231_bx =
{
	.slaveAddr = 0x15,
	.regAddr = 0xFF,
};
LINK_232_Bx_T glink_232_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x00,
};
LINK_233_Bx_T glink_233_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x01,
};
LINK_234_Bx_T glink_234_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x02,
};
LINK_235_Bx_T glink_235_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x03,
};
LINK_236_Bx_T glink_236_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x04,
};
LINK_237_Bx_T glink_237_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x05,
};
LINK_238_Bx_T glink_238_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x06,
};
LINK_239_Bx_T glink_239_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x07,
};
LINK_240_Bx_T glink_240_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x08,
};
LINK_241_Bx_T glink_241_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x09,
};
LINK_242_Bx_T glink_242_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0A,
};
LINK_243_Bx_T glink_243_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0B,
};
LINK_244_Bx_T glink_244_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0C,
};
LINK_245_Bx_T glink_245_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0D,
};
LINK_246_Bx_T glink_246_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0E,
};
LINK_247_Bx_T glink_247_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x0F,
};
LINK_248_Bx_T glink_248_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x10,
};
LINK_249_Bx_T glink_249_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x11,
};
LINK_250_Bx_T glink_250_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x12,
};
LINK_251_Bx_T glink_251_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x13,
};
LINK_252_Bx_T glink_252_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x14,
};
LINK_253_Bx_T glink_253_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x15,
};
LINK_254_Bx_T glink_254_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x16,
};
LINK_255_Bx_T glink_255_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x17,
};
LINK_256_Bx_T glink_256_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x18,
};
LINK_257_Bx_T glink_257_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x19,
};
LINK_258_Bx_T glink_258_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1A,
};
LINK_259_Bx_T glink_259_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1B,
};
LINK_260_Bx_T glink_260_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1C,
};
LINK_261_Bx_T glink_261_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1D,
};
LINK_262_Bx_T glink_262_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1E,
};
LINK_263_Bx_T glink_263_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x1F,
};
LINK_264_Bx_T glink_264_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x20,
};
LINK_265_Bx_T glink_265_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x21,
};
LINK_266_Bx_T glink_266_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x22,
};
LINK_267_Bx_T glink_267_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x23,
};
LINK_268_Bx_T glink_268_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x24,
};
LINK_269_Bx_T glink_269_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x25,
};
LINK_270_Bx_T glink_270_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x26,
};
LINK_271_Bx_T glink_271_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x27,
};
LINK_272_Bx_T glink_272_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x28,
};
LINK_273_Bx_T glink_273_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x29,
};
LINK_274_Bx_T glink_274_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2A,
};
LINK_275_Bx_T glink_275_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2B,
};
LINK_276_Bx_T glink_276_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2C,
};
LINK_277_Bx_T glink_277_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2D,
};
LINK_278_Bx_T glink_278_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2E,
};
LINK_279_Bx_T glink_279_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x2F,
};
LINK_280_Bx_T glink_280_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x30,
};
LINK_281_Bx_T glink_281_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x31,
};
LINK_282_Bx_T glink_282_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x32,
};
LINK_283_Bx_T glink_283_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x33,
};
LINK_284_Bx_T glink_284_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x34,
};
LINK_285_Bx_T glink_285_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x35,
};
LINK_286_Bx_T glink_286_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x36,
};
LINK_287_Bx_T glink_287_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x37,
};
LINK_288_Bx_T glink_288_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x38,
};
LINK_289_Bx_T glink_289_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x39,
};
LINK_290_Bx_T glink_290_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3A,
};
LINK_291_Bx_T glink_291_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3B,
};
LINK_292_Bx_T glink_292_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3C,
};
LINK_293_Bx_T glink_293_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3D,
};
LINK_294_Bx_T glink_294_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3E,
};
LINK_295_Bx_T glink_295_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x3F,
};
LINK_296_Bx_T glink_296_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x40,
};
LINK_297_Bx_T glink_297_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x41,
};
LINK_298_Bx_T glink_298_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x42,
};
LINK_299_Bx_T glink_299_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x43,
};
LINK_300_Bx_T glink_300_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x44,
};
LINK_301_Bx_T glink_301_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x45,
};
LINK_302_Bx_T glink_302_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x46,
};
LINK_303_Bx_T glink_303_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x47,
};
LINK_304_Bx_T glink_304_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x48,
};
LINK_305_Bx_T glink_305_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x49,
};
LINK_306_Bx_T glink_306_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4A,
};
LINK_307_Bx_T glink_307_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4B,
};
LINK_308_Bx_T glink_308_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4C,
};
LINK_309_Bx_T glink_309_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4D,
};
LINK_310_Bx_T glink_310_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4E,
};
LINK_311_Bx_T glink_311_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x4F,
};
LINK_312_Bx_T glink_312_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x50,
};
LINK_313_Bx_T glink_313_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x51,
};
LINK_314_Bx_T glink_314_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x52,
};
LINK_315_Bx_T glink_315_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x53,
};
LINK_316_Bx_T glink_316_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x54,
};
LINK_317_Bx_T glink_317_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x55,
};
LINK_318_Bx_T glink_318_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x56,
};
LINK_319_Bx_T glink_319_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x57,
};
LINK_320_Bx_T glink_320_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x58,
};
LINK_321_Bx_T glink_321_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x59,
};
LINK_322_Bx_T glink_322_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5A,
};
LINK_323_Bx_T glink_323_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5B,
};
LINK_324_Bx_T glink_324_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5C,
};
LINK_325_Bx_T glink_325_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5D,
};
LINK_326_Bx_T glink_326_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5E,
};
LINK_327_Bx_T glink_327_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x5F,
};
LINK_328_Bx_T glink_328_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x60,
};
LINK_329_Bx_T glink_329_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x61,
};
LINK_330_Bx_T glink_330_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x62,
};
LINK_331_Bx_T glink_331_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x63,
};
LINK_332_Bx_T glink_332_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x64,
};
LINK_333_Bx_T glink_333_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x65,
};
LINK_334_Bx_T glink_334_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x66,
};
LINK_335_Bx_T glink_335_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x67,
};
LINK_336_Bx_T glink_336_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x68,
};
LINK_337_Bx_T glink_337_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x69,
};
LINK_338_Bx_T glink_338_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6A,
};
LINK_339_Bx_T glink_339_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6B,
};
LINK_340_Bx_T glink_340_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6C,
};
LINK_341_Bx_T glink_341_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6D,
};
LINK_342_Bx_T glink_342_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6E,
};
LINK_343_Bx_T glink_343_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x6F,
};
LINK_344_Bx_T glink_344_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x70,
};
LINK_345_Bx_T glink_345_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x71,
};
LINK_346_Bx_T glink_346_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x72,
};
LINK_347_Bx_T glink_347_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x73,
};
LINK_348_Bx_T glink_348_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x74,
};
LINK_349_Bx_T glink_349_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x75,
};
LINK_350_Bx_T glink_350_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x76,
};
LINK_351_Bx_T glink_351_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x77,
};
LINK_352_Bx_T glink_352_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x78,
};
LINK_353_Bx_T glink_353_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x79,
};
LINK_354_Bx_T glink_354_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7A,
};
LINK_355_Bx_T glink_355_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7B,
};
LINK_356_Bx_T glink_356_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7C,
};
LINK_357_Bx_T glink_357_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7D,
};
LINK_358_Bx_T glink_358_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7E,
};
LINK_359_Bx_T glink_359_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x7F,
};
LINK_360_Bx_T glink_360_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x80,
};
LINK_361_Bx_T glink_361_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x81,
};
LINK_362_Bx_T glink_362_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x82,
};
LINK_363_Bx_T glink_363_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x83,
};
LINK_367_Bx_T glink_367_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x87,
};
LINK_368_Bx_T glink_368_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x88,
};
LINK_369_Bx_T glink_369_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x89,
};
LINK_370_Bx_T glink_370_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8A,
};
LINK_371_Bx_T glink_371_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8B,
};
LINK_372_Bx_T glink_372_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8C,
};
LINK_373_Bx_T glink_373_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8D,
};
LINK_374_Bx_T glink_374_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8E,
};
LINK_375_Bx_T glink_375_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x8F,
};
LINK_376_Bx_T glink_376_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x90,
};
LINK_377_Bx_T glink_377_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x91,
};
LINK_378_Bx_T glink_378_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x92,
};
LINK_379_Bx_T glink_379_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x93,
};
LINK_380_Bx_T glink_380_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x94,
};
LINK_381_Bx_T glink_381_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x95,
};
LINK_382_Bx_T glink_382_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x96,
};
LINK_383_Bx_T glink_383_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x97,
};
LINK_384_Bx_T glink_384_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x98,
};
LINK_385_Bx_T glink_385_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x99,
};
LINK_386_Bx_T glink_386_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9A,
};
LINK_387_Bx_T glink_387_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9B,
};
LINK_388_Bx_T glink_388_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9C,
};
LINK_389_Bx_T glink_389_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9D,
};
LINK_390_Bx_T glink_390_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9E,
};
LINK_391_Bx_T glink_391_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0x9F,
};
LINK_392_Bx_T glink_392_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA0,
};
LINK_393_Bx_T glink_393_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA1,
};
LINK_394_Bx_T glink_394_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA2,
};
LINK_395_Bx_T glink_395_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA3,
};
LINK_396_Bx_T glink_396_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA4,
};
LINK_397_Bx_T glink_397_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA5,
};
LINK_398_Bx_T glink_398_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA6,
};
LINK_399_Bx_T glink_399_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA7,
};
LINK_400_Bx_T glink_400_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA8,
};
LINK_401_Bx_T glink_401_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xA9,
};
LINK_402_Bx_T glink_402_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAA,
};
LINK_403_Bx_T glink_403_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAB,
};
LINK_404_Bx_T glink_404_bx =
{
	.slaveAddr = 0x16,
	.regAddr = 0xAC,
};
LINK_808_Bx_T glink_808_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x40,
};
LINK_809_Bx_T glink_809_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x41,
};
LINK_810_Bx_T glink_810_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x42,
};
LINK_811_Bx_T glink_811_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x43,
};
LINK_812_Bx_T glink_812_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x44,
};
LINK_813_Bx_T glink_813_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x45,
};
LINK_814_Bx_T glink_814_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x46,
};
LINK_815_Bx_T glink_815_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x47,
};
LINK_816_Bx_T glink_816_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x48,
};
LINK_817_Bx_T glink_817_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x49,
};
LINK_818_Bx_T glink_818_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4A,
};
LINK_819_Bx_T glink_819_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4B,
};
LINK_820_Bx_T glink_820_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4C,
};
LINK_821_Bx_T glink_821_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4D,
};
LINK_822_Bx_T glink_822_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4E,
};
LINK_823_Bx_T glink_823_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x4F,
};
LINK_824_Bx_T glink_824_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x50,
};
LINK_825_Bx_T glink_825_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x51,
};
LINK_826_Bx_T glink_826_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x52,
};
LINK_827_Bx_T glink_827_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x53,
};
LINK_828_Bx_T glink_828_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x54,
};
LINK_829_Bx_T glink_829_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x55,
};
LINK_830_Bx_T glink_830_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x56,
};
LINK_831_Bx_T glink_831_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x57,
};
LINK_832_Bx_T glink_832_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x58,
};
LINK_833_Bx_T glink_833_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x59,
};
LINK_834_Bx_T glink_834_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5A,
};
LINK_835_Bx_T glink_835_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5B,
};
LINK_836_Bx_T glink_836_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5C,
};
LINK_837_Bx_T glink_837_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5D,
};
LINK_838_Bx_T glink_838_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5E,
};
LINK_839_Bx_T glink_839_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x5F,
};
LINK_840_Bx_T glink_840_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x60,
};
LINK_841_Bx_T glink_841_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x61,
};
LINK_842_Bx_T glink_842_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x62,
};
LINK_843_Bx_T glink_843_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x63,
};
LINK_844_Bx_T glink_844_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x64,
};
LINK_845_Bx_T glink_845_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x65,
};
LINK_846_Bx_T glink_846_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x66,
};
LINK_847_Bx_T glink_847_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x67,
};
LINK_848_Bx_T glink_848_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x68,
};
LINK_849_Bx_T glink_849_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x69,
};
LINK_850_Bx_T glink_850_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6A,
};
LINK_851_Bx_T glink_851_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6B,
};
LINK_852_Bx_T glink_852_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6C,
};
LINK_853_Bx_T glink_853_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6D,
};
LINK_854_Bx_T glink_854_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6E,
};
LINK_855_Bx_T glink_855_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x6F,
};
LINK_856_Bx_T glink_856_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x70,
};
LINK_857_Bx_T glink_857_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x71,
};
LINK_858_Bx_T glink_858_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x72,
};
LINK_859_Bx_T glink_859_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x73,
};
LINK_860_Bx_T glink_860_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x74,
};
LINK_861_Bx_T glink_861_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x75,
};
LINK_862_Bx_T glink_862_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x76,
};
LINK_863_Bx_T glink_863_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x77,
};
LINK_864_Bx_T glink_864_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x78,
};
LINK_865_Bx_T glink_865_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x79,
};
LINK_866_Bx_T glink_866_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7A,
};
LINK_867_Bx_T glink_867_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7B,
};
LINK_868_Bx_T glink_868_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7C,
};
LINK_869_Bx_T glink_869_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7D,
};
LINK_870_Bx_T glink_870_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7E,
};
LINK_871_Bx_T glink_871_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x7F,
};
LINK_872_Bx_T glink_872_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x80,
};
LINK_873_Bx_T glink_873_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x81,
};
LINK_874_Bx_T glink_874_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x82,
};
LINK_875_Bx_T glink_875_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x83,
};
LINK_876_Bx_T glink_876_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x84,
};
LINK_877_Bx_T glink_877_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x85,
};
LINK_878_Bx_T glink_878_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x86,
};
LINK_879_Bx_T glink_879_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x87,
};
LINK_880_Bx_T glink_880_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x88,
};
LINK_881_Bx_T glink_881_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x89,
};
LINK_882_Bx_T glink_882_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8A,
};
LINK_883_Bx_T glink_883_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8B,
};
LINK_884_Bx_T glink_884_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8C,
};
LINK_885_Bx_T glink_885_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8D,
};
LINK_886_Bx_T glink_886_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8E,
};
LINK_887_Bx_T glink_887_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x8F,
};
LINK_888_Bx_T glink_888_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x90,
};
LINK_889_Bx_T glink_889_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x91,
};
LINK_890_Bx_T glink_890_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x92,
};
LINK_891_Bx_T glink_891_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x93,
};
LINK_892_Bx_T glink_892_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x94,
};
LINK_893_Bx_T glink_893_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x95,
};
LINK_894_Bx_T glink_894_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x96,
};
LINK_895_Bx_T glink_895_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x97,
};
LINK_896_Bx_T glink_896_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x98,
};
LINK_897_Bx_T glink_897_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x99,
};
LINK_898_Bx_T glink_898_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9A,
};
LINK_899_Bx_T glink_899_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9B,
};
LINK_900_Bx_T glink_900_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9C,
};
LINK_901_Bx_T glink_901_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9D,
};
LINK_902_Bx_T glink_902_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9E,
};
LINK_903_Bx_T glink_903_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0x9F,
};
LINK_904_Bx_T glink_904_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA0,
};
LINK_905_Bx_T glink_905_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA1,
};
LINK_906_Bx_T glink_906_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA2,
};
LINK_907_Bx_T glink_907_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA3,
};
LINK_908_Bx_T glink_908_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA4,
};
LINK_909_Bx_T glink_909_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA5,
};
LINK_910_Bx_T glink_910_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA6,
};
LINK_911_Bx_T glink_911_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA7,
};
LINK_912_Bx_T glink_912_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA8,
};
LINK_913_Bx_T glink_913_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xA9,
};
LINK_914_Bx_T glink_914_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAA,
};
LINK_915_Bx_T glink_915_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAB,
};
LINK_916_Bx_T glink_916_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAC,
};
LINK_917_Bx_T glink_917_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAD,
};
LINK_918_Bx_T glink_918_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAE,
};
LINK_919_Bx_T glink_919_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xAF,
};
LINK_920_Bx_T glink_920_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB0,
};
LINK_921_Bx_T glink_921_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB1,
};
LINK_922_Bx_T glink_922_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB2,
};
LINK_923_Bx_T glink_923_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB3,
};
LINK_924_Bx_T glink_924_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB4,
};
LINK_925_Bx_T glink_925_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB5,
};
LINK_926_Bx_T glink_926_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB6,
};
LINK_927_Bx_T glink_927_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB7,
};
LINK_928_Bx_T glink_928_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB8,
};
LINK_929_Bx_T glink_929_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xB9,
};
LINK_930_Bx_T glink_930_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBA,
};
LINK_931_Bx_T glink_931_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBB,
};
LINK_932_Bx_T glink_932_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBC,
};
LINK_933_Bx_T glink_933_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBD,
};
LINK_934_Bx_T glink_934_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBE,
};
LINK_935_Bx_T glink_935_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xBF,
};
LINK_936_Bx_T glink_936_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC0,
};
LINK_937_Bx_T glink_937_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC1,
};
LINK_938_Bx_T glink_938_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC2,
};
LINK_939_Bx_T glink_939_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC3,
};
LINK_940_Bx_T glink_940_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC4,
};
LINK_941_Bx_T glink_941_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC5,
};
LINK_942_Bx_T glink_942_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC6,
};
LINK_943_Bx_T glink_943_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC7,
};
LINK_944_Bx_T glink_944_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC8,
};
LINK_945_Bx_T glink_945_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xC9,
};
LINK_946_Bx_T glink_946_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCA,
};
LINK_947_Bx_T glink_947_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCB,
};
LINK_948_Bx_T glink_948_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCC,
};
LINK_949_Bx_T glink_949_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCD,
};
LINK_950_Bx_T glink_950_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCE,
};
LINK_951_Bx_T glink_951_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xCF,
};
LINK_952_Bx_T glink_952_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD0,
};
LINK_953_Bx_T glink_953_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD1,
};
LINK_954_Bx_T glink_954_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD2,
};
LINK_955_Bx_T glink_955_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD3,
};
LINK_956_Bx_T glink_956_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD4,
};
LINK_957_Bx_T glink_957_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD5,
};
LINK_958_Bx_T glink_958_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD6,
};
LINK_959_Bx_T glink_959_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD7,
};
LINK_960_Bx_T glink_960_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD8,
};
LINK_961_Bx_T glink_961_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xD9,
};
LINK_962_Bx_T glink_962_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDA,
};
LINK_963_Bx_T glink_963_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDB,
};
LINK_964_Bx_T glink_964_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDC,
};
LINK_965_Bx_T glink_965_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDD,
};
LINK_966_Bx_T glink_966_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDE,
};
LINK_967_Bx_T glink_967_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xDF,
};
LINK_968_Bx_T glink_968_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE0,
};
LINK_969_Bx_T glink_969_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE1,
};
LINK_970_Bx_T glink_970_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE2,
};
LINK_971_Bx_T glink_971_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE3,
};
LINK_972_Bx_T glink_972_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE4,
};
LINK_973_Bx_T glink_973_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE5,
};
LINK_974_Bx_T glink_974_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE6,
};
LINK_975_Bx_T glink_975_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE7,
};
LINK_976_Bx_T glink_976_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE8,
};
LINK_977_Bx_T glink_977_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xE9,
};
LINK_978_Bx_T glink_978_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEA,
};
LINK_979_Bx_T glink_979_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEB,
};
LINK_980_Bx_T glink_980_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEC,
};
LINK_981_Bx_T glink_981_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xED,
};
LINK_982_Bx_T glink_982_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEE,
};
LINK_983_Bx_T glink_983_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xEF,
};
LINK_984_Bx_T glink_984_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF0,
};
LINK_985_Bx_T glink_985_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF1,
};
LINK_986_Bx_T glink_986_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF2,
};
LINK_987_Bx_T glink_987_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF3,
};
LINK_988_Bx_T glink_988_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF4,
};
LINK_989_Bx_T glink_989_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF5,
};
LINK_990_Bx_T glink_990_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF6,
};
LINK_991_Bx_T glink_991_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF7,
};
LINK_992_Bx_T glink_992_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF8,
};
LINK_993_Bx_T glink_993_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xF9,
};
LINK_994_Bx_T glink_994_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFA,
};
LINK_995_Bx_T glink_995_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFB,
};
LINK_996_Bx_T glink_996_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFC,
};
LINK_997_Bx_T glink_997_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFD,
};
LINK_998_Bx_T glink_998_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFE,
};
LINK_999_Bx_T glink_999_bx =
{
	.slaveAddr = 0x18,
	.regAddr = 0xFF,
};
LINK_1000_Bx_T glink_1000_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x00,
};
LINK_1001_Bx_T glink_1001_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x01,
};
LINK_1002_Bx_T glink_1002_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x02,
};
LINK_1003_Bx_T glink_1003_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x03,
};
LINK_1004_Bx_T glink_1004_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x04,
};
LINK_1005_Bx_T glink_1005_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x05,
};
LINK_1006_Bx_T glink_1006_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x06,
};
LINK_1007_Bx_T glink_1007_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x07,
};
LINK_1008_Bx_T glink_1008_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x08,
};
LINK_1009_Bx_T glink_1009_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x09,
};
LINK_1010_Bx_T glink_1010_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0A,
};
LINK_1011_Bx_T glink_1011_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0B,
};
LINK_1012_Bx_T glink_1012_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0C,
};
LINK_1013_Bx_T glink_1013_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0D,
};
LINK_1014_Bx_T glink_1014_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0E,
};
LINK_1015_Bx_T glink_1015_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x0F,
};
LINK_1016_Bx_T glink_1016_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x10,
};
LINK_1017_Bx_T glink_1017_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x11,
};
LINK_1018_Bx_T glink_1018_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x12,
};
LINK_1019_Bx_T glink_1019_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x13,
};
LINK_1020_Bx_T glink_1020_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x14,
};
LINK_1021_Bx_T glink_1021_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x15,
};
LINK_1022_Bx_T glink_1022_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x16,
};
LINK_1023_Bx_T glink_1023_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x17,
};
LINK_1024_Bx_T glink_1024_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x18,
};
LINK_1025_Bx_T glink_1025_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x19,
};
LINK_1026_Bx_T glink_1026_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1A,
};
LINK_1027_Bx_T glink_1027_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1B,
};
LINK_1028_Bx_T glink_1028_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1C,
};
LINK_1029_Bx_T glink_1029_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1D,
};
LINK_1030_Bx_T glink_1030_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1E,
};
LINK_1031_Bx_T glink_1031_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x1F,
};
LINK_1032_Bx_T glink_1032_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x20,
};
LINK_1033_Bx_T glink_1033_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x21,
};
LINK_1034_Bx_T glink_1034_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x22,
};
LINK_1035_Bx_T glink_1035_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x23,
};
LINK_1036_Bx_T glink_1036_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x24,
};
LINK_1037_Bx_T glink_1037_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x25,
};
LINK_1038_Bx_T glink_1038_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x26,
};
LINK_1039_Bx_T glink_1039_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x27,
};
LINK_1040_Bx_T glink_1040_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x28,
};
LINK_1041_Bx_T glink_1041_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x29,
};
LINK_1042_Bx_T glink_1042_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2A,
};
LINK_1043_Bx_T glink_1043_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2B,
};
LINK_1044_Bx_T glink_1044_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2C,
};
LINK_1045_Bx_T glink_1045_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2D,
};
LINK_1046_Bx_T glink_1046_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2E,
};
LINK_1047_Bx_T glink_1047_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x2F,
};
LINK_1048_Bx_T glink_1048_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x30,
};
LINK_1049_Bx_T glink_1049_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x31,
};
LINK_1050_Bx_T glink_1050_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x32,
};
LINK_1051_Bx_T glink_1051_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x33,
};
LINK_1052_Bx_T glink_1052_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x34,
};
LINK_1053_Bx_T glink_1053_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x35,
};
LINK_1054_Bx_T glink_1054_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x36,
};
LINK_1055_Bx_T glink_1055_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x37,
};
LINK_1056_Bx_T glink_1056_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x38,
};
LINK_1057_Bx_T glink_1057_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x39,
};
LINK_1058_Bx_T glink_1058_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3A,
};
LINK_1059_Bx_T glink_1059_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3B,
};
LINK_1060_Bx_T glink_1060_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3C,
};
LINK_1061_Bx_T glink_1061_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3D,
};
LINK_1062_Bx_T glink_1062_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3E,
};
LINK_1063_Bx_T glink_1063_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x3F,
};
LINK_1064_Bx_T glink_1064_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x40,
};
LINK_1065_Bx_T glink_1065_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x41,
};
LINK_1066_Bx_T glink_1066_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x42,
};
LINK_1067_Bx_T glink_1067_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x43,
};
LINK_1068_Bx_T glink_1068_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x44,
};
LINK_1069_Bx_T glink_1069_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x45,
};
LINK_1070_Bx_T glink_1070_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x46,
};
LINK_1071_Bx_T glink_1071_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x47,
};
LINK_1072_Bx_T glink_1072_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x48,
};
LINK_1073_Bx_T glink_1073_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x49,
};
LINK_1074_Bx_T glink_1074_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4A,
};
LINK_1075_Bx_T glink_1075_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4B,
};
LINK_1076_Bx_T glink_1076_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4C,
};
LINK_1077_Bx_T glink_1077_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4D,
};
LINK_1078_Bx_T glink_1078_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4E,
};
LINK_1079_Bx_T glink_1079_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x4F,
};
LINK_1080_Bx_T glink_1080_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x50,
};
LINK_1081_Bx_T glink_1081_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x51,
};
LINK_1082_Bx_T glink_1082_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x52,
};
LINK_1083_Bx_T glink_1083_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x53,
};
LINK_1084_Bx_T glink_1084_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x54,
};
LINK_1085_Bx_T glink_1085_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x55,
};
LINK_1086_Bx_T glink_1086_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x56,
};
LINK_1087_Bx_T glink_1087_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x57,
};
LINK_1088_Bx_T glink_1088_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x58,
};
LINK_1089_Bx_T glink_1089_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x59,
};
LINK_1090_Bx_T glink_1090_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5A,
};
LINK_1091_Bx_T glink_1091_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5B,
};
LINK_1092_Bx_T glink_1092_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5C,
};
LINK_1093_Bx_T glink_1093_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5D,
};
LINK_1094_Bx_T glink_1094_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5E,
};
LINK_1095_Bx_T glink_1095_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x5F,
};
LINK_1096_Bx_T glink_1096_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x60,
};
LINK_1097_Bx_T glink_1097_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x61,
};
LINK_1098_Bx_T glink_1098_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x62,
};
LINK_1099_Bx_T glink_1099_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x63,
};
LINK_1100_Bx_T glink_1100_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x64,
};
LINK_1101_Bx_T glink_1101_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x65,
};
LINK_1102_Bx_T glink_1102_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x66,
};
LINK_1103_Bx_T glink_1103_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x67,
};
LINK_1104_Bx_T glink_1104_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x68,
};
LINK_1105_Bx_T glink_1105_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x69,
};
LINK_1106_Bx_T glink_1106_bx =
{
	.slaveAddr = 0x19,
	.regAddr = 0x6A,
};
L9A_3CH_INTR_0_Bx_T gl9a_3ch_intr_0_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x00,
};
L9A_3CH_INTR_1_Bx_T gl9a_3ch_intr_1_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x01,
};
L9A_3CH_INTR_EN_0_Bx_T gl9a_3ch_intr_en_0_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x02,
};
L9A_3CH_INTR_EN_1_Bx_T gl9a_3ch_intr_en_1_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x03,
};
LLPLL_0_Bx_T gllpll_0_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x04,
};
LLPLL_1_Bx_T gllpll_1_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x05,
};
LLPLL_2_Bx_T gllpll_2_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x06,
};
LLPLL_3_Bx_T gllpll_3_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x07,
};
LLPLL_4_Bx_T gllpll_4_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x08,
};
LLPLL_5_Bx_T gllpll_5_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x09,
};
LLPLL_6_Bx_T gllpll_6_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0A,
};
LLPLL_7_Bx_T gllpll_7_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0B,
};
LLPLL_8_Bx_T gllpll_8_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0C,
};
LLPLL_9_Bx_T gllpll_9_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0D,
};
LLPLL_10_Bx_T gllpll_10_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0E,
};
LLPLL_11_Bx_T gllpll_11_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x0F,
};
LLPLL_12_Bx_T gllpll_12_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x10,
};
LLPLL_13_Bx_T gllpll_13_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x11,
};
LLPLL_14_Bx_T gllpll_14_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x12,
};
LLPLL_15_Bx_T gllpll_15_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x13,
};
LLPLL_16_Bx_T gllpll_16_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x14,
};
LLPLL_17_Bx_T gllpll_17_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x15,
};
LLPLL_18_Bx_T gllpll_18_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x16,
};
LLPLL_19_Bx_T gllpll_19_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x17,
};
LLPLL_20_Bx_T gllpll_20_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x18,
};
LLPLL_21_Bx_T gllpll_21_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x19,
};
LLPLL_22_Bx_T gllpll_22_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1A,
};
LLPLL_23_Bx_T gllpll_23_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1B,
};
LLPLL_24_Bx_T gllpll_24_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1C,
};
LLPLL_25_Bx_T gllpll_25_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1D,
};
LLPLL_26_Bx_T gllpll_26_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1E,
};
LLPLL_27_Bx_T gllpll_27_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x1F,
};
AFE_3CH_0_Bx_T gafe_3ch_0_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x20,
};
AFE_3CH_1_Bx_T gafe_3ch_1_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x21,
};
AFE_3CH_2_Bx_T gafe_3ch_2_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x22,
};
AFE_3CH_3_Bx_T gafe_3ch_3_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x23,
};
AFE_3CH_4_Bx_T gafe_3ch_4_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x24,
};
AFE_3CH_5_Bx_T gafe_3ch_5_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x25,
};
AFE_3CH_6_Bx_T gafe_3ch_6_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x26,
};
AFE_3CH_7_Bx_T gafe_3ch_7_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x27,
};
ADC_DIG_0_Bx_T gadc_dig_0_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x28,
};
ADC_DIG_1_Bx_T gadc_dig_1_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x29,
};
ADC_DIG_2_Bx_T gadc_dig_2_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2A,
};
ADC_DIG_3_Bx_T gadc_dig_3_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2B,
};
ADC_DIG_4_Bx_T gadc_dig_4_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2C,
};
ADC_DIG_5_Bx_T gadc_dig_5_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2D,
};
ADC_DIG_6_Bx_T gadc_dig_6_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2E,
};
ADC_DIG_7_Bx_T gadc_dig_7_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x2F,
};
ADC_DIG_8_Bx_T gadc_dig_8_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x30,
};
ADC_DIG_9_Bx_T gadc_dig_9_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x31,
};
ADC_DIG_10_Bx_T gadc_dig_10_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x32,
};
ADC_DIG_11_Bx_T gadc_dig_11_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x33,
};
ADC_DIG_12_Bx_T gadc_dig_12_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x34,
};
ADC_DIG_13_Bx_T gadc_dig_13_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x35,
};
ADC_DIG_14_Bx_T gadc_dig_14_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x36,
};
ADC_DIG_15_Bx_T gadc_dig_15_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x37,
};
ADC_DIG_16_Bx_T gadc_dig_16_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x38,
};
ADC_DIG_17_Bx_T gadc_dig_17_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x39,
};
ADC_DIG_18_Bx_T gadc_dig_18_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3A,
};
ADC_DIG_19_Bx_T gadc_dig_19_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3B,
};
ADC_DIG_20_Bx_T gadc_dig_20_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3C,
};
ADC_DIG_21_Bx_T gadc_dig_21_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3D,
};
ADC_DIG_22_Bx_T gadc_dig_22_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3E,
};
ADC_DIG_23_Bx_T gadc_dig_23_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x3F,
};
ADC_DIG_24_Bx_T gadc_dig_24_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x40,
};
ADC_DIG_25_Bx_T gadc_dig_25_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x41,
};
ADC_DIG_26_Bx_T gadc_dig_26_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x42,
};
ADC_DIG_27_Bx_T gadc_dig_27_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x43,
};
ADC_DIG_28_Bx_T gadc_dig_28_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x44,
};
ADC_DIG_29_Bx_T gadc_dig_29_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x45,
};
ADC_DIG_30_Bx_T gadc_dig_30_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x46,
};
ADC_DIG_31_Bx_T gadc_dig_31_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x47,
};
ADC_DIG_32_Bx_T gadc_dig_32_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x48,
};
ADC_DIG_33_Bx_T gadc_dig_33_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x49,
};
ADC_DIG_34_Bx_T gadc_dig_34_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4A,
};
ADC_DIG_35_Bx_T gadc_dig_35_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4B,
};
ADC_DIG_36_Bx_T gadc_dig_36_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4C,
};
ADC_DIG_37_Bx_T gadc_dig_37_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4D,
};
ADC_DIG_38_Bx_T gadc_dig_38_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4E,
};
ADC_DIG_39_Bx_T gadc_dig_39_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x4F,
};
ADC_DIG_40_Bx_T gadc_dig_40_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x50,
};
ADC_DIG_41_Bx_T gadc_dig_41_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x51,
};
ADC_DIG_42_Bx_T gadc_dig_42_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x52,
};
ADC_DIG_43_Bx_T gadc_dig_43_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x53,
};
ADC_DIG_44_Bx_T gadc_dig_44_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x54,
};
ADC_DIG_45_Bx_T gadc_dig_45_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x55,
};
ADC_DIG_46_Bx_T gadc_dig_46_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x56,
};
ADC_DIG_47_Bx_T gadc_dig_47_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x57,
};
ADC_DIG_48_Bx_T gadc_dig_48_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x58,
};
ADC_DIG_49_Bx_T gadc_dig_49_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x59,
};
ADC_DIG_50_Bx_T gadc_dig_50_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5A,
};
ADC_DIG_51_Bx_T gadc_dig_51_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5B,
};
ADC_DIG_52_Bx_T gadc_dig_52_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5C,
};
ADC_DIG_53_Bx_T gadc_dig_53_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5D,
};
ADC_DIG_54_Bx_T gadc_dig_54_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5E,
};
ADC_DIG_55_Bx_T gadc_dig_55_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x5F,
};
ADC_DIG_56_Bx_T gadc_dig_56_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x60,
};
ADC_DIG_57_Bx_T gadc_dig_57_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x61,
};
ADC_DIG_58_Bx_T gadc_dig_58_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x62,
};
ADC_DIG_59_Bx_T gadc_dig_59_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x63,
};
ADC_DIG_60_Bx_T gadc_dig_60_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x64,
};
ADC_DIG_61_Bx_T gadc_dig_61_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x65,
};
ADC_DIG_62_Bx_T gadc_dig_62_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x66,
};
ADC_DIG_63_Bx_T gadc_dig_63_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x67,
};
ADC_DIG_64_Bx_T gadc_dig_64_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x68,
};
ADC_DIG_65_Bx_T gadc_dig_65_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x69,
};
ADC_DIG_66_Bx_T gadc_dig_66_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6A,
};
ADC_DIG_67_Bx_T gadc_dig_67_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6B,
};
ADC_DIG_68_Bx_T gadc_dig_68_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6C,
};
ADC_DIG_69_Bx_T gadc_dig_69_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6D,
};
ADC_DIG_70_Bx_T gadc_dig_70_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6E,
};
ADC_DIG_71_Bx_T gadc_dig_71_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x6F,
};
ADC_DIG_72_Bx_T gadc_dig_72_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x70,
};
ADC_DIG_73_Bx_T gadc_dig_73_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x71,
};
ADC_DIG_74_Bx_T gadc_dig_74_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x72,
};
ADC_DIG_75_Bx_T gadc_dig_75_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x73,
};
ADC_DIG_76_Bx_T gadc_dig_76_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x74,
};
ADC_DIG_77_Bx_T gadc_dig_77_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x75,
};
ADC_DIG_78_Bx_T gadc_dig_78_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x76,
};
ADC_DIG_79_Bx_T gadc_dig_79_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x77,
};
ADC_DIG_80_Bx_T gadc_dig_80_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x78,
};
ADC_DIG_81_Bx_T gadc_dig_81_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x79,
};
ADC_DIG_82_Bx_T gadc_dig_82_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7A,
};
ADC_DIG_83_Bx_T gadc_dig_83_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7B,
};
ADC_DIG_84_Bx_T gadc_dig_84_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7C,
};
ADC_DIG_85_Bx_T gadc_dig_85_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7D,
};
ADC_DIG_86_Bx_T gadc_dig_86_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7E,
};
ADC_DIG_87_Bx_T gadc_dig_87_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x7F,
};
ADC_DIG_88_Bx_T gadc_dig_88_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x80,
};
ADC_DIG_89_Bx_T gadc_dig_89_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x81,
};
ADC_DIG_90_Bx_T gadc_dig_90_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x82,
};
ADC_DIG_91_Bx_T gadc_dig_91_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x83,
};
ADC_DIG_92_Bx_T gadc_dig_92_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x84,
};
ADC_DIG_93_Bx_T gadc_dig_93_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x85,
};
ADC_DIG_94_Bx_T gadc_dig_94_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x86,
};
ADC_DIG_95_Bx_T gadc_dig_95_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x87,
};
ADC_DIG_96_Bx_T gadc_dig_96_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x88,
};
ADC_DIG_97_Bx_T gadc_dig_97_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x89,
};
ADC_DIG_98_Bx_T gadc_dig_98_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8A,
};
ADC_DIG_99_Bx_T gadc_dig_99_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8B,
};
ADC_DIG_100_Bx_T gadc_dig_100_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8C,
};
ADC_DIG_101_Bx_T gadc_dig_101_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8D,
};
ADC_DIG_102_Bx_T gadc_dig_102_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8E,
};
ADC_DIG_103_Bx_T gadc_dig_103_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x8F,
};
ADC_DIG_104_Bx_T gadc_dig_104_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x90,
};
ADC_DIG_105_Bx_T gadc_dig_105_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x91,
};
ADC_DIG_106_Bx_T gadc_dig_106_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x92,
};
ADC_DIG_107_Bx_T gadc_dig_107_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x93,
};
ADC_DIG_108_Bx_T gadc_dig_108_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x94,
};
ADC_DIG_109_Bx_T gadc_dig_109_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x95,
};
ADC_DIG_110_Bx_T gadc_dig_110_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x96,
};
ADC_DIG_111_Bx_T gadc_dig_111_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x97,
};
ADC_DIG_112_Bx_T gadc_dig_112_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x98,
};
ADC_DIG_113_Bx_T gadc_dig_113_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x99,
};
ADC_DIG_114_Bx_T gadc_dig_114_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9A,
};
ADC_DIG_115_Bx_T gadc_dig_115_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9B,
};
ADC_DIG_116_Bx_T gadc_dig_116_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9C,
};
ADC_DIG_117_Bx_T gadc_dig_117_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9D,
};
ADC_DIG_118_Bx_T gadc_dig_118_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9E,
};
ADC_DIG_119_Bx_T gadc_dig_119_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0x9F,
};
ADC_DIG_120_Bx_T gadc_dig_120_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA0,
};
ADC_DIG_121_Bx_T gadc_dig_121_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA1,
};
ADC_DIG_122_Bx_T gadc_dig_122_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA2,
};
ADC_DIG_123_Bx_T gadc_dig_123_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA3,
};
ADC_DIG_124_Bx_T gadc_dig_124_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA4,
};
ADC_DIG_125_Bx_T gadc_dig_125_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA5,
};
ADC_DIG_126_Bx_T gadc_dig_126_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA6,
};
ADC_DIG_127_Bx_T gadc_dig_127_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA7,
};
ADC_DIG_128_Bx_T gadc_dig_128_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA8,
};
ADC_DIG_129_Bx_T gadc_dig_129_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xA9,
};
ADC_DIG_130_Bx_T gadc_dig_130_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAA,
};
ADC_DIG_131_Bx_T gadc_dig_131_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAB,
};
ADC_DIG_132_Bx_T gadc_dig_132_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAC,
};
ADC_DIG_133_Bx_T gadc_dig_133_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAD,
};
ADC_DIG_134_Bx_T gadc_dig_134_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAE,
};
ADC_DIG_135_Bx_T gadc_dig_135_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xAF,
};
ADC_DIG_136_Bx_T gadc_dig_136_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB0,
};
ADC_DIG_137_Bx_T gadc_dig_137_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB1,
};
ADC_DIG_138_Bx_T gadc_dig_138_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB2,
};
ADC_DIG_139_Bx_T gadc_dig_139_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB3,
};
ADC_DIG_140_Bx_T gadc_dig_140_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB4,
};
ADC_DIG_141_Bx_T gadc_dig_141_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB5,
};
ADC_DIG_142_Bx_T gadc_dig_142_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB6,
};
ADC_DIG_143_Bx_T gadc_dig_143_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB7,
};
ADC_DIG_144_Bx_T gadc_dig_144_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB8,
};
ADC_DIG_145_Bx_T gadc_dig_145_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xB9,
};
ADC_DIG_146_Bx_T gadc_dig_146_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBA,
};
ADC_DIG_147_Bx_T gadc_dig_147_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBB,
};
ADC_DIG_148_Bx_T gadc_dig_148_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBC,
};
ADC_DIG_149_Bx_T gadc_dig_149_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBD,
};
ADC_DIG_150_Bx_T gadc_dig_150_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBE,
};
ADC_DIG_151_Bx_T gadc_dig_151_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xBF,
};
ADC_DIG_152_Bx_T gadc_dig_152_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC0,
};
ADC_DIG_153_Bx_T gadc_dig_153_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC1,
};
ADC_DIG_154_Bx_T gadc_dig_154_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC2,
};
ADC_DIG_155_Bx_T gadc_dig_155_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC3,
};
ADC_DIG_156_Bx_T gadc_dig_156_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC4,
};
ADC_DIG_157_Bx_T gadc_dig_157_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC5,
};
ADC_DIG_158_Bx_T gadc_dig_158_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC6,
};
ADC_DIG_159_Bx_T gadc_dig_159_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC7,
};
ADC_DIG_160_Bx_T gadc_dig_160_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC8,
};
ADC_DIG_161_Bx_T gadc_dig_161_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xC9,
};
ADC_DIG_162_Bx_T gadc_dig_162_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCA,
};
ADC_DIG_163_Bx_T gadc_dig_163_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCB,
};
ADC_DIG_164_Bx_T gadc_dig_164_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCC,
};
ADC_DIG_165_Bx_T gadc_dig_165_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCD,
};
ADC_DIG_166_Bx_T gadc_dig_166_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCE,
};
ADC_DIG_167_Bx_T gadc_dig_167_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xCF,
};
ADC_DIG_168_Bx_T gadc_dig_168_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD0,
};
ADC_DIG_169_Bx_T gadc_dig_169_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD1,
};
ADC_DIG_170_Bx_T gadc_dig_170_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD2,
};
ADC_DIG_171_Bx_T gadc_dig_171_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD3,
};
ADC_DIG_172_Bx_T gadc_dig_172_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD4,
};
ADC_DIG_173_Bx_T gadc_dig_173_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD5,
};
ADC_DIG_174_Bx_T gadc_dig_174_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD6,
};
ADC_DIG_175_Bx_T gadc_dig_175_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD7,
};
ADC_DIG_176_Bx_T gadc_dig_176_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD8,
};
ADC_DIG_177_Bx_T gadc_dig_177_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xD9,
};
ADC_DIG_178_Bx_T gadc_dig_178_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDA,
};
ADC_DIG_179_Bx_T gadc_dig_179_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDB,
};
ADC_DIG_180_Bx_T gadc_dig_180_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDC,
};
ADC_DIG_181_Bx_T gadc_dig_181_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDD,
};
ADC_DIG_182_Bx_T gadc_dig_182_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDE,
};
ADC_DIG_183_Bx_T gadc_dig_183_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xDF,
};
ADC_DIG_184_Bx_T gadc_dig_184_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE0,
};
ADC_DIG_185_Bx_T gadc_dig_185_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE1,
};
ADC_DIG_186_Bx_T gadc_dig_186_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE2,
};
ADC_DIG_187_Bx_T gadc_dig_187_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE3,
};
ADC_DIG_188_Bx_T gadc_dig_188_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE4,
};
ADC_DIG_189_Bx_T gadc_dig_189_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE5,
};
ADC_DIG_190_Bx_T gadc_dig_190_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE6,
};
ADC_DIG_191_Bx_T gadc_dig_191_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE7,
};
ADC_DIG_192_Bx_T gadc_dig_192_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE8,
};
ADC_DIG_193_Bx_T gadc_dig_193_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xE9,
};
ADC_DIG_194_Bx_T gadc_dig_194_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEA,
};
ADC_DIG_195_Bx_T gadc_dig_195_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEB,
};
ADC_DIG_196_Bx_T gadc_dig_196_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEC,
};
ADC_DIG_197_Bx_T gadc_dig_197_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xED,
};
ADC_DIG_198_Bx_T gadc_dig_198_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEE,
};
ADC_DIG_199_Bx_T gadc_dig_199_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xEF,
};
ADC_DIG_200_Bx_T gadc_dig_200_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF0,
};
ADC_DIG_201_Bx_T gadc_dig_201_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF1,
};
ADC_DIG_202_Bx_T gadc_dig_202_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF2,
};
ADC_DIG_203_Bx_T gadc_dig_203_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF3,
};
ADC_DIG_204_Bx_T gadc_dig_204_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF4,
};
ADC_DIG_205_Bx_T gadc_dig_205_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF5,
};
ADC_DIG_206_Bx_T gadc_dig_206_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF6,
};
ADC_DIG_207_Bx_T gadc_dig_207_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF7,
};
ADC_DIG_208_Bx_T gadc_dig_208_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF8,
};
ADC_DIG_209_Bx_T gadc_dig_209_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xF9,
};
ADC_DIG_210_Bx_T gadc_dig_210_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFA,
};
ADC_DIG_211_Bx_T gadc_dig_211_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFB,
};
ADC_DIG_212_Bx_T gadc_dig_212_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFC,
};
ADC_DIG_213_Bx_T gadc_dig_213_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFD,
};
ADC_DIG_214_Bx_T gadc_dig_214_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFE,
};
ADC_DIG_215_Bx_T gadc_dig_215_bx =
{
	.slaveAddr = 0x1A,
	.regAddr = 0xFF,
};
ADC_DIG_216_Bx_T gadc_dig_216_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x00,
};
ADC_DIG_217_Bx_T gadc_dig_217_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x01,
};
ADC_DIG_218_Bx_T gadc_dig_218_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x02,
};
ADC_DIG_219_Bx_T gadc_dig_219_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x03,
};
ADC_DIG_220_Bx_T gadc_dig_220_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x04,
};
ADC_DIG_221_Bx_T gadc_dig_221_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x05,
};
ADC_DIG_222_Bx_T gadc_dig_222_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x06,
};
ADC_DIG_223_Bx_T gadc_dig_223_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x07,
};
ADC_DIG_224_Bx_T gadc_dig_224_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x08,
};
ADC_DIG_225_Bx_T gadc_dig_225_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x09,
};
ADC_DIG_226_Bx_T gadc_dig_226_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0A,
};
ADC_DIG_227_Bx_T gadc_dig_227_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0B,
};
ADC_DIG_228_Bx_T gadc_dig_228_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0C,
};
ADC_DIG_229_Bx_T gadc_dig_229_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0D,
};
ADC_DIG_230_Bx_T gadc_dig_230_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0E,
};
ADC_DIG_231_Bx_T gadc_dig_231_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x0F,
};
ADC_DIG_232_Bx_T gadc_dig_232_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x10,
};
ADC_DIG_233_Bx_T gadc_dig_233_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x11,
};
ADC_DIG_234_Bx_T gadc_dig_234_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x12,
};
ADC_DIG_235_Bx_T gadc_dig_235_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x13,
};
ADC_DIG_236_Bx_T gadc_dig_236_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x14,
};
ADC_DIG_237_Bx_T gadc_dig_237_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x15,
};
ADC_DIG_238_Bx_T gadc_dig_238_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x16,
};
ADC_DIG_239_Bx_T gadc_dig_239_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x17,
};
ADC_DIG_240_Bx_T gadc_dig_240_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x18,
};
/*
ADC_DIG_241_Bx_T gadc_dig_241_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x18,
};
ADC_DIG_242_Bx_T gadc_dig_242_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x18,
};
*/
ADC_DIG_243_Bx_T gadc_dig_243_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x19,
};
ADC_DIG_244_Bx_T gadc_dig_244_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1A,
};
ADC_DIG_245_Bx_T gadc_dig_245_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1B,
};
ADC_DIG_246_Bx_T gadc_dig_246_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1C,
};
ADC_DIG_247_Bx_T gadc_dig_247_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1D,
};
ADC_DIG_248_Bx_T gadc_dig_248_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1E,
};
ADC_DIG_249_Bx_T gadc_dig_249_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x1F,
};
ADC_DIG_250_Bx_T gadc_dig_250_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x20,
};
ADC_DIG_251_Bx_T gadc_dig_251_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x21,
};
ADC_DIG_252_Bx_T gadc_dig_252_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x22,
};
ADC_DIG_253_Bx_T gadc_dig_253_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x23,
};
ADC_DIG_254_Bx_T gadc_dig_254_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x24,
};
ADC_DIG_255_Bx_T gadc_dig_255_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x25,
};
ADC_DIG_256_Bx_T gadc_dig_256_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x26,
};
ADC_DIG_257_Bx_T gadc_dig_257_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x27,
};
ADC_DIG_258_Bx_T gadc_dig_258_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x28,
};
ADC_DIG_259_Bx_T gadc_dig_259_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x29,
};
ADC_DIG_260_Bx_T gadc_dig_260_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2A,
};
ADC_DIG_261_Bx_T gadc_dig_261_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2B,
};
ADC_DIG_262_Bx_T gadc_dig_262_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2C,
};
ADC_DIG_263_Bx_T gadc_dig_263_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2D,
};
ADC_DIG_264_Bx_T gadc_dig_264_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2E,
};
ADC_DIG_265_Bx_T gadc_dig_265_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x2F,
};
ADC_DIG_266_Bx_T gadc_dig_266_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x30,
};
ADC_DIG_267_Bx_T gadc_dig_267_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x31,
};
ADC_DIG_268_Bx_T gadc_dig_268_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x32,
};
ADC_DIG_269_Bx_T gadc_dig_269_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x33,
};
ADC_DIG_270_Bx_T gadc_dig_270_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x34,
};
ADC_DIG_271_Bx_T gadc_dig_271_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x35,
};
ADC_DIG_272_Bx_T gadc_dig_272_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x36,
};
ADC_DIG_273_Bx_T gadc_dig_273_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x37,
};
ADC_DIG_274_Bx_T gadc_dig_274_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x38,
};
ADC_DIG_275_Bx_T gadc_dig_275_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x39,
};
ADC_DIG_276_Bx_T gadc_dig_276_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3A,
};
ADC_DIG_277_Bx_T gadc_dig_277_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3B,
};
ADC_DIG_278_Bx_T gadc_dig_278_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3C,
};
ADC_DIG_279_Bx_T gadc_dig_279_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3D,
};
/*
ADC_DIG_280_Bx_T gadc_dig_280_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3D,
};
ADC_DIG_281_Bx_T gadc_dig_281_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3D,
};
*/
ADC_DIG_282_Bx_T gadc_dig_282_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3E,
};
ADC_DIG_283_Bx_T gadc_dig_283_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x3F,
};
ADC_DIG_284_Bx_T gadc_dig_284_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x40,
};
ADC_DIG_285_Bx_T gadc_dig_285_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x41,
};
ADC_DIG_286_Bx_T gadc_dig_286_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x42,
};
ADC_DIG_287_Bx_T gadc_dig_287_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x43,
};
ADC_DIG_288_Bx_T gadc_dig_288_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x44,
};
ADC_DIG_289_Bx_T gadc_dig_289_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x45,
};
ADC_DIG_290_Bx_T gadc_dig_290_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x46,
};
ADC_DIG_291_Bx_T gadc_dig_291_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x47,
};
ADC_DIG_292_Bx_T gadc_dig_292_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x48,
};
ADC_DIG_293_Bx_T gadc_dig_293_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x49,
};
ADC_DIG_294_Bx_T gadc_dig_294_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x4A,
};
ADC_DIG_295_Bx_T gadc_dig_295_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x4B,
};
ADC_DIG_296_Bx_T gadc_dig_296_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x4C,
};
ADC_DIG_297_Bx_T gadc_dig_297_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x4D,
};
ADC_DIG_298_Bx_T gadc_dig_298_bx =
{
	.slaveAddr = 0x1B,
	.regAddr = 0x4E,
};
AUDIO_PLL_0_Bx_T gaudio_pll_0_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x00,
};
AUDIO_PLL_1_Bx_T gaudio_pll_1_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x01,
};
AUDIO_PLL_2_Bx_T gaudio_pll_2_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x02,
};
AUDIO_PLL_3_Bx_T gaudio_pll_3_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x03,
};
AUDIO_PLL_4_Bx_T gaudio_pll_4_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x04,
};
AUDIO_PLL_5_Bx_T gaudio_pll_5_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x05,
};
AUDIO_PLL_6_Bx_T gaudio_pll_6_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x06,
};
AFE_AAD_0_Bx_T gafe_aad_0_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x07,
};
AFE_AAD_1_Bx_T gafe_aad_1_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x08,
};
AFE_AAD_2_Bx_T gafe_aad_2_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x09,
};
AFE_AAD_3_Bx_T gafe_aad_3_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0A,
};
AFE_AAD_4_Bx_T gafe_aad_4_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0B,
};
AFE_AAD_5_Bx_T gafe_aad_5_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0C,
};
AFE_AAD_6_Bx_T gafe_aad_6_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0D,
};
AFE_AAD_7_Bx_T gafe_aad_7_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0E,
};
AFE_AAD_8_Bx_T gafe_aad_8_bx =
{
	.slaveAddr = 0x1C,
	.regAddr = 0x0F,
};
AFE_ACODEC_0_Bx_T gafe_acodec_0_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x00,
};
AFE_ACODEC_1_Bx_T gafe_acodec_1_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x01,
};
AFE_ACODEC_2_Bx_T gafe_acodec_2_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x02,
};
AFE_ACODEC_3_Bx_T gafe_acodec_3_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x03,
};
AFE_ACODEC_4_Bx_T gafe_acodec_4_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x04,
};
AFE_ACODEC_5_Bx_T gafe_acodec_5_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x05,
};
ACODEC_0_Bx_T gacodec_0_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0B,
};
ACODEC_1_Bx_T gacodec_1_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0C,
};
ACODEC_2_Bx_T gacodec_2_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0D,
};
ACODEC_3_Bx_T gacodec_3_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0E,
};
ACODEC_4_Bx_T gacodec_4_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x0F,
};
ACODEC_5_Bx_T gacodec_5_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x10,
};
ACODEC_6_Bx_T gacodec_6_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x11,
};
ACODEC_7_Bx_T gacodec_7_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x12,
};
ACODEC_8_Bx_T gacodec_8_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x13,
};
ACODEC_9_Bx_T gacodec_9_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x14,
};
ACODEC_10_Bx_T gacodec_10_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x15,
};
ACODEC_11_Bx_T gacodec_11_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x16,
};
ACODEC_12_Bx_T gacodec_12_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x17,
};
ACODEC_13_Bx_T gacodec_13_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x18,
};
ACODEC_14_Bx_T gacodec_14_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x19,
};
ACODEC_15_Bx_T gacodec_15_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1A,
};
ACODEC_16_Bx_T gacodec_16_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1B,
};
ACODEC_17_Bx_T gacodec_17_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1C,
};
ACODEC_18_Bx_T gacodec_18_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1D,
};
ACODEC_19_Bx_T gacodec_19_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1E,
};
ACODEC_20_Bx_T gacodec_20_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x1F,
};
ACODEC_21_Bx_T gacodec_21_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x20,
};
ACODEC_31_Bx_T gacodec_31_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2A,
};
ACODEC_32_Bx_T gacodec_32_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2B,
};
ACODEC_33_Bx_T gacodec_33_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2C,
};
ACODEC_34_Bx_T gacodec_34_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2D,
};
ACODEC_35_Bx_T gacodec_35_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2E,
};
ACODEC_36_Bx_T gacodec_36_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x2F,
};
ACODEC_37_Bx_T gacodec_37_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x30,
};
ACODEC_38_Bx_T gacodec_38_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x31,
};
ACODEC_39_Bx_T gacodec_39_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x32,
};
ACODEC_40_Bx_T gacodec_40_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x33,
};
ACODEC_41_Bx_T gacodec_41_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x34,
};
ACODEC_42_Bx_T gacodec_42_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x35,
};
ACODEC_43_Bx_T gacodec_43_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x36,
};
ACODEC_44_Bx_T gacodec_44_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x37,
};
ACODEC_45_Bx_T gacodec_45_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x38,
};
ACODEC_46_Bx_T gacodec_46_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x39,
};
ACODEC_47_Bx_T gacodec_47_bx =
{
	.slaveAddr = 0x1D,
	.regAddr = 0x3A,
};
HDMI_PHY_N_01_Bx_T ghdmi_phy_n_01_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x01,
};
HDMI_PHY_N_02_Bx_T ghdmi_phy_n_02_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x02,
};
HDMI_PHY_N_03_Bx_T ghdmi_phy_n_03_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x03,
};
HDMI_PHY_N_04_Bx_T ghdmi_phy_n_04_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x04,
};
HDMI_PHY_N_05_Bx_T ghdmi_phy_n_05_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x05,
};
HDMI_PHY_N_06_Bx_T ghdmi_phy_n_06_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x06,
};
HDMI_PHY_N_07_Bx_T ghdmi_phy_n_07_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x07,
};
HDMI_PHY_N_08_Bx_T ghdmi_phy_n_08_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x08,
};
HDMI_PHY_N_09_Bx_T ghdmi_phy_n_09_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x09,
};
HDMI_PHY_N_0A_Bx_T ghdmi_phy_n_0a_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x0A,
};
HDMI_PHY_N_10_Bx_T ghdmi_phy_n_10_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x10,
};
HDMI_PHY_N_11_Bx_T ghdmi_phy_n_11_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x11,
};
HDMI_PHY_N_12_Bx_T ghdmi_phy_n_12_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x12,
};
HDMI_PHY_N_13_Bx_T ghdmi_phy_n_13_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x13,
};
HDMI_PHY_N_14_Bx_T ghdmi_phy_n_14_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x14,
};
HDMI_PHY_N_15_Bx_T ghdmi_phy_n_15_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x15,
};
HDMI_PHY_N_16_Bx_T ghdmi_phy_n_16_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x16,
};
HDMI_PHY_N_17_Bx_T ghdmi_phy_n_17_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x17,
};
HDMI_PHY_N_18_Bx_T ghdmi_phy_n_18_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x18,
};
HDMI_PHY_N_19_Bx_T ghdmi_phy_n_19_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x19,
};
HDMI_PHY_N_1A_Bx_T ghdmi_phy_n_1a_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1A,
};
HDMI_PHY_N_1B_Bx_T ghdmi_phy_n_1b_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1B,
};
HDMI_PHY_N_1C_Bx_T ghdmi_phy_n_1c_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1C,
};
HDMI_PHY_N_1D_Bx_T ghdmi_phy_n_1d_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1D,
};
HDMI_PHY_N_1E_Bx_T ghdmi_phy_n_1e_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1E,
};
HDMI_PHY_N_1F_Bx_T ghdmi_phy_n_1f_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x1F,
};
HDMI_PHY_N_20_Bx_T ghdmi_phy_n_20_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x20,
};
HDMI_PHY_N_21_Bx_T ghdmi_phy_n_21_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x21,
};
HDMI_PHY_N_22_Bx_T ghdmi_phy_n_22_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x22,
};
HDMI_PHY_N_23_Bx_T ghdmi_phy_n_23_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x23,
};
HDMI_PHY_N_24_Bx_T ghdmi_phy_n_24_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x24,
};
HDMI_PHY_N_25_Bx_T ghdmi_phy_n_25_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x25,
};
HDMI_PHY_N_26_Bx_T ghdmi_phy_n_26_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x26,
};
HDMI_PHY_N_30_Bx_T ghdmi_phy_n_30_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x30,
};
HDMI_PHY_N_31_Bx_T ghdmi_phy_n_31_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x31,
};
HDMI_PHY_N_32_Bx_T ghdmi_phy_n_32_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x32,
};
HDMI_PHY_N_33_Bx_T ghdmi_phy_n_33_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x33,
};
HDMI_PHY_N_34_Bx_T ghdmi_phy_n_34_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x34,
};
HDMI_PHY_N_35_Bx_T ghdmi_phy_n_35_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x35,
};
HDMI_PHY_N_36_Bx_T ghdmi_phy_n_36_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x36,
};
HDMI_PHY_N_40_Bx_T ghdmi_phy_n_40_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x40,
};
HDMI_PHY_N_41_Bx_T ghdmi_phy_n_41_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x41,
};
HDMI_PHY_N_42_Bx_T ghdmi_phy_n_42_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x42,
};
HDMI_PHY_N_43_Bx_T ghdmi_phy_n_43_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x43,
};
HDMI_PHY_N_44_Bx_T ghdmi_phy_n_44_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x44,
};
HDMI_PHY_N_45_Bx_T ghdmi_phy_n_45_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x45,
};
HDMI_PHY_N_46_Bx_T ghdmi_phy_n_46_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x46,
};
HDMI_PHY_N_47_Bx_T ghdmi_phy_n_47_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x47,
};
HDMI_PHY_N_48_Bx_T ghdmi_phy_n_48_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x48,
};
HDMI_PHY_N_49_Bx_T ghdmi_phy_n_49_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x49,
};
HDMI_PHY_N_4A_Bx_T ghdmi_phy_n_4a_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4A,
};
HDMI_PHY_N_4B_Bx_T ghdmi_phy_n_4b_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4B,
};
HDMI_PHY_N_4C_Bx_T ghdmi_phy_n_4c_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4C,
};
HDMI_PHY_N_4D_Bx_T ghdmi_phy_n_4d_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4D,
};
HDMI_PHY_N_4E_Bx_T ghdmi_phy_n_4e_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4E,
};
HDMI_PHY_N_4F_Bx_T ghdmi_phy_n_4f_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x4F,
};
HDMI_PHY_N_50_Bx_T ghdmi_phy_n_50_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x50,
};
HDMI_PHY_N_51_Bx_T ghdmi_phy_n_51_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x51,
};
HDMI_PHY_N_52_Bx_T ghdmi_phy_n_52_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x52,
};
HDMI_PHY_N_53_Bx_T ghdmi_phy_n_53_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x53,
};
HDMI_PHY_N_54_Bx_T ghdmi_phy_n_54_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x54,
};
HDMI_PHY_N_80_Bx_T ghdmi_phy_n_80_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x80,
};
HDMI_PHY_N_81_Bx_T ghdmi_phy_n_81_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x81,
};
HDMI_PHY_N_82_Bx_T ghdmi_phy_n_82_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x82,
};
HDMI_PHY_N_83_Bx_T ghdmi_phy_n_83_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x83,
};
HDMI_PHY_N_84_Bx_T ghdmi_phy_n_84_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x84,
};
HDMI_PHY_N_85_Bx_T ghdmi_phy_n_85_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x85,
};
HDMI_PHY_N_86_Bx_T ghdmi_phy_n_86_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x86,
};
HDMI_PHY_N_87_Bx_T ghdmi_phy_n_87_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x87,
};
HDMI_PHY_N_88_Bx_T ghdmi_phy_n_88_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x88,
};
HDMI_PHY_N_89_Bx_T ghdmi_phy_n_89_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x89,
};
HDMI_PHY_N_8A_Bx_T ghdmi_phy_n_8a_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8A,
};
HDMI_PHY_N_8B_Bx_T ghdmi_phy_n_8b_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8B,
};
HDMI_PHY_N_8C_Bx_T ghdmi_phy_n_8c_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8C,
};
HDMI_PHY_N_8D_Bx_T ghdmi_phy_n_8d_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8D,
};
HDMI_PHY_N_8E_Bx_T ghdmi_phy_n_8e_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8E,
};
HDMI_PHY_N_8F_Bx_T ghdmi_phy_n_8f_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x8F,
};
HDMI_PHY_N_90_Bx_T ghdmi_phy_n_90_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x90,
};
HDMI_PHY_N_91_Bx_T ghdmi_phy_n_91_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x91,
};
HDMI_PHY_N_92_Bx_T ghdmi_phy_n_92_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x92,
};
HDMI_PHY_N_93_Bx_T ghdmi_phy_n_93_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0x93,
};
HDMI_PHY_N_A0_Bx_T ghdmi_phy_n_a0_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA0,
};
HDMI_PHY_N_A1_Bx_T ghdmi_phy_n_a1_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA1,
};
HDMI_PHY_N_A2_Bx_T ghdmi_phy_n_a2_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA2,
};
HDMI_PHY_N_A3_Bx_T ghdmi_phy_n_a3_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA3,
};
HDMI_PHY_N_A4_Bx_T ghdmi_phy_n_a4_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA4,
};
HDMI_PHY_N_A5_Bx_T ghdmi_phy_n_a5_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA5,
};
HDMI_PHY_N_A6_Bx_T ghdmi_phy_n_a6_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA6,
};
HDMI_PHY_N_A7_Bx_T ghdmi_phy_n_a7_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA7,
};
HDMI_PHY_N_A8_Bx_T ghdmi_phy_n_a8_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA8,
};
HDMI_PHY_N_A9_Bx_T ghdmi_phy_n_a9_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xA9,
};
HDMI_PHY_N_AA_Bx_T ghdmi_phy_n_aa_bx =
{
	.slaveAddr = 0x38,
	.regAddr = 0xAA,
};

