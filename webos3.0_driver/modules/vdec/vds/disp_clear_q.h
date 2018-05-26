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
 * date       2011.05.31
 * note       Additional information.
 *
 */

#ifndef _VDEC_DISP_CLEAR_Q_H_
#define _VDEC_DISP_CLEAR_Q_H_


#include "vdec_type_defs.h"

typedef struct clear_q clear_q_t;

#include "disp_q.h"



#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
	UINT32 ui32FrameIdx;
	unsigned long y;
}S_DISPCLEARQ_BUF_T;

void DISP_CLEAR_Q_Init(void);

clear_q_t *DISP_CLEAR_Q_Open (int size);
void DISP_CLEAR_Q_Close (clear_q_t *q);
BOOLEAN DISP_CLEAR_Q_Push_Index (clear_q_t *q, int index);
BOOLEAN DISP_CLEAR_Q_Push_Y (clear_q_t *q, unsigned long y);
BOOLEAN DISP_CLEAR_Q_Pop (clear_q_t *q, S_DISPCLEARQ_BUF_T *data);


#ifdef __cplusplus
}
#endif

#endif /* _VDEC_DISP_CLEAR_Q_H_ */

