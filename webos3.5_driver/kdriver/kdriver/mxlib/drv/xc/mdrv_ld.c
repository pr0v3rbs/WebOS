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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    Mdrv_ld.c
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_LD_C_

#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "halCHIP.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#include "mhal_ld.h"
#include "mdrv_ldm_dma.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"

#if SUPPORT_LD_SPI
#include "mdrv_ld_spi.h"
#include "mhal_ld_spi.h"
#endif

#ifndef UNUSED
#define UNUSED(x)  ((x) = (x))
#endif

#ifndef ASSERT
#define ASSERT(arg) do {\
                        if (!(arg)) {\
                            printf("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #arg);\
                        }\
                    } while (0)
#endif

#define H_SCACLING_TABLE_SIZE   128
#define V_SCACLING_TABLE_SIZE   128
#define COMPENSATION_LENGTH     512

#define LD_BYTE_PER_WORD			16

#if  (LD_ENABLE == 1)
#include "mdrv_ld_compensation.h"
#include "mdrv_ld_edge2d.h"

#define LD_LED_NUMBER   (((MS_U16)pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH) * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)
#define LD_PANEL_WIDTH  pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u16PanelWidth // 1920
#define LD_PANEL_HEIGHT pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u16PanelHeight // 1080


#define EDGE2D_PACKET_LENGTH      16
#define H_V_RATIO_CONST           1048576ul

// When mspi_internal_clk=27M
// DIV_2   = 0.074us
// DIV_4   = 0.148us
// DIV_8   = 0.296us
// DIV_16  = 0.592us
// DIV_32  = 1.185us
// DIV_64  = 2.370us
// DIV_128 = 4.740us
// DIV_256 = 9.481us

#if 0
// LG spec:
// SPI clock:           1.66us ~ 10us   ==> MSPI_CLK_SPD__CPU_DIV_256
// V sync to first clk: 20us ~ 50u5     ==> 20 = Tr_Start=16, Tr_End=0
// Between byte to byte: 5 ~ 20         ==> 10 = Tr_Start=5, Tr_End=0
static ST_DRV_LD_INFO _stDrvLDInfo_Defalut =
{
    1920, 1080, // Panel size

    4, // u8LedNumH
    2, // u8LedNumV

    32, 32, // Backlight

    Tbl_LD_Compensation_LG37inch_LR10,
    Tbl_LD_Edge2D_LG37inch_LR10,

   // MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    10, // u8SpiTime_VSyncWidth
    20, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG37inch_LR10 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    5, // u8LedNumV

    16, 10, // Backlight

    Tbl_LD_Compensation_LG37inch_LR10,
    Tbl_LD_Edge2D_LG37inch_LR10,

   // MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    10, // u8SpiTime_VSyncWidth
    20, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG42inch_LR16 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_LG42inch_LR16,
    Tbl_LD_Edge2D_LG42inch_LR16,

    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG55inch_LR12 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    6, // u8LedNumV

    24, 18, // Backlight

    Tbl_LD_Compensation_LG55inch_LR12,
    Tbl_LD_Edge2D_LG55inch_LR12,

    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x85}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_CMO42inch_LR16 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_CMO42inch_LR16,
    Tbl_LD_Edge2D_CMO42inch_LR16,

    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG50inch_4K2K_VB1 =
{
    3840, 2160, // Panel size

    1, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_CMO42inch_LR16,
    Tbl_LD_Edge2D_CMO42inch_LR16,

    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};
#endif

//static MS_U8 s_au8DrvLD_SPIBuf[DRV_LD_SPI_BUF_MAX_SIZE];
//static MS_U8 s_au8DrvLD_SPI_delay[DRV_LD_SPI_BUF_MAX_SIZE];

MS_U8 au8Coeff_HScaling[128*6] =
{
    0x00,    0x00,    0x01,    0xFF,    0x00,    0x00,
    0x00,    0x00,    0x02,    0xFE,    0x00,    0x00,
    0x00,    0x00,    0x03,    0xFD,    0x00,    0x00,
    0x00,    0x00,    0x04,    0xFC,    0x00,    0x00,
    0x00,    0x00,    0x05,    0xFB,    0x00,    0x00,
    0x00,    0x00,    0x06,    0xFA,    0x00,    0x00,
    0x00,    0x00,    0x07,    0xF9,    0x00,    0x00,
    0x00,    0x00,    0x08,    0xF8,    0x00,    0x00,
    0x00,    0x00,    0x09,    0xF7,    0x00,    0x00,
    0x00,    0x00,    0x0A,    0xF6,    0x00,    0x00,
    0x00,    0x00,    0x0B,    0xF5,    0x00,    0x00,
    0x00,    0x00,    0x0C,    0xF4,    0x00,    0x00,
    0x00,    0x00,    0x0D,    0xF3,    0x00,    0x00,
    0x00,    0x00,    0x0E,    0xF2,    0x00,    0x00,
    0x00,    0x00,    0x0F,    0xF1,    0x00,    0x00,
    0x00,    0x00,    0x10,    0xF0,    0x00,    0x00,
    0x00,    0x00,    0x11,    0xEF,    0x00,    0x00,
    0x00,    0x00,    0x12,    0xEE,    0x00,    0x00,
    0x00,    0x00,    0x13,    0xED,    0x00,    0x00,
    0x00,    0x00,    0x14,    0xEC,    0x00,    0x00,
    0x00,    0x00,    0x15,    0xEB,    0x00,    0x00,
    0x00,    0x00,    0x16,    0xEA,    0x00,    0x00,
    0x00,    0x00,    0x17,    0xE9,    0x00,    0x00,
    0x00,    0x00,    0x18,    0xE8,    0x00,    0x00,
    0x00,    0x00,    0x19,    0xE7,    0x00,    0x00,
    0x00,    0x00,    0x1A,    0xE6,    0x00,    0x00,
    0x00,    0x00,    0x1B,    0xE5,    0x00,    0x00,
    0x00,    0x00,    0x1C,    0xE4,    0x00,    0x00,
    0x00,    0x00,    0x1D,    0xE3,    0x00,    0x00,
    0x00,    0x00,    0x1E,    0xE2,    0x00,    0x00,
    0x00,    0x00,    0x1F,    0xE1,    0x00,    0x00,
    0x00,    0x00,    0x20,    0xE0,    0x00,    0x00,
    0x00,    0x00,    0x21,    0xDF,    0x00,    0x00,
    0x00,    0x00,    0x22,    0xDE,    0x00,    0x00,
    0x00,    0x00,    0x23,    0xDD,    0x00,    0x00,
    0x00,    0x00,    0x24,    0xDC,    0x00,    0x00,
    0x00,    0x00,    0x25,    0xDB,    0x00,    0x00,
    0x00,    0x00,    0x26,    0xDA,    0x00,    0x00,
    0x00,    0x00,    0x27,    0xD9,    0x00,    0x00,
    0x00,    0x00,    0x28,    0xD8,    0x00,    0x00,
    0x00,    0x00,    0x29,    0xD7,    0x00,    0x00,
    0x00,    0x00,    0x2A,    0xD6,    0x00,    0x00,
    0x00,    0x00,    0x2B,    0xD5,    0x00,    0x00,
    0x00,    0x00,    0x2C,    0xD4,    0x00,    0x00,
    0x00,    0x00,    0x2D,    0xD3,    0x00,    0x00,
    0x00,    0x00,    0x2E,    0xD2,    0x00,    0x00,
    0x00,    0x00,    0x2F,    0xD1,    0x00,    0x00,
    0x00,    0x00,    0x30,    0xD0,    0x00,    0x00,
    0x00,    0x00,    0x31,    0xCF,    0x00,    0x00,
    0x00,    0x00,    0x32,    0xCE,    0x00,    0x00,
    0x00,    0x00,    0x33,    0xCD,    0x00,    0x00,
    0x00,    0x00,    0x34,    0xCC,    0x00,    0x00,
    0x00,    0x00,    0x35,    0xCB,    0x00,    0x00,
    0x00,    0x00,    0x36,    0xCA,    0x00,    0x00,
    0x00,    0x00,    0x37,    0xC9,    0x00,    0x00,
    0x00,    0x00,    0x38,    0xC8,    0x00,    0x00,
    0x00,    0x00,    0x39,    0xC7,    0x00,    0x00,
    0x00,    0x00,    0x3A,    0xC6,    0x00,    0x00,
    0x00,    0x00,    0x3B,    0xC5,    0x00,    0x00,
    0x00,    0x00,    0x3C,    0xC4,    0x00,    0x00,
    0x00,    0x00,    0x3D,    0xC3,    0x00,    0x00,
    0x00,    0x00,    0x3E,    0xC2,    0x00,    0x00,
    0x00,    0x00,    0x3F,    0xC1,    0x00,    0x00,
    0x00,    0x00,    0x40,    0xC0,    0x00,    0x00,
    0x00,    0x00,    0x41,    0xBF,    0x00,    0x00,
    0x00,    0x00,    0x42,    0xBE,    0x00,    0x00,
    0x00,    0x00,    0x43,    0xBD,    0x00,    0x00,
    0x00,    0x00,    0x44,    0xBC,    0x00,    0x00,
    0x00,    0x00,    0x45,    0xBB,    0x00,    0x00,
    0x00,    0x00,    0x46,    0xBA,    0x00,    0x00,
    0x00,    0x00,    0x47,    0xB9,    0x00,    0x00,
    0x00,    0x00,    0x48,    0xB8,    0x00,    0x00,
    0x00,    0x00,    0x49,    0xB7,    0x00,    0x00,
    0x00,    0x00,    0x4A,    0xB6,    0x00,    0x00,
    0x00,    0x00,    0x4B,    0xB5,    0x00,    0x00,
    0x00,    0x00,    0x4C,    0xB4,    0x00,    0x00,
    0x00,    0x00,    0x4D,    0xB3,    0x00,    0x00,
    0x00,    0x00,    0x4E,    0xB2,    0x00,    0x00,
    0x00,    0x00,    0x4F,    0xB1,    0x00,    0x00,
    0x00,    0x00,    0x50,    0xB0,    0x00,    0x00,
    0x00,    0x00,    0x51,    0xAF,    0x00,    0x00,
    0x00,    0x00,    0x52,    0xAE,    0x00,    0x00,
    0x00,    0x00,    0x53,    0xAD,    0x00,    0x00,
    0x00,    0x00,    0x54,    0xAC,    0x00,    0x00,
    0x00,    0x00,    0x55,    0xAB,    0x00,    0x00,
    0x00,    0x00,    0x56,    0xAA,    0x00,    0x00,
    0x00,    0x00,    0x57,    0xA9,    0x00,    0x00,
    0x00,    0x00,    0x58,    0xA8,    0x00,    0x00,
    0x00,    0x00,    0x59,    0xA7,    0x00,    0x00,
    0x00,    0x00,    0x5A,    0xA6,    0x00,    0x00,
    0x00,    0x00,    0x5B,    0xA5,    0x00,    0x00,
    0x00,    0x00,    0x5C,    0xA4,    0x00,    0x00,
    0x00,    0x00,    0x5D,    0xA3,    0x00,    0x00,
    0x00,    0x00,    0x5E,    0xA2,    0x00,    0x00,
    0x00,    0x00,    0x5F,    0xA1,    0x00,    0x00,
    0x00,    0x00,    0x60,    0xA0,    0x00,    0x00,
    0x00,    0x00,    0x61,    0x9F,    0x00,    0x00,
    0x00,    0x00,    0x62,    0x9E,    0x00,    0x00,
    0x00,    0x00,    0x63,    0x9D,    0x00,    0x00,
    0x00,    0x00,    0x64,    0x9C,    0x00,    0x00,
    0x00,    0x00,    0x65,    0x9B,    0x00,    0x00,
    0x00,    0x00,    0x66,    0x9A,    0x00,    0x00,
    0x00,    0x00,    0x67,    0x99,    0x00,    0x00,
    0x00,    0x00,    0x68,    0x98,    0x00,    0x00,
    0x00,    0x00,    0x69,    0x97,    0x00,    0x00,
    0x00,    0x00,    0x6A,    0x96,    0x00,    0x00,
    0x00,    0x00,    0x6B,    0x95,    0x00,    0x00,
    0x00,    0x00,    0x6C,    0x94,    0x00,    0x00,
    0x00,    0x00,    0x6D,    0x93,    0x00,    0x00,
    0x00,    0x00,    0x6E,    0x92,    0x00,    0x00,
    0x00,    0x00,    0x6F,    0x91,    0x00,    0x00,
    0x00,    0x00,    0x70,    0x90,    0x00,    0x00,
    0x00,    0x00,    0x71,    0x8F,    0x00,    0x00,
    0x00,    0x00,    0x72,    0x8E,    0x00,    0x00,
    0x00,    0x00,    0x73,    0x8D,    0x00,    0x00,
    0x00,    0x00,    0x74,    0x8C,    0x00,    0x00,
    0x00,    0x00,    0x75,    0x8B,    0x00,    0x00,
    0x00,    0x00,    0x76,    0x8A,    0x00,    0x00,
    0x00,    0x00,    0x77,    0x89,    0x00,    0x00,
    0x00,    0x00,    0x78,    0x88,    0x00,    0x00,
    0x00,    0x00,    0x79,    0x87,    0x00,    0x00,
    0x00,    0x00,    0x7A,    0x86,    0x00,    0x00,
    0x00,    0x00,    0x7B,    0x85,    0x00,    0x00,
    0x00,    0x00,    0x7C,    0x84,    0x00,    0x00,
    0x00,    0x00,    0x7D,    0x83,    0x00,    0x00,
    0x00,    0x00,    0x7E,    0x82,    0x00,    0x00,
    0x00,    0x00,    0x7F,    0x81,    0x00,    0x00,
    0x00,    0x00,    0x80,    0x80,    0x00,    0x00
};

MS_U32 MDrv_LD_AdjustBaseAddress(void* pInstance)
{
	MS_U32 u32Offset = 0;
	switch(MHal_LD_GetMiuSelect(pInstance))
	{
		case 1:
			u32Offset = HAL_MIU1_BASE;
			break;
			
		case 2:
			u32Offset = HAL_MIU2_BASE;
			break;
			
		case 0:
		default:
			u32Offset = HAL_MIU0_BASE;
			break;
	}

	return u32Offset;
}

MS_U8 MDrv_LD_MIUReadByte(void* pInstance, MS_U32 u32Addr, MS_U32 u32Offset)
{
	MS_U8 Ret = 0;

	u32Addr += MDrv_LD_AdjustBaseAddress(pInstance);
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
	Ret = (*(volatile MS_U8 *) (MsOS_PA2KSEG1(u32Addr + u32Offset) ));
#else
	Ret = (*(volatile MS_U8 *) (MsOS_PA2KSEG0(u32Addr + u32Offset) ));
#endif

	return Ret;
}

MS_U16 MDrv_LD_MIURead2Bytes(void* pInstance, MS_U32 u32Addr, MS_U32 u32Offset)
{
	MS_U16 Ret = 0;

	u32Addr += MDrv_LD_AdjustBaseAddress(pInstance);
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
	Ret = (*(volatile MS_U16 *) (MsOS_PA2KSEG1(u32Addr + u32Offset) ));
#else
	Ret = (*(volatile MS_U16 *) (MsOS_PA2KSEG0(u32Addr + u32Offset) ));
#endif

	return Ret;
}

void MDrv_LD_MIUWriteByte(void* pInstance, MS_U32 u32Addr, MS_U32 u32Offset, MS_U8 u8Value)
{
	u32Addr += MDrv_LD_AdjustBaseAddress(pInstance);
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
	(*((volatile MS_U8*)(MsOS_PA2KSEG1(u32Addr + u32Offset) ))) = (MS_U8)(u8Value);
#else
	(*((volatile MS_U8*)(MsOS_PA2KSEG0(u32Addr + u32Offset) ))) = (MS_U8)(u8Value);
#endif	
}

void MDrv_LD_MIUWrite2Bytes(void* pInstance, MS_U32 u32Addr, MS_U32 u32Offset, MS_U16 u16Value)
{
	u32Addr += MDrv_LD_AdjustBaseAddress(pInstance);
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
	(*((volatile MS_U16*)(MsOS_PA2KSEG1(u32Addr + u32Offset) ))) = (MS_U16)(u16Value);
#else
	(*((volatile MS_U16*)(MsOS_PA2KSEG0(u32Addr + u32Offset) ))) = (MS_U16)(u16Value);
#endif	

}

// ================== URSA New LD ====================
#if (LED_MULTI_SUPPORT_ENABLE)
extern t_sDrvLdCusPtrMap sCusPtrMap_LED_SEC85 ;
#else
extern t_sDrvLdCusPtrMap * psLEDCusPtrMap ;
#endif

MS_BOOL s_bMDrvLD_InitFinished = FALSE;
MS_BOOL s_bMDrvLD_SignalHalved;
static MS_U16 gu16LedDriverID =0;
#ifdef LD_SUPPORT_1XN_NX1
MS_BOOL s_bMDrvLD_Led_1xN = FALSE;
MS_BOOL s_bMDrvLD_Led_Nx1 = FALSE;
#endif
extern t_sDrvLdCusPtrMap sCusPtrMap;
MS_U32 s_u32LDBaseAddr = 0;
MS_BOOL gbLEDDemoModeOn = FALSE;

void MDrv_LD_SetLedDev(MS_U16 LedID)
{
    gu16LedDriverID = LedID;
}

MS_U16 MDrv_LD_GetLedDev(void)
{
    return gu16LedDriverID ;
}

MS_BOOL MDrv_LD_CUS_Init(MS_U16 LED_TYPE)
{
	sCusPtrMap = *psLEDCusPtrMap;

    LED_TYPE = 0;//fix lint error
    return TRUE;
}

void MDrv_LD_SPI_Init(void* pInstance)
{
#if SUPPORT_LD_SPI
    //MDrv_LDM_DMA_SetSpiTriggerMode(E_LDM_SPI_TRIGER_STOP); // Stop LDM-DMA

    {//MSPI setting

        StuMSPICtrlInfo MSPICtrlInfo;

        MSPICtrlInfo.eMSPIClockGen=psDrvLdInfo->eMSPIClockGen; //MSPI_CLK_SPD_24M;
        MSPICtrlInfo.eMSPIClockDiv=psDrvLdInfo->eMSPIClockDiv; //MSPI_CLK_SPD__DIV_8;
        MSPICtrlInfo.u8ClockPolarity=psDrvLdInfo->u8ClockPolarity;//0 ;
        MSPICtrlInfo.u8ClockPhase=psDrvLdInfo->u8ClockPhase; //0 ;
            // Setup timing&delay parameter
        MSPICtrlInfo.u8Time_FromTriggerToFirstClock=psDrvLdInfo->u8Time_FromTriggerToFirstClock; //0 // 1 cycle
        MSPICtrlInfo.u8Time_LastClockToDone=psDrvLdInfo->u8Time_LastClockToDone; //0; // 1 cycle
        MSPICtrlInfo.u8Time_BetweenByteToByte=psDrvLdInfo->u8Time_BetweenByteToByte; //0; // 0 cycle
        MSPICtrlInfo.u8Time_FromWriteToRead=psDrvLdInfo->u8Time_FromWriteToRead; //0; // 0 cycle

        if( MDrv_SPI_Init(TRUE,MSPICtrlInfo) == FALSE )
        {
            printf("Init drvMSPI failed!\n");
            return ;
        }
#if 0
        if (DRV_LD_SPI_SET_WRITE_BUF_SIZE_16BIT)
        {
                MSPI_WRITEBYTE(REG_MSPI__WRITE_BUF_SIZE, 2);
                MHal_LD_Set_SpiDepack16En(pInstance, TRUE);
        }
		else
		{
	        MSPI_WRITEBYTE(REG_MSPI__WRITE_BUF_SIZE, 1); // init for HW mode
	        #if ENABLE_TWO_SPI
	        MSPI_WRITEBYTE2(REG_MSPI__WRITE_BUF_SIZE, 1); // init for HW mode
	        #endif
		}
#endif
    }

//    MDrv_SPI_VSyncLikeInit();//PWM Vsync like
//    MDrv_SPI_SttInit();//SPI Stt
#endif
}

void MDrv_LD_LDF_Init(void* pInstance)
{
    MHal_LD_Set_LDF_BacklightWidth(pInstance, psDrvLdPQInfo->u8LDFWidth);
    MHal_LD_Set_LDF_BacklightHeight(pInstance, psDrvLdPQInfo->u8LDFHeight);

    MHal_LD_Set_InputHScalingDownRatio(pInstance, ((MS_U32)psDrvLdPQInfo->u8LDFWidth << 20)/psDrvLdInfo->u16PanelWidth + 1); // W_ledx1024x1024/W_lcd + 1
    MHal_LD_Set_InputVScalingDownRatio(pInstance, ((MS_U32)psDrvLdPQInfo->u8LDFHeight << 20)/psDrvLdInfo->u16PanelHeight + 1); // H_ledx1024x1024/H_lcd + 1

    //MS_U32 LDF_pack_length = (((psDrvLdPQInfo->u8LDFWidth/2 - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;
    MS_U32 LDF_pack_length = (((psDrvLdPQInfo->u8LDFWidth - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;
    //MS_U32 LDF_mem_size = LDF_pack_length * psDrvLdPQInfo->u8LDFHeight * 4;
    MS_U32 LDF_mem_size = LDF_pack_length * psDrvLdPQInfo->u8LDFHeight * 2;
    LDF_mem_size = (LDF_mem_size + 0xFF) >> 8 << 8; // align at 0x100

    MHal_LD_Set_LDF_FrameBufBaseAddr_L0(pInstance, (psDrvLdInfo->u32BaseAddr                 ));
    MHal_LD_Set_LDF_FrameBufBaseAddr_L1(pInstance, (psDrvLdInfo->u32BaseAddr +   LDF_mem_size));
    //MHal_LD_Set_LDF_FrameBufBaseAddr_R0((psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size));
    //MHal_LD_Set_LDF_FrameBufBaseAddr_R1((psDrvLdInfo->u32BaseAddr + 3*LDF_mem_size));

    MHal_LD_Set_LDF_HLPFEn(pInstance, TRUE);
    MHal_LD_Set_LDF_HLPFThreshold(pInstance, 0);
    MHal_LD_Set_LDF_HLPFStep(pInstance, 0);

    MHal_LD_Set_LDF_DCMaxAlpha(pInstance, psDrvLdPQInfo->u8DCMaxAlpha);
    MHal_LD_Set_LDF_WriteDCMaxEn(pInstance, 0);
    MHal_LD_Set_LDF_WriteDCMaxOFEn(pInstance, 1);

    MHal_LD_Set_LDF_PixelWhiteModeEn(pInstance, 1);
}

void MDrv_LD_LDB_Init(void* pInstance)
{
    //MS_U8 ldb_width = psDrvLdPQInfo->bEdge2DEn ? psDrvLdPQInfo->u8LEDWidth : psDrvLdPQInfo->u8LEDWidth/2 + 1;
    MS_U8 ldb_width = psDrvLdPQInfo->u8LEDWidth;
    if (psDrvLdPQInfo->bEdge2DEn && psDrvLdPQInfo->eLEDType == HAL_LD_EDGE_LR_TYPE)
    {
        MHal_LD_Set_LDB_BacklightWidth(pInstance, psDrvLdPQInfo->u8LEDHeight);
        MHal_LD_Set_LDB_BacklightHeight(pInstance, ldb_width);
    }
    else
    {
        MHal_LD_Set_LDB_BacklightWidth(pInstance, ldb_width);
        MHal_LD_Set_LDB_BacklightHeight(pInstance, psDrvLdPQInfo->u8LEDHeight);
    }

    MHal_LD_Set_LSF_BacklightWidth(pInstance, psDrvLdPQInfo->bEdge2DEn ? psDrvLdPQInfo->u8LSFWidth : ldb_width);
    MHal_LD_Set_LSF_BacklightHeight(pInstance, psDrvLdPQInfo->u8LSFHeight);

    MHal_LD_Set_DMA_BacklightWidth(pInstance, psDrvLdInfo->u8DMAWidth);
    MHal_LD_Set_DMA_BacklightHeight(pInstance, psDrvLdInfo->u8DMAHeight);

    MHal_LD_Set_FrameWidth(pInstance, psDrvLdInfo->u16PanelWidth);
    MHal_LD_Set_FrameHeight(pInstance, psDrvLdInfo->u16PanelHeight);

    MHal_LD_Set_OutputHScalingUpRatio(pInstance, (((MS_U32)psDrvLdPQInfo->u8LSFWidth/2)<<20) / ((psDrvLdInfo->u16PanelWidth/2) - 1)); // (W_led/2 + 1)x1024x1024/(W_lcd/2 - 1)
    MHal_LD_Set_OutputVScalingUpRatio(pInstance, ((MS_U32)psDrvLdPQInfo->u8LSFHeight<<20) / (psDrvLdInfo->u16PanelHeight - 1)); // H_ledx1024x1024/(H_lcd - 1)

    //MS_U32 LDF_pack_length = (((psDrvLdPQInfo->u8LDFWidth/2 - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;
    MS_U32 LDF_pack_length = (((psDrvLdPQInfo->u8LDFWidth - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;
    //MS_U32 LDF_mem_size = LDF_pack_length * psDrvLdPQInfo->u8LDFHeight * 4;
    MS_U32 LDF_mem_size = LDF_pack_length * psDrvLdPQInfo->u8LDFHeight * 2;
    MS_U32 LDB_pack_length = (((ldb_width - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;
    MS_U32 LDB_mem_size = LDB_pack_length * psDrvLdPQInfo->u8LEDHeight;
    MS_U32 EDGE2D_mem_size = (psDrvLdPQInfo->u8LSFWidth * psDrvLdPQInfo->u8LSFHeight) * MDrv_LD_PackLength;
    LDF_mem_size = (LDF_mem_size + 0xFF) >> 8 << 8; // align at 0x100
    LDB_mem_size = (LDB_mem_size + 0xFF) >> 8 << 8; // align at 0x100
    EDGE2D_mem_size = (EDGE2D_mem_size + 0xFF) >> 8 << 8; // align at 0x100

    //MHal_LD_Set_LDB_FrameBufBaseAddr_L0((psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size               ));
    //MHal_LD_Set_LDB_FrameBufBaseAddr_L1((psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size               ));
    //MHal_LD_Set_LDB_FrameBufBaseAddr_R0((psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size + LDB_mem_size));
    //MHal_LD_Set_LDB_FrameBufBaseAddr_R1((psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size + LDB_mem_size));
    MHal_LD_Set_LDB_FrameBufBaseAddr_L0(pInstance, (psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size               ));
    MHal_LD_Set_LDB_FrameBufBaseAddr_L1(pInstance, (psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size               ));
    //MHal_LD_Set_Edge2D_BufBaseAddr((psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size + 2*LDB_mem_size));
    MHal_LD_Set_Edge2D_BufBaseAddr(pInstance, (psDrvLdInfo->u32BaseAddr + 2*LDF_mem_size + LDB_mem_size));
    MHal_LD_Set_SPI_BufBaseAddr(pInstance, (psDrvLdInfo->u32BaseAddr + 4*LDF_mem_size + 2*LDB_mem_size + EDGE2D_mem_size));

    MHal_LD_Set_Edge2DEn(pInstance, psDrvLdPQInfo->bEdge2DEn);
    MHal_LD_Set_Edge2DGainEn(pInstance, FALSE);
    MHal_LD_Set_Edge2DDirectTypeEn(pInstance, psDrvLdPQInfo->eLEDType == HAL_LD_DIRECT_TYPE ? 1 : 0);

    MHal_LD_Set_LSF_H_InitialPhase_L(pInstance, psDrvLdPQInfo->u32LsfInitialPhase_HL); // need fine tune
    MHal_LD_Set_LSF_H_InitialPhase_R(pInstance, psDrvLdPQInfo->u32LsfInitialPhase_HR); // need fine tune
    MHal_LD_Set_LSF_V_InitialPhase(pInstance, psDrvLdPQInfo->u32LsfInitialPhase_V); // need fine tune
    MHal_LD_Set_LSF_H_Boundary_L(pInstance, 1); // _ENABLE -> duplicate
    //MHal_LD_Set_LSF_H_Boundary_R(0);
    MHal_LD_Set_LSF_V_Boundary(pInstance, 1);

    MHal_LD_Set_EdgeLevelShift(pInstance, 0); // 0:normal  1:x2  2:x4
    MHal_LD_Set_BlendingDitherEn(pInstance, FALSE);

    MHal_LD_Set_CompensationEn(pInstance, psDrvLdPQInfo->bCompensationEn);
    MHal_LD_Set_CompensationBlend(pInstance, psDrvLdPQInfo->u8CompensationBlend);
    MHal_LD_Set_CompensationDitherEn(pInstance, FALSE);

    MHal_LD_Set_BacklightGlobeMode(pInstance, psDrvLdInfo->bBacklightGlobeMode);
    MHal_LD_Set_BacklightGlobeGain(pInstance, psDrvLdInfo->u8BacklightGlobeGain);
    MHal_LD_Set_BacklightDataAlign(pInstance, psDrvLdInfo->u8BacklightGlobeAlign); // 0:16b  1:12b  2:4b  3:10b

#if ( DRV_LD_USE_SWSPI )
#else
#if ENABLE_TWO_SPI
    MHal_LD_Set_DmaMiuPack_Offset_Length(pInstance, 0, 4, 3);
#else
    MHal_LD_Set_DmaMiuPack_Offset_Length(pInstance, 0x0,0x1F);
#endif
    MHal_LD_Set_DmaEnable(pInstance, 1);
    MHal_LD_Set_DmaAccessYEnd(pInstance, psDrvLdInfo->u8DMAHeight); // ???  u8LedNumV or u8LD_BacklightV ???
#endif

}


void MDrv_LD_SW_Init(void* pInstance)
{
    MHal_LD_Set_LEDType(pInstance, (MS_U8)psDrvLdPQInfo->eLEDType);
    MHal_LD_Set_LED_BacklightWidth(pInstance, psDrvLdPQInfo->u8LEDWidth);
    MHal_LD_Set_LED_BacklightHeight(pInstance, psDrvLdPQInfo->u8LEDHeight);

    MHal_LD_Set_SW_LocalDimmingStrength(pInstance, psDrvLdPQInfo->u8LDStrength);
    MHal_LD_Set_SW_GlobalDimmingStrength(pInstance, psDrvLdPQInfo->u8GDStrength);
    MHal_LD_Set_SW_BacklightInGamma(pInstance, psDrvLdPQInfo->u8InGamma);
    MHal_LD_Set_SW_BacklightOutGamma(pInstance, psDrvLdPQInfo->u8OutGamma);
    MHal_LD_Set_SW_MinClampValue(pInstance, psDrvLdPQInfo->u8BLMinValue);

    MHal_LD_Set_SW_SpatialFilterStrength(pInstance, psDrvLdPQInfo->u8SFStrength);
    MHal_LD_Set_SW_SpatialFilterStrength2(pInstance, psDrvLdPQInfo->u8SFStrength2);
    MHal_LD_Set_SW_SpatialFilterStrength3(pInstance, psDrvLdPQInfo->u8SFStrength3);

    MHal_LD_Set_SW_TemporalFilterStrengthUp(pInstance, psDrvLdPQInfo->u8TFStrengthUp);
    MHal_LD_Set_SW_TemporalFilterStrengthDn(pInstance, psDrvLdPQInfo->u8TFStrengthDn);
    MHal_LD_Set_SW_TemporalFilterLowTh(pInstance, psDrvLdPQInfo->u8TFLowThreshold);
    MHal_LD_Set_SW_TemporalFilterHighTh(pInstance, psDrvLdPQInfo->u8TFHightThreshold);

    MHal_LD_Set_SW_PulseMode(pInstance, 0);
    MHal_LD_Set_SW_PulseId(pInstance, 0);
    MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0);
    MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0);

    MHal_LD_Set_DebugMode(pInstance, 0);
    MHal_LD_Set_LSFOutMode(pInstance, 0); // 0:normal  1:lsf  2:full
    MHal_LD_Set_CompensationOutMode(pInstance, 0); // 0:normal  1:overflow  2:overflow only  3:grid

    MHal_LD_Set_SW_ReadLDFEn(pInstance, 1);
    MHal_LD_Set_SW_AlgorithmEn(pInstance, 1);
    MHal_LD_Set_SW_WriteLDBEn(pInstance, 1);
    MHal_LD_Set_SW_WriteSPIEn(pInstance, 1);
	MHal_LD_Set_MaxSpeedLimit(pInstance, 0);
}


void MDrv_LD_Load_CompensationTable(void* pInstance, const MS_U8* pu8CompTable)
{
    MS_U16 u16Lut = 0;
    MS_U16 i = 0;

	if(pu8CompTable == NULL) return;

    MHal_LD_Set_CompensationMode(pInstance, FALSE); // TRUE -> incremental mode
    for (i = 0; i < 256; i++)
    {
        MHal_LD_Set_CompensationLUTAddress(pInstance, (MS_U8)i);
        u16Lut = *pu8CompTable++;
        u16Lut |= (*pu8CompTable++) << 8;
        MHal_LD_Write_CompensationLUT(pInstance, u16Lut);
        MHal_LD_Set_CompensationWriteEn(pInstance, 1);
    }
}


void MDrv_LD_Load_Edge2DTable(void* pInstance, const MS_U8* pu8Edge2DTable)
{
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 lsf_width = MHal_LD_Get_LSF_BacklightWidth(pInstance);
    MS_U16 lsf_height = MHal_LD_Get_LSF_BacklightHeight(pInstance);
    MS_U16 table_width = (led_width>>1) * (((led_height-1)>>1) + 1);
    MS_U16 table_height = lsf_width * lsf_height;
    MS_U32 addr_base = MHal_LD_Get_Edge2D_BufBaseAddr(pInstance);
    MS_U32 addr_offset = 0;
    MS_U16 i = 0, j = 0;

    MS_U16 pack_length = MDrv_LD_PackLength;

	if(pu8Edge2DTable == NULL) return;

    for (j = 0; j < table_height; j++)
    {
        addr_offset = j * pack_length;
        for (i = 0; i < table_width; i++)
        {
            MDrv_LD_MIUWriteByte(pInstance, addr_base, addr_offset++, *pu8Edge2DTable++);
        }
    }
}

// ================== URSA New LD ====================


MS_BOOL MApi_XC_LD_Init_U2(void* pInstance, EN_LD_PANEL_TYPE enLDPanelType )
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
//    MS_U32 u32R2BaseAddr = 0;

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

	MDrv_LD_Init(pInstance);

#if 0
    //DRAM buffer address Init
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf0Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf1Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf1Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf0Base = MDrv_LD_Get_SPIData_BaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf1Base = MDrv_LD_Get_SPIData_BaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32Edge2DCoeffBase = MDrv_LD_Get_EDGE2D_BaseAddr() - u32R2BaseAddr;
#endif

#if 0
    switch(enLDPanelType)
    {
        case E_LD_PANEL_LG37inch_LR10:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG37inch_LR10;
            break;
        case E_LD_PANEL_LG42inch_LR16:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG42inch_LR16;
            break;
        case E_LD_PANEL_LG55inch_LR12:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG55inch_LR12;
            break;
        case E_LD_PANEL_CMO42inch_LR16:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_CMO42inch_LR16;
            break;
        case E_LD_PANEL_LG50inch_4K2K_VB1:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG50inch_4K2K_VB1;
            break;
        case E_LD_PANEL_DEFAULT:
            //_stDrvLDInfo = _stDrvLDInfo__defalut;
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_Defalut;
            break;
        default:
        //printf("Unknown LD panel type=%u\n", enLDPanelType);
            ASSERT(enLDPanelType < E_LD_PANEL_NUMS);
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

            return FALSE;
    }

    pXCResourcePrivate->stdrv_LD._enLDPanelType = enLDPanelType;

    MDrv_LD_Init();
    MDrv_LD_CommonInit();
    MDrv_XC_LD_SetPanelType(enLDPanelType);

#endif
    MDrv_XC_LD_ISR(pInstance);

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE


    return TRUE;
}

MS_BOOL MApi_XC_LD_Init(EN_LD_PANEL_TYPE enLDPanelType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_LD_INIT XCArgs;
    XCArgs.enLDPanelType = enLDPanelType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_LD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_SetMiuSelect(void *pInstance, MS_U8 u8Select)
{
	MHal_LD_SetMiuSelect(pInstance, u8Select);
}

MS_BOOL MApi_XC_LD_SetMemoryAddress_U2(void* pInstance,
                                      MS_U8 u8MIU ,
                                      MS_U32 u32LDFBase0,
                                      MS_U32 u32LDFBase1,
                                      MS_U32 u32LDBBase0,
                                      MS_U32 u32LDBBase1,
                                      MS_U32 u32EDGE2DBase,
                                      MS_U32 u32LEDOffset)
{
    //MS_U32 u32R2BaseAddr = 0;
    //XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if 1
	//MS_U8 u8MiuSelect = 0;
	s_u32LDBaseAddr = u32LDFBase0;

	if(u32LDFBase0 > HAL_MIU2_BASE)
	{
		s_u32LDBaseAddr -= HAL_MIU2_BASE;
	//	u8MiuSelect = 2;
	}
	else if(u32LDFBase0 > HAL_MIU1_BASE)
	{
		s_u32LDBaseAddr -= HAL_MIU1_BASE;
	//	u8MiuSelect = 1;
	}		

	//MDrv_LD_SetMiuSelect(pInstance, u8MiuSelect);

#else
#if(HW_DESIGN_LD_VER == 1) // add
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x0C, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x00, 0x0C); // IP select
        u32LDFBase0 = u32LDFBase0 + HAL_MIU1_BASE;
        u32LDFBase1 = u32LDFBase1 + HAL_MIU1_BASE;
        u32LDBBase0 = u32LDBBase0 + HAL_MIU1_BASE;
        u32LDBBase1 = u32LDBBase1 + HAL_MIU1_BASE;
        u32EDGE2DBase = u32EDGE2DBase + HAL_MIU1_BASE;
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x0C, 0x0C); // IP select
    }
#elif(HW_DESIGN_LD_VER == 2)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF0, 0x08, 0x08); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF0, 0x00, 0x08); // IP select
        u32LDFBase0 = u32LDFBase0 + HAL_MIU1_BASE;
        u32LDFBase1 = u32LDFBase1 + HAL_MIU1_BASE;
        u32LDBBase0 = u32LDBBase0 + HAL_MIU1_BASE;
        u32LDBBase1 = u32LDBBase1 + HAL_MIU1_BASE;
        u32EDGE2DBase = u32EDGE2DBase + HAL_MIU1_BASE;
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF0, 0x00, 0x08); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF0, 0x08, 0x08); // IP select
    }
#elif(HW_DESIGN_LD_VER == 3)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x0C, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x00, 0x0C); // IP select
        u32LDFBase0 = u32LDFBase0 + HAL_MIU1_BASE;
        u32LDFBase1 = u32LDFBase1 + HAL_MIU1_BASE;
        u32LDBBase0 = u32LDBBase0 + HAL_MIU1_BASE;
        u32LDBBase1 = u32LDBBase1 + HAL_MIU1_BASE;
        u32EDGE2DBase = u32EDGE2DBase + HAL_MIU1_BASE;
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x0C, 0x0C); // IP select
    }

#elif(HW_DESIGN_LD_VER == 4)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF2, 0x40, 0x40); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF2, 0x00, 0x40); // IP select
        u32LDFBase0 = u32LDFBase0 + HAL_MIU1_BASE;
        u32LDFBase1 = u32LDFBase1 + HAL_MIU1_BASE;
        u32LDBBase0 = u32LDBBase0 + HAL_MIU1_BASE;
        u32LDBBase1 = u32LDBBase1 + HAL_MIU1_BASE;
        u32EDGE2DBase = u32EDGE2DBase + HAL_MIU1_BASE;
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF2, 0x00, 0x40); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF2, 0x40, 0x40); // IP select
    }

