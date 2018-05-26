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
 *  Core driver implementation for ci device.
 *
 *  author		Srinivasan Shanmugam (srinivasan.shanmugam@lge.com)
 *  author		Hwajeong Lee (hwajeong.lee@lge.com)
 *  author		Jinhwan Bae (jinhwan.bae@lge.com) - modifier
 *  author		Hyunho Kim (hyunho747.kim@lge.com) - modifier
 *  version		1.0
 *  date		2010.02.19
 *  note		Additional information.
 *
 *  @addtogroup lg1150_ci
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
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

#include "os_util.h"
#include "ci_drv.h"
#include "ci_cis.h"
#include "ci_io.h"
#include "ci_io_dma.h"
#include "ci_regdefs.h"
#include "ci_defs.h"			// for delay definition, jinhwan.bae

#include "../sys/sys_regs.h"	//for CTOP CTRL Reg. map
#include "ci_coredrv.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern irqreturn_t CI_ISR_Handler( int irq, void *dev_id, struct pt_regs *regs );

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
volatile S_CI_REG_T* gpstCIReg = NULL;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile UINT32 guwDetectCard = 0;
volatile UINT32 guwIsPowerRestart = 0;

unsigned long	CiModBaseAddr[MAX_MOD_NUM] = { 0, };

unsigned int	gCIChipRev = 0;
unsigned int	gCIRegBase = 0;
unsigned int	gCIRegSize = 0;
unsigned int	gCIModBase = 0;
unsigned int	gCIModSize = 0;

#define MAX_CHECK_COUNT_DIV_R	2
#define MAX_CHECK_COUNT_DIV_W	2

UINT32 gCI_KDRV_MaxCount_R = MAX_CHECK_COUNT;
UINT32 gCI_KDRV_MaxCount_W = MAX_CHECK_COUNT;

static UINT32 *pDefault_CI_Delay = NULL;
UINT32 gCI_KDRV_Delay[CI_D_ENUM_MAX];
static const UINT32 gDefault_CI_Delay[CI_D_ENUM_MAX] = { DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_OK,				// 5
											DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_NG,				// 10
											DEFAULT_CI_DELAY_CIS_END_WRITE_COR,						// 100
											DEFAULT_CI_DELAY_CIS_DURING_READ_TUPLE,					// 5
											DEFAULT_CI_DELAY_CIS_END_READ_TUPLE_INITIAL,			// SLEEP_VALUE_INIT, 20
											DEFAULT_CI_DELAY_CIS_PARSE_NON_CI_TUPLE,				// 3
											DEFAULT_CI_DELAY_INIT_POWER_CONTROL,					// H13 Blocked, L9 5
											DEFAULT_CI_DELAY_INIT_AFTER_INTERRUPT_ENABLE,			// 10
											DEFAULT_CI_DELAY_CAM_INIT_BTW_VCC_CARDRESET,			// 300
											DEFAULT_CI_DELAY_CAM_INIT_BTW_CARDRESET_NOTRESET,		// 5
											DEFAULT_CI_DELAY_IO_SOFT_RESET_CHECK_FR,				// 10
											DEFAULT_CI_DELAY_IO_END_SOFT_RESET,						// L9 Blocked, 0 Originally (10)
											DEFAULT_CI_DELAY_IO_NEGOBUF_BEFORE_SOFTRESET,			// L9 Blocked, 0 Originally (100)
											DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_DA,					// 10
											DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_FR,					// 10
											DEFAULT_CI_DELAY_IO_NEGOBUF_AFTER_WRITE_DATA,			// 5 , Previously (10)
											DEFAULT_CI_DELAY_IO_READ_CHECK_DA,						// 10
											DEFAULT_CI_DELAY_IO_WRITE_CHECK_DA,						// 10
											DEFAULT_CI_DELAY_IO_WRITE_CHECK_FR,						// 10
											DEFAULT_CI_DELAY_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE,		// 10
											DEFAULT_CI_DELAY_IO_WRITE_MIDDLE_BYTE_CHECK_WE,			// 10
											DEFAULT_CI_DELAY_IO_WRITE_LAST_BYTE_CHECK_WE		};	// 10

static const UINT32 gDefault_CI_Delay_H15[CI_D_ENUM_MAX] = { DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_OK,				// 5
											DEFAULT_CI_DELAY_CIS_CONFIG_FIRST_TUPLE_NG,				// 10
											DEFAULT_CI_DELAY_CIS_END_WRITE_COR,						// 100
											DEFAULT_CI_DELAY_CIS_DURING_READ_TUPLE,					// 5
											DEFAULT_CI_DELAY_CIS_END_READ_TUPLE_INITIAL,			// SLEEP_VALUE_INIT, 20
											DEFAULT_CI_DELAY_CIS_PARSE_NON_CI_TUPLE,				// 3
											DEFAULT_CI_DELAY_INIT_POWER_CONTROL,					// H13 Blocked, L9 5
											DEFAULT_CI_DELAY_INIT_AFTER_INTERRUPT_ENABLE,			// 10
											DEFAULT_CI_DELAY_CAM_INIT_BTW_VCC_CARDRESET,			// 300
											DEFAULT_CI_DELAY_CAM_INIT_BTW_CARDRESET_NOTRESET,		// 5
											DEFAULT_CI_DELAY_IO_SOFT_RESET_CHECK_FR,				// 10
											DEFAULT_CI_DELAY_IO_END_SOFT_RESET,						// L9 Blocked, 0 Originally (10)
											DEFAULT_CI_DELAY_IO_NEGOBUF_BEFORE_SOFTRESET,			// L9 Blocked, 0 Originally (100)
											DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_DA,					// 10
											DEFAULT_CI_DELAY_IO_NEGOBUF_CHECK_FR,					// 10
											DEFAULT_CI_DELAY_IO_NEGOBUF_AFTER_WRITE_DATA,			// 5 , Previously (10)
											DEFAULT_CI_DELAY_IO_READ_CHECK_DA/MAX_CHECK_COUNT_DIV_R,														// 2
											DEFAULT_CI_DELAY_IO_WRITE_CHECK_DA/MAX_CHECK_COUNT_DIV_W,														// 2
											DEFAULT_CI_DELAY_IO_WRITE_CHECK_FR/MAX_CHECK_COUNT_DIV_W,														// 2
											DEFAULT_CI_DELAY_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE,		// 10
											DEFAULT_CI_DELAY_IO_WRITE_MIDDLE_BYTE_CHECK_WE,			// 10
											DEFAULT_CI_DELAY_IO_WRITE_LAST_BYTE_CHECK_WE		};	// 10

static DEFINE_SPINLOCK(ci_core_lock);

UINT64 gCI_HW_IO_Write_Count = 0;
UINT64 gCI_HW_IO_Write_Count_FR_Retry = 0;

UINT32 gCI_CAM_Init_Done = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

static UINT32 *g_ci_irq_sync = NULL;
static int	g_ci_irq_num = 0;



#if 1 /* Add code for supporting Hibernation Suspend/Resume 2013. 08. 14. jinhwan.bae */

/*
 * (Ref. DDI0203B_SMC_PL092_trm.pdf)
 * SMC RegBase = 0xFF400000
 * SMC RegBase + 0x38 : SMBIDCYR2
 *                      dle cycle control for memory bank2
 *                      [31:04] Reserved
 *                      [03:00] IDCY (RW) (Default - 1111b)
 *                              Idle or turn-around cycles
 *                              Turn around time = IDCY x t_HCLK
 * SMC RegBase + 0x4C : SMBCR2
 *                      Control for memory bank2
 *                      [31:08] Reserved
 *                      [07:06] MW (RW)
 *                              Memory Width
 *                              00 - 8bit
 *                              01 - 16bit
 *                              02 - 32bit
 *                      [05:05] BM (RW)
 *                              0 - Non-burst memory devices (default)
 *                              1 - Burst ROM memory
 *                      [04:04] WP (RW)
 *                              0 - No write protection (default)
 *                              1 - Write protected
 *                      [03:03] CSPol (RW)
 *                              0 - Active-LOW SMCS (default)
 *                              1 - Active-HIGH SMCS (default)
 *                      [02:02] WaitEn (RW)
 *                              Ext. memory controller wait signal enable
 *                              0 - Not controlled by the ext. wait sig. (default)
 *                              1 - SMC looks for the ext. wait input sig, SMWAIT.
 *                      [01:01] WaitPol (RW)
 *                              0 - SMWAIT is Active-LOW (default)
 *                              1 - SMWAIT is Active-HIGH
 *                      [00:00] RBLE (RW)
 *                              Read byte lane enable
 */

UINT32 gSMCVirtAddr = 0;

