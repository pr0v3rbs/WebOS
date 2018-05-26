/*
 * linux/include/asm-arm/arch-LG1152/platform.h
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

#ifndef __ASM_ARCH_PLATFORM_L9_H
#define __ASM_ARCH_PLATFORM_L9_H

#ifdef L9_MEMC_BASE

/* <linux>/arch/arm/mach-lg1152/include/mach/platform.h */

#else

/*
 * LG1152 Peripheral addresses
 */
#define L9_MEMC_BASE				0xff000000		/* Memory devices */
#define L9_SDHC_BASE				(L9_MEMC_BASE+0x000000)
#define L9_NANDC_BASE				(L9_MEMC_BASE+0x100000)
#define L9_DMAC_SLAVE_BASE			(L9_MEMC_BASE+0x200000)
#define L9_SMC_BASE					(L9_MEMC_BASE+0x400000)
#define L9_SRAMBUF_BASE				(L9_MEMC_BASE+0xf00000)

#define L9_DMAAPB_BASE				0xfe000000		/* DMAAPB */
#define L9_UART0_BASE				(L9_DMAAPB_BASE+0x000000)   /* Uart 0 */
#define L9_UART1_BASE				(L9_DMAAPB_BASE+0x100000)   /* Uart 1 */
#define L9_UART2_BASE				(L9_DMAAPB_BASE+0x200000)   /* Uart 2 */
#define L9_I2C0_BASE				(L9_DMAAPB_BASE+0x300000)   /* I2C 0 */
#define L9_I2C1_BASE				(L9_DMAAPB_BASE+0x400000)   /* I2C 1 */
#define L9_I2C2_BASE				(L9_DMAAPB_BASE+0x500000)   /* I2C 2 */
#define L9_I2C3_BASE				(L9_DMAAPB_BASE+0x600000)   /* I2C 3 */
#define L9_SCI_BASE					(L9_DMAAPB_BASE+0x700000)   /* Smart Card Interface */
#define L9_SPI0_BASE				(L9_DMAAPB_BASE+0x800000)   /* SPI 0 */
#define L9_SPI1_BASE				(L9_DMAAPB_BASE+0x900000)   /* SPI 1 */

#define L9_COREAPB_BASE				0xfd000000		/* CoreAPB */
#define L9_REMAP_BASE				(L9_COREAPB_BASE+0x000000)  /* Remap and pause */
#define L9_TIMER_BASE			(L9_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define L9_TIMER0_1_BASE			(L9_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define L9_WATCHDOG_BASE			(L9_COREAPB_BASE+0x200000)  /* Watchdog */
#define L9_TOPCTRL_BASE				(L9_COREAPB_BASE+0x300000)  /* Top Control */
#define L9_GPIO0_BASE				(L9_COREAPB_BASE+0x400000)  /* GPIO port 0 */
#define L9_GPIO1_BASE				(L9_COREAPB_BASE+0x500000)  /* GPIO port 1 */
#define L9_GPIO2_BASE				(L9_COREAPB_BASE+0x600000)  /* GPIO port 2 */
#define L9_GPIO3_BASE				(L9_COREAPB_BASE+0x700000)  /* GPIO port 3 */
#define L9_GPIO4_BASE				(L9_COREAPB_BASE+0x800000)  /* GPIO port 4 */
#define L9_GPIO5_BASE				(L9_COREAPB_BASE+0x900000)  /* GPIO port 5 */
#define L9_GPIO6_BASE				(L9_COREAPB_BASE+0xa00000)  /* GPIO port 6 */
#define L9_GPIO7_BASE				(L9_COREAPB_BASE+0xb00000)  /* GPIO port 7 */
#define L9_GPIO8_BASE				(L9_COREAPB_BASE+0xc00000)  /* GPIO port 8 */
#define L9_GPIO9_BASE				(L9_COREAPB_BASE+0xd00000)  /* GPIO port 9 */
#define L9_GPIO10_BASE				(L9_COREAPB_BASE+0xe00000)  /* GPIO port 10 */
#define L9_GPIO11_BASE				(L9_COREAPB_BASE+0xf00000)  /* GPIO port 11 */

#define L9_USB_PORT1_BASE           0xfc000000
#define L9_USB_SLAVE1_BASE          (L9_USB_PORT1_BASE+0x0)	/* USB slave port 1 */
#define L9_USB1_EHCI0_BASE			(L9_USB_SLAVE1_BASE+0x0000)
#define L9_USB1_OHCI0_BASE			(L9_USB_SLAVE1_BASE+0x4000)
#define L9_USB1_OHCI1_BASE			(L9_USB_SLAVE1_BASE+0x8000)

#define L9_USB_PORT0_BASE			0xfb000000
#define L9_USB_SLAVE0_BASE			(L9_USB_PORT0_BASE+0x0)	/* USB slave port 0 */
#define L9_USB0_EHCI0_BASE			(L9_USB_SLAVE0_BASE+0x0000)
#define L9_USB0_OHCI0_BASE			(L9_USB_SLAVE0_BASE+0x4000)
#define L9_USB0_OHCI1_BASE			(L9_USB_SLAVE0_BASE+0x8000)

