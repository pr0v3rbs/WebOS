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

#include "../vip_drv.h"
#include "../vip_core.h"
#include "../vip_usb.h"
#include "../vip_queue.h"
#include "../vip_mem.h"
#include "../vip_proc.h"

#include "../vip_drv_hal.h"

#include "vip_reg_h15_a0.h"

#include "vip_fw_h15_a0.h"
#include "vip_ldb_h15_a0.h"

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/*
 *
 */
struct vip_msg_num {
	int		max;
	int		unit;
};

static const struct vip_msg_num g_vip_img_msg_range[VIP_IMG_FUNC_MAX] = {
	[VIP_IMG_FUNC_DETECT] = {
		.max	= VIP_MSG_MAX_OBJ_DETECT,
		.unit	= sizeof(struct vip_msg_detect),
	},
	[VIP_IMG_FUNC_GESTURE] = {
		.max	= VIP_MSG_MAX_OBJ_GESTURE,
		.unit	= sizeof(struct vip_msg_gesture),
	},
};

static const char *g_vip_img_func_name[VIP_IMG_FUNC_MAX] = {
	[VIP_IMG_FUNC_DETECT] = "Face Detection",
	[VIP_IMG_FUNC_GESTURE] = "Finger Gesture",
};

/*
 *
 */
//#define VIP_DEBUG_TOP_REG

#if defined(VIP_DEBUG_TOP_REG)
#define	__vip_debug_line()	\
		do {				\
			vipm_debug_hal("==================================================\n");	\
		} while(0)

#define __vip_debug_top_reg_msg(vip, idx, name)	\
		do {				\
			vipm_debug_hal("  %s 0x%04x[0x%08x]\n",	\
						name, vip_top_reg(idx),	\
						vip_top_readl_core(vip, idx));	\
		} while(0)
#endif

//#define VIP_PRT_TOP_REG

#if defined(VIP_PRT_TOP_REG)
#define print_top_reg(idx)	\
		do {	\
			char *idx_name = #idx;	\
			printk(" %24s : 0x%04x\n", idx_name, vip_top_reg(idx));	\
		} while(0)

#define print_ipc_reg(idx)	\
		do {	\
			char *idx_name = #idx;	\
			printk(" %24s : 0x%04x\n", idx_name, vip_ipc_reg(idx));	\
		} while(0)
#endif

void __VIP_Debug_Print_Reg_Top(void)
{
#if defined(VIP_PRT_TOP_REG)
	if (vipm_debug_hal_enabled()) {
		printk("%s\n", __func__);
		printk(" [TOP]\n");
		print_top_reg(TOP_RUN_STALL);
		print_top_reg(TOP_PRID);
		print_top_reg(TOP_PWAIT);
		print_top_reg(TOP_INTR_EN_0);
		print_top_reg(TOP_INTR_ST_0);
		print_top_reg(TOP_INTR_CLR_0);
		print_top_reg(TOP_INTR_EN_1);
		print_top_reg(TOP_INTR_ST_1);
		print_top_reg(TOP_INTR_CLR_1);
		print_top_reg(TOP_NMI_EN);
		print_top_reg(TOP_NMI_ST);
		print_top_reg(TOP_NMI_CLR);
		print_top_reg(TOP_INTR_CPU_EN);
		print_top_reg(TOP_INTR_CPU_ST);
		print_top_reg(TOP_INTR_CPU_CLR);

		print_top_reg(TOP_C0_AXI_BASE_5);
		print_top_reg(TOP_C0_AXI_BASE_60);
		print_top_reg(TOP_C0_AXI_BASE_64);
		print_top_reg(TOP_C0_AXI_BASE_68);
		print_top_reg(TOP_C0_AXI_BASE_6C);
		print_top_reg(TOP_C0_AXI_BASE_70);
		print_top_reg(TOP_C0_AXI_BASE_74);
		print_top_reg(TOP_C0_AXI_BASE_78);
		print_top_reg(TOP_C0_AXI_BASE_7C);

		print_top_reg(TOP_APB_AXI_BASE_SEL);
		print_top_reg(TOP_C1_AXI_BASE_5);
		print_top_reg(TOP_C1_AXI_BASE_60);
		print_top_reg(TOP_C1_AXI_BASE_64);
		print_top_reg(TOP_C1_AXI_BASE_68);
		print_top_reg(TOP_C1_AXI_BASE_6C);
		print_top_reg(TOP_C1_AXI_BASE_70);
		print_top_reg(TOP_C1_AXI_BASE_74);
		print_top_reg(TOP_C1_AXI_BASE_78);
		print_top_reg(TOP_C1_AXI_BASE_7C);

		print_top_reg(TOP_CPU_APB_BASE);
		print_top_reg(TOP_USB_APB_BASE);
		print_top_reg(TOP_AXI_USER);
		print_top_reg(TOP_AHB_USER);
		print_top_reg(TOP_MON_CPU_TX_MODE);
		print_top_reg(TOP_MON_CPU_TX_ADDR);
		print_top_reg(TOP_MON_CPU_TX_WD);
		print_top_reg(TOP_MON_CPU_TX_RD);

		print_top_reg(TOP_INTR_INDEX);

		print_top_reg(TOP_MEM_POWER_CTRL);
		print_top_reg(TOP_MEM_OWNER);
		print_top_reg(TOP_BACKDOOR_EN);
		print_top_reg(TOP_ACCEPT_RESP);

		print_top_reg(TOP_RENDEZVOUE_0);
		print_top_reg(TOP_RENDEZVOUE_1);
		print_top_reg(TOP_GPR_0);
		print_top_reg(TOP_GPR_1);
		print_top_reg(TOP_GPR_2);
		print_top_reg(TOP_GPR_3);
		print_top_reg(TOP_SEMA_0);
		print_top_reg(TOP_SEMA_1);
		print_top_reg(TOP_SEMA_2);
		print_top_reg(TOP_REV_NUM);

		printk(" [IPC]\n");
		print_ipc_reg(IPC_IN_LEARNING_DATA);
		print_ipc_reg(IPC_IN_PARAMETER);
		print_ipc_reg(IPC_IN_BUFFER);
		print_ipc_reg(IPC_IN_WH);
		print_ipc_reg(IPC_IN_SF);
		print_ipc_reg(IPC_OUT_BUFFER);
		print_ipc_reg(IPC_OBJ_MAX);
		print_ipc_reg(IPC_OBJ_DET);
		print_ipc_reg(IPC_OBJ_DATA);
	}
#endif
}

/*
 *
 */
//#define _VIP_TRACE_INITIAL_STEP

#if defined(_VIP_TRACE_INITIAL_STEP)
#define	_VIP_TRACE_REG_ACCESS
#define _VIP_TRACE_IPC_ACCESS
#endif

#define _vip_top(_vip, _idx)	((_vip)->regb + vip_top_reg(_idx))
#define _vip_ipc(_vip, _idx)	((_vip)->ipcb + vip_ipc_reg(_idx))

