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
 *  main driver implementation for pvr device.
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  author		murugan.d (murugan.d@lge.com)
 *  version		1.0
 *  date		2010.02.05
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "pvr_impl.h"
#include "pvr_drv.h"
#include "pvr_core.h"
#include "pvr_reg.h"
#include "pvr_pie.h"
#include "pvr_reg_ctrl.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/



#define PVR_ISR_INFO(ch,fmt,args...)		PVR_PRINT_ISR("PVR ISR(CH_%c)> "fmt, ch+'A', ##args)


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/



/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * Function to calculate the current download buffer level based on
 * memorized write and read pointers
 */
static UINT32 CheckDownloadSpace(PVR_CTRL_T *ctrl)
{
	UINT32 size;
	size = (ctrl->buf.dn.rptr + ctrl->buf.dn.size - (ctrl->buf.dn.wptr + 1)) % ctrl->buf.dn.size;

	if(size <= ctrl->unit_buf.size)
	{
		ctrl->buf_state.dn = LX_PVR_BUF_STAT_FULL;
		ctrl->stats.dn_overflow++;
		if(ctrl->dn.errmsg_enable)
		{
			/* In this case, the system was almost crashed */
			ctrl->dn.errmsg_enable = 0;		// disable error message
			PVR_PRINT_ERROR("PVR-UNITBUF : Download Buffer Overflow");
		}
	}
	else if(size < ctrl->dn.warn_bufsize)
	{
		if(ctrl->dn.errmsg_enable)
		{
			UINT32 elapsed = jiffies_to_msecs(jiffies - ctrl->dn.warn_time);
			if(elapsed >= ctrl->dn.warn_interval)
			{
				PVR_PRINT_WARNING("PVR-UNITBUF : Download Buffer Almost Full - %dKbytes remained", size/1024);
				ctrl->dn.warn_time = jiffies;
			}
		}
	}

	return size;
}


#define		_PVR_GET_PICTURE(data)		((data&0x00C00000) == 0x00C00000) ? "SI" :\
										((data&0x00800000) == 0x00800000) ? "S" :\
										((data&0x00400000) == 0x00400000) ? "I" :\
										((data&0x00200000) == 0x00200000) ? "P" :\
										((data&0x00100000) == 0x00100000) ? "B" : "NONE"


static void PVR_ISR_DN_UnitBuf(LX_PVR_CH_T ch)
{
	PVR_CTRL_T *ctrl = PVR_GetCtrl(ch);
	UINT32	unitbuf_num;
	UINT32	buf_num, pkt_cnt;
	UINT32	wptr;

	/* Get H/W register value */
	DVR_DN_GetWritePtrReg(ch, &wptr);
	DVR_DN_GetBufInfo(ch, &buf_num, &pkt_cnt);

	PVR_ISR_INFO(ch, "DN UNIT BUFFER. buf_num[%d], pkt_cnt[%d]", buf_num, pkt_cnt);

	CheckDownloadSpace(ctrl);

	ctrl->stats.dn_unitbuf++;

	/* Update Write PTR */
	ctrl->buf.dn.wptr += ctrl->unit_buf.size;
	if(ctrl->buf.dn.wptr >= ctrl->buf.dn.end)
		ctrl->buf.dn.wptr = ctrl->buf.dn.base;	/* Alignd to unit buf size */

	/* Get next unit buffer num */
	ctrl->unit_buf.curr = (ctrl->unit_buf.curr + 1) % ctrl->unit_buf.total;


	/* Check H/W BUF_NUM */
	if(buf_num != ctrl->unit_buf.buf_num)
	{
		PVR_PRINT_WARNING("PVR-UNITBUF : BUF NUM ERROR. REG[%d],VAR[%d]", buf_num, ctrl->unit_buf.buf_num);
	}
	ctrl->unit_buf.buf_num = (buf_num + 1) % PVR_DN_BUF_NUM_MAX;	/* BUF_NUM informs the downloaded buffer counter */

	/* Check H/W wptr */
	unitbuf_num = (wptr - ctrl->buf.dn.base)/ctrl->unit_buf.size;	/* Unit Buffer Num base on H/W wptr */

	wptr = ctrl->buf.dn.base + unitbuf_num * ctrl->unit_buf.size;	/* alignd to unit buf size */
	if(ctrl->buf.dn.wptr != wptr)
	{
		PVR_PRINT_WARNING("wptr:0x%x, hw:0x%x", ctrl->buf.dn.wptr, wptr);
		ctrl->buf.dn.wptr = wptr;
	}

	/* Check S/W unit buffer num */
	if(unitbuf_num != ctrl->unit_buf.curr)
	{
		PVR_PRINT_WARNING("PVR-UNITBUF : BUF NUM MISMATCHED. H/W[%d] S/W[%d]", unitbuf_num, ctrl->unit_buf.curr);
		ctrl->unit_buf.curr = unitbuf_num;
	}

	PVR_KDRV_LOG( PVR_PIE_DEBUG ,"ISR: UNIT_BUF Ch[%d] BufNum[%d]", (UINT32)ch, unitbuf_num);
}


