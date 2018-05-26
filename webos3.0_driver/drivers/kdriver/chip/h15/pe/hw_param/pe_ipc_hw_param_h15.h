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

/** @file pe_ipc_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_IPC_HW_PARAM_H15_H_
#define	_PE_IPC_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_ipc_l_init_h15a0.h"
#include "pe_ipc_l_atv_480_default_h15a0.h"
#include "pe_ipc_l_atv_576_default_h15a0.h"
#include "pe_ipc_l_hd_default_h15a0.h"
#include "pe_ipc_l_sd_default_h15a0.h"
#include "pe_ipc_l_dtv_480_default_h15a0.h"
#include "pe_ipc_l_dtv_576_default_h15a0.h"
#include "pe_ipc_l_av_480_default_h15a0.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------*/

/**
 *	ipc hw param reg type
 */
typedef struct {
	PE_H15A0_ND1_IPC_CTRL_00_T            	ipc_ctrl_00;              //0xc8004200L
	PE_H15A0_ND1_IPC_CTRL_01_T            	ipc_ctrl_01;              //0xc8004204L
	PE_H15A0_ND1_IPC_CTRL_02_T            	ipc_ctrl_02;              //0xc8004208L
	PE_H15A0_ND1_IPC_CTRL_03_T            	ipc_ctrl_03;              //0xc800420cL
	PE_H15A0_ND1_IPC_CTRL_04_T            	ipc_ctrl_04;              //0xc8004210L
	PE_H15A0_ND1_IPC_CTRL_05_T            	ipc_ctrl_05;              //0xc8004214L
	PE_H15A0_ND1_IPC_CTRL_06_T            	ipc_ctrl_06;              //0xc8004218L
	PE_H15A0_ND1_IPC_CTRL_07_T            	ipc_ctrl_07;              //0xc800421cL
	PE_H15A0_ND1_IPC_CTRL_08_T            	ipc_ctrl_08;              //0xc8004220L
	PE_H15A0_ND1_IPC_CTRL_09_T            	ipc_ctrl_09;              //0xc8004224L
	PE_H15A0_ND1_IPC_CTRL_10_T            	ipc_ctrl_10;              //0xc8004228L
	PE_H15A0_ND1_IPC_CTRL_11_T            	ipc_ctrl_11;              //0xc800422cL
	PE_H15A0_ND1_IPC_CTRL_12_T            	ipc_ctrl_12;              //0xc8004230L
	PE_H15A0_ND1_IPC_CTRL_13_T            	ipc_ctrl_13;              //0xc8004234L
	PE_H15A0_ND1_IPC_CTRL_14_T            	ipc_ctrl_14;              //0xc8004238L
	PE_H15A0_ND1_IPC_CTRL_15_T            	ipc_ctrl_15;              //0xc800423cL
	PE_H15A0_ND1_IPC_CTRL_16_T            	ipc_ctrl_16;              //0xc8004240L
	PE_H15A0_ND1_IPC_CTRL_17_T            	ipc_ctrl_17;              //0xc8004244L
	PE_H15A0_ND1_IPC_CTRL_18_T            	ipc_ctrl_18;              //0xc8004248L
	PE_H15A0_ND1_IPC_CTRL_19_T            	ipc_ctrl_19;              //0xc800424cL
	PE_H15A0_ND1_IPC_CTRL_34_T            	ipc_ctrl_34;              //0xc8004280L
	PE_H15A0_ND1_IPC_CTRL_35_T            	ipc_ctrl_35;              //0xc8004284L
	PE_H15A0_ND1_IPC_CTRL_36_T            	ipc_ctrl_36;              //0xc8004288L
	PE_H15A0_ND1_IPC_CTRL_37_T            	ipc_ctrl_37;              //0xc800428cL
	PE_H15A0_ND1_IPC_CTRL_38_T            	ipc_ctrl_38;              //0xc8004290L
	PE_H15A0_ND1_IPC_CTRL_39_T            	ipc_ctrl_39;              //0xc8004294L
	PE_H15A0_ND1_IPC_CTRL_40_T            	ipc_ctrl_40;              //0xc8004298L
	PE_H15A0_ND1_IPC_CTRL_41_T            	ipc_ctrl_41;              //0xc800429cL
	PE_H15A0_ND1_IPC_CTRL_42_T            	ipc_ctrl_42;              //0xc80042a0L
	PE_H15A0_ND1_IPC_CTRL_43_T            	ipc_ctrl_43;              //0xc80042a4L
	PE_H15A0_ND1_IPC_CTRL_44_T            	ipc_ctrl_44;              //0xc80042a8L
	PE_H15A0_ND1_IPC_CTRL_20_T            	ipc_ctrl_20;              //0xc80042f0L
	PE_H15A0_ND1_IPC_CTRL_21_T            	ipc_ctrl_21;              //0xc80042f4L
	PE_H15A0_ND1_IPC_CTRL_22_T            	ipc_ctrl_22;              //0xc80042f8L
	PE_H15A0_ND1_IPC_CTRL_23_T            	ipc_ctrl_23;              //0xc80042fcL
	PE_H15A0_ND1_IPC_CTRL_24_T            	ipc_ctrl_24;              //0xc8004300L
	PE_H15A0_ND1_IPC_CTRL_25_T            	ipc_ctrl_25;              //0xc8004304L
	PE_H15A0_ND1_IPC_CTRL_26_T            	ipc_ctrl_26;              //0xc8004308L
	PE_H15A0_ND1_IPC_CTRL_27_T            	ipc_ctrl_27;              //0xc800430cL
	PE_H15A0_ND1_IPC_CTRL_28_T            	ipc_ctrl_28;              //0xc8004310L
	PE_H15A0_ND1_IPC_CTRL_29_T            	ipc_ctrl_29;              //0xc8004314L
	PE_H15A0_ND1_IPC_CTRL_30_T            	ipc_ctrl_30;              //0xc8004318L
	PE_H15A0_ND1_IPC_CTRL_31_T            	ipc_ctrl_31;              //0xc800431cL
	PE_H15A0_ND1_IPC_CTRL_32_T            	ipc_ctrl_32;              //0xc8004320L
	PE_H15A0_ND1_IPC_CTRL_33_T            	ipc_ctrl_33;              //0xc8004324L
} PE_IPC_HW_PARAM_REG_H15A0_T;

/**
 *	ipc hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_IPC_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_IPC_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_IPC_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_IPC_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_IPC_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_IPC_HW_PARAM_H15_H_ */

