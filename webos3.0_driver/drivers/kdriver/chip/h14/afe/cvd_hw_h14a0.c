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
 *  @author		won.hur
 *  @version	1.0
 *  @date		2011-08-19
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/uaccess.h>
#include <linux/irq.h>
#include "afe_drv.h"
#include "linux/delay.h"

#include "sys_regs.h"
#include "os_util.h"
//#include "de_pe0_reg_l9.h"
#include "pe_reg_h14.h"

#include "cvd_module.h"
#include "cvd_hw_h14a0.h"
#include "de_cvd_reg_h14ax.h"
/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/
#define STANDARD_CDTO_INC_VALUE  0x21F07C1F

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
//#define VPORT_REG_DIRECT_ACCESS 0
#define	HSTART_SHIFT_DUE_TO_DE_CROP_WORKAROUND	1

// CVD Memory Map ( Base Addr:0x7bb00000, End Addr:0x7befffff, Addr Size:0x00400000)
#define CVD_BASE_ADDR	0x7BB00000

//#define L9_SLOW_AGC_WORKAROUND
//#define L9_USE_SYNCTIP_ONLY_MODE
#define USE_NEW_GATE_VALUES

#define H14_CVD_INIT_OADJ_C 	// 2012.02.24 won.hur

//#define CVD_REG_SATURATION_ADJUST
#define CVD_REG_OADJ_C_COEFF_ADJUST
#define H14_IRIS_CLAMPAGC_V2_CONTROL_WA_FOR_CVD_UNLOCK
/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/
// shadow register
//extern volatile VPORT_L9B0_REG_T __iomem *gpRegVPORT_L9B0;
// real
//extern volatile VPORT_L9B0_REG_T __iomem *gpRealRegVPORT_L9B0;
// H13B0 CVD Register Access
extern volatile DE_CVD_REG_H14Ax_T __iomem *gpRegCVD_H14Ax;
extern volatile DE_CVD_REG_H14Ax_T __iomem *gpRealRegCVD_H14Ax; 

// shadow register
//extern DE_P0L_REG_L9B_T gDE_P0L_L9B;


//extern CVD_STATE_T	Current_State;
//extern CVD_STATE_T	Next_State;

extern int gEnableScartFB;
//extern BOOLEAN g_CVD_RF_Input_Mode;
extern CVD_ADAPTIVE_PEAK_NOMINAL_CONTROL_T g_CVD_AGC_Peak_Nominal_Control ;
extern CVD_PATTERN_DETECTION_T g_CVD_Pattern_Detection_t ;
//extern LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	g_CVD_Color_System_Support ;
extern CVD_SET_SYSTEM_3CS_H14A0_T g_SetColorSystem_3CS_H14A0[];
extern CVD_STATUS_3CS_T	g_CVD_Status_3CS ;
extern OS_SEM_T	g_CVD_Sema;
/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

#ifdef H14_CVD_INIT_OADJ_C
static UINT32 g_initial_c_coeff = 0x1050;
#else
static UINT32 g_initial_c_coeff = 0;
#endif


/*========================================================================================
  Implementation Group
  ========================================================================================*/
int CVD_H14Ax_ClampAGC_OnOff(UINT8 on_off)
{
	//JUHEE : no more supported in L9
#if 0
	if(on_off)
	{
		CVD_H14Ax_RdFL(iris_099);
		CVD_H14Ax_Wr01(iris_099, cvd_cvd1_clampagc_on, 0x1);
		CVD_H14Ax_WrFL(iris_099);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_099);
		CVD_H14Ax_Wr01(iris_099, cvd_cvd1_clampagc_on, 0x0);
		CVD_H14Ax_WrFL(iris_099);
	}
#endif
	return 0;
}

int CVD_H14Ax_Clamp_Current_Control(UINT8 value)
{
	if(value <= 0x3)
	{
		ACE_REG_H14A0_RdFL(afe_cvbs_2);
		ACE_REG_H14A0_Wr01(afe_cvbs_2, cvbs_icon, value);
		ACE_REG_H14A0_WrFL(afe_cvbs_2);
		return 0;
	}
	return -1;
}

int CVD_H14Ax_Force_Color_Kill(UINT8 color_kill_enable)
{
#if 0
	if(color_kill_enable)
	{
		CVD_H14Ax_RdFL(iris_096);
		CVD_H14Ax_Wr01(iris_096, reg_ckill, 0x0);
		CVD_H14Ax_WrFL(iris_096);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_096);
		CVD_H14Ax_Wr01(iris_096, reg_ckill, 0x1c00);
		CVD_H14Ax_WrFL(iris_096);
	}
#endif
	if(color_kill_enable)
	{
		CVD_H14Ax_RdFL(iris_023);
		CVD_H14Ax_Wr01(iris_023, reg_user_ckill_mode, 0x1);
		CVD_H14Ax_WrFL(iris_023);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_023);
		CVD_H14Ax_Wr01(iris_023, reg_user_ckill_mode, 0x0);
		CVD_H14Ax_WrFL(iris_023);
	}
	return 0;
}


void CVD_H14Ax_Program_Color_System_buffer_only(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	/*
	CVD_H14Ax_RdFL(iris_mif_gmua_001);
	CVD_H14Ax_Wr01(iris_mif_gmua_001, reg_comb3_buffer_size, pSet_system_t->reg_comb3_buffer_size);
	CVD_H14Ax_WrFL(iris_mif_gmua_001);
	*/
	CVD_H14Ax_RdFL(iris_mif_gmua_002);
	CVD_H14Ax_Wr01(iris_mif_gmua_002, reg_fld1_init_rd_pel, pSet_system_t->reg_fld1_init_rd_pel);
	CVD_H14Ax_WrFL(iris_mif_gmua_002);
	CVD_H14Ax_RdFL(iris_mif_gmua_003);
	CVD_H14Ax_Wr01(iris_mif_gmua_003, reg_fld2_init_rd_pel, pSet_system_t->reg_fld2_init_rd_pel);
	CVD_H14Ax_WrFL(iris_mif_gmua_003);
	CVD_H14Ax_RdFL(iris_mif_gmua_004);
	CVD_H14Ax_Wr01(iris_mif_gmua_004, reg_fld3_init_rd_pel, pSet_system_t->reg_fld3_init_rd_pel);
	CVD_H14Ax_WrFL(iris_mif_gmua_004);
	CVD_H14Ax_RdFL(iris_mif_gmua_005);
	CVD_H14Ax_Wr01(iris_mif_gmua_005, reg_fld4_init_rd_pel, pSet_system_t->reg_fld4_init_rd_pel);
	CVD_H14Ax_WrFL(iris_mif_gmua_005);

	CVD_H14Ax_RdFL(iris_013);
	CVD_H14Ax_Wr01(iris_013, reg_colour_mode, pSet_system_t->reg_colour_mode);
	CVD_H14Ax_WrFL(iris_013);
	/*
	CVD_H14Ax_RdFL(iris_013);
	CVD_H14Ax_Wr01(iris_013, reg_hpixel, pSet_system_t->reg_hpixel);
	CVD_H14Ax_WrFL(iris_013);

	CVD_H14Ax_RdFL(iris_029);
	CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, pSet_system_t->reg_cdto_inc);
	CVD_H14Ax_WrFL(iris_029);
	*/
	return;
}
void CVD_H14Ax_Program_Color_System_Main_Multi(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
#ifndef H14_3DCOMB_ERROR_WORKAROUND
	if(pSet_system_t->reg_comb3_buffer_size > 1)
	{
		CVD_H14Ax_RdFL(iris_mif_gmua_001);
		CVD_H14Ax_Wr01(iris_mif_gmua_001, reg_comb3_buffer_size, pSet_system_t->reg_comb3_buffer_size);
		CVD_H14Ax_WrFL(iris_mif_gmua_001);
		CVD_H14Ax_RdFL(iris_mif_gmua_002);
		CVD_H14Ax_Wr01(iris_mif_gmua_002, reg_fld1_init_rd_pel, pSet_system_t->reg_fld1_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_002);
		CVD_H14Ax_RdFL(iris_mif_gmua_003);
		CVD_H14Ax_Wr01(iris_mif_gmua_003, reg_fld2_init_rd_pel, pSet_system_t->reg_fld2_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_003);
		CVD_H14Ax_RdFL(iris_mif_gmua_004);
		CVD_H14Ax_Wr01(iris_mif_gmua_004, reg_fld3_init_rd_pel, pSet_system_t->reg_fld3_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_004);
		CVD_H14Ax_RdFL(iris_mif_gmua_005);
		CVD_H14Ax_Wr01(iris_mif_gmua_005, reg_fld4_init_rd_pel, pSet_system_t->reg_fld4_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_005);
	}
#endif


	// by Kim.min 2010/06/03
	CVD_H14Ax_RdFL(iris_013);
	CVD_H14Ax_Wr03(iris_013, reg_colour_mode, pSet_system_t->reg_colour_mode, reg_vline_625, pSet_system_t->reg_vline_625, reg_hpixel, pSet_system_t->reg_hpixel);
	CVD_H14Ax_WrFL(iris_013);
	CVD_H14Ax_RdFL(iris_014);
	CVD_H14Ax_Wr01(iris_014, reg_ped, pSet_system_t->reg_ped);
	CVD_H14Ax_WrFL(iris_014);
	//added 110411 by kim.min
	CVD_H14Ax_RdFL(iris_131);
	CVD_H14Ax_Wr01(iris_131, reg_adc_blank_level, pSet_system_t->reg_adc_blank_level);
	CVD_H14Ax_WrFL(iris_131);
	CVD_H14Ax_RdFL(iris_015);
	//120503 moved reg_hagc_en to Reg_Init Function
	CVD_H14Ax_Wr01(iris_015, reg_cagc_en, pSet_system_t->reg_cagc_en );
	CVD_H14Ax_WrFL(iris_015);

	CVD_H14Ax_RdFL(iris_017);
	CVD_H14Ax_Wr01(iris_017, reg_hagc, pSet_system_t->reg_hagc);
	CVD_H14Ax_WrFL(iris_017);
	//yc_delay setting is moved to CVD PQ
	//gVportRegBx->iris_019.reg_yc_delay = pSet_system_t->reg_yc_delay;
	//CVD_H14Ax_RdFL(iris_019);
	//CVD_H14Ax_Wr01(iris_019, reg_yc_delay, pSet_system_t->reg_yc_delay);
	//CVD_H14Ax_WrFL(iris_019);
	// need Reg debug
	//
	CVD_H14Ax_RdFL(iris_096);
#ifdef USE_NEW_GATE_VALUES
	CVD_H14Ax_Wr02(iris_096, reg_cagc_gate_start, pSet_system_t->reg_cagc_gate_start_new, reg_cagc_gate_end, pSet_system_t->reg_cagc_gate_end_new);
#else
	CVD_H14Ax_Wr02(iris_096, reg_cagc_gate_start, pSet_system_t->reg_cagc_gate_start, reg_cagc_gate_end, pSet_system_t->reg_cagc_gate_end);
#endif
	CVD_H14Ax_WrFL(iris_096);
	CVD_H14Ax_RdFL(iris_029);
	CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, pSet_system_t->reg_cdto_inc);
	CVD_H14Ax_WrFL(iris_029);
	CVD_H14Ax_RdFL(iris_030);
	CVD_H14Ax_Wr01(iris_030, reg_hdto_inc, pSet_system_t->reg_hdto_inc);
	CVD_H14Ax_WrFL(iris_030);


		CVD_H14Ax_RdFL(iris_036);
		CVD_H14Ax_Wr01(iris_036, reg_hactive_start, pSet_system_t->reg_hactive_start_54M);
		CVD_H14Ax_Wr01(iris_036, reg_hactive_width, pSet_system_t->reg_hactive_width_54M);
		CVD_H14Ax_WrFL(iris_036);

	CVD_H14Ax_RdFL(iris_037);
	CVD_H14Ax_Wr02(iris_037, reg_vactive_start, pSet_system_t->reg_vactive_start, reg_vactive_height, pSet_system_t->reg_vactive_height);
//	CVD_H14Ax_Wr02(iris_036, reg_vactive_start, pSet_system_t->reg_vactive_start_L9B0, reg_vactive_height, pSet_system_t->reg_vactive_height);
	CVD_H14Ax_WrFL(iris_037);
	CVD_H14Ax_RdFL(iris_016);
	CVD_H14Ax_Wr01(iris_016, reg_ntsc443_mode, pSet_system_t->reg_ntsc443_mode);
	CVD_H14Ax_WrFL(iris_016);
	CVD_H14Ax_RdFL(iris_035);
	//CVD_H14Ax_Wr01(iris_034, reg_burst_gate_end, pSet_system_t->reg_burst_gate_end);
#ifdef USE_NEW_GATE_VALUES
	CVD_H14Ax_Wr02(iris_035, reg_burst_gate_start, pSet_system_t->reg_burst_gate_start_new, reg_burst_gate_end, pSet_system_t->reg_burst_gate_end_new);
#else
	CVD_H14Ax_Wr02(iris_035, reg_burst_gate_start, pSet_system_t->reg_burst_gate_start_3CS, reg_burst_gate_end, pSet_system_t->reg_burst_gate_end_3CS);
#endif
	CVD_H14Ax_WrFL(iris_035);

	// 110828: L9Bx moved to PE
	/*
	CVD_H14Ax_RdFL(iris_044);
	CVD_H14Ax_Wr01(iris_044, reg_secam_ybw, pSet_system_t->reg_secam_ybw);
	CVD_H14Ax_WrFL(iris_044);
	CVD_H14Ax_RdFL(iris_045);
	CVD_H14Ax_Wr01(iris_045, reg_auto_secam_level, pSet_system_t->reg_auto_secam_level);
	CVD_H14Ax_WrFL(iris_045);

	CVD_H14Ax_RdFL(iris_046);
	CVD_H14Ax_Wr01(iris_046, reg_lose_chromalock_mode, pSet_system_t->reg_lose_chromalock_mode);
	CVD_H14Ax_WrFL(iris_046);
	CVD_H14Ax_RdFL(iris_060);
	CVD_H14Ax_Wr06(iris_060, reg_noise_th, pSet_system_t->reg_noise_th, reg_noise_th_en, pSet_system_t->reg_noise_th_en, reg_lowfreq_vdiff_gain, pSet_system_t->reg_lowfreq_vdiff_gain, reg_chroma_vdiff_gain, pSet_system_t->reg_chroma_vdiff_gain, reg_horiz_diff_ygain, pSet_system_t->reg_horiz_diff_ygain, reg_horiz_diff_cgain, pSet_system_t->reg_horiz_diff_cgain);
	CVD_H14Ax_WrFL(iris_060);
	CVD_H14Ax_RdFL(iris_061);
	CVD_H14Ax_Wr04(iris_061, reg_y_noise_th_gain, pSet_system_t->reg_y_noise_th_gain, reg_c_noise_th_gain, pSet_system_t->reg_c_noise_th_gain, reg_burst_noise_th_gain, pSet_system_t->reg_burst_noise_th_gain, reg_vadap_burst_noise_th_gain, pSet_system_t->reg_vadap_burst_noise_th_gain);
	CVD_H14Ax_WrFL(iris_061);
	CVD_H14Ax_RdFL(iris_062);
	// 110623 : by kim.min adaptive_chroma_mode will be changed according to the input condition(RF or AV).
	//CVD_H14Ax_Wr02(iris_062, reg_motion_mode, pSet_system_t->reg_motion_mode, reg_adaptive_chroma_mode, pSet_system_t->reg_adaptive_chroma_mode);
	CVD_H14Ax_Wr01(iris_062, reg_motion_mode, pSet_system_t->reg_motion_mode);
	CVD_H14Ax_WrFL(iris_062);
	*/
#ifndef H14_COMB2D_ONLY_CONTROL
	CVD_H14Ax_RdFL(iris_064);
	CVD_H14Ax_Wr02(iris_064, reg_comb2d_only, pSet_system_t->reg_comb2d_only, reg_fb_sync, pSet_system_t->reg_fb_sync);
	//CVD_H14Ax_Wr01(iris_064, reg_fb_sync, pSet_system_t->reg_fb_sync);
	CVD_H14Ax_WrFL(iris_064);

	//121025 : added for H13B0 revision support
	CVD_H14Ax_RdFL(iris_066);
	CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, pSet_system_t->reg_comb2d_only);
	CVD_H14Ax_WrFL(iris_066);
#endif
	// 110828: L9Bx moved to PE 'reg_md_noise_th'

	CVD_H14Ax_RdFL(iris_069);
	CVD_H14Ax_Wr02(iris_069, reg_vactive_md_start, pSet_system_t->reg_vactive_md_start, reg_vactive_md_height, pSet_system_t->reg_vactive_md_height);
	CVD_H14Ax_WrFL(iris_069);

	CVD_H14Ax_RdFL(iris_090);
	// 110828: L9Bx "reg_motion_config" moved to PE
	CVD_H14Ax_Wr02(iris_090, reg_hactive_md_start, pSet_system_t->reg_hactive_md_start, reg_hactive_md_width, pSet_system_t->reg_hactive_md_width);
	CVD_H14Ax_WrFL(iris_090);
	// 110828: L9Bx "reg_status_motion_mode" moved to PE

	// kim.min 0622
	CVD_H14Ax_RdFL(iris_094);
#ifdef USE_NEW_GATE_VALUES
	CVD_H14Ax_Wr03(iris_094, reg_cordic_gate_end, pSet_system_t->reg_cordic_gate_end_new, reg_cordic_gate_start, pSet_system_t->reg_cordic_gate_start_new, reg_phase_offset_range, pSet_system_t->reg_phase_offset_range);
#else
	CVD_H14Ax_Wr03(iris_094, reg_cordic_gate_end, pSet_system_t->reg_cordic_gate_end_3CS, reg_cordic_gate_start, pSet_system_t->reg_cordic_gate_start_3CS, reg_phase_offset_range, pSet_system_t->reg_phase_offset_range);
#endif
	CVD_H14Ax_WrFL(iris_094);

	// kim.min 0716
	//	_iow(&gVportRegBx->iris_086, 8, 0, pSet_system_t->reg_tcomb_chroma_level);
	//	_iow(&gVportRegBx->iris_086, 8, 8, pSet_system_t->reg_hf_luma_chroma_offset);
	//	_iow(&gVportRegBx->iris_086, 8, 24, pSet_system_t->reg_chroma_level);
	//kim.min 1103
	// setting of below register is moved to _CVD_L9Ax_Set_Output_Range()
	//	gVportRegBx->iris_118.reg_oadj_y_offo = pSet_system_t->reg_oadj_y_offo;
	//kim.min 0906
	// setting of below register is moved to _CVD_L9Ax_Set_Output_Range()
	//	gVportRegBx->iris_119.reg_oadj_y_coeff = pSet_system_t->reg_oadj_y_coeff;
	CVD_H14Ax_RdFL(iris_016);
	CVD_H14Ax_Wr01(iris_016, reg_pal60_mode, pSet_system_t->reg_pal60_mode);
	CVD_H14Ax_WrFL(iris_016);
	//kim.min 0920
	CVD_H14Ax_RdFL(iris_027);
	CVD_H14Ax_Wr01(iris_027, reg_hstate_max, pSet_system_t->reg_hstate_max);
	CVD_H14Ax_WrFL(iris_027);

	//by dws : remove mdelay
	//mdelay(10); //0619
//	OS_MsecSleep(5);

	//No use
//	CVD_H14Ax_RdFL(top_005);
//	CVD_H14Ax_Wr01(top_005, swrst_irisyc, 0);
//	CVD_H14Ax_WrFL(top_005);
	//added 0212 for better 3dcomb operation on RF signal.
#ifndef H14_FAST_3DCOMB_WORKAROUND
#ifndef	H14_3DCOMB_ERROR_WORKAROUND
	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Wr01(iris_024, reg_hnon_std_threshold, pSet_system_t->reg_hnon_std_threshold);
	CVD_H14Ax_WrFL(iris_024);
#else
	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Wr01(iris_024, reg_hnon_std_threshold, 0x0);
	CVD_H14Ax_WrFL(iris_024);

	CVD_H14Ax_RdFL(iris_067);
	CVD_H14Ax_Wr01(iris_067, reg_vf_nstd_en, 0x0);	// default 0x1
	CVD_H14Ax_WrFL(iris_067);
#endif
#endif

	//added 110415 for Jo Jo Gunpo filed stream : no signal issue
	CVD_H14Ax_RdFL(iris_078);
	CVD_H14Ax_Wr01(iris_078, reg_vsync_signal_thresh, pSet_system_t->reg_vsync_signal_thresh);
	CVD_H14Ax_WrFL(iris_078);

   //added 110608 ( for PAL Test(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
	/*
	CVD_H14Ax_RdFL(iris_048);
	CVD_H14Ax_Wr02(iris_048, reg_fixed_cstate, pSet_system_t->reg_fixed_cstate, cstate, pSet_system_t->reg_cstate);
	CVD_H14Ax_WrFL(iris_048);
	*/
	// For proper color system detection, at first set cstate value to default.

#if 0
   //gogosing added 110610 (for russia ATV field stream color system issue)	// 이곳에 두면 재현 잘 됨
   		CVD_H14Ax_RdFL(iris_182);
		CVD_H14Ax_Wr01(iris_182, reg_cs_chroma_burst5or10, pSet_system_t->cs_chroma_burst5or10);
		CVD_H14Ax_WrFL(iris_182);

		CVD_H14Ax_RdFL(iris_193);
		CVD_H14Ax_Wr01(iris_193, reg_cs1_chroma_burst5or10, pSet_system_t->cs1_chroma_burst5or10);
		CVD_H14Ax_WrFL(iris_193);
