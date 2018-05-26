/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  DVB-CI Command Interface Physical Layer
 *
 *  author		Hyunho Kim (hyunho747.kim@lge.com)
 *  version	0.6
 *  date		2014.06.20
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#ifndef	_CI_IO_DMA_H_
#define	_CI_IO_DMA_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/types.h>

#include "ci_cfg.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#if	defined(CI_USE_DMAC)

#ifndef BOOL
#define BOOL	unsigned char
#endif

#define AHB_SRAM_ADDR		0xFFF00000
#define AHB_SRAM_SIZE		0x4000		//16KB

/*
 * H15 SRAM (2014.08.08)
 * 0xFFF0_0000 ~ 0xFFF0_0FFF (4KB) : TrustZone
 * 0xFFF0_1000 ~ 0xFFF0_1FFF (4KB) : W/A
 * 0xFFF0_2000 ~ 0xFFF0_3FFF (8KB) : Kernel
 */
#define AHB_SRAM_CI_BASE	(AHB_SRAM_ADDR + 0x1000)	//2014.08.08
//#define AHB_SRAM_CI_BASE	(AHB_SRAM_ADDR + 0x2000)	//2014.07.30
#define AHB_SRAM_CI_SIZE	(0x100)

#define AMBA_DMA_LLI_ADDR	0xFFF03000
#define AMBA_DMA_LLI_SIZE	0x1000

#define AMBA_DMA_REG_ADDR	0xFF200000
#define AMBA_DMA_REG_SIZE	0x1000

#define SWAP_LONG(x) \
		((UINT32)( \
		(((UINT32)(x) & (UINT32)0x000000ffUL) << 24) | \
		(((UINT32)(x) & (UINT32)0x0000ff00UL) <<  8) | \
		(((UINT32)(x) & (UINT32)0x00ff0000UL) >>  8) | \
		(((UINT32)(x) & (UINT32)0xff000000UL) >> 24) ))

#define SWAP_WORD(x) \
		((UINT16)( \
		(((UINT16)(x) & (UINT16)0x00ff) <<  8) | \
		(((UINT16)(x) & (UINT16)0xff00) >>  8) ))

#define DMAC_MAX_CHANNEL	8

#define CI_DMAC_CHANNEL		7
#if 1
#define CI_DMAC_REG_R_CH	CI_DMAC_CHANNEL
#define CI_DMAC_REG_W_CH	CI_DMAC_CHANNEL
#define CI_DMAC_CAM_R_CH	CI_DMAC_CHANNEL
#define CI_DMAC_CAM_W_CH	CI_DMAC_CHANNEL
#else
#define CI_DMAC_REG_R_CH	4
#define CI_DMAC_REG_W_CH	5
#define CI_DMAC_CAM_R_CH	6
#define CI_DMAC_CAM_W_CH	7
#endif

typedef struct {
	UINT32	src;
	UINT32	dest;
	UINT32	next;
	UINT32	cr;
} LLI_T;

typedef enum {
	DMAC_FLOW_M2M_DMA,
	DMAC_FLOW_M2P_DMA,
	DMAC_FLOW_P2M_DMA,
	DMAC_FLOW_P2P_DMA,
	DMAC_FLOW_P2P_D,
	DMAC_FLOW_M2P_P,
	DMAC_FLOW_P2M_P,
	DMAC_FLOW_P2P_S
} DMAC_FLOW_T;

typedef enum {
	DMAC_BURST_SIZE_1,
	DMAC_BURST_SIZE_4,
	DMAC_BURST_SIZE_8,
	DMAC_BURST_SIZE_16,
	DMAC_BURST_SIZE_32,
	DMAC_BURST_SIZE_64,
	DMAC_BURST_SIZE_128,
	DMAC_BURST_SIZE_256,
} BURST_SIZE_T;

typedef enum {
	DMAC_BUS_WIDTH_8BIT = 0,
	DMAC_BUS_WIDTH_16BIT,
	DMAC_BUS_WIDTH_32BIT,
} BUS_WIDTH_T;

