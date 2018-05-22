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
#include "drvDMD_INTERN_DVBC.h"
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
#include "halDMD_INTERN_DVBC.h"
#include "halDMD_INTERN_common.h"
#include "../../include/drvSAR.h"
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
        if (_s32DMD_DVBC_Mutex == -1) return FALSE; \
        if (_u8DMD_DVBC_DbgLevel == DMD_DVBC_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBC_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBC_Mutex);\
        if (_u8DMD_DVBC_DbgLevel == DMD_DVBC_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

MS_U8		 DVBC_TS_PHASE_EN =0;
MS_U8		 DVBC_TS_PHASE_NUM = 0; 
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_dvbc_intern_version = {
    .MW = { DMD_DVBC_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbc_intern_version;
#endif
static const char pDMD_DVBC_MutexString[]={"Mutex DMD DVBC"};
static DMD_DVBC_InitData _sDMD_DVBC_InitData;
static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime=0, u32DMD_DVBC_ScanCount=0;

#ifdef CHIP_KAISER

//for dual demod setting

//demod NO0
static DMD_DVBC_InitData _sDMD_DVBC_InitData_dmd0;
//static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
//static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info_dmd0;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout_dmd0 = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime_dmd0=0, u32DMD_DVBC_ScanCount_dmd0=0;

//demod NO1
static DMD_DVBC_InitData _sDMD_DVBC_InitData_dmd1;
//static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
//static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info_dmd1;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout_dmd1 = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime_dmd1=0, u32DMD_DVBC_ScanCount_dmd1=0;

static MS_U8 drv_demod_swtich_status=0xff;  //

#endif

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
#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
{
    char pDMD_DVBC_MutexString[16];
   // MS_U32 u32IfFrequency = 36167L, u32FsFrequency = 45474L;
   // MS_U8 u8IQSwap=0, u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;
    
    if (_s32DMD_DVBC_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init more than once\n"));
        return FALSE;
    }
    
    if (NULL == strncpy(pDMD_DVBC_MutexString,"Mutex DMD DVBC",16))
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init strcpy Fail\n"));
        return FALSE;    
    }
    _s32DMD_DVBC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBC_MutexString, MSOS_PROCESS_SHARED);
    if ( _s32DMD_DVBC_Mutex == -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init Create Mutex Fail\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_Init\n");
    }
    #endif

    if (u8Sar_Channel != 0xFF)
    {
        MDrv_SAR_Adc_Config(u8Sar_Channel, TRUE);
    }
    DMD_LOCK();
    //_u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_DEBUG;
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBC);
    HAL_DMD_RegInit();


     bRFAGCTristateEnable = (u8AGC_Tristate_Ctrl& (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
     bIFAGCTristateEnable = (u8AGC_Tristate_Ctrl & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control

    
    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }
   
    INTERN_DVBC_InitClkgen(bRFAGCTristateEnable);  //RF_AGC control no use in initclkgen
    DMD_UNLOCK();

    return TRUE;
}


MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{  
     DMD_LOCK();
     
    if ( sizeof(_sDMD_DVBC_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init input data structure incorrect\n"));
        return FALSE;
    }

    u16DMD_DVBC_AutoSymbol_Timeout = 10000;
    u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
    u16DMD_DVBC_FixSymbol_FixQam_Timeout=2000;
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=4) 
        {
            u16DMD_DVBC_AutoSymbol_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3]; 
            u16DMD_DVBC_AutoSymbol_Timeout =  (u16DMD_DVBC_AutoSymbol_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[4];           
            if (u16DMD_DVBC_AutoSymbol_Timeout < 5000) u16DMD_DVBC_AutoSymbol_Timeout=5000;
            //printf("u16DMD_DVBC_AutoSymbol_Timeout %d\n",u16DMD_DVBC_AutoSymbol_Timeout);
            
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[5];
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout =  (u16DMD_DVBC_FixSymbol_AutoQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[6];           
            if (u16DMD_DVBC_FixSymbol_AutoQam_Timeout < 2000) u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
            //printf("u16DMD_DVBC_FixSymbol_AutoQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_AutoQam_Timeout);            

            u16DMD_DVBC_FixSymbol_FixQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[7];
            u16DMD_DVBC_FixSymbol_FixQam_Timeout =  (u16DMD_DVBC_FixSymbol_FixQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[8];           
            if (u16DMD_DVBC_FixSymbol_FixQam_Timeout < 1000) u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
            //printf("u16DMD_DVBC_FixSymbol_FixQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_FixQam_Timeout);            
        }
        else
        {
        }
    }
    else
    {
    }


    
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBC_DMD51_Individual_Initialization(_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize);
        }
        else
        {
            printf("u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBC_DMD51_Individual_Initialization(  NULL, 0);
    }

    INTERN_DVBC_Version(&sDMD_DVBC_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    printf("firmware version: %x\n",sDMD_DVBC_Info.u16Version);
    #endif
    return TRUE;
}
#else
	MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
	{
				printf("Doesn't support DVBC_Dual_Individual_Init function!!!\n");
				return false;
	}

	MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
	{
				printf("Doesn't support DVBC_Dual_Public_Init function!!!\n");
				return false;
	}
#endif
MS_BOOL MDrv_DMD_DVBC_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBC_MutexString[16];
    MS_U32 u32IfFrequency = 36167L, u32FsFrequency = 45474L;
    MS_U8 u8IQSwap = 0, u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    if (_s32DMD_DVBC_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBC_MutexString,"Mutex DMD DVBC",16))
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBC_MutexString, MSOS_PROCESS_SHARED);
    if ( _s32DMD_DVBC_Mutex == -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init Create Mutex Fail\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DVBC_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DVBC_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
        MDrv_SAR_Adc_Config(_sDMD_DVBC_InitData.u8SarChannel, TRUE);
    }
    DMD_LOCK();
    //_u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_DEBUG;
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBC);
    HAL_DMD_RegInit();

    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
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

    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=3)
        {
            u32IfFrequency = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[4]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[5]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[6]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[7]; // IF frequency
            u32FsFrequency = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[8]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[9]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[10]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[11]; // FS frequency
            u8IQSwap = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[12]; // IQ Swap
			u8IQSwap = u8IQSwap;
            u8ADCIQMode = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }

    u16DMD_DVBC_AutoSymbol_Timeout = 10000;
    u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
    u16DMD_DVBC_FixSymbol_FixQam_Timeout=2000;
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=4)
        {
            u16DMD_DVBC_AutoSymbol_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[17];
            u16DMD_DVBC_AutoSymbol_Timeout =  (u16DMD_DVBC_AutoSymbol_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[18];
            if (u16DMD_DVBC_AutoSymbol_Timeout < 5000) u16DMD_DVBC_AutoSymbol_Timeout=5000;
            //printf("u16DMD_DVBC_AutoSymbol_Timeout %d\n",u16DMD_DVBC_AutoSymbol_Timeout);

            u16DMD_DVBC_FixSymbol_AutoQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[19];
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout =  (u16DMD_DVBC_FixSymbol_AutoQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[20];
            if (u16DMD_DVBC_FixSymbol_AutoQam_Timeout < 2000) u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
            //printf("u16DMD_DVBC_FixSymbol_AutoQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_AutoQam_Timeout);

            u16DMD_DVBC_FixSymbol_FixQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[21];
            u16DMD_DVBC_FixSymbol_FixQam_Timeout =  (u16DMD_DVBC_FixSymbol_FixQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[22];
            if (u16DMD_DVBC_FixSymbol_FixQam_Timeout < 1000) u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
            //printf("u16DMD_DVBC_FixSymbol_FixQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_FixQam_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    #ifdef MS_DEBUG
    printf("u32IfFrequency %ld\n",u32IfFrequency);
    printf("u32FsFrequency %ld\n",u32FsFrequency);
    printf("u8IQSwap %d\n",u8IQSwap);
    #endif

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }

	// oga
	 DVBC_TS_PHASE_EN = 0;
	 DVBC_TS_PHASE_NUM = 0; 	
   if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
   {
   	if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=5)	// version bigger than 5, apply TS phase solution
   	{
		 DVBC_TS_PHASE_EN = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[INDEX_C_TS_PHASE_EN];
		 DVBC_TS_PHASE_NUM = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[INDEX_C_TS_PHASE_NUM];   
		 printf("##DVBC: TS check: bTsPhaseEn = %d, u16TsPhaseNum = %d\n",DVBC_TS_PHASE_EN,DVBC_TS_PHASE_NUM);
   	}
	else
	{
		printf("##DVBC:TS Phase check !!, board version smaller than 4\n");
	}
   }
   else	// if init board define is NULL TS phase needs check.
   {
   	printf("##DVBC: TS Phase check !!\n");
   }
	
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBC_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize);
        }
        else
        {
            printf("u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBC_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }
    INTERN_DVBC_Version(&sDMD_DVBC_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    printf("firmware version: %x\n",sDMD_DVBC_Info.u16Version);
    #endif
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBC_Exit(void)
{
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_Exit\n");
    }
    #endif
    DMD_LOCK();
    INTERN_DVBC_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBC_Mutex);
    _s32DMD_DVBC_Mutex= -1;
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBC_SetDbgLevel(DMD_DVBC_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMD_DVBC_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

const DMD_DVBC_Info* MDrv_DMD_DVBC_GetInfo(void)
{
	    #ifdef MS_DEBUG
    MS_U8 state = 0;
	

    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetInfo\n");
    }
    #endif	
//for DBG	
#ifdef MS_DEBUG	
	MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &state);
	printf(" @INTERN_DVBC_GetLock FSM 0x%x\n",state);
        INTERN_DVBC_Show_AGC_Info();
	INTERN_DVBC_info();
#endif

    return (const DMD_DVBC_Info*)&sDMD_DVBC_Info;
}

MS_BOOL MDrv_DMD_DVBC_GetLibVer(const MSIF_Version **ppVersion)
{
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetLibVer\n");
    }
    #endif
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbc_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_DVBC_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBC_Version(ver);
    //printf("MDrv_DMD_DVBC_GetFWVer %x\n", *ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL MDrv_DMD_DVBC_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#ifdef CHIP_K1
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetDSPReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
#else
  	printf("Not Support function: MDrv_DMD_DVBC_GetDSPReg\n");
	return FALSE;
#endif
}

