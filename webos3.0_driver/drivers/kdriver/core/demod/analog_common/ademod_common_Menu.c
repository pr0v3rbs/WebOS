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

/*****************************************************************************
**
**  Name: ademod_common_Menu.c
**
**  Description: channel Setting  functions
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  KDRV_Dbg_Menu
**                  LX_ADEMOD_Result  ADEMOD_Demodulator_Set_ChannelScan
**                  LX_ADEMOD_Result  ADEMOD_Channel_SetForce
**                  LX_ADEMOD_Result  ADEMOD_Channel_Set
**
**  References:     
**
**  Exports:    
**
**  Dependencies:   ademod_common_demod.h 
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/

#if defined( __cplusplus )
extern "C"
{
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_Menu.h"
#include "ademod_common_Presets.h"
#include "ademod_common_UserUtilities.h"


LX_ADEMOD_Result KDRV_Dbg_Menu(Handle_t hDemod, UINT32 argc, UINT32 *val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 Optimization = 0;

	// Enable I2C optimization
	Optimization = 1;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_I2C_WRITE_OPTIM, &Optimization, TRUE);

	switch (argc)
	{
		case 0x1 :
		{
//			status = AFT_Auto_Adjust(hDemod,  val);
		}
		break;
		case 0x2 :
		{
			status = Carr_Offset_Get(hDemod,  val);
		}
		break;
		case 0x3 :
		{
			status = Video_Gain_Get(hDemod,  val);
		}
		break;
		case 0x4 :
		{
			status = Video_Gain_Set(hDemod,  val);
		}
		break;
		case 0x5 :
		{
			status = Video_Offs_Get(hDemod,  val);
		}
		break;
		case 0x6 :
		{
			status = Video_Offs_Set(hDemod,  val);
		}
		break;
		case 0x7 :
		{
			status = Video_Clips_Get(hDemod,  val);
		}
		break;
		case 0x8 :
		{
			status = Video_Clips_Set(hDemod,  val);
		}
		break;
		case 0x9 :
		{
			status = Video_BandMode_Get(hDemod,  val);
		}
		break;
		case 0xa :
		{
			status = Video_BandMode_Set(hDemod,  val);
		}
		break;

		case 0xb :
		{
			status = Video_Bandwidth_Get(hDemod,  val);
		}
		break;

		case 0xc :
		{
			status = Video_Bandwidth_Set(hDemod,  val);
		}
		break;

		case 0x10 :
		{
			status = Video_Backoff_Get(hDemod,  val);
		}
		break;
		case 0x11 :
		{
			status = Video_Backoff_Set(hDemod,  val);
		}
		break;
		case 0x12 :
		{
			status = Video_DigitalGain_Get(hDemod,  val);
		}
		break;
		case 0x13 :
		{
			status = Video_DigitalGain_Set(hDemod,  val);
		}
		break;

		case 0x20 :
		{
			status = Agc_Speed_Get(hDemod,  val);
		}
		break;
		case 0x21:
		{
			status = Agc_Speed_Set(hDemod,  val);
		}
		break;

		case 0x30 :
		{
			status = Audio_Mode_Get(hDemod,  val);
		}
		break;
		case 0x31 :
		{
			status = Audio_SIF_Set(hDemod,  val);
		}
		break;
		case 0x32 :
		{
			status = Audio_SIFFreq_Get(hDemod,  val);
		}
		break;
		case 0x33 :
		{
			status = Audio_SIFFreq_Set(hDemod,  val);
		}
		break;
		case 0x34 :
		{
			status = Audio_Mono_Set(hDemod,  val);
		}
		break;
		case 0x35 :
		{
			status = Audio_MonoCutoff_Get(hDemod,  val);
		}
		break;
		case 0x36 :
		{
			status = Audio_MonoCutoff_Set(hDemod,  val);
		}
		break;
		case 0x37 :
		{
			status = Audio_Gain_Get(hDemod,  val);
		}
		break;
		case 0x38 :
		{
			status = Audio_Gain_Set(hDemod,  val);
		}
		break;

		case 0x39 :
		{
			status = Audio_Offset_Get(hDemod,  val);
		}
		break;
		case 0x3a :
		{
			status = Audio_Offset_Set(hDemod,  val);
		}
		break;

		case 0x50 :
		{
			status = Video_LockRange_Get(hDemod,  val);
		}
		break;
		case 0x51 :
		{
			status = Video_LockRange_Set(hDemod,  val);
		}
		break;
		case 0x52 :
		{
			status = Lock_Status_Get(hDemod,  val);
		}
		break;
		case 0x53 :
		{
			status = Sync_Status_Get(hDemod,  val);
		}
		break;
		case 0x54 :
		{
			status = IF_Freq_Get(hDemod,  val);
		}
		break;
		case 0x55 :
		{
			status = IF_Freq_Set(hDemod,  val);
		}
		break;
		default :
		{
			DEMOD_PRINT("0x2 -	Get carrier offset.\n\r");
			DEMOD_PRINT("0x3 -	Get Video Gain.\n\r");
			DEMOD_PRINT("0x4 -	Set Video Gain.\n\r");
			DEMOD_PRINT("0x5 -	Get Video Offset.\n\r");
			DEMOD_PRINT("0x6 -	Set Video Offset.\n\r");
			DEMOD_PRINT("0x7 -	Get Video Clipset.\n\r");
			DEMOD_PRINT("0x8 -	Set Video Clipset.\n\r");
			DEMOD_PRINT("0x9 -	Get Band Mode.\n\r");
			DEMOD_PRINT("0xa -	Set Band Mode.\n\r");
			DEMOD_PRINT("0xb -	Get Video Bandwidth.\n\r");
			DEMOD_PRINT("0xc -	Set Video Bandwidth.\n\r");

			DEMOD_PRINT("0x10 -	Get Backoff.\n\r");
			DEMOD_PRINT("0x11 -	Set Backoff.\n\r");
			DEMOD_PRINT("0x12 -	Get Digital Gain.\n\r");
			DEMOD_PRINT("0x13 -	Set digital gain.\n\r");

			DEMOD_PRINT("0x20 -	Get Agc Speed.\n\r");
			DEMOD_PRINT("0x21 -	Set Agc Speed.\n\r");

			DEMOD_PRINT("0x30 -	Get Audio mode SIF/Mono.\n\r");
			DEMOD_PRINT("0x31 -	Set Audio mode SIF.\n\r");
			DEMOD_PRINT("0x32 -	Get Audio SIF frequency cut-off.\n\r");
			DEMOD_PRINT("0x33 -	Set Audio SIF frequency.\n\r");
			
			DEMOD_PRINT("0x34 -	Set Audio mode Mono.\n\r");
			DEMOD_PRINT("0x35 -	Get Audio SIF frequency cut-off.\n\r");
			DEMOD_PRINT("0x36 -	Set Audio SIF frequency.\n\r");

			DEMOD_PRINT("0x37 -	Get Audio SIF Gain.\n\r");
			DEMOD_PRINT("0x38 -	Set Audio SIF Gain.\n\r");
			DEMOD_PRINT("0x39 -	Get Audio SIF offset.\n\r");
			DEMOD_PRINT("0x3a -	Set Audio SIF offset.\n\r");

			DEMOD_PRINT("0x50 -	Get Lock Range.\n\r");
			DEMOD_PRINT("0x51 -	Set Lock Range.\n\r");
			DEMOD_PRINT("0x52 -	Get lock status.\n\r");
			DEMOD_PRINT("0x53 -	Get sync status.\n\r");
			DEMOD_PRINT("0x54 -	Get IF frequency.\n\r");
			DEMOD_PRINT("0x55 -	Set IF frequency.\n\r");
		}
		break;
	}
	return (status);
}


LX_ADEMOD_Result ADEMOD_Demodulator_Set_ChannelScan(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_InputMode InputMode = (pSmartTune->Filter & 0x10000000) ? LX_ADEMOD_INPUTMODE_LEGACY : LX_ADEMOD_INPUTMODE_DIGITAL;
	UINT32 VideoStandard = (pSmartTune->Filter & 0x0FFFFFFF);
	UINT32 AudioStandard = LX_ADEMOD_AUDIO_STANDARD_NONE;

	UINT32 value[2];
	UINT32 smartValue[1];

	value[0]=1;
	value[1] = 0x1;
	ADEMOD_ChanScan_Set(hDemod, value, TRUE); //I2C optimize 적용

	// Execute System Init preset
	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_SYSTEM_INIT);
	// Execute Video Mode preset
	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_MODE);

	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_REGS);

	smartValue[0] = pSmartTune->SmplRate;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_SMPL_RATE, smartValue, FALSE);

	//  Start demodulation
	if (status == retOK) {
		status = ADEMOD_Demodulate(hDemod,
			InputMode,
			VideoStandard,
			AudioStandard);
	}

	value[0] = 0;
	ADEMOD_ChanScan_Set(hDemod, value, TRUE); //I2C optimize 적용해제

	return (status);
}

LX_ADEMOD_Result ADEMOD_Channel_SetForce(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_InputMode InputMode = (pSmartTune->Filter & 0x10000000) ? LX_ADEMOD_INPUTMODE_LEGACY : LX_ADEMOD_INPUTMODE_DIGITAL;
	UINT32 VideoStandard = (pSmartTune->Filter & 0x0FFFFFFF);
	UINT32 AudioStandard = LX_ADEMOD_AUDIO_STANDARD_NONE;

	UINT32 value[1];

	value[0] = pSmartTune->SmplRate;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_SMPL_RATE, value, FALSE);

	value[0] = pSmartTune->IF;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_FRQ, value, FALSE);

	if (status == retOK) {
		status = ADEMOD_Demodulate(hDemod,
			InputMode,
			VideoStandard,
			AudioStandard);
	}

	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_REGS);

	return (status);
}




LX_ADEMOD_Result ADEMOD_Channel_Set(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune, BOOL Update)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_InputMode InputMode = (pSmartTune->Filter & 0x10000000) ?  LX_ADEMOD_INPUTMODE_LEGACY :  LX_ADEMOD_INPUTMODE_DIGITAL;
	UINT32 VideoStandard = (pSmartTune->Filter & 0x0FFFFFFF);
	UINT32 AudioStandard = LX_ADEMOD_AUDIO_STANDARD_NONE;

	UINT32 value[2];

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	value[0]=1;
	value[1] = 0x1;
	ADEMOD_ChanScan_Set(hDemod, value, TRUE); //I2C optimize 적용

//	status = Tuner_Tune(pSmartTune); // tuner is controlled by the GUI
	if (Update || pSmartTune->Update) {

		// Set parameter: FMLG_PARM_SMPL_RATE
	//	if (status == retOK && pSmartTune->Update)
		{
			UINT32 value[1];
			value[0] = pSmartTune->SmplRate;
			status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_SMPL_RATE, value, FALSE);
		}
		// Set parameter: FMLG_PARM_VID_FRQ
	//	if (status == retOK && pSmartTune->Update)
		{
			UINT32 value[1];
			value[0] = pSmartTune->IF;
			status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_FRQ, value, FALSE); //091014 Smarttune시 VID_FRQ는 동일(SECAM L' 때문에)
		}



		// Execute register portion of the Video Mode preset
		status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_REGS);

		// Start demodulation
		if (status == retOK) {
			if (Update == TRUE) {	//jeongpil.yun for M14
				status = ADEMOD_Demodulate(hDemod,
					InputMode,
					VideoStandard,
					AudioStandard);
			}

		}
	}
	value[0] = 0;
	ADEMOD_ChanScan_Set(hDemod, value, TRUE); //I2C optimize 적용해제

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return (status);
}

LX_ADEMOD_Result ADEMOD_Demodulator_Set(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune)
{
	LX_ADEMOD_Result status = retOK;

	// Execute System Init preset
	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_SYSTEM_INIT);
	// Execute Video Mode preset
	status = ADEMOD_Preset_Set(hDemod, pSmartTune, LX_ADEMOD_PRESET_MODE);
	// Execute Smart Tune and Start demodulation
	status = ADEMOD_Channel_Set(hDemod, pSmartTune, TRUE);

	return (status);
}
#if defined( __cplusplus )
}
#endif
