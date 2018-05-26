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
#ifndef _VDEC_PROC_H
#define _VDEC_PROC_H

#include <linux/proc_fs.h>

#include "decoder.h"

extern struct proc_dir_entry *vdec_proc_root_entry;

int proc_make_instance (vdec_t *vdec);
void proc_rm_instance (vdec_t *vdec);

int proc_init(void);
void proc_cleanup (void);

#endif

