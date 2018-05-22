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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_common.h"
#include "drvSAR.h"
#include "utopia.h"   
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
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBT_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBT_Mutex);\
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_dvbt_intern_version = {
    .MW = { DMD_DVBT_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

static DMD_DVBT_InitData _sDMD_DVBT_InitData;
static DMD_DbgLv _u8DMDDbgLevel=DMD_DBGLV_NONE;
static MS_S32 _s32DMD_DVBT_Mutex=-1;
static DMD_DVBT_Info sDMD_DVBT_Info;
static MS_U16 u16DMD_DVBT_TPS_Timeout = 1500, u16DMD_DVBT_FEC_Timeout=6000;
static MS_U32 u32DMD_DVBT_IfFrequency = 36167L, u32DMD_DVBT_FsFrequency = 45474L;
static MS_U8 u8DMD_DVBT_IQSwap=0;
static DMD_RF_CHANNEL_BANDWIDTH eDMD_DVBT_BandWidth=E_DMD_RF_CH_BAND_8MHz;
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
MS_BOOL MDrv_DMD_DVBT_Init(DMD_DVBT_InitData *pDMD_DVBT_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBT_MutexString[16];
    MS_U8 u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    if (_s32DMD_DVBT_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBT_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBT_MutexString,"Mutex DMD DVBT",16))
    {
        DMD_DBG(printf("MDrv_DMD_DVBT_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBT_MutexString, MSOS_PROCESS_SHARED);
    if (_s32DMD_DVBT_Mutex == -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBT_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMDDbgLevel = DMD_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBT_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DVBT_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBT_InitData, pDMD_DVBT_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DVBT_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        MDrv_SAR_Adc_Config(_sDMD_DVBT_InitData.u8SarChannel, TRUE);
    }

    DMD_LOCK();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBT);
    HAL_DMD_RegInit();

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
        }
        else
        {
            bRFAGCTristateEnable = 1;
            bIFAGCTristateEnable = 0;
        }
    }
    else
    {
        bRFAGCTristateEnable = 1;
        bIFAGCTristateEnable = 0;
    }

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=3)
        {
            u32DMD_DVBT_IfFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[4]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[5]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[6]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[7]; // IF frequency
            u32DMD_DVBT_FsFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[8]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[9]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[10]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[11]; // FS frequency
            u8DMD_DVBT_IQSwap = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[12]; // IQ Swap

            u8ADCIQMode = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }
    #ifdef MS_DEBUG
    printf("u32DMD_DVBT_IfFrequency %ld\n",u32DMD_DVBT_IfFrequency);
    printf("u32DMD_DVBT_FsFrequency %ld\n",u32DMD_DVBT_FsFrequency);
    printf("u8DMD_DVBT_IQSwap %d\n",u8DMD_DVBT_IQSwap);
    #endif

    u16DMD_DVBT_TPS_Timeout = 1500;
    u16DMD_DVBT_FEC_Timeout = 6000;
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=4)
        {
            u16DMD_DVBT_TPS_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[17]; // TPS timeout in ms
            u16DMD_DVBT_TPS_Timeout =  (u16DMD_DVBT_TPS_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[18];
            if (u16DMD_DVBT_TPS_Timeout < 700) u16DMD_DVBT_TPS_Timeout=700;
            //printf("u16DMD_DVBT_TPS_Timeout %d\n",u16DMD_DVBT_TPS_Timeout);

            u16DMD_DVBT_FEC_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[19]; // FEC timeout in ms
            u16DMD_DVBT_FEC_Timeout =  (u16DMD_DVBT_FEC_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[20];
            if (u16DMD_DVBT_FEC_Timeout < 2500) u16DMD_DVBT_FEC_Timeout=2500;
            //printf("u16DMD_DVBT_FEC_Timeout %d\n",u16DMD_DVBT_FEC_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize);
        }
        else
        {
            printf("u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }

    INTERN_DVBT_Version(&sDMD_DVBT_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    printf("firmware version: %x\n",sDMD_DVBT_Info.u16Version);
    #endif
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBT_Exit(void)
{
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_Exit\n");
    }
    #endif
    DMD_LOCK();
    INTERN_DVBT_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBT_Mutex);
    _s32DMD_DVBT_Mutex= -1;
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBT_SetDbgLevel(DMD_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMDDbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

DMD_DVBT_Info* MDrv_DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE eInfoType)
{
    DMD_LOCK();
    switch (eInfoType)
    {
        case E_DMD_DVBT_MODULATION_INFO:
            INTERN_DVBT_Show_Modulation_info();
            break;
        case E_DMD_DVBT_DEMOD_INFO:
            INTERN_DVBT_Show_Demod_Info();
            break;
        case E_DMD_DVBT_LOCK_INFO:
            INTERN_DVBT_Show_Lock_Info();
            break;
        case E_DMD_DVBT_PRESFO_INFO:
            INTERN_DVBT_Show_PRESFO_Info();
            break;
        case E_DMD_DVBT_LOCK_TIME_INFO:
            INTERN_DVBT_Show_Lock_Time_Info();
            break;
        case E_DMD_DVBT_BER_INFO:
            INTERN_DVBT_Show_BER_Info();
            break;
        case E_DMD_DVBT_AGC_INFO:
            INTERN_DVBT_Show_AGC_Info();
            break;
        default:
            #ifdef MS_DEBUG
            printf("MDrv_DMD_DVBT_GetInfo %d Error\n", eInfoType);
            #endif
            break;
    }
    DMD_UNLOCK();
    return &sDMD_DVBT_Info;
}

MS_BOOL MDrv_DMD_DVBT_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbt_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBT_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBT_Version(ver);
    //printf("MDrv_DMD_DVBT_GetFWVer %x\n",*ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_DMD_DVBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_SetSerialControl %x\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; // TS_CLK
        }
        else
        {
            u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
        }
    }
    else
    {
        u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
    }
    bRet=INTERN_DVBT_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_SetConfig(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG)
{
    return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW, bSerialTS, bPalBG, 0, u32DMD_DVBT_IfFrequency, u32DMD_DVBT_FsFrequency, u8DMD_DVBT_IQSwap);
}


MS_BOOL MDrv_DMD_DVBT_SetConfigHPLP(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel)
{
    return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW, bSerialTS, bPalBG, bLPSel, u32DMD_DVBT_IfFrequency, u32DMD_DVBT_FsFrequency, u8DMD_DVBT_IQSwap);
}


