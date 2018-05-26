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
 *  register access interface for the LG1150 hardware pvr
 *
 *  author		Murugan Durairaj (murugan.d@lge.com)
 *  version		1.1
 *  date		2010.03.20
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
#include "pvr_reg_ctrl.h"
#include "pvr_reg.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PVR_REG_TRACE_ENTER(format, args...) \
	PVR_PRINT_REG("<== PVR_REG(CH_%c) %s"format, ch?'B':'A', __FUNCTION__, ##args)
#define PVR_REG_TRACE_EXIT(format, args...) \
	PVR_PRINT_REG("==> PVR_REG(CH_%c) %s"format, ch?'B':'A', __FUNCTION__, ##args)

#define PVR_REG_EX_TRACE_ENTER(format, args...) \
	PVR_PRINT_REG_EX("<== PVR_REG(CH_%c) %s"format, ch?'B':'A', __FUNCTION__, ##args)
#define PVR_REG_EX_TRACE_EXIT(format, args...) \
	PVR_PRINT_REG_EX("==> PVR_REG(CH_%c) %s"format, ch?'B':'A', __FUNCTION__, ##args)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
DVR_REG_CTRL_T	*g_dvr_reg_ctrl;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static spinlock_t pvr_reg_lock;


/*========================================================================================
	Implementation Group
========================================================================================*/

int PVR_REG_Init(void)
{
	LX_PVR_CFG_T* cfg;
	volatile UINT32* phys_base;
	UINT32* shdw_base;

	cfg = PVR_CFG_GetConfig();

	g_dvr_reg_ctrl = (DVR_REG_CTRL_T*)OS_KMalloc(sizeof(DVR_REG_CTRL_T));
	PVR_CHECK_ERROR(g_dvr_reg_ctrl == NULL, return RET_ERROR, "kmalloc fail");

	phys_base = (volatile UINT32*)ioremap(cfg->regBase, PVR_REG_SIZE);
	PVR_CHECK_ERROR(phys_base == NULL, return RET_ERROR, "ioremap fail");

	shdw_base = (UINT32*)OS_KMalloc(PVR_REG_SIZE);
	PVR_CHECK_ERROR(shdw_base == NULL, return RET_ERROR, "kmalloc fail");

	g_dvr_reg_ctrl->phys.base = phys_base;
	g_dvr_reg_ctrl->shdw.base = shdw_base;


	/* TOP */
	g_dvr_reg_ctrl->phys.top = (volatile DVR_TOP_REG_T*)((ULONG)phys_base + PVR_TOP_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.top = (DVR_TOP_REG_T*)((ULONG)shdw_base + PVR_TOP_REG_OFFSET);

	/* PIE A */
	g_dvr_reg_ctrl->phys.pie[LX_PVR_CH_A] = (volatile DVR_PIE_REG_T*)((ULONG)phys_base + PVR_PIE_A_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.pie[LX_PVR_CH_A] = (DVR_PIE_REG_T*)((ULONG)shdw_base + PVR_PIE_A_REG_OFFSET);

	/* PIE B */
	g_dvr_reg_ctrl->phys.pie[LX_PVR_CH_B] = (volatile DVR_PIE_REG_T*)((ULONG)phys_base + PVR_PIE_B_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.pie[LX_PVR_CH_B] = (DVR_PIE_REG_T*)((ULONG)shdw_base + PVR_PIE_B_REG_OFFSET);

	/* DN A */
	g_dvr_reg_ctrl->phys.dn[LX_PVR_CH_A] = (volatile DVR_DN_REG_T*)((ULONG)phys_base + PVR_DN_A_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.dn[LX_PVR_CH_A] = (DVR_DN_REG_T*)((ULONG)shdw_base + PVR_DN_A_REG_OFFSET);

	/* DN B */
	g_dvr_reg_ctrl->phys.dn[LX_PVR_CH_B] = (volatile DVR_DN_REG_T*)((ULONG)phys_base + PVR_DN_B_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.dn[LX_PVR_CH_B] = (DVR_DN_REG_T*)((ULONG)shdw_base + PVR_DN_B_REG_OFFSET);

	/* UP A */
	g_dvr_reg_ctrl->phys.up[LX_PVR_CH_A] = (volatile DVR_UP_REG_T*)((ULONG)phys_base + PVR_UP_A_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.up[LX_PVR_CH_A] = (DVR_UP_REG_T*)((ULONG)shdw_base + PVR_UP_A_REG_OFFSET);

	/* UP B */
	g_dvr_reg_ctrl->phys.up[LX_PVR_CH_B] = (volatile DVR_UP_REG_T*)((ULONG)phys_base + PVR_UP_B_REG_OFFSET);
	g_dvr_reg_ctrl->shdw.up[LX_PVR_CH_B] = (DVR_UP_REG_T*)((ULONG)shdw_base + PVR_UP_B_REG_OFFSET);


	g_dvr_reg_ctrl->size[PVR_REG_BLOCK_TOP]	= 0x14;
	g_dvr_reg_ctrl->size[PVR_REG_BLOCK_PIE]	= 0x100;
	g_dvr_reg_ctrl->size[PVR_REG_BLOCK_DN]	= 0x24;
	g_dvr_reg_ctrl->size[PVR_REG_BLOCK_UP]	= 0x60;


	spin_lock_init (&pvr_reg_lock);

	return RET_OK;
}



/************ PVR TOP REGISTER **********************/
void PVR_REG_EnableIntr(UINT32 bits, BOOLEAN enable)
{
	unsigned long flag;
	UINT32 value;

	PVR_PRINT_REG_EX("<== PVR_REG %s(bits=0x%08x, enable=%d)", __FUNCTION__, bits, enable);

	spin_lock_irqsave (&pvr_reg_lock, flag);
	value = PVR_TOP_REG_READ32(intr_en);
	if(enable)	value |= bits;
	else		value &= ~bits;
	PVR_TOP_REG_WRITE32(intr_en, value);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}


/********** PVR DOWNLOAD REGISTER ********************/

void DVR_DN_EnableReg(LX_PVR_CH_T ch, BOOLEAN enable)
{
	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_DN_RdFL(ch, dn_ctrl);
	PVR_DN_Wr01(ch, dn_ctrl, record, enable);
	PVR_DN_WrFL(ch, dn_ctrl);
}

void DVR_DN_PauseReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_DN_RdFL(ch, dn_ctrl);
	PVR_DN_Wr01(ch, dn_ctrl, paus_rec, enable);
	PVR_DN_WrFL(ch, dn_ctrl);
}

void DVR_DN_EnableINTReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	UINT32 bits;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	bits = (ch == LX_PVR_CH_A) ?
			(PVR_INTR_UNIT_BUF_A | PVR_INTR_DN_ERR_A) :
			(PVR_INTR_UNIT_BUF_B | PVR_INTR_DN_ERR_B);

	PVR_REG_EnableIntr(bits, enable);
}

void DVR_DN_ConfigureIntrLevel(LX_PVR_CH_T ch, UINT32 bufNumLim, UINT32 pktNumLim )
{
	PVR_REG_TRACE_ENTER("(bufNumLim=%d,pktNumLim=%d)", bufNumLim, pktNumLim);

	/* Configure the packet limit and buffer limit for interrupt */
	PVR_DN_RdFL(ch, dn_buf_lim);
	PVR_DN_Wr01(ch, dn_buf_lim, pkt_cnt_lim, pktNumLim & 0x3FFF);
	PVR_DN_Wr01(ch, dn_buf_lim, buf_num_lim, bufNumLim & 0x1F);
	PVR_DN_WrFL(ch, dn_buf_lim);
}

void DVR_DN_GetPacketBufLimit(LX_PVR_CH_T ch ,UINT32 *pktNumLim, UINT32 *bufNumLim )
{
	PVR_DN_RdFL(ch, dn_buf_lim);
	PVR_DN_Rd01(ch, dn_buf_lim, pkt_cnt_lim, *pktNumLim);
	PVR_DN_Rd01(ch, dn_buf_lim, buf_num_lim, *bufNumLim);

	PVR_REG_TRACE_EXIT("(*pktNumLim=%d, *bufNumLin=%d)", *pktNumLim, *bufNumLim);
}

void DVR_DN_GetBufInfo(LX_PVR_CH_T ch, UINT32 *buf_num, UINT32 *pkt_cnt)
{
	PVR_DN_RdFL(ch, dn_buf_info);
	PVR_DN_Rd01(ch, dn_buf_info, buf_num, *buf_num);
	PVR_DN_Rd01(ch, dn_buf_info, pkt_cnt, *pkt_cnt);

	PVR_REG_TRACE_EXIT("(*buf_num=%d, *pkt_cnt=%d)", *buf_num, *pkt_cnt);
}

void DVR_DN_GetBufBoundReg( LX_PVR_CH_T ch, UINT32 *base, UINT32 *end)
{
	PVR_DN_RdFL(ch, dn_buf_sptr);
	PVR_DN_RdFL(ch, dn_buf_eptr);

	PVR_DN_Rd01(ch, dn_buf_sptr, dn_buf_sptr, *base);
	PVR_DN_Rd01(ch, dn_buf_eptr, dn_buf_eptr, *end);

	*base <<= 12;
	*end  <<= 12;

	PVR_REG_TRACE_EXIT("(*base=0x%08x, *end=0x%08x)", *base, *end);
}

void DVR_DN_SetBufBoundReg(LX_PVR_CH_T ch, UINT32 base, UINT32 end)
{
	PVR_REG_TRACE_ENTER("(base=0x%08x, end=0x%08x)", base, end);

	base >>= 12;
	end  >>= 12;

	PVR_DN_RdFL(ch, dn_buf_sptr);
	PVR_DN_RdFL(ch, dn_buf_eptr);

	PVR_DN_Wr01(ch, dn_buf_sptr, dn_buf_sptr, base);
	PVR_DN_Wr01(ch, dn_buf_eptr, dn_buf_eptr, end);

	PVR_DN_WrFL(ch, dn_buf_sptr);
	PVR_DN_WrFL(ch, dn_buf_eptr);
}

void DVR_DN_GetWritePtrReg( LX_PVR_CH_T ch, UINT32 *wptr)
{
	PVR_DN_RdFL(ch, dn_buf_wptr);
	PVR_DN_Rd01(ch, dn_buf_wptr, dn_buf_wptr, *wptr);
	*wptr <<= 3;

	PVR_REG_EX_TRACE_EXIT("(*wptr=0x%08x)", *wptr);
}

void DVR_DN_SetWritePtrReg(LX_PVR_CH_T ch, UINT32 wptr)
{
	PVR_REG_TRACE_ENTER("(wptr=0x%08x)", wptr);

	wptr >>= 3;
	PVR_DN_RdFL(ch, dn_buf_wptr);
	PVR_DN_Wr01(ch, dn_buf_wptr, dn_buf_wptr, wptr);
	PVR_DN_WrFL(ch, dn_buf_wptr);
}

void DVR_DN_SetPIDReg(LX_PVR_CH_T ch, UINT32 pid)
{
	PVR_REG_TRACE_ENTER("(pid=0x%x)", pid);

	PVR_DN_RdFL(ch, dn_vpid);
	PVR_DN_Wr01(ch, dn_vpid, pid, pid & 0x1fff);
	PVR_DN_WrFL(ch, dn_vpid);
}


/************ PVR UPLOAD REGISTER **********************/

void DVR_UP_EnableReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, upen, enable);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_UP_SetNumOfSyncError(LX_PVR_CH_T ch, UINT8 num)
{
	PVR_REG_TRACE_ENTER("(num=%d)", num);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, num_serr, num);
	PVR_UP_WrFL(ch, up_mode);
}

void DVR_UP_SetInputTSMode(LX_PVR_CH_T ch, UINT8 packet_len)
{
	PVR_REG_TRACE_ENTER("(packet_len=%d)", packet_len);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, its_mod, (packet_len == 192) ? 0 : 1);
	PVR_UP_WrFL(ch, up_mode);
}


void DVR_UP_CopyTimeStamp(LX_PVR_CH_T ch)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("()");

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, tcp, 1);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
	OS_UsecDelay(1);
}

void DVR_UP_AutoTimeStampCopyReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, atcp, enable);
	PVR_UP_WrFL(ch, up_mode);
}

void DVR_UP_SetTimeStampCheck(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, tsc, (enable) ? 0 : 1);
	PVR_UP_WrFL(ch, up_mode);
}

