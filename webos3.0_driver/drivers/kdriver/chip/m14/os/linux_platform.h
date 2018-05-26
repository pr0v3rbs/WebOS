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

#ifndef __ASM_ARCH_PLATFORM_M14_H
#define __ASM_ARCH_PLATFORM_M14_H

#ifdef M14_MEMC_BASE

/* <linux>/arch/arm/mach-lg1154/include/mach/platform.h */

#else

/*
 * LG1154 Peripheral addresses
 */
#define M14_MEMC_BASE				0xff000000		/* Memory devices */
#define M14_SDHC_BASE				(M14_MEMC_BASE+0x000000)
#define M14_DMAC_SLAVE_BASE			(M14_MEMC_BASE+0x200000)
#define M14_SMC_BASE				(M14_MEMC_BASE+0x400000)
#define M14_SRAMBUF_BASE			(M14_MEMC_BASE+0xf00000)

#define M14_DMAAPB_BASE				0xfe000000		/* DMAAPB */
#define M14_UART0_BASE				(M14_DMAAPB_BASE+0x000000)   /* Uart 0 */
#define M14_UART1_BASE				(M14_DMAAPB_BASE+0x100000)   /* Uart 1 */
#define M14_UART2_BASE				(M14_DMAAPB_BASE+0x200000)   /* Uart 2 */
#define M14_I2C0_BASE				(M14_DMAAPB_BASE+0x300000)   /* I2C 0 */
#define M14_I2C1_BASE				(M14_DMAAPB_BASE+0x400000)   /* I2C 1 */
#define M14_I2C2_BASE				(M14_DMAAPB_BASE+0x500000)   /* I2C 2 */
#define M14_I2C3_BASE				(M14_DMAAPB_BASE+0x600000)   /* I2C 3 */
#define M14_SCI_BASE				(M14_DMAAPB_BASE+0x700000)   /* Smart Card Interface */
#define M14_SPI0_BASE				(M14_DMAAPB_BASE+0x800000)   /* SPI 0 */
#define M14_SPI1_BASE				(M14_DMAAPB_BASE+0x900000)   /* SPI 1 */

#define M14_COREAPB_BASE			0xfd000000		/* CoreAPB */
#define M14_REMAP_BASE				(M14_COREAPB_BASE+0x000000)  /* Remap and pause */
#define M14_TIMER_BASE				(M14_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define M14_TIMER0_1_BASE			(M14_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define M14_WATCHDOG_BASE			(M14_COREAPB_BASE+0x200000)  /* Watchdog */
#define M14_TOPCTRL_BASE			(M14_COREAPB_BASE+0x300000)  /* Top Control */

#define M14_GPIO__BASE				(M14_COREAPB_BASE+0x400000)  /* GPIO Base */
#define M14_GPIO0_BASE				(M14_GPIO__BASE+0x000000)  /* GPIO port 0 */
#define M14_GPIO1_BASE				(M14_GPIO__BASE+0x010000)  /* GPIO port 1 */
#define M14_GPIO2_BASE				(M14_GPIO__BASE+0x020000)  /* GPIO port 2 */
#define M14_GPIO3_BASE				(M14_GPIO__BASE+0x030000)  /* GPIO port 3 */
#define M14_GPIO4_BASE				(M14_GPIO__BASE+0x040000)  /* GPIO port 4 */
#define M14_GPIO5_BASE				(M14_GPIO__BASE+0x050000)  /* GPIO port 5 */
#define M14_GPIO6_BASE				(M14_GPIO__BASE+0x060000)  /* GPIO port 6 */
#define M14_GPIO7_BASE				(M14_GPIO__BASE+0x070000)  /* GPIO port 7 */
#define M14_GPIO8_BASE				(M14_GPIO__BASE+0x080000)  /* GPIO port 8 */
#define M14_GPIO9_BASE				(M14_GPIO__BASE+0x090000)  /* GPIO port 9 */
#define M14_GPIO10_BASE				(M14_GPIO__BASE+0x0a0000)  /* GPIO port 10 */
#define M14_GPIO11_BASE				(M14_GPIO__BASE+0x0b0000)  /* GPIO port 11 */
#define M14_GPIO12_BASE				(M14_GPIO__BASE+0x0c0000)  /* GPIO port 12 */
#define M14_GPIO13_BASE				(M14_GPIO__BASE+0x0d0000)  /* GPIO port 13 */
#define M14_GPIO14_BASE				(M14_GPIO__BASE+0x0e0000)  /* GPIO port 14 */
#define M14_GPIO15_BASE				(M14_GPIO__BASE+0x0f0000)  /* GPIO port 15 */
#define M14_GPIO16_BASE				(M14_GPIO__BASE+0x100000)  /* GPIO port 16 */
#define M14_GPIO17_BASE				(M14_GPIO__BASE+0x110000)  /* GPIO port 17 */

