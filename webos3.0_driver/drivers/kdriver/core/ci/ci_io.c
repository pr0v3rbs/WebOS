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
 *  author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version	0.6
 *  date		2010.02.22
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */


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

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/memory.h>

#include "ci_defs.h"
#include "os_util.h"

#include "ci_kapi.h"
#include "ci_drv.h"
#include "ci_coredrv.h"
#include "ci_io.h"
#include "ci_io_dma.h"

/*=============================================================================+
| Local Symbol Definition                                                      |
+=============================================================================*/

/*=============================================================================+
| Global Variable Declaration                                                  |
+=============================================================================*/

UINT16	CiNegoBufSize[MAX_MOD_NUM] = {	0 };

extern volatile UINT32 guwDetectCard;
extern volatile S_CI_REG_T* gpstCIReg;
extern volatile UINT32 guwIsPowerRestart;

extern UINT32 gCI_KDRV_MaxCount_R;
extern UINT32 gCI_KDRV_MaxCount_W;
extern UINT32 gCI_KDRV_Delay[CI_D_ENUM_MAX];	// jinhwan.bae for delay

extern UINT64 gCI_HW_IO_Write_Count;
extern UINT64 gCI_HW_IO_Write_Count_FR_Retry;

UINT32	CiOptChkModeSimple[MAX_MOD_NUM] = {	0, };

typedef enum
{
	CI_ERR_CHK_MODE_FULL		= 0,
	CI_ERR_CHK_MODE_SIMPLE,
	CI_ERR_CHK_MODE_MAX
} CI_ERR_CHK_MODE_T;

/*=============================================================================+
| Function Definition                                                          |
+=============================================================================*/

#define MAXIM_RETRY_COUNT ( 600 )

static UINT8 *gIOBuf = NULL;

/*
 *
 */
struct ci_io_ops g_ci_io_ops;

static void _ci_rmb_0(void)
{

}
static void _ci_wmb_0(void)
{

}
static void _ci_rmb_2(void)
{
	rmb();
}
static void _ci_wmb_2(void)
{
	wmb();
}
static void _ci_rmb_3(void)
{
	mb();
}
static void _ci_wmb_3(void)
{
	mb();
}
static void _ci_rmb_4(void)
{
	mb();
	udelay(10);
}
static void _ci_wmb_4(void)
{
	mb();
	udelay(10);
}

static UINT16 _ci_readw_0(volatile void *addr)
{
	UINT16 val16;
	val16 = *(volatile UINT16 *)(addr);
	return val16;
}
static void _ci_writew_0(UINT16 val16, volatile void *addr)
{
	*(volatile UINT16 *)(addr) = val16;
}
static UINT8 _ci_cam_readb_0(volatile void *addr)
{
	UINT8 val8;
	val8 = *((volatile UINT8 *)(addr));
	return val8;
}
static UINT16 _ci_cam_readw_0(volatile void *addr)
{
	UINT16 val16;
	val16 = *((volatile u16 *)(addr));
	return val16;
}
static UINT32 _ci_cam_readl_0(volatile void *addr)
{
	UINT32 val32;
	val32 = *((volatile u32 *)(addr));
	return val32;
}
static void _ci_cam_writeb_0(UINT8 val8, volatile void *addr)
{
	*(volatile UINT8 *)(addr) = val8;
}
static void _ci_cam_writew_0(UINT16 val16, volatile void *addr)
{
	*(volatile UINT16 *)(addr) = val16;
}
static void _ci_cam_writel_0(UINT32 val32, volatile void *addr)
{
	*(volatile UINT32 *)(addr) = val32;
}

static UINT16 _ci_readw_2(volatile void *addr)
{
	return readw(addr);
}
static void _ci_writew_2(UINT16 val16, volatile void *addr)
{
	writew(val16, addr);
}
static UINT8 _ci_cam_readb_2(volatile void *addr)
{
	return readb(addr);
}
static UINT16 _ci_cam_readw_2(volatile void *addr)
{
	return readw(addr);
}
static UINT32 _ci_cam_readl_2(volatile void *addr)
{
	return readw(addr);
}
static void _ci_cam_writeb_2(UINT8 val8, volatile void *addr)
{
	writew(val8, addr);
}
static void _ci_cam_writew_2(UINT16 val16, volatile void *addr)
{
	writew(val16, addr);
}
static void _ci_cam_writel_2(UINT32 val32, volatile void *addr)
{
	writew(val32, addr);
}

static UINT16 _ci_readw_3(volatile void *addr)
{
	UINT16 val16;
	val16 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val16;
}
static void _ci_writew_3(UINT16 val16, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val16, addr);
}
static UINT8 _ci_cam_readb_3(volatile void *addr)
{
	UINT8 val8;
	val8 = readb(addr);
	(g_ci_io_ops.rmb)();
	return val8;
}
static UINT16 _ci_cam_readw_3(volatile void *addr)
{
	UINT8 val16;
	val16 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val16;
}
static UINT32 _ci_cam_readl_3(volatile void *addr)
{
	UINT32 val32;
	val32 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val32;
}
static void _ci_cam_writeb_3(UINT8 val8, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val8, addr);
}
static void _ci_cam_writew_3(UINT16 val16, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val16, addr);
}
static void _ci_cam_writel_3(UINT32 val32, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val32, addr);
}

static UINT16 _ci_readw_4(volatile void *addr)
{
	UINT16 val16;
	(g_ci_io_ops.rmb)();
	val16 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val16;
}
static void _ci_writew_4(UINT16 val16, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val16, addr);
	(g_ci_io_ops.wmb)();
}
static UINT8 _ci_cam_readb_4(volatile void *addr)
{
	UINT8 val8;
	(g_ci_io_ops.rmb)();
	val8 = readb(addr);
	(g_ci_io_ops.rmb)();
	return val8;
}
static UINT16 _ci_cam_readw_4(volatile void *addr)
{
	UINT8 val16;
	(g_ci_io_ops.rmb)();
	val16 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val16;
}
static UINT32 _ci_cam_readl_4(volatile void *addr)
{
	UINT32 val32;
	(g_ci_io_ops.rmb)();
	val32 = readw(addr);
	(g_ci_io_ops.rmb)();
	return val32;
}
static void _ci_cam_writeb_4(UINT8 val8, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val8, addr);
	(g_ci_io_ops.wmb)();
}
static void _ci_cam_writew_4(UINT16 val16, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val16, addr);
	(g_ci_io_ops.wmb)();
}
static void _ci_cam_writel_4(UINT32 val32, volatile void *addr)
{
	(g_ci_io_ops.wmb)();
	writew(val32, addr);
	(g_ci_io_ops.wmb)();
}

