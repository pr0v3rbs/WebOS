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

#ifndef _REG_IPC_H13A0_h
#define _REG_IPC_H13A0_h

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
} H13A0_SET_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x4e04 set_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 prid                        : 16;  //  0:15
} H13A0_SET_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x4e08 ext_intr_event ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_arm     : 30;  //  0:29
	UINT32 edma_interrupt_event_arm    : 1;   //    30
	UINT32 wdg_interrupt_event         : 1;   //    31
} H13A0_EXT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
	0x4e0c ext_intr_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_arm    : 30;  //  0:29
	UINT32 edma_interrupt_enable_arm   : 1;   //    30
	UINT32 wdg_interrupt_enable        : 1;   //    31
} H13A0_EXT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
	0x4e10 ext_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_status_arm    : 30;  //  0:29
	UINT32 edma_interrupt_status_arm   : 1;   //    30
	UINT32 wdg_interrupt_status        : 1;   //    31
} H13A0_EXT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e14 ext_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_arm     : 30;  //  0:29
	UINT32 edma_interrupt_clear_arm    : 1;   //    30
	UINT32 wdg_interrupt_clear         : 1;   //    31
} H13A0_EXT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
	0x4e18 int_intr_event ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_event_mcu    : 1;   //    30
} H13A0_INT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
	0x4e1c int_intr_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_mcu    : 30;  //  0:29
	UINT32 edma_interrupt_enable_mcu   : 1;   //    30
} H13A0_INT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
	0x4e20 int_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_staus_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_status_mcu   : 1;   //    30
} H13A0_INT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e24 int_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_mcu     : 30;  //  0:29
	UINT32 edma_interrupt_clear_mcu    : 1;   //    30
} H13A0_INT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
	0x4e28 srom_boot_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset             : 32;  //  0:31
} H13A0_SROM_BOOT_OFFSET_T;

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
} H13A0_ATLAS_PORT_SEL_T;

