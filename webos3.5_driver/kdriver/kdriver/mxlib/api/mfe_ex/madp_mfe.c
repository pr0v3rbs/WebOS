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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (uMStar Confidential Informationv) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __UBOOT__
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#endif
#endif

#include "MFE_chip.h"
#if defined(_MFE_UTOPIA_)
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "drvMIU.h"
#endif
#include "mfe_type.h"
#include "mfe_common.h"
#include "madp_ms_dprintf.h"

#include "mdrv_mfe_io.h"
#include "drv_mfe_st.h"
#include "madp_mfe.h"
#include "apiVOIPMFE.h"
#if defined(_SUPPORT_JPE_)
#include "Exif.h"
#include "jcmarker.h"
#endif

#if defined(__MFE_G2__)
#include "riumap.h"
#endif

#include "madp_mfe_int.h"

#include "apiMFE_priv.h"



#define CEILING_ALIGN(value, align) (((unsigned int)(value)+((align)-1)) & ~((align)-1))

#if defined(_MFE_UTOPIA_)
extern void MDrv_MFE_ISR(InterruptNum eIntNum);
#endif

#define ALIGN_TO_256(f)           (((f) + 255) & ~255)
#define mfe_fd 0

PVR_Info* appPVR_Info;

static MFE_BOOL MAdp_MFE_Change_State(MFE_STATE* mfe_state,MFE_STATE in_state)
{

    *mfe_state = in_state;

    return TRUE;
}

MS_U8 *my_malloc(MS_VIRT* ppVAStart,unsigned int size,const char* msg)
{
    MFE_U8 *ret = (MFE_U8 *)*ppVAStart;
    ms_dprintf(ADP_L2,"MFE ADP,my_malloc[%s]: VA_START:0x%x,SIZE:0x%x\n", msg, (unsigned int)ret, size);

#ifdef MEM_CHECK
            ms_dprintf(ADP_L1,"my_malloc:Check mem set.\n");
            memset(ret, 0xFF, size);
            sleep(100000);
#endif

    *ppVAStart += size;
    return ret;
}


static MFE_BOOL MAdp_MFE_Proc_nonThread(MEMMAP_CUR_t input_frame)
{

    MS_U32 nDispOrder=0;    //     Driver is responsible for frame buffer re-ordering
    MS_VIRT Vaddr;
    MS_S32 Vsize=0;
    BITSFRAME_INFO bitsinfo;
#if defined(_SUPPORT_JPE_)
    int fdc_size = 0;
#endif
    memset(&bitsinfo,0x0,sizeof(BITSFRAME_INFO));
#ifdef _MFE_M1_
    //get last frame DropFlag.
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MFE_U32)(&appPVR_Info->nDropFlag));

    //get current frame SkipFlag
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_SKIPFLAG,  (MFE_U32)(&appPVR_Info->nSkipFlag));
#endif
    //if(appPVR_Info->nDropFlag==0)
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DISPORDER, (MFE_U32)&nDispOrder);
#if defined(_MFE_UTOPIA_)
    if(appPVR_Info->enableISR) {
        mfe_open_int();
        MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_MFE);
    }
#endif
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_ENC_ONEFRAME, (MFE_U32)&input_frame);

    if(appPVR_Info->nSkipFlag==1) {
        ms_dprintf(ADP_L2,"SkipFrame\n");
#ifndef _GenSkipHeader_
        return TRUE;
#endif
    }
    Vsize = 0;
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC)  {//jpeg
        //FDC_t fdc;
        int q_table[128];
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_QTABLE, (MFE_U32)&q_table);
//        set_fdc(appPVR_Info->nDispWidth,appPVR_Info->nDispHeight);
        get_fdc(appPVR_Info->BitstreamOutBuffer.miuPointer,&fdc_size,appPVR_Info->nDispWidth,appPVR_Info->nDispHeight,
            &q_table[0],&q_table[64],appPVR_Info->exif_enable);

        ms_dprintf(ADP_L2,"header size = 0x%x\n",fdc_size);
        //memcpy(appPVR_Info->BitstreamOutBuffer.miuPointer,&fdc,fdc_size);
        Vsize = fdc_size;
    }
#endif
#ifdef _SW_BUF_MODE_
    if(appPVR_Info->m_bFrameMode == 0) {
         ms_dprintf(ADP_L1,"fake rowdone\n");
        mfe_fake_row_mode(input_frame,appPVR_Info->nBufWidth,appPVR_Info->nBufHeight);
    }
