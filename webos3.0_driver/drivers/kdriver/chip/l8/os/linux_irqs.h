#ifndef __ASM_ARCH_IRQS_L8_H__
#define __ASM_ARCH_IRQS_L8_H__
/*
 *  linux/include/asm-arm/arch-dtvsoc/irqs.h
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
#define L8_INT_UART0		0	/* UART 0 */
#define L8_INT_UART1		1
#define L8_INT_UART2		2
#define L8_INT_SPI0		3	/* SPI */
#define L8_INT_CPU_SP		3
#define L8_INT_SPI1		4	/* SPI */
#define L8_INT_SCI			5	/* Smart Card Interface */
#define L8_INT_TIMER0_1		6	/* Timer 0 and 1 */
#define L8_INT_WDOG		7	/* Watchdog timer */
#define L8_INT_GPIO0		8	/* GPIO 0 */
#define L8_INT_GPIO1		9	/* GPIO 1 */
#define L8_INT_GPIO2		10
#define L8_INT_GPIO3		11
#define L8_INT_I2C0		12	/* I2C0 */
#define L8_INT_I2C1		13	/* I2C1 */
#define L8_INT_I2C2		14
#define L8_INT_I2C3		15
#define L8_INT_DMAC		16	/* DMAINT */
#define L8_INT_NFC			17
#define L8_INT_SDHC		18
#define L8_INT_USB0_OHCI		19
#define L8_INT_USB0_EHCI		20
#define L8_INT_USB1_OHCI		21
#define L8_INT_USB1_EHCI		22
#define L8_INT_DVBCI		23
#define L8_INT_FEMAC		24
#define L8_INT_SP			25
#define L8_INT_SC			26
#define	L8_INT_APB_TOUT_CTRL	27
#define L8_INT_SLIKROAD0		28
#define L8_INT_SLIKROAD1		29
#define L8_INT_SLIKROAD2		30
#define L8_INT_FRC			31
#define L8_INT_VENC0		32
#define L8_INT_VENC1		33
#define L8_INT_VDEC		34
#define L8_INT_GPU_GP		35
#define L8_INT_GPU_MMUGP		36
#define L8_INT_GPU_PP0		37
#define L8_INT_GPU_MMUPP0		38
#define L8_INT_AUD0		39
#define L8_INT_AUD1		40
#define L8_INT_DE			41
#define L8_INT_DE_IPC		42
#define L8_INT_SDEC		43	/* SDEC */
#define L8_INT_DVR			44	/* DVR */
#define L8_INT_VP_IRIS		45
#define L8_INT_VP_HDMI		46
#define L8_INT_VP_3CH		47
#define	L8_INT_AAD			48
#define	L8_INT_GFX			49
#define	L8_INT_SLT			50
#define L8_INT_EXT0		51
#define L8_INT_EXT1		52	/* 80 */
#define L8_INT_EXT2		53	/* EXT2 */
#define L8_INT_EXT3		54
#define L8_INT_USB2_OHCI		55
#define L8_INT_USB3_OHCI		56
#define L8_INT_USB2_EHCI		57
#define L8_INT_I2C4		58	/* I2C4 */
#define L8_INT_I2C5		59	/* I2C5 */
#define L8_INT_PMUL8_IRQ_CPU0		60
#if 0
/*
 *  Interrupt bit positions
 *
 */
