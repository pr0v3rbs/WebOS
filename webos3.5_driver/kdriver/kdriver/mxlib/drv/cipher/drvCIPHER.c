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
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvAESDMA.c
// @brief  AESDMA Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvCIPHER.h"
#include "drvMMIO.h"
#include "halRSA.h"
#include "halCIPHER.h"
#include "utopia.h"

#include "drvAESDMA_v2.h"
#include "drvAESDMA_private.h"

#define CIPHER_UTOPIA2 1
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define CIPHERIntEnable()                       MsOS_EnableInterrupt (CRYPTODMA_IRQ)
#define CIPHERIntDisable()                      MsOS_DisableInterrupt(CRYPTODMA_IRQ)
#define CIPHERIntAttach()                       MsOS_AttachInterrupt (CRYPTODMA_IRQ, (InterruptCb )_CIPHER_Isr)
#define CIPHERIntDetach()                       MsOS_DetachInterrupt (CRYPTODMA_IRQ)

#define AESDMAPA2KSEG1(addr)                    ((void *)(((MS_U32)addr) | 0xa0000000)) //physical -> unchched


#define RSAInitChk()                               if(_bRSA_Init == FALSE){ DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]\n", __func__, __LINE__)); return DRVAESDMA_FAIL; }
//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define CIPHER_TASK_STACK_SIZE (4096)
#define CIPHER_CAVID           (0x0F)
#define HASH_HMAC_KEYPAD_SIZE  (64)
#define HASH_RPT_SIZE_ALL      (10)
#define HASH_RPT_SIZE_STATUS   (2)

#define HASH_MD5_DIGEST_SIZE    (16)
#define HASH_SHA1_DIGEST_SIZE   (20)
#define HASH_SHA256_DIGEST_SIZE (32)
#define HASH_BLOCK_SIZE         (64)

#define CIPHER_THREAD_ID_MAX    (0xFFFF)

#define DRV_CIPHER_TA_SHA_CHECK_SIZE    8
#define DRV_CIPHER_TA_LUT3_BITMAP_LUT   0x1
#define DRV_CIPHER_TA_LUT3_BITMAP_M     0x2
#define DRV_CIPHER_TA_LUT3_BITMAP_BC    0x4
#define DRV_CIPHER_TA_KCV_SIZE          16   //HW limitation, need padding to 16 bytes
#define DRV_CIPHER_TA_LUT_SIZE          256
#define DRV_CIPHER_TA_M_SIZE            128
#define DRV_CIPHER_TA_BC_SIZE           16

#define DRV_CIPHER_TA_SHA_FAIL          (0x00000010)
#define DRV_CIPHER_TA_KCV_FAIL          (0x00000020)
#define DRV_CIPHER_TA_LUT_FAIL          (0x00000030)
#define DRV_CIPHER_TA_M_FAIL            (0x00000040)
#define DRV_CIPHER_TA_BC_FAIL           (0x00000050)

#define DRV_CIPHER_NO_REPORT_BUFFER     (0x00000003)

//Interrupt
#define DRV_CIPHER_DMA_DATA_INIT        (0x00000000)
#define DRV_CIPHER_DONE                 (0x00000100)
#define DRV_CIPHER_CALLBACK             (0x0000000F)

//Report status
#define DRV_CIPHER_REPORT_OK            (0x80000000)

typedef struct
{
    MS_U32 u32LUT;  //LUT3 fill in 512 bytes table, each has 9 bits to describe
    MS_U32 u32M;
    MS_U32 u32BC;
}DRV_CIPHER_TA_LUT3;


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
static MS_BOOL _bCIPHERDrv_Inited = FALSE;
MS_S32  _CipherMtx = 0;
MS_S32  _CipherRSAMtx;
MS_S32  _CipherSemaphore;

MS_S32  _AESDMAMtx = 0;

MS_BOOL bDES_Key = FALSE;

#if CIPHER_UTOPIA2
static void *pu32CipherInst = NULL;
#endif

static CIPHER_DBGMSG_LEVEL _CIPHERDBGLevel = CIPHER_DBG_ERR;
static MSIF_Version _drv_cipher_version = {
    .DDI = { CIPHER_DRV_VERSION, },
};

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
//Isr proc
static void*                            _pCIPHERTaskStack;
static MS_U8                            _u8CIPHER_StackBuffer[CIPHER_TASK_STACK_SIZE];
static MS_S32                           _s32CIPHERTaskId;

static MS_U32                           _u32LastErr;

typedef enum
{
    E_CIPHER_DMA_DONE,
    E_CIPHER_HASH_DONE,
}CIPHER_WAIT;

static CIPHER_WAIT _eCipherWait;

#endif

#define CIPHER_REPORT_BUFFER_MAX 2

typedef struct
{
    MS_U32  u32Report;
    MS_U32  u32DoneBytes;
    MS_U32  u32ExcMsg;
    MS_U8   *pu8Digest_Buf;
    MS_U32  u32Digest_Buf_Size;
    P_DrvCIPHER_EvtCallback pfCallback;
    MS_BOOL bUsed;
}CIPHER_REPORT;

static CIPHER_REPORT _CIPHER_ReportBuffer[CIPHER_REPORT_BUFFER_MAX];
static MS_U32 _u32CallbackID = 1;

static MS_U16 _u16ThreadID = 1;
//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

static MS_BOOL                        _bRSA_Init = FALSE;

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
static MS_S32                           _s32CIPHEREventId;

#define _CIPHER_SetEvent(flag)             MsOS_SetEvent(_s32CIPHEREventId, (flag))
#define _CIPHER_GetEvent(events)           MsOS_WaitEvent(_s32CIPHEREventId, DRV_CIPHER_DONE, &events, E_OR_CLEAR, MSOS_WAIT_FOREVER)

#endif
//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define DEBUG_CIPHER(debug_level, x)     do { if (_CIPHERDBGLevel >= (debug_level)) (x); } while(0)


//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)
static void _CIPHER_Isr(void)
{
    MS_U32  u32Rpt[10] = {0};
    MS_U32  u32ExcMsg = 0;
    MS_U32  u32RptId = 0;
    MS_U8   u8BufNum     = 0;
    MS_U8   *pu8Digest_Buf  = NULL;
    MS_U32  u32Digest_Buf_Size = 0;

    HAL_CIPHER_IntClear();

    if(E_CIPHER_DMA_DONE == _eCipherWait)
    {
        HAL_CIPHER_DMA_GetRpt(u32Rpt);
        HAL_CIPHER_GetException(&u32ExcMsg);

        u32RptId = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;
        u8BufNum = u32RptId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

        _CIPHER_ReportBuffer[u8BufNum].u32Report    = u32Rpt[0];
        _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = u32Rpt[1];
        _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg    = u32ExcMsg;

    }
    else  //E_CIPHER_HASH_DONE
    {
        //Get all register report, include status and digest
        HAL_CIPHER_Hash_GetRpt(u32Rpt, HASH_RPT_SIZE_ALL);
        HAL_CIPHER_GetException(&u32ExcMsg);

        u32RptId = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;
        u8BufNum = u32RptId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

        _CIPHER_ReportBuffer[u8BufNum].u32Report    = u32Rpt[0];
        _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = u32Rpt[1];
        _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg    = u32ExcMsg;

        pu8Digest_Buf      = _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf;
        u32Digest_Buf_Size = _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size;

        if((pu8Digest_Buf != NULL) && (u32Digest_Buf_Size != 0))
        {
            //Avoid status report, only copy digest to output address
            memcpy(pu8Digest_Buf, u32Rpt+HASH_RPT_SIZE_STATUS, u32Digest_Buf_Size);
        }

    }

    _u32CallbackID = u32RptId;

    _CIPHER_SetEvent(DRV_CIPHER_DONE); // Trigger task
    CIPHERIntEnable();

}

