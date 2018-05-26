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
 * version    1.0
 * date       2011.05.10
 * note
 *
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/string.h>	// memset
#include <linux/delay.h>

#include "vdec_type_defs.h"

#include "hal/top_hal_api.h"
#include "hal/lq_hal_api.h"

#include "vdec_stc_timer.h"

#include "log.h"

logm_define (vdec_stc, log_level_warning);

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

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
static STC_TIMER_CALLBACK_T _gpfnCallback;

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

void STC_TIMER_Init(STC_TIMER_CALLBACK_T pfnCallback)
{
	TOP_HAL_EnableIntr(BUFFER_STATUS);

	STC_TIMER_Stop();

	// Set Feed Timer
#if !defined(CHIP_NAME_h13) || (CHIP_REV>=0xb0)
	logm_info(vdec_stc,"vdec stc timer init \n");
	TOP_HAL_SetFeedTimerValue(FEED_TIMER_Feed_msPERIOD);
	TOP_HAL_EnableFeedTimer();
#else
	logm_info(vdec_stc,"vdec stc timer init \n");
	LQ_HAL_SetAEmptyLevel(STC_TIMER_ID, 0);
	LQ_HAL_SetEmptyTimer(STC_TIMER_ID, FEED_TIMER_Feed_msPERIOD);
	LQ_HAL_EnableEmptyTimer(STC_TIMER_ID);
#endif

	_gpfnCallback = pfnCallback;
}

void STC_TIMER_Stop(void)
{
	if(TOP_HAL_IsBufIntrEnable(STC_TIMER_ISR_FEED_MASK)==1)
	{
		TOP_HAL_DisableBufIntr(STC_TIMER_ISR_FEED_MASK);
		TOP_HAL_ClearBufIntr(STC_TIMER_ISR_FEED_MASK);
	}

}

void STC_TIMER_Start(SINT32 timerValue)
{
	if(TOP_HAL_IsBufIntrEnable(STC_TIMER_ISR_FEED_MASK)==0)
	{
		if(timerValue>0){
			TOP_HAL_SetFeedTimerValue(timerValue);
		}
		TOP_HAL_EnableBufIntr(STC_TIMER_ISR_FEED_MASK);
	}
}

UINT32 STC_TIMER_GetStatus(void)
{
	return TOP_HAL_IsBufIntrEnable(STC_TIMER_ISR_FEED_MASK);
}

void STC_TIMER_ISR(UINT8 ui8Ch)
{
	_gpfnCallback();
	TOP_HAL_ClearBufIntrMsk(1 << LQC0_ALMOST_EMPTY);
}