static void __used vip_top_writel_core(struct vip_dev *vip,
								unsigned int val,
								unsigned int idx)
{
	vip_writel(val, _vip_top(vip, idx));
#if defined(_VIP_TRACE_REG_ACCESS)
	vipm_debug_hal("--- (w) top[0x%04x] = 0x%08x\n", vip_top_reg(idx), val);
#endif
}

static unsigned int __used vip_top_readl_core(struct vip_dev *vip,
								unsigned int idx)
{
	unsigned int val = vip_readl(_vip_top(vip, idx));
#if defined(_VIP_TRACE_REG_ACCESS)
	vipm_debug_hal("--- (r) 0x%08x = top[0x%04x]\n", val, vip_top_reg(idx));
#endif
	return val;
}

static void __used vip_top_writel_core_multi(struct vip_dev *vip,
									void *data, int size,
									unsigned int idx)
{
	if (data)
		memcpy((void *)_vip_top(vip, idx), data, size);
}

static void __used vip_top_readl_core_multi(struct vip_dev *vip,
									void *data, int size,
									unsigned int idx)
{
	if (data)
		memcpy(data, (void *)_vip_top(vip, idx), size);
}

static void __used vip_ipc_writel_core(struct vip_dev *vip,
								unsigned int val,
								unsigned int idx)
{
	vip_writel(val, _vip_ipc(vip, idx));
#if defined(_VIP_TRACE_IPC_ACCESS)
	vipm_debug_hal("--- (w) ipc[0x%04x] = 0x%08x\n", vip_ipc_reg(idx), val);
#endif
}

static unsigned int __used vip_ipc_readl_core(struct vip_dev *vip,
								unsigned int idx)
{
	unsigned int val = vip_readl(_vip_ipc(vip, idx));
#if defined(_VIP_TRACE_IPC_ACCESS)
	vipm_debug_hal("--- (r) 0x%08x = ipc[0x%04x]\n", val, vip_ipc_reg(idx));
#endif
	return val;
}

static void __used vip_ipc_writel_core_multi(struct vip_dev *vip,
									void *data, int size,
									unsigned int idx)
{
	if (data)
		memcpy((void *)_vip_ipc(vip, idx), data, size);
}

static void __used vip_ipc_readl_core_multi(struct vip_dev *vip,
									void *data, int size,
									unsigned int idx)
{
	if (data)
		memcpy(data, (void *)_vip_ipc(vip, idx), size);
}

/*========================================================================================
	Implementation Group
========================================================================================*/
static inline int _VIP_Hal_Check(struct vip_dev *vip)
{
	if (vip == NULL) {
		vipm_err("NULL vip\n");
		return -1;
	}
	if (vip->regb == NULL) {
		vipm_err("NULL vip->regb\n");
		return -1;
	}
	return 0;
}

#if defined(VIP_GESTURE_ON)
static void _VIP_HAL_Set_Mode_H15_A0(struct vip_dev *vip, unsigned int mode)
{
	vip_ipc_writel_core(vip, mode, IPC_IN_MODE);
}

static void _VIP_HAL_Set_Res_H15_A0(struct vip_dev *vip, unsigned int res)
{
	vip_ipc_writel_core(vip, res, IPC_IN_SCREEN_SIZE);
}
#endif

static int _VIP_HAL_Set_Cmd_H15_A0(struct vip_dev *vip, int cmd)
{
	if (cmd == VIP_IPC_CMD_INIT) {
		vip_ipc_writel_core(vip, IPC_IN_CMD_INIT, IPC_IN_CMD_INFO);
		return 0;
	}
	else if (cmd == VIP_IPC_CMD_RUN) {
		vip_ipc_writel_core(vip, IPC_IN_CMD_RUN, IPC_IN_CMD_INFO);
		return 0;
	}

	return -EINVAL;
}

static void _VIP_HAL_RunTrigger_H15_A0(struct vip_dev *vip)
{
	vip_top_writel_core(vip, 0x1, TOP_INTR_CLR_0);	/* Clear Interrupt */
	vip_top_writel_core(vip, 0x1, TOP_INTR_EN_0);	/* Enable 0xC884_3010 */
	vip_top_writel_core(vip, 0x1, TOP_INTR_ST_0);	/* Trigger High Level */
	{
	#if defined(VIP_DEBUG_TOP_REG)
		__vip_debug_line();
		__vip_debug_top_reg_msg(vip, TOP_INTR_CLR_0, "Intr. CL :");
		__vip_debug_top_reg_msg(vip, TOP_INTR_EN_0, "Intr. En :");
		__vip_debug_top_reg_msg(vip, TOP_INTR_ST_0, "Intr. ST :");
		__vip_debug_line();
	#endif
	}
}

#define VIP_HAL_WAIT_INTR_MAX		(60 * 1000 * 1000)	// 60 sec
#define VIP_HAL_WAIT_INTR_INTERVAL	100					// 100 usec
static int _VIP_HAL_Wait_Intr_H15_A0(struct vip_dev *vip, int irq)
{
	unsigned long time_limit = 0;
	int time_limit_on = 0;
	unsigned int intstatus = 0;

	time_limit = VIP_HAL_WAIT_INTR_MAX / VIP_HAL_WAIT_INTR_INTERVAL;
	time_limit_on = !!(time_limit);

	vipm_debug_hal("%s\n", (irq)? "Check interrupt" : "Polling");
	while (1) {
		if (irq) {
			if (!vip_test_bit(VIP_FLAG_INIT_D, &vip->flags)) {
				break;
			}
		} else {
			intstatus = vip_top_readl_core(vip, TOP_INTR_CPU_ST);
			if (intstatus & 0x01) {
				break;
			}
		}
		if (time_limit_on) {
			time_limit--;
			if (!time_limit) {
				vipm_err("Timeout\n");
				return -ETIMEDOUT;
			}
		}
		VIP_Delay_US(VIP_HAL_WAIT_INTR_INTERVAL);
	}
	if (!irq) {
		vip_top_writel_core(vip, intstatus, TOP_INTR_CPU_CLR);
		vip_clear_bit(VIP_FLAG_INIT_D, &vip->flags);
	}

	return 0;
}

static int _VIP_HAL_FW_Init_H15_A0(struct vip_dev *vip)
{
//	struct vip_hal_req_ipc ipc_req = { 0, };
	int ret = 0;

	vip_set_bit(VIP_FLAG_INIT_D, &vip->flags);

	vip_top_writel_core(vip, VIP_IPC_CPU_APB_BASE, TOP_CPU_APB_BASE);
#if defined(VIP_GESTURE_ON)
	_VIP_HAL_Set_Mode_H15_A0(vip, 0x00);
	_VIP_HAL_Set_Res_H15_A0(vip, (1920<<16) | 1080);
#endif

	_VIP_HAL_Set_Cmd_H15_A0(vip, VIP_IPC_CMD_INIT);

#if	1
	vip_ipc_writel_core(vip, (VIP_MAX_WIDTH<<16) | VIP_MAX_HEIGHT, IPC_IN_WH);
#else
	ipc_req.iaddr = 0;
	ipc_req.iwh = (VIP_MAX_WIDTH<<16) | VIP_MAX_HEIGHT;
	ipc_req.isf = 0;
	ipc_req.oaddr = VIP_IPC_OUT_MSG_BUFFER;
	ipc_req.obj_max = VIP_MSG_MAX_OBJ_DETECT;
	ipc_req.obj_out = VIP_IPC_OBJ_DET_RESET;
	_vip_hal_set_run_ipc(vip, &ipc_req);
#endif

	_VIP_HAL_RunTrigger_H15_A0(vip);
	vipm_debug_hal("Init detector\n");

	ret = _VIP_HAL_Wait_Intr_H15_A0(vip, vip->irq);

	return ret;
}

