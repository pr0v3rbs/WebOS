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

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif


#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "halAESDMA.h"
#include "drvAESDMA.h"
#include "utopia.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
//#define AESDMAIntEnable()                       MsOS_EnableInterrupt(E_INT_IRQ_WADR_ERR);
//#define AESDMAIntDisable()                      MsOS_DisableInterrupt(E_INT_IRQ_WADR_ERR);
//#define AESDMAIntAttach()                       MsOS_AttachInterrupt(E_INT_IRQ_WADR_ERR, (InterruptCb )_AESDMA_Isr)
//#define AESDMAIntDetach()                       MsOS_DetachInterrupt (E_INT_IRQ_WADR_ERR)

#define AESDMAPA2KSEG1(addr)                    ((void *)(((MS_U32)addr) | 0xa0000000)) //physical -> unchched

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
//#define AESDMA_TASK_STACK_SIZE 4096

//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
//#ifndef MSOS_TYPE_LINUX
static DrvAESDMA_Event                  _AESDMAEvtNotify;
static P_DrvAESDMA_EvtCallback          _AESDMACallback;
//#endif // #ifndef MSOS_TYPE_LINUX

//static void*                            _pAESDMATaskStack;
//static MS_U8                            _u8AESDMA_StackBuffer[AESDMA_TASK_STACK_SIZE];
//static MS_S32                           _s32AESDMATaskId;
//static MS_U32                           _u32LastErr;

static MS_U32                           _u32MIU0_Addr, _u32MIU1_Addr , _u32MIU2_Addr, _u32MIU_num;
static DrvAESDMA_DbgLevel               _u32AESDMADBGLevel;

//#define _DEBUG2K(fmt, args...) { printf("[Utopia2K] %s (%d)(%s)", __FUNCTION__, __LINE__, __TIME__); printf (fmt, ##args);}
#define _DEBUG2K(fmt, args...)

static MSIF_Version _drv_aesdma_version = {
    .DDI = { AESDMA_DRV_VERSION, },
};

#if (AESDMA_UTOPIA20)
void* pInstantAesdma = NULL;
void* pAttributeAesdma = NULL;
#endif

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
//static MS_S32                           _s32AESDMAMutexId;
//static MS_S32                           _s32AESDMAEventId;
static MS_U8                            _u8IsInit = FALSE;
#ifndef MSOS_TYPE_NUTTX
static MS_BOOL                          bSecureModeEnable = FALSE;
#endif

//#define _AESDMA_SetEvent(flag)          MsOS_SetEvent(_s32AESDMAEventId, (flag))
//#define _AESDMA_GetEvent(events)        MsOS_WaitEvent(_s32AESDMAEventId, AESDMA_STATES_GROUP, &events, E_OR_CLEAR, MSOS_WAIT_FOREVER)


//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define DEBUG_AES_DMA(debug_level, x)     do { if (_u32AESDMADBGLevel >= (debug_level)) (x); } while(0)