static void CI_InitPowerPol(void)
{
#if 0
	UINT32 *pwr_pol_reg;
	UINT32 reg_val;

	pwr_pol_reg = (UINT32 *)ioremap(0xC898E494, 0x04);
	if (pwr_pol_reg == NULL) {
		CI_SOC_MSG_ERR("NULL pwr_pol_reg\n");
		return;
	}
	CI_SOC_MSG_DBG("pwr_pol_reg = 0x%08p\n", pwr_pol_reg);

	reg_val = readl(pwr_pol_reg) | 0x01;
	writel(reg_val, pwr_pol_reg);

	CI_SOC_MSG_DBG("pwr_pol_reg val = 0x%08x\n", readl(pwr_pol_reg));
	iounmap(pwr_pol_reg);
#endif
}

static void CI_InitDCO(void)
{
#if 0
	UINT32 *dco_reg;

	dco_reg = (UINT32 *)ioremap(0xC88300C0, 0x04);
	if (dco_reg == NULL) {
		CI_SOC_MSG_ERR("NULL dco_reg\n");
		return;
	}
	CI_SOC_MSG_DBG("dco_reg = 0x%08p\n", dco_reg);
	CI_SOC_MSG_DBG("dco_reg val = 0x%08x\n", readl(dco_reg));
	iounmap(dco_reg);
#endif
}

void CI_InitSMC(void)
{
	gSMCVirtAddr = (UINT32) ioremap(0xFF400038, 0x1C);
	if(lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
		*(UINT32*)(gSMCVirtAddr) = (UINT32)0x0;
		*(UINT32*)(gSMCVirtAddr+0x14) = (UINT32)0x45;
		CI_InitPowerPol();
		CI_InitDCO();
	} else {
		*(UINT32*)(gSMCVirtAddr) = (UINT32)0x0;
		*(UINT32*)(gSMCVirtAddr+0x14) = (UINT32)0x45;
	}

	CI_SOC_MSG_DBG("gMSCVirtAddr : 0x%08x <- 0xFF400038\n", gSMCVirtAddr);
	CI_SOC_MSG_DBG("gMSCVirtAddr + 0x00 = 0x%08x\n", *(UINT32*)(gSMCVirtAddr));
	CI_SOC_MSG_DBG("gMSCVirtAddr + 0x14 = 0x%08x\n", *(UINT32*)(gSMCVirtAddr+0x14));
	return;
}

void CI_UnmapSMC(void)
{
	if (gSMCVirtAddr) {
		iounmap((void *)gSMCVirtAddr);
		gSMCVirtAddr = 0;
	}

	return;
}

//volatile UINT32 citd;
static void CI_RegTest(void)
{
#if 0
	if (1) {
		volatile UINT32 i;

		CI_SOC_MSG_DBG("Reg Test - Interval (16-bit aligned)\n");

		ci_writew(INIT_VAL, gpstCIReg->uniCntrlStat3.unCntrlStat3);	//0x0004
	//	for (i=0 ; i<9 ; i++);
	//	ndelay(100);
	//	citd++;
		ci_writew(INIT_VAL, gpstCIReg->uniIntrCntrl.unIntrCntrl);	//0x002a
	}

	if (1) {
		volatile UINT32 i;

		CI_SOC_MSG_DBG("Reg Test - Interval (32-bit aligned)\n");

		ci_writew(INIT_VAL, gpstCIReg->uniCntrlStat1.unCntrlStat1);		//0x0000
	//	for (i=0 ; i<10 ; i++);
	//	ndelay(100);
	//	citd++;
		ci_writew(0x0F03, gpstCIReg->uniPCCardCntrl2.unPCCardCntrl2);	//0x0008
	}

	if (0) {
		CI_SOC_MSG_DBG("Reg Test - Repeat\n");

		ci_writew(0x0003, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);
		ci_writew(0x0001, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);
		ci_writew(0x0003, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);
	}
#endif
}

void CI_DefaultInit(void)
{
	UINT16 	unRetVal = 0;
	unsigned long flags;

	spin_lock_irqsave(&ci_core_lock, flags);

	// Initialize global varaibles
	guwDetectCard 			= 0;
	guwIsPowerRestart 		= 0;

	gCI_CAM_Init_Done = 0;

	/* jinhwan.bae 20140303 make ci related ports internal pull-up for no-ci platform */
	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev() >= LX_CHIP_REV(H15, A0) )
	{
		/*
		 * CTR42(0xC898E4A8) [31:26] pull-up for cam_* port
		 */
		UINT32 reg_val;
		reg_val = CTOP_CTRL_H15A0_READ(VIP, 0xA8);
		reg_val &= 0x03FFFFFF;	// enable pull-up [31:26]
		CTOP_CTRL_H15A0_WRITE(VIP, 0xA8, reg_val);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev() >= LX_CHIP_REV(H14, A0) )
	{
		/* TBD - should be checked CTOP Register  */
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, C0) )
	{
		/* 0xC0019048 */
		/* set TOP CTRL18 , offset 0x48 */
		/* Some bits are reversed on the contrary to M14B0 */
		CTOP_CTRL_M14B0_WRITE(TOP, 0x48, 0x80080088);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, B0) )
	{
		/* 0xC0019048 */
		/* set TOP CTRL18 , offset 0x48,  to 0 */
		CTOP_CTRL_M14B0_WRITE(TOP, 0x48, 0);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, A0) )
	{
		/* set CTOP CTRL53, offset 0xD4, to 0 */
		CTOP_CTRL_M14_WRITE(0xD4, 0);
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if( lx_chip_rev() >= LX_CHIP_REV(H13, A0) )
	{
		/* set CTOP CTRL53, offset 0xD4, to 0 */
		CTOP_CTRL_H13_WRITE(0xD4, 0);
	}
#endif
	else
	{
		CI_SOC_MSG_ERR("Invalid Chip 0x%x\n", lx_chip_rev());
	}

	CI_RegTest();

	CI_SOC_MSG_DBG("Reset Card Control 1\n");
	ci_writew(INIT_VAL, gpstCIReg->uniCntrlStat1.unCntrlStat1);

	// Set Not-POD Mode
	CI_SOC_MSG_DBG("Set Not-POD Mode\n");
	ci_writew(INIT_VAL, gpstCIReg->uniCntrlStat3.unCntrlStat3);

	CI_SOC_MSG_DBG_2("Init Interrupt Control\n");
	ci_writew(INIT_VAL, gpstCIReg->uniIntrCntrl.unIntrCntrl);

	// Interrupt Enable
	CI_SOC_MSG_DBG_2("Interrupt Enable\n");
	ci_writew(0xFFFE, gpstCIReg->uniIntrMask.unIntrMask);

	spin_unlock_irqrestore(&ci_core_lock, flags);

#ifdef _CI_KDRV_DELAY_USLEEP_RANGE_
	CI_SOC_MSG_NOTI("CI Driver use usleep_range \n");
#else
	CI_SOC_MSG_NOTI("CI Driver use msleep \n");
#endif
	// for interrupt signal is stabled
	CI_HW_IO_SLEEP(CI_D_INIT_AFTER_INTERRUPT_ENABLE);

	spin_lock_irqsave(&ci_core_lock, flags);

	unRetVal = ci_readw(gpstCIReg->uniIntrCntrl.unIntrCntrl);

	// CI Module detect ... check if module is already inserted
	CI_SOC_MSG_DBG_2("Module Detection\n");
	if((ci_readw(gpstCIReg->uniCntrlStat2.unCntrlStat2) & CD1_CD2_HIGH) == 0x00)
	{
		if (!guwDetectCard) {
			// CAM Inserted
			unRetVal |= CD_POL;
			guwDetectCard = 1;
			CI_SOC_MSG_INFO("CAM Module is inserted ...\n");
		}
	}
	else
	{
		if (guwDetectCard) {
			// CAM Removed
			unRetVal &= ~CD_POL;
			guwDetectCard = 0;
			CI_SOC_MSG_INFO("CAM Module is not inserted ... \n");
		}
	}

	CI_SOC_MSG_DBG_2("Clear Interrupt Control : 0x%04x\n", unRetVal);
	ci_writew(unRetVal, gpstCIReg->uniIntrCntrl.unIntrCntrl);

#if 1
	CI_SetPCCardBusTiming(PCMCIA_BUS_SPEED_LOW);
#else
	CI_SOC_MSG_DBG_2("PCMCIA Bus Speed Min - Cntrl2\n");
	ci_writew(0x0F03, gpstCIReg->uniPCCardCntrl2.unPCCardCntrl2);

	CI_SOC_MSG_DBG_2("PCMCIA Bus Speed Min - Cntrl3\n");
	ci_writew(0x0003, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);
#endif

	CI_SOC_MSG_DBG_2("Init Interrupt Flag\n");
	ci_writew(INIT_VAL, gpstCIReg->uniIntrFlag.unIntrFlag);

	spin_unlock_irqrestore(&ci_core_lock, flags);

	CI_SOC_MSG_DBG_2("Done\n");
	return;

}