#define L8_INTMASK_UART0          (1 << L8_INT_UART0)
#define L8_INTMASK_UART1          (1 << L8_INT_UART1)
#define L8_INTMASK_UART2          (1 << L8_INT_UART2)
#define L8_INTMASK_CPU_SP		(1 << L8_INT_CPU_SE)
#define L8_INTMASK_SPI0           (1 << L8_INT_SPI0)
#define L8_INTMASK_SPI1           (1 << L8_INT_SPI1)
#define L8_INTMASK_SCI            (1 << L8_INT_SCI)
#define L8_INTMASK_TIMER0_1       (1 << L8_INT_TIMER0_1)
#define L8_INTMASK_WDOG           (1 << L8_INT_WDOG)
#define L8_INTMASK_GPIO0          (1 << L8_INT_GPIO0)
#define L8_INTMASK_GPIO1          (1 << L8_INT_GPIO1)
#define L8_INTMASK_GPIO2          (1 << L8_INT_GPIO2)
#define L8_INTMASK_GPIO3          (1 << L8_INT_GPIO3)
#define L8_INTMASK_I2C0           (1 << L8_INT_I2C0)
#define L8_INTMASK_I2C1           (1 << L8_INT_I2C1)
#define L8_INTMASK_I2C2           (1 << L8_INT_I2C2)
#define L8_INTMASK_I2C3           (1 << L8_INT_I2C3)
#define L8_INTMASK_DMAC           (1 << L8_INT_DMAC)
#define L8_INTMASK_NFC            (1 << L8_INT_NFC)
#define L8_INTMASK_SDHC           (1 << L8_INT_SDHC)
#define L8_INTMASK_USB0_OHCI      (1 << L8_INT_USB0_OHCI)
#define L8_INTMASK_USB0_EHCI      (1 << L8_INT_USB0_EHCI)
#define L8_INTMASK_USB1_OHCI      (1 << L8_INT_USB1_OHCI)
#define L8_INTMASK_USB1_EHCI      (1 << L8_INT_USB1_EHCI)
#define L8_INTMASK_DVBCI          (1 << L8_INT_DVBCI)
#define L8_INTMASK_FEMAC          (1 << L8_INT_FEMAC)
#define L8_INTMASK_SP             (1 << L8_INT_SP)
#define L8_INTMASK_SC             (1 << L8_INT_SC)
#define L8_INTMASK_APB_TOUT_CTRL  (1 << L8_INT_APB_TOUT_CTRL)
#define L8_INTMASK_SLIKROAD0      (1 << L8_INT_SLIKROAD0)
#define L8_INTMASK_SLIKROAD1      (1 << L8_INT_SLIKROAD1)
#define L8_INTMASK_SLIKROAD2      (1 << L8_INT_SLIKROAD2)
#define L8_INTMASK_FRC            (1 << L8_INT_FRC)
#define L8_INTMASK_VENC0          (1 << L8_INT_VENC0)
#define L8_INTMASK_VENC1          (1 << L8_INT_VENC1)
#define L8_INTMASK_VDEC           (1 << L8_INT_VDEC)
#define L8_INTMASK_GPU_GP         (1 << L8_INT_GPU_GP)
#define L8_INTMASK_GPU_MMUGP      (1 << L8_INT_GPU_MMUGP)
#define L8_INTMASK_GPU_PP0        (1 << L8_INT_GPU_PP0)
#define L8_INTMASK_GPU_MMUPP0     (1 << L8_INT_GPU_MMUPP0)
#define L8_INTMASK_AUD0           (1 << L8_INT_AUD0)
#define L8_INTMASK_AUD1           (1 << L8_INT_AUD1)
#define L8_INTMASK_DE             (1 << L8_INT_DE)
#define L8_INTMASK_DE_IPC         (1 << L8_INT_DE_IPC)
#define L8_INTMASK_SDEC           (1 << L8_INT_SDEC)
#define L8_INTMASK_DVR            (1 << L8_INT_DVR)
#define L8_INTMASK_VP_IRIS        (1 << L8_INT_VP_IRIS)
#define L8_INTMASK_VP_HDMI        (1 << L8_INT_VP_HDMI)
#define L8_INTMASK_VP_3CH         (1 << L8_INT_VP_3CH)
#define L8_INTMASK_AAD            (1 << L8_INT_AAD)
#define L8_INTMASK_GFX            (1 << L8_INT_GFX)
#define L8_INTMASK_SLT            (1 << L8_INT_SLT)
#define L8_INTMASK_EXT0           (1 << L8_INT_EXT0)
#define L8_INTMASK_EXT1           (1 << L8_INT_EXT1)
#define L8_INTMASK_EXT2           (1 << L8_INT_EXT2)
#define L8_INTMASK_EXT3           (1 << L8_INT_EXT3)
#endif