#elif(HW_DESIGN_LD_VER > 4)
    {
      printf("\r\n Local Dimming do not Support  this HW Version ");
      return FALSE;
    }
#endif

    MDrv_LD_Set_LDF_FrameBufBaseAddr(0, u32LDFBase0);
    MDrv_LD_Set_LDF_FrameBufBaseAddr(1, u32LDFBase1);
    MDrv_LD_Set_LDB_FrameBufBaseAddr(0, u32LDBBase0);
    MDrv_LD_Set_LDB_FrameBufBaseAddr(1, u32LDBBase1);
    MDrv_LD_Set_EDGE2D_BaseAddr(u32EDGE2DBase);
    MDrv_LD_Set_EDGE2D_BaseAddr(u32EDGE2DBase);
    MDrv_LD_Set_LEDData_BaseOffset(u32LEDOffset);

#endif

/*
    //DRAM buffer address Init
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf0Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf1Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf1Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf0Base = MDrv_LD_Get_SPIData_BaseAddr(0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf1Base = MDrv_LD_Get_SPIData_BaseAddr(1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32Edge2DCoeffBase = MDrv_LD_Get_EDGE2D_BaseAddr() - u32R2BaseAddr;
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

   */

    return TRUE;
}

MS_BOOL MApi_XC_LD_SetMemoryAddress(MS_U8 u8MIU ,
                                    MS_U32 u32LDFBase0,
                                    MS_U32 u32LDFBase1,
                                    MS_U32 u32LDBBase0,
                                    MS_U32 u32LDBBase1,
                                    MS_U32 u32EDGE2DBase,
                                    MS_U32 u32LEDOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_MEMORYADDRESS XCArgs;
    XCArgs.u8MIU = u8MIU;
    XCArgs.u32LDFBase0 = u32LDFBase0;
    XCArgs.u32LDFBase1 = u32LDFBase1;
    XCArgs.u32LDBBase0 = u32LDBBase0;
    XCArgs.u32LDBBase1 = u32LDBBase1;
    XCArgs.u32EDGE2DBase = u32EDGE2DBase;
    XCArgs.u32LEDOffset = u32LEDOffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_MEMORYADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_GetSpiAvgData(void* pInstance, MS_U8 *pu8LDValue)
{
	if(pu8LDValue != NULL)
	{
		*pu8LDValue = MHal_LD_Get_SPI_AvgData(pInstance);
	}
}

MS_BOOL MApi_XC_LD_Get_Value_U2(void* pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    MS_BOOL bReturn = TRUE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

	MDrv_LD_GetSpiAvgData(pInstance, pu8LDValue);

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    UNUSED(u8WSize);
    UNUSED(u8HSize);

    return bReturn;
}

MS_BOOL MApi_XC_LD_Get_Value(MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_LD_VALUE XCArgs;
    XCArgs.pu8LDValue = pu8LDValue;
    XCArgs.u8WSize = u8WSize;
    XCArgs.u8HSize = u8HSize;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LD_VALUE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_Set_SPICmd_U2(void* pInstance, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_BOOL bReturn = TRUE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

	MDrv_LDM_DMA_SetSpiCmd(pInstance, u8Mask, bEnable);

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_LD_Set_SPICmd(MS_U8 u8Mask, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_SPI_CMD XCArgs;
    XCArgs.u8LDMask = u8Mask;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_SPI_CMD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_CommonInit(void)
{
#if 0
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U16 u16PanelWidth = LD_PANEL_WIDTH;
    MS_U16 u16PanelHeight = LD_PANEL_HEIGHT;
    MS_U16 u16Value = 0, u16Mask = 0;

    //LDF Init
    //horizontal low-pass filter before MAXin enable
    SC_W2BYTEMSK(REG_SC_BK2E_1D_L, (ENABLE<<12), BIT(12));
#if (LD_PANEL== 1)
    //blending weight parameter DC*(8-reg_alpha)+MAX*reg_alpha
    SC_W2BYTEMSK(REG_SC_BK2E_1F_L, 0x02, 0x0F);
#endif

    //pixel white mode enable
    SC_W2BYTEMSK(REG_SC_BK2E_1F_L, (ENABLE<<7), BIT(7));
    //Write out RGB/DC/MAX data
    SC_W2BYTEMSK(REG_SC_BK2E_1F_L, (DISABLE<<8), BIT(8));
    //LSF HSU Init
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
    SC_W4BYTE(REG_SC_BK2E_38_L, 0x8000 & 0xFFFFF);
    SC_W4BYTE(REG_SC_BK2E_3A_L, 0x8000 & 0xFFFFF);
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT)&(~0)), (((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT)|0)&LD_LSFCONFIG_VALID));
#elif(HW_DESIGN_LD_VER == 1)
    SC_W2BYTEMSK(REG_SC_BK2E_3B_L, 0x80,0x00FF);
    SC_W2BYTEMSK(REG_SC_BK2E_3B_L, 0x80<<8,0xFF00);
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT|E_LD_LSF_SIGNBIT)&(~0)), ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT|E_LD_LSF_SIGNBIT)|0)&LD_LSFCONFIG_VALID);
    //vlpf enable hlpf enable
    SC_W2BYTEMSK(REG_SC_BK2E_3E_L, (MS_U16)(((DISABLE)<<14)|((DISABLE)<<15)), 0xC000);
    //hlpf 10-bit mode
    SC_W2BYTEMSK(REG_SC_BK2E_3D_L, (DISABLE<<14),  BIT(14));
    //LSFLPF HTapsSel
    SC_W2BYTEMSK(REG_SC_BK2E_46_L, (E_LD_LSFLPF_9TAPS<<7), 0x0180);
    //pel_offset_en
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, (DISABLE<<9), BIT(9));
#endif
    // lsf out mode
#if (LD_PANEL== 1)
    SC_W2BYTEMSK(REG_SC_BK2E_72_L, (E_LD_LSF_OUT_0x3FF<<1), 0x0E);
#endif

    SC_W2BYTEMSK(REG_SC_BK2E_70_L, (u16PanelWidth-1)&0x1FFF, 0x1FFF);
    SC_W2BYTEMSK(REG_SC_BK2E_71_L, (u16PanelHeight-1)&0x0FFF, 0x0FFF);

    //MDrv_LD_Load_Hscaling_Table(au8Coeff_HScaling);
    //MDrv_LD_Load_Vscaling_Table(au8Coeff_HScaling);

    //Compensation Init
    SC_W2BYTEMSK(REG_SC_BK2E_46_L, (DISABLE<<6),  BIT(6));
    //pixel blending between cmpensation and ori-data
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, (0x07<<4), 0x01F0);
    //comp_out_mode
    SC_W2BYTEMSK(REG_SC_BK2E_72_L, (E_LD_COMP_OUT_NORMAL<<4), 0x0030);
#if(HW_DESIGN_LD_VER == 1)
    //Offset Init
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, (DISABLE<<9), BIT(9));
    ////Compensation Init
    SC_W2BYTEMSK(REG_SC_BK2E_46_L, (DISABLE<<14), BIT(14));
    SC_W2BYTEMSK(REG_SC_BK2E_46_L, 0x00, 0x3E00);
    //Dither Init
    u16Value = (E_LD_DITHER_COMPENSATION_OUT&(~(E_LD_DITHER_MIN_SELECT|E_LD_DITHER_COMPENSATION|E_LD_DITHER_SATURATION)));
    u16Mask  = (E_LD_DITHER_COMPENSATION_OUT|(E_LD_DITHER_MIN_SELECT|E_LD_DITHER_COMPENSATION|E_LD_DITHER_SATURATION))&LD_DITHER3C_VALID;
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, u16Value, u16Mask);
    //combine value and mask.
    u16Value = (0&(~E_LD_DITHER_VLPF|E_LD_DITHER_HLPF|E_LD_DITHER_OFFSET));
    u16Mask  = (0|(E_LD_DITHER_VLPF|E_LD_DITHER_HLPF|E_LD_DITHER_OFFSET))&LD_DITHER3D_VALID;
    SC_W2BYTEMSK(REG_SC_BK2E_3D_L, u16Value, u16Mask);
    //min clamp value
    SC_W2BYTEMSK(REG_SC_BK2E_76_L, 0x9, 0x03FF);
