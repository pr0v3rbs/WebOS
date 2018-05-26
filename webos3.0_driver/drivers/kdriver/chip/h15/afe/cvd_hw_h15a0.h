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



/** @file
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     wonsik.do
 *  @version    1.0
 *  @date       2012-05-04
 *  @note       Additional information.
 */

#ifndef	_CVD_HW_H15A0_H_
#define	_CVD_HW_H15A0_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#undef	H15_STABLE_HSYNC_WORKAROUND
#define	H15_HSYNC_ENHANCEMENT
#define	H15_VSYNC_ENHANCEMENT

#define L9_DONT_KILL_CHROMALOCK_ON_CLOCK_LOST
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

	typedef struct {
		UINT32 	reg_comb3_buffer_size;
		UINT32 	reg_fld1_init_rd_pel;
		UINT32 	reg_fld2_init_rd_pel;
		UINT32 	reg_fld3_init_rd_pel;
		UINT32 	reg_fld4_init_rd_pel;

		UINT8	reg_cvd1_clampagc_on;
		//added 0716
		UINT8	reg_phase_offset_range;
		UINT8	reg_ispal_th;

		UINT8	reg_colour_mode;
		UINT8	reg_vline_625;
		UINT8	reg_hpixel;
		UINT8	reg_ped;
		UINT16	reg_adc_blank_level; 	//added 110411
		UINT8	reg_cagc_en;
		UINT8	reg_agc_half_en;
		UINT8	reg_dc_clamp_mode;
		UINT8	reg_hagc_field_mode;	//faster agc, but screen blinking

	   UINT8	reg_chroma_bw_lo;

		UINT8	reg_adaptive_mode;
		UINT8	reg_adaptive_3dcomb_mode;

		UINT8	reg_pal60_mode;

		UINT8	reg_hagc;
		UINT8	reg_yc_delay;		// no used
		UINT8	reg_cagc_gate_start;
		UINT8	reg_cagc_gate_end;

		UINT32	reg_cdto_inc;
		UINT32	reg_hdto_inc;

		//UINT8	reg_hactive_start;
		//UINT8	reg_hactive_width;
		UINT8	reg_vactive_start;
		UINT8	reg_vactive_height;
		UINT8	reg_ntsc443_mode;
		UINT8	reg_cagc;
		UINT8	reg_cagc_av;

		UINT8	reg_secam_ybw;
		UINT8	reg_auto_secam_level;
		UINT8	reg_lose_chromalock_mode;

		UINT8	reg_motion_mode;
		UINT8	reg_adaptive_chroma_mode;

		UINT8	reg_comb2d_only;
		UINT8	reg_fb_sync;

		// UINT8	reg_md_noise_th;
		// UINT8	reg_md_noise_th_en;
		UINT8	reg_vactive_md_start;
		UINT8	reg_vactive_md_height;

		UINT8	reg_hactive_md_start;
		UINT8	reg_hactive_md_width;

		//   UINT8	reg_motion_config;

		// 2dComb setting were here

		UINT8	reg_hstate_max;
		UINT8	reg_dcrestore_accum_width;

		UINT8	reg_hactive_start_54M;
		UINT8	reg_hactive_width_54M;

		//added 110212 (For 3D Comb operation on RF Signal)
		UINT8	reg_hnon_std_threshold;

		//added 110415 (Jo Jo Gunpo filed stream : no signal issue)
		UINT8	reg_vsync_signal_thresh;

		//added 110608 ( for PAL Test(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
		//111108 ( for All Color System(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
		UINT8	reg_fixed_cstate;
		UINT8	reg_cstate;

		//gogosing added 110610
		UINT8 cs_chroma_burst5or10;
		UINT8 cs1_chroma_burst5or10;

		//kim.min 0906
		UINT8	reg_hrs_ha_start;
		UINT8	reg_hrs_ha_width;

		//110908 for L9B0 cordic_gate settings
		UINT8	reg_cordic_gate_start_3CS;
		UINT8	reg_cordic_gate_end_3CS;

		//110908 for L9B0 burst_gate settings
		UINT8	reg_burst_gate_start_3CS;
		UINT8	reg_burst_gate_end_3CS;

		//110919 Scart RGB Align for each color system
		UINT8	reg_fb_vstart_odd;
		UINT8	reg_fb_vstart_even;
		UINT8	reg_fb_hstart;

		//111017 : New cagc/cordic/burst gate start/end values for valid color output from CVD
		UINT8	reg_cagc_gate_start_new;
		UINT8	reg_cagc_gate_end_new;
		UINT8	reg_cordic_gate_start_new;
		UINT8	reg_cordic_gate_end_new;
		UINT8	reg_burst_gate_start_new;
		UINT8	reg_burst_gate_end_new;

		//111213 : added reg_dcrestore_hsync_mid
		UINT8	reg_dcrestore_hsync_mid;

		//111215 : added reg_contrast
		UINT8	reg_contrast;
		UINT8	reg_contrast_av;
		//111230	: added reg_fb_height_half
		UINT8	reg_fb_height_half;

		//120105	: added reg_saturation for SECAM
		UINT8	reg_saturation;

		//gogosing added 110613
		// SECAM threshold for keeping PAL stable 20110613
		UINT8 cs_issecam_th;

		// added from M14B0 Revision
		UINT32	reg_comb3_buffer_size_md;
		UINT32	reg_fld1_init_rd_pel_md;

		// added 121207 for hstart of dcrestore_lpf_en ON
		UINT32	reg_rf_hrs_ha_start;

		//140113	: added for AKN reddish face problem : burst gate shifted and hue modified 
		UINT8	reg_hue;

		//140321 : added reg_dcrestore_hsync_mid_av for PAL-M VCR (AIWA MX100)
		UINT8	reg_dcrestore_hsync_mid_av;

	}	CVD_SET_SYSTEM_3CS_H15A0_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int CVD_H15Ax_ClampAGC_OnOff(UINT8 on_off);