static void _CIPHER_Isr_Proc(void)
{
    MS_U32  u32Events = 0;
    MS_U32  u32Rpt    = 0;
    MS_BOOL bSuccess  = FALSE;
    MS_U32  u32ExcMsg = 0;
    MS_U32  u32RptId  = 0;
    MS_U8   u8BufNum  = 0;
    P_DrvCIPHER_EvtCallback pfCallBack = NULL;

    while (1)
    {
        _CIPHER_GetEvent(u32Events);

        if (HAS_FLAG(u32Events, DRV_CIPHER_DONE))
        {
            //Handleing callback
            u8BufNum   = _u32CallbackID % (MS_U32)CIPHER_REPORT_BUFFER_MAX;
            u32Rpt     = _CIPHER_ReportBuffer[u8BufNum].u32Report;
            pfCallBack = _CIPHER_ReportBuffer[u8BufNum].pfCallback;
            u32ExcMsg  = HAL_CIPHER_ReadException(_CIPHER_ReportBuffer[u8BufNum].u32ExcMsg);

            u32RptId   = u32Rpt & REG_CIPHER_RPT_THREAD_MSK;

            bSuccess = FALSE;

            if(u32Rpt & DRV_CIPHER_REPORT_OK)
            {
                bSuccess = TRUE;
            }

            if(u32ExcMsg != 0)
            {
                HAL_CIPHER_ResetException();
            }

            _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
            _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
            _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
            _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
            _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
            _CIPHER_ReportBuffer[u8BufNum].pfCallback         = NULL;
            _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

            MsOS_ReleaseSemaphore(_CipherSemaphore);
            if(pfCallBack != NULL)
            {
                pfCallBack(u32RptId, bSuccess, u32ExcMsg);

            }
        }

    } // Task while loop
}

#endif

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret     = DRV_CIPHER_OK;
    MS_U32  u32Rpt[2]  = {0};
    MS_U32  u32RptId   = 0;
    MS_U8   u8BufNum   = 0;
    MS_BOOL bDone      = FALSE;

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    //Check register report
    if(FALSE == HAL_CIPHER_DMA_CmdDone(u32CmdId, &u32Ret))
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:DMA cmd[%lu] NOT done!\n", __func__, __LINE__, u32CmdId));
        bDone = FALSE;
        goto ISDMADONE_FINISH;
    }

    HAL_CIPHER_DMA_GetRpt(u32Rpt);

    u32RptId  = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;

    //Check report id > CmdId, report is overwrite
    if(u32RptId > u32CmdId)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:DMA report is overwrite!\n", __func__, __LINE__));
        bDone = TRUE;
        goto ISDMADONE_FINISH;
    }

    if(u32Ret != 0)
    {
        *pu32Exception = HAL_CIPHER_ReadException(u32Ret);
        HAL_CIPHER_ResetException();
    }

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:DMA fail!\n", __func__, __LINE__));
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISDMADONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_ReadMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret         = DRV_CIPHER_OK;
    MS_U32  u32Rpt[10]     = {0};
    MS_U32  u32RptId       = 0;
    MS_U8   u8BufNum       = 0;
    MS_BOOL bDone          = FALSE;

    MS_U8   *pu8Digest_Buf = NULL;
    MS_U32  u32Digest_Buf_Size = 0;

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    if(FALSE == HAL_CIPHER_Hash_CmdDone(u32CmdId, &u32Ret))
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:HASH cmd[%lu] NOT done!\n", __func__, __LINE__, u32CmdId));
        bDone = FALSE;
        goto ISHASHDONE_FINISH;
    }

    //Get all register report, include status and digest
    HAL_CIPHER_Hash_GetRpt(u32Rpt, HASH_RPT_SIZE_ALL);
    u32RptId = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;

    //Check report id > CmdId, report is overwrite
    if(u32RptId > u32CmdId)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]ERR:HASH report is overwrite!\n", __func__, __LINE__));
        bDone = TRUE;
        goto ISHASHDONE_FINISH;
    }

    pu8Digest_Buf      = _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf;
    u32Digest_Buf_Size = _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size;

    //Avoid status report, only copy digest to output address
    if((pu8Digest_Buf != NULL) && (u32Digest_Buf_Size > 0))
    {
        memcpy( pu8Digest_Buf, u32Rpt+HASH_RPT_SIZE_STATUS, u32Digest_Buf_Size);
    }

    //Reset exception flag for polling mode
    if(u32Ret != 0)
    {
        *pu32Exception = HAL_CIPHER_ReadException(u32Ret);
        HAL_CIPHER_ResetException();
    }

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH fail!\n", __func__, __LINE__));
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISHASHDONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_FlushMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}

#if 0
static void _dump(MS_U8 *ptr, MS_U32 size)
{
    int i = 0;
    printf("[DUMP Start]\n");
    for(i = 0; i < size; i++)
    {
        printf("0x%02x ", *(ptr+i));
    }
    printf("[DUMP Finish]\n");
}
#endif

MS_U32 _AESDMA_MSBF2Native(MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

void _ReverseU8Buf(MS_U8 *buf, MS_U32 len)
{
    MS_U8 i = 0;
    MS_U8 j = len-1;
    MS_U8 temp = 0;

    if((buf==NULL)||(len<=1)) return;

    while(i<j)
    {
         temp   = buf[i];
         buf[i] = buf[j];
         buf[j] = temp;
         i++;
         j--;
     }
}
//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Init(void)
{
    MS_U32 u32Bank, u32BankSize;
    MS_U32 i = 0;

#if CIPHER_UTOPIA2
    if(pu32CipherInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pu32CipherInst, 0x0, NULL))
        {
            printf("UtopiaOpen CIPHER fail\n");
            return DRVAESDMA_FAIL;
        }
    }
#endif

    if(_bCIPHERDrv_Inited == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:Double Init!\n", __FUNCTION__, __LINE__));
        return DRV_CIPHER_OK;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    HAL_CIPHER_SetBank(u32Bank);
    HAL_CIPHER_ResetStatus(TRUE, TRUE);

    HAL_CIPHER_SWReset();

    _u16ThreadID   = 1;
    _u32CallbackID = 1;

    for(i=0;i<CIPHER_REPORT_BUFFER_MAX;i++)
    {
        _CIPHER_ReportBuffer[i].u32Report          = 0;
        _CIPHER_ReportBuffer[i].u32DoneBytes       = 0;
        _CIPHER_ReportBuffer[i].u32ExcMsg          = 0;
        _CIPHER_ReportBuffer[i].pu8Digest_Buf      = NULL;
        _CIPHER_ReportBuffer[i].u32Digest_Buf_Size = 0;
        _CIPHER_ReportBuffer[i].bUsed              = FALSE;
    }

    _CipherMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"CIPHER Mutex", MSOS_PROCESS_SHARED);
	_AESDMAMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"AESDMA Mutex", MSOS_PROCESS_SHARED);
    _CipherSemaphore = MsOS_CreateSemaphore(1, E_MSOS_FIFO, (char *)"CIPHER Semaphore");

	_bCIPHERDrv_Inited = TRUE;
