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
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/io.h>

#include "vdec_type_defs.h"
#include "hal/vdec_base.h"
#include "vd2_rc0_reg.h"
#include "vd2_rc1_reg.h"
#include "hal/vdc_hal_api.h"
#include "mcu_base.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define VD2_RC0_BASE		(VDEC_BASE + 0x3C000)
#define VD2_RC1_BASE		(VDEC_BASE + 0x3E000)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

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
static volatile VD2_RC0_REG_T	*pstVD2RC0_Reg = NULL;
static volatile VD2_RC1_REG_T	*pstVD2RC1_Reg = NULL;
static volatile VD2_RC0_REG_T	stVD2RC0_RegShadow[1];
static volatile VD2_RC1_REG_T	stVD2RC1_RegShadow[1];
static spinlock_t				_gstApbConfLock;

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
	pstVD2RC0_Reg = (volatile VD2_RC0_REG_T *)ioremap(VD2_RC0_BASE, 0x2000);
	pstVD2RC1_Reg = (volatile VD2_RC1_REG_T *)ioremap(VD2_RC1_BASE, 0x2000);
	spin_lock_init(&_gstApbConfLock);

	SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE)
				| HEVC0_HW_INT_EN
				| HEVC1_HW_INT_EN
				| H264_HW_INT_EN
				));

	SetReg(MCU1_BASE + 0x90, 0x40040000);
	SetReg(MCU1_BASE + 0x9c, 0x40040000);

	VDC_HAL_ResetMach(VDEC_CORE_NUM_CNM, 0);
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
}
EXPORT_SYMBOL(VDC_HAL_Suspend);

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
}
EXPORT_SYMBOL(VDC_HAL_Resume);

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
	UINT32 ui32Version = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(version);
		VD2_RC0_Rd01(version, yyyymmdd, ui32Version);
		break;
	default:
		break;
	}

	return ui32Version;
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
	UINT32	u32RetVal = -1;
	ULONG	ulFlags;

	spin_lock_irqsave(&_gstApbConfLock, ulFlags);

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(apb_conf);
		VD2_RC0_Rd01(apb_conf, reg_apb_mach0_offset, u32RetVal);
		VD2_RC0_Wr01(apb_conf, reg_apb_mach0_offset, ui8Offset);
		VD2_RC0_WrFL(apb_conf);
		break;
	default:
		break;
	}

	spin_unlock_irqrestore(&_gstApbConfLock, ulFlags);

	return u32RetVal;
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
	UINT32 u32RetVal = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		break;
	default:
		break;
	}

	return u32RetVal;
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(sw_reset);
		VD2_RC0_Wr01(sw_reset, sw_reset_wave320, 1);
		VD2_RC0_WrFL(sw_reset);
		break;
	default:
		break;
	}
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(mach_conf);
		VD2_RC0_Wr01(mach_conf, reg_core0_intr_mode, (IntrMode & 0x1));
		VD2_RC0_WrFL(mach_conf);
		break;
	default:
		break;
	}
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
	UINT8 ui8IntrMode = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(mach_conf);
		VD2_RC0_Rd01(mach_conf, reg_core0_intr_mode, ui8IntrMode);
		break;
	default:
		break;
	}

	return ui8IntrMode;
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
	UINT8 ui8IdleStatus = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(mach_conf);
		VD2_RC0_Rd01(mach_conf, reg_core0_vpu_idle, ui8IdleStatus);
		break;
	default:
		break;
	}

	return ui8IdleStatus;
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
	UINT8 ui8UnderRun = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(mach_conf);
		VD2_RC0_Rd01(mach_conf, reg_core0_vpu_underrun, ui8UnderRun);
		break;
	default:
		break;
	}

	return ui8UnderRun;
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
	unsigned long	flags;

	spin_lock_irqsave(&_gstApbConfLock, flags);

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		if( ui8IntrSrc == 0 )
		{
			SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) | HEVC0_HW_INT_EN));
		}
		else if( ui8IntrSrc == 1 )
		{
			SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) | HEVC1_HW_INT_EN));
		}
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(intr_i_en);
		VD2_RC0_Wr(intr_i_en, (VD2_RC0_Rd(intr_i_en) | (1 << ui8IntrSrc)));
		VD2_RC0_WrFL(intr_i_en);
		SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) | H264_HW_INT_EN));
		break;
	default:
		break;
	}

	spin_unlock_irqrestore(&_gstApbConfLock, flags);
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
	BOOLEAN			bRet = FALSE;
	unsigned long 	flags;

	spin_lock_irqsave(&_gstApbConfLock, flags);

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		if( ui8IntrSrc == 0 )
		{
			SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << HEVC0_HW_INT_EN)));
		}
		else if( ui8IntrSrc == 1 )
		{
			SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << HEVC1_HW_INT_EN)));
		}
		break;
	case VDEC_CORE_NUM_CNM:
		SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << H264_HW_INT_EN)));
		VD2_RC0_RdFL(intr_i_en);
		bRet = !!(VD2_RC0_Rd(intr_i_en) & (1 << ui8IntrSrc));
		VD2_RC0_Wr(intr_i_en, (VD2_RC0_Rd(intr_i_en) & ~(1 << ui8IntrSrc)));
		VD2_RC0_WrFL(intr_i_en);
		break;
	default:
		break;
	}

	spin_unlock_irqrestore(&_gstApbConfLock, flags);

	return bRet;
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(intr_i_cl);
		VD2_RC0_Wr(intr_i_cl, (1 << ui8IntrSrc));
		VD2_RC0_WrFL(intr_i_cl);
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(intr_i_cl);
		VD2_RC0_Wr(intr_i_cl, ui32IntrMsk);
		VD2_RC0_WrFL(intr_i_cl);
		break;
	default:
		break;
	}
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << HEVC0_HW_INT_EN)));
		SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << HEVC1_HW_INT_EN)));
		break;
	case VDEC_CORE_NUM_CNM:
		SetReg(AD_M1_INTR_ENABLE, (GetReg(AD_M1_INTR_ENABLE) & ~(1 << H264_HW_INT_EN)));
		VD2_RC0_RdFL(intr_i_en);
		VD2_RC0_Wr(intr_i_en, 0x0);
		VD2_RC0_WrFL(intr_i_en);

		VD2_RC0_RdFL(intr_i_cl);
		VD2_RC0_Wr(intr_i_cl, 0xFFFFFFFF);
		VD2_RC0_WrFL(intr_i_cl);
		break;
	default:
		break;
	}
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
	BOOLEAN			bRet = FALSE;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		if( ui8IntrSrc == 0 )
		{
			bRet = !!(GetReg(AD_M1_INTR_ENABLE) & (1 << HEVC0_HW_INT_EN));
		}
		else if( ui8IntrSrc == 1 )
		{
			bRet = !!(GetReg(AD_M1_INTR_ENABLE) & (1 << HEVC1_HW_INT_EN));
		}
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(intr_i_en);
		bRet = !!((VD2_RC0_Rd(intr_i_en) & (1 << ui8IntrSrc)) & (GetReg(AD_M1_INTR_ENABLE) & (1 << H264_HW_INT_EN)));
		break;
	default:
		break;
	}

	return bRet;
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
	UINT32 ui32Ret = 0;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(intr_i_st);
		ui32Ret = VD2_RC0_Rd(intr_i_st);
		break;
	default:
		break;
	}

	return ui32Ret;
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(feed_timer_conf);
		VD2_RC0_Wr01(feed_timer_conf, feed_timer_enable, 1);
		VD2_RC0_WrFL(feed_timer_conf);
		break;
	default:
		break;
	}
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(feed_timer_conf);
		VD2_RC0_Wr01(feed_timer_conf, feed_timer_enable, 0);
		VD2_RC0_WrFL(feed_timer_conf);
		break;
	default:
		break;
	}
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
	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(feed_timer_conf);
		VD2_RC0_Wr01(feed_timer_conf, feed_timer, ui32Value*200*1000); // for 200mhz domain, input:milli second
		VD2_RC0_WrFL(feed_timer_conf);
		break;
	default:
		break;
	}
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
	UINT32 ui32Value = -1;

	switch( ui8CoreNum )
	{
	case VDEC_CORE_NUM_HEVC:
		break;
	case VDEC_CORE_NUM_CNM:
		VD2_RC0_RdFL(feed_timer_conf);
		VD2_RC0_Rd01(feed_timer_conf, feed_timer, ui32Value);
		break;
	default:
		break;
	}

	return ui32Value;
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
    return VDC_HAL_RET_OK;
}
EXPORT_SYMBOL(VDC_HAL_ExtraCmd);

/*========================================================================================
    Static Implementation Group
========================================================================================*/

/** @} */

