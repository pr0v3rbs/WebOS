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
 *  driver register implementation for sci device.
 *	sci device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.02.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

/*----------------------------------------------------------------------------------------
 File Inclusions
----------------------------------------------------------------------------------------*/
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/ioport.h>
#include <linux/slab.h>

#include "sci_reg.h"
#include "sci_drv.h"
#include "os_util.h"

#include "../sys/sys_regs.h"	//for CTOP CTRL Reg. map

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
struct SCI_MEM_T
{
	UINT32 base;
	UINT32 size;
};


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile SCI_REG_T* gpRegSCI;
volatile SCI_REG_T* gpRealRegSCI;

volatile struct SCI_MEM_T p_iomem;
volatile struct SCI_MEM_T v_iomem;

#ifdef INCLUDE_L9_CHIP_KDRV
static struct SCI_MEM_T memL9Ax =
{
	.base = 0xFE700000,
	.size = 0x1000,
};
#endif

static struct SCI_MEM_T memH13Ax =
{
	.base = 0xFE700000,
	.size = 0x1000,
};


/*----------------------------------------------------------------------------------------
	Global Function Definition
----------------------------------------------------------------------------------------*/
SCI_RETURN_T SCI_RegInit(void)
{
	SCI_RETURN_T ret = SCI_SUCCESS;
	int res = 0;
	UINT32 chip = lx_chip_rev();

	/* Init SCI Register access */

#if 1 // code for pass H15 path
    if(chip >= LX_CHIP_REV(H15, A0))
	{
		p_iomem.base = H15_SCI_BASE;
		p_iomem.size = sizeof(SCI_REG_T);
	}
    else if(chip >= LX_CHIP_REV(H14, A0))
#else
	if(chip >= LX_CHIP_REV(H14, A0))
#endif
	{
		p_iomem.base = H14_SCI_BASE;
		p_iomem.size = sizeof(SCI_REG_T);
	}
	else if(chip >= LX_CHIP_REV(M14, A0)) // LX_CHIP_REV(M14, B0) is same and defined one with M14_SCI_BASE
	{
		p_iomem.base = M14_SCI_BASE;
		p_iomem.size = sizeof(SCI_REG_T);
	}
    else if(chip >= LX_CHIP_REV_H13_A0)
	{
		p_iomem.base = memH13Ax.base;
		p_iomem.size = memH13Ax.size;
	}
#ifdef INCLUDE_L9_CHIP_KDRV
	else if(chip >= LX_CHIP_REV_L9_A0)
	{
		p_iomem.base = memL9Ax.base;
		p_iomem.size = memL9Ax.size;
    }
#if 0
    else // code for pass H15 path
    {
        ret = SCI_ERROR_CONFIG;
        return ret;
    }
#endif
#endif

	res = check_mem_region(p_iomem.base, p_iomem.size);
	if ( res != 0 )
	{
		SCI_ERROR("Error %d: Allocation for I/O memory is failed,\n", res);
		ret = SCI_NOMEM;
		return ret;
	}
	request_mem_region(p_iomem.base, p_iomem.size, SCI_MODULE);
	gpRealRegSCI = ioremap(p_iomem.base, p_iomem.size);
	if (gpRealRegSCI == NULL)
	{
		SCI_ERROR("error ioremap on %d : \n", __LINE__);
		ret = SCI_NOMEM;
		return ret;
	}
	v_iomem.base = (UINT32)gpRealRegSCI;
	v_iomem.size = p_iomem.size;

	/* Init Shadow SCI Register access */
	gpRegSCI = (SCI_REG_T*)kmalloc(sizeof(SCI_REG_T), GFP_KERNEL);
	if (gpRegSCI == NULL)
	{
		SCI_ERROR("error kmalloc on %d : \n", __LINE__);
		ret = SCI_NOMEM;
		release_mem_region(p_iomem.base, p_iomem.size);
		iounmap( (UINT32*)v_iomem.base );
		return ret;
	}
    memset(gpRegSCI, 0, sizeof(SCI_REG_T));

    // Initialize value of shadow reg to reset value
    SCI_RegSetInit();

	return ret;
}

