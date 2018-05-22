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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   str_dscmb.c
/// @brief  DSCMB Suspend-to-RAM (STR) related code
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "mst_str.h"
#include "str_dscmb.h"

#define DSCMB_STR_DBG_ENABLE             1

#if DSCMB_STR_DBG_ENABLE
#define DBG_DSCMB_STR(_f)               (_f)
#define DBG_DSCMB_STR_ERR(_f)           (_f)
#define DBG_DSCMB_STR_ENTRY(_f)         (_f)
#define DBG_DSCMB_STR_INFO(_f)          (_f)
#define DBG_DSCMB_STR_EXIT(_f)          (_f)
#else
#define DBG_DSCMB_STR(_f)               (_f)
#define DBG_DSCMB_STR_ERR(_f)           (_f)
#define DBG_DSCMB_STR_ENTRY(_f)
#define DBG_DSCMB_STR_INFO(_f)
#define DBG_DSCMB_STR_EXIT(_f)
#endif

static void* uInstantDscmb = NULL;
static void* uAttributeDscmb = NULL;


extern int suspend_dscmb(void)
{
    DBG_DSCMB_STR_ENTRY(printk("[DSCMB STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("[DSCMB STR Enter]\n");

    Utopia_MApi_Dscmb_UtopiaOpen();

//Utopia_MApi_Dscmb_Init();//only for test

    if( MDrv_DSCMB2_SetPowerState(E_POWER_SUSPEND) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] MDrv_DSCMB2_SetPowerState(E_POWER_SUSPEND) fail. \n", __FUNCTION__, __LINE__));
    }

    Utopia_MApi_Dscmb_UtopiaClose();

    DBG_DSCMB_STR_EXIT(printk("[DSCMB STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("Function Exit\n");
    return 0;
}

extern int resume_dscmb(void)
{
    DBG_DSCMB_STR_ENTRY(printk("[DSCMB STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("[DSCMB STR Enter]\n");

    Utopia_MApi_Dscmb_UtopiaOpen();

    if( MDrv_DSCMB2_SetPowerState(E_POWER_RESUME) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] MDrv_DSCMB2_SetPowerState(E_POWER_RESUME) fail. \n", __FUNCTION__, __LINE__));
    }

    Utopia_MApi_Dscmb_UtopiaClose();

    DBG_DSCMB_STR_EXIT(printk("[DSCMB STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));
    STR_PRINT("Function Exit\n");

    return 0;
}

MS_BOOL Utopia_MApi_Dscmb_UtopiaOpen(void)
{
    DBG_DSCMB_STR_ENTRY(printk("[DSCMB STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));
    if ( uInstantDscmb != NULL)
    {
    	DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] uInstantDscmb not null. \n", __FUNCTION__, __LINE__));
    }
    if( UtopiaOpen(MODULE_DSCMB, &uInstantDscmb, 0, uAttributeDscmb) !=  UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] UtopiaOpen MODULE_DSCBB fail. \n", __FUNCTION__, __LINE__));

        if(UtopiaClose(uInstantDscmb) != UTOPIA_STATUS_SUCCESS)
        {
            DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] UtopiaClose MODULE_DSCBB fail. \n", __FUNCTION__, __LINE__));
			uInstantDscmb = NULL;
            return FALSE;
        }
        return FALSE;
    }

    return TRUE;
}

MS_BOOL Utopia_MApi_Dscmb_Init(void)
{
    DBG_DSCMB_STR_ENTRY(printk("[DSCMB STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( UtopiaIoctl(uInstantDscmb, E_MDRV_CMD_DSCMB_Init,NULL) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] UtopiaIoctl E_MDRV_CMD_DSCMB_Init fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    DBG_DSCMB_STR_EXIT(printk("[DSCMB STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));

    return TRUE;
}

MS_BOOL Utopia_MApi_Dscmb_SetPowerState(void)
{
    DBG_DSCMB_STR_ENTRY(printk("[DSCMB STR Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( UtopiaIoctl(uInstantDscmb, E_MDRV_CMD_DSCMB_SetPowerState,NULL) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB STR Error][%s][%d] UtopiaIoctl E_MDRV_CMD_DSCMB_SetPowerState fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    DBG_DSCMB_STR_EXIT(printk("[DSCMB STR Exit][%s][%d].\n", __FUNCTION__, __LINE__));

    return TRUE;
}


MS_BOOL Utopia_MApi_Dscmb_UtopiaClose(void)
{
    MS_U32 u32Ret = FALSE;

    DBG_DSCMB_STR_ENTRY(printk("[DSCMB UTP Entry][%s][%d].\n", __FUNCTION__, __LINE__));

    if( uInstantDscmb == NULL )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB UTP Error][%s][%d] uInstantDscmb == NULL. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    //u32Ret = UtopiaIoctl(uInstantDscmb, E_MDRV_CMD_DSCMB_Exit, NULL);//only for test
    //if( u32Ret != UTOPIA_STATUS_SUCCESS )
    //{
    //    DBG_DSCMB_STR_ERR(printk("[DSCMB UTP Error][%s][%d] (%ld)u32Ret != UTOPIA_STATUS_SUCCESS. \n", __FUNCTION__, __LINE__, u32Ret));
    //    return FALSE;
    //}

    if( UtopiaClose(uInstantDscmb) != UTOPIA_STATUS_SUCCESS )
    {
        DBG_DSCMB_STR_ERR(printk("[DSCMB UTP Error][%s][%d] UtopiaClose MODULE_DSCBB fail. \n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    uInstantDscmb = NULL;
    uAttributeDscmb = NULL;

    DBG_DSCMB_STR_EXIT(printk("[DSCMB UTP Exit][%s][%d].\n", __FUNCTION__, __LINE__));

    return TRUE;
}