#endif

/*
 *
 */
extern BOOLEAN gbIsCISChecked;
SINT32 CI_Check_CD_Pol(UINT32 opt, const char *func)
{
	SINT32 ret = 0;
#if defined(CI_POL_CHECK_ON)
	UINT16 pol_state;
	UINT16 regVal;
	unsigned long flags;

	spin_lock_irqsave(&ci_core_lock, flags);

	pol_state = ci_readw(gpstCIReg->uniCntrlStat2.unCntrlStat2);

	if((pol_state & CD1_CD2_HIGH) != 0x00)
	{
		if (guwDetectCard) {
			regVal = ci_readw(gpstCIReg->uniIntrCntrl.unIntrCntrl);
			regVal &= ~CD_POL;
			guwDetectCard = 0;
			gbIsCISChecked = FALSE;
			ci_writew(regVal, gpstCIReg->uniIntrCntrl.unIntrCntrl);
			CI_SOC_MSG_INFO("Abnormal CD Status : Removed [%s]\n", (char *)func);
		}
		ret = 1;
	}
	else
	{
		if (opt) {
			if (!guwDetectCard) {
				regVal = ci_readw(gpstCIReg->uniIntrCntrl.unIntrCntrl);
				regVal |= CD_POL;
				guwDetectCard = 1;
				ci_writew(regVal, gpstCIReg->uniIntrCntrl.unIntrCntrl);
				CI_SOC_MSG_INFO("Abnormal CD Status : Inserted [%s]\n", (char *)func);
			}
			ret = 2;
		}
	}

	spin_unlock_irqrestore(&ci_core_lock, flags);
#endif

	return ret;
}

#if 0
SINT32 CI_Check_Init_Reg_Status(void)
{
	SINT32 ret = OK;
	UINT16 unRegVal;
	unsigned long flags;

	spin_lock_irqsave(&ci_core_lock, flags);

	unRegVal = ci_readw(gpstCIReg->uniCntrlStat1.unCntrlStat1);
	if ((unRegVal & (VCC_SW0|EN_PCCARD)) != (VCC_SW0|EN_PCCARD)) {
		ret = NOT_OK;
	} else if (unRegVal & CARD_RESET) {
		ret = NOT_OK;
	}
	if (ret != NOT_OK) {
		CI_SOC_MSG_CIS_ERR("Invalid gpstCIReg->uniCntrlStat1 : 0x%04X\n", unRegVal);
	}

	spin_unlock_irqrestore(&ci_core_lock, flags);

	return ret;
}
#endif

/*
 *
 */
SINT32 CI_Init_Irq(void)
{
	int ret;

	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
		g_ci_irq_num = H15_IRQ_DVBCI;
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0)) {
		g_ci_irq_num = H14_IRQ_DVBCI;
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0)) {
		g_ci_irq_num = M14_B0_IRQ_DVBCI;
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0)) {
		g_ci_irq_num = M14_A0_IRQ_DVBCI;
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0)) {
		g_ci_irq_num = H13_IRQ_DVBCI;
	}

	if (!g_ci_irq_num) {
		CI_SOC_MSG_ERR("CI IRQ Num not defined!\n");
		return NOT_OK;
	}
	CI_SOC_MSG_DBG("CI IRQ Num = %d\n", g_ci_irq_num);

	// for request/free irq sync jinhwan.bae 20140303
	g_ci_irq_sync = (UINT32*)OS_KMalloc(sizeof(UINT32));
	if (g_ci_irq_sync == NULL) {
		CI_SOC_MSG_ERR("NULL : g_ci_irq_sync\n");
		return NOT_OK;
	}

	ret = request_irq(g_ci_irq_num, (irq_handler_t)CI_ISR_Handler , 0, "GPCI", g_ci_irq_sync);
	if (ret) {
		CI_SOC_MSG_ERR("Request IRQ failed!\n");
		return NOT_OK;
	}

	return OK;
}

void CI_Free_Irq(void)
{
	if (g_ci_irq_num) {
		free_irq(g_ci_irq_num, g_ci_irq_sync);
		OS_KFree(g_ci_irq_sync);
		g_ci_irq_num = 0;
		g_ci_irq_sync = NULL;
	}
}

/**
 *	Initialize CICAM and start Irq
 */
SINT32 CI_Initialize( void )
{
#ifdef CI_IO_DELAY_PRINT
	ktime_t start_t, end_t;
#endif
	UINT32	i = 0;

	if (ci_dma_init()) {
		CI_SOC_MSG_ERR("ci_dma_init failed\n");
		return NOT_OK;
	}

	/* CI Delay Constant Set */
	if( lx_chip_rev() >= LX_CHIP_REV(H15, A0) ) {
		for(i=0; i<CI_D_ENUM_MAX; i++) {
			gCI_KDRV_Delay[i] = gDefault_CI_Delay_H15[i];
		}
		pDefault_CI_Delay = (UINT32 *)gDefault_CI_Delay_H15;
		gCI_KDRV_MaxCount_R = MAX_CHECK_COUNT * MAX_CHECK_COUNT_DIV_R;
		gCI_KDRV_MaxCount_W = MAX_CHECK_COUNT * MAX_CHECK_COUNT_DIV_W;
	} else {
		for(i=0; i<CI_D_ENUM_MAX; i++) {
			gCI_KDRV_Delay[i] = gDefault_CI_Delay[i];
		}
		pDefault_CI_Delay = (UINT32 *)gDefault_CI_Delay;
		gCI_KDRV_MaxCount_R = MAX_CHECK_COUNT;
		gCI_KDRV_MaxCount_W = MAX_CHECK_COUNT;
	}
#ifdef CI_IO_DELAY_PRINT
	for(i=0; i<CI_D_ENUM_MAX; i++)
	{
		CI_SOC_MSG_DBG("gCI_KDRV_Delay[%d] = %d msec\n", i, gCI_KDRV_Delay[i]);
		start_t = ktime_get();
		CI_HW_IO_SLEEP(i);
		end_t = ktime_get();
		CI_SOC_MSG_TIME_US(start_t, end_t);
	}
#endif

	gpstCIReg = NULL;
	CiModBaseAddr[MOD_A] = 0;

	gCIChipRev = lx_chip_rev();
	CI_SOC_MSG_DBG("Chip Rev.  = 0x%08x\n", gCIChipRev);

	gCIRegBase = 0;
	gCIRegSize = 0;
	gCIModBase = 0;
	gCIModSize = 0;
#if 1
	if( lx_chip_rev() >= LX_CHIP_REV(H15, A0) ) {
		gCIRegBase = H15_CI_REG_BASE;
		gCIRegSize = 0x40;
		gCIModBase = H15_CI_CAM_BASE;
		gCIModSize = 0x8000;
		if (g_ci_burst_mode) {
		#ifdef _CI_ACCESS_BURST_MODE_IO_
			gCIModSize = 0x8010;
		#endif
		}
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H14, A0) ) {
		gCIRegBase = H14_CI_REG_BASE;
		gCIRegSize = 0x3E;
		gCIModBase = H14_CI_CAM_BASE;
		gCIModSize = 0x8000;
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, B0) ) {
		gCIRegBase = M14_B0_CI_REG_BASE;
		gCIRegSize = 0x3E;
		gCIModBase = M14_B0_CI_CAM_BASE;
		gCIModSize = 0x8000;
		if (g_ci_burst_mode) {
		#ifdef _CI_ACCESS_BURST_MODE_IO_
			gCIModSize = 0x8010;
		#endif
		}
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, A0) ) {
		gCIRegBase = M14_A0_CI_REG_BASE;
		gCIRegSize = 0x3E;
		gCIModBase = M14_A0_CI_CAM_BASE;
		gCIModSize = 0x8000;
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H13, A0) ) {
		gCIRegBase = H13_CI_REG_BASE;
		gCIRegSize = 0x3E;
		gCIModBase = H13_CI_CAM_BASE;
		gCIModSize = 0x8000;
	}
	CI_SOC_MSG_DBG("CI RegBase = 0x%08x\n", gCIRegBase);
	CI_SOC_MSG_DBG("CI RegSize = 0x%08x\n", gCIRegSize);
	CI_SOC_MSG_DBG("CI ModBase = 0x%08x\n", gCIModBase);
	CI_SOC_MSG_DBG("CI ModSize = 0x%08x\n", gCIModSize);
	if (!gCIRegBase) {
		return NOT_OK;
	}

	CI_SOC_MSG_DBG("Mapping - ioremap\n");
	if ((lx_chip_rev() == LX_CHIP_REV(H15, A0)) ||
		(lx_chip_rev() == LX_CHIP_REV(H15, A1)))
	{
		CiModBaseAddr[MOD_A] = (unsigned long)kmalloc(gCIModSize + gCIRegSize, GFP_KERNEL | GFP_DMA);
		gpstCIReg = (S_CI_REG_T *)(CiModBaseAddr[MOD_A] + 0x10000);
		gCIRegBase = gCIModBase;
	} else {
		gpstCIReg = ( S_CI_REG_T* )ioremap(gCIRegBase, gCIRegSize);
		CiModBaseAddr[MOD_A] = (unsigned long)ioremap(gCIModBase, gCIModSize);
	}

	CI_InitSMC();
