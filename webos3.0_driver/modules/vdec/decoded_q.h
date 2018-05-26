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
#ifndef _DECODED_Q_H
#define _DECODED_Q_H

#include "include/vdec_kapi.h"

typedef struct _decoded_q decoded_q_t;

struct _decoded_q
{
	int (*push) (decoded_q_t *q, decoded_buffer_t *buf);
	int (*max_size) (decoded_q_t *q);
	int (*size) (decoded_q_t *q);
};

#define decoded_q_push(q,buf)	(q)->push(q,buf)
#define decoded_q_max_size(q)	(q)->max_size(q)
#define decoded_q_size(q)	(q)->size(q)

#endif
