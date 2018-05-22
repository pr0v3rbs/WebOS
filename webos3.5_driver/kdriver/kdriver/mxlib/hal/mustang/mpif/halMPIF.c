//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>

// Common Definition
#include "MsCommon.h"

// Internal Definition
#include "regMPIF.h"
#include "halMPIF.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }

// MPIF
#define MPIF_RIU8								((unsigned char  volatile *)_hal_mpif.u32MpifBaseAddr)
#define MPIF_READ(addr)                     	READ_WORD((_hal_mpif.u32MpifBaseAddr + (addr << 2)))
#define MPIF_WRITE(addr, val)               	WRITE_WORD((_hal_mpif.u32MpifBaseAddr + (addr << 2)), (val))
#define MPIF_WRITE_MASK(addr, val, mask)    	WRITE_WORD_MASK(_hal_mpif.u32MpifBaseAddr + ((addr)<<2), (val), (mask))
#define MPIF_REG8(saddr, idx)					MPIF_RIU8[(saddr << 2) + (idx << 1) - ((idx) & 1)]


#define CHIPTOP_READ(addr)                     	READ_WORD((_hal_mpif.u32ChipBaseAddr + (addr << 2)))
#define CHIPTOP_WRITE(addr, val)               	WRITE_WORD((_hal_mpif.u32ChipBaseAddr + (addr << 2)), (val))
#define CHIPTOP_WRITE_MASK(addr, val, mask)    	WRITE_WORD_MASK(_hal_mpif.u32ChipBaseAddr + ((addr)<<2), (val), (mask))

#define CLKGEN0_READ(addr)                     	READ_WORD((_hal_mpif.u32Clkgen0BaseAddr + (addr << 2)))
#define CLKGEN0_WRITE(addr, val)               	WRITE_WORD((_hal_mpif.u32Clkgen0BaseAddr + (addr << 2)), (val))
#define CLKGEN0_WRITE_MASK(addr, val, mask)    	WRITE_WORD_MASK(_hal_mpif.u32Clkgen0BaseAddr + ((addr)<<2), (val), (mask))

#ifdef MPIF_SPI_SUPPORT
#define MPIF_SPI_SET_CMD_LEN(len)				(MPIF_WRITE_MASK(REG_MPIF_SPI_CONTROL, BITS(10:8, len), BMASK(10:8)))
#define MPIF_SPI_SET_DATA_LEN(len)				(MPIF_WRITE(REG_MPIF_SPI_DATA_LENGTH, len))
#define MPIF_SPI_SET_RW_MODE(mode)				(MPIF_WRITE_MASK(REG_MPIF_4WIRE_SPI_CONTROL, BITS(1:1, (mode & 0xFF)), BMASK(1:1)))
#define MPIF_SPI_4WSPI_FIRED()					(MPIF_WRITE_MASK(REG_MPIF_4WIRE_SPI_CONTROL, BITS(0:0, 1), BMASK(0:0)))
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  u32ChipBaseAddr;
    MS_U32  u32MpifBaseAddr;
    MS_U32  u32Clkgen0BaseAddr;
} _HAL_MPIF_BASE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 _u8MPIFDbgLevel = 0;

HAL_MPIF_CONFIG _hal_mpif_cfg = {0};

