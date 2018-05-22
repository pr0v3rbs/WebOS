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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_GOP_H_
#define _HAL_GOP_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#include "mdrv_types.h"

#define MAX_GOP_SUPPORT                             4
#define MAX_GOP_MUX                                 4
#define MAX_GOP0_GWIN                               2
#define MAX_GOP1_GWIN                               2
#define MAX_GOP2_GWIN                               1
#define MAX_GOP3_GWIN                               1

#define GOP0_Gwin0Id                                0
#define GOP0_Gwin1Id                                1
#define GOP1_Gwin0Id                                2
#define GOP1_Gwin1Id                                3
#define GOP2_Gwin0Id                                4
#define GOP3_Gwin0Id                                5

#define GOP0_GwinIdBase                             GOP0_Gwin0Id
#define GOP1_GwinIdBase                             MAX_GOP0_GWIN
#define GOP2_GwinIdBase                             MAX_GOP0_GWIN + MAX_GOP1_GWIN
#define GOP3_GwinIdBase                             MAX_GOP0_GWIN + MAX_GOP1_GWIN + MAX_GOP2_GWIN
#define GOP_TotalGwinNum                            (MAX_GOP0_GWIN+MAX_GOP1_GWIN+MAX_GOP2_GWIN+MAX_GOP3_GWIN)

#define GOP_BIT0    0x01
#define GOP_BIT1    0x02
#define GOP_BIT2    0x04
#define GOP_BIT3    0x08
#define GOP_BIT4    0x10
#define GOP_BIT5    0x20
#define GOP_BIT6    0x40
#define GOP_BIT7    0x80
#define GOP_BIT8    0x0100
#define GOP_BIT9    0x0200
#define GOP_BIT10   0x0400
#define GOP_BIT11   0x0800
#define GOP_BIT12   0x1000
#define GOP_BIT13   0x2000
#define GOP_BIT14   0x4000
#define GOP_BIT15   0x8000

#define GOP_REG_WORD_MASK                           0xffff
#define GOP_REG_HW_MASK                             0xff00
#define GOP_REG_LW_MASK                             0x00ff

#define GOP_FIFO_BURST_ALL                          (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)
#define GOP_FIFO_BURST_MIDDLE                       (GOP_BIT8|GOP_BIT9)
#define GOP_FIFO_BURST_SHORT                        (GOP_BIT8)

#define GOP_FIFO_BURST_MASK                         (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)
#define GOP_FIFO_THRESHOLD                          0x30

#define GOP_WordUnit                                32
#define GOP_PD 										0xEB
#define GOP_Mux0_Offset 							0x0
#define GOP_Mux1_Offset 							0x3
#define GOP_Mux2_Offset 							0x1
#define GOP_Mux3_Offset 							0x2
#define bAutoAdjustMirrorHSize 						TRUE

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define EN_DBG_GOP              0
//#define MAX_GWIN_SUPPORT        4       ///< Specify the number of GWIN per GOP
#define MAX_GOP0_GWIN_SUPPORT        2       ///< Specify the number of GWIN per GOP
#define MAX_GOP1_GWIN_SUPPORT        2       ///< Specify the number of GWIN per GOP
#define MAX_GOP2_GWIN_SUPPORT        1       ///< Specify the number of GWIN per GOP
#define MAX_GOP3_GWIN_SUPPORT        1       ///< Specify the number of GWIN per GOP
#define MAX_GOP_SUPPORT                   4       ///< Specify the number of GOP (2(GOP4G) + 1(GOP1G))
#define GOP23_GWINID_OFFSET           0   //in different chip, maybe gop2/gop3 gwinid offset maybe different, ex: T3: gwinid is 1 of gop2/gop3, T8:gwinid is 0 of gop2/gop3
#define TOTAL_GWIN_SUPPORT      (MAX_GOP0_GWIN_SUPPORT + MAX_GOP1_GWIN_SUPPORT + MAX_GOP2_GWIN_SUPPORT + MAX_GOP3_GWIN_SUPPORT) //9       ///< Specify the total GWIN (2x4(GOP4G)+1(GOP1G))

#define GOP_PALETTE_ENTRY_NUM   256
#define BLINK_PALETTE_ENTRY_NUM 32

#define GOP_IP_PD          10    ///Chip characteristic
#define GOP_OP_PD          0x12F
#define GOP_DST_TYGE_VALIDBITS   2

#define GOP_VSYNC_INTERRUPT_FLIP_ENABLE 0
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

#define _BIT_MASK(bits)                  ((1<<bits)-1)

/// Define GOP HW Type
typedef enum
{
    /// GOP 0 (4 Gwin Support)
    E_GOP4G_0 = 0,
    /// GOP 1 (2 Gwin Support)
    E_GOP2G_1 = 1,
    /// GOP SIGNAL (1 Gwin Support)
    E_GOP1G = 2,
    /// GOP SIGNAL (1 Gwin Support which can be used for cursor)
    E_GOP1GX = 3,
}GOP_HW_Type;

typedef enum
{
    /// Color format RGB555 and Blink.
    E_GOP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_GOP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_GOP_COLOR_ARGB4444        =2,
    /// Color format 555.
    E_GOP_COLOR_RGB555          =3,
    /// Color format I8 (256-entry palette).
    E_GOP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_GOP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_GOP_COLOR_ARGB1555        =6,
    /// Color format I2.
    E_GOP_COLOR_I2              =7,
    /// Color format YUV422_VU7Y8, Packet format: V7Y8,U7Y8,......
    E_GOP_COLOR_YUV422_VU7Y8    =8,
    /// Color format YUV422_VU8Y8. Packet format: V8Y8,U8Y8,......
    E_GOP_COLOR_YUV422_VU8Y8    =9,
    /// Color format I1
    E_GOP_COLOR_I1              =10,
    /// Invalid color format.
    E_GOP_COLOR_INVALID
} GOPColorType;



typedef struct {
    GOP_HW_Type eGOP_Type;
    U8 u8Wid;
    GOPColorType eColorType;
    U32 u32SrcX;
    U32 u32SrcY;
    U32 u32DispX;
    U32 u32DispY;
    U32 u32Width;
    U32 u32Height;
    U32 u32DRAMRBlkStart;
    U32 u32DRAMRBlkHSize;
    U32 u32DRAMRBlkVSize;
} MS_GOP_CREATE_GWIN, *PMS_GOP_CREATE_GWIN;


// GOP DWIN (1 Dwin Support)
#define E_GOP_DWIN 4

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
U8 _HAL_GOP_GetBnkOfstByGop(U8 gop, U32 *pBnkOfst);
void _HAL_GOP_Read16Reg(U32 u32addr, U16* pu16ret);
void _HAL_GOP_Write16Reg(U32 u32addr, U16 u16val, U16 mask);
void _HAL_GOP_Write32Reg(U32 u32addr, U32 u32val);
void _HAL_GOP_Init(U8 u8GOP);
void _HAL_GOP_SetBlending(U8 u8GOP, U8 u8win, U8 bEnable, U8 u8coef);
void _HAL_GOP_SetGOPEnable2SC(U8 u8GOP, U8 bEnable);
extern B16 MHal_GOP_ClearIRQ( void ) ;
extern B16 MHal_GOP_MaskIRQ( void ) ;

#endif // _HAL_TEMP_H_