#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    //Isr event
    _s32CIPHEREventId = MsOS_CreateEventGroup("CIPHER_Event");
    if (_s32CIPHEREventId < 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]_s32CIPHEREventId < 0 \n", __FUNCTION__, __LINE__));
        _u32LastErr = DRV_CIPHER_FAIL;
        return _u32LastErr;
    }

    //Task stack
    _pCIPHERTaskStack = _u8CIPHER_StackBuffer;

    //Isr Task
    _s32CIPHERTaskId = MsOS_CreateTask((TaskEntry)_CIPHER_Isr_Proc,
                                        0,
                                        E_TASK_PRI_SYS,
                                        TRUE,
                                        _pCIPHERTaskStack,
                                        CIPHER_TASK_STACK_SIZE,
                                        "CIPHER_Task");

    if (_s32CIPHERTaskId < 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]_s32CIPHERTaskId < 0 \n", __FUNCTION__, __LINE__));
        MsOS_DeleteEventGroup(_s32CIPHEREventId);
        _u32LastErr = DRV_CIPHER_FAIL;
        return _u32LastErr;
    }

    HAL_CIPHER_IntEnable();

    CIPHERIntDisable();
    CIPHERIntDetach();
    CIPHERIntAttach();
    CIPHERIntEnable();

#endif

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Reset(void)
{
	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

	MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE, TRUE);
    HAL_CIPHER_SWReset();
    HAL_CIPHER_ResetException();

    _u16ThreadID   = 1;
    _u32CallbackID = 1;

	DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Cipher Reset!\n", __func__, __LINE__));

    MsOS_ReleaseMutex(_CipherMtx);

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel)
{
	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    _CIPHERDBGLevel = eDBGMsgLevel;
    HAL_CIPHER_SetDbgLevel(eDBGMsgLevel);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Debug level[%d]!\n", __func__, __LINE__,  eDBGMsgLevel));

    MsOS_ReleaseMutex(_CipherMtx);

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return DRV_CIPHER_FAIL;
    }

    *ppVersion = &_drv_cipher_version;

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8  u8BufNum  = 0;

	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.stOutput.u32Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:DMA data size is zero!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:DMA CmdId is NULL!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n", __FUNCTION__, __LINE__, (unsigned int)stCfg.u32CAVid));

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error: Set CAVid [0x%x]fail!\n",
            __func__, __LINE__, (unsigned int)stCfg.u32CAVid));
        goto DMA_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo( stCfg.stAlgo);
    HAL_CIPHER_DMA_Set_Key ( stCfg.stKey);
    HAL_CIPHER_DMA_Set_IV(stCfg.stKey.pu8IVData, stCfg.stKey.u8IVLen);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]\n",
            __func__, __LINE__, stCfg.stAlgo.eMainAlgo, stCfg.stAlgo.eSubAlgo, stCfg.stAlgo.eResAlgo, stCfg.stAlgo.eSBAlgo));

	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stCfg.stInput.eMemoryType, NULL, 0);
    HAL_CIPHER_DMA_Set_OutputDstTo(stCfg.stOutput.eMemoryType, NULL, 0);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input source from [%d], Output source from [%d]\n",
            __func__, __LINE__, stCfg.stInput.eMemoryType, stCfg.stOutput.eMemoryType));

    // Set File-in Addr //
    HAL_CIPHER_DMA_Set_FileinDesc ( stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
    HAL_CIPHER_DMA_Set_FileoutDesc( stCfg.stOutput.u32Addr, stCfg.stOutput.u32Addr + stCfg.stOutput.u32Size - 1);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input address [0x%08x], size[%lu], Output address [0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size, (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size));

    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:CIPHER report buffer is used!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto DMA_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_DMA_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    if(FALSE == HAL_CIPHER_DMA_Start(stCfg.bDecrypt, eIntMode , TRUE , _u16ThreadID))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s]Error:DMA Fail!\n", __func__));
        ret =  DRV_CIPHER_FAIL;
        goto DMA_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
        __func__, __LINE__, eIntMode, _u16ThreadID));

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;

DMA_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;

}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret  = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen    = 32;
    MS_U32 u32HashSrc   = 0;
    MS_U32 u32HashDst   = 0;
    MS_U8  u8BufNum    = 0;

	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH data size is zero!\n", __func__, __LINE__));
        goto HASHAUTO_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH CmdId is NULL!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Wrong buffer size[%lu]!\n", __func__, __LINE__, stCfg.u32Digest_Buf_Size));
        goto HASHAUTO_FINISH;
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid
    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n", __FUNCTION__, __LINE__, (unsigned int)stCfg.u32CAVid));

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error: Set CAVid [0x%x]fail!\n",
            __func__, __LINE__, (unsigned int)stCfg.u32CAVid));
        goto HASHAUTO_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Algorithm [%d]\n",
            __func__, __LINE__, stCfg.eAlgo));

    //Set IV to FIPS
    HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS);
    HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0);

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set source memory type [%d] fail\n",
            __func__, __LINE__, stCfg.stInput.eMemoryType));
        goto HASHAUTO_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Input address[0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));
        goto HASHAUTO_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input address[0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

#if 0 //Default output to Register
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA((MS_U32)stCfg.pu8Digest_Buf), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHAUTO_FINISH;
    }
