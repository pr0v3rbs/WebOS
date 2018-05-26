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
 *  DVB-CI Command Interface Physical Layer - DMA
 *
 *  author		Hyunho Kim (hyunho747.kim@lge.com)
 *  version	0.6
 *  date		2014.06.20
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

#if	defined(CI_USE_DMAC)

/*=============================================================================+
| Include                                                                      |
+=============================================================================*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/delay.h>	// for usleep_range, jinhwan.bae 20131018
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/memory.h>
#include "ci_defs.h"
#include "os_util.h"

#include "ci_kapi.h"

#include "ci_drv.h"
#include "ci_io.h"
#include "ci_io_dma.h"
#include "ci_coredrv.h"

/*=============================================================================+
| Local Symbol Definition                                                      |
+=============================================================================*/

/*=============================================================================+
| Global Variable Declaration                                                  |
+=============================================================================*/

#ifdef __arm__
#define	CI_DMAC_ASM

#define CI_DMAC_REG_ASM()	\
		register unsigned int a0 __asm__("r2");	\
		register unsigned int a1 __asm__("r3");	\
		register unsigned int a2 __asm__("r4");	\
		register unsigned int a3 __asm__("r5");	\
		register unsigned int a4 __asm__("r6")

#define CI_DMAC_REG_ASM_SET(_pCh, _src, _dst, _con, _cfg)	\
		a0 = (_src);												\
		a1 = (_dst);												\
		a2 = 0;														\
		a3 = (_con);												\
		a4 = (_cfg);												\
		__asm__ __volatile__("stmia	%0!, {%1, %2, %3, %4}\n\t" 		\
							"str	%5, [%0]"						\
			: 														\
			: "r" (_pCh), "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4))
#endif

//#define CI_DMAC_REG_PRINT
//#define CI_DMAC_CAM_PRINT
//#define CI_DMAC_USE_LLI
//#define CI_DMAC_ADDR_CHK

//#define CI_MEM_IN_DRAM

//#define CI_DMA_BARRIER()	mb()
#define CI_DMA_BARRIER()

extern unsigned int	gCIChipRev;
extern unsigned int	gCIRegBase;
extern unsigned int	gCIRegSize;
extern unsigned int	gCIModBase;
extern unsigned int	gCIModSize;

#define CI_DMAC_REG_R_CH	CI_DMAC_CHANNEL
#define CI_DMAC_REG_W_CH	CI_DMAC_CHANNEL
#define CI_DMAC_CAM_R_CH	CI_DMAC_CHANNEL
#define CI_DMAC_CAM_W_CH	CI_DMAC_CHANNEL

#define CI_DMAC_CON_BASE	(0x81000000)
#define CI_DMAC_CON_BYTE	(CI_DMAC_CON_BASE | 0x01)
#define CI_DMAC_CON_WORD	(CI_DMAC_CON_BASE |	0x01 |\
							(DMAC_BUS_WIDTH_16BIT<<DMAC_CTRL_DWIDTH_SHIFT) |	\
							(DMAC_BUS_WIDTH_16BIT<<DMAC_CTRL_SWIDTH_SHIFT))
#define CI_DMAC_CON_LONG	(CI_DMAC_CON_BASE |	0x01 |\
							(DMAC_BUS_WIDTH_32BIT<<DMAC_CTRL_DWIDTH_SHIFT) |	\
							(DMAC_BUS_WIDTH_32BIT<<DMAC_CTRL_SWIDTH_SHIFT))

#define CI_DMAC_CFG_BASE	(DMAC_CH_CONF_ENABLE | DMAC_CH_CONF_ITC)

volatile DMAC_CH_T *pDMAC_Ch_REG_R;
volatile DMAC_CH_T *pDMAC_Ch_REG_W;
volatile DMAC_CH_T *pDMAC_Ch_CAM_R;
volatile DMAC_CH_T *pDMAC_Ch_CAM_W;

//volatile DMAC_REG_T * pDMAC = (volatile DMAC_REG_T *)ARMBA_DMA_REG_ADDR;
volatile DMAC_REG_T *pDMAC = NULL;
#if defined(CI_DMAC_USE_LLI)
volatile LLI_T *pLLI = NULL;
#endif
volatile UINT8 *pMEM_Virt = NULL;
#if defined(CI_MEM_IN_DRAM)
volatile UINT8 *pMEM_Phys = NULL;
#else
#define pMEM_Phys	AHB_SRAM_CI_BASE
#endif

volatile DMAC_INFO_T DMACInfo;

static int ci_dmac_disable_ch( UINT8 ChNum, BOOL Force );