//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
static MS_BOOL _AESDMA_Chk_MIU(MS_U32 u32InAddr, MS_U32 u32InLen, MS_U32 u32OutSAddr, MS_U32 u32OutEAddr)
{

    if (_u32MIU_num == 3)
    {
        if ((_u32MIU1_Addr > u32InAddr) & (_u32MIU1_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr > u32OutSAddr) & (_u32MIU1_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu0->miu0
            return TRUE;
        }

        if ((_u32MIU1_Addr > u32InAddr) & (_u32MIU1_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr < u32OutSAddr) & (_u32MIU1_Addr < u32OutEAddr) & (_u32MIU2_Addr > u32OutSAddr) & (_u32MIU2_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,TRUE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu0->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr > u32InAddr) & (_u32MIU1_Addr > u32InAddr+u32InLen) & (_u32MIU2_Addr <= u32OutSAddr) & (_u32MIU2_Addr <= u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,TRUE); // miu0->miu2
            return TRUE;
        }

        if ((_u32MIU1_Addr <= u32InAddr) & (_u32MIU1_Addr <= u32InAddr+u32InLen) & (_u32MIU2_Addr > u32InAddr) & (_u32MIU2_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr > u32OutSAddr) & (_u32MIU1_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(TRUE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu1->miu0
            return TRUE;
        }
        
        if ((_u32MIU1_Addr <= u32InAddr) & (_u32MIU1_Addr <= u32InAddr+u32InLen) & (_u32MIU2_Addr > u32InAddr) & (_u32MIU2_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr <= u32OutSAddr) & (_u32MIU1_Addr <= u32OutEAddr) & (_u32MIU2_Addr > u32OutSAddr) & (_u32MIU2_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(TRUE,TRUE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu1->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr <= u32InAddr) & (_u32MIU1_Addr <= u32InAddr+u32InLen) & (_u32MIU2_Addr > u32InAddr) & (_u32MIU2_Addr > u32InAddr+u32InLen) & (_u32MIU2_Addr <= u32OutSAddr) & (_u32MIU2_Addr <= u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(TRUE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,TRUE); // miu1->miu2
            return TRUE;
        }

        if ((_u32MIU2_Addr <= u32InAddr) & (_u32MIU2_Addr <= u32InAddr+u32InLen) & (_u32MIU1_Addr > u32OutSAddr) & (_u32MIU1_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(TRUE,FALSE); // miu2->miu0
            return TRUE;
        }

        if ((_u32MIU2_Addr <= u32InAddr) & (_u32MIU2_Addr <= u32InAddr+u32InLen) & (_u32MIU1_Addr <= u32OutSAddr) & (_u32MIU1_Addr <= u32OutEAddr) & (_u32MIU2_Addr > u32OutSAddr) & (_u32MIU2_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,TRUE);
            AESDMA_Set_MIU2_Path(TRUE,FALSE); // miu2->miu1
            return TRUE;
        }

        if ((_u32MIU2_Addr <= u32InAddr) & (_u32MIU2_Addr <= u32InAddr+u32InLen) & (_u32MIU2_Addr <= u32OutSAddr) & (_u32MIU2_Addr <= u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(TRUE,TRUE); // miu2->miu2
            return TRUE;
        }
        return FALSE;
    }
    else if (_u32MIU_num == 2)
    {
        if ((_u32MIU1_Addr > u32InAddr) & (_u32MIU1_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr > u32OutSAddr) & (_u32MIU1_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE); // miu0->miu0
            return TRUE;
        }

        if ((_u32MIU1_Addr < u32InAddr) & (_u32MIU1_Addr < u32InAddr+u32InLen) & (_u32MIU1_Addr < u32OutSAddr) & (_u32MIU1_Addr < u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(TRUE,TRUE); // miu1->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr > u32InAddr) & (_u32MIU1_Addr > u32InAddr+u32InLen) & (_u32MIU1_Addr <= u32OutSAddr) & (_u32MIU1_Addr <= u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,TRUE); // miu0->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr <= u32InAddr) & (_u32MIU1_Addr <= u32InAddr+u32InLen) & (_u32MIU1_Addr > u32OutSAddr) & (_u32MIU1_Addr > u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(TRUE,FALSE); // miu1->miu0
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        if ((_u32MIU0_Addr < u32InAddr) & (_u32MIU0_Addr < u32InAddr+u32InLen) & (_u32MIU0_Addr < u32OutSAddr) & (_u32MIU0_Addr < u32OutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE); // miu0->miu0

        }
        return TRUE;
    }
}

#if 0
static void _AESDMA_Isr(void)
{
    MS_U32                 u32AESDMAStatus;

    // Direct register access
    u32AESDMAStatus = AESDMA_Get_AESDMA_Status();

    if (HAS_FLAG(u32AESDMAStatus, AESDMA_STATES_GROUP))
    {
        _AESDMA_SetEvent(HAS_FLAG(u32AESDMAStatus, AESDMA_STATES_GROUP)); // Trigger task
    }
    AESDMA_Clear_Int();
    AESDMA_Enable_Int();
    AESDMAIntEnable();
}

static void _AESDMA_Isr_Proc(void)
{
    MS_U32              u32Events;

    while (1)
    {
        _AESDMA_GetEvent(u32Events);
        if (u32Events & AESDMA_STATES_GROUP)
        {
            if (HAS_FLAG(u32Events, AESDMA_STATES_GROUP) && _AESDMAEvtNotify && _AESDMACallback)
            {
                switch (u32Events & AESDMA_STATES_GROUP)
                {
                    case AESDMA_DMA_DONE:
                        if (HAS_FLAG(_AESDMAEvtNotify, E_DRVAESDMA_EVENT_DMA_DONE))
                        {
                            MsOS_ReadMemory();
                            _AESDMACallback(E_DRVAESDMA_EVENT_DMA_DONE);
                        }
                        break;
                    case AESDMA_DMA_PAUSE:
                        if (HAS_FLAG(_AESDMAEvtNotify, E_DRVAESDMA_EVENT_DMA_PAUSE))
                        {
                            _AESDMACallback(E_DRVAESDMA_EVENT_DMA_PAUSE);
                        }
                        break;
                   default:
                        break;
                }
            }
        }
    } // Task while loop
}
#endif

MS_U32 _AESDMA_MSBF2Native(const MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_Init(MS_U32 u32miu0addr , MS_U32 u32miu1addr , MS_U32 u32miunum)
{

#if (AESDMA_UTOPIA20)
    _DEBUG2K("0x%x, 0x%x, 0x%x \n", u32miu0addr, u32miu1addr, u32miunum);
    AESDMA_INIT sAesdmaInit;

    sAesdmaInit.u32miu0addr = u32miu0addr;
    sAesdmaInit.u32miu1addr = u32miu1addr;
    sAesdmaInit.u32miunum = u32miunum;

    if(UtopiaOpen(MODULE_AESDMA|KERNEL_MODE, &pInstantAesdma, 0, pAttributeAesdma) !=  UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Open AESDMA fail\n"));
        return DRVAESDMA_FAIL;
    }

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Init,(void*)&sAesdmaInit) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Init fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Init(u32miu0addr, u32miu1addr, u32miunum);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_U32 u32miu0addr , MS_U32 u32miu1addr , MS_U32 u32miunum)
{
    MS_U32 u32Bank, u32BankSize;  // Non-PM bank
    MS_U32 u32IRBank, u32IrBankSize;  // PM bank

    DEBUG_AES_DMA(E_DRVAESDMA_DBG_L2, printf("%s(0x%08X, 0x%08X, 0x%08X)\n", __FUNCTION__, (int)u32miu0addr, (int)u32miu1addr, (int)u32miunum));

    if(_u8IsInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_BDMA))
        {
            MS_DEBUG_MSG(printf("MDrv_MMIO_GetBASE (NonPM base)fail\n"));
            MS_ASSERT(0);
        }

        if (FALSE == MDrv_MMIO_GetBASE(&u32IRBank, &u32IrBankSize, MS_MODULE_IR))
        {
            MS_DEBUG_MSG(printf("MDrv_MMIO_GetBASE (PM base)fail\n"));
            MS_ASSERT(0);
        }
        _u8IsInit = TRUE;
		
        HAL_AESDMA_SetBank(u32Bank, u32IRBank);
		
        AESDMA_Enable_Clk();
        _u32MIU_num = u32miunum;
        _u32MIU0_Addr = 0;
        if(_u32MIU_num == 2)
        {
            _u32MIU1_Addr = AESDMA_GetMIU1Base();
        }
        
        if(_u32MIU_num == 3)
        {
            _u32MIU1_Addr = AESDMA_GetMIU1Base();
            _u32MIU2_Addr = AESDMA_GetMIU2Base();
        }

        // ++ Release reset that asserted in the DC off
        // *(volatile MS_U32*)(0xbf800000+(0x2380<<2)) |= BIT15;
        // -- Release reset that asserted in the DC off

        AESDMA_Reset();

#if 0
        _AESDMAEvtNotify = E_DRVAESDMA_EVENT_DATA_INIT;
        _AESDMACallback = NULL;

        AESDMA_HW_Patch();

        AESDMAIntAttach();
        AESDMAIntEnable();

        AESDMA_Enable_Int();

        _s32AESDMAEventId = MsOS_CreateEventGroup("AESDMA_Event");
        if (_s32AESDMAEventId < 0)
        {
            MsOS_DeleteMutex(_s32AESDMAMutexId);
            _u32LastErr = DRVAESDMA_FAIL;
            return _u32LastErr;
        }

        _pAESDMATaskStack = _u8AESDMA_StackBuffer;
        _s32AESDMATaskId = MsOS_CreateTask((TaskEntry)_AESDMA_Isr_Proc,
                                        NULL,
                                        E_TASK_PRI_MEDIUM,
                                        TRUE,
                                        _pAESDMATaskStack,
                                        AESDMA_TASK_STACK_SIZE,
                                        "AESDMA_ISR_Task");
        if (_s32AESDMATaskId < 0)
        {
            MsOS_DeleteEventGroup(_s32AESDMAEventId);
            MsOS_DeleteMutex(_s32AESDMAMutexId);
            _u32LastErr = DRVAESDMA_FAIL;
            return _u32LastErr;
        }
#endif
    }
    else
    {
        AESDMA_Enable_Clk();
        AESDMA_Reset();
        _u8IsInit = TRUE;
    }

    return DRVAESDMA_OK;
}

//--------------------------------------------------------------------------------------------------
/// Set key to the AESDMA
/// @param pu8Key \b IN: The key is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8Key. 16 for AES/TDES and 8 for DES
/// @return DRVAESDMA_OK - Success
/// @return DRVAESDMA_INVALID_PARAM - Invalid key length
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
    _DEBUG2K("\n");
    MS_U32 u32Tmp[4] = {0};

    if(pu8Key == NULL)
    {
        AESDMA_Sel_Key(TRUE); // key from KL
        return DRVAESDMA_OK;
    }

    // key from cipherkey
    if(u32Len == 16)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8Key);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8Key + 4);
        u32Tmp[1] = _AESDMA_MSBF2Native(pu8Key + 8);
        u32Tmp[0] = _AESDMA_MSBF2Native(pu8Key + 12);
    }
    else if(u32Len == 8)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8Key);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8Key + 4);
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Set_CipherKey(u32Tmp);
    AESDMA_Sel_Key(FALSE);

    return DRVAESDMA_OK;
}

//--------------------------------------------------------------------------------------------------
/// Set IV to the AESDMA
/// @param pu8IV \b IN: The IV is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8IV. 16 for AES and 8 for TDES/DES
/// @return DRVAESDMA_OK - Success
/// @return DRVAESDMA_INVALID_PARAM - Invalid IV length
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
    _DEBUG2K("\n");
    MS_U32 u32Tmp[4] = {0};

    if(pu8IV == NULL)
    {
        return DRVAESDMA_OK;
    }

    if(u32Len == 16)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8IV);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8IV + 4);
        u32Tmp[1] = _AESDMA_MSBF2Native(pu8IV + 8);
        u32Tmp[0] = _AESDMA_MSBF2Native(pu8IV + 12);
    }
    else if(u32Len == 8)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8IV);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8IV + 4);
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Set_InitVector(u32Tmp);

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
    _DEBUG2K("\n");
    if (pCipherKey != NULL)
    {
        AESDMA_Set_CipherKey(pCipherKey);
        AESDMA_Sel_Key(FALSE); // key from cipherkey
    }
    else
    {
        AESDMA_Sel_Key(TRUE); // key from KL
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetKey,(void*)pCipherKey) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetKey fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey(pCipherKey);
#endif

}

//DRVAESDMA_RESULT MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType)
//{
//    AESDMA_Parser_Enable_Two_Keys(TRUE);
//    switch (stKeyType)
//    {
//    case E_DRVAESDMA_ODD_KEY:
//        AESDMA_Set_CipherOddKey(pCipherKey);
//        break;
//
//    case E_DRVAESDMA_EVEN_KEY:
//        AESDMA_Set_CipherEvenKey(pCipherKey);
//        break;
//
//    default:
//        break;
//    }
//
//    return DRVAESDMA_OK;
//}

//--------------------------------------------------------------------------------------------------
/// Set Odd/Even Key to the AESDMA
/// @param pCipherKey \b IN: The key is represented as vectors of 4-bytes
/// @param stKeyType \b IN: The key type of input key.
/// @return DRVAESDMA_OK - Success
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType)
{
    AESDMA_Parser_Enable_Two_Keys(TRUE);
    switch (stKeyType)
    {
    case E_DRVAESDMA_ODD_KEY:
        AESDMA_Set_CipherOddKey(pCipherKey);
        break;

    case E_DRVAESDMA_EVEN_KEY:
        AESDMA_Set_CipherEvenKey(pCipherKey);
        break;

    default:
        break;
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector)
{
	_DEBUG2K("\n");
    if (pInitVector != NULL)
    {
        AESDMA_Set_OddInitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector)
{
	_DEBUG2K("\n");
#ifndef MSOS_TYPE_NUTTX
    if(bSecureModeEnable)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        if (NULL == pInstantAesdma)
            return DRVAESDMA_FAIL;

        if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_SetOddIV, (void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
        {
            MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetOddIV fail\n"));
            return DRVAESDMA_FAIL;
        }
        return DRVAESDMA_OK;
    }
#else
    return _MDrv_AESDMA_SetOddIV(pInitVector);
#endif
}


DRVAESDMA_RESULT MDrv_AESDMA_SetSecureKey(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetSecureKey,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetSecureKey fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_Sel_SecureKey();
    return DRVAESDMA_OK;
#endif
}


DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void)
{
    AESDMA_Sel_SecureKey();
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
    _DEBUG2K("\n");
    if (pInitVector != NULL)
    {
        AESDMA_Set_InitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetIV,(void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetIV fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey(pInitVector);
#endif

}

DRVAESDMA_RESULT MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode)
{
    _DEBUG2K("\n");
    return AESDMA_QueryCipherMode(eMode);
}

DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
    _DEBUG2K("\n");
    switch (eMode)
    {
        case E_DRVAESDMA_CIPHER_CTR:
        case E_DRVAESDMA_CIPHER_DES_CTR:
        case E_DRVAESDMA_CIPHER_TDES_CTR:
	        AESDMA_Set_CIPHER_ENG((AESDMA_CipherMode)eMode, FALSE);
            break;
        case E_DRVAESDMA_CIPHER_ECB:
        case E_DRVAESDMA_CIPHER_CBC:
        case E_DRVAESDMA_CIPHER_DES_ECB:
        case E_DRVAESDMA_CIPHER_DES_CBC:
        case E_DRVAESDMA_CIPHER_TDES_ECB:
        case E_DRVAESDMA_CIPHER_TDES_CBC:
        case E_DRVAESDMA_CIPHER_CTS_CBC:
        case E_DRVAESDMA_CIPHER_CTS_ECB:
        case E_DRVAESDMA_CIPHER_DES_CTS_CBC:
        case E_DRVAESDMA_CIPHER_DES_CTS_ECB:
        case E_DRVAESDMA_CIPHER_TDES_CTS_CBC:
        case E_DRVAESDMA_CIPHER_TDES_CTS_ECB:
            AESDMA_Set_CIPHER_ENG((AESDMA_CipherMode)eMode, bDescrypt);
            break;
        default:
            break;
    }
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    PAESDMA_SEL_ENG pSelEngParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pSelEngParam = (PAESDMA_SEL_ENG)malloc(sizeof(AESDMA_SEL_ENG));
    if (NULL == pSelEngParam)
        return DRVAESDMA_FAIL;
    
    pSelEngParam->eMode= eMode;
    pSelEngParam->bDescrypt= bDescrypt;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SelEng,(void*)pSelEngParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SelEng fail\n"));
        free(pSelEngParam);
        return DRVAESDMA_FAIL;
    }
    free(pSelEngParam);
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SelEng(eMode,bDescrypt);
#endif

}

