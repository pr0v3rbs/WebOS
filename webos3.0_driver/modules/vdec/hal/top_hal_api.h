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
 * author     youngki.lyu@lge.com
 * version    0.1
 * date       2011.02.23
 * note       Additional information.
 *
 */

#ifndef _TOP_HAL_API_h
#define _TOP_HAL_API_h


#include "vdec_type_defs.h"


typedef enum{
	MACH0				= 0,
	MACH1				= 1,
	NOT_ALLOWED_MEMACCESS		= 2,
	VDO				= 3,
#if defined(CHIP_NAME_d14)
	VSYNC0				= 6,
	VSYNC1				= 7,
#else
	VSYNC0				= 4,
	VSYNC1				= 5,
	VSYNC2				= 6,
	VSYNC3				= 7,
#endif
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
	VSYNC4				= 8,
	VSYNC5				= 9,
#else
	LQ0_DONE			= 8,
	LQ1_DONE			= 9,
	LQ2_DONE			= 10,
	LQ3_DONE			= 11,
#endif
	PDEC0_AU_DETECT			= 12,
	PDEC1_AU_DETECT			= 13,
	PDEC2_AU_DETECT			= 14,
	BUFFER_STATUS			= 16,
	VDO_GDI_ERROR			= 17,
	MACH0_SRST_DONE			= 18,
	MACH1_SRST_DONE			= 19,
	PDEC_SRST_DONE			= 20,
	VDO_SRST_DONE			= 21,
	VENC				= 22,
	VSYNC0_READY			= 23,
	VSYNC1_READY			= 24,
	MONITOR0			= 25,
	MONITOR1			= 26,
//	MONITOR2			= 27,
	TIMER0				= 28,
	TIMER1				= 29,
	TIMER2				= 30,
	TIMER3				= 31,
}VDEC_INTR_T;

typedef enum{
	PDEC0_CPB_ALMOST_FULL		= 0,
	PDEC0_CPB_ALMOST_EMPTY		= 1,
	PDEC0_AUB_ALMOST_FULL		= 2,
	PDEC0_AUB_ALMOST_EMPTY		= 3,
	PDEC0_BDRC_ALMOST_FULL		= 4,
	PDEC0_BDRC_ALMOST_EMPTY		= 5,
	PDEC0_IES_ALMOST_FULL		= 6,
	PDEC1_CPB_ALMOST_FULL		= 8,
	PDEC1_CPB_ALMOST_EMPTY		= 9,
	PDEC1_AUB_ALMOST_FULL		= 10,
	PDEC1_AUB_ALMOST_EMPTY		= 11,
	PDEC1_BDRC_ALMOST_FULL		= 12,
	PDEC1_BDRC_ALMOST_EMPTY		= 13,
	PDEC1_IES_ALMOST_FULL		= 14,
	PDEC2_CPB_ALMOST_FULL		= 16,
	PDEC2_CPB_ALMOST_EMPTY		= 17,
	PDEC2_AUB_ALMOST_FULL		= 18,
	PDEC2_AUB_ALMOST_EMPTY		= 19,
	PDEC2_BDRC_ALMOST_FULL		= 20,
	PDEC2_BDRC_ALMOST_EMPTY		= 21,
	PDEC2_IES_ALMOST_FULL		= 22,
	LQC0_ALMOST_EMPTY		= 24,
	LQC1_ALMOST_EMPTY		= 25,
	LQC2_ALMOST_EMPTY		= 26,
	LQC3_ALMOST_EMPTY		= 27,
	LQC0_ALMOST_FULL		= 28,
	LQC1_ALMOST_FULL		= 29,
	LQC2_ALMOST_FULL		= 30,
	LQC3_ALMOST_FULL		= 31,
}BUFFER_INTR_T;

void TOP_HAL_Init(void);
void TOP_HAL_Suspend(void);
void TOP_HAL_Resume(void);
UINT32 TOP_HAL_GetVersion(void);
void TOP_HAL_ResetMach(UINT8 vcore_num);
void TOP_HAL_ResetPDECAll(void);
void TOP_HAL_SetPdecInputSelection(UINT8 ui8PdecCh, UINT8 ui8TECh);
UINT8 TOP_HAL_GetPdecInputSelection(UINT8 ui8PdecCh);
void TOP_HAL_EnablePdecInput(UINT8 ui8PdecCh);
void TOP_HAL_DisablePdecInput(UINT8 ui8PdecCh);
UINT8 TOP_HAL_IsEnablePdecInput(UINT8 ui8PdecCh);
void TOP_HAL_SetLQSyncMode(UINT8 ui8LQCh, UINT8 ui8sync);
UINT8 TOP_HAL_GetLQSyncMode(UINT8 ui8LQCh);
void TOP_HAL_SetLQInputSelection(UINT8 ui8LQCh, UINT8 ui8PdecCh);
UINT8 TOP_HAL_GetLQInputSelection(UINT8 ui8LQCh);
void TOP_HAL_EnableLQInput(UINT8 ui8LQCh);
void TOP_HAL_DisableLQInput(UINT8 ui8LQCh);
UINT8 TOP_HAL_IsEnableLQInput(UINT8 ui8LQCh);
void TOP_HAL_SetMachIntrMode(UINT8 vcore_num, UINT8 IntrMode);
UINT8 TOP_HAL_GetMachIntrMode(UINT8 vcore_num);
UINT8 TOP_HAL_GetMachIdleStatus(UINT8 vcore_num);
UINT8 TOP_HAL_GetMachUnderRunStatus(UINT8 vcore_num);