static int ci_ioremap_dmac(void)
{
#if defined(CI_MEM_IN_DRAM)
	if (pMEM_Virt == NULL) {
		dma_addr_t dma_addr;
		pMEM_Virt = dma_alloc_coherent(NULL, AHB_SRAM_CI_SIZE, &dma_addr, GFP_KERNEL);
		if ((pMEM_Virt == NULL) || !dma_addr) {
			CI_SOC_MSG_ERR("NULL pMEM_Virt\n");
			return -1;
		}
		pMEM_Phys = dma_addr;
		CI_SOC_MSG_DBG("pMEM_Virt : 0x%p\n", pMEM_Virt);
		CI_SOC_MSG_DBG("pMEM_Phys : 0x%p\n", pMEM_Phys);
	}
#else
	if (pMEM_Virt == NULL) {
		pMEM_Virt = (volatile UINT8 *)ioremap(pMEM_Phys, AHB_SRAM_CI_SIZE);
		if (pMEM_Virt == NULL) {
			CI_SOC_MSG_ERR("NULL pMEM_Virt\n");
			return -1;
		}
		CI_SOC_MSG_DBG("pMEM_Virt ioremap : 0x%p <- 0x%08x\n", pMEM_Virt, pMEM_Phys);
	}
#endif

	if (pDMAC == NULL) {
		pDMAC = (volatile DMAC_REG_T *)ioremap(AMBA_DMA_REG_ADDR, AMBA_DMA_REG_SIZE);
		if (pDMAC == NULL) {
			CI_SOC_MSG_ERR("NULL pDMAC\n");
			return -1;
		}
		CI_SOC_MSG_DBG("pDMAC ioremap : 0x%p\n", pDMAC);
	}

#if defined(CI_DMAC_USE_LLI)
	if (pLLI== NULL) {
		pLLI = (volatile LLI_T *)ioremap(AMBA_DMA_LLI_ADDR, AMBA_DMA_LLI_SIZE);
		if (pLLI == NULL) {
			CI_SOC_MSG_ERR("NULL pLLI\n");
			return -1;
		}
		CI_SOC_MSG_DBG("pLLI ioremap : 0x%p\n", pLLI);
	}
#endif
	return 0;
}

static void ci_iounmap_dmac(void)
{
#if defined(CI_DMAC_USE_LLI)
	if (pLLI != NULL) {
		iounmap(pLLI);
		pLLI = NULL;
	}
#endif

	if (pDMAC != NULL) {
		iounmap(pDMAC);
		pDMAC = NULL;
	}

#if defined(CI_MEM_IN_DRAM)
	if (pMEM_Virt != NULL) {
		kfree(pMEM_Virt);
		pMEM_Virt = NULL;
	}
#else
	if (pMEM_Virt != NULL) {
		iounmap(pMEM_Virt);
		pMEM_Virt = NULL;
	}
#endif
}

static void ci_enable_dmac(void)
{
	if (pDMAC == NULL) {
		return;
	}

	if (!IS_DMAC_ENABLED()) {
		pDMAC->Configuration = (DMAC_MASTER1_ENDIAN|DMAC_MASTER2_ENDIAN|DMAC_CONF_ENABLE);
	}
}

static void ci_disable_dmac(void)
{
	int i;

	if (pDMAC == NULL){
		return;
	}
#if defined(CI_DMAC_USE_LLI)
	if (pLLI == NULL){
		return;
	}
#endif

	if( pDMAC->EnbldChns & 0xFF ){
		for( i=0; i<DMAC_MAX_CHANNEL; i++ ){
			if( IS_DMAC_CH_ENABLED(i) ){
				/* Force disable DMA Channel */
				if( ci_dmac_disable_ch(i, 1)) {
					return;
				}
			}
		}
	}
	pDMAC->Configuration &= ~DMAC_CONF_ENABLE;
}

static int ci_dmac_enable_ch( UINT8 ChNum )
{
	if (pDMAC == NULL){
		return -1;
	}
#if defined(CI_DMAC_USE_LLI)
	if (pLLI == NULL){
		return -1;
	}
#endif

	/* Check if the channel is already enabled */
	if( !IS_DMAC_CH_ENABLED(ChNum) ){
		/* You MUST initialize the channel before you enable it. Additionally, you must
		   set the Enable bit of the DMAC before you enable any channels */
		pDMAC->Ch[ChNum].Configuration |= DMAC_CH_CONF_ENABLE;
	}

	return 0;
}