/*
 * PLAY_MOD:
 *	000 - Normal play
 *	001 - X2 Faster
 *	010 - X2 Slower
 *	011 - X4 Slower
 *	100 - Trick play mode
 */
void DVR_UP_ChangePlaymode(LX_PVR_CH_T ch , UINT8 playMode )
{
	PVR_REG_TRACE_ENTER("(playMode=%d)", playMode);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, play_mod, playMode);
	PVR_UP_WrFL(ch, up_mode);
}

BOOLEAN DVR_UP_GetEnable(LX_PVR_CH_T ch)
{
	BOOLEAN	enable;

	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Rd01(ch, up_conf, upen, enable);

	PVR_REG_TRACE_EXIT("() RETURN:%d", enable);

    return enable;
}

BOOLEAN DVR_UP_GetPause(LX_PVR_CH_T ch)
{
	BOOLEAN	enable;

	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Rd01(ch, up_conf, upps, enable);

	PVR_REG_TRACE_EXIT("() RETURN:%d", enable);

    return enable;
}

UINT32 DVR_UP_GetErrorStat(LX_PVR_CH_T ch)
{
	UINT32 value;

	PVR_UP_RdFL(ch, up_err_stat);
	PVR_UP_Rd(ch, up_err_stat, value);
	value &= (UP_ERR_STAT_OVERLAP | UP_ERR_STAT_SYNC_ERR);

	PVR_REG_EX_TRACE_EXIT("() RETURN:%d", value);

	return value;
}

