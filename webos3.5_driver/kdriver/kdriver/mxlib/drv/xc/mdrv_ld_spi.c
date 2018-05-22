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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvPWM.c
/// @brief  Pulse Width Modulation Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Include Files
#ifndef _MDRV_LD_SPI_C_
#define _MDRV_LD_SPI_C_

#include "mhal_ld.h"
#include "mdrv_ld_spi.h"
#include "mhal_ld_spi.h"

static MS_BOOL s_bMDrvSPI_InitFinished = FALSE;
StuMSPICtrlInfo s_MSPICtrlInfo;
static MS_BOOL s_bMspiOperationIsDone = 0;
static MS_U8 s_aucSpiWriteBuf_Real[SPI_WRITE_BUF_SIZE];
static MS_U8 s_aucSpiWriteBuf_User[SPI_WRITE_BUF_SIZE];
static MS_BOOL s_bSpiWriteBufUserChanged = FALSE;
static MS_U8 s_aucSpiWriteBuf_BitLen[SPI_WRITE_BUF_SIZE];
static MS_U8 s_aucSpiWriteBuf_Delay[SPI_WRITE_BUF_SIZE];
static MS_U16 s_u16SpiWriteBufSize = 0;
static MS_U16 s_u16SpiWriteBuf_Index = 0;

static EnuSpiTriggerMode s_eSpiTriggerMode = E_SPI_TRIGGER_NONE;
static EnuSpiState s_eSpiState = E_SPI_STATE__NONE;

#if 0
static void _MHal_MSPI_SetupTimingAndDelay(StuMSPICtrlInfo* pMSPICtrlInfo)
{
    MHal_MSPI_SetClockGen( pMSPICtrlInfo->eMSPIClockGen ) ;
    MHal_MSPI_SetClockDiv( pMSPICtrlInfo->eMSPIClockDiv  ) ;

    MHal_MSPI_SetClockPolarity( pMSPICtrlInfo->u8ClockPolarity );
    MHal_MSPI_SetClockPhase( pMSPICtrlInfo->u8ClockPhase );

    // The time from "reg_mspi_trigger" to first SPI clock
    MSPI_WRITEBYTE(REG_MSPI__TIME_TR_START, pMSPICtrlInfo->u8Time_FromTriggerToFirstClock);
    // The time from last SPI clock to "reg_mspi_done_flag"
    MSPI_WRITEBYTE(REG_MSPI__TIME_TR_END, pMSPICtrlInfo->u8Time_LastClockToDone);
    // The time between byte to byte transfer
    MSPI_WRITEBYTE(REG_MSPI__TIME_B_TO_B, pMSPICtrlInfo->u8Time_BetweenByteToByte);
    // The time between last write and first read,
    MSPI_WRITEBYTE(REG_MSPI__TIME_W_TO_R, pMSPICtrlInfo->u8Time_FromWriteToRead);
}

static void _MHal_MSPI_SetWriteBufBitLen(MS_U8 u8BitLen)
{
    MS_U16 u16Data;


    if( u8BitLen == 0 || u8BitLen > 8 )
        return;

    u8BitLen -= 1;

    u16Data = u8BitLen;
    u16Data |= (((MS_U16)u8BitLen)<<3);
    u16Data |= (((MS_U16)u8BitLen)<<6);
    u16Data |= (((MS_U16)u8BitLen)<<9);

    MSPI_WRITE2BYTESMASK(REG_MSPI__WRITE_BUF_BIT_LEN, u16Data, 0xFFF);
    MSPI_WRITE2BYTESMASK(REG_MSPI__WRITE_BUF_BIT_LEN+2, u16Data, 0xFFF);
}

static void _MHal_MSPI_SetReadBufBitLen(MS_U8 u8BitLen)
{
    MS_U16 u16Data;


    if( u8BitLen == 0 || u8BitLen > 8 )
        return;

    u8BitLen -= 1;

    u16Data = u8BitLen;
    u16Data |= (((MS_U16)u8BitLen)<<3);
    u16Data |= (((MS_U16)u8BitLen)<<6);
    u16Data |= (((MS_U16)u8BitLen)<<9);

    MSPI_WRITE2BYTESMASK(REG_MSPI__READ_BUF_BIT_LEN, u16Data, 0xFFF);
    MSPI_WRITE2BYTESMASK(REG_MSPI__READ_BUF_BIT_LEN+2, u16Data, 0xFFF);
}
#endif