static int ci_dmac_disable_ch( UINT8 ChNum, BOOL Force )
{
	if (pDMAC == NULL){
		return -1;
	}
#if defined(CI_DMAC_USE_LLI)
	if (pLLI == NULL){
		return -1;
	}
#endif

	if( IS_DMAC_CH_ENABLED(ChNum) ){
		/* Enabled Channel Only */
		if( Force == TRUE ){
			/* NOTICE : You might lose FIFO data if force disable is done */
			pDMAC->Ch[ChNum].Configuration &= ~DMAC_CH_CONF_ENABLE;
		} else {
			/* Try to wait for consuming all FIFO data */
			/* 1. Set the Halt bit */
			pDMAC->Ch[ChNum].Configuration |= DMAC_CH_CONF_HALT;
			/* 2. Poll the Active bit until it reaches to 0 */
			while( pDMAC->Ch[ChNum].Configuration & DMAC_CH_CONF_ACTIVE ) ;
			/* 3. Clear Channel Enable bit */
			pDMAC->Ch[ChNum].Configuration &= ~DMAC_CH_CONF_ENABLE;
		}
	}
	return 0;
}

#if 0
static int ci_dmac_program_channel( UINT8 ChNum, UINT32 Src, UINT32 Dest, UINT32 LLI, UINT32 Ctrl, UINT32 Conf )
{
	if (pDMAC == NULL){
		return -1;
	}
#if defined(CI_DMAC_USE_LLI)
	if (pLLI == NULL){
		return -1;
	}
#endif

	/* Clear pending interrupts on the channel. The previous channel operation might left interrupts inacitve. */
//	pDMAC->IntTCClear |= (0x1<<ChNum);
	pDMAC->IntErrClr |= (0x1<<ChNum);

	/* Write source address */
	pDMAC->Ch[ChNum].SrcAddr = Src;

	/* Write dest address */
	pDMAC->Ch[ChNum].DestAddr = Dest;

	/* Write address of next Linked List Item Register */
	pDMAC->Ch[ChNum].LLI = LLI;

	/* Write Control Information */
	pDMAC->Ch[ChNum].Control = Ctrl;

	/* Write Channel Configuration Information. If the Enable bit is set, then the DMA Channel is automatically enabled. */
	pDMAC->Ch[ChNum].Configuration = Conf;
#if 0
	printk("Ctrl = 0x%08x\n", Ctrl);
	printk("Conf = 0x%08x\n", Conf);
#endif

	return 0;
//	return ci_dmac_enable_ch( ChNum );
}
#endif

int ci_dmac_set_transfer( DMAC_TRANSACTION_T *pReq , UINT32 Ctrl)
{
	UINT32 Conf;
	int ChNum = pReq->ChNum;

	if (pDMAC == NULL){
		return -1;
	}
#if defined(CI_DMAC_USE_LLI)
	if (pLLI == NULL){
		return -1;
	}
#endif

#if 0
	/* Determine the channel is inacitve */
	if( IS_DMAC_CH_ENABLED(pReq->ChNum) ){
		CI_SOC_MSG_ERR("dma channel %d enabled\n", pReq->ChNum);
		return -1;
	}
#endif

//	memcpy((void *)&DMACInfo.Ch[pReq->ChNum], (void *)pReq, sizeof(DMAC_TRANSACTION_T) );

#if 0
	Conf = DMAC_CH_CONF_ENABLE;
	Conf |= ((pReq->Flow&0x7)<<DMAC_CH_CONF_FLOW_CTRL_SHIFT);
	Conf |= ((pReq->DestPeri&0xF)<<DMAC_CH_CONF_DEST_PERI_SHIFT);
	Conf |= ((pReq->SrcPeri&0xF)<<DMAC_CH_CONF_SRC_PERI_SHIFT);
	Conf |= DMAC_CH_CONF_ITC;
#else
	Conf = DMAC_CH_CONF_ENABLE | DMAC_CH_CONF_ITC;
#endif

//	return ci_dmac_program_channel( pReq->ChNum, pReq->Src, pReq->Dest, pReq->LLI, Ctrl, Conf);

//	pDMAC->IntTCClear |= (0x1<<ChNum);
	pDMAC->IntErrClr |= (0x1<<ChNum);

	/* Write source address */
	pDMAC->Ch[ChNum].SrcAddr = pReq->Src;

	/* Write dest address */
	pDMAC->Ch[ChNum].DestAddr = pReq->Dest;

	/* Write address of next Linked List Item Register */
	pDMAC->Ch[ChNum].LLI = pReq->LLI;

	/* Write Control Information */
	pDMAC->Ch[ChNum].Control = Ctrl;

	/* Write Channel Configuration Information. If the Enable bit is set, then the DMA Channel is automatically enabled. */
	pDMAC->Ch[ChNum].Configuration = Conf;

	return 0;
}