#elif(HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
    //Dither Init
    u16Value = (E_LD_DITHER_COMPENSATION_OUT&(~(E_LD_DITHER_COMPENSATION)));
    u16Mask  = (E_LD_DITHER_COMPENSATION_OUT|(E_LD_DITHER_COMPENSATION))&LD_DITHER3C_VALID;
    SC_W2BYTEMSK(REG_SC_BK2E_3C_L, u16Value, u16Mask);
#endif
    //BL Globe Gain Init
    SC_W2BYTEMSK(REG_SC_BK2E_4B_L,(0x20)&0x00FF, 0x00FF);
    //BL GlobeGainMode
    SC_W2BYTEMSK(REG_SC_BK2E_4B_L,(E_LD_BL_GLOBAL_GAIN_8BIT<<8), 0x0700);
#endif
}

#if 0
void MDrv_LD_Print_Compensation_Table(void)
{
    MS_U16 u16Addr = 0;
    MS_U16 u16Index = 0;

    printf("\nHal_LD_Print_Compensation_Table\n");

    SC_W2BYTEMSK(REG_SC_BK2E_72_L, ENABLE, BIT(0));

    for( u16Addr = 0; u16Addr < 256; ++ u16Addr )
    {
        if(u16Addr % 0x10 == 0) printf("\n");
        SC_W2BYTEMSK(REG_SC_BK2E_3F_L, u16Addr,0x00FF);// write address for compensation LUT
        SC_W2BYTEMSK(REG_SC_BK2E_3F_L, ENABLE<<11, BIT(11)); // Compensation LUT read
        printf("%02X%02X ", ((SC_R2BYTE(REG_SC_BK2E_3E_L)&0xFF00)>>8), (SC_R2BYTE(REG_SC_BK2E_3E_L)&0x00FF));
    }

    SC_W2BYTEMSK(REG_SC_BK2E_72_L, 0, BIT(0));
}
#endif

