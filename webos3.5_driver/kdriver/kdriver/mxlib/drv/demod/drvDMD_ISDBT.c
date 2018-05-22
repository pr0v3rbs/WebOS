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
#include "utopia.h"
// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_ISDBT.h"
#ifdef CONFIG_AMAZON
//#include "../../hal/amazon/halDMD_EXTERN_ISDBT.h"
//#include "../../hal/macaw12/amazon/halDMD_EXTERN_ISDBT.h"
//#include "../../hal/k1/amazon/halDMD_EXTERN_ISDBT.h"
//#include "../../hal/a7/amazon/halDMD_EXTERN_ISDBT.h"

//#include "halDMD_INTERN_ISDBT.h"
#else
//#include "halDMD_INTERN_ISDBT.h"
#endif
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// TS Series mode
// ISDBT_TS_SERIAL = 0x01 => series mode
// ISDBT_TS_SERIAL = 0x00 => parallel mode
// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#if 0
#define TS_CLK_INV         1
#define TS_PARALLEL_OUTPUT 1
#define ISDBT_TS_CLK_INV        TS_CLK_INV  // TS clock inverseion
#if TS_PARALLEL_OUTPUT
#define ISDBT_TS_SERIAL     0x00
#define ISDBT_TS_CLK_DIVNUM   31
#define ISDBT_TS_DATA_SWAP   0x00  // TS data inversion
#else
#define ISDBT_TS_SERIAL     0x01
#define ISDBT_TS_CLK_DIVNUM     4
#define ISDBT_TS_DATA_SWAP  0x00  // TS data inversion
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_u8DMD_ISDBT_DbgLevel == DMD_ISDBT_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_ISDBT_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_ISDBT_Mutex);\
        if (_u8DMD_ISDBT_DbgLevel == DMD_ISDBT_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)


s_ISDBT_HAL_FUNCTION_TABLE    sISDBT_HAL_FUNCTION_TABLE = {0};

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MSIF_Version _drv_dmd_ISDBT_intern_version;

static DMD_ISDBT_InitData _sDMD_ISDBT_InitData;
static DMD_ISDBT_DbgLv _u8DMD_ISDBT_DbgLevel=DMD_ISDBT_DBGLV_NONE;
static MS_S32 _s32DMD_ISDBT_Mutex=-1;
static DMD_ISDBT_Info sDMD_ISDBT_Info;

MS_U16  u16DMD_ISDBT_AGC_REF_VALUE = 0x00; 
MS_U16  u16DMD_ISDBT_TUNER_IF_KHZ; 
MS_U8    u8DMD_ISDBT_TS_CONFIG;
MS_BOOL bDMD_ISDBT_TUNER_IS_IQ_SWAP;
MS_U32  u32DMD_ISDBT_TDI_START_ADDR;

extern MS_U8    u8DMD_I2C_SLAVE_ADDR;
extern MS_U8    u8DMD_I2C_SLAVE_BUS;
extern MS_BOOL  bIS_EXTERN_DMD;
extern s_I2C_Interface_func sI2cInterfaceFunc;    

