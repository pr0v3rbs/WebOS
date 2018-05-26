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

/** @file pe_dnr_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_DNR_HW_PARAM_H15_H_
#define	_PE_DNR_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_dnr_l_init_h15a0.h"
#include "pe_dnr_l_atv_480_default_h15a0.h"
#include "pe_dnr_l_atv_576_default_h15a0.h"
#include "pe_dnr_l_dtv_sd_default_h15a0.h"
#include "pe_dnr_l_av_default_h15a0.h"
#include "pe_dnr_l_hd_default_h15a0.h"
#include "pe_dnr_l_pc_default_h15a0.h"
#include "pe_dnr_l_sd_default_h15a0.h"

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
 *	dnr hw param reg type
 */
typedef struct {
	PE_H15A0_DNR1_DNR_MAX_CTRL_T           	dnr_max_ctrl;            //0xc8007194L
	PE_H15A0_DNR1_DNR_DBAR_CTRL_T          	dnr_dbar_ctrl;           //0xc8007198L
	PE_H15A0_DNR1_MNR_CTRL_0_T             	mnr_ctrl_0;              //0xc80071a0L
	PE_H15A0_DNR1_MNR_CTRL_1_T             	mnr_ctrl_1;              //0xc80071a4L
	PE_H15A0_DNR1_MNR_CTRL_2_T             	mnr_ctrl_2;              //0xc80071a8L
	PE_H15A0_DNR1_MNR_CTRL_3_T             	mnr_ctrl_3;              //0xc80071acL
	PE_H15A0_DNR1_MNR_CTRL_4_T             	mnr_ctrl_4;              //0xc80071b0L
	PE_H15A0_DNR1_MNR_CTRL_5_T             	mnr_ctrl_5;              //0xc80071b4L
	PE_H15A0_DNR1_DETAIL_CTRL_T            	detail_ctrl;             //0xc80071b8L
	PE_H15A0_DNR1_DC_BNR_CTRL_0_T          	dc_bnr_ctrl_0;           //0xc80071bcL
	PE_H15A0_DNR1_DC_BNR_CTRL_1_T          	dc_bnr_ctrl_1;           //0xc80071c0L
	PE_H15A0_DNR1_DC_BNR_CTRL_2_T          	dc_bnr_ctrl_2;           //0xc80071c4L
	PE_H15A0_DNR1_DC_BNR_CTRL_3_T          	dc_bnr_ctrl_3;           //0xc80071c8L
	PE_H15A0_DNR1_DC_BNR_CTRL_4_T          	dc_bnr_ctrl_4;           //0xc80071ccL
	PE_H15A0_DNR1_DC_BNR_CTRL_5_T          	dc_bnr_ctrl_5;           //0xc80071d0L
	PE_H15A0_DNR1_AC_BNR_CTRL_0_T          	ac_bnr_ctrl_0;           //0xc80071d4L
	PE_H15A0_DNR1_AC_BNR_CTRL_1_T          	ac_bnr_ctrl_1;           //0xc80071d8L
	PE_H15A0_DNR1_AC_BNR_CTRL_2_T          	ac_bnr_ctrl_2;           //0xc80071dcL
	PE_H15A0_DNR1_AC_BNR_CTRL_3_T          	ac_bnr_ctrl_3;           //0xc80071e4L
	PE_H15A0_DNR1_AC_BNR_CTRL_4_T          	ac_bnr_ctrl_4;           //0xc80071e8L
	PE_H15A0_DNR1_AC_BNR_CTRL_5_T          	ac_bnr_ctrl_5;           //0xc80071f0L
	PE_H15A0_DNR1_AC_BNR_CTRL_6_T          	ac_bnr_ctrl_6;           //0xc80071f4L
	PE_H15A0_DNR1_AC_BNR_CTRL_7_T          	ac_bnr_ctrl_7;           //0xc80071f8L
	PE_H15A0_DNR1_AC_BNR_CTRL_8_T          	ac_bnr_ctrl_8;           //0xc8007200L
	PE_H15A0_DNR1_AC_BNR_CTRL_9_T          	ac_bnr_ctrl_9;           //0xc8007204L
	PE_H15A0_DNR1_AC_BNR_CTRL_10_T         	ac_bnr_ctrl_10;          //0xc8007208L
	PE_H15A0_DNR1_AC_BNR_CTRL_11_T         	ac_bnr_ctrl_11;          //0xc800720cL
	PE_H15A0_DNR1_AC_BNR_CTRL_12_T         	ac_bnr_ctrl_12;          //0xc8007210L
	PE_H15A0_DNR1_SC_BNR_CTRL_0_T          	sc_bnr_ctrl_0;           //0xc800726cL
	PE_H15A0_DNR1_SC_BNR_CTRL_1_T          	sc_bnr_ctrl_1;           //0xc8007270L
	PE_H15A0_DNR1_SC_BNR_CTRL_2_T          	sc_bnr_ctrl_2;           //0xc8007274L
	PE_H15A0_DNR1_SC_BNR_CTRL_3_T          	sc_bnr_ctrl_3;           //0xc8007278L
	PE_H15A0_DNR1_SC_BNR_CTRL_4_T          	sc_bnr_ctrl_4;           //0xc800727cL
	PE_H15A0_DNR1_SC_BNR_CTRL_5_T          	sc_bnr_ctrl_5;           //0xc8007280L
	PE_H15A0_DNR1_SC_BNR_CTRL_6_T          	sc_bnr_ctrl_6;           //0xc8007284L
	PE_H15A0_DNR1_TEXTURE_CTRL_0_T         	texture_ctrl_0;          //0xc800728cL
}PE_DNR_HW_PARAM_REG_H15A0_T;

/**
 *	dnr hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_DNR_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_DNR_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_DNR_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_DNR_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_DNR_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_DNR_HW_PARAM_H15_H_ */