#define L9_ETHER_BASE				0xfa000000
#define L9_FEMAC_BASE				(L9_ETHER_BASE+0x0)		/* Ethernet */
#if 0
#define L9_USBBT_BASE				0xf9000000
#define L9_USBBT0_EHCI_BASE			(L9_USBBT_BASE+0x0000)
#define L9_USBBT0_OHCI_BASE			(L9_USBBT_BASE+0x4000)
#define L9_USBBT1_OHCI_BASE			(L9_USBBT_BASE+0x8000)
#else
#define	L9_I2C__BASE				0xf9000000
#define L9_I2C4_BASE                (L9_I2C__BASE+0x000000)   /* I2C 4 */
#define L9_I2C5_BASE                (L9_I2C__BASE+0x100000)   /* I2C 5 */
#define L9_I2C6_BASE                (L9_I2C__BASE+0x200000)   /* I2C 6 */
#define L9_I2C7_BASE                (L9_I2C__BASE+0x300000)   /* I2C 7 */
#define L9_I2C8_BASE                (L9_I2C__BASE+0x400000)   /* I2C 7 */
#endif
/* ------------------------------------------------------------------------
 *  L2 Cache Controller
 * ------------------------------------------------------------------------
 */
#define L9_L2CC_BASE				0xf1000000
#define L9_L2CACHE_BASE				(L9_L2CC_BASE+0x0)		/* L2 Cache Controller */

/* ------------------------------------------------------------------------
 *  Distributed Interrupt Controller (Peripheral Base)
 * ------------------------------------------------------------------------
 */
#define L9_PERIPH_BASE				0xf0000000
#define L9_SCU_BASE					(L9_PERIPH_BASE+0x0000)
#define L9_GIC_CPU_BASE				(L9_PERIPH_BASE+0x0100)
#define L9_LOCALTIMER_BASE			(L9_PERIPH_BASE+0x0600)
#define L9_LOCALWDOG_BASE			(L9_PERIPH_BASE+0x0620)
#define L9_GIC_DIST_BASE			(L9_PERIPH_BASE+0x1000)

/* ------------------------------------------------------------------------
 *  Chip select base addresses
 * ------------------------------------------------------------------------
 */
#define L9_CS_BASE					0xd0000000
#define L9_CS0_BASE					(L9_CS_BASE+0x0000000)
#define L9_CS1_BASE					(L9_CS_BASE+0x4000000)
#define L9_CS2_BASE					(L9_CS_BASE+0x8000000)
#define L9_DVBCI_BASE				(L9_CS2_BASE+0x0)
#define L9_CS3_BASE					(L9_CS_BASE+0xc000000)

/* ------------------------------------------------------------------------
 *  PL301 & AVREG
 * ------------------------------------------------------------------------
 */
#define L9_PL301_BASE				0xc0000000
#define L9_SILKLOAD0_BASE			(L9_PL301_BASE+0x000000)
#define L9_SILKLOAD1_BASE			(L9_PL301_BASE+0x001000)
#define L9_SILKLOAD2_BASE			(L9_PL301_BASE+0x002000)

#define L9_VDEC_BASE				(L9_PL301_BASE+0x003000)
#define	L9_IOCD_BASE				(L9_PL301_BASE+0x005000)
#define L9_ADEC_BASE				(L9_PL301_BASE+0x008000)
#define L9_VENC_BASE				(L9_PL301_BASE+0x00A000)
#define L9_TE_BASE					(L9_PL301_BASE+0x00B000)
#define L9_GPU_BASE					(L9_PL301_BASE+0x00D000)
#define L9_GFX_BASE					(L9_PL301_BASE+0x01D000)
#define L9_SLT_BASE					(L9_PL301_BASE+0x01E000)
#define L9_CSS_BASE					(L9_PL301_BASE+0x01F000)
#define L9_DE_BASE					(L9_PL301_BASE+0x020000)
#define L9_VBI_BASE					(L9_PL301_BASE+0x025000)
#define L9_SC_BASE					(L9_PL301_BASE+0x105000)


#if 0
#define L9_PL301_CPU_BASE			(L9_PL301_BASE+0x15000)
#define L9_FRC_BASE					(L9_PL301_BASE+0x18000)
#define L9_TCON_BASE				(L9_PL301_BASE+0x1a000)
#define L9_OIF_LED_BASE				(L9_PL301_BASE+0x1c000)
#define L9_3CH_BASE					(L9_PL301_BASE+0x1d000)
#define L9_HDMI_BASE				(L9_PL301_BASE+0x1e000)
#define L9_RESERVED0				(L9_PL301_BASE+0x1f000)
#define L9_PL301_D2_BASE			(L9_PL301_BASE+0x20000)
#define L9_PL301_B2_BASE			(L9_PL301_BASE+0x21000)
#define L9_PL301_D1_BASE			(L9_PL301_BASE+0x22000)
#define L9_PL301_A3_BASE			(L9_PL301_BASE+0x23000)
#define L9_RESERVED1				(L9_PL301_BASE+0x24000)
#define L9_PL301_C1_BASE			(L9_PL301_BASE+0x25000)
#define L9_PL301_C2_BASE			(L9_PL301_BASE+0x26000)
#endif
/* ------------------------------------------------------------------------
 *  Sync registers for dual core booting
 * ------------------------------------------------------------------------
 */
#define L9_SYS_FLAGSSET				(L9_TOPCTRL_BASE+0x02dc)
#define L9_SYS_SYNCSET				(L9_TOPCTRL_BASE+0x02e0)

/* ------------------------------------------------------------------------
 *  FRC
 * ------------------------------------------------------------------------
 */
#define L9_DDR3_FRC_BASE			0x50000000
#define L9_DRAM_FRC_BASE			(L9_DDR3_FRC_BASE+0x0)

/* ------------------------------------------------------------------------
 *  SDRAM
 * ------------------------------------------------------------------------
 */
#define L9_SDRAM0_BASE				0x10000000
#define L9_SDRAM0_BASE_REMAPPED		0x00000000
#define L9_SDRAM_BASE				L9_SDRAM0_BASE_REMAPPED

#endif /* L9_MEMC_BASE */

#endif
