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
#include "os_util.h"

#include "sdec_impl.h"
#include "sdec_hal.h"
#include "sdec_reg_ctrl.h"


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
MPG_REG_CTRL_T *g_sdec_mpg_reg_ctrl;


/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/


int SDEC_HAL_MPGInit(void)
{
	int core, ch, num, idx;
	LX_SDEC_CFG_T *cfg;

	cfg = SDEC_CFG_GetConfig();

	g_sdec_mpg_reg_ctrl = (MPG_REG_CTRL_T*)OS_KMalloc(sizeof(MPG_REG_CTRL_T) * SDEC_MAX_CORE);
	SDEC_CHECK_ERROR(g_sdec_mpg_reg_ctrl == NULL, return RET_ERROR, "kmalloc fail");

	for(core=0; core<cfg->nCore; core++)
	{
		g_sdec_mpg_reg_ctrl[core].size = sizeof(MPG_REG_T);

		for(ch=0; ch<2; ch++)
		{
			g_sdec_mpg_reg_ctrl[core].phys[ch].addr = (volatile UINT32*)ioremap(cfg->mpgRegBase[core][ch], sizeof(MPG_REG_T));
			SDEC_CHECK_ERROR(g_sdec_mpg_reg_ctrl[core].phys[ch].addr == NULL, return RET_ERROR, "ioremap fail");

			g_sdec_mpg_reg_ctrl[core].shdw[ch].addr = (UINT32*)OS_KMalloc(sizeof(MPG_REG_T));
			SDEC_CHECK_ERROR(g_sdec_mpg_reg_ctrl[core].shdw[ch].addr == NULL, return RET_ERROR, "kmalloc fail");

			for(num = 0; num < 64; num++)
			{
				g_sdec_mpg_reg_ctrl[core].pidf_data[ch][num] = 0x1fff0000;
			}

			for(num=0; num<64; num++)
			{
				for(idx=0; idx<7; idx++)
				{
					g_sdec_mpg_reg_ctrl[core].secf_data[ch][num][idx] = 0xffffffff;
				}
			}
		}
	}

	return RET_OK;
}


TPI_ISTAT SDEC_HAL_TPIGetIntrStat(UINT8 ch)
{
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, tpi_istat);
	MPG_Rd(ch, tpi_istat, val);

exit:
	return *(TPI_ISTAT*)&val;
}

SE_ISTAT SDEC_HAL_SEGetIntrStat(UINT8 ch)
{
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, se_istat);
	MPG_Rd(ch, se_istat, val);

exit:
	return *(SE_ISTAT*)&val;
}

int SDEC_HAL_ConfSetPESReadyCheck(UINT8 ch, UINT8 chk_ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, conf);

	switch(chk_ch)
	{
		case 0:		MPG_Wr01(ch, conf, pes_rdy_chk_a, val);	break;
		case 1:		MPG_Wr01(ch, conf, pes_rdy_chk_b, val);	break;
		default:	goto exit;
	}

	MPG_WrFL(ch, conf);

	ret = RET_OK;

exit:
	return ret;
}

CHAN_STAT SDEC_HAL_GetChannelStatus(UINT8 ch)
{
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, chan_stat);
	MPG_Rd(ch, chan_stat, val);

exit:
	return *(CHAN_STAT*)&val;
}

UINT32 SDEC_HAL_GetChannelStatus2(UINT8 ch)
{
	UINT32			val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, chan_stat);
	MPG_Rd(ch, chan_stat, val);

exit:
	return val;
}

UINT8 SDEC_HAL_ChanStatGetMWFOverFlow(UINT8 ch)
{
	UINT32			val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, chan_stat);
	MPG_Rd(ch, chan_stat, val);

exit:
	return ((( val & 0x00400000) == 0x00400000) ? 1 : 0);
}


UINT32 SDEC_HAL_CCCheckEnableGet(UINT8 ch, UINT8 idx)
{
	UINT32			val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, cc_chk_en[idx]);
	MPG_Rd(ch, cc_chk_en[idx], val);

exit:
	return val;
}

int SDEC_HAL_CCCheckEnableSet(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, cc_chk_en[idx]);
	MPG_Wr(ch, cc_chk_en[idx], val);
	MPG_WrFL(ch, cc_chk_en[idx]);

exit:
	return ret;
}