/*-----------------------------------------------------------------------------
	0x4e30 pdebug_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_status               : 8;   //  0: 7
} H13A0_PDEBUG_STATUS_T;

/*-----------------------------------------------------------------------------
	0x4e34 pdebug_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_data                 : 32;  //  0:31
} H13A0_PDEBUG_DATA_T;

/*-----------------------------------------------------------------------------
	0x4e38 pdebug_pc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_pc                   : 32;  //  0:31
} H13A0_PDEBUG_PC_T;

/*-----------------------------------------------------------------------------
	0x4e3c axi_user_id ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 axi_user_id                 : 3;   //  0: 2
} H13A0_AXI_USER_ID_T;

/*-----------------------------------------------------------------------------
	0x4e40 sram0_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram0_atlas_offset          : 32;  //  0:31
} H13A0_SRAM0_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e44 sram1_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram1_atlas_offset          : 32;  //  0:31
} H13A0_SRAM1_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e48 sram2_atlas_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram2_atlas_offset          : 32;  //  0:31
} H13A0_SRAM2_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x4e4c srom_boot_offset2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset2            : 32;  //  0:31
} H13A0_SROM_BOOT_OFFSET2_T;

/*-----------------------------------------------------------------------------
	0x4e5c spp_mcu_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spp_mcu_version_f           : 32;   // 31: 0
} H13A0_SPP_MCU_VERSION_T;

/*-----------------------------------------------------------------------------
	0x4f00 tx_locked_id ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_locked_id                : 32;  //  0:31
} H13A0_TX_LOCKED_ID_T;

/*-----------------------------------------------------------------------------
	0x4f04 tx_src_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_src_addr                 : 32;  //  0:31
} H13A0_TX_SRC_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f08 tx_dst_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_dst_addr                 : 32;  //  0:31
} H13A0_TX_DST_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f0c tx_nxt_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_nxt_addr                 : 32;  //  0:31
} H13A0_TX_NXT_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f10 tx_length ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tx_length                   : 32;  //  0:31
} H13A0_TX_LENGTH_T;

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
} H13A0_EDMA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x4f18 pattern_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_addr                : 32;  //  0:31
} H13A0_PATTERN_ADDR_T;

/*-----------------------------------------------------------------------------
	0x4f1c pattern_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_addr                : 32;  //  0:31
} H13A0_PATTERN_DATA_T;

typedef struct {
	H13A0_SET_CTRL0_T           set_ctrl0;                   // 0x4e00
	H13A0_SET_CTRL1_T           set_ctrl1;                   // 0x4e04
	H13A0_EXT_INTR_EVENT_T      ext_intr_event;              // 0x4e08
	H13A0_EXT_INTR_ENABLE_T     ext_intr_enable;             // 0x4e0c
	H13A0_EXT_INTR_STATUS_T     ext_intr_status;             // 0x4e10
	H13A0_EXT_INTR_CLEAR_T      ext_intr_clear;              // 0x4e14
	H13A0_INT_INTR_EVENT_T      int_intr_event;              // 0x4e18
	H13A0_INT_INTR_ENABLE_T     int_intr_enable;             // 0x4e1c
	H13A0_INT_INTR_STATUS_T     int_intr_status;             // 0x4e20
	H13A0_INT_INTR_CLEAR_T      int_intr_clear;              // 0x4e24
	H13A0_SROM_BOOT_OFFSET_T    srom_boot_offset;            // 0x4e28
	H13A0_ATLAS_PORT_SEL_T      atlas_port_sel;              // 0x4e2c
	H13A0_PDEBUG_STATUS_T       pdebug_status;               // 0x4e30
	H13A0_PDEBUG_DATA_T         pdebug_data;                 // 0x4e34
	H13A0_PDEBUG_PC_T           pdebug_pc;                   // 0x4e38
	H13A0_AXI_USER_ID_T         axi_user_id;                 // 0x4e3c
	H13A0_SRAM0_ATLAS_OFFSET_T  sram0_atlas_offset;          // 0x4e40
	H13A0_SRAM1_ATLAS_OFFSET_T  sram1_atlas_offset;          // 0x4e44
	H13A0_SRAM2_ATLAS_OFFSET_T  sram2_atlas_offset;          // 0x4e48
	H13A0_SROM_BOOT_OFFSET2_T   srom_boot_offset2;           // 0x4e4c
	UINT32                      reserved0[3];                // 0x4e50~0x4e58
	H13A0_SPP_MCU_VERSION_T     spp_mcu_version;             // 0x4e5c
	UINT32                      reserved1[40];               // 0x4e60~0x4efc
	H13A0_TX_LOCKED_ID_T        tx_locked_id;                // 0x4f00
	H13A0_TX_SRC_ADDR_T         tx_src_addr;                 // 0x4f04
	H13A0_TX_DST_ADDR_T         tx_dst_addr;                 // 0x4f08
	H13A0_TX_NXT_ADDR_T         tx_nxt_addr;                 // 0x4f0c
	H13A0_TX_LENGTH_T           tx_length;                   // 0x4f10
	H13A0_EDMA_CTRL_T           edma_ctrl;                   // 0x4f14
	H13A0_PATTERN_ADDR_T        pattern_addr;                // 0x4f18
	H13A0_PATTERN_DATA_T        pattern_data;                // 0x4f1c
} DE_IPC_REG_H13A0_T;


/*
	DPPB TOP
*/

