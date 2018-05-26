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

/** @file pe_tnr_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_TNR_HW_PARAM_H15_H_
#define	_PE_TNR_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_tnr_l_init_h15a0.h"
#include "pe_tnr_l_hd_default_h15a0.h"
#include "pe_tnr_l_sd_default_h15a0.h"
#include "pe_tnr_l_ud_default_h15a0.h"
#include "pe_tnr_l_secam_default_h15a0.h"
#include "pe_tnr_l_cam_hd_default_h15a0.h"
#include "pe_tnr_l_atv_default_h15a0.h"

#include "pe_tnr_l_lut_init_h15a0.h"
#include "pe_tnr_l_lut_ud_default_h15a0.h"
#include "pe_tnr_l_lut_hd_default_h15a0.h"
#include "pe_tnr_l_lut_sd_default_h15a0.h"
#include "pe_tnr_l_lut_user_off_h15a0.h"
#include "pe_tnr_l_lut_user_low_h15a0.h"
#include "pe_tnr_l_lut_user_med_h15a0.h"
#include "pe_tnr_l_lut_user_high_h15a0.h"
#include "pe_tnr_l_lut_cam_hd_default_h15a0.h"


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
 *	tnr hw param reg type
 */
typedef struct {
	PE_H15A0_ND1_TNR_CTRL_00_T            	tnr_ctrl_00;              //0xc8004140L
	PE_H15A0_ND1_TNR_CTRL_01_T            	tnr_ctrl_01;              //0xc8004144L
	PE_H15A0_ND1_TNR_CTRL_02_T            	tnr_ctrl_02;              //0xc8004148L
	PE_H15A0_ND1_TNR_CTRL_03_T            	tnr_ctrl_03;              //0xc800414cL
	PE_H15A0_ND1_TNR_CTRL_04_T            	tnr_ctrl_04;              //0xc8004150L
	PE_H15A0_ND1_TNR_CTRL_05_T            	tnr_ctrl_05;              //0xc8004154L
	PE_H15A0_ND1_TNR_CTRL_06_T            	tnr_ctrl_06;              //0xc8004158L
	PE_H15A0_ND1_TNR_CTRL_07_T            	tnr_ctrl_07;              //0xc800415cL
	PE_H15A0_ND1_TNR_CTRL_08_T            	tnr_ctrl_08;              //0xc8004160L
	PE_H15A0_ND1_TNR_CTRL_09_T            	tnr_ctrl_09;              //0xc8004164L
	PE_H15A0_ND1_TNR_CTRL_10_T            	tnr_ctrl_10;              //0xc8004168L
	PE_H15A0_ND1_TNR_CTRL_11_T            	tnr_ctrl_11;              //0xc800416cL
	PE_H15A0_ND1_TNR_CTRL_16_T            	tnr_ctrl_16;              //0xc8004180L
	PE_H15A0_ND1_TNR_CTRL_17_T            	tnr_ctrl_17;              //0xc8004184L
	PE_H15A0_ND1_TNR_CTRL_18_T            	tnr_ctrl_18;              //0xc8004188L
	PE_H15A0_ND1_TNR_CTRL_19_T            	tnr_ctrl_19;              //0xc800418cL
	PE_H15A0_ND1_TNR_CTRL_20_T            	tnr_ctrl_20;              //0xc8004190L
	PE_H15A0_ND1_TNR_CTRL_21_T            	tnr_ctrl_21;              //0xc8004194L
	PE_H15A0_ND1_TNR_CTRL_24_T            	tnr_ctrl_24;              //0xc80041a0L
	PE_H15A0_ND1_TNR_CTRL_25_T            	tnr_ctrl_25;              //0xc80041a4L
	PE_H15A0_ND1_TNR_CTRL_26_T            	tnr_ctrl_26;              //0xc80041a8L
	PE_H15A0_ND1_TNR_CTRL_27_T            	tnr_ctrl_27;              //0xc80041acL
	PE_H15A0_ND1_TNR_CTRL_28_T            	tnr_ctrl_28;              //0xc80041b0L
	PE_H15A0_ND1_TNR_CTRL_29_T            	tnr_ctrl_29;              //0xc80041b4L
	PE_H15A0_ND1_TNR_CTRL_30_T            	tnr_ctrl_30;              //0xc80041b8L
	PE_H15A0_ND1_TNR_CTRL_31_T            	tnr_ctrl_31;              //0xc80041bcL
	PE_H15A0_ND1_TNR_CTRL_32_T            	tnr_ctrl_32;              //0xc80041e0L
	PE_H15A0_ND1_TNR_CTRL_40_T            	tnr_ctrl_40;              //0xc80043d0L
	PE_H15A0_ND1_TNR_CTRL_41_T            	tnr_ctrl_41;              //0xc80043d4L
	PE_H15A0_ND1_TNR_CTRL_42_T            	tnr_ctrl_42;              //0xc80043d8L
	PE_H15A0_ND1_TNR_CTRL_43_T            	tnr_ctrl_43;              //0xc80043dcL
	PE_H15A0_ND1_TNR_CTRL_44_T            	tnr_ctrl_44;              //0xc80043e0L
	PE_H15A0_ND1_TNR_CTRL_45_T            	tnr_ctrl_45;              //0xc80043e4L
	PE_H15A0_ND1_TNR_CTRL_46_T            	tnr_ctrl_46;              //0xc80043e8L
	PE_H15A0_ND1_TNR_CTRL_47_T            	tnr_ctrl_47;              //0xc80043ecL
	PE_H15A0_ND1_TNR_CTRL_48_T            	tnr_ctrl_48;              //0xc80043f0L
	PE_H15A0_ND1_TNR_CTRL_49_T            	tnr_ctrl_49;              //0xc80043f4L
	PE_H15A0_ND1_TNR_CTRL_50_T            	tnr_ctrl_50;              //0xc80043f8L
	PE_H15A0_ND1_TNR_CTRL_51_T            	tnr_ctrl_51;              //0xc80043fcL
}PE_TNR_HW_PARAM_REG_H15A0_T;