#define M14_USB_PORT1_BASE			0xfc000000
#define M14_USB_SLAVE1_BASE			(M14_USB_PORT1_BASE+0x0)	/* USB slave port 1 */
#define M14_USB1_EHCI0_BASE			(M14_USB_SLAVE1_BASE+0x0000)
#define M14_USB1_OHCI0_BASE			(M14_USB_SLAVE1_BASE+0x4000)
#define M14_USB1_OHCI1_BASE			(M14_USB_SLAVE1_BASE+0x8000)

#define M14_USB_PORT0_BASE			0xfb000000
#define M14_USB_SLAVE0_BASE			(M14_USB_PORT0_BASE+0x0)	/* USB slave port 0 */
#define M14_USB0_EHCI0_BASE			(M14_USB_SLAVE0_BASE+0x0000)
#define M14_USB0_OHCI0_BASE			(M14_USB_SLAVE0_BASE+0x4000)
#define M14_USB0_OHCI1_BASE			(M14_USB_SLAVE0_BASE+0x8000)

#define M14_ETHER_BASE				0xfa000000
#define M14_FEMAC_BASE				(M14_ETHER_BASE+0x0)		/* Ethernet */
#if 0
#define M14_USBBT_BASE				0xf9000000
#define M14_USBBT0_EHCI_BASE		(M14_USBBT_BASE+0x0000)
#define M14_USBBT0_OHCI_BASE		(M14_USBBT_BASE+0x4000)
#define M14_USBBT1_OHCI_BASE		(M14_USBBT_BASE+0x8000)
#else
#define	M14_I2C__BASE				0xf9000000
#define M14_I2C4_BASE				(M14_I2C__BASE+0x000000)   /* I2C 4 */
#define M14_I2C5_BASE				(M14_I2C__BASE+0x100000)   /* I2C 5 */
#define M14_I2C6_BASE				(M14_I2C__BASE+0x200000)   /* I2C 6 */
#define M14_I2C7_BASE				(M14_I2C__BASE+0x300000)   /* I2C 7 */
#define M14_I2C8_BASE				(M14_I2C__BASE+0x400000)   /* I2C 8 */
#define M14_I2C9_BASE				(M14_I2C__BASE+0x500000)   /* I2C 9 */
#endif
/* ------------------------------------------------------------------------
 *  L2 Cache Controller
 * ------------------------------------------------------------------------
 */
#define M14_L2CC_BASE				0xf1000000
#define M14_L2CACHE_BASE			(M14_L2CC_BASE+0x0)		/* L2 Cache Controller */

/* ------------------------------------------------------------------------
 *  Distributed Interrupt Controller (Peripheral Base)
 * ------------------------------------------------------------------------
 */
#define M14_PERIPH_BASE				0xf0000000
#define M14_SCU_BASE				(M14_PERIPH_BASE+0x0000)
#define M14_GIC_CPU_BASE			(M14_PERIPH_BASE+0x0100)
#define M14_LOCALTIMER_BASE			(M14_PERIPH_BASE+0x0600)
#define M14_LOCALWDOG_BASE			(M14_PERIPH_BASE+0x0620)
#define M14_GIC_DIST_BASE			(M14_PERIPH_BASE+0x1000)

/* ------------------------------------------------------------------------
 *  Chip select base addresses
 * ------------------------------------------------------------------------
 */
#define M14_CS_BASE					0xd0000000
#define M14_CS0_BASE				(M14_CS_BASE+0x0000000)
#define M14_CS1_BASE				(M14_CS_BASE+0x4000000)
#define M14_CS2_BASE				(M14_CS_BASE+0x8000000)
#define M14_DVBCI_BASE				(M14_CS2_BASE+0x0)
#define M14_CS3_BASE				(M14_CS_BASE+0xc000000)

/* ------------------------------------------------------------------------
 *  PL301 & AVREG
 * ------------------------------------------------------------------------
 */
#define M14_PL301_BASE				0xc0000000
#define M14_SILKLOAD0_BASE			(M14_PL301_BASE+0x000000)
#define M14_SILKLOAD1_BASE			(M14_PL301_BASE+0x001000)

#define	M14_IOCD_BASE				(M14_PL301_BASE+0x002000)
#define M14_VDEC_BASE				(M14_PL301_BASE+0x004000)
#define M14_VENC_BASE				(M14_PL301_BASE+0x006000)
#define M14_TE_BASE					(M14_PL301_BASE+0x007000)
#define M14_AUD_BASE				(M14_PL301_BASE+0x009000)
#define M14_AAD_BASE				(M14_PL301_BASE+0x009C00)
#define M14_GFX_BASE				(M14_PL301_BASE+0x00B000)
#define M14_HDMI_LINK_BASE			(M14_PL301_BASE+0x00C000)
#define M14_CPUBUS_BASE				(M14_PL301_BASE+0x00D000)
#define M14_LBUS_BASE				(M14_PL301_BASE+0x00E000)
#define M14_GBUS_BASE				(M14_PL301_BASE+0x010000)
#define M14_SLT_BASE				(M14_PL301_BASE+0x012000)
#define M14_DE_BASE					(M14_PL301_BASE+0x013000)
#define M14_VBI_BASE				(M14_PL301_BASE+0x019000)
#define M14_GPU_BASE				(M14_PL301_BASE+0x01A000)
#define M14_CTOP_REG_BASE			(M14_PL301_BASE+0x01B000)
#define M14_BVE_BASE				(M14_PL301_BASE+0x020000)
#define M14_SC_BASE					(M14_PL301_BASE+0x225000)

