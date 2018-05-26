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
#include "cvd_task_h13b0.h"
#include "cvd_control_h13b0.h"
#include "cvd_hw_h13b0.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define BURST_GATE_ADJUST_FOR_BURST_POSITION_SHIFT
#define L9_ADAPTIVE_BURST_GATE_END_MODE

#undef CVD_USE_NO_BURST_FLAG

//Workaround for Field Stream(양천구 신월동 11채널 영상 Sync흔들림)
//Set hstate_fixed to '1' on NTSC RF signal
#define L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC

#define	SYSTEM_DETECTION_BY_CORDIC_ONLY
//#define	SYSTEM_DETECTION_BY_CORDIC_CS0_CS1

// Workaround for stable AGC on EBC field stream
//Disable AGC WA for H13B0 Revision
//#define L9_WA_FOR_EBS_STABLE_AGC
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
#ifdef H13_CDTO_CONTROL
#define	CVD_STABLE_COUNT_1	3
#else
#define	CVD_STABLE_COUNT_1	2
#endif
#else
#define	CVD_STABLE_COUNT_1	1
#endif
#define	CVD_STABLE_COUNT_20	20

//Workaround for PERU noisy field signal(low freq cut-off, status_noise is 0x3ff), flickering and unstable sync
//Disable AGC WA for H13B0 Revision
//#define L9_AGC_BYPASS_WA_FOR_NOISY_SIGNAL

//Workaround for Field Stream(대전 대덕구 화면 펄럭임발생 문제 : 수직 동기 떨림 빈도 100%)
//Set vsync_cntl 0x2, vsync_cntl_noisy 0x1, vloop_tc 0x3
//#define L9_VSYNC_CNTL_FOR_STABLE_VSYNC

// Workaround for DONG_GO_DONG_RAK stream :	20120208
#define L9_CHANGE_FIELD_DETECT_MODE_ON_DONG_GO_DONG_RAK

//Disable AGC WA for H13B0 Revision
//#define L9_ADAPTIVE_AGC_PEAK_EN_CONTROL
#define H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
//#define L9_DONT_KILL_CHROMALOCK_ON_CLOCK_LOST
#define L9_BYPASS_AGC_ON_ADAPTIVE_BURST_GATE_END_MODE

#define CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD	8	// 10 to 8 : 140428

#define CVD_ADAPTIVE_BURST_GATE_COUNT_ITERATION	20	// 20 is about 1 sec.
#define CVD_ADAPTIVE_BURST_GATE_THRESHOLD_0	0x3D090		// Threshold for enter CDTO value(hex value of 250K)
#define CVD_ADAPTIVE_BURST_GATE_THRESHOLD_1	0xCE4			// Threshold for CORDIC FREQ VALUE *20 (hex value of 3.3k)
#define CVD_ADAPTIVE_BURST_GATE_THRESHOLD_3	0x27100		// Threshold for leave CDTO value(hex value of 160K)
#define CVD_STABLE_COUNT_FOR_AGC_CONTROL	20

#define FAST_VLINE625_DETECT

#define CVD_BLACK_LEVEL_CONTROL

#define	H13_VDCT_WA_FOR_FAST_VLINE_DETECTION
#define	H13_HDCT_WA_FOR_HLOCK_ERROR

#define H13_CVD_VFREQ_STABLE_COUNT	5
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
//extern CVD_STATE_T	Current_State ;
//extern CVD_STATE_T	Next_State ;
//extern LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T g_CVD_Color_System_Support;
//extern int		gAutoProgramming;
//extern BOOLEAN gPortChangeDetected;
extern CVD_PATTERN_DETECTION_T g_CVD_Pattern_Detection_t ;
//extern BOOLEAN g_CVD_RF_Input_Mode;
extern CVD_SET_SYSTEM_3CS_H13B0_T g_SetColorSystem_3CS_H13B0[];
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static CVD_STATE_T CVD_H13Bx_Check_Color_System_Support(LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	forced_color_system);

static UINT32 g_mute_off_count = 0;
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#ifdef L9_WA_FOR_EBS_STABLE_AGC
static int g_Stable_AGC_WA_for_EBS_Enabled = 0;
#endif

//static UINT8 MainCVD_Same_State_Count=0;
/*========================================================================================
	Implementation Group
========================================================================================*/