#endif

	//Set HOS = 1 , Output will be MSB First
	HAL_CIPHER_Hash_SetHOS(TRUE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:CIPHER report buffer is used!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto HASHAUTO_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Digest buffer address[0x%08x], buffer size[%lu]\n",
                __func__, __LINE__, (unsigned int)stCfg.pu8Digest_Buf, stCfg.u32Digest_Buf_Size));

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_HASH_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHAUTO_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
            __func__, __LINE__, eIntMode, _u16ThreadID));

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;
HASHAUTO_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASHManual(DRV_CIPHER_HASHCFG_MANUAL stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    HAL_CIPHER_IWCTYPE  eIWCType = E_CIPHER_HASH_IWC_MANUAL; //Init Word Count
    MS_U32  u32IVSel    = CMD_HASH_IV_FIPS;
    MS_U8   *pu8IV      = NULL;
    MS_U32  u32DataSize = 0;
    MS_U32  u32IVSize   = 0;
    MS_BOOL bAutoPad    = FALSE;
    MS_U32  u32OutLen   = 32;
    MS_U32  u32HashSrc  = 0;
    MS_U32  u32HashDst  = 0;
    MS_U8   u8BufNum    = 0;

	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    //Check data size
    u32DataSize = stCfg.stInput.u32Size;

    if(u32DataSize == 0)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH data size is [%lu]!\n", __func__, __LINE__, u32DataSize));
        goto HASHMANUAL_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH CmdId is NULL!\n", __func__, __LINE__));
        goto HASHMANUAL_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    //Check buffer and size
    if((stCfg.pu8Digest_Buf != NULL) && (stCfg.u32Digest_Buf_Size != u32OutLen))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Wrong buffer size[%lu]!\n", __func__, __LINE__, stCfg.u32Digest_Buf_Size));
        goto HASHMANUAL_FINISH;
    }

    //Check IV setting
    if(stCfg.pu8SetIV != NULL)
    {
        pu8IV = stCfg.pu8SetIV;
        u32IVSel = CMD_HASH_IV_CMD;

        if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)
        {
            u32IVSize = HASH_SHA1_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)
        {
            u32IVSize = HASH_SHA256_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)
        {
            u32IVSize = HASH_MD5_DIGEST_SIZE;
        }
    }
    else
    {
        //NULL IV
        if(stCfg.eIVMode == E_CIPHER_HASH_IV_CMD)
        {
            ret = DRV_CIPHER_BAD_PARAM;
            DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:NULL IV!\n", __func__, __LINE__));
            goto HASHMANUAL_FINISH;
        }
    }

    //IV and IWC is from previous report
    if(stCfg.eIVMode == E_CIPHER_HASH_IV_PRV)
    {
        u32IVSel = CMD_HASH_IV_PRV_RPT;
        eIWCType = E_CIPHER_HASH_IWC_PRV;
    }

    //AutoPad only works on last stage, previous stage is 64 bytes alignment
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        bAutoPad = TRUE;
    }
    else
    {
        if((u32DataSize % (MS_U32)HASH_BLOCK_SIZE) != 0 )
        {
            ret = DRV_CIPHER_BAD_PARAM;
            DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH block is 64 bytes alignment!\n", __func__, __LINE__));
            goto HASHMANUAL_FINISH;
        }
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid
    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n", __FUNCTION__, __LINE__, (unsigned int)stCfg.u32CAVid));

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error: Set CAVid [0x%x]fail!\n",
            __func__, __LINE__, (unsigned int)stCfg.u32CAVid));
        goto HASHMANUAL_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, bAutoPad, FALSE);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Algorithm[%d], AutoPad[%d]\n",
        __func__, __LINE__, stCfg.eAlgo, bAutoPad));

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(pu8IV, u32IVSize , u32IVSel))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set IV fail, IV size[%lu]\n",
        __func__, __LINE__, u32IVSize));
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IV size[%lu], IV Mode[%d]\n",
        __func__, __LINE__, u32IVSize, stCfg.eIVMode));

    //Set Init Word Count
    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(eIWCType, stCfg.u32StartByte))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set IWC fail, IWC[%d], StartBytes[%lu]\n",
        __func__, __LINE__, eIWCType, stCfg.u32StartByte));
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IWC[%d], StartBytes[%lu]\n",
        __func__, __LINE__, eIWCType, stCfg.u32StartByte));

    //Set HMAC Key, must MDrv_Cipher_Init first to set CAVid, I_KeyPad
    HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, stCfg.eHMACKPad, FALSE);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HMAC Key Source[%d], Key Index[%u], KeyPad[%d]\n",
        __func__, __LINE__, stCfg.stHMACKey.eKeySrc, stCfg.stHMACKey.u8KeyIdx, stCfg.eHMACKPad));

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set source memory type [%d] fail\n",
            __func__, __LINE__, stCfg.stInput.eMemoryType));
        goto HASHMANUAL_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Input address[0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input address[0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }
#if 0
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHMANUAL_FINISH;
    }
#endif
	//Set HOS = 1, Output will be MSB First
	//IV auto mode use previous report digest as IV, report must be LSB first
    if(stCfg.pu8Digest_Buf == NULL)
    {
        HAL_CIPHER_Hash_SetHOS(FALSE);
    }
    else
    {
        HAL_CIPHER_Hash_SetHOS(TRUE);
    }

    //Set total size if last command
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        HAL_CIPHER_Hash_SetMsgLength(stCfg.u32StartByte + stCfg.stInput.u32Size);
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Message length, StartBytes[0x%08x], Input Size[%lu]\n",
        __func__, __LINE__, (unsigned int)stCfg.u32StartByte, stCfg.stInput.u32Size));


    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:CIPHER report buffer[%u] is used!\n", __func__, __LINE__, u8BufNum));
       ret =  DRV_CIPHER_NO_REPORT_BUFFER;
       goto HASHMANUAL_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Digest buffer address[0x%08x], buffer size[%lu]\n",
                __func__, __LINE__, (unsigned int)stCfg.pu8Digest_Buf, stCfg.u32Digest_Buf_Size));


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_HASH_DONE;
#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
            __func__, __LINE__, eIntMode, _u16ThreadID));

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;

HASHMANUAL_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HMAC(DRV_CIPHER_HMACCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen   = 0;
    MS_U32 u32HashSrc  = 0;
    MS_U32 u32HashDst  = 0;
    MS_U8  u8BufNum    = 0;

#if defined (MSOS_TYPE_NOS) || defined (MCU_AEON)
    MS_U32 Rpt[10]     = {0};
    MS_U32 u32WaitCnt  = 0;
    MS_U32 u32ErrorMsg = 0;
#endif

	if(_bCIPHERDrv_Inited == FALSE)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init First!\n", __func__, __LINE__));
        return DRV_CIPHER_FAIL;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH data size is zero!\n", __func__, __LINE__));
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH CmdId is NULL!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Wrong buffer size[%lu]!\n", __func__, __LINE__, stCfg.u32Digest_Buf_Size));
        goto HASHHMAC_FINISH;
    }
    //------------Inner hash----------------------------
    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);
    //Set CAVid
    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error: Set CAVid [0x%x]fail!\n",
            __func__, __LINE__, (unsigned int)stCfg.u32CAVid));
        goto HASHHMAC_FINISH;
    }
    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Algorithm [%d]\n",
                __func__, __LINE__, stCfg.eAlgo));

    //Set IV to FIPS
    HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS);
    HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0);

    //Set HMAC Key, must MDrv_Cipher_Init first to set CAVid, I_KeyPad
    HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_IKPAD, FALSE);

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set source memory type [%d] fail\n",
            __func__, __LINE__, stCfg.stInput.eMemoryType));
        goto HASHHMAC_FINISH;
    }
    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Input address[0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));
        goto HASHHMAC_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input address[0x%08x], size[%lu]\n",
        __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size));

    //Set output destination, HMAC inner hash output to register
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }
#if 0
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }
#endif
	//Set HOS = 0, HW limitation
	HAL_CIPHER_Hash_SetHOS(FALSE);

    HAL_CIPHER_Hash_SetMsgLength(stCfg.stInput.u32Size + HASH_HMAC_KEYPAD_SIZE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_HASH_DONE;
    eIntMode     = E_DMA_INT_ENABLE;

#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
            __func__, __LINE__, eIntMode, _u16ThreadID));

