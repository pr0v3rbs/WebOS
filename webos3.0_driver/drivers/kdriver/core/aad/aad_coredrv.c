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
 *  AAD core driver functions.
 *	aad device will teach you how to make device driver with new platform.
 *
 *  author		Sudhaher (kanthiraj.s@lge.com)
 *  version		1.0
 *  date		2010.02.24
 *  note		Additional information.
 *
 *  @addtogroup lg1150_aad
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "os_util.h" //for alloc functions
#include <linux/kthread.h>

#include "aad_drv.h"
#include "aad_coredrv.h"
#include "aad_kapi.h"
#include "debug_util.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* KDRV AAD DRIVER Version Info. */
#define	KDRV_AAD_DRV_VERSION		2014111100


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern wait_queue_head_t 	AAD_ASD_Task_wait;
extern int 					AAD_ASD_Task_waitq_flag;


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
//define AAD Reg.
volatile AAD_REG_T		*gpRealRegAAD;
AAD_REG_T				*gpRegAAD;

//This value is set by Suspend and Resume.
BOOLEAN					g_bAadSuspended = FALSE;

//This value is set by AAD_SIF_InitModule function.
LX_AAD_INIT_TYPE_T				g_eAadInitType		= LX_AAD_INIT_TYPE_NONE;

//This value is set by IOCTL function.
BOOLEAN							g_bHighDevOnOff		= FALSE;	///< The Status of High Deviation to setted

//This value is used by AAD_RegInitAfterAADReset function.
LX_AAD_HIGH_DEV_SET_T 			g_eHighDevMode		= {FALSE, LX_AAD_HIGH_DEV_LEVEL_OFF};
LX_AAD_SIF_THRESHOLD_T 			g_stThresholdLevel	= {LX_AAD_THRES_NONE,				\
													   CARRIER2_MAGNITUDE_THRESHOLD,	\
													   CARRIER2_QUALITY_THRESHOLD_HI,	\
													   CARRIER2_QUALITY_THRESHOLD_LO,	\
													   ID_MAGNITUDE_THRESHOLD,			\
													   ID_MAGNITUDE_THRESHOLD,			\
													   NICAM_ERROR_AAOS_THD_HI,			\
													   NICAM_ERROR_AAOS_THD_LO};

//This value is set by AAD_SIF_SetBandSetup.
LX_AAD_SIF_SOUNDSYSTEM_T		g_i_eSystem 		= LX_AAD_SIF_SYSTEM_UNKNOWN;

//This value is set by AAD_SIF_SetModeSetup.
LX_AAD_SIF_STANDARD_T			g_i_eStandard 		= LX_AAD_SIF_NUM_SOUND_STD;

//This value is set by AAD_SIF_SetBandSetup.
LX_AAD_SIF_STANDARD_INFO_T		g_io_stStandardInfo = {LX_AAD_SIF_DETECTING_AVALIBILITY,	\
													   LX_AAD_SIF_DETECTING_AVALIBILITY,
													   LX_AAD_SIF_DETECTING_EXSISTANCE };

//This value is set by AAD_SIF_GetCurAnalogMode.
LX_ATV_AUDIO_MODE_GET_T 		g_o_eAudioMode 		= LX_ATV_GET_NTSC_BTSC_UNKNOWN;

//This value is set by AAD_SIF_SetUserAnalogMode.
LX_ATV_AUDIO_MODE_SET_T 		g_i_eAudioModeSet 	= LX_ATV_SET_NTSC_BTSC_UNKNOWN;

//This value is set by AAD_NTSC_MN_A2_Monitor_Task.
LX_AAD_A2_MONO_ST_STATE_T		g_stCurMonoStState 	= LX_AAD_A2_DEFAULT0_STATE;

//Current LX_AAD_SIF_INPUT_T sifSource
LX_AAD_SIF_INPUT_T 				g_i_eSifSource 		= LX_AAD_SIF_INPUT_INTERNAL;

//Threshold PAL B High Deviation Off Count for India.
UINT8	g_ui8PALBHighDevThreshold = HIGH_DEV_OFF_COUNT_IN;

//Current PAL B High Deviation Task Run Count for India.
UINT8	g_ui8PALBTaskCount = 0;

//Current PAL B High Deviation Enabled Status for India.
UINT8	g_ui8HighDevEabled = HIGH_DEV_UNKNOWN_STATUS;

//Current PAL B High Deviation Check Status for India.
UINT8	g_ui8PALBContiCount = 0;

//Current PAL B High Deviation On Check Status for India.
UINT8	g_ui8PALBContiCountOn = 0;

//Current PAL B/G Carrier1 Quality Value for Indonesia
UINT16	g_ui16Carrier1Quality_ID = 0;


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int AAD_DoASD ( LX_AAD_SIF_SOUNDSYSTEM_T eSoundSystem, BOOLEAN bScanMode );
static int AAD_SetAndStartASD ( AAD_ASD_CARRIER_T eASDCar, UINT8 ui8RegValue, UINT32 ui32Timeout );
static int AAD_DetectSoundSystem( LX_AAD_SIF_SOUNDSYSTEM_T eSoundSystem );
static int AAD_ASD_CheckCarrierDetection ( void );
static LX_AAD_SIF_SOUNDSYSTEM_T AAD_GetSoundSystem (void);
static int AAD_SetManualVolumeForHDEVMode ( LX_ATV_AUDIO_MODE_SET_T eAudioModeSet );
static int AAD_SECAM_L_GetManualVolumeForAdjust ( void );
static int AAD_SECAM_L_SetManualVolumeForAdjust_H1x ( void );
static int AAD_SECAM_L_SetNICAMVolume ( void );
static int AAD_NTSC_MN_A2_Monitor_Task( void );
static int AAD_PAL_DK_A2_Monitor_Task( void );
static int AAD_PAL_B_A2_India_Monitor_Task( void );


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
//semaphore for AAD Module
static OS_SEM_T	g_AAD_Sema;

//SECAM L Weight Value for Output Volume Adjust
static UINT32	g_ui32AadSECAMWeight = NUMBER_OF_SECAM_L_WEIGHT;

//SECAM L Magnitude Value for Output Volume Adjust
static UINT32	g_ui32AadSECAMVolume[NUMBER_OF_SECAM_L_MAGNITUDE] = {0x2E00, 0x2E80, 0x2F00, 0x2F80, 0x3000, 0x3080, 0x3100, 0x3180, 0x3200, 0x3280, 0x3300};

//SECAM L Task Disable for Output Volume Adjust
static UINT32	g_ui32AadSECAMDisable = FALSE;

//SECAM L Magnitude Value for Output Volume Adjust in H13
static UINT32	g_ui32AadSECAMVolume_H13[NUMBER_OF_SECAM_L_MAGNITUDE_H13] = {0x2F40, 0x2FC0, 0x3040, 0x30C0, 0x3140,	\
																			 0x31C0, 0x3240, 0x32C0, 0x3340, 0x33C0, 	\
																			 0x3440, 0x34C0, 0x3540, 0x35C0, 0x3640};

//SECAM L Task Disable for Output Volume Adjust in H13
static UINT32	g_ui32AadSECAMDisable_H13 = FALSE;

//SECAM L Magnitude Value for Output Volume Adjust in H15
static UINT32	g_ui32AadSECAMVolume_H15[NUMBER_OF_SECAM_L_MAGNITUDE_H15] = {0x2F40, 0x2FC0, 0x3040, 0x30C0, 0x3140,	\
																			 0x31C0, 0x3240, 0x32C0, 0x3340, 0x33C0, 	\
																			 0x3440, 0x34C0, 0x3540, 0x35C0, 0x3640};

//SECAM L Task Disable for Output Volume Adjust in H15
static UINT32	g_ui32AadSECAMDisable_H15 = FALSE;

//AAD Task Running Count for AAD Module
static UINT32	g_ui32AadTaskRunningCount = 0;

//g_ui32AadPrintCount is only increased in AAD_SIF_GetCurAnalogMode function
static UINT32	g_ui32AadPrintCount = 0;

/* Related to AAD monitor task */
static LX_AAD_TASK_MSG_T	g_suiAadWaitQForMonitor;
static wait_queue_head_t 	Aad_Monitor_WaitQ;
static DECLARE_WAIT_QUEUE_HEAD( Aad_Monitor_WaitQ );
static LX_ATV_AUDIO_MODE_GET_T 	g_MonitorAudioMode = LX_ATV_GET_NTSC_A2_UNKNOWN;


/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * Initializes the aad module.
 * AAD module shall be initialized itself with pre-defined configuration value.\n
 * AAD module includes Analog Audio Decoder parts.\n
 * And, serveral global variable is set by LX_AAD_INIT_TYPE_T value.
 * @li register device and interrupts.
*/
int AAD_SIF_InitModule ( LX_AAD_INIT_TYPE_T eInitType, BOOLEAN bInitialized )
{
	//Check a bInitialized status
	if(bInitialized == FALSE)
	{
		//Initialize AAD semaphore
		OS_InitMutex(&g_AAD_Sema, OS_SEM_ATTR_DEFAULT);
	}

	if ( eInitType == LX_AAD_INIT_ATSC_A2_SELECT )
	{
		AAD_DEBUG ("ATSC A2 SELECTED(KOREA)\n");
	}
	else if ( eInitType == LX_AAD_INIT_ATSC_BTSC_SELECT )
	{
		AAD_DEBUG ("BTSC SELECTED\n");
	}
	else if ( eInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
	{
		AAD_DEBUG ("BTSC SELECTED(US)\n");
	}
	else if ( eInitType == LX_AAD_INIT_DVB_SELECT )
	{
		AAD_DEBUG ("DVB SELECTED\n");
	}
	else if ( eInitType == LX_AAD_INIT_DVB_ID_SELECT )
	{
		AAD_DEBUG ("DVB SELECTED(ID)\n");
	}
	else if ( eInitType == LX_AAD_INIT_DVB_IN_SELECT )
	{
		AAD_DEBUG ("DVB SELECTED(IN)\n");
	}
	else if ( eInitType == LX_AAD_INIT_DVB_CN_SELECT )
	{
		AAD_DEBUG ("DVB SELECTED(CN)\n");
	}
	else if ( eInitType == LX_AAD_INIT_DVB_AJJA_SELECT )
	{
		AAD_DEBUG ("DVB SELECTED(AJJA)\n");
	}
	else
	{
		AAD_ERROR ("LX_AAD_INIT_TYPE_NONE(0x%X)!!!\n", g_eAadInitType);
	}

	//Copy AAD init type
	g_eAadInitType = eInitType;

	AAD_WARN ("AAD Initialzed 0x%X, Ver. = %d\n", eInitType, KDRV_AAD_DRV_VERSION);
	return RET_OK;
}

/**
 * Sets High Deviaton Level Value.
 * These settings do not apply to the single carrier hierarchical modulation modes(EIAJ, BTSC and FM-Radio).
 * @see
*/
int AAD_SIF_SetHighDeviationMode ( LX_AAD_HIGH_DEV_SET_T eHighDevMode )
{
	UINT8 	*debugString[5] = {"OFF(50KHz)", "Level 1(100KHz)", "Level 2(200KHz)",	\
							   "Level 3(384KHz)", "Level 4(540KHz)" };

	BOOLEAN bNicamMode = FALSE;

	//Mutes by MuteCt bit in Mute Control Register.
	AAD_RdFL(mute_ctrl);
	AAD_Wr01(mute_ctrl, mutect, 0b11);
	AAD_Wr01(mute_ctrl, muteov, 0b1);
	AAD_WrFL(mute_ctrl);

	//In IN(India) model, Adjust Volume Level.
	if ((g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT) 	\
	  &&(eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_OFF)	\
	  &&(lx_chip() != LX_CHIP_H13) )
	{
		//Read manual_output_level_left_ch reg.
		AAD_RdFL(manual_out_lvl_left_ch);

		//Read manual_output_level_right_ch reg.
		AAD_RdFL(manual_out_lvl_right_ch);

		//Read I2S Gain Control reg.
		AAD_RdFL(i2s_ctrl);

		AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x17); // HDEV option : Off
		AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x17); // suji.baek@lge.com(28 -> 17), 2014.11.11

		//Set a 0-bit shitf for I2S volume for a left, right volume
		AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x0); // 0-bit shift

		//Write manual_output_level_left_ch reg.
		AAD_WrFL(manual_out_lvl_left_ch);

		//Write manual_output_level_right_ch reg.
		AAD_WrFL(manual_out_lvl_right_ch);

		//Write I2S Gain Control reg.
		AAD_WrFL(i2s_ctrl);
	}

	//Get high deviation level using FM AM demod 1 control register
	AAD_RdFL(fm_am_demod_ctrl);

	switch (eHighDevMode.highDevLevel)
	{
		case LX_AAD_HIGH_DEV_LEVEL_OFF: ///< High Deviation OFF(50KHz)
			AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			break;

		case LX_AAD_HIGH_DEV_LEVEL_1: 	///< High Deviation Level 1(100KHz)
			//In DVB model, set also Demod 1 filter
			if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT		\
			  || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT	\
			  || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT 	\
			  || g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
			{
				//Demod 1 Maximum FM Deviation: 100kHz
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x1);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);

				//Demod 1 Filter: 200kHz lowpass to reduce noise in high deviation(300 ~ 400 %)
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x1);
			}
			else
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x1);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			break;

		case LX_AAD_HIGH_DEV_LEVEL_2: 	///< High Deviation Level 2(200KHz)
			//In DVB model, set also Demod 1 filter
			if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT		\
			  || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT  \
			  || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT 	\
			  || g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x1);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x1);
			}
			else
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x1);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			break;

		case LX_AAD_HIGH_DEV_LEVEL_3: 	///< High Deviation Level 3(384KHz)
			//In DVB model, set also Demod 1 filter
			if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT		\
			  || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT  \
			  || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT 	\
			  || g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x2);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x2);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			else
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x2);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			break;

		case LX_AAD_HIGH_DEV_LEVEL_4: 	///< High Deviation Level 4(540KHz)
			//In IN(India) model, set also Demod 1 filter
			if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT		\
			  || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT  \
			  || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT 	\
			  || g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x3);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x3);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			else
			{
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x3);
				AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			}
			break;

		default:
			AAD_DEBUG ("High Deviation : RET_INVALID_PARAMS 1\n");
			AAD_Wr01(fm_am_demod_ctrl, dmd1dv,  0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1dv2, 0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1fl,  0x0);
			AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);
			break;
	}

	//Set high deviation level using FM AM demod 1 control register
	AAD_WrFL(fm_am_demod_ctrl);

	//Set DEMOD2 function
	//Get high deviation level using FM AM demod 2 control register
	AAD_RdFL(fm_dqpsk_demod_ctrl);

	switch (eHighDevMode.highDevLevel)
	{
		case LX_AAD_HIGH_DEV_LEVEL_OFF: ///< High Deviation OFF(50KHz)
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x0);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x0);
			break;

		case LX_AAD_HIGH_DEV_LEVEL_1: 	///< High Deviation Level 1(100KHz)
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x1);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x0);
			break;

		case LX_AAD_HIGH_DEV_LEVEL_2: 	///< High Deviation Level 2(200KHz)
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x0);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x1);
			break;

		case LX_AAD_HIGH_DEV_LEVEL_3: 	///< High Deviation Level 3(384KHz)
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x2);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x0);
			break;

		case LX_AAD_HIGH_DEV_LEVEL_4: 	///< High Deviation Level 4(540KHz)
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x3);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x0);
			break;

		default:
			AAD_DEBUG ("High Deviation : RET_INVALID_PARAMS 1\n");
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv,  0x0);
			AAD_Wr01(fm_dqpsk_demod_ctrl, dmd2dv2, 0x0);
			break;
	}

	//Set high deviation level using FM AM demod 2 control register
	AAD_WrFL(fm_dqpsk_demod_ctrl);

	//In IN(India) model, Adjust Volume Level.
	if ((g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT)	\
	  &&(eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_4)	\
	  &&(lx_chip() != LX_CHIP_H13) )
	{
		//Read manual_output_level_left_ch reg.
		AAD_RdFL(manual_out_lvl_left_ch);

		//Read manual_output_level_right_ch reg.
		AAD_RdFL(manual_out_lvl_right_ch);

		//Read I2S Gain Control reg.
		AAD_RdFL(i2s_ctrl);

		// HDEV option : On, 4.9dB Boost
		//Set  a 0dB level for a left, right  volume
		AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x8D);
		AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x8D);

		//Set  a 1-bit shift for I2S volume for a left, right  volume
		AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift

		//Write manual_output_level_left_ch reg.
		AAD_WrFL(manual_out_lvl_left_ch);

		//Write manual_output_level_right_ch reg.
		AAD_WrFL(manual_out_lvl_right_ch);

		//Write I2S Gain Control reg.
		AAD_WrFL(i2s_ctrl);
	}

	//Unmutes by MuteCt bit in Mute Control Register.
	AAD_RdFL(mute_ctrl);
	AAD_Wr01(mute_ctrl, mutect, 0b00);
	AAD_Wr01(mute_ctrl, muteov, 0b0);
	AAD_WrFL(mute_ctrl);

	//Read registers
	AAD_RdFL(status);
	AAD_RdFL(fm_am_demod_ctrl);
	AAD_RdFL(fm_dqpsk_demod_ctrl);

	//Restore a AAD I2S Gain Control for High Deviation Off Mode.
	if( eHighDevMode.bHighDevOnOff == FALSE )
	{
		//Read I2S Gain Control reg.
		AAD_RdFL(i2s_ctrl);

		//Set a 0-bit shitf for I2S volume for a left, right volume
		AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x0); // 0-bit shift

		//Set a NICAM mode to set a volume for High Devication mode.
		switch (g_i_eAudioModeSet)
		{
			case LX_ATV_SET_PAL_NICAM_MONO:
			case LX_ATV_SET_PAL_NICAM_STEREO:
			case LX_ATV_SET_PAL_NICAM_DUALI:
			case LX_ATV_SET_PAL_NICAM_DUALII:
			case LX_ATV_SET_PAL_NICAM_DUALI_II:		//app. calls this mode in UI DUAL I+II osd.
				bNicamMode = TRUE;
				break;

			default:
				bNicamMode = FALSE;
				break;
		}

		//Check a chip revision
		if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			//SECAM L SIF Level Adjust : H13, soohong.jang(2013.12.18)
			if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L && bNicamMode == FALSE)
			{
				//Set  a 1-bit shift for I2S volume for a left, right  volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
			}
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			;
		}
		else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			//SECAM L SIF Level Adjust : M14, ileun.kwak(2013.12.09)
			if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L && bNicamMode == FALSE)
			{
				/* Check a SIF Internal Source. */
				if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
				{
					//Set  a 1-bit shift for I2S volume for a left, right  volume
					AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
				}
				else
				{
					//Set  a 1-bit shift for I2S volume for a left, right  volume
					AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
				}
			}
		}
		else 	//if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
		{
			//SECAM L SIF Level Adjust : H13, soohong.jang(2013.12.18)
			if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L && bNicamMode == FALSE)
			{
				//Set  a 1-bit shift for I2S volume for a left, right  volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
			}
		}

		//Write I2S Gain Control reg.
		AAD_WrFL(i2s_ctrl);
	}

	//Sanity check for debug print
	if( eHighDevMode.highDevLevel >= LX_AAD_HIGH_DEV_LEVEL_4 )
		eHighDevMode.highDevLevel = LX_AAD_HIGH_DEV_LEVEL_4;

	AAD_DEBUG_TMP ("High Deviation %s(status = 0x%X)\n", debugString[eHighDevMode.highDevLevel], AAD_Rd(status));
	AAD_DEBUG_TMP ("demod1 = 0x%X, demod2 = 0x%X\n", AAD_Rd(fm_am_demod_ctrl), AAD_Rd(fm_dqpsk_demod_ctrl));
	AAD_DEBUG_TMP ("OnOff  = 0x%X, Level = 0x%X\n", eHighDevMode.bHighDevOnOff, eHighDevMode.highDevLevel);

	/* Copy High Deviation On or Off Status and Level */
	g_eHighDevMode.bHighDevOnOff = eHighDevMode.bHighDevOnOff;
	g_eHighDevMode.highDevLevel  = eHighDevMode.highDevLevel;

	return RET_OK;
}


