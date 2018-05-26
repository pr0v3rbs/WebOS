/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
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

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <linux/file.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "vip_drv.h"
#include "vip_core.h"
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

#ifdef USE_KDRV_CODES_FOR_H15
#include "./h15/vip_drv_hal_h15_a0.h"
#endif	/* INCLUDE_H15_CHIP_KDRV */

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 *
 */
void VIP_Get_IMG_Hal_Ops(struct vip_dev *vip)
{
	LX_MEMCFG_T *mem_cfg;
	struct vip_hal_ops *hal_ops;

	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
		mem_cfg = (LX_MEMCFG_T *)&gMemCfgVisionDSP[VIP_MEM_CFG_IDX_H15_A0];
		vipm_debug_hal("[%s] base[0x%08x], size[0x%08x]",
						mem_cfg->name, mem_cfg->base, mem_cfg->size);

		hal_ops = VIP_HAL_Get_Ops_H15_A0(vip);
		hal_ops->cfg.base_addr = mem_cfg->base;
		hal_ops->cfg.base_size = mem_cfg->size;

	#if (__VIP_MEMORY_MAP_TYPE == 0)
		hal_ops->cfg.imgb = (void *)(mem_cfg->base + 0x02000000);	//H15_VIP_IMG_BUF_ADDR
	#endif

		vip->vhal_ops = (struct vip_hal_ops *)hal_ops;

		VIP_HAL_Set_Axi_Param_H15_A0(vip);

		return;
	}

	vipm_err("<<< %s : LX_CHIP_REV => Unknown(0x%X)\n", __F__, lx_chip_rev());
}

static void *_VIP_HAL_IOMem_Alloc(unsigned int base,
							unsigned int size,
							char *name)
{
	void *addr = NULL;

	if (request_mem_region(base, size, name) == NULL) {
		vipm_err("[%s] request_mem_region failed : base[0x%08x] size[0x%08x]\n",
				name, base, size);
		goto out;
	}

	addr = (unsigned char *)ioremap(base, size);
	if (addr == NULL) {
		release_mem_region(base, size);
		vipm_err("[%s] ioremap failed : base[0x%08x] size[0x%08x]\n",
				name, base, size);
		goto out;
	}

	vipm_debug_hal("[%s] addr[0x%p] <- base[0x%08x]", name, addr, base);

out:
	return addr;
}

static void _VIP_HAL_IOMem_Free(void *addr,
							unsigned int base,
							unsigned int size,
							char *name)
{
	if (addr != NULL) {
		iounmap(addr);
	}
	if (base && size) {
		release_mem_region(base, size);
	}
	if (name != NULL) {
		vipm_debug_hal("Free %s\n", name);
	} else {
		vipm_debug_hal("Free 0x%p\n", addr);
	}
}

static void _VIP_HAL_Firm_Free(void *addr,
								struct vip_firm_resource *rsc)
{
	_VIP_HAL_IOMem_Free(addr, rsc->start, rsc->size, rsc->name);
}

static void *_VIP_HAL_Firm_Region(struct vip_firm_resource *rsc)
{
	void *addr = NULL;

	if (!rsc->size || (rsc->name == NULL)) {
		vipm_err("NULL size[%d] or name[0x%p]\n",
				(int)rsc->size, rsc->name);
		goto out;
	}

	addr = _VIP_HAL_IOMem_Alloc(rsc->start, rsc->size, rsc->name);
	if (addr == NULL) {
		vipm_err("[%s] IO Mem Failed : start[0x%08x] size[0x%08x]\n",
				rsc->name, rsc->start, rsc->size);
		goto out;
	}
	vipm_debug_hal("[%s] IO Mem Allocated : start[0x%08x] size[0x%08x]\n",
					rsc->name, rsc->start, rsc->size);

out:
	return addr;
}

static void _VIP_HAL_Firm_Load_Cpy(unsigned char *addr,
									struct vip_firm_resource *rsc)
{
	if (addr != NULL) {
		vipm_debug_hal("Firmware loading : %s\n", rsc->prt_name);
		vipm_debug_hal("                 : 0x%p <- 0x%p [%d]\n",
						addr, rsc->data, rsc->size);
		memcpy((void *)addr, (void *)rsc->data, rsc->size);
	}
}

static int _VIP_HAL_Firm_Load_FW(struct vip_dev *vip,
									struct vip_firm_resource *rsc)
{
	unsigned char *addr;
	int ret = 0;

