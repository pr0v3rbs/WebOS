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
////////////////////////////////////////////////////////////////////////////////
// Common Definition
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvPWM.h"
#include "halPWM.h"
#include "drvMMIO.h"
#include "MsDevice.h"
#include "MsOS.h"
#include "drvSEM.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

#define PWM_DBG_FUNC()            if (_geDbfLevel >= E_PWM_DBGLV_ALL)                   \
                                       {MS_DEBUG_MSG(printf("\t====   %s   ====\n", __FUNCTION__));}

#define PWM_DBG_INFO(x, args...)  if (_geDbfLevel >= E_PWM_DBGLV_INFO )                 \
                                       {MS_DEBUG_MSG(printf(x, ##args));}

#define PWM_DBG_ERR(x, args...)   if (_geDbfLevel >= E_PWM_DBGLV_ERR_ONLY)				\
                                       {MS_DEBUG_MSG(printf(x, ##args));}


#define PWM_SEM_LOCK()  //MDrv_SEM_Lock(E_SEM_PM, SEM_WAIT_FOREVER);
#define PWM_SEM_UNLOCK() //MDrv_SEM_Unlock(E_SEM_PM);

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL  _gbInitPWM = FALSE;
static PWM_DbgLv _gbInitPWM_DbgLv = E_PWM_DBGLV_NONE;
//static PWM_Arg _gsPWMDesc;
static PWM_DbgLv _geDbfLevel = E_PWM_DBGLV_NONE;
static MSIF_Version _drv_pwm_version = {
    .DDI = { PWM_DRV_VERSION },
};

static PWM_DrvStatus   _pwmStatus =
                        {
                            .PWM0 = FALSE,
                            .PWM1 = FALSE,
                            .PWM2 = FALSE,
                            .PWM3 = FALSE,
                            .PWM4 = FALSE,
                            .PWM5 = FALSE,
                            .PWM6 = FALSE,
                            .PWM7 = FALSE,
                            .PWM8 = FALSE,
                        };

#if (PWM_UTOPIA20)
#include "utopia.h"
#endif

#if (PWM_UTOPIA20)
MS_BOOL bU20PWMOpened = FALSE;
void* pInstantPwm;
void* pAttributePwm;
#endif
////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _PWM_IsInit(void);

//-------------------------------------------------------------------------------------------------
/// Description: Set debug level for debug message
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_SetDbgLevel(PWM_DbgLv eLevel)
{
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, eLevel);

    _geDbfLevel = eLevel;

    return E_PWM_OK;
}

//------------------------------------------------------------------------------
/// Description : Show the PWM driver version
/// @param  ppVersion \b OUT: output PWM driver version
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetLibVer(const MSIF_Version **ppVersion)
{
    PWM_DBG_FUNC();

    if (!ppVersion)
        return E_PWM_FAIL;

    *ppVersion = &_drv_pwm_version;
    return E_PWM_OK;
}

//------------------------------------------------------------------------------
/// Description : Show the PWM_PAD is PWM(True) or GPIO(false)
/// @param  pStatus \b OUT: output PWM driver status
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetStatus(PWM_DrvStatus *pStatus)
{
    memcpy(pStatus, &_pwmStatus, sizeof(PWM_DrvStatus));
    return E_PWM_OK;
}

//-------------------------------------------------------------------------------------------------
/// Description : Check PWM is initial or not
/// @return TRUE : PWM was initied
/// @return FALSE : PWM was not initied
//-------------------------------------------------------------------------------------------------
static MS_BOOL _PWM_IsInit(void)
{
    PWM_DBG_FUNC();
    if (!_gbInitPWM)
        PWM_DBG_ERR("Call MDrv_PWM_Init first!\n");
    return _gbInitPWM;
}

//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Init(PWM_DbgLv eLevel)
{
    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseAddr1 = 0;
    MS_U32 u32BaseAddr2 = 0;
    MS_U32 u32BaseSize = 0; /* No use */

	_gbInitPWM_DbgLv = eLevel; //store for STD resume
    MDrv_PWM_SetDbgLevel(eLevel);

    if(!_gbInitPWM)
    {
        if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PWM))
        {
            PWM_DBG_ERR("Get IOMAP Base faill!\n");
            return E_PWM_FAIL;
        }

        if(!MDrv_MMIO_GetBASE(&u32BaseAddr1, &u32BaseSize, MS_MODULE_CHIPTOP))
        {
            PWM_DBG_ERR("Get CHIPTOP Base faill!\n");
            return E_PWM_FAIL;
        }

        if(!MDrv_MMIO_GetBASE(&u32BaseAddr2, &u32BaseSize, MS_MODULE_PM))
        {
            PWM_DBG_ERR("Get PM Base faill!\n");
            return E_PWM_FAIL;
        }
        MHAL_PWM_SetIOMapBase(u32BaseAddr, u32BaseAddr2);
        MHAL_PWM_SetChipTopIOMapBase(u32BaseAddr1);
        PWM_DBG_INFO("%s has MMIO base 0x%08X\r\n", __FUNCTION__, (int) u32BaseAddr);
        /* Set All pad output and Is PWM. But it still needs to be enable */
        if( !MHAL_PWM_Init() )
        {
            PWM_DBG_ERR("PWM Hal Initial Fail\n");
            return E_PWM_FAIL;
        }

        //MDrv_SEM_Init(); // for sc4 bank protect

        _gbInitPWM = TRUE;
        return E_PWM_OK;
    }
    else
    {
        PWM_DBG_ERR("PWM has be initiated!\n");
        return E_PWM_OK;
    }
}

//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result _MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM)
{
    PWM_Result ret = E_PWM_OK;
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bOenPWM);

    switch(u8IndexPWM)
    {
        case E_PWM_CH0:
            _pwmStatus.PWM0 = TRUE;
            break;
        case E_PWM_CH1:
            _pwmStatus.PWM1 = TRUE;
            break;
        case E_PWM_CH2:
            _pwmStatus.PWM2 = TRUE;
            break;
        case E_PWM_CH3:
            _pwmStatus.PWM3 = TRUE;
            break;
        case E_PWM_CH4:
            _pwmStatus.PWM4 = TRUE;
            break;
        case E_PWM_CH5:
            _pwmStatus.PWM5 = TRUE;
            break;
        case E_PWM_CH6:
            _pwmStatus.PWM6 = TRUE;
            break;
        case E_PWM_CH7:
            _pwmStatus.PWM7 = TRUE;
            break;
        case E_PWM_CH8:
            _pwmStatus.PWM8 = TRUE;
            break;
        default:
            MS_ASSERT(0);
    }

    if( !(MHAL_PWM_Oen(u8IndexPWM, bOenPWM)||_PWM_IsInit()) )
    {
        ret = E_PWM_FAIL;
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result _MDrv_PWM_Period(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int)u32PeriodPWM);

    PWM_SEM_LOCK();
    do{
        MHAL_PWM_Period(u8IndexPWM, u32PeriodPWM);
        ret = E_PWM_OK;
    }while(0);
    PWM_SEM_UNLOCK();

    return ret;
}


PWM_Result MDrv_PWM_GetProperty(PWM_Property eProperty, PWM_ChNum u8IndexPWM, MS_U32 *u32PWMVar)
{
    PWM_SEM_LOCK();
    switch(eProperty)
    {
        case E_PWM_GetPeriod:
            *u32PWMVar = MHAL_PWM_GetPeriod(u8IndexPWM);
            break;
        case E_PWM_GetDutyCycle:
            *u32PWMVar = MHAL_PWM_GetDutyCycle(u8IndexPWM);
            break;
        default:
            printf("[Utopia] PWM is not support!!\n");
            return E_PWM_FAIL;
            break;
    }
     PWM_SEM_UNLOCK();
    return E_PWM_OK;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result _MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int)u32DutyPWM);

    PWM_SEM_LOCK();
    do{
        MHAL_PWM_DutyCycle(u8IndexPWM, u32DutyPWM);
        ret = E_PWM_OK;
    }while(0);
    PWM_SEM_UNLOCK();

    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Unit_Div of the pwm
/// @param u16UnitDivPWM    \b the Unit Div value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result _MDrv_PWM_UnitDiv(MS_U16 u16UnitDivPWM)
{
    PWM_Result ret = E_PWM_OK;
    PWM_DBG_INFO("%s(0x%04X)\r\n", __FUNCTION__, u16UnitDivPWM);

    PWM_SEM_LOCK();
    if(!MHAL_PWM_UnitDiv(u16UnitDivPWM))
    {
        ret = E_PWM_FAIL;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
void _MDrv_PWM_Div(PWM_ChNum u8IndexPWM, MS_U16 u16DivPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, 0x%04X)\r\n", __FUNCTION__, u8IndexPWM, u16DivPWM);
    MHAL_PWM_Div(u8IndexPWM, u16DivPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
void _MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, MS_BOOL bPolPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bPolPWM);
    MHAL_PWM_Polarity(u8IndexPWM, bPolPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Vsync Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bVdbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void _MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bVdbenPWM);
    MHAL_PWM_VDBen(u8IndexPWM, bVdbenPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Hsync reset of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bRstPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void _MDrv_PWM_ResetEn(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bRstPWM);
    MHAL_PWM_Vrest(u8IndexPWM, bRstPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void _MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bDbenPWM);
    MHAL_PWM_DBen(u8IndexPWM, bDbenPWM);
    PWM_SEM_UNLOCK();
}

void MDrv_PWM_IMPULSE_EN(PWM_ChNum u8IndexPWM, MS_BOOL bdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bdbenPWM);
    MHAL_PWM_IMPULSE_EN(u8IndexPWM, bdbenPWM);
    PWM_SEM_UNLOCK();
}

void MDrv_PWM_ODDEVEN_SYNC(PWM_ChNum u8IndexPWM, MS_BOOL bdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bdbenPWM);
    MHAL_PWM_ODDEVEN_SYNC(u8IndexPWM, bdbenPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Rst Mux of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bMuxPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstMux(PWM_ChNum u8IndexPWM, MS_BOOL bMuxPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bMuxPWM);
    MHAL_PWM_RstMux(u8IndexPWM, bMuxPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Rst_Cnt of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u8RstCntPWM    \b the Rst_Cnt value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstCnt(PWM_ChNum u8IndexPWM, MS_U8 u8RstCntPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, 0x%02X)\r\n", __FUNCTION__, u8IndexPWM, u8RstCntPWM);
    MHAL_PWM_RstCnt(u8IndexPWM, u8RstCntPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Bypass Unit of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bBypassPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_BypassUnit(PWM_ChNum u8IndexPWM, MS_BOOL bBypassPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bBypassPWM);
    MHAL_PWM_BypassUnit(u8IndexPWM, bBypassPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM0 and PWM1
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM1 donate internal divider to PWM0    \n
///     10: PWM0 donate internal divider to PWM1    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM01_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( MHAL_PWM01_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM2 and PWM3
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM3 donate internal divider to PWM2    \n
///     10: PWM2 donate internal divider to PWM3    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM23_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( MHAL_PWM23_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM6 and PWM7
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM7 donate internal divider to PWM6    \n
///     10: PWM6 donate internal divider to PWM7    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM67_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( MHAL_PWM67_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Shift of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Shift value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result _MDrv_PWM_Shift(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int) u32DutyPWM);
    PWM_SEM_LOCK();
    if ( MHAL_PWM_Shift(u8IndexPWM, u32DutyPWM) )
    {
        ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}


void _MDrv_PWM_Nvsync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bNvsPWM);
    MHAL_PWM_Nvsync(u8IndexPWM, bNvsPWM);
    PWM_SEM_UNLOCK();
}


void MDrv_PWM_Align(PWM_ChNum u8IndexPWM, MS_BOOL bAliPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bAliPWM);
    MHAL_PWM_Align(u8IndexPWM, bAliPWM);
    PWM_SEM_UNLOCK();
}


//-------------PM BASE---------------

void MDrv_PM_PWM_Enable(void)
{
    MHAL_PM_PWM_Enable();
}

void MDrv_PM_PWM_Period(MS_U16 u16PeriodPWM)
{
   MHAL_PM_PWM_Period(u16PeriodPWM);
}

void MDrv_PM_PWM_DutyCycle(MS_U16 u16DutyPWM)
{
   MHAL_PM_PWM_DutyCycle(u16DutyPWM);
}

void MDrv_PM_PWM_Div(MS_U8 u8DivPWM)
{
    MHAL_PM_PWM_Div(u8DivPWM);
}

void MDrv_PM_PWM_Polarity(MS_BOOL bPolPWM)
{
    MHAL_PM_PWM_Polarity(bPolPWM);
}

void MDrv_PM_PWM_Dben(MS_BOOL bDbenPWM)
{
    MHAL_PM_PWM_DBen(bDbenPWM);
}
MS_U16 MDrv_PWM_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitPWM = FALSE;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_PWM_Init(_gbInitPWM_DbgLv);

			_prev_u16PowerState = u16PowerState;
			u16Return = 1;//RESUME_OK;
		}
		else
		{
			printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = 3;//SUSPEND_FAILED;
		}
	}
	else
	{
		printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = FALSE;
	}

	return u16Return;// for success
}