#ifdef MPIF_SPI_SUPPORT
HAL_MPIF_SPI_CONFIG _hal_mpif_spi_cfg =
{
	.u8SerialMode = 0,
	.u8LeadingCyc = 1,
	.u8TrailingCyc = 1,
	.u8CMDLen = 1,
	.bSrcMIU = 0,
	.u8MIUSel = 0,
};
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static _HAL_MPIF_BASE _hal_mpif =
{
    .u32ChipBaseAddr = BASEADDR_RIU + BK_CHIPTOP,
    .u32MpifBaseAddr = BASEADDR_RIU + BK_MPIF,
    .u32Clkgen0BaseAddr = BASEADDR_RIU + BK_CLKGEN0,
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
// Wait the interrupt event
// If u32timeout = 0, wait forever
static MS_BOOL _HAL_MPIF_BusyWait_InterruptEvent(MS_U16 u16event_bit, MS_U32 u32timeout)
{
	MS_BOOL bRes = FALSE;
    MS_U32 u32i = 0;
	MS_U16 u16temp;
	MS_BOOL bloop = TRUE;

    // Use a limit value to prevent infinite loop
    while(bloop)
    {
    	u16temp = MPIF_READ(REG_MPIF_INTERRYPT_EVENT_STATUS);
        if ((u16temp & u16event_bit) == u16event_bit)
        {
        	MPIF_WRITE(REG_MPIF_INTERRYPT_EVENT_STATUS, u16temp); //clear status
            return TRUE;
        }
		if(u32timeout > 0)
			bloop = (++u32i < u32timeout);
    }

    DEBUG_MPIF(MPIF_DBGLV_INFO, printf("%s Timeout\n", __FUNCTION__));

    return bRes;
}

// Wait the interrupt or error event
// return MPIF_EVENT_TX_DONE if transmission done, or MPIF_EVENT_TX_ERROR if transmission error
// otherwise, return MPIF_EVENT_NULL
// If u32timeout = 0, wait forever
static MS_U8 _HAL_MPIF_BusyWait_InterruptErrorEvent(MS_U16 u16event_bit, MS_U16 u16error_bit, MS_U32 u32timeout)
{
    MS_U8 u8Ret = MPIF_EVENT_NULL;
    MS_U32 u32i = 0;
    MS_U16 u16temp = 0;
    MS_BOOL bloop = TRUE;

    // Use a limit value to prevent infinite loop
    while(bloop)
    {
        u16temp = MPIF_READ(REG_MPIF_INTERRYPT_EVENT_STATUS);

        if((u16temp & MPIF_EVENT_BUSYTIMEOUT) == MPIF_EVENT_BUSYTIMEOUT)
        {
            u8Ret = MPIF_EVENT_BUSYTIMEOUT;
            break;
        }

        // Check if TX done or TX error
        if((u16event_bit > MPIF_EVENTBIT_1A) && ((u16temp & u16error_bit) == u16error_bit))
        {
            u8Ret = MPIF_EVENT_TX_ERROR;
            break;
        }

        if((u16temp & u16event_bit)== u16event_bit)
        {
            u8Ret = MPIF_EVENT_TX_DONE;
            break;
        }
        if(u32timeout > 0)
			bloop = (++u32i < u32timeout);
    }

    if(u8Ret != MPIF_EVENT_TX_DONE)
        DEBUG_MPIF(MPIF_DBGLV_INFO, printf("%s interrupt failed status=0x%x\n", __FUNCTION__, u16temp));

    MPIF_WRITE(REG_MPIF_INTERRYPT_EVENT_STATUS, u16temp); //clear status

    return u8Ret;
}

#ifdef MPIF_SPI_SUPPORT

static MS_BOOL _HAL_MPIF_SPI_SwitchSource(MS_U8 u8src)
{
	MPIF_WRITE_MASK(REG_MPIF_SPI_CONTROL, BITS(11:11, (u8src & 0xFF)), BMASK(11:11));
	_hal_mpif_spi_cfg.bSrcMIU = (u8src == MPIF_SPI_SRC_MIU);

	return TRUE;
}

static MS_BOOL _HAL_MPIF_SPI_Set_CMD(MS_U32 u32cmd)
{
	MPIF_WRITE(REG_MPIF_SPI_COMMAND_VALUE_L, u32cmd & 0xFFFF);
	MPIF_WRITE(REG_MPIF_SPI_COMMAND_VALUE_H, (u32cmd >> 16) & 0xFFFF);

	return TRUE;
}

static MS_BOOL _HAL_MPIF_SPI_Set_BaseMIUAddr(MS_PHYADDR u32BaseAddr)
{
	MPIF_WRITE(REG_MPIF_SPI_MIU_BASE_ADDRESS_L, (u32BaseAddr >> 3) & 0xFFFF);
	MPIF_WRITE(REG_MPIF_SPI_MIU_BASE_ADDRESS_H,
		((u32BaseAddr >> 3) >> MPIF_SPI_MIU_BASEADDR_H_SHIFT) & MPIF_SPI_MIU_BASEADDR_H_MASK);

	return TRUE;
}

#endif  //MPIF_SPI_SUPPORT

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_MPIF_Config(MS_U32 u32RegBaseAddr)
{
    DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(0x%08X)\n", __FUNCTION__, (int)u32RegBaseAddr));
    _hal_mpif.u32ChipBaseAddr = u32RegBaseAddr + BK_CHIPTOP;
    _hal_mpif.u32MpifBaseAddr = u32RegBaseAddr + BK_MPIF;
    _hal_mpif.u32Clkgen0BaseAddr = u32RegBaseAddr + BK_CLKGEN0;

    DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("u32ChipBaseAddr: 0x%x, u32MpifBaseAddr 0x%x, u32Clkgen0BaseAddr 0x%x\n",
                (unsigned int)_hal_mpif.u32ChipBaseAddr, (unsigned int)_hal_mpif.u32MpifBaseAddr, (unsigned int)_hal_mpif.u32Clkgen0BaseAddr));
}

