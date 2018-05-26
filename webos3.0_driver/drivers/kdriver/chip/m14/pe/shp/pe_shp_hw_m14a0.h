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
 *	@date		2012.04.27
 *	@see		
 */

#ifndef	_PE_SHP_HW_M14A0_H_
#define	_PE_SHP_HW_M14A0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "m14a0/pe_shp_l_init_m14a0.h"
#include "m14a0/pe_shp_r_init_m14a0.h"
#include "m14a0/pe_shp_l_rf_ntsc_default_m14a0.h"
#include "m14a0/pe_shp_l_rf_pal_default_m14a0.h"
#include "m14a0/pe_shp_l_av_ntsc_default_m14a0.h"
#include "m14a0/pe_shp_l_av_pal_default_m14a0.h"
#include "m14a0/pe_shp_l_pc_default_m14a0.h"
#include "m14a0/pe_shp_l_comp480i_default_m14a0.h"
#include "m14a0/pe_shp_l_comp480p_default_m14a0.h"
#include "m14a0/pe_shp_l_hdmi480i_default_m14a0.h"
#include "m14a0/pe_shp_l_hdmi480p_default_m14a0.h"
#include "m14a0/pe_shp_l_hdmi_hd_default_m14a0.h"
#include "m14a0/pe_shp_l_dtv_sd_default_m14a0.h"
#include "m14a0/pe_shp_l_dtv_hd_default_m14a0.h"
#include "m14a0/pe_shp_r_rf_ntsc_default_m14a0.h"
#include "m14a0/pe_shp_r_rf_pal_default_m14a0.h"
#include "m14a0/pe_shp_r_av_ntsc_default_m14a0.h"
#include "m14a0/pe_shp_r_av_pal_default_m14a0.h"
#include "m14a0/pe_shp_r_pc_default_m14a0.h"
#include "m14a0/pe_shp_r_comp480i_default_m14a0.h"
#include "m14a0/pe_shp_r_comp480p_default_m14a0.h"
#include "m14a0/pe_shp_r_hdmi480i_default_m14a0.h"
#include "m14a0/pe_shp_r_hdmi480p_default_m14a0.h"
#include "m14a0/pe_shp_r_hdmi_hd_default_m14a0.h"
#include "m14a0/pe_shp_r_dtv_sd_default_m14a0.h"
#include "m14a0/pe_shp_r_dtv_hd_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_hd_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_pc_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_av_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_avrf_secam_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_dtv_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_hdmi_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_rf_nt_default_m14a0.h"
#include "m14a0/pe_vflt_c_l_sd_rf_pal_default_m14a0.h"

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

#endif /* _PE_SHP_HW_M14A0_H_ */
