/*
 * linux/include/asm-arm/arch-DTVSOC/platform.h
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

#ifndef __ASM_ARCH_PLATFORM_L8_H
#define __ASM_ARCH_PLATFORM_L8_H

#ifdef L8_MEMC_BASE

/* <linux>/arch/arm/mach-dtvsoc/include/mach/platform.h */

#else
/*
 * DTVSOC Peripheral addresses
 */
#define L8_MEMC_BASE		0xff000000		/* Memory devices */
#define L8_VIRT_MEMC_BASE		0xff000000
#define L8_SDHC_BASE		(L8_VIRT_MEMC_BASE+0x000000)
#define L8_NANDC_BASE		(L8_VIRT_MEMC_BASE+0x100000)
#define L8_DMAC_BASE		(L8_VIRT_MEMC_BASE+0x200000)
#define L8_EMAC_BASE		(L8_VIRT_MEMC_BASE+0x400000)
#define L8_SRAMBUF_BASE		(L8_VIRT_MEMC_BASE+0xf00000)

#define L8_DMAAPB_BASE		0xfe000000	/* DMAAPB */
#define L8_VIRT_DMAAPB_BASE		0xfe000000
#define L8_UART0_BASE		(L8_VIRT_DMAAPB_BASE+0x000000)   /* Uart 0 */
#define L8_UART1_BASE		(L8_VIRT_DMAAPB_BASE+0x100000)   /* Uart 1 */
#define L8_UART2_BASE		(L8_VIRT_DMAAPB_BASE+0x200000)   /* Uart 2 */
#define L8_I2C0_BASE		(L8_VIRT_DMAAPB_BASE+0x300000)   /* I2C 0 */
#define L8_I2C1_BASE		(L8_VIRT_DMAAPB_BASE+0x400000)   /* I2C 1 */
#define L8_I2C2_BASE		(L8_VIRT_DMAAPB_BASE+0x500000)   /* I2C 2 */
#define L8_I2C3_BASE		(L8_VIRT_DMAAPB_BASE+0x600000)   /* I2C 3 */
#define L8_SCI_BASE			(L8_VIRT_DMAAPB_BASE+0x700000)   /* Smart Card Interface */
#define L8_SPI0_BASE		(L8_VIRT_DMAAPB_BASE+0x800000)   /* SPI 0 */
#define L8_SPI1_BASE		(L8_VIRT_DMAAPB_BASE+0x900000)   /* SPI 1 */
#define L8_I2C4_BASE		(L8_VIRT_DMAAPB_BASE+0xA00000)   /* I2C 4 */
#define L8_I2C5_BASE		(L8_VIRT_DMAAPB_BASE+0xB00000)   /* I2C 5 */

#define L8_COREAPB_BASE		0xfd000000	/* CoreAPB */
#define L8_VIRT_COREAPB_BASE	0xfd000000
#define L8_REMAP_BASE		(L8_VIRT_COREAPB_BASE+0x000000)  /* Remap and pause */
#define L8_TIMER0_1_BASE		(L8_VIRT_COREAPB_BASE+0x100000)  /* Timer 0 and 1 */
#define L8_WATCHDOG_BASE		(L8_VIRT_COREAPB_BASE+0x200000)  /* Watchdog */
#define L8_TOP_CTRL			(L8_VIRT_COREAPB_BASE+0x300000)  /* Top Control */
#define L8_GPIO0_BASE		(L8_VIRT_COREAPB_BASE+0x400000)  /* GPIO port 0 */
#define L8_GPIO1_BASE		(L8_VIRT_COREAPB_BASE+0x500000)  /* GPIO port 1 */
#define L8_GPIO2_BASE		(L8_VIRT_COREAPB_BASE+0x600000)  /* GPIO port 2 */
#define L8_GPIO3_BASE		(L8_VIRT_COREAPB_BASE+0x700000)  /* GPIO port 3 */
#define L8_GPIO4_BASE		(L8_VIRT_COREAPB_BASE+0x800000)  /* GPIO port 4 */
#define L8_GPIO5_BASE		(L8_VIRT_COREAPB_BASE+0x900000)  /* GPIO port 5 */
#define L8_GPIO6_BASE		(L8_VIRT_COREAPB_BASE+0xa00000)  /* GPIO port 6 */
#define L8_GPIO7_BASE		(L8_VIRT_COREAPB_BASE+0xb00000)  /* GPIO port 7 */
#define L8_GPIO8_BASE		(L8_VIRT_COREAPB_BASE+0xc00000)  /* GPIO port 8 */
#define L8_GPIO9_BASE		(L8_VIRT_COREAPB_BASE+0xd00000)  /* GPIO port 9 */
#define L8_GPIO10_BASE		(L8_VIRT_COREAPB_BASE+0xe00000)  /* GPIO port 10 */
#define L8_GPIO11_BASE		(L8_VIRT_COREAPB_BASE+0xf00000)  /* GPIO port 11 */

