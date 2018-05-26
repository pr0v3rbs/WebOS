/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     sooya.joo@lge.com
 * version    0.1
 * date       2010.03.11
 * note       Additional information.
 *
 * @addtogroup h13_vdec
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/io.h>
#include <linux/module.h>
#include <linux/spinlock.h>

#include "hal/vdec_base.h"
#include "vd3_rc.h"
#include "hal/top_hal_api.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define VD3_RC_BASE						(VDEC_BASE + 0x60C00)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static volatile VD3_RC_REG_T		*pstVd3RcReg = NULL;
static volatile VD3_RC_REG_T		pstVd3RcShd[1];
static spinlock_t					_gstApbConfLock;

/**
********************************************************************************
* @brief
*   register setting
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  DTV_STATUS_T
********************************************************************************
*/
void VD3_HAL_Init(void)
{
	pstVd3RcReg		= (volatile VD3_RC_REG_T*)ioremap(VD3_RC_BASE, 0x90);
	spin_lock_init(&_gstApbConfLock);
}

UINT32 VD3_HAL_GetVersion(void)
{
	UINT32 ui32Version;

	VD3_RC_RdFL(version);
	VD3_RC_Rd01(version, yyyymmdd, ui32Version);

	return ui32Version;
}


void VD3_HAL_SetVsyncSrc(UINT8 ui8Ch, UINT8 ui8Src)
{
	//if(ui8Ch>=LQC_NUM_OF_CHANNEL)
	//	return;
	//if(ui8Src>=DE_IF_NUM_OF_CHANNEL)
	//	return;

	VD3_RC_RdFL(vsync_src_sel);

	switch(ui8Ch)
	{
	case 0:
		VD3_RC_Wr01(vsync_src_sel, reg_vsync0_src_sel, ui8Src&0x1);
		break;
	case 1:
		VD3_RC_Wr01(vsync_src_sel, reg_vsync1_src_sel, ui8Src&0x1);
		break;
	case 2:
		VD3_RC_Wr01(vsync_src_sel, reg_vsync2_src_sel, ui8Src&0x1);
		break;
	case 3:
		VD3_RC_Wr01(vsync_src_sel, reg_vsync3_src_sel, ui8Src&0x1);
		break;
	}
	VD3_RC_WrFL(vsync_src_sel);
}

UINT8 VD3_HAL_GetVsyncSrc(UINT8 ui8Ch)
{
	UINT8 ui8Src=0;
	if(ui8Ch>=LQC_NUM_OF_CHANNEL)
		return -1;

	VD3_RC_RdFL(vsync_src_sel);

	switch(ui8Ch)
	{
	case 0:
		VD3_RC_Rd01(vsync_src_sel, reg_vsync0_src_sel, ui8Src);
		break;
	case 1:
		VD3_RC_Rd01(vsync_src_sel, reg_vsync1_src_sel, ui8Src);
		break;
	case 2:
		VD3_RC_Rd01(vsync_src_sel, reg_vsync2_src_sel, ui8Src);
		break;
	case 3:
		VD3_RC_Rd01(vsync_src_sel, reg_vsync3_src_sel, ui8Src);
		break;
	}

	return ui8Src;
}


/* internal(MCU side) interrupt control */
//#warning "need to protect with spinlock?"
void VD3_HAL_EnableInterIntr(VDEC_INTR_T ui32IntrSrc)
{
	VD3_RC_RdFL(intr_i_en);
	VD3_RC_Wr(intr_i_en, VD3_RC_Rd(intr_i_en) |(1 << ui32IntrSrc));
	VD3_RC_WrFL(intr_i_en);
}
EXPORT_SYMBOL(VD3_HAL_EnableInterIntr);

int VD3_HAL_DisableInterIntr(VDEC_INTR_T ui32IntrSrc)
{
	int ret;

	VD3_RC_RdFL(intr_i_en);
	ret = !! (VD3_RC_Rd(intr_i_en) & (1 << ui32IntrSrc));
	VD3_RC_Wr(intr_i_en, VD3_RC_Rd(intr_i_en) & ~(1 << ui32IntrSrc));
	VD3_RC_WrFL(intr_i_en);

	return ret;
}
EXPORT_SYMBOL(VD3_HAL_DisableInterIntr);

