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
//        including without limitation, you and yor any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitra«–cordance
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
#include "utopia.h"   //dan@20130502 merge STR function from utopia_fastboot
// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"

#include "drvDMD_VD_MBX.h"
#include "drvDMD_common.h"
#include "drvDMD_DTMB.h"
//#include "drvDMD_INTERN_DTMB.h"
//#include "halDMD_INTERN_DTMB.h"

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
        if (_s32DMD_DTMB_Mutex == -1) return FALSE; \
        if (_u8DMD_DTMB_DbgLevel == DMD_DTMB_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DTMB_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DTMB_Mutex);\
        if (_u8DMD_DTMB_DbgLevel == DMD_DTMB_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

s_DTMB_HAL_FUNCTION_TABLE    sDTMB_HAL_FUNCTION_TABLE = {0};
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 0
static MSIF_Version _drv_dmd_dvbt_intern_version = {
    .MW = { DMD_DVBT_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_DTMB_intern_version;
#endif

static DMD_DTMB_InitData _sDMD_DTMB_InitData;
static DMD_DTMB_DbgLv _u8DMD_DTMB_DbgLevel=DMD_DTMB_DBGLV_NONE;
static MS_S32 _s32DMD_DTMB_Mutex=-1;
static DMD_DTMB_Info sDMD_DTMB_Info;


MS_BOOL bDMD_DTMB_TUNER_GAIN_INVERT;//TSB Sony Tuner set 1.
MS_U16  u16DMD_DTMB_TUNER_IF_KHZ;
MS_U8    u8DMD_DTMB_TS_CONFIG;
MS_BOOL bDMD_DTMB_TUNER_IS_IQ_SWAP;
MS_U32  u32DMD_DTMB_TDI_START_ADDR;
MS_BOOL bDMD_DTMB_TS_IS_SERIAL_MODE;
MS_U16  u16DMD_DTMB_AGC_REFERENCE;
MS_BOOL  bDMD_DTMB_IS_DUAL;
static DMD_DTMB_DEMOD_TYPE  _eLAST_DTMB_DEMOD_TYPE = DMD_DTMB_DEMOD_NULL; //dan@20130502 merge STR function from utopia_fastboot
MS_BOOL bDMD_DTMB_LOAD_CODE_BY_SSPI;
MS_BOOL bDMD_DTMB_SSPI_USE_TS_PIN;


extern MS_U8    u8DMD_I2C_SLAVE_ADDR;
extern MS_U8    u8DMD_I2C_SLAVE_BUS;
extern MS_BOOL  bIS_EXTERN_DMD;//From ...\utopia\mxlib\drv\demod\drvDMD_common.c
extern s_I2C_Interface_func sI2cInterfaceFunc;

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
MS_BOOL MDrv_DMD_DTMB_Init(DMD_DTMB_InitData *pDMD_DTMB_InitData, MS_U32 u32InitDataLen)
{
    MS_BOOL bRFAGCTristateEnable;
    if (_s32DMD_DTMB_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init more than once\n"));
        return FALSE;
    }

    if ((_s32DMD_DTMB_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD DTMB", MSOS_PROCESS_SHARED)) == -1)
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init Create Mutex Fail\n"));
        return FALSE;
    }


    //_u8DMD_DTMB_DbgLevel=DMD_DTMB_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_INFO)
    {
        printf("MDrv_DMD_DTMB_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DTMB_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DTMB_InitData, pDMD_DTMB_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init input data structure incorrect\n"));
        return FALSE;
    }

    //MDrv_SYS_DMD_VD_MBX_Init();
    HAL_DMD_RegInit();

    if (_sDMD_DTMB_InitData.u8DMD_DTMB_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_DTMB_InitData.u8DMD_DTMB_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 0;//1;
    }

    bRFAGCTristateEnable = bRFAGCTristateEnable;


   // u16DMD_ISDBT_AGC_REF_VALUE = _sDMD_ISDBT_InitData.u16AgcReferenceValue;
    u16DMD_DTMB_TUNER_IF_KHZ   = _sDMD_DTMB_InitData.u16IF_KHZ;
    bDMD_DTMB_TUNER_IS_IQ_SWAP = _sDMD_DTMB_InitData.bIQSwap;
    u16DMD_DTMB_AGC_REFERENCE = _sDMD_DTMB_InitData.u16AGC_REFERENCE;
    u32DMD_DTMB_TDI_START_ADDR= _sDMD_DTMB_InitData.u32TdiStartAddr;
    bDMD_DTMB_IS_DUAL = _sDMD_DTMB_InitData.u8IS_DUAL;
    bDMD_DTMB_TUNER_GAIN_INVERT = _sDMD_DTMB_InitData.bTunerGainInvert;
    printf("IF FREQ %d\n",u16DMD_DTMB_TUNER_IF_KHZ);
  //  u32DMD_ISDBT_TDI_START_ADDR= _sDMD_ISDBT_InitData.u32TdiStartAddr;

  //NEW ADD for EXT DEMOD
   u8DMD_DTMB_TS_CONFIG = (_sDMD_DTMB_InitData.u5TsConfigByte_DivNum << 3) |
                                                  (_sDMD_DTMB_InitData.u1TsConfigByte_ClockInv << 2) |
                                                  (_sDMD_DTMB_InitData.u1TsConfigByte_DataSwap << 1) |
                                                  (_sDMD_DTMB_InitData.u1TsConfigByte_SerialMode);

    bDMD_DTMB_TS_IS_SERIAL_MODE = _sDMD_DTMB_InitData.u1TsConfigByte_SerialMode;
    u8DMD_I2C_SLAVE_ADDR = _sDMD_DTMB_InitData.u8I2CSlaveAddr;
    u8DMD_I2C_SLAVE_BUS = _sDMD_DTMB_InitData.u8I2CSlaveBus;
    bIS_EXTERN_DMD = _sDMD_DTMB_InitData.bIsExtDemod;
    bDMD_DTMB_LOAD_CODE_BY_SSPI = _sDMD_DTMB_InitData.bIsUseSspiLoadCode;
    bDMD_DTMB_SSPI_USE_TS_PIN   = _sDMD_DTMB_InitData.bIsSspiUseTsPin;
    
    sI2cInterfaceFunc.I2C_WriteBytes = _sDMD_DTMB_InitData.I2C_WriteBytes;
    sI2cInterfaceFunc.I2C_ReadBytes = _sDMD_DTMB_InitData.I2C_ReadBytes;

    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);

    DMD_LOCK();

    //#ifdef CHIP_KAISER
    //MDrv_DMD_DTMB_Initial_Hal_Interface();
    //#endif

    if(sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD == NULL)
    {
        DMD_UNLOCK();
        printf("HAL_DMD_DTMB_IOCTL_CMD doesn't initiate!!!\n");
        return FALSE;
    }
    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DTMB);
    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);
    if(bIS_EXTERN_DMD == FALSE)
    {
        //MDrv_DMD_DTMB_InitClk();//
        sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_InitClk,&bRFAGCTristateEnable/*NULL*/);
    }
    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);

    #ifndef CHIP_KAISER
    {
    //printf("if in K3 dual DMD, wrong func of download!!!\n");
    //MDrv_DMD_DTMB_DownLoad();//
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Download, NULL);
    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);

    //MDrv_DMD_DTMB_FWVERSION();//
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_FWVERSION, NULL);

    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);

    }
    #endif

    DMD_UNLOCK();
    printf("%s, %d, %s\n",__FUNCTION__,__LINE__,__FILE__);

    return TRUE;
}