#endif
//    MsOS_DelayTask(20); //delay 2 ms
    //get bits

    while(1) {
#ifdef _GenSkipHeader_
        if(appPVR_Info->nSkipFlag==0)
#endif

        //mfe_wait_int(appPVR_Info->enableISR);
        mfe_wait_int(appPVR_Info->enableISR,appPVR_Info->ISRWaitTime);

        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GETBITS, (MFE_U32)(&bitsinfo));

        if(bitsinfo.status != BITS_INFO_FRAMEDONE) {
            ms_dprintf(ADP_L0,"Get Bits fasil!,bitsinfo.status = %d \n",bitsinfo.status);
            return FALSE;
        }

        MFE_ASSERT(bitsinfo.size >= 0);
        if(appPVR_Info->enableSecurityMode){
            Vsize = bitsinfo.size;
            goto NONCOPY;
        }
        Vaddr  = (MFE_U32) MAdp_MPool_PA2KSEG1(bitsinfo.miuAddress+appPVR_Info->MfeApiInfo.miu_offset);

        ms_dprintf(ADP_L2,"bitsinfo.miuAddress = 0x%x,Vaddr=0x%x,size =0x%x\n",(unsigned int)bitsinfo.miuAddress,(unsigned int)Vaddr,(unsigned int)bitsinfo.size);
        if((appPVR_Info->BitstreamOutBufferSize-Vsize) < bitsinfo.size) {
            ms_dprintf(ADP_L0,"BitstreamOutBufferSize is too small\n");
            return FALSE;
        }
        //memcpy(appPVR_Info->BitstreamOutBuffer.miuPointer +Vsize, (MFE_U8 *)Vaddr, bitsinfo.size);
        appPVR_Info->BitstreamOutBuffer.miuAddress = bitsinfo.miuAddress + appPVR_Info->MfeApiInfo.miu_offset;
        Vsize += bitsinfo.size;

        ms_dprintf(ADP_L2,"memcpy 0x%x, total 0x%x\n",(unsigned int)bitsinfo.size,(unsigned int)Vsize);
        if(bitsinfo.status == BITS_INFO_FRAMEDONE) {//frame done
            if(appPVR_Info->enableCABAC) {
                //CABAC stuffing byte
                if(bitsinfo.stuffing_bytes!=0) {
                    int i;
                    MS_U8 *obufp = (MS_U8*)(Vaddr) + Vsize;

                    ms_dprintf(ADP_L2,"CABAC stuffing byte 0x%x\n",bitsinfo.stuffing_bytes);
                    for(i=0;i<bitsinfo.stuffing_bytes/3;i++) {
                        *obufp++ = 0x0;
                        *obufp++ = 0x0;
                        *obufp++ = 0x3;
                    }
                    for(i=0;i<bitsinfo.stuffing_bytes%3;i++) {
                        *obufp++ = 0x0;
                    }

                }
                Vsize += bitsinfo.stuffing_bytes;
            }
            ms_dprintf(ADP_L2,"frame done\n");
            break;
        }
        else if(bitsinfo.status == BITS_INFO_BUFFULL) {
            ms_dprintf(ADP_L0,"frame full\n");
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_CLEAR_BITSBUF, 1);
        }
        else if (bitsinfo.status == BITS_INFO_IMG_BUFFULL){
            ms_dprintf(ADP_L0,"input buf full\n");
            return FALSE;
        } else {
            ms_dprintf(ADP_L0,"[MFE]unknown state\n");
            return FALSE;
        }
    }

NONCOPY:
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_CLEAR_BITSBUF, 1);
    appPVR_Info->BitstreamOutBuffer.size = Vsize;
#ifndef MSOS_TYPE_LINUX_KERNEL
    if(Vsize)
        appPVR_Info->g_StreamCallBackFunc(appPVR_Info->g_pCustomCtx, (MFE_S32)bitsinfo.voptype,
            appPVR_Info->BitstreamOutBuffer.miuPointer, Vsize, 0x3);

    appPVR_Info->g_CleanCallBackFunc(1,(MFE_U32)&input_frame);
#else
    ms_dprintf(ADP_L2,"Kernel mode not support callback.Type:%d Addr:0x%x, Size:0x%x\n", (unsigned int)bitsinfo.voptype, (unsigned int)appPVR_Info->BitstreamOutBuffer.miuPointer, (unsigned int)Vsize);
#endif
    ms_dprintf(ADP_L2,"frame size = 0x%x\n",appPVR_Info->BitstreamOutBuffer.size);

#if (DEBUG_LEVEL & ADP_L2)
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MFE_U32)(&appPVR_Info->nDropFlag));
    if(appPVR_Info->nDropFlag)
        ms_dprintf(ADP_L2,"Dropframe.");
#endif
    return TRUE;
}