DRVAESDMA_RESULT MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
	_DEBUG2K("%d, %d, %d, %d\n",u32PTN, u32Mask, bPSin_Enable, bPSout_Enable);
#if (AESDMA_UTOPIA20)
    PAESDMA_SET_PS pSetPsParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pSetPsParam = (PAESDMA_SET_PS)malloc(sizeof(AESDMA_SET_PS));
    if (NULL == pSetPsParam)
        return DRVAESDMA_FAIL;

    pSetPsParam->u32PTN = u32PTN;
    pSetPsParam->u32Mask = u32Mask;
    pSetPsParam->bPSin_Enable = bPSin_Enable;
    pSetPsParam->bPSout_Enable = bPSout_Enable;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetPS,(void*)pSetPsParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetPS fail\n"));
        free(pSetPsParam);
        return DRVAESDMA_FAIL;
    }
    free(pSetPsParam);
    return DRVAESDMA_OK;
#else
    AESDMA_Set_PS_PTN(u32PTN);
    AESDMA_Set_PS_Mask(u32Mask);
    AESDMA_Set_PS_ENG(bPSin_Enable, bPSout_Enable);
    AESDMA_Enable_Int();

    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
	_DEBUG2K("%d, %d, %d, %d\n",u32PTN, u32Mask, bPSin_Enable, bPSout_Enable);
    AESDMA_Set_PS_PTN(u32PTN);
    AESDMA_Set_PS_Mask(u32Mask);
    AESDMA_Set_PS_ENG(bPSin_Enable, bPSout_Enable);
    AESDMA_Enable_Int();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_PSRelease(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PSRelease,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_PSRelease fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_Set_PS_Release();
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void)
{
	_DEBUG2K("\n");
    AESDMA_Set_PS_Release();
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_U32 u32FileinAddr, MS_U32 u32FileInNum, MS_U32 u32FileOutSAddr, MS_U32 u32FileOutEAddr)
{
    _DEBUG2K("\n");
    // check miu addr
    if (!_AESDMA_Chk_MIU(u32FileinAddr, u32FileInNum, u32FileOutSAddr, u32FileOutEAddr))
    {
        return DRVAESDMA_MIU_ADDR_ERROR;
    }

    if ((u32FileOutSAddr == 0) | (u32FileOutEAddr == 0))
    {
        AESDMA_Set_FileinDesc(u32FileinAddr , u32FileInNum);
    }
    else
    {
        AESDMA_Set_FileinDesc(u32FileinAddr, u32FileInNum);
        AESDMA_Set_FileoutDesc(u32FileOutSAddr, u32FileOutEAddr);
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetFileInOut(MS_U32 u32FileinAddr, MS_U32 u32FileInNum, MS_U32 u32FileOutSAddr, MS_U32 u32FileOutEAddr)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    PAESDMA_FILE_INOUT pFileInOutParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pFileInOutParam = (PAESDMA_FILE_INOUT)malloc(sizeof(AESDMA_FILE_INOUT));
    if (NULL == pFileInOutParam)
        return DRVAESDMA_FAIL;
    
    pFileInOutParam->u32FileinAddr= u32FileinAddr;
    pFileInOutParam->u32FileInNum= u32FileInNum;
    pFileInOutParam->u32FileOutSAddr= u32FileOutSAddr;
    pFileInOutParam->u32FileOutEAddr= u32FileOutEAddr;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetFileInOut,(void*)pFileInOutParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetFileInOut fail\n"));
        free(pFileInOutParam);
        return DRVAESDMA_FAIL;
    }
    free(pFileInOutParam);
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetFileInOut(u32FileinAddr,u32FileInNum,u32FileOutSAddr,u32FileOutEAddr);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart)
{
    _DEBUG2K("\n");
    MsOS_FlushMemory();
    AESDMA_Start(bStart);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Start(MS_BOOL bStart)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Start,(void*)&bStart) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Start fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Start(bStart);
#endif
}

DRVAESDMA_RESULT MDrv_AESDMA_Reset(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Reset,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Reset fail\n"));
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    AESDMA_Reset();
    AESDMA_HW_Patch();
    AESDMA_Enable_Int();
    AESDMA_Parser_Enable_HWParser(FALSE);
    return DRVAESDMA_OK;
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void)
{
    _DEBUG2K("\n");
    AESDMA_Reset();
    AESDMA_HW_Patch();
    AESDMA_Enable_Int();
    AESDMA_Parser_Enable_HWParser(FALSE);
    return DRVAESDMA_OK;
}