void HAL_MPIF_Set_Clock(MS_U8 u8clk)
{
	CLKGEN0_WRITE_MASK(REG_CKG_MPIF, BITS(5:0, ((MS_U16)(u8clk & 0xFF))), BMASK(5:0));
}

void HAL_MPIF_CLK_Disable(MS_BOOL bDisabled)
{
	CLKGEN0_WRITE_MASK(REG_CKG_MPIF, BITS(0:0, (MS_U8)bDisabled), BMASK(0:0));
}

//-------------------------------------------------------------------------
//Please set clock, trun around cycle, commend data with,
//slave data width first to meet SPIF status
//--------------------------------------------------------------------------
void HAL_MPIF_Init(HAL_MPIF_CONFIG *pmpifconf)
{
    DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("%s(0x%x)\n", __FUNCTION__, (unsigned int)pmpifconf));

    CHIPTOP_WRITE_MASK(REG_ALLPAD_IN, BITS(15:15, 0), BMASK(15:15)); //Disable all pad in
	CHIPTOP_WRITE_MASK(REG_MPIF_MODE, BITS(11:9, 3), BMASK(11:9)); //set 4 bit mode of pad to support all modes

	//CHIPTOP_WRITE_MASK(0x05, BITS(15:11, 0x1D), BMASK(15:11)); //driving
	//CHIPTOP_WRITE_MASK(0x06, BITS(0:0, 1), BMASK(0:0)); //driving

	HAL_MPIF_Set_Clock(MPIF_CLOCK_SEL);

    HAL_MPIF_SET_1A2XTRCycle(pmpifconf->u8trc_1A2X);
	HAL_MPIF_SET_3X4ATRCycle(pmpifconf->u8trc_3X4A);
	HAL_MPIF_SET_WaitCycle(pmpifconf->u8wc);

	HAL_MPIF_SET_SlaveDataWidth(0, pmpifconf->u8Slave0Dw);
	HAL_MPIF_SET_SlaveDataWidth(1, pmpifconf->u8Slave1Dw);
	HAL_MPIF_SET_SlaveDataWidth(2, pmpifconf->u8Slave2Dw);
	HAL_MPIF_SET_SlaveDataWidth(3, pmpifconf->u8Slave3Dw);

	HAL_MPIF_CmdDataWidth(pmpifconf->u8CmdDw);

	MPIF_WRITE(REG_MPIF_INTERRUPT_EVENT_MASK, 0xFFFF); //enable all interrupt
	MPIF_WRITE(REG_MPIF_INTERRYPT_EVENT_STATUS, 0xFFFF); //clear interrupt status

	HAL_MPIF_SWReset();

	MPIF_WRITE_MASK(REG_MPIF_MIU_WPROTECT_CONTROL, BITS(3:2, 0), BMASK(3:2)); //disable dumy write
	MPIF_WRITE_MASK(REG_MPIF_MISC2, BITS(10:10, 1), BMASK(10:10)); //enable reg_mpif_miu_w4wlast_done
	//MPIF_WRITE_MASK(REG_MPIF_THROTTLE_CFG, BITS(1:0, MPIF_CLK_THROTTLE_RATE_CYL3), BMASK(1:0));
	//MPIF_WRITE_MASK(REG_MPIF_THROTTLE_CFG, BITS(5:4, MPIF_CLK_THROTTLE_SIZE_8BYTES), BMASK(5:4));

	DEBUG_MPIF(MPIF_DBGLV_DEBUG, printf("MPIF init done!\n"));
}

// Wait the channel is free via checking the channel busy status
MS_BOOL HAL_MPIF_BusyWait_ChannelFree(MS_U8 u8event_bit, MS_U32 u32timeout)
{
    MS_U32 u32i;

    // Use a limit value to prevent infinite loop
    for (u32i=0; u32i < u32timeout; u32i++)
    {
        if ((MPIF_READ(REG_MPIF_CHANNEL_BUSY_STATUS) & u8event_bit) == 0x00)
            return TRUE;
    }

    DEBUG_MPIF(MPIF_DBGLV_INFO, printf("%s Timeout\n", __FUNCTION__));

    return FALSE;
}


// Write data to the specific SPIF register via LC1A
MS_BOOL HAL_MPIF_LC1A_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8index, MS_U8 *pu8data)
{
	MS_BOOL bRes = TRUE;
	MS_U16 u16Conf = 0;

    // Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_1A, MAX_TIMEOUT))
        return FALSE;

	u16Conf = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(u8index & 0x07) << 4);

	if(bWite == TRUE)
		u16Conf |= ((MS_U16)(*pu8data & 0xFF) << 8);

	MPIF_WRITE(REG_MPIF_LC1A_SETTINGS, u16Conf);
	MPIF_WRITE(REG_MPIF_LC1A_SETTINGS, u16Conf | 0x01); //fired

    // Wait for LC1A write done
    if(!_HAL_MPIF_BusyWait_InterruptEvent(MPIF_EVENTBIT_1A, MAX_TIMEOUT))
    {
    	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("CH 1A RW Failed\n"));
		bRes = FALSE;
    }

    // Retrieve the returned data
    if(bRes && (bWite == FALSE))
        *pu8data = (MS_U8)((MPIF_READ(REG_MPIF_LC1A_SETTINGS) & 0xFF00) >> 8);

    return bRes;
}

