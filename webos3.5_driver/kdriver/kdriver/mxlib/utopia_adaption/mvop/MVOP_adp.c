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
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "drvMVOP.h"
#include "drvMVOP_v2.h"
#include "MVOP_adp.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
    //extern void* spt_MS_U8;
    extern void* spt_MSIF_Version;
    //extern void* spt_MS_BOOL;
    // 0 pointer
    UADP_SPT_0NXT_DEF(MVOP_InputCfg);
    UADP_SPT_0NXT_DEF(MVOP_TileFormat);
	UADP_SPT_0NXT_DEF(MVOP_Timing);
	UADP_SPT_0NXT_DEF(MVOP_DST_DispInfo);
	UADP_SPT_0NXT_DEF(MVOP_TimingInfo_FromRegisters);
	UADP_SPT_0NXT_DEF(MVOP_VidStat);
	UADP_SPT_0NXT_DEF(MVOP_DrvInfo);
	UADP_SPT_0NXT_DEF(MVOP_DrvStatus);
	UADP_SPT_0NXT_DEF(MVOP_Handle);
	UADP_SPT_0NXT_DEF(MVOP_BaseAddInput);
	UADP_SPT_0NXT_DEF(MVOP_VC1RangeMapInfo);
	UADP_SPT_0NXT_DEF(MVOP_EVDBaseAddInput);
	UADP_SPT_0NXT_DEF(MVOP_EVDFeature);
	UADP_SPT_0NXT_DEF(MVOP_CapInput);
	UADP_SPT_0NXT_DEF(MVOP_ComdFlag);
    //UADP_SPT_0NXT_DEF(MSIF_Version);
    //UADP_SPT_0NXT_DEF(MS_U8);
    UADP_SPT_0NXT_DEF(MS_BOOL);

    UADP_SPT_0NXT_DEF(stMVOP_ENABLE);
    UADP_SPT_0NXT_DEF(stMVOP_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT_DEF(stMVOP_SET_MONO_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_GET_HSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_GET_VSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_GET_HSTART);
    UADP_SPT_0NXT_DEF(stMVOP_GET_VSTART);
    UADP_SPT_0NXT_DEF(stMVOP_GET_IS_INTERLACE);
    UADP_SPT_0NXT_DEF(stMVOP_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT_DEF(stMVOP_CHECK_CAPABILITY);
    UADP_SPT_0NXT_DEF(stMVOP_GET_MAX_HOFFSET);
    UADP_SPT_0NXT_DEF(stMVOP_GET_MAX_VOFFSET);
    UADP_SPT_0NXT_DEF(stMVOP_SET_DBG_LEVEL);
    UADP_SPT_0NXT_DEF(stMVOP_GET_INFO);
    UADP_SPT_0NXT_DEF(stMVOP_SET_CLK);
    UADP_SPT_0NXT_DEF(stMVOP_SET_PATTERN);
    UADP_SPT_0NXT_DEF(stMVOP_SET_TILE_FORMAT);
    UADP_SPT_0NXT_DEF(stMVOP_SET_FIXVTT);
    UADP_SPT_0NXT_DEF(stMVOP_SET_MMIO_MAPBASE);
    UADP_SPT_0NXT_DEF(stMVOP_SET_BASEADD);
    UADP_SPT_0NXT_DEF(stMVOP_SEL_OP_FIELD);
    UADP_SPT_0NXT_DEF(stMVOP_SET_REGSIZE_FROM_MVD);
    UADP_SPT_0NXT_DEF(stMVOP_SET_START_POS);
    UADP_SPT_0NXT_DEF(stMVOP_SET_IMAGE_WIDTH_HIGHT);
    UADP_SPT_0NXT_DEF(stMVOP_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_MIU_SWITCH);
    UADP_SPT_0NXT_DEF(stMVOP_INV_OP_VS);
    UADP_SPT_0NXT_DEF(stMVOP_FORCE_TOP);
    UADP_SPT_0NXT_DEF(stMVOP_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_GET_BASE_ADD);
    UADP_SPT_0NXT_DEF(stMVOP_SEND_BLUE_SCREEN);
    UADP_SPT_0NXT_DEF(stMVOP_SET_FREQUENCY);
    UADP_SPT_0NXT_DEF(stMVOP_ENABLE_INTERRUPT);
    UADP_SPT_0NXT_DEF(stMVOP_GET_INT_STATUS);

    UADP_SPT_0NXT_DEF(stMVOP_SUB_ENABLE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_MONO_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_HSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_VSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_HSTART);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_VSTART);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_IS_INTERLACE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_CHECK_CAPABILITY);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_MAX_HOFFSET);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_MAX_VOFFSET);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_CLK);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_PATTERN);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_TILE_FORMAT);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_FIXVTT);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_MMIO_MAPBASE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_MIU_SWITCH);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_BASEADD);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_BASE_ADD);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_ENABLE_INTERRUPT);
    UADP_SPT_0NXT_DEF(stMVOP_SUB_GET_INT_STATUS);

    UADP_SPT_0NXT_DEF(stMVOP_EX_INIT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_EXIT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_ENABLE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_CLK);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_PATTERN);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_TILE_FORMAT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_ENABLE_BLACK_BG);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_MONO_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_FIXVTT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_MIU_SWITCH);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_SET_CLK);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_HSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_VSIZE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_HSTART);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_VSTART);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_IS_INTERLACE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT_DEF(stMVOP_EX_CHECK_CAPABILITY);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_DST_INFO);
    UADP_SPT_0NXT_DEF(stMVOP_EX_ENABLE_INTERRUPT);
    UADP_SPT_0NXT_DEF(stMVOP_EX_GET_INT_STATUS);
    // 1 pointer
	UADP_SPT_1NXT_DEF(stMVOP_SET_INPUTCFG);
    UADP_SPT_1NXT_DEF(stMVOP_SET_OTPUTCFG);
	UADP_SPT_1NXT_DEF(stMVOP_GET_IS_ENABLE);
	UADP_SPT_1NXT_DEF(stMVOP_GET_OUTPUT_TIMING);
    UADP_SPT_1NXT_DEF(stMVOP_GET_LIBVER_VIR);
	UADP_SPT_1NXT_DEF(stMVOP_GET_LIBVER); //double pointer **
	UADP_SPT_1NXT_DEF(stMVOP_GET_STATUS);
    UADP_SPT_1NXT_DEF(stMVOP_GET_DST_INFO);

    UADP_SPT_1NXT_DEF(stMVOP_SUB_SET_INPUTCFG);
    UADP_SPT_1NXT_DEF(stMVOP_SUB_SET_OTPUTCFG);
    UADP_SPT_1NXT_DEF(stMVOP_SUB_GET_STATUS);
    UADP_SPT_1NXT_DEF(stMVOP_SUB_GET_IS_ENABLE);
    UADP_SPT_1NXT_DEF(stMVOP_SUB_GET_OUTPUT_TIMING);
    UADP_SPT_1NXT_DEF(stMVOP_SUB_GET_DST_INFO);

    UADP_SPT_1NXT_DEF(stMVOP_EX_SET_INPUTCFG);
    UADP_SPT_1NXT_DEF(stMVOP_EX_SET_OTPUTCFG);
    UADP_SPT_1NXT_DEF(stMVOP_EX_GET_OUTPUT_TIMING);
    UADP_SPT_1NXT_DEF(stMVOP_EX_GET_IS_ENABLE);
    UADP_SPT_1NXT_DEF(stMVOP_EX_GET_STATUS);

    // 2 pointers
    UADP_SPT_2NXT_DEF(stMVOP_SET_COMMAND);
    UADP_SPT_2NXT_DEF(stMVOP_GET_COMMAND);

    /* Set Command */
    //0x101
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_MIN_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_MIN);
    //0x102
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR);
    //0x103
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK_U16);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK);
    //0x104
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT);
    //0x105
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_RGB_FMT_RGB);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_RGB_FMT);
    //0x106
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE);
    //0x107
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE_VOID);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE);
    //0x108
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_2ND_CFG_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_2ND_CFG);
    //0x109
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE);
    //0x10a
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS);
    //0x10b
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_FIELD_DUPLICATE_RPTFLD);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_FIELD_DUPLICATE);
    //0x10c
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_VSYNC_MODE_U8);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VSYNC_MODE);
    //0x10d
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE);
    //0x10e
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE);
    //0x10f
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW_ADDIN);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW);
    //0x110
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_FIRE_MVOP_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_FIRE_MVOP);
    //0x111
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_VC1_RANGE_MAP_VC1);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VC1_RANGE_MAP);
    //0x112
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_POWER_STATE_VOID);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_POWER_STATE);
    //0x113
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE);
    //0x114
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_EVD_BASE_ADD_EVDADD);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_EVD_BASE_ADD);
    //0x115
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_EVD_FEATURE_EVDFT);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_EVD_FEATURE);
    //0x116
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT);

    //0x117
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_SETCMD_RESET_SETTING_VOID);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_RESET_SETTING);

    /* Get Command */
    //0x401
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_3DLR_ALT_OUT_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_3DLR_ALT_OUT);
    //0x402
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_MIRROR_MODE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_MIRROR_MODE);
    //0x403
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_3DLR_2ND_CFG_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_3DLR_2ND_CFG);
    //0x404
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE_BOOL);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE);
    //0x405
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_BASE_ADD_BITS_U8);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_BASE_ADD_BITS);
    //0x406
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM_U32);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM);
    //0x407
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_MAXIMUM_CLK_FRE);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_MAXIMUM_CLK);
    //0x408
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_CURRENT_CLK_FRE);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_CURRENT_CLK);
    //0x409
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW_ADD);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW);
    //0x40a
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART_U16);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART);
    //0x40b
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART_U16);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART);
    //0x40c
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_VCOUNT_U16);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_VCOUNT);
    //0x40d
    //UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_HANDSHAKE_MODE_HSMODE);
    //UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_HANDSHAKE_MODE);
    //0x40f,0x410
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_GETCMD_INPUT_SELECT);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_INPUT);
    //0x411,0x412
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_CMD_VDEC_SOURCE);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_VDEC_SOURCE);
    //0x413
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_SETCMD_VDEC_SOURCE);
    //0x414
    UADP_SPT_0NXT_DEF(stTYPE_MVOP_CMD_OUTPUT_CFG);
    UADP_SPT_2NXT_DEF(stTYPE_MVOP_GETCMD_OUTPUT_CFG);

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

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#if 0
typedef struct
{
    const MSIF_Version *pVersion;
} stMVOP_GET_LIBVER_VIR,*pstMVOP_GET_LIBVER_VIR;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 MVOP_adp_Init(FUtopiaIOctl *pIoctl)
{
    // drvMVOP.h
	UADP_SPT_0NXT(MVOP_TileFormat);
	UADP_SPT_0NXT(MVOP_Timing);
	UADP_SPT_0NXT(MVOP_DST_DispInfo);
	UADP_SPT_0NXT(MVOP_TimingInfo_FromRegisters);
	UADP_SPT_0NXT(MVOP_VidStat);
	UADP_SPT_0NXT(MVOP_DrvInfo);
	UADP_SPT_0NXT(MVOP_DrvStatus);
	UADP_SPT_0NXT(MVOP_Handle);
	UADP_SPT_0NXT(MVOP_BaseAddInput);
	UADP_SPT_0NXT(MVOP_VC1RangeMapInfo);
	UADP_SPT_0NXT(MVOP_EVDBaseAddInput);
	UADP_SPT_0NXT(MVOP_EVDFeature);
	UADP_SPT_0NXT(MVOP_CapInput);
	UADP_SPT_0NXT(MVOP_ComdFlag);
    UADP_SPT_0NXT(MVOP_InputCfg);
    //UADP_SPT_0NXT(MSIF_Version);
    UADP_SPT_0NXT(MS_BOOL);

    // drvMVOP_v2.h
    // 0 pointer
    UADP_SPT_0NXT(stMVOP_ENABLE);
    UADP_SPT_0NXT(stMVOP_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT(stMVOP_SET_MONO_MODE);
    UADP_SPT_0NXT(stMVOP_GET_HSIZE);
    UADP_SPT_0NXT(stMVOP_GET_VSIZE);
    UADP_SPT_0NXT(stMVOP_GET_HSTART);
    UADP_SPT_0NXT(stMVOP_GET_VSTART);
    UADP_SPT_0NXT(stMVOP_GET_IS_INTERLACE);
    UADP_SPT_0NXT(stMVOP_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT(stMVOP_CHECK_CAPABILITY);
    UADP_SPT_0NXT(stMVOP_GET_MAX_HOFFSET);
    UADP_SPT_0NXT(stMVOP_GET_MAX_VOFFSET);
    UADP_SPT_0NXT(stMVOP_SET_DBG_LEVEL);
    UADP_SPT_0NXT(stMVOP_GET_INFO);
    UADP_SPT_0NXT(stMVOP_SET_CLK);
    UADP_SPT_0NXT(stMVOP_SET_PATTERN);
    UADP_SPT_0NXT(stMVOP_SET_TILE_FORMAT);
    UADP_SPT_0NXT(stMVOP_SET_FIXVTT);
    UADP_SPT_0NXT(stMVOP_SET_MMIO_MAPBASE);
    UADP_SPT_0NXT(stMVOP_SET_BASEADD);
    UADP_SPT_0NXT(stMVOP_SEL_OP_FIELD);
    UADP_SPT_0NXT(stMVOP_SET_REGSIZE_FROM_MVD);
    UADP_SPT_0NXT(stMVOP_SET_START_POS);
    UADP_SPT_0NXT(stMVOP_SET_IMAGE_WIDTH_HIGHT);
    UADP_SPT_0NXT(stMVOP_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT(stMVOP_MIU_SWITCH);
    UADP_SPT_0NXT(stMVOP_INV_OP_VS);
    UADP_SPT_0NXT(stMVOP_FORCE_TOP);
    UADP_SPT_0NXT(stMVOP_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT(stMVOP_GET_BASE_ADD);
    UADP_SPT_0NXT(stMVOP_SEND_BLUE_SCREEN);
    UADP_SPT_0NXT(stMVOP_SET_FREQUENCY);
    UADP_SPT_0NXT(stMVOP_ENABLE_INTERRUPT);
    UADP_SPT_0NXT(stMVOP_GET_INT_STATUS);

    UADP_SPT_0NXT(stMVOP_SUB_ENABLE);
    UADP_SPT_0NXT(stMVOP_SUB_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT(stMVOP_SUB_SET_MONO_MODE);
    UADP_SPT_0NXT(stMVOP_SUB_GET_HSIZE);
    UADP_SPT_0NXT(stMVOP_SUB_GET_VSIZE);
    UADP_SPT_0NXT(stMVOP_SUB_GET_HSTART);
    UADP_SPT_0NXT(stMVOP_SUB_GET_VSTART);
    UADP_SPT_0NXT(stMVOP_SUB_GET_IS_INTERLACE);
    UADP_SPT_0NXT(stMVOP_SUB_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT(stMVOP_SUB_CHECK_CAPABILITY);
    UADP_SPT_0NXT(stMVOP_SUB_GET_MAX_HOFFSET);
    UADP_SPT_0NXT(stMVOP_SUB_GET_MAX_VOFFSET);
    UADP_SPT_0NXT(stMVOP_SUB_SET_CLK);
    UADP_SPT_0NXT(stMVOP_SUB_SET_PATTERN);
    UADP_SPT_0NXT(stMVOP_SUB_SET_TILE_FORMAT);
    UADP_SPT_0NXT(stMVOP_SUB_SET_FIXVTT);
    UADP_SPT_0NXT(stMVOP_SUB_SET_MMIO_MAPBASE);
    UADP_SPT_0NXT(stMVOP_SUB_MIU_SWITCH);
    UADP_SPT_0NXT(stMVOP_SUB_SET_BASEADD);
    UADP_SPT_0NXT(stMVOP_SUB_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT(stMVOP_SUB_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT(stMVOP_SUB_GET_BASE_ADD);
    UADP_SPT_0NXT(stMVOP_SUB_ENABLE_INTERRUPT);
    UADP_SPT_0NXT(stMVOP_SUB_GET_INT_STATUS);

    UADP_SPT_0NXT(stMVOP_EX_INIT);
    UADP_SPT_0NXT(stMVOP_EX_EXIT);
    UADP_SPT_0NXT(stMVOP_EX_ENABLE);
    UADP_SPT_0NXT(stMVOP_EX_SET_CLK);
    UADP_SPT_0NXT(stMVOP_EX_SET_PATTERN);
    UADP_SPT_0NXT(stMVOP_EX_SET_TILE_FORMAT);
    UADP_SPT_0NXT(stMVOP_EX_ENABLE_UV_SHIFT);
    UADP_SPT_0NXT(stMVOP_EX_ENABLE_BLACK_BG);
    UADP_SPT_0NXT(stMVOP_EX_SET_MONO_MODE);
    UADP_SPT_0NXT(stMVOP_EX_SET_FIXVTT);
    UADP_SPT_0NXT(stMVOP_EX_MIU_SWITCH);
    UADP_SPT_0NXT(stMVOP_EX_SET_VOP_MIRROR_MODE);
    UADP_SPT_0NXT(stMVOP_EX_ENABLE_FREERUN_MODE);
    UADP_SPT_0NXT(stMVOP_EX_SET_CLK);
    UADP_SPT_0NXT(stMVOP_EX_GET_HSIZE);
    UADP_SPT_0NXT(stMVOP_EX_GET_VSIZE);
    UADP_SPT_0NXT(stMVOP_EX_GET_HSTART);
    UADP_SPT_0NXT(stMVOP_EX_GET_VSTART);
    UADP_SPT_0NXT(stMVOP_EX_GET_IS_INTERLACE);
    UADP_SPT_0NXT(stMVOP_EX_GET_IS_HDUPLICATE);
    UADP_SPT_0NXT(stMVOP_EX_CHECK_CAPABILITY);
    UADP_SPT_0NXT(stMVOP_EX_GET_DST_INFO);
    UADP_SPT_0NXT(stMVOP_EX_ENABLE_INTERRUPT);
    UADP_SPT_0NXT(stMVOP_EX_GET_INT_STATUS);
    // 1 pointer
	UADP_SPT_1NXT(stMVOP_SET_INPUTCFG, pCfg, MVOP_InputCfg);
    UADP_SPT_1NXT(stMVOP_SET_OTPUTCFG, pstVideoStatus, MVOP_VidStat);
	UADP_SPT_1NXT(stMVOP_GET_IS_ENABLE, pbEnable, MS_BOOL);
	UADP_SPT_1NXT(stMVOP_GET_OUTPUT_TIMING, pMVOPTiming, MVOP_Timing);
    //UADP_SPT_1NXT(stMVOP_GET_LIBVER_VIR, pVersion, MSIF_Version);
	//UADP_SPT_1NXT(stMVOP_GET_LIBVER, ppVersion, stMVOP_GET_LIBVER_VIR); //double pointer **
	UADP_SPT_1NXT(stMVOP_GET_STATUS, pMVOPStat, MVOP_DrvStatus);
    UADP_SPT_1NXT(stMVOP_GET_DST_INFO, pDstInfo, MVOP_DST_DispInfo);

    UADP_SPT_1NXT(stMVOP_SUB_SET_INPUTCFG, pCfg, MVOP_InputCfg);
    UADP_SPT_1NXT(stMVOP_SUB_SET_OTPUTCFG, pstVideoStatus, MVOP_VidStat);
    UADP_SPT_1NXT(stMVOP_SUB_GET_STATUS, pMVOPStat, MVOP_DrvStatus);
    UADP_SPT_1NXT(stMVOP_SUB_GET_IS_ENABLE, pbEnable, MS_BOOL);
    UADP_SPT_1NXT(stMVOP_SUB_GET_OUTPUT_TIMING, pMVOPTiming, MVOP_Timing);
    UADP_SPT_1NXT(stMVOP_SUB_GET_DST_INFO, pDstInfo, MVOP_DST_DispInfo);

    UADP_SPT_1NXT(stMVOP_EX_SET_INPUTCFG, pCfg, MVOP_InputCfg);
    UADP_SPT_1NXT(stMVOP_EX_SET_OTPUTCFG, pstVideoStatus, MVOP_VidStat);
    UADP_SPT_1NXT(stMVOP_EX_GET_OUTPUT_TIMING, pMVOPTiming, MVOP_Timing);
    UADP_SPT_1NXT(stMVOP_EX_GET_IS_ENABLE, pbEnable, MS_BOOL);
    UADP_SPT_1NXT(stMVOP_EX_GET_STATUS, pMVOPStat, MVOP_DrvStatus);

    // 2 pointers
    //UADP_SPT_2NXT(stMVOP_SET_COMMAND, stHd, MVOP_Handle, pPara, void);
    //UADP_SPT_2NXT(stMVOP_GET_COMMAND, stHd, MVOP_Handle, pPara, void);

    /* Set Command */
    //0x101
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_VSIZE_MIN_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VSIZE_MIN,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_MIN_BOOL);
    //0x102
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR_BOOL);
    //0x103
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK_U16,MS_U16);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK_U16);
    //0x104
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_BOOL);
    //0x105
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_RGB_FMT_RGB,MVOP_RgbFormat);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_RGB_FMT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_RGB_FMT_RGB);
    //0x106
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE_BOOL);
    //0x107
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE_VOID,void); //check
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE_VOID);
    //0x108
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_3DLR_2ND_CFG_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_3DLR_2ND_CFG,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_2ND_CFG_BOOL);
    //0x109
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE_BOOL);
    //0x10a
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS_BOOL);
    //0x10b
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_FIELD_DUPLICATE_RPTFLD,MVOP_RptFldMode);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_FIELD_DUPLICATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_FIELD_DUPLICATE_RPTFLD);
    //0x10c
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_VSYNC_MODE_U8,MS_U8);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VSYNC_MODE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSYNC_MODE_U8);
    //0x10d
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE_BOOL);
    //0x10e
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE_BOOL);
    //0x10f
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW_ADDIN,MVOP_BaseAddInput);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW_ADDIN);
    //0x110
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_FIRE_MVOP_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_FIRE_MVOP,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_FIRE_MVOP_BOOL);
    //0x111
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_VC1_RANGE_MAP_VC1,MVOP_VC1RangeMapInfo);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VC1_RANGE_MAP,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VC1_RANGE_MAP_VC1);
    //0x112
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_POWER_STATE_VOID,void);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_POWER_STATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_POWER_STATE_VOID);
    //0x113
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE_BOOL);
    //0x114
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_EVD_BASE_ADD_EVDADD,MVOP_EVDBaseAddInput);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_EVD_BASE_ADD,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_EVD_BASE_ADD_EVDADD);
    //0x115
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_EVD_FEATURE_EVDFT,MVOP_EVDFeature);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_EVD_FEATURE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_EVD_FEATURE_EVDFT);
    //0x116
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT_BOOL);
    //0x117
    UADP_SPT_NAME0NXT(stTYPE_MVOP_SETCMD_RESET_SETTING_VOID,void);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_RESET_SETTING,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_RESET_SETTING_VOID);


    /* Get Command */
    //0x401
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_3DLR_ALT_OUT_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_3DLR_ALT_OUT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_MIN_BOOL);
    //0x402
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_MIRROR_MODE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_MIRROR_MODE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR_BOOL);
    //0x403
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_3DLR_2ND_CFG_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_3DLR_2ND_CFG,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK_U16);
    //0x404
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE_BOOL,MS_BOOL);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_BOOL);
    //0x405
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_BASE_ADD_BITS_U8,MS_U8);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_BASE_ADD_BITS,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_RGB_FMT_RGB);
    //0x406
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM_U32,MS_U32);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE_BOOL);
    //0x407
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_MAXIMUM_CLK_FRE,MVOP_FREQUENCY); //check
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_MAXIMUM_CLK,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE_VOID);
    //0x408
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_CURRENT_CLK_FRE,MVOP_FREQUENCY);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_CURRENT_CLK,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_2ND_CFG_BOOL);
    //0x409
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW_ADD,MVOP_BaseAddInput);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE_BOOL);
    //0x40a
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART_U16,MS_U16);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS_BOOL);
    //0x40b
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART_U16,MS_U16);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_FIELD_DUPLICATE_RPTFLD);
    //0x40c
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_VCOUNT_U16,MS_U16);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_VCOUNT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSYNC_MODE_U8);

    //0x40f
    #if 0
    UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_INPUT_SELECT,MVOP_InputSel);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_INPUT,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_GETCMD_INPUT_SELECT);

    UADP_SPT_NAME0NXT(stTYPE_MVOP_CMD_VDEC_SOURCE,MVOP_VDEC_Source);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_VDEC_SOURCE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_CMD_VDEC_SOURCE);

    UADP_SPT_NAME2NXT(stTYPE_MVOP_SETCMD_VDEC_SOURCE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_CMD_VDEC_SOURCE);

    UADP_SPT_NAME0NXT(stTYPE_MVOP_CMD_OUTPUT_CFG,MVOP_VidStat);
    UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_OUTPUT_CFG,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_CMD_OUTPUT_CFG);
    #endif
    //0x40d
    //UADP_SPT_NAME0NXT(stTYPE_MVOP_GETCMD_HANDSHAKE_MODE_HSMODE,MVOP_HSMode);
    //UADP_SPT_NAME2NXT(stTYPE_MVOP_GETCMD_HANDSHAKE_MODE,stMVOP_SET_COMMAND,stHd,MVOP_Handle,pPara,stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE_BOOL);

    *pIoctl= (FUtopiaIOctl)MVOP_adp_Ioctl;
	return 0;
}