//-------------------------------------------------------------------------------------------------
/// Description: inverse 3D flag
/// @param bInvPWM   \b True/False for Enable/Disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_INV_3D_Flag(MS_BOOL bInvPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    if ( MHAL_PWM_INV_3D_Flag(bInvPWM))
    {
        ret = E_PWM_OK;
    }
    return ret;
}

#if (PWM_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_PWM_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20PWMOpened)
    {
        if(UtopiaOpen(MODULE_PWM | KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open PWM fail\n");
    		return FALSE;
        }
        bU20PWMOpened = TRUE;
    }
    return TRUE;
}
#endif

PWM_Result MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM)
{
#if (PWM_UTOPIA20)
    MS_U32 u32Ret;
    PWM_PARAM_Oen PwmParam_Oen;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return 0;
    PwmParam_Oen.bOenPWM = bOenPWM;
    PwmParam_Oen.u8IndexPWM = u8IndexPWM;
    u32Ret = UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Oen,(MS_U32*)&PwmParam_Oen);
    return (PWM_Result)u32Ret;
#else
     return _MDrv_PWM_Oen(u8IndexPWM,bOenPWM);
#endif
}

PWM_Result MDrv_PWM_Period(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM)
{
#if (PWM_UTOPIA20)
    MS_U32 u32Ret;
    PWM_PARAM_Period PwmParam_Period;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return 0;
    PwmParam_Period.u8IndexPWM = u8IndexPWM;
    PwmParam_Period.u32PeriodPWM = u32PeriodPWM;
    u32Ret = UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Period,(MS_U32*) &PwmParam_Period);
    return (PWM_Result)u32Ret;
