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
 *  @author		wonsik.do
 *  @version	1.0 
 *  @date		2012-04-25
 *  @note		Additional information. 
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "afe_drv.h"
#include "os_util.h"

#include "cvd_hw_h13a0.h"
#include "cvd_control_h13a0.h"
/************** L9Bx CVD New PE Settings ******************************/
#include "cvd_pe_h13a0/CVD_L_AV_NTSC_443_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_NTSC_M_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_PAL60_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_PALI_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_PALM_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_PALNc_default.h"
#include "cvd_pe_h13a0/CVD_L_AV_SECAM_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_NTSC_443_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_NTSC_M_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_PAL60_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_PALI_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_PALM_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_PALNc_default.h"
#include "cvd_pe_h13a0/CVD_L_RF_SECAM_default.h"



/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

#define NOISE_TH_FOR_PEAK_EN_CONTROL		0x170
#define AGC_GAIN_TH_FOR_PEAK_EN_CONTROL		0xB80
#define DCRESTORE_ACCUM_WIDTH_FOR_WEAK_RF	0x22
#define DCRESTORE_ACCUM_WIDTH_DEFAULT		0x1B
#define NOISE_THRESHOLD_WEAK_RF				0x80
#define NOISE_THRESHOLD_DEFAULT				0x32
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern CVD_STATUS_3CS_T	g_CVD_Status_3CS ;
//extern CVD_STATE_T	gMainCVD_State;
//extern CVD_STATE_T	gPrevious_Main_System;

extern OS_SEM_T	g_CVD_Sema;
//extern LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	g_CVD_Color_System_Support ;
//extern int		gAutoProgramming;
extern CVD_PATTERN_DETECTION_T g_CVD_Pattern_Detection_t ;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