MS_BOOL MDrv_DMD_DVBC_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#ifdef CHIP_K1
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_SetDSPReg %x %x\n", u16Addr, pu8Data);
    }
    #endif

    return bRet;
#else
  	printf("Not Support function: MDrv_DMD_DVBC_SetDSPReg\n");
	return FALSE;
#endif
}


MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif
    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_SetSerialControl %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[2];
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
    bRet=INTERN_DVBC_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_SetConfig(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
    return MDrv_DMD_DVBC_SetConfig_symbol_rate_list(u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
}

MS_BOOL MDrv_DMD_DVBC_SetConfig_symbol_rate_list(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_SetConfig %d %d %ld %d %d\n", u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[2];
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
    bRet=INTERN_DVBC_Config(u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk, pu16_symbol_rate_list,u8_symbol_rate_list_num);
    sDMD_DVBC_Info.u16SymbolRate = u16SymbolRate;
    sDMD_DVBC_Info.eQamMode = eQamMode;
    sDMD_DVBC_Info.u32IFFreq = u32IFFreq;
    sDMD_DVBC_Info.bSpecInv = bSpecInv;
    sDMD_DVBC_Info.bSerialTS = bSerialTS;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBC_Active(bEnable);
    sDMD_DVBC_Info.u32ChkScanTimeStart = MsOS_GetSystemTime();
    u32DMD_DVBC_ScanCount=0;
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("%s %ld\n", __FUNCTION__, sDMD_DVBC_Info.u32ChkScanTimeStart);
    }
    #endif
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus)
{
    return MDrv_DMD_DVBC_GetLockWithRFPower(eType, eLockStatus, 200.0f, -200.0f);
}

