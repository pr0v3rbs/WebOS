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

#ifndef _REG_IPC_H15A0_H_
#define _REG_IPC_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc0025000L set_reg_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 run_stall                          : 1 ;  //     0     
	UINT32 start_vector_sel                   : 1 ;  //     1     
	UINT32 p_debug_enable                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 external_interrupt_en              : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 mcu_sw_reset                       : 1 ;  //     8     
} H15A0_SET_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0025004L set_reg_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 prid                               : 16;  // 15: 0     
} H15A0_SET_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0025008L ext_intr_event ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_arm            : 31;  // 30: 0     
	UINT32 wdg_interrupt_event                : 1 ;  //    31     
} H15A0_EXT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
		0xc002500cL ext_intr_enable ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_arm           : 31;  // 30: 0     
	UINT32 wdg_interrupt_enable               : 1 ;  //    31     
} H15A0_EXT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
		0xc0025010L ext_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_status_arm           : 31;  // 30: 0     
	UINT32 wdg_interrupt_status               : 1 ;  //    31     
} H15A0_EXT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0025014L ext_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_arm            : 31;  // 30: 0     
	UINT32 wdg_interrupt_clear                : 1 ;  //    31     
} H15A0_EXT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc0025018L int_intr_event ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_event_mcu            : 30;  // 29: 0     
	UINT32 edma_interrupt_event_mcu           : 1 ;  //    30     
} H15A0_INT_INTR_EVENT_T;