int	CVD_H13Bx_Task_3cdetect(void)
{
#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
	static UINT32 gVariance_Status_Cdto_Inc = 0;
	static int	  gAdaptive_Burst_Gate_Counter = 0;
	int	Adaptive_Burst_Gate_Enable = 0;
	static int Prev_Adaptive_Burst_Gate_Enable = 0;
	static UINT32 gAccumulated_Cordic_Freq_Value = 0;
#endif
	static UINT8	No_Signal_Flag, HV_Lock_Flag;
	//static UINT8	No_Signal_Count = 0;

//	static UINT8	 MainCVD_No_Signal = 0;
	//static UINT8	 MainCVD_Prev_Written_Color_System = 0;
//	static BOOLEAN	 MainCVD_Is_Prev_RF_Input = FALSE;

	//	static UINT8 MainCVD_State_Transition_Count=0;
	//AFE_PRINT("In MainCVD Work\n");
	//CVD_DEBUG("MainCVD State = [%d]\n ", g_CVD_Status_3CS.cvd_color_system);
	//static	UINT32 Same_State_Count = 0;
	static UINT32	cvd_noise_sum	= 0;
	//	static UINT32	chromalock_count	= 0;
	unsigned int avg_noise_level;
#ifdef L9_AGC_BYPASS_WA_FOR_NOISY_SIGNAL
	static int stable_state2_count = 0;
#endif

	static UINT8 burst_mag_prev = 0, burst_mag_next = 0;
	static UINT32 burst_mag_array[10];
	UINT32 burst_mag_sum = 0;
	UINT32 burst_mag_avg;
	static UINT32 prev_burst_mag_avg ;
	static UINT32 cagc_target ;
	static UINT32 cagc_written ;

	static UINT16	status_noise_array[8] = {0,};
	static UINT16	status_agc_gain_array[8] = {0,};
	static UINT16	vline_625_array[H13_CVD_VFREQ_STABLE_COUNT] = {0,};

	//LX_AFE_CVD_PQ_MODE_T	cvd_pe_color_system;
	int loop, avg_tmp = 0;
	int	agc_gain_tmp = 0;
	int vline_625_sum = 0;
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
	static UINT8	vcr_detect_count;
	static int 		gHstateMaxCounter = 0;
#endif
	CVD_STATE_T	check_supporting_color_system;
#ifdef H13_COMB2D_ONLY_CONTROL
//	UINT32 rbuf1_empty, rbuf2_empty, rbuf3_empty, rbuf4_empty, wbuf_ful, wbuf_empty;
#endif
	static int prev_black_level = -1;

//	CVD_H13Bx_Check_CVD_Memory();

#ifdef FAST_VLINE625_DETECT
	CVD_H13Bx_Fast_Vline_625_Using_Vdetec_Vcount();
#endif
	/* Read Status CVD Registers */
	g_CVD_Status_3CS.no_signal_flag = CVD_H13Bx_Get_No_Signal_Flag();
	g_CVD_Status_3CS.h_lock_flag = CVD_H13Bx_Get_HLock_Flag();
	g_CVD_Status_3CS.v_lock_flag = CVD_H13Bx_Get_VLock_Flag();
	// ORing hlock & vlock makes frequent state transition to NO_SIGNAL_STATE on noisy signal.
	HV_Lock_Flag = g_CVD_Status_3CS.h_lock_flag && g_CVD_Status_3CS.v_lock_flag;
	// So ANDing hlock & vlock will be used for cheking cvd lock state.
	//HV_Lock_Flag = g_CVD_Status_3CS.h_lock_flag || g_CVD_Status_3CS.v_lock_flag;
	g_CVD_Status_3CS.vline_625_flag = CVD_H13Bx_Get_Vline_625_Flag();
	g_CVD_Status_3CS.cvd_pal_flag = CVD_H13Bx_Get_PAL_Flag();
	g_CVD_Status_3CS.cvd_secam_flag = CVD_H13Bx_Get_SECAM_Flag();//gogosing
	g_CVD_Status_3CS.cvd_chromalock_flag = CVD_H13Bx_Get_Chromalock_Flag();
	g_CVD_Status_3CS.status_noise = CVD_H13Bx_Get_Noise_Status();
	g_CVD_Status_3CS.no_burst_flag = CVD_H13Bx_Get_NoBurst_Flag();
	g_CVD_Status_3CS.cvd_fc_flag = CVD_H13Bx_Get_FC_Flag(LX_CVD_MAIN);
	g_CVD_Status_3CS.cvd_cordic_freq = CVD_H13Bx_Get_Cordic_Freq(LX_CVD_MAIN);
	g_CVD_Status_3CS.cvd_cordic_freq += CVD_H13Bx_Get_Cordic_Freq(LX_CVD_MAIN);
	g_CVD_Status_3CS.cvd_cordic_freq += CVD_H13Bx_Get_Cordic_Freq(LX_CVD_MAIN);
	g_CVD_Status_3CS.cvd_cordic_freq += CVD_H13Bx_Get_Cordic_Freq(LX_CVD_MAIN);
	g_CVD_Status_3CS.cvd_cordic_freq = (g_CVD_Status_3CS.cvd_cordic_freq>>2);
	//g_CVD_Status_3CS.cvd_cordic_freq += CVD_Get_Cordic_Freq(LX_CVD_MAIN);
	//g_CVD_Status_3CS.cvd_cordic_freq = g_CVD_Status_3CS.cvd_cordic_freq /5;
		g_CVD_Status_3CS.status_agc_gain = CVD_H13Bx_Get_Status_AGC_Gain();
		g_CVD_Status_3CS.hnon_standard_flag = CVD_H13Bx_Get_HNon_Standard_Flag();
		g_CVD_Status_3CS.vnon_standard_flag = CVD_H13Bx_Get_VNon_Standard_Flag();
	/*
	   if(g_CVD_Status_3CS.in_atv_tunning == FALSE)
	   CVD_AGC_Bypass_Function((UINT32)g_CVD_Status_3CS.status_noise, (UINT32)g_CVD_Status_3CS.h_lock_flag, (UINT32)g_CVD_Status_3CS.no_signal_flag );
	 */
	g_CVD_Status_3CS.vline_625_reg = CVD_H13Bx_Get_Vline_625_Reg();

	for(loop = (H13_CVD_VFREQ_STABLE_COUNT - 1) ;loop > 0;loop--) {
		vline_625_array[loop] = vline_625_array[loop-1] ;
		vline_625_sum += vline_625_array[loop];
	}
	vline_625_array[0] = g_CVD_Status_3CS.vline_625_reg;
	vline_625_sum += vline_625_array[0];

	if(vline_625_sum >= H13_CVD_VFREQ_STABLE_COUNT )	// stable 625 line : 50Hz
		g_CVD_Status_3CS.vline_625_stable = 1;
	else if(vline_625_sum == 0 )	// stable 525 line : 60Hz
		g_CVD_Status_3CS.vline_625_stable = 0;

	AFE_TRACE("vline_625_reg[%d], vline_625_sum[%d] , vline_625_stable[%d] \n", g_CVD_Status_3CS.vline_625_reg, vline_625_sum, g_CVD_Status_3CS.vline_625_stable);

	for(loop = 7;loop > 0;loop--) {
		status_noise_array[loop] = status_noise_array[loop-1] ;
		avg_tmp += status_noise_array[loop];
	}
	status_noise_array[0] = g_CVD_Status_3CS.status_noise;
	avg_tmp += status_noise_array[0];

	g_CVD_Status_3CS.avg_status_noise = avg_tmp >> 3;

	for(loop = 7;loop > 0;loop--) {
		status_agc_gain_array[loop] = status_agc_gain_array[loop-1] ;
		agc_gain_tmp += status_agc_gain_array[loop];
	}
	status_agc_gain_array[0] = g_CVD_Status_3CS.status_agc_gain;
	agc_gain_tmp += status_agc_gain_array[0];

	g_CVD_Status_3CS.avg_status_agc_gain = agc_gain_tmp >> 3;

	g_CVD_Status_3CS.cs0_pal_flag = CVD_H13Bx_Get_PAL_Flag_CS0();
	g_CVD_Status_3CS.cs0_secam_flag = CVD_H13Bx_Get_SECAM_Flag_CS0();//gogosing
	g_CVD_Status_3CS.cs0_chromalock_flag = CVD_H13Bx_Get_Chromalock_Flag_CS0();
	g_CVD_Status_3CS.cs0_fc_flag = CVD_H13Bx_Get_FC_Flag_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq = CVD_H13Bx_Get_Cordic_Freq_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS0();
	g_CVD_Status_3CS.cs0_cordic_freq = g_CVD_Status_3CS.cs0_cordic_freq /5;

	g_CVD_Status_3CS.cs1_pal_flag = CVD_H13Bx_Get_PAL_Flag_CS1();
	g_CVD_Status_3CS.cs1_secam_flag = CVD_H13Bx_Get_SECAM_Flag_CS1();//gogosing
	g_CVD_Status_3CS.cs1_chromalock_flag = CVD_H13Bx_Get_Chromalock_Flag_CS1();
	g_CVD_Status_3CS.cs1_fc_flag = CVD_H13Bx_Get_FC_Flag_CS1();
	g_CVD_Status_3CS.cs1_cordic_freq = CVD_H13Bx_Get_Cordic_Freq_CS1();
	g_CVD_Status_3CS.cs1_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS1();
	g_CVD_Status_3CS.cs1_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS1();
	g_CVD_Status_3CS.cs1_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS1();
	//g_CVD_Status_3CS.cs1_cordic_freq += CVD_H13Bx_Get_Cordic_Freq_CS1();
	g_CVD_Status_3CS.cs1_cordic_freq = (g_CVD_Status_3CS.cs1_cordic_freq>>2);
	//g_CVD_Status_3CS.cs1_cordic_freq = g_CVD_Status_3CS.cs1_cordic_freq /5;
	g_CVD_Status_3CS.cvd_burst_mag = CVD_H13Bx_Get_CVD_Burst_Mag_Value();

	g_CVD_Status_3CS.cs0_secam_flag &= CVD_H13Bx_Get_SECAM_Flag_CS0();//130204

	if( (g_CVD_Status_3CS.h_lock_flag || g_CVD_Status_3CS.v_lock_flag || (g_CVD_Status_3CS.cvd_lock_stable_count > 0) ) \
			&& (g_CVD_Status_3CS.cvd_lock_stable_count < 10) )
	{
		CVD_THREAD_DEBUG("NO[%d],HL[%d],VL[%d],V625[%d],PAL[%d],SECAM[%d],ChromaL[%d]\n cordic_freq[%d], noise[%d], , noburst[%d], c_state[%d], st_count[%d]\n ",g_CVD_Status_3CS.no_signal_flag, g_CVD_Status_3CS.h_lock_flag, g_CVD_Status_3CS.v_lock_flag, g_CVD_Status_3CS.vline_625_flag, g_CVD_Status_3CS.cvd_pal_flag, g_CVD_Status_3CS.cvd_secam_flag, g_CVD_Status_3CS.cvd_chromalock_flag, g_CVD_Status_3CS.cvd_cordic_freq, g_CVD_Status_3CS.status_noise, g_CVD_Status_3CS.no_burst_flag, g_CVD_Status_3CS.cvd_color_system, g_CVD_Status_3CS.cvd_lock_stable_count);
		CVD_THREAD_DEBUG("cs0_pal[%d],cs0_secam[%d],cs0_chroma[%d],cs0_cordic[%d],cs1_pal[%d],cs1_secam[%d],cs1_chroma[%d],cs1_cordic[%d],\n ",g_CVD_Status_3CS.cs0_pal_flag, g_CVD_Status_3CS.cs0_secam_flag, g_CVD_Status_3CS.cs0_chromalock_flag, g_CVD_Status_3CS.cs0_cordic_freq, g_CVD_Status_3CS.cs1_pal_flag, g_CVD_Status_3CS.cs1_secam_flag, g_CVD_Status_3CS.cs1_chromalock_flag, g_CVD_Status_3CS.cs1_cordic_freq);
	}

	//110414 No_Signal_Flag is not working well
	No_Signal_Flag = 0;

	switch(g_CVD_Status_3CS.cvd_color_system)
	{
		case CVD_STATE_VideoNotReady:
#ifdef CVD_CH42_SKIP_WORKAROUND
			//CVD_ClampAGC_OnOff(0); // agc off on No signal for ch42 no signal workaround
			//			if ( lx_chip_rev( ) < LX_CHIP_REV( L9, A0 ) )
			//				CVD_Clamp_Current_Control(0x3);	// 1118 enlarge clamp current in no sigal state
			//			else
			//			{
			//Disable Clampagc reset workaround (Clamp AGC problem is fixed by disabling x2 LPF setting at Set_AAF_Params)
			//CVD_Reset_Clampagc();
			//CVD_Clamp_Current_Control(0x3);	// 1118 enlarge clamp current in no sigal state
			//			}
#endif
			// dws 0809 for 42ch no signal problem
			if(g_CVD_Status_3CS.cvd_no_signal_count > 4) 
			{
#ifdef	H13_HDCT_WA_FOR_HLOCK_ERROR
				if( (g_CVD_Status_3CS.no_signal_flag == 0) && (g_CVD_Status_3CS.h_lock_flag == 0) \
						&& (g_CVD_Status_3CS.v_lock_flag == 1) && (g_CVD_Status_3CS.vnon_standard_flag == 1) )
				{
					CVD_H13Bx_Reset_hdct(1);
					AFE_PRINT("!!! CVD HDCT WORKAROUND !!!\n");
					OS_MsecSleep(5);
					CVD_H13Bx_Reset_hdct(0);
				}
#endif
			}
			else if(g_CVD_Status_3CS.cvd_no_signal_count == 4) // Remain No Signal for 2 seconds
			{
				// dws 0906 for color kill on no signal
				CVD_H13Bx_Force_Color_Kill(1);
				g_CVD_Status_3CS.cvd_no_signal_count++;
			}
			else
				g_CVD_Status_3CS.cvd_no_signal_count++;
			//MainCVD_No_Signal = 1;
#if 0
			if ( HV_Lock_Flag /* || !No_Signal_Flag */)	// HV sync detected
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoReadyToDetect;
			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			break;
#endif

		case CVD_STATE_VideoReadyToDetect:
			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;
			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;
			else if(g_CVD_Status_3CS.color_system_support == (LX_COLOR_SYSTEM_PAL_M | LX_COLOR_SYSTEM_NTSC_M | LX_COLOR_SYSTEM_PAL_NC) )	// For Brazil
			{
				if ( g_CVD_Status_3CS.vline_625_flag ) //Vertical 625 Line
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
				else
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
					//g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
			}
			else // for Multi System
			{
				if ( g_CVD_Status_3CS.vline_625_flag ) //Vertical 625 Line
				{
					if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
					else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
					else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
				}
				else
				{
					if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
					else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
					else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;
					else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)
						g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;
				}
			}

			if(g_CVD_Status_3CS.cvd_next_state != CVD_STATE_VideoNotReady)
				CVD_CSD_DEBUG("CVD Lock !!! g_CVD_Color_System_Support[0x%x], check_supporting_color_system[0x%x], Next_State[%d], port changed [%d]\n", g_CVD_Status_3CS.color_system_support, check_supporting_color_system, g_CVD_Status_3CS.cvd_next_state, g_CVD_Status_3CS.port_change_detected);

			break;

		case CVD_STATE_NTSC:
			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;
#ifdef CVD_USE_NO_BURST_FLAG
			else if(!g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// For No burst signal, with 525 line system remain in NTSC
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
#endif
			else if(g_CVD_Status_3CS.vline_625_flag)
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
			}
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			// 111028 wonsik.do : Check PAL-M before NTSC-M to fix slow color system transition to PAL-M (both NTSC-M & PAL-M locked for long time)
			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)) // Transition to PALm
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			else if(!g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)) // Remain in NTSC
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;

			else if(g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)) // Transition to PAL60
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			else if(!g_CVD_Status_3CS.cvd_pal_flag && !g_CVD_Status_3CS.cs0_pal_flag && !g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)) // Transition to NTSC443
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
			break;

		case CVD_STATE_NTSC443:
			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(!g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// For no burst signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
#endif

			else if(g_CVD_Status_3CS.vline_625_flag)
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
			}
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif

			else if(!g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)) // Remain in NTSC443
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;

			else if(!g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)) // Transition to NTSC
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)) // Transition to PALm
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			else if(g_CVD_Status_3CS.cvd_pal_flag && !g_CVD_Status_3CS.cs0_pal_flag && !g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)) // Transition to PAL60
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			//120613 for EIDEN NTSC-443 to PAL-m transition
			else if(!g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq > (FC_MORE_THRESHOLD+ 30) )  && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD)&& (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)) 
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;
			break;

		case CVD_STATE_PALi:

			g_CVD_Status_3CS.cs0_secam_flag &= CVD_H13Bx_Get_SECAM_Flag_CS0();//130204

			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// no burst 625 line signal
			{
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
			}
#endif
			else if(g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_color_detected)	// no burst 625 line signal
			{
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
			}

			else if(!g_CVD_Status_3CS.vline_625_flag)	// Transition to 525 line system
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;
			}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			else if(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Remain in PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;

			//111028 added cvd choromalock flag to AND with cvd pal flag to determine SECAM detection
			//110809 added cvd pal flag
			//130123 added burst mag for secam detection
			//130201 cvd_fc_flag more than 0 for secam detection
			else if(!(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag) && !g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_secam_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cvd_burst_mag > 0x100) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD)  && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Transition to SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			// 140301 : for faster SECAM detection
			// Added 120613 for EIDEN-3116A SECAM detection
			// 130123 : modified cvd_cordic_freq
			// 130615 : only for AV inputs (indonesia field stream detected as SECAM : PAL_130603_0715_490 )
			// 140121 : modified for EIDEN SECAM RF detection(both PAL & SECAM detected)
			else if( (g_CVD_Status_3CS.status_noise < 500) &&
					(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq > ( FC_MORE_THRESHOLD + 0) )) &&
					!g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_secam_flag && g_CVD_Status_3CS.cs0_chromalock_flag && 
					!g_CVD_Status_3CS.cs1_pal_flag && !g_CVD_Status_3CS.cs1_secam_flag && /*!g_CVD_Status_3CS.cs1_chromalock_flag &&*/ 	//added 140121 for EIDEN RF
					( abs(g_CVD_Status_3CS.cs0_cordic_freq - 128) < 0x30 ) &&
					( g_CVD_Status_3CS.cs1_cordic_freq < (FC_LESS_THRESHOLD - 30) ) &&
				/*	(g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && */
					(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM) ) // Transition to SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Transition to PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;