void MDrv_LD_Load_Compensation_Table(MS_U8* pu8CompTable,MS_U16 u16CompTableLength)
{
#if 0
    MS_U16 u16Addr = 0;
    MS_U16 u16Index = 0;
    MS_U16 u16Tmp = 0, u16Tmp2 =0;
    if (pu8CompTable != NULL)
    {
        for( u16Addr = 0; u16Addr < u16CompTableLength/2; ++ u16Addr )
        {
            SC_W2BYTEMSK(REG_SC_BK2E_3F_L, u16Addr, 0x00FF);// write address for compensation LUT
            SC_W2BYTEMSK(REG_SC_BK2E_3D_L, ((pu8CompTable[u16Index+1]<<8) + pu8CompTable[u16Index]), 0x0FFF);

            u16Index += 2;
            SC_W2BYTEMSK(REG_SC_BK2E_3F_L, (ENABLE<<10), BIT(10)); // Compensation LUT write

            //MsOS_DelayTaskUs(1);
            for(u16Tmp = 0; u16Tmp < 255; ++u16Tmp)
            {
                u16Tmp2 = u16Tmp;
            }
        }
    }
#endif
}

/*
Edge 2D Table Format
Normal: Width = LED Number(HxV) / 4
        Height = Backlight size (HxV)

Special(for Edge-LR panel and odd V size)
        Width = (LED Height+1) x V / 4
        Height = Backlight size (HxV)

H-direction: zero padding to Packet Length (16byte)
*/
void MDrv_LD_Load_EDGE2D_Table(void* pInstance, MS_U8 u8LEDWidth, MS_U8 u8LEDHeight, MS_U8 u8BLWidth, MS_U8 u8BLHeight, MS_U8* pu8Edge2DTable)
{
    MS_U16 u16RowIdx = 0, u16ColumnIdx = 0;
    MS_U16 u16Tbl_Active_Width = ((u8LEDWidth*u8LEDHeight)+2)/4;
    MS_U16 u16Tbl_Active_Height = (MS_U16)u8BLWidth * u8BLHeight;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    for(u16RowIdx = 0; u16RowIdx < u16Tbl_Active_Height; ++u16RowIdx)
    {
        for(u16ColumnIdx = 0; u16ColumnIdx < EDGE2D_PACKET_LENGTH; ++u16ColumnIdx)
        {
            if(u16ColumnIdx < u16Tbl_Active_Width)
            {
                if (pu8Edge2DTable != NULL)
                {
                    MDrv_LD_MIUWriteByte(pInstance, LD_BUF_Edge2DCoe, (u16RowIdx*EDGE2D_PACKET_LENGTH + u16ColumnIdx), pu8Edge2DTable[u16RowIdx*u16Tbl_Active_Width + u16ColumnIdx]);
                }
            }
            else
            {
                MDrv_LD_MIUWriteByte(pInstance, LD_BUF_Edge2DCoe, (u16RowIdx*EDGE2D_PACKET_LENGTH + u16ColumnIdx), 0);
            }
        }
    }
}

MS_BOOL MDrv_XC_LD_SetPanelType(EN_LD_PANEL_TYPE enLDPanelType )
{
#if 0
#if 0
    MS_U16 i;
    MS_U16 u16TotalSPIDataLen = 0;
#endif
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U32 u32Ratio = 0;
    MS_U16 u16Tmp = 0;
    MS_U16 u16PanelWidth   = LD_PANEL_WIDTH;
    MS_U16 u16PanelHeight  = LD_PANEL_HEIGHT;
    UNUSED(enLDPanelType);
#if 0 //
    if( _enLDPanelType != enLDPanelType )
    {
    #if( DRV_LD_USE_SWSPI )
        MDrv_SPI_SetTriggerMode(E_SPI_TRIGGER_NONE);
    #else
    #endif
    }
#endif
#if 0
    Hal_MSPI_SetClockSpeed( _stDrvLDInfo.eMSPIClockSpeed );
    Hal_MSPI_SetClockPolarity( _stDrvLDInfo.u8SpiClockPolarity );
    Hal_MSPI_SetClockPhase( _stDrvLDInfo.u8SpiClockPhase );

#if( DRV_LD_USE_SWSPI )

    // Setup SPI cmd data...
    for( i = 0; i < _stDrvLDInfo.u8SpiCmdLen; ++ i )
    {
        s_au8DrvLD_SPIBuf[i] = _stDrvLDInfo.au16SpiCmd[i];
    }

    // Setup MSPI timing ...
    u16TotalSPIDataLen = _stDrvLDInfo.u8SpiCmdLen + LD_LED_NUMBER;
    if( _stDrvLDInfo.u8CheckSumMode )
    {
        u16TotalSPIDataLen += 1;
    }

    // Setup the delay time before every data
    //s_au8DrvLD_SPI_delay[0] = (_stDrvLDInfo.u8SpiTime_VSyncWidth + _stDrvLDInfo.u8SpiTime_VSync_To_FirstClock);
    s_au8DrvLD_SPI_delay[0] = _stDrvLDInfo.u8SpiTime_VSync_To_FirstClock;

    for( i = 1; i < u16TotalSPIDataLen; ++ i )
    {
        s_au8DrvLD_SPI_delay[i] = _stDrvLDInfo.u8SpiTime_Byte_to_Byte;
    }
    MDrv_SPI_SetWriteBuf_Delay( s_au8DrvLD_SPI_delay, u16TotalSPIDataLen );

#else
#endif
#endif
    //LR and odd vertical block case, need patch for HW Edge2D bug
    if((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH == 2) && (pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV % 2) == 1)
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV+1)-1);
        SC_W2BYTEMSK(REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV+1)-1), 0x003F);
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }
    else if((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH == 2) && !(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV % 2))
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)-1);
        SC_W2BYTEMSK(REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)-1), 0x003F);
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }
    else
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH)-1);
        SC_W2BYTEMSK(REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH)-1), 0x003F);
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }

    u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV-1)<<5)|(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH-1);
    SC_W2BYTEMSK(REG_SC_BK2E_03_L, u16Tmp, 0x03FF);
#if ( HW_DESIGN_LD_VER == 3)
    SC_W2BYTEMSK(REG_SC_BK2E_4C_L, (pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH-1), 0x003F);
    SC_W2BYTEMSK(REG_SC_BK2E_4C_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV-1)<<8), 0x03F00);