int ci_dmac_wait(unsigned int ch)
{
	unsigned int result;
#if 0
	unsigned long start;

	start = timer_usec();
#endif

#if 1
	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << ch)) {
			pDMAC->IntTCClear |= (1<<ch);
			break;
		}
	}
#else
	while (1) {
		result = readl(DMAC_INTR_TC_STATUS);
		if (result & (1 << ch)) {
			writel(DMAC_INTR_TC_CLEAR, result);
			break;
		}
	}
#endif
#if 0
	while (1) {
		if (!IS_DMAC_CH_ENABLED(ch)) {
			break;
		}
	}
#endif
	return 0;
}

static inline int ci_dma_request_m2m(int channel,
									unsigned long src, unsigned long dst, uint32_t tsize,
									int width)
{
	DMAC_TRANSACTION_T *Req;
#if defined(CI_DMAC_USE_LLI)
	volatile LLI_T  *LLI;
#endif

#if defined(CI_DMAC_USE_LLI)
	UINT32 remainder = tsize;
	UINT32 max_tsize = 0xfff;
	//UINT32 max_tsize = 0x040;
	int i;
#endif
	UINT32 Ctrl;

//	Req = (DMAC_TRANSACTION_T *)&DMACInfo.Ch[CI_DMAC_CHANNEL];
	Req = (DMAC_TRANSACTION_T *)&DMACInfo.Ch[channel];

#if 0
	//printf("start set m ctrl reg\n");
	Req->ChNum = channel;
#if 0 /* dwkim */
	Req->DI = 0;
	Req->SI = 0;
#else
	Req->DI = 0;
	Req->SI = 0;
#endif
	Req->D = DMAC_APB_MASTER1;
	Req->S = DMAC_APB_MASTER2;

#if 1
	Req->SrcWidth = width;
	Req->DestWidth = width;
	Req->DBSize = DMAC_BURST_SIZE_1;
	Req->SBSize = DMAC_BURST_SIZE_1;
#else
	Req->SrcWidth = DMAC_BUS_WIDTH_32BIT;
	Req->DestWidth = DMAC_BUS_WIDTH_32BIT;
#if 1
	Req->DBSize = DMAC_BURST_SIZE_256;
	Req->SBSize = DMAC_BURST_SIZE_256;
#endif
#if 0
	Req->DBSize = DMAC_BURST_SIZE_1;
	Req->SBSize = DMAC_BURST_SIZE_1;
#endif
#endif
	Req->Flow = DMAC_FLOW_M2M_DMA;		/* memory to memory */
	Req->DestPeri = DMAC_REQ_MEMORY;
	Req->SrcPeri = DMAC_REQ_MEMORY;
	Req->RepeatNum = 1;

//	Ctrl = DMAC_CTRL_PROTECTION_PRIVILEGE|DMAC_CTRL_PROTECTION_BUFFERABLE;
//	Ctrl = DMAC_CTRL_PROTECTION_PRIVILEGE;
	Ctrl = 0;
	Ctrl |= ((Req->DI&0x1)<<DMAC_CTRL_DI_SHIFT);
	Ctrl |= ((Req->SI&0x1)<<DMAC_CTRL_SI_SHIFT);
	Ctrl |= ((Req->D&0x1)<<DMAC_CTRL_D_SHIFT);
	Ctrl |= ((Req->S&0x1)<<DMAC_CTRL_S_SHIFT);
	Ctrl |= ((Req->DestWidth&0x7)<<DMAC_CTRL_DWIDTH_SHIFT);
	Ctrl |= ((Req->SrcWidth&0x7)<<DMAC_CTRL_SWIDTH_SHIFT);
	Ctrl |= ((Req->DBSize&0x7)<<DMAC_CTRL_DBSIZE_SHIFT);
	Ctrl |= ((Req->SBSize&0x7)<<DMAC_CTRL_SBSIZE_SHIFT);
#else
	Req->SrcWidth = width;
	Req->DestWidth = width;

//	Ctrl = DMAC_CTRL_PROTECTION_PRIVILEGE|DMAC_CTRL_PROTECTION_BUFFERABLE;
	/*
	Ctrl = DMAC_CTRL_PROTECTION_PRIVILEGE;
	Ctrl |= (0x1<<DMAC_CTRL_S_SHIFT);
	*/
	Ctrl = (0x1<<DMAC_CTRL_S_SHIFT);
	Ctrl |= ((Req->DestWidth&0x7)<<DMAC_CTRL_DWIDTH_SHIFT);
	Ctrl |= ((Req->SrcWidth&0x7)<<DMAC_CTRL_SWIDTH_SHIFT);
#endif

#if !defined(CI_DMAC_USE_LLI)
	Req->Src  = src;
	Req->Dest = dst;

//	ci_dmac_set_transfer(Req, Ctrl | tsize | DMAC_CTRL_TC_IE);

	{
		UINT32 Conf;
		int ChNum = Req->ChNum;

		Conf = DMAC_CH_CONF_ENABLE | DMAC_CH_CONF_ITC;

	//	pDMAC->IntTCClear |= (0x1<<ChNum);
		pDMAC->IntErrClr |= (0x1<<ChNum);

		/* Write source address */
		pDMAC->Ch[ChNum].SrcAddr = Req->Src;

		/* Write dest address */
		pDMAC->Ch[ChNum].DestAddr = Req->Dest;

		pDMAC->Ch[channel].LLI = 0;

		/* Write Control Information */
		pDMAC->Ch[ChNum].Control = Ctrl | tsize | DMAC_CTRL_TC_IE;

		/* Write Channel Configuration Information. If the Enable bit is set, then the DMA Channel is automatically enabled. */
		pDMAC->Ch[ChNum].Configuration = Conf;
	}

//	ci_dmac_set_transfer(Req, Ctrl | tsize);
#else
	i = 0;

	LLI = (volatile LLI_T *)pLLI;
//	LLI = (volatile LLI_T *)(0xFFF03000);

	while (remainder > max_tsize)
	{
		UINT32 cur_tsize = max_tsize;
		UINT32 cur_ctrl = Ctrl;

		remainder -= max_tsize;

		cur_ctrl |= cur_tsize;
		LLI[i].src  = src;
		LLI[i].dest = dst;
		LLI[i].next = (u32)(u64)(&LLI[i+1]);
		LLI[i].cr   = cur_ctrl;

		src += cur_tsize*(1<<width);
		dst += cur_tsize*(1<<width);
		i++;
	}

	if (remainder) {
		UINT32 cur_tsize = remainder;
		UINT32 cur_ctrl = Ctrl;

		cur_ctrl |= cur_tsize | DMAC_CTRL_TC_IE;
		LLI[i].src  = src;
		LLI[i].dest = dst;
		LLI[i].next = 0;
		LLI[i].cr   = cur_ctrl;
	}
#if 1 // dwkim
	Req->Src  = LLI[0].src;
	Req->Dest = LLI[0].dest;
	Req->LLI  = LLI[0].next | Req->D;
	Req->TransferSize = LLI[0].cr & 0xfff; /* XXX: useless */
#endif
	ci_dmac_set_transfer(Req, LLI[0].cr);

#endif

	return 0;
}