#else
	if( lx_chip_rev() >= LX_CHIP_REV(H15, A0) ) {
// Set the register area
		gpstCIReg = ( S_CI_REG_T* )ioremap(H15_CI_REG_BASE, 0x3E);
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(H15_CI_CAM_BASE, 0x8000);

		// Control for SMC H14_TBD jinhwan.bae please check the address and functionalities
		CI_InitSMC();
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H14, A0) )
	{
		// Set the register area
		gpstCIReg = ( S_CI_REG_T* )ioremap(H14_CI_REG_BASE, 0x3E);
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(H14_CI_CAM_BASE, 0x8000);

		// Control for SMC H14_TBD jinhwan.bae please check the address and functionalities
		CI_InitSMC();
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, B0) )
	{
		// Set the register area
		gpstCIReg = ( S_CI_REG_T* )ioremap(M14_B0_CI_REG_BASE, 0x3E);
#ifdef _CI_ACCESS_BURST_MODE_IO_
		/* 	jinhwan.bae for IO Read/Write Register
			DATA32_RD(mId)			DWORD(CiModBaseAddr[mId]  + DATA32_REG) */
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(M14_B0_CI_CAM_BASE, 0x8010);
#else
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(M14_B0_CI_CAM_BASE, 0x8000);
#endif

		// Control for SMC M14_TBD jinhwan.bae please check the address and functionalities
		CI_InitSMC();
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(M14, A0) )
	{
		// Set the register area
		gpstCIReg = ( S_CI_REG_T* )ioremap(M14_A0_CI_REG_BASE, 0x3E);
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(M14_A0_CI_CAM_BASE, 0x8000);

		// Control for SMC
		CI_InitSMC();
	}
	else if( lx_chip_rev() >= LX_CHIP_REV(H13, A0) )
	{
		// Set the register area
		gpstCIReg = ( S_CI_REG_T* )ioremap(H13_CI_REG_BASE, 0x3E);
		CiModBaseAddr[MOD_A]	= (unsigned long) ioremap(H13_CI_CAM_BASE, 0x8000);

		// Control for SMC
		CI_InitSMC();

		/* jinhwan.bae 2012. 05. 28 Difference exist from L9. VCC inverter design is differ from L9.
		  * If on, at the time of VCC_SW0 set in Control and Status Register 1, bit 2 VCCEN_N,
		  * Card VCC (5V_CI) is down to 2V, Card is not active, CIS can't be read
		  * Remember H13 Bring Up 													*/
#if 0
		/* release reset */
		ci_writew(0x0000, gpstCIReg->uniCntrlStat1.unCntrlStat1);
		CI_HW_IO_SLEEP(CI_D_INIT_POWER_CONTROL);

		// CTOP Power control (H13)
		CTOP_CTRL_H13A0_RdFL(ctr32);
		CTOP_CTRL_H13A0_Wr01(ctr32, cam_vcc_pol, 0x1);
		CTOP_CTRL_H13A0_WrFL(ctr32);
#endif
	}
#endif

	CI_SOC_MSG_DBG("Mapping : gpstCIReg            = 0x%p\n", gpstCIReg);
	CI_SOC_MSG_DBG("Mapping : CiModBaseAddr[MOD_A] = 0x%lx\n", CiModBaseAddr[MOD_A]);
	if ((gpstCIReg == NULL) || !CiModBaseAddr[MOD_A]) {
		return NOT_OK;
	}

//	CI_RegPrint();

	CI_DefaultInit();

	if (CI_Init_Irq() != OK) {
		return NOT_OK;
	}

	return OK;
}

/**
 *	free Irq
*/
SINT32 CI_UnInitialize( void )
{
	if (gpstCIReg != NULL) {
		CI_Free_Irq();

		if ((lx_chip_rev() == LX_CHIP_REV(H15, A0)) ||
			(lx_chip_rev() == LX_CHIP_REV(H15, A1)))
		{
			kfree((void *)(CiModBaseAddr[MOD_A]));
		//	kfree((void *)(gpstCIReg));
		} else {
			iounmap((void __iomem *)CiModBaseAddr[MOD_A]);
			iounmap((void __iomem *)gpstCIReg);
		}

		CiModBaseAddr[MOD_A] = 0;
		gpstCIReg = NULL;

		ci_dma_uninit();
	}

	return OK;
}

/**
 *	Reset CAM module
*/
SINT32 CI_ResetCI( void )
{
	guwIsPowerRestart = 1;

	return OK;
}

static void _CI_CAM_Off(void)
{
	/* Power Off */
	gCI_CAM_Init_Done = 0;

	ci_writew(0x0000, gpstCIReg->uniCntrlStat1.unCntrlStat1);
#if 0
	CI_PRT_REG_CTRL1(gpstCIReg);
#endif
}

static int _CI_CAM_Init(void)
{
	UINT16 unRegVal = 0x0;

	gCI_HW_IO_Write_Count = 0;

	/* jinhwan.bae 20131018
	    change constant 300 to gCI_KDRV_Delay[CI_D_CAM_INIT_BTW_VCC_CARDRESET] , default 300 */
	//delay 300ms
	CI_HW_IO_SLEEP(CI_D_CAM_INIT_BTW_VCC_CARDRESET);

	/* Power On */
	/*
	 * VCCEN_N = Active-LOW
	 * CTOP power polarity default setting = By-pass
	 * AP2151WG(IC700) EN = Active-High
	 * So, 'unRegVal |= VCC_SW0' generates HIGH signal
	 * and makes power IC enable signal.
	 */
	unRegVal = ci_readw(gpstCIReg->uniCntrlStat1.unCntrlStat1);
	unRegVal |= VCC_SW0;
	ci_writew(unRegVal, gpstCIReg->uniCntrlStat1.unCntrlStat1);

	/* Enable Card */
	CI_HW_IO_SLEEP(CI_D_CAM_INIT_BTW_VCC_CARDRESET);

	unRegVal = ci_readw(gpstCIReg->uniCntrlStat1.unCntrlStat1);
	unRegVal |= (EN_PCCARD | CARD_RESET);
	ci_writew(unRegVal, gpstCIReg->uniCntrlStat1.unCntrlStat1);

	CI_HW_IO_SLEEP(CI_D_CAM_INIT_BTW_CARDRESET_NOTRESET);

	unRegVal &= ~CARD_RESET;
	ci_writew(unRegVal, gpstCIReg->uniCntrlStat1.unCntrlStat1);
#if 0
	CI_PRT_REG_CTRL1(gpstCIReg);
#endif

	CI_ChangeAccessMode(ACCESS_1BYTE_MODE);

	gCI_CAM_Init_Done = 1;

	return OK;
}

/**
 *	CI CAM initialize (Manual HotSwap Sequence)
*/
SINT32 CI_CAMInit( void )
{
	_CI_CAM_Off();

	return _CI_CAM_Init();
}

/**
 *	CAM power off
*/
SINT32 CI_CAMPowerOff( void )
{
	guwIsPowerRestart = 1;

	_CI_CAM_Off();

	return OK;
}

/**
 *	CAM power on
*/
SINT32 CI_CAMPowerOnCompleted( void )
{
	SINT32 wRetVal = OK;

	guwIsPowerRestart = 0;
#if	0
	wRetVal = _CI_CAM_Init();
#endif
	return wRetVal;
}


