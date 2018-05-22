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
#ifndef _MHAL_LD_SPI_H_
#define _MHAL_LD_SPI_H_

#include "mhal_ld.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "xc_hwreg_utility2.h"

#define REG_1E98    0x101E98

#define REG_1900    0x153B00
#define REG_1A00    0x1A00
#define REG_1B00    0x1B00
#define REG_1C00    0x1C00

#define MSPI_WRITEBYTE(u32Reg, u8Val)                              	MDrv_WriteByte((REG_1900+0x100*MSPI_PORT_NUM+(u32Reg)), u8Val)
#define MSPI_WRITEBYTEMASK(u32Reg, u8Val, mask)            	MDrv_WriteByteMask((REG_1900+0x100*MSPI_PORT_NUM+(u32Reg)), u8Val,mask)
#define MSPI_WRITE2BYTESMASK(u32Reg, u8Val, mask)      	MDrv_Write2ByteMask((REG_1900+0x100*MSPI_PORT_NUM+(u32Reg)), u8Val, mask)
#define MSPI_WRITEBIT(u32Reg, u8Val, mask)                      	MDrv_WriteRegBit((REG_1900+0x100*MSPI_PORT_NUM+(u32Reg)), u8Val, mask)

#define REG_MSPI__WRITE_BUF_START   (0x80)
#define REG_MSPI__READ_BUF_START    (0x88)
#define REG_MSPI__WRITE_BUF_SIZE    (0x10)
#define REG_MSPI__READ_BUF_SIZE     (0x91)
#define REG_MSPI__CTRL              (0x92)
#define REG_MSPI__CLOCK_RATE        (0x93)
#define REG_MSPI__TIME_TR_START     (0x94) // The time from "reg_mspi_trigger" to first SPI clock
#define REG_MSPI__TIME_TR_END       (0x95) // The time from last SPI clock to "reg_mspi_done_flag"
#define REG_MSPI__TIME_B_TO_B       (0x96) // The time between byte to byte transfer
#define REG_MSPI__TIME_W_TO_R       (0x97) // The time between last write and first read,
#define REG_MSPI__WRITE_BUF_BIT_LEN (0x98) // Bit Length selection for write buffer0 1
#define REG_MSPI__READ_BUF_BIT_LEN  (0x9C) // Bit Length selection for write buffer0 1
#define REG_MSPI__TRIGGER_TRANSFER  (0xB4) // trigger starting data transfer
#define REG_MSPI__DONE_FLAG         (0xB6) // transfer status
#define REG_MSPI__CLEAR_DONE        (0xB8) // clear done status
#define REG_MSPI__CHIP_SELECT       (0xBE) // reg_chip_select 1~8


#define MASK_MSPI_CTRL__EN_MSPI         			BIT(0)
#define MASK_MSPI_CTRL__NOT_RESET       		BIT(1)
#define MASK_MSPI_CTRL__EN_MSPI_INT     		BIT(2)
#define MASK_MSPI_CTRL__EN_3_WIRE_MODE  	BIT(4)
#define MASK_MSPI_CTRL__CLOCK_PHASE     		BIT(6)
#define MASK_MSPI_CTRL__CLOCK_POLARITY  	BIT(7)

#define MSPI_SEND_BYTES                 2// 1~2 BYTES
#define SPI_WRITE_BUF_SIZE      480//242//64
#define ENABLE_SW_SPI           1
#define ENABLE_DEBUG_SW_SPI     (ENABLE_SW_SPI&&0)
#define DRV_LD_SPI_SET_WRITE_BUF_SIZE_16BIT 	0

typedef struct
{
    EnuMSPI_CLOCK_GEN eMSPIClockGen;
    EnuMSPI_CLOCK_DIV eMSPIClockDiv;    
    
    MS_U8 u8ClockPolarity; // 0 or 1
    MS_U8 u8ClockPhase;    // 0 or 1

    MS_U8 u8Time_FromTriggerToFirstClock;  // The time from "reg_mspi_trigger" to first SPI clock.     0 => Delay 1 MSPI clock
    MS_U8 u8Time_LastClockToDone;    // The time from last SPI clock to "reg_mspi_done_flag"     0 => Delay 1 MSPI clock
    MS_U8 u8Time_BetweenByteToByte;  // The time between byte to byte transfer       0 => Delay 0 MSPI clock
    MS_U8 u8Time_FromWriteToRead;    // The time between last write and first read   0 => Delay 0 MSPI clock

} StuMSPICtrlInfo;

typedef enum
{
    E_SPI_TRIGGER_NONE,
    E_SPI_TRIGGER_BY_ONE_TIME,
    E_SPI_TRIGGER_BY_V_SYNC,
    E_SPI_TRIGGER_BY_EVERY_V_SYNC,
} EnuSpiTriggerMode;

typedef enum
{
    E_SPI_STATE__NONE,
    E_SPI_STATE__W_WAIT_VSYNC_INT, // For write first byte
    E_SPI_STATE__W_WAIT_MSPI_INT, // For write other byte

} EnuSpiState;

#define _MDRV_MSPI_DEC_
#if(ENABLE_DEBUG_SW_SPI)
_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_VS_IntCnt;

_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_VS_Int_t1;
_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_VS_Int_t2;

_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_MSPI_IntCnt;
_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_MSPI_IntCnt_Err;

_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_MSPI_Int_t;
_MDRV_MSPI_DEC_ MS_U32 g_u32DrvSPI_Debug_Int_t[SPI_WRITE_BUF_SIZE];
#endif

void MHal_MSPI_SetClockGen( EnuMSPI_CLOCK_GEN eClockGen );
void MHal_MSPI_SetClockDiv( EnuMSPI_CLOCK_DIV eClockDiv );
void MHal_MSPI_SetClockPolarity( MS_U8 u8ClockPolarity );
void MHal_MSPI_SetClockPhase( MS_U8 u8ClockPhase );

#endif //_MHAL_LD_SPI_H_
