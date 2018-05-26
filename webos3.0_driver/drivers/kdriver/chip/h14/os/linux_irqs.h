#ifndef __ASM_ARCH_IRQS_H14_H__
#define __ASM_ARCH_IRQS_H14_H__
/*
 *  linux/include/asm-arm/arch-lg115x/irqs.h
 *
 *  Copyright (C) 2011 LGE Ltd.
 */

/* ------------------------------------------------------------------------
 *  Interrupts - bit assignment (primary)
 * ------------------------------------------------------------------------
 */
//#define MPCORE_GTIMER_IRQ	27
#define H14_IRQ_LOCALTIMER	29
#define H14_IRQ_LOCALWDOG	30

#define H14_IRQ_GIC_START	32

#define H14_IRQ_UART0		(H14_IRQ_GIC_START +  0)
#define H14_IRQ_UART1		(H14_IRQ_GIC_START +  1)
#define H14_IRQ_UART2		(H14_IRQ_GIC_START +  2)
#define H14_IRQ_SPI0		(H14_IRQ_GIC_START +  3)
#define H14_IRQ_SPI1		(H14_IRQ_GIC_START +  4)
#define H14_IRQ_SCI			(H14_IRQ_GIC_START +  5)
#define H14_IRQ_TIMER		(H14_IRQ_GIC_START +  6)
#define H14_IRQ_WDOG0		(H14_IRQ_GIC_START +  7)
#define H14_IRQ_GPIO0_2		(H14_IRQ_GIC_START +  8)
#define H14_IRQ_GPIO3_5		(H14_IRQ_GIC_START +  9)
#define H14_IRQ_GPIO6_8		(H14_IRQ_GIC_START + 10)
#define H14_IRQ_GPIO9_11	(H14_IRQ_GIC_START + 11)
#define H14_IRQ_I2C0		(H14_IRQ_GIC_START + 12)
#define H14_IRQ_I2C1		(H14_IRQ_GIC_START + 13)
#define H14_IRQ_I2C2		(H14_IRQ_GIC_START + 14)
#define H14_IRQ_I2C3		(H14_IRQ_GIC_START + 15)
#define H14_IRQ_DMAC0		(H14_IRQ_GIC_START + 16)
#define H14_IRQ_IRBLASTER	(H14_IRQ_GIC_START + 17)
#define H14_IRQ_EMMC		(H14_IRQ_GIC_START + 18)
#define H14_IRQ_SSUSB0		(H14_IRQ_GIC_START + 19)
#define H14_IRQ_SSUSB0_ERR	(H14_IRQ_GIC_START + 20)
#define H14_IRQ_SSUSB1		(H14_IRQ_GIC_START + 21)
#define H14_IRQ_SSUSB1_ERR	(H14_IRQ_GIC_START + 22)
#define H14_IRQ_SSUSB2		(H14_IRQ_GIC_START + 23)
#define H14_IRQ_SSUSB2_ERR	(H14_IRQ_GIC_START + 24)
#define H14_IRQ_DVBCI		(H14_IRQ_GIC_START + 25)
/* reserved 26 */
#define H14_IRQ_64TIMER		(H14_IRQ_GIC_START + 27)
#define H14_IRQ_SC			(H14_IRQ_GIC_START + 28)
#define H14_IRQ_APB_TOUT_CTR	(H14_IRQ_GIC_START + 29)
#define H14_IRQ_I2C4		(H14_IRQ_GIC_START + 30)
#define H14_IRQ_I2C5		(H14_IRQ_GIC_START + 31)
#define H14_IRQ_I2C6		(H14_IRQ_GIC_START + 32)
#define H14_IRQ_I2C7		(H14_IRQ_GIC_START + 33)
#define H14_IRQ_I2C8		(H14_IRQ_GIC_START + 34)
#define H14_IRQ_I2C9		(H14_IRQ_GIC_START + 35)
#define H14_IRQ_GPIO12_14	(H14_IRQ_GIC_START + 36)
#define H14_IRQ_GPIO15_17	(H14_IRQ_GIC_START + 37)
#define H14_IRQ_GEM			(H14_IRQ_GIC_START + 38)
/* reserved 39,40 */
#define H14_IRQ_SSUSB3		(H14_IRQ_GIC_START + 41)
#define H14_IRQ_SSUSB3_ERR	(H14_IRQ_GIC_START + 42)
#define H14_IRQ_ATLAS0		(H14_IRQ_GIC_START + 43)
#define H14_IRQ_ATLAS1		(H14_IRQ_GIC_START + 44)
#define H14_IRQ_SDEC0		(H14_IRQ_GIC_START + 45)
#define H14_IRQ_DVR			(H14_IRQ_GIC_START + 46)
#define H14_IRQ_VDEC0		(H14_IRQ_GIC_START + 47)
#define H14_IRQ_VDEC1		(H14_IRQ_GIC_START + 48)
#define H14_IRQ_VENC0		(H14_IRQ_GIC_START + 49)
#define H14_IRQ_VENC1		(H14_IRQ_GIC_START + 50)
#define H14_IRQ_ICOD0		(H14_IRQ_GIC_START + 51)
#define H14_IRQ_ICOD1		(H14_IRQ_GIC_START + 52)
#define H14_IRQ_GPU_GP		(H14_IRQ_GIC_START + 53)
#define H14_IRQ_GFX			(H14_IRQ_GIC_START + 54)
#define H14_IRQ_AUD0		(H14_IRQ_GIC_START + 55)
#define H14_IRQ_AUD1		(H14_IRQ_GIC_START + 56)
#define H14_IRQ_AAD			(H14_IRQ_GIC_START + 57)
#define H14_IRQ_SDEC1		(H14_IRQ_GIC_START + 58)
/* free/reserved IRQ +59 */
/* free/reserved IRQ +60 */
/* free/reserved IRQ +61 */
#define H14_IRQ_DE_BCPU		(H14_IRQ_GIC_START + 62)
#define H14_IRQ_IPC_BCPU		(H14_IRQ_GIC_START + 63)
#define H14_IRQ_OSD_BCPU		(H14_IRQ_GIC_START + 64)
#define H14_IRQ_CVD_BCPU		(H14_IRQ_GIC_START + 65)
#define H14_IRQ_LBUS		(H14_IRQ_GIC_START + 66)
#define H14_IRQ_GBUS		(H14_IRQ_GIC_START + 67)
#define H14_IRQ_SLT			(H14_IRQ_GIC_START + 68)
#define H14_IRQ_BVE1		(H14_IRQ_GIC_START + 69)		// differ from H13/M14
#define H14_IRQ_BVE0		(H14_IRQ_GIC_START + 70)		// differ from H13/M14
#define H14_IRQ_HDMI		(H14_IRQ_GIC_START + 71)
/* free/reserved IRQ +72 */
/* free/reserved IRQ +73 */
/* free/reserved IRQ +74 */
/* free/reserved IRQ +75 */
/* free/reserved IRQ +76 */
/* free/reserved IRQ +77 */
#define H14_IRQ_AFE3CH		(H14_IRQ_GIC_START + 78)
#define H14_IRQ_GBB			(H14_IRQ_GIC_START + 79)
#define H14_IRQ_H13A_GPIO		(H14_IRQ_GIC_START + 80)
/* free/reserved IRQ +81 */
#define H14_IRQ_EXT0		(H14_IRQ_GIC_START + 82)
#define H14_IRQ_EXT1		(H14_IRQ_GIC_START + 83)
#define H14_IRQ_EXT2		(H14_IRQ_GIC_START + 84)
#define H14_IRQ_EXT3		(H14_IRQ_GIC_START + 85)
#define H14_IRQ_PMUIRQ0		(H14_IRQ_GIC_START + 86)
#define H14_IRQ_PMUIRQ1		(H14_IRQ_GIC_START + 87)
#define H14_IRQ_L2CC		(H14_IRQ_GIC_START + 88)

#define H14_NR_IRQS			(H14_IRQ_GIC_START + 128)

#define MAX_GIC_NR			1

#endif /* __ASM_ARCH_IRQS_H14_H__ */

