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

/** @file pe_msc_hw_param_h15.h
 *
 *  hw header for picture enhance. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2013.04.28
 *	@see		
 */

#ifndef	_PE_MSC_HW_PARAM_H15_H_
#define	_PE_MSC_HW_PARAM_H15_H_

/*----------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------*/
#include "pe_reg.h"

#include "pe_msc_06t_m_flt_0006_h15.h"
#include "pe_msc_06t_m_flt_0306_h15.h"
#include "pe_msc_12t_m_flt_0002_h15.h"
#include "pe_msc_12t_m_flt_0003_h15.h"
#include "pe_msc_12t_m_flt_0006_h15.h"
#include "pe_msc_12t_m_flt_0304_h15.h"
#include "pe_msc_12t_m_flt_0306_h15.h"
#include "pe_msc_12t_m_flt_0404_h15.h"
#include "pe_msc_12t_m_flt_0406_h15.h"
#include "pe_msc_12t_m_flt_0606_h15.h"
#include "pe_msc_12t_m_flt_0806_h15.h"
#include "pe_msc_12t_m_flt_1006_h15.h"
#include "pe_msc_12t_m_flt_1410_h15.h"
#include "pe_msc_12t_s_flt_0610_h15.h"
#include "pe_msc_12t_s_flt_1210_h15.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------*/
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_BASE		(120000)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0002		(120002)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0003		(120003)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0006		(120006)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0304		(120304)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0306		(120306)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0404		(120404)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0406		(120406)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0606		(120606)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0610		(120610)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_0806		(120806)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_1006		(121006)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_1210		(121210)
#define PE_SHP_HW_H15_SCL_CO_NUM_12T_1410		(121410)

#define PE_SHP_HW_H15_SCL_CO_NUM_06T_BASE		(60000)
#define PE_SHP_HW_H15_SCL_CO_NUM_06T_0006		(60006)
#define PE_SHP_HW_H15_SCL_CO_NUM_06T_0306		(60306)

#define PE_SHP_HW_H15_SCL_CO_NUM_H12T_M_DFLT	PE_SHP_HW_H15_SCL_CO_NUM_12T_0006
#define PE_SHP_HW_H15_SCL_CO_NUM_H12T_S_DFLT	PE_SHP_HW_H15_SCL_CO_NUM_12T_0610
#define PE_SHP_HW_H15_SCL_CO_NUM_V06T_M_DFLT	PE_SHP_HW_H15_SCL_CO_NUM_06T_0006
#define PE_SHP_HW_H15_SCL_CO_NUM_V12T_M_DFLT	PE_SHP_HW_H15_SCL_CO_NUM_12T_0006
#define PE_SHP_HW_H15_SCL_CO_NUM_V12T_S_DFLT	PE_SHP_HW_H15_SCL_CO_NUM_12T_0610

/*----------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_MSC_HW_PARAM_H15_H_ */