/**
 *	tnr hw param reg type
 */
typedef struct {
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_00;          //0xc80041d4L 00. x7y7 x6y6 reg_p2_blend_ctrl_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_01;          //0xc80041d4L 01. x5y5 x4y4 reg_p2_blend_ctrl_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_02;          //0xc80041d4L 02. x3y3 x2y2 reg_p2_blend_ctrl_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_03;          //0xc80041d4L 03. x1y1 x0y0 reg_p2_blend_ctrl_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_04;          //0xc80041d4L 04. x5y5 x4y4 reg_p2_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_05;          //0xc80041d4L 05. x3y3 x2y2 reg_p2_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_06;          //0xc80041d4L 06. x1y1 x0y0 reg_p2_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_07;          //0xc80041d4L 07. x7y7 x6y6 reg_detail_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_08;          //0xc80041d4L 08. x5y5 x4y4 reg_detail_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_09;          //0xc80041d4L 09. x3y3 x2y2 reg_detail_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_10;          //0xc80041d4L 10. x1y1 x0y0 reg_detail_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_11;          //0xc80041d4L 11. x7y7 x6y6 reg_detail_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_12;          //0xc80041d4L 12. x5y5 x4y4 reg_detail_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_13;          //0xc80041d4L 13. x3y3 x2y2 reg_detail_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_14;          //0xc80041d4L 14. x1y1 x0y0 reg_detail_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_15;          //0xc80041d4L 15. x7y7 x6y6 reg_flat_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_16;          //0xc80041d4L 16. x5y5 x4y4 reg_flat_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_17;          //0xc80041d4L 17. x3y3 x2y2 reg_flat_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_18;          //0xc80041d4L 18. x1y1 x0y0 reg_flat_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_19;          //0xc80041d4L 19. x7y7 x6y6 reg_flat_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_20;          //0xc80041d4L 20. x5y5 x4y4 reg_flat_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_21;          //0xc80041d4L 21. x3y3 x2y2 reg_flat_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_22;          //0xc80041d4L 22. x1y1 x0y0 reg_flat_cir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_23;          //0xc80041d4L 23. x1y1 x0y0 reg_luma_gain_ctrl
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_24;          //0xc80041d4L 24. x3x2 x1x0 reg_skin_gain_ctrl_yy
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_25;          //0xc80041d4L 25. x3x2 x1x0 reg_skin_gain_ctrl_cb
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_26;          //0xc80041d4L 26. x3x2 x1x0 reg_skin_gain_ctrl_cr
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_27;          //0xc80041d4L 27. x1y1 x0y0 reg_skin_motion_ctrl
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_28;          //0xc80041d4L 28. reg_detail_luma_ctrl
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_29;          //0xc80041d4L 29. x3y3 x2y2 reg_NLM_Protect
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_30;          //0xc80041d4L 30. x1y1 x0y0 reg_NLM_Protect
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_31;          //0xc80041d4L 31. x5y5 x4y4 reg_cc_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_32;          //0xc80041d4L 32. x3y3 x2y2 reg_cc_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_33;          //0xc80041d4L 33. x1y1 x0y0 reg_cc_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_34;          //0xc80041d4L 34. x5y5 x4y4 reg_ne_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_35;          //0xc80041d4L 35. x3y3 x2y2 reg_ne_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_36;          //0xc80041d4L 36. x1y1 x0y0 reg_ne_dist_max_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_37;          //0xc80041d4L 37. reserved
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_38;          //0xc80041d4L 38. reserved
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_39;          //0xc80041d4L 39. x7y7 x6y6 reg_simple_ma_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_40;          //0xc80041d4L 40. x5y5 x4y4 reg_simple_ma_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_41;          //0xc80041d4L 41. x3y3 x2y2 reg_simple_ma_iir_pt
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_lut_42;          //0xc80041d4L 42. x1y1 x0y0 reg_simple_ma_iir_pt
}PE_TNL_HW_PARAM_REG_H15A0_T;

/**
 *	tnr hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd1_user;

	union {
		UINT32			*addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd1_mask;

	union {
		UINT32			 *addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd1_data;

	union {
		UINT32			 *addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd2_user;

	union {
		UINT32			*addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd2_mask;

	union {
		UINT32			 *addr;
		PE_TNR_HW_PARAM_REG_H15A0_T *data;
	} nd2_data;
} PE_TNR_HW_PARAM_DATA_H15A0_T;

/**
 *	tnr lut (tnl) hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_TNL_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_TNL_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_TNL_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_TNL_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_TNL_HW_PARAM_DATA_H15A0_T;

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_TNR_HW_PARAM_H15_H_ */
