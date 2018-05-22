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
//==============================================================================
// [mdrv_sc_dynamicscaling.c]
// Date: 2012/10/29
// Descriptions: dynamic scaling related functions
//==============================================================================
#ifndef  _MDRV_SC_DYNAMICSCALING_C_
#define  _MDRV_SC_DYNAMICSCALING_C_

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

//#include "Debug.h"
#include "drvXC_IOPort.h"
#include "MsTypes.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_dynamicscaling.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "xc_hwreg_utility2.h"
#include "mdrv_sc_dynamicscaling.h"

E_APIXC_ReturnValue MDrv_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
	bReturn = MHal_XC_Get_DSForceIndexSupported(pInstance, eWindow);
    return bReturn?E_APIXC_RET_OK:E_APIXC_RET_FAIL;
}

E_APIXC_ReturnValue MDrv_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
	MHal_XC_Set_DSIndexSourceSelect(pInstance, eDSIdxSrc, eWindow);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    MHal_XC_Set_DSForceIndex(pInstance, bEnable, u8Index, eWindow);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[eWindow] = bEnable;

    return E_APIXC_RET_OK;
}

MS_BOOL MDrv_XC_Is_DSForceIndexEnabled(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[eWindow];
}

E_APIXC_ReturnValue MApi_XC_Get_DSForceIndexSupported_U2(void* pInstance, SCALER_WIN eWindow)
{
    return MDrv_XC_Get_DSForceIndexSupported(pInstance, eWindow);
}