#endif

   //kim.min 0906
	CVD_H14Ax_RdFL(iris_263);
	CVD_H14Ax_Wr02(iris_263, reg_hrs_ha_start, pSet_system_t->reg_hrs_ha_start, reg_hrs_ha_width, pSet_system_t->reg_hrs_ha_width);
	CVD_H14Ax_WrFL(iris_263);

	//110901 : Start RGB Initail settings.
	//110919 : Modified to Set H/V offset of SCART RGB for each color system.
	CVD_H14Ax_RdFL(fastblank_009);
	CVD_H14Ax_RdFL(fastblank_010);
	CVD_H14Ax_Wr01(fastblank_009, reg_fb_vstart_odd, pSet_system_t->reg_fb_vstart_odd);
	CVD_H14Ax_Wr01(fastblank_010, reg_fb_vstart_even, pSet_system_t->reg_fb_vstart_even);
	CVD_H14Ax_Wr01(fastblank_010, reg_fb_height_half, pSet_system_t->reg_fb_height_half);
	CVD_H14Ax_Wr01(fastblank_010, reg_fb_hstart, pSet_system_t->reg_fb_hstart);
	CVD_H14Ax_WrFL(fastblank_009);
	CVD_H14Ax_WrFL(fastblank_010);

	//111129 wonsik.do
	CVD_H14Ax_RdFL(iris_074);
	CVD_H14Ax_Wr01(iris_074, reg_dcrestore_accum_width, pSet_system_t->reg_dcrestore_accum_width);
	CVD_H14Ax_WrFL(iris_074);

	CVD_H14Ax_RdFL(iris_076);
	CVD_H14Ax_Wr01(iris_076, reg_dcrestore_hsync_mid, pSet_system_t->reg_dcrestore_hsync_mid);
	CVD_H14Ax_WrFL(iris_076);

	if(g_CVD_Status_3CS.in_rf_mode == TRUE) {
		//111215 : added reg_contrast
		CVD_H14Ax_RdFL(iris_021);
		CVD_H14Ax_Wr01(iris_021, reg_contrast, pSet_system_t->reg_contrast);
		CVD_H14Ax_WrFL(iris_021);

		CVD_H14Ax_RdFL(iris_022);
		CVD_H14Ax_Wr01(iris_022, reg_cagc, pSet_system_t->reg_cagc);
		CVD_H14Ax_WrFL(iris_022);
	}
	else {	// AV mode
		//111215 : added reg_contrast
		CVD_H14Ax_RdFL(iris_021);
		CVD_H14Ax_Wr01(iris_021, reg_contrast, pSet_system_t->reg_contrast_av);
		CVD_H14Ax_WrFL(iris_021);

		CVD_H14Ax_RdFL(iris_022);
		CVD_H14Ax_Wr01(iris_022, reg_cagc, pSet_system_t->reg_cagc_av);
		CVD_H14Ax_WrFL(iris_022);
	}

   //120105 : added reg_saturation
	CVD_H14Ax_RdFL(iris_021);
	CVD_H14Ax_Wr01(iris_021, reg_saturation, pSet_system_t->reg_saturation);
	CVD_H14Ax_WrFL(iris_021);

	//121101 : added for H13B0
#ifndef H14_3DCOMB_ERROR_WORKAROUND
	if(pSet_system_t->reg_comb3_buffer_size > 1)
	{
		CVD_H14Ax_RdFL(iris_mif_gmua_md_001);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_001, reg_comb3_buffer_size_md, pSet_system_t->reg_comb3_buffer_size_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_001);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_002);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_002, reg_fld1_init_rd_pel_md, pSet_system_t->reg_fld1_init_rd_pel_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_002);
	}
#endif
}

void CVD_H14Ax_Program_Color_System_CS(CVD_SELECT_CDETECT_T cs_sel, CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{

	if (cs_sel == CVD_SEL_CS_CS0) // select cs0;
	{
		CVD_H14Ax_RdFL(iris_180);
		CVD_H14Ax_Wr01(iris_180, reg_cs_adaptive_chroma_mode, pSet_system_t->reg_adaptive_chroma_mode);
		CVD_H14Ax_Wr01(iris_180, reg_cs_auto_secam_level, pSet_system_t->reg_auto_secam_level);
		CVD_H14Ax_WrFL(iris_180);

		CVD_H14Ax_RdFL(iris_181);
		//gogoging SECAM threshold for keeping PAL stable 20110613
		CVD_H14Ax_Wr01(iris_181, reg_cs_issecam_th, pSet_system_t->cs_issecam_th);
		CVD_H14Ax_Wr01(iris_181, reg_cs_phase_offset_range, pSet_system_t->reg_phase_offset_range);
		CVD_H14Ax_WrFL(iris_181);

		CVD_H14Ax_RdFL(iris_183);
		CVD_H14Ax_Wr01(iris_183, reg_cs_secam_ybw, pSet_system_t->reg_secam_ybw);
		CVD_H14Ax_Wr01(iris_183, reg_cs_adaptive_mode, pSet_system_t->reg_adaptive_mode);
		CVD_H14Ax_Wr01(iris_183, reg_cs_colour_mode, pSet_system_t->reg_colour_mode);
		CVD_H14Ax_Wr01(iris_183, reg_cs_ntsc443_mode, pSet_system_t->reg_ntsc443_mode);
		CVD_H14Ax_Wr01(iris_183, reg_cs_pal60_mode, pSet_system_t->reg_pal60_mode);
		//gogosing PAL에서 0x5c  설정 시 FSC position  테스트 시 secam, pal transition 지속됨 --> 326e 원복 110627
		//cs0, cs1 burst gate width reg. ready
		CVD_H14Ax_Wr01(iris_183, reg_cs_chroma_burst5or10, pSet_system_t->cs_chroma_burst5or10);
		CVD_H14Ax_Wr01(iris_183, reg_cs_cagc_en, pSet_system_t->reg_cagc_en);
		CVD_H14Ax_WrFL(iris_183);

		CVD_H14Ax_RdFL(iris_184);
		CVD_H14Ax_Wr01(iris_184, reg_cs_cagc, pSet_system_t->reg_cagc);
		CVD_H14Ax_WrFL(iris_184);

		CVD_H14Ax_RdFL(iris_185);
		CVD_H14Ax_Wr01(iris_185, reg_cs_chroma_bw_lo, pSet_system_t->reg_chroma_bw_lo);
		CVD_H14Ax_WrFL(iris_185);

		CVD_H14Ax_RdFL(iris_186);
		CVD_H14Ax_Wr01(iris_186, reg_cs_cdto_inc, pSet_system_t->reg_cdto_inc);
		CVD_H14Ax_WrFL(iris_186);

		CVD_H14Ax_RdFL(iris_187);
		CVD_H14Ax_Wr01(iris_187, reg_cs_lose_chromalock_mode, pSet_system_t->reg_lose_chromalock_mode);
		CVD_H14Ax_WrFL(iris_187);

		//added 110608 ( for PAL Test(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
		/*
		CVD_H14Ax_RdFL(iris_186);
		CVD_H14Ax_Wr02(iris_186, reg_cs_fixed_cstate, pSet_system_t->reg_fixed_cstate, reg_cs_cstate, pSet_system_t->reg_cstate);
		CVD_H14Ax_WrFL(iris_186);
		*/

		//added 110908
		CVD_H14Ax_RdFL(iris_272);
#ifdef USE_NEW_GATE_VALUES
		CVD_H14Ax_Wr02(iris_272, reg_burst1_gate_start, pSet_system_t->reg_burst_gate_start_new, reg_burst1_gate_end, pSet_system_t->reg_burst_gate_end_new);
#else
		CVD_H14Ax_Wr02(iris_272, reg_burst1_gate_start, pSet_system_t->reg_burst_gate_start_3CS, reg_burst1_gate_end, pSet_system_t->reg_burst_gate_end_3CS);
#endif
		CVD_H14Ax_WrFL(iris_272);

		CVD_H14Ax_RdFL(iris_273);
#ifdef USE_NEW_GATE_VALUES
		CVD_H14Ax_Wr02(iris_273, reg_cordic1_gate_start, pSet_system_t->reg_cordic_gate_start_new, reg_cordic1_gate_end, pSet_system_t->reg_cordic_gate_end_new);
#else
		CVD_H14Ax_Wr02(iris_273, reg_cordic1_gate_start, pSet_system_t->reg_cordic_gate_start_3CS, reg_cordic1_gate_end, pSet_system_t->reg_cordic_gate_end_3CS);
#endif
		CVD_H14Ax_WrFL(iris_273);
	}
	else	// cs1 selected
	{
		CVD_H14Ax_RdFL(iris_191);
		CVD_H14Ax_Wr01(iris_191, reg_cs1_adaptive_chroma_mode, pSet_system_t->reg_adaptive_chroma_mode);
		CVD_H14Ax_Wr01(iris_191, reg_cs1_auto_secam_level, pSet_system_t->reg_auto_secam_level);
		CVD_H14Ax_WrFL(iris_191);

		CVD_H14Ax_RdFL(iris_192);
		CVD_H14Ax_Wr01(iris_192, reg_cs1_phase_offset_range, pSet_system_t->reg_phase_offset_range);
		CVD_H14Ax_WrFL(iris_192);

		CVD_H14Ax_RdFL(iris_194);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_secam_ybw, pSet_system_t->reg_secam_ybw);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_adaptive_mode, pSet_system_t->reg_adaptive_mode);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_colour_mode, pSet_system_t->reg_colour_mode);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_ntsc443_mode, pSet_system_t->reg_ntsc443_mode);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_pal60_mode, pSet_system_t->reg_pal60_mode);
		//gogosing PAL에서 0x5c  설정 시 FSC position  테스트 시 secam, pal transition 지속됨 --> 326e 원복 110627
		//cs0, cs1 burst gate width reg. ready
		CVD_H14Ax_Wr01(iris_194, reg_cs1_chroma_burst5or10, pSet_system_t->cs1_chroma_burst5or10);
		CVD_H14Ax_Wr01(iris_194, reg_cs1_cagc_en, pSet_system_t->reg_cagc_en);
		CVD_H14Ax_WrFL(iris_194);

		CVD_H14Ax_RdFL(iris_195);
		CVD_H14Ax_Wr01(iris_195, reg_cs1_cagc, pSet_system_t->reg_cagc);
		CVD_H14Ax_WrFL(iris_195);

		CVD_H14Ax_RdFL(iris_196);
		CVD_H14Ax_Wr01(iris_196, reg_cs1_chroma_bw_lo, pSet_system_t->reg_chroma_bw_lo);
		CVD_H14Ax_WrFL(iris_196);

		CVD_H14Ax_RdFL(iris_197);
		CVD_H14Ax_Wr01(iris_197, reg_cs1_cdto_inc, pSet_system_t->reg_cdto_inc);
		CVD_H14Ax_WrFL(iris_197);

		CVD_H14Ax_RdFL(iris_198);
		CVD_H14Ax_Wr01(iris_198, reg_cs1_lose_chromalock_mode, pSet_system_t->reg_lose_chromalock_mode);
		CVD_H14Ax_WrFL(iris_198);

		//added 110608 ( for PAL Test(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
		/*
		CVD_H14Ax_RdFL(iris_197);
		CVD_H14Ax_Wr02(iris_197, reg_cs1_fixed_cstate, pSet_system_t->reg_fixed_cstate, reg_cs1_cstate, pSet_system_t->reg_cstate);
		CVD_H14Ax_WrFL(iris_197);
		*/

		//added 110908
		CVD_H14Ax_RdFL(iris_272);
#ifdef USE_NEW_GATE_VALUES
		CVD_H14Ax_Wr02(iris_272, reg_burst2_gate_start, pSet_system_t->reg_burst_gate_start_new, reg_burst2_gate_end, pSet_system_t->reg_burst_gate_end_new);
#else
		CVD_H14Ax_Wr02(iris_272, reg_burst2_gate_start, pSet_system_t->reg_burst_gate_start_3CS, reg_burst2_gate_end, pSet_system_t->reg_burst_gate_end_3CS);
#endif
		CVD_H14Ax_WrFL(iris_272);

		CVD_H14Ax_RdFL(iris_273);
#ifdef USE_NEW_GATE_VALUES
		CVD_H14Ax_Wr02(iris_273, reg_cordic2_gate_start, pSet_system_t->reg_cordic_gate_start_new, reg_cordic2_gate_end, pSet_system_t->reg_cordic_gate_end_new);
#else
		CVD_H14Ax_Wr02(iris_273, reg_cordic2_gate_start, pSet_system_t->reg_cordic_gate_start_3CS, reg_cordic2_gate_end, pSet_system_t->reg_cordic_gate_end_3CS);
#endif
		CVD_H14Ax_WrFL(iris_273);
	}

	return;
}

void CVD_H14Ax_Program_Color_System_PreJob(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	CVD_DEBUG("%s entered [%d][%d]\n",__func__, pSet_system_t->reg_fixed_cstate, pSet_system_t->reg_cstate);
	// For proper color system detection, at first set cstate value to default.
	CVD_H14Ax_RdFL(iris_049);
	CVD_H14Ax_Wr02(iris_049, reg_fixed_cstate, pSet_system_t->reg_fixed_cstate, reg_cstate, pSet_system_t->reg_cstate);
	CVD_H14Ax_WrFL(iris_049);
}

void CVD_H14Ax_Program_Color_System_PreJob2(void)
{
	CVD_DEBUG("%s entered \n",__func__);
   //added 111226 ( default register setting for fast 3Dcomb operation )
	// restore default values of reg_agc_half_en to '0', and reg_nstd_hysis to '7' for EBS field stream support.
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 0);
	CVD_H14Ax_WrFL(iris_015);
	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 7);
	CVD_H14Ax_WrFL(iris_024);


	//added 120114 for stable agc (restore default values)
	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_cntl, 0x1);
	CVD_H14Ax_WrFL(iris_025);
	CVD_H14Ax_RdFL(iris_074);
	CVD_H14Ax_Wr01(iris_074, reg_dcrestore_gain, 0x0);
	CVD_H14Ax_WrFL(iris_074);
}

void CVD_H14Ax_Program_Color_System_PostJob(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	CVD_DEBUG("%s entered [%d][%d]\n",__func__, pSet_system_t->reg_fixed_cstate, pSet_system_t->reg_cstate);
   //added 110608 ( for PAL Test(Sub Carrier Pull in Range) reg_fixed_cstate : 1, reg_cstate : 7 )
	CVD_H14Ax_RdFL(iris_049);
	CVD_H14Ax_Wr02(iris_049, reg_fixed_cstate, pSet_system_t->reg_fixed_cstate, reg_cstate, pSet_system_t->reg_cstate);
	CVD_H14Ax_WrFL(iris_049);
}

void CVD_H14Ax_Program_Color_System_PostJob2(void)
{
//	CVD_DEBUG("%s entered \n",__func__);
   //added 111226 ( for EBS 060225_1636 stream , change reg_agc_half_en to '1', reg_nstd_hysis to '0' )
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 1);
	CVD_H14Ax_WrFL(iris_015);
	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 0);
	CVD_H14Ax_WrFL(iris_024);

	//added 120114 for stable agc
	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_cntl, 0x0);
	CVD_H14Ax_WrFL(iris_025);
	CVD_H14Ax_RdFL(iris_074);
	CVD_H14Ax_Wr01(iris_074, reg_dcrestore_gain, 0x3);
	CVD_H14Ax_WrFL(iris_074);
}

void CVD_H14Ax_SW_Reset(LX_AFE_CVD_SELECT_T select_main_sub)
{
		// SET SW reset registers to '1'
		//		gVportRegBx->reg_swrst_exta.reg_swrst_exta = 1;
		//		gVportRegBx->reg_swrst_exta.reg_swrst_extb = 1;
		//from cvd_test.cmm
		CVD_H14Ax_RdFL(iris_012);
		/*
		CVD_H14Ax_Rd01(iris_011, reg_cvd_soft_reset, temp);
		AFE_PRINT("top_001 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->top_001- (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("iris_mif_gmua_001 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->iris_mif_gmua_001- (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("iris_mif_gmua_008 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->iris_mif_gmua_008- (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("iris_de_ctrl_001 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->iris_de_ctrl_001 - (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("fast_blank_status_001 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->fast_blank_status_001 - (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("iris_001 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->iris_001- (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("iris_011 :  %x \n",(UINT32)&gpRealRegVPORT_L9B0->iris_011- (UINT32)gpRealRegVPORT_L9B0);
		AFE_PRINT("cvd_soft_reset status before set to 1 : [%x]\n", temp);
		AFE_PRINT("iris_011 value:[%x]\n", REG_RD(0xc0024230));
		REG_WD(0xc0024230		,1);
		*/
		CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 1);
		CVD_H14Ax_WrFL(iris_012);

		/* LLPLL/CST SWRST & 3CH_DIG SWRST */
		ACE_REG_H14A0_RdFL(soft_reset_0);
		ACE_REG_H14A0_RdFL(llpll_0);
		ACE_REG_H14A0_Wr01(soft_reset_0, swrst_pix, 1);
		ACE_REG_H14A0_Wr01(llpll_0, reset_n, 0);
		ACE_REG_H14A0_Wr01(llpll_0, llpll_pdb, 0);
		ACE_REG_H14A0_WrFL(soft_reset_0);
		ACE_REG_H14A0_WrFL(llpll_0);

		// dws added
		//by dws : remove mdelay
		//mdelay(10); //0809
		OS_MsecSleep(5);
		//gVportRegBx->iris_063.reg_lbadrgen_rst = 1;
		//Clear SW Reset Registers to '0'
		//		gVportRegBx->reg_swrst_exta.reg_swrst_exta = 0;
		//		gVportRegBx->reg_swrst_exta.reg_swrst_extb = 0;
		//from cvd_test.cmm
		CVD_H14Ax_RdFL(iris_012);
		CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 0);
		CVD_H14Ax_WrFL(iris_012);

		/* LLPLL/CST SWRST & 3CH_DIG SWRST */
		ACE_REG_H14A0_RdFL(soft_reset_0);
		ACE_REG_H14A0_RdFL(llpll_0);
		ACE_REG_H14A0_Wr01(soft_reset_0, swrst_pix, 0);
		ACE_REG_H14A0_Wr01(llpll_0, reset_n, 1);
		ACE_REG_H14A0_Wr01(llpll_0, llpll_pdb, 1);
		ACE_REG_H14A0_WrFL(soft_reset_0);
		ACE_REG_H14A0_WrFL(llpll_0);

		// dws added
		//gVportRegBx->iris_063.reg_lbadrgen_rst = 0;

}

// BOOLEAN PowerOnOff
// TRUE : Power Down
// FALSE : Power Up
void CVD_H14Ax_Power_Down(LX_AFE_CVD_SELECT_T select_main_sub, BOOLEAN PowerOnOFF)
{
	if(select_main_sub == LX_CVD_MAIN)
	{
		//if(PowerOnOFF==TRUE)
		if(PowerOnOFF==FALSE) // from cvd_test.cmm
		{
			// At First, dr3p_pdb should be turned off(Workaround code for denc latchup)
			// Move to I2C for support L9A Internel I2C 8MHz - 20110623 by sh.myoung -
			//ACE_REG_H14A0_RdFL(main_pll_4);
			//gmain_pll_4.dr3p_pdb = 0;
			//ACE_REG_H14A0_WrFL(main_pll_4);

			//ACE_REG_H14A0_RdFL(main_pll_4);
			//gmain_pll_4.dr3p_pdb = 1;
			//ACE_REG_H14A0_WrFL(main_pll_4);

			//	mdelay(10);

			//cvd on
			// CVD Power On (Default settings)
			ACE_REG_H14A0_RdFL(afe_cvbs_1);
			ACE_REG_H14A0_RdFL(afe_cvbs_3);
			ACE_REG_H14A0_Wr01(afe_cvbs_1, cvbs_pdbm, 0x1);
		// cvbs_pdb ON , in CVD_H14Ax_Channel_Power_Control()
		//	ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_pdb, 0x1);
			ACE_REG_H14A0_WrFL(afe_cvbs_1);
			ACE_REG_H14A0_WrFL(afe_cvbs_3);

			ACE_REG_H14A0_RdFL(afe_vbuf_0);
			ACE_REG_H14A0_RdFL(afe_vbuf_1);

			/* VDAC & BUFFER Power Down Setting */
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 1);
			// buf2 is not used.
			ACE_REG_H14A0_Wr01(afe_vbuf_0, buf_pdb2, 0);

			ACE_REG_H14A0_WrFL(afe_vbuf_0);
			ACE_REG_H14A0_WrFL(afe_vbuf_1);
		}
		else
		{
			//CVD off

			ACE_REG_H14A0_RdFL(main_pll_4);
			ACE_REG_H14A0_RdFL(afe_cvbs_1);
			ACE_REG_H14A0_RdFL(afe_cvbs_3);
			ACE_REG_H14A0_Wr01(afe_cvbs_1, cvbs_pdbm, 0);
			ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_pdb, 0);
			ACE_REG_H14A0_Wr01(main_pll_4, dr3p_pdb, 0);
			ACE_REG_H14A0_WrFL(main_pll_4);
			ACE_REG_H14A0_WrFL(afe_cvbs_1);
			ACE_REG_H14A0_WrFL(afe_cvbs_3);

			ACE_REG_H14A0_RdFL(afe_vbuf_0);
			ACE_REG_H14A0_RdFL(afe_vbuf_1);

			/* VDAC & BUFFER Power Down Setting */
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 0);
			ACE_REG_H14A0_Wr01(afe_vbuf_0, buf_pdb2, 0);

			ACE_REG_H14A0_WrFL(afe_vbuf_0);
			ACE_REG_H14A0_WrFL(afe_vbuf_1);
		}
	}
	else
	{
	}
}

