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
/// file    Mdrv_hdmi.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  MDRV_HDMI_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_hdmi.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_adc.h"
#include "xc_hwreg_utility2.h"
#include "drv_sc_mux.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_if_v2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define msg_hdmi(x) //x
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMI_POLLING_STACK_SIZE         1U
#define HDMI_POLLING_INTERVAL           10U

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
MS_S32 slHDMIPollingTaskID = -1;
MS_U8 ucHDMIPollingStack[HDMI_POLLING_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/// Debug information
static MSIF_Version _drv_hdmi_version = {.DDI = { HDMI_DRV_VERSION },};
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void MDrv_HDMI_ReadEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize);
static void MDrv_HDMI_WriteEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u16EDIDSize);

static void _MDrv_HDMI_PollingTask(void)
{
    while(TRUE)
    {
        Hal_HDMI_StablePolling();

        MsOS_DelayTask(HDMI_POLLING_INTERVAL);
    }
}

MS_BOOL _MDrv_HDMI_CreateTask(void)
{
    slHDMIPollingTaskID = MsOS_CreateTask((TaskEntry) _MDrv_HDMI_PollingTask,
                                     (MS_U32) 0,
                                     E_TASK_PRI_MEDIUM,
                                     TRUE,
                                     (void *)ucHDMIPollingStack,
                                     HDMI_POLLING_STACK_SIZE,
                                     "HDMI polling task");
    
    if(slHDMIPollingTaskID < 0)
    {
        printf("MsOS_CreateTask failed!!\n");
        MsOS_DeleteEventGroup(slHDMIPollingTaskID);
       // MsOS_DeleteMutex(*ps32VdecExMutexId);
        return FALSE;
    }

    return TRUE;
}

void MDrv_HDCP22_InitCBFunc_U2(void* pInstance, HDCP22_Recv_CBF pCBFunc, void* pContext)
{
	XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

	UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
	_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
	_XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

	if ( pCBFunc != NULL )
	{
		XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    	UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

		//Hal_HDCP22_RecvMsgCB = pCBFunc;
		//pDummyCont = pContext;
		pXCResourcePrivate->stdrv_HDMI.Hal_HDCP22_RecvMsgCB = pCBFunc;
		pXCResourcePrivate->stdrv_HDMI.pDummyCont = pContext;
	}


#if SC2_ENABLE
	_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
	_XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}
void MDrv_HDCP22_InitCBFunc(HDCP22_Recv_CBF pCBFunc, void* pContext)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP22_InitCBFunc XCArgs;
    XCArgs.pCBFunc = pCBFunc;
	XCArgs.pContext = pContext;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_INIT_CBFUNC, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDCP22_PortInit_U2(void* pInstance, MS_U8 ucPortIdx)
{
	Hal_HDCP22_PortInit(ucPortIdx);
}
void MDrv_HDCP22_PortInit(MS_U8 ucPortIdx)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP22_PortInit XCArgs;
    XCArgs.ucPortIdx = ucPortIdx;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_PORT_INIT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDCP22_PollingReadDone_U2(void* pInstance, MS_U8 ucPortIdx)
{
	return Hal_HDCP22_PollingReadDone(ucPortIdx);
}
MS_BOOL MDrv_HDCP22_PollingReadDone(MS_U8 ucPortIdx)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDCP22_PollingReadDone XCArgs;
    XCArgs.ucPortIdx = ucPortIdx;
	XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_POLLING_READ_DONE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDCP22_EnableCipher_U2(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
	Hal_HDCP22_EnableCipher(ucPortType, ucPortIdx, bIsEnable);
}
void MDrv_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP22_EnableCipher XCArgs;
    XCArgs.ucPortType = ucPortType;
	XCArgs.ucPortIdx = ucPortIdx;
	XCArgs.bIsEnable = bIsEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_ENABLE_CIPHER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDCP22_SendMsg_U2(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy)
{
	Hal_HDCP22_SendMsg(ucPortType, ucPortIdx, pucData, dwDataLen, pDummy);
}
void MDrv_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP22_SendMsg XCArgs;
	XCArgs.ucPortType = ucPortType;
    XCArgs.ucPortIdx = ucPortIdx;
	XCArgs.pucData = pucData;
	XCArgs.dwDataLen = dwDataLen;
	XCArgs.pDummy = pDummy;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_SEND_MSG, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDCP22_Handler_U2(void* pInstance, MS_U8 ucPortIdx)
{
	Hal_HDCP22_Handler(ucPortIdx);
}
void MDrv_HDCP22_Handler(MS_U8 ucPortIdx)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP22_Handler XCArgs;
	XCArgs.ucPortIdx = ucPortIdx;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_HDCP22_HANDLER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset);

//-------------------------------------------------------------------------------------------------
/// report which patch function hdmi needs
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_Func_Caps_U2(void* pInstance)
{
    return Hal_HDMI_Func_Caps();
}

MS_U16 MDrv_HDMI_Func_Caps(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_FUNC_CAPS XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_FUNC_CAPS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check the HDCP is exist or not
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDCP_isexist(void)
{
    MS_HDCP_STATUS_INFO_t* enStatus_info;
    enStatus_info = MDrv_HDCP_GetStatus();

    if(enStatus_info->b_St_Reserve_1 && enStatus_info->b_St_ENC_En)
    {
        msg_hdmi(printf("HDCP is exit"));
        return TRUE;
    }
    else
    {
        msg_hdmi(printf("No HDCP"));
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------------
/// DDC clock on/off control
/// @param  bEnable      \b IN: ON/OFF control
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDCP_ddc_en(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType), bEnable);
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDCP_ddc_en(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType), bEnable);
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    Hal_HDCP_ddc_en(bEnable);
#endif
}

void MDrv_HDCP_Enable(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// DDC clock on/off control
/// @param  enInputPortType  \b IN: Input source selection
/// @param  bEnable          \b IN: ON/OFF control
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_SetHdcpEnable_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable)
{
    /*Only DVI port can enable/disable hdcp key*/
    if(enInputPortType < INPUT_PORT_DVI0 ||  enInputPortType > INPUT_PORT_DVI3)
    {
        return FALSE;
    }

#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDCP_ddc_en(enInputPortType, bEnable);
#else
    enInputPortType = enInputPortType;
    Hal_HDCP_ddc_en(bEnable);
#endif
    return TRUE;
}

