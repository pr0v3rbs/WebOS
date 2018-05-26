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

/** @file pe_pblur_dnr_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_PBLUR_DNR_HW_PARAM_H15_H_
#define	_PE_PBLUR_DNR_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_pblur_dnr_l_init_h15a0.h"
#include "pe_pblur_dnr_l_atv_nt_default_h15a0.h"
#include "pe_pblur_dnr_l_atv_pal_default_h15a0.h"
#include "pe_pblur_dnr_l_av_default_h15a0.h"
#include "pe_pblur_dnr_l_secam_default_h15a0.h"
#include "pe_pblur_dnr_l_comp_sd_default_h15a0.h"
#include "pe_pblur_dnr_l_dtv_sd_default_h15a0.h"
#include "pe_pblur_dnr_l_hdmi_sd_default_h15a0.h"
#include "pe_pblur_dnr_l_hd_default_h15a0.h"
#include "pe_pblur_dnr_l_pc_default_h15a0.h"

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
 *	pbl(pblur_dnr) hw param reg type
 */
typedef struct {
	PE_H15A0_DNR1_REG_C420_CTRL_T          	reg_c420_ctrl;           //0xc800719cL
	PE_H15A0_DNR1_C_PRE_BLUR_CTRL_0_T      	c_pre_blur_ctrl_0;       //0xc8007288L
}PE_PBL_HW_PARAM_REG_H15A0_T;

/**
 * pbl(pblur_dnr) hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_PBL_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_PBL_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_PBL_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_PBL_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_PBL_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_PBLUR_DNR_HW_PARAM_H15_H_ */
