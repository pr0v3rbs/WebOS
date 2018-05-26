#ifndef __ASM_ARCH_IRQS_M14_H__
#define __ASM_ARCH_IRQS_M14_H__
/*
 *  linux/include/asm-arm/arch-lg1153/irqs.h
 *
 *  Copyright (C) 2011 LGE Ltd.
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
#define M14_IRQ_LOCALTIMER      29
#define M14_IRQ_LOCALWDOG		30

#define M14_IRQ_GIC_START		32

#define M14_IRQ_UART0			(M14_IRQ_GIC_START +  0)	/* UART 0 */
#define M14_IRQ_UART1			(M14_IRQ_GIC_START +  1)
#define M14_IRQ_UART2			(M14_IRQ_GIC_START +  2)
#define M14_IRQ_SPI0			(M14_IRQ_GIC_START +  3)	/* SPI */
#define M14_IRQ_SPI1			(M14_IRQ_GIC_START +  4)	/* SPI */
#define M14_IRQ_SCI				(M14_IRQ_GIC_START +  5)	/* Smart Card Interface */
#define M14_IRQ_TIMER		    (M14_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define M14_IRQ_TIMER0_1		(M14_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define M14_IRQ_WATCHDOG		(M14_IRQ_GIC_START +  7)	/* Watchdog timer */
#define M14_IRQ_GPIO0_2			(M14_IRQ_GIC_START +  8)	/* GPIO 0 */
#define M14_IRQ_GPIO3_5			(M14_IRQ_GIC_START +  9)	/* GPIO 1 */
#define M14_IRQ_GPIO6_8			(M14_IRQ_GIC_START + 10)
#define M14_IRQ_GPIO9_11		(M14_IRQ_GIC_START + 11)
#define M14_IRQ_I2C0			(M14_IRQ_GIC_START + 12)	/* I2C0 */
#define M14_IRQ_I2C1			(M14_IRQ_GIC_START + 13)	/* I2C1 */
#define M14_IRQ_I2C2			(M14_IRQ_GIC_START + 14)
#define M14_IRQ_I2C3			(M14_IRQ_GIC_START + 15)
#define M14_IRQ_DMAC			(M14_IRQ_GIC_START + 16)	/* DMAINT */

#define M14_IRQ_EMMC			(M14_IRQ_GIC_START + 18)






#define M14_IRQ_DVBCI			(M14_IRQ_GIC_START + 25)
#define M14_IRQ_FEMAC			(M14_IRQ_GIC_START + 26)
#define M14_IRQ_SSUSB			(M14_IRQ_GIC_START + 27)

#define	M14_IRQ_APB_TOUT_CTR	(M14_IRQ_GIC_START + 29)
#define M14_IRQ_I2C4			(M14_IRQ_GIC_START + 30)
#define M14_IRQ_I2C5			(M14_IRQ_GIC_START + 31)
#define M14_IRQ_I2C6			(M14_IRQ_GIC_START + 32)
#define M14_IRQ_I2C7			(M14_IRQ_GIC_START + 33)
#define M14_IRQ_I2C8			(M14_IRQ_GIC_START + 34)
#define M14_IRQ_I2C9		    (M14_IRQ_GIC_START + 35)
#define M14_IRQ_GPIO12_14		(M14_IRQ_GIC_START + 36)
#define M14_IRQ_GPIO15_17		(M14_IRQ_GIC_START + 37)
#define M14_IRQ_SSUSB_HOST_SYSTEM_ERR    (M14_IRQ_GIC_START + 38)