/*
#ifdef CONFIG_AMAZON
MS_BOOL (*HAL_DMD_ISDBT_IOCTL_CMD)(e_HAL_COMMAND eCmd, void *pHandle) = HAL_EXTERN_ISDBT_IOCTL_CMD;
#else
MS_BOOL (*HAL_DMD_ISDBT_IOCTL_CMD)(e_HAL_COMMAND eCmd, void *pHandle) = HAL_INTERN_ISDBT_IOCTL_CMD;
#endif
*/
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
MS_BOOL MDrv_DMD_ISDBT_Init(DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen)
{
    MS_BOOL bRFAGCTristateEnable;
    MS_U16  u16DmdFwVer;
    if (_s32DMD_ISDBT_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init more than once\n"));
        return FALSE;
    }

    if ((_s32DMD_ISDBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD ISDBT", MSOS_PROCESS_SHARED)) == -1)
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMD_ISDBT_DbgLevel=DMD_ISDBT_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMD_ISDBT_DbgLevel >= DMD_ISDBT_DBGLV_INFO)
    {
        printf("MDrv_DMD_ISDBT_Init\n");
    }
    #endif
    
    if ( sizeof(_sDMD_ISDBT_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_ISDBT_InitData, pDMD_ISDBT_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init input data structure incorrect\n"));
        return FALSE;
    }
    
    //MDrv_SYS_DMD_VD_MBX_Init();
    HAL_DMD_RegInit();
    
    if (_sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 1;
    }
    bRFAGCTristateEnable = bRFAGCTristateEnable;
    u16DMD_ISDBT_AGC_REF_VALUE = _sDMD_ISDBT_InitData.u16AgcReferenceValue;
    u16DMD_ISDBT_TUNER_IF_KHZ   = _sDMD_ISDBT_InitData.u16IF_KHZ;
    bDMD_ISDBT_TUNER_IS_IQ_SWAP = _sDMD_ISDBT_InitData.bIQSwap;
    u32DMD_ISDBT_TDI_START_ADDR= _sDMD_ISDBT_InitData.u32TdiStartAddr;
    
    u8DMD_ISDBT_TS_CONFIG = (_sDMD_ISDBT_InitData.u5TsConfigByte_DivNum << 3) |
                                                  (_sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv << 2) |
                                                  (_sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap << 1) |
                                                  (_sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode);


    u8DMD_I2C_SLAVE_ADDR = _sDMD_ISDBT_InitData.u8I2CSlaveAddr;
    u8DMD_I2C_SLAVE_BUS = _sDMD_ISDBT_InitData.u8I2CSlaveBus;
    bIS_EXTERN_DMD = _sDMD_ISDBT_InitData.bIsExtDemod;

    sI2cInterfaceFunc.I2C_WriteBytes = _sDMD_ISDBT_InitData.I2C_WriteBytes;
    sI2cInterfaceFunc.I2C_ReadBytes = _sDMD_ISDBT_InitData.I2C_ReadBytes;
    DMD_LOCK();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_ISDBT);
    if(sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD == NULL)
    {
        DMD_UNLOCK();
        printf("HAL_DMD_ISDBT_IOCTL_CMD doesn't initiate!!!\n");
        return FALSE;
    }

    if(bIS_EXTERN_DMD == FALSE)
    {
        sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_InitClk,NULL);
    }
    
    /*
    if(HAL_DMD_ISDBT_IOCTL_CMD == NULL)
    {
        DMD_UNLOCK();
        printf("HAL_DMD_ISDBT_IOCTL_CMD doesn't initiate!!!\n");
        return FALSE;
    }
    */
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Download, NULL); 
    
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_FWVERSION, (void *)&u16DmdFwVer);
    //HAL_EXTERN_ISDBT_IOCTL_CMD(CMD_ISDBT_FWVERSION, NULL);

    
    DMD_UNLOCK();
    
    return TRUE;
}

MS_BOOL MDrv_DMD_ISDBT_Exit(void)
{
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_SoftReset, NULL); //dan add to force demod enter reset mode
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Exit, NULL);
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_ISDBT_Mutex);
    _s32DMD_ISDBT_Mutex= -1;
    return TRUE;
}

MS_BOOL MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMD_ISDBT_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

DMD_ISDBT_Info* MDrv_DMD_ISDBT_GetInfo(void)
{
    sDMD_ISDBT_Info.u8Version=0;
    return &sDMD_ISDBT_Info;
}