/**
@fn void SCI_L9_WriteReg(UINT8 addr, UINT8 data)
@brief Write a data into a register of smartcard interface device
@param addr	: address for a data to be written
@param data : data to be written
@return void
*/
void SCI_RegCleanup(void)
{
	release_mem_region(p_iomem.base, p_iomem.size);
	p_iomem.base = 0;
	iounmap( (UINT32*)v_iomem.base );
	v_iomem.base = 0;
	kfree((void *)gpRegSCI);
	gpRegSCI = NULL;
}


void SCI_RegDump(void)
{
    UINT8 regSize = sizeof(SCI_REG_T);
    UINT8 i, j, loopCount;


	SCI_RdFL(sci_stat);
    SCI_RdFL(sci_err);
    SCI_RdFL(sci_intr_stat);

    SCI_PRINT("=============== SCI Register ===============\n");

    loopCount = regSize / 16;

    SCI_PRINT("ADDR\t       0        4        8        C\n");
    SCI_PRINT("======\t======== ======== ======== ========\n");
    for(i = 0; i < loopCount; i++)
    {
        SCI_PRINT("0x%04X\t", i * 16);

        for(j = 0; j < 4; j++)
                SCI_PRINT("%08X ", *((UINT32 *)gpRegSCI + i * 4 + j));
        SCI_PRINT("\n");
    }

    loopCount = (regSize % 16) / 4;
    SCI_PRINT("0x%04X\t", i * 16);
        for(j = 0; j < loopCount; j++)
            SCI_PRINT("%08X", *((UINT8 *)gpRegSCI + i * 4 + j));
    SCI_PRINT("\n============================================\n");

}

void SCI_RegWrite(UINT8 offset, UINT32 value)
{
//#define SCI_Rd(_r)			*((volatile UINT32*)(&(gpRegSCI->_r)))
//#define SCI_Wr(_r,_v)			((SCI_Rd(_r))=((volatile UINT32)(_v)))

    *(volatile UINT32 *)((UINT8 *)gpRegSCI + offset) = (volatile UINT32)value;
//#define SCI_WrFL(_r)			((gpRealRegSCI->_r)=(gpRegSCI->_r))
    *(volatile UINT32 *)((UINT8 *)gpRealRegSCI + offset) = *(volatile UINT32 *)((UINT8 *)gpRegSCI + offset);

}