static void PVR_ISR_UP_AlmostFullAndEmpty(LX_PVR_CH_T ch)
{
	UINT32 al_ist;	/* Almost Interrupt Status. Full or Empty */
	PVR_CTRL_T *ctrl = PVR_GetCtrl(ch);

	PVR_UP_RdFL(ch, up_buf_stat);
	PVR_UP_Rd(ch, up_buf_stat, al_ist);

	if (al_ist & UP_BUF_STAT_AL_IST_EMPTY)
	{
		PVR_ISR_INFO(ch, "UP Almost Empty");
		// Signal to consumer
		ctrl->stats.up_almost_empty++;

		//Disable the Almost intr before ack, it will be enabled after updating write ptr

		// TODO: almost interrupt전체에 대해 disable을 수행한다. 맞나?
		if(ch == LX_PVR_CH_A)	PVR_REG_EnableIntr(PVR_INTR_ALMOST_A, FALSE);
		else					PVR_REG_EnableIntr(PVR_INTR_ALMOST_B, FALSE);
	}

	if (al_ist & UP_BUF_STAT_AL_IST_FULL )
	{
		PVR_ISR_INFO(ch, "UP Almost Full");
		DVR_UP_SetIntrAck(ch, UP_CONF_IACK_ALMOST_IACK);
	}

}

static void PVR_ISR_UP_Error(LX_PVR_CH_T ch)
{
	UP_ERR_STAT stat;
	PVR_CTRL_T *ctrl = PVR_GetCtrl(ch);

	stat = PVR_UP_REG_READ(ch, up_err_stat);
	if(stat.overlap)
	{
		PVR_ISR_INFO(ch, "UP Error overlap");
		ctrl->stats.up_overlap++;
		ctrl->err_stat.up_overlap = 1;
		/* Set interrupt ack after the write pointer updated */
	}

	if(stat.sync_err)
	{
		PVR_ISR_INFO(ch, "UP Error sync");
		ctrl->stats.up_sync_err++;

		//ACK in case of interrupts not handled
		DVR_UP_SetIntrAck(ch, UP_CONF_IACK_ERR_IACK);
	}
}

static void PVR_ISR_UP_ReadEndPause(LX_PVR_CH_T ch)
{
	PVR_ISR_INFO(ch, "UP Read End Pause");

	if(ch == LX_PVR_CH_A)	PVR_REG_EnableIntr(PVR_INTR_REP_A, FALSE);
	else					PVR_REG_EnableIntr(PVR_INTR_REP_B, FALSE);
}

static void PVR_ISR_PIE_StartCodeDetection(LX_PVR_CH_T ch)
{
	PVR_CTRL_T *ctrl = PVR_GetCtrl(ch);
	UINT32 ui32BufNum;
	UINT32 idx;

#ifndef USE_NEW_PIE_EXTRACTION
	UINT32 ui32WrIdx = 0;
#endif

	PVR_PIE_IND_T pie_ind;
	PVR_PIE_IND_T pind[32];
	UINT32 pind_cnt;


	PVR_PIE_RdFL(ch, pie_stat);
	PVR_PIE_Rd01(ch, pie_stat, pind_cnt, pind_cnt);

	PVR_ISR_INFO(ch, "PIE SCD. cnt=%d", pind_cnt);

#ifndef USE_NEW_PIE_EXTRACTION
	ui32WrIdx = ctrl->ui32PieIndex;
#endif
	ui32BufNum = ctrl->unit_buf.curr;

	PVR_PRINT_PIE_DEBUG("PIE-ISR: COUNT=%d, BufNum=%d", pind_cnt, ui32BufNum);
	for(idx=0; idx<pind_cnt; idx++)
	{
		pie_ind.val = PVR_PIE_REG_READ32(ch, pie_ind[idx]);

#ifdef USE_NEW_PIE_EXTRACTION
		pind[idx].val = pie_ind.val;
		PVR_PRINT_PIE_DEBUG("PIE-ISR:D[0x%08x] 2ndByte[0x%02x] MAXP[%d],SCD/BPIS[%d%d%d%d] BUF_NUM[%d] PKT_CNT[%d]",
							pie_ind.val,
							pie_ind.gscd.byte_info,
							pie_ind.gscd.maxp,
							pie_ind.gscd.scd0, pie_ind.gscd.scd1,
							pie_ind.gscd.scd2, pie_ind.gscd.scd3,
							pie_ind.gscd.buf_num, pie_ind.gscd.pack_cnt);
#else
		if(ctrl->pie_table.type == LX_PVR_PIE_TYPE_HEVCTS)
		{
			pind[idx].val = pie_ind.val;
			PVR_KDRV_LOG( PVR_PIE_DEBUG ,"PIE-ISR:D[0x%08x] 2ndByte[0x%02x] MAXP[%d],SCD[%d%d%d%d] BUF_NUM[%d] PKT_CNT[%d]",
							pie_ind.val,
							pie_ind.gscd.byte_info,
							pie_ind.gscd.maxp,
							pie_ind.gscd.scd0, pie_ind.gscd.scd1,
							pie_ind.gscd.scd2, pie_ind.gscd.scd3,
							pie_ind.gscd.buf_num, pie_ind.gscd.pack_cnt);
		}
		else
		{
			PVR_KDRV_LOG( PVR_PIE_DEBUG ,"PIE-ISR: Wr[%d] D[0x%08x] [%s] Buf[%d] BUF_NUM[%d] PKT_CNT[%d]",
				ui32WrIdx, pie_ind.val, _PVR_GET_PICTURE(pie_ind.val), ui32BufNum,
				pie_ind.mpeg2.buf_num, pie_ind.mpeg2.pack_cnt);

			ctrl->astPieTable[ui32WrIdx].ui32Val = pie_ind.val;
			ctrl->astBufTable[ui32WrIdx] = ui32BufNum;
			++ui32WrIdx;
			if (ui32WrIdx == PIE_MAX_ENTRIES_LOCAL)
			{
				/* Wrap around */
				ui32WrIdx = 0;
			}
		}
#endif

	}

#ifdef USE_NEW_PIE_EXTRACTION
	PVR_PIE_PutIndex(ctrl, pind, pind_cnt);
#else
	if(ctrl->pie_table.type == LX_PVR_PIE_TYPE_HEVCTS)
	{
		PVR_PIE_PutIndex(ctrl, pind, pind_cnt);
	}
	else
	{
		ctrl->ui32PieIndex = ui32WrIdx;
	}
#endif

	// Ack the pie interrupt
	DVR_PIE_SetIntrAck(ch);
}