MS_BOOL MDrv_DMD_DVBC_GetLockWithRFPower(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm)
{
    MS_U32 u32CurrScanTime=0;
    MS_BOOL bRet=FALSE;
    DMD_LOCK();
    if ( eType == DMD_DVBC_GETLOCK ) // for channel scan
    {
        if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_FEC_LOCK, 200.0f, -200.0f, 0))
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            MS_U32 u32Timeout = 10000;//(sDMD_DVBC_Info.u16SymbolRate) ? 5000 : 15000;
            if (sDMD_DVBC_Info.u16SymbolRate == 0)
            {
                u32Timeout = u16DMD_DVBC_AutoSymbol_Timeout;
            }
            else
            {
                u32Timeout = (sDMD_DVBC_Info.eQamMode == DMD_DVBC_QAMAUTO) ? u16DMD_DVBC_FixSymbol_AutoQam_Timeout : u16DMD_DVBC_FixSymbol_FixQam_Timeout;
            }

            u32CurrScanTime=MsOS_GetSystemTime();
            if (u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart < u32Timeout)
            {
                if (u32DMD_DVBC_ScanCount==0)
                {
                    u32DMD_DVBC_PrevScanTime=u32CurrScanTime;
                    u32DMD_DVBC_ScanCount++;
                }

                if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
                {
                    sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
                }
                else
                {
                    sDMD_DVBC_Info.u8SarValue=0xFF;
                }

                bRet=HAL_DMD_GetRFLevel(&fCurrRFPowerDbm, fCurrRFPowerDbm, sDMD_DVBC_Info.u8SarValue,
                                                                            _sDMD_DVBC_InitData.pTuner_RfagcSsi, _sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef, _sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef, _sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef, _sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef, _sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size);

                if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_NO_CHANNEL, fCurrRFPowerDbm, fNoChannelRFPowerDbm, u32CurrScanTime-u32DMD_DVBC_PrevScanTime))
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:NO_CHANNEL\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
                else if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_ATV_DETECT, 200.0f, -200.0f, 0))
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:ATV_DETECT\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
#ifdef NEW_TR_MODULE
                else if ((INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TR_EVER_LOCK, 200.0f, -200.0f, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart) > 500))
