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

/** @file pe_clc_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_CLC_HW_PARAM_H15_H_
#define	_PE_CLC_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_clc_l_init_h15a0.h"
#include "pe_clc_l_hd_default_h15a0.h"
#include "pe_clc_l_pc_default_h15a0.h"
#include "pe_clc_l_sd_50_default_h15a0.h"
#include "pe_clc_l_sd_60_default_h15a0.h"

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
 *	clc hw param reg type
 */
typedef struct {
	PE_H15A0_ND1_CLC_CTRL_00_T            	clc_ctrl_00;              //0xc8004260L
	PE_H15A0_ND1_CLC_CTRL_01_T            	clc_ctrl_01;              //0xc8004264L
	PE_H15A0_ND1_CLC_CTRL_02_T            	clc_ctrl_02;              //0xc8004268L
	PE_H15A0_ND1_CLC_CTRL_03_T            	clc_ctrl_03;              //0xc800426cL
	PE_H15A0_ND1_CLC_CTRL_04_T            	clc_ctrl_04;              //0xc8004270L
	PE_H15A0_ND1_CLC_CTRL_05_T            	clc_ctrl_05;              //0xc8004274L
	PE_H15A0_ND1_CLC_CTRL_06_T            	clc_ctrl_06;              //0xc8004278L
	PE_H15A0_ND1_CLC_CTRL_07_T            	clc_ctrl_07;              //0xc800436cL
}PE_CLC_HW_PARAM_REG_H15A0_T;

/**
 *	clc hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_CLC_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_CLC_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_CLC_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_CLC_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_CLC_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_CLC_HW_PARAM_H15_H_ */