MS_BOOL MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_ISDBT_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_ISDBT_GetFwVer(MS_U16 *pu16FwVer)
{
    MS_BOOL bRet = TRUE; 
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_FWVERSION, (void *)pu16FwVer);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_DMD_ReadReg(u16Addr, pu8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_DMD_WriteReg(u16Addr, u8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_SetSerialControl(MS_U8 u8TsConfigData)
{
    MS_BOOL bRet=TRUE;
    
    /*
    u8DMD_ISDBT_TS_CONFIG &= ~0xF9;
    if(bEnable == TRUE) //Serial mode
    {
        // Fix clock rate of Series mode to 216/4= 54 MHz
        u8DMD_ISDBT_TS_CONFIG = 0x01;
        u8DMD_ISDBT_TS_CONFIG |= 0x04 << 3;    
    }
    else
    {
        // Fix clock rate of Series mode to 216/31= 6.97 MHz
        u8DMD_ISDBT_TS_CONFIG &= ~0x01;
        u8DMD_ISDBT_TS_CONFIG |= 0x1F << 3;    
    }
    */

    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_TS_INTERFACE_CONFIG, &u8TsConfigData);
    DMD_UNLOCK();    
    return bRet;
}

void MDrv_DMD_ISDBT_SetReset(void)
{
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_SoftReset, NULL);
    DMD_UNLOCK();
}

MS_BOOL MDrv_DMD_ISDBT_SetConfig(MS_BOOL bEnable)
{
    MS_BOOL bRet=TRUE;
    MS_BOOL bRFAGCTristateEnable;
    if (_sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 1;
    }
    bRFAGCTristateEnable = bRFAGCTristateEnable;
    DMD_LOCK();
    if(bIS_EXTERN_DMD == FALSE)
    {
        sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_InitClk,NULL);
    }
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Download, NULL);
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_SoftReset, NULL);
    if (bEnable)
    {
        bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Active, NULL);
    }


    sDMD_ISDBT_Info.u32ISDBTScanTimeStart=MsOS_GetSystemTime();
    sDMD_ISDBT_Info.eStatus=DMD_ISDBT_CHECKING;
    sDMD_ISDBT_Info.u32ISDBTLockStatus=0;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;
    //DMD_LOCK();
    //bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_SoftReset, NULL);
    //if (bEnable)
    //{
    //    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Active, NULL);
    //}
    //DMD_UNLOCK();
    return bRet;
}

static DMD_ISDBT_LOCK_STATUS _MDrv_DMD_ISDBT_CheckLock(void)
{    
    //printf("_MDrv_DMD_ISDBT_CheckLock\n");
    MS_BOOL      bCheckFecResult = FALSE;
    MS_BOOL      bCheckIcfoResult = FALSE;

    bCheckFecResult = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_FEC_Lock, NULL);

    //printf("Start time = %ld\n", sDMD_ISDBT_Info.u32ISDBTScanTimeStart);
    //printf("Time Diff= %ld\n", (MsOS_GetSystemTime() - sDMD_ISDBT_Info.u32ISDBTScanTimeStart));
    
    if (bCheckFecResult)
    {
        //sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSNR, &testtt);
        sDMD_ISDBT_Info.u32ISDBTLockStatus |= DMD_ISDBT_LOCK_FEC_LOCK;
        sDMD_ISDBT_Info.u32ISDBTFECLockTime = MsOS_GetSystemTime();
        return DMD_ISDBT_LOCK;
    }
    else
    {
        #if 1 // follow DVBT?
        if ((sDMD_ISDBT_Info.u32ISDBTLockStatus & DMD_ISDBT_LOCK_FEC_LOCK) && ((MsOS_GetSystemTime()-sDMD_ISDBT_Info.u32ISDBTFECLockTime) < 100))
        {
            return DMD_ISDBT_LOCK;
        }
        else
        {
            sDMD_ISDBT_Info.u32ISDBTLockStatus &= (~DMD_ISDBT_LOCK_FEC_LOCK);
        }
        #endif

        if (sDMD_ISDBT_Info.u32ISDBTLockStatus & DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK)
        {
            if ((MsOS_GetSystemTime()-sDMD_ISDBT_Info.u32ISDBTScanTimeStart) < _sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime)
            {
                return DMD_ISDBT_CHECKING;
            }
        }
        else
        {
            bCheckIcfoResult = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_ICFO_CH_EXIST_Lock, NULL);
            
            if (bCheckIcfoResult)
            {
                sDMD_ISDBT_Info.u32ISDBTLockStatus |= DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_ISDBT_DbgLevel >= DMD_ISDBT_DBGLV_DEBUG)
                {
                    printf("DMD_ISDBT_LOCK_VSB_PRE_LOCK\n");
                }
                #endif
                return DMD_ISDBT_CHECKING;
            }
            else
            {
                if ((MsOS_GetSystemTime()-sDMD_ISDBT_Info.u32ISDBTScanTimeStart) < _sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime)  //_sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = 300 ms
                {
                    return DMD_ISDBT_CHECKING;
                }
            }
        }
        return DMD_ISDBT_UNLOCK;
    }
}


DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_TYPE eType)
{
    DMD_LOCK();
    switch (eType)
    {
        case DMD_ISDBT_GETLOCK:
            sDMD_ISDBT_Info.eStatus = _MDrv_DMD_ISDBT_CheckLock();
            break;
        case DMD_ISDBT_GETLOCK_FSA_TRACK_LOCK:
            sDMD_ISDBT_Info.eStatus = (sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_FSA_TRACK_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_PSYNC_LOCK:
            sDMD_ISDBT_Info.eStatus = (sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_PSYNC_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_ICFO_CH_EXIST_LOCK:
            sDMD_ISDBT_Info.eStatus = (sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_ICFO_CH_EXIST_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_FEC_LOCK:
            sDMD_ISDBT_Info.eStatus = (sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Check_FEC_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;

    }
    DMD_UNLOCK();
    return sDMD_ISDBT_Info.eStatus;
}

MS_BOOL MDrv_DMD_ISDBT_GetModulationMode(EN_ISDBT_Layer LayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode)
{
    MS_BOOL  bRet = TRUE;
    s_ISDBT_GET_MODULATION   sIsdbtGetModulationMode;
    s_ISDBT_GET_TimeInterleaving sIsdbtGetTimeInterleaving;
    s_ISDBT_GET_CodeRate           sIsdbtGetCodeRate;

    sIsdbtGetModulationMode.eIsdbtLayer = LayerIndex;
    sIsdbtGetTimeInterleaving.eIsdbtLayer = LayerIndex;
    sIsdbtGetCodeRate.eIsdbtLayer = LayerIndex;
    
    DMD_LOCK();
    bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalCodeRate, &sIsdbtGetCodeRate);
    sIsdbtModulationMode->eIsdbtCodeRate = sIsdbtGetCodeRate.eCodeRate;
    bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalGuardInterval, &sIsdbtModulationMode->eIsdbtGI);
    bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalTimeInterleaving, &sIsdbtGetTimeInterleaving);
    sIsdbtModulationMode->eIsdbtTDI = sIsdbtGetTimeInterleaving.eTimeInterleaving;
    bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalFFTValue, &sIsdbtModulationMode->eIsdbtFFT);
    bRet &= sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalModulation, &sIsdbtGetModulationMode);
    sIsdbtModulationMode->eIsdbtConstellation = sIsdbtGetModulationMode.eConstellation;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_GetSignalStrength(MS_U16 *u16Strength)
{
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalStrength, (void *)u16Strength);
    DMD_UNLOCK();
    return TRUE;
}
MS_U16 MDrv_DMD_ISDBT_GetSignalQualityCombine(void)
{
    MS_U16    u16SignalQuality= 0;
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalQualityCombine, &u16SignalQuality);
    DMD_UNLOCK();
    return u16SignalQuality;	
}
MS_U16 MDrv_DMD_ISDBT_GetSignalQuality(void)
{
    MS_U16    u16SignalQuality= 0;
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalQuality, &u16SignalQuality);
    DMD_UNLOCK();
    return u16SignalQuality;
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerA(void)
{
    MS_U16    u16SignalQuality= 0;
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalQualityOfLayerA, &u16SignalQuality);
    DMD_UNLOCK();
    return u16SignalQuality;
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerB(void)
{
    MS_U16    u16SignalQuality= 0;
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalQualityOfLayerB, &u16SignalQuality);
    DMD_UNLOCK();
    return u16SignalQuality;
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerC(void)
{
    MS_U16    u16SignalQuality= 0;
    DMD_LOCK();
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSignalQualityOfLayerC, &u16SignalQuality);
    DMD_UNLOCK();
    return u16SignalQuality;
}