#else
                else if ((INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TR_EVER_LOCK, 200.0f, -200.0f, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart) > 5000))
#endif
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:TR\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
                else
                {
                    *eLockStatus = DMD_DVBC_CHECKING;
                }
            }
            else
            {
                #ifdef MS_DEBUG
                printf("%s %ld UNLOCK:TIMEOUT %ld\n", __FUNCTION__, MsOS_GetSystemTime(), u32Timeout);
                #endif
                *eLockStatus = DMD_DVBC_UNLOCK;
            }
            u32DMD_DVBC_PrevScanTime=u32CurrScanTime;
        }
        sDMD_DVBC_Info.eLockStatus = *eLockStatus;
        #ifdef MS_DEBUG
        if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
        {
            printf("%s %ld %d\n", __FUNCTION__, MsOS_GetSystemTime(), sDMD_DVBC_Info.eLockStatus);
        }
        #endif
    }
    else
    {
        if (INTERN_DVBC_GetLock(eType, 200.0f, -200.0f, 0) == TRUE)
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            *eLockStatus = DMD_DVBC_UNLOCK;
        }
    }
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetLock %x %x\n", eType, *eLockStatus);
    }
    #endif
    bRet=TRUE;
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}

MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBC_Info.u8SarValue=0xFF;
    }
    bRet=INTERN_DVBC_GetSignalStrength(u16Strength, (const DMD_DVBC_InitData *)(&_sDMD_DVBC_InitData), sDMD_DVBC_Info.u8SarValue, fRFPowerDbm);
    sDMD_DVBC_Info.u16Strength=*u16Strength;
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBC_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}

