#ifndef __ASM_ARCH_IRQS_L9_H__
#define __ASM_ARCH_IRQS_L9_H__
/*
 *  linux/include/asm-arm/arch-lg1152/irqs.h
 *
 *  Copyright (C) 2008 LGE Ltd.
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

/* ------------------------------------------------------------------------
 *  Interrupts - bit assignment (primary)
 * ------------------------------------------------------------------------
 */
#define L9_IRQ_LOCALTIMER      29
#define L9_IRQ_LOCALWDOG		30

#define L9_IRQ_GIC_START		32

#define L9_IRQ_UART0			(L9_IRQ_GIC_START +  0)	/* UART 0 */
#define L9_IRQ_UART1			(L9_IRQ_GIC_START +  1)
#define L9_IRQ_UART2			(L9_IRQ_GIC_START +  2)
#define L9_IRQ_SPI0			(L9_IRQ_GIC_START +  3)	/* SPI */
#define L9_IRQ_SPI1			(L9_IRQ_GIC_START +  4)	/* SPI */
#define L9_IRQ_SCI				(L9_IRQ_GIC_START +  5)	/* Smart Card Interface */
#define L9_IRQ_TIMER		(L9_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define L9_IRQ_TIMER0_1		(L9_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define L9_IRQ_WATCHDOG		(L9_IRQ_GIC_START +  7)	/* Watchdog timer */
#define L9_IRQ_GPIO0_2			(L9_IRQ_GIC_START +  8)	/* GPIO 0 */
#define L9_IRQ_GPIO3_5			(L9_IRQ_GIC_START +  9)	/* GPIO 1 */
#define L9_IRQ_GPIO6_8			(L9_IRQ_GIC_START + 10)
#define L9_IRQ_GPIO9_11		(L9_IRQ_GIC_START + 11)
#define L9_IRQ_I2C0			(L9_IRQ_GIC_START + 12)	/* I2C0 */
#define L9_IRQ_I2C1			(L9_IRQ_GIC_START + 13)	/* I2C1 */
#define L9_IRQ_I2C2			(L9_IRQ_GIC_START + 14)
#define L9_IRQ_I2C3			(L9_IRQ_GIC_START + 15)
#define L9_IRQ_DMAC			(L9_IRQ_GIC_START + 16)	/* DMAINT */
#define L9_IRQ_NFC				(L9_IRQ_GIC_START + 17)
#define L9_IRQ_SDHC			(L9_IRQ_GIC_START + 18)
#define L9_IRQ_USB0_OHCI0		(L9_IRQ_GIC_START + 19)
#define L9_IRQ_USB0_EHCI0		(L9_IRQ_GIC_START + 20)
#define L9_IRQ_USB0_OHCI1		(L9_IRQ_GIC_START + 21)
#define L9_IRQ_USB1_OHCI0		(L9_IRQ_GIC_START + 22)
#define L9_IRQ_USB1_EHCI0		(L9_IRQ_GIC_START + 23)
#define L9_IRQ_USB1_OHCI1		(L9_IRQ_GIC_START + 24)
#define L9_IRQ_DVBCI			(L9_IRQ_GIC_START + 25)

#ifdef CONFIG_LG1152_FPGA_REV0
#define L9_IRQ_FEMAC			(L9_IRQ_GIC_START + 24)
#elif defined(CONFIG_LG1152_FPGA_REV1) || defined(CONFIG_BOARD_TYPE_EVALUATION)
#define L9_IRQ_FEMAC			(L9_IRQ_GIC_START + 26)
#endif

