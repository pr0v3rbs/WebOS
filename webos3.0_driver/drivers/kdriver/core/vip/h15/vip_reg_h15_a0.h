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

#ifndef _VIP_REG_H15_A0_H_
#define _VIP_REG_H15_A0_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */
/* CTOP */
#define VIP_CTOP_REG_BASE			0xC898E400
#define VIP_CTOP_REG_SIZE			0x100
#define VIP_CTOP_REG_CTR00			0x00
#define VIP_CTOP_REG_CTR16			0x40
#define VIP_CTOP_REG_CTR19			0x4C
#define VIP_CTOP_REG_CTR24			0x60
#define VIP_CTOP_REG_CTR30			0x78
#define VIP_CTOP_REG_CTR31			0x7C

/* Core register area */
#define	VIP_REG_CORE_BASE			(0xC8840000)
#define	VIP_REG_CORE_SIZE			(0x4000)
#define	VIP_REG_CORE_OFFSET			(0x3000)
#define	VIP_REG_CORE_END			(VIP_REG_CORE_BASE + VIP_REG_CORE_SIZE - 1)

#define	VIP_IRQ_INDEX				(32 + 68)

/*
 * Table Map - VIP view
 */
#define VIP_DRAM0_CPU_APB_BASE			0x3FF70000
#define VIP_DRAM0_CPU_APB_SIZE			0x10000		/* 64KB */
#define VIP_DRAM1_CPU_APB_BASE			0x3FFF0000
#define VIP_DRAM0_CPU_APB_SIZE			0x10000		/* 64KB */
#define VIP_IRAM_CPU_APB_BASE			0x40000000
#define VIP_IRAM_CPU_APB_SIZE			0x4000		/* 16KB */
#define VIP_SROM_CPU_APB_BASE			0x50000000
#define VIP_SROM_CPU_APB_SIZE			0x1000000	/* 16MB */
#define VIP_SRAM_CPU_APB_BASE			0x60000000
#define VIP_SRAM_CPU_APB_SIZE			0x20000000	/* 512MB */
#define VIP_L2_SCRATCH_CPU_APB_BASE		0xE0000000
#define VIP_L2_SCRATCH_CPU_APB_SIZE		0x20000		/* 128KB */
#define VIP_UART_CPU_APB_BASE			0xF0000000
#define VIP_UART_CPU_APB_SIZE			0x1000		/* 4KB */
#define	VIP_IPC_CPU_APB_BASE			0xF0001000
#define VIP_IPC_CPU_APB_SIZE			0x1000		/* 4KB */
#define VIP_RC_CPU_APB_BASE				0xF0002000
#define VIP_RC_CPU_APB_SIZE				0x1000		/* 4KB */
#define VIP_USB_CPU_APB_BASE			0xF0800000
#define VIP_USB_CPU_APB_SIZE			0x800000	/* 8MB */

#define VIP_MEMORY_SIZE				0x4000000	//64MB

#if (__VIP_MEMORY_MAP_TYPE == 8)
#define VIP_MEMORY_BASE				0x74400000	//Pioneer
#else
#define VIP_MEMORY_BASE				0x34000000	//FPGA
#endif

#if (__VIP_MEMORY_MAP_TYPE == 0)	//Old
#define VIP_REG_FIRM_OFFSET			(0)
#define VIP_REG_LNDB_OFFSET			(0x03D00000)
#define VIP_REG_FIRM_BASE			(VIP_MEMORY_BASE + VIP_REG_FIRM_OFFSET)
#define	VIP_REG_LNDB_BASE			(VIP_MEMORY_BASE + VIP_REG_LNDB_OFFSET)
#define VIP_REG_FIRM_MAX_SIZE		(0x02000000)
#define VIP_REG_LNDB_MAX_SIZE		(VIP_MEMORY_SIZE - VIP_REG_LNDB_OFFSET)

#else
#define VIP_REG_FIRM_OFFSET			(0x03E00000)
#define VIP_REG_LNDB_OFFSET			(0x03C00000)
#define VIP_REG_FIRM_MAX_SIZE		(VIP_MEMORY_SIZE - VIP_REG_FIRM_OFFSET)
#define VIP_REG_LNDB_MAX_SIZE		(VIP_MEMORY_SIZE - VIP_REG_LNDB_OFFSET)
#endif

