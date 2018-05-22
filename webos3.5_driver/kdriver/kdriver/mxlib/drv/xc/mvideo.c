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
/// file    Mvideo.c
/// @brief  Main API in XC library
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  MVIDEO_C
#define  ADC_Index_CheckDelay 50
#define  ADC_Index_CheckCnt 6
#define  ADC_Index_CheckLimit 10

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
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
#include "mhal_adc.h"
#include "mdrv_frc.h"
#include "mhal_frc.h"
#include "mhal_sc.h"
#include "mhal_dynamicscaling.h"
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_Internal.h"

#include "xc_Analog_Reg.h"
#include "hwreg_ipmux.h"

#include "drv_sc_scaling.h"
#include "drv_sc_mux.h"
#include "drv_sc_DIP_scaling.h"
#include "mdrv_sc_dynamicscaling.h"
#include "drvscaler_nr.h"
#include "drvMMIO.h"
#include "mhal_menuload.h"

//Add for A5
#include "mhal_dip.h"

// Tobe refined
#include "xc_hwreg_utility2.h"
#include "mhal_mux.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "mhal_sc.h"

#if (XC_SUPPORT_CMA == TRUE)
#include "drvCMAPool.h"
#include "msos/linux/ion_uapi.h"
#include "msos/linux/mdrv_cma_pool_st.h"
#include "halCHIP.h"
#endif

/*
    XC_INITDATA_VERSION                      current version : 1    //1: Add an field eScartIDPort_Sel for port selection
*/
#define MODULE_XC MODULE_XC
#define MODULE_XC_KERNEL MODULE_XC|KERNEL_MODE
//#define SUPPORT_HW_TESTPATTERN 1
#define OP1_IRE_80 818
#define OP1_IRE_100 1023


#define CHKSTABLE_SIGNAL_BF_ADC 1
#define CHKINTVAL 1
#define MAXTIME 40
//PQ_Function_Info    s_PQ_Function_Info = {0, 0, 0, 0, 0, 0, 0, 0, 0};
PQ_Function_Info    s_PQ_Function_Info[MAX_XC_DEVICE_NUM_SC2];//MAX_XC_DEVICE_NUM // = {{0, 0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0, 0}};

MS_U8 MApi_Pnl_Get_LPLL_Type(void);
MS_BOOL MApi_PNL_Get_TCON_Capability(void);
//void MApi_PNL_CalExtLPLLSETbyDClk(MS_U32 ldHz);
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define VERIFY_MVIDEO_FPGA  0

#if 0
typedef enum
{
    SCAN_AUTO = 0,
    SCAN_PROGRESSIVE = SCAN_AUTO,
    SCAN_INTERLACE = 1
} EN_VIDEO_SCAN_TYPE;
#endif

#ifndef XC_HLINEARSCALING_VER
#define XC_HLINEARSCALING_VER 0
#endif

#ifndef ENABLE_DI_8LB_MODE
#define ENABLE_DI_8LB_MODE 0
#endif

//#define SUPPORT_HDMI20_420
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _api_xc_version = {
    .DDI = { XC_API_VERSION },
};
#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
//static MS_BOOL bIsHWPatternEnabled = FALSE;
//static MS_BOOL bIsMute = FALSE;
//static MS_BOOL bIsInputSrcDisabled = FALSE;
MS_BOOL bIsIREMode_Open = FALSE;
static MS_U16  OP1_RVal = 0;
static MS_U16  OP1_GVal = 0;
static MS_U16  OP1_BVal = 0;
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
extern MS_BOOL bColorMatrixEnable;
#else
extern bColorMatrixEnable;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */

#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if(XC_CONTEXT_ENABLE)
MS_U8 _XC_Client[] = {"Linux XC driver"};

static MS_BOOL _MApi_XC_Init_Context(void *pInstance, MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bRet = TRUE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if 0
    if (pXCResourcePrivate->stdrvXC_MVideo.g_pSContext)
    {
        // The context instance has been created already
        // before somewhere sometime in the same process.
        *pbFirstDrvInstant = bNeedInitShared;
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }

#ifdef MSOS_TYPE_LINUX
    MS_U32 u32ShmId;
    MS_U32 u32Addr;
    MS_U32 u32BufSize;

    MsOS_SHM_Init();
    if (FALSE == MsOS_SHM_GetId(_XC_Client, sizeof(XC_Context_Shared), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId(_XC_Client, sizeof(XC_Context_Shared), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "XC: SHM allocation failed!\n");
            bRet = FALSE;
        }
        if(bRet)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "XC: [%s][%d] This is first initial 0x%08lx\n", __FUNCTION__, __LINE__, u32Addr);
            memset( (MS_U8*)u32Addr, 0, sizeof(XC_Context_Shared));
            bNeedInitShared = TRUE;
        }
    }
    if(bRet)
    {
        pShared = (XC_Context_Shared *)u32Addr;
    }
#endif

    if(pShared == NULL)
    {
        pShared = &(pXCResourcePrivate->stdrvXC_MVideo._SCShared);
        bNeedInitShared = TRUE;
    }

    *pbFirstDrvInstant = bNeedInitShared;


    if (bNeedInitShared)
    {
        memset(pShared, 0, sizeof(XC_Context_Shared));
    }
#endif

    *pbFirstDrvInstant = pXCResourcePrivate->stdrvXC_MVideo.bNeedInitShared;

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return bRet;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Exit the XC
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Exit_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if (_XC_Mutex == -1)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return TRUE;
    }
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    // Protect memory
    MDrv_XC_FreezeImg(pInstance, ENABLE, MAIN_WINDOW);


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE


#ifdef K3_U2
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo.u8MaxWindowNum > 1)
        {
            // DISABLE FIClk1
            MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, CKG_FICLK_F1_GATED);                // Enable clock

            // DISABLE IDClk1
            MDrv_WriteRegBit(REG_CKG_IDCLK1, ENABLE, CKG_IDCLK1_GATED);

            // DISABLE SIDClk1
            MDrv_WriteRegBit(REG_CKG_SIDCLK1, ENABLE, CKG_SIDCLK1_GATED);
        }

        // DISABLE FIClk2
        MDrv_WriteRegBit(REG_CKG_FICLK_F2, ENABLE, CKG_FICLK_F2_GATED);                    // Enable clock

        // DISABLE FClk
        MDrv_WriteRegBit(REG_CKG_FCLK, ENABLE, CKG_FCLK_GATED);                            // Enable clock

        // DISABLE IDClk2
        MDrv_WriteRegBit(REG_CKG_IDCLK2, ENABLE, CKG_IDCLK2_GATED);                        // Enable clock

        // DISABLE SIDClk2
        MDrv_WriteRegBit(REG_CKG_SIDCLK2, ENABLE, CKG_SIDCLK2_GATED);
    }
    else // pDeviceId->u32Id == E_XC_DEVICE1
    {
        // DISABLE FIClk2
        MDrv_WriteRegBit(REG_CKG_SC1_FECLK_F2, ENABLE, CKG_SC1_FECLK_F2_GATED);                    // Enable clock

        // DISABLE FClk
        MDrv_WriteRegBit(REG_CKG_SC1_FCLK, ENABLE, CKG_SC1_FCLK_GATED);                            // Enable clock

        // DISABLE IDClk2
        MDrv_WriteRegBit(REG_CKG_SC1_IDCLK2, ENABLE, CKG_SC1_IDCLK2_GATED);                        // Enable clock

        // DISABLE SIDClk2
        MDrv_WriteRegBit(REG_CKG_SC1_SIDCLK2, ENABLE, CKG_SC1_SIDCLK2_GATED);
    }
#else

#if(HW_DESIGN_4K2K_VER == 4)
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo.u8MaxWindowNum > 1)
        {
            // DISABLE FIClk1
            MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, CKG_FICLK_F1_GATED);                // Enable clock

            // DISABLE IDClk1
            MDrv_WriteRegBit(REG_CKG_IDCLK1, ENABLE, CKG_IDCLK1_GATED);
        }

        // DISABLE EDCLK_F2
        Hal_SC_set_edclk(pInstance, 0x00, DISABLE, MAIN_WINDOW);
        // DISABLE EDCLK_F1
        Hal_SC_set_edclk(pInstance, 0x00, DISABLE, SUB_WINDOW);

        // DISABLE FICLK2_F2
        Hal_SC_set_ficlk2(pInstance, 0x00, DISABLE, MAIN_WINDOW);

        // DISABLE FIClk_F2
        MDrv_WriteRegBit(REG_CKG_FICLK_F2, ENABLE, CKG_FICLK_F2_GATED);                    // Enable clock

        // DISABLE FClk
        MDrv_WriteRegBit(REG_CKG_FCLK, ENABLE, CKG_FCLK_GATED);                            // Enable clock

        // DISABLE IDClk2
        MDrv_WriteRegBit(REG_CKG_IDCLK2, ENABLE, CKG_IDCLK2_GATED);                        // Enable clock
    }
    else
    {

#if SC2_ENABLE
#ifdef MONACO_SC2_PATCH
        // DISABLE FIClk_F2
        MDrv_WriteRegBit(REG_CKG_S2_FICLK2_F2, ENABLE, CKG_FICLK2_F2_GATED);                    // Enable clock
#endif

#else
        // DISABLE FIClk_F2
        MDrv_WriteRegBit(REG_CKG_S2_FICLK_F2, ENABLE, CKG_FICLK_F2_GATED);                    // Enable clock

#endif //#if SC2_ENABLE


        // DISABLE FClk
        MDrv_WriteRegBit(REG_CKG_S2_FCLK, ENABLE, CKG_S2_FCLK_GATED);                            // Enable clock

        // DISABLE IDClk2
        MDrv_WriteRegBit(REG_CKG_S2_IDCLK2, ENABLE, CKG_S2_IDCLK2_GATED);                        // Enable clock

    }
#else
    if(pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo.u8MaxWindowNum > 1)
    {
        // DISABLE FIClk1
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, CKG_FICLK_F1_GATED);                // Enable clock

        // DISABLE IDClk1
        MDrv_WriteRegBit(REG_CKG_IDCLK1, ENABLE, CKG_IDCLK1_GATED);
    }

    // DISABLE EDCLK_F2
    Hal_SC_set_edclk(pInstance, 0x00, DISABLE, MAIN_WINDOW);
    // DISABLE EDCLK_F1
    Hal_SC_set_edclk(pInstance, 0x00, DISABLE, SUB_WINDOW);

    // DISABLE FICLK2_F2
    Hal_SC_set_ficlk2(pInstance, 0x00, DISABLE, MAIN_WINDOW);

    // DISABLE FIClk_F2
    MDrv_WriteRegBit(REG_CKG_FICLK_F2, ENABLE, CKG_FICLK_F2_GATED);                    // Enable clock

    // DISABLE FClk
    MDrv_WriteRegBit(REG_CKG_FCLK, ENABLE, CKG_FCLK_GATED);                            // Enable clock

    // DISABLE IDClk2
    MDrv_WriteRegBit(REG_CKG_IDCLK2, ENABLE, CKG_IDCLK2_GATED);                        // Enable clock
#endif
#endif

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE



    // Release ISR
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        MsOS_DetachInterrupt(E_INT_IRQ_DISP);
    }
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        MsOS_DetachInterrupt(E_INT_IRQ_EC_BRIDGE);
    }

    // Clear internal variable
    mvideo_sc_variable_init(pInstance, TRUE, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData);

    // free Fastboot related global variable
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp != NULL)
    {
        free(pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp);
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp = NULL;
    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp != NULL)
    {
        free(pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp);
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp = NULL;
    }

    // Clear internal variable
    mvideo_sc_variable_init(pInstance, TRUE, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData);


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    memset(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData, 0 , sizeof(XC_INITDATA) );
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_ENTRY();
    memset(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData, 0 , sizeof(XC_INITDATA) );
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    // For XC re-init
    MsOS_DeleteMutex(_XC_Mutex);
    _XC_Mutex = -1;

    MsOS_DeleteMutex(_XC_ISR_Mutex);
    _XC_ISR_Mutex = -1;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;

}

MS_BOOL MApi_XC_Exit(void)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_EXIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_EXIT, (void*)&XCArgs) != 0)
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
/// Exit the XC
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Close_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if (_XC_Mutex == -1)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return TRUE;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    // Release ISR
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        MsOS_DetachInterrupt(E_INT_IRQ_DISP);
    }
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        MsOS_DetachInterrupt(E_INT_IRQ_EC_BRIDGE);
    }
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE


#if 1
    // For XC re-init
    MsOS_DeleteMutex(_XC_Mutex);
    _XC_Mutex = -1;

    MsOS_DeleteMutex(_XC_ISR_Mutex);
    _XC_ISR_Mutex = -1;
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;

}

MS_BOOL MApi_XC_Close(void)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CLOSE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CLOSE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Open_U2(void* pInstance)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (_XC_Mutex == -1)
    {
        _XC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_Mutex", MSOS_PROCESS_SHARED);
        if (_XC_Mutex == -1)
        {
            printf("[MAPI XC]_XC_Mutex create mutex fail\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create mutex fail\n", __LINE__);
            XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

            return FALSE;
        }
    }

    if (_XC_ISR_Mutex == -1)
    {
        _XC_ISR_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_ISR_Mutex", MSOS_PROCESS_SHARED);
        if (_XC_ISR_Mutex == -1)
        {
            printf("[MAPI XC]_XC_ISR_Mutex create mutex fail\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create ISR mutex fail\n", __LINE__);
            XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

            return FALSE;
        }
    }

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DISP, MDrv_SC_isr);
    }
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DISP1, MDrv_SC1_isr);
    }
#ifndef MSOS_TYPE_LINUX_KERNEL
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        MsOS_EnableInterrupt(E_INT_IRQ_DISP);
    }
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
    }
#endif


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;

}

MS_BOOL MApi_XC_Open(void)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_OPEN XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OPEN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_IsPNLYUVOutput(void *pInstance)
{
    //Pnl output is affected by input signal, so change to xc driver to report it as below
    return MHal_XC_IsPNLYUVOutput(pInstance);
}

MS_BOOL MDrv_XC_Is2K2KToFrc(void *pInstance)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if FRC_INSIDE
    // For Einstein FRC 4K2K case, for non-4K2K input, need to sent 2Kx2K to FRC
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal> 3000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal> 2000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500) &&  // 4K2K 60 out
            (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K))
    {
        bRet = TRUE;
    }
#endif
    return bRet;
}

MS_BOOL MApi_XC_SetIOMapBase_i_U2(void* pInstance)
{
    MS_U32 _XCRIUBaseAddress = 0, _PMRIUBaseAddress = 0, u32NonPMBankSize = 0, u32PMBankSize = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


    if(MDrv_MMIO_GetBASE( &_XCRIUBaseAddress, &u32NonPMBankSize, MS_MODULE_XC ) != TRUE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init GetBASE failure\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    else
    {
        //printf("MApi_XC_Init GetBase success %lx\n", _XCRIUBaseAddress);
    }

    if(MDrv_MMIO_GetBASE( &_PMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ) != TRUE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init Get PM BASE failure\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    else
    {
        //printf("MApi_XC_Init GetPMBase success %lx\n", _PMRIUBaseAddress);
    }
    //printf("XC RIU base=%lx\n", _XCRIUBaseAddress);

    MDrv_XC_init_riu_base(_XCRIUBaseAddress, _PMRIUBaseAddress);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;
}

MS_BOOL MApi_XC_SetIOMapBase_i(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CMD_SET_IOMAPBASE_I XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IOMAPBASE_I, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_SetIOMapBase_i(void* pInstance)
{
    MS_U32 _XCRIUBaseAddress = 0, _PMRIUBaseAddress = 0, u32NonPMBankSize = 0, u32PMBankSize = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


    if(MDrv_MMIO_GetBASE( &_XCRIUBaseAddress, &u32NonPMBankSize, MS_MODULE_XC ) != TRUE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init GetBASE failure\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    else
    {
        //printf("MApi_XC_Init GetBase success %lx\n", _XCRIUBaseAddress);
    }

    if(MDrv_MMIO_GetBASE( &_PMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ) != TRUE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init Get PM BASE failure\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    else
    {
        //printf("MApi_XC_Init GetPMBase success %lx\n", _PMRIUBaseAddress);
    }
    //printf("XC RIU base=%lx\n", _XCRIUBaseAddress);

    MDrv_XC_init_riu_base(_XCRIUBaseAddress, _PMRIUBaseAddress);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;
}

//This API only designed for DFB calling.
//Supernova involved xc lib only by initializing this function.
//So we need to create mutext in this API.
//it's useless for new dfb-xc design
MS_BOOL MDrv_XC_SetIOMapBase_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._bDFBInit = TRUE; //This flag is to compatible old dfb version.
    //new dfb will not call this func,
    //instead of set u32InitDataLen=0 of MApi_XC_Init()
    printf("MDrv_XC_SetIOMapBase(), do nothing, it's assumed DFB init case!\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}
MS_BOOL MDrv_XC_SetIOMapBase(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CMD_SET_IOMAPBASE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IOMAPBASE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_PreInit_FRC(void *pInstance, XC_PREINIT_INFO_t *pPanelInfo)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    ///patch
    ///g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType = E_XC_3D_PANEL_SHUTTER;
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc, pPanelInfo, sizeof(XC_PREINIT_INFO_t));
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

E_APIXC_ReturnValue MApi_XC_PreInit_U2( void* pInstance, E_XC_PREINIT_t eType ,void* para, MS_U32 u32Length)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    switch(eType)
    {
        case E_XC_PREINIT_FRC:
            if(u32Length != sizeof(XC_PREINIT_INFO_t))
            {
                printf("Error size of parameters\n");
                return E_APIXC_RET_FAIL;
            }

#if SC2_ENABLE
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
            MApi_XC_PreInit_FRC(pInstance, (XC_PREINIT_INFO_t *)para);
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
            MApi_XC_PreInit_FRC(pInstance, (XC_PREINIT_INFO_t *)para);
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            break;
        case E_XC_PREINIT_FRC_3D_TYPE:
            if(u32Length != sizeof(XC_PREINIT_INFO_t))
            {
                printf("Error size of parameters\n");
                return E_APIXC_RET_FAIL;
            }

#if SC2_ENABLE
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
            UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
            XC_PREINIT_INFO_t* stFRCInfo = (XC_PREINIT_INFO_t *)para;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType = stFRCInfo->FRCInfo.u8FRC3DPanelType;

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            break;
        default:
        case E_XC_PREINIT_NULL:

            break;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_PreInit( E_XC_PREINIT_t eType ,void* para, MS_U32 u32Length)
{
    if (pu32XCInst == NULL)
    {
        XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_FULL);
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_PREINIT XCArgs;
    XCArgs.eType = eType;
    XCArgs.para = para;
    XCArgs.u32Length = u32Length;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_PREINIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

static MS_BOOL _MApi_XC_Init_WithoutCreateMutex(void *pInstance, XC_INITDATA *pXC_InitData, MS_U32 u32InitDataLen)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo._bSkipSWReset == FALSE)
    {

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE


        MDrv_SC_SetSkipWaitVsync(pInstance, MAIN_WINDOW, TRUE);
        MDrv_SC_SetSkipWaitVsync(pInstance, SUB_WINDOW , TRUE);
        MDrv_XC_SoftwareReset(pInstance, REST_IP_ALL, MAIN_WINDOW);

#if 0
#if (XC_SUPPORT_CMA == TRUE)
        
                stXC_CMA_Config[CMA_XC_SELF_MEM].u32HeapID = ION_XC_HEAP_ID;  //FIXME //22 is correct
#if defined(CHIP_MONET)
                stXC_CMA_Config[CMA_XC_COBUFF_MEM].u32HeapID = XC_DUAL_MIU_HID;
#else
                stXC_CMA_Config[CMA_XC_COBUFF_MEM].u32HeapID = 0xff;//XC_INVALID_HEAP_ID;
#endif
                stXC_CMA_Config[CMA_XC_SELF_MEM].u64AddrHeapOffset = 0;
                stXC_CMA_Config[CMA_XC_COBUFF_MEM].u64AddrHeapOffset = 0;
        
                /* 1. do CMA_POOL_INIT */
                CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_id     = stXC_CMA_Config[CMA_XC_SELF_MEM].u32HeapID;
                CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].flags       = CMA_FLAG_MAP_VMA;
                MsOS_DelayTaskUs(1);
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d][CMA], CMA self heapid = %d!\n",__func__, __LINE__,CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_id);
        
                MS_BOOL ret = MApi_CMA_Pool_Init(&CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM]);
        
                if(ret == FALSE)
                {
                    printf("\033[35m   Function = %s, Line = %d, CMA_POOL_INIT ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
                    //assert(0);
                }
                else
                {
                    //printf("\033[35m  Function = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].pool_handle_id);
                    //printf("\033[35m  Function = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__,__LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].miu);
                    //printf("\033[35m  Function = %s, Line = %d, get heap_miu_start_offset is 0x%lx\033[m\n", __PRETTY_FUNCTION__, __LINE__, (MS_U32)CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_miu_start_offset);
                    //printf("\033[35m  Function = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_length);
                }
        
#endif
#endif
        // adc init
        MDrv_ADC_init(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock/1000UL
                      ,pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bIsShareGround
                      ,pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.eScartIDPort_Sel);

        Hal_SC_enable_window(pInstance, FALSE,SUB_WINDOW);
        MDrv_XC_DisableInputSource(pInstance, ENABLE, MAIN_WINDOW);
        MDrv_XC_DisableInputSource(pInstance, ENABLE, SUB_WINDOW);

        // default frame buffer address init
        MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size, MAIN_WINDOW);

        MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size, SUB_WINDOW);
#if (HW_DESIGN_4K2K_VER == 4)
        // default frcm frame buffer address init

        MDrv_XC_SetFRCMFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Start_Addr,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Size, MAIN_WINDOW);
        MDrv_XC_SetFRCMFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Start_Addr,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Size, SUB_WINDOW);
#endif
        MDrv_XC_init_fbn_win(pInstance, MAIN_WINDOW);

        MDrv_XC_init_fbn_win(pInstance, SUB_WINDOW);

        //HW initial settting of MCDi is turned on (the address of MCDi is 0).
        //Driver should turn it off before Fclk on, otherwise it will start write whole memory.
        MDrv_SC_EnableMCDI(pInstance, DISABLE,E_XC_MCDI_BOTH); // Main Window
        MDrv_SC_EnableMCDI(pInstance, DISABLE,E_XC_MCDI_SUB_BOTH); // Sub Window

        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bEnableIPAutoCoast)
        {
            // enable IP auto coast
            MDrv_XC_EnableIPAutoCoast(pInstance, ENABLE);
        }

        /**
            * !!! Attention !!! The ISR interface is different from Chakra2 and Utopia.
            * In Chakra2, there are two para needed, MHAL_SavedRegisters and vector.
            * In Utopia, there is no parameter.
            */
#ifndef MSOS_TYPE_LINUX_KERNEL
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            MsOS_AttachInterrupt(E_INT_IRQ_DISP, MDrv_SC_isr);
            MDrv_Write2Byte(0x101274, 0x8);
#if 0
#ifndef MSOS_TYPE_LINUX_KERNEL
            printf("\033[1;32m************* atatch MDrv_SC_isr ********\033[m\n");
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
            printf("\033[1;32m************* Kernel atatch MDrv_SC_isr ********\033[m\n");
#endif
#endif
        }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            MsOS_AttachInterrupt(E_INT_IRQ_DISP1, MDrv_SC1_isr);
        }
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            //printf("\033[1;32m************* atatch MDrv_SC_isr ********\033[m\n");
            MsOS_EnableInterrupt(E_INT_IRQ_DISP);
        }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
        }
#endif


        // Enable XC interrupts
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
        //MDrv_WriteRegBit(L_BK_VOP(0x25), ENABLE, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_25_L, 0x80, BIT(7));

        // Daten FixMe, move from panel init to here, need to check why
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
        //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(1));  //Using new ovs_ref
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x02, BIT(1));

        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_WriteByteMask(L_BK_IP1F2(0x09), 0x02, BIT(3) | BIT(2) | BIT(1) | BIT(0));  //YLock
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x02, BIT(3) | BIT(2) | BIT(1) | BIT(0));
        //Extend the pixel of bottom A session at the right side [7:0]
        //Extend the pixel of bottom B session at the left side[15:8] for PIP issue
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_14_L, (OFFSET_PIXEL_ALIGNMENT << 8) | (OFFSET_PIXEL_ALIGNMENT));

        //HSYNC Tolerance for Mode Change
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, 0x04, 0x1F);

        //----------------------------
        // Enable clock
        //----------------------------

        // Enable sub window clock
#ifdef K3_U2
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            // Enable FIClk2
            MDrv_WriteByteMask(REG_CKG_FICLK_F2, CKG_FICLK_F2_FLK, CKG_FICLK_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_FICLK_F1_IDCLK2
            MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_INVERT);                   // Not Invert
            MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_GATED);                    // Enable clock

            // Enable FClk
            MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_170MHZ, CKG_FCLK_MASK);                   // select 170MHz, if not enough (for example, scaling bandwidth not enough), increase this
            MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_INVERT);                           // Not Invert
            MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_GATED);                            // Enable clock

            // Enable IDClk2
            MDrv_WriteByteMask(REG_CKG_IDCLK2, CKG_IDCLK2_XTAL, CKG_IDCLK2_MASK);               // initial to XTAL first, will be changed when switch input source
            MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_INVERT);                       // Not Invert
            MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_GATED);                        // Enable clock

            // Enable SIDClk2
            MDrv_WriteRegBit(REG_CKG_SIDCLK2, DISABLE, CKG_SIDCLK2_INVERT);
            MDrv_WriteRegBit(REG_CKG_SIDCLK2, DISABLE, CKG_SIDCLK2_GATED);
        }
        else //E_XC_DEVICE1 == pDeviceId->u32Id
        {
            // Enable FIClk2
            MDrv_WriteByteMask(REG_CKG_SC1_FECLK_F2, CKG_SC1_FECLK_F2_SC1_FCLK, CKG_SC1_FECLK_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_FICLK_F1_IDCLK2
            MDrv_WriteRegBit(REG_CKG_SC1_FECLK_F2, DISABLE, CKG_SC1_FECLK_F2_INVERT);                   // Not Invert
            MDrv_WriteRegBit(REG_CKG_SC1_FECLK_F2, DISABLE, CKG_SC1_FECLK_F2_GATED);                    // Enable clock

            // Enable FClk
            MDrv_WriteByteMask(REG_CKG_SC1_FCLK, CKG_SC1_FCLK_123MHZ, CKG_SC1_FCLK_MASK);                   // select 170MHz, if not enough (for example, scaling bandwidth not enough), increase this
            MDrv_WriteRegBit(REG_CKG_SC1_FCLK, DISABLE, CKG_SC1_FCLK_INVERT);                           // Not Invert
            MDrv_WriteRegBit(REG_CKG_SC1_FCLK, DISABLE, CKG_SC1_FCLK_GATED);                            // Enable clock

            // Enable IDClk2
            MDrv_WriteByteMask(REG_CKG_SC1_IDCLK2, CKG_SC1_IDCLK2_XTAL, CKG_SC1_IDCLK2_MASK);               // initial to XTAL first, will be changed when switch input source
            MDrv_WriteRegBit(REG_CKG_SC1_IDCLK2, DISABLE, CKG_SC1_IDCLK2_INVERT);                       // Not Invert
            MDrv_WriteRegBit(REG_CKG_SC1_IDCLK2, DISABLE, CKG_SC1_IDCLK2_GATED);                        // Enable clock
            MDrv_Write2ByteMask(REG_CKG_SC1_FECLK2_F2, CKG_SC1_FECLK2_F2_SC1_FCLK, CKG_SC1_FECLK2_F2_MASK);  // enable ficlk2_f2

            // Enable SIDClk2
            MDrv_WriteRegBit(REG_CKG_SC1_SIDCLK2, DISABLE, CKG_SC1_SIDCLK2_INVERT);
            MDrv_WriteRegBit(REG_CKG_SC1_SIDCLK2, DISABLE, CKG_SC1_SIDCLK2_GATED);
        }
#else

#if(HW_DESIGN_4K2K_VER == 4)
        if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        {
            // Enable FIClk2
            MDrv_WriteByteMask(REG_CKG_FICLK_F2, CKG_FICLK_F2_FLK, CKG_FICLK_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_FICLK_F1_IDCLK2
            MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_INVERT);                   // Not Invert
            MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_GATED);                    // Enable clock

            // Enable FClk
            MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_DEFAULT, CKG_FCLK_MASK);                   // select 170MHz, if not enough (for example, scaling bandwidth not enough), increase this
            MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_INVERT);                           // Not Invert
            MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_GATED);                            // Enable clock

            // Enable IDClk2
            MDrv_WriteByteMask(REG_CKG_IDCLK2, CKG_IDCLK2_XTAL, CKG_IDCLK2_MASK);               // initial to XTAL first, will be changed when switch input source
            MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_INVERT);                       // Not Invert
            MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_GATED);                        // Enable clock

            // Enable EDClk
            Hal_SC_set_edclk(pInstance, CKG_EDCLK_F2_345MHZ, ENABLE, MAIN_WINDOW);
            Hal_SC_set_edclk(pInstance, CKG_EDCLK_F1_345MHZ, ENABLE, SUB_WINDOW);

            // Enable FMClk
            Hal_SC_set_fmclk(psXCInstPri, ENABLE);

            // Enable FIClk2_f2
            Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_FCLK, ENABLE, MAIN_WINDOW);

        }
        else
        {

#if SC2_ENABLE
#ifdef MONACO_SC2_PATCH
            // Enable FIClk2
            MDrv_WriteByteMask(REG_CKG_S2_FICLK2_F2, CKG_S2_FICLK2_F2_FCLK, CKG_S2_FICLK2_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_S2_FICLK_F1_IDCLK2
            MDrv_WriteRegBit(REG_CKG_S2_FICLK2_F2, DISABLE, CKG_S2_FICLK2_F2_INVERT);                   // Not Invert
            MDrv_WriteRegBit(REG_CKG_S2_FICLK2_F2, DISABLE, CKG_S2_FICLK2_F2_GATED);                    // Enable clock
#endif
#else
            // Enable FIClk2
            MDrv_WriteByteMask(REG_CKG_S2_FICLK_F2, CKG_S2_FICLK_F2_FLK, CKG_S2_FICLK_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_S2_FICLK_F1_IDCLK2
            MDrv_WriteRegBit(REG_CKG_S2_FICLK_F2, DISABLE, CKG_S2_FICLK_F2_INVERT);                   // Not Invert
            MDrv_WriteRegBit(REG_CKG_S2_FICLK_F2, DISABLE, CKG_S2_FICLK_F2_GATED);                    // Enable clock

#endif //#if SC2_ENABLE


            // Enable FClk
            MDrv_WriteByteMask(REG_CKG_S2_FCLK, CKG_S2_FCLK_DEFAULT, CKG_S2_FCLK_MASK);                   // select 170MHz, if not enough (for example, scaling bandwidth not enough), increase this
            MDrv_WriteRegBit(REG_CKG_S2_FCLK, DISABLE, CKG_S2_FCLK_INVERT);                           // Not Invert
            MDrv_WriteRegBit(REG_CKG_S2_FCLK, DISABLE, CKG_S2_FCLK_GATED);                            // Enable clock

            // Enable IDClk2
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, CKG_S2_IDCLK2_XTAL, CKG_S2_IDCLK2_MASK);               // initial to XTAL first, will be changed when switch input source
            MDrv_WriteRegBit(REG_CKG_S2_IDCLK2, DISABLE, CKG_S2_IDCLK2_INVERT);                       // Not Invert
            MDrv_WriteRegBit(REG_CKG_S2_IDCLK2, DISABLE, CKG_S2_IDCLK2_GATED);                        // Enable clock

            // Enable FIClk2_f2
            Hal_SC_set_ficlk2(pInstance, CKG_S2_FICLK2_F2_FCLK, ENABLE, MAIN_WINDOW);

#if SC2_ENABLE

#ifdef MONACO_SC2_PATCH
            // Enable reg_sc2_odclk_div_en
            MDrv_WriteByteMask(0x101E2C , 0x0E, 0x0E);
            MDrv_WriteByteMask(0x101E30 , 0x11, 0x11);
#endif

#endif //#if SC2_ENABLE

        }
#else
        // Enable FIClk2
        MDrv_WriteByteMask(REG_CKG_FICLK_F2, CKG_FICLK_F2_FLK, CKG_FICLK_F2_MASK);          // select FClk first, if pre-scaling down -> change to CKG_FICLK_F1_IDCLK2
        MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_FICLK_F2, DISABLE, CKG_FICLK_F2_GATED);                    // Enable clock

        // Enable FClk
        MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_DEFAULT, CKG_FCLK_MASK);                   // select 170MHz, if not enough (for example, scaling bandwidth not enough), increase this
        MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_INVERT);                           // Not Invert
        MDrv_WriteRegBit(REG_CKG_FCLK, DISABLE, CKG_FCLK_GATED);                            // Enable clock

        // Enable IDClk2
        MDrv_WriteByteMask(REG_CKG_IDCLK2, CKG_IDCLK2_XTAL, CKG_IDCLK2_MASK);               // initial to XTAL first, will be changed when switch input source
        MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_INVERT);                       // Not Invert
        MDrv_WriteRegBit(REG_CKG_IDCLK2, DISABLE, CKG_IDCLK2_GATED);                        // Enable clock

        // Enable EDClk
        Hal_SC_set_edclk(pInstance, CKG_EDCLK_F2_345MHZ, ENABLE, MAIN_WINDOW);
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
        Hal_SC_set_edclk(pInstance, CKG_EDCLK_F1_345MHZ, ENABLE, SUB_WINDOW);
        }
        // Enable FMClk
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);

        // Enable FIClk2_f2
        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_FCLK, ENABLE, MAIN_WINDOW);
#endif
#endif

        MDrv_SC_SetDisplay_LineBuffer_Mode(pInstance, ENABLE);
        MDrv_SC_SetDisplay_Start_Mode(pInstance, ENABLE);

        //Set line buffer merge address
        //This value will not changed. It is depend on line buffer size of main window.
        MDrv_SC_Set_LB_MergeAddress(pInstance);

        //For pip, need to enable sub line buffer first
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
        MDrv_XC_FilLineBuffer(pInstance, ENABLE, SUB_WINDOW);
        }
        // Set PAFRC mixed with noise dither disable---Move the control to pnl mod init
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK24_3F_L, 0x0, 0x8);

        //extra fetch between main and sub
        //if the setting value is too big, hw cannot catch up
        //by CC, it's safe to set 1366x768 panel the same with 1920x1080 panel.
        Hal_SC_Set_extra_fetch_line(pInstance, 0x05);//H:extra fetch line
        Hal_SC_Set_extra_adv_line(pInstance, 0x03);  //V:extra advance line

        MDrv_SC_SetSkipWaitVsync(pInstance, MAIN_WINDOW, FALSE);
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
        MDrv_SC_SetSkipWaitVsync(pInstance, SUB_WINDOW , FALSE);
        }
        // enable osd layer blending with main window
#if (VIDEO_OSD_SWITCH_VER > 1)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_23_L, BIT(6), BIT(6));
#endif
#ifdef K3_U2
        //Mantis issue of 0266525 . Need to toggle IP write mask field count clear before input V sync .
        //Main window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(4), BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(7));
        //Sub window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(4), BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(7));
#endif
        //if mweEnable not be called, sub win will be open default, we need to black it at first.
        //so that to avoid garbage when opening sub win.
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(5), BIT(5));
        gSrcInfo[SUB_WINDOW].bBlackscreenEnabled = TRUE;
#if SUPPORT_SEAMLESS_ZAPPING
        // for seamless zapping, this bit will maximize IP motion detection inside scaler
        // this motion setting will NOT override PQ's setting.
        // no need to be controlled by Qmap
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_45_L, BIT(9) , BIT(9));
#endif

        // this should only turned on when Top Bottom input with PreV scaling down enabled.
        // but the hardware's default value is ON so we turned off when inited.
        HAL_SC_Set_vsd_3D_autofactor_reset(pInstance, DISABLE, MAIN_WINDOW);

        // Enable IPM tune after DS feature
        // Initial setting recommand by designer
        MDrv_XC_EnableIPMTuneAfterDS(pInstance, DISABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_05_L, 0x03FF , 0x03FF);

#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        // default frame buffer address init
        MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size, MAIN_WINDOW);
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
        MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size, SUB_WINDOW);
        }
        
#if (HW_DESIGN_4K2K_VER == 4)
        // default frcm frame buffer address init
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
        MDrv_XC_SetFRCMFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Start_Addr,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Size, MAIN_WINDOW);
        MDrv_XC_SetFRCMFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Start_Addr,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Size, SUB_WINDOW);
        }
#endif
        //when from mboot to AP, there is an OS switch, so we need to do interrupt handle again.
        /**
         * !!! Attention !!! The ISR interface is different from Chakra2 and Utopia.
         * In Chakra2, there are two para needed, MHAL_SavedRegisters and vector.
         * In Utopia, there is no parameter.
         */
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            MsOS_AttachInterrupt(E_INT_IRQ_DISP, MDrv_SC_isr);
        }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            MsOS_AttachInterrupt(E_INT_IRQ_DISP1, MDrv_SC1_isr);
        }
#ifndef MSOS_TYPE_LINUX_KERNEL
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            MsOS_EnableInterrupt(E_INT_IRQ_DISP);
        }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
        }
#endif
    }

#if FRC_INSIDE
    // In A5 new feature,pre-arbiter can support (IPM_W and IPS_R)/(IPM_R and IPS_W)/(OP_R and OPW)  use the same MIU request client
    //Others, total 6 MIU request client

    // Current setting is : Turn off the max request function of pre-arbiter(2 to 1)
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_63_L,0x0A00); // 0xFF00 // miu merge arbiter IP0
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_68_L,0x0A00); // 0xFF00 // miu merge arbiter IP1
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_6A_L,0x1000); // 0xFF00 // miu merge arbiter OP
    MHal_FRC_3DLR_Select(pInstance, FALSE);
#endif

    //for HDMI AV mute, when AVMUTE = 1, HDMI will notice to XC to disable the input source on Main/Sub
    Hal_SC_Enable_AVMute(pInstance, MAIN_WINDOW);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
    Hal_SC_Enable_AVMute(pInstance, SUB_WINDOW);
    }
#if (HW_DESIGN_4K2K_VER == 4)
    // for FD_mask can control FRCM write
    Hal_SC_set_frcm_to_FD_mask(pInstance, ENABLE, MAIN_WINDOW);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
    Hal_SC_set_frcm_to_FD_mask(pInstance, ENABLE, SUB_WINDOW);
    }
    // for HW Auto No signal can control FRCM freeze
    Hal_SC_set_frcm_to_freeze(pInstance, ENABLE, MAIN_WINDOW);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
    Hal_SC_set_frcm_to_freeze(pInstance, ENABLE, SUB_WINDOW);
    }
#endif

    Hal_SC_Init(pInstance);

    Hal_SC_set_T3D_H_size(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);

    if(!(MHal_XC_Init_Patch(pInstance, ENABLE, MAIN_WINDOW)))
    {
        printf("no need to patch\n");
    }
    // SW Patch, default use 3 3 Frame buffer
    // Please reference MHal_XC_Init_Patch about SC_BK32_19 and SC_BK32_59
    gSrcInfo[MAIN_WINDOW].Status2.eFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
    gSrcInfo[SUB_WINDOW].Status2.eFRCMFrameStoreNumber  = IMAGE_STORE_3_FRAMES;
//    MApi_XC_SetCompressionMode(ENABLE, MAIN_WINDOW);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize the XC
/// If the length is 0, it won't do anything except the mutex and share memory. Usually it is the action of DFB
/// @param  pXC_InitData                  \b IN: the Initialized Data
/// @param  u32InitDataLen                \b IN: the length of the initialized data
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Init_U2(void* pInstance, XC_INITDATA *pXC_InitData, MS_U32 u32InitDataLen)
{
    char wordISR[] = {"_XC_ISR_Mutex"};


#if(XC_CONTEXT_ENABLE)
    MS_BOOL bFirstInstance = TRUE;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //MS_U8 u8VerCharIndex = 0 ;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#ifdef MSOS_TYPE_LINUX_KERNEL
    init_waitqueue_head(&pXCResourcePrivate->stXC_Utility._XC_EventQueue);
#endif

#if(XC_CONTEXT_ENABLE)
    _MApi_XC_Init_Context(pInstance, &bFirstInstance);
    if (!bFirstInstance)
    {
        //The XC instance already exists
        //return TRUE;
    }

    if (&(pXCResourcePrivate->stdrvXC_MVideo._SContext) == NULL || &(pXCResourcePrivate->stdrvXC_MVideo._SCShared) == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create context fail\n", __LINE__);
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return FALSE;
    }

#endif

    if (_XC_Mutex == -1)
    {
        _XC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_Mutex", MSOS_PROCESS_SHARED);
        if (_XC_Mutex == -1)
        {
            printf("[MAPI XC]_XC_Mutex create mutex fail\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create mutex fail\n", __LINE__);
            XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return FALSE;
        }
    }

    if (_XC_ISR_Mutex == -1)
    {
        _XC_ISR_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, wordISR, MSOS_PROCESS_SHARED);
        if (_XC_ISR_Mutex == -1)
        {
            printf("[MAPI XC]_XC_ISR_Mutex create mutex fail\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create ISR mutex fail\n", __LINE__);
            XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return FALSE;
        }
    }

    if (_PCMonitor_Mutex == -1)
    {
        _PCMonitor_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_PCMonitor_Mutex", MSOS_PROCESS_SHARED);
        if (_PCMonitor_Mutex == -1)
        {
            printf("[MAPI XC]_PCMonitor_Mutex create mutex fail\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[MAPI XC][%06d] create _PCMonitor_Mutex fail\n", __LINE__);
            XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return FALSE;
        }
    }


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    // This should be the first function. get MMIO base
    if (MDrv_XC_SetIOMapBase_i(pInstance) != TRUE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init MDrv_XC_SetIOMapBase() failure\n");
    }


#if SC2_ENABLE
    MDrv_XC_SetDeviceOffset(pInstance);
#else //SC2_ENABLE
    MDrv_XC_SetDeviceOffset();
#endif //#if SC2_ENABLE





    if (pXCResourcePrivate->sthal_SC.bIsGstreamerProcess == TRUE)
    {

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Skip the following init in Gstreamer process\n");
        return TRUE;
    }

    if (pXCResourcePrivate->sthal_SC.bIsCaptureProcess == TRUE)     {

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Skip the following init in Capture process\n");
        return TRUE;
    }

#if 1	
	if(((pXC_InitData->stPanelInfo.u16DefaultHTotal==0)
		||(pXC_InitData->stPanelInfo.u16DefaultVTotal==0)
		||(pXC_InitData->stPanelInfo.u32MinSET==0)
		||(pXC_InitData->stPanelInfo.u32MaxSET==0))
		&&
		((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal!=0)
		&&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal!=0)
		&&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET!=0))
		&&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET!=0))
	 {
	  	SC_W2BYTEMSK(0, REG_SC_BK03_42_L, 0x0044, 0x00FF);
		printf("get panel parameter failed %s %d  \n",__FUNCTION__,__LINE__);
		return TRUE;
	}
	else
	{
	}
#endif
    // If the length is 0, it won't do anything except the mutex and share memory. Usually it is the action of DFB
    // if the _bDFBInit is TRUE, it also indicates DFB init case

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    if((0 == u32InitDataLen) || (TRUE == pXCResourcePrivate->stdrvXC_MVideo._bDFBInit))
    {
        pXCResourcePrivate->stdrvXC_MVideo._bDFBInit = FALSE;

        mvideo_sc_variable_init(pInstance, bFirstInstance, NULL);

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Init(): the u32InitDataLen is zero or DFB init case.\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return TRUE;
    }
    MDrv_XC_FPLLCusReset(pInstance);

    pXCResourcePrivate->stdrvXC_MVideo_Context._bEnableEuro = FALSE;//Euro HDTV support flag

    if(MApi_PNL_Get_TCON_Capability())
    {
        if (pXC_InitData->stPanelInfo.eLPLL_Type >= E_XC_PNL_LPLL_EXT)
        {
            pXC_InitData->stPanelInfo.eLPLL_Type = (E_XC_PNL_LPLL_TYPE)MApi_Pnl_Get_LPLL_Type();
        }
        printf("%s, %d, pXC_InitData->stPanelInfo.eLPLL_Type=%d\n", __FUNCTION__, __LINE__, pXC_InitData->stPanelInfo.eLPLL_Type);
    }

    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData, pXC_InitData, ((u32InitDataLen>=sizeof(XC_INITDATA))? sizeof(XC_INITDATA):u32InitDataLen));

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitDataLen_Suspend = u32InitDataLen;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "Main_FB(start, size)=(%lx, %lu)\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "Sub_FB (start, size)=(%lx, %lu)\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size);

    // Scart ID port selection is available after version 1.
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XC_version < 1)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.eScartIDPort_Sel = E_XC_SCARTID_NONE;
    }


    mvideo_sc_variable_init(pInstance, bFirstInstance, pXC_InitData);
    _MApi_XC_Init_WithoutCreateMutex(pInstance, pXC_InitData, u32InitDataLen);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    //MApi_XC_SetDbgLevel(XC_DBGLEVEL_SETTIMING|XC_DBGLEVEL_SETWINDOW);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return TRUE;
}

MS_BOOL MApi_XC_Init(XC_INITDATA *pXC_InitData, MS_U32 u32InitDataLen)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_XC_KERNEL, &pu32XC_Kernel_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen Kernel XC failed\n");
            return FALSE;
        }
    }

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }
    stXC_INIT XCArgs;
    XCArgs.pXC_InitData = pXC_InitData;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
//   XCArgs.pXC_InitData = pXC_InitData;
//   XCArgs.u32InitDataLen = u32InitDataLen;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

E_APIXC_ReturnValue MDrv_XC_Init_MISC(void *pInstance, XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MST_PANEL_INFO_t stFRCPanelInfo;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if(u32InitMiscDataLen != sizeof(XC_INITMISC))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!SUPPORT_IMMESWITCH) && (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH ))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!SUPPORT_DVI_AUTO_EQ) && (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_DVI_AUTO_EQ) )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!FRC_INSIDE) &&
            ((pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE) || (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return E_APIXC_RET_FAIL;
    }

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // Start only when all setting ready.
    memset(&stFRCPanelInfo, 0, sizeof(MST_PANEL_INFO_t));

    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc, pXC_Init_Misc, u32InitMiscDataLen);
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitMiscDataLen_Suspend = u32InitMiscDataLen;

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FAST_GET_VFREQ)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFast_Get_VFreq = TRUE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFast_Get_VFreq = FALSE;
    }

#if (FRC_INSIDE)
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K))
    {
        if (FALSE == pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited)
        {
            MHal_CLKGEN_FRC_Init(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode);
            MDrv_FRC_PNLInfo_Transform(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo,
                                       &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc,
                                       &stFRCPanelInfo);
            // when OUTPUT is 4k2k@60Hz then we turn on FRC
            // in other case , we stil do FRC init but we choose to bypass FRC
            if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq < 600)
                    && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 3840)
                    && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 2160))
            {
                MDrv_FRC_ByPass_Enable(pInstance, TRUE);
            }
            else
            {
                MDrv_FRC_ByPass_Enable(pInstance, FALSE);
            }

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC) // init ready?! if yes, continue
            {
                MDrv_FRC_Init(pInstance, &stFRCPanelInfo, &(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo));
                gSrcInfo[MAIN_WINDOW].Status2.bInFRCMode = TRUE; //Store FRC enable state in share memory for multi-process
                gSrcInfo[SUB_WINDOW].Status2.bInFRCMode = gSrcInfo[MAIN_WINDOW].Status2.bInFRCMode;
                MHal_FRC_3DLR_Select(pInstance, TRUE);
            }
            pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited = TRUE;
        }

        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
        {
            // SW patch here, if AP set keep OP as 4K2K, need to adjust DE and Htt before FRC setting is 4K2K
            if (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) < 3000)
            {
                if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
                {

#if SC2_ENABLE
                    _MLOAD_ENTRY(pInstance);

#else //SC2_ENABLE
                    _MLOAD_ENTRY();
#endif //#if SC2_ENABLE



                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal, 0x1FFF);
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), 0x0FFF);

                    MDrv_XC_MLoad_Fire(pInstance, TRUE);

#if SC2_ENABLE
                    _MLOAD_RETURN(pInstance);

#else //SC2_ENABLE
                    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


                }
                else
                {
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal);
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
                }
            }
        }

        //printf("\n## FRC %s mode\n",g_XC_Pnl_Misc.FRCInfo.bFRC?"NORMAL":"BYPASS");
        //printf("## FRC LPLL mode =%d\n\n",g_XC_InitData.stPanelInfo.u8LPLL_Mode);
        //printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    }
    else
    {
        MDrv_FRC_ByPass_Enable(pInstance, TRUE);
        //printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    }
#endif
#if (HW_DESIGN_4K2K_VER == 4)

#if SC2_ENABLE
    if (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
            ||(psXCInstPri->u32DeviceID == 1))

#else //SC2_ENABLE
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE)

#endif //#if SC2_ENABLE

    {
        MDrv_SC_Enable_LegacyMode(pInstance, TRUE);
    }
    else
    {
        MDrv_SC_Enable_LegacyMode(pInstance, FALSE);
    }
#endif
    MHal_XC_Force_Mute_IPW(pInstance, TRUE, MAIN_WINDOW);
    //printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Init_MISC_U2(void* pInstance, XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    eReturn = MDrv_XC_Init_MISC(pInstance, pXC_Init_Misc, u32InitMiscDataLen);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    eReturn = MDrv_XC_Init_MISC(pInstance, pXC_Init_Misc, u32InitMiscDataLen);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return eReturn;
}


E_APIXC_ReturnValue MApi_XC_Init_MISC(XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_INIT_MISC XCArgs;
    XCArgs.pXC_Init_Misc = pXC_Init_Misc;
    XCArgs.u32InitMiscDataLen = u32InitMiscDataLen;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT_MISC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_GetMISCStatus_U2(void* pInstance, XC_INITMISC *pXC_Init_Misc)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(pXC_Init_Misc,&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc,sizeof(XC_INITMISC));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(pXC_Init_Misc,&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc,sizeof(XC_INITMISC));
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_GetMISCStatus(XC_INITMISC *pXC_Init_Misc)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_MISC_STATUS XCArgs;
    XCArgs.pXC_Init_Misc = pXC_Init_Misc;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_GetChipCaps_U2(void* pInstance, E_XC_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;

    eReturn = MDrv_XC_GetChipCaps(eCapType, pRet, ret_size);

    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_GetChipCaps(E_XC_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_CHIP_CAPS XCArgs;
    XCArgs.eCapType = eCapType;
    XCArgs.pRet = pRet;
    XCArgs.ret_size = ret_size;
    XCArgs.eReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CHIPCAPS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_U32 MApi_XC_GetCapability_U2(void* pInstance, MS_U32 u32Id)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    MS_U32 u32Val = 0;

    switch (u32Id)
    {
        case E_XC_SUPPORT_IMMESWITCH:
            MDrv_XC_GetChipCaps(E_XC_IMMESWITCH,&u32Val,sizeof(u32Val));
            break;

        case E_XC_SUPPORT_DVI_AUTO_EQ:
            MDrv_XC_GetChipCaps(E_XC_DVI_AUTO_EQ,&u32Val,sizeof(u32Val));
            break;

        case  E_XC_SUPPORT_FRC_INSIDE:
            MDrv_XC_GetChipCaps(E_XC_FRC_INSIDE,&u32Val,sizeof(u32Val));
            break;
        default:
            u32Val = 0;
            break;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return u32Val;
}

MS_U32 MApi_XC_GetCapability(MS_U32 u32Id)
{
    if (pu32XCInst == NULL)
    {
        XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_FULL);
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_GET_CAPABILITY XCArgs;
    XCArgs.u32Id = u32Id;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CAPABILITY, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}


//-------------------------------------------------------------------------------------------------
/// Enable sub window clock.
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableCLK_for_SUB(void *pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_XC_EnableCLK_for_SUB(pInstance, bEnable);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

//-------------------------------------------------------------------------------------------------
/// Enable DIP window clock.
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_SC_EnableCLK_for_DIP(pInstance, bEnable);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

//-------------------------------------------------------------------------------------------------
/// This function will change panel type dynamically
/// @param  pstPanelInfo      \b IN: the new panel type
//-------------------------------------------------------------------------------------------------
void MApi_XC_ChangePanelType_U2(void* pInstance, XC_PANEL_INFO *pstPanelInfo)
{
    XC_PANEL_INFO_EX stPanelInfoEx;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //Reset information used by ex panel info
    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
    stPanelInfoEx.u16VFreq = PANEL_INFO_EX_INVALID_ALL;
    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_SetExPanelInfo(pInstance, FALSE, &stPanelInfoEx);
    _XC_ENTRY(pInstance);
    MDrv_XC_GetDefaultHVSyncInfo(pInstance);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_SetExPanelInfo(pInstance, FALSE, &stPanelInfoEx);
    _XC_ENTRY();
    MDrv_XC_GetDefaultHVSyncInfo(pInstance);
    _XC_RETURN();
#endif //#if SC2_ENABLE




    if (pstPanelInfo->eLPLL_Type == E_XC_PNL_LPLL_EXT)
    {
        pstPanelInfo->eLPLL_Type = (E_XC_PNL_LPLL_TYPE)MApi_Pnl_Get_LPLL_Type();
    }
    //printf("%s, %d, pstPanelInfo->eLPLL_Type=%d\n", __FUNCTION__, __LINE__, pstPanelInfo->eLPLL_Type);

    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo, pstPanelInfo, sizeof(XC_PANEL_INFO));
    memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), &(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), sizeof(XC_PANEL_TIMING));
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"MApi_XC_ChangePanelType_U2 DE u16Width = %d, u16Height = %d\n", pstPanelInfo->u16Width, pstPanelInfo->u16Height);
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_ChangePanelType(XC_PANEL_INFO *pstPanelInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CHANGE_PANELTYPE XCArgs;
    XCArgs.pstPanelInfo = pstPanelInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CHANGE_PANELTYPE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function will let scaler driver know if to decide best pre-scaling down ratio based on FB size of not.
/// Please set this before call MApi_XC_SetWindow()
/// @param  bEnable      \b IN: enable this feature or not
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetAutoPreScaling_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].Status2.bAutoBestPreScaling = bEnable;
    //printf("MApi_XC_SetAutoPreScaling(): obsolete interface\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_SetAutoPreScaling(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_AUTO_PRESCALING XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_AUTO_PRESCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function will enable/diable output black pattern
/// @param  bEnable      \b IN: TRUE: enable, FALSE: otherwise
//-------------------------------------------------------------------------------------------------
void MApi_XC_Set_BLSK_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_SC_Set_BLSK(pInstance, bEnable);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_SC_Set_BLSK(pInstance, bEnable);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_Set_BLSK(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BLACKSCREEN XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BLACKSCREEN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


void MApi_XC_GenerateBlackVideoForBothWin_U2( void* pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    if(E_MLOAD_ENABLED == MDrv_XC_MLoad_GetStatus(pInstance))
    {
        MDrv_SC_GenerateBlackVideoForBothWin(pInstance, bEnable);
    }
    else
    {
        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
        MDrv_SC_GenerateBlackVideo(pInstance, bEnable, MAIN_WINDOW);
        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, SUB_WINDOW);
        MDrv_SC_GenerateBlackVideo( pInstance, bEnable, SUB_WINDOW);
    }
    gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled = bEnable;
    gSrcInfo[SUB_WINDOW].bBlackscreenEnabled = bEnable;

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_GenerateBlackVideoForBothWin( MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BOTHWINDOW_BLACKVIDEO XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BOTHWINDOW_BLACKVIDEO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function will enable/diable output black pattern
/// @param  bEnable      \b IN: TRUE: enable, FALSE: otherwise
//-------------------------------------------------------------------------------------------------
void MApi_XC_GenerateBlackVideo_U2( void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();

#endif //#if SC2_ENABLE

    //wait VSync and do setting in Vblanking to avoid broken picture
#if 0
    if(MDrv_XC_MLoad_GetStatus(pInstance) != E_MLOAD_ENABLED)
    {
        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
    }
#endif
    // control both window for URSA9 + 3D case
    if((eWindow == MAIN_WINDOW) && ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM) || (pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)))
    {
        eWindow = MAX_WINDOW;
    }

    if ( eWindow < MAX_WINDOW-1 )
    {
        MDrv_SC_GenerateBlackVideo(pInstance, bEnable, eWindow );
        gSrcInfo[eWindow].bBlackscreenEnabled = bEnable;
    }
    else
    {
        MDrv_SC_GenerateBlackVideoForBothWin(pInstance, bEnable);
        gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled = gSrcInfo[SUB_WINDOW].bBlackscreenEnabled = bEnable;
    }

#if TEST_NEW_DYNAMIC_NR   //Why turn on/of DNR here, but not in SetBlueScreen?
    // DNR should be turned on only if video is avaliable.m
    if (mvideo_sc_is_enable_3dnr(pInstance, MAIN_WINDOW))
    {
        //printk("Enable 3DNR %bu (flag is 0x%x\n", !bEnable, g_SrcInfo.u16ColorFinetuneFlag & FINETUNE_3DNR);
        MDrv_Scaler_Enable3DNR(pInstance, !bEnable, MAIN_WINDOW);
    }
#endif

#if 0
    // Enable FPD function when source is MVOP ouput & "Not" in FBL mode.
    {
        if( (IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType) ) &&
                (!MDrv_XC_IsCurrentFrameBufferLessMode()))
        {
            MApi_XC_set_FD_Mask( !bEnable );
        }
    }
#endif

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_GenerateBlackVideo ( MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BLACKVIDEO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BLACKVIDEO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Query whether current XC is black video enabled or not
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsBlackVideoEnable_U2( void* pInstance, SCALER_WIN eWindow )
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    MS_BOOL bEnable = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bEnable = gSrcInfo[eWindow].bBlackscreenEnabled;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bEnable = gSrcInfo[eWindow].bBlackscreenEnabled;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bEnable;
}

MS_BOOL MApi_XC_IsBlackVideoEnable( SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_BLACKVIDEO_ENABLE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_BLACKVIDEO_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// wait for input sync
/// @param  u8NumVSyncs        \b IN: the number of VSync we are going to wait
/// @param  u16Timeout         \b IN: time out
/// @return @ref MS_U8 retun the number of left Vsync if timeout
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_WaitInputVSync_U2(void* pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    MS_U8 u8InputVsyncTime;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    u8InputVsyncTime = MDrv_XC_wait_input_vsync(pInstance, u8NumVSyncs, u16Timeout, eWindow);
    //return MDrv_XC_wait_input_vsync(u8NumVSyncs, u16Timeout, eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    u8InputVsyncTime = MDrv_XC_wait_input_vsync(pInstance, u8NumVSyncs, u16Timeout, eWindow);
    //return MDrv_XC_wait_input_vsync(u8NumVSyncs, u16Timeout, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return u8InputVsyncTime;
}

MS_U8 MApi_XC_WaitInputVSync(MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }

    stXC_SET_WAIT_INPUT_VSYNC XCArgs;
    XCArgs.u8NumVSyncs = u8NumVSyncs;
    XCArgs.u16Timeout = u16Timeout;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0xFF;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_WAIT_INPUT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0xFF;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// wait for output sync
/// @param  u8NumVSyncs        \b IN: the number of VSync we are going to wait
/// @param  u16Timeout         \b IN: time out
/// @return @ref MS_U8 retun the number of left Vsync if timeout
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_WaitOutputVSync_U2(void* pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    MS_U8 u8OutputVsyncTime;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    u8OutputVsyncTime = MDrv_XC_wait_output_vsync(pInstance, u8NumVSyncs, u16Timeout, eWindow);
    //return MDrv_XC_wait_output_vsync(u8NumVSyncs, u16Timeout, eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    u8OutputVsyncTime = MDrv_XC_wait_output_vsync(pInstance, u8NumVSyncs, u16Timeout, eWindow);
    //return MDrv_XC_wait_output_vsync(u8NumVSyncs, u16Timeout, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return u8OutputVsyncTime;
}

MS_U8 MApi_XC_WaitOutputVSync(MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }

    stXC_SET_WAIT_OUTPUT_VSYNC XCArgs;
    XCArgs.u8NumVSyncs = u8NumVSyncs;
    XCArgs.u16Timeout = u16Timeout;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0xFF;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_WAIT_OUTPUT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0xFF;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// wait for output sync
/// @param  u8NumVSyncs        \b IN: the number of VSync we are going to wait
/// @param  u16Timeout         \b IN: time out
/// @return @ref MS_U8 retun the number of left Vsync if timeout
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_XC_WaitOutputVSync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    MS_U8 u8OutputVsyncTime;
    u8OutputVsyncTime = MDrv_XC_wait_output_vsync(pInstance, u8NumVSyncs, u16Timeout, eWindow);
    return u8OutputVsyncTime;
}

void msAPI_Scaler_Field_Detect(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrcType, EN_VIDEO_SCAN_TYPE enScanType, SCALER_WIN eWindow)
{

    MS_U8 reg_IP1F2_1D, reg_IP1F2_23;
    MS_U16 reg_IP1F2_21;
    MS_U16 u16Vtt = 0;

    // Note:
    // IP1F2_1D[13]: Enable auto no signal filter mode.
    // This functionality is to improve timing detection stability.

    if(IsSrcTypeDigitalVD(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "VD \n");
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_21 = 0x0403;
        reg_IP1F2_23 = 0x30;
    }
    else if(IsSrcTypeDTV(enInputSrcType) || IsSrcTypeStorage(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "DTV ");
        if (enScanType == SCAN_INTERLACE)
        {
            reg_IP1F2_1D = 0xA1;
            reg_IP1F2_21 = 0x0403;
            reg_IP1F2_23 = 0x30;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0400;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }
    }
    else if (IsSrcTypeHDMI(enInputSrcType))
    {

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "HDMI ");
        if ((enScanType == SCAN_INTERLACE)
                || (MDrv_SC_GetInterlaceInPModeStatus(pInstance, eWindow)
                    && (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW)))
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0003;      // enable DE -> no field invert, disable DE -> use field invert
            reg_IP1F2_23 = 0x20;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");

            u16Vtt = MDrv_SC_ip_get_verticaltotal(pInstance, eWindow);
            if((u16Vtt/2)%2 )
            {
                reg_IP1F2_21 |= BIT(8);
            }
            else
            {
                reg_IP1F2_21 &= ~BIT(8);
            }
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }

        reg_IP1F2_21 |= BIT(12);
        if( (MApi_XC_GetHdmiSyncMode(eWindow) == HDMI_SYNC_HV)
                &&(enScanType == SCAN_INTERLACE) )
        {
            XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
            UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
            MS_U16 u16Cur_IP1F2_21Val = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L);
            if( u16Cur_IP1F2_21Val&0xC000 )
            {
                if( reg_IP1F2_21&BIT(8) )
                {
                    reg_IP1F2_21 &= ~BIT(8);
                }
                else
                {
                    reg_IP1F2_21 |= BIT(8);
                }
                //printf("daniel_test patch REG_SC_BK01_21_L: FieldInvert\n");
            }
            else
            {
            }
        }

    }
    else if(IsSrcTypeVga(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "DSUB ");
        if (enScanType == SCAN_INTERLACE)
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0103;
            reg_IP1F2_23 = 0x10;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }
    }
    else if (IsSrcTypeCapture(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "DIP ");
        reg_IP1F2_21 = 0x0101;//IP1 sometimes will detect opcapture to interlace,we force it to progressive
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_23 = 0x08; //0x20  // Vtt will use "IP1F2_23 * 16" as lower bound.
    }
    else // YPbPr
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "YPbPr ");
        if(MDrv_XC_ip_GetEuroHdtvStatus(pInstance, MAIN_WINDOW) == EURO_AUS_HDTV_NORMAL)
        {
            reg_IP1F2_21 = 0x0100;
        }
        else
        {
            reg_IP1F2_21 = 0x0000;
            //reg_IP1F2_21 = 0x0100;
        }
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_23 = 0x08; //0x20  // Vtt will use "IP1F2_23 * 16" as lower bound.
    }


    MDrv_SC_setfield(pInstance, reg_IP1F2_1D, reg_IP1F2_21, reg_IP1F2_23, eWindow);

}

//-------------------------------------------------------------------------------------------------
/// Get the specific window's sync status
/// @param  eCurrentSrc             \b IN: the current input source
/// @param  sXC_Sync_Status         \b OUT:store the sync status
/// @param  eWindow                 \b IN: which window we are going to get
//-------------------------------------------------------------------------------------------------
void MApi_XC_GetSyncStatus_U2(void* pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_GetSyncStatus(pInstance, eCurrentSrc, sXC_Sync_Status, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_XC_GetSyncStatus(pInstance, eCurrentSrc, sXC_Sync_Status, eWindow);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return;
}

void MApi_XC_GetSyncStatus(INPUT_SOURCE_TYPE_t eCurrentSrc, XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_SYNC_STATUS XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.sXC_Sync_Status = sXC_Sync_Status;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SYNC_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_GetSyncStatus(void *pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    E_MUX_INPUTPORT _enPort = MDrv_XC_Mux_GetHDMIPort(pInstance, eCurrentSrc);

    MDrv_XC_ip_get_sync_status(pInstance, sXC_Sync_Status, _enPort , eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

//-------------------------------------------------------------------------------------------------
/// Set input source type
/// @param  enInputSourceType      \b IN: which input source typewe are going to set
/// @param  eWindow                \b IN: which window we are going to set
/// @return void
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetInputSource_U2( void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,SCALER_WIN eWindow )
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow] == enInputSourceType)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "MApi_XC_SetInputSource[Win %u] return because old src %d -> %d\n"
                     , eWindow
                     , pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow], enInputSourceType);

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow] = enInputSourceType;
    }

    MS_U8 u8InputSrcSel, u8SyncSel, u8VideoSel=0, u8isYPbPr, u8HankShakingMD;

    //Disable new mode interlaced detect before set input source
    MDrv_XC_SetNewModeInterlacedDetect(pInstance, DISABLE, 0, DISABLE, FALSE, FALSE, eWindow);

    MDrv_SC_ForceInterlaceInPMode(pInstance, DISABLE,eWindow);
    //IP1 sometimes will detect opcapture to interlace,we force it to progressive
    Hal_SC_ip_set_user_def_interlace_status(pInstance, DISABLE, DISABLE,eWindow);


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE



#if SUPPORT_IP_HDMI_FOR_HV_MODE
    // Should disable "pixel repetition" for other source
    if(!(IsSrcTypeHDMI(enInputSourceType) || IsSrcTypeDVI(enInputSourceType)))
    {
        Hal_SC_ip_set_fir_down_sample_divider(pInstance, DISABLE,0x00,eWindow); //Reset Pixel repetition, when switch hv mode
    }
#endif

    if(IsSrcTypeVga(enInputSourceType))
    {
        u8InputSrcSel = IP_ANALOG1;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = FALSE;
        u8HankShakingMD = FALSE;
        MApi_XC_ADC_SwitchSource(pInstance,1); // Force ADC to reset after source switch
    }
    else if(IsSrcTypeYPbPr(enInputSourceType))
    {
        u8InputSrcSel = IP_ANALOG1;
        u8SyncSel = (SYNC_ON_GREEN << 1) | SOG;
        u8isYPbPr = TRUE;
        u8HankShakingMD = FALSE;
        MApi_XC_ADC_SwitchSource(pInstance,1); // Force ADC to reset after source switch
    }
    else if(IsSrcTypeHDMI(enInputSourceType))
    {
#if SUPPORT_IP_HDMI_FOR_HV_MODE
        // set to DE mode to sync with IP_HDMI below, to make system status sync.
        // to fix hdmi 720p-->atv-->720p, then MDrv_XC_Switch_DE_HV_Mode_By_Timing()
        // won't set IP_VIDEO.
        //first time bootup and do 3d conversion, sub window will be set to hdmi also.
        //and sub window will override the HV mode of main. wrong
        //HV mode will be taken as a whole system feature, not main/sub window's
        //beside, we have only one HDMI engine.
        if(!((eWindow == SUB_WINDOW) && (IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW]))))
        {
            MDrv_XC_SetHdmiSyncMode(pInstance, HDMI_SYNC_DE,eWindow);
        }
        u8InputSrcSel = IP_HDMI;
        u8VideoSel = IP_HDMI;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
#else
        if( MDrv_XC_GetHdmiSyncMode(pInstance,eWindow) == HDMI_SYNC_DE)
        {
            u8InputSrcSel = IP_HDMI;
            u8VideoSel = IP_HDMI;
            u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        }
        else
        {
            u8InputSrcSel = IP_VIDEO;
            u8VideoSel = IP_MST_VD_A;
            u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        }
#endif
        u8isYPbPr = TRUE;
        u8HankShakingMD = FALSE;

    }
    else if(IsSrcTypeDigitalVD(enInputSourceType))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_MST_VD_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
        u8HankShakingMD = FALSE;
    }
#ifdef __EXTVD
    else if(IsUseExtVDPort(enInputPortType))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_CCIR656_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
        u8HankShakingMD = FALSE;
    }
#endif
    else if(IsSrcTypeDTV(enInputSourceType) || (IsSrcTypeStorage(enInputSourceType)))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_MST_VD_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
        u8HankShakingMD = TRUE;
    }
    else if(IsSrcTypeCapture(enInputSourceType))
    {
        u8InputSrcSel = IP_DVI; // DIP uses IP_HDTV
        u8VideoSel = IP_CCIR656_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = FALSE;
        u8HankShakingMD = FALSE;
        MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
#ifdef K3_U2
        MDrv_SC_ip_set_input_sync_reference_edge( pInstance, TAILING_EDGE, TAILING_EDGE, eWindow );
#endif
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        //IP1 sometimes will detect opcapture to interlace,we force it to progressive
        Hal_SC_ip_set_user_def_interlace_status(pInstance, ENABLE, DISABLE, eWindow);
        gSrcInfo[eWindow].enInputSourceType = enInputSourceType;

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return;
    }
    else
    {
        printf("Unknow Input Type \n");
        //main: vga, switch to browser(storage), select sub source to ypbpr
        //sub display garbage. because main's source select is still in vga status, conflict.
        //Thus, we need to set main to a resevered on: IP_HDTV in mapi_video_base::finalize()
        // of MApi_XC_SetInputSource(INPUT_SOURCE_NONE, SDK2DriverScalerWinTypeTrans(m_enDstWin));
        u8InputSrcSel = IP_HDTV; //by CC, treat IP_HDTV as reserved
        u8VideoSel = 0;
        u8SyncSel = 0;
        u8isYPbPr = 0;
        u8HankShakingMD = 0;
        MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
        //IP1 sometimes will detect opcapture to interlace,we force it to progressive
        if( IsSrcTypeCapture(enInputSourceType))
        {
            Hal_SC_ip_set_user_def_interlace_status(pInstance, ENABLE, DISABLE, eWindow);
        }

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return;
    }

    MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
    if(IsSrcTypeVga(enInputSourceType))
    {
        MDrv_SC_ip_set_image_wrap(pInstance,  DISABLE, DISABLE, eWindow );
    }
    else
    {
        MDrv_SC_ip_set_image_wrap(pInstance,  ENABLE, ENABLE, eWindow );
    }
    MDrv_SC_ip_set_handshaking_md(pInstance, u8HankShakingMD, eWindow);
    MDrv_SC_ip_set_de_bypass_mode(pInstance, DISABLE, eWindow); // fix HDMI <-> component switch problems

    if(IsSrcTypeVga(enInputSourceType))
    {
        MDrv_SC_ip_set_input_sync_reference_edge(pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        MDrv_ip_set_input_10bit(pInstance, ENABLE, eWindow );

        MDrv_SC_ip_set_coast_input(pInstance, PC_SOURCE, eWindow );
        MDrv_SC_ip_set_ms_filter(pInstance, ENABLE,0,eWindow);
        MDrv_SC_ip_set_coast_window(pInstance, 0x0C, 0x0C, eWindow );
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, DISABLE, 0x00, eWindow );
    }
    else if(IsSrcTypeYPbPr(enInputSourceType))
    {
        MDrv_SC_ip_set_input_sync_reference_edge(pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        MDrv_SC_ip_set_input_vsync_delay(pInstance, NO_DELAY, eWindow );
        MDrv_SC_ip_set_ms_filter(pInstance, ENABLE,0,eWindow);
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        MDrv_ip_set_input_10bit(pInstance, ENABLE, eWindow );

        MDrv_SC_ip_set_coast_input(pInstance, COMPONENT_VIDEO, eWindow );
        MDrv_SC_ip_set_coast_window(pInstance, 0x0C, 0x0C, eWindow );
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, GLITCH_REMOVAL, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x01, eWindow );
    }
    else if(IsSrcTypeHDMI(enInputSourceType))
    {
        MDrv_SC_ip_set_input_sync_reference_edge(pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
#ifdef K3_U2
#else
        Hal_HDMI_Set_YUV422to444_Bypass(TRUE);
#endif

        if( MDrv_XC_GetHdmiSyncMode(pInstance,eWindow) == HDMI_SYNC_DE)
        {
            MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
            MDrv_SC_ip_set_de_only_mode(pInstance, ENABLE, eWindow );
            MDrv_SC_ip_set_de_bypass_mode(pInstance, DISABLE, eWindow);
            MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
            MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x07, eWindow );
#ifdef K3_U2
            Hal_HDMI_Set_YUV422to444_Bypass(TRUE);
#endif
        }
        else
        {
            MDrv_SC_ip_set_input_vsync_delay(pInstance, NO_DELAY, eWindow );
            MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
            // Use DE as capture window
            MDrv_SC_ip_set_de_bypass_mode(pInstance, ENABLE, eWindow);
            // Disable scaling for get correct DE.
            //MDrv_SC_Enable_PreScaling(FALSE, FALSE, eWindow);
            // Set full range capture size for de-bypass mode
            //MDrv_SC_SetFullRangeCapture(eWindow);
            MDrv_SC_ip_set_input_sync_sample_mode(pInstance, GLITCH_REMOVAL, eWindow );
            MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x01, eWindow );
#ifdef K3_U2
            Hal_HDMI_Set_YUV422to444_Bypass(FALSE);
#endif
        }

        MDrv_ip_set_input_10bit(pInstance, ENABLE, eWindow );

        MDrv_SC_ip_set_ms_filter(pInstance, DISABLE,0,eWindow);

        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        MDrv_SC_ip_set_ms_filter(pInstance, DISABLE,0,eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x07, eWindow );
    }
    else if(IsSrcTypeDigitalVD(enInputSourceType)
#ifdef __EXTVD
            | IsUseExtVDPort(enInputPortType)
#endif
           )
    {
        MDrv_SC_ip_set_input_sync_reference_edge(pInstance, TAILING_EDGE, TAILING_EDGE, eWindow );
        MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );

#ifdef __EXTVD
        if(IsUseExtVDPort(enInputPortType))
        {
            MS_U16 u16tmp;
            MDrv_ip_set_input_10bit(pInstance, DISABLE, eWindow );

            //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
            //MDrv_WriteByte(L_BK_IP1F2(0x08), (MDrv_ReadByte(L_BK_IP1F2(0x08)) & 0x7F));
            //u16tmp = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_08_L);
            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_08_L, u16tmp & 0x007F, 0x00FF);
        }
        else
#endif

        {
            MDrv_ip_set_input_10bit(pInstance, ENABLE, eWindow );
        }
        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        MDrv_SC_ip_set_ms_filter(pInstance, DISABLE,0,eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, DISABLE, 0x00, eWindow );
    }
    else //DTV
    {
        //set HSYNC reference to leading edge, the capature start will be 1/2 Hsync when input source is from DC0/DC1
        MDrv_SC_ip_set_input_sync_reference_edge(pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        MDrv_ip_set_input_10bit(pInstance, ENABLE, eWindow );

        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        MDrv_SC_ip_set_ms_filter(pInstance, DISABLE,0,eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, DISABLE, 0x00, eWindow );
    }

    // set  h0027 [2] : true to enable DE only mode Glitch Protect for position
    // to fix HDMI 720P YUV422 HStart error & HDE lack of 1
    if (IsSrcTypeHDMI(enInputSourceType))
        MDrv_SC_ip_set_DE_Mode_Glitch(pInstance, 0x24 , eWindow ); // VSync glitch removal with line less than 2(DE only)
    else
        MDrv_SC_ip_set_DE_Mode_Glitch(pInstance, 0x04 , eWindow ); // CHANNEL_ATTRIBUTE function control

    msAPI_Scaler_Field_Detect(pInstance, enInputSourceType, SCAN_AUTO, eWindow );

    MDrv_XC_reset_ip(pInstance, eWindow );

    gSrcInfo[eWindow].enInputSourceType = enInputSourceType;
    XC_PRINTF("[%s,%5d] gSrcInfo[eWindow].enInputSourceType:%u \n",__func__,__LINE__,gSrcInfo[eWindow].enInputSourceType);

#if 0
    if(g_bIMMESWITCH_DVI_POWERSAVING)
    {
        if((enInputSourceType < INPUT_SOURCE_HDMI || enInputSourceType > INPUT_SOURCE_HDMI4))
            MDrv_DVI_ForceAllPortsEnterPS();
        else
            u8PSDVIStableCount[enInputSourceType - INPUT_SOURCE_HDMI] = 0;
    }
#endif

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_SetInputSource( INPUT_SOURCE_TYPE_t enInputSourceType,SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_INPUTSOURCE XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_INPUTSOURCE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


void MApi_XC_Set_FieldDetect_U2(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrcType, EN_VIDEO_SCAN_TYPE enScanType, SCALER_WIN eWindow)
{
    msAPI_Scaler_Field_Detect(pInstance, enInputSrcType, enScanType, eWindow );
}

void MApi_XC_Set_FieldDetect(INPUT_SOURCE_TYPE_t enInputSrcType, EN_VIDEO_SCAN_TYPE enScanType, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_Field_Detect XCArgs;
    XCArgs.enInputSourceType = enInputSrcType;
    XCArgs.enVideoScanType = enScanType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FIELD_DETECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

static void msAPI_Scaler_SetMode(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow )
{
    MS_BOOL bFBL = pSrcInfo->bFBL;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_BOOL bUseYUVSpace;
    //MS_BOOL bBlackscreenEnabled;
    MS_U8 interlace_type;
    static MS_U8 gu8interlace_type;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //----------------------------------------------------
    // Setup Memory format
    //----------------------------------------------------
    if(pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[eWindow] == E_MS_XC_MEM_FMT_AUTO)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "E_MS_XC_MEM_FMT_AUTO\n");
        if(!bInterlace &&
                ((IsSrcTypeHDMI(enInputSourceType) && !g_HdmiPollingStatus.bIsHDMIMode) || // DVI plugged into HDMI connector
                 IsSrcTypeVga(enInputSourceType)  ) )
        {   // always keep memory foramt to 444 for corresponding to Y/C post-scaling filter
            pSrcInfo->bMemFmt422 = FALSE;
        }
        else
        {   // if input source is 422, HW will convert to 444 automatically
            pSrcInfo->bMemFmt422 = TRUE;
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "E_MS_XC_MEM_FMT_422\n");
        if(pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[eWindow] == E_MS_XC_MEM_FMT_422)
            {
            pSrcInfo->bMemFmt422 = TRUE;
            }
        else
            {
            pSrcInfo->bMemFmt422 = FALSE;
    }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR); //K3_PQ

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)); //K3_PQ

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MemFmt422=%u\n", (MS_U16)pSrcInfo->bMemFmt422);
#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc == NULL)
            bUseYUVSpace = 0;
        else
            bUseYUVSpace = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc(eWindow == SUB_WINDOW ? PQ_SUB_WINDOW : PQ_MAIN_WINDOW,
                           pSrcInfo->bForceRGBin == ENABLE ? PQ_FOURCE_COLOR_RGB : PQ_FOURCE_COLOR_DEFAULT);
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc_ex == NULL)
            bUseYUVSpace = 0;
        else
            bUseYUVSpace = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc_ex(psXCInstPri->u32DeviceID, eWindow == SUB_WINDOW ? PQ_SUB_WINDOW : PQ_MAIN_WINDOW,
                           pSrcInfo->bForceRGBin == ENABLE ? PQ_FOURCE_COLOR_RGB : PQ_FOURCE_COLOR_DEFAULT);
    }
#endif

    if (bUseYUVSpace)
    {
        pSrcInfo->bUseYUVSpace = TRUE;
    }
    else
    {
        pSrcInfo->bUseYUVSpace = FALSE;
    }

#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt == NULL)
            pSrcInfo->bMemYUVFmt =  TRUE;
        else
            pSrcInfo->bMemYUVFmt = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt(eWindow == SUB_WINDOW ? PQ_SUB_WINDOW : PQ_MAIN_WINDOW,
                                   pSrcInfo->bForceRGBin == ENABLE ? PQ_FOURCE_COLOR_RGB : PQ_FOURCE_COLOR_DEFAULT);
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt_ex == NULL)
            pSrcInfo->bMemYUVFmt =  TRUE;
        else
            pSrcInfo->bMemYUVFmt = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt_ex(psXCInstPri->u32DeviceID, eWindow == SUB_WINDOW ? PQ_SUB_WINDOW : PQ_MAIN_WINDOW,
                                   pSrcInfo->bForceRGBin == ENABLE ? PQ_FOURCE_COLOR_RGB : PQ_FOURCE_COLOR_DEFAULT);
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "bMemYUVFmt=%u, RGBin=%u\n", pSrcInfo->bMemYUVFmt, pSrcInfo->bForceRGBin);

    if((pSrcInfo->bMemYUVFmt != TRUE) && (eWindow == SUB_WINDOW)) //IP_CSC output is RGB, so skip 444 to 422
    {
        pSrcInfo->bMemFmt422 = FALSE;//Now, only care about Sub case to reduce side effect
    }
    //-------------------------------------------
    // Set De-interlaced mode and Memory format
    //-------------------------------------------
    //printf("==> Set PQ MEMORY Mode: %d\n", eWindow);

#ifdef K3_U2
    // Tempararily not dumping MADi PQ table in seamless zapping
    if((Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow)) && (Hal_SC_get_freezeimg(pInstance, eWindow)))
    {
        Hal_SC_SetPX2MemFormat(pInstance, eWindow);
        interlace_type = PQ_DEINT_3DDI_HISTORY;
        pSrcInfo->u8BitPerPixel = 24;

        //PQ_IP_MemFormat_Main 422MF
        HAL_SC_set_memory_bit_fmt(pInstance,FALSE,FALSE);
        //444 to 422 (PQ_IP_444To422_Main) (on)
        HAL_SC_ip_set_444to422_filter_mod(pInstance,TRUE);
        //422To444(PQ_IP_422To444_Main on)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0C_L, 0x80 ,0x0080);
    }
    else
    {
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID) // SC0
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat == NULL)
                 interlace_type = 0;
            else
                    interlace_type = (MS_U8) s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat(
                                         eWindow == SUB_WINDOW ?
                                         PQ_SUB_WINDOW :
                                         PQ_MAIN_WINDOW,
                                         pSrcInfo->bMemFmt422,
                                         bFBL,
                                         &(pSrcInfo->u8BitPerPixel));
        }
        else // SC1
        {
            Hal_SC_SetPX2MemFormat(pInstance, eWindow);
            interlace_type = PQ_DEINT_3DDI_HISTORY;
            pSrcInfo->u8BitPerPixel = 24;
        }
    }
#else

#if SC2_ENABLE
#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if (pSrcInfo->b3DMenuload)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat == NULL)
                   interlace_type = 0;
            else
            {


                    interlace_type = (MS_U8) s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat(
                                         eWindow == SUB_WINDOW ?
                                         PQ_SUB_WINDOW :
                                         PQ_MAIN_WINDOW,
                                         pSrcInfo->bMemFmt422,
                                         bFBL,
                                         &(pSrcInfo->u8BitPerPixel ));
                    gu8interlace_type = interlace_type;

            }
        }
        if (pSrcInfo->b3DMenuload == 0)
        {
            interlace_type = gu8interlace_type;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D crop menuload for PQ memory format config \n");
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat_ex == NULL)
            interlace_type = 0;
        else
            interlace_type = (MS_U8) s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat_ex(psXCInstPri->u32DeviceID,
                             eWindow == SUB_WINDOW ?
                             PQ_SUB_WINDOW :
                             PQ_MAIN_WINDOW,
                             pSrcInfo->bMemFmt422,
                             bFBL,
                             &(pSrcInfo->u8BitPerPixel));
    }
#endif
#else
    if (pSrcInfo->b3DMenuload)
    {
    if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat == NULL)
        interlace_type = 0;
    else
        interlace_type = (MS_U8) s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat(
            eWindow == SUB_WINDOW ?
            PQ_SUB_WINDOW :
            PQ_MAIN_WINDOW,
            pSrcInfo->bMemFmt422,
            bFBL,
            &(pSrcInfo->u8BitPerPixel));
            }
    if (pSrcInfo->b3DMenuload)
    {
        gu8interlace_type = interlace_type;
    }
    else
    {
        interlace_type = gu8interlace_type;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D crop menuload for PQ memory format config \n");
    }
#endif
#if SUPPORT_SEAMLESS_ZAPPING
    Hal_SC_SetPX2MemFormat(pInstance, eWindow);
    if (IsVMirrorMode(eWindow))
    {
        MS_U16 u16IPM3DLRFlags = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L);
        // 2D mode
        if(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE || MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_MODE_NONE)
        {
            if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
            {
                if((u16IPM3DLRFlags&BIT(4)) == 0)
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(4), BIT(4));//F2 IPM 3D LR invert
                }
            }
            else
            {
                if((u16IPM3DLRFlags&BIT(4)) == BIT(4))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0, BIT(4));//F2 IPM 3D LR invert
                }
            }
        }
    }
#endif
#endif


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR); //K3_PQ

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)); //K3_PQ

#endif //#if SC2_ENABLE

    pSrcInfo->eDeInterlaceMode = (MS_DEINTERLACE_MODE)interlace_type;
    //printf("DI mode =%x, bitPerPixel=%d\n",pSrcInfo->eDeInterlaceMode, pSrcInfo->u8BitPerPixel);
    if(bFBL && (pSrcInfo->stDispWin.height != pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height))
    {
        if(SUPPORT_SPECIAL_FRAMELOCK == FALSE)
        {
            MDrv_SC_set_std_display_window(pInstance, eWindow);  //only support full screen
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[FBL]Set to std_display_window(panel size, full screen) \n");
        }
    }


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    // set field shift line  // set shift line between even/odd field
    //if((OP1_RVal != OP1_IRE_80) && (OP1_RVal != OP1_IRE_100))
    {
    MDrv_SC_set_shift_line(pInstance, pSrcInfo->bFBL, pSrcInfo->eDeInterlaceMode, eWindow);
    }
    //------------------------------------
    // Setup Delay line
    //------------------------------------
    /* move to MDrv_SC_Set_LockFreeze_Point()
    if(bFBL)
    {
        pSrcInfo->u8DelayLines = 1;
        //printf("[FBL]Set delay line\n");
    }
    else if( (pSrcInfo->eDeInterlaceMode == MS_DEINT_2DDI_AVG) ||
             (pSrcInfo->eDeInterlaceMode == MS_DEINT_2DDI_BOB))
    {
        pSrcInfo->u8DelayLines = 1;    // 5
    }
    else
    {
        pSrcInfo->u8DelayLines = 2;
    }

    // Setup delay line
    Hal_SC_set_delayline( pSrcInfo->u8DelayLines, eWindow );           // set delay line for trigger point, for 2DDI
    */
    msAPI_Scaler_Field_Detect(pInstance, enInputSourceType, (pSrcInfo->bInterlace?SCAN_INTERLACE:SCAN_PROGRESSIVE), eWindow );

    //--------------
    // PIP related
    //--------------
    if(PIP_SUPPORTED)
    {
        if(eWindow == MAIN_WINDOW)
        {
            if(pSrcInfo->bInterlace == TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, BIT(13), BIT(13));
            }
            else
            {
#if SUPPORT_SEAMLESS_ZAPPING
                // If memory format is PX2 progressive mode, set to use interlace line buffer setting
                if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, BIT(13), BIT(13));
                }
                else
#endif
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, 0, BIT(13));
                }
            }
        }
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

//This function is used in internal (as driver), so do not set _XC_SEMAPHORE_ENTRY
void MApi_XC_Set_PQ_SourceData(void *pInstance,
                               SCALER_WIN eWindow,
                               INPUT_SOURCE_TYPE_t enInputSourceType,
                               XC_InternalStatus *pstXC_WinTime_Info)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    PQ_WIN ePQWin;
    PQ_INPUT_SOURCE_TYPE enPQSourceType=PQ_INPUT_SOURCE_NONE;
    MS_PQ_Mode_Info stPQModeInfo;

    switch (eWindow)
    {
        default:
        case MAIN_WINDOW:
            ePQWin = PQ_MAIN_WINDOW;
            break;

        case SUB_WINDOW:
            ePQWin = PQ_SUB_WINDOW;
            break;
    }

    if(IsSrcTypeVga(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_VGA; // VGA
    }
    else if(IsSrcTypeATV(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_TV; // TV
    }
    else if(IsSrcTypeDTV(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_DTV; //DTV
    }
    else if(IsSrcTypeAV(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_CVBS; // AV
    }
    else if(IsSrcTypeScart(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_SCART; // SCART
    }
    else if(IsSrcTypeSV(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_SVIDEO; // SV
    }
    else if(IsSrcTypeHDMI(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_HDMI; // HDMI
    }
    else if(IsSrcTypeYPbPr(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_YPBPR; // COMP
    }
    else if(IsSrcTypeJpeg(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_JPEG; // JPEG
    }
    else if(IsSrcTypeStorage(enInputSourceType))
    {
        enPQSourceType = PQ_INPUT_SOURCE_STORAGE; // Storage
    }
    else if(enInputSourceType == INPUT_SOURCE_KTV)
    {
        enPQSourceType = PQ_INPUT_SOURCE_KTV; // KTV
    }
    else
    {
        MS_ASSERT(0);
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PQSetSource:Input src=%u, win=%u,\n", enInputSourceType, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PQSetSource:   PQ src=%u, win=%u\n", enPQSourceType, eWindow);

    stPQModeInfo.bFBL               = pstXC_WinTime_Info->bFBL;
    stPQModeInfo.bInterlace         = pstXC_WinTime_Info->bInterlace;
    stPQModeInfo.u16input_hsize     = pstXC_WinTime_Info->stCapWin.width;
    stPQModeInfo.u16input_vsize     = pstXC_WinTime_Info->stCapWin.height;
    stPQModeInfo.u16input_vfreq     = pstXC_WinTime_Info->u16InputVFreq;
    stPQModeInfo.u16input_vtotal    = pstXC_WinTime_Info->u16InputVTotal;
    stPQModeInfo.u16ouput_vfreq     = pstXC_WinTime_Info->u16InputVFreq;
    stPQModeInfo.u16display_hsize   = pstXC_WinTime_Info->stDispWin.width;
    stPQModeInfo.u16display_vsize   = pstXC_WinTime_Info->stDispWin.height;

#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo(ePQWin, enPQSourceType, &stPQModeInfo);
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo_ex != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo_ex(psXCInstPri->u32DeviceID, ePQWin, enPQSourceType, &stPQModeInfo);
        }
    }
#endif

#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype(ePQWin, enPQSourceType);
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype_ex != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype_ex(psXCInstPri->u32DeviceID, ePQWin, enPQSourceType);
        }
    }
#endif

}

///////////////////////////////////////////////////
/// ADC API
///////////////////////////////////////////////////
void MDrv_XC_ADC_SetMode(void *pInstance, INPUT_SOURCE_TYPE_t enSourceType, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //MS_U16 u16Width;//, u16RealWidth;
    MS_U16 u16PixClk = 0, u16RealPixClk = 0; // Add the initial value
    MS_U16 u16HorizontalTotal;
    XC_IP_SYNC_STATUS sXC_Sync_Status;
    PQ_ADC_SAMPLING_INFO stPQADCSamplingInfo;

    // Give a initial setting.
    memset(&stPQADCSamplingInfo,0,sizeof(PQ_ADC_SAMPLING_INFO));

    if ( IsSrcTypeVga(enSourceType) || IsSrcTypeYPbPr(enSourceType) )
    {
        // get sync status

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
        MDrv_XC_GetSyncStatus(pInstance, enSourceType, &sXC_Sync_Status, eWindow);
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
        MDrv_XC_GetSyncStatus(pInstance, enSourceType, &sXC_Sync_Status, eWindow);
        _XC_RETURN();
#endif //#if SC2_ENABLE



        // get real width
        //u16Width = pSrcInfo->stCapWin.width;
        if(pSrcInfo->bHDuplicate)
        {
#ifdef ENABLE_TV_SC2_PQ
            if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
            {
                if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
                {
                    stPQADCSamplingInfo.enPQSourceType = IsSrcTypeVga(enSourceType) ? PQ_INPUT_SOURCE_VGA : PQ_INPUT_SOURCE_YPBPR;
                    stPQADCSamplingInfo.stPQModeInfo.bFBL               = pSrcInfo->bFBL;
                    stPQADCSamplingInfo.stPQModeInfo.bInterlace         = pSrcInfo->bInterlace;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_hsize     = pSrcInfo->stCapWin.width;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vsize     = pSrcInfo->stCapWin.height;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vfreq     = pSrcInfo->u16InputVFreq;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vtotal    = pSrcInfo->u16InputVTotal;
                    stPQADCSamplingInfo.stPQModeInfo.u16ouput_vfreq     = pSrcInfo->u16InputVFreq;
                    stPQADCSamplingInfo.stPQModeInfo.u16display_hsize   = pSrcInfo->stDispWin.width;
                    stPQADCSamplingInfo.stPQModeInfo.u16display_vsize   = pSrcInfo->stDispWin.height;

                    if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_ADC_SAMPLING, (void *)&stPQADCSamplingInfo, sizeof(PQ_ADC_SAMPLING_INFO)) == FALSE)
                    {
                        stPQADCSamplingInfo.u16ratio = 2;
                    }
                }
                else
                {
                    stPQADCSamplingInfo.u16ratio = 2;
                }
            }
#ifdef ENABLE_TV_SC2_PQ
            else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
            {
                if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex)
                {
                    stPQADCSamplingInfo.enPQSourceType = IsSrcTypeVga(enSourceType) ? PQ_INPUT_SOURCE_VGA : PQ_INPUT_SOURCE_YPBPR;
                    stPQADCSamplingInfo.stPQModeInfo.bFBL               = pSrcInfo->bFBL;
                    stPQADCSamplingInfo.stPQModeInfo.bInterlace         = pSrcInfo->bInterlace;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_hsize     = pSrcInfo->stCapWin.width;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vsize     = pSrcInfo->stCapWin.height;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vfreq     = pSrcInfo->u16InputVFreq;
                    stPQADCSamplingInfo.stPQModeInfo.u16input_vtotal    = pSrcInfo->u16InputVTotal;
                    stPQADCSamplingInfo.stPQModeInfo.u16ouput_vfreq     = pSrcInfo->u16InputVFreq;
                    stPQADCSamplingInfo.stPQModeInfo.u16display_hsize   = pSrcInfo->stDispWin.width;
                    stPQADCSamplingInfo.stPQModeInfo.u16display_vsize   = pSrcInfo->stDispWin.height;

                    if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow, E_PQ_IOCTL_ADC_SAMPLING, (void *)&stPQADCSamplingInfo, sizeof(PQ_ADC_SAMPLING_INFO)) == FALSE)
                    {
                        stPQADCSamplingInfo.u16ratio = 2;
                    }
                }
                else
                {
                    stPQADCSamplingInfo.u16ratio = 2;
                }
            }
#endif

            //u16RealWidth = u16Width / stPQADCSamplingInfo.u16ratio;
        }
        else
        {
            //u16RealWidth = u16Width;
            stPQADCSamplingInfo.u16ratio = 1;
        }

        // Set Hsync polarity
        MDrv_XC_ADC_hpolarity_setting(pInstance, sXC_Sync_Status.u8SyncStatus & XC_MD_HSYNC_POR_BIT);

        // Set user default phase
        MDrv_XC_ADC_SetPhaseEx(pInstance, pSrcInfo->u16DefaultPhase); // setting ADC phase

        //Sog Filter behavior is before the ADC double sampling, so for Sog Filter, we don't need to use
        //double horizontal total for the bandwidth calculation.
        MS_U8 checkcnt = 0;
#if SC2_ENABLE
        _XC_ENTRY(pInstance);
        u16RealPixClk = MDrv_XC_cal_pixclk(pInstance, pSrcInfo->u16DefaultHtotal, eWindow);
        while(u16RealPixClk < ADC_Index_CheckLimit)
        {
            MsOS_DelayTask(ADC_Index_CheckDelay);
            u16RealPixClk = MDrv_XC_cal_pixclk(pInstance, pSrcInfo->u16DefaultHtotal, eWindow);
            ++checkcnt;
            if(checkcnt > ADC_Index_CheckCnt)
                break;
        }
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
        u16RealPixClk = MDrv_XC_cal_pixclk(pInstance, pSrcInfo->u16DefaultHtotal, eWindow);
        _XC_RETURN();
#endif //#if SC2_ENABLE



        MDrv_XC_ADC_Reload(pInstance, enSourceType, u16RealPixClk, pSrcInfo->u16DefaultHtotal, stPQADCSamplingInfo.u16ratio);

        // Get Htt
        if(pSrcInfo->bHDuplicate)
        {
            u16HorizontalTotal = pSrcInfo->u16DefaultHtotal * stPQADCSamplingInfo.u16ratio; // for better quality
        }
        else
        {
            u16HorizontalTotal = pSrcInfo->u16DefaultHtotal;
        }

        // Calculate pixel clock, the unit is MHz

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
        u16PixClk = MDrv_XC_cal_pixclk(pInstance, u16HorizontalTotal, eWindow);
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
        u16PixClk = MDrv_XC_cal_pixclk(pInstance, u16HorizontalTotal, eWindow);
        _XC_RETURN();
#endif //#if SC2_ENABLE



        if (IsSrcTypeVga(enSourceType))
        {
            // ADC reset, Patch PLL unlock problem.
            MDrv_XC_ADC_reset(pInstance, REST_ADC);
        }
        else
        {
            MDrv_XC_ADC_clamp_placement_setting(pInstance, u16PixClk);
        }

        MDrv_XC_ADC_vco_ctrl(pInstance, IsSrcTypeHDMI(enSourceType), u16PixClk);
    }
    else
    {
        MDrv_XC_ADC_Reload(pInstance, enSourceType, u16RealPixClk, pSrcInfo->u16DefaultHtotal, stPQADCSamplingInfo.u16ratio);
    }
}

void MApi_XC_ADC_SwitchSource(void* pInstance, MS_BOOL bUpdateStatus)
{
    MDrv_ADC_SourceSwitch(pInstance,bUpdateStatus);
}

static MS_BOOL _Mdrv_XC_CopySetWinInfo(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    if(pstXC_SetWin_Info == NULL)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].enInputSourceType     = pstXC_SetWin_Info->enInputSourceType;

    gSrcInfo[eWindow].stCapWin.x            = pstXC_SetWin_Info->stCapWin.x;
    gSrcInfo[eWindow].stCapWin.y            = pstXC_SetWin_Info->stCapWin.y;
    gSrcInfo[eWindow].stCapWin.width        = pstXC_SetWin_Info->stCapWin.width;
    gSrcInfo[eWindow].stCapWin.height       = pstXC_SetWin_Info->stCapWin.height;

    gSrcInfo[eWindow].stDispWin.x           = pstXC_SetWin_Info->stDispWin.x;
    gSrcInfo[eWindow].stDispWin.y           = pstXC_SetWin_Info->stDispWin.y;
    gSrcInfo[eWindow].stDispWin.width       = pstXC_SetWin_Info->stDispWin.width;
    gSrcInfo[eWindow].stDispWin.height      = pstXC_SetWin_Info->stDispWin.height;

    gSrcInfo[eWindow].stCropWin.x           = pstXC_SetWin_Info->stCropWin.x;
    gSrcInfo[eWindow].stCropWin.y           = pstXC_SetWin_Info->stCropWin.y;
    gSrcInfo[eWindow].stCropWin.width       = pstXC_SetWin_Info->stCropWin.width;
    gSrcInfo[eWindow].stCropWin.height      = pstXC_SetWin_Info->stCropWin.height;

    gSrcInfo[eWindow].bInterlace            = pstXC_SetWin_Info->bInterlace;
    gSrcInfo[eWindow].bHDuplicate           = pstXC_SetWin_Info->bHDuplicate;
    gSrcInfo[eWindow].u16InputVFreq         = pstXC_SetWin_Info->u16InputVFreq;
    gSrcInfo[eWindow].u16InputVTotal        = pstXC_SetWin_Info->u16InputVTotal;
    gSrcInfo[eWindow].u16DefaultHtotal      = pstXC_SetWin_Info->u16DefaultHtotal;
    if(pstXC_SetWin_Info->u16DefaultPhase != 0)
    {
        gSrcInfo[eWindow].u16DefaultPhase       = pstXC_SetWin_Info->u16DefaultPhase;
    }
    else
    {
        gSrcInfo[eWindow].u16DefaultPhase       = pstXC_SetWin_Info->u8DefaultPhase;//compatible for old AP code
    }
    gSrcInfo[eWindow].bHCusScaling          = pstXC_SetWin_Info->bHCusScaling;
    gSrcInfo[eWindow].u16HCusScalingSrc     = pstXC_SetWin_Info->u16HCusScalingSrc;
    gSrcInfo[eWindow].u16HCusScalingDst     = pstXC_SetWin_Info->u16HCusScalingDst;
    gSrcInfo[eWindow].bVCusScaling          = pstXC_SetWin_Info->bVCusScaling;
    gSrcInfo[eWindow].u16VCusScalingSrc     = pstXC_SetWin_Info->u16VCusScalingSrc;
    gSrcInfo[eWindow].u16VCusScalingDst     = pstXC_SetWin_Info->u16VCusScalingDst;
    gSrcInfo[eWindow].bDisplayNineLattice   = pstXC_SetWin_Info->bDisplayNineLattice;
    gSrcInfo[eWindow].bpqmap                = pstXC_SetWin_Info->bpqmap;
    gSrcInfo[eWindow].btimingchg            = pstXC_SetWin_Info->btimingchg;
    gSrcInfo[eWindow].bxcoutput             = pstXC_SetWin_Info->bxcoutput;
    gSrcInfo[eWindow].bmemfmt               = pstXC_SetWin_Info->bmemfmt;
    gSrcInfo[eWindow].bcropwindowchg        = pstXC_SetWin_Info->bcropwindowchg;
    return TRUE;
}

static void _MDrv_XC_CheckCropWin(XC_SETWIN_INFO *pstXC_SetWin_Info)
{
    if(pstXC_SetWin_Info->stCropWin.width > pstXC_SetWin_Info->stCapWin.width)
    {
        pstXC_SetWin_Info->stCropWin.width = pstXC_SetWin_Info->stCapWin.width;
    }

    if(pstXC_SetWin_Info->stCropWin.height > pstXC_SetWin_Info->stCapWin.height)
    {
        pstXC_SetWin_Info->stCropWin.height = pstXC_SetWin_Info->stCapWin.height;
    }

    if(pstXC_SetWin_Info->stCropWin.x > (pstXC_SetWin_Info->stCapWin.width - pstXC_SetWin_Info->stCropWin.width))
    {
        pstXC_SetWin_Info->stCropWin.x = pstXC_SetWin_Info->stCapWin.width - pstXC_SetWin_Info->stCropWin.width;
    }
    if(pstXC_SetWin_Info->stCropWin.y > (pstXC_SetWin_Info->stCapWin.height - pstXC_SetWin_Info->stCropWin.height))
    {
        pstXC_SetWin_Info->stCropWin.y = pstXC_SetWin_Info->stCapWin.height - pstXC_SetWin_Info->stCropWin.height;
    }
}

MS_BOOL MDrv_XC_Get_VirtualBox_Info(void *pInstance,XC_VBOX_INFO *pstXC_VboxInfo)
{
    MS_U8 u8FrameCnt = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));


    if(pstXC_VboxInfo)
    {
        pstXC_VboxInfo->u32FBSize[0] = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size;
        pstXC_VboxInfo->u32FBSize[1] = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size;
        pstXC_VboxInfo->u16VBox_PanelHsize = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        pstXC_VboxInfo->u16VBox_PanelVsize = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

        // temp for sub
        pstXC_VboxInfo->u16VBox_Htotal[1] = 1920;
        pstXC_VboxInfo->u16VBox_Vtotal[1] = 1080;

        if(XC_SUPPORT_4K2K)
        {
            // in 4K2K case, the Madi mode is P_Mode10 = 20bit/pixels
            u8FrameCnt = (pstXC_VboxInfo->u32FBSize[0]) / (pstXC_VboxInfo->u16VBox_PanelHsize*pstXC_VboxInfo->u16VBox_PanelVsize * 20/8);
            if(u8FrameCnt >= 2)
            {
#if FRC_INSIDE
                /// FRC Enable
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited)
                {
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_4K2K)
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                    }
                    else //if(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)) and others
                    {
                        // 2K2K only for Napoli with FRC_On
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080*2;
                    }
                }
                else  /// FRC Disable
                {
                    if((pstXC_VboxInfo->u16Vfreq[0] <= 300)&&(pstXC_VboxInfo->bInterface[0] == FALSE))
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;//1920; //3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;//1080; //2160;
                    }
                    else
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                    }
                }
#else
                if(!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE) && (pstXC_VboxInfo->bInterface[0] == FALSE) )
                {
					pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                }
                else if(!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE) && (pstXC_VboxInfo->bInterface[0] == TRUE) )// DI 300MHz
                {
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                }
                else // exp : Nike, Monaco_legacy_mode
                {
                    if((pstXC_VboxInfo->u16Vfreq[0] <= 300)&&(pstXC_VboxInfo->bInterface[0] == FALSE))
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                    }
                    else
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                    }
                }
#endif
            }
            else
            {
#if FRC_INSIDE
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited
                        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K))
                {
                    // 2K2K only for Napoli with FRC_On
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080*2;
                }
                else
#endif
                {
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                }
            }
        }
        else
        {
            pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
            pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

E_APIXC_ReturnValue MApi_XC_Get_VirtualBox_Info_U2(void* pInstance, XC_VBOX_INFO *pstXC_VboxInfo)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    bRet = MDrv_XC_Get_VirtualBox_Info(pInstance,pstXC_VboxInfo);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue MApi_XC_Get_VirtualBox_Info(XC_VBOX_INFO *pstXC_VboxInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Get_VBOX_Info XCArgs;
    XCArgs.pstXC_VboxInfo = pstXC_VboxInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VBOX_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the specific window
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen         \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MApi_XC_Check_Multiprocess_SetWindow_Status(void *pInstance)
{
	MS_BOOL bret;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
	// For Multiprocess call set window racing issue (e.g. Gstreamer DS and DTV process 3D on/off case)
#if SC2_ENABLE
		_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
		_XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
		UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
	bret = pXCResourcePrivate->bsetwindow_multiprocess;
		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pXCResourcePrivate->bsetwindow_multiprocess = %u\n", pXCResourcePrivate->bsetwindow_multiprocess);

#if SC2_ENABLE
		_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
		_XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
	XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;

}

static MS_BOOL _MApi_XC_SetWindow(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, MS_BOOL bDualWindow, SCALER_WIN eWindow)
{
#if VERIFY_MVIDEO_FPGA
    MS_BOOL binterlace =FALSE;
#endif
//    MS_BOOL bEnableHighSpeedFclk = FALSE;

	/////////////////////////////////////////////////
	// For Multiprocess call set window racing issue (e.g. Gstreamer DS and DTV process 3D on/off case)
	// If bsetwindow_multiprocess is true, meaning already an other process is executing setwindow function, wait till it is done
	// timeout threadhold: 160ms, checking every 20ms.
	/////////////////////////////////////////////////
	MS_U8 u8timecout = 0;
    while (_MApi_XC_Check_Multiprocess_SetWindow_Status(pInstance))
    {
    	MsOS_DelayTask(20);
		u8timecout ++;
		if (u8timecout > 5) // time out threadhold = 100ms
		{
			printf("[Timeout] Multiprocess calling set window !! \n");
			break;
		}
    }

    MS_U32 u32DisPOddCheck = 0x00;
    MS_BOOL bRet = TRUE;
    //MS_WINDOW_TYPE DispWin;
    MS_WINDOW_TYPE DEWin;
    Hal_SC_get_disp_de_window(pInstance,&DEWin);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //_u16XCDbgSwitch_InternalUseOnly = pXCResourcePrivate->u16XCDbgSwitch_InternalUseOnly;
    pXCResourcePrivate->bsetwindow_multiprocess = TRUE;
    MDrv_SC_SetPQHSDFlag(pInstance, FALSE); //Reset variable
    gSrcInfo[eWindow].b3DMenuload = 1;
    pXCResourcePrivate->btvservice = pstXC_SetWin_Info->btvservice;
    pXCResourcePrivate->u16PreScalingAutoMute = pstXC_SetWin_Info->u16PreScalingAutoMute;
   /* if (pXCResourcePrivate->btvservice && (pXCResourcePrivate->sthal_SC.bDynamicScalingEnable))
    {
       // memcpy(&pstXC_SetWin_Info->stCapWin, &pXCResourcePrivate->stdrvXC_MVideo_Context.stCapWin, sizeof(MS_WINDOW_TYPE));

        if(pXCResourcePrivate->stdrvXC_MVideo_Context.stDispWin.width!=0 && pXCResourcePrivate->stdrvXC_MVideo_Context.stDispWin.height!=0)
            memcpy(&pstXC_SetWin_Info->stDispWin, &pXCResourcePrivate->stdrvXC_MVideo_Context.stDispWin, sizeof(MS_WINDOW_TYPE));

	if(pXCResourcePrivate->stdrvXC_MVideo_Context.stCropWin.width!=0 && pXCResourcePrivate->stdrvXC_MVideo_Context.stCropWin.height!=0)
            memcpy(&pstXC_SetWin_Info->stCropWin, &pXCResourcePrivate->stdrvXC_MVideo_Context.stCropWin, sizeof(MS_WINDOW_TYPE));
    }
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stCapWin, &pstXC_SetWin_Info->stCapWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stDispWin, &pstXC_SetWin_Info->stDispWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stCropWin, &pstXC_SetWin_Info->stCropWin, sizeof(MS_WINDOW_TYPE));*/
#ifdef K3_U2
    // Prepare seamless zapping, reset commands in next DS index
    if((Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow)) && (Hal_SC_get_freezeimg(pInstance, eWindow)))
    {
        Hal_SC_PrepareSeamlessZapping(pInstance, eWindow);
    }
#endif
    //OPWrite(UC feature) should be disabled before setwindow. and then disable opw limit protect.
    //otherwise the opw limit protect will block op1 cmd, the sub window will also be blocked.
    //when setwindow is finished, the opw limit protect should be enabled before OPWrite(UC feature) is enabled.
    if(eWindow == MAIN_WINDOW)
    {
        if(MDrv_XC_GetSkipOPWriteOffInSetWindow(pInstance, eWindow) == FALSE)
        {
            Hal_SC_SetOPWriteOff(pInstance, ENABLE);//Disable OPWrite first, Enable OPWrite when FPLL done
        }
#if 0 // RD suggest move to init
        MS_U32 u32OPWLimitBase = 0x00;
        u32OPWLimitBase = ((MDrv_XC_GetIPMBase(pInstance, 0, eWindow)*BYTE_PER_WORD + MDrv_XC_GetDNRBufSize(pInstance, eWindow)) / BYTE_PER_WORD - 1) ;
        MDrv_SC_set_opm_write_limit(pInstance, FALSE, 0 , u32OPWLimitBase , eWindow);
#endif
    }
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_41_L, 0, 0xFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_41_L, 0, 0xFF00);
        if ((pXCResourcePrivate->stdrvXC_3D.ePreInputMode == MDrv_XC_Get_3D_Input_Mode(pInstance,eWindow)) &&
                (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode == MDrv_XC_Get_3D_Output_Mode(pInstance)) &&
                (pXCResourcePrivate->stdrvXC_3D.u16PreDiswidth == gSrcInfo[eWindow].stDispWin.width) &&
                (pXCResourcePrivate->stdrvXC_3D.u16PreDisheight == gSrcInfo[eWindow].stDispWin.height) &&
                (pXCResourcePrivate->stdrvXC_3D.u16PreCapwidth == gSrcInfo[eWindow].stCapWin.width) &&
                (pXCResourcePrivate->stdrvXC_3D.u16PreCapheight == gSrcInfo[eWindow].stCapWin.height) &&
                (gSrcInfo[eWindow].bBlackscreenEnabled == 0) &&
                (MDrv_XC_Get_3D_Input_Mode(pInstance,eWindow) != E_XC_3D_INPUT_MODE_NONE) &&
                ((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM)                          ||
                 (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)                   ||
                 (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) ||
                 (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)))
        {
            if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
            {
                ;
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," 3D input/output format is same, not need to set _MApi_XC_SetWindow\n");
                gSrcInfo[eWindow].b3DMenuload = 0;
            }
        }
    }
    // we've to support interface changed without re-compile user application
    // please specify new added interface default value here
    //_u16XCDbgSwitch_InternalUseOnly = (XC_DBGLEVEL_SETWINDOW|XC_DGBLEVEL_CROPCALC|XC_DBGLEVEL_SETTIMING);
    if(u32InitDataLen != sizeof(XC_SETWIN_INFO))
    {
        // customer application different version with our driver
        // start from customized pre H/V scaling
        gSrcInfo[eWindow].Status2.bPreHCusScaling = FALSE;
        gSrcInfo[eWindow].Status2.bPreVCusScaling = FALSE;
        gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = 0;
        gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = 0;
        gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = 0;
        gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = 0;
    }
    else
    {
        gSrcInfo[eWindow].Status2.bPreHCusScaling = pstXC_SetWin_Info->bPreHCusScaling;
        gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = pstXC_SetWin_Info->u16PreHCusScalingSrc;
        gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = pstXC_SetWin_Info->u16PreHCusScalingDst;

        if(gSrcInfo[eWindow].Status2.u16PreHCusScalingDst > gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"H can't do pre scaling up, change to auto prescaling\n");
            gSrcInfo[eWindow].Status2.bPreHCusScaling = FALSE;
            gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = 0;
            gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = 0;
        }

        if (gSrcInfo[eWindow].Status2.bPreHCusScaling)
        {
            if (gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc == 0 || gSrcInfo[eWindow].Status2.u16PreHCusScalingDst == 0)
            {
                gSrcInfo[eWindow].Status2.bPreHCusScaling = FALSE;
                gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = 0;
                gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = 0;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"PreHCus Src or Dst can not be 0 \n");

#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

                return FALSE;
            }
        }
        else
        {
            gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = 0;
            gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = 0;
        }

        gSrcInfo[eWindow].Status2.bPreVCusScaling = pstXC_SetWin_Info->bPreVCusScaling;
        gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = pstXC_SetWin_Info->u16PreVCusScalingSrc;
        gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = pstXC_SetWin_Info->u16PreVCusScalingDst;

        if(gSrcInfo[eWindow].Status2.u16PreVCusScalingDst > gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"V can't do pre scaling up, change to auto prescaling\n");
            gSrcInfo[eWindow].Status2.bPreVCusScaling = FALSE;
            gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = 0;
            gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = 0;
        }

        if (gSrcInfo[eWindow].Status2.bPreVCusScaling)
        {
            if (gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc == 0 || gSrcInfo[eWindow].Status2.u16PreVCusScalingDst == 0  )
            {
                gSrcInfo[eWindow].Status2.bPreVCusScaling = FALSE;
                gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = 0;
                gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = 0;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"PreVCus Src or Dst can not be 0 \n");

#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

                return FALSE;
            }
        }
        else
        {
            gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = 0;
            gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = 0;
        }
    }


    if(pstXC_SetWin_Info->bInterlace)
    {
        //The component 1080i can't frame lock when capture start is odd.
        {
            // the Capture start should be even
            if(pstXC_SetWin_Info->stCapWin.y%2)
            {
                pstXC_SetWin_Info->stCapWin.y += 1;
                #if(HW_DESIGN_3D_VER == 5)
                if (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_TOP_BOTTOM)
                {
                    ; // can not descrease for monaco
                }
                else
                #endif
                pstXC_SetWin_Info->stCapWin.height -= 1; //Y+1, so height-1 to keep not capture outside
            }
        }

        // the Capture height should be even
        if(pstXC_SetWin_Info->stCapWin.height%2)
        {
            #if(HW_DESIGN_3D_VER == 5)
            if (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_TOP_BOTTOM)
            {
                ; // can not descrease for monaco
            }
            else
            #endif
            pstXC_SetWin_Info->stCapWin.height -= 1;
        }
        // the Capture height should be 4
        #if(HW_DESIGN_3D_VER == 5)
        if((pstXC_SetWin_Info->stCapWin.height%4) && (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_TOP_BOTTOM))
        {
            pstXC_SetWin_Info->stCapWin.height = (pstXC_SetWin_Info->stCapWin.height/4) *4 ;
        }
        #endif
    }

    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID && XC_DEVICE1_IS_INTERLACE_OUT)
    {
        pstXC_SetWin_Info->stDispWin.y /= 2;
        pstXC_SetWin_Info->stDispWin.height /= 2;
    }

    // TGEN does not allow display window height <= 5 or it cannot decide VDE_End
    if (pstXC_SetWin_Info->stDispWin.height <= 5)
    {
        pstXC_SetWin_Info->stDispWin.height = 5;
    }

    _MDrv_XC_CheckCropWin(pstXC_SetWin_Info);

    if (((pstXC_SetWin_Info->stDispWin.x
            + pstXC_SetWin_Info->stDispWin.width
            + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart)
            > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal)
             &&!((pXCResourcePrivate->sthal_SC.bDynamicScalingEnable) ))
    {
        pstXC_SetWin_Info->stDispWin.width = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal
                                             - pstXC_SetWin_Info->stDispWin.x
                                             - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
        printf("Display Window size is exceeded Htotal, display window horizontal end is limited to Htotal\n");
    }

    if (((pstXC_SetWin_Info->stDispWin.y
            + pstXC_SetWin_Info->stDispWin.height
            + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)
            > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal)
            &&!((pXCResourcePrivate->sthal_SC.bDynamicScalingEnable) ))
    {
        pstXC_SetWin_Info->stDispWin.height = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal
                                              - pstXC_SetWin_Info->stDispWin.y
                                              - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
        printf("Display Window size is exceeded Vtotal, display window vertical end is limited to Vtotal\n");
    }
//patch for monaco 3D SC HW limitation, if input is SBS, 2D_HW, PA, CB, capture window's width must be align by 4
#if(HW_DESIGN_3D_VER == 5) //monaco mode
    if((E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW))
            || (E_XC_3D_INPUT_PIXEL_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW))
            || (E_XC_3D_INPUT_CHECK_BORAD == MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW)))
    {
        pstXC_SetWin_Info->stCapWin.width &= ~0x03;
    }
#endif
    // copy user data to internal data
    // here is a patch first, will remove after seperate Internal XC status with set window information
    //u32CopyLen = sizeof(XC_SETWIN_INFO) - (sizeof(MS_BOOL)*2) - (sizeof(MS_U16)*4);
    //memcpy(&gSrcInfo[eWindow], pstXC_SetWin_Info, u32CopyLen);
    //do not use memcopy(), strongly depend on structure definition
    _Mdrv_XC_CopySetWinInfo(pInstance, pstXC_SetWin_Info, eWindow);

#ifndef DISABLE_3D_FUNCTION
    bRet = MDrv_SC_3D_FormatValidation(pInstance, &gSrcInfo[eWindow], eWindow);
    //for 3D handle invalid display window
    if(E_XC_3D_OUTPUT_CHECKBOARD_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
    {
        gSrcInfo[eWindow].stDispWin.y &= ~0x1;
        gSrcInfo[eWindow].stDispWin.x &= ~0x1;
        gSrcInfo[eWindow].stDispWin.height &= ~0x1;
        gSrcInfo[eWindow].stDispWin.width &= ~0x1;
    }
    else if((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MDrv_XC_Get_3D_Output_Mode(pInstance))
            || (E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
            || ((E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)) && MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)))
    {
        gSrcInfo[eWindow].stDispWin.y &= ~0x1;
        gSrcInfo[eWindow].stDispWin.height &= ~0x1;
    }
    else if(E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
    {
        gSrcInfo[eWindow].stDispWin.x &= ~0x1;
        gSrcInfo[eWindow].stDispWin.width &= ~0x1;
    }
    else if((E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance))
            || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MDrv_XC_Get_3D_Output_Mode(pInstance)))
    {
        gSrcInfo[eWindow].stDispWin.width &= ~0x1;
    }
    else if((E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance))
            || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MDrv_XC_Get_3D_Output_Mode(pInstance)))
    {
        gSrcInfo[eWindow].stDispWin.height &= ~0x1;
    }
#endif

    // assert check
    MS_ASSERT((gSrcInfo[eWindow].bHCusScaling==TRUE)?((gSrcInfo[eWindow].u16HCusScalingSrc!=0)&&(gSrcInfo[eWindow].u16HCusScalingDst!=0)):1);
    MS_ASSERT((gSrcInfo[eWindow].bVCusScaling==TRUE)?((gSrcInfo[eWindow].u16VCusScalingSrc!=0)&&(gSrcInfo[eWindow].u16VCusScalingDst!=0)):1);
    MS_ASSERT((gSrcInfo[eWindow].Status2.bPreHCusScaling==TRUE)?((gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc!=0)&&(gSrcInfo[eWindow].Status2.u16PreHCusScalingDst!=0)):1);
    MS_ASSERT((gSrcInfo[eWindow].Status2.bPreVCusScaling==TRUE)?((gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc!=0)&&(gSrcInfo[eWindow].Status2.u16PreVCusScalingDst!=0)):1);

    // The total pixel conut of display window can not be odd.
    u32DisPOddCheck = gSrcInfo[eWindow].stDispWin.width * gSrcInfo[eWindow].stDispWin.height;
    // if u32DisPOddCheck is odd, that means both of disp.width and disp.height is odd.
    // we adjust width to even.
    if (u32DisPOddCheck & 0x01)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > gSrcInfo[eWindow].stDispWin.width )
        {
            gSrcInfo[eWindow].stDispWin.width = ( gSrcInfo[eWindow].stDispWin.width + 1 ) & ~0x1;
        }
    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        gSrcInfo[eWindow].stDispWin.height = gSrcInfo[eWindow].stDispWin.height/4;
    }
    else if(MDrv_XC_Is2K2KToFrc(pInstance))
    {
        gSrcInfo[eWindow].stDispWin.x = gSrcInfo[eWindow].stDispWin.x/2;
        gSrcInfo[eWindow].stDispWin.width = gSrcInfo[eWindow].stDispWin.width/2;
    }
    // if mirror, need to re-cal display window but Ninelattice!!
    if(IsVMirrorMode(eWindow) &&(!gSrcInfo[eWindow].bDisplayNineLattice) && (!((eWindow == SUB_WINDOW)&&(pXCResourcePrivate->bDualWinForAPVR == TRUE))))
    {
        MS_U32 u32TempWidth = gSrcInfo[eWindow].stDispWin.width;
        MS_U32 u32TempHeight = gSrcInfo[eWindow].stDispWin.height;
#ifndef DISABLE_3D_FUNCTION

        if(((MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
                || (MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW))
                && (MDrv_XC_Get_3D_HW_Version() >= 1))
        {
            gSrcInfo[eWindow].stDispWin.x = gSrcInfo[eWindow].stDispWin.x * 2;
        }
        else if(((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM)
                 || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM_HW))
                && ((MDrv_XC_Get_3D_HW_Version() > 1)
                    || ((MDrv_XC_Get_3D_HW_Version() == 1) && (!MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)))))
        {
            gSrcInfo[eWindow].stDispWin.y = gSrcInfo[eWindow].stDispWin.y * 2;
        }

        //for ver3d =1, display window will be half when pass into driver,so here need double it
        if(((MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
                || (MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW))
                && (MDrv_XC_Get_3D_HW_Version() == 1))
        {
            u32TempWidth *= 2;
        }
#endif

        if(MDrv_XC_Is2K2KToFrc(pInstance))
        {
            if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2 > ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth ) )
            {
                gSrcInfo[eWindow].stDispWin.x =
                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2 - ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth );
            }
            else
            {
                gSrcInfo[eWindow].stDispWin.x = 0;
            }
        }
        else
        {
            if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth ) )
            {
                gSrcInfo[eWindow].stDispWin.x =
                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth );
            }
            else
            {
                gSrcInfo[eWindow].stDispWin.x = 0;
            }
        }
#ifndef DISABLE_3D_FUNCTION

        //for ver3d =1, display window will be half when pass into driver,so here need double it
        if(((MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_TOP_BOTTOM)
                || (MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_TOP_BOTTOM_HW))
                && (MDrv_XC_Get_3D_HW_Version() == 1))
        {
            u32TempHeight *= 2;
        }
#endif
        if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > ( gSrcInfo[eWindow].stDispWin.y + u32TempHeight ) )
        {
            gSrcInfo[eWindow].stDispWin.y =
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - ( gSrcInfo[eWindow].stDispWin.y + u32TempHeight );
        }
        else
        {
            gSrcInfo[eWindow].stDispWin.y = 0;
        }
#ifndef DISABLE_3D_FUNCTION
        if(((MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
                || (MDrv_XC_Get_3D_Output_Mode(pInstance)== E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW))
                &&(MDrv_XC_Get_3D_HW_Version() >= 1))
        {
            gSrcInfo[eWindow].stDispWin.x /= 2;

            //adjust main/sub display window position for ver1
            //for ver>=2, this logic is at MDrv_XC_3D_LoadReg
            if(MDrv_XC_Get_3D_HW_Version() == 1)
            {
                if(eWindow == MAIN_WINDOW)
                {
                    gSrcInfo[eWindow].stDispWin.x += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2;
                }
                else
                {
                    if(gSrcInfo[eWindow].stDispWin.x >= pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2)
                    {
                        gSrcInfo[eWindow].stDispWin.x -= pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2;
                    }
                }
            }
        }
        else if(((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM)
                 || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM_HW))
                && ((MDrv_XC_Get_3D_HW_Version() > 1)
                    || ((MDrv_XC_Get_3D_HW_Version() == 1) && (!MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)))))
        {
            gSrcInfo[eWindow].stDispWin.y /= 2;

            //adjust main/sub display window position
            //for ver>=2, this logic is at MDrv_XC_3D_LoadReg
            if(MDrv_XC_Get_3D_HW_Version() == 1)
            {
                if(eWindow == MAIN_WINDOW)
                {
                    gSrcInfo[eWindow].stDispWin.y += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height / 2;
                }
                else
                {
                    if(gSrcInfo[eWindow].stDispWin.y >= pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height / 2)
                    {
                        gSrcInfo[eWindow].stDispWin.y -= pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height / 2;
                    }
                }
            }
        }
#endif
    }
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_AdjustHShift(pInstance, &gSrcInfo[eWindow], eWindow);
#endif

#if ENABLE_SW_DS

    XC_PRINTF("[%s,%5d] MDrv_XC_GetDynamicScalingStatus():%d MDrv_XC_Is_DSForceIndexEnabled():%d  \n",__func__,__LINE__,MDrv_XC_GetDynamicScalingStatus(pInstance),MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow));

    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
    {
        gSrcInfo[eWindow].stCapWin.width = pstXC_SetWin_Info->stCapWin.width;
        gSrcInfo[eWindow].stCapWin.height = pstXC_SetWin_Info->stCapWin.height;
        gSrcInfo[eWindow].stCropWin.width = pstXC_SetWin_Info->stCropWin.width;
        gSrcInfo[eWindow].stCropWin.height = pstXC_SetWin_Info->stCropWin.height;
    }
#endif

    gSrcInfo[eWindow].stDispWin.x += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
    gSrcInfo[eWindow].stDispWin.y += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;

    // dump debug msg
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= SetWindow Start (Window : %u, src: %u) =============\n",eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CropWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"DispWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stDispWin.x,gSrcInfo[eWindow].stDispWin.y, gSrcInfo[eWindow].stDispWin.width, gSrcInfo[eWindow].stDispWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Panel   x: %4u  y: %4u  w: %4u  h: %4u \n",
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Mirror/Interlace/Hdup = %u/%u/%u\n", IsVMirrorMode(eWindow), pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"H/V total = (%u, %u), VFreq = %u\n", pstXC_SetWin_Info->u16DefaultHtotal, pstXC_SetWin_Info->u16InputVTotal, pstXC_SetWin_Info->u16InputVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Pre  H cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].Status2.bPreHCusScaling, gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreHCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Pre  V cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].Status2.bPreVCusScaling, gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreVCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Post H cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].bHCusScaling, gSrcInfo[eWindow].u16HCusScalingSrc, gSrcInfo[eWindow].u16HCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Post V cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].bVCusScaling, gSrcInfo[eWindow].u16VCusScalingSrc, gSrcInfo[eWindow].u16VCusScalingDst);

    XC_PRINTF("[%s,%5d] ============= SetWindow Start (Window : %u, src: %u) =============  \n",__func__,__LINE__,eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_PRINTF("[%s,%5d] CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",__func__,__LINE__,gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_PRINTF("[%s,%5d] CropWin  x: %4u  y: %4u  w: %4u  h: %4u  \n",__func__,__LINE__,gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);
    XC_PRINTF("[%s,%5d] DispWin  x: %4u  y: %4u  w: %4u  h: %4u  \n",__func__,__LINE__,gSrcInfo[eWindow].stDispWin.x,gSrcInfo[eWindow].stDispWin.y, gSrcInfo[eWindow].stDispWin.width, gSrcInfo[eWindow].stDispWin.height);
    XC_PRINTF("[%s,%5d] Panel  x: %4u  y: %4u  w: %4u  h: %4u  \n",__func__,__LINE__,pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart,
              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart,
              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width,
              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);
    XC_PRINTF("[%s,%5d] Mirror/Interlace/Hdup = %u/%u/%u  \n",__func__,__LINE__,IsVMirrorMode(eWindow), pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_PRINTF("[%s,%5d] H/V total = (%u, %u), VFreq = %u  \n",__func__,__LINE__,pstXC_SetWin_Info->u16DefaultHtotal, pstXC_SetWin_Info->u16InputVTotal, pstXC_SetWin_Info->u16InputVFreq);
    XC_PRINTF("[%s,%5d] Mirror/Interlace/Hdup = %u/%u/%u  \n",__func__,__LINE__,IsVMirrorMode(eWindow), pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_PRINTF("[%s,%5d] Pre  H cus scaling %u (%u -> %u)  \n",__func__,__LINE__,gSrcInfo[eWindow].Status2.bPreHCusScaling, gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreHCusScalingDst);
    XC_PRINTF("[%s,%5d] Pre  V cus scaling %u (%u -> %u)  \n",__func__,__LINE__,gSrcInfo[eWindow].Status2.bPreVCusScaling, gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreVCusScalingDst);
    XC_PRINTF("[%s,%5d] Post  H cus scaling %u (%u -> %u)  \n",__func__,__LINE__,gSrcInfo[eWindow].bHCusScaling, gSrcInfo[eWindow].u16HCusScalingSrc, gSrcInfo[eWindow].u16HCusScalingDst);
    XC_PRINTF("[%s,%5d] Post  V cus scaling %u (%u -> %u)  \n",__func__,__LINE__,gSrcInfo[eWindow].bVCusScaling, gSrcInfo[eWindow].u16VCusScalingSrc, gSrcInfo[eWindow].u16VCusScalingDst);

#if VERIFY_MVIDEO_FPGA  //non-de-interlace function
    if(gSrcInfo[eWindow].bInterlace)
    {
        gSrcInfo[eWindow].bInterlace = 0;
        binterlace =TRUE;//gSrcInfo[eWindow].u16V_DisSize /= 2;
    }
#endif


    gSrcInfo[eWindow].Status2.u16VirtualBox_Width = 0;
    gSrcInfo[eWindow].Status2.u16VirtualBox_Height = 0;

    XC_VBOX_INFO pstXCVBox;
    memset(&pstXCVBox, 0, sizeof(XC_VBOX_INFO));
    pstXCVBox.bInterface[eWindow] = gSrcInfo[eWindow].bInterlace;
    pstXCVBox.u16Vfreq[eWindow] = pstXC_SetWin_Info->u16InputVFreq;

    gSrcInfo[eWindow].u16InputVFreq = pstXC_SetWin_Info->u16InputVFreq;

    if (MDrv_XC_Get_VirtualBox_Info(pInstance,&pstXCVBox) == TRUE)
    {
        gSrcInfo[eWindow].Status2.u16VirtualBox_Width = pstXCVBox.u16VBox_Htotal[eWindow];
        gSrcInfo[eWindow].Status2.u16VirtualBox_Height = pstXCVBox.u16VBox_Vtotal[eWindow];

        XC_PRINTF("[%s,%5d] [DS] u16VBox_Htotal:%d u16VBox_Vtotal:%d  \n",__func__,__LINE__,gSrcInfo[eWindow].Status2.u16VirtualBox_Width,gSrcInfo[eWindow].Status2.u16VirtualBox_Height);
    }
    else
    {
        XC_PRINTF("[%s,%5d] [DS] MDrv_XC_Get_VirtualBox_Info ERR!! u16VBox_Htotal:%d u16VBox_Vtotal:%d  \n",__func__,__LINE__,gSrcInfo[eWindow].Status2.u16VirtualBox_Width,gSrcInfo[eWindow].Status2.u16VirtualBox_Height);
    }

#if ENABLE_SW_DS
    XC_InternalStatus SrcInfo;
    memcpy(&SrcInfo,&gSrcInfo[eWindow],sizeof(XC_InternalStatus));

    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
    {
        SrcInfo.stCapWin.width     = SrcInfo.Status2.u16VirtualBox_Width;
        SrcInfo.stCapWin.height    = SrcInfo.Status2.u16VirtualBox_Height;
    }
    MApi_XC_Set_PQ_SourceData(pInstance, eWindow, gSrcInfo[eWindow].enInputSourceType, &SrcInfo);
#else
    // Scaler must update source before width / height checking.
    MApi_XC_Set_PQ_SourceData(pInstance, eWindow, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow]);
#endif

    //if HW 3d ver > 1 in 3d case, sub PQ source type don't set, because there is no setwin(sub). So load first row in PQ table in quality_map_sub.
    //it need set sub PQ source type the same with Main's source type.
#ifndef DISABLE_3D_FUNCTION
    if((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_MODE_NONE)
            && (MDrv_XC_Get_3D_HW_Version() > 1)
            && (eWindow == MAIN_WINDOW))
    {
        MApi_XC_Set_PQ_SourceData(pInstance, SUB_WINDOW, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow]);
    }
#endif
    // Capture width & height can not be 0 !!
    if (gSrcInfo[eWindow].stCapWin.width == 0 || gSrcInfo[eWindow].stCapWin.height == 0 ||
            gSrcInfo[eWindow].stCropWin.width == 0 || gSrcInfo[eWindow].stCropWin.height == 0 ||
            gSrcInfo[eWindow].stDispWin.width == 0 || gSrcInfo[eWindow].stDispWin.height == 0 )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Cap/Crop/Disp width or height can not be 0 \n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return FALSE;
    }

    if(eWindow == SUB_WINDOW)
    {
        //For PIP, enable IP Read/write

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
        MDrv_XC_SetScalerMemoryRequest(pInstance, TRUE, E_XC_MEMORY_IP_READ_REQUEST, SUB_WINDOW);
        MDrv_XC_SetScalerMemoryRequest(pInstance, TRUE, E_XC_MEMORY_IP_WRITE_REQUEST, SUB_WINDOW);
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
        MDrv_XC_SetScalerMemoryRequest(pInstance, TRUE, E_XC_MEMORY_IP_READ_REQUEST, SUB_WINDOW);
        MDrv_XC_SetScalerMemoryRequest(pInstance, TRUE, E_XC_MEMORY_IP_WRITE_REQUEST, SUB_WINDOW);
        _XC_RETURN();
#endif //#if SC2_ENABLE


    }


    if (IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType)   ||
            IsSrcTypeYPbPr(gSrcInfo[eWindow].enInputSourceType) ||
            IsSrcTypeScart(gSrcInfo[eWindow].enInputSourceType) )
    {
        if(!((E_XC_3D_INPUT_MODE_NONE != MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                && (SUB_WINDOW == eWindow)))// don't set adc mode when set sub window at 3d
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"ADC Set Mode \n");
                //LM15U: SC0 SCART IN --> Turn on APVR SC2 (Scart-in), the ADC table will reload.
                //Noise happens in SC0.
                if(psXCInstPri->u32DeviceID == 0)
                {
         #if CHKSTABLE_SIGNAL_BF_ADC     

			MS_U8 checkcnt = 0;
			if(MDrv_XC_PCMonitor_Get_HFreq(pInstance, DISABLE,eWindow)!=0)
			{
				while((abs(MDrv_SC_ip_get_horizontalPeriod(pInstance, eWindow )-MDrv_XC_PCMonitor_Get_HFreq(pInstance, DISABLE,eWindow)) > MD_HPERIOD_TORLANCE ))
				{
					MsOS_DelayTask(CHKINTVAL);
					++checkcnt;
					
					if(checkcnt > MAXTIME)
					break;
				}
			}
	#endif
				
                    MDrv_XC_ADC_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);
                }
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    msAPI_Scaler_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

    msAPI_Scaler_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);

    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#if ENABLE_REQUEST_FBL
    if ( (MDrv_XC_IsRequestFrameBufferLessMode(pInstance) && MDrv_XC_IsCurrentRequest_FrameBufferLessMode(pInstance))
            && (!MDrv_XC_IsCurrentFrameBufferLessMode(pInstance)) )
    {
        printf("====No Crop for RFBL==\n");

        gSrcInfo[eWindow].stCropWin.x = 0;
        gSrcInfo[eWindow].stCropWin.y = 0;
        gSrcInfo[eWindow].stCropWin.width = gSrcInfo[eWindow].stCapWin.width;
        gSrcInfo[eWindow].stCropWin.height= gSrcInfo[eWindow].stCapWin.height;
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"UCNR enabled: %s \n", (MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow) == TRUE) ? "NO" : "YES");

#ifdef ENABLE_SCALING_WO_MUTE
    if(DISABLE == gSrcInfo[eWindow].bBlackscreenEnabled)
    {
        MDrv_SC_Adjust_Prescaling_Ratio(pInstance, eWindow);
    }
#endif

#if ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
    {
       // gSrcInfo[eWindow].stCapWin.x = gSrcInfo[eWindow].stCropWin.x;
		//gSrcInfo[eWindow].stCapWin.y = gSrcInfo[eWindow].stCropWin.y;
        	//gSrcInfo[eWindow].stCapWin.width = gSrcInfo[eWindow].stCropWin.width;
        //gSrcInfo[eWindow].stCapWin.height = gSrcInfo[eWindow].stCropWin.height;
    }
#endif

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    MDrv_SC_set_prescaling_ratio(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow );
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

    MDrv_SC_set_prescaling_ratio(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow );
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CropWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);

    MDrv_SC_set_crop_window(pInstance, &gSrcInfo[eWindow], eWindow );

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CropWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"DispWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stDispWin.x,gSrcInfo[eWindow].stDispWin.y, gSrcInfo[eWindow].stDispWin.width, gSrcInfo[eWindow].stDispWin.height);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    MDrv_SC_set_postscaling_ratio(pInstance, &gSrcInfo[eWindow], eWindow );

#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling(
                eWindow == SUB_WINDOW ?
                PQ_SUB_WINDOW :
                PQ_MAIN_WINDOW,
                gSrcInfo[eWindow].bFBL,
                gSrcInfo[eWindow].bPreV_ScalingDown,
                gSrcInfo[eWindow].ScaledCropWin.y);
    }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling_ex != NULL)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling_ex(psXCInstPri->u32DeviceID,
                    eWindow == SUB_WINDOW ?
                    PQ_SUB_WINDOW :
                    PQ_MAIN_WINDOW,
                    gSrcInfo[eWindow].bFBL,
                    gSrcInfo[eWindow].bPreV_ScalingDown,
                    gSrcInfo[eWindow].ScaledCropWin.y);
        }
        }
#endif
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    // FIXME: direct use variable instead of stDBreg?
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    // FIXME: direct use variable instead of stDBreg?
    _XC_ENTRY();
#endif //#if SC2_ENABLE



#ifndef DISABLE_3D_FUNCTION
    //for 3D handle invalid crop window,or else LR status may be inverse.
    if(E_XC_3D_INPUT_CHECK_BORAD == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {
        gSrcInfo[eWindow].ScaledCropWin.y &= ~0x1;
        gSrcInfo[eWindow].ScaledCropWin.x &= ~0x1;
        gSrcInfo[eWindow].ScaledCropWin.height &= ~0x1;
        gSrcInfo[eWindow].ScaledCropWin.width &= ~0x1;
    }
    else if(E_XC_3D_INPUT_LINE_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {
        gSrcInfo[eWindow].ScaledCropWin.y &= ~0x1;
        gSrcInfo[eWindow].ScaledCropWin.height &= ~0x1;
    }
    else if(E_XC_3D_INPUT_PIXEL_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {
        gSrcInfo[eWindow].ScaledCropWin.x &= ~0x1;
        gSrcInfo[eWindow].ScaledCropWin.width &= ~0x1;
    }
#endif
#if (HW_DESIGN_4K2K_VER == 4)

#if 0 // This is no meaning patch, must removed.
    // 4K2K cases
    // SW Patch, default use 3 Frame buffer for SOC and FRC
    // Please reference MHal_XC_Init_Patch about SC_BK32_19 and SC_BK32_59
    if((gSrcInfo[eWindow].stCapWin.width >= 3840  &&  gSrcInfo[eWindow].stCapWin.height == 2160) && (!MDrv_XC_GetDynamicScalingStatus(pInstance)) )
    {
        gSrcInfo[eWindow].Status2.eFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
		 if(!HAL_SC_Get_IsGstreamerProcess())
		 {
	        MApi_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_19_L,0x03, 0xf);
	        MApi_XC_MLoad_WriteCmd(pInstance,REG_SC_BK32_19_L,0x03, 0xf);
	        MApi_XC_MLoad_Fire(pInstance,TRUE);
		 }
		 else
		 {
	        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L,0x03, 0xf);
	        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_19_L,0x03, 0xf);
		 }
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"4K2K 4096x2160 case, use default 3 frame count ,  eFRCMFrameStoreNumber[%d] = %d \n",eWindow,gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber);
    }
#endif
    // temp solution for monaco
    // MDrv_SC_set_fetch_number_limit will change by MDrv_SC_set_frcm_fetch_number_limit when it stable
    // but function name always is MDrv_SC_set_fetch_number_limit
    MDrv_SC_set_frcm_fetch_number_limit(pInstance, &gSrcInfo[eWindow], eWindow );
#else
    MDrv_SC_set_fetch_number_limit(pInstance, &gSrcInfo[eWindow], eWindow );
#endif
    MDrv_sc_set_capture_window(pInstance, eWindow );

#if VERIFY_MVIDEO_FPGA //non-de-interlace function=> celear second frame half garbage
    if(binterlace)
    {
        gSrcInfo[eWindow].stDispWin.height/= 2;
        gSrcInfo[eWindow].u16InputVTotal /= 2;
    }
#endif
    // FIXME: direct use variable instead of stDBreg?
    MDrv_SC_set_display_window(pInstance,  eWindow );

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Physical DispWin HStart: %4u  HEnd: %4u  Vstart: %4u  VEnd: %4u \n",
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd);
    if ((eWindow == MAIN_WINDOW) && (gSrcInfo[eWindow].b3DMenuload == 0))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," 3D input/output format is same, not need to set _MApi_XC_SetWindow\n");


#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else
        _XC_RETURN();
#endif //#if SC2_ENABLE

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
        return TRUE;
    }

    // Adjust FCLK by different case
    if (gSrcInfo[eWindow].bFBL)
    {
        MDrv_SC_set_Fclk(pInstance, EN_FCLK_FBL);
    }
    else
    {
        MDrv_SC_set_Fclk(pInstance, EN_FCLK_NORMAL);
    }

    // Adjust display line buffer start mode by different case
    if (gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL)
    {
        MDrv_SC_SetDisplay_Start_Mode(pInstance, DISABLE);
    }
    else
    {
        MDrv_SC_SetDisplay_Start_Mode(pInstance, ENABLE);
    }

// Set 3D status
    pXCResourcePrivate->stdrvXC_3D.b3Dstate =   (!((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_MODE_NONE)))
                                                 && (!((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_MODE_NONE)))
                                                 && ((!(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_NORMAL_2D_HW )) || ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)) || ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)));

    if (bDualWindow == TRUE)
    {
        if (eWindow == MAIN_WINDOW)
        {
            memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stMainDBreg, &pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, sizeof(SC_SWDB_INFO));
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualMainWinDone = TRUE;
        }
        else
        {
            memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stSubDBreg
                   , &pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, sizeof(SC_SWDB_INFO));
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualSubWinDone = TRUE;
        }

        if (pXCResourcePrivate->stdrvXC_MVideo._bSetDualMainWinDone
                && pXCResourcePrivate->stdrvXC_MVideo._bSetDualSubWinDone)
        {
            MDrv_SC_dual_sw_db(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.stMainDBreg
                               , &pXCResourcePrivate->stdrvXC_MVideo_Context.stSubDBreg);
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualMainWinDone = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualSubWinDone = FALSE;
        }
    }
    else
    {
        MDrv_SC_sw_db(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, eWindow );
    }

    if((2200 <=gSrcInfo[MAIN_WINDOW].stCropWin.width) && (MAIN_WINDOW == eWindow))
    {
        //For Nike 4k2k
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, 0, 0xFFFF);
    }

    //For VIP peaking setting
    MDrv_XC_VIP_Peaking_Setting(pInstance, eWindow);

#if SUPPORT_SEAMLESS_ZAPPING
    if(INPUT_SOURCE_DTV == gSrcInfo[eWindow].enInputSourceType)
    {
        //When HDMI 4K2K open compress mode change to PX2 mode need to toggle the compress flag clear
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_40_L, BIT(0), BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_40_L, 0, BIT(0));
    }
#endif


#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if(bRet == FALSE)
    {
#ifndef DISABLE_3D_FUNCTION

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[eWindow], eWindow);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[eWindow], eWindow);
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
    }

#if FRC_INSIDE
    // SW patch for MM 4K BW issue
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal > 3000)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch > 1920)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen > 1080))
    {
        /// for BW issue
        if ((IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))||(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)))
        {
            // SC
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, BIT(14), BIT(14));  // turn off ip read for bw issue
            //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK08_10_L, 0x7FFE); //[0] 0: 6bit, 1: 8bit, setup 6 bit
        }
        else
        {
            if(!gSrcInfo[MAIN_WINDOW].bFBL)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x00, BIT(14));
            }
            //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK08_10_L, 0x7FFF); //[0] 0: 6bit, 1: 8bit , Default is 8 bit
        }
    }
    else
    {
        if(!gSrcInfo[MAIN_WINDOW].bFBL)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x00, BIT(14));
        }
        //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK08_10_L, 0x7FFF); //[0] 0: 6bit, 1: 8bit , Default is 8 bit
    }
    // SC opm line buffer lens
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_24_L, 0xA03E); // OP read request
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_68_L, 0x3E00); // IPM arbiter
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_63_L, 0x3F00); // IPM arbiter
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_22_L, 0x2020); // IPM request length

#endif

    //frc w/r pointer patch to 1 because DS tearing
    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, 0x2000,BIT(13)|BIT(14)|BIT(15)); // IPM request length
    }
	else
	{
        // tearing also happen in app/games lg store left bottom AD, and it is not DS
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, 0x2000,BIT(13)|BIT(14)|BIT(15)); // IPM request length
	}

#if 1
    if((((pstXC_SetWin_Info->enInputSourceType >= INPUT_SOURCE_HDMI)&&(pstXC_SetWin_Info->enInputSourceType <= INPUT_SOURCE_HDMI_MAX))||
            ((pstXC_SetWin_Info->enInputSourceType >= INPUT_SOURCE_DVI)&&(pstXC_SetWin_Info->enInputSourceType <= INPUT_SOURCE_DVI_MAX)))
             && (pstXC_SetWin_Info->btvservice == 1))
            //&& !MApi_XC_IsGstreamerProcess())
    {
#if SUPPORT_HDMI20
        if(Hal_HDMI_CheckHDMI20_Setting(MDrv_XC_Mux_GetHDMIPort(pInstance, pstXC_SetWin_Info->enInputSourceType)))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," HDMI is 2P mode\n");
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_03_L,0x0000); // [7] reg_spt_2p_oe_en [0] reg_spt_420_en

            if(!MDrv_SC_set_2p_mode(pInstance,ENABLE,eWindow))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
            }

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,0x00, BIT(0));  // patch
            Hal_SC_set_bws_mode(pInstance, ENABLE, eWindow);
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," HDMI is 1P mode g_HdmiPollingStatus.u8ColorFormat = %d\n", g_HdmiPollingStatus.u8ColorFormat);
            /// patch for HDMI2.0 4K2K@420
            if((pstXC_SetWin_Info->stCapWin.width >= 0x780)    &&
               (pstXC_SetWin_Info->stCapWin.height >= 0x870)   &&
               (pstXC_SetWin_Info->u16InputVFreq >= 490)       &&
               (((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_FRAME_PACKING) ||
                  (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_LINE_ALTERNATIVE))))
            {
                //BK02_IP2
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," ip2 csc bypass\n");
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L,0x0000);
                //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L,0x0000);
                //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L,0x0000);
                if(!MDrv_SC_set_2p_mode(pInstance,ENABLE,eWindow))
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
                }
            }
            else
            {
                //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L,0x0000);
                //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L,0x0000);
                if(!MDrv_SC_set_2p_mode(pInstance,DISABLE,eWindow))
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
                }
            }

            if((pstXC_SetWin_Info->stCapWin.width >= 0x780) &&
               (pstXC_SetWin_Info->stCapWin.height >= 0x870) &&
               (((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_FRAME_PACKING) ||
                  (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_LINE_ALTERNATIVE)))||!(gSrcInfo[eWindow].Status2.eMirrorMode == MIRROR_NORMAL))
            {
                //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,0x00, BIT(0));  // patch
                Hal_SC_set_bws_mode(pInstance, ENABLE, eWindow);
            }
            else
            {
                //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,BIT(0), BIT(0));  // patch
                Hal_SC_set_bws_mode(pInstance, DISABLE, eWindow);
            }
            if (g_HdmiPollingStatus.u8ColorFormat != MS_HDMI_COLOR_YUV_420)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK40_03_L, 0x0000, 0x0001);
            }
        }
#endif
    }
    else if((pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_STORAGE)
            ||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_STORAGE2)
            ||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_DTV)
            ||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_DTV2)
            ||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_DTV3)  )
    {
        if((pstXC_SetWin_Info->stCapWin.width >= 0xF00)
                &&(pstXC_SetWin_Info->stCapWin.height >= 0x870)
                &&(pstXC_SetWin_Info->u16InputVFreq >= 330))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
            if(!MDrv_SC_set_2p_mode(pInstance,ENABLE,eWindow))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
            }

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,0x00, BIT(0));  // patch
            Hal_SC_set_bws_mode(pInstance, ENABLE, eWindow);
        }

        //monaco mm DS VB size always be 4k2k , mvop timing is 4k2k , 2p mode
        else if ( (MDrv_XC_GetDynamicScalingStatus(pInstance)) &&
                //(gSrcInfo[eWindow].Status2.u16VirtualBox_Width >= 0xF00) && (gSrcInfo[eWindow].Status2.u16VirtualBox_Height>= 0x870)&&(pstXC_SetWin_Info->u16InputVFreq >= 330) )
                ((R2BYTEMSK(0x101422,BIT(0))== BIT(0) && R2BYTEMSK(0x1014A6,BIT(1))== BIT(1)) //Main MVOP 4K2K 2p mode
                || (R2BYTEMSK(0x101322,BIT(0))== BIT(0) && R2BYTEMSK(0x1013A6,BIT(1))== BIT(1))))//Sub MVOP 4K2K 2p mode
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," monaco mm DS VB size always be 4k2k , mvop timing is 4k2k , 2p mode\n");
            if(!MDrv_SC_set_2p_mode(pInstance,ENABLE,eWindow))
				{
					XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
				}
            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,0x00, BIT(0));  // patch
            Hal_SC_set_bws_mode(pInstance, ENABLE, eWindow);
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," MVOP is 1P mode\n");
            if(!MDrv_SC_set_2p_mode(pInstance,DISABLE,eWindow))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," Please check the Scaler ID and eWindow\n");
            }

            if(!MDrv_XC_GetDynamicScalingStatus(pInstance))
            {
                if((pstXC_SetWin_Info->stCapWin.width >= 0xF00)
                        &&(pstXC_SetWin_Info->stCapWin.height >= 0x870))
                {
                    // 4k2k 30hz go through new mode no  need enable bws;
                }
                else
                {
                    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,BIT(0), BIT(0));  // patch
                    Hal_SC_set_bws_mode(pInstance, DISABLE, eWindow);
                }
            }
        }
    }
    else
    {
        MDrv_SC_set_2p_mode(pInstance,DISABLE,eWindow);
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L,BIT(0), BIT(0));  // patch
        Hal_SC_set_bws_mode(pInstance, DISABLE, eWindow);
    }

#endif

#if (HW_DESIGN_4K2K_VER == 4)
    MS_U16 u16Height  = SC_R2BYTEMSK(0, REG_SC_BK01_06_L, 0x1FFF);
    MS_U16 u16Vtt     = SC_R2BYTEMSK(0, REG_SC_BK01_1F_L, 0x1FFF);
    if (  (pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_VGA)
            &&(u16Vtt - u16Height <= 5))
    {
        MDrv_XC_MLoad_get_IP_trig_p(pInstance, &pXCResourcePrivate->sthal_SC.u16OldIPTrain, &pXCResourcePrivate->sthal_SC.u16OldIPDisp);
        MDrv_XC_MLoad_set_IP_trig_p(pInstance, 0x02, 0x03);
        MDrv_XC_MemControl_Switch_Method_By_Vcnt(pInstance, FALSE, 0x0);
    }
#if ENABLE_SW_DS
    else if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)
        && (pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_TV
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_CVBS
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_SCART
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_DTV
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_YPBPR
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_HDMI
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_HDMI2
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_HDMI3
        && pstXC_SetWin_Info->enInputSourceType != INPUT_SOURCE_HDMI4))
    {
        MDrv_XC_MLoad_set_IP_trig_p(pInstance, 0x01, 0x02);
        MDrv_XC_MemControl_Switch_Method_By_Vcnt(pInstance, FALSE, 0x00);
        //MDrv_XC_edclk_pd_control(pInstance, FALSE, 0x00, 0x00);
    }
    else
#endif
    {
        MS_U16 u16TrainTriggerPoint = 0;
        MS_U16 u16DispTriggerPoint = 0;
        MDrv_XC_MLoad_get_IP_trig_p(pInstance, &u16TrainTriggerPoint, &u16DispTriggerPoint);
        MDrv_XC_MemControl_Switch_Method_By_Vcnt(pInstance, TRUE, 0x04);
        if ((pXCResourcePrivate->sthal_SC.u16OldIPTrain != u16TrainTriggerPoint)
                || (pXCResourcePrivate->sthal_SC.u16OldIPDisp != u16DispTriggerPoint))
        {
            MDrv_XC_MLoad_set_IP_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16OldIPTrain, pXCResourcePrivate->sthal_SC.u16OldIPDisp);
        }
    }

#endif
    return bRet;
}

static MS_BOOL _Mdrv_XC_CopyDSSetWinInfo(XC_InternalStatus *pSrcInfo, XC_SETWIN_INFO *pstXC_SetWin_Info)
{
    if((pstXC_SetWin_Info == NULL) || (pSrcInfo == NULL))
    {
        return FALSE;
    }
    pSrcInfo->enInputSourceType     = pstXC_SetWin_Info->enInputSourceType;
    pSrcInfo->stCapWin.x            = pstXC_SetWin_Info->stCapWin.x;
    pSrcInfo->stCapWin.y            = pstXC_SetWin_Info->stCapWin.y;
    pSrcInfo->stCapWin.width        = pstXC_SetWin_Info->stCapWin.width;
    pSrcInfo->stCapWin.height       = pstXC_SetWin_Info->stCapWin.height;

    pSrcInfo->stDispWin.x           = pstXC_SetWin_Info->stDispWin.x;
    pSrcInfo->stDispWin.y           = pstXC_SetWin_Info->stDispWin.y;
    pSrcInfo->stDispWin.width       = pstXC_SetWin_Info->stDispWin.width;
    pSrcInfo->stDispWin.height      = pstXC_SetWin_Info->stDispWin.height;

    pSrcInfo->stCropWin.x           = pstXC_SetWin_Info->stCropWin.x;
    pSrcInfo->stCropWin.y           = pstXC_SetWin_Info->stCropWin.y;
    pSrcInfo->stCropWin.width       = pstXC_SetWin_Info->stCropWin.width;
    pSrcInfo->stCropWin.height      = pstXC_SetWin_Info->stCropWin.height;

    pSrcInfo->bInterlace            = pstXC_SetWin_Info->bInterlace;
    pSrcInfo->bHDuplicate           = pstXC_SetWin_Info->bHDuplicate;
    pSrcInfo->u16InputVFreq         = pstXC_SetWin_Info->u16InputVFreq;
    pSrcInfo->u16InputVTotal        = pstXC_SetWin_Info->u16InputVTotal;
    pSrcInfo->u16DefaultHtotal      = pstXC_SetWin_Info->u16DefaultHtotal;
    if(pstXC_SetWin_Info->u16DefaultPhase != 0)
    {
        pSrcInfo->u16DefaultPhase       = pstXC_SetWin_Info->u16DefaultPhase;
    }
    else
    {
        pSrcInfo->u16DefaultPhase       = pstXC_SetWin_Info->u8DefaultPhase;//compatible for old AP code
    }
    pSrcInfo->bHCusScaling          = pstXC_SetWin_Info->bHCusScaling;
    pSrcInfo->u16HCusScalingSrc     = pstXC_SetWin_Info->u16HCusScalingSrc;
    pSrcInfo->u16HCusScalingDst     = pstXC_SetWin_Info->u16HCusScalingDst;
    pSrcInfo->bVCusScaling          = pstXC_SetWin_Info->bVCusScaling;
    pSrcInfo->u16VCusScalingSrc     = pstXC_SetWin_Info->u16VCusScalingSrc;
    pSrcInfo->u16VCusScalingDst     = pstXC_SetWin_Info->u16VCusScalingDst;
    pSrcInfo->bDisplayNineLattice   = pstXC_SetWin_Info->bDisplayNineLattice;
    pSrcInfo->u8BitPerPixel         = pstXC_SetWin_Info->u8BitPerPixel;
    pSrcInfo->bMemFmt422         = pstXC_SetWin_Info->bMemFmt422;

    return TRUE;
}

static void MDrv_SC_set_ds_dnrsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL bIsConnectOPM, MS_U32* pu32OneLineOffset, MS_BOOL bDoMirror, SCALER_WIN eWindow, SC_SWDB_INFO *pstDBreg, XC_SETWIN_INFO *pstXC_SetWin_Info)
{
    MS_U16 u16V_Writelimit;
    MS_U32 u32Offset, u32DNRBase0, u32WritelimitBase=0;
    MS_U32 u32OPMBase0;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U16 u16AlignWidth = 0;
    //MS_U32 u32BaseOffset = 0, u32Framesize;
    MS_U32 u32BaseOffset = 0;
    MS_U32 u32BaseAddr, u32BuffSize, u32Framesize;
    MS_U32 u32OneLineOffset = 0;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16DNROffset = 0;
    MS_U8 u8FrameNum = 0;
    MS_U16 u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling;
    MS_U16 u16DNROffsetFor3D = 0;
    pSrcInfo->bIsConnectOPM = bIsConnectOPM;
    pstDBreg->bIsConnectOPM = pSrcInfo->bIsConnectOPM;
    if (bIsConnectOPM)
    {
        u32BaseAddr = pstXC_SetWin_Info->u32DSIPMBase0;
        u32BuffSize = pstXC_SetWin_Info->u32DSIPMBufSize;
    }
    else
    {
        u32BaseAddr = pstXC_SetWin_Info->u32DSFRCMBase0;
        u32BuffSize = pstXC_SetWin_Info->u32DSFRCMBufSize;
    }


    u16DNROffset = (pSrcInfo->Status2.u16VirtualBox_Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);

	XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u16DNROffset=%u\n",u16DNROffset);
	XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS] stSrcInfo.u16IPMFetch[%d]  \n", pSrcInfo->u16IPMFetch);

if (bDoMirror)
    {
#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
        Mdrv_SC_set_frcm_pre_align_pixel(pInstance, FALSE, 0, eWindow);
        if (pstDBreg->eMirrorMode == MIRROR_H_ONLY || pstDBreg->eMirrorMode == MIRROR_HV)
        {
            u16AlignWidth = u16DNROffset - pSrcInfo->u16IPMFetch;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"align width = %d\n", u16AlignWidth);

            // the hardware alignment in OPM > IPM case, it will get more data for alignment
            // disable it in the nine lattice mode
            if(pSrcInfo->bDisplayNineLattice)
            {
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
            }
            else
            {
                //these chips: they need sw to decide prealign or not.
                //here set to TRUE, then hw will prealign u16AlignWidth
                //if here is FALSE, hw won't do prealign

                //to fix H/W bugs, OPM need to be aligned.
                //so do not use pre-align. use OPM line buffer offset instead.
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, 0, eWindow);
            }
        }
#else
        if (pstDBreg->eMirrorMode == MIRROR_H_ONLY || pstDBreg->eMirrorMode == MIRROR_HV)
        {
            //M10, J2, A2 and later chip, HW will auto insert pixel to alignment u16IPMFetch
            //below u16AlignWidth is used for CBCR swap control
#ifdef K3_U2
            if (pSrcInfo->bMemFmt422)
                u16AlignWidth = u16DNROffset - pSrcInfo->u16H_SizeAfterPreScaling;
            else
                u16AlignWidth = u16DNROffset - pSrcInfo->u16IPMFetch;;
#endif
            // the hardware alignment in OPM > IPM case, it will get more data for alignment
            // disable it in the nine lattice mode
            if(pSrcInfo->bDisplayNineLattice)
            {
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
            }
            else
            {
                //these chips: when sw set it to TRUE, then hw will do the prealign.
                //because hw can calculate by itself, even for the alignwidth==0 case,
                //so sw don't need to worry about it, just set it to TRUE.
                Hal_SC_set_pre_align_pixel(pInstance, TRUE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at start
            }
        }
#endif
        else
        {
            u16AlignWidth = 0;
            Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        }
    }
    else
    {
        u16AlignWidth = 0;
        Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        u16AlignWidth = u16DNROffset - pSrcInfo->u16IPMFetch;
        if (pstDBreg->eMirrorMode == MIRROR_H_ONLY || pstDBreg->eMirrorMode == MIRROR_HV)
        {
            Mdrv_SC_set_frcm_pre_align_pixel(pInstance, TRUE, u16AlignWidth, eWindow);
        }
        else
        {
            Mdrv_SC_set_frcm_pre_align_pixel(pInstance, FALSE, 0, eWindow);
        }
    }

#ifndef DISABLE_3D_FUNCTION
        if((E_XC_3D_INPUT_SIDE_BY_SIDE_FULL == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
           || (E_XC_3D_INPUT_CHECK_BORAD == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
           || (E_XC_3D_INPUT_PIXEL_ALTERNATIVE == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]))
        {
            u16AlignWidth /= 2;            //opm fetch actual align size
            u16H_SizeAfterPreScaling /= 2; //actual prescaling hsize
        }
#endif
#if 0 // u16AlignWidth always is 0
    // alignwidth must be even, other color will be wrong and need to
    // use cb cr swap to correct it.
    if((u16AlignWidth & 0x01) && (HAL_SC_get_memory_bit_fmt(pInstance, eWindow) == 0))
    {
        Hal_SC_set_422_cbcr_swap(pInstance, ENABLE, eWindow);
    }
    else
    {
        Hal_SC_set_422_cbcr_swap(pInstance, DISABLE, eWindow);
    }
#endif
    u16DNROffsetFor3D = u16DNROffset;
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16DNROffsetFor3D,&u16DNROffset, eWindow);
#endif

    u32Framesize = pSrcInfo->Status2.u16VirtualBox_Height * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>Framesize(%d)= VSizeAfterPreSD(%d) * DNROffset(%d) * BytesPer2Pixel(%d)/2\n",(int)u32Framesize,pSrcInfo->u16V_SizeAfterPreScaling,u16DNROffset,u8BytesPer2Pixel);

    if (u32Framesize * 2 > u32BuffSize)
    {
        //this case should always not be happened
        printf("Attention! Scaler DNR Buffer is not enough for this timing, eWindow=%u!\n", eWindow);
    }

    // enable 4 frame mode if input is progressive & frame buffer is enough
    // , regardless the input source and timing because 4 frame mode is better in avoid tearing
    //
    #if 0 // not need ??
        XC_FRAME_STORE_NUMBER enLastFBNum = gSrcInfo[eWindow].Status2.eFrameStoreNumber;

        /*Update Frame buffer usage status and memory linear mode*/
        // MDrv_XC_GetPQSuggestedFrameNum(eWindow) is the frame count that PQ want to set
        // gSrcInfo[eWindow].Status2.eFrameStoreNumber is the frame count that sw code(_Mdrv_SC_CalcFBNum)
        //     decides according to frame size and other condition
        if (bIsConnectOPM)
        {
        _Mdrv_XC_Refine_FrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                           &gSrcInfo[eWindow].Status2.eFrameStoreNumber,
                           bInterlace,
                           u32Framesize,
                           u32BuffSize,
                           eWindow);
        }
        #if (HW_DESIGN_4K2K_VER == 4)
        else
        {
            _Mdrv_XC_Refine_FRCMFrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                               &gSrcInfo[eWindow].Status2.eFrameStoreNumber,
                               u32Framesize,
                               u32BuffSize,
                               eWindow);
        }
        #endif
    #endif
    if (bInterlace)
    {
        pSrcInfo->Status2.eFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        pSrcInfo->bLinearMode = 0;
        u32Offset =  u32BuffSize/2;
    }
    else
    {
        pSrcInfo->Status2.eFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        pSrcInfo->bLinearMode = 0;
        u32Offset =  u32BuffSize/3;
    }

	u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance,pSrcInfo->Status2.eFrameStoreNumber, pSrcInfo->bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%lu, DNRBufSize=%lu\n", u32Offset, u32BuffSize);
    u32DNRBase0 = u32BaseAddr;//MS_IPM_BASE0(eWindow);
    u32OPMBase0 = u32DNRBase0;

    if (bIsConnectOPM)
    {
        pstDBreg->u32OPMBase0    = u32OPMBase0;
    }
    else
    {
        pstDBreg->u32FRCM_RBase0  = u32OPMBase0;
    }
    // Cal DNR V write limit
    u16V_Writelimit = pSrcInfo->stCapWin.height;

    if( bInterlace )
        u16V_Writelimit = (u16V_Writelimit+1) / 2;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> i=%d, u16V_Writelimit[%d]= (u16V_Writelimit+1) / 2\n",bInterlace,u16V_Writelimit);
    // Cal DNR write limit

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"preview =>WritelimitBase[%u][%x] = (MS_IPM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD)\n",eWindow,(int)u32WritelimitBase, eWindow, (int)u32BaseAddr,(int)u32Offset);

    if(pSrcInfo->Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32WritelimitBase = (u32BaseAddr) + (((u32Offset + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD);
    else
        u32WritelimitBase = (u32BaseAddr) + (((u32Offset + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD);

     u32WritelimitBase = (eWindow == MAIN_WINDOW) ? (u32WritelimitBase - 1) | F2_WRITE_LIMIT_EN : (u32WritelimitBase - 1) | F1_WRITE_LIMIT_EN;

// V-Mirror
		if (bDoMirror)
		{
			if (pstDBreg->eMirrorMode == MIRROR_V_ONLY || pstDBreg->eMirrorMode == MIRROR_HV)
			{
				if(!pSrcInfo->bDisplayNineLattice)
				{
					MS_BOOL bMirrorBufferOverflow = FALSE;

					if( bInterlace )
					{
						u32BaseOffset = pSrcInfo->Status2.u16VirtualBox_Width * ((pSrcInfo->Status2.u16VirtualBox_Height-1)/2) * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD * u8FrameNum;
					}
					else
					{
						u32BaseOffset = pSrcInfo->Status2.u16VirtualBox_Width * (pSrcInfo->Status2.u16VirtualBox_Height -1) * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD * u8FrameNum;
					}

					XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," u32BaseOffset = %lx\n",u32BaseOffset);
					u32DNRBase0 += u32BaseOffset;

					// field packing mode, only dnrbase0 is used
					if((u32DNRBase0 < u32BaseOffset) || (u32DNRBase0 - u32BaseOffset < u32BaseAddr))
					{
						bMirrorBufferOverflow = TRUE;
					}


					if(bMirrorBufferOverflow)
					{
						printf("[%s,%5d] Attention! Scaler memory is overflow!\n",__FUNCTION__,__LINE__);
						MS_ASSERT(0);
					}
				}

				u32WritelimitBase = u32BaseAddr - 1;

				if(eWindow == MAIN_WINDOW)
					u32WritelimitBase= u32WritelimitBase | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
				else
					u32WritelimitBase= u32WritelimitBase | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
				}
		}


    pstDBreg->u16VLen = pSrcInfo->u16V_Length;
    if(eWindow == MAIN_WINDOW)
    {
        pstDBreg->u16VWritelimit = u16V_Writelimit | F2_V_WRITE_LIMIT_EN;
        pstDBreg->u16VFRCMWritelimit = u16V_Writelimit | F2_V_WRITE_LIMIT_EN;
    }
    else
    {
        pstDBreg->u16VWritelimit = u16V_Writelimit | F1_V_WRITE_LIMIT_EN;
    }

    pstDBreg->u32WritelimitBase = u32WritelimitBase;	
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]pstDBreg->u16DNROffset[%u]\n", u16DNROffset);
    pstDBreg->u16DNROffset = u16DNROffset;
    pstDBreg->u16DNRFetch  = pSrcInfo->u16IPMFetch;

    pstDBreg->u32DNRBase0  = u32DNRBase0;
    pstDBreg->u16OPMOffset = u16DNROffset;
    pstDBreg->u16AlignWidth = u16AlignWidth;
    pstDBreg->u16FRCM_ROffset = u16DNROffset;
    pstDBreg->u16FRCM_WOffset = u16DNROffset;
    pSrcInfo->u32IPMBase0 = u32DNRBase0 * BYTE_PER_WORD;
    pSrcInfo->u16IPMOffset = u16DNROffset;
    *pu32OneLineOffset = u32OneLineOffset;
    
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"pstDBreg->u16DNROffset[%u],pstDBreg->u16DNRFetch[%u]\n", pstDBreg->u16DNROffset,pstDBreg->u16DNRFetch);
	XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"pstDBreg->u32DNRBase0[%lx],pstDBreg->u16OPMOffset[%u]\n",pstDBreg->u32DNRBase0,pstDBreg->u16OPMOffset);
	XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"pstDBreg->u16FRCM_ROffset[%u],pstDBreg->u16FRCM_WOffset[%u]\n",pstDBreg->u16FRCM_ROffset,pstDBreg->u16FRCM_WOffset);
	XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"pSrcInfo->u32IPMBase0[%lx],pSrcInfo->u16IPMOffset[%u]\n",pSrcInfo->u32IPMBase0,pSrcInfo->u16IPMOffset);
}

static void MDrv_SC_set_ds_opmsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL bDoMirror, MS_BOOL bDoCrop, MS_U32 u32OneLineOffset, SCALER_WIN eWindow, SC_SWDB_INFO *pstDBreg, XC_SETWIN_INFO *pstXC_SetWin_Info)
{
    MS_U16 u16LBOffset, u16OPMFetch, u16OPMFetchAlignment;
    MS_U32 u32OPMBase0, u32OPMCropOffset =0, u32OPMPixelOffset;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U32 u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16DNROffsetFor3D = 0;
    MS_U16 u16DNROffset = pstDBreg->u16DNROffset;
    u16DNROffsetFor3D = u16DNROffset;

    u32Framesize = pSrcInfo->Status2.u16VirtualBox_Height * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    u32OPMBase0 = pstDBreg->u32OPMBase0; // ??
    /////////////////////////////////////////////////
    // Set OPM Memeory Settings                    //
    /////////////////////////////////////////////////

    // Cal OPM Base address
    // compute address offset for cropping
    // cropping shift h unit: 16 pixel
    // v unit: 2 line(for interlace), 1 line(for progressive)
    u32OPMPixelOffset = 0;

#if (HW_DESIGN_4K2K_VER < 4)
    if(bInterlace)
    {
        u32OPMPixelOffset = (MS_U32)((pSrcInfo->ScaledCropWin.y/2) * u16DNROffsetFor3D);
    }
    else
    {
        u32OPMPixelOffset = (MS_U32)((pSrcInfo->ScaledCropWin.y) * u16DNROffsetFor3D);
    }
    u32OPMPixelOffset += (MS_U32)(pSrcInfo->ScaledCropWin.x);
    MS_U32 u32OPM_Mirror_offset = 0;
    switch(pstDBreg->eMirrorMode)
    {
        case MIRROR_H_ONLY:
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][H Mirror] \n");
            u32OPM_Mirror_offset = (MS_U32)(pSrcInfo->Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
            break;
        case MIRROR_V_ONLY:
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][V Mirror] \n");
            u32OPM_Mirror_offset = (MS_U32) ((pSrcInfo->Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * pSrcInfo->Status2.u16VirtualBox_Width);
            break;
        case MIRROR_HV:
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][HV Mirror] \n");
            u32OPM_Mirror_offset = (MS_U32) ((pSrcInfo->Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * pSrcInfo->Status2.u16VirtualBox_Width);
            if(bInterlace)
            {
                u32OPM_Mirror_offset = u32OPM_Mirror_offset/2;
            }
            u32OPM_Mirror_offset += (MS_U32)(pSrcInfo->Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
            break;
        default:
            break;
    }
    u32OPMPixelOffset += u32OPM_Mirror_offset;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS] ScaledCrop[%u][%u][%u][%u], H_SizeAfterPreScaling[%u], V_SizeAfterPreScaling[%u] \n",pSrcInfo->ScaledCropWin.x, pSrcInfo->ScaledCropWin.y, pSrcInfo->ScaledCropWin.width, pSrcInfo->ScaledCropWin.height, pSrcInfo->u16H_SizeAfterPreScaling,pSrcInfo->u16V_SizeAfterPreScaling);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS] u32OPM_Mirror_offset = %lx, u32OPMPixelOffset = %lx\n",u32OPM_Mirror_offset, u32OPMPixelOffset);

#endif
    // calculate LB offset
    u16LBOffset = u32OPMPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
    pSrcInfo->Status2.u16LBOffset = u16LBOffset;
    u32OPMPixelOffset -= u16LBOffset;                                    // these pixels rely on base offset

    if (pSrcInfo->bInterlace)
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        pSrcInfo->bLinearMode = 0;
    }
    else
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        pSrcInfo->bLinearMode = 0;
    }
    // calcuate OPM offset
    u32OPMPixelOffset *= MDrv_XC_TransFrameNumToFactor(pInstance, pSrcInfo->Status2.eFrameStoreNumber, pSrcInfo->bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    if(u32OPMPixelOffset != 0)
    {
        u32OPMCropOffset = u32OPMPixelOffset * u8BytesPer2Pixel / 2 / BYTE_PER_WORD;
    }    
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS] u32OPMBase0 = %lx, u32OPMCropOffset = %lx\n",u32OPMBase0,u32OPMCropOffset);
    //KT: T3 PIP HW issue, When SUB ON, the HW operation will decrease "u16LBOffset" 2 times.
    MS_U16 CropWinWidthTemp = 0;
    
#if (HW_DESIGN_4K2K_VER == 4)
    CropWinWidthTemp = pSrcInfo->u16H_SizeAfterPreScaling;
#else
    CropWinWidthTemp = pSrcInfo->ScaledCropWin.width;
#endif

#ifndef DISABLE_3D_FUNCTION
    if (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
    {
        CropWinWidthTemp /= 2;
        if((CropWinWidthTemp & 0x1)!= 0)
        {
            CropWinWidthTemp = (CropWinWidthTemp+1)& ~1;
        }
    }
#endif
    u16OPMFetch = CropWinWidthTemp + u16LBOffset; // using LBOffset still need to add some points to avoid generating garbage


    {
        //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align
        //  OPM fetch:
        //            Janus /T7 --> 32pix align
        //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align

        MS_U16 u16tempOPM;
        if((u16OPMFetch != pSrcInfo->u16IPMFetch)  &&  (E_XC_3D_OUTPUT_MODE_NONE == pstXC_SetWin_Info->eOutput3DMode))
        {
            //UCDi need grab six more pixels, not 3D case.(a7p need 6 more, others need 2 more)
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1) + 6;
            if (!bDoCrop || (u16tempOPM > pSrcInfo->u16H_SizeAfterPreScaling - (pSrcInfo->ScaledCropWin.x - u16LBOffset)))
            {
                u16OPMFetch = u16OPMFetch & ~1;
            }
            else
            {
                u16OPMFetch = u16tempOPM;
            }
        }
        else
        {
            u16OPMFetch = ( u16OPMFetch + 1 ) & ~1;  //  pixels  alignment
        }

        if (!pSrcInfo->bMemFmt422)
        {
            u16OPMFetchAlignment = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
            if (bDoCrop)
            {
                //HW issue for OPM fetch when OP write enable, OPM Fetch have to offset pixels alignment avoid green garbage .
                if (pSrcInfo->u16H_SizeAfterPreScaling >= (u16OPMFetchAlignment+ pSrcInfo->ScaledCropWin.x - u16LBOffset))
                {
                    u16OPMFetch = u16OPMFetchAlignment;
                }
                else
                {
                    //Overflow, set the OPM Fetch to the max data area .
                    u16OPMFetch = pSrcInfo->u16H_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.x + u16LBOffset;
                    u16OPMFetch = u16OPMFetch  & ~1;
                }
            }
            else
            {
                u16OPMFetch = u16OPMFetch  & ~1;
            }
        }
        else
        {
            if(2200>=u16OPMFetch)   //Nike 4k2k opm fetch can't do ALIGNMENT
                u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
    }
    //u16DispOffset = 0;
    u32OPMBase0 += u32OPMCropOffset;
    pstDBreg->u32OPMBase0  = u32OPMBase0;
    pstDBreg->u16OPMFetch  = u16OPMFetch;
    pstDBreg->u16LBOffset  = u16LBOffset;

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pstDBreg->u32OPMBase0[%lx],pstDBreg->u16OPMFetch[%u]\n", pstDBreg->u32OPMBase0,pstDBreg->u16OPMFetch);
}

static void MDrv_SC_set_ds_frcmwsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_U32* pu32OneLineOffset, SCALER_WIN eWindow, SC_SWDB_INFO *pstDBreg, XC_SETWIN_INFO *pstXC_SetWin_Info)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MS_U16 u16V_FRCMWritelimit;
    MS_U32 u32Offset, u32FRCM_WBase0, u32WritelimitBase=0;
    MS_U32 u32OneLineOffset = 0;
    MS_U16 u16AlignWidth=0;
    MS_U32 u32BaseOffset = 0;
    MS_U32 u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16FRCM_WOffset = 0;
    MS_U8 u8FrameNum = 0;

    /////////////////////////////////////////////////
    // Set FRCM_W Memeory Settings                    //
    /////////////////////////////////////////////////
    // FRCM_W Fetch/Offset = OPM Fetch/Offset
    pstDBreg->u16FRCM_WFetch = pstDBreg->u16OPMFetch;
    pstDBreg->u16FRCM_WOffset = pstDBreg->u16OPMOffset;

    u16FRCM_WOffset = (pSrcInfo->Status2.u16VirtualBox_Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
    u32Framesize = pSrcInfo->Status2.u16VirtualBox_Height * (MS_U32)u16FRCM_WOffset * (MS_U32)u8BytesPer2Pixel/2;

    if((u32Framesize * 2 > pstXC_SetWin_Info->u32DSFRCMBufSize))
    {
        //this case should always not be happened
        printf("Attention! Scaler FRCM Buffer is not enough for this timing, eWindow=%u!\n", eWindow);
    }

    // enable 4 frame mode if input is progressive & frame buffer is enough
    // , regardless the input source and timing because 4 frame mode is better in avoid tearing
    //

    if (pSrcInfo->bInterlace)
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        pSrcInfo->bLinearMode = 0;
        u32Offset =  pstXC_SetWin_Info->u32DSFRCMBufSize/2;
    }
    else
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        pSrcInfo->bLinearMode = 0;
        u32Offset =  pstXC_SetWin_Info->u32DSFRCMBufSize/3;
    }

	u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance,pSrcInfo->Status2.eFrameStoreNumber, pSrcInfo->bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    //u32Offset = (MS_U32)pSrcInfo->u16V_Length * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    //fix u32DNRBase1 at SCALER_DNR_BUF_LEN/2 for not using blue-screen at channel change in ATV
    u32FRCM_WBase0 = pstXC_SetWin_Info->u32DSFRCMBase0;
    pstDBreg->u32FRCM_RBase0  = u32FRCM_WBase0;


    // Cal FRCM V write limit
    {
        u16V_FRCMWritelimit = pSrcInfo->stCapWin.height;
    }

    // Cal FRCM write limit

    if(pSrcInfo->Status2.eFRCMFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32WritelimitBase = pstXC_SetWin_Info->u32DSFRCMBase0 + (((u32Offset + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD);
    else
        u32WritelimitBase = pstXC_SetWin_Info->u32DSFRCMBase0 + (((u32Offset + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD);

      u32WritelimitBase = (eWindow == MAIN_WINDOW) ? (u32WritelimitBase - 1) | F2_WRITE_LIMIT_EN : (u32WritelimitBase - 1) | F1_WRITE_LIMIT_EN;
// V-mirror
		if (pstDBreg->eMirrorMode == MIRROR_V_ONLY || pstDBreg->eMirrorMode == MIRROR_HV)
		{
			if(!pSrcInfo->bDisplayNineLattice)
			{
				MS_U16 u16V_SizeAfterPreScaling = pSrcInfo->u16V_SizeAfterPreScaling;
				MS_U16 u16FRCM_WOffsetTemp = u16FRCM_WOffset;
				MS_U16 u16Ratio = 1;
				MS_BOOL bMirrorBufferOverflow = FALSE;

				//base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
				u32OneLineOffset = u8FrameNum * (MS_U32)u16Ratio * (MS_U32)u16FRCM_WOffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

				//Progressived mode, non-linear address
				//base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
				u32BaseOffset = (MS_U32)((MS_U32)u16Ratio * u8FrameNum*(u16V_SizeAfterPreScaling-1)) * (MS_U32)u16FRCM_WOffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

				u32FRCM_WBase0 += u32BaseOffset;

				// field packing mode, only dnrbase0 is used
				if((u32FRCM_WBase0 < u32BaseOffset) || (u32FRCM_WBase0 - u32BaseOffset < pstXC_SetWin_Info->u32DSFRCMBase0))
				{
					bMirrorBufferOverflow = TRUE;
				}

				if(bMirrorBufferOverflow)
				{
					printf("[%s,%5d] Attention! Scaler memory is overflow!\n",__FUNCTION__,__LINE__);
					MS_ASSERT(0);
				}
			}

			u32WritelimitBase = pstXC_SetWin_Info->u32DSFRCMBase0 - 1;

			if(eWindow == MAIN_WINDOW)
				u32WritelimitBase= u32WritelimitBase | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
			else
				u32WritelimitBase= u32WritelimitBase | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
		}

    pstDBreg->u16FRCMVLen = pSrcInfo->u16FRCMV_Length;
    if(eWindow == MAIN_WINDOW)
    pstDBreg->u16VFRCMWritelimit = u16V_FRCMWritelimit | F2_V_WRITE_LIMIT_EN;
    else
    pstDBreg->u16VFRCMWritelimit = u16V_FRCMWritelimit | F1_V_WRITE_LIMIT_EN;

    pstDBreg->u32FRCMWritelimitBase = u32WritelimitBase;
    pstDBreg->u16FRCM_WOffset = u16FRCM_WOffset;
    pstDBreg->u16FRCM_WFetch  = pstDBreg->u16OPMFetch;
    pstDBreg->u32FRCM_WBase0  = u32FRCM_WBase0;
    pstDBreg->u16AlignWidth = u16AlignWidth;
    *pu32OneLineOffset = u32OneLineOffset;
#endif
}

static void MDrv_SC_set_ds_frcmrsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_U32 u32OneLineOffset, SCALER_WIN eWindow, SC_SWDB_INFO *pstDBreg, XC_SETWIN_INFO *pstXC_SetWin_Info)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MS_U16 u16LBOffset, u16DispOffset, u16FRCM_RFetch, u16FRC_RFetchAlignment;
    MS_U32 u32FRCM_RBase0, u32FRCM_RCropOffset =0, u32FRCM_RPixelOffset;
    MS_U16 u16AlignWidth=0;
    //MS_U32 u32BaseOffset = 0;
    //MS_U32 u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16FRCM_ROffset = 0;

    u16FRCM_ROffset = pstDBreg->u16DNROffset;
    u32FRCM_RBase0  = pstDBreg->u32FRCM_RBase0;
    //u32BaseOffset = pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[eWindow];
    /////////////////////////////////////////////////
    // Set FRCM_R Memeory Settings                    //
    /////////////////////////////////////////////////

    // Cal FRCM_R Base address

    // compute address offset for cropping
    // cropping shift h unit: 16 pixel
    // v unit: 2 line(for interlace), 1 line(for progressive)

    // how to do?
    {
#ifndef DISABLE_3D_FUNCTION
        MS_U16 u16FRCM_ROffset3D = u16FRCM_ROffset;
        //MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16FRCM_ROffset3D,&u16FRCM_ROffset, eWindow);
        if (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
        {
            u16FRCM_ROffset3D = ((u16FRCM_ROffset3D / 2) + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
            u16FRCM_ROffset = u16FRCM_ROffset3D*2;
        }
#endif
        u32FRCM_RPixelOffset = (MS_U32)((pSrcInfo->ScaledCropWin.y) * u16FRCM_ROffset3D);
        u32FRCM_RPixelOffset += (MS_U32)(pSrcInfo->ScaledCropWin.x);

		MS_U32 u32FRCM_Mirror_offset = 0;
		switch(pstDBreg->eMirrorMode)
		{
			case MIRROR_H_ONLY:
				XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][H Mirror] \n");
				u32FRCM_Mirror_offset = (MS_U32)(pSrcInfo->Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
				break;
			case MIRROR_V_ONLY:
				XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][V Mirror] \n");
				u32FRCM_Mirror_offset = (MS_U32) ((pSrcInfo->Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * pSrcInfo->Status2.u16VirtualBox_Width);
				break;
			case MIRROR_HV:
				XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS][HV Mirror] \n");
			u32FRCM_Mirror_offset = (MS_U32) ((pSrcInfo->Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * pSrcInfo->Status2.u16VirtualBox_Width);
			u32FRCM_Mirror_offset += (MS_U32)(pSrcInfo->Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
				break;
			default:
				break;
    }

			u32FRCM_RPixelOffset +=	u32FRCM_Mirror_offset;
		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS] ScaledCrop[%u][%u][%u][%u], H_SizeAfterPreScaling[%u], V_SizeAfterPreScaling[%u] \n",pSrcInfo->ScaledCropWin.x, pSrcInfo->ScaledCropWin.y, pSrcInfo->ScaledCropWin.width, pSrcInfo->ScaledCropWin.height, pSrcInfo->u16H_SizeAfterPreScaling,pSrcInfo->u16V_SizeAfterPreScaling);
		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," [DS] u32FRCM_Mirror_offset = %lx, u32FRCM_RPixelOffset = %lx\n",u32FRCM_Mirror_offset, u32FRCM_RPixelOffset);
    }

    // calculate LB offset
    u16LBOffset = u32FRCM_RPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
    // line buffer need to check.
    pSrcInfo->Status2.u16LBOffset = u16LBOffset;
    u32FRCM_RPixelOffset -= u16LBOffset;                                    // these pixels rely on base offset    
    if (pSrcInfo->bInterlace)
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        pSrcInfo->bLinearMode = 0;
    }
    else
    {
        pSrcInfo->Status2.eFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        pSrcInfo->bLinearMode = 0;
    }
    // calcuate FRCM_R offset
    u32FRCM_RPixelOffset *= MDrv_XC_TransFrameNumToFactor(pInstance, pSrcInfo->Status2.eFRCMFrameStoreNumber, pSrcInfo->bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    if(u32FRCM_RPixelOffset != 0)
    {
        MS_U8 u8BytesPer2PixelTemp = u8BytesPer2Pixel;
#ifndef DISABLE_3D_FUNCTION
        //MDrv_SC_3D_Adjust_BytesPer2Pixel(pInstance, &u8BytesPer2PixelTemp,eWindow);
        if ((E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]) ||
            (E_XC_3D_INPUT_TOP_BOTTOM == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]))
        {
            u8BytesPer2PixelTemp *= 2;
        }
#endif
        u32FRCM_RCropOffset = u32FRCM_RPixelOffset * u8BytesPer2PixelTemp / 2 / BYTE_PER_WORD;
    }


    //KT: T3 PIP HW issue, When SUB ON, the HW operation will decrease "u16LBOffset" 2 times.

    MS_U16 CropWinWidthTemp = pSrcInfo->ScaledCropWin.width;
#ifndef DISABLE_3D_FUNCTION
    //MDrv_SC_3D_Adjust_CropWinWidth(pInstance, &CropWinWidthTemp,eWindow);
    if (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
    {
        CropWinWidthTemp /= 2;
        if((CropWinWidthTemp & 0x1)!= 0)
        {
            CropWinWidthTemp = (CropWinWidthTemp+1)& ~1;
        }
    }
#endif
    u16FRCM_RFetch = CropWinWidthTemp + u16LBOffset; // using LBOffset still need to add some points to avoid generating garbage

    //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align
    //  OPM fetch:
    //            Janus /T7 --> 32pix align
    //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align
    MS_U16 u16tempFRCM_R;

    if((u16LBOffset & 0x1) != 0)
    {
        //HW issue for OPM fetch&422to444: when lboffset not even, OPM should add 2 to avoid right line garbage
        u16tempFRCM_R = (( u16FRCM_RFetch + 1 ) & ~1) + 2;		
        if(u16tempFRCM_R > pSrcInfo->u16H_SizeAfterPreScaling - (pSrcInfo->ScaledCropWin.x - u16LBOffset))
        {
            u16LBOffset--;//Overflow, set lb offset even and alignment OPM
            u16FRCM_RFetch = u16FRCM_RFetch & ~1; //Make even directly, since lb offset has minus 1
        }
        else
        {
            u16FRCM_RFetch = u16tempFRCM_R;
        }
    }
    else if((u16FRCM_RFetch != pSrcInfo->u16IPMFetch)  &&  (E_XC_3D_OUTPUT_MODE_NONE == pstXC_SetWin_Info->eOutput3DMode))
    {
        //UCDi need grab six more pixels, not 3D case.(a7p need 6 more, others need 2 more)
        u16tempFRCM_R = (( u16FRCM_RFetch + 1 ) & ~1) + 6;
        if(u16tempFRCM_R > pSrcInfo->u16H_SizeAfterPreScaling - (pSrcInfo->ScaledCropWin.x - u16LBOffset))
        {
            u16FRCM_RFetch = u16FRCM_RFetch & ~1;
        }
        else
        {
            u16FRCM_RFetch = u16tempFRCM_R;
        }
    }
    else
    {
        u16FRCM_RFetch = ( u16FRCM_RFetch + 1 ) & ~1;  //  pixels  alignment
    }
    if (!pSrcInfo->bMemFmt422)
    {
        u16FRC_RFetchAlignment = (u16FRCM_RFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        //HW issue for OPM fetch when OP write enable, OPM Fetch have to offset pixels alignment avoid green garbage .
        if(pSrcInfo->u16H_SizeAfterPreScaling >= (u16FRC_RFetchAlignment+pSrcInfo->ScaledCropWin.x - u16LBOffset))
        {
            u16FRCM_RFetch = u16FRC_RFetchAlignment;
        }
        else
        {
            //Overflow, set the OPM Fetch to the max data area .
            u16FRCM_RFetch = pSrcInfo->u16H_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.x + u16LBOffset;
            u16FRCM_RFetch = u16FRCM_RFetch  & ~1;
        }
    }
    else
    {
        if(2200>=u16FRCM_RFetch)   //Nike 4k2k opm fetch can't do ALIGNMENT
            u16FRCM_RFetch = (u16FRCM_RFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
    }
    u16DispOffset = 0;
    // adjsut OPMBase address for cropping
    u32FRCM_RBase0 += u32FRCM_RCropOffset;
    //-----------------------------------------------------------------
    pstDBreg->u16LBOffset  = u16LBOffset;
    pstDBreg->u16DispOffset  = u16DispOffset;
    pstDBreg->u32FRCM_RBase0  = u32FRCM_RBase0;
    pstDBreg->u16FRCM_RFetch  = u16FRCM_RFetch;
    pstDBreg->u16AlignWidth = u16AlignWidth;
    //-----------------------------------------------------------------
#endif
}

static void _MDrv_SC_ds_3D_Adjust_PreScaling(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                             E_XC_3D_OUTPUT_MODE eOutputMode,
                                             XC_InternalStatus *pSrcInfo,
                                             SCALER_WIN eWindow)
{
    MS_BOOL b3DPreVScaling=FALSE, b3DPreHScaling=FALSE;
#if (HW_DESIGN_3D_VER != 4)
    if((eOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) ||
       (eOutputMode == E_XC_3D_OUTPUT_FRAME_L))
    {
        //for best quality, don't do prescaling when not necessary
        if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
        }

        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
        }
    }
#endif

    if(eInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
    {
        if(eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
        {
            if(pSrcInfo->bInterlace)
            {
                if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
                }
            }
            else
            {
                if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
                }
            }
        }
    }
    else if(eInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
    {
        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            if(eOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            {
                b3DPreHScaling=TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width;
            }
        }
    }
    else if(eInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
    {
        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            if(eOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            {
                b3DPreHScaling=TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            }
            else if(eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                b3DPreVScaling=TRUE;
                pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                if(pSrcInfo->stCapWin.height <= pSrcInfo->stDispWin.height/2)
                {
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
                }
                else
                {
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
                }
            }
            else if(eOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
            {
                if(pSrcInfo->stCapWin.height > pSrcInfo->stDispWin.height)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
                }
            }
        }
    }
#if 0
    if(MDrv_SC_GetPQHSDFlag(pInstance) == TRUE)
    {
        b3DPreHScaling = FALSE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;
    }
#endif
    //For double HD topbottom, like 1920x2160, 1280x1440
    if(((eInputMode == E_XC_3D_INPUT_TOP_BOTTOM) && (eOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)) && (pSrcInfo->stCapWin.height >= 1440))
    {
        b3DPreVScaling = FALSE;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->u16V_SizeAfterPreScaling;
    }
#if 0
    //(1)protect HCusscaling size according to the line buffer
    if(!(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
            || IS_INPUT_NORMAL_2D_HW(eWindow)
            || IS_INPUT_CHECK_BOARD(eWindow)
            || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
            || IS_OUTPUT_FRAME_ALTERNATIVE()
#if (HW_DESIGN_3D_VER >= 4)
            || IS_OUTPUT_TOP_BOTTOM()
#endif
            || MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow)
        )
            ||(IS_INPUT_LINE_ALTERNATIVE(eWindow)&&IS_OUTPUT_TOP_BOTTOM())
            ||(IS_INPUT_NORMAL_2D_HW(eWindow)&&IS_OUTPUT_TOP_BOTTOM_HW())
            ||(IS_INPUT_NORMAL_2D_HW(eWindow)&&IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
      )
    {
        //These foramts will use subwindow, need check if the linebuff is enough
        //MS_3D_LINE_BFF_MAX is DI line buffer max
        if(pSrcInfo->Status2.u16PreHCusScalingDst > MS_3D_LINE_BFF_MAX)
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = MS_3D_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "For output line buffer limit => H :PreHScaling=%s, src=%u, dst=%u\n",
                         (b3DPreHScaling?"Yes":"No"), pSrcInfo->Status2.u16PreHCusScalingSrc, pSrcInfo->Status2.u16PreHCusScalingDst);

        }
    }
#endif
    //(3)alignment check
    if(eInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
    {
        //For SBS input, should make sure the Hsize/2 is even to avoid the color mismatch issue in 422 mode
        if(pSrcInfo->Status2.u16PreHCusScalingDst & (BIT(1)))
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingDst = (pSrcInfo->Status2.u16PreHCusScalingDst>>2)<<2;
        }
    }

    if(eInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
    {
        //For TPB/LAP input, should make sure the Vsize is even
        if(pSrcInfo->Status2.u16PreVCusScalingDst & BIT(0))
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingDst & ~0x01;
        }
    }

    if(b3DPreVScaling || b3DPreHScaling)
    {
        pSrcInfo->Status2.bPreHCusScaling = TRUE;//If enable, then need Enable customer scaling both
        pSrcInfo->Status2.bPreVCusScaling = TRUE;
    }
}

void _MDrv_SC_ds_3D_Adjust_PstScaling(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                      E_XC_3D_OUTPUT_MODE eOutputMode,
                                      XC_InternalStatus *pSrcInfo,
                                      SCALER_WIN eWindow)
{
    MS_BOOL b3DPstVScaling=FALSE, b3DPstHScaling=FALSE;

    if ((eOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) ||
        (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)       ||
        (eOutputMode == E_XC_3D_OUTPUT_FRAME_L))
    {
        if(eInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(eInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(eInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }

        if(eOutputMode == E_XC_3D_OUTPUT_FRAME_L)
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16VCusScalingDst <<= 1; //For 3D->2D L/R frame output, set to scaling up twice
        }
    }
    else if(eOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
    {
        if(eInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
    }
    else if(eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        if(eInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        {
            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;

        }
        else if(eInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
    }

#if 0
    if((!MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, eInputMode, eOutputMode, eWindow))
            && ((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == eOutputMode)
                || (E_XC_3D_OUTPUT_TOP_BOTTOM == eOutputMode)
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == eOutputMode)
                || (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == eOutputMode)))
    {
        pSrcInfo->u16HCusScalingDst += (pXCResourcePrivate->stdrvXC_3D._u163DHShift&0xFF);
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16HCusScalingSrc=%d, u16HCusScalingDst=%d\n", pSrcInfo->u16HCusScalingSrc, pSrcInfo->u16HCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16VCusScalingSrc=%d, u16VCusScalingDst=%d\n", pSrcInfo->u16VCusScalingSrc, pSrcInfo->u16VCusScalingDst);

    if(b3DPstVScaling || b3DPstHScaling)
    {
        pSrcInfo->bVCusScaling = TRUE;
        pSrcInfo->bHCusScaling = TRUE;
    }

}

//-------------------------------------------------------------------------------------------------
/// Set the specific window
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen         \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
// 3D DS only support SBSH/TNB
MS_BOOL MApi_XC_SetDynamicScalingWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16ScaleSrc = 0, u16ScaleDst = 0;
    MS_U16 u16SrcHWidth = 0;
    MS_U32 u32TmpPre=0;
    MS_U32 u32TmpPost=0;
    MS_U32 u32Value = 0;
    MS_U32 u32OneLineOffset = 0;
    SC_SWDB_INFO stDBreg;
    XC_InternalStatus stSrcInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (pstXC_SetWin_Info == NULL)
    {
        return FALSE;
    }
    if((pstXC_SetWin_Info->stCapWin.width == 0) || (pstXC_SetWin_Info->stCapWin.height == 0) ||
       (pstXC_SetWin_Info->stCropWin.width == 0) || (pstXC_SetWin_Info->stCropWin.height == 0) ||
       (pstXC_SetWin_Info->stDispWin.width == 0) || (pstXC_SetWin_Info->stDispWin.height == 0))
    {
        //printf("XC CapWin is error ! (Width=%d,Height=%d)\n",pSrcInfo->stCapWin.width,pSrcInfo->stCapWin.height);
        return FALSE;
    }

    memset(&stDBreg,0,sizeof(SC_SWDB_INFO));
    memset(&stSrcInfo,0,sizeof(XC_InternalStatus));

    // Set 3D status
    stDBreg.b3Dstate =   (!((pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)))
                                                 && (!((pstXC_SetWin_Info->eOutput3DMode == E_XC_3D_OUTPUT_MODE_NONE)))
                                                 && ((!(pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D_HW )) || ((pstXC_SetWin_Info->eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)) || ((pstXC_SetWin_Info->eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)));
	// 3D DS only support TNB and SBSH input
    if (stDBreg.b3Dstate == 1)
    {
        if((E_XC_3D_INPUT_TOP_BOTTOM == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])  ||
           (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF ==  pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]))
        {
            ;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[3D DS] Unsupported 3D DS input format !!!\n");
            return FALSE;
        }
    }

#ifndef DISABLE_3D_FUNCTION
//patch for monaco 3D SC HW limitation, if input is SBS, 2D_HW, PA, CB, capture window's width must be align by 4
#if(HW_DESIGN_3D_VER == 5) //monaco mode
    if((E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])  ||
       (E_XC_3D_INPUT_PIXEL_ALTERNATIVE ==  pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]) ||
       (E_XC_3D_INPUT_CHECK_BORAD ==  pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW]))
    {
        pstXC_SetWin_Info->stCapWin.width &= ~0x03;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[3D DS] pstXC_SetWin_Info->stCapWin.widt[%d] ( if input is SBS, 2D_HW, PA, CB, capture window's width must be align by 4) \n", pstXC_SetWin_Info->stCapWin.width);
    }
#endif

    //for 3D handle invalid display window
    if((E_XC_3D_OUTPUT_LINE_ALTERNATIVE    == pstXC_SetWin_Info->eOutput3DMode) ||
       (E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW == pstXC_SetWin_Info->eOutput3DMode) ||
       (E_XC_3D_OUTPUT_TOP_BOTTOM          == pstXC_SetWin_Info->eOutput3DMode))
      //|| ((E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)) && MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)))
    {
        pstXC_SetWin_Info->stDispWin.y &= ~0x1;
        pstXC_SetWin_Info->stDispWin.height &= ~0x1;
    }
    else if( E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eOutput3DMode)
    {
        pstXC_SetWin_Info->stDispWin.width &= ~0x1;
    }
    else if(E_XC_3D_OUTPUT_TOP_BOTTOM == pstXC_SetWin_Info->eOutput3DMode)
    {
        pstXC_SetWin_Info->stDispWin.height &= ~0x1;
    }
#endif
    MS_U32 u32TempWidth = pstXC_SetWin_Info->stDispWin.width;
    if(IsHMirrorMode(eWindow))
    {
    	if(MDrv_XC_Is2K2KToFrc(pInstance))
    	{
    		if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2 > ( pstXC_SetWin_Info->stDispWin.x + u32TempWidth ) )
    		{
    				pstXC_SetWin_Info->stDispWin.x =
    				pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width / 2 - ( pstXC_SetWin_Info->stDispWin.x + u32TempWidth );
    		}
    		else
    		{
    			pstXC_SetWin_Info->stDispWin.x = 0;
    		}
    	}
    	else
    	{
    		if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > ( pstXC_SetWin_Info->stDispWin.x + u32TempWidth ) )
    		{
    				pstXC_SetWin_Info->stDispWin.x =
    				pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - ( pstXC_SetWin_Info->stDispWin.x + u32TempWidth );
    		}
    		else
    		{
    			pstXC_SetWin_Info->stDispWin.x = 0;
    		}
    	}
    }

    if(IsVMirrorMode(eWindow))
	{
		if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > ( pstXC_SetWin_Info->stDispWin.y + pstXC_SetWin_Info->stDispWin.height) )
		{
				pstXC_SetWin_Info->stDispWin.y =
				pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - ( pstXC_SetWin_Info->stDispWin.y + pstXC_SetWin_Info->stDispWin.height);
		}
		else
		{
			pstXC_SetWin_Info->stDispWin.y = 0;
		}
    }

    //pstXC_SetWin_Info->stDispWin.x += pstXC_SetWin_Info->u16panelHStart;
    //pstXC_SetWin_Info->stDispWin.y += pstXC_SetWin_Info->u16panelHStart;
    _Mdrv_XC_CopyDSSetWinInfo(&stSrcInfo, pstXC_SetWin_Info);
    _MDrv_XC_CheckCropWin(pstXC_SetWin_Info);

    stDBreg.eMirrorMode = gSrcInfo[(eWindow)].Status2.eMirrorMode;
    stDBreg.u16H_CapStart       = pstXC_SetWin_Info->stCapWin.x;
    stDBreg.u16H_CapSize        = pstXC_SetWin_Info->stCapWin.width;
    stDBreg.u16V_CapStart       = pstXC_SetWin_Info->stCapWin.y;
    stDBreg.u16V_CapSize        = pstXC_SetWin_Info->stCapWin.height;
    stDBreg.bInterlace          = pstXC_SetWin_Info->bInterlace;

    stSrcInfo.stDispWin.x += pstXC_SetWin_Info->u16panelHStart;
    stSrcInfo.stDispWin.y += pstXC_SetWin_Info->u16panelVStart;
    stDBreg.u16H_DisStart = stSrcInfo.stDispWin.x;
    stDBreg.u16V_DisStart = stSrcInfo.stDispWin.y;

    // MDrv_XC_Get_VirtualBox_Info
    if (pstXC_SetWin_Info->bInterlace)
    {
        stSrcInfo.Status2.u16VirtualBox_Width  = 1920;
        stSrcInfo.Status2.u16VirtualBox_Height = 1080;
    }
    else
    {
        if(XC_SUPPORT_4K2K)
		{
			stSrcInfo.Status2.u16VirtualBox_Width  = 3840;
        	stSrcInfo.Status2.u16VirtualBox_Height = 2160;
		}
		else
		{
			stSrcInfo.Status2.u16VirtualBox_Width  = 1920;
        	stSrcInfo.Status2.u16VirtualBox_Height = 1080;
		}
    }
    if (stDBreg.b3Dstate == 1)
    {
       //MDrv_SC_3D_Adjust_PreScaling(pInstance, eInputMode, eOutputMode, pSrcInfo, eWindow);
       _MDrv_SC_ds_3D_Adjust_PreScaling(pInstance, pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW], pstXC_SetWin_Info->eOutput3DMode, &stSrcInfo, eWindow);
    }

    //MDrv_SC_set_prescaling_ratio(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow );
    //-----------------------------------------
    // Vertical
    //-----------------------------------------
    u16ScaleSrc = pstXC_SetWin_Info->stCapWin.height;
    u16ScaleDst = pstXC_SetWin_Info->stCapWin.height;
    MS_BOOL bForcePreH = FALSE;
    if( (pstXC_SetWin_Info->stDispWin.height*10) < (pstXC_SetWin_Info->stCropWin.height*6) ) // Need scaling down < 0.6
    {
        u16ScaleSrc = pstXC_SetWin_Info->stCapWin.height;
        u16ScaleDst = pstXC_SetWin_Info->stCapWin.height;

        if( (pstXC_SetWin_Info->stDispWin.height*10) < (pstXC_SetWin_Info->stCropWin.height) ) // Need V scaling down because H not enough
        {
            u16ScaleDst = pstXC_SetWin_Info->stCapWin.height/4;
        }
        else if( (pstXC_SetWin_Info->stDispWin.height*11) < (pstXC_SetWin_Info->stCropWin.height*3) )
        {
            u16ScaleDst = pstXC_SetWin_Info->stCapWin.height/2;
        }
    }
    if((pstXC_SetWin_Info->stCapWin.height >= 1200) && (pstXC_SetWin_Info->stDispWin.height < pstXC_SetWin_Info->stCapWin.height))
    {
       u16ScaleDst= pstXC_SetWin_Info->stDispWin.height;
    }
    else
    {
        //u16ScaleDst = pstXC_SetWin_Info->stCapWin.height;// pSrcInfo->stCropWin.width;//disable horizontal prescaling
        if(pstXC_SetWin_Info->stCropWin.height*6 > pstXC_SetWin_Info->stDispWin.height*10)
        {
            //Force H prescling due to fclk BW
            bForcePreH = TRUE;
        }
    }

    if( pstXC_SetWin_Info->bInterlace )
    {
        if (u16ScaleDst % 2 )
        {
            u16ScaleDst = ( u16ScaleDst + 1) & ~0x1;
            if(u16ScaleDst > pstXC_SetWin_Info->stCapWin.height)
            {
                u16ScaleDst -= 2; //Prescaling size must in range of capture window
            }
        }
    }
    if (u16ScaleSrc > u16ScaleDst) //pre scaling only support scaling down
    {
        if (u16ScaleSrc == 0)
        {
            u32Value = 0;
            u32TmpPre = 0;
            stSrcInfo.bPreV_ScalingDown = FALSE;
        }
        else
        {
            u32Value = (MS_U32)u16ScaleDst * 0x80000 / u16ScaleSrc;
            u32TmpPre = V_PreScalingDownRatio(u16ScaleSrc, u16ScaleDst);
            u32TmpPre &= 0xFFFFFFL;
            u32TmpPre |= 0x80000000L;
            stSrcInfo.bPreV_ScalingDown = TRUE;
        }
        HAL_SC_Set_VInitFactorTwo(pInstance, u32Value,eWindow);
        HAL_SC_Set_vsd_output_line_count(pInstance, DISABLE,u16ScaleDst,eWindow);
    }
    else
    {
        u16ScaleDst = u16ScaleSrc;
        u32TmpPre = 0;
        stSrcInfo.bPreV_ScalingDown = FALSE;
        HAL_SC_Set_VInitFactorTwo(pInstance, 0x80000, eWindow);
        HAL_SC_Set_vsd_output_line_count(pInstance, DISABLE,u16ScaleDst,eWindow);
    }
    stDBreg.u32V_PreScalingRatio = u32TmpPre;

    stSrcInfo.Status2.u16PreVCusScalingSrc = u16ScaleSrc;
    stSrcInfo.Status2.u16PreVCusScalingDst = u16ScaleDst;
    stSrcInfo.u16V_SizeAfterPreScaling = u16ScaleDst;
    stSrcInfo.u16V_Length = stSrcInfo.u16V_SizeAfterPreScaling;
    stSrcInfo.u16FRCMV_Length = stSrcInfo.u16V_Length;

    //-----------------------------------------
    // Horizontal
    // HSD rule: Framecount ensure > 3D scaling > customer scaling > QMap HSD sampling > auto fit
    //-----------------------------------------
    //for prevent Src == Dst, HCusScaling = TRUE case
    if(stSrcInfo.Status2.u16PreHCusScalingSrc == stSrcInfo.Status2.u16PreHCusScalingDst)
    {
        stSrcInfo.Status2.bPreHCusScaling = FALSE;
    }
    u16SrcHWidth = pstXC_SetWin_Info->stCapWin.width;

	// If 4k resolution scaling down to less than original height, force H pre-scaling
    if(pstXC_SetWin_Info->stCapWin.height >= 2160 && (pstXC_SetWin_Info->stDispWin.height < pstXC_SetWin_Info->stCapWin.height))
	{
        // Need to force H pre-scaling if already V pre-scaling
        // Force H pre-scaling down to less than 1280px
        if(pstXC_SetWin_Info->stDispWin.width > 1280)
        {
            stSrcInfo.u16H_SizeAfterPreScaling = 1280;
        }
        else
        {
            if(pstXC_SetWin_Info->stCapWin.width > pstXC_SetWin_Info->stDispWin.width)
            {
                stSrcInfo.u16H_SizeAfterPreScaling = pstXC_SetWin_Info->stDispWin.width;
            }
            else
            {
                stSrcInfo.u16H_SizeAfterPreScaling = pstXC_SetWin_Info->stDispWin.width;
            }
        }		
		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
	}
    else
    {
        MS_WINDOW_TYPE stDispWinSub, stDispDEWin;                
        memset(&stDispWinSub, 0, sizeof(MS_WINDOW_TYPE));
        memset(&stDispDEWin, 0, sizeof(MS_WINDOW_TYPE));
        Hal_SC_get_disp_window(pInstance, SUB_WINDOW, &stDispWinSub);
        Hal_SC_get_disp_de_window(pInstance, &stDispDEWin);   
        // PIP with APVR case (Sub window enable and window is outside of disp DE
        // Force prescaling down due to fclk BW. 
         if (MDrv_XC_Is_SubWindowEanble(pInstance) && 
            ((stDispWinSub.x >= stDispDEWin.x + stDispDEWin.width -1) || (stDispWinSub.y >= stDispDEWin.y + stDispDEWin.height - 1)))
        {
    	    if(pstXC_SetWin_Info->stCropWin.width*7 > stSrcInfo.stDispWin.width*10)
    	    {
    	        stSrcInfo.u16H_SizeAfterPreScaling = (stSrcInfo.stDispWin.width + 1) & ~0x01;			
    		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
    	    }
    	    else
    	    {
    	        stSrcInfo.u16H_SizeAfterPreScaling = stSrcInfo.stCapWin.width;// pSrcInfo->stCropWin.width;//disable horizontal prescaling        
    		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
    	    }
        }
        else
        {        
    	    if(pstXC_SetWin_Info->stCropWin.width*6 > stSrcInfo.stDispWin.width*10)
    	    {
    	        stSrcInfo.u16H_SizeAfterPreScaling = (stSrcInfo.stCropWin.width/4 + 1) & ~0x01;			
    		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
    	    }
            else if(pstXC_SetWin_Info->stCropWin.width*7 > stSrcInfo.stDispWin.width*10) 
    	    {
    	        stSrcInfo.u16H_SizeAfterPreScaling = (stSrcInfo.stCropWin.width/2 + 1) & ~0x01;			
    		XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
    	    }
    	    else
    	    {
                if((pstXC_SetWin_Info->stCropWin.width > stSrcInfo.stDispWin.width) && (bForcePreH))
                {
                    // Force prescaling down due to fclk BW.
                    stSrcInfo.u16H_SizeAfterPreScaling = (stSrcInfo.stCropWin.width/2 + 1) & ~0x01;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
                }
                else
                {
                    stSrcInfo.u16H_SizeAfterPreScaling = stSrcInfo.stCapWin.width;// pSrcInfo->stCropWin.width;//disable horizontal prescaling
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS]u16H_SizeAfterPreScaling[%u]\n", stSrcInfo.u16H_SizeAfterPreScaling);
                }
    	    }
        }
    }

    if(stSrcInfo.u16H_SizeAfterPreScaling & BIT(0))
    {
        //YUV422, h size must be even to prevent showing of garbage at the end of line
        if(stSrcInfo.u16H_SizeAfterPreScaling < stSrcInfo.stCapWin.width)
        {
            stSrcInfo.u16H_SizeAfterPreScaling = (stSrcInfo.u16H_SizeAfterPreScaling + 1) & ~0x1;
        }
    }

    if (u16SrcHWidth > stSrcInfo.u16H_SizeAfterPreScaling)
    {
        MS_U16 u16div = 1;
        #ifndef SUPPORT_4K2K_PHOTO_ZOOM
        // To avoid overflow when do H_PreScalingDownRatioAdv/H_PreScalingDownRatioAdv_No_Minus1
        if(u16SrcHWidth > 4096)
        {
            while(1)
            {
                MS_U16 u16ret;

                u16div <<= 1;

                if (u16div == 0)
                {
                    u16div = 1;
                    break;
                }

                u16ret = u16SrcHWidth/u16div;

                if(u16ret == 0)
                {
                    u16div = 1;
                    break;
                }
                else if(u16ret< 4096 && u16ret > stSrcInfo.u16H_SizeAfterPreScaling)
                {
                    break;
                }
            }
        }
        #endif
        if (u16SrcHWidth == 0 )
            u32TmpPre = 0;
        else
            u32TmpPre = (H_PreScalingDownRatio(u16SrcHWidth, stSrcInfo.u16H_SizeAfterPreScaling));
        u32TmpPre &= 0x7FFFFFL;
        u32TmpPre |= 0x80000000L;
    }
    else
    {
        u32TmpPre = 0x100000L;
    }
    stDBreg.u32H_PreScalingRatio = u32TmpPre;
    stDBreg.u16FRCMVLen = stSrcInfo.u16FRCMV_Length;

    //MDrv_SC_set_crop_window(pInstance, &gSrcInfo[eWindow], eWindow );
    if ((stSrcInfo.stCapWin.width > 0) && (stSrcInfo.stCapWin.height > 0))
    {
        stSrcInfo.ScaledCropWin.x     = (MS_U16)((MS_U32)stSrcInfo.stCropWin.x * stSrcInfo.u16H_SizeAfterPreScaling / stSrcInfo.stCapWin.width);
        stSrcInfo.ScaledCropWin.width = (MS_U16)( ( (MS_U32)stSrcInfo.stCropWin.width * stSrcInfo.u16H_SizeAfterPreScaling + ( stSrcInfo.stCapWin.width / 2  ) ) / stSrcInfo.stCapWin.width);
        stSrcInfo.ScaledCropWin.y     = (MS_U16)( (MS_U32) stSrcInfo.stCropWin.y * stSrcInfo.u16V_SizeAfterPreScaling / stSrcInfo.stCapWin.height);
        stSrcInfo.ScaledCropWin.height= (MS_U16)( ( (MS_U32)stSrcInfo.stCropWin.height * stSrcInfo.u16V_SizeAfterPreScaling + ( stSrcInfo.stCapWin.height / 2 ) ) / stSrcInfo.stCapWin.height);
        switch(pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
        {

           case  E_XC_3D_INPUT_TOP_BOTTOM:
                 stSrcInfo.ScaledCropWin.y     = (MS_U16)( (MS_U32) (stSrcInfo.stCropWin.y/2) * stSrcInfo.u16V_SizeAfterPreScaling / stSrcInfo.stCapWin.height);
                 break ;

           case  E_XC_3D_INPUT_SIDE_BY_SIDE_FULL:
           case  E_XC_3D_INPUT_SIDE_BY_SIDE_HALF:
           case  E_XC_3D_INPUT_CHECK_BORAD:
           case  E_XC_3D_INPUT_PIXEL_ALTERNATIVE:
                 stSrcInfo.ScaledCropWin.x     = (MS_U16)((MS_U32) (stSrcInfo.stCropWin.x/2) * stSrcInfo.u16H_SizeAfterPreScaling / stSrcInfo.stCapWin.width);
                 break ;
           case  E_XC_3D_INPUT_FRAME_ALTERNATIVE:
           default :
                 break ;
        }
    }
    // Error handling.
    if (pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        if( stSrcInfo.u16H_SizeAfterPreScaling < (stSrcInfo.ScaledCropWin.width + stSrcInfo.ScaledCropWin.x) )
        {
            if(stSrcInfo.ScaledCropWin.width <= stSrcInfo.u16H_SizeAfterPreScaling)
            {
                stSrcInfo.ScaledCropWin.x = (stSrcInfo.u16H_SizeAfterPreScaling - stSrcInfo.ScaledCropWin.width);
            }
            else
            {
                stSrcInfo.ScaledCropWin.x = 0;
                stSrcInfo.ScaledCropWin.width = stSrcInfo.u16H_SizeAfterPreScaling;
            }
        }
        if( stSrcInfo.u16V_SizeAfterPreScaling < (stSrcInfo.ScaledCropWin.height + stSrcInfo.ScaledCropWin.y) )
        {
            if(stSrcInfo.ScaledCropWin.height <= stSrcInfo.u16V_SizeAfterPreScaling)
            {
                stSrcInfo.ScaledCropWin.y = (stSrcInfo.u16V_SizeAfterPreScaling - stSrcInfo.ScaledCropWin.height) & ~0x1;
            }
            else
            {
                stSrcInfo.ScaledCropWin.height = stSrcInfo.u16V_SizeAfterPreScaling;
            }
        }
    }
    stSrcInfo.u16FRCMV_Length = stSrcInfo.ScaledCropWin.height;
    stDBreg.u16FRCMVLen = stSrcInfo.u16FRCMV_Length;

    if(IsVMirrorMode(eWindow))
    {
        if (stSrcInfo.ScaledCropWin.y%2)
        {
            if (stSrcInfo.ScaledCropWin.height%2)
            {
                stDBreg.bopm_lcnt_inv = TRUE;
            }
            else
            {
                stDBreg.bopm_lcnt_inv = FALSE;
            }
        }
        else
        {
            if (stSrcInfo.ScaledCropWin.height%2)
            {
                stDBreg.bopm_lcnt_inv = FALSE;
            }
            else
            {
                stDBreg.bopm_lcnt_inv = TRUE;
            }
        }
    }
    else
    {
        if(stSrcInfo.ScaledCropWin.y%2)
        {
            stDBreg.bopm_lcnt_inv = TRUE;
        }
        else
        {
            stDBreg.bopm_lcnt_inv = FALSE;
        }
    }

    if (stDBreg.b3Dstate == 1)
    {
        //MDrv_SC_3D_Adjust_PstScaling
        _MDrv_SC_ds_3D_Adjust_PstScaling(pInstance, pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW], pstXC_SetWin_Info->eOutput3DMode, &stSrcInfo, eWindow);
    }

    //MDrv_SC_set_postscaling_ratio(pInstance, &gSrcInfo[eWindow], eWindow );
    //-----------------------------------------
    // Horizontal
    //-----------------------------------------
    if (stSrcInfo.bHCusScaling == FALSE)
    {
        u16ScaleSrc = stSrcInfo.ScaledCropWin.width;
        u16ScaleDst = stSrcInfo.stDispWin.width;
    }
    else
    {
        u16ScaleSrc = stSrcInfo.u16HCusScalingSrc;
        u16ScaleDst = stSrcInfo.u16HCusScalingDst;
        //make sure crop window in the range of window after prescaling
        if((u16ScaleSrc > stSrcInfo.u16H_SizeAfterPreScaling) ||
           (u16ScaleSrc + stSrcInfo.ScaledCropWin.x > stSrcInfo.u16H_SizeAfterPreScaling))
        {
            MS_U16 u16PstScaledWidth;
            u16PstScaledWidth = (MS_U32)(u16ScaleDst*(stSrcInfo.u16H_SizeAfterPreScaling - stSrcInfo.ScaledCropWin.x))/u16ScaleSrc;
            //Check if the scaled content can full fill the display window, otherwise garbage will show out.
            if(u16PstScaledWidth < stSrcInfo.stDispWin.width)
            {
                if(u16ScaleSrc > stSrcInfo.u16H_SizeAfterPreScaling)
                {
                    u16ScaleSrc = stSrcInfo.u16H_SizeAfterPreScaling & ~0x01;
                    stSrcInfo.ScaledCropWin.x = 0;
                }
                else
                {
                    stSrcInfo.ScaledCropWin.x = (stSrcInfo.u16H_SizeAfterPreScaling - u16ScaleSrc) & ~0x01;
                }
            }
        }
    }
    //store pst scaling info here
    stSrcInfo.u16HCusScalingSrc = u16ScaleSrc;
    stSrcInfo.u16HCusScalingDst = u16ScaleDst;
    if (u16ScaleDst != u16ScaleSrc)
    {
        u32TmpPost = H_PostScalingRatio_adv(u16ScaleSrc, u16ScaleDst);
        u32TmpPost &= 0xFFFFFFL;
        u32TmpPost |= 0x1000000L;
    }
    else
    {
        u32TmpPost = 0x100000L;
    }
    stDBreg.u32H_PostScalingRatio = u32TmpPost;

    //-----------------------------------------
    // Vertical
    //-----------------------------------------
    if (stSrcInfo.bVCusScaling == FALSE)
    {
        u16ScaleSrc = stSrcInfo.ScaledCropWin.height;
        u16ScaleDst = stSrcInfo.stDispWin.height;
    }
    else
    {
        u16ScaleSrc = stSrcInfo.u16VCusScalingSrc;
        u16ScaleDst = stSrcInfo.u16VCusScalingDst;
        //make sure crop window in the range of window after prescaling
        if((u16ScaleSrc > stSrcInfo.u16V_SizeAfterPreScaling) ||
           (u16ScaleSrc + stSrcInfo.ScaledCropWin.y > stSrcInfo.u16V_SizeAfterPreScaling))
        {
            MS_U16 u16PstScaledHeight;
            u16PstScaledHeight = (MS_U32)(u16ScaleDst*(stSrcInfo.u16V_SizeAfterPreScaling - stSrcInfo.ScaledCropWin.y))/u16ScaleSrc;
            //Check if the scaled content can full fill the display window, otherwise garbage will show out.
            if(u16PstScaledHeight < stSrcInfo.stDispWin.height)
            {
                if(u16ScaleSrc > stSrcInfo.u16V_SizeAfterPreScaling)
                {
                    u16ScaleSrc = stSrcInfo.u16V_SizeAfterPreScaling & ~0x01;
                    stSrcInfo.ScaledCropWin.y = 0;
                }
                else
                {
                    stSrcInfo.ScaledCropWin.y = (stSrcInfo.u16V_SizeAfterPreScaling - u16ScaleSrc) & ~0x01;
                }
            }
        }
    }
    //store pst scaling info here
    stSrcInfo.u16VCusScalingSrc = u16ScaleSrc;
    stSrcInfo.u16VCusScalingDst = u16ScaleDst;
    if (u16ScaleSrc == u16ScaleDst)
    {
        u32TmpPost = 0x100000L;
    }
    else
    {
        if ((u16ScaleDst-1) == 0)
        {
            u32TmpPost = 0;
        }
        else
        {
            u32TmpPost = V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
	    }
    }
    u32TmpPost &= 0xFFFFFFL;    //scaling factor support 24bits
    if (u32TmpPost != 0x100000L)
    {
        u32TmpPost |= 0x1000000L;
    }
    stSrcInfo.u16V_Length = u16ScaleSrc;
    stDBreg.u32V_PostScalingRatio = u32TmpPost;

    //MDrv_SC_set_frcm_fetch_number_limit(pInstance, &gSrcInfo[eWindow], eWindow );
    // MDrv_SC_set_ipmsetting(pInstance, pSrcInfo, eWindow);
    stSrcInfo.u16IPMFetch = stSrcInfo.u16H_SizeAfterPreScaling;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS] stSrcInfo.u16IPMFetch[%d]  \n", stSrcInfo.u16IPMFetch);
    if (stSrcInfo.bMemFmt422)
    {
        stSrcInfo.u16IPMFetch = (stSrcInfo.u16IPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
    }	
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[DS] stSrcInfo.u16IPMFetch[%d]  \n", stSrcInfo.u16IPMFetch);
    if (stSrcInfo.u16IPMFetch > MST_LINE_BFF_MAX)
    {
        stSrcInfo.u16IPMFetch = MST_LINE_BFF_MAX;
    }
#if (HW_DESIGN_4K2K_VER == 4)
    if (stSrcInfo.bInterlace)
    {
        MDrv_SC_set_ds_dnrsetting(pInstance, &stSrcInfo, TRUE, &u32OneLineOffset, FALSE, eWindow, &stDBreg, pstXC_SetWin_Info);
        MDrv_SC_set_ds_opmsetting(pInstance, &stSrcInfo, FALSE, FALSE, u32OneLineOffset, eWindow, &stDBreg, pstXC_SetWin_Info);
        MDrv_SC_set_ds_frcmwsetting(pInstance, &stSrcInfo, &u32OneLineOffset, eWindow, &stDBreg, pstXC_SetWin_Info);
        MDrv_SC_set_ds_frcmrsetting(pInstance, &stSrcInfo, u32OneLineOffset, eWindow, &stDBreg, pstXC_SetWin_Info);
    }
    else
    {
       	 	MDrv_SC_set_ds_dnrsetting(pInstance, &stSrcInfo, FALSE, &u32OneLineOffset, TRUE, eWindow, &stDBreg, pstXC_SetWin_Info);
		MDrv_SC_set_ds_frcmrsetting(pInstance, &stSrcInfo, u32OneLineOffset, eWindow, &stDBreg, pstXC_SetWin_Info);
		}
#else
       	 	MDrv_SC_set_ds_dnrsetting(pInstance, &stSrcInfo, TRUE, &u32OneLineOffset, TRUE, eWindow, &stDBreg, pstXC_SetWin_Info);
            MDrv_SC_set_ds_opmsetting(pInstance, &stSrcInfo, FALSE, FALSE, u32OneLineOffset, eWindow, &stDBreg, pstXC_SetWin_Info);
#endif

#ifndef DISABLE_3D_FUNCTION
    //MDrv_SC_3D_Adjust_FetchOffset(pInstance, pSrcInfo, eWindow);
    if(E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == pstXC_SetWin_Info->eInput3DMode[MAIN_WINDOW])
    {
        stDBreg.u16OPMOffset = stDBreg.u16DNROffset/2;
    }
#endif


    // FIXME: direct use variable instead of stDBreg?
    //MDrv_SC_set_display_window(pInstance,  eWindow );
    stDBreg.u16H_DisStart = stSrcInfo.stDispWin.x;
    if ( stSrcInfo.stDispWin.width == 0 )
    {
        stDBreg.u16H_DisEnd = stSrcInfo.stDispWin.x;
    }
    else
    {
        stDBreg.u16H_DisEnd = stSrcInfo.stDispWin.x +  stSrcInfo.stDispWin.width - 1;
    }
    stDBreg.u16V_DisStart = stSrcInfo.stDispWin.y;
    if ( stSrcInfo.stDispWin.height == 0 )
    {
        stDBreg.u16V_DisEnd = stSrcInfo.stDispWin.y;
    }
    else
    {
        stDBreg.u16V_DisEnd = stSrcInfo.stDispWin.y +  stSrcInfo.stDispWin.height - 1;
    }


    //MDrv_SC_sw_db(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, eWindow );
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "bPreV_ScalingDown[%d]  \n", stSrcInfo.bPreV_ScalingDown);
    //if it's playing enabling DS, using fclk causes purple stripes showing upon video.	
    Hal_SC_set_ficlk(pInstance, TRUE , eWindow );
    Hal_SC_ds_db(pInstance, &stDBreg, eWindow, &stSrcInfo, pstXC_SetWin_Info);

#ifndef DISABLE_3D_FUNCTION
    //MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[eWindow], eWindow);
#endif

    return bRet;
}

MS_BOOL MApi_XC_SetDynamicScalingWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    // only support main window
    if (eWindow >= SUB_WINDOW)
    {
        return FALSE;
    }
    stXC_SET_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMICSCALING_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the specific window
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen         \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    MS_BOOL bEnable8LBMode = FALSE;
    XC_INITMISC stXC_Init_Misc;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _SETWINDOW_ENTRY();

    memset(&stXC_Init_Misc,0,sizeof(XC_INITMISC));

    if (_MApi_XC_SetWindow(pInstance, pstXC_SetWin_Info, sizeof(XC_SETWIN_INFO), FALSE, eWindow) == FALSE)
    {
        goto SETWINDOW_EXIT;
    }

#ifndef DISABLE_3D_FUNCTION

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[eWindow], eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[eWindow], eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
    bRet = TRUE;

    //all return should come here, because this code will enable opm write limit addr protect
SETWINDOW_EXIT:

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(eWindow == MAIN_WINDOW)
    {
#if 0 // RD suggest move to init
        MS_U32 u32OPWLimitBase = 0x00;
        u32OPWLimitBase = ((MDrv_XC_GetIPMBase(pInstance, 0, eWindow)*BYTE_PER_WORD + MDrv_XC_GetDNRBufSize(pInstance, eWindow)) / BYTE_PER_WORD - 1) ;

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
        MDrv_SC_set_opm_write_limit(pInstance, TRUE, 0 , u32OPWLimitBase , eWindow);
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
        MDrv_SC_set_opm_write_limit(pInstance, TRUE, 0 , u32OPWLimitBase , eWindow);
        _XC_RETURN();
#endif //#if SC2_ENABLE


#endif
    }

//there is xc performance issue for napoli hw, so need enable 8 LB mode by designer's advice
#if ENABLE_DI_8LB_MODE
    bEnable8LBMode = ((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_MODE_NONE)
                      && (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE)
                      && MDrv_XC_Is2K2KToFrc(pInstance)
                      && (((gSrcInfo[MAIN_WINDOW].stCapWin.width == DOUBLEHD_1080X2P_HSIZE)
                           && (gSrcInfo[MAIN_WINDOW].stCapWin.height > (DOUBLEHD_1080X2P_VSIZE - DOUBLEHD_1080X2P_GARBAGE_VSIZE - 10))
                           && (!IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))) // MM case, enable UC costs too much BW, make video temp garbage, so not enable UC for MM
                          || (MApi_XC_Is_SubWindowEanble()
                              && ((MApi_XC_R2BYTE(REG_SC_BK0F_09_L) == 1080) && (MApi_XC_R2BYTE(REG_SC_BK0F_0A_L) == 2159)))));
#endif

    pXCResourcePrivate->btvservice = 0;
    pXCResourcePrivate->bsetwindow_multiprocess = FALSE;
    gSrcInfo[eWindow].bpqmap     = 0;
    gSrcInfo[eWindow].btimingchg = 0;
    gSrcInfo[eWindow].bxcoutput  = 0;
    gSrcInfo[eWindow].bmemfmt    = 0;
    gSrcInfo[eWindow].bcropwindowchg    = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW," bsetwindow_multiprocess = 0\n");


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    MApi_XC_GetMISCStatus(&stXC_Init_Misc);
    // monaco bringup temp close ucnr
    if (!(stXC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SKIP_UC_DOWNLOAD))
    {
#ifdef ENABLE_TV_SC2_PQ
        if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
            {
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow,
                        E_PQ_IOCTL_SET_UCFEATURE,
                        (void *)&bEnable8LBMode,
                        sizeof(MS_BOOL));
            }
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex)
            {
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                        E_PQ_IOCTL_SET_UCFEATURE,
                        (void *)&bEnable8LBMode,
                        sizeof(MS_BOOL));
            }
        }
#endif
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= SetWindow Done (Window : %u, src: %u) =============\n",eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    _SETWINDOW_RETURN();

    return bRet;
}

MS_BOOL MApi_XC_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#ifndef DISABLE_3D_FUNCTION
static MS_BOOL _MDrv_XC_3D_LoadReg(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                   E_XC_3D_OUTPUT_MODE eOutputMode,
                                   XC_InternalStatus *pSrcInfo,
                                   SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (eWindow == MAIN_WINDOW)
    {
        memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, &pXCResourcePrivate->stdrvXC_MVideo_Context.stMainDBreg, sizeof(SC_SWDB_INFO));
    }
    else
    {
        memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg, &pXCResourcePrivate->stdrvXC_MVideo_Context.stSubDBreg, sizeof(SC_SWDB_INFO));
    }
    return MDrv_XC_3D_LoadReg(pInstance, eInputMode, eOutputMode, pSrcInfo, eWindow);

}
#endif

MS_BOOL MApi_XC_SetDualWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info_Main, XC_SETWIN_INFO *pstXC_SetWin_Info_Sub)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _SETWINDOW_ENTRY();

    if (_MApi_XC_SetWindow(pInstance, pstXC_SetWin_Info_Main, sizeof(XC_SETWIN_INFO), TRUE, MAIN_WINDOW) == FALSE)
    {
        goto SETWINDOW_EXIT;
    }

    if (_MApi_XC_SetWindow(pInstance, pstXC_SetWin_Info_Sub, sizeof(XC_SETWIN_INFO), TRUE, SUB_WINDOW) == FALSE)
    {
        goto SETWINDOW_EXIT;
    }
#ifndef DISABLE_3D_FUNCTION

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[MAIN_WINDOW], MAIN_WINDOW);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _MDrv_XC_3D_LoadReg(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW), MDrv_XC_Get_3D_Output_Mode(pInstance), &gSrcInfo[MAIN_WINDOW], MAIN_WINDOW);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
    bRet = TRUE;
    _SETWINDOW_RETURN();

    //all return should come here, because this code will enable opm write limit addr protect
SETWINDOW_EXIT:
    {
#if 0 // RD suggest move to init
        MS_U32 u32OPWLimitBase = 0x00;

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        u32OPWLimitBase = ((MDrv_XC_GetIPMBase(pInstance, 0, MAIN_WINDOW)*BYTE_PER_WORD + MDrv_XC_GetDNRBufSize(pInstance, MAIN_WINDOW)) / BYTE_PER_WORD - 1) ;
        _XC_ENTRY(pInstance);
        MDrv_SC_set_opm_write_limit(pInstance, TRUE, 0 , u32OPWLimitBase , MAIN_WINDOW);
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        u32OPWLimitBase = ((MDrv_XC_GetIPMBase(pInstance, 0, MAIN_WINDOW)*BYTE_PER_WORD + MDrv_XC_GetDNRBufSize(pInstance, MAIN_WINDOW)) / BYTE_PER_WORD - 1) ;
        _XC_ENTRY();
        MDrv_SC_set_opm_write_limit(pInstance, TRUE, 0 , u32OPWLimitBase , MAIN_WINDOW);
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#endif
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= Set Dual Window Done =============\n");
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    if (bRet == FALSE)
    {
        _SETWINDOW_RETURN();
    }
    return bRet;
}

MS_BOOL MApi_XC_SetDualWindow(XC_SETWIN_INFO *pstXC_SetWin_Info_Main, XC_SETWIN_INFO *pstXC_SetWin_Info_Sub)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_DUALWINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info_Main = pstXC_SetWin_Info_Main;
    XCArgs.pstXC_SetWin_Info_Sub = pstXC_SetWin_Info_Sub;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DUALWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#if FRC_INSIDE
void Mapi_XC_FRC_Set_User_MemoryFMT(void *pInstance, MS_U8 u8Mode, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo.bEnableUserMode = bEnable;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = u8Mode;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

MS_U8 Mapi_XC_FRC_Get_MemoryFMT(void *pInstance) //NO_USE
{
    MS_U8 u8Val = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    u8Val = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return u8Val;
}

void Mapi_XC_FRC_Set_MemoryFMT(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                               E_XC_3D_OUTPUT_MODE e3dOutputMode,
                               E_XC_3D_PANEL_TYPE e3dPanelType)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo.bEnableUserMode == FALSE)
    {
        // RGB and DVI
        if(0) //(IsSrcTypeVga(enInputSourceType) ||
            //(IsSrcTypeHDMI(enInputSourceType)&(!g_HdmiPollingStatus.bIsHDMIMode)))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "### is RGB Space\n");
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_RGB_10_SPECIAL;
        }
        else
        {
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            {
                if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### E_XC_3D_OUTPUT_FRAME_ALTERNATIVE\n");
                    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080)||
                            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1280x720))
                    {
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### is YUV Space, 960x1080\n");
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_YUV_10_SPECIAL;
                    }
                    //else if((g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x1080)||
                    //   (g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x540))
                    else
                    {
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### is YC Space, 1920x1080/1920x540\n");
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_YC_10_SPECIAL;
                    }
                }
                else if(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF\n");
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### is YUV Space, SBS 960x1080\n");
                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_YUV_10_SPECIAL;
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### is YC Space, others\n");
                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_YC_10_SPECIAL;
                }
            }
            else
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode = FRC_IP_MEM_IP_YC_10_SPECIAL;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"### is YC Space, bypass\n");
            }
        }
    }
    else
    {
        printf("\n\n@@Use User define for debug\n\n");
    }
#if 1
    if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            &&(e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
            &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1280x720))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16PanelWidth = 1280;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16PanelHeigh= 720;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16PanelWidth = 1920;
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16PanelHeigh = 540;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16PanelHeigh = 1080;
        }
    }
#endif
    MDrv_FRC_SetMemFormat(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo);

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080)&&
                (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u16FB_YcountLinePitch /= 2;
        }
        MDrv_FRC_OPM_SetBaseOfset(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo, e3dOutputMode);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

//-------------------------------------------------------------------------------------------------
/// set 3D convert format type
/// @param  e3dInputMode                \b IN: the input format
/// @param  e3dOutputMode               \b IN: the format we want to convert to
/// @param  e3dPanelType                \b IN: the panel type.
//-------------------------------------------------------------------------------------------------
void MApi_XC_FRC_Set_3D_Mode(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                             E_XC_3D_OUTPUT_MODE e3dOutputMode,
                             E_XC_3D_PANEL_TYPE e3dPanelType)
{


    MS_U8 u8FRC_Pnl_type = E_FRC_PNL_TYPE_PASSIVE;
    MS_U8 u83D_FI_out = E_XC_3D_OUTPUT_FI_MODE_NONE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (e3dPanelType == E_XC_3D_PANEL_SHUTTER)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            u8FRC_Pnl_type = E_FRC_PANEL_3D_TYPE_ACTIVE_240_1920_540;
        }
        else
        {
            u8FRC_Pnl_type = E_FRC_PNL_TYPE_ACTIVE;
        }
    }
    else if(e3dPanelType == E_XC_3D_PANEL_PELLICLE)
    {
        u8FRC_Pnl_type = E_FRC_PNL_TYPE_PASSIVE;
    }
    else
    {
        u8FRC_Pnl_type = E_FRC_PNL_TYPE_2D;
    }

    u83D_FI_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out;

    switch(e3dInputMode)
    {
        case E_XC_3D_INPUT_MODE_NONE:
        case E_XC_3D_INPUT_NORMAL_2D_HW:
            MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, e3dInputMode, e3dOutputMode, u83D_FI_out);
            break;

        default:
        case E_XC_3D_INPUT_FRAME_PACKING:
            if(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_FRAME_PACKING, E_XC_3D_OUTPUT_TOP_BOTTOM, u83D_FI_out);
            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_FRAME_PACKING, E_XC_3D_OUTPUT_FRAME_ALTERNATIVE, u83D_FI_out);
            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_FRAME_PACKING, E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF, u83D_FI_out);
            }
            else if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_MODE_NONE, E_XC_3D_OUTPUT_MODE_NONE, E_XC_3D_OUTPUT_FI_1920x1080);
            }
            else
            {
                printf("\n##[%s]1. Please check the 3D input and out mode\n\n", __FUNCTION__);
            }
            break;
        case E_XC_3D_INPUT_SIDE_BY_SIDE_HALF:
            if(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_SIDE_BY_SIDE_HALF, E_XC_3D_OUTPUT_TOP_BOTTOM, u83D_FI_out);

            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_SIDE_BY_SIDE_HALF, E_XC_3D_OUTPUT_FRAME_ALTERNATIVE, u83D_FI_out);

            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_SIDE_BY_SIDE_HALF, E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF, u83D_FI_out);
            }
            else if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_MODE_NONE, E_XC_3D_OUTPUT_MODE_NONE, E_XC_3D_OUTPUT_FI_1920x1080);
            }
            else
            {
                printf("\n##[%s]2. Please check the 3D input and out mode\n\n", __FUNCTION__);
            }

            break;
        case E_XC_3D_INPUT_SIDE_BY_SIDE_FULL:
            if(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_SIDE_BY_SIDE_FULL, E_XC_3D_OUTPUT_TOP_BOTTOM, u83D_FI_out);

            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_SIDE_BY_SIDE_FULL, E_XC_3D_OUTPUT_FRAME_ALTERNATIVE, u83D_FI_out);
            }
            else if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_MODE_NONE, E_XC_3D_OUTPUT_MODE_NONE, E_XC_3D_OUTPUT_FI_1920x1080);
            }
            else
            {
                printf("\n##[%s]2. Please check the 3D input and out mode\n\n", __FUNCTION__);
            }
            break;
        case E_XC_3D_INPUT_TOP_BOTTOM:
            if(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_TOP_BOTTOM, E_XC_3D_OUTPUT_TOP_BOTTOM, u83D_FI_out);
            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_TOP_BOTTOM, E_XC_3D_OUTPUT_FRAME_ALTERNATIVE, u83D_FI_out);

            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_OUTPUT_TOP_BOTTOM, E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF, u83D_FI_out);

            }
            else if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_MODE_NONE, E_XC_3D_OUTPUT_MODE_NONE, E_XC_3D_OUTPUT_FI_1920x1080);
            }
            else
            {
                printf("\n##[%s]3. Please check the 3D input and out mode\n\n", __FUNCTION__);
            }
            break;
        case E_XC_3D_INPUT_FRAME_ALTERNATIVE:
            if(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_FRAME_ALTERNATIVE, E_XC_3D_OUTPUT_TOP_BOTTOM, u83D_FI_out);
            }
            else if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_FRAME_ALTERNATIVE, E_XC_3D_OUTPUT_FRAME_ALTERNATIVE, u83D_FI_out);
            }
            else if((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
            {
                MDrv_FRC_Set_3D_QMap(pInstance, u8FRC_Pnl_type, E_XC_3D_INPUT_MODE_NONE, E_XC_3D_OUTPUT_MODE_NONE, E_XC_3D_OUTPUT_FI_1920x1080);
            }
            else
            {
                printf("\n##[%s]1. Please check the 3D input and out mode\n\n", __FUNCTION__);
            }
            break;


    }

    switch(e3dOutputMode)
    {
        //for 2D to 3D use

        // ParaCnt=4, P0 = 2D to 3D ConversionMode(0=Disable, 1=Enable)
        //                  P1 = 3D Depth Gain(0~31)
        //                  P2 = 3D Depth Offset(0~127)
        //                  P3 = Artificial Gain(0~15)

        case E_XC_3D_OUTPUT_CHECKBOARD_HW:
        case E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW:
        case E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW:
        case E_XC_3D_OUTPUT_FRAME_L_HW:
        case E_XC_3D_OUTPUT_FRAME_R_HW:
            if(!Hal_XC_FRC_R2_Set_2DTo3DMode(1, 0x13, 0x69, 0x0E))
            {
                printf("Enable 2DTo3D Failed\n");
            }
            break;

        default:
            if(!Hal_XC_FRC_R2_Set_2DTo3DMode(0, 0x13, 0x69, 0x8))
            {
                printf("Disable 2DTo3D Failed\n");
            }
            break;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}
#endif
extern MS_U16 _MDrv_SC_GetOutputVPeriod(void *pInstance);
MS_BOOL MApi_XC_FRC_BypassMFC_U2(void* pInstance, MS_BOOL bEnable)
{
#if FRC_INSIDE
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MS_U8 u8Mode = FRC_IP_MEM_IP_RGB_10_SPECIAL;
    E_XC_3D_INPUT_MODE e3dInputMode = E_XC_3D_INPUT_MODE_NONE;
    E_XC_3D_OUTPUT_MODE e3dOutputMode = E_XC_3D_OUTPUT_MODE_NONE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    E_XC_3D_PANEL_TYPE e3dPanelType = MDrv_XC_Get_3D_Panel_Type(pInstance);
    MS_U16 u16DelayCount = 0;

    FRC_R2_CMD_PARAMETER_t tFRC_R2_Para;

    memset(&tFRC_R2_Para, 0, sizeof(FRC_R2_CMD_PARAMETER_t));

    ///Current spec doesn't have bypass MFC in 3D mode, this is only for PC RGB bypass mode
    if((e3dInputMode != MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW))||(e3dOutputMode != MDrv_XC_Get_3D_Output_Mode(pInstance)))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return FALSE;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    // for Agate, Freeze memory
    W2BYTEMSK(L_FRC_SC134(0x07), BIT(0), BIT(0));

    if(bEnable)
    {
        printf("@@@ MFC Bypass Enable\n");
        printf("@@@1,Set MFC CMD \n");

        tFRC_R2_Para.p1=0x0F;
        tFRC_R2_Para.p2=0;
        tFRC_R2_Para.p3=0;

        MDrv_XC_SendCmdToFRC(pInstance, 0x20, 3, tFRC_R2_Para );
        MDrv_Write2ByteMask(0x3020C0, 0x00, BIT(0));  // IPM CSC oFF
        // for Agate
        MDrv_Write2ByteMask(0x302660, 0x00, BIT(8));  // data flow don't go through the MFC section
        MDrv_Write2ByteMask(0x302C80, 0x00, BIT(0)); // MFC Stage disable
        MDrv_Write2ByteMask(0x30290E, 0x00, 0xFFFF); // MFC disable
        W2BYTEMSK(L_FRC_SC134(0x07), BIT(1), BIT(1));
        //Disable 6R mode
        W2BYTEMSK(L_FRC_SC134(0x19), 0x00, BIT(8));


        MDrv_Write2ByteMask(0x303002, 0x00, BIT(3));   // OP2 CSC oFF
        u16DelayCount = 7;
        u8Mode = FRC_IP_MEM_IP_RGB_10_SPECIAL;
    }
    else
    {
        printf("@@@ MFC Bypass Disable\n");
        printf("@@@1,Set MFC CMD \n");

        tFRC_R2_Para.p1=0x02;
        tFRC_R2_Para.p2=0;
        tFRC_R2_Para.p3=0;

        MDrv_XC_SendCmdToFRC(pInstance, 0x20, 3, tFRC_R2_Para );

        MDrv_Write2ByteMask(0x3020C0, BIT(0), BIT(0));
        // for Agate
        MDrv_Write2ByteMask(0x302660, BIT(8), BIT(8));
        MDrv_Write2ByteMask(0x302C80, BIT(0), BIT(0));
        MDrv_Write2ByteMask(0x30290E, 0x1D2F, 0xFFFF);
        W2BYTEMSK(L_FRC_SC134(0x07), 0, BIT(1));
        //Enable 6R mode
        W2BYTEMSK(L_FRC_SC134(0x19), BIT(8), BIT(8));

        MDrv_Write2ByteMask(0x303002, BIT(3), BIT(3));
        u16DelayCount = 10;
        u8Mode = FRC_IP_MEM_IP_YUV_10_SPECIAL;
    }
    printf("@@@2,Load MFC table\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    Mapi_XC_FRC_Set_User_MemoryFMT(pInstance, u8Mode, bEnable);
    MDrv_XC_FRC_SetWindow(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    Mapi_XC_FRC_Set_User_MemoryFMT(pInstance, u8Mode, bEnable);
    MDrv_XC_FRC_SetWindow(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    // for Agate, need to wait the memory write finish
    u16DelayCount *= 20;//_MDrv_SC_GetOutputVPeriod(); //get delay time
    printf("Enable Delay=%u\n", u16DelayCount);
    MsOS_DelayTask(u16DelayCount);

    // for Agate, un Freeze memory
    W2BYTEMSK(L_FRC_SC134(0x07), 0x00, BIT(0));

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return TRUE;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return FALSE;
#endif

}

MS_BOOL MApi_XC_FRC_BypassMFC(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_FRC_BYPASS XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FRC_BYPASS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_FRC_Mute_Skip_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate ->bSkipFRC = bEnable;
}

void MApi_XC_FRC_Mute_Skip_Enable(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_FRC_MUTE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FRC_MUTE_SKIP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_FRC_Mute_U2(void* pInstance, MS_BOOL bEnable)
{
    MDrv_FRC_Mute(pInstance, bEnable);
}

void MApi_XC_FRC_Mute(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_FRC_MUTE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FRC_MUTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_FRC_SetWindow(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                           E_XC_3D_OUTPUT_MODE e3dOutputMode,
                           E_XC_3D_PANEL_TYPE e3dPanelType)
{
#if FRC_INSIDE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal> 3000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal> 2000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500) &&  // 4K2K panel
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ) &&
            (E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)))
    {
        // For FRC inside chip, ex.Einstein/Napoli
        // 4K1K FA output case, FRC only can handle TB in to FA out
        // FRC input is TB, output is FA
        e3dInputMode = E_XC_3D_INPUT_TOP_BOTTOM;
        e3dOutputMode = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE;
    }

    Mapi_XC_FRC_Set_MemoryFMT(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    MApi_XC_FRC_Set_3D_Mode(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 3840)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 2160) )
    {
        printf("@@@ %s: 4k2k patch\n", __FUNCTION__);
        //FRC IPM/OPM
        MDrv_FRC_IPM_SetOffset(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
        MDrv_FRC_IPM_SetFetchNum(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
        MDrv_FRC_OPM_SetOffset(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
        MDrv_FRC_OPM_SetFetchNum(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
        //FRC OP HSU/VSU
        MDrv_FRC_HSU_SetScalingSize(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width,
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
        MDrv_FRC_VSU_SetScalingSize(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height,
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);
        //force RGB 444
        MDrv_FRC_CSC_SelectPath(pInstance, E_FRC_COLOR_PATH_RGB_444);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
#endif
}

void MApi_XC_FRC_SetWindow_U2(void* pInstance,
                              E_XC_3D_INPUT_MODE e3dInputMode,
                              E_XC_3D_OUTPUT_MODE e3dOutputMode,
                              E_XC_3D_PANEL_TYPE e3dPanelType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_FRC_SetWindow(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_XC_FRC_SetWindow(pInstance, e3dInputMode, e3dOutputMode, e3dPanelType);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_FRC_SetWindow(E_XC_3D_INPUT_MODE e3dInputMode,
                           E_XC_3D_OUTPUT_MODE e3dOutputMode,
                           E_XC_3D_PANEL_TYPE e3dPanelType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_WINDOW XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.e3dPanelType = e3dPanelType;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set report window
/// @param  bEnable        \b IN: enable or disable report window
/// @param  Window         \b IN: the window position and size
/// @param  u8Color        \b IN: report window's color
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetRepWindow_U2(void* pInstance, MS_BOOL bEnable,MS_WINDOW_TYPE Window,MS_U8 u8Color)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    MDrv_SC_rep_window(pInstance,bEnable,Window,u8Color);
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_RepWindow, &Window, sizeof(MS_WINDOW_TYPE));
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
    MDrv_SC_rep_window(pInstance,bEnable,Window,u8Color);
    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_RepWindow, &Window, sizeof(MS_WINDOW_TYPE));
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_SetRepWindow(MS_BOOL bEnable,MS_WINDOW_TYPE Window,MS_U8 u8Color)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_REPORT_WINDOW XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.Window = Window;
    XCArgs.u8Color = u8Color;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_REPORT_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// update display window registers with input window
/// @param  eWindow                          \b IN: Main or sub window
/// @param  pdspwin                          \b IN: window info that will be setted to registers
//-------------------------------------------------------------------------------------------------
void    MApi_XC_SetDispWinToReg_U2(void* pInstance, MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    MDrv_XC_set_dispwin_to_reg(pInstance, eWindow, pstDspwin);
    memcpy(&gSrcInfo[eWindow].stDispWin, pstDspwin, sizeof(MS_WINDOW_TYPE));
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
    MDrv_XC_set_dispwin_to_reg(pInstance, eWindow, pstDspwin);
    memcpy(&gSrcInfo[eWindow].stDispWin, pstDspwin, sizeof(MS_WINDOW_TYPE));
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void    MApi_XC_SetDispWinToReg(MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_DISPLAY_WIN_TO_REG XCArgs;
    XCArgs.pstDspwin = pstDspwin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DISPLAY_WIN_TO_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// get current display window registers setting
/// @param  eWindow                          \b IN : Main or sub window
/// @param  pdspwin                          \b OUT: Pointer for ouput disp window register
//-------------------------------------------------------------------------------------------------
void    MApi_XC_GetDispWinFromReg_U2(void* pInstance, MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, pstDspwin);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, pstDspwin);
    _XC_RETURN();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void    MApi_XC_GetDispWinFromReg(MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_DISPLAY_WIN_FROM_REG XCArgs;
    XCArgs.pstDspwin = pstDspwin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DISPLAY_WIN_FROM_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/// MUX API
///////////////////////////////////////////////////

/**
 * InputSource Mux is a controller which handles all of the video path.
 * Before using video source, user have to create a data path between source and sink.
 * For example, DTV -> MainWindow or DTV -> SubWindow.
 */

/////////////////////////////////////////
// Configuration
/////////////////////////////////////////


/********************************************************************************/
/*                 Static Variable and Functions (Private)                      */
/********************************************************************************/

/********************************************************************************/
/*                 Functions                                                    */
/********************************************************************************/

//////////////////////////////////////
//    Data Query Functions
//////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/// Get input port from input source type
/// @param  src                 \b IN: the query based on the input source type
/// @param  port_ids            \b OUT: the port ID we get
/// @param  u8port_count        \b OUT: how many ports we get
//-------------------------------------------------------------------------------------------------
void MApi_XC_Mux_GetPort(void *pInstance, INPUT_SOURCE_TYPE_t src, E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.input_source_to_input_port != NULL )
    {
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.input_source_to_input_port(src,port_ids,u8port_count);
    }
    else
    {
        *u8port_count = 0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get input port from input HDMI source type
/// @param  src                 \b IN: the query based on the input HDMI type
/// @return @ref E_MUX_INPUTPORT
//-------------------------------------------------------------------------------------------------
E_MUX_INPUTPORT MApi_XC_Mux_GetHDMIPort_U2( void* pInstance, INPUT_SOURCE_TYPE_t src )
{
    E_MUX_INPUTPORT ePort = INPUT_PORT_NONE_PORT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    ePort = MDrv_XC_Mux_GetHDMIPort(pInstance, src);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    ePort = MDrv_XC_Mux_GetHDMIPort(pInstance, src);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return ePort;
}

E_MUX_INPUTPORT MApi_XC_Mux_GetHDMIPort( INPUT_SOURCE_TYPE_t src )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return INPUT_PORT_NONE_PORT;
    }

    stXC_GET_MUX_HDMIPORT XCArgs;
    XCArgs.src = src;
    XCArgs.eReturnValue = INPUT_PORT_NONE_PORT;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MUX_HDMIPORT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return INPUT_PORT_NONE_PORT;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Support MHL Path Info
/// @param u8MhlSupportInfo               \b IN:  MHL Support Info
///@return @ref MS_BOOL True if query success.
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Mux_SetSupportMhlPathInfo_U2(void* pInstance, MS_U8 u8MhlSupportInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo = u8MhlSupportInfo;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo = u8MhlSupportInfo;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Mux_SetSupportMhlPathInfo(MS_U8 u8MhlSupportInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_MUX_SUPPORT_MHL_PATHINFO XCArgs;
    XCArgs.u8MhlSupportInfo = u8MhlSupportInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_SUPPORT_MHL_PATHINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set MHL Hot Plug Inverse Info
/// @param bIsMhlHotPlugInverse               \b IN:  MHL Hot Plug Inverse Info
///@return @ref MS_BOOL True if query success.
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Mux_SetMhlHotPlugInverseInfo_U2(void* pInstance, MS_BOOL bIsMhlHotPlugInverse)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsMhlHotPlugInverse = bIsMhlHotPlugInverse;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsMhlHotPlugInverse = bIsMhlHotPlugInverse;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Mux_SetMhlHotPlugInverseInfo(MS_BOOL bIsMhlHotPlugInverse)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_MUX_MHL_HOTPLUG_INVERSE_INFO XCArgs;
    XCArgs.bIsMhlHotPlugInverse = bIsMhlHotPlugInverse;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_MHL_HOTPLUG_INVERSE_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
//////////////////////////////////////
//    Set path & power
//////////////////////////////////////

static void _MApi_XC_Mux_SetPath(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType, E_DEST_TYPE enOutputType)
{
    MS_U8 u8Port_count = 0;
    E_MUX_INPUTPORT enPorts[3] = {INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT};
    MS_U8 MS_U8index = 0 ;
    MS_U8 dest = (MS_U8)enOutputType;
    // Get source ports.

    MApi_XC_Mux_GetPort(pInstance, enInputSourceType , enPorts , &u8Port_count );
    // Initial : Turn off Y/C mux
    // This is only for one VD
    if ( IsSrcTypeDigitalVD(enInputSourceType) && enOutputType != OUTPUT_CVBS1 && enOutputType != OUTPUT_CVBS2 )
    {
        MDrv_XC_mux_turnoff_ymux(pInstance);
        MDrv_XC_mux_turnoff_cmux(pInstance);
    }

    // For backward compatibility ( Remove if app layer ready )
    // Because we can not change the API, we add a check here for old behavior.
    if ( IsSrcTypeVga(enInputSourceType) && u8Port_count == 1)
    {
        MDrv_XC_mux_set_sync_port_by_dataport(pInstance, enPorts[0]);
    }

    for (  ;  MS_U8index < u8Port_count ; MS_U8index++ )
    {
        //printf(" Port index: %d  Port type: %d \n", MS_U8index ,enPorts[MS_U8index]);
        MDrv_XC_mux_dispatch(pInstance, enPorts[MS_U8index],(E_MUX_OUTPUTPORT)dest);
    }

    //printf(" Output port: %d \n", enOutputType);

}

//////////////////////////////////////
//    Path Managerment Functions
//////////////////////////////////////

/*!
 *  Check path is valid or not
 *  @param src The input source of a path
 *  @param dest The destination of a path
 *  @return BOOL true if path is valid.
 */
MS_BOOL _MApi_XC_Mux_CheckValidPath(void *pInstance, INPUT_SOURCE_TYPE_t src , E_DEST_TYPE dest)
{
    // After T3, there is no spec about SV -> CVBS out. (HW design)
    if ( IsSrcTypeSV(src) && ( dest == OUTPUT_CVBS1 || dest == OUTPUT_CVBS2 ) )
    {
        return Hal_XC_SVOutput_GetCaps(pInstance);
    }
    return TRUE;
}

/*!
 *  Search a patch and return the path index if the desire path founded
 *  @param src The input source of a path
 *  @param dest The destination of a path
 *  @return Path Id stored in Mux Controller
 */
static MS_S16 _MApi_XC_Mux_SearchPath(void *pInstance, INPUT_SOURCE_TYPE_t src, E_DEST_TYPE dest)
{
    MS_S16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( i = 0 ; i < MAX_DATA_PATH_SUPPORTED; i++)
    {
        if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src == src &&
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest == dest &&
                (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & DATA_PATH_USING)
           )
        {
            return i;
        }
    }
    return -1; // No maching path
}

static MS_S16 _MApi_XC_Mux_GetAvailablePath(void *pInstance, E_PATH_TYPE e_type)
{
    MS_S16 index = 0, boundary = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if ( e_type == PATH_TYPE_ASYNCHRONOUS )
    {
        index = MAX_SYNC_DATA_PATH_SUPPORTED ;
        boundary = MAX_DATA_PATH_SUPPORTED ;
    }
    else
    {
        boundary = MAX_SYNC_DATA_PATH_SUPPORTED ;
    }

    for (; index < boundary; index++)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[index].u8PathStatus & DATA_PATH_USING)
        {
            continue;
        }
        else
        {
            return index;
        }
    }

    return -1; // No maching path

}

// Return sources connected with Main window or Sub window
static MS_U8 _MApi_XC_Mux_GetUsingSources(void *pInstance, INPUT_SOURCE_TYPE_t* src_array )
{
    MS_U8 i = 0, j = 0 , available_source_count = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {
        // Retrieve sources which is using right now.
        if (  ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ( DATA_PATH_USING ) ) != ( DATA_PATH_USING ) )
            continue;

        // Check if source on path i already in src_array
        for ( j = 0 ; j < available_source_count ; j++ )
        {
            if ( src_array[j] == pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src )
                break;
        }

        if ( j >= available_source_count )
        {
            // no same source in src_array
            // only return source which connected with main or sub window
            if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest != OUTPUT_CVBS1 &&
                    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest != OUTPUT_CVBS2 )
            {
                src_array[available_source_count] = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src ;
		 // printf("\33[0;35m  i = %d source = %d [%s][%d] \33[m \n",i,pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src,__FUNCTION__,__LINE__);
                available_source_count++;
            }
        }
    }
    return available_source_count;
}
// Public Functions

//-------------------------------------------------------------------------------------------------
/// Initialize Mux before using Mux controller
/// @param  input_source_to_input_port      \b IN: a function to map from input source to input port
//-------------------------------------------------------------------------------------------------
void MApi_XC_Mux_Init_U2(void* pInstance, void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count ) )
{
    MS_U16 i;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount = 0;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.input_source_to_input_port = input_source_to_input_port;
    for ( i = 0 ; i < MAX_DATA_PATH_SUPPORTED; i++)
    {
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus = 0;
    }

    for(i=0; i<INPUT_SOURCE_NUM; i++)
    {
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[i] = 0;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_Mux_Init(void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count ) )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_INIT XCArgs;
    XCArgs.input_source_to_input_port = input_source_to_input_port;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Monitor the source of Mux
/// @param  bRealTimeMonitorOnly      \b IN: define whether do the monitor routine only for real time
//-------------------------------------------------------------------------------------------------
void MApi_XC_Mux_SourceMonitor_U2(void* pInstance, MS_BOOL bRealTimeMonitorOnly)
{
    MS_S16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    for ( i = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        if (  ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
                == ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
        {
            // execute path_thread()
            if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].path_thread != NULL)
            {

#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].path_thread(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src, bRealTimeMonitorOnly);
                _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].path_thread(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src, bRealTimeMonitorOnly);
                _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            }

            // execute destination periodic handler
            if(((pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ENABLE_PERIODIC_HANDLER) == ENABLE_PERIODIC_HANDLER) &&
                    (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_periodic_handler != NULL))
            {

#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_periodic_handler(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src, bRealTimeMonitorOnly );
                _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_periodic_handler(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src, bRealTimeMonitorOnly );
                _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            }
        }

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    }

    // This is patch for imme-switch. It can be fix in the feature.
    Hal_DVI_IMMESWITCH_PS_SW_Path();

}

void MApi_XC_Mux_SourceMonitor(MS_BOOL bRealTimeMonitorOnly)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_SOURCE_MONITOR XCArgs;
    XCArgs.bRealTimeMonitorOnly = bRealTimeMonitorOnly;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_SOURCE_MONITOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// A path is disabled after creating it (\ref MApi_XC_Mux_CreatePath)
/// If a path is not enabled,   \ref MApi_XC_Mux_SourceMonitor will bypass it.
/// @param PathId               \b IN: PathId The Path you want to enable
/// @return 1 if enable successfuly. Return -1 if enable fail
//-------------------------------------------------------------------------------------------------
MS_S16 MApi_XC_Mux_EnablePath_U2(void* pInstance, MS_U16 PathId)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if ( PathId >= MAX_DATA_PATH_SUPPORTED )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1;
    }
    else
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].u8PathStatus |= DATA_PATH_ENABLE;
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].u8PathStatus |= DATA_PATH_ENABLE;
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return 1;
    }
}

MS_S16 MApi_XC_Mux_EnablePath(MS_U16 PathId)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_ENABLE_PATH XCArgs;
    XCArgs.PathId = PathId;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_ENABLE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}


#if (ENABLE_NONSTD_INPUT_MCNR==1)

extern MS_U8 MDrv_AVD_GetRegValue(MS_U16 u16Addr);
#define BK_AFEC_CC    (0x35CC)

static void _MApi_XC_Sys_Detect_UnStd_Input_Status_Init(void)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal = FALSE;
    pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt = 0;
}

void _MApi_XC_CVBSUnStdISR(SC_INT_SRC eIntNum, void * pParam)
{    
    UNUSED(eIntNum);
    UNUSED(pParam);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if((MDrv_AVD_GetRegValue(BK_AFEC_CC) & 0x18) == 0x18)        // non-standard input
    {
        pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt = 0;

        if (pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal == FALSE)
        {
            Hal_SC_SetOPWriteOff(pInstance, TRUE);
            pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "_MApi_XC_CVBSUnStdISR: unStdSignal\n");
        }
    }
    else
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal == TRUE)
        {
            pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt++;

            if(pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt > 10)
            {
                Hal_SC_SetOPWriteOff(pInstance, FALSE);
                pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt = 0;
                pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal = FALSE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "_MApi_XC_CVBSUnStdISR: StdSignal\n");
            }
        }
    }
}

#endif

//-------------------------------------------------------------------------------------------------
/// delete a path from Mux Controller.
/// @param src              \b IN: Type of input source of a path you want to delete
/// @param dest             \b IN: Type of destination of a path you want to delete
/// @return 1 if delete successfuly. Return -1 if delete fail
//-------------------------------------------------------------------------------------------------
MS_S16 MApi_XC_Mux_DeletePath_U2(void* pInstance, INPUT_SOURCE_TYPE_t src, E_DEST_TYPE dest)
{
    // Maximum number of sources is the number of pathes
    INPUT_SOURCE_TYPE_t _InputSource[MAX_SYNC_DATA_PATH_SUPPORTED] ;
    MS_U8 _source_count = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    MS_S16 PathId = _MApi_XC_Mux_SearchPath(pInstance, src,dest);

    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"=========== MApi_XC_Mux_DeletePath(ID 0x%x) is called =========== \n", PathId);
    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Source : %d   Destination : %d \n",src,dest);

    if ( PathId != -1 )
    {
        MS_U8 u8Index = 0;

        // Initial _InputSource
        for (; u8Index < MAX_SYNC_DATA_PATH_SUPPORTED; u8Index++)
        {
            _InputSource[u8Index] = INPUT_SOURCE_NONE;
        }

        // Clear status.
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].u8PathStatus = 0;

        if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount > 0)
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount--;

        // Update ADC setting for multi-source.
        if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount > 0 )
        {
            _source_count = _MApi_XC_Mux_GetUsingSources(pInstance, _InputSource);

            XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Total using source current %d \n",_source_count);

            MDrv_XC_ADC_SetInputSource(pInstance, _InputSource, _source_count);
        }

        if ( IsSrcTypeHDMI(src) || IsSrcTypeDVI(src)  )
        {
            // Power down all HDMI power
            // because HDMI cannot PIP with HDMI, so we can power down all the HDMIs
            Hal_SC_mux_set_dvi_mux(pInstance, 0xFF, HAL_OUTPUTPORT_NONE_PORT);
        }

        //Disable SUB_Window CLK
        if(dest == OUTPUT_SCALER_SUB_WINDOW)
        {
            MApi_XC_EnableCLK_for_SUB(pInstance, DISABLE);
            MDrv_XC_Enable_Extra_Request(pInstance, DISABLE);
        }
        else if(dest == OUTPUT_SCALER_DWIN)
        {
            MApi_XC_EnableCLK_for_DIP(pInstance, DISABLE);
        }

        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"=========== Path deleted success =========== \n");

        // Decrease src create time
        if(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[src] > 0)
        {
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[src]--;
        }

        if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[src] == 0)
        {
            if ( IsSrcTypeYPbPr(src) || IsSrcTypeVga(src)  )
            {
                // ADC have to set as free run
                MDrv_XC_ADC_Set_Freerun(pInstance, TRUE);
            }
        }

#if (ENABLE_NONSTD_INPUT_MCNR == 1)
        MS_U8 i;

        for(i=0; i<_source_count; i++)
        {
            if(IsSrcTypeATV(_InputSource[i]))
            {
                break;
            }
        }

        if(i==_source_count)
        {
            // there is no more ATV input, de-attach ISR
            MDrv_XC_InterruptDeAttach(pInstance, XC_INT_VSINT, _MApi_XC_CVBSUnStdISR, &pXCResourcePrivate->stdrvXC_MVideo.u8UnStdOutput[MAIN_WINDOW]);
        }
#endif

        // We clear Input source variable here. To reset scaler IP again if same source connected again.
        if (dest == OUTPUT_SCALER_MAIN_WINDOW)
        {
            pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW] = INPUT_SOURCE_NONE;
        }
        else if (dest == OUTPUT_SCALER_SUB_WINDOW)
        {
            pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[SUB_WINDOW] = INPUT_SOURCE_NONE;
        }

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"This path is not exist\n");
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"=========== Path deleted fail =========== \n");

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1;
    }
}

MS_S16 MApi_XC_Mux_DeletePath(INPUT_SOURCE_TYPE_t src, E_DEST_TYPE dest)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_DELETE_PATH XCArgs;
    XCArgs.src = src;
    XCArgs.dest = dest;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_DELETE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

void MApi_XC_Set_Interlace_Status(MS_BOOL bIsInterlace,SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_INTERLACE_STATUS XCArgs;
    XCArgs.bIsInterlace = bIsInterlace;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_INTERLACE_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

void MApi_XC_Set_Interlace_Status_U2(void *pInstance,MS_BOOL bIsInterlace,SCALER_WIN eWindow)
{
    MS_U8 reg_IP1F2_1D, reg_IP1F2_23;
    MS_U16 reg_IP1F2_21;

    if (bIsInterlace)
    {
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_21 = 0x0403;
        reg_IP1F2_23 = 0x30;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
    }
    else
    {
        reg_IP1F2_1D = 0x21;
        reg_IP1F2_21 = 0x0400;
        reg_IP1F2_23 = 0x00;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
    }

    MDrv_SC_setfield(pInstance, reg_IP1F2_1D, reg_IP1F2_21, reg_IP1F2_23, eWindow);

}


//-------------------------------------------------------------------------------------------------
/// Trigger Sync Event on Mux Controller.
/// The Mux Controller will pass src and *para to each Sync Event Handler (refer synchronous_event_handler of MUX_DATA_PATH also).
///  ( Delegate Function:
///    Send necessary parameter to SyncEventHandler of each synchronous_path which source is 'src' )
/// @param src          \b IN: The input source which triggers Sync Event
/// @param para         \b IN: A pointer points to parameters which need to pass to Event Handler
//-------------------------------------------------------------------------------------------------
void MApi_XC_Mux_TriggerPathSyncEvent_U2( void* pInstance, INPUT_SOURCE_TYPE_t src , void* para)
{
    MS_S16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    for ( i = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        if (  ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
                != ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            continue;
        }

        // send 'src' to event handler of each path
        if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src == src &&
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].synchronous_event_handler != NULL)
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].synchronous_event_handler(src, para);
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].synchronous_event_handler(src, para);
            _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        }

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    }
}

void MApi_XC_Mux_TriggerPathSyncEvent( INPUT_SOURCE_TYPE_t src , void* para)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_TRIGGER_PATH_SYNC_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_TRIGGER_PATH_SYNC_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/*!
 *  Trigger Sync Event on Mux Controller.
 *  The Mux Controller will pass src and *para to each Sync Event Handler (refer to synchronous_event_handler
 *   of MUX_DATA_PATH also).
 *
 *  ( Delegate Function:
 *    Send necessary parameter to SyncEventHandler of each synchronous_path which source is 'src' )
 *  @param src The input source which triggers Sync Event
 *  @param para A pointer points to parameters which need to pass to Event Handler
 */
//-------------------------------------------------------------------------------------------------
/// Set the specific window
/// @param  src             \b IN: the source type for handler
/// @param para         \b IN: A pointer points to parameters which need to pass to Event Handler
//-------------------------------------------------------------------------------------------------
void MApi_XC_Mux_TriggerDestOnOffEvent_U2( void* pInstance, INPUT_SOURCE_TYPE_t src , void* para)
{
    MS_S16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    for ( i = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        if (  ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
                != ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            continue;
        }

        // send 'src' to event handler of each path
        if ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src == src &&
                pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_on_off_event_handler != NULL)
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_on_off_event_handler(src, para);
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_on_off_event_handler(src, para);
            _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        }

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    }
}

void MApi_XC_Mux_TriggerDestOnOffEvent( INPUT_SOURCE_TYPE_t src , void* para)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_TRIGGER_DEST_ONOFF_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_TRIGGER_DEST_ONOFF_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function was used to enable/disable the destination periodic handler
/// After enabled periodic hander, Mux Controller will pass the parameters to this handler and execute it periodically
/// @param  src             \b IN: the source type for handler
/// @param  bEnable         \b IN: Enable/Disable the priodic handler.
/// @return 1 if successfuly. Return -1 if failed.
//-------------------------------------------------------------------------------------------------
MS_S16 MApi_XC_Mux_OnOffPeriodicHandler_U2( void* pInstance, INPUT_SOURCE_TYPE_t src, MS_BOOL bEnable)
{
    MS_S16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( i = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {
        if (  ( pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
                != ( DATA_PATH_ENABLE | DATA_PATH_USING ) )
        {
            continue;
        }

        // send 'src' to event handler of each path
        if(bEnable)
        {
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus |= ENABLE_PERIODIC_HANDLER;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus &= ~ENABLE_PERIODIC_HANDLER;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return i;
}

MS_S16 MApi_XC_Mux_OnOffPeriodicHandler( INPUT_SOURCE_TYPE_t src, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_ONOFF_PERIODIC_HANDLER XCArgs;
    XCArgs.src = src;
    XCArgs.bEnable = bEnable;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_ONOFF_PERIODIC_HANDLER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

typedef enum
{
    E_OnPathAdd = 0,
    E_OnPathDel= 1,
    E_PathMax,
} E_PathStatus;

static MS_U8 _MApi_XC_Mux_GetPathInfo(void* pInstance, XC_MUX_PATH_INFO* Paths)
{
    MS_S16 i;
    MS_U8 count;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( i = 0, count = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & DATA_PATH_USING)
        {
            Paths[count].src = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src;
            Paths[count].dest = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest;
            Paths[count].path_thread = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].path_thread;
            Paths[count].SyncEventHandler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].synchronous_event_handler;
            Paths[count].DestOnOff_Event_Handler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_on_off_event_handler;
            Paths[count].dest_periodic_handler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_periodic_handler;
            count++;
            //printf(" \33[0;35m  Path Id: [%d]  Source: %d   Destination: %d  \33[m \n",i, Paths[count].src,Paths[count].dest);
        }
    }

    return count;
}

//-------------------------------------------------------------------------------------------------
/// Create a path in Mux Controller.
/// @param  Path_Info               \b IN: the information of the path
/// @param  u32InitDataLen          \b IN: the length of the Path_Info
/// @return @ref MS_S16 return the path id, or -1 when any error is happened
//-------------------------------------------------------------------------------------------------
MS_S16 MApi_XC_Mux_CreatePath_U2(void* pInstance, XC_MUX_PATH_INFO* Path_Info , MS_U32 u32InitDataLen)
{
    MS_S16 PathId;
    MS_U8 u8Index=0;

    // Maximum number of sources is the number of pathes
    INPUT_SOURCE_TYPE_t _InputSource[MAX_SYNC_DATA_PATH_SUPPORTED] ;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    // SC2 modification
    MS_U8 u8DigitalVD_count = 0 ;
    MS_U8 u8Spath_count =  0 ;
    XC_MUX_PATH_INFO SC_Paths[MAX_DATA_PATH_SUPPORTED] = {0};
    MS_U8 _source_count = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if(u32InitDataLen != sizeof(XC_MUX_PATH_INFO) || Path_Info == NULL)
    {
        // ASSERT when driver is not initiallized
        MS_ASSERT(0);
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"=========== MApi_XC_Mux_CreatePath is called =========== \n");
    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Source : %d   Destination : %d \n",Path_Info->src,Path_Info->dest);

    if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount >= MAX_DATA_PATH_SUPPORTED)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"There is no avaliable path. Current Total Path Count : %d ",pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount );

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1; // No avaliable path.
    }

    if ( _MApi_XC_Mux_SearchPath(pInstance, Path_Info->src,Path_Info->dest) != -1)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Path already exist\n");

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1; // Path exist
    }

    if ( _MApi_XC_Mux_CheckValidPath(pInstance, Path_Info->src, Path_Info->dest) == FALSE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Path is invalid\n");

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1; // Path is invalid
    }


    if(_XC_Mutex == -1)
    {
        MS_ASSERT(0);
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"XC dirver have to be initiallized first \n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return -1;
    }

    //Add one path
    PathId= _MApi_XC_Mux_GetAvailablePath(pInstance, Path_Info->Path_Type);

    if ( PathId == -1 )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_MUX,"There is no avaliable path. Current Total Path Count : %d ",pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount );

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return -1; // no available path
    }

    // Path create.
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u8PathCount++;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].e_PathType = Path_Info->Path_Type;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].src = Path_Info->src;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].dest = Path_Info->dest;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].u8PathStatus = DATA_PATH_USING;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].path_thread = Path_Info->path_thread;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].synchronous_event_handler = Path_Info->SyncEventHandler;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].dest_on_off_event_handler = Path_Info->DestOnOff_Event_Handler;
    pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[PathId].dest_periodic_handler = Path_Info->dest_periodic_handler;


    // Set physical registers after path created.
    _MApi_XC_Mux_SetPath(pInstance, Path_Info->src,Path_Info->dest);

    // Initial _InputSource
    for (; u8Index < MAX_SYNC_DATA_PATH_SUPPORTED; u8Index++)
    {
        _InputSource[u8Index] = INPUT_SOURCE_NONE;
    }

    // Update ADC setting for multi-source.
    _source_count = _MApi_XC_Mux_GetUsingSources(pInstance, _InputSource);

    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Total using source current %d \n",_source_count);

   u8Spath_count = _MApi_XC_Mux_GetPathInfo(pInstance, SC_Paths);
   MS_U8 i = 0;
   for (  i = 0 ; i < u8Spath_count ; ++i )
   {
        if( IsSrcTypeDigitalVD(SC_Paths[i].src) )
        {
              ++u8DigitalVD_count;
         }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"Total using source current =%d    Total using path count = %d \n",_source_count , u8Spath_count);
    if( u8DigitalVD_count >1)//Another window already connect to
    {
          XC_LOG_TRACE(XC_DBGLEVEL_MUX," Some window already connect  one of ATV/AV/Scart in , no need to set ADC source again ->  DigitalVD_count : %d  \n",u8DigitalVD_count);
    }
    else
    {
          MDrv_XC_ADC_SetInputSource(pInstance, _InputSource, _source_count);
    }
    // Check if need calibration or not
    if(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[Path_Info->src] == 0 )
    {
        if (u8DigitalVD_count > 1 )//Another already connect to AVD
	{
            XC_LOG_TRACE(XC_DBGLEVEL_MUX," Some window already connect  one of ATV/AV/Scart in so no need to do calibration again  \n");
        }
        else
        {
            // calibrate input source
            if(IsSrcTypeDigitalVD(Path_Info->src))
            {
                if(Path_Info->dest == OUTPUT_OFFLINE_DETECT)
                {
                    MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, Path_Info->src));
                }
            }
            else if(Path_Info->dest != OUTPUT_OFFLINE_DETECT)
            {
                MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, Path_Info->src));
            }
            XC_LOG_TRACE(XC_DBGLEVEL_MUX," Need to do ADC calibration for first time link source (%d)    \n",Path_Info->src);
            if(IsSrcTypeYPbPr(Path_Info->src))
            {
                MDrv_XC_ADC_SetSoGCal(pInstance);
                MsOS_DelayTask(10);
            }

            if ( IsSrcTypeYPbPr(Path_Info->src) ||IsSrcTypeVga(Path_Info->src)  )
            {
                // ADC have to set as free run
                MDrv_XC_ADC_Set_Freerun(pInstance, TRUE);
            }
        }

    }

    // Increate src create time
    if(pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[Path_Info->src] < 0xFFFF)
    {
        pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.u16SrcCreateTime[Path_Info->src]++;
    }

    //Enable Sub_Window CLK
    if(Path_Info->dest == OUTPUT_SCALER_SUB_WINDOW)
    {
        // Disable Pre-Scaling for ATV pink issue
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L, 0x00 , BIT(15) );
        MApi_XC_EnableCLK_for_SUB(pInstance, ENABLE);
    }
    else if(Path_Info->dest == OUTPUT_SCALER_MAIN_WINDOW)
    {
        // Disable Pre-Scaling for ATV pink issue
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, 0x00 , BIT(15) );
    }
    else if(Path_Info->dest == OUTPUT_SCALER_DWIN)
    {
        // Enable DIP window clock
        MApi_XC_EnableCLK_for_DIP(pInstance, ENABLE);
    }


#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE




    XC_LOG_TRACE(XC_DBGLEVEL_MUX,"=========== Path created success =========== \n");

#if (ENABLE_NONSTD_INPUT_MCNR == 1)
    if(IsSrcTypeATV(Path_Info->src) &&
            (!MApi_XC_InterruptIsAttached(XC_INT_VSINT, _MApi_XC_CVBSUnStdISR, &pXCResourcePrivate->stdrvXC_MVideo.u8UnStdOutput[MAIN_WINDOW])))
    {
        // Re-init state machine
        _MApi_XC_Sys_Detect_UnStd_Input_Status_Init();

        // Attach ISR
        MDrv_XC_InterruptAttach(pInstance, XC_INT_VSINT, _MApi_XC_CVBSUnStdISR, &pXCResourcePrivate->stdrvXC_MVideo.u8UnStdOutput[MAIN_WINDOW]);
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return PathId;
}

MS_BOOL MApi_XC_Get_Record_Flag_U2(void* pInstance)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = FALSE;
#if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE


    bReturn = pXCResourcePrivate->bRecord_Flag;

#if SC2_ENABLE
         _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
         _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return bReturn;
}

MS_BOOL MApi_XC_Get_Record_Flag(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }
    stXC_GET_RECORD_FLAG XCArgs;
    XCArgs.bReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RECORD_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

void MApi_XC_Set_Record_Flag_U2(void* pInstance,MS_BOOL bRecord_Flag)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE


    pXCResourcePrivate->bRecord_Flag = bRecord_Flag;

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

}
void MApi_XC_Set_Record_Flag(MS_BOOL bRecord_Flag)
{
   stXC_SET_RECORD_FLAG XCArgs;
   XCArgs.bRecord_Flag = bRecord_Flag;

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

   if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RECORD_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
   {
       printf("Obtain XC engine fail\n");
       return;
   }

   if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SET_RECORD_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
   {
       printf("Obtain XC engine fail\n");
       return;
   }
}

MS_S16 MApi_XC_Mux_CreatePath(XC_MUX_PATH_INFO* Path_Info , MS_U32 u32InitDataLen)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_CREATE_PATH XCArgs;
    XCArgs.Path_Info = Path_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MUX_CREATE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Map input source to VD Ymux port
/// @param  u8InputSourceType      \b IN: input source type
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_MUX_MapInputSourceToVDYMuxPORT_U2( void* pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u8Return = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType );
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u8Return = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType );
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u8Return;
}

MS_U8 MApi_XC_MUX_MapInputSourceToVDYMuxPORT( INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_MUX_INPUTSOURCE2VDYMUXPORT XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MUX_INPUTSOURCE2VDYMUXPORT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get Paths in driver.
/// @param  Paths      \b OUT: Path information
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_Mux_GetPathInfo_U2(void* pInstance, XC_MUX_PATH_INFO* Paths)
{
    MS_S16 i;
    MS_U8 count;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( i = 0, count = 0 ; i < MAX_SYNC_DATA_PATH_SUPPORTED ; i++)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].u8PathStatus & DATA_PATH_USING)
        {
            Paths[count].src = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].src;
            Paths[count].dest = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest;
            Paths[count].path_thread = pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].path_thread;
            Paths[count].SyncEventHandler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].synchronous_event_handler;
            Paths[count].DestOnOff_Event_Handler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_on_off_event_handler;
            Paths[count].dest_periodic_handler =  pXCResourcePrivate->stdrvXC_MVideo.s_InputSource_Mux_attr.Mux_DataPaths[i].dest_periodic_handler;
            count++;
            //printf(" Path Id: [%d]  Source: %d   Destination: %d \n",i, s_InputSource_Mux_attr.Mux_DataPaths[i].src,
            //s_InputSource_Mux_attr.Mux_DataPaths[i].dest);
        }
    }


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return count;
}

MS_U8 MApi_XC_Mux_GetPathInfo(XC_MUX_PATH_INFO* Paths)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_MUX_PATHINFO XCArgs;
    XCArgs.Paths = Paths;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MUX_PATHINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get XC library version
/// @param  ppVersion                  \b OUT: store the version in this member
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if (!ppVersion)
    {
        return E_APIXC_RET_FAIL;
    }

    *ppVersion = &_api_xc_version;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;
}
E_APIXC_ReturnValue MApi_XC_GetLibVer(const MSIF_Version **ppVersion)
{
    //This function may be called before creating pu32XCInst
    if (!ppVersion)
    {
        return E_APIXC_RET_FAIL;
    }

    *ppVersion = &_api_xc_version;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;

//Coverity Structually dead code
#if 0
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_LIB_VER XCArgs;
    XCArgs.ppVersion = ppVersion;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LIB_VERSION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get XC Information
/// @return @ref XC_ApiInfo returnthe XC information in this member
//-------------------------------------------------------------------------------------------------
XC_ApiInfo * MApi_XC_GetInfo_U2(void* pInstance)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_GetInfo(pInstance, &pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_GetInfo(pInstance, &pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return &pXCResourcePrivate->stdrvXC_MVideo._stXC_ApiInfo;
}

const XC_ApiInfo * MApi_XC_GetInfo(void)
{
    stXC_CMD_GET_INFO XCArgs;
    XCArgs.stReturnValue = NULL;

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return XCArgs.stReturnValue;
    }
    static XC_ApiInfo xcInfo = {0,};
    XCArgs.stReturnValue = &xcInfo;
    //XCArgs.stReturnValue = (XC_ApiInfo*)malloc(sizeof(XC_ApiInfo));
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return XCArgs.stReturnValue; // return meaningless value because get instance failed.
    }
    else
    {
        return XCArgs.stReturnValue;
    }
}


//-------------------------------------------------------------------------------------------------
/// Extended interface for Getting XC Status of specific window(Main/Sub)
/// @param  pDrvStatusEx                  \b OUT: store the status
/// @param  eWindow                     \b IN: which window(Main/Sub) is going to get status
/// @return @ref MS_U16 return the copied length of input structure
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetStatusEx_U2(void* pInstance, XC_ApiStatusEx *pDrvStatusEx, SCALER_WIN eWindow)
{
    XC_ApiStatusEx DrvStatusEx;
    MS_U16 u16CopiedLength = sizeof(XC_ApiStatusEx);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if((NULL == pDrvStatusEx) || (pDrvStatusEx->u16ApiStatusEX_Length == 0))
    {
        //We cosider compatible operation form version2 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_GetStatusEx: Null paramter or Wrong u16ApiStatusEX_Length!!\n")

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return 0;
    }

    // the version control is coming in with version 1
    if(pDrvStatusEx->u32ApiStatusEx_Version < 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_GetStatusEx: please check your u32ApiStatusEx_Version, it should not set to 0!!\n")
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return 0;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //new AP + old lib, only the length corresponding to old lib has meaning.
    if(pDrvStatusEx->u32ApiStatusEx_Version > API_STATUS_EX_VERSION)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_GetStatusEx: this old version XC lib has only length:%u driver status!!\n",
                     sizeof(XC_ApiStatusEx))
        //we still copy the min size of both structure, but AP should take care of it.
        u16CopiedLength = sizeof(XC_ApiStatusEx);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pDrvStatusEx->u32ApiStatusEx_Version < API_STATUS_EX_VERSION) || (pDrvStatusEx->u16ApiStatusEX_Length < sizeof(XC_ApiStatusEx)))
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_GetStatusEx: new version XC lib shouldn't access to the space which doesn't exist in old structure!!\n")
        u16CopiedLength = pDrvStatusEx->u16ApiStatusEX_Length;
    }

    DrvStatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION; //Return the actual version of the returned value for APP to use
    DrvStatusEx.u16ApiStatusEX_Length = u16CopiedLength; //Return the actual length of the returned value for APP to know whick info is valid

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();

#endif //#if SC2_ENABLE


    //----------------------
    // Customer setting
    //----------------------
    DrvStatusEx.enInputSourceType = gSrcInfo[eWindow].enInputSourceType;

    //----------------------
    // Window
    //----------------------
    memcpy(&DrvStatusEx.stCapWin, &gSrcInfo[eWindow].stCapWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&DrvStatusEx.stCropWin, &gSrcInfo[eWindow].stCropWin, sizeof(MS_WINDOW_TYPE));
    /* real crop win in memory */
    memcpy(&DrvStatusEx.ScaledCropWin, &gSrcInfo[eWindow].ScaledCropWin, sizeof(MS_WINDOW_TYPE));
    //Assign display window for AP layer
    memcpy(&DrvStatusEx.stDispWin, &gSrcInfo[eWindow].stDispWin, sizeof(MS_WINDOW_TYPE));

    if(DrvStatusEx.stDispWin.x >= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16HStart)
    {
        DrvStatusEx.stDispWin.x -= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16HStart;
    }
    if(DrvStatusEx.stDispWin.y >= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart)
    {
        DrvStatusEx.stDispWin.y -= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart;
    }

    if(MDrv_XC_Is2K2KToFrc(pInstance))
    {
        DrvStatusEx.stDispWin.x = DrvStatusEx.stDispWin.x * 2;
        DrvStatusEx.stDispWin.width= DrvStatusEx.stDispWin.width * 2;
    }

    //----------------------
    // Timing
    //----------------------
    DrvStatusEx.bInterlace = gSrcInfo[eWindow].bInterlace;
    DrvStatusEx.bHDuplicate = gSrcInfo[eWindow].bHDuplicate;
    DrvStatusEx.u16InputVFreq = gSrcInfo[eWindow].u16InputVFreq;
    DrvStatusEx.u16InputVTotal = gSrcInfo[eWindow].u16InputVTotal;
    DrvStatusEx.u16DefaultHtotal = gSrcInfo[eWindow].u16DefaultHtotal;
    DrvStatusEx.u8DefaultPhase = (MS_U8)gSrcInfo[eWindow].u16DefaultPhase;
    DrvStatusEx.u16DefaultPhase = gSrcInfo[eWindow].u16DefaultPhase;

    //----------------------
    // customized scaling
    //----------------------
    DrvStatusEx.bHCusScaling = gSrcInfo[eWindow].bHCusScaling;
    DrvStatusEx.u16HCusScalingSrc = gSrcInfo[eWindow].u16HCusScalingSrc;
    DrvStatusEx.u16HCusScalingDst = gSrcInfo[eWindow].u16HCusScalingDst;
    DrvStatusEx.bVCusScaling = gSrcInfo[eWindow].bVCusScaling;
    DrvStatusEx.u16VCusScalingSrc = gSrcInfo[eWindow].u16VCusScalingSrc;
    DrvStatusEx.u16VCusScalingDst = gSrcInfo[eWindow].u16VCusScalingDst;

    DrvStatusEx.bPreHCusScaling = gSrcInfo[eWindow].Status2.bPreHCusScaling;
    DrvStatusEx.u16PreHCusScalingSrc = gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc;
    DrvStatusEx.u16PreHCusScalingDst = gSrcInfo[eWindow].Status2.u16PreHCusScalingDst;
    DrvStatusEx.bPreVCusScaling = gSrcInfo[eWindow].Status2.bPreVCusScaling;
    DrvStatusEx.u16PreVCusScalingSrc = gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc;
    DrvStatusEx.u16PreVCusScalingDst = gSrcInfo[eWindow].Status2.u16PreVCusScalingDst;

    //--------------
    // 9 lattice
    //--------------
    DrvStatusEx.bDisplayNineLattice = gSrcInfo[eWindow].bDisplayNineLattice;

    //----------------------
    // XC internal setting
    //----------------------

    /* scaling ratio */
    DrvStatusEx.u16H_SizeAfterPreScaling = gSrcInfo[eWindow].u16H_SizeAfterPreScaling;
    DrvStatusEx.u16V_SizeAfterPreScaling = gSrcInfo[eWindow].u16V_SizeAfterPreScaling;
    DrvStatusEx.bPreV_ScalingDown = gSrcInfo[eWindow].bPreV_ScalingDown;


    /* others */
    DrvStatusEx.u32Op2DclkSet = gSrcInfo[eWindow].u32Op2DclkSet;

    /* Video screen status */
    DrvStatusEx.bBlackscreenEnabled = gSrcInfo[eWindow].bBlackscreenEnabled;
    DrvStatusEx.bBluescreenEnabled = gSrcInfo[eWindow].bBluescreenEnabled;
    DrvStatusEx.u16VideoDark = gSrcInfo[eWindow].u16VideoDark;

    DrvStatusEx.u16V_Length = gSrcInfo[eWindow].u16V_Length;
    DrvStatusEx.u16BytePerWord = gSrcInfo[eWindow].u16BytePerWord;
    DrvStatusEx.u16OffsetPixelAlignment = gSrcInfo[eWindow].u16OffsetPixelAlignment;
    DrvStatusEx.u8BitPerPixel = gSrcInfo[eWindow].u8BitPerPixel;

    DrvStatusEx.eDeInterlaceMode = gSrcInfo[eWindow].eDeInterlaceMode;
    DrvStatusEx.u8DelayLines = gSrcInfo[eWindow].u8DelayLines;
    DrvStatusEx.bMemFmt422 = gSrcInfo[eWindow].bMemFmt422;
    DrvStatusEx.eMemory_FMT = gSrcInfo[eWindow].eMemory_FMT;

    DrvStatusEx.bForceNRoff = gSrcInfo[eWindow].bForceNRoff;
    DrvStatusEx.bEn3DNR = gSrcInfo[eWindow].bEn3DNR;
    DrvStatusEx.bUseYUVSpace = gSrcInfo[eWindow].bUseYUVSpace;
    DrvStatusEx.bMemYUVFmt = gSrcInfo[eWindow].bMemYUVFmt;
    DrvStatusEx.bForceRGBin = gSrcInfo[eWindow].bForceRGBin;
    DrvStatusEx.bLinearMode = gSrcInfo[eWindow].bLinearMode;

    // frame lock related
    // only main can select FB/FBL because panel output timing depends on main window, sub will always use FB
    DrvStatusEx.bFBL = gSrcInfo[eWindow].bFBL;
    DrvStatusEx.bFastFrameLock = gSrcInfo[eWindow].bFastFrameLock;
    DrvStatusEx.bDoneFPLL = MDrv_Scaler_GetFPLLDoneStatus(pInstance);
    DrvStatusEx.bEnableFPLL = gSrcInfo[eWindow].bEnableFPLL;
    DrvStatusEx.bFPLL_LOCK = gSrcInfo[eWindow].bFPLL_LOCK;

    // Capture_Memory
    DrvStatusEx.u32IPMBase0 = gSrcInfo[eWindow].u32IPMBase0;
    DrvStatusEx.u32IPMBase1 = gSrcInfo[eWindow].u32IPMBase1;
    DrvStatusEx.u32IPMBase2 = gSrcInfo[eWindow].u32IPMBase2;
    DrvStatusEx.u16IPMOffset = gSrcInfo[eWindow].u16IPMOffset;
    DrvStatusEx.u16IPMFetch = gSrcInfo[eWindow].u16IPMFetch;

    //HSizeChangedBeyondPQ status
    DrvStatusEx.bPQSetHSD = !(gSrcInfo[eWindow].bHCusScaling || gSrcInfo[eWindow].Status2.bPreHCusScaling
                              || MDrv_XC_GetHSizeChangeManuallyFlag(pInstance, eWindow));
#ifndef DISABLE_3D_FUNCTION
    DrvStatusEx.bIsHWDepthAdjSupported = MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow),
                                         MDrv_XC_Get_3D_Output_Mode(pInstance),
                                         eWindow);
    DrvStatusEx.bIs2LineMode = MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, &gSrcInfo[eWindow], eWindow);
#else
    DrvStatusEx.bIsHWDepthAdjSupported = FALSE;
    DrvStatusEx.bIs2LineMode = FALSE;
#endif
    DrvStatusEx.bIsPNLYUVOutput = MDrv_XC_IsPNLYUVOutput(pInstance);

    //if inputSource is HDMI, get the HDMI pixel repetition info.
    if(IsSrcTypeHDMI(DrvStatusEx.enInputSourceType))
    {
        DrvStatusEx.u8HDMIPixelRepetition = MDrv_HDMI_avi_infoframe_info(_BYTE_5) & 0x0F;
    }
    else
    {
        DrvStatusEx.u8HDMIPixelRepetition = 0;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    //shorter Ap structure with longer lib structure, we should handle it to avoid access-violation.
    memcpy(pDrvStatusEx, &DrvStatusEx, u16CopiedLength);

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return u16CopiedLength;
}

MS_U16 MApi_XC_GetStatusEx(XC_ApiStatusEx *pDrvStatusEx, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_STATUS_EX XCArgs;
    XCArgs.pDrvStatusEx = pDrvStatusEx;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_STATUS_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get XC Status of specific window(Main/Sub)
/// @param  pDrvStatus                  \b OUT: store the status
/// @param  eWindow                     \b IN: which window(Main/Sub) is going to get status
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_GetStatus(void *pInstance, XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //----------------------
    // Customer setting
    //----------------------
    pDrvStatus->enInputSourceType = gSrcInfo[eWindow].enInputSourceType;

    //----------------------
    // Window
    //----------------------
    memcpy(&pDrvStatus->stCapWin, &gSrcInfo[eWindow].stCapWin, sizeof(MS_WINDOW_TYPE));
    memcpy(&pDrvStatus->stCropWin, &gSrcInfo[eWindow].stCropWin, sizeof(MS_WINDOW_TYPE));
    /* real crop win in memory */
    memcpy(&pDrvStatus->ScaledCropWin, &gSrcInfo[eWindow].ScaledCropWin, sizeof(MS_WINDOW_TYPE));
    //Assign display window for AP layer
    memcpy(&pDrvStatus->stDispWin, &gSrcInfo[eWindow].stDispWin, sizeof(MS_WINDOW_TYPE));

    if(pDrvStatus->stDispWin.x >= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16HStart)
    {
        pDrvStatus->stDispWin.x -= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16HStart;
    }
    if(pDrvStatus->stDispWin.y >= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart)
    {
        pDrvStatus->stDispWin.y -= gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart;
    }

    //----------------------
    // Timing
    //----------------------
    pDrvStatus->bInterlace = gSrcInfo[eWindow].bInterlace;
    pDrvStatus->bHDuplicate = gSrcInfo[eWindow].bHDuplicate;
    pDrvStatus->u16InputVFreq = gSrcInfo[eWindow].u16InputVFreq;
    pDrvStatus->u16InputVTotal = gSrcInfo[eWindow].u16InputVTotal;
    pDrvStatus->u16DefaultHtotal = gSrcInfo[eWindow].u16DefaultHtotal;
    pDrvStatus->u8DefaultPhase = (MS_U8)gSrcInfo[eWindow].u16DefaultPhase;

    //----------------------
    // customized scaling
    //----------------------
    pDrvStatus->bHCusScaling = gSrcInfo[eWindow].bHCusScaling;
    pDrvStatus->u16HCusScalingSrc = gSrcInfo[eWindow].u16HCusScalingSrc;
    pDrvStatus->u16HCusScalingDst = gSrcInfo[eWindow].u16HCusScalingDst;
    pDrvStatus->bVCusScaling = gSrcInfo[eWindow].bVCusScaling;
    pDrvStatus->u16VCusScalingSrc = gSrcInfo[eWindow].u16VCusScalingSrc;
    pDrvStatus->u16VCusScalingDst = gSrcInfo[eWindow].u16VCusScalingDst;

    //--------------
    // 9 lattice
    //--------------
    pDrvStatus->bDisplayNineLattice = gSrcInfo[eWindow].bDisplayNineLattice;

    //----------------------
    // XC internal setting
    //----------------------

    /* scaling ratio */
    pDrvStatus->u16H_SizeAfterPreScaling = gSrcInfo[eWindow].u16H_SizeAfterPreScaling;
    pDrvStatus->u16V_SizeAfterPreScaling = gSrcInfo[eWindow].u16V_SizeAfterPreScaling;
    pDrvStatus->bPreV_ScalingDown = gSrcInfo[eWindow].bPreV_ScalingDown;


    /* others */
    pDrvStatus->u32Op2DclkSet = gSrcInfo[eWindow].u32Op2DclkSet;

    /* Video screen status */
    pDrvStatus->bBlackscreenEnabled = gSrcInfo[eWindow].bBlackscreenEnabled;
    pDrvStatus->bBluescreenEnabled = gSrcInfo[eWindow].bBluescreenEnabled;
    pDrvStatus->u16VideoDark = gSrcInfo[eWindow].u16VideoDark;

    pDrvStatus->u16V_Length = gSrcInfo[eWindow].u16V_Length;
    pDrvStatus->u16BytePerWord = gSrcInfo[eWindow].u16BytePerWord;
    pDrvStatus->u16OffsetPixelAlignment = gSrcInfo[eWindow].u16OffsetPixelAlignment;
    pDrvStatus->u8BitPerPixel = gSrcInfo[eWindow].u8BitPerPixel;

    pDrvStatus->eDeInterlaceMode = gSrcInfo[eWindow].eDeInterlaceMode;
    pDrvStatus->u8DelayLines = gSrcInfo[eWindow].u8DelayLines;
    pDrvStatus->bMemFmt422 = gSrcInfo[eWindow].bMemFmt422;
    pDrvStatus->eMemory_FMT = gSrcInfo[eWindow].eMemory_FMT;

    pDrvStatus->bForceNRoff = gSrcInfo[eWindow].bForceNRoff;
    pDrvStatus->bEn3DNR = gSrcInfo[eWindow].bEn3DNR;
    pDrvStatus->bUseYUVSpace = gSrcInfo[eWindow].bUseYUVSpace;
    pDrvStatus->bMemYUVFmt = gSrcInfo[eWindow].bMemYUVFmt;
    pDrvStatus->bForceRGBin = gSrcInfo[eWindow].bForceRGBin;
    pDrvStatus->bLinearMode = gSrcInfo[eWindow].bLinearMode;

    // frame lock related
    // only main can select FB/FBL because panel output timing depends on main window, sub will always use FB
    pDrvStatus->bFBL = gSrcInfo[eWindow].bFBL;
    pDrvStatus->bFastFrameLock = gSrcInfo[eWindow].bFastFrameLock;
    pDrvStatus->bDoneFPLL = MDrv_Scaler_GetFPLLDoneStatus(pInstance);
    pDrvStatus->bEnableFPLL = gSrcInfo[eWindow].bEnableFPLL;
    pDrvStatus->bFPLL_LOCK = gSrcInfo[eWindow].bFPLL_LOCK;

    // Capture_Memory
    pDrvStatus->u32IPMBase0 = gSrcInfo[eWindow].u32IPMBase0;
    pDrvStatus->u32IPMBase1 = gSrcInfo[eWindow].u32IPMBase1;
    pDrvStatus->u32IPMBase2 = gSrcInfo[eWindow].u32IPMBase2;
    pDrvStatus->u16IPMOffset = gSrcInfo[eWindow].u16IPMOffset;
    pDrvStatus->u16IPMFetch = gSrcInfo[eWindow].u16IPMFetch;
    pDrvStatus->u16OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;

    // For GST DS informaton
    {
        //if(sizeof(XC_ApiStatus) > 148)
        {
            pDrvStatus->u32DSIPMBase0   = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] / BYTE_PER_WORD ;
            pDrvStatus->u32DSIPMBufSize = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow];
            pDrvStatus->u32DSFRCMBase0   = pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] / BYTE_PER_WORD ;
            pDrvStatus->u32DSFRCMBufSize = pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow];
            pDrvStatus->u32MISC_B       = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return TRUE;
}

MS_BOOL MApi_XC_GetStatus_U2(void* pInstance, XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE

    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
#endif //#if SC2_ENABLE

    bReturn = MDrv_XC_GetStatus(pInstance, pDrvStatus, eWindow);
    if(MDrv_XC_Is2K2KToFrc(pInstance))
    {
        pDrvStatus->stDispWin.x = pDrvStatus->stDispWin.x * 2;
        pDrvStatus->stDispWin.width= pDrvStatus->stDispWin.width * 2;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_GetStatus(XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_STATUS XCArgs;
    XCArgs.pDrvStatus = pDrvStatus;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set XC debug level
/// @param  u16DbgSwitch                \b IN: turn on/off debug switch
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetDbgLevel_U2(void* pInstance, MS_U16 u16DbgSwitch, E_XC_MESSAGE_TYPE eMessageType)
{
    //_u16XCDbgSwitch_InternalUseOnly = u16DbgSwitch;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _u16XCDbgSwitch_InternalUseOnly = u16DbgSwitch;
    _eXCDbgSwitch_MessageType = eMessageType;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    pXCResourcePrivate->u16XCDbgSwitch_InternalUseOnly = _u16XCDbgSwitch_InternalUseOnly;
#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    return TRUE;
}

MS_BOOL MApi_XC_SetDbgLevel(MS_U16 u16DbgSwitch, E_XC_MESSAGE_TYPE eMessageType)
{
    if (pu32XCInst == NULL)
    {
        MS_U32 u32deviceID = 0;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_SET_DBG_LEVEL XCArgs;
    XCArgs.u16DbgSwitch = u16DbgSwitch;
    XCArgs.eMessageType = eMessageType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DBG_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get scaler output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetOutputVFreqX100_U2(void* pInstance)
{
    MS_U16 u16OutputVfreqX100;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16OutputVfreqX100 = MDrv_SC_GetOutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u16OutputVfreqX100 = MDrv_SC_GetOutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return u16OutputVfreqX100;
}

MS_U16 MApi_XC_GetOutputVFreqX100(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_OUTPUT_VFREQX100 XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OUTPUT_VFREQX100, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get OP1 output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetOP1OutputVFreqX100_U2(void* pInstance)
{
    MS_U16 u16OutputVfreqX100;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16OutputVfreqX100 = MDrv_SC_GetOP1OutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u16OutputVfreqX100 = MDrv_SC_GetOP1OutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return u16OutputVfreqX100;
}

MS_U16 MApi_XC_GetOP1OutputVFreqX100(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_OP1_OUTPUT_VFREQX100 XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OP1_OUTPUT_VFREQX100, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set NR on/off
/// @param bEn              \b IN: enable/disable NR
/// @param eWindow          \b IN: which window(Main/Sub) is going to get status
//-------------------------------------------------------------------------------------------------
void MApi_XC_Set_NR_U2(void* pInstance, MS_BOOL bEn, SCALER_WIN eWindow)
{
#if TEST_NEW_DYNAMIC_NR
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_Scaler_EnableNR(pInstance, bEn, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_Scaler_EnableNR(pInstance, bEn, eWindow);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
#endif
}

void MApi_XC_Set_NR(MS_BOOL bEn, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_NR XCArgs;
    XCArgs.bEn = bEn;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Create a function for PQ in SC.
/// @param  PQ_Function_Info               \b IN: the information of the function
/// @param  u32InitDataLen          \b IN: the length of the PQ_Function_Info
//-------------------------------------------------------------------------------------------------
void MApi_XC_PQ_LoadFunction_U2(void* pInstance, PQ_Function_Info* function_Info , MS_U32 u32InitDataLen)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(function_Info == NULL)
    {
        MS_ASSERT(0);
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt= NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl = NULL;
#ifdef ENABLE_TV_SC2_PQ
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo_ex = NULL;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex = NULL;
#endif
    }
    else
    {
        // Path create.
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype = function_Info->pq_deside_srctype;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode = function_Info->pq_disable_filmmode;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable = function_Info->pq_load_scalingtable;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling = function_Info->pq_set_420upsampling;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc = function_Info->pq_set_csc;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt= function_Info->pq_get_memyuvfmt;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat = function_Info->pq_set_memformat;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo = function_Info->pq_set_modeInfo;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl = function_Info->pq_ioctl;
#ifdef ENABLE_TV_SC2_PQ
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_deside_srctype_ex = function_Info->pq_deside_srctype_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex = function_Info->pq_disable_filmmode_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex = function_Info->pq_load_scalingtable_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_420upsampling_ex = function_Info->pq_set_420upsampling_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_csc_ex = function_Info->pq_set_csc_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_get_memyuvfmt_ex = function_Info->pq_get_memyuvfmt_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_memformat_ex = function_Info->pq_set_memformat_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_set_modeInfo_ex = function_Info->pq_set_modeInfo_ex;
        s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex = function_Info->pq_ioctl_ex;
#endif
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_PQ_LoadFunction(PQ_Function_Info* function_Info , MS_U32 u32InitDataLen)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PQ_LOADFUNCTION XCArgs;
    XCArgs.function_Info = function_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PQ_LOADFUNCTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_SetOSD2VEMode_U2(void* pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_XC_SetOSD2VEmode(pInstance, eVOPSelOSD_MUX);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_SetOSD2VEMode(EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OSD2VEMODE XCArgs;
    XCArgs.eVOPSelOSD_MUX = eVOPSelOSD_MUX;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSD2VEMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_IP2_PreFilter_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MDrv_SC_IP2_PreFilter_Enable(pInstance, bEnable);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    MDrv_SC_IP2_PreFilter_Enable(pInstance, bEnable);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_IP2_PreFilter_Enable(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_IP2_PREFILTER_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IP2_PREFILTER_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_KeepPixelPointerAppear_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    Hal_SC_enable_cursor_report(pInstance, bEnable);
    pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear = bEnable;
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
    Hal_SC_enable_cursor_report(pInstance, bEnable);
    pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear = bEnable;
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_KeepPixelPointerAppear(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_KEEP_PIXELPOINTER_APPEAR XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_KEEP_PIXELPOINTER_APPEAR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

E_APIXC_GET_PixelRGB_ReturnValue MApi_XC_Get_Pixel_RGB_U2(void* pInstance, XC_Get_Pixel_RGB *pData, SCALER_WIN eWindow)
{
    E_APIXC_GET_PixelRGB_ReturnValue enRet;

    MS_WINDOW_TYPE DispWin;
    MS_WINDOW_TYPE DEWin;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    Hal_SC_get_disp_de_window(pInstance, &DEWin);

    if(pData->enStage == E_XC_GET_PIXEL_STAGE_AFTER_DLC)
        MDrv_XC_get_dispwin_from_reg(pInstance, eWindow,&DispWin );
    else
        MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &DispWin);

    pData->u16x += DEWin.x;
    pData->u16y += DEWin.y;

    if(pData->u16x > DEWin.x + DEWin.width ||
            pData->u16y > DEWin.y + DEWin.height)
    {
        enRet = E_GET_PIXEL_RET_OUT_OF_RANGE;
    }
    else
    {
        if(Hal_SC_get_pixel_rgb(pInstance, pData))
            enRet = E_GET_PIXEL_RET_OK;
        else
            enRet = E_GET_PIXEL_RET_FAIL;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return enRet;
}

E_APIXC_GET_PixelRGB_ReturnValue MApi_XC_Get_Pixel_RGB(XC_Get_Pixel_RGB *pData, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_GET_PIXEL_RET_OUT_OF_RANGE;
    }

    stXC_GET_PIXELRGB XCArgs;
    XCArgs.pData = pData;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_GET_PIXEL_RET_OUT_OF_RANGE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_PIXELRGB, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_GET_PIXEL_RET_OUT_OF_RANGE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_ReportPixelInfo_U2(void* pInstance, MS_XC_REPORT_PIXELINFO *pstRepPixInfo)
{
    MS_BOOL enRet = FALSE;
    MS_WINDOW_TYPE stDEWin;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if(NULL == pstRepPixInfo)
    {
        printf("MApi_XC_ReportPixelInfo: pstRepPixInfo is NULL!!\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE




    Hal_SC_get_disp_de_window(pInstance, &stDEWin);

    if((pstRepPixInfo->u16XStart > pstRepPixInfo->u16XEnd)
            || (pstRepPixInfo->u16XStart >= stDEWin.width)
            || (pstRepPixInfo->u16XEnd >= stDEWin.width)
            || (pstRepPixInfo->u16YStart > pstRepPixInfo->u16YEnd)
            || (pstRepPixInfo->u16YStart >= stDEWin.height)
            || (pstRepPixInfo->u16YEnd >= stDEWin.height)
      )
    {
        printf("Attention! MApi_XC_ReportPixelInfo: Invalid Param! Report Window=(%u,%u,%u,%u), DE Window=(%u,%u,%u,%u)\n",
               pstRepPixInfo->u16XStart, pstRepPixInfo->u16XEnd, pstRepPixInfo->u16YStart, pstRepPixInfo->u16YEnd,
               stDEWin.x, stDEWin.x + stDEWin.width - 1, stDEWin.y, stDEWin.y + stDEWin.height - 1);

#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE

        _XC_RETURN();
#endif //#if SC2_ENABLE

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }

    //add de
    pstRepPixInfo->u16XStart += stDEWin.x;
    pstRepPixInfo->u16XEnd += stDEWin.x;
    pstRepPixInfo->u16YStart += stDEWin.y;
    pstRepPixInfo->u16YEnd += stDEWin.y;

    enRet = Hal_XC_ReportPixelInfo(pInstance, pstRepPixInfo);

    //restore for user
    pstRepPixInfo->u16XStart -= stDEWin.x;
    pstRepPixInfo->u16XEnd -= stDEWin.x;
    pstRepPixInfo->u16YStart -= stDEWin.y;
    pstRepPixInfo->u16YEnd -= stDEWin.y;

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return enRet;
}

MS_BOOL MApi_XC_ReportPixelInfo(MS_XC_REPORT_PIXELINFO *pstRepPixInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_REPORT_PIXEL_INFO XCArgs;
    XCArgs.pstRepPixInfo = pstRepPixInfo;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_REPORT_PIXEL_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

///This is an Obsolete functions, try use MApi_XC_Set_MemFmtEx
MS_BOOL MApi_XC_Set_MemFmt_U2(void* pInstance, MS_XC_MEM_FMT eMemFmt)
{
    MS_BOOL bret = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    if(TRUE == MDrv_XC_Set_MemFmtEx(pInstance, eMemFmt, MAIN_WINDOW))
    {
        bret = MDrv_XC_Set_MemFmtEx(pInstance, eMemFmt, SUB_WINDOW);
    }
    else
    {
        bret = FALSE;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;
}

MS_BOOL MApi_XC_Set_MemFmt(MS_XC_MEM_FMT eMemFmt)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_MEMORY_FORMAT XCArgs;
    XCArgs.eMemFmt = eMemFmt;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MEMORY_FORMAT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_Set_MemFmtEx(void *pInstance, MS_XC_MEM_FMT eMemFmt, SCALER_WIN eWindow)
{
    MS_BOOL bret = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eMemFmt <= E_MS_XC_MEM_FMT_422)
    {
        pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[eWindow] = eMemFmt;
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;
}

MS_BOOL MApi_XC_Set_MemFmtEx_U2(void* pInstance, MS_XC_MEM_FMT eMemFmt, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_Set_MemFmtEx(pInstance, eMemFmt, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_Set_MemFmtEx(pInstance, eMemFmt, eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_Set_MemFmtEx(MS_XC_MEM_FMT eMemFmt, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_MEMORY_FORMAT_EX XCArgs;
    XCArgs.eMemFmt = eMemFmt;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MEMORY_FORMAT_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


MS_BOOL MApi_XC_IsRequestFrameBufferLessMode_U2(void* pInstance)
{
    MS_BOOL bret = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    bret = MDrv_XC_IsRequestFrameBufferLessMode(pInstance);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;
}

MS_BOOL MApi_XC_IsRequestFrameBufferLessMode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_REQUEST_FRAMEBUFFERLESS_MODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set Flag to bypass software reset in MApi_XC_Init()
/// @param  bFlag               \b IN: TRUE: initialize XC by skipping SW reset; FALSE: initialize XC in normal case
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SkipSWReset_U2(void* pInstance, MS_BOOL bFlag)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._bSkipSWReset = bFlag;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._bSkipSWReset = bFlag;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_SkipSWReset(MS_BOOL bFlag)
{
    if (pu32XCInst == NULL)
    {
        MS_U32 u32deviceID = 0;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    if (pu32XC_Kernel_Inst == NULL)
    {
        MS_U32 u32deviceID = 0;
        if(UtopiaOpen(MODULE_XC, &pu32XC_Kernel_Inst, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }


    stXC_SET_SKIP_SWRESET XCArgs;
    XCArgs.bFlag = bFlag;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SET_SKIP_SWRESET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        //     return E_APIXC_RET_FAIL;
    }
    else
    {
        //     return XCArgs.eReturnValue;
    }


    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SKIP_SWRESET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Select which DE to vop
/// @param  bFlag               \b IN: @ref E_OP2VOP_DE_SEL
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_OP2VOPDESel_U2(void* pInstance, E_OP2VOP_DE_SEL eVopDESel)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    eRet = Hal_SC_OP2VOPDESel(pInstance, eVopDESel);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    eRet = Hal_SC_OP2VOPDESel(pInstance, eVopDESel);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eRet;
}

E_APIXC_ReturnValue MApi_XC_OP2VOPDESel(E_OP2VOP_DE_SEL eVopDESel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP2VOP_DE_SELECTION XCArgs;
    XCArgs.eVopDESel = eVopDESel;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OP2VOP_DE_SELECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set Video and OSD Layer at Scaler VOP
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetOSDLayer_U2(void* pInstance, E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    eRet = MDrv_XC_SetOSDLayer(pInstance, eVOPOSDLayer, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eRet;
}

E_APIXC_ReturnValue MApi_XC_SetOSDLayer(E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSDLAYER XCArgs;
    XCArgs.eVOPOSDLayer = eVOPOSDLayer;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSDLAYER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get Video and OSD Layer Enum at Scaler VOP
/// @return Enum value
//-------------------------------------------------------------------------------------------------
E_VOP_OSD_LAYER_SEL MApi_XC_GetOSDLayer_U2(void* pInstance, SCALER_WIN eWindow)
{
    E_VOP_OSD_LAYER_SEL eSelect = E_VOP_LAYER_RESERVED;

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    eSelect = MDrv_XC_GetOSDLayer(pInstance, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eSelect;
}

E_VOP_OSD_LAYER_SEL MApi_XC_GetOSDLayer(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_VOP_LAYER_RESERVED;
    }

    stXC_GET_OSDLAYER XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_VOP_LAYER_RESERVED;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OSDLAYER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_VOP_LAYER_RESERVED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Constant Alpha Value of Video
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetVideoAlpha_U2(void* pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    eRet = MDrv_XC_SetVideoAlpha(pInstance, u8Val, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eRet;
}

E_APIXC_ReturnValue MApi_XC_SetVideoAlpha(MS_U8 u8Val, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VIDEO_ALPHA XCArgs;
    XCArgs.u8Val = u8Val;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VIDEO_ALPHA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get Constant Alpha Value of Video
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_GetVideoAlpha_U2(void* pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    eRet = MDrv_XC_GetVideoAlpha(pInstance, pu8Val, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eRet;

}

E_APIXC_ReturnValue MApi_XC_GetVideoAlpha(MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_VIDEO_ALPHA XCArgs;
    XCArgs.pu8Val = pu8Val;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VIDEO_ALPHA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get field packing mode support status
/// @return TRUE(success) or FALSE(fail)
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsFieldPackingModeSupported_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    bRet = MDrv_XC_Get_SCMI_Type();
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bRet;
}

MS_BOOL MApi_XC_IsFieldPackingModeSupported(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FIELDPACKINGMODE_SUPPORTED XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FIELDPACKINGMODE_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SkipWaitVsync_U2( void* pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_SC_SetSkipWaitVsync(pInstance, eWindow, bIsSkipWaitVsyn);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_SC_SetSkipWaitVsync(pInstance, eWindow, bIsSkipWaitVsyn);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_SkipWaitVsync( MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_XC_KERNEL, &pu32XC_Kernel_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen Kernel XC failed\n");
            return FALSE;
        }
    }

    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SKIP_WAIT_VSYNC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bIsSkipWaitVsyn = bIsSkipWaitVsyn;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// Get Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_SkipWaitVsync_U2( void* pInstance, MS_BOOL eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


    return MDrv_SC_GetSkipWaitVsync(pInstance, eWindow);
}

MS_BOOL MApi_XC_Get_SkipWaitVsync( MS_BOOL eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SKIP_WAIT_VSYNC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bIsSkipWaitVsyn = FALSE;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the VGA SOG on or off
/// @param  bEnable                \b IN: bEnable =1, Turn on the VGA SOG; bEnable =0, Turn off the VGA SOG
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetVGASogEn_U2(void* pInstance, MS_BOOL bVGASogEn)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_SetVGASogEn(pInstance, bVGASogEn);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_XC_SetVGASogEn(pInstance, bVGASogEn);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_SetVGASogEn(MS_BOOL bVGASogEn)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VGA_SOG_ENABLE XCArgs;
    XCArgs.bVGASogEn = bVGASogEn;
    XCArgs.eReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VGA_SOG_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set pixel shift
/// @param  s8H               \b IN: pixel shift H
/// @param  s8V               \b IN: pixel shift V
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
//Support Maximum +-4 adjust
#define MAX_V_ADJUST_VALUE  (4)
//The max jump is from -4 to +4, means jump 8
#define MAX_V_ADJUST_STEP   (MAX_V_ADJUST_VALUE<<1)
E_APIXC_ReturnValue MApi_XC_SetPixelShift_U2(void* pInstance, MS_S8 s8H, MS_S8 s8V)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    //printf("MApi_XC_SetPixelShift(H=%d,V=%d)\n", s8H, s8V);
    if((s8V != 0) && (gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart < 8))
    {
        //If first adjust V, and default vstart <8, reset all XC window setting
        MS_WINDOW_TYPE stDEWin;
        memcpy(&stDEWin, &(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), sizeof(MS_WINDOW_TYPE));
        gSrcInfo[MAIN_WINDOW].stDispWin.y -= stDEWin.y;
        gSrcInfo[SUB_WINDOW].stDispWin.y -= stDEWin.y;
        stDEWin.y = MAX_V_ADJUST_STEP;
        gSrcInfo[MAIN_WINDOW].stDispWin.y += stDEWin.y;
        gSrcInfo[SUB_WINDOW].stDispWin.y += stDEWin.y;
        MDrv_XC_UpdateWindow(pInstance, &stDEWin, &(gSrcInfo[MAIN_WINDOW].stDispWin), &(gSrcInfo[SUB_WINDOW].stDispWin), &stDEWin, NULL);
    }

    if ((s8H >= -MAX_V_ADJUST_VALUE) && (s8H <= MAX_V_ADJUST_VALUE) && (s8V >= -MAX_V_ADJUST_VALUE) && (s8V <= MAX_V_ADJUST_VALUE))
    {
        if (MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) == MIRROR_HV)
        {
            s8H = s8H * -1;
            s8V = s8V * -1;
            printf("Mirror! H=%d,V=%d\n", s8H, s8V);
        }

        if((s8H == 0) && (s8V == 0))
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, 0, BIT(15));
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15), BIT(15));

        eRet = MDrv_SC_ShiftDisplayWindowDirectly(pInstance, s8H, s8V, pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift, pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift);
        pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift = s8H;
        pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift = s8V;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift = 0;
        pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift = 0;
        printf("Error: Mapp_SetPixelShift(%d, %d): Invalid value range.\n", s8H, s8V);
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return eRet;
}

E_APIXC_ReturnValue MApi_XC_SetPixelShift(MS_S8 s8H, MS_S8 s8V)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_PIXEL_SHIFT XCArgs;
    XCArgs.s8H = s8H;
    XCArgs.s8V = s8V;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PIXEL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the specific window for traveling mode(VE capture function)
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen         \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetTravelingWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //The component 1080i can't frame lock when capture start is odd.
    if(pstXC_SetWin_Info->bInterlace)
    {
        // the Capture start should be even
        if(pstXC_SetWin_Info->stCapWin.y%2)
        {
            pstXC_SetWin_Info->stCapWin.y += 1;
        }
        pstXC_SetWin_Info->stCapWin.height = pstXC_SetWin_Info->stCapWin.height & (~0x01);
    }
    pstXC_SetWin_Info->stCapWin.width = pstXC_SetWin_Info->stCapWin.width & (~0x01);


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // copy user data to internal data
    // here is a patch first, will remove after seperate Internal XC status with set window information
    //u32CopyLen = sizeof(XC_SETWIN_INFO) - (sizeof(MS_BOOL)*2) - (sizeof(MS_U16)*4);
    //memcpy(&gSrcInfo[eWindow], pstXC_SetWin_Info, u32CopyLen);
    //do not use memcpy(), strongly depend on structure definition
    _Mdrv_XC_CopySetWinInfo(pInstance, pstXC_SetWin_Info, eWindow);

    gSrcInfo[eWindow].stDispWin.x += gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16HStart;
    gSrcInfo[eWindow].stDispWin.y += gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16VStart;

    // dump debug msg
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= SetTravelingWindow Start (Window : %u, src: %u) =============\n",eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CropWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"DispWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stDispWin.x,gSrcInfo[eWindow].stDispWin.y, gSrcInfo[eWindow].stDispWin.width, gSrcInfo[eWindow].stDispWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Panel   x: %4u  y: %4u  w: %4u  h: %4u \n",
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Mirror/Interlace/Hdup = %u/%u/%u\n", IsVMirrorMode(eWindow), pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"H/V total = (%u, %u), VFreq = %u\n", pstXC_SetWin_Info->u16DefaultHtotal, pstXC_SetWin_Info->u16InputVTotal, pstXC_SetWin_Info->u16InputVFreq);

    // Scaler must update source before width / height checking.
    MApi_XC_Set_PQ_SourceData(pInstance, eWindow, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow]);

    // Capture width & height can not be 0 !!
    if ((gSrcInfo[eWindow].stCapWin.width < 0x20) || (gSrcInfo[eWindow].stCapWin.height < 0x20) ||
            (gSrcInfo[eWindow].stCapWin.x > 0x7FF)    || (gSrcInfo[eWindow].stCapWin.y > 0x7FF))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Cap Width/Height can not be smaller than 0x20, or X/Y can not bigger than 0x7FF \n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return FALSE;
    }
    /* //Useless code
    #if ENABLE_REQUEST_FBL
        if(s_PQ_Function_Info.pq_ioctl)
        {
            s_PQ_Function_Info.pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_RFBL_CTRL, NULL, NULL);
        }
    #endif
    */

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MDrv_sc_set_capture_window(pInstance, eWindow );//Fine tune capture window
    Hal_SC_ip_set_capture_h_start(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapStart, eWindow); //Set the tuned capture window
    Hal_SC_ip_set_capture_v_start(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart, eWindow);
    Hal_SC_ip_set_capture_h_size (pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize , eWindow);
    Hal_SC_ip_set_capture_v_size (pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapSize , eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    MDrv_sc_set_capture_window(pInstance, eWindow );//Fine tune capture window
    Hal_SC_ip_set_capture_h_start(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapStart, eWindow); //Set the tuned capture window
    Hal_SC_ip_set_capture_v_start(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart, eWindow);
    Hal_SC_ip_set_capture_h_size (pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize , eWindow);
    Hal_SC_ip_set_capture_v_size (pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapSize , eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE



    if (IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType)   ||
            IsSrcTypeYPbPr(gSrcInfo[eWindow].enInputSourceType) ||
            IsSrcTypeScart(gSrcInfo[eWindow].enInputSourceType) )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"ADC Set Mode \n");
        MDrv_XC_ADC_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    msAPI_Scaler_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);
    /*
        if((s_PQ_Function_Info.pq_set_csc == NULL) || (s_PQ_Function_Info.pq_set_memformat == NULL))
        {
            Hal_SC_set_csc(TRUE, eWindow);
            Hal_SC_set_444To422(ENABLE, eWindow);
            gSrcInfo[eWindow].bUseYUVSpace = TRUE;
            gSrcInfo[eWindow].bMemYUVFmt = TRUE;
        }
    */
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= SetTravelingWindow Done (Window : %u, src: %u) =============\n",eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return TRUE;
}

MS_BOOL MApi_XC_SetTravelingWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_TRAVELING_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_TRAVELING_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Set_Zorder_MLoadEn
/// @param  bEnable               \b IN: whether to menuload
/// Do an example when you want to menuload OSDLayer :
/// MApi_XC_Set_Zorder_MLoadEn(TRUE)
/// call following function
/// --->MApi_XC_SetOSDLayerBlending()
/// --->MApi_XC_SetOSDLayerAlpha()
/// MApi_XC_Set_Zorder_MLoadEn(FALSE)
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Set_Zorder_MLoadEn_U2(void* pInstance, MS_BOOL bEn)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    MDrv_XC_Set_Zorder_MLoadEn(pInstance, bEn);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Set_Zorder_MLoadEn(MS_BOOL bEn)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_ZORDER_MLOADEN stXCArgs;
    stXCArgs.bEn = bEn;
    stXCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSD_LAYER_BLEANDING, (void*)&stXCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return stXCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// MApi_XC_SetVideoOnOSD
/// @param  E_VDO_ON_OSD_LAYER               \b IN: set video show on which osd layer
/// @param  eWindow                                     \b IN: set main or sub video data to memory
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetVideoOnOSD_U2(void* pInstance, E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_SetVideoOnOSD(pInstance, enlayer, eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    bRet = MDrv_SC_SetVideoOnOSD(pInstance, enlayer, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bRet;

}

E_APIXC_ReturnValue MApi_XC_SetVideoOnOSD(E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VIDEO_ON_OSD XCArgs;
    XCArgs.enlayer = enlayer;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VIDEO_ON_OSD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_SetOSDLayerBlending
/// @param  u8Layer               \b IN: which osd layer will blend
/// @param  bEnable               \b IN: whether to blend
/// @param  eWindow               \b IN: which window blend with
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetOSDLayerBlending_U2(void* pInstance, MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_SetOSDLayerBlending(pInstance, u8Layer, bEnable, eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    bRet = MDrv_SC_SetOSDLayerBlending(pInstance, u8Layer, bEnable, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bRet;
}

E_APIXC_ReturnValue MApi_XC_SetOSDLayerBlending(MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSD_LAYER_BLENDING stXCArgs;
    stXCArgs.u8Layer = u8Layer;
    stXCArgs.bEnable = bEnable;
    stXCArgs.eWindow = eWindow;
    stXCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSD_LAYER_BLEANDING, (void*)&stXCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return stXCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// MApi_XC_SetOSDLayerAlpha
/// @param  u8Layer                 \b IN: which osd layer will blend
/// @param  u16Alpha                \b IN: blending alpha
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetOSDLayerAlpha_U2(void* pInstance, MS_U8 u8Layer, MS_U8 u8Alpha)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_SetOSDLayerAlpha(pInstance, u8Layer, u8Alpha);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    bRet = MDrv_SC_SetOSDLayerAlpha(pInstance, u8Layer, u8Alpha);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bRet;
}

E_APIXC_ReturnValue MApi_XC_SetOSDLayerAlpha(MS_U8 u8Layer, MS_U8 u8Alpha)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSD_LAYER_ALPHA stXCArgs;
    stXCArgs.u8Layer = u8Layer;
    stXCArgs.u8Alpha = u8Alpha;
    stXCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSD_LAYER_ALPHA, (void*)&stXCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return stXCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// MApi_XC_SetOSDBlendingFormula
/// @param  enOsdIndex                              \b IN: osd index
/// @param  enType                                  \b IN: osd blending type
/// @param  eWindow                                 \b IN: blending with main or sub video
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetOSDBlendingFormula_U2(void* pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_OK;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_SetOSDBlendingFormula(pInstance, enOsdIndex, enType, eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    bRet = MDrv_SC_SetOSDBlendingFormula(pInstance, enOsdIndex, enType, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bRet;
}

E_APIXC_ReturnValue MApi_XC_SetOSDBlendingFormula(E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSD_BLENDING_FORMULA XCArgs;
    XCArgs.enOsdIndex = enOsdIndex;
    XCArgs.enType = enType;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OSD_BLENDING_FORMULA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_SetScaling
/// @param  bEnable                               \b IN: set enable or disable
/// @param  eScaling_type                      \b IN: set scaling type, pre-scaling, post-scaling, both-scaling
/// @param  eVector_type                       \b IN: set H vector, v vector, and HV vector
/// @param  eWindow                             \b IN: set main or sub video data to memory
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetScaling_U2( void* pInstance, MS_BOOL bEnable, E_XC_SCALING_TYPE eScaling_type, E_XC_VECTOR_TYPE eVector_type, SCALER_WIN eWindow )
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eScaling_type == E_XC_PRE_SCALING)
    {
        if(eWindow == MAIN_WINDOW)
        {
            switch(eVector_type)
            {
                case E_XC_H_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, bEnable<<15 , BIT(15) );
                    break;
                case E_XC_V_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, bEnable<<15 , BIT(15) );
                    break;
                default:
                case E_XC_HV_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, bEnable<<15 , BIT(15) );
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, bEnable<<15 , BIT(15) );
                    break;
            }
        }
        else
        {
            switch(eVector_type)
            {
                case E_XC_H_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L, bEnable<<15 , BIT(15) );
                    break;
                case E_XC_V_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L, bEnable<<15 , BIT(15) );
                    break;
                default:
                case E_XC_HV_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L, bEnable<<15 , BIT(15) );
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L, bEnable<<15 , BIT(15) );
                    break;
            }
        }
    }
    else if(eScaling_type == E_XC_POST_SCALING)
    {
        if(eWindow == MAIN_WINDOW)
        {
            switch(eVector_type)
            {
                case E_XC_H_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, bEnable<<8 , BIT(8) );
                    break;
                case E_XC_V_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, bEnable<<8 , BIT(8) );
                    break;
                default:
                case E_XC_HV_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, bEnable<<8 , BIT(8) );
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, bEnable<<8 , BIT(8) );
                    break;
            }
        }
        else
        {
            switch(eVector_type)
            {
                case E_XC_H_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_28_L, bEnable<<8 , BIT(8) );
                    break;
                case E_XC_V_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_2A_L, bEnable<<8 , BIT(8) );
                    break;
                default:
                case E_XC_HV_VECTOR:
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_28_L, bEnable<<8 , BIT(8) );
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_2A_L, bEnable<<8 , BIT(8) );
                    break;
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_SetScaling( MS_BOOL bEnable, E_XC_SCALING_TYPE eScaling_type, E_XC_VECTOR_TYPE eVector_type, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SCALING XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eScaling_type = eScaling_type;
    XCArgs.eVector_type = eVector_type;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_SetMCDIBufferAddress
/// @param  u32FBAddress                              \b IN: frame buffer base addr
/// @param  u32FBSize                                   \b IN: frame buffer size
/// @param  eType                                         \b IN: set frame buffer for MCDI ME1 or ME2
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetMCDIBufferAddress_U2(void* pInstance, MS_PHYADDR u32FBAddress, MS_PHYADDR u32FBSize, E_XC_MCDI_TYPE eType)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_SC_SetMCDIBufferAddress(pInstance, u32FBAddress, u32FBSize, eType);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_SC_SetMCDIBufferAddress(pInstance, u32FBAddress, u32FBSize, eType);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_SetMCDIBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, E_XC_MCDI_TYPE eType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MCDI_BUFFERADDRESS XCArgs;
    XCArgs.u32FBAddress = u32FBAddress;
    XCArgs.u32FBSize = u32FBSize;
    XCArgs.eType = eType;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MCDI_BUFFERADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_EnableMCDI
/// @param  bEnable                                      \b IN: enable/disable MCDI
/// @param  eType                                         \b IN: control MCDI ME1 or ME2 or both
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableMCDI_U2(void* pInstance, MS_BOOL bEnable, E_XC_MCDI_TYPE eType)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_SC_EnableMCDI(pInstance, bEnable, eType);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_SC_EnableMCDI(pInstance, bEnable, eType);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_EnableMCDI(MS_BOOL bEnable, E_XC_MCDI_TYPE eType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MCDI_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eType = eType;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MCDI_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_SendCmdToFRC
/// @param  u8Cmd                                      \b IN: CMD
/// @param  count                                         \b IN: parameter Counter
/// @param  pFRC_R2_Para                           \b IN: input parameter
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SendCmdToFRC_U2( void* pInstance, MS_U8 u8Cmd, MS_U8 count, FRC_R2_CMD_PARAMETER_t pFRC_R2_Para )
{
    MS_BOOL bret = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    bret = MDrv_XC_SendCmdToFRC(pInstance, u8Cmd, count, pFRC_R2_Para);

#if (HW_DESIGN_3D_VER == 4)
    // for 2Dto3D case, the FRC FICLK need to be changed
    //E_MAPI_FRC_MB_CMD_SET_2D_TO_3D_MODE = 0x40,  ParaCnt=5,
    //          P0 = 2D to 3D ConversionMode(0=Disable, 1=Enable)
    //          P1 = 3D Depth Gain(0~31)
    //          P2 = 3D Depth Offset(0~127)
    //          P3 = Artificial Gain(0~15)
    //          P4 = L/R Swap (0=Normal, 1=L/R Swap)
    if(u8Cmd == 0x40)
    {
        if(pFRC_R2_Para.p1==ENABLE)
        {
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0004, 0x0004); // clk_idclk_frc
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
        }
        else
        {
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0004); // clk_fdclk_frc
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
            W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
        }
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;
}

MS_BOOL MApi_XC_SendCmdToFRC( MS_U8 u8Cmd, MS_U8 count, FRC_R2_CMD_PARAMETER_t pFRC_R2_Para )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SEND_CMD2FRC XCArgs;
    XCArgs.u8Cmd = u8Cmd;
    XCArgs.count = count;
    XCArgs.pFRC_R2_Para = pFRC_R2_Para;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SEND_CMD2FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_GetMsgFromFRC
/// @param  u8Cmd                                      \b IN: CMD
/// @param  pu8ParaCount                            \b IN: Counter
/// @param  pu8Para                                     \b IN: input parameter
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetMsgFromFRC_U2(void* pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para)
{
    MS_BOOL bret = FALSE;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    bret = MDrv_XC_GetMsgFromFRC(pInstance, pu8Cmd, pu8ParaCount, pu8Para);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bret;

}

MS_BOOL MApi_XC_GetMsgFromFRC(MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_MSG_FROM_FRC XCArgs;
    XCArgs.pu8Cmd = pu8Cmd;
    XCArgs.pu8ParaCount = pu8ParaCount;
    XCArgs.pu8Para = pu8Para;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MSG_FROM_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set XC bandwidth saving X-mode
/// @param  bEnable                   \b IN: enable or disable
/// @param  eWindow                 \b IN: which window we are going to enable or disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_Set_BWS_Mode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    Hal_SC_set_bws_mode(pInstance, bEnable, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_Set_BWS_Mode(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BWS_MODE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_SET_BWS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_ReadFrameBufferFrameCount_U2(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    MDrv_XC_Set_ReadFrameBufferFrameCount(pInstance, value, eWindow);

}
void MApi_XC_Set_ReadFrameBufferFrameCount_FRC_U2(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    MDrv_XC_Set_ReadFrameBufferFrameCount_FRC(pInstance, value, eWindow);
}

void MApi_XC_Set_RWPointerDiff_U2(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    MDrv_XC_Set_RWPointerDiff(pInstance, value, eWindow);
}
void MApi_XC_Set_RWPointerDiff_FRC_U2(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    MDrv_XC_Set_RWPointerDiff_FRC(pInstance, value, eWindow);
}

MS_U16 MApi_XC_Get_ReadFrameBufferFrameCount_U2(void* pInstance, SCALER_WIN eWindow)
{
    return MDrv_XC_Get_ReadFrameBufferFrameCount(pInstance, eWindow);
}
MS_U16 MApi_XC_Get_ReadFrameBufferFrameCount_FRC_U2(void* pInstance, SCALER_WIN eWindow)
{
    return MDrv_XC_Get_ReadFrameBufferFrameCount_FRC(pInstance, eWindow);
}

MS_U16 MApi_XC_Get_RWPointerDiff_U2(void* pInstance,  SCALER_WIN eWindow)
{
    return MDrv_XC_Get_RWPointerDiff(pInstance, eWindow);
}
MS_U16 MApi_XC_Get_RWPointerDiff_FRC_U2(void* pInstance, SCALER_WIN eWindow)
{
    return MDrv_XC_Get_RWPointerDiff_FRC(pInstance, eWindow);
}

void MApi_XC_Set_ReadFrameBufferFrameCount(MS_U16 u16Value, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_RFBFC XCArgs;
    XCArgs.u16Value = u16Value;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RFBFC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}
void MApi_XC_Set_ReadFrameBufferFrameCount_FRC(MS_U16 u16Value, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_RFBFC_FRC XCArgs;
    XCArgs.u16Value = u16Value;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RFBFC_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

void MApi_XC_Set_RWPointerDiff(MS_U16 u16Value, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_RWPD XCArgs;
    XCArgs.u16Value = u16Value;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RWPD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}
void MApi_XC_Set_RWPointerDiff_FRC(MS_U16 u16Value, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_RWPD_FRC XCArgs;
    XCArgs.u16Value = u16Value;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RWPD_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

MS_U16 MApi_XC_Get_ReadFrameBufferFrameCount(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_RFBFC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RFBFC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }

}
MS_U16 MApi_XC_Get_ReadFrameBufferFrameCount_FRC(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_RFBFC_FRC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RFBFC_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }

}

MS_U16 MApi_XC_Get_RWPointerDiff(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_RWPD XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RWPD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }

}
MS_U16 MApi_XC_Get_RWPointerDiff_FRC(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_RWPD_FRC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RWPD_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set xc FRC ColorPathCtrl
/// @param  epath_type               \b IN: Select the path
/// @param  bEnable                    \b IN: enable or disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_FRC_ColorPathCtrl_U2( void* pInstance, MS_U16 u16Path_sel, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if(u16Path_sel & FRC_PATHCTRL_OP2COLORMATRIX)
    {
        MDrv_FRC_OP2_ColorMatrixEn(pInstance, bEnable);
    }
    if(u16Path_sel & FRC_PATHCTRL_OP2CSCDITHER)
    {
        MDrv_FRC_OP2_CscDitherEn(pInstance, bEnable);
    }
    if(u16Path_sel & FRC_PATHCTRL_OP2BRIGHTNESS)
    {
        MDrv_FRC_OP2_BrightnessEn(pInstance, bEnable);
    }
    if(u16Path_sel & FRC_PATHCTRL_OP2CONTRAST)
    {
        MDrv_FRC_OP2_ContrastEn(pInstance, bEnable);
    }
    if(u16Path_sel & FRC_PATHCTRL_OP2NOISEROUND)
    {
        MDrv_FRC_OP2_NoiseRoundEn(pInstance, bEnable);
    }

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_FRC_ColorPathCtrl( MS_U16 u16Path_sel, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_COLOR_PATH_CONTROL XCArgs;
    XCArgs.u16Path_sel = u16Path_sel;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_COLOR_PATH_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set XC FRC_OP2 SetRGBGain
/// @param  u16RedGain                 \b IN: Red Gain
/// @param  u16GreenGain              \b IN: Green Gain
/// @param  u16BlueGain                \b IN: Blue Gain
//-------------------------------------------------------------------------------------------------
void MApi_XC_FRC_OP2_SetRGBGain_U2(void* pInstance, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_FRC_OP2_SetRGBGain(pInstance, u16RedGain,  u16GreenGain,  u16BlueGain);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_FRC_OP2_SetRGBGain(MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_OP2_RGBGAIN XCArgs;
    XCArgs.u16RedGain = u16RedGain;
    XCArgs.u16GreenGain = u16GreenGain;
    XCArgs.u16BlueGain = u16BlueGain;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_OP2_RGBGAIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set XC FRC_OP2 SetRGBOffset
/// @param  u16RedOffset                 \b IN: Red Offset
/// @param  u16GreenOffset              \b IN: Green Offset
/// @param  u16BlueOffset                \b IN: Blue Offset
//-------------------------------------------------------------------------------------------------
void MApi_XC_FRC_OP2_SetRGBOffset_U2(void* pInstance, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_FRC_OP2_SetRGBOffset(pInstance, u16RedOffset,  u16GreenOffset,  u16BlueOffset);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_FRC_OP2_SetRGBOffset(MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_OP2_RGBOFFSET XCArgs;
    XCArgs.u16RedOffset = u16RedOffset;
    XCArgs.u16GreenOffset = u16GreenOffset;
    XCArgs.u16BlueOffset = u16BlueOffset;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_OP2_RGBOFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// set XC FRC_OP2 SetDither
/// @param  u16dither                 \b IN: Dither setting
//-------------------------------------------------------------------------------------------------
void MApi_XC_FRC_OP2_SetDither_U2(void* pInstance, MS_U16 u16dither)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MDrv_FRC_OP2_SetDither(pInstance, u16dither);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_FRC_OP2_SetDither(MS_U16 u16dither)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_OP2_DITHER XCArgs;
    XCArgs.u16dither = u16dither;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_OP2_DITHER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_ForceReadFrame
/// @param  bEnable                                 \b IN: enable/disable keep specified frame
/// @param  u16FrameIndex                           \b IN: control specified frame to show
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_ForceReadFrame_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16FrameIndex)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

#if SC2_ENABLE
    _XC_ENTRY(pInstance);

    bRet = MDrv_SC_ForceReadFrame(pInstance, bEnable, u16FrameIndex);

    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();

    bRet = MDrv_SC_ForceReadFrame(pInstance, bEnable, u16FrameIndex);

    _XC_RETURN();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return bRet;
}

E_APIXC_ReturnValue MApi_XC_ForceReadFrame(MS_BOOL bEnable, MS_U16 u16FrameIndex)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_FORCE_READ_FRAME XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u16FrameIndex = u16FrameIndex;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FORCE_READ_FRAME, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


E_APIXC_ReturnValue MApi_XC_OSDC_InitSetting_U2(void* pInstance,
        E_XC_OSDC_TGEN_Type e_osdctgen_type,
        MS_XC_OSDC_TGEN_INFO *pstOC_Tgen_Cus,
        MS_XC_OSDC_CTRL_INFO *pstOC_Ctrl)
{

    if(!SUPPORT_OSD_HSLVDS_PATH)
    {
        printf("Please check the osdc capability \n");
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    gSrcInfo[MAIN_WINDOW].Status2.bIsInitOSDC = TRUE;

    if(e_osdctgen_type == E_XC_OSDC_TGEN_MANUAL)
    {
        if(NULL == pstOC_Tgen_Cus)
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
        }

        memcpy(&gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING, pstOC_Tgen_Cus, sizeof(XC_OSDC_TIMING));
        memcpy(&gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL,  pstOC_Ctrl,     sizeof(XC_OSDC_MSIC_CTRL));
    }
    else
    {
        switch(e_osdctgen_type)
        {
            case E_XC_OSDC_TGEN_1366x768:
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal        = 0x618;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal        = 0x326;

                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_Start   = 0x04;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_End     = 0x06;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_Start = 0x34;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_End   = 0x589;

                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_Start   = 0x04;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_End     = 0x06;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_Start = 0x10;
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_End   = 0x309;
                break;

            case E_XC_OSDC_TGEN_1920x1080:
                if(pstOC_Ctrl->u16OC_Lpll_type == E_XC_PNL_LPLL_HS_LVDS)
                {
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal        = 0x7B4; //0x800;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal        = 0x448; //0x500;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_Start = 0x34;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_End   = 0x7B3;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_Start = 0x10;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_End   = 0x447;
                }
                else
                {
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal        = 0x7FF; //should be odd(total cycle: htt+1),
                    //vby one tx need even
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal        = 0x500;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_Start = 0x34;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_End   = 0x7B3;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_Start = 0x10;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_End   = 0x447;

                }
                break;
            case E_XC_OSDC_TGEN_3840x2160:
                if(pstOC_Ctrl->u16OC_Lpll_type == E_XC_PNL_LPLL_HS_LVDS)
                {
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal        = 0xFFF;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal        = 0x982;//0xA00;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_Start = 0x34;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_End   = 0xF33;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_Start = 0x10;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_End   = 0x87F;
                }
                else
                {
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal        = 0x10FF;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal        = 0x900;//0xA00;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Hsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_Start = 0x34;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HframDE_End   = 0xF33;

                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_Start   = 0x04;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_Vsync_End     = 0x06;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_Start = 0x10;
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VframDE_End   = 0x87F;
                }
                break;

            case E_XC_OSDC_TGEN_MAX:
            default:
                break;
        }
        memcpy(&gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL, pstOC_Ctrl, sizeof(XC_OSDC_MSIC_CTRL));
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_OSDC_InitSetting(E_XC_OSDC_TGEN_Type e_osdctgen_type,
        MS_XC_OSDC_TGEN_INFO *pstOC_Tgen_Cus,
        MS_XC_OSDC_CTRL_INFO *pstOC_Ctrl)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_OSDC_INITSETTING XCArgs;
    XCArgs.e_osdctgen_type = e_osdctgen_type;
    XCArgs.pstOC_Tgen_Cus = pstOC_Tgen_Cus;
    XCArgs.pstOC_Ctrl = pstOC_Ctrl;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OSDC_INITSETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_XC_OSDC_SetOutVfreqx10_U2(void* pInstance, MS_U16 u16Vfreq)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ = u16Vfreq;
    printf("OSDC_FREQ=%u\n",pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ = u16Vfreq;
    printf("OSDC_FREQ=%u\n",pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_OSDC_SetOutVfreqx10(MS_U16 u16Vfreq)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_OSDC_SET_OUTPUTVFREQX10 XCArgs;
    XCArgs.u16Vfreq = u16Vfreq;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OSDC_SET_OUTPUTVFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

E_APIXC_ReturnValue MApi_XC_OSDC_Control_U2(void* pInstance, MS_U32 eCtrl_type)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LPLL_Type = 0;
    u8LPLL_Type =(MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;

    if((u8LPLL_Type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)&&(u8LPLL_Type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
    {
#ifdef SUPPORT_MOD_ADBANK_SEPARATE

#else
        //Close the RT51 OSDC handshack code flow
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(3), BIT(3));
#endif
    }

    eReturn  = MDrv_XC_OSDC_Control(pInstance, eCtrl_type);

    if((u8LPLL_Type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)&&(u8LPLL_Type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
    {
#ifdef SUPPORT_MOD_ADBANK_SEPARATE

#else
        //Open the RT51 OSDC handshack code flow
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, 0, BIT(3));
#endif
    }


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_OSDC_Control(MS_U32 eCtrl_type)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_OSDC_CONTROL XCArgs;
    XCArgs.eCtrl_type = eCtrl_type;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OSDC_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_OSDC_GetDstInfo_U2(void* pInstance, MS_OSDC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    XC_OSDC_TIMING stOC_Tgen;
    XC_OSDC_MSIC_CTRL stOC_Ctrl;

    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    if(NULL == pDstInfo)
    {
        printf("MApi_XC_OSDC_GetDstInfo():pDstInfo is NULL\n");
         return E_APIXC_RET_FAIL;
    }
    // the version control is coming in with version 1
    if(pDstInfo->ODSC_DISPInfo_Version < 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_OSDC_GetDstInfo: please check your ODSC_DISPInfo_Version, it should not set to 0!!\n")
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return E_APIXC_RET_FAIL;
    }


    if(u32SizeofDstInfo != sizeof(MS_OSDC_DST_DispInfo))
    {
        printf("MApi_XC_OSDC_GetDstInfo():u16SizeofDstInfo is different from the MS_XC_OSDC_DST_DispInfo defined, check header file!\n");
        eRet = E_APIXC_RET_FAIL;
    }
    else if(MDrv_XC_OSDC_GetDataFromRegister(pInstance, &stOC_Tgen, &stOC_Ctrl))
    {
        pDstInfo->VDTOT                = stOC_Tgen.u16OC_Tgen_VTotal;
        pDstInfo->DEVST                = stOC_Tgen.u16OC_Tgen_VframDE_Start;
        pDstInfo->DEVEND               = stOC_Tgen.u16OC_Tgen_VframDE_End;
        pDstInfo->HDTOT                = stOC_Tgen.u16OC_Tgen_HTotal;
        pDstInfo->DEHST                = stOC_Tgen.u16OC_Tgen_HframDE_Start;
        pDstInfo->DEHEND               = stOC_Tgen.u16OC_Tgen_HframDE_End;

        pDstInfo->SYNCHST              = stOC_Tgen.u16OC_Tgen_Hsync_Start;
        pDstInfo->SYNCHEND             = stOC_Tgen.u16OC_Tgen_Hsync_End;
        pDstInfo->SYNCVST              = stOC_Tgen.u16OC_Tgen_Vsync_Start;
        pDstInfo->SYNCVEND             = stOC_Tgen.u16OC_Tgen_Vsync_End;

        pDstInfo->bCLK_EN              = stOC_Ctrl.bOC_ClK_En;
        pDstInfo->bMIXER_BYPASS_EN     = stOC_Ctrl.bOC_Mixer_Bypass_En;
        pDstInfo->bMIXER_INVALPHA_EN   = stOC_Ctrl.bOC_Mixer_InvAlpha_En;
        pDstInfo->bMIXER_HSTVFDEOUT_EN = stOC_Ctrl.bOC_Mixer_Hsync_Vfde_Out;
        pDstInfo->bMIXER_HFDEVFDEOUT_EN= stOC_Ctrl.bOC_Mixer_Hfde_Vfde_Out;

        eRet = E_APIXC_RET_OK;
    }
    else
    {
        pDstInfo->VDTOT                = 0;
        pDstInfo->HDTOT                = 0;
        pDstInfo->DEHST                = 0;
        pDstInfo->DEHEND               = 0;
        pDstInfo->DEVST                = 0;
        pDstInfo->DEVEND               = 0;

        pDstInfo->SYNCHST              = 0;
        pDstInfo->SYNCHEND             = 0;
        pDstInfo->SYNCVST              = 0;
        pDstInfo->SYNCVEND             = 0;

        pDstInfo->bCLK_EN              = DISABLE;
        pDstInfo->bMIXER_BYPASS_EN     = DISABLE;
        pDstInfo->bMIXER_INVALPHA_EN   = DISABLE;
        pDstInfo->bMIXER_HSTVFDEOUT_EN = DISABLE;
        pDstInfo->bMIXER_HFDEVFDEOUT_EN= DISABLE;

        eRet = E_APIXC_RET_FAIL;
    }

    return eRet;
}

E_APIXC_ReturnValue MApi_XC_OSDC_GetDstInfo(MS_OSDC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_OSDC_GET_DESTINATION_INFO XCArgs;
    XCArgs.pDstInfo = pDstInfo;
    XCArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OSDC_GET_DESTINATION_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#define LOGTIMECOST  FALSE
MS_U32 MApi_XC_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState)
{
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    MS_U8 i = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
#if LOGTIMECOST
    MS_U32 u32Begin = MsOS_GetSystemTime();
#endif

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (enPowerState == E_POWER_SUSPEND)
    {
        pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState = enPowerState;
        pXCResourcePrivate->stdrvXC_MVideo._bSkipSWReset = FALSE;
        MsOS_DisableInterrupt(E_INT_IRQ_DISP);
        MsOS_DetachInterrupt(E_INT_IRQ_DISP);
        u32Ret = UTOPIA_STATUS_SUCCESS;
    }
    else if (enPowerState == E_POWER_RESUME)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState == E_POWER_SUSPEND)
        {
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[MAIN_WINDOW] = 0xFFFFFFFF;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[MAIN_WINDOW] = 0x0;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[SUB_WINDOW] = 0xFFFFFFFF;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[SUB_WINDOW] = 0x0;
#if FRC_INSIDE
            pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited = FALSE;
#endif

            _MApi_XC_Init_WithoutCreateMutex(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitDataLen_Suspend);

            MDrv_XC_OSDC_Control(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32OSDCCtrltype_Suspend);

            MDrv_XC_Init_MISC(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitMiscDataLen_Suspend);

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadBufByteLen_Suspend > 0)
            {

#if SC2_ENABLE
                _MLOAD_ENTRY(pInstance);
                MDrv_XC_MLoad_Init(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadPhyAddr_Suspend);
                _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
                _MLOAD_ENTRY();
                MDrv_XC_MLoad_Init(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadPhyAddr_Suspend);
                _MLOAD_RETURN();
#endif //#if SC2_ENABLE


            }

            for(i = 0; i < E_XC_MCDI_MAX; i++)
            {
                if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBSize_Suspend[i] > 0) && (i < E_XC_MCDI_SUB_BOTH))
                {
                    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBAddress[i]=0xFFFFFFFF;
                    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBSize[i]=0x0;
                    MDrv_SC_SetMCDIBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBAddress_Suspend[i],
                                                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBSize_Suspend[i],
                                                 (E_XC_MCDI_TYPE)i);
                }
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableMCDI_Suspend[i])
                {
                    MDrv_SC_EnableMCDI(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableMCDI_Suspend[i], (E_XC_MCDI_TYPE)i);
                }
            }

            for(i = 0; i < MAX_WINDOW; i++)
            {
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[i] > 0)
                {
                    MDrv_XC_SetDispWindowColor(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[i], (SCALER_WIN)i);
                }
            }

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32FrameColor_Suspend > 0)
            {
                MDrv_XC_SetFrameColor(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32FrameColor_Suspend);
            }
#if 0 //(LD_ENABLE == 1)
            // Local Dimming Recovery procedure
            // In Local Dimming situation, we don't know if AP has a mmap for Local Dimming
            // we can only judge it by checking if it is inited so we will try to resume.
            if (pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished == TRUE)
            {
                MDrv_XC_LD_Set_MIUSel(pInstance, pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u8MIU);

                MDrv_XC_LD_SetMemoryAddress(pInstance, pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf0Base,
                                            pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf1Base,
                                            pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base,
                                            pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf1Base,
                                            pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32Edge2DCoeffBase,
                                            (pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf0Base
                                             -pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base));

                MDrv_XC_LD_Init(pInstance, pXCResourcePrivate->stdrv_LD._enLDPanelType);

                // Must call or otherwise our local dimming report value will be only 1 pixel
                // but actually we want it to be the whole screen so we divided it into 32*18 areas of the panel screen.
                MS_U8 u8LDData[576] = {0};
                MDrv_LD_Get_Value(pInstance, u8LDData, 32, 18);
            }
#endif

            pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState = enPowerState;
            u32Ret = UTOPIA_STATUS_SUCCESS;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Ret = UTOPIA_STATUS_FAIL;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %u\n",__FUNCTION__,__LINE__,enPowerState);
        u32Ret = UTOPIA_STATUS_FAIL;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


#if LOGTIMECOST
    printf("=========[%s,%5d] cost %lu ms========\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime() - u32Begin);
#endif

    return u32Ret;
}

MS_U32 MApi_XC_SetPowerState(EN_POWER_MODE enPowerState)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_FAIL;
    }

    stXC_SET_POWERSTATE XCArgs;
    XCArgs.enPowerState = enPowerState;
    XCArgs.u32ReturnValue = UTOPIA_STATUS_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_POWERSTATE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}


//-------------------------------------------------------------------------------------------------
/// MApi_XC_SetByPassOSDVsyncPos
/// @param  u16VsyncStart                              \b IN: OSD vsync start
/// @param  u16VsyncEnd                                \b IN: OSD vsync end
//-------------------------------------------------------------------------------------------------
void MApi_XC_BYPASS_SetOSDVsyncPos_U2(void* pInstance, MS_U16 u16VsyncStart, MS_U16 u16VsyncEnd)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");


#if SC2_ENABLE
    _XC_ENTRY(pInstance);

    MHal_XC_ByPass_SetOSDVsyncPos(pInstance, u16VsyncStart, u16VsyncEnd);

    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();

    MHal_XC_ByPass_SetOSDVsyncPos(pInstance, u16VsyncStart, u16VsyncEnd);

    _XC_RETURN();
#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

}

void MApi_XC_BYPASS_SetOSDVsyncPos(MS_U16 u16VsyncStart, MS_U16 u16VsyncEnd)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BYPASS_OSDVSYNC_POS XCArgs;
    XCArgs.u16VsyncStart = u16VsyncStart;
    XCArgs.u16VsyncEnd = u16VsyncEnd;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BYPASS_OSDVSYNC_POS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_BYPASS_SetInputSrc
/// @param  bEnable                              \b IN: ENABLE/DISABLE bypass mode
/// @param  input                                \b IN: Select the input source for bypass mode
//-------------------------------------------------------------------------------------------------
void MApi_XC_BYPASS_SetInputSrc_U2(void* pInstance, MS_BOOL bEnable, E_XC_BYPASS_InputSource input)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    MHal_XC_BYPASS_Setinputclk(pInstance, bEnable,input);

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MHal_XC_Bypass_SetinputSrc(pInstance, input);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    MHal_XC_Bypass_SetinputSrc(pInstance, input);
    _XC_RETURN();
#endif //#if SC2_ENABLE



    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void MApi_XC_BYPASS_SetInputSrc(MS_BOOL bEnable, E_XC_BYPASS_InputSource input)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BYPASS_INPUTSRC XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.input = input;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BYPASS_INPUTSRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_SetSeamlessZapping
/// @param  eWindow                 \b IN: which window we are going to enable or disable
/// @param  bEnable                 \b IN: ENABLE/DISABLE seamless zapping
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SetSeamlessZapping_U2(void* pInstance, SCALER_WIN eWindow, MS_BOOL bEnable)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    if(!SUPPORT_SEAMLESS_ZAPPING)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    if(!MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        if(!bEnable)
        {
            eRet = E_APIXC_RET_OK;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d] Dynamic scaling is not initialized\n", __FUNCTION__, __LINE__);
            eRet = E_APIXC_RET_FAIL;
        }
    }
    else
    {

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE


        if(bEnable)
        {
            MDrv_XC_Set_DSIndexSourceSelect(pInstance, E_XC_DS_INDEX_SCALER, eWindow);
        }
        else
        {
            MDrv_XC_Set_DSIndexSourceSelect(pInstance, E_XC_DS_INDEX_MVOP, eWindow);
        }

        eRet = Hal_SC_SetSeamlessZapping(pInstance, eWindow, bEnable);
        XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d] set Seamless Zapping[%s] = %x\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", bEnable);

#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE



    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return eRet;
}

E_APIXC_ReturnValue MApi_XC_SetSeamlessZapping(SCALER_WIN eWindow, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_SEAMLESS_ZAPPING XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SEAMLESS_ZAPPING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


E_APIXC_ReturnValue MApi_XC_GetSeamlessZappingStatus_U2(void* pInstance, SCALER_WIN eWindow, MS_BOOL* pbEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");

    E_APIXC_ReturnValue eReturnValue = E_APIXC_RET_FAIL;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    *pbEnable = Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d] get Seamless Zapping[%s] = %x\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", *pbEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    *pbEnable = Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d] get Seamless Zapping[%s] = %x\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", *pbEnable);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    eReturnValue = E_APIXC_RET_OK;

    return eReturnValue;
}

E_APIXC_ReturnValue MApi_XC_GetSeamlessZappingStatus(SCALER_WIN eWindow, MS_BOOL* pbEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_SEAMLESS_ZAPPING_STATUS XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.pbEnable = pbEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SEAMLESS_ZAPPING_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Vtrack_SetPayloadData
/// @param  u16Timecode                       \b IN: The time code is coded on 16 bits. It is calculated based on the number of days from January 1st.
/// @param  u8ID                              \b IN: Operator ID
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Vtrack_SetPayloadData_U2(void* pInstance, MS_U16 u16Timecode, MS_U8 u8OperatorID)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    eReturn = MHal_XC_Vtrack_SetPayloadData(pInstance, u16Timecode, u8OperatorID);
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    eReturn = MHal_XC_Vtrack_SetPayloadData(pInstance, u16Timecode, u8OperatorID);
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Vtrack_SetPayloadData(MS_U16 u16Timecode, MS_U8 u8OperatorID)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_PAYLOADDATA XCArgs;
    XCArgs.u16Timecode = u16Timecode;
    XCArgs.u8OperatorID = u8OperatorID;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VTRACK_PAYLOADDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Vtrack_SetUserDefindedSetting
/// @param  bEnable                              \b IN: ENABLE/DISABLE Customized Setting
/// @param  pu8Setting                           \b IN: pointer to targert user definded setting data (include setting1 to setting3)
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Vtrack_SetUserDefindedSetting_U2(void *pInstance, MS_BOOL bUserDefinded, MS_U8 *pu8Setting)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    eReturn = MHal_XC_Vtrack_SetUserDefindedSetting(pInstance, bUserDefinded, pu8Setting);
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    eReturn = MHal_XC_Vtrack_SetUserDefindedSetting(pInstance, bUserDefinded, pu8Setting);
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Vtrack_SetUserDefindedSetting(MS_BOOL bUserDefinded, MS_U8 *pu8Setting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_USERDEFINED_SETTING XCArgs;
    XCArgs.bUserDefinded = bUserDefinded;
    XCArgs.pu8Setting = pu8Setting;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VTRACK_USERDEFINED_SETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Vtrack_Enable
/// @param  u8FrameRate                          \b IN: ENABLE/DISABLE The FrameRateIn is based on the output format of the box
/// @param  bEnable                              \b IN: ENABLE/DISABLE Vtrack
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Vtrack_Enable_U2(void* pInstance, MS_U8 u8FrameRate, MS_BOOL bEnable)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    eReturn = MHal_XC_Vtrack_Enable(pInstance, u8FrameRate, bEnable);
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    eReturn = MHal_XC_Vtrack_Enable(pInstance, u8FrameRate, bEnable);
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE



    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Vtrack_Enable(MS_U8 u8FrameRate, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_ENABLE XCArgs;
    XCArgs.u8FrameRate = u8FrameRate;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VTRACK_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Pre-set the specific window with PQ info
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  eWindow                \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_PreSetPQInfo_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U32 u32DisPOddCheck = 0x00;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_SC_SetPQHSDFlag(pInstance, FALSE); //Reset variable

    if(pstXC_SetWin_Info->bInterlace)
    {
        //The component 1080i can't frame lock when capture start is odd.
#ifdef K3_U2
        if(!IsSrcTypeDTV( pstXC_SetWin_Info->enInputSourceType ))
#endif
        {
            // the Capture start should be even
            if(pstXC_SetWin_Info->stCapWin.y%2)
            {
                pstXC_SetWin_Info->stCapWin.y += 1;
                pstXC_SetWin_Info->stCapWin.height -= 1; //Y+1, so height-1 to keep not capture outside
            }
        }

        // the Capture height should be even
        if(pstXC_SetWin_Info->stCapWin.height%2)
        {
            pstXC_SetWin_Info->stCapWin.height -= 1;
        }
    }

    // copy user data to internal data
    // here is a patch first, will remove after seperate Internal XC status with set window information
    //u32CopyLen = sizeof(XC_SETWIN_INFO) - (sizeof(MS_BOOL)*2) - (sizeof(MS_U16)*4);
    //memcpy(&gSrcInfo[eWindow], pstXC_SetWin_Info, u32CopyLen);
    //do not use memcopy(), strongly depend on structure definition
    _Mdrv_XC_CopySetWinInfo(pInstance, pstXC_SetWin_Info, eWindow);

    // The total pixel conut of display window can not be odd.
    u32DisPOddCheck = gSrcInfo[eWindow].stDispWin.width * gSrcInfo[eWindow].stDispWin.height;
    // if u32DisPOddCheck is odd, that means both of disp.width and disp.height is odd.
    // we adjust width to even.
    if (u32DisPOddCheck & 0x01)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > gSrcInfo[eWindow].stDispWin.width )
        {
            gSrcInfo[eWindow].stDispWin.width = ( gSrcInfo[eWindow].stDispWin.width + 1 ) & ~0x1;
        }
    }

    // if mirror, need to re-cal display window but Ninelattice!!
    if(IsVMirrorMode(eWindow) &&(!gSrcInfo[eWindow].bDisplayNineLattice))
    {
        MS_U32 u32TempWidth = gSrcInfo[eWindow].stDispWin.width;
        MS_U32 u32TempHeight = gSrcInfo[eWindow].stDispWin.height;

        if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth ) )
        {
            gSrcInfo[eWindow].stDispWin.x =
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - ( gSrcInfo[eWindow].stDispWin.x + u32TempWidth );
        }
        else
        {
            gSrcInfo[eWindow].stDispWin.x = 0;
        }


        if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > ( gSrcInfo[eWindow].stDispWin.y + u32TempHeight ) )
        {
            gSrcInfo[eWindow].stDispWin.y =
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - ( gSrcInfo[eWindow].stDispWin.y + u32TempHeight );
        }
        else
        {
            gSrcInfo[eWindow].stDispWin.y = 0;
        }
    }
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_FormatValidation(pInstance, &gSrcInfo[eWindow], eWindow);
    MDrv_SC_3D_AdjustHShift(pInstance, &gSrcInfo[eWindow], eWindow);
#endif
    gSrcInfo[eWindow].stDispWin.x += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
    gSrcInfo[eWindow].stDispWin.y += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;

    // dump debug msg
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"============= SetWindow Start (Window : %u, src: %u) =============\n",eWindow, gSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCapWin.x, gSrcInfo[eWindow].stCapWin.y, gSrcInfo[eWindow].stCapWin.width, gSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"CropWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stCropWin.x, gSrcInfo[eWindow].stCropWin.y, gSrcInfo[eWindow].stCropWin.width, gSrcInfo[eWindow].stCropWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"DispWin x: %4u  y: %4u  w: %4u  h: %4u \n",gSrcInfo[eWindow].stDispWin.x,gSrcInfo[eWindow].stDispWin.y, gSrcInfo[eWindow].stDispWin.width, gSrcInfo[eWindow].stDispWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Panel   x: %4u  y: %4u  w: %4u  h: %4u \n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart
                 , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart
                 , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width
                 , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Mirror/Interlace/Hdup = %u/%u/%u\n", IsVMirrorMode(eWindow), pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"H/V total = (%u, %u), VFreq = %u\n", pstXC_SetWin_Info->u16DefaultHtotal, pstXC_SetWin_Info->u16InputVTotal, pstXC_SetWin_Info->u16InputVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Pre  H cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].Status2.bPreHCusScaling, gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreHCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Pre  V cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].Status2.bPreVCusScaling, gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc, gSrcInfo[eWindow].Status2.u16PreVCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Post H cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].bHCusScaling, gSrcInfo[eWindow].u16HCusScalingSrc, gSrcInfo[eWindow].u16HCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Post V cus scaling %u (%u -> %u)\n", gSrcInfo[eWindow].bVCusScaling, gSrcInfo[eWindow].u16VCusScalingSrc, gSrcInfo[eWindow].u16VCusScalingDst);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    // Scaler must update source before width / height checking.
    MApi_XC_Set_PQ_SourceData(pInstance, eWindow, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow]);
    msAPI_Scaler_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], eWindow);

}

void MApi_XC_PreSetPQInfo(XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_PRESET_PQINFO XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_PRESET_PQINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Mapi_XC_Is_OP1_TestPattern_Enabled
/// @return MS_BOOL: TRUE - enabled, FALSED - disabled
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Is_OP1_TestPattern_Enabled_U2(void* pInstance)
{
#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
#else
    return FALSE;
#endif
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Is_OP1_TestPattern_Enabled(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_OP1_TESTPATTERN_ENABLED XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_GET_OP1_TESTPATTERN_ENABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
MS_BOOL MApi_XC_Is_OP1_IRE_InFactoryMode(void)
{
    return bIsIREMode_Open;
}
E_APIXC_ReturnValue MApi_XC_Set_OP2_TestPattern_RGB_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data,MS_U16 u16CSC)
{
    bIsIREMode_Open = TRUE;
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
#ifndef DISABLE_HW_PATTERN_FUNCTION

#ifdef SUPPORT_HW_TESTPATTERN
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //MS_U16 u16HWInputVtt =  MApi_XC_R2BYTEMSK(REG_SC_BK01_1F_L,0xFFFF);
    //MS_U16 u16HWInputHPeriod = MApi_XC_R2BYTEMSK( REG_SC_BK01_20_L, 0xFFFF);
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    MS_WINDOW_TYPE stWin = {0,0,0,0};

    Hal_SC_get_disp_de_window(pInstance, &stWin);
    if (FALSE == pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled)
    {
        pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled = Hal_SC_Is_InputSource_Disable(pInstance, MAIN_WINDOW) >> 7;
        pXCResourcePrivate->stdrvXC_MVideo.bIsMute = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1)) >> 1;
    }


    if(gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled)
    {
        if (!bEnable)// Pattern Off
        {
            pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = FALSE;
            bIsIREMode_Open= FALSE;
            if(bColorMatrixEnable == FALSE)
            {
                 SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L,0, BIT(4));
            }
            else
            {
                 SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L, BIT(4), BIT(4));
            }

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_32_L, u16R_Data, 0x03FF);//R
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_33_L, u16G_Data, 0x03FF);//G
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_34_L, u16B_Data, 0x03FF);//B
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_30_L, 0, BIT(0) );
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_31_L, 0, BIT(0) );
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0x3000, 0x3000);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0,BIT(7));
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_02_L, pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled,BIT(7));
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, BIT(1), BIT(1) );
        }
        else// Pattern On
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, stWin.height,0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, stWin.width, 0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, stWin.width, 0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, stWin.height,0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, stWin.width, 0xFFFF);

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_32_L, u16R_Data, 0x03FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_33_L, u16G_Data, 0x03FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_34_L, u16B_Data, 0x03FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0, 0x3000);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_30_L, BIT(0), BIT(0) );
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_31_L, BIT(0), BIT(0) );

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L, 0,BIT(4));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0,BIT(7));
            pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = TRUE;
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, 0, BIT(1) );
        }
    
    }
    else
    {	
#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
#endif //#if SC2_ENABLE
	
        if (!bEnable)// Pattern Off
        {
            pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = FALSE;
            bIsIREMode_Open= FALSE;
            if(bColorMatrixEnable == FALSE)
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_2F_L,0, BIT(4));
            }
            else
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_2F_L, BIT(4), BIT(4));
            }

            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_32_L, u16R_Data, 0x03FF);//R
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_33_L, u16G_Data, 0x03FF);//G
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_34_L, u16B_Data, 0x03FF);//B
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_30_L, 0, BIT(0) );
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_31_L, 0, BIT(0) );
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_01_L, 0x3000, 0x3000);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_02_L, 0,BIT(7));
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_02_L, pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled,BIT(7));
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, BIT(1), BIT(1) );
        }

        else// Pattern On
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK62_62_L, stWin.height,0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK62_61_L, stWin.width, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK63_51_L, stWin.width, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK63_52_L, stWin.height,0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK63_55_L, stWin.width, 0xFFFF);

            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_32_L, u16R_Data, 0x03FF);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_33_L, u16G_Data, 0x03FF);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_34_L, u16B_Data, 0x03FF);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_01_L, 0, 0x3000);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_30_L, BIT(0), BIT(0) );
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK25_31_L, BIT(0), BIT(0) );

            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_2F_L, 0,BIT(4));
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_02_L, 0,BIT(7));
            pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = TRUE;
            //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, 0, BIT(1) );
        }
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
		 
#if SC2_ENABLE
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE
    }
#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif


    return enResult;
}

E_APIXC_ReturnValue MApi_XC_Set_OP2_TestPattern_RGB( MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data,MS_U16 u16CSC)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP2_TESTPATTERN_RGB XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u16R_Data = u16R_Data;
    XCArgs.u16G_Data = u16G_Data;
    XCArgs.u16B_Data = u16B_Data;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;
    XCArgs.u16CSC = u16CSC;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OP2_TESTPATTERN_RGB, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_OP1_TestPattern_RGB_U2(void* pInstance, EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode,MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal)
{
    bIsIREMode_Open = TRUE;
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
#ifndef DISABLE_HW_PATTERN_FUNCTION

#ifdef SUPPORT_HW_TESTPATTERN
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE

    MS_U16 u16_T3d_status;
    u16_T3d_status = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK62_08_L, 0x0010);
    if (FALSE == pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled)
    {
        pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled = Hal_SC_Is_InputSource_Disable(pInstance, MAIN_WINDOW) >> 7;
        pXCResourcePrivate->stdrvXC_MVideo.bIsMute = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1)) >> 1;
    }

    if (E_XC_OP1_PATTERN_OFF == ePattern)
    {
        pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = FALSE;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1),BIT(1)); //window color en : on
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK62_08_L,u16_T3d_status, 0x00FF);
        MDrv_SC_OP1_PatGen_Disable(pInstance);
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_24_L, 0x03,0xFFFF);
        bIsIREMode_Open= FALSE;
        }
        else
        {
        MDrv_SC_Set_OP1_TestPattern_RGB(pInstance, ePattern,eMode,u16RVal,u16GVal,u16BVal);
        OP1_RVal= u16RVal;
        OP1_GVal = u16GVal;
        OP1_BVal = u16BVal;
        pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = TRUE;
        }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif


    return enResult;
}

E_APIXC_ReturnValue MApi_XC_Set_OP1_TestPattern_RGB(EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode,MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP1_TESTPATTERN_RGB XCArgs;
    XCArgs.ePattern = ePattern;
    XCArgs.eMode = eMode;
    XCArgs.u16RVal = u16RVal;
    XCArgs.u16GVal = u16GVal;
    XCArgs.u16BVal = u16BVal;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OP1_TESTPATTERN_RGB, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


E_APIXC_ReturnValue MApi_XC_Set_IP1_PatternRGB_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data,MS_U16 u16CSC,MS_BOOL bCCIR)
{
    bIsIREMode_Open = TRUE;
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
#ifndef DISABLE_HW_PATTERN_FUNCTION

#ifdef SUPPORT_HW_TESTPATTERN
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE

    if(gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled)
    {
        if(bEnable)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_40_L, BIT(0)|BIT(6)|BIT(11)|BIT(12), BIT(0)|BIT(6)|BIT(11)|BIT(12));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_42_L, u16R_Data|u16G_Data, 0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_43_L, u16B_Data, 0x00FF);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_42_L, 0, 0xFFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_43_L, 0, 0x00FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_40_L, 0, BIT(0)|BIT(6)|BIT(11)|BIT(12) );
        }
    }
    else
    {

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
#endif //#if SC2_ENABLE


         MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_IP_MAIN_SYNC);
         MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_2F_L, u16CSC,0x0010);//enable color matrix
         if (bEnable)
         {
             MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_40_L, BIT(0)|BIT(6)|BIT(11)|BIT(12), BIT(0)|BIT(6)|BIT(11)|BIT(12) );
             MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_42_L, u16R_Data|u16G_Data, 0xFFFF);
		     MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_43_L, u16B_Data, 0x00FF);
         }
         else
         {
		     MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_42_L, 0, 0xFFFF);
		     MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_43_L, 0, 0x00FF);
             MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_40_L, 0, BIT(0)|BIT(6)|BIT(11)|BIT(12) );
         }
         MDrv_XC_MLoad_Fire(pInstance, TRUE);
         MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_OP_SYNC);
#if SC2_ENABLE
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE
    }
#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif


    return enResult;
}


E_APIXC_ReturnValue MApi_XC_Set_IP1_PatternRGB(MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data,MS_U16 u16CSC,MS_BOOL bCCIR)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_IP1_TESTPATTERN_RGB XCArgs;
 //   XCArgs.ePattern = ePattern;
 //   XCArgs.eMode = eMode;
    XCArgs.bEnable = bEnable;
    XCArgs.u16R_Data = u16R_Data;
    XCArgs.u16G_Data = u16G_Data;
    XCArgs.u16B_Data = u16B_Data;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;
   XCArgs.u16CSC = u16CSC;
    XCArgs.bCCIR = bCCIR;
    if(UtopiaIoctl(pu32XCInst, E_XC_SET_IP1_TESTPATTERN_RGB, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Set_OP1_TestPattern
/// @param  ePattern                                      \b IN: select test pattern
/// @param  eMode                                         \b IN: choose the patgen mode
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL or E_APIXC_RET_FAIL_FUN_NOT_SUPPORT
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Set_OP1_TestPattern_U2(void* pInstance, EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode)
{
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;

#ifndef DISABLE_HW_PATTERN_FUNCTION

#ifdef SUPPORT_HW_TESTPATTERN
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    if (FALSE == pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled)
    {
        pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled = Hal_SC_Is_InputSource_Disable(pInstance, MAIN_WINDOW) >> 7;
        pXCResourcePrivate->stdrvXC_MVideo.bIsMute = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1)) >> 1;
    }

    if (E_XC_OP1_PATTERN_OFF == ePattern)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1),BIT(1)); //window color en : on

        MDrv_SC_OP1_PatGen_Disable(pInstance);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsMute << 1),BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled << 7),BIT(7));

        pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = FALSE;
    }
    else
    {
        MDrv_SC_Set_OP1_TestPattern(pInstance, ePattern,eMode);
        pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = TRUE;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

#else
    enResult = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif


    return enResult;
}

E_APIXC_ReturnValue MApi_XC_Set_OP1_TestPattern(EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP1_TESTPATTERN XCArgs;
    XCArgs.ePattern = ePattern;
    XCArgs.eMode = eMode;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OP1_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_Set_OP2_Pattern
/// @param  bEnable               \b IN: enable/disable OP2  pattern
/// @param  u16R_Data             \b IN: when enable OP2  patter ,Data Generator for r_cr constant
/// @param  u16G_Data             \b IN: when enable OP2  patter ,Data Generator for g_y constant
/// @param  u16B_Data             \b IN: when enable OP2  patter ,Data Generator for b_cb constant
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL or E_APIXC_RET_FAIL_FUN_NOT_SUPPORT
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Set_OP2_Pattern_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data)
{
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
#if (SUPPORT_OP2_TEST_PATTERN >0)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    if(bEnable == TRUE)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern == FALSE)
        {
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern = TRUE;
            pXCResourcePrivate->stdrvXC_MVideo.u16OP2_color_3x3 = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L,0xFFFF);
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2InputSrcDisabled = Hal_SC_Is_InputSource_Disable(pInstance, MAIN_WINDOW) >> 7;
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2Mute = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1)) >> 1;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, 0,BIT(1));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0,BIT(7)); //input source disable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L,0x0000,0xFFFF); //disable 3x3
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_32_L, u16R_Data, BMASK(9:0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_33_L, u16G_Data, BMASK(9:0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_34_L, u16B_Data, BMASK(9:0));
    }
    else
    {
        if (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern == TRUE)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L, pXCResourcePrivate->stdrvXC_MVideo.u16OP2_color_3x3, 0xFFFF); //restore 3x3
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2Mute<< 1),BIT(1));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2InputSrcDisabled<< 7),BIT(7));
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern = FALSE;
        }
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_30_L, (bEnable), BIT(0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK25_31_L, (bEnable),  BIT(0));

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE



#endif
    return enResult;
}

E_APIXC_ReturnValue MApi_XC_Set_OP2_Pattern(MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP2_TESTPATTERN  XCArgs;
    XCArgs.bEnable = bEnable ;
    XCArgs.u16R_Data = u16R_Data ;
    XCArgs.u16G_Data = u16G_Data;
    XCArgs.u16B_Data = u16B_Data;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OP2_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_CheckWhiteBalancePatternModeSupport
/// @param  eWBPatternMode   \b IN: White balance Pattern gen Mode
/// @return MS_BOOL: TRUE - support, FALSED - unsupport
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_CheckWhiteBalancePatternModeSupport_U2(void* pInstance, EN_XC_WB_PATTERN_MODE enWBPatternMode)
{
#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
    if(enWBPatternMode == E_XC_OP1_PATTERN_MODE)
    {
        return TRUE;
    }
    else
#endif
        if(SUPPORT_OP2_TEST_PATTERN)
        {
            if(enWBPatternMode == E_XC_OP2_PATTERN_MODE)
            {
                return TRUE;
            }
        }
    return FALSE;
#else
    if(SUPPORT_OP2_TEST_PATTERN)
    {
        if(enWBPatternMode == E_XC_OP2_PATTERN_MODE)
        {
            return TRUE;
        }
    }
    return FALSE;
#endif
}

MS_BOOL MApi_XC_CheckWhiteBalancePatternModeSupport(EN_XC_WB_PATTERN_MODE enWBPatternMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_CHECK_WB_TESTPATTERN  XCArgs;
    XCArgs.eWBPattern = enWBPatternMode;
    XCArgs.eReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CHECK_WHITEBALANCE_PATTERN_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
E_APIXC_ReturnValue MApi_XC_SetHLinearScaling_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bSign, MS_U16 u16Delta, SCALER_WIN eWindow)
{
#if XC_HLINEARSCALING_VER > 0
    MS_U16 u16DeltaInit = 0;


#if SC2_ENABLE
    _XC_ENTRY(pInstance);

#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(bEnable)
    {
        if(0 == SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, BIT(8)))
        {
            printf("Waring: there is no H_postscaling enabled, the HLinearScaling won't work!\n");
        }

        u16DeltaInit = (960 * u16Delta) / 512;

        if((u16DeltaInit < 2) || (u16Delta > 0x7F) || (eWindow != MAIN_WINDOW))
        {

#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
            _XC_RETURN();
#endif //#if SC2_ENABLE


            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"MApi_XC_SetHLinearScaling(): u16Delta=%u, u16DeltaInit=%u, eWindow=%u\n", u16Delta, u16DeltaInit, eWindow);
            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
        }

        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"MApi_XC_SetHLinearScaling(): u16DeltaInit=%u\n", u16DeltaInit);

        //enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, BIT(6), BIT(6));

        //zone 0 width: 0x3C0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 1<<12, BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_14_L, 0xE0, 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 1<<8, BIT(8));

        //zone 1 width: 0x3C0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 1<<13, BIT(13));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_14_L, 0xE0<<8, 0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 1<<9, BIT(9));

        //zone 0 1 Delta
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_16_L, (u16Delta<<8) | (u16Delta));

        //zone 0 1 Delta init
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_15_L, u16DeltaInit<<8, 0x7F00);

        //Delta init Signed
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_15_L, bSign<<15, BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 0, BIT(6));
    }


#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE



    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MApi_XC_SetHLinearScaling(MS_BOOL bEnable, MS_BOOL bSign, MS_U16 u16Delta, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_HLINEARSCALING XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bSign = bSign;
    XCArgs.u16Delta = u16Delta;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_HLINEARSCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_EnableT3D_U2(void* pInstance, MS_BOOL bEnable)
{
    MS_BOOL bRet = FALSE;
#ifdef ENABLE_CHIP_BRINGUP
    //bEnable = FALSE;
#endif


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_EnableT3D(pInstance, bEnable);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
    bRet = MDrv_XC_EnableT3D(pInstance, bEnable);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue MApi_XC_EnableT3D(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_EnableT3D XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_ENABLE_T3D, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_FRC_InputTiming_U2(void* pInstance, E_XC_FRC_InputTiming enFRC_InputTiming)
{
#if FRC_INSIDE
    MS_BOOL bRet = FALSE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bRet = MDrv_XC_Set_FRC_InputTiming(pInstance, enFRC_InputTiming);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bRet = MDrv_XC_Set_FRC_InputTiming(pInstance, enFRC_InputTiming);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MApi_XC_Set_FRC_InputTiming(E_XC_FRC_InputTiming enFRC_InputTiming)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Set_FRC_InputTiming XCArgs;
    XCArgs.enFRC_InputTiming = enFRC_InputTiming;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_FRC_INPUTTIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Get_FRC_InputTiming_U2(void* pInstance, E_XC_FRC_InputTiming *penFRC_InputTiming)
{
#if FRC_INSIDE
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE



    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(penFRC_InputTiming)
    {
        *penFRC_InputTiming = pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming;
        bRet = TRUE;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MApi_XC_Get_FRC_InputTiming(E_XC_FRC_InputTiming *penFRC_InputTiming)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Get_FRC_InputTiming XCArgs;
    XCArgs.penFRC_InputTiming = penFRC_InputTiming;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_GET_FRC_INPUTTIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
#if 0
E_APIXC_ReturnValue MApi_XC_Get_VirtualBox_Info_U2(void* pInstance, XC_VBOX_INFO *pstXC_VboxInfo)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8FrameCnt = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pstXC_VboxInfo)
    {
        pstXC_VboxInfo->u32FBSize[0] = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size;
        pstXC_VboxInfo->u32FBSize[1] = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size;
        pstXC_VboxInfo->u16VBox_PanelHsize = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        pstXC_VboxInfo->u16VBox_PanelVsize = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

        // temp for sub
        pstXC_VboxInfo->u16VBox_Htotal[1] = 1920;
        pstXC_VboxInfo->u16VBox_Vtotal[1] = 1080;

        if(XC_SUPPORT_4K2K)
        {
            // in 4K2K case, the Madi mode is P_Mode10 = 20bit/pixels
            u8FrameCnt = (pstXC_VboxInfo->u32FBSize[0]) / (pstXC_VboxInfo->u16VBox_PanelHsize*pstXC_VboxInfo->u16VBox_PanelVsize * 20/8);
            if(u8FrameCnt >= 2)
            {
#if FRC_INSIDE
                /// FRC Enable
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited)
                {
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_4K2K)
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                    }
                    else //if(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)) and others
                    {
                        // 2K2K only for Napoli with FRC_On
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080*2;
                    }
                }
                else  /// FRC Disable
                {
                    if((pstXC_VboxInfo->u16Vfreq[0] <= 300)&&(pstXC_VboxInfo->bInterface[0] == FALSE))
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                    }
                    else
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                    }
                }
#else
                if(0)//(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_MONACO_MODE)
                {
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                }
                else // exp : Nike, Monaco_legacy_mode
                {
                    if((pstXC_VboxInfo->u16Vfreq[0] <= 300)&&(pstXC_VboxInfo->bInterface[0] == FALSE))
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                    }
                    else
                    {
                        pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                        pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                    }
                }
#endif
                {

                    pstXC_VboxInfo->u16VBox_Htotal[0] = 3840;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 2160;
                }
            }
            else
            {
#if FRC_INSIDE
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited
                        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K))
                {
                    // 2K2K only for Napoli with FRC_On
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080*2;
                }
                else
#endif
                {
                    pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
                    pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
                }
            }
        }
        else
        {
            pstXC_VboxInfo->u16VBox_Htotal[0] = 1920;
            pstXC_VboxInfo->u16VBox_Vtotal[0] = 1080;
        }
        bRet = TRUE;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue MApi_XC_Get_VirtualBox_Info(XC_VBOX_INFO *pstXC_VboxInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Get_VBOX_Info XCArgs;
    XCArgs.pstXC_VboxInfo = pstXC_VboxInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VBOX_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
#endif
E_APIXC_ReturnValue MApi_XC_Set_OSD_Detect_U2(void* pInstance,MS_BOOL bEnable, MS_U32 Threhold)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bRet = Hal_SC_SetOSDDetect(pInstance, bEnable,Threhold);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bRet = Hal_SC_SetOSDDetect(pInstance, bEnable,Threhold);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_OSD_Detect(MS_BOOL bEnable, MS_U32 Threhold)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Set_OSD_DETECT XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.Threhold = Threhold;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OSD_DETECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


E_APIXC_ReturnValue MApi_XC_Set_3D_TP_InnerPattern_U2(void* pInstance, MS_BOOL bEnable,  MS_3D_InnerInfo st3DinnerInfo)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);




#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    Hal_SC_ip_Set_TestPattern(pInstance,st3DinnerInfo.u16ip1[MAIN_WINDOW], st3DinnerInfo.u16ip1value[MAIN_WINDOW], MAIN_WINDOW);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK03_40_L, st3DinnerInfo.u16ip1[SUB_WINDOW], 0xFFFF);
    SC_W2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK03_43_L, st3DinnerInfo.u16ip1value[SUB_WINDOW]);

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE



#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Set_3D_TP_InnerPattern(MS_BOOL bEnable, MS_3D_InnerInfo st3DinnerInfo)
{
#if 1
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Set_3D_TP_InnerPattern XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.st3dinner.u16ip1[MAIN_WINDOW] = st3DinnerInfo.u16ip1[MAIN_WINDOW];
    XCArgs.st3dinner.u16ip1[SUB_WINDOW] = st3DinnerInfo.u16ip1[SUB_WINDOW];
    XCArgs.st3dinner.u16ip1value[MAIN_WINDOW] = st3DinnerInfo.u16ip1value[MAIN_WINDOW];
    XCArgs.st3dinner.u16ip1value[SUB_WINDOW] = st3DinnerInfo.u16ip1value[SUB_WINDOW];
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_3D_TP_INNERPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
#endif
}


void MApi_XC_FRC_IPM_TestPattern_U2(void* pInstance, MS_BOOL bEnable,MS_U8 u8TestMode)
{
#if 0
    _XC_ENTRY();
    MDrv_FRC_IPM_TestPattern(bEnable,u8TestMode);
    _XC_RETURN();
#endif
}

void MApi_XC_FRC_IPM_TestPattern(MS_BOOL bEnable,MS_U8 u8TestMode)
{
#if 0
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_IPM_TESTPATTERN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Pattern_type = u8TestMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_IPM_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

void MApi_XC_FRC_OPM_TestPattern_U2(void* pInstance, MS_BOOL bEnable,MS_U8 u8TestMode)
{
#if 0
    _XC_ENTRY();
    MDrv_FRC_OPM_TestPattern(bEnable,u8TestMode);
    _XC_RETURN();
#endif
}

void MApi_XC_FRC_OPM_TestPattern(MS_BOOL bEnable,MS_U8 u8TestMode)
{
#if 0
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_OPM_TESTPATTERN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Pattern_type = u8TestMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_OPM_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

void MApi_XC_FRC_OP_TestPattern_U2(void* pInstance, MS_BOOL bEnable,MS_U8 u8TestMode, MS_U8 u8Color)
{
#if 0 //no use
    _XC_ENTRY();
    MDrv_FRC_OP_TestPattern(bEnable,u8TestMode, u8Color);
    _XC_RETURN();
#endif
}

void MApi_XC_FRC_OP_TestPattern(MS_BOOL bEnable,MS_U8 u8TestMode, MS_U8 u8Color)
{
#if 0 //no use
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRC_OP_TESTPATTERN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Pattern_type = u8TestMode;
    XCArgs.u8Color= u8Color;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_OP_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

void MApi_XC_Set_OP_TestPattern_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8TestMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();

#endif //#if SC2_ENABLE

    if (u8TestMode == 1) // the test pattern between HVSP & Display Line Buffer
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK20_1F_L, bEnable<<10, BIT(10));
    }
    else // the test pattern between OPM & HVSP (post_scaling)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK20_10_L, bEnable<<15, BIT(15));
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE
}

void MApi_XC_Set_OP_TestPattern(MS_BOOL bEnable, MS_U8 u8TestMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OP_TESTPATTERN  XCArgs;
    XCArgs.bEnable = bEnable ;
    XCArgs.u8TestMode = u8TestMode ;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_OP_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_VOP_TestPattern_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();

#endif //#if SC2_ENABLE

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_13_L, bEnable<<8, BIT(8));

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE
}

void MApi_XC_Set_VOP_TestPattern(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_VOP_TESTPATTERN  XCArgs;
    XCArgs.bEnable = bEnable ;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_VOP_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void	MApi_XC_SetCodedWindow_U2(void* pInstance, MS_WINDOW_TYPE stCodedwin, SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE

    gSrcInfo[eWindow].Status2.stCodedWin.x       = stCodedwin.x;
    gSrcInfo[eWindow].Status2.stCodedWin.y       = stCodedwin.y;
    gSrcInfo[eWindow].Status2.stCodedWin.width   = stCodedwin.width;
    gSrcInfo[eWindow].Status2.stCodedWin.height  = stCodedwin.height;

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

void	MApi_XC_SetCodedWindow(MS_WINDOW_TYPE stCodedwin, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CODED_WIN XCArgs;
    XCArgs.stCodedwin = stCodedwin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CODED_WIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//newly added
void MApi_XC_Set_FrcBypassFlag(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }

    stXC_SET_FRC_BYPASS_FLAG XCArgs;
    XCArgs.bIsBypass = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRC_BYPASS_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}

void MApi_XC_Set_FrcBypassFlag_U2(void* pInstance, MS_BOOL bEnable)
{
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsbypassFRC = bEnable;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
}

MS_BOOL MApi_XC_Get_Smartzoom_U2(void*pInstance)
{
    MS_BOOL bEnalbe= FALSE;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    bEnalbe = MDrv_SC_GetSmartzoom(pInstance);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE

    return bEnalbe;

}

MS_BOOL MApi_XC_Get_SmartZoom(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }

    stXC_GET_SMARTZOOM XCArgs;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SMARTZOOM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

void MApi_XC_Set_Smartzoom_U2(void*pInstance,MS_BOOL bEnable)
{
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsSmartzoomOpen = bEnable;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
}
void MApi_XC_Set_SmartZoom(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }

    stXC_SET_SMARTZOOM XCArgs;
    XCArgs.bIsSmartZoomOpen = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SMARTZOOM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}
void MApi_XC_Set_StreamPause(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }

    stXC_SET_STREAM_PAUSE XCArgs;
    XCArgs.bIsPause = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_STREAM_PAUSE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}

void MApi_XC_Set_StreamPause_U2(void* pInstance, MS_BOOL bEnable)
{
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsStreamPause = bEnable;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
}


MS_BOOL MApi_XC_Get_StreamPause(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }

    stXC_GET_STREAM_PAUSE XCArgs;
    XCArgs.bIsPause = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_STREAM_PAUSE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bIsPause;
    }
}



MS_BOOL MApi_XC_Get_StreamPause_U2(void* pInstance)
{
    MS_BOOL bIsPause = 0;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    bIsPause = pXCResourcePrivate->bIsStreamPause;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return bIsPause;
}

void MApi_XC_OP_Chk_Stus_U2(void* pInstance)
{
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    MHal_XC_DetectLBErr_Rst(pInstance);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
}

E_APIXC_ReturnValue MApi_XC_OP_Chk_Stus(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    stXC_GET_STREAM_PAUSE XCArgs;
    XCArgs.bIsPause = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_OP_CHK_STUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return E_APIXC_RET_OK;
    }
}

void MApi_XC_SetXCOutputMode(EN_XC_SOC_OUTPUT_MODE enXCOutputMode)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }

    stXC_SET_OUTPUT_MODE XCArgs;
    XCArgs.enXCOutputMode = enXCOutputMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}

void MApi_XC_SetXCOutputMode_U2(void* pInstance, EN_XC_SOC_OUTPUT_MODE enXCOutputMode)
{
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display.enXCOutputMode = enXCOutputMode;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
}


EN_XC_SOC_OUTPUT_MODE MApi_XC_GetXCOutputMode(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }

    stXC_GET_OUTPUT_MODE XCArgs;
    XCArgs.enXCOutputMode = E_XC_SOC_OUTPUT_MAX_MODE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OUTPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_SOC_OUTPUT_MAX_MODE;
    }
    else
    {
        return XCArgs.enXCOutputMode;
    }
}

EN_XC_SOC_OUTPUT_MODE MApi_XC_GetXCOutputMode_U2(void* pInstance)
{
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    EN_XC_SOC_OUTPUT_MODE enXCOutputMode = pXCResourcePrivate->stdrvXC_Display.enXCOutputMode;
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return enXCOutputMode;
}

E_APIXC_ReturnValue MApi_XC_Get_OSD_Detect_U2(void* pInstance,MS_BOOL *pbOSD)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bRet = Hal_SC_GetOSDDetect(pInstance, pbOSD);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bRet = Hal_SC_GetOSDDetect(pInstance, pbOSD);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    if (bRet)
    {
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue MApi_XC_Get_OSD_Detect(MS_BOOL *pbOSD)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Get_OSD_DETECT XCArgs;
    XCArgs.pbOSD = pbOSD;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_GET_OSD_DETECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_Is2K2KToFrcMode_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bRet = MDrv_XC_Is2K2KToFrc(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bRet = MDrv_XC_Is2K2KToFrc(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bRet;
}

MS_BOOL MApi_XC_Is2K2KToFrcMode(void)
{
    //return MDrv_XC_Is2K2KToFrc();
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_Is2K2KToFrcMode  XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_IS2K2KTOFRCMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


#if 1

MS_BOOL MApi_XC_Set_XC_VOP_U2(void * pInstance, MS_XC_VOP_Data stXCVOPData)
{

    MS_BOOL bResult = TRUE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    printf("[%s][%d]pPNLInstancePrivate->u32DeviceID= %lx\n", __FUNCTION__, __LINE__, psXCInstPri->u32DeviceID);
    if(psXCInstPri->u32DeviceID == 0)
    {
#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
        return FALSE;
    }

    MDrv_XC_Set_VOP(pInstance, &stXCVOPData);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE


    return bResult;
}

MS_BOOL MApi_XC_Set_XC_VOP(MS_XC_VOP_Data stXCVOPData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SetXC_VOP XCArgs;
    XCArgs.stXC_VOPData = stXCVOPData;
    XCArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_XC_VOP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturn;
    }

}


#endif //#if SC2_ENABLE

//Additonal function for offline detect .
void MApi_XC_Offline_ADC_SetMode_U2(void* pInstance,MS_U16 u16DefaultPhase, MS_U16 u16DefaultHtotal, MS_BOOL bHDuplicate, INPUT_SOURCE_TYPE_t enSourceType, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    if(!IsSrcTypeYPbPr(enSourceType))
        return;

    MS_U16 u16PixClk = 0, u16RealPixClk = 0; // Add the initial value
    MS_U16 u16HorizontalTotal,u16ratio;
    XC_IP_SYNC_STATUS sXC_Sync_Status;

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    MDrv_XC_GetSyncStatus(pInstance,enSourceType, &sXC_Sync_Status, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    MDrv_XC_ADC_hpolarity_setting(pInstance,sXC_Sync_Status.u8SyncStatus & XC_MD_HSYNC_POR_BIT);
    MDrv_XC_ADC_SetPhaseEx(pInstance,u16DefaultPhase); // setting ADC phase

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    u16RealPixClk = MDrv_XC_cal_pixclk(pInstance,u16DefaultHtotal, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    if(bHDuplicate)
        u16ratio = 2;
    else
        u16ratio = 1;
    MDrv_XC_ADC_Reload(pInstance,enSourceType, u16RealPixClk, u16DefaultHtotal, u16ratio);

    // Get Htt
    if(bHDuplicate)
    {
        u16HorizontalTotal = u16DefaultHtotal * u16ratio; // for better quality
    }
    else
    {
        u16HorizontalTotal = u16DefaultHtotal;
    }

    // Calculate pixel clock, the unit is MHz
#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    u16PixClk = MDrv_XC_cal_pixclk(pInstance,u16HorizontalTotal, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    MDrv_XC_ADC_clamp_placement_setting(pInstance,u16PixClk);
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
}
void MApi_XC_Offline_ADC_SetMode(MS_U16 u16DefaultPhase, MS_U16 u16DefaultHtotal, MS_BOOL bHDuplicate, INPUT_SOURCE_TYPE_t enSourceType, SCALER_WIN eWindow)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_Offline_ADC_Setmode XCArgs;
    XCArgs.u16DefaultPhase = u16DefaultPhase;
    XCArgs.u16DefaultHtotal = u16DefaultHtotal;
    XCArgs.bHDuplicate = bHDuplicate;
    XCArgs.enSourceType = enSourceType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_OFFLINE_ADC_SETMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


void MApi_XC_Pre_ADC_SetMode_U2(void* pInstance,MS_U16 u16DefaultPhase, MS_U16 u16DefaultHtotal, MS_BOOL bHDuplicate, INPUT_SOURCE_TYPE_t enSourceType, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    if(!IsSrcTypeYPbPr(enSourceType))
        return;

    MS_U16 u16PixClk = 0, u16RealPixClk = 0; // Add the initial value
    MS_U16 u16HorizontalTotal,u16ratio;
    XC_IP_SYNC_STATUS sXC_Sync_Status;

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    MDrv_XC_GetSyncStatus(pInstance,enSourceType, &sXC_Sync_Status, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    MDrv_XC_ADC_hpolarity_setting(pInstance,sXC_Sync_Status.u8SyncStatus & XC_MD_HSYNC_POR_BIT);
    MDrv_XC_ADC_SetPhaseEx(pInstance,u16DefaultPhase); // setting ADC phase

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    u16RealPixClk = MDrv_XC_cal_pixclk(pInstance,u16DefaultHtotal, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    if(bHDuplicate)
        u16ratio = 2;
    else
        u16ratio = 1;
    MDrv_XC_ADC_Reload(pInstance,enSourceType, u16RealPixClk, u16DefaultHtotal, u16ratio);
    // Get Htt
    if(bHDuplicate)
    {
        u16HorizontalTotal = u16DefaultHtotal * u16ratio; // for better quality
    }
    else
    {
        u16HorizontalTotal = u16DefaultHtotal;
    }

    // Calculate pixel clock, the unit is MHz
#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE
    u16PixClk = MDrv_XC_cal_pixclk(pInstance,u16HorizontalTotal, eWindow);
#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE

    MDrv_XC_ADC_clamp_placement_setting(pInstance,u16PixClk);
#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
}
void MApi_XC_Pre_ADC_SetMode(MS_U16 u16DefaultPhase, MS_U16 u16DefaultHtotal, MS_BOOL bHDuplicate, INPUT_SOURCE_TYPE_t enSourceType, SCALER_WIN eWindow)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_Offline_ADC_Setmode XCArgs;
    XCArgs.u16DefaultPhase = u16DefaultPhase;
    XCArgs.u16DefaultHtotal = u16DefaultHtotal;
    XCArgs.bHDuplicate = bHDuplicate;
    XCArgs.enSourceType = enSourceType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_PRE_ADC_SETMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get SWDSIndex
/// @return @ref MS_U8 return SWDSIndex
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetSWDSIndex_U2(void* pInstance)
{

    MS_U8 u8SWDSIndex;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_PRINTF("[%s][%d]pPNLInstancePrivate->u32DeviceID= %lx\n", __FUNCTION__, __LINE__, psXCInstPri->u32DeviceID);

#if ENABLE_SW_DS
    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;
    u8SWDSIndex = pXCResourcePrivate->sthal_SC.u8DynamicScalingNextIndex[MAIN_WINDOW][mvop_sel];
#else
    u8SWDSIndex = 0;
#endif

    XC_PRINTF("[%s,%5d] u8SWDSIndex:%d  \n",__func__,__LINE__,u8SWDSIndex);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE

    return u8SWDSIndex;

}

__attribute__((weak)) MS_U8 MApi_XC_GetSWDSIndex(SCALER_WIN eWindow)
{

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_SWDSIndex XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SWDSIndex, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)  //E_XC_CMD_GET_SWDSIndex
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }

}

void MApi_XC_Set_SVP_Status_U2(void* pInstance,MS_BOOL bIsSecure)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsSVP = bIsSecure;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsSVP = bIsSecure;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return ;
}


void MApi_XC_Set_SVP_Status(MS_BOOL bIsSecure)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }
    stXC_SET_SVP_STATUS XCArgs;
    XCArgs.bReturnValue = bIsSecure;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SVP_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}


MS_BOOL MApi_XC_Get_SVP_Status_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->bIsSVP;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->bIsSVP;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_Get_SVP_Status(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_SVP_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SVP_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

void MApi_XC_Set_VTBlock_Status_U2(void* pInstance,MS_BOOL bIsSecure)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsVTBlock= bIsSecure;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bIsVTBlock= bIsSecure;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return ;
}


void MApi_XC_Set_VTBlock_Status(MS_BOOL bIsSecure)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }
    stXC_SET_VTBlock_STATUS XCArgs;
    XCArgs.bReturnValue = bIsSecure;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VTBlock_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}


MS_BOOL MApi_XC_Get_VTBlock_Status_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->bIsVTBlock;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->bIsVTBlock;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_Get_VTBlock_Status(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_VTBlock_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VTBlock_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

MS_U64 MApi_XC_Get_CMA_Addr_U2(void *pInstance, XC_CMA_CLIENT enCmaClient, MS_U32 u32GetCMASize, SCALER_WIN eWindow)
{
    return MHal_XC_Get_CMA_Addr(pInstance, enCmaClient, u32GetCMASize, eWindow);
}

MS_U64 MApi_XC_Get_CMA_Addr(XC_CMA_CLIENT enCmaClient, MS_U32 u32GetCMASize, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_CMA_ADDR XCArgs;
    XCArgs.enCmaClient = enCmaClient;
    XCArgs.u32GetCMASize = u32GetCMASize;
    XCArgs.eWindow = eWindow;
    XCArgs.u64ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CMA_ADDR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.u64ReturnValue;
    }
}

MS_BOOL MApi_XC_Release_CMA_U2(void *pInstance, XC_CMA_CLIENT enCmaClient, SCALER_WIN eWindow)
{
    return MHal_XC_Release_CMA(pInstance, enCmaClient, eWindow);
}

MS_BOOL MApi_XC_Release_CMA(XC_CMA_CLIENT enCmaClient, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_RELEASE_CMA XCArgs;
    XCArgs.enCmaClient = enCmaClient;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_RELEASE_CMA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_CMA_Init_U2(void* pInstance)	
{	
    MS_BOOL ret = FALSE;
   	
#if (XC_SUPPORT_CMA == TRUE)	

   stXC_CMA_Config[CMA_XC_SELF_MEM].u32HeapID = ION_XC_HEAP_ID;  //FIXME //22 is correct	16542
#if defined(CHIP_MONET)	
   stXC_CMA_Config[CMA_XC_COBUFF_MEM].u32HeapID = XC_DUAL_MIU_HID;	
#else	
   stXC_CMA_Config[CMA_XC_COBUFF_MEM].u32HeapID = 0xff;//XC_INVALID_HEAP_ID;	16546
#endif	
   stXC_CMA_Config[CMA_XC_SELF_MEM].u64AddrHeapOffset = 0;	
   stXC_CMA_Config[CMA_XC_COBUFF_MEM].u64AddrHeapOffset = 0;	

   /* 1. do CMA_POOL_INIT */	
   CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_id     = stXC_CMA_Config[CMA_XC_SELF_MEM].u32HeapID;	
   CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].flags       = CMA_FLAG_MAP_VMA;	
   MsOS_DelayTaskUs(1);	
   XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d][CMA], CMA self heapid = %d!\n",__func__, __LINE__,CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_id);	

   ret = MApi_CMA_Pool_Init(&CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM]);	

   if(ret == FALSE)	
   {	
       printf("\033[35m   Function = %s, Line = %d, CMA_POOL_INIT ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);	
       //assert(0);	
   }	
   else	
   {	
       //printf("\033[35m  Function = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].pool_handle_id);	
       //printf("\033[35m  Function = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__,__LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].miu);	
       //printf("\033[35m  Function = %s, Line = %d, get heap_miu_start_offset is 0x%lx\033[m\n", __PRETTY_FUNCTION__, __LINE__, (MS_U32)CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_miu_start_offset);	
       //printf("\033[35m  Function = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, CMA_Pool_Init_PARAM[CMA_XC_SELF_MEM].heap_length);	
   }	

#endif	

    return ret;      //CID:117616	
}	

MS_BOOL MApi_XC_CMA_Init()
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }

    stXC_CMA_INIT XCArgs;
    XCArgs.bReturnValue = TRUE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CMA_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");	
        return FALSE;	
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_SetOPWriteOff_U2(void* pInstance,MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    Hal_SC_SetOPWriteOff(pInstance, bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    Hal_SC_SetOPWriteOff(pInstance, bEnable);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return ;
}

void MApi_XC_SetOPWriteOff(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }
    stXC_SET_OPWRITE_OFF XCArgs;
    XCArgs.bReturnValue = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OPWrite_OFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}


MS_BOOL MApi_XC_GetOPWriteOff_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = Hal_SC_GetOPWriteOff(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = Hal_SC_GetOPWriteOff(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_GetOPWriteOff(void)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return FALSE;
        }
    }

    stXC_GET_OPWRITE_OFF XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OPWrite_OFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}


#if 1
MS_BOOL MApi_XC2_Init(XC_INITDATA *pXC_InitData, MS_U32 u32InitDataLen)
{
    MS_U32 u32deviceID = 1;

    if (pu32XCInst_1 == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return FALSE;
        }
    }
    stXC_INIT XCArgs;
    XCArgs.pXC_InitData = pXC_InitData;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.bReturnValue = FALSE;


//   XCArgs.pXC_InitData = pXC_InitData;
//   XCArgs.u32InitDataLen = u32InitDataLen;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
void MApi_XC2_Mux_Init(void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count ) )
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_INIT XCArgs;
    XCArgs.input_source_to_input_port = input_source_to_input_port;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
MS_BOOL MApi_XC2_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


void MApi_XC2_SetInputSource( INPUT_SOURCE_TYPE_t enInputSourceType,SCALER_WIN eWindow )
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_INPUTSOURCE XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_INPUTSOURCE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC2_Get_SkipWaitVsync( MS_BOOL eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SKIP_WAIT_VSYNC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bIsSkipWaitVsyn = FALSE;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC2_SkipWaitVsync( MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SKIP_WAIT_VSYNC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bIsSkipWaitVsyn = bIsSkipWaitVsyn;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


MS_BOOL MApi_XC2_Set_XC_VOP(MS_XC_VOP_Data stXCVOPData)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SetXC_VOP XCArgs;
    XCArgs.stXC_VOPData = stXCVOPData;
    XCArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_XC_VOP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturn;
    }

}

MS_BOOL MApi_XC2_GetStatus(XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_STATUS XCArgs;
    XCArgs.pDrvStatus = pDrvStatus;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC2_GenerateBlackVideo ( MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BLACKVIDEO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_BLACKVIDEO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
void MApi_XC2_PQ_LoadFunction(PQ_Function_Info* function_Info , MS_U32 u32InitDataLen)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PQ_LOADFUNCTION XCArgs;
    XCArgs.function_Info = function_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_PQ_LOADFUNCTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC2_EnableMCDI(MS_BOOL bEnable, E_XC_MCDI_TYPE eType)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MCDI_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eType = eType;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MCDI_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_S16 MApi_XC2_Mux_DeletePath(INPUT_SOURCE_TYPE_t src, E_DEST_TYPE dest)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_DELETE_PATH XCArgs;
    XCArgs.src = src;
    XCArgs.dest = dest;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_DELETE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_S16 MApi_XC2_Mux_CreatePath(XC_MUX_PATH_INFO* Path_Info , MS_U32 u32InitDataLen)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_CREATE_PATH XCArgs;
    XCArgs.Path_Info = Path_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_CREATE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_S16 MApi_XC2_Mux_EnablePath(MS_U16 PathId)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_ENABLE_PATH XCArgs;
    XCArgs.PathId = PathId;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_ENABLE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

void MApi_XC2_Mux_TriggerPathSyncEvent( INPUT_SOURCE_TYPE_t src , void* para)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_TRIGGER_PATH_SYNC_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_TRIGGER_PATH_SYNC_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC2_Mux_TriggerDestOnOffEvent( INPUT_SOURCE_TYPE_t src , void* para)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_TRIGGER_DEST_ONOFF_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_TRIGGER_DEST_ONOFF_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC2_Mux_SourceMonitor(MS_BOOL bRealTimeMonitorOnly)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MUX_SOURCE_MONITOR XCArgs;
    XCArgs.bRealTimeMonitorOnly = bRealTimeMonitorOnly;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_SOURCE_MONITOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC2_SetDbgLevel(MS_U16 u16DbgSwitch, E_XC_MESSAGE_TYPE eMessageType)
{
    if (pu32XCInst_1 == NULL)
    {
        MS_U32 u32deviceID = 1;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_SET_DBG_LEVEL XCArgs;
    XCArgs.u16DbgSwitch = u16DbgSwitch;
    XCArgs.eMessageType = eMessageType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_DBG_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_S16 MApi_XC2_Mux_OnOffPeriodicHandler( INPUT_SOURCE_TYPE_t src, MS_BOOL bEnable)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return -1;
    }

    stXC_SET_MUX_ONOFF_PERIODIC_HANDLER XCArgs;
    XCArgs.src = src;
    XCArgs.bEnable = bEnable;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_MUX_ONOFF_PERIODIC_HANDLER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_U32 MApi_XC2_GetCapability(MS_U32 u32Id)
{
    MS_U32 u32deviceID = 1;

    if (pu32XCInst_1 == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_GET_CAPABILITY XCArgs;
    XCArgs.u32Id = u32Id;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_CAPABILITY, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC2_PreInit( E_XC_PREINIT_t eType ,void* para, MS_U32 u32Length)
{
    MS_U32 u32deviceID = 1;

    if (pu32XCInst_1 == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &u32deviceID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_PREINIT XCArgs;
    XCArgs.eType = eType;
    XCArgs.para = para;
    XCArgs.u32Length = u32Length;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_PREINIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
void MApi_XC2_GetSyncStatus(INPUT_SOURCE_TYPE_t eCurrentSrc, XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_SYNC_STATUS XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.sXC_Sync_Status = sXC_Sync_Status;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_SYNC_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