MS_BOOL MDrv_HDMI_SetHdcpEnable(E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SET_HDCPENABLE XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check the ending of the Vsync
/// @param  bStable      \b IN: Stable or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_Vsync_end_en_U2(void* pInstance, MS_BOOL bStable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(bStable)
    {
        if(!pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count)
        {
            pXCResourcePrivate->stdrv_HDMI.u32VsyncTimer = MsOS_GetSystemTime();
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count = TRUE;
        }
        if ((pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done) && ((MsOS_GetSystemTime() - pXCResourcePrivate->stdrv_HDMI.u32VsyncTimer) > 4500) )
        {
            Hal_HDCP_Vsync_end_en(TRUE);
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done = FALSE;
        }
    }
    else
    {
        if (!pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done)
        {
            Hal_HDCP_Vsync_end_en(FALSE);
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done = TRUE;
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count = FALSE;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDCP_Vsync_end_en(MS_BOOL bStable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP_VSYNC_END_EN XCArgs;
    XCArgs.bStable = bStable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_VSYNC_END_EN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Exit the HDMI and turn off the related setting of HDMI
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Exit_U2( void* pInstance )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDMI_exit(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDMI_exit(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    Hal_HDMI_exit();
#endif
#if 0
    if(g_bIMMESWITCH_DVI_POWERSAVING)
        memset(u8PSDVIStableCount, 0, 4*sizeof(MS_U8));
#endif
}

void MDrv_HDMI_Exit( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_EXIT, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


//-------------------------------------------------------------------------------------------------
/// Init the setting for HDMI
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_init_internal(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_HDMI._info.u8SupportPorts = 0;
    pXCResourcePrivate->stdrv_HDMI._status.bIsInitialized = TRUE;
    pXCResourcePrivate->stdrv_HDMI._status.bIsRunning     = TRUE;

    if (SUPPORT_IMMESWITCH)
    {
    	pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH);
    }
    else
    {
    	pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH = FALSE;
    }

    if (SUPPORT_IMMESWITCH_DVI_POWERSAVING)
    {
    	pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH_DVI_POWERSAVING) >> 1;
    }
    else
    {
    	pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING = FALSE;
    }
    #ifdef K3_U2
    if (SUPPORT_DVI_AUTO_EQ)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_DVI_AUTO_EQ) >> 2;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = FALSE;
    }
    #else
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = SUPPORT_DVI_AUTO_EQ;
    #endif

    msg_hdmi(printf("[%s][%d]pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH=%x\n", __FUNCTION__, __LINE__,
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH));
    msg_hdmi(printf("[%s][%d]pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING=%x\n", __FUNCTION__, __LINE__,
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING));

    Hal_HDMI_init(pInstance);

}

void MDrv_HDMI_init_U2( void* pInstance )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_init_internal(pInstance);
    _MDrv_HDMI_CreateTask();
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_HDMI_init_internal(pInstance);
    _MDrv_HDMI_CreateTask();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


#if 0
    if(g_bIMMESWITCH_DVI_POWERSAVING)
        memset(u8PSDVIStableCount, 0, 4*sizeof(MS_U8));
#endif
}

void MDrv_HDMI_init( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_INIT, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the equalizer volumn for HDMI
/// @param  enEq      \b IN: ON/OFF the equlizer
/// @param  u8EQValue      \b IN: The volumn of equalizer
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Set_EQ_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    INPUT_SOURCE_TYPE_t src;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    for(src = INPUT_SOURCE_HDMI; src < INPUT_SOURCE_HDMI_MAX; src++)
    {
        Hal_HDMI_Set_EQ(MDrv_XC_Mux_GetHDMIPort(pInstance, src), enEq, u8EQValue);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    for(src = INPUT_SOURCE_HDMI; src < INPUT_SOURCE_HDMI_MAX; src++)
    {
        Hal_HDMI_Set_EQ(MDrv_XC_Mux_GetHDMIPort(pInstance, src), enEq, u8EQValue);
    }
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    Hal_HDMI_Set_EQ(enEq, u8EQValue);
#endif
}

void MDrv_HDMI_Set_EQ(MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SET_EQ XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_EQ, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the equalizer volumn for HDMI to single port
/// @param  enEq      \b IN: ON/OFF the equlizer
/// @param  u8EQValue      \b IN: The volumn of equalizer
/// @param  enInputPortType      \b IN: The appointed port
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_Set_EQ_To_Port_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_Set_EQ(enInputPortType, enEq, u8EQValue);
    return TRUE;
#else
    printf("[Warning]Unsupport set EQ to single port!");
    return FALSE;
#endif
}

MS_BOOL MDrv_HDMI_Set_EQ_To_Port(MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SET_EQ_TO_PORT XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable the Audio mute event for Vivaldi
/// @param  u16MuteEvent      \b IN: The mute event
/// @param  u16MuteMask       \b IN: The mute mask
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Audio_MUTE_Enable_U2(void* pInstance, MS_U16 u16MuteEvent,MS_U16 u16MuteMask)
{
    Hal_HDMI_Audio_MUTE_Enable(u16MuteEvent, u16MuteMask);
}

void MDrv_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent,MS_U16 u16MuteMask)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_AUDIO_MUTE_ENABLE XCArgs;
    XCArgs.u16MuteEvent = u16MuteEvent;
    XCArgs.u16MuteMask = u16MuteMask;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear the Audio mute status.
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Audio_Status_Clear_U2(void* pInstance)
{
    Hal_HDMI_Audio_Status_Clear();
}

void MDrv_HDMI_Audio_Status_Clear(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Control the Hot Plug Detection
/// in HIGH Level, the voltage level is 2.4 ~ 5.3V
/// in LOW Level,  the voltage level is 0 ~ 0.4V
/// Enable hotplug GPIO[0] output and set output value
/// @param  bHighLow                \b IN: High/Low control
/// @param  enInputPortType      \b IN: Input source selection
/// @param  bInverse                 \b IN: Inverse or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_pullhpd_internal(void *pInstance, MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    msg_hdmi(printf("src hpd = %x \n",enInputPortType));
    // Fix me. Do not use HDMI_PORT_ALL and HDMI_PORT_A.
    // use E_MUX_INPUTPORT.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bHDMIInverseBackUp[enInputPortType - INPUT_PORT_DVI0] = bInverse;

    Hal_HDMI_pullhpd(pInstance,bHighLow, enInputPortType, bInverse);
}

void MDrv_HDMI_pullhpd_U2( void* pInstance, MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_pullhpd_internal(pInstance, bHighLow, enInputPortType, bInverse);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_HDMI_pullhpd_internal(pInstance, bHighLow, enInputPortType, bInverse);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDMI_pullhpd( MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_PULLHPD XCArgs;
    XCArgs.bHighLow = bHighLow;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bInverse = bInverse;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PULLHPD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// HDMI G control information
/// @param  gcontrol                \b IN: G control setting
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_GC_Info_U2(void* pInstance, HDMI_GControl_INFO_t gcontrol)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16Return = Hal_HDMI_gcontrol_info(gcontrol);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u16Return = Hal_HDMI_gcontrol_info(gcontrol);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u16Return;
}

MS_U16 MDrv_HDMI_GC_Info(HDMI_GControl_INFO_t gcontrol)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GC_INFO XCArgs;
    XCArgs.gcontrol = gcontrol;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GC_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// HDMI PLL control setting 1
/// @param  pllctrl                \b IN: PLL control
/// @param  bread                \b IN: Read/Write (1/0)
/// @param  u16value           \b IN: Write value
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    return (Hal_HDMI_pll_ctrl1(pllctrl, bread, u16value));
}
//-------------------------------------------------------------------------------------------------
/// HDMI PLL control setting 2
/// @param  pllctrl                \b IN: PLL control
/// @param  bread                \b IN: Read/Write (1/0)
/// @param  u16value           \b IN: Write value
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    return (Hal_HDMI_pll_ctrl2(pllctrl, bread, u16value));
}

////////////////////////


/******************************************************************************/
/// for DVI
/******************************************************************************/
//-------------------------------------------------------------------------------------------------
/// Check the clock is less 70MHz or not with DVI source
/// @param  enInputPortType                \b IN: Input source
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz_U2( void* pInstance, E_MUX_INPUTPORT enInputPortType )
{
    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
            return Hal_DVI_clk_lessthan70mhz_usingportc(FALSE);
        case INPUT_PORT_DVI2:
            return Hal_DVI_clk_lessthan70mhz_usingportc(TRUE);
    }
}

MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz( E_MUX_INPUTPORT enInputPortType )
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SETFORCLOCKLESSTHAN70MHZ XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_DVI_SoftwareReset_U2( void* pInstance, MS_U16 u16Reset )
{
    Hal_DVI_sw_reset(u16Reset);

}

void MDrv_DVI_SoftwareReset( MS_U16 u16Reset )
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_SOFTWARERESET XCArgs;
    XCArgs.u16Reset = u16Reset;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_DVI_IF_CMD_SOFTWARERESET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Detect the the clock is lose or not from ADC DTOP
/// @param  enInputPortType                \b IN: Input source
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_dvi_clock_detect(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bIsLoss;

#if SUPPORT_HDMI_RX_NEW_FEATURE
    bIsLoss = Hal_DVI_clklose_det(enInputPortType);
#else
    if (enInputPortType == INPUT_PORT_DVI2)
    {
        bIsLoss = Hal_DVI_clklose_det(TRUE);
    }
    else
    {
        bIsLoss = Hal_DVI_clklose_det(FALSE);
    }
#endif

    return bIsLoss;
}