MS_BOOL MDrv_DMD_DTMB_Exit(void)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();
    // Kenji HAL_INTERN_DTMB_Exit();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Exit, NULL);// no do in HAL
    bRet=bRet;
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DTMB_Mutex);
    _s32DMD_DTMB_Mutex= -1;
    return TRUE;
}

MS_BOOL MDrv_DMD_DTMB_SetDbgLevel(DMD_DTMB_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMD_DTMB_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

DMD_DTMB_Info* MDrv_DMD_DTMB_GetInfo(void)
{
    sDMD_DTMB_Info.u8Version=0;
    return &sDMD_DTMB_Info;
}

MS_BOOL MDrv_DMD_DTMB_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_DTMB_intern_version;
    DMD_UNLOCK();
    return TRUE;
}


MS_BOOL MDrv_DMD_DTMB_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_DMD_ReadReg(u16Addr, pu8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_DMD_WriteReg(u16Addr, u8Data); // Kenji
    DMD_UNLOCK();
    return bRet;
}



//MS_BOOL MDrv_DMD_DTMB_SetSerialControl(MS_BOOL bEnable)
MS_BOOL MDrv_DMD_DTMB_SetSerialControl(MS_U8 u8TsConfigData)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_TS_INTERFACE_CONFIG, &u8TsConfigData);
    bRet=bRet;
    DMD_UNLOCK();
    return TRUE;
}