void SCI_RegSetInit(void)
{
	SCI_Wr(sci_fc, 0x01);
	SCI_Wr(sci_ext, 0x03);
	SCI_Wr(sci_dds, 0x00);
	SCI_Wr(sci_set, 0x00);
	SCI_Wr(sci_conf, 0xFF);
	SCI_Wr(sci_mode, 0x00);
	SCI_Wr(sci_di, 0x01);
	SCI_Wr(sci_tos, 0x4D);
	SCI_Wr(sci_br, 0x25);
	SCI_Wr(sci_ft, 0x02);
	SCI_Wr(sci_intr_mask, 0x3F);
	SCI_Wr(sci_agt, 0x00);
	SCI_Wr(sci_wi, 0x06);
#if 0
	SCI_Wr(sci_dly, 0x03);
#else
    /* 20131210 jinhwan.bae to support New BCAS card type A, ACK after RST is about 6.75ms
       make HW IP wait time is 49600*2 ticks / 6.25M = 15.8ms
       before , 40000*2 ticks / 6.25M = 12.8 ms
       IP raise ATR-timeout after that, regardless of Driver Wait time */
	SCI_Wr(sci_dly, 0x1F);
#endif
	SCI_Wr(sci_pol_crc_l, 0x0A);
	SCI_Wr(sci_pol_crc_h, 0x0A);
	SCI_Wr(sci_rf_depth, 0x3F);
	SCI_Wr(sci_tf_depth, 0x1F);

	SCI_WrFL(sci_fc);
	SCI_WrFL(sci_ext);
	SCI_WrFL(sci_dds);
	SCI_WrFL(sci_set);
	SCI_WrFL(sci_conf);
	SCI_WrFL(sci_mode);
	SCI_WrFL(sci_di);
	SCI_WrFL(sci_tos);
	SCI_WrFL(sci_br);
	SCI_WrFL(sci_ft);
	SCI_WrFL(sci_intr_mask);
	SCI_WrFL(sci_agt);
	SCI_WrFL(sci_wi);
	SCI_WrFL(sci_dly);
	SCI_WrFL(sci_pol_crc_l);
	SCI_WrFL(sci_pol_crc_h);
	SCI_WrFL(sci_rf_depth);
	SCI_WrFL(sci_tf_depth);

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
    /* 2014. 06. 05. Hyunjong.yim Code for H15 Pull-up */
    else if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
    {
        CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
        CTOP_CTRL_H15A0_RdFL(VIP, ctr32);

        CTOP_CTRL_H15A0_Wr01(VIP, ctr36, sdio_en, 0);

        CTOP_CTRL_H15A0_Wr01(VIP, ctr32, gp15_5_mux_en, 1);
        CTOP_CTRL_H15A0_Wr01(VIP, ctr32, gp15_4_mux_en, 1);
        CTOP_CTRL_H15A0_Wr01(VIP, ctr32, gp15_3_mux_en, 1);
        CTOP_CTRL_H15A0_Wr01(VIP, ctr32, gp15_2_mux_en, 1);
        CTOP_CTRL_H15A0_Wr01(VIP, ctr32, gp16_6_mux_en, 1);

        CTOP_CTRL_H15A0_WrFL(VIP, ctr32);
        CTOP_CTRL_H15A0_WrFL(VIP, ctr36);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
    else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		/* TBD */
		/* 20130824, jinhwan.bae
			CTOP CTR49 have some pull up register, but format is different from previous chips.
		     	After one more chek to CTOP engineer and insert the routine */
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CTOP_CTRL_M14B0_RdFL(TOP, ctr16);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control, 0);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control_1, 0);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control_2, 0);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control_3, 0);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control_4, 0);
		CTOP_CTRL_M14B0_Wr01(TOP, ctr16, pull_up_control_5, 0);
		CTOP_CTRL_M14B0_WrFL(TOP, ctr16);
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CTOP_CTRL_M14A0_RdFL(ctr57);
		CTOP_CTRL_M14A0_Wr01(ctr57, pull_up_control_sd_clk, 0);
		CTOP_CTRL_M14A0_Wr01(ctr57, pull_up_control_sd_cmd, 0);
		CTOP_CTRL_M14A0_Wr01(ctr57, pull_up_control_sd_cd_n, 0);
		CTOP_CTRL_M14A0_Wr01(ctr57, pull_up_control_sd_wp_n, 0);
		CTOP_CTRL_M14A0_Wr01(ctr57, pull_up_control_sd_data, 0);
		CTOP_CTRL_M14A0_WrFL(ctr57);
	}
#endif
	else
	{
		SCI_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
	}
}

void SCI_RegSelVoltage(UINT8 val)
{
	SCI_Wr01(sci_set, vcc_sel, val);
	SCI_WrFL(sci_set);
}

UINT8 SCI_RegGetVoltage(void)
{
    UINT8 val;

    SCI_Rd01(sci_set, vcc_sel, val);

    return val;
}

UINT8 SCI_RegGetStatus(void)
{
	SCI_RdFL(sci_stat);
    return SCI_Rd(sci_stat);
}

UINT8 SCI_RegGetCardStatus(void)
{
    UINT8 state;
	SCI_RdFL(sci_stat);
	SCI_Rd01(sci_stat, card, state);
    return state;
}
UINT8 SCI_RegGetErrorStatus(void)
{
    SCI_RdFL(sci_err);
    return SCI_Rd(sci_err);;
}

void SCI_RegEnableInterrupt(UINT8 mask)
{
	UINT8 intrEn;
	intrEn = SCI_Rd(sci_intr_mask); //

    intrEn &= ~mask;
	SCI_Wr(sci_intr_mask, intrEn);
	SCI_WrFL(sci_intr_mask);
}

