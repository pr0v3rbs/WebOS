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

/** @file pe_res_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_RES_HW_PARAM_H15_H_
#define	_PE_RES_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_shp_l_init_h15a0.h"
#include "pe_sre_init_h15a0.h"
#include "pe_sre_sqm_init_h15a0.h"
#include "pe_sre_res_init_h15a0.h"
#include "pe_shp_l_atv_480_default_h15a0.h"
#include "pe_shp_l_atv_576_default_h15a0.h"
#include "pe_shp_l_dtv_sd_default_h15a0.h"
#include "pe_shp_l_sd_default_h15a0.h"
#include "pe_shp_l_hd_default_h15a0.h"
#include "pe_shp_l_uhd_default_h15a0.h"
#include "pe_shp_l_hdmi_hd_default_h15a0.h"
#include "pe_sre_res_atv_480_default_h15a0.h"
#include "pe_sre_res_atv_576_default_h15a0.h"
#include "pe_sre_res_dtv_sd_default_h15a0.h"
#include "pe_sre_res_sd_default_h15a0.h"
#include "pe_sre_res_hd_default_h15a0.h"
#include "pe_sre_res_hdmi_hd_default_h15a0.h"
#include "pe_sre_sd_default_h15a0.h"
#include "pe_sre_hd_default_h15a0.h"
#include "pe_sre_comp_hd_default_h15a0.h"
#include "pe_shp_l_pc_default_h15a0.h"
#include "pe_sre_res_pc_default_h15a0.h"
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
 *	shp hw param reg type
 */