void DVR_UP_PauseReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, flush, (enable) ? 0 : 1);
	PVR_UP_WrFL(ch, up_mode);

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, upps, (enable) ? 1 : 0);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_UP_RepPauseReg(LX_PVR_CH_T ch ,BOOLEAN enable, UINT32 pptr)
{
	unsigned long flag;
	UINT32 bits;

	PVR_REG_TRACE_ENTER("(enable=%d, pptr=0x%08x)", enable, pptr);

	if ( enable )
	{
		pptr >>= 2;
		PVR_UP_RdFL(ch, up_buf_pptr);
		PVR_UP_Wr01(ch, up_buf_pptr, up_buf_pptr, pptr);
		PVR_UP_WrFL(ch, up_buf_pptr);
	}

	bits = (ch == LX_PVR_CH_A) ? PVR_INTR_REP_A : PVR_INTR_REP_B;
	PVR_REG_EnableIntr(bits, enable);

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, rep, enable);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_UP_SetSpeedReg(LX_PVR_CH_T ch ,SINT32 speed)
{
	PVR_REG_TRACE_ENTER("(speed=%d)", speed);

	PVR_UP_RdFL(ch, up_max_jitter);
	PVR_UP_Wr01(ch, up_max_jitter, al_jitter, speed);
	PVR_UP_WrFL(ch, up_max_jitter);
}


