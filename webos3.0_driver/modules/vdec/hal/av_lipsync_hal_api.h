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
 * DE IPC HAL.
 * (Register Access Hardware Abstraction Layer)
 *
 * author     youngki.lyu (youngki.lyu@lge.com)
 * version    0.1
 * date       2011.10.19
 * note       Additional information.
 *
 */

#ifndef _VDEC_AV_LIPSYNC_HAL_API_H_
#define _VDEC_AV_LIPSYNC_HAL_API_H_


void AV_LipSync_HAL_Init(void);
void AV_LipSync_HAL_Suspend (void);
void AV_LipSync_HAL_Resume (void);
void AV_Set_AVLipSyncBase(int ch_vdec, int ch_adec,
		unsigned int *_base_stc, unsigned int *_base_pts);
void AV_LipSync_HAL_Adec_GetBase(int ch,
		unsigned int *base_stc, unsigned int *base_pts);


#endif /* _VDEC_AV_LIPSYNC_HAL_API_H_ */