/*
 *  IRQ interrupts definitions are the same the INT definitions
 *  held within platform.h
 */

#define L8_INT_LOCALTIMER	29
#define L8_INTMASK_LOCALTIMER	(1 << L8_INT_LOCALTIMER)
#define L8_IRQ_LOCALTIMER		L8_INT_LOCALTIMER

#define L8_INT_LOCALWDOG		30
#define L8_INTMASK_LOCALWDOG	(1 << L8_INT_LOCALWDOG)
#define L8_IRQ_LOCALWDOG		L8_INT_LOCALWDOG

#define L8_IRQ_GIC_START		32

#define L8_IRQ_UART0          (L8_IRQ_GIC_START + L8_INT_UART0)
#define L8_IRQ_UART1          (L8_IRQ_GIC_START + L8_INT_UART1)
#define L8_IRQ_UART2          (L8_IRQ_GIC_START + L8_INT_UART2)
#define L8_IRQ_CPU_SP         (L8_IRQ_GIC_START + L8_INT_CPU_SP)
#define L8_IRQ_SPI0           (L8_IRQ_GIC_START + L8_INT_SPI0)
#define L8_IRQ_SPI1           (L8_IRQ_GIC_START + L8_INT_SPI1)
#define L8_IRQ_SCI            (L8_IRQ_GIC_START + L8_INT_SCI)
#define L8_IRQ_TIMER0_1       (L8_IRQ_GIC_START + L8_INT_TIMER0_1)
#define L8_IRQ_WDOG           (L8_IRQ_GIC_START + L8_INT_WDOG)
#define L8_IRQ_GPIO0          (L8_IRQ_GIC_START + L8_INT_GPIO0)
#define L8_IRQ_GPIO1          (L8_IRQ_GIC_START + L8_INT_GPIO1)
#define L8_IRQ_GPIO2          (L8_IRQ_GIC_START + L8_INT_GPIO2)
#define L8_IRQ_GPIO3          (L8_IRQ_GIC_START + L8_INT_GPIO3)
#define L8_IRQ_I2C0           (L8_IRQ_GIC_START + L8_INT_I2C0)
#define L8_IRQ_I2C1           (L8_IRQ_GIC_START + L8_INT_I2C1)
#define L8_IRQ_I2C2           (L8_IRQ_GIC_START + L8_INT_I2C2)
#define L8_IRQ_I2C3           (L8_IRQ_GIC_START + L8_INT_I2C3)
#define L8_IRQ_DMAC           (L8_IRQ_GIC_START + L8_INT_DMAC)
#define L8_IRQ_NFC            (L8_IRQ_GIC_START + L8_INT_NFC)
#define L8_IRQ_SDHC           (L8_IRQ_GIC_START + L8_INT_SDHC)
#define L8_IRQ_USB0_OHCI      (L8_IRQ_GIC_START + L8_INT_USB0_OHCI)
#define L8_IRQ_USB0_EHCI      (L8_IRQ_GIC_START + L8_INT_USB0_EHCI)
#define L8_IRQ_USB1_OHCI      (L8_IRQ_GIC_START + L8_INT_USB1_OHCI)
#define L8_IRQ_USB1_EHCI      (L8_IRQ_GIC_START + L8_INT_USB1_EHCI)
#define L8_IRQ_DVBCI          (L8_IRQ_GIC_START + L8_INT_DVBCI)
#define L8_IRQ_FEMAC          (L8_IRQ_GIC_START + L8_INT_FEMAC)
#define L8_IRQ_SP             (L8_IRQ_GIC_START + L8_INT_SP)
#define L8_IRQ_SC             (L8_IRQ_GIC_START + L8_INT_SC)
#define	L8_IRQ_APB_TOUT_CTRL  (L8_IRQ_GIC_START + L8_INT_APB_TOUT_CTRL)
#define L8_IRQ_SLIKROAD0      (L8_IRQ_GIC_START + L8_INT_SLIKROAD0)
#define L8_IRQ_SLIKROAD1      (L8_IRQ_GIC_START + L8_INT_SLIKROAD1)
#define L8_IRQ_SLIKROAD2      (L8_IRQ_GIC_START + L8_INT_SLIKROAD2)
#define L8_IRQ_FRC            (L8_IRQ_GIC_START + L8_INT_FRC)
#define L8_IRQ_VENC0          (L8_IRQ_GIC_START + L8_INT_VENC0)
#define L8_IRQ_VENC1          (L8_IRQ_GIC_START + L8_INT_VENC1)
#define L8_IRQ_VDEC           (L8_IRQ_GIC_START + L8_INT_VDEC)
#define L8_IRQ_GPU_GP         (L8_IRQ_GIC_START + L8_INT_GPU_GP)
#define L8_IRQ_GPU_MMUGP      (L8_IRQ_GIC_START + L8_INT_GPU_MMUGP)
#define L8_IRQ_GPU_PP0        (L8_IRQ_GIC_START + L8_INT_GPU_PP0)
#define L8_IRQ_GPU_MMUPP0     (L8_IRQ_GIC_START + L8_INT_GPU_MMUPP0)
#define L8_IRQ_AUD0           (L8_IRQ_GIC_START + L8_INT_AUD0)
#define L8_IRQ_AUD1           (L8_IRQ_GIC_START + L8_INT_AUD1)
#define L8_IRQ_DE             (L8_IRQ_GIC_START + L8_INT_DE)
#define L8_IRQ_DE_IPC         (L8_IRQ_GIC_START + L8_INT_DE_IPC)
#define L8_IRQ_SDEC           (L8_IRQ_GIC_START + L8_INT_SDEC)
#define L8_IRQ_DVR            (L8_IRQ_GIC_START + L8_INT_DVR)
#define L8_IRQ_VP_IRIS        (L8_IRQ_GIC_START + L8_INT_VP_IRIS)
#define L8_IRQ_VP_HDMI        (L8_IRQ_GIC_START + L8_INT_VP_HDMI)
#define L8_IRQ_VP_3CH         (L8_IRQ_GIC_START + L8_INT_VP_3CH)
#define L8_IRQ_AAD            (L8_IRQ_GIC_START + L8_INT_AAD)
#define L8_IRQ_GFX            (L8_IRQ_GIC_START + L8_INT_GFX)
#define L8_IRQ_SLT            (L8_IRQ_GIC_START + L8_INT_SLT)
#define L8_IRQ_EXT0           (L8_IRQ_GIC_START + L8_INT_EXT0)
#define L8_IRQ_EXT1           (L8_IRQ_GIC_START + L8_INT_EXT1)
#define L8_IRQ_EXT2           (L8_IRQ_GIC_START + L8_INT_EXT2)
#define L8_IRQ_EXT3           (L8_IRQ_GIC_START + L8_INT_EXT3)
#define L8_IRQ_USB2_OHCI      (L8_IRQ_GIC_START + L8_INT_USB2_OHCI)
#define L8_IRQ_USB3_OHCI      (L8_IRQ_GIC_START + L8_INT_USB3_OHCI)
#define L8_IRQ_USB2_EHCI      (L8_IRQ_GIC_START + L8_INT_USB2_EHCI)
#define L8_IRQ_I2C4           (L8_IRQ_GIC_START + L8_INT_I2C4)
#define L8_IRQ_I2C5           (L8_IRQ_GIC_START + L8_INT_I2C5)
#define L8_IRQ_PMU_CPU0       (L8_IRQ_GIC_START + L8_INT_PMUL8_IRQ_CPU0)