static irqreturn_t PVR_ISR_Handler(int irq, void *dev_id)
{
	UINT32	intrSt;

	intrSt = PVR_TOP_REG_READ32(intr_st);

	//Clear the interrupts
	PVR_TOP_REG_WRITE32(intr_cl, intrSt);

//#define PVR_DEBUG_ISR_INTERVAL
#ifdef PVR_DEBUG_ISR_INTERVAL
	{
		static UINT64 tick = 0;
		UINT64 t = OS_GetUsecTicks();
		PVR_PRINT_NOTI("INTERVAL : %dus. STATUS:0x%08x\n", (UINT32)(t - tick), intrSt);
		tick = t;
	}
#endif


	/* UPLOAD */
	if (intrSt &  PVR_INTR_UP_MASK )
	{
		/* Upload Almost Full/Empty interrupt */
		if (intrSt & PVR_INTR_ALMOST_A) PVR_ISR_UP_AlmostFullAndEmpty(LX_PVR_CH_A);
		if (intrSt & PVR_INTR_ALMOST_B) PVR_ISR_UP_AlmostFullAndEmpty(LX_PVR_CH_B);

		/* UP Error interrupt */
		if (intrSt & PVR_INTR_UP_ERR_A) PVR_ISR_UP_Error(LX_PVR_CH_A);
		if (intrSt & PVR_INTR_UP_ERR_B) PVR_ISR_UP_Error(LX_PVR_CH_B);

		/* UP Read End Pause */
		if (intrSt & PVR_INTR_REP_A) PVR_ISR_UP_ReadEndPause(LX_PVR_CH_A);
		if (intrSt & PVR_INTR_REP_B) PVR_ISR_UP_ReadEndPause(LX_PVR_CH_B);
	}

	/* PIE */
	if (intrSt & PVR_INTR_PIE_MASK )
	{
		if (intrSt & PVR_INTR_SCD_A) PVR_ISR_PIE_StartCodeDetection(LX_PVR_CH_A);
		if (intrSt & PVR_INTR_SCD_B) PVR_ISR_PIE_StartCodeDetection(LX_PVR_CH_B);
	}

	/* DOWNLOAD */
	if (intrSt & PVR_INTR_DN_MASK )
	{
		if (intrSt & PVR_INTR_UNIT_BUF_A) PVR_ISR_DN_UnitBuf(LX_PVR_CH_A);
		if (intrSt & PVR_INTR_UNIT_BUF_B) PVR_ISR_DN_UnitBuf(LX_PVR_CH_B);
	}

	/* Ignore all other unhandled interrupts */


	return IRQ_HANDLED;
}



int PVR_ISR_Init(void)
{
	LX_PVR_CFG_T* cfg;

	cfg = PVR_CFG_GetConfig();

	if (request_irq(cfg->irqNum, PVR_ISR_Handler, 0,"pvr", cfg))
	{
		PVR_KDRV_LOG( PVR_ERROR ,"request_irq failed\n");
		return -1;
	}
	PVR_KDRV_LOG( PVR_TRACE ,"Inserting PVR IRQ[%d] Success\n", cfg->irqNum);

	return 0;
}


void PVR_ISR_Cleanup(void)
{
	LX_PVR_CFG_T* cfg;

	cfg = PVR_CFG_GetConfig();

	free_irq(cfg->irqNum, cfg);
}