#define M14_IRQ_SDEC			(M14_IRQ_GIC_START + 45)
#define M14_IRQ_DVR			    (M14_IRQ_GIC_START + 46)
#define M14_IRQ_VDEC0			(M14_IRQ_GIC_START + 47)
#define M14_IRQ_VDEC1			(M14_IRQ_GIC_START + 48)
#define M14_IRQ_VENC0			(M14_IRQ_GIC_START + 49)
#define M14_IRQ_VENC1			(M14_IRQ_GIC_START + 50)
#define M14_IRQ_ICOD0			(M14_IRQ_GIC_START + 51)
#define M14_IRQ_ICOD1			(M14_IRQ_GIC_START + 52)
#define M14_IRQ_GPU_GPU		    (M14_IRQ_GIC_START + 53)
#define M14_IRQ_GPU_GFX		    (M14_IRQ_GIC_START + 54)
#define M14_IRQ_AUD0		    (M14_IRQ_GIC_START + 55)
#define M14_IRQ_AUD1			(M14_IRQ_GIC_START + 56)
#define M14_IRQ_AAD		        (M14_IRQ_GIC_START + 57)

#define M14_IRQ_DE_BCPU			(M14_IRQ_GIC_START + 62)
#define M14_IRQ_IPC_BCPU		(M14_IRQ_GIC_START + 63)
#define M14_IRQ_OSD_BCPU		(M14_IRQ_GIC_START + 64)
#define M14_IRQ_CVD_BCPU		(M14_IRQ_GIC_START + 65)
#define M14_IRQ_LBUS		    (M14_IRQ_GIC_START + 66)
#define M14_IRQ_GBUS		    (M14_IRQ_GIC_START + 67)
#define M14_IRQ_SLT		        (M14_IRQ_GIC_START + 68)
#define M14_IRQ_BVE0			(M14_IRQ_GIC_START + 69)
#define M14_IRQ_BVE1			(M14_IRQ_GIC_START + 70)
#define M14_IRQ_HDMI			(M14_IRQ_GIC_START + 71)

/* M14 A0 IRQ NUM */
#define M14_A0_IRQ_SDEC			(M14_IRQ_GIC_START + 45)
#define M14_A0_IRQ_DVR			(M14_IRQ_GIC_START + 46)
#define M14_A0_IRQ_VDEC0		(M14_IRQ_GIC_START + 47)
#define M14_A0_IRQ_VDEC1		(M14_IRQ_GIC_START + 48)
#define M14_A0_IRQ_VENC0		(M14_IRQ_GIC_START + 49)
#define M14_A0_IRQ_VENC1		(M14_IRQ_GIC_START + 50)
#define M14_A0_IRQ_ICOD0		(M14_IRQ_GIC_START + 51)
#define M14_A0_IRQ_ICOD1		(M14_IRQ_GIC_START + 52)
#define M14_A0_IRQ_GPU_GPU		(M14_IRQ_GIC_START + 53)
#define M14_A0_IRQ_GPU_GFX		(M14_IRQ_GIC_START + 54)
#define M14_A0_IRQ_AUD0			(M14_IRQ_GIC_START + 55)
#define M14_A0_IRQ_AUD1			(M14_IRQ_GIC_START + 56)
#define M14_A0_IRQ_AAD			(M14_IRQ_GIC_START + 57)

#define M14_A0_IRQ_DE_BCPU		(M14_IRQ_GIC_START + 62)
#define M14_A0_IRQ_IPC_BCPU		(M14_IRQ_GIC_START + 63)
#define M14_A0_IRQ_OSD_BCPU		(M14_IRQ_GIC_START + 64)
#define M14_A0_IRQ_CVD_BCPU		(M14_IRQ_GIC_START + 65)
#define M14_A0_IRQ_LBUS			(M14_IRQ_GIC_START + 66)
#define M14_A0_IRQ_GBUS			(M14_IRQ_GIC_START + 67)
#define M14_A0_IRQ_SLT			(M14_IRQ_GIC_START + 68)
#define M14_A0_IRQ_BVE0			(M14_IRQ_GIC_START + 69)
#define M14_A0_IRQ_BVE1			(M14_IRQ_GIC_START + 70)
#define M14_A0_IRQ_HDMI			(M14_IRQ_GIC_START + 71)

