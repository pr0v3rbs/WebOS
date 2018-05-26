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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_hal.h"
#include "sdec_reg_ctrl.h"
#include "sdec_drv.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
SDIO_REG_CTRL_T *g_sdec_sdio_reg_ctrl;


/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static spinlock_t sdec_io_lock[SDEC_MAX_CORE];


int SDEC_HAL_IOInit(void)
{
	int core;
	LX_SDEC_CFG_T *cfg;

	cfg = SDEC_CFG_GetConfig();
	g_sdec_sdio_reg_ctrl = (SDIO_REG_CTRL_T*)OS_KMalloc(sizeof(SDIO_REG_CTRL_T) * SDEC_MAX_CORE);
	SDEC_CHECK_ERROR(g_sdec_sdio_reg_ctrl == NULL, return RET_ERROR, "kmalloc fail");

	for(core=0; core<cfg->nCore; core++)
	{
		g_sdec_sdio_reg_ctrl[core].size = sizeof(SDIO_REG_T);

		g_sdec_sdio_reg_ctrl[core].phys.addr = (volatile UINT32*)ioremap(cfg->sdioRegBase[core], sizeof(SDIO_REG_T));
		SDEC_CHECK_ERROR(g_sdec_sdio_reg_ctrl[core].phys.addr == NULL, return RET_ERROR, "ioremap fail");

		g_sdec_sdio_reg_ctrl[core].shdw.addr = (UINT32*)OS_KMalloc(sizeof(SDIO_REG_T));
		SDEC_CHECK_ERROR(g_sdec_sdio_reg_ctrl[core].shdw.addr == NULL, return RET_ERROR, "kmalloc fail");

		spin_lock_init (&sdec_io_lock[core]);
	}

	return RET_OK;
}

int SDEC_HAL_SetGPBBaseAddr(UINT8 core, UINT8 gpb_base)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, gpb_base_addr);
	SDIO_Wr01(core, gpb_base_addr, gpb_base, gpb_base);
	SDIO_WrFL(core, gpb_base_addr);

	ret = RET_OK;
	return ret;
}

UINT32 SDEC_HAL_GetGPBBaseAddr(UINT8 core)
{
	UINT32 val = 0;

	SDIO_RdFL(core, gpb_base_addr);
	SDIO_Rd(core, gpb_base_addr, val); 	/* read */

	return val;
}

UINT32 SDEC_HAL_GetSTCCErrorCtrl(UINT8 core, UINT8 idx)
{
	UINT32	val  = 0;

	SDIO_RdFL(core, stcc_err_ctrl[idx]);
	SDIO_Rd(core, stcc_err_ctrl[idx], val);	/* read */

	return val;
}

int SDEC_HAL_SetSTCCErrorCtrl(UINT8 core, UINT8 idx, UINT32 err_max, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, stcc_err_ctrl[idx]);
	SDIO_Wr01(core, stcc_err_ctrl[idx], err_max, err_max);
	SDIO_Wr01(core, stcc_err_ctrl[idx], en, en);
	SDIO_WrFL(core, stcc_err_ctrl[idx]);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_SDMWCLastBValidMode(UINT8 core, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, test);
	SDIO_Wr01(core, test, last_bval, en);
	SDIO_WrFL(core, test);

	ret = RET_OK;

	return ret;
}


int SDEC_HAL_EnableVideoReady(UINT8 core, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, test);
	switch(idx)
	{
		case 0 : SDIO_Wr01(core, test, vid0_rdy_en, en); break;
		case 1 : SDIO_Wr01(core, test, vid1_rdy_en, en); break;
		default : goto exit;
	}
	SDIO_WrFL(core, test);

	ret = RET_OK;

exit:
	return ret;
}


int SDEC_HAL_EnableAudioReady(UINT8 core, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, test);
	switch(idx)
	{
		case 0 : SDIO_Wr01(core, test, aud0_rdy_en, en); break;
		case 1 : SDIO_Wr01(core, test, aud1_rdy_en, en); break;
		default : goto exit;
	}
	SDIO_WrFL(core, test);

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_EnableAutoIncr(UINT8 core, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, test);
	SDIO_Wr01(core, test, auto_incr, en);
	SDIO_WrFL(core, test);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_SetTs2PesPid(UINT8 core, UINT16 pid)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, cdic2_ts2pes);
	SDIO_Wr01(core, cdic2_ts2pes, PID, pid);
	SDIO_WrFL(core, cdic2_ts2pes);

	ret = RET_OK;
	return ret;
}

UINT16 SDEC_HAL_GetTs2PesPid(UINT8 core)
{
	UINT16	val  = 0;

	SDIO_RdFL(core, cdic2_ts2pes);
	SDIO_Rd01(core, cdic2_ts2pes, PID, val);

	return val;
}

UINT32 SDEC_HAL_IntrStatReadAndClear(UINT8 core)
{
	UINT32 val = 0;

	SDIO_RdFL(core, intr_stat);
	SDIO_Rd(core, intr_stat, val);	/* read */
	SDIO_WrFL(core, intr_stat);		/* clear */

	return val;
}