int SDEC_HAL_ExtConfSECIDcont(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, seci_dcont, val);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_ExtConfSECICCError(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, seci_cce, val);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_ExtConfVideoDupPacket(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, dpkt_vid, val);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_ExtConfDcontDupPacket(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, dpkt_dcont, val);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_ExtConfGPBOverWrite(UINT8 ch, UINT8 en)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, gpb_ow, en);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_ExtConfGPBFullLevel(UINT8 ch, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, ext_conf);
	MPG_Wr01(ch, ext_conf, gpb_f_lev, val);
	MPG_WrFL(ch, ext_conf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, tpi_iconf);
	MPG_Wr01(ch, tpi_iconf, pusi, val);
	MPG_WrFL(ch, tpi_iconf);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_TPISetIntrAutoScCheck(UINT8 ch, UINT8 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, tpi_iconf);
	MPG_Wr01(ch, tpi_iconf, auto_sc_chk, val);
	MPG_WrFL(ch, tpi_iconf);

	ret = RET_OK;
exit:
	return ret;
}


UINT32 SDEC_HAL_GPBGetDataIntrStat(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_d_istat[idx]);
	MPG_Rd01(ch, gpb_d_istat[idx], gpb_d_istat, val);
	MPG_WrFL(ch, gpb_d_istat[idx]);
exit:
	return val;
}

int SDEC_HAL_GPBSetFullIntr(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	/* n/2 th pid filter is mapped to mth bit section filter. */
	/* (n /2 + 1) th pid filter is mapped to (m + 32) th bit section filter.  because one register can handle only 32-bits */
	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, gpb_f_iconf[map_idx]);
	MPG_Rd(ch, gpb_f_iconf[map_idx], val);
	MPG_Wr01(ch, gpb_f_iconf[map_idx], gpb_f_iconf, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, gpb_f_iconf[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_GPBClearFullIntr(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	/* n/2 th pid filter is mapped to mth bit section filter. */
	/* (n /2 + 1) th pid filter is mapped to (m + 32) th bit section filter.  because one register can handle only 32-bits */
	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, gpb_f_iconf[map_idx]);
	MPG_Rd(ch, gpb_f_iconf[map_idx], val);
	MPG_Wr01(ch, gpb_f_iconf[map_idx], gpb_f_iconf, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, gpb_f_iconf[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_GPBGetFullIntrStat(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_f_istat[idx]);
	MPG_Rd01(ch, gpb_f_istat[idx], gpb_f_istat, val);
	MPG_WrFL(ch, gpb_f_istat[idx]);
exit:
	return val;
}


UINT32 SDEC_HAL_GPBGetWritePtr(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_w_ptr[idx]);
	MPG_Rd01(ch, gpb_w_ptr[idx], gpb_w_ptr, val);
exit:
	return val;
}


UINT32 SDEC_HAL_GPBGetReadPtr(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_r_ptr[idx]);
	MPG_Rd01(ch, gpb_r_ptr[idx], gpb_r_ptr, val);
exit:
	return val;
}

int SDEC_HAL_GPBSetReadPtr(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_r_ptr[idx]);
	MPG_Wr01(ch, gpb_r_ptr[idx], gpb_r_ptr, val);
	MPG_WrFL(ch, gpb_r_ptr[idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_GPBSetWritePtr(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_w_ptr[idx]);
	MPG_Wr01(ch, gpb_w_ptr[idx], gpb_w_ptr, val);
	MPG_WrFL(ch, gpb_w_ptr[idx]);

	ret = RET_OK;
exit:
	return ret;
}


UINT32 SDEC_HAL_GPBGetLowerBnd(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_bnd[idx]);
	MPG_Rd01(ch, gpb_bnd[idx], l_bnd, val);
exit:
	return val;
}

UINT32 SDEC_HAL_GPBGetUpperBnd(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_bnd[idx]);
	MPG_Rd01(ch, gpb_bnd[idx], u_bnd, val);
exit:
	return val;
}

