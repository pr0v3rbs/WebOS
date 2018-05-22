//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file  DMX_adp.c
/// @brief  Demux adaption API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/uaccess.h>
#include<linux/kernel.h>

#include "apiDMX.h"
#include "DMX_adp.h"

#include "utopia.h"
#include "utopia_adp.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
#else
#define CPY_FROM_USER                   memcpy
#define CPY_to_USER                     memcpy
#endif

#define DMX_ADP_MSG(_f, _a...)                             //printf(_f, ##_a)

#define LEN_U32_X1                      4
#define LEN_U32_X2                      8
#define LEN_U32_X3                      12
#define LEN_U32_X4                      16


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 DMX_adp_Init(FUtopiaIOctl *pIoctl)
{    
    *pIoctl= (FUtopiaIOctl)DMX_adp_Ioctl;
    return 0;
}

MS_U32 DMX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32                      u32Ret = UTOPIA_STATUS_FAIL, u32data = 1;       

    char                        buffer_arg[128];        
    char                        buffer_array0[128]; 
    char                        buffer_array1[128]; 
    char                        buffer_array2[32];  

    void                            *pNxtPtr = 0, *pNxtPtr1 = 0;

    DMX_CAP_PARAM                   stCapParam; 
    PDMX_TSO_OUTCLK_PARAM           pstTsoOutClkDes     = NULL;

    PDMX_WP_PARAM                   pstWPParamDes       = NULL;
    PDMX_CAP_PARAM                  pstCapParamDes      = NULL;
    PDMX_CAP_EX_PARAM               pstCapExParamDes    = NULL;
    PDMX_FLT_INFO_PARAM             pstFltInfoDes       = NULL;
    PDMX_FLT_EVENT_PARAM            pFltEventDes        = NULL;
    PDMX_FLT_SEC_PAT_PARAM          pFltSecPatDes       = NULL;
    PDMX_CMDRUN_PARAM               pstCmdRunDes        = NULL;
    PDMX_PKT_DISCONCNT_PARAM        pstDicConCntDes     = NULL;
    PDMX_PKT_DROPCNT_PARAM          pstDropCntDes       = NULL;
    PDMX_PKT_LOCKCNT_PARAM          pstLockCntDes       = NULL;
    PDMX_PKT_AVCNT_PARAM            pstAVPktCntDes      = NULL;
    PDMX_TSO_LOCSTRID_PARAM         pstTsoLocStrIdDes   = NULL;
    PDMX_FQ_INIT_PARAM              pstFqInitDes        = NULL;
    PDMX_MSTR_SYNCBY_PARAM          pstMstrSynByDes     = NULL;
    PDMX_PVR_INFO_PARAM             pstPvrInfoDes       = NULL;
    PDMX_PVR_FLTTYPE_PARAM          pstPvrFltTypeDes    = NULL;
    PDMX_FILE_START_PARAM           pstFileStartDes     = NULL;
    PDMX_FILE_INFO_PARAM            pstFileInfoDes      = NULL;
    PDMX_MMFI_START_PARAM           pstMMFIStartDes     = NULL;
    PDMX_PKT_MODE_PARAM             pstPktModeDes       = NULL;
    DMX_OutputPad_Cfg               *pstOutPadCfgDes    = NULL;
    PDMX_TSO_CFG_PARAM              pstTSOConfigure     = NULL;
    
    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }
	
    switch(u32Cmd)
    {
        // General API
        case MApi_CMD_DMX_Init:
        case MApi_CMD_DMX_Resume:   
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSPParam));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSPParam));
            break;

        case MApi_CMD_DMX_Exit:
        case MApi_CMD_DMX_ForceExit:        
        case MApi_CMD_DMX_Suspend:  
        case MApi_CMD_DMX_ChkFwAlive:   
        case MApi_CMD_DMX_Reset:    
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;          

        case MApi_CMD_DMX_SetFwMiuDataAddr:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_WProtectEnable:                       
            pstWPParamDes = (PDMX_WP_PARAM)buffer_arg;
            stCapParam.Quetype = DMX_CAP_TSP_HWWP_SET_NUM;
            stCapParam.pdata = (void*)&u32data;
            u32Ret = UtopiaIoctl(pInstanceTmp,MApi_CMD_DMX_GetCap,(void*)&stCapParam);
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_WP_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)pstWPParamDes->pu32StartAddr, sizeof(MS_U32) * u32data);
            CPY_FROM_USER((void*)buffer_array1, (void*)pstWPParamDes->pu32EndAddr, sizeof(MS_U32) * u32data);
            pstWPParamDes->pu32StartAddr = (MS_U32*)buffer_array0;
            pstWPParamDes->pu32EndAddr = (MS_U32*)buffer_array1;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;
            
        case MApi_CMD_DMX_OrzWProtectEnable:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;

        case MApi_CMD_DMX_ReadDropCount:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_SetPowerState:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_POWSTATE_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_POWSTATE_PARAM));
            break;

        case MApi_CMD_DMX_SetOwner:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);            
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_GetCap:
            pstCapParamDes = (PDMX_CAP_PARAM)buffer_arg;        
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CAP_PARAM));
            if((pstCapParamDes->Quetype >= DMX_CAP_SHAREKEY_FLT_RANGE) && (pstCapParamDes->Quetype <= DMX_CAP_SHAREKEY_FLT2_RANGE))
            {
                u32data = 2;
            }
            pNxtPtr = pstCapParamDes->pdata;
            pstCapParamDes->pdata = (void*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pNxtPtr, pstCapParamDes->pdata, sizeof(MS_U32) * u32data);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_GetCapEx:
            pstCapExParamDes = (PDMX_CAP_EX_PARAM)buffer_arg;        
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CAP_EX_PARAM));
            CPY_FROM_USER((void*)buffer_array0, pstCapExParamDes->StrQuetype, pstCapExParamDes->InputStrLen);
            pNxtPtr = pstCapExParamDes->pdata;
            pstCapExParamDes->StrQuetype = (char*)buffer_array0;
            pstCapExParamDes->pdata = (void*)buffer_array1;              
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            CPY_to_USER(pNxtPtr, pstCapExParamDes->pdata, pstCapExParamDes->OutputSize);
            break;    

        case MApi_CMD_DMX_SetBurstLen:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_BURSTTYPE));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;

        //Flow Control
        case MApi_CMD_DMX_Flow:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLOW_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FLOW_PARAM));
            break;

        case MApi_CMD_DMX_PVR_Flow:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PVRFLOWPARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PVRFLOWPARAM));
            break;

        case MApi_CMD_DMX_Flow_ParlInvert:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);    
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Flow_ENABLE:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);    
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_OutPadCfg:  
            pstOutPadCfgDes = (DMX_OutputPad_Cfg*)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_OutputPad_Cfg));
            CPY_FROM_USER((void*)buffer_array0, pstOutPadCfgDes->pu32Resv, LEN_U32_X1);
            pstOutPadCfgDes->pu32Resv = buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_TsOutPhase:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_FlowDscmbEng:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLOW_DSCMBENG_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FLOW_DSCMBENG_PARAM));
            break;
        case MApi_CMD_DMX_DropScmbPkt:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_DROP_SCMB_PKT_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_DROP_SCMB_PKT_PARAM));
            break;

        //STC API
        case MApi_CMD_DMX_GetPcr:
        case MApi_CMD_DMX_Stc_Get:  
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);    
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_Stc_Set:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_Stc_UpdateCtrl:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Stc_SetOffset:
        case MApi_CMD_DMX_Stc_ClkAdjust:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        //Filter API
        case MApi_CMD_DMX_Open:             
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_TYPE_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FLT_TYPE_PARAM));
            break;

        case MApi_CMD_DMX_Close:
        case MApi_CMD_DMX_Start:    
        case MApi_CMD_DMX_Stop:             
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_Info:
            pstFltInfoDes = (PDMX_FLT_INFO_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_INFO_PARAM)); 
            if(pstFltInfoDes->bSet == TRUE)
            {
                CPY_FROM_USER((void*)buffer_array0, (void*)pstFltInfoDes->pDmxFltInfo, sizeof(DMX_Flt_info));
                CPY_FROM_USER((void*)buffer_array1, (void*)pstFltInfoDes->pDmxFltType, sizeof(DMX_FILTER_TYPE));
            }
            pNxtPtr = (void*)pstFltInfoDes->pDmxFltInfo;
            pNxtPtr1 = (void*)pstFltInfoDes->pDmxFltType;
            pstFltInfoDes->pDmxFltInfo = (DMX_Flt_info*)buffer_array0;
            pstFltInfoDes->pDmxFltType = (DMX_FILTER_TYPE*)buffer_array1;
            
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  

            if(pstFltInfoDes->bSet == FALSE)
            {
                CPY_to_USER(pNxtPtr, (void*)buffer_array0, sizeof(DMX_Flt_info));
                CPY_to_USER(pNxtPtr1, (void*)buffer_array1, sizeof(DMX_FILTER_TYPE));
            }
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_Pid:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);            
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_IsStart:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_CopyData:  //core function must do copy_to_user for mem copy        
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_COPY_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pArgs, (void*)buffer_arg, sizeof(DMX_FLT_COPY_PARAM));
            break;

        case MApi_CMD_DMX_Proc:
            pFltEventDes = (PDMX_FLT_EVENT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_EVENT_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)pFltEventDes->pEvent, sizeof(DMX_EVENT));
            pNxtPtr = (void*)(pFltEventDes->pEvent);
            pFltEventDes->pEvent = (DMX_EVENT*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);    
            CPY_to_USER(pNxtPtr, (void*)pFltEventDes->pEvent, sizeof(DMX_EVENT));
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_ChangeFltSrc:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_TYPE_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FLT_TYPE_PARAM));
            break;

        case MApi_CMD_DMX_GetOwner:
        case MApi_CMD_DMX_GetPESScmbSts:    
        case MApi_CMD_DMX_GetTsScmbSts: 
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_GetFltScmbSts:         
            CPY_FROM_USER((void*)buffer_arg, pArgs, 20);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, 20);
            break;    

        // Section API
        case MApi_CMD_DMX_Sec_Reset:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg,LEN_U32_X2);
            break;

        case MApi_CMD_DMX_Sec_ReadAddr:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg,LEN_U32_X4);
            break;

        case MApi_CMD_DMX_Sec_GetWriteAddr:
        case MApi_CMD_DMX_Sec_GetStartAddr: 
        case MApi_CMD_DMX_Sec_GetEndAddr:   
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Sec_SetPattern:
            pFltSecPatDes = (PDMX_FLT_SEC_PAT_PARAM)buffer_arg;         

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FLT_SEC_PAT_PARAM));
            CPY_FROM_USER(buffer_array0, pFltSecPatDes->pu8Pattern, pFltSecPatDes->u32MatchSize);
            CPY_FROM_USER(buffer_array1, pFltSecPatDes->pu8Mask, pFltSecPatDes->u32MatchSize);
            CPY_FROM_USER(buffer_array2, pFltSecPatDes->pu8NotMask, pFltSecPatDes->u32MatchSize);
            pFltSecPatDes->pu8Pattern   = (MS_U8*)buffer_array0;
            pFltSecPatDes->pu8Mask      = (MS_U8*)buffer_array1;
            pFltSecPatDes->pu8NotMask   = (MS_U8*)buffer_array2;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        //TTX API
        case MApi_CMD_DMX_GetTTXWrite:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_GetAccess:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_ReleaseAccess:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        // AVFIFO control
        case MApi_CMD_DMX_AVFIFO_Reset:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_AVFIFO_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_AVFIFO_PARAM));
            break;

        case MApi_CMD_DMX_AVFIFO_Status:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_AVFIFO_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_AVFIFO_PARAM));
            break;

        case MApi_CMD_DMX_RemoveDupAVFifoPkt:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_AVFIFO_DROP_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_AVFIFO_DROP_PARAM));
            break;

        case MApi_CMD_DMX_AUBD_Enable:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        //PVR Playback API
        case MApi_CMD_DMX_FI_PlayStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_FI_TimestampMode:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_FI_SetPlaybackStampClk:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TIMESTAMP_CLK_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TIMESTAMP_CLK_PARAM));
            break;

        // PVR Engine API
        case MApi_CMD_DMX_Pvr_Open:
            pstPvrInfoDes = (PDMX_PVR_INFO_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PVR_INFO_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstPvrInfoDes->pPvrInfo), sizeof(DMX_Pvr_info));
            pstPvrInfoDes->pPvrInfo = (DMX_Pvr_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_Pvr_Close:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_Pvr_PidOpen:
            pstPvrFltTypeDes = (PDMX_PVR_FLTTYPE_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PVR_FLTTYPE_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PVR_FLTTYPE_PARAM));
            break;

        case MApi_CMD_DMX_Pvr_PidClose:
        case MApi_CMD_DMX_Pvr_Start:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_Pvr_Stop:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Pvr_GetWriteAddr:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Pvr_SetPktMode:
        case MApi_CMD_DMX_Pvr_SetRecordStamp:       
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Pvr_GetRecordStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_Pvr_SetMobfKey:
            CPY_FROM_USER((void*)buffer_arg, pArgs, 20);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, 20);
            break;

        case MApi_CMD_DMX_Pvr_SetRecordStampClk:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TIMESTAMP_CLK_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TIMESTAMP_CLK_PARAM));
            break;

        // File-in Engine API
        case MApi_CMD_DMX_FI_Start:     
            pstFileStartDes = (PDMX_FILE_START_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_START_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FILE_START_PARAM));
            break;

        case MApi_CMD_DMX_FI_Stop:
        case MApi_CMD_DMX_FI_Pause: 
        case MApi_CMD_DMX_FI_Resume:        
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_FI_Info:
        case MApi_CMD_DMX_FI_Eng_Info:
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
            CPY_FROM_USER((void*)buffer_array0, pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
            pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_FI_IsIdle:
        case MApi_CMD_DMX_FI_IsBusy:    
        case MApi_CMD_DMX_FI_IsPause:   
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_FI_CmdQReset:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_FI_CmdQGetEmptyNum:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_FI_BypassFileTimestamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            break;

        case MApi_CMD_DMX_FI_CmdQGetFifoLevel:
        case MApi_CMD_DMX_FI_GetFileTimeStamp:  
        case MApi_CMD_DMX_FI_GetReadAddr:   
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_FI_SetMobfKey:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        //MMFI  API
        case MApi_CMD_DMX_MMFI_IsIdle:
        case MApi_CMD_DMX_MMFI_IsBusy:  
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_MMFI_CmdQReset:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_MMFI_GetEmptyNum:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_MMFI_Start:
            pstMMFIStartDes = (PDMX_MMFI_START_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_MMFI_START_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_MMFI_START_PARAM));
            break;

         case MApi_CMD_DMX_MMFI_GetFileTimeStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

         case MApi_CMD_DMX_MMFI_PidOpen:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_MMFI_FLT_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);    
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_MMFI_FLT_PARAM));
            break;

        case MApi_CMD_DMX_MMFI_PidClose:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3); 
            break;

        case MApi_CMD_DMX_MMFI_GetFifoLevel:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_MMFI_PlayStamp_Set:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3); 
            break;

        case MApi_CMD_DMX_MMFI_PlayStamp_Get:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_MMFI_SetTimeStampClk:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_MMFI_RemoveDupAVPkt:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_AVFIFO_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_AVFIFO_PARAM));
            break;

        case MApi_CMD_DMX_MMFI_SetMOBFKey:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

		case MApi_CMD_DMX_MMFI_Timestamp_En:
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        //Debug API
        case MApi_CMD_DMX_SetDbgLevel:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_DBGMSG_LEVEL));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_GetFwVer:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_CmdRun:
            pstCmdRunDes = (PDMX_CMDRUN_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_CMDRUN_PARAM));
            if(pstCmdRunDes->u32DataNum > 0)
            {
                CPY_FROM_USER((void*)buffer_array0, pstCmdRunDes->pData, sizeof(MS_U32) * pstCmdRunDes->u32DataNum);
            }
            pstCmdRunDes->pData = (void*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);          
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        //Utopia 1.0 compatible API
        case MApi_CMD_DMX_SetFw:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_SetHK:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        case MApi_CMD_DMX_Init_1_0:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_TSPInit_1_0:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSPParam));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSPParam));
            break;

        case MApi_CMD_DMX_ReleseHWSemp:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(LEN_U32_X1));
            break;
        case MApi_CMD_DMX_GetLibVer:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)&u32data);
            CPY_to_USER(pArgs, (void*)(&u32data), LEN_U32_X1);
            break;      

        case MApi_CMD_DMX_STC64ModeEnable:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        //Merge stream API
        case MApi_CMD_DMX_SetPktMode:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_MODE_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_MODE_PARAM));
            break;

        case MApi_CMD_DMX_SetMergeStrSync:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)(&u32data), LEN_U32_X3);
            break;

        //TSO API
        case MApi_CMD_DMX_TSO_FileInfo:
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
            CPY_FROM_USER((void*)buffer_array0, pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
            pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;
            
        case MApi_CMD_DMX_TSO_FileIsIdle:
        case MApi_CMD_DMX_TSO_FileGetCmdQEmptyNum:  
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_TSO_FileStart:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;
            
        case MApi_CMD_DMX_TSO_FileStop:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;
            
        case MApi_CMD_DMX_TSO_FilePlayStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
             CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;
            
        case MApi_CMD_DMX_TSO_FileGetTimeStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;
                
        case MApi_CMD_DMX_TSO_FileBypassStamp:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;
            
        case MApi_CMD_DMX_TSO_FileTimeStampEnable:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        // DBG INFO API
        case MApi_CMD_DMX_DBG_GET_DISCONCNT:
            pstDicConCntDes = (PDMX_PKT_DISCONCNT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_DISCONCNT_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstDicConCntDes->pInfo), sizeof(DMX_DisContiCnt_info));
            pstDicConCntDes->pInfo = (DMX_DisContiCnt_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_DISCONCNT_PARAM));
            break;
            
        case MApi_CMD_DMX_DBG_GET_DROPPKTCNT:
            pstDropCntDes = (PDMX_PKT_DROPCNT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_DROPCNT_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstDropCntDes->pInfo), sizeof(DMX_DropPktCnt_info));
            pstDropCntDes->pInfo = (DMX_DropPktCnt_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_DROPCNT_PARAM));
            break;
            
        case MApi_CMD_DMX_DBG_GET_LOCKPKTCNT:
            pstLockCntDes = (PDMX_PKT_LOCKCNT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_LOCKCNT_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstLockCntDes->pInfo), sizeof(DMX_LockPktCnt_info));
            pstLockCntDes->pInfo = (DMX_LockPktCnt_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_LOCKCNT_PARAM));
            break;
            
        case MApi_CMD_DMX_DBG_GET_AVPKTCNT:
            pstAVPktCntDes = (PDMX_PKT_AVCNT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_AVCNT_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstAVPktCntDes->pInfo), sizeof(DMX_AVPktCnt_info));
            pstAVPktCntDes->pInfo = (DMX_AVPktCnt_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_AVCNT_PARAM));
            break;
            
        case MApi_CMD_DMX_GET_SECTEI_PKTCNT:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_PKT_SECTEI_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_PKT_SECTEI_PARAM));
            break;
            
        case MApi_CMD_DMX_RESET_SECTEI_PKTCNT:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;
            
        case MApi_CMD_DMX_GET_SECDISCON_PKTCNT:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;
            
        case MApi_CMD_DMX_RESET_SECDISCON_PKTCNT:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;

        // BOX Series Only
        case MApi_CMD_DMX_Pvr_CBSize:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER((void*)buffer_arg, pArgs, LEN_U32_X4);
            break;
            
        case MApi_CMD_DMX_Pvr_SetCaMode:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

        case MApi_CMD_DMX_Pvr_IsStart:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_TSO_OutClk:
            pstTsoOutClkDes = (PDMX_TSO_OUTCLK_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSO_OUTCLK_PARAM));          
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSO_OUTCLK_PARAM));            
            break;

        case MApi_CMD_DMX_STCEng_Sel:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_WaitTspCbEvt: 
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSP_IOSIGNAL));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg,sizeof(DMX_TSP_IOSIGNAL));  
            break;
            
        case MApi_CMD_DMX_TSO_LocStrId:
            pstTsoLocStrIdDes = (PDMX_TSO_LOCSTRID_PARAM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSO_LOCSTRID_PARAM));
            if(pstTsoLocStrIdDes->bSet == TRUE)
            {
                CPY_FROM_USER((void*)buffer_array0, (void*)(pstTsoLocStrIdDes->pu8StrId), sizeof(MS_U8));
            }
            pNxtPtr = pstTsoLocStrIdDes->pu8StrId;
            pstTsoLocStrIdDes->pu8StrId = (MS_U8*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  

            if(pstTsoLocStrIdDes->bSet == FALSE)
            {
                CPY_to_USER(pNxtPtr, (void*)(pstTsoLocStrIdDes->pu8StrId),sizeof(MS_U8));
            }            
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_TSO_CmdQReset:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);
            break;        

		case MApi_CMD_DMX_TSO_InputCfg:
			CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSO_InputCfg));
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSO_InputCfg));
			break;

		case MApi_CMD_DMX_TSO_OutputCfg:
			CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_TSO_OutputCfg));
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_TSO_OutputCfg));
			break;
			
		case MApi_CMD_DMX_TSO_OutputEn:		
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
			break;			

        case MApi_CMD_DMX_TSO_Configure:
            pstTSOConfigure = (PDMX_TSO_CFG_PARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            CPY_FROM_USER((void*)buffer_arg, pArgs, pstTSOConfigure->u32TsoCfgDataSize);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, pstTSOConfigure->u32TsoCfgDataSize);
            break;

        //FIQ
        case MApi_CMD_DMX_FQ_SetFltRushPass:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);            
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);     
            break;

        case MApi_CMD_DMX_FQ_Init:
            pstFqInitDes = (PDMX_FQ_INIT_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)pstFqInitDes, pArgs, sizeof(DMX_FQ_INIT_PARAM)); 
            CPY_FROM_USER((void*)buffer_array0, (void*)(pstFqInitDes->pInfo), sizeof(DMX_FQ_Info));
            pstFqInitDes->pInfo = (DMX_FQ_Info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)pstFqInitDes, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_FQ_Exit:
        case MApi_CMD_DMX_FQ_RushEnable:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

        case MApi_CMD_DMX_FQ_SkipRushData:
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FQ_SKIP_RUSH_DATA_PARAM));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, sizeof(DMX_FQ_SKIP_RUSH_DATA_PARAM));
            break;

        case MApi_CMD_DMX_TSO_SvqBufSet:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);  
            break;

        case MApi_CMD_DMX_MStr_SyncBy:  
            pstMstrSynByDes = (PDMX_MSTR_SYNCBY_PARAM)buffer_arg;
            
            CPY_FROM_USER((void*)pstMstrSynByDes, pArgs, sizeof(DMX_MSTR_SYNCBY_PARAM)); 
            if(pstMstrSynByDes->bSet == TRUE)
            {
                CPY_FROM_USER((void*)buffer_array0, (void*)pstMstrSynByDes->pu8SyncByte, sizeof(MS_U8));
            }            
            pNxtPtr = (void*)pstMstrSynByDes->pu8SyncByte;
            pstMstrSynByDes->pu8SyncByte = (MS_U8*)buffer_array0;     
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(pstMstrSynByDes->bSet == FALSE)
            {
                CPY_to_USER(pNxtPtr, (void*)pstMstrSynByDes->pu8SyncByte,sizeof(MS_U8));
            }      
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

        case MApi_CMD_DMX_GetIntCnt:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);  
            break;
            
        case MApi_CMD_DMX_DropEn:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);  
            break;

        case MApi_CMD_DMX_TeiRmErrPkt:
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);  
            break;

        case MApi_CMD_DMX_SetFwDbgParam:
        case MApi_CMD_DMX_PVR_MOBFEn:    
            CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);  
            break;

		case MApi_CMD_DMX_MMFI_Info:			
            pstFileInfoDes = (PDMX_FILE_INFO_PARAM)buffer_arg;
            
			CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(DMX_FILE_INFO_PARAM));
            CPY_FROM_USER((void*)buffer_array0, (void*)pstFileInfoDes->pFileinInfo, sizeof(DMX_Filein_info));
            pstFileInfoDes->pFileinInfo = (DMX_Filein_info*)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);  
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X1);
            break;

		case MApi_CMD_DMX_MMFI_BypassStamp:	
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            break;

		case MApi_CMD_DMX_Get_DbgPortInfo:		
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);
            break;

		case MApi_CMD_DMX_Open_MultiFlt:	
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X3);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X4);
            break;

		case MApi_CMD_DMX_File_PVR_PidOpen:	
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, 20);
            break;

		case MApi_CMD_DMX_File_PVR_PidClose:	
		case MApi_CMD_DMX_File_PVR_Start:		
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);   
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);  
            break;
		
		case MApi_CMD_DMX_File_PVR_Stop:		
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X1);   
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X2);  
			break;

		case MApi_CMD_DMX_Pvr_Pause:
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X2);   
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, LEN_U32_X3);  
			break;	

		case MApi_CMD_DMX_Pvr_Pid:
			CPY_FROM_USER((void*)buffer_arg, pArgs, LEN_U32_X4);   
			u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(pArgs, (void*)buffer_arg, 20);
			break;	
			
        default:
            DMX_ADP_MSG("DMXADP - Unknown commend!!!\n");
            break;
    }

    //printf("[DMX] E %ld\n", u32Ret);
    return u32Ret;
    
}

#endif


