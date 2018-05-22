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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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

#ifndef _API_VOIP_MFE_H_
#define _API_VOIP_MFE_H_

#ifdef __cplusplus
extern "C"
{
#endif

// below function is used for T8/T12/Janus2 VOIP project

/* vop coding modes */
// type: I_VOP, P_VOP, or B_VOP
#define I_VOP        0
#define P_VOP        1
#define B_VOP        2


 typedef enum {
    PQ_LOW,
    PQ_NORMAL,
    PQ_HIGH
 } PQ_LEVEL;

 typedef enum {
    VOIP_MPEG4,
    VOIP_H263,
    VOIP_H264
 } VOIP_CODEC_TYPE;


 typedef enum {
    MFE_Invalid            = 0x00,
    MFE_Init                 = 0x20,
    MFE_Idle                =0x30,
    MFE_Busy               =0x40,
    MFE_DeInit             =0x50,
    MFE_Error               =0x60
 } MFE_STATE;

// <<< Calling sequences >>>
//
//      MApi_MFE_SetGOP
//      MApi_MFE_SetBufferInfo
//      MApi_MFE_Initialize
//      MApi_MFE_Encode
//      MApi_MFE_CompressOnePicture     //loop
//      (optional) MApi_MFE_GetVOL
//      (optional) MApi_MFE_ResetBitrate
//      (optional) MApi_MFE_ResetFramerate
//      MApi_MFE_DeInitialize



/*!
 * \brief
 *  create MFE thread, start encoder and wait input frame.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_Encode(void);

/*!
 * \brief
 *  encode one frame. This function will wake up MFE thread to encode, then return directly without wait encode done. MFE thread will callback when enocde done.
 *
 * \param[in] YUVPlane The image physical address. it must be 256 alignment.
 * \param[in] bForceIframe force this frame tobe I.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_CompressOnePicture(MS_U32 YUVPlane, MS_BOOL bForceIframe);

/*!
 * \brief
 *  Set memory buffer for MFE.
 *
 * \param[in] s32MiuType miu 0, 1 or both.
 * \param[in] u32MfePA physical address without offset. must be 256 alignment.
 * \param[in] u32MiuOffset miu offset.
 * \param[in] u32MemSize buffer size..
 * \return TRUE if success.
 * \sa
 * \note
 * \warning MIU type must be the same with DIP. You must switch miu before encode.
 */
MS_BOOL MApi_MFE_SetBufferInfo(MS_S32 s32MiuType, MS_U32 u32MfePA, MS_U32 u32MiuOffset, MS_U32 u32MemSize);

/*!
 * \brief
 *  Set memory buffer for MFE.
 *
 * \param[out] Buf get SPS, PPS. size = (Buf[1]<<8)|Buf[0];.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_GetVOL(MS_S8* Buf);

// bFirst_or_last_byte_of_Frame: 0x00 error state, 0x10 first , 0x01 last at frame, 0x11 one frame
typedef void (*StreamCallback)(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame);

typedef void (*CleanCallback)(MS_U32 u32FrameCoded, MS_U32 u32YUVPlane);

/*!
 * \brief
 *  Set GOP size for MFE.
 *
 * \param[in] PbetweenI GOP size when is_infinite=false. If is_infinite=TRUE, PbetweenI will only used for RateControl.That means there is no I frame(all are pframes) only if MApi_MFE_CompressOnePicture be called and bForceIframe=true.
 * \param[in] is_infinite =TRUE will never get I frame. PbetweenI still must set for ratecontrol, you can set a large number. ex:  (1000,TRUE);
 * \return TRUE if success.
 * \sa
 * \note Each I frame include SPS and PPS for SKYPE requested.
 * \warning GOP size always required by MFE. This function must be called before MApi_MFE_Initialize.
 */
MS_BOOL MApi_MFE_SetGOP(MS_S32 PbetweenI,MS_BOOL is_infinite);

/*!
 * \brief
 *  Changed Bitrate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] bitrate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetBitrate(MS_S32 bitrate);

/*!
 * \brief
 *  Changed Framerate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] framerate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetFramerate(MS_S32 framerate);

/*!
 * \brief
 *  Changed Framerate and Bitrate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] bitrate
 * \param[in] framerate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetBitrateAndFramerate(MS_S32 bitrate,MS_S32 framerate);
/*!
 * \brief
 *  MFE initial function.
 *
 * \param[in] codec_type h263/mpeg4/h264.
 * \param[in] u32Width The image width.
 * \param[in] u32Height The image height.
 * \param[in] picture_quality encode quality, higher quality higher bitrate.
 * \param[in] out_cb Callback function will be called after encoded one frame done.
 * \param[in] mfe_CleanBufStatus Callback function will be called after encoded one frame done.
 * \param[in] pCtx use for Callback function.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_Initialize(VOIP_CODEC_TYPE codec_type,MS_U32 u32Width, MS_U32 u32Height, PQ_LEVEL picture_quality, StreamCallback out_cb, CleanCallback mfe_CleanBufStatus, void *pCtx);

/*!
 * \brief
 *  MFE deinitial function. delete MFE thread.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning.
 */
MS_BOOL MApi_MFE_DeInitialize(void);

/*!
 * \brief
 *  Get MFE state. Only idle state can call the Dynamic function and compress.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning.
 */
MFE_STATE MApi_MFE_GetState(void);

#ifdef __cplusplus
}
#endif

#endif