E_APIXC_ReturnValue MApi_XC_Get_DSForceIndexSupported(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_DS_FORCE_INDEX_SUPPORTED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DS_FORCE_INDEX_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_DSIndexSourceSelect_U2(void* pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
#if SC2_ENABLE
             _XC_ENTRY(pInstance);
#else //SC2_ENABLE
            _XC_ENTRY();
#endif //#if SC2_ENABLE
    eReturn = MDrv_XC_Set_DSIndexSourceSelect(pInstance, eDSIdxSrc, eWindow);
#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
           _XC_RETURN();
#endif //#if SC2_ENABLE

    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Set_DSIndexSourceSelect(E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_INDEX_SOURCE_SELECT XCArgs;
    XCArgs.eDSIdxSrc = eDSIdxSrc;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DS_INDEX_SOURCE_SELECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_DSForceIndex_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
#if SC2_ENABLE
             _XC_ENTRY(pInstance);
#else //SC2_ENABLE
            _XC_ENTRY();
#endif //#if SC2_ENABLE
    eReturn = MDrv_XC_Set_DSForceIndex(pInstance, bEnable, u8Index, eWindow);
#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
           _XC_RETURN();
#endif //#if SC2_ENABLE
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Set_DSForceIndex(MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_FORCE_INDEX XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Index = u8Index;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DS_FORCE_INDEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get Dynamic Scaling Status
/// @return @ref MS_BOOL TRUE: dynamic scaling is enabled; FALSE: dynamic scaling is not enabled.
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetDynamicScalingStatus_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    bReturn = MDrv_XC_GetDynamicScalingStatus(pInstance);
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return bReturn;
}

MS_BOOL MApi_XC_GetDynamicScalingStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_DS_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DS_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_GetDynamicScalingStatus(void *pInstance)
{
    return MHAL_SC_Get_DynamicScaling_Status(pInstance);
}

MS_BOOL MDrv_XC_EnableIPMTuneAfterDS(void *pInstance, MS_BOOL bEnable)
{
    return MHAL_SC_Enable_IPMTuneAfterDS(pInstance, bEnable);
}


//-------------------------------------------------------------------------------------------------
/// Set Dynamic Scaling
/// @param  pstDSInfo              \b IN: the information of Dynamic Scaling
/// @param  u32DSInforLen          \b IN: the length of the pstDSInfo
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetDynamicScaling_U2(void* pInstance, XC_DynamicScaling_Info *pstDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
{
    MS_BOOL bStatus;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    if(u32DSInfoLen != sizeof(XC_DynamicScaling_Info))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
#if SC2_ENABLE
             _XC_ENTRY(pInstance);
#else //SC2_ENABLE
            _XC_ENTRY();
#endif //#if SC2_ENABLE
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"It's dynamic scaling case(%u,%u,%u)\n",
            pstDSInfo->bOP_DS_On,pstDSInfo->bIPS_DS_On,pstDSInfo->bIPM_DS_On);

    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d]SLZ[%s] use DS memory base addr = %lx, MIU_sel =%x, Depth=%u\n",
            __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", pstDSInfo->u32DS_Info_BaseAddr, pstDSInfo->u8MIU_Select, pstDSInfo->u8DS_Index_Depth);

    bStatus = MDrv_XC_Set_DynamicScaling(
        pInstance,
        pstDSInfo->u32DS_MMap_Addr,
        pstDSInfo->u32DS_MMap_Size,
        pstDSInfo->u32DS_Info_BaseAddr,
        pstDSInfo->u8MIU_Select,
        pstDSInfo->u8DS_Index_Depth,
        pstDSInfo->bOP_DS_On,
        pstDSInfo->bIPS_DS_On,
        pstDSInfo->bIPM_DS_On,
        eWindow);
#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
           _XC_RETURN();
#endif //#if SC2_ENABLE
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bStatus;
}

MS_BOOL MApi_XC_SetDynamicScaling(XC_DynamicScaling_Info *pstDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_DYNAMIC_SCALING XCArgs;
    XCArgs.pstDSInfo = pstDSInfo;
    XCArgs.u32DSInfoLen = u32DSInfoLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMIC_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_Set_DynamicScaling(void *pInstance, MS_U32 u32DSMemAddr, MS_U32 u32DSMemSize,MS_U32 u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
    return MHAL_SC_Set_DynamicScaling(pInstance, u32DSMemAddr, u32DSMemSize, u32MemBaseAddr, u8MIU_Select, u8IdxDepth, bOP_On, bIPS_On, bIPM_On, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// Set Dynamic Scaling OnOff
/// @param	bOP_On, bIPS_On, IPM_On			   \b IN: the enable bit of Dynamic Scaling
/// @param	eWindow 			   \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetDynamicScaling_OnOff_U2(void* pInstance, MS_BOOL bOP_On,MS_BOOL bIPS_On,MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
	MDrv_XC_Set_DynamicScaling_OPIP_OnOff(pInstance, bOP_On, bIPS_On, bIPM_On, eWindow);
}

void MApi_XC_SetDynamicScaling_OnOff(MS_BOOL bOP_On,MS_BOOL bIPS_On,MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
	if (pu32XCInst == NULL)
	{
		printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
		return;
	}

	stXC_SET_DYNAMIC_SCALING_ONOFF XCArgs;
	XCArgs.eWindow = eWindow;
	XCArgs.bOP_On = bOP_On;
	XCArgs.bIPS_On = bIPS_On;
	XCArgs.bIPM_On = bIPM_On;

	if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMIC_SCALING_ONOFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain XC engine fail\n");
		return;
	}
	else
	{
		return;
	}
}

void MDrv_XC_Set_DynamicScaling_OPIP_OnOff(void *pInstance, MS_BOOL bOP_On,MS_BOOL bIPS_On,MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
	MHAL_SC_Set_DynamiScaling_OPIP_OnOff(pInstance, bOP_On, bIPS_On, bIPM_On, eWindow);
}

void MApi_XC_Set_DynamicScalingFlag_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
		_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
		_XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    MHAL_SC_Set_DynamicScalingFlag(pInstance, bEnable);
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

}

void MApi_XC_Set_DynamicScalingFlag(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {

        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
  		return;

    }

    stXC_SET_DYNAMIC_SCALING_FLAG XCArgs;
    XCArgs.bReturnValue= bEnable;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMIC_SCALING_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }

}

MS_BOOL MApi_XC_Is_SupportSWDS_U2(void *pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    #if ENABLE_SW_DS
        bReturn = TRUE;
    #else
        bReturn = FALSE;
    #endif

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return bReturn;
}

 __attribute__((weak)) MS_BOOL MApi_XC_Is_SupportSWDS(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_Is_SupportSWDS XCArgs;
    XCArgs.bEnable = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_IS_SupportSWDS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)  //E_XC_CMD_GET_SWDSIndex
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.bEnable;
    }

}

void MApi_XC_Set_GstreamerProcess(MS_BOOL bEnable)
{

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }


    if (pu32XC_Kernel_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XC_Kernel_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }

    stXC_SET_GSTREAMER_PROCESS XCArgs;
    XCArgs.bReturnValue = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GSTREAMER_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SET_GSTREAMER_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }


}