static inline void ci_dma_request_m2m_word_reg_r(unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_REG_R;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_REG_R_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_REG_R_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_WORD, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_WORD;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_REG_R_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_REG_R_CH);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_word_reg_w(unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_REG_W;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_REG_W_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_REG_W_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_WORD, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_WORD;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_REG_W_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_REG_W_CH);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_byte_cam_r(unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_CAM_R;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_CAM_R_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_CAM_R_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_BYTE, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_BYTE;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_CAM_R_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_CAM_R_CH);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_byte_cam_w(unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_CAM_W;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_CAM_W_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_CAM_W_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_BYTE, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_BYTE;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_CAM_W_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_CAM_W_CH);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_word(int channel, unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = (volatile DMAC_CH_T *)&pDMAC->Ch[channel];
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<channel);
	pDMAC->IntErrClr |= (0x1<<channel);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_WORD, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_WORD;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << channel)) {
			pDMAC->IntTCClear |= (1<<channel);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_long(int channel, unsigned long src, unsigned long dst)
{
	volatile DMAC_CH_T *pDMAC_Ch = (volatile DMAC_CH_T *)&pDMAC->Ch[channel];
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<channel);
	pDMAC->IntErrClr |= (0x1<<channel);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_LONG, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_LONG;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << channel)) {
			pDMAC->IntTCClear |= (1<<channel);
			break;
		}
	}
}

#if defined(CI_USE_DMAC_MULTI)
static inline void ci_dma_request_m2m_byte_cam_r_multi(unsigned long src, unsigned long dst, int size)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_CAM_R;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_CAM_R_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_CAM_R_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_BASE | (0x1<<DMAC_CTRL_DI_SHIFT) | size, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_BASE | (0x1<<DMAC_CTRL_DI_SHIFT) | size;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_CAM_R_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_CAM_R_CH);
			break;
		}
	}
}