void HW_IO_SetChkMode(UINT32 chk_mode)
{
#if defined(CI_OPT_CHK_MODE)
	int i;

	chk_mode = (chk_mode)? CI_ERR_CHK_MODE_SIMPLE : CI_ERR_CHK_MODE_FULL;
	for (i=0 ; i<MAX_MOD_NUM ; i++) {
		CiOptChkModeSimple[i] = chk_mode;
	}
	CI_SOC_MSG_NOTI("Set check mode : %s\n",
				(chk_mode == CI_ERR_CHK_MODE_SIMPLE)? "Simple" : "Full");
#endif
}

UINT32 HW_IO_GetChkMode(void)
{
	UINT32 chk_mode = CI_ERR_CHK_MODE_FULL;
#if defined(CI_OPT_CHK_MODE)
	chk_mode = (CiOptChkModeSimple[0])? CI_ERR_CHK_MODE_SIMPLE : CI_ERR_CHK_MODE_FULL;

	CI_SOC_MSG_NOTI("Get check mode : %s\n",
				(chk_mode == CI_ERR_CHK_MODE_SIMPLE)? "Simple" : "Full");
#endif
	return chk_mode;
}

int HW_IO_Init(void)
{
	if (gIOBuf != NULL) {
		return HW_IO_OK;
	}

	HW_IO_SetChkMode(0);

	gIOBuf = OS_Malloc(MAX_POSS_BUF_SIZE);
	if (gIOBuf == NULL) {
		CI_SOC_MSG_ERR("Memory alloc for gIOBuf failed\n");
		return HW_IO_FAIL;
	}

	g_ci_io_ops.opt_barrier = 0;
	g_ci_io_ops.opt_rw_core = 0;
	if (lx_chip() == LX_CHIP_H15) {
		if ((lx_chip_rev() == LX_CHIP_REV(H15, A0)) ||
			(lx_chip_rev() == LX_CHIP_REV(H15, A1)))
		{
			g_ci_io_ops.opt_barrier = 0;
			g_ci_io_ops.opt_rw_core = 5;
		} else {
			g_ci_io_ops.opt_barrier = 3;
			g_ci_io_ops.opt_rw_core = 3;
		}
	}

	switch (g_ci_io_ops.opt_barrier) {
		case 4 :
			g_ci_io_ops.rmb = _ci_rmb_4;
			g_ci_io_ops.wmb = _ci_wmb_4;
			break;
		case 3 :
			g_ci_io_ops.rmb = _ci_rmb_3;
			g_ci_io_ops.wmb = _ci_wmb_3;
			break;
		case 2 :
			g_ci_io_ops.rmb = _ci_rmb_2;
			g_ci_io_ops.wmb = _ci_wmb_2;
			break;
		default :
			g_ci_io_ops.rmb = _ci_rmb_0;
			g_ci_io_ops.wmb = _ci_wmb_0;
			break;
	}

	switch (g_ci_io_ops.opt_rw_core) {
		case 5 :
		#if defined(CI_USE_DMAC)
			g_ci_io_ops.readw 		= ci_dma_reg_readw;
			g_ci_io_ops.writew		= ci_dma_reg_writew;
			g_ci_io_ops.cam_readb	= ci_dma_cam_readb;
			g_ci_io_ops.cam_readw	= ci_dma_cam_readw;
			g_ci_io_ops.cam_readl	= ci_dma_cam_readl;
			g_ci_io_ops.cam_writeb	= ci_dma_cam_writeb;
			g_ci_io_ops.cam_writew	= ci_dma_cam_writew;
			g_ci_io_ops.cam_writel	= ci_dma_cam_writel;
		#else
			g_ci_io_ops.readw 		= _ci_readw_2;
			g_ci_io_ops.writew		= _ci_writew_2;
			g_ci_io_ops.cam_readb	= _ci_cam_readb_2;
			g_ci_io_ops.cam_readw	= _ci_cam_readw_2;
			g_ci_io_ops.cam_readl	= _ci_cam_readl_2;
			g_ci_io_ops.cam_writeb	= _ci_cam_writeb_2;
			g_ci_io_ops.cam_writew	= _ci_cam_writew_2;
			g_ci_io_ops.cam_writel	= _ci_cam_writel_2;
		#endif
			break;

		case 4 :
			g_ci_io_ops.readw 		= _ci_readw_4;
			g_ci_io_ops.writew		= _ci_writew_4;
			g_ci_io_ops.cam_readb	= _ci_cam_readb_4;
			g_ci_io_ops.cam_readw	= _ci_cam_readw_4;
			g_ci_io_ops.cam_readl	= _ci_cam_readl_4;
			g_ci_io_ops.cam_writeb	= _ci_cam_writeb_4;
			g_ci_io_ops.cam_writew	= _ci_cam_writew_4;
			g_ci_io_ops.cam_writel	= _ci_cam_writel_4;
			break;

		case 3 :
			g_ci_io_ops.readw 		= _ci_readw_3;
			g_ci_io_ops.writew		= _ci_writew_3;
			g_ci_io_ops.cam_readb	= _ci_cam_readb_3;
			g_ci_io_ops.cam_readw	= _ci_cam_readw_3;
			g_ci_io_ops.cam_readl	= _ci_cam_readl_3;
			g_ci_io_ops.cam_writeb	= _ci_cam_writeb_3;
			g_ci_io_ops.cam_writew	= _ci_cam_writew_3;
			g_ci_io_ops.cam_writel	= _ci_cam_writel_3;
			break;

		case 2 :
			g_ci_io_ops.readw 		= _ci_readw_2;
			g_ci_io_ops.writew		= _ci_writew_2;
			g_ci_io_ops.cam_readb	= _ci_cam_readb_2;
			g_ci_io_ops.cam_readw	= _ci_cam_readw_2;
			g_ci_io_ops.cam_readl	= _ci_cam_readl_2;
			g_ci_io_ops.cam_writeb	= _ci_cam_writeb_2;
			g_ci_io_ops.cam_writew	= _ci_cam_writew_2;
			g_ci_io_ops.cam_writel	= _ci_cam_writel_2;
			break;

		default :
			g_ci_io_ops.readw 		= _ci_readw_0;
			g_ci_io_ops.writew		= _ci_writew_0;
			g_ci_io_ops.cam_readb	= _ci_cam_readb_0;
			g_ci_io_ops.cam_readw	= _ci_cam_readw_0;
			g_ci_io_ops.cam_readl	= _ci_cam_readl_0;
			g_ci_io_ops.cam_writeb	= _ci_cam_writeb_0;
			g_ci_io_ops.cam_writew	= _ci_cam_writew_0;
			g_ci_io_ops.cam_writel	= _ci_cam_writel_0;
			break;
	}

	CI_SOC_MSG_NOTI("CI IO Initialized [%d,%d]\n",
					g_ci_io_ops.opt_barrier, g_ci_io_ops.opt_rw_core);

	return HW_IO_OK;
}

