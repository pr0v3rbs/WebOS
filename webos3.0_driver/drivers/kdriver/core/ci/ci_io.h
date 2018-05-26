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

#ifndef	_CI_IO_H_
#define	_CI_IO_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/types.h>

//#include <asm/byteorder.h>
//#include <asm/barrier.h>
//#include <asm/pgtable.h>

#include "ci_defs.h"
#include "ci_regdefs.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define HW_IO_OK			0
#define HW_IO_BUSY			1
#define HW_IO_FAIL			(-1)
#define MEM_ALLOC_FAILED	(-2)

#define	DATA_REG			0x00000000
#define	DATA32_REG			0x00008000		//for burst mode (32bit)
#define	COMM_REG			0x00000002
#define	STAT_REG			0x00000002
#define	SIZE_REG_LS			0x00000004
#define	SIZE_REG_MS			0x00000006

/* status register bit definitions */
#define	CI_RE				0x01
#define	CI_WE				0x02
#define	CI_FR				0x40
#define	CI_DA				0x80
#define CI_IIR				0x10 /* for CI plus */

/* command register bit definitions	*/
#define	CI_HC				0x01
#define	CI_SW				0x02
#define	CI_SR				0x04
#define	CI_RS				0x08 // avoid collision with RS in "dtv_msgs.h"
#define	CI_FRIE				0x40
#define	CI_DAIE				0x80

/* host & module buffer size */
#define HOST_BUF_SIZE		65535
#define	MOD_BUF_SIZE_MIN	16
#define	MOD_BUF_SIZE_MAX	65535

#define MAX_CHECK_COUNT		500

//#define MAX_POSS_BUF_SIZE	1000
#define MAX_POSS_BUF_SIZE	4096

/* maximum number of module	*/
//#define MAX_MOD_NUM		2
//#define MAX_MOD_NUM		1

/* IO register access macro */

struct ci_io_ops {
	unsigned int	opt_barrier;
	unsigned int	opt_rw_core;
	UINT16			(*readw)(volatile void *addr);
	void			(*writew)(UINT16 val16, volatile void *addr);
	UINT8			(*cam_readb)(volatile void *addr);
	UINT16			(*cam_readw)(volatile void *addr);
	UINT32			(*cam_readl)(volatile void *addr);
	void			(*cam_writeb)(UINT8 val8, volatile void *addr);
	void			(*cam_writew)(UINT16 val16, volatile void *addr);
	void			(*cam_writel)(UINT32 val32, volatile void *addr);
	//
	void			(*rmb)(void);
	void			(*wmb)(void);
};

extern struct ci_io_ops g_ci_io_ops;

#define _ci_readw(_r)				(g_ci_io_ops.readw)(&(_r))
#define _ci_writew(_v, _r)			(g_ci_io_ops.writew)(_v, &(_r))
#define _ci_cam_readb(_a)			(g_ci_io_ops.cam_readb)((volatile void *)(_a))
#define _ci_cam_readw(_a)			(g_ci_io_ops.cam_readw)((volatile void *)(_a))
#define _ci_cam_readl(_a)			(g_ci_io_ops.cam_readl)((volatile void *)(_a))
#define _ci_cam_writeb(_v, _a)		(g_ci_io_ops.cam_writeb)(_v, (volatile void *)(_a))
#define _ci_cam_writew(_v, _a)		(g_ci_io_ops.cam_writew)(_v, (volatile void *)(_a))
#define _ci_cam_writel(_v, _a)		(g_ci_io_ops.cam_writel)(_v, (volatile void *)(_a))

//

#define ci_readw(_r)				_ci_readw(_r)
#define ci_writew(_v, _r)			_ci_writew(_v, _r)

#define BYTE(addr)					_ci_cam_readb(addr)
#define WORD(addr)					_ci_cam_readw(addr)		/* Burst mode */
#define DWORD(addr)					_ci_cam_readl(addr)		/* Burst mode */
#define CONFIG_REG(addr)			_ci_cam_readb(addr)