/**
 *	Check CIS(Card Information Structure)
*/
SINT32 CI_CheckCIS( void )
{
	SINT32 wRetVal = NOT_OK;
	SINT32 ret;

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

#if 0
	if (CI_Check_Init_Reg_Status() != OK) {
		return NOT_OK;
	}
#endif

	/* Set POD mode & PCMCIA attribute memory R/W */
	CI_SOC_MSG_DBG_2("Set POD Mode\n");
	ci_writew(POD_MODE, gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE ATTR\n");
	ci_writew(CHIP_MODE_ATTR, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	ret = CIS_Config(MOD_A);
	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (ret == CIS_OK) {
		CI_SOC_MSG_INFO("Check CIS <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Check CIS <<F>>\n");
	}

	return wRetVal;
}

/**
 *	Write COR(Configuration Option Register)
*/
SINT32 CI_WriteCOR ( void )
{
	SINT32 wRetVal = NOT_OK;

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3\n");
	ci_writew(INIT_VAL, gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE ATTR\n");
	ci_writew(CHIP_MODE_ATTR, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	if (CIS_WriteCOR(MOD_A) == CIS_OK) {
		CI_SOC_MSG_INFO("Wirte COR <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Wirte COR <<F>>\n");
	}

	return wRetVal;
}

/**
 *	Check whether CI module is detect or remove
*/
SINT32 CI_CAMDetectStatus( UINT32 *o_puwIsCardDetected )
{
//	SINT32 wRetVal = NOT_OK;

	if (o_puwIsCardDetected == NULL) {
		CI_DTV_SOC_MSG_INV_ARG();
	//	CI_SOC_MSG_ERR("[ CI_CAMDetectStatus: Invalidate argument <<F>> ]\n");
		return NOT_OK;
	}

	CI_Check_CD_Pol(1, __func__);

#if 0
	if (guwDetectCard) {
		CI_SOC_MSG_DBG("CAM Module is inserted ... \n");
		*o_puwIsCardDetected = 1;
	} else {
	//	CI_SOC_MSG_DBG("CAM Module is removed ... \n");
		*o_puwIsCardDetected = 0;
	}
#else
	*o_puwIsCardDetected = !!guwDetectCard;
	CI_SOC_MSG_DBG("CAM Module is %s ... \n",
					(guwDetectCard)? "inserted" : "removed");
#endif

	return OK;
}

/**
 *	Read data from CI module
*/
SINT32 CI_ReadData( UINT8 *o_pbyData, UINT16 *io_pwDataBufSize, int to_user )
{
	SINT32 wRetVal = NOT_OK;

	if ((o_pbyData == NULL) || (io_pwDataBufSize == NULL)) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	if(HW_IO_Read(MOD_A, o_pbyData, io_pwDataBufSize, to_user) == HW_IO_OK) {
		CI_SOC_MSG_INFO("Read IO Data <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Read IO Data <<F>>\n");
	}

	return wRetVal;
}

/**
 * Negotiate the buffer size between host and CI Module.
*/
SINT32 CI_NegoBuff( UINT32 *o_puwBufSize )
{
	SINT32 wRetVal = NOT_OK;

	if (o_puwBufSize == NULL) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	if (HW_IO_NegoBuf(MOD_A, o_puwBufSize) == HW_IO_OK) {
		CI_SOC_MSG_INFO("Calc Negotiation Buffer <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Calc Negotiation Buffer <<F>>\n");
		*o_puwBufSize = 0;
	}

	return wRetVal;
}

/**
 * Read DA (Data Available register) status
 */
SINT32 CI_ReadDAStatus( UINT32 *o_puwIsDataAvailable, int log_sel )
{
//	SINT32 wRetVal = NOT_OK;

	if (o_puwIsDataAvailable == NULL) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	*o_puwIsDataAvailable = !!(CHECK_DA(MOD_A) & DA_STATUS);
	if (log_sel) {
		CI_SOC_MSG_INFO("DA register status 0x%x\n", *o_puwIsDataAvailable);
	} else {
		CI_SOC_MSG_DBG_2("DA register status 0x%x\n", *o_puwIsDataAvailable);
	}

	return OK;
}

/**
 *	Write data to CI module
*/
SINT32 CI_WriteData( UINT8 *i_pbyData, UINT32 i_wDataBufSize, int from_user )
{
	SINT32 wRetVal = NOT_OK;

	if ((i_pbyData == NULL) || !i_wDataBufSize) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	if (HW_IO_Write(MOD_A, i_pbyData, i_wDataBufSize, from_user) == HW_IO_OK) {
		CI_SOC_MSG_INFO("Write IO Data <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Write IO Data <<F>>\n");
	}

	return wRetVal;
}

/**
 *	Physical Reset
*/
SINT32 CI_ResetPhysicalIntrf( void )
{
//	SINT32 wRetVal = NOT_OK;

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	STAT_WD(MOD_A, RESET_PHY_INT);

	CI_SOC_MSG_INFO("Reset Physical Intrf <<S>>\n");

	return OK;
}

/**
 *	Set RS Bit to soft reset CAM
*/
SINT32 CI_SetRS(void )
{
	SINT32 wRetVal = NOT_OK;

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	if (HW_IO_SetRS(MOD_A) == HW_IO_OK) {
		CI_SOC_MSG_INFO("Set RS <<S>>\n");
		wRetVal = OK;
	} else {
		CI_SOC_MSG_ERR("Set RS <<F>>\n");
	}

	return wRetVal;
}

/**
 *	Read IIR(Initialze Interface Request) status form CI module
*/
SINT32 CI_ReadIIRStatus( UINT32 *o_puwIIRStatus, int log_sel )
{
//	SINT32 wRetVal = NOT_OK;
	volatile UINT8 statusReg;

	if (o_puwIIRStatus == NULL) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	if (!gCI_CAM_Init_Done) {
		CI_DTV_SOC_MSG_NO_INIT();
		return NOT_OK;
	}

	CI_SOC_MSG_DBG_2("Init Control Status 3 : POD_MODE | IO_INT_MODE\n");
	ci_writew((POD_MODE | IO_INT_MODE), gpstCIReg->uniCntrlStat3.unCntrlStat3);
	CI_SOC_MSG_DBG_2("Set MODE IO\n");
	ci_writew(CHIP_MODE_IO, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);
#if 0
	CI_PRT_REG_CTRL3(gpstCIReg);
	CI_PRT_REG_PC_CTRL1(gpstCIReg);
#endif

	statusReg = STAT_RD_IO(MOD_A);
	*o_puwIIRStatus = !!(statusReg & CI_IIR);
//	*o_puwIIRStatus = !!(CHECK_IIR(MOD_A) & IIR_STATUS);
	if (log_sel) {
		CI_SOC_MSG_INFO("Read IIR Status[0x%02X] <<S>>\n", statusReg);
	} else {
		CI_SOC_MSG_DBG_2("Read IIR Status[0x%02X] <<S>>\n", statusReg);
	}

	return OK;
}

/**
 *	Check CAM type
*/
SINT32 CI_CheckCAMType( SINT8 *o_pRtnValue, UINT8 *o_puwCheckCAMType )
{
	SINT32 wRetVal = NOT_OK;

	if ((o_pRtnValue == NULL) || (o_puwCheckCAMType == NULL)) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

	wRetVal = CIS_WhatCAM((UINT8)MOD_A, o_puwCheckCAMType);
	*o_pRtnValue = wRetVal;	// if not process CIS function, return -1

	return wRetVal;
}

/**
 *	Get CI+ Support Version
*/
SINT32 CI_GetCIPlusSupportVersion( SINT8 *o_pRtnValue, UINT32 *o_puwVersion )
{
	SINT32 wRetVal = NOT_OK;

	if ((o_pRtnValue == NULL) || (o_puwVersion == NULL)) {
		CI_DTV_SOC_MSG_INV_ARG();
		CI_SOC_MSG_ERR("CI_GetCIPlusSupportVersion failed\n");
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		CI_SOC_MSG_ERR("CI_GetCIPlusSupportVersion failed\n");
		return NOT_OK;
	}

	wRetVal = CIS_GetCIPlusCAMSupportVersion( (UINT8) MOD_A, o_puwVersion );
	*o_pRtnValue = wRetVal;		// if not process CIS function, return -1

	return wRetVal;
}

/**
 *	Get CI+ Operator Profile
*/
SINT32 CI_GetCIPlusOperatorProfile( SINT8 *o_pRtnValue, UINT32 *o_puwProfile)
{
	SINT32 wRetVal = NOT_OK;

	if ((o_pRtnValue == NULL) || (o_puwProfile == NULL)) {
		CI_DTV_SOC_MSG_INV_ARG();
		CI_SOC_MSG_ERR("CI_GetCIPlusOperatorProfile failed\n");
		return NOT_OK;
	}

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		CI_SOC_MSG_ERR("CI_GetCIPlusOperatorProfile failed\n");
		return NOT_OK;
	}

	wRetVal = CIS_GetCIPlusCAMOperatorProfile( (UINT8) MOD_A, o_puwProfile );
	*o_pRtnValue = wRetVal;		// if not process CIS function, return -1

	return wRetVal;
}

/**
 *	Print CIModule's register
*/
SINT32 CI_RegPrint( void )
{
	SINT32 wRetVal = NOT_OK;

	if( gpstCIReg )
	{
		CI_SOC_MSG_DBG("Reg. Print\n");

		CI_PRT_REG_CTRL1(gpstCIReg);
		CI_PRT_REG_CTRL2(gpstCIReg);
		CI_PRT_REG_CTRL3(gpstCIReg);
		CI_PRT_REG_PC_CTRL1(gpstCIReg);
		CI_PRT_REG_PC_CTRL2(gpstCIReg);
		CI_PRT_REG_PC_CTRL3(gpstCIReg);
		CI_PRT_REG_BURST_CTRL(gpstCIReg);
		CI_PRT_REG_INT_FLAG(gpstCIReg);
		CI_PRT_REG_INT_MASK(gpstCIReg);
		CI_PRT_REG_INT_CTRL(gpstCIReg);
		CI_PRT_REG_PC_ADDR_IDX(gpstCIReg);
		CI_PRT_REG_PC_PON_INTV1(gpstCIReg);
		CI_PRT_REG_PC_PON_INTV2(gpstCIReg);
		CI_PRT_REG_EN_INTV1(gpstCIReg);
		CI_PRT_REG_EN_INTV2(gpstCIReg);
		CI_PRT_REG_CARD_INS_DONE_INTV1(gpstCIReg);
		CI_PRT_REG_CARD_INS_DONE_INTV2(gpstCIReg);
		CI_PRT_REG_CARD_REM_DONE_INTV1(gpstCIReg);
		CI_PRT_REG_CARD_REM_DONE_INTV2(gpstCIReg);

		CI_SOC_MSG_DBG("\n");
		wRetVal = OK;
	}

	return wRetVal;
}

/**
 *	Print CIModule's register
*/
SINT32 CI_RegWrite( UINT32 ui32Offset, UINT32 ui32Value )
{
	SINT32 wRetVal = OK;

	switch(ui32Offset)
	{
		case 0x00 :	{ ci_writew(ui32Value, gpstCIReg->uniCntrlStat1.unCntrlStat1);	break; }
		case 0x02 : { ci_writew(ui32Value, gpstCIReg->uniCntrlStat2.unCntrlStat2);	break; }
		case 0x04 :	{ ci_writew(ui32Value, gpstCIReg->uniCntrlStat3.unCntrlStat3);	break; }
		case 0x06 :	{ ci_writew(ui32Value, gpstCIReg->uniPCCardCntrl1.unPCCardCntrl1);	break; }
		case 0x08 :	{ ci_writew(ui32Value, gpstCIReg->uniPCCardCntrl2.unPCCardCntrl2);	break; }
		case 0x0A :	{ ci_writew(ui32Value, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);	break; }
		//
		case 0x10 :	{ ci_writew(ui32Value, gpstCIReg->uniBurstCntrl.unBurstCntrl);	break; }
		case 0x26 :	{ ci_writew(ui32Value, gpstCIReg->uniIntrFlag.unIntrFlag);	break; }
		case 0x28 :	{ ci_writew(ui32Value, gpstCIReg->uniIntrMask.unIntrMask);	break; }
		case 0x2A :	{ ci_writew(ui32Value, gpstCIReg->uniIntrCntrl.unIntrCntrl);	break; }
		case 0x2C :	{ ci_writew(ui32Value, gpstCIReg->unPCMCIAAddrInd);	break; }
		case 0x2E :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl1.unHWHotSwapCntrl1);	break; }
		//
		case 0x30 :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl2.unHWHotSwapCntrl2);	break; }
		case 0x32 :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl3.unHWHotSwapCntrl3);	break; }
		case 0x34 :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl4.unHWHotSwapCntrl4);	break; }
		case 0x36 :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl5.unHWHotSwapCntrl5);	break; }
		case 0x38 :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl6.unHWHotSwapCntrl6);	break; }
		case 0x3A :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl7.unHWHotSwapCntrl7);	break; }
		case 0x3C :	{ ci_writew(ui32Value, gpstCIReg->uniHWHotSwapCntrl8.unHWHotSwapCntrl8);	break; }
		//
		default	:	{ CI_SOC_MSG_ERR("NO VALID ADDRESS in CI REG (0x%x)\n",ui32Offset);		wRetVal = NOT_OK; 	break;	}

	}

	return wRetVal;
}

