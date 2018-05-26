/*
 * linux/include/asm-arm/arch-LG1154/platform.h
 *
 * Copyright (c) ARM Limited 2003.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_PLATFORM_H15_H
#define __ASM_ARCH_PLATFORM_H15_H


/*
 * Physical base addresses
 */
#define H15_DMAAPB_BASE			(0xfe000000)
#define H15_UART0_BASE			(H15_DMAAPB_BASE+0x000000)
#define H15_UART1_BASE			(H15_DMAAPB_BASE+0x100000)
#define H15_UART2_BASE			(H15_DMAAPB_BASE+0x200000)
#define H15_I2C0_BASE			(H15_DMAAPB_BASE+0x300000)
#define H15_I2C1_BASE			(H15_DMAAPB_BASE+0x400000)
#define H15_I2C2_BASE			(H15_DMAAPB_BASE+0x500000)
#define H15_I2C3_BASE			(H15_DMAAPB_BASE+0x600000)
#define H15_SCI_BASE			(H15_DMAAPB_BASE+0x700000)
#define H15_SPI0_BASE			(H15_DMAAPB_BASE+0x800000)
#define H15_SPI1_BASE			(H15_DMAAPB_BASE+0x900000)

#define H15_COREAPB_BASE		(0xfd000000)
#define H15_REMAP_BASE			(H15_COREAPB_BASE+0x000000)
#define H15_TIMER_BASE			(H15_COREAPB_BASE+0x100000)
#define H15_WDOG0_BASE			(H15_COREAPB_BASE+0x200000)
#define H15_TOPCTRL_BASE		(H15_COREAPB_BASE+0x300000)


#define H15_GPIO0_BASE			(H15_COREAPB_BASE+0x400000)
#define H15_GPIO1_BASE			(H15_COREAPB_BASE+0x410000)
#define H15_GPIO2_BASE			(H15_COREAPB_BASE+0x420000)
#define H15_GPIO3_BASE			(H15_COREAPB_BASE+0x430000)
#define H15_GPIO4_BASE			(H15_COREAPB_BASE+0x440000)
#define H15_GPIO5_BASE			(H15_COREAPB_BASE+0x450000)
#define H15_GPIO6_BASE			(H15_COREAPB_BASE+0x460000)
#define H15_GPIO7_BASE			(H15_COREAPB_BASE+0x470000)
#define H15_GPIO8_BASE			(H15_COREAPB_BASE+0x480000)
#define H15_GPIO9_BASE			(H15_COREAPB_BASE+0x490000)
#define H15_GPIO10_BASE			(H15_COREAPB_BASE+0x4a0000)
#define H15_GPIO11_BASE			(H15_COREAPB_BASE+0x4b0000)
#define H15_GPIO12_BASE			(H15_COREAPB_BASE+0x4c0000)
#define H15_GPIO13_BASE			(H15_COREAPB_BASE+0x4d0000)
#define H15_GPIO14_BASE			(H15_COREAPB_BASE+0x4e0000)
#define H15_GPIO15_BASE			(H15_COREAPB_BASE+0x4f0000)
#define H15_GPIO16_BASE			(H15_COREAPB_BASE+0x500000)
#define H15_GPIO17_BASE			(H15_COREAPB_BASE+0x510000)


#define	H15_I2C__BASE			(0xf9000000)
#define H15_I2C4_BASE			(H15_I2C__BASE+0x000000)
#define H15_I2C5_BASE			(H15_I2C__BASE+0x100000)
#define H15_I2C6_BASE			(H15_I2C__BASE+0x200000)
#define H15_I2C7_BASE			(H15_I2C__BASE+0x300000)
#define H15_I2C8_BASE			(H15_I2C__BASE+0x400000)
#define H15_I2C9_BASE			(H15_I2C__BASE+0x500000)


#define H15_CS_BASE				(0xD0000000)
#define H15_CS0_BASE			(H15_CS_BASE+0x0000000)
#define H15_CS1_BASE			(H15_CS_BASE+0x4000000)
#define H15_CS2_BASE			(H15_CS_BASE+0x8000000)
#define H15_CS3_BASE			(H15_CS_BASE+0xc000000)