void DVR_UP_SetWritePtrReg(LX_PVR_CH_T ch, UINT32 wptr)
{
	PVR_REG_EX_TRACE_ENTER("(wptr=0x%08x)", wptr);

	wptr >>= 2;
	PVR_UP_RdFL(ch, up_buf_wptr);
	PVR_UP_Wr01(ch, up_buf_wptr, up_buf_wptr, wptr);
	PVR_UP_WrFL(ch, up_buf_wptr);
}

void DVR_UP_SetReadPtrReg(LX_PVR_CH_T ch, UINT32 rptr)
{
	PVR_REG_TRACE_ENTER("(rptr=0x%08x)", rptr);

	rptr >>= 2;
	PVR_UP_RdFL(ch, up_buf_rptr);
	PVR_UP_Wr01(ch, up_buf_rptr, up_buf_rptr, rptr);
	PVR_UP_WrFL(ch, up_buf_rptr);
}

void DVR_UP_GetPointersReg(LX_PVR_CH_T ch, UINT32 *wptr, UINT32 *rptr)
{

    /* Read the write and read pointers & return to the caller */
	PVR_UP_RdFL(ch, up_buf_wptr);
	PVR_UP_RdFL(ch, up_buf_rptr);

	PVR_UP_Rd01(ch, up_buf_wptr, up_buf_wptr, *wptr);
	PVR_UP_Rd01(ch, up_buf_rptr, up_buf_rptr, *rptr);

	*wptr <<= 2;
	*rptr <<= 2;

	PVR_REG_EX_TRACE_EXIT("(*wptr=0x%08x, *rptr=0x%08x)", *wptr, *rptr);
}

void DVR_UP_SetBufBoundReg(LX_PVR_CH_T ch, UINT32 base, UINT32 end)
{
	PVR_REG_TRACE_ENTER("(base=0x%08x, end=0x%08x)", base, end);

	base >>= 12;
	end  >>= 12;

	PVR_UP_RdFL(ch, up_buf_sptr);
	PVR_UP_RdFL(ch, up_buf_eptr);

	PVR_UP_Wr01(ch, up_buf_sptr, up_buf_sptr, base);
	PVR_UP_Wr01(ch, up_buf_eptr, up_buf_eptr, end);

	PVR_UP_WrFL(ch, up_buf_sptr);
	PVR_UP_WrFL(ch, up_buf_eptr);
}