typedef struct {
	PE_H15A0_SHPL_PE1_DERH_CTRL_00_T      	pe1_derh_ctrl_00;        //0xc800d4b0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_01_T      	pe1_derh_ctrl_01;        //0xc800d4b4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_02_T      	pe1_derh_ctrl_02;        //0xc800d4b8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_03_T      	pe1_derh_ctrl_03;        //0xc800d4bcL
	PE_H15A0_SHPL_PE1_DERH_CTRL_04_T      	pe1_derh_ctrl_04;        //0xc800d4c0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_05_T      	pe1_derh_ctrl_05;        //0xc800d4c4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_06_T      	pe1_derh_ctrl_06;        //0xc800d4c8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_07_T      	pe1_derh_ctrl_07;        //0xc800d4ccL
	PE_H15A0_SHPL_PE1_DERH_CTRL_08_T      	pe1_derh_ctrl_08;        //0xc800d4d0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_09_T      	pe1_derh_ctrl_09;        //0xc800d4d4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_0A_T      	pe1_derh_ctrl_0a;        //0xc800d4d8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_0B_T      	pe1_derh_ctrl_0b;        //0xc800d4dcL
	PE_H15A0_SHPL_PE1_DERH_CTRL_0C_T      	pe1_derh_ctrl_0c;        //0xc800d4e0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_10_T    	pe1_coring_ctrl_10;      //0xc800d4e4L
	PE_H15A0_SHPL_PE1_DERV_CTRL_0_T       	pe1_derv_ctrl_0;         //0xc800d4f0L
	PE_H15A0_SHPL_PE1_DERV_CTRL_1_T       	pe1_derv_ctrl_1;         //0xc800d4f4L
	PE_H15A0_SHPL_PE1_DERV_CTRL_2_T       	pe1_derv_ctrl_2;         //0xc800d4f8L
	PE_H15A0_SHPL_PE1_DERV_CTRL_3_T       	pe1_derv_ctrl_3;         //0xc800d4fcL
	PE_H15A0_SHPL_PE1_DP_CTRL_00_T        	pe1_dp_ctrl_00;          //0xc800d510L
	PE_H15A0_SHPL_PE1_DP_CTRL_01_T        	pe1_dp_ctrl_01;          //0xc800d514L
	PE_H15A0_SHPL_PE1_DP_CTRL_02_T        	pe1_dp_ctrl_02;          //0xc800d518L
	PE_H15A0_SHPL_PE1_DP_CTRL_03_T        	pe1_dp_ctrl_03;          //0xc800d51cL
	PE_H15A0_SHPL_PE1_SP_CTRL_00_T        	pe1_sp_ctrl_00;          //0xc800d530L
	PE_H15A0_SHPL_PE1_SP_CTRL_01_T        	pe1_sp_ctrl_01;          //0xc800d534L
	PE_H15A0_SHPL_PE1_SP_CTRL_02_T        	pe1_sp_ctrl_02;          //0xc800d538L
	PE_H15A0_SHPL_PE1_SP_CTRL_03_T        	pe1_sp_ctrl_03;          //0xc800d53cL
	PE_H15A0_SHPL_PE1_SP_CTRL_04_T        	pe1_sp_ctrl_04;          //0xc800d540L
	PE_H15A0_SHPL_PE1_SP_CTRL_05_T        	pe1_sp_ctrl_05;          //0xc800d544L
	PE_H15A0_SHPL_PE1_SP_CTRL_06_T        	pe1_sp_ctrl_06;          //0xc800d548L
	PE_H15A0_SHPL_PE1_SP_CTRL_07_T        	pe1_sp_ctrl_07;          //0xc800d54cL
	PE_H15A0_SHPL_PE1_SP_CTRL_08_T        	pe1_sp_ctrl_08;          //0xc800d550L
	PE_H15A0_SHPL_PE1_SP_CTRL_09_T        	pe1_sp_ctrl_09;          //0xc800d554L
	PE_H15A0_SHPL_PE1_SP_CTRL_0A_T        	pe1_sp_ctrl_0a;          //0xc800d558L
	PE_H15A0_SHPL_PE1_SP_CTRL_0B_T        	pe1_sp_ctrl_0b;          //0xc800d55cL
	PE_H15A0_SHPL_PE1_SP_CTRL_0C_T        	pe1_sp_ctrl_0c;          //0xc800d560L
	PE_H15A0_SHPL_PE1_MP_CTRL_00_T        	pe1_mp_ctrl_00;          //0xc800d570L
	PE_H15A0_SHPL_PE1_MP_CTRL_01_T        	pe1_mp_ctrl_01;          //0xc800d574L
	PE_H15A0_SHPL_PE1_MP_CTRL_02_T        	pe1_mp_ctrl_02;          //0xc800d578L
	PE_H15A0_SHPL_PE1_MP_CTRL_03_T        	pe1_mp_ctrl_03;          //0xc800d57cL
	PE_H15A0_SHPL_PE1_MP_CTRL_04_T        	pe1_mp_ctrl_04;          //0xc800d580L
	PE_H15A0_SHPL_PE1_MP_CTRL_05_T        	pe1_mp_ctrl_05;          //0xc800d584L
	PE_H15A0_SHPL_PE1_MP_CTRL_06_T        	pe1_mp_ctrl_06;          //0xc800d588L
	PE_H15A0_SHPL_PE1_MP_CTRL_07_T        	pe1_mp_ctrl_07;          //0xc800d58cL
	PE_H15A0_SHPL_PE1_MP_CTRL_08_T        	pe1_mp_ctrl_08;          //0xc800d590L
	PE_H15A0_SHPL_PE1_MP_CTRL_09_T        	pe1_mp_ctrl_09;          //0xc800d594L
	PE_H15A0_SHPL_PE1_MP_CTRL_0A_T        	pe1_mp_ctrl_0a;          //0xc800d598L
	PE_H15A0_SHPL_PE1_MP_CTRL_0B_T        	pe1_mp_ctrl_0b;          //0xc800d59cL
	PE_H15A0_SHPL_PE1_MP_CTRL_0C_T        	pe1_mp_ctrl_0c;          //0xc800d5a0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_00_T    	pe1_coring_ctrl_00;      //0xc800d5b0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_01_T    	pe1_coring_ctrl_01;      //0xc800d5b4L
	PE_H15A0_SHPL_PE1_CORING_CTRL_02_T    	pe1_coring_ctrl_02;      //0xc800d5b8L
	PE_H15A0_SHPL_PE1_CORING_CTRL_03_T    	pe1_coring_ctrl_03;      //0xc800d5bcL
	PE_H15A0_SHPL_PE1_CORING_CTRL_04_T    	pe1_coring_ctrl_04;      //0xc800d5c0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_05_T    	pe1_coring_ctrl_05;      //0xc800d5c4L
	PE_H15A0_SHPL_PE1_CORING_CTRL_06_T    	pe1_coring_ctrl_06;      //0xc800d5c8L
	PE_H15A0_SHPL_PE1_CORING_CTRL_07_T    	pe1_coring_ctrl_07;      //0xc800d5ccL
	PE_H15A0_SHPL_PE1_CORING_CTRL_08_T    	pe1_coring_ctrl_08;      //0xc800d5d0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_09_T    	pe1_coring_ctrl_09;      //0xc800d5d4L
	PE_H15A0_SHPL_PE1_PTI_CTRL_00_T       	pe1_pti_ctrl_00;         //0xc800d600L
	PE_H15A0_SHPL_PE1_PTI_CTRL_01_T       	pe1_pti_ctrl_01;         //0xc800d604L
	PE_H15A0_SHPL_PE1_STI_CTRL_00_T       	pe1_sti_ctrl_00;         //0xc800d608L
	PE_H15A0_SHPL_PE1_STI_CTRL_01_T       	pe1_sti_ctrl_01;         //0xc800d60cL
	PE_H15A0_SHPL_PE1_DJ_CTRL_00_T        	pe1_dj_ctrl_00;          //0xc800d620L
	PE_H15A0_SHPL_PE1_DJ_CTRL_01_T        	pe1_dj_ctrl_01;          //0xc800d624L
	PE_H15A0_SHPL_PE1_DJ_CTRL_02_T        	pe1_dj_ctrl_02;          //0xc800d628L
	PE_H15A0_SHPL_PE1_DJ_CTRL_03_T        	pe1_dj_ctrl_03;          //0xc800d62cL
	PE_H15A0_SHPL_PE1_CTI_CTRL_00_T       	pe1_cti_ctrl_00;         //0xc800d630L
	PE_H15A0_SHPL_PE1_CTI_CTRL_01_T       	pe1_cti_ctrl_01;         //0xc800d634L
	PE_H15A0_SHPL_PE1_CTI_CTRL_02_T       	pe1_cti_ctrl_02;         //0xc800d638L
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_00_T     	pe1_debug_ctrl_00;       //0xc800d650L
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_01_T     	pe1_debug_ctrl_01;       //0xc800d654L
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_02_T     	pe1_debug_ctrl_02;       //0xc800d658L	
	PE_H15A0_SHPL_PE1_TGEN_CTRL_00_T	    pe1_tgen_ctrl_00;        //0xc008d6a0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_01_T        pe1_tgen_ctrl_01;        //0xc008d6a4L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_02_T        pe1_tgen_ctrl_02;        //0xc008d6a8L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_03_T        pe1_tgen_ctrl_03;        //0xc008d6acL
	PE_H15A0_SHPL_PE1_TGEN_CTRL_04_T   	    pe1_tgen_ctrl_04;        //0xc008d6b0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_05_T        pe1_tgen_ctrl_05;        //0xc008d6b4L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_06_T        pe1_tgen_ctrl_06;        //0xc008d6b8L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_07_T        pe1_tgen_ctrl_07;        //0xc008d6bcL
	PE_H15A0_SHPL_PE1_TGEN_CTRL_08_T        pe1_tgen_ctrl_08;        //0xc008d6c0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_09_T        pe1_tgen_ctrl_09;        //0xc008d6c4L 	
}PE_RES_HW_PARAM_REG_H15A0_T;
/**
 *	sre shp(srs) hw param reg type
 */