static inline void ci_dma_request_m2m_byte_cam_w_multi(unsigned long src, unsigned long dst, int size)
{
	volatile DMAC_CH_T *pDMAC_Ch = pDMAC_Ch_CAM_W;
#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM();
#endif
	unsigned int result;

//	pDMAC->IntTCClear |= (0x1<<CI_DMAC_CAM_W_CH);
	pDMAC->IntErrClr |= (0x1<<CI_DMAC_CAM_W_CH);

#if	defined(CI_DMAC_ASM)
	CI_DMAC_REG_ASM_SET(pDMAC_Ch, src, dst, CI_DMAC_CON_BASE | (0x1<<DMAC_CTRL_SI_SHIFT) | size, CI_DMAC_CFG_BASE);
#else
	pDMAC_Ch->SrcAddr = src;
	pDMAC_Ch->DestAddr = dst;
	pDMAC_Ch->Control = CI_DMAC_CON_BASE | (0x1<<DMAC_CTRL_SI_SHIFT) | size;
	pDMAC_Ch->Configuration = CI_DMAC_CFG_BASE;
#endif

	while (1) {
		result = pDMAC->IntTCStatus;
		if (result & (1 << CI_DMAC_CAM_W_CH)) {
			pDMAC->IntTCClear |= (1<<CI_DMAC_CAM_W_CH);
			break;
		}
	}
}
#endif

static int ci_dma_wait_m2m(int channel)
{
//	ci_dmac_wait(CI_DMAC_CHANNEL);
	ci_dmac_wait(channel);
	return 0;
}

int ci_dma_test_sram_copy_word(UINT16 val16)
{
	UINT16 retVal16 = 0;
	volatile unsigned long src, dst;
	volatile unsigned long dst_virt;

	((volatile UINT16 *)pMEM_Virt)[0] = val16;

	src = (unsigned long)pMEM_Phys;
	dst = ((unsigned long)pMEM_Phys) + 0x100;

	ci_dma_request_m2m(CI_DMAC_CHANNEL, src, dst, 1, DMAC_BUS_WIDTH_16BIT);
	ci_dma_wait_m2m(CI_DMAC_CHANNEL);

	dst_virt = ((unsigned long)pMEM_Virt) + 0x100;
	retVal16 = ((volatile UINT16 *)dst_virt)[0];
	if (val16 != retVal16) {
		CI_SOC_MSG_ERR("Error: src val[0x%04X] != dst val[0x%04X]\n",
						val16, retVal16);
		return -1;
	}
	return 0;
}

int ci_dma_test_sram_copy_long(UINT32 val32)
{
	UINT32 retVal32 = 0;
	volatile unsigned long src, dst;
	volatile unsigned long dst_virt;

	((volatile UINT32 *)pMEM_Virt)[0] = val32;

	src = (unsigned long)pMEM_Phys;
	dst = ((unsigned long)pMEM_Phys) + 0x100;

	ci_dma_request_m2m(CI_DMAC_CHANNEL, src, dst, 1, DMAC_BUS_WIDTH_32BIT);
	ci_dma_wait_m2m(CI_DMAC_CHANNEL);

	dst_virt = ((unsigned long)pMEM_Virt) + 0x100;
	retVal32 = ((volatile UINT32 *)dst_virt)[0];
	if (val32 != retVal32) {
		CI_SOC_MSG_ERR("Error: src val[0x%08X] != dst val[0x%08X]\n",
						val32, retVal32);
		return -1;
	}
	return 0;
}

int ci_dma_test(void)
{
	int i;

	for (i=0 ; i<10000 ; i++) {
		printk("[%d]\n", i);
	//	if (ci_dma_test_sram_copy_word(i)) {
		if (ci_dma_test_sram_copy_long(i)) {
			return -1;
		}
	}
	CI_SOC_MSG_DBG("SRAM Copy Test Done\n");
	return 0;
}