MS_U32 MDrv_AESDMA_GetStatus(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    MS_U32 u32RetVal = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_GetStatus,(void*)&u32RetVal) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_GetStatus fail\n"));
        return 0;
    }
    _DEBUG2K("RetVal: 0x%x\n", u32RetVal);
    return u32RetVal;
#else
    return AESDMA_Get_AESDMA_Status();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(MS_U32 *u32RetVal)
{
	*u32RetVal = AESDMA_Get_AESDMA_Status();
	_DEBUG2K("RetVal: 0x%x\n", *u32RetVal);
    return DRVAESDMA_OK;
}


DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(void)
{
    if(AESDMA_Get_AESDMA_IsFinished())
    {
        MsOS_ReadMemory();
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_IsFinished(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    MS_U32 u32TimeOut = 0x1000;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_IsFinished, (void*)&u32TimeOut) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_IsFinished fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_IsFinished();
#endif
}

MS_U32 MDrv_AESDMA_GetPSMatchedByteCNT(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    MS_U32 u32Count = 0x000;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_GetPSMatchedByteCNT, (void*)&u32Count) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_GetPSMatchedByteCNT fail\n"));
        return 0;
    }

    return u32Count;
#else
    return AESDMA_Get_PS_MatchedBytecnt();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(MS_U32 *u32RetVal)
{
	MS_U32 u32Val = AESDMA_Get_PS_MatchedBytecnt();
	*u32RetVal = u32Val;
	_DEBUG2K("RetVal: 0x%x\n", u32Val);
    return u32Val;
}

MS_U32 MDrv_AESDMA_GetPSMatchedPTN(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    return UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_GetPSMatchedPTN,NULL);
#else
    return AESDMA_Get_PS_MatchedPTN();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void)
{
    return AESDMA_Get_PS_MatchedPTN();
}


DRVAESDMA_RESULT MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    PAESDMA_NOTIFY pNotifyParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pNotifyParam = (PAESDMA_NOTIFY)malloc(sizeof(AESDMA_NOTIFY));

    pNotifyParam->eEvents = eEvents;
    pNotifyParam->pfCallback = pfCallback;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Notify, (void*)&pNotifyParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Notify fail\n"));
        free (pNotifyParam);
        return DRVAESDMA_FAIL;
    }

    _AESDMAEvtNotify = eEvents;
    _AESDMACallback = pfCallback;

    free (pNotifyParam);
    return DRVAESDMA_OK;
#else
    if (pfCallback)
    {
        AESDMA_Enable_Int();
    }
    else
    {
        AESDMA_Disable_Int();
    }

    _AESDMAEvtNotify = eEvents;
    _AESDMACallback = pfCallback;

    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
    if (pfCallback)
    {
        AESDMA_Enable_Int();
    }
    else
    {
        AESDMA_Disable_Int();
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel)
{
    _u32AESDMADBGLevel = DbgLevel;
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVAESDMA_FAIL;
    }

    *ppVersion = &_drv_aesdma_version;

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    MS_U32 Index;
    MS_U8 *u8VABuf;
    MS_U8 u8RndNum;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Rand, (void*)&u8RndNum) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Rand fail\n"));
        return DRVAESDMA_FAIL;
    }


    u8VABuf = (MS_U8 *)MsOS_PA2KSEG1((MS_U32)u32PABuf);

    for( Index = 0; Index < u32Size; Index++ )
    {
        *(u8VABuf + Index) = u8RndNum;
    }

    return DRVAESDMA_OK;