#define STAT_RD(mId)				_ci_cam_readb((CiModBaseAddr[mId] + STAT_REG))
#define STAT_RD_IO(mId)				STAT_RD(mId)
#define	DATA_RD(mId)				_ci_cam_readb((CiModBaseAddr[mId] + DATA_REG))
#define	DATA_RD_IO(mId)				DATA_RD(mId)
#define	SIZE_LS_RD(mId)				_ci_cam_readb((CiModBaseAddr[mId] + SIZE_REG_LS))
#define	SIZE_MS_RD(mId)				_ci_cam_readb((CiModBaseAddr[mId] + SIZE_REG_MS))

#define	COMM_WR(mId, val)			_ci_cam_writeb(val, (CiModBaseAddr[mId] + COMM_REG))
#define STAT_WD(mId, val)			_ci_cam_writeb(val, (CiModBaseAddr[mId] + STAT_REG))
#define	DATA_WR(mId, val)			_ci_cam_writeb(val, (CiModBaseAddr[mId] + DATA_REG))
#define DATA_WR_IO(mId, val)		DATA_WR(mId, val)
#define	SIZE_LS_WR(mId, val)		_ci_cam_writeb(val, (CiModBaseAddr[mId] + SIZE_REG_LS))
#define	SIZE_MS_WR(mId, val)		_ci_cam_writeb(val, (CiModBaseAddr[mId] + SIZE_REG_MS))

#define	DATA32_RD(mId)				_ci_cam_readl((CiModBaseAddr[mId] + DATA32_REG))
#define	DATA32_WR(mId, val)			_ci_cam_writel(val, (CiModBaseAddr[mId] + DATA32_REG))

#define CHECK_DA(mId)				(STAT_RD_IO(mId)&CI_DA)
#define CHECK_FR(mId)				(STAT_RD_IO(mId)&CI_FR)
#define CHECK_RE(mId)				(STAT_RD_IO(mId)&CI_RE)
#define CHECK_WE(mId)				(STAT_RD_IO(mId)&CI_WE)

/* for CI plus */
#define CHECK_IIR(mId)				(STAT_RD_IO(mId)&CI_IIR)

#define STRING_ENTER
//#define STRING_ENTER	"\n"

#define CI_DTV_SOC_Message(_idx, format, args...) 	DBG_PRINT(g_ci_debug_fd, _idx, format STRING_ENTER, ##args)
#define LX_IS_ERR(err)  ((err) != (0) ? (1) : (0))

#if 0 // before '13 logm jinhwan.bae
//log level
#define CI_DBG_INFO			0	/* 0x00000001  */
#define CI_INFO				1	/* 0x00000002  */
#define CI_ERR				2	/* 0x00000004  */
#define CIS_INFO			3	/* 0x00000008  */
#define CIS_TUPLE_DUMP		4	/* 0x00000010  */
#define CIS_PARSE_DUMP		5	/* 0x00000020 */
#define CI_IO_INFO			6	/* 0x00000040  */
#define CIS_CIPLUS			7	/* 0x00000080 */
#define CIS_ERR				8	/* 0x00000100 */
#else
#define CI_ERR				LX_LOGM_LEVEL_ERROR			/* 0 */
#define CI_WARNING			LX_LOGM_LEVEL_WARNING		/* 1 */
#define CI_NOTI				LX_LOGM_LEVEL_NOTI			/* 2 */
#define CI_INFO				LX_LOGM_LEVEL_INFO			/* 3 */
//
#define CI_DEBUG			LX_LOGM_LEVEL_DEBUG			/* 4 */
#define CI_TRACE			LX_LOGM_LEVEL_TRACE			/* 5 */
#define CI_DRV				(LX_LOGM_LEVEL_TRACE + 1)	/* 6 */
#define CI_NORMAL			(LX_LOGM_LEVEL_TRACE + 2)	/* 7 */
//
#define CI_ISR				(LX_LOGM_LEVEL_TRACE + 3)	/* 8 */
#define CI_DBG_INFO			(LX_LOGM_LEVEL_TRACE + 4)	/* 9 */
#define CIS_INFO			(LX_LOGM_LEVEL_TRACE + 5)	/* 10 */
#define CIS_TUPLE_DUMP		(LX_LOGM_LEVEL_TRACE + 6)	/* 11 */
//
#define CIS_PARSE_DUMP		(LX_LOGM_LEVEL_TRACE + 7)	/* 12 */
#define CI_IO_INFO			(LX_LOGM_LEVEL_TRACE + 8)	/* 13 */
#define CIS_CIPLUS			(LX_LOGM_LEVEL_TRACE + 9)	/* 14 */
#define CIS_ERR				(LX_LOGM_LEVEL_TRACE + 10)	/* 15 */
//
#define CI_DBG_INFO_2		(LX_LOGM_LEVEL_TRACE + 11)	/* 16 */
#define CI_DBG_INFO_3		(LX_LOGM_LEVEL_TRACE + 12)	/* 17 */
#define CIS_TUPLE_DUMP_2	(LX_LOGM_LEVEL_TRACE + 13)	/* 18 */
#define CIS_PARSE_DUMP_2	(LX_LOGM_LEVEL_TRACE + 14)	/* 19 */
//
#define CI_ERR_2			(LX_LOGM_LEVEL_TRACE + 15)	/* 20 */
#endif