MS_BOOL MDrv_SPI_Init(MS_BOOL bForce ,StuMSPICtrlInfo MSPICtrlInfo)
{
    MS_U16 i;

    if( s_bMDrvSPI_InitFinished )
    {
        if( bForce == FALSE )
        {
            // Already inited!
            return TRUE;
        }
    }

    // Setup clock parameter
    s_MSPICtrlInfo.eMSPIClockGen = MSPICtrlInfo.eMSPIClockGen; //MSPI_CLK_SPD_24M;
    s_MSPICtrlInfo.eMSPIClockDiv = MSPICtrlInfo.eMSPIClockDiv; //MSPI_CLK_SPD__DIV_8;
    s_MSPICtrlInfo.u8ClockPolarity = MSPICtrlInfo.u8ClockPolarity;//0 ;
    s_MSPICtrlInfo.u8ClockPhase =  MSPICtrlInfo.u8ClockPhase; //0 ;
    // Setup timing&delay parameter
    s_MSPICtrlInfo.u8Time_FromTriggerToFirstClock = MSPICtrlInfo.u8Time_FromTriggerToFirstClock; //0; // 1 cycle
    s_MSPICtrlInfo.u8Time_LastClockToDone = MSPICtrlInfo.u8Time_LastClockToDone; //0; // 1 cycle
    s_MSPICtrlInfo.u8Time_BetweenByteToByte = MSPICtrlInfo.u8Time_BetweenByteToByte; //0; // 0 cycle
    s_MSPICtrlInfo.u8Time_FromWriteToRead = MSPICtrlInfo.u8Time_FromWriteToRead; //0; // 0 cycle

#if 0
    _MHal_MSPI_SetupTimingAndDelay(&s_MSPICtrlInfo);

    _MHal_MSPI_SetWriteBufBitLen(8);
    _MHal_MSPI_SetReadBufBitLen(8);

    MSPI_WRITEBIT(REG_MSPI__CTRL, 1, MASK_MSPI_CTRL__EN_MSPI);
    MSPI_WRITEBIT(REG_MSPI__CTRL, 1, MASK_MSPI_CTRL__NOT_RESET);

    MSPI_WRITEBIT(REG_MSPI__CTRL, 1, MASK_MSPI_CTRL__EN_MSPI_INT);

    MSPI_WRITEBYTE(REG_MSPI__CLEAR_DONE, 1); // Clear done
#else
	MSPI_WRITEBYTE(0x10, 0x01); //reg_mspi_wbf_size
	MSPI_WRITEBYTE(0x11, 0x00); //reg_mspi_rbf_size
	MSPI_WRITEBYTE(0x12, 0x03); //reg_mspi_ctrl[7:0], now [1]:1 [0]:1
	MSPI_WRITEBYTE(0x13, 0x06); //reg_mspi_clock_rate ?

	MSPI_WRITEBYTE(0x14, 0x01); //reg_tr_start_time ?
	MSPI_WRITEBYTE(0x15, 0x00); //reg_tr_end_time ?

	MSPI_WRITEBYTE(0x16, 0x03); //reg_tbyte_interval_time ?
	MSPI_WRITEBYTE(0x17, 0x00); //reg_rw_turn_around_time ?

	MDrv_WriteByte(0x101EB4, 0x4A); // set gpio to spi1
#endif

    s_bMspiOperationIsDone = 0;

    s_eSpiTriggerMode = E_SPI_TRIGGER_NONE;

    // Setup write buffer ...
    for( i = 0; i < SPI_WRITE_BUF_SIZE; ++ i )
    {
        s_aucSpiWriteBuf_Real[i] = 0xFF;
        s_aucSpiWriteBuf_User[i] = 0xFF;
        s_bSpiWriteBufUserChanged = FALSE;
        s_aucSpiWriteBuf_BitLen[i] = 0x3F;
        s_aucSpiWriteBuf_Delay[i] = 0;
    #if (ENABLE_DEBUG_SW_SPI)
        g_u32DrvSPI_Debug_Int_t[i] = 0;
    #endif
    }
    s_u16SpiWriteBufSize = 0;
    s_u16SpiWriteBuf_Index = 0;

    s_eSpiState = E_SPI_STATE__NONE;


#if(ENABLE_DEBUG_SW_SPI )
    g_u32DrvSPI_Debug_VS_IntCnt = 0;
    g_u32DrvSPI_Debug_MSPI_IntCnt = 0;
    g_u32DrvSPI_Debug_MSPI_IntCnt_Err = 0;
#endif


    s_bMDrvSPI_InitFinished = TRUE;

#if(ENABLE_DEBUG_SW_SPI )
    MDrv_IRQ_Attach(E_IRQ_MSPI1_INT, (InterruptCb)FRC_R2_IRQ_ISR);

    MDrv_IRQ_UnMask(E_IRQ_MSPI1_INT);
#endif

printf("\n -- ox %s()\n",__FUNCTION__);

    return s_bMDrvSPI_InitFinished;
}

#endif //_MDRV_LD_SPI_C_
////////////////////////////////////////////////////////////////////////////////