//-------------------------------------------------------------------------------------------------
/// Reset the DVI setting
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_reset_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    INPUT_SOURCE_TYPE_t enInputSourceType = INPUT_SOURCE_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        enInputSourceType = gSrcInfo[MAIN_WINDOW].enInputSourceType;
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        enInputSourceType = gSrcInfo[SUB_WINDOW].enInputSourceType;
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return;
    }
    if(Hal_DVI_irq_info(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), IRQ_DVI_CK_CHG) && (!pXCResourcePrivate->stdrv_HDMI._ResetDone))
    {
        MS_U16 u8_IRQ_STATUS;

        u8_IRQ_STATUS = Hal_DVI_irq_info(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), IRQ_DVI_CK_CHG);
        Hal_DVI_irq_clear(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), u8_IRQ_STATUS);
        Hal_DVI_irq_clear(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), ~IRQ_ALL_BIT);

        if (u8_IRQ_STATUS == IRQ_DVI_CK_CHG)
        {
            // not do DVI software reset at here; Avoid the snow image

            pXCResourcePrivate->stdrv_HDMI._ResetDone = TRUE;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(Hal_DVI_irq_info(IRQ_DVI_CK_CHG) && (!pXCResourcePrivate->stdrv_HDMI._ResetDone))
    {
        MS_U16 u8_IRQ_STATUS;

        u8_IRQ_STATUS = Hal_DVI_irq_info(IRQ_DVI_CK_CHG);
        Hal_DVI_irq_clear(u8_IRQ_STATUS);
        Hal_DVI_irq_clear(~IRQ_ALL_BIT);

        if (u8_IRQ_STATUS == IRQ_DVI_CK_CHG)
        {
            // not do DVI software reset at here; Avoid the snow image

            pXCResourcePrivate->stdrv_HDMI._ResetDone = TRUE;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
}

void mdrv_dvi_reset(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_RESET, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifdef DVI_NEW_Patch
// 20081226 - DVI+HDCP snow noise patch - start ...
//-------------------------------------------------------------------------------------------------
/// avoid the snow image while AC on/off and source change for DVI 70Mhz issue, setting 1
/// @param  enInputPortType                \b IN: Input source
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_clock_70mhz_swpatch1(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    MS_BOOL bClockLess70MHz;
    E_MUX_INPUTPORT enMuxPort =MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);

//    if(MDrv_ReadByte( H( REG_HDMI_INT_STATUS) ) & BIT5)
    if(R2BYTEMSK(REG_HDMI_61_L, BIT13))
    {
        #if ( WATCH_DOG == ENABLE )
            MDrv_Sys_ClearWatchDog();
        #endif

        switch(enMuxPort)
        {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
//            if(!(MDrv_ReadByte(H_BK_ADC_DTOP(0x57)) & BIT3)) // DVI clock is stable
            if(!Hal_DVI_clk_stable_usingportc(NO)) // DVI clock is stable
            {
                //printf("\r\nAaron - HDCP status = %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x79)));
//                if(!(R2BYTEMSK(0x2C33, BIT5))) // Video clock is unstable
                if(!(R2BYTEMSK(0x2C32, BIT13))) // Video clock is unstable
                {
                    g_DVI70MhzUnstableCounter++;
                    if(g_DVI70MhzUnstableCounter >= 2)
                    {
                        g_DVI70MhzUnstableCounter = 0;
                        //bClockLess70MHz = (MDrv_ReadByte(H_BK_ADC_DTOP(0x57)) & BIT2) > 0 ? 1 : 0;
                        bClockLess70MHz = Hal_DVI_clk_lessthan70mhz_usingportc(NO); //Not Port C
                        //printf("\r\nAaron - CLK_CNT = %bx, AF= %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x57)), MDrv_ReadByte(H_BK_ADC_DTOP(0x57)));
                        //printf("\r\nAaron - VCO= %bx\r\n", MDrv_ReadByte(L_BK_ADC_ATOP(0x68)));

                        if (g_bDVILess70MHz != bClockLess70MHz)
                        {
                            //printf("\r\nAaron - SWP1  CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
                            g_bDVILess70MHz = bClockLess70MHz;
                            MDrv_HDMI_dvi_adjust(bClockLess70MHz);

                        }
                    }
                }
                g_DVI70MhzPollingCounter = 20; // 20 msec
            }
            break;

        case INPUT_PORT_DVI2:

//            if(!(MDrv_ReadByte(H_BK_ADC_DTOP(0x58)) & BIT3)) // DVI clock is stable
            if(!Hal_DVI_clk_stable_usingportc(YES)) // DVI clock is stable
            {
//                if(!(R2BYTEMSK(0x2C33, BIT5))) // Video clock is unstable
                if(!(R2BYTEMSK(0x2C32, BIT13))) // Video clock is unstable
                {
                    g_DVI70MhzUnstableCounter++;
                    if(g_DVI70MhzUnstableCounter >= 2)
                    {
                        g_DVI70MhzUnstableCounter = 0;

                        //bClockLess70MHz = (MDrv_ReadByte(H_BK_ADC_DTOP(0x58)) & BIT2) > 0 ? 1 : 0;
                        bClockLess70MHz = Hal_DVI_clk_lessthan70mhz_usingportc(YES); //Is Port C
                        //printf("\r\nAaron - CLK_CNT = %bx, AF= %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x58)), MDrv_ReadByte(H_BK_ADC_DTOP(0x58)));
                        //printf("\r\nAaron - VCO= %bx\r\n", MDrv_ReadByte(L_BK_ADC_ATOP(0x68)));

                        if (g_bDVILess70MHz != bClockLess70MHz)
                        {
                            //printf("\r\nAaron - CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
                            g_bDVILess70MHz = bClockLess70MHz;
                            MDrv_HDMI_dvi_adjust(bClockLess70MHz);

                        }
                    }
                }
                g_DVI70MhzPollingCounter = 20; // 20 msec
            }
            break;
        }

        // clear interrupt status
        W2BYTEMSK(REG_HDMI_63_L, BIT13, BIT13);  //REG_HDMI_INT_CLEAR
        W2BYTEMSK(REG_HDMI_63_L,     0, BIT13);  //REG_HDMI_INT_CLEAR
        //printf("\r\nAaron- DVICLOCK_PATH = %d\r\n", DVICLOCK_PATH);
    }
}

//-------------------------------------------------------------------------------------------------
/// avoid the snow image while AC on/off and source change for DVI 70Mhz issue, setting 2
/// @param  enInputPortType                \b IN: Input source
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_clock_70mhz_swpatch2(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType) //NO_USE
{
    MS_BOOL bClockLess70MHz;
    static MS_U16 g_DVITMDSClock = 0;
    MS_U16 u16DVITMDSClock, u16DVIClockDiff;
    E_MUX_INPUTPORT enMuxPort =MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);

//    if(R2BYTEMSK(0x2C33, BIT5)) // Video clock is stable
    if(R2BYTEMSK(0x2C32, BIT13)) // Video clock is stable
    {
        g_DVI70MhzUnstableCounter = 0;
        g_DVI70MhzPollingCounter = 0;

        #if ( WATCH_DOG == ENABLE )
            //msAPI_Timer_ResetWDT();
            MDrv_Sys_ClearWatchDog();
        #endif

        switch(enMuxPort)
        {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
            if(Hal_DVI_clk_stable_usingportc(NO)) // input clock is not really stable
            {
                //printf("\r\nAaron - False alarm, SWP2 TMDS Clock= %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF);
                return;
            }
            break;

        case INPUT_PORT_DVI2:
            if(Hal_DVI_clk_stable_usingportc(YES)) // input clock is not really stable
            {
                //printf("\r\nAaron - False alarm, SWP2 TMDS Clock= %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF);
                return;
            }
            break;
        }

//        u16DVITMDSClock = MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF;
        u16DVITMDSClock = R2BYTE(REG_HDMI_11_L) & 0x1FFF;
        u16DVIClockDiff = (g_DVITMDSClock >= u16DVITMDSClock) ? (g_DVITMDSClock - u16DVITMDSClock) : (u16DVITMDSClock - g_DVITMDSClock);
        if(u16DVIClockDiff > 10) // tolerance = 1MHz
        {
            //printf("\r\nAaron - SWP2 Clock_Old = %d <-> Clock_New = %d \r\n", g_DVITMDSClock, u16DVITMDSClock);
            g_DVITMDSClock = u16DVITMDSClock;
            g_DVI70MhzPollingCounter = 20; // 20 msec
            return;
        }
//        bClockLess70MHz = (MDrv_ReadByte(H(__REG_HDMI(0x11))) & 0x60) > 0 ? 0 : 1;
        bClockLess70MHz = (R2BYTEMSK(REG_HDMI_11_L, HBMASK) & 0x60) > 0 ? 0 : 1;
     //printf("\r\nAaron - HDCP status = %bx, INT timer= %d\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x79)), MDrv_ReadByte(H(__REG_HDMI(0x09))));
     //printf("\r\nAaron - SWP2 CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
     //printf("\r\nAaron - SWP2 TMDS Clock = %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF );
        if (g_bDVILess70MHz != bClockLess70MHz)
        {
            g_bDVILess70MHz = bClockLess70MHz;
            MDrv_HDMI_dvi_adjust(bClockLess70MHz);
        }
    }
}
// 20081226 - DVI+HDCP snow noise patch - end ...
#endif
//-------------------------------------------------------------------------------------------------
/// Get/Set error status
/// @param  u8value                \b IN: Write value
/// @param  bread                   \b IN: Read/Write (1/0)
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_err_status_update_U2(void* pInstance, MS_U8 u8value, MS_BOOL bread)
{
   MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
   _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
   u8Return = Hal_HDMI_err_status_update( u8value,  bread);
   _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
   _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
   u8Return = Hal_HDMI_err_status_update( u8value,  bread);
   _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

   return u8Return;
}

MS_U8 MDrv_HDMI_err_status_update(MS_U8 u8value, MS_BOOL bread)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_ERR_STATUS_UPDATE XCArgs;
    XCArgs.u8value = u8value;
    XCArgs.bread = bread;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U16 MDrv_HDMI_GetTMDSFreq_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    return Hal_HDMI_GetTMDSFreq();
#else
    return 0;
#endif
}

MS_U16 MDrv_HDMI_GetTMDSFreq(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_TMDS_FREQ XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_TMDS_FREQ, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// HDMI packet reset
/// @param  breset                \b IN: Enabl reset or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_pkt_reset_internal(void *pInstance, HDMI_REST_t breset )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDMI_pkt_reset(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType), breset);
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDMI_pkt_reset(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType), breset);
    }
#else
    Hal_HDMI_pkt_reset(breset);
#endif

#if SUPPORT_HDMI_RX_NEW_FEATURE
    //pkt reset cannot clear status in DVI mode  in non-immeswitch case, RD suggest to use DVI sw reset
    if(!pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH) // Only apply in non-imme switch case
    {
        if(breset & REST_HDMI_STATUS )
        {
            Hal_DVI_Software_Reset(INPUT_PORT_DVI0, BIT(6));
        }
    }
#else
    // Do nothing
#endif
}

void MDrv_HDMI_pkt_reset_U2( void* pInstance, HDMI_REST_t breset )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_pkt_reset_internal(pInstance, breset);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_HDMI_pkt_reset_internal(pInstance, breset);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDMI_pkt_reset( HDMI_REST_t breset )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_PKT_RESET XCArgs;
    XCArgs.breset = breset;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PKT_RESET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable Hot plug Detection out enable
/// @param  benable                \b IN: Enabl output or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_hpd_en(MS_BOOL benable)
{
    Hal_HDMI_hpd_en(benable);
}

void MDrv_HDCP_initproductionkey_internal(void *pInstance, MS_U8 * pu8HdcpKeyData )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp, pu8HdcpKeyData, HDCP_KEY_LENGTH * sizeof(MS_U8));// HDCP key length is 289 byte
    Hal_HDCP_initproductionkey(pu8HdcpKeyData);
}

