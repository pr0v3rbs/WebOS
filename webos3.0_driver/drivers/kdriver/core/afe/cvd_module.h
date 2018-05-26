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
 *  @author     dws
 *  @version    1.0
 *  @date       2010-11-19
 *  @note       Additional information.
 */

#ifndef	_AFE_LDRV_CVD_H_
#define	_AFE_LDRV_CVD_H_

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
#include "afe_kapi.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define H13A0_VBI_INTERRUPT_MASK	0x00000004
#define H14A0_VBI_INTERRUPT_MASK	0x00000004
#define M14A0_VBI_INTERRUPT_MASK	0x00000004
#define M14B0_VBI_INTERRUPT_MASK	0x00000004
#define H15A0_VBI_INTERRUPT_MASK	0x00000004

#define H13A0_VSYNC_INTERRUPT_MASK	0x08000000
#define H13B0_VSYNC_INTERRUPT_MASK	0x08000000
#define H14A0_VSYNC_INTERRUPT_MASK	0x08000000
#define M14A0_VSYNC_INTERRUPT_MASK	0x08000000
#define M14B0_VSYNC_INTERRUPT_MASK	0x08000000
#define H15A0_VSYNC_INTERRUPT_MASK	0x08000000

/* Cordiq Frequency definition */
//#define	FC_MORE_THRESHHOLD				(210 + 30) //by dws temp
//#define	FC_MORE_THRESHOLD				(210 + 20) //by dws 1211
//#define	FC_MORE_THRESHOLD				(210 + 40) //by dws 0503
#define	FC_MORE_THRESHOLD				(210) 	// by dws 111018
#define FC_LESS_THRESHOLD				60

#define GET_BITS(val,nd,wd)			(((val)>>(nd))&(0xFFFFFFFF>>(32-(wd))))
#define GET_PVAL(val,nd,wd)			(GET_BITS(val,nd,wd)<<(nd))
#define REG_WD( addr, value )	    ( *( volatile UINT32 * )( addr )) = ( UINT32 )( value )
#define REG_RD( addr )	            ( *( volatile UINT32 * )( addr ))

//#define L8_MAIN_CVD_INTERRUPT_MASK	0x1FFE
//#define L8_MAIN_CVD_INTERRUPT_MASK	0x1F9E
//#define L8_MAIN_CVD_INTERRUPT_MASK	0x0F8E
//#define L8_MAIN_CVD_INTERRUPT_MASK	0x1D8E
#define L8_MAIN_CVD_INTERRUPT_MASK	0x1DCE
#define L8_VBI_INTERRUPT_MASK	0x8000

//#define L9_CVD_INTERRUPT_MASK	0x00300068
//#define L9_CVD_INTERRUPT_MASK	0x00300048
//#define L9_CVD_INTERRUPT_MASK	0x00000048
#define L9_CVD_INTERRUPT_MASK	0x0
//#define L9_CVD_INTERRUPT_MASK	0x003000CC
#define L9_VBI_INTERRUPT_MASK	0x00000004
//#define L9_CVD_INTERRUPT_MASK	0x0
//#define L9_VBI_INTERRUPT_MASK	0x0
//#define L9_CVD_INTERRUPT_MASK	0x00000000
//#define L9_VBI_INTERRUPT_MASK	0x00000000
#define L9_CVD_INTERRUPT_PENDING 0x80000000

// #########################
// For 3DCOMB ERROR TEST ONLY
// #########################
#define H13_3DCOMB_ERROR_WORKAROUND
//#define H13_3DCOMB_TEST_TMP
// #########################
//#define H13_CDTO_CONTROL

//#define CVD_TEST_DWS
//#define H13_3DCOMB_ERROR_WORKAROUND

//120218 for faster 3Dcomb operation
//#define L9_FAST_3DCOMB_WORKAROUND
#define H13_FAST_3DCOMB_WORKAROUND

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
//#define	H13_COMB2D_ONLY_CONTROL
#undef	H13_FAST_3DCOMB_WORKAROUND
#endif

#define M14_3DCOMB_ERROR_WORKAROUND
#define M14_FAST_3DCOMB_WORKAROUND
#ifdef	M14_3DCOMB_ERROR_WORKAROUND
#undef	M14_FAST_3DCOMB_WORKAROUND
#endif

#define H14_3DCOMB_ERROR_WORKAROUND
#define H14_FAST_3DCOMB_WORKAROUND
#ifdef	H14_3DCOMB_ERROR_WORKAROUND
#undef	H14_FAST_3DCOMB_WORKAROUND
#endif

//#define H15_3DCOMB_ERROR_WORKAROUND
#define H15_3DCOMB_ERROR_WORKAROUND
#define H15_FAST_3DCOMB_WORKAROUND
#ifdef	H15_3DCOMB_ERROR_WORKAROUND
#undef	H15_FAST_3DCOMB_WORKAROUND
#endif
//#define H13_PAL_STABLE_VSYNC_WORKAROUND		// WSS slicing error
//#define H13_ENABLE_PALM_STABLE_SYNC_MODE
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	CVD_STATE_Default=0,	// CVD Register Default
	CVD_STATE_NTSC =1,
	CVD_STATE_NTSCj=2,
	CVD_STATE_NTSC443 =3,
	CVD_STATE_PALi =4,
	CVD_STATE_PALn =5,
	CVD_STATE_PALm =6,
	CVD_STATE_PALCn =7,
	CVD_STATE_PAL60 =8,
	CVD_STATE_SECAM =9,
	CVD_STATE_VideoNotReady,
	CVD_STATE_VideoReadyToDetect,
	CVD_STATE_StandardDetection,
	CVD_STATE_MAX_NUM,
}	CVD_STATE_T;

typedef enum {
	CVD_FC_LESS_FLAG = 0,
	CVD_FC_SAME_FLAG = 1,
	CVD_FC_MORE_FLAG = 2,
} CVD_FC_FLAG_T;

#if 1 //gogosing burst mag status check for color burst level test (color ¨¡©÷¨úiAu ¢¥eAA) 2011.06.11
typedef enum {
	CVD_BURST_MAG_STATE_SAME  		= 0 ,
	CVD_BURST_MAG_STATE_VERY_SMALL  = 1,
	CVD_BURST_MAG_STATE_SMALL 		= 2,
	CVD_BURST_MAG_STATE_BIG   		= 3,
}CVD_BURST_MAG_STATE_T;
#endif

typedef enum {
	CVD_3DCOMB_CONTROL_HW,
	CVD_3DCOMB_CONTROL_LF,
	CVD_3DCOMB_CONTROL_HF,
} CVD_3DCOMB_CONTROL_T;

typedef enum {
	CVD_3DCOMB_STATE_00	= 0x0,
	CVD_3DCOMB_STATE_10	= 0x10,
	CVD_3DCOMB_STATE_21	= 0x21,
	CVD_3DCOMB_STATE_22	= 0x22,
	CVD_3DCOMB_STATE_23	= 0x23,
	CVD_3DCOMB_STATE_30	= 0x30,
} CVD_3DCOMB_STATE_T;