#if defined(VIP_FIRM_DOWN_FROM_KADAP)
struct vip_firm_resource _vip_firms_h15_a0[] = {
	[0] = {
		.data		= NULL,
		.start		= 0,		//VIP_REG_FIRM_BASE,
		.size		= 0,
		.size_max	= VIP_REG_FIRM_MAX_SIZE,
		.name		= NULL,
		.prt_name	= NULL,
	},
};
#else	//__VIP_FIRM_DOWN_FROM_KADAP
struct vip_firm_resource _vip_firms_h15_a0[] = {
	[0] = {
		.data		= (void *)vip_fw_h15_a0,
		.start		= 0,		//VIP_REG_FIRM_BASE,
		.size		= VIP_FIRM_IOREMAP_SIZE(sizeof(vip_fw_h15_a0)),
		.size_max	= VIP_REG_FIRM_MAX_SIZE,
		.name		= "VIP FIRM",
		.prt_name	= (char *)vip_fw_name_h15_a0,
	},
#if !defined(VIP_GESTURE_ON)
	[1] = {
		.data		= (void *)vip_ldb_h15_a0,
		.start		= 0,		//VIP_REG_LNDB_BASE,
		.size		= VIP_FIRM_IOREMAP_SIZE(sizeof(vip_ldb_h15_a0)),
		.size_max	= VIP_REG_LNDB_MAX_SIZE,
		.name		= "VIP LNDB",
		.prt_name	= (char *)vip_ldb_name_h15_a0,
	},
#endif
};
#endif	//__VIP_FIRM_DOWN_FROM_KADAP


#define VIP_HAL_SET_C1_AXI_BASE

static struct vip_axi_set_param _vip_c0_axi_set_param_h15_a0[] = {
	{	TOP_C0_AXI_BASE_5_VAL,  TOP_C0_AXI_BASE_5	},
	{	TOP_C0_AXI_BASE_60_VAL, TOP_C0_AXI_BASE_60	},
	{	TOP_C0_AXI_BASE_64_VAL, TOP_C0_AXI_BASE_64	},
	{	TOP_C0_AXI_BASE_68_VAL, TOP_C0_AXI_BASE_68	},
	{	TOP_C0_AXI_BASE_6C_VAL, TOP_C0_AXI_BASE_6C	},
	{	TOP_C0_AXI_BASE_70_VAL, TOP_C0_AXI_BASE_70	},
	{	TOP_C0_AXI_BASE_74_VAL, TOP_C0_AXI_BASE_74	},
	{	TOP_C0_AXI_BASE_78_VAL, TOP_C0_AXI_BASE_78	},
	{	TOP_C0_AXI_BASE_7C_VAL, TOP_C0_AXI_BASE_7C	},
};

#if defined(VIP_HAL_SET_C1_AXI_BASE)
static struct vip_axi_set_param _vip_c1_axi_set_param_h15_a0[] = {
	{	TOP_C0_AXI_BASE_5_VAL,  TOP_C1_AXI_BASE_5	},
	{	TOP_C0_AXI_BASE_60_VAL, TOP_C1_AXI_BASE_60	},
	{	TOP_C0_AXI_BASE_64_VAL, TOP_C1_AXI_BASE_64	},
	{	TOP_C0_AXI_BASE_68_VAL, TOP_C1_AXI_BASE_68	},
	{	TOP_C0_AXI_BASE_6C_VAL, TOP_C1_AXI_BASE_6C	},
	{	TOP_C0_AXI_BASE_70_VAL, TOP_C1_AXI_BASE_70	},
	{	TOP_C0_AXI_BASE_74_VAL, TOP_C1_AXI_BASE_74	},
	{	TOP_C0_AXI_BASE_78_VAL, TOP_C1_AXI_BASE_78	},
	{	TOP_C0_AXI_BASE_7C_VAL, TOP_C1_AXI_BASE_7C	},
};
#endif

static void do_VIP_HAL_Set_Axi_Param_H15_A0(struct vip_dev *vip,
											struct vip_axi_set_param *prarm)
{
	unsigned int base_addr = vip->vhal_ops->cfg.base_addr;
//	unsigned int base_size = vip->vhal_ops->cfg.base_size;
	unsigned int axi60_val;

	if (base_addr >= VIP_SROM_CPU_APB_BASE) {
		prarm[0].val = vip->firm_base - VIP_SROM_CPU_APB_BASE;
		axi60_val = base_addr - VIP_SRAM_CPU_APB_BASE;
		prarm[1].val = axi60_val;
	} else {
		prarm[0].val = vip->firm_base + VIP_SROM_CPU_APB_BASE_REV;
		axi60_val = base_addr + VIP_SRAM_CPU_APB_BASE_REV;
		prarm[1].val = axi60_val;
	}
	prarm[2].val = (axi60_val - 0x04000000);
	prarm[3].val = (axi60_val - 0x08000000);
	prarm[4].val = (axi60_val - 0x0C000000);
	prarm[5].val = (axi60_val - 0x10000000);
	prarm[6].val = (axi60_val - 0x14000000);
	prarm[7].val = (axi60_val - 0x18000000);
	prarm[8].val = (axi60_val - 0x1C000000);
}

void VIP_HAL_Set_Axi_Param_H15_A0(void *vip_prev)
{
#if (__VIP_MEMORY_MAP_TYPE > 0)
	struct vip_dev *vip = vip_prev;

	vip->firm_base = vip->vhal_ops->cfg.base_addr + VIP_REG_FIRM_OFFSET;
	vip->lndb_base = vip->vhal_ops->cfg.base_addr + VIP_REG_LNDB_OFFSET;

	do_VIP_HAL_Set_Axi_Param_H15_A0(vip, _vip_c0_axi_set_param_h15_a0);
#if defined(VIP_HAL_SET_C1_AXI_BASE)
	do_VIP_HAL_Set_Axi_Param_H15_A0(vip, _vip_c1_axi_set_param_h15_a0);
#endif

#endif
}