#define L8_NR_IRQS            (L8_IRQ_GIC_START + 64)
#define L8_IRQ_GIC_END        (L8_IRQ_GIC_START + L8_NR_IRQS)

#if 0
#define IRQMASK_UART0      L8_INTMASK_UART0
#define IRQMASK_UART1      L8_INTMASK_UART1
#define IRQMASK_UART2      L8_INTMASK_UART2
#define IRQMASK_CPU_SP		L8_INTMASK_CPU_SP
#define IRQMASK_SPI0       L8_INTMASK_SPI0
#define IRQMASK_SPI1       L8_INTMASK_SPI1
#define IRQMASK_SCI        L8_INTMASK_SCI
#define IRQMASK_TIMER0_1   L8_INTMASK_TIMER0_1
#define IRQMASK_WDOG       L8_INTMASK_WDOG
#define IRQMASK_GPIO0      L8_INTMASK_GPIO0
#define IRQMASK_GPIO1      L8_INTMASK_GPIO1
#define IRQMASK_GPIO2      L8_INTMASK_GPIO2
#define IRQMASK_GPIO3      L8_INTMASK_GPIO3
#define IRQMASK_I2C0       L8_INTMASK_I2C0
#define IRQMASK_I2C1       L8_INTMASK_I2C1
#define IRQMASK_I2C2       L8_INTMASK_I2C2
#define IRQMASK_I2C3       L8_INTMASK_I2C3
#define IRQMASK_DMAC       L8_INTMASK_DMAC
#define IRQMASK_NFC        L8_INTMASK_NFC
#define IRQMASK_SDHC       L8_INTMASK_SDHC
#define IRQMASK_USB0_OHCI  L8_INTMASK_USB0_OHCI
#define IRQMASK_USB0_EHCI  L8_INTMASK_USB0_EHCI
#define IRQMASK_USB1_OHCI  L8_INTMASK_USB1_OHCI
#define IRQMASK_USB1_EHCI  L8_INTMASK_USB1_EHCI
#define IRQMASK_DVBCI      L8_INTMASK_DVBCI
#define IRQMASK_FEMAC      L8_INTMASK_FEMAC
#define IRQMASK_SP         L8_INTMASK_SP
#define IRQMASK_SC         L8_INTMASK_SC
#define IRQMASK_APB_TOUT_CTRL     L8_INTMASK_APB_TOUT_CTRL
#define IRQMASK_SLIKROAD0  L8_INTMASK_SLIKROAD0
#define IRQMASK_SLIKROAD1  L8_INTMASK_SLIKROAD1
#define IRQMASK_SLIKROAD2  L8_INTMASK_SLIKROAD2
#define IRQMASK_FRC        L8_INTMASK_FRC
#define IRQMASK_VENC0      L8_INTMASK_VENC0
#define IRQMASK_VENC1      L8_INTMASK_VENC1
#define IRQMASK_VDEC       L8_INTMASK_VDEC
#define IRQMASK_GPU_GP     L8_INTMASK_GPU_GP
#define IRQMASK_GPU_MMUGP  L8_INTMASK_GPU_MMUGP
#define IRQMASK_GPU_PP0    L8_INTMASK_GPU_PP0
#define IRQMASK_GPU_MMUPP0 L8_INTMASK_GPU_MMUPP0
#define IRQMASK_AUD0       L8_INTMASK_AUD0
#define IRQMASK_AUD1       L8_INTMASK_AUD1
#define IRQMASK_DE         L8_INTMASK_DE
#define IRQMASK_DE_IPC     L8_INTMASK_DE_IPC
#define IRQMASK_SDEC       L8_INTMASK_SDEC
#define IRQMASK_DVR        L8_INTMASK_DVR
#define IRQMASK_VP_IRIS    L8_INTMASK_VP_IRIS
#define IRQMASK_VP_HDMI    L8_INTMASK_VP_HDMI
#define IRQMASK_VP_3CH     L8_INTMASK_VP_3CH
#define IRQMASK_AAD        L8_INTMASK_AAD
#define IRQMASK_GFX        L8_INTMASK_GFX
#define IRQMASK_SLT        L8_INTMASK_SLT
#define IRQMASK_EXT0       L8_INTMASK_EXT0
#define IRQMASK_EXT1       L8_INTMASK_EXT1
#define IRQMASK_EXT2       L8_INTMASK_EXT2
#define IRQMASK_EXT3       L8_INTMASK_EXT3
#endif

#endif /* __ASM_ARCH_IRQS_H__ */