typedef struct {
   UINT32 	reg_comb3_buffer_size;
   UINT32 	reg_fld1_init_rd_pel;
   UINT32 	reg_fld2_init_rd_pel;
   UINT32 	reg_fld3_init_rd_pel;
   UINT32 	reg_fld4_init_rd_pel;

   UINT32 	reg_comb3_buffer_size_L9Bx;
   UINT32 	reg_fld1_init_rd_pel_L9Bx;
   UINT32 	reg_fld2_init_rd_pel_L9Bx;
   UINT32 	reg_fld3_init_rd_pel_L9Bx;
   UINT32 	reg_fld4_init_rd_pel_L9Bx;

   UINT8	reg_cvd1_clampagc_on;
   //gogosing added 110613
   // SECAM threshold for keeping PAL stable 20110613
	UINT8 cs_issecam_th;

   //added 0716
   UINT8	reg_phase_offset_range;
   UINT8	reg_ispal_th;
   UINT8	reg_cordic_gate_start;
   UINT8	reg_cordic_gate_end;

   UINT8	reg_colour_mode;
   UINT8	reg_vline_625;
   UINT8	reg_hpixel;
   UINT8	reg_ped;
   UINT16	reg_adc_blank_level; 	//added 110411
   UINT8	reg_hagc_en;
   UINT8	reg_cagc_en;
   UINT8	reg_agc_half_en;
   UINT8	reg_dagc_en;
   UINT8	reg_dc_clamp_mode;
   UINT8	reg_mv_hagc_mode;
   UINT8	reg_hagc_field_mode;
   UINT8	reg_chroma_bw_lo;
   UINT8	reg_adaptive_mode;
   UINT8	reg_adaptive_3dcomb_mode;

	UINT8	reg_pal60_mode;

   UINT8	reg_hagc;
   UINT8	reg_adc_updn_swap;
   UINT8	reg_yc_delay;
   UINT8	reg_cagc_gate_start;
   UINT8	reg_cagc_gate_end;
   UINT32	reg_cdto_inc;
   UINT32	reg_hdto_inc;
   UINT8	reg_hactive_start;
   UINT8	reg_hactive_width;
   UINT8	reg_vactive_start;
   UINT8	reg_vactive_height;
   UINT8	reg_ntsc443_mode;
   UINT8	reg_cagc;
   UINT8	reg_burst_gate_end;
   UINT8	reg_secam_ybw;
   UINT8	reg_auto_secam_level;
   UINT8	reg_lose_chromalock_mode;
   UINT8	reg_noise_th;
   UINT8	reg_noise_th_en;
   UINT8	reg_lowfreq_vdiff_gain;
   UINT8	reg_chroma_vdiff_gain;
   UINT8	reg_horiz_diff_ygain;
   UINT8	reg_horiz_diff_cgain;
   UINT8	reg_y_noise_th_gain;
   UINT8	reg_c_noise_th_gain;
   UINT8	reg_burst_noise_th_gain;
   UINT8	reg_vadap_burst_noise_th_gain;
   UINT8	reg_motion_mode;
   UINT8	reg_adaptive_chroma_mode;
   UINT8	reg_comb2d_only;
   UINT8	reg_fb_sync;
   UINT8	reg_md_noise_th;
   UINT8	reg_md_noise_th_en;
   UINT8	reg_vactive_md_start;
   UINT8	reg_vactive_md_height;
   UINT8	reg_hactive_md_start;
   UINT8	reg_hactive_md_width;
   UINT8	reg_motion_config;
//   UINT8	reg_2dcomb_ycrdc_lcr_on;
   UINT8	reg_status_motion_mode;
   UINT8	reg_frame_motion_th;
   UINT8	reg_chroma_level;
   UINT8	reg_hf_luma_chroma_offset;
   UINT8	reg_tcomb_chroma_level;
   UINT8	reg_2dcomb_byp;
   UINT8	reg_2dcomb_cband_sel;
   UINT8	reg_2dcomb_ccombo_sel;
   UINT8	reg_2dcomb_ycrdc_cvsel;
   UINT8	reg_2dcomb_ycrdc_chsel;
   UINT8	reg_2dcomb_ycrdc_lrsel;
   UINT8	reg_2dcomb_divider_th;
   UINT16	reg_2dcomb_ycrdc_yhsymth;
   UINT16	reg_2dcomb_ycrdc_yhdiffth;
   UINT16	reg_2dcomb_ycrdc_cvdiffth;
   UINT8	reg_2dcomb_ycrdc_yvsel;
   UINT8	reg_2dcomb_ycrdc_yhsel;
   UINT8	reg_2dcomb_ycrdc_hth_on;
   UINT8	reg_2dcomb_ycrdc_hth_sel;
   UINT16	reg_2dcomb_ycrdc_hth;
   UINT8	reg_2dcomb_ycrdc_lrtype;
   UINT8	reg_2dcomb_ycrdc_dsel;
   UINT8	reg_2dcomb_ycrdc_lcr_on;
   UINT8	reg_2dcomb_ycrdc_method;
   UINT8	reg_2dcomb_cyrdc_crvalid;	//B0?
   UINT8	reg_2dcomb_cyrdc_lrtype;	//B0?
   UINT8	reg_2dcomb_cyrdc_dsel;		//B0?
   UINT16	reg_2dcomb_cyrdc_chsymth;	//B0?
   UINT16	reg_2dcomb_cyrdc_chsymth1;	//B0?
   UINT8	reg_2dcomb_cyrdc_method;	//B0?
   UINT8	reg_2dcomb_cyrdc_lcr_on;	//B0?

   //added 0906
   UINT16	reg_oadj_y_coeff;


   //added 0920
	UINT8	reg_hstate_max;				//B0?
	UINT8	reg_dcrestore_accum_width;	//B0?

	//added 1103
   UINT16	reg_oadj_y_offo;
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

   // for L9B0 vactive start adjust
	UINT8	reg_vactive_start_L9B0;

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
   //111230	: added reg_fb_height_half
   UINT8	reg_fb_height_half;

   //120105	: added reg_saturation for SECAM
   UINT8	reg_saturation;

   UINT8	reg_contrast_av;
   UINT8	reg_cagc_av;

   //120113 : for NTSC stable
//   UINT8	reg_hstate_fixed;
}	CVD_SET_SYSTEM_MAIN_T;