#else
    MS_U32 Index;
    MS_U8 *u8VABuf = (MS_U8 *)MsOS_PA2KSEG1((MS_U32)u32PABuf);

    for( Index = 0; Index < u32Size; Index++ )
    {
        *(u8VABuf + Index) = AESDMA_Rand();
    }
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Get_Rand_Num(MS_U8 *pu8Num)
{

    *pu8Num = AESDMA_Rand();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetClk,(void*)&benable) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Set_Clk fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else

    if(benable)
    {
        AESDMA_Enable_Clk();
    }
    else
    {
        AESDMA_Disable_Clk();
    }

    return DRVAESDMA_OK;
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
	_DEBUG2K("benable = %x \n", benable);
    if(benable)
    {
        AESDMA_Enable_Clk();
    }
    else
    {
        AESDMA_Disable_Clk();
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_U32 u32PAInBuf, MS_U32 u32Size, MS_U32 u32PAOutBuf)
{
    MS_U8 *u8scattergather_format = 0; //[B7, B6, B5, B4] = MIU byte address;[B3, B2, B1, B0] = the length of this message block
    MS_U32 u32scattergather_Address = 0;
    MS_U32 u32OutOffset = 0;
    MS_U32 u32timeout_count = 0;

    DEBUG_AES_DMA(E_DRVAESDMA_DBG_L1, printf("%s(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n", __FUNCTION__, (int)eMode, (int)u32PAInBuf, (int)u32Size, (int)u32PAOutBuf));

    switch (eMode)
    {
    case E_AESDMA_SHA1:
        u32OutOffset = 20; //Out 160-bit
        break;

    case E_AESDMA_SHA256:
        u32OutOffset = 32; //Out 256-bit
        break;

    default:
        return DRVAESDMA_INVALID_PARAM;
    }
#if 1
    // check miu addr
    if (!_AESDMA_Chk_MIU(u32PAInBuf, u32Size, u32PAOutBuf, (u32PAOutBuf + u32OutOffset)))
    {
        DEBUG_AES_DMA(E_DRVAESDMA_DBG_L1, printf("@@@DRVAESDMA_MIU_ADDR_ERROR\n"));
        return DRVAESDMA_MIU_ADDR_ERROR;
    }
#endif
    // Clear all SHA setting(clear the last outputs)
    SHA_Reset();

    // Error handler for 16-byte alignment limitation
    if(u32PAInBuf & WORD_MASK)
    {
        DEBUG_AES_DMA(E_DRVAESDMA_DBG_L1, printf("Input Address should be 16-byte alignment\n"));
        return DRVAESDMA_FAIL;
    }

    u32scattergather_Address = ALIGN_8(u32PAInBuf + u32Size);

    u32Size /= SHA_UNIT_SIZE;
    u32PAInBuf /= SHA_UNIT_SIZE;

    if(u32PAInBuf > _u32MIU1_Addr)
    {
        SHA_SetAddress((u32scattergather_Address | SHA_MIU1_ENABLE) / SHA_UNIT_SIZE);
        u32PAInBuf = u32PAInBuf | SHA_MIU1_ENABLE;
    }
    else
    {
        SHA_SetAddress((u32scattergather_Address) / SHA_UNIT_SIZE);
    }

    SHA_SetLength(u32Size);

    u8scattergather_format = (MS_U8 *)MsOS_PA2KSEG1(u32scattergather_Address);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x00)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x08)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x10)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x18)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((u32PAInBuf)>>0x00)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((u32PAInBuf)>>0x08)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((u32PAInBuf)>>0x10)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((u32PAInBuf)>>0x18)&0xFF);

    SHA_SelMode((AESDMA_SHAMode)eMode);
    MsOS_DelayTaskUs(40);

    MsOS_FlushMemory();

    MsOS_DelayTaskUs(200);

    SHA_Start(1);

    while ((SHA_Get_Status() & SHARNG_CTRL_SHA_READY) == 0)
    {
        MsOS_DelayTaskUs(10);
        u32timeout_count++;
#if (SHA_WAIT==1)
        //500ms timeout
        if(u32timeout_count>SHA_TIMEOUT_VALUE)
        {
            SHA_Reset();
            return DRVAESDMA_FAIL;
        }
#endif
    }


    SHA_Out(MsOS_PA2KSEG1(u32PAOutBuf));
    while ((SHA_Get_Status() & SHARNG_CTRL_SHA_BUSY) != 0)
        ;

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_U32 u32PAInBuf, MS_U32 u32Size, MS_U32 u32PAOutBuf)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    PSHA_CALCULATE pShaCalParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pShaCalParam = (PSHA_CALCULATE)malloc(sizeof(SHA_CALCULATE));
    if (NULL == pShaCalParam)
        return DRVAESDMA_FAIL;
    
    pShaCalParam->eMode= eMode;
    pShaCalParam->u32PAInBuf= u32PAInBuf;
    pShaCalParam->u32Size= u32Size;
    pShaCalParam->u32PAOutBuf= u32PAOutBuf;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_SHA_Calculate,(void*)pShaCalParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_SHA_Calculate fail\n"));
        free(pShaCalParam);
        return DRVAESDMA_FAIL;
    }

    free(pShaCalParam);
    return DRVAESDMA_OK;
#else
    return _MDrv_SHA_Calculate(eMode,u32PAInBuf,u32Size,u32PAOutBuf, NULL);
#endif    
}
DRVAESDMA_RESULT MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{
    _DEBUG2K("\n");
    //not implement yet
    return DRVAESDMA_OK;
}



#ifdef MOBF_ENABLE
//-------------------------------------------------------------------------------------------------
/// MOBF BDMA without any Encrypt (sw patch)
///

/// @return AESDMA_Result
/// @note Must be called after MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_DES_CTR,1)
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_DmaOnly(void)
{
    MOBF_DisableTDES();
    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF encrypt (from AESDMA to MIU)
/// @param  u32Key                \b IN: Encrypt Key
/// @param  bEnable                \b IN: Encrypt Enable/disable

/// @return AESDMA_Result
/// @note
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    MOBF_WriteKey(u32Key);
    MOBF_WriteEnable(bEnable);

    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF decrypt (from MIU to AESDMA)
/// @param  u32Key                \b IN: Decrypt Key
/// @param  bEnable                \b IN: Decrypt Enable/disable

/// @return AESDMA_Result
/// @note
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    MOBF_ReadKey(u32Key);
    MOBF_ReadEnable(bEnable);

    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF One Way
/// @return AESDMA_Result
/// @note disable read mobf key
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_OneWay(void)
{
    MOBF_OneWay();
    return DRVAESDMA_OK;
}

#endif


//-------------------------------------------------------------------------------------------------
/// Mask Scrmb bit(for decrypt using)
/// @param  bEnable                \b IN: Mask Scrmb Enable/disable
/// @return AESDMA_Result
/// @note mask Scrmb bit
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Mask_Scrmb(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_MASKSCRMB,(void*)&bEnable) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_MaskScrmb fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Mask_Scrmb(bEnable);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Scrmb Pattern
/// @param  ePattern                \b IN: input Scrmb pattern(10/11)
/// @return AESDMA_Result
/// @note set Scrmb Pattern
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Set_ScrmbPattern(ePattern);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN,(void*)&ePattern) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_SetScrmbPattern fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_ScrmbPattern(ePattern);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Scrmb Pattern
/// @param  ePattern                \b IN: input Scrmb pattern(10/11)
/// @return AESDMA_Result
/// @note set Scrmb Pattern
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Set_AddedScrmbPattern(ePattern);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
	_DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN,(void*)&ePattern) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_SetAddedScrmbPattern fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_AddedScrmbPattern(ePattern);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Query Pid Count
/// @return Pid Count
/// @note query pid count
//-------------------------------------------------------------------------------------------------
MS_U8 _MDrv_AESDMA_Parser_QueryPidCount(MS_U8* retCount)
{
    _DEBUG2K("\n");
    *retCount = AESDMA_Parser_Query_PidCount();
    return *retCount;
}

MS_U8 MDrv_AESDMA_Parser_QueryPidCount(void)
{
#if (AESDMA_UTOPIA20)

    MS_U8 mCount = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT,(void*)&mCount) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_QueryPidCount fail\n"));
        return mCount;
    }
    return mCount;

#else
    return AESDMA_Parser_Query_PidCount();
#endif
}