int CVD_H15Ax_Clamp_Current_Control(UINT8 value);
void CVD_H15Ax_Set_Picture_Enhancement(BOOLEAN ycrdc_lcr_on, BOOLEAN cyrdc_lcr_on);
void CVD_H15Ax_Program_Color_System_Main_Multi(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
void CVD_H15Ax_Program_Color_System_CS(CVD_SELECT_CDETECT_T cs_sel, CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
void CVD_H15Ax_Program_Color_System_PreJob(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
void CVD_H15Ax_Program_Color_System_PreJob2(void);
void CVD_H15Ax_Program_Color_System_PostJob(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
void CVD_H15Ax_Program_Color_System_PostJob2(void);
void CVD_H15Ax_SW_Reset(LX_AFE_CVD_SELECT_T select_main_sub);
void CVD_H15Ax_Power_Down(LX_AFE_CVD_SELECT_T select_main_sub, BOOLEAN PowerOnOFF);
void CVD_H15Ax_Reg_Init(LX_AFE_CVD_SELECT_T select_main_sub);
int CVD_H15Ax_Set_Source_Type(LX_AFE_CVD_SET_INPUT_T	cvd_input_info);
int CVD_H15Ax_Set_Scart_Overlay(BOOLEAN arg);
int CVD_H15Ax_Get_Scart_FB_En(void);
int CVD_H15Ax_Set_Scart_FB_En(int fb_en_ctrl);
UINT8 CVD_H15Ax_Get_FC_Flag(LX_AFE_CVD_SELECT_T select_main_sub);
UINT8 CVD_H15Ax_Get_CVD_Burst_Mag_Flag(CVD_STATE_T	color_system);
int CVD_H15Ax_Set_CVD_CAGC(UINT8 state,CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t, CVD_STATE_T	color_system);
UINT8 CVD_H15Ax_Get_Cordic_Freq(LX_AFE_CVD_SELECT_T select_main_sub);
UINT8 CVD_H15Ax_Get_Cordic_Freq_CS0(void);
UINT8 CVD_H15Ax_Get_FC_Flag_CS0(void);
UINT8 CVD_H15Ax_Get_Cordic_Freq_CS1(void);
UINT8 CVD_H15Ax_Get_FC_Flag_CS1(void);
int CVD_H15Ax_Get_FB_Status(LX_AFE_SCART_MODE_T *pScart_fb_mode);
int CVD_H15Ax_Get_Scart_AR(LX_AFE_SCART_AR_INFO_T	*pScart_ar_param);
void CVD_H15Ax_Print_Vport_Version(void);
int CVD_H15Ax_Vport_Output(UINT32 arg);
int CVD_H15Ax_Vport_Reg_Read(UINT32 arg);
int CVD_H15Ax_Vport_Reg_Write(UINT32 arg);
int CVD_H15Ax_Get_No_Signal_Flag(void);
int CVD_H15Ax_Get_HLock_Flag(void);
int CVD_H15Ax_Get_VLock_Flag(void);
int CVD_H15Ax_Get_Vline_625_Flag(void);
int CVD_H15Ax_Get_Vdetect_Vcount_625_Flag(void);
int CVD_H15Ax_Get_PAL_Flag(void);
int CVD_H15Ax_Get_SECAM_Flag(void);
int CVD_H15Ax_Get_Chromalock_Flag(void);
int CVD_H15Ax_Get_PAL_Flag_CS0(void);
int CVD_H15Ax_Get_SECAM_Flag_CS0(void);
int CVD_H15Ax_Get_Chromalock_Flag_CS0(void);
int CVD_H15Ax_Get_PAL_Flag_CS1(void);
int CVD_H15Ax_Get_SECAM_Flag_CS1(void);
int CVD_H15Ax_Get_Chromalock_Flag_CS1(void);
int CVD_H15Ax_Get_Noise_Status(void);
int CVD_H15Ax_Get_NoBurst_Flag(void);
int CVD_H15Ax_Get_CHB_No_Signal_Flag(void);
int CVD_H15Ax_Get_CHB_HLock_Flag(void);
int CVD_H15Ax_Get_CHB_VLock_Flag(void);
int CVD_H15Ax_Get_CHB_Chromalock_Flag(void);
int CVD_H15Ax_Get_CHB_PAL_Flag(void);
int CVD_H15Ax_Get_CHB_Vline_625_Flag(void);
int CVD_H15Ax_Get_CHB_SECAM_Flag(void);
int CVD_H15Ax_Reset_irisyc(int enable);
int CVD_H15Ax_Reset_vdct(int enable);
int CVD_H15Ax_Reset_hdct(int enable);
int CVD_H15Ax_Reset_cdct(int enable);
int	CVD_H15Ax_Read_fld_cnt_value(UINT16 *pfld_hfcnt_value, UINT16 *pfld_lfcnt_value);
int CVD_H15Ax_Set_motion_mode(UINT8	md_mode_value, UINT8 motion_mode_value);
int CVD_H15Ax_Channel_Power_Control(UINT32 on_off); 
int CVD_H15Ax_Reset_Clampagc(void);
int CVD_H15Ax_Bypass_Control(LX_AFE_CVD_BYPASS_CONTROL_T *cvd_bypass_control_t);
int CVD_H15Ax_Get_Vline_625_Reg(void);
int CVD_H15Ax_OnOff_VDAC(BOOLEAN bonoff);
int CVD_H15Ax_Set_PE_Params(CVD_REG_PARAM_T	*pCVD_PE_Param_t, UINT32 size, LX_AFE_CVD_PQ_MODE_T cvd_pq_mode);
int CVD_H15Ax_swrst_CS(CVD_SELECT_CDETECT_T cs_sel);
int CVD_H15Ax_Set_Hstate_Max(UINT32	hstate_max_value);
int CVD_H15Ax_Set_Hstate_Fixed(UINT32	value);
int CVD_H15Ax_AGC_Bypass(int Bypass_Enable);
int CVD_H15Ax_Set_dcrestore_accum_width(int value);
int CVD_H15Ax_Set_SCART_CSC(int black_level);
int CVD_H15Ax_Set_comb2d_only(int value);
int CVD_H15Ax_Set_Noise_Threshold(int value);
int CVD_H15Ax_Set_AGC_Peak_Nominal(UINT8	value);	// 7bit value
int CVD_H15Ax_Get_AGC_Peak_Nominal(void);
int CVD_H15Ax_Get_PE0_Motion_Value(int *p_tnr_x_avg_t, int *p_tnr_x_avg_s, int *p_tpd_s_status);
int CVD_H15Ax_Set_Contrast_Brightness(int contrast, int brightness);
int CVD_H15Ax_Set_Dcrestore_Gain(int value);
//set reg_agc_bypass & reg_dcrestore_gain to default value
void CVD_H15Ax_Set_for_Normal_Signal(void);
//set AGC to Bypass, and set DCrestore gain to 1/8 on weak RF signal ( status nois is max value 0x3FF);
void CVD_H15Ax_Set_for_Noisy_Signal(void);
int CVD_H15Ax_Get_Status_AGC_Gain(void);
int CVD_H15Ax_Set_AGC_Peak_En(int enable);
int CVD_H15Ax_Get_HNon_Standard_Flag(void);
int CVD_H15Ax_Get_VNon_Standard_Flag(void);
int CVD_H15Ax_Set_Noburst_Ckill(unsigned int value);
int CVD_H15Ax_Get_Global_Motion_Value(void);
/* This Function is workaround code for JOJO Gunpo stream (vsync unstable);*/
int CVD_H15Ax_Set_for_Stable_Vsync(UINT32	Enable);
/* This Function is used for workaround code as to fix dong-go-dong-rak */
int CVD_H15Ax_Set_for_Field_Detect_Mode(UINT32 mode);
/* This function is used for workaround code for brasil color instable issue */
int CVD_H15Ax_Set_for_Burst_Gate_End_On_Noisy(UINT8 original_value, UINT32 Enable);
/* This function is used for workaround code for brasil color instable issue */
UINT32 CVD_H15Ax_Differential_Status_Cdto_Inc_Value(void);
UINT32 CVD_H15Ax_Read_Cordic_Freq_Value(void);
int	CVD_H15Ax_Read_VCR_Detected(void);
int CVD_H15Ax_Get_CVD_Burst_Mag_Value(void);
int CVD_H15Ax_Set_CVD_Saturation_Value(int value);
int CVD_H15Ax_Get_CAGC_Value(void);
int CVD_H15Ax_Get_Saturation_Value(void);
int CVD_H15Ax_Get_AGC_Peak_En_Value(void);
int CVD_H15Ax_Get_AGC_Bypass_Value(void);
int CVD_H15Ax_Get_Vdetect_Vcount_Value(void);
int CVD_H15Ax_Get_oadj_c_coeff_value(void);
int CVD_H15Ax_OnOff_Chromalock_Ckill(BOOLEAN bonoff);
int CVD_H15Ax_Reset_mif(int enable);
int CVD_H15Ax_Set_HNon_Standard_Threshold(int value);
int CVD_H15Ax_Force_Color_Kill(UINT8 color_kill_enable);
int CVD_H15Ax_Get_Crunky_Status(LX_AFE_CVD_CK_T *pCK_Detection_t);
int CVD_H15Ax_Fast_Vline_625_Using_Vdetec_Vcount(void);
int CVD_H15Ax_Burst_Gate_Control(UINT32	gate_start, UINT32 gate_end);
int CVD_H15Ax_Read_Buffer_Status(UINT32 *rbuf1_empty, UINT32 *rbuf2_empty, UINT32 *rbuf3_empty, UINT32 *rbuf4_empty, UINT32 *wbuf_empty, UINT32 *wbuf_ful);
void CVD_H15Ax_Program_Color_System_buffer_only(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
int CVD_H15Ax_vf_nstd_control(unsigned int value);
int CVD_H15Ax_Set_CAGC_Value(UINT32 cagc_value);
int CVD_H15Ax_Enable_Stable_Sync_Mode(int enable);
int CVD_H15Ax_Get_ColorBar_Level(UINT8 *Composite_Level);
int CVD_H15Ax_chromalock_level_control(unsigned int value);
int CVD_H15Ax_Set_SCART_Filter(void);
int CVD_H15Ax_agc_half_en_control(int enable);//gogosing
int CVD_H15Ax_Hstart_Position_Compensator(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
int CVD_H15Ax_Set_Motion_Expand(int enable);
int CVD_H15Ax_Set_CCR(int ccr_value);
int CVD_H15Ax_Check_CVD_Memory(void);
int CVD_H15Ax_Set_PALm_Stable_HSync_Mode(int enable);
int CVD_H15Ax_Set_PALm_Stable_VSync_Mode(int enable);
int CVD_H15Ax_Set_comb2d_only_md(int value);
int CVD_H15Ax_Get_comb2d_only(void);
int CVD_H15Ax_Get_comb2d_only_md(void);
int CVD_H15Ax_3DComb_Workaround(int step);
void CVD_H15Ax_Program_Color_System_Size(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
void CVD_H15Ax_Program_Color_System_Size_md(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t);
int CVD_H15Ax_Set_Vsync_Cntl(CVD_SET_SYSTEM_3CS_H15A0_T *pSet_system_t, int enable);
int CVD_H15Ax_CVD_Soft_Reset(void);
int CVD_H15Ax_Set_Blue_Mode(int blue);
int CVD_H15Ax_Set_OADJ(CVD_REG_PARAM_T	*pCVD_Black_Level_Param_t, UINT32 size);
int CVD_H15Ax_Init_Level_Detector(CVD_STATE_T color_system);
int CVD_H15Ax_Get_AFE_CVBS_status(UINT8 *cvbs_icon_value, UINT8 *cvbs_pdb_value, UINT8 *cvbs_cp_value, UINT8 *cvbs_insel_value);
int CVD_H15Ax_Get_AFE_VBUF_status(UINT8 *buf_ycm_value, UINT8 *buf_sel1_value, UINT8 *buf_pdb1_value, UINT8 *vdac_pdb_value);
int CVD_H15Ax_Set_Vactive_Params(int vline_625_stable);
int CVD_H15Ax_Set_Nosignal_Vactive_Params(void);
int CVD_H15Ax_VDAC_mute_control(int enable);
int CVD_H15Ax_Set_Internal_Demod_Mode(BOOLEAN bInternal_Demod);
int CVD_H15Ax_Set_Cpump_Auto_Stip_Mode(int mode);
int CVD_H15Ax_OnOff_DMD_DAC(BOOLEAN bonoff);
int CVD_H15Ax_Set_Blend_Ratio(int value);

int CVD_H15Ax_Set_PE_Param_Flag(int signal_lock, int color_system, int rf_av_mode, int write_done);
int CVD_H15Ax_Set_Hsync_Enhance(int enable);
int CVD_H15Ax_Set_Vsync_Enhance(int system);
int CVD_H15Ax_Get_Hstate_Max(void);
int CVD_H15Ax_Set_ycsep_Blend(int blend);
int CVD_H15Ax_Set_clampagc_updn(int updn_value);
int CVD_H15Ax_Get_status_clamp_updn(void);
int CVD_H15Ax_Set_dc_clamp_mode(int mode);
int CVD_H15Ax_Set_Cpump_Auto_Stip_Value(int value);
int CVD_H15Ax_Set_Cpump_Auto_Stip_Noisy_Value(int value);
int CVD_H15Ax_Set_ColorBar_Params(int enable);
int CVD_H15Ax_Set_Hue_Value(int value);
int CVD_H15Ax_Get_Hue_Value(void);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