int HW_IO_Uninit(void)
{
	if (gIOBuf != NULL) {
		OS_Free(gIOBuf);
		gIOBuf = NULL;
	}

	CI_SOC_MSG_NOTI("CI IO Uninitialized\n");

	return HW_IO_OK;
}

/*----------------------------------------------------------------------------*/
/*
   NAME:         HW_IO_ResetSoft

   DESCRIPTION:  Perform software reset of hardware IO interface.

   PARAMETERS:   mId = module id

   RETURN VALUE: HW_IO_OK   = success
                 HW_IO_FAIL = fail
                                                                              */
/*----------------------------------------------------------------------------*/

/**
 *	Perform software reset of hardware IO interface
 */
int HW_IO_ResetSoft(MOD_ID mId)
{
	volatile UINT8 statusReg;
	int		chkCnt = MAXIM_RETRY_COUNT;

	COMM_WR(mId, CI_RS);

	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			return HW_IO_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (statusReg & CI_FR) {
	//	if (CHECK_FR(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_SOFT_RESET_CHECK_FR);
	}
	if( chkCnt <= 0 )
	{
		CI_SOC_MSG_ERR("HW_IO_ResetSoft: status error (FR=0), statusReg=0x%02X\n",
						statusReg);
	//	return HW_IO_FAIL;
	}

	/* CI spec doesn't say that RS must be cleared on soft reset. but */
	/* in case of some modules RS must be cleared to complete soft reset */
	COMM_WR(mId, 0x00);

	// OS_MsecSleep(10) L9 Originally Blocked by comment , but Previous Exist, So Ready with 0 jinhwan.bae
	CI_HW_IO_SLEEP(CI_D_IO_END_SOFT_RESET);

	return HW_IO_OK;
}


/*----------------------------------------------------------------------------*/
/*
   NAME:         HW_IO_SetRS

   DESCRIPTION:  Set RS Bit

   PARAMETERS:   mId = module id

   RETURN VALUE: HW_IO_OK   = success
                 HW_IO_FAIL = fail
                                                                              */
/*----------------------------------------------------------------------------*/

/**
 *	Perform set RS bit to 1 to softreset CAM
 *	Actually it's Netcast Request to meet CI+ 1.3 Confirmance Test
 *	FR Bit is not concerned, it's Netcast Needs, just return ASAP without FR checking.
 */

