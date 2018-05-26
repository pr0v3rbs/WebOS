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

#ifndef	_VIP_DRV_HAL_H_
#define	_VIP_DRV_HAL_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#define VIP_FIRM_PAGE_ALIGN_MASK	0xFF
#define VIP_FIRM_IOREMAP_SIZE(x) 	(((x) + VIP_FIRM_PAGE_ALIGN_MASK ) & ~(VIP_FIRM_PAGE_ALIGN_MASK))

struct vip_axi_set_param {
	unsigned int		val;
	unsigned int		reg_idx;
};

struct vip_firm_resource {
	unsigned int		*data;
	resource_size_t		start;
	resource_size_t		size;
	resource_size_t		size_max;
	char				*name;
	char				*prt_name;
};

#if	1
#define vip_writel(_v, _r)		writel((_v), (_r));
#define vip_readl(_r)			readl((_r))
#else
#define vip_writel(_v, _r)		*((volatile unsigned int*)(_r)) = (_v)
#define vip_readl(_r)			*((volatile unsigned int*)(_r))
#endif

extern void VIP_Get_IMG_Hal_Ops(struct vip_dev *vip);

extern void VIP_HAL_Firm_Exit(struct vip_dev *vip);
extern int VIP_HAL_Firm_Down(struct vip_dev *vip);

extern void VIP_HAL_Free_Device(struct vip_dev *vip);
extern int VIP_HAL_Alloc_Device(struct vip_dev *vip);

#ifdef	__cplusplus
}
#endif /* __cplusplus */


#endif	/* _VIP_DRV_HAL_H_ */