typedef struct {
	PE_H15A0_SR_SHP_DERH_CTRL_00_T      	pe1_derh_ctrl_00;        //0xc800a0b0L
	PE_H15A0_SR_SHP_DERH_CTRL_01_T      	pe1_derh_ctrl_01;        //0xc800a0b4L
	PE_H15A0_SR_SHP_DERH_CTRL_02_T      	pe1_derh_ctrl_02;        //0xc800a0b8L
	PE_H15A0_SR_SHP_DERH_CTRL_03_T      	pe1_derh_ctrl_03;        //0xc800a0bcL
	PE_H15A0_SR_SHP_DERH_CTRL_04_T      	pe1_derh_ctrl_04;        //0xc800a0c0L
	PE_H15A0_SR_SHP_DERH_CTRL_05_T      	pe1_derh_ctrl_05;        //0xc800a0c4L
	PE_H15A0_SR_SHP_DERH_CTRL_06_T      	pe1_derh_ctrl_06;        //0xc800a0c8L
	PE_H15A0_SR_SHP_DERH_CTRL_07_T      	pe1_derh_ctrl_07;        //0xc800a0ccL
	PE_H15A0_SR_SHP_DERH_CTRL_08_T      	pe1_derh_ctrl_08;        //0xc800a0d0L
	PE_H15A0_SR_SHP_DERH_CTRL_09_T      	pe1_derh_ctrl_09;        //0xc800a0d4L
	PE_H15A0_SR_SHP_DERH_CTRL_0A_T      	pe1_derh_ctrl_0a;        //0xc800a0d8L
	PE_H15A0_SR_SHP_DERH_CTRL_0B_T      	pe1_derh_ctrl_0b;        //0xc800a0dcL
	PE_H15A0_SR_SHP_DERH_CTRL_0C_T      	pe1_derh_ctrl_0c;        //0xc800a0e0L
	PE_H15A0_SR_SHP_PE1_CORING_CTRL_10_T    pe1_coring_ctrl_10;      //0xc800a1d4L
	PE_H15A0_SR_SHP_DERV_CTRL_0_T       	pe1_derv_ctrl_0;         //0xc800a0f0L
	PE_H15A0_SR_SHP_DERV_CTRL_1_T       	pe1_derv_ctrl_1;         //0xc800a0f4L
	PE_H15A0_SR_SHP_DERV_CTRL_2_T       	pe1_derv_ctrl_2;         //0xc800a0f8L
	PE_H15A0_SR_SHP_DERV_CTRL_3_T       	pe1_derv_ctrl_3;         //0xc800a0fcL
	PE_H15A0_SR_SHP_DP_CTRL_00_T        	pe1_dp_ctrl_00;          //0xc800a110L
	PE_H15A0_SR_SHP_DP_CTRL_01_T        	pe1_dp_ctrl_01;          //0xc800a114L
	PE_H15A0_SR_SHP_DP_CTRL_02_T        	pe1_dp_ctrl_02;          //0xc800a118L
	PE_H15A0_SR_SHP_DP_CTRL_03_T        	pe1_dp_ctrl_03;          //0xc800a11cL
	PE_H15A0_SR_SHP_SP_CTRL_00_T        	pe1_sp_ctrl_00;          //0xc800a130L
	PE_H15A0_SR_SHP_SP_CTRL_01_T        	pe1_sp_ctrl_01;          //0xc800a134L
	PE_H15A0_SR_SHP_SP_CTRL_02_T        	pe1_sp_ctrl_02;          //0xc800a138L
	PE_H15A0_SR_SHP_SP_CTRL_03_T        	pe1_sp_ctrl_03;          //0xc800a13cL
	PE_H15A0_SR_SHP_SP_CTRL_04_T        	pe1_sp_ctrl_04;          //0xc800a140L
	PE_H15A0_SR_SHP_SP_CTRL_05_T        	pe1_sp_ctrl_05;          //0xc800a144L
	PE_H15A0_SR_SHP_SP_CTRL_06_T        	pe1_sp_ctrl_06;          //0xc800a148L
	PE_H15A0_SR_SHP_SP_CTRL_07_T        	pe1_sp_ctrl_07;          //0xc800a14cL
	PE_H15A0_SR_SHP_SP_CTRL_08_T        	pe1_sp_ctrl_08;          //0xc800a150L
	PE_H15A0_SR_SHP_SP_CTRL_09_T        	pe1_sp_ctrl_09;          //0xc800a154L
	PE_H15A0_SR_SHP_SP_CTRL_0A_T        	pe1_sp_ctrl_0a;          //0xc800a158L
	PE_H15A0_SR_SHP_SP_CTRL_0B_T        	pe1_sp_ctrl_0b;          //0xc800a15cL
	PE_H15A0_SR_SHP_SP_CTRL_0C_T        	pe1_sp_ctrl_0c;          //0xc800a160L
	PE_H15A0_SR_SHP_MP_CTRL_00_T        	pe1_mp_ctrl_00;          //0xc800a170L
	PE_H15A0_SR_SHP_MP_CTRL_01_T        	pe1_mp_ctrl_01;          //0xc800a174L
	PE_H15A0_SR_SHP_MP_CTRL_02_T        	pe1_mp_ctrl_02;          //0xc800a178L
	PE_H15A0_SR_SHP_MP_CTRL_03_T        	pe1_mp_ctrl_03;          //0xc800a17cL
	PE_H15A0_SR_SHP_MP_CTRL_04_T        	pe1_mp_ctrl_04;          //0xc800a180L
	PE_H15A0_SR_SHP_MP_CTRL_05_T        	pe1_mp_ctrl_05;          //0xc800a184L
	PE_H15A0_SR_SHP_MP_CTRL_06_T        	pe1_mp_ctrl_06;          //0xc800a188L
	PE_H15A0_SR_SHP_MP_CTRL_07_T        	pe1_mp_ctrl_07;          //0xc800a18cL
	PE_H15A0_SR_SHP_MP_CTRL_08_T        	pe1_mp_ctrl_08;          //0xc800a190L
	PE_H15A0_SR_SHP_MP_CTRL_09_T        	pe1_mp_ctrl_09;          //0xc800a194L
	PE_H15A0_SR_SHP_MP_CTRL_0A_T        	pe1_mp_ctrl_0a;          //0xc800a198L
	PE_H15A0_SR_SHP_MP_CTRL_0B_T        	pe1_mp_ctrl_0b;          //0xc800a19cL
	PE_H15A0_SR_SHP_MP_CTRL_0C_T        	pe1_mp_ctrl_0c;          //0xc800a1a0L
	PE_H15A0_SR_SHP_CORING_CTRL_00_T    	pe1_coring_ctrl_00;      //0xc800a1b0L
	PE_H15A0_SR_SHP_CORING_CTRL_01_T    	pe1_coring_ctrl_01;      //0xc800a1b4L
	PE_H15A0_SR_SHP_CORING_CTRL_02_T    	pe1_coring_ctrl_02;      //0xc800a1b8L
	PE_H15A0_SR_SHP_CORING_CTRL_03_T    	pe1_coring_ctrl_03;      //0xc800a1bcL
	PE_H15A0_SR_SHP_CORING_CTRL_04_T    	pe1_coring_ctrl_04;      //0xc800a1c0L
	PE_H15A0_SR_SHP_CORING_CTRL_05_T    	pe1_coring_ctrl_05;      //0xc800a1c4L
	PE_H15A0_SR_SHP_CORING_CTRL_06_T    	pe1_coring_ctrl_06;      //0xc800a1c8L
	PE_H15A0_SR_SHP_CORING_CTRL_07_T    	pe1_coring_ctrl_07;      //0xc800a1ccL
	PE_H15A0_SR_SHP_CORING_CTRL_08_T    	pe1_coring_ctrl_08;      //0xc800a1d0L
	PE_H15A0_SR_SHP_CORING_CTRL_09_T    	pe1_coring_ctrl_09;      //0xc800a1d4L
	PE_H15A0_SR_SHP_PTI_CTRL_00_T       	pe1_pti_ctrl_00;         //0xc800d600L
	PE_H15A0_SR_SHP_PTI_CTRL_01_T       	pe1_pti_ctrl_01;         //0xc800d604L
	PE_H15A0_SR_SHP_STI_CTRL_00_T       	pe1_sti_ctrl_00;         //0xc800d608L
	PE_H15A0_SR_SHP_STI_CTRL_01_T       	pe1_sti_ctrl_01;         //0xc800d60cL
	PE_H15A0_SR_SHP_DJ_CTRL_00_T        	pe1_dj_ctrl_00;          //0xc800d620L
	PE_H15A0_SR_SHP_DJ_CTRL_01_T        	pe1_dj_ctrl_01;          //0xc800d624L
	PE_H15A0_SR_SHP_DJ_CTRL_02_T        	pe1_dj_ctrl_02;          //0xc800d628L
	PE_H15A0_SR_SHP_DJ_CTRL_03_T        	pe1_dj_ctrl_03;          //0xc800d62cL
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_00_T     	pe1_debug_ctrl_00;       //0xc800dc50L
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_01_T     	pe1_debug_ctrl_01;       //0xc800dc54L
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_02_T     	pe1_debug_ctrl_02;       //0xc800dc58L
	PE_H15A0_SR_SHP_TGEN_CTRL_00_T	    	pe1_tgen_ctrl_00;        //0xc800a2a0L
	PE_H15A0_SR_SHP_TGEN_CTRL_01_T        	pe1_tgen_ctrl_01;        //0xc800a2a4L
	PE_H15A0_SR_SHP_TGEN_CTRL_02_T        	pe1_tgen_ctrl_02;        //0xc800a2a8L
	PE_H15A0_SR_SHP_TGEN_CTRL_03_T       	pe1_tgen_ctrl_03;        //0xc800a2acL
	PE_H15A0_SR_SHP_TGEN_CTRL_04_T   	    pe1_tgen_ctrl_04;        //0xc800a2b0L
	PE_H15A0_SR_SHP_TGEN_CTRL_05_T        	pe1_tgen_ctrl_05;        //0xc800a2b4L
	PE_H15A0_SR_SHP_TGEN_CTRL_06_T        	pe1_tgen_ctrl_06;        //0xc800a2b8L
	PE_H15A0_SR_SHP_TGEN_CTRL_07_T        	pe1_tgen_ctrl_07;        //0xc800a2bcL
	PE_H15A0_SR_SHP_TGEN_CTRL_08_T        	pe1_tgen_ctrl_08;        //0xc800a2c0L
	PE_H15A0_SR_SHP_TGEN_CTRL_09_T        	pe1_tgen_ctrl_09;        //0xc800a2c4L
}PE_SRS_HW_PARAM_REG_H15A0_T;