//-------------------------------------------------------------------------------------------------
/// Init the HDCP production key
/// @param  pu8HdcpKeyData                \b IN: HDCP key
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_initproductionkey_U2( void* pInstance, MS_U8 * pu8HdcpKeyData )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDCP_initproductionkey_internal(pInstance, pu8HdcpKeyData);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_HDCP_initproductionkey_internal(pInstance, pu8HdcpKeyData);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData )
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP_INITPRODUCTIONKEY XCArgs;
    XCArgs.pu8HdcpKeyData = pu8HdcpKeyData;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear the HDCP Flag
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_ClearStatus_U2( void* pInstance )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDCP_clearflag(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDCP_clearflag(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    Hal_HDCP_clearflag();
#endif
}

void MDrv_HDCP_ClearStatus( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_CLEARSTATUS, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable audio output or not
/// @param  bEnable                \b IN: Enable or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_audio_output_U2( void* pInstance, MS_BOOL bEnable )
{
    Hal_HDMI_audio_output(bEnable);
}

void MDrv_HDMI_audio_output( MS_BOOL bEnable )
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_AUDIO_OUTPUT XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AUDIO_OUTPUT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI packet info
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_packet_info(void)
{
    return Hal_HDMI_packet_info();
}
//-------------------------------------------------------------------------------------------------
/// audio content protection info
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_cp_hdr_info_U2(void* pInstance)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u8Return = Hal_HDMI_audio_content_protection_info();
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u8Return = Hal_HDMI_audio_content_protection_info();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u8Return;
}

MS_U8 MDrv_HDMI_audio_cp_hdr_info(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_AUDIO_CP_HDR_INFO XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the AVI infoframe information
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_avi_infoframe_info(MS_U8 u8byte)
{
    return Hal_HDMI_avi_infoframe_info(u8byte);
}

//-------------------------------------------------------------------------------------------------
/// Get manufactor ID information(0x47~0x53)
/// @param  u8byte *               \b IN: byte info selection
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Get_MID_info_U2(void* pInstance, MS_U8 * pu8ManufactorData)
{
    MS_U8 i=0;
//    pkt_value.PKT_SPD_VALUE = u16tmp[0] & BIT(2);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    Hal_HDMI_packet_ctrl(0x04); //set SPD

    for(i=0; i<25; i++)
    {
        (pu8ManufactorData[i])=Hal_HDMI_avi_infoframe_info(i+2*(47-40)+1);
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDMI_Get_MID_info(MS_U8 * pu8ManufactorData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_GET_MID_INFO XCArgs;
    XCArgs.pu8ManufactorData = pu8ManufactorData;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_MID_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI packet value
/// @param  u8state                \b IN: MS_HDMI_PACKET_STATE_t
/// @param  u8byte_idx                \b IN: Byte index
/// @param  *pValue                \b IN: packet value
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_get_packet_value_U2(void* pInstance, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = Hal_HDMI_get_packet_value( u8state, u8byte_idx, pValue);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = Hal_HDMI_get_packet_value( u8state, u8byte_idx, pValue);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MDrv_HDMI_get_packet_value(MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_GET_PACKET_VALUE XCArgs;
    XCArgs.u8state = u8state;
    XCArgs.u8byte_idx = u8byte_idx;
    XCArgs.pValue = pValue;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_PACKET_VALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the audio channel status
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_channel_status_U2(void* pInstance, MS_U8 u8byte)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u8Return = Hal_HDMI_audio_channel_status(u8byte);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u8Return = Hal_HDMI_audio_channel_status(u8byte);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u8Return;
}

MS_U8 MDrv_HDMI_audio_channel_status(MS_U8 u8byte)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_AUDIO_CHANNEL_STATUS XCArgs;
    XCArgs.u8byte = u8byte;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDCP status
/// @return @ref MS_HDCP_STATUS_INFO_t
//-------------------------------------------------------------------------------------------------
static MS_HDCP_STATUS_INFO_t* _MDrv_HDCP_GetStatus(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if SUPPORT_HDMI_RX_NEW_FEATURE
    MS_U16 u16Status = 0;
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        u16Status = Hal_HDCP_getstatus(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        u16Status = Hal_HDCP_getstatus(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
    }
#else
    MS_U16 u16Status = Hal_HDCP_getstatus();
#endif

    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDMI_Mode = u16Status & BIT(0);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_Reserve_1 = u16Status & BIT(1);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDCP_Ver = u16Status & BIT(2);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_AVMute = u16Status & BIT(3);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_ENC_En = u16Status & BIT(4);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_AKsv_Rcv = u16Status & BIT(5);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_ENC_Dis = u16Status & BIT(6);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_Reserve_7 = u16Status & BIT(7);

    return &(pXCResourcePrivate->stdrv_HDMI.enStatus_info);
}

MS_HDCP_STATUS_INFO_t* MDrv_HDCP_GetStatus_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_HDCP_STATUS_INFO_t* pstReturnInfo = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    pstReturnInfo = _MDrv_HDCP_GetStatus(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    pstReturnInfo = _MDrv_HDCP_GetStatus(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return pstReturnInfo;
}

MS_HDCP_STATUS_INFO_t* MDrv_HDCP_GetStatus(void )
{
    stHDCP_GETSTATUS XCArgs;
    memset(&XCArgs, 0, sizeof(stHDCP_GETSTATUS));

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.stReturnValue;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.stReturnValue;
    }
    else
    {
        return XCArgs.stReturnValue;
    }
}

void MDrv_DVI_HF_adjust_internal(void)
{
    Hal_DVI_HF_adjust();
}

void MDrv_DVI_HF_adjust_U2( void* pInstance )
{
    MDrv_DVI_HF_adjust_internal();
}

void MDrv_DVI_HF_adjust(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_DVI_HF_ADJUST, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Returne Phase status
/// @param  bClockLessThan70MHz                \b IN: u8Channel
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_DVI_ChannelPhaseStatus_U2(void* pInstance, MS_DVI_CHANNEL_TYPE u8Channel)
{
    return Hal_DVI_ChannelPhaseStatus(u8Channel);
}

MS_U16 MDrv_DVI_ChannelPhaseStatus( MS_DVI_CHANNEL_TYPE u8Channel)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stDVI_CHANNELPHASESTATUS XCArgs;
    XCArgs.u8Channel = u8Channel;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_DVI_IF_CMD_CHANNELPHASESTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check DVI accumulator and reset status.
/// @param
/// Due to DVI accumulator may be overflow when previous source is MHL,
/// this function will monitor MHL port's accumulator, and reset when it hits boundary.
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_Accumulator_Monitor(void)
{
    Hal_DVI_Accumulator_Monitor();
}

//-------------------------------------------------------------------------------------------------
/// Adjust the ADC for DVI by the condition is less than 70Mhz or not
/// @param  bClockLessThan70MHz                \b IN: Is Less 70Mhz or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_dvi_adjust_U2( void* pInstance, MS_BOOL bClockLessThan70MHz)
{
    Hal_DVI_adc_adjust(bClockLessThan70MHz);
}

void MDrv_HDMI_dvi_adjust( MS_BOOL bClockLessThan70MHz)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_DVI_ADJUST XCArgs;
    XCArgs.bClockLessThan70MHz = bClockLessThan70MHz;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_DVI_ADJUST, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Packet control
/// @param  u16pkt                \b IN: Packet info
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_packet_ctrl(MS_U16 u16pkt)
{
    Hal_HDMI_packet_ctrl(u16pkt);
}
//-------------------------------------------------------------------------------------------------
/// audio content payload info
/// @param  u8byte                \b IN: Byte number
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_cp_data_info(MS_U8 u8byte)
{
    return Hal_HDMI_audio_payload_info(u8byte);
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI packet
/// @param  bEnable                \b IN: Enable receive or not
/// @return @ref MS_HDMI_PACKET_VALUE_t
//-------------------------------------------------------------------------------------------------
MS_HDMI_PACKET_VALUE_t MDrv_HDMI_Packet_Received_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16tmp[1];
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_HDMI_PACKET_VALUE_t enReturn;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if(bEnable)
    {
        u16tmp[0] = MDrv_HDMI_packet_info();
        //Need to write again to clear the packet flag
        MDrv_HDMI_packet_ctrl(u16tmp[0]);
    }
    else
    {
        u16tmp[0] = 0;
    }

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_MPEG_VALUE = u16tmp[0] & BIT(0);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_AUI_VALUE = u16tmp[0] & BIT(1);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_SPD_VALUE = u16tmp[0] & BIT(2);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_AVI_VALUE = u16tmp[0] & BIT(3);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_GC_VALUE = u16tmp[0] & BIT(4);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ASAMPLE_VALUE = u16tmp[0] & BIT(5);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ACR_VALUE = u16tmp[0] & BIT(6);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE = u16tmp[0] & BIT(7);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_NULL_VALUE = u16tmp[0] & BIT(8);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ISRC2_VALUE = u16tmp[0] & BIT(9);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ISRC1_VALUE = u16tmp[0] & BIT(10);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ACP_VALUE = u16tmp[0] & BIT(11);
    pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_ONEBIT_AUD_VALUE = u16tmp[0] & BIT(12);

    enReturn = pXCResourcePrivate->stdrv_HDMI.pkt_value;

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return enReturn;
}

MS_HDMI_PACKET_VALUE_t MDrv_HDMI_Packet_Received(MS_BOOL bEnable)
{
    stHDMI_PACKET_RECEIVED XCArgs;
    XCArgs.bEnable = bEnable;
    memset(&XCArgs.stReturnValue, 0, sizeof(MS_HDMI_PACKET_VALUE_t));

    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.stReturnValue;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_PACKET_RECEIVED, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.stReturnValue;
    }
    else
    {
        return XCArgs.stReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI color format
/// @return @ref MS_HDMI_COLOR_FORMAT
//-------------------------------------------------------------------------------------------------
MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat_U2(void* pInstance)
{
    MS_HDMI_COLOR_FORMAT pkt_ColorFormat=MS_HDMI_COLOR_UNKNOWN;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    switch (  MDrv_HDMI_avi_infoframe_info(_BYTE_1)& 0x60 )
    {
        case 0x00:
            pkt_ColorFormat = MS_HDMI_COLOR_RGB;
            break;
        case 0x40:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_444;
            break;
        case 0x20:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_422;
            break;
        case 0x60:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_420;
            break;
        default:
            break;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return pkt_ColorFormat;
}

MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return MS_HDMI_COLOR_UNKNOWN;
    }

    stHDMI_GET_COLORFORMAT XCArgs;
    XCArgs.eReturnValue = MS_HDMI_COLOR_UNKNOWN;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_COLORFORMAT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_COLOR_UNKNOWN;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI color range
/// @return @ref EN_HDMI_COLOR_RANGE
//-------------------------------------------------------------------------------------------------
EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange_U2(void* pInstance)
{
    EN_HDMI_COLOR_RANGE enPKTColorRange = E_HDMI_COLOR_RANGE_DEFAULT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    switch (MDrv_HDMI_avi_infoframe_info(_BYTE_3) & 0x0C)
    {
        case 0x04:
            enPKTColorRange = E_HDMI_COLOR_RANGE_LIMIT;
            break;
        case 0x08:
            enPKTColorRange = E_HDMI_COLOR_RANGE_FULL;
            break;
        case 0x0C:
            enPKTColorRange = E_HDMI_COLOR_RANGE_RESERVED;
            break;
        case 0x00:
        default:
            break;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return enPKTColorRange;
}

EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_COLOR_RANGE_RESERVED;
    }

    stHDMI_GET_COLORRANGE XCArgs;
    XCArgs.eReturnValue = E_HDMI_COLOR_RANGE_RESERVED;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_COLORRANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_COLOR_RANGE_RESERVED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI aspect ratio info
/// @param  bEn_PAR                \b IN: Enable to get the Picture Aspect Ratio
/// @param  bEn_AFAR              \b IN: Enable to get the Active Format Aspect Ratio
/// @return @ref MS_HDMI_AR_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio_U2(void* pInstance, MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR)
{
    MS_HDMI_AR_TYPE enPar_type;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_U8 u8AR = MDrv_HDMI_avi_infoframe_info(_BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MS_U8 u8AR = MDrv_HDMI_avi_infoframe_info(_BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    // Picture Aspect Ratio - PAR
    if(bEn_PAR)
    {
        switch(u8AR & HDMI_AR_MASK)
        {
            case 0x00:     // IF0[13..12] 00
                enPar_type = HDMI_Pic_AR_NODATA;
                break;
            case 0x10:     // IF0[13..12] 01, 4:3
                enPar_type = HDMI_Pic_AR_4x3;
                break;
            case 0x20:     // IF0[13..12] 10, 16:9
                enPar_type = HDMI_Pic_AR_16x9;
                break;
            case 0x30:     // IF0[13..12] 11, reserved
            default:
                enPar_type = HDMI_Pic_AR_RSV;
                break;
        }
    }
    else
    {
        enPar_type = HDMI_Pic_AR_RSV;
    }

    return enPar_type;
}

MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio(MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return HDMI_AF_AR_Reserve_0;
    }

    stHDMI_GET_ASPECTRATIO XCArgs;
    XCArgs.bEn_PAR = bEn_PAR;
    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_AF_AR_Reserve_0;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
//-------------------------------------------------------------------------------------------------
/// Get the HDMI aspect ratio info
/// @param  bEn_AFAR              \b IN: Enable to get the Active Format Aspect Ratio
/// @return @ref MS_HDMI_AR_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2(void* pInstance, MS_BOOL bEn_AFAR)
{
    MS_HDMI_AR_TYPE enAfar_types;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_U8 u8AR = MDrv_HDMI_avi_infoframe_info(_BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MS_U8 u8AR = MDrv_HDMI_avi_infoframe_info(_BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    // Active Format Aspect Ratio - AFAR
    if(bEn_AFAR)
    {
        switch(u8AR & HDMI_AFAR_MASK)
        {
            case 0x02:    // IF0[11..8] 0010, box 16:9 (top)
                enAfar_types = HDMI_AF_AR_16x9_Top;
                break;
            case 0x03:    // IF0[11..8] 0011, box 14:9 (top)
                enAfar_types = HDMI_AF_AR_14x9_Top;
                break;
            case 0x04:    // IF0[11..8] 0100, box >16:9 (centre)
                enAfar_types = HDMI_AF_AR_GT_16x9;
                break;
            case 0x08:    // IF0[11..8] 1000, same as picture
                enAfar_types = HDMI_AF_AR_SAME;
                break;
            case 0x09:    // IF0[11..8] 1001, 4:3 Center
                enAfar_types = HDMI_AF_AR_4x3_C;
                break;
            case 0x0A:    // IF0[11..8] 1010, 16:9 Center
                enAfar_types = HDMI_AF_AR_16x9_C;
                break;
            case 0x0B:    // IF0[11..8] 1011, 14:9 Center
                enAfar_types = HDMI_AF_AR_14x9_C;
                break;
            case 0x0D:    // IF0[11..8] 1101, 4:3 with shoot and protect 14:9 centre
                enAfar_types = HDMI_AF_AR_4x3_with_14x9_C;
                break;
            case 0x0E:    // IF0[11..8] 1110, 16:9 with shoot and protect 14:9 centre
                enAfar_types = HDMI_AF_AR_16x9_with_14x9_C;
                break;
            case 0x0F:    // IF0[11..8] 1111, 16:9 with shoot and protect 4:3 centre.
                enAfar_types = HDMI_AF_AR_16x9_with_4x3_C;
                break;

            default:
                enAfar_types = HDMI_AF_AR_SAME;
                break;
        }
    }
    else
    {
        enAfar_types = HDMI_AF_AR_SAME;
    }

    return enAfar_types;
}

MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio(MS_BOOL bEn_AFAR)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return HDMI_AF_AR_Reserve_0;
    }

    stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO XCArgs;
    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_AF_AR_Reserve_0;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the content type reported from player
/// @return @ref MS_HDMI_CONTENT_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type_U2(void* pInstance)
{
    MS_HDMI_CONTENT_TYPE enCN = MS_HDMI_CONTENT_NoData;
    MS_U8 u8Temp = 0;
    MS_BOOL bITC = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bITC = MDrv_HDMI_avi_infoframe_info(_BYTE_3)& 0x80;
    u8Temp = MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x30;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bITC = MDrv_HDMI_avi_infoframe_info(_BYTE_3)& 0x80;
    u8Temp = MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x30;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    switch (u8Temp)
    {
        case 0x00:
            if(bITC)
                enCN = MS_HDMI_CONTENT_Graphics;
            else
                enCN = MS_HDMI_CONTENT_NoData;
            break;
        case 0x10:
            enCN = MS_HDMI_CONTENT_Photo;
            break;
        case 0x20:
            enCN = MS_HDMI_CONTENT_Cinema;
            break;
        case 0x30:
            enCN = MS_HDMI_CONTENT_Game;
            break;
        default:
            enCN = MS_HDMI_CONTENT_NoData;
            break;
    }

    return enCN;
}

MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_CONTENT_TYPE XCArgs;
    XCArgs.enReturnValue = MS_HDMI_CONTENT_NoData;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_CONTENT_NoData;
    }
    else
    {
        return XCArgs.enReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the Pixel repetition from player
/// @return @ref MS_HDMI_COLOR_FORMAT
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_Get_Pixel_Repetition_U2(void* pInstance)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u8Return = MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x0F;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u8Return = MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x0F;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u8Return;
}

MS_U8 MDrv_HDMI_Get_Pixel_Repetition(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_PIXEL_REPETITION XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the AVI InfoFrame version
/// @return @ref EN_AVI_INFOFRAME_VERSION
//-------------------------------------------------------------------------------------------------
EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer_U2(void* pInstance)
{
    EN_AVI_INFOFRAME_VERSION enPktVersion= E_AVI_INFOFRAME_VERSION_NON;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    enPktVersion = Hal_HDMI_avi_infoframe_info_ver();

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return enPktVersion;
}

EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_AVI_INFOFRAME_VERSION_NON;
    }

    stHDMI_GET_AVIINFOFRAMEVER XCArgs;
    XCArgs.eReturnValue = E_AVI_INFOFRAME_VERSION_NON;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_AVI_INFOFRAME_VERSION_NON;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the active information present from AVI
/// @return
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    if (BIT(4) == (MDrv_HDMI_avi_infoframe_info(_BYTE_1) & BIT(4)) )
        bReturn = TRUE;
    else
        bReturn = FALSE;

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Pull DVI Clock to low
/// @param  bPullLow                          \b IN: Set DVI clock to low
/// @param  enInputPortType              \b IN: Input source
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_ClkPullLow_U2(void* pInstance, MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
    Hal_DVI_ClkPullLow(bPullLow, enInputPortType);
}

void MDrv_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_CLKPULLLOW XCArgs;
    XCArgs.bPullLow = bPullLow;
    XCArgs.enInputPortType = enInputPortType;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_DVI_IF_CMD_CLKPULLLOW, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Switch DVI source
/// @param  enInputPortType              \b IN: Input source
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_SwitchSrc_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    Hal_DVI_SwitchSrc(enInputPortType);
}

void MDrv_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_SWITCHSRC XCArgs;
    XCArgs.enInputPortType = enInputPortType;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_DVI_IF_CMD_SWITCHSRC, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_DVI_IMMESWITCH_PS_SW_Path(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH) // Only apply in imme switch case
    {
        Hal_DVI_IMMESWITCH_PS_SW_Path();
    }
}

/// lib version
HDMI_Ret_Value MDrv_HDMI_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_HDMI_RET_FAIL;
    }

    *ppVersion = &_drv_hdmi_version;
    return E_HDMI_RET_OK;
}

HDMI_Ret_Value MDrv_HDMI_GetLibVer(const MSIF_Version **ppVersion)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETLIBVER XCArgs;
    XCArgs.ppVersion = ppVersion;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETLIBVER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// lib information
HDMI_Ret_Value  MDrv_HDMI_GetInfo_U2(void* pInstance, MS_HDMI_INFO *pInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    memcpy((void*)pInfo, (void*)&(pXCResourcePrivate->stdrv_HDMI._info), sizeof(MS_HDMI_INFO));

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE



    return E_HDMI_RET_OK;
}

HDMI_Ret_Value  MDrv_HDMI_GetInfo(MS_HDMI_INFO *pInfo)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETINFO XCArgs;
    XCArgs.pInfo = pInfo;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GETINFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// curretn status
HDMI_Ret_Value  MDrv_HDMI_GetStatus_U2(void* pInstance, MS_HDMI_Status *pStatus)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy((void*)pStatus, (void*)&(pXCResourcePrivate->stdrv_HDMI._status), sizeof(MS_HDMI_Status));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy((void*)pStatus, (void*)&(pXCResourcePrivate->stdrv_HDMI._status), sizeof(MS_HDMI_Status));
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return E_HDMI_RET_OK;
}

HDMI_Ret_Value  MDrv_HDMI_GetStatus(MS_HDMI_Status *pStatus)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETSTATUS XCArgs;
    XCArgs.pStatus = pStatus;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// DDC
static void MDrv_HDMI_ReadEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize)
{
    MS_U16 i;

    HAL_HDMI_DDCRam_SelectPort(eDDCRamType);

    for( i = 0; i < u8EDIDSize; i++)
    {
        if(eDDCRamType == E_XC_PROG_VGA_EDID)
        {
            DDC_WRITE_MASK(REG_DDC_A_ACCESS,REG_DDC_A_ACCESS_BITS(1),REG_DDC_A_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEADDR,REG_DDC_A_WRITEADDR_BITS(i),REG_DDC_A_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_A_READPULSE,REG_DDC_A_READPULSE_BITS(1),REG_DDC_A_READPULSE_MASK);
            MsOS_DelayTaskUs(2);
            *(u8EDID + i) = (MS_U8)(DDC_READ_MASK(REG_DDC_A_READDATA, REG_DDC_A_READDATA_MASK)>>8);
        }
        else
        {
            DDC_WRITE_MASK(REG_DDC_D_ACCESS,REG_DDC_D_ACCESS_BITS(1),REG_DDC_D_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEADDR,REG_DDC_D_WRITEADDR_BITS(i),REG_DDC_D_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_D_READPULSE,REG_DDC_D_READPULSE_BITS(1),REG_DDC_D_READPULSE_MASK);
            MsOS_DelayTaskUs(2);
            *(u8EDID + i) = (MS_U8)(DDC_READ_MASK(REG_DDC_D_READDATA, REG_DDC_D_READDATA_MASK)>>8);
        }
    }
}

// DDC
static void MDrv_HDMI_WriteEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize)
{
    MS_U16 i;

    HAL_HDMI_DDCRam_SelectPort(eDDCRamType);

    for( i = 0; i < u8EDIDSize; i++)
    {
        if(eDDCRamType == E_XC_PROG_VGA_EDID)
        {
            DDC_WRITE_MASK(REG_DDC_A_ACCESS,REG_DDC_A_ACCESS_BITS(0),REG_DDC_A_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEADDR,REG_DDC_A_WRITEADDR_BITS(i),REG_DDC_A_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEDATA,REG_DDC_A_WRITEDATA_BITS(*(u8EDID + i)),REG_DDC_A_WRITEDATA_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEPULSE,REG_DDC_A_WRITEPULSE_BITS(1),REG_DDC_A_WRITEPULSE_MASK);
        }
        else
        {
            DDC_WRITE_MASK(REG_DDC_D_ACCESS,REG_DDC_D_ACCESS_BITS(0),REG_DDC_D_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEADDR,REG_DDC_D_WRITEADDR_BITS(i),REG_DDC_D_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEDATA,REG_DDC_D_WRITEDATA_BITS(*(u8EDID + i)),REG_DDC_D_WRITEDATA_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEPULSE,REG_DDC_D_WRITEPULSE_BITS(1),REG_DDC_D_WRITEPULSE_MASK);
        }
    }
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI0_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect 0 ddc
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI1_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect 1 ddc
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI2_WP,BITS(5:5,1),BMASK(5:5));     // Write Protect 2 ddc
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI3_WP,BITS(5:5,1),BMASK(5:5));     //Write Protect  3 ddc
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_ADC0_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect analog ddc
            break;
        default:
            MS_ASSERT(0);
    }
}

static void MDrv_HDMI_DDCRAM_Enable(E_XC_DDCRAM_PROG_TYPE eDDCRamType)
{
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI0_EN,BITS(15:15,1),BMASK(15:15)); // enable digital 0 ddc
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI1_EN,BITS(15:15,1),BMASK(15:15)); // enable digital 1 ddc
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI2_EN,BITS(7:7,1),BMASK(7:7));     // enable digital 2 ddc
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI3_EN,BITS(7:7,1),BMASK(7:7));     // enable digital 3 ddc
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_ADC0_EN,BITS(15:15,1),BMASK(15:15)); // enable analog ddc
            break;
        default:
            MS_ASSERT(0);
    }
}