MFE_BOOL MFE_SetRIU_Base(void)
{

#ifdef __MFE_G2__
    MFE_U32 u32RiuBaseAdd=0;
    MsIoMapInit();
    u32RiuBaseAdd = MsIoMapGetRiuBase(RIUBASE_MFE0);

    ms_dprintf(ADP_L2,"MFE Init u32RiuBaseAdd = 0x%X\n", (unsigned int)u32RiuBaseAdd);
#elif defined(WIN32)
    MFE_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0xa80;
#else
    MFE_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW)){
        ms_dprintf(ADP_L0,"MFE Init Err: MMIO_GetBASE failure\n");
        printf("[i]MFE Init Err: MMIO_GetBASE failure, u32RiuBaseAdd = 0x%lx, u32NonPMBankSize = 0x%lx\n",u32RiuBaseAdd,u32NonPMBankSize);
        return FALSE;
    }

    if((u32RiuBaseAdd != 0xfd200000) || (u32RiuBaseAdd == 0x00))
    {
        printf("[i]MFE Init Err: MMIO_GetBASE failure, u32RiuBaseAdd = 0x%lx, u32NonPMBankSize = 0x%lx\n",u32RiuBaseAdd,u32NonPMBankSize);
        printf("[i]do back trace [%s][%d] ================+\n",__FUNCTION__,__LINE__);
    }

    printf("[i][%d]MFE Init u32RiuBaseAdd = 0x%X, u32NonPMBankSize = 0x%X, \n",__LINE__,(unsigned int)u32RiuBaseAdd,(unsigned int)u32NonPMBankSize);
    ms_dprintf(ADP_L3,"MFE Init u32RiuBaseAdd = 0x%X, u32NonPMBankSize = 0x%X, \n", (unsigned int)u32RiuBaseAdd,(unsigned int)u32NonPMBankSize);
    //MDrv_MFE_SetOSRegBase(u32RiuBaseAdd);
#endif
     mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_RIU_BASE, u32RiuBaseAdd);

    return TRUE;

}


MS_BOOL MApi_MFE_CompressOnePicture_ex(MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe)
{
    return _MApi_MFE_CompressOnePicture(appPVR_Info, YUVPlane, bForceIframe);
}

MS_BOOL MApi_MFE_Encode(void)
{
    return _MApi_MFE_Encode(appPVR_Info);
}

MS_BOOL MApi_MFE_DeInitialize(void)
{

    // old api didnot support multi instance, so always power off mfe when deInit.
    POWERON_t power_Info;
    power_Info.is_off = 1;
    power_Info.clock_level = MFE_CLK_FAST;
    mfe_ioctl((MFE_S32)NULL, MFE_IOC_POWEROFF, (MFE_U32)&power_Info);
    return _MApi_MFE_DeInit(appPVR_Info);
}


MFE_STATE MApi_MFE_GetState(void)
{
    return appPVR_Info->mfe_state;
}
MS_BOOL MApi_MFE_GetVOL_ex(void *header_info)
{
    return _MApi_MFE_GetVOL(appPVR_Info,header_info);
}

MS_BOOL MApi_MFE_Initialize_ex(PVR_Info* in_Info,StreamCallback out_cb,CleanCallback
mfe_CleanBufStatus, void *ctx)
{

    MS_BOOL ret;
    ret = _MApi_MFE_Init(in_Info);
    if(ret == FALSE)
        return FALSE;
    appPVR_Info->g_pCustomCtx = ctx;
    appPVR_Info->g_StreamCallBackFunc = out_cb;
    appPVR_Info->g_CleanCallBackFunc = mfe_CleanBufStatus;

    return ret;

}