MS_BOOL MDrv_DMD_DTMB_SetReset(void)//
{
    MS_BOOL ret;
    DMD_LOCK();
    ret=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SoftReset, NULL);
    DMD_UNLOCK();

    return ret;
}

MS_BOOL MDrv_DMD_DTMB_SetConfig(DMD_DTMB_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    MS_BOOL bRet=true;
    MS_BOOL bRFAGCTristateEnable;

    if (_sDMD_DTMB_InitData.u8DMD_DTMB_InitExt != NULL)
    {
        bRFAGCTristateEnable = *(_sDMD_DTMB_InitData.u8DMD_DTMB_InitExt);
    }
    else
    {
        bRFAGCTristateEnable = 1;
    }
    bRFAGCTristateEnable=bRFAGCTristateEnable;
    DMD_LOCK();

    //MDrv_DMD_DTMB_SetReset();//
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_InitClk,&bRFAGCTristateEnable/*NULL*/);
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SoftReset, NULL);

    //HAL_INTERN_DTMB_InitClk(bRFAGCTristateEnable);
    //HAL_INTERN_DTMB_Download();
    //MDrv_DMD_DTMB_SetReset();//HAL_INTERN_DTMB_SoftReset();

    if (bEnable)
    {
        switch (eType)
        {
            case DMD_DTMB_DEMOD_DTMB:
                bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetDtmbMode, NULL);//MDrv_DMD_DTMB_SetDtmbMode();//_HAL_EXTERN_DTMB_SetDtmbMode();
                break;
            case DMD_DTMB_DEMOD_DVBC:
                bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetDvbcMode, NULL);//MDrv_DMD_DTMB_SetDvbcMode();//_HAL_EXTERN_DTMB_SetDvbcMode();
                break;
            default:
                bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetModeClean, NULL);//MDrv_DMD_DTMB_SetModeClean();//HAL_INTERN_DTMB_SetModeClean();
                break;
        }
    }

    sDMD_DTMB_Info.u32DTMBScanTimeStart=MsOS_GetSystemTime();
    sDMD_DTMB_Info.eStatus=DMD_DTMB_CHECKING;
    sDMD_DTMB_Info.u32DTMBLockStatus=0;
    _eLAST_DTMB_DEMOD_TYPE = eType;    //dan@20130502 merge STR function from utopia_fastboot
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_SetActive(MS_BOOL bEnable)
{
    return TRUE;
}