void TOP_HAL_EnableExtIntr(VDEC_INTR_T ui32IntrSrc);
int TOP_HAL_DisableExtIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearExtIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearExtIntrMsk(UINT32 ui32IntrMsk);
void TOP_HAL_DisableExtIntrAll(void);
UINT8 TOP_HAL_IsExtIntrEnable(VDEC_INTR_T eVdecIntrSrc);
UINT32 TOP_HAL_GetExtIntrStatus(void);
void TOP_HAL_EnableInterIntr(VDEC_INTR_T ui32IntrSrc);
int TOP_HAL_DisableInterIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearInterIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearInterIntrMsk(UINT32 ui32IntrMsk);
void TOP_HAL_DisableInterIntrAll(void);
UINT8 TOP_HAL_IsInterIntrEnable(VDEC_INTR_T ui32IntrSrc);
UINT32 TOP_HAL_GetInterIntrStatus(void);


void TOP_HAL_EnableIntr(VDEC_INTR_T ui32IntrSrc);
int TOP_HAL_DisableIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearIntr(VDEC_INTR_T ui32IntrSrc);
void TOP_HAL_ClearIntrMsk(UINT32 ui32IntrMsk);
void TOP_HAL_DisableIntrAll(void);
UINT8 TOP_HAL_IsIntrEnable(VDEC_INTR_T ui32IntrSrc);
UINT32 TOP_HAL_GetIntrStatus(void);


void TOP_HAL_EnableBufIntr(BUFFER_INTR_T ui32IntrSrc);
void TOP_HAL_DisableBufIntr(BUFFER_INTR_T ui32IntrSrc);
void TOP_HAL_ClearBufIntr(BUFFER_INTR_T ui32IntrSrc);
void TOP_HAL_ClearBufIntrMsk(UINT32 ui32IntrMsk);
void TOP_HAL_DisableBufIntrAll(void);
UINT8 TOP_HAL_IsBufIntrEnable(BUFFER_INTR_T ui32IntrSrc);
UINT32 TOP_HAL_GetBufIntrStatus(void);
UINT32 TOP_HAL_GetGSTCC(void);
UINT32 TOP_HAL_SetApbMachOffset(UINT8 ui8CoreNume, UINT8 ui8Offset);
UINT32 TOP_HAL_SetApbVDO(UINT8 ui8GdiSelection, UINT8 ui8Offset);
UINT32 TOP_HAL_SetApbConfig(UINT32 ui32Value);
void TOP_HAL_SetTimerIntrSelection(UINT8 ui8TimerIntrNum, UINT8 ui8TimerCh);
UINT8 TOP_HAL_GetTimerIntrSelection(UINT8 ui8TimerIntrNum);
void TOP_HAL_EnableFeedTimer(void);
void TOP_HAL_DisableFeedTimer(void);
void TOP_HAL_SetFeedTimerValue(UINT32 ui32Value);
UINT32 TOP_HAL_GetFeedTimerValue(void);
void TOP_HAL_EnableFeedTimer(void);
void TOP_HAL_DisableFeedTimer(void);
void TOP_HAL_SetFeedTimerValue(UINT32 ui32Value);
UINT32 TOP_HAL_GetFeedTimerValue(void);

void	TOP_HAL_EnableVsyncIntr(VDEC_INTR_T s);
int	TOP_HAL_DisableVsyncIntr(VDEC_INTR_T s);
void	TOP_HAL_ClearVsyncIntr(VDEC_INTR_T s);
void	TOP_HAL_ClearVsyncIntrMsk(VDEC_INTR_T s);
UINT8	TOP_HAL_IsVsyncIntrEnable(VDEC_INTR_T s);
UINT32	TOP_HAL_GetVsyncIntrStatus(void);

#endif // endif of _TOP_HAL_API_h