static void _VIP_HAL_Init_Axi_H15_A0(struct vip_dev *vip)
{
	struct vip_axi_set_param *paxi_param;
	int idx;

	vipm_debug_hal("Map type : %d\n", __VIP_MEMORY_MAP_TYPE);

#if (__VIP_MEMORY_MAP_TYPE == 0)
	/* 2. RAM Partitioning */
	vip_top_writel_core(vip, H15_VIP_MEM_OWNER_BOLERO, TOP_MEM_OWNER);
#endif

	/* 3. Memory map configuration */
	paxi_param = (struct vip_axi_set_param *)_vip_c0_axi_set_param_h15_a0;
	for (idx=0 ; idx<ARRAY_SIZE(_vip_c0_axi_set_param_h15_a0) ; idx++) {
		vip_top_writel_core(vip, paxi_param->val, paxi_param->reg_idx);

		vipm_debug_hal("Reg[0x%08x] = 0x%08x\n",
					(unsigned int)_vip_top(vip, paxi_param->reg_idx),
					vip_top_readl_core(vip, paxi_param->reg_idx));
		paxi_param++;
	}
#if defined(VIP_HAL_SET_C1_AXI_BASE)
	paxi_param = (struct vip_axi_set_param *)_vip_c1_axi_set_param_h15_a0;
	for (idx=0 ; idx<ARRAY_SIZE(_vip_c1_axi_set_param_h15_a0) ; idx++) {
		vip_top_writel_core(vip, paxi_param->val, paxi_param->reg_idx);

		vipm_debug_hal("Reg[0x%08x] = 0x%08x\n",
					(unsigned int)_vip_top(vip, paxi_param->reg_idx),
					vip_top_readl_core(vip, paxi_param->reg_idx));
		paxi_param++;
	}
#endif
}

static int _VIP_HAL_Check_RevNum_H15_A0(struct vip_dev *vip)
{
	int rev_num;

	rev_num = vip_top_readl_core(vip, TOP_REV_NUM);
	if (rev_num != VIP_REV_NUM_VAL) {
		vipm_debug_hal("Wrong REV_NUM : 0x%08x\n", rev_num);
		return -ENODEV;
	}
	vipm_debug_hal("REV_NUM : 0x%08x\n", rev_num);
	return 0;
}

static int _VIP_HAL_Exit_Reg_H15_A0(struct vip_dev *vip)
{
#if 0
	vip_top_writel_core(vip, VIP_RUN_STALL_SLEEP, TOP_RUN_STALL);
	vip_top_writel_core(vip, 0x1, TOP_MEM_POWER_CTRL);
	{
	#if defined(VIP_DEBUG_TOP_REG)
		__vip_debug_line();
		__vip_debug_top_reg_msg(vip, TOP_MEM_POWER_CTRL, "Shutdown :");
		__vip_debug_line();
	#endif
	}
#endif
	return 0;
}

static int _VIP_HAL_Init_Reg_H15_A0(struct vip_dev *vip)
{
	int ret = 0;

	vip_top_writel_core(vip, 0x00000000, TOP_MEM_POWER_CTRL);

	ret = _VIP_HAL_Check_RevNum_H15_A0(vip);
	if (ret) {
		goto out;
	}

	_VIP_HAL_Init_Axi_H15_A0(vip);

out:
	return ret;
}

#define	VIP_HAL_DSP_RST_DELAY	(2000)
//#define	VIP_HAL_DSP_RST_DELAY	(1)

#define VIP_HAL_RST_TIME_MAX		(1 * 1000 * 1000)	// 1 sec
#define VIP_HAL_RST_TIME_INTERVAL	100					// 100 usec

static int _VIP_HAL_Rst_Intr_H15_A0(struct vip_dev *vip)
{
	int ret = 0;
#if 1
	unsigned long time_limit = 0;
	int time_limit_on = 0;
	int count = 0;

	vipm_debug_hal("Reset Interrupt\n");

	time_limit = VIP_HAL_RST_TIME_MAX / VIP_HAL_RST_TIME_INTERVAL;
	time_limit_on = !!(time_limit);

	while (1) {
		vip_top_writel_core(vip, 0x01, TOP_INTR_CLR_0);	/* Clear IRQ */
		if (!vip_top_readl_core(vip, TOP_INTR_ST_0)) {
			break;
		}
		if (time_limit_on) {
			count++;
			time_limit--;
			if (!time_limit) {
				vipm_err("'CPU->VIP' interrupt isn't cleared\n");
				ret = -1;
			//	break;
				goto out;
			}
		}
	//	VIP_Delay_MS(10);
		VIP_Delay_US(VIP_HAL_RST_TIME_INTERVAL);
	}
	vip_top_writel_core(vip, 0x01, TOP_INTR_EN_0);	/* Enable IRQ */
	vipm_debug_hal("Time to reset intr. : %d00 usec\n", count);
#else
	vip_top_writel_core(vip, 0x01, TOP_INTR_EN_0);	/* Enable IRQ */
	vip_top_writel_core(vip, 0x01, TOP_INTR_CLR_0);	/* Clear IRQ */
#endif
//	vipm_debug_hal("waiting : %d msec\n", VIP_HAL_DSP_RST_DELAY);
//	VIP_Delay_MS(VIP_HAL_DSP_RST_DELAY);

//	vip_top_writel_core(vip, !!(vip->irq), TOP_INTR_CPU_EN);

out:
	return ret;
}

#ifdef INCLUDE_KDRV_VER_FPGA
#define VIP_IPC_ENV_TIME_MAX		(10 * 1000 * 1000)	// 10 sec
#else
#define VIP_IPC_ENV_TIME_MAX		(1 * 1000 * 1000)	// 1 sec
#endif
#define VIP_IPC_ENV_TIME_INTERVAL	100					// 100 usec

static int _VIP_HAL_Ipc_Env_H15_A0(struct vip_dev *vip)
{
	unsigned long time_limit = 0;
	unsigned int status;
	int time_limit_on = 0;
	int count = 0;
	int ret = 0;

	vipm_debug_hal("Init IPC\n");

#if defined(_VIP_TRACE_INITIAL_STEP)
//	time_limit = 5;
	time_limit = 0;
#else
	time_limit = VIP_IPC_ENV_TIME_MAX / VIP_IPC_ENV_TIME_INTERVAL;
#endif
	time_limit_on = !!(time_limit);

	vip_top_writel_core(vip, H15_VIP_IPC_CPU_APB, TOP_CPU_APB_BASE);
	while (1) {
		status = vip_ipc_readl_core(vip, IPC_STATUS);
		if (status == VIP_IPC_STATUS_WAIT) {
			break;
		}
		if (time_limit_on) {
			count++;
			time_limit--;
			if (!time_limit) {
				vipm_err("IPC Status isn't cleared : status = %d\n", status);
			#if defined(_VIP_TRACE_INITIAL_STEP)
				break;
			#else
				ret = -1;
				goto out;
			#endif
			}
		}
	//	VIP_Delay_MS(10);
		VIP_Delay_US(VIP_IPC_ENV_TIME_INTERVAL);
	}
	vipm_debug_hal("Time to complete VIP booting : %d00 usec\n", count);

out:
	return ret;
}

#define VIP_RUN_STALL_DELAY		50

