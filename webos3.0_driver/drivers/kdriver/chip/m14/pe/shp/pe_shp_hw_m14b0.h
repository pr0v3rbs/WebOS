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

/** @file pe_shp_hw_m14a0.h
 *
 *  driver header for picture enhance sharpness parameters. ( used only within kdriver )
 *	- initial settings
 *	- default settings for each format
 *	- name definition...
 *		1. init param	: [module]_[l/r]_init_param_[chip_rev]
 *		2. default param: [module]_[l/r]_[sub_module]_default_[input]_[sd/hd]_[info]_param_[chip_rev]
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.09.15
 *	@see		
 */

#ifndef	_PE_SHP_HW_M14B0_H_
#define	_PE_SHP_HW_M14B0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_reg.h"

/* shp */
#include "m14b0/pe_shp_l_init_m14b0.h"
#include "m14b0/pe_shp_l_atv_default_m14b0.h"
#include "m14b0/pe_shp_l_av_480_default_m14b0.h"
#include "m14b0/pe_shp_l_av_576_default_m14b0.h"
#include "m14b0/pe_shp_l_hd_default_m14b0.h"
#include "m14b0/pe_shp_l_pc_default_m14b0.h"
#include "m14b0/pe_shp_l_sd_480_default_m14b0.h"
#include "m14b0/pe_shp_l_sd_576_default_m14b0.h"

#include "m14c0/pe_shp_l_atv_default_m14c0.h"
#include "m14c0/pe_shp_l_av_480_default_m14c0.h"
#include "m14c0/pe_shp_l_av_576_default_m14c0.h"
#include "m14c0/pe_shp_l_hd_default_m14c0.h"
#include "m14c0/pe_shp_l_pc_default_m14c0.h"
#include "m14c0/pe_shp_l_sd_480_default_m14c0.h"
#include "m14c0/pe_shp_l_sd_576_default_m14c0.h"

/* sre */
#include "m14b0/pe_sre_l_init_m14b0.h"
#include "m14b0/pe_sre_l_atv_default_m14b0.h"
#include "m14b0/pe_sre_l_av_ntsc_default_m14b0.h"
#include "m14b0/pe_sre_l_av_pal_default_m14b0.h"
#include "m14b0/pe_sre_l_dtv_hd_default_m14b0.h"
#include "m14b0/pe_sre_l_dtv_sd_default_m14b0.h"
#include "m14b0/pe_sre_l_hdmi_hd_default_m14b0.h"
#include "m14b0/pe_sre_l_hdmi_sd_default_m14b0.h"
#include "m14b0/pe_sre_l_pc_default_m14b0.h"
/* sre lut */
#include "m14b0/pe_sre_l_lut_init_ai_m14b0.h"
#include "m14b0/pe_sre_l_lut_hd_default_ai_m14b0.h"
#include "m14b0/pe_sre_l_lut_sd_default_ai_m14b0.h"
/* vfilter c */
#include "m14b0/pe_vflt_c_l_atv_nt_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_atv_pal_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_av_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_comp_sd_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_dtv_sd_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_hd_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_hdmi_sd_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_pc_default_m14b0.h"
#include "m14b0/pe_vflt_c_l_secam_default_m14b0.h"
/* cti p0d */
#include "m14b0/pe_cti_p0d_l_atv_ntsc_default_m14b0.h"
#include "m14b0/pe_cti_p0d_l_atv_pal_default_m14b0.h"
#include "m14b0/pe_cti_p0d_l_sd_default_m14b0.h"
#include "m14b0/pe_cti_p0d_l_hd_default_m14b0.h"
#include "m14b0/pe_cti_p0d_l_pc_default_m14b0.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SHP_HW_M14B0_H_ */