/*-----------------------------------------------------------------------------
	0x0000 top_runstall ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	runstall                        : 1;	//     0
} H13A0_MCU1_RUNSTALL;

/*-----------------------------------------------------------------------------
	0x0004 top_stat_vector_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stat_vector_sel                 : 1;	//     0
} H13A0_MCU1_STAT_VECTOR_SEL;

/*-----------------------------------------------------------------------------
	0x0008 top_pdebugenable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdebug_enable                   : 1;	//     0
} H13A0_MCU1_PDEBUGENABLE;

/*-----------------------------------------------------------------------------
	0x000c top_pdebugdata ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdebug_data                     ;   	// 31: 0
} H13A0_MCU1_PDEBUGDATA;

/*-----------------------------------------------------------------------------
	0x0010 top_pdebugpc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdebug_pc                       ;   	// 31: 0
} H13A0_MCU1_PDEBUGPC;

/*-----------------------------------------------------------------------------
	0x0014 top_pdebugstatus ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdebug_status                   : 8;	//  0: 7
} H13A0_MCU1_PDEBUGSTATUS;

/*-----------------------------------------------------------------------------
	0x0018 top_tr_addr_offset_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tr_addr_offset_0                ;   	// 31: 0
} H13A0_MCU1_TR_ADDR_OFFSET_0;

/*-----------------------------------------------------------------------------
	0x001c top_tr_addr_offset_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tr_addr_offset_1                ;   	// 31: 0
} H13A0_MCU1_TR_ADDR_OFFSET_1;

/*-----------------------------------------------------------------------------
	0x0020 top_tr_addr_offset_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tr_addr_offset_2                ;   	// 31: 0
} H13A0_MCU1_TR_ADDR_OFFSET_2;

/*-----------------------------------------------------------------------------
	0x0024 top_tr_addr_offset_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tr_addr_offset_3                ;   	// 31: 0
} H13A0_MCU1_TR_ADDR_OFFSET_3;

/*-----------------------------------------------------------------------------
	0x0028 top_arm_intr_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	arm_wdg_int_en                      :  1,   ///     0
	arm_dma_int_en                      :  1,   ///     1
	arm_led_int_en                      :  1,   ///     2 
	arm_dppa_int_en                     :  1,   ///     3
	arm_sre_int_en                      :  1,   ///     4 
	arm_lrx_int_en                      :  1,   ///     5
	arm_dvo_int_en                      :  1,   ///     6
	arm_dvi_int_en                      :  1,   ///     7
	arm_h3d0_int_en                     :  1,   ///     8
	arm_h3d1_int_en                     :  1,   ///     9
	reserved                            :  5,   /// 10:14
	arm_swi_int_en                      : 17;   /// 15:31
} H13A0_MCU1_ARM_INTR_EN;

/*-----------------------------------------------------------------------------
	0x002c top_arm_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	arm_wdg_int_status                  :  1,   ///     0
	arm_dma_int_status                  :  1,   ///     1
	arm_led_int_status                  :  1,   ///     2 
	arm_dppa_int_status                 :  1,   ///     3
	arm_sre_int_status                  :  1,   ///     4 
	arm_lrx_int_status                  :  1,   ///     5
	arm_dvo_int_status                  :  1,   ///     6
	arm_dvi_int_status                  :  1,   ///     7
	arm_h3d0_int_status                 :  1,   ///     8
	arm_h3d1_int_status                 :  1,   ///     9
	reserved                            :  5,   /// 10:14
	arm_swi_int_status                  : 17;   /// 15:31
} H13A0_MCU1_ARM_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0030 top_arm_intr_raw_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	arm_wdg_int_rawstat                  :  1,   ///     0
	arm_dma_int_rawstat                  :  1,   ///     1
	arm_led_int_rawstat                  :  1,   ///     2 
	arm_dppa_int_rawstat                 :  1,   ///     3
	arm_sre_int_rawstat                  :  1,   ///     4 
	arm_lrx_int_rawstat                  :  1,   ///     5
	arm_dvo_int_rawstat                  :  1,   ///     6
	arm_dvi_int_rawstat                  :  1,   ///     7
	arm_h3d0_int_rawstat                 :  1,   ///     8
	arm_h3d1_int_rawstat                 :  1,   ///     9
	reserved                            :  5,   /// 10:14
	arm_swi_int_rawstat                 : 17;   /// 15:31
} H13A0_MCU1_ARM_INTR_RAW_STATUS;

/*-----------------------------------------------------------------------------
	0x0034 top_arm_intr_clr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	arm_wdg_int_clear                  :  1,   ///     0
	arm_dma_int_clear                  :  1,   ///     1
	arm_led_int_clear                  :  1,   ///     2 
	arm_dppa_int_clear                 :  1,   ///     3
	arm_sre_int_clear                  :  1,   ///     4 
	arm_lrx_int_clear                  :  1,   ///     5
	arm_dvo_int_clear                  :  1,   ///     6
	arm_dvi_int_clear                  :  1,   ///     7
	arm_h3d0_int_clear                 :  1,   ///     8
	arm_h3d1_int_clear                 :  1,   ///     9
	reserved                           :  5,   /// 10:14
	arm_swi_int_clear                  : 17;   /// 15:31
} H13A0_MCU1_ARM_INTR_CLR;

/*-----------------------------------------------------------------------------
	0x0038 top_de_intr_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_wdg_int_en                      :  1,   ///     0
	de_dma_int_en                      :  1,   ///     1
	de_led_int_en                      :  1,   ///     2 
	de_dppa_int_en                     :  1,   ///     3
	de_sre_int_en                      :  1,   ///     4 
	de_lrx_int_en                      :  1,   ///     5
	de_dvo_int_en                      :  1,   ///     6
	de_dvi_int_en                      :  1,   ///     7
	de_h3d0_int_en                     :  1,   ///     8
	de_h3d1_int_en                     :  1,   ///     9
	de_swi_int_en                      : 21;   /// 10:31
} H13A0_MCU1_DE_INTR_EN;

/*-----------------------------------------------------------------------------
	0x003c top_de_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_wdg_int_status                  :  1,   ///     0
	de_dma_int_status                  :  1,   ///     1
	de_led_int_status                  :  1,   ///     2 
	de_dppa_int_status                 :  1,   ///     3
	de_sre_int_status                  :  1,   ///     4 
	de_lrx_int_status                  :  1,   ///     5
	de_dvo_int_status                  :  1,   ///     6
	de_dvi_int_status                  :  1,   ///     7
	de_h3d0_int_status                 :  1,   ///     8
	de_h3d1_int_status                 :  1,   ///     9
	de_swi_int_status                  : 21;   /// 10:31
} H13A0_MCU1_DE_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0040 top_de_intr_raw_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_wdg_int_rawstat                  :  1,   ///     0
	de_dma_int_rawstat                  :  1,   ///     1
	de_led_int_rawstat                  :  1,   ///     2 
	de_dppa_int_rawstat                 :  1,   ///     3
	de_sre_int_rawstat                  :  1,   ///     4 
	de_lrx_int_rawstat                  :  1,   ///     5
	de_dvo_int_rawstat                  :  1,   ///     6
	de_dvi_int_rawstat                  :  1,   ///     7
	de_h3d0_int_rawstat                 :  1,   ///     8
	de_h3d1_int_rawstat                 :  1,   ///     9
	de_swi_int_rawstat                  : 21;   /// 10:31
} H13A0_MCU1_DE_INTR_RAW_STATUS;

/*-----------------------------------------------------------------------------
	0x0044 top_de_intr_clr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_wdg_int_clear                  :  1,   ///     0
	de_dma_int_clear                  :  1,   ///     1
	de_led_int_clear                  :  1,   ///     2 
	de_dppa_int_clear                 :  1,   ///     3
	de_sre_int_clear                  :  1,   ///     4 
	de_lrx_int_clear                  :  1,   ///     5
	de_dvo_int_clear                  :  1,   ///     6
	de_dvi_int_clear                  :  1,   ///     7
	de_h3d0_int_clear                 :  1,   ///     8
	de_h3d1_int_clear                 :  1,   ///     9
	de_swi_int_clear                  : 21;   /// 10:31
} H13A0_MCU1_DE_INTR_CLR;

/*-----------------------------------------------------------------------------
	0x0048 top_rc_intr_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rc_wdg_int_en                      :  1,   ///     0
	rc_gpio_int_en                     :  1,   ///     1
	rc_dmadone_int_en                  :  1,   ///     2 
	rc_swi_int_en                      : 29;   ///  3:31 
} H13A0_MCU1_RC_INTR_EN;

/*-----------------------------------------------------------------------------
	0x004c top_rc_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rc_wdg_int_status                      :  1,   ///     0
	rc_gpio_int_status                     :  1,   ///     1
	rc_dmadone_int_status                  :  1,   ///     2 
	rc_swi_int_status                      : 29;   ///  3:31 
} H13A0_MCU1_RC_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x0050 top_rc_intr_raw_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rc_wdg_int_rawstat                      :  1,   ///     0
	rc_gpio_int_rawstat                     :  1,   ///     1
	rc_dmadone_int_rawstat                  :  1,   ///     2 
	rc_swi_int_rawstat                      : 29;   ///  3:31 
} H13A0_MCU1_RC_INTR_RAW_STATUS;

/*-----------------------------------------------------------------------------
	0x0054 top_rc_intr_clr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rc_wdg_int_clear                      :  1,   ///     0
	rc_gpio_int_clear                     :  1,   ///     1
	rc_dmadone_int_clear                  :  1,   ///     2 
	rc_swi_int_clear                      : 29;   ///  3:31 
} H13A0_MCU1_RC_INTR_CLR;

/*-----------------------------------------------------------------------------
	0x0058 top_dma_crc_clr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dma_crc_clear                   : 1;	//     0
} H13A0_MCU1_DMA_CRC_CLR;

/*-----------------------------------------------------------------------------
	0x005c top_dma_crc_hec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dma_crc_hec                     : 8;	//  0: 7
} H13A0_MCU1_DMA_CRC_HEC;

/*-----------------------------------------------------------------------------
	0x0060 top_dma_crc_aal5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dma_crc_aal5                    ;   	// 31: 0
} H13A0_MCU1_DMA_CRC_AAL5;

/*-----------------------------------------------------------------------------
	0x0064 top_ipc0 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0068 top_ipc1 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x006c top_ipc2 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0070 top_ipc3 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0074 top_ipc4 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0078 top_ipc5 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x007c top_ipc6 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0080 top_ipc7 ''
------------------------------------------------------------------------------*/
/*	no field */