#define L8_USB_PORT1_BASE           0xfc000000
#define L8_VIRT_USB_PORT1_BASE      0xfc000000
#define L8_USB_SLAVE1_BASE          (L8_VIRT_USB_PORT1_BASE+0x0)	/* USB slave port 1 */
#define L8_USB1_EHCI_BASE           L8_USB_SLAVE1_BASE
#define L8_USB1_OHCI_BASE           (L8_USB_SLAVE1_BASE+0x4000)

#define L8_USB_PORT0_BASE		0xfb000000
#define L8_VIRT_USB_PORT0_BASE	0xfb000000
#define L8_USB_SLAVE0_BASE		(L8_VIRT_USB_PORT0_BASE+0x0)	/* USB slave port 0 */
#define L8_USB0_EHCI_BASE		L8_USB_SLAVE0_BASE
#define L8_USB0_OHCI_BASE		(L8_USB_SLAVE0_BASE+0x4000)

#define L8_ETHER_BASE		0xfa000000
#define L8_VIRT_ETHER_BASE		0xfa000000
#define L8_FEMAC_BASE		(L8_VIRT_ETHER_BASE+0x0)		/* Ethernet */

#define L8_L2CC_BASE		0xf1000000
#define L8_VIRT_L2CC_BASE		0xf1000000
#define L8_L2CACHE_BASE		(L8_VIRT_L2CC_BASE+0x0)		/* L2 Cache Controller */

#define L8_USB_PORT2_BASE		0xf9000000
#define L8_VIRT_USB_PORT2_BASE	0xf9000000
#define L8_USB_SLAVE2_BASE		(L8_VIRT_USB_PORT2_BASE+0x0)	/* USB slave port 2,3 */
#define L8_USB2_EHCI_BASE		L8_USB_SLAVE2_BASE
#define L8_USB2_OHCI_BASE		(L8_USB_SLAVE2_BASE+0x4000)
#define L8_USB3_OHCI_BASE		(L8_USB_SLAVE2_BASE+0x8000)

/* ------------------------------------------------------------------------
 *  Distributed Interrupt Controller (Peripheral Base)
 * ------------------------------------------------------------------------
 */
#define L8_PERIPH_BASE		0xf0000000
#define L8_VIRT_PERIPH_BASE		0xf0000000
#define L8_SCU_BASE			(L8_VIRT_PERIPH_BASE+0x0000)
#define L8_GIC_CPU_BASE		(L8_VIRT_PERIPH_BASE+0x0100)
#define L8_LOCALTIMER_BASE		(L8_VIRT_PERIPH_BASE+0x0600)
#define L8_LOCALWDOG_BASE		(L8_VIRT_PERIPH_BASE+0x0620)
#define L8_GIC_DIST_BASE		(L8_VIRT_PERIPH_BASE+0x1000)

/* ------------------------------------------------------------------------
 *  PL301 & AVREG
 * ------------------------------------------------------------------------
 */
#define L8_PL301_BASE		0xc0000000
#define L8_VIRT_PL301_BASE		0xf6000000
#define L8_SILKLOAD0_BASE		(L8_VIRT_PL301_BASE+0x00000)
#define L8_SILKLOAD1_BASE		(L8_VIRT_PL301_BASE+0x01000)
#define L8_SILKLOAD2_BASE		(L8_VIRT_PL301_BASE+0x02000)
#define L8_VDEC_BASE		(L8_VIRT_PL301_BASE+0x03000)
#define L8_SC_BASE			(L8_VIRT_PL301_BASE+0x04000)
#define L8_SP_BASE			(L8_VIRT_PL301_BASE+0x05000)
#define L8_ADEC_BASE		(L8_VIRT_PL301_BASE+0x06000)
#define L8_2CH_BASE			(L8_VIRT_PL301_BASE+0x07000)
#define L8_VENC_BASE		(L8_VIRT_PL301_BASE+0x08000)
#define L8_TE_BASE			(L8_VIRT_PL301_BASE+0x09000)
#define L8_GPU_BASE			(L8_VIRT_PL301_BASE+0x0b000)

#define L8_PL301_CPU_BASE		(L8_VIRT_PL301_BASE+0x15000)
#define L8_DE_BASE			(L8_VIRT_PL301_BASE+0x16000)
#define L8_FRC_BASE			(L8_VIRT_PL301_BASE+0x18000)
#define L8_TCON_BASE		(L8_VIRT_PL301_BASE+0x1a000)
#define L8_OIF_LED_BASE		(L8_VIRT_PL301_BASE+0x1c000)
#define L8_3CH_BASE			(L8_VIRT_PL301_BASE+0x1d000)
#define L8_HDMI_BASE		(L8_VIRT_PL301_BASE+0x1e000)
#define L8_AAD_BASE			(L8_VIRT_PL301_BASE+0x1f000)