void CVD_H14Ax_Reg_Init(LX_AFE_CVD_SELECT_T select_main_sub)
{
	//110915 : release f54m, cvbs reset first.
	// CVD_H13_BRINGUP : soft_reset_5 to soft_reset_2
	ACE_REG_H14A0_RdFL(soft_reset_2);

	// CVD_H13_BRINGUP :from h/w script
	ACE_REG_H14A0_Wr01(soft_reset_2, swrst_vdac, 0);

	ACE_REG_H14A0_Wr01(soft_reset_2, swrst_f54m, 0);
	ACE_REG_H14A0_Wr01(soft_reset_2, swrst_cvbs, 0);
	ACE_REG_H14A0_WrFL(soft_reset_2);

	ACE_REG_H14A0_RdFL(data_bridge_0);
	ACE_REG_H14A0_RdFL(clock_control_1);
	ACE_REG_H14A0_Wr01(data_bridge_0, reg_cvbs_clock_rate, 1);
	// CVD_H13_BRINGUP : clock_control_1 to clock_control_1
	ACE_REG_H14A0_Wr01(clock_control_1, sel_inv_f54m_clock, 1);
	ACE_REG_H14A0_WrFL(data_bridge_0);
	ACE_REG_H14A0_WrFL(clock_control_1);

	// CTop CVD Clock Settings
	// H13_BRINGUP
	/*
	   CTOP_CTRL_H14A0_RdFL(ctr25);
	   CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs54, 0x0);
	   CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs27, 0x0);
	   CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_vbi, 0x0);
	   CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_scart, 0x0);
	   CTOP_CTRL_H14A0_WrFL(ctr25);
	 */

	// 110901 : L9B0 Scart RGB register setting.
	// H13_BRINGUP : no reg_ch3pix_clk_sel register
	/*
	   CTOP_CTRL_H14A0_RdFL(ctr26_reg_extclk_div_sel);
	   CTOP_CTRL_H14A0_Wr01(ctr26_reg_extclk_div_sel, reg_ch3pix_clk_sel, 0x1);
	   CTOP_CTRL_H14A0_WrFL(ctr26_reg_extclk_div_sel);
	 */

	// 3D Comb memory mapping
	if(gpCvdMemCfg->memory_base == 0)
		AFE_PRINT("###### Error Wrong Memory Base Address ##### !!!!!!!!!!!!!!!!!!!!!!!\n");
	else {
		AFE_PRINT("### CVD Buffer Memory Base [0x%x]\n", gpCvdMemCfg->memory_base);
		CVD_H14Ax_RdFL(iris_mif_gmua_007);
		CVD_H14Ax_Wr01(iris_mif_gmua_007, reg_gmau_cmd_base, gpCvdMemCfg->memory_base);
		CVD_H14Ax_WrFL(iris_mif_gmua_007);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_007);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_007, reg_gmau_cmd_base_md, gpCvdMemCfg->memory_base + 0x420000);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_007);
	}

	CVD_H14Ax_RdFL(iris_020);
	CVD_H14Ax_Wr01(iris_020, reg_blue_mode, 0x3);	// from 0x0 atv no-signal free-run

	CVD_H14Ax_WrFL(iris_020);
	// 0909 by kim.min(same setting with FPGA)

	ACE_REG_H14A0_RdFL(afe_cvbs_1);
	ACE_REG_H14A0_RdFL(afe_cvbs_2);

	ACE_REG_H14A0_Wr01(afe_cvbs_1, cvbs_lpf, 1);
	ACE_REG_H14A0_Wr01(afe_cvbs_2, cvbs_bw, 0);
	ACE_REG_H14A0_Wr01(afe_cvbs_2, cvbs_byp, 1);

	ACE_REG_H14A0_WrFL(afe_cvbs_1);
	ACE_REG_H14A0_WrFL(afe_cvbs_2);

	// 1210 Invert Odd/Even Field on B0 Revision(H/W Change or DE Firmware Change?)
	// But This has problem on CC data slicing.
	//gVportRegBx->iris_044.reg_flip_field = 1;
	// Initial Register Setting For Scart Fast Blanking.
	CVD_H14Ax_RdFL(fastblank_001);
	// Blend Ratio 0x08 is for test only
	//CVD_H14Ax_Wr04(fastblank_001, reg_fb_2line_delay, 0x1, reg_fb_blend_ratio, 0x08, reg_fb3ch_delay, 0xC, reg_fb_latency, 0x16);

	// CVD_H13_BRINGUP : SCART FB Changed !!!!
	//	reg_fb3ch_delay removed, reg_fb_2line_delay removed, reg_fb_lmflag_off added.
	//CVD_H14Ax_Wr04(fastblank_001, reg_fb_2line_delay, 0x1, reg_fb_blend_ratio, 0x10, reg_fb3ch_delay, 0xC, reg_fb_latency, 0x16);
	CVD_H14Ax_Wr02(fastblank_001, reg_fb_blend_ratio, 0x10, reg_fb_latency, 0x16);
	CVD_H14Ax_WrFL(fastblank_001);
	//For Test, Mix CVBS & RGB signal
	//gVportRegBx->fastblank_001.reg_fb_blend_ratio = 0x10;
	// For Faster 3D Comb lock(kim.min 20110110)
	// But, This produced 3D-Comb Lock-Unlocking problem on NTSC RF Signal
	// Setting reg_hnon_std_threshold to 0x0c(0x06) solve lock-unlock problem(20110221)
	CVD_H14Ax_RdFL(iris_024);
	//111007 by kim.min
	//CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 0x3);
	//CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 0x9);
	//111121 by kim.min
	//CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 0x7);
	// 121226
	CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 0x7);
	CVD_H14Ax_WrFL(iris_024);

	//111122 by gogosing
	CVD_H14Ax_RdFL(iris_064);
	CVD_H14Ax_Wr01(iris_064, reg_field_latency, 0x0);		//130111 : no hw reset	//0x5	//from default value 0x3
	CVD_H14Ax_WrFL(iris_064);
	//added 121217
	CVD_H14Ax_RdFL(iris_066);
	CVD_H14Ax_Wr01(iris_066, reg_field_latency_md, 0x0);	//130111 : no hw reset	//from default value 0x3
	CVD_H14Ax_WrFL(iris_066);
	//added 121217
	CVD_H14Ax_RdFL(iris_mif_gmua_008);
	CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0x0);	// default value 0 //from default value 0x0
	CVD_H14Ax_WrFL(iris_mif_gmua_008);

	// 110823 by kim.min
	CVD_H14Ax_RdFL(iris_074);
	//CVD_H14Ax_Wr02(iris_073, reg_syncmid_filter_en, 0x1, reg_dcrestore_accum_width, 0x1b);
	// 111102 by kim.min : for brazil PAL-M weak RF
	//CVD_H14Ax_Wr06(iris_073, reg_dcrestore_gain, 0x3, reg_syncmid_nobp_en, 0x1, reg_dcrestore_kill_enable, 0x1, reg_dcrestore_no_bad_bp, 0x1, reg_syncmid_filter_en, 0x1, reg_dcrestore_accum_width, 0x25);
	// 111114 by kim.min : modified reg_dcrestore_gain to 0x1(from 0x3, default 0x0) to fix H/V unlock problem on full white pattern.
	// 111121 by kim.min : modified reg_dcrestore_gain to 0x0(from 0x3, default 0x0)
	// 111121 by kim.min : modified reg_syncmid_nobp_en to 0x0(from 0x1, default 0x0)
	//CVD_H14Ax_Wr06(iris_073, reg_dcrestore_gain, 0x0, reg_syncmid_nobp_en, 0x0, reg_dcrestore_kill_enable, 0x1, reg_dcrestore_no_bad_bp, 0x1, reg_syncmid_filter_en, 0x1, reg_dcrestore_accum_width, 0x25);
	//111129 wonsik.do : dcrestore_accum_width is need to changed by color system
	CVD_H14Ax_Wr05(iris_074, reg_dcrestore_gain, 0x0, reg_syncmid_nobp_en, 0x0, reg_dcrestore_kill_enable, 0x1, reg_dcrestore_no_bad_bp, 0x1, reg_syncmid_filter_en, 0x1);
	CVD_H14Ax_WrFL(iris_074);

	// 111102 by kim.min : for brazil PAL-M weak RF
	CVD_H14Ax_RdFL(iris_071);
	CVD_H14Ax_Wr01(iris_071, reg_cagc_tc_ismall, 0x2);
	CVD_H14Ax_WrFL(iris_071);
	CVD_H14Ax_RdFL(iris_072);
	CVD_H14Ax_Wr02(iris_072, reg_cagc_tc_ibig, 0x1, reg_cagc_tc_p, 0x3);
	CVD_H14Ax_WrFL(iris_072);
	CVD_H14Ax_RdFL(iris_184);
	CVD_H14Ax_Wr03(iris_184, reg_cs_cagc_tc_ismall, 0x2, reg_cs_cagc_tc_ibig, 0x1, reg_cs_cagc_tc_p, 0x3);
	CVD_H14Ax_WrFL(iris_184);
	CVD_H14Ax_RdFL(iris_195);
	CVD_H14Ax_Wr03(iris_195, reg_cs1_cagc_tc_ismall, 0x2, reg_cs1_cagc_tc_ibig, 0x1, reg_cs1_cagc_tc_p, 0x3);
	CVD_H14Ax_WrFL(iris_195);


	// 110929 wonsik.do : restore to default value(agc_peak), for AWC test
	// Fast AGC operation : 110729
#ifdef L9_SLOW_AGC_WORKAROUND
	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_nominal, 0x7F);	//Default : 0x0A
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_cntl, 0x0);		//Default : 0x1

	(iris_025);
#endif

	// At first, VDAC power should be turned off to hide transient artifact : 110803
	CVD_H14Ax_OnOff_VDAC(FALSE);

#if 0
	// by gogosing
	// Use fixed syncmid point, to enhance sync stability on weak RF signal.
	CVD_H14Ax_RdFL(iris_054);
	CVD_H14Ax_Wr01(iris_054, reg_cpump_fixed_syncmid, 0x1);
	// always reset accumulators when no-signal// gogosing 원복 as default 0x01
	//CVD_H14Ax_Wr01(iris_054, reg_cpump_accum_mode, 0x0);
	CVD_H14Ax_WrFL(iris_054);
#else
	CVD_H14Ax_RdFL(iris_055);
	CVD_H14Ax_Wr01(iris_055, reg_cpump_fixed_syncmid, 0x0);
	CVD_H14Ax_Wr01(iris_055, reg_cpump_accum_mode, 0x0);
	CVD_H14Ax_WrFL(iris_055);
#endif

	// #### iris_014 Register Settings From Program_Color_System_Main_Multi ####
	//CVD_H14Ax_RdFL(iris_015);
	// For faster agc speed at channel change, set agc_half_en value to '0'
	//CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 0);
	// For faster agc speed at channel change, set hagc_field_mode value to '1'
	//CVD_H14Ax_Wr01(iris_014, reg_hagc_field_mode, 1);
	//CVD_H14Ax_WrFL(iris_015);

#ifdef L9_USE_SYNCTIP_ONLY_MODE
	//gogosing reg_hmgc 0x40 --> 0x60 with sync tip mode clamp
	CVD_H14Ax_RdFL(iris_076);
	CVD_H14Ax_Wr01(iris_076, reg_hmgc, 0x60);
	CVD_H14Ax_WrFL(iris_076);

	CVD_H14Ax_RdFL(iris_015);
	// dc_clamp_mode : 0(auto), 1(backporch), 2(synctip), 3(off)
	CVD_H14Ax_Wr01(iris_015, reg_dc_clamp_mode, 0x2);
	CVD_H14Ax_WrFL(iris_015);
#else
	CVD_H14Ax_RdFL(iris_076);
	CVD_H14Ax_Wr01(iris_076, reg_hmgc, 0x60);
	CVD_H14Ax_WrFL(iris_076);

	CVD_H14Ax_RdFL(iris_015);
	// dc_clamp_mode : 0(auto), 1(backporch), 2(synctip), 3(off)
	CVD_H14Ax_Wr01(iris_015, reg_dc_clamp_mode, 0x0);
	CVD_H14Ax_WrFL(iris_015);
#endif

	//110823 : New setting values by kim.min
	CVD_H14Ax_RdFL(iris_054);
	CVD_H14Ax_Wr01(iris_054, reg_cpump_auto_stip_noisy, 0x1);
	CVD_H14Ax_Wr01(iris_054, reg_cpump_auto_stip_no_signal, 0x1);
	CVD_H14Ax_Wr01(iris_054, reg_cpump_auto_stip_unlocked, 0x1);
	CVD_H14Ax_WrFL(iris_054);

	//0906 : filed inversion is needed ???
	//0909 : CVD filed inversion
	CVD_H14Ax_RdFL(iris_043);
	//CVD_H14Ax_Wr01(iris_043, reg_field_polarity, 0x1);	// setting to '1' inverts CVD odd/even field
	CVD_H14Ax_Wr01(iris_043, reg_field_polarity, 0x0);	// setting to '1' inverts CVD odd/even field
	CVD_H14Ax_WrFL(iris_043);

	//111104 by kim.min : picture blinking on weak RF signal
	CVD_H14Ax_RdFL(iris_017);
	CVD_H14Ax_Wr01(iris_017, reg_noise_thresh, 0x80);	// default 0x32
	CVD_H14Ax_WrFL(iris_017);

	//111121 kim.min : modified SCART RGB CSC.
	// Initial black level low
	CVD_H14Ax_Set_SCART_CSC(0);

	CVD_H14Ax_Set_SCART_Filter();

	//111221 by kim.min for better color standard detection performance
	CVD_H14Ax_RdFL(iris_188);
	CVD_H14Ax_Wr01(iris_188, reg_cs_chroma_sel, 0x1);	// default 0x0
	CVD_H14Ax_WrFL(iris_188);
	CVD_H14Ax_RdFL(iris_199);
	CVD_H14Ax_Wr01(iris_199, reg_cs1_chroma_sel, 0x1);	// default 0x0
	CVD_H14Ax_WrFL(iris_199);

#ifdef H13_CVD_INIT_OADJ_C
	CVD_H14Ax_RdFL(iris_121);
	CVD_H14Ax_Wr01(iris_121, reg_oadj_c_offi, 0x600);	// 2012.02.24 won.hur
	CVD_H14Ax_Wr01(iris_121, reg_oadj_c_offo, 0x200);	// 2012.02.24 won.hur
	CVD_H14Ax_WrFL(iris_121);

	// H13_BRINGUP
	CVD_H14Ax_RdFL(iris_123);
	CVD_H14Ax_Wr01(iris_123, reg_oadj_cr_offi, 0x600);	
	CVD_H14Ax_Wr01(iris_123, reg_oadj_cr_offo, 0x200);
	CVD_H14Ax_WrFL(iris_123);

	CVD_H14Ax_RdFL(iris_122);
	CVD_H14Ax_Wr01(iris_122, reg_oadj_c_coeff, 0x1000);	// 2012.02.24 won.hur
	// H13_BRINGUP
	CVD_H14Ax_Wr01(iris_122, reg_oadj_cr_coeff, 0x1000);
	CVD_H14Ax_WrFL(iris_122);
#endif


#ifdef L9_FAST_3DCOMB_WORKAROUND
	//120218 for fast 3D Comb operation
	CVD_H14Ax_RdFL(iris_067);
	CVD_H14Ax_Wr01(iris_067, reg_vf_nstd_en, 0x0);	// default 0x1
	CVD_H14Ax_WrFL(iris_067);
#endif
#ifdef H14_FAST_3DCOMB_WORKAROUND
	//120803 : WA for 3DComb buffer error
	CVD_H14Ax_RdFL(iris_067);
	CVD_H14Ax_Wr01(iris_067, reg_vf_nstd_en, 0x1);	// default 0x1
	CVD_H14Ax_WrFL(iris_067);
#endif
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_hagc_en, 0x1);
	CVD_H14Ax_WrFL(iris_015);

	CVD_H14Ax_RdFL(iris_019);
	CVD_H14Ax_Wr01(iris_019, reg_adc_updn_swap, 0x1);
	CVD_H14Ax_WrFL(iris_019);

	//121119 : for Fast Chroma Lock/Unlocking !!!
	CVD_H14Ax_RdFL(iris_047);
	CVD_H14Ax_Wr01(iris_047, reg_lose_chromalock_level, 0x5);	//default 0x7
	CVD_H14Ax_WrFL(iris_047);

	//121205 : Fast H/V Locking on channel change
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 1);
	CVD_H14Ax_WrFL(iris_015);

	//130123 : new cordic setting
	CVD_H14Ax_RdFL(iris_344);
	CVD_H14Ax_RdFL(iris_345);
	CVD_H14Ax_RdFL(iris_346);
	CVD_H14Ax_Wr01(iris_344, reg_cordic_ctrl0, 		0x80000003);
	CVD_H14Ax_Wr01(iris_345, reg_cs_cordic_ctrl0, 	0x80000003);
	CVD_H14Ax_Wr01(iris_346, reg_cs1_cordic_ctrl0, 	0x80000003);
	CVD_H14Ax_WrFL(iris_344);
	CVD_H14Ax_WrFL(iris_345);
	CVD_H14Ax_WrFL(iris_346);

//	CVD_H14Ax_Init_Level_Detector();
}

int CVD_H14Ax_Set_Source_Type(LX_AFE_CVD_SET_INPUT_T	cvd_input_info)
{

	//120118 : first disable SCART FB at source change
	CVD_H14Ax_Set_Scart_FB_En(0);
	gEnableScartFB = 0;		// not used

	if(cvd_input_info.cvd_input_source_type == LX_CVD_INPUT_SOURCE_CVBS) // for composite
	{

		//			AFE_PRINT("Composite input\n");
		CVD_H14Ax_RdFL(iris_013);
		CVD_H14Ax_Wr01(iris_013, reg_yc_src, 0);
		CVD_H14Ax_WrFL(iris_013);
		CVD_DEBUG("Input source = [%d]\n", cvd_input_info.cvbs_input_port);
		//gVportRegBx->top_002.reg_exta_sel = 0;	// from cvd_test.cmm
		//gVportRegBx->top_002.reg_extb_sel = 0;	// from cvd_test.cmm
		switch(cvd_input_info.cvbs_input_port)
		{
			case LX_AFE_CVBS_IN1:
				ACE_REG_H14A0_RdFL(afe_cvbs_3);
				ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_insel, 0x0);
				ACE_REG_H14A0_WrFL(afe_cvbs_3);
				break;
			case LX_AFE_CVBS_IN2:
				ACE_REG_H14A0_RdFL(afe_cvbs_3);
				ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_insel, 0x1);
				ACE_REG_H14A0_WrFL(afe_cvbs_3);
				break;
			case LX_AFE_CVBS_IN3:
				ACE_REG_H14A0_RdFL(afe_cvbs_3);
				ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_insel, 0x2);
				ACE_REG_H14A0_WrFL(afe_cvbs_3);
				break;
			case LX_AFE_CVBS_IN4:
			case LX_AFE_CVBS_IN5:
			case LX_AFE_CVBS_IN6:
			default:
				AFE_ERROR("No [%d] port supported !!!! [%s][%d]\n", cvd_input_info.cvbs_input_port, __func__, __LINE__ );
				break;
		}
	}
	else // for S-Video
	{
		AFE_ERROR("S-Video is not supported.\n");
	}
	//Current_State = CVD_STATE_VideoNotReady;
	//Next_State = CVD_STATE_VideoNotReady;
	return 0;
}

int CVD_H14Ax_Set_Scart_Overlay(BOOLEAN bEnable)
{
	if(bEnable == TRUE)
	{
		// 110901 : L9B0 Scart RGB register setting.
		// Moved to Reg_Init
		/*
		CTOP_CTRL_L9B_RdFL(ctr26_reg_extclk_div_sel);
		CTOP_CTRL_L9B_Wr01(ctr26_reg_extclk_div_sel, reg_ch3pix_clk_sel, 0x1);
		CTOP_CTRL_L9B_WrFL(ctr26_reg_extclk_div_sel);
		*/

		//20111110 : modified not to directly set reg_fb_en register. reg_fb_en is set in ADC periodic task.
		gEnableScartFB = 1;
		/*
		CVD_H14Ax_RdFL(fastblank_001);
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_en, 0x1);
		CVD_H14Ax_WrFL(fastblank_001);
		*/
		// Following Register settings were move to CVD_L9Ax_Reg_Init
		/*
		   gVportRegBx->fastblank_001.reg_fb_2line_delay = 0x1;
		   gVportRegBx->fastblank_001.reg_fb_blend_ratio = 0x8;
		   gVportRegBx->fastblank_001.reg_fb3ch_delay = 0xC;
		   gVportRegBx->fastblank_001.reg_fb_latency = 0x16;
		 */
	}
	else
	{
		//20111110 : modified not to directly set reg_fb_en register. reg_fb_en is set in ADC periodic task.
		gEnableScartFB = 0;
		//20111221 : scart FB enable should be disabled immediately (ADC periodic task can go to sleep state on AV/ATV input condition)
		//20120118 : If SCART Fast Blanking toggle, immediate call to Scart FB EN result in screen flickering.
	//	CVD_H14Ax_Set_Scart_FB_En(0);	// Disable FB_EN
		/*
		CVD_H14Ax_RdFL(fastblank_001);
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_en, 0x0);
		CVD_H14Ax_WrFL(fastblank_001);
		*/
		//		gVportRegBx->fastblank_001.reg_fb_blend_ratio = 0x8;
	}
	return 0;
}