/*
 *
 */
#define CI_SOC_MSG_ERR(format, args...)		\
		CI_DTV_SOC_Message(CI_ERR, format, ##args)
#define CI_SOC_MSG_WARN(format, args...)	\
		CI_DTV_SOC_Message(CI_WARNING, format, ##args)
#define CI_SOC_MSG_NOTI(format, args...)	\
		CI_DTV_SOC_Message(CI_NOTI, format, ##args)
#define CI_SOC_MSG_INFO(format, args...)	\
		CI_DTV_SOC_Message(CI_INFO, format, ##args)
#define CI_SOC_MSG_DEBUG(format, args...)	\
		CI_DTV_SOC_Message(CI_DEBUG, format, ##args)
#define CI_SOC_MSG_TRACE(format, args...)	\
		CI_DTV_SOC_Message(CI_TRACE, format, ##args)

#define CI_SOC_MSG_DRV(format, args...)		\
		CI_DTV_SOC_Message(CI_DRV, format, ##args)
#define CI_SOC_MSG_NORM(format, args...)	\
		CI_DTV_SOC_Message(CI_NORMAL, format, ##args)
#define CI_SOC_MSG_ISR(format, args...)		\
		CI_DTV_SOC_Message(CI_ISR, format, ##args)

#define CI_SOC_MSG_DBG(format, args...)		\
		CI_DTV_SOC_Message(CI_DBG_INFO, format, ##args)
#define CI_SOC_MSG_DBG_2(format, args...)	\
		CI_DTV_SOC_Message(CI_DBG_INFO_2, format, ##args)
#define CI_SOC_MSG_DBG_3(format, args...)	\
		CI_DTV_SOC_Message(CI_DBG_INFO_3, format, ##args)

#define CI_SOC_MSG_CIS_INFO(format, args...)	\
		CI_DTV_SOC_Message(CIS_INFO, format, ##args)
#define CI_SOC_MSG_CIS_TUPLE(format, args...)	\
		CI_DTV_SOC_Message(CIS_TUPLE_DUMP, format, ##args)
#define CI_SOC_MSG_CIS_PARSE(format, args...)	\
		CI_DTV_SOC_Message(CIS_PARSE_DUMP, format, ##args)
#define CI_SOC_MSG_IO_INFO(format, args...)		\
		CI_DTV_SOC_Message(CI_IO_INFO, format, ##args)
#define CI_SOC_MSG_CIS_CIPLUS(format, args...)	\
		CI_DTV_SOC_Message(CIS_CIPLUS, format, ##args)
#define CI_SOC_MSG_CIS_ERR(format, args...)		\
		CI_DTV_SOC_Message(CIS_ERR, format, ##args)

#define CI_SOC_MSG_CIS_TUPLE_2(format, args...)	\
		CI_DTV_SOC_Message(CIS_TUPLE_DUMP_2, format, ##args)
#define CI_SOC_MSG_CIS_PARSE_2(format, args...)	\
		CI_DTV_SOC_Message(CIS_PARSE_DUMP_2, format, ##args)

#define CI_SOC_MSG_ERR_2(format, args...)	\
		CI_DTV_SOC_Message(CI_ERR_2, format, ##args)

#define	CI_SOC_MSG_TIME_US(_start, _end)			\
		do {									\
			CI_SOC_MSG_DBG("start[%lld us], end[%lld us], elapsed[%lld us]\n",	\
						ktime_to_us(_start), ktime_to_us(_end),		\
						ktime_to_us(ktime_sub(_end, _start)));		\
		} while(0)

#define	CI_SOC_MSG_TIME_MS(_start, _end)			\
		do {									\
			CI_SOC_MSG_DBG("start[%lld ms], end[%lld ms], elapsed[%lld ms]\n",	\
						ktime_to_ms(_start), ktime_to_ms(_end),		\
						ktime_to_ms(ktime_sub(_end, _start)));		\
		} while(0)

#define	CI_SOC_MSG_TIME_US_IO(_size, _start, _end)			\
		do {									\
			CI_SOC_MSG_NOTI("size[%d] - start[%lld us], end[%lld us], elapsed[%lld us] %s\n",	\
						_size,										\
						ktime_to_us(_start), ktime_to_us(_end),		\
						ktime_to_us(ktime_sub(_end, _start)),		\
						(_size)? "<<S>>" : "<<F>>");				\
		} while(0)

/*
 *
 */
#define CI_DTV_SOC_MSG_INV_ARG()	\
		CI_SOC_MSG_ERR("[ Invalid argument <<F>> ]\n")
#define CI_DTV_SOC_MSG_NO_CAM()		\
		CI_SOC_MSG_ERR("[ CAM not inserted <<F>> ]\n")
#define CI_DTV_SOC_MSG_NO_INIT()		\
		CI_SOC_MSG_ERR_2("[ CAM Init Required <<F>> ]\n")
/*
 *
 */
#define CI_PRT_REG_MESSAGE(format, args...)		\
		CI_SOC_MSG_ERR(format, ##args)

#define CI_PRT_REG_CTRL1(_reg)	\
		CI_PRT_REG_MESSAGE("Control and Status 1         [0x%04x]", ci_readw((_reg)->uniCntrlStat1.unCntrlStat1))
#define CI_PRT_REG_CTRL2(_reg)	\
		CI_PRT_REG_MESSAGE("Control and Status 2         [0x%04x]", ci_readw((_reg)->uniCntrlStat2.unCntrlStat2))
#define CI_PRT_REG_CTRL3(_reg)	\
		CI_PRT_REG_MESSAGE("Control and Status 3         [0x%04x]", ci_readw((_reg)->uniCntrlStat3.unCntrlStat3))
#define CI_PRT_REG_PC_CTRL1(_reg)	\
		CI_PRT_REG_MESSAGE("PC Card Control 1            [0x%04x]", ci_readw((_reg)->uniPCCardCntrl1.unPCCardCntrl1))
#define CI_PRT_REG_PC_CTRL2(_reg)	\
		CI_PRT_REG_MESSAGE("PC Card Control 2            [0x%04x]", ci_readw((_reg)->uniPCCardCntrl2.unPCCardCntrl2))
#define CI_PRT_REG_PC_CTRL3(_reg)	\
		CI_PRT_REG_MESSAGE("PC Card Control 3            [0x%04x]", ci_readw((_reg)->uniPCCardCntrl3.unPCCardCntrl3))
#define CI_PRT_REG_BURST_CTRL(_reg)	\
		CI_PRT_REG_MESSAGE("Burst Control                [0x%04x]", ci_readw((_reg)->uniBurstCntrl.unBurstCntrl))
#define CI_PRT_REG_INT_FLAG(_reg)	\
		CI_PRT_REG_MESSAGE("Interrupt Flag               [0x%04x]", ci_readw((_reg)->uniIntrFlag.unIntrFlag))
#define CI_PRT_REG_INT_MASK(_reg)	\
		CI_PRT_REG_MESSAGE("Interrupt Mask               [0x%04x]", ci_readw((_reg)->uniIntrMask.unIntrMask))
#define CI_PRT_REG_INT_CTRL(_reg)	\
		CI_PRT_REG_MESSAGE("Interrupt Control            [0x%04x]", ci_readw((_reg)->uniIntrCntrl.unIntrCntrl))
#define CI_PRT_REG_PC_ADDR_IDX(_reg)	\
		CI_PRT_REG_MESSAGE("PCMCIA Address Index Reg.    [0x%04x]", ci_readw((_reg)->unPCMCIAAddrInd))
#define CI_PRT_REG_PC_PON_INTV1(_reg)	\
		CI_PRT_REG_MESSAGE("Power On Interval 1          [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl1.unHWHotSwapCntrl1))
#define CI_PRT_REG_PC_PON_INTV2(_reg)	\
		CI_PRT_REG_MESSAGE("Power On Interval 2          [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl2.unHWHotSwapCntrl2))
#define CI_PRT_REG_EN_INTV1(_reg)	\
		CI_PRT_REG_MESSAGE("Enable Interval 1            [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl3.unHWHotSwapCntrl3))
#define CI_PRT_REG_EN_INTV2(_reg)	\
		CI_PRT_REG_MESSAGE("Enable Interval 2            [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl4.unHWHotSwapCntrl4))
#define CI_PRT_REG_CARD_INS_DONE_INTV1(_reg)	\
		CI_PRT_REG_MESSAGE("Card Insert Done Interval 1  [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl5.unHWHotSwapCntrl5))
#define CI_PRT_REG_CARD_INS_DONE_INTV2(_reg)	\
		CI_PRT_REG_MESSAGE("Card Insert Done Interval 2  [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl6.unHWHotSwapCntrl6))
#define CI_PRT_REG_CARD_REM_DONE_INTV1(_reg)	\
		CI_PRT_REG_MESSAGE("Card Removal Done Interval 1 [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl7.unHWHotSwapCntrl7))
#define CI_PRT_REG_CARD_REM_DONE_INTV2(_reg)	\
		CI_PRT_REG_MESSAGE("Card Removal Done Interval 2 [0x%04x]", ci_readw((_reg)->uniHWHotSwapCntrl8.unHWHotSwapCntrl8))

#ifdef _CI_KDRV_DELAY_USLEEP_RANGE_
#define _HW_IO_SLEEP(_t_min, _t_max)	\
		usleep_range((_t_min)*1000, ((_t_max)?_t_max:_t_min)*1000)
#else
// apply flexible delay time
#define _HW_IO_SLEEP(_t_min, _t_max)	\
		OS_MsecSleep((_t_min))
#endif

#define CI_HW_IO_SLEEP(_idx)	\
		_HW_IO_SLEEP(gCI_KDRV_Delay[_idx], 0);

#if	defined(__KERNEL__) && defined(CI_KERN_LOG)
#define ci_kern_debug(msg...) \
	do { \
		printk(KERN_DEBUG "dvb-ci: " msg); \
	} while (0)

#define ci_kern_info(msg...) \
	do { \
		printk(KERN_INFO "dvb-ci: " msg); \
	} while (0)
#else
#define ci_kern_debug(msg...)	do{	}while(0)
#define ci_kern_info(msg...)	do{	}while(0)
#endif

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
/* mapping MOD_ID to base address */
/* this variable is declared in "buffnego.c" */
extern volatile S_CI_REG_T* gpstCIReg;
extern unsigned long	CiModBaseAddr[MAX_MOD_NUM];

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/
extern void HW_IO_SetChkMode(UINT32 chk_mode);
extern UINT32 HW_IO_GetChkMode(void);

extern int HW_IO_Init(void);
extern int HW_IO_Uninit(void);
extern int HW_IO_ResetSoft(MOD_ID mId);
extern int HW_IO_NegoBuf( MOD_ID mId, UINT32 *o_pwBufSize );
extern int HW_IO_Read( MOD_ID mId, UINT8 *o_pbyData, UINT16 *io_pwDataBufSize, int to_user);
extern int HW_IO_Write( MOD_ID mId ,UINT8 *i_pbyData, UINT32 i_wDataBufSize, int from_user );
extern int HW_IO_SetRS(MOD_ID mId);
extern int CI_IO_EnableLog( UINT32 ulArg);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _CI_IO_H_ */
