/* ****************************************************************************************
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 1998-2010 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * *****************************************************************************************/

/**
 * @file
 *
 * HAL driver implementation for VDC device of VDEC.
 * VDEC device will teach you how to make device driver with new platform.
 *
 * author     youngwoo.jin@lge.com
 * version    0.1
 * date       2013.05.14
 * note       Additional information.
 *
 * @addtogroup d13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "hal/vdec_base.h"
#include "hal/vdc_hal_api.h"
#include "hal/top_hal_api.h"
#include "hal/h15/a0/g1_top_reg.h"

#define LOG_NAME vdec_hal_vdc
#include "log.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define _GET_INT_SRC(_src)																\
	( (_src < VDC_HAL_INT_SRC_RESET_BASE)?												\
		(_src - VDC_HAL_INT_SRC_BASE + MACH0) :											\
		(_src - VDC_HAL_INT_SRC_RESET_BASE + MACH0_SRST_DONE) )

#define _GET_INT_SRC_MASK(_src)															\
	( (_src & ((1 << VDC_HAL_INT_SRC_RESET_BASE) - 1)) |								\
	  ((_src >> VDC_HAL_INT_SRC_RESET_BASE) << (MACH0_SRST_DONE)) )

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static volatile G1_TOP_REG_T        *stp_vdec1_top_reg = NULL;
static volatile G1_TOP_REG_T        *stp_vdec2_top_reg = NULL;
static volatile G1_TOP_REG_T        *stp_vdec1_top_shadow[1];
static volatile G1_TOP_REG_T        *stp_vdec2_top_shadow[1];
static spinlock_t                   gst_g1_isr_lock;
static int                          dec_owner = VDC_HAL_RET_OK;

/*========================================================================================
    Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Init(void)
{
    stp_vdec1_top_reg = (volatile G1_TOP_REG_T *)ioremap(VDEC1_REG_TOP_BASE, 0x40);
    stp_vdec2_top_reg = (volatile G1_TOP_REG_T *)ioremap(VDEC2_REG_TOP_BASE, 0x40);

    spin_lock_init (&gst_g1_isr_lock);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Suspend(void)
{
    log_noti ("suspend...\n");
    memcpy ((void*)stp_vdec1_top_shadow, (void*)stp_vdec1_top_reg,
            sizeof (stp_vdec1_top_shadow));
    memcpy ((void*)stp_vdec2_top_shadow, (void*)stp_vdec2_top_reg,
            sizeof (stp_vdec2_top_shadow));
    log_noti ("suspend... done. \n");
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_Resume(void)
{
    log_noti ("resume...\n");
    memcpy ((void*)stp_vdec1_top_reg, (void*)stp_vdec1_top_shadow,
            sizeof (stp_vdec1_top_shadow));
    memcpy ((void*)stp_vdec2_top_reg, (void*)stp_vdec2_top_shadow,
            sizeof (stp_vdec2_top_shadow));
    log_noti ("resume... done. \n");
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetVersion(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetVersion();
}
EXPORT_SYMBOL(VDC_HAL_GetVersion);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_SetApbMachOffset(UINT8 ui8CoreNum, UINT8 ui8Offset)
{
	return TOP_HAL_SetApbMachOffset(ui8CoreNum, ui8Offset);
}
EXPORT_SYMBOL(VDC_HAL_SetApbMachOffset);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_SetApbVDO(UINT8 ui8CoreNum, UINT8 ui8GdiSelection, UINT8 ui8Offset)
{
	return TOP_HAL_SetApbVDO(ui8GdiSelection, ui8Offset);
}
EXPORT_SYMBOL(VDC_HAL_SetApbVDO);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ResetMach(UINT8 ui8CoreNum, UINT8 ui8SubIdx)
{
	TOP_HAL_ResetMach(ui8SubIdx);
}
EXPORT_SYMBOL(VDC_HAL_ResetMach);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_SetMachIntrMode(UINT8 ui8CoreNum, UINT8 IntrMode)
{
	TOP_HAL_SetMachIntrMode(ui8CoreNum, IntrMode);
}
EXPORT_SYMBOL(VDC_HAL_SetMachIntrMode);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachIntrMode(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachIntrMode(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachIntrMode);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachIdleStatus(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachIdleStatus(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachIdleStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT8	VDC_HAL_GetMachUnderRunStatus(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetMachUnderRunStatus(ui8CoreNum);
}
EXPORT_SYMBOL(VDC_HAL_GetMachUnderRunStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_EnableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
    volatile G1_TOP_REG_T *g1_top_reg = NULL;
    unsigned long flags;

    switch (ui8CoreNum)
    {
        case VDEC_CORE_NUM_CNM:
        case VDEC_CORE_NUM_G2:
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
            TOP_HAL_EnableIntr(ui8IntrSrc);
            break;
        case VDEC_CORE_NUM_G1:
            spin_lock_irqsave (&gst_g1_isr_lock,flags);
            if (ui8IntrSrc == 0) /* g1 case, intrsrc is core number */
            {
                g1_top_reg = stp_vdec1_top_reg;
            }
            else if (ui8IntrSrc == 1)
            {
                g1_top_reg = stp_vdec2_top_reg;
            }
            else
            {
                log_error ("not existed core \n");
                spin_unlock_irqrestore (&gst_g1_isr_lock,flags);
                return;
            }
            g1_top_reg->intr_mcu_en.g1_intr = 1;
            spin_unlock_irqrestore (&gst_g1_isr_lock,flags);
            break;
        default:
            break;
    }
}
EXPORT_SYMBOL(VDC_HAL_EnableInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
BOOLEAN	VDC_HAL_DisableInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
    volatile G1_TOP_REG_T *g1_top_reg = NULL;
    unsigned long flags;

    switch (ui8CoreNum)
    {
        case VDEC_CORE_NUM_CNM:
        case VDEC_CORE_NUM_G2:
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
            return TOP_HAL_DisableIntr(ui8IntrSrc);
        case VDEC_CORE_NUM_G1:
            spin_lock_irqsave (&gst_g1_isr_lock,flags);
            if (ui8IntrSrc == 0)
            {
                g1_top_reg = stp_vdec1_top_reg;
            }
            else if (ui8IntrSrc == 1)
            {
                g1_top_reg = stp_vdec2_top_reg;
            }
            else
            {
                log_error ("not existed core \n");
                spin_unlock_irqrestore (&gst_g1_isr_lock,flags);
                return false;
            }
            g1_top_reg->intr_mcu_en.g1_intr = 0;
            spin_unlock_irqrestore (&gst_g1_isr_lock,flags);
            return true;
        default:
            break;
    }

    return false;
}
EXPORT_SYMBOL(VDC_HAL_DisableInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ClearInterrupt(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
    volatile G1_TOP_REG_T *g1_top_reg = NULL;
    unsigned long flags;

    switch (ui8CoreNum)
    {
        case VDEC_CORE_NUM_CNM:
        case VDEC_CORE_NUM_G2:
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
            TOP_HAL_ClearIntr(ui8IntrSrc);
            break;
        case VDEC_CORE_NUM_G1:
            spin_lock_irqsave (&gst_g1_isr_lock, flags);
            if (ui8IntrSrc == 0) /* g1 case, intrsrc is core number */
            {
                g1_top_reg = stp_vdec1_top_reg;
            }
            else if (ui8IntrSrc == 1)
            {
                g1_top_reg = stp_vdec2_top_reg;
            }
            else
            {
                log_error ("not existed core \n");
                spin_unlock_irqrestore (&gst_g1_isr_lock,flags);
                return;
            }
            g1_top_reg->intr_mcu_cl.g1_intr = 1;
            spin_unlock_irqrestore (&gst_g1_isr_lock, flags);
            break;
        default:
            break;
    }
}
EXPORT_SYMBOL(VDC_HAL_ClearInterrupt);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_ClearInterruptMask(UINT8 ui8CoreNum, UINT32 ui32IntrMsk)
{
	ui32IntrMsk = _GET_INT_SRC_MASK(ui32IntrMsk);
	TOP_HAL_ClearIntrMsk(ui32IntrMsk);
}
EXPORT_SYMBOL(VDC_HAL_ClearInterruptMask);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_DisableInterruptAll(UINT8 ui8CoreNum)
{
	TOP_HAL_DisableIntrAll();
}
EXPORT_SYMBOL(VDC_HAL_DisableInterruptAll);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
BOOLEAN	VDC_HAL_IsInterruptEnable(UINT8 ui8CoreNum, UINT8 ui8IntrSrc)
{
	ui8IntrSrc = _GET_INT_SRC(ui8IntrSrc);
	return TOP_HAL_IsIntrEnable(ui8IntrSrc);
}
EXPORT_SYMBOL(VDC_HAL_IsInterruptEnable);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetInterruptStatus(UINT8 ui8CoreNum)
{
	UINT32	ui32RetVal = 0x00;
	UINT32	ui32Status = TOP_HAL_GetIntrStatus();

    switch ( ui8CoreNum )
    {
        case VDEC_CORE_NUM_CNM:
	if ( ui32Status & (1 << MACH0) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_BASE + 0));
	}
            if ( ui32Status & (1 << MACH0_SRST_DONE) ) {
                ui32RetVal |= (1 << (VDC_HAL_INT_SRC_RESET_BASE + 0));
            }
            break;
        case VDEC_CORE_NUM_G2:
	if ( ui32Status & (1 << MACH1) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_BASE + 1));
	}
	if ( ui32Status & (1 << MACH1_SRST_DONE) ) {
		ui32RetVal |= (1 << (VDC_HAL_INT_SRC_RESET_BASE + 1));
	}
            break;
        default:
            break;
    }

	return ui32RetVal;
}
EXPORT_SYMBOL(VDC_HAL_GetInterruptStatus);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_EnableFeedTimer(UINT8 ui8CoreNum)
{
	TOP_HAL_EnableFeedTimer();
}
EXPORT_SYMBOL(VDC_HAL_EnableFeedTimer);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_DisableFeedTimer(UINT8 ui8CoreNum)
{
	TOP_HAL_DisableFeedTimer();
}
EXPORT_SYMBOL(VDC_HAL_DisableFeedTimer);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	VDC_HAL_SetFeedTimerValue(UINT8 ui8CoreNum, UINT32 ui32Value)
{
	TOP_HAL_SetFeedTimerValue(ui32Value);
}
EXPORT_SYMBOL(VDC_HAL_SetFeedTimerValue);

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
UINT32	VDC_HAL_GetFeedTimerValue(UINT8 ui8CoreNum)
{
	return TOP_HAL_GetFeedTimerValue();
}
EXPORT_SYMBOL(VDC_HAL_GetFeedTimerValue);

