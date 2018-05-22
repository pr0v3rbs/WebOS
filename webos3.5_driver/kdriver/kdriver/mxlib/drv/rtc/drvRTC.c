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
/// file    drvWDT.c
/// @brief  Piu Watch Dog Timer Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsDevice.h"
#include "MsVersion.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvMMIO.h"
#include "drvRTC.h"
#include "halRTC.h"
#include "MsOS.h"
#include "utopia_dapi.h"

#define RTC_DEBUG   1

#ifdef RTC_DEBUG
    #define RTC_DBG_FUNC()            if (_rtcDbfLevel >= E_RTC_DBGLV_ALL) \
                                           {printf("\t====   %s   ====\n", __FUNCTION__);}
    #define RTC_DBG_INFO(x, args...)  if (_rtcDbfLevel >= E_RTC_DBGLV_INFO ) \
                                           {printf(x, ##args);}
    #define RTC_DBG_ERR(x, args...)   if (_rtcDbfLevel >= E_RTC_DBGLV_ERR_ONLY) \
                                           {printf(x, ##args);}
#else
    #define RTC_DBG_FUNC()             while (0)
    #define RTC_DBG_INFO(x, args...)   while (0)
    #define RTC_DBG_ERR(x, args...)    while (0)
#endif

#define RTC_MUTEX_LOCK()            
#define RTC_MUTEX_UNLOCK()          
#define RTC_MUTEX_DELETE()

#if (RTC_UTOPIA20)
#include "utopia.h"
#endif
////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL  _gbInitRTC = FALSE;
static RTC_DbgLv _rtcDbfLevel = E_RTC_DBGLV_ERR_ONLY;
static MSIF_Version _drv_rtc_version = {
    .DDI = { RTC_DRV_VERSION },
};
// for Fastboot 
static E_MS_DRV_RTC g_eRtc = 0;
static MS_U32 g_u32Freq = 0;
static MS_U32 g_u32Xtal =0;
static MS_U32 g_u32Sec = 0;
static InterruptCb g_pIntCb = NULL;

#if (RTC_UTOPIA20)
MS_BOOL bU20RTCOpened = FALSE;
void* pInstantRtc;
void* pAttributeRtc;
#endif

static void MDrv_RTC_Int_Mode(E_MS_DRV_RTC eRtc, InterruptCb pIntCb)
{
    if(pIntCb)
    {
        HAL_RTC_IntMask((E_MS_RTC)eRtc, DISABLE);
        MsOS_AttachInterrupt(E_INT_IRQ_RTC,pIntCb);
        MsOS_EnableInterrupt(E_INT_IRQ_RTC);
    }
    else
    {
        HAL_RTC_IntMask((E_MS_RTC)eRtc, ENABLE);
        MsOS_DetachInterrupt(E_INT_IRQ_RTC);
        MsOS_DisableInterrupt(E_INT_IRQ_RTC);
    }
}

MS_BOOL MDrv_RTC_GetLibVer(const MSIF_Version **ppVersion)
{
    RTC_DBG_FUNC();

    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_rtc_version;
    return TRUE;
}


MS_BOOL _MDrv_RTC_Init(E_MS_DRV_RTC eRtc, MS_U32 u32Freq, MS_U32 u32Xtal, MS_U32 u32Sec,InterruptCb pIntCb)
{
    if(u32Xtal<u32Freq || u32Freq==0)
        return FALSE;
	// store for FastBoot 
	g_eRtc = eRtc;
	g_u32Freq = u32Freq;
	g_u32Xtal = u32Xtal;
	g_u32Sec = u32Sec;
	g_pIntCb = pIntCb;
    if (!(_gbInitRTC& (1<<eRtc)))
    {
        MS_U32 u32BaseAddr, u32BaseSize;
        if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            RTC_DBG_ERR("Get IOMAP Base faill!\n");
            return FALSE;
        }

        HAL_RTC_SetIOMapBase(u32BaseAddr);

        HAL_RTC_RESET((E_MS_RTC)eRtc, ENABLE);
        HAL_RTC_Set_Frequency((E_MS_RTC)eRtc,u32Freq, u32Xtal);
        HAL_RTC_Match_Counter((E_MS_RTC)eRtc,u32Sec*u32Freq);
        printf("u32Freq*u32Freq:%lx\n",u32Freq*u32Freq);
        HAL_RTC_Loading ((E_MS_RTC)eRtc, ENABLE);
        HAL_RTC_Reading((E_MS_RTC)eRtc, ENABLE);
        MDrv_RTC_Int_Mode(eRtc,pIntCb);
        HAL_RTC_Set_Counter((E_MS_RTC)eRtc,0); //start from 0
        HAL_RTC_Counter((E_MS_RTC)eRtc, ENABLE);
        HAL_RTC_Wrap_Count ((E_MS_RTC)eRtc, ENABLE);
        _gbInitRTC|=(1<<eRtc);
        return TRUE;
    }
    else
        return FALSE;
}

MS_U32 _MDrv_RTC_GetCount(E_MS_DRV_RTC eRtc)
{
    if(_gbInitRTC)
        return HAL_RTC_Read_Counter((E_MS_RTC)eRtc);
    else
        return 0;
}

void _MDrv_RTC_DeInit(E_MS_DRV_RTC eRtc)
{
    //if(_gbInitRTC&(1<<eRtc))
    //{
    HAL_RTC_RESET((E_MS_RTC)eRtc, DISABLE);
    HAL_RTC_Counter((E_MS_RTC)eRtc, DISABLE);
    //    _gbInitRTC&=(~(1<<eRtc));
    //}
}
 
MS_U16 MDrv_RTC_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitRTC = FALSE;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_RTC_Init( g_eRtc, g_u32Freq, g_u32Xtal, g_u32Sec,g_pIntCb);

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

#if (RTC_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_RTC_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20RTCOpened)
    {
        if(UtopiaOpen(MODULE_RTC|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open RTC fail\n");
    		return FALSE;
        }
        bU20RTCOpened = TRUE;
    }
    return TRUE;
}
#endif

MS_BOOL MDrv_RTC_Init(E_MS_DRV_RTC eRtc, MS_U32 u32Freq, MS_U32 u32Xtal, MS_U32 u32Sec,InterruptCb pIntCb)
{
#if (RTC_UTOPIA20)
    MS_U32 u32Ret;
    PRTC_INIT_PARAM pRtcParam_Init = NULL;
    if (FALSE == _MDrv_RTC_CheckUtopia20Open(&pInstantRtc, 0, pAttributeRtc))
        return 0;	
    u32Ret = UtopiaIoctl(pInstantRtc,MDrv_CMD_RTC_INIT,(MS_U32*)pRtcParam_Init);
    free(pRtcParam_Init);
    return (MS_BOOL)u32Ret;    
#else
     return _MDrv_RTC_Init(eRtc,u32Freq,u32Xtal,u32Sec,pIntCb);
#endif	
}

MS_U32 MDrv_RTC_GetCount(E_MS_DRV_RTC eRtc)
{
#if (RTC_UTOPIA20)
    MS_U32 u32Ret;
    PRTC_GETCOUNT_PARAM pRtcParam_GetCount = NULL;
    if (FALSE == _MDrv_RTC_CheckUtopia20Open(&pInstantRtc, 0, pAttributeRtc))
        return 0;	
    u32Ret = UtopiaIoctl(pInstantRtc,MDrv_CMD_RTC_Get_Count,(MS_U32*)pRtcParam_GetCount);
    free(pRtcParam_GetCount);
    return (MS_BOOL)u32Ret;    
#else
     return _MDrv_RTC_GetCount(eRtc);
#endif	
}

void MDrv_RTC_DeInit(E_MS_DRV_RTC eRtc)
{
#if (RTC_UTOPIA20)
    PRTC_DEINIT_PARAM pRtcParam_DeInit = NULL;
    if (FALSE == _MDrv_RTC_CheckUtopia20Open(&pInstantRtc, 0, pAttributeRtc))
        return ;	
    UtopiaIoctl(pInstantRtc,MDrv_CMD_RTC_DeInit,(MS_U32*)pRtcParam_DeInit);
    free(pRtcParam_DeInit);       
#else
     _MDrv_RTC_DeInit(eRtc);
#endif	
}