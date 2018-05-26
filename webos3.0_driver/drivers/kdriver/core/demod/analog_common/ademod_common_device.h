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
**  Name: ademod_common_device.h
**
**  Description:    ABB generic device software interface.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_COMMON_DEVICE_H_
#define _ADEMOD_COMMON_DEVICE_H_


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

//#ifndef Handle_t
//typedef void *          Handle_t;
//#endif
#include "ademod_common_demod.h"	// *** CHANGED BY ALDO ***

/*
** Analog video standard enumeration compliant with DirectShow standard.
*/
typedef enum LX_ADEMOD_AnalogVideoStandard_tag {
    AnalogVideo_None       = 0x00000000,
    AnalogVideo_NTSC_M     = 0x00000001,
    AnalogVideo_NTSC_M_J   = 0x00000002,
    AnalogVideo_NTSC_433   = 0x00000004,
    AnalogVideo_PAL_B      = 0x00000010,
    AnalogVideo_PAL_D      = 0x00000020,
    AnalogVideo_PAL_H      = 0x00000080,
    AnalogVideo_PAL_I      = 0x00000100,
    AnalogVideo_PAL_M      = 0x00000200,
    AnalogVideo_PAL_N      = 0x00000400,
    AnalogVideo_PAL_60     = 0x00000800,
    AnalogVideo_SECAM_B    = 0x00001000,
    AnalogVideo_SECAM_D    = 0x00002000,
    AnalogVideo_SECAM_G    = 0x00004000,
    AnalogVideo_SECAM_H    = 0x00008000,
    AnalogVideo_SECAM_K    = 0x00010000,
    AnalogVideo_SECAM_K1   = 0x00020000,
    AnalogVideo_SECAM_L    = 0x00040000,
    AnalogVideo_SECAM_L1   = 0x00080000,
    AnalogVideo_PAL_N_COMBO = 0x00100000
} LX_ADEMOD_AnalogVideoStandard;

/*
** Signal input.
*/
typedef enum LX_ADEMOD_SignalInputType_tag {
    LX_ADEMOD_SignalInputAntenna,
    LX_ADEMOD_SignalInputCable,
} LX_ADEMOD_SignalInputType;

/*
** Signal source.
*/
typedef enum LX_ADEMOD_SignalSourceType_tag {
    LX_ADEMOD_SignalSourceDigital,
    LX_ADEMOD_SignalSourceAnalog
} LX_ADEMOD_SignalSourceType;

/*
** Interface.
*/

typedef enum LX_ADEMOD_DeviceInterfaceType_tag {
	LX_ADEMOD_DeviceInterfaceNone,
    LX_ADEMOD_DeviceInterfaceParallel,
    LX_ADEMOD_DeviceInterfaceUSB
} LX_ADEMOD_DeviceInterfaceType;

typedef struct LX_ADEMOD_DeviceHeader_tag {
	Handle_t handle;
	unsigned int device_addr;
	unsigned int device_id;
	unsigned int interface_id;
	Handle_t h_silent;
	unsigned int size;
} LX_ADEMOD_DeviceHeader, *pADEMOD_DeviceHeader;

typedef struct ADEMOD_InterfaceParallel_tag {
	unsigned int pin_scl;
	unsigned int pin_sda_in;
	unsigned int pin_sda_out;
	unsigned int pin_led;
	unsigned int pol_scl;
	unsigned int pol_sda_in;
	unsigned int pol_sda_out;
	unsigned int pol_led;
} LX_ADEMOD_InterfaceParallel, *pADEMOD_InterfaceParallel;

typedef struct ADEMOD_InterfaceUSB_tag {
	unsigned int tbd1;
	unsigned int tbd2;
} LX_ADEMOD_InterfaceUSB, *pADEMOD_InterfaceUSB;

#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_COMMON_DEVICE_H_
