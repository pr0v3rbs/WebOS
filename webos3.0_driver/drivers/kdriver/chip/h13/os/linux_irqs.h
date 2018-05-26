#ifndef __ASM_ARCH_IRQS_H13_H__
#define __ASM_ARCH_IRQS_H13_H__
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
#define H13_IRQ_LOCALTIMER      29
#define H13_IRQ_LOCALWDOG		30

#define H13_IRQ_GIC_START		32

#define H13_IRQ_UART0			(H13_IRQ_GIC_START +  0)	/* UART 0 */
#define H13_IRQ_UART1			(H13_IRQ_GIC_START +  1)
#define H13_IRQ_UART2			(H13_IRQ_GIC_START +  2)
#define H13_IRQ_SPI0			(H13_IRQ_GIC_START +  3)	/* SPI */
#define H13_IRQ_SPI1			(H13_IRQ_GIC_START +  4)	/* SPI */
#define H13_IRQ_SCI				(H13_IRQ_GIC_START +  5)	/* Smart Card Interface */
#define H13_IRQ_TIMER		    (H13_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define H13_IRQ_TIMER0_1		(H13_IRQ_GIC_START +  6)	/* Timer 0 and 1 */
#define H13_IRQ_WATCHDOG		(H13_IRQ_GIC_START +  7)	/* Watchdog timer */
#define H13_IRQ_GPIO0_2			(H13_IRQ_GIC_START +  8)	/* GPIO 0 */
#define H13_IRQ_GPIO3_5			(H13_IRQ_GIC_START +  9)	/* GPIO 1 */
#define H13_IRQ_GPIO6_8			(H13_IRQ_GIC_START + 10)
#define H13_IRQ_GPIO9_11		(H13_IRQ_GIC_START + 11)
#define H13_IRQ_I2C0			(H13_IRQ_GIC_START + 12)	/* I2C0 */
#define H13_IRQ_I2C1			(H13_IRQ_GIC_START + 13)	/* I2C1 */
#define H13_IRQ_I2C2			(H13_IRQ_GIC_START + 14)
#define H13_IRQ_I2C3			(H13_IRQ_GIC_START + 15)
#define H13_IRQ_DMAC			(H13_IRQ_GIC_START + 16)	/* DMAINT */

#define H13_IRQ_EMMC			(H13_IRQ_GIC_START + 18)






#define H13_IRQ_DVBCI			(H13_IRQ_GIC_START + 25)
#define H13_IRQ_FEMAC			(H13_IRQ_GIC_START + 26)
#define H13_IRQ_SSUSB			(H13_IRQ_GIC_START + 27)

#define	H13_IRQ_APB_TOUT_CTR	(H13_IRQ_GIC_START + 29)
#define H13_IRQ_I2C4			(H13_IRQ_GIC_START + 30)
#define H13_IRQ_I2C5			(H13_IRQ_GIC_START + 31)
#define H13_IRQ_I2C6			(H13_IRQ_GIC_START + 32)
#define H13_IRQ_I2C7			(H13_IRQ_GIC_START + 33)
#define H13_IRQ_I2C8			(H13_IRQ_GIC_START + 34)
#define H13_IRQ_I2C9		    (H13_IRQ_GIC_START + 35)
#define H13_IRQ_GPIO12_14		(H13_IRQ_GIC_START + 36)
#define H13_IRQ_GPIO15_17		(H13_IRQ_GIC_START + 37)
#define H13_IRQ_SSUSB_HOST_SYSTEM_ERR    (H13_IRQ_GIC_START + 38)




#define H13_IRQ_ATLAS0			(H13_IRQ_GIC_START + 43)
#define H13_IRQ_ATLAS1			(H13_IRQ_GIC_START + 44)
#define H13_IRQ_SDEC			(H13_IRQ_GIC_START + 45)
#define H13_IRQ_DVR			    (H13_IRQ_GIC_START + 46)
#define H13_IRQ_VDEC0			(H13_IRQ_GIC_START + 47)
#define H13_IRQ_VDEC1			(H13_IRQ_GIC_START + 48)
#define H13_IRQ_VENC0			(H13_IRQ_GIC_START + 49)
#define H13_IRQ_VENC1			(H13_IRQ_GIC_START + 50)
#define H13_IRQ_ICOD0			(H13_IRQ_GIC_START + 51)
#define H13_IRQ_ICOD1			(H13_IRQ_GIC_START + 52)
#define H13_IRQ_GPU_GPU		    (H13_IRQ_GIC_START + 53)	
#define H13_IRQ_GPU_GFX		    (H13_IRQ_GIC_START + 54)
#define H13_IRQ_AUD0		    (H13_IRQ_GIC_START + 55)	
#define H13_IRQ_AUD1			(H13_IRQ_GIC_START + 56)
#define H13_IRQ_AAD		        (H13_IRQ_GIC_START + 57)




#define H13_IRQ_DE_BCPU			(H13_IRQ_GIC_START + 62)
#define H13_IRQ_IPC_BCPU		(H13_IRQ_GIC_START + 63)
#define H13_IRQ_OSD_BCPU		(H13_IRQ_GIC_START + 64)
#define H13_IRQ_CVD_BCPU		(H13_IRQ_GIC_START + 65)
#define H13_IRQ_LBUS		    (H13_IRQ_GIC_START + 66)	
#define H13_IRQ_GBUS		    (H13_IRQ_GIC_START + 67)	
#define H13_IRQ_SLT		        (H13_IRQ_GIC_START + 68)	
#define H13_IRQ_BVE0			(H13_IRQ_GIC_START + 69)
#define H13_IRQ_BVE1			(H13_IRQ_GIC_START + 70)	
#define H13_IRQ_HDMI		(H13_IRQ_GIC_START + 71)






#define H13_IRQ_AFE3CH			(H13_IRQ_GIC_START + 78)	
#define H13_IRQ_GBB			    (H13_IRQ_GIC_START + 79)	
#define H13_IRQ_H13A_GPIO   	(H13_IRQ_GIC_START + 80)

#define H13_IRQ_EXT0			(H13_IRQ_GIC_START + 82)	
#define H13_IRQ_EXT1			(H13_IRQ_GIC_START + 83)	
#define H13_IRQ_EXT2			(H13_IRQ_GIC_START + 84)	
#define H13_IRQ_EXT3			(H13_IRQ_GIC_START + 85) 	
#define H13_IRQ_PMUIRQ0			(H13_IRQ_GIC_START + 86)	
#define H13_IRQ_PMUIRQ1			(H13_IRQ_GIC_START + 87)	

#define H13_IRQ_CTI0		    (H13_IRQ_GIC_START + 89)	
#define H13_IRQ_CTI1			(H13_IRQ_GIC_START + 90)

#define H13_NR_IRQS				(H13_IRQ_GIC_START + 128)
#define H13_IRQ_GIC_END			H13_NR_IRQS

#define MAX_GIC_NR			1

#endif /* __ASM_ARCH_IRQS_H__ */
