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
 *  driver interface header for Mixed IP device. ( used only within kdriver )
 *	Mixed IP device will teach you how to make device driver with new platform.
 *
 *  @author		Jong-Sang Oh(jongsang.oh@lge.com)
 *  @version	1.0
 *  @date		2014.05.20
 *
 *  @addtogroup lg1152_adec
 *	@{
 */

#ifndef	_MIXEDIP_COREDRV_H15_A0_H_
#define	_MIXEDIP_COREDRV_H15_A0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "os_util.h" 	//for alloc functions

#include "debug_util.h"
#include "audio_kapi.h"

#include "sys_regs.h"	//for CTOP and ACE TOP CTRL Reg. map

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//define Power-up and down sequence delay
#define MIXEDIP_POWER_OFF_DELAY		(1+1)		// 1 us

#define AAD_POWER_OFF_DELAY			(200+20)	// 200 ms
#define AAD_DCO_RESETB_DELAY		(5+1)		// 5 us
#define AAD_SPLL_PDB_DELAY			(200+20)	// 200 us
#define AAD_ADC_PDB_DELAY			(1+1)		// 3 cycles(36.864M(26ns * 3)
#define AAD_SW_RESET_DELAY			(200+20)	// 200 us

#define AUAD_PDB_IN_DELAY			(200+20)	// 200 ms
#define HP_PDB_DELAY				(40+4)		// 40 ms
#define AUDA_PDB_DELAY				(40+4)		// 40 ms
#define AUDA_PDB_DSE_DELAY			(40+4)		// 40 ms
#define AUMI_PDB_IN_DELAY			(200+20)	// 200 ms

//define mute sequence delay
#define MIXEDIP_RESET_DELAY_10MS	(10+1)		// 10ms
#define MIXEDIP_RESET_DELAY_100US	(100+1)		// 100us
#define MIXEDIP_MUTE_DELAY_30MS		(30+1)		// 30ms

//define MUTE On/Off debug print
//#define ENABLE_MUTE_DEBUG

/* define H15 DTO_A Clock Loop Value. */
#define H15_DTO_A_CLOCK_LOOP_VALUE_48KHZ		0x0F423F
#define H15_DTO_A_CLOCK_LOOP_VALUE_44_1KHZ		0x0F423F
#define H15_DTO_A_CLOCK_LOOP_VALUE_32KHZ		0x0F423F


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int H15A0_ANALOG_ReadRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 *value);
int H15A0_ANALOG_WriteRegister(UINT8 slaveAddr, UINT8 regAddr, UINT8 value);

int H15A0_MIXED_IP_ADEC_InitClockRegisters( void );
int H15A0_MIXED_IP_ADEC_ResetClockRegisters( void );
int H15A0_MIXED_IP_ADEC_ResetAADClockRegisters( void );
int H15A0_MIXED_IP_ADEC_SetClockSource( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, LX_AUD_CLOCK_SRC_T ui8AdecClockSrc, LX_AUD_MASTER_SPDIF_OUTPUT_T spdifOutput );
int H15A0_MIXED_IP_ADEC_SetPcmClockRate( LX_AUD_SAMPLING_FREQ_T ui32SrcFreq, UINT32 ui32PcmClock );
int H15A0_MIXED_IP_ADEC_SetSPDIFSamplingFreq ( LX_AUD_SAMPLING_FREQ_T ui32SamplingFreq, LX_AUD_SPDIF_DTO_RATE_T stSpdifDtoRate, AUD_DTO_T dto );

int H15A0_MIXED_IP_ADEC_InitCodecRegisters ( void );
int H15A0_MIXED_IP_ADEC_StartCodec ( void );
int H15A0_MIXED_IP_ADEC_StopCodec ( void );

int H15A0_MIXED_IP_ADEC_SetADCPortNumber ( UINT8 ui8PortNum );
int H15A0_MIXED_IP_ADEC_SetDACOutVolume ( LX_AUD_DAC_VOL_T stDACOutVol );
int H15A0_MIXED_IP_ADEC_SetDACOutMute ( LX_AUD_OUTPUT_PORT_MUTE_T stOutPortMute );

int H15A0_MIXED_IP_ADEC_SetSpeakerFreq ( LX_AUD_SAMPLING_FREQ_T samplingFreq );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _MIXEDIP_COREDRV_H15_A0_H_ */

/** @} */