// Write data to the specific SPIF register via LC2A
MS_BOOL HAL_MPIF_LC2A_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16 *pu16data)
{
	MS_U16 u16Conf = 0;

    // Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_2A, MAX_TIMEOUT))
        return FALSE;

    // Set the register address
    MPIF_WRITE(REG_MPIF_LC2A_REG_ADDRESS, u16addr);

    // Put the data into the sending buffer
    if(bWite == TRUE)
        MPIF_WRITE(REG_MPIF_LC2A_REG_DATA, *pu16data);

    // Fire the read command, write control register
    u16Conf = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(bchecksum & 0x01) << 4) | ((MS_U16)(u8rtx & 0x03) << 6) | 0x01; //fired
    MPIF_WRITE(REG_MPIF_LC2A_REG_CONTROL, u16Conf);

    // Wait for LC2A write done
    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_2A, MPIF_EVENTBIT_2A_ERR, MAX_TIMEOUT) != MPIF_EVENT_TX_DONE)
    {
    	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("CH 2A RW Failed\n"));
        return FALSE;
    }

    // Retrieve the returned data
    if(bWite == FALSE)
        *pu16data = MPIF_READ(REG_MPIF_LC2A_REG_DATA);

    return TRUE;
}

MS_BOOL HAL_MPIF_LC2B_RW(MS_BOOL bWite, MS_U8 u8slaveid, MS_BOOL bchecksum, MS_U8 u8rtx, MS_U16 u16addr, MS_U16 *pu16data)
{
	MS_U16 u16Conf = 0;

    // Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_2B, MAX_TIMEOUT))
        return FALSE;

    // Set the register address
    MPIF_WRITE(REG_MPIF_LC2B_REG_ADDRESS, u16addr);

    if(bWite == TRUE)
		MPIF_WRITE(REG_MPIF_LC2B_REG_DATA, *pu16data);

	// Fire the read command, write control register
    u16Conf = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(bchecksum & 0x01) << 4) | ((MS_U16)(u8rtx & 0x03) << 6) | 0x01; //fired
    MPIF_WRITE(REG_MPIF_LC2B_REG_CONTROL, u16Conf);

    // Wait for LC2B write done
    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_2B, MPIF_EVENTBIT_2B_ERR, MAX_TIMEOUT) != MPIF_EVENT_TX_DONE)
    {
    	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("CH 2B RW Failed\n"));
        return FALSE;
    }

    // Retrieve the returned data
	if(bWite == FALSE)
		*pu16data = MPIF_READ(REG_MPIF_LC2B_REG_DATA);

    return TRUE;
}

// Read data from the SPIF to the RIU via LC3A
// u8len is WORD data len
MS_BOOL HAL_MPIF_LC3A_RIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8len, MS_U8 *pu8data, HAL_MPIF_3XCTL* p3xctl)
{
	MS_U8 u8i;
	MS_U16 u16data = 0;
	MS_U8 *pu8tr = pu8data;

    // Set data length
    MPIF_WRITE(REG_MPIF_LC3A_PACKET_LENGTH, 1); //unit is 16 bytes

	// Put the data into the sending buffer
	if(bWite == TRUE)
	{
    	for (u8i=0; u8i<u8len; u8i++)
        	MPIF_REG8(REG_MPIF_LC3A_PACKET_DATA, u8i) = *pu8tr++;
		for (; u8i<16; u8i++)
			MPIF_REG8(REG_MPIF_LC3A_PACKET_DATA, u8i) = 0;
	}

	u16data = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(p3xctl->bchksum_en & 0x01) << 4) |
					((MS_U16)(p3xctl->bretrx_idx & 0x01) << 5) | ((MS_U16)(p3xctl->u8retrx & 0x03) << 6) |
					((MS_U16)(p3xctl->bfmode & 0x01) << 10) | ((MS_U16)(p3xctl->bNotWaitMIUDone & 0x01) << 11) |
					((MS_U16)(p3xctl->u8Wcnt & 0x0F) << 12);

	MPIF_WRITE(REG_MPIF_LC3A_PACKET_CONTROL, u16data);
	MPIF_WRITE(REG_MPIF_LC3A_PACKET_CONTROL, u16data | 0x01);	//fired

	// Wait for LC3A write done
    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_3A, MPIF_EVENTBIT_3A_ERR, MAX_TIMEOUT) != MPIF_EVENT_TX_DONE)
        return FALSE;

	if(bWite == TRUE)
        return TRUE;

	for (u8i=0; u8i<u8len; u8i++)
    	*pu8tr++ = MPIF_REG8(REG_MPIF_LC3A_PACKET_DATA, u8i);

    return TRUE;
}