#else
     return _MDrv_PWM_Period(u8IndexPWM,u32PeriodPWM);
#endif
}

PWM_Result MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
#if (PWM_UTOPIA20)
    MS_U32 u32Ret;
    PWM_PARAM_DutyCycle PwmParam_DutyCycle;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return 0;
    PwmParam_DutyCycle.u32DutyPWM = u32DutyPWM;
    PwmParam_DutyCycle.u8IndexPWM = u8IndexPWM;
    u32Ret = UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_DutyCycle,(MS_U32*)&PwmParam_DutyCycle);
    return (PWM_Result)u32Ret;
#else
     return _MDrv_PWM_DutyCycle(u8IndexPWM,u32DutyPWM);
#endif
}

PWM_Result MDrv_PWM_UnitDiv(MS_U16 u16UnitDivPWM)
{
#if (PWM_UTOPIA20)
    MS_U32 u32Ret;
    PWM_PARAM_UnitDiv PwmParam_UnitDiv;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return 0;
    PwmParam_UnitDiv.u16UnitDivPWM = u16UnitDivPWM;
    u32Ret = UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_UnitDiv,(MS_U32*) &PwmParam_UnitDiv);
    return (PWM_Result)u32Ret;
#else
     return _MDrv_PWM_UnitDiv(u16UnitDivPWM);
