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
 *  img driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#ifndef _VIP_DRV_IMG_H_
#define _VIP_DRV_IMG_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

enum vip_img_fault_completion {
	VIP_IMG_FAULT_COMPLETION_NONE		= VIP_FAULT_COMPLETION_NONE,
	VIP_IMG_FAULT_COMPLETION_POLLING	= VIP_FAULT_COMPLETION_POLLING,
	VIP_IMG_FAULT_COMPLETION_WAITING	= VIP_FAULT_COMPLETION_WAITING,
	VIP_IMG_FAULT_COMPLETION_ABORTED	= VIP_FAULT_COMPLETION_ABORTED,
	VIP_IMG_FAULT_COMPLETION_RUN		= VIP_FAULT_COMPLETION_RUN,
	VIP_IMG_FAULT_COMPLETION_MAX		= VIP_FAULT_COMPLETION_MAX,
};

extern void *VIP_IMG_Get_FOPS(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _VIP_DRV_IMG_H_ */