// Read data from the SPIF to the RIU via LC3A
// u8len is WORD data len
MS_BOOL HAL_MPIF_LC3B_RIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U8 u8len, MS_U8 *pu8data, HAL_MPIF_3XCTL* p3xctl)
{
	MS_U8 u8i;
	MS_U16 u16data = 0;
	MS_U8 *pu8tr = pu8data;

    // Set data length
    MPIF_WRITE(REG_MPIF_LC3B_PACKET_LENGTH, 1); //unit is 16 bytes

    // Put the data into the sending buffer
	if(bWite == TRUE)
	{
    	for (u8i=0; u8i<u8len; u8i++)
        	MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, u8i) = *pu8tr++;
		for (; u8i<16; u8i++)
			MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, u8i) = 0;
	}

	u16data = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(p3xctl->bchksum_en & 0x01) << 4) |
					((MS_U16)(p3xctl->bretrx_idx & 0x01) << 5) | ((MS_U16)(p3xctl->u8retrx & 0x03) << 6) |
					((MS_U16)(p3xctl->bfmode & 0x01) << 10) | ((MS_U16)(p3xctl->bNotWaitMIUDone & 0x01) << 11) |
					((MS_U16)(p3xctl->u8Wcnt & 0x0F) << 12);

	MPIF_WRITE(REG_MPIF_LC3B_PACKET_CONTROL, u16data);
	MPIF_WRITE(REG_MPIF_LC3B_PACKET_CONTROL, u16data | 0x01);

	// Wait for LC3B write done
    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_3B, MPIF_EVENTBIT_3B_ERR, MAX_TIMEOUT) != MPIF_EVENT_TX_DONE)
        return FALSE;

	if(bWite == TRUE)
        return TRUE;

	for (u8i=0; u8i<u8len; u8i++)
    	*pu8tr++ = MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, u8i);

    return TRUE;
}

// Read data from the SPIF to the MIU via LC3A
// u32len is number of bytes
MS_BOOL HAL_MPIF_LC3A_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U32 u32len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_3XCTL* p3xctl)
{
    MS_U16 u16data;

    // Set the MIU address as the destination address
    u16data = (MS_U16)((u32miu_addr >> 3) & 0xFFFF);
    MPIF_WRITE(REG_MPIF_LC3A_PACKET_MIU_BASE_ADDRESS_L, u16data);
    u16data = (MS_U16)((u32miu_addr >> 19) & 0xFFFF);
    MPIF_WRITE(REG_MPIF_LC3A_PACKET_MIU_BASE_ADDRESS_H, u16data);

    // Set data length
    MPIF_WRITE(REG_MPIF_LC3A_PACKET_LENGTH, (MS_U16)(u32len >> 4));

    // Fire the write command
    u16data = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(p3xctl->bchksum_en & 0x01) << 4) |
    				((MS_U16)(p3xctl->bretrx_idx & 0x01) << 5) | ((MS_U16)(p3xctl->u8retrx & 0x03) << 6) | 0x100 |
    				((MS_U16)(p3xctl->u8MIUSel & 0x01) << 9) | ((MS_U16)(p3xctl->bfmode & 0x01) << 10) |
    				((MS_U16)(p3xctl->bNotWaitMIUDone & 0x01) << 11) | ((MS_U16)(p3xctl->u8Wcnt & 0x0F) << 12);
    MPIF_WRITE(REG_MPIF_LC3A_PACKET_CONTROL, u16data);
	MPIF_WRITE(REG_MPIF_LC3A_PACKET_CONTROL, u16data | 0x01);

    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_3A, MPIF_EVENTBIT_3A_ERR, MAX_TIMEOUT) == MPIF_EVENT_TX_DONE)
		return TRUE;

    return FALSE;
}// Read data from the SPIF to the MIU via LC3A

