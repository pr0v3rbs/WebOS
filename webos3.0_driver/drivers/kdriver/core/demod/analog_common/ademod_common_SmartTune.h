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
**  Name: ademod_common_SmartTune.h
**
**  Description:   ABB  API example. SmartTune table descriptor.
**
**  Dependencies:   TBD.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_COMMON_SMARTTUNE_H_
#define _ADEMOD_COMMON_SMARTTUNE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
**  Filter
**  A bitmap:  bits 0-27: Video format,
**             bit    28: Analog mode,
**             bit    29: Digital mode,
**             bit    30: Antenna,
**             bit    31: Cable.
*/
typedef enum LX_ADEMOD_SmartTuneFilter_tag
{
    LX_ADEMOD_SmartTuneFilterMode		= 0x0FFFFFFF,
    LX_ADEMOD_SmartTuneFilterAnalog	= 0x10000000,
    LX_ADEMOD_SmartTuneFilterDigital	= 0x20000000,
    LX_ADEMOD_SmartTuneFilterAntenna	= 0x40000000,
    LX_ADEMOD_SmartTuneFilterCable	= 0x80000000,
} LX_ADEMOD_SmartTuneFilter;

// SmartTune descriptor
typedef struct LX_ADEMOD_SmartTuneDescriptor_tag {
	UINT32 Freq;		// Picture Carrier
	UINT32 SmplRate;	// Sampling rate
	UINT32 IF;			// IF
	UINT32 Id;			// Id
    UINT32 Filter;		// A bitmap: bits 0-27 - Video format, bits 28-29: 0-Analog Antenna, 1-Digital Antenna, 2-Analog Cable, 3-Digital Cable
                        // bits 30-31 - Rerserved.
	UINT32 Update;      // 0-Update not required, 1- Update required
	UINT32 Options;     // a bitmap: bit 0: 24 MHz enable, bit1: Clock dithering enable
} LX_ADEMOD_SmartTuneDescriptor, *pSmartTuneDescriptor;

// SmartTune LUT Entry
typedef struct LX_ADEMOD_SmartTuneEntry_tag {
	UINT32 LoFreq;		// Low threshold of the frequency interval
	UINT32 HiFreq;		// High threshold of the frequency interval
	UINT32 SmplRate;	// Sampling rate
	UINT32 IF;			// IF
    UINT32 Filter;		// A bitmap: bits 0-27 - Video format, bits 28-29: 0-Analog Antenna, 1-Digital Antenna, 2-Analog Cable, 3-Digital Cable
                        // bits 30-31 - Rerserved.
} LX_ADEMOD_SmartTuneEntry, *pSmartTuneEntry;


#ifdef __cplusplus
}
#endif

#endif /* _ADEMOD_COMMON_SMARTTUNE_H_ */
