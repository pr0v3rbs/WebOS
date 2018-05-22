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
/// @file   mdrv_vif_io.h
/// @brief  VIF Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_VIF_IO_H_
#define _MDRV_VIF_IO_H_

//#include <asm-mips/types.h>
#include "mdrv_types.h"
#include "mdrv_vif_st.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define VIF_IOC_MAGIC                'v'

#define MDRV_VIF_INIT                       _IOR(VIF_IOC_MAGIC, 0, VIF_Init_Info_st)
#define MDRV_VIF_SETCLOCK                   _IOR(VIF_IOC_MAGIC, 1, BOOL)
#define MDRV_VIF_RESET                       _IO(VIF_IOC_MAGIC, 2)
#define MDRV_VIF_HANDLER                    _IOR(VIF_IOC_MAGIC, 3, BOOL)
#define MDRV_VIF_SET_SOUND_SYSTEM           _IOR(VIF_IOC_MAGIC, 4, VIFSoundSystem)
#define MDRV_VIF_SET_IF_FREQ                _IOR(VIF_IOC_MAGIC, 5, IfFrequencyType)
#define MDRV_VIF_READ_CR_FOE                _IOW(VIF_IOC_MAGIC, 6, U8)
#define MDRV_VIF_READ_CR_LOCK_STATUS        _IOW(VIF_IOC_MAGIC, 7, U8)
#define MDRV_VIF_BYPASS_DBB_AUDIO_FLT      _IOR(VIF_IOC_MAGIC, 8, BOOL)
#define MDRV_VIF_SET_FREQ_BAND              _IOR(VIF_IOC_MAGIC, 9, FrequencyBand)
#define MDRV_VIF_GET_INPUT_LEVEL_INDICATOR  _IOW(VIF_IOC_MAGIC, 10, BOOL)
#define MDRV_VIF_SET_PARAMETER             _IOWR(VIF_IOC_MAGIC, 11, VIF_Para_Info_st)
#define MDRV_VIF_SHIFTCLK               _IOR(VIF_IOC_MAGIC, 12, BYTE)
#define VIF_IOC_MAXNR                   13


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    int                         s32VIFMajor;
    int                         s32VIFMinor;
    struct cdev                 cDevice;
    struct file_operations      VIFFop;
    struct fasync_struct        *async_queue; /* asynchronous readers */
} VIF_ModHandle_t;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#endif // _MDRV_VIF_IO_H_