MS_BOOL MDrv_DMD_DVBC_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBC_Info.u8SarValue=0xFF;
    }
    bRet=INTERN_DVBC_GetSignalQuality(u16Quality, (const DMD_DVBC_InitData *)(&_sDMD_DVBC_InitData), sDMD_DVBC_Info.u8SarValue, fRFPowerDbm);
    sDMD_DVBC_Info.u16Quality=*u16Quality;
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("%s %d %d\n", __FUNCTION__, bRet, sDMD_DVBC_Info.u16Quality);
    }
    #endif

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}
#ifdef CHIP_KAISER
MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();
    if((demod_no==0)  &&(drv_demod_swtich_status!=0))
    {
    	//save the demod 1 parameter back
    	_sDMD_DVBC_InitData_dmd1=_sDMD_DVBC_InitData;
	sDMD_DVBC_Info_dmd1=sDMD_DVBC_Info;
	u16DMD_DVBC_AutoSymbol_Timeout_dmd1=u16DMD_DVBC_AutoSymbol_Timeout;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1=u16DMD_DVBC_FixSymbol_AutoQam_Timeout;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1=u16DMD_DVBC_FixSymbol_FixQam_Timeout;
	u32DMD_DVBC_PrevScanTime_dmd1=u32DMD_DVBC_PrevScanTime;
	u32DMD_DVBC_ScanCount_dmd1=u32DMD_DVBC_ScanCount;
    }
    else if((demod_no==1)  &&(drv_demod_swtich_status!=1))
    {
    	//save the demod 0 parameter back
    	_sDMD_DVBC_InitData_dmd0=_sDMD_DVBC_InitData;
	sDMD_DVBC_Info_dmd0=sDMD_DVBC_Info;
	u16DMD_DVBC_AutoSymbol_Timeout_dmd0=u16DMD_DVBC_AutoSymbol_Timeout;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0=u16DMD_DVBC_FixSymbol_AutoQam_Timeout;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0=u16DMD_DVBC_FixSymbol_FixQam_Timeout;
	u32DMD_DVBC_PrevScanTime_dmd0=u32DMD_DVBC_PrevScanTime;
	u32DMD_DVBC_ScanCount_dmd0=u32DMD_DVBC_ScanCount;
    }

    
    bRet=INTERN_DVBC_ActiveDmdSwitch(demod_no);

    if((demod_no==0)  &&(drv_demod_swtich_status!=0))
    {
    	//insert the demod 0 parameter
    	_sDMD_DVBC_InitData=_sDMD_DVBC_InitData_dmd0;
	sDMD_DVBC_Info=sDMD_DVBC_Info_dmd0;
	u16DMD_DVBC_AutoSymbol_Timeout=u16DMD_DVBC_AutoSymbol_Timeout_dmd0;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout=u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout=u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0;
	u32DMD_DVBC_PrevScanTime=u32DMD_DVBC_PrevScanTime_dmd0;
	u32DMD_DVBC_ScanCount=u32DMD_DVBC_ScanCount_dmd0;
    }
    else if((demod_no==1)  &&(drv_demod_swtich_status!=1))
    {
    	//insert the demod 1 parameter
    	_sDMD_DVBC_InitData=_sDMD_DVBC_InitData_dmd1;
	sDMD_DVBC_Info=sDMD_DVBC_Info_dmd1;
	u16DMD_DVBC_AutoSymbol_Timeout=u16DMD_DVBC_AutoSymbol_Timeout_dmd1;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout=u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout=u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1;
	u32DMD_DVBC_PrevScanTime=u32DMD_DVBC_PrevScanTime_dmd1;
	u32DMD_DVBC_ScanCount=u32DMD_DVBC_ScanCount_dmd1;
    }

    drv_demod_swtich_status=demod_no;
    DMD_UNLOCK();
    return bRet;
}
#else
MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
	printf("Doesn't support DVBC_ActiveDmdSwitch function!!!\n");
	return false;
}
#endif
MS_BOOL MDrv_DMD_DVBC_GetSNR(float *fSNR)
{
    MS_BOOL bRet;
    DMD_LOCK();
    bRet=INTERN_DVBC_GetSNR(fSNR);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer(float *ber)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBC_GetPostViterbiBer(ber);
    DMD_UNLOCK();

	  return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBC_GetPacketErr(pktErr);
    DMD_UNLOCK();

	  return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetCellID(MS_U16 *u16CellID)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBC_Get_CELL_ID(u16CellID);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetCellID %x\n", *u16CellID);
    }
    #endif
    return bRet;
}

MS_BOOL MDrv_DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    INTERN_DVBC_GetCurrentModulationType(pQAMMode);
    INTERN_DVBC_GetCurrentSymbolRate(u16SymbolRate);
    INTERN_DVBC_Get_FreqOffset(pFreqOff,8);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBC_GetStatus %d %d %f\n", *pQAMMode, *u16SymbolRate, *pFreqOff);
    }
    #endif
    return bRet;
}

MS_U32 MDrv_DMD_DVBC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
	u32Return = u32Return;
    if (u16PowerState == E_POWER_SUSPEND)
	{
        MDrv_DMD_DVBC_Exit();
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            MDrv_DMD_DVBC_Init(&_sDMD_DVBC_InitData, sizeof(_sDMD_DVBC_InitData));
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