static DMD_DTMB_LOCK_STATUS _MDrv_DMD_DTMB_CheckLock(void)
{
    MS_BOOL      bCheckFecResult = FALSE;
    MS_BOOL      bCheckPreResult = FALSE;
    MS_BOOL      bCheckFsynResult = FALSE;

	bCheckFecResult = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_FEC_Lock, NULL);//MDrv_DMD_DTMB_Vsb_FEC_Lock();

   //printf("Start time = %ld\n", sDMD_ISDBT_Info.u32ISDBTScanTimeStart);
   //printf("_MDrv_DMD_DTMB_CheckLock\n");

    if (bCheckFecResult)//(HAL_INTERN_DTMB_Vsb_FEC_Lock())
    {
        sDMD_DTMB_Info.u32DTMBLockStatus |= DMD_DTMB_LOCK_DTMB_FEC_LOCK;
        sDMD_DTMB_Info.u32DTMBFECLockTime = MsOS_GetSystemTime();
        return DMD_DTMB_LOCK;
    }
    else//FEC UNLOCK
    {
        #if 1 // follow DVBT?// FEC Locking Time??
        if ((sDMD_DTMB_Info.u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_FEC_LOCK) && ((MsOS_GetSystemTime()-sDMD_DTMB_Info.u32DTMBFECLockTime) < 100))
        {
            return DMD_DTMB_LOCK;
        }
        else//Step 0 or 5
        {
            sDMD_DTMB_Info.u32DTMBLockStatus &= (~DMD_DTMB_LOCK_DTMB_FEC_LOCK);
        }
        #endif

        if (sDMD_DTMB_Info.u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_PRE_LOCK)//Step 3,4
        {
            if (sDMD_DTMB_Info.u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_PNM_LOCK)//Step 4
            {
                if ((MsOS_GetSystemTime()-sDMD_DTMB_Info.u32DTMBScanTimeStart) < _sDMD_DTMB_InitData.u16DTMBFECLockCheckTime)
                {
                    return DMD_DTMB_CHECKING;
                }
            }
            else//Step 3
            {
            	  bCheckFsynResult=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL);//MDrv_DMD_DTMB_FSync_Lock();
                if (bCheckFsynResult)//(HAL_INTERN_DTMB_Vsb_FSync_Lock())
                {
                    sDMD_DTMB_Info.u32DTMBLockStatus |= DMD_DTMB_LOCK_DTMB_PNM_LOCK;
                    //printf("DMD_DTMB_LOCK_DTMB_PNM_LOCK\n");
                    #ifdef MS_DEBUG
                    if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_DEBUG)
                    {
                    }
                    #endif
                    return DMD_DTMB_CHECKING;
                }
                else
                {
                	  //printf("aa: %ld %ld\n",MsOS_GetSystemTime(),sDMD_DTMB_Info.u32DTMBScanTimeStart);
                    if ((MsOS_GetSystemTime()-sDMD_DTMB_Info.u32DTMBScanTimeStart) < _sDMD_DTMB_InitData.u16DTMBPNMLockCheckTime)
                    {
                        return DMD_DTMB_CHECKING;
                    }
                }
            }
        }
        else//Step 1,2
        {    bCheckPreResult=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_PreLock, NULL);
            if (bCheckPreResult)//(HAL_INTERN_DTMB_Vsb_PreLock())
            {
                sDMD_DTMB_Info.u32DTMBLockStatus |= DMD_DTMB_LOCK_DTMB_PRE_LOCK;
                //printf("DMD_DTMB_LOCK_DTMB_PRE_LOCK\n");
                #ifdef MS_DEBUG
                if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_DEBUG)
                {
                    printf("DMD_DTMB_LOCK_DTMB_PRE_LOCK\n");
                }
                #endif
                return DMD_DTMB_CHECKING;
            }
            else
            {
                if ((MsOS_GetSystemTime()-sDMD_DTMB_Info.u32DTMBScanTimeStart) < _sDMD_DTMB_InitData.u16DTMBPreLockCheckTime)
                {
                    return DMD_DTMB_CHECKING;
                }
            }
        }
        return DMD_DTMB_UNLOCK;
    }
}