static int ci_dma_ch_init(void)
{
	DMAC_TRANSACTION_T *Req;
	int channel;

	pDMAC_Ch_REG_R = (volatile DMAC_CH_T *)&pDMAC->Ch[CI_DMAC_REG_R_CH];
	pDMAC_Ch_REG_W = (volatile DMAC_CH_T *)&pDMAC->Ch[CI_DMAC_REG_W_CH];
	pDMAC_Ch_CAM_R = (volatile DMAC_CH_T *)&pDMAC->Ch[CI_DMAC_CAM_R_CH];
	pDMAC_Ch_CAM_W = (volatile DMAC_CH_T *)&pDMAC->Ch[CI_DMAC_CAM_W_CH];

	for (channel=0 ; channel<DMAC_MAX_CHANNEL ; channel++) {
		Req = (DMAC_TRANSACTION_T *)&DMACInfo.Ch[channel];

		Req->ChNum = channel;
		Req->DI = 0;
		Req->SI = 0;
		Req->D = DMAC_APB_MASTER1;
		Req->S = DMAC_APB_MASTER2;
		Req->LLI = 0;
		Req->DBSize = DMAC_BURST_SIZE_1;
		Req->SBSize = DMAC_BURST_SIZE_1;
		Req->Flow = DMAC_FLOW_M2M_DMA;		/* memory to memory */
		Req->DestPeri = DMAC_REQ_MEMORY;
		Req->SrcPeri = DMAC_REQ_MEMORY;
		Req->RepeatNum = 1;

		pDMAC->Ch[channel].LLI = 0;

		pDMAC->IntTCClear |= (0x1<<channel);
		pDMAC->IntErrClr |= (0x1<<channel);
	}
	return 0;
}

int ci_dma_init(void)
{
	if (lx_chip() != LX_CHIP_H15) {
		return 0;
	}
	if ((lx_chip_rev() != LX_CHIP_REV(H15, A0)) &&
		(lx_chip_rev() != LX_CHIP_REV(H15, A1)))
	{
		return 0;
	}

	if (ci_ioremap_dmac()) {
		return -1;
	}
	ci_enable_dmac();

	ci_dma_ch_init();

//	ci_dma_test();

#if defined(CI_DMAC_ASM)
	CI_SOC_MSG_NOTI("CI DMA Initialized(with ASM)\n");
#elif defined(CI_USE_DMAC_MULTI)
	CI_SOC_MSG_NOTI("CI DMA Initialized(with Multi-IO)\n");
#else
	CI_SOC_MSG_NOTI("CI DMA Initialized\n");
#endif

	return 0;
}

void ci_dma_uninit(void)
{
	if (lx_chip() != LX_CHIP_H15) {
		return;
	}
	if ((lx_chip_rev() != LX_CHIP_REV(H15, A0)) &&
		(lx_chip_rev() != LX_CHIP_REV(H15, A1)))
	{
		return;
	}

	ci_disable_dmac();
	ci_iounmap_dmac();
	CI_SOC_MSG_NOTI("CI DMA Uninitialized\n");
}

/*
 *
 */
//#define ci_dma_reg_base		gpstCIReg
#define ci_dma_reg_base		CiModBaseAddr[MOD_A]
#define ci_dma_cam_base		CiModBaseAddr[MOD_A]

UINT16 ci_dma_reg_readw(volatile void *addr)
{
	UINT16 retVal16 = 0;
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < (unsigned long)ci_dma_reg_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI reg[0x%08x]\n", addr, (UINT32)ci_dma_reg_base);
		return 0;
	}
#endif

	offset = ((unsigned long)addr - (unsigned long)ci_dma_reg_base);
	src = ((unsigned long)gCIRegBase) + offset;
	dst = (unsigned long)pMEM_Phys;

	ci_dma_request_m2m_word_reg_r(src, dst);

	retVal16 = ((volatile UINT16 *)pMEM_Virt)[0];

#ifdef CI_DMAC_REG_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("read val = 0x%04x\n", retVal16);
#endif

	CI_DMA_BARRIER();

	return retVal16;
}

void ci_dma_reg_writew(UINT16 val16, volatile void *addr)
{
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < (unsigned long)ci_dma_reg_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI reg[0x%08x]\n", addr, (UINT32)ci_dma_reg_base);
		return;
	}
#endif

	((volatile UINT16 *)pMEM_Virt)[0] = val16;

	offset = ((unsigned long)addr - (unsigned long)ci_dma_reg_base);
	src = (unsigned long)pMEM_Phys;
	dst = ((unsigned long)gCIRegBase) + offset;

	ci_dma_request_m2m_word_reg_w(src, dst);

#ifdef CI_DMAC_REG_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("write val = 0x%04x\n", val16);
#endif

	CI_DMA_BARRIER();
}

UINT8 ci_dma_cam_readb(volatile void *addr)
{
	UINT8 retVal8 = 0;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return 0;
	}
#endif

	ci_dma_request_m2m_byte_cam_r((unsigned long)gCIModBase + ((unsigned long)addr - ci_dma_cam_base),
								(unsigned long)pMEM_Phys);

	retVal8 = ((volatile UINT8 *)pMEM_Virt)[0];

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("read val = 0x%02x\n", retVal8);
#endif

	CI_DMA_BARRIER();

	return retVal8;
}