int SDEC_HAL_GPBSetBnd(UINT8 ch, UINT8 idx, UINT32 l_bnd, UINT32 u_bnd)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, gpb_bnd[idx]);
	MPG_Wr01(ch, gpb_bnd[idx], l_bnd, l_bnd);
	MPG_Wr01(ch, gpb_bnd[idx], u_bnd, u_bnd);
	MPG_WrFL(ch, gpb_bnd[idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_KMEMSet(UINT8 ch, UINT8 key_type, UINT8 pid_idx, UINT8 odd_key, UINT8 word_idx, UINT32 val)
{
	int ret = RET_ERROR;

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);
	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	/* set pid filter index */
	MPG_Wr01(ch, kmem_addr, key_type, key_type);
	MPG_Wr01(ch, kmem_addr, word_idx, word_idx);
	MPG_Wr01(ch, kmem_addr, pid_idx, pid_idx);
	MPG_Wr01(ch, kmem_addr, odd_key, odd_key);
	MPG_WrFL(ch, kmem_addr);

	/* write data */
	val = swab32(val);
	MPG_RdFL(ch, kmem_data);
	MPG_Wr01(ch, kmem_data, kmem_data, val);
	MPG_WrFL(ch, kmem_data);
#if 0
	/* H14_TBD, M14_TBD, why we use direct access instead of macro? jinhwan.bae */
	if(lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		stSDEC_MPG_RegH14A0[ch]->kmem_data.kmem_data = swab32(val);
	else if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		stSDEC_MPG_RegM14B0[ch]->kmem_data.kmem_data = swab32(val);
	else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		stSDEC_MPG_RegM14A0[ch]->kmem_data.kmem_data = swab32(val);
	else if(lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		stSDEC_MPG_RegH13A0[ch]->kmem_data.kmem_data = swab32(val);
#endif
//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_KMEMGet(UINT8 ch, UINT8 key_type, UINT8 pid_idx, UINT8 odd_key, UINT8 word_idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

	/* set pid filter index */
	MPG_RdFL(ch, kmem_addr);
	MPG_Wr01(ch, kmem_addr, key_type, key_type);
	MPG_Wr01(ch, kmem_addr, word_idx, word_idx);
	MPG_Wr01(ch, kmem_addr, pid_idx, pid_idx);
	MPG_Wr01(ch, kmem_addr, odd_key, odd_key);
	MPG_WrFL(ch, kmem_addr);

	/* read data */
	/* jinhwan.bae this function is not checked from register modification in H13 */
	MPG_RdFL(ch, kmem_data);
	MPG_Rd(ch, kmem_data, val);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);
exit:
	return val;
}

int SDEC_HAL_PIDFSetCRCBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, pes_crc_en[map_idx]);
	MPG_Rd(ch, pes_crc_en[map_idx], val);
	MPG_Wr01(ch, pes_crc_en[map_idx], crc16_en, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, pes_crc_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_PIDFClearCRCBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, pes_crc_en[map_idx]);
	MPG_Rd(ch, pes_crc_en[map_idx], val);
	MPG_Wr01(ch, pes_crc_en[map_idx], crc16_en, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, pes_crc_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_PIDFSetPidfData(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* write data */
	MPG_RdFL(ch, pidf_data);
	MPG_Wr01(ch, pidf_data, pidf_data, val);
	MPG_WrFL(ch, pidf_data);
#endif

	/* write data */
//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. write 01 to shadow
	MPG_Wr01(ch, pidf_data, pidf_data, val);
//	3. write data from shadow's pidf_data to shashadow
	MPG_WrFL_SShadow(ch, idx);
//	4. set pidf_addr
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);
//	5. write data to real reg
	MPG_WrFL(ch, pidf_data);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}


UINT32 SDEC_HAL_PIDFGetPidfData(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0 // read only is done by shashadow
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* read data */
	MPG_RdFL(ch, pidf_data);
	MPG_Rd01(ch, pidf_data, pidf_data, val);
#endif

//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. read and return the shadow value - it's same as original
	MPG_Rd01(ch, pidf_data, pidf_data, val);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);
exit:
	return val;
}