void VD3_HAL_ClearInterIntr(VDEC_INTR_T ui32IntrSrc)
{
	VD3_RC_RdFL(intr_i_cl);
	VD3_RC_Wr(intr_i_cl, VD3_RC_Rd(intr_i_cl) |(1 << ui32IntrSrc));
	VD3_RC_WrFL(intr_i_cl);
}
EXPORT_SYMBOL(VD3_HAL_ClearInterIntr);

void VD3_HAL_ClearInterIntrMsk(UINT32 ui32IntrMsk)
{
	VD3_RC_RdFL(intr_i_cl);
	VD3_RC_Wr(intr_i_cl, ui32IntrMsk);
	VD3_RC_WrFL(intr_i_cl);
}
EXPORT_SYMBOL(VD3_HAL_ClearInterIntrMsk);

void VD3_HAL_DisableInterIntrAll(void)
{
	VD3_RC_RdFL(intr_i_en);
	VD3_RC_Wr(intr_i_en, 0x0);
	VD3_RC_WrFL(intr_i_en);

	VD3_RC_RdFL(intr_i_cl);
	VD3_RC_Wr(intr_i_cl, 0xFFFFFFFF);
	VD3_RC_WrFL(intr_i_cl);
}
EXPORT_SYMBOL(VD3_HAL_DisableInterIntrAll);

UINT8 VD3_HAL_IsInterIntrEnable(VDEC_INTR_T ui32IntrSrc)
{
	UINT32 ui32Intr;
	VD3_RC_RdFL(intr_i_en);
	ui32Intr = VD3_RC_Rd(intr_i_en);
	if(VD3_RC_Rd(intr_i_en) & (1 << ui32IntrSrc))
	{
		return TRUE;
	}

	return FALSE;
}
EXPORT_SYMBOL(VD3_HAL_IsInterIntrEnable);

UINT32 VD3_HAL_GetInterIntrStatus(void)
{
	VD3_RC_RdFL(intr_i_st);
	return VD3_RC_Rd(intr_i_st);
}
EXPORT_SYMBOL(VD3_HAL_GetInterIntrStatus);

void VD3_HAL_SetTimerIntrSelection(UINT8 ui8TimerIntrNum, UINT8 ui8TimerCh)
{
	VD3_RC_RdFL(timer_src_sel);

	switch( ui8TimerIntrNum )
	{
	case 0:
		VD3_RC_Wr01(timer_src_sel, reg_timer0_src_sel, ui8TimerCh);
		break;

	case 1:
		VD3_RC_Wr01(timer_src_sel, reg_timer1_src_sel, ui8TimerCh);
		break;

	case 2:
		VD3_RC_Wr01(timer_src_sel, reg_timer2_src_sel, ui8TimerCh);
		break;

	case 3:
		VD3_RC_Wr01(timer_src_sel, reg_timer3_src_sel, ui8TimerCh);
		break;

	default:
		break;
	}

	VD3_RC_WrFL(timer_src_sel);
}

UINT8 VD3_HAL_GetTimerIntrSelection(UINT8 ui8TimerIntrNum)
{
	UINT8 ui8TimerCh;

	VD3_RC_RdFL(timer_src_sel);

	switch( ui8TimerIntrNum )
	{
	case 0:
		VD3_RC_Rd01(timer_src_sel, reg_timer0_src_sel, ui8TimerCh);
		break;

	case 1:
		VD3_RC_Rd01(timer_src_sel, reg_timer1_src_sel, ui8TimerCh);
		break;

	case 2:
		VD3_RC_Rd01(timer_src_sel, reg_timer2_src_sel, ui8TimerCh);
		break;

	case 3:
		VD3_RC_Rd01(timer_src_sel, reg_timer3_src_sel, ui8TimerCh);
		break;

	default:
		return -1;
	}

	return ui8TimerCh;
}

/** @} */

