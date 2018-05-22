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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// file   drvDSCMB.c
/// @brief  Descrambler (DSCMB) Driver Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "drvMMIO.h"
#include "drvCA.h"
#include "regCA.h"
#include "halCA.h"

static MS_BOOL _bCADrv_Inited = FALSE;

#define DRV_CA_ASSERT( _bool_, _msg_, ret) \
    do { if(!(_bool_)) { printf(_msg_); return (ret); } } while(0)

#define DRV_CA_INIT_CHECK(ret) DRV_CA_ASSERT(_bCADrv_Inited == TRUE, "The drvCA is not initialized\n", ret)

//--------------------------------------------------------------------------------------------------
/// Initialize CA driver
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_Init(void)
{
    MS_U32 u32RegBank, u32BankSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32RegBank, &u32BankSize, MS_MODULE_HW))
    {
        MS_DEBUG_MSG(printf("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    HAL_CA_SetBank(u32RegBank);

    if(HAL_CA_Init() == FALSE)
    {
        MS_DEBUG_MSG(printf("[%s][%d] HAL CA init fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    _bCADrv_Inited = TRUE;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// @brief Enable Secure Boot
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_EnableSecureBoot();
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if Secure Boot Enabled
/// @return TRUE: Enabled
/// @return FALSE: Disabled
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsSecureBootEnabled();
}

//-------------------------------------------------------------------------------------------------
/// @brief Check if the value is zero in OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_IsBlank(addr);
}

//-------------------------------------------------------------------------------------------------
/// @brief Read 32bits value from OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @return OTP value
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_CA_OTP_Read(MS_U32 addr)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Read(addr);
}

//-------------------------------------------------------------------------------------------------
/// @brief Write 32bits value to OTP
/// @param[in] addr from 0x0000 to 0x1ffc
/// @param[in] value
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value)
{
    DRV_CA_INIT_CHECK(FALSE);
    return HAL_CA_OTP_Write(addr, value);
}