MS_BOOL MApi_MFE_SetConfig(MFE_ENC_CONF conf_type, void *value)
{
    unsigned long  arg = 0;
    int set_value;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    switch (conf_type) {
        case MFE_ENC_SETCONF_FRAME_TYPE:
            set_value = (*((int*) value));
            ms_dprintf(ADP_L2,"MApi_MFE_SetConfig %d\n",set_value );
            if(set_value == I_VOP)
                mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, &arg);
            break;
        case MFE_ENC_SETCONF_CLEAR_POWERSUSPEND:
            {
                MFE_S32 MFE_fd = 0;
#if 1 //str
                mfe_ioctl((MFE_S32)&MFE_fd, MFE_IOC_CLEAR_POWER_SUSPEND, &arg);
#else
                ms_dprintf(ADP_L0,"MFE_ENC_SETCONF_CLEAR_POWERSUSPEND only support EINSTEIN\n");
                return FALSE;
#endif
                break;
            }
        case MFE_ENC_SETCONF_RESET_ISR:
            {
                MFE_S32 MFE_fd = 0;
                PVR_Info* in_Info = (PVR_Info*)value;
                appPVR_Info->enableISR = in_Info->enableISR;
                appPVR_Info->ISRWaitTime= in_Info->ISRWaitTime;
                mfe_close_int();
                if(appPVR_Info->enableISR) {
                    mfe_open_int();
                }
                mfe_ioctl((MFE_S32)&MFE_fd, MFE_IOC_RESET_ISR, (MFE_U32)(appPVR_Info));
            }
            break;
        case MFE_ENC_SETCONF_CHANGE_FRAME_RATE:
            set_value = (*((int*) value));
            if(set_value == 0) {
                ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->FrameRatex100 = set_value;
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_FRAMERATE,  (MFE_U32)(appPVR_Info));
            break;
        case MFE_ENC_SETCONF_CHANGE_BIT_RATE:
            set_value = (*((int*) value));
            if(set_value == 0) {
                ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->nBitrate =  set_value;
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE,  (MFE_U32)(appPVR_Info));
            break;
        case MFE_ENC_SETCONF_CHANGE_BIT_RATE_AND_FRAME_RATE:
            {
                PVR_Info* in_Info = (PVR_Info*)value;
                appPVR_Info->nBitrate =  in_Info->nBitrate;
                appPVR_Info->FrameRatex100 =  in_Info->FrameRatex100;
                if(_MApi_MFE_SetBitrateFramerate(appPVR_Info) == FALSE)
                    return FALSE;
#if 0
                if(in_Info->nBitrate== 0 ) {
                    ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
                    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                    return FALSE;
                }
                if(in_Info->FrameRatex100 == 0) {
                    ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
                    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                    return FALSE;
                }
                appPVR_Info->nBitrate =  in_Info->nBitrate;
                appPVR_Info->FrameRatex100 =  in_Info->FrameRatex100;
                mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE_AND_FRAMERATE,  (MFE_U32)(appPVR_Info));
#endif
            }
            break;
        case MFE_ENC_SETCONF_FRAME_TAG:
            appPVR_Info->out_frametag_top = (*((int*) value));
            break;
        case MFE_ENC_SETCONF_RESET_SPSPPS:
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_SPSPPS, &arg);
            break;
        case MFE_ENC_SETCONF_OUT_BSP_BUFFER:
        {
            MEMMAP_t* outbuf = (MEMMAP_t *)value;
            if(outbuf->miuPointer == NULL || outbuf->size == 0) {
                ms_dprintf(ADP_L0,"outbuf->miuPointer == NULL || outbuf->size == 0\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->BitstreamOutBuffer.miuPointer = outbuf->miuPointer;
            appPVR_Info->BitstreamOutBuffer.miuAddress = outbuf->miuAddress;
            appPVR_Info->BitstreamOutBuffer.size = 0;
            appPVR_Info->BitstreamOutBufferSize = outbuf->size;
            break;
        }
        case MFE_ENC_SETCONF_JPE_QUALITY:
            set_value = (*((int*) value));
            if(set_value <0 || set_value > 100) {
                ms_dprintf(ADP_L0,"quality not support %d\n",set_value);
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->quality = (int)set_value;
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_QUALITY,  (MFE_U32)(set_value));
            break;
        case MFE_ENC_SETCONF_SET_COLORFORMAT:
#if defined(_MFE_EINSTEIN_) || defined(_MFE_NIKE_) || defined(_MFE_MADISON_) || defined(_MFE_CLIPPERS_) || defined(_MFE_EINSTEIN3_) || defined(_MFE_MONACO_) || defined(_MFE_MUJI_) || defined(_MFE_MUNICH_) || defined(_MFE_MUSTANG_)
            set_value = (*((int*) value));
            appPVR_Info->bColorFormat =  (MFE_COLOR_FORMAT_INFO)set_value;
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_COLOR_FORMAT,  (MFE_U32)(appPVR_Info));
            break;
#else
            ms_dprintf(ADP_L0,"Not support MFE_ENC_SETCONF_SET_COLORFORMAT on this chip\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
#endif
        default:
            ms_dprintf(ADP_L0,"conf_type error\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}

MS_BOOL MApi_MFE_GetConfig(MFE_ENC_CONF conf_type, void *value)
{

    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    switch (conf_type) {
        case MFE_ENC_GETCONF_FRAME_TAG:
            (*((unsigned int*) value)) = appPVR_Info->out_frametag_top;
            break;
        case MFE_ENC_GETCONF_POWERSUSPEND:
        {
#if 1 // str
            mfe_ioctl(mfe_fd, MFE_IOC_GET_POWER_SUSPEND, (MFE_U32)value);
#else
            ms_dprintf(ADP_L0,"MFE_ENC_GETCONF_POWERSUSPEND only support EINSTEIN\n");
            return FALSE;
#endif
            break;
        }

        case MFE_ENC_GETCONF_OUT_BSP_BUFFER:
        {
            //memcpy(value,&appPVR_Info->BitstreamOutBuffer,sizeof(MEMMAP_t));
            _MApi_MFE_GetOBuf(appPVR_Info,(MEMMAP_t*)value);
            ms_dprintf(ADP_L2,"appPVR_Info->BitstreamOutBuffer.size = 0x%x\n",appPVR_Info->BitstreamOutBuffer.size);
            break;
        }
        case MFE_ENC_GETCONF_JPE_QUALITY:
            (*((int*) value)) = appPVR_Info->quality;
            break;
        case MFE_ENC_GETCONF_DROP:
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MFE_U32)(&appPVR_Info->nDropFlag));
            *((unsigned int *)value) = appPVR_Info->nDropFlag;
            break;
        default:
            ms_dprintf(ADP_L0,"conf_type error\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}


MS_BOOL _MApi_MFE_Init(PVR_Info* mfe_Info)
{
    ms_dprintf(ADP_L1,"_MApi_MFE_Init\n");
    MS_U32 temp;
    MS_U32 nFrameSize;
    MS_U32 MallocToDrvSize ;
    MS_U32 MallocToDrvFrame;
    appPVR_Info = mfe_Info;

    //parameter check
    if((appPVR_Info->nCodecType != MPEG4_ENC) && (appPVR_Info->nCodecType != H263_ENC)
        && (appPVR_Info->nCodecType != H264_ENC) && (appPVR_Info->nCodecType != JPEG_ENC )){
        ms_dprintf(ADP_L0,"nCodecType error\n");
        return FALSE;
    }
#ifdef MSOS_TYPE_LINUX_KERNEL
    if(appPVR_Info->enableVTRateControl) {
        ms_dprintf(ADP_L0,"Kernel mode didnot support VT rate control!\n");
        return FALSE;
    }
#endif
    //video parameter
    if(appPVR_Info->nCodecType != JPEG_ENC){
        if(appPVR_Info->FrameRatex100/100 <= 0) {
                ms_dprintf(ADP_L0,"FrameRatex100 cant be %d\n",appPVR_Info->FrameRatex100);
                return FALSE;
        }
        if(appPVR_Info->nBbetweenP != 0) {
                ms_dprintf(ADP_L0,"nBbetweenP cant be %d\n",appPVR_Info->nBbetweenP);
                return FALSE;
        }
        //if(appPVR_Info->nBitrate <= 0) {
        //    ms_dprintf(ADP_L0,"nBitrate cant be %d\n",appPVR_Info->nBitrate);
        //    return FALSE;
        //}
        if(appPVR_Info->width==0 ||appPVR_Info->height == 0) {
            ms_dprintf(ADP_L0,"appPVR_Info->width==0 ||appPVR_Info->height == 0\n");
            return FALSE;
        }
#if defined(_MFE_EINSTEIN_) || defined(_MFE_EINSTEIN3_) || defined(_MFE_MONACO_) || defined(_MFE_MUJI_)
        if(appPVR_Info->height > 1088) {
            ms_dprintf(ADP_L0,"appPVR_Info->height > 1088\n");
            return FALSE;
        }
        if(appPVR_Info->width > 1920) {
            ms_dprintf(ADP_L0,"appPVR_Info->width > 1920\n");
            return FALSE;
        }
#else
        if(appPVR_Info->height > 720) {
            ms_dprintf(ADP_L0,"appPVR_Info->height > 720\n");
            return FALSE;
        }
        if(appPVR_Info->width > 1280) {
            ms_dprintf(ADP_L0,"appPVR_Info->width > 1280\n");
            return FALSE;
        }
        if(appPVR_Info->enableCABAC) {
            ms_dprintf(ADP_L0,"This Chip didnot support CABAC.\n");
            ms_dprintf(ADP_L0,"Re set to CAVLC.\n");
            appPVR_Info->enableCABAC = 0;
        }
#endif
        if(appPVR_Info->nCodecType == MPEG4_ENC){
            if(appPVR_Info->TimeIncreamentRes==0 || appPVR_Info->VopTimeIncreament == 0)
                return FALSE;
        }
        if(appPVR_Info->m_bFrameMode != 1) {
            ms_dprintf(ADP_L0,"appPVR_Info->m_bFrameMode != 1\n");
             return FALSE;
        }
    }
    else{ //jpeg
        if((appPVR_Info->quality < 1) || (appPVR_Info->quality > 100)) {
            ms_dprintf(ADP_L0,"quality cant be %d\n",appPVR_Info->quality);
            return FALSE;
        }
        if(appPVR_Info->height > 2592) {
            ms_dprintf(ADP_L0,"height cant be %d\n",appPVR_Info->height);
            return FALSE;
        }
        if(appPVR_Info->width > 2960) {
            ms_dprintf(ADP_L0,"width cant be %d\n",appPVR_Info->width);
            return FALSE;
        }
    }

    if(!appPVR_Info->enableSecurityMode && appPVR_Info->BitstreamOutBuffer.miuPointer == NULL) {
        ms_dprintf(ADP_L0,"appPVR_Info->BitstreamOutBuffer.miuPointer == NULL\n");
        return FALSE;
    }
    if(appPVR_Info->enableSecurityMode) {
        ms_dprintf(ADP_L0,"enableSecurityMode\n");
    }
    if((appPVR_Info->MfeAdvInfo.input_imi_en!=0) && (appPVR_Info->MfeAdvInfo.low_bandwidth_en!=0)) {
        ms_dprintf(ADP_L0,"MfeAdvInfo.input_imi_en =%d,MfeAdvInfo.low_bandwidth_en = %d\n",
            appPVR_Info->MfeAdvInfo.input_imi_en,appPVR_Info->MfeAdvInfo.low_bandwidth_en);
        return FALSE; //cannot enable at the same time.
    }
    if(appPVR_Info->bInterlace != PROGRESSIVE) {
        if(appPVR_Info->nBufHeight % 32 != 0){
            ms_dprintf(ADP_L0,"Height must %% 32 for interlace mode\n");
            return FALSE;
        }
    }
#if defined(_MFE_EAGLE_)||defined(_MFE_EDISON_) //eagle only support MSTTILE
    if(appPVR_Info->bColorFormat == YUVTILE) {
        ms_dprintf(ADP_L0,"Only support MSTTILE, force to MSTTILE.\n");
        appPVR_Info->bColorFormat = MSTTILE;
    }
#endif
    ms_dprintf(ADP_L3,"video parameter check done\n");
    //set RIU BASE
    if(MFE_SetRIU_Base() == FALSE) {
        ms_dprintf(ADP_L0,"MFE_SetRIU_Base() == FALSE\n");
        return FALSE;
    }

    appPVR_Info->nDropFlag = 0;

    appPVR_Info->nOutBuffer = OBUF_NUM;
#ifdef MSOS_TYPE_LINUX_KERNEL
    appPVR_Info->OutBufferSize = OBUF_SIZE;
#else
    appPVR_Info->OutBufferSize = appPVR_Info->BitstreamOutBufferSize;
#endif
    if((appPVR_Info->nBufHeight*appPVR_Info->nBufWidth) >= (1280*720))
        MallocToDrvFrame = MAX_BUFPOOL_COUNT -1; // Only 1 ref.
    else
        MallocToDrvFrame = MAX_BUFPOOL_COUNT;

    if(appPVR_Info->nCodecType == H264_ENC) {
        unsigned int out_buf_size = appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer;
        if(appPVR_Info->bColorFormat == YUVTILE) {
            nFrameSize = appPVR_Info->nBufHeight*appPVR_Info->nBufWidth*3/2;
            if(appPVR_Info->bInterlace != PROGRESSIVE)
                nFrameSize = (appPVR_Info->nBufHeight>>1)*appPVR_Info->nBufWidth*3/2;
        } else { // MSTTILE or YUV422
            int DstSizeY = CEILING_ALIGN(appPVR_Info->nBufHeight,32)*appPVR_Info->nBufWidth;
            int DstSizeUV = CEILING_ALIGN(appPVR_Info->nBufHeight,64)*appPVR_Info->nBufWidth/2;
            nFrameSize = DstSizeY + DstSizeUV;
            if(appPVR_Info->bInterlace != PROGRESSIVE) {
                int DstSizeY = CEILING_ALIGN(appPVR_Info->nBufHeight/2,32)*appPVR_Info->nBufWidth;
                int DstSizeUV = CEILING_ALIGN(appPVR_Info->nBufHeight/2,64)*appPVR_Info->nBufWidth/2;
                nFrameSize = DstSizeY + DstSizeUV;
            }
        }
            MallocToDrvSize = nFrameSize*MallocToDrvFrame
                + out_buf_size
                + (appPVR_Info->nBufWidth>>4)*64 + 10*1024;
    }
    else if(appPVR_Info->nCodecType == H263_ENC || appPVR_Info->nCodecType == MPEG4_ENC) {
        nFrameSize = appPVR_Info->nBufHeight*appPVR_Info->nBufWidth*3/2;
        MallocToDrvSize = nFrameSize*MallocToDrvFrame+appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer +
             (appPVR_Info->nBufWidth>>4)*64 +
             appPVR_Info->nBufHeight*appPVR_Info->nBufWidth/256*16 + 10*1024;
    }
    else {
        appPVR_Info->MfeApiInfo.mfe_phy_buf_size = appPVR_Info->MfeApiInfo.mfe_phy_buf_size -
            (ALIGN_TO_256(appPVR_Info->MfeApiInfo.mfe_phy_buf_addr) -appPVR_Info->MfeApiInfo.mfe_phy_buf_addr);
        appPVR_Info->MfeApiInfo.mfe_phy_buf_addr = ALIGN_TO_256(appPVR_Info->MfeApiInfo.mfe_phy_buf_addr);

        appPVR_Info->nOutBuffer = OBUF_NUM;
        appPVR_Info->OutBufferSize = MallocToDrvSize = appPVR_Info->MfeApiInfo.mfe_phy_buf_size;
    }

    if(appPVR_Info->enableSecurityMode) {
        MallocToDrvSize = MallocToDrvSize - (appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer);
    }
    ms_dprintf(ADP_L0,"mfe_phy_buf_addr = 0x%x, mfe_phy_buf_size =0x%x, MallocToDrvSize=0x%x\n",appPVR_Info->MfeApiInfo.mfe_phy_buf_addr, (unsigned int)appPVR_Info->MfeApiInfo.mfe_phy_buf_size,(unsigned int)MallocToDrvSize);
    if(appPVR_Info->MfeApiInfo.mfe_phy_buf_size < MallocToDrvSize) {
        ms_dprintf(ADP_L0,"appPVR_Info->MfeApiInfo.mfe_phy_buf_size < MallocToDrvSize\n");
        return FALSE;
    }
#ifdef MSOS_TYPE_LINUX_KERNEL
    appPVR_Info->BitsBuffStart = appPVR_Info->MfeApiInfo.mfe_phy_buf_addr;
#else
    // malloc memory for DRV level
    temp = (MS_U32)my_malloc(&appPVR_Info->MfeApiInfo.mfe_vir_buf_ptr, MallocToDrvSize,"pretend to allocate for MFE driver");

    //pretend to allocate for MFE
    appPVR_Info->BitsBuffStart = (MFE_U32)MAdp_MPool_VA2PA((void*)(temp));
#endif
    appPVR_Info->BitsBuffSize = MallocToDrvSize;
    appPVR_Info->BitsBuffStart -= (appPVR_Info->BitsBuffStart & appPVR_Info->MfeApiInfo.miu_offset);

    appPVR_Info->MfeApiInfo.Encode_stop = 0;
    appPVR_Info->mfe_drvHandle = (void*)MfeDrvMemMalloc(sizeof(MFE_CONFIG),"mfe_drvHandle");
    if ( mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_INIT, (MFE_U32)(appPVR_Info)) != 0)
        return FALSE;

    if(!appPVR_Info->enableSecurityMode)
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_MIU_OFFSET, appPVR_Info->MfeApiInfo.miu_offset);

    if(appPVR_Info->enableISR) {
        ms_dprintf(ADP_L0,"MFE open interrupt\n");
        mfe_open_int();
    }
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC) {
        init_fdc();
        set_fdc(appPVR_Info->nDispWidth,appPVR_Info->nDispHeight);
    }
#endif

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Init);

    return TRUE;

}
MS_BOOL _MApi_MFE_Encode(PVR_Info* mfe_Info)
{
    appPVR_Info = mfe_Info;
    ms_dprintf(ADP_L1,"_MApi_MFE_Encode\n");
    if(appPVR_Info->mfe_state == MFE_Init)
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);
    else {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }
    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}
