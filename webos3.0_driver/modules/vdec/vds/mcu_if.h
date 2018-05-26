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
 * main driver implementation for MCU I/F.
 *
 * author     taewoong.kim@lge.com
 * version    1.0
 * date       2013.08.09
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */

#ifndef _VDEC_MCU_IF_H_
#define _VDEC_MCU_IF_H_

#include "vdec_type_defs.h"
#include "disp_q.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	BOOLEAN		bCur;
	BOOLEAN		bNext;
	UINT32		u32DispPeriod;
	UINT32		u32FieldCnt;
} MCUIF_DISP_STATE;

typedef struct
{
	UINT32		u32VsyncCh;
	BOOLEAN		bEmpty;
	UINT32		u32VsyncGstc;
	UINT32		u32SyncDuration;
} VSYNC_INFO_T;

typedef enum {
	MMCU_PIPE_SET_REQPIPE,
	MMCU_PIPE_SET_NEWFRAME,
	MPIPE_CLR_FRAME,
	MPIPE_VSYNC_STATE,
	MPIPE_INIT_CH,
	MPIPE_CLOSE_CH,
	MPIPE_SEL_DE,
	MPIPE_SEL_VSYNC,
	MPIPE_DISP_STATE,
	MPIPE_SET_DUALOUT,
	MPIPE_SET_REPEAT
} MMCU_PIPE_CMD;

typedef struct {
	UINT32	delimiter;
	UINT32	eType;
	UINT32	size;
	UINT32	u32Param[7];
} MMCU_PIPE_ENTRY_T;

#ifdef __cplusplus
}
#endif

void MCU_IF_Init(void);
void _mpipe_send_frame(UINT8 u8DeIfCh, S_DISPQ_BUF_T *pstDispBuf);
void _mpipe_sel_vsync(UINT8 u8DeIfCh, UINT8 u8VsyncCh);
void _mpipe_send_init_ch(UINT8 u8DeIfCh);
void _mpipe_send_del_ch(UINT8 u8DeIfCh);
void _mpipe_send_set_dualout(UINT8 u8MirrorSrc, UINT8 u8MirrorPort);
void _mpipe_send_set_repeat(UINT8 u8DeIfCh, UINT8 u8nTimes);
void _mpipe_send_target(UINT8 u8DeIfCh, UINT8 u8TargetDe);
#endif /* _VDEC_MCU_IF_H_ */