int SDEC_HAL_PIDFEnable(UINT8 ch, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* read & write data */
	MPG_RdFL(ch, pidf_data);
	MPG_Rd01(ch, pidf_data, pidf_data, val);

	if(en == SDEC_HAL_ENABLE)	val |=  DEC_EN;
	else						val &= ~DEC_EN;

	MPG_Wr01(ch, pidf_data, pidf_data, val);
	MPG_WrFL(ch, pidf_data);
#endif

	/* write data */
//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. read 01 field of shadow
	MPG_Rd01(ch, pidf_data, pidf_data, val);
//	3. modify the value
	if(en == SDEC_HAL_ENABLE)	val |=  DEC_EN;
	else						val &= ~DEC_EN;
//	4. write 01 to shadow
	MPG_Wr01(ch, pidf_data, pidf_data, val);
//	5. write data from shadow's pidf_data to shashadow
	MPG_WrFL_SShadow(ch, idx);
//	6. set pidf_addr
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);
//	7. write data to real reg
	MPG_WrFL(ch, pidf_data);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_PIDFScrambleCheck(UINT8 ch, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* read & write data */
	MPG_RdFL(ch, pidf_data);
	MPG_Rd01(ch, pidf_data, pidf_data, val);

	if(en == SDEC_HAL_ENABLE)	val |=  TPI_IEN;
	else						val &= ~TPI_IEN;

	MPG_Wr01(ch, pidf_data, pidf_data, val);
	MPG_WrFL(ch, pidf_data);
#endif

	/* write data */
//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. read 01 field of shadow
	MPG_Rd01(ch, pidf_data, pidf_data, val);
//	3. modify the value
	if(en == SDEC_HAL_ENABLE)	val |=  TPI_IEN;
	else						val &= ~TPI_IEN;
//	4. write 01 to shadow
	MPG_Wr01(ch, pidf_data, pidf_data, val);
//	5. write data from shadow's pidf_data to shashadow
	MPG_WrFL_SShadow(ch, idx);
//	6. set pidf_addr
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);
//	7. write data to real reg
	MPG_WrFL(ch, pidf_data);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_PIDFDescEnable(UINT8 ch, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* read & write data */
	MPG_RdFL(ch, pidf_data);
	MPG_Rd01(ch, pidf_data, pidf_data, val);

	if(en == SDEC_HAL_DISABLE)	val |=  NO_DSC;
	else						val &= ~NO_DSC;

	MPG_Wr01(ch, pidf_data, pidf_data, val);
	MPG_WrFL(ch, pidf_data);
#endif

	/* write data */
//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. read 01 field of shadow
	MPG_Rd01(ch, pidf_data, pidf_data, val);
//	3. modify the value
	if(en == SDEC_HAL_DISABLE)	val |=  NO_DSC;
	else						val &= ~NO_DSC;
//	4. write 01 to shadow
	MPG_Wr01(ch, pidf_data, pidf_data, val);
//	5. write data from shadow's pidf_data to shashadow
	MPG_WrFL_SShadow(ch, idx);
//	6. set pidf_addr
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);
//	7. write data to real reg
	MPG_WrFL(ch, pidf_data);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_PIDFDownloadEnable(UINT8 ch, UINT8 idx, UINT8 en)
{
	int ret = RET_ERROR;
	UINT32	val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, pidf_addr);
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);

	/* read & write data */
	MPG_RdFL(ch, pidf_data);
	MPG_Rd01(ch, pidf_data, pidf_data, val);

	/* To delete PAYLOAD_PES for Section Download - for JP 2013.02.04 jinhwan.bae */
	// if(en == SDEC_HAL_ENABLE)	val |=  ( DL_EN | PAYLOAD_PES );
	if(en == SDEC_HAL_ENABLE)	val |=  DL_EN;
	else						val &= ~DL_EN;

	MPG_Wr01(ch, pidf_data, pidf_data, val);
	MPG_WrFL(ch, pidf_data);
#endif

	/* write data */
//	1. read data from shashadow to shadow's pidf_data
	MPG_RdFL_SShadow(ch, idx);
//	2. read 01 field of shadow
	MPG_Rd01(ch, pidf_data, pidf_data, val);
//	3. modify the value
	if(en == SDEC_HAL_ENABLE)	val |=  DL_EN;
	else						val &= ~DL_EN;
//	4. write 01 to shadow
	MPG_Wr01(ch, pidf_data, pidf_data, val);
//	5. write data from shadow's pidf_data to shashadow
	MPG_WrFL_SShadow(ch, idx);
//	6. set pidf_addr
	MPG_Wr01(ch, pidf_addr, pidf_idx, idx);
	MPG_WrFL(ch, pidf_addr);