void MDrv_HDMI_READ_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    (u32SizeOfInfo)=(u32SizeOfInfo); //this Var is temp for UNUSE

    MDrv_HDMI_DDCRAM_Enable(pstDDCRam_Info->eDDCProgType);

    MDrv_HDMI_ReadEDID(pstDDCRam_Info->eDDCProgType, pstDDCRam_Info->EDID, pstDDCRam_Info->u16EDIDSize);

    HAL_HDMI_DDCRAM_SetPhyAddr(pstDDCRam_Info);
}

void MDrv_HDMI_READ_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{


	MDrv_HDMI_DDCRAM_Enable(pstDDCRam_Info->eDDCProgType);


		MDrv_HDMI_ReadEDID(pstDDCRam_Info->eDDCProgType, pstDDCRam_Info->EDID, pstDDCRam_Info->u16EDIDSize);


		HAL_HDMI_DDCRAM_SetPhyAddr(pstDDCRam_Info);
		return ;

   /* if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_READ_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_READ_DDCRAM, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }*/
}

void MDrv_HDMI_PROG_DDCRAM_internal(void *pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    (u32SizeOfInfo)=(u32SizeOfInfo); //this Var is temp for UNUSE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pstDDCRam_Info->eDDCProgType != E_XC_PROG_VGA_EDID)
    {
        memcpy(pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[pstDDCRam_Info->eDDCProgType - E_XC_PROG_DVI0_EDID], pstDDCRam_Info->EDID, EDID_KEY_LENGTH*sizeof(MS_U8));
    }
    MDrv_HDMI_DDCRAM_Enable(pstDDCRam_Info->eDDCProgType);

    MDrv_HDMI_WriteEDID(pstDDCRam_Info->eDDCProgType, pstDDCRam_Info->EDID, pstDDCRam_Info->u16EDIDSize);

    HAL_HDMI_DDCRAM_SetPhyAddr(pstDDCRam_Info);
}