MS_BOOL MDrv_DMD_DVBT_SetConfigHPLPSetIF(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U32 u32IFFreq, MS_U32 u32FSFreq, MS_U8 u8IQSwap)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_SetConfigHPLPSetIF %d %d %d %d %ld %ld %d\n", BW, bSerialTS, bPalBG, bLPSel, u32IFFreq, u32FSFreq, u8IQSwap);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; // TS_CLK
        }
        else
        {
            u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
        }
    }
    else
    {
        u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
    }

    bRet=INTERN_DVBT_Config(BW, bSerialTS, bPalBG, bLPSel, u8TSClk, u32IFFreq, u32FSFreq, u8IQSwap);
    eDMD_DVBT_BandWidth=BW;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBT_Active(bEnable);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE eType, DMD_LOCK_STATUS *eLockStatus)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();

    if ( eType == E_DMD_DMD_DVBT_GETLOCK ) // for channel scan
    {
        *eLockStatus = INTERN_DVBT_Lock(u16DMD_DVBT_TPS_Timeout, u16DMD_DVBT_FEC_Timeout);
    }
    else
    {
        if (INTERN_DVBT_GetLock(eType) == TRUE)
        {
            *eLockStatus = E_DMD_LOCK;
        }
        else
        {
            *eLockStatus = E_DMD_UNLOCK;
        }
    }
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetLock %d\n", bRet);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}

MS_BOOL MDrv_DMD_DVBT_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    MS_U8 u8SarValue;
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT_GetSignalStrength(u16Strength, (const DMD_DVBT_InitData *)(&_sDMD_DVBT_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBT_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}

MS_BOOL MDrv_DMD_DVBT_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_U8 u8SarValue=0;
    MS_BOOL bRet=0;

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT_GetSignalQuality(u16Quality, (const DMD_DVBT_InitData *)(&_sDMD_DVBT_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetSNR(float *fSNR)
{
    DMD_LOCK();
    *fSNR=INTERN_DVBT_GetSNR();
    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL MDrv_DMD_DVBT_GetPostViterbiBer(float *ber)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_GetPostViterbiBer(ber);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetPreViterbiBer(float *ber)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_GetPreViterbiBer(ber);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;
    float   fBER;

    DMD_LOCK();
    INTERN_DVBT_GetPostViterbiBer(&fBER);
    bRet=INTERN_DVBT_GetPacketErr(pktErr);
    if ((*pktErr ==1) && (fBER<= 0.000001)) // for no signal case, from Oga
    {
        *pktErr = 0x3FF;
    }
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetPacketErr %d\n", *pktErr);
    }
    #endif
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetTPSInfo(MS_U16 *u16Info)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_Get_TPS_Info(u16Info);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetCellID(MS_U16 *u16CellID)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_Get_CELL_ID(u16CellID);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetCellID %d\n", *u16CellID);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_GetFreqOffset(float *pFreqOff)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8BW=8;

    DMD_LOCK();
    switch (eDMD_DVBT_BandWidth)
    {
        case E_DMD_RF_CH_BAND_6MHz:
            u8BW=6;
            break;

        case E_DMD_RF_CH_BAND_7MHz:
            u8BW=7;
            break;

        case E_DMD_RF_CH_BAND_8MHz:
        default:
            u8BW=8;
            break;
    }
    bRet=INTERN_DVBT_Get_FreqOffset(pFreqOff, u8BW);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT_GetStatus %d %f\n", u8BW, *pFreqOff);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel, DMD_CODERATE code_rate, float write_value)
{
    return INTERN_DVBT_NORDIG_SSI_Table_Write(constel, code_rate, write_value);
}

MS_BOOL MDrv_DMD_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL constel, DMD_CODERATE code_rate, float *read_value)
{
    return INTERN_DVBT_NORDIG_SSI_Table_Read(constel, code_rate, read_value);
}

MS_U32 MDrv_DMD_DVBT_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = u32Return;
    if (u16PowerState == E_POWER_SUSPEND)
	{
        MDrv_DMD_DVBT_Exit();
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            MDrv_DMD_DVBT_Init(&_sDMD_DVBT_InitData, sizeof(_sDMD_DVBT_InitData));
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
