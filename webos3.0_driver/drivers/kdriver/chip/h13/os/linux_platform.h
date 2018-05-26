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

#ifndef __ASM_ARCH_PLATFORM_H13_H
#define __ASM_ARCH_PLATFORM_H13_H

#ifdef H13_MEMC_BASE

/* <linux>/arch/arm/mach-lg1154/include/mach/platform.h */

#else

/*
 * LG1154 Peripheral addresses
 */
#define H13_MEMC_BASE				0xff000000		/* Memory devices */
#define H13_SDHC_BASE				(H13_MEMC_BASE+0x000000)
#define H13_DMAC_SLAVE_BASE			(H13_MEMC_BASE+0x200000)
#define H13_SMC_BASE				(H13_MEMC_BASE+0x400000)
#define H13_SRAMBUF_BASE			(H13_MEMC_BASE+0xf00000)

#define H13_DMAAPB_BASE				0xfe000000		/* DMAAPB */
#define H13_UART0_BASE				(H13_DMAAPB_BASE+0x000000)   /* Uart 0 */
#define H13_UART1_BASE				(H13_DMAAPB_BASE+0x100000)   /* Uart 1 */
#define H13_UART2_BASE				(H13_DMAAPB_BASE+0x200000)   /* Uart 2 */
#define H13_I2C0_BASE				(H13_DMAAPB_BASE+0x300000)   /* I2C 0 */
#define H13_I2C1_BASE				(H13_DMAAPB_BASE+0x400000)   /* I2C 1 */
#define H13_I2C2_BASE				(H13_DMAAPB_BASE+0x500000)   /* I2C 2 */
#define H13_I2C3_BASE				(H13_DMAAPB_BASE+0x600000)   /* I2C 3 */
#define H13_SCI_BASE				(H13_DMAAPB_BASE+0x700000)   /* Smart Card Interface */
#define H13_SPI0_BASE				(H13_DMAAPB_BASE+0x800000)   /* SPI 0 */
#define H13_SPI1_BASE				(H13_DMAAPB_BASE+0x900000)   /* SPI 1 */

#define H13_COREAPB_BASE			0xfd000000		/* CoreAPB */
#define H13_REMAP_BASE				(H13_COREAPB_BASE+0x000000)  /* Remap and pause */
#define H13_TIMER_BASE				(H13_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define H13_TIMER0_1_BASE			(H13_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define H13_WATCHDOG_BASE			(H13_COREAPB_BASE+0x200000)  /* Watchdog */
#define H13_TOPCTRL_BASE			(H13_COREAPB_BASE+0x300000)  /* Top Control */

#define H13_GPIO__BASE				(H13_COREAPB_BASE+0x400000)  /* GPIO Base */
#define H13_GPIO0_BASE				(H13_GPIO__BASE+0x000000)  /* GPIO port 0 */
#define H13_GPIO1_BASE				(H13_GPIO__BASE+0x010000)  /* GPIO port 1 */
#define H13_GPIO2_BASE				(H13_GPIO__BASE+0x020000)  /* GPIO port 2 */
#define H13_GPIO3_BASE				(H13_GPIO__BASE+0x030000)  /* GPIO port 3 */
#define H13_GPIO4_BASE				(H13_GPIO__BASE+0x040000)  /* GPIO port 4 */
#define H13_GPIO5_BASE				(H13_GPIO__BASE+0x050000)  /* GPIO port 5 */
#define H13_GPIO6_BASE				(H13_GPIO__BASE+0x060000)  /* GPIO port 6 */
#define H13_GPIO7_BASE				(H13_GPIO__BASE+0x070000)  /* GPIO port 7 */
#define H13_GPIO8_BASE				(H13_GPIO__BASE+0x080000)  /* GPIO port 8 */
#define H13_GPIO9_BASE				(H13_GPIO__BASE+0x090000)  /* GPIO port 9 */
#define H13_GPIO10_BASE				(H13_GPIO__BASE+0x0a0000)  /* GPIO port 10 */
#define H13_GPIO11_BASE				(H13_GPIO__BASE+0x0b0000)  /* GPIO port 11 */
#define H13_GPIO12_BASE				(H13_GPIO__BASE+0x0c0000)  /* GPIO port 12 */
#define H13_GPIO13_BASE				(H13_GPIO__BASE+0x0d0000)  /* GPIO port 13 */
#define H13_GPIO14_BASE				(H13_GPIO__BASE+0x0e0000)  /* GPIO port 14 */
#define H13_GPIO15_BASE				(H13_GPIO__BASE+0x0f0000)  /* GPIO port 15 */
#define H13_GPIO16_BASE				(H13_GPIO__BASE+0x100000)  /* GPIO port 16 */
#define H13_GPIO17_BASE				(H13_GPIO__BASE+0x110000)  /* GPIO port 17 */

#define H13_USB_PORT1_BASE			0xfc000000
#define H13_USB_SLAVE1_BASE			(H13_USB_PORT1_BASE+0x0)	/* USB slave port 1 */
#define H13_USB1_EHCI0_BASE			(H13_USB_SLAVE1_BASE+0x0000)
#define H13_USB1_OHCI0_BASE			(H13_USB_SLAVE1_BASE+0x4000)
#define H13_USB1_OHCI1_BASE			(H13_USB_SLAVE1_BASE+0x8000)

#define H13_USB_PORT0_BASE			0xfb000000
#define H13_USB_SLAVE0_BASE			(H13_USB_PORT0_BASE+0x0)	/* USB slave port 0 */
#define H13_USB0_EHCI0_BASE			(H13_USB_SLAVE0_BASE+0x0000)
#define H13_USB0_OHCI0_BASE			(H13_USB_SLAVE0_BASE+0x4000)
#define H13_USB0_OHCI1_BASE			(H13_USB_SLAVE0_BASE+0x8000)