/*-----------------------------------------------------------------------------
		0xc002501cL int_intr_enable ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_enable_mcu           : 30;  // 29: 0     
	UINT32 edma_interrupt_enable_mcu          : 1 ;  //    30     
} H15A0_INT_INTR_ENABLE_T;

/*-----------------------------------------------------------------------------
		0xc0025020L int_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_staus_mcu            : 30;  // 29: 0     
	UINT32 edma_interrupt_status_mcu          : 1 ;  //    30     
} H15A0_INT_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0025024L int_intr_clear ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ipc_interrupt_clear_mcu            : 30;  // 29: 0     
	UINT32 edma_interrupt_clear_mcu           : 1 ;  //    30     
} H15A0_INT_INTR_CLEAR_T;

/*-----------------------------------------------------------------------------
		0xc0025028L srom_boot_offset1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset1                   : 32;  // 31: 0     
} H15A0_SROM_BOOT_OFFSET1_T;

/*-----------------------------------------------------------------------------
		0xc002502cL srom_boot_offset2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 boot_img_offset2                   : 32;  // 31: 0     
} H15A0_SROM_BOOT_OFFSET2_T;

/*-----------------------------------------------------------------------------
		0xc0025030L atlas_port_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram0_atlas_port_sel               : 4 ;  //  3: 0     
	UINT32 sram1_atlas_port_sel               : 4 ;  //  7: 4     
	UINT32 sram2_atlas_port_sel               : 4 ;  // 11: 8     
	UINT32 sram3_atlas_port_sel               : 4 ;  // 15:12     
	UINT32 port_sel_domination                : 1 ;  //    16     
} H15A0_ATLAS_PORT_SEL_T;

/*-----------------------------------------------------------------------------
		0xc0025034L pdebug_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_status                      : 8 ;  //  7: 0     
} H15A0_PDEBUG_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0025038L pdebug_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_data                        : 32;  // 31: 0     
} H15A0_PDEBUG_DATA_T;

/*-----------------------------------------------------------------------------
		0xc002503cL pdebug_pc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pdebug_pc                          : 32;  // 31: 0     
} H15A0_PDEBUG_PC_T;

/*-----------------------------------------------------------------------------
		0xc0025040L axi_user_id ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_user_id                         : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 ar_user_id                         : 2 ;  //  5: 4     
} H15A0_AXI_USER_ID_T;

/*-----------------------------------------------------------------------------
		0xc0025044L sram0_atlas_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram0_atlas_offset                 : 32;  // 31: 0     
} H15A0_SRAM0_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0025048L sram1_atlas_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram1_atlas_offset                 : 32;  // 31: 0     
} H15A0_SRAM1_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc002504cL sram2_atlas_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram2_atlas_offset                 : 32;  // 31: 0     
} H15A0_SRAM2_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0025050L sram3_atlas_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sram3_atlas_offset                 : 32;  // 31: 0     
} H15A0_SRAM3_ATLAS_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc0025054L inbound_start ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 inbound_start                      : 16;  // 15: 0     
} H15A0_INBOUND_START_T;

/*-----------------------------------------------------------------------------
		0xc0025058L inbound_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 inbound_offset                     : 32;  // 31: 0     
} H15A0_INBOUND_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc002505cL spp_mcu_version ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_SPP_MCU_VERSION_T;

/*-----------------------------------------------------------------------------
		0xc0025060L intr_mcu_flag ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dei_mcu_intr_mcu                   : 1 ;  //     0     
	UINT32 dem_mcu_intr_mcu                   : 1 ;  //     1     
	UINT32 deo_mcu_intr_mcu                   : 1 ;  //     2     
	UINT32 dpp_mcu_intr_mcu                   : 1 ;  //     3     
	UINT32 intr_frc_dvi                       : 1 ;  //     4     
	UINT32 intr_a3c                           : 1 ;  //     5     
	UINT32 intr_dmc                           : 1 ;  //     6     
	UINT32 intr_frc_ma                        : 1 ;  //     7     
	UINT32 intr_frc_mc                        : 1 ;  //     8     
	UINT32 intr_frc_mb                        : 1 ;  //     9     
	UINT32 intr_frc_dvo                       : 1 ;  //    10     
	UINT32 intr_svp                           : 1 ;  //    11     
	UINT32 intr_dpwmtx0                       : 1 ;  //    12     
	UINT32 intr_dpwmtx1                       : 1 ;  //    13     
	UINT32 intr_dpwmtx2                       : 1 ;  //    14     
	UINT32 intr_dpwmrx0                       : 1 ;  //    15     
	UINT32 intr_dpwmrx1                       : 1 ;  //    16     
	UINT32 dpe_intr_o                         : 1 ;  //    17     
} H15A0_INTR_MCU_FLAG_T;

/*-----------------------------------------------------------------------------
		0xc0025064L intr_mcu_flag_en ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dei_mcu_intr_mcu_en                : 1 ;  //     0     
	UINT32 dem_mcu_intr_mcu_en                : 1 ;  //     1     
	UINT32 deo_mcu_intr_mcu_en                : 1 ;  //     2     
	UINT32 dpp_mcu_intr_mcu_en                : 1 ;  //     3     
	UINT32 intr_frc_dvi_en                    : 1 ;  //     4     
	UINT32 intr_a3c_en                        : 1 ;  //     5     
	UINT32 intr_dmc_en                        : 1 ;  //     6     
	UINT32 intr_frc_ma_en                     : 1 ;  //     7     
	UINT32 intr_frc_mc_en                     : 1 ;  //     8     
	UINT32 intr_frc_mb_en                     : 1 ;  //     9     
	UINT32 intr_frc_dvo_en                    : 1 ;  //    10     
	UINT32 intr_svp_en                        : 1 ;  //    11     
	UINT32 intr_dpwmtx0_en                    : 1 ;  //    12     
	UINT32 intr_dpwmtx1_en                    : 1 ;  //    13     
	UINT32 intr_dpwmtx2_en                    : 1 ;  //    14     
	UINT32 intr_dpwmrx0_en                    : 1 ;  //    15     
	UINT32 intr_dpwmrx1_en                    : 1 ;  //    16     
	UINT32 dpe_intr_o_en                      : 1 ;  //    17     
} H15A0_INTR_MCU_FLAG_EN_T;

/*-----------------------------------------------------------------------------
		0xc0025068L intr_cpu_flag ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvi_mcu_intr_cpu                   : 1 ;  //     0     
	UINT32 imx_mcu_intr_cpu                   : 1 ;  //     1     
	UINT32 nd1_mcu_intr_cpu                   : 1 ;  //     2     
	UINT32 nd2_mcu_intr_cpu                   : 1 ;  //     3     
	UINT32 gsc_mcu_intr_cpu                   : 1 ;  //     4     	
	UINT32 vsd_mcu_intr_cpu                   : 1 ;  //     5     	
	UINT32 cco_mcu_intr_cpu                   : 1 ;  //     6     	
	UINT32 sub_mcu_intr_cpu                   : 1 ;  //     7     	
} H15A0_INTR_CPU_FLAG_T;

/*-----------------------------------------------------------------------------
		0xc0025100L cpu_ipc0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_CPU_IPC0_T;

/*-----------------------------------------------------------------------------
		0xc00251fcL cpu_ipc63 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} H15A0_CPU_IPC63_T;

typedef struct {
	H15A0_SET_CTRL0_T              	        set_ctrl0;                    //0xc0025000L
	H15A0_SET_CTRL1_T              	        set_ctrl1;                    //0xc0025004L
	H15A0_EXT_INTR_EVENT_T             	ext_intr_event;               //0xc0025008L
	H15A0_EXT_INTR_ENABLE_T            	ext_intr_enable;              //0xc002500cL
	H15A0_EXT_INTR_STATUS_T            	ext_intr_status;              //0xc0025010L
	H15A0_EXT_INTR_CLEAR_T             	ext_intr_clear;               //0xc0025014L
	H15A0_INT_INTR_EVENT_T             	int_intr_event;               //0xc0025018L
	H15A0_INT_INTR_ENABLE_T            	int_intr_enable;              //0xc002501cL
	H15A0_INT_INTR_STATUS_T            	int_intr_status;              //0xc0025020L
	H15A0_INT_INTR_CLEAR_T             	int_intr_clear;               //0xc0025024L
	H15A0_SROM_BOOT_OFFSET1_T          	srom_boot_offset1;            //0xc0025028L
	H15A0_SROM_BOOT_OFFSET2_T          	srom_boot_offset2;            //0xc002502cL
	H15A0_ATLAS_PORT_SEL_T             	atlas_port_sel;               //0xc0025030L
	H15A0_PDEBUG_STATUS_T              	pdebug_status;                //0xc0025034L
	H15A0_PDEBUG_DATA_T                	pdebug_data;                  //0xc0025038L
	H15A0_PDEBUG_PC_T                  	pdebug_pc;                    //0xc002503cL
	H15A0_AXI_USER_ID_T                	axi_user_id;                  //0xc0025040L
	H15A0_SRAM0_ATLAS_OFFSET_T         	sram0_atlas_offset;           //0xc0025044L
	H15A0_SRAM1_ATLAS_OFFSET_T         	sram1_atlas_offset;           //0xc0025048L
	H15A0_SRAM2_ATLAS_OFFSET_T         	sram2_atlas_offset;           //0xc002504cL
	H15A0_SRAM3_ATLAS_OFFSET_T         	sram3_atlas_offset;           //0xc0025050L
	H15A0_INBOUND_START_T              	inbound_start;                //0xc0025054L
	H15A0_INBOUND_OFFSET_T             	inbound_offset;               //0xc0025058L
	H15A0_SPP_MCU_VERSION_T            	spp_mcu_version;              //0xc002505cL
	H15A0_INTR_MCU_FLAG_T              	intr_mcu_flag;                //0xc0025060L
	H15A0_INTR_MCU_FLAG_EN_T           	intr_mcu_flag_en;             //0xc0025064L
	H15A0_INTR_CPU_FLAG_T              	intr_cpu_flag;                //0xc0025068L
	UINT32                             	reserved00[37];               //0xc002506cL~0xc00250fcL
	H15A0_CPU_IPC0_T                   	cpu_ipc0;                     //0xc0025100L
	UINT32                             	reserved01[62];               //0xc0025104L~0xc00251f8L
	H15A0_CPU_IPC63_T                  	cpu_ipc63;                    //0xc00251fcL
} DE_IPC_REG_H15A0_T;

#endif
