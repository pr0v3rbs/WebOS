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
/// file    drvPNL_ex.c
/// @brief  Panel Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _DRVPNL_EX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "drvPNL_ex.h"
#include "halPNL.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include "string.h"
#define do_div(x,y) ((x)/=(y))
#endif
#include "pnl_hwreg_utility2.h"
// Internal Definition
#include "drvMMIO.h"
#include "pnl_tcon_tbl.h"
#ifndef _DRV_SYS_H_
#include "drvSYS.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define UNUSED(x)       (x=x)
#define TCON_TAB_MAX_SIZE  2048

#define NONPM_BASE                  (0x100000)


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

// Internal Data

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _MDrv_PNL_TCON_DumpPSRegTab(MS_U8 u8TconTab[TCON_TAB_MAX_SIZE], MS_U8 u8Tcontype, MS_U8 *pu8delay)
{
#if 1 // move to APP
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
    MS_U8  u8signal_type = 0;

    PNL_DBG(PNL_DBGLEVEL_INIT, "tab: power sequence \n");

    if (u8TconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8TconTab[u32tabIdx]<<8) + u8TconTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8TconTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8TconTab[(u32tabIdx +3)] & 0xFF;
        u8signal_type = u8TconTab[(u32tabIdx +6)];

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u32Addr = (u32Addr | 0x100000);

        if(u8Tcontype == u8signal_type)
        {
            PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);

            if (u32Addr & 0x1)
    {
                u32Addr --;
                W2BYTEMSK(u32Addr, (u16Value << 8), (u16Mask << 8));
    }
    else
    {
                W2BYTEMSK(u32Addr, u16Value, u16Mask);
    }
            *pu8delay = u8TconTab[(u32tabIdx +5)];
}
        u32tabIdx = u32tabIdx + 7;
    }

    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
#endif
}

MS_U8 MDrv_PNL_TCONMAP_Power_Sequence(MS_U8 *pTCONTable, MS_U8 u8Tcontype, MS_U8 u8TconPnlIndex, MS_BOOL bEnable)
{
	MS_U8 u8delay = 0xFF;
	E_TCON_POWER_SEQUENCE_SIGNAL_TYPE eTconType;
	//u8TconPnlIndex = u8TconPnlIndex;
	switch(u8Tcontype)
	{
		case 0:
			eTconType = TCON_SIGNAL_POL;
			break;
		case 1:
			eTconType = TCON_SIGNAL_VGH;
			break;
		case 2:
			eTconType = TCON_SIGNAL_SOE;
			break;
		case 3:
			eTconType = TCON_SIGNAL_VST;
			break;
		case 4:
			eTconType = TCON_SIGNAL_GCLK;
			break;
		default:
			eTconType = TCON_SIGNAL_NUMS;
			break;
	}

	if (bEnable)
	{
	 // FIXME Where is the TConMAP_Main ?
	  //pTCONTable = TConMAP_Main[eTconPnlIndex].pTConPower_Sequence_OnTab;
	  _MDrv_PNL_TCON_DumpPSRegTab(pTCONTable,eTconType, &u8delay);
	}
	else
	{
	 // FIXME
	 //pTCONTable = TConMAP_Main[eTconPnlIndex].pTConPower_Sequence_OffTab;
	 _MDrv_PNL_TCON_DumpPSRegTab(pTCONTable,eTconType, &u8delay);
	}
	return u8delay;

}



