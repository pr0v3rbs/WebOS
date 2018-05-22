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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvCPU.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif


#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "regCHIP.h"
#include "halCHIP.h"

#include "drvMMIO.h"
#include "drvCPU.h"
#include "halCPU.h"
#include "regCPU.h"
#if (CPU_UTOPIA20)
#include "utopia.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define COPRO_DBG_FUNC()             if (_coproDbgLevel >= E_COPRO_DBGLV_ALL) \
                                        {printf("\t====   %s   ====\n", __FUNCTION__);}
#define COPRO_DBG_INFO(x, args...)   if (_coproDbgLevel >= E_COPRO_DBGLV_INFO ) \
                                        {printf(x, ##args);}
#define COPRO_DBG_ERR(x, args...)    if (_coproDbgLevel >= E_COPRO_DBGLV_ERR_ONLY) \
                                        {printf(x, ##args);}
#define CPU_DEBUGINFO(x) //x

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//MST_PANEL_INFO_t  g_Panel_Info;
MS_U8 u8QueueClass;
MS_U8 parasize, *TmpQueue, QueueSize, QueueLength, u8Temp;
MS_U8 u8WritePtr, u8ReadPtr, u8QLen;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL g_bBEON_HOST = FALSE;
MS_BOOL bWait_MMAP_Receive=FALSE;

#if (CPU_UTOPIA20)
void* pInstantCpu = NULL;
void* pAttributeCpu = NULL;
#endif

// Uart driver info
static COPRO_DrvInfo                 _coproInfo =
                                    {
                                        .Init = FALSE,
                                        .u32Addr = 0,
                                    };

static COPRO_DrvStatus               _coproStatus =
                                    {
                                        .HwBusy = FALSE,
                                    };

static MSIF_Version _coproVersion = {
    .DDI = { COPRO_DRV_VERSION },
};

static COPRO_DbgLv _coproDbgLevel = E_COPRO_DBGLV_ERR_ONLY;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void _MDrv_CPU_Disable(void)
{
    _coproStatus.HwBusy = TRUE;
    HAL_COPRO_Disable();
    _coproStatus.HwBusy = FALSE;
}

void _MDrv_CPU_Enable(MS_U32 u32_ADR)
{
    _coproStatus.HwBusy = TRUE;
    _coproInfo.u32Addr = u32_ADR;
    HAL_COPRO_Enable(u32_ADR);
    _coproStatus.HwBusy = FALSE;
}

void _MDrv_CPU_SetHost(MS_BOOL bHost)
{
     g_bBEON_HOST = bHost;
}

void _MDrv_CPU_InitFront(void)
{
    if(MDrv_COPRO_GetBase()==FALSE)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("MDrv_COPRO_Init Fail\n"));
        return;
    }

    HAL_COPRO_Init_Front();
	_coproInfo.Init = FALSE;
    _coproStatus.HwBusy = FALSE;
}

void _MDrv_CPU_InitEnd(MS_U32 u32_ADR)
{
    HAL_COPRO_Init_End(u32_ADR);
    // Driver Initialization
    _coproInfo.Init = TRUE;
    _coproInfo.u32Addr = u32_ADR;
    _coproStatus.HwBusy = FALSE;
}

const COPRO_DrvInfo* _MDrv_CPU_GetInfo(void)
{
    return (&_coproInfo);
}

COPRO_Result _MDrv_CPU_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_COPRO_FAIL;
    }

    *ppVersion = &_coproVersion;

    return E_COPRO_OK;
}

COPRO_Result _MDrv_CPU_GetStatus(COPRO_DrvStatus *pStatus)
{
    memcpy(pStatus, &_coproStatus, sizeof(COPRO_DrvStatus));
    return E_COPRO_OK;
}

void _MDrv_CPU_SetDbgLevel(MS_U8 level)
{
    _coproDbgLevel = (COPRO_DbgLv)level;
}


