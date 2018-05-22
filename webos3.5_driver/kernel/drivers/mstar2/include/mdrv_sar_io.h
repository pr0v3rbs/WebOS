////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_sar_io.h
/// @brief  SAR Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SAR_IO_H_
#define _MDRV_SAR_IO_H_

#include "mdrv_types.h"
#include "mdrv_sar_st.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define SAR_IOC_MAGIC                's'

#define MDRV_SAR_INIT                    _IO(SAR_IOC_MAGIC, 0)
#define MDRV_SAR_CH_INFO                _IOWR(SAR_IOC_MAGIC, 1, SAR_CFG_t)
#define MDRV_SAR_CH_GET_KEY             _IOWR(SAR_IOC_MAGIC, 2, SAR_Key_t)
#define MDRV_SAR_GET_KEY_CODE          _IOWR(SAR_IOC_MAGIC, 3, SAR_Key_t)
#define MDRV_SAR_GET_ADC_VALUE          _IOWR(SAR_IOC_MAGIC, 4, SAR_Key_t)		// GP2R_20101026_jy.song : Touch Key HW Revison ReadÀ§ÇÑ ADC Port Control
#define MDrv_SAR_SET_SCAN_MODE			_IOW(SAR_IOC_MAGIC, 5, U8)
#define MDrv_SAR_GET_SCAN_DATA			_IOR(SAR_IOC_MAGIC, 6,U8)
#define MDrv_SAR_SET_PDP_ERROR			_IO(SAR_IOC_MAGIC, 7)


#define SAR_IOC_MAXNR                   7

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    int                         s32SARMajor;
    int                         s32SARMinor;
    struct cdev                 cDevice;
    struct file_operations      SARFop;
    struct fasync_struct        *async_queue; /* asynchronous readers */
} SAR_ModHandle_t;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#endif // _MDRV_SAR_IO_H_