DMD_DTMB_LOCK_STATUS MDrv_DMD_DTMB_GetLock(DMD_DTMB_GETLOCK_TYPE eType)
{

    DMD_LOCK();
    switch (eType)
    {
        case DMD_DTMB_GETLOCK:
            //Temporarily hard code. DTMB should only use DTMB Demod.
            //printf("\n DMD_DTMB_GETLOCK: [%s,%s,%5d]\n",__FILE__,__FUNCTION__,__LINE__);

                    //printf("sDMD_DTMB_Info.eStatus =%d \r\n",sDMD_DTMB_Info.eStatus);
                    sDMD_DTMB_Info.eStatus = _MDrv_DMD_DTMB_CheckLock();
           //printf("\n[%s,%s,%5d]\n",__FILE__,__FUNCTION__,__LINE__);

            break;
        case DMD_DTMB_GETLOCK_VSB_AGCLOCK:
            //return (HAL_INTERN_DTMB_Vsb_QAM_AGCLock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_QAM_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_QAM_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_VSB_PRELOCK:
            //return (HAL_INTERN_DTMB_Vsb_PreLock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_PreLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus =(sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_PreLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_VSB_FSYNCLOCK:
        	   //printf("\n DMD_DTMB_GETLOCK_VSB_FSYNCLOCK [%s,%s,%5d]\n",__FILE__,__FUNCTION__,__LINE__);
        	   //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL))?DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
        	   sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL))?DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
        	   break;
        case DMD_DTMB_GETLOCK_VSB_CELOCK:
            //return (HAL_INTERN_DTMB_Vsb_FSync_Lock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL))?DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL))?DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_VSB_FECLOCK:
        	   //printf("\n DMD_DTMB_GETLOCK_VSB_FECLOCK: [%s,%s,%5d]\n",__FILE__,__FUNCTION__,__LINE__);
            //return (HAL_INTERN_DTMB_Vsb_FEC_Lock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_FEC_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_FEC_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_QAM_AGCLOCK:
            //return (HAL_INTERN_DTMB_Vsb_QAM_AGCLock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_QAM_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_QAM_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_QAM_PRELOCK:
            //return (HAL_INTERN_DTMB_QAM_PreLock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            //return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_PreLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_PreLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_QAM_MAINLOCK:
            //return (HAL_INTERN_DTMB_QAM_Main_Lock()) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
           // return (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_Main_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            sDMD_DTMB_Info.eStatus = (sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_Main_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
    }
    DMD_UNLOCK();
    return sDMD_DTMB_Info.eStatus;
}


//======================VT Modify for Galileo================================//

#if 1

DMD_DTMB_DEMOD_TYPE MDrv_DMD_DTMB_GetModulationMode(void)
{
    MS_BOOL bRet=TRUE;
    DMD_DTMB_DEMOD_TYPE eType;
    //DMD_LOCK();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Check8VSB64_256QAM, &eType);
    bRet=bRet;
    //DMD_UNLOCK();
    return eType;
}




MS_U16 MDrv_DMD_DTMB_ReadPKTERR(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16    u16ReadPKTERR= 0;
    DMD_LOCK();
    bRet= sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ReadPKTERR, &u16ReadPKTERR);
    bRet=bRet;
    DMD_UNLOCK();
    return u16ReadPKTERR;
}

MS_U8 MDrv_DMD_DTMB_GetSNRPercentage(void)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8GetSNRPercentage;
    DMD_LOCK();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ReadSNRPercentage, &u8GetSNRPercentage);
    bRet=bRet;
    DMD_UNLOCK();
    return u8GetSNRPercentage;
}





MS_U8 MDrv_DMD_DTMB_GetSignalQuality(void)
{
    MS_BOOL bRet=TRUE;
    MS_U8 quality;
    DMD_LOCK();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_CheckSignalCondition, &quality);
    bRet=bRet;
    DMD_UNLOCK();
    return quality;
}

MS_BOOL MDrv_DMD_DTMB_QAM_Main_Lock(void)//
{
    MS_BOOL mainlock;
    DMD_LOCK();
    mainlock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_Main_Lock, NULL);
    DMD_UNLOCK();
    return mainlock;
}


MS_BOOL MDrv_DMD_DTMB_QAM_PreLock(void)//
{
    MS_BOOL prelock;
    DMD_LOCK();
    prelock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_QAM_PreLock, NULL);
    DMD_UNLOCK();
    return prelock;
}

//

MS_U16 MDrv_DMD_DTMB_ReadFrequencyOffset(void)
{
	  MS_BOOL bRet=TRUE;
    MS_U16    u16ReadFreqOffset= 0;
    DMD_LOCK();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ReadFrequencyOffset, &u16ReadFreqOffset);
    bRet=bRet;
    DMD_UNLOCK();
    return u16ReadFreqOffset;
}