#endif
}

void MDrv_PWM_Div(PWM_ChNum u8IndexPWM, MS_U16 u16DivPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_Div PwmParam_Div;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_Div.u8IndexPWM = u8IndexPWM;
    PwmParam_Div.u16DivPWM = u16DivPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Div,(MS_U32*)&PwmParam_Div);
#else
    _MDrv_PWM_Div(u8IndexPWM,u16DivPWM);
#endif
}

void MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, MS_BOOL bPolPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_Polarity PwmParam_Polarity;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_Polarity.u8IndexPWM = u8IndexPWM;
    PwmParam_Polarity.bPolPWM = bPolPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Polarity,(MS_U32*) &PwmParam_Polarity);
#else
    _MDrv_PWM_Polarity(u8IndexPWM,bPolPWM);
#endif
}

void MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_Vdben PwmParam_Vdben;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_Vdben.u8IndexPWM = u8IndexPWM;
    PwmParam_Vdben.bVdbenPWM = bVdbenPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Vdben,(MS_U32*) &PwmParam_Vdben);
#else
    _MDrv_PWM_Vdben(u8IndexPWM,bVdbenPWM);
#endif
}

void MDrv_PWM_ResetEn(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_ResetEn PwmParam_ResetEn;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_ResetEn.u8IndexPWM = u8IndexPWM;
    PwmParam_ResetEn.bRstPWM = bRstPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_ResetEn,(MS_U32*)&PwmParam_ResetEn);
