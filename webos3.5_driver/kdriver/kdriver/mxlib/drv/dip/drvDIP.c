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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @file   drvDIP.c
/// This file contains the Mstar DIP driver
/// @author MStar Semiconductor Inc.
/// @brief  DIP
///////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h> //for memcpy, memset
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal Definition
#include "drvDIP.h"
#include "halDIP.h"
#include "drvMMIO.h"


#ifdef  DIP_DEBUG

#define DIP_DBG(x1, args...)            printf(x1, ## args)
#define assert(p)   do {\
                        if (!(p)) {\
                            DIP_DBG("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #p);\
                            while(1);\
                        }\
                    } while (0)

#define ASSERT(arg)                  assert((arg))
#else

#define DIP_DBG(x1, args...)
#define ASSERT(arg) do {\
                        if (!(arg)) {\
                            printf("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #arg);\
                        }\
                    } while (0)

#endif

static MS_S32 s32DIPMutex = -1;

#ifdef DIP_XC_MODE
static DIP_ERRCODE (*fpXCSetDIPInfo)(MS_U16 ,MS_U32 *,MS_U16 ) = NULL;
#endif
//-----------------------------------------------------------------------------
/// Init DIP
/// @param  void \n
///
/// @return void \n
///
/// @note
//-----------------------------------------------------------------------------
void MDrv_DIP_Init(MS_U32 u32InputMode)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)TRUE;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_INIT,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    } 
#endif
    MHal_DIP_Init(u32InputMode);

    s32DIPMutex = MsOS_CreateMutex(E_MSOS_FIFO, "DIP_Mutex", MSOS_PROCESS_SHARED);
    if(s32DIPMutex < -1)
        printf("\n%s, %d, create mutex fail\n", __FUNCTION__, __LINE__);     
}



//-----------------------------------------------------------------------------
/// MAdp_DIP_SetFrameInfo
/// @param
/// @return void \n
/// @note
//-----------------------------------------------------------------------------
void MDrv_DIP_SetFrameInfo(MS_U32 u32FrameX, MS_U32 u32FrameY,
    MS_U32 u32FrameWidth, MS_U32 u32FrameHeight, MS_BOOL bInterLace)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)bInterLace;    
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_SET_SCANTYPE,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }    
#endif
    MHal_DIP_SetFrameInfo(u32FrameX, u32FrameY, u32FrameWidth, u32FrameHeight, bInterLace);

}
//-----------------------------------------------------------------------------
/// MDrv_DIP_InputMode
/// @param
/// @return void \n
/// @note
//-----------------------------------------------------------------------------
void MDrv_DIP_InputMode(MS_U32 u32InputMode)
{
#ifdef DIP_XC_MODE
        MS_U32 u32Info;
    
        u32Info = (MS_U32)u32InputMode;    
        if(fpXCSetDIPInfo!=NULL)
        {
            if(fpXCSetDIPInfo(E_DIP_SET_INPUTMODE,(MS_U32 *)&u32Info,sizeof(u32Info)))
                printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
        }    
#endif
    MHal_DIP_SetIntputMode(u32InputMode);
    
}
//-----------------------------------------------------------------------------
/// MDrv_DIP_SetNRBuf
/// @param
/// @return void \n
/// @note
//-----------------------------------------------------------------------------
void MDrv_DIP_SetNRBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{

    MHal_DIP_SetNRBuff(u8BufCnt, u32BufWidth, u32BufHeight, u32BufStart,  u32BufEnd);
}


void MDrv_DIP_SetDIBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{
#ifdef DIP_XC_MODE
    DIP_PROPERTY DipInfo;

    DipInfo.u8BufCnt = u8BufCnt;
    DipInfo.u16W = u32BufWidth;
    DipInfo.u16H = u32BufHeight;
    DipInfo.u32FbAddr0 = u32BufStart;
    DipInfo.u32FbAddr1 = u32BufEnd;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_SET_INFO ,(MS_U32 *)&DipInfo,sizeof(DIP_PROPERTY)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }                
#endif
    MHal_DIP_SetDIBuff(u8BufCnt, u32BufWidth, u32BufHeight, u32BufStart, u32BufEnd);

}

void MDrv_DIP_GetDIInfo(DI_INFO *pDIInfo)
{

    MHal_DIP_GetDIInfo(pDIInfo);
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_GET_BUFSTUS,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    } 
    pDIInfo->u16Valid_DIBuf = u32Info;
#endif    
}

void MDrv_DIP_GetNRInfo(NR_INFO *pNRInfo)
{

    MHal_DIP_GetNRInfo(pNRInfo);   
}

void MDrv_DIP_EnableNRDI(MS_BOOL bEnableNR, MS_BOOL bEnableSNR, MS_BOOL bEnableTNR, MS_BOOL bEnableDI)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)bEnableDI; 
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_ENABLE  ,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }                
#endif
    MHal_DIP_EnableNRandDI(bEnableNR, bEnableSNR, bEnableTNR, bEnableDI);
}

