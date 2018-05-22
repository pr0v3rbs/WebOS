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
/// file    drvAVD.c
/// @brief  AVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "utopia.h"   //dan@20130502 merge STR function from utopia_fastboot
// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_ATSC.h"
#include "halDMD_INTERN_ATSC.h"
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_u8DMD_ATSC_DbgLevel == DMD_ATSC_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_ATSC_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_ATSC_Mutex);\
        if (_u8DMD_ATSC_DbgLevel == DMD_ATSC_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 0
static MSIF_Version _drv_dmd_dvbt_intern_version = {
    .MW = { DMD_DVBT_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_atsc_intern_version;
#endif

static DMD_ATSC_InitData _sDMD_ATSC_InitData;
static DMD_ATSC_DbgLv _u8DMD_ATSC_DbgLevel=DMD_ATSC_DBGLV_NONE;
static MS_S32 _s32DMD_ATSC_Mutex=-1;
static DMD_ATSC_Info sDMD_ATSC_Info;

MS_U16  u16DMD_ATSC_TUNER_IF_KHZ;
MS_BOOL bDMD_ATSC_TUNER_IS_IQ_SWAP;
static DMD_ATSC_DEMOD_TYPE  _eLAST_ATSC_DEMOD_TYPE = DMD_ATSC_DEMOD_NULL; //dan@20130502 merge STR function from utopia_fastboot
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_ATSC_Init(DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen)
{
    MS_BOOL bRFAGCTristateEnable;
    if (_s32DMD_ATSC_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init more than once\n"));
        return FALSE;
    }

    if ((_s32DMD_ATSC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD DVBT", MSOS_PROCESS_SHARED)) == -1)
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMD_ATSC_DbgLevel=DMD_ATSC_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_INFO)
    {
        printf("MDrv_DMD_ATSC_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_ATSC_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_ATSC_InitData, pDMD_ATSC_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init input data structure incorrect\n"));
        return FALSE;
    }

    //MDrv_SYS_DMD_VD_MBX_Init();
    HAL_DMD_RegInit();

    if (_sDMD_ATSC_InitData.u8DMD_ATSC_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_ATSC_InitData.u8DMD_ATSC_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 1;
    }
    
    u16DMD_ATSC_TUNER_IF_KHZ   = _sDMD_ATSC_InitData.u16IF_KHZ;
    bDMD_ATSC_TUNER_IS_IQ_SWAP = _sDMD_ATSC_InitData.bIQSwap;
    
    DMD_LOCK();
    HAL_INTERN_ATSC_InitClk(bRFAGCTristateEnable);
    HAL_INTERN_ATSC_Download();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_ATSC);
    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_Exit(void)
{
    DMD_LOCK();
    // Kenji HAL_INTERN_ATSC_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_ATSC_Mutex);
    _s32DMD_ATSC_Mutex= -1;
    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMD_ATSC_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

DMD_ATSC_Info* MDrv_DMD_ATSC_GetInfo(void)
{
    sDMD_ATSC_Info.u8Version=0;
    return &sDMD_ATSC_Info;
}

MS_BOOL MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_atsc_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ATSC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ATSC_SetSerialControl(MS_BOOL bEnable)
{
    return TRUE;
}

void MDrv_DMD_ATSC_SetReset(void)
{
    DMD_LOCK();
    HAL_INTERN_ATSC_SoftReset();
    DMD_UNLOCK();
}

MS_BOOL MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    MS_BOOL bRet=true;
    MS_BOOL bRFAGCTristateEnable;

    if (_sDMD_ATSC_InitData.u8DMD_ATSC_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_ATSC_InitData.u8DMD_ATSC_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 1;
    }
    DMD_LOCK();
    HAL_INTERN_ATSC_InitClk(bRFAGCTristateEnable);
    HAL_INTERN_ATSC_Download();

    HAL_INTERN_ATSC_SoftReset();
    if (bEnable)
    {
        switch (eType)
        {
            case DMD_ATSC_DEMOD_ATSC_VSB:
                bRet = HAL_INTERN_ATSC_SetVsbMode();
                break;
            case DMD_ATSC_DEMOD_ATSC_64QAM:
                bRet = HAL_INTERN_ATSC_Set64QamMode();
                break;
            case DMD_ATSC_DEMOD_ATSC_256QAM:
                bRet = HAL_INTERN_ATSC_Set256QamMode();
                break;
            default:
                bRet = HAL_INTERN_ATSC_SetModeClean();
                break;
        }
    }

    sDMD_ATSC_Info.u32ATSCScanTimeStart=MsOS_GetSystemTime();
    sDMD_ATSC_Info.eStatus=DMD_ATSC_CHECKING;
    sDMD_ATSC_Info.u32ATSCLockStatus=0;
    
    _eLAST_ATSC_DEMOD_TYPE = eType;    //dan@20130502 merge STR function from utopia_fastboot
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ATSC_SetActive(MS_BOOL bEnable)
{
    //MS_BOOL bRet;
    //DMD_LOCK();
    // Kenji bRet=INTERN_DVBT_Active(bEnable);
    //DMD_UNLOCK();
    //return bRet;
    return TRUE;
}

static DMD_ATSC_LOCK_STATUS _MDrv_DMD_ATSC_VSB_CheckLock(void)
{
	  //printf("_MDrv_DMD_ATSC_VSB_CheckLock\n");
    if (HAL_INTERN_ATSC_Vsb_FEC_Lock())
    {
        sDMD_ATSC_Info.u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_FEC_LOCK;
        sDMD_ATSC_Info.u32ATSCFECLockTime = MsOS_GetSystemTime();
        return DMD_ATSC_LOCK;
    }
    else
    {
        #if 1 // follow DVBT?
        if ((sDMD_ATSC_Info.u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_FEC_LOCK) && ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCFECLockTime) < 100))
        {
            return DMD_ATSC_LOCK;
        }
        else
        {
            sDMD_ATSC_Info.u32ATSCLockStatus &= (~DMD_ATSC_LOCK_VSB_FEC_LOCK);
        }
        #endif

        if (sDMD_ATSC_Info.u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_PRE_LOCK)
        {
            if (sDMD_ATSC_Info.u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_FSYNC_LOCK)
            {
                if ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCScanTimeStart) < _sDMD_ATSC_InitData.u16VSBFECLockCheckTime)
                {
                    return DMD_ATSC_CHECKING;
                }
            }
            else
            {
                if (HAL_INTERN_ATSC_Vsb_FSync_Lock())
                {
                    sDMD_ATSC_Info.u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_FSYNC_LOCK;
                    //printf("DMD_ATSC_LOCK_VSB_FSYNC_LOCK\n");
                    #ifdef MS_DEBUG
                    if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                    {
                        printf("DMD_ATSC_LOCK_VSB_FSYNC_LOCK\n");
                    }
                    #endif
                    return DMD_ATSC_CHECKING;
                }
                else
                {
                	  //printf("aa: %ld %ld\n",MsOS_GetSystemTime(),sDMD_ATSC_Info.u32ATSCScanTimeStart);
                    if ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCScanTimeStart) < _sDMD_ATSC_InitData.u16VSBFSyncLockCheckTime)
                    {
                        return DMD_ATSC_CHECKING;
                    }
                }
            }
        }
        else
        {
            if (HAL_INTERN_ATSC_Vsb_PreLock())
            {
                sDMD_ATSC_Info.u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_PRE_LOCK;
                //printf("DMD_ATSC_LOCK_VSB_PRE_LOCK\n");
                #ifdef MS_DEBUG
                if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                {
                    printf("DMD_ATSC_LOCK_VSB_PRE_LOCK\n");
                }
                #endif
                return DMD_ATSC_CHECKING;
            }
            else
            {
                if ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCScanTimeStart) < _sDMD_ATSC_InitData.u16VSBPreLockCheckTime)
                {
                    return DMD_ATSC_CHECKING;
                }
            }
        }
        return DMD_ATSC_UNLOCK;
    }
}