#ifdef	SYSTEM_DETECTION_BY_CORDIC_ONLY
			else if((g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Remain in PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
#ifdef SYSTEM_DETECTION_BY_CORDIC_CS0_CS1
			else if((g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Transition to SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
			else if((g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Transition to PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
#endif
#endif
			else
			{
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
			}

			break;

		case CVD_STATE_PALm:

			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(!g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// no burst 525 line
			{
				if(g_CVD_Status_3CS.color_system_support == (LX_COLOR_SYSTEM_PAL_M | LX_COLOR_SYSTEM_NTSC_M | LX_COLOR_SYSTEM_PAL_NC) )	// For Brazil
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
				else
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
			}
#endif

			else if(g_CVD_Status_3CS.vline_625_flag)
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
			}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			else if(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= ( FC_MORE_THRESHOLD + 0x40) ) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)) // Remain in PALm
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			else if(g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)) // Transition to PAL60
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			else if(!g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)) // Transition to NTSC
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;

			else if(!g_CVD_Status_3CS.cvd_pal_flag && !g_CVD_Status_3CS.cs0_pal_flag && !g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) /*&& (g_CVD_Status_3CS.cs0_cordic_freq < FC_LESS_THRESHOLD)*/ && (g_CVD_Status_3CS.cs1_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)) // Transition to NTSC443
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			break;

		case CVD_STATE_PALCn:

			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// no burst 625 line
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
#endif

			else if(!g_CVD_Status_3CS.vline_625_flag)		// Transition to 525 line systems
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;
			}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			else if(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Remain in PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;

			else if(!g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_secam_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Transition to SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Transition to PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;

#ifdef	SYSTEM_DETECTION_BY_CORDIC_ONLY
			else if((g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Remain in PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;

#ifdef SYSTEM_DETECTION_BY_CORDIC_CS0_CS1
			else if((g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Transition to SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			else if((g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Transition to PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
#endif
#endif

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;

			break;
		case CVD_STATE_PAL60:

			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(!g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// no burst 525 line
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
#endif

			else if(g_CVD_Status_3CS.vline_625_flag)
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
			}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			else if(g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)) // Remain in PAL60
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			else if(!g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)) // Transition to NTSC
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;

			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)) // Transition to PALm
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;

			else if(!g_CVD_Status_3CS.cvd_pal_flag && !g_CVD_Status_3CS.cs0_pal_flag && !g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq < FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)) // Transition to NTSC443
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;

			break;

		case CVD_STATE_SECAM:

			if ( !HV_Lock_Flag || No_Signal_Flag)	// No signal
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;

			// For user force color system
			else if ((check_supporting_color_system = CVD_H13Bx_Check_Color_System_Support(g_CVD_Status_3CS.color_system_support)) > 0)
				g_CVD_Status_3CS.cvd_next_state = check_supporting_color_system;

#ifdef CVD_USE_NO_BURST_FLAG
			else if(g_CVD_Status_3CS.vline_625_flag && g_CVD_Status_3CS.no_burst_flag)	// no burst 625line
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;
#endif

			else if(!g_CVD_Status_3CS.vline_625_flag)		//Transition to 625line system
			{
				if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_M)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALm;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_NTSC_443)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_NTSC443;
				else if(g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_60)
					g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PAL60;
			}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
			else if (g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_1)
				g_CVD_Status_3CS.cvd_next_state = g_CVD_Status_3CS.cvd_color_system;
#endif
			else if(!g_CVD_Status_3CS.cvd_pal_flag && g_CVD_Status_3CS.cvd_secam_flag && g_CVD_Status_3CS.cvd_chromalock_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Remain in SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			else if(g_CVD_Status_3CS.cs0_pal_flag && g_CVD_Status_3CS.cs0_chromalock_flag && (g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Transition to PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;

			else if(g_CVD_Status_3CS.cs1_pal_flag && g_CVD_Status_3CS.cs1_chromalock_flag && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Transition to PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;

			// 130123 : added for no burst signal
			else if(!g_CVD_Status_3CS.cvd_pal_flag && !g_CVD_Status_3CS.cvd_secam_flag && (g_CVD_Status_3CS.cvd_burst_mag < 0x300) && !g_CVD_Status_3CS.cs1_pal_flag && !g_CVD_Status_3CS.cs1_chromalock_flag && !g_CVD_Status_3CS.cs1_secam_flag && (g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Transition to PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;

#ifdef	SYSTEM_DETECTION_BY_CORDIC_ONLY
			else if((g_CVD_Status_3CS.cvd_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cvd_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_SECAM)) // Remain in SECAM
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;
#ifdef SYSTEM_DETECTION_BY_CORDIC_CS0_CS1
			else if((g_CVD_Status_3CS.cs0_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs0_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_G)) // Transition to PALi
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALi;

			else if((g_CVD_Status_3CS.cs1_cordic_freq <= FC_MORE_THRESHOLD) && (g_CVD_Status_3CS.cs1_cordic_freq >= FC_LESS_THRESHOLD) && (g_CVD_Status_3CS.color_system_support & LX_COLOR_SYSTEM_PAL_NC)) // Transition to PALCn
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_PALCn;
#endif
#endif

			else
				g_CVD_Status_3CS.cvd_next_state = CVD_STATE_SECAM;

			break;

		default:
			break;
	}

#ifdef L9_FAST_3DCOMB_WORKAROUND
// 120720 : Moved following CVD_H13Bx_Set_HNon_Standard_Threshold to same state
// Setting HNon_Standard_Threshold to '0' in transition periode makes abnormal 3DComb operation ?????
	if(g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_20) {
		if( (g_CVD_Status_3CS.hnon_standard_flag) && (g_CVD_Status_3CS.in_atv_tunning == FALSE) && (g_CVD_Status_3CS.cvd_next_state != CVD_STATE_SECAM) && (g_CVD_Status_3CS.cvd_next_state != CVD_STATE_NTSC443)) {	// hnon standard detected  && not in auto tuning && color system is not in SECAM/NTSC443
			CVD_H13Bx_Set_HNon_Standard_Threshold(0);
		}
		else if(g_CVD_Status_3CS.cvd_next_state < CVD_STATE_VideoNotReady) {
			CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_hnon_std_threshold);
		}
		else {
			//		CVD_DEBUG("HNon_Std not detected in not locked state \n");
		}
	}
#endif
	// #1 	: sync lock detected
	//		: reset "g_CVD_Status_3CS.cvd_no_signal_count"
	if((g_CVD_Status_3CS.cvd_next_state != CVD_STATE_VideoNotReady) && (g_CVD_Status_3CS.cvd_next_state != CVD_STATE_VideoReadyToDetect))
	{
		g_CVD_Status_3CS.cvd_no_signal_count = 0;
		// Continuous Write on ckill register when signal locked state.
		//		CVD_H13Bx_Force_Color_Kill(0);
	}

	// #2 	: State Change Detected !!!!!
	//		:
	if( (g_CVD_Status_3CS.cvd_next_state != g_CVD_Status_3CS.cvd_color_system) || (g_CVD_Status_3CS.port_change_detected == TRUE))
	{
		CVD_CSD_DEBUG("Before State Change : Locked Stable count [%d]\n", g_CVD_Status_3CS.cvd_lock_stable_count);
		g_CVD_Status_3CS.cvd_lock_stable_count = 0;
		cvd_noise_sum = 0;	//clear noise sum value
		//		chromalock_count = 0;	//clear noise sum value
	//	g_CVD_Status_3CS.port_change_detected = FALSE;

		//g_CVD_Status_3CS.cvd_color_system = g_CVD_Status_3CS.cvd_next_state;
		CVD_CSD_DEBUG("CVD State Change to [%d]\n", g_CVD_Status_3CS.cvd_next_state);
		CVD_CSD_DEBUG("NO[%d],HL[%d],VL[%d],V625[%d],PAL[%d],SECAM[%d],ChromaL[%d]\n cordic_freq[%d], noise[%d], , noburst[%d], c_state[%d], g_State[%d], prev[%d]\n ",g_CVD_Status_3CS.no_signal_flag, g_CVD_Status_3CS.h_lock_flag, g_CVD_Status_3CS.v_lock_flag, g_CVD_Status_3CS.vline_625_flag, g_CVD_Status_3CS.cvd_pal_flag, g_CVD_Status_3CS.cvd_secam_flag, g_CVD_Status_3CS.cvd_chromalock_flag, g_CVD_Status_3CS.cvd_cordic_freq, g_CVD_Status_3CS.status_noise, g_CVD_Status_3CS.no_burst_flag, g_CVD_Status_3CS.cvd_color_system, g_CVD_Status_3CS.cvd_color_system, g_CVD_Status_3CS.cvd_prev_color_system);
		CVD_CSD_DEBUG("CS0:PAL[%d],SECAM[%d],ChromaL[%d] cordic_freq[%d]\n ", g_CVD_Status_3CS.cs0_pal_flag, g_CVD_Status_3CS.cs0_secam_flag, g_CVD_Status_3CS.cs0_chromalock_flag, g_CVD_Status_3CS.cs0_cordic_freq);
		CVD_CSD_DEBUG("CS1:PAL[%d],SECAM[%d],ChromaL[%d] cordic_freq[%d]\n ", g_CVD_Status_3CS.cs1_pal_flag, g_CVD_Status_3CS.cs1_secam_flag, g_CVD_Status_3CS.cs1_chromalock_flag, g_CVD_Status_3CS.cs1_cordic_freq);
		CVD_CSD_DEBUG("Color_System_Support[0x%x], port changed [%d]\n", g_CVD_Status_3CS.color_system_support, g_CVD_Status_3CS.port_change_detected);

		// For proper color system detection, at first set cstate value to default.
		//CVD_H13Bx_Program_Color_System_PreJob(&g_SetColorSystem_3CS_H13B0[CVD_STATE_Default]);
		CVD_H13Bx_No_Color_Cstate_Control(&g_CVD_Status_3CS, 1);

		//120328 : not to excute following function on signal transition period.
		//CVD_NoBurst_CKill_Control(&g_CVD_Status_3CS);

		for(loop = 0;loop < 10;loop++) {
			burst_mag_array[loop] = 0xFFFF ;
			burst_mag_sum += burst_mag_array[loop];
		}

		burst_mag_prev=0;
		burst_mag_next=0;
		prev_burst_mag_avg = 0xbc0;
		cagc_target = 0xbc0;

#ifdef H13_COMB2D_ONLY_CONTROL
#ifndef H13_3DCOMB_ERROR_WORKAROUND
			//120705 : Enable comb2d_only when state changed !!!
			CVD_DEBUG("### Comb2d only [1] [%s][%d] ###\n",__func__, __LINE__ );
			CVD_H13Bx_Set_comb2d_only(1);
			CVD_H13Bx_Set_comb2d_only_md(1);
#endif
#endif

#ifdef H13_FAST_3DCOMB_WORKAROUND
		if(g_CVD_Status_3CS.in_atv_tunning == FALSE)
		{
			//Enable vf_nstd_en when state changed (WA for 3DComb buffer control error)
			CVD_H13Bx_vf_nstd_control(1);	//default value when state changed
			// Set Hnon_std_threshold to '0' when state changed (WA for 3DComb buffer control error)
			CVD_H13Bx_Set_HNon_Standard_Threshold(0);	//121222 : kim.min
		}
#endif
		// #2.1 	: State Change & Previously Locked State is NTSC
		if( g_CVD_Status_3CS.cvd_prev_color_system == CVD_STATE_NTSC)
		{
			// Restore cagc value to default
			CVD_CSD_DEBUG("Cagc value to default \n");
			CVD_H13Bx_Set_CVD_CAGC(CVD_BURST_MAG_STATE_BIG,&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_prev_color_system], g_CVD_Status_3CS.cvd_prev_color_system);

			//120131
			/*
			//if saturation value is adjusted, restoring initial value is needed (for same color system detection case)
			if ( lx_chip_rev() >= LX_CHIP_REV( L9, B0) ) {
			CVD_H13Bx_Set_CVD_Saturation_Value(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_prev_color_system].reg_saturation);
			}
			 */

#ifdef L9_WA_FOR_EBS_STABLE_AGC
			// restore default values of reg_agc_half_en to '0', and reg_nstd_hysis to '7' for EBS field stream support.
			// restore default values of reg_agc_peak_cntl to '1', and reg_dcrestore_gain to '0' for stable AGC on NTSC RF
			if ( g_Stable_AGC_WA_for_EBS_Enabled)
			{
				g_Stable_AGC_WA_for_EBS_Enabled = 0;
				CVD_H13Bx_Program_Color_System_PreJob2();
			}
#endif
#ifdef L9_AGC_BYPASS_WA_FOR_NOISY_SIGNAL
			// for PERU Weak RF signal
			if (g_CVD_Status_3CS.noisy_signal_detected == 1) {
				CVD_CSD_DEBUG("CVD Normal RF Signal(default) \n");
				// changed to normal signal
				g_CVD_Status_3CS.noisy_signal_detected = 0;
				CVD_H13Bx_Set_for_Normal_Signal();
			}
#endif
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
			//		CVD_CSD_DEBUG("Set hstate_fixed value to 0 (default) \n");
			CVD_H13Bx_Set_Hstate_Fixed(0);
			CVD_H13Bx_Set_Hstate_Max(5);
#endif
#ifdef L9_VSYNC_CNTL_FOR_STABLE_VSYNC // disabled
			CVD_H13Bx_Set_for_Stable_Vsync(0);
#endif
#ifdef L9_CHANGE_FIELD_DETECT_MODE_ON_DONG_GO_DONG_RAK

			CVD_H13Bx_Set_for_Field_Detect_Mode(2);
#endif
#ifdef L9_ADAPTIVE_AGC_PEAK_EN_CONTROL
			//120222 setting agc_peak_en to initial value is moved to CVD_STABLE_COUNT_1
			// for NTSC system agc_peak_en should be '1' on no signal state.
			//CVD_AGC_Peak_En_Control(2, 0, 0);	//force on peak_en
#endif

#ifdef BURST_GATE_ADJUST_FOR_BURST_POSITION_SHIFT
			if ( g_CVD_Status_3CS.burst_gate_modified )
			{
				CVD_H13Bx_Burst_Gate_Control(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_prev_color_system].reg_burst_gate_start_new, g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_prev_color_system].reg_burst_gate_end_new);

				g_CVD_Status_3CS.burst_gate_modified = 0;
			}
#endif
		}
		// #2.2 	: State Change & Previously Locked State is PALm
		else if( g_CVD_Status_3CS.cvd_prev_color_system == CVD_STATE_PALm)
		{
#ifdef H13_ENABLE_PALM_STABLE_SYNC_MODE
			CVD_H13Bx_Set_PALm_Stable_HSync_Mode(0);
#endif
#ifdef L9_WA_FOR_EBS_STABLE_AGC
			// restore default values of reg_agc_half_en to '0', and reg_nstd_hysis to '7' for EBS field stream support.
			// restore default values of reg_agc_peak_cntl to '1', and reg_dcrestore_gain to '0' for stable AGC on NTSC RF
			if ( g_Stable_AGC_WA_for_EBS_Enabled)
			{
				g_Stable_AGC_WA_for_EBS_Enabled = 0;
				CVD_H13Bx_Program_Color_System_PreJob2();
			}
#endif
#ifdef L9_DONT_KILL_CHROMALOCK_ON_CLOCK_LOST
			CVD_H13Bx_OnOff_Chromalock_Ckill(1);
#endif
#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
			CVD_H13Bx_Set_for_Burst_Gate_End_On_Noisy(g_SetColorSystem_3CS_H13B0[CVD_STATE_PALm].reg_burst_gate_end_new ,0);
#ifdef L9_BYPASS_AGC_ON_ADAPTIVE_BURST_GATE_END_MODE
			CVD_H13Bx_AGC_Bypass(0);
#endif
#endif
		}
		// #2.3 	: State Change & Previously Locked State is PALi
		else if( g_CVD_Status_3CS.cvd_prev_color_system == CVD_STATE_PALi) {

			// Restore cagc value to default
			CVD_CSD_DEBUG("Cagc value to default : color_system [%d] \n", g_CVD_Status_3CS.cvd_prev_color_system);
			CVD_H13Bx_Set_CVD_CAGC(CVD_BURST_MAG_STATE_BIG,&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_prev_color_system], g_CVD_Status_3CS.cvd_prev_color_system);

#ifdef	H13_PAL_STABLE_VSYNC_WORKAROUND
			CVD_H13Bx_Set_Vsync_Cntl(&g_SetColorSystem_3CS_H13B0[CVD_STATE_PALi], 0);	// direct vsync
#endif
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
			//		CVD_CSD_DEBUG("Set hstate_fixed value to 0 (default) \n");
			CVD_H13Bx_Set_Hstate_Fixed(0);
#endif
			//120217 : added for PAL channel change
		} // #2.4 	: State Change & Previously Locked State is SECAM
		else if( g_CVD_Status_3CS.cvd_prev_color_system == CVD_STATE_SECAM) {
		} // #2.5 	: State Change & Previously Locked State is not NTSC/PALm/PALi/SECAM
		else {
		}
		// NTSC Pattern Detection Function
		if( g_CVD_Pattern_Detection_t.bEnable == TRUE )
		{
			g_CVD_Pattern_Detection_t.pattern_found = 0x0 ;
			g_CVD_Pattern_Detection_t.prev_motion_value = 0x0 ;
			g_CVD_Pattern_Detection_t.global_motion_count = 0x0 ;
			g_CVD_Pattern_Detection_t.static_pattern_found = 0x0 ;
			g_CVD_Pattern_Detection_t.static_pattern_count = 0x0 ;
		}


		if(g_CVD_Status_3CS.in_atv_tunning == FALSE)
			CVD_H13Bx_AGC_Bypass_Function(0, 0, 1 );
		//CVD_AGC_Bypass_Function((UINT32)g_CVD_Status_3CS.status_noise, (UINT32)g_CVD_Status_3CS.h_lock_flag, (UINT32)g_CVD_Status_3CS.no_signal_flag );
		//if((g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC) ||(g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALi))
		// Sync Detected !!!

		// #3 	: State Change Detected && Sync Lock Detected !!!
		//		:
		if(g_CVD_Status_3CS.cvd_next_state < CVD_STATE_VideoNotReady)
		{

			/* Cpump Auto Stip to 1 */
			/* Cpump_Auto_Stip should be '1' : White saturation on normal signal */
			//CVD_H13Bx_Set_Cpump_Auto_Stip_Value(1);

			// If any color system is detected, disable color kill.
			CVD_H13Bx_Force_Color_Kill(0);

			if( g_CVD_Status_3CS.in_rf_mode == TRUE )
			{
				CVD_H13Bx_agc_half_en_control(0);//gogosing agc
			}

			/*
			//120217 for test only
			if ( lx_chip_rev( ) >= LX_CHIP_REV( L9, B0 ) )
			CVD_H13Bx_Reset_mif(0);
			 */

			//		if(g_CVD_Status_3CS.cvd_prev_color_system != g_CVD_Status_3CS.cvd_next_state) // IF from no signal, do not re-setting cvd register to the same value

			// #4 	: State Change from No signal or Color System Change
			//		:
			if( (g_CVD_Status_3CS.cvd_prev_color_system != g_CVD_Status_3CS.cvd_next_state) || (g_CVD_Status_3CS.cvd_color_system==CVD_STATE_VideoNotReady) || ( g_CVD_Status_3CS.prev_av_rf_mode != g_CVD_Status_3CS.in_rf_mode) )
			{

				// #5 	: State Change from No signal, and different color system from previously locked color system OR AV/RF input source change.
				//		:
				//if( ( g_CVD_Status_3CS.cvd_prev_color_system != g_CVD_Status_3CS.cvd_next_state) || (MainCVD_Is_Prev_RF_Input != g_CVD_Status_3CS.in_rf_mode))
				CVD_CSD_DEBUG("Color System Change or Locked: prev system[%d], next[%d], cvd system[%d] ,port changed [%d], prev_rf[%d], in_rf[%d]\n", g_CVD_Status_3CS.cvd_prev_color_system ,g_CVD_Status_3CS.cvd_next_state, g_CVD_Status_3CS.cvd_color_system,  g_CVD_Status_3CS.port_change_detected,  g_CVD_Status_3CS.prev_av_rf_mode,  g_CVD_Status_3CS.in_rf_mode);

				if( ( g_CVD_Status_3CS.cvd_prev_color_system != g_CVD_Status_3CS.cvd_next_state) || (g_CVD_Status_3CS.port_change_detected == TRUE) || ( g_CVD_Status_3CS.prev_av_rf_mode != g_CVD_Status_3CS.in_rf_mode))
				{
					CVD_CSD_DEBUG("Setting CVD [%d]\n", g_CVD_Status_3CS.cvd_next_state);

					/*
					   if((g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC) ||(g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSCj)|| (g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC443) )
					   {
					   gEnable_SW3DCOMBControl = 1;
					   cvd_pe_color_system = LX_CVD_PQ_NTSC_M;
					   }
					   else
					   {
					   gEnable_SW3DCOMBControl = 0;
					   CVD_Set_motion_mode(0x1, 0x0) ;
					   cvd_pe_color_system = LX_CVD_PQ_PAL;
					   }

					   CVD_Set_PQ_Mode(cvd_pe_color_system);

					 */
					{
						//AFE_PRINT("HDCT/CDCT Reset 1\n");
						CVD_H13Bx_Reset_hdct(1);
						CVD_H13Bx_Reset_cdct(1);
#ifdef H13_COMB2D_ONLY_CONTROL
						/*
						//120718 : comb2d_only on control when color system change
						CVD_DEBUG("$$$ Comb2d only [1] $$$\n");
						CVD_H13Bx_Read_Buffer_Status(&rbuf1_empty, &rbuf2_empty, &rbuf3_empty, &rbuf4_empty, &wbuf_ful, &wbuf_empty);
						CVD_DEBUG("### rbuf1_empty[%d], rbuf2_empty[%d], rbuf3_empty[%d], rbuf4_empty[%d], wbuf_ful[%d], wbuf_empty[%d] #### \n", rbuf1_empty, rbuf2_empty, rbuf3_empty, rbuf4_empty, wbuf_ful, wbuf_empty);
						CVD_H13Bx_Set_comb2d_only(1);
						 */
#endif

						OS_MsecSleep(5);
					}

#ifdef	H13_3DCOMB_ERROR_WORKAROUND
					CVD_H13Bx_3DComb_Workaround(1);
#endif

					CVD_H13Bx_Set_Color_System_3CS(g_CVD_Status_3CS.cvd_next_state);
					//MainCVD_Prev_Written_Color_System = g_CVD_Status_3CS.cvd_next_state;
					g_CVD_Status_3CS.cvd_prev_color_system = g_CVD_Status_3CS.cvd_next_state;
					//MainCVD_Is_Prev_RF_Input = g_CVD_Status_3CS.in_rf_mode;

					{
						CVD_H13Bx_Reset_hdct(0);
						CVD_H13Bx_Reset_cdct(0);
					}

					// Move CVD PQ setting function from DDI to Kernel
					if(1) {
						if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_NTSC_M);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALi) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_SECAM) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_SECAM_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_SECAM);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC443) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_NTSC_443_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_NTSC_443);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PAL60) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_60_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_60);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALm) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_M_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_M);
						}
						else if ( g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALCn) {
							if(g_CVD_Status_3CS.in_rf_mode == TRUE)
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_NC_RF);
							else
								CVD_Set_PQ_Mode(LX_CVD_PQ_PAL_NC);
						}
						else
							AFE_ERROR("ERROR Invalid Color system [%s][%d]\n", __func__, __LINE__);
					}
					g_CVD_Status_3CS.prev_av_rf_mode = g_CVD_Status_3CS.in_rf_mode;
				}
				// #6 	: State Change from No signal, and but same color system with previsouly locked system.
				//		: IF from VideoReadyToDetect, do not re-setting cvd register to the same value
				else
				{
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
					CVD_H13Bx_3DComb_Workaround(1);
#endif
					CVD_CSD_DEBUG("Same system [%d] again\n", g_CVD_Status_3CS.cvd_next_state);
#ifdef H13_COMB2D_ONLY_CONTROL
					/*
					//120718 : Disable comb2d_only when CVD signal detected according to the next color system
					CVD_DEBUG("$$$ Comb2d only [0] $$$\n");
					CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
					 */
#endif
				}

				//	CVD_H13Bx_Force_Color_Kill(0);