#if defined (MSOS_TYPE_NOS) || defined (MCU_AEON)

    while(FALSE == HAL_CIPHER_Hash_CmdDone(_u16ThreadID, &u32ErrorMsg) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
        MsOS_DelayTask(20);
        u32WaitCnt++;
    }
    //First time should print HMAC error flag
    if(u32ErrorMsg != 0)
    {
        HAL_CIPHER_ReadException(u32ErrorMsg);
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning;HASH timeout!\n", __func__, __LINE__));
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    HAL_CIPHER_Hash_GetRpt(Rpt, HASH_RPT_SIZE_STATUS);

    //Check report id > CmdId, report is overwrite
    if((Rpt[0] & REG_CIPHER_RPT_THREAD_MSK) > _u16ThreadID)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:Hash report is overwrite!\n", __func__, __LINE__));
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    if((Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        ret = DRV_CIPHER_FAIL;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HMAC Inner Hash Fail!\n", __func__, __LINE__));
        goto HASHHMAC_FINISH;
    }

    MsOS_ReleaseSemaphore(_CipherSemaphore);
#endif

    _u16ThreadID++;

    MsOS_ReleaseMutex(_CipherMtx);

    //----------------Outer hash------------------------------

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    eIntMode = E_DMA_INT_NONE;

    //Set CAVid
    HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid);
    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );

    //Set IV to FIPS
    HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS);
    HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0);

    //Set HMAC Key,
    HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_OKPAD, FALSE);

    //No need to set Hash message source
    //HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc);

    //Set input message address, outer hash from previous report
    if(FALSE == HAL_CIPHER_Hash_SetMsg(NULL, 0,  CMD_HASH_SRC_PRV_RPT))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //Set output destination
    HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst);

#if 0 //Default output to Register
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }
#endif

	//Set HOS = 1
	HAL_CIPHER_Hash_SetHOS(TRUE);

    HAL_CIPHER_Hash_SetMsgLength(u32OutLen + HASH_HMAC_KEYPAD_SIZE); //Hash output size + O_keypad_size

    u8BufNum = _u16ThreadID % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Digest buffer address[0x%08x], buffer size[%lu]\n",
                __func__, __LINE__, (unsigned int)stCfg.pu8Digest_Buf, stCfg.u32Digest_Buf_Size));


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_HASH_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

     DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
            __func__, __LINE__, eIntMode, _u16ThreadID));

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;

HASHHMAC_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

#if 0
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAParser(CIPHER_PARSERCFG stParserCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8  u8BufNum  = 0;

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check thread id max
    if(_u16ThreadID == CIPHER_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }

    //Check data size
    if((stParserCfg.stInput.u32Size == 0) || (stParserCfg.stOutput.u32Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:DMA data size is zero!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto PARSER_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:DMA CmdId is NULL!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_BAD_PARAM;
        goto PARSER_FINISH;
    }

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n", __FUNCTION__, __LINE__, (unsigned int)stParserCfg.u32CAVid));

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stParserCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error: Set CAVid [0x%x]fail!\n",
            __func__, __LINE__, (unsigned int)stParserCfg.u32CAVid));
        goto PARSER_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo( stParserCfg.stAlgo);
    HAL_CIPHER_DMA_Set_Key ( stParserCfg.stKey);
    HAL_CIPHER_DMA_Set_IV(stParserCfg.stKey.pu8IVData, stParserCfg.stKey.u8IVLen);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]\n",
            __func__, __LINE__, stParserCfg.stAlgo.eMainAlgo, stParserCfg.stAlgo.eSubAlgo, stParserCfg.stAlgo.eResAlgo, stParserCfg.stAlgo.eSBAlgo));

	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stParserCfg.stInput.eMemoryType, NULL, 0);
    HAL_CIPHER_DMA_Set_OutputDstTo(stParserCfg.stOutput.eMemoryType, NULL, 0);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input source from [%d], Output source from [%d]\n",
            __func__, __LINE__, stParserCfg.stInput.eMemoryType, stParserCfg.stOutput.eMemoryType));

    // Set File-in Addr //
    HAL_CIPHER_DMA_Set_FileinDesc ( stParserCfg.stInput.u32Addr, stParserCfg.stInput.u32Size);
    HAL_CIPHER_DMA_Set_FileoutDesc( stParserCfg.stOutput.u32Addr, stParserCfg.stOutput.u32Addr + stParserCfg.stOutput.u32Size - 1);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Input address [0x%08x], size[%lu], Output address [0x%08x], size[%lu]\n",
            __func__, __LINE__, (unsigned int)stParserCfg.stInput.u32Addr, stParserCfg.stInput.u32Size, (unsigned int)stParserCfg.stOutput.u32Addr, stParserCfg.stOutput.u32Size));

    // Set Hardware Parser
    HAL_CIPHER_PARSER_Set_Mode(stParserCfg.eParserMode);

    HAL_CIPHER_PARSER_Set_PID(stParserCfg.stPID);
    HAL_CIPHER_PARSER_Set_SCB(stParserCfg.stTSCfg.eSCB);

    HAL_CIPHER_PARSER_Set_ForceSCB(stParserCfg.stTSCfg.bInScrb, stParserCfg.stTSCfg.eFSCB);
    HAL_CIPHER_PARSER_Rmv_SCB(stParserCfg.stTSCfg.bRmvScrb);

    HAL_CIPHER_PARSER_Set_MaskSCB(stParserCfg.stTSCfg.bTsScrbMask);

    HAL_CIPHER_PARSER_Set_Key2(stParserCfg.stKey2.pu8Key2, stParserCfg.stKey2.u8Key2Len);
    HAL_CIPHER_PARSER_Set_IV2(stParserCfg.stKey2.pu8IV2, stParserCfg.stKey2.u8IV2Len);

    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning:CIPHER report buffer is used!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto PARSER_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON)

    _eCipherWait = E_CIPHER_DMA_DONE;

    if(stParserCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stParserCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    if(FALSE == HAL_CIPHER_DMA_Start(stParserCfg.bDecrypt, eIntMode , TRUE , _u16ThreadID))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:PARSER Fail!\n", __func__, __LINE__));
        ret =  DRV_CIPHER_FAIL;
        goto PARSER_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IntMode[%d], CmdId[%hu]\n",
        __func__, __LINE__, eIntMode, _u16ThreadID));

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;

PARSER_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;

}
#endif

#ifdef ENABLE_BGC
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Enable(MS_U32 u32EngId,MS_U32 u32Algo, MS_U8 *pu8Golden, MS_U32 u32SAddr, MS_U32 u32Size)
{
     //Set BGC Algo
     HAL_CIPHER_BGC_Algo(u32EngId, u32Algo);

     //Set BGC Period
     HAL_CIPHER_BGC_Period(u32EngId, E_CIPHER_BGC_PERIOD_1MS);

     //Set BGC Range
     HAL_CIPHER_BGC_Range(u32EngId, u32SAddr, (u32SAddr + u32Size-1) );

     //Set BGC Golden
     HAL_CIPHER_BGC_Golden(u32EngId, pu8Golden);

     //Set BGC Enable
     HAL_CIPHER_BGC_Start(u32EngId, TRUE);


    return 0;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Check(MS_U32 u32EngId, MS_BOOL *bOK)
{
    if(HAL_CIPHER_BGC_Check(u32EngId) == FALSE)
    {
        *bOK = FALSE;
    }
    else
    {
        *bOK = TRUE;
    }
    return 0;
}

#endif
//////////////////////////////////////////////////////////////////
static DRV_CIPHER_KEY _CipherKey = {
        E_CIPHER_KSRC_CPU,
            0,
            0,
            0,
            0,
            0
};
static MS_BOOL _isDecrypt = TRUE;
static DRV_CIPHER_ALGO _stAlgo = {0};
static DRV_CIPHER_DATA _stInput  = {0};
static DRV_CIPHER_DATA _stOutput = {0};
static MS_U32 _RetId = 0;
static MS_U8 _u8IV[16] = {0};
static MS_U8 _u8Key[16] = {0};
static MS_U8 _u8ZeroData[16] = {0};

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_U32 u32miu0addr , MS_U32 u32miu1addr , MS_U32 u32miunum)
{
    //Use default CAVID on old interface
    MDrv_CIPHER_Init();

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Init(MS_U32 u32miu0addr , MS_U32 u32miu1addr , MS_U32 u32miunum)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_INIT pArgs;

    if(pu32CipherInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pu32CipherInst, 0x0, NULL))
        {
            printf("UtopiaOpen CIPHER fail\n");
            return DRVAESDMA_FAIL;
        }
    }

    pArgs.u32miu0addr = u32miu0addr;
    pArgs.u32miu1addr = u32miu1addr;
    pArgs.u32miunum = u32miunum;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_Init, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Init(u32miu0addr, u32miu1addr, u32miunum);
#endif
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}


DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
    //DRV_CIPHER_ALGO stAlgo;
    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Cipher mode[%d], Decrypt[%d]!\n", __func__, __LINE__, eMode, bDescrypt));

    bDES_Key = FALSE;
    switch(eMode)
    {
        case E_DRVAESDMA_CIPHER_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_DES_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_DES_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_DES_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_TDES_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_DES_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;

        case E_DRVAESDMA_CIPHER_DES_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        default:
            _stAlgo.eMainAlgo = 0;
            _stAlgo.eSubAlgo = 0;
            _stAlgo.eResAlgo = 0;
            _stAlgo.eSBAlgo = 0;
            break;
    }

    _isDecrypt = bDescrypt;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Algo 0x%x 0x%x 0x%x 0x%x\n",
        __func__, __LINE__, _stAlgo.eMainAlgo, _stAlgo.eSubAlgo,_stAlgo.eResAlgo,_stAlgo.eSBAlgo));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SEL_ENG pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.bDescrypt = bDescrypt;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SelEng(eMode, bDescrypt);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
    if(pu8Key && (u32Len <= 16) && (u32Len > 0))
    {
        // Key from ACPU
        _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;

        memcpy(_u8Key, (MS_U8*)pu8Key, u32Len);
        _CipherKey.pu8KeyData = (MS_U8 *)_u8Key;

        _CipherKey.u8KeyLen = u32Len;
        _CipherKey.u8KeyIdx = 0;
    }
    else
    {
        //Key from KL
        _CipherKey.eKeySrc = E_CIPHER_KSRC_KL;
        _CipherKey.pu8KeyData = 0;
        _CipherKey.u8KeyLen = 0;
        _CipherKey.u8KeyIdx = 0;  // always use 0
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Key Source[%d], Key Length[%u], Key Index[%u]\n",
        __func__, __LINE__, _CipherKey.eKeySrc, _CipherKey.u8KeyLen, _CipherKey.u8KeyIdx));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_KEY_EX pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.pu8Key = pu8Key;
    pArgs.u32Len = u32Len;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetKey_Ex, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey_Ex(pu8Key, u32Len);
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
    if(pu8IV && (u32Len <= 16) && (u32Len > 0))
    {
        memcpy(_u8IV, pu8IV, 16);
        _CipherKey.pu8IVData = (MS_U8 *)_u8IV;
        _CipherKey.u8IVLen = u32Len;
    }
    else
    {
        _CipherKey.pu8IVData = 0;
        _CipherKey.u8IVLen = 0;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IV Length[%u]\n", __func__, __LINE__, _CipherKey.u8IVLen));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_IV_EX pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.pu8IV = pu8IV;
    pArgs.u32Len = u32Len;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetIV_Ex, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetIV_Ex(pu8IV, u32Len);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
    if(pCipherKey)
    {
        MS_U32 u32Tmp[4] = {0};
        if(!bDES_Key)
        {
		    u32Tmp[3] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey);
            u32Tmp[2] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 4);
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 8);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 12);
        }
        else
        {
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 4);

        }
		// Key from ACPU
        _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;

        memcpy(_u8Key,(MS_U8 *)u32Tmp, 16);
        _CipherKey.pu8KeyData = (MS_U8 *)_u8Key;
        _CipherKey.u8KeyLen = 16;
        _CipherKey.u8KeyIdx = 0;
    }
    else
    {
        //Key from KL
        memset(_u8Key, 0, 16);
        _CipherKey.eKeySrc = E_CIPHER_KSRC_KL;
        _CipherKey.pu8KeyData = 0;
        _CipherKey.u8KeyLen = 16;
        _CipherKey.u8KeyIdx = 0;
    }

	DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Key Source[%d], Key Length[%u], Key Index[%u], DES Key[%d]\n",
        __func__, __LINE__, _CipherKey.eKeySrc, _CipherKey.u8KeyLen, _CipherKey.u8KeyIdx, bDES_Key));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_KEY pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.pCipherKey = pCipherKey;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetKey, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey(pCipherKey);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetSecureKey(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetSecureKey, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetSecureKey();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
    if(pInitVector)
    {
		MS_U32 u32Tmp[4] = {0};
        if(!bDES_Key)
        {
		    u32Tmp[3] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector);
            u32Tmp[2] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 4);
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 8);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 12);
        }
        else
        {
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 4);
        }
		memcpy(_u8IV,(MS_U8 *)u32Tmp, 16);
        _CipherKey.pu8IVData = (MS_U8 *)_u8IV;
        _CipherKey.u8IVLen = 16;
    }
    else
    {
        memset(_u8IV, 0, 16);
        _CipherKey.pu8IVData = 0;
        _CipherKey.u8IVLen = 0;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]IV Length[%u], DES IV[%d]\n", __func__, __LINE__, _CipherKey.u8IVLen, bDES_Key));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_IV pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.pInitVector = pInitVector;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetIV, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetIV(pInitVector);
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_PS pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.u32PTN = u32PTN;
    pArgs.u32Mask = u32Mask;
    pArgs.bPSin_Enable = bPSin_Enable;
    pArgs.bPSout_Enable = bPSout_Enable;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetPS, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetPS(u32PTN, u32Mask, bPSin_Enable, bPSout_Enable);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_PSRelease(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_PSRelease, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_PSRelease();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_U32 u32FileinAddr, MS_U32 u32FileInNum, MS_U32 u32FileOutSAddr, MS_U32 u32FileOutEAddr)
{

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]FileIn address[0x%x], FileIn num[0x%x], FileOut start address[0x%x], FileOut end address[0x%x]\n",
        __func__, __LINE__,
        (unsigned int)u32FileinAddr,
        (unsigned int)u32FileInNum,
        (unsigned int)u32FileOutSAddr,
        (unsigned int)u32FileOutEAddr));

    _stInput.u32Addr = u32FileinAddr;
    _stInput.u32Size = u32FileInNum;
    _stInput.eMemoryType = E_CIPHER_DRAM;

    _stOutput.u32Addr = u32FileOutSAddr;
    _stOutput.u32Size = (u32FileOutEAddr - u32FileOutSAddr) + 1;
    _stOutput.eMemoryType = E_CIPHER_DRAM;

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetFileInOut(MS_U32 u32FileinAddr, MS_U32 u32FileInNum, MS_U32 u32FileOutSAddr, MS_U32 u32FileOutEAddr)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_FILE_INOUT pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.u32FileinAddr = u32FileinAddr;
    pArgs.u32FileInNum  = u32FileInNum;
    pArgs.u32FileOutSAddr = u32FileOutSAddr;
    pArgs.u32FileOutEAddr = u32FileOutEAddr;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetFileInOut(u32FileinAddr, u32FileInNum, u32FileOutSAddr, u32FileOutEAddr);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart)
{
    DRV_CIPHER_DMACFG stCfg = {{0}};

    MsOS_FlushMemory();

    stCfg.stAlgo   = _stAlgo;
    stCfg.u32CAVid = 0;
    stCfg.stKey    = _CipherKey;
    stCfg.stInput  = _stInput;
    stCfg.stOutput = _stOutput;

    stCfg.bDecrypt = _isDecrypt;

    MDrv_CIPHER_DMACalc(stCfg, &_RetId);

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Decrypt[%d], CmdId[%lu]!\n",
        __func__, __LINE__, _isDecrypt, _RetId));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Start(MS_BOOL bStart)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_START pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.bStart = bStart;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_Start, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Start(bStart);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void)
{
    MDrv_CIPHER_Reset();

    // Default Key from ACPU
    _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;
    _CipherKey.pu8KeyData = _u8ZeroData;
    _CipherKey.u8KeyLen = 16;
    _CipherKey.u8KeyIdx = 0;

    //Clear IV
    _CipherKey.pu8IVData = _u8ZeroData;
    _CipherKey.u8IVLen = 16;

    //Clear Algo
    _stAlgo.eMainAlgo = 0;
    _stAlgo.eSubAlgo  = 0;
    _stAlgo.eResAlgo  = 0;
    _stAlgo.eSBAlgo   = 0;

    //Clear Input
    _stInput.u32Addr = 0;
    _stInput.u32Size = 0;
    _stInput.eMemoryType = 0;

    //Clear Output
    _stOutput.u32Addr = 0;
    _stOutput.u32Size = 0;
    _stOutput.eMemoryType = 0;

    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]\n", __func__, __LINE__));

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Reset(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Reset();
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(void)
{
    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_GetStatus(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_GetStatus, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_GetStatus();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(void)
{
    MS_BOOL bDone = FALSE;
    MS_U32 u32Ret = 0;

    bDone = MDrv_CIPHER_IsDMADone(_RetId, &u32Ret);

    if(TRUE == bDone)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]CmdId[%lu] Not Done!\n",
            __func__, __LINE__, _RetId));
        return DRVAESDMA_FAIL;
    }
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_IsFinished(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_IsFinished, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_IsFinished();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC MS_U32 MDrv_AESDMA_GetPSMatchedByteCNT(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_GetPSMatchedByteCNT, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_GetPSMatchedByteCNT();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC MS_U32 MDrv_AESDMA_GetPSMatchedPTN(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_GetPSMatchedPTN, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_GetPSMatchedPTN();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_NOTIFY pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.eEvents = eEvents;
    pArgs.pfCallback = pfCallback;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_Notify, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Notify(eEvents, pfCallback);
#endif
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel)
{
    MDrv_CIPHER_SetDbgLevel(DbgLevel);

    return DRVAESDMA_OK;
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{

    if(u32Size == 0 && u32PABuf == 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Buffer or Size is zero!\n", __func__, __LINE__));
        return 0xFFFFFFFF;
    }

    MS_U8 *pu8VABuf = (MS_U8 *)MsOS_PA2KSEG1((MS_U32)u32PABuf);

    if( FALSE == HAL_CIPHER_Misc_Random(pu8VABuf,u32Size))
    {
        return 0xFFFFFFFF;
    }

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_RAND pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.u32PABuf = u32PABuf;
    pArgs.u32Size = u32Size;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_Rand, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Rand(u32PABuf, u32Size);
#endif
}


DRVAESDMA_RESULT _MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not Support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_AESDMA_SetClk, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Set_Clk(benable);
#endif
}

DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_U32 u32PAInBuf, MS_U32 u32Size, MS_U32 u32PAOutBuf)
{
    DRV_CIPHER_HASHCFG stCfg = {0};
    CIPHER_HASH_ALGO eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    MS_U32 u32DgstSize = 20;
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]eMode[%d], InBuffer[0x%08x], Size[%lu], OutBuffer[0x%08x]!\n",
        __func__, __LINE__, eMode, (unsigned int)u32PAInBuf, u32Size, (unsigned int)u32PAOutBuf));

    switch(eMode)
    {
    case E_DRVAESDMA_SHA1:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
        break;
    case E_DRVAESDMA_SHA256:
        eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        u32DgstSize = 32;
        break;
    default:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
    }

    stCfg.eAlgo              = eAlgo;
    stCfg.stInput.u32Addr    = u32PAInBuf;
    stCfg.stInput.u32Size    = u32Size;
    stCfg.pu8Digest_Buf      = (MS_U8 *)(MsOS_PA2KSEG1(u32PAOutBuf));
    stCfg.u32Digest_Buf_Size = u32DgstSize;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASH(stCfg, &_RetId))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH Fail!\n", __func__, __LINE__));
        return DRVAESDMA_FAIL;
    }

    while((FALSE == MDrv_CIPHER_IsHASHDone(_RetId, &u32Ret)) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
        MsOS_DelayTask(20);
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning;HASH timeout!\n", __func__, __LINE__));
       return DRVAESDMA_FAIL;
    }

    //Reverse MSB to LSB for old API compatible
    _ReverseU8Buf(stCfg.pu8Digest_Buf, u32DgstSize );

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_U32 u32PAInBuf, MS_U32 u32Size, MS_U32 u32PAOutBuf)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.u32PAInBuf = u32PAInBuf;
    pArgs.u32Size = u32Size;
    pArgs.u32PAOutBuf = u32PAOutBuf;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_SHA_Calculate, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_SHA_Calculate(eMode, u32PAInBuf, u32Size, u32PAOutBuf);
#endif
}