	if (vip->vhal_ops->cfg.load_fw) {
		rsc->start = (resource_size_t)vip->firm_base;

		if (rsc->size > rsc->size_max) {
			vipm_err("[%s] Size overflow : data[0x%p] size[0x%08x]\n",
					rsc->name, rsc->data, rsc->size);
			return -EINVAL;
		}

		if (vip->firm_addr == NULL) {
			addr = _VIP_HAL_Firm_Region(rsc);
			if (addr == NULL) {
				vipm_err("NULL addr\n");
				ret = -ENOMEM;
				goto out;
			}
			vip->firm_addr = addr;
		}
		_VIP_HAL_Firm_Load_Cpy(vip->firm_addr, rsc);
	}

out:
	return ret;
}

#if !defined(VIP_GESTURE_ON)
static int _VIP_HAL_Firm_Load_LDB(struct vip_dev *vip,
									struct vip_firm_resource *rsc)
{
	unsigned char *addr;
	int ret = 0;

	if (vip->vhal_ops->cfg.load_ldb) {
		rsc->start = (resource_size_t)vip->lndb_base;

		if (rsc->size > rsc->size_max) {
			vipm_err("[%s] Size overflow : data[0x%p] size[0x%08x]\n",
					rsc->name, rsc->data, rsc->size);
			return -EINVAL;
		}

		if (vip->lndb_addr == NULL) {
			addr = _VIP_HAL_Firm_Region(rsc);
			if (addr == NULL) {
				vipm_err("NULL addr\n");
				ret = -ENOMEM;
				goto out;
			}
			vip->lndb_addr = addr;
		}
		_VIP_HAL_Firm_Load_Cpy(vip->lndb_addr, rsc);
	}

out:
	return ret;
}
#endif

void VIP_HAL_Firm_Exit(struct vip_dev *vip)
{
	struct vip_firm_resource *rsc = vip->vhal_ops->cfg.firmware;

	if (rsc) {
		if (vip->firm_addr) {
			_VIP_HAL_Firm_Free(vip->firm_addr, rsc);
			vip->firm_addr = NULL;
		}

	#if !defined(VIP_GESTURE_ON)
		if (vip->lndb_addr) {
			rsc++;
			_VIP_HAL_Firm_Free(vip->lndb_addr, rsc);
			vip->lndb_addr = NULL;
		}
	#endif
	}
}

int VIP_HAL_Firm_Down(struct vip_dev *vip)
{
	struct vip_firm_resource *rsc = vip->vhal_ops->cfg.firmware;
	int ret = 0;

	if (rsc) {
	#if defined(VIP_FIRM_DOWN_FROM_KADAP)
		rsc->data = (unsigned int *)vip->firm_ctrl.addr;
		rsc->size = (resource_size_t)vip->firm_ctrl.size;
		rsc->name = "VIP FIRM";
		rsc->prt_name = "GR - bin";
	#endif

		ret = _VIP_HAL_Firm_Load_FW(vip, rsc);
		if (ret) {
			goto out;
		}

	#if !defined(VIP_GESTURE_ON)
		rsc++;
		ret = _VIP_HAL_Firm_Load_LDB(vip, rsc);
		if (ret) {
			goto out;
		}
	#endif
	}
	return 0;

out:
	VIP_HAL_Firm_Exit(vip);
	return ret;
}

/**
 * ioremap vip memory
 * @see
*/
void VIP_HAL_Free_Device(struct vip_dev *vip)
{
	struct vip_reg_cfg *reg_cfg;

	if (vip->regb != NULL) {
		reg_cfg = vip->reg_cfg;

		vipm_debug_hal("Free for reg[0x%08x]\n", reg_cfg->reg_base);
		_VIP_HAL_IOMem_Free(vip->regb,
						reg_cfg->reg_base,
						reg_cfg->reg_size,
						reg_cfg->chip_name);

	}
	vip->regb = NULL;
	vip->ipcb = NULL;
}

int VIP_HAL_Alloc_Device(struct vip_dev *vip)
{
	struct vip_drvdata *drv_data = vip_dev_priv(vip);
	struct vip_reg_cfg *reg_cfg;
	void __iomem *regb = vip->regb;
	int ret = 0;

	if (drv_data && drv_data->is_suspended) {
		return ret;
	}

	if (regb == NULL) {
		reg_cfg = vip->reg_cfg;

		vipm_debug_hal("Alloc for reg[0x%08x]\n", reg_cfg->reg_base);
		regb = _VIP_HAL_IOMem_Alloc(reg_cfg->reg_base,
									reg_cfg->reg_size,
									reg_cfg->chip_name);
		if (regb == NULL) {
			vipm_err("NULL vip->regb\n");
			return -ENOMEM;
		}
		vip->regb = regb;
		vip->ipcb = regb;
	}

	return ret;
}