MS_U32 MVOP_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        // Main MVOP
        // Basic Command
        case E_MVOP_CMD_INIT:
        case E_MVOP_CMD_EXIT:
        case E_MVOP_CMD_ENABLE_BLACK_BG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_ENABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_INPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SET_INPUTCFG, spt_stMVOP_SET_INPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_OTPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SET_OTPUTCFG, spt_stMVOP_SET_OTPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_ENABLE_UV_SHIFT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_ENABLE_UV_SHIFT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_MONO_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SET_MONO_MODE, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        // Get Command
        case E_MVOP_CMD_GET_HSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_HSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_VSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_VSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_HSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_HSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_VSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_VSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_IS_INTERLACE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_IS_INTERLACE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_IS_HDUPLICATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_GET_IS_HDUPLICATE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_IS_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_IS_ENABLE, spt_stMVOP_GET_IS_ENABLE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_OUTPUT_TIMING:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_OUTPUT_TIMING, spt_stMVOP_GET_OUTPUT_TIMING, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_LIBVER:
            break;
        case E_MVOP_CMD_CHECK_CAPABILITY:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_CHECK_CAPABILITY, spt_stMVOP_CHECK_CAPABILITY, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_MAX_HOFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_MAX_HOFFSET, spt_stMVOP_GET_MAX_HOFFSET, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_MAX_VOFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_MAX_VOFFSET, spt_stMVOP_GET_MAX_VOFFSET, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_INFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_INFO, spt_stMVOP_GET_INFO, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_STATUS, spt_stMVOP_GET_STATUS,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_DST_INFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_DST_INFO, spt_stMVOP_GET_DST_INFO, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_BASE_ADD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_BASE_ADD, spt_stMVOP_GET_BASE_ADD, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_GET_INT_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_GET_INT_STATUS, spt_stMVOP_GET_INT_STATUS,buffer_arg,sizeof(buffer_arg));
            break;
        // Set Command
        case E_MVOP_CMD_SET_DBG_LEVEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_DBG_LEVEL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_CLK:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_CLK, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_PATTERN:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_PATTERN, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_TILE_FORMAT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_TILE_FORMAT, spt_stMVOP_SET_TILE_FORMAT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_FIXVTT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_FIXVTT, spt_stMVOP_SET_FIXVTT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_MMIO_MAPBASE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SET_MMIO_MAPBASE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_BASEADD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_BASEADD, spt_stMVOP_SET_BASEADD, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SEL_OP_FIELD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SEL_OP_FIELD, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_REGSIZE_FROM_MVD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_REGSIZE_FROM_MVD, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_START_POS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_START_POS, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_IMAGE_WIDTH_HIGHT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_IMAGE_WIDTH_HIGHT, spt_stMVOP_SET_IMAGE_WIDTH_HIGHT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_VOP_MIRROR_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_VOP_MIRROR_MODE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        // Others
        case E_MVOP_CMD_MIU_SWITCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_MIU_SWITCH, spt_stMVOP_MIU_SWITCH,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_INV_OP_VS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_INV_OP_VS, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_FORCE_TOP:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_FORCE_TOP, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_ENABLE_FREERUN_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_ENABLE_FREERUN_MODE, spt_stMVOP_ENABLE_FREERUN_MODE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SEND_BLUE_SCREEN:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SEND_BLUE_SCREEN, spt_stMVOP_SEND_BLUE_SCREEN, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SET_COMMAND:
        {
            stMVOP_SET_COMMAND ptr;
            if(CPY_FROM_USER(&ptr, (stMVOP_SET_COMMAND __user *)pArgs, sizeof(stMVOP_SET_COMMAND)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            switch(ptr.eCmd)
            {
                case E_MVOP_CMD_SET_VSIZE_MIN:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VSIZE_MIN, spt_stTYPE_MVOP_SETCMD_VSIZE_MIN,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_STB_FD_MASK_CLR:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR, spt_stTYPE_MVOP_SETCMD_STB_FD_MASK_CLR,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_3DLR_INST_VBLANK:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK, spt_stTYPE_MVOP_SETCMD_3DLR_INST_VBLANK,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_3DLR_ALT_OUT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_3DLR_ALT_OUT, spt_stTYPE_MVOP_SETCMD_3DLR_ALT_OUT,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_RGB_FMT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_RGB_FMT, spt_stTYPE_MVOP_SETCMD_RGB_FMT,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_SW_CTRL_FIELD_ENABLE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE, spt_stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_ENABLE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_SW_CTRL_FIELD_DSIABLE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE, spt_stTYPE_MVOP_SETCMD_SW_CTRL_FIELD_DSIABLE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_3DLR_2ND_CFG:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_3DLR_2ND_CFG, spt_stTYPE_MVOP_SETCMD_3DLR_2ND_CFG,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_VSIZE_DUPLICATE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE, spt_stTYPE_MVOP_SETCMD_VSIZE_DUPLICATE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_3DLR_ALT_OUT_SBS:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS, spt_stTYPE_MVOP_SETCMD_3DLR_ALT_OUT_SBS,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_FIELD_DUPLICATE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_FIELD_DUPLICATE, spt_stTYPE_MVOP_SETCMD_FIELD_DUPLICATE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_VSYNC_MODE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VSYNC_MODE, spt_stTYPE_MVOP_SETCMD_VSYNC_MODE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_VSIZE_X4_DUPLICATE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE, spt_stTYPE_MVOP_SETCMD_VSIZE_X4_DUPLICATE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_HSIZE_X4_DUPLICATE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE, spt_stTYPE_MVOP_SETCMD_HSIZE_X4_DUPLICATE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_BASE_ADD_MULTI_VIEW:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW, spt_stTYPE_MVOP_SETCMD_BASE_ADD_MULTI_VIEW,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_FIRE_MVOP:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_FIRE_MVOP, spt_stTYPE_MVOP_SETCMD_FIRE_MVOP,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_VC1_RANGE_MAP:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VC1_RANGE_MAP, spt_stTYPE_MVOP_SETCMD_VC1_RANGE_MAP,buffer_arg,sizeof(buffer_arg));
                    break;
                //case E_MVOP_CMD_SET_POWER_STATE:
                //    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_POWER_STATE, spt_stTYPE_MVOP_SETCMD_POWER_STATE,buffer_arg,sizeof(buffer_arg));
                //    break;
                case E_MVOP_CMD_SET_420_BW_SAVING_MODE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE, spt_stTYPE_MVOP_SETCMD_420_BW_SAVING_MODE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_EVD_BASE_ADD:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_EVD_BASE_ADD, spt_stTYPE_MVOP_SETCMD_EVD_BASE_ADD,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_EVD_FEATURE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_EVD_FEATURE, spt_stTYPE_MVOP_SETCMD_EVD_FEATURE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_MVD_LATE_REPEAT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT, spt_stTYPE_MVOP_SETCMD_MVD_LATE_REPEAT,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_RESET_SETTING:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_RESET_SETTING, spt_stTYPE_MVOP_SETCMD_RESET_SETTING,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_SET_VDEC_SOURCE:
                    #if 1
                    {
                        stMVOP_SET_COMMAND kernelptr;
                        MVOP_Handle stHd;
                        MVOP_VDEC_Source eMVOPResource;
                        CPY_FROM_USER((void*)(&kernelptr), (void*)(&ptr), sizeof(stMVOP_SET_COMMAND));
                        kernelptr.stHd = &stHd;
                        kernelptr.pPara= &eMVOPResource;
                        kernelptr.eCmd = ptr.eCmd;
                        CPY_FROM_USER((void*)(kernelptr.stHd), (void*)(ptr.stHd),sizeof(MVOP_Handle));
                        CPY_FROM_USER((void*)(kernelptr.pPara), (void*)(ptr.pPara),sizeof(MVOP_VDEC_Source));
                        u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&kernelptr);
                        ptr.eRet = kernelptr.eRet;
                    }
                    #else
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_SETCMD_VDEC_SOURCE, spt_stTYPE_MVOP_SETCMD_VDEC_SOURCE,buffer_arg,sizeof(buffer_arg));
                    #endif
                    break;
                default:
                        break;
            }
        }
            break;
        case E_MVOP_CMD_GET_COMMAND:
        {
            stMVOP_GET_COMMAND ptr;
            if(CPY_FROM_USER(&ptr, (stMVOP_GET_COMMAND __user *)pArgs, sizeof(stMVOP_GET_COMMAND)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            switch(ptr.eCmd)
            {
                case E_MVOP_CMD_GET_3DLR_ALT_OUT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_3DLR_ALT_OUT, spt_stTYPE_MVOP_GETCMD_3DLR_ALT_OUT,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_MIRROR_MODE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_MIRROR_MODE, spt_stTYPE_MVOP_GETCMD_MIRROR_MODE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_3DLR_2ND_CFG:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_3DLR_2ND_CFG, spt_stTYPE_MVOP_GETCMD_3DLR_2ND_CFG,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_VSIZE_DUPLICATE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE, spt_stTYPE_MVOP_GETCMD_VSIZE_DUPLICATE,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_BASE_ADD_BITS:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_BASE_ADD_BITS, spt_stTYPE_MVOP_GETCMD_BASE_ADD_BITS,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_TOTAL_MVOP_NUM:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM, spt_stTYPE_MVOP_GETCMD_TOTAL_MVOP_NUM,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_MAXIMUM_CLK:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_MAXIMUM_CLK, spt_stTYPE_MVOP_GETCMD_MAXIMUM_CLK,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_CURRENT_CLK:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_CURRENT_CLK, spt_stTYPE_MVOP_GETCMD_CURRENT_CLK,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_BASE_ADD_MULTI_VIEW:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW, spt_stTYPE_MVOP_GETCMD_BASE_ADD_MULTI_VIEW,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_TOP_FIELD_IMAGE_VSTART:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART, spt_stTYPE_MVOP_GETCMD_TOP_FIELD_IMAGE_VSTART,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_BOTTOM_FIELD_IMAGE_VSTART:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART, spt_stTYPE_MVOP_GETCMD_BOTTOM_FIELD_IMAGE_VSTART,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_VCOUNT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_VCOUNT, spt_stTYPE_MVOP_GETCMD_VCOUNT,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_MVOP_CMD_GET_INPUT:
                case E_MVOP_CMD_GET_SUB_INPUT:
                    #if 1
                    {
                        stMVOP_GET_COMMAND kernelptr;
                        MVOP_Handle stHd;
                        MVOP_InputSel eInputSel;
                        CPY_FROM_USER((void*)(&kernelptr), (void*)(&ptr), sizeof(stMVOP_GET_COMMAND));
                        kernelptr.stHd = &stHd;
                        kernelptr.pPara= &eInputSel;
                        kernelptr.eCmd = ptr.eCmd;
                        CPY_FROM_USER((void*)(kernelptr.stHd), (void*)(ptr.stHd),sizeof(MVOP_Handle));
                         u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&kernelptr);
                        CPY_to_USER((void*)(ptr.pPara), (void*)(kernelptr.pPara), ptr.u32ParaSize);
                        ptr.eRet = kernelptr.eRet;
                    }
                    #else
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_INPUT, spt_stTYPE_MVOP_GETCMD_INPUT,buffer_arg,sizeof(buffer_arg));
                    #endif
                    break;
                case E_MVOP_CMD_GET_VDEC_SOURCE:
                    #if 1
                    {
                        stMVOP_GET_COMMAND kernelptr;
                        MVOP_Handle stHd;
                        MVOP_VDEC_Source eMVOPResource;
                        CPY_FROM_USER((void*)(&kernelptr), (void*)(&ptr), sizeof(stMVOP_GET_COMMAND));
                        kernelptr.stHd = &stHd;
                        kernelptr.pPara= &eMVOPResource;
                        kernelptr.eCmd = ptr.eCmd;
                        CPY_FROM_USER((void*)(kernelptr.stHd), (void*)(ptr.stHd),sizeof(MVOP_Handle));
                        u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&kernelptr);
                        CPY_to_USER((void*)(ptr.pPara), (void*)(kernelptr.pPara), ptr.u32ParaSize);
                        ptr.eRet = kernelptr.eRet;
                    }
                    #else
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_VDEC_SOURCE, spt_stTYPE_MVOP_GETCMD_VDEC_SOURCE,buffer_arg,sizeof(buffer_arg));                    
                    #endif
                    break;
                case E_MVOP_CMD_GET_OUTPUT_CFG:
                    #if 1
                    {
                        stMVOP_GET_COMMAND kernelptr;
                        MVOP_Handle stHd;
                        MVOP_VidStat stMVOPCfg;
                        CPY_FROM_USER((void*)(&kernelptr), (void*)(&ptr), sizeof(stMVOP_GET_COMMAND));
                        kernelptr.stHd = &stHd;
                        kernelptr.pPara= &stMVOPCfg;
                        kernelptr.eCmd = ptr.eCmd;
                        CPY_FROM_USER((void*)(kernelptr.stHd), (void*)(ptr.stHd),sizeof(MVOP_Handle));
                        u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&kernelptr);
                        CPY_to_USER((void*)(ptr.pPara), (void*)(kernelptr.pPara), ptr.u32ParaSize);
                        ptr.eRet = kernelptr.eRet;
                    }
                    #else
                    u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stTYPE_MVOP_GETCMD_OUTPUT_CFG, spt_stTYPE_MVOP_GETCMD_OUTPUT_CFG,buffer_arg,sizeof(buffer_arg));
                    #endif
                    break;
                //case E_MVOP_CMD_GET_HANDSHAKE_MODE:
                    //u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, stTYPE_MVOP_GETCMD_HANDSHAKE_MODE, stTYPE_MVOP_GETCMD_HANDSHAKE_MODE,buffer_arg,sizeof(buffer_arg));
                    //break;
                default:
                        break;
            }
        }
                break;
        case E_MVOP_CMD_SET_FREQUENCY:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SET_FREQUENCY, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_ENABLE_INTERRUPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_ENABLE_INTERRUPT, spt_stMVOP_ENABLE_INTERRUPT,buffer_arg,sizeof(buffer_arg));
            break;
        // Sub MVOP
        // Basic Command
        case E_MVOP_CMD_SUB_INIT:
        case E_MVOP_CMD_SUB_EXIT:
        case E_MVOP_CMD_SUB_ENABLE_BLACK_BG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SUB_ENABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_INPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SUB_SET_INPUTCFG, spt_stMVOP_SET_INPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_OTPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SUB_SET_OTPUTCFG, spt_stMVOP_SET_OTPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_ENABLE_UV_SHIFT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SUB_ENABLE_UV_SHIFT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_MONO_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_SUB_SET_MONO_MODE, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        // Get Command
        case E_MVOP_CMD_SUB_GET_HSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_HSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_VSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_VSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_HSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_HSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_VSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_VSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_IS_INTERLACE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_IS_INTERLACE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_IS_HDUPLICATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_IS_HDUPLICATE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_IS_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_GET_IS_ENABLE, spt_stMVOP_SUB_GET_IS_ENABLE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_OUTPUT_TIMING:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_GET_OUTPUT_TIMING, spt_stMVOP_SUB_GET_OUTPUT_TIMING, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_CHECK_CAPABILITY:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_CHECK_CAPABILITY, spt_stMVOP_SUB_CHECK_CAPABILITY, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_MAXHOFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_GET_MAX_HOFFSET, spt_stMVOP_SUB_GET_MAX_HOFFSET, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_MAXVOFFSET:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_GET_MAX_VOFFSET, spt_stMVOP_SUB_GET_MAX_VOFFSET, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_DST_INFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_GET_DST_INFO, spt_stMVOP_SUB_GET_DST_INFO, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_BASEADD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_BASEADD, spt_stMVOP_SUB_SET_BASEADD, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_GET_INT_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_GET_INT_STATUS, buffer_arg,sizeof(buffer_arg));
            break;
        // Set Command
        case E_MVOP_CMD_SUB_SET_CLK:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_CLK, spt_stMVOP_SUB_SET_CLK, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_PATTERN:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_PATTERN, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_TILE_FORMAT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_TILE_FORMAT, spt_stMVOP_SUB_SET_TILE_FORMAT,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_FIXVTT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_FIXVTT, spt_stMVOP_SUB_SET_FIXVTT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_MMIO_MAPBASE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stMVOP_SUB_SET_MMIO_MAPBASE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_BASEADD:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_BASEADD, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_SET_VOP_MIRRORMODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_SET_VOP_MIRROR_MODE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        // Others
        case E_MVOP_CMD_SUB_MIU_SWITCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_MIU_SWITCH, spt_stMVOP_SUB_MIU_SWITCH,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_ENABLE_FREERUN_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_ENABLE_FREERUN_MODE, spt_stMVOP_SUB_ENABLE_FREERUN_MODE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_SUB_ENABLE_INTERRUPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_SUB_ENABLE_INTERRUPT, spt_stMVOP_SUB_ENABLE_INTERRUPT,buffer_arg,sizeof(buffer_arg));
            break;
        // 3rd MVOP
        // Basic Command
        case E_MVOP_CMD_EX_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_INIT, spt_stMVOP_EX_INIT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_EXIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_EXIT, spt_stMVOP_EX_EXIT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_EX_ENABLE, spt_stMVOP_EX_ENABLE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_INPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_EX_SET_INPUTCFG, spt_stMVOP_EX_SET_INPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_OTPUTCFG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_EX_SET_OTPUTCFG, spt_stMVOP_EX_SET_OTPUTCFG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_ENABLE_BLACK_BG:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_ENABLE_BLACK_BG, spt_stMVOP_EX_ENABLE_BLACK_BG, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_ENABLE_UV_SHIFT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_EX_ENABLE_UV_SHIFT, spt_stMVOP_EX_ENABLE_UV_SHIFT,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_MONO_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stMVOP_EX_SET_MONO_MODE, spt_stMVOP_EX_SET_MONO_MODE,buffer_arg,sizeof(buffer_arg));
            break;
        // Get Command
        case E_MVOP_CMD_EX_GET_HSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_HSIZE, spt_stMVOP_EX_GET_HSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_VSIZE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_VSIZE, spt_stMVOP_EX_GET_VSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_HSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_HSTART, spt_stMVOP_EX_GET_HSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_VSTART:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_VSTART, spt_stMVOP_EX_GET_VSTART, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_IS_INTERLACE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_IS_INTERLACE, spt_stMVOP_EX_GET_IS_INTERLACE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_IS_HDUPLICATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_IS_HDUPLICATE, spt_stMVOP_EX_GET_IS_HDUPLICATE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_STATUS, spt_stMVOP_EX_GET_STATUS,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_CHECK_CAPABILITY:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_CHECK_CAPABILITY, spt_stMVOP_EX_CHECK_CAPABILITY, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_DST_INFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_DST_INFO, spt_stMVOP_EX_GET_DST_INFO, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_OUTPUT_TIMING:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_OUTPUT_TIMING, spt_stMVOP_EX_GET_OUTPUT_TIMING, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_IS_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_IS_ENABLE, spt_stMVOP_EX_GET_IS_ENABLE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_GET_INT_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_GET_INT_STATUS, spt_stMVOP_EX_GET_INT_STATUS, buffer_arg,sizeof(buffer_arg));
            break;
        // Set Command
        case E_MVOP_CMD_EX_SET_CLK:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_SET_CLK, spt_stMVOP_EX_SET_CLK, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_PATTERN:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_SET_PATTERN, spt_stMVOP_EX_SET_PATTERN, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_TILEFORMAT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_SET_TILE_FORMAT, spt_stMVOP_EX_SET_TILE_FORMAT,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_FIXVTT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_SET_FIXVTT, spt_stMVOP_EX_SET_FIXVTT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_SET_VOP_MIRRORMODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_SET_VOP_MIRROR_MODE, spt_stMVOP_EX_SET_VOP_MIRROR_MODE,buffer_arg,sizeof(buffer_arg));
            break;
        // Others
        case E_MVOP_CMD_EX_MIU_SWITCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_MIU_SWITCH, spt_stMVOP_EX_MIU_SWITCH,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_ENABLE_FREERUN_MODE:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_ENABLE_FREERUN_MODE, spt_stMVOP_EX_ENABLE_FREERUN_MODE,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MVOP_CMD_EX_ENABLE_INTERRUPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stMVOP_EX_ENABLE_INTERRUPT, spt_stMVOP_EX_ENABLE_INTERRUPT,buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;
    }
    return u32Ret;
}

#endif