#else
    SC_W2BYTEMSK(REG_SC_BK2E_4C_L, u16Tmp, 0x03FF); // For LSF
#endif


    u32Ratio= (H_V_RATIO_CONST * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH / u16PanelWidth)+1;
    SC_W4BYTE(REG_SC_BK2E_19_L, u32Ratio&0x0FFFFF);
    u32Ratio= (H_V_RATIO_CONST * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV / u16PanelHeight)+1;
    SC_W4BYTE(REG_SC_BK2E_1B_L, u32Ratio&0x0FFFFF);

    u32Ratio= H_V_RATIO_CONST*pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH/(u16PanelWidth-1);
    SC_W4BYTE(REG_SC_BK2E_30_L, u32Ratio&0x0FFFFF);
    u32Ratio= H_V_RATIO_CONST*pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV/(u16PanelHeight-1);
    SC_W4BYTE(REG_SC_BK2E_32_L, u32Ratio&0x0FFFFF);


    MDrv_LD_Load_Compensation_Table(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.pu8CompTable,COMPENSATION_LENGTH);

#if (LD_PANEL== 1)
    SC_W2BYTEMSK(REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
#endif

    MDrv_LD_Load_EDGE2D_Table(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.pu8Edge2DTable);
    //MDrv_LD_Print_Compensation_Table();
    return TRUE;
#else
	return TRUE;
#endif
}

EN_LD_PANEL_TYPE Mdrv_LD_GetPanelType(void)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    return  pXCResourcePrivate->stdrv_LD._enLDPanelType;
}

MS_BOOL MDrv_LD_Init(void* pInstance)
{
    if( s_bMDrvLD_InitFinished )
        return TRUE;

    MDrv_LD_CUS_Init(MDrv_LD_GetLedDev());

#if 1
    if (psDrvLdInfo->eStage == LD_SPI_STAGE)
    {
        psDrvLdPQInfo->u8LSFWidth = psDrvLdInfo->u8LEDNumW;
        psDrvLdPQInfo->u8LSFHeight = psDrvLdInfo->u8LEDNumH;
        psDrvLdPQInfo->u8LEDWidth = psDrvLdInfo->u8LEDNumW;
        psDrvLdPQInfo->u8LEDHeight = psDrvLdInfo->u8LEDNumH;
        psDrvLdPQInfo->u8LDFWidth = psDrvLdInfo->u8LEDNumW;
        psDrvLdPQInfo->u8LDFHeight = psDrvLdInfo->u8LEDNumH;
        psDrvLdPQInfo->bEdge2DEn = FALSE;
        psDrvLdPQInfo->pu8CompTable = NULL;
        psDrvLdPQInfo->bCompensationEn = FALSE;
        psDrvLdPQInfo->pu8Edge2DTable = NULL;

        psDrvLdPQInfo->u32LsfInitialPhase_HL = 0x80000;
        psDrvLdPQInfo->u32LsfInitialPhase_HR = 0x80000;
        psDrvLdPQInfo->u32LsfInitialPhase_V = 0x80000;
        psDrvLdPQInfo->u8DCMaxAlpha = 0x8;
        psDrvLdPQInfo->u8CompensationBlend = 0x10;

        psDrvLdPQInfo->u8GDStrength = 0xFF;
        psDrvLdPQInfo->u8LDStrength = 0xFF;
        psDrvLdPQInfo->u8TFStrengthUp = 0x80;
        psDrvLdPQInfo->u8TFStrengthDn = 0xF8;
        psDrvLdPQInfo->u8TFLowThreshold = 0x28;
        psDrvLdPQInfo->u8TFHightThreshold = 0x38;
        psDrvLdPQInfo->u8SFStrength = 0xFF;
        psDrvLdPQInfo->u8SFStrength2 = 0xFF;
        psDrvLdPQInfo->u8SFStrength3 = 0xFF;
        psDrvLdPQInfo->u8BLMinValue = 0x10;
        psDrvLdPQInfo->u8NRStrength = 0x30;
    }
#endif		

#ifdef LD_SUPPORT_1XN_NX1
    if (psDrvLdInfo->u8LEDNumW == 1)
    {
        s_bMDrvLD_Led_1xN = TRUE;
        //psDrvLdPQInfo->u8LSFWidth = 2;
        psDrvLdPQInfo->u8LEDWidth = 2;
        psDrvLdPQInfo->u8LDFWidth = (psDrvLdPQInfo->u8LDFWidth < 2? 2: psDrvLdPQInfo->u8LDFWidth);
    }
    if (psDrvLdInfo->u8LEDNumH == 1)
    {
        s_bMDrvLD_Led_Nx1 = TRUE;
        //psDrvLdPQInfo->u8LSFHeight = 2;
        psDrvLdPQInfo->u8LEDHeight = 2;
        psDrvLdPQInfo->u8LDFHeight = (psDrvLdPQInfo->u8LDFHeight < 2? 2: psDrvLdPQInfo->u8LDFHeight);
    }
#endif

	psDrvLdInfo->u32BaseAddr = s_u32LDBaseAddr;

    MDrv_LD_SPI_Init(pInstance);
    MDrv_LDM_DMA_Init(pInstance);
    MDrv_LD_LDF_Init(pInstance);
    MDrv_LD_LDB_Init(pInstance);
    MDrv_LD_SW_Init(pInstance);

    if (psDrvLdPQInfo->bCompensationEn)
    	MDrv_LD_Load_CompensationTable(pInstance, psDrvLdPQInfo->pu8CompTable);

    if (psDrvLdPQInfo->bEdge2DEn)
        MDrv_LD_Load_Edge2DTable(pInstance, psDrvLdPQInfo->pu8Edge2DTable);

#if SUPPORT_LD_SPI
	if(1)
    //if((CustomerType==CustomerType_TSB_Unicorn)&&(PANEL_TYPE_SEL == PNL_CMIV650DK1_KD1_4K2K120Hz))
    {
        MHal_LD_Set_SPI_VsyncSel(pInstance, 0);//  1:PWM ,0:Vsync
    }
    else
    {
        MHal_LD_Set_SPI_VsyncSel(pInstance, 1);//  1:PWM ,0:Vsync
    }

	MHal_LD_Set_DMA1_PackLength(pInstance, 0x05);
	MHal_LD_Set_DMA1_En(pInstance, TRUE);
#endif
	MHal_LD_InitU02(pInstance);
    MHal_LD_Set_LocalDimmingEn(pInstance, TRUE);

    s_bMDrvLD_InitFinished = TRUE;
    s_bMDrvLD_SignalHalved = FALSE;

	// set the default value to dummy register
	MHal_LD_SetDummyReg(pInstance, 0x334C, 0xFFFF);

    return TRUE;
}


#if 0
static MS_U8 _MDrv_LD_CalCheckSum( MS_U8* pu8DataBuf, MS_U16 u16DataLen )
{
    MS_U8 u8CheckSum = 0;
    MS_U16 i;

    if (pu8DataBuf == NULL)
    {
        return u8CheckSum;
    }
    switch( _stDrvLDInfo.u8CheckSumMode )
    {
        case 1: // TODO
            u8CheckSum = pu8DataBuf[0];
            for( i = 1; i < u16DataLen; ++ i )
            {
                u8CheckSum = u8CheckSum ^ pu8DataBuf[i];
            }
            break;
        default:
            u8CheckSum = 0;
            break;
    }

    return u8CheckSum;
}
#endif

MS_BOOL MDrv_LD_SetLocalDimmingData(MS_U8* pu8LocalDimmingData, MS_U16 u16DataLen)
{
    #if 0
    MS_U8 h,v;
    MS_U16 i;
    MS_U16 u16SPIDataLen = 0;
    #endif

//  PRINT_FUNCTION_LINE();
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if( (pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished == FALSE) || (pu8LocalDimmingData == NULL)
        || (u16DataLen == 0) || (u16DataLen > LD_LED_NUMBER))
    {

        return FALSE;
    }
    #if 0
    printf("MDrv_LD_SetLocalDimmingData:\n");
    i = 0;
    for( v = 0; v < _stDrvLDInfo.u8LedNumV; ++ v )
    {
        for( h = 0; h < _stDrvLDInfo.u8LedNumH; ++ h )
        {
          printf("0x%02X,", pu8LocalDimmingData[i] );
            i += 1;
        }
       printf("\n" );
    }
   #endif

#if 0 // mark for hw spi driver
#if( DRV_LD_USE_SWSPI )
    // Cmd len + data len must be small 256
    if( (_stDrvLDInfo.u8SpiCmdLen+u16DataLen) >= DRV_LD_SPI_BUF_MAX_SIZE )
    {
        return FALSE;
    }

    u16SPIDataLen = _stDrvLDInfo.u8SpiCmdLen;

    // Setup dimming data
    for( i = 0; i < u16DataLen; ++ i )
    {
        s_au8DrvLD_SPIBuf[u16SPIDataLen+i] = pu8LocalDimmingData[i];
    }
    u16SPIDataLen += u16DataLen;

    // Setup check sum
    if( _stDrvLDInfo.u8CheckSumMode )
    {
        s_au8DrvLD_SPIBuf[u16SPIDataLen] = _MDrv_LD_CalCheckSum(s_au8DrvLD_SPIBuf, u16SPIDataLen);
        u16SPIDataLen += 1;
    }

#if 0 // Debug
    {
        printf("Send to SPI:\n");
        for( i = 0; i < u16SPIDataLen; ++ i )
        {
            printf("[%X]", s_au8DrvLD_SPIBuf[i]);
        }
        printf("\n");
    }
#endif

    MDrv_SPI_WriteBytes( s_au8DrvLD_SPIBuf, u16SPIDataLen, E_SPI_TRIGGER_BY_EVERY_V_SYNC);

#else // Use DLM-DMA
#endif
#endif
    return TRUE;
}


void MApi_LD_Set3DMode(MS_BOOL bEnable)
{
#if 0
    SC_W2BYTEMSK(REG_SC_BK2E_29_L, bEnable, BIT(0));
#else
	UNUSED(bEnable);
#endif
}