//-------------------------------------------------------------------------------------------------
/// Set Pid
/// @param  u8Index                 \b IN: Pid Index(0/1)
/// @param  u16Pid                   \b IN: Pid Value(max value is 0x1FFF)
/// @return AESDMA_Result
/// @note set Pid
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
    _DEBUG2K("\n");
    MS_U8 u8PidCount;
    // check Pid value
    if (u16Pid > AESDMA_PARSER_PID_MAX)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    // check Pid count
    u8PidCount = AESDMA_Parser_Query_PidCount();
    if (u8Index >= u8PidCount)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Parser_Set_Pid(u8Index,u16Pid);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
    _DEBUG2K("u8Index: 0x%x, u16Pid: 0x%x\n", u8Index, u16Pid);
#if (AESDMA_UTOPIA20)
    AESDMA_PARSER_PID_CONFIGURATIONS mConfig;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    mConfig.u8PidIndex = u8Index;
    mConfig.u16Pid = u16Pid;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_SETPID,(void*)&mConfig) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_SetPid fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    MS_U8 u8PidCount;
    // check Pid value
    if (u16Pid > AESDMA_PARSER_PID_MAX)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    // check Pid count
    u8PidCount = AESDMA_Parser_Query_PidCount();
    if (u8Index >= u8PidCount)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Parser_Set_Pid(u8Index,u16Pid);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Bypass Pid
/// @param  bEnable                \b IN: Bypass Pid Enable/disable
/// @return AESDMA_Result
/// @note bypass Pid
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Bypass_Pid(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_BYPASSPID,(void*)&bEnable) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_BypassPid fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Bypass_Pid(bEnable);
    return DRVAESDMA_OK;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Init Parser Encrypt
/// @param  eMode                \b IN: input parser mode
/// @return AESDMA_Result
/// @note init parser encrypt
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    AESDMA_Parser_Insert_Scrmb(TRUE);

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{

    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_ENCRYPT,(void*)&eMode) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_Encrypt fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    AESDMA_Parser_Insert_Scrmb(TRUE);

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Init Parser Decrypt
/// @param  eMode                \b IN: input parser mode
/// @return AESDMA_Result
/// @note init parser decrypt
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
    _DEBUG2K("\n");
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    AESDMA_Parser_Remove_Scrmb(TRUE);

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_DECRYPT,(void*)&eMode) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_Encrypt fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    AESDMA_Parser_Remove_Scrmb(TRUE);

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{

    MS_U32 u32KeyLen = 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = (MS_U32 *)(pstSign);
    MS_U32 *pu32Key = (MS_U32 *)(pstKey);

    _DEBUG2K ("(eMode = 0x%08X, Sign = 0x%08X, Key = 0x%08X)\n", (int)eMode, (int)pstSign, (int)pstKey);

	RSA_Reset();
	RSA_Ind32Ctrl(1);//ind32_ctrl=0xE0

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
        {
            _DEBUG2K("\n");
            u32KeyLen = 128;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            _DEBUG2K("\n");
            u32KeyLen = 128;
            u8Public = 0;
        }
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
        {
            _DEBUG2K("\n");
            u32KeyLen = 256;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            _DEBUG2K("\n");
            u32KeyLen = 256;
            u8Public = 0;
        }
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
        {
            _DEBUG2K("\n");
            u32KeyLen = 32;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA256_PRIVATE:
        {
            _DEBUG2K("\n");
            u32KeyLen = 32;
            u8Public = 0;
        }
        break;
    default:
        _DEBUG2K("\n");
        return DRVAESDMA_INVALID_PARAM;
    }

	RSA_LoadSram((pu32Sig+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)-1),E_RSA_ADDRESS_A);
    RSA_LoadSram((pu32Key+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)-1),E_RSA_ADDRESS_N);
    RSA_LoadSram((pu32Key+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)),E_RSA_ADDRESS_E);

    RSA_SetKeyLength((u32KeyLen/RSA_UNIT_SIZE)-1);
    RSA_SetKeyType(0,u8Public); //sw key, public key

	RSA_ExponetialStart();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    _DEBUG2K("eMode: 0x%x\n", (int)eMode);
#if (AESDMA_UTOPIA20)
    PRSA_CALCULATE pRsaCalParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pRsaCalParam = (PRSA_CALCULATE)malloc(sizeof(RSA_CALCULATE));
    if (NULL == pRsaCalParam)
        return DRVAESDMA_FAIL;

    pRsaCalParam->pstSign = (DrvAESDMA_RSASig *)malloc(sizeof(DrvAESDMA_RSASig));
    memcpy (pRsaCalParam->pstSign, pstSign, sizeof(DrvAESDMA_RSASig));

    pRsaCalParam->pstKey = (DrvAESDMA_RSAKey *)malloc(sizeof(DrvAESDMA_RSAKey));
    memcpy (pRsaCalParam->pstKey, pstKey, sizeof(DrvAESDMA_RSAKey));

    pRsaCalParam->eMode= eMode;

    _DEBUG2K("eMode: 0x%x\n", (int)pRsaCalParam->eMode);


    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_Calculate,(void*)pRsaCalParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_RSA_Calculate fail\n"));
        free (pRsaCalParam->pstSign);
        free (pRsaCalParam->pstKey);
        free(pRsaCalParam);
        return DRVAESDMA_FAIL;
    }
    free (pRsaCalParam->pstSign);
    free (pRsaCalParam->pstKey);
    free(pRsaCalParam);
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Calculate(pstSign,pstKey,eMode);
#endif    
}

DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void)
{
    if(!RSA_Get_RSA_IsFinished())
    {
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}

DRVAESDMA_RESULT MDrv_RSA_IsFinished(void)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    MS_U32 u32Timeout = 0x100000;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_IsFinished,(void*)&u32Timeout) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_RSA_IsFinished fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_IsFinished();
#endif    
}

DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
	_DEBUG2K("eMode: 0x%x\n", eMode);
    MS_U32 i = 0;
    MS_U32 u32KeyLen = 0;

    MS_U32 *pu32RSAOut = (MS_U32 *)(pstRSAOut);

    RSA_Ind32Ctrl(0);//ind32_ctrl=0xC0

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
    case E_DRVAESDMA_RSA1024_PRIVATE:
        u32KeyLen = 128;
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
    case E_DRVAESDMA_RSA2048_PRIVATE:
        u32KeyLen = 256;
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
    case E_DRVAESDMA_RSA256_PRIVATE:
        u32KeyLen = 32;
        break;
    default:
    	_DEBUG2K("Error: Mode 0x%x\n", eMode);
        return DRVAESDMA_INVALID_PARAM;
    }

    _DEBUG2K("RSA Out Addr: 0x%x\n", pstRSAOut);
    for( i = 0; i<(u32KeyLen/RSA_UNIT_SIZE); i++)
    {
        RSA_SetFileOutAddr(i);
        RSA_FileOutStart();
        *(pu32RSAOut+i) = RSA_FileOut();    
    }

    //clear the used key whenever calculating is finished
    RSA_Reset();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    _DEBUG2K("eMode: 0x%x\n", eMode);
#if (AESDMA_UTOPIA20)
    PRSA_OUTPUT pRsaOutParam = NULL;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    pRsaOutParam = (PRSA_OUTPUT)malloc(sizeof(RSA_OUTPUT));
    if (NULL == pRsaOutParam)
    {
        return DRVAESDMA_FAIL;
    }
        
    pRsaOutParam->eMode= eMode;
    pRsaOutParam->pstRSAOut= pstRSAOut;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_Output,(void*)pRsaOutParam) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_RSA_Output fail\n"));
        free(pRsaOutParam);
        return DRVAESDMA_FAIL;
    }
    free(pRsaOutParam);
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Output(eMode,pstRSAOut);
#endif     
}

MS_U32 MDrv_AESDMA_SetPowerState(EN_POWER_MODE u16PowerState)
{
    _DEBUG2K("\n");
    static EN_POWER_MODE u16PreAESDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        u16PreAESDMAPowerState = u16PowerState;
		_u8IsInit = FALSE;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        //MsOS_DeleteTask(_s32AESDMATaskId);
        //MsOS_DeleteMutex(_s32AESDMAMutexId);
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreAESDMAPowerState == E_POWER_SUSPEND)
        {
            MDrv_AESDMA_Init(0, 0x20000000, 2);

            u16PreAESDMAPowerState = u16PowerState;
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
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

DRVAESDMA_RESULT MDrv_AESDMA_Exit(void)
{
    _DEBUG2K("\n");
    if (_u8IsInit == FALSE)
    {
        MS_DEBUG_MSG(printf("AESDMA is not init\n"));
        return DRVAESDMA_FAIL;
    }

#if (AESDMA_UTOPIA20)
    if(UtopiaClose(pInstantAesdma) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("close AESDMA instance fail\n"));
        pInstantAesdma = NULL;
        return DRVAESDMA_FAIL;
    }
    pInstantAesdma = NULL;
#endif
    return DRVAESDMA_OK;
}

//reserved for secure protect function
/*
DRVAESDMA_RESULT MDrv_AESDMA_SecureSetKey(MS_U32 *pCipherKey)
{
    if (pCipherKey != NULL)
    {
        AESDMA_Secure_SetCipherKey(pCipherKey);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureProtect(MS_BOOL bEnable)
{
    AESDMA_SecureProtect(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureSetIV(MS_U32 *pInitVector)
{
    if (pInitVector != NULL)
    {
        AESDMA_Secure_SetInitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetFileOutEnable(MS_BOOL bEnable)
{
    AESDMA_Set_FileOutEnable(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureStart(MS_BOOL bStart)
{
    MsOS_FlushMemory();
    AESDMA_Secure_Start(bStart);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureIsFinished(void)
{
    if(AESDMA_Get_AESDMA_SecureIsFinished())
    {
        MsOS_ReadMemory();
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}
*/

DRVAESDMA_RESULT _MDrv_AESDMA_IsSecretKeyInNormalBank(MS_U8 *retVal)
{
    _DEBUG2K("\n");
    *retVal = AESDMA_IsSecretKeyInNormalBank ();
    if (*retVal == 0)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        return DRVAESDMA_OK;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_IsSecretKeyInNormalBank(void)
{

#if (AESDMA_UTOPIA20)

    MS_U8 retVal = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK,(void*)&retVal) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Parser_Encrypt fail\n"));
        return DRVAESDMA_FAIL;
    }

    _DEBUG2K("Ret: %d\n", retVal);

    if (retVal == 0)
        return DRVAESDMA_FAIL;
    else
        return DRVAESDMA_OK;


#else
    if (AESDMA_IsSecretKeyInNormalBank() == 0)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        return DRVAESDMA_OK;
    }
#endif
}

DRVAESDMA_RESULT MDrv_HDCP_ProcessCipher(MS_U8 u8Idx, MS_U8* pu8privData, MS_U8 *pu8Ks, MS_U8 *pu8LC128)
{
    MS_U8 i = 0;
    MS_U8 ucTmp[16] = {0};
    MS_U8 u8TmpSize = sizeof (ucTmp)/sizeof(MS_U8);

    //
    //get ks XOR LC128
    //
    for ( i = 0; i < u8TmpSize; i++ )
    {
        ucTmp[i] = pu8Ks[i] ^ pu8LC128[i];
    }


    HDCP_ProcessCipher(u8Idx, ucTmp, pu8privData);

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable)
{
	_DEBUG2K("\n");
	AESDMA_Parser_Enable_Two_Keys(bEnable);
	return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable)
{
	_DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_ENABLE_Two_Key,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_CMD_AESDMA_ENABLE_Two_Key fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
	_DEBUG2K("\n");
	AESDMA_Parser_Enable_Two_Keys(bEnable);
	return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector)
{
    _DEBUG2K("\n");
    if (pInitVector != NULL)
    {
    	AESDMA_Set_CipherOddKey(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector)
{
	_DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_SetOddKey,(void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetOddKey fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    if (pInitVector != NULL)
    {
    	AESDMA_Set_CipherOddKey(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_ParserStart(MS_BOOL bStart)
{
    _DEBUG2K("\n");
    MsOS_FlushMemory();
    AESDMA_Start(bStart);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_ParserStart(MS_BOOL bStart)
{
    _DEBUG2K("\n");
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_Start,(void*)&bStart) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_Start fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_ParserStart(bStart);
#endif
}