MS_U32 _MDrv_CPU_GetBase(void)
{
    MS_U32 u32NonPMBankSize;
    MS_U32 u32NonPMBankAddr = 0;

    _coproStatus.HwBusy = TRUE;

    ///-Disable aeon
    if( !MDrv_MMIO_GetBASE( &u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_HW))
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("_COPRO_Init: IOMap failure\n"));
    }
    else
    {
        HAL_COPRO_RegSetBase(u32NonPMBankAddr);
        MS_DEBUG_MSG(COPRO_DBG_ERR("_COPRO_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr));
    }
    _coproStatus.HwBusy = FALSE;

    return u32NonPMBankAddr;
}

MS_U32 _MDrv_CPU_QueryClock(void)
{
    return HAL_CPU_QueryClock();
}


//#####################################################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_CPU_GetVerString()
/// @brief \b Function \b Description: This function shows the version of
///                                    cpu driver version
/// @param <IN>        \b None :
/// @param <OUT>       \b *pVerString : pointer to a U8 string to
///                                     store the CPU driver version
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b VER  : Library version string
////////////////////////////////////////////////////////////////////////////////
const COPRO_DrvInfo* MDrv_COPRO_GetInfo(void)
{
    return _MDrv_CPU_GetInfo();
}

COPRO_Result MDrv_COPRO_GetLibVer(const MSIF_Version **ppVersion)
{
    return _MDrv_CPU_GetLibVer(ppVersion);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_GetStatus
/// @brief \b Function \b Description: Get status
/// @param  <IN>     \b pStatus : a pointer to status
/// @param  <OUT>    \b pStatus : get the status from the pointer
/// @param  <RET>    \b COPRO_Result :
////////////////////////////////////////////////////////////////////////////////
COPRO_Result MDrv_COPRO_GetStatus(COPRO_DrvStatus *pStatus)
{
  #if (CPU_UTOPIA20)
    PCPU_GETSTATUS_PARAM pGetStatusParam = NULL;
    
    MS_BOOL bFlagClose = FALSE;
    if (pInstantCpu==NULL)
    {
        if(UtopiaOpen(MODULE_CPU, &pInstantCpu, 0, pAttributeCpu) !=  UTOPIA_STATUS_SUCCESS)
        {
            MS_DEBUG_MSG(COPRO_DBG_ERR("Open CPU fail\n"));
            return E_COPRO_FAIL;
        }
        bFlagClose = TRUE;
    }

    pGetStatusParam = (PCPU_GETSTATUS_PARAM)malloc(sizeof(CPU_GETSTATUS_PARAM));
    if (pGetStatusParam==NULL)
    {
        return E_COPRO_FAIL;
    }
    pGetStatusParam->pStatus = pStatus;
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_GetStatus,(void*)pGetStatusParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_GetStatus failure\n"));
    }
   
    free(pGetStatusParam); 
    
    if (bFlagClose)
    {
        if(UtopiaClose(pInstantCpu) != UTOPIA_STATUS_SUCCESS)
        {
            MS_DEBUG_MSG(COPRO_DBG_ERR("close CPU fail\n"));
        }	
        pInstantCpu = 0;
    }  
   
    return E_COPRO_OK;
  #else
    return _MDrv_CPU_GetStatus(pStatus);
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_SetDbgLevel
/// @brief \b Function \b Description: Seet debug level
/// @param  <IN>     \b level : debug level
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_COPRO_SetDbgLevel(MS_U8 u8Level)
{
  #if (CPU_UTOPIA20)
    PCPU_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;
    if (pInstantCpu==NULL)
    {
        return;
    }
    pSetDbgLevelParam = (PCPU_SETDBGLEVEL_PARAM)malloc(sizeof(CPU_SETDBGLEVEL_PARAM));
    if (pSetDbgLevelParam==NULL)
    {
        return;
    }
    pSetDbgLevelParam->u8Level = u8Level;
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_SetDbgLevel,(void*)pSetDbgLevelParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_SetDbgLevel failure\n"));
    }
    free(pSetDbgLevelParam);
  #else
    _MDrv_CPU_SetDbgLevel(u8Level);
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SetBEON_Host
/// @brief \b Function \b Description: Set BEON as Host
/// @param  <IN>     \b bHost : TRUE : BEON is Host, FALSE : BEON is not host
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_SetBEON_Host(MS_BOOL bHost)
{
  #if (CPU_UTOPIA20)
    PCPU_SETHOST_PARAM pSetHostParam = NULL;
    if (pInstantCpu==NULL)
    {
        return;
    }
    pSetHostParam = (PCPU_SETHOST_PARAM)malloc(sizeof(CPU_SETHOST_PARAM));
    if (pSetHostParam==NULL)
    {
        return;
    }
    pSetHostParam->bHost = bHost;
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_SetHost,(void*)pSetHostParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_SetHost failure\n"));
    }
    free(pSetHostParam);
  #else
     _MDrv_CPU_SetHost(bHost);
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_Disable
/// @brief \b Function \b Description: Reset & disable co-processor
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_COPRO_Disable(void)
{
  #if (CPU_UTOPIA20)
    if (pInstantCpu==NULL)
    {
        return;
    }
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_Disable,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_Disable failure\n"));
    }
  #else
    _MDrv_CPU_Disable();
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_Enable
/// @brief \b Function \b Description: Release to enable co-processor
/// @param  <IN>     \b u32_ADR : Set physical address
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_COPRO_Enable(MS_U32 u32_ADR)
{
  #if (CPU_UTOPIA20)
    PCPU_ENABLE_PARAM pEnableParam = NULL;
    if (pInstantCpu==NULL)
    {
        return;
    }
    pEnableParam = (PCPU_ENABLE_PARAM)malloc(sizeof(CPU_ENABLE_PARAM));
    if (pEnableParam==NULL)
    {
        return;
    }
    pEnableParam->u32_ADR = u32_ADR;
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_Enable,(void*)pEnableParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_Enable failure\n"));
    }
    free(pEnableParam);
  #else
    _MDrv_CPU_Enable(u32_ADR);
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_GetBase
/// @brief \b Function \b Description: Get RIU access base
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b MS_U32 : RIU access base
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_COPRO_GetBase(void)
{
  #if (CPU_UTOPIA20)
    MS_U32 u32BaseAddr = 0;
    MS_BOOL bFlagClose = FALSE;
    if (pInstantCpu==NULL)
    {
        if(UtopiaOpen(MODULE_CPU, &pInstantCpu, 0, pAttributeCpu) !=  UTOPIA_STATUS_SUCCESS)
        {
            MS_DEBUG_MSG(COPRO_DBG_ERR("Open CPU fail\n"));
            return E_COPRO_FAIL;
        }
        bFlagClose = TRUE;
    }
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_GetBase,(void*)(&u32BaseAddr)) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_GetStatus failure\n"));
    }
    if (bFlagClose)
    {
        //if(UtopiaClose(pInstantCpu) != UTOPIA_STATUS_SUCCESS)
        //{
        //    MS_DEBUG_MSG(COPRO_DBG_ERR("close CPU fail\n"));
        //}	
        //pInstantCpu = 0;
    }
    return u32BaseAddr;
  #else
    return _MDrv_CPU_GetBase();
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_Init_Front
/// @brief \b Function \b Description: Reset & disable co-processor
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_COPRO_Init_Front(void)
{
	#if (CPU_UTOPIA20)
    if(pInstantCpu)
    {
	    //if(UtopiaClose(pInstantCpu) != UTOPIA_STATUS_SUCCESS)
	    //{
		//    MS_DEBUG_MSG(COPRO_DBG_ERR("close CPU fail\n"));
	    //}
        //pInstantCpu = NULL;
    }
	#endif
    return _MDrv_CPU_InitFront();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_COPRO_Init_End
/// @brief \b Function \b Description: Release & enable co-processor
/// @param  <IN>     \b u32_ADR : Set physical address
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void MDrv_COPRO_Init_End(MS_U32 u32_ADR)
{
	#if (CPU_UTOPIA20)
    if(pInstantCpu==NULL)
    {
	    if(UtopiaOpen(MODULE_CPU|KERNEL_MODE, &pInstantCpu, 0, pAttributeCpu) !=  UTOPIA_STATUS_SUCCESS)
	    {
		    MS_DEBUG_MSG(COPRO_DBG_ERR("Open CPU fail\n"));
		    _coproInfo.Init = FALSE;
	    }
    }
	#endif
    return _MDrv_CPU_InitEnd(u32_ADR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_CPU_QueryClock
/// @brief \b Function \b Description: Get Host CPU clock
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_CPU_QueryClock(void)
{
  #if (CPU_UTOPIA20)
    MS_U32 u32QueryClock;
    MS_BOOL bFlagClose = FALSE;
    if (pInstantCpu==NULL)
    {
        if(UtopiaOpen(MODULE_CPU|KERNEL_MODE, &pInstantCpu, 0, pAttributeCpu) !=  UTOPIA_STATUS_SUCCESS)
        {
            MS_DEBUG_MSG(COPRO_DBG_ERR("Open CPU fail\n"));
            return E_COPRO_FAIL;
        }
        bFlagClose = TRUE;
    }
    if (UtopiaIoctl(pInstantCpu,MDrv_CMD_CPU_QueryClock,(void*)(&u32QueryClock)) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(COPRO_DBG_ERR("Ioctl MDrv_CMD_CPU_QueryClock failure\n"));
    }
    if (bFlagClose)
    {
        //if(UtopiaClose(pInstantCpu) != UTOPIA_STATUS_SUCCESS)
        //{
        //    MS_DEBUG_MSG(COPRO_DBG_ERR("close CPU fail\n"));
        //}	
		//pInstantCpu = 0;
    }
    return u32QueryClock;
  #else
    return _MDrv_CPU_QueryClock();
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_CPU_SetPowerState
/// @brief \b Function \b Description: Support Suspend/Resume mechanism 
/// @param  <IN>     \b u16PowerState : set power state
/// @param  <OUT>    \b None :
/// @param  <RET>    \b MS_U32 : return utopia status
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_CPU_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL; 

	if (u16PowerState == E_POWER_SUSPEND)
	{
	    MDrv_COPRO_Disable();
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_COPRO_Init_Front();
            MDrv_COPRO_Init_End(_coproInfo.u32Addr);
			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
		    MS_DEBUG_MSG(COPRO_DBG_ERR("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__));
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
        MS_DEBUG_MSG(COPRO_DBG_ERR("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState));
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}