/**
 *	sre hw param reg type
 */
typedef struct {
	PE_H15A0_FT_GRAD_SM_T					ft_grad_sm; 				  //0xc8008000L
	PE_H15A0_FT_LOCALMM_VALUE_SCALE_T		ft_localmm_value_scale; 	  //0xc8008004L
	PE_H15A0_FT_LOCALMM_TH_T				ft_localmm_th;				  //0xc8008008L
	PE_H15A0_FT_LOCALMM_SH_T				ft_localmm_sh;				  //0xc800800cL
	PE_H15A0_FT_LTV_SH_T					ft_ltv_sh;					  //0xc8008010L
	PE_H15A0_FT_LTV_TH_DIFF_T				ft_ltv_th_diff; 			  //0xc8008014L
	PE_H15A0_FT_SHARP_SM_T					ft_sharp_sm;				  //0xc8008018L
	PE_H15A0_FT_SHARP_TH_T					ft_sharp_th;				  //0xc800801cL
	PE_H15A0_FT_SHARP_SCALE_SH_T			ft_sharp_scale_sh;			  //0xc8008020L
	PE_H15A0_FT_SHARP_CTRL_WE_T 			ft_sharp_ctrl_we;			  //0xc8008024L
	PE_H15A0_CORE1_IU_EN_LTV_TH_T			core1_iu_en_ltv_th; 		  //0xc8008080L
	PE_H15A0_CORE1_IU_EN_LTV_GRDAVG_TH_T	core1_iu_en_ltv_grdavg_th;	  //0xc8008084L
	PE_H15A0_CORE1_IU_EN_GNI_TH0_T			core1_iu_en_gni_th0;		  //0xc8008088L
	PE_H15A0_CORE1_IU_EN_GNI_TH1_T			core1_iu_en_gni_th1;		  //0xc800808cL
	PE_H15A0_CORE1_IU_EN_GNI_TH2_T			core1_iu_en_gni_th2;		  //0xc8008090L
	PE_H15A0_CORE1_IU_EN_GNI_TH3_T			core1_iu_en_gni_th3;		  //0xc8008094L
	PE_H15A0_CORE1_IU_EN_GRD_MUL_T			core1_iu_en_grd_mul;		  //0xc8008098L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG0_T		core1_lut_id_um_grdavg0;	  //0xc8008100L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG1_T		core1_lut_id_um_grdavg1;	  //0xc8008104L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG2_T		core1_lut_id_um_grdavg2;	  //0xc8008108L
	PE_H15A0_CORE1_LUT_ID_UM_GRDAVG3_T		core1_lut_id_um_grdavg3;	  //0xc800810cL
	PE_H15A0_CORE1_WE_DH_MINMAX_T			core1_we_dh_minmax; 		  //0xc8008110L
	PE_H15A0_CORE1_WE_DH_GRD_COST_WEI_T 	core1_we_dh_grd_cost_wei;	  //0xc8008114L
	PE_H15A0_CORE1_WE_DH_NOISE_TH1_T		core1_we_dh_noise_th1;		  //0xc8008118L
	PE_H15A0_CORE1_WE_DH_NOISE_TH2_T		core1_we_dh_noise_th2;		  //0xc800811cL
	PE_H15A0_CORE1_WE_DH_NOISE_STR_T		core1_we_dh_noise_str;		  //0xc8008120L
	PE_H15A0_CORE1_WE_DH_LTV_TH_T			core1_we_dh_ltv_th; 		  //0xc8008124L
	PE_H15A0_CORE1_WE_DH_LTV_GRD_TH_T		core1_we_dh_ltv_grd_th; 	  //0xc8008128L
	PE_H15A0_CORE1_WE_DH_LTV_GRD_STR_T		core1_we_dh_ltv_grd_str;	  //0xc800812cL
	PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH1_T 	core1_iu_um_clip_ltv_th1;	  //0xc8008140L
	PE_H15A0_CORE1_IU_UM_CLIP_LTV_TH2_T 	core1_iu_um_clip_ltv_th2;	  //0xc8008144L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG0_T		core1_lut_iu_um_grdavg0;	  //0xc8008148L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG1_T		core1_lut_iu_um_grdavg1;	  //0xc800814cL
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG2_T		core1_lut_iu_um_grdavg2;	  //0xc8008150L
	PE_H15A0_CORE1_LUT_IU_UM_GRDAVG3_T		core1_lut_iu_um_grdavg3;	  //0xc8008154L
	PE_H15A0_CORE1_WE_UH_MINMAX_T			core1_we_uh_minmax; 		  //0xc8008158L
	PE_H15A0_CORE1_WE_UH_GRD_COST_WEI_T 	core1_we_uh_grd_cost_wei;	  //0xc800815cL
	PE_H15A0_CORE1_WE_UH_NOISE_TH1_T		core1_we_uh_noise_th1;		  //0xc8008160L
	PE_H15A0_CORE1_WE_UH_NOISE_TH2_T		core1_we_uh_noise_th2;		  //0xc8008164L
	PE_H15A0_CORE1_WE_UH_NOISE_STR_T		core1_we_uh_noise_str;		  //0xc8008168L
	PE_H15A0_CORE1_WE_UH_LTV_TH_T			core1_we_uh_ltv_th; 		  //0xc800816cL
	PE_H15A0_CORE1_WE_UH_LTV_GRD_TH_T		core1_we_uh_ltv_grd_th; 	  //0xc8008170L
	PE_H15A0_CORE1_WE_UH_LTV_GRD_STR_T		core1_we_uh_ltv_grd_str;	  //0xc8008174L
	PE_H15A0_CORE1_WE_JONE_BASE_MUL_T     	core1_we_jone_base_mul;       //0xc80081a0L
	PE_H15A0_CORE1_WE_JONE_SS_SHIFT_T     	core1_we_jone_ss_shift;       //0xc80081a4L
	PE_H15A0_CORE1_WE_JONE_SS_R_T         	core1_we_jone_ss_r;           //0xc80081a8L
	PE_H15A0_CORE1_WE_JONE_MM_TH_T        	core1_we_jone_mm_th;          //0xc80081acL
	PE_H15A0_CORE1_WE_JONE_MINMAX_T       	core1_we_jone_minmax;         //0xc80081b0L
	PE_H15A0_CORE1_WE_JONE_BASE_T         	core1_we_jone_base;           //0xc80081b4L
	PE_H15A0_CORE1_WE_JONE_GRDAVG_TH1_T   	core1_we_jone_grdavg_th1;     //0xc80081b8L
	PE_H15A0_CORE1_WE_JONE_GRDAVG_TH2_T   	core1_we_jone_grdavg_th2;     //0xc80081bcL
	PE_H15A0_CORE1_WE_JONE_D_JO_GRDAVG_T  	core1_we_jone_d_jo_grdavg;    //0xc80081c0L
	PE_H15A0_CORE1_WE_JONE_R_GRD_MUL1_T   	core1_we_jone_r_grd_mul1;     //0xc80081c4L
	PE_H15A0_CORE1_WE_JONE_R_GRD_MUL2_T   	core1_we_jone_r_grd_mul2;     //0xc80081c8L
	PE_H15A0_CORE1_WE_JONE_R_GRD_OFFSET_T 	core1_we_jone_r_grd_offset;   //0xc80081ccL
	PE_H15A0_CORE1_WE_JONE_D_RGR1_T       	core1_we_jone_d_rgr1;         //0xc80081d0L
	PE_H15A0_CORE1_WE_JONE_D_RGR2_T       	core1_we_jone_d_rgr2;         //0xc80081d4L
	PE_H15A0_CORE1_WE_ID_LQ_GRD_TH_T      	core1_we_id_lq_grd_th;        //0xc8008200L
	PE_H15A0_CORE1_WE_ID_LQ_GRD_RATIO_T   	core1_we_id_lq_grd_ratio;     //0xc8008204L
	PE_H15A0_CORE1_WE_ID_LQ_LTV_MIN_MAX_T 	core1_we_id_lq_ltv_min_max;   //0xc8008208L
	PE_H15A0_CORE1_WE_ID_LQ_LTV_MUL_T     	core1_we_id_lq_ltv_mul;       //0xc800820cL
	PE_H15A0_CORE1_WE_ID_LQ_GRDAVG_CLIP_T 	core1_we_id_lq_grdavg_clip;   //0xc8008210L
	PE_H15A0_CORE1_WE_ID_WHQ_BASE_T       	core1_we_id_whq_base;         //0xc8008214L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL0_T 	core1_we_id_hq_grdavg_mul0;   //0xc8008218L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH1_T     	core1_we_id_hq_ltv_th1;       //0xc800821cL
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH2_T     	core1_we_id_hq_ltv_th2;       //0xc8008220L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_GRD_TH_T  	core1_we_id_hq_ltv_grd_th;    //0xc8008224L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD1_T	core1_we_id_m_whq_ltv1_grd1;  //0xc8008228L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV1_GRD2_T	core1_we_id_m_whq_ltv1_grd2;  //0xc800822cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV2_GRD_T 	core1_we_id_m_whq_ltv2_grd;   //0xc8008230L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV3_GRD_T 	core1_we_id_m_whq_ltv3_grd;   //0xc8008234L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH1_T	core1_we_id_hq_ltv_th_high1;  //0xc8008238L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_TH_HIGH2_T	core1_we_id_hq_ltv_th_high2;  //0xc800823cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LTV_HIGH_T 	core1_we_id_m_whq_ltv_high;   //0xc8008240L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH1_T  	core1_we_id_hq_grdavg_th1;    //0xc8008244L
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_TH2_T  	core1_we_id_hq_grdavg_th2;    //0xc8008248L
	PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG1_T  	core1_we_id_m_whq_grdavg1;    //0xc800824cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_GRDAVG2_T  	core1_we_id_m_whq_grdavg2;    //0xc8008250L
	PE_H15A0_CORE1_WE_ID_HQ_SS_TH1_T      	core1_we_id_hq_ss_th1;        //0xc8008254L
	PE_H15A0_CORE1_WE_ID_HQ_SS_TH2_T      	core1_we_id_hq_ss_th2;        //0xc8008258L
	PE_H15A0_CORE1_WE_ID_M_WHQ_SS1_T      	core1_we_id_m_whq_ss1;        //0xc800825cL
	PE_H15A0_CORE1_WE_ID_HQ_GRDAVG_MUL1_T 	core1_we_id_hq_grdavg_mul1;   //0xc8008260L
	PE_H15A0_CORE1_WE_ID_RATIO_HQ_T       	core1_we_id_ratio_hq;         //0xc8008264L
	PE_H15A0_CORE1_WE_ID_LTV_1B_T         	core1_we_id_ltv_1b;           //0xc8008268L
	PE_H15A0_CORE1_WE_ID_HQ_LTV_LTH_T     	core1_we_id_hq_ltv_lth;       //0xc800826cL
	PE_H15A0_CORE1_WE_ID_HQ_LTV_HTH_T     	core1_we_id_hq_ltv_hth;       //0xc8008270L
	PE_H15A0_CORE1_WE_ID_HQ_GRD_TH_T      	core1_we_id_hq_grd_th;        //0xc8008274L
	PE_H15A0_CORE1_WE_ID_HQ_R_T           	core1_we_id_hq_r;             //0xc8008278L
	PE_H15A0_CORE1_WE_ID_HQ_LMM_TH_T      	core1_we_id_hq_lmm_th;        //0xc800827cL
	PE_H15A0_CORE1_WE_ID_M_WHQ_LMM_T      	core1_we_id_m_whq_lmm;        //0xc8008280L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH1_T	core1_we_id_hq_low_grdavg_th1;//0xc8008284L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_GRDAVG_TH2_T	core1_we_id_hq_low_grdavg_th2;//0xc8008288L
	PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_T    	core1_we_id_hq_low_rgrd;      //0xc800828cL
	PE_H15A0_CORE1_WE_ID_HQ_LOW_RGRD_OFFSET_T	core1_we_id_hq_low_rgrd_offset;//0xc8008290L
	PE_H15A0_CORE1_WE_ID_M_WHQ_LOW_RGRD_T 	core1_we_id_m_whq_low_rgrd;   //0xc8008294L
	PE_H15A0_CORE1_WE_MAT_FS_SS1_T        	core1_we_mat_fs_ss1;          //0xc8008298L
	PE_H15A0_CORE1_WE_MAT_FS_SS2_T        	core1_we_mat_fs_ss2;          //0xc800829cL
	PE_H15A0_CORE1_WE_MAT_FS_LTV1_T       	core1_we_mat_fs_ltv1;         //0xc80082a0L
	PE_H15A0_CORE1_WE_MAT_FS_LTV2_T       	core1_we_mat_fs_ltv2;         //0xc80082a4L
	PE_H15A0_CORE1_WE_MAT_T               	core1_we_mat;                 //0xc80082a8L
	PE_H15A0_CORE1_WE_MAT_C_T             	core1_we_mat_c;               //0xc80082acL
	PE_H15A0_PQ_CLASS_THRESH_MINMAX1_T    	pq_class_thresh_minmax1;      //0xc8008300L
	PE_H15A0_PQ_CLASS_THRESH_MINMAX2_T    	pq_class_thresh_minmax2;      //0xc8008304L
	PE_H15A0_PQ_CLASS_THRESH_BLUR1_T      	pq_class_thresh_blur1;        //0xc8008308L
	PE_H15A0_PQ_CLASS_THRESH_BLUR2_T      	pq_class_thresh_blur2;        //0xc800830cL
	PE_H15A0_PQ_CLASS_THRESH_BLUR3_T      	pq_class_thresh_blur3;        //0xc8008310L
	PE_H15A0_PQ_CLASS_THRESH_BLUR4_T      	pq_class_thresh_blur4;        //0xc8008314L
	PE_H15A0_PQ_CLASS_THRESH_GRAD1_T      	pq_class_thresh_grad1;        //0xc8008318L
	PE_H15A0_PQ_CLASS_THRESH_GRAD2_T      	pq_class_thresh_grad2;        //0xc800831cL
	PE_H15A0_PQ_CLASS_THRESH_GRAD3_T      	pq_class_thresh_grad3;        //0xc8008320L
	PE_H15A0_PQ_CLASS_THRESH_GRAD4_T      	pq_class_thresh_grad4;        //0xc8008324L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD1_T  	pq_class_thresh_diffgrad1;    //0xc8008328L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD2_T  	pq_class_thresh_diffgrad2;    //0xc800832cL
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD3_T  	pq_class_thresh_diffgrad3;    //0xc8008330L
	PE_H15A0_PQ_CLASS_THRESH_DIFFGRAD4_T  	pq_class_thresh_diffgrad4;    //0xc8008334L
	PE_H15A0_PQ_CLASS_THRESH_SS0_T        	pq_class_thresh_ss0;          //0xc8008338L
	PE_H15A0_PQ_CLASS_THRESH_SS1_T        	pq_class_thresh_ss1;          //0xc800833cL
	PE_H15A0_PQ_CLASS_THRESH_SS2_T        	pq_class_thresh_ss2;          //0xc8008340L
	PE_H15A0_PQ_CLASS_THRESH_SS3_T        	pq_class_thresh_ss3;          //0xc8008344L
	PE_H15A0_FT_UP_GRD_BL23_0_T           	ft_up_grd_bl23_0;             //0xc8008500L
	PE_H15A0_FT_UP_GRD_BL23_1_T           	ft_up_grd_bl23_1;             //0xc8008504L
	PE_H15A0_FT_UP_GRDAVG_BL23_0_T        	ft_up_grdavg_bl23_0;          //0xc8008508L
	PE_H15A0_FT_UP_GRDAVG_BL23_1_T        	ft_up_grdavg_bl23_1;          //0xc800850cL
	PE_H15A0_FT_UP_SS_BL23_0_T            	ft_up_ss_bl23_0;              //0xc8008510L
	PE_H15A0_FT_UP_SS_BL23_1_T            	ft_up_ss_bl23_1;              //0xc8008514L
	PE_H15A0_FT_UP_LMM3_BL23_0_T          	ft_up_lmm3_bl23_0;            //0xc8008518L
	PE_H15A0_FT_UP_LMM3_BL23_1_T          	ft_up_lmm3_bl23_1;            //0xc800851cL
	PE_H15A0_FT_UP_LTV_BL23_0_T           	ft_up_ltv_bl23_0;             //0xc8008520L
	PE_H15A0_FT_UP_LTV_BL23_1_T           	ft_up_ltv_bl23_1;             //0xc8008524L
}PE_SRE_HW_PARAM_REG_H15A0_T;

/**
 *	shp hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_RES_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_RES_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_RES_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_RES_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_RES_HW_PARAM_DATA_H15A0_T;

/**
 *	sre shp(srs) hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_SRS_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_SRS_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_SRS_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_SRS_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_SRS_HW_PARAM_DATA_H15A0_T;

/**
 *	sre hw param data type
 */
typedef struct {
	union {
		UINT32			*addr;
		PE_SRE_HW_PARAM_REG_H15A0_T *data;
	} reg_dflt;

	union {
		UINT32			 *addr;
		PE_SRE_HW_PARAM_REG_H15A0_T *data;
	} reg_user;

	union {
		UINT32			*addr;
		PE_SRE_HW_PARAM_REG_H15A0_T *data;
	} reg_mask;

	union {
		UINT32			 *addr;
		PE_SRE_HW_PARAM_REG_H15A0_T *data;
	} reg_data;
} PE_SRE_HW_PARAM_DATA_H15A0_T;


/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_RES_HW_PARAM_H15_H_ */

