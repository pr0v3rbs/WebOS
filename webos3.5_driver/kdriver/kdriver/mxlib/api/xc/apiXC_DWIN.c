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
/// file  apiXC_Adc.c
/// brief  Scaler API layer Interface
/// author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_DWIN_C_
#define _API_XC_DWIN_C_
#endif
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
#include "MsOS.h"
#ifdef UTOPIA_V2
#include "utopia.h"
#include "utopia_driver_id.h"
#include "apiXC_DWIN_v2.h"
#endif
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "mvideo_context.h"
#include "drv_sc_DIP_scaling.h"


// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif
//#define TRACE_ADC_INPUT(x) //x
#define msg_video(x) //x
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef UTOPIA_V2
void* pInstantDip[MAX_DIP_WINDOW] = {NULL};
void* pInstantDipUser[MAX_DIP_WINDOW] = {NULL};
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//MS_XC_MEM_FMT s_DIPeMemFmt[MAX_WINDOW] = {E_MS_XC_MEM_FMT_AUTO, E_MS_XC_MEM_FMT_AUTO};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#define DIPDRIVER_BASE 0

/////////////////////////////////////////
// Configuration
/////////////////////////////////////////
#if 0
// Path
typedef struct s_Data_Path
{

    //////////////////////////////
    // Path Type
    //////////////////////////////
    E_PATH_TYPE e_PathType;

    //////////////////////////////
    // Path Status
    //////////////////////////////

    /*
        Make the signal go through the path. If this flag is not enabled,
        synchronous_event_handler and source_monitor are useless.
     */
    #define DATA_PATH_ENABLE BIT(0)

    /*
        Used to identify a path is created or not. ( 1: path created )
     */
    #define DATA_PATH_USING BIT(1)

    /*
        Used to identify the priodic handler enabled or disabled
     */
    #define ENABLE_PERIODIC_HANDLER BIT(2)

    MS_U8 u8PathStatus;

    //////////////////////////////
    // A path has a format like Src -> Dest
    //////////////////////////////
    INPUT_SOURCE_TYPE_t src;
    E_DEST_TYPE dest;

    //////////////////////////////
    // thread for source.
    //////////////////////////////
    void (*path_thread)(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when synchronous event triggered.
    ///////////////////////////////////////
    void (* synchronous_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when destination on/off event triggered.
    ///////////////////////////////////////
    void (* dest_on_off_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed periodically after enabled destinatio period handler
    ///////////////////////////////////////
    void (* dest_periodic_handler )(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);
}MUX_DATA_PATH;
#endif

#if 0
// Path Collection
typedef struct s_Mux_InputSource_Attr
{
    // The number of pathes which is used right now.
    MS_U8  u8PathCount;

    void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count );
    // Pathes.
    // Path array in MAPP_MUX is constructed [Sync path, Sync path, ..., Async path, Async path]
    MUX_DATA_PATH Mux_DataPaths[MAX_DATA_PATH_SUPPORTED];

    // indicate how many times the src created
    MS_U16 u16SrcCreateTime[INPUT_SOURCE_NUM];
}InputSource_Mux_Attr;

#endif
E_APIXC_ReturnValue MApi_XC_DIP_QueryResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] != NULL)
    {
        printf("DIP already Opened\n");
    }
    else if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Open DIP fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    //Query Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_QUERY_RESOURCE,(MS_U32* )pResource) == UTOPIA_STATUS_NO_RESOURCE)
    {
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

E_APIXC_ReturnValue MApi_XC_DIP_GetResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] != NULL)
    {
        printf("DIP already Opened\n");
    }
    else if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Open DIP fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    //Get Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_GET_RESOURCE,(MS_U32* )pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MApi_XC_DIP_ReleaseResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] != NULL)
    {
        printf("DIP already Opened\n");
    }
    else if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Open DIP fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    //Free Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_RELEASE_RESOURCE,(MS_U32* )pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//MS_BOOL PreScaling = FALSE;