/**
 * Sets a SIF Band(B/G, I, DK, L, M).
 * This function sets a SIF sound system.
 * @see
*/
int AAD_SIF_SetBandSetup ( LX_AAD_SIF_SOUNDSYSTEM_T eSoundSystem )
{
	UINT8	ui8SysSel;
	int		eRetVal = RET_OK;

	LX_AAD_HIGH_DEV_SET_T eHighDevMode;

	UINT8 	*debugString[6] = {"SYSTEM_BG", "SYSTEM_I", "SYSTEM_DK", "SYSTEM_L", "SYSTEM_MN",	\
							   "SYSTEM_UNKNOWN" };

	OS_LockMutex(&g_AAD_Sema);

	//Clear a Audio Mode.
	g_MonitorAudioMode = LX_ATV_GET_NTSC_A2_UNKNOWN;

	OS_UnlockMutex(&g_AAD_Sema);

	//do Mono/Stereo detect  function if DTV is ATSC A2 mode
	if ( g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT )
	{
		//Check a current audio sound system.
		if(g_i_eSystem == eSoundSystem)
		{
			OS_LockMutex(&g_AAD_Sema);

			//Set a default audio mode state.
			g_stCurMonoStState = LX_AAD_A2_DEFAULT0_STATE;

			OS_UnlockMutex(&g_AAD_Sema);

			//Read registers
			AAD_RdFL(system_select);
			AAD_RdFL(status);

			//Sanity check for debug print
			if( eSoundSystem > LX_AAD_SIF_SYSTEM_MN )
				eSoundSystem = LX_AAD_SIF_SYSTEM_MN + 1;

			AAD_DEBUG_TMP ("AAD_SIF_SetBandSetup : Sound System %d(%s)(syssel = %d, status = 0x%2X)\n",	\
							eSoundSystem, debugString[eSoundSystem], AAD_Rd(system_select), AAD_Rd(status));
			return eRetVal;
		}

		/* Start a AAD Monitor Task for SIF KOREA A2 Mode */
		if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
		{
			(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
		}

		//Set a Carrier 2 Quality AAOS Threshold High / Low Register
		//Org. threshold high is 0x34 and low is 0x21. Value is set according to live signal quality from CH7, 11 in KOREA M/N A2 signal.
		//Default value is changed to 0x21.
		AAD_RdFL(car2_qual_aaos_thd_hi);
		AAD_Wr01(car2_qual_aaos_thd_hi, mn2thh, (CAR2_QUAL_THRES_HI_A2 >> 3));
		AAD_WrFL(car2_qual_aaos_thd_hi);

#ifdef	ENABLE_AAD_TEST_MODE	//test only
#else
		//Set a ID Magnitude Threshold High / Low Register
		//Org. threshold high is 0x64 and low is 0x14.
		//Value is set according to live signal quality from CH7, 11 in KOREA M/N A2 signal.
		AAD_RdFL(id_mag_thd_hi);
		AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
		AAD_WrFL(id_mag_thd_hi);

		AAD_RdFL(id_mag_thd_lo);
		AAD_Wr01(id_mag_thd_lo, idthl, g_stThresholdLevel.stereoIDMagLow);
		AAD_WrFL(id_mag_thd_lo);
#endif	//#ifndef	ENABLE_AAD_TEST_MODE	//test only

		//Disable AAOS(Automatic Audio Output Select).
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten,  0);
		AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
		AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
		AAD_WrFL(audio_out_ctrl);
	}
	else if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
	{
		//Set a BTSC Pre Expander Gain for M14/H14(H15, H14, M14 BTSC Stereo Separation : improve +4~5db)
		/* Check LX Chip Revision Number */
		if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			AAD_RdFL(btsc_pre_expander_gain);
			AAD_Wr01(btsc_pre_expander_gain, btsc_pre_exp_gain, 0x10);	//0x298 : 0x10
			AAD_WrFL(btsc_pre_expander_gain);
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			AAD_RdFL(btsc_pre_expander_gain);
			AAD_Wr01(btsc_pre_expander_gain, btsc_pre_exp_gain, 0x10);	//0x298 : 0x10
			AAD_WrFL(btsc_pre_expander_gain);
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			AAD_RdFL(btsc_pre_expander_gain);
			AAD_Wr01(btsc_pre_expander_gain, btsc_pre_exp_gain, 0x10);	//0x298 : 0x10
			AAD_WrFL(btsc_pre_expander_gain);
		}
		else 	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			AAD_DEBUG ("LX_CHIP_REV(H13, XX)(0x%X)\n", lx_chip_rev());
		}

		//Set a ID Magnitude Threshold High / Low Register
		//Org. threshold high is 0x64 and low is 0x14.
		AAD_RdFL(id_mag_thd_hi);
		AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
		AAD_WrFL(id_mag_thd_hi);

		AAD_RdFL(id_mag_thd_lo);
		AAD_Wr01(id_mag_thd_lo, idthl, g_stThresholdLevel.stereoIDMagLow);
		AAD_WrFL(id_mag_thd_lo);

		//Set a FM Subcarrier Magnitude AAOS Threshold High Register
		//Org. threshold high is 0x4B Value
		AAD_RdFL(fm_subcar_mag_aaos_thd_hi);
		AAD_Wr01(fm_subcar_mag_aaos_thd_hi, fmsmth, FM_SUBCARRIER_MAG_THD_HI_BTSC);
		AAD_WrFL(fm_subcar_mag_aaos_thd_hi);

		//Set a FM Subcarrier Noise AAOS Threshold High / Low Register
		//Org. threshold high is 0x14 and low is 0x0A Value
		AAD_RdFL(fm_subcar_noise_aaos_thd_hi);
		AAD_Wr01(fm_subcar_noise_aaos_thd_hi, fmsnth, FM_SUBCARRIER_NOISE_THD_HI_BTSC);
		AAD_WrFL(fm_subcar_noise_aaos_thd_hi);

		AAD_RdFL(fm_subcar_noise_aaos_thd_lo);
		AAD_Wr01(fm_subcar_noise_aaos_thd_lo, fmsntl, FM_SUBCARRIER_NOISE_THD_LO_BTSC);
		AAD_WrFL(fm_subcar_noise_aaos_thd_lo);

		//Set a Pilot Mag AAOS Threshold High / Low Register
		//Org. threshold high is 0xB0 and low is 0x64 Value
		AAD_RdFL(pilot_mag_aaos_thd_hi);
		AAD_Wr01(pilot_mag_aaos_thd_hi, pilmth, PILOT_MAG_AAOS_THD_HI_BTSC);
		AAD_WrFL(pilot_mag_aaos_thd_hi);

		AAD_RdFL(pilot_mag_aaos_thd_lo);
		AAD_Wr01(pilot_mag_aaos_thd_lo, pilmtl, PILOT_MAG_AAOS_THD_LO_BTSC);
		AAD_WrFL(pilot_mag_aaos_thd_lo);

		//Disable AAOS(Automatic Audio Output Select).
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten,  0);
		AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
		AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
		AAD_WrFL(audio_out_ctrl);
	}
	//do Mono/Stereo detect  function if DTV is ATSC A2 mode for DVB AJJA Model.
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT )
	{
		if ( eSoundSystem == LX_AAD_SIF_SYSTEM_MN )
		{
			//Check a current audio sound system.
			if(g_i_eSystem == eSoundSystem)
			{
				OS_LockMutex(&g_AAD_Sema);

				//Set a default audio mode state.
				g_stCurMonoStState = LX_AAD_A2_DEFAULT0_STATE;

				OS_UnlockMutex(&g_AAD_Sema);

				//Read registers
				AAD_RdFL(system_select);
				AAD_RdFL(status);

				AAD_DEBUG_TMP ("AAD_SIF_SetBandSetup : Sound System %d(%s)(syssel = %d, status = 0x%2X)\n",	\
								eSoundSystem, debugString[eSoundSystem], AAD_Rd(system_select), AAD_Rd(status));
				return eRetVal;
			}

			/* Start a AAD Monitor Task for SIF KOREA A2 Mode */
			if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
			{
				(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
			}

			//Set a Carrier 2 Quality AAOS Threshold High / Low Register
			//Org. threshold high is 0x34 and low is 0x21. Value is set according to live signal quality from CH7, 11 in KOREA M/N A2 signal.
			//Default value is changed to 0x21.
			AAD_RdFL(car2_qual_aaos_thd_hi);
			AAD_Wr01(car2_qual_aaos_thd_hi, mn2thh, (CAR2_QUAL_THRES_HI_A2 >> 3));
			AAD_WrFL(car2_qual_aaos_thd_hi);

#ifdef	ENABLE_AAD_TEST_MODE	//test only
#else
			//Set a ID Magnitude Threshold High / Low Register
			//Org. threshold high is 0x64 and low is 0x14.
			//Value is set according to live signal quality from CH7, 11 in KOREA M/N A2 signal.
			AAD_RdFL(id_mag_thd_hi);
			AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
			AAD_WrFL(id_mag_thd_hi);

			AAD_RdFL(id_mag_thd_lo);
			AAD_Wr01(id_mag_thd_lo, idthl, g_stThresholdLevel.stereoIDMagLow);
			AAD_WrFL(id_mag_thd_lo);
#endif	//#ifndef	ENABLE_AAD_TEST_MODE	//test only

			//Disable AAOS(Automatic Audio Output Select).
			AAD_RdFL(audio_out_ctrl);
			AAD_Wr01(audio_out_ctrl, aouten,  0);
			AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
			AAD_WrFL(audio_out_ctrl);
		}
		else
		{
			/* Stop a AAD Monitor Task for SIF KOREA A2 Mode */
			if(g_suiAadWaitQForMonitor == LX_AAD_TASK_START)
			{
				(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_STOP);
			}

			//Carrier 1 Quality AAOS Threshold High / Low Register
			//Org. threshold high is 0x22 and low is 0x16 Value
			AAD_RdFL(car1_qual_aaos_thd_hi);
			AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_DVB);
			AAD_WrFL(car1_qual_aaos_thd_hi);

			AAD_RdFL(car1_qual_aaos_thd_lo);
			AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_DVB);
			AAD_WrFL(car1_qual_aaos_thd_lo);

			//NICAM Error Rate AAOS Threshold High / Low Register
			//Org. threshold high is 0x50 and low is 0x30 Value
			AAD_RdFL(nicam_err_aaos_thd_hi);
			AAD_Wr01(nicam_err_aaos_thd_hi, nierth, g_stThresholdLevel.nicamErrorRateHigh);
			AAD_WrFL(nicam_err_aaos_thd_hi);

			AAD_RdFL(nicam_err_aaos_thd_lo);
			AAD_Wr01(nicam_err_aaos_thd_lo, niertl, g_stThresholdLevel.nicamErrorRateLow);
			AAD_WrFL(nicam_err_aaos_thd_lo);

			//Enable AAOS(Automatic Audio Output Select)
			AAD_RdFL(audio_out_ctrl);
			AAD_Wr01(audio_out_ctrl, aouten, 1);
			AAD_WrFL(audio_out_ctrl);
		}
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT )
	{
		//Carrier 1 Quality AAOS Threshold High / Low Register
		//Org. threshold high is 0x22 and low is 0x16 Value
		AAD_RdFL(car1_qual_aaos_thd_hi);
		AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_DVB);
		AAD_WrFL(car1_qual_aaos_thd_hi);

		AAD_RdFL(car1_qual_aaos_thd_lo);
		AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_DVB);
		AAD_WrFL(car1_qual_aaos_thd_lo);

		//NICAM Error Rate AAOS Threshold High / Low Register
		//Org. threshold high is 0x50 and low is 0x30 Value
		AAD_RdFL(nicam_err_aaos_thd_hi);
		AAD_Wr01(nicam_err_aaos_thd_hi, nierth, g_stThresholdLevel.nicamErrorRateHigh);
		AAD_WrFL(nicam_err_aaos_thd_hi);

		AAD_RdFL(nicam_err_aaos_thd_lo);
		AAD_Wr01(nicam_err_aaos_thd_lo, niertl, g_stThresholdLevel.nicamErrorRateLow);
		AAD_WrFL(nicam_err_aaos_thd_lo);

		//Enable AAOS(Automatic Audio Output Select)
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten, 1);
		AAD_WrFL(audio_out_ctrl);
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT )
	{
		//Carrier 1 Quality AAOS Threshold High / Low Register
		//Org. threshold high is 0x22 and low is 0x16 Value
		// 2011.11.12 by suyeon.jung
		AAD_RdFL(car1_qual_aaos_thd_hi);
		AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_DVB);
		AAD_WrFL(car1_qual_aaos_thd_hi);

		//Enable AAOS(Automatic Audio Output Select)
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten, 1);
		AAD_WrFL(audio_out_ctrl);
	}
	//In IN(India) model, set a ID Magnitude Threshold High Register to near max value
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
	{
		/* Check LX Chip Revision Number */
		if (lx_chip() != LX_CHIP_H13 && g_bHighDevOnOff == TRUE)
		{
			OS_LockMutex(&g_AAD_Sema);

			//Set a High Dev On : 540Khz
			eHighDevMode.bHighDevOnOff = TRUE;
			eHighDevMode.highDevLevel  = LX_AAD_HIGH_DEV_LEVEL_4;

			// Set a High Deviation Mode.
			(void)AAD_SIF_SetHighDeviationMode(eHighDevMode);

			//Threshold PAL B High Deviation Off Count for India.
			g_ui8PALBHighDevThreshold = HIGH_DEV_OFF_COUNT_IN;

			//Clears a PAL B Task Count.
			g_ui8PALBTaskCount = 0;

			//Clears a PAL B High Deviation Status.
			g_ui8HighDevEabled = HIGH_DEV_UNKNOWN_STATUS;

			//Clears a PAL B High Deviation Check Count.
			g_ui8PALBContiCount = 0;

			//Clears a PAL B High Deviation On Check Count.
			g_ui8PALBContiCountOn = 0;

			OS_UnlockMutex(&g_AAD_Sema);

			/* Start a AAD Monitor Task for PAL B A2 India Mode */
			if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
			{
				(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
			}
		}

		//Set a Forced Mono in India.
		g_stThresholdLevel.stereoIDMagHigh = ID_MAG_THRES_INDIA_0xF8;

		AAD_RdFL(id_mag_thd_hi);
		AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
		AAD_WrFL(id_mag_thd_hi);

		//Disable AAOS(Automatic Audio Output Select) for Forced Mono in India.
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten,  0);
		AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
		AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
		AAD_WrFL(audio_out_ctrl);
	}
	//In ID(Indonesia) model, set a ID xxxx
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT )
	{
		//Carrier 1 Quality AAOS Threshold High / Low Register
		//Org. threshold high is 0x22 and low is 0x16 Value
		AAD_RdFL(car1_qual_aaos_thd_hi);
		AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_ID);
		AAD_WrFL(car1_qual_aaos_thd_hi);

		AAD_RdFL(car1_qual_aaos_thd_lo);
		AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_ID);
		AAD_WrFL(car1_qual_aaos_thd_lo);

		//NICAM Error Rate AAOS Threshold High / Low Register
		//Org. threshold high is 0x50 and low is 0x30 Value
		AAD_RdFL(nicam_err_aaos_thd_hi);
		AAD_Wr01(nicam_err_aaos_thd_hi, nierth, g_stThresholdLevel.nicamErrorRateHigh);
		AAD_WrFL(nicam_err_aaos_thd_hi);

		AAD_RdFL(nicam_err_aaos_thd_lo);
		AAD_Wr01(nicam_err_aaos_thd_lo, niertl, g_stThresholdLevel.nicamErrorRateLow);
		AAD_WrFL(nicam_err_aaos_thd_lo);

		//Enable AAOS(Automatic Audio Output Select)
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten, 1);
		AAD_WrFL(audio_out_ctrl);
	}

	/* Check LX Chip Revision Number */
	if (lx_chip() != LX_CHIP_H13)
	{
		//In IN(India) model, Set a default 540Khz High Deviation Filter
		if ( g_eAadInitType != LX_AAD_INIT_DVB_IN_SELECT )
		{
			//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
			AAD_SIF_SetHighDeviationMode(g_eHighDevMode);
		}
		else if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT && g_bHighDevOnOff == FALSE )
		{
			//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
			AAD_SIF_SetHighDeviationMode(g_eHighDevMode);
		}
	}
	else
	{
		//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
		AAD_SIF_SetHighDeviationMode(g_eHighDevMode);
	}

	//Check a current sound sysytem is detected.
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, syssel, ui8SysSel);
	if ( ui8SysSel == 0xF )	//sound system is unknown
	{
		switch ( eSoundSystem )
		{
			case LX_AAD_SIF_SYSTEM_BG:
				AAD_Wr01(system_select, syssel, 0x1); //FM-Mono/NICAM

				//In IN(India) model, Disable Stereo Carrier Search Control.
				if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
				{
					//Stop Stereo Carrier Search Control
					AAD_RdFL(stereo_car_search_ctrl);
					AAD_Wr01(stereo_car_search_ctrl, srchen, 0x0);
					AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x0);
					AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b000);
					AAD_WrFL(stereo_car_search_ctrl);
				}
				else
				{
					//Start Stereo Carrier Search Control(Skip A2DK1, A2DK2 and A2DK3 in BG system)
					//Syssel change automatically when a stereo carrier is detected
					AAD_RdFL(stereo_car_search_ctrl);
					AAD_Wr01(stereo_car_search_ctrl, srchen, 0x2);
					AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x2);
					AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b111);
					AAD_WrFL(stereo_car_search_ctrl);
				}

				//For workaround, NICAM high frequency area deviation is larger than low area.
				//Set a Carrier Average Frequency Threshold(ASD, SCS)
				//Org. threshold high is 0x18.
				AAD_RdFL(car_aver_freq_thd);
				AAD_Wr01(car_aver_freq_thd, asfth, CARRIER_AVG_FREQ_THD_DVB_BG);
				AAD_WrFL(car_aver_freq_thd);
				break;

			case LX_AAD_SIF_SYSTEM_I:
				AAD_Wr01(system_select, syssel, 0x3); //FM-Mono/NICAM
				break;

			case LX_AAD_SIF_SYSTEM_DK:
				AAD_Wr01(system_select, syssel, 0x7); //FM-Mono/NICAM

				//Start Stereo Carrier Search Control
				//Syssel change automatically when a stereo carrier is detected
				AAD_RdFL(stereo_car_search_ctrl);
				AAD_Wr01(stereo_car_search_ctrl, srchen, 0x2);
				AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x2);
				AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b000);
				AAD_WrFL(stereo_car_search_ctrl);
				break;

			case LX_AAD_SIF_SYSTEM_L:
				AAD_Wr01(system_select, syssel, 0x2); //AM-Mono/NICAM

				/* Check LX Chip Revision Number */
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					/* Start a AAD Monitor Task for SIF SECAM L Mode */
					if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
					{
						(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
					}

					//AGC Gain Control Register, SECAM L(AM) Volume Level Issue(2014.02.11)
					AAD_RdFL(manual_agc);
					AAD_Wr01(manual_agc, gain, 0x8);
					AAD_Wr01(manual_agc, manualmode, 0x1);
					AAD_WrFL(manual_agc);
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					/* Start a AAD Monitor Task for SIF SECAM L Mode */
					if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
					{
						(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
					}

					/* Check a SIF Internal Source. */
					if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
					{
						//AGC Gain Control Register, SECAM L(AM) Volume Max Issue(2013.12.09)
						AAD_RdFL(manual_agc);
						AAD_Wr01(manual_agc, gain, 0x2);
						AAD_Wr01(manual_agc, manualmode, 0x1);
						AAD_WrFL(manual_agc);

						//Carrier 1 Phase Noise AAOS Threshold High / Low Register(2013.11.25)
						//Org. threshold high is 0x80 and low is 0x20 Value
						AAD_RdFL(car1_phase_noise_aaos_thd_hi);
						AAD_Wr01(car1_phase_noise_aaos_thd_hi, pn1thh, CAR1_PAHSE_NOISE_AAOS_THD_HI);
						AAD_WrFL(car1_phase_noise_aaos_thd_hi);

						AAD_RdFL(car1_phase_noise_aaos_thd_lo);
						AAD_Wr01(car1_phase_noise_aaos_thd_lo, pn1thl, CAR1_PAHSE_NOISE_AAOS_THD_LO);
						AAD_WrFL(car1_phase_noise_aaos_thd_lo);
					}
					else
					{
						//AGC Gain Control Register, SECAM L(AM) Volume Max Issue(2013.12.09)
						AAD_RdFL(manual_agc);
						AAD_Wr01(manual_agc, gain, 0x2);
						AAD_Wr01(manual_agc, manualmode, 0x1);
						AAD_WrFL(manual_agc);
					}
				}
				else 	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					/* Start a AAD Monitor Task for SIF SECAM L Mode */
					if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
					{
						(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
					}

					//AGC Gain Control Register, SECAM L(AM) Volume Level Issue(2014.02.11)
					AAD_RdFL(manual_agc);
					AAD_Wr01(manual_agc, gain, 0x8);
					AAD_Wr01(manual_agc, manualmode, 0x1);
					AAD_WrFL(manual_agc);
				}
				break;

			case LX_AAD_SIF_SYSTEM_MN:
				if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
				{
					AAD_Wr01(system_select, syssel, 0xA); //FM-Stereo + SAP (BTSC)
				}
				else
				{
					AAD_Wr01(system_select, syssel, 0x8); //FM-Stereo (A2)
				}
				break;

			case LX_AAD_SIF_SYSTEM_UNKNOWN:
				eRetVal = RET_INVALID_PARAMS;
				break;

			default:
				eRetVal = RET_INVALID_PARAMS;
				break;
		}

		//Set a current sound system.
		AAD_WrFL(system_select);
	}

	//Copy for global value
	g_i_eSystem = eSoundSystem;

	//Reset audio mode in channel is changed.
	g_o_eAudioMode = LX_ATV_GET_NTSC_BTSC_UNKNOWN;
	g_io_stStandardInfo.detectedStandard = LX_AAD_SIF_NOT_AVAILE;
	g_io_stStandardInfo.availability 	 = LX_AAD_SIF_DETECTING_EXSISTANCE;

	//Read registers
	AAD_RdFL(system_select);
	AAD_RdFL(status);

	//Sanity check for debug print
	if( eSoundSystem > LX_AAD_SIF_SYSTEM_MN )
		eSoundSystem = LX_AAD_SIF_SYSTEM_MN + 1;

	AAD_DEBUG_TMP ("AAD_SIF_SetBandSetup : Sound System %d(%s)(syssel = %d, status = 0x%2X)\n",	\
					eSoundSystem, debugString[eSoundSystem], AAD_Rd(system_select), AAD_Rd(status));

	return eRetVal;
}

/**
 * Sets a SIF sound standard input mode.
 * This function sets a SIF sound standard.
 * @see
*/
int AAD_SIF_SetModeSetup ( LX_AAD_SIF_STANDARD_T eStandard )
{
	int		eRetVal = RET_OK;

	UINT8	ui8SysSel;
	BOOLEAN	bNeedToSet = FALSE;

	LX_AAD_SIF_SOUNDSYSTEM_T 	eTVSoundSystem = LX_AAD_SIF_SYSTEM_UNKNOWN;

	UINT8 	*debugString[17] = {"MODE_DETECT", 	"BG_NICAM", 	"BG_FM", 	"BG_A2", 	"I_NICAM",	\
							  	"I_FM", 		"DK_NICAM", 	"DK_FM", 	"DK1_A2", 	"DK2_A2",	\
							  	"DK3_A2", 		"L_NICAM", 		"L_AM", 	"MN_A2", 	"MN_BTSC",	\
							  	"MN_EIAJ", 		"NUM_SOUND_STD" };

	//Check a current sound sysytem is detected.
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, syssel, ui8SysSel);

	//Set Mode using system select register based on the input parameter
	switch (eStandard)
	{
		case LX_AAD_SIF_MODE_DETECT:
			//do ASD same as above and does not blocking call
			//doing without blocking call is not possible
			eRetVal = RET_INVALID_PARAMS;
			break;

		case LX_AAD_SIF_BG_NICAM:
		case LX_AAD_SIF_BG_FM:
		case LX_AAD_SIF_BG_A2:
			//Check a detected sound system
			eTVSoundSystem = AAD_GetSoundSystem();
			if (eTVSoundSystem != LX_AAD_SIF_SYSTEM_BG )
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x0);
			}
			break;

		case LX_AAD_SIF_I_NICAM:
		case LX_AAD_SIF_I_FM:
			//Check a detected sound system
			eTVSoundSystem = AAD_GetSoundSystem();
			if (eTVSoundSystem != LX_AAD_SIF_SYSTEM_I )
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x3);
			}
			break;

		case LX_AAD_SIF_DK_NICAM:
		case LX_AAD_SIF_DK_FM:
		case LX_AAD_SIF_DK1_A2:
		case LX_AAD_SIF_DK2_A2:
		case LX_AAD_SIF_DK3_A2:
			//Check a detected sound system
			eTVSoundSystem = AAD_GetSoundSystem();
			if (eTVSoundSystem != LX_AAD_SIF_SYSTEM_DK )
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x4);
			}
			break;

		case LX_AAD_SIF_L_NICAM:
		case LX_AAD_SIF_L_AM:
			//Check a detected sound system
			eTVSoundSystem = AAD_GetSoundSystem();
			if (eTVSoundSystem != LX_AAD_SIF_SYSTEM_L )
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x2);
			}
			break;

		case LX_AAD_SIF_MN_A2:
			//Check a detected sound system
			if (ui8SysSel != 8)
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x8);

				//Disable AAOS(Automatic Audio Output Select) for Forced Mono in A2 system.
				AAD_RdFL(audio_out_ctrl);
				AAD_Wr01(audio_out_ctrl, aouten,  0);
				AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
				AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
				AAD_WrFL(audio_out_ctrl);
			}
			break;

		case LX_AAD_SIF_MN_BTSC:
			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
			{
				//Check a detected sound system
				if (ui8SysSel != 0xA)
				{
					bNeedToSet = TRUE;
					AAD_Wr01(system_select, syssel, 0xA);
				}
			}
			else
			{
				//wait 1000 ms for debug print
				AAD_SIF_TIME_DELAY_MS (ERROR_PRINT_DELAY);

				AAD_DEBUG("Error : BTSC Not Enabled!\n");
			}
			break;

		case LX_AAD_SIF_MN_EIAJ:
			//Check a detected sound system
			if (ui8SysSel != 0x9)
			{
				bNeedToSet = TRUE;
				AAD_Wr01(system_select, syssel, 0x9);
			}
			break;

		default:
			eRetVal = RET_INVALID_PARAMS;
			break;
	}

	//Sanity check for debug print
	if( eStandard > LX_AAD_SIF_NUM_SOUND_STD )
		eStandard = LX_AAD_SIF_NUM_SOUND_STD;

	//Check bNeedToSet flag
	if (bNeedToSet == TRUE)
	{
		//Set a current sound system and standard.
		AAD_WrFL(system_select);

		AAD_DEBUG ("AAD_SIF_SetModeSetup : Sound Standard %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
					eStandard, debugString[eStandard], AAD_Rd(system_select), AAD_Rd(status));
	}

	//Copy for global value
	g_i_eStandard = eStandard;

	//Read registers
	AAD_RdFL(system_select);
	AAD_RdFL(status);

	//For debugging print
	if( g_i_eStandard != eStandard )
	{
		AAD_DEBUG_TMP ("AAD_SIF_SetModeSetup : Sound Standard %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
						eStandard, debugString[eStandard], AAD_Rd(system_select), AAD_Rd(status) );
	}

	//For debugging print
	if((g_ui32AadPrintCount % DEBUG_MSG_PRINT_TIME) == 0)	//DDI calls every 30 ms
	{
		AAD_DEBUG_TMP ("AAD_SIF_SetModeSetup : Sound Standard %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
						eStandard, debugString[eStandard], AAD_Rd(system_select), AAD_Rd(status));
	}

	AAD_PRINT ("AAD_SIF_SetModeSetup : Sound Standard %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
				eStandard, debugString[eStandard], AAD_Rd(system_select), AAD_Rd(status));

	return eRetVal;
}


/**
 * Sets a user analog TV stereo mode.
 * Analog Audio Output Mode is setted to Analog Audio Decoder by User Mode or Analog Audio Task.
 * @see
*/
int AAD_SIF_SetUserAnalogMode ( LX_ATV_AUDIO_MODE_SET_T eAudioModeSet )
{
	int		eRetVal = RET_OK;

	BOOLEAN	bNeedToDebug = FALSE;

	UINT8 *debugStrMoSt[25] = {"PAL_MONO", 	  "PAL_MONO_F",    "PAL_ST", 		"PAL_ST_F", 		"PAL_DUAL_I",	\
							   "PAL_DUAL_II", "PAL_DUAL_I_II", "NICAM_MONO", 	"NICAM_MONO_F", 	"NICAM_ST", \
							   "NICAM_ST_F",  "NICAM_DUAL_I",  "NICAM_DUAL_II", "NICAM_DUAL_I_II", 	"NICAM_DUAL_F", \
							   "PAL_UNKNOWN", "A2_MONO", 	   "A2_STEREO", 	"A2_SAP",			"A2_UNKNOWN", \
							   "BTSC_MONO",   "BTSC_ST", 	   "BTSC_SAP_MONO", "BTSC_SAP_ST", 		"BTSC_UNKNOWN" };

	UINT8 *debugStrInit[10] = {"TYPE_NONE",   "ATSC_A2", 		"ATSC_BTSC",   	"DVB",		"DVB_ID",	\
							   "DVB_IN(DIA)", "DVB_CN", 		"BTSC_US",   	"DVB_AJJA",	"TYPE_MAX" };

	UINT8 *debugStrSys[6] =   {"SYSTEM_BG", "SYSTEM_I", "SYSTEM_DK", "SYSTEM_L", "SYSTEM_MN",	\
							   "SYSTEM_UNKNOWN" };

	//Set a NICAM mode to set a volume for High Devication mode.
	(void)AAD_SetManualVolumeForHDEVMode(eAudioModeSet);

	//Read audio output control  reg.
	AAD_RdFL(audio_out_ctrl);

	if ( g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT )
	{
		//Disable AAOS(Automatic Audio Output Select)
		AAD_Wr01(audio_out_ctrl, aouten, 0);
	}
	else if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
	{
		//Disable AAOS(Automatic Audio Output Select)
		AAD_Wr01(audio_out_ctrl, aouten, 0);

		//Read Standard Select reg.
		AAD_RdFL(standard_select);
	}
	else if ( (g_eAadInitType == LX_AAD_INIT_DVB_SELECT) || (g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT)	\
			||(g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT) )
	{
		//Enable AAOS(Automatic Audio Output Select) if audio system is not PAL D/K system.
		if(g_i_eSystem != LX_AAD_SIF_SYSTEM_DK)
		{
			AAD_Wr01(audio_out_ctrl, aouten, 1);
		}
		else
		{
			//Disable AAOS(Automatic Audio Output Select) if audio mode is Mono in PAL D/K system.
			if(eAudioModeSet == LX_ATV_SET_PAL_MONO)
			{
				/* Start a AAD Monitor Task for SIF D/K Mode */
				if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START && g_i_eSystem == LX_AAD_SIF_SYSTEM_DK)
				{
					//Clears a 	g_ui32AadTaskRunningCount to disable AAOS after aad task runs in 1 second.
					g_ui32AadTaskRunningCount = 0;

					(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
				}
			}
			else
			{
				/* Stop a AAD Monitor Task for SIF D/K Mode */
				if(g_suiAadWaitQForMonitor != LX_AAD_TASK_STOP && g_i_eSystem == LX_AAD_SIF_SYSTEM_DK)
				{
					//Clears a 	g_ui32AadTaskRunningCount to disable AAOS after aad task runs in 1 seconds.
					g_ui32AadTaskRunningCount = 0;

					(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_STOP);
				}

				//Enable AAOS(Automatic Audio Output Select)
				AAD_Wr01(audio_out_ctrl, aouten, 1);
			}
		}
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
	{
		//Disable AAOS(Automatic Audio Output Select)
		AAD_Wr01(audio_out_ctrl, aouten, 0);
	}

	switch (eAudioModeSet)
	{
		case LX_ATV_SET_PAL_MONO:
		case LX_ATV_SET_PAL_UNKNOWN:
		case LX_ATV_SET_PAL_NICAM_MONO:
		case LX_ATV_SET_NTSC_A2_MONO:
		case LX_ATV_SET_NTSC_A2_UNKNOWN:
			AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
			break;

		case LX_ATV_SET_NTSC_BTSC_MONO:
		case LX_ATV_SET_NTSC_BTSC_UNKNOWN:
			AAD_Wr01(standard_select, stndrd, 0x0); 	//BTSC (Mono or Stereo)

			AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
			break;

		case LX_ATV_SET_PAL_STEREO:
		case LX_ATV_SET_NTSC_A2_STEREO:
		case LX_ATV_SET_PAL_NICAM_STEREO:
			AAD_Wr01(audio_out_ctrl, ltsel, 0x1);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x1);
			break;

		case LX_ATV_SET_NTSC_BTSC_STEREO:
			AAD_Wr01(standard_select, stndrd, 0x0); 	//BTSC (Mono or Stereo)

			AAD_Wr01(audio_out_ctrl, ltsel,   0x1);
			AAD_Wr01(audio_out_ctrl, rtsel,   0x1);
			break;

		case LX_ATV_SET_PAL_DUALI:
		case LX_ATV_SET_PAL_NICAM_DUALI:
			AAD_Wr01(audio_out_ctrl, ltsel, 0x2);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x2);
			break;

		case LX_ATV_SET_NTSC_BTSC_SAP_MONO:
		case LX_ATV_SET_NTSC_BTSC_SAP_STEREO:	//Not Used in DDI function
			AAD_Wr01(standard_select, stndrd, 0x1); 	// BTSC(Mono or SAP)

			AAD_Wr01(audio_out_ctrl, ltsel,   0x3);	//SAP
			AAD_Wr01(audio_out_ctrl, rtsel,   0x3);	//SAP
			break;

		case LX_ATV_SET_PAL_DUALII:
		case LX_ATV_SET_NTSC_A2_SAP:
		case LX_ATV_SET_PAL_NICAM_DUALII:
			AAD_Wr01(audio_out_ctrl, ltsel, 0x3);	//SAP
			AAD_Wr01(audio_out_ctrl, rtsel, 0x3);	//SAP
			break;

		case LX_ATV_SET_PAL_DUALI_II:
		case LX_ATV_SET_PAL_NICAM_DUALI_II:		//app. calls this mode in UI DUAL I+II osd.
			AAD_Wr01(audio_out_ctrl, ltsel, 0x2);	//L : DUAL I
			AAD_Wr01(audio_out_ctrl, rtsel, 0x3);	//R : DUAL II
			break;

		case LX_ATV_SET_PAL_MONO_FORCED:
		case LX_ATV_SET_PAL_STEREO_FORCED:
		case LX_ATV_SET_PAL_NICAM_MONO_FORCED:
		case LX_ATV_SET_PAL_NICAM_STEREO_FORCED:
		case LX_ATV_SET_PAL_NICAM_DUAL_FORCED:		//app. calls this mode in UI Mono osd in DUAL mode
			//Disables AAOS temporily for this mode
			AAD_Wr01(audio_out_ctrl, aouten,  0);

			AAD_Wr01(audio_out_ctrl, ltsel, 0x0);
			AAD_Wr01(audio_out_ctrl, rtsel, 0x0);
			break;

		default:
			eAudioModeSet = LX_ATV_SET_NTSC_BTSC_UNKNOWN;
			eRetVal = RET_INVALID_PARAMS;
			break;
	}

	//Set a debugging print flag.
	if( g_i_eAudioModeSet != eAudioModeSet )
	{
		bNeedToDebug = TRUE;
	}

	//Copy for global value to use high deviation setting.
	g_i_eAudioModeSet = eAudioModeSet;

	//Set other value by model base
	if ( g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT )
	{
		//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
		AAD_SIF_SetHighDeviationMode(g_eHighDevMode);
	}
	else if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
	{
		//Write Standard Select reg.
		AAD_WrFL(standard_select);

		//Read registers
		AAD_RdFL(standard_select);
		AAD_DEBUG_TMP ("SetUserAnalogMode: standard_select = 0x%2X\n", AAD_Rd(standard_select));
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
	{
		OS_LockMutex(&g_AAD_Sema);

		//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
		AAD_SIF_SetHighDeviationMode(g_eHighDevMode);

		OS_UnlockMutex(&g_AAD_Sema);
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT )
	{
		//Set High Deviation Level : The HDEV reg. is auto cleared by AAD H/W reset, Overwrite and AAOS function.
		AAD_SIF_SetHighDeviationMode(g_eHighDevMode);
	}

	//In DVB model, set also Demod 1 filter for 100kHz lowpass filter  to reduce forced mono sweep noise in PAL B/G NICAM signal.(2014.04.12)
	if ( g_eAadInitType == LX_AAD_INIT_DVB_SELECT		\
	  || g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT	\
	  || g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT )
	{
		/* Check LX Chip Revision Number */
		if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			;
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			;
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			//Demod 1 Filter: 100kHz lowpass to reduce forced mono sweep noise in PAL B/G NICAM signal.(2014.04.12)
			if( (g_i_eSystem	== LX_AAD_SIF_SYSTEM_BG)		\
			  &&(g_i_eSifSource == LX_AAD_SIF_INPUT_EXTERNAL)	\
			  &&(eAudioModeSet	== LX_ATV_SET_PAL_NICAM_MONO_FORCED || eAudioModeSet == LX_ATV_SET_PAL_NICAM_STEREO_FORCED || eAudioModeSet == LX_ATV_SET_PAL_NICAM_DUAL_FORCED) )
			{
				//Get high deviation level using FM AM demod 1 control register
				AAD_RdFL(fm_am_demod_ctrl);

				//Set 100Khz lowpass filter if NICAM is present.
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl,	0x0);
				AAD_Wr01(fm_am_demod_ctrl, dmd1fl2, 0x0);

				//Set high deviation level using FM AM demod 1 control register
				AAD_WrFL(fm_am_demod_ctrl);
			}
		}
		else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			;
		}
	}

	//Write audio output control  reg.
	AAD_WrFL(audio_out_ctrl);

	//Sanity check for debug print
	if( eAudioModeSet >= LX_ATV_SET_NTSC_BTSC_UNKNOWN )
		eAudioModeSet = LX_ATV_SET_NTSC_BTSC_UNKNOWN;

	//Sanity check for debug print
	if( g_eAadInitType > LX_AAD_INIT_TYPE_MAX )
		g_eAadInitType = LX_AAD_INIT_TYPE_MAX;

	//Sanity check for debug print
	if( g_i_eSystem > LX_AAD_SIF_SYSTEM_MN )
		g_i_eSystem = LX_AAD_SIF_SYSTEM_MN + 1;

	//For debugging print
	if( bNeedToDebug == TRUE )
	{
		//Read registers
		AAD_RdFL(manual_out_lvl_left_ch);
		AAD_RdFL(audio_out_ctrl);
		AAD_RdFL(status);
		AAD_DEBUG ("SetUserAnalogMode %10s[output = 0x%2X](status = 0x%2X)\n", debugStrMoSt[eAudioModeSet], AAD_Rd(audio_out_ctrl), AAD_Rd(status));
		AAD_DEBUG ("SetUserAnalogMode : InitType = %10s, eSystem = %10s, level = 0x%X\n", debugStrInit[g_eAadInitType], debugStrSys[g_i_eSystem], AAD_Rd(manual_out_lvl_left_ch));
	}

	AAD_PRINT ("SetUserAnalogMode : InitType = %10s, eSystem = %10s\n", debugStrInit[g_eAadInitType], debugStrSys[g_i_eSystem]);

	return eRetVal;
}


/**
 * Sets a threshold level of SIF sound system.
 * This function sets the threshold level of stereo or mono sound detection in current SIF input.
 * @see
*/
int AAD_SIF_SetThresholdLevel ( LX_AAD_SIF_THRESHOLD_T level )
{
	//Check the threshold mode
	if(level.mode & LX_AAD_THRES_CAR2_MAGITUDE)
	{
		//Copy A2 Threshold Level
		g_stThresholdLevel.carrier2Magitude = level.carrier2Magitude;
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : carrier2Magitude = 0x%X\n", g_stThresholdLevel.carrier2Magitude);
	}

	//Check the threshold mode
	if(level.mode & LX_AAD_THRES_CAR2_QUALITY_HI)
	{
		//Copy A2 Threshold Level
		g_stThresholdLevel.carrier2QualityHigh = level.carrier2QualityHigh;
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : carrier2QualityHigh = 0x%X\n", g_stThresholdLevel.carrier2QualityHigh);

		//Set a Carrier 2 Quality AAOS Threshold High / Low Register
		AAD_RdFL(car2_qual_aaos_thd_hi);
		AAD_Wr01(car2_qual_aaos_thd_hi, mn2thh, (g_stThresholdLevel.carrier2QualityHigh >> 3));
		AAD_WrFL(car2_qual_aaos_thd_hi);

		//Read registers
		AAD_RdFL(car2_qual_aaos_thd_hi);
		AAD_RdFL(car2_qual_aaos_thd_lo);
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : CAR2 Quality High  = 0x%X, Low = 0x%X\n",	\
				   AAD_Rd(car2_qual_aaos_thd_hi), AAD_Rd(car2_qual_aaos_thd_lo));
	}

	//Check the threshold mode
	if(level.mode & LX_AAD_THRES_CAR2_QUALITY_LO)
	{
		//Copy A2 Threshold Level
		g_stThresholdLevel.carrier2QualityLow = level.carrier2QualityLow;
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : carrier2QualityLow = 0x%X\n", g_stThresholdLevel.carrier2QualityLow);

		//Set a Carrier 2 Quality AAOS Threshold High / Low Register
		AAD_RdFL(car2_qual_aaos_thd_lo);
		AAD_Wr01(car2_qual_aaos_thd_lo, mn2thl, (g_stThresholdLevel.carrier2QualityLow >> 3));
		AAD_WrFL(car2_qual_aaos_thd_lo);

		//Read registers
		AAD_RdFL(car2_qual_aaos_thd_hi);
		AAD_RdFL(car2_qual_aaos_thd_lo);
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : CAR2 Quality High  = 0x%X, Low = 0x%X\n",	\
				   AAD_Rd(car2_qual_aaos_thd_hi), AAD_Rd(car2_qual_aaos_thd_lo));
	}

	//Check the threshold mode
	if(level.mode & LX_AAD_THRES_ID_MAGNITUDE)
	{
		//Copy A2 Threshold Level by chip reviosn(This value is decided by experimental result in PT measurement.
		/////////////////////////////////////////////////////////////////////////////
		//H15 A0 :  0x168: 0x20, 0x16C: 0x1A
		//M14 B1 :  0x168: 0x12, 0x16C: 0x07(Internal)
		//M14 B1 :  0x168: 0x20, 0x16C: 0x1A(External)
		//H13 B0 :  0x168: 0x20, 0x16C: 0x1A
		//L9  B0 :  0x168: 0x0F, 0x16C: 0x0C
		/* Check LX Chip Revision Number */
		if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
		{
			g_stThresholdLevel.stereoIDMagHigh = level.stereoIDMagHigh + 21;
			g_stThresholdLevel.stereoIDMagLow  = level.stereoIDMagLow  + 15;
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			g_stThresholdLevel.stereoIDMagHigh = level.stereoIDMagHigh + 21;
			g_stThresholdLevel.stereoIDMagLow  = level.stereoIDMagLow  + 15;
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			/* Check a SIF Internal Source. */
			if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
			{
				g_stThresholdLevel.stereoIDMagHigh = level.stereoIDMagHigh + 7;

				/* Check Threshold Low Value for below 0. */
				if (level.stereoIDMagHigh >= 11)
				{
					g_stThresholdLevel.stereoIDMagLow = level.stereoIDMagHigh - 4;
				}
				else
				{
					g_stThresholdLevel.stereoIDMagLow = 0x07;
				}
			}
			else
			{
				g_stThresholdLevel.stereoIDMagHigh = level.stereoIDMagHigh + 21;
				g_stThresholdLevel.stereoIDMagLow  = level.stereoIDMagLow  + 15;
			}
		}
		else 	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			g_stThresholdLevel.stereoIDMagHigh = level.stereoIDMagHigh + 21;
			g_stThresholdLevel.stereoIDMagLow  = level.stereoIDMagLow  + 15;
		}
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : stereoIDMagHigh = 0x%X, stereoIDMagLow = 0x%X\n",	\
				   g_stThresholdLevel.stereoIDMagHigh, g_stThresholdLevel.stereoIDMagLow);

		//Set a ID Magnitude Threshold High / Low Register
		AAD_RdFL(id_mag_thd_hi);
		AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
		AAD_WrFL(id_mag_thd_hi);

		AAD_RdFL(id_mag_thd_lo);
		AAD_Wr01(id_mag_thd_lo, idthl, g_stThresholdLevel.stereoIDMagLow);
		AAD_WrFL(id_mag_thd_lo);

		//Read registers
		AAD_RdFL(id_mag_thd_hi);
		AAD_RdFL(id_mag_thd_lo);
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : ID High = 0x%X, Low = 0x%X\n",	\
				   AAD_Rd(id_mag_thd_hi), AAD_Rd(id_mag_thd_lo));
	}

	//Check the threshold mode
	if ( level.mode & LX_AAD_THRES_NICAM_ERROR )
	{
		g_stThresholdLevel.nicamErrorRateHigh = level.nicamErrorRateHigh;
		g_stThresholdLevel.nicamErrorRateLow  = level.nicamErrorRateLow;

		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : nicamErrorRateHigh = 0x%X, nicamErrorRateLow = 0x%X\n",	\
				   g_stThresholdLevel.nicamErrorRateHigh, g_stThresholdLevel.nicamErrorRateLow);

		//NICAM Error Rate AAOS Threshold High / Low Register
		//Org. threshold high is 0x50 and low is 0x30 Value
		AAD_RdFL(nicam_err_aaos_thd_hi);
		AAD_Wr01(nicam_err_aaos_thd_hi, nierth, g_stThresholdLevel.nicamErrorRateHigh);
		AAD_WrFL(nicam_err_aaos_thd_hi);

		AAD_RdFL(nicam_err_aaos_thd_lo);
		AAD_Wr01(nicam_err_aaos_thd_lo, niertl, g_stThresholdLevel.nicamErrorRateLow);
		AAD_WrFL(nicam_err_aaos_thd_lo);

		//Read registers
		AAD_RdFL(nicam_err_aaos_thd_hi);
		AAD_RdFL(nicam_err_aaos_thd_lo);
		AAD_DEBUG("AAD_SIF_SetBTSCA2Level : NICAM High = 0x%X, Low = 0x%X\n",	\
				   AAD_Rd(nicam_err_aaos_thd_hi), AAD_Rd(nicam_err_aaos_thd_lo));
	}

	return RET_OK;
}

/**
 * Sets a SIF Band(B/G, I, DK, L, M) and returns the strength of input signal.
 * Gets also a detected sound system and signal strength level.
 * @see
*/
int AAD_SIF_GetBandDetect ( LX_AAD_SIF_SYSTEM_INFO_T *pstSystemInfo )
{
	UINT8		value;
	int			iRetVal = RET_OK;

	UINT16		ui16Carrier1Quality   = 0;
	UINT16		ui16Carrier1Magnitude = 0;
	UINT16		ui16PrevCarrier1Quality = 0;
	UINT16		ui16DiffCarrier1Quality = 0;

	UINT8 	  	*debugString[6] = {"BG", "I", "DK", "L", "MN", "UNKNOWN" };
	UINT8		ui8SetStrNum = 0, ui8DebugStrNum = 0;

	BOOLEAN		bManualScanMode = FALSE;

	LX_AAD_SIF_SOUNDSYSTEM_T 	eTVSoundSystem = LX_AAD_SIF_SYSTEM_UNKNOWN;

	//Set a manual scan mode.
	bManualScanMode = pstSystemInfo->bManualMode;

	//////////////////////////////////////////////////////////////
	//Set a manual scan mode.
	bManualScanMode = TRUE;
	//////////////////////////////////////////////////////////////

	//Check a ASD or Manual Mode to detect audio sound system.
	if (bManualScanMode == FALSE)
	{
		//Do ASD using both carrier (6.5 and  4.5MHz carrier) and return detected TV system
		iRetVal = AAD_DoASD( pstSystemInfo->setSystem, TRUE );
		if ( RET_OK != iRetVal )
		{
			AAD_DEBUG ("ASD Failed!!! \n");
		}
	}
	else
	{
		//Check a SIF Signal to detect sound system using manual mode.
		iRetVal = AAD_DetectSoundSystem(pstSystemInfo->setSystem);
	}

	//Check a detected sound system
	eTVSoundSystem = AAD_GetSoundSystem();

	//In case of China signal, M/N ASD function is sometimes failed. In this case, MW detects M/N signal by video signal.
	if (eTVSoundSystem != pstSystemInfo->setSystem)
	{
		AAD_DEBUG ("ASD system : SET = 0x%X, Get = 0x%X !!! \n", pstSystemInfo->setSystem, eTVSoundSystem);
	}

	//In case of China signal, M/N ASD function is sometimes faild, so do not check error case.
	if (iRetVal == RET_OK)
	{
		//Check a ASD or Manual Mode to detect audio sound system.
		if (bManualScanMode == FALSE)
		{
			//Wait to read signal strength
			AAD_SIF_TIME_DELAY_MS (SIGNAL_QUALITY_DELAY);
		}

		AAD_RdFL(car1_fm_qual0);
		AAD_Rd01(car1_fm_qual0, cr1ql, value);
		ui16Carrier1Quality  = value << 8;

		AAD_RdFL(car1_fm_qual1);
		AAD_Rd01(car1_fm_qual1, cr1ql, value);
		ui16Carrier1Quality += value;
		AAD_DEBUG("Carrier1 Quality Current   0x%4X\n", ui16Carrier1Quality );

		AAD_RdFL(car1_aver_mag0);
		AAD_Rd01(car1_aver_mag0, cr1mg, value);
		ui16Carrier1Magnitude  = value << 8;

		AAD_RdFL(car1_aver_mag1);
		AAD_Rd01(car1_aver_mag1, cr1mg, value);
		ui16Carrier1Magnitude += value;
		AAD_DEBUG("Carrier1 Magnitude Current 0x%4X\n", ui16Carrier1Magnitude );

		//Clears a Carrier1 Quality Value if channel scan is started.
		if (pstSystemInfo->setSystem == LX_AAD_SIF_SYSTEM_BG)
			g_ui16Carrier1Quality_ID = 0;

		//Check a ui16Carrier1Quality value under 0x100 for weak signal.
		if (ui16Carrier1Quality < CAR1_QUAL_THD_WEAK_SIGNAL_ID && g_ui16Carrier1Quality_ID < (CAR1_QUAL_THD_WEAK_SIGNAL_ID + 1))
		{
			//Save a previous ui16Carrier1Quality
			ui16PrevCarrier1Quality = ui16Carrier1Quality;

			//Wait to read signal strength
			AAD_SIF_TIME_DELAY_MS (SIGNAL_QUALITY_DELAY);

			AAD_RdFL(car1_fm_qual0);
			AAD_Rd01(car1_fm_qual0, cr1ql, value);
			ui16Carrier1Quality  = value << 8;

			AAD_RdFL(car1_fm_qual1);
			AAD_Rd01(car1_fm_qual1, cr1ql, value);
			ui16Carrier1Quality += value;
			AAD_DEBUG("Carrier1 Quality Current   0x%4X\n", ui16Carrier1Quality );

			// Compute Difference Carrier1 Quality.
			if(ui16Carrier1Quality >= ui16PrevCarrier1Quality)
				ui16DiffCarrier1Quality = ui16Carrier1Quality - ui16PrevCarrier1Quality;
			else
				ui16DiffCarrier1Quality = ui16PrevCarrier1Quality - ui16Carrier1Quality;

			//Check a ui16Carrier1Quality value under 0x100 for weak signal.
			if (ui16Carrier1Quality < CAR1_QUAL_THD_WEAK_SIGNAL_ID && ui16DiffCarrier1Quality >= CAR1_QUAL_DIFF_WEAK_SIGNAL_ID)
			{
				//In ID(Indonesia) model, set carrier1 quality value to 0x100 for weak signal .
				if (pstSystemInfo->setSystem == LX_AAD_SIF_SYSTEM_BG && g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT)
					ui16Carrier1Quality = CAR1_QUAL_THD_WEAK_SIGNAL_ID + 1;
				else
					ui16Carrier1Quality = CAR1_QUAL_THD_WEAK_SIGNAL_ID;

				AAD_WARN("Forced Carrier1 Quality 0x%X, Diff 0x%X\n", ui16Carrier1Quality, ui16DiffCarrier1Quality);
			}
			else
			{
				// Check Max Carrier1 Quality.
				if(ui16Carrier1Quality < ui16PrevCarrier1Quality)
					ui16Carrier1Quality = ui16PrevCarrier1Quality;

				AAD_WARN("Carrier1 Quality 0x%X, Diff 0x%X\n", ui16Carrier1Quality, ui16DiffCarrier1Quality);
			}
		}
		else
		{
			//Check a ui16Carrier1Quality value upper 0x100 for strong signal.
			if (ui16Carrier1Quality >= CAR1_QUAL_THD_WEAK_SIGNAL_ID)
			{
				//Set debug string number
				if( eTVSoundSystem <= LX_AAD_SIF_SYSTEM_MN )
					ui8DebugStrNum = eTVSoundSystem;
				else
					ui8DebugStrNum = LX_AAD_SIF_SYSTEM_MN + 1;

				AAD_WARN("Carrier1 Quality 0x%4X => %s\n", ui16Carrier1Quality, debugString[ui8DebugStrNum]);
			}
			else
			{
				AAD_WARN("Carrier1 Quality 0x%X\n", ui16Carrier1Quality);
			}
		}

		//Check a ui16Carrier1Quality value if value is smaller than (CAR1_QUAL_THD_WEAK_SIGNAL_ID + 1).
		if (g_ui16Carrier1Quality_ID < (CAR1_QUAL_THD_WEAK_SIGNAL_ID + 1))
		{
			//Updates a Carrier1 Quality Value if channel scan is started.
			g_ui16Carrier1Quality_ID = ui16Carrier1Quality;
		}

		//Copy a detected system
		pstSystemInfo->detectedSystem = eTVSoundSystem;

		//Copy SNR value for Carrier 1 Quality and Magnitude
		pstSystemInfo->bandStrength  = ui16Carrier1Quality;
		pstSystemInfo->bandMagnitude = ui16Carrier1Magnitude;
	}
	else
	{
		//Check a detected sound system
		pstSystemInfo->detectedSystem = LX_AAD_SIF_SYSTEM_UNKNOWN;

		//Copy SNR value for Carrier 1 Quality and Magnitude to set default value.
		pstSystemInfo->bandStrength  = 0;
		pstSystemInfo->bandMagnitude = 0;
	}

	//Set set string number
	if( pstSystemInfo->setSystem <= LX_AAD_SIF_SYSTEM_MN )
		ui8SetStrNum = pstSystemInfo->setSystem;
	else
		ui8SetStrNum = LX_AAD_SIF_SYSTEM_MN + 1;

	//Set debug string number
	if( eTVSoundSystem <= LX_AAD_SIF_SYSTEM_MN )
		ui8DebugStrNum = eTVSoundSystem;
	else
		ui8DebugStrNum = LX_AAD_SIF_SYSTEM_MN + 1;

	AAD_DEBUG_TMP("AAD_SIF_GetBandDetect : set : %d(%s) => detect : %d(%s), Quality 0x%X, Magnitude 0x%X\n\n",	\
			   	   pstSystemInfo->setSystem, debugString[ui8SetStrNum], eTVSoundSystem, debugString[ui8DebugStrNum],	\
				   pstSystemInfo->bandStrength, pstSystemInfo->bandMagnitude);

	return RET_OK;
}

/**
 * Checks if a SIF standard such as NICAM, A2(DK), FM exists in the input SIF signal.
 * Gets also a detected sound standard and signal existence..
 * @see
*/
int AAD_SIF_CheckSoundMode ( LX_AAD_SIF_STANDARD_INFO_T *pstStandardInfo )
{
	int			eRetVal = RET_OK;
	UINT8		ui8SysSel;
	UINT8		ui8Sndstat, ui8MonoSt, ui8BiSap;

	UINT8		debugMsg[DEBUG_MSG_BUFFER_SIZE];	//50

	//Set dafault value
	pstStandardInfo->detectedStandard = LX_AAD_SIF_NOT_AVAILE;
	pstStandardInfo->availability     = LX_AAD_SIF_ABSENT;
	strcpy(debugMsg, "RESET ");

	//Read a decoding TV system & standard select and status register
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, syssel, ui8SysSel);

	AAD_RdFL(status);
	AAD_Rd01(status, sndstat,   ui8Sndstat);
	AAD_Rd01(status, mono_str,  ui8MonoSt);
	AAD_Rd01(status, bisap,	    ui8BiSap);
	AAD_PRINT("AAD_SIF_CheckSoundMode : SysSel = %d, status = 0x%2X, sndstat = %d\n",	\
										ui8SysSel, AAD_Rd(status), ui8Sndstat);

	switch ( pstStandardInfo->setStandard )
	{
		case LX_AAD_SIF_NICAM:

			//Check NICAM mode
			strcpy(debugMsg, "NICAM ");

			switch (ui8SysSel)
			{
				case 0x1:
				case 0x2:
				case 0x3:
				case 0x7:
					pstStandardInfo->detectedStandard = LX_AAD_SIF_NICAM;
					AAD_PRINT("LX_AAD_SIF_NICAM\n");

					//Check the availability of SIF carrier
					if ( ui8Sndstat == 0x2 )	//NICAM(digital sound) is available. This status needs long time to set value.
					{
						pstStandardInfo->availability = LX_AAD_SIF_PRESENT;
						AAD_PRINT("LX_AAD_SIF_PRESENT\n");
						strcat(debugMsg, "OK");
					}
					else
					{
						pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
						AAD_PRINT("LX_AAD_SIF_ABSENT\n");
						strcat(debugMsg, "NG");
					}
					break;

				default:
					pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
					AAD_PRINT("LX_AAD_SIF_ABSENT\n");
					strcat(debugMsg, "NG");
					break;
			}
			break;

		case LX_AAD_SIF_A2:

			//Check A2 mode
			strcpy(debugMsg, "A2 ");

			switch (ui8SysSel)
			{
				case 0x0:
				case 0x4:
				case 0x5:
				case 0x6:
				case 0x8:
					pstStandardInfo->detectedStandard = LX_AAD_SIF_A2;
					AAD_PRINT("LX_AAD_SIF_A2\n");
					strcpy(debugMsg, "A2 ");
					break;
			}

			//Check the availability of SIF carrier
			if ( ui8Sndstat == 0x2 )	//NICAM(digital sound) is available. This status needs long time to set value.
			{
				pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
				AAD_PRINT("LX_AAD_SIF_ABSENT\n");
				strcat(debugMsg, "NG(NICAM)");
			}
			else if ( ui8Sndstat == 0x0 )	//Analog Sound Standard(AM/FM) active
			{
				pstStandardInfo->availability = LX_AAD_SIF_PRESENT;
				AAD_PRINT("LX_AAD_SIF_PRESENT\n");
				strcat(debugMsg, "PRESENT");
			}
			else
			{
				pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
				AAD_PRINT("LX_AAD_SIF_ABSENT\n");
				strcat(debugMsg, "NG");
			}
			break;

		case LX_AAD_SIF_A2_DK1:
		case LX_AAD_SIF_A2_DK2:
		case LX_AAD_SIF_A2_DK3:

			//Check A2 mode
			strcpy(debugMsg, "A2 ");

			switch (ui8SysSel)
			{
				case 0x4:
					pstStandardInfo->detectedStandard = LX_AAD_SIF_A2_DK1;
					AAD_PRINT("LX_AAD_SIF_A2_DK1\n");
					strcpy(debugMsg, "A2_DK1 ");
					break;

				case 0x5:
					pstStandardInfo->detectedStandard = LX_AAD_SIF_A2_DK2;
					AAD_PRINT("LX_AAD_SIF_A2_DK2\n");
					strcpy(debugMsg, "A2_DK2 ");
					break;

				case 0x6:
					pstStandardInfo->detectedStandard = LX_AAD_SIF_A2_DK3;
					AAD_PRINT("LX_AAD_SIF_A2_DK3\n");
					strcpy(debugMsg, "A2_DK3 ");
					break;
			}

			//Check the availability of SIF carrier
			if ( ui8Sndstat == 0x2 )	//NICAM(digital sound) is available. This status needs long time to set value.
			{
				pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
				AAD_PRINT("LX_AAD_SIF_ABSENT\n");
				strcat(debugMsg, "NG(NICAM)");
			}
			else if ( ui8Sndstat == 0x0 )	//Analog Sound Standard(AM/FM) active
			{
				pstStandardInfo->availability = LX_AAD_SIF_PRESENT;
				AAD_PRINT("LX_AAD_SIF_PRESENT\n");
				strcat(debugMsg, "PRESENT");
			}
			else
			{
				pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
				AAD_PRINT("LX_AAD_SIF_ABSENT\n");
				strcat(debugMsg, "NG");
			}
			break;

		case LX_AAD_SIF_FM:

			//Check FM mode
			strcpy(debugMsg, "A2 ");

			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
			{
				if ( ui8SysSel == 0xB )
				{
					pstStandardInfo->detectedStandard = LX_AAD_SIF_FM;
					AAD_PRINT("LX_AAD_SIF_FM\n");
					strcpy(debugMsg, "BTSC FM ");
				}
			}
			else
			{
				if ( (( 0x1 <= ui8SysSel ) && ( ui8SysSel <= 0x3 ))\
					||( 0x7 == ui8SysSel )
				    ||(( 0xB <= ui8SysSel ) && ( ui8SysSel <= 0xF )) )
				{
					pstStandardInfo->detectedStandard = LX_AAD_SIF_FM;
					AAD_PRINT("FM \n");
					strcpy(debugMsg, "FM ");
				}
			}

			//Check the availability of SIF carrier
			if ( ui8Sndstat == 0x2 )	//NICAM(digital sound) is available. This status needs long time to set value.
			{
				pstStandardInfo->availability = LX_AAD_SIF_ABSENT;
				AAD_PRINT("LX_AAD_SIF_ABSENT\n");
				strcat(debugMsg, "NG(NICAM)");
			}
			else if ( (ui8Sndstat == 0x0)	//Analog Sound Standard(AM/FM) active
				    ||(ui8MonoSt == 1) || (ui8BiSap == 1) )	//No signal case, this value is set.
			{
				pstStandardInfo->availability = LX_AAD_SIF_PRESENT;
				AAD_PRINT("LX_AAD_SIF_PRESENT\n");
				strcat(debugMsg, "PRESENT");
			}
			else	//Bad reception condition of analogl sound -> PRESENT
			{
				pstStandardInfo->availability = LX_AAD_SIF_PRESENT;
				AAD_PRINT("LX_AAD_SIF_PRESENT\n");
				strcat(debugMsg, "PRESENT");
			}
			break;

		case LX_AAD_SIF_DETECTING_AVALIBILITY:
			eRetVal = RET_INVALID_PARAMS;
			strcpy(debugMsg, "DETEC ");
			break;

		default:
			eRetVal = RET_INVALID_PARAMS;
			strcpy(debugMsg, "INVAL ");
			break;
	}

	//For debugging print
	if(g_io_stStandardInfo.availability != pstStandardInfo->availability)
	{
		AAD_DEBUG_TMP("CheckSoundMode : %d -> %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
					   pstStandardInfo->setStandard, pstStandardInfo->detectedStandard, debugMsg, ui8SysSel, AAD_Rd(status));
	}
	else if((g_ui32AadPrintCount % DEBUG_MSG_PRINT_TIME) == 0) //DDI calls every 30 ms
	{
		AAD_DEBUG_TMP("CheckSoundMode : %d -> %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
					   pstStandardInfo->setStandard, pstStandardInfo->detectedStandard, debugMsg, ui8SysSel, AAD_Rd(status));
	}

	//Copy for global value
	g_io_stStandardInfo.detectedStandard = pstStandardInfo->detectedStandard;
	g_io_stStandardInfo.availability 	 = pstStandardInfo->availability;

	AAD_PRINT("AAD_SIF_CheckSoundMode : %d -> %d(%s)(SysSel = %d, status = 0x%2X)\n",	\
			   pstStandardInfo->setStandard, pstStandardInfo->detectedStandard, debugMsg, ui8SysSel, AAD_Rd(status));

 	return eRetVal;
}

/**
 * Gets a threshold level in the input SIF signal.
 * The current stereo level is returned using LX_AAD_SIF_THRESHOLD_T value.
 * @see
*/
int AAD_SIF_GetThresholdLevel ( LX_AAD_SIF_THRESHOLD_T	*pLevel )
{
	//Check the threshold mode
	if(pLevel->mode & LX_AAD_THRES_CAR2_MAGITUDE)
	{
		//Get a BTSC and A2 Threshold level
		pLevel->carrier2Magitude = g_stThresholdLevel.carrier2Magitude;

		AAD_DEBUG("AAD_SIF_GetBTSCA2Level : carrier2Magitude = 0x%X\n", g_stThresholdLevel.carrier2Magitude);
	}

	//Check the threshold mode
	if(pLevel->mode & LX_AAD_THRES_CAR2_QUALITY_HI)
	{
		//Get a BTSC and A2 Threshold level
		pLevel->carrier2QualityHigh = g_stThresholdLevel.carrier2QualityHigh;

		AAD_DEBUG("AAD_SIF_GetBTSCA2Level : CAR2 Quality High = 0x%X, Low = 0x%X\n",	\
				   g_stThresholdLevel.carrier2QualityHigh, g_stThresholdLevel.carrier2QualityLow);
	}

	//Check the threshold mode
	if(pLevel->mode & LX_AAD_THRES_CAR2_QUALITY_LO)
	{
		//Get a BTSC and A2 Threshold level
		pLevel->carrier2QualityLow  = g_stThresholdLevel.carrier2QualityLow;

		AAD_DEBUG("AAD_SIF_GetBTSCA2Level : CAR2 Quality High = 0x%X, Low = 0x%X\n",	\
				   g_stThresholdLevel.carrier2QualityHigh, g_stThresholdLevel.carrier2QualityLow);
	}

	//Check the threshold mode
	if(pLevel->mode & LX_AAD_THRES_ID_MAGNITUDE)
	{
		//Read registers
		AAD_RdFL(id_mag_thd_hi);
		AAD_RdFL(id_mag_thd_lo);

		//Get a BTSC and A2 Threshold level
		pLevel->stereoIDMagHigh = AAD_Rd(id_mag_thd_hi);
		pLevel->stereoIDMagLow  = AAD_Rd(id_mag_thd_lo);

		AAD_DEBUG("AAD_SIF_SetBTSCA2Level(Register): Stereo ID High = 0x%X, Low = 0x%X\n",	\
				   AAD_Rd(id_mag_thd_hi), AAD_Rd(id_mag_thd_lo));

		AAD_DEBUG("AAD_SIF_GetBTSCA2Level(Global)  : Stereo ID High = 0x%X, Low = 0x%X\n",	\
				   g_stThresholdLevel.stereoIDMagHigh, g_stThresholdLevel.stereoIDMagLow);
	}

	//Check the threshold mode
	if ( pLevel->mode & LX_AAD_THRES_NICAM_ERROR )
	{
		//Get a NICAM Error Rate Threshold level
		pLevel->nicamErrorRateHigh = g_stThresholdLevel.nicamErrorRateHigh;
		pLevel->nicamErrorRateLow  = g_stThresholdLevel.nicamErrorRateLow;

		AAD_DEBUG("AAD_SIF_GetBTSCA2Level : NICAM High = 0x%X, Low = 0x%X\n",	\
				   g_stThresholdLevel.nicamErrorRateHigh, g_stThresholdLevel.nicamErrorRateLow);
	}

	//Debug Print A2 Threshold Level
	//Read registers
	AAD_RdFL(car2_qual_aaos_thd_hi);
	AAD_RdFL(car2_qual_aaos_thd_lo);
	AAD_RdFL(id_mag_thd_hi);
	AAD_RdFL(id_mag_thd_lo);
	AAD_RdFL(nicam_err_aaos_thd_hi);
	AAD_RdFL(nicam_err_aaos_thd_lo);

	AAD_DEBUG("AAD_SIF_GetBTSCA2Level : carrier2Magitude = 0x%X\n", g_stThresholdLevel.carrier2Magitude);
	AAD_DEBUG("AAD_SIF_GetBTSCA2Level : CAR2 High  = 0x%X, Low = 0x%X, ID High = 0x%X, Low = 0x%X\n",	\
			   AAD_Rd(car2_qual_aaos_thd_hi), AAD_Rd(car2_qual_aaos_thd_lo),	\
			   AAD_Rd(id_mag_thd_hi), AAD_Rd(id_mag_thd_lo));
	AAD_DEBUG("AAD_SIF_GetBTSCA2Level : NICAM High = 0x%X, Low = 0x%X\n",	\
			   AAD_Rd(nicam_err_aaos_thd_hi), AAD_Rd(nicam_err_aaos_thd_lo));

	return RET_OK;
}


/**
 * Gets the setting value of analog TV stereo mode by user or analog task.
 * Analog Audio Output Mode is returned from User Mode or Analog Audio Task in Analog Audio Decoder.
 * @see
*/
int AAD_SIF_GetCurAnalogMode ( LX_ATV_AUDIO_MODE_GET_T *pAudioMode )
{
	UINT8 		ui8SysSel;
	UINT8		ui8NicamCtrlBits;
	UINT32		ui8PrmCarDet, ui8SecCarDet, ui8MonoSt, ui8BiSap;

	UINT8		value;
	UINT16	  	ui16Carrier2Magnitude = 0;
	UINT16	  	ui16Carrier2Quality   = 0;
	UINT16	  	ui16Carrier2QualityQCTest   = CAR2_QUAL_THRES_HI_QC_TEST;
	UINT16	  	ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX;
	UINT16	  	ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN;
	UINT8		ui8StIdMag = 0;
	UINT8		ui8BiIdMag = 0;

	LX_ATV_AUDIO_MODE_GET_T 	eAudioMode;

	UINT8 *debugString[25] = {"PAL_MONO", 	"PAL_STEREO", 	"PAL_DUAL", 	"NICAM_MONO", 		"NICAM_STEREO",	\
							  "NICAM_DUAL", "PAL_UNKNOWN", 	"ERROR1", 		"ERROR2", 			"ERROR3", \
							  "ERROR4", 	"ERROR5", 		"ERROR6", 		"ERROR7", 			"ERROR8", \
							  "ERROR9", 	"A2_MONO", 		"A2_STEREO", 	"A2_SAP", 			"A2_UNKNOWN", \
							  "BTSC_MONO", 	"BTSC_STEREO", 	"BTSC_SAP_MONO","BTSC_SAP_STEREO", 	"BTSC_UNKNOWN" };

	//Mono, Stereo and SAP detection checks primary and secondardary carrier  because of THAT IP bug.
	//Read a decoding TV system & standard select and status register
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, syssel, ui8SysSel);

	//Read status register
	AAD_RdFL(status);
	AAD_Rd01(status, prmcardet, ui8PrmCarDet);
	AAD_Rd01(status, seccardet, ui8SecCarDet);
	AAD_Rd01(status, mono_str,  ui8MonoSt);
	AAD_Rd01(status, bisap,	    ui8BiSap);

	//Set a default audio mode
	eAudioMode = LX_ATV_GET_PAL_MONO;

	//Audio Output Result Register
	switch ( ui8SysSel )
	{
		case 0x1://FM-Mono/NICAM(B/G)
		case 0x2://AM-Mono/NICAM(L)
		case 0x3://FM-Mono/NICAM(I)
		case 0x7://FM-Mono/NICAM(D/K)
		{
			//Just read the high byte so that low byte will be updated
			AAD_RdFL(nicam_ctrl_bits0);
			AAD_RdFL(nicam_ctrl_bits1);
			AAD_Rd01(nicam_ctrl_bits1, nicctl, ui8NicamCtrlBits);

			ui8NicamCtrlBits = (ui8NicamCtrlBits & 0xE) >> 1;
			AAD_PRINT("NICAM Control bits 0x%x\n", ui8NicamCtrlBits);

			if ((ui8BiSap == 1) && (ui8NicamCtrlBits == 0x2 ))			//Dual Mode
			{
				eAudioMode = LX_ATV_GET_PAL_NICAM_DUAL;
			}
			else if ((ui8MonoSt == 1) && (ui8NicamCtrlBits == 0x0 ))	//Stereo Mode
			{
				eAudioMode = LX_ATV_GET_PAL_NICAM_STEREO;
			}
			else if ((ui8MonoSt == 0) && (ui8NicamCtrlBits == 0x1 ))	//Mono Mode
			{
				eAudioMode = LX_ATV_GET_PAL_NICAM_MONO;
			}
			else if ((ui8MonoSt == 0) && (ui8NicamCtrlBits == 0x3 ))	//Data Mode
			{
				eAudioMode = LX_ATV_GET_PAL_NICAM_MONO;
			}
			else //default
			{
				AAD_ISR_DEBUG("PAL NICAM : Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

				eAudioMode = LX_ATV_GET_PAL_MONO;
			}
		}
		break;

		case 0x0://FM-Stereo (A2, B/G)
		{
			if ((ui8PrmCarDet == 1) && (ui8SecCarDet == 1))
			{
				if (ui8MonoSt == 1)		//Stereo
				{
					eAudioMode = LX_ATV_GET_PAL_STEREO;
				}
				else if (ui8BiSap == 1)	//Bilingual
				{
					eAudioMode = LX_ATV_GET_PAL_DUAL;
				}
				else if (ui8MonoSt == 0)//Mono
				{
					eAudioMode = LX_ATV_GET_PAL_MONO;
				}
				else	//default 1 : Unknown -> Mono
				{
					AAD_ISR_DEBUG("PAL A2 : 1_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
				}
			}
			else if (ui8PrmCarDet == 1)
			{
				if (ui8MonoSt == 0)		//Mono
				{
					eAudioMode = LX_ATV_GET_PAL_MONO;
				}
				else	//default 2 : Unknown -> Mono(GP3 SW search other sound mode if audio mode is unknown.)
				{
					AAD_ISR_DEBUG("PAL A2 : 2_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
				}
			}
			else if (ui8SecCarDet == 1)
			{
				AAD_RdFL(stereo_id_mag);
				AAD_Rd01(stereo_id_mag, stmag, ui8StIdMag);

				AAD_RdFL(biling_id_mag);
				AAD_Rd01(biling_id_mag, bimag, ui8BiIdMag);

				//0xF8 : ID Magnitude for Turkey(field stream issue : Stereo)
				if( (ui8StIdMag >= ID_MAG_THRES_DVB_A2_BIG)	\
				  ||(ui8BiIdMag >= ID_MAG_THRES_DVB_A2_BIG) )
				{
					//This case statement check only ST/ID value.(The ui8MonoSt and ui8BiSap varies continuously.)
					if(ui8StIdMag >= ui8BiIdMag)
					{
						eAudioMode = LX_ATV_GET_PAL_STEREO;
					}
					else
					{
						eAudioMode = LX_ATV_GET_PAL_DUAL;
					}
				}
				else	//default 3 : Unknown -> Mono(GP3 SW search other sound mode if audio mode is unknown.)
				{
					AAD_ISR_DEBUG("PAL A2 : 3_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

					*pAudioMode = LX_ATV_GET_PAL_UNKNOWN;
				}
			}
			else		//default 4 : Unknown -> Mono
			{
				AAD_ISR_DEBUG("PAL A2 : 4_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

				eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
			}
			break;
		}

		case 0x4://FM-Stereo (A2, D/K1)
		case 0x5://FM-Stereo (A2, D/K2)
		case 0x6://FM-Stereo (A2, D/K3)
		{
			/* Check a AAD Monitor Task for SIF D/K Mode */
			if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
			{
				if ((ui8PrmCarDet == 1) && (ui8SecCarDet == 1))
				{
					if (ui8MonoSt == 1)		//Stereo
					{
						eAudioMode = LX_ATV_GET_PAL_STEREO;
					}
					else if (ui8BiSap == 1)	//Bilingual
					{
						eAudioMode = LX_ATV_GET_PAL_DUAL;
					}
					else if (ui8MonoSt == 0)//Mono
					{
						eAudioMode = LX_ATV_GET_PAL_MONO;
					}
					else	//default 1 : Unknown -> Mono
					{
						AAD_ISR_DEBUG("PAL A2 : 1_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

						eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
					}
				}
				else if (ui8PrmCarDet == 1)
				{
					if (ui8MonoSt == 0)		//Mono
					{
						eAudioMode = LX_ATV_GET_PAL_MONO;
					}
					else	//default 2 : Unknown -> Mono(GP3 SW search other sound mode if audio mode is unknown.)
					{
						AAD_ISR_DEBUG("PAL A2 : 2_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

						eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
					}
				}
				else if (ui8SecCarDet == 1)
				{
					AAD_RdFL(stereo_id_mag);
					AAD_Rd01(stereo_id_mag, stmag, ui8StIdMag);

					AAD_RdFL(biling_id_mag);
					AAD_Rd01(biling_id_mag, bimag, ui8BiIdMag);

					//0xF8 : ID Magnitude for Turkey(field stream issue : Stereo)
					if( (ui8StIdMag >= ID_MAG_THRES_DVB_A2_BIG)	\
					  ||(ui8BiIdMag >= ID_MAG_THRES_DVB_A2_BIG) )
					{
						//This case statement check only ST/ID value.(The ui8MonoSt and ui8BiSap varies continuously.)
						if(ui8StIdMag >= ui8BiIdMag)
						{
							eAudioMode = LX_ATV_GET_PAL_STEREO;
						}
						else
						{
							eAudioMode = LX_ATV_GET_PAL_DUAL;
						}
					}
					else	//default 3 : Unknown -> Mono(GP3 SW search other sound mode if audio mode is unknown.)
					{
						AAD_ISR_DEBUG("PAL A2 : 3_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

						*pAudioMode = LX_ATV_GET_PAL_UNKNOWN;
					}
				}
				else		//default 4 : Unknown -> Mono
				{
					AAD_ISR_DEBUG("PAL A2 : 4_Unknown -> Mono(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
				}
			}
			else
			{
				//Get a audio mode from aad monitor task.
				//Check a AAD monitor task for PAL A2 D/K Signal
				eAudioMode = g_MonitorAudioMode;

				AAD_ISR_DEBUG("PAL A2 D/K: eAudioMode = %d(status = 0x%X)\n", eAudioMode, AAD_Rd(status));
			}
			break;
		}

		case 0x8://FM-Stereo (A2, M/N)
		{
			//For default case
			eAudioMode = LX_ATV_GET_NTSC_A2_MONO;

			if(ui8PrmCarDet == 1)
			{
#ifdef	ENABLE_AAD_TEST_MODE	//test only
#else
				//Check for HE QE TEST SIF signal
				AAD_RdFL(car2_fm_qual0);
				AAD_Rd01(car2_fm_qual0, cr2ql, value);
				ui16Carrier2Quality  = value << 8;

				AAD_RdFL(car2_fm_qual1);
				AAD_Rd01(car2_fm_qual1, cr2ql, value);
				ui16Carrier2Quality += value;

				/* Check LX Chip Revision Number */
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					ui16Carrier2QualityQCTest	= CAR2_QUAL_THRES_HI_QC_TEST;		//0x600
					ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX;		//0x5FF
					ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN;		//0x400
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					ui16Carrier2QualityQCTest	= CAR2_QUAL_THRES_HI_QC_TEST;		//0x600
					ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX;		//0x5FF
					ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN;		//0x400
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					/* Check a SIF Internal Source. */
					if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
					{
						ui16Carrier2QualityQCTest	= CAR2_QUAL_THRES_HI_QC_TEST_M14_INT;		//0x580
						ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX_M14_INT; 	//0x57F
						ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN_M14_INT; 	//0x280
					}
					else
					{
						ui16Carrier2QualityQCTest	= CAR2_QUAL_THRES_HI_QC_TEST;		//0x600
						ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX; 	//0x5FF
						ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN; 	//0x400
					}
				}
				else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					ui16Carrier2QualityQCTest	= CAR2_QUAL_THRES_HI_QC_TEST;		//0x600
					ui16Carrier2QualityIlsanMax = CAR2_QUAL_THRES_HI_ILSAN_MAX;		//0x5FF
					ui16Carrier2QualityIlsanMin = CAR2_QUAL_THRES_HI_ILSAN_MIN;		//0x400
				}

				//Org. threshold high is 0x64 and low is 0x14.
				AAD_RdFL(id_mag_thd_hi);
				AAD_RdFL(id_mag_thd_lo);

				if(ui16Carrier2Quality >= ui16Carrier2QualityQCTest)	//0x600
				{
					AAD_Wr01(id_mag_thd_hi, idthh, g_stThresholdLevel.stereoIDMagHigh);
					AAD_Wr01(id_mag_thd_lo, idthl, g_stThresholdLevel.stereoIDMagLow);
				}
				else if( (ui16Carrier2Quality <= ui16Carrier2QualityIlsanMax)	\
					   &&(ui16Carrier2Quality >= ui16Carrier2QualityIlsanMin) )	//0x400 ~ 0x5FF, Ulsan Ilsan-Dong field stream issue
				{
					AAD_Wr01(id_mag_thd_hi, idthh, ID_MAG_THRES_HI_ATSC_A2_ILSAN);	//0x28
					AAD_Wr01(id_mag_thd_lo, idthl, ID_MAG_THRES_LO_ATSC_A2_ILSAN);	//0x12
				}
				else
				{
					AAD_Wr01(id_mag_thd_hi, idthh, ID_MAG_THRES_HI_ATSC_A2);	//0x70
					AAD_Wr01(id_mag_thd_lo, idthl, ID_MAG_THRES_LO_ATSC_A2);	//0x12
				}

				AAD_WrFL(id_mag_thd_hi);
				AAD_WrFL(id_mag_thd_lo);
#endif	//#ifdef	ENABLE_AAD_TEST_MODE	//test only

				//Check for Live or Field SIF signal
				AAD_RdFL(car2_aver_mag0);
				AAD_Rd01(car2_aver_mag0, cr2mg, value);
				ui16Carrier2Magnitude  = value << 8;

				AAD_RdFL(car2_aver_mag1);
				AAD_Rd01(car2_aver_mag1, cr2mg, value);
				ui16Carrier2Magnitude += value;

				AAD_RdFL(stereo_id_mag);
				AAD_Rd01(stereo_id_mag, stmag, ui8StIdMag);

				AAD_RdFL(biling_id_mag);
				AAD_Rd01(biling_id_mag, bimag, ui8BiIdMag);

				//0xF8 : ID Magnitude for Korea(Chungnam Asan field stream issue : Stereo)
				if( (ui8SecCarDet == 1)		\
				  &&((ui8StIdMag >= ID_MAG_THRES_ATSC_A2_BIG) || (ui8BiIdMag >= ID_MAG_THRES_ATSC_A2_BIG)) \
				  &&(ui16Carrier2Magnitude >= CAR2_MAG_THRES_ATSC_A2_US04) )
				{
					AAD_PRINT("A2 : GetCurAnalogMode : StId = 0x%X, BiId = 0x%X(status = 0x%X)\n", ui8StIdMag, ui8BiIdMag, AAD_Rd(status));

					//This case statement check only ST/ID value.(The ui8MonoSt and ui8BiSap varies continuously.)
					if(ui8StIdMag >= ui8BiIdMag)
					{
						eAudioMode = LX_ATV_GET_NTSC_A2_STEREO;
					}
					else
					{
						eAudioMode = LX_ATV_GET_NTSC_A2_SAP;
					}
				}
				//Normal Case  or 0x0CD0 : -22.5(?)dbFS for Korea(West Door field stream issue : Mono)
				//Normal Case  or 0x0800 : -22.5(?)dbFS for Korea(Ra Sung field stream issue : Stereo)
				//Normal Case  or 0x400 ~ 0x5FF : Ulsan Ilsan-Dong field stream issue : A2_SAP
#ifdef	ENABLE_AAD_TEST_MODE	//test only
				else if( ui8SecCarDet == 1 )
#else
				else if( (ui8SecCarDet == 1) && (ui16Carrier2Magnitude >= CAR2_MAG_THRES_ATSC_A2) )
#endif	//#ifndef	ENABLE_AAD_TEST_MODE	//test only
				{
					AAD_PRINT("A2 : GetCurAnalogMode : Car2Mag = 0x%X, Car2Qual = 0x%X(status = 0x%X)\n", ui16Carrier2Magnitude, ui16Carrier2Quality, AAD_Rd(status));

					if(ui8MonoSt == 1)
					{
						eAudioMode = LX_ATV_GET_NTSC_A2_STEREO;
					}
					else if(ui8BiSap == 1)
					{
						eAudioMode = LX_ATV_GET_NTSC_A2_SAP;
					}
				}

				AAD_PRINT("A2 : GetCurAnalogMode : SecCar = %d, ui8MonoSt = %d, ui8BiSap = %d(status = 0x%X)\n", ui8SecCarDet, ui8MonoSt, ui8BiSap, AAD_Rd(status));
			}
			break;
		}

		case 0xA://FM-Stereo + SAP (BTSC) or FM-Stereo (Radio - Europe)
		case 0xC://FM-Stereo (Radio - Europe)
		case 0xD://FM-Stereo (Radio - Europe)
		case 0xE://FM-Stereo (Radio - Europe)
		{
			/* BTSC */
			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )	 //FM-Stereo + SAP (BTSC)
			{
				/* Do not use ui8SecCarDet flag because ui8SecCarDet flag does not set in BTSC SAP signal.(2013.03.09) */
				if (ui8PrmCarDet == 1)
				{
					if ((ui8BiSap == 1) && (ui8MonoSt == 1))	//SAP and Stereo
					{
						eAudioMode = LX_ATV_GET_NTSC_BTSC_SAP_STEREO;
					}
					else if (ui8BiSap == 1)		//SAP and Mono
					{
						eAudioMode = LX_ATV_GET_NTSC_BTSC_SAP_MONO;
					}
					else if (ui8MonoSt == 1)	//Stereo
					{
						eAudioMode = LX_ATV_GET_NTSC_BTSC_STEREO;
					}
					else if (ui8MonoSt == 0)	//Mono
					{
						eAudioMode = LX_ATV_GET_NTSC_BTSC_MONO;
					}
					else	//default 1 : Unknown -> Mono(NTSC system request any audio mode.)
					{
						AAD_ISR_DEBUG("BTSC : 1_Undef -> Mono(status = 0x%X)\n", AAD_Rd(status));

						eAudioMode = LX_ATV_GET_NTSC_BTSC_MONO;
					}
				}
				else		//default 2 : Unknown -> Mono(NTSC system request any audio mode.)
				{
					AAD_ISR_DEBUG("BTSC : 2_Undef -> Mono(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_NTSC_BTSC_MONO;
				}
			}
			else //FM-Stereo (Radio - Europe)
			{
				if (ui8PrmCarDet == 1)
				{
					if (ui8BiSap == 1)
					{
						eAudioMode = LX_ATV_GET_PAL_DUAL;
					}
					else if (ui8MonoSt == 1)
					{
						eAudioMode = LX_ATV_GET_PAL_STEREO;
					}
					else if (ui8MonoSt == 0)
					{
						eAudioMode = LX_ATV_GET_PAL_MONO;
					}
					else	//default 1 : Unknown
					{
						AAD_ISR_DEBUG("FM-Radio : 1_Undef(status = 0x%X)\n", AAD_Rd(status));

						eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
					}
				}
				else		//default 2 : Unknown
				{
					AAD_ISR_DEBUG("FM-Radio : 2_Undef(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
				}
			}
			break;
		}

		case 0x9://FM-Stereo (EIA-J)
		case 0xB://FM-Stereo (Radio - US)
		{
			if (ui8PrmCarDet == 1)
			{
				if (ui8MonoSt == 1)		//Stereo
				{
					eAudioMode = LX_ATV_GET_NTSC_A2_STEREO;
				}
				else if (ui8BiSap == 1)	//Bilingual
				{
					eAudioMode = LX_ATV_GET_NTSC_A2_SAP;
				}
				else if (ui8MonoSt == 0)//Mono
				{
					eAudioMode = LX_ATV_GET_NTSC_A2_MONO;
				}
				else	//default 1 : Unknown -> Mono
				{
					AAD_ISR_DEBUG("NTSC : 1_Undef -> Mono(status = 0x%X)\n", AAD_Rd(status));

					eAudioMode = LX_ATV_GET_NTSC_A2_MONO;
				}
			}
			else		//default 2 : Unknown -> Mono
			{
				AAD_ISR_DEBUG("NTSC : 2_Undef -> Mono(status = 0x%X)\n", AAD_Rd(status));

				eAudioMode = LX_ATV_GET_NTSC_A2_MONO;
			}
		}
		break;

		case 0xF:	//unknown/FM-Stereo (Radio - Europe)
		default:
			AAD_ISR_DEBUG("default : PAL unknown(status = 0x%X)\n", AAD_Rd(status));

			eAudioMode = LX_ATV_GET_PAL_UNKNOWN;
			break;
	}

	//Sanity check for debug print
	if( eAudioMode >= LX_ATV_GET_NTSC_BTSC_UNKNOWN )
		eAudioMode = LX_ATV_GET_NTSC_BTSC_UNKNOWN;

	//Check a AAD monitor task for KOREA A2 Signal
	if ( (g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT)	\
	   ||(g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT && g_i_eSystem == LX_AAD_SIF_SYSTEM_MN) )
	{
		//Check a forced ST / BI status
		if(g_MonitorAudioMode != LX_ATV_GET_NTSC_A2_UNKNOWN)
		{
			//Copy for return value to forced mono.
			eAudioMode = LX_ATV_GET_NTSC_A2_MONO;

			if((g_ui32AadPrintCount++ % DEBUG_MSG_PRINT_TIME) == 0)	//DDI calls every 30 ms in DVB mode or 20 ms in ATSC mode
			{
				AAD_DEBUG_TMP("GetCurAnalogMode(g_MonitorAudioMode = 0x%X) : Get 0x%x(%s)(SysSel = %d, status = 0x%2X)\n",	\
						   	   g_MonitorAudioMode, eAudioMode, debugString[eAudioMode], ui8SysSel, AAD_Rd(status));
			}
		}
	}

	//For debugging print
	if( g_o_eAudioMode != eAudioMode )
	{
		//Read registers
		AAD_RdFL(audio_out_ctrl);

		AAD_DEBUG("GetAnalogMode(g_MonitorAudioMode = 0x%X) : 0x%x(%s)(SysSel = %d, status = 0x%2X, output = 0x%2X)\n",	\
				   g_MonitorAudioMode, eAudioMode, debugString[eAudioMode], ui8SysSel, AAD_Rd(status), AAD_Rd(audio_out_ctrl));
	}
	else
	{
		if((g_ui32AadPrintCount++ % DEBUG_MSG_PRINT_TIME) == 0)	//DDI calls every 30 ms in DVB mode or 20 ms in ATSC mode
		{
			AAD_DEBUG_TMP("GetAnalogMode  : Mode 0x%x(%s)(SysSel = %d, status = 0x%2X)\n",	\
						   eAudioMode, debugString[eAudioMode], ui8SysSel, AAD_Rd(status));

			if(ui16Carrier2Magnitude)
			{
				AAD_DEBUG_TMP("Car2Mag = 0x%04x, Car2Ql = 0x%04x, StIdMag = 0x%x, BiIdMag = 0x%x\n",	\
						   	   ui16Carrier2Magnitude, ui16Carrier2Quality, ui8StIdMag, ui8BiIdMag);
			}
		}
	}

	//Copy for return value
	*pAudioMode = eAudioMode;

	//Copy for global value
	g_o_eAudioMode = eAudioMode;

	AAD_PRINT("AAD_SIF_GetCurAnalogMode : Get 0x%x(%s)(SysSel = %d, status = 0x%2X)\n",	\
			   eAudioMode, debugString[eAudioMode], ui8SysSel, AAD_Rd(status));

	return RET_OK;
}


/**
 * Checks whether SIF(Sound Intermediate Frequency) carrier exists or not.
 * This function is used for checking the SIF of the current analog signal.
 * @see
*/
int AAD_SIF_GetSIFSatus ( BOOLEAN *pSIFStatus )
{
	UINT8	ui8PrmCarDet, ui8SecCarDet;

	//Check if any primary or secondary carrier is detected using status register and return as boolean value.
	AAD_RdFL(status);
	AAD_Rd01(status, prmcardet, ui8PrmCarDet);
	AAD_Rd01(status, seccardet, ui8SecCarDet);

	//Check primary or secondary carrier status
	if ((ui8PrmCarDet == 1) || (ui8SecCarDet == 1))
	{
		*pSIFStatus = TRUE;
	}
	else
	{
		*pSIFStatus = FALSE;
	}

	//Check carrier status for debug
	(void) AAD_ASD_CheckCarrierDetection();

	AAD_DEBUG("AAD_SIF_GetSIFSatus : %d(status = 0x%X)\n", *pSIFStatus, AAD_Rd(status));

	return RET_OK;
}

/**
* Enables or Disables a AAD Monitor task for detection A2 mono signal.
* This function is used to workaround code for misdection Mono and Stereo signal.
 * @see
*/
int AAD_SIF_SetTaskSatus ( BOOLEAN bTaskStatus )
{
	// AAD Monitor Task Start for KOREA A2 Signal
	if( bTaskStatus == TRUE )
	{
		g_suiAadWaitQForMonitor = LX_AAD_TASK_START;
	}
	else	//AAD Monitor Task Stop for KOREA A2 Signal
	{
		g_suiAadWaitQForMonitor = LX_AAD_TASK_STOP;
	}

	//Send event for monitor task
	wake_up_interruptible_all(&Aad_Monitor_WaitQ);

	AAD_DEBUG("AAD_SIF_SetTaskSatus : bTaskStatus = %d\n", bTaskStatus);
	return RET_OK;
}

/**
 * Set AAD SIF input port.
 * @see
*/
int AAD_SIF_SetAADSIFInputPort(LX_AAD_SIF_INPUT_T sifSource)
{
	//Save current AAD SIF Input Source
	g_i_eSifSource = sifSource;

	AAD_WARN("AAD_SIF_SetAADSIFInputPort : sifSource = %d\n", sifSource);
	return RET_OK;
}

/**
 * Read write to registers.
 * This function is used for verification.
 * @see
*/
int AAD_SIF_ReadAndWriteReg ( LX_AAD_REG_INFO_T	*pstRegInfo )
{
	//Check a vitual memory access
	if(pstRegInfo->addr < (sizeof (AAD_REG_T) - 4))
	{
		switch (pstRegInfo->mode)
		{
			case LX_AAD_REG_READ:
			{
				memcpy ( &(pstRegInfo->readdata), (UINT32*)(((UINT32)gpRealRegAAD) + pstRegInfo->addr), sizeof (UINT32));
				AAD_PRINT ("<<< AAD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_WRITE:
			{
				AAD_PRINT (">>> AAD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
				memcpy ((UINT32*)(((UINT32)gpRealRegAAD) + pstRegInfo->addr), (UINT32*)&pstRegInfo->writedata, sizeof (UINT32));
			}
			break;

			case LX_AAD_REG_WRNRD:
			{
				AAD_PRINT (">>> AAD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);
				memcpy ((UINT32*)(((UINT32)gpRealRegAAD) + pstRegInfo->addr), (UINT32*)&pstRegInfo->writedata, sizeof (UINT32));
				AAD_SIF_TIME_DELAY_MS (AAD_REG_READ_DELAY);
				memcpy ( &(pstRegInfo->readdata), (UINT32*)(((UINT32)gpRealRegAAD) + pstRegInfo->addr), sizeof (UINT32));
				AAD_PRINT ("<<< AAD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_GET_MAX_ADDR:
			{
				pstRegInfo->readdata = sizeof (AAD_REG_T) - 4 + ADDR_OF_SECAM_L_MAX;
				AAD_PRINT ("<<< LX_AAD_REG_GET_MAX_ADDR	addr = 0x%2X\n", pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_READ_ARRAY:
			case LX_AAD_REG_READ_ALL:
			default:
				return RET_ERROR;
				break;
		}
	}
	else
	{
		switch (pstRegInfo->mode)
		{
			case LX_AAD_REG_READ:
			{
				/* Check LX Chip Revision Number */
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H15 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H15)
						pstRegInfo->readdata = g_ui32AadSECAMVolume_H15[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H15)/4];
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H15)
						pstRegInfo->readdata = g_ui32AadSECAMDisable_H15;
					else
						pstRegInfo->readdata = 0;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN && pstRegInfo->addr <= ADDR_OF_MAG_MAX)
						pstRegInfo->readdata = g_ui32AadSECAMVolume[(pstRegInfo->addr - ADDR_OF_MAG_MIN)/4];
					else if(pstRegInfo->addr == ADDR_OF_WEIGHT)
						pstRegInfo->readdata = g_ui32AadSECAMWeight;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE)
						pstRegInfo->readdata = g_ui32AadSECAMDisable;
					else
						pstRegInfo->readdata = 0;
				}
				else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H13 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H13)
						pstRegInfo->readdata = g_ui32AadSECAMVolume_H13[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H13)/4];
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H13)
						pstRegInfo->readdata = g_ui32AadSECAMDisable_H13;
					else
						pstRegInfo->readdata = 0;
				}

				AAD_PRINT ("<<< AAD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_WRITE:
			{
				AAD_PRINT (">>> AAD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);

				/* Check LX Chip Revision Number */
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H15 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H15)
						g_ui32AadSECAMVolume_H15[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H15)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H15)
						g_ui32AadSECAMDisable_H15 = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN && pstRegInfo->addr <= ADDR_OF_MAG_MAX)
						g_ui32AadSECAMVolume[(pstRegInfo->addr - ADDR_OF_MAG_MIN)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_WEIGHT)
						g_ui32AadSECAMWeight = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE)
						g_ui32AadSECAMDisable = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;
				}
				else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H13 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H13)
						g_ui32AadSECAMVolume_H13[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H13)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H13)
						g_ui32AadSECAMDisable_H13 = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;
				}
			}
			break;

			case LX_AAD_REG_WRNRD:
			{
				AAD_PRINT (">>> AAD_REG_WRITE addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->writedata);

				/* Check LX Chip Revision Number */
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H15 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H15)
						g_ui32AadSECAMVolume_H15[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H15)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H15)
						g_ui32AadSECAMDisable_H15 = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;

					AAD_SIF_TIME_DELAY_MS (AAD_REG_READ_DELAY);

					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H15 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H15)
						pstRegInfo->readdata = g_ui32AadSECAMVolume_H15[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H15)/4];
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H15)
						pstRegInfo->readdata = g_ui32AadSECAMDisable_H15;
					else
						pstRegInfo->readdata = 0;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					;
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN && pstRegInfo->addr <= ADDR_OF_MAG_MAX)
						g_ui32AadSECAMVolume[(pstRegInfo->addr - ADDR_OF_MAG_MIN)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_WEIGHT)
						g_ui32AadSECAMWeight = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE)
						g_ui32AadSECAMDisable = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;

					AAD_SIF_TIME_DELAY_MS (AAD_REG_READ_DELAY);

					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN && pstRegInfo->addr <= ADDR_OF_MAG_MAX)
						pstRegInfo->readdata = g_ui32AadSECAMVolume[(pstRegInfo->addr - ADDR_OF_MAG_MIN)/4];
					else if(pstRegInfo->addr == ADDR_OF_WEIGHT)
						pstRegInfo->readdata = g_ui32AadSECAMWeight;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE)
						pstRegInfo->readdata = g_ui32AadSECAMDisable;
					else
						pstRegInfo->readdata = 0;
				}
				else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H13 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H13)
						g_ui32AadSECAMVolume_H13[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H13)/4] = pstRegInfo->writedata;
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H13)
						g_ui32AadSECAMDisable_H13 = pstRegInfo->writedata;
					else
						pstRegInfo->writedata = 0;

					AAD_SIF_TIME_DELAY_MS (AAD_REG_READ_DELAY);

					if(pstRegInfo->addr >= ADDR_OF_MAG_MIN_H13 && pstRegInfo->addr <= ADDR_OF_MAG_MAX_H13)
						pstRegInfo->readdata = g_ui32AadSECAMVolume_H13[(pstRegInfo->addr - ADDR_OF_MAG_MIN_H13)/4];
					else if(pstRegInfo->addr == ADDR_OF_ADJUST_DISABLE_H13)
						pstRegInfo->readdata = g_ui32AadSECAMDisable_H13;
					else
						pstRegInfo->readdata = 0;
				}

				AAD_PRINT ("<<< AAD_REG_READ  addr = 0x%2X, data = 0x%2X\n", pstRegInfo->addr, pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_GET_MAX_ADDR:
			{
				pstRegInfo->readdata = sizeof (AAD_REG_T) - 4 + ADDR_OF_SECAM_L_MAX;
				AAD_PRINT ("<<< LX_AAD_REG_GET_MAX_ADDR	addr = 0x%2X\n", pstRegInfo->readdata );
			}
			break;

			case LX_AAD_REG_READ_ARRAY:
			case LX_AAD_REG_READ_ALL:
			default:
				return RET_ERROR;
				break;
		}
	}

	return RET_OK;
}

/**
 * Start ASD function.
*/
int AAD_DoASD ( LX_AAD_SIF_SOUNDSYSTEM_T eSoundSystem, BOOLEAN bScanMode )
{
	int			eRetVal = RET_ERROR;

	UINT8		ui8SysSel = 0;

	UINT8 *debugString[6] = {"SYSTEM_BG", "SYSTEM_I", "SYSTEM_DK", "SYSTEM_L", "SYSTEM_MN",	\
							 "SYSTEM_UNKNOWN" };

	//do AAOS function if DTV is DVB mode
	if ( (g_eAadInitType == LX_AAD_INIT_DVB_SELECT)    || (g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT)	\
	   ||(g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT) || (g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT) )
	{
		//Stop AAOS(Automatic Audio Output Select)
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten, 0);
		AAD_WrFL(audio_out_ctrl);

		//Stop Stereo Carrier Search Control
		AAD_RdFL(stereo_car_search_ctrl);
		AAD_Wr01(stereo_car_search_ctrl, srchen, 0x0);
		AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x0);
		AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b000);
		AAD_WrFL(stereo_car_search_ctrl);
	}

	switch (eSoundSystem)
	{
		case LX_AAD_SIF_SYSTEM_BG:

			//For workaround, NICAM high frequency area deviation is larger than low area.
			//Set a Carrier Average Frequency Threshold High(ASD, SCS)
			//Org. threshold high is 0x18.
			AAD_RdFL(car_aver_freq_thd);
			AAD_Wr01(car_aver_freq_thd, asfth, 0x30);
			AAD_WrFL(car_aver_freq_thd);

			//4.5MHz => 0x3 - PAL Chrom
			eRetVal = AAD_SetAndStartASD ( AAD_ASD_4_5_MHZ, 0x3, B_G_L_ASD_TIMEOUT_DELAY );

			//Start Stereo Carrier Search Control(Skip A2DK1, A2DK2 and A2DK3 in BG system)
			//Syssel change automatically when a stereo carrier is detected
			AAD_RdFL(stereo_car_search_ctrl);
			AAD_Wr01(stereo_car_search_ctrl, srchen, 0x2);
			AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x2);
			AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b111);
			AAD_WrFL(stereo_car_search_ctrl);

			//Check a ASD status.
			if ( RET_OK != eRetVal )
			{
				//Set a current sound system.
				AAD_RdFL(system_select);
				AAD_Wr01(system_select, syssel, 0x1); //FM-Mono/NICAM
				AAD_WrFL(system_select);

				break;
			}

			//If any of B/G system detected, return success
			AAD_RdFL(system_select);
			AAD_Rd01(system_select, syssel, ui8SysSel);
			if ( ui8SysSel == 0x0 )
			{
				AAD_DEBUG("B/G : FM-Stereo(A2) carrier detected\n");
				eRetVal = RET_OK;
			}
			else if ( ui8SysSel == 0x1 )
			{
				AAD_DEBUG("B/G : FM-Mono/NICAM carrier detected\n");
				eRetVal = RET_OK;
			}
			break;

		case LX_AAD_SIF_SYSTEM_I:
			//4.5MHz => 0x3 - PAL Chroma
			if ( RET_OK != AAD_SetAndStartASD ( AAD_ASD_4_5_MHZ, 0x3, M_N_I_ASD_TIMEOUT_DELAY ) )
			{
				//Set a current sound system.
				AAD_RdFL(system_select);
				AAD_Wr01(system_select, syssel, 0x3); //FM-Mono/NICAM
				AAD_WrFL(system_select);

				break;
			}

			//If I system detected in system select, return success
			AAD_RdFL(system_select);
			AAD_Rd01(system_select, syssel, ui8SysSel);
			if ( ui8SysSel == 0x3 )
			{
				AAD_DEBUG("I   : FM-Mono/NICAM carrier detected\n");
				eRetVal = RET_OK;
			}
			break;

		case LX_AAD_SIF_SYSTEM_DK:

			//6.5MHz => 0x1 - D/K1, D/K2, D/K3 or D/K NICAM
			eRetVal = AAD_SetAndStartASD ( AAD_ASD_6_5_MHZ, 0x1, D_K_ASD_TIMEOUT_DELAY );

			//Start Stereo Carrier Search Control
			//Syssel change automatically when a stereo carrier is detected
			AAD_RdFL(stereo_car_search_ctrl);
			AAD_Wr01(stereo_car_search_ctrl, srchen, 0x2);
			AAD_Wr01(stereo_car_search_ctrl, chgmod, 0x2);
			AAD_Wr01(stereo_car_search_ctrl, vsphase_y_rnw, 0b000);
			AAD_WrFL(stereo_car_search_ctrl);

			//Check a ASD status.
			if ( RET_OK != eRetVal )
			{
				//Set a current sound system.
				AAD_RdFL(system_select);
				AAD_Wr01(system_select, syssel, 0x7); //FM-Mono/NICAM
				AAD_WrFL(system_select);

				break;
			}

			//If D/K system detected in system select, return success
			AAD_RdFL(system_select);
			AAD_Rd01(system_select, syssel, ui8SysSel);
			if (( ui8SysSel == 0x4 ) || ( ui8SysSel == 0x5 ) || ( ui8SysSel == 0x6 ))
			{
				AAD_DEBUG("D/K1, 2, 3 : FM-Stereo(A2) carrier detected(SysSel = %d)\n", ui8SysSel);
				eRetVal = RET_OK;
			}
			else if ( ui8SysSel == 0x7 )
			{
				AAD_DEBUG("D/K : FM-Mono/NICAM carrier detected\n");
				eRetVal = RET_OK;
			}
			break;

		case LX_AAD_SIF_SYSTEM_L:
			//6.5MHz => 0x0 - SECAM L NICAM
			if ( RET_OK != AAD_SetAndStartASD ( AAD_ASD_6_5_MHZ, 0x0, B_G_L_ASD_TIMEOUT_DELAY ) )
			{
				//Set a current sound system.
				AAD_RdFL(system_select);
				AAD_Wr01(system_select, syssel, 0x2); //AM-Mono/NICAM
				AAD_WrFL(system_select);

				break;
			}

			//If L system detected in system select, return success
			AAD_RdFL(system_select);
			AAD_Rd01(system_select, syssel, ui8SysSel);
			if ( ui8SysSel == 0x2 )
			{
				AAD_DEBUG("L   : FM-Mono/NICAM carrier detected\n");
				eRetVal = RET_OK;
			}
			break;

		case LX_AAD_SIF_SYSTEM_MN:
			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )	 //Check BTSC only
			{
				if ( RET_OK != AAD_SetAndStartASD ( AAD_ASD_4_5_MHZ, 0x0, M_N_I_ASD_TIMEOUT_DELAY ) ) //BTSC
				{
					//Set a current sound system.
					AAD_RdFL(system_select);
					AAD_Wr01(system_select, syssel, 0xA); //FM-Stereo + SAP (BTSC)
					AAD_WrFL(system_select);

					break;
				}

				//If M/N-BTSC system detected in system select, return success
				AAD_RdFL(system_select);
				AAD_Rd01(system_select, syssel, ui8SysSel);
				if ( ui8SysSel == 0xA )
				{
					AAD_DEBUG("BTSC : FM-Stereo+SAP carrier detected\n");
					eRetVal = RET_OK;
				}
			}
			else //Check for A2
			{
				if ( RET_OK != AAD_SetAndStartASD ( AAD_ASD_4_5_MHZ, 0x2, M_N_I_ASD_TIMEOUT_DELAY ) )	//A2
				{
					//Set a current sound system.
					AAD_RdFL(system_select);
					AAD_Wr01(system_select, syssel, 0x8); //FM-Stereo (A2)
					AAD_WrFL(system_select);

					break;
				}

				//If M/N-A2 system is detected in system select, return success
				AAD_RdFL(system_select);
				AAD_Rd01(system_select, syssel, ui8SysSel);
				if ( ui8SysSel == 0x8 )	//A2
				{
					eRetVal = RET_OK;
					AAD_DEBUG("M/N : FM-Stereo(A2) Carrier detected\n");
					break;
				}
			}
			break;

		case LX_AAD_SIF_SYSTEM_UNKNOWN:
			eRetVal = RET_INVALID_PARAMS;

			//May be we can implement after complete all system test
			break;

		default:
			break;
	}

	//do AAOS function if DTV is DVB mode
	if ( (g_eAadInitType == LX_AAD_INIT_DVB_SELECT)    || (g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT)	\
	   ||(g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT) || (g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT) )
	{
		//Enable AAOS(Automatic Audio Output Select).
		AAD_RdFL(audio_out_ctrl);
		AAD_Wr01(audio_out_ctrl, aouten, 1);

		//If D/K system detected in system select not in channel scan mode.
		if(bScanMode == FALSE)
		{
			//Get a audio system
			AAD_RdFL(system_select);
			AAD_Rd01(system_select, syssel, ui8SysSel);

			if (( ui8SysSel == 0x4 ) || ( ui8SysSel == 0x5 ) || ( ui8SysSel == 0x6 ))
			{
				//Disable AAOS(Automatic Audio Output Select) for in PAL D/K A2 system.
				AAD_Wr01(audio_out_ctrl, aouten,  0);

				/* Start a AAD Monitor Task for SIF D/K Mode */
				if(g_suiAadWaitQForMonitor != LX_AAD_TASK_START)
				{
					(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_START);
				}
			}
			else if ( ui8SysSel == 0x7 )
			{
				/* Start a AAD Monitor Task for SIF D/K Mode */
				if(g_suiAadWaitQForMonitor != LX_AAD_TASK_STOP)
				{
					(void)AAD_SIF_SetTaskSatus(LX_AAD_TASK_STOP);
				}
			}
		}

		//Set a AAOS Register.
		AAD_WrFL(audio_out_ctrl);
	}

	//Sanity check for debug print
	if( eSoundSystem > LX_AAD_SIF_SYSTEM_MN )
		eSoundSystem = LX_AAD_SIF_SYSTEM_MN + 1;

	AAD_RdFL(system_select);
	AAD_RdFL(status);
	AAD_PRINT ("AAD_DoASD : Sound System %d(%s)(syssel = %d, status = 0x%X)\n",	\
				eSoundSystem, debugString[eSoundSystem], AAD_Rd(system_select), AAD_Rd(status) );

	return eRetVal;
}

int AAD_SetAndStartASD ( AAD_ASD_CARRIER_T eASDCar, UINT8 ui8RegValue, UINT32	ui32Timeout )
{
	int		eRetVal = RET_ERROR;

	//Clears a system detect register.
	AAD_RdFL(auto_system_detect_ctrl);
	AAD_Wr01(auto_system_detect_ctrl, frfive, 0x0);
	AAD_Wr01(auto_system_detect_ctrl, sxfive, 0);

	if ( eASDCar == AAD_ASD_4_5_MHZ )
	{
		AAD_Wr01(auto_system_detect_ctrl, frfive, ui8RegValue);
	}
	else if ( eASDCar == AAD_ASD_6_5_MHZ )
	{
		//Set a PAL Chroma Carrier for D/K and L system
		AAD_Wr01(auto_system_detect_ctrl, frfive, 0x3);		//PAL Chroma Carrier
		AAD_Wr01(auto_system_detect_ctrl, sxfive, ui8RegValue);
	}
	else
	{
		AAD_ERROR ("AAD_SetAndStartASD : Failed(eASDCar = %d)!!!\n", eASDCar);
		return RET_ERROR;
	}

	//Sets a system detect register.
	AAD_WrFL(auto_system_detect_ctrl);

	/* Reset AAD_ASD_Task_waitq_flag value */
	AAD_ASD_Task_waitq_flag = 0;

	/* Enable interrupt */
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		//Un-mask status interrupt in status register for ASD Complete
		AAD_RdFL(interrupt_mask);
		AAD_Wr(interrupt_mask, 0x7F);
		AAD_WrFL(interrupt_mask);

		//Enable AAD interrupt(Status change output enable, Polarity : Active High)
		AAD_RdFL(status_pin_ctrl);
		AAD_Wr01(status_pin_ctrl, staten, 1);
		AAD_WrFL(status_pin_ctrl);
	}

	//Enable ASD(Automatic System Dection)
	AAD_RdFL(system_select);
	AAD_Wr01(system_select, asden, 1);
	AAD_WrFL(system_select);

	//Wait timeout to do ASD function
	eRetVal = AAD_ASD_WaitEvent_Intrruptible(ui32Timeout);

	//Read a system detect register.
	AAD_RdFL(auto_system_detect_ctrl);
	AAD_PRINT ("AAD_SetAndStartASD : System Detect Control reg val [0x%x]\n", AAD_Rd(auto_system_detect_ctrl));

	return eRetVal;
}

/**
 * Detects a PAL Audio Signal Strength After ASD fails to Scan Audio Signal.
 * @see
*/
static int AAD_DetectSoundSystem( LX_AAD_SIF_SOUNDSYSTEM_T eSoundSystem )
{
	int			iRetVal = RET_OK;

	UINT8		ui8PrmCarDet;
	UINT8		ui8Sndstat;

	UINT8 	*debugString[6] = {"BG", "I", "DK", "L", "MN", "UNKNOWN" };

	//Set a current sound system.
	AAD_RdFL(system_select);
	AAD_Wr01(system_select, asden, 0);		//Disable ASD function

	//Carrier 1 Quality AAOS Threshold High / Low Register
	//Org. threshold high is 0x22 and low is 0x16 Value
	AAD_RdFL(car1_qual_aaos_thd_hi);
	AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_DVB);
	AAD_WrFL(car1_qual_aaos_thd_hi);

	AAD_RdFL(car1_qual_aaos_thd_lo);
	AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_DVB);
	AAD_WrFL(car1_qual_aaos_thd_lo);

	//In ID(Indonesia) model, set a threshold value.
	if ( g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT )
	{
		//Carrier 1 Quality AAOS Threshold High / Low Register
		//Org. threshold high is 0x22 and low is 0x16 Value
		AAD_RdFL(car1_qual_aaos_thd_hi);
		AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_ID);
		AAD_WrFL(car1_qual_aaos_thd_hi);

		AAD_RdFL(car1_qual_aaos_thd_lo);
		AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_ID);
		AAD_WrFL(car1_qual_aaos_thd_lo);
	}

	switch ( eSoundSystem )
	{
		case LX_AAD_SIF_SYSTEM_BG:
			AAD_Wr01(system_select, syssel, 0x1); //FM-Mono/NICAM

			break;

		case LX_AAD_SIF_SYSTEM_I:
			AAD_Wr01(system_select, syssel, 0x3); //FM-Mono/NICAM
			break;

		case LX_AAD_SIF_SYSTEM_DK:
			AAD_Wr01(system_select, syssel, 0x7); //FM-Mono/NICAM

			//Carrier 1 Quality AAOS Threshold High / Low Register
			//Org. threshold high is 0x22 and low is 0x16 Value
			AAD_RdFL(car1_qual_aaos_thd_hi);
			AAD_Wr01(car1_qual_aaos_thd_hi, mn1thh, CARRIER1_QUAL_AAOS_THD_HI_DK);
			AAD_WrFL(car1_qual_aaos_thd_hi);

			AAD_RdFL(car1_qual_aaos_thd_lo);
			AAD_Wr01(car1_qual_aaos_thd_lo, mn1thl, CARRIER1_QUAL_AAOS_THD_LO_DK);
			AAD_WrFL(car1_qual_aaos_thd_lo);
			break;

		case LX_AAD_SIF_SYSTEM_L:
			AAD_Wr01(system_select, syssel, 0x2); //AM-Mono/NICAM
			break;

		case LX_AAD_SIF_SYSTEM_MN:
			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
			{
				AAD_Wr01(system_select, syssel, 0xA); //FM-Stereo + SAP (BTSC)
			}
			else
			{
				AAD_Wr01(system_select, syssel, 0x8); //FM-Stereo (A2)
			}
			break;

		case LX_AAD_SIF_SYSTEM_UNKNOWN:
			iRetVal = RET_INVALID_PARAMS;
			break;

		default:
			iRetVal = RET_INVALID_PARAMS;
			break;
	}

	//Set a current sound system.
	AAD_WrFL(system_select);

	//Wait to read signal strength
	AAD_SIF_TIME_DELAY_MS (SIGNAL_QUALITY_DELAY_MANUAL);

	//Read status register
	AAD_RdFL(status);
	AAD_Rd01(status, prmcardet, ui8PrmCarDet);
	AAD_Rd01(status, sndstat,   ui8Sndstat);
	AAD_DEBUG("Primary Carrier   %d(status = 0x%X)\n", ui8PrmCarDet, AAD_Rd(status) );

	//Sanity check for debug print
	if( eSoundSystem > LX_AAD_SIF_SYSTEM_MN )
		eSoundSystem = LX_AAD_SIF_SYSTEM_MN + 1;

	AAD_DEBUG_TMP ("AAD_SIF_SetBandSetup : Sound System %d(%s)(syssel = %d, status = 0x%2X)\n", \
					eSoundSystem, debugString[eSoundSystem], AAD_Rd(system_select), AAD_Rd(status));

	//Check a Primary Carrier Status to detect PAL B/G, I, D/K, M/N Signal
	if ( ui8PrmCarDet == 1 )
	{
		AAD_WARN("Primary Carrier Detected : %s\n", debugString[eSoundSystem]);
	}
	else if ( ui8Sndstat == 0x2 )	//NICAM(digital sound) is available.
	{
		AAD_WARN("NICAM Signal Detected : %s\n", debugString[eSoundSystem]);
	}
	else
	{
		AAD_WARN("No Primary Carrier & NICAM Signal : %s\n", debugString[eSoundSystem]);

		//Wait to read signal strength
		AAD_SIF_TIME_DELAY_MS (SIGNAL_QUALITY_DELAY_MANUAL);
	}

	//Set a RET_OK to detect Carrier1 Quality Value.
	iRetVal = RET_OK;

	return iRetVal;
}

/**
 * Wait for ASD to complete.
 * @see AAD_ASD_WakeUp_Interruptible
*/
int AAD_ASD_WaitEvent_Intrruptible ( UINT32	ui32Timeout )
{
	UINT8	ui8AsdEn;

	UINT64	start, elapsed;

	AAD_DEBUG_TMP ("Wait %d ms for ASD to complete!!!\n", ui32Timeout);

	//Get a current mili-second time
	start = jiffies;

	//Change note for ui32Timeout * 2 : need more time than spec.
	wait_event_interruptible_timeout(AAD_ASD_Task_wait, AAD_ASD_Task_waitq_flag != 0, msecs_to_jiffies(ui32Timeout * 2));

	//Get a elapsed mili-second time
	elapsed = (UINT32)jiffies_to_msecs(jiffies - start);

	//Read a status register
	AAD_RdFL(status);

	if(AAD_ASD_Task_waitq_flag)
	{
		AAD_DEBUG("ASD is completed in %d ms.(0x%X)\n", (UINT32)elapsed, AAD_Rd(status));
	}
	else
	{
		AAD_DEBUG("ASD is timeout in %d ms.(0x%X)\n", (UINT32)elapsed, AAD_Rd(status));
	}

	//Read a system select registers.
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, asden, ui8AsdEn);

	//Check ASD status
	if (ui8AsdEn == 0)
	{
		return RET_OK;
	}
	else
	{
		//Disable ASD function
		AAD_Wr01(system_select, asden, 0);
		AAD_WrFL(system_select);

		return RET_ERROR;
	}
}

/**
 * Signal ASD is completed.
 * @see AAD_ASD_WaitEvent_Intrruptible
*/
void AAD_ASD_WakeUp_Interruptible ( void )
{
	AAD_ASD_Task_waitq_flag = 1;

	wake_up_interruptible(&AAD_ASD_Task_wait);

	return;
}

/**
 * Check a audio carrier and status detection.
 * @see AAD_DoASD
*/
int AAD_ASD_CheckCarrierDetection ( void )
{
	UINT32		ui8PrmCarDet, ui8SecCarDet, ui8MonoSt, ui8IMono, ui8BiSap;

	UINT8		value;
	UINT8 		ui8StdSel;

	UINT16		ui16Carrier1Magnitude = 0;
	UINT16	  	ui16Carrier2Magnitude = 0;
	UINT16		ui16Carrier1Quality = 0;
	UINT16	  	ui16Carrier2Quality = 0;

	UINT8		ui8NicamCtrlBits;
	UINT16		ui16NicamErrorRate;

	//Read status register
	AAD_RdFL(status);
	AAD_Rd01(status, prmcardet, ui8PrmCarDet);
	AAD_Rd01(status, seccardet, ui8SecCarDet);
	AAD_Rd01(status, mono_str,  ui8MonoSt);
	AAD_Rd01(status, imono,     ui8IMono);
	AAD_Rd01(status, bisap,	    ui8BiSap);

	if ((ui8PrmCarDet == 1) && (ui8SecCarDet == 1))
	{
		AAD_DEBUG("Primary & Secondary Carrier Detected\n");
	}
	else if (ui8PrmCarDet == 1)
	{
		AAD_DEBUG("Primary Carrier Detected\n");
	}
	else if (ui8SecCarDet == 1)
	{
		AAD_DEBUG("Secondary Carrier Detected\n");
	}

	if (ui8MonoSt == 1)
	{
		AAD_DEBUG("Stereo Bit Detected\n");
	}

	if (ui8IMono == 1)
	{
		AAD_DEBUG("NICAM Bit Detected\n");
	}

	if (ui8BiSap == 1)
	{
		AAD_DEBUG("BI-SAP Bit Detected\n");
	}

	AAD_RdFL(car1_aver_mag0);
	AAD_Rd01(car1_aver_mag0, cr1mg, value);
	ui16Carrier1Magnitude  = value << 8;

	AAD_RdFL(car1_aver_mag1);
	AAD_Rd01(car1_aver_mag1, cr1mg, value);
	ui16Carrier1Magnitude += value;
	AAD_DEBUG("Carrier1 Magnitude Current 0x%4X\n", ui16Carrier1Magnitude );

	AAD_RdFL(car2_aver_mag0);
	AAD_Rd01(car2_aver_mag0, cr2mg, value);
	ui16Carrier2Magnitude  = value << 8;

	AAD_RdFL(car2_aver_mag1);
	AAD_Rd01(car2_aver_mag1, cr2mg, value);
	ui16Carrier2Magnitude += value;
	AAD_DEBUG("Carrier2 Magnitude Current 0x%4X\n", ui16Carrier2Magnitude );

	AAD_RdFL(car1_fm_qual0);
	AAD_Rd01(car1_fm_qual0, cr1ql, value);
	ui16Carrier1Quality  = value << 8;

	AAD_RdFL(car1_fm_qual1);
	AAD_Rd01(car1_fm_qual1, cr1ql, value);
	ui16Carrier1Quality += value;
	AAD_DEBUG("Carrier1 Quality Current   0x%4X\n", ui16Carrier1Quality );

	AAD_RdFL(car2_fm_qual0);
	AAD_Rd01(car2_fm_qual0, cr2ql, value);
	ui16Carrier2Quality  = value << 8;

	AAD_RdFL(car2_fm_qual1);
	AAD_Rd01(car2_fm_qual1, cr2ql, value);
	ui16Carrier2Quality += value;
	AAD_DEBUG("Carrier2 Quality Current   0x%4X\n", ui16Carrier2Quality );

	AAD_RdFL(stereo_id_mag);
	AAD_RdFL(biling_id_mag);
	AAD_DEBUG("Stereo ID Mag. Current     0x%4X\n", AAD_Rd(stereo_id_mag));
	AAD_DEBUG("Biling ID Mag. Current     0x%4X\n", AAD_Rd(biling_id_mag));

	//Print global variables
	AAD_DEBUG("carrier2Magitude    = 0x%4X\n", g_stThresholdLevel.carrier2Magitude);
	AAD_DEBUG("carrier2QualityHigh = 0x%4X, Low = 0x%4X\n",	\
			   g_stThresholdLevel.carrier2QualityHigh, g_stThresholdLevel.carrier2QualityLow);
	AAD_DEBUG("stereoIDMagHigh     = 0x%4X, Low = 0x%4X\n",	\
			   g_stThresholdLevel.stereoIDMagHigh, g_stThresholdLevel.stereoIDMagLow);

	//Carrier 1 Quality AAOS Threshold High / Low Register
	AAD_RdFL(car1_qual_aaos_thd_hi);
	AAD_RdFL(car1_qual_aaos_thd_lo);
	AAD_DEBUG("Carrier1 Quality Threshold 0x%4X(0x%4X)\n",	\
			   AAD_Rd(car1_qual_aaos_thd_hi) << 3, AAD_Rd(car1_qual_aaos_thd_lo) << 3 );

	//Carrier 2 Quality AAOS Threshold High / Low Register
	AAD_RdFL(car2_qual_aaos_thd_hi);
	AAD_RdFL(car2_qual_aaos_thd_lo);
	AAD_DEBUG("Carrier2 Quality Threshold 0x%4X(0x%4X)\n",	\
			   AAD_Rd(car2_qual_aaos_thd_hi) << 3, AAD_Rd(car2_qual_aaos_thd_lo) << 3 );

	//ID Magnitude Threshold High / Low Register
	AAD_RdFL(id_mag_thd_hi);
	AAD_RdFL(id_mag_thd_lo);
	AAD_DEBUG("St/Bil Magnitude Threshold 0x%4X(0x%4X)\n",	\
			   AAD_Rd(id_mag_thd_hi), AAD_Rd(id_mag_thd_lo) );

	//Read a standard select  register
	//Read Standard Select reg.
	AAD_RdFL(standard_select);
	AAD_Rd01(standard_select, stndrd, ui8StdSel);

	if ( ui8StdSel == 0x7 )
	{
		AAD_RdFL(nicam_err0);
		AAD_Rd01(nicam_err0, nicerr, value);
		ui16NicamErrorRate  = value << 8;

		AAD_RdFL(nicam_err1);
		AAD_Rd01(nicam_err1, nicerr, value);
		ui16NicamErrorRate += value;
		AAD_DEBUG("NICAM Error Rate Current   0x%04X\n", ui16NicamErrorRate );

		//NICAM Error Rate AAOS Threshold High / Low Register
		AAD_RdFL(nicam_err_aaos_thd_hi);
		AAD_RdFL(nicam_err_aaos_thd_lo);
		AAD_DEBUG("NICAM Error Rate AAOS Threshold 0x%04X(0x%04X)\n",	\
				   AAD_Rd(nicam_err_aaos_thd_hi) << 7, AAD_Rd(nicam_err_aaos_thd_lo) << 7 );

		//Just read the high byte so that low byte will be updated
		AAD_RdFL(nicam_ctrl_bits0);
		AAD_RdFL(nicam_ctrl_bits1);
		AAD_Rd01(nicam_ctrl_bits1, nicctl, ui8NicamCtrlBits);

		ui8NicamCtrlBits = (ui8NicamCtrlBits & 0xE) >> 1;
		AAD_DEBUG("NICAM Control bits 0x%x\n", ui8NicamCtrlBits);
	}

	return RET_OK;
}

/**
 * Get dectected Sound System.
 * @see AAD_SIF_GetBandDetect
*/
LX_AAD_SIF_SOUNDSYSTEM_T AAD_GetSoundSystem (void)
{
	UINT8	ui8SysSel;

	LX_AAD_SIF_SOUNDSYSTEM_T	eTVSystem = LX_AAD_SIF_SYSTEM_UNKNOWN;

	//ASD completed, use system select register to find detected sound system
	//Check a current sound sysytem is detected.
	AAD_RdFL(system_select);
	AAD_Rd01(system_select, syssel, ui8SysSel);

	switch (ui8SysSel)
	{
		case 0x0:
		case 0x1:
			eTVSystem = LX_AAD_SIF_SYSTEM_BG;
			break;

		case 0x2:
			eTVSystem = LX_AAD_SIF_SYSTEM_L;
			break;

		case 0x3:
			eTVSystem = LX_AAD_SIF_SYSTEM_I;
			break;

		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
			eTVSystem = LX_AAD_SIF_SYSTEM_DK;
			break;

		case 0x8:
		case 0x9:
			eTVSystem = LX_AAD_SIF_SYSTEM_MN;
			break;

		case 0xA:
			if ( g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_SELECT || g_eAadInitType == LX_AAD_INIT_ATSC_BTSC_US_SELECT )
			{
				eTVSystem = LX_AAD_SIF_SYSTEM_MN;
			}
			else
			{
				eTVSystem = LX_AAD_SIF_SYSTEM_UNKNOWN; //FM-Radio
			}
			break;

		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
		default:
			eTVSystem = LX_AAD_SIF_SYSTEM_UNKNOWN; //FM-Radio
			break;
	}

	return eTVSystem;
}

/**
 * Set a manual volume by aad output mode for High Devication mode.
 * @see
*/
int AAD_SetManualVolumeForHDEVMode ( LX_ATV_AUDIO_MODE_SET_T eAudioModeSet )
{
	BOOLEAN 	bNicamMode	 = FALSE;
	BOOLEAN 	bModeChanged = FALSE;

	//Read manual_output_level_left_ch reg.
	AAD_RdFL(manual_out_lvl_left_ch);

	//Read manual_output_level_right_ch reg.
	AAD_RdFL(manual_out_lvl_right_ch);

	//Read I2S Gain Control reg.
	AAD_RdFL(i2s_ctrl);

	//Set a NICAM mode to set a volume for High Devication mode.
	switch (eAudioModeSet)
	{
		case LX_ATV_SET_PAL_NICAM_MONO:
		case LX_ATV_SET_PAL_NICAM_STEREO:
		case LX_ATV_SET_PAL_NICAM_DUALI:
		case LX_ATV_SET_PAL_NICAM_DUALII:
		case LX_ATV_SET_PAL_NICAM_DUALI_II:		//app. calls this mode in UI DUAL I+II osd.
			bNicamMode = TRUE;

			//Set a 0-bit shitf for I2S volume for a left, right volume
			AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x0); // 0-bit shift
			break;

		default:
			bNicamMode = FALSE;
			break;
	}

#if 0
	----------------------------------------------------------
	Band Width	        50K	    100K	200K	384K	540K
	----------------------------------------------------------
	출력 gain	        47186	23953	11797	6144	4096
	출력 gain 비율	    1	    0.508	0.25	0.13	0.087
	----------------------------------------------------------
	크기 비교 50k 기준	1	    1.97	4	    7.68	11.52
	크기 비교 100k 기준	0.51	1	    2.03	3.9	    5.85
	크기 비교 200k 기준	0.25	0.49	1	    1.92	2.88
	크기 비교 384k 기준	0.13	0.26	0.52	1	    1.5
	크기 비교 540k 기준	0.09	0.17	0.35	0.67	1
	----------------------------------------------------------
	/* Change High Deviation Level Value */
	if(RMM_GROUP_IS_KR)
	{
		aadVolGain = ADEC_DSP_VOLUME_GAIN_0DB *  2;	//DSP volume : * 2,  200/100Khz
	}
	else if(RMM_GROUP_IS_AJJA_ALL)
	{
		aadVolGain = ADEC_DSP_VOLUME_GAIN_4_9DB;	//DSP volume : * 1.75,  540/100Khz, 2012.03.29
	}
	else if(RMM_GROUP_IS_DVB)
	{
		aadVolGain = ADEC_DSP_VOLUME_GAIN_0DB *  4;	//DSP volume : * 11,  384/100Khz
	}
	else
	{
		aadVolGain = ADEC_DSP_VOLUME_GAIN_0DB *  2;	//DSP volume : * 11,  100/50Khz
	}
#endif

	//Check a High Deviation Mode in no NICAM mode : 100 -> 200Khz
	if ( g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT )
	{
		if( g_eHighDevMode.bHighDevOnOff == TRUE )
		{
			//Set a 1-bit shitf for I2S volume for a left, right volume
			AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift

			//Set a Mode Changed.
			bModeChanged = TRUE;
		}
	}
	//Need to set in Indonesia level problem
	//Need to set in SECAM L level problem
	//Set a volume level by audio sound modulation
	else if ( (g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT ) || (g_i_eSystem == LX_AAD_SIF_SYSTEM_L) )
	{
		//Set a Mode Changed.
		bModeChanged = TRUE;

		//Check a NICAM mode to boost audio level.
		if(bNicamMode == TRUE)
		{
			//Set  a normal level for a left, right volume
			AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x80);
			AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x80);
		}
		else
		{
			//Set  a 2 times level for a left, right volume
			//정밀측정 NG 항목인 deviation 250% max distortion, volume curve 특성(2013.02.07)
			AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xC0);
			AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xC0);

			//Check a High Deviation Mode in no NICAM mode : 100 -> 540Khz
			if(g_eHighDevMode.bHighDevOnOff == TRUE)
			{
				if(g_eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_4)
				{
					// HDEV option : On, 4.9dB Boost
					// HDEV On시 aad volume gain 값을 E1 ->  8D 로 변경하면 10.13 W로 출력(2013.02.19)
					//Set  a 0dB level for a left, right  volume
					AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x8D);
					AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x8D);

					//Set  a 1-bit shift for I2S volume for a left, right  volume
					AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
				}
				else if(g_eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_3)
				{
					//Set  a 2-bit shift for I2S volume for a left, right  volume
					AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x2); // 2-bit shift
				}
				else
				{
					//Set  a 1-bit shift for I2S volume for a left, right  volume
					AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
				}
			}
			else
			{
				//Check a chip revision
				if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
				{
					//SECAM L SIF Level Adjust : H13, soohong.jang(2013.12.06)
					//AGC Gain Control Register, SECAM L(AM) Volume Level Issue(2014.02.11)
					if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L)
					{
						AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xB7);
						AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xB7);

						//Set  a 1-bit shift for I2S volume for a left, right  volume
						AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
					}
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					;
				}
				else if(lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					//SECAM L SIF Level Adjust : M14, ileun.kwak(2013.11.09)
					if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L)
					{
						/* Check a SIF Internal Source. */
						if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
						{
							AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xA3);
							AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xA3);

							//Set  a 1-bit shift for I2S volume for a left, right  volume
							AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
						}
						else
						{
							AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xAD);
							AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xAD);

							//Set  a 1-bit shift for I2S volume for a left, right  volume
							AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
						}
					}
				}
				else 	//if(lx_chip_rev() >= LX_CHIP_REV(H13, B0))
				{
					//SECAM L SIF Level Adjust : H13, soohong.jang(2013.12.06)
					//AGC Gain Control Register, SECAM L(AM) Volume Level Issue(2014.02.11)
					if (g_i_eSystem == LX_AAD_SIF_SYSTEM_L)
					{
						AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xB7);
						AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xB7);

						//Set  a 1-bit shift for I2S volume for a left, right  volume
						AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
					}
				}
			}
		}
	}
	else if ( (g_eAadInitType == LX_AAD_INIT_DVB_SELECT) || (g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT)	\
			||(g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT) )
	{
		//Set a Mode Changed.
		bModeChanged = TRUE;

		//Set  a 2 times for a left, right  volume
		//정밀측정 NG 항목인 deviation 250% max distortion, volume curve 특성(2013.02.07)
		AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xC0);
		AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xC0);

		//Check a High Deviation Mode in no NICAM mode
		if(g_eHighDevMode.bHighDevOnOff == TRUE &&  bNicamMode == FALSE)
		{
			//Check a High Deviation Mode in no NICAM mode : 100 -> 540Khz
			if(g_eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_4)
			{
				// HDEV option : On, 4.9dB Boost
				// HDEV On시 aad volume gain 값을 E1 ->  8D 로 변경하면 10.13 W로 출력(2013.02.19)
				//Set  a 0dB level for a left, right  volume
				AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x8D);
				AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x8D);

				//Set  a 1-bit shift for I2S volume for a left, right  volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
			}
			//Check a High Deviation Mode in no NICAM mode : 100 -> 384Khz
			else if(g_eHighDevMode.highDevLevel == LX_AAD_HIGH_DEV_LEVEL_3)
			{
				//Set  a 1-bit shift for I2S volume for a left, right  volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 2-bit shift
			}
			else
			{
				//Set  a 1-bit shift for I2S volume for a left, right  volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
			}
		}
	}
	else if ( g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT )
	{
		//Set a Mode Changed.
		bModeChanged = TRUE;

		//Check a High Deviation Mode in no NICAM mode : 100 -> 540Khz
		if(g_eHighDevMode.bHighDevOnOff == TRUE &&  bNicamMode == FALSE)
		{
			// HDEV option : On, 4.9dB Boost
			// HDEV On시 aad volume gain 값을 E1 ->  8D 로 변경하면 10.13 W로 출력(2013.02.19)
			//Set  a 0dB level for a left, right  volume
			AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x8D);
			AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x8D);

			//Set  a 1-bit shift for I2S volume for a left, right  volume
			AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
		}
		else
		{
			//Set  a -6dB(allow the decoder to robustly decode 200% deviation in allmoded) level for a left, right  volume
			//정밀측정 NG 항목인 deviation 250% max distortion, volume curve 특성(2013.02.07)
			AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0xC0); // HDEV option : Off
			AAD_Wr01(manual_out_lvl_right_ch, levrt,  0xC0);

			/* Check LX Chip Revision Number */
			if (lx_chip() != LX_CHIP_H13 && g_bHighDevOnOff == TRUE)
			{
				//Set Volume for High Deviation Off 50Khz(2014.03.30)
				AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x17); // HDEV option : Off
				AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x17); // suji.baek@lge.com(0x28 -> 0x17), 2014.11.11

				//Set a 0-bit shitf for I2S volume for a left, right volume
				AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x0); // 0-bit shift
			}
		}
	}
	else
	{
		//Check a High Deviation Mode in no NICAM mode : 50 -> 100Khz
		if(g_eHighDevMode.bHighDevOnOff == TRUE)
		{
			//Set a Mode Changed.
			bModeChanged = TRUE;

			//Set  a 1-bit shift for I2S volume for a left, right  volume
			AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x1); // 1-bit shift
		}
	}

	//Check a mode changed.
	if(bModeChanged == TRUE)
	{
		//Write manual_output_level_left_ch reg.
		AAD_WrFL(manual_out_lvl_left_ch);

		//Write manual_output_level_right_ch reg.
		AAD_WrFL(manual_out_lvl_right_ch);

		//Write I2S Gain Control reg.
		AAD_WrFL(i2s_ctrl);

		AAD_DEBUG("AAD_SetManualVolume : eAudioModeSet = 0x%X(i2s = 0x%X, level = 0x%X)\n", eAudioModeSet, AAD_Rd(i2s_ctrl), AAD_Rd(manual_out_lvl_left_ch));
	}
	else if(bNicamMode == TRUE)
	{
		//Write I2S Gain Control reg.
		AAD_WrFL(i2s_ctrl);

		AAD_DEBUG("AAD_SetManualVolume(NICAM) : eAudioModeSet = 0x%X(i2s = 0x%X, level = 0x%X)\n", eAudioModeSet, AAD_Rd(i2s_ctrl), AAD_Rd(manual_out_lvl_left_ch));
	}

	AAD_PRINT("AAD_SetManualVolume : eAudioModeSet = 0x%X\n", eAudioModeSet );
	return RET_OK;
}

/**
 * Get a SECAM L Manual Volume For Adjust after check SIF Magnitude.
 * @see
*/
static int AAD_SECAM_L_GetManualVolumeForAdjust ( void )
{
	int		iRetVal = RET_OK;

	UINT8	value;
	UINT8	ui8OutputStep 	= 0x00;
	UINT8	ui8OutputVolume = 0xA3;

	UINT16	ui16Carrier1Magnitude = 0;

	//Read manual_output_level_left_ch reg.
	AAD_RdFL(manual_out_lvl_left_ch);

	//Read manual_output_level_right_ch reg.
	AAD_RdFL(manual_out_lvl_right_ch);

	//Read car1_aver_mag0 reg.
	AAD_RdFL(car1_aver_mag0);
	AAD_Rd01(car1_aver_mag0, cr1mg, value);
	ui16Carrier1Magnitude  = value << 8;

	//Read car1_aver_mag1 reg.
	AAD_RdFL(car1_aver_mag1);
	AAD_Rd01(car1_aver_mag1, cr1mg, value);
	ui16Carrier1Magnitude += value;
	AAD_DEBUG("Carrier1 Magnitude Current 0x%4X\n", ui16Carrier1Magnitude );

	/* Check a SIF Magnitude. */
	if( ui16Carrier1Magnitude < g_ui32AadSECAMVolume[0] )
		ui8OutputStep = 0;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[0] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[1] )
		ui8OutputStep = 10;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[1] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[2] )
		ui8OutputStep = 9;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[2] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[3] )
		ui8OutputStep = 8;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[3] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[4] )
		ui8OutputStep = 7;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[4] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[5] )
		ui8OutputStep = 6;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[5] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[6] )
		ui8OutputStep = 5;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[6] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[7] )
		ui8OutputStep = 4;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[7] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[8] )
		ui8OutputStep = 3;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[8] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[9] )
		ui8OutputStep = 2;
	else if( ui16Carrier1Magnitude >= g_ui32AadSECAMVolume[9] && ui16Carrier1Magnitude < g_ui32AadSECAMVolume[10] )
		ui8OutputStep = 1;
	else
		ui8OutputStep = 0;

	/* Check a SIF Internal Source. */
	if (g_i_eSifSource == LX_AAD_SIF_INPUT_INTERNAL)
	{
		/* Compute output gain. */
		ui8OutputVolume = 0xA3 + (g_ui32AadSECAMWeight * ui8OutputStep);
	}
	else
	{
		/* Compute output gain. */
		ui8OutputVolume = 0xAD + (g_ui32AadSECAMWeight * ui8OutputStep);
	}

	/* Check a previous volume. */
	if (AAD_Rd(manual_out_lvl_left_ch) != ui8OutputVolume && g_ui32AadSECAMDisable == FALSE)
	{
		AAD_Wr01(manual_out_lvl_left_ch,  levlft, ui8OutputVolume);
		AAD_Wr01(manual_out_lvl_right_ch, levrt,  ui8OutputVolume);

		//Write manual_output_level_left_ch reg.
		AAD_WrFL(manual_out_lvl_left_ch);

		//Write manual_output_level_right_ch reg.
		AAD_WrFL(manual_out_lvl_right_ch);

		AAD_DEBUG("GetManualVolumeForAdjust : Magnitude 0x%4X, Weight %d, Step %d, Volume = 0x%X\n",	\
				   ui16Carrier1Magnitude, g_ui32AadSECAMWeight, ui8OutputStep, ui8OutputVolume);
	}

	return iRetVal;
}

/**
 * Get a SECAM L Manual Volume For Adjust after check SIF Magnitude for H1x(External A-Demod).
 * @see
*/
static int AAD_SECAM_L_SetManualVolumeForAdjust_H1x ( void )
{
	int 	iRetVal = RET_OK;

	UINT8	value;
	UINT8	ui8OutputVolume = 0xB7;

	UINT16	ui16Carrier1Magnitude = 0;

	UINT32	ui32AadSECAMDisable_H1x = FALSE;

	UINT32	ui32AadSECAMVolume_H1x[NUMBER_OF_SECAM_L_MAGNITUDE_H1x] = {0, };

	//Read manual_output_level_left_ch reg.
	AAD_RdFL(manual_out_lvl_left_ch);

	//Read manual_output_level_right_ch reg.
	AAD_RdFL(manual_out_lvl_right_ch);

	//Read car1_aver_mag0 reg.
	AAD_RdFL(car1_aver_mag0);
	AAD_Rd01(car1_aver_mag0, cr1mg, value);
	ui16Carrier1Magnitude  = value << 8;

	//Read car1_aver_mag1 reg.
	AAD_RdFL(car1_aver_mag1);
	AAD_Rd01(car1_aver_mag1, cr1mg, value);
	ui16Carrier1Magnitude += value;
	AAD_DEBUG("Carrier1 Magnitude Current 0x%4X\n", ui16Carrier1Magnitude );

	/* Check LX Chip Revision Number */
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		/* Copy local variable. */
		(void)memcpy((void *)&(ui32AadSECAMVolume_H1x[0]), (void *)&(g_ui32AadSECAMVolume_H15[0]), sizeof(g_ui32AadSECAMVolume_H15));
		ui32AadSECAMDisable_H1x = g_ui32AadSECAMDisable_H15;
	}
	else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		/* Copy local variable. */
		(void)memcpy((void *)&(ui32AadSECAMVolume_H1x[0]), (void *)&(g_ui32AadSECAMVolume_H13[0]), sizeof(g_ui32AadSECAMVolume_H13));
		ui32AadSECAMDisable_H1x = g_ui32AadSECAMDisable_H13;
	}

	/* Check a SIF Magnitude. */
	if( ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[0] )
		ui8OutputVolume = 0xB7;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[0] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[1] )
		ui8OutputVolume = 0xB8;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[1] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[2] )
		ui8OutputVolume = 0xB5;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[2] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[3] )
		ui8OutputVolume = 0xB2;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[3] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[4] )
		ui8OutputVolume = 0xAF;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[4] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[5] )
		ui8OutputVolume = 0xAC;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[5] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[6] )
		ui8OutputVolume = 0xAA;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[6] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[7] )
		ui8OutputVolume = 0xA8;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[7] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[8] )
		ui8OutputVolume = 0xA6;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[8] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[9] )
		ui8OutputVolume = 0xA4;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[9] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[10] )
		ui8OutputVolume = 0xA2;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[10] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[11] )
		ui8OutputVolume = 0xA1;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[11] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[12] )
		ui8OutputVolume = 0xA0;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[12] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[13] )
		ui8OutputVolume = 0x9F;
	else if( ui16Carrier1Magnitude >= ui32AadSECAMVolume_H1x[13] && ui16Carrier1Magnitude < ui32AadSECAMVolume_H1x[14] )
		ui8OutputVolume = 0x9E;
	else
		ui8OutputVolume = 0x9D;

	/* Check a previous volume. */
	if (AAD_Rd(manual_out_lvl_left_ch) != ui8OutputVolume && ui32AadSECAMDisable_H1x == FALSE)
	{
		AAD_Wr01(manual_out_lvl_left_ch,  levlft, ui8OutputVolume);
		AAD_Wr01(manual_out_lvl_right_ch, levrt,  ui8OutputVolume);

		//Write manual_output_level_left_ch reg.
		AAD_WrFL(manual_out_lvl_left_ch);

		//Write manual_output_level_right_ch reg.
		AAD_WrFL(manual_out_lvl_right_ch);

		AAD_DEBUG("SetManualVolumeForAdjust_H1x : Magnitude 0x%4X, Volume = 0x%X\n",	\
				   ui16Carrier1Magnitude, ui8OutputVolume);
	}

	return iRetVal;
}

/**
 * Set a SECAM L NICAM Volume For Restore.
 * @see
*/
static int AAD_SECAM_L_SetNICAMVolume ( void )
{
	int		iRetVal = RET_OK;

	//Read manual_output_level_left_ch reg.
	AAD_RdFL(manual_out_lvl_left_ch);

	//Read manual_output_level_right_ch reg.
	AAD_RdFL(manual_out_lvl_right_ch);

	//Read I2S Gain Control reg.
	AAD_RdFL(i2s_ctrl);

	//Set  a normal level for a left, right volume
	AAD_Wr01(manual_out_lvl_left_ch,  levlft, 0x80);
	AAD_Wr01(manual_out_lvl_right_ch, levrt,  0x80);

	//Set a 0-bit shitf for I2S volume for a left, right volume
	AAD_Wr01(i2s_ctrl, i2s_gain_01, 0x0); // 0-bit shift

	//Write manual_output_level_left_ch reg.
	AAD_WrFL(manual_out_lvl_left_ch);

	//Write manual_output_level_right_ch reg.
	AAD_WrFL(manual_out_lvl_right_ch);

	//Write I2S Gain Control reg.
	AAD_WrFL(i2s_ctrl);

	AAD_DEBUG("AAD_SECAM_L_SetNICAMVolume(NICAM) : i2s = 0x%X, level = 0x%X\n", AAD_Rd(i2s_ctrl), AAD_Rd(manual_out_lvl_left_ch));

	return iRetVal;
}

/**
 * Routine for monitor task to check mono / stereo detection.
 * @see
*/
int AAD_Monitor_Task( void *i_pvParam )
{
	UINT64	ui64TaskTimeout = AAD_MONOTOR_TASK_NO_TIMEOUT;

	AAD_PRINT( "AAD> Task is created!\n" );

	do
	{
		/* Check stop condition when device is closed. */
		if( kthread_should_stop())
		{
			AAD_NOTI( "AAD> Task - exit!\n" );
			break;
		}

		/* Reset a wait queue monitor variable. */
		g_suiAadWaitQForMonitor = LX_AAD_TASK_NONE_MSG;

		wait_event_interruptible_timeout( Aad_Monitor_WaitQ,	\
										  g_suiAadWaitQForMonitor != LX_AAD_TASK_NONE_MSG,	\
										  msecs_to_jiffies(ui64TaskTimeout) );		//50 ms

		//Set a Monitor task : Start and Stop timeout
		if(g_suiAadWaitQForMonitor == LX_AAD_TASK_START)
		{
			ui64TaskTimeout = AAD_MONOTOR_TASK_TIMEOUT;		//50ms
		}
		else if(g_suiAadWaitQForMonitor == LX_AAD_TASK_STOP)
		{
			ui64TaskTimeout = AAD_MONOTOR_TASK_NO_TIMEOUT;
		}

		AAD_NOTI("AAD_Monitor_Task(msg = %d, timeout = %d)\n", g_suiAadWaitQForMonitor, (UINT32)ui64TaskTimeout);

		//Check a AAD monitor task for KOREA A2 Signal
		if ( ((g_eAadInitType == LX_AAD_INIT_ATSC_A2_SELECT) \
		    ||(g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT && g_i_eSystem == LX_AAD_SIF_SYSTEM_MN) )
		   &&(g_bAadSuspended == FALSE) )
		{
			OS_LockMutex(&g_AAD_Sema);

			#ifdef	ENABLE_AAD_TEST_MODE	//test only
			#else

			//Check a forced ST / BI status
			(void)AAD_NTSC_MN_A2_Monitor_Task();

			#endif	//#ifdef	ENABLE_AAD_TEST_MODE	//test only

			OS_UnlockMutex(&g_AAD_Sema);
		}
		//Check a AAD monitor task for PAL B India A2 Signal Status
		else if ( (g_eAadInitType == LX_AAD_INIT_DVB_IN_SELECT && lx_chip() != LX_CHIP_H13 && g_bHighDevOnOff == TRUE) \
				&&(g_bAadSuspended == FALSE) )
		{
			//Do wait after channel change.
			if(g_ui8HighDevEabled == HIGH_DEV_UNKNOWN_STATUS)
			{
				//Wait to read signal strength
				AAD_SIF_TIME_DELAY_MS (AAD_MONOTOR_TASK_PAL_D_IN);	//300ms
			}

			OS_LockMutex(&g_AAD_Sema);

			//Check a PAL B  status
			(void)AAD_PAL_B_A2_India_Monitor_Task();

			OS_UnlockMutex(&g_AAD_Sema);
		}
		else if ( ((g_eAadInitType == LX_AAD_INIT_DVB_SELECT)		\
				 ||(g_eAadInitType == LX_AAD_INIT_DVB_AJJA_SELECT)	\
				 ||(g_eAadInitType == LX_AAD_INIT_DVB_CN_SELECT)	\
				 ||(g_eAadInitType == LX_AAD_INIT_DVB_ID_SELECT))	\
				 &&(g_bAadSuspended == FALSE) )
		{
			//Enable AAOS(Automatic Audio Output Select) if audio system is not PAL D/K system.
			if(g_i_eSystem == LX_AAD_SIF_SYSTEM_DK)
			{
				OS_LockMutex(&g_AAD_Sema);

				//Increase a g_ui32AadTaskRunningCount
				g_ui32AadTaskRunningCount++;

				//Check a 1 second running after AAD task starts.
				if(g_ui32AadTaskRunningCount == AAD_MONOTOR_TASK_1SECOND_RUN)
				{
					//Disable AAOS(Automatic Audio Output Select) if audio mode is Mono in PAL D/K system.
					if(g_i_eAudioModeSet == LX_ATV_SET_PAL_MONO)
					{
						//Read audio output control  reg.
						AAD_RdFL(audio_out_ctrl);

						//Disable AAOS(Automatic Audio Output Select) => Move to AAD Task to set after 1 second.
						AAD_Wr01(audio_out_ctrl, aouten, 0);

						//Write audio output control  reg.
						AAD_WrFL(audio_out_ctrl);

						AAD_DEBUG("AAD_Monitor_Task(PAL DK) : g_i_eAudioModeSet = 0x%X(output = 0x%X)\n", g_i_eAudioModeSet, AAD_Rd(audio_out_ctrl));
					}
				}

				//Check a forced DK Mono status
				(void)AAD_PAL_DK_A2_Monitor_Task();

				OS_UnlockMutex(&g_AAD_Sema);
			}
			//Check a SIF Magnitude if audio system is not PAL SECAM L system.
			else if(g_i_eSystem == LX_AAD_SIF_SYSTEM_L)
			{
				//Wait to read signal strength
				AAD_SIF_TIME_DELAY_MS (AAD_MONOTOR_TASK_SECAM_L);	//500ms

				OS_LockMutex(&g_AAD_Sema);

				/* Check SECAM L NICAM Mode. */
				if( (g_o_eAudioMode == LX_ATV_GET_PAL_NICAM_MONO)	\
				  ||(g_o_eAudioMode == LX_ATV_GET_PAL_NICAM_STEREO) \
				  ||(g_o_eAudioMode == LX_ATV_GET_PAL_NICAM_DUAL) )
				{
					/* Stop a AAD Monitor Task for SIF SECAM L Mode */
					if(g_suiAadWaitQForMonitor != LX_AAD_TASK_STOP)
					{
						g_suiAadWaitQForMonitor = LX_AAD_TASK_STOP;
					}

					/* Stop AAD Monitor. */
					ui64TaskTimeout = AAD_MONOTOR_TASK_NO_TIMEOUT;

					AAD_NOTI("AAD_Monitor_Task(msg = %d, timeout = %d)\n", g_suiAadWaitQForMonitor, (UINT32)ui64TaskTimeout);

					/* sET a SECAM L NICAM Volume. */
					(void)AAD_SECAM_L_SetNICAMVolume();
				}
				else
				{
					/* Check LX Chip Revision Number */
					if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
					{
						/* Check a SIF SECAM L Magnitude. */
						(void)AAD_SECAM_L_SetManualVolumeForAdjust_H1x();
					}
					else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
					{
						;
					}
					else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
					{
						/* Check a SIF SECAM L Magnitude. */
						(void)AAD_SECAM_L_GetManualVolumeForAdjust();
					}
					else	//if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
					{
						/* Check a SIF SECAM L Magnitude. */
						(void)AAD_SECAM_L_SetManualVolumeForAdjust_H1x();
					}
				}

				OS_UnlockMutex(&g_AAD_Sema);
			}
		}
	} while ( TRUE );

	return 0;
}

/** @} */

/**
 * NTSC MN A2 Audio Mode Detect Function for workaround.
*/
static int AAD_NTSC_MN_A2_Monitor_Task( void )
{
	static UINT32	ui32CurStatus = 0xFF;
	static UINT32	ui8PrevStatus = 0xFF;

	static UINT32	ui32ContinuousMonoCount   = 0;
	static UINT32	ui32ContinuousStereoCount = 0;

	//Check a Mono/Stereo Status
	AAD_RdFL(status);
	ui32CurStatus = AAD_Rd(status);

	switch (g_stCurMonoStState)
	{
		case LX_AAD_A2_DEFAULT0_STATE:
			g_stCurMonoStState = LX_AAD_A2_WAIT1_STATE;
			break;

		case LX_AAD_A2_WAIT1_STATE:
			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD1-1> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif

			if(ui32CurStatus == 0x13 && ui8PrevStatus == 0x13)
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ3_ST_STATE;
			}
			else if(ui32CurStatus == 0x3 && ui8PrevStatus == 0x3)
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ2_MONO_STATE;
			}

			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD1-2> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif
			break;

		case LX_AAD_A2_SEQ2_MONO_STATE:
			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD2-1> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
 			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif

			if(ui32CurStatus == 0x3)
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ2_MONO_STATE;
			}
			else if(ui32CurStatus == 0x13)
			{
				if(ui32ContinuousMonoCount > 40)
				{
					g_stCurMonoStState = LX_AAD_A2_WAIT1_STATE;
				}
				else
				{
					g_stCurMonoStState = LX_AAD_A2_SEQ3_ST_STATE;
				}
			}

			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD2-2> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif
			break;

		case LX_AAD_A2_SEQ3_ST_STATE:
			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD3-1> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif

			if(ui32CurStatus == 0x13)
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ3_ST_STATE;
			}
			else if(ui32CurStatus == 0x3)
			{
				if(ui8PrevStatus == 0x13)	// 방이동 case(2013.04.17)
				{
				if(ui32ContinuousStereoCount > 40)
				{
					g_stCurMonoStState = LX_AAD_A2_WAIT1_STATE;
				}
				else
				{
					g_stCurMonoStState = LX_AAD_A2_SEQ4_MONO_STATE;
				}
			}
				else	//수동 측정 case에 과도구간이 발생하면 WAIT1으로 이동(2013.04.17)
				{
					g_stCurMonoStState = LX_AAD_A2_WAIT1_STATE;
				}
			}

			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD3-2> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif
			break;

		case LX_AAD_A2_SEQ4_MONO_STATE:
			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD6> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif

			g_MonitorAudioMode = LX_ATV_GET_NTSC_A2_MONO;

			// 혹시 실수로 이곳에 빠져도 Stereo mode이면 SEQ3_ST로 이동
			// 방이동의 경우 2초가 max이므로 Count는 40정도임.
			// 그러나 정밀측정에서는 이보다 더 길 수 있으므로
			// 3초(count = 60)로 setting함.(2013.04.17)
			if(ui32ContinuousStereoCount > 60)
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ3_ST_STATE;
			}
			else
			{
				g_stCurMonoStState = LX_AAD_A2_SEQ4_MONO_STATE;
			}
			break;

		default:
			#ifdef AAD_ATSC_A2_TASK_DEBUG
			AAD_DEBUG( "AAD_9> Cur = 0x%X, Prev = 0x%X, CMo = %d, CSt = %d, St = %d\n",	\
			ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
			#endif
			//g_stCurMonoStState = LX_AAD_A2_WAIT1_STATE;
			//g_MonitorAudioMode = LX_ATV_GET_NTSC_A2_UNKNOWN;
			break;
	}

#ifndef AAD_ATSC_A2_TASK_DEBUG
	AAD_INFO( "AAD> g_MonitorAudioMode = 0x%02X, Cur = 0x%02X, Prev = 0x%02X, CntMo = %02d, CntSt = %02d, State = %02d\n",	\
			   g_MonitorAudioMode, ui32CurStatus, ui8PrevStatus, ui32ContinuousMonoCount, ui32ContinuousStereoCount, g_stCurMonoStState );
#endif

	//Check a current and previous Mono Count
	if(ui32CurStatus == 0x3 && ui8PrevStatus == 0x3)
	{
		ui32ContinuousMonoCount++;
	}
	else
	{
		ui32ContinuousMonoCount = 0;
	}

	//Check a current and previous Stereo Count
	if(ui32CurStatus == 0x13 && ui8PrevStatus == 0x13)
	{
		ui32ContinuousStereoCount++;
	}
	else
	{
		ui32ContinuousStereoCount = 0;
	}

	//Update a current MO/ST Status
	ui8PrevStatus = (UINT8)ui32CurStatus;

	return RET_OK;
}

/**
 * PAL DK A2 Audio Mode Detect Function for workaround.
*/
static int AAD_PAL_DK_A2_Monitor_Task( void )
{
	static UINT32	ui8PrmCarDet, ui8SecCarDet, ui8MonoSt, ui8BiSap;

	static UINT32	ui32MonoCount = 0;
	static UINT32	ui32StereoCount = 0;
	static UINT32	ui32BilingualCount = 0;

	static UINT32  aadTaskPrintCount = 0;

	static LX_ATV_AUDIO_MODE_GET_T currentAudioMode  = LX_ATV_GET_NTSC_BTSC_UNKNOWN;
	static LX_ATV_AUDIO_MODE_GET_T previousAudioMode = LX_ATV_GET_NTSC_BTSC_UNKNOWN;

	//Check a Mono/Stereo/BiSap Status
	AAD_RdFL(status);
	AAD_Rd01(status, prmcardet, ui8PrmCarDet);
	AAD_Rd01(status, seccardet, ui8SecCarDet);
	AAD_Rd01(status, mono_str,  ui8MonoSt);
	AAD_Rd01(status, bisap,	    ui8BiSap);

	//Set a Mono/Stereo/BiSap Status
	if ((ui8PrmCarDet == 1) && (ui8SecCarDet == 1))
	{
		if (ui8MonoSt == 1)		//Stereo
		{
			currentAudioMode = LX_ATV_GET_PAL_STEREO;
		}
		else if (ui8BiSap == 1)	//Bilingual
		{
			currentAudioMode = LX_ATV_GET_PAL_DUAL;
		}
		else	//Mono
		{
			currentAudioMode = LX_ATV_GET_PAL_MONO;
		}
	}
	else if ((ui8PrmCarDet == 1) && (ui8MonoSt == 1))	//Primary Carrier & Stereo ID
	{
		currentAudioMode = LX_ATV_GET_PAL_MONO;
	}
	else if ((ui8PrmCarDet == 1) && (ui8BiSap == 1))	//Primary Carrier & Bilingual ID
	{
		currentAudioMode = LX_ATV_GET_PAL_MONO;
	}
	else if (ui8PrmCarDet == 1)		//Primary Carrier & Bilingual ID
	{
		currentAudioMode = LX_ATV_GET_PAL_MONO;
	}
	else
	{
		currentAudioMode = LX_ATV_GET_PAL_UNKNOWN;
	}

	//Increase a Mono/Stereo/BiSap Status Count
	if(currentAudioMode == LX_ATV_GET_PAL_MONO)
	{
		if(previousAudioMode == LX_ATV_GET_PAL_MONO)
			ui32MonoCount++;
		else
			if(ui32MonoCount >= ID_ST_CONTINUOUS_COUNT_DK)		//3
				ui32MonoCount++;
			else
				ui32MonoCount = 0;

		if(ui32MonoCount >= ID_ST_CONTINUOUS_COUNT_DK)
		{
			g_MonitorAudioMode = LX_ATV_GET_PAL_MONO;

			ui32StereoCount    = 0;
			ui32BilingualCount = 0;
		}
	}
	else if(currentAudioMode == LX_ATV_GET_PAL_STEREO)
	{
		if(previousAudioMode == LX_ATV_GET_PAL_STEREO)
			ui32StereoCount++;
		else
			if(ui32StereoCount >= ID_ST_CONTINUOUS_COUNT_DK)		//3
				ui32StereoCount++;
			else
				ui32StereoCount = 0;

		if(ui32StereoCount >= ID_ST_CONTINUOUS_COUNT_DK)
		{
			g_MonitorAudioMode = LX_ATV_GET_PAL_STEREO;

			ui32MonoCount	   = 0;
			ui32BilingualCount = 0;
		}
	}
	else if(currentAudioMode == LX_ATV_GET_PAL_DUAL)
	{
		if(previousAudioMode == LX_ATV_GET_PAL_DUAL)
			ui32BilingualCount++;
		else
			if(ui32BilingualCount >= ID_ST_CONTINUOUS_COUNT_DK)		//3
				ui32BilingualCount++;
			else
				ui32BilingualCount = 0;

		if(ui32BilingualCount >= ID_ST_CONTINUOUS_COUNT_DK)
		{
			g_MonitorAudioMode = LX_ATV_GET_PAL_DUAL;

			ui32MonoCount   = 0;
			ui32StereoCount = 0;
		}
	}
	else
	{
		g_MonitorAudioMode = LX_ATV_GET_PAL_UNKNOWN;

		ui32MonoCount      = 0;
		ui32StereoCount    = 0;
		ui32BilingualCount = 0;
	}

	if(currentAudioMode != previousAudioMode)
	{
		AAD_DEBUG( "AAD> Mode = %d(%d->%d), Mo = %2d, St = %2d, Bi = %2d(0x%2X)\n",	\
					g_MonitorAudioMode, previousAudioMode, currentAudioMode, ui32MonoCount, ui32StereoCount, ui32BilingualCount, AAD_Rd(status));
	}
	else if((++aadTaskPrintCount % AAD_MONOTOR_TASK_PRINT_TIME) == 0)	//Task calls every 50 ms.
	{
		AAD_INFO( "AAD> Mode = %d(%d->%d), Mo = %2d, St = %2d, Bi = %2d(0x%2X)\n",	\
				   g_MonitorAudioMode, previousAudioMode, currentAudioMode, ui32MonoCount, ui32StereoCount, ui32BilingualCount, AAD_Rd(status));
	}

	//Update a current audio mode Status
	previousAudioMode = currentAudioMode;

	return RET_OK;
}

/**
 * PAL B A2 India Audio High Deviation Mode Detect Function for workaround.
*/
static int AAD_PAL_B_A2_India_Monitor_Task( void )
{
	static UINT32	aadTaskPrintCount  = 0;

	UINT8		value;

	UINT8		ui8HighDevStatus = HIGH_DEV_UNKNOWN_STATUS;

	UINT16		ui16Carrier1Quality   = 0;
	UINT16		ui16Carrier2Magnitude = 0;
	UINT16		ui16Quality1ToMag2 	  = 0;

	LX_AAD_HIGH_DEV_SET_T eHighDevMode;

	// Read Car1 Qual.
	AAD_RdFL(car1_fm_qual0);
	AAD_Rd01(car1_fm_qual0, cr1ql, value);
	ui16Carrier1Quality  = value << 8;

	AAD_RdFL(car1_fm_qual1);
	AAD_Rd01(car1_fm_qual1, cr1ql, value);
	ui16Carrier1Quality += value;
	AAD_PRINT("Carrier1 Quality Current   0x%4X\n", ui16Carrier1Quality );

	// Read Car2 Mag.
	AAD_RdFL(car2_aver_mag0);
	AAD_Rd01(car2_aver_mag0, cr2mg, value);
	ui16Carrier2Magnitude  = value << 8;

	AAD_RdFL(car2_aver_mag1);
	AAD_Rd01(car2_aver_mag1, cr2mg, value);
	ui16Carrier2Magnitude += value;
	AAD_PRINT("Carrier2 Magnitude Current 0x%4X\n", ui16Carrier2Magnitude );

	// Compute Difference Carrier1 Quality to Carrier2 Mag.
	if(ui16Carrier1Quality >= ui16Carrier2Magnitude)
		ui16Quality1ToMag2 = ui16Carrier1Quality - ui16Carrier2Magnitude;
	else
		ui16Quality1ToMag2 = ui16Carrier2Magnitude - ui16Carrier1Quality;

	// Check Carrier1 Quality for HDEV OFF condition.
	if(ui16Carrier1Quality < 0x800 && ui16Quality1ToMag2 < 0x680)
	{
		if( (ui16Carrier1Quality  > 0x400 && ui16Carrier2Magnitude < 0x400)	\
		  ||(ui16Carrier1Quality <= 0x400 && ui16Carrier2Magnitude < 0x900) )
		{
			//Add more delay for second loop.
			if((g_ui8PALBContiCount % 2) == 0)
			{
				//Wait to read signal strength
				AAD_SIF_TIME_DELAY_MS (HIGH_DEV_TASK_DELAY);	//10ms
			}

			// Increase g_ui8PALBContiCount
			g_ui8PALBContiCount++;

			// Set a Max Value for g_ui8PALBContiCount.
			if(g_ui8PALBContiCount > g_ui8PALBHighDevThreshold)
			{
				g_ui8PALBContiCount = g_ui8PALBHighDevThreshold;
			}
		}
		else
		{
			//Check a 2 value.
			if(g_ui8PALBContiCount > 2 && g_ui8PALBContiCount < g_ui8PALBHighDevThreshold)
			{
				// Decrease g_ui8PALBContiCount
				g_ui8PALBContiCount -= 2;
			}
			//Check a 1 value.
			else if(g_ui8PALBContiCount > 1 && g_ui8PALBContiCount < g_ui8PALBHighDevThreshold)
			{
				// Decrease g_ui8PALBContiCount
				g_ui8PALBContiCount --;
			}
		}
	}
	else
	{
		//Check a 2 value.
		if(g_ui8PALBContiCount > 2 && g_ui8PALBContiCount < g_ui8PALBHighDevThreshold)
		{
			// Decrease g_ui8PALBContiCount
			g_ui8PALBContiCount -= 2;
		}
		//Check a 1 value.
		else if(g_ui8PALBContiCount > 1 && g_ui8PALBContiCount < g_ui8PALBHighDevThreshold)
		{
			// Decrease g_ui8PALBContiCount
			g_ui8PALBContiCount --;
		}
	}

	// Set a High Deviation Mode.
	if(g_ui8PALBContiCount == g_ui8PALBHighDevThreshold)
	{
		ui8HighDevStatus = HIGH_DEV_OFF_STATUS;
	}
	else
	{
		ui8HighDevStatus = HIGH_DEV_ON_STATUS;
	}

	// Check Carrier1 Quality for HDEV OFF TO ON condition.
	if(ui16Carrier1Quality < 0xC00 && ui16Quality1ToMag2 < 0xA00)
	{
		if( (ui16Carrier1Quality  > 0x600 && ui16Carrier2Magnitude < 0x600)	\
		  ||(ui16Carrier1Quality <= 0x600 && ui16Carrier2Magnitude < 0xB00) )
		{
			// Set a Max Value for g_ui8PALBContiCount.
			if(g_ui8PALBContiCount > g_ui8PALBHighDevThreshold)
			{
				//Check a 1 value.
				if(g_ui8PALBContiCountOn > 1 && g_ui8PALBContiCountOn < HIGH_DEV_OFF_TO_ON_COUNT_IN)
				{
					// Decrease g_ui8PALBContiCountOn
					g_ui8PALBContiCountOn --;
				}
			}
		}
		else
		{
			// Check a High Deviation Off Mode.
			if(g_ui8PALBContiCount == g_ui8PALBHighDevThreshold)
			{
				// Increase g_ui8PALBContiCountOn
				g_ui8PALBContiCountOn++;
			}
		}
	}
	else
	{
		// Check a High Deviation Off Mode.
		if(g_ui8PALBContiCount == g_ui8PALBHighDevThreshold)
		{
			// Increase g_ui8PALBContiCountOn
			g_ui8PALBContiCountOn++;
		}
	}

	// Set a Max Value for g_ui8PALBContiCountOn.
	if(g_ui8PALBContiCountOn > HIGH_DEV_OFF_TO_ON_COUNT_IN)
	{
		g_ui8PALBContiCountOn = HIGH_DEV_OFF_TO_ON_COUNT_IN;
	}

	// Set a High Deviation Off To On Mode.
	if(g_ui8PALBContiCountOn == HIGH_DEV_OFF_TO_ON_COUNT_IN)
	{
		ui8HighDevStatus = HIGH_DEV_ON_STATUS;
	}

	// Check a current and global status and task count.
	if( (ui8HighDevStatus != g_ui8HighDevEabled && g_ui8PALBTaskCount <= HIGH_DEV_TASK_MAX_IN)	\
	  ||(g_ui8PALBContiCountOn == HIGH_DEV_OFF_TO_ON_COUNT_IN) )
	{
		if(ui8HighDevStatus == HIGH_DEV_ON_STATUS)
		{
			//Set a High Dev On : 540Khz
			eHighDevMode.bHighDevOnOff = TRUE;
			eHighDevMode.highDevLevel  = LX_AAD_HIGH_DEV_LEVEL_4;
		}
		else
		{
			//Set a High Dev Off : 50Khz
			eHighDevMode.bHighDevOnOff = FALSE;
			eHighDevMode.highDevLevel  = LX_AAD_HIGH_DEV_LEVEL_OFF;
		}

		// Check a previous setting
		if(eHighDevMode.bHighDevOnOff != g_eHighDevMode.bHighDevOnOff)
		{
			// Set a High Deviation Mode.
			(void)AAD_SIF_SetHighDeviationMode(eHighDevMode);

			if(ui8HighDevStatus == HIGH_DEV_ON_STATUS)
			{
				//Threshold PAL B High Deviation Off Count for India.
				g_ui8PALBHighDevThreshold = HIGH_DEV_ON_TO_OFF_COUNT_IN;

				//Clears a PAL B Task Count.
				g_ui8PALBTaskCount = 0;

				//Clears a PAL B High Deviation Check Count.
				g_ui8PALBContiCount = 0;

				//Clears a PAL B Task Count : High Deviation On.
				g_ui8PALBContiCountOn = 0;

				AAD_WARN( "AAD> Set HDev1 540Khz(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n",	\
						   g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
						   ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
			}
			else
			{
				//Clears a PAL B Task Count : High Deviation On.
				g_ui8PALBContiCountOn = 0;

				// Set a Max Value for g_ui8PALBTaskCount.
				g_ui8PALBTaskCount = HIGH_DEV_TASK_MAX_IN;

				AAD_WARN( "AAD> Set HDev 50Khz(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n",	\
						   g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
						   ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
			}
		}

		// Update g_ui8HighDevEabled status
		g_ui8HighDevEabled = ui8HighDevStatus;
	}
	else if( (g_ui8PALBTaskCount == g_ui8PALBHighDevThreshold && g_ui8PALBContiCount == HIGH_DEV_OFF_COUNT_0_IN)	\
		   ||(g_ui8PALBTaskCount == (HIGH_DEV_TASK_MAX_IN-1)  && g_ui8HighDevEabled != HIGH_DEV_OFF_STATUS) )
	{
		//Threshold PAL B High Deviation Off Count for India.
		g_ui8PALBHighDevThreshold = HIGH_DEV_ON_TO_OFF_COUNT_IN;

		// Update g_ui8HighDevEabled status
		g_ui8HighDevEabled = HIGH_DEV_ON_STATUS;

		//Clears a PAL B Task Count : High Deviation On.
		g_ui8PALBContiCountOn = 0;

		// Set a Max Value for g_ui8PALBTaskCount.
		g_ui8PALBTaskCount = HIGH_DEV_TASK_MAX_IN;

		AAD_WARN( "AAD> Set HDev2 540Khz(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n", \
				   g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
				   ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
	}

	//Set a PAL D Task Count.
	g_ui8PALBTaskCount++;

	// Set a Max Value for g_ui8PALBTaskCount.
	if(g_ui8PALBTaskCount > HIGH_DEV_TASK_MAX_IN)
	{
		g_ui8PALBTaskCount = HIGH_DEV_TASK_MAX_IN;
	}

	// Print a Debug 50Khz Filter Setting.
	if(g_ui8PALBTaskCount < HIGH_DEV_TASK_MAX_IN)
	{
		AAD_DEBUG( "AAD> HDev(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n", \
					g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
					ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
	}

	//Check a current status.
	if((++aadTaskPrintCount % AAD_MONOTOR_TASK_PRINT_TIME) == 0)	//Task calls every 50 ms.
	{
		AAD_DEBUG( "AAD> HDev(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n", \
					g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
					ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
	}
	else
	{
		AAD_INFO( "AAD> HDev(%2d->%2d(%2d)) = %d->%d, QUAL1 = 0x%4X, CAR2 = 0x%4X, DIFF = 0x%4X\n", \
				   g_ui8PALBTaskCount, g_ui8PALBContiCount, g_ui8PALBContiCountOn, g_ui8HighDevEabled, ui8HighDevStatus,	\
				   ui16Carrier1Quality, ui16Carrier2Magnitude, ui16Quality1ToMag2);
	}

	return RET_OK;
}