#define L8_PL301_B1_BASE		(L8_VIRT_PL301_BASE+0x20000)
#define L8_PL301_B2_BASE		(L8_VIRT_PL301_BASE+0x21000)
#define L8_PL301_A1_BASE		(L8_VIRT_PL301_BASE+0x22000)
#define L8_PL301_A3_BASE		(L8_VIRT_PL301_BASE+0x23000)
#define L8_PL301_A4_BASE		(L8_VIRT_PL301_BASE+0x24000)
#define L8_PL301_C1_BASE		(L8_VIRT_PL301_BASE+0x25000)
#define L8_PL301_C2_BASE		(L8_VIRT_PL301_BASE+0x26000)

/* ------------------------------------------------------------------------
 * Secure Processor Registers
 * ------------------------------------------------------------------------
 */
#define L8_CPU_SHARING_BASE		(L8_SP_BASE+0x000)
#define L8_SP_SHARING_BASE		(L8_SP_BASE+0x004)
#define L8_SHARING_STATUS_BASE	(L8_SP_BASE+0x008)
#define L8_STORE_ENCKEY_BASE	(L8_SP_BASE+0x010)
#define L8_STORE_DECKEY_BASE	(L8_SP_BASE+0x090)
#define L8_SP_ROM_BASE		(L8_SP_BASE+0x200)
#define L8_SP_RAM_BASE		(L8_SP_BASE+0x204)
#define L8_SP_PRID_BASE		(L8_SP_BASE+0x208)
#define L8_SP_CONTROL_BASE		(L8_SP_BASE+0x20c)
#define L8_CPU_INTR_BASE		(L8_SP_BASE+0x210)
#define L8_SP_INTR_BASE		(L8_SP_BASE+0x220)
#define L8_OTP_CONTROL_BASE		(L8_SP_BASE+0x230)
#define L8_SC_CONTROL_BASE		(L8_SP_BASE+0x234)
#define L8_BOOT_STATUS0_BASE	(L8_SP_BASE+0x238)
#define L8_BOOT_STATUS1_BASE	(L8_SP_BASE+0x23c)
#define L8_BOOT_STATUS2_BASE	(L8_SP_BASE+0x240)
#define L8_BOOT_STATUS3_BASE	(L8_SP_BASE+0x244)
#define L8_BOOT_STATUS4_BASE	(L8_SP_BASE+0x248)
#define L8_SP_VERSION		(L8_SP_BASE+0x24c)

/* ------------------------------------------------------------------------
 *  Sync registers for dual core booting
 * ------------------------------------------------------------------------
 */
#ifdef CONFIG_SMP
#define L8_SYNC_BASE		0xffffffff				/* 64M - 8K , FIXME */
#define L8_VIRT_SYNC_BASE		0x00000000
#define L8_CPUSYNC_BASE		(L8_VIRT_SYNC_BASE+0x0)
#define L8_STARTUP_BASE		(L8_VIRT_SYNC_BASE+0x4)
#endif

/* ------------------------------------------------------------------------
 *  Chip select base addresses
 * ------------------------------------------------------------------------
 */
#define L8_CHIPSELECT_BASE		0xb0000000

#define L8_CS3_BASE			0xbc000000
#define L8_VIRT_CS3_BASE		0xf5000000

#define L8_CS2_BASE			0xb8000000
#define L8_VIRT_CS2_BASE		0xf4000000
#define L8_DVBCI_BASE		(L8_VIRT_CS2_BASE+0x0)

#define L8_CS1_BASE			0xb4000000
#define L8_VIRT_CS1_BASE		0xf3000000
#define L8_CS8900_BASE		(L8_VIRT_CS1_BASE+0x0)

#define L8_CS0_BASE			0xb0000000
#define L8_VIRT_CS0_BASE		0xf2000000

/* ------------------------------------------------------------------------
 *  FRC
 * ------------------------------------------------------------------------
 */
#define L8_DDR3_FRC_BASE		0x50000000
#define L8_VIRT_DDR3_FRC_BASE	0xe0000000
#define L8_DRAM_FRC_BASE		(L8_VIRT_DDR3_FRC_BASE+0x0)

/* ------------------------------------------------------------------------
 *  SDRAM
 * ------------------------------------------------------------------------
 */
#define L8_SDRAM0_BASE		0x10000000
#define L8_SDRAM1_BASE		0x50000000
#define L8_SDRAM0_BASE_REMAPPED	0x00000000
#define L8_SDRAM1_BASE_REMAPPED	0x40000000
#define L8_SDRAM_BASE		L8_SDRAM0_BASE_REMAPPED

/* VIC definitions in include/asm-arm/hardware/vic.h */
/* GID definitions in include/asm-arm/hardware/gic.h */

#endif /* L8_MEMC_BASE */

#endif