#ifdef CVD_CH42_SKIP_WORKAROUND
				CVD_Clamp_Current_Control(0);	// 1118 change clamp current to zero
#endif

#ifdef	H13_STABLE_HSYNC_WORKAROUND
				//For RF Signal
				if(g_CVD_Status_3CS.in_atv_tunning == FALSE)
				{
					if( (g_CVD_Status_3CS.in_rf_mode == TRUE) \
							&& ( (g_CVD_Status_3CS.cvd_next_state == CVD_STATE_NTSC) || (g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALm)) )
						CVD_H13Bx_Enable_Stable_Sync_Mode(1);		//for RF nosignal (dcrestore_lpf_en ON)
					else
						CVD_H13Bx_Enable_Stable_Sync_Mode(0);		//default state

					if ((g_CVD_Status_3CS.in_rf_mode == TRUE) )
					   CVD_H13Bx_Hstart_Position_Compensator(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state]);
				}
#endif

#ifdef H13_ENABLE_PALM_STABLE_SYNC_MODE
				if ((g_CVD_Status_3CS.cvd_next_state == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) \
						&& (g_CVD_Status_3CS.in_atv_tunning == FALSE))
				{
					CVD_H13Bx_Set_PALm_Stable_HSync_Mode(1);
				}
#endif
#ifdef H13_ENABLE_PALM_STABLE_SYNC_MODE
				if ((g_CVD_Status_3CS.cvd_next_state != CVD_STATE_PALm) || (g_CVD_Status_3CS.in_rf_mode == FALSE) \
						|| (g_CVD_Status_3CS.in_atv_tunning == TRUE))
				{
					CVD_H13Bx_Set_PALm_Stable_VSync_Mode(0);
				}
#endif
			}
			//Program_Color_System_Main_only_system_set(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state]);
			//SW reset is needed here???
			//CVD_SW_Reset(LX_CVD_MAIN);
			//gMainCVD_State = g_CVD_Status_3CS.cvd_next_state;

		}
		// #7 	: State Change to NO SIGNAL !!!!!
		//		:
		else if(g_CVD_Status_3CS.cvd_next_state != CVD_STATE_VideoReadyToDetect)  // No Signal or color system change
		{
			/* Cpump Auto Stip to 0 : default for no signal white noise */
			/* Cpump_Auto_Stip should be '1' : White saturation on normal signal */
			//CVD_H13Bx_Set_Cpump_Auto_Stip_Value(0);

#ifdef CVD_CH42_SKIP_WORKAROUND
			CVD_Clamp_Current_Control(0x3);	// 1118 enlarge clamp current in no sigal state
#endif
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
//			CVD_H13Bx_3DComb_Workaround(0);
#ifdef H13_CDTO_CONTROL
			CVD_H13Bx_3DComb_Workaround(4);
#endif
#endif

			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoNotReady;

#ifdef	H13_VDCT_WA_FOR_FAST_VLINE_DETECTION
			CVD_H13Bx_Reset_vdct(1);
			OS_MsecSleep(5);
			CVD_H13Bx_Reset_vdct(0);
#endif

#ifdef H13_COMB2D_ONLY_CONTROL
#if 0
			//120705 : Enable comb2d_only when no signal detected !!!
			CVD_DEBUG("### Comb2d only [1] [%s][%d] ###\n",__func__, __LINE__ );
			//CVD_H13Bx_Read_Buffer_Status(&rbuf1_empty, &rbuf2_empty, &rbuf3_empty, &rbuf4_empty, &wbuf_ful, &wbuf_empty);
			//CVD_DEBUG("### rbuf1_empty[%d], rbuf2_empty[%d], rbuf3_empty[%d], rbuf4_empty[%d], wbuf_ful[%d], wbuf_empty[%d] #### \n", rbuf1_empty, rbuf2_empty, rbuf3_empty, rbuf4_empty, wbuf_ful, wbuf_empty);
			CVD_H13Bx_Set_comb2d_only(1);
			CVD_H13Bx_Set_comb2d_only_md(1);
#endif
#endif

			//gogosing agc :121206
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
	//		CVD_H13Bx_AGC_Peak_En_Control(0, 0, 0); //force off peak_en
	//		CVD_H13Bx_agc_half_en_control(1);//gogosing agc
#endif


			CVD_H13Bx_Set_Nosignal_Vactive_Params();
		}
		// #8 	: State Change to videoreadytodetect !!!!!
		//		: never enter here
		else
		{
			// From Ok : SW workaround for 3D comb operation 2010/06/04
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoReadyToDetect;
		}
		//MainCVD_Same_State_Count = 0;

		//1126 for fast standard detection
		//gMainCVD_State = g_CVD_Status_3CS.cvd_next_state;
		g_CVD_Status_3CS.cvd_color_system = g_CVD_Status_3CS.cvd_next_state;

		//120711 : modified not to use noburst_ckill register. 
		// Jitter on noburst cross hatch signal can be improved by setting reg_fixed_cstate to '0', reg_cstate to '5'
		// cstate values were modified(reg_fixed_cstate to '1', reg_cstate to '7') to satisfy sub-carrier pull-in range test.
		// CVD driver is modified that, when chroma is Unlocked and burst magnitude is Not small cstate values will be modified.

		// Set Noburst Ckill to 0(default value) whenever state change occur
		//CVD_CSD_DEBUG("Set No Burst CKill to 0\n");
		//CVD_H13Bx_NoBurst_CKill_Control(&g_CVD_Status_3CS, 1);
		if(g_CVD_Status_3CS.port_change_detected)
		{
			CVD_CSD_DEBUG("@@@ Port changed ... clear prev color system @@@\n");
			g_CVD_Status_3CS.cvd_prev_color_system = CVD_STATE_VideoNotReady;
			g_CVD_Status_3CS.port_change_detected = FALSE;
		}
	} // #2 	: State Change Detected 

	// #8 	: Same State Again
	else // same state again
	{

		// #9 	: Same State and Sync Lock
		//		CVD_CSD_DEBUG("Same State and Sync Lock , g_CVD_Status_3CS.cvd_next_state[%d], gAutoProgramming[%d]\n",g_CVD_Status_3CS.cvd_next_state, g_CVD_Status_3CS.in_atv_tunning );

		if (g_CVD_Status_3CS.cvd_next_state < CVD_STATE_VideoNotReady)
		{
			if(g_CVD_Status_3CS.in_atv_tunning == FALSE) {

				cvd_noise_sum += g_CVD_Status_3CS.status_noise;
				//CVD_DEBUG("@@@ status_noise[0x%x], noise_sum[0x%x], stable_count[%d] @@@\n", g_CVD_Status_3CS.status_noise, cvd_noise_sum, g_CVD_Status_3CS.cvd_lock_stable_count);

				//				if(g_CVD_Status_3CS.cvd_chromalock_flag)
				//					chromalock_count++;


				if(g_CVD_Status_3CS.cvd_lock_stable_count > CVD_STABLE_COUNT_20) {
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC

					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC)  && (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.in_atv_tunning == FALSE)) {
						gHstateMaxCounter++;
						if(CVD_H13Bx_Read_VCR_Detected() && (CVD_H13Bx_Get_Global_Motion_Value() > 0x10)  && (g_CVD_Status_3CS.hnon_standard_flag == 0) ) // When Global Motion Value is high
							vcr_detect_count++;

						if(gHstateMaxCounter >= 20)
						{
							if ( ( vcr_detect_count >=  CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD ) && !g_CVD_Pattern_Detection_t.static_pattern_found)
							{
								CVD_H13Bx_Set_Hstate_Max(3);
								CVD_H13Bx_Set_Hstate_Fixed(0);		// VCR tape Rating issue
							}
// removed due to hsync unstable when vcr detected/not detected toggling
#if 0
							else CVD_H13Bx_Set_Hstate_Max(5);
#endif

							gHstateMaxCounter = 0;
							vcr_detect_count = 0;
						}
					}
#endif

#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE)  )
					{
						if(gAdaptive_Burst_Gate_Counter == 0)
						{
							if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
								gVariance_Status_Cdto_Inc = CVD_H13Bx_Differential_Status_Cdto_Inc_Value();
								gAccumulated_Cordic_Freq_Value = CVD_H13Bx_Read_Cordic_Freq_Value();
							}
							gAdaptive_Burst_Gate_Counter++;
						}
						else if(gAdaptive_Burst_Gate_Counter < CVD_ADAPTIVE_BURST_GATE_COUNT_ITERATION)
						{
							if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
								gVariance_Status_Cdto_Inc += CVD_H13Bx_Differential_Status_Cdto_Inc_Value();
								gAccumulated_Cordic_Freq_Value += CVD_H13Bx_Read_Cordic_Freq_Value();
							}
							gAdaptive_Burst_Gate_Counter++;
						}
						else if(gAdaptive_Burst_Gate_Counter >= CVD_ADAPTIVE_BURST_GATE_COUNT_ITERATION)
						{
							if((gVariance_Status_Cdto_Inc >= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_0)&&(gAccumulated_Cordic_Freq_Value >= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_1))
								Adaptive_Burst_Gate_Enable = 1;
							else if(gVariance_Status_Cdto_Inc <= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_3)
								Adaptive_Burst_Gate_Enable = 0;
							else
								Adaptive_Burst_Gate_Enable = 2;

							if((Adaptive_Burst_Gate_Enable == Prev_Adaptive_Burst_Gate_Enable)&&(Adaptive_Burst_Gate_Enable != 2))
							{
								CVD_H13Bx_Set_for_Burst_Gate_End_On_Noisy(g_SetColorSystem_3CS_H13B0[CVD_STATE_PALm].reg_burst_gate_end_new ,Adaptive_Burst_Gate_Enable);
#ifdef L9_BYPASS_AGC_ON_ADAPTIVE_BURST_GATE_END_MODE
								CVD_H13Bx_AGC_Bypass(Adaptive_Burst_Gate_Enable);
#endif
							}
							Prev_Adaptive_Burst_Gate_Enable = Adaptive_Burst_Gate_Enable;

							//AFE_PRINT("AVERAGE CDTO_INC[%x] CORDIC_FREQ[%d] RESULT[%d]\n", gVariance_Status_Cdto_Inc, gAccumulated_Cordic_Freq_Value, Adaptive_Burst_Gate_Enable );
							gAdaptive_Burst_Gate_Counter = 0;
						}
					}
#endif
#ifdef L9_AGC_BYPASS_WA_FOR_NOISY_SIGNAL

					stable_state2_count ++;

					if(stable_state2_count >= CVD_STABLE_COUNT_FOR_AGC_CONTROL) {

						avg_noise_level = cvd_noise_sum / stable_state2_count;
						stable_state2_count = 0;
						cvd_noise_sum = 0;

						//CVD_CSD_DEBUG("# avg noise level(stable state) [0x%x]#\n", avg_noise_level);

						// for PERU Weak RF signal
						//for noisy signal , diable agc(agc_bypass) & set dcrestore gain to 1/8 for stable sync and AGC.
						if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
							if( avg_noise_level >=  (CVD_H13B0_NOISE_MAX - 0x10) ) {
								if (g_CVD_Status_3CS.noisy_signal_detected == 0) {
									CVD_WA_DEBUG("CVD Noisy RF Signal(stable state) \n");
									// changed to noisy signal
									g_CVD_Status_3CS.noisy_signal_detected = 1;
									CVD_H13Bx_Set_for_Noisy_Signal();
								}
							}
							else if( avg_noise_level < (CVD_H13B0_NOISE_MAX - 0x30) ) {
								if (g_CVD_Status_3CS.noisy_signal_detected == 1) {
									CVD_WA_DEBUG("CVD Normal RF Signal(stable state) \n");
									// changed to normal signal
									g_CVD_Status_3CS.noisy_signal_detected = 0;
									CVD_H13Bx_Set_for_Normal_Signal();
								}
							}
						}

					}
#endif
					/* On Brazil (PAL-M) weak RF Signal, AGC gain sometimes became high value result in white picture.
					   Disabling AGC is needed on this weak RF signal( No h-lock, high noise level )
					 */
					if( (g_CVD_Status_3CS.in_atv_tunning == FALSE) && (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm))
						CVD_H13Bx_AGC_Bypass_Function((UINT32)g_CVD_Status_3CS.avg_status_noise, (UINT32)g_CVD_Status_3CS.h_lock_flag, (UINT32)g_CVD_Status_3CS.no_signal_flag );

				}
				else if(g_CVD_Status_3CS.cvd_lock_stable_count == CVD_STABLE_COUNT_20) {

					/*
					   CVD_DEBUG("chromalock[%d], chromalock_count[%d]\n", g_CVD_Status_3CS.cvd_chromalock_flag, chromalock_count);
					   if( (g_CVD_Status_3CS.cvd_chromalock_flag) || (chromalock_count > 3))	// burst exist???
					   {
					   CVD_H13Bx_Program_Color_System_PostJob(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state]);
					   }
					 */
#ifdef L9_FAST_3DCOMB_WORKAROUND
					CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_hnon_std_threshold);
#endif
#ifdef H13_FAST_3DCOMB_WORKAROUND
					//CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_hnon_std_threshold);
					//Enable vf_nstd_en on same state 2
					//CVD_H13Bx_vf_nstd_control(1);	//default value when state changed
#endif
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC)&& (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.in_atv_tunning == FALSE)) {
						if ( ( vcr_detect_count >=  CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD ) && !g_CVD_Pattern_Detection_t.static_pattern_found)
						{
							CVD_WA_DEBUG("VCR Detected [%d] \n", vcr_detect_count);
							CVD_H13Bx_Set_Hstate_Max(3);
							CVD_H13Bx_Set_Hstate_Fixed(0);		// VCR tape Rating issue
						}
						else {
							CVD_H13Bx_Set_Hstate_Max(5);
							CVD_H13Bx_Set_Hstate_Fixed(1);		// VCR tape Rating issue
						}
						vcr_detect_count = 0;
						gHstateMaxCounter = 0;
					}
#endif

					//				CVD_DEBUG("### CVD Same State Max 2 !!! ###\n");

#ifdef L9_AGC_BYPASS_WA_FOR_NOISY_SIGNAL
					avg_noise_level = cvd_noise_sum / (g_CVD_Status_3CS.cvd_lock_stable_count + 1);
					cvd_noise_sum = 0;	//
					stable_state2_count = 0;
					CVD_WA_DEBUG("#avg noise level [0x%x]#\n", avg_noise_level);

					// for PERU Weak RF signal
					//for noisy signal , diable agc(agc_bypass) & set dcrestore gain to 1/8 for stable sync and AGC.
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
						// use long-time averaged value
						if( avg_noise_level >=  (CVD_H13B0_NOISE_MAX - 0x10) ) {
							if (g_CVD_Status_3CS.noisy_signal_detected == 0) {
								CVD_WA_DEBUG("CVD Noisy RF Signal \n");
								// changed to noisy signal
								g_CVD_Status_3CS.noisy_signal_detected = 1;
								CVD_H13Bx_Set_for_Noisy_Signal();
							}
						}
					}
#endif
#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
						if((gVariance_Status_Cdto_Inc >= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_0)&&(gAccumulated_Cordic_Freq_Value >= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_1))
							Adaptive_Burst_Gate_Enable = 1;
						else if(gVariance_Status_Cdto_Inc <= CVD_ADAPTIVE_BURST_GATE_THRESHOLD_3)
							Adaptive_Burst_Gate_Enable = 0;
						else
							Adaptive_Burst_Gate_Enable = 2;

						if((Adaptive_Burst_Gate_Enable == Prev_Adaptive_Burst_Gate_Enable)&&(Adaptive_Burst_Gate_Enable != 2))
						{
							CVD_H13Bx_Set_for_Burst_Gate_End_On_Noisy( g_SetColorSystem_3CS_H13B0[CVD_STATE_PALm].reg_burst_gate_end_new , Adaptive_Burst_Gate_Enable);
#ifdef L9_BYPASS_AGC_ON_ADAPTIVE_BURST_GATE_END_MODE
							CVD_H13Bx_AGC_Bypass(Adaptive_Burst_Gate_Enable);
#endif
						}
						//AFE_PRINT("AVERAGE CDTO_INC[%x] CORDIC_FREQ[%d] RESULT[%d]\n", gVariance_Status_Cdto_Inc, gAccumulated_Cordic_Freq_Value, Adaptive_Burst_Gate_Enable);
						gAdaptive_Burst_Gate_Counter = 0;
					}
#endif
#if 0
#ifdef L9_WA_FOR_EBS_STABLE_AGC
					// set reg_agc_half_en to '1', and reg_nstd_hysis to '0' for EBS field stream
					// set reg_agc_peak_cntl to '0', and reg_dcrestore_gain to '3' for stable AGC on NTSC RF
					if (( lx_chip_rev() >= LX_CHIP_REV( L9, B0)) && (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) )
					{
						g_Stable_AGC_WA_for_EBS_Enabled = 1;
						CVD_H13Bx_Program_Color_System_PostJob2();
					}
#endif
#endif
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
			//		CVD_H13Bx_AGC_Peak_En_Control(2, 0, 0);	// force on peak_en
#endif
#ifdef L9_VSYNC_CNTL_FOR_STABLE_VSYNC // disabled
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
						CVD_H13Bx_Set_for_Stable_Vsync(1);
					}
#endif

#ifdef	H13_STABLE_HSYNC_WORKAROUND
					avg_noise_level = cvd_noise_sum / (g_CVD_Status_3CS.cvd_lock_stable_count + 1);
					//Stable Hsync for NTSC RF Field Streams
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE))
					{
						CVD_DEBUG("### avg noise level[0x%x],burst mag[0x%x] at stable20 #\n", avg_noise_level, g_CVD_Status_3CS.cvd_burst_mag);
						if( (avg_noise_level > 0x240) && (g_CVD_Status_3CS.cvd_burst_mag > 0x1800) )	// for NS Home-shopping field stream
							CVD_H13Bx_Enable_Stable_Sync_Mode(3);
					}
#endif
#ifdef H13_ENABLE_PALM_STABLE_SYNC_MODE
					avg_noise_level = cvd_noise_sum / (g_CVD_Status_3CS.cvd_lock_stable_count + 1);
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE)	\
							&& (avg_noise_level > 0x3F0))
					{

						CVD_DEBUG("### avg noise level at stable20 [0x%x]#\n", avg_noise_level);
						CVD_H13Bx_Set_PALm_Stable_VSync_Mode(1);
					}
					else
						CVD_H13Bx_Set_PALm_Stable_VSync_Mode(0);
#endif

#ifdef	H13_PAL_STABLE_VSYNC_WORKAROUND
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALi)&& (g_CVD_Status_3CS.in_rf_mode == TRUE)\
							&& (g_CVD_Status_3CS.in_atv_tunning == FALSE)) 
						CVD_H13Bx_Set_Vsync_Cntl(&g_SetColorSystem_3CS_H13B0[CVD_STATE_PALi], 2);	// PLL vsync
#endif


					g_CVD_Status_3CS.cvd_lock_stable_count++;
				}
#if 0
				else if(g_CVD_Status_3CS.cvd_lock_stable_count == (CVD_STABLE_COUNT_1 + 2) )
				{
#ifdef H13_COMB2D_ONLY_CONTROL
					CVD_DEBUG("$$$ Comb2d only MD [0] $$$\n");
					CVD_H13Bx_Set_comb2d_only_md(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
#endif
					g_CVD_Status_3CS.cvd_lock_stable_count++;
				}
				else if(g_CVD_Status_3CS.cvd_lock_stable_count == (CVD_STABLE_COUNT_1 + 1) )
				{
#ifdef H13_COMB2D_ONLY_CONTROL
					CVD_DEBUG("$$$ Comb2d only [0] $$$\n");
					CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
#endif
					g_CVD_Status_3CS.cvd_lock_stable_count++;
				}
#endif
				else if(g_CVD_Status_3CS.cvd_lock_stable_count == (CVD_STABLE_COUNT_1 + 2) )
				{
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
					CVD_H13Bx_AGC_Peak_En_Control(2, 0, 0); // force on peak_en //gogosing agc

					/* 131219 : disable agc half en control : dark video after channel change (from no signal)*/
					/*
					if( g_CVD_Status_3CS.in_rf_mode == TRUE )
					{
						if(!g_CVD_Status_3CS.use_internal_demod)	// if using internal demod, agc_half_en should be '1'
							CVD_H13Bx_agc_half_en_control(0);//gogosing agc
					}
					*/
#endif
					g_CVD_Status_3CS.cvd_lock_stable_count++;
				}
				else if(g_CVD_Status_3CS.cvd_lock_stable_count == CVD_STABLE_COUNT_1)
				{
					CVD_H13Bx_Init_Level_Detector(g_CVD_Status_3CS.cvd_color_system);

#ifdef L9_DONT_KILL_CHROMALOCK_ON_CLOCK_LOST
					/*
					if( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) )
						CVD_H13Bx_OnOff_Chromalock_Ckill(0);
						*/
#endif
#ifdef	H13_STABLE_HSYNC_WORKAROUND
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE))
					{
							CVD_H13Bx_Enable_Stable_Sync_Mode(2);
					}
#endif

					//121206
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
					// peak_en :enable is moved to stable2 state for faster agc operation
				    //CVD_H13Bx_AGC_Peak_En_Control(2, 0, 0); // force on peak_en //gogosing agc
					/* 131219 : disable agc half en control : dark video after channel change (from no signal)*/
					/*
					if( g_CVD_Status_3CS.in_rf_mode == TRUE )
					{
						CVD_H13Bx_agc_half_en_control(0);//gogosing agc
					}
					*/
#endif
					//					CVD_DEBUG("### CVD Same State Max 1 !!! ###\n");

					//won.hur : added chip selection for L9B0
					//@2011.08.24
#ifdef H13_FAST_3DCOMB_WORKAROUND
					// Disable vf_nstd_en on stable state.(WA for 3DComb buffer control error)
					CVD_H13Bx_vf_nstd_control(0);	//for fast 3DComb operation
					// Set hnon_std_threshold values on stable state.(WA for 3DComb buffer control error)
					CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_hnon_std_threshold);
#endif

#ifdef L9_CHANGE_FIELD_DETECT_MODE_ON_DONG_GO_DONG_RAK
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) )
						CVD_H13Bx_Set_for_Field_Detect_Mode(1);
#endif
					//CVD_H13Bx_Program_Color_System_PostJob(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state]);
#ifdef L9_WA_FOR_EBS_STABLE_AGC
					// set reg_agc_half_en to '1', and reg_nstd_hysis to '0' for EBS field stream
					// set reg_agc_peak_cntl to '0', and reg_dcrestore_gain to '3' for stable AGC on NTSC RF
					// 120309 : modified not to execute SW workaround for stable AGC during ATV auto searching. This workaround sometimes makes channel skips.
					if (((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) || (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm)) && (g_CVD_Status_3CS.in_rf_mode == TRUE)  && (g_CVD_Status_3CS.in_atv_tunning == FALSE))
					{
						g_Stable_AGC_WA_for_EBS_Enabled = 1;
						CVD_H13Bx_Program_Color_System_PostJob2();
					}
#endif

#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
					if ( ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) || (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALi) )&& (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.in_atv_tunning == FALSE)) {
						//	CVD_DEBUG("Set hstate_fixed value to 1 \n");
						CVD_H13Bx_Set_Hstate_Fixed(1);
						if(g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC)
						{
							/* CH-17 hsync shift problem after channel change */
							//CVD_H13Bx_Set_Hstate_Max(3);
							vcr_detect_count = 0;
						}
					}
#endif
#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
					if ((g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
						gVariance_Status_Cdto_Inc = CVD_H13Bx_Differential_Status_Cdto_Inc_Value();
						gAccumulated_Cordic_Freq_Value = CVD_H13Bx_Read_Cordic_Freq_Value();
					}
#endif

					g_CVD_Status_3CS.cvd_lock_stable_count++;
#ifdef CVD_CH42_SKIP_WORKAROUND
					//CVD_Clamp_Current_Control(0);	// 1118 change clamp current to zero
#endif
					//120222 : setting agc_peak_en initial values
					// PALi & SECAM RF : initially off
					// NTSC & others : initially on
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
	//				CVD_H13Bx_AGC_Peak_En_Control(0, 0, 0);	//force off peak_en
#endif

#ifdef	H13_PAL_STABLE_VSYNC_WORKAROUND
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALi)&& (g_CVD_Status_3CS.in_rf_mode == TRUE)\
							&& (g_CVD_Status_3CS.in_atv_tunning == FALSE)) 
						CVD_H13Bx_Set_Vsync_Cntl(&g_SetColorSystem_3CS_H13B0[CVD_STATE_PALi], 1);	// vsync_l_cnt & vactive_start
#endif
				}

				else if(g_CVD_Status_3CS.cvd_lock_stable_count < CVD_STABLE_COUNT_20)
				{
#ifdef	H13_3DCOMB_ERROR_WORKAROUND
					if(g_CVD_Status_3CS.cvd_lock_stable_count == 0)
					{
						CVD_H13Bx_Program_Color_System_Size(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system]);
						CVD_H13Bx_3DComb_Workaround(2);
						CVD_H13Bx_Program_Color_System_Size_md(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system]);
						CVD_H13Bx_3DComb_Workaround(3);
					}
#ifdef H13_CDTO_CONTROL
					else if(g_CVD_Status_3CS.cvd_lock_stable_count == 1)
					{
						CVD_H13Bx_3DComb_Workaround(4);
					}
#endif
#endif

#ifdef L9_FAST_3DCOMB_WORKAROUND
#if 0
					if( (g_CVD_Status_3CS.hnon_standard_flag) && (g_CVD_Status_3CS.in_atv_tunning == FALSE) && (g_CVD_Status_3CS.cvd_color_system != CVD_STATE_SECAM) && (g_CVD_Status_3CS.cvd_color_system != CVD_STATE_NTSC443)) {	// hnon standard detected  && not in auto tuning && color system is not in SECAM/NTSC443
						CVD_H13Bx_Set_HNon_Standard_Threshold(0);
					}
					else
						CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_hnon_std_threshold);

#endif
#endif
#ifdef L9_ADAPTIVE_BURST_GATE_END_MODE
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALm) && (g_CVD_Status_3CS.in_rf_mode == TRUE) ) {
						gVariance_Status_Cdto_Inc += CVD_H13Bx_Differential_Status_Cdto_Inc_Value();
						gAccumulated_Cordic_Freq_Value += CVD_H13Bx_Read_Cordic_Freq_Value();
					}
#endif
#ifdef L9_HSTATE_FIXED_CONTROL_FOR_STABLE_SYNC
					if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC)&& (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.in_atv_tunning == FALSE)) {
						if(CVD_H13Bx_Read_VCR_Detected() && (CVD_H13Bx_Get_Global_Motion_Value() > 0x10)  && (g_CVD_Status_3CS.hnon_standard_flag == 0) ) // When Global Motion Value is high
							vcr_detect_count++;
					}

					if(g_CVD_Status_3CS.cvd_lock_stable_count == 10)
					{
						// CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD : 2=>4 
						if ( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC)&& (g_CVD_Status_3CS.in_rf_mode == TRUE) && (g_CVD_Status_3CS.in_atv_tunning == FALSE)) {
							if ( (vcr_detect_count+1 >= CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD>>1) && !g_CVD_Pattern_Detection_t.static_pattern_found)
							{
								CVD_WA_DEBUG("VCR Detected [%d/%d] \n", vcr_detect_count+1, CVD_HSTATE_FIX_VCR_COUNT_THRESHOLD>>1);
								CVD_H13Bx_Set_Hstate_Max(3);
								CVD_H13Bx_Set_Hstate_Fixed(0);		// VCR tape Rating issue
							}
							else {
								CVD_H13Bx_Set_Hstate_Max(5);
								CVD_H13Bx_Set_Hstate_Fixed(1);		// VCR tape Rating issue
							}
						}
					}

#endif
					if(g_CVD_Status_3CS.cvd_lock_stable_count > (CVD_STABLE_COUNT_1 + 1) )
					{
						/*
						   if( (CVD_H13Bx_Get_comb2d_only() == 0) && CVD_H13Bx_Get_comb2d_only_md() )
						   {
						   CVD_DEBUG("$$$ Comb2d only MD [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
						   CVD_H13Bx_Set_comb2d_only_md(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
						   }
						   else if(g_CVD_Status_3CS.h_lock_flag && g_CVD_Status_3CS.v_lock_flag && g_CVD_Status_3CS.cvd_chromalock_flag && CVD_H13Bx_Get_comb2d_only()\
						   && (g_CVD_Status_3CS.hnon_standard_flag == 0) && (g_CVD_Status_3CS.vnon_standard_flag == 0) )
						   {
						   CVD_DEBUG("$$$ Comb2d only [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
						   CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
						   }
						 */
						if(g_CVD_Status_3CS.h_lock_flag && g_CVD_Status_3CS.v_lock_flag && g_CVD_Status_3CS.cvd_chromalock_flag && CVD_H13Bx_Get_comb2d_only()\
								&& (g_CVD_Status_3CS.hnon_standard_flag == 0) && (g_CVD_Status_3CS.vnon_standard_flag == 0) )
						{
#if 0
#ifdef H13_COMB2D_ONLY_CONTROL
							CVD_DEBUG("$$$ Comb2d only [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
							CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
							CVD_DEBUG("$$$ Comb2d only MD [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
							CVD_H13Bx_Set_comb2d_only_md(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
#endif
#ifdef H13_FAST_3DCOMB_WORKAROUND
							// Disable vf_nstd_en on stable state.(WA for 3DComb buffer control error)
							CVD_H13Bx_vf_nstd_control(0);	//for fast 3DComb operation
							// Set hnon_std_threshold values on stable state.(WA for 3DComb buffer control error)
							CVD_H13Bx_Set_HNon_Standard_Threshold(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_hnon_std_threshold);
#endif
#endif
						}
					}
					g_CVD_Status_3CS.cvd_lock_stable_count++;
				}

				if(g_CVD_Status_3CS.cvd_lock_stable_count > 0)
				{
					//Enable NoBurst CKill Control after color system is detected and stable
					//CVD_H13Bx_NoBurst_CKill_Control(&g_CVD_Status_3CS, 0);

					//120711 : adaptive cstate control for sub-carrier pull-in-range test
					//CVD_H13Bx_Cstate_Control_for_SC_PullInRange(&g_CVD_Status_3CS, 0);
					CVD_H13Bx_No_Color_Cstate_Control(&g_CVD_Status_3CS, 0);
				}

				if(g_CVD_Status_3CS.cvd_lock_stable_count > CVD_STABLE_COUNT_1)
				{

					if (( g_CVD_Pattern_Detection_t.bEnable == TRUE) && (g_CVD_Status_3CS.in_rf_mode == TRUE) )
						CVD_H13Bx_Pattern_Detection(&g_CVD_Pattern_Detection_t );
#if 1
					//gogosing burst mag status check for color burst level test (color 틀어짐 대응) 2011.06.11
					//Apply cagc control for NTSC(color 틀어짐 대응) 2011.10.14
					if( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_PALi) && (g_CVD_Status_3CS.in_rf_mode == TRUE)\
							&& (g_CVD_Status_3CS.avg_status_noise < 110) )
					{
						burst_mag_prev = burst_mag_next;
						burst_mag_next = CVD_H13Bx_Get_CVD_Burst_Mag_Flag(g_CVD_Status_3CS.cvd_color_system);

						if ( (burst_mag_next != burst_mag_prev)  )
						{
							CVD_WA_DEBUG("burst_mag_prev = %d , burst_mag_next = %d \n",(int)burst_mag_prev,(int)burst_mag_next);
							CVD_H13Bx_Set_CVD_CAGC(burst_mag_next,&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system], g_CVD_Status_3CS.cvd_color_system);
							//CVD_DEBUG("################# CAGC changed gogosing ############### \n");
						}
					}
					else if( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == TRUE) )
					{
						//	burst_mag_prev = burst_mag_next;
						//	burst_mag_next = CVD_Get_CVD_Burst_Mag_Flag(g_CVD_Status_3CS.cvd_color_system);

						// 121207 : FSC Motion Expand : Diagonal Line on Boundary of ColorBar Pattern when FSC shift(+/- 10Hz)
						if( g_CVD_Pattern_Detection_t.pattern_found )
						{
							CVD_H13Bx_Set_Motion_Expand(1);
							CVD_H13Bx_Set_CCR(0);
						}
						else 
						{
							CVD_H13Bx_Set_Motion_Expand(0);
							CVD_H13Bx_Set_CCR(1);
						}

						burst_mag_sum = 0;

						for(loop = 9;loop > 0;loop--) {
							burst_mag_array[loop] = burst_mag_array[loop-1] ;
							burst_mag_sum += burst_mag_array[loop];
						}

						burst_mag_array[0] = CVD_H13Bx_Get_CVD_Burst_Mag_Value();

						burst_mag_sum += burst_mag_array[0];

						burst_mag_avg = burst_mag_sum / 10;

#ifdef BURST_GATE_ADJUST_FOR_BURST_POSITION_SHIFT
						//130916 : sub carrier delay
						// Modify burst_gate_start/end 
						if (g_CVD_Status_3CS.avg_status_noise < 110) 
						{
							if( g_CVD_Pattern_Detection_t.pattern_found )
							{
								if ( (burst_mag_avg < 0x600) && ( g_CVD_Status_3CS.burst_gate_modified != 2 ) )
								{
									CVD_H13Bx_Burst_Gate_Control(0x2a, 0x49);
									g_CVD_Status_3CS.burst_gate_modified = 2;
								}
								else if ( (burst_mag_avg > 0x610) && (burst_mag_avg < 0x800) && ( g_CVD_Status_3CS.burst_gate_modified == 2) )
								{
									CVD_H13Bx_Burst_Gate_Control(0x2e, 0x49);
									//CVD_H13Bx_Burst_Gate_Control(0x30, 0x49);
									g_CVD_Status_3CS.burst_gate_modified = 1;
								}
								else if ( (burst_mag_avg > 0x600) && (burst_mag_avg < 0x800) && ( g_CVD_Status_3CS.burst_gate_modified != 1) )
								{
									CVD_H13Bx_Burst_Gate_Control(0x2e, 0x49);
									//CVD_H13Bx_Burst_Gate_Control(0x30, 0x49);
									g_CVD_Status_3CS.burst_gate_modified = 1;
								}
								else if ( (burst_mag_avg > 0x810) && ( g_CVD_Status_3CS.burst_gate_modified != 0) )
								{
									CVD_H13Bx_Burst_Gate_Control(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_start_new, g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_end_new);

									g_CVD_Status_3CS.burst_gate_modified = 0;
								}
							}
							else if( g_CVD_Status_3CS.burst_gate_modified ) {
								CVD_H13Bx_Burst_Gate_Control(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_start_new, g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system].reg_burst_gate_end_new);

								g_CVD_Status_3CS.burst_gate_modified = 0;
							}
						}
#endif

						//120127 : modified by kd.park
						//120131 : if pattern not found do not adjust cagc register
#if 0
						if( (burst_mag_avg > 0xbc0) )
#endif

							if( (burst_mag_avg > 0xbc0) || ( g_CVD_Pattern_Detection_t.pattern_found != 1)
								|| (g_CVD_Status_3CS.cvd_chromalock_flag == 0 ) ){	// added for burst mag test (chroma unlock)
								burst_mag_next = CVD_BURST_MAG_STATE_BIG;
								prev_burst_mag_avg = 0xbc0;

								//121114 : why reg_saturation???
								//cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_saturation;

								if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_HIGH)	// NTSC-J
								{
									cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc;

									if(CVD_H13Bx_Get_CAGC_Value() != g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc)
									{
										CVD_WA_DEBUG("NTSC-J Mode : Set CAGC to [0x%x]\n", g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc);
										CVD_H13Bx_Set_CAGC_Value(g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc);
										CVD_H13Bx_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_HIGH, LX_CVD_INPUT_SOURCE_ATTRIBUTE_RF);
									}
								}
								else if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_LOW)	// NTSC-M
								{
									cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc;
									if(CVD_H13Bx_Get_CAGC_Value() != g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc)
									{
										CVD_WA_DEBUG("NTSC-M Mode : Set CAGC to [0x%x]\n", g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc);
										CVD_H13Bx_Set_CAGC_Value(g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc);
										CVD_H13Bx_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_LOW, LX_CVD_INPUT_SOURCE_ATTRIBUTE_RF);
									}
								}
								else
								{
									cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc;
									if(CVD_H13Bx_Get_CAGC_Value() != g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc)
									{
										CVD_WA_DEBUG("Auto Mode : Set CAGC to [0x%x]\n", g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc);
										CVD_H13Bx_Set_CAGC_Value(g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc);
										CVD_H13Bx_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_LOW, LX_CVD_INPUT_SOURCE_ATTRIBUTE_RF);
									}
									//	CVD_WA_DEBUG("NTSC Auto Mode \n");
								}

							}
							else if( (burst_mag_avg < 0x280) && \
								(g_CVD_Status_3CS.avg_status_noise < 110) )
							{
								burst_mag_next = CVD_BURST_MAG_STATE_VERY_SMALL;
								prev_burst_mag_avg = 0x280;
								cagc_target = 0x20;
							}
							else if (g_CVD_Status_3CS.avg_status_noise < 110) {
								burst_mag_next = (burst_mag_avg / 0x17) + 0x5;
								if(burst_mag_next > g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_saturation)
									burst_mag_next = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_saturation;
								cagc_target = burst_mag_next;
							}
							else
							{
								CVD_THREAD_DEBUG("color bar pattern found,but noisy [%d]\n", g_CVD_Status_3CS.avg_status_noise);
							}

						//CVD_DEBUG("burst_mag = [0x%x], burst_mag_avg = [0x%x], prev_burst_mag_avg = [0x%x], burst_mag_next [0x%x], burst_mag_prev[0x%x] \n",burst_mag_array[0], burst_mag_avg, prev_burst_mag_avg, burst_mag_next, burst_mag_prev);

						if ( ( ( (burst_mag_next >= 0x5) && ( abs(prev_burst_mag_avg - burst_mag_avg) > 0x80) )  \
									||  ( ( burst_mag_next < 5) && (burst_mag_next != burst_mag_prev) ) ) \
								&& (g_CVD_Status_3CS.cvd_lock_stable_count > (CVD_STABLE_COUNT_1 + 10 )) )
						{
							CVD_WA_DEBUG("burst_mag_prev = 0x%x , burst_mag_next = 0x%x \n",burst_mag_prev,burst_mag_next);
							CVD_WA_DEBUG("burst_mag_avg = 0x%x\n",burst_mag_avg);

							cagc_written = CVD_H13Bx_Get_CAGC_Value();

							CVD_WA_DEBUG("cagc_written = 0x%x , cagc_target = 0x%x \n",cagc_written,cagc_target);

							if(cagc_written > cagc_target)
								burst_mag_next = cagc_written - 1;
							else if(cagc_written < cagc_target)
								burst_mag_next = cagc_written + 1;
							else {
								CVD_WA_DEBUG("CAGC Target Reached !!! [0x%x]\n",cagc_written);
								prev_burst_mag_avg = burst_mag_avg;
								burst_mag_prev = burst_mag_next;
								burst_mag_next = cagc_written;
							}

							CVD_H13Bx_Set_CVD_CAGC(burst_mag_next,&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system], g_CVD_Status_3CS.cvd_color_system);
							//CVD_DEBUG("################# CAGC changed gogosing ############### \n");
						}

					}
					else if( (g_CVD_Status_3CS.cvd_color_system == CVD_STATE_NTSC) && (g_CVD_Status_3CS.in_rf_mode == FALSE) )	// For NTSC AV
					{
						if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_HIGH)	// NTSC-J
						{
							cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc_av;

							if(CVD_H13Bx_Get_CAGC_Value() != g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc_av)
							{
								CVD_WA_DEBUG("NTSC-J Mode : Set CAGC to [0x%x]\n", g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc_av);
								CVD_H13Bx_Set_CAGC_Value(g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSCj].reg_cagc_av);
								CVD_H13Bx_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_HIGH, LX_CVD_INPUT_SOURCE_ATTRIBUTE_AV);
							}
						}
						else if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_LOW)	// NTSC-M
						{
							cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc_av;
							if(CVD_H13Bx_Get_CAGC_Value() != g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc_av)
							{
								CVD_WA_DEBUG("NTSC-M Mode : Set CAGC to [0x%x]\n", g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc_av);
								CVD_H13Bx_Set_CAGC_Value(g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc_av);
								CVD_H13Bx_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_LOW, LX_CVD_INPUT_SOURCE_ATTRIBUTE_AV);
							}
						}
						else
						{
							//	CVD_WA_DEBUG("NTSC Auto Mode \n");
							cagc_target = g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC].reg_cagc_av;
						}
					}
					else
					{
						/*
						   burst_mag_prev=0;//pal 이외 시스템에서 초기화
						   burst_mag_next=0;
						 */
					}

					if( g_CVD_Status_3CS.in_rf_mode == FALSE )	// For SCART Black Level Control
					{

						if(prev_black_level != g_CVD_Status_3CS.cvd_black_level)
						{
							CVD_DEBUG("SCART Black Level to [%d]\n", g_CVD_Status_3CS.cvd_black_level);

							if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_HIGH)
							{
								CVD_H13Bx_Set_SCART_CSC(1);
							}
							else if(g_CVD_Status_3CS.cvd_black_level == LX_AFE_CVD_BLACK_LEVEL_LOW)
							{
								CVD_H13Bx_Set_SCART_CSC(0);
							}
							else
							{
							}
							prev_black_level = g_CVD_Status_3CS.cvd_black_level;
						}
					}
#endif

				}

			}
		}
		// #10 	: Same State , but no signal
		//		:
		else
		{
#ifdef H13_ADAPTIVE_AGC_PEAK_EN_CONTROL
			if( (g_CVD_Status_3CS.h_lock_flag == 0) && ( g_CVD_Status_3CS.v_lock_flag == 0) )
			{
				// to prevent frequent agc peak control on weak RF signal
				// set agc peak en off only when h_lock & v_lock are unlocked
				CVD_H13Bx_AGC_Peak_En_Control(0, 0, 0); //force off peak_en
				if( g_CVD_Status_3CS.in_rf_mode == TRUE )
				{
					CVD_H13Bx_agc_half_en_control(1);//gogosing agc
				}
			}
#endif
			g_CVD_Status_3CS.cvd_lock_stable_count = 0;
		}

		//g_CVD_Status_3CS.cvd_color_system = g_CVD_Status_3CS.cvd_next_state;
#if 0
		//periodic reset of CS0/CS1 is needed???
		if ( lx_chip_rev( ) >= LX_CHIP_REV( L9, B0 ) )
		{
			CVD_H13Bx_swrst_CS( CVD_SEL_CS_CS0);
			CVD_H13Bx_swrst_CS( CVD_SEL_CS_CS1);
		}
#endif


	}
	/*
	if(g_mute_off_count < 20)
	{
		g_mute_off_count ++;
	}
	else if(g_mute_off_count == 20) {

		AFE_PRINT("### 1 SEC after Mute off ###\n");
#ifdef H13_COMB2D_ONLY_CONTROL
#ifndef H13_3DCOMB_TEST_TMP
		CVD_DEBUG("$$$ Comb2d only [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
		CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
	//	CVD_DEBUG("$$$ Comb2d only MD [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
	//	CVD_H13Bx_Set_comb2d_only_md(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
#endif
#endif
		g_mute_off_count ++;
	}
	*/


//	CVD_H13Bx_Comb2d_Only_Test(0,0,0,0,0,0);

	return 0;
}

static CVD_STATE_T CVD_H13Bx_Check_Color_System_Support(LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	forced_color_system)
{
	CVD_STATE_T	system_ret = CVD_STATE_Default;

	// For user force color system
	if(forced_color_system == LX_COLOR_SYSTEM_NTSC_M)		// For Kor/US
		system_ret = CVD_STATE_NTSC;
	else if(forced_color_system == LX_COLOR_SYSTEM_PAL_G)
		system_ret = CVD_STATE_PALi;
	else if(forced_color_system == LX_COLOR_SYSTEM_PAL_NC)
		system_ret = CVD_STATE_PALCn;
	else if(forced_color_system == LX_COLOR_SYSTEM_PAL_M)
		system_ret = CVD_STATE_PALm;
	else if(forced_color_system == LX_COLOR_SYSTEM_SECAM)
		system_ret = CVD_STATE_SECAM;
	else if(forced_color_system == LX_COLOR_SYSTEM_NTSC_443)
		system_ret = CVD_STATE_NTSC443;
	else if(forced_color_system == LX_COLOR_SYSTEM_PAL_60)
		system_ret = CVD_STATE_PAL60;

	return system_ret;
}

int CVD_H13Bx_Channel_Change_Control(BOOLEAN bATV_Channel_Change)
{
	if(bATV_Channel_Change)
	{
#ifdef H13_COMB2D_ONLY_CONTROL
#ifndef H13_3DCOMB_TEST_TMP
		CVD_H13Bx_Set_comb2d_only(1);
		CVD_H13Bx_Set_comb2d_only_md(1);
		CVD_DEBUG("### Comb2d only [1] [%s][%d] ###\n",__func__, __LINE__ );
#endif
#endif
		CVD_DEBUG("ATV Channel Change Detected !!!\n");

//		g_CVD_Status_3CS.port_change_detected = TRUE;
	}
	else
	{
		CVD_DEBUG("### Mute off ### : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
#ifdef H13_COMB2D_ONLY_CONTROL
#ifndef H13_3DCOMB_TEST_TMP
		CVD_DEBUG("$$$ Comb2d only [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
		CVD_H13Bx_Set_comb2d_only(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
	//	CVD_DEBUG("$$$ Comb2d only MD [0] : stable count[%d]$$$\n", g_CVD_Status_3CS.cvd_lock_stable_count);
	//	CVD_H13Bx_Set_comb2d_only_md(g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_next_state].reg_comb2d_only);
#endif
#endif
		CVD_DEBUG("ATV Channel Change End !!!\n");
		g_mute_off_count = 0;
	}

	return 0;
}

int CVD_H13Bx_Set_Color_System_3CS(CVD_STATE_T cvd_next_state)
{
	switch(cvd_next_state)
	{
		case CVD_STATE_NTSC:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_NTSC;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_PAL60;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALm;
			break;

		case CVD_STATE_NTSC443:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_NTSC443;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_NTSC;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALm;
			break;

		case CVD_STATE_PALm:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_PALm;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_PAL60;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_NTSC;
			break;

		case CVD_STATE_PAL60:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_PAL60;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_NTSC;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALm;
			break;

		case CVD_STATE_PALi:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_PALi;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_SECAM;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALCn;
			break;

		case CVD_STATE_PALCn:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_PALCn;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_SECAM;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALi;
			break;

		case CVD_STATE_SECAM:
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_SECAM;
			g_CVD_Status_3CS.cs0_color_system = CVD_STATE_PALi;
			g_CVD_Status_3CS.cs1_color_system = CVD_STATE_PALCn;
			break;

		default:
			AFE_PRINT("ERROR INVALID COLOR SYSTEM!!![%d]\n", cvd_next_state);
			break;
	}

	if(g_CVD_Status_3CS.cvd_color_system <= CVD_STATE_SECAM)
	{

		CVD_CSD_DEBUG("Set Color System : CVD[%d], CS0[%d], CS1[%d]\n", g_CVD_Status_3CS.cvd_color_system, g_CVD_Status_3CS.cs0_color_system, g_CVD_Status_3CS.cs1_color_system);
		/*
		   CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system]);
		   CVD_H13Bx_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC]);
		 */
		CVD_H13Bx_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cvd_color_system]);


		CVD_H13Bx_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cs0_color_system]);
		CVD_H13Bx_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_H13B0[g_CVD_Status_3CS.cs1_color_system]);

		//111221 by kim.min for better color standard detection performance
		CVD_H13Bx_swrst_CS( CVD_SEL_CS_CS0);
		CVD_H13Bx_swrst_CS( CVD_SEL_CS_CS1);
	}

	return 0;
}