void MDrv_HDMI_PROG_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_PROG_DDCRAM_internal(pInstance, pstDDCRam_Info, u32SizeOfInfo);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_HDMI_PROG_DDCRAM_internal(pInstance, pstDDCRam_Info, u32SizeOfInfo);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDMI_PROG_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_PROG_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PROG_DDCRAM, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/** According to the previous experience, some specific source devices would
send strange DVI / HDMI signal in slow frequency(like 480p) let us in trouble.
So it's better to increase DVI updated phase count to 100 lines in low
resolution to overcome that situation, originally DVI updated phase count is 1.
So the patch would check input DVI frequency first, if small than 50MHz, DVI
updated phase count would increase to 100 lines. Otherwise, DVI updated phase
count would be recovered to 1 line. */
void MDrv_HDMI_SetUpdatePhaseLineCount_U2(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    E_MUX_INPUTPORT enPort = INPUT_PORT_NONE_PORT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    enPort = MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);
    if( enPort == INPUT_PORT_DVI0 || enPort == INPUT_PORT_DVI1 ||
        enPort == INPUT_PORT_DVI3 )
    {
        if( R2BYTEMSK(REG_DVI_ATOP_61_L, BMASK(9:8)) == 2<<8 ||
            R2BYTEMSK(REG_DVI_ATOP_61_L, BMASK(9:8)) == 3<<8 ) //<50Mhz
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0x0000, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0x0000);
//            printf("Low resolution\n");

        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0);