//
MS_BOOL MDrv_DMD_DTMB_GetSignalStrength(MS_U16 *u16Strength)
{
    //MS_U16    u16ReadIFAGC= 0;
    MS_BOOL bRet=TRUE;
    DMD_LOCK();
    bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ReadIFAGC, u16Strength);
    DMD_UNLOCK();
    return bRet;// u16ReadIFAGC;
}


MS_BOOL MDrv_DMD_DTMB_Set256QamMode(void)//
{
    MS_BOOL ret;
    DMD_LOCK();
    ret=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Set256QamMode, NULL);
    DMD_UNLOCK();
    return ret;
}

//
MS_BOOL MDrv_DMD_DTMB_SetDvbcMode(void)//
{
    MS_BOOL ret;
    DMD_LOCK();
    ret=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetDvbcMode, NULL);
    DMD_UNLOCK();
    return ret;
}

//
MS_BOOL MDrv_DMD_DTMB_SetModeClean(void)//
{
    MS_BOOL ret;
    DMD_LOCK();
    ret=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetModeClean, NULL);
    DMD_UNLOCK();
    return ret;
}

//
MS_BOOL MDrv_DMD_DTMB_SetDtmbMode(void)//
{
    MS_BOOL ret;
    DMD_LOCK();
    ret=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SetDtmbMode, NULL);
    DMD_UNLOCK();
    return ret;
}

//


//
MS_BOOL MDrv_DMD_DTMB_Vsb_CE_Lock(void)//
{
    MS_BOOL celock;
    DMD_LOCK();
    celock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_CE_Lock, NULL);
    DMD_UNLOCK();
    return celock;
}

//
MS_BOOL MDrv_DMD_DTMB_Vsb_FEC_Lock(void)
{
    MS_BOOL feclock;
    DMD_LOCK();
    feclock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_FEC_Lock, NULL);
    DMD_UNLOCK();
    return feclock;
}

//
MS_BOOL MDrv_DMD_DTMB_FSync_Lock(void)
{
    MS_BOOL fsynclock;
    DMD_LOCK();
    fsynclock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_PNM_Lock, NULL);
    DMD_UNLOCK();
    return fsynclock;
}

//
MS_BOOL MDrv_DMD_DTMB_Vsb_PreLock(void)
{
    MS_BOOL vsbpreclock;
    DMD_LOCK();
    vsbpreclock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Dtmb_PreLock, NULL);
    DMD_UNLOCK();
    return vsbpreclock;
}

//
MS_BOOL MDrv_DMD_DTMB_Vsb_QAM_AGCLock(void)
{
    MS_BOOL agclock;
    DMD_LOCK();
    agclock=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Vsb_QAM_AGCLock, NULL);
    DMD_UNLOCK();
    return agclock;
}

MS_BOOL MDrv_DMD_DTMB_IIC_BYPASS_MODE(MS_BOOL    bEnable)
{
    MS_BOOL bRet = FALSE;
    DMD_LOCK();
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_IIC_Bypass_Mode, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_GetPostViterbiBer(float *ber)
{
    MS_BOOL bRet = FALSE;
    //s_ISDBT_GET_BER_VALUE   sIsdbtGetBerValue;
    //sIsdbtGetBerValue.eIsdbtLayer = LayerIndex;
    DMD_LOCK();
    //bRet = sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD(CMD_ISDBT_GetPostViterbiBer, &sIsdbtGetBerValue);
    //*ber = sIsdbtGetBerValue.fBerValue;
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GetPostViterbiBer, ber);
    //*ber = 0;
    DMD_UNLOCK();

    bRet=TRUE;
    return bRet;
}
MS_BOOL MDrv_DMD_DTMB_CHECK_CURRENT_LOADING(void)
{
  //CMD_DTMB_CHECK_CURRENT_LOADING
  MS_BOOL bRet = FALSE;

  DMD_LOCK();

  bRet=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_CHECK_CURRENT_LOADING, NULL);
  bRet=bRet;
  DMD_UNLOCK();

	return bRet;// Normal Case, without overloading, return true;
}