typedef struct {
	H13A0_MCU1_RUNSTALL                     	top_runstall                     ;	// 0x0000 : ''
	H13A0_MCU1_STAT_VECTOR_SEL              	top_stat_vector_sel              ;	// 0x0004 : ''
	H13A0_MCU1_PDEBUGENABLE                 	top_pdebugenable                 ;	// 0x0008 : ''
	H13A0_MCU1_PDEBUGDATA                   	top_pdebugdata                   ;	// 0x000c : ''
	H13A0_MCU1_PDEBUGPC                     	top_pdebugpc                     ;	// 0x0010 : ''
	H13A0_MCU1_PDEBUGSTATUS                 	top_pdebugstatus                 ;	// 0x0014 : ''
	H13A0_MCU1_TR_ADDR_OFFSET_0             	top_tr_addr_offset_0             ;	// 0x0018 : ''
	H13A0_MCU1_TR_ADDR_OFFSET_1             	top_tr_addr_offset_1             ;	// 0x001c : ''
	H13A0_MCU1_TR_ADDR_OFFSET_2             	top_tr_addr_offset_2             ;	// 0x0020 : ''
	H13A0_MCU1_TR_ADDR_OFFSET_3             	top_tr_addr_offset_3             ;	// 0x0024 : ''
	H13A0_MCU1_ARM_INTR_EN                  	top_arm_intr_en                  ;	// 0x0028 : ''
	H13A0_MCU1_ARM_INTR_STATUS              	top_arm_intr_status              ;	// 0x002c : ''
	H13A0_MCU1_ARM_INTR_RAW_STATUS          	top_arm_intr_raw_status          ;	// 0x0030 : ''
	H13A0_MCU1_ARM_INTR_CLR                 	top_arm_intr_clr                 ;	// 0x0034 : ''
	H13A0_MCU1_DE_INTR_EN                   	top_de_intr_en                   ;	// 0x0038 : ''
	H13A0_MCU1_DE_INTR_STATUS               	top_de_intr_status               ;	// 0x003c : ''
	H13A0_MCU1_DE_INTR_RAW_STATUS           	top_de_intr_raw_status           ;	// 0x0040 : ''
	H13A0_MCU1_DE_INTR_CLR                  	top_de_intr_clr                  ;	// 0x0044 : ''
	H13A0_MCU1_RC_INTR_EN                   	top_rc_intr_en                   ;	// 0x0048 : ''
	H13A0_MCU1_RC_INTR_STATUS               	top_rc_intr_status               ;	// 0x004c : ''
	H13A0_MCU1_RC_INTR_RAW_STATUS           	top_rc_intr_raw_status           ;	// 0x0050 : ''
	H13A0_MCU1_RC_INTR_CLR                  	top_rc_intr_clr                  ;	// 0x0054 : ''
	H13A0_MCU1_DMA_CRC_CLR                  	top_dma_crc_clr                  ;	// 0x0058 : ''
	H13A0_MCU1_DMA_CRC_HEC                  	top_dma_crc_hec                  ;	// 0x005c : ''
	H13A0_MCU1_DMA_CRC_AAL5                 	top_dma_crc_aal5                 ;	// 0x0060 : ''
	UINT32                                  	top_ipc0                         ;	// 0x0064 : ''
	UINT32                                  	top_ipc1                         ;	// 0x0068 : ''
	UINT32                                  	top_ipc2                         ;	// 0x006c : ''
	UINT32                                  	top_ipc3                         ;	// 0x0070 : ''
	UINT32                                  	top_ipc4                         ;	// 0x0074 : ''
	UINT32                                  	top_ipc5                         ;	// 0x0078 : ''
	UINT32                                  	top_ipc6                         ;	// 0x007c : ''
	UINT32                                  	top_ipc7                         ;	// 0x0080 : ''
} DE_IPC1_REG_H13A0_T;

#endif
