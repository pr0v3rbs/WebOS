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
**  Name: ademod_common_UserUtilities.h
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
** 					LX_ADEMOD_Result Video_BandMode_Set
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


#ifndef _ADEMOD_COMMON_USERUITILITIES_H_
#define _ADEMOD_COMMON_USERUITILITIES_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ademod_common_demod.h"

// Register
#define LX_ADEMOD_MAX_REGISTERS		   (600)
#define LX_ADEMOD_MAX_REGISTER_NAME		(50)
#define LX_ADEMOD_MAX_OPTION_LENGTH		(80)
#define LX_ADEMOD_MAX_PARAM_ATTR			(80)


#ifndef toupper
#define toupper( c ) ( ((c) >= 'a') && ((c) <= 'z') ? (c) - 'a' + 'A' : (c) )
#endif

typedef char args[LX_ADEMOD_MAX_OPTION_LENGTH];
typedef struct LX_ADEMOD_Register_tag {
	char			name[LX_ADEMOD_MAX_REGISTER_NAME];
	UINT32			addr;
	UINT32			size;
}  LX_ADEMOD_Register, *pLX_ADEMOD_Register;

// AFT descriptor
typedef struct LX_ADEMOD_AFTDescriptor_tag {
	int freq_offs;
	UINT32 aft;
} LX_ADEMOD_AFTDescriptor, *pLX_ADEMOD_AFTDescriptor;

// Filter cut-off descriptor
typedef struct LX_ADEMOD_FilterCutoffDescriptor_tag {
	UINT32 sample_rate;
	UINT32 freq_cutoff;
} LX_ADEMOD_FilterCutoffDescriptor, *pLX_ADEMOD_FilterCutoffDescriptor;