void MDrv_DIP_GetDIBufCount(MS_U32 *pu32Count)
{
    *pu32Count = (MS_U32)MHal_DIP_GetDiBuffCount();
}

void MDrv_DIP_ClearDIBufStatus(MS_U8 u8Arg)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)u8Arg;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_CLR_BUFSTUS,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }    
#endif
    // BufferIndex from 0 ~ (u8DiBuffCnt-1)
    MHal_DIP_ClearDiBuffStatus(u8Arg);
}


void MDrv_DIP_GetDIFrameCount(MS_U32 *pu32FrameCount)
{
    *pu32FrameCount = MHal_DIP_GetDiBuffFrameCount();
}

void MDrv_DIP_SetYUVOrder(DIP_YUV_ORDER dipOrder)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)dipOrder;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_SET_YUORD,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }                
#endif
    MHal_DIP_SetYUVOrder((MS_U32)dipOrder);
}

void MDrv_DIP_GetDIBufStatus(MS_U32 *pu32DIBufStatus)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_GET_BUFSTUS ,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }   
    *pu32DIBufStatus = u32Info;
#else
    *pu32DIBufStatus = MHal_DIP_GetDiBuffStatus();
#endif

}

void MDrv_DIP_GetDIShow(void)
{

}


void MDrv_DIP_SetMMIOMapBase(void)
{
    MS_U32 u32NonPMBankSize, u32NonPMBankAdd;
    ASSERT(MDrv_MMIO_GetBASE( &u32NonPMBankAdd, &u32NonPMBankSize, MS_MODULE_DIP)==TRUE);
    MHAL_DIP_RegSetBase(u32NonPMBankAdd);
}

void MDrv_DIP_SetWebCamBuff(MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{
#ifdef DIP_XC_MODE
    DIP_WEBCAM_PROPERTY WebCamInfo;
    
    WebCamInfo.u32WebCamAddr0 = u32BufStart;
    WebCamInfo.u32WebCamAddr1 = u32BufEnd;
    WebCamInfo.u16WebCamW = u32BufWidth;
    WebCamInfo.u16WebCamH = u32BufHeight;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_SET_WEBCAM ,(MS_U32 *)&WebCamInfo,sizeof(DIP_WEBCAM_PROPERTY)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }                
#endif
    MHal_DIP_SetWebCamBuff(u32BufWidth, u32BufHeight, u32BufStart, u32BufEnd);
}

void MDrv_DIP_Trigger(void)
{
#ifdef DIP_XC_MODE
    MS_U32 u32Info;

    u32Info = (MS_U32)TRUE;
    if(fpXCSetDIPInfo!=NULL)
    {
        if(fpXCSetDIPInfo(E_DIP_ONESHOT ,(MS_U32 *)&u32Info,sizeof(u32Info)))
            printf("[%s:%d] Error!!!\n",__FUNCTION__,__LINE__);
    }                
#endif
    MHal_DIP_Trigger();
}

MS_BOOL MDrv_DIP_OneShot(MS_U32 u32BufWidth, MS_U32 u32BufHeight
    , MS_U32 u32InputBufStart, MS_U32 u32OutputBufStart, MS_U8 u8OutputBufCnt)
{    
    if (FALSE == MsOS_ObtainMutex(s32DIPMutex, MSOS_WAIT_FOREVER))
    {
        printf("\n%s, %d, obtain mutex fail\n", __FUNCTION__, __LINE__); 
        return FALSE;
    }

    MDrv_DIP_SetWebCamBuff(u32BufWidth, u32BufHeight,u32InputBufStart, u32InputBufStart+u32BufWidth*u32BufHeight*2);
    MDrv_DIP_SetDIBuf(u8OutputBufCnt, u32BufWidth, u32BufHeight, u32OutputBufStart, u32OutputBufStart+u32BufWidth*u32BufHeight*3/2*u8OutputBufCnt);
    MDrv_DIP_Trigger();

    if (FALSE == MsOS_ReleaseMutex(s32DIPMutex))
    {
        printf("\n%s, %d, release mutex fail\n", __FUNCTION__, __LINE__); 
        return FALSE;
    }

    return TRUE;
}   

MS_BOOL MDrv_DIP_RegisterXCSetDIPInfo(DIP_ERRCODE (*fpDIP_CB)(MS_U16 cmd,MS_U32 *arg,MS_U16 size))
{
#ifdef DIP_XC_MODE
    if(fpDIP_CB == NULL)
    {
        printf("[%s:%d]register CB function is Null\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    fpXCSetDIPInfo = fpDIP_CB;
#endif    
    return TRUE;
}

DIP_ERRCODE MDrv_DIP_GetChipCaps(EN_DIP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    switch(eCapType)
    {
        case E_DIP_CAP_CONFIG:
                #ifdef SW_DIP
                    *pRet = E_DIP_CFG_SW;
                #else
                    *pRet = E_DIP_CFG_HW;
                #endif
            break;
        default:
            printf("[%s]not support DIP capability case: %d\n",__FUNCTION__, eCapType);
            return DIP_ERR_FAIL;
            break;
    }
    return DIP_ERR_OK;
}