static int _VIP_HAL_Init_Env_H15_A0(struct vip_dev *vip)
{
	int ret = 0;

	vip_top_writel_core(vip, VIP_IPC_CPU_APB_BASE, TOP_CPU_APB_BASE);
	vip_ipc_writel_core(vip, 0x00, IPC_STATUS);

	/* 5. Stall & Reset */
	vipm_debug_hal("Release stall\n");
	vip_top_writel_core(vip, VIP_RUN_STALL_INIT_VAL, TOP_RUN_STALL);
	VIP_Delay_MS(VIP_RUN_STALL_DELAY);
	vipm_debug_hal("TOP_RUN_STALL[0x%p] = 0x%08x\n",
					_vip_top(vip, TOP_RUN_STALL),
					vip_top_readl_core(vip, TOP_RUN_STALL));

	/* 6. reset interrupt */
	ret = _VIP_HAL_Rst_Intr_H15_A0(vip);
	if (ret) {
		goto out;
	}

	/* 7. IPC setting */
	ret = _VIP_HAL_Ipc_Env_H15_A0(vip);
	if (ret) {
		goto out;
	}

out:
	return ret;
}

static void _VIP_HAL_GetVer_H15_A0(struct vip_dev *vip)
{
#if defined(VIP_GESTURE_ON)
	vip->lib_ver = vip_ipc_readl_core(vip, IPC_OUT_VER);
#else
	vip->lib_ver = 0x00000001;
#endif
}

static int VIP_HAL_Init_H15_A0(struct vip_dev *vip)
{
	int ret;

	__VIP_Debug_Print_Reg_Top();

	ret = _VIP_Hal_Check(vip);
	if (ret) {
		goto out;
	}

	ret = _VIP_HAL_Init_Reg_H15_A0(vip);
	if (ret) {
		vipm_err("Error in VIP_InitReg_H15_A0\n");
		goto out;
	}

	ret = VIP_HAL_Firm_Down(vip);
	if (ret) {
		goto out_reg;
	}

	ret = _VIP_HAL_Init_Env_H15_A0(vip);
	if (ret) {
		vipm_err("Error in VIP_IMG_InitEnv\n");
		goto out_firm;
	}

	{
	#if defined(VIP_DEBUG_TOP_REG)
		__vip_debug_line();
		__vip_debug_top_reg_msg(vip, TOP_MEM_OWNER, "RAM      :");

		__vip_debug_top_reg_msg(vip, TOP_RUN_STALL, "Stall    :");

		__vip_debug_top_reg_msg(vip, TOP_INTR_EN_0, "Intr. En :");
		__vip_debug_top_reg_msg(vip, TOP_INTR_ST_0, "Intr. ST :");	/* Trigger */
		__vip_debug_top_reg_msg(vip, TOP_INTR_CLR_0, "Intr. CL :");

		__vip_debug_top_reg_msg(vip, TOP_CPU_APB_BASE, "APB Base :");
		__vip_debug_line();
	#endif
	}

	ret = _VIP_HAL_FW_Init_H15_A0(vip);
	if (ret) {
		goto out_env;
	}

	_VIP_HAL_GetVer_H15_A0(vip);

	return 0;

out_env:

out_firm:
	VIP_HAL_Firm_Exit(vip);

out_reg:
	_VIP_HAL_Exit_Reg_H15_A0(vip);

out:
	return ret;
}

static int VIP_HAL_Reset_H15_A0(struct vip_dev *vip)
{
	int ret = 0;

	ret = _VIP_Hal_Check(vip);
	if (ret) {
		goto out;
	}

out:
	return ret;
}

static int VIP_HAL_Exit_H15_A0(struct vip_dev *vip)
{
	int ret;

	ret = _VIP_Hal_Check(vip);
	if (ret) {
		goto out;
	}

	VIP_HAL_Firm_Exit(vip);

	_VIP_HAL_Exit_Reg_H15_A0(vip);

out:
	return ret;
}

static int _VIP_HAL_Stall_Control_H15_A0(struct vip_dev *vip, int stall_on)
{
	unsigned int stall_param;
	int ret = 0;

	ret = _VIP_Hal_Check(vip);
	if (ret) {
		goto out;
	}

	stall_param = (stall_on)? VIP_RUN_STALL_SLEEP : VIP_RUN_STALL_RESUME;

	vip_top_writel_core(vip, stall_param, TOP_RUN_STALL);

out:
	return ret;
}

static int VIP_HAL_Sleep_H15_A0(struct vip_dev *vip)
{
	return _VIP_HAL_Stall_Control_H15_A0(vip, 1);
}

static int VIP_HAL_Resume_H15_A0(struct vip_dev *vip)
{
	return _VIP_HAL_Stall_Control_H15_A0(vip, 0);
}

static int VIP_HAL_Scene_Change_H15_A0(struct vip_dev *vip)
{
	return _VIP_HAL_FW_Init_H15_A0(vip);
}

static int VIP_HAL_SetMode_H15_A0(struct vip_dev *vip)
{
#if defined(VIP_GESTURE_ON)
	unsigned int gesture_mode;
	unsigned int eGestureMode = 0;
	unsigned int eCameraType = 0;
	unsigned int eSupportEvent = 0;

	eGestureMode = (vip->set_mode_val & 0xFF) & VIP_IN_MODE_MASK_SET_GR_OP;
	eCameraType = ((vip->set_mode_val>>8) & 0xFF) & VIP_IN_MODE_MASK_CAM_TYPE;
//	eSupportEvent = (vip->set_mode>>16) & VIP_IN_MODE_MASK_EVENT_OUT;

	gesture_mode = vip_ipc_readl_core(vip, IPC_IN_MODE);

	gesture_mode &= ~(VIP_IN_MODE_MASK_SET_GR_OP<<VIP_IN_MODE_BASE_SET_GR_OP);
	gesture_mode &= ~(VIP_IN_MODE_MASK_CAM_TYPE<<VIP_IN_MODE_BASE_CAM_TYPE);

	gesture_mode |= (eGestureMode<<VIP_IN_MODE_BASE_SET_GR_OP);
	gesture_mode |= (eCameraType<<VIP_IN_MODE_BASE_CAM_TYPE);
	gesture_mode |= (eSupportEvent<<VIP_IN_MODE_BASE_EVENT_OUT);

	vip->set_mode = gesture_mode;
	_VIP_HAL_Set_Mode_H15_A0(vip, gesture_mode);
	vipm_noti("[%s] set mode = 0x%08x\n", vip_dev_name(vip), gesture_mode);
#endif
	return 0;
}

static int VIP_HAL_SetRes_H15_A0(struct vip_dev *vip)
{
#if defined(VIP_GESTURE_ON)
	_VIP_HAL_Set_Res_H15_A0(vip, vip->set_res);
	vipm_noti("[%s] set res = w[%ld] h[%ld]\n", vip_dev_name(vip),
			(vip->set_res>>16) & 0xFFFF, vip->set_res & 0xFFFF);
#endif
	return 0;
}

/*
 *
 */