#if 0
/****************************************************************************************************
**  Function:      @^Device_Interface_Set
**
**  Description:   @^Set device Interface.
**
**  Mnemonic:      @^sint
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - interface ID: 1- Parallel, 2- USB, 3- Fast Usb, 4- Server USB.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Device_Interface_Set(Handle_t hDemod, UINT32* val);
#endif

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
LX_ADEMOD_Result AFT_Auto_Adjust(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Carr_Offset_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Gain_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Gain_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Offs_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Offs_Set(Handle_t hDemod,  UINT32* val);
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
LX_ADEMOD_Result Video_Clips_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Clips_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_LockRange_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_LockRange_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_SampleRate_Get(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Video_SampleRate_Set
**
**  Description:   @^Set Sampling Rate.
**
**  Mnemonic:      @^srange
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
LX_ADEMOD_Result Video_SampleRate_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Backoff_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Backoff_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Target_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Target_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_DigitalGain_Get(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Video_DigitalGain_Set
**
**  Description:   @^Set digital gain in Narrowband mode.
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
LX_ADEMOD_Result Video_DigitalGain_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_BandMode_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_BandMode_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Agc_Speed_Get(Handle_t hDemod,  UINT32* val);

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
**						val[1] - dec factor.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Agc_Speed_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Agc_DigitalSpeed_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Agc_DigitalSpeed_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_SIF_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_Mode_Get(Handle_t hDemod,  UINT32* val);

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
**						val[0] - cut-off.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_SIFFreq_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_SIFFreq_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_SIFOut_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_SIFOut_Set(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Audio_Mono_Set
**
**  Description:   @^Set Audio mode Mono.
**
**  Mnemonic:      @^mono
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms   - parameter string.
**                 @^val - array of in/out values, TBD
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Audio_Mono_Set(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Audio_MonoCutoff_Get
**
**  Description:   @^Get Audio SIF frequency cut-off.
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
LX_ADEMOD_Result Audio_MonoCutoff_Get(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Audio_MonoCutoff_Set
**
**  Description:   @^Set Audio SIF frequency.
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
LX_ADEMOD_Result Audio_MonoCutoff_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_Gain_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Audio_Offset_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result  Audio_Offset_Set(Handle_t hDemod,  UINT32* val);


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
LX_ADEMOD_Result  Audio_Gain_Set(Handle_t hDemod,  UINT32* val);



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
LX_ADEMOD_Result RegisterRead(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^RegisterWrite
**
**  Description:   @^Writes to a register.
**
**  Mnemonic:      @^rw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
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
LX_ADEMOD_Result RegisterWrite(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^RegisterWrite
**
**  Description:   @^Dumps registers into a file.
**
**  Mnemonic:      @^dump
**
**  Parameters:    @^hDemod - Demodulator's handle,
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result RegisterDump(Handle_t hDemod);

/****************************************************************************************************
**  Function:      @^OpenRegisterLog
**
**  Description:   @^Opens register log file.
**
**  Mnemonic:      @^logopen
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
LX_ADEMOD_Result OpenRegisterLog(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^CloseRegisterLog
**
**  Description:   @^Closes register log file.
**
**  Mnemonic:      @^logclose
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
LX_ADEMOD_Result CloseRegisterLog(Handle_t hDemod,  UINT32* val);
#if 0
/****************************************************************************************************
**  Function:      @^ParameterRead
**
**  Description:   @^Reads parameter.
**
**  Mnemonic:      @^pr
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - parameter id,
**						val[1..(size - 1)] - parameter attributes (returned),
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result ParameterRead(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^ParameterWrite
**
**  Description:   @^Writes to a parameter.
**
**  Mnemonic:      @^pw
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - parameter id,
**						val[1..(size - 1)] - parameter attributes,
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result ParameterWrite(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^ParameterDump
**
**  Description:   @^Dumps API parameters into a file.
**
**  Mnemonic:      @^pdump
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
LX_ADEMOD_Result ParameterDump(Handle_t hDemod,  UINT32* val);
#endif

/****************************************************************************************************
**  Function:      @^I2C_Optimization
**
**  Description:   @^Enables/disables I2C optimization.
**
**  Mnemonic:      @^i2cop
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - 1 - i2c optimization enabled, 0 - disabled.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result I2C_Optimization(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^I2C_Count_Reset
**
**  Description:   @^Resets I2C counter.
**
**  Mnemonic:      @^i2clr
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
LX_ADEMOD_Result I2C_Count_Reset(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^I2C_Count_Get
**
**  Description:   @^Returns I2C counter.
**
**  Mnemonic:      @^i2cget
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms  - parameter string.
**                 @^val - array of in/out values, where
**						val[0] - i2c counter.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result I2C_Count_Get(Handle_t hDemod,  UINT32* val);

/****************************************************************************************************
**  Function:      @^Lock_Status_Get
**
**  Description:   @^Returns lock status.
**
**  Mnemonic:      @^lock
**
**  Parameters:    @^hDemod - Demodulator's handle,
**                 @^parms - parameter string,
**                 @^val - array of in/out values, where
**						val[0] - out lock status, 1 - locked, 0 - unlocked.
**
**  Returns:       status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  @@
****************************************************************************************************/
LX_ADEMOD_Result Lock_Status_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Communication_Status_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Sync_Status_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Pll_Status_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result IF_Status_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Bandwidth_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Video_Bandwidth_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Lif_Bandwidth_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Lif_Bandwidth_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result IF_Freq_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result IF_Freq_Set(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Lif_CF_Get(Handle_t hDemod,  UINT32* val);

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
LX_ADEMOD_Result Lif_CF_Set(Handle_t hDemod,  UINT32* val);

/****************************************************
**  Housekeeping utilities
****************************************************/

/****************************************************************************************************
**  Function:      SplitArg
**
**  Description:   Splits argument string into sub-strings.
**
**  Mnemonic:
**
**  Parameters:    parms  - parameter string,
**				   argv   - pointer to array of substrings,
**				   count  - input - max number of arguments, output - actual number of arguments
**
**  Returns:       status:
**                      retOK           - No errors
**                      retUserSizeTooBig - Too many arguments
**
****************************************************************************************************/
LX_ADEMOD_Result SplitArg( args* argv, int* count);

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
void atoi64(UINT64 *value, char *str, int base);


#ifdef __cplusplus
}
#endif
#endif //_ADEMOD_COMMON_USERUITILITIES_H_
