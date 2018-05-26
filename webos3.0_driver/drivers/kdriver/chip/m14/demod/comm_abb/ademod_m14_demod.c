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
**  Name:ademod_m14_demod.c
**
**  Description:    ABB Demodulator software interface.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  DEMOD_M14_Analog_Demod_Open
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Demod_Close
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Demodulate
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Parameter_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Parameter_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_AGC_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_AGC_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_ChanScan_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_ChanScan_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_SleepMode_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_SleepMode_Get
**
**  References:    
**
**  Exports:        LX_ADEMOD_Result  DEMOD_M14_Analog_Demod_Open
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Demod_Close
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Demodulate
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Parameter_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_Parameter_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_AGC_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_AGC_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_ChanScan_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_ChanScan_Get
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_SleepMode_Set
**                  LX_ADEMOD_Result  DEMOD_M14_Analog_SleepMode_Get
**
**  Dependencies:   ademod_m14_demod.h for system configuration data.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
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

#include "ademod_m14_config.h"
//#include <stdlib.h>                     /*  for NULL      */
#include "ademod_common.h"



#include "ademod_m14_demod.h"
#include "ademod_m14_regaddress.h"
#include "ademod_m14_factory.h"


BOOLEAN _gSpectrumInv = FALSE;

static LX_ADEMOD_M14_Instance gADEMOD_Info[1];

#define MAX_REG_TABLE_SIZE	(400)
static UINT32 exclude_regs[MAX_REG_TABLE_SIZE];
static UINT32 include_regs[MAX_REG_TABLE_SIZE];

static int sys_obs_regs[] = {
	0x0800, // TWA_MASTERCTRL
	0x0801, // TWA_MASTERSTART
	0x0802, // TWA_MASTERTXFER
	0x0100, // Programmed later in SmartTune table
	0x0114, // Programmed later in SmartTune table
	0x013B, // Programmed later in SmartTune table
	0x013E, // Programmed later in SmartTune table
	0x013F, // Programmed later in SmartTune table
	0x0935, // Programmed later in SmartTune table
	0x0937, // Programmed later in SmartTune table
	0x0939, // Programmed later in SmartTune table
	0x093B, // Programmed later in SmartTune table
	//linda - hybrid AGC//0x0A0C, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0A09, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0A32, // IFVGA not used by any customer
	0x0A26, // IFVGA not used by any customer
	0x0A28, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0354, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0355, // IFVGA not used by any customer
	0x0356, // IFVGA not used by any customer
	0x0357, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0358, // IFVGA not used by any customer
	0x0359, // IFVGA not used by any customer
	0x035A, // IFVGA not used by any customer
	0x035B, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0361, // IFVGA not used by any customer
	0x0365, // IFVGA not used by any customer
	0x036B, // IFVGA not used by any customer
	0x036F, // IFVGA not used by any customer
	0x0418, // This register is obsolete in Spyder
	0x02A9, // Not used
	0x02B2, // Same as POD
	-1, // terminator
};

static int obs_regs[] = {
	0x800, // TWA_MASTERCTRL
	0x801, // TWA_MASTERSTART
	0x802, // TWA_MASTERTXFER
	0x0404, // will be handled by the GUI post processing
	0x041D, // will be handled by the GUI post processing
	0x0442, // will be handled by the GUI post processing
	0x095C, // will be handled by the GUI post processing
	0x095D, // will be handled by the GUI post processing
	//linda - hybrid AGC//0x0A0C, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0A09, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0A32, // IFVGA not used by any customer
	0x0A26, // IFVGA not used by any customer
	0x0A28, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0354, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0355, // IFVGA not used by any customer
	0x0356, // IFVGA not used by any customer
	0x0357, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0358, // IFVGA not used by any customer
	0x0359, // IFVGA not used by any customer
	0x035A, // IFVGA not used by any customer
	0x035B, // IFVGA not used by any customer
	//linda - hybrid AGC//0x0361, // IFVGA not used by any customer
	0x0365, // IFVGA not used by any customer
	0x036B, // IFVGA not used by any customer
	0x036F, // IFVGA not used by any customer
	0x0418, // This register is obsolete in Spyder
	0x02A9, // Not used
	0x02B2, // Same as POD
	-1, // terminator
};

static int smart_tune_regs[] = {
	0x0000,
	0x0020,
	0x002E,
	0x0032,
	0x0036,
	0x003B,
	0x0047,
	0x0024,
	0x0004,
	0x0008,
	0x0027,
	0x0299,
	0x028D,
	0x0290,
	0x02E1,
	0x02ED,
	0x02E4,
	0x072B,
	0x002E,
	0x0032,
	0x0036,
	0x0004,
	0x0008,
	0x002C,
	0x0100,
	0x013B,
	0x0114,
	0x011E,
	0x0121,
	0x0103,
	0x0104,
	0x0105,
	0x0108,
	0x0117,
	0x011A,
	0x095A,
	-1, // terminator
};

/*
**  Local Function Prototypes - not available for external access.
*/
static LX_ADEMOD_Result DEMOD_M14_Analog_Context(Handle_t hDevice, LX_ADEMOD_M14_Instance** ppLX_ADEMOD_instance);

