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
 *  main driver implementation for vision processor device.
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

#ifndef _VIP_DRV_HAL_H15_A0_H_
#define _VIP_DRV_HAL_H15_A0_H_

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>

#include "vip_debug.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void VIP_HAL_Set_Axi_Param_H15_A0(void *vip_priv);
extern void *VIP_HAL_Get_Ops_H15_A0(void *vip_priv);

#ifdef	__cplusplus
}
#endif /* __cplusplus */


#endif	/* _VIP_DRV_HAL_H15_A0_H_ */

