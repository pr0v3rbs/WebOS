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
 * author     junghyun.son@lge.com
 * version    1.0
 * date       2013.10.17
 * note       Additional information.
 *
 * @addtogroup lg115x_vdc
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "vdc_util.h"

static UINT32 _GetGcd(UINT32 a, UINT32 b);

UINT32 CalcFrameDuration(UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv)
{
	UINT32      ui32FrameDuration90K = 0;

	UINT32      ui32FrameRateRes_Scaled;
	UINT32      ui32_90kHz_Scaled = 90000;
	UINT32      ui32Gcd;
	UINT32      ui320xFFFFFFFF_Boundary;
	UINT32      ui32FrameRateDiv_Temp;

	const UINT32    ui32ScaleUnit = 10; // must > 1

	if( ui32FrameRateRes == 0 || ui32FrameRateDiv == 0)
		return 0;

	ui32Gcd = _GetGcd( ui32_90kHz_Scaled, ui32FrameRateRes );

	ui32_90kHz_Scaled /= ui32Gcd;
	ui32FrameRateRes_Scaled = ui32FrameRateRes / ui32Gcd;

	ui320xFFFFFFFF_Boundary = 0xFFFFFFFF / ui32_90kHz_Scaled;

	ui32FrameRateDiv_Temp = ui32FrameRateDiv;
	while( ui32_90kHz_Scaled )
	{
		if( ui32FrameRateDiv_Temp <= ui320xFFFFFFFF_Boundary )
			break;

		ui32FrameRateDiv_Temp /= ui32ScaleUnit;
		ui32_90kHz_Scaled /= ui32ScaleUnit;
		ui32FrameRateRes_Scaled /= ui32ScaleUnit;
	}

	// FrameDuration90K = 90KHz / (FrameRateRes / FrameRateDiv)
	ui32FrameDuration90K = ui32_90kHz_Scaled * ui32FrameRateDiv / ui32FrameRateRes_Scaled; // = 90kHz / FrameRate

	return ui32FrameDuration90K;
}

static UINT32 _GetGcd(UINT32 a, UINT32 b)
{
	if(a%b == 0)
		return b;
	return _GetGcd(b,a%b);
}

void BubbleSort(long long int array[], int n)
{
    int i, j;
    long long int tmp;
    for(i=0; i<n; i++)
    {
        for(j=0; j<n-i-1; j++)
        {
            if( array[j] > array[j+1] )
            {
                tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}