static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_Clear(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_Set(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_RegisterSet(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_Initialize(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_Start(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_Update_Clear(Handle_t hDevice);

static LX_ADEMOD_Result DEMOD_M14_Analog_I2C_Optimization_Set(Handle_t hDevice, UINT32* pValue);

static LX_ADEMOD_Result DEMOD_M14_Analog_I2C_Optimization_Get(Handle_t hDevice, UINT32* pValue);

/*
**  API implementation.
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Demod_Open
**
**  Description:    Creates and Initializes the instance of specified demodulator.
**
**  Parameters:     DeviceId     - Device type + h/w instance id.
**                  hUserData    - User-defined data, if needed for the
**                                 FmLg_ReadSub() & FmLg_WriteSub functions.
**                  UserSize     - The size of the User Data structure in BYTES.
**                             NOTE: Max size is 128 bytes.
**                  phDevice     - Pointer to Device Handle
**
**  Returns:        status:
**                      retOK             - No errors
**                      retErrBadHandle   - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Demod_Open ( UINT32     DeviceId,
                              UINT32*    pUserData,
					          UINT32     UserSize,
					          Handle_t*  phDevice )
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	pLX_ADEMOD_M14_Instance pInstance = NULL;

    /*  Check the argument before using  */
	if (phDevice == NULL) {
        return retErrBadHandle;
	}


	if (UserSize > MAX_SIZE_USER)
	{
		return retUserSizeTooBig;
	}
	DEMOD_PRINT( "[ADEMOD](%s:%d)DeviceId = %d !!!\n", __F__, __L__,DeviceId);

	// initialize instance
	DeviceId |= LX_ADEMOD_DEMOD_deBug_1_FLAG;
	
	pInstance = &gADEMOD_Info[0];
	pInstance->common.device_id = DeviceId;
	pInstance->common.input_mode = LX_ADEMOD_INPUTMODE_NONE;
	pInstance->common.sleep_mode = LX_ADEMOD_SLEEP_MODE_NONE;
#if LX_ADEMOD_REGISTER_CACHE_ENABLED
	{
		UINT32 cache_options = 0;
		// Register cache can be used to debug software while no actual hardware is available.
		memset(pInstance->reg_cache, LX_ADEMOD_CACHE_FILL_BYTE, sizeof(pInstance->reg_cache));
		memset(pInstance->reg_cache_flags, 0, sizeof(pInstance->reg_cache_flags));
		RegisterCacheBufferSet((void*)pInstance->reg_cache);
		RegisterCacheFlagsSet((void*)pInstance->reg_cache_flags);
		// Register cache can also be used to optimize I2C traffic.
		if (LX_ADEMOD_DEVICE_DEBUG(DeviceId) & LX_ADEMOD_DEMOD_DEBUG_CACHE_FLAG) { // use register cache for debugging
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_WRITE;
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_READ;
		}
		if (LX_ADEMOD_DEVICE_DEBUG(DeviceId) & LX_ADEMOD_DEMOD_DEBUG_I2COPT_FLAG) { // I2C optimization
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_WRITE;
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_OPTIM;
		}
		RegisterCacheOptionsSet(cache_options);
	}
#endif

	// copy user data into context
	memcpy((void*)pInstance->common.user, (void*)pUserData, UserSize);
	pInstance->common.hUserData = (UINT32 *)pInstance->common.user;

	// clear user update "sticky" bits
	status = DEMOD_M14_Analog_Update_Clear((Handle_t)pInstance);

	// apply factory settings to parameters
	status = DEMOD_M14_Analog_Factory_Clear((Handle_t)pInstance);
	status = DEMOD_M14_Analog_Factory_Set((Handle_t)pInstance);

	if (!(LX_ADEMOD_DEVICE_DEBUG(DeviceId) & LX_ADEMOD_DEMOD_DEBUG_HWSKIP_FLAG)) { // Skip hardware init
		// initialize hardware
		status = DEMOD_M14_Analog_Initialize((Handle_t)pInstance);
	}

	if (retOK == status) {
        *phDevice = (Handle_t)pInstance;
		pInstance->digagc.hDevice = (Handle_t)pInstance;
	}

    return (status);
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Demod_Close
**
**  Description:    Closes instance of the demodulator.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Demod_Close(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}
	// put demodulator into sleep mode
	DEMOD_M14_Analog_SleepMode_Set(hDevice, LX_ADEMOD_SLEEP_MODE_LEVEL_1);

	// invalidate the handle
	pInstance->common.device_id = 0;
    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Demodulate
**
**  Description:    Starts demodulation in specified mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  InputMode    - The input mode. Can be one of the following:
**                                 LX_ADEMOD_INPUTMODE_LEGACY,
**                                 LX_ADEMOD_INPUTMODE_DIGITAL,
**                                 LX_ADEMOD_INPUTMODE_ATSC.
**                  VideoStandard - Legacy video standard (see LX_ADEMOD_VIDEO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_VIDEO_STANDARD_NONE.
**                  AudioStandard - Legacy audio standard (see LX_ADEMOD_AUDIO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_AUDIO_STANDARD_NONE.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Demodulate(Handle_t hDevice, UINT32 InputMode, UINT32 VideoStandard, UINT32 AudioStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;
	UINT32 Flags = 0;
	UINT32 AudioMode = LX_ADEMOD_AUDIO_MODE_NONE;
	UINT32 Divider = 0;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	// Clear dedicated counter
	I2C_CounterClear(1);
	// Apply system-level factory settings
	pInstance->common.input_mode = LX_ADEMOD_INPUTMODE_NONE;
	// Apply factory settings for selected mode
	status = DEMOD_M14_Analog_Factory_Set(hDevice);
	// set video standard
	pInstance->common.video_std = VideoStandard;
	// set audioo standard
	pInstance->common.audio_std = AudioStandard;
	// set input mode
	pInstance->common.input_mode = InputMode;
	// Apply factory settings for selected mode
	status = DEMOD_M14_Analog_Factory_Set(hDevice);


	// This function sets clocks appropriate to the mode!!!
	// Therefore it should be called before everything else in this sequence.
	// Otherwice registers will not be programmed.
	status  = DEMOD_M14_Analog_AudioProc_Mode_Get(&pInstance->audio, &AudioMode, pInstance->common.hUserData);
	status  = DEMOD_M14_Analog_VideoProc_SampleRateDivider_Get(&pInstance->video, &Divider, pInstance->common.hUserData);
	status  = DEMOD_M14_Analog_Bsp_Mode_Set(&pInstance->bsp, InputMode, AudioMode, Divider, pInstance->common.hUserData);
	status  = DEMOD_M14_Analog_LegacyDemod_Flags_Get(&pInstance->legacy, &Flags);
	// Update SIF/Mono flag
	status  = DEMOD_M14_Analog_LegacyDemod_Flags_Set(&pInstance->legacy, Flags, pInstance->common.hUserData, FALSE);

	// Update the LX_ADEMOD_FFLAG_44n36 flag
	status  = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &Flags);
	if ((pInstance->video.sampling_rate == LX_ADEMOD_SAMPLING_RATE_240MHz) ||
		(pInstance->video.sampling_rate == LX_ADEMOD_SAMPLING_RATE_250MHz))
	{
		Flags |= LX_ADEMOD_FFLAG_44n36;
	}
	else
	{
		Flags &= ~LX_ADEMOD_FFLAG_44n36; // default
	}

	// modified by j.y. won for LG3100
	if(pInstance->video.inp_frequency > 40000000L)
		Flags |= LX_ADEMOD_FFLAG_44n36;
	else
		Flags &= ~LX_ADEMOD_FFLAG_44n36;

	status  = DEMOD_M14_Analog_VideoProc_Flags_Set(&pInstance->video, Flags, LX_ADEMOD_FFLAG_44n36);

	if (LX_ADEMOD_INPUTMODE_LEGACY == InputMode) {
		status  = DEMOD_M14_Analog_Bsp_Standard_Set(&pInstance->bsp, VideoStandard, pInstance->common.hUserData);
		status  = DEMOD_M14_Analog_VideoProc_Standard_Set(&pInstance->video, VideoStandard);
		status  = DEMOD_M14_Analog_AudioProc_Standard_Set(&pInstance->audio, VideoStandard, AudioStandard);
		status  = DEMOD_M14_Analog_LegacyDemod_Standard_Set(&pInstance->legacy, VideoStandard, AudioStandard);
		status  = DEMOD_M14_Analog_AGC_Standard_Set(&pInstance->agc, VideoStandard);
		status  = DEMOD_M14_Analog_DigAGC_Standard_Set(&pInstance->digagc, VideoStandard);

		//status  = DEMOD_M14_Analog_AudioProc_Standard_Set(&pInstance->audio, AudioStandard);
	}
	else {
		// Low IF mode
		// set group delay to bypass mode
		status  = DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set(&pInstance->legacy, NULL, pInstance->common.hUserData, TRUE);
	}

	// DEMOD_M14_Analog_VideoProc_Mode_Set uses bandwidth and inp_freq for its calculations,
	// therefore it must be set after DEMOD_M14_Analog_VideoProc_Standard_Set where these
	// parameters are defined
	status  = DEMOD_M14_Analog_VideoProc_Mode_Set(&pInstance->video, InputMode);
	status  = DEMOD_M14_Analog_Agc_Mode_Set(&pInstance->agc, InputMode);
	status  = DEMOD_M14_Analog_DigAgc_Mode_Set(&pInstance->digagc, InputMode);

	// start demodulation
	status = DEMOD_M14_Analog_Start(hDevice);
	// Apply factory registers for selected mode
	status = DEMOD_M14_Analog_Factory_RegisterSet(hDevice);


//	RegisterCachePostProcess(pInstance->common.hUserData, Flags); // post process (soft reset, clear FIFO etd)

	memset(pInstance->reg_cache_flags, 0, sizeof(pInstance->reg_cache_flags)); // clear cache flags
	RegisterCachePostProcess(pInstance->common.hUserData); // post process (soft reset, clear FIFO etd)

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Parameter_Set
**
**  Description:    Sets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**                  UpdateNow    - When TRUE ?the value of the parameter will be
**                                 propagated into the registers during the function call;
**                                 When FALSE ?the value of the parameter will be
**                                 internally stored until any other function is called
**                                 with UpdateNow parameter equal TRUE or FmLg_Demodulate
**                                 function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Parameter_Set(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	switch (ParamID) {
		case LX_ADEMOD_PARM_LOG_OPEN:
			{
				char name[80];
				int j = 0; // "exclude" counter
				int k = 0; // "include" counter
				// pParamValue[0] - major number, specifies video standard
				// pParamValue[1] - minor number, specifies smart tune in MHz.
				sprintf(name, ".\\LX_ADEMOD_SAMPLE\\src\\user\\log_%d_%d.txt", pParamValue[0], pParamValue[1]);
				memset(include_regs, -1, sizeof(include_regs));
				memset(exclude_regs, -1, sizeof(exclude_regs));

				if (pParamValue[0] == 0) {
					if (pParamValue[1] == 0) {
						// system table
						int i = 0;
						// exclude system obsolete registers
						while (sys_obs_regs[i] >= 0) {
							exclude_regs[j] = sys_obs_regs[i];
							i++;
							j++;
						}
						// exclude smart tune registers
						i = 0;
						while (smart_tune_regs[i] >= 0) {
							exclude_regs[j] = smart_tune_regs[i];
							i++;
							j++;
						}
						exclude_regs[j++] = 0x0114; // programmed later in mode table
						exclude_regs[j++] = 0x013B; // programmed later in mode table
					}
					else {
						// full table
					}
				}
				else {
					// format
					if (pParamValue[1] == 0) {
						// major (standard) table
						int i = 0;
						// exclude obsolete registers
						while (obs_regs[i] >= 0) {
							exclude_regs[j] = obs_regs[i];
							i++;
							j++;
						}
						// exclude smart tune registers
						i = 0;
						while (smart_tune_regs[i] >= 0) {
							exclude_regs[j] = smart_tune_regs[i];
							i++;
							j++;
						}
					}
					else {
						// minor (smart tune) table
						int i = 0;
						// exclude obsolete registers
						while (obs_regs[i] >= 0) {
							exclude_regs[j] = obs_regs[i];
							i++;
							j++;
						}
						// include smart tune registers
						i = 0;
						while (smart_tune_regs[i] >= 0) {
							include_regs[k] = smart_tune_regs[i];
							i++;
							k++;
						}
					}
				}

//				status = RegisterLogOpen(pInstance->common.hUserData, name, (k) ? (int *)include_regs : NULL, (j) ? (int *)exclude_regs : NULL, 0);
			}
			break;

		case LX_ADEMOD_PARM_LOG_CLOSE:
//			status = RegisterLogClose(pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_MODE:
			status = DEMOD_M14_Analog_AudioProc_Mode_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_VID_FRQ:		// IF Input frequency
			status  = DEMOD_M14_Analog_VideoProc_InputFreq_Set(&pInstance->video, pParamValue[0]);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_AUD_FRQ:		// audio frequency
			status  = DEMOD_M14_Analog_AudioProc_InputFreq_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_SMPL_RATE:		// Sampling rate
			status  = DEMOD_M14_Analog_VideoProc_SampleRate_Set(&pInstance->video, pParamValue[0]);
			status  = DEMOD_M14_Analog_RFAGC_SampleRate_Set(&pInstance->agc, pParamValue[0]);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_SMPL_RATE_DIV:		// Sampling rate divider
			status  = DEMOD_M14_Analog_VideoProc_SampleRateDivider_Set(&pInstance->video, pParamValue[0]);
			status  = DEMOD_M14_Analog_AudioProc_SampleRateDivider_Set(&pInstance->audio, pParamValue[0]);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_FACTORY:		// Reset to factory settings
			status  = DEMOD_M14_Analog_Update_Clear((Handle_t)pInstance);
			break;

		case LX_ADEMOD_PARM_QLTY_MODE:
			break;

		case LX_ADEMOD_PARM_SPCTR_INV:
			{
			UINT32 flags = 0;

			status  = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &flags);
#if 1
			if (_gSpectrumInv == FALSE)
			{
				flags |= LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}
			else if (_gSpectrumInv == TRUE)
			{
				flags &= ~LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}

#else
			if (pParamValue[0] == 0)
			{
				flags |= LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}
			else if (pParamValue[0] == 1)
			{
				flags &= ~LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}
#endif
			else {
				return retErrBadDevice;
			}
			status  = DEMOD_M14_Analog_VideoProc_Flags_Set(&pInstance->video, flags, LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP);

			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}

			}
			break;

		case LX_ADEMOD_PARM_VID_BNDW:
			status  = DEMOD_M14_Analog_VideoProc_Bandwidth_Set(&pInstance->video, pParamValue[0]);
			//status  = DEMOD_M14_Analog_LowIF_Bandwidth_Set(&pInstance->low_if, pParamValue[0]);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_LOWIF_BNDW:
			status  = DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set(&pInstance->video, pParamValue[0]);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		case LX_ADEMOD_PARM_SNR:
			break;

		case LX_ADEMOD_PARM_PWR:
			break;

		case LX_ADEMOD_PARM_BER:
			break;

		case LX_ADEMOD_PARM_GPIO:
			break;

		case LX_ADEMOD_PARM_AGC_GAIN:
			status = DEMOD_M14_Analog_AGC_Gain_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_OPTIONS:
			status = DEMOD_M14_Analog_AGC_Options_Set(&pInstance->agc, pParamValue[0], pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_GRP_DEL:
			{
				LX_ADEMOD_GroupDelayLUT GrDel_LUT;
				int size = ((LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) & LX_ADEMOD_DEMOD_deBug_1_FLAG) ? LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1 : LX_ADEMOD_GROUP_DELAY_LUT_SIZE);
				int i;
				for (i = 0; i < size; i++) {
					GrDel_LUT[i] =  pParamValue[i];
				}
				status = DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set(&pInstance->legacy, (UINT16 *)GrDel_LUT, pInstance->common.hUserData, UpdateNow);
			}
			break;

		case LX_ADEMOD_PARM_SYNC_DEC:
			break;

		case LX_ADEMOD_PARM_CONFIG_OPTIONS:
			status = DEMOD_M14_Analog_Bsp_Flags_Set(&pInstance->bsp, pParamValue[0], pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VSB_ENERGY:
			status = DEMOD_M14_Analog_VideoProc_VSB_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_MAIN_CUTOFF:
			status = DEMOD_M14_Analog_VideoProc_Cutoff_Set(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_CARR_OFFS:
			status = DEMOD_M14_Analog_VideoProc_CarrierOffset_Set(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;


		////////////////////////////////////////////////////////////////////////////
		// RF AGC
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_AGC_FASTMODE:
			status =  DEMOD_M14_Analog_Bsp_FastAGC_Set(&pInstance->bsp, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_OUTMODE:
			status =  DEMOD_M14_Analog_Bsp_OutAGC_Set(&pInstance->bsp, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_WANDMODE:
			status =  DEMOD_M14_Analog_Bsp_WandAGC_Set(&pInstance->bsp, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_GAIN_THR:
			status = DEMOD_M14_Analog_AGC_GainThrsh_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_GAIN_PRESET:
			status = DEMOD_M14_Analog_AGC_GainPreset_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_RFIIR_BW:
			status = DEMOD_M14_Analog_AGC_RFIIR_Bndw_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_LPD_ATTACK:
			status = DEMOD_M14_Analog_AGC_LPD_Speed_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_LPD_DECAY:
			status = DEMOD_M14_Analog_AGC_LPD_Decay_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_LPD_FASTDECAY:
			status = DEMOD_M14_Analog_AGC_LPD_FastDecay_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_LPD_DECAYWIN:
			status = DEMOD_M14_Analog_AGC_LPD_DecayWindow_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_LPD_DECAYTHR:
			status = DEMOD_M14_Analog_AGC_LPD_DecayThresh_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_DECFACTOR:
			status = DEMOD_M14_Analog_AGC_LPD_DecFactor_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_INSTAB_GAIN_ADJ:
			status = DEMOD_M14_Analog_AGC_InstabGainAdj_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_BNDW:
			status = DEMOD_M14_Analog_AGC_RF_Bndw_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_SPEED:
			status = DEMOD_M14_Analog_AGC_RF_Speed_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AGC_BACKOFF:
			status = DEMOD_M14_Analog_AGC_RF_Backoff_Set(&pInstance->agc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		////////////////////////////////////////////////////////////////////////////
		// Digital AGC
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_DIGAGC_GAIN_THR:
			status = DEMOD_M14_Analog_DigAGC_GainThrsh_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET:
			status = DEMOD_M14_Analog_DigAGC_GainPreset_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE:
			status = DEMOD_M14_Analog_DigAGC_CVBS_Mode_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_LPD_ATTACK:
			status = DEMOD_M14_Analog_DigAGC_LPD_Speed_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_LPD_DECAY:
			status = DEMOD_M14_Analog_DigAGC_LPD_Decay_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_LPD_FASTDECAY:
			status = DEMOD_M14_Analog_DigAGC_LPD_FastDecay_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_LPD_DECAYWIN:
			status = DEMOD_M14_Analog_DigAGC_LPD_DecayWindow_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_LPD_DECAYTHR:
			status = DEMOD_M14_Analog_DigAGC_LPD_DecayThresh_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BNDW:
			status = DEMOD_M14_Analog_DigAGC_Bndw_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED:
			status = DEMOD_M14_Analog_DigAGC_Speed_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BACKOFF:
			status = DEMOD_M14_Analog_DigAGC_Backoff_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_MODE:
			status = DEMOD_M14_Analog_LegacyDemod_MixedMode_Set(&pInstance->legacy, pParamValue[0], pInstance->common.hUserData, UpdateNow);
			status = DEMOD_M14_Analog_AGC_MixedMode_Set(&pInstance->agc, pParamValue[0], pInstance->common.hUserData, UpdateNow);
			status = DEMOD_M14_Analog_DigAgc_MixedMode_Set(&pInstance->digagc, pParamValue[0], pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET:
			status = DEMOD_M14_Analog_DigAgc_Target_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_DECIMATION_RATIO:
			status = DEMOD_M14_Analog_DigAgc_DecimationRatio_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_THR:
			status = DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_OFFS:
			status = DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BNDW_COH:
			status = DEMOD_M14_Analog_DigAGC_BndwCoherent_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED_COH:
			status = DEMOD_M14_Analog_DigAGC_SpeedCoherent_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR:
			status = DEMOD_M14_Analog_DigAGC_SpeedHybrid_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BACKOFF_COH:
			status = DEMOD_M14_Analog_DigAGC_BackoffCoherent_Set(&pInstance->digagc, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;
		////////////////////////////////////////////////////////////////////////////
		// Video Carrier Recovery (CR)
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_VIDCR_OVERMOD:
			status = DEMOD_M14_Analog_VideoProc_Overmodulation_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_OVERMOD_PHASEREV:
			status = DEMOD_M14_Analog_VideoProc_OvermodPhaseRev_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_RANGE:  // Pull-in range
			status  = DEMOD_M14_Analog_VideoProc_Range_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_FRLOOP_SPEED:
			status = DEMOD_M14_Analog_VideoProc_FRLoopSpeed_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_FRLOOP_GAIN:
			status = DEMOD_M14_Analog_VideoProc_FRLoopGain_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_PHLOOP_SPEED:
			status = DEMOD_M14_Analog_VideoProc_PHLoopSpeed_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_PHLOOP_GAIN:
			status = DEMOD_M14_Analog_VideoProc_PHLoopGain_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_LOCKDET:
			status = DEMOD_M14_Analog_VideoProc_LockDet_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_LOCKDETERR:
			status = DEMOD_M14_Analog_VideoProc_LockDetErr_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_LOCKDETLINES:
			status = DEMOD_M14_Analog_VideoProc_LockDetLines_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_UNLOCKDET:
			status = DEMOD_M14_Analog_VideoProc_UnlockDet_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_UNLOCKDETERR:
			status = DEMOD_M14_Analog_VideoProc_UnlockDetErr_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_UNLOCKDETLINES:
			status = DEMOD_M14_Analog_VideoProc_UnlockDetLines_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTELOCKDET:
			status = DEMOD_M14_Analog_VideoProc_MutelockDet_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTELOCKDETERR:
			status = DEMOD_M14_Analog_VideoProc_MutelockDetErr_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTELOCKDETLINES:
			status = DEMOD_M14_Analog_VideoProc_MutelockDetLines_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDET:
			status = DEMOD_M14_Analog_VideoProc_MuteunlockDet_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETERR:
			status = DEMOD_M14_Analog_VideoProc_MuteunlockDetErr_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_MUTEUNLOCKDETLINES:
			status = DEMOD_M14_Analog_VideoProc_MuteunlockDetLines_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_OVERMOD_WEIGHTING:
			{
				UINT32 Flags;
				status  = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &Flags);
				if (pParamValue[0] == 0) { // disable
					Flags |= LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
				}
				else { // enable
					Flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
				}
				status  = DEMOD_M14_Analog_VideoProc_Flags_Set(&pInstance->video, Flags, LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT);
			}
			break;

		case LX_ADEMOD_PARM_VIDCR_RESET:
			status = DEMOD_M14_Analog_VideoProc_CRReset_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDCR_OUT_CF:
			status = DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;


		////////////////////////////////////////////////////////////////////////////
		// Video Bandwidth (BW)
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_VIDBW_VIRTUALFS:
			status = DEMOD_M14_Analog_VideoProc_VirtualFS_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDBW_TINC:
			status = DEMOD_M14_Analog_VideoProc_Tinc_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VIDOFFS_TWEAK:
			status = DEMOD_M14_Analog_VideoProc_OffsTweak_Set(&pInstance->video, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				status = DEMOD_M14_Analog_Demodulate(hDevice, pInstance->common.input_mode, pInstance->common.video_std, pInstance->common.audio_std);
			}
			break;

		////////////////////////////////////////////////////////////////////////////
		// Audio
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_AUD_SIF_BW:
			status = DEMOD_M14_Analog_AudioProc_SifBw_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				UINT32 video_freq[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)+1]; // add 1 for sampling rate
				UINT32 video_flags;
				status = DEMOD_M14_Analog_VideoProc_InputFreq_GetX(&pInstance->video, video_freq, pInstance->common.hUserData);
				status = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &video_flags);
				status = DEMOD_M14_Analog_AudioProc_Start(&pInstance->audio, video_freq, video_flags, pInstance->common.hUserData);
			}
			break;

		case LX_ADEMOD_PARM_AUD_SIF_FRQ:
			status = DEMOD_M14_Analog_AudioProc_SifFreq_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				UINT32 video_freq[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)+1]; // add 1 for sampling rate
				UINT32 video_flags;
				status = DEMOD_M14_Analog_VideoProc_InputFreq_GetX(&pInstance->video, video_freq, pInstance->common.hUserData);
				status = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &video_flags);
				status = DEMOD_M14_Analog_AudioProc_Start(&pInstance->audio, video_freq, video_flags, pInstance->common.hUserData);
			}
			break;

		case LX_ADEMOD_PARM_AUD_MONO_FRQ:
			status = DEMOD_M14_Analog_AudioProc_MonoFreq_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_MONO_GAIN:
			status = DEMOD_M14_Analog_AudioProc_MonoGain_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_DEVIATION:
			status = DEMOD_M14_Analog_AudioProc_Deviation_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE:
			status = DEMOD_M14_Analog_AudioProc_BBSampleRate_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				UINT32 video_freq[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)+1]; // add 1 for sampling rate
				UINT32 video_flags;
				status = DEMOD_M14_Analog_VideoProc_InputFreq_GetX(&pInstance->video, video_freq, pInstance->common.hUserData);
				status = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &video_flags);
				status = DEMOD_M14_Analog_AudioProc_Start(&pInstance->audio, video_freq, video_flags, pInstance->common.hUserData);
			}
			break;

		case LX_ADEMOD_PARM_AUD_DEEMPHASIS:
			status = DEMOD_M14_Analog_AudioProc_Deemphasis_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_BSBNDEXTRAGAIN:
			status = DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_TARG:
			status = DEMOD_M14_Analog_AudioProc_AGCTarget_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_GAIN_THR:
			status = DEMOD_M14_Analog_AudioProc_AGCGainThresh_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_GAIN_PRESET:
			status = DEMOD_M14_Analog_AudioProc_AGCGainPreset_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_MIX:
			status = DEMOD_M14_Analog_AudioProc_Mix_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_AUTOMUTE:
			status = DEMOD_M14_Analog_AudioProc_Mute_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_MUTESPEED:
			status = DEMOD_M14_Analog_AudioProc_MuteSpeed_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_BUZZDELAY:
			status = DEMOD_M14_Analog_AudioProc_BuzzDelay_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_OPTIONS:
			status = DEMOD_M14_Analog_AudioProc_Options_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			if (UpdateNow) {
				UINT32 video_freq[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)+1]; // add 1 for sampling rate
				UINT32 video_flags;
				status = DEMOD_M14_Analog_VideoProc_InputFreq_GetX(&pInstance->video, video_freq, pInstance->common.hUserData);
				status = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &video_flags);
				status = DEMOD_M14_Analog_AudioProc_Start(&pInstance->audio, video_freq, video_flags, pInstance->common.hUserData);
			}
			break;

		////////////////////////////////////////////////////////////////////////////
		// DACs
		////////////////////////////////////////////////////////////////////////////

		case LX_ADEMOD_PARM_VID_GAIN:
			status = DEMOD_M14_Analog_LegacyDemod_Gain_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			status = DEMOD_M14_Analog_LowIF_Gain_Set(&pInstance->low_if, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VID_OFFS:
			status = DEMOD_M14_Analog_LegacyDemod_Offset_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			status = DEMOD_M14_Analog_LowIF_Offset_Set(&pInstance->low_if, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VID_CLIP:
			status = DEMOD_M14_Analog_LegacyDemod_Clip_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VID_GAIN_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VID_CLIP_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Clip_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_VID_OFFS_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_GAIN:
			status = DEMOD_M14_Analog_AudioProc_Gain_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_OFFS:
			status = DEMOD_M14_Analog_AudioProc_Offset_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_CLIP:
			status = DEMOD_M14_Analog_AudioProc_Clip_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_AUD_GAIN_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Gain_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;
		
		case LX_ADEMOD_PARM_AUD_OFFS_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Offset_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;
		
		case LX_ADEMOD_PARM_AUD_CLIP_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Clip_Set(&pInstance->audio, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		case LX_ADEMOD_PARM_I2C_WRITE_COUNT:
			I2C_CounterClear(0);
			break;

		case LX_ADEMOD_PARM_I2C_WRITE_OPTIM:
			DEMOD_M14_Analog_I2C_Optimization_Set(hDevice, pParamValue);
			break;

		case LX_ADEMOD_PARM_SYNC_OPTIONS:
			status = DEMOD_M14_Analog_LegacyDemod_SyncOptions_Set(&pInstance->legacy, pParamValue, pInstance->common.hUserData, UpdateNow);
			break;

		default:
			break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Parameter_Get
**
**  Description:    Gets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Parameter_Get(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	switch (ParamID) {
		case LX_ADEMOD_PARM_AUD_MODE:
			status = DEMOD_M14_Analog_AudioProc_Mode_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_FRQ:		// IF Input frequency
			status = DEMOD_M14_Analog_VideoProc_InputFreq_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_FRQ:		// IF Input frequency
			status = DEMOD_M14_Analog_AudioProc_InputFreq_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_SMPL_RATE:		// Sampling rate
			status = DEMOD_M14_Analog_VideoProc_SampleRate_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_SMPL_RATE_DIV: // Sampling rate divider
			status = DEMOD_M14_Analog_VideoProc_SampleRateDivider_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VIDCR_RANGE:  // Pull-in range
			status  = DEMOD_M14_Analog_VideoProc_Range_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_QLTY_MODE:
			break;

		case LX_ADEMOD_PARM_SPCTR_INV:
			{
			UINT32 flags = 0;
			status  = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &flags);
			pParamValue[0] = (flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP) ? 0 : 1;
			}
			break;

		case LX_ADEMOD_PARM_VID_BNDW:
			status = DEMOD_M14_Analog_VideoProc_Bandwidth_Get(&pInstance->video, pParamValue);
			break;

		case LX_ADEMOD_PARM_LOWIF_BNDW:
			status  = DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get(&pInstance->video, pParamValue);
			break;

		case LX_ADEMOD_PARM_VIDCR_OUT_CF:
			status = DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VIDCR_OVERMOD:
			status = DEMOD_M14_Analog_VideoProc_Overmodulation_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_GAIN:
			status = DEMOD_M14_Analog_LegacyDemod_Gain_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_CLIP:
			status = DEMOD_M14_Analog_LegacyDemod_Clip_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_OFFS:
			status = DEMOD_M14_Analog_LegacyDemod_Offset_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_GAIN_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_CLIP_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Clip_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VID_OFFS_RC:
			status = DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VIDBW_VIRTUALFS:
			status = DEMOD_M14_Analog_VideoProc_VirtualFS_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VIDBW_TINC:
			status = DEMOD_M14_Analog_VideoProc_Tinc_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_VIDOFFS_TWEAK:
			status = DEMOD_M14_Analog_VideoProc_OffsTweak_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_GAIN:
			status = DEMOD_M14_Analog_AudioProc_Gain_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_CLIP:
			status = DEMOD_M14_Analog_AudioProc_Clip_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_OFFS:
			status = DEMOD_M14_Analog_AudioProc_Offset_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_GAIN_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Gain_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;
		
		case LX_ADEMOD_PARM_AUD_OFFS_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Offset_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;
		
		case LX_ADEMOD_PARM_AUD_CLIP_RC:
			status = DEMOD_M14_Analog_AudioProc_RC_Clip_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_LOCK_STAT:
			if (pInstance->common.input_mode == LX_ADEMOD_INPUTMODE_LEGACY)
			{
				UINT8 temp = 0;
				status = ADEMOD_ReadByte(pInstance->common.hUserData, CR_STATUSCNTRBITS, &temp);
				pParamValue[0] = (temp & 0x10) ? 1 : 0; // LX_ADEMOD_CR_LOCK_FREQ=1.
			}
			else if (pInstance->common.input_mode == LX_ADEMOD_INPUTMODE_DIGITAL)
			{
			}
			else
			{
				status = retErrBadMode;
			}
			break;

		case LX_ADEMOD_PARM_SYNC_STAT:
			if (pInstance->common.input_mode == LX_ADEMOD_INPUTMODE_LEGACY)
			{
				UINT8 temp = 0;
				status = ADEMOD_ReadByte(pInstance->common.hUserData, SYNC_STATUS, &temp);
				pParamValue[0] = temp;
			}
			else if (pInstance->common.input_mode == LX_ADEMOD_INPUTMODE_DIGITAL)
			{
				pParamValue[0] = 0; // TBD
			}
			else
			{
				status = retErrBadMode;
			}
			break;

		case LX_ADEMOD_PARM_IF_LEVEL_STAT:
			status = DEMOD_M14_Analog_Bsp_Flags_Get(&pInstance->bsp, pParamValue);
			status = DEMOD_M14_Analog_DigAGC_Gain_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_SNR:
			status = retErrBadMode;
			break;

		case LX_ADEMOD_PARM_PWR:
			break;

		case LX_ADEMOD_PARM_BER:
			status = retErrBadMode;
			break;

		case LX_ADEMOD_PARM_GPIO:
			break;

		case LX_ADEMOD_PARM_AGC_GAIN:
			status = DEMOD_M14_Analog_AGC_Gain_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_GAIN_THR:
			status = DEMOD_M14_Analog_AGC_GainThrsh_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_GAIN_PRESET:
			status = DEMOD_M14_Analog_AGC_GainPreset_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_RFIIR_BW:
			status = DEMOD_M14_Analog_AGC_RFIIR_Bndw_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_SPEED:
			status = DEMOD_M14_Analog_AGC_RF_Speed_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_BACKOFF:
			status = DEMOD_M14_Analog_AGC_RF_Backoff_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_GAIN_THR:
			status = DEMOD_M14_Analog_DigAGC_GainThrsh_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED:
			status = DEMOD_M14_Analog_DigAGC_Speed_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BACKOFF:
			status = DEMOD_M14_Analog_DigAGC_Backoff_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_BACKOFF_COH:
			status = DEMOD_M14_Analog_DigAGC_BackoffCoherent_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE:
			status = DEMOD_M14_Analog_DigAGC_CVBS_Mode_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED_COH:
			status = DEMOD_M14_Analog_DigAGC_SpeedCoherent_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR:
			status = DEMOD_M14_Analog_DigAGC_SpeedHybrid_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_RF_LEVEL_STAT:
			status = DEMOD_M14_Analog_DigAGC_RFLevel_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_OPTIONS:
			status = DEMOD_M14_Analog_AGC_Options_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_SYNC_OPTIONS:
			status = DEMOD_M14_Analog_LegacyDemod_SyncOptions_Get(&pInstance->legacy, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_GRP_DEL:
			{
				LX_ADEMOD_GroupDelayLUT GrDel_LUT;
				int size = ((LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) & LX_ADEMOD_DEMOD_deBug_1_FLAG) ? LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1 : LX_ADEMOD_GROUP_DELAY_LUT_SIZE);
				int i;
				status = DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get(&pInstance->legacy, (UINT16 *)GrDel_LUT, pInstance->common.hUserData);
				for (i = 0; i < size; i++) {
					pParamValue[i] = GrDel_LUT[i];
				}
			}
			break;

		case LX_ADEMOD_PARM_SYNC_DEC:
			break;

		case LX_ADEMOD_PARM_DVBT_STAT:
			status = DEMOD_M14_Analog_LowIF_Status_Get(&pInstance->low_if, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_ISALIVE_STAT:
			{
			UINT8 temp = 0;
			status = ADEMOD_ReadByte(pInstance->common.hUserData, CHIP_ID, &temp);

			pParamValue[0] = (temp == 4) ? 1 : 0;
			}
			break;

		case LX_ADEMOD_PARM_CONFIG_OPTIONS:
			status = DEMOD_M14_Analog_Bsp_Flags_Get(&pInstance->bsp, pParamValue);
			break;

		case LX_ADEMOD_PARM_VSB_ENERGY:
			status = DEMOD_M14_Analog_VideoProc_VSB_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_MAIN_CUTOFF:
			status = DEMOD_M14_Analog_VideoProc_Cutoff_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_CARR_OFFS:
			status = DEMOD_M14_Analog_VideoProc_CarrierOffset_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_API_VER:
			pParamValue[0] = LX_ADEMOD_VER;
			break;

		case LX_ADEMOD_PARM_I2C_WRITE_COUNT:
			status = I2C_CounterGet(0, pParamValue);
			break;

		case LX_ADEMOD_PARM_I2C_WRITE_OPTIM:
			status = DEMOD_M14_Analog_I2C_Optimization_Get(hDevice, pParamValue);
			break;

		case LX_ADEMOD_PARM_FW_VER:
			{
				UINT8 major_version = 0;
				UINT8 minor_version = 0;
				UINT8 build = 0;
				UINT8 revision = 0;
				// Read Software Version - format VBYTE2.BYTE1.BYTE0
				ADEMOD_ReadByte(pInstance->common.hUserData, RESERVED_TEST1, &major_version);
				ADEMOD_ReadByte(pInstance->common.hUserData, RESERVED_TEST2, &minor_version);
				ADEMOD_ReadByte(pInstance->common.hUserData, RESERVED_TEST3, &build);
				ADEMOD_ReadByte(pInstance->common.hUserData, RESERVED_TEST4, &revision);
				pParamValue[0] = (major_version) +
								 (minor_version << 8) +
								 (build << 16) +
								 (revision << 24);
			}
			break;

		case LX_ADEMOD_PARM_AUD_SIF_BW:
			status = DEMOD_M14_Analog_AudioProc_SifBw_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_DEVIATION:
			status = DEMOD_M14_Analog_AudioProc_Deviation_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE:
			status = DEMOD_M14_Analog_AudioProc_BBSampleRate_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_DEEMPHASIS:
			status = DEMOD_M14_Analog_AudioProc_Deemphasis_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_BSBNDEXTRAGAIN:
			status = DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_SIF_FRQ:
			status = DEMOD_M14_Analog_AudioProc_SifFreq_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_MONO_FRQ:
			status = DEMOD_M14_Analog_AudioProc_MonoFreq_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_MONO_GAIN:
			status = DEMOD_M14_Analog_AudioProc_MonoGain_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_OPTIONS:
			status = DEMOD_M14_Analog_AudioProc_Options_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_TARG:
			status = DEMOD_M14_Analog_AudioProc_AGCTarget_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_GAIN_THR:
			status = DEMOD_M14_Analog_AudioProc_AGCGainThresh_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_AGC_GAIN_PRESET:
			status = DEMOD_M14_Analog_AudioProc_AGCGainPreset_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_MIX:
			status = DEMOD_M14_Analog_AudioProc_Mix_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_AUTOMUTE:
			status = DEMOD_M14_Analog_AudioProc_Mute_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_MUTESPEED:
			status = DEMOD_M14_Analog_AudioProc_MuteSpeed_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AUD_BUZZDELAY:
			status = DEMOD_M14_Analog_AudioProc_BuzzDelay_Get(&pInstance->audio, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_MODE:
			status = DEMOD_M14_Analog_AGC_MixedMode_Get(&pInstance->agc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET:
			status = DEMOD_M14_Analog_DigAgc_Target_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_DECIMATION_RATIO:
			status = DEMOD_M14_Analog_DigAgc_DecimationRatio_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_THR:
			status = DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Get(&pInstance->digagc, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_DIGAGC_NBND_OFFS:
			status = DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Get(&pInstance->video, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_FASTMODE:
			status =  DEMOD_M14_Analog_Bsp_FastAGC_Get(&pInstance->bsp, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_OUTMODE:
			status =  DEMOD_M14_Analog_Bsp_OutAGC_Get(&pInstance->bsp, pParamValue, pInstance->common.hUserData);
			break;

		case LX_ADEMOD_PARM_AGC_WANDMODE:
			status =  DEMOD_M14_Analog_Bsp_WandAGC_Get(&pInstance->bsp, pParamValue, pInstance->common.hUserData);
			break;

		default:
			break;

	}

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Set
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see FmLg_AGC_Descriptor).
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or FmLg_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	status = DEMOD_M14_Analog_AGC_Parameters_Set(&pInstance->agc, pDescriptor, pInstance->common.hUserData, UpdateNow);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Get
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see FmLg_AGC_Descriptor).
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Get(Handle_t hDevice, UINT32* pDescriptor)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	status = DEMOD_M14_Analog_AGC_Parameters_Get(&pInstance->agc, pDescriptor, pInstance->common.hUserData);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_ChanScan_Set
**
**  Description:    Sets parameters for channel scan mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the channel scan descriptor,
**						where pDescriptor[0] - mode: 1 - enabled, 0 - disabled,
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or FmLg_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_ChanScan_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if (pDescriptor[0] != 0) { // channel scan enabled
		pInstance->flags |= LX_ADEMOD_FFLAG_CHAN_SCAN_EN;
	}
	else { // channel scan disabled
		pInstance->flags &= ~LX_ADEMOD_FFLAG_CHAN_SCAN_EN;
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_ChanScan_Get
**
**  Description:    Returns the channel scan status.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_ChanScan_Get(Handle_t hDevice, UINT32* pDescriptor)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	pDescriptor[0] = (pInstance->flags & LX_ADEMOD_FFLAG_CHAN_SCAN_EN) ? 1 : 0; // channel scan enabled

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_I2C_Optimization_Set
**
**  Description:    Sets parameters for channel scan mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pValue		 - Pointer to the I2C optimization descriptor, where:
**						where pValue[0] - mode: 1 - enabled, 0 - disabled.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_I2C_Optimization_Set(Handle_t hDevice, UINT32* pValue)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

#if LX_ADEMOD_REGISTER_CACHE_ENABLED

	if (pValue[0] != 0) { // i2c optimization enabled
		UINT32 cache_options = 0;
		RegisterCacheOptionsGet(&cache_options);
		if (cache_options & LX_ADEMOD_CACHE_OPTIONS_OPTIM) { // optimization permanently enabled
			pInstance->flags |= LX_ADEMOD_FFLAG_I2C_OPTIM_EN; // remember in a dedicated flag
		}
		else {
			// enable i2c optimization temporarily
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_WRITE;
			cache_options |= LX_ADEMOD_CACHE_OPTIONS_OPTIM;
			RegisterCacheOptionsSet(cache_options);
		}
	}
	else { // // i2c optimization disabled
		if ((pInstance->flags & LX_ADEMOD_FFLAG_I2C_OPTIM_EN) == 0) { // optimization was off before scan
			UINT32 cache_options = 0;
			// disable i2c optimization
			RegisterCacheOptionsGet(&cache_options);
			cache_options &= ~LX_ADEMOD_CACHE_OPTIONS_WRITE;
			cache_options &= ~LX_ADEMOD_CACHE_OPTIONS_OPTIM;
			RegisterCacheOptionsSet(cache_options);
		}
	}

#endif

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_I2C_Optimization_Get
**
**  Description:    Returns the I2C optimization status.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pValue		 - Pointer to the I2C optimization descriptor, where:
**						where pValue[0] - mode: 1 - enabled, 0 - disabled.
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_I2C_Optimization_Get(Handle_t hDevice, UINT32* pValue)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	pValue[0] = (pInstance->flags & LX_ADEMOD_FFLAG_I2C_OPTIM_EN) ? 1 : 0; // I2C optimization enabled

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_SleepMode_Set
**
**  Description:    Toggles the sleep mode on and off.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  Mode         - The sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**                      retErrBadMode   - Invalid mode
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_SleepMode_Set(Handle_t hDevice, UINT32 Mode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	switch (Mode) {
		case LX_ADEMOD_SLEEP_MODE_NONE:
			if (pInstance->common.sleep_mode != LX_ADEMOD_SLEEP_MODE_NONE) {
				// wake up
				status = DEMOD_M14_Analog_Bsp_Sleep_Set(&pInstance->bsp, Mode, pInstance->common.hUserData);
				pInstance->common.sleep_mode = LX_ADEMOD_SLEEP_MODE_NONE;
			}
			break;
		case LX_ADEMOD_SLEEP_MODE_LEVEL_1:
			if (pInstance->common.sleep_mode != LX_ADEMOD_SLEEP_MODE_LEVEL_1) {
				// low-power mode 1
				status = DEMOD_M14_Analog_Bsp_Sleep_Set(&pInstance->bsp, Mode, pInstance->common.hUserData);
				pInstance->common.sleep_mode = LX_ADEMOD_SLEEP_MODE_LEVEL_1;
			}
			break;
		default:
			status = retErrBadMode;
			break;
	}

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_SleepMode_Get
**
**  Description:    Returns the sleep mode status.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pMode        - Pointer to the sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_SleepMode_Get(Handle_t hDevice, UINT32 *pMode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	*pMode = pInstance->common.sleep_mode;

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Preset
**
**  Description:    Reads preset of parameters from specified file.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pFile        - Preset file name.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Preset(Handle_t hDevice, char* pFile)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if (!pFile) {
		return retErrBadPtr;
	}

	// add file parsing here

    return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Context_Set
**
**  Description:    Set the Context for Fm1150
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Context_Set(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

    return status;
}

/*
** Static functions implementation
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Context
**
**  Description:    Performs casting from Device handle to Demodulator instance.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  ppLX_ADEMOD_instance - Pointer to the instance to be returned.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Context(Handle_t hDevice, LX_ADEMOD_M14_Instance** ppLX_ADEMOD_instance)
{
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if (!hDevice) {
		return retErrBadHandle;
	}

	pInstance = (pLX_ADEMOD_M14_Instance)hDevice;


	*ppLX_ADEMOD_instance = pInstance;

	RegisterCacheBufferSet((void*)pInstance->reg_cache);
	RegisterCacheFlagsSet((void*)pInstance->reg_cache_flags);

    return retOK;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Factory_Clear
**
**  Description:    Clears factory settings.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_Clear(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	// Set the factory flags before applying factory settings.
	// This is required in order to differentiate between the user-defined parameters and factory settings.
	// NOTE: User-defined parameters (for example GUI XML preset files) always take precedence over factory settings.
	pInstance->legacy.factory			= 0;
	pInstance->bsp.factory				= 0;
	pInstance->video.factory			= 0;
	pInstance->audio.factory			= 0;
	pInstance->low_if.common.factory	= 0;
	pInstance->agc.factory				= 0;
	pInstance->agc.common.factory		= 0;
	pInstance->digagc.factory			= 0;

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Factory_Set
**
**  Description:    Initializes demodulator's context with factory settings.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_Set(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;
	const LX_ADEMOD_FACTORY_SETTINGS *factorySettings = NULL;
	UINT32 Size = 0;
	UINT32 i, j;
	UINT32 value[LX_ADEMOD_MAX_PARAM_SIZE];
	UINT32 param_id;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);
	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}
	DEMOD_PRINT("LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) = 0x%x \n", LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id));

	// Set the factory flags before applying factory settings.
	// This is required in order to differentiate between the user-defined parameters and factory settings.
	// NOTE: User-defined parameters (for example GUI XML preset files) always take precedence over factory settings.
	pInstance->legacy.factory			|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->bsp.factory				|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->video.factory			|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->audio.factory			|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->low_if.common.factory	|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->agc.factory				|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->agc.common.factory		|= LX_ADEMOD_FFLAG_UPDATE;
	pInstance->digagc.factory			|= LX_ADEMOD_FFLAG_UPDATE;

	if ((LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) & LX_ADEMOD_DEMOD_DEBUG_A2_FLAG) ||
		(LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) & LX_ADEMOD_DEMOD_deBug_1_FLAG))
	{ // A2 flag set
		pInstance->legacy.factory			|= LX_ADEMOD_FFLAG_A2;
		pInstance->bsp.factory				|= LX_ADEMOD_FFLAG_A2;
		pInstance->video.factory			|= LX_ADEMOD_FFLAG_A2;
		pInstance->audio.factory			|= LX_ADEMOD_FFLAG_A2;
		pInstance->low_if.common.factory	|= LX_ADEMOD_FFLAG_A2;
		pInstance->agc.factory				|= LX_ADEMOD_FFLAG_A2;
		pInstance->agc.common.factory		|= LX_ADEMOD_FFLAG_A2;
		pInstance->digagc.factory			|= LX_ADEMOD_FFLAG_A2;
	}

	if (LX_ADEMOD_DEVICE_DEBUG(pInstance->common.device_id) & LX_ADEMOD_DEMOD_deBug_1_FLAG)
	{ // A2 flag set
		pInstance->legacy.factory			|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->bsp.factory				|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->video.factory			|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->audio.factory			|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->low_if.common.factory	|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->agc.factory				|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->agc.common.factory		|= LX_ADEMOD_FFLAG_DBG_1;
		pInstance->digagc.factory			|= LX_ADEMOD_FFLAG_DBG_1;
	}


	// Call factory settings
	status = DEMOD_M14_Analog_Factory_Table_Get(&pInstance->common, &factorySettings, &Size);

	for (i = 0; i < Size; i++) {
		memset(value, 0, sizeof(value));
		param_id = (UINT32)((*factorySettings)[i][0]);
		switch (param_id)
		{
			case LX_ADEMOD_PARM_DIGAGC_GAIN_THR:
				// 5- byte values
				value[0] = (UINT32) ((*factorySettings)[i][1]);
				value[1] = (UINT32) ((*factorySettings)[i][2]);
 				value[2] = (UINT32)(((*factorySettings)[i][3] & 0xffffffff));
 				value[3] = (UINT32)(((*factorySettings)[i][3] >> 32));
 				value[4] = (UINT32)(((*factorySettings)[i][4] & 0xffffffff));
 				value[5] = (UINT32)(((*factorySettings)[i][4] >> 32));
				break;
			case LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET:
				// 5- byte values
				value[0] = (UINT32) ((*factorySettings)[i][1]);
				value[1] = (UINT32) ((*factorySettings)[i][2]);
 				value[2] = (UINT32)(((*factorySettings)[i][3] & 0xffffffff));
 				value[3] = (UINT32)(((*factorySettings)[i][3] >> 32));
				break;
			case LX_ADEMOD_REGISTER_SET:
				// skip
				break;
			default:
				for (j = 0; j < LX_ADEMOD_MAX_PARAM_SIZE-1; j++) {
					value[j] = (UINT32)((*factorySettings)[i][j+1]);
					// some parameters are int64, TBD
				}
				break;
		}


		ADEMOD_Parameter_Set(hDevice, param_id, value, FALSE);
	}

	// Clear factory flags at the end of factory settings update
	pInstance->legacy.factory			&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->bsp.factory				&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->video.factory			&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->audio.factory			&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->low_if.common.factory	&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->agc.factory				&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->agc.common.factory		&= ~LX_ADEMOD_FFLAG_UPDATE;
	pInstance->digagc.factory			&= ~LX_ADEMOD_FFLAG_UPDATE;

    DEMOD_PRINT( "[ABB M14](%s:%d) end  !!!\n",__F__,__L__);

    return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Factory_RegisterSet
**
**  Description:    Initializes demodulator's selected registers with factory settings.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Factory_RegisterSet(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;
	const LX_ADEMOD_FACTORY_SETTINGS *factorySettings = NULL;
	UINT32 Size = 0;
	UINT32 i;
	UINT32 value[LX_ADEMOD_MAX_PARAM_SIZE];
	UINT32 param_id;
	UINT32 reg_addr;
	UINT32 reg_size;
	UINT32 reg_data;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	// Call factory settings
	status = DEMOD_M14_Analog_Factory_Table_Get(&pInstance->common, &factorySettings, &Size);

	for (i = 0; i < Size; i++) {
		memset(value, 0, sizeof(value));
		param_id = (UINT32)((*factorySettings)[i][0]);
		switch (param_id)
		{
			case LX_ADEMOD_REGISTER_SET:
				// NOTE: 5-byte registers need special handling TBD
				reg_addr = (UINT32)((*factorySettings)[i][1]);
				reg_data = (UINT32)((*factorySettings)[i][2]);
				reg_size = (UINT32)((*factorySettings)[i][3]) / 8;
				ADEMOD_Register_Set(hDevice, reg_addr, reg_size, reg_data);
				break;
			default:
				// skip
				break;
		}
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Update_Clear
**
**  Description:    Clears user's update "sticky" bits.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Update_Clear(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}
	// clear cache flags
	memset(pInstance->reg_cache_flags, 0, sizeof(pInstance->reg_cache_flags));

	// instance has just been opened, call cold-start initialization.
	status = DEMOD_M14_Analog_VideoProc_Factory_Set(&pInstance->video);
	status = DEMOD_M14_Analog_Bsp_Factory_Set(&pInstance->bsp);
	status = DEMOD_M14_Analog_AGC_Factory_Set(&pInstance->agc);
	status = DEMOD_M14_Analog_DigAGC_Factory_Set(&pInstance->digagc);
	status = DEMOD_M14_Analog_LegacyDemod_Factory_Set(&pInstance->legacy);
	status = DEMOD_M14_Analog_AudioProc_Factory_Set(&pInstance->audio);
	status = DEMOD_M14_Analog_LowIF_Factory_Set(&pInstance->low_if);

	// set UPDATE flag and clear all user flags
	pInstance->legacy.update		= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->bsp.update			= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->video.update			= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->video.update1		= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->audio.update			= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->low_if.common.update = LX_ADEMOD_UFLAG_UPDATE;
	pInstance->agc.update			= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->agc.common.update	= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->agc.rf.update		= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->agc.rf_dac.update	= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->agc.if_dac.update	= LX_ADEMOD_UFLAG_UPDATE;
	pInstance->digagc.update		= LX_ADEMOD_UFLAG_UPDATE;

	// clear all the feature flags
	pInstance->legacy.flags = 0;
	pInstance->bsp.flags = LX_ADEMOD_CONFIG_FLAGS;
	pInstance->video.flags = LX_ADEMOD_VID_CTRL_FLAGS_FCT;
	pInstance->audio.flags = LX_ADEMOD_AUD_CTRL_FLAGS_FCT;
	pInstance->low_if.common.flags = LX_ADEMOD_LOWIF_CTRL_FLAGS_FCT;
	pInstance->agc.flags = 0;
	pInstance->digagc.flags = 0;


    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Initialize
**
**  Description:    Initializes LGT10 hardware.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Initialize(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	status = DEMOD_M14_Analog_Bsp_Init(&pInstance->bsp, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_VideoProc_Init(&pInstance->video, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_AudioProc_Init(&pInstance->audio, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_LegacyDemod_Init(&pInstance->legacy, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_LowIF_Disable(&pInstance->low_if, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_AGC_Init(&pInstance->agc, pInstance->common.hUserData);

	status = DEMOD_M14_Analog_DigAGC_Init(&pInstance->digagc, pInstance->common.hUserData);

	// Power all clocks down except Micro.
	// Micro's clock should be running for firmware download.
	// Appropriate clocks will be powered up when mode is set.
	// status = LX_ADEMOD_Bsp_Clock_Set(&pInstance->bsp, LX_ADEMOD_ALL_CLK_POWER_DOWN & ~LX_ADEMOD_MICRO_CLK_POWER_DOWN, pInstance->common.hUserData);

    return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Start
**
**  Description:    Starts demodulation.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Start(Handle_t hDevice)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_M14_Instance pInstance = NULL;
	UINT32 video_freq[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)+1]; // add 1 for sampling rate
	UINT32 video_flags;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if ((status = DEMOD_M14_Analog_Context(hDevice, &pInstance)) != retOK) {
		return status;
	}

	status = DEMOD_M14_Analog_VideoProc_Start(&pInstance->video, pInstance->common.hUserData);
	status = DEMOD_M14_Analog_AGC_Start(&pInstance->agc, pInstance->common.hUserData);
	status = DEMOD_M14_Analog_DigAGC_Start(&pInstance->digagc, pInstance->common.hUserData);

	switch (pInstance->common.input_mode) {
		case LX_ADEMOD_INPUTMODE_LEGACY:
			status = DEMOD_M14_Analog_LowIF_Disable(&pInstance->low_if, pInstance->common.hUserData);
			status = DEMOD_M14_Analog_VideoProc_InputFreq_GetX(&pInstance->video, video_freq, pInstance->common.hUserData);
			status = DEMOD_M14_Analog_VideoProc_Flags_Get(&pInstance->video, &video_flags);
			status = DEMOD_M14_Analog_LegacyDemod_Start(&pInstance->legacy, video_freq, video_flags, pInstance->common.hUserData);
			status = DEMOD_M14_Analog_AudioProc_Start(&pInstance->audio, video_freq, video_flags, pInstance->common.hUserData);
		break;

		case LX_ADEMOD_INPUTMODE_DVBT:
			status = DEMOD_M14_Analog_AudioProc_Init(&pInstance->audio, pInstance->common.hUserData);
			status = DEMOD_M14_Analog_LowIF_Enable(&pInstance->low_if, pInstance->common.hUserData);
		break;

		case LX_ADEMOD_INPUTMODE_ATSC:
			// mode is not supported yet
		default:
			status = retErrBadParam;
		break;

	}

	status = DEMOD_M14_Analog_Bsp_Start(&pInstance->bsp, pInstance->common.hUserData);
    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

    return status;
}