#define VIP_SROM_CPU_APB_BASE_REV	0xB0000000
#define VIP_SRAM_CPU_APB_BASE_REV	0xA0000000

#if 0
#if (__VIP_MEMORY_MAP_TYPE == 0)	//Old
#define TOP_C0_AXI_BASE_5_VAL		0xE4000000
#define TOP_C0_AXI_BASE_60_VAL		0xD4100000
#define TOP_C0_AXI_BASE_64_VAL		0xD4100000
#define TOP_C0_AXI_BASE_68_VAL		0xD4100000
#define TOP_C0_AXI_BASE_6C_VAL		0xD4100000
#define TOP_C0_AXI_BASE_70_VAL		0xD4100000
#define TOP_C0_AXI_BASE_74_VAL		0xD4100000
#define TOP_C0_AXI_BASE_78_VAL		0xD4100000
#define TOP_C0_AXI_BASE_7C_VAL		0xD4100000
#else

#if (VIP_MEMORY_BASE >= VIP_SROM_CPU_APB_BASE)
#define TOP_C0_AXI_BASE_5_VAL		(VIP_REG_FIRM_BASE - VIP_SROM_CPU_APB_BASE)
#else
// base + (0x1_0000_0000 - 0x5000_0000)
#define TOP_C0_AXI_BASE_5_VAL		(VIP_REG_FIRM_BASE + VIP_SROM_CPU_APB_BASE_REV)
#endif

#if (VIP_MEMORY_BASE >= VIP_SRAM_CPU_APB_BASE)
#define TOP_C0_AXI_BASE_60_VAL		(VIP_MEMORY_BASE - VIP_SRAM_CPU_APB_BASE)
#else
// base + (0x1_0000_0000 - 0x6000_0000)
#define TOP_C0_AXI_BASE_60_VAL		(VIP_MEMORY_BASE + VIP_SRAM_CPU_APB_BASE_REV)
#endif

#define TOP_C0_AXI_BASE_64_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x04000000)
#define TOP_C0_AXI_BASE_68_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x08000000)
#define TOP_C0_AXI_BASE_6C_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x0C000000)
#define TOP_C0_AXI_BASE_70_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x10000000)
#define TOP_C0_AXI_BASE_74_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x14000000)
#define TOP_C0_AXI_BASE_78_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x18000000)
#define TOP_C0_AXI_BASE_7C_VAL		(TOP_C0_AXI_BASE_60_VAL - 0x1C000000)
#endif

#else

#define TOP_C0_AXI_BASE_5_VAL		0
#define TOP_C0_AXI_BASE_60_VAL		0
#define TOP_C0_AXI_BASE_64_VAL		0
#define TOP_C0_AXI_BASE_68_VAL		0
#define TOP_C0_AXI_BASE_6C_VAL		0
#define TOP_C0_AXI_BASE_70_VAL		0
#define TOP_C0_AXI_BASE_74_VAL		0
#define TOP_C0_AXI_BASE_78_VAL		0
#define TOP_C0_AXI_BASE_7C_VAL		0

#endif

/*-----------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/*
 *
 */
enum VIP_REG_TOP_IDX_GRP_0 {
	TOP_RUN_STALL = 0x00,	/* 0x00 => 0x00 */
	TOP_PRID,
	TOP_PWAIT,
	TOP_RSVD_3,
	TOP_INTR_EN_0,
	TOP_INTR_ST_0,
	TOP_INTR_CLR_0,
	TOP_INTR_EN_1,
	TOP_INTR_ST_1,			/* 0x08 => 0x20 */
	TOP_INTR_CLR_1,
	TOP_NMI_EN,
	TOP_NMI_ST,
	TOP_NMI_CLR,
	TOP_INTR_CPU_EN,
	TOP_INTR_CPU_ST,
	TOP_INTR_CPU_CLR,
};

#if 1
#define VIP_RUN_STALL_INIT_VAL		0x00
#define VIP_RUN_STALL_OFF			0x00
#define VIP_RUN_STALL_SLEEP			0x01
#define VIP_RUN_STALL_RESUME		VIP_RUN_STALL_INIT_VAL
#else
enum {
	VIP_RUN_STALL_CORE0 = 0,
	VIP_RUN_STALL_CORE1,
};