#else
    _MDrv_PWM_ResetEn(u8IndexPWM,bRstPWM);
#endif
}

void MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_Dben PwmParam_Dben;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_Dben.u8IndexPWM = u8IndexPWM;
    PwmParam_Dben.bDbenPWM = bDbenPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Dben,(MS_U32*) &PwmParam_Dben);
#else
    _MDrv_PWM_Dben(u8IndexPWM,bDbenPWM);
#endif
}

PWM_Result MDrv_PWM_Shift(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
#if (PWM_UTOPIA20)
    MS_U32 u32Ret;
    PWM_PARAM_Shift PwmParam_Shift;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return 0;
    PwmParam_Shift.u8IndexPWM = u8IndexPWM;
    PwmParam_Shift.u32DutyPWM = u32DutyPWM;
    u32Ret = UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Shift,(MS_U32*) &PwmParam_Shift);
    return (PWM_Result)u32Ret;
#else
     return _MDrv_PWM_Shift(u8IndexPWM,u32DutyPWM);
#endif
}

void MDrv_PWM_Nvsync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM)
{
#if (PWM_UTOPIA20)
    PWM_PARAM_Nvsync PwmParam_Nvsync;
    if (FALSE == _MDrv_PWM_CheckUtopia20Open(&pInstantPwm, 0, pAttributePwm))
        return ;
    PwmParam_Nvsync.u8IndexPWM = u8IndexPWM;
    PwmParam_Nvsync.bNvsPWM = bNvsPWM;
    UtopiaIoctl(pInstantPwm,MDrv_CMD_PWM_Nvsync,(MS_U32*)&PwmParam_Nvsync);
#else
    _MDrv_PWM_Nvsync(u8IndexPWM,bNvsPWM);
#endif
}
