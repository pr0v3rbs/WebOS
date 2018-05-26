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
**  Name: ademod_common_UserUtilities.c
**
**  Description:    User Utilities functions.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  AFT_Auto_Adjust
**					LX_ADEMOD_Result  Carr_Offset_Get
**					LX_ADEMOD_Result  Video_Gain_Get
**					LX_ADEMOD_Result  Video_Offs_Get
**					LX_ADEMOD_Result  Video_Offs_Get
**					LX_ADEMOD_Result  Video_Offs_Set
**					LX_ADEMOD_Result  Video_BandMode_Get
**					LX_ADEMOD_Result  Video_LockRange_Get
**					LX_ADEMOD_Result  Video_LockRange_Set
**					LX_ADEMOD_Result  Video_SampleRate_Get
**					LX_ADEMOD_Result  Video_SampleRate_Set
**					LX_ADEMOD_Result  Video_Backoff_Get
**					LX_ADEMOD_Result  Video_Backoff_Set
**					LX_ADEMOD_Result  Video_Target_Get
**					LX_ADEMOD_Result  Video_Target_Set
**					LX_ADEMOD_Result  Video_DigitalGain_Get
**					LX_ADEMOD_Result  Video_DigitalGain_Set
**					LX_ADEMOD_Result  Agc_Speed_Get
**					LX_ADEMOD_Result  Agc_Speed_Set
**					LX_ADEMOD_Result  Agc_DigitalSpeed_Get
**					LX_ADEMOD_Result  Agc_DigitalSpeed_Set
**					LX_ADEMOD_Result  Audio_SIF_Set
**					LX_ADEMOD_Result  Audio_Mode_Get
**					LX_ADEMOD_Result  Audio_SIFFreq_Get
**					LX_ADEMOD_Result  Audio_SIFFreq_Set
**					LX_ADEMOD_Result  Audio_SIFOut_Get
**					LX_ADEMOD_Result  Audio_SIFOut_Set
**					LX_ADEMOD_Result  Audio_Mono_Set
**					LX_ADEMOD_Result  Audio_MonoCutoff_Get
**					LX_ADEMOD_Result  Audio_MonoCutoff_Set
**					LX_ADEMOD_Result  Audio_Gain_Get
**					LX_ADEMOD_Result  Audio_Gain_Set
**					LX_ADEMOD_Result  Audio_Offset_Get
**					LX_ADEMOD_Result  Audio_Offset_Set
**					LX_ADEMOD_Result  RegisterDump
**					LX_ADEMOD_Result  RegisterRead
**					LX_ADEMOD_Result  RegisterWrite
**					LX_ADEMOD_Result  OpenRegisterLog
**					LX_ADEMOD_Result  CloseRegisterLog
**					LX_ADEMOD_Result  I2C_Optimization
**					LX_ADEMOD_Result  I2C_Count_Reset
**					LX_ADEMOD_Result  I2C_Count_Get
**					LX_ADEMOD_Result  Lock_Status_Get
**					LX_ADEMOD_Result  Communication_Status_Get
**					LX_ADEMOD_Result  Sync_Status_Get
**					LX_ADEMOD_Result  Pll_Status_Get
**					LX_ADEMOD_Result  RF_Status_Get
**					LX_ADEMOD_Result  IF_Status_Get
**					LX_ADEMOD_Result  Video_Bandwidth_Get
**					LX_ADEMOD_Result  Video_Bandwidth_Set
**					LX_ADEMOD_Result  Lif_Bandwidth_Get
**					LX_ADEMOD_Result  Lif_Bandwidth_Set
**					LX_ADEMOD_Result  IF_Freq_Get
**					LX_ADEMOD_Result  IF_Freq_Set
**					LX_ADEMOD_Result  Lif_CF_Get
**					LX_ADEMOD_Result  Lif_CF_Set
**
**  References:     
**
**  Exports:        
**
**  Dependencies:   ademod_common_demod.h for basic types.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE (1)
#if defined( __cplusplus )
extern "C"
{
#endif

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

#include "ademod_common.h"
#include "ademod_common_Menu.h"
#include "ademod_common_Presets.h"
#include "ademod_common_UserUtilities.h"
#include "ademod_common_Regtable.h"
#include "ademod_common_Paramtable.h"
#include "ademod_common_userdef.h"

extern LX_ADEMOD_REGENTRY *ADEMOD_REGS;
extern LX_ADEMOD_PARMENTRY *ADEMOD_PARAMS;

#define LX_ADEMOD_AFT_MAX_STEPS (10)

static LX_ADEMOD_FilterCutoffDescriptor CutoffLut[] = {{207,18000},{176,15250},{172,15000},{168,14600},{161,14000},{149,13000},{138,12000},{126,11000},{115,10000},{103,9000},{92,8000},{80,7000},{60,5250}};



/****************************************************************************************************
**  Function:      @^AFT_Auto_Adjust
**
**  Description:   @^Gets AFT offset and Adjust Tuner
**
**  Mnemonic:      @^aft
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - frequency offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result AFT_Auto_Adjust(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	int freq_offset = 0;



	UINT32 value1[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_LOCK_STAT)];
	UINT32 value2[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_SYNC_STAT)];

	ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_LOCK_STAT, value1 );
	if (value1[0]) // Are we locked?
	{
		DEMOD_DEBUG_PRINT("Carrier locked!\n\r");
		Carr_Offset_Get(hDemod, (UINT32 *)&freq_offset);

		ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_SYNC_STAT, value2 ); // Get Sync Status
		DEMOD_DEBUG_PRINT("Sync Status (0x%X):  ", value2[0]);

		switch (value2[0]&03)
		{
			case 0:
				DEMOD_DEBUG_PRINT("HSync unlocked, VSync unlocked\n\r");
				break;
			case 1:
				DEMOD_DEBUG_PRINT("HSync locked, VSync unlocked\n\r");
				break;
			case 2:
				DEMOD_DEBUG_PRINT("HSync unlocked, VSync locked\n\r");
				break;
			case 3:
				DEMOD_DEBUG_PRINT("HSync locked, VSync locked\n\r");
				break;
		#if 0
			default:
				DEMOD_DEBUG_PRINT("HSync unstable, VSync unstable\n\r");
				break;
		#endif
		
		}
	}
	else
	{
		DEMOD_DEBUG_PRINT("UNLOCKED\n\r");
	}
	DEMOD_DEBUG_PRINT("\n\r");

	return status;
}

/****************************************************************************************************
**  Function:      @^Carr_Offset_Get
**
**  Description:   @^Get carrier offset.
**
**  Mnemonic:      @^coffs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - frequency offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Carr_Offset_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_CARR_OFFS)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_CARR_OFFS, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("carrier offset: %d[KHz]\n", value[0]);

	return status;
}

/****************************************************************************************************
**  Function:      @^Video_Gain_Get
**
**  Description:   @^Get Video Gain.
**
**  Mnemonic:      @^gain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - gain.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Gain_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_GAIN)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_GAIN_RC)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_GAIN, value);
	if (val != NULL) {
		val[1] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Scart gain: %d(0x%x)\n", value[0],value[0]);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_GAIN_RC, RcValue);

	val[1] = RcValue[1];

	DEMOD_DEBUG_PRINT("RC mode : %s, RC gain: %d(0x%x)\n", (RcValue[0] = 1) ? "RC" : "INTPL",RcValue[1],RcValue[1]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Gain_Set
**
**  Description:   @^Set Video Gain.
**
**  Mnemonic:      @^sgain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - gain.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Gain_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_GAIN)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_GAIN_RC)];

	if (val != NULL) {
		value[0] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	if(val[0] == 0)
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_GAIN, value, TRUE);
	else
	{
		if(val[0] == 1) RcValue[0] = 1;
		else RcValue[0] = 0;

		RcValue[1] = val[1];
		
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_GAIN_RC, RcValue, TRUE);
	}	

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Offs_Get
**
**  Description:   @^Get Video Offset.
**
**  Mnemonic:      @^offs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Offs_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS_RC)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_OFFS, value);

	if (val != NULL) {
		val[1] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Scart mode : offset: %d\n", value[0]);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_OFFS_RC, RcValue);

	val[1] = RcValue[0];

	DEMOD_DEBUG_PRINT("RC mode : %s,  offset: %d(0x%x)\n", (RcValue[0] == 1) ? "RC" : "INTPL",RcValue[1],RcValue[1]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Offs_Set
**
**  Description:   @^Set Video Offset.
**
**  Mnemonic:      @^soffs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Offs_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS_RC)];

	if (val != NULL) {
		value[0] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	if(val[0] == 0)
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_OFFS, value, TRUE);
	else
	{
		if(val[0] == 1) RcValue[0] = 1;
		else RcValue[0] = 0;

		RcValue[1] = val[1];
		
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_OFFS_RC, RcValue, TRUE);
	}	

	return status;
}

/****************************************************************************************************
**  Function:      @^Video_Clips_Get
**
**  Description:   @^Get Video cliff.
**
**  Mnemonic:      @^offs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Clips_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_CLIP)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_CLIP_RC)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_CLIP, value);

	if (val != NULL) {
		val[1] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Scart mode : Clip set: %d\n", value[0]);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_CLIP_RC, RcValue);

	val[1] = RcValue[0];

	DEMOD_DEBUG_PRINT("RC mode : %s,  Clip set: %d(0x%x)\n", (RcValue[0] == 1) ? "RC" : "INTPL",RcValue[1],RcValue[1]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Offs_Set
**
**  Description:   @^Set Video Clip.
**
**  Mnemonic:      @^soffs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Clips_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS)];
	UINT32 RcValue[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_OFFS_RC)];

	if (val != NULL) {
		value[0] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	if(val[0] == 0)
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_CLIP, value, TRUE);
	else
	{
		if(val[0] == 1) RcValue[0] = 1;
		else RcValue[0] = 0;

		RcValue[1] = val[1];
		
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_CLIP_RC, RcValue, TRUE);
	}	

	return status;
}



/****************************************************************************************************
**  Function:      @^Video_BandMode_Get
**
**  Description:   @^Get Power Sense (band) mode.
**
**  Mnemonic:      @^band
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - band mode: 0 - wide band, 1 - mixed, 2 - narroe band.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_BandMode_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_NBND_MODE)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_NBND_MODE, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	switch (value[0]) {
	case 0: DEMOD_DEBUG_PRINT("mode: wideband\n"); break;
	case 1: DEMOD_DEBUG_PRINT("mode: mixed\n"); break;
	case 2: DEMOD_DEBUG_PRINT("mode: narrowband\n"); break;
	case 3: DEMOD_DEBUG_PRINT("mode: Hybrid\n"); break;
	default: DEMOD_DEBUG_PRINT("mode: unknown\n"); break;
	}

	return status;
}

/****************************************************************************************************
**  Function:      @^Video_BandMode_Set
**
**  Description:   @^Get Power Sense (band) mode.
**
**  Mnemonic:      @^band
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - band mode: 0 - wide band, 1 - mixed, 2 - narroe band.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_BandMode_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_NBND_MODE)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);


	if (val != NULL) {
		value[0] = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_NBND_MODE, value, TRUE);

	switch (value[0]) {

	case 0: DEMOD_DEBUG_PRINT("Set mode: wideband\n"); break;
	case 1: DEMOD_DEBUG_PRINT("Set mode: mixed\n"); break;
	case 2: DEMOD_DEBUG_PRINT("Set mode: narrowband\n"); break;
	case 3: DEMOD_DEBUG_PRINT("Set mode: Hybrid\n"); break;
	default: DEMOD_DEBUG_PRINT("Set mode: unknown\n"); break;
	}

	return status;
}



/****************************************************************************************************
**  Function:      @^Video_LockRange_Get
**
**  Description:   @^Get Lock Range.
**
**  Mnemonic:      @^range
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - min range (Hz),
**						val[1] - max range (Hz).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_LockRange_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VIDCR_RANGE)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VIDCR_RANGE, value);

	if (val != NULL) {
		val[0] = value[1];
 		val[1] = value[2];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("min: %d [Hz], max: %d [Hz]\n", value[1], value[2]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_LockRange_Set
**
**  Description:   @^Set Lock Range.
**
**  Mnemonic:      @^srange
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - min range (Hz),
**						val[1] - max range (Hz).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_LockRange_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VIDCR_RANGE)];

	if (val != NULL) {
		value[0] = 0; // direct setting
		value[1] = val[0];
		value[2] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VIDCR_RANGE, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_SampleRate_Get
**
**  Description:   @^Get Sampling Rate.
**
**  Mnemonic:      @^srate
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Sample Rate (Hz),
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_SampleRate_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_SMPL_RATE)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_SMPL_RATE, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("sample rate: %d [Hz]\n", value[0]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_SampleRate_Set
**
**  Description:   @^Set Sampling Rate.
**
**  Mnemonic:      @^ssrate
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Sample Rate (Hz),
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_SampleRate_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_SMPL_RATE)];

	if (val != NULL) {
		value[0] = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_SMPL_RATE, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Backoff_Get
**
**  Description:   @^Get Backoff.
**
**  Mnemonic:      @^backoff
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Backoff (dB * 10).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Backoff_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AGC_BACKOFF)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AGC_BACKOFF, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("backoff: %d (%d[dB])\n", value[0], value[0]/10);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Backoff_Set
**
**  Description:   @^Set Backoff.
**
**  Mnemonic:      @^sbackoff
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Backoff (dB * 10).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Backoff_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AGC_BACKOFF)];

	if (val != NULL) {
		value[0] = 0; // direct set
 		value[1] = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AGC_BACKOFF, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Target_Get
**
**  Description:   @^Get Hybrid mode target.
**
**  Mnemonic:      @^targ
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - target (dB * 10).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Target_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("target: %d\n", value[0]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_Target_Set
**
**  Description:   @^Set Hybrid mode target.
**
**  Mnemonic:      @^starg
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - target (dB * 10).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Target_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
#if 0
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET)];


	if (val != NULL) {
		value[0] = 0; // direct set
 		value[1] = val[0];
	}

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_HYBR_TARGET, value, TRUE);
#endif

	return status;
}
/****************************************************************************************************
**  Function:      @^Video_DigitalGain_Get
**
**  Description:   @^Get Digital Gain in Narrowband mode.
**
**  Mnemonic:      @^dgain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Min Gain LSB,
**						val[1] - Min Gain MSB,
**						val[2] - Max Gain LSB,
**						val[3] - Max Gain MSB.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_DigitalGain_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_GAIN_THR)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	value[0] = LX_ADEMOD_AGC_MODE_RF;
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_THR, value);


	DEMOD_DEBUG_PRINT(" \n << LX_ADEMOD_AGC_MODE_RF >>\n");
	DEMOD_DEBUG_PRINT(" gain: min - 0x%llx(%lld), max - 0x%llx(%lld)\n", value[2] + ((UINT64)value[3] << 32), value[2] + ((UINT64)value[3] << 32), value[4] + ((UINT64)value[5] << 32), value[4] + ((UINT64)value[5] << 32));

	value[0] = LX_ADEMOD_AGC_MODE_IF;
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_THR, value);


	DEMOD_DEBUG_PRINT(" \n << LX_ADEMOD_AGC_MODE_IF >>\n");
	DEMOD_DEBUG_PRINT(" gain: min - 0x%llx(%lld), max - 0x%llx(%lld)\n", value[2] + ((UINT64)value[3] << 32), value[2] + ((UINT64)value[3] << 32), value[4] + ((UINT64)value[5] << 32), value[4] + ((UINT64)value[5] << 32));

	value[0] = LX_ADEMOD_AGC_MODE_DIG;
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_THR, value);


	DEMOD_DEBUG_PRINT(" \n << LX_ADEMOD_AGC_MODE_DIG >>\n");
	DEMOD_DEBUG_PRINT(" gain: min - 0x%llx(%lld), max - 0x%llx(%lld)\n", value[2] + ((UINT64)value[3] << 32), value[2] + ((UINT64)value[3] << 32), value[4] + ((UINT64)value[5] << 32), value[4] + ((UINT64)value[5] << 32));


	value[0] = LX_ADEMOD_AGC_MODE_DIG_NBND;
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_THR, value);

	if (val != NULL) {
		val[0] = value[2];
		val[1] = value[3];
		val[2] = value[4];
		val[3] = value[5];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT(" \n << LX_ADEMOD_AGC_MODE_DIG_NBND >>\n");
	DEMOD_DEBUG_PRINT(" gain: min - 0x%llx(%lld), max - 0x%llx(%lld)\n", value[2] + ((UINT64)value[3] << 32), value[2] + ((UINT64)value[3] << 32), value[4] + ((UINT64)value[5] << 32), value[4] + ((UINT64)value[5] << 32));

	return status;
}


/****************************************************************************************************
**  Function:      @^Video_DigitalGain_Set
**
**  Description:   @^Set digital gain in Narrowband mode. Params: <Min Gain LSB> <Min Gain MSB> <Max Gain LSB> <Max Gain MSB>
**
**  Mnemonic:      @^sdgain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Min Gain LSB,
**						val[1] - Min Gain MSB,
**						val[2] - Max Gain LSB,
**						val[3] - Max Gain MSB.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_DigitalGain_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_GAIN_THR)];


	if (val != NULL) {
		value[0] = val[0];
		value[1] = val[1]; 
		value[2] = val[2];
		value[3] = val[3];
		value[4] = val[4];
		value[5] = val[5];
	}
	else
	{
		return retErrBadPtr;
	}	

	// program threshold
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_THR, value, TRUE);
	// program preset
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Agc_Speed_Get
**
**  Description:   @^Get Agc Speed.
**
**  Mnemonic:      @^speed
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - agc speed.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Agc_Speed_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AGC_SPEED)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AGC_SPEED, value);

	if (val != NULL) {
		val[0] = value[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("agc speed: poserr_RF - %d, negerr_RF - %d, poserr_IF - %d, negerr_IF - %d\n", value[1], value[2], value[3], value[4]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Agc_Speed_Set
**
**  Description:   @^Set Agc Speed.
**
**  Mnemonic:      @^sspeed
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - agc speed,
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Agc_Speed_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 speed = 0;
	UINT32 mode = 0;

	if (val != NULL) {
		speed     = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	// program speed
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_NBND_MODE, &mode);
	if (mode == LX_ADEMOD_AGC_LPF_MODE_WIDEBAND) {
		UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AGC_SPEED)];
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AGC_SPEED, value);
		value[0] = 0; // direct mode
		value[1] = speed;
		value[2] = speed;
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AGC_SPEED, value, TRUE);
	}
	else if (mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID) {
		UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR)];
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR, value);
		value[0] = 0; // direct mode
		value[1] = speed;
		value[2] = speed;
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR, value, TRUE);
	}
	else {
		UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_SPEED)];
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED, value);
		value[0] = 0; // direct mode
		value[1] = speed;
		value[2] = speed;
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED, value, TRUE);
	}

	return status;
}


/****************************************************************************************************
**  Function:      @^Agc_DigitalSpeed_Get
**
**  Description:   @^Get Agc Speed.
**
**  Mnemonic:      @^digspeed
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - digital agc speed RF,
**						val[1] - digital agc speed IF,
**						val[2] - digital agc agc speed Digital.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Agc_DigitalSpeed_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_SPEED)];
	UINT32 mode = 0;

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_NBND_MODE, &mode);

	if (mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID) {
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR, value);
	}
	else {
		UINT32 cvbs_mode = 0;
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE, &cvbs_mode);
		if (cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT) {
			status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_COH, value);
		}
		else { // non-coherent
			status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED, value);
		}
	}

	if (val != NULL) {
		val[0] = value[1];
		val[1] = value[3];
		val[2] = value[5];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("digital agc speed: RF - %d, IF - %d, Digital - %d\n", value[1], value[3], value[5]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Agc_DigitalSpeed_Set
**
**  Description:   @^Set Digital Agc Speed.
**
**  Mnemonic:      @^sdigspeed
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - digital agc speed RF,
**						val[1] - digital agc speed IF,
**						val[2] - digital agc agc speed Digital.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Agc_DigitalSpeed_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_DIGAGC_SPEED)];
	UINT32 speed_rf = 0;
	UINT32 speed_if = 0;
	UINT32 speed_digital = 0;
	UINT32 mode = 0;


	if (val != NULL) {
		speed_rf      = val[0];
		speed_if      = val[1];
		speed_digital = val[2];
	}
	else
	{
		return retErrBadPtr;
	}	
	value[0] = 0; // direct mode
	value[1] = speed_rf;
	value[2] = speed_rf;
	value[3] = speed_if;
	value[4] = speed_if;
	value[5] = speed_digital;
	value[6] = speed_digital;
	// program speed
	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_NBND_MODE, &mode);
	if (mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID) {
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_HYBR, value, TRUE);
	}
	else {
		UINT32 cvbs_mode = 0;
		status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_DIGAGC_COHERENT_MODE, &cvbs_mode);
		if (cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT) {
			status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED_COH, value, TRUE);
		}
		else { // non-coherent
			status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_DIGAGC_SPEED, value, TRUE);
		}
	}

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_SIF_Set
**
**  Description:   @^Set Audio mode SIF.
**
**  Mnemonic:      @^sif
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIF_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_MODE)];

	value[0] = LX_ADEMOD_AUDIO_MODE_SIF;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_MODE, value, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^Audio_Mode_Get
**
**  Description:   @^Gets Audio mode SIF/Mono.
**
**  Mnemonic:      @^audmode
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - audio mode: 1- Mono, 6- SIF.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_Mode_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_MODE)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_MODE, value);

    if (val != NULL) {
		val[0] = value[0];
    }
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("audio mode: %s\n", (value[0] == LX_ADEMOD_AUDIO_MODE_SIF) ? "SIF" : "Mono");

	return status;
}

/****************************************************************************************************
**  Function:      @^Audio_SIFFreq_Get
**
**  Description:   @^Get Audio SIF frequency cut-off.
**
**  Mnemonic:      @^sfr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Sif Lower Freq.
**						val[1] - Sif Upper Freq.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIFFreq_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_SIF_FRQ)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_SIF_FRQ, value);

	if (val != NULL) {
		val[0] = value[0];
		val[1] = value[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("sif freq: low - %d (0x%x) [Hz], high - %d (0x%x) [Hz]\n", value[0],value[0], value[1], value[1]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_SIFFreq_Set
**
**  Description:   @^Set Audio SIF frequency.
**
**  Mnemonic:      @^ssfr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Sif Lower Freq.
**						val[1] - Sif Upper Freq.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIFFreq_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_SIF_FRQ)];


	if (val != NULL) {
		value[0] = val[0];
		value[1] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_SIF_FRQ, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_SIFOut_Get
**
**  Description:   @^Get Audio SIF output.
**
**  Mnemonic:      @^sout
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - sif output.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIFOut_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_SIF_OUT)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Register_Get(hDemod, 0x935,2, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("sif out: %d\n", value[0]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_SIFOut_Set
**
**  Description:   @^Set Audio SIF output.
**
**  Mnemonic:      @^ssout
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - sif output: 0 - 320mV, 1 - 400mV, 2 - 530mV, 3 - 800mV;
**						val[1] - Audio mode: 1- mono, 6- SIF
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIFOut_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_SIF_OUT)];

	if (val != NULL) {
		value[0] = val[0];
		value[1] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_SIF_OUT, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_Mono_Set
**
**  Description:   @^Set Audio mode Mono.
**
**  Mnemonic:      @^mono
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_Mono_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_MODE)];

	value[0] = LX_ADEMOD_AUDIO_MODE_MONO;
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_MODE, value, TRUE);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_MonoCutoff_Get
**
**  Description:   @^Get Audio Mono frequency cut-off.
**
**  Mnemonic:      @^cutoff
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Cutoff Freq (Hz).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_MonoCutoff_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE)];
	UINT32 cutoff = 5250; // Hz - lowest value in the table
	UINT32 i = 0;

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE, value);

	// translate sample rate value into cutoff frequency using CutoffLut
	for (i = 0; i < sizeof(CutoffLut) / sizeof(LX_ADEMOD_FilterCutoffDescriptor); i++) {
		if (value[0] >= CutoffLut[i].sample_rate) {
			cutoff = CutoffLut[i].freq_cutoff;
			break;
		}
	}

	if (val != NULL) {
		val[0] = cutoff;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("filter cut-off: %d\n", cutoff);

	return status;
}


/****************************************************************************************************
**  Function:      @^Audio_MonoCutoff_Set
**
**  Description:   @^Set Audio Mono frequency cut-off.
**
**  Mnemonic:      @^scutoff
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Cutoff Freq (Hz).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_MonoCutoff_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE)] = {60};
	UINT32 cutoff = 0;
	UINT32 i = 0;


	if (val != NULL) {
		cutoff = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	// translate cutoff frequency into sample rate value using CutoffLut
	for (i = 0; i < sizeof(CutoffLut) / sizeof(LX_ADEMOD_FilterCutoffDescriptor); i++) {
		if (cutoff >= CutoffLut[i].freq_cutoff) {
			value[0] = CutoffLut[i].sample_rate;
			break;
		}
	}

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_BBSAMPLE_RATE, value, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^Audio_Gain_Get
**
**  Description:   @^Get Audio Gain.
**
**  Mnemonic:      @^gain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - gain.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_Gain_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_GAIN)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_GAIN, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Audio gain: %d (0x%x)\n", value[0], value[0]);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_GAIN_RC, value);

	val[0] = value[0];

	DEMOD_DEBUG_PRINT("Audio RC gain: %d (0x%x)\n", value[0], value[0]);


	return status;
}


/****************************************************************************************************
**  Function:      @^ Audio_Gain_Set
**
**  Description:   @^Set  Audio Gain.
**
**  Mnemonic:      @^sgain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - gain.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result  Audio_Gain_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_GAIN)];

	if (val != NULL) {
		value[0] = val[1];
		value[1] = 0; //SIF
	}
	else
	{
		return retErrBadPtr;
	}	

	if(val[0] == 0)
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_GAIN, value, TRUE);
	else
	{

		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_GAIN_RC, value, TRUE);
	}	

	return status;
}

/****************************************************************************************************
**  Function:      @^Audio_Offset_Get
**
**  Description:   @^Get Audio Offset.
**
**  Mnemonic:      @^Offset
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - Offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_Offset_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_OFFS)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_OFFS, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Audio offset : %d (0x%x)\n", value[0], value[0]);

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_AUD_OFFS_RC, value);
	val[0] = value[0];

	DEMOD_DEBUG_PRINT("Audio offset : %d (0x%x)\n", value[0], value[0]);

	return status;
}


/****************************************************************************************************
**  Function:      @^ Audio_Offset_Set
**
**  Description:   @^Set  Audio Offset.
**
**  Mnemonic:      @^sgain
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - offset.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result  Audio_Offset_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_OFFS)];

	if (val != NULL) {
		value[0] = val[1];
	}
	else
	{
		return retErrBadPtr;
	}	

	if(val[0] == 0)
		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_OFFS, value, TRUE);
	else
	{

		status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_AUD_OFFS_RC, value, TRUE);
	}	

	return status;
}



/****************************************************************************************************
**  Function:      @^RegisterDump
**
**  Description:   @^Dumps registers into a file.
**
**  Mnemonic:      @^dump [options] [file]
**
**  Parameters:    @^hDemod - Demodulator's handle,
**
**	Options:	   -f		- filter.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result RegisterDump(Handle_t hDemod)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_REGENTRY *reg = NULL;

	DEMOD_DEBUG_PRINT ("LG3100 ABB Register Dump\n");
	DEMOD_DEBUG_PRINT ("Copyright (c) 2013 SIC Lab. LG Electronics \n\n");
	DEMOD_DEBUG_PRINT ("Addr  Value	   Mnemonic\n");

	for (reg = (LX_ADEMOD_REGENTRY*)&ADEMOD_REGS; reg->desc != NULL; reg++)
	{
		if (reg->size < LX_ADEMOD_MAX_REGISTER_LENGTH) {
			UINT32 reg_val = 0;
			status = ADEMOD_Register_Get(hDemod, reg->addr, reg->size, &reg_val);
			DEMOD_DEBUG_PRINT ("0x%4.4X	0x%2.2X  %s %s\n", reg->addr, reg_val, reg->desc, (status == retOK) ? "" : "read failed");
		}
		else {
			UINT32 reg_val = 0;
			UINT32 msb = 0;
			// 5-byte registers require special treatment
			status = ADEMOD_Register_Get(hDemod, reg->addr, 4, &reg_val);
			// Read MSByte as a separate transaction
			status = ADEMOD_Register_Get(hDemod, reg->addr + 4, reg->size - LX_ADEMOD_MAX_REGISTER_LENGTH + 1, &msb);
			DEMOD_DEBUG_PRINT ("0x%4.4X	0x%X%2.2X  %s %s\n", reg->addr, msb, reg_val, reg->desc, (status == retOK) ? "" : "read failed");
		}
	}

	return status;
}

/****************************************************************************************************
**  Function:      @^RegisterRead
**
**  Description:   @^Reads register.
**
**  Mnemonic:      @^rr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - register addr,
**						val[1] - register value lsb (returned),
**						val[2] - register value msb (returned),
**						val[3] - register size (returned).
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result RegisterRead(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_REGENTRY *reg = NULL;
	int reg_addr = -1;
	BOOL found = FALSE;


	if (val != NULL) {
		reg_addr = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	


	if (reg_addr >= 0) {
		// search by address
		for (reg = (LX_ADEMOD_REGENTRY*)&ADEMOD_REGS; reg->desc != NULL; reg++)
		{
			if (reg->addr == (UINT32)reg_addr) {
				found = TRUE;
				break;
			}
		}
	}

	if (found) {

		if (reg->size < LX_ADEMOD_MAX_REGISTER_LENGTH) {
			UINT32 reg_val = 0;
			status = ADEMOD_Register_Get(hDemod, reg->addr, reg->size, &reg_val);
			val[0] = reg->addr;
			val[1] = reg_val;
			val[2] = 0;
			val[3] = reg->size;
			DEMOD_DEBUG_PRINT ("0x%4.4X  0x%2.2X  %s %s\n", reg->addr, reg_val, reg->desc, (status == retOK) ? "" : "read failed");
		}
		else {
			UINT32 reg_val = 0;
			UINT32 msb = 0;
			// 5-byte registers require special treatment
			status = ADEMOD_Register_Get(hDemod, reg->addr, 4, &reg_val);
			// Read MSByte as a separate transaction
			status = ADEMOD_Register_Get(hDemod, reg->addr + 4, reg->size - LX_ADEMOD_MAX_REGISTER_LENGTH + 1, &msb);
			val[0] = reg->addr;
			val[1] = reg_val;
			val[2] = msb;
			val[3] = reg->size;
			DEMOD_DEBUG_PRINT ("0x%4.4X  0x%X%2.2X  %s %s\n", reg->addr, msb, reg_val, reg->desc, (status == retOK) ? "" : "read failed");
		}
	}
	else {
		status = retErr;
	}

	return status;
}

/****************************************************************************************************
**  Function:      @^RegisterWrite
**
**  Description:   @^Writes to a register.
**
**  Mnemonic:      @^rw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, where
**						val[0] - register addr,
**						val[1] - register value lsb,
**						val[2] - register value msb.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result RegisterWrite(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_REGENTRY *reg = NULL;
	UINT64 reg_val = 0;
	char str[80];
	int reg_addr = -1;
	BOOL found = FALSE;

	if (val != NULL) {
		reg_addr = val[0];
		reg_val  = val[1] + ((UINT64)val[2] << 32);
	}
	else
	{
		return retErrBadParam;
	}

	if (reg_addr >= 0) {
		// search by address
		for (reg = (LX_ADEMOD_REGENTRY*)&ADEMOD_REGS; reg->desc != NULL ; reg++)
		{
			if (reg->addr == (UINT32)reg_addr) {
				found = TRUE;
				break;
			}
		}
	}
	if (found) {
		if (reg->size < LX_ADEMOD_MAX_REGISTER_LENGTH)
		{
			status = ADEMOD_Register_SetX(hDemod, reg->addr, reg->size, (UINT32)reg_val, TRUE);
			DEMOD_DEBUG_PRINT ("0x%4.4X  0x%2.2X  %s %s\n", reg->addr, (UINT32)reg_val, reg->desc, (status == retOK) ? "" : "write failed");
		}
		else
		{ // 5-byte registers
			status = ADEMOD_Register_SetX(hDemod, reg->addr, 4, (UINT32)reg_val, TRUE);
			status = ADEMOD_Register_SetX(hDemod, reg->addr + 4, reg->size - LX_ADEMOD_MAX_REGISTER_LENGTH + 1, (UINT32)(reg_val >> 32), TRUE);
			DEMOD_DEBUG_PRINT ("0x%4.4X  0x%X%2.2X  %s %s\n", reg->addr, (UINT32)(reg_val >> 32), (UINT32)reg_val, reg->desc, (status == retOK) ? "" : "write failed");
		}
	}
	else {
		DEMOD_DEBUG_PRINT ("register %s not found\n", str);
	}

	return status;
}

/****************************************************************************************************
**  Function:      @^OpenRegisterLog
**
**  Description:   @^Opens register log file.
**
**  Mnemonic:      @^logopen [id]
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result OpenRegisterLog(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_LOG_OPEN)];

	value[0] = 0; // for full log
	value[1] = 1; // default

	if (val != NULL) {
		value[1] = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_LOG_OPEN, value, TRUE);

	return status;
}
/****************************************************************************************************
**  Function:      @^CloseRegisterLog
**
**  Description:   @^Closes register log file.
**
**  Mnemonic:      @^logclose
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result CloseRegisterLog(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_LOG_CLOSE, NULL, TRUE);

	return status;
}
#if 0
/****************************************************************************************************
**  Function:      @^ParameterDump
**
**  Description:   @^Dumps API parameters into a file.
**
**  Mnemonic:      @^pdump
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result ParameterDump(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
//	FILE *outfile;
	LX_ADEMOD_PARMENTRY *par = NULL;
	char *options = NULL;
	UINT32 i = 0;

	DEMOD_DEBUG_PRINT ( "LG3100 Parameter Dump\n");
	DEMOD_DEBUG_PRINT ("Copyright (c) 2013 SIC Lab. LG Electronics \n\n");
	DEMOD_DEBUG_PRINT ( "Id      Size      Mnemonic\n");
	for (par = (LX_ADEMOD_PARMENTRY*)&ADEMOD_PARAMS; strlen(par->desc) != 0; par++)
	{
		UINT32 value[LX_ADEMOD_MAX_PARAM_ATTR];
		UINT32 count = par->size;
		DEMOD_DEBUG_PRINT ( "0x%4.4X  %2d  %s\n", par->id, par->size, par->desc);
		status = ADEMOD_Parameter_Get(hDemod, par->id, value);
		if (status == retOK) {
			args argv[LX_ADEMOD_MAX_PARAM_ATTR];
			status = SplitArg(par->attr, argv, (int *)&count);
			for (i = 0; i < count; i++) {
				if (par->format == 16) {
					DEMOD_DEBUG_PRINT ( "%s\t0x%4.4X\n", argv[i], value[i]);
				}
				else {
					DEMOD_DEBUG_PRINT ( "%s\t%4.4d\n", argv[i], value[i]);
				}
			}
		}
		else {
			DEMOD_DEBUG_PRINT ( "0x%4.4X  %s read failed\n", par->id, par->desc);
		}
	}

//	fclose (outfile);

	return status;
}

/****************************************************************************************************
**  Function:      @^ParameterRead
**
**  Description:   @^Reads parameter.
**
**  Mnemonic:      @^pr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - parameter id,
**						val[1] - parameter size,
**						val[2] - parameter format,
**						val[3] - parameter flags,
**						val[4..(size + 4 - 1)] - parameter attributes (returned),
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  NOTE*:		   Parameter parms contain parameter name + all attribute names as a string.
**                 It is up to caller to allocate memory to accomodate memory buffer long enough
**                 to fit all the attributes description. The worst case scenario would be Group Gelay.
**                 This parameter has 80 coefficients.
**  @@
****************************************************************************************************/
LX_ADEMOD_Result ParameterRead(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_PARMENTRY *par = NULL;
	int parm_id = -1;
	UINT32 i = 0;
	BOOL found = FALSE;

	if (parm_id >= 0) {
		// search by id
		for (par = (LX_ADEMOD_PARMENTRY*)&ADEMOD_PARAMS; strlen(par->desc) != 0; par++)
		{
			if (par->id == (UINT32)parm_id) {
				found = TRUE;
				break;
			}
		}
	}

	if (found) {
		UINT32 value[LX_ADEMOD_MAX_PARAM_ATTR];
		UINT32 count = par->size;
		//strcpy(parms, par->desc);
		status = ADEMOD_Parameter_Get(hDemod, parm_id, value);
		if (status == retOK) {
			args argv[LX_ADEMOD_MAX_PARAM_ATTR];
			status = SplitArg(par->attr, argv, (int *)&count);
			DEMOD_DEBUG_PRINT ("0x%4.4X  %s\n", par->id, par->desc);
			for (i = 0; i < count; i++) {
				if (par->format == 16) {
					DEMOD_DEBUG_PRINT ("%s\t0x%4.4X\n", argv[i], value[i]);
				}
				else {
					DEMOD_DEBUG_PRINT ("%s\t%4.4d\n", argv[i], value[i]);
				}
			}
		}
		else {
			DEMOD_DEBUG_PRINT ("0x%4.4X  %s read failed\n", par->id, par->desc);
		}
		if (val != NULL) {
			val[0] = par->id;
			val[1] = par->size;
			val[2] = par->format;
			val[3] = par->flags;
			for (i = 0; i < count; i++) {
				val[i+4] = value[i];
			}
		}
	}
	else {
		status = retErr;
	}

	return status;
}

/****************************************************************************************************
**  Function:      @^ParameterWrite
**
**  Description:   @^Writes to a parameter.
**
**  Mnemonic:      @^pw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - parameter id,
**						val[1..(size - 1)] - parameter attributes,
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result ParameterWrite(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	LX_ADEMOD_PARMENTRY *par = NULL;
	char str[80];
	int parm_id = -1;
	UINT32 i = 0;
	BOOL found = FALSE;

	if (val != NULL) {
		parm_id = val[0];
	}


	if (parm_id >= 0) {
		// search by address
		for (par = (LX_ADEMOD_PARMENTRY*)&ADEMOD_PARAMS; strlen(par->desc) != 0; par++)
		{
			if (par->id == (UINT32)parm_id) {
				found = TRUE;
				break;
			}
		}
	}

	if (found) {
		UINT32 value[LX_ADEMOD_MAX_PARAM_ATTR];
		UINT32 count = par->size;
		if (val != NULL) {
			for (i = 0; i < count; i++) {
				value[i] = val[i+1];
			}
		}
		// set parameter
		status = ADEMOD_Parameter_Set(hDemod, parm_id, value, TRUE);
	}
	else {
		DEMOD_DEBUG_PRINT ("parameter %s not found\n", str);
	}

	return status;
}
#endif

/****************************************************************************************************
**  Function:      @^I2C_Optimization
**
**  Description:   @^Enables/disables I2C optimization.
**
**  Mnemonic:      @^i2cop
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, where
**						val[0] - 1 - i2c optimization enabled, 0 - disabled.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result I2C_Optimization(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_I2C_WRITE_OPTIM)];

	if (val != NULL) {
		value[0] = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_I2C_WRITE_OPTIM, value, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^I2C_Count_Reset
**
**  Description:   @^Resets I2C counter.
**
**  Mnemonic:      @^i2clr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result I2C_Count_Reset(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_I2C_WRITE_COUNT)];

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_I2C_WRITE_COUNT, value, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^I2C_Count_Get
**
**  Description:   @^Returns I2C counter.
**
**  Mnemonic:      @^i2cget
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - i2c counter.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result I2C_Count_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_I2C_WRITE_COUNT)];

	status = ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_I2C_WRITE_COUNT, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	
	DEMOD_DEBUG_PRINT("I2C write counter = %d\n", value[0]);

	return status;
}


/****************************************************************************************************
**  Function:      @^Lock_Status_Get
**
**  Description:   @^Returns lock status.
**
**  Mnemonic:      @^lock
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                
**                 @^val - array of in/out values, where
**						val[0] - out lock status, 1 - locked, 0 - unlocked.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lock_Status_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 stat = 0;

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_LOCK_STAT, &stat);

	if (val != NULL) {
		val[0] = stat;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("lock status: %s\n", (stat == 1) ? "locked" : "unlocked");

	return status;
}

/****************************************************************************************************
**  Function:      @^Communication_Status_Get
**
**  Description:   @^Returns communication status.
**
**  Mnemonic:      @^com
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - out communication status, 1 - I2C fine, 0 - I2C failed.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Communication_Status_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 stat = 0;

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_ISALIVE_STAT, &stat);

	if (val != NULL) {
		val[0] = stat;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("communication status: %s\n", (stat == 1) ? "responding" : "not responding");

	return status;
}

/****************************************************************************************************
**  Function:      @^Sync_Status_Get
**
**  Description:   @^Returns sync status.
**
**  Mnemonic:      @^sync
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - out sync status, a bitmap defined as follows:
**							bit[0] - 1 - H-Sync locked, 0 - unlocked;
**							bit[1] - 1 - V-Sync locked, 0 - unlocked;
**							bit[2] - 1 - H-Sync stable, 0 - unstable;
**							bit[3] - 1 - V-Sync stable, 0 - unstable;
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Sync_Status_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 stat = 0;

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_SYNC_STAT, &stat);

	if (val != NULL) {
		val[0] = stat;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("H-sync status: %s,%s\n", (stat & 0x1) ? "locked" : "unlocked", (stat & 0x4) ? "stable" : "unstable");
	DEMOD_DEBUG_PRINT("V-sync status: %s,%s\n", (stat & 0x2) ? "locked" : "unlocked", (stat & 0x8) ? "stable" : "unstable");

	return status;
}

/****************************************************************************************************
**  Function:      @^Pll_Status_Get
**
**  Description:   @^Returns PLL status.
**
**  Mnemonic:      @^pll
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - PLL status, 1 - locked, 0 - unlocked.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Pll_Status_Get(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 stat = 0;

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_PLL_STAT, &stat);

	if (val != NULL) {
		val[0] = stat;
	}
	else
	{
		return retErrBadPtr;
	}	
	DEMOD_DEBUG_PRINT("pll status: %s\n", (stat == 1) ? "locked" : "unlocked");

	return status;
}

/****************************************************************************************************
**  Function:      @^RF_Status_Get
**
**  Description:   @^Returns RF level status.
**
**  Mnemonic:      @^rf
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result RF_Status_Get(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;

	//TBD

	return status;
}

/****************************************************************************************************
**  Function:      @^IF_Status_Get
**
**  Description:   @^Returns IF level status.
**
**  Mnemonic:      @^ifs
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result IF_Status_Get(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_IF_LEVEL_STAT)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_IF_LEVEL_STAT, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("IF level: %d\n", value[0]);

	return status;
}

/****************************************************************************************************
**  Function:      @^Video_Bandwidth_Get
**
**  Description:   @^Returns Video Bandwidth.
**
**  Mnemonic:      @^bw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - bandwidth.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Bandwidth_Get(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 bandwidth = 0;

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_BNDW, &bandwidth);

	if (val != NULL) {
		val[0] = bandwidth;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("bandwidth: %d\n", bandwidth);

	return status;
}

/****************************************************************************************************
**  Function:      @^Video_Bandwidth_Set
**
**  Description:   @^Sets Video Bandwidth.
**
**  Mnemonic:      @^sbw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - bandwidth.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Video_Bandwidth_Set(Handle_t hDemod, UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
 	UINT32 bandwidth = 0;

	if (val != NULL) {
		bandwidth = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_BNDW, &bandwidth, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^Lif_Bandwidth_Get
**
**  Description:   @^Returns Lif Bandwidth.
**
**  Mnemonic:      @^lifbw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - bandwidth.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lif_Bandwidth_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 bandwidth = 0;

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_LOWIF_BNDW, &bandwidth);

	if (val != NULL) {
		val[0] = bandwidth;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("lif bandwidth: %d\n", bandwidth);

	return status;
}

/****************************************************************************************************
**  Function:      @^Lif_Bandwidth_Set
**
**  Description:   @^Sets Lif Bandwidth.
**
**  Mnemonic:      @^slifbw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - bandwidth.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lif_Bandwidth_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 bandwidth = 0;

	if (val != NULL) {
		bandwidth = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_LOWIF_BNDW, &bandwidth, TRUE);

	return status;
}
/****************************************************************************************************
**  Function:      @^IF_Freq_Get
**
**  Description:   @^Returns IF frequency.
**
**  Mnemonic:      @^ifr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - if.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result IF_Freq_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 value[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ)];

	DEMOD_DEBUG_PRINT(" \n ====== %s ==========\n",__F__);

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VID_FRQ, value);

	if (val != NULL) {
		val[0] = value[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("IF: %d\n", value[0]);

	return status;
}

/****************************************************************************************************
**  Function:      @^IF_Freq_Set
**
**  Description:   @^Sets IF frequency.
**
**  Mnemonic:      @^setifr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - if.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result IF_Freq_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 if_freq = 0;
	if (val != NULL) {
		if_freq = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	
	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VID_FRQ, &if_freq, TRUE);

	return status;
}

/****************************************************************************************************
**  Function:      @^Lif_CF_Get
**
**  Description:   @^Returns LIF center frequency.
**
**  Mnemonic:      @^lifcf
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - lif CF.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lif_CF_Get(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 lif_cf;

	status |= ADEMOD_Parameter_Get(hDemod, LX_ADEMOD_PARM_VIDCR_OUT_CF, &lif_cf);

	if (val != NULL) {
		val[0] = lif_cf;
	}
	else
	{
		return retErrBadPtr;
	}	

	DEMOD_DEBUG_PRINT("Lif CF: %d\n", lif_cf);

	return status;
}

/****************************************************************************************************
**  Function:      @^Lif_CF_Set
**
**  Description:   @^Sets LIF center frequency.
**
**  Mnemonic:      @^slifcf
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 
**                 @^val - array of in/out values, where
**						val[0] - lif CF.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lif_CF_Set(Handle_t hDemod,  UINT32* val)
{
	LX_ADEMOD_Result status = retOK;
	UINT32 lif_cf;

	if (val != NULL) {
		lif_cf = val[0];
	}
	else
	{
		return retErrBadPtr;
	}	

	status = ADEMOD_Parameter_Set(hDemod, LX_ADEMOD_PARM_VIDCR_OUT_CF, &lif_cf, TRUE);

	return status;
}


/****************************************************
**  Housekeeping utilities
****************************************************/
#if 0
/****************************************************************************************************
**  Function:      SplitArg
**
**  Description:   Splits argument string into sub-strings.
**
**  Mnemonic:
**
**  Parameters:    
**				   argv   - pointer to array of substrings,
**				   count  - input - max number of arguments, output - actual number of arguments
**
**  Returns:       status:
**                      retOK           - No errors
**                      retUserSizeTooBig - Too many arguments
**
****************************************************************************************************/
LX_ADEMOD_Result SplitArg( args* argv, int* count)
{
	LX_ADEMOD_Result status = retOK;
	char p1[2048];
	char* p2 = NULL;
	char* separator = " ";
	int argc = 0;

	memset(p1,0,1024);
//	strcpy(p1, parms);
	p2 = strtok(p1, separator);
	while (p2)
	{
		if (argc >= *count) {
			return retUserSizeTooBig;
		}
		strncpy(argv[argc], p2, LX_ADEMOD_MAX_OPTION_LENGTH);
		p2 = strtok(NULL, " ");
		argc++;
	}

	*count = argc;

	return status;
}
#endif
/****************************************************************************************************
**  Function:      atoi64
**
**  Description:   converts string into 64-bit integer.
**
**  Mnemonic:
**
**  Parameters:    value  - returned value ,
**				   str    - input string,
**				   base   - either 10 or 16
**
**  Returns:       N/A
**
****************************************************************************************************/
void atoi64(UINT64 *value, char *str, int base)
{
	UINT64 val = 0;
	UINT32 i = 0;
	char ch;
	if ((base != 10) && (base != 16)) base = 10;
	for (i = 0; i < strlen(str); i++)
	{
		val *= base;
		ch = str[i];
		if		(ch >= '0' && ch <= '9') val += (ch -'0');
		else if (ch >= 'a' && ch <= 'f') val += (ch -'a' + 10);
		else if (ch >= 'A' && ch <= 'F') val += (ch -'A' + 10);
		else continue; // skip the character
	}
	*value = val;
}




#if defined( __cplusplus )
}
#endif