#define VIP_RUN_STALL_BIT_CORE0		BIT(VIP_RUN_STALL_CORE0)
#define VIP_RUN_STALL_BIT_CORE1		BIT(VIP_RUN_STALL_CORE1)

#define VIP_RUN_STALL_INIT_VAL		(~VIP_RUN_STALL_BIT_CORE0 & VIP_RUN_STALL_BIT_CORE1)
#define VIP_RUN_STALL_OFF			0x00
#define VIP_RUN_STALL_SLEEP			VIP_RUN_STALL_BIT_CORE0
//#define VIP_RUN_STALL_RESUME		VIP_RUN_STALL_OFF
#define VIP_RUN_STALL_RESUME		VIP_RUN_STALL_INIT_VAL
#endif

#define VIP_INTR_CPU_ST_MASK		0x0000FFFF
#define VIP_INTR_CPU_ST_ID_MASK		0xFFFF0000
#define VIP_INTR_CPU_ST_ID_OFFSET	16

enum VIP_REG_TOP_IDX_GRP_1 {
	TOP_C0_AXI_BASE_5 = 0x10,	/* 16 => 0x40 */
	TOP_C0_AXI_BASE_60,
	TOP_C0_AXI_BASE_64,
	TOP_C0_AXI_BASE_68,
	TOP_C0_AXI_BASE_6C,
	TOP_C0_AXI_BASE_70,
	TOP_C0_AXI_BASE_74,
	TOP_C0_AXI_BASE_78,
	TOP_C0_AXI_BASE_7C,
};

enum VIP_REG_TOP_IDX_GRP_2 {
	TOP_APB_AXI_BASE_SEL = 0x1F,	/* 31 => 0x7C */
	TOP_C1_AXI_BASE_5,
	TOP_C1_AXI_BASE_60,
	TOP_C1_AXI_BASE_64,
	TOP_C1_AXI_BASE_68,
	TOP_C1_AXI_BASE_6C,
	TOP_C1_AXI_BASE_70,
	TOP_C1_AXI_BASE_74,
	TOP_C1_AXI_BASE_78,
	TOP_C1_AXI_BASE_7C,
};

enum VIP_REG_TOP_IDX_GRP_3 {
	TOP_CPU_APB_BASE = 0x30,	/* 0x30 => 0xC0 */
	TOP_USB_APB_BASE,
	TOP_AXI_USER,
	TOP_AHB_USER,
	TOP_MON_CPU_TX_MODE,
	TOP_MON_CPU_TX_ADDR,
	TOP_MON_CPU_TX_WD,
	TOP_MON_CPU_TX_RD,
};

enum VIP_REG_TOP_IDX_GRP_4 {
	TOP_INTR_INDEX = 0x40,		/* 0x40 => 0x100 */
};

enum VIP_REG_TOP_IDX_GRP_5 {
	TOP_MEM_POWER_CTRL = 0x48,	/* 0x48 => 0x120 */
};

enum VIP_REG_TOP_IDX_GRP_6 {
	TOP_MEM_OWNER = 0x50,		/* 0x50 => 0x140 */
	TOP_BACKDOOR_EN,
	TOP_ACCEPT_RESP,
};

enum VIP_REG_TOP_IDX_GRP_7 {
	TOP_RENDEZVOUE_0 = 0x3F4,	/* 0x3F4 => 0xFD0 */
	TOP_RENDEZVOUE_1,
};

enum VIP_REG_TOP_IDX_GRP_8 {
	TOP_GPR_0 = 0x3F8,			/* 0x3F8 => 0xFE0 */
	TOP_GPR_1,
	TOP_GPR_2,
	TOP_GPR_3,
	TOP_SEMA_0,
	TOP_SEMA_1,
	TOP_SEMA_2,
	TOP_REV_NUM,
};

#define VIP_REV_NUM_VAL			0x20140224

#define VIP_REG_TOP_BASE(idx)	(0x3000+((idx)<<2))
#define vip_top_reg(idx)		VIP_REG_TOP_BASE(idx)

/*
 *
 */
#define VIP_IN_CMD_BASE_RUNNING_MODE	4	//[07:04]
#define VIP_IN_CMD_BASE_RUN_IDLE		0	//[00:00]
#define VIP_IN_CMD_MASK_RUNNING_MODE	0x0F
#define VIP_IN_CMD_MASK_RUN_IDLE		0x01