MS_BOOL _MApi_MFE_GetVOL(PVR_Info* mfe_Info, void *header_info)
{
    ms_dprintf(ADP_L1,"_MApi_MFE_GetVOL\n");
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }


    if(header_info == NULL) {
        ms_dprintf(ADP_L0,"header_info == NULL\n");
        return FALSE;
    }


    //MPEG4
    if(appPVR_Info->nCodecType == MPEG4_ENC) {
        VOL_INFO_t *vol_info = header_info;
        vol_info->pHeader = appPVR_Info->BitstreamOutBuffer.miuPointer;
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_VOL, (MFE_U32)vol_info);
    }
    else if(appPVR_Info->nCodecType == H264_ENC) { // H264
        SPSPPS_INFO_t *spspps_info = header_info;
        //don't use it to get SPS/PPS
        //spspps_info->pHeaderSPS = appPVR_Info->BitstreamOutBuffer.miuPointer;
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_VOL, (MFE_U32)spspps_info);
        appPVR_Info->BitstreamOutBuffer.size = spspps_info->PPSLen + spspps_info->SPSLen;
    }
    else {
        ms_dprintf(ADP_L0,"non-support codec type %d\n",appPVR_Info->nCodecType);
        return FALSE;
    }

    return TRUE;
}
MS_BOOL _MApi_MFE_CompressOnePicture(PVR_Info* mfe_Info, MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe)
{
    unsigned long  arg = 0;
    MFE_BOOL ret;
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    ms_dprintf(ADP_L3,"MApi_MFE_CompressOnePicture()\n");

    //check input alignment
    if(appPVR_Info->bColorFormat == YUVTILE || appPVR_Info->bColorFormat == MSTTILE) {
        if((YUVPlane.Cur_Y0.miuAddress & 0xFF) !=0)
            return FALSE;
        if((YUVPlane.Cur_C0.miuAddress & 0xFF) !=0)
            return FALSE;
    } else { //422 only need 8 byte align
        if((YUVPlane.Cur_Y0.miuAddress & 0x7) !=0)
            return FALSE;
    }
    if(appPVR_Info->m_bFrameMode==0) {
        if((YUVPlane.Cur_Y1.miuAddress & 0xFF) !=0)
            return FALSE;
        if((YUVPlane.Cur_C1.miuAddress & 0xFF) !=0)
            return FALSE;
    }
    if(bForceIframe) {
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, &arg);
    }
    //sem_post(&madp_sem_input);
    ret = MAdp_MFE_Proc_nonThread(YUVPlane);
    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
    return ret;

}
MS_BOOL _MApi_MFE_DeInit(PVR_Info* mfe_Info)
{

    int err = 0;
    MFE_U32 temp=0;
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"MApi_MFE_DeInitialize state check error.[%d] \
            Still Force deinitialize the mfe..\n",appPVR_Info->mfe_state);
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_DeInit);

    appPVR_Info->MfeApiInfo.Encode_stop = 1;

    if(appPVR_Info->mfe_drvHandle != NULL) //mfe_drvHandle = NULL, not init or init fail
    {
        err = mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_FINISH, temp);
        MfeDrvMemFree(&appPVR_Info->mfe_drvHandle,"mfe_drvHandle");
    }

    if(appPVR_Info->enableISR) {
        ms_dprintf(ADP_L0,"MFE close interrupt\n");
        mfe_close_int();
    }
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC)
        deinit_fdc();