typedef struct {
	UINT8			ChNum;
	DMAC_FLOW_T		Flow;
	UINT16			TransferSize;
	UINT8			DestPeri;
	UINT8			SrcPeri;
	SINT16			RepeatNum;
	BOOL			DI;
	BOOL			SI;
	BOOL			D;
	BOOL			S;
	UINT32			Src;
	UINT32			Dest;
	UINT32			LLI;
	BUS_WIDTH_T	SrcWidth;
	BUS_WIDTH_T	DestWidth;
	BURST_SIZE_T	DBSize;
	BURST_SIZE_T	SBSize;
} DMAC_TRANSACTION_T;


typedef struct {
	UINT32		SrcAddr;
	UINT32		DestAddr;
	UINT32		LLI;
	UINT32		Control;
	UINT32		Configuration;
	UINT32		Res[3];
} DMAC_CH_T;

typedef struct {
	UINT32		IntStaus;
	UINT32		IntTCStatus;
	UINT32		IntTCClear;
	UINT32		IntErrorStatus;
	UINT32		IntErrClr;
	UINT32		RawIntTCStaus;
	UINT32		RawIntErrorStatus;
	UINT32		EnbldChns;
	UINT32		SoftBReq;
	UINT32		SoftSReq;
	UINT32		SoftLBReq;
	UINT32		SoftLSReq;
	UINT32		Configuration;
	UINT32		Sync;
	UINT32		Reserved[0x32];
	DMAC_CH_T	Ch[DMAC_MAX_CHANNEL];
} DMAC_REG_T;

typedef struct {

	DMAC_TRANSACTION_T	Ch[DMAC_MAX_CHANNEL];
} DMAC_INFO_T;

enum {
	DMAC_CH_0 = 0,
	DMAC_CH_1,
	DMAC_CH_2,
	DMAC_CH_3,
	DMAC_CH_4,
	DMAC_CH_5,
	DMAC_CH_6,
	DMAC_CH_7
};


#define DMAC_REQ_SSP2_RX		11
#define DMAC_REQ_SSP2_TX		10
#define DMAC_REQ_SSP1_RX		9
#define DMAC_REQ_SSP1_TX		8
#define DMAC_REQ_SSP0_RX		7
#define DMAC_REQ_SSP0_TX		6
#define DMAC_REQ_UART2_TX		5
#define DMAC_REQ_UART2_RX		4
#define DMAC_REQ_UART1_TX		3
#define DMAC_REQ_UART1_RX		2
#define DMAC_REQ_UART0_TX		1
#define DMAC_REQ_UART0_RX		0
#define DMAC_REQ_MEMORY			0

#define DMAC_APB_MASTER1					0
#define DMAC_APB_MASTER2					1

#define DMAC_LITTLE_ENDIAN_MODE				0x0
#define DMAC_BIG_ENDIAN_MODE				0x1
#define DMAC_MASTER2_ENDIAN					(DMAC_LITTLE_ENDIAN_MODE<<2)
#define DMAC_MASTER1_ENDIAN					(DMAC_LITTLE_ENDIAN_MODE<<1)
#define DMAC_CONF_ENABLE					0x00000001

#define DMAC_CH_LLI_MASK					0xFFFFFFFC

#define DMAC_CTRL_TC_IE						0x80000000
#define DMAC_CTRL_PROTECTION_MASK			0x70000000
#if 1
#define DMAC_CTRL_PROTECTION_PRIVILEGE		0x10000000
#define DMAC_CTRL_PROTECTION_BUFFERABLE		0x20000000
#define DMAC_CTRL_PROTECTION_CACHEABLE		0x40000000
#else
#define DMAC_CTRL_PROTECTION_PRIVILEGE		0x40000000
#define DMAC_CTRL_PROTECTION_BUFFERABLE		0x20000000
#define DMAC_CTRL_PROTECTION_CACHEABLE		0x10000000
#endif
#define DMAC_CTRL_DI						0x08000000
#define DMAC_CTRL_DI_SHIFT					27
#define DMAC_CTRL_SI						0x04000000
#define DMAC_CTRL_SI_SHIFT					26
#define DMAC_CTRL_D							0x02000000
#define DMAC_CTRL_D_SHIFT					25
#define DMAC_CTRL_S							0x01000000
#define DMAC_CTRL_S_SHIFT					24

