/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

#ifndef _REG_IPC_H13B0_h
#define _REG_IPC_H13B0_h

/*-----------------------------------------------------------------------------
	0x4e00 set_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 run_stall                   : 1;   //     0
	UINT32 start_vector_sel            : 1;   //     1
	UINT32 p_debug_enable              : 1;   //     2
	UINT32                             : 21;  //  3:23 reserved
	UINT32 sw_reset_fdone              : 1;   //    24
	UINT32                             : 3;   // 25:27 reserved
	UINT32 sw_reset                    : 1;   //    28
} H13B0_SET_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x4e04 set_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 prid                        : 16;  //  0:15
} H13B0_SET_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x4e08 ext_intr_event ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_arm     : 30;  //  0:29
	UINT32 edma_interrupt_event_arm    : 1;   //    30
	UINT32 wdg_interrupt_event         : 1;   //    31
} H13B0_EXT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
	0x4e0c ext_intr_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_arm    : 30;  //  0:29
	UINT32 edma_interrupt_enable_arm   : 1;   //    30
	UINT32 wdg_interrupt_enable        : 1;   //    31
} H13B0_EXT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
	0x4e10 ext_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_status_arm    : 30;  //  0:29
	UINT32 edma_interrupt_status_arm   : 1;   //    30
	UINT32 wdg_interrupt_status        : 1;   //    31
} H13B0_EXT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e14 ext_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_arm     : 30;  //  0:29
	UINT32 edma_interrupt_clear_arm    : 1;   //    30
	UINT32 wdg_interrupt_clear         : 1;   //    31
} H13B0_EXT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
	0x4e18 int_intr_event ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_event_mcu    : 1;   //    30
} H13B0_INT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
	0x4e1c int_intr_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_mcu    : 30;  //  0:29
	UINT32 edma_interrupt_enable_mcu   : 1;   //    30
} H13B0_INT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
	0x4e20 int_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_staus_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_status_mcu   : 1;   //    30
} H13B0_INT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e24 int_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_clear_mcu    : 1;   //    30
} H13B0_INT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
	0x4e28 srom_boot_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset             : 32;  //  0:31
} H13B0_SROM_BOOT_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e2c atlas_port_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram2_atlas_port_sel        : 3;   //  0: 2
	UINT32                             : 1;   //     3 reserved
	UINT32 sram1_atlas_port_sel        : 3;   //  4: 6
	UINT32                             : 1;   //     7 reserved
	UINT32 sram0_atlas_port_sel        : 3;   //  8:10
	UINT32                             : 1;   //    11 reserved
	UINT32 edma_atlas_port_sel         : 3;   // 12:14
} H13B0_ATLAS_PORT_SEL_T;

/*-----------------------------------------------------------------------------
	0x4e30 pdebug_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_status               : 8;   //  0: 7
} H13B0_PDEBUG_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e34 pdebug_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_data                 : 32;  //  0:31
} H13B0_PDEBUG_DATA_T;

/*-----------------------------------------------------------------------------
	0x4e38 pdebug_pc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_pc                   : 32;  //  0:31
} H13B0_PDEBUG_PC_T;

/*-----------------------------------------------------------------------------
	0x4e3c axi_user_id ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 axi_user_id                 : 3;   //  0: 2
} H13B0_AXI_USER_ID_T;

/*-----------------------------------------------------------------------------
	0x4e40 sram0_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram0_atlas_offset          : 32;  //  0:31
} H13B0_SRAM0_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e44 sram1_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram1_atlas_offset          : 32;  //  0:31
} H13B0_SRAM1_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e48 sram2_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram2_atlas_offset          : 32;  //  0:31
} H13B0_SRAM2_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e4c srom_boot_offset2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset2            : 32;  //  0:31
} H13B0_SROM_BOOT_OFFSET2_T;

/*-----------------------------------------------------------------------------
	0x4e5c spp_mcu_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spp_mcu_version_f           : 32;   // 31: 0
} H13B0_SPP_MCU_VERSION_T;

/*-----------------------------------------------------------------------------
	0x4f00 tx_locked_id ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_locked_id                : 32;  //  0:31
} H13B0_TX_LOCKED_ID_T;

/*-----------------------------------------------------------------------------
	0x4f04 tx_src_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_src_addr                 : 32;  //  0:31
} H13B0_TX_SRC_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f08 tx_dst_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_dst_addr                 : 32;  //  0:31
} H13B0_TX_DST_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f0c tx_nxt_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_nxt_addr                 : 32;  //  0:31
} H13B0_TX_NXT_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f10 tx_length ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_length                   : 32;  //  0:31
} H13B0_TX_LENGTH_T;

/*-----------------------------------------------------------------------------
	0x4f14 edma_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_start                    : 1;   //     0
	UINT32 tx_dir                      : 3;   //  1: 3
	UINT32 tx_nxt_dir                  : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 tx_done                     : 1;   //     8
	UINT32 tx_locked                   : 1;   //     9
	UINT32 tx_switch_en                : 1;   //    10
	UINT32                             : 1;   //    11 reserved
	UINT32 tx_atomic_size              : 2;   // 12:13
} H13B0_EDMA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x4f18 pattern_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_addr                : 32;  //  0:31
} H13B0_PATTERN_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f1c pattern_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_addr                : 32;  //  0:31
} H13B0_PATTERN_DATA_T;

typedef struct {
	H13B0_SET_CTRL0_T           set_ctrl0;                   // 0x4e00
	H13B0_SET_CTRL1_T           set_ctrl1;                   // 0x4e04
	H13B0_EXT_INTR_EVENT_T      ext_intr_event;              // 0x4e08
	H13B0_EXT_INTR_ENABLE_T     ext_intr_enable;             // 0x4e0c
	H13B0_EXT_INTR_STATUS_T     ext_intr_status;             // 0x4e10
	H13B0_EXT_INTR_CLEAR_T      ext_intr_clear;              // 0x4e14
	H13B0_INT_INTR_EVENT_T      int_intr_event;              // 0x4e18
	H13B0_INT_INTR_ENABLE_T     int_intr_enable;             // 0x4e1c
	H13B0_INT_INTR_STATUS_T     int_intr_status;             // 0x4e20
	H13B0_INT_INTR_CLEAR_T      int_intr_clear;              // 0x4e24
	H13B0_SROM_BOOT_OFFSET_T    srom_boot_offset;            // 0x4e28
	H13B0_ATLAS_PORT_SEL_T      atlas_port_sel;              // 0x4e2c
	H13B0_PDEBUG_STATUS_T       pdebug_status;               // 0x4e30
	H13B0_PDEBUG_DATA_T         pdebug_data;                 // 0x4e34
	H13B0_PDEBUG_PC_T           pdebug_pc;                   // 0x4e38
	H13B0_AXI_USER_ID_T         axi_user_id;                 // 0x4e3c
	H13B0_SRAM0_ATLAS_OFFSET_T  sram0_atlas_offset;          // 0x4e40
	H13B0_SRAM1_ATLAS_OFFSET_T  sram1_atlas_offset;          // 0x4e44
	H13B0_SRAM2_ATLAS_OFFSET_T  sram2_atlas_offset;          // 0x4e48
	H13B0_SROM_BOOT_OFFSET2_T   srom_boot_offset2;           // 0x4e4c
	UINT32                      reserved0[3];                // 0x4e50~0x4e58
	H13B0_SPP_MCU_VERSION_T     spp_mcu_version;             // 0x4e5c
	UINT32                      reserved1[40];               // 0x4e60~0x4efc
	H13B0_TX_LOCKED_ID_T        tx_locked_id;                // 0x4f00
	H13B0_TX_SRC_ADDR_T         tx_src_addr;                 // 0x4f04
	H13B0_TX_DST_ADDR_T         tx_dst_addr;                 // 0x4f08
	H13B0_TX_NXT_ADDR_T         tx_nxt_addr;                 // 0x4f0c
	H13B0_TX_LENGTH_T           tx_length;                   // 0x4f10
	H13B0_EDMA_CTRL_T           edma_ctrl;                   // 0x4f14
	H13B0_PATTERN_ADDR_T        pattern_addr;                // 0x4f18
	H13B0_PATTERN_DATA_T        pattern_data;                // 0x4f1c
} DE_IPC_REG_H13B0_T;

#endif