// Read data from the SPIF to the MIU via LC3A
// u32len is number of bytes
MS_BOOL HAL_MPIF_LC3B_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U32 u32len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_3XCTL* p3xctl)
{
    MS_U16 u16data;

    // Set the MIU address as the destination address
    u16data = (MS_U16)((u32miu_addr >> 3) & 0xFFFF);
    MPIF_WRITE(REG_MPIF_LC3B_PACKET_MIU_BASE_ADDRESS_L, u16data);
    u16data = (MS_U16)((u32miu_addr >> 19) & 0xFFFF);
    MPIF_WRITE(REG_MPIF_LC3B_PACKET_MIU_BASE_ADDRESS_H, u16data);

    // Set data length
    MPIF_WRITE(REG_MPIF_LC3B_PACKET_LENGTH, (MS_U16)(u32len >> 4));

    // Fire the write command
    u16data = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(p3xctl->bchksum_en) << 4) |
    				((MS_U16)(p3xctl->bretrx_idx & 0x01) << 5) | ((MS_U16)(p3xctl->u8retrx & 0x03) << 6) | 0x100 |
    				((MS_U16)(p3xctl->u8MIUSel & 0x01) << 9) | ((MS_U16)(p3xctl->bfmode & 0x01) << 10) |
    				((MS_U16)(p3xctl->bNotWaitMIUDone & 0x01) << 11) | ((MS_U16)(p3xctl->u8Wcnt & 0x0F) << 12);
    MPIF_WRITE(REG_MPIF_LC3B_PACKET_CONTROL, u16data);
	MPIF_WRITE(REG_MPIF_LC3B_PACKET_CONTROL, u16data | 0x01);

	if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_3B, MPIF_EVENTBIT_3B_ERR, MAX_TIMEOUT) == MPIF_EVENT_TX_DONE)
		return TRUE;

    return FALSE;
}// Read data from the SPIF to the MIU via LC3A

// Read data from the SPIF to the MIU via LC4A
MS_BOOL HAL_MPIF_LC4A_MIURW(MS_BOOL bWite, MS_U8 u8slaveid, MS_U16 u16len, MS_U32 u32miu_addr, MS_U32 u32spif_mdr, HAL_MPIF_4ACTL* p4actl)
{
    MS_U16 u16data;

    // Set the MIU address as the source address
    u16data = (u32miu_addr >> 3) & 0xFFFF;
    MPIF_WRITE(REG_MPIF_LC4A_STREAM_MIU_BASE_ADDRESS_L, u16data);
    u16data = (u32miu_addr >> 19) & 0xFFFF;
    MPIF_WRITE(REG_MPIF_LC4A_STREAM_MIU_BASE_ADDRESS_H, u16data);

    // Set data length
    MPIF_WRITE(REG_MPIF_LC4A_STREAM_LENGTH, u16len);

    // Fire the read command, write control register
    u16data = ((MS_U16)(bWite & 0x01) << 1) | ((MS_U16)(u8slaveid & 0x03) << 2) | ((MS_U16)(p4actl->u8retrx_limit & 0x03) << 6) |
    				((MS_U16)(p4actl->bNotWaitMIUDone & 0x01) << 8) | ((MS_U16)(p4actl->u8MIUSel & 0x01) << 9) |
    				((MS_U16)(p4actl->u8granular & 0x03) << 10) | ((MS_U16)(p4actl->u8Wcnt & 0x0F) << 12);

	MPIF_WRITE(REG_MPIF_LC4A_STREAM_CONTROL, u16data);
	MPIF_WRITE(REG_MPIF_LC4A_STREAM_CONTROL, u16data | 0x01);

    // Wait for LC4A read done
    if(_HAL_MPIF_BusyWait_InterruptErrorEvent(MPIF_EVENTBIT_4A, MPIF_EVENTBIT_4A_ERR, MAX_TIMEOUT) == MPIF_EVENT_TX_DONE)
        return TRUE;

    return FALSE;
}

void HAL_MPIF_SET_1A2XTRCycle(MS_U8 u8trc)
{
	MS_U16 u16data;

	u16data = ((MS_U16)u8trc) & 0x03;
	MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(3:2, u16data) , BMASK(3:2));

	u16data = (((MS_U16)u8trc) & 0x04) >> 2;
	MPIF_WRITE_MASK(REG_MPIF_MISC2, BITS(3:3, u16data), BMASK(3:3));

	_hal_mpif_cfg.u8trc_1A2X = u8trc;
}

void HAL_MPIF_SET_3X4ATRCycle(MS_U8 u8trc)
{
	MS_U16 u16data;

	u16data = (((MS_U16)u8trc) & 0x07);
	MPIF_WRITE_MASK(REG_MPIF_MISC2, BITS(2:0, u16data), BMASK(2:0));

	_hal_mpif_cfg.u8trc_3X4A = u8trc;
}

void HAL_MPIF_SET_WaitCycle(MS_U8 u8wc)
{
	MS_U16 u16data = (MS_U16)u8wc;

	MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(5:4, u16data), BMASK(5:4));
	_hal_mpif_cfg.u8wc = u8wc;
}

