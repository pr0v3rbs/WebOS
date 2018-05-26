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

/** @file pe_ifc_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_IFC_HW_PARAM_H15_H_
#define	_PE_IFC_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_ifc_l_init_h15a0.h"
#include "pe_ifc_l_atv_ntsc_default_h15a0.h"
#include "pe_ifc_l_pc_default_h15a0.h"

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
 *	ifc hw param reg type
 */
typedef struct {
	PE_H15A0_DNR1_IFC_CTRL_0_T             	ifc_ctrl_0;              //0xc8007214L
	PE_H15A0_DNR1_IFC_CTRL_1_T             	ifc_ctrl_1;              //0xc8007218L
	PE_H15A0_DNR1_IFC_CTRL_2_T             	ifc_ctrl_2;              //0xc800721cL
	PE_H15A0_DNR1_IFC_CTRL_3_T             	ifc_ctrl_3;              //0xc8007220L
	PE_H15A0_DNR1_IFC_CTRL_4_T             	ifc_ctrl_4;              //0xc8007224L
	PE_H15A0_DNR1_IFC_CTRL_5_T             	ifc_ctrl_5;              //0xc8007228L
	PE_H15A0_DNR1_IFC_CTRL_6_T             	ifc_ctrl_6;              //0xc800722cL
	PE_H15A0_DNR1_IFC_CTRL_7_T             	ifc_ctrl_7;              //0xc8007230L
	PE_H15A0_DNR1_IFC_CTRL_8_T             	ifc_ctrl_8;              //0xc8007234L
	PE_H15A0_DNR1_IFC_CTRL_9_T             	ifc_ctrl_9;              //0xc8007238L
	PE_H15A0_DNR1_IFC_CTRL_10_T            	ifc_ctrl_10;             //0xc800723cL
	PE_H15A0_DNR1_IFC_CTRL_11_T            	ifc_ctrl_11;             //0xc8007240L
	PE_H15A0_DNR1_IFC_CTRL_12_T            	ifc_ctrl_12;             //0xc8007244L
	PE_H15A0_DNR1_IFC_CTRL_13_T            	ifc_ctrl_13;             //0xc8007248L
	PE_H15A0_DNR1_IFC_CTRL_14_T            	ifc_ctrl_14;             //0xc800724cL
	PE_H15A0_DNR1_IFC_CTRL_15_T            	ifc_ctrl_15;             //0xc8007250L
	PE_H15A0_DNR1_IFC_CTRL_16_T            	ifc_ctrl_16;             //0xc8007254L
	PE_H15A0_DNR1_IFC_CTRL_17_T            	ifc_ctrl_17;             //0xc8007258L
	PE_H15A0_DNR1_IFC_CTRL_18_T            	ifc_ctrl_18;             //0xc800725cL
	PE_H15A0_DNR1_IFC_CTRL_19_T            	ifc_ctrl_19;             //0xc8007260L
	PE_H15A0_DNR1_IFC_CTRL_20_T            	ifc_ctrl_20;             //0xc8007264L
	PE_H15A0_DNR1_IFC_CTRL_21_T            	ifc_ctrl_21;             //0xc8007268L
}PE_IFC_HW_PARAM_REG_H15A0_T;

/**
 *	ifc hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_IFC_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_IFC_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_IFC_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_IFC_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_IFC_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_IFC_HW_PARAM_H15_H_ */