#endif
    appPVR_Info = NULL;
    //MfeDrvMemFree(appPVR_Info,"appPVR_Info");
    //MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Invalid);


    return (err == 0);


}

MS_BOOL _MApi_MFE_GetOBuf(PVR_Info* mfe_Info,MEMMAP_t* obuf)
{
    appPVR_Info = mfe_Info;
    memcpy(obuf,&appPVR_Info->BitstreamOutBuffer,sizeof(MEMMAP_t));

    return TRUE;
}

MS_BOOL _MApi_MFE_SetBitrateFramerate(PVR_Info* mfe_Info)
{

    appPVR_Info = mfe_Info;
    if(appPVR_Info->nBitrate== 0 ) {
        ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
        return FALSE;
    }
    if(appPVR_Info->FrameRatex100 == 0) {
        ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
        return FALSE;
    }
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE_AND_FRAMERATE,  (MFE_U32)(appPVR_Info));

    return TRUE;
}

MS_BOOL _MApi_MFE_PowerOff(PVR_Info* mfe_Info)
{
    MFE_S32 MFE_fd = 0;
    POWERON_t power_Info;
    appPVR_Info = mfe_Info;
    power_Info.is_off = 1;
    power_Info.clock_level = MFE_CLK_FAST;
    mfe_ioctl((MFE_S32)&MFE_fd, MFE_IOC_POWEROFF, (MFE_U32)&power_Info);

    return TRUE;
}