int SDEC_HAL_EnableIntrPcr(UINT8 core, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, intr_en);
	SDIO_Wr01(core, intr_en, pcr, en);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrTimeBaseDiscontinuity(UINT8 core, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, intr_en);
	SDIO_Wr01(core, intr_en, tb_dcont, en);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrBDRC(UINT8 core, UINT8 idx, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT8	val = 0;

	SDIO_RdFL(core, intr_en);
	SDIO_Rd01(core, intr_en, bdrc, val);

	val |= ( 1 << idx );

	SDIO_Wr01(core, intr_en, bdrc, val);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrErrReport(UINT8 core, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, intr_en);
	SDIO_Wr01(core, intr_en, err_rpt, en);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrGPBData(UINT8 core, UINT8 ch, UINT8 isHigh, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT8	val = 0;

	SDIO_RdFL(core, intr_en);
	SDIO_Rd01(core, intr_en, gpb_data, val);

	/* CH A + Low  = 0 + 0 = 0
	 * CH A + High = 0 + 1 = 1
	 * CH B + Low  = 2 + 0 = 2
	 * CH B + High = 2 + 1 = 3
	 */
	val |= 1 << ( ch * 2 + isHigh );

	SDIO_Wr01(core, intr_en, gpb_data, val);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrGPBFull(UINT8 core, UINT8 ch, UINT8 isHigh, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT8	val = 0;

	SDIO_RdFL(core, intr_en);
	SDIO_Rd01(core, intr_en, gpb_full, val);

	/* CH A + Low  = 0 + 0 = 0
	 * CH A + High = 0 + 1 = 1
	 * CH B + Low  = 2 + 0 = 2
	 * CH B + High = 2 + 1 = 3
	 */
	val |= 1 << ( ch * 2 + isHigh );

	SDIO_Wr01(core, intr_en, gpb_full, val);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrTPInfo(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT8	val = 0;

	SDIO_RdFL(core, intr_en);
	SDIO_Rd01(core, intr_en, tp_info, val);

	val |= ( 1 << ch );

	SDIO_Wr01(core, intr_en, tp_info, val);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableIntrSectionErr(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT8	val = 0;

	SDIO_RdFL(core, intr_en);
	SDIO_Rd01(core, intr_en, sec_err, val);

	val |= ( 1 << ch );

	SDIO_Wr01(core, intr_en, sec_err, val);
	SDIO_WrFL(core, intr_en);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_EnableInterrupt(UINT8 core, E_SDEC_INTR_T eSdecIntrSrc, UINT8 en)
{
	int 	ret = RET_ERROR;

	switch(eSdecIntrSrc)
	{
		case PCR:					SDEC_HAL_EnableIntrPcr(core, en);	break;

		case TB_DCOUNT:				SDEC_HAL_EnableIntrTimeBaseDiscontinuity(core, en);	break;

		case BDRC_0:				SDEC_HAL_EnableIntrBDRC(core, 0, en);	break;
		case BDRC_1:				SDEC_HAL_EnableIntrBDRC(core, 1, en);	break;
		case BDRC_2:				SDEC_HAL_EnableIntrBDRC(core, 2, en);	break;
		case BDRC_3:				SDEC_HAL_EnableIntrBDRC(core, 3, en);	break;

		case ERR_RPT:				SDEC_HAL_EnableIntrErrReport(core, en);	break;

		case GPB_DATA_CHA_GPL:		SDEC_HAL_EnableIntrGPBData(core, 0, 0, en);	break;
		case GPB_DATA_CHA_GPH:		SDEC_HAL_EnableIntrGPBData(core, 0, 1, en);	break;
		case GPB_DATA_CHB_GPL:		SDEC_HAL_EnableIntrGPBData(core, 1, 0, en);	break;
		case GPB_DATA_CHB_GPH:		SDEC_HAL_EnableIntrGPBData(core, 1, 1, en);	break;

		case GPB_FULL_CHA_GPL:		SDEC_HAL_EnableIntrGPBFull(core, 0, 0, en);	break;
		case GPB_FULL_CHA_GPH:		SDEC_HAL_EnableIntrGPBFull(core, 0, 1, en);	break;
		case GPB_FULL_CHB_GPL:		SDEC_HAL_EnableIntrGPBFull(core, 1, 0, en);	break;
		case GPB_FULL_CHB_GPH:		SDEC_HAL_EnableIntrGPBFull(core, 1, 1, en);	break;

		case TP_INFO_CHA:			SDEC_HAL_EnableIntrTPInfo(core, 0, en);	break;
		case TP_INFO_CHB:			SDEC_HAL_EnableIntrTPInfo(core, 1, en);	break;
		case SEC_ERR_CHA:			SDEC_HAL_EnableIntrSectionErr(core, 0, en);	break;
		case SEC_ERR_CHB:			SDEC_HAL_EnableIntrSectionErr(core, 1, en);	break;
		default:					goto exit;
		break;
	}

	ret = RET_OK;

exit:
	return ret;
}

UINT32 SDEC_HAL_GetErrorInterruptStat(UINT8 core)
{
	UINT32 val = 0;

	SDIO_RdFL(core, err_intr_stat);
	SDIO_Rd(core, err_intr_stat, val);	/* read */

	return val;
}

int SDEC_HAL_EnableErrorInterrupt(UINT8 core, E_SDEC_ERRINTR_T eSdecErrIntr, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, err_intr_en);
	switch(eSdecErrIntr)
	{
		case MPG_SD:		SDIO_Wr01(core, err_intr_en, mpg_sd, val);	break;
		case MPG_CC:		SDIO_Wr01(core, err_intr_en, mpg_cc, val);	break;
		case MPG_DUP:		SDIO_Wr01(core, err_intr_en, mpg_dup, val);	break;
		case MPG_TS:		SDIO_Wr01(core, err_intr_en, mpg_ts, val);	break;
		case MPG_PD:		SDIO_Wr01(core, err_intr_en, mpg_pd, val);	break;
		case STCC_DCONT:	SDIO_Wr01(core, err_intr_en, stcc_dcont, val);	break;
		case CDIF_RPAGE:	SDIO_Wr01(core, err_intr_en, cdif_rpage, val);	break;
		case CDIF_WPAGE:	SDIO_Wr01(core, err_intr_en, cdif_wpage, val);	break;
		case CDIF_OVFLOW:	SDIO_Wr01(core, err_intr_en, cdif_ovflow, val);	break;
		case SB_DROPPED:	SDIO_Wr01(core, err_intr_en, sb_dropped, val);	break;
		case SYNC_LOST:		SDIO_Wr01(core, err_intr_en, sync_lost, val);	break;
		case TEST_DCONT:	SDIO_Wr01(core, err_intr_en, test_dcont, val);	break;
		default:					goto exit;
		break;
	}
	SDIO_WrFL(core, err_intr_en);

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_SDMWCReset(UINT8 core, UINT32 val)
{
	int 	ret = RET_ERROR;

#if 0
	if( lx_chip_rev()	>= LX_CHIP_REV(H13, A0) )
	{
		SD_RdFL_H13A0(SDIO, sdmwc_sw_reset);
		SD_Wr_H13A0(SDIO, sdmwc_sw_reset, val);
		SD_WrFL_H13A0(SDIO, sdmwc_sw_reset);
	}
#else
	SDIO_RdFL(core, sdmwc_sw_reset);
	SDIO_Wr(core, sdmwc_sw_reset, val);
	SDIO_WrFL(core, sdmwc_sw_reset);
#endif

	ret = RET_OK;

	return ret;
}

UINT32 SDEC_HAL_SDMWCGet(UINT8 core)
{
	UINT32			val = 0;

#if 0
	if( lx_chip_rev()	>= LX_CHIP_REV(H13, A0) )
	{
		SD_RdFL_H13A0(SDIO, sdmwc_sw_reset);
		SD_Rd_H13A0(SDIO, sdmwc_sw_reset, val);
	}
#else
	SDIO_RdFL(core, sdmwc_sw_reset);
	SDIO_Rd(core, sdmwc_sw_reset, val);
#endif

	return val;
}

UINT32 SDEC_HAL_SDMWCGetStatus(UINT8 core)
{
	UINT32			val = 0;

#if 0
	if( lx_chip_rev()	>= LX_CHIP_REV(H13, A0) )
	{
		SD_RdFL_H13A0(SDIO, sdmwc_stat);
		SD_Rd_H13A0(SDIO, sdmwc_stat, val);
	}
#else
	SDIO_RdFL(core, sdmwc_stat);
	SDIO_Rd(core, sdmwc_stat, val);
#endif

	return val;
}

UINT32 SDEC_HAL_STCCGetASG(UINT8 core)
{
	UINT32	val = 0;

	SDIO_RdFL(core, stcc_asg);
	SDIO_Rd(core, stcc_asg, val);

	return val;
}


UINT8 SDEC_HAL_STCCGetMain(UINT8 core)
{
	UINT8	val = 0;

	SDIO_RdFL(core, stcc_asg);
	SDIO_Rd01(core, stcc_asg, main, val);
	return val;
}

int SDEC_HAL_STCCSetMain(UINT8 core, UINT8 ch)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, stcc_asg);
	SDIO_Wr01(core, stcc_asg, main, ch);
	SDIO_WrFL(core, stcc_asg);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_STCCSetVideoAssign(UINT8 core, UINT8 idx, UINT8 ch)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, stcc_asg);
	switch(idx)
	{
		case 0: SDIO_Wr01(core, stcc_asg, vid0, ch);	break;
		case 1: SDIO_Wr01(core, stcc_asg, vid1, ch);	break;
		default : goto exit;
	}
	SDIO_WrFL(core, stcc_asg);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_STCCSetAudioAssign(UINT8 core, UINT8 idx, UINT8 ch)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, stcc_asg);
	switch(idx)
	{
		case 0: SDIO_Wr01(core, stcc_asg, aud0, ch);	break;
		case 1: SDIO_Wr01(core, stcc_asg, aud1, ch);	break;
		default : goto exit;
	}
	SDIO_WrFL(core, stcc_asg);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_STCCGetReg(UINT8 core, UINT8 ch)
{
	UINT32 val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);

	return val;
}

int SDEC_HAL_STCCSetReg(UINT8 core, UINT8 ch, UINT32 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	ret = RET_OK;

	return ret;
}


int SDEC_HAL_STCCSetSTC(UINT8 core, UINT8 ch, UINT32 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, stcc[ch * 4 + 1]);
	SDIO_Wr(core, stcc[ch * 4 + 1], val);
	SDIO_WrFL(core, stcc[ch * 4 + 1]);

	ret = RET_OK;

	return ret;
}

#define STC_MODE_LATCHED	0
#define STC_MODE_LIVE		1
static int _SDEC_HAL_STCCGetSTC(UINT8 core, UINT8 ch, UINT8 mode, UINT32 *stcc_41_10, UINT32 *stcc_9_0)
{
	UINT32 val;
	unsigned long flag;

	spin_lock_irqsave (&sdec_io_lock[core], flag);

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	if(mode == STC_MODE_LATCHED)
		val &= ~(0x1 << 8); 	// Latched Mode. Set RD_MODE to 0
	else
		val |= (0x1 << 8);		// Live Mode. Set RD_MODE to 1
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	SDIO_RdFL(core, stcc[ch * 4 + 1]);
	SDIO_Rd(core, stcc[ch * 4 + 1], val);
	*stcc_41_10 = val;

	SDIO_RdFL(core, stcc[ch * 4 + 3]);
	SDIO_Rd(core, stcc[ch * 4 + 3], val);
	val = extract_bits(val, 0x3FF, 16);
	*stcc_9_0 = val;

	spin_unlock_irqrestore (&sdec_io_lock[core], flag);

	return RET_OK;
}


int SDEC_HAL_STCCGetLiveSTC(UINT8 core, UINT8 ch, UINT32 *stcc_41_10, UINT32 *stcc_9_0)
{
	return _SDEC_HAL_STCCGetSTC(core, ch, STC_MODE_LIVE, stcc_41_10, stcc_9_0);
}

int SDEC_HAL_STCCGetLatchedSTC(UINT8 core, UINT8 ch, UINT32 *stcc_41_10, UINT32 *stcc_9_0)
{
	return _SDEC_HAL_STCCGetSTC(core, ch, STC_MODE_LATCHED, stcc_41_10, stcc_9_0);
}

int SDEC_HAL_STCCGetPCR(UINT8 core, UINT8 ch, UINT32 *pcr_41_10, UINT32 *pcr_9_0)
{
	UINT32	val;

	SDIO_RdFL(core, stcc[ch * 4 + 2]);
	SDIO_Rd(core, stcc[ch * 4 + 2], val);
	*pcr_41_10 = val;

	SDIO_RdFL(core, stcc[ch * 4 + 3]);
	SDIO_Rd(core, stcc[ch * 4 + 3], val);
	*pcr_9_0 = extract_bits(val, 0x3FF, 0);

	return RET_OK;
}


int SDEC_HAL_STCCEnable(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( en << 31);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_STCCSetCh(UINT8 core, UINT8 ch, UINT8 chan)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( chan << 29);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);


	ret = RET_OK;

	return ret;
}