void HAL_MPIF_SET_SlaveDataWidth(MS_U8 u8slaveid, MS_U8 u8dw)
{
	MS_U16 u16data = (MS_U16)u8dw;

	switch(u8slaveid)
	{
		case MPIF_SLAVE_ID_0:
			MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(7:6, u16data), BMASK(7:6));
			_hal_mpif_cfg.u8Slave0Dw = u8dw;
			break;
		case MPIF_SLAVE_ID_1:
			MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(9:8, u16data), BMASK(9:8));
			_hal_mpif_cfg.u8Slave1Dw = u8dw;
			break;
		case MPIF_SLAVE_ID_2:
			MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(11:10, u16data), BMASK(11:10));
			_hal_mpif_cfg.u8Slave2Dw = u8dw;
			break;
		case MPIF_SLAVE_ID_3:
			MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(13:12, u16data), BMASK(13:12));
			_hal_mpif_cfg.u8Slave3Dw = u8dw;
			break;
	}
}

void HAL_MPIF_CmdDataWidth(MS_U8 u8dw)
{
	CHIPTOP_WRITE_MASK(REG_MPIF_MODE, BITS(10:11, ((MS_U16)(u8dw+1))), BMASK(10:11)); //set 1 bit mode as default
	MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(15:14, ((MS_U16)(u8dw&0xFF))), BMASK(15:14));
	_hal_mpif_cfg.u8CmdDw = u8dw;
}

void HAL_MPIF_SWReset(void)
{
	MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(0:0, 0), BMASK(0:0));
	MPIF_WRITE_MASK(REG_MPIF_MISC1, BITS(0:0, 1), BMASK(0:0));
}

MS_BOOL HAL_MPIF_SetCmdDataMode(MS_U8 u8slaveid, MS_U8 u8cmdwidth, MS_U8 u8datawidth)
{
    MS_U16 u16data;

    u16data = MPIF_READ(REG_MPIF_MISC1);
    switch(u8slaveid & 0x03)
    {
        case MPIF_SLAVE_ID_0:
            u16data &= ~0xC0;
            u16data |= ((MS_U16)u8datawidth << 6);
            break;
        case MPIF_SLAVE_ID_1:
            u16data &= ~0x300;
            u16data |= ((MS_U16)u8datawidth << 8);
            break;
        case MPIF_SLAVE_ID_2:
            u16data &= ~0xC00;
            u16data |= ((MS_U16)u8datawidth << 10);
            break;
        case MPIF_SLAVE_ID_3:
            u16data &= ~0x3000;
            u16data |= ((MS_U16)u8datawidth << 12);
            break;
    }
    u16data &= ~0xC000;
    u16data |= ((MS_U16)u8cmdwidth << 14);
    MPIF_WRITE(REG_MPIF_MISC1, u16data);

    return TRUE;
}

MS_BOOL HAL_MPIF_SetSlave_ClkInv_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum)
{
	MS_U16 u16data;
	MS_U8 u8path;

	u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		return FALSE;

	u16data = ((MS_U16)(u8DlyBufNum & 0x0F) << 8) | 0x1000;
	if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC2, &u16data))
        return FALSE;

    return TRUE;
}

MS_BOOL HAL_MPIF_SetSlave_Clk_Delay(MS_U8 u8slaveid, MS_U8 u8DlyBufNum)
{
	MS_U16 u16data;
	MS_U8 u8path;

	u8path = MPIF_2X_DEST_SPIF;
	if(!HAL_MPIF_LC1A_RW(TRUE, u8slaveid, 1, &u8path))
		return FALSE;

	u16data = (MS_U16)(u8DlyBufNum & 0x0F);
	if(!HAL_MPIF_LC2A_RW(TRUE, u8slaveid, MPIF_CHECKSUM_SEL, MPIF_MAX_RTX_SEL, REG_SPIF_MISC2, &u16data))
        return FALSE;

    return TRUE;
}

#ifdef MPIF_SPI_SUPPORT

//Default slave id is 0, separate IO mode is 0, not wait miu done" is 0
MS_BOOL HAL_MPIF_SPI_Config(MS_U8 u8slaveid, HAL_MPIF_SPI_CONFIG *pspiconfig)
{
	MS_U16 u16data = 0;

	// Set the turn around cycle. Default is 1. If such cycle is unecessary, set it to 0
	HAL_MPIF_SET_1A2XTRCycle(0);
	HAL_MPIF_SET_3X4ATRCycle(0);

	u16data = (u8slaveid & 0x03) | ((pspiconfig->u8SerialMode & 0x03) << 2) |
				((pspiconfig->u8LeadingCyc & 0x03) << 4) | ((pspiconfig->u8TrailingCyc & 0x03) << 6) |
				((pspiconfig->u8CMDLen & 0x07) << 8) | ((pspiconfig->bSrcMIU & 0x01) << 11)	|
				((pspiconfig->u8MIUSel & 0x01) << 12);

	MPIF_WRITE(REG_MPIF_SPI_CONTROL, u16data);

	return TRUE;
}