MS_BOOL _MApi_MFE_ISREnable(PVR_Info* mfe_Info)
{
    MFE_S32 MFE_fd = 0;
    appPVR_Info = mfe_Info;
    if(mfe_Info->enableISR) {
        mfe_open_int();
    } else {
        mfe_close_int();
    }
    mfe_ioctl((MFE_S32)&MFE_fd, MFE_IOC_SET_ISR, (MFE_U32)(appPVR_Info));
    return TRUE;
}

MS_BOOL _MApi_MFE_SetColorFormat(PVR_Info* mfe_Info)
{

    appPVR_Info = mfe_Info;
#if defined(_MFE_EINSTEIN_) || defined(_MFE_NIKE_) || defined(_MFE_MADISON_) || defined(_MFE_CLIPPERS_) || defined(_MFE_EINSTEIN3_) || defined(_MFE_MONACO_) || defined(_MFE_MUJI_) || defined(_MFE_MUNICH_) || defined(_MFE_MUSTANG_)
            mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_COLOR_FORMAT,  (MFE_U32)(appPVR_Info));
#else
            ms_dprintf(ADP_L0,"Not support MFE_ENC_SETCONF_SET_COLORFORMAT on this chip\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
#endif

    return TRUE;
}

MS_BOOL _MApi_MFE_ResetSPSPPS(PVR_Info* mfe_Info)
{
    unsigned long  arg = 0;
    appPVR_Info = mfe_Info;
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_SPSPPS,&arg);

    return TRUE;
}