#define DMAC_CTRL_DWIDTH_SHIFT				21
#define DMAC_CTRL_SWIDTH_SHIFT				18
#define DMAC_CTRL_DBSIZE_SHIFT				15
#define DMAC_CTRL_SBSIZE_SHIFT				12

#define DMAC_CH_CONF_HALT					0x00040000
#define DMAC_CH_CONF_ACTIVE					0x00020000
#define DMAC_CH_CONF_LOCK					0x00010000
#define DMAC_CH_CONF_ITC					0x00008000
#define DMAC_CH_CONF_IE						0x00004000

#define DMAC_CH_CONF_FLOW_CTRL_MASK			0x00003800
#define DMAC_CH_CONF_FLOW_CTRL_SHIFT		11
#define DMAC_CH_CONF_FLOW_CTRL_P2P_S		(DMAC_FLOW_P2P_S<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_P2M_P		(DMAC_FLOW_P2M_P<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_M2P_P		(DMAC_FLOW_M2P_P<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_P2P_D		(DMAC_FLOW_P2P_D<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_P2P_DMA		(DMAC_FLOW_P2P_DMA<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_P2M_DMA		(DMAC_FLOW_P2M_DMA<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_M2P_DMA		(DMAC_FLOW_M2P_DMA<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)
#define DMAC_CH_CONF_FLOW_CTRL_M2M_DMA		(DMAC_FLOW_M2M_DMA<<DMAC_CH_CONF_FLOW_CTRL_SHIFT)

#define DMAC_CH_CONF_DEST_PERI_MASK			(0xF<<DMAC_CH_CONF_DEST_PERI_SHIFT)
#define DMAC_CH_CONF_DEST_PERI_SHIFT		6
#define DMAC_CH_CONF_SRC_PERI_MASK			(0xF<<DMAC_CH_CONF_SRC_PERI_SHIFT)
#define DMAC_CH_CONF_SRC_PERI_SHIFT			1

#define DMAC_CH_CONF_ENABLE					0x00000001

#define IS_DMAC_ENABLED()					(pDMAC->Configuration & DMAC_CONF_ENABLE)
#define IS_DMAC_CH_ENABLED(Ch)				((pDMAC->EnbldChns & (0x1<<Ch)))

#define M_LLI_LOOP_MAX			3
#define M_LLI_BASE				0xFFF03000
//#define DEBUG_TIMING

#define LM_LLI
#define GM_LLI

#define DMAC_INTR_STATUS 		0xff200000
#define DMAC_INTR_TC_STATUS 	0xff200004
#define DMAC_INTR_TC_CLEAR 		0xff200008

extern int ci_dma_test(void);

extern UINT16 ci_dma_reg_readw(volatile void *addr);
extern void ci_dma_reg_writew(UINT16 val16, volatile void *addr);
extern UINT8 ci_dma_cam_readb(volatile void *addr);
extern UINT16 ci_dma_cam_readw(volatile void *addr);
extern UINT32 ci_dma_cam_readl(volatile void *addr);
extern void ci_dma_cam_writeb(UINT8 val8, volatile void *addr);
extern void ci_dma_cam_writew(UINT16 val16, volatile void *addr);
extern void ci_dma_cam_writel(UINT32 val32, volatile void *addr);

extern void ci_dma_cam_readb_multi(volatile void *addr, volatile void *buf, int size);
extern void ci_dma_cam_writeb_multi(volatile void *addr, volatile void *buf, int size);

#endif	/* CI_USE_DMAC */

extern int ci_dma_init(void);
extern void ci_dma_uninit(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_IO_DMA_H_ */