MS_BOOL HAL_MPIF_SPI_RIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_U8 *pu8data, MS_U16 u16datalen)
{
	MS_U16 u16ii;
	MS_U8 *pu8tr = pu8data;

	if(u16datalen > MPIF_SPI_RIU_MAXLEN)
	{
		printf("ERR: [%s][%d] Packet Length must less than 16 bytes!!!", __FUNCTION__, __LINE__);
		return FALSE;
	}

	// Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_4WIRESPI, MAX_TIMEOUT))
        return FALSE;

	if(_hal_mpif_spi_cfg.bSrcMIU)
		_HAL_MPIF_SPI_SwitchSource(MPIF_SPI_SRC_RIU);

	if(bWrite)
		MPIF_SPI_SET_RW_MODE(MPIF_SPI_WRITE_MODE);
	else
		MPIF_SPI_SET_RW_MODE(MPIF_SPI_READ_MODE);

	MPIF_SPI_SET_CMD_LEN(u8cmdlen);
    if (u8cmdlen!=0)
		_HAL_MPIF_SPI_Set_CMD(*pu32cmd);

	MPIF_SPI_SET_DATA_LEN(u16datalen);

	if(bWrite && u16datalen)
	{
		for (u16ii=0; u16ii<u16datalen; u16ii++)
        	MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, (MS_U8)u16ii) = *pu8tr++;
		for (;u16ii<MPIF_SPI_RIU_MAXLEN; u16ii++)
        	MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, (MS_U8)u16ii) = 0;
	}
	else
	{
		//clear data
		for (u16ii=0; u16ii<MPIF_SPI_RIU_MAXLEN; u16ii++)
        	MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, (MS_U8)u16ii) = 0;
	}

	MPIF_SPI_4WSPI_FIRED();

	if(!_HAL_MPIF_BusyWait_InterruptEvent(MPIF_EVENTBIT_4WIRESPI, MAX_TIMEOUT))
    {
    	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("CH 4WIRESPI Failed\n"));
		return FALSE;
    }

	if(!bWrite)
	{
		for (u16ii=0; u16ii < u16datalen; u16ii++)
    		*pu8tr++ = MPIF_REG8(REG_MPIF_LC3B_PACKET_DATA, (MS_U8)u16ii);

#if 0
		printf("[%s]: ", __FUNCTION__);
		for(u16ii = 0; u16ii < u16datalen; u16ii++)
		{
			printf("%02x ", pu8data[u16ii]);
		}
#endif
	}

	return TRUE;
}

MS_BOOL HAL_MPIF_SPI_MIU_RWData(MS_BOOL bWrite, MS_U32 *pu32cmd, MS_U8 u8cmdlen, MS_PHYADDR u32MiuAddr, MS_U16 u16datalen)
{
	if((u32MiuAddr % 8))
	{
		printf("ERR: [%s][%d] MIU Address must be 8 bytes unit!!!", __FUNCTION__, __LINE__);
		return FALSE;
	}

	// Waiting for the channel being free
    if(!HAL_MPIF_BusyWait_ChannelFree(MPIF_EVENTBIT_4WIRESPI, MAX_TIMEOUT))
        return FALSE;

	if(_hal_mpif_spi_cfg.bSrcMIU == FALSE)
		_HAL_MPIF_SPI_SwitchSource(MPIF_SPI_SRC_MIU);

	MPIF_SPI_SET_CMD_LEN(u8cmdlen);
    if (u8cmdlen!=0)
		_HAL_MPIF_SPI_Set_CMD(*pu32cmd);
	_HAL_MPIF_SPI_Set_BaseMIUAddr(u32MiuAddr);
	MPIF_SPI_SET_DATA_LEN(u16datalen);

	if(bWrite)
		MPIF_SPI_SET_RW_MODE(MPIF_SPI_WRITE_MODE);
	else
		MPIF_SPI_SET_RW_MODE(MPIF_SPI_READ_MODE);

	MPIF_SPI_4WSPI_FIRED();

	if(!_HAL_MPIF_BusyWait_InterruptEvent(MPIF_EVENTBIT_4WIRESPI, MAX_TIMEOUT))
    {
    	DEBUG_MPIF(MPIF_DBGLV_ERR, printf("CH 4WIRESPI Failed\n"));
		return FALSE;
    }

	return TRUE;
}

#endif //MPIF_SPI_SUPPORT