int CVD_H14Ax_Get_Scart_FB_En(void)
{
	int ret;
	CVD_H14Ax_RdFL(fastblank_001);
	CVD_H14Ax_Rd01(fastblank_001, reg_fb_en, ret);
	return ret;
}

int CVD_H14Ax_Set_Scart_FB_En(int fb_en_ctrl)
{
	int ret = 0;
	static int fb_en_status = 0;

	if( (fb_en_ctrl > 0) && (fb_en_status == 0))  {
		CVD_H14Ax_RdFL(fastblank_001);
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_en, 0x1);
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_blend_ratio, 0x10);
		CVD_H14Ax_WrFL(fastblank_001);
		fb_en_status = 1;
	}
	else if ( (fb_en_ctrl == 0) && (fb_en_status > 0) ) {
		CVD_H14Ax_RdFL(fastblank_001);
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_en, 0x0);
		// When Input Change from SCART RGB to AV/ATV, prevent CVD black screen 
		CVD_H14Ax_Wr01(fastblank_001, reg_fb_blend_ratio, 0x0);
		CVD_H14Ax_WrFL(fastblank_001);
		fb_en_status = 0;
	}
	else
		ret = -1;

	return ret;
}

UINT8 CVD_H14Ax_Get_FC_Flag(LX_AFE_CVD_SELECT_T select_main_sub)
{
	UINT8 cordic_freq_status = 0;

	CVD_H14Ax_RdFL(iris_008);
	CVD_H14Ax_Rd01(iris_008, reg_status_cordic_freq, cordic_freq_status);
	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);

	if(cordic_freq_status > FC_MORE_THRESHOLD)
		return CVD_FC_MORE_FLAG;
	else if(cordic_freq_status < FC_LESS_THRESHOLD)
		return CVD_FC_LESS_FLAG;
	else
		return CVD_FC_SAME_FLAG;
}

#if 1
//gogosing burst mag status check for color burst level test (color 틀어짐 대응) 2011.06.11
UINT8 CVD_H14Ax_Get_CVD_Burst_Mag_Flag(CVD_STATE_T	color_system)
{
	UINT16 burst_mag_status;
	CVD_H14Ax_RdFL(iris_004);
	CVD_H14Ax_Rd01(iris_004,reg_status_burst_mag,burst_mag_status);

	//111212 by kd.park for MBC low burst magnitude
	if(color_system == CVD_STATE_NTSC) {
		if (g_CVD_Pattern_Detection_t.pattern_found == 1 ) {	// New Pattern Detection using Global Motion Value
			if(burst_mag_status > 0x650)
				return CVD_BURST_MAG_STATE_BIG; // big : 3
			else if(burst_mag_status < 0x520 && burst_mag_status > 0x420)
				return CVD_BURST_MAG_STATE_SMALL; // small : 2
			else if(burst_mag_status < 0x240)
				return CVD_BURST_MAG_STATE_VERY_SMALL; //very small : 1
			else
				return CVD_BURST_MAG_STATE_SAME; // same : 0
		}
		else
			return CVD_BURST_MAG_STATE_BIG; // big : 3
	}
	else {
	// Pattern detection for PAL disabled
	//	if (g_CVD_AGC_Peak_Nominal_Control.pattern_found == 1 ) {	// PAL RF with test pattern detected ???
			if(burst_mag_status > 0x520)//0x620 --> 0x520
				return CVD_BURST_MAG_STATE_BIG; // big : 3
			else if(burst_mag_status < 0x420 && burst_mag_status > 0x320)
				return CVD_BURST_MAG_STATE_SMALL; // small : 2
			else if(burst_mag_status < 0x240)
				return CVD_BURST_MAG_STATE_VERY_SMALL; //very small : 1
			else
				return CVD_BURST_MAG_STATE_SAME; // same : 0
	//	}
	//	else
	//		return CVD_BURST_MAG_STATE_BIG; // big : 3
	}
}

int CVD_H14Ax_Set_CVD_CAGC(UINT8 state,CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t, CVD_STATE_T	color_system)
{
#ifdef CVD_REG_OADJ_C_COEFF_ADJUST
	int color_compensator_value;
	int oadj_c_coeff_value;
#endif
#ifdef CVD_REG_SATURATION_ADJUST
	int saturation_value;
#endif
	int initial_cagc_value;

	if(state==0)
 		return 0;//not changed

	if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_HIGH)	// NTSC-J
		initial_cagc_value = g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSCj].reg_cagc;
	else
		initial_cagc_value = g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSC].reg_cagc;

    switch(state)
	{
		case 3: //big
			CVD_H14Ax_RdFL(iris_022);
			if(g_CVD_Status_3CS.in_rf_mode == TRUE) {
				CVD_H14Ax_Wr01(iris_022, reg_cagc, pSet_system_t->reg_cagc);

				CVD_WA_DEBUG("Write CAGC Value[BIG](%d) : [0x%x]\n",__LINE__, pSet_system_t->reg_cagc);
				if(color_system == CVD_STATE_NTSC) {

#ifdef CVD_REG_OADJ_C_COEFF_ADJUST
					color_compensator_value = 0 ;
					oadj_c_coeff_value = color_compensator_value + g_initial_c_coeff;
					CVD_H14Ax_RdFL(iris_122);
					CVD_H14Ax_Wr01(iris_122, reg_oadj_c_coeff, oadj_c_coeff_value);
					// H13_BRINGUP
					CVD_H14Ax_Wr01(iris_122, reg_oadj_cr_coeff, oadj_c_coeff_value);
					CVD_H14Ax_WrFL(iris_122);
					CVD_WA_DEBUG("Saturation Value(oadj_c_coeff)(%d) : [0x%x]\n",__LINE__, oadj_c_coeff_value);
#endif
#ifdef CVD_REG_SATURATION_ADJUST
					saturation_value = pSet_system_t->reg_saturation ;
					CVD_H14Ax_RdFL(iris_021);
					CVD_H14Ax_Wr01(iris_021, reg_saturation, saturation_value);
					CVD_H14Ax_WrFL(iris_021);
//					CVD_WA_DEBUG("Saturation Value : [0x%x]\n", saturation_value);
#endif
				}
			}
			else
				CVD_H14Ax_Wr01(iris_022, reg_cagc, pSet_system_t->reg_cagc_av);
			CVD_H14Ax_WrFL(iris_022);
			break;

		case 2: // small
			CVD_H14Ax_RdFL(iris_022);
			if(color_system == CVD_STATE_NTSC) {
				if (g_CVD_AGC_Peak_Nominal_Control.pattern_found == 1 ) {	// NTSC RF with test pattern detected ???
					CVD_H14Ax_Wr01(iris_022, reg_cagc,0x50); //120109 by kd.park for test pattern
					CVD_WA_DEBUG("Write CAGC Value[NT/S](%d) : [0x%x]\n",__LINE__, 0x50);
				}
				else {
					CVD_H14Ax_Wr01(iris_022, reg_cagc,0x50); //111212 by kd.park for MBC low burst magnitude
					CVD_WA_DEBUG("Write CAGC Value[NT/S](%d) : [0x%x]\n",__LINE__, 0x50);
				}
			}
			else {
				CVD_H14Ax_Wr01(iris_022, reg_cagc,0x45); //0x45); color 수평 noise 대응 0x45 --> 0x2c
				CVD_WA_DEBUG("Write CAGC Value[PAL/S](%d) : [0x%x]\n",__LINE__, 0x45);
			}
			CVD_H14Ax_WrFL(iris_022);
			break;

		case 1: // very small
			CVD_H14Ax_RdFL(iris_022);

			if(color_system == CVD_STATE_NTSC) {
				CVD_H14Ax_Wr01(iris_022, reg_cagc,0x20); //120127 : by kd.park

				CVD_WA_DEBUG("Write CAGC Value[NT](%d) : [0x%x]\n",__LINE__, state);
#ifdef CVD_REG_OADJ_C_COEFF_ADJUST
				color_compensator_value = (initial_cagc_value - 0x20) * 0x20 ;
				oadj_c_coeff_value = color_compensator_value + g_initial_c_coeff;
				CVD_H14Ax_RdFL(iris_122);
				CVD_H14Ax_Wr01(iris_122, reg_oadj_c_coeff, oadj_c_coeff_value);
				// H13_BRINGUP
				CVD_H14Ax_Wr01(iris_122, reg_oadj_cr_coeff, oadj_c_coeff_value);
				CVD_H14Ax_WrFL(iris_122);
				CVD_WA_DEBUG("Saturation Value[NT](oadj_c_coeff) : [0x%x]\n", oadj_c_coeff_value);
#endif
#ifdef CVD_REG_SATURATION_ADJUST
				saturation_value = pSet_system_t->reg_saturation + (0x80 - 0x20);
				CVD_H14Ax_RdFL(iris_021);
				CVD_H14Ax_Wr01(iris_021, reg_saturation, saturation_value);
				CVD_H14Ax_WrFL(iris_021);
				//					CVD_DEBUG("Saturation Value : [0x%x]\n", saturation_value);
#endif
			}
			else
			{
				CVD_H14Ax_Wr01(iris_022, reg_cagc,0x2c); //0x45); color 수평 noise 대응 0x45 --> 0x2c
				CVD_WA_DEBUG("Write CAGC Value[PAL/S](%d) : [0x%x]\n",__LINE__, 0x2c);
			}

			CVD_H14Ax_WrFL(iris_022);
			break;

		default:
			if( (color_system == CVD_STATE_NTSC) && (state >= 20)) {

				if(state > initial_cagc_value)
					state = initial_cagc_value;

				CVD_H14Ax_RdFL(iris_022);

				CVD_H14Ax_Wr01(iris_022, reg_cagc, state);

				CVD_H14Ax_WrFL(iris_022);
				CVD_WA_DEBUG("Write CAGC Value[NT](%d) : [0x%x]\n",__LINE__, state);

#ifdef CVD_REG_OADJ_C_COEFF_ADJUST
					color_compensator_value = (initial_cagc_value - state) * 0x20 ;
					oadj_c_coeff_value = color_compensator_value + g_initial_c_coeff;
					CVD_H14Ax_RdFL(iris_122);
					CVD_H14Ax_Wr01(iris_122, reg_oadj_c_coeff, oadj_c_coeff_value);
					// H13_BRINGUP
					CVD_H14Ax_Wr01(iris_122, reg_oadj_cr_coeff, oadj_c_coeff_value);
					CVD_H14Ax_WrFL(iris_122);
					CVD_WA_DEBUG("Saturation Value[NT](oadj_c_coeff) : [0x%x]\n", oadj_c_coeff_value);
#endif
#ifdef CVD_REG_SATURATION_ADJUST
				saturation_value = pSet_system_t->reg_saturation + (0x80 - state);
				CVD_H14Ax_RdFL(iris_021);
				CVD_H14Ax_Wr01(iris_021, reg_saturation, saturation_value);
				CVD_H14Ax_WrFL(iris_021);

//				CVD_DEBUG("Saturation Value : [0x%x]\n", saturation_value);
#endif
			}

			break;
	}

	return 0;
}
#endif

UINT8 CVD_H14Ax_Get_Cordic_Freq(LX_AFE_CVD_SELECT_T select_main_sub)
{
	UINT8 cordic_freq_status = 0;

	CVD_H14Ax_RdFL(iris_008);
	CVD_H14Ax_Rd01(iris_008, reg_status_cordic_freq, cordic_freq_status);
	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);

	return cordic_freq_status;
}

UINT8 CVD_H14Ax_Get_Cordic_Freq_CS0(void)
{
	UINT8 cordic_freq_status;
	CVD_H14Ax_RdFL(iris_189);
	CVD_H14Ax_Rd01(iris_189, reg_cs_status_cordic_freq, cordic_freq_status);
	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);
	return cordic_freq_status;
}

UINT8 CVD_H14Ax_Get_FC_Flag_CS0(void)
{
	UINT8 cordic_freq_status;

	CVD_H14Ax_RdFL(iris_189);
	CVD_H14Ax_Rd01(iris_189, reg_cs_status_cordic_freq, cordic_freq_status);

	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);

	if(cordic_freq_status > FC_MORE_THRESHOLD)
		return CVD_FC_MORE_FLAG;
	else if(cordic_freq_status < FC_LESS_THRESHOLD)
		return CVD_FC_LESS_FLAG;
	else
		return CVD_FC_SAME_FLAG;
}

UINT8 CVD_H14Ax_Get_Cordic_Freq_CS1(void)
{
	UINT8 cordic_freq_status;
	CVD_H14Ax_RdFL(iris_200);
	CVD_H14Ax_Rd01(iris_200, reg_cs1_status_cordic_freq, cordic_freq_status);
	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);
	return cordic_freq_status;
}

UINT8 CVD_H14Ax_Get_FC_Flag_CS1(void)
{
	UINT8 cordic_freq_status;

	CVD_H14Ax_RdFL(iris_200);
	CVD_H14Ax_Rd01(iris_200, reg_cs1_status_cordic_freq, cordic_freq_status);

	cordic_freq_status = (UINT8)((SINT8)cordic_freq_status + 0x80);

	if(cordic_freq_status > FC_MORE_THRESHOLD)
		return CVD_FC_MORE_FLAG;
	else if(cordic_freq_status < FC_LESS_THRESHOLD)
		return CVD_FC_LESS_FLAG;
	else
		return CVD_FC_SAME_FLAG;
}

int CVD_H14Ax_Get_FB_Status(LX_AFE_SCART_MODE_T *pScart_fb_mode)
{
	UINT32					status_fb_in_cnt;

	// FB Status Register has Bug ( '0' '1' transition when fb signal exist)
	CVD_H14Ax_RdFL(iris_364);
	CVD_H14Ax_Rd01(iris_364, reg_status_fb_in_cnt, status_fb_in_cnt);

	if(status_fb_in_cnt > 0x32a00)	// 720*576 = 0x65400, 0x65400/2 = 0x32a00
		*pScart_fb_mode = LX_SCART_MODE_RGB;
	else
		*pScart_fb_mode = LX_SCART_MODE_CVBS;
#if 0
	UINT8	scart_fb_state;
	CVD_H14Ax_RdFL(fast_blank_status_001);
	CVD_H14Ax_Rd01(fast_blank_status_001, reg_fb_state, scart_fb_state);
	if(scart_fb_state > 0)
		*pScart_fb_mode = LX_SCART_MODE_RGB;
	else
		*pScart_fb_mode = LX_SCART_MODE_CVBS;
#endif 
	return 0;
}

int CVD_H14Ax_Get_Scart_AR(LX_AFE_SCART_AR_INFO_T	*pScart_ar_param)
{
	UINT8	sc1_sid1, sc1_sid2;

	if(pScart_ar_param->Scart_Id == LX_SCART_ID_1)
	{
		ACE_REG_H14A0_RdFL(afe_3ch_6);
		ACE_REG_H14A0_Rd02(afe_3ch_6, afe3ch_sc1_sid1, sc1_sid1, afe3ch_sc1_sid2, sc1_sid2);

		if((sc1_sid2==0)&&(sc1_sid1==0))
			pScart_ar_param->Scart_AR = LX_SCART_AR_INVALID;
		else if((sc1_sid2==1)&&(sc1_sid1==1))
			pScart_ar_param->Scart_AR = LX_SCART_AR_4_3;
		else
			pScart_ar_param->Scart_AR = LX_SCART_AR_16_9;
	}
	else
		return -1;

	return 0;
}

void CVD_H14Ax_Print_Vport_Version(void)
{
	UINT32 vport_version = 0;
	CVD_H14Ax_RdFL(top_001);
	CVD_H14Ax_Rd01(top_001, iris_ver, vport_version);
	AFE_PRINT("Vport Version : [%x]\n", vport_version);
}

int CVD_H14Ax_Vport_Output(UINT32 arg)
{
	// No more use

	return 0;
}


int CVD_H14Ax_Get_No_Signal_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_001);
	CVD_H14Ax_Rd01(iris_001, reg_no_signal, ret);
	return ret;
}

int CVD_H14Ax_Get_HLock_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_001);
	CVD_H14Ax_Rd01(iris_001, reg_hlock, ret);
	return ret;
}

int CVD_H14Ax_Get_VLock_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_001);
	CVD_H14Ax_Rd01(iris_001, reg_vlock, ret);
	return ret;
}

int CVD_H14Ax_Get_Vline_625_Flag(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_vline_625_detected, ret);

	return ret;
}

int CVD_H14Ax_Fast_Vline_625_Using_Vdetec_Vcount(void)
{
	//return 1;
	static int wrong_vline_count = 0;
	static int vline_625_count = 0;
	static int vline_525_count = 0;
	int ret, vline_reg, vcount;

	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_vline_625_detected, ret);

	//for fixed vfreq system, 50/60Hz detection is no use.
	if( (( g_CVD_Status_3CS.color_system_support & (LX_COLOR_SYSTEM_NTSC_M |LX_COLOR_SYSTEM_PAL_M | LX_COLOR_SYSTEM_NTSC_443 | LX_COLOR_SYSTEM_PAL_60)) == 0 ) || \
		( ( g_CVD_Status_3CS.color_system_support & (LX_COLOR_SYSTEM_PAL_G |LX_COLOR_SYSTEM_PAL_NC | LX_COLOR_SYSTEM_SECAM)) == 0 ) )
		return ret;

	CVD_H14Ax_RdFL(iris_013);
	CVD_H14Ax_Rd01(iris_013, reg_vline_625, vline_reg);

	CVD_H14Ax_RdFL(iris_274);
	CVD_H14Ax_Rd01(iris_274, reg_status_vdetect_vcount, vcount);

	if( (vline_reg == 0x1) && (ret == 0x1) && (vcount > 0x340) && (vcount < 0x350) && (CVD_H14Ax_Get_VLock_Flag() > 0) )			// 0x344
	{
		CVD_DEBUG("!525 line ???, vcount = [0x%x]\n", vcount);
		wrong_vline_count ++;
	}
	else
		wrong_vline_count = 0;

	if( (vline_reg == 0x0) && (ret == 0x1) && (CVD_H14Ax_Get_VLock_Flag() > 0)  && ( vcount < 0x250) && (vcount > 0x230) )	// 0x23e
	{
		CVD_DEBUG("625 line ???, vcount = [0x%x]\n", vcount);
		vline_625_count ++;
	}
	else
		vline_625_count = 0;

	if( (vline_reg == 0x1) && (ret == 0x0) && (CVD_H14Ax_Get_VLock_Flag() > 0))	// 0x23e
	{
		CVD_DEBUG("525 line ???, vcount = [0x%x]\n", vcount);
		vline_525_count ++;
	}
	else
		vline_525_count = 0;

	if (wrong_vline_count > 1)
	{
		wrong_vline_count = 0;
		CVD_DEBUG("Force Set vline to 525 line\n");
		CVD_H14Ax_Wr01(iris_013, reg_vline_625, 0);
		CVD_H14Ax_WrFL(iris_013);
	}
	else if( vline_625_count > 1)
	{
		vline_625_count = 0;
		CVD_DEBUG("Set vline to 625 line\n");
		CVD_H14Ax_Wr01(iris_013, reg_vline_625, 1);
		CVD_H14Ax_WrFL(iris_013);
	}
	else if( vline_525_count > 1)
	{
		vline_525_count = 0;
		CVD_DEBUG("Set vline to 525 line\n");
		CVD_H14Ax_Wr01(iris_013, reg_vline_625, 0);
		CVD_H14Ax_WrFL(iris_013);
	}

	return ret;
}

int CVD_H14Ax_Get_Vdetect_Vcount_625_Flag(void)
{
	//return 1;
	static int vline_625_flag = 0;
	int vcount;
	CVD_H14Ax_RdFL(iris_274);
	CVD_H14Ax_Rd01(iris_274, reg_status_vdetect_vcount, vcount);

	if( (vcount > 0x1FC) && (vcount < 0x21c))
		vline_625_flag = 0;
	else if( (vcount > 0x260) && (vcount < 0x280))
		vline_625_flag = 1;
	else
		CVD_DEBUG("vcount unstable [%d]\n", vcount);

	return vline_625_flag;
}


int CVD_H14Ax_Get_PAL_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_pal_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_SECAM_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_secam_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_Chromalock_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_001);
	CVD_H14Ax_Rd01(iris_001, reg_chromalock, ret);
	return ret;
}

int CVD_H14Ax_Get_PAL_Flag_CS0(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_190);
	CVD_H14Ax_Rd01(iris_190, reg_cs_pal_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_SECAM_Flag_CS0(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_189);
	CVD_H14Ax_Rd01(iris_189, reg_cs_secam_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_Chromalock_Flag_CS0(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_190);
	CVD_H14Ax_Rd01(iris_190, reg_cs_chromalock, ret);
	return ret;
}

int CVD_H14Ax_Get_PAL_Flag_CS1(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_201);
	CVD_H14Ax_Rd01(iris_201, reg_cs1_pal_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_SECAM_Flag_CS1(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_200);
	CVD_H14Ax_Rd01(iris_200, reg_cs1_secam_detected, ret);
	return ret;
}

int CVD_H14Ax_Get_Chromalock_Flag_CS1(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_201);
	CVD_H14Ax_Rd01(iris_201, reg_cs1_chromalock, ret);
	return ret;
}

int CVD_H14Ax_Get_Noise_Status(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_009);
	CVD_H14Ax_Rd01(iris_009, reg_status_noise, ret);
	return ret;
}

int CVD_H14Ax_Get_NoBurst_Flag(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_noburst_detected, ret);
	return ret;
}

int CVD_H14Ax_Reset_irisyc(int enable)
{
	if(enable > 0)
	{
	// CVD_H13_BRINGUP : soft_reset_5 to soft_reset_2
		ACE_REG_H14A0_RdFL(soft_reset_2);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_f54m, 1);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_cvbs, 1);
		ACE_REG_H14A0_WrFL(soft_reset_2);
	}
	else
	{
	// CVD_H13_BRINGUP : soft_reset_5 to soft_reset_2
		ACE_REG_H14A0_RdFL(soft_reset_2);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_f54m, 0);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_cvbs, 0);
		ACE_REG_H14A0_WrFL(soft_reset_2);
	}

	return 0;
}

int CVD_H14Ax_Reset_vdct(int enable)
{
	if(enable >0)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_vdct, 1);
		CVD_H14Ax_WrFL(iris_175);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_vdct, 0);
		CVD_H14Ax_WrFL(iris_175);
	}
	return 0;
}

int CVD_H14Ax_Reset_hdct(int enable)
{
	if(enable >0)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_hdct, 1);
		CVD_H14Ax_WrFL(iris_175);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_hdct, 0);
		CVD_H14Ax_WrFL(iris_175);
	}
	return 0;
}

int CVD_H14Ax_Reset_cdct(int enable)
{
	if(enable >0)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_cdct, 1);
		CVD_H14Ax_WrFL(iris_175);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, swrst_cdct, 0);
		CVD_H14Ax_WrFL(iris_175);
	}
	return 0;
}

// This function needs to be modified or deleted due to the delection of
// IRIS_209 register.
// Code related to this register is disabled!
int	CVD_H14Ax_Read_fld_cnt_value(UINT16 *pfld_hfcnt_value, UINT16 *pfld_lfcnt_value)
{
#if 0 //won.hur
	CVD_H14Ax_RdFL(iris_209);
	CVD_H14Ax_Rd02(iris_209, status_fld_lfcnt, *pfld_lfcnt_value, status_fld_hfcnt, *pfld_hfcnt_value);
	return 0;
#else
	return -1;
#endif
}


// This function needs to be modified or deleted due to the delection of
// IRIS_190 register.
// Code related to this register is disabled!
int CVD_H14Ax_Set_motion_mode(UINT8	md_mode_value, UINT8 motion_mode_value)
{

#if 0 // won.hur
	CVD_H14Ax_RdFL(iris_190);
	CVD_H14Ax_Wr01(iris_190, cvd_3dcomb_md_mode, md_mode_value);
	CVD_H14Ax_WrFL(iris_190);
#endif // won.hur

	CVD_H14Ax_RdFL(iris_063);
	CVD_H14Ax_Wr01(iris_063, reg_motion_mode, motion_mode_value);
	CVD_H14Ax_WrFL(iris_063);
	return 0;
}



// won.hur : This function needs to be changed!!!
// This is because L9B0 has no ghslvdstx2
// This part is disabled!!
int CVD_H14Ax_Channel_Power_Control(UINT32 on_off)
{
	UINT8	dr3p_pdb_status, cvbs_pdbm_status, cvbs_pdb_status, cvbs_cp_status;
	static int power_status = 0;

	CVD_DEBUG("%s entered :  %d \n",__func__, on_off);

	if(on_off)	// power ON
	{
		OS_MsecSleep(5);

		// Normal condition
		ACE_REG_H14A0_RdFL(main_pll_4);
		ACE_REG_H14A0_RdFL(afe_cvbs_1);
		ACE_REG_H14A0_RdFL(afe_cvbs_3);

		ACE_REG_H14A0_Rd01(afe_cvbs_1, cvbs_pdbm, cvbs_pdbm_status);
		ACE_REG_H14A0_Rd01(afe_cvbs_3, cvbs_pdb, cvbs_pdb_status);
		ACE_REG_H14A0_Rd01(main_pll_4, dr3p_pdb, dr3p_pdb_status);
		ACE_REG_H14A0_Rd01(afe_cvbs_3, cvbs_cp, cvbs_cp_status);

		if(cvbs_pdbm_status && cvbs_pdb_status && dr3p_pdb_status && cvbs_cp_status)	// all cvd power is already on !!@!!
		{
			CVD_DEBUG("%s : CVD Power Already ON !!!\n",__func__);
			return 0;
		}

		ACE_REG_H14A0_Wr01(afe_cvbs_1, cvbs_pdbm, 0x1);
		ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_pdb, 0x1);
		ACE_REG_H14A0_Wr01(main_pll_4, dr3p_pdb, 0x1);

		//added 110622 : by tommy.lee to disable clamp control on power down condition
		ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_cp, 1);
		ACE_REG_H14A0_WrFL(main_pll_4);
		ACE_REG_H14A0_WrFL(afe_cvbs_1);
		ACE_REG_H14A0_WrFL(afe_cvbs_3);

#ifdef H13_IRIS_CLAMPAGC_V2_CONTROL_WA_FOR_CVD_UNLOCK
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_clampagc_v2, 0);
		CVD_H14Ax_WrFL(iris_175);
#endif
		OS_MsecSleep(20);
		//by dws : remove mdelay
		//mdelay(5);
		//OS_MsecSleep(5);

		CTOP_CTRL_H14A0_RdFL(ctr25);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs54, 0x0);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs27, 0x0);
		CTOP_CTRL_H14A0_WrFL(ctr25);

		//121210 : cvd soft reset during ADC Power down condition
		CVD_H14Ax_RdFL(iris_012);
		CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 0);
		CVD_H14Ax_WrFL(iris_012);

/*
		ACE_REG_H14A0_RdFL(soft_reset_5);
		ACE_REG_H14A0_Wr01(soft_reset_5, swrst_f54m, 0);
		ACE_REG_H14A0_Wr01(soft_reset_5, swrst_cvbs, 0);
		ACE_REG_H14A0_WrFL(soft_reset_5);
		*/

		CVD_H14Ax_3DComb_Workaround(20); // enable  workaround function

		power_status = 1;
	}
	else	// power off
	{
		CVD_H14Ax_3DComb_Workaround(10); // Memory Reset & bypass workaround function

		if(power_status)
			OS_MsecSleep(5);

		//121210 : cvd soft reset during ADC Power down condition
		CVD_H14Ax_RdFL(iris_012);
		CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 1);
		CVD_H14Ax_WrFL(iris_012);

		CTOP_CTRL_H14A0_RdFL(ctr25);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs54, 0x1);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cvbs27, 0x1);
		CTOP_CTRL_H14A0_WrFL(ctr25);

		if(power_status)
			OS_MsecSleep(20);

#ifdef H13_IRIS_CLAMPAGC_V2_CONTROL_WA_FOR_CVD_UNLOCK
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_clampagc_v2, 1);
		CVD_H14Ax_WrFL(iris_175);
#endif
#if 0 // won.hur
		//L9A_DIE TX to L9D_DIE off
		ACE_REG_H14A0_RdFL(hslvdstx2_0);
		ACE_REG_H14A0_RdFL(hslvdstx2_1);
		ACE_REG_H14A0_Wr01(hslvdstx2_0, pdb3, 0);
		ACE_REG_H14A0_Wr01(hslvdstx2_1, ch_en3, 0);
		ACE_REG_H14A0_WrFL(hslvdstx2_0);
		ACE_REG_H14A0_WrFL(hslvdstx2_1);
#endif // won.hur

		// Power down
		/*
		   ACE_REG_H14A0_RdFL(soft_reset_5);
		   ACE_REG_H14A0_Wr01(soft_reset_5, swrst_f54m, 0x1);
		   ACE_REG_H14A0_Wr01(soft_reset_5, swrst_cvbs, 0x1);
		   ACE_REG_H14A0_WrFL(soft_reset_5);
		 */

		ACE_REG_H14A0_RdFL(afe_cvbs_3);

		//Do Not Turn Off CVBS_PDBM, to enable clock  for DENC
		//gafe_cvbs_1.cvbs_pdbm = 0;
		ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_pdb, 0);

		//added 110622 : by tommy.lee to disable clamp control on power down condition
		ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_cp, 0);
		ACE_REG_H14A0_WrFL(afe_cvbs_3);

#ifdef H13_COMB2D_ONLY_CONTROL
		//120705 : Enable comb2d_only mode when CVBS ADC is in power down mode.
		CVD_DEBUG("### Comb2d only [1] [%s][%d] ###\n",__func__, __LINE__ );
		CVD_H14Ax_Set_comb2d_only(1);
		CVD_H14Ax_Set_comb2d_only_md(1);
#endif
		if(power_status)
			OS_MsecSleep(5);

		power_status = 0;
	}
	return 0;
}

int CVD_H14Ax_Reset_Clampagc(void)
{
	//ADC_DEBUG("Reset Clampagc Entered\n");
	CVD_H14Ax_RdFL(iris_175);
	CVD_H14Ax_Wr01(iris_175, iris_clampagc_v2, 0x1);
	CVD_H14Ax_WrFL(iris_175);
	//by dws : remove mdelay
	//mdelay(5);
	OS_MsecSleep(5);
	CVD_H14Ax_RdFL(iris_175);
	CVD_H14Ax_Wr01(iris_175, iris_clampagc_v2, 0x0);
	CVD_H14Ax_WrFL(iris_175);
	return 0;
}

int CVD_H14Ax_Bypass_Control(LX_AFE_CVD_BYPASS_CONTROL_T *cvd_bypass_control_t)
{
	ACE_REG_H14A0_RdFL(afe_vbuf_1);
	ACE_REG_H14A0_RdFL(afe_vbuf_0);
	ACE_REG_H14A0_RdFL(afe_vbuf_2);
	ACE_REG_H14A0_RdFL(afe_vbuf_3);

	CVD_DEBUG("%s entered :  buf1[%d] buf2[%d] src[%d]\n",__func__, cvd_bypass_control_t->buf_out_1_sel, cvd_bypass_control_t->buf_out_2_sel, cvd_bypass_control_t->cvbs_source_sel);

	switch(cvd_bypass_control_t->buf_out_1_sel)
	{
		case CVD_BYPASS_DAC:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x4);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x0);
			break;

		case CVD_BYPASS_CVBS_WITH_CLAMPING:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x5);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x1);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x1);

			ACE_REG_H14A0_RdFL(afe_cvbs_3);
			ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_cp, 0);
			ACE_REG_H14A0_WrFL(afe_cvbs_3);
			break;

		case CVD_BYPASS_CVBS_WITH_CLAMPING_AV:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x5);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x1);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x1);

			ACE_REG_H14A0_RdFL(afe_cvbs_3);
			ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_cp, 1);
			ACE_REG_H14A0_WrFL(afe_cvbs_3);
			break;

		case CVD_BYPASS_CVBS_WITHOUT_CLAMPING:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x7);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x1);

			ACE_REG_H14A0_RdFL(afe_cvbs_3);
			ACE_REG_H14A0_Wr01(afe_cvbs_3, cvbs_cp, 1);
			ACE_REG_H14A0_WrFL(afe_cvbs_3);
			break;

		case CVD_BYPASS_CVBS_BUF_CLAMPING_ON:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x5);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x1);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x1);
			break;

		case CVD_BYPASS_CVBS_BUF_CLAMPING_OFF:
//			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x7);
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel1, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp2, 0x3);
			ACE_REG_H14A0_Wr01(afe_vbuf_2, buf_clp1, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_lpf, 0x0);
			ACE_REG_H14A0_Wr01(afe_vbuf_3, bufclp_vref, 0x1);
			break;

		default:
			break;
	}

	switch(cvd_bypass_control_t->buf_out_2_sel)
	{
		case CVD_BYPASS_DAC:
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel2, 0x0);
			break;

		case CVD_BYPASS_CVBS_WITH_CLAMPING:
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel2, 0x3);
			break;

		case CVD_BYPASS_CVBS_WITHOUT_CLAMPING:
			ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_sel2, 0x1);
			break;

		default:
			break;
	}

	if(cvd_bypass_control_t->cvbs_source_sel == CVD_BYPASS_CVBS_IN1)
		ACE_REG_H14A0_Wr01(afe_vbuf_0, buf_ycm, 0x1);
	else
		ACE_REG_H14A0_Wr01(afe_vbuf_0, buf_ycm, 0x0);

	ACE_REG_H14A0_WrFL(afe_vbuf_3);
	ACE_REG_H14A0_WrFL(afe_vbuf_2);
	ACE_REG_H14A0_WrFL(afe_vbuf_1);
	ACE_REG_H14A0_WrFL(afe_vbuf_0);

	return 0;
}

int CVD_H14Ax_Get_Vline_625_Reg(void)
{
	int ret;
	CVD_H14Ax_RdFL(iris_013);
	CVD_H14Ax_Rd01(iris_013, reg_vline_625, ret);
	return ret;
}

int CVD_H14Ax_OnOff_VDAC(BOOLEAN bonoff)
{

	CVD_DEBUG("%s entered :  %d \n",__func__, bonoff);

	if(bonoff)	// power on
	{
		CTOP_CTRL_H14A0_RdFL(ctr25);
		ACE_REG_H14A0_RdFL(afe_vbuf_1);
		ACE_REG_H14A0_RdFL(afe_vdac_0);

		ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 0x1);
		ACE_REG_H14A0_Wr01(afe_vdac_0, vdac_pdb, 0x1);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cve54, 0x0);	//De-Assert Reset

		ACE_REG_H14A0_WrFL(afe_vdac_0);
		ACE_REG_H14A0_WrFL(afe_vbuf_1);
		CTOP_CTRL_H14A0_WrFL(ctr25);

		/*
		CVD_H14Ax_VDAC_mute_control(1);
		OS_MsecSleep(5);
		CVD_H14Ax_VDAC_mute_control(0);
		*/
	}
	else	// power off
	{
		CTOP_CTRL_H14A0_RdFL(ctr25);
		ACE_REG_H14A0_RdFL(afe_vbuf_1);
		ACE_REG_H14A0_RdFL(afe_vdac_0);

		ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 0x0);
		ACE_REG_H14A0_Wr01(afe_vdac_0, vdac_pdb, 0x0);
		CTOP_CTRL_H14A0_Wr01(ctr25, reg_swrst_cve54, 0x1);	//Assert Reset

		ACE_REG_H14A0_WrFL(afe_vdac_0);
		ACE_REG_H14A0_WrFL(afe_vbuf_1);
		CTOP_CTRL_H14A0_WrFL(ctr25);

	//	CVD_H14Ax_VDAC_mute_control(1);
	}

	return 0;
}

int CVD_H14Ax_VDAC_mute_control(int enable)
{

	AFE_TRACE("%s entered :  %d \n",__func__, enable);

	if(enable)	//mute enable
	{
		ACE_REG_H14A0_RdFL(afe_vbuf_1);
		ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 0x0);
		ACE_REG_H14A0_WrFL(afe_vbuf_1);

		ACE_REG_H14A0_RdFL(soft_reset_2);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_vdac, 0x1);
		ACE_REG_H14A0_WrFL(soft_reset_2);
	}
	else		//mute disable
	{
		ACE_REG_H14A0_RdFL(afe_vbuf_1);
		ACE_REG_H14A0_Wr01(afe_vbuf_1, buf_pdb1, 0x1);
		ACE_REG_H14A0_WrFL(afe_vbuf_1);

		ACE_REG_H14A0_RdFL(soft_reset_2);
		ACE_REG_H14A0_Wr01(soft_reset_2, swrst_vdac, 0x0);
		ACE_REG_H14A0_WrFL(soft_reset_2);
	}

	return 0;
}

int CVD_H14Ax_Set_PE_Params(CVD_REG_PARAM_T	*pCVD_PE_Param_t, UINT32 size, LX_AFE_CVD_PQ_MODE_T cvd_pq_mode)
{
	int count;
	UINT32 value;
	UINT32 recvAddr;

	CVD_DEBUG("%s \n", __func__);

	for(count=0; count < size;count ++)
	{

		recvAddr = pCVD_PE_Param_t->cvd_phy_addr;
		value = pCVD_PE_Param_t->cvd_pe_value;
		/*
		   if		(recvAddr <= 0x2000) recvAddr += DTVSOC_DE_BASE;
		   else if (recvAddr <= 0xffff) recvAddr += DTVSOC_VIRT_PL301_BASE;
		 */
		//120201 : for NTSC CAGC adjust
		// H13_BRINGUP
		if(recvAddr == 0x43e8)
			g_initial_c_coeff = value & 0xFFFF;
		/*
		if(recvAddr == 0x4718)
			value = 0;
			*/

		//For IRIS_049 : reg_fixed_cstate & reg_cstate
		if(recvAddr != 0x42C4 )
		{
			recvAddr = recvAddr + (UINT32)gpRealRegCVD_H14Ax - 0x4100;
			REG_WD(recvAddr, value);
		}

		//CVD_DEBUG("%s [0x%x:0x%x]\n", __func__, recvAddr, value);

		pCVD_PE_Param_t ++;

	}

	return 0;
}

int CVD_H14Ax_swrst_CS(CVD_SELECT_CDETECT_T cs_sel)
{
	if (cs_sel == CVD_SEL_CS_CS0) // select cs0;
	{
		CVD_H14Ax_RdFL(iris_180);
		CVD_H14Ax_Wr01(iris_180,  reg_cs_sw_rst, 1);
		CVD_H14Ax_WrFL(iris_180);

		OS_MsecSleep(5);

		CVD_H14Ax_RdFL(iris_180);
		CVD_H14Ax_Wr01(iris_180,  reg_cs_sw_rst, 0);
		CVD_H14Ax_WrFL(iris_180);
	}
	else if (cs_sel == CVD_SEL_CS_CS1) // select cs1;
	{
		CVD_H14Ax_RdFL(iris_191);
		CVD_H14Ax_Wr01(iris_191,  reg_cs1_sw_rst, 1);
		CVD_H14Ax_WrFL(iris_191);

		OS_MsecSleep(5);

		CVD_H14Ax_RdFL(iris_191);
		CVD_H14Ax_Wr01(iris_191,  reg_cs1_sw_rst, 0);
		CVD_H14Ax_WrFL(iris_191);
	}
	else
		return -1;

	return 0;
}

int CVD_H14Ax_Set_Hstate_Max(UINT32	hstate_max_value)
{

//	CVD_DEBUG("%s entered :  %d \n",__func__, hstate_max_value);

	if(hstate_max_value > 5)
		return -1;

	CVD_H14Ax_RdFL(iris_027);
	CVD_H14Ax_Wr01(iris_027,  reg_hstate_max, hstate_max_value);
	CVD_H14Ax_WrFL(iris_027);

	return 0;
}

int CVD_H14Ax_Set_Hstate_Fixed(UINT32	value)
{

//	CVD_DEBUG("%s entered :  %d \n",__func__, value);

	value &= 0x1;

	CVD_H14Ax_RdFL(iris_027);
	CVD_H14Ax_Wr01(iris_027,  reg_hstate_fixed, value);
	CVD_H14Ax_WrFL(iris_027);

	return 0;
}


int CVD_H14Ax_AGC_Bypass(int Bypass_Enable)
{

	AFE_TRACE("%s entered :  %d \n",__func__, Bypass_Enable);

	if(Bypass_Enable == 1) {

		CVD_H14Ax_RdFL(iris_098);
		CVD_H14Ax_Wr01(iris_098,  reg_agc_bypass, 0x1);
		CVD_H14Ax_WrFL(iris_098);
	}
	else {
		CVD_H14Ax_RdFL(iris_098);
		CVD_H14Ax_Wr01(iris_098,  reg_agc_bypass, 0x0);
		CVD_H14Ax_WrFL(iris_098);
	}

	return 0;
}

int CVD_H14Ax_Set_dcrestore_accum_width(int value)
{

	CVD_DEBUG("%s entered :  %d \n",__func__, value);

	CVD_H14Ax_RdFL(iris_074);
	CVD_H14Ax_Wr01(iris_074, reg_dcrestore_accum_width, value);
	CVD_H14Ax_WrFL(iris_074);

	return 0;
}

int CVD_H14Ax_Set_SCART_CSC(int black_level)
{
	CVD_DEBUG("%s entered : black_level[%d]\n",__func__, black_level);

	CVD_H14Ax_RdFL(fastblank_002);
	CVD_H14Ax_RdFL(fastblank_003);
	CVD_H14Ax_RdFL(fastblank_004);
	CVD_H14Ax_RdFL(fastblank_005);
	CVD_H14Ax_RdFL(fastblank_006);
	CVD_H14Ax_RdFL(fastblank_007);
	CVD_H14Ax_RdFL(fastblank_008);
	CVD_H14Ax_RdFL(fastblank_009);

#if 0
	CVD_H14Ax_Wr02(fastblank_002, reg_fb_csc_coef0, 0x0810, reg_fb_csc_coef1, 0x0191 );
	CVD_H14Ax_Wr02(fastblank_003, reg_fb_csc_coef2, 0x041D, reg_fb_csc_coef3, 0x7B58 );
	CVD_H14Ax_Wr02(fastblank_004, reg_fb_csc_coef4, 0x0706, reg_fb_csc_coef5, 0x7DA2 );
	CVD_H14Ax_Wr02(fastblank_005, reg_fb_csc_coef6, 0x7A1D, reg_fb_csc_coef7, 0x7EDD );
	CVD_H14Ax_Wr01(fastblank_006, reg_fb_csc_coef8, 0x0706 );
	CVD_H14Ax_Wr02(fastblank_009, reg_fb_csc_ofst4, 0x202, reg_fb_csc_ofst5, 0x204 );
#endif
#if 1		//Ultra Black Support
	if(black_level)	//High
	{
		CVD_H14Ax_Wr02(fastblank_002, reg_fb_csc_coef0, 0x0898, reg_fb_csc_coef1, 0x019B );
		CVD_H14Ax_Wr02(fastblank_003, reg_fb_csc_coef2, 0x0437, reg_fb_csc_coef3, 0x7B22 );
		CVD_H14Ax_Wr02(fastblank_004, reg_fb_csc_coef4, 0x0757, reg_fb_csc_coef5, 0x7D87 );
		CVD_H14Ax_Wr02(fastblank_005, reg_fb_csc_coef6, 0x79E8, reg_fb_csc_coef7, 0x7ED3 );
		CVD_H14Ax_Wr01(fastblank_006, reg_fb_csc_coef8, 0x0745 );
		CVD_H14Ax_Wr02(fastblank_007, reg_fb_csc_ofst0, 0x0, reg_fb_csc_ofst1, 0x0 );
		CVD_H14Ax_Wr02(fastblank_008, reg_fb_csc_ofst2, 0x0, reg_fb_csc_ofst3, 0x02D );
		CVD_H14Ax_Wr02(fastblank_009, reg_fb_csc_ofst4, 0x204, reg_fb_csc_ofst5, 0x204 );
	}
	else			//Low
	{
		CVD_H14Ax_Wr02(fastblank_002, reg_fb_csc_coef0, 0x0927, reg_fb_csc_coef1, 0x01DF );
		CVD_H14Ax_Wr02(fastblank_003, reg_fb_csc_coef2, 0x04E7, reg_fb_csc_coef3, 0x7A55 );
		CVD_H14Ax_Wr02(fastblank_004, reg_fb_csc_coef4, 0x088B, reg_fb_csc_coef5, 0x7D20 );
		CVD_H14Ax_Wr02(fastblank_005, reg_fb_csc_coef6, 0x78EA, reg_fb_csc_coef7, 0x7EA0 );
		CVD_H14Ax_Wr01(fastblank_006, reg_fb_csc_coef8, 0x0876 );
		CVD_H14Ax_Wr02(fastblank_007, reg_fb_csc_ofst0, 0x0, reg_fb_csc_ofst1, 0x0 );
		CVD_H14Ax_Wr02(fastblank_008, reg_fb_csc_ofst2, 0x0, reg_fb_csc_ofst3, 0x7E6 );
		CVD_H14Ax_Wr02(fastblank_009, reg_fb_csc_ofst4, 0x204, reg_fb_csc_ofst5, 0x204 );
	}
#else
if(black_level)	//High
	{
		CVD_H14Ax_Wr02(fastblank_002, reg_fb_csc_coef0, 0x0862, reg_fb_csc_coef1, 0x0191 );
		CVD_H14Ax_Wr02(fastblank_003, reg_fb_csc_coef2, 0x041D, reg_fb_csc_coef3, 0x7B58 );
		CVD_H14Ax_Wr02(fastblank_004, reg_fb_csc_coef4, 0x0706, reg_fb_csc_coef5, 0x7DA2 );
		CVD_H14Ax_Wr02(fastblank_005, reg_fb_csc_coef6, 0x7A1D, reg_fb_csc_coef7, 0x7EDD );
		CVD_H14Ax_Wr01(fastblank_006, reg_fb_csc_coef8, 0x0706 );
		CVD_H14Ax_Wr02(fastblank_007, reg_fb_csc_ofst0, 0x0, reg_fb_csc_ofst1, 0x0 );
		CVD_H14Ax_Wr02(fastblank_008, reg_fb_csc_ofst2, 0x0, reg_fb_csc_ofst3, 0x02C );
		CVD_H14Ax_Wr02(fastblank_009, reg_fb_csc_ofst4, 0x200, reg_fb_csc_ofst5, 0x200 );
	}
	else			//Low
	{
		CVD_H14Ax_Wr02(fastblank_002, reg_fb_csc_coef0, 0x08EE, reg_fb_csc_coef1, 0x01D3 );
		CVD_H14Ax_Wr02(fastblank_003, reg_fb_csc_coef2, 0x04C9, reg_fb_csc_coef3, 0x7A93 );
		CVD_H14Ax_Wr02(fastblank_004, reg_fb_csc_coef4, 0x082D, reg_fb_csc_coef5, 0x7D3F );
		CVD_H14Ax_Wr02(fastblank_005, reg_fb_csc_coef6, 0x7927, reg_fb_csc_coef7, 0x7EAC );
		CVD_H14Ax_Wr01(fastblank_006, reg_fb_csc_coef8, 0x082D );
		CVD_H14Ax_Wr02(fastblank_007, reg_fb_csc_ofst0, 0x0, reg_fb_csc_ofst1, 0x0 );
		CVD_H14Ax_Wr02(fastblank_008, reg_fb_csc_ofst2, 0x0, reg_fb_csc_ofst3, 0x7E6 );
		CVD_H14Ax_Wr02(fastblank_009, reg_fb_csc_ofst4, 0x200, reg_fb_csc_ofst5, 0x200 );
	}
#endif

	CVD_H14Ax_WrFL(fastblank_002);
	CVD_H14Ax_WrFL(fastblank_003);
	CVD_H14Ax_WrFL(fastblank_004);
	CVD_H14Ax_WrFL(fastblank_005);
	CVD_H14Ax_WrFL(fastblank_006);
	CVD_H14Ax_WrFL(fastblank_007);
	CVD_H14Ax_WrFL(fastblank_008);
	CVD_H14Ax_WrFL(fastblank_009);

	return 0;
}

int CVD_H14Ax_Set_comb2d_only(int value)
{

	AFE_TRACE("%s entered :  %d \n",__func__, value);
	value &= 0x1;

	CVD_H14Ax_RdFL(iris_064);
	CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, value);
	CVD_H14Ax_WrFL(iris_064);

	return 0;
}

int CVD_H14Ax_Set_comb2d_only_md(int value)
{

	AFE_TRACE("%s entered :  %d \n",__func__, value);
	value &= 0x1;

	//added 121217
	CVD_H14Ax_RdFL(iris_066);
	CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, value);
	CVD_H14Ax_WrFL(iris_066);

	return 0;
}

int CVD_H14Ax_Get_comb2d_only(void)
{
	int value;

	CVD_H14Ax_RdFL(iris_064);
	CVD_H14Ax_Rd01(iris_064, reg_comb2d_only, value);

	return value;
}

int CVD_H14Ax_Get_comb2d_only_md(void)
{
	int value;

	CVD_H14Ax_RdFL(iris_066);
	CVD_H14Ax_Rd01(iris_066, reg_comb2d_only_md, value);

	return value;
}

int CVD_H14Ax_lbadrgen_rst_control(int value)
{

	AFE_TRACE("%s entered :  %d \n",__func__, value);
	value &= 0x1;

	CVD_H14Ax_RdFL(iris_064);
	CVD_H14Ax_Wr01(iris_064, reg_lbadrgen_rst, value);
	CVD_H14Ax_WrFL(iris_064);

	return 0;
}

int CVD_H14Ax_Set_Noise_Threshold(int value)
{

	AFE_TRACE("%s entered :  %d \n",__func__, value);

	CVD_H14Ax_RdFL(iris_017);
	CVD_H14Ax_Wr01(iris_017, reg_noise_thresh, value);	// default 0x32
	CVD_H14Ax_WrFL(iris_017);

	return 0;
}

int CVD_H14Ax_Set_AGC_Peak_Nominal(UINT8	value)	// 7bit value
{
	value &= 0x7F;

	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_nominal, value);	//Default : 0x0A
	CVD_H14Ax_WrFL(iris_025);

	return 0;
}

int CVD_H14Ax_Get_AGC_Peak_Nominal(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Rd01(iris_025, reg_agc_peak_nominal, ret);	//Default : 0x0A

	return ret;
}

int CVD_H14Ax_Get_PE0_Motion_Value(int *p_tnr_x_avg_t, int *p_tnr_x_avg_s, int *p_tpd_s_status)
{
	PE_P0L_H14_RdFL(p0l_tnr_status_00);
	PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
	//############### H14_BRINGUP wonsik.do #################################
//	PE_P0L_H14_Rd01(p0l_tnr_status_00, x_avg_t, *p_tnr_x_avg_t);
//	PE_P0L_H14_Rd01(p0l_tnr_status_00, x_avg_s, *p_tnr_x_avg_s);
	*p_tnr_x_avg_t = 0;
	*p_tnr_x_avg_s = 0;
	//############### H14_BRINGUP wonsik.do #################################
	PE_P1L_H14_Rd01(pe1_dse_ctrl_01, reg_s_status_0, *p_tpd_s_status);

	return 0;
}

int CVD_H14Ax_Set_Contrast_Brightness(int contrast, int brightness)
{
	CVD_H14Ax_RdFL(iris_021);
	CVD_H14Ax_Wr01(iris_021, reg_contrast, contrast);
	CVD_H14Ax_Wr01(iris_021, reg_brightness, brightness);
	CVD_H14Ax_WrFL(iris_021);

	return 0;
}

int CVD_H14Ax_Set_Dcrestore_Gain(int value)
{
	value &= 0x3;

	CVD_H14Ax_RdFL(iris_074);
	// Inital Value is '0'
	// Set reg_dcrestore_gain to '3' on weak RF signal for sync stability
	CVD_H14Ax_Wr01(iris_074, reg_dcrestore_gain, value);
	CVD_H14Ax_WrFL(iris_074);

	return 0;
}

//set reg_agc_bypass & reg_dcrestore_gain to default value
void CVD_H14Ax_Set_for_Normal_Signal(void)
{
	CVD_DEBUG("%s entered \n",__func__);
	CVD_H14Ax_AGC_Bypass(0x0);
//	CVD_H14Ax_Set_Dcrestore_Gain(0x0);
}

//set AGC to Bypass, and set DCrestore gain to 1/8 on weak RF signal ( status nois is max value 0x3FF)
void CVD_H14Ax_Set_for_Noisy_Signal(void)
{
	CVD_DEBUG("%s entered \n",__func__);
	CVD_H14Ax_AGC_Bypass(0x1);
//	CVD_H14Ax_Set_Dcrestore_Gain(0x3);
}

int CVD_H14Ax_Get_Status_AGC_Gain(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_007);
	CVD_H14Ax_Rd01(iris_007, reg_status_agc_gain, ret);

	return ret;
}

int CVD_H14Ax_Set_AGC_Peak_En(int enable)
{
	enable &= 0x1;

//	CVD_DEBUG("%s entered :  %d \n",__func__, enable);

	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Wr01(iris_025, reg_agc_peak_en, enable);
	CVD_H14Ax_WrFL(iris_025);

	return 0;

}

int CVD_H14Ax_Get_HNon_Standard_Flag(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_hnon_standard, ret);

	return ret;
}

int CVD_H14Ax_Get_VNon_Standard_Flag(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_002);
	CVD_H14Ax_Rd01(iris_002, reg_vnon_standard, ret);

	return ret;
}

int CVD_H14Ax_Set_Noburst_Ckill(unsigned int value)
{
	value &= 0x1;

//	CVD_DEBUG("%s entered :  %d \n",__func__, value);

	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Wr01(iris_024, reg_noburst_ckill, value);
	CVD_H14Ax_WrFL(iris_024);

	return 0;

}

int CVD_H14Ax_Get_Global_Motion_Value(void)
{
	int ret;

	unsigned int value;

	CVD_H14Ax_RdFL(iris_266);
	CVD_H14Ax_Rd01(iris_266, reg_ycsep_3d_status0, value);

	ret = value & 0xFF;

	return ret;
}

/* This Function is workaround code for JOJO Gunpo stream (vsync unstable)*/
int CVD_H14Ax_Set_for_Stable_Vsync(UINT32	Enable)
{
//	CVD_DEBUG("%s entered :  %d \n",__func__, Enable);

	if(Enable == 1) {
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 0x2);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl_noisy, 0x1);
		CVD_H14Ax_WrFL(iris_042);

		CVD_H14Ax_RdFL(iris_043);
		CVD_H14Ax_Wr01(iris_043,  reg_vloop_tc, 0x3);
		CVD_H14Ax_WrFL(iris_043);
	}
	else if(Enable == 2)
	{
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 0x3);
		CVD_H14Ax_WrFL(iris_042);
	}
	else if(Enable == 3)
	{
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 0x0);
		CVD_H14Ax_WrFL(iris_042);

	}
	else {
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 0x1);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl_noisy, 0x0);
		CVD_H14Ax_WrFL(iris_042);

		CVD_H14Ax_RdFL(iris_043);
		CVD_H14Ax_Wr01(iris_043,  reg_vloop_tc, 0x2);
		CVD_H14Ax_WrFL(iris_043);
	}
	return 0;
}

/* This Function is used for workaround code as to fix dong-go-dong-rak */
int CVD_H14Ax_Set_for_Field_Detect_Mode(UINT32 mode)
{

	CVD_H14Ax_RdFL(iris_043);

	if(mode == 3) 		CVD_H14Ax_Wr01(iris_043,  reg_field_detect_mode, 0x3);
	else if(mode == 2)  CVD_H14Ax_Wr01(iris_043,  reg_field_detect_mode, 0x2);
	else if(mode == 1)  CVD_H14Ax_Wr01(iris_043,  reg_field_detect_mode, 0x1);
	else if(mode == 0)  CVD_H14Ax_Wr01(iris_043,  reg_field_detect_mode, 0x0);
	else CVD_H14Ax_Wr01(iris_043,  reg_field_detect_mode, 0x2);	// Go to default value

	CVD_H14Ax_WrFL(iris_043);

	return 0;
}

/* This function is used for workaround code for brasil color instable issue */
int CVD_H14Ax_Set_for_Burst_Gate_End_On_Noisy(UINT8 original_value, UINT32 Enable)
{
	if(Enable == 0)
	{
		CVD_H14Ax_RdFL(iris_035);
		CVD_H14Ax_Wr01(iris_035,  reg_burst_gate_end, original_value);
		CVD_H14Ax_WrFL(iris_035);
		//AFE_PRINT("BURST GATE END 0x51\n");
	}
	else
	{
		CVD_H14Ax_RdFL(iris_035);
		CVD_H14Ax_Wr01(iris_035,  reg_burst_gate_end, 0x61);
		CVD_H14Ax_WrFL(iris_035);
		//AFE_PRINT("BURST GATE END 0x61\n");
	}

	return 0;
}


/* This function is used for workaround code for brasil color instable issue */
UINT32 CVD_H14Ax_Differential_Status_Cdto_Inc_Value(void)
{
	int ret = 0;
	UINT32 Current_Inc_Value = 0;
	static UINT32 Prev_Inc_Value = STANDARD_CDTO_INC_VALUE;
	UINT32 Difference_Value = 0;


	CVD_H14Ax_RdFL(iris_006);
	CVD_H14Ax_Rd01(iris_006, reg_status_cdto_inc, Current_Inc_Value);

	Difference_Value = abs(Current_Inc_Value - Prev_Inc_Value);
	Prev_Inc_Value = Current_Inc_Value;

	//AFE_PRINT("CVD_CDTO : Variance[%d]\n", Difference_Value);

	ret = Difference_Value;
	return ret;
}


UINT32 CVD_H14Ax_Read_Cordic_Freq_Value(void)
{
	int ret = 0;
	UINT8 Cordic_Value = 0;


	CVD_H14Ax_RdFL(iris_008);
	CVD_H14Ax_Rd01(iris_008, reg_status_cordic_freq, Cordic_Value);

	Cordic_Value = (UINT8)((SINT8)Cordic_Value + 0x80);

	ret = Cordic_Value;
	return ret;
}


int	CVD_H14Ax_Read_VCR_Detected(void)
{
	int ret = 0;
	UINT32 VCR_Detected_Flag = 0;

	CVD_H14Ax_RdFL(iris_003);
	CVD_H14Ax_Rd01(iris_003, reg_vcr, VCR_Detected_Flag);

	if(VCR_Detected_Flag) ret = 1;
	else ret = 0;

	return ret;
}

int CVD_H14Ax_Get_CVD_Burst_Mag_Value(void)
{
	UINT16 burst_mag_status;
	CVD_H14Ax_RdFL(iris_004);
	CVD_H14Ax_Rd01(iris_004,reg_status_burst_mag,burst_mag_status);

	return (int)burst_mag_status;
}

int CVD_H14Ax_Set_CVD_Saturation_Value(int value)
{
	value &= 0xFF;

	CVD_H14Ax_RdFL(iris_021);
	CVD_H14Ax_Wr01(iris_021, reg_saturation, value);
	CVD_H14Ax_WrFL(iris_021);

	return 0;
}

int CVD_H14Ax_Get_CAGC_Value(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_022);
	CVD_H14Ax_Rd01(iris_022, reg_cagc, ret);

	return ret;
}

int CVD_H14Ax_Get_Saturation_Value(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_021);
	CVD_H14Ax_Rd01(iris_021, reg_saturation, ret);

	return ret;
}

int CVD_H14Ax_Get_AGC_Peak_En_Value(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_025);
	CVD_H14Ax_Rd01(iris_025, reg_agc_peak_en, ret);

	return ret;
}

int CVD_H14Ax_Get_AGC_Bypass_Value(void)
{
	int ret;

	CVD_H14Ax_RdFL(iris_098);
	CVD_H14Ax_Rd01(iris_098,  reg_agc_bypass, ret);

	return ret;
}
int CVD_H14Ax_Get_Vdetect_Vcount_Value(void)
{
	int vcount;
	CVD_H14Ax_RdFL(iris_274);
	CVD_H14Ax_Rd01(iris_274, reg_status_vdetect_vcount, vcount);

	return vcount;
}

int CVD_H14Ax_Get_oadj_c_coeff_value(void)
{
	int oadj_c_coeff_value;
	CVD_H14Ax_RdFL(iris_122);
	CVD_H14Ax_Rd01(iris_122, reg_oadj_c_coeff, oadj_c_coeff_value);

	return oadj_c_coeff_value;
}

int CVD_H14Ax_OnOff_Chromalock_Ckill(BOOLEAN bonoff)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, bonoff);
	CVD_H14Ax_RdFL(iris_047);

	if(bonoff)
		CVD_H14Ax_Wr01(iris_047, reg_lose_chromalock_ckill, 0x1);
	else
		CVD_H14Ax_Wr01(iris_047, reg_lose_chromalock_ckill, 0x0);

	CVD_H14Ax_WrFL(iris_047);
	return 0;
}

int CVD_H14Ax_Reset_mif(int enable)
{
	if(enable >0)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 1);
		CVD_H14Ax_WrFL(iris_175);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0);
		CVD_H14Ax_WrFL(iris_175);
	}
	return 0;
}

int CVD_H14Ax_Set_HNon_Standard_Threshold(int value)
{
	int threshold;
	CVD_H14Ax_RdFL(iris_024);
	CVD_H14Ax_Rd01(iris_024, reg_hnon_std_threshold, threshold);
	if(threshold != value) {
		CVD_DEBUG("HNon_Std_Threshold to [0x%x]\n", value);
		CVD_H14Ax_Wr01(iris_024, reg_hnon_std_threshold, value);
		CVD_H14Ax_WrFL(iris_024);
	}

	return 0;
}

int CVD_H14Ax_Get_Crunky_Status(LX_AFE_CVD_CK_T *pCK_Detection_t)
{
	if(CVD_H14Ax_Get_HLock_Flag() && CVD_H14Ax_Get_VLock_Flag())
	{
		CVD_H14Ax_RdFL(iris_001);
		CVD_H14Ax_Rd02(iris_001, reg_mv_vbi_detected, pCK_Detection_t->ck_vbi_detected , reg_mv_colourstripes, pCK_Detection_t->ck_colorstrip_detected);
	}
	else
	{
		pCK_Detection_t->ck_vbi_detected = 0;
		pCK_Detection_t->ck_colorstrip_detected = 0;
	}

	return 0;
}

/* This function is used for workaround code for no burst signal jitter issue */
int CVD_H14Ax_Burst_Gate_Control(UINT32	gate_start, UINT32 gate_end)
{
	CVD_DEBUG("CVD Burst Gate Control : Start[0x%x], End[0x%x]\n", gate_start, gate_end);

	CVD_H14Ax_RdFL(iris_035);
	CVD_H14Ax_Wr02(iris_035, reg_burst_gate_start, gate_start, reg_burst_gate_end, gate_end);
	CVD_H14Ax_WrFL(iris_035);

	return 0;
}

int CVD_H14Ax_Read_Buffer_Status(UINT32 *rbuf1_empty, UINT32 *rbuf2_empty, UINT32 *rbuf3_empty, UINT32 *rbuf4_empty, UINT32 *wbuf_empty, UINT32 *wbuf_ful)
{
	CVD_H14Ax_RdFL(iris_mif_gmua_mon_001);
	CVD_H14Ax_RdFL(iris_mif_gmua_mon_002);
	CVD_H14Ax_RdFL(iris_mif_gmua_mon_003);
	CVD_H14Ax_Rd02(iris_mif_gmua_mon_001, ro_rbuf1_empty, *rbuf1_empty , ro_rbuf2_empty, *rbuf2_empty);
	CVD_H14Ax_Rd02(iris_mif_gmua_mon_002, ro_rbuf3_empty, *rbuf3_empty , ro_rbuf4_empty, *rbuf4_empty);
	CVD_H14Ax_Rd02(iris_mif_gmua_mon_003, ro_wbuf_full, *wbuf_ful, ro_wbuf_empty, *wbuf_empty );

	return 0;
}

int CVD_H14Ax_vf_nstd_control(unsigned int value)
{
	CVD_H14Ax_RdFL(iris_067);
	CVD_H14Ax_Wr01(iris_067, reg_vf_nstd_en, value);	// default 0x1
	CVD_H14Ax_WrFL(iris_067);

	return 0;
}

int CVD_H14Ax_Set_CAGC_Value(UINT32 cagc_value)
{
	if(cagc_value > 0xff)
		return -1;

	CVD_H14Ax_RdFL(iris_022);
	CVD_H14Ax_Wr01(iris_022, reg_cagc, cagc_value);
	CVD_H14Ax_WrFL(iris_022);

	return 0;
}

int CVD_H14Ax_Enable_Stable_Sync_Mode(int enable)
{
	int lpf_on;

	CVD_DEBUG("%s entered :  %d \n",__func__, enable);

	if(enable == 3)	// for NTSC RF Signal (strong hsync filter)
	{
		//0x4774
		// falling/rising_phase
		CVD_H14Ax_RdFL(iris_347);
		//CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018102);
		CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018002);
		CVD_H14Ax_WrFL(iris_347);
		//0x4798
		CVD_H14Ax_RdFL(iris_356);
		CVD_H14Ax_Wr01(iris_356, reg_hdetect_nr_ctrl4, 0x09861a86);
		CVD_H14Ax_WrFL(iris_356);
		//0x4778
		// eq_phase
		CVD_H14Ax_RdFL(iris_348);
		//CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018102);
		CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018002);
		CVD_H14Ax_WrFL(iris_348);
		//0x479c
		CVD_H14Ax_RdFL(iris_357);
		CVD_H14Ax_Wr01(iris_357, reg_hdetect_nr_ctrl5, 0x00001a86);
		CVD_H14Ax_WrFL(iris_357);
		//0x477c
		// fine_phase
		CVD_H14Ax_RdFL(iris_349);
		//CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018102);
		CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018042);
		CVD_H14Ax_WrFL(iris_349);
		//0x47a0
		CVD_H14Ax_RdFL(iris_358);
		CVD_H14Ax_Wr01(iris_358, reg_hdetect_nr_ctrl6, 0x00001a86);
		CVD_H14Ax_WrFL(iris_358);
		//0x4780
		// phase_diff
		CVD_H14Ax_RdFL(iris_350);
		//CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018102);
		CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018002);
		CVD_H14Ax_WrFL(iris_350);
		//0x47a4
		CVD_H14Ax_RdFL(iris_359);
		CVD_H14Ax_Wr01(iris_359, reg_hdetect_nr_ctrl7, 0x40001a86);
		CVD_H14Ax_WrFL(iris_359);
		//reg_clampagc_syncs_lpf_on '1'
		CVD_H14Ax_RdFL(iris_076);
		CVD_H14Ax_Rd01(iris_076, reg_dcrestore_lpf_en, lpf_on);
		CVD_H14Ax_Wr01(iris_076, reg_dcrestore_lpf_en, lpf_on | 0x02);
		CVD_H14Ax_WrFL(iris_076);

	}
	else if(enable == 2)	// for NTSC RF Signal 
	{
		//0x4774
		// falling/rising_phase
		CVD_H14Ax_RdFL(iris_347);
		//CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018102);
		CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018000);
		CVD_H14Ax_WrFL(iris_347);
		//0x4798
		CVD_H14Ax_RdFL(iris_356);
		CVD_H14Ax_Wr01(iris_356, reg_hdetect_nr_ctrl4, 0x09861a86);
		CVD_H14Ax_WrFL(iris_356);
		//0x4778
		// eq_phase
		CVD_H14Ax_RdFL(iris_348);
		//CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018102);
		CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018002);
		CVD_H14Ax_WrFL(iris_348);
		//0x479c
		CVD_H14Ax_RdFL(iris_357);
		CVD_H14Ax_Wr01(iris_357, reg_hdetect_nr_ctrl5, 0x00001a86);
		CVD_H14Ax_WrFL(iris_357);
		//0x477c
		// fine_phase
		CVD_H14Ax_RdFL(iris_349);
		//CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018102);
		CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018080);
		CVD_H14Ax_WrFL(iris_349);
		//0x47a0
		CVD_H14Ax_RdFL(iris_358);
		CVD_H14Ax_Wr01(iris_358, reg_hdetect_nr_ctrl6, 0x00001a86);
		CVD_H14Ax_WrFL(iris_358);
		//0x4780
		// phase_diff
		CVD_H14Ax_RdFL(iris_350);
		//CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018102);
		CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018082);
		CVD_H14Ax_WrFL(iris_350);
		//0x47a4
		CVD_H14Ax_RdFL(iris_359);
		CVD_H14Ax_Wr01(iris_359, reg_hdetect_nr_ctrl7, 0x40001a86);
		CVD_H14Ax_WrFL(iris_359);
		//reg_clampagc_syncs_lpf_on '1'
		CVD_H14Ax_RdFL(iris_076);
		CVD_H14Ax_Rd01(iris_076, reg_dcrestore_lpf_en, lpf_on);
		CVD_H14Ax_Wr01(iris_076, reg_dcrestore_lpf_en, lpf_on | 0x02);
		CVD_H14Ax_WrFL(iris_076);

	}
	else if(enable == 1)	// for RF signal but not locked, enabled dcrestore_lpf_en.
	{
		//0x4774
		CVD_H14Ax_RdFL(iris_347);
		CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018000);
		CVD_H14Ax_WrFL(iris_347);
		//0x4798
		CVD_H14Ax_RdFL(iris_356);
		CVD_H14Ax_Wr01(iris_356, reg_hdetect_nr_ctrl4, 0x09861a86);
		CVD_H14Ax_WrFL(iris_356);
		//0x4778
		CVD_H14Ax_RdFL(iris_348);
		CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018000);
		CVD_H14Ax_WrFL(iris_348);
		//0x479c
		CVD_H14Ax_RdFL(iris_357);
		CVD_H14Ax_Wr01(iris_357, reg_hdetect_nr_ctrl5, 0x00001a86);
		CVD_H14Ax_WrFL(iris_357);
		//0x477c
		CVD_H14Ax_RdFL(iris_349);
		CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018080);
		CVD_H14Ax_WrFL(iris_349);
		//0x47a0
		CVD_H14Ax_RdFL(iris_358);
		CVD_H14Ax_Wr01(iris_358, reg_hdetect_nr_ctrl6, 0x00001a86);
		CVD_H14Ax_WrFL(iris_358);
		//0x4780
		CVD_H14Ax_RdFL(iris_350);
		CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018080);
		CVD_H14Ax_WrFL(iris_350);
		//0x47a4
		CVD_H14Ax_RdFL(iris_359);
		CVD_H14Ax_Wr01(iris_359, reg_hdetect_nr_ctrl7, 0x40001a86);
		CVD_H14Ax_WrFL(iris_359);
		//reg_clampagc_syncs_lpf_on '0'
		CVD_H14Ax_RdFL(iris_076);
		CVD_H14Ax_Rd01(iris_076, reg_dcrestore_lpf_en, lpf_on);
		//CVD_H14Ax_Wr01(iris_076, reg_dcrestore_lpf_en, lpf_on & 0x1);
		CVD_H14Ax_Wr01(iris_076, reg_dcrestore_lpf_en, lpf_on | 0x02);
		CVD_H14Ax_WrFL(iris_076);
	}
	else // for Auto Programming and AV input mode, restoring default register values
	{
		//0x4774
		CVD_H14Ax_RdFL(iris_347);
		CVD_H14Ax_Wr01(iris_347, reg_hdetect_nr_ctrl0, 0x28018100);
		CVD_H14Ax_WrFL(iris_347);
		//0x4798
		CVD_H14Ax_RdFL(iris_356);
		CVD_H14Ax_Wr01(iris_356, reg_hdetect_nr_ctrl4, 0x09861a86);
		CVD_H14Ax_WrFL(iris_356);
		//0x4778
		CVD_H14Ax_RdFL(iris_348);
		CVD_H14Ax_Wr01(iris_348, reg_hdetect_nr_ctrl1, 0x28018100);
		CVD_H14Ax_WrFL(iris_348);
		//0x479c
		CVD_H14Ax_RdFL(iris_357);
		CVD_H14Ax_Wr01(iris_357, reg_hdetect_nr_ctrl5, 0x00001a86);
		CVD_H14Ax_WrFL(iris_357);
		//0x477c
		CVD_H14Ax_RdFL(iris_349);
		CVD_H14Ax_Wr01(iris_349, reg_hdetect_nr_ctrl2, 0x28018100);
		CVD_H14Ax_WrFL(iris_349);
		//0x47a0
		CVD_H14Ax_RdFL(iris_358);
		CVD_H14Ax_Wr01(iris_358, reg_hdetect_nr_ctrl6, 0x00001a86);
		CVD_H14Ax_WrFL(iris_358);
		//0x4780
		CVD_H14Ax_RdFL(iris_350);
		CVD_H14Ax_Wr01(iris_350, reg_hdetect_nr_ctrl3, 0x28018100);
		CVD_H14Ax_WrFL(iris_350);
		//0x47a4
		CVD_H14Ax_RdFL(iris_359);
		CVD_H14Ax_Wr01(iris_359, reg_hdetect_nr_ctrl7, 0x40001a86);
		CVD_H14Ax_WrFL(iris_359);
		//reg_clampagc_syncs_lpf_on '0'
		CVD_H14Ax_RdFL(iris_076);
		CVD_H14Ax_Rd01(iris_076, reg_dcrestore_lpf_en, lpf_on);
		CVD_H14Ax_Wr01(iris_076, reg_dcrestore_lpf_en, lpf_on & 0x1);
		CVD_H14Ax_WrFL(iris_076);
	}
	return 0;

}

int CVD_H14Ax_Hstart_Position_Compensator(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	int lpf_on;

	CVD_H14Ax_RdFL(iris_076);
	CVD_H14Ax_Rd01(iris_076, reg_dcrestore_lpf_en, lpf_on);

	if(lpf_on & 0x02)
	{
		CVD_H14Ax_RdFL(iris_263);
		CVD_H14Ax_Wr01(iris_263, reg_hrs_ha_start, pSet_system_t->reg_rf_hrs_ha_start);
		CVD_H14Ax_WrFL(iris_263);
	}

	return 0;
}

int CVD_H14Ax_Init_Level_Detector(CVD_STATE_T color_system)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, color_system);

	if(color_system == CVD_STATE_NTSC) {
		CVD_H14Ax_Wr01(lvl_001, reg_ycsep_lvl_ctrl0, 0x3);	// [1]:measure block height (0=64, 1=128), [0] : measure block width (0=8*4, 1=16*4)
		CVD_H14Ax_WrFL(lvl_001);
		CVD_H14Ax_Wr01(lvl_002, reg_ycsep_lvl_ctrl1, 0x04203b56);	// [0][1][2][3] hstart point
		CVD_H14Ax_WrFL(lvl_002);
		CVD_H14Ax_Wr01(lvl_003, reg_ycsep_lvl_ctrl2, 0x728da8c3);	// [4][5][6][7] hstart point
		CVD_H14Ax_WrFL(lvl_003);
		CVD_H14Ax_Wr01(lvl_006, reg_ycsep_lvl_ctrl5, 0x04203b56);	// [8][9][10][11] hstart point
		CVD_H14Ax_WrFL(lvl_006);
		CVD_H14Ax_Wr01(lvl_007, reg_ycsep_lvl_ctrl6, 0x728da8c3);	// [12][13][14][15] hstart point
		CVD_H14Ax_WrFL(lvl_007);
	}
	else if(color_system == CVD_STATE_PALi) {
		CVD_H14Ax_Wr01(lvl_001, reg_ycsep_lvl_ctrl0, 0x3);	// [1]:measure block height (0=64, 1=128), [0] : measure block width (0=8*4, 1=16*4)
		CVD_H14Ax_WrFL(lvl_001);
		CVD_H14Ax_Wr01(lvl_002, reg_ycsep_lvl_ctrl1, 0x0624425f);	// [0][1][2][3] hstart point
		CVD_H14Ax_WrFL(lvl_002);
		CVD_H14Ax_Wr01(lvl_003, reg_ycsep_lvl_ctrl2, 0x7d9ab8d5);	// [4][5][6][7] hstart point
		CVD_H14Ax_WrFL(lvl_003);
		CVD_H14Ax_Wr01(lvl_006, reg_ycsep_lvl_ctrl5, 0x0624425f);	// [8][9][10][11] hstart point
		CVD_H14Ax_WrFL(lvl_006);
		CVD_H14Ax_Wr01(lvl_007, reg_ycsep_lvl_ctrl6, 0x7d9ab8d5);	// [12][13][14][15] hstart point
		CVD_H14Ax_WrFL(lvl_007);
	}
	else if(color_system == CVD_STATE_PALm) {
		CVD_H14Ax_Wr01(lvl_001, reg_ycsep_lvl_ctrl0, 0x2);	// [1]:measure block height (0=64, 1=128), [0] : measure block width (0=8*4, 1=16*4)
		CVD_H14Ax_WrFL(lvl_001);
		CVD_H14Ax_Wr01(lvl_002, reg_ycsep_lvl_ctrl1, 0x041c344c);	// [0][1][2][3] hstart point
		CVD_H14Ax_WrFL(lvl_002);
		CVD_H14Ax_Wr01(lvl_003, reg_ycsep_lvl_ctrl2, 0x647c94ab);	// [4][5][6][7] hstart point
		CVD_H14Ax_WrFL(lvl_003);
		CVD_H14Ax_Wr01(lvl_006, reg_ycsep_lvl_ctrl5, 0x041c344c);	// [8][9][10][11] hstart point
		CVD_H14Ax_WrFL(lvl_006);
		CVD_H14Ax_Wr01(lvl_007, reg_ycsep_lvl_ctrl6, 0x647c94ab);	// [12][13][14][15] hstart point
		CVD_H14Ax_WrFL(lvl_007);
	}
	else if(color_system == CVD_STATE_PALCn) {
		CVD_H14Ax_Wr01(lvl_001, reg_ycsep_lvl_ctrl0, 0x2);	// [1]:measure block height (0=64, 1=128), [0] : measure block width (0=8*4, 1=16*4)
		CVD_H14Ax_WrFL(lvl_001);
		CVD_H14Ax_Wr01(lvl_002, reg_ycsep_lvl_ctrl1, 0x031c344c);	// [0][1][2][3] hstart point
		CVD_H14Ax_WrFL(lvl_002);
		CVD_H14Ax_Wr01(lvl_003, reg_ycsep_lvl_ctrl2, 0x647c94ac);	// [4][5][6][7] hstart point
		CVD_H14Ax_WrFL(lvl_003);
		CVD_H14Ax_Wr01(lvl_006, reg_ycsep_lvl_ctrl5, 0x031c344c);	// [8][9][10][11] hstart point
		CVD_H14Ax_WrFL(lvl_006);
		CVD_H14Ax_Wr01(lvl_007, reg_ycsep_lvl_ctrl6, 0x647c94ac);	// [12][13][14][15] hstart point
		CVD_H14Ax_WrFL(lvl_007);
	}

	return 0;
}

int CVD_H14Ax_Get_ColorBar_Level(UINT8 *Composite_Level)
{
	UINT32 level_7_4, level_3_0, level_15_12, level_11_8;

	CVD_H14Ax_RdFL(lvl_010);
	CVD_H14Ax_Rd01(lvl_010,  reg_status_ycsep_lvl0, level_7_4);
	AFE_TRACE("[7_4]th = [0x%x]\n", level_7_4);
	CVD_H14Ax_RdFL(lvl_011);
	CVD_H14Ax_Rd01(lvl_011,  reg_status_ycsep_lvl1, level_3_0);
	AFE_TRACE("[3_0]th = [0x%x]\n", level_3_0);

	CVD_H14Ax_RdFL(lvl_016);
	CVD_H14Ax_Rd01(lvl_016,  reg_status_ycsep_lvl6, level_15_12);
	AFE_TRACE("[15_12]th = [0x%x]\n", level_15_12);
	CVD_H14Ax_RdFL(lvl_017);
	CVD_H14Ax_Rd01(lvl_017,  reg_status_ycsep_lvl7, level_11_8);
	AFE_TRACE("[11_8]th = [0x%x]\n", level_11_8);

	Composite_Level[15] = (level_15_12 >> 24) & 0xFF;
	Composite_Level[14] = (level_15_12 >> 16) & 0xFF;
	Composite_Level[13] = (level_15_12 >> 8) & 0xFF;
	Composite_Level[12] = (level_15_12 >> 0) & 0xFF;

	Composite_Level[11] = (level_11_8 >> 24) & 0xFF;
	Composite_Level[10] = (level_11_8 >> 16) & 0xFF;
	Composite_Level[9] = (level_11_8 >> 8) & 0xFF;
	Composite_Level[8] = (level_11_8 >> 0) & 0xFF;

	Composite_Level[7] = (level_7_4 >> 24) & 0xFF;
	Composite_Level[6] = (level_7_4 >> 16) & 0xFF;
	Composite_Level[5] = (level_7_4 >> 8) & 0xFF;
	Composite_Level[4] = (level_7_4 >> 0) & 0xFF;

	Composite_Level[3] = (level_3_0 >> 24) & 0xFF;
	Composite_Level[2] = (level_3_0 >> 16) & 0xFF;
	Composite_Level[1] = (level_3_0 >> 8) & 0xFF;
	Composite_Level[0] = (level_3_0 >> 0) & 0xFF;

	return 0;
}

int CVD_H14Ax_chromalock_level_control(unsigned int value)
{
	CVD_H14Ax_RdFL(iris_047);
	CVD_H14Ax_Wr01(iris_047, reg_lose_chromalock_level, value);	// default 0x7
	CVD_H14Ax_WrFL(iris_047);

	return 0;
}

int CVD_H14Ax_Set_SCART_Filter(void)
{
	CVD_DEBUG("%s entered \n",__func__);
	CVD_H14Ax_RdFL(iris_360);
	CVD_H14Ax_RdFL(iris_361);
	CVD_H14Ax_RdFL(iris_362);
	CVD_H14Ax_RdFL(iris_363);
	CVD_H14Ax_Wr01(iris_360, reg_fb_clpf_ctrl0, 0x00010000 );
	CVD_H14Ax_Wr01(iris_361, reg_fb_clpf_ctrl1, 0x00000000 );
	CVD_H14Ax_Wr01(iris_362, reg_fb_clpf_ctrl2, 0x00000000 );
	CVD_H14Ax_Wr01(iris_363, reg_fb_clpf_ctrl3, 0x20004000 );
	CVD_H14Ax_WrFL(iris_360);
	CVD_H14Ax_WrFL(iris_361);
	CVD_H14Ax_WrFL(iris_362);
	CVD_H14Ax_WrFL(iris_363);
	return 0;
}

int CVD_H14Ax_agc_half_en_control(int enable) //gogosing agc
{
	// For faster agc speed at channel change, set hagc_field_mode value to '1'
	if(enable){
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 1); //gogosing
	CVD_H14Ax_WrFL(iris_015);
	}
	else{
	CVD_H14Ax_RdFL(iris_015);
	CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 0); //gogosing
	CVD_H14Ax_WrFL(iris_015);
	}
	return 0;
}

int CVD_H14Ax_Set_Motion_Expand(int enable)
{
	UINT32 tmp;
	static int	motion_expand_enabled = -1;

	if(motion_expand_enabled == enable)
		return 0;
	else
		motion_expand_enabled = enable;

	CVD_DEBUG("%s [%d] \n",__func__, enable);

	if(enable)
	{
		// 0x4570[8] = 1
		CVD_H14Ax_RdFL(iris_220);
		CVD_H14Ax_Rd01(iris_220, reg_ycsep_3d_ctrl0, tmp);
		tmp = tmp | 0x100;
		CVD_H14Ax_Wr01(iris_220, reg_ycsep_3d_ctrl0, tmp);
		CVD_H14Ax_WrFL(iris_220);

		// 0x4590[31:24] = 0
		CVD_H14Ax_RdFL(iris_228);
		CVD_H14Ax_Rd01(iris_228, reg_ycsep_st_ctrl0, tmp);
		tmp = tmp & 0x00FFFFFF;
		CVD_H14Ax_Wr01(iris_228, reg_ycsep_st_ctrl0, tmp);
		CVD_H14Ax_WrFL(iris_228);

	}
	else
	{
		// 0x4570[8] = 0
		CVD_H14Ax_RdFL(iris_220);
		CVD_H14Ax_Rd01(iris_220, reg_ycsep_3d_ctrl0, tmp);
		tmp = tmp & 0xFFFFFEFF;
		CVD_H14Ax_Wr01(iris_220, reg_ycsep_3d_ctrl0, tmp);
		CVD_H14Ax_WrFL(iris_220);

		// 0x4590[31:24] = 0x0C 
		CVD_H14Ax_RdFL(iris_228);
		CVD_H14Ax_Rd01(iris_228, reg_ycsep_st_ctrl0, tmp);
		tmp = tmp | 0x0C000000;
		CVD_H14Ax_Wr01(iris_228, reg_ycsep_st_ctrl0, tmp);
		CVD_H14Ax_WrFL(iris_228);

	}

	return 0;
}