//-------------------------------------------------------------------------------------------------
/// Reset DIP for restart capture from buffer 0
/// @return E_APIXC_RET_OK - Success
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SWReset(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_RESET;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(MS_U32* )pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_RESET fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
	MDrv_XC_DIP_SWReset(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}


//-------------------------------------------------------------------------------------------------
/// Init the status for DIP path
/// @return E_APIXC_RET_OK - Success
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_InitByDIP(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] != NULL)
    {
        printf("DIP already Opened\n");
    }
    else if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Open DIP fail\n");
		utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
	//init
	pResource->eWindow = eWindow;
	if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INIT,(MS_U32* )pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_INIT fail\n");
		utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }

    utopia_free(pResource);
#else
	MDrv_XC_DIP_Init(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
///Set the frame buffer address for specific window
/// @param  u32FBAddress      \b IN: the frame buffer address
/// @param  u32FBSize           \b IN: the frame buffer size
/// @param  eWindow             \b IN: set address for which window(DIP_WINDOW)
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetFrameBufferAddress(MS_PHYADDR u32FBAddress, MS_PHYADDR u32FBSize, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(NULL,DISABLE, eWindow);
    MDrv_XC_DWIN_SetFrameBufferAddress(NULL,u32FBAddress, u32FBSize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for specify input source type for DIP path
/// @param enInputSourceType                  \b IN: the inputsource type
/// @param  eWindow                                \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetInputSource(INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
	MDrv_XC_DWIN_SetInputSource(NULL,enInputSourceType, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for get input source type for DIP path
/// @param enInputSourceType                  \b OUT: the inputsource type
/// @param  eWindow                                \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_GetInputSource(INPUT_SOURCE_TYPE_t *enInputSourceType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_GetInputSource(NULL,enInputSourceType,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable input source for DIP path
/// @param bEnable                  \b IN: enable or disable inputsource
/// @param  eWindow               \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_DisableInputSource(MS_BOOL bDisable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_DisableInputSource(NULL,bDisable, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the specific window
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen             \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                     \b IN: which window we are going to set
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_WINDOW* pSetWindow = NULL;

    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWindow = (DIP_SET_WINDOW*)utopia_malloc(sizeof(DIP_SET_WINDOW));
    if (pSetWindow == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetWindow->u32DataLen = u32InitDataLen;
    memcpy(&(pSetWindow->WinInfo),pstXC_SetWin_Info, sizeof(XC_SETWIN_INFO));
	pSetWindow->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINDOW,(MS_U32* )pSetWindow) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_WINDOW fail\n");
		utopia_free(pSetWindow);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetWindow);

    return E_APIXC_RET_OK;
#else
	MDrv_XC_DIP_SetWindow(NULL,pstXC_SetWin_Info,u32InitDataLen,eWindow);
#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API to get DE window for DIP path
/// @param psWin                  \b OUT: the DE window info to get
/// @param  eWindow             \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_GetDEWindow(MS_WINDOW_TYPE *psWin, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_get_DE_Window(NULL,psWin, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//---------------------------------

//---- Aileen
//-------------------------------------------------------------------------------------------------
/// Set the Vertical start of capture window for specific window
/// @param  u16Vstart        \b IN: the window info to get
/// @param  eWindow          \b IN: for which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVstart(MS_U16 u16Vstart , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_v_start(NULL,u16Vstart, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the horizontal start of capture window for specific window
/// @param  u16Hstart        \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHstart(MS_U16 u16Hstart , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_h_start(NULL,u16Hstart, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the Vertical size of capture window for specific window
/// @param  u16Vsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVsize(MS_U16 u16Vsize , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_v_size(NULL,u16Vsize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the Horizontal size of capture window for specific window
/// @param  u16Hsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHsize(MS_U16 u16Hsize , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_h_size(NULL,u16Hsize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Get the capture window for specific window
/// @param  capture_win      \b OUT: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to get
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_GetCaptureWindow(MS_WINDOW_TYPE* capture_win, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_get_capture_window(NULL,capture_win, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable RGB to YUV color space convertion
/// @param bEnable \b IN: enable or disable
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnableR2Y(MS_BOOL benable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bR2Y = benable;
    pSetProperty->eFlag = DIP_PRO_R2Y;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_R2Y fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DWIN_EnableR2YCSC(NULL,benable, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//=============== DIP =====================//
//-------------------------------------------------------------------------------------------------
/// Set  FrameRateCtrl
/// @param  bEnable           \b IN: FrameRateCtrl enable or not
/// @param  u16Src             \b IN: Src Frame
/// @param  u16Dst            \b IN: Dst Frame
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_FrameRateCtrl(MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stFrameRateControl.bFrameRateCtrl = bEnable;
    pSetProperty->Property.stFrameRateControl.u16In = u16In;
    pSetProperty->Property.stFrameRateControl.u16Out = u16Out;
    pSetProperty->eFlag = DIP_PRO_FRC;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_FRC fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetFRC(NULL,bEnable,u16In,u16Out,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set DIP win property
/// @param  pstDIPWinProperty     \b IN: @ref ST_DIP_WINPROPERTY
/// @param  eWindow         \b IN: the window
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetDIPWinProperty(ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_WIN_PROPERTY* pSetWinProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWinProperty = (DIP_SET_WIN_PROPERTY*)utopia_malloc(sizeof(DIP_SET_WIN_PROPERTY));
    if (pSetWinProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetWinProperty->WinProperty),pstDIPWinProperty, sizeof(ST_XC_DIP_WINPROPERTY));
	pSetWinProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINPROPERTY,(MS_U32* )pSetWinProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_WINPROPERTY fail\n");
		utopia_free(pSetWinProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetWinProperty);
#else
	MDrv_XC_DIP_SetWinProperty(NULL,pstDIPWinProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Get DWIN GetBufferInfo
//-------------------------------------------------------------------------------------------------
BUFFER_INFO MApi_XC_DIP_GetBufInfo(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	BUFFER_INFO BufInfo;
    DIP_BUF_INFO* pBufInfo = NULL;
	DIP_RESOURCE* pResource = NULL;

    memset(&BufInfo, 0, sizeof(BUFFER_INFO));
    if(pInstantDip[eWindow] == NULL)
    {
    	pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
		if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
		{
			printf("Open DIP fail\n");
			utopia_free(pResource);
			return BufInfo;
		}
		utopia_free(pResource);
    }

    pBufInfo = (DIP_BUF_INFO*)utopia_malloc(sizeof(DIP_BUF_INFO));
    if (pBufInfo == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return BufInfo;
    }
	memset(&(pBufInfo->BufInfo), 0, sizeof(BUFFER_INFO));
	pBufInfo->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_GET_BUFINFO,(MS_U32* )pBufInfo) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_GET_BUFINFO fail\n");
		utopia_free(pBufInfo);
        return BufInfo;
    }
    memcpy(&BufInfo,&(pBufInfo->BufInfo), sizeof(BUFFER_INFO));
    utopia_free(pBufInfo);
    return BufInfo;
#else
	return MDrv_XC_DIP_GetBufferInfo(NULL,eWindow);
#endif
#else
    BUFFER_INFO BufInfo;
    memset(&BufInfo, 0, sizeof(BUFFER_INFO));
	return BufInfo;
#endif

}

/********************************************************************************/
/// Set gop dwin capture one frame
/// @return E_APIXC_RET_OK - Success
/********************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrame(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_ONESHOT;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(MS_U32* )pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ONESHOT fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
    MDrv_XC_DIP_CaptureOneFrame(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/********************************************************************************/
/// Set XC dwin capture one frame
/// @return E_APIXC_RET_OK - Success
/********************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrameFast(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_ONESHOTFAST;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(MS_U32* )pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ONESHOTFAST fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
    MDrv_XC_DIP_CaptureOneFrame2(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// clear DWIN interrupt
/// @param u8mask \b IN: interrup mask
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_ClearInt(MS_U16 u8mask, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->IntrStautus = u8mask;
    pIntrStus->eFlag = DIP_INTR_CLEAR;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(MS_U32* )pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_CLEAR fail\n");
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else
    MDrv_XC_DIP_ClearIntr(NULL,u8mask,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable DWIN
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_Ena(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;

    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    if(bEnable)
        pTrigMode->eTrigMode = DIP_TRIG_ON;
    else
        pTrigMode->eTrigMode = DIP_TRIG_OFF;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(MS_U32* )pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        if(bEnable)
            printf("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ON fail\n");
        else
            printf("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_OFF fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
	MDrv_XC_DIP_EnableCaptureStream(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/******************************************************************************/
/// Set interrupt mask of XC DWIN.
/// @param u8mask \b IN bitmask of interrupt type
///   - bit7 DWIN VSYNC interrupt mask
///   - bit6 Dump Window interlace Bottom Field ACK Interrupt Mask
///   - bit5 Dump Window interlace Top Field ACK Interrupt Mask
///   - bit4 Dump Window Progressive ACK Interrupt Mask
///   - bit3:1 Reserved
///   - bit0 GWIN VSYNC Interrupt Mask
/// @param bEnable \b IN
///   - # TRUE enable interrupts specified by u8mask
///   - # FALSE disable interrupts specified by u8mask
/// @return E_APIXC_RET_OK - Success
/******************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_EnaInt(MS_U16 u8mask, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->IntrStautus = u8mask;
    if(bEnable)
        pIntrStus->eFlag = DIP_INTR_ENABLE;
    else
        pIntrStus->eFlag = DIP_INTR_DISABLE;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(MS_U32* )pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        if(bEnable)
            printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_ENABLE fail\n");
        else
            printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_DISABLE fail\n");
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else
    MDrv_XC_DIP_EnableIntr(NULL,u8mask, bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/********************************************************************************/
/// Get XC Dwin interrupt status
/// @return XC Dwin interrupt status
/********************************************************************************/
MS_U16 MApi_XC_DIP_GetIntStatus(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	MS_U16 u16IntrStus = 0;
    DIP_INTER_STUS* pIntrStus = NULL;
	DIP_RESOURCE* pResource = NULL;
	if(pInstantDip[eWindow] == NULL)
	{
		pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
		if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
		{
			printf("Open DIP fail\n");
			utopia_free(pResource);
			return u16IntrStus;
		}

		utopia_free(pResource);
	}
    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return u16IntrStus;
    }
    pIntrStus->eFlag = DIP_INTR_GET;
	pIntrStus->IntrStautus=0;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(MS_U32* )pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_GET fail\n");
		utopia_free(pIntrStus);
        return u16IntrStus;
    }
    u16IntrStus = pIntrStus->IntrStautus;
    utopia_free(pIntrStus);

    return u16IntrStus;
#else
    return MDrv_XC_DIP_GetIntrStatus(NULL,eWindow);
#endif
#else
    MS_U16 u16IntrStus = 0;
	return u16IntrStus;
#endif

}

//******************************************************************************
/// Set DWIN data source scan type
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetSourceScanType(EN_XC_DIP_SCAN_MODE enScanMode, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.eScanMode = enScanMode;
    pSetProperty->eFlag = DIP_PRO_SCANMODE;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SCANMODE fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SelectSourceScanType(NULL,(EN_XC_DWIN_SCAN_TYPE)enScanMode,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable DWIN Interlace write
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnaInterlaceWrite(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bInterlaceW = bEnable;
    pSetProperty->eFlag = DIP_PRO_INTERLACE_W;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_INTERLACE_W fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetInterlaceWrite(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN Y2R
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnableY2R(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bY2R = bEnable;
    pSetProperty->eFlag = DIP_PRO_Y2R;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_Y2R fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetY2R(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// Set DWIN Capture format
/// @param fmt \b IN: buffer format to store DWIN content
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetOutputDataFmt(EN_XC_DIP_DATA_FMT fmt, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.eDataFmt = fmt;
    pSetProperty->eFlag = DIP_PRO_DATAFMT;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_DATAFMT fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetDataFmt(NULL,(EN_DRV_XC_DWIN_DATA_FMT)fmt,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
/*****************************************************************************/
/// Set DWIN Capture constant alpha value for format ARGB8888
/// @param u8AlphaVal \b IN: constant alpha value
/// @return E_APIXC_RET_OK - Success
/*****************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_SetAlpha(MS_U8 u8AlphaVal, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.u8Alpha = u8AlphaVal;
    pSetProperty->eFlag = DIP_PRO_ALPHA;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_ALPHA fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetAlphaValue(NULL,u8AlphaVal,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN UV SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapUV(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bSwapUV = bEnable;
    pSetProperty->eFlag = DIP_PRO_SWAPUV;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPUV fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetUVSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN YC SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapYC(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bSwapYC = bEnable;
    pSetProperty->eFlag = DIP_PRO_SWAPYC;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPYC fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetYCSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN RGB SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapRGB(MS_BOOL bEnable, EN_XC_DIP_RGB_SWAP_TYPE enSwapType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stSwapRGB.bSwap = bEnable;
    pSetProperty->Property.stSwapRGB.eSwapType = enSwapType;
    pSetProperty->eFlag = DIP_PRO_SWAPRGB;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPRGB fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetRGBSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set output capture mux
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetOutputCapture(MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stOpCapture.bOpCapture = bEnable;
    pSetProperty->Property.stOpCapture.eOpCapture = eOpCapture;
    pSetProperty->eFlag = DIP_PRO_OP_CAPTURE;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_OP_CAPTURE fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetOutputCapture(NULL,bEnable,eOpCapture,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set DIPW H & V Mirror
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetMirror(MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stMirror.bHMirror = bHMirror;
    pSetProperty->Property.stMirror.bVMirror = bVMirror;
    pSetProperty->eFlag = DIP_PRO_MIRROR;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_MIRROR fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetMirror(NULL,bHMirror,bVMirror,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set DIPR Property
/// @return E_APIXC_RET_OK - Success
//******************************************************************************

E_APIXC_ReturnValue MApi_XC_DIP_SetDIPRProperty(ST_XC_DIPR_PROPERTY *pstDIPRProperty,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_DIPR_PROPERTY* pSetDIPRProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetDIPRProperty = (DIP_SET_DIPR_PROPERTY*)utopia_malloc(sizeof(DIP_SET_DIPR_PROPERTY));
    if (pSetDIPRProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetDIPRProperty->DIPRProperty),pstDIPRProperty, sizeof(ST_XC_DIPR_PROPERTY));
	pSetDIPRProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_DIPR_PROPERTY,(MS_U32* )pSetDIPRProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_DIPR_PROPERTY fail\n");
		utopia_free(pSetDIPRProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetDIPRProperty);
#else
    MDrv_XC_DIP_SetDIPRProperty(NULL,pstDIPRProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for MApi_XC_DIP_Rotation
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_Rotation(MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eTmpWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stRotation.bRotation = bRotation;
    pSetProperty->Property.stRotation.eRoDirection = eRoDirection;
    pSetProperty->eFlag = DIP_PRO_ROTATION;
	pSetProperty->eWindow = eTmpWindow;
    if(UtopiaIoctl(pInstantDip[eTmpWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_ROTATION fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetRotation(NULL,bRotation,eRoDirection,eTmpWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_DIP_SetPinpon
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetPinpon(MS_BOOL bPinpon,MS_PHY u32PinponAddr,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_PROPERTY* pSetProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stPinpon.bPinpon = bPinpon;
    pSetProperty->Property.stPinpon.u32PinponAddr = u32PinponAddr;
    pSetProperty->eFlag = DIP_PRO_PINPON;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(MS_U32* )pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_PINPON fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
		MDrv_XC_DIP_SetPinpon(NULL,bPinpon,u32PinponAddr,eWindow);
#endif
		return E_APIXC_RET_OK;
#else
		return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_InterruptAttach
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_InterruptAttach(InterruptCb pIntCb,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;
    if(pInstantDipUser[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP,(void**)&pInstantDipUser[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open DIP fail\n");
            return E_APIXC_RET_FAIL;
        }
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->pIntCb = pIntCb;
    pIntrStus->eFlag = DIP_INTR_ATTACH;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDipUser[eWindow],MDrv_CMD_DIP_INTERRUPT,(MS_U32* )pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_ATTACH fail\n");
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
	MDrv_XC_DIP_InterruptAttach(NULL,pIntCb,eWindow);
	_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
	MDrv_XC_DIP_InterruptAttach(NULL,pIntCb,eWindow);
	_XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);

#endif //#if SC2_ENABLE


#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_InterruptDetach
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_InterruptDetach(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;
    if(pInstantDipUser[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP,(void**)&pInstantDipUser[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open DIP fail\n");
            return E_APIXC_RET_FAIL;
        }
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->eFlag = DIP_INTR_DETACH;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDipUser[eWindow],MDrv_CMD_DIP_INTERRUPT,(MS_U32* )pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_DETACH fail\n");
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
	MDrv_XC_DIP_InterruptDetach(NULL,eWindow);
	_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
   _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
	MDrv_XC_DIP_InterruptDetach(NULL,eWindow);
	_XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);

#endif //#if SC2_ENABLE


#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_DIP_SetPowerState
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
MS_U32 MApi_XC_DIP_SetPowerState(EN_POWER_MODE u16PowerState)
{
#ifndef DISABLE_DIP_FUNCTION
#ifdef UTOPIA_V2

#else

#endif
#endif
    //return UTOPIA_STATUS_SUCCESS;
    return 0;
}
//================================ Obsolete Function Start =======================================
//-------------------------------------------------------------------------------------------------
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!! below function will be "REMOVED" or "REFACTORING" later, please do not use it !!!!!!!!!
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Set DWIN base0 base 1 address
/// @param  u32FBAddress0,u32FBAddress1     \b IN: the framebuffer address
/// @param  eWindow         \b IN: the window
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetWinProperty_Ex(MS_U8 u8BufCnt ,MS_U16 u16Width, MS_U16 u16Height, MS_PHY u32BufStart ,MS_PHY u32BufEnd, SCALER_DIP_SOURCE_TYPE eSource, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

    ST_XC_DIP_WINPROPERTY stDIPWinProperty;
    stDIPWinProperty.u8BufCnt    = u8BufCnt;
    stDIPWinProperty.u16Width    = u16Width;
    stDIPWinProperty.u16Pitch    = 0x0;
    stDIPWinProperty.u16Height   = u16Height;
    stDIPWinProperty.u32BufStart = u32BufStart;
    stDIPWinProperty.u32BufEnd   = u32BufEnd;
    stDIPWinProperty.enSource     = eSource;
#ifdef UTOPIA_V2
	DIP_SET_WIN_PROPERTY* pSetWinProperty = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        printf("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWinProperty = (DIP_SET_WIN_PROPERTY*)utopia_malloc(sizeof(DIP_SET_WIN_PROPERTY));
    if (pSetWinProperty == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetWinProperty->WinProperty),&stDIPWinProperty, sizeof(ST_XC_DIP_WINPROPERTY));
	pSetWinProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINPROPERTY,(MS_U32* )pSetWinProperty) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_SET_WINPROPERTY fail\n");
		utopia_free(pSetWinProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetWinProperty);
#else
    MDrv_XC_DIP_SetWinProperty(NULL,&stDIPWinProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//================================ Obsolete Function End =======================================

//=============== DIP =====================//
