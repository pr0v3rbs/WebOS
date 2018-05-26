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

/** @file pe_nrd_hw_m14b0.h
 *
 *  driver header for picture enhance noise reduction parameters. ( used only within kdriver )
 *	- initial settings
 *	- default settings for each format
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.10.28
 *	@see		
 */

#ifndef	_PE_NRD_HW_M14B0_H_
#define	_PE_NRD_HW_M14B0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_reg.h"

/* tnr */
#include "m14b0/pe_nr_l_init_m14b0.h"
#include "m14b0/pe_tnr_l_hd_i_default_m14b0.h"
#include "m14b0/pe_tnr_l_hd_p_default_m14b0.h"
#include "m14b0/pe_tnr_l_hd_i_default_for_oled_m14b0.h"
#include "m14b0/pe_tnr_l_hd_p_default_for_oled_m14b0.h"
#include "m14b0/pe_tnr_l_comp_hd_i_3d_default_m14b0.h"
#include "m14b0/pe_tnr_l_comp_hd_p_3d_default_m14b0.h"
#include "m14b0/pe_tnr_l_pc_default_m14b0.h"
#include "m14b0/pe_tnr_l_dtv_sd_480_i_default_m14b0.h"
#include "m14b0/pe_tnr_l_dtv_sd_576_i_default_m14b0.h"
#include "m14b0/pe_tnr_l_sd_i_default_m14b0.h"
#include "m14b0/pe_tnr_l_dtv_sd_p_default_m14b0.h"
#include "m14b0/pe_tnr_l_sd_p_default_m14b0.h"
#include "m14b0/pe_tnr_l_av_ntsc_default_m14b0.h"
#include "m14b0/pe_tnr_l_av_pal_default_m14b0.h"
#include "m14b0/pe_tnr_l_av_secam_default_m14b0.h"

#include "m14c0/pe_tnr_l_hd_i_default_m14c0.h"
#include "m14c0/pe_tnr_l_hd_p_default_m14c0.h"
#include "m14c0/pe_tnr_l_hd_i_default_for_oled_m14c0.h"
#include "m14c0/pe_tnr_l_hd_p_default_for_oled_m14c0.h"
#include "m14c0/pe_tnr_l_comp_hd_i_3d_default_m14c0.h"
#include "m14c0/pe_tnr_l_comp_hd_p_3d_default_m14c0.h"
#include "m14c0/pe_tnr_l_pc_default_m14c0.h"
#include "m14c0/pe_tnr_l_dtv_sd_480_i_default_m14c0.h"
#include "m14c0/pe_tnr_l_dtv_sd_576_i_default_m14c0.h"
#include "m14c0/pe_tnr_l_sd_i_default_m14c0.h"
#include "m14c0/pe_tnr_l_dtv_sd_p_default_m14c0.h"
#include "m14c0/pe_tnr_l_sd_p_default_m14c0.h"
#include "m14c0/pe_tnr_l_av_ntsc_default_m14c0.h"
#include "m14c0/pe_tnr_l_av_pal_default_m14c0.h"
#include "m14c0/pe_tnr_l_av_secam_default_m14c0.h"

/* tnr alpha lut */
#include "m14b0/pe_tnr_l_alphalut_init_ai_m14b0.h"
#include "m14b0/pe_tnr_alphalut_l_hd_i_default_m14b0.h"
#include "m14b0/pe_tnr_alphalut_l_hd_p_default_m14b0.h"
#include "m14b0/pe_tnr_alphalut_l_sd_i_default_m14b0.h"
#include "m14b0/pe_tnr_alphalut_l_sd_p_default_m14b0.h"
#include "m14b0/pe_tnr_alphalut_l_secam_default_m14b0.h"
/* dnr */
#include "m14b0/pe_dnr_l_atv_default_m14b0.h"
#include "m14b0/pe_dnr_l_av_default_m14b0.h"
#include "m14b0/pe_dnr_l_hd_default_m14b0.h"
#include "m14b0/pe_dnr_l_pc_default_m14b0.h"
#include "m14b0/pe_dnr_l_sd_default_m14b0.h"

#include "m14c0/pe_dnr_l_atv_default_m14c0.h"
#include "m14c0/pe_dnr_l_av_default_m14c0.h"
#include "m14c0/pe_dnr_l_hd_i_default_m14c0.h"
#include "m14c0/pe_dnr_l_hd_p_default_m14c0.h"
#include "m14c0/pe_dnr_l_pc_default_m14c0.h"
#include "m14c0/pe_dnr_l_sd_i_default_m14c0.h"
#include "m14c0/pe_dnr_l_sd_p_default_m14c0.h"


/* ifc */
#include "m14b0/pe_ifc_l_atv_ntsc_default_m14b0.h"
#include "m14b0/pe_ifc_l_pc_default_m14b0.h"

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

#endif /* _PE_NRD_HW_M14B0_H_ */