void DVR_UP_GetBufBoundReg(LX_PVR_CH_T ch, UINT32 *base, UINT32 *end)
{
	PVR_UP_RdFL(ch, up_buf_sptr);
	PVR_UP_RdFL(ch, up_buf_eptr);

	PVR_UP_Rd01(ch, up_buf_sptr, up_buf_sptr, *base);
	PVR_UP_Rd01(ch, up_buf_eptr, up_buf_eptr, *end);

	*base <<= 12;
	*end  <<= 12;

	PVR_REG_TRACE_EXIT("(*base=0x%08x, *end=0x%08x)", *base, *end);
}

void DVR_UP_ResetBlock(LX_PVR_CH_T ch)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("()");

	// Reset Upload Block with nm_rst 1-> 0
	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, nm_rst, 1);
	PVR_UP_WrFL(ch, up_conf);

	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, nm_rst, 0);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_DN_ResetBlock(LX_PVR_CH_T ch)
{
	PVR_REG_TRACE_ENTER("()");

	PVR_DN_RdFL(ch, dn_ctrl);
	PVR_DN_Wr01(ch, dn_ctrl, reset_sw, 1);
	PVR_DN_WrFL(ch, dn_ctrl);
}

void DVR_UP_EnableEmptyLevelReg(LX_PVR_CH_T ch, BOOLEAN enable)
{
	UINT32 al_imsk;
	UINT32 bits;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	bits = (ch == LX_PVR_CH_A) ? PVR_INTR_ALMOST_A : PVR_INTR_ALMOST_B;
	PVR_REG_EnableIntr(bits, enable);

	//Both full and empty interrupt masks are enabled time being
	al_imsk = enable ? 0 : (UP_BUF_STAT_AL_IST_FULL|UP_BUF_STAT_AL_IST_EMPTY);
	PVR_UP_RdFL(ch, up_buf_stat);
	PVR_UP_Wr01(ch, up_buf_stat, al_imsk, al_imsk);
	PVR_UP_WrFL(ch, up_buf_stat);

}

void DVR_UP_EnableErrIntr(LX_PVR_CH_T ch, BOOLEAN enable)
{
	UINT32 bits;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	bits = (ch == LX_PVR_CH_A) ? PVR_INTR_UP_ERR_A : PVR_INTR_UP_ERR_B;
	PVR_REG_EnableIntr(bits, enable);
}

void DVR_UP_SetEmptyLevelReg(LX_PVR_CH_T ch, UINT32 level)
{
	PVR_REG_TRACE_ENTER("(level=%d)", level);

	level  >>= 12;

	PVR_UP_RdFL(ch, up_al_empty_lev);
	PVR_UP_RdFL(ch, up_al_full_lev);

	PVR_UP_Wr01(ch, up_al_empty_lev, al_empty_lev, level);
	PVR_UP_Wr01(ch, up_al_full_lev, al_full_lev, level);

	PVR_UP_WrFL(ch, up_al_empty_lev);
	PVR_UP_WrFL(ch, up_al_full_lev);
}

void DVR_UP_GetTSCJitterReg(LX_PVR_CH_T ch, UINT32 *jitter)
{
	PVR_UP_RdFL(ch, up_tsc_jitter);
	PVR_UP_Rd01(ch, up_tsc_jitter, tsc_jitter, *jitter);

	PVR_REG_TRACE_EXIT("(*jitter=%d)", *jitter);
}