#endif
//=====================VT End For Galileo===================================//
MS_BOOL MDrv_DMD_DTMB_SEL_DMD(MS_U8 DMD_NUM)
{
    MS_BOOL bRet;
    DMD_LOCK();
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SELDMD, &DMD_NUM);
    DMD_UNLOCK();
    return bRet;
}
MS_U8 MDrv_DMD_DTMB_GET_DMD_NUM(void)
{
    MS_U8 eDMD_NUM;
    DMD_LOCK();
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GETDMD_NUM, &eDMD_NUM);
    DMD_UNLOCK();
    return eDMD_NUM;
}
MS_BOOL MDrv_DMD_DTMB_Read_uCPKT_ERR(MS_U16 *u16PacketErr)
{
    MS_BOOL bRet;
    DMD_LOCK();
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ReadPKTERR,u16PacketErr);
    DMD_UNLOCK();
    return bRet;
}
MS_BOOL MDrv_DMD_DTMB_LoadFW(MS_U8 DMD_NUM)
{
    DMD_LOCK();
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_TOPControl,&DMD_NUM);
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_Download,&DMD_NUM);
    DMD_UNLOCK();
    return TRUE;
}
MS_BOOL MDrv_DMD_DTMB_ADCOut_CTL(MS_U8 DMD_NUM)
{
    DMD_LOCK();
    sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_ADCOut_CTL, &DMD_NUM);
    DMD_UNLOCK();
    return TRUE;
}
#ifdef USE_UTOPIA2P0
//dan@20130502 merge STR function from utopia_fastboot[begin]
MS_U32 MDrv_DMD_DTMB_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static MS_BOOL bIs_DTV_Mode=0;
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    printf("[Kavana]enter MDrv_DMD_DTMB_SetPowerState!!\n");
    printf("[Kavana]enter MDrv_DMD_DTMB_SetPowerState u16PowerState = %d!!\n", u16PowerState);
    if (u16PowerState == E_POWER_SUSPEND)
    {
        //DMD_LOCK();
        bIs_DTV_Mode=sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_CHECK_FW_STATUS,NULL);
        sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_TURN_ON_DOWNLOAD_FW_FLOW, NULL);
        //DMD_UNLOCK();
        MDrv_DMD_DTMB_Exit();
        
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
            MDrv_DMD_DTMB_Init(&_sDMD_DTMB_InitData, sizeof(_sDMD_DTMB_InitData));
            MDrv_DMD_DTMB_SetConfig(_eLAST_DTMB_DEMOD_TYPE, TRUE);
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
//dan@20130502 merge STR function from utopia_fastboot[end]
#endif

MS_BOOL MDrv_DMD_DTMB_SWITCH_SSPI_GPIO(MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_SW_SSPI_GPIO, &bEnable);
    
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    MS_BOOL bRet = TRUE;
    s_DTMB_GPIO_INTERFACE sGpioInterface;
    
    sGpioInterface.u8Pin = u8Pin;
    
    DMD_LOCK();
    
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GPIO_GET_LEVEL, &sGpioInterface);
    
    DMD_UNLOCK();
    
    *bLevel = sGpioInterface.bLevel;
    
    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    MS_BOOL bRet = TRUE;
    s_DTMB_GPIO_INTERFACE sGpioInterface;
    
    sGpioInterface.u8Pin  = u8Pin;
    sGpioInterface.bLevel = bLevel;
    
    DMD_LOCK();
    
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GPIO_SET_LEVEL, &sGpioInterface);
    
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DTMB_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    MS_BOOL bRet = TRUE;
    s_DTMB_GPIO_INTERFACE sGpioInterface;
    
    sGpioInterface.u8Pin = u8Pin;
    sGpioInterface.bLevel = bEnableOut;
    
    DMD_LOCK();
    
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GPIO_OUT_ENABLE, &sGpioInterface);
    
    DMD_UNLOCK();

    return bRet;
}
MS_BOOL MDrv_DMD_DTMB_GetPreLdpcBer(float *pber)
{
    MS_BOOL bRet = TRUE;
    DMD_LOCK();
    bRet = sDTMB_HAL_FUNCTION_TABLE.HAL_DMD_DTMB_IOCTL_CMD(CMD_DTMB_GET_PRELDPCBER, pber);
    DMD_UNLOCK();
    return bRet;
}