//            printf("High resolution\n");
        }

    }
    else if(enPort == INPUT_PORT_DVI2)
    {
        if( R2BYTEMSK(REG_DVI_ATOP_6A_L, BMASK(9:8)) == 2<<8 ||
            R2BYTEMSK(REG_DVI_ATOP_6A_L, BMASK(9:8)) == 3<<8 ) //<50Mhz
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0x0000, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0x0000);
//            printf("Low resolution 2\n");
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0);
//            printf("High resolution 2\n");
        }

    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MDrv_HDMI_SetUpdatePhaseLineCount(INPUT_SOURCE_TYPE_t enInputSourceType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SETUPDATEPHASELINECOUNT XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_Get_PollingStatus_U2(void* pInstance, HDMI_POLLING_STATUS_t **pStatus)
{
    *pStatus = &g_HdmiPollingStatus;
}

void MDrv_HDMI_Get_PollingStatus(HDMI_POLLING_STATUS_t **pStatus)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_GET_POLLINGSTATUS XCArgs;
    XCArgs.pStatus = NULL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_IsHDMI_Mode_U2(void* pInstance)
{
    return g_HdmiPollingStatus.bIsHDMIMode;
}

MS_BOOL MDrv_HDMI_IsHDMI_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_ISHDMI_MODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ISHDMI_MODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the extended colorimetry from packet
/// @return @ref MS_HDMI_EXT_COLORIMETRY_FORMAT
//-------------------------------------------------------------------------------------------------
MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry_U2(void* pInstance)
{
    MS_HDMI_EXT_COLORIMETRY_FORMAT pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_UNKNOWN;
    MS_U8 u8AVI_info2, u8AVI_info3;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    u8AVI_info2 = MDrv_HDMI_avi_infoframe_info(_BYTE_2);
    msg_hdmi(printf(" MDrv_HDMI_Get_ExtColorimetry_U2 u8AVI_info2 is 0X%X , avi_info2&0xc0  0x%X \n",avi_info2,(avi_info2&0xc0)));
    if((u8AVI_info2&0xc0) == 0xc0 )
    {
        u8AVI_info3= MDrv_HDMI_avi_infoframe_info(_BYTE_3);
        msg_hdmi(printf(" MDrv_HDMI_Get_ExtColorimetry_U2 u8AVI_info3 is 0X%X \n",avi_info3));
        switch (u8AVI_info3 & 0x70)
        {
            case 0x00:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_XVYCC601;
                break;
            case 0x10:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_XVYCC709;
                break;
            case 0x20:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_SYCC601;
                break;
            case 0x30:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_ADOBEYCC601;
                break;
            case 0x40:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_ADOBERGB;
                break;
            default:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_UNKNOWN;
                break;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return pkt_ExtColorimetry;
}

MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_EXT_COLORIMETRY XCArgs;
    XCArgs.enReturnValue = MS_HDMI_EXT_COLOR_UNKNOWN;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_EXT_COLOR_UNKNOWN;
    }
    else
    {
        return XCArgs.enReturnValue;
    }
}

MS_U16 _MDrv_HDMI_GetHDE(void)
{
    return Hal_HDMI_GetHDE();
}

MS_U16 _MDrv_HDMI_GetHTT(void)
{
    return Hal_HDMI_GetHTT();
}

MS_U16 _MDrv_HDMI_GetVDE(void)
{
    return Hal_HDMI_GetVDE();
}

MS_U16 _MDrv_HDMI_GetVTT(void)
{
    return Hal_HDMI_GetVTT();
}
#if 0
void MDrv_HDMI_ENABLE_IMMESWITCH( MS_BOOL bEnImmeswitch,MS_BOOL bEnPowSaving )
{
    if(SUPPORT_IMMESWITCH)
    {
        g_bIsIMMESWITCH = bEnImmeswitch;
        g_bIMMESWITCH_DVI_POWERSAVING = bEnPowSaving;
    }
    else
    {
        g_bIsIMMESWITCH = FALSE;
        g_bIMMESWITCH_DVI_POWERSAVING = FALSE;
    }
}
#endif

#if 0
void MDrv_HDMI_ImmeswitchPS_Handler(void)
{
    E_MUX_INPUTPORT port;
    if(g_bIMMESWITCH_DVI_POWERSAVING)
    {
        for(port = INPUT_PORT_DVI0; port <=INPUT_PORT_DVI3; port++)
        {
            //if(!Hal_DVI_GetPowerSavingStatus(port)) // not need to check if power saving mode is not enable
            //continue;

            if(Hal_DVI_GetDEStableStatus(port)) // stable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] < 255)
                    u8PSDVIStableCount[port-INPUT_PORT_DVI0] += 1;
            }
            else // unstable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] > 0)
                {
                    Hal_DVI_EnhanceImmeswitch(port, FALSE);
                    msg_hdmi(printf("$$$ MDrv_HDMI_ImmeswitchPS_Handler:  port %d is unstable\n", port-INPUT_PORT_DVI0));
                }
                u8PSDVIStableCount[port-INPUT_PORT_DVI0] = 0;
            }

            if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] >= 3) // enable Immeswithc enhancement only when continous DE stable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] == 3)
                {
                    Hal_DVI_EnhanceImmeswitch(port, TRUE);
                    msg_hdmi(printf("$$$ MDrv_HDMI_ImmeswitchPS_Handler:  port %d is stable\n", port-INPUT_PORT_DVI0));
                }
            }
        }
    }
}

void MDrv_DVI_ForceAllPortsEnterPS(void)
{
    if(g_bIMMESWITCH_DVI_POWERSAVING)
    {
        msg_hdmi(printf("$$$ MDrv_DVI_ForceAllPortsEnterPS \n"));
        Hal_DVI_ForceAllPortsEnterPS();
    }
}
#endif

MS_BOOL MDrv_HDMI_Check4K2K_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val = E_HDMI_NA;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {
        val = Hal_HDMI_Check_Additional_Format();
        if(Hal_HDMI_Is_Input_Large_166MHz() && (val == E_HDMI_4Kx2K_FORMAT))
        {
            msg_hdmi(printf("~~~ HDMI 4Kx2K case, input bigger than 166Mhz ~~~\n"));

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return TRUE;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
    return FALSE;
}

MS_BOOL MDrv_HDMI_Check4K2K(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_CHECK_4K2K XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_CHECK_4K2K, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_3D_4Kx2K_Process_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val = E_HDMI_NA;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {
        val = Hal_HDMI_Check_Additional_Format();
        if(Hal_HDMI_Is_Input_Large_166MHz() && ((val == E_HDMI_3D_FORMAT)|(val == E_HDMI_4Kx2K_FORMAT)))
        {
            Hal_HDMI_AVG_ScaleringDown(TRUE);
            msg_hdmi(printf("~~~ HDMI 3D or 4Kx2K scaling down= %x\n", val));
        }
        else
        {
            Hal_HDMI_AVG_ScaleringDown(FALSE);
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    // Do nothing
#endif
}

void MDrv_HDMI_3D_4Kx2K_Process(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_SetHPD_HL_U2(void* pInstance, MS_BOOL Value)
{

#if SUPPORT_HDMI_RX_NEW_FEATURE
        Hal_HDMI_SetHPD_HL(Value);
#else
        // Do nothing
#endif


}
void MDrv_HDMI_SetHPD_HL(MS_BOOL bTrue)
{

    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SetHPD_HL XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_SetHPD_HL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}
void MDrv_HDMI_AVG_ScaleringDown_U2(void* pInstance, MS_BOOL bTrue)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_AVG_ScaleringDown(bTrue);
#else
    // Do nothing
#endif
}


void MDrv_HDMI_AVG_ScaleringDown(MS_BOOL bTrue)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_AVG_SCALERINGDOWN XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}


E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


  //  _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return Hal_HDMI_Check_Additional_Format();
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return E_HDMI_NA;
    }
#else
    return E_HDMI_NA;// Do nothing
#endif
}

E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_NA;
    }

    stHDMI_CHECK_ADDITIONAL_FORMAT XCArgs;
    XCArgs.eReturnValue = E_HDMI_NA;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_NA;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return Hal_HDMI_Get_3D_Structure();
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return E_XC_3D_INPUT_MODE_NONE;
    }
#else
    return E_XC_3D_INPUT_MODE_NONE;// Do nothing
#endif
}

E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_XC_3D_INPUT_MODE_NONE;
    }

    stHDMI_GET_3D_STRUCTURE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_INPUT_MODE_NONE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_INPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return Hal_HDMI_Get_3D_Ext_Data();
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return E_3D_EXT_DATA_MODE_MAX;
    }
#else
    return E_3D_EXT_DATA_MODE_MAX; // Do nothing
#endif
}