int HW_IO_SetRS(MOD_ID mId)
{
	COMM_WR(mId, CI_RS);

	return HW_IO_OK;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
   NAME:         HW_IO_NegoBuf

   DESCRIPTION:  Perform buffer negotiation.

   PARAMETERS:   mId = module id

   RETURN VALUE: HW_IO_OK   = success
                 HW_IO_FAIL = fail
                                                                              */
/*----------------------------------------------------------------------------*/
/**
 *	Perform buffer negotiation
 */
int HW_IO_NegoBuf( MOD_ID mId, UINT32 *o_pwBufSize )
{

	int		chkCnt;
	UINT16	size;
	UINT16	sizeLen;
	volatile UINT8 statusReg;

	if (NULL == o_pwBufSize) {
		CI_SOC_MSG_ERR("CI-IO NegoBuf Null ptr failed!\n");
		return -1;
	}

	if (mId != MOD_A) {
		return HW_IO_FAIL;    // single module
	}

	CiNegoBufSize[mId] = 0;

	CI_SOC_MSG_IO_INFO("STATUS Register value before reset => 0x%x\n", STAT_RD(mId));

	// OS_MsecSleep(100) L9 Originally Blocked by comment , but Previous Exist, So Ready with 0 jinhwan.bae
	CI_HW_IO_SLEEP(CI_D_IO_NEGOBUF_BEFORE_SOFTRESET);

	/* STEP 1: PERFORM SOFTWARE RESET ----------------------------------------*/
	CI_SOC_MSG_DBG_2("STEP 1 : Perform SW Reset\n");
	if (HW_IO_ResetSoft(mId) != HW_IO_OK) {
		CI_SOC_MSG_ERR("HW_IO_NegoBuf : HW_IO_ResetSoft() failed\n");
		return HW_IO_FAIL;
	}

	/* STEP 2: READ BUFFER SIZE FROM MODULE ----------------------------------*/
	CI_SOC_MSG_DBG_2("STEP 2 : Read Buffer size from Module\n");
	COMM_WR(mId, CI_SR);
	chkCnt = MAXIM_RETRY_COUNT;

	while (chkCnt--) {
		if (!guwDetectCard) {			// for ISR
			return HW_IO_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (statusReg & CI_DA) {
	//	if (CHECK_DA(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_NEGOBUF_CHECK_DA);
	}

	if (chkCnt <= 0) {
		COMM_WR(mId, 0x00);
		CI_SOC_MSG_ERR("HW_IO_NegoBuf: data not available, statusReg=0x%02X\n",
						statusReg);
		return HW_IO_FAIL;
	}

	/* read the length of buffer size */
	CI_SOC_MSG_DBG_2("Read MS_RD\n");
	sizeLen = (UINT16) SIZE_MS_RD(mId);
	sizeLen = (sizeLen<<8) & 0xFF00;
	CI_SOC_MSG_DBG_2("Read LS_RD\n");
	sizeLen |= (UINT16) SIZE_LS_RD(mId);

	CI_SOC_MSG_IO_INFO("Length of module buffer size = %d\n", sizeLen);

	/* read the high byte of buffer size */
	size = (UINT16) DATA_RD(mId);
	if (sizeLen == 2) {
		/* read the low byte of buffer size */
		size = (size<<8)&0xFF00;
		size |= (UINT16) DATA_RD(mId);
	}

	CI_SOC_MSG_IO_INFO("Module %c buffer size = %d\n", 'A'+mId, size);

	COMM_WR(mId, 0x00);

	/* STEP 3: DETERMINE BUFFER SIZE -----------------------------------------*/
	CI_SOC_MSG_DBG_2("STEP 3 : Determine Buffer Size\n");
	if ((size<MOD_BUF_SIZE_MIN) || (size>MOD_BUF_SIZE_MAX)) {
		CI_SOC_MSG_ERR("HW_IO_NegoBuf: invalid module buffer size");
		return HW_IO_FAIL;
	}
#if 0
	if (size > HOST_BUF_SIZE) {
		size = (UINT16) HOST_BUF_SIZE;
	}
#endif

	CiNegoBufSize[mId] = size;

	*o_pwBufSize = size;

	CI_SOC_MSG_IO_INFO("Negotiated buffer size = %d\n", size);

	/* STEP 4: WRITE BUFFER SIZE TO MODULE -----------------------------------*/
	COMM_WR(mId, CI_SW|CI_HC);

	chkCnt = MAXIM_RETRY_COUNT;

	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			return HW_IO_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (statusReg & CI_FR) {
	//	if (CHECK_FR(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_NEGOBUF_CHECK_FR);
	}

	if (chkCnt <= 0) {
		CI_SOC_MSG_ERR("HW_IO_NegoBuf: status error (FR=0), statusReg=0x%02X\n",
						statusReg);
		return HW_IO_FAIL;
	}

	/* write the size of buffer size */
	CI_SOC_MSG_DBG_2("Write Buffer Size : LS_WR - 0x02\n");
	SIZE_LS_WR(mId, (UINT8) 0x02);

	CI_SOC_MSG_DBG_2("Write Buffer Size : MS_WR - 0x00\n");
	SIZE_MS_WR(mId, (UINT8) 0x00);

	/* write the high byte of buffer size */
	CI_SOC_MSG_DBG_2("Write High byte of Buffer Size\n");
	DATA_WR(mId, (UINT8) ((size>>8)&0xFF));

	/* write the low byte of buffer size */
	CI_SOC_MSG_DBG_2("Write Low byte of Buffer Size\n");
	DATA_WR(mId, (UINT8) (size&0xFF));

	//  OS_MsecSleep(10) In L9, Previous 10 Exist. Please Refer to that at the problem. jinhwan.bae
	CI_HW_IO_SLEEP(CI_D_IO_NEGOBUF_AFTER_WRITE_DATA);

	COMM_WR(mId, 0x00);

	CI_SOC_MSG_IO_INFO("Buffer negotiation SUCCESS\n");

	return HW_IO_OK;
}

extern ktime_t g_read_t, g_write_t;

/**
 *	Perform Read from CI Module
 */
int	HW_IO_Read( MOD_ID mId, UINT8 *o_pbyData, UINT16 *io_pwDataBufSize, int to_user )
{
#ifdef CI_IO_TIME_PRINT
	ktime_t	start_t, data_t, end_t;
#endif
//	UINT8	aucBuf[MAX_POSS_BUF_SIZE];
	UINT8	*aucBuf = NULL;
	UINT8	*pBuf = NULL;
	UINT32  uiTmp = 0;
	UINT16	size;
	UINT16	i = 0;
	volatile UINT8 statusReg;
	int		chkCnt;
	int		burst_mode = !!g_ci_burst_mode;
	UINT32	err_chk_mode = CI_ERR_CHK_MODE_FULL;

#if defined(CI_OPT_CHK_MODE)
	err_chk_mode = CiOptChkModeSimple[mId];
#endif

#ifdef CI_IO_TIME_PRINT
	start_t = ktime_get();
#endif

	aucBuf = gIOBuf;
	if (aucBuf == NULL) {
		CI_SOC_MSG_ERR("Null gIOBuf\n");
		return HW_IO_FAIL;
	}

	/* Set 1 Byte Mode Default, */
	if (burst_mode) {
		CI_ChangeAccessMode(ACCESS_1BYTE_MODE);
	}

	/* confirm that data is available */
	chkCnt = gCI_KDRV_MaxCount_R;
	while (chkCnt--) {
		if (!guwDetectCard) {			// for ISR
			return HW_IO_FAIL;
		}
		if (guwIsPowerRestart) {		// for power restart
			return HW_IO_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (statusReg & CI_DA) {
	//	if (CHECK_DA(mId)) {
		//	CI_SOC_MSG_ERR("DA checked\n");
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_READ_CHECK_DA);
	}

	if (chkCnt <= 0) {
		CI_SOC_MSG_ERR("HW_IO_Read: IO busy (DA=0), statusReg=0x%02X\n",
						statusReg);
		return HW_IO_BUSY;
	}
	/* read the size of data */
	size = (UINT16) SIZE_MS_RD(mId);
	size = (size<<8) & 0xFF00;
	size |= (UINT16) SIZE_LS_RD(mId);
	CI_SOC_MSG_DBG_2("size = 0x%04x\n", size);

	if (size < 2) {
		CI_SOC_MSG_ERR("HW_IO_Read: Too small size[%d]\n", size);
		return HW_IO_FAIL;
	}
	if (!CiNegoBufSize[mId]) {
		CI_SOC_MSG_ERR("HW_IO_Read: Zero CiNegBuffSize[%d]\n", mId);
		return HW_IO_FAIL;
	}
	if (size > CiNegoBufSize[mId]) {
		CI_SOC_MSG_ERR("HW_IO_Read: size is bigger than buffer (size=%d)\n", size);
		return HW_IO_FAIL;
	}

//	CI_SOC_MSG_ERR("* Input len : %d\n", (*io_pwDataBufSize));
//	CI_SOC_MSG_ERR("* Trans len : %d\n", size);

	if (*io_pwDataBufSize <= size) {
		size = *io_pwDataBufSize;
	} else {
		*io_pwDataBufSize = size;
	}

	uiTmp = size;
	if (uiTmp > MAX_POSS_BUF_SIZE) {
		pBuf = OS_KMalloc(uiTmp);
		if (pBuf == NULL) {
			CI_SOC_MSG_ERR("HW_IO_Read: malloc <<F>>\n");
			return MEM_ALLOC_FAILED;
		}
	} else {
		pBuf = aucBuf;
	}

	/* read the first byte */
	pBuf[0] = DATA_RD_IO(mId);

	if (err_chk_mode != CI_ERR_CHK_MODE_SIMPLE) {
	 	statusReg = STAT_RD(mId);
		if ((statusReg&CI_DA) || !(statusReg&CI_RE)) {
			CI_SOC_MSG_ERR("HW_IO_Read: IO status error (DA=1 or RE=0), status reg=0x%02x\n", statusReg);
			goto HW_IO_READ_FAIL;
		}
	}

#ifdef CI_IO_TIME_PRINT
	data_t = ktime_get();
#endif

#ifdef _CI_ACCESS_BURST_MODE_IO_
	if (burst_mode) {
		/* read middle bytes */
		/* Using 32 bit burst mode in M14B0
		    32 bit burst mode : DATA32_RD, Read CiModBaseAddr + DATA32_REG(0x8000), 0xD8008000
		    1 byte mode : DATA_REG, Read CiModBaseAddr + DATA_REG(0x0000), 0xD8000000		*/

		/* This Routine is only for middle bytes.
		    Processing Data Size is Target Size - First Byte(already read) - Last Byte (Last Byte will be read by BYTE mode, for checking RE inversion) */
		int remain_size = size - 2;
		int loop_cnt = 0;
		int remainer_cnt = 0;
		UINT32 data_value = 0;

		if (remain_size > 4)
		{
			CI_ChangeAccessMode(ACCESS_4BYTE_MODE);

			while (remain_size > 4)
			{
				/* Read 32 bit burst data in DATA32_REG */
				data_value = DATA32_RD(mId);

				if (err_chk_mode != CI_ERR_CHK_MODE_SIMPLE) {
					/* jinhwan.bae insert to check the Read Error Check - is it valid in burst mode ? */
					statusReg = STAT_RD_IO(mId);
					if (!(statusReg&CI_RE)) {
				//	if (!CHECK_RE(mId)) {
						CI_SOC_MSG_IO_INFO("HW_IO_Read: IO status error in Burst Read32 (RE=0)\n");
						CI_SOC_MSG_IO_INFO("remain %d bytes of %d bytes, status reg=0x%02X\n",
											remain_size, size, statusReg);

						goto HW_IO_READ_FAIL;
					}
				}

				/* Save data to buffer. To escape alignment trap error, just byte processing */
				/* index 1 ~ 4, considering 1st Byte, already read and saved index 0 */
				pBuf[(4*loop_cnt)+1] = ( (data_value & 0x000000FF) >> 0);
				pBuf[(4*loop_cnt)+2] = ( (data_value & 0x0000FF00) >> 8);
				pBuf[(4*loop_cnt)+3] = ( (data_value & 0x00FF0000) >> 16);
				pBuf[(4*loop_cnt)+4] = ( (data_value & 0xFF000000) >> 24);

				remain_size -= 4;
				loop_cnt++;
			}
			CI_SOC_MSG_DBG("HW_IO_Read: Burst loop count = %d\n", loop_cnt);
		}

		CI_ChangeAccessMode(ACCESS_1BYTE_MODE);

		for (remainer_cnt=0; remainer_cnt<remain_size; remainer_cnt++)
		{
			/* start with + index 1, considering 1st Byte, already read and saved index 0, 4byte data set, may be read and saved index 1 ~ 4 */
			pBuf[(4*loop_cnt)+1+remainer_cnt] = DATA_RD_IO(mId);

			if (err_chk_mode != CI_ERR_CHK_MODE_SIMPLE) {
				statusReg = STAT_RD_IO(mId);
				if (!(statusReg&CI_RE)) {
					CI_SOC_MSG_IO_INFO("HW_IO_Read: IO status error (RE=0)\n");
					CI_SOC_MSG_IO_INFO("remain %d bytes of %d bytes, status reg=0x%02X\n",
									remain_size, size, statusReg);
					goto HW_IO_READ_FAIL;
				}
			}
		}

		/* move buffer pointer to last byte */
		i = 4*loop_cnt + remain_size + 1;
	}
	else
#endif
	{
	#if	defined(CI_USE_DMAC_MULTI)
		ci_dma_cam_readb_multi((volatile void *)(CiModBaseAddr[mId] + DATA_REG), (volatile void *)&pBuf[1], size - 2);
		if (!guwDetectCard) {		// for ISR
			goto HW_IO_READ_FAIL;
		}
		if (guwIsPowerRestart) { 	// for power restart
			goto HW_IO_READ_FAIL;
		}

		if (err_chk_mode != CI_ERR_CHK_MODE_SIMPLE) {
			statusReg = STAT_RD_IO(mId);
			if (!(statusReg&CI_RE)) {
				CI_SOC_MSG_ERR("HW_IO_Read: IO status error (RE=0)\n");
				CI_SOC_MSG_ERR("%d'th byte of %d bytes, status reg=0x%02X\n", i, size, statusReg);
				goto HW_IO_READ_FAIL;
			}
		}
		i = size - 1;
	#else
		/* read middle bytes - original one, byte mode , same as H13, H14, M14Ax */
		for (i=1; i<(size-1); i++) {
			if (!guwDetectCard) {		// for ISR
				goto HW_IO_READ_FAIL;
			}
			if (guwIsPowerRestart) { 	// for power restart
				goto HW_IO_READ_FAIL;
			}

			pBuf[i] = DATA_RD_IO(mId);

			if (err_chk_mode != CI_ERR_CHK_MODE_SIMPLE) {
				statusReg = STAT_RD_IO(mId);
				if (!(statusReg&CI_RE)) {
					CI_SOC_MSG_ERR("HW_IO_Read: IO status error (RE=0)\n");
					CI_SOC_MSG_ERR("%d'th byte of %d bytes, status reg=0x%02X\n", i, size, statusReg);
				#if 0
					mdelay(1);
					statusReg = STAT_RD_IO(mId);
					if (!(statusReg&CI_RE)) {
						CI_SOC_MSG_ERR("HW_IO_Read: IO status error(again) (RE=0)\n");
						CI_SOC_MSG_ERR("%d'th byte of %d bytes, status reg=0x%02X\n", i, size, statusReg);
						goto HW_IO_READ_FAIL;
					}
				#else
					goto HW_IO_READ_FAIL;
				#endif
				}
			}
		}
	#endif
	}

	/* read the last byte */
	pBuf[i] = DATA_RD_IO(mId);

	statusReg = STAT_RD_IO(mId);
	if (statusReg&CI_RE) {
		if (*io_pwDataBufSize <= size) {
			size = *io_pwDataBufSize;
		} else {
			*io_pwDataBufSize = size;
		}

		CI_SOC_MSG_ERR("HW_IO_Read: IO status error (RE=1)\n");
		CI_SOC_MSG_ERR("size=%d, status reg=0x%02X\n", size, statusReg);
	#if 0
		mdelay(1);
		statusReg = STAT_RD_IO(mId);
		if (statusReg&CI_RE) {
			CI_SOC_MSG_ERR("HW_IO_Read: IO status error(again) (RE=1)\n");
			CI_SOC_MSG_ERR("size=%d, status reg=0x%02X\n", size, statusReg);
			//
        	CI_SOC_MSG_ERR("/* read the last byte */\n");
	        CI_SOC_MSG_ERR("|Data|Stat|Size  |\n");
	        CI_SOC_MSG_ERR("|0x%02X|0x%02X|0x%04X|\n", pBuf[i], statusReg, size);
			goto HW_IO_READ_FAIL;
		}
	#else
		goto HW_IO_READ_FAIL;
	#endif
	}

#ifdef CI_IO_DUMP
	CI_SOC_MSG_IO_INFO( "Read Data(%d): ", size);
	for (i=0; i<size; i++) {
		CI_SOC_MSG_IO_INFO("0x%02X ", pBuf[i]);
	}
	CI_SOC_MSG_IO_INFO("\n");

//	CI_SOC_MSG_IO_INFO("Read %d bytes IO Data Success ... \n", size);
#endif /* CI_IO_DUMP */

#ifndef CI_IO_TIME_PRINT
	CI_SOC_MSG_IO_INFO("Original data size is %d bytes <<S>>\n", size);
#endif

	if (*io_pwDataBufSize <= size) {
		size = *io_pwDataBufSize;
	} else {
		*io_pwDataBufSize = size;
	}

	if (to_user) {
	#ifndef CI_IO_TIME_PRINT
		CI_SOC_MSG_IO_INFO("CI-IO> copy_to_user - gonna start\n");
		CI_SOC_MSG_IO_INFO("Addr[0x%x] Len[%d]\n",  (UINT32)o_pbyData, size);
	#endif
		if (copy_to_user(( void __user * )o_pbyData, pBuf, size)) {
			CI_SOC_MSG_ERR("CI-IO> copy_to_user <<F>>\n");
			goto HW_IO_READ_FAIL;
	    }
	} else {
	#ifndef CI_IO_TIME_PRINT
		CI_SOC_MSG_IO_INFO("CI-IO> memcpy to - gonna start\n");
		CI_SOC_MSG_IO_INFO("Addr[0x%x] Len[%d]\n",  (UINT32)o_pbyData, size);
	#endif
		memcpy((void *)o_pbyData, pBuf, size);
	}

	if (uiTmp > MAX_POSS_BUF_SIZE) {
		OS_Free(pBuf);
	}

#ifdef CI_IO_TIME_PRINT
	end_t = ktime_get();
	CI_SOC_MSG_TIME_US_IO(size, start_t, end_t);
//	CI_SOC_MSG_TIME_US_IO(size, data_t, end_t);
	g_read_t = ktime_add(g_read_t, ktime_sub(end_t, start_t));
#else
	CI_SOC_MSG_IO_INFO("%d bytes of data read <<S>>\n", size);
#endif

	return HW_IO_OK;

HW_IO_READ_FAIL:

	if (uiTmp > MAX_POSS_BUF_SIZE) {
		OS_Free( pBuf );
	}

#ifdef CI_IO_TIME_PRINT
	end_t = ktime_get();
	CI_SOC_MSG_TIME_US_IO(0, start_t, end_t);
	g_read_t = ktime_add(g_read_t, ktime_sub(end_t, start_t));
#else
	CI_SOC_MSG_ERR("\n Read IO Data <<F>>\n\n");
#endif
	return HW_IO_FAIL;
}

/**
 *	Perform Write to CI Module
 */
int HW_IO_Write( MOD_ID mId ,UINT8 *i_pbyData, UINT32 i_wDataBufSize, int from_user  )
{
#ifdef CI_IO_TIME_PRINT
	ktime_t start_t, data_t, end_t;
#endif
//	UINT8  *pBuf = i_pbyData;
	UINT8  *pBuf = NULL;
	UINT16	size = i_wDataBufSize;
	UINT16  uiTmp;
	UINT16	i;
	volatile UINT8 statusReg;
	int		chkCnt;
	int		burst_mode = !!g_ci_burst_mode;
	UINT32	err_chk_mode = CI_ERR_CHK_MODE_FULL;

#if defined(CI_OPT_CHK_MODE)
	err_chk_mode = CiOptChkModeSimple[mId];
#endif

#ifdef CI_IO_TIME_PRINT
	start_t = ktime_get();
#endif

	if (size < 2) {
		CI_SOC_MSG_ERR("HW_IO_Read: Too small size[%d]\n", size);
		return HW_IO_FAIL;
	}
	if (!CiNegoBufSize[mId]) {
		CI_SOC_MSG_ERR("HW_IO_Read: Zero CiNegBuffSize[%d]\n", mId);
		return HW_IO_FAIL;
	}

	uiTmp = size;
	if (uiTmp > MAX_POSS_BUF_SIZE) {
		pBuf = OS_KMalloc(uiTmp);
		if (pBuf == NULL) {
			CI_SOC_MSG_ERR("HW_IO_Write: malloc <<F>>\n");
			return MEM_ALLOC_FAILED;
		}
	} else {
		pBuf = gIOBuf;
	}

	if (from_user) {
	#ifndef CI_IO_TIME_PRINT
		CI_SOC_MSG_IO_INFO("CI-IO> copy_from_user - gonna start\n");
		CI_SOC_MSG_IO_INFO("Addr[0x%x] Len[%d]\n",  (UINT32)i_pbyData, size);
	#endif
		if (copy_from_user(pBuf, ( void __user * )i_pbyData, size)) {
			CI_SOC_MSG_ERR("CI-IO> copy_from_user <<F>>\n");
			goto HW_IO_WRITE_FAIL;
	    }
	} else {
	#ifndef CI_IO_TIME_PRINT
		CI_SOC_MSG_IO_INFO("CI-IO> memcpy from - gonna start\n");
		CI_SOC_MSG_IO_INFO("Addr[0x%x] Len[%d]\n",  (UINT32)i_pbyData, size);
	#endif
		memcpy(pBuf, (void *)i_pbyData, size);
	}

	/* Set 1 Byte Mode Default, */
	if (burst_mode) {
		CI_ChangeAccessMode(ACCESS_1BYTE_MODE);
	}

#ifdef CI_IO_DUMP
	CI_SOC_MSG_IO_INFO( "Send Data(%d): ", size);
	for (i=0; i<size; i++) {
		CI_SOC_MSG_IO_INFO("0x%02X ", pBuf[i]);
	}
	CI_SOC_MSG_IO_INFO("\n");
#endif /* CI_IO_DUMP */

	/* confirm that data is not available */
	chkCnt = gCI_KDRV_MaxCount_W;
	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			goto HW_IO_WRITE_FAIL;
		}
		if (guwIsPowerRestart) {	// for power restart
			goto HW_IO_WRITE_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (!(statusReg & CI_DA)) {
	//	if (!CHECK_DA(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_WRITE_CHECK_DA);
	}

	if (chkCnt <= 0) {
		CI_SOC_MSG_ERR("HW_IO_Write: IO busy (DA=1), statusReg=0x%02X\n",
						statusReg);
		if (uiTmp > MAX_POSS_BUF_SIZE) {
			OS_Free(pBuf);
		}
		return HW_IO_BUSY;
	}

#if defined(CI_MON_FR_RETRY)
	gCI_HW_IO_Write_Count++;
	gCI_HW_IO_Write_Count_FR_Retry = 0;
#endif

	/* set HC=1 */
	COMM_WR(mId, CI_HC);

	/* confirm that module is free */
	chkCnt = gCI_KDRV_MaxCount_W;
	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			goto HW_IO_WRITE_FAIL;
		}
		if (guwIsPowerRestart) {	// for power restart
			goto HW_IO_WRITE_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (statusReg & CI_FR) {
	//	if (CHECK_FR(mId)) {
			break;
		}
	#if defined(CI_MON_FR_RETRY)
		gCI_HW_IO_Write_Count_FR_Retry++;
	#endif
		CI_HW_IO_SLEEP(CI_D_IO_WRITE_CHECK_FR);
	}

#if defined(CI_MON_FR_RETRY)
	if (gCI_HW_IO_Write_Count_FR_Retry) {
		CI_SOC_MSG_DBG_3("FR Retry[%lld] @ [%lld]\n",
					gCI_HW_IO_Write_Count_FR_Retry,
					gCI_HW_IO_Write_Count);
	}
#endif

	if (chkCnt <= 0) {
		COMM_WR(mId, 0x00);
		CI_SOC_MSG_ERR("HW_IO_Write: IO status error (FR=0), statusReg=0x%02X\n",
						statusReg);
		goto HW_IO_WRITE_FAIL;
	}

	// write the size of data
	SIZE_LS_WR(mId, (UINT8) (size&0x00FF));
	SIZE_MS_WR(mId, (UINT8) ((size>>8)&0x00FF));
	CI_SOC_MSG_DBG_2("size = 0x%04x\n", size);

	// write the first byte
	DATA_WR_IO(mId, pBuf[0]);

	chkCnt = MAX_CHECK_COUNT;
	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			goto HW_IO_WRITE_FAIL;
		}
		if (guwIsPowerRestart) {	// for Power reset
			goto HW_IO_WRITE_FAIL;
		}
		if (err_chk_mode == CI_ERR_CHK_MODE_SIMPLE) {
			break;
		}
		statusReg = STAT_RD_IO(mId);
		if (!(statusReg & CI_FR) && (statusReg & CI_WE)) {
	//	if (!CHECK_FR(mId) && CHECK_WE(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE);
	}

	if (chkCnt <= 0) {
		CI_SOC_MSG_ERR("HW_IO_Write: IO status error (FR=1 or WE=0), statusReg=0x%02X\n",
						statusReg);
		goto HW_IO_WRITE_FAIL;
	}

#ifdef CI_IO_TIME_PRINT
	data_t = ktime_get();
#endif

#ifdef _CI_ACCESS_BURST_MODE_IO_
	if (burst_mode) {
		/* write middle bytes */
		/* Using 32 bit burst mode in M14B0
		    32 bit burst mode : DATA32_RD, Write CiModBaseAddr + DATA32_REG(0x8000), 0xD8008000
		    1 byte mode : DATA_REG, Write CiModBaseAddr + DATA_REG(0x0000), 0xD8000000		*/

		/* This Routine is only for middle bytes.
		    Processing Data Size is Target Size - First Byte(already write) - Last Byte (Last Byte will be written by BYTE mode, for checking WE inversion) */
		int remain_size = size - 2;
		int loop_cnt = 0;
		int remainer_cnt = 0;
		UINT32 data_value = 0;

		if (remain_size > 4)
		{
			CI_ChangeAccessMode(ACCESS_4BYTE_MODE);

			while (remain_size > 4)
			{
				/* make 32bit data to write */
				/* index 1 ~ 4, considering 1st Byte, already read and saved index 0 */
				data_value = pBuf[(4*loop_cnt)+1]  << 0;
				data_value = pBuf[(4*loop_cnt)+2]  << 8;
				data_value = pBuf[(4*loop_cnt)+3]  << 16;
				data_value = pBuf[(4*loop_cnt)+4]  << 24;

				/* Write 32 bit burst data to DATA32_REG */
				DATA32_WR(mId, data_value);

				remain_size -= 4;
				loop_cnt++;

				chkCnt = MAX_CHECK_COUNT;
				/* check write error */
				while (chkCnt--) {
					if (!guwDetectCard) {		// for ISR
						goto HW_IO_WRITE_FAIL;
					}
					if (guwIsPowerRestart) { 	// for Power reset
						goto HW_IO_WRITE_FAIL;
					}
					if (err_chk_mode == CI_ERR_CHK_MODE_SIMPLE) {
						break;
					}
					statusReg = STAT_RD_IO(mId);
					if (statusReg & CI_WE) {
				//	if (CHECK_WE(mId)) {
						break;
					}
					CI_HW_IO_SLEEP(CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE);
				}

				if (chkCnt <= 0) {
					CI_SOC_MSG_ERR("HW_IO_Write: IO status error in 32bit burst mode (WE=0), stauts=0x%02X\n",
									statusReg);
					goto HW_IO_WRITE_FAIL;
				}
			}
			CI_SOC_MSG_DBG("HW_IO_Write: Burst loop count = %d\n", loop_cnt);
		}

		CI_ChangeAccessMode(ACCESS_1BYTE_MODE);

		for (remainer_cnt=0; remainer_cnt<remain_size; remainer_cnt++)
		{
			/* start with + index 1, considering 1st Byte, already write index 0 and 4byte data set, may be written and composed by index 1 ~ 4 */
			DATA_WR(mId, pBuf[(4*loop_cnt)+1+remainer_cnt]);

			chkCnt = MAX_CHECK_COUNT;
			while (chkCnt--) {
				if (!guwDetectCard) {		// for ISR
					goto HW_IO_WRITE_FAIL;
				}
				if (guwIsPowerRestart) { 	//for Power reset
					goto HW_IO_WRITE_FAIL;
				}
				if (err_chk_mode == CI_ERR_CHK_MODE_SIMPLE) {
					break;
				}
				statusReg = STAT_RD_IO(mId);
				if (statusReg & CI_WE) {
			//	if (CHECK_WE(mId)) {
					break;
				}
				CI_HW_IO_SLEEP(CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE);
			}

			if (chkCnt <= 0) {
				CI_SOC_MSG_ERR("HW_IO_Write: IO status error in remainer of 32bit burst mode (WE=0), stauts=0x%02X\n",
							statusReg);
				goto HW_IO_WRITE_FAIL;
			}
		}

		/* move buffer pointer to last byte */
		i = 4*loop_cnt + remain_size + 1;
	}
	else
#endif
	{
	#if	defined(CI_USE_DMAC_MULTI)
		ci_dma_cam_writeb_multi((volatile void *)(CiModBaseAddr[mId] + DATA_REG), (volatile void *)&pBuf[1], size - 2);
		chkCnt = MAX_CHECK_COUNT;
		while (chkCnt--) {
			if (!guwDetectCard) {		// for ISR
				goto HW_IO_WRITE_FAIL;
			}
			if (guwIsPowerRestart) {	// for Power reset
				goto HW_IO_WRITE_FAIL;
			}
			if (err_chk_mode == CI_ERR_CHK_MODE_SIMPLE) {
				break;
			}
			statusReg = STAT_RD_IO(mId);
			if (statusReg & CI_WE) {
		//	if (CHECK_WE(mId)) {
				break;
			}
			CI_HW_IO_SLEEP(CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE);
		}
		//if (!alphaCrypt && chkCnt <= 0)
		if (chkCnt <= 0) {
			CI_SOC_MSG_ERR("HW_IO_Write: IO status error (WE=0), status=0x%02X\n",
							statusReg);
			goto HW_IO_WRITE_FAIL;
		}
		i = size - 1;
	#else
		/* write middle bytes - original one, byte mode , H13, H14, M14Ax */
		for (i=1; i<(size-1); i++)
		{
			DATA_WR(mId, pBuf[i]);

			chkCnt = MAX_CHECK_COUNT;
			while (chkCnt--) {
				if (!guwDetectCard) {		// for ISR
					goto HW_IO_WRITE_FAIL;
				}
				if (guwIsPowerRestart) {	// for Power reset
					goto HW_IO_WRITE_FAIL;
				}
				if (err_chk_mode == CI_ERR_CHK_MODE_SIMPLE) {
					break;
				}
				statusReg = STAT_RD_IO(mId);
				if (statusReg & CI_WE) {
			//	if (CHECK_WE(mId)) {
					break;
				}
				CI_HW_IO_SLEEP(CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE);
			}

			//if (!alphaCrypt && chkCnt <= 0)
			if (chkCnt <= 0) {
				CI_SOC_MSG_ERR("HW_IO_Write: IO status error (WE=0), status=0x%02X\n",
								statusReg);
				goto HW_IO_WRITE_FAIL;
			}
		}
	#endif
	}

	/* write the last byte */
	DATA_WR_IO(mId, pBuf[i]);

	chkCnt = MAX_CHECK_COUNT;
	while (chkCnt--) {
		if (!guwDetectCard) {		// for ISR
			goto HW_IO_WRITE_FAIL;
		}
		if (guwIsPowerRestart) {	//for Power reset
			goto HW_IO_WRITE_FAIL;
		}
		statusReg = STAT_RD_IO(mId);
		if (!(statusReg & CI_WE)) {
	//	if (!CHECK_WE(mId)) {
			break;
		}
		CI_HW_IO_SLEEP(CI_D_IO_WRITE_LAST_BYTE_CHECK_WE);
	}

	if (chkCnt <= 0) {
		CI_SOC_MSG_ERR("HW_IO_Write: IO status error (WE=1), statusReg=0x%02X\n",
						statusReg);
		goto HW_IO_WRITE_FAIL;
	}

	COMM_WR(mId, 0x00);

	if (uiTmp > MAX_POSS_BUF_SIZE) {
		OS_Free(pBuf);
	}

#ifdef CI_IO_TIME_PRINT
	end_t = ktime_get();
	CI_SOC_MSG_TIME_US_IO(size, start_t, end_t);
//	CI_SOC_MSG_TIME_US_IO(size, data_t, end_t);
	g_write_t = ktime_add(g_write_t, ktime_sub(end_t, start_t));
#else
	CI_SOC_MSG_IO_INFO("%d bytes of data write <<S>>\n", size);
//	CI_SOC_MSG_IO_INFO("Write IO Data Success ... \n ");
#endif

	return HW_IO_OK;

HW_IO_WRITE_FAIL:

	COMM_WR(mId, 0x00);

	if (uiTmp > MAX_POSS_BUF_SIZE) {
		OS_Free(pBuf);
	}

#ifdef CI_IO_TIME_PRINT
	end_t = ktime_get();
	CI_SOC_MSG_TIME_US_IO(0, start_t, end_t);
	g_write_t = ktime_add(g_write_t, ktime_sub(end_t, start_t));
#else
	CI_SOC_MSG_ERR("\n HW_IO_Write <<F>>\n\n");
#endif

	return HW_IO_FAIL;
}

int CI_IO_EnableLog( UINT32 ulArg)
{
	int eRet = NOT_OK;
	int idx;

	CI_SOC_MSG_DBG("<--CI_IO_EnableLog\n");

	for (idx = 0; idx < LX_MAX_MODULE_DEBUG_NUM; idx++) {
		if (ulArg & (1<<idx)) {
			OS_DEBUG_EnableModuleByIndex(g_ci_debug_fd, idx, DBG_COLOR_NONE);
		} else {
			OS_DEBUG_DisableModuleByIndex(g_ci_debug_fd, idx);
		}
	}

	CI_SOC_MSG_DBG("-->CI_IO_EnableLog\n");

	eRet = OK;

	return (eRet);
}

/*----------------------------------------------------------------------------*/