//	7. write data to real reg
	MPG_WrFL(ch, pidf_data);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFSetSecfData(UINT8 ch, UINT8 secf_idx, UINT8 word_idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if(secf_idx > 63 || word_idx > 6)
		goto exit;
//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

#if 0
	/* set pid filter index */
	MPG_RdFL(ch, secf_addr);
	MPG_Wr01(ch, secf_addr, secf_idx, secf_idx);
	MPG_Wr01(ch, secf_addr, word_idx, word_idx);
	MPG_WrFL(ch, secf_addr);

	/* write data */
	MPG_RdFL(ch, secf_data);
	MPG_Wr01(ch, secf_data, secf_data, val);
	MPG_WrFL(ch, secf_data);
#else
	/* set pid filter index */
//	MPG_RdFL(ch, secf_addr);
	MPG_Wr01(ch, secf_addr, secf_idx, secf_idx);
	MPG_Wr01(ch, secf_addr, word_idx, word_idx);
	MPG_WrFL(ch, secf_addr);

	/* write data */
//	MPG_RdFL(ch, secf_data);
	MPG_Wr01(ch, secf_data, secf_data, val);
	MPG_WrFL(ch, secf_data);

	MPG_WrFL_Secf_SShadow(ch, secf_idx, word_idx);
#endif

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	ret = RET_OK;
exit:
	return ret;
}


UINT32 SDEC_HAL_SECFGetSecfData(UINT8 ch, UINT8 secf_idx, UINT8 word_idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

#if 0
//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

	/* set pid filter index */
	MPG_RdFL(ch, secf_addr);
	MPG_Wr01(ch, secf_addr, secf_idx, secf_idx);
	MPG_Wr01(ch, secf_addr, word_idx, word_idx);
	MPG_WrFL(ch, secf_addr);

	/* read data */
	MPG_RdFL(ch, secf_data);
	MPG_Rd01(ch, secf_data, secf_data, val);

//	SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);
#else
	/* read data from section filter shadow register */
	MPG_RdFL_Secf_SShadow(ch, secf_idx, word_idx);
	MPG_Rd01(ch, secf_data, secf_data, val);
#endif

exit:
	return val;
}

/* get whether section filter is linked to pid filter index
 * read secf_map ( idx * 2 , idx * 2 + 1 ) and get 'or' value. */
UINT32 SDEC_HAL_SECFGetLinkedMap(UINT8 ch, UINT8 idx)
{
	UINT32 val_h = 0, val_l = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_map[idx * 2 + 0]);
	MPG_Rd01(ch, secf_map[idx * 2 + 0], secf_map, val_l);
	MPG_RdFL(ch, secf_map[idx * 2 + 1]);
	MPG_Rd01(ch, secf_map[idx * 2 + 1], secf_map, val_h);

exit:
	return ( val_l | val_h );
}


UINT32 SDEC_HAL_SECFGetMap(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_map[idx]);
	MPG_Rd01(ch, secf_map[idx], secf_map, val);

exit:
	return val;
}