MS_BOOL MApi_XC_LD_SetLevel_U2(void* pInstance, EN_LD_MODE enMode)
{
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

	MHal_LD_Set_SW_PulseMode(pInstance, 0);
	MHal_LD_Set_LSFOutMode(pInstance, 0);

	switch(enMode)
	{
		case E_LD_MODE_HIGH:
			MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseId(pInstance, 0);
			
			MHal_LD_Set_CompensationEn(pInstance, 1);
			MHal_LD_Set_CompensationBlend(pInstance, 15);
			MHal_LD_Set_SW_SpatialFilterStrength(pInstance, 110);
			MHal_LD_Set_SW_SpatialFilterStrength2(pInstance, 52);
			MHal_LD_Set_SW_SpatialFilterStrength3(pInstance, 21);
			MHal_LD_Set_SW_SpatialFilterStrength4(pInstance, 0);
			MHal_LD_Set_SW_SpatialFilterStrength5(pInstance, 0);
			MHal_LD_Set_LDF_DCMaxAlpha(pInstance, 4);
			MHal_LD_Set_SW_TemporalFilterStrengthUp(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterStrengthDn(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterLowTh(pInstance, 20);
			MHal_LD_Set_SW_TemporalFilterHighTh(pInstance, 54);
			MHal_LD_Set_SW_BacklightOutGamma(pInstance, 4);
			MHal_LD_Set_SW_BacklightInGamma(pInstance, 15);
			MHal_LD_Set_SW_LocalDimmingStrength(pInstance, 255);			
			MHal_LD_Set_SW_GlobalDimmingStrength(pInstance, 255);
			MHal_LD_Set_SW_SpatialFilterModeEn(pInstance, TRUE);

			MHal_LD_Set_SW_ReadLDFEn(pInstance, 1);
			MHal_LD_Set_SW_AlgorithmEn(pInstance, 1);
			MHal_LD_Set_SW_WriteLDBEn(pInstance, 1);
			MHal_LD_Set_SW_WriteSPIEn(pInstance, 1);			
			break;

		case E_LD_MODE_MID:
			MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseId(pInstance, 0);
			
			MHal_LD_Set_CompensationEn(pInstance, 1);
			MHal_LD_Set_CompensationBlend(pInstance, 15);
			MHal_LD_Set_SW_SpatialFilterStrength(pInstance, 100);
			MHal_LD_Set_SW_SpatialFilterStrength2(pInstance, 50);
			MHal_LD_Set_SW_SpatialFilterStrength3(pInstance, 28);
			MHal_LD_Set_SW_SpatialFilterStrength4(pInstance, 0);
			MHal_LD_Set_SW_SpatialFilterStrength5(pInstance, 0);
			MHal_LD_Set_LDF_DCMaxAlpha(pInstance, 4);
			MHal_LD_Set_SW_TemporalFilterStrengthUp(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterStrengthDn(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterLowTh(pInstance, 20);
			MHal_LD_Set_SW_TemporalFilterHighTh(pInstance, 54);
			MHal_LD_Set_SW_BacklightOutGamma(pInstance, 3);
			MHal_LD_Set_SW_BacklightInGamma(pInstance, 15);
			MHal_LD_Set_SW_LocalDimmingStrength(pInstance, 255);			
			MHal_LD_Set_SW_GlobalDimmingStrength(pInstance, 255);
			MHal_LD_Set_SW_SpatialFilterModeEn(pInstance, TRUE);

			MHal_LD_Set_SW_ReadLDFEn(pInstance, 1);
			MHal_LD_Set_SW_AlgorithmEn(pInstance, 1);
			MHal_LD_Set_SW_WriteLDBEn(pInstance, 1);
			MHal_LD_Set_SW_WriteSPIEn(pInstance, 1);			
			break;

		case E_LD_MODE_LOW:
			MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0);
			MHal_LD_Set_SW_PulseId(pInstance, 0);
			
			MHal_LD_Set_CompensationEn(pInstance, 1);
			MHal_LD_Set_CompensationBlend(pInstance, 15);
			MHal_LD_Set_SW_SpatialFilterStrength(pInstance, 92);
			MHal_LD_Set_SW_SpatialFilterStrength2(pInstance, 48);
			MHal_LD_Set_SW_SpatialFilterStrength3(pInstance, 34);
			MHal_LD_Set_SW_SpatialFilterStrength4(pInstance, 0);
			MHal_LD_Set_SW_SpatialFilterStrength5(pInstance, 0);
			MHal_LD_Set_LDF_DCMaxAlpha(pInstance, 4);
			MHal_LD_Set_SW_TemporalFilterStrengthUp(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterStrengthDn(pInstance, 246);
			MHal_LD_Set_SW_TemporalFilterLowTh(pInstance, 20);
			MHal_LD_Set_SW_TemporalFilterHighTh(pInstance, 54);
			MHal_LD_Set_SW_BacklightOutGamma(pInstance, 2);
			MHal_LD_Set_SW_BacklightInGamma(pInstance, 15);
			MHal_LD_Set_SW_LocalDimmingStrength(pInstance, 255);			
			MHal_LD_Set_SW_GlobalDimmingStrength(pInstance, 255);
			MHal_LD_Set_SW_SpatialFilterModeEn(pInstance, TRUE);
			
			MHal_LD_Set_SW_ReadLDFEn(pInstance, 1);
			MHal_LD_Set_SW_AlgorithmEn(pInstance, 1);
			MHal_LD_Set_SW_WriteLDBEn(pInstance, 1);
			MHal_LD_Set_SW_WriteSPIEn(pInstance, 1);			
			break;

		case E_LD_MODE_OFF:
			MHal_LD_Set_CompensationEn(pInstance, FALSE);
			MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0xFF);
			MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0xFF);
			MHal_LD_Set_SW_PulseId(pInstance, 0);
			break;

		case E_LD_MODE_DEMO:
			MHal_LD_Set_CompensationEn(pInstance, TRUE);
			MHal_LD_Set_SW_PulseLEDIntensity(pInstance, 0xFF);
			MHal_LD_Set_SW_PulseLDBIntensity(pInstance, 0xFF);
			MHal_LD_Set_SW_PulseId(pInstance, 2);
			break;

		case E_LD_MODE_LED_DEMO_ON:
			gbLEDDemoModeOn = TRUE;
			MHal_LD_Set_SW_PulseMode(pInstance, 0x0E);
			MHal_LD_Set_LSFOutMode(pInstance, 0x05);
			MHal_LD_Set_CompensationEn(pInstance, FALSE);
			break;

		case E_LD_MODE_LED_DEMO_OFF:
			gbLEDDemoModeOn = FALSE;
			MHal_LD_Set_SW_PulseMode(pInstance, 0);
			MHal_LD_Set_LSFOutMode(pInstance, 0);
			MHal_LD_Set_CompensationEn(pInstance, TRUE);
			break;

		case E_LD_MODE_LED_MIRROR_ON:
			MHal_LD_SetSPIMirror(pInstance, TRUE);
			break;

		case E_LD_MODE_LED_MIRROR_OFF:
			MHal_LD_SetSPIMirror(pInstance, FALSE);
			break;

		default:
#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
                _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

			return FALSE;

	}

    switch(enMode)
    {
        case E_LD_MODE_OFF:
			MHal_LD_EnableLedOutput(pInstance, FALSE);
            break;

		case E_LD_MODE_HIGH:
        case E_LD_MODE_MID:
		case E_LD_MODE_LOW:
		case E_LD_MODE_DEMO:
		case E_LD_MODE_LED_DEMO_ON:
		case E_LD_MODE_LED_DEMO_OFF:
			MHal_LD_EnableLedOutput(pInstance, TRUE);
            break;

		default:
            break;
	}

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    return TRUE;
}

MS_BOOL MApi_XC_LD_SetLevel(EN_LD_MODE enMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_LEVEL XCArgs;
    XCArgs.enMode = enMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_Test1(void)
{
    MS_U16 i;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

//    PRINT_FUNCTION_LINE();

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if( pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished == FALSE )
    {
        return;
    }
    if( pXCResourcePrivate->stdrv_LD._u16LedPos < LD_LED_NUMBER )
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0;
        }
        pXCResourcePrivate->stdrv_LD._au8Test_LedData[pXCResourcePrivate->stdrv_LD._u16LedPos] = 0xFF;
    }
    else if( pXCResourcePrivate->stdrv_LD._u16LedPos == (LD_LED_NUMBER) )
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0;
        }
    }
    else // LD_LED_NUMBER + 1
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0xFF;
        }
    }
    pXCResourcePrivate->stdrv_LD._u16LedPos += 1;

    if( pXCResourcePrivate->stdrv_LD._u16LedPos >= (LD_LED_NUMBER+2) )
    {
        pXCResourcePrivate->stdrv_LD._u16LedPos = 0;
    }

/*
    _au8Test_LedData[0] = 0x33;
    _au8Test_LedData[1] = 0x44;
    _au8Test_LedData[2] = 0x55;
    _au8Test_LedData[3] = 0x66;
    _au8Test_LedData[4] = 0x77;
    _au8Test_LedData[5] = 0x88;
    _au8Test_LedData[6] = 0x99;
    _au8Test_LedData[7] = 0xAA;
*/

    MDrv_LD_SetLocalDimmingData(pXCResourcePrivate->stdrv_LD._au8Test_LedData, LD_LED_NUMBER);
}

//DRAM R/W test, Move frame buffer 0 data from LDF to LDB
void MDrv_LD_Test2(void* pInstance)
{
    MS_U8 u8Idx = 0;
    MS_U16 u16Buf = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    for(u8Idx = 0; u8Idx < (((MS_U16)pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH) *pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV); u8Idx += 2)
    {
        u16Buf = MDrv_LD_MIURead2Bytes(pInstance, LD_BUF_ReadDBuf_0, u8Idx);
        MDrv_LD_MIUWrite2Bytes(pInstance, LD_BUF_WriteBuf_0, u8Idx, u16Buf);
    }
}

#if(HW_DESIGN_LD_VER == 1)
void MDrv_LD_Load_Hscaling_Table(MS_U8* pu8CoefTable)
{
    MS_U8 u8ByAddr = 0;
    MS_U16 u16WIndex = 0;

    SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, (ENABLE<<10), BIT(10)); //Write Horizontal
    if (pu8CoefTable == NULL)
    {
        return;
    }

    while (u8ByAddr < H_SCACLING_TABLE_SIZE)
    {
        SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, u8ByAddr ,0x00FF);

        SC_W2BYTEMSK(0,REG_SC_BK2E_34_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_34_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK2E_35_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_35_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK2E_36_L,  pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_36_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);

        SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, (ENABLE<<8), BIT(8)); //Write trigger
        u8ByAddr++;
    }
}

void MDrv_LD_Load_Vscaling_Table(MS_U8* pu8CoefTable)
{
    MS_U8 u8ByAddr = 0;
    MS_U16 u16WIndex = 0;

    SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, (DISABLE<<10), BIT(10)); //Write Vertical

    if (pu8CoefTable == NULL)
    {
        return;
    }

    while (u8ByAddr < V_SCACLING_TABLE_SIZE)
    {
        SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, u8ByAddr,0x00FF);

        SC_W2BYTEMSK(0,REG_SC_BK2E_34_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_34_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK2E_35_L, pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_35_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK2E_36_L, pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK2E_36_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);

        SC_W2BYTEMSK(0,REG_SC_BK2E_3A_L, (ENABLE<<8), BIT(8)); //Write trigger
        u8ByAddr++;
    }
}

void MDrvl_LD_Set_DmaXOffset(MS_U8 u8DmaIdx, MS_U8 u8XStart, MS_U8 u8XEnd)
{
    MS_U32 u32RegAddr = REG_SC_BK2E_64_L + (u8DmaIdx*2);

    SC_W2BYTEMSK(0,u32RegAddr,u8XStart, 0x001F);
    SC_W2BYTEMSK(0,u32RegAddr,(u8XEnd<<8), 0x1F00);
}

void MDrv_LD_Set_DmaYOffset(MS_U8 u8DmaIdx, MS_U8 u8YStart)
{
    MS_U32 u32RegAddr = REG_SC_BK2E_6C_L + u8DmaIdx/2;
    if(u8DmaIdx%2 == 0)
    {
        SC_W2BYTEMSK(0,u32RegAddr, u8YStart, 0x1F);
    }
    else
    {
        SC_W2BYTEMSK(0,u32RegAddr, (u8YStart)<<8, 0x1F00);
    }
}

#endif