CVD_SET_SYSTEM_3CS_H13A0_T g_SetColorSystem_3CS_H13A0[]=
{ 
		// new L9Bx 3DComb Buffer Value 						cvd1_clampagc_on    colour_mode    ped  	   cagc_en	 dc_clamp			adaptive_mode	hagc  					cdto_inc	hdtc_inc  	vactive_start		cagc   	cagc_av    	 	 lose_chromalock  	comb2d_only		  			hactive_md_s/w	hstate_max		hactive_start_54M	  vsync_signal_thr			cs/cs1_chroma_burst5or10	cordic_gate_s/e_3CS							fb_hstart								burst_gate_s/e				  contrast_av  			cs_issecam_th
		//																phase_offset	 vline_625		adc_blank_level		  hagc_field 		 adaptive_3d	  yc_delay											vactive_height	  		  secam_ybw			motion_mode		 fb_sync									 dcrestore_accum_width		hnon_std_thr	fixed_cstate				hrs_ha_start					burst_gate_s/e_3CS					cagc_gate_s/e									dcrestore_hsync_mid		  saturation
		//																	  ispal_th		  hpixel				agc_half_en		chroma_bw_lo		pal60			cagc_gate_s/e									  ntsc443					auto_secam_level adaptive_chroma	vactive_md_s/h										  hactive_width_54M				 	cstate						hrs_ha_width							fb_vstart_o/e						cordic_gate_s/e							contrast	fb_height_half
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
	// default																																																																																																																																			//default
		{955500, 	716625, 	477750, 	238875, 	0x0, 	0x0, 	0x20, 0x1f, 0x0, 0x0, 0x0, 0x1, 0x3c0, 0x1, 0x1, 0x0, 0x0, 	0x0, 	0x0, 0x1, 	0, 	0xDD, 0x0, 	0x32, 0x50, 0x21F07C1F, 0x20000000, 0x23,	0x61, 0x0, 	0x8A,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x0, 	0x0, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x3, 0x1b, 		0x84, 0x50,		0x06, 0x0, 		0x0,	0x5, 	0x0, 0x0,	0x82,	0x50,	0x3c, 0x6e,		0x32, 0x46,		0x13, 0x14,	0x82,	0x32, 0x50, 	0x3c, 0x6e, 	0x32, 0x46,		0xFE,	0x80, 0x80, 0x60, 0x80,		0x08},
	//NTSC-M																							   			   	   	   		 				 	   	   	   	 									                                                                                                                                                                                                                                                                                                                             			  	         	//NTSC M 
		{0xE946A, 	0xAF83B, 	0x74DC5, 	0x3AE05, 	0x38F, 	0x1, 	0x80, 0x18, 0x0, 0x0, 0x0, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x0, 0x1, 	0, 	0xDD, 0x0, 	0x32, 0x50, 0x21F07C1F, 0x20000000, 0x27,	0x61, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x1b,		0x86, 0x50,		0x18, 0x10,		0x1,	0x7, 	0x0, 0x0,	0x61,	0x50,	0x32, 0x46,		0x32, 0x4b,		0x08, 0x09,	0x8a,	0x34, 0x47, 	0x34, 0x47, 	0x34, 0x47,		0xF7,	0x82, 0x80, 0x60, 0x80,		0x08},
	//NTSC-J,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      			  	         	//NTSC J 
		{0xE946A, 	0xAE83B, 	0x74DC5, 	0x3AE05, 	0x71D, 	0x1, 	0x80, 0x18, 0x0, 0x0, 0x0, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x0, 0x1, 	0, 	0xCD, 0x0, 	0x32, 0x50, 0x21F07C1F, 0x20000000, 0x27,	0x61, 0x0, 	0x80,	0x80, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x25,		0x86, 0x50,		0x18, 0x10,		0x1,	0x7, 	0x0, 0x0,	0x61,	0x50,	0x32, 0x46,		0x32, 0x46,		0x08, 0x09,	0x8a,	0x34, 0x47, 	0x34, 0x47, 	0x34, 0x47,		0xFE,	0x82, 0x80, 0x60, 0x80,		0x08},
	//NTSC-443,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    			  	         	//NTSC 443 
		{0 , 		0 , 		0 , 		0 , 		0 , 	0x1, 	0x80, 0x18, 0x0, 0x0, 0x0, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x0, 0x0, 	0, 	0xDD, 0x0, 	0x32, 0x50, 0x2A098ACB, 0x20000000, 0x27,	0x61, 0x1, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x1, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x25,		0x7c, 0x50,		0x18, 0x10,		0x1,	0x7, 	0x0, 0x0,	0x4D,	0x50,	0x32, 0x46,		0x32, 0x46,		0x08, 0x09,	0x8a,	0x34, 0x42, 	0x34, 0x42, 	0x34, 0x42,		0xFE,	0x82, 0x80, 0x60, 0x80,		0x08 },
	//PAL-I,B,G,H,D,                                                                                                                                                                                         	                                                                                                                                                                                                                                                                                                                     			  	         	//PAL i 
		{0x2B4C0A, 	0x207D79, 	0x15AA76,	0xAD773, 	0x470, 	0x1, 	0x80, 0x18, 0x1, 0x1, 0x1, 0x0, 0x3f0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x2, 0x1, 	0, 	0xDC, 0x3, 	0x32, 0x50, 0x2A098ACB, 0x20000000, 0x2f,	0xC1, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x2c, 0xbf, 	0x6E, 0x50, 	0x5, 0x25,		0x88, 0x50,		0x18, 0x10, 	0x1,	0x7, 	0x0, 0x0,	0x5c,	0x50,	0x32, 0x5e,		0x32, 0x6E,		0x08, 0x08,	0x8d,	0x34, 0x4a, 	0x34, 0x4a, 	0x34, 0x4a,		0xFE,	0x7B, 0x80, 0x6e, 0x80,		0x08 },
	//PAL_N,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       			  	         	//PAL N 
		{0x2B4C0A, 	0x207D79, 	0x15AA76,	0xAD773, 	0x470, 	0x1, 	0x80, 0x18, 0x1, 0x1, 0x1, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x2, 0x1, 	0, 	0xDD, 0x3, 	0x32, 0x50, 0x2A098ACB, 0x20000000, 0x2f,	0xC1, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x2c, 0xbf, 	0x6E, 0x50, 	0x5, 0x25,		0x88, 0x50,		0x18, 0x10, 	0x1,	0x7, 	0x0, 0x0,	0x5c,	0x50,	0x32, 0x6e,		0x32, 0x46,		0x08, 0x08,	0x8d,	0x34, 0x4a, 	0x34, 0x4a, 	0x34, 0x4a,		0xFE,	0x7B, 0x80, 0x6e, 0x80,		0x08 },
	//PAL-M,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       			  	         	// PAL M 
		{0x1D20A2,	0x15DC09,	0xE93E0, 	0x74BB7, 	0x38E, 	0x1, 	0x80, 0x18, 0x2, 0x0, 0x0, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x2, 0x1, 	0, 	0xDD, 0x3, 	0x32, 0x50, 0x21E6EFA4, 0x20000000, 0x27,	0x61, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x25,		0x88, 0x50,		0x30, 0x10, 	0x1,	0x7, 	0x0, 0x0,	0x61,	0x50,	0x32, 0x46,		0x32, 0x46,		0x08, 0x09,	0x8a,	0x34, 0x51, 	0x34, 0x51, 	0x34, 0x51,		0xFE,	0x82, 0x80, 0x60, 0x80,		0x08  },
	//PAL-CN,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      			  	         	// PAL CN 
		{0x22FB22, 	0x1A3FF1,	0x118128,	0x8C25F, 	0x396, 	0x1, 	0x80, 0x18, 0x3, 0x1, 0x1, 0x0, 0x3c0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x2, 0x1, 	0, 	0xDC, 0x3, 	0x32, 0x50, 0x21f69446, 0x20000000, 0x2f,	0xC1, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x0, 0x0, 	0x2c, 0xbf, 	0x6E, 0x50, 	0x5, 0x25,		0x92, 0x4a,		0x18, 0x10, 	0x1,	0x7, 	0x0, 0x0,	0x6e,	0x50,	0x32, 0x6e,		0x32, 0x46,		0x08, 0x08,	0x8d,	0x34, 0x4f, 	0x34, 0x4f, 	0x34, 0x4f,		0xFE,	0x7B, 0x80, 0x6e, 0x80,		0x08  },	
	//PAL-60,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      			  	         	// PAL 60 
		{0 , 		0 ,	 		0 , 		0 , 		0 , 	0x1, 	0x80, 0x18, 0x1, 0x0, 0x0, 0x0, 0x3f0, 0x1, 0x0, 0x0, 0x0, 	0x0, 	0x2, 0x0, 	1, 	0xDD, 0x3, 	0x32, 0x50, 0x2A098ACB, 0x20000000, 0x27,	0x61, 0x0, 	0x80,	0x8a, 0x0, 	0x0, 0x3, 	0x0, 0x1, 	0x1, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x25,		0x7D, 0x50,		0x18, 0x10,		0x1,	0x7, 	0x0, 0x0,	0x52,	0x50,	0x32, 0x46,		0x32, 0x46,		0x08, 0x09,	0x8a,	0x34, 0x4a, 	0x34, 0x4a, 	0x34, 0x4a,		0xFE,	0x7B, 0x80, 0x60, 0x80,		0x08  },
	//SECAM,                                                                                                                   	     	          	   	           	                                                       	               //110809                                                                                                                                                                                                                                                                                  			  	         	// SECAM 
		{0 , 		0 , 		0 , 		0 , 		0 , 	0x1, 	0x80, 0x18, 0x4, 0x1, 0x1, 0x0, 0x3f0, 0x0, 0x0, 0x0, 0x0, 	0x0, 	0x0, 0x0, 	0, 	0xDC, 0x5, 	0x32, 0x50, 0x28A33BB2, 0x20000000, 0x2b,	0xC1, 0x0, 	0x80,	0x8a, 0x1, 	0x1, 0x3, 	0x0, 0x1, 	0x1, 0x0, 	0x24, 0x5F, 	0x6E, 0x50, 	0x5, 0x25,		0x8D, 0x50,		0x18, 0x10,		0x0,	0x2, 	0x0, 0x0,	0x78,	0x50,	0x32, 0x60,		0x32, 0x6E,		0x08, 0x08,	0x8d,	0x32, 0x50, 	0x32, 0x60, 	0x32, 0x6e,		0xFE,	0x80, 0x80, 0x6e, 0x80,0x20  },
																																																																																																																																				  //120912 for valid level
};			

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int CVD_H13Ax_AGC_Peak_En_Control(int force_peak_enable, int avg_noise, int avg_agc_gain)
{
	CVD_DEBUG("%s entered :  En[%d],noise[0x%x],gain[0x%x] \n",__func__, force_peak_enable, avg_noise, avg_agc_gain);
	if (force_peak_enable == 2) {
		CVD_H13Ax_Set_AGC_Peak_En(1);
	}
	else if (force_peak_enable == 0) {
		CVD_H13Ax_Set_AGC_Peak_En(0);
	}
	else if (force_peak_enable == 1) {
		if ( (avg_noise > NOISE_TH_FOR_PEAK_EN_CONTROL) && (avg_agc_gain > AGC_GAIN_TH_FOR_PEAK_EN_CONTROL)) {
			CVD_DEBUG("##### AGC PEAK EN Disabled !!!!! ##### \n");
			CVD_H13Ax_Set_AGC_Peak_En(0);
		}
		else {
			//				CVD_DEBUG("##### AGC PEAK EN Enabled !!!!! ##### \n");
			CVD_H13Ax_Set_AGC_Peak_En(1);
		}
	}

	return 0;
}

int CVD_H13Ax_NoBurst_CKill_Control(CVD_STATUS_3CS_T *pCVD_Status, int clear_noburst_ckill)
{

	static int no_color_count = 0;

	/*
	   CVD_DEBUG("%s entered :  chroma_lock[%d],h_lock[%d],v_lock[%d], hnon_standard[%d], vnon_standard[%d], \n",\
	   __func__, pCVD_Status->cvd_chromalock_flag, pCVD_Status->h_lock_flag, pCVD_Status->v_lock_flag, pCVD_Status->hnon_standard_flag, pCVD_Status->vnon_standard_flag);
	 */

	if(clear_noburst_ckill) {
		g_CVD_Status_3CS.no_color_detected = 0;
		no_color_count = 0;
		CVD_DEBUG("Clear NoBurst Ckill Reg!\n");

		CVD_H13Ax_Set_Noburst_Ckill(0);

	}
	else if( (pCVD_Status->cvd_chromalock_flag == 0) /* && (pCVD_Status->cs0_chromalock_flag == 0) && (pCVD_Status->cs1_chromalock_flag == 0) */ \
			&& (pCVD_Status->h_lock_flag == 1) && (pCVD_Status->v_lock_flag == 1)\
			&& ( ( (pCVD_Status->cvd_color_system == CVD_STATE_PALi) && (pCVD_Status->cvd_secam_flag == 0) && (pCVD_Status->cvd_pal_flag == 0)) || (pCVD_Status->cvd_color_system == CVD_STATE_NTSC) ) \
			/*		&& (pCVD_Status->hnon_standard_flag == 1) */&& (pCVD_Status->vnon_standard_flag == 0) \
			&& (CVD_H13Ax_Get_CVD_Burst_Mag_Value() < 0x600)	//Added 120709: burst ckill only for small burst signal
		   ) {
		if(no_color_count > 5) {
		}
		else if(no_color_count == 5) {
			CVD_DEBUG("No Burst Detected !!!\n");

			CVD_H13Ax_Set_Noburst_Ckill(1);

			g_CVD_Status_3CS.no_color_detected = 1;
			no_color_count ++;
		}
		else
			no_color_count ++;
	}
	else {
		if(g_CVD_Status_3CS.no_color_detected == 1) {
			CVD_DEBUG("Burst Detected !\n");

			CVD_H13Ax_Set_Noburst_Ckill(0);
		}

		g_CVD_Status_3CS.no_color_detected = 0;
		no_color_count = 0;
	}

	return 0;
}

int CVD_H13Ax_Burst_Gate_Control_for_NoBurst(CVD_STATUS_3CS_T *pCVD_Status, int clear_burst_gate)
{

	static int no_color_count = 0;

	/*
	   CVD_DEBUG("%s entered :  chroma_lock[%d],h_lock[%d],v_lock[%d], hnon_standard[%d], vnon_standard[%d], \n",\
	   __func__, pCVD_Status->cvd_chromalock_flag, pCVD_Status->h_lock_flag, pCVD_Status->v_lock_flag, pCVD_Status->hnon_standard_flag, pCVD_Status->vnon_standard_flag);
	 */

	if(clear_burst_gate) {
		g_CVD_Status_3CS.no_color_detected = 0;
		no_color_count = 0;
		CVD_DEBUG("Set to default burst gate values!\n");

		CVD_H13Ax_Set_Noburst_Ckill(0);

	}
	else if( (pCVD_Status->cvd_chromalock_flag == 0) /* && (pCVD_Status->cs0_chromalock_flag == 0) && (pCVD_Status->cs1_chromalock_flag == 0) */ \
			&& (pCVD_Status->h_lock_flag == 1) && (pCVD_Status->v_lock_flag == 1)\
			&& ( ( (pCVD_Status->cvd_color_system == CVD_STATE_PALi) && (pCVD_Status->cvd_secam_flag == 0) && (pCVD_Status->cvd_pal_flag == 0)) || (pCVD_Status->cvd_color_system == CVD_STATE_NTSC) ) \
			/*		&& (pCVD_Status->hnon_standard_flag == 1) */&& (pCVD_Status->vnon_standard_flag == 0) \
		//	&& (CVD_H13Ax_Get_CVD_Burst_Mag_Value() < 0x600)	//Added 120709: burst ckill only for small burst signal
		   ) {
		if(no_color_count > 5) {
		}
		else if(no_color_count == 5) {
			CVD_DEBUG("No Burst Detected !!!\n");

			CVD_H13Ax_Burst_Gate_Control(g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_start_new, 0xC0);

			g_CVD_Status_3CS.no_color_detected = 1;
			no_color_count ++;
		}
		else
			no_color_count ++;
	}
	else {
		if(g_CVD_Status_3CS.no_color_detected == 1) {
			CVD_DEBUG("Burst Detected !\n");

			CVD_H13Ax_Burst_Gate_Control(g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_start_new, g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_end_new);
		}

		g_CVD_Status_3CS.no_color_detected = 0;
		no_color_count = 0;
	}

	return 0;
}

int CVD_H13Ax_Cstate_Control_for_SC_PullInRange(CVD_STATUS_3CS_T *pCVD_Status, int init_cstate) 
{

	static int chroma_unlock_count = 0;
	static int no_burst_count = 0;

	   CVD_DEBUG("%s entered :  chroma_lock[%d],h_lock[%d],v_lock[%d], hnon_standard[%d], vnon_standard[%d], \n",\
	   __func__, pCVD_Status->cvd_chromalock_flag, pCVD_Status->h_lock_flag, pCVD_Status->v_lock_flag, pCVD_Status->hnon_standard_flag, pCVD_Status->vnon_standard_flag);

	if(init_cstate) {
		g_CVD_Status_3CS.no_color_detected = 0;
		chroma_unlock_count = 0;
		CVD_DEBUG("Set to default cstate values!\n");

		CVD_H13Ax_Program_Color_System_PreJob(&g_SetColorSystem_3CS_H13A0[CVD_STATE_Default]);

	}
	else if( (pCVD_Status->cvd_chromalock_flag == 0) /* && (pCVD_Status->cs0_chromalock_flag == 0) && (pCVD_Status->cs1_chromalock_flag == 0) */ \
			&& (pCVD_Status->h_lock_flag == 1) && (pCVD_Status->v_lock_flag == 1)\
			/*		&& (pCVD_Status->hnon_standard_flag == 1) */&& (pCVD_Status->vnon_standard_flag == 0) \
			&& (CVD_H13Ax_Get_CVD_Burst_Mag_Value() > 0x1000)
		   ) {
		if(chroma_unlock_count > 5) {
		}
		else if(chroma_unlock_count == 5) {
			CVD_DEBUG("Chroma Unlock and Normal Burst Magnitude!!!\n");

			CVD_H13Ax_Program_Color_System_PostJob(&g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_color_system]);

			g_CVD_Status_3CS.no_color_detected = 1;
			chroma_unlock_count ++;
		}
		else
			chroma_unlock_count ++;
	}
	else {
		if(g_CVD_Status_3CS.no_color_detected == 1) {
			CVD_DEBUG("Chroma Lock Detected !\n");

			CVD_H13Ax_Program_Color_System_PreJob(&g_SetColorSystem_3CS_H13A0[CVD_STATE_Default]);
		}

		g_CVD_Status_3CS.no_color_detected = 0;
		chroma_unlock_count = 0;
	}

	// to fix slow color kill on no burst signal

	if( (pCVD_Status->cvd_chromalock_flag == 0) /* && (pCVD_Status->cs0_chromalock_flag == 0) && (pCVD_Status->cs1_chromalock_flag == 0) */ \
			&& (pCVD_Status->h_lock_flag == 1) && (pCVD_Status->v_lock_flag == 1)\
			/*		&& (pCVD_Status->hnon_standard_flag == 1) */&& (pCVD_Status->vnon_standard_flag == 0) \
			&& ( ( (pCVD_Status->cvd_color_system == CVD_STATE_PALi) && (pCVD_Status->cvd_secam_flag == 0) && (pCVD_Status->cvd_pal_flag == 0)) || (pCVD_Status->cvd_color_system == CVD_STATE_NTSC) ) \
			&& (CVD_H13Ax_Get_CVD_Burst_Mag_Value() < 0x600)
	  ) {
		if(no_burst_count > 2) {
		}
		else if(no_burst_count == 2) {
			CVD_DEBUG("Chroma Unlock and Small Burst !!!\n");

			CVD_H13Ax_Force_Color_Kill(1);
			no_burst_count ++;
		}
		else
			no_burst_count ++;

	}
	else if( (pCVD_Status->cvd_chromalock_flag == 1) && (no_burst_count > 2))
	{
		CVD_DEBUG("Chroma Locked!!!\n");
		no_burst_count = 0;

		CVD_H13Ax_Force_Color_Kill(0);
	}
	return 0;
}

int CVD_H13Ax_Pattern_Detection(CVD_PATTERN_DETECTION_T *pCVD_Pattern_Detection_t )
{
	static CVD_PE0_MOTION_VALUE_T pe0_motion_value_t = {{0,}};

	int s_status_diff ;

	pe0_motion_value_t.tnr_x_avg_t[3] = pe0_motion_value_t.tnr_x_avg_t[2] ;
	pe0_motion_value_t.tnr_x_avg_t[2] = pe0_motion_value_t.tnr_x_avg_t[1] ;
	pe0_motion_value_t.tnr_x_avg_t[1] = pe0_motion_value_t.tnr_x_avg_t[0] ;
	pe0_motion_value_t.tnr_x_avg_s[3] = pe0_motion_value_t.tnr_x_avg_s[2] ;
	pe0_motion_value_t.tnr_x_avg_s[2] = pe0_motion_value_t.tnr_x_avg_s[1] ;
	pe0_motion_value_t.tnr_x_avg_s[1] = pe0_motion_value_t.tnr_x_avg_s[0] ;
	pe0_motion_value_t.tpd_s_status[3] = pe0_motion_value_t.tpd_s_status[2] ;
	pe0_motion_value_t.tpd_s_status[2] = pe0_motion_value_t.tpd_s_status[1] ;
	pe0_motion_value_t.tpd_s_status[1] = pe0_motion_value_t.tpd_s_status[0] ;

	CVD_H13Ax_Get_PE0_Motion_Value(&pe0_motion_value_t.tnr_x_avg_t[0], &pe0_motion_value_t.tnr_x_avg_s[0], &pe0_motion_value_t.tpd_s_status[0] );

	pe0_motion_value_t.avg_tnr_x_avg_t = (pe0_motion_value_t.tnr_x_avg_t[3] +  pe0_motion_value_t.tnr_x_avg_t[2] +  pe0_motion_value_t.tnr_x_avg_t[1] +  pe0_motion_value_t.tnr_x_avg_t[0] ) >> 2;
	pe0_motion_value_t.avg_tnr_x_avg_s = (pe0_motion_value_t.tnr_x_avg_s[3] +  pe0_motion_value_t.tnr_x_avg_s[2] +  pe0_motion_value_t.tnr_x_avg_s[1] +  pe0_motion_value_t.tnr_x_avg_s[0] ) >> 2;
	pe0_motion_value_t.avg_tpd_s_status = (pe0_motion_value_t.tpd_s_status[3] +  pe0_motion_value_t.tpd_s_status[2] +  pe0_motion_value_t.tpd_s_status[1] +  pe0_motion_value_t.tpd_s_status[0] ) >> 2;

//	CVD_DEBUG("PE0 : avg_t[0x%x], avg_s[0x%x], s_status[0x%x] \n", pe0_motion_value_t.avg_tnr_x_avg_t, pe0_motion_value_t.avg_tnr_x_avg_s, pe0_motion_value_t.avg_tpd_s_status);

	pCVD_Pattern_Detection_t->global_motion_value = CVD_H13Ax_Get_Global_Motion_Value();

	pCVD_Pattern_Detection_t->global_motion_diff = pCVD_Pattern_Detection_t->prev_motion_value - pCVD_Pattern_Detection_t->global_motion_value;

	if( pCVD_Pattern_Detection_t->pattern_found == 1) {
		s_status_diff = 0x60;
	}
	else
		s_status_diff = 0;

	if( ( abs(pCVD_Pattern_Detection_t->global_motion_diff) <= pCVD_Pattern_Detection_t->motion_diff_th) \
			&& (pe0_motion_value_t.avg_tnr_x_avg_t < pCVD_Pattern_Detection_t->tnr_x_avg_t_th) \
			&& (pe0_motion_value_t.avg_tnr_x_avg_s < pCVD_Pattern_Detection_t->tnr_x_avg_s_th) \
			&& (pe0_motion_value_t.avg_tpd_s_status > ( pCVD_Pattern_Detection_t->tpd_s_status_th - s_status_diff) ) )
		//	&& (pe0_motion_value_t.avg_tpd_s_status > pCVD_Pattern_Detection_t->tpd_s_status_th) )
	{ 	// no global motion ??? pattern ???
		if(pCVD_Pattern_Detection_t->global_motion_count < pCVD_Pattern_Detection_t->motion_count_max)  {
			pCVD_Pattern_Detection_t->global_motion_count++;
			CVD_DEBUG("Pattern diff [%d],  avg_t[0x%x], avg_s[0x%x], s_status[0x%x], count[%d]\n", pCVD_Pattern_Detection_t->global_motion_diff, pe0_motion_value_t.avg_tnr_x_avg_t, pe0_motion_value_t.avg_tnr_x_avg_s, pe0_motion_value_t.avg_tpd_s_status, pCVD_Pattern_Detection_t->global_motion_count);
		}
	}
	else {
		if( pCVD_Pattern_Detection_t->pattern_found == 1) {
			pCVD_Pattern_Detection_t->global_motion_count --;
			CVD_DEBUG("Pattern Not Found : diff [%d],  avg_t[0x%x], avg_s[0x%x], s_status[0x%x],  count[%d]\n", pCVD_Pattern_Detection_t->global_motion_diff,  pe0_motion_value_t.avg_tnr_x_avg_t,pe0_motion_value_t.avg_tnr_x_avg_s, pe0_motion_value_t.avg_tpd_s_status, pCVD_Pattern_Detection_t->global_motion_count);
		}
		else
			pCVD_Pattern_Detection_t->global_motion_count = 0;
	}

	if( (pCVD_Pattern_Detection_t->global_motion_count > pCVD_Pattern_Detection_t->motion_count_th) && (pCVD_Pattern_Detection_t->pattern_found == 0) ) {
		pCVD_Pattern_Detection_t->pattern_found = 1;
		CVD_DEBUG("Pattern Detected (global motion)\n");
	}
	else if( (pCVD_Pattern_Detection_t->global_motion_count < pCVD_Pattern_Detection_t->motion_count_th) &&  (pCVD_Pattern_Detection_t->pattern_found == 1) ) {
		pCVD_Pattern_Detection_t->global_motion_count = 0;
		pCVD_Pattern_Detection_t->pattern_found = 0;
		CVD_DEBUG("Pattern Not Detected (global motion)\n");
	}

	pCVD_Pattern_Detection_t->prev_motion_value = pCVD_Pattern_Detection_t->global_motion_value;

	return 0;

}

int CVD_H13Ax_Set_PQ_Mode(LX_AFE_CVD_PQ_MODE_T cvd_pq_mode)
{
	CVD_REG_PARAM_T	*pCVD_PE_Param_t;
	UINT32	size;
	static LX_AFE_CVD_PQ_MODE_T prev_cvd_pq_mode = LX_CVD_PQ_DEFAULT;

	CVD_DEBUG("@@@@@ CVD Set PQ Mode to [%d]@@@@@\n", cvd_pq_mode);

	/*
	   if(prev_cvd_pq_mode ==cvd_pq_mode) {
	   CVD_DEBUG("SAME CVD PE Param, bypass PE settings !\n");
	   return -1;
	   }
	 */

	switch(cvd_pq_mode)
	{
		case LX_CVD_PQ_NTSC_M:		// AV NTSC
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_ntsc_m_l_default;
			size	= sizeof(cvd_av_ntsc_m_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_NTSC_M_RF:	// RF NTSC
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_ntsc_m_l_default;
			size	= sizeof(cvd_rf_ntsc_m_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL:			// AV PAL
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_pali_l_default;
			size	= sizeof(cvd_av_pali_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_RF:		// RF PAL
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_pali_l_default;
			size	= sizeof(cvd_rf_pali_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_SECAM:		// AV SECAM
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_secam_l_default;
			size	= sizeof(cvd_av_secam_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_SECAM_RF:	// RF SECAM
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_secam_l_default;
			size	= sizeof(cvd_rf_secam_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_NTSC_443:		// AV NTSC 443
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_ntsc_443_l_default;
			size	= sizeof(cvd_av_ntsc_443_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_NTSC_443_RF:	// RF NTSC 443
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_ntsc_443_l_default;
			size	= sizeof(cvd_rf_ntsc_443_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_60:			// AV PAL 60
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_pal60_l_default;
			size	= sizeof(cvd_av_pal60_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_60_RF:		// RF PAL 60
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_pal60_l_default;
			size	= sizeof(cvd_rf_pal60_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_M:		// AV PAL M
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_palm_l_default;
			size	= sizeof(cvd_av_palm_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_M_RF:	// RF PAL M
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_palm_l_default;
			size	= sizeof(cvd_rf_palm_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_NC:		// AV PAL NC
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_av_palnc_l_default;
			size	= sizeof(cvd_av_palnc_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		case LX_CVD_PQ_PAL_NC_RF:	// RF PAL NC
			pCVD_PE_Param_t = (CVD_REG_PARAM_T *)cvd_rf_palnc_l_default;
			size	= sizeof(cvd_rf_palnc_l_default) / sizeof(CVD_REG_PARAM_T) ;
			break;
		default:
			AFE_PRINT("@@@ ERROR unknown color system [%d] @@@\n", cvd_pq_mode);
			return -1;
	}

	prev_cvd_pq_mode = cvd_pq_mode;

	CVD_H13Ax_Set_PE_Params(pCVD_PE_Param_t, size, cvd_pq_mode);


	return 0;

}

/* On Brazil (PAL-M) weak RF Signal, AGC gain sometimes became high value result in white picture.
   Disabling AGC is needed on this weak RF signal( No h-lock, high noise level )
   */
int CVD_H13Ax_AGC_Bypass_Function(UINT32 noise_status, UINT32 hlock_flag, UINT32 no_signal_flag )
{
	int ret = 0;
	static int bypass_enable = 0;

	OS_LockMutex(&g_CVD_Sema);
	//CVD_DEBUG("%s entered :  %d,%d,%d, %d \n",__func__, noise_status, hlock_flag, no_signal_flag, bypass_enable);

	if( ( noise_status == CVD_H13A0_NOISE_MAX) && ( hlock_flag == 0 )&& ( no_signal_flag == 0)) {
		if(bypass_enable == 0) {
			bypass_enable = 1;
			CVD_H13Ax_AGC_Bypass(bypass_enable);
			//CVD_H13Ax_Set_dcrestore_accum_width(0x1b);
			//111109 by kim.min for weak cable RF signals.
			CVD_H13Ax_Set_dcrestore_accum_width(DCRESTORE_ACCUM_WIDTH_FOR_WEAK_RF);
			CVD_DEBUG("## Enable AGC Bypass\n");
		}
	}
	else if(bypass_enable) {
		bypass_enable = 0;
		CVD_H13Ax_AGC_Bypass(bypass_enable);
		if(g_CVD_Status_3CS.cvd_prev_color_system < CVD_STATE_VideoNotReady)
			CVD_H13Ax_Set_dcrestore_accum_width(g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_prev_color_system].reg_dcrestore_accum_width);
		CVD_DEBUG("## Disable AGC Bypass\n");
	}
	/*
	   if(noise_status == CVD_H13A0_NOISE_MAX)
	   CVD_H13Ax_Set_dcrestore_accum_width(0x28);
	   else if(noise_status < 0x100)
	   CVD_H13Ax_Set_dcrestore_accum_width(0x22);
	 */
	OS_UnlockMutex(&g_CVD_Sema);
	return ret;
}

int CVD_H13Ax_Init_Global_Params(void)
{
	//for Pattern Detection Using Global Motion
	g_CVD_Pattern_Detection_t.bEnable = TRUE ;
	g_CVD_Pattern_Detection_t.prev_motion_value = 0 ;
	//	g_CVD_Pattern_Detection_t.motion_diff_th = 3 ;
	g_CVD_Pattern_Detection_t.motion_diff_th = 1 ;
	g_CVD_Pattern_Detection_t.motion_count_th = 40 ;
	g_CVD_Pattern_Detection_t.motion_count_max = 50 ;
	g_CVD_Pattern_Detection_t.global_motion_count = 0 ;
	g_CVD_Pattern_Detection_t.pattern_found = 0 ;
	//		g_CVD_Pattern_Detection_t.tnr_x_avg_t_th = 0x100 ;
	g_CVD_Pattern_Detection_t.tnr_x_avg_t_th = 0x30 ;
	g_CVD_Pattern_Detection_t.tnr_x_avg_s_th = 0x30 ;
	//g_CVD_Pattern_Detection_t.tnr_x_avg_s_th = 0x40 ;
	g_CVD_Pattern_Detection_t.tpd_s_status_th = 0x80 ;

	g_CVD_Status_3CS.cvd_initialized = 1;

	return 0;
}

int CVD_H13Ax_Set_for_Tunning(BOOLEAN bEnable)
{
	int ret = 0;

	OS_LockMutex(&g_CVD_Sema);

	if(bEnable == TRUE)
	{
		g_CVD_Status_3CS.in_atv_tunning = 1;
		CVD_H13Ax_Set_dcrestore_accum_width(DCRESTORE_ACCUM_WIDTH_DEFAULT);
		CVD_H13Ax_Set_Noise_Threshold(NOISE_THRESHOLD_DEFAULT);

		// restore default value of reg_hnon_std_threshold
#ifdef L9_FAST_3DCOMB_WORKAROUND
		CVD_H13Ax_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13A0[CVD_STATE_NTSC].reg_hnon_std_threshold);
#endif
#ifdef H13_FAST_3DCOMB_WORKAROUND
		//Enable vf_nstd_en when auto tuning
		CVD_H13Ax_vf_nstd_control(1);	
		// restore default value of reg_hnon_std_threshold
		CVD_H13Ax_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13A0[CVD_STATE_NTSC].reg_hnon_std_threshold);
#endif
	}
	else
	{
		g_CVD_Status_3CS.in_atv_tunning = 0;
		if(g_CVD_Status_3CS.cvd_prev_color_system < CVD_STATE_VideoNotReady)
			CVD_H13Ax_Set_dcrestore_accum_width(g_SetColorSystem_3CS_H13A0[g_CVD_Status_3CS.cvd_prev_color_system].reg_dcrestore_accum_width);
		CVD_H13Ax_Set_Noise_Threshold(NOISE_THRESHOLD_WEAK_RF);
	}
	OS_UnlockMutex(&g_CVD_Sema);
	return ret;
}

int CVD_H13Ax_Get_Reg_States_Detail(LX_AFE_CVD_STATES_DETAIL_T	*pcvd_states_detail_t)
{
	/* Read Status Registers */
	pcvd_states_detail_t->No_Signal_Flag = CVD_H13Ax_Get_No_Signal_Flag();
	pcvd_states_detail_t->HLock_Flag = CVD_H13Ax_Get_HLock_Flag();
	pcvd_states_detail_t->VLock_Flag = CVD_H13Ax_Get_VLock_Flag();
	pcvd_states_detail_t->VLine_625_Flag = CVD_H13Ax_Get_Vline_625_Flag();
	pcvd_states_detail_t->Pal_Flag = CVD_H13Ax_Get_PAL_Flag();
	pcvd_states_detail_t->Secam_Flag = CVD_H13Ax_Get_SECAM_Flag();
	pcvd_states_detail_t->Chromalock_Flag = CVD_H13Ax_Get_Chromalock_Flag();
	pcvd_states_detail_t->NoBurst_Flag = CVD_H13Ax_Get_NoBurst_Flag();
	pcvd_states_detail_t->FC_Flag = CVD_H13Ax_Get_FC_Flag(LX_CVD_MAIN);
	pcvd_states_detail_t->cvd_cordic_freq = CVD_H13Ax_Get_Cordic_Freq(LX_CVD_MAIN);
	pcvd_states_detail_t->status_noise = CVD_H13Ax_Get_Noise_Status();

	pcvd_states_detail_t->Pal_Flag_CS0 = CVD_H13Ax_Get_PAL_Flag_CS0();
	pcvd_states_detail_t->Secam_Flag_CS0 = CVD_H13Ax_Get_SECAM_Flag_CS0();
	pcvd_states_detail_t->Chromalock_Flag_CS0 = CVD_H13Ax_Get_Chromalock_Flag_CS0();
	pcvd_states_detail_t->FC_Flag_CS0 = CVD_H13Ax_Get_FC_Flag_CS0();
	pcvd_states_detail_t->cvd_cordic_freq_CS0 = CVD_H13Ax_Get_Cordic_Freq_CS0();

	pcvd_states_detail_t->Pal_Flag_CS1 = CVD_H13Ax_Get_PAL_Flag_CS1();
	pcvd_states_detail_t->Secam_Flag_CS1 = CVD_H13Ax_Get_SECAM_Flag_CS1();
	pcvd_states_detail_t->Chromalock_Flag_CS1 = CVD_H13Ax_Get_Chromalock_Flag_CS1();
	pcvd_states_detail_t->FC_Flag_CS1 = CVD_H13Ax_Get_FC_Flag_CS1();
	pcvd_states_detail_t->cvd_cordic_freq_CS1 = CVD_H13Ax_Get_Cordic_Freq_CS1();
	CVD_H13Ax_Get_PE0_Motion_Value(&pcvd_states_detail_t->tnr_x, &pcvd_states_detail_t->tnr_s, &pcvd_states_detail_t->s_status );
	pcvd_states_detail_t->motion_value = CVD_H13Ax_Get_Global_Motion_Value();
	pcvd_states_detail_t->agc_gain = CVD_H13Ax_Get_Status_AGC_Gain();
	pcvd_states_detail_t->cagc = CVD_H13Ax_Get_CAGC_Value();
	//pcvd_states_detail_t->saturation = CVD_H13Ax_Get_Saturation_Value();
	pcvd_states_detail_t->saturation = CVD_H13Ax_Get_oadj_c_coeff_value();
	pcvd_states_detail_t->hnon_standard = CVD_H13Ax_Get_HNon_Standard_Flag();
	pcvd_states_detail_t->vnon_standard = CVD_H13Ax_Get_VNon_Standard_Flag();
	pcvd_states_detail_t->burst_mag = CVD_H13Ax_Get_CVD_Burst_Mag_Value();
	pcvd_states_detail_t->vdetect_vcount = CVD_H13Ax_Get_Vdetect_Vcount_Value();
	pcvd_states_detail_t->agc_peak_en = CVD_H13Ax_Get_AGC_Peak_En_Value();
	pcvd_states_detail_t->agc_bypass = CVD_H13Ax_Get_AGC_Bypass_Value();
	pcvd_states_detail_t->status_clamp_updn = CVD_H13Ax_Get_status_clamp_updn();

	return 0;
}