#define VIP_IN_MODE_BASE_CAM_TYPE		16	//[19:16]
#define VIP_IN_MODE_BASE_EVENT_OUT		12	//[12:12]
#define VIP_IN_MODE_BASE_CROP_EN		8	//[08:08]
#define VIP_IN_MODE_BASE_SET_GR_OP		0	//[00:00]
#define VIP_IN_MODE_MASK_CAM_TYPE		0x0F
#define VIP_IN_MODE_MASK_EVENT_OUT		0x01
#define VIP_IN_MODE_MASK_CROP_EN		0x01
#define VIP_IN_MODE_MASK_SET_GR_OP		0x01

#define VIP_OUT_VER_BASE_YEAR			16
#define VIP_OUT_VER_BASE_MON			8
#define VIP_OUT_VER_BASE_DAY			0

#define VIP_OUT_RESULT0_BASE_X			16
#define VIP_OUT_RESULT0_BASE_Y			0
#define VIP_OUT_RESULT0_MASK_X			0xFFFF
#define VIP_OUT_RESULT0_MASK_Y			0xFFFF

#define VIP_OUT_RESULT1_BASE_X			16
#define VIP_OUT_RESULT1_BASE_Y			0
#define VIP_OUT_RESULT1_MASK_X			0xFFFF
#define VIP_OUT_RESULT1_MASK_Y			0xFFFF

#define VIP_OUT_RESULT2_BASE_ALERT		20	//[23:20]
#define VIP_OUT_RESULT2_BASE_EVENT		16	//[19:16]
#define VIP_OUT_RESULT2_BASE_CLICK		8	//[08:08]
#define VIP_OUT_RESULT2_BASE_BUTTON		0	//[03:00]
#define VIP_OUT_RESULT2_MASK_ALERT		0x0F
#define VIP_OUT_RESULT2_MASK_EVENT		0x0F
#define VIP_OUT_RESULT2_MASK_CLICK		0x01
#define VIP_OUT_RESULT2_MASK_BUTTON		0x0F

#define VIP_OUT_CROP_OFF_BASE_X			16
#define VIP_OUT_CROP_OFF_BASE_Y			0
#define VIP_OUT_CROP_OFF_MASK_X			0xFFFF
#define VIP_OUT_CROP_OFF_MASK_Y			0xFFFF

#define VIP_OUT_CROP_SIZE_BASE_W		16
#define VIP_OUT_CROP_SIZE_BASE_H		0
#define VIP_OUT_CROP_SIZE_MASK_W		0xFFFF
#define VIP_OUT_CROP_SIZE_MASK_H		0xFFFF

enum VIP_IPC_CMD_VAL {
	VIP_IPC_CMD_INIT = 0,
	VIP_IPC_CMD_RUN,
	VIP_IPC_CMD_MAX,
};

#if defined(VIP_GESTURE_ON)

#define IPC_IN_CMD_INIT		0x01
#define IPC_IN_CMD_RUN		0x11

enum VIP_REG_IPC_IDX_GRP_0 {
	IPC_IN_CMD_INFO = 0x00,
	IPC_IN_MODE,
	IPC_IN_META_DATA,
	/* */
	IPC_IN_BUFFER,
	IPC_IN_WH,
	IPC_IN_SF,
	/* */
	IPC_OUT_VER,
	IPC_OUT_RESULT0,
	IPC_OUT_RESULT1,
	IPC_OUT_RESULT2,
	/* */
	IPC_OUT_CROP_OFFSET,
	IPC_OUT_CROP_SIZE,
	/* */
	IPC_IN_SCREEN_SIZE,
};

enum VIP_REG_IPC_IDX_GRP_1 {	// for FD (old version)
	IPC_OUT_BUFFER = 5,
	IPC_OBJ_MAX,
	IPC_OBJ_DET,
	/* */
	IPC_OBJ_DATA,
};

enum VIP_REG_IPC_IDX_GRP_2 {
//	IPC_STATUS			= 0x18,	/* 0x18 => 0x60 */	//Temp
	IPC_STATUS			= 0x3C,	/* 0x3C => 0xF0 */
};

#else	//__VIP_GESTURE_ON

