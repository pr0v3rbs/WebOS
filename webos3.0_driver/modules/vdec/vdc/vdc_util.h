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
 * Video decoding driver header for vdec device.
 * VDEC device will teach you how to make device driver with lg115x platform.
 *
 * author     Junghyun son(junghyun.son@lge.com)
 * version    1.0
 * date       2012.05.10
 * note       Additional information.
 *
 */

#ifndef _VDC_UTIL_H_
#define _VDC_UTIL_H_

#include "vdec_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/

UINT32 CalcFrameDuration(UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv);
void BubbleSort(long long int array[], int n);

#ifdef __cplusplus
}
#endif
#endif /* _VDC_UTIL_H_ */
