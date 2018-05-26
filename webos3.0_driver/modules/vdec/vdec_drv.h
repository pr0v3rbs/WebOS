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
 *  driver interface header for vdec device. ( used only within kdriver )
 *	vdec device will teach you how to make device driver with new platform.
 *
 *  @author		seokjoo.lee (seokjoo.lee@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1152_vdec
 * @{
 */

#ifndef	_VDEC_DRV_H_
#define	_VDEC_DRV_H_

extern	void     VDEC_PreInit(void);
extern	int      VDEC_Init(void);
extern	void     VDEC_Cleanup(void);

extern struct proc_dir_entry *vdec_proc_root_entry;

#endif /* _VDEC_DRV_H_ */

/** @} */