MS_U8 MDrv_DMD_ISDBT_GetSNRPercentage(void)
{
    return TRUE;
}

MS_BOOL MDrv_DMD_ISDBT_IIC_BYPASS_MODE(MS_BOOL    bEnable)
{
    MS_BOOL bRet = FALSE;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_IIC_Bypass_Mode, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_Read_PKT_ERR(EN_ISDBT_Layer LayerIndex, MS_U16 *u16PacketErr)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GET_PKT_ERR   sIsdbtGetPktErr;
    DMD_LOCK();
    sIsdbtGetPktErr.eIsdbtLayer = LayerIndex;
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_Read_PKT_ERR, &sIsdbtGetPktErr);
    *u16PacketErr = sIsdbtGetPktErr.u16PacketErr;
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_DMD_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL   bEnable)
{
    MS_BOOL bRet = FALSE;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_SWITCH_SSPI_GPIO, &bEnable);
    DMD_UNLOCK();

    return bRet;

}
MS_BOOL MDrv_DMD_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GPIO_INTERFACE  sIsdbtGpioInterface;
    sIsdbtGpioInterface.u8Pin = u8Pin;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GPIO_GET_LEVEL, &sIsdbtGpioInterface);
    *bLevel = sIsdbtGpioInterface.bLevel;
    DMD_UNLOCK();

    return bRet;

}


MS_BOOL MDrv_DMD_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GPIO_INTERFACE  sIsdbtGpioInterface;
    sIsdbtGpioInterface.u8Pin = u8Pin;
    sIsdbtGpioInterface.bLevel = bLevel;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GPIO_SET_LEVEL, &sIsdbtGpioInterface);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_DMD_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GPIO_INTERFACE  sIsdbtGpioInterface;
    sIsdbtGpioInterface.u8Pin = u8Pin;
    sIsdbtGpioInterface.bLevel = bEnableOut;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GPIO_OUT_ENABLE, &sIsdbtGpioInterface);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_DMD_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GET_BER_VALUE   sIsdbtGetBerValue;
    sIsdbtGetBerValue.eIsdbtLayer = LayerIndex;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetPostViterbiBer, &sIsdbtGetBerValue);
    *ber = sIsdbtGetBerValue.fBerValue;
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber)
{
    MS_BOOL bRet = FALSE;
    s_ISDBT_GET_BER_VALUE   sIsdbtGetBerValue;
    sIsdbtGetBerValue.eIsdbtLayer = LayerIndex;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetPreViterbiBer, &sIsdbtGetBerValue);
    *ber = sIsdbtGetBerValue.fBerValue;
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_GetFreqOffset(float *pFreqOff)
{
    MS_BOOL bRet = FALSE;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetFreqOffset, (void *)pFreqOff);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_GetSNR(float *pf_snr)
{
    MS_BOOL bRet = FALSE;
    DMD_LOCK();
    bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetSNR, (void *)pf_snr);
    DMD_UNLOCK();

    return bRet;
}

MS_U32 MDrv_DMD_ISDBT_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static MS_BOOL bIs_DTV_Mode=0;
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    if (u16PowerState == E_POWER_SUSPEND)
    {
        //DMD_LOCK();
        bIs_DTV_Mode=sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_CHECK_FW_STATUS,NULL);
        sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_TURN_ON_DOWNLOAD_FW_FLOW, NULL);
        //DMD_UNLOCK();
        MDrv_DMD_ISDBT_Exit();

        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            printf("\nVT: (Check Mode In DRV:) ATV Mode=%d\n",bIs_DTV_Mode);   
        	                      
            if (bIs_DTV_Mode)
            {
                MDrv_DMD_ISDBT_Init(&_sDMD_ISDBT_InitData, sizeof(_sDMD_ISDBT_InitData));
                MDrv_DMD_ISDBT_SetConfig(TRUE);
            }
        
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
    u32Return=u32Return;
    return UTOPIA_STATUS_SUCCESS;
}
