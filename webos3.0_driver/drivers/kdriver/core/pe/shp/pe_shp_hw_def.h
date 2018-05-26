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

/** @file pe_shp_hw_def.h
 *
 *  driver header for picture enhance sharpness parameters. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_SHP_HW_DEF_H_
#define	_PE_SHP_HW_DEF_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/**	MSC_CTRL_L 0x1100 [2]v_scaler_config
* - v12t mode '0' : Luminance H12tap/V12tap, Chrominance H08tap/V08tap (SD input stream)
*                   see PE_SHP_HW_XX_USE_V12TM_C08T_SEPARATE_HV
* - v06t mode '1' : Luminance H08tap/V06tap, Chrominance H08tap/V04tap (HD and SD input stream both)
*/
/* v12t mode luma horizontal : h12tap */
#define PE_SHP_SCL_HW_Y_H_12T_COEF_NUM	120
#define PE_SHP_SCL_HW_Y_H_12T_NUM		192

/* v12t mode luma vertical : v12tap */
#define PE_SHP_SCL_HW_Y_V_12T_COEF_NUM	120
#define PE_SHP_SCL_HW_Y_V_12T_NUM		192

/* v06t mode luma horizontal : h8tap */
#define PE_SHP_SCL_HW_Y_H_08T_COEF_NUM	66
#define PE_SHP_SCL_HW_Y_H_08T_NUM		112

/* v06t mode luma vertical : v6tap */
#define PE_SHP_SCL_HW_Y_V_06T_COEF_NUM	66
#define PE_SHP_SCL_HW_Y_V_06T_NUM		64

/* chroma 8tap */
#define PE_SHP_SCL_HW_C_08T_COEF_NUM	48
#define PE_SHP_SCL_HW_C_08T_NUM			80
/* chroma 4tap */
#define PE_SHP_SCL_HW_C_04T_COEF_NUM	48
#define PE_SHP_SCL_HW_C_04T_NUM			48

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
extern const PE_REG_PARAM_T shp_scl_y_h_08tap_dn75_coeff[PE_SHP_SCL_HW_Y_H_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_06tap_dn75_coeff[PE_SHP_SCL_HW_Y_V_06T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_08tap_dn75_coeff[PE_SHP_SCL_HW_C_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_04tap_dn75_coeff[PE_SHP_SCL_HW_C_04T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_h_08tap_dn50_coeff[PE_SHP_SCL_HW_Y_H_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_06tap_dn50_coeff[PE_SHP_SCL_HW_Y_V_06T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_08tap_dn50_coeff[PE_SHP_SCL_HW_C_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_04tap_dn50_coeff[PE_SHP_SCL_HW_C_04T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_h_08tap_dn25_coeff[PE_SHP_SCL_HW_Y_H_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_06tap_dn25_coeff[PE_SHP_SCL_HW_Y_V_06T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_08tap_dn25_coeff[PE_SHP_SCL_HW_C_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_04tap_dn25_coeff[PE_SHP_SCL_HW_C_04T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_h_12tap_dn50_coeff[PE_SHP_SCL_HW_Y_H_12T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_12tap_dn14_coeff[PE_SHP_SCL_HW_Y_V_12T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_h_12tap_coeff[PE_SHP_SCL_HW_Y_H_12T_COEF_NUM][PE_SHP_SCL_HW_Y_H_12T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_12tap_coeff[PE_SHP_SCL_HW_Y_V_12T_COEF_NUM][PE_SHP_SCL_HW_Y_V_12T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_h_08tap_coeff[PE_SHP_SCL_HW_Y_H_08T_COEF_NUM][PE_SHP_SCL_HW_Y_H_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_06tap_coeff[PE_SHP_SCL_HW_Y_V_06T_COEF_NUM][PE_SHP_SCL_HW_Y_V_06T_NUM];
extern const PE_REG_PARAM_T shp_scl_y_v_06tap_half_coeff[PE_SHP_SCL_HW_Y_V_06T_COEF_NUM][PE_SHP_SCL_HW_Y_V_06T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_08tap_coeff[PE_SHP_SCL_HW_C_08T_COEF_NUM][PE_SHP_SCL_HW_C_08T_NUM];
extern const PE_REG_PARAM_T shp_scl_c_04tap_coeff[PE_SHP_SCL_HW_C_04T_COEF_NUM][PE_SHP_SCL_HW_C_04T_NUM];

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SHP_HW_DEF_H_ */