/* M14 B0 IRQ NUM */
#define M14_B0_IRQ_SDEC			(M14_IRQ_GIC_START + 43)	// modified
#define M14_B0_IRQ_DVR			(M14_IRQ_GIC_START + 44)	// modified
#define M14_B0_IRQ_SDEC1		(M14_IRQ_GIC_START + 45)	// modified
#define M14_B0_IRQ_VDEC0		(M14_IRQ_GIC_START + 48)	// modified
#define M14_B0_IRQ_VDEC1		(M14_IRQ_GIC_START + 49)	// modified
#define M14_B0_IRQ_HEVC			(M14_IRQ_GIC_START + 50)	// modified
#define M14_B0_IRQ_VENC0		(M14_IRQ_GIC_START + 51)	// modified
#define M14_B0_IRQ_VENC1		(M14_IRQ_GIC_START + 52)	// modified
#define M14_B0_IRQ_ICOD0		(M14_IRQ_GIC_START + 53)	// modified
#define M14_B0_IRQ_ICOD1		(M14_IRQ_GIC_START + 54)	// modified
#define M14_B0_IRQ_GPU_GPU		(M14_IRQ_GIC_START + 55)	// modified
#define M14_B0_IRQ_GPU_GFX		(M14_IRQ_GIC_START + 56)	// modified
#define M14_B0_IRQ_AUD0			(M14_IRQ_GIC_START + 57)	// modified
#define M14_B0_IRQ_AUD1			(M14_IRQ_GIC_START + 58)	// modified
#define M14_B0_IRQ_AAD			(M14_IRQ_GIC_START + 59)	// modified

#define M14_B0_IRQ_DE_IDS		(M14_IRQ_GIC_START + 60)	// modified
#define M14_B0_IRQ_DE_IDS_M0	(M14_IRQ_GIC_START + 61)	// modified
#define M14_B0_IRQ_DE_IDS_M1	(M14_IRQ_GIC_START + 62)	// modified
#define M14_B0_IRQ_DE_IDS_CVD	(M14_IRQ_GIC_START + 63)	// modified
#define M14_B0_IRQ_DE_IDS_OSD	(M14_IRQ_GIC_START + 64)	// modified
#define M14_B0_IRQ_HDMI_LINK	(M14_IRQ_GIC_START + 65)	// modified
#define M14_B0_IRQ_LBUS			(M14_IRQ_GIC_START + 66)
#define M14_B0_IRQ_GBUS			(M14_IRQ_GIC_START + 67)
#define M14_B0_IRQ_SLT			(M14_IRQ_GIC_START + 68)




#define M14_IRQ_AFE3CH			(M14_IRQ_GIC_START + 78)
#define M14_IRQ_GBB			    (M14_IRQ_GIC_START + 79)
#define M14_IRQ_M14A_GPIO   	(M14_IRQ_GIC_START + 80)

#define M14_IRQ_EXT0			(M14_IRQ_GIC_START + 82)
#define M14_IRQ_EXT1			(M14_IRQ_GIC_START + 83)
#define M14_IRQ_EXT2			(M14_IRQ_GIC_START + 84)
#define M14_IRQ_EXT3			(M14_IRQ_GIC_START + 85)
#define M14_IRQ_PMUIRQ0			(M14_IRQ_GIC_START + 86)
#define M14_IRQ_PMUIRQ1			(M14_IRQ_GIC_START + 87)

#define M14_IRQ_CTI0		    (M14_IRQ_GIC_START + 89)
#define M14_IRQ_CTI1			(M14_IRQ_GIC_START + 90)

#define M14_NR_IRQS				(M14_IRQ_GIC_START + 128)
#define M14_IRQ_GIC_END			M14_NR_IRQS

#define MAX_GIC_NR			1

#endif /* __ASM_ARCH_IRQS_H__ */