E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_3D_EXT_DATA_MODE_MAX;
    }

    stHDMI_GET_3D_EXT_DATA XCArgs;
    XCArgs.eReturnValue = E_3D_EXT_DATA_MODE_MAX;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_CMD_GET_3D_EXT_DATA, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_3D_EXT_DATA_MODE_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MDrv_HDMI_Get_3D_Meta_Field_U2(void* pInstance, sHDMI_3D_META_FIELD *pdata)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
        Hal_HDMI_Get_3D_Meta_Field(pdata);
    else
        memset(pdata, 0, sizeof(sHDMI_3D_META_FIELD));

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


#else
    // Do nothing
#endif
}

MS_U8 MDrv_HDMI_Get_VIC_Code_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_AVI_VALUE) // AVI packet should be received first
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return Hal_HDMI_Get_VIC_Code();
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return 0;
    }
#else
    return 0;// Do nothing
#endif
}

MS_U8 MDrv_HDMI_Get_VIC_Code(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_VIC_CODE XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_VIC_CODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

void MDrv_HDMI_Get_3D_Meta_Field(sHDMI_3D_META_FIELD *pdata)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_GET_3D_META_FIELD XCArgs;
    XCArgs.pdata = pdata;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_3D_META_FIELD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_HDMI.pkt_value.PKT_VS_VALUE) // VSP packet should be received first
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        //_XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        return Hal_HDMI_Get_4Kx2K_VIC_Code();
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return E_VIC_NA;
    }
#else
    return E_VIC_NA;// Do nothing
#endif
}

E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_VIC_NA;
    }

    stHDMI_GET_4KX2K_VIC_CODE XCArgs;
    XCArgs.eReturnValue = E_VIC_NA;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_VIC_NA;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// HDMI ARC pin control
//     - enInputPortType: INPUT_PORT_DVI0 / INPUT_PORT_DVI1 / INPUT_PORT_DVI2 / INPUT_PORT_DVI3
//     - bEnable: ARC enable or disable
//     - bDrivingHigh: ARC driving current high or low, suggest driving current should be set to high when ARC is enable
void MDrv_HDMI_ARC_PINControl_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_ARC_PINControl(enInputPortType, bEnable, bDrivingHigh);
#else
    // Do nothing
#endif
}

void MDrv_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_ARC_PINCONTROL XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bDrivingHigh = bDrivingHigh;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_HDMI_IF_CMD_ARC_PINCONTROL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//[6]: soft-reset hdmi
//[5]: soft-reset hdcp
//[4]: soft-reset dvi
void MDrv_DVI_Software_Reset_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_DVI_Software_Reset(enInputPortType, u16Reset);
#else
    // Do nothing
#endif
}

void MDrv_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_SOFTWARE_RESET XCArgs;
    XCArgs.enInputPortType= enInputPortType;
    XCArgs.u16Reset = u16Reset;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

MS_U32 MDrv_HDMI_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState)
{
    MS_U16 u32Return = UTOPIA_STATUS_FAIL;
    //MS_CEC_INIT_INFO stCECInfo;
    XC_DDCRAM_PROG_INFO stDDCRamInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (enPowerState == E_POWER_SUSPEND)
    {
        pXCResourcePrivate->stdrv_HDMI._prev_enPowerState = enPowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (enPowerState == E_POWER_RESUME)
    {

        if (pXCResourcePrivate->stdrv_HDMI._prev_enPowerState == E_POWER_SUSPEND)
        {
            // EDID and DDC Ram
            {

            // init Digital EDID 0
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[E_XC_PROG_DVI0_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI0_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x10; //0x5555;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));


            // init Digital EDID 1
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[E_XC_PROG_DVI1_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI1_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x20; //0x3131;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));

            // init Digital EDID 2
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[E_XC_PROG_DVI2_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI2_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x30; //9292;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));

            // init Digital EDID 3
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[E_XC_PROG_DVI3_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI3_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x40; //0x9292;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0x9D;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0x9C;


            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));
            }

            MDrv_HDMI_init_internal(pInstance);

            // init HDCP key again.
            MDrv_HDCP_initproductionkey_internal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp);

            // HPD
            {
                E_MUX_INPUTPORT eHDMIPort;

                // split to 2 loop for time saving
                for (eHDMIPort = INPUT_PORT_DVI0;eHDMIPort <= INPUT_PORT_DVI3;eHDMIPort++)
                {
                    Hal_DVI_ClkPullLow(FALSE, eHDMIPort);
                }

                MsOS_DelayTask(100);

                for (eHDMIPort = INPUT_PORT_DVI0;eHDMIPort <= INPUT_PORT_DVI3;eHDMIPort++)
                {
                    MDrv_HDMI_pullhpd_internal(pInstance, ENABLE, eHDMIPort, pXCResourcePrivate->stdrvXC_MVideo_Context.g_bHDMIInverseBackUp[eHDMIPort - INPUT_PORT_DVI0]);
                }
            }

#if 0
            // CEC
            stCECInfo.u32XTAL_CLK_Hz = MST_XTAL_CLOCK_HZ;
            MApi_CEC_Init(&stCECInfo);
#endif
            pXCResourcePrivate->stdrv_HDMI._prev_enPowerState = enPowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__func__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__func__,__LINE__,enPowerState);
        u32Return = UTOPIA_STATUS_NOT_SUPPORTED;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u32Return;// for success
}

MS_U32 MDrv_HDMI_SetPowerState(EN_POWER_MODE enPowerState)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_FAIL;
    }

    stHDMI_SET_POWERSTATE XCArgs;
    XCArgs.enPowerState = enPowerState;
    XCArgs.u32ReturnValue = UTOPIA_STATUS_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_POWERSTATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_BOOL MDrv_HDMI_CheckHDMI20_Setting_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
#if SUPPORT_HDMI20
    return Hal_HDMI_CheckHDMI20_Setting(enInputPortType);
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_HDMI_CheckHDMI20_Setting(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_CheckHDMI20_Setting XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_StablePolling_U2(void* pInstance)
{
	Hal_HDMI_StablePolling();
}

void MDrv_HDMI_StablePolling(void)
{
	if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_STABLE_POLLING, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// Get HDMI HDE
MS_U16 MDrv_HDMI_GetHde_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    stHDMI_GETHVSTATUS stReturnInfo;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    stReturnInfo.u16ReturnValue = _MDrv_HDMI_GetHDE();
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return stReturnInfo.u16ReturnValue;
}

MS_U16 MDrv_HDMI_GetHde(void )
{
    stHDMI_GETHVSTATUS XCArgs;
    memset(&XCArgs, 0, sizeof(stHDMI_GETHVSTATUS));

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.u16ReturnValue;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_HDE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.u16ReturnValue;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

// Get HDMI HDE
MS_U16 MDrv_HDMI_GetVde_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    stHDMI_GETHVSTATUS stReturnInfo;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    stReturnInfo.u16ReturnValue = _MDrv_HDMI_GetVDE();
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return stReturnInfo.u16ReturnValue;
}

MS_U16 MDrv_HDMI_GetVde(void )
{
    stHDMI_GETHVSTATUS XCArgs;
    memset(&XCArgs, 0, sizeof(stHDMI_GETHVSTATUS));

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.u16ReturnValue;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_VDE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.u16ReturnValue;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

// Get HDMI Htt
MS_U16 MDrv_HDMI_GetHtt_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    stHDMI_GETHVSTATUS stReturnInfo;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    stReturnInfo.u16ReturnValue = _MDrv_HDMI_GetHTT();
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return stReturnInfo.u16ReturnValue;
}

MS_U16 MDrv_HDMI_GetHtt(void )
{
    stHDMI_GETHVSTATUS XCArgs;
    memset(&XCArgs, 0, sizeof(stHDMI_GETHVSTATUS));

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.u16ReturnValue;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_HTT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.u16ReturnValue;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U16 MDrv_HDMI_GetVtt_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    stHDMI_GETHVSTATUS stReturnInfo;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    stReturnInfo.u16ReturnValue = _MDrv_HDMI_GetVTT();
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    return stReturnInfo.u16ReturnValue;
}

MS_U16 MDrv_HDMI_GetVtt(void )
{
    stHDMI_GETHVSTATUS XCArgs;
    memset(&XCArgs, 0, sizeof(stHDMI_GETHVSTATUS));

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.u16ReturnValue;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_VTT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.u16ReturnValue;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U8 MDrv_HDMI_GetSourceVersion_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    return Hal_HDMI_GetSourceVersionType(enInputPortType);
}

MS_U8 MDrv_HDMI_GetSourceVersion(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_Get_Source_Version XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucSourceVersion= FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_SOURCE_VERSION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.ucSourceVersion;
    }
}

void MDrv_HDMI_Set5VDetectGPIOSelect_U2(void* pInstance, MS_U32 ul5VDetectGPIOIndex)
{
    Hal_HDMI_Set5VDetectGPIOSelect(ul5VDetectGPIOIndex);
}

void SYMBOL_WEAK MDrv_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_Set5VDetectGPIOSelect XCArgs;
    XCArgs.ul5VDetectGPIOSelect = ul5VDetectGPIOIndex;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_GetDEStableStatus_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    return Hal_DVI_GetDEStableStatus(enInputPortType);
}

MS_BOOL MDrv_HDMI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_Get_De_Stable_Status XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bStatus= FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bStatus;
    }
}

void MDrv_HDMI_DDCChannelEnable_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable)
{
    Hal_HDMI_DDCChannelEnable(enInputPortType, bEnable);
}

void MDrv_HDMI_DDCChannelEnable(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_DDC_Channel_Enable XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DDC_CHANNEL_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