typedef struct {
	//	UINT8 CB_PDB; // chb cvd power down
	UINT8 chbreg_hv_delay;
	UINT8 chbreg_hpixel;
	UINT8 chbreg_vline_625;
	UINT8 chbreg_colour_mode;
	UINT8 chbreg_ntsc443_mode;
	UINT8 chbreg_luma_notch_bw;
	UINT8 chbreg_chroma_bw_lo;
	UINT8 chbreg_chroma_burst5or10;
	UINT8 chbreg_ped;
	//UINT8 chbreg_lbadrgen_rst; //chb cvd reset
	UINT8 chbreg_adaptive_mode;
	UINT8 chbreg_adaptive_chroma_mode;
	UINT8 chbreg_hagc;
	UINT8 chbreg_cagc;
	UINT32 chbreg_cdto_inc;
	UINT8 chbreg_dr_freq;
	UINT8 chbreg_db_freq;
	UINT8 chbreg_hactive_start;
	UINT8 chbreg_vactive_start;
	UINT8 chbreg_vactive_height;
	UINT8 chbreg_auto_secam_level;
	UINT8 chbreg_palsw_level;
	UINT8 chbreg_lose_chromalock_mode;
	UINT8 chbreg_cagc_tc_p;
	UINT8 chbreg_cagc_tc_ibig;
	UINT8 chbreg_cagc_tc_ismall;
	//UINT8 reg_swrst_chbcvd; //chb cvd reset
} CVD_SET_SYSTEM_CHB_T;

typedef struct {
	UINT32	cvd_phy_addr;
	UINT32	cvd_pe_value;
} CVD_REG_PARAM_T;

/**********************************************************************/
/************** L8Bx CVD PE Settings **********************************/
/**********************************************************************/

typedef struct {
	UINT8	reg_lpf_0_en;
	UINT16	reg_lpf_tap_0_0;
	UINT16	reg_lpf_tap_0_1;
	UINT16	reg_lpf_tap_0_2;
	UINT16	reg_lpf_tap_0_3;
	UINT16	reg_lpf_tap_0_4;
	UINT16	reg_lpf_tap_0_5;
	UINT16	reg_lpf_tap_0_6;
	UINT16	reg_lpf_tap_0_7;
	UINT16	reg_lpf_tap_0_8;
	UINT16	reg_lpf_tap_0_9;
	UINT16	reg_lpf_tap_0_10;
	UINT16	reg_lpf_tap_0_11;
	UINT16	reg_lpf_tap_0_12;
} CVD_L8Bx_SET_AAF_T;

typedef struct {
	UINT8	DR3P_M;
	UINT8	DR3P_NPC;
	UINT8	DR3P_NSC;
	UINT8	DR3P_OD_CVDCLK;
	UINT8	DR3P_OD_RECCLK;
	UINT8	DR3P_PS23C;
	UINT8	DR3P_CIH;
	UINT8	clksel_phy;
	UINT8	clksel_irisin;
	UINT8	clksel_linkapll;
	UINT8	reg_cres_clksel;
	UINT8	reg_cres_0px_sel;
	UINT16	reg_cres_blend_bcdefault;
	UINT8	reg_iris_cresampler_on;
	UINT8	reg_cres_lpfa_on;
	UINT16	reg_cres_lpfa_coeff0;
	UINT16	reg_cres_lpfa_coeff1;
	UINT16	reg_cres_lpfa_coeff2;
	UINT16	reg_cres_lpfa_coeff3;
	UINT16	reg_cres_lpfa_coeff4;
	UINT16	reg_cres_lpfa_coeff5;
	UINT8	reg_cres_lpfb_on;
	UINT16 	reg_cres_lpfb_coeff0;
	UINT16	reg_cres_lpfb_coeff1;
	UINT16	reg_cres_lpfb_coeff2;
	UINT16	reg_cres_lpfb_coeff3;
	UINT16	reg_cres_lpfb_coeff4;
	UINT8	reg_dtrs_revalpha_sel  ;
	UINT8	reg_dtrs_dlysel;
	UINT8	reg_cres_blend_method;
	UINT8	reg_cres_blend_sel;
}	CVD_L8Bx_SET_CRES54MHZ_T;

typedef struct {
	UINT8	reg_2dcomb_byp;
	UINT8	reg_2dcomb_cband_sel;
	UINT8	reg_2dcomb_ccombo_sel;
	UINT8	reg_2dcomb_ycrdc_cvsel;
	UINT8	reg_2dcomb_ycrdc_chsel;
	UINT8	reg_2dcomb_ycrdc_lrsel;

	UINT16	reg_2dcomb_ycrdc_yhsymth;
	UINT16	reg_2dcomb_ycrdc_yhdiffth;

	UINT16 	reg_2dcomb_ycrdc_cvdiffth;

	UINT8 	reg_2dcomb_ycrdc_yvsel;
	UINT8 	reg_2dcomb_ycrdc_yhsel;
	UINT8 	reg_2dcomb_ycrdc_hth_on;
	UINT16 	reg_2dcomb_ycrdc_hth_sel;
	UINT8 	reg_2dcomb_ycrdc_hth;
	
	UINT8 	reg_2dcomb_ycrdc_lrtype;
	UINT8 	reg_2dcomb_ycrdc_dsel;
	UINT8 	reg_2dcomb_ycrdc_lcr_on;
	UINT16 	reg_2dcomb_ycrdc_method;

	UINT16 	reg_2dcomb_ycrdc_yvdiv_th0;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th1;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th2;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th3;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th4;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th5;
	UINT16	reg_2dcomb_ycrdc_yvdiv_th6;
}	CVD_L8Bx_SET_DOT_CRAWL_T;

typedef struct {
	UINT8	reg_2dcomb_cyrdc_method;
	UINT8	reg_2dcomb_cyrdc_lcr_on;
	UINT8	reg_2dcomb_cyrdc_crvalid;
	UINT8	reg_2dcomb_cyrdc_lrtype;
	UINT8	reg_2dcomb_cyrdc_dsel;
	UINT8	reg_2dcomb_cyrdc_dsel1;

	UINT16	reg_2dcomb_cyrdc_chsymth;
	UINT16	reg_2dcomb_cyrdc_chsymth1;
	
	UINT8	reg_2dcomb_cyrdc_hvth;
	UINT8	reg_2dcomb_cyrdc_cvdiv_xsel;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th0;
	
	UINT16	reg_2dcomb_cyrdc_cvdiv_th1;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th2;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th3;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th4;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th5;
	UINT16	reg_2dcomb_cyrdc_cvdiv_th6;
	UINT16	reg_2dcomb_cyrdc_chdiffth;

	UINT8	reg_2dcomb_cyrdc_chdiffon;
	UINT8	reg_2dcomb_cyrdc_chdiffsel;

	UINT16	reg_2dcomb_cyrdc_dir_on;
	UINT16	reg_2dcomb_cyrdc_v0b1_dinsel;

	UINT8	reg_2dcomb_cyrdc_v1b4_cinsel;
	UINT8	reg_2dcomb_cyrdc_v1b4_dinsel;
	UINT8	reg_2dcomb_cyrdc_v1b4_th;

	UINT8	reg_2dcomb_cyrdc_hth_on;
	UINT16	reg_2dcomb_cyrdc_hth;

} 	CVD_L8Bx_SET_CROSS_COLOR_REDUCER_T;

typedef struct {
	UINT8	reg_motion_mode;
	UINT8	reg_dot_suppress_mode;
	UINT8	reg_adaptive_chroma_mode;
	UINT8	reg_motion_config;
	UINT8	reg_status_motion_mode;
	UINT8	reg_flat_luma_shift;
	UINT8	reg_flat_chroma_shift;
	UINT8	reg_flat_luma_offset;
	UINT8	reg_flat_chroma_offset;
	UINT16	reg_frame_motion_th0;
	UINT16	reg_frame_motion_th1;
	UINT8	reg_chroma_level;
	UINT8	reg_tcomb_chroma_level;

	UINT8	reg_3dcomb_mdy_disel1;
	UINT8	reg_3dcomb_mdglo_mode;
	UINT8	reg_3dcomb_mdglo_m1_lth0;
	UINT8	reg_3dcomb_mdglo_m1_uth0;
	UINT8	reg_3dcomb_mdglo_m1_sameth;

}	CVD_L8Bx_SET_MOTION_DETECTOR_T;

typedef struct {
	UINT8	reg_3dcomb_md_mode;
	UINT8	reg_3dcomb_mdy_dir;
	UINT8	reg_3dcomb_mdy_disel;
	UINT8	reg_3dcomb_mdy_sel;
	UINT8	reg_3dcomb_mdlf_actv_sel;
	UINT8	reg_3dcomb_mdlf_actv_th;
	UINT8	reg_3dcomb_mdhf_actv_th;
	UINT8	reg_3dcomb_mdlf_blk_th;
	UINT8	reg_3dcomb_mdhf_blk_th;
	UINT8	reg_3dcomb_mdclass_csel;
	UINT8	reg_3dcomb_mdclass_dsel;
	UINT8	reg_3dcomb_mdclass_fldsel;
	UINT16	reg_3dcomb_mdlf_fld_th0;
	UINT16	reg_3dcomb_mdlf_fld_th1;
	UINT16	reg_3dcomb_mdlf_fld_th2;
	UINT16	reg_3dcomb_mdlf_fld_th3;
	UINT8	reg_3dcomb_mdhf_fld_sel;
	UINT16	reg_3dcomb_mdhf_fld_th0;
	UINT16	reg_3dcomb_mdhf_fld_th1;
	UINT16	reg_3dcomb_mdhf_fld_th2;
	UINT16	reg_3dcomb_mdhf_fld_th3;
	UINT16	reg_3dcomb_mdhlf_fld_sub_th0;
	UINT8	reg_3dcomb_mdlf_fld_grd;
	UINT8	reg_3dcomb_mdhf_fld_grd;
	UINT32	reg_3dcomb_mdlf_fld_grd_alp;
	UINT32	reg_3dcomb_mdlf_fld_grd_alp__2;
	UINT32	reg_3dcomb_mdhf_fld_grd_alp;
	UINT32	reg_3dcomb_mdhf_fld_grd_alp__2;
//	UINT8	reg_3dcomb_coefi_pow;
//	UINT16	reg_3dcomb_coefi;
//	UINT32	reg_3dcomb_coefi__2;
//	UINT32	reg_3dcomb_coefi__3;
	UINT8	reg_3dcomb_mdpd32_on;
	UINT8	reg_3dcomb_mdpd32_sel;
	UINT32	reg_3dcomb_mdpd32_th;
}	CVD_L8Bx_SET_GLOBAL_CLASSIFIER_T;

typedef struct {
	UINT8	reg_3dcomb_coefi_pow;
	UINT16	reg_3dcomb_coefi;
	UINT32	reg_3dcomb_coefi__2;
	UINT32	reg_3dcomb_coefi__3;
}	CVD_L8Bx_SET_BLENDING_T;

typedef struct {
	UINT8	reg_hrs_alpha_yoff;
	UINT8	reg_hrs_alpha_cboff;
	UINT8	reg_hrs_alpha_croff;
	//added 110126
	UINT8	reg_yc_delay;
//	UINT8	reg_ycadj_cr_p;
	UINT8	reg_cb_delay;
}	CVD_L8Bx_SET_OUPUT_DELAY_T;

typedef struct {
	UINT16	reg_oadj_y_offi;
	UINT16	reg_oadj_y_offo;
	UINT16	reg_oadj_y_coeff;
	UINT16	reg_oadj_c_offi;
	UINT16	reg_oadj_c_offo;
	UINT16	reg_oadj_c_coeff;
}	CVD_L8Bx_SET_OUPUT_RANGE_T;

/**********************************************************************/

#if 0
	CVD_STATE_T	detected_color_systeme;
	/* pdb */
	UINT32		comb3_buffer_size;		//SIC added
	UINT32		fld1_int_rd_pel;		//SIC added
	UINT32		fld2_int_rd_pel;		//SIC added
	UINT32		fld3_int_rd_pel;		//SIC added
	UINT32		fld4_int_rd_pel;		//SIC added
	UINT8		color_mode;
	UINT8		vline_625;
	UINT8		hpixel;
	UINT8		ped;
	UINT8		hagc_en;				//SIC added
	UINT8		cagc_en;				//SIC added
	UINT8		agc_half_en;			//SIC added
	UINT8		dagc_en;				//SIC added
	UINT8		dc_clamp_mode;			//SIC added
	UINT8		mv_hagc_mode;			//SIC added
	UINT8		hagc_field_mode;		//SIC added
	UINT8		chroma_bw_lo;			//SIC added
	UINT8		adaptive_mode;
	UINT8		adaptive_3dcomb_mode;	//SIC
	UINT8		hagc;					//SIC
	UINT8		adc_updn_swap;			//SIC
	UINT8		cagc_gate_start;		//SIC
	UINT8		cagc_gate_end;			//SIC
	UINT32		cdto_inc;
	UINT32		hdto_inc;				//SIC
	UINT8		hactive_start;
	UINT8		hactive_width;			//SIC
	UINT8		vactive_start;
	UINT8		vactive_height;
	UINT8		ntsc443_mode;
	UINT8		cagc;
	UINT8		burst_gate_end;
	UINT8		burst_gate_start;		//zoran
	UINT8		hagcval_macrovision;	//zoran only
	UINT8		hagcval_normal;			//zoran only
	UINT8		yc_delay;				//zoran
	UINT8		secam_ybw;				//SIC only
	UINT8		auto_secam_level;		//SIC only
	UINT8		noise_th;				//SIC only
	UINT8		noise_th_en;			//SIC only
	UINT8		lowfreq_vdiff_gain		//SIC only
	UINT8		chroma_vdiff_gain 		//SIC only
	UINT8		horiz_diff_ygain  		//SIC only
	UINT8		horiz_diff_cgain  		//SIC only
	UINT8		y_noise_th_gain   		//SIC only
	UINT8		c_noise_th_gain   		//SIC only
	UINT8		burst_noise_th_gain		//SIC only
	UINT8		vadap_burst_noise_th_gain	//SIC only
	UINT8		motion_mode				//SIC only
	UINT8		adaptive_chroma_mode	//SIC only
	UINT8		comb2d_only
	UINT8		fb_sync					//SIC only
	UINT8		md_noise_th				//SIC only
	UINT8		md_noise_th_en				//SIC only
	UINT8		2dcomb_ycrdc_lcr_on		//SIC only
	/*swrst */
}	CVD_SET_SYSTEM_T;
#endif

typedef struct {
	UINT8	no_signal_flag;
	UINT8	h_lock_flag;
	UINT8	v_lock_flag;
	UINT8	vline_625_flag;
	UINT16	status_noise;
	UINT16	avg_status_noise;
	UINT16	status_agc_gain;
	UINT16	avg_status_agc_gain;
	int		noisy_signal_detected;
	UINT8	no_burst_flag;

	UINT8	cvd_fc_flag;
	UINT16	cvd_cordic_freq;	///< cordic_freq + 128
	UINT8	cvd_secam_flag;
	UINT8	cvd_pal_flag;
	UINT8	cvd_chromalock_flag;

	UINT8	cs0_fc_flag;
	UINT16	cs0_cordic_freq;	///< cordic_freq + 128
	UINT8	cs0_secam_flag;
	UINT8	cs0_pal_flag;
	UINT8	cs0_chromalock_flag;

	UINT8	cs1_fc_flag;
	UINT16	cs1_cordic_freq;	///< cordic_freq + 128
	UINT8	cs1_secam_flag;
	UINT8	cs1_pal_flag;
	UINT8	cs1_chromalock_flag;

	CVD_STATE_T	cvd_color_system;
	CVD_STATE_T	cs0_color_system;
	CVD_STATE_T	cs1_color_system;

	UINT8	in_color_system_detecting;	///< cvd in color system detection process
	UINT8	color_system_detected;		///< color system detected
	CVD_STATE_T	cvd_next_state;
	CVD_STATE_T	cs0_next_state;
	CVD_STATE_T	cs1_next_state;
	UINT8	no_color_detected;
	UINT8	hnon_standard_flag;
	UINT8	vnon_standard_flag;
	CVD_STATE_T	cvd_prev_color_system;
	CVD_STATE_T	cvd_prev_state;

	UINT8	in_atv_tunning;
	BOOLEAN	in_rf_mode;
	BOOLEAN prev_av_rf_mode;
	BOOLEAN port_change_detected;
	BOOLEAN cvd_ntsc_only_mode;
	UINT32	cvd_lock_stable_count;
	UINT32	cvd_no_signal_count;
	UINT8	afe_peak_en;
	UINT32	cvd_burst_mag;
	UINT8	vline_625_reg;
	UINT8	vline_625_stable;
	UINT8	use_internal_demod;
	UINT8	cvd_initialized;
	UINT8	burst_gate_modified;	/// < burst_gate_start/end modified (2:0x2a,0x49, 1:0x30,0x49, 0:default)

	LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	color_system_support;
	LX_AFE_CVD_BLACK_LEVEL_T cvd_black_level;
} CVD_STATUS_3CS_T;

typedef enum {
	CVD_SEL_CS_CVD = 0,
	CVD_SEL_CS_CS0 = 1,
	CVD_SEL_CS_CS1 = 2,
} CVD_SELECT_CDETECT_T;

typedef struct {
	BOOLEAN bEnable;
	int	white_ratio_th;
	int	white_frame_max_th;
	int	white_frame_on;
	int	white_frame_off;
	int print_en;
} CVD_AGC_PEAK_CONTROL_T;

typedef struct {
	BOOLEAN bEnable;
	int	tnr_x_avg_t_th;
	int	tnr_x_avg_s_th;
	int	tpd_s_status_th;
	int pattern_frame_max_th;
	int pattern_frame_on;
	int pattern_frame_off;
	int pattern_update;
	int pattern_frame_cnt;
	int reg_agc_peak_nominal;
	int reg_contrast;
	int reg_brightness;
	int pattern_found;
	int print_en;
	int noise_th;
} CVD_ADAPTIVE_PEAK_NOMINAL_CONTROL_T;

typedef struct {
	int	tnr_x_avg_t[4];
	int	tnr_x_avg_s[4];
	int	tpd_s_status[4];
	int	noise_status[16];
	int	avg_tnr_x_avg_t;
	int	avg_tnr_x_avg_s;
	int	avg_tpd_s_status;
	int	avg_noise_status;
} CVD_PE0_MOTION_VALUE_T;

typedef struct {
	BOOLEAN bEnable;
	int global_motion_value;
	int global_motion_diff;
	int prev_motion_value;
	int	motion_diff_th;
	int	motion_count_th;
	int motion_count_max;
	int global_motion_count;
	int pattern_found;
	int print_en;
	int tnr_x_avg_t_th;
	int tnr_x_avg_s_th;
	int tpd_s_status_th;
	int colorbar_diff_sum_th;
	int	static_pattern_found;
	int	static_pattern_count;
} CVD_PATTERN_DETECTION_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

int CVD_Init(LX_AFE_CVD_SELECT_T select_main_sub);
void CVD_UnInit(void);
int CVD_Program_Color_System(LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T program_color_system_t);
int CVD_Set_NTSC_Only(UINT32 arg);
int CVD_Set_Color_System(UINT32 arg);
void CVD_Get_Timing_Info(UINT32 arg);
int CVD_Start_Timer(UINT32 arg);
int CVD_Stop_Timer(UINT32 arg);
int CVD_Get_States(UINT32 arg);
int CVD_Picture_Enhancement_Test(LX_AFE_CVD_TEST_PARAM_T *stCVD_Test_Param);
int CVD_Set_SW_3DCOMB_Control(UINT32 arg);
int CVD_Enable_Status_Fld_Print(BOOLEAN  bEnable);
int CVD_Enable_3DCOMB_State_Print(BOOLEAN  bEnable);
int CVD_Set_Source_Type_Control(LX_AFE_CVD_SET_INPUT_T	cvd_input_info);
int CVD_Get_States_Detail(UINT32 arg);
int CVD_Enable_Detection_Interrupt(int enable);
int CVD_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_T cvd_black_level);
int CVD_Set_Use_Internal_Demod(BOOLEAN bInternal_Mode);
#ifdef	KDRV_CONFIG_PM
int CVD_RunSuspend(void);
int CVD_RunResume(void);
#endif


//	function pointer
//extern void (*CVD_Program_Color_System_Main_Multi)(CVD_SET_SYSTEM_MAIN_T *pSet_system_t);
int CVD_SW_Reset(LX_AFE_CVD_SELECT_T select_main_sub);
int CVD_Power_Down(LX_AFE_CVD_SELECT_T select_main_sub, BOOLEAN PowerOnOFF);
int CVD_Set_Source_Type(LX_AFE_CVD_SET_INPUT_T	cvd_input_info);
int CVD_Clamp_Current_Control(UINT8 value);

int CVD_Set_Scart_Overlay(BOOLEAN bEnable);
int CVD_Reg_Init(LX_AFE_CVD_SELECT_T select_main_sub);
int CVD_Get_Scart_AR(LX_AFE_SCART_AR_INFO_T	*pScart_ar_param);
int CVD_Vport_Reg_Read(LX_AFE_REG_RW_T *pReg_addr_data_t);
int CVD_Vport_Reg_Write(LX_AFE_REG_RW_T *pReg_addr_data_t);
int CVD_Get_FB_Status(LX_AFE_SCART_MODE_T *pScart_fb_mode);

int CVD_Channel_Power_Control(UINT32 on_off) ;

int CVD_Bypass_Control(LX_AFE_CVD_BYPASS_CONTROL_T *cvd_bypass_control_t);

int CVD_Set_Hstate_Max(UINT32	hstate_max_value);
int CVD_Set_for_Tunning(BOOLEAN bEnable);
int CVD_Set_PQ_Mode(LX_AFE_CVD_PQ_MODE_T cvd_pq_mode);
int CVD_Get_Crunky_Status(LX_AFE_CVD_CK_T *pCK_Detection_t);
int CVD_Set_Scart_FB_En(int fb_en_ctrl);
int CVD_OnOff_VDAC(BOOLEAN bonoff);

int	CVD_Get_Noise_Status(void);
int CVD_Test_Program(LX_AFE_CVD_TEST_PARAM_T *stCVD_Test_Param);

int	CVD_Channel_Change_Control(BOOLEAN bATV_Channel_Change);
int CVD_VDAC_mute_control(int enable);

int CVD_Set_Internal_Demod_Mode(BOOLEAN bInternal_Demod);
int CVD_Set_Blue_Mode(int blue);
int CVD_OnOff_DMD_DAC(BOOLEAN bonoff);
int CVD_Set_Blend_Ratio(int value);

int CVD_Get_HLock_Flag(void);
int CVD_Get_VLock_Flag(void);

int CVD_Set_Vsync_Enhance(int system);
int CVD_Set_Hsync_Enhance(int enable);
int CVD_Get_Vline_625_Flag(void);

int CVD_SetInterrupt(int on_off);

int CVD_Get_HNon_Standard_Flag(void);
int CVD_Get_VNon_Standard_Flag(void);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