void SCI_RegDisableInterrupt(UINT8 mask)
{
	UINT8 intrEn;
	intrEn = SCI_Rd(sci_intr_mask);

    intrEn |= mask;
	SCI_Wr(sci_intr_mask, intrEn);
	SCI_WrFL(sci_intr_mask);
}

UINT8 SCI_RegGetInterruptMask(void)
{
	UINT8 intrMask;

    intrMask = SCI_Rd(sci_intr_mask);

    return intrMask;
}

UINT8 SCI_RegGetInterruptStatus(void)
{
	UINT8 intrState;

    SCI_RdFL(sci_intr_stat);
    intrState = SCI_Rd(sci_intr_stat);

    return intrState;
}

void SCI_RegWriteData(UINT8 data)
{
    SCI_Wr(sci_data, data);
    SCI_WrFL(sci_data);
}

UINT8 SCI_RegReadData(void)
{
    UINT8 recvData;
    SCI_RdFL(sci_data);
    SCI_Rd01(sci_data, data, recvData);

    return recvData;
}

void SCI_RegClearFIFO(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, clr_ft, 1);
	SCI_Wr01(sci_cmd, clr_fr, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegEnableTransmit(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, tx_en, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegEnableReceive(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, rx_en, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegPowerOn(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, pwr_on, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegPowerOff(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, pwr_off, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegReset(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_Wr01(sci_cmd, reset, 1);
	SCI_WrFL(sci_cmd);
}

void SCI_RegSetBaud(UINT8 brValue, UINT8 ftValue)
{
	SCI_Wr01(sci_br, br, brValue);
	SCI_WrFL(sci_br);


	SCI_Wr01(sci_ft, ft, ftValue);
	SCI_WrFL(sci_ft);
}

void SCI_RegSetReceiveFIFODepth(UINT8 depth)
{
	SCI_Wr01(sci_rf_depth, rf_depth, depth);
	SCI_WrFL(sci_rf_depth);
}

void SCI_RegSetTransmitFIFODepth(UINT8 depth)
{
	SCI_Wr01(sci_tf_depth, tf_depth, depth);
	SCI_WrFL(sci_tf_depth);
}

UINT8 SCI_RegGetNAD(void)
{
    UINT8 nad;
	SCI_RdFL(sci_nad);
	nad = SCI_Rd(sci_nad);

    return nad;
}

void SCI_RegSetNAD(UINT8 nad)
{
	SCI_Wr(sci_nad, nad);
	SCI_WrFL(sci_nad);
}

UINT8 SCI_RegGetPCB(void)
{
    UINT8 pcb;
	SCI_RdFL(sci_pcb);
	pcb = SCI_Rd(sci_pcb);

    return pcb;
}

void SCI_RegSetPCB(UINT8 pcb)
{
	SCI_Wr(sci_pcb, pcb);
	SCI_WrFL(sci_pcb);
}

UINT8 SCI_RegGetLength(void)
{
    UINT8 length;
	SCI_RdFL(sci_len);
	length = SCI_Rd(sci_len);

    return length;
}

void SCI_RegSetLength(UINT8 length)
{
	SCI_Wr(sci_len, length);
	SCI_WrFL(sci_len);
}

void SCI_RegSendNullCmd(void)
{
	SCI_Wr(sci_cmd, 0);
	SCI_WrFL(sci_cmd);
}

void SCI_RegSetFlowControl(UINT8 parityRetry, UINT8 rxFlowControlEnable, UINT8 txFlowControlEnable)
{
	SCI_Wr01(sci_fc, par_retry, parityRetry);
	SCI_Wr01(sci_fc, r_fc_en, rxFlowControlEnable);
	SCI_Wr01(sci_fc, t_fc_en, txFlowControlEnable);
	SCI_WrFL(sci_fc);
}

void SCI_RegSetTimeOut(UINT8 bwiValue, UINT8 cwiValue)
{
	SCI_Wr01(sci_tos, bwi, bwiValue);
	SCI_Wr01(sci_tos, cwi, cwiValue);
	SCI_WrFL(sci_tos);
}

void SCI_RegSetDI(UINT8 diValue)
{
	SCI_Wr01(sci_di, di, diValue);
	SCI_WrFL(sci_di);
}

void SCI_RegSetAddionalGuardTime(UINT8 guardTime)
{
	SCI_Wr01(sci_agt, agt, guardTime);
	SCI_WrFL(sci_agt);
}

void SCI_RegSetWorkWaitTime(UINT8 waitTime)
{
	SCI_Wr01(sci_wi, wi, waitTime);
	SCI_WrFL(sci_wi);
}

UINT8 SCI_RegGetClkDiv(void)
{
    UINT8 clkdiv;
    SCI_Rd01(sci_dly, clk_div, clkdiv);

    return clkdiv;
}

void SCI_RegSetDelay(UINT8 resetDuration, UINT8 clkDiv)
{
	SCI_Wr(sci_dly, 0);
	SCI_Wr01(sci_dly, rst_dur, resetDuration);
	SCI_Wr01(sci_dly, clk_div, clkDiv);
	SCI_WrFL(sci_dly);
}

void SCI_RegSetDelayPwrMonSignal(UINT8 delay)
{
    SCI_Wr01(sci_ext, p_mon_dly, delay);
    SCI_WrFL(sci_ext);
}

void SCI_RegSetExtSmartCardChipInterface(UINT8 flag)
{
	SCI_Wr01(sci_ext, if_mod, flag);
	SCI_WrFL(sci_ext);
}

void SCI_RegSetCardResetActiveLow(UINT8 flag)
{
	SCI_Wr01(sci_dds, reset_low, flag);
	SCI_WrFL(sci_dds);
}

void SCI_RegSetPwrMonHigh(UINT8 flag)
{
	SCI_Wr01(sci_dds, p_mon_hi, !flag);
	SCI_WrFL(sci_dds);
}

void SCI_RegEnableModeAutoRXChange(void)
{
	SCI_Wr01(sci_mode, tx_to_rx, ENABLE);
	SCI_WrFL(sci_mode);
}

void SCI_RegDisableModeAutoRXChange(void)
{
	SCI_Wr01(sci_mode, tx_to_rx, DISABLE);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeBigEndian(void)
{
	SCI_Wr01(sci_mode, lbe, LBE_BIG_BIT_ENDIAN);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeLittleEndian(void)
{
	SCI_Wr01(sci_mode, lbe, LBE_LITTLE_BIT_ENDIAN);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeConversion(UINT8 convValue)
{
	SCI_Wr01(sci_mode, conv, convValue);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetMaxRepetition(UINT8 value)
{
	SCI_Wr01(sci_mode, max_rpt, value);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeDataCheckLRC(void)
{
	SCI_Wr01(sci_mode, edc, EDC_LRC);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeDataCheckCRC(void)
{
	SCI_Wr01(sci_mode, edc, EDC_CRC);
	SCI_WrFL(sci_mode);
}

void SCI_RegSetModeByteORBlock(UINT8 flag)
{
	SCI_Wr01(sci_mode, t, flag);
	SCI_WrFL(sci_mode);
}
void SCI_RegSetCRCDirection(UINT8 direction)
{
	SCI_Wr01(sci_set, par_crc, direction);
	SCI_WrFL(sci_set);
}
void SCI_RegSetParityEven(void)
{
	SCI_Wr01(sci_set, poe, POE_EVEN);
	SCI_WrFL(sci_set);
}

void SCI_RegSetParityOdd(void)
{
	SCI_Wr01(sci_set, poe, POE_ODD);
	SCI_WrFL(sci_set);
}

void SCI_RegSetBGTDelay22etu(void)
{
	SCI_Wr01(sci_set, bgt_dly, BGT_DLY_22_ETU);
	SCI_WrFL(sci_set);

}
void SCI_RegSetBGTDelay16etu(void)
{
	SCI_Wr01(sci_set, bgt_dly, BGT_DLY_16_ETU);
	SCI_WrFL(sci_set);
}

void SCI_RegSetParityDuration(UINT8 flag)
{
	SCI_Wr01(sci_set, par_dur, flag);
	SCI_WrFL(sci_set);
}