int SDEC_HAL_SECFSetMap(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_map[idx]);
	MPG_Wr01(ch, secf_map[idx], secf_map, val);
	MPG_WrFL(ch, secf_map[idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_SECFSetMapBit(UINT8 ch, UINT8 idx, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	/* n/2 th pid filter is mapped to mth bit section filter. */
	/* (n /2 + 1) th pid filter is mapped to (m + 32) th bit section filter.  because one register can handle only 32-bits */
	if( loc < 32 )	map_idx = idx * 2;
	else			map_idx = idx * 2 + 1;

	MPG_RdFL(ch, secf_map[map_idx]);
	MPG_Rd(ch, secf_map[map_idx], val);
	MPG_Wr01(ch, secf_map[map_idx], secf_map, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, secf_map[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFClearMapBit(UINT8 ch, UINT8 idx, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	/* n/2 th pid filter is mapped to mth bit section filter. */
	/* (n /2 + 1) th pid filter is mapped to (m + 32) th bit section filter.  because one register can handle only 32-bits */
	if( loc < 32 )	map_idx = idx * 2;
	else			map_idx = idx * 2 + 1;

	MPG_RdFL(ch, secf_map[map_idx]);
	MPG_Rd(ch, secf_map[map_idx], val);
	MPG_Wr01(ch, secf_map[map_idx], secf_map, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, secf_map[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_SECFGetEnable(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_en[idx]);
	MPG_Rd01(ch, secf_en[idx], secf_en, val);
exit:
	return val;
}

UINT8 SDEC_HAL_SECFGetEnableBit(UINT8 ch, UINT8 loc)
{
	UINT32 val = 0;
	UINT8 map_idx = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_en[map_idx]);
	MPG_Rd01(ch, secf_en[map_idx], secf_en, val);
exit:
	return check_one_bit(val, ( loc % 32 ));
}



int SDEC_HAL_SECFSetEnable(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_en[idx]);
	MPG_Wr01(ch, secf_en[idx], secf_en, val);
	MPG_WrFL(ch, secf_en[idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_SECFSetEnableBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_en[map_idx]);
	MPG_Rd(ch, secf_en[map_idx], val);
	MPG_Wr01(ch, secf_en[map_idx], secf_en, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, secf_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFClearEnableBit(UINT8 ch,  UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_en[map_idx]);
	MPG_Rd(ch, secf_en[map_idx], val);
	MPG_Wr01(ch, secf_en[map_idx], secf_en, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, secf_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_SECFGetBufValid(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secfb_valid[idx]);
	MPG_Rd01(ch, secfb_valid[idx], secfb_valid, val);
exit:
	return check_one_bit(val, idx);
}


int SDEC_HAL_SECFSetBufValid(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secfb_valid[idx]);
	MPG_Wr01(ch, secfb_valid[idx], secfb_valid, val);
	MPG_WrFL(ch, secfb_valid[idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_SECFSetBufValidBit(UINT8 ch,  UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secfb_valid[map_idx]);
	MPG_Rd(ch, secfb_valid[map_idx], val);
	MPG_Wr01(ch, secfb_valid[map_idx], secfb_valid, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, secfb_valid[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFClearBufValidBit(UINT8 ch,  UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secfb_valid[map_idx]);
	MPG_Rd(ch, secfb_valid[map_idx], val);
	MPG_Wr01(ch, secfb_valid[map_idx], secfb_valid, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, secfb_valid[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

UINT32 SDEC_HAL_SECFGetMapType(UINT8 ch, UINT8 idx)
{
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_mtype[idx]);
	MPG_Rd01(ch, secf_mtype[idx], secf_mtype, val);
exit:
	return val;
}

UINT8 SDEC_HAL_SECFGetMapTypeBit(UINT8 ch, UINT8 loc)
{
	UINT32 val = 0;
	UINT8 map_idx = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_mtype[map_idx]);
	MPG_Rd01(ch, secf_mtype[map_idx], secf_mtype, val);

exit:
	return check_one_bit(val, ( loc % 32 ));
}


int SDEC_HAL_SECFSetMapType(UINT8 ch, UINT8 idx, UINT32 val)
{
	int ret = RET_ERROR;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	MPG_RdFL(ch, secf_mtype[idx]);
	MPG_Wr01(ch, secf_mtype[idx], secf_mtype, val);
	MPG_WrFL(ch, secf_mtype[idx]);

	ret = RET_OK;
exit:
	return ret;
}


int SDEC_HAL_SECFSetMapTypeBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_mtype[map_idx]);
	MPG_Rd(ch, secf_mtype[map_idx], val);
	MPG_Wr01(ch, secf_mtype[map_idx], secf_mtype, val | (1 << (loc % 32)) );
	MPG_WrFL(ch, secf_mtype[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFClearMapTypeBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, secf_mtype[map_idx]);
	MPG_Rd(ch, secf_mtype[map_idx], val);
	MPG_Wr01(ch, secf_mtype[map_idx], secf_mtype, val & ~(1 << (loc % 32)) );
	MPG_WrFL(ch, secf_mtype[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFSetCRCBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, sec_crc_en[map_idx]);
	MPG_Rd01(ch, sec_crc_en[map_idx], crc_en, val);

	val |= 1 << (loc % 32);

	MPG_Wr01(ch, sec_crc_en[map_idx], crc_en, val );
	MPG_WrFL(ch, sec_crc_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

int SDEC_HAL_SECFClearCRCBit(UINT8 ch, UINT8 loc)
{
	int ret = RET_ERROR;
	UINT8 map_idx = 0;
	UINT32 val = 0;

	SDEC_CHECK_NORMAL_CHANNEL(ch, goto exit);

	if( loc < 32 )	map_idx = 0;
	else			map_idx = 1;

	MPG_RdFL(ch, sec_crc_en[map_idx]);

	val &= ~(1 << (loc % 32));

	MPG_Wr01(ch, sec_crc_en[map_idx], crc_en, val );
	MPG_WrFL(ch, sec_crc_en[map_idx]);

	ret = RET_OK;
exit:
	return ret;
}

