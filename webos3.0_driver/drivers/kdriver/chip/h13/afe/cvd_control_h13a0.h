/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 



#ifndef	_CVD_CONTROL_H13A0_H_
#define	_CVD_CONTROL_H13A0_H_

/** @file
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     wonsik.do
 *  @version    1.0
 *  @date       2012-04-25
 *  @note       Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "cvd_module.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define CVD_H13A0_NOISE_MAX	0x3FF

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int CVD_H13Ax_Init_Global_Params(void);
int CVD_H13Ax_AGC_Bypass_Function(UINT32 noise_status, UINT32 hlock_flag, UINT32 no_signal_flag );
int CVD_H13Ax_NoBurst_CKill_Control(CVD_STATUS_3CS_T *pCVD_Status, int clear_noburst_ckill);
int CVD_H13Ax_AGC_Peak_En_Control(int force_peak_enable, int avg_noise, int avg_agc_gain);
int CVD_H13Ax_Pattern_Detection(CVD_PATTERN_DETECTION_T *pCVD_Pattern_Detection_t );
int CVD_H13Ax_Set_PQ_Mode(LX_AFE_CVD_PQ_MODE_T cvd_pq_mode);
int CVD_H13Ax_Get_Reg_States_Detail(LX_AFE_CVD_STATES_DETAIL_T	*pcvd_states_detail_t);
int CVD_H13Ax_Set_for_Tunning(BOOLEAN bEnable);
int CVD_H13Ax_Cstate_Control_for_SC_PullInRange(CVD_STATUS_3CS_T *pCVD_Status, int init_cstate);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */
