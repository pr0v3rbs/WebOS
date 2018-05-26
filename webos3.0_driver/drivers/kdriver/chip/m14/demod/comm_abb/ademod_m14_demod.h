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
**  Name: ademod_m14_demod.h
**
**  Description:    ABB M14 Demodulator software interface.
**
**  Functions
**  Implemented:    FmLg_Result  DEMOD_M14_Analog_Demod_Open
**                  FmLg_Result  DEMOD_M14_Analog_Demod_Close
**                  FmLg_Result  DEMOD_M14_Analog_Demodulate
**                  FmLg_Result  DEMOD_M14_Analog_Parameter_Set
**                  FmLg_Result  DEMOD_M14_Analog_Parameter_Get
**                  FmLg_Result  DEMOD_M14_Analog_AGC_Set
**                  FmLg_Result  DEMOD_M14_Analog_AGC_Get
**                  FmLg_Result  DEMOD_M14_Analog_ChanScan_Set
**                  FmLg_Result  DEMOD_M14_Analog_ChanScan_Get
**                  FmLg_Result  DEMOD_M14_Analog_SleepMode_Set
**                  FmLg_Result  DEMOD_M14_Analog_SleepMode_Get
**                  FmLg_Result  DEMOD_M14_Analog_HostI2C_Write
**                  FmLg_Result  DEMOD_M14_Analog_HostI2C_Read
**
**  References:     
**
**  Exports:        FmLg_Result  DEMOD_M14_Analog_Demod_Open
**                  FmLg_Result  DEMOD_M14_Analog_Demod_Close
**                  FmLg_Result  DEMOD_M14_Analog_Demodulate
**                  FmLg_Result  DEMOD_M14_Analog_Parameter_Set
**                  FmLg_Result  DEMOD_M14_Analog_Parameter_Get
**                  FmLg_Result  DEMOD_M14_Analog_AGC_Set
**                  FmLg_Result  DEMOD_M14_Analog_AGC_Get
**                  FmLg_Result  DEMOD_M14_Analog_ChanScan_Set
**                  FmLg_Result  DEMOD_M14_Analog_ChanScan_Get
**                  FmLg_Result  DEMOD_M14_Analog_SleepMode_Set
**                  FmLg_Result  DEMOD_M14_Analog_SleepMode_Get
**                  FmLg_Result  DEMOD_M14_Analog_HostI2C_Write
**                  FmLg_Result  DEMOD_M14_Analog_HostI2C_Read
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**                  ademod_common_userdef.h  - for interfaces defined by the user,
**                                  such as ReadData, WriteData, Sleep.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_M14_DEMOD_H_
#define _ADEMOD_M14_DEMOD_H_

#include "ademod_common.h"
#include "ademod_common_userdef.h"
#include "ademod_common_demod.h"



#include "ademod_m14_video.h"
#include "ademod_m14_audio.h"
#include "ademod_m14_legacy.h"
#include "ademod_m14_lowif.h"
#include "ademod_m14_bsp.h"
#include "ademod_m14_agc.h"
#include "ademod_m14_agc_dig.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  LGT10 Demodulator instance
*/
typedef struct LX_ADEMOD_M14_Instance_tag
{
    LX_ADEMOD_Demod_Instance common; /* NOTE: this must be the first item in the structure */
	// Add hardware specific attributes here
	LX_ADEMOD_VideoProc	video;
	LX_ADEMOD_AudioProc	audio;
	LX_ADEMOD_LegacyDemod	legacy;
	LX_ADEMOD_M14_LowIF		low_if;
	LX_ADEMOD_Bsp			bsp;
	LX_ADEMOD_Agc			agc;
	LX_ADEMOD_DigAgc		digagc;
#if LX_ADEMOD_REGISTER_CACHE_ENABLED
	UINT8				reg_cache[MAX_REG_CACHE_SIZE]; // reg cache for debug purpose
	UINT8				reg_cache_flags[MAX_REG_CACHE_SIZE]; // reg cache flags for debug purpose
#endif
	UINT32				flags;
}  LX_ADEMOD_M14_Instance, *pLX_ADEMOD_M14_Instance;


/*
**  Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_I2C_OPTIM_EN			(0x00000001L) // 1 = I2C writes optimization enabled, 0 = disabled
#define LX_ADEMOD_FFLAG_CHAN_SCAN_EN			(0x00000002L) // 1 = channel sacan enabled, 0 = disabled


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
					          Handle_t*  phDevice );


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
LX_ADEMOD_Result DEMOD_M14_Analog_Demod_Close(Handle_t hDevice);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Demodulate
**
**  Description:    Starts demodulation in specified mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  InputMode    - The input mode. Can be one of the following:
**                                 LX_ADEMOD_INPUTMODE_LEGACY,
**                                 LX_ADEMOD_INPUTMODE_DVBT,
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
LX_ADEMOD_Result DEMOD_M14_Analog_Demodulate( Handle_t hDevice,
							 UINT32 InputMode,
							 UINT32 VideoStandard,
							 UINT32 AudioStandard);


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
LX_ADEMOD_Result DEMOD_M14_Analog_Parameter_Set( Handle_t hDevice,
							    UINT32 ParamID,
								UINT32* pParamValue,
								BOOL UpdateNow);


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
LX_ADEMOD_Result DEMOD_M14_Analog_Parameter_Get( Handle_t hDevice,
						        UINT32 ParamID,
							    UINT32* pParamValue);


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Set( Handle_t hDevice,
						  UINT32* pDescriptor,
						  BOOL UpdateNow);


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Get( Handle_t hDevice,
						  UINT32* pDescriptor);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_ChanScan_Set
**
**  Description:    Sets the channel scan profile for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
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
LX_ADEMOD_Result DEMOD_M14_Analog_ChanScan_Set( Handle_t hDevice,
							   UINT32* pDescriptor,
							   BOOL UpdateNow);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_ChanScan_Get
**
**  Description:    Returns the channel scan status for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_ChanScan_Get( Handle_t hDevice,
							   UINT32* pDescriptor);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_SleepMode_Set
**
**  Description:    Toggles the sleep mode on and off.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  OnOff        - When TRUE the sleep mode will be enabled;
**                                 When FALSE the normal mode will be restored.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_SleepMode_Set( Handle_t hDevice,
							    UINT32 Mode);


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
LX_ADEMOD_Result DEMOD_M14_Analog_SleepMode_Get( Handle_t hDevice,
							    UINT32 *pMode);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Preset
**
**  Description:    Reads preset of parameters from specified file.
**
**  Parameters:     hDevice      - Device Handle returned by FmLg_Demod_Open,
**                  pFile        - Preset file name or NULL.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
**  Note: When parameter file is not specified, then parameters will be
**        initialized with appropriate defaults.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Preset(Handle_t hDevice, char* pFile);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Context_Set(Handle_t hDevice);

#if defined( __cplusplus )
}
#endif


#endif  //_ADEMOD_M14_DEMOD_H_
