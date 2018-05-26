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
 * date       2012.01.19
 * note
 *
 */

#ifndef _VDEC_STC_TIMER_DRV_H_
#define _VDEC_STC_TIMER_DRV_H_

#include "../hal/vdec_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*STC_TIMER_CALLBACK_T)(void);

#define STC_TIMER_ID 0
#define STC_TIMER_ISR_FEED_MASK	(LQC0_ALMOST_EMPTY)
#define FEED_TIMER_Feed_msPERIOD	50	// ms

void STC_TIMER_Init(STC_TIMER_CALLBACK_T pfnCallback);
void STC_TIMER_Stop(void);
void STC_TIMER_Start(SINT32 timerValue);
UINT32 STC_TIMER_GetStatus(void);

void STC_TIMER_ISR(UINT8 ui8Ch);

#ifdef __cplusplus
}
#endif
#endif /* _VDEC_STC_TIMER_DRV_H_*/