UINT16 ci_dma_cam_readw(volatile void *addr)
{
	UINT16 retVal16 = 0;
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return 0;
	}
#endif

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)gCIModBase + offset;
	dst = (unsigned long)pMEM_Phys;

	ci_dma_request_m2m_word(CI_DMAC_CAM_R_CH, src, dst);
//	ci_dma_wait_m2m(CI_DMAC_CAM_R_CH);

	retVal16 = ((volatile UINT16 *)pMEM_Virt)[0];

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("read val = 0x%04x\n", retVal16);
#endif

	CI_DMA_BARRIER();

	return retVal16;
}

UINT32 ci_dma_cam_readl(volatile void *addr)
{
	UINT32 retVal32 = 0;
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return 0;
	}
#endif

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)gCIModBase + offset;
	dst = (unsigned long)pMEM_Phys;

	ci_dma_request_m2m_long(CI_DMAC_CAM_R_CH, src, dst);
//	ci_dma_wait_m2m(CI_DMAC_CAM_R_CH);

	retVal32 = ((volatile UINT32 *)pMEM_Virt)[0];

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("read val = 0x%08x\n", retVal32);
#endif

	CI_DMA_BARRIER();

	return retVal32;
}

void ci_dma_cam_writeb(UINT8 val8, volatile void *addr)
{
#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return;
	}
#endif

	((volatile UINT8 *)pMEM_Virt)[0] = val8;

	ci_dma_request_m2m_byte_cam_w((unsigned long)pMEM_Phys,
								(unsigned long)gCIModBase + ((unsigned long)addr - ci_dma_cam_base));

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("write val = 0x%02x\n", val8);
#endif

	CI_DMA_BARRIER();
}

void ci_dma_cam_writew(UINT16 val16, volatile void *addr)
{
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return;
	}
#endif

	((volatile UINT16 *)pMEM_Virt)[0] = val16;

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)pMEM_Phys;
	dst = (unsigned long)gCIModBase + offset;

	ci_dma_request_m2m_word(CI_DMAC_CAM_W_CH, src, dst);
//	ci_dma_wait_m2m(CI_DMAC_CAM_W_CH);

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("write val = 0x%04x\n", val16);
#endif

	CI_DMA_BARRIER();
}

void ci_dma_cam_writel(UINT32 val32, volatile void *addr)
{
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return;
	}
#endif

	((volatile UINT32 *)pMEM_Virt)[0] = val32;

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)pMEM_Phys;
	dst = (unsigned long)gCIModBase + offset;

	ci_dma_request_m2m_long(CI_DMAC_CAM_W_CH, src, dst);
//	ci_dma_wait_m2m(CI_DMAC_CAM_W_CH);

#ifdef CI_DMAC_CAM_PRINT
	CI_SOC_MSG_DBG("addr[0x%p]\n", addr);
	CI_SOC_MSG_DBG("src[0x%08x] -> dst[0x%08x]\n", (UINT32)src, (UINT32)dst);
	CI_SOC_MSG_DBG("write val = 0x%08x\n", val32);
#endif

	CI_DMA_BARRIER();
}

#if defined(CI_USE_DMAC_MULTI)
void ci_dma_cam_readb_multi(volatile void *addr, volatile void *buf, int size)
{
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return 0;
	}
#endif

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)gCIModBase + offset;
	dst = (unsigned long)pMEM_Phys;

	ci_dma_request_m2m_byte_cam_r_multi(src, dst, size);

	memcpy((void *)buf, (void *)pMEM_Virt, size);

	CI_DMA_BARRIER();
}

void ci_dma_cam_writeb_multi(volatile void *addr, volatile void *buf, int size)
{
	unsigned long offset;
	volatile unsigned long src, dst;

#if defined(CI_DMAC_ADDR_CHK)
	if ((unsigned long)addr < ci_dma_cam_base) {
		CI_SOC_MSG_ERR("addr[0x%p] < CI CAM[0x%08x]\n", addr, (UINT32)ci_dma_cam_base);
		return;
	}
#endif

	offset = ((unsigned long)addr - ci_dma_cam_base);
	src = (unsigned long)pMEM_Phys;
	dst = (unsigned long)gCIModBase + offset;

	memcpy((void *)pMEM_Virt, (void *)buf, size);

	ci_dma_request_m2m_byte_cam_w_multi(src, dst, size);

	CI_DMA_BARRIER();
}
#endif

#else

int ci_dma_init(void)
{
	return 0;
}

void ci_dma_uninit(void)
{

}

#endif	//CI_USE_DMAC