int SDEC_HAL_STCCEnableCopy(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( en << 1);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_STCCEnableLatch(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( en << 0);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_STCCReset(UINT8 core, UINT8 ch)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( 1 << 30);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_AVSTCReset(UINT8 core, UINT8 ch)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;
	/* AV STC Reset Condition, from IPT , 2013. 02. 26 */
	UINT32	val_stc_9_0 = 0x32b;
	UINT32 	val_stc_41_10 = 0xffffffff;

	/* Set stc_41_10 */
	SDIO_RdFL(core, stcc[ch * 4 + 1]);
	SDIO_Rd(core, stcc[ch * 4 + 1], val);
	val |= ( val_stc_41_10 ); // set all 1 to stc_41_10
	SDIO_Wr(core, stcc[ch * 4 + 1], val);
	SDIO_WrFL(core, stcc[ch * 4 + 1]);

	/* Set stc_9_0 */
	SDIO_RdFL(core, stcc[ch * 4 + 3]);
	SDIO_Rd(core, stcc[ch * 4 + 3], val);
	val &= ( 0xfc00ffff );	// reset stc_9_0
	val |= ( val_stc_9_0 << 16);
	SDIO_Wr(core, stcc[ch * 4 + 3], val);
	SDIO_WrFL(core, stcc[ch * 4 + 3]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_STCCSetPCRPid(UINT8 core, UINT8 ch, UINT16 pid)
{
	int 	ret = RET_ERROR;
	UINT32	val = 0;

	SDIO_RdFL(core, stcc[ch * 4]);
	SDIO_Rd(core, stcc[ch * 4], val);
	val |= ( pid << 16);
	SDIO_Wr(core, stcc[ch * 4], val);
	SDIO_WrFL(core, stcc[ch * 4]);


	ret = RET_OK;

	return ret;
}


int SDEC_HAL_STCCSetSubStccRate(UINT8 core, UINT32 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, sub_stcc_rate);
	SDIO_Wr(core, sub_stcc_rate, val);
	SDIO_WrFL(core, sub_stcc_rate);

	ret = RET_OK;

	return ret;
}


UINT32 SDEC_HAL_STCCGetSubStccRate(UINT8 core)
{
	UINT32	val = 0;

	SDIO_RdFL(core, sub_stcc_rate);
	SDIO_Rd(core, sub_stcc_rate, val);

	return val;
}

/* M14_TBD, H14_TBD merge into 1 routine with gstc0 and 1 , jinhwan.bae */
int SDEC_HAL_GSTC(UINT8 core, UINT32 *pStcc_bs_32_1, UINT32 *pStcc_bs_0)
{
	int 	ret = RET_OK;
	UINT32	val = 0;
#if 0
	if( lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		*pStcc_bs_32_1 		= stSDEC_IO_RegH13A0->gstc0.stcc_bs_32_1;
		*pStcc_bs_0 		= stSDEC_IO_RegH13A0->gstc0.stcc_bs_0;
	}
#else
	SDIO_RdFL(core, gstc0[1]);
	SDIO_Rd(core, gstc0[1], val);
	(*pStcc_bs_32_1) = val;

	SDIO_RdFL(core, gstc0[2]);
	SDIO_Rd(core, gstc0[2], val);
	val = extract_bits(val, 0x1, 12);
	(*pStcc_bs_0) = val;
#endif

	return ret;
}

int SDEC_HAL_GSTCReset(UINT8 core)
{
	int 	ret = RET_OK;
	UINT32	val = 0;
#if 0
	if( lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		SD_RdFL_H13A0(SDIO, gstc0);
		SD_Wr01_H13A0(SDIO, gstc0, rst, 1);
		SD_WrFL_H13A0(SDIO, gstc0);
	}
#else
	SDIO_RdFL(core, gstc0[0]);
	SDIO_Rd(core, gstc0[0], val);
	val |= ( 1 << 30);
	SDIO_Wr(core, gstc0[0], val);
	SDIO_WrFL(core, gstc0[0]);
#endif

	return ret;
}

int SDEC_HAL_GSTC1(UINT8 core, UINT32 *pStcc_bs_32_1, UINT32 *pStcc_bs_0)
{
	int 	ret = RET_OK;
	UINT32	val = 0;
#if 0
	if( lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		*pStcc_bs_32_1 		= stSDEC_IO_RegH13A0->gstc1.stcc_bs_32_1;
		*pStcc_bs_0 		= stSDEC_IO_RegH13A0->gstc1.stcc_bs_0;
	}
#else
	SDIO_RdFL(core, gstc1[1]);
	SDIO_Rd(core, gstc1[1], val);
	(*pStcc_bs_32_1) = val;

	SDIO_RdFL(core, gstc1[2]);
	SDIO_Rd(core, gstc1[2], val);
	val = extract_bits(val, 0x1, 12);
	(*pStcc_bs_0) = val;
#endif

	return ret;
}


int SDEC_HAL_GSTC1Reset(UINT8 core)
{
	int 	ret = RET_OK;
	UINT32	val = 0;
#if 0
	if( lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		SD_RdFL_H13A0(SDIO, gstc1);
		SD_Wr01_H13A0(SDIO, gstc1, rst, 1);
		SD_WrFL_H13A0(SDIO, gstc1);
	}
#else
	SDIO_RdFL(core, gstc1[0]);
	SDIO_Rd(core, gstc1[0], val);
	val |= ( 1 << 30);
	SDIO_Wr(core, gstc1[0], val);
	SDIO_WrFL(core, gstc1[0]);
#endif

	return ret;
}


int SDEC_HAL_GSTCSetValue(UINT8 core, UINT8 index, UINT32 val)
{
	int 	ret = RET_ERROR;

	/* extension modification is precondition of base modification */
	if(index == 0)
	{
		SDIO_Wr(core, gstc0[2], 0);
		SDIO_WrFL(core, gstc0[2]);

		SDIO_Wr(core, gstc0[1], val);
		SDIO_WrFL(core, gstc0[1]);

		ret = RET_OK;
	}
	else if(index == 1)
	{
		SDIO_Wr(core, gstc1[2], 0);
		SDIO_WrFL(core, gstc1[2]);

		SDIO_Wr(core, gstc1[1], val);
		SDIO_WrFL(core, gstc1[1]);

		ret = RET_OK;
	}

	return ret;
}


int SDEC_HAL_DSCSetCasType(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], cas_type, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCSetBlkMode(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], blk_mode, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCSetResMode(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], res_mode, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCSetKeySize(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], key_size, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCEnablePESCramblingCtrl(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], psc_en, en);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCSetEvenMode(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], even_mode, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_DSCSetOddMode(UINT8 core, UINT8 ch, UINT8 val)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdic_dsc[ch]);
	SDIO_Wr01(core, cdic_dsc[ch], odd_mode, val);
	SDIO_WrFL(core, cdic_dsc[ch]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CIDCReset(UINT8 core, UINT8 ch)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if( ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Wr01(core, cdic2, rst, 1);
		SDIO_WrFL(core, cdic2);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Wr01(core, cdic3, rst, 1);
		SDIO_WrFL(core, cdic3);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Wr01(core, cdic[ch], rst, 1);
		SDIO_WrFL(core, cdic[ch]);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_CIDCMinSyncByteDetection(UINT8 core, UINT8 ch, UINT8 val)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if(ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Wr01(core, cdic2, min_sb_det, val);
		SDIO_WrFL(core, cdic2);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Wr01(core, cdic3, min_sb_det, val);
		SDIO_WrFL(core, cdic3);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Wr01(core, cdic[ch], min_sb_det, val);
		SDIO_WrFL(core, cdic[ch]);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_CIDCMaxSyncByteDrop(UINT8 core, UINT8 ch, UINT8 val)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if(ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Wr01(core, cdic2, max_sb_drp, val);
		SDIO_WrFL(core, cdic2);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Wr01(core, cdic3, max_sb_drp, val);
		SDIO_WrFL(core, cdic3);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Wr01(core, cdic[ch], max_sb_drp, val);
		SDIO_WrFL(core, cdic[ch]);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_CIDCSetSrc(UINT8 core, UINT8 ch, UINT8 val)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if(ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Wr01(core, cdic2, src, val);
		SDIO_WrFL(core, cdic2);
	}
	else if(ch == 3)
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Wr01(core, cdic3, src, val);
		SDIO_WrFL(core, cdic3);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Wr01(core, cdic[ch], src, val);
		SDIO_WrFL(core, cdic[ch]);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_CIDCRead(UINT8 core, UINT8 ch, UINT32 *pVal)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if(ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Rd(core, cdic2, *pVal);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Rd(core, cdic3, *pVal);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Rd(core, cdic[ch], *pVal);
	}

	ret = RET_OK;

exit:
	return ret;
}

int SDEC_HAL_CIDCGetStatus(UINT8 core, UINT8 ch, CDIC *pCdic)
{
	int 			ret 		= RET_ERROR;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);


	if(ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Rd01(core, cdic2, 	src, 			pCdic->src);
		SDIO_Rd01(core, cdic2, 	no_wdata, 		pCdic->no_wdata);
		SDIO_Rd01(core, cdic2, 	sync_lost,		pCdic->sync_lost);
		SDIO_Rd01(core, cdic2, 	sb_dropped,		pCdic->sb_dropped);
		SDIO_Rd01(core, cdic2, 	cdif_empty,		pCdic->cdif_empty);
		SDIO_Rd01(core, cdic2, 	cdif_full,		pCdic->cdif_full);
		SDIO_Rd01(core, cdic2, 	pd_wait,		pCdic->pd_wait);
		SDIO_Rd01(core, cdic2, 	cdif_ovflow,	pCdic->cdif_ovflow);
		SDIO_Rd01(core, cdic2, 	cdif_wpage,		pCdic->cdif_wpage);
		SDIO_Rd01(core, cdic2, 	cdif_rpage,		pCdic->cdif_rpage);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Rd01(core, cdic3, 	src, 			pCdic->src);
		SDIO_Rd01(core, cdic3, 	no_wdata, 		pCdic->no_wdata);
		SDIO_Rd01(core, cdic3, 	sync_lost,		pCdic->sync_lost);
		SDIO_Rd01(core, cdic3, 	sb_dropped,		pCdic->sb_dropped);
		SDIO_Rd01(core, cdic3, 	cdif_empty,		pCdic->cdif_empty);
		SDIO_Rd01(core, cdic3, 	cdif_full,		pCdic->cdif_full);
		SDIO_Rd01(core, cdic3, 	pd_wait,		pCdic->pd_wait);
		SDIO_Rd01(core, cdic3, 	cdif_ovflow,	pCdic->cdif_ovflow);
		SDIO_Rd01(core, cdic3, 	cdif_wpage,		pCdic->cdif_wpage);
		SDIO_Rd01(core, cdic3, 	cdif_rpage,		pCdic->cdif_rpage);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Rd01(core, cdic[ch], 	src, 			pCdic->src);
		SDIO_Rd01(core, cdic[ch], 	no_wdata, 		pCdic->no_wdata);
		SDIO_Rd01(core, cdic[ch], 	sync_lost,		pCdic->sync_lost);
		SDIO_Rd01(core, cdic[ch], 	sb_dropped,		pCdic->sb_dropped);
		SDIO_Rd01(core, cdic[ch], 	cdif_empty,		pCdic->cdif_empty);
		SDIO_Rd01(core, cdic[ch], 	cdif_full,		pCdic->cdif_full);
		SDIO_Rd01(core, cdic[ch], 	pd_wait,		pCdic->pd_wait);
		SDIO_Rd01(core, cdic[ch], 	cdif_ovflow,	pCdic->cdif_ovflow);
		SDIO_Rd01(core, cdic[ch], 	cdif_wpage,		pCdic->cdif_wpage);
		SDIO_Rd01(core, cdic[ch], 	cdif_rpage,		pCdic->cdif_rpage);
	}

	ret = RET_OK;

exit:
	return ret;
}

UINT8 SDEC_HAL_CIDCGetCdif_OverFlow(UINT8 core, UINT8 ch)
{
	UINT8	val = 0;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);

	if( ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Rd01(core, cdic2, cdif_ovflow, val);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Rd01(core, cdic3, cdif_ovflow, val);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Rd01(core, cdic[ch], cdif_ovflow, val);
	}

exit:
	return val;
}

UINT8 SDEC_HAL_CIDCGetCdif_Full(UINT8 core, UINT8 ch)
{
	UINT8	val = 0;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);


	if( ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Rd01(core, cdic2, cdif_full, val);

	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Rd01(core, cdic3, cdif_full, val);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Rd01(core, cdic[ch], cdif_full, val);
	}

exit:
	return val;
}

UINT32 SDEC_HAL_CIDCGet(UINT8 core, UINT8 ch)
{
	UINT32			val = 0;
	LX_SDEC_CFG_T*	cfg = SDEC_CFG_GetConfig();

	LX_SDEC_HAL_CHECK_CH(cfg, ch);


	if( ch == 2)
	{
		SDIO_RdFL(core, cdic2);
		SDIO_Rd(core, cdic2, val);
	}
	else if( ch == 3 )
	{
		SDIO_RdFL(core, cdic3);
		SDIO_Rd(core, cdic3, val);
	}
	else
	{
		SDIO_RdFL(core, cdic[ch]);
		SDIO_Rd(core, cdic[ch], val);
	}

exit:
	return val;
}

int SDEC_HAL_CIDC3DlConf(UINT8 core, UINT8 ch, UINT8 val)
{
	int 			ret 		= RET_ERROR;

	/* value is 0, 1 */
	if( val > 1 )	goto exit;

	switch(ch)
	{
		case 0: /* DL0_SEL */
			SDIO_RdFL(core, cdic3_dl_conf);
			SDIO_Wr01(core, cdic3_dl_conf, dl0_sel, val);
			SDIO_WrFL(core, cdic3_dl_conf);
			break;
		case 1: /* DL1_SEL */
			SDIO_RdFL(core, cdic3_dl_conf);
			SDIO_Wr01(core, cdic3_dl_conf, dl1_sel, val);
			SDIO_WrFL(core, cdic3_dl_conf);
			break;
		default :
			goto exit;
	}

	ret = RET_OK;

exit:
	return ret;
}


int SDEC_HAL_CDIC2PIDFSetPidfData(UINT8 core, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	/* read register */
	SDIO_RdFL(core, cdic3_dl_conf);

	/* write index , data, write enable */
	SDIO_Wr01(core, cdic3_dl_conf, pidf_addr, idx);
	SDIO_Wr01(core, cdic3_dl_conf, pidf_val, val);
	SDIO_Wr01(core, cdic3_dl_conf, pid_wr_en, SDEC_HAL_ENABLE);

 	SDIO_WrFL(core, cdic3_dl_conf);

	ret = RET_OK;
	return ret;
}


UINT32 SDEC_HAL_CDIC2PIDFGetPidfData(UINT8 core, UINT8 idx)
{
	UINT32 val = 0;

	/* set pid filter index */
	SDIO_RdFL(core, cdic3_dl_conf);
	SDIO_Wr01(core, cdic3_dl_conf, pidf_addr, idx);
	SDIO_WrFL(core, cdic3_dl_conf);

	/* read data */
	SDIO_RdFL(core, cdic3_dl_conf);
	SDIO_Rd01(core, cdic3_dl_conf, pidf_val, val);

	return val;
}


int SDEC_HAL_CDIC2PIDFEnablePidFilter(UINT8 core, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, cdic3_dl_conf);

	switch(idx){
		case 0:	SDIO_Wr01(core, cdic3_dl_conf, pidf0_en, en);	break;
		case 1:	SDIO_Wr01(core, cdic3_dl_conf, pidf1_en, en);	break;
		case 2:	SDIO_Wr01(core, cdic3_dl_conf, pidf2_en, en);	break;
		case 3:	SDIO_Wr01(core, cdic3_dl_conf, pidf3_en, en);	break;
		default : break;
	}
	SDIO_WrFL(core, cdic3_dl_conf);

	ret = RET_OK;
	return ret;
}

UINT8 SDEC_HAL_CDIC2GetPIDFEnable(UINT8 core, UINT8 idx)
{
	UINT8 en = SDEC_HAL_DISABLE;

	SDIO_RdFL(core, cdic3_dl_conf);
	switch(idx){
		case 0:	SDIO_Rd01(core, cdic3_dl_conf, pidf0_en, en);	break;
		case 1:	SDIO_Rd01(core, cdic3_dl_conf, pidf1_en, en);	break;
		case 2:	SDIO_Rd01(core, cdic3_dl_conf, pidf2_en, en);	break;
		case 3:	SDIO_Rd01(core, cdic3_dl_conf, pidf3_en, en);	break;
		default : break;
	}
	SDIO_WrFL(core, cdic3_dl_conf);

	return en;
}



int SDEC_HAL_CDIC2DlExtConf(UINT8 core, UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDIO_RdFL(core, cdic3_dl_conf);
	switch(ch){
		case 0:	SDIO_Wr01(core, cdic3_dl_conf, dl0_ext_sel, val);	break;
		case 1:	SDIO_Wr01(core, cdic3_dl_conf, dl1_ext_sel, val);	break;
		default : break;
	}
	SDIO_WrFL(core, cdic3_dl_conf);

	ret = RET_OK;
	return ret;
}




int SDEC_HAL_CDIPEnable(UINT8 core, UINT8 idx, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdip[idx]);
	SDIO_Wr01(core, cdip[idx], en, en);
	SDIO_WrFL(core, cdip[idx]);

	ret = RET_OK;

	return ret;
}


/**
 * Set CDIP setting
 *
 * @param	idx				[in] CDIP index
 * @param	isParallel		[in] is this port using for parallel or serial ( 0 = serial 1 = parallel )
 * @param	isFirst			[in] is this port first in parallel? only used if isParallel is 1.
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 */
int SDEC_HAL_CDIP(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdip[idx]);

	if(pCfg != NULL)
	{
		SDIO_Wr01(core, cdip[idx], sync_type, 	pCfg->sync_type);
		SDIO_Wr01(core, cdip[idx], pconf, 		pCfg->pconf);
		SDIO_Wr01(core, cdip[idx], clk_div,		pCfg->clk_div);
		SDIO_Wr01(core, cdip[idx], val_en,		pCfg->val_en);
		SDIO_Wr01(core, cdip[idx], en,			1);
		SDIO_Wr01(core, cdip[idx], dtype, 		SDEC_HAL_CDIP_MPEG2TS);
		SDIO_Wr01(core, cdip[idx], val_en,		pCfg->val_en);
		SDIO_Wr01(core, cdip[idx], req_en,		pCfg->req_en);
		SDIO_Wr01(core, cdip[idx], err_en,		pCfg->err_en);
		SDIO_Wr01(core, cdip[idx], req_act_lo,	pCfg->req_act_lo);
		SDIO_Wr01(core, cdip[idx], val_act_lo,	pCfg->val_act_lo);
		SDIO_Wr01(core, cdip[idx], clk_act_lo,	pCfg->clk_act_lo);
		SDIO_Wr01(core, cdip[idx], err_act_hi,	pCfg->err_act_hi);
		SDIO_Wr01(core, cdip[idx], test_en,		0);
	}
	else
	{
		SDIO_Wr01(core, cdip[idx], sync_type, 	SDEC_HAL_CDIP_47DETECTION);
		SDIO_Wr01(core, cdip[idx], clk_div,		0x8);
		SDIO_Wr01(core, cdip[idx], pconf, 		SDEC_HAL_CDIP_SERIAL_0);
		SDIO_Wr01(core, cdip[idx], val_en,		0);
		SDIO_Wr01(core, cdip[idx], en,			0);
		SDIO_Wr01(core, cdip[idx], dtype, 		SDEC_HAL_CDIP_MPEG2TS);
		SDIO_Wr01(core, cdip[idx], val_en,		0);
		SDIO_Wr01(core, cdip[idx], req_en,		0);
		SDIO_Wr01(core, cdip[idx], err_en,		0);
		SDIO_Wr01(core, cdip[idx], req_act_lo,	0);
		SDIO_Wr01(core, cdip[idx], val_act_lo,	0);
		SDIO_Wr01(core, cdip[idx], clk_act_lo,	0);
		SDIO_Wr01(core, cdip[idx], err_act_hi,	0);
		SDIO_Wr01(core, cdip[idx], test_en,		0);

	}

	SDIO_WrFL(core, cdip[idx]);

	ret = RET_OK;

	return ret;
}


int SDEC_HAL_CDIOPEnable(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdiop[ch]);
	SDIO_Wr01(core, cdiop[ch], en, en);
	SDIO_WrFL(core, cdiop[ch]);

	ret = RET_OK;

	return ret;
}

/**
 * Set CDIOP setting
 *
 * @param	idx				[in] CDIP index
 * @param	inout			[in] 0 : in 1: out
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 */
int SDEC_HAL_CDIOP(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdiop[idx]);

	SDIO_Wr01(core, cdiop[idx], sync_type, 	pCfg->sync_type);
	SDIO_Wr01(core, cdiop[idx], pconf, 		pCfg->pconf);
	SDIO_Wr01(core, cdiop[idx], clk_div,		pCfg->clk_div);
	SDIO_Wr01(core, cdiop[idx], val_en,		pCfg->val_en);
	SDIO_Wr01(core, cdiop[idx], cdinout,		pCfg->cdin_out);
	SDIO_Wr01(core, cdiop[idx], val_sel,		pCfg->val_sel);
	SDIO_Wr01(core, cdiop[idx], en,			1);
	SDIO_Wr01(core, cdiop[idx], dtype, 		SDEC_HAL_CDIP_MPEG2TS);
	SDIO_Wr01(core, cdiop[idx], val_en,		pCfg->val_en);
	SDIO_Wr01(core, cdiop[idx], req_en,		pCfg->req_en);
	SDIO_Wr01(core, cdiop[idx], err_en,		pCfg->err_en);
	SDIO_Wr01(core, cdiop[idx], req_act_lo,	pCfg->req_act_lo);
	SDIO_Wr01(core, cdiop[idx], val_act_lo,	pCfg->val_act_lo);
	SDIO_Wr01(core, cdiop[idx], clk_act_lo,	pCfg->clk_act_lo);
	SDIO_Wr01(core, cdiop[idx], err_act_hi,	pCfg->err_act_hi);
	SDIO_Wr01(core, cdiop[idx], test_en,		0);

	SDIO_WrFL(core, cdiop[idx]);

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_CDIPAEnable(UINT8 core, UINT8 ch, UINT8 en)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdipa[ch]);
	SDIO_Wr01(core, cdipa[ch], en, en);
	SDIO_WrFL(core, cdipa[ch]);

	ret = RET_OK;
	return ret;
}

/**
 * Set CDIP setting
 *
 * @param	idx				[in] CDIP index
 * @param	isParallel		[in] is this port using for parallel or serial ( 0 = serial 1 = parallel )
 * @param	isFirst			[in] is this port first in parallel? only used if isParallel is 1.
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 */
int SDEC_HAL_CDIPA(UINT8 core, UINT8 idx,	LX_SDEC_CFG_INPUT_T *pCfg)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdipa[idx]);

	if(pCfg != NULL)
	{
		SDIO_Wr01(core,  cdipa[idx], sync_type, 	pCfg->sync_type);
		SDIO_Wr01(core,  cdipa[idx], pconf, 		pCfg->pconf);
		SDIO_Wr01(core,  cdipa[idx], clk_div,		pCfg->clk_div);
		SDIO_Wr01(core,  cdipa[idx], val_en,		pCfg->val_en);
		SDIO_Wr01(core,  cdipa[idx], en,			1);
		SDIO_Wr01(core,  cdipa[idx], dtype, 		SDEC_HAL_CDIP_MPEG2TS);
		SDIO_Wr01(core,  cdipa[idx], val_en,		pCfg->val_en);
		SDIO_Wr01(core,  cdipa[idx], req_en,		pCfg->req_en);
		SDIO_Wr01(core,  cdipa[idx], err_en,		pCfg->err_en);
		SDIO_Wr01(core,  cdipa[idx], req_act_lo,	pCfg->req_act_lo);
		SDIO_Wr01(core,  cdipa[idx], val_act_lo,	pCfg->val_act_lo);
		SDIO_Wr01(core,  cdipa[idx], clk_act_lo,	pCfg->clk_act_lo);
		SDIO_Wr01(core,  cdipa[idx], err_act_hi,	pCfg->err_act_hi);
		SDIO_Wr01(core,  cdipa[idx], test_en,		0);
	}
	else
	{
		SDIO_Wr01(core,  cdipa[idx], sync_type, 	SDEC_HAL_CDIP_47DETECTION);
		SDIO_Wr01(core,  cdipa[idx], clk_div,		0x8);
		SDIO_Wr01(core,  cdipa[idx], pconf, 		SDEC_HAL_CDIP_SERIAL_0);
		SDIO_Wr01(core,  cdipa[idx], val_en,		0);
		SDIO_Wr01(core,  cdipa[idx], en,			0);
		SDIO_Wr01(core,  cdipa[idx], dtype, 		SDEC_HAL_CDIP_MPEG2TS);
		SDIO_Wr01(core,  cdipa[idx], val_en,		0);
		SDIO_Wr01(core,  cdipa[idx], req_en,		0);
		SDIO_Wr01(core,  cdipa[idx], err_en,		0);
		SDIO_Wr01(core,  cdipa[idx], req_act_lo,	0);
		SDIO_Wr01(core,  cdipa[idx], val_act_lo,	0);
		SDIO_Wr01(core,  cdipa[idx], clk_act_lo,	0);
		SDIO_Wr01(core,  cdipa[idx], err_act_hi,	0);
		SDIO_Wr01(core,  cdipa[idx], test_en,		0);
	}

	SDIO_WrFL(core, cdipa[idx]);

	ret = RET_OK;
	return ret;
}

/* jinhwan.bae 20131223
    add CDIPx_2nd Configuration for supporting external clock usage in serial input port
    only serial input port support original external clock operation,
    parallel input port used internal re-generation clock with internal 200MHz clock */
int SDEC_HAL_CDIPn_2ND(UINT8 core, UINT8 port, LX_SDEC_CFG_INPUT_T *cfg_input)
{
	int ret = RET_ERROR;
	UINT32	val;

/* values are informed from IPT, bit 2 en is key register */
	if(cfg_input == NULL || cfg_input->clock_mode == LX_SDEC_INPUT_CLOCK_INT)
	{
		val = 0;
	}
	else
	{
		switch(cfg_input->sync_type_2nd)
		{
			case LX_SDEC_SYNC_EXT_SOP_LSB_BIT:	val = 0x1037; break;
			case LX_SDEC_SYNC_EXT_SOP_MSB_BIT:	val = 0x2037; break;
			case LX_SDEC_SYNC_EXT_SOP_ALL_BIT:	val = 0x3037; break;
			case LX_SDEC_SYNC_INT_SYNC_BYTE:
			default:							val = 0x0037; break;
		}
	}

#if 0	/* In M14C0, Register Name was changed but register offset & field value is same */
	if(lx_chip() == LX_CHIP_M14 && lx_chip_rev() >= LX_CHIP_REV(M14, C0))
	{
		switch(port)
		{
			case 0:		// CDIP0
			{
				SDIO_Wr_EX(M14C0, core, cdip0_2nd, val);
				SDIO_WrFL_EX(M14C0, core, cdip0_2nd);
				break;
			}

			case 1:		// CDIP1
			{
				SDIO_Wr_EX(M14C0, core, cdip1_2nd, val);
				SDIO_WrFL_EX(M14C0, core, cdip1_2nd);
				break;
			}

			case 2:		// CDIP2
			{
				SDIO_Wr_EX(M14C0, core, cdip2_2nd, val);
				SDIO_WrFL_EX(M14C0, core, cdip2_2nd);
				break;
			}

			case 3:		// CDIP3
			{
				SDIO_Wr_EX(M14C0, core, cdip3_2nd, val);
				SDIO_WrFL_EX(M14C0, core, cdip3_2nd);
				break;
			}
			default : break;
		}
	}
	else
#endif
	{
		switch(port)
		{
			case 0:		// CDIP0
			{
				SDIO_Wr(core, cdip0_2nd, val);
				SDIO_WrFL(core, cdip0_2nd);
				break;
			}

			case 1:		// CDIPA
			{
				SDIO_Wr(core, cdipa_2nd, val);
				SDIO_WrFL(core, cdipa_2nd);
				break;
			}

			case 2:		// CDIP3
			{
				SDIO_Wr(core, cdip3_2nd, val);
				SDIO_WrFL(core, cdip3_2nd);
				break;
			}
			case 3:		// CDIP4
			{
				SDIO_Wr(core, cdip4_2nd, val);
				SDIO_WrFL(core, cdip4_2nd);
				break;
			}
			default :
				goto exit;
		}
	}

	ret = RET_OK;

exit:
	return ret;
}