void DVR_UP_SetIntrAck(LX_PVR_CH_T ch, UINT8 value)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("(value=%d)", value);

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_UP_RdFL(ch, up_conf);
	PVR_UP_Wr01(ch, up_conf, iack, value);
	PVR_UP_WrFL(ch, up_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_UP_SetWaitCycle (LX_PVR_CH_T ch, UINT16 waitCycle )
{
	PVR_REG_TRACE_ENTER("(waitCycle=%d)", waitCycle);

	PVR_UP_RdFL(ch, up_mode);
	PVR_UP_Wr01(ch, up_mode, wait_cycle, waitCycle);
	PVR_UP_WrFL(ch, up_mode);
}

void DVR_UP_SetTimeStampClkSource(LX_PVR_CH_T ch, UINT32 fixed_clk)
{
	PVR_REG_TRACE_ENTER("(fixed_clk=%d)", fixed_clk);

	PVR_UP_RdFL(ch, up_tmstamp_sel);
	PVR_UP_Wr01(ch, up_tmstamp_sel, tmstamp_sel, (fixed_clk) ? 1 : 0);
	PVR_UP_WrFL(ch, up_tmstamp_sel);
}

UINT32 DVR_UP_GetStat0(LX_PVR_CH_T ch)
{
	UINT32 value;

	PVR_UP_RdFL(ch, up_stat0);
	PVR_UP_Rd(ch, up_stat0, value);

	PVR_REG_EX_TRACE_EXIT("() RETURN:0x%x", value);

	return value;
}

/************ PVR PIE REGISTER ***************************/

void DVR_PIE_SetModeReg(LX_PVR_CH_T ch, UINT32 mode)
{
	PVR_REG_TRACE_ENTER("(mode=%d)", mode);

	PVR_PIE_RdFL(ch, pie_mode);
	PVR_PIE_Wr01(ch, pie_mode, pie_mode, mode & 0x3);
	PVR_PIE_WrFL(ch, pie_mode);
}

void DVR_PIE_ConfigureSCD(LX_PVR_CH_T ch, UINT8 index, UINT32 mask, UINT32 value, UINT8 enable )
{
	PVR_REG_TRACE_ENTER("(index=%d, mask=%d, value=%d, enable=%d)", index, mask, value, enable);

	if(index > 3) return;

	PVR_PIE_Wr(ch, pie_scd_mask[index], mask);
	PVR_PIE_WrFL(ch, pie_scd_mask[index]);

	PVR_PIE_Wr(ch, pie_scd_value[index], value);
	PVR_PIE_WrFL(ch, pie_scd_value[index]);

	PVR_PIE_RdFL(ch, pie_mode);
	switch(index)
	{
		case 0: PVR_PIE_Wr01(ch, pie_mode, scd0_en, enable); break;
		case 1: PVR_PIE_Wr01(ch, pie_mode, scd1_en, enable); break;
		case 2: PVR_PIE_Wr01(ch, pie_mode, scd2_en, enable); break;
		case 3: PVR_PIE_Wr01(ch, pie_mode, scd3_en, enable); break;
		default: break;
	}
	PVR_PIE_WrFL(ch, pie_mode);
}

void DVR_PIE_GscdByteInfoConfig(LX_PVR_CH_T ch, UINT8 byteSel0, UINT8 byteSel1, UINT8 byteSel2, UINT8 byteSel3 )
{
	PVR_REG_TRACE_ENTER("(byteSel0=%d, byteSel1=%d, byteSel2=%d, byteSel3=%d)",
		byteSel0, byteSel1, byteSel2, byteSel3);

	PVR_PIE_RdFL(ch, pie_mode);
	PVR_PIE_Wr01(ch, pie_mode, scd0_bsel, byteSel0);
	PVR_PIE_Wr01(ch, pie_mode, scd1_bsel, byteSel1);
	PVR_PIE_Wr01(ch, pie_mode, scd2_bsel, byteSel2);
	PVR_PIE_Wr01(ch, pie_mode, scd3_bsel, byteSel3);
	PVR_PIE_WrFL(ch, pie_mode);
}

void DVR_PIE_EnableSCDReg(LX_PVR_CH_T ch, BOOLEAN enable)
{
	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	PVR_PIE_RdFL(ch, pie_mode);
	PVR_PIE_Wr01(ch, pie_mode, gscd_en, enable);
	PVR_PIE_WrFL(ch, pie_mode);
}

void DVR_PIE_EnableINTReg(LX_PVR_CH_T ch ,BOOLEAN enable)
{
	UINT32 bits;

	PVR_REG_TRACE_ENTER("(enable=%d)", enable);

	bits = (ch == LX_PVR_CH_A) ? PVR_INTR_SCD_A : PVR_INTR_SCD_B;
	PVR_REG_EnableIntr(bits, enable);
}


void DVR_PIE_SetIntrAck(LX_PVR_CH_T ch)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("()");

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_PIE_RdFL(ch, pie_conf);
	PVR_PIE_Wr01(ch, pie_conf, iack, 1);
	PVR_PIE_WrFL(ch, pie_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}

void DVR_PIE_ResetBlock(LX_PVR_CH_T ch)
{
	unsigned long flag;

	PVR_REG_TRACE_ENTER("()");

	spin_lock_irqsave (&pvr_reg_lock, flag);
	PVR_PIE_RdFL(ch, pie_conf);
	PVR_PIE_Wr01(ch, pie_conf, rst, 1);
	PVR_PIE_WrFL(ch, pie_conf);
	spin_unlock_irqrestore (&pvr_reg_lock, flag);
}