#define IPC_IN_CMD_INIT		0x00
#define IPC_IN_CMD_RUN		0x01

enum VIP_REG_IPC_IDX_GRP_0 {
	IPC_IN_CMD_INFO = 0x00,
	IPC_IN_META_DATA,
	/* */
	IPC_IN_BUFFER,
	IPC_IN_WH,
	IPC_IN_SF,
	/* */
	IPC_OUT_BUFFER,
	IPC_OBJ_MAX,
	IPC_OBJ_DET,
	/* */
	IPC_OBJ_DATA,
};

enum VIP_REG_IPC_IDX_GRP_1 {
	IPC_STATUS			= 0x3C,	/* 0x3C => 0xF0 */
};

#endif	//__VIP_GESTURE_ON


enum VIP_IPC_STATUS {
	VIP_IPC_STATUS_RSVD = 0,
	VIP_IPC_STATUS_WAIT,
	VIP_IPC_STATUS_PROCESSING,
	VIP_IPC_STATUS_MAX,
};

#define	VIP_REG_IPC_BASE(idx)	(0x0000+((idx)<<2))
#define	vip_ipc_reg(idx)		VIP_REG_IPC_BASE(idx)

#if	0
struct vip_reg_h15
{
#if	1
	unsigned int	__rsvd_0000_2FFC[0x3000>>2];
#endif
	unsigned int	run_stall;							// 0x000 : from (0x3884_0000 + 0x3000)
	unsigned int	prid;								// 0x004
	unsigned int	pwait;								// 0x008
	unsigned int	__rsvd_00C;							// 0x00C
	unsigned int	intr_en_0;							// 0x010
	unsigned int	intr_st_0;							// 0x014
	unsigned int	intr_clr_0;							// 0x018
	unsigned int	intr_en_1;							// 0x01C
	unsigned int	intr_st_1;							// 0x020
	unsigned int	intr_clr_1;							// 0x024
	unsigned int	nmi_en;								// 0x028
	unsigned int	nmi_st;								// 0x02C
	unsigned int	nmi_clr;							// 0x030
	unsigned int	intr_cpu_en;						// 0x034
	unsigned int	intr_cpu_st;						// 0x038
	unsigned int	intr_cpu_clr;						// 0x03C
	unsigned int	c0_axi_base_5;						// 0x040
	unsigned int	c0_axi_base_60;						// 0x044
	unsigned int	c0_axi_base_64;						// 0x048
	unsigned int	c0_axi_base_68;						// 0x04C
	unsigned int	c0_axi_base_6C;						// 0x050
	unsigned int	c0_axi_base_70;						// 0x054
	unsigned int	c0_axi_base_74;						// 0x058
	unsigned int	c0_axi_base_78;						// 0x05C
	unsigned int	c0_axi_base_7C;						// 0x060
	unsigned int	__rsvd_064_078[(0x7C-0x64)>>2];		// 0x064 ~ 0x078
	unsigned int	apb_axi_base_sel;					// 0x07C
	unsigned int	c1_axi_base_5;						// 0x080
	unsigned int	c1_axi_base_60;						// 0x084
	unsigned int	c1_axi_base_64;						// 0x088
	unsigned int	c1_axi_base_68;						// 0x08C
	unsigned int	c1_axi_base_6C;						// 0x090
	unsigned int	c1_axi_base_70;						// 0x094
	unsigned int	c1_axi_base_74;						// 0x098
	unsigned int	c1_axi_base_78;						// 0x09C
	unsigned int	c1_axi_base_7C;						// 0x0A0
	unsigned int	__rsvd_0A4_0BC[(0xC0-0xA4)>>2];		// 0x0A4 ~ 0x0BC
	unsigned int	cpu_apb_base;						// 0x0C0
	unsigned int	usb_apb_base;						// 0x0C4
	unsigned int	axi_user;							// 0x0C8
	unsigned int	ahb_user;							// 0x0CC
	unsigned int	mon_cpu_tx_mode;					// 0x0D0
	unsigned int	mon_cpu_tx_addr;					// 0x0D4
	unsigned int	mon_cpu_tx_wd;						// 0x0D8
	unsigned int	mon_cpu_tx_rd;						// 0x0DC
	unsigned int	_rsvd_0E0_0FC[(0x100-0xE0)>>2];		// 0x0E0 ~ 0x0FC
	unsigned int	intr_index;							// 0x100
	unsigned int	__rsvd_104_12C[(0x120-0x104)>>2];	// 0x104 ~ 0x11C
	unsigned int	mem_power_ctrl;						// 0x120
	unsigned int	__rsvd_104_12C[(0x140-0x124)>>2];	// 0x124 ~ 0x13C
	unsigned int	mem_owner;							// 0x140
	unsigned int	backdoor_en;						// 0x144
	unsigned int	accept_resp;						// 0x148
	unsigned int	__rsvd_14C_FCC[(0xFD0-0x14C)>>2];	// 0x14C ~ 0xFCC
	unsigned int	rendezvoue_0;						// 0xFD0
	unsigned int	rendezvoue_0;						// 0xFD4
	unsigned int	__rsvd_FD8_FDC[(0xFE0-0xFD8)>>2];	// 0xFD8 ~ 0xFDC
	unsigned int	gpr_0;								// 0xFE0
	unsigned int	gpr_1;								// 0xFE4
	unsigned int	gpr_2;								// 0xFE8
	unsigned int	gpr_3;								// 0xFEC
	unsigned int	sema_0;								// 0xFF0
	unsigned int	sema_1;								// 0xFF4
	unsigned int	sema_2;								// 0xFF8
	unsigned int	rev_num;							// 0xFFC
};