DRVAESDMA_RESULT _MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{
    DRV_CIPHER_HASHCFG_MANUAL stCipherCfg = {0};
    CIPHER_HASH_ALGO eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    MS_U32 u32DgstSize = 20;
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]eMode[%d], Stage[%d], Done Bytes[%lu]!\n",
        __func__, __LINE__, stCfg.eMode, eStage, u32DoneBytes));
    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]InAddress[0x%08x], InSize[%lu], OutAddress[0x%08x], OutSize[%lu]!\n",
        __func__, __LINE__, (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size, (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size));

    switch(stCfg.eMode)
    {
    case E_DRVAESDMA_SHA1:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
        break;
    case E_DRVAESDMA_SHA256:
        eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        u32DgstSize = 32;
        break;
    default:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
    }

    //Algo
    stCipherCfg.eAlgo = eAlgo;

    //Input
    stCipherCfg.stInput.u32Addr = MsOS_VA2PA(stCfg.stInput.u32Addr);
    stCipherCfg.stInput.u32Size = stCfg.stInput.u32Size;

    //Output
    stCipherCfg.pu8Digest_Buf      = (MS_U8 *)stCfg.stOutput.u32Addr;
    stCipherCfg.u32Digest_Buf_Size = stCfg.stOutput.u32Size;

    //Stage
    stCipherCfg.eStage = eStage;

    //StartBytes
    stCipherCfg.u32StartByte = u32DoneBytes;

    //IV
    stCipherCfg.pu8SetIV = pu8SetIV;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASHManual(stCipherCfg, &_RetId))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:HASH Fail!\n", __func__, __LINE__));
        return DRVAESDMA_FAIL;
    }

    while((FALSE == MDrv_CIPHER_IsHASHDone(_RetId, &u32Ret)) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
        MsOS_DelayTask(20);
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Warning;HASH timeout!\n", __func__, __LINE__));
       return DRVAESDMA_FAIL;
    }

    //Reverse MSB to LSB for old API compatible
    if(eStage == E_DRVAESDMA_HASH_STAGE_LAST)
    {
        _ReverseU8Buf(stCipherCfg.pu8Digest_Buf, u32DgstSize );
    }

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE_MANUAL pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.stCfg = stCfg;
    pArgs.eStage = eStage;
    pArgs.u32DoneBytes = u32DoneBytes;
    pArgs.pu8SetIV = pu8SetIV;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_SHA_CalculateManual, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_SHA_CalculateManual(stCfg, eStage, u32DoneBytes, pu8SetIV);
#endif
}


#ifdef MOBF_ENABLE
DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_DmaOnly(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_OneWay(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}
#endif

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC MS_U8 MDrv_AESDMA_Parser_QueryPidCount(void)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support!\n", __func__, __LINE__));
    return 0;
}

DLL_PUBLIC MS_BOOL MDrv_AESDMA_Lock(void)
{
    MsOS_ObtainMutex(_AESDMAMtx, MSOS_WAIT_FOREVER);
    return DRVAESDMA_OK;
}

DLL_PUBLIC MS_BOOL MDrv_AESDMA_Unlock(void)
{
    MsOS_ReleaseMutex(_AESDMAMtx);
    return DRVAESDMA_OK;
}

#ifndef MSOS_TYPE_NUTTX
DRVAESDMA_RESULT MDrv_AESDMA_SetSecurityInfo(DrvAESDMA_SecureInfo *pSecureInfo)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not realy unlock here!\n", __func__, __LINE__));
    return 0;
}
#endif

MS_BOOL _MDrv_RSA_Init(void)
{
    MS_U32 u32Bank, u32BankSize;

    if(_bRSA_Init == FALSE)
    {
        if(FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Get MMIO base fail\n", __FUNCTION__, __LINE__));
            return FALSE;
        }

        HAL_RSA_SetBank(u32Bank);

        _CipherRSAMtx = MsOS_CreateMutex( E_MSOS_FIFO, (char*)"CIPHER RSA mutex", MSOS_PROCESS_SHARED);
        _bRSA_Init = TRUE;
    }
    return TRUE;
}

DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 u32KeyLen = 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;

    if( pstSign==NULL || pstKey==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:RSA Init fail\n", __FUNCTION__, __LINE__));
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);

    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]RSA Mode[%d]\n", __func__, __LINE__, eMode));

    HAL_RSA_LoadSram((pstKey->u32KeyE), E_RSA_ADDRESS_E);
    HAL_RSA_LoadSram((pstKey->u32KeyN), E_RSA_ADDRESS_N);
    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);

    HAL_RSA_SetKeyLength((u32KeyLen/RSA_UNIT_SIZE)-1);
    HAL_RSA_SetKeyType(E_RSA_USE_SW_KEY, u8Public); //sw key, public key
    HAL_RSA_ExponetialStart();

RSA_CAL_FINISH:
    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    RSA_CALCULATE pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.pstSign = pstSign;
    pArgs.pstKey = pstKey;
    pArgs.eMode = eMode;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_RSA_Calculate, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Calculate(pstSign, pstKey, eMode);
#endif
}
DRVAESDMA_RESULT MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U8  u8KeySrc= 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;

    if( pstSign==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_HW_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:RSA Init fail\n", __FUNCTION__, __LINE__));
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_HW_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);

    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA_HWKEY_PRIVATE:
        {
            u8KeySrc = E_RSA_HW_PRI_KEY;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC1:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY1;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC2:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY2;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_HW_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]RSA Mode[%d], Key Source[%u], Public[%u]\n",
        __func__, __LINE__, eMode, u8KeySrc, u8Public));

    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);

    HAL_RSA_SetKeyType(E_RSA_USE_HW_KEY, u8Public);
    HAL_RSA_Rootkey_Sel(u8KeySrc);
    HAL_RSA_ExponetialStart();

RSA_CAL_HW_FINISH:
    return ret;
}

DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void)
{
    RSAInitChk();

    if(HAL_RSA_Get_RSA_IsFinished()==0)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]RSA Not Done!\n", __func__, __LINE__));
        return DRVAESDMA_FAIL;
    }
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_IsFinished(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_RSA_IsFinished, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_IsFinished();
#endif
}

DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 i = 0;
    MS_U32 u32KeyLen = 0;
    MS_U32 *pu32RSAOut = (MS_U32 *)(pstRSAOut->u32RSAOut);

    RSAInitChk();

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_OUTPUT_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]RSA Mode[%d], Key length[%lu]\n",
        __func__, __LINE__, eMode, u32KeyLen));

    HAL_RSA_SetFileOutAddr(64-(u32KeyLen/sizeof(MS_U32)));
    HAL_RSA_Ind32Ctrl(RAM_READ);
    HAL_RSA_FileOutStart();
	for( i = 0; i<(u32KeyLen/RSA_UNIT_SIZE); i++)
	{
		pu32RSAOut[(u32KeyLen/RSA_UNIT_SIZE)-1-i] = HAL_RSA_FileOut();
	}

RSA_OUTPUT_FINISH:
    MsOS_ReleaseMutex(_CipherRSAMtx);
    return ret;
}
DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    RSA_OUTPUT pArgs;
    if(pu32CipherInst == NULL)
    {
        printf("[%s]ERROR : Cipher instance NULL\n", __FUNCTION__);
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.pstRSAOut = pstRSAOut;

    u32Ret = UtopiaIoctl(pu32CipherInst, MDrv_CMD_RSA_Output, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        printf("[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Output(eMode, pstRSAOut);
#endif
}