#define H15_PSEL_BASE			(0xC8000000)
#define H15_CVI_BASE			(H15_PSEL_BASE+0x000000)
#define H15_IMX_BASE			(H15_PSEL_BASE+0x001000)
#define H15_ND0_BASE			(H15_PSEL_BASE+0x004000)
#define H15_ND1_BASE			(H15_PSEL_BASE+0x005000)
#define H15_SUB_BASE			(H15_PSEL_BASE+0x006000)
#define H15_GSC_BASE			(H15_PSEL_BASE+0x007000)
#define H15_SRS_BASE			(H15_PSEL_BASE+0x008000)
#define H15_VSD_BASE			(H15_PSEL_BASE+0x00B000)
#define H15_DPE_BASE			(H15_PSEL_BASE+0x010000)
#define H15_CCO_BASE			(H15_PSEL_BASE+0x024000)
#define H15_MCU_BASE			(H15_PSEL_BASE+0x026000)
#define H15_HDMI_RX_BASE		(H15_PSEL_BASE+0x200000)
#define H15_HDMI_RX_LINK_BASE	(H15_PSEL_BASE+0x210000)
#define H15_VBI_BASE			(H15_PSEL_BASE+0x212000)
#define H15_NOC_E_BASE			(H15_PSEL_BASE+0x213000)
#define H15_HDMI_TX_BASE		(H15_PSEL_BASE+0x300000)
#define H15_M1_BASE				(H15_PSEL_BASE+0x404000)
#define H15_SMMU_GFX0_BASE		(H15_PSEL_BASE+0x420000)
#define H15_SMMU_GFX1_BASE		(H15_PSEL_BASE+0x430000)
#define H15_GFX0_BASE			(H15_PSEL_BASE+0x440000)
#define H15_VDEC0_BASE			(H15_PSEL_BASE+0x450000)
#define H15_NOC_G_BASE			(H15_PSEL_BASE+0x453000)
#define H15_SMMU_VDEC0_0		(H15_PSEL_BASE+0x460000)
#define H15_SMMU_VDEC0_1		(H15_PSEL_BASE+0x470000)
#define H15_M2_BASE				(H15_PSEL_BASE+0x504000)
#define H15_VDEC1_BASE			(H15_PSEL_BASE+0x530000)
#define H15_VDEC2_BASE			(H15_PSEL_BASE+0x560000)
#define H15_SMMU_CCO_0_BASE		(H15_PSEL_BASE+0x570000)
#define H15_SMMU_CCO_1_BASE		(H15_PSEL_BASE+0x580000)

#define H15_CTOP_CVI_BASE		(H15_PSEL_BASE+0x600000)
#define H15_CTOP_IMX_BASE		(H15_PSEL_BASE+0x600400)
#define H15_CTOP_ND0_BASE		(H15_PSEL_BASE+0x601000)
#define H15_CTOP_ND1_BASE		(H15_PSEL_BASE+0x601400)
#define H15_CTOP_SUB_BASE		(H15_PSEL_BASE+0x601800)
#define H15_CTOP_GSC_BASE		(H15_PSEL_BASE+0x601C00)
#define H15_CTOP_SRS_BASE		(H15_PSEL_BASE+0x602000)
#define H15_CTOP_VSD_BASE		(H15_PSEL_BASE+0x602C00)
#define H15_CRC_VSD_BASE		(H15_PSEL_BASE+0x602E00)
#define H15_CTOP_DPE_BASE		(H15_PSEL_BASE+0x604000)
#define H15_CRC_DPE_BASE		(H15_PSEL_BASE+0x604200)
#define H15_CTOP_CCO_BASE		(H15_PSEL_BASE+0x609000)
#define H15_CTOP_MCU_BASE		(H15_PSEL_BASE+0x609800)

#define H15_CTOP_M1_BASE		(H15_PSEL_BASE+0x705000)
#define H15_CTOP_GFX_BASE		(H15_PSEL_BASE+0x708000)
#define H15_QOS_GFX_BASE		(H15_PSEL_BASE+0x708200)
#define H15_CTOP_VDEC0_BASE		(H15_PSEL_BASE+0x714000)
#define H15_QOS_CCO_BASE		(H15_PSEL_BASE+0x70D000)
#define H15_CTOP_VDEC1_BASE		(H15_PSEL_BASE+0x744000)
#define H15_CTOP_M2_BASE		(H15_PSEL_BASE+0x745000)
#define H15_CTOP_VDEC2_BASE		(H15_PSEL_BASE+0x750000)

#define H15_M0_BASE				(H15_PSEL_BASE+0x804000)
#define H15_NOCL_BASE			(H15_PSEL_BASE+0x808000)
#define H15_SMMU_VENC0_BASE		(H15_PSEL_BASE+0x810000)
#define H15_SMMU_AUD_BASE		(H15_PSEL_BASE+0x820000)
#define H15_TE_BASE				(H15_PSEL_BASE+0x830000)
#define H15_ICOD_BASE			(H15_PSEL_BASE+0x834000)
#define H15_AUD_BASE			(H15_PSEL_BASE+0x836000)
#define H15_AAD_BASE			(H15_PSEL_BASE+0x836C00)
#define H15_VENC_BASE			(H15_PSEL_BASE+0x838000)
#define H15_VIP_BASE			(H15_PSEL_BASE+0x840000)

#define H15_CTOP_M0_BASE		(H15_PSEL_BASE+0x985000)
#define H15_CTOP_TI_BASE		(H15_PSEL_BASE+0x98C000)
#define H15_CTOP_AUD_BASE		(H15_PSEL_BASE+0x98D800)
#define H15_CTOP_VENC_BASE		(H15_PSEL_BASE+0x98E000)
#define H15_CTOP_VIP_BASE		(H15_PSEL_BASE+0x98E400)

#define H15_GPU_BASE			(H15_PSEL_BASE+0xA00000)
#define H15_SMMU_GPU_BASE		(H15_PSEL_BASE+0xA10000)
#define H15_CTOP_GPU_BASE		(H15_PSEL_BASE+0xAC0000)
#define H15_QOS_GPU_BASE		(H15_PSEL_BASE+0xAC0200)
#define H15_CTOP_CPU_BASE		(H15_PSEL_BASE+0xAFF000)



#endif //__ASM_ARCH_PLATFORM_H15_H