void MApi_XC_Set_GstreamerProcess_U2(void* pInstance, MS_BOOL bEnable)
{

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    HAL_SC_Set_IsGstreamerProcess(bEnable);
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

}


MS_BOOL MApi_XC_IsGstreamerProcess(void)
{

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_GSTREAMER_PROCESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_GSTREAMER_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

MS_BOOL MApi_XC_IsGstreamerProcess_U2(void* pInstance)
{

    MS_BOOL bReturn = FALSE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    bReturn = HAL_SC_Get_IsGstreamerProcess();
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return bReturn;

}

void MApi_XC_Set_SeamlessPlay(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }

    stXC_SET_SEAMLESS_PLAY XCArgs;
    XCArgs.bReturnValue= bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SEAMLESS_PLAY, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}

void MApi_XC_Set_SeamlessPlay_U2(void* pInstance, MS_BOOL bEnable)
{
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
	MDrv_XC_Set_SeamlessPlay(bEnable);
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
}

void MDrv_XC_Set_SeamlessPlay(MS_BOOL bEnable)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC.bseamlessPlay= bEnable;
}

MS_BOOL MApi_XC_Get_IsSeamlessPlay(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_SEAMLESS_PLAY XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SEAMLESS_PLAY, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Get_IsSeamlessPlay_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->sthal_SC.bseamlessPlay;
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return bReturn;
}

void MApi_XC_Set_CaptureProcess(MS_BOOL bEnable)
{

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }


    if (pu32XC_Kernel_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XC_Kernel_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }

    stXC_SET_CAPTURE_PROCESS XCArgs;
    XCArgs.bReturnValue = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CAPTURE_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SET_CAPTURE_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }


}

void MApi_XC_Set_CaptureProcess_U2(void* pInstance, MS_BOOL bEnable)
{

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    HAL_SC_Set_IsCaptureProcess(bEnable);
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

}


MS_BOOL MApi_XC_IsCaptureProcess(void)
{

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_CAPTURE_PROCESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CAPTURE_PROCESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

MS_BOOL MApi_XC_IsCaptureProcess_U2(void* pInstance)
{

    MS_BOOL bReturn = FALSE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    bReturn = HAL_SC_Get_IsCaptureProcess();
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return bReturn;

}


#ifdef UFO_XC_SWDS_SW_FILM_ENABLE
E_APIXC_ReturnValue MDrv_XC_Set_DS_ForceI_DeInterlaceMode(void *pInstance, MS_BOOL bPmode,  MS_BOOL bIsTopField, MS_U16 u16FrameRate, SCALER_WIN eWindow)
{
    MHal_XC_Set_DS_ForceI_DeInterlaceMode(pInstance,  bPmode, bIsTopField,  u16FrameRate,  eWindow);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Set_DS_ForceI_DeInterlaceMode_U2(void *pInstance, MS_BOOL bPmode,  MS_BOOL bIsTopField, MS_U16 u16FrameRate, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    eReturn = MDrv_XC_Set_DS_ForceI_DeInterlaceMode(pInstance, bPmode, bIsTopField, u16FrameRate, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eReturn;
}

//-------------------------------------------------------------------------------------------------
/// Set SWDS force I mode film de interlate mode: enable sw manual film for top/bottom detection
/// @param  bPmode            \b IN: current decoded stream progressive/interlace mode
/// @param  bIsTopField       \b IN: current decoded stream top/bottom field
/// @param  u16FrameRate       \b IN: current decoded stream framerate
/// @param  eWindow           \b IN: which window we are going to set
E_APIXC_ReturnValue MApi_XC_Set_DS_ForceI_DeInterlaceMode(MS_BOOL bPmode,  MS_BOOL bIsTopField, MS_U16 u16FrameRate, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_FORCEI_DEIMODE XCArgs;
    XCArgs.bCurrentPmode = bPmode;
    XCArgs.bIsTopField = bIsTopField;
    XCArgs.u16CurrentFramerate = u16FrameRate;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DS_FORCEI_DEIMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
#endif


#undef _MDRV_SC_DYNAMICSCALING_C_
#endif //_MDRV_SC_DYNAMICSCALING_C_