#define L9_IRQ_SP				(L9_IRQ_GIC_START + 27)
#define L9_IRQ_SC				(L9_IRQ_GIC_START + 28)
#define	L9_IRQ_APB_TOUT_CTR	(L9_IRQ_GIC_START + 29)
#define L9_IRQ_I2C4			(L9_IRQ_GIC_START + 30)
#define L9_IRQ_I2C5			(L9_IRQ_GIC_START + 31)
#define L9_IRQ_I2C6			(L9_IRQ_GIC_START + 32)
#define L9_IRQ_I2C7			(L9_IRQ_GIC_START + 33)
#define L9_IRQ_I2C8			(L9_IRQ_GIC_START + 34)
#define L9_IRQ_RESERVED0		(L9_IRQ_GIC_START + 35)
#define L9_IRQ_RESERVED1		(L9_IRQ_GIC_START + 36)
#define L9_IRQ_RESERVED2		(L9_IRQ_GIC_START + 37)
#define L9_IRQ_RESERVED3		(L9_IRQ_GIC_START + 38)
#define L9_IRQ_RESERVED4		(L9_IRQ_GIC_START + 39)
#define L9_IRQ_RESERVED5		(L9_IRQ_GIC_START + 40)
#define L9_IRQ_RESERVED6		(L9_IRQ_GIC_START + 41)
#define L9_IRQ_RESERVED7		(L9_IRQ_GIC_START + 42)
#define L9_IRQ_ATLAS0			(L9_IRQ_GIC_START + 43)
#define L9_IRQ_ATLAS1			(L9_IRQ_GIC_START + 44)
#define L9_IRQ_ATLAS2			(L9_IRQ_GIC_START + 45)
#define L9_IRQ_VENC0			(L9_IRQ_GIC_START + 46)
#define L9_IRQ_VENC1			(L9_IRQ_GIC_START + 47)
#define L9_IRQ_VDEC0			(L9_IRQ_GIC_START + 48)
#define L9_IRQ_VDEC1			(L9_IRQ_GIC_START + 49)
#define L9_IRQ_ICOD0			(L9_IRQ_GIC_START + 50)
#define L9_IRQ_ICOD1			(L9_IRQ_GIC_START + 51)
#define L9_IRQ_GPU_GP			(L9_IRQ_GIC_START + 52)
#define L9_IRQ_GPU_GPMMU		(L9_IRQ_GIC_START + 53)	
#define L9_IRQ_GPU_PP0			(L9_IRQ_GIC_START + 54)
#define L9_IRQ_GPU_PPMMU0		(L9_IRQ_GIC_START + 55)	
#define L9_IRQ_GPU_PP1			(L9_IRQ_GIC_START + 56)
#define L9_IRQ_GPU_PPMMU1		(L9_IRQ_GIC_START + 57)	
#define L9_IRQ_GPU_PP2			(L9_IRQ_GIC_START + 58)
#define L9_IRQ_GPU_PPMMU2		(L9_IRQ_GIC_START + 59)	
#define L9_IRQ_GPU_PP3			(L9_IRQ_GIC_START + 60)
#define L9_IRQ_GPU_PPMMU3		(L9_IRQ_GIC_START + 61)	
#define L9_IRQ_GPU_PMU			(L9_IRQ_GIC_START + 62)
#define L9_IRQ_AUD0			(L9_IRQ_GIC_START + 63)
#define L9_IRQ_AUD1			(L9_IRQ_GIC_START + 64)
#define L9_IRQ_DE_BCPU			(L9_IRQ_GIC_START + 65)
#define L9_IRQ_IPC_BCPU		(L9_IRQ_GIC_START + 66)	
#define L9_IRQ_CVD_BCPU		(L9_IRQ_GIC_START + 67)	
#define L9_IRQ_OSD_BCPU		(L9_IRQ_GIC_START + 68)	
#define L9_IRQ_SDEC			(L9_IRQ_GIC_START + 69)
#define L9_IRQ_DVR				(L9_IRQ_GIC_START + 70)	
#define L9_IRQ_AAD				(L9_IRQ_GIC_START + 71)
#define L9_IRQ_GFX				(L9_IRQ_GIC_START + 72)	
#define L9_IRQ_SLT				(L9_IRQ_GIC_START + 73)	
#define L9_IRQ_RESERVED8		(L9_IRQ_GIC_START + 74)		
#define L9_IRQ_RESERVED9		(L9_IRQ_GIC_START + 75)		
#define L9_IRQ_RESERVED10		(L9_IRQ_GIC_START + 76)		
#define L9_IRQ_RESERVED11		(L9_IRQ_GIC_START + 77)		
#define L9_IRQ_HDMI1			(L9_IRQ_GIC_START + 78)	
#define L9_IRQ_HDMI2			(L9_IRQ_GIC_START + 79)	
#define L9_IRQ_AFE3CH			(L9_IRQ_GIC_START + 80)	
#define L9_IRQ_GBB				(L9_IRQ_GIC_START + 81)
#define L9_IRQ_EXT0			(L9_IRQ_GIC_START + 82)	
#define L9_IRQ_EXT1			(L9_IRQ_GIC_START + 83)	
#define L9_IRQ_EXT2			(L9_IRQ_GIC_START + 84)	
#define L9_IRQ_EXT3			(L9_IRQ_GIC_START + 85) 	
#define L9_IRQ_PMUIRQ0			(L9_IRQ_GIC_START + 86)	
#define L9_IRQ_PMUIRQ1			(L9_IRQ_GIC_START + 87)	
#define L9_IRQ_L2CC			(L9_IRQ_GIC_START + 88)	

#define L9_NR_IRQS				(L9_IRQ_GIC_START + 128)
#define L9_IRQ_GIC_END			L9_NR_IRQS

#define MAX_GIC_NR			1

#endif /* __ASM_ARCH_IRQS_H__ */