struct vip_ipc_h15
{
	unsigned int	in_learn_data;
	unsigned int	in_parameter;
	/* */
	unsigned int	in_buffer;
	unsigned int	in_width_height;
	unsigned int	in_stride_format;
	/* */
	unsigned int	out_buffer;
	unsigned int	obj_max;
	unsigned int	obj_detected;
	/* */
	unsigned char	obj_data[0x100-0x20];
};
#endif

#define VIP_LDB_DATA_ADDR				0x63C00000

/*
 *
 */
#define	VIP_IPC_CPU_OUT_BASE	(VIP_IPC_CPU_APB_BASE + 0x20)

#define	VIP_IPC_OUT_MSG_BUFFER	VIP_IPC_CPU_OUT_BASE

#define	VIP_IPC_OBJ_DET_RESET	(-1)

#define VIP_IPC_IMG_SRC_ADDR	0x70000000
#define	VIP_IPC_IMG_LOW_MASK	0x000FFFFF


/*
 *
 */
#define VIP_MEM_OWNER_CORE0		0x00000000
#define VIP_MEM_OWNER_CORE1		0x00111111
#define VIP_MEM_OWNER_DUET		0x00010011
#define VIP_MEM_OWNER_BOLERO	0x00010001	/* 3/4 to core0 */

/*
 *
 */
#define	VIP_IMG_BUF_ADDR		(VIP_MEMORY_BASE + 0x02000000)

/*
 *
 */
#define H15_VIP_IPC_IMG_SRC_ADDR	VIP_IPC_IMG_SRC_ADDR
#define	H15_VIP_IMG_LOW_MASK		VIP_IPC_IMG_LOW_MASK

#define	H15_VIP_IPC_CPU_APB			VIP_IPC_CPU_APB_BASE
#define H15_VIP_CPU_OUT_BASE		VIP_IPC_CPU_OUT_BASE

#define H15_VIP_LDB_DATA_ADDR		VIP_LDB_DATA_ADDR

#define	H15_VIP_IPC_OUT_BUFFER		VIP_IPC_CPU_OUT_BASE

#define	H15_VIP_IPC_OBJ_RST			VIP_IPC_OBJ_DET_RESET

#define	H15_VIP_IMG_BUF_ADDR		VIP_IMG_BUF_ADDR

#define H15_VIP_MEM_OWNER_CORE0		VIP_MEM_OWNER_CORE0
#define H15_VIP_MEM_OWNER_CORE1		VIP_MEM_OWNER_CORE1
#define H15_VIP_MEM_OWNER_DUET		VIP_MEM_OWNER_DUET
#define H15_VIP_MEM_OWNER_BOLERO	VIP_MEM_OWNER_BOLERO	/* 3/4 to core0 */


#ifdef __cplusplus
}
#endif

#endif	/* _VIP_REG_H15_A0_H_ */