#define H13_ETHER_BASE				0xfa000000
#define H13_FEMAC_BASE				(H13_ETHER_BASE+0x0)		/* Ethernet */
#if 0
#define H13_USBBT_BASE				0xf9000000
#define H13_USBBT0_EHCI_BASE		(H13_USBBT_BASE+0x0000)
#define H13_USBBT0_OHCI_BASE		(H13_USBBT_BASE+0x4000)
#define H13_USBBT1_OHCI_BASE		(H13_USBBT_BASE+0x8000)
#else
#define	H13_I2C__BASE				0xf9000000
#define H13_I2C4_BASE				(H13_I2C__BASE+0x000000)   /* I2C 4 */
#define H13_I2C5_BASE				(H13_I2C__BASE+0x100000)   /* I2C 5 */
#define H13_I2C6_BASE				(H13_I2C__BASE+0x200000)   /* I2C 6 */
#define H13_I2C7_BASE				(H13_I2C__BASE+0x300000)   /* I2C 7 */
#define H13_I2C8_BASE				(H13_I2C__BASE+0x400000)   /* I2C 8 */
#define H13_I2C9_BASE				(H13_I2C__BASE+0x500000)   /* I2C 9 */
#endif
/* ------------------------------------------------------------------------
 *  L2 Cache Controller
 * ------------------------------------------------------------------------
 */
#define H13_L2CC_BASE				0xf1000000
#define H13_L2CACHE_BASE			(H13_L2CC_BASE+0x0)		/* L2 Cache Controller */

/* ------------------------------------------------------------------------
 *  Distributed Interrupt Controller (Peripheral Base)
 * ------------------------------------------------------------------------
 */
#define H13_PERIPH_BASE				0xf0000000
#define H13_SCU_BASE				(H13_PERIPH_BASE+0x0000)
#define H13_GIC_CPU_BASE			(H13_PERIPH_BASE+0x0100)
#define H13_LOCALTIMER_BASE			(H13_PERIPH_BASE+0x0600)
#define H13_LOCALWDOG_BASE			(H13_PERIPH_BASE+0x0620)
#define H13_GIC_DIST_BASE			(H13_PERIPH_BASE+0x1000)

/* ------------------------------------------------------------------------
 *  Chip select base addresses
 * ------------------------------------------------------------------------
 */
#define H13_CS_BASE					0xd0000000
#define H13_CS0_BASE				(H13_CS_BASE+0x0000000)
#define H13_CS1_BASE				(H13_CS_BASE+0x4000000)
#define H13_CS2_BASE				(H13_CS_BASE+0x8000000)
#define H13_DVBCI_BASE				(H13_CS2_BASE+0x0)
#define H13_CS3_BASE				(H13_CS_BASE+0xc000000)

/* ------------------------------------------------------------------------
 *  PL301 & AVREG
 * ------------------------------------------------------------------------
 */
#define H13_PL301_BASE				0xc0000000
#define H13_SILKLOAD0_BASE			(H13_PL301_BASE+0x000000)
#define H13_SILKLOAD1_BASE			(H13_PL301_BASE+0x001000)
#define	H13_IOCD_BASE				(H13_PL301_BASE+0x002000)

#define H13_VDEC_BASE				(H13_PL301_BASE+0x004000)
#define H13_VENC_BASE				(H13_PL301_BASE+0x006000)
#define H13_TE_BASE					(H13_PL301_BASE+0x007000)
#define H13_AUD_BASE				(H13_PL301_BASE+0x009000)
#define H13_AAD_BASE				(H13_PL301_BASE+0x009C00)
#define H13_GFX_BASE				(H13_PL301_BASE+0x00B000)
#define H13_HDMI_LINK_BASE			(H13_PL301_BASE+0x00C000)
#define H13_CPUBUS_BASE				(H13_PL301_BASE+0x00D000)
#define H13_LBUS_BASE				(H13_PL301_BASE+0x00E000)
#define H13_GBUS_BASE				(H13_PL301_BASE+0x010000)
#define H13_SLT_BASE				(H13_PL301_BASE+0x012000)
#define H13_DE_BASE					(H13_PL301_BASE+0x013000)
#define H13_VBI_BASE				(H13_PL301_BASE+0x019000)
#define H13_GPU_BASE				(H13_PL301_BASE+0x01A000)
#define H13_CTOP_REG_BASE			(H13_PL301_BASE+0x01B000)

#define H13_BVE_BASE				(H13_PL301_BASE+0x020000)

#define H13_SC_BASE					(H13_PL301_BASE+0x225000)

/* ------------------------------------------------------------------------
 *  Sync registers for dual core booting
 * ------------------------------------------------------------------------
 */
#define H13_SYS_FLAGSSET			(H13_TOPCTRL_BASE+0x02dc)
#define H13_SYS_SYNCSET				(H13_TOPCTRL_BASE+0x02e0)

/* ------------------------------------------------------------------------
 *  FRC
 * ------------------------------------------------------------------------
 */
#define H13_DDR3_FRC_BASE			0x50000000
#define H13_DRAM_FRC_BASE			(H13_DDR3_FRC_BASE+0x0)

/* ------------------------------------------------------------------------
 *  SDRAM
 * ------------------------------------------------------------------------
 */
#define H13_SDRAM0_BASE				0x10000000
#define H13_SDRAM0_BASE_REMAPPED	0x00000000
#define H13_SDRAM_BASE				H13_SDRAM0_BASE_REMAPPED

#endif /* H13_MEMC_BASE */

#endif