/**
 ********************************************************************************
 * @brief
 *
 * @remarks
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_HAL_RET_T VDC_HAL_ExtraCmd(UINT8 ui8CoreNum, VDC_HAL_CMD_T eCmd)
{
    switch (eCmd)
    {
        case VDC_HAL_CMD_SET_DEC_MODE:
            if (ui8CoreNum == VDEC_CORE_NUM_G1)
            {
                /* g1 dec mode set */
                stp_vdec1_top_reg->dec_mode.reg_dec_mode = 0;
                stp_vdec2_top_reg->dec_mode.reg_dec_mode = 0;
                dec_owner = VDC_HAL_RET_G1_OCCUPIED;
            }
            else if (ui8CoreNum == VDEC_CORE_NUM_HEVC)
            {
                /* hevc dec mode set */
                stp_vdec1_top_reg->dec_mode.reg_dec_mode = 1;
                stp_vdec2_top_reg->dec_mode.reg_dec_mode = 1;
                dec_owner = VDC_HAL_RET_HEVC_OCCUPIED;
            }
            break;
        case VDC_HAL_CMD_RETURN_OCCUPANCY:
            dec_owner = VDC_HAL_RET_OK;
            break;
        case VDC_HAL_CMD_GET_DEC_MODE:
            return dec_owner;
        default:
            break;
    }

    return VDC_HAL_RET_OK;
}
EXPORT_SYMBOL(VDC_HAL_ExtraCmd);

/*========================================================================================
    Static Implementation Group
========================================================================================*/

/** @} */