#define M14_IP_REG_BASE				0xc0000000
/* M14 A0 REG BASE ADDRESS */
#define	M14_A0_IOCD_BASE			(M14_IP_REG_BASE+0x002000)
#define M14_A0_VDEC_BASE			(M14_IP_REG_BASE+0x004000)
#define M14_A0_VENC_BASE			(M14_IP_REG_BASE+0x006000)
#define M14_A0_TE_BASE				(M14_IP_REG_BASE+0x007000)
#define M14_A0_AUD_BASE				(M14_IP_REG_BASE+0x009000)
#define M14_A0_AAD_BASE				(M14_IP_REG_BASE+0x009C00)
#define M14_A0_GFX_BASE				(M14_IP_REG_BASE+0x00B000)
#define M14_A0_HDMI_LINK_BASE		(M14_IP_REG_BASE+0x00C000)
#define M14_A0_CPUBUS_BASE			(M14_IP_REG_BASE+0x00D000)
#define M14_A0_LBUS_BASE			(M14_IP_REG_BASE+0x00E000)
#define M14_A0_GBUS_BASE			(M14_IP_REG_BASE+0x010000)
#define M14_A0_SLT_BASE				(M14_IP_REG_BASE+0x012000)
#define M14_A0_DE_BASE				(M14_IP_REG_BASE+0x013000)
#define M14_A0_VBI_BASE				(M14_IP_REG_BASE+0x019000)
#define M14_A0_GPU_BASE				(M14_IP_REG_BASE+0x01A000)
#define M14_A0_CTOP_REG_BASE		(M14_IP_REG_BASE+0x01B000)
#define M14_A0_BVE_BASE				(M14_IP_REG_BASE+0x020000)
#define M14_A0_SC_BASE				(M14_IP_REG_BASE+0x225000)

/* M14 B0 REG BASE ADDRESS */
#define	M14_B0_IOCD_BASE			(M14_IP_REG_BASE+0x002000)
#define M14_B0_VDEC_BASE			(M14_IP_REG_BASE+0x004000)
#define M14_B0_HEVC_BASE			(M14_IP_REG_BASE+0x006000)
#define M14_B0_VENC_BASE			(M14_IP_REG_BASE+0x007000)	// modified
#define M14_B0_TE_BASE				(M14_IP_REG_BASE+0x008000)	// modified
#define M14_B0_GFX_BASE				(M14_IP_REG_BASE+0x00B000)
#define M14_B0_AUD_BASE				(M14_IP_REG_BASE+0x00C000)	// modified
#define M14_B0_AAD_BASE				(M14_IP_REG_BASE+0x00CC00)	// modified
#define M14_B0_HDMI_LINK_BASE		(M14_IP_REG_BASE+0x00E000)	// modified
#define M14_B0_SLT_BASE				(M14_IP_REG_BASE+0x01F000)	// modified
#define M14_B0_LBUS_BASE			(M14_IP_REG_BASE+0x010000)	// modified
#define M14_B0_GBUS_BASE			(M14_IP_REG_BASE+0x014000)	// modified
#define M14_B0_GPU_CTRL_BASE		(M14_IP_REG_BASE+0x018000)	// modified
#define M14_B0_CTOP_REG_BASE		(M14_IP_REG_BASE+0x019000)	// modified
#define M14_B0_DE_BASE				(M14_IP_REG_BASE+0x020000)	// modified
#define M14_B0_VBI_BASE				(M14_IP_REG_BASE+0x04F000)	// modified
#define M14_B0_GPU_BASE				(M14_IP_REG_BASE+0x060000)	// modified
#define M14_B0_SC_BASE				(M14_IP_REG_BASE+0x225000)


/* ------------------------------------------------------------------------
 *  Sync registers for dual core booting
 * ------------------------------------------------------------------------
 */
#define M14_SYS_FLAGSSET			(M14_TOPCTRL_BASE+0x02dc)
#define M14_SYS_SYNCSET				(M14_TOPCTRL_BASE+0x02e0)

/* ------------------------------------------------------------------------
 *  FRC
 * ------------------------------------------------------------------------
 */
#define M14_DDR3_FRC_BASE			0x50000000
#define M14_DRAM_FRC_BASE			(M14_DDR3_FRC_BASE+0x0)

/* ------------------------------------------------------------------------
 *  SDRAM
 * ------------------------------------------------------------------------
 */
#define M14_SDRAM0_BASE				0x10000000
#define M14_SDRAM0_BASE_REMAPPED	0x00000000
#define M14_SDRAM_BASE				M14_SDRAM0_BASE_REMAPPED

#endif /* M14_MEMC_BASE */

#endif