MS_BOOL _MApi_MFE_SetFrameType(PVR_Info* mfe_Info, MS_S32 frametype)
{
    unsigned long  arg = 0;
    appPVR_Info = mfe_Info;
    if(frametype == I_VOP)
        mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, &arg);

    return TRUE;
}

MS_BOOL _MApi_MFE_GetOutputInfo(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size)
{
    appPVR_Info = mfe_Info;
    *size = appPVR_Info->BitstreamOutBuffer.size;
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_FRAME_TYPE,  (MFE_U32)(frametype));

    return TRUE;
}

MS_BOOL _MApi_MFE_SetVUI(PVR_Info *mfe_Info)
{
    appPVR_Info = mfe_Info;
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_VUI, (MFE_U32)(appPVR_Info));

    return TRUE;
}

MS_BOOL _MApi_MFE_GetConf_PowerSuspend(PVR_Info *mfe_Info,MFE_U32 *pwr_bits)
{
    appPVR_Info = mfe_Info;
    mfe_ioctl((MFE_S32)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_POWER_SUSPEND, (MFE_U32)(pwr_bits));

    return TRUE;
}

MS_BOOL _MApi_MFE_SetConf_Clear_PowerSuspend(PVR_Info *mfe_Info)
{
    mfe_ioctl(appPVR_Info->mfe_drvHandle, MFE_IOC_CLEAR_POWER_SUSPEND, (MFE_U32)NULL);

    return TRUE;
}