static int _VIP_HAL_Get_Axi_Base_Idx_H15_A0(struct vip_dev *vip,
									unsigned int ipc_img_src_addr,
									int base_sel)
{
	int idx = -1;
#if 1
	int addr_idx;

	addr_idx = (ipc_img_src_addr>>(24+2))<<2;
	if ((addr_idx < 0x60) || (addr_idx > 0x7C)) {
		vipm_err("Invalid : addr_idx[0x%08x] <- ipc_img_src_addr[0x%08x], base_sel(%d)\n",
				addr_idx, ipc_img_src_addr, base_sel);
		return -1;
	}
	idx = (base_sel)? TOP_C1_AXI_BASE_60 : TOP_C0_AXI_BASE_60;
	idx += ((addr_idx - 0x60)>>2);
#else
	switch (ipc_img_src_addr&0xFF000000)
	{
	case VIP_DSP_MEM_BASE_6:
	case (VIP_DSP_MEM_BASE_6+0x01000000):
	case (VIP_DSP_MEM_BASE_6+0x02000000):
	case (VIP_DSP_MEM_BASE_6+0x03000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_60 : TOP_C0_AXI_BASE_60;
		break;
	case (VIP_DSP_MEM_BASE_6+0x04000000):
	case (VIP_DSP_MEM_BASE_6+0x05000000):
	case (VIP_DSP_MEM_BASE_6+0x06000000):
	case (VIP_DSP_MEM_BASE_6+0x07000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_64 : TOP_C0_AXI_BASE_64;
		break;
	case (VIP_DSP_MEM_BASE_6+0x08000000):
	case (VIP_DSP_MEM_BASE_6+0x09000000):
	case (VIP_DSP_MEM_BASE_6+0x0A000000):
	case (VIP_DSP_MEM_BASE_6+0x0B000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_68 : TOP_C0_AXI_BASE_68;
		break;
	case (VIP_DSP_MEM_BASE_6+0x0C000000):
	case (VIP_DSP_MEM_BASE_6+0x0D000000):
	case (VIP_DSP_MEM_BASE_6+0x0E000000):
	case (VIP_DSP_MEM_BASE_6+0x0F000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_6C : TOP_C0_AXI_BASE_6C;
		break;
	case (VIP_DSP_MEM_BASE_6+0x10000000):
	case (VIP_DSP_MEM_BASE_6+0x11000000):
	case (VIP_DSP_MEM_BASE_6+0x12000000):
	case (VIP_DSP_MEM_BASE_6+0x13000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_70 : TOP_C0_AXI_BASE_70;
		break;
	case (VIP_DSP_MEM_BASE_6+0x14000000):
	case (VIP_DSP_MEM_BASE_6+0x15000000):
	case (VIP_DSP_MEM_BASE_6+0x16000000):
	case (VIP_DSP_MEM_BASE_6+0x17000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_74 : TOP_C0_AXI_BASE_74;
		break;
	case (VIP_DSP_MEM_BASE_6+0x18000000):
	case (VIP_DSP_MEM_BASE_6+0x19000000):
	case (VIP_DSP_MEM_BASE_6+0x1A000000):
	case (VIP_DSP_MEM_BASE_6+0x1B000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_78 : TOP_C0_AXI_BASE_78;
		break;
	case (VIP_DSP_MEM_BASE_6+0x1C000000):
	case (VIP_DSP_MEM_BASE_6+0x1D000000):
	case (VIP_DSP_MEM_BASE_6+0x1E000000):
	case (VIP_DSP_MEM_BASE_6+0x1F000000):
		idx = (base_sel)? TOP_C1_AXI_BASE_7C : TOP_C0_AXI_BASE_7C;
		break;
	}
#endif
	return idx;
}

static int _VIP_HAL_Run_Common_H15_A0(struct vip_req *areq)
{
	struct vip_dev *vip = areq->vip;
	struct vip_io_ifmt *iformat = &areq->ifmt_list->format;
	unsigned int phys_addr, remap_addr;
	unsigned int ipc_img_src_addr;
	unsigned int addr_h, addr_l;
	unsigned int iwh, isf;
	int axi_base_idx_0 = TOP_C0_AXI_BASE_70;
	int axi_base_idx_1 = TOP_C1_AXI_BASE_70;
	int ret = 0;

	vip_top_writel_core(vip, H15_VIP_IPC_CPU_APB, TOP_CPU_APB_BASE);
	_VIP_HAL_Set_Cmd_H15_A0(vip, VIP_IPC_CMD_RUN);
	vip_ipc_writel_core(vip, H15_VIP_LDB_DATA_ADDR, IPC_IN_META_DATA);

	phys_addr = (unsigned int)VIP_Mem_Get_Paddr(vip, areq->ibuf);
#if (__VIP_BUFFER_ALIGN_TEST > 0)
	if (areq->ibuf->bsrc == NULL) {
		phys_addr = (phys_addr + vip->buf_align) & ~(vip->buf_align-1);
		vipm_debug_hal("phys_addr[0x%08x] <- get_paddr[0x%p]\n",
					phys_addr, vip_mem_get_paddr(vip, areq->ibuf));
	}
#endif
	vipm_debug_hal("phys_addr = 0x%08x\n", phys_addr);

	ipc_img_src_addr = (areq->ipc_imgb)? (unsigned int)areq->ipc_imgb :
										H15_VIP_IPC_IMG_SRC_ADDR;
	vipm_debug_hal("ipc_img_addr = 0x%08x\n", ipc_img_src_addr);

	/*
	 * ipc_img_src_adr + remap_addr = src_img_pa
	 * 0x7000_0000 + 0xC600_000 = 0x1_3600_0000 => 0x3600_0000
	 */
	remap_addr = (phys_addr >= ipc_img_src_addr)?	\
				phys_addr - ipc_img_src_addr :		\
				phys_addr + ~(ipc_img_src_addr) + 1;
	addr_l = remap_addr & H15_VIP_IMG_LOW_MASK;
	addr_h = remap_addr & ~H15_VIP_IMG_LOW_MASK;
	vipm_debug_hal("addr_h[0x%08x] & addr_l[0x%08x | 0x%08x]\n",
				addr_h, ipc_img_src_addr, addr_l);
	addr_l |= ipc_img_src_addr;

	axi_base_idx_0 = _VIP_HAL_Get_Axi_Base_Idx_H15_A0(vip, ipc_img_src_addr, 0);
	if (axi_base_idx_0 < 0) {
		ret = -EFAULT;
		goto out;
	}
	vipm_debug_hal("reg[0x%x] <- addr_h[0x%08x]\n",
				vip_top_reg(axi_base_idx_0), addr_h);
	vip_top_writel_core(vip, addr_h, axi_base_idx_0);
#if defined(VIP_HAL_SET_C1_AXI_BASE)
	axi_base_idx_1 = _VIP_HAL_Get_Axi_Base_Idx_H15_A0(vip, ipc_img_src_addr, 1);
	if (axi_base_idx_1 < 0) {
		ret = -EFAULT;
		goto out;
	}
	vipm_debug_hal("reg[0x%x] <- addr_h[0x%08x]\n",
				vip_top_reg(axi_base_idx_1), addr_h);
	vip_top_writel_core(vip, addr_h, axi_base_idx_1);
#endif
	vip_ipc_writel_core(vip, addr_l, IPC_IN_BUFFER);

	if (!iformat->stride) {
		if (iformat->format) {	//YUV
			iformat->stride = iformat->width * 2;
		} else {				//RGB
			iformat->stride = iformat->width * 3;
		}
	}

	iwh = (iformat->width<<16) | iformat->height;
	isf = (iformat->stride<<16) | iformat->format;

	vip_ipc_writel_core(vip, iwh, IPC_IN_WH);
	vip_ipc_writel_core(vip, isf, IPC_IN_SF);

out:
	return ret;
}

static int _VIP_HAL_Run_Detect_H15_A0(struct vip_req *areq)
{
	struct vip_dev *vip;

	int obj_max = g_vip_img_msg_range[VIP_IMG_FUNC_DETECT].max;
	int ret = 0;

	vip = areq->vip;

	ret = _VIP_HAL_Run_Common_H15_A0(areq);
	if (ret) {
		goto out;
	}

	vip_ipc_writel_core(vip, H15_VIP_IPC_OUT_BUFFER, IPC_OUT_BUFFER);
	vip_ipc_writel_core(vip, obj_max, IPC_OBJ_MAX);
	vip_ipc_writel_core(vip, H15_VIP_IPC_OBJ_RST, IPC_OBJ_DET);

	_VIP_HAL_RunTrigger_H15_A0(vip);

out:
	return ret;
}

static int _VIP_HAL_Run_Gesture_H15_A0(struct vip_req *areq)
{
#if defined(VIP_GESTURE_ON)
	struct vip_dev *vip;
//	unsigned int mode;
	int ret = 0;

	vip = areq->vip;

	ret = _VIP_HAL_Run_Common_H15_A0(areq);
	if (ret) {
		goto out;
	}

	_VIP_HAL_Set_Mode_H15_A0(vip, areq->set_mode);

//	vip_ipc_writel_core(vip, H15_VIP_IPC_OUT_BUFFER, IPC_OUT_BUFFER);
//	vip_ipc_writel_core(vip, obj_max, IPC_OBJ_MAX);
//	vip_ipc_writel_core(vip, H15_VIP_IPC_OBJ_RST, IPC_OBJ_DET);

	_VIP_HAL_RunTrigger_H15_A0(vip);

out:
	return ret;

#else	//VIP_GESTURE_ON
	vipm_err("Not supported...\n");
	return -ESRCH;
#endif	//VIP_GESTURE_ON
}

static int _VIP_HAL_Msg_Detect_H15_A0(struct vip_req *areq)
{
	struct vip_dev *vip;
	unsigned int obj_max;
	unsigned int obj_num;
	unsigned int obj_unit;
	int ret = 0;

	vip = areq->vip;

	obj_max = g_vip_img_msg_range[areq->func].max;
	obj_unit = g_vip_img_msg_range[areq->func].unit;

	areq->odata = NULL;
	areq->osize = 0;
	areq->onum = 0;

	obj_num = vip_ipc_readl_core(vip, IPC_OBJ_DET);
	if (obj_num > obj_max) {
		vipm_err("%s : Wrong object no. : %d\n", __F__, obj_num);
		ret = -EFAULT;
		goto out;
	}

	areq->onum = obj_num;
	vipm_debug_hal("%d object(s) detected\n", obj_num);
	if (obj_num) {
		areq->osize = (obj_unit * obj_num);
		areq->odata = (void *)_vip_ipc(vip, IPC_OBJ_DATA);
	}

out:
	return ret;
}

static int _VIP_HAL_Msg_Gesture_H15_A0(struct vip_req *areq)
{
#if defined(VIP_GESTURE_ON)
	struct vip_dev *vip;
	struct vip_msg_gesture *gesture;
	struct vip_msg_crop *crop;
	unsigned int result;

	vip = areq->vip;

	gesture = &areq->gesture;

	result = vip_ipc_readl_core(vip, IPC_OUT_RESULT0);
	gesture->posx = (result>>VIP_OUT_RESULT1_BASE_X) & VIP_OUT_RESULT1_MASK_X;
	gesture->posy = (result>>VIP_OUT_RESULT1_BASE_Y) & VIP_OUT_RESULT1_MASK_Y;
	gesture->posx = (gesture->posx == 0xFFFF)? -1 : gesture->posx;
	gesture->posy = (gesture->posy == 0xFFFF)? -1 : gesture->posy;

	result = vip_ipc_readl_core(vip, IPC_OUT_RESULT1);
	gesture->handposx = (result>>VIP_OUT_RESULT1_BASE_X) & VIP_OUT_RESULT1_MASK_X;
	gesture->handposy = (result>>VIP_OUT_RESULT1_BASE_Y) & VIP_OUT_RESULT1_MASK_Y;
	gesture->handposx = (gesture->handposx == 0xFFFF)? -1 : gesture->handposx;
	gesture->handposy = (gesture->handposy == 0xFFFF)? -1 : gesture->handposy;

#if 1
	if ((gesture->posx == -1) &&
		(gesture->posy == -1) &&
		(gesture->handposx == -1) &&
		(gesture->handposy == -1))
	{
		gesture->osd_button_num = -1;
		gesture->click = 0;
		gesture->event_result = 0;
		gesture->gesture_alert = 0;
	}
	else
#endif
	{
		result = vip_ipc_readl_core(vip, IPC_OUT_RESULT2);
		gesture->osd_button_num = (result>>VIP_OUT_RESULT2_BASE_BUTTON) & VIP_OUT_RESULT2_MASK_BUTTON;
		gesture->osd_button_num = (gesture->osd_button_num > 5)? -1 : gesture->osd_button_num;
		gesture->click = (result>>VIP_OUT_RESULT2_BASE_CLICK) & VIP_OUT_RESULT2_MASK_CLICK;
		gesture->event_result = (result>>VIP_OUT_RESULT2_BASE_EVENT) & VIP_OUT_RESULT2_MASK_EVENT;
		gesture->gesture_alert = (result>>VIP_OUT_RESULT2_BASE_ALERT) & VIP_OUT_RESULT2_MASK_ALERT;
	}

	vipm_debug_hal("# gr : px[%3d] py[%3d] hx[%3d] hy[%3d] o[%d] c[%d] e[%d] a[%d]\n",
					gesture->posx, gesture->posy,
					gesture->handposx, gesture->handposy,
					gesture->osd_button_num, gesture->click,
					gesture->event_result, gesture->gesture_alert);

	crop = &areq->crop;

	result = vip_ipc_readl_core(vip, IPC_IN_CMD_INFO);
	crop->enable = (result>>VIP_IN_MODE_BASE_CROP_EN) & VIP_IN_MODE_MASK_CROP_EN;

	result = vip_ipc_readl_core(vip, IPC_OUT_CROP_OFFSET);
	crop->offsetx = (result>>VIP_OUT_CROP_OFF_BASE_X) & VIP_OUT_CROP_OFF_MASK_X;
	crop->offsety = (result>>VIP_OUT_CROP_OFF_BASE_Y) & VIP_OUT_CROP_OFF_MASK_Y;
	crop->offsetx = (crop->offsetx == 0xFFFF)? -1 : crop->offsetx;
	crop->offsety = (crop->offsety == 0xFFFF)? -1 : crop->offsety;

	result = vip_ipc_readl_core(vip, IPC_OUT_CROP_SIZE);
	crop->width = (result>>VIP_OUT_CROP_SIZE_BASE_W) & VIP_OUT_CROP_SIZE_MASK_W;
	crop->height = (result>>VIP_OUT_CROP_SIZE_BASE_H) & VIP_OUT_CROP_SIZE_MASK_H;
	crop->width = (crop->width == 0xFFFF)? -1 : crop->width;
	crop->height = (crop->height == 0xFFFF)? -1 : crop->height;

	vipm_debug_hal("# cr : e[%d] x[%3d] y[%3d] w[%3d] h[%3d]\n",
					crop->enable,
					crop->offsetx, crop->offsety,
					crop->width, crop->height);

	areq->onum = 1;
	areq->osize = sizeof(struct vip_msg_gesture) + sizeof(struct vip_msg_crop);
	areq->odata = (void *)&areq->gesture;
#endif
	return 0;
}

static int VIP_HAL_Run_H15_A0(struct vip_req *areq)
{
	int reset = 0;
	int scene_change = 0;
	int ret;

	ret = _VIP_Hal_Check(areq->vip);
	if (ret) {
		goto out;
	}

	vipm_debug_hal("%s\n", prt_names(areq->func, g_vip_img_func_name));

	if (VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_RESET)) {
		VIP_Clr_Areq_Flags(areq, VIP_REQ_FLAG_RESET);
		reset = 1;
		scene_change = 1;
	} else if (VIP_Test_Areq_Flags(areq, VIP_REQ_FLAG_SCENE_CHANGE)) {
		VIP_Clr_Areq_Flags(areq, VIP_REQ_FLAG_SCENE_CHANGE);
		scene_change = 1;
	}

	if (reset) {
		VIP_HAL_Reset_H15_A0(areq->vip);
	}
	if (scene_change) {
		_VIP_HAL_FW_Init_H15_A0(areq->vip);
	}

	switch (areq->func) {
		case VIP_IMG_FUNC_DETECT :
			vipm_debug_hal("Face Detection\n");
			ret = _VIP_HAL_Run_Detect_H15_A0(areq);
			break;
		case VIP_IMG_FUNC_GESTURE :
			vipm_debug_hal("Finger Gesture\n");
			ret = _VIP_HAL_Run_Gesture_H15_A0(areq);
			break;
		default :
			vipm_err("Invalid func : [%d]\n", areq->func);
			ret = -EINVAL;
			break;
	}

out:
	return ret;
}

static int VIP_HAL_Msg_H15_A0(struct vip_req *areq)
{
	int ret;

	if (areq->fault || areq->error) {
		vipm_err("Error in result : fault[%d] error[%d]\n",
						areq->fault, areq->error);
		return -EIO;
	}

	ret = _VIP_Hal_Check(areq->vip);
	if (ret) {
		goto out;
	}

	switch (areq->func) {
		case VIP_IMG_FUNC_DETECT :
			vipm_debug_hal("Face Detection\n");
			ret = _VIP_HAL_Msg_Detect_H15_A0(areq);
			break;
		case VIP_IMG_FUNC_GESTURE :
			ret = _VIP_HAL_Msg_Gesture_H15_A0(areq);
			break;
		default :
			vipm_err("Invalid func : [%d]\n", areq->func);
			ret = -EINVAL;
			break;
	}

out:
	return ret;
}

static int VIP_HAL_Poll_H15_A0(struct vip_req *areq)
{
	int ret;

	if (areq->fault || areq->error) {
		vipm_err("Error in result : fault[%d] error[%d]\n",
						areq->fault, areq->error);
		return -EBUSY;
	}

	ret = _VIP_Hal_Check(areq->vip);
	if (ret) {
		goto out;
	}

	if (areq->func == VIP_IMG_FUNC_DETECT) {
		ret = vip_ipc_readl_core(areq->vip, IPC_OBJ_DET);
		if (ret == H15_VIP_IPC_OBJ_RST) {
			return -EBUSY;
		}
	} else if (areq->func == VIP_IMG_FUNC_GESTURE) {
		ret = vip_ipc_readl_core(areq->vip, IPC_IN_CMD_INFO);
		ret >>= VIP_IN_CMD_BASE_RUN_IDLE;
		ret &= VIP_IN_CMD_MASK_RUN_IDLE;
		if (ret) {
			return -EBUSY;
		}
	} else {
		vipm_err("Invalid func : [%d]\n", areq->func);
		return -EINVAL;
	}

	vipm_debug_hal("Poll result : 0x%08x\n", ret);

	{
	#if defined(VIP_DEBUG_TOP_REG)
		struct vip_dev *vip = areq->vip;

		__vip_debug_line();
		__vip_debug_top_reg_msg(vip, TOP_INTR_EN_0, "Intr. En :");
		__vip_debug_top_reg_msg(vip, TOP_INTR_CLR_0, "Intr. CL :");
		__vip_debug_top_reg_msg(vip, TOP_INTR_ST_0, "Intr. ST :");	/* Trigger */
		__vip_debug_line();
	#endif
	}

out:
	return ret;
}

const struct vip_hal_ops g_vip_hal_ops_h15_a0 = {
	.name		= "VIP HAL OPS - H15 A0",
	/* Param : vip */
	.init			= VIP_HAL_Init_H15_A0,
	.exit			= VIP_HAL_Exit_H15_A0,
	.reset			= VIP_HAL_Reset_H15_A0,
	/* Param : vip */
	.sleep			= VIP_HAL_Sleep_H15_A0,
	.resume			= VIP_HAL_Resume_H15_A0,
	.scene_change	= VIP_HAL_Scene_Change_H15_A0,
	/* Param : vip */
	.setmode		= VIP_HAL_SetMode_H15_A0,
	.setres			= VIP_HAL_SetRes_H15_A0,
	/* Param : areq */
	.run			= VIP_HAL_Run_H15_A0,
	.msg			= VIP_HAL_Msg_H15_A0,
	.poll			= VIP_HAL_Poll_H15_A0,
	/* Param : vip */
	.top_writel		= vip_top_writel_core,
	.top_readl		= vip_top_readl_core,
	.ipc_writel		= vip_ipc_writel_core,
	.ipc_readl		= vip_ipc_readl_core,
	/* */
	.cfg = {
		.base_addr	= 0,
		.base_size	= 0,
		/* */
		.firmware	= (void *)_vip_firms_h15_a0,
		.load_fw	= 1,
		.load_ldb	= 1,
		/* */
		.ipc_imgb	= (void *)H15_VIP_IPC_IMG_SRC_ADDR,
		.imgb		= NULL,
	},
};

void *VIP_HAL_Get_Ops_H15_A0(void *vip_priv)
{
	return (void *)&g_vip_hal_ops_h15_a0;
}