void MDrv_LD_Set_LDF_FrameBufBaseAddr(MS_U8 u8BufIdx, MS_U32 u32LDFBase)
{
#if 0
    MS_U32 u32RegAddr = REG_SC_BK2E_04_L + (u8BufIdx*4);
    SC_W4BYTE(u32RegAddr, (u32LDFBase/LD_MIU_ALIGN)& MEMORY_MASK);
#else
	UNUSED(u8BufIdx);
	UNUSED(u32LDFBase);
#endif
}

MS_U32 MDrv_LD_Get_LDF_FrameBufBaseAddr(MS_U8 u8BufIdx)
{
#if 0
    MS_U32 u32RegAddr = REG_SC_BK2E_04_L + (u8BufIdx*4);
    return ((MS_U32)((SC_R4BYTE(u32RegAddr)& MEMORY_MASK)*LD_MIU_ALIGN) );
#else
	UNUSED(u8BufIdx);
	return 0;
#endif
}


void MDrv_LD_Set_LDB_FrameBufBaseAddr(MS_U8 u8BufIdx, MS_U32 u32LDBBase)
{
#if 0
    MS_U32 u32RegAddr = REG_SC_BK2E_4D_L + (u8BufIdx*4);
    SC_W4BYTE(u32RegAddr, (u32LDBBase/LD_MIU_ALIGN));
#else
	UNUSED(u8BufIdx);
	UNUSED(u32LDBBase);
#endif
}

MS_U32 MDrv_LD_Get_LDB_FrameBufBaseAddr(MS_U8 u8BufIdx)
{
#if 0
    MS_U32 u32RegAddr = REG_SC_BK2E_4D_L + (u8BufIdx*4);
    return ((MS_U32)(SC_R4BYTE(u32RegAddr)& MEMORY_MASK)*LD_MIU_ALIGN);
#else
	UNUSED(u8BufIdx);
	return 0;
#endif
}

void MDrv_LD_Set_LEDData_BaseOffset(MS_U32 u32LEDOffset)
{
#if 0
    //SPI data base address = LDB baseaddress + SPI data base address offset
    SC_W4BYTE(REG_SC_BK2E_7E_L,(u32LEDOffset/LD_MIU_ALIGN) & MEMORY_MASK);
#else
	UNUSED(u32LEDOffset);
#endif
}

MS_U32 MDrv_LD_Get_SPIData_BaseAddr(MS_U8 u8BufIdx)
{
#if 0
    //SPI data base address = LDB baseaddress + SPI data base address offset
    MS_U32 u32SPIBaseOffset = SC_R4BYTE(REG_SC_BK2E_7E_L)& MEMORY_MASK;
    MS_U32 u32LDBBaseAddr = MDrv_LD_Get_LDB_FrameBufBaseAddr(u8BufIdx);
    return (u32LDBBaseAddr + (MS_U32)(u32SPIBaseOffset*LD_MIU_ALIGN));
#else
	UNUSED(u8BufIdx);
	return 0;
#endif
}

void MDrv_LD_Set_EDGE2D_BaseAddr(MS_U32 u32EDGE2DBase)
{
#if 0
    SC_W4BYTE(REG_SC_BK2E_79_L, (u32EDGE2DBase/LD_MIU_ALIGN) & MEMORY_MASK);
#else
	UNUSED(u32EDGE2DBase);
#endif
}

MS_U32 MDrv_LD_Get_EDGE2D_BaseAddr(void)
{
#if 0
    return ((MS_U32)(SC_R4BYTE(REG_SC_BK2E_79_L)& MEMORY_MASK)*LD_MIU_ALIGN);
#else
	return 0;
#endif
}


void MDrv_LD_Set_LocalDimmingEn(MS_BOOL bEn)
{
#if 0
    if (bEn)
    {
        SC_W2BYTEMSK(REG_SC_BK2E_01_L, BIT(0), BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(REG_SC_BK2E_01_L, 0x0, BIT(0));
    }
#else
	UNUSED(bEn);
#endif
}

MS_BOOL MDrv_LD_Get_LocalDimmingEn(void)
{
#if 0
    return ((MS_BOOL)SC_R2BYTE(REG_SC_BK2E_01_L)&0x01);
#else
	return TRUE;
#endif
}

ST_DRV_LD_INFO MDrv_LD_GetLDInfo(void)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrv_LD._stDrvLDInfo;
}

MS_BOOL MApi_Set_BLLevel_U2(void* pInstance, MS_U8 u8BLLevel)
{
#if 0
    SC_W2BYTEMSK(REG_SC_BK2E_29_L, ((MS_U16)(u8BLLevel<<8)), 0xFF00);
#else
    UNUSED(u8BLLevel);
#endif
    return TRUE;
}

MS_BOOL MApi_Set_BLLevel(MS_U8 u8BLLevel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_BL_LEVEL XCArgs;
    XCArgs.u8BLLevel = u8BLLevel;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BL_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags_U2(void* pInstance, MS_BOOL bRefreshSPIData)
{
#if 0
    SC_W2BYTEMSK(REG_SC_BK2E_29_L, (bRefreshSPIData)<<2 ,BIT(2));
#else
	UNUSED(pInstance);
	UNUSED(bRefreshSPIData);
#endif
    return TRUE;
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags(MS_BOOL bRefreshSPIData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_NOTUPDATE_SPI_DATA_FLAGS XCArgs;
    XCArgs.bRefreshSPIData = bRefreshSPIData;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_TurnoffLDBL_U2(void* pInstance, MS_BOOL bTurnoffBL)
{
#if 0
    SC_W2BYTEMSK(REG_SC_BK2E_29_L, (bTurnoffBL)<<1 ,BIT(1));
#else
	UNUSED(pInstance);
	UNUSED(bTurnoffBL);
#endif
    return TRUE;
}

MS_BOOL MApi_Set_TurnoffLDBL(MS_BOOL bTurnoffBL)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_TURNOFF_LDBL XCArgs;
    XCArgs.bTurnoffBL = bTurnoffBL;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_TURNOFF_LDBL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_UsermodeLDFlags_U2(void* pInstance, MS_BOOL bUserLDFlags)
{
#if 0
    SC_W2BYTEMSK(REG_SC_BK2E_29_L, bUserLDFlags , BIT(0));
#else
	UNUSED(pInstance);
	UNUSED(bUserLDFlags);
#endif
    return TRUE;
}

MS_BOOL MApi_Set_UsermodeLDFlags(MS_BOOL bUserLDFlags)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_USERMODEFLAGS XCArgs;
    XCArgs.bUserLDFlags = bUserLDFlags;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_USERMODEFLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#else
MS_BOOL MApi_XC_LD_SetMemoryAddress_U2(void* pInstance,
                                      MS_U8 u8MIU ,
                                      MS_U32 u32LDFBase0,
                                      MS_U32 u32LDFBase1,
                                      MS_U32 u32LDBBase0,
                                      MS_U32 u32LDBBase1,
                                      MS_U32 u32EDGE2DBase,
                                      MS_U32 u32LEDOffset)
{
    //not support
    UNUSED(u8MIU);
    UNUSED(u32LDFBase0);
    UNUSED(u32LDFBase1);
    UNUSED(u32LDBBase0);
    UNUSED(u32LDBBase1);
    UNUSED(u32EDGE2DBase);
    UNUSED(u32EDGE2DBase);
    UNUSED(u32LEDOffset);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetMemoryAddress(MS_U8 u8MIU ,
                                    MS_U32 u32LDFBase0,
                                    MS_U32 u32LDFBase1,
                                    MS_U32 u32LDBBase0,
                                    MS_U32 u32LDBBase1,
                                    MS_U32 u32EDGE2DBase,
                                    MS_U32 u32LEDOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_MEMORYADDRESS XCArgs;
    XCArgs.u8MIU = u8MIU;
    XCArgs.u32LDFBase0 = u32LDFBase0;
    XCArgs.u32LDFBase1 = u32LDFBase1;
    XCArgs.u32LDBBase0 = u32LDBBase0;
    XCArgs.u32LDBBase1 = u32LDBBase1;
    XCArgs.u32EDGE2DBase = u32EDGE2DBase;
    XCArgs.u32LEDOffset = u32LEDOffset;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_MEMORYADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_GetSpiAvgData(void* pInstance, MS_U8 *pu8LDValue)
{
	if(pu8LDValue != NULL)
	{
		*pu8LDValue = MHal_LD_Get_SPI_AvgData(pInstance);
	}
}

MS_BOOL MApi_XC_LD_Get_Value_U2(void* pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    //not support
    UNUSED(u8WSize);
    UNUSED(u8HSize);

	MDrv_LD_GetSpiAvgData(pInstance, pu8LDValue);

    return TRUE;
}

MS_BOOL MApi_XC_LD_Get_Value(MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_LD_VALUE XCArgs;
    XCArgs.pu8LDValue = pu8LDValue;
    XCArgs.u8WSize = u8WSize;
    XCArgs.u8HSize = u8HSize;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LD_VALUE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_Set_SPICmd_U2(void* pInstance, MS_U8 u8Mask, MS_BOOL bEnable)
{
    UNUSED(pInstance);
    UNUSED(u8Mask);
    UNUSED(bEnable);

    return FALSE;
}

MS_BOOL MApi_XC_LD_Set_SPICmd(MS_U8 u8Mask, MS_BOOL bEnable)
{
    UNUSED(u8Mask);
    UNUSED(bEnable);

    return FALSE;
}


MS_BOOL MApi_XC_LD_SetLevel_U2(void* pInstance, EN_LD_MODE enMode)
{
    //not support
    UNUSED(enMode);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetLevel(EN_LD_MODE enMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_LEVEL XCArgs;
    XCArgs.enMode = enMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_Init_U2(void* pInstance, EN_LD_PANEL_TYPE enLDPanelType )
{
    UNUSED(enLDPanelType);
    return TRUE;
}

MS_BOOL MApi_XC_LD_Init(EN_LD_PANEL_TYPE enLDPanelType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_LD_INIT XCArgs;
    XCArgs.enLDPanelType = enLDPanelType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_LD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_BLLevel_U2(void* pInstance, MS_U8 u8BLLevel)
{
    UNUSED(u8BLLevel);
    return TRUE;
}

MS_BOOL MApi_Set_BLLevel(MS_U8 u8BLLevel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_BL_LEVEL XCArgs;
    XCArgs.u8BLLevel = u8BLLevel;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BL_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_TurnoffLDBL_U2(void* pInstance, MS_BOOL bTurnoffBL)
{
    UNUSED(bTurnoffBL);
    return TRUE;
}

MS_BOOL MApi_Set_TurnoffLDBL(MS_BOOL bTurnoffBL)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_TURNOFF_LDBL XCArgs;
    XCArgs.bTurnoffBL = bTurnoffBL;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_TURNOFF_LDBL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_UsermodeLDFlags_U2(void* pInstance, MS_BOOL bUserLDFlags)
{
    UNUSED(bUserLDFlags);
    return TRUE;
}

MS_BOOL MApi_Set_UsermodeLDFlags(MS_BOOL bUserLDFlags)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_USERMODEFLAGS XCArgs;
    XCArgs.bUserLDFlags = bUserLDFlags;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_USERMODEFLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags_U2(void* pInstance, MS_BOOL bRefreshSpidata)
{
    UNUSED(bRefreshSpidata);
    return TRUE;
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags(MS_BOOL bRefreshSPIData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_NOTUPDATE_SPI_DATA_FLAGS XCArgs;
    XCArgs.bRefreshSPIData = bRefreshSPIData;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#endif

