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

/**
 * @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     youngki.lyu@lge.com
 * version    1.0
 * date       2011.05.31
 * note       Additional information.
 *
 * @addtogroup lg1152_vdec
 * @{
 */


#define LOG_NAME	vdec_cq

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "disp_clear_q.h"

#include "log.h"


struct clear_q
{
	S_DISPCLEARQ_BUF_T *buffer;
	int size;

	volatile int head, tail;
};

void DISP_CLEAR_Q_Init(void)
{
}

clear_q_t *DISP_CLEAR_Q_Open(int size)
{
	clear_q_t *q;

	q = kcalloc (sizeof (*q), 1, GFP_KERNEL);

	q->buffer = kmalloc (sizeof(S_DISPCLEARQ_BUF_T) * size, GFP_KERNEL);
	if( q->buffer == NULL )
	{
		printk (KERN_ERR "kmalloc failed\n");
		return NULL;
	}

	q->size = size;

	log_info ("%p\n", q);
	return q;
}

void DISP_CLEAR_Q_Close(clear_q_t *q)
{
	log_info ("%p\n", q);
	kfree (q->buffer);
	kfree (q);
}

static BOOLEAN DISP_CLEAR_Q_Push(clear_q_t *q, S_DISPCLEARQ_BUF_T *data)
{
	int head;

	log_debug ("%p, index %x, y %07lx\n", q,
			data->ui32FrameIdx,
			data->y);
	if ((q->head + 1 % q->size) == q->tail)
	{
		log_warning ("clear q full\n");
		return FALSE;
	}

	q->buffer[q->head] = *data;

	/* maintain an atomic operation for queue head and tail pointer */
	head = q->head;
	head ++;
	head %= q->size;
	q->head = head;

	return TRUE;
}

BOOLEAN DISP_CLEAR_Q_Push_Index(clear_q_t *q, int index)
{
	S_DISPCLEARQ_BUF_T data;

	data.ui32FrameIdx = index;
	data.y = -1;

	return DISP_CLEAR_Q_Push (q, &data);
}

BOOLEAN DISP_CLEAR_Q_Push_Y(clear_q_t *q, unsigned long y)
{
	S_DISPCLEARQ_BUF_T data;

	data.ui32FrameIdx = -1;
	data.y = y;

	return DISP_CLEAR_Q_Push (q, &data);
}

BOOLEAN DISP_CLEAR_Q_Pop(clear_q_t *q, S_DISPCLEARQ_BUF_T *data)
{
	int tail;

	if (q->head == q->tail)
		return FALSE;

	*data = q->buffer[q->tail];

	/* maintain an atomic operation for queue head and tail pointer */
	tail = q->tail;
	tail ++;
	tail %= q->size;
	q->tail = tail;

	log_trace ("%p, index %x, y %07lx\n", q,
			data->ui32FrameIdx,
			data->y);
	return TRUE;
}