/*  Start of BDRC Stub for TS Output */
UINT32 SDEC_HAL_CDIOP_GetStatus(UINT8 core, UINT8 idx)
{
	UINT32			val = 0;

	SDIO_RdFL(core, cdiop[idx]);
	SDIO_Rd(core, cdiop[idx], val);

	return val;
}

int SDEC_HAL_BDRC_Enable(UINT8 core, UINT8 idx, UINT8 en)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, en, en);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, en, en);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, en, en);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_Reset(UINT8 core, UINT8 idx)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, rst, 1);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, rst, 1);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, rst, 1);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetWptrUpdate(UINT8 core, UINT8 idx)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, wptr_upd, 1);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, wptr_upd, 1);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, wptr_upd, 1);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_EnableWptrAutoUpdate(UINT8 core, UINT8 idx, UINT8 en)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, wptr_auto, en);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, wptr_auto, en);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, wptr_auto, en);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetGPBChannel(UINT8 core, UINT8 idx, UINT8 gpb_chan)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, gpb_chan, gpb_chan);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, gpb_chan, gpb_chan);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, gpb_chan, gpb_chan);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetGPBIndex(UINT8 core, UINT8 idx, UINT8 gpb_idx)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, gpb_idx, gpb_idx);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, gpb_idx, gpb_idx);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, gpb_idx, gpb_idx);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetQlen(UINT8 core, UINT8 idx, UINT8 q_len)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, q_len, q_len);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, q_len, q_len);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, q_len, q_len);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetDtype(UINT8 core, UINT8 idx, UINT8 dtype)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc0);
			SDIO_Wr01(core, bdrc0, dtype, dtype);
			SDIO_WrFL(core, bdrc0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc1);
			SDIO_Wr01(core, bdrc1, dtype, dtype);
			SDIO_WrFL(core, bdrc1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc3);
			SDIO_Wr01(core, bdrc3, dtype, dtype);
			SDIO_WrFL(core, bdrc3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_BDRC_SetBufferEmptyLevel(UINT8 core, UINT8 idx, UINT32 buf_e_lev)
{
	int 	ret = RET_ERROR;

	switch(idx)
	{
		case 0 :
			SDIO_RdFL(core, bdrc_lev0);
			SDIO_Wr01(core, bdrc_lev0, buf_e_lev, buf_e_lev);
			SDIO_WrFL(core, bdrc_lev0);
			break;
		case 1 :
			SDIO_RdFL(core, bdrc_lev1);
			SDIO_Wr01(core, bdrc_lev1, buf_e_lev, buf_e_lev);
			SDIO_WrFL(core, bdrc_lev1);
			break;
		case 3 :
			SDIO_RdFL(core, bdrc_lev3);
			SDIO_Wr01(core, bdrc_lev3, buf_e_lev, buf_e_lev);
			SDIO_WrFL(core, bdrc_lev3);
			break;
		default :
			break;
	}

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_Reset(UINT8 core, UINT8 idx)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], rst, 1);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_SetSrc(UINT8 core, UINT8 idx, UINT8 src)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], src, src);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_SetTsoSrc(UINT8 core, UINT8 idx, UINT8 tso_src)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], tso_src, tso_src);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_SetIDMask(UINT8 core, UINT8 idx, UINT8 id_mask)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], id_mask, id_mask);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_SetIDMsb(UINT8 core, UINT8 idx, UINT8 id_msb)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], id_msb, id_msb);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDOC_SetIDValue(UINT8 core, UINT8 idx, UINT8 id_value)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdoc[idx]);
	SDIO_Wr01(core, cdoc[idx], id_value, id_value);
	SDIO_WrFL(core, cdoc[idx]);

	ret = RET_OK;

	return ret;
}

int SDEC_HAL_CDIOP_SetOut(UINT8 core, UINT8 idx,	LX_SDEC_CFG_OUTPUT_T *pCfg)
{
	int 	ret = RET_ERROR;

	SDIO_RdFL(core, cdiop[idx]);

	SDIO_Wr01(core, cdiop[idx], en,			1);
	SDIO_Wr01(core, cdiop[idx], clk_act_lo,	pCfg->clk_act_lo);
	SDIO_Wr01(core, cdiop[idx], val_act_lo,	pCfg->val_act_lo);
	SDIO_Wr01(core, cdiop[idx], req_act_lo,	pCfg->req_act_lo);
	SDIO_Wr01(core, cdiop[idx], val_en,		pCfg->val_en);
	SDIO_Wr01(core, cdiop[idx], req_en,		pCfg->req_en);
	SDIO_Wr01(core, cdiop[idx], val_sel,		pCfg->val_sel);
	SDIO_Wr01(core, cdiop[idx], cdinout,		pCfg->cdin_out);
	SDIO_Wr01(core, cdiop[idx], pconf, 		pCfg->pconf);
	SDIO_Wr01(core, cdiop[idx], clk_div,		pCfg->clk_div);
	SDIO_Wr01(core, cdiop[idx], sync_type, 	pCfg->sync_type);

	SDIO_WrFL(core, cdiop[idx]);

	ret = RET_OK;

	return ret;
}

/*  End of BDRC Stub for TS Output */


int SDEC_HAL_SetGSTCClkSource(UINT8 core, UINT8 gstc_index, UINT8 fixed_sysclk)
{
	int ret = RET_ERROR;

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		if(gstc_index == 0)
		{
			SDIO_RdFL(core, sysclk_sel);
			SDIO_Wr01(core, sysclk_sel, gstcc_src_sel, fixed_sysclk);
			SDIO_Wr01(core, sysclk_sel, gstcc_mode, fixed_sysclk);
			SDIO_WrFL(core, sysclk_sel);
		}
		else if(gstc_index == 1)
		{
			SDIO_RdFL(core, sysclk_sel);
			SDIO_Wr01(core, sysclk_sel, gstcc1_src_sel, fixed_sysclk);
			SDIO_Wr01(core, sysclk_sel, gstcc1_mode, fixed_sysclk);
			SDIO_WrFL(core, sysclk_sel);
		}
	}

	ret = RET_OK;
	return ret;
}

int SDEC_HAL_SetTimeStampClkSource(UINT8 core, UINT8 fixed_sysclk)
{
	int ret = RET_ERROR;

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		SDIO_RdFL(core, sysclk_sel);
		if(fixed_sysclk)
		{
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_a, 1);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_b, 1);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_c, 1);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_d, 1);
		}
		else
		{
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_a, 0);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_b, 0);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_c, 0);
			SDIO_Wr01(core, sysclk_sel, tmstamp_src_sel_d, 0);
		}
		SDIO_WrFL(core, sysclk_sel);
	}

	ret = RET_OK;
	return ret;
}