static DMD_ATSC_LOCK_STATUS _MDrv_DMD_ATSC_QAM_CheckLock(void)
{
    if (HAL_INTERN_ATSC_QAM_Main_Lock())
    {
        sDMD_ATSC_Info.u32ATSCLockStatus |= DMD_ATSC_LOCK_QAM_MAIN_LOCK;
        sDMD_ATSC_Info.u32ATSCFECLockTime = MsOS_GetSystemTime();
        return DMD_ATSC_LOCK;
    }
    else
    {
        #if 1 // follow DVBT?
        if ((sDMD_ATSC_Info.u32ATSCLockStatus & DMD_ATSC_LOCK_QAM_MAIN_LOCK) && ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCFECLockTime) < 100))
        {
            return DMD_ATSC_LOCK;
        }
        else
        {
            sDMD_ATSC_Info.u32ATSCLockStatus &= (~DMD_ATSC_LOCK_QAM_MAIN_LOCK);
        }
        #endif

        if (sDMD_ATSC_Info.u32ATSCLockStatus & DMD_ATSC_LOCK_QAM_PRE_LOCK)
        {
            if ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCScanTimeStart) < _sDMD_ATSC_InitData.u16QAMMainLockCheckTime)
            {
                return DMD_ATSC_CHECKING;
            }
        }
        else
        {
            if (HAL_INTERN_ATSC_QAM_PreLock())
            {
                sDMD_ATSC_Info.u32ATSCLockStatus |= DMD_ATSC_LOCK_QAM_PRE_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                {
                    printf("DMD_ATSC_LOCK_QAM_PRE_LOCK\n");
                }
                #endif
                return DMD_ATSC_CHECKING;
            }
            else
            {
                if ((MsOS_GetSystemTime()-sDMD_ATSC_Info.u32ATSCScanTimeStart) < _sDMD_ATSC_InitData.u16QAMPreLockCheckTime)
                {
                    return DMD_ATSC_CHECKING;
                }
            }
        }
        return DMD_ATSC_UNLOCK;
    }
}

DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK_TYPE eType)
{
    DMD_LOCK();
    switch (eType)
    {
        case DMD_ATSC_GETLOCK:
            switch (HAL_INTERN_ATSC_Check8VSB64_256QAM())
            {
                case DMD_ATSC_DEMOD_ATSC_VSB:
                    sDMD_ATSC_Info.eStatus = _MDrv_DMD_ATSC_VSB_CheckLock();
                    break;
                case DMD_ATSC_DEMOD_ATSC_64QAM:
                    sDMD_ATSC_Info.eStatus = _MDrv_DMD_ATSC_QAM_CheckLock();
                    break;
                case DMD_ATSC_DEMOD_ATSC_256QAM:
                    sDMD_ATSC_Info.eStatus = _MDrv_DMD_ATSC_QAM_CheckLock();
                    break;
                default:
                    break;
            }
            break;
        case DMD_ATSC_GETLOCK_VSB_AGCLOCK:
            //return (HAL_INTERN_ATSC_Vsb_QAM_AGCLock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_Vsb_QAM_AGCLock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK;  
            break;
        case DMD_ATSC_GETLOCK_VSB_PRELOCK:
            //return (HAL_INTERN_ATSC_Vsb_PreLock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_Vsb_PreLock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK;  
            break;
        case DMD_ATSC_GETLOCK_VSB_FSYNCLOCK:
        case DMD_ATSC_GETLOCK_VSB_CELOCK:
            //return (HAL_INTERN_ATSC_Vsb_FSync_Lock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_Vsb_FSync_Lock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK;  
            break;
        case DMD_ATSC_GETLOCK_VSB_FECLOCK:
            //return (HAL_INTERN_ATSC_Vsb_FEC_Lock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_Vsb_FEC_Lock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK;  
            break;
        case DMD_ATSC_GETLOCK_QAM_AGCLOCK:
            //return (HAL_INTERN_ATSC_Vsb_QAM_AGCLock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_Vsb_QAM_AGCLock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK; 
            break;
        case DMD_ATSC_GETLOCK_QAM_PRELOCK:
            //return (HAL_INTERN_ATSC_QAM_PreLock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_QAM_PreLock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK; 
            break;
        case DMD_ATSC_GETLOCK_QAM_MAINLOCK:
            //return (HAL_INTERN_ATSC_QAM_Main_Lock()) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            if(HAL_INTERN_ATSC_QAM_Main_Lock())
                sDMD_ATSC_Info.eStatus = DMD_ATSC_LOCK;
            else
                sDMD_ATSC_Info.eStatus = DMD_ATSC_UNLOCK; 
            break;
    }
    DMD_UNLOCK();
    return sDMD_ATSC_Info.eStatus;
}

DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_GetModulationMode(void)
{
    DMD_ATSC_DEMOD_TYPE eType;
    DMD_LOCK();
    eType=HAL_INTERN_ATSC_Check8VSB64_256QAM();
    DMD_UNLOCK();
    return eType;
}

MS_BOOL MDrv_DMD_ATSC_GetSignalStrength(MS_U16 *u16Strength)
{
    DMD_LOCK();
    *u16Strength = HAL_INTERN_ATSC_ReadIFAGC();
    DMD_UNLOCK();
    return TRUE;
}

DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_GetSignalQuality(void)
{
    DMD_ATSC_SIGNAL_CONDITION eSignalQuality;
    DMD_LOCK();
    HAL_INTERN_ATSC_CheckSignalCondition(&eSignalQuality);
    DMD_UNLOCK();
    return eSignalQuality;
}

MS_U8 MDrv_DMD_ATSC_GetSNRPercentage(void)
{
    MS_U8 bRet;
    DMD_LOCK();
    bRet=HAL_INTERN_ATSC_ReadSNRPercentage();
    DMD_UNLOCK();
    return bRet;
}
//dan@20130502 merge STR function from utopia_fastboot[begin]
MS_U32 MDrv_DMD_ATSC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
	  
    if (u16PowerState == E_POWER_SUSPEND)
    {
        DMD_LOCK();
        HAL_INTERN_ATSC_TURN_ON_DOWNLOAD_FW_FLOW();
        DMD_UNLOCK();
        MDrv_DMD_ATSC_Exit();

        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {  
            MDrv_DMD_ATSC_Init(&_sDMD_ATSC_InitData, sizeof(_sDMD_ATSC_InitData));
		        
            MDrv_DMD_ATSC_SetConfig(_eLAST_ATSC_DEMOD_TYPE, TRUE);
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;  
	}    
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
    	u32Return = FALSE;
    }  
    return UTOPIA_STATUS_SUCCESS;
}
//dan@20130502 merge STR function from utopia_fastboot[end]

#endif//#if 0