//CCR off only when NTSC Color Bar
int CVD_H14Ax_Set_CCR(int ccr_value)
{
	UINT32 tmp;
	static int	prev_ccr_value = -1;

	if(prev_ccr_value == ccr_value)
		return 0;
	else
		prev_ccr_value = ccr_value;

	CVD_DEBUG("%s [%d] \n",__func__, ccr_value);

	if(ccr_value == 0)
	{
		// 0x4740[3:0] = 0
		CVD_H14Ax_RdFL(iris_334);
		CVD_H14Ax_Rd01(iris_334, reg_ycsep_2d_ctrl37, tmp);
		tmp = tmp & 0xFFFFFFFC;
		CVD_H14Ax_Wr01(iris_334, reg_ycsep_2d_ctrl37, tmp);
		CVD_H14Ax_WrFL(iris_334);
	}
	else
	{
		// 0x4740[3:0] = 0
		CVD_H14Ax_RdFL(iris_334);
		CVD_H14Ax_Rd01(iris_334, reg_ycsep_2d_ctrl37, tmp);
		tmp = (tmp & 0xFFFFFFFC) | 0x00000002;	// by choi :130129
		CVD_H14Ax_Wr01(iris_334, reg_ycsep_2d_ctrl37, tmp);
		CVD_H14Ax_WrFL(iris_334);
	}

	return 0;
}

int CVD_H14Ax_Check_CVD_Memory(void)
{
	UINT32 cnt_mx_lvl, req_cnt_mx_lvl, cnt_mx_lvl_md, req_cnt_mx_lvl_md;
	UINT32 blue_mode;

	CVD_H14Ax_RdFL(iris_mif_gmua_mon_004);
	CVD_H14Ax_RdFL(iris_mif_gmua_mon_007);
	CVD_H14Ax_RdFL(iris_020);

	CVD_H14Ax_Rd01(iris_mif_gmua_mon_004, ro_read0_svid_cnt_mx_lvl, cnt_mx_lvl);
	CVD_H14Ax_Rd01(iris_mif_gmua_mon_004, ro_wbuf_read_req_cnt_mx_lvl, req_cnt_mx_lvl);
	CVD_H14Ax_Rd01(iris_mif_gmua_mon_007, ro_read0_svid_cnt_mx_lvl_md, cnt_mx_lvl_md);
	CVD_H14Ax_Rd01(iris_mif_gmua_mon_007, ro_wbuf_read_req_cnt_mx_lvl_md, req_cnt_mx_lvl_md);
	CVD_H14Ax_Rd01(iris_020, reg_blue_mode, blue_mode);

	if( (cnt_mx_lvl) || (req_cnt_mx_lvl) || (cnt_mx_lvl_md) || (req_cnt_mx_lvl_md) || (blue_mode == 3))
	{
		CVD_H14Ax_RdFL(iris_mif_gmua_mon_001);
		CVD_H14Ax_RdFL(iris_mif_gmua_mon_002);
		CVD_H14Ax_RdFL(iris_mif_gmua_mon_003);
		CVD_H14Ax_RdFL(iris_mif_gmua_mon_005);
		CVD_H14Ax_RdFL(iris_mif_gmua_mon_006);

		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!! CVD Memory ERROR !!!\n");
		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
		AFE_PRINT("!!! GMUA1 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_001) );
		AFE_PRINT("!!! GMUA2 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_002) );
		AFE_PRINT("!!! GMUA3 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_003) );
		AFE_PRINT("!!! GMUA4 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_004) );
		AFE_PRINT("!!! GMUA5 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_005) );
		AFE_PRINT("!!! GMUA6 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_006) );
		AFE_PRINT("!!! GMUA7 = [0x%08x]\n", CVD_H14Ax_Rd(iris_mif_gmua_mon_007) );
		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
		AFE_PRINT("!!!!!!!!!!!!!!!!!!!!\n");
	}

	return 0;
}

/* Enhanced Sync Stablity on PAL-M Weak RF signal */
int CVD_H14Ax_Set_PALm_Stable_HSync_Mode(int enable)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, enable);

	if(enable)
	{
		//for Brazil Field Stream : NTSC_120912_00607
		CVD_H14Ax_RdFL(iris_031);
		CVD_H14Ax_Wr01(iris_031, reg_hsync_phase_offset, 0x28);
		CVD_H14Ax_WrFL(iris_031);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_031);
		CVD_H14Ax_Wr01(iris_031, reg_hsync_phase_offset, 0x3e);	//default value
		CVD_H14Ax_WrFL(iris_031);
	}

	return 0;
}

/* Enhanced Sync Stablity on PAL-M Weak RF signal */
int CVD_H14Ax_Set_PALm_Stable_VSync_Mode(int enable)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, enable);

	if(enable)
	{
		//for Brazil Field Stream : NTSC_120912_0442
		CVD_H14Ax_RdFL(iris_041);
		CVD_H14Ax_Wr01(iris_041, reg_vsync_l_cnt, 0x4);
		CVD_H14Ax_WrFL(iris_041);

		CVD_H14Ax_RdFL(iris_015);
		CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 1);
		CVD_H14Ax_WrFL(iris_015);
		CVD_H14Ax_RdFL(iris_024);
		CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 2);
		CVD_H14Ax_WrFL(iris_024);

		//added 120114 for stable agc
		CVD_H14Ax_RdFL(iris_025);
		CVD_H14Ax_Wr01(iris_025, reg_agc_peak_cntl, 0x0);
		CVD_H14Ax_WrFL(iris_025);
		CVD_H14Ax_RdFL(iris_074);
		CVD_H14Ax_Wr01(iris_074, reg_dcrestore_gain, 0x3);
		CVD_H14Ax_WrFL(iris_074);

		CVD_H14Ax_RdFL(iris_054);
		CVD_H14Ax_Wr01(iris_054, reg_cpump_auto_stip_mode, 0x1);
		CVD_H14Ax_WrFL(iris_054);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_041);
		CVD_H14Ax_Wr01(iris_041, reg_vsync_l_cnt, 0x0);	//default value
		CVD_H14Ax_WrFL(iris_041);

		CVD_H14Ax_RdFL(iris_015);
		CVD_H14Ax_Wr01(iris_015, reg_agc_half_en, 0);
		CVD_H14Ax_WrFL(iris_015);
		CVD_H14Ax_RdFL(iris_024);
		CVD_H14Ax_Wr01(iris_024, reg_nstd_hysis, 7);
		CVD_H14Ax_WrFL(iris_024);


		//added 120114 for stable agc (restore default values)
		CVD_H14Ax_RdFL(iris_025);
		CVD_H14Ax_Wr01(iris_025, reg_agc_peak_cntl, 0x1);
		CVD_H14Ax_WrFL(iris_025);
		CVD_H14Ax_RdFL(iris_074);
		CVD_H14Ax_Wr01(iris_074, reg_dcrestore_gain, 0x0);
		CVD_H14Ax_WrFL(iris_074);

		CVD_H14Ax_RdFL(iris_054);
		CVD_H14Ax_Wr01(iris_054, reg_cpump_auto_stip_mode, 0x2);
		CVD_H14Ax_WrFL(iris_054);
	}

	return 0;
}

int CVD_H14Ax_3DComb_Workaround(int step)
{
#ifdef H14_CDTO_CONTROL
	UINT32 cdto_value;
#endif
	static int func_bypass = 0;

	CVD_WA_DEBUG("%s entered :  step[%d], bypass[%d] \n",__func__, step, func_bypass);

	if( (func_bypass == 1) && (step != 20) )
		return 0;

	OS_LockMutex(&g_CVD_Sema);

	if(step == 10)		// step 1 from CVD Channel Power Control(UI task)
	{
		func_bypass = 1;	// bypass workaround function (in other input source)
		step = 1;
	}
	else if(step == 20)	// back to CVD input source
	{
		func_bypass = 0;	// bypass workaround function (in other input source)
	}

	if(step == 1)
	{
#ifdef H14_CDTO_CONTROL
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value |= 0x80000000;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0x1);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0x1f);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0x1);
		CVD_H14Ax_WrFL(iris_064);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x1);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x1f);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x1);
		CVD_H14Ax_WrFL(iris_066);
	}
	else if(step == 2)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0);
		CVD_H14Ax_WrFL(iris_064);

	}
	else if(step == 3)
	{
		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x0);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x0);
		CVD_H14Ax_WrFL(iris_066);
	}
	else if(step == 4)
	{
#ifdef H14_CDTO_CONTROL
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value &= 0x7FFFFFFF;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif
	}
	else if(step == 0)
	{
#if 0
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value &= 0x7FFFFFFF;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif
		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x0);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0);
		CVD_H14Ax_WrFL(iris_064);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x0);
		CVD_H14Ax_WrFL(iris_066);
	}

	OS_UnlockMutex(&g_CVD_Sema);

	return 0;
}
/*
int CVD_H14Ax_3DComb_Workaround(int step)
{
	UINT32 cdto_value;

	AFE_PRINT("%s entered :  %d \n",__func__, step);

	if(step == 1)
	{
#if 0
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value |= 0x80000000;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0x1);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x1);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0x1f);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x1f);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0x1);
		CVD_H14Ax_WrFL(iris_064);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x1);
		CVD_H14Ax_WrFL(iris_066);
	}
	else if(step == 2)
	{
#if 0
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value |= 0x80000000;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif

		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x0);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0);
		CVD_H14Ax_WrFL(iris_064);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x0);
		CVD_H14Ax_WrFL(iris_066);
	}
	else if(step == 0)
	{
#if 0
		CVD_H14Ax_RdFL(iris_029);
		CVD_H14Ax_Rd01(iris_029, reg_cdto_inc, cdto_value);
		cdto_value &= 0x7FFFFFFF;
		CVD_H14Ax_Wr01(iris_029, reg_cdto_inc, cdto_value);
		CVD_H14Ax_WrFL(iris_029);
#endif
		CVD_H14Ax_RdFL(iris_mif_gmua_008);
		CVD_H14Ax_Wr01(iris_mif_gmua_008, reg_mif_ctrl, 0);
		CVD_H14Ax_WrFL(iris_mif_gmua_008);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_010);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_010, reg_mif_ctrl_md, 0x0);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_010);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau, 0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_175);
		CVD_H14Ax_Wr01(iris_175, iris_mif_gmau_md, 0x0);
		CVD_H14Ax_WrFL(iris_175);

		CVD_H14Ax_RdFL(iris_064);
		CVD_H14Ax_Wr01(iris_064, reg_comb2d_only, 0);
		CVD_H14Ax_WrFL(iris_064);

		CVD_H14Ax_RdFL(iris_066);
		CVD_H14Ax_Wr01(iris_066, reg_comb2d_only_md, 0x0);
		CVD_H14Ax_WrFL(iris_066);
	}

	return 0;
}
*/

void CVD_H14Ax_Program_Color_System_Size(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	CVD_CSD_DEBUG("Set CVD Buffer Size [0x%x]\n", pSet_system_t->reg_comb3_buffer_size);

	if(pSet_system_t->reg_comb3_buffer_size > 1)
	{
		CVD_H14Ax_RdFL(iris_mif_gmua_001);
		CVD_H14Ax_Wr01(iris_mif_gmua_001, reg_comb3_buffer_size, pSet_system_t->reg_comb3_buffer_size);
		CVD_H14Ax_WrFL(iris_mif_gmua_001);
		CVD_H14Ax_RdFL(iris_mif_gmua_002);
		CVD_H14Ax_Wr01(iris_mif_gmua_002, reg_fld1_init_rd_pel, pSet_system_t->reg_fld1_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_002);
		CVD_H14Ax_RdFL(iris_mif_gmua_003);
		CVD_H14Ax_Wr01(iris_mif_gmua_003, reg_fld2_init_rd_pel, pSet_system_t->reg_fld2_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_003);
		CVD_H14Ax_RdFL(iris_mif_gmua_004);
		CVD_H14Ax_Wr01(iris_mif_gmua_004, reg_fld3_init_rd_pel, pSet_system_t->reg_fld3_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_004);
		CVD_H14Ax_RdFL(iris_mif_gmua_005);
		CVD_H14Ax_Wr01(iris_mif_gmua_005, reg_fld4_init_rd_pel, pSet_system_t->reg_fld4_init_rd_pel);
		CVD_H14Ax_WrFL(iris_mif_gmua_005);

		/*
		CVD_H14Ax_RdFL(iris_mif_gmua_md_001);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_001, reg_comb3_buffer_size_md, pSet_system_t->reg_comb3_buffer_size_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_001);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_002);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_002, reg_fld1_init_rd_pel_md, pSet_system_t->reg_fld1_init_rd_pel_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_002);
		*/
	}
}

void CVD_H14Ax_Program_Color_System_Size_md(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t)
{
	CVD_CSD_DEBUG("Set CVD Buffer Size MD [0x%x]\n", pSet_system_t->reg_comb3_buffer_size_md);

	if(pSet_system_t->reg_comb3_buffer_size > 1)
	{
		CVD_H14Ax_RdFL(iris_mif_gmua_md_001);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_001, reg_comb3_buffer_size_md, pSet_system_t->reg_comb3_buffer_size_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_001);

		CVD_H14Ax_RdFL(iris_mif_gmua_md_002);
		CVD_H14Ax_Wr01(iris_mif_gmua_md_002, reg_fld1_init_rd_pel_md, pSet_system_t->reg_fld1_init_rd_pel_md);
		CVD_H14Ax_WrFL(iris_mif_gmua_md_002);
	}
}

// for Indonesia Field Stream (PAL_120928_0331, PAL_121002_0306)
int CVD_H14Ax_Set_Vsync_Cntl(CVD_SET_SYSTEM_3CS_H14A0_T *pSet_system_t, int enable)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, enable);

	if(enable == 2)
	{
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 0);	// PLL vsync
		CVD_H14Ax_WrFL(iris_042);
	}
	else if(enable == 1)
	{
		CVD_H14Ax_RdFL(iris_041);
		CVD_H14Ax_Wr01(iris_041,  reg_vsync_l_cnt, 2);	// default 0
		CVD_H14Ax_WrFL(iris_041);

		CVD_H14Ax_RdFL(iris_037);
		CVD_H14Ax_Wr01(iris_037, reg_vactive_start, pSet_system_t->reg_vactive_start - 2);
		CVD_H14Ax_WrFL(iris_037);
	}
	else
	{
		CVD_H14Ax_RdFL(iris_042);
		CVD_H14Ax_Wr01(iris_042,  reg_vsync_cntl, 1);	// direct vsync
		CVD_H14Ax_WrFL(iris_042);

		CVD_H14Ax_RdFL(iris_041);
		CVD_H14Ax_Wr01(iris_041,  reg_vsync_l_cnt, 0);	// default 0
		CVD_H14Ax_WrFL(iris_041);

		CVD_H14Ax_RdFL(iris_037);
		CVD_H14Ax_Wr01(iris_037, reg_vactive_start, pSet_system_t->reg_vactive_start);
		CVD_H14Ax_WrFL(iris_037);
	}

	return 0;
}

int CVD_H14Ax_CVD_Soft_Reset(void)
{
	CVD_H14Ax_RdFL(iris_012);
	CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 1);
	CVD_H14Ax_WrFL(iris_012);

	OS_MsecSleep(5);

	CVD_H14Ax_RdFL(iris_012);
	CVD_H14Ax_Wr01(iris_012, reg_cvd_soft_reset, 0);
	CVD_H14Ax_WrFL(iris_012);

	return 0;
}

int CVD_H14Ax_Set_Blue_Mode(int blue)
{
	CVD_H14Ax_RdFL(iris_020);
	CVD_H14Ax_Wr01(iris_020, reg_blue_mode, blue);
	CVD_H14Ax_WrFL(iris_020);

	return 0;
}
int CVD_H14Ax_Set_OADJ(CVD_REG_PARAM_T	*pCVD_Black_Level_Param_t, UINT32 size)
{
	int count;
	UINT32 value;
	UINT32 recvAddr;
	CVD_DEBUG("%s \n", __func__);
	for(count=0; count < size;count ++)
	{
		recvAddr = pCVD_Black_Level_Param_t->cvd_phy_addr;
		value = pCVD_Black_Level_Param_t->cvd_pe_value;
		{
			recvAddr = recvAddr + (UINT32)gpRealRegCVD_H14Ax - 0x4100;
			REG_WD(recvAddr, value);
		}
		pCVD_Black_Level_Param_t ++;
	}
	return 0;
}

int CVD_H14Ax_Get_AFE_CVBS_status(UINT8 *cvbs_icon_value, UINT8 *cvbs_pdb_value, UINT8 *cvbs_cp_value, UINT8 *cvbs_insel_value)
{
	ACE_REG_H14A0_RdFL(afe_cvbs_2);
	ACE_REG_H14A0_RdFL(afe_cvbs_3);

	ACE_REG_H14A0_Rd01(afe_cvbs_2, cvbs_icon, *cvbs_icon_value);
	ACE_REG_H14A0_Rd01(afe_cvbs_3, cvbs_pdb, *cvbs_pdb_value);
	ACE_REG_H14A0_Rd01(afe_cvbs_3, cvbs_cp, *cvbs_cp_value);
	ACE_REG_H14A0_Rd01(afe_cvbs_3, cvbs_insel, *cvbs_insel_value);

	return 0;
}

int CVD_H14Ax_Get_AFE_VBUF_status(UINT8 *buf_ycm_value, UINT8 *buf_sel1_value, UINT8 *buf_pdb1_value, UINT8 *vdac_pdb_value)
{
	ACE_REG_H14A0_RdFL(afe_vbuf_0);
	ACE_REG_H14A0_RdFL(afe_vbuf_1);
	ACE_REG_H14A0_RdFL(afe_vdac_0);

	ACE_REG_H14A0_Rd01(afe_vbuf_0, buf_ycm, *buf_ycm_value);
	ACE_REG_H14A0_Rd01(afe_vbuf_1, buf_sel1, *buf_sel1_value);
	ACE_REG_H14A0_Rd01(afe_vbuf_1, buf_pdb1, *buf_pdb1_value);
	ACE_REG_H14A0_Rd01(afe_vdac_0, vdac_pdb, *vdac_pdb_value);

	return 0;
}

int CVD_H14Ax_Set_Vactive_Params(int vline_625_stable)
{
	static int vline_625_stable_prev = -1;

	if(vline_625_stable != vline_625_stable_prev)
	{
		vline_625_stable_prev = vline_625_stable;

		if( (vline_625_stable) \
			/*	&& ( ( g_CVD_Status_3CS.color_system_support & (LX_COLOR_SYSTEM_PAL_G |LX_COLOR_SYSTEM_PAL_NC | LX_COLOR_SYSTEM_SECAM)) == 0 )*/ )	// 50Hz system detected, and 60Hz only system supporting
		{
			CVD_H14Ax_RdFL(iris_037);
			//CVD_H14Ax_Wr02(iris_037, reg_vactive_start, g_SetColorSystem_3CS_H14A0[CVD_STATE_PALi].reg_vactive_start, reg_vactive_height, g_SetColorSystem_3CS_H14A0[CVD_STATE_PALi].reg_vactive_height);
			CVD_H14Ax_Wr01(iris_037, reg_vactive_height, g_SetColorSystem_3CS_H14A0[CVD_STATE_PALi].reg_vactive_height);
			CVD_H14Ax_WrFL(iris_037);
		}
		else if ( (vline_625_stable == 0) \
				/* && (( g_CVD_Status_3CS.color_system_support & (LX_COLOR_SYSTEM_NTSC_M |LX_COLOR_SYSTEM_PAL_M | LX_COLOR_SYSTEM_NTSC_443 | LX_COLOR_SYSTEM_PAL_60)) == 0 ) */ )				// 60Hz system detected, and 50Hz only system supporting
		{
			CVD_H14Ax_RdFL(iris_037);
			//CVD_H14Ax_Wr02(iris_037, reg_vactive_start, g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSC].reg_vactive_start, reg_vactive_height, g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSC].reg_vactive_height);
			CVD_H14Ax_Wr01(iris_037, reg_vactive_height, g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSC].reg_vactive_height);
			CVD_H14Ax_WrFL(iris_037);
		}

	}


	return 0;
}

int CVD_H14Ax_Set_Blend_Ratio(int value)
{

	CVD_DEBUG("%s entered :  %d \n",__func__, value);

	if(value > 0x10)
		return -1;

	CVD_H14Ax_RdFL(fastblank_001);
	CVD_H14Ax_Wr01(fastblank_001, reg_fb_blend_ratio, value);
	CVD_H14Ax_WrFL(fastblank_001);

	return 0;
}

int CVD_H14Ax_Set_ycsep_Blend(int blend)
{
	int blend_value;
	blend &= 0xF;

	CVD_H14Ax_RdFL(iris_236);
	CVD_H14Ax_Rd01(iris_236,  reg_ycsep_blend_ctrl0, blend_value);

	blend_value = ( blend_value & 0xFFFFFFF0 ) | blend;

	CVD_H14Ax_Wr01(iris_236, reg_ycsep_blend_ctrl0, blend_value);
	CVD_H14Ax_WrFL(iris_236);

	return 0;
}

int CVD_H14Ax_Set_clampagc_updn(int updn_value)
{

	CVD_H14Ax_RdFL(iris_178);
	CVD_H14Ax_Wr01(iris_178, reg_clampagc_updn, updn_value);
	CVD_H14Ax_WrFL(iris_178);

	return 0;
}

int CVD_H14Ax_Get_status_clamp_updn(void)
{
	int status_updn;

	CVD_H14Ax_RdFL(iris_179);
	CVD_H14Ax_Rd01(iris_179,  reg_status_updn, status_updn);

	return status_updn;
}

int CVD_H14Ax_Set_dc_clamp_mode(int mode)
{
	CVD_H14Ax_RdFL(iris_015);
	// dc_clamp_mode : 0(auto), 1(backporch), 2(synctip), 3(off)
	CVD_H14Ax_Wr01(iris_015, reg_dc_clamp_mode, mode);
	CVD_H14Ax_WrFL(iris_015);

	return 0;
}
