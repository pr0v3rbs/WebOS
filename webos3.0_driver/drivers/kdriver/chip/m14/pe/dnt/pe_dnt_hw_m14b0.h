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

/** @file pe_dnt_hw_m14b0.h
 *
 *  driver header for picture enhance deinterlacer parameters. ( used only within kdriver )
 *	- initial settings
 *	- default settings for each format
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.10.28
 *	@see		
 */

#ifndef	_PE_DNT_HW_M14B0_H_
#define	_PE_DNT_HW_M14B0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_reg.h"

/* ipc */
#include "m14b0/pe_ipc_l_init_m14b0.h"
#include "m14b0/pe_ipc_l_atv_480_default_m14b0.h"
#include "m14b0/pe_ipc_l_atv_576_default_m14b0.h"
#include "m14b0/pe_ipc_l_av_480_default_m14b0.h"
#include "m14b0/pe_ipc_l_av_576_default_m14b0.h"
#include "m14b0/pe_ipc_l_dtv_480_default_m14b0.h"
#include "m14b0/pe_ipc_l_dtv_576_default_m14b0.h"
#include "m14b0/pe_ipc_l_dtv_hd_default_m14b0.h"
#include "m14b0/pe_ipc_l_dtv_hd_tp_default_m14b0.h"
#include "m14b0/pe_ipc_l_hd_default_m14b0.h"
#include "m14b0/pe_ipc_l_pc_default_m14b0.h"
#include "m14b0/pe_ipc_l_sd_480_default_m14b0.h"
#include "m14b0/pe_ipc_l_sd_576_default_m14b0.h"
#include "m14b0/pe_ipc_l_secam_default_m14b0.h"

#include "m14c0/pe_ipc_l_atv_480_default_m14c0.h"
#include "m14c0/pe_ipc_l_atv_576_default_m14c0.h"
#include "m14c0/pe_ipc_l_av_480_default_m14c0.h"
#include "m14c0/pe_ipc_l_av_576_default_m14c0.h"
#include "m14c0/pe_ipc_l_dtv_480_default_m14c0.h"
#include "m14c0/pe_ipc_l_dtv_576_default_m14c0.h"
#include "m14c0/pe_ipc_l_dtv_hd_default_m14c0.h"
#include "m14c0/pe_ipc_l_dtv_hd_tp_default_m14c0.h"
#include "m14c0/pe_ipc_l_hd_default_m14c0.h"
#include "m14c0/pe_ipc_l_pc_default_m14c0.h"
#include "m14c0/pe_ipc_l_sd_480_default_m14c0.h"
#include "m14c0/pe_ipc_l_sd_576_default_m14c0.h"
#include "m14c0/pe_ipc_l_secam_default_m14c0.h"

/* clc */
#include "m14b0/pe_clc_l_init_m14b0.h"
#include "m14b0/pe_clc_l_hd_default_m14b0.h"
#include "m14b0/pe_clc_l_pc_default_m14b0.h"
#include "m14b0/pe_clc_l_sd_50_default_m14b0.h"
#include "m14b0/pe_clc_l_sd_60_default_m14b0.h"
/* vfilter y */
#include "m14b0/pe_vflt_y_l_init_m14b0.h"
#include "m14b0/pe_vflt_y_l_atv_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_av_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_hd_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_pc_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_sd_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_sd_dtv_default_m14b0.h"
#include "m14b0/pe_vflt_y_l_secam_default_m14b0.h"

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

#endif /* _PE_DNT_HW_M14B0_H_ */