/**
 *	SET CI Module's Delay Value
*/
SINT32 CI_CAMSetDelay( CI_DELAY_TYPE_T eDelayType, UINT32 uiDelayValue )
{
	SINT32 wRetVal = NOT_OK;
	UINT32 i = 0;

	if (eDelayType > CI_D_ENUM_MAX) {
		CI_DTV_SOC_MSG_INV_ARG();
		return NOT_OK;
	}

	if (eDelayType == CI_D_ENUM_MAX) {
		/* All Delay Value * 10 ms */
		CI_SOC_MSG_INFO("\nAll Delay Values Are Increased to ORG x uiDelayValue(%d) ms \n", uiDelayValue);

		for (i=0 ; i<CI_D_ENUM_MAX ; i++) {
			gCI_KDRV_Delay[i] = gCI_KDRV_Delay[i] * uiDelayValue;
		}
	} else {
		CI_SOC_MSG_INFO("\nSet the eDelayType[%d] from [%d]ms to [%d]ms\n",eDelayType,gCI_KDRV_Delay[eDelayType],uiDelayValue);
		gCI_KDRV_Delay[eDelayType] = uiDelayValue;
	}

	wRetVal = OK;	/* All Works Done */

	return wRetVal;
}

/**
 *	Print Out CI Module's Delay Value
*/
SINT32 CI_CAMPrintDelayValues( void )
{
	SINT32 wRetVal = NOT_OK;

	CI_SOC_MSG_ERR("CI_D_CIS_CONFIG_FIRST_TUPLE_OK          [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_CONFIG_FIRST_TUPLE_OK], 			gCI_KDRV_Delay[CI_D_CIS_CONFIG_FIRST_TUPLE_OK]);
	CI_SOC_MSG_ERR("CI_D_CIS_CONFIG_FIRST_TUPLE_NG          [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_CONFIG_FIRST_TUPLE_NG], 			gCI_KDRV_Delay[CI_D_CIS_CONFIG_FIRST_TUPLE_NG]);
	CI_SOC_MSG_ERR("CI_D_CIS_END_WRITE_COR                  [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_END_WRITE_COR], 					gCI_KDRV_Delay[CI_D_CIS_END_WRITE_COR]);
	CI_SOC_MSG_ERR("CI_D_CIS_DURING_READ_TUPLE              [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_DURING_READ_TUPLE], 				gCI_KDRV_Delay[CI_D_CIS_DURING_READ_TUPLE]);
	CI_SOC_MSG_ERR("CI_D_CIS_END_READ_TUPLE_INITIAL         [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_END_READ_TUPLE_INITIAL], 		gCI_KDRV_Delay[CI_D_CIS_END_READ_TUPLE_INITIAL]);
	CI_SOC_MSG_ERR("CI_D_CIS_PARSE_NON_CI_TUPLE             [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CIS_PARSE_NON_CI_TUPLE], 			gCI_KDRV_Delay[CI_D_CIS_PARSE_NON_CI_TUPLE]);
	CI_SOC_MSG_ERR("CI_D_INIT_POWER_CONTROL                 [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_INIT_POWER_CONTROL], 				gCI_KDRV_Delay[CI_D_INIT_POWER_CONTROL]);
	CI_SOC_MSG_ERR("CI_D_INIT_AFTER_INTERRUPT_ENABLE        [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_INIT_AFTER_INTERRUPT_ENABLE], 		gCI_KDRV_Delay[CI_D_INIT_AFTER_INTERRUPT_ENABLE]);
	CI_SOC_MSG_ERR("CI_D_CAM_INIT_BTW_VCC_CARDRESET         [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CAM_INIT_BTW_VCC_CARDRESET], 		gCI_KDRV_Delay[CI_D_CAM_INIT_BTW_VCC_CARDRESET]);
	CI_SOC_MSG_ERR("CI_D_CAM_INIT_BTW_CARDRESET_NOTRESET    [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_CAM_INIT_BTW_CARDRESET_NOTRESET], 	gCI_KDRV_Delay[CI_D_CAM_INIT_BTW_CARDRESET_NOTRESET]);
	CI_SOC_MSG_ERR("CI_D_IO_SOFT_RESET_CHECK_FR             [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_SOFT_RESET_CHECK_FR], 			gCI_KDRV_Delay[CI_D_IO_SOFT_RESET_CHECK_FR]);
	CI_SOC_MSG_ERR("CI_D_IO_END_SOFT_RESET                  [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_END_SOFT_RESET], 				gCI_KDRV_Delay[CI_D_IO_END_SOFT_RESET]);
	CI_SOC_MSG_ERR("CI_D_IO_NEGOBUF_BEFORE_SOFTRESET        [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_NEGOBUF_BEFORE_SOFTRESET], 		gCI_KDRV_Delay[CI_D_IO_NEGOBUF_BEFORE_SOFTRESET]);
	CI_SOC_MSG_ERR("CI_D_IO_NEGOBUF_CHECK_DA                [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_NEGOBUF_CHECK_DA], 				gCI_KDRV_Delay[CI_D_IO_NEGOBUF_CHECK_DA]);
	CI_SOC_MSG_ERR("CI_D_IO_NEGOBUF_CHECK_FR                [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_NEGOBUF_CHECK_FR], 				gCI_KDRV_Delay[CI_D_IO_NEGOBUF_CHECK_FR]);
	CI_SOC_MSG_ERR("CI_D_IO_NEGOBUF_AFTER_WRITE_DATA        [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_NEGOBUF_AFTER_WRITE_DATA], 		gCI_KDRV_Delay[CI_D_IO_NEGOBUF_AFTER_WRITE_DATA]);
	CI_SOC_MSG_ERR("CI_D_IO_READ_CHECK_DA                   [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_READ_CHECK_DA], 					gCI_KDRV_Delay[CI_D_IO_READ_CHECK_DA]);
	CI_SOC_MSG_ERR("CI_D_IO_WRITE_CHECK_DA                  [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_WRITE_CHECK_DA], 					gCI_KDRV_Delay[CI_D_IO_WRITE_CHECK_DA]);
	CI_SOC_MSG_ERR("CI_D_IO_WRITE_CHECK_FR                  [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_WRITE_CHECK_FR], 					gCI_KDRV_Delay[CI_D_IO_WRITE_CHECK_FR]);
	CI_SOC_MSG_ERR("CI_D_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE  [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE],	gCI_KDRV_Delay[CI_D_IO_WRITE_FIRST_BYTE_STAT_RD_FR_WE]);
	CI_SOC_MSG_ERR("CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE      [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE], 		gCI_KDRV_Delay[CI_D_IO_WRITE_MIDDLE_BYTE_CHECK_WE]);
	CI_SOC_MSG_ERR("CI_D_IO_WRITE_LAST_BYTE_CHECK_WE        [%4d][%4d]\n",
		pDefault_CI_Delay[CI_D_IO_WRITE_LAST_BYTE_CHECK_WE], 		gCI_KDRV_Delay[CI_D_IO_WRITE_LAST_BYTE_CHECK_WE]);

	wRetVal = OK;	/* All Works Done */

	return wRetVal;
}

/**
 *	Set PCMCIA PC Card Bus Timing Fast or Slow
*/
SINT32 CI_SetPCCardBusTiming( CI_BUS_SPEED_T speed )
{
	SINT32 wRetVal = NOT_OK;

	CI_SOC_MSG_INFO("SET PCMCIA PC Card Bus Timing for CI (%x) \n", (UINT32)speed);

	switch( speed )
	{
		case PCMCIA_BUS_SPEED_MIN :
		case PCMCIA_BUS_SPEED_LOW :
		{
			ci_writew(0x0F03, gpstCIReg->uniPCCardCntrl2.unPCCardCntrl2);
			ci_writew(0x0003, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);

			break;
		}
		case PCMCIA_BUS_SPEED_HIGH :
		case PCMCIA_BUS_SPEED_MAX :
		{
			//change the register from 0x101 to 0x201 (20130218, ohsung.roh)
			ci_writew(0x0201, gpstCIReg->uniPCCardCntrl2.unPCCardCntrl2);
			ci_writew(0x0001, gpstCIReg->uniPCCardCntrl3.unPCCardCntrl3);

			break;
		}
		default : break;
	}

	wRetVal = OK;	/* All Works Done */

	return wRetVal;
}

static UINT16 BurstCntrlVal[] = {
	[ACCESS_1BYTE_MODE]	= 0x00,
	[ACCESS_2BYTE_MODE]	= 0x01,
	[ACCESS_4BYTE_MODE]	= 0x03,
};

SINT32 CI_ChangeAccessMode ( CI_ACCESS_MODE_T mode )
{
	SINT32 wRetVal = NOT_OK;

	CI_SOC_MSG_INFO("SET CI_ChangeAccessMode for CI (%x) \n", (UINT32)mode);

#if 1
	if (mode <= ACCESS_4BYTE_MODE) {
		ci_writew(BurstCntrlVal[mode], gpstCIReg->uniBurstCntrl.unBurstCntrl);
	}
#else
	switch( mode )
	{
		case ACCESS_1BYTE_MODE :
		{
			ci_writew(0x0000, gpstCIReg->uniBurstCntrl.unBurstCntrl);
			break;
		}
		case ACCESS_2BYTE_MODE :
		{
			ci_writew(0x0001, gpstCIReg->uniBurstCntrl.unBurstCntrl);
			break;
		}
		case ACCESS_4BYTE_MODE :
		{
			ci_writew(0x0003, gpstCIReg->uniBurstCntrl.unBurstCntrl);
			break;
		}
		default :
			break;
	}
#endif

	wRetVal = OK;	/* All Works Done */

	return wRetVal;
}

#if defined(CI_DATA_TEST_ON)
/*
 * [Caution] CI_DATA_TEST_ID
 * In Viaccess CAM, the 1st ID for IO access don't need to be 0x01.
 * But, in ASTRA CI plus CAM, the 1st ID is supposed to be 0x01.
 */
#define CI_DATA_TEST_ID		0x01	//0x01 ~ 0xFF

#define CI_TEST_BUF_SIZE	4096
#define CI_TEST_DATA_SIZE	80

#define CI_DATA_TEST_LOOP		1000

//#define CI_PRINT_IO_DATA
//#define CI_CMP_IO_DATA

struct CI_TEST_DATA {
	UINT32	size;
	UINT8	data[CI_TEST_DATA_SIZE];
};
typedef struct CI_TEST_DATA CI_TEST_DATA_T;

#if defined(CI_CMP_IO_DATA)
static const CI_TEST_DATA_T g_ci_write_vector[] = {
	[0] = {  5, { CI_DATA_TEST_ID, 0x00, 0x82, 0x01, 0x01, } },		//Creaet_T_C
	[1] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },		//Data_last
	[2] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },		//RCV
	[3] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
#if 0
	[4] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x01, 0x00, 0x41, 0x00, 0x01, } },
	[5] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x01, 0x9F, 0x80, 0x10, 0x00, } },
#endif
};

/*
static const CI_TEST_DATA_T g_ci_read_vector[] = {
	[0] = {  9, CI_DATA_TEST_ID, 0x00, 0x83, 0x01, 0x01, 0x80, 0x02, 0x01, 0x00, },
	[1] = {  6, CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x80, },
	[2] = { 15, CI_DATA_TEST_ID, 0x00, 0x0A0, 0x07, 0x01, 0x91, 0x04, 0x00, 0x01, 0x00, 0x41, 0x80, 0x02, 0x01, 0x00, },
	[3] = {  6, CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, },
	[4] = {  6, CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, },
	[5] = {  6, CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, },
};
*/
static const CI_TEST_DATA_T g_ci_read_vector[] = {
	[0] = {  9, { CI_DATA_TEST_ID, 0x00, 0x83, 0x01, 0x01, 0x80, 0x02, 0x01, 0x00, } },
	[1] = {  6, { CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, } },
	[2] = {  6, { CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, } },
	[3] = {  6, { CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, } },
	[4] = {  6, { CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, } },
	[5] = {  6, { CI_DATA_TEST_ID, 0x00, 0x80, 0x02, 0x01, 0x00, } },
};
#else
static const CI_TEST_DATA_T g_ci_write_vector[] = {
	[0] = {  5, { CI_DATA_TEST_ID, 0x00, 0x82, 0x01, 0x01, } },		//Creaet_T_C
	[1] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },		//Data_last
	[2] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },		//RCV
	[3] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
#if 0
	[4] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x01, 0x00, 0x41, 0x00, 0x01, } },
	[5] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x01, 0x9F, 0x80, 0x10, 0x00, } },
	[6] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[7] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[8] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[9] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x01, 0x9F, 0x80, 0x12, 0x00, } },
	[10] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[11] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[12] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[13] = { 69, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x41, 0x01, 0x90, 0x02, 0x00, 0x01, 0x9F, 0x80, 0x11, 0x38, 0x00, 0x01, 0x00, 0x41, 0x00, 0x02, 0x00, 0x43, 0x00, 0x03, 0x00, 0x41, 0x00, 0x20, 0x00, 0x42, 0x00, 0x24, 0x00, 0x41, 0x00, 0x10, 0x00, 0x41, 0x00, 0x40, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41, 0x00, 0x8D, 0x10, 0x01, 0x00, 0x8E, 0x10, 0x01, 0x00, 0x8C, 0x10, 0x02, 0x00, 0x60, 0x60, 0x03, 0x00, 0x8F, 0x10, 0x01, 0x00, 0x96, 0x10, 0x01, } },
	[14] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[15] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[16] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[17] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[18] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[19] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[20] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x8D, 0x10, 0x01, 0x00, 0x02, } },
	[21] = { 16, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0C, 0x01, 0x90, 0x02, 0x00, 0x02, 0x9F, 0x81, 0x11, 0x03, 0x65, 0x6E, 0x67, } },
	[22] = { 16, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0C, 0x01, 0x90, 0x02, 0x00, 0x02, 0x9F, 0x81, 0x01, 0x03, 0x47, 0x42, 0x52, } },
	[23] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x24, 0x00, 0x41, 0x00, 0x03, } },
	[24] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x20, 0x00, 0x42, 0x00, 0x04, } },
	[25] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x02, 0x00, 0x43, 0x00, 0x05, } },
	[26] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x05, 0x9F, 0x80, 0x20, 0x00, } },
	[27] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x40, 0x00, 0x41, 0x00, 0x06, } },
	[28] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[29] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[30] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[31] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[32] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[33] = { 15, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0B, 0x01, 0x90, 0x02, 0x00, 0x06, 0x9F, 0x88, 0x02, 0x02, 0x01, 0x01, } },
	[34] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x8C, 0x10, 0x02, 0x00, 0x07, } },
	[35] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x07, 0x9F, 0x90, 0x11, 0x00, } },
	[36] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[37] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x92, 0x07, 0x00, 0x00, 0x03, 0x00, 0x41, 0x00, 0x08, } },
	[38] = { 13, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x09, 0x01, 0x90, 0x02, 0x00, 0x08, 0x9F, 0x80, 0x30, 0x00, } },
	[39] = { 14, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x0A, 0x01, 0x90, 0x02, 0x00, 0x07, 0x9F, 0x90, 0x02, 0x01, 0x01, } },
	[40] = {  5, { CI_DATA_TEST_ID, 0x00, 0x81, 0x01, 0x01, } },
	[41] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
	[42] = { 52, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x30, 0x01, 0x90, 0x02, 0x00, 0x08, 0x9F, 0x80, 0x32, 0x27, 0x03, 0x27, 0x77, 0x0B, 0x00, 0x17, 0x01, 0x09, 0x14, 0x18, 0x30, 0xF0, 0x67, 0x86, 0x0E, 0x00, 0x80, 0x11, 0x0F, 0x9F, 0x20, 0xFC, 0x01, 0x80, 0x11, 0x0D, 0x7A, 0x20, 0xFC, 0x1B, 0x03, 0xFF, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00, } },
	[43] = {  5, { CI_DATA_TEST_ID, 0x00, 0xA0, 0x01, 0x01, } },
#endif
};
#endif

ktime_t g_read_t, g_write_t;
UINT32 g_read_size, g_write_size;

SINT32 CI_DataTest(MOD_ID mId)
{
	UINT8 *test_buf = NULL;
	UINT16 read_size;
	int loop;
	int i, j;
	int ret;
	UINT32 chkCnt;
	UINT32 read_status;
	SINT32 wRetVal = OK;

	if (!guwDetectCard) {
		CI_DTV_SOC_MSG_NO_CAM();
		return NOT_OK;
	}

//	return ci_dma_test();

	test_buf = OS_KMalloc(CI_TEST_BUF_SIZE);
	if (test_buf == NULL) {
		CI_SOC_MSG_ERR("Null test buf\n");
		return NOT_OK;
	}

	memset(&g_read_t, 0x00, sizeof(g_read_t));
	memset(&g_write_t, 0x00, sizeof(g_write_t));
	g_read_size = 0;
	g_write_size = 0;

	CI_SOC_MSG_NOTI("<<CI Data Pattern Test : %d pattern RW x %d time\n",
				ARRAY_SIZE(g_ci_write_vector),
				CI_DATA_TEST_LOOP);

	for (loop=0 ; loop<CI_DATA_TEST_LOOP ; loop++) {
		for (i=0 ; i<ARRAY_SIZE(g_ci_write_vector) ; i++) {
			CI_SOC_MSG_NOTI("<<Test Loop[%d] - Vector[%d]\n", loop, i);

		#if	defined(CI_PRINT_IO_DATA)
			CI_SOC_MSG_DBG("Send Data(%d): \n", g_ci_write_vector[i].size);
			for (j=0 ; j<g_ci_write_vector[i].size ; j++) {
				CI_SOC_MSG_DBG("0x%02X ", g_ci_write_vector[i].data[j]);
			}
			CI_SOC_MSG_DBG("\n");
		#endif

			ret = CI_WriteData((UINT8 *)g_ci_write_vector[i].data, g_ci_write_vector[i].size, 0);
			g_write_size += g_ci_write_vector[i].size;
			if (ret != HW_IO_OK) {
				CI_SOC_MSG_ERR("CI_WriteData failed : %d\n", ret);
				wRetVal = NOT_OK;
				goto out;
			}

			CI_HW_IO_SLEEP(CI_D_IO_READ_CHECK_DA);

			chkCnt = MAX_CHECK_COUNT;
			while (chkCnt--) {
			#if 0
				read_status = 1;
				ret = CI_ReadIIRStatus(&read_status, 0);
				if (ret) {
					CI_SOC_MSG_ERR("CI_ReadIIRStatus failed : %d\n", ret);
					wRetVal = NOT_OK;
					goto out;
				}
				if (!read_status) {
					CI_HW_IO_SLEEP(CI_D_IO_READ_CHECK_DA);
					continue;
				}
			#endif

				read_status = 0;
				ret = CI_ReadDAStatus(&read_status, 0);
				if (ret) {
					CI_SOC_MSG_ERR("CI_ReadDAStatus failed : %d\n", ret);
					wRetVal = NOT_OK;
					goto out;
				}
				if (read_status) {
				//	CI_SOC_MSG_DBG("Data avaliable\n");
					break;
				}
				CI_HW_IO_SLEEP(CI_D_IO_READ_CHECK_DA);
			}
			if (!chkCnt) {
				CI_SOC_MSG_ERR("DA Wait Timeout\n");
				wRetVal = NOT_OK;
				goto out;
			}

			CI_HW_IO_SLEEP(CI_D_IO_READ_CHECK_DA);

			read_size = CI_TEST_BUF_SIZE;
			ret = CI_ReadData((UINT8 *)test_buf, &read_size, 0);
			g_read_size += read_size;

		#if	0//defined(CI_CMP_IO_DATA)
			if (read_size) {
				int chk_size = g_ci_read_vector[i].size;
				if ((ret != HW_IO_OK) ||
					(chk_size != read_size) ||
					memcmp((void *)g_ci_read_vector[i].data, (void *)test_buf, chk_size)) {
					int j;

					CI_SOC_MSG_DBG("HW_IO_Read: Data different\n");
					CI_SOC_MSG_DBG("HW_IO_Read: chk_size=%d, size=%d\n", chk_size, read_size);
					CI_SOC_MSG_DBG("[Read Vector]\n");
					for (j=0 ; j<chk_size ; j++) {
						CI_SOC_MSG_DBG(" 0x%02x", g_ci_read_vector[i].data[j]);
					}
					CI_SOC_MSG_DBG("\n");
					CI_SOC_MSG_DBG("[Test Buffer]\n");
					for (j=0 ; j<chk_size ; j++) {
						CI_SOC_MSG_DBG(" 0x%02x", test_buf[j]);
					}
					CI_SOC_MSG_DBG("\n");
					wRetVal = NOT_OK;
					goto out;
				}
			}

			if (ret != HW_IO_OK) {
				CI_SOC_MSG_ERR("CI_ReadData failed : %d\n", ret);
				wRetVal = NOT_OK;
				goto out;
			}
		#else
			if (!read_size || (ret != HW_IO_OK)) {
				CI_SOC_MSG_ERR("CI_ReadData(%d) failed : %d\n", read_size, ret);
				wRetVal = NOT_OK;
				goto out;
			}
			{
			#if defined(CI_PRINT_IO_DATA)
				CI_SOC_MSG_DBG("Read Data(%d): \n", read_size);
				for (j=0 ; j<read_size ; j++) {
					CI_SOC_MSG_DBG("0x%02X ", ((u8 *)test_buf)[j]);
				}
				CI_SOC_MSG_DBG("\n");
			#endif
			}
		#endif

		#if 0
			if (i==2) {
				if (read_size == 6) {
					i--;
				}
			}
		#endif
		}
	}

	CI_SOC_MSG_NOTI("total write size[%d], total elapsed[%lld us]\n",
					g_write_size, ktime_to_us(g_write_t));
	CI_SOC_MSG_NOTI("total read size[%d], total elapsed[%lld us]\n",
					g_read_size, ktime_to_us(g_read_t));

out:
	if (test_buf != NULL) {
		OS_Free(test_buf);
	}
	return wRetVal;
}
#endif	/* CI_DATA_TEST_ON */

