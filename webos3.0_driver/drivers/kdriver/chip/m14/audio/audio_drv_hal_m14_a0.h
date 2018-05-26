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
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	2.0
 *  date	2013.05.16
 *  note	Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_HAL_M14_A0_H_
#define	_AUDIO_DRV_HAL_M14_A0_H_

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
#include "os_util.h" //for alloc functions

#include "base_types.h"
#include "audio_kapi.h"
#include "audio_drv.h"
#include "audio_drv_hal.h"

#include "sys_regs.h"	//for ACE TOP CTRL Reg. map

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#include <asm/hardware.h> // For Register base address
#endif

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
SINT32 M14_A0_AUDIO_IOREMAP( void );
SINT32 M14_A0_AUDIO_WriteReg ( LX_AUD_REG_ADDRESS_T addr, UINT32 data );
UINT32 M14_A0_AUDIO_ReadReg ( LX_AUD_REG_ADDRESS_T addr );
BOOLEAN M14_A0_AUDIO_CheckResetStatus ( UINT8 ui8ResetFlag );
SINT32 M14_A0_AUDIO_SetReset ( UINT8 ui8ResetFlag );
SINT32 M14_A0_AUDIO_ClearReset ( UINT8 ui8ResetFlag );
SINT32 M14_A0_AUDIO_InitRegister ( void );
SINT32 M14_A0_AUDIO_InitRegForCheckbit ( void );
SINT32 M14_A0_AUDIO_StallDspToReset ( void );
SINT32 M14_A0_AUDIO_ClearDSP1VerInfo ( void );
void M14_A0_AUDIO_DisplayDSP1VerInfo ( UINT64 startTick );
SINT32 M14_A0_AUDIO_ClearDSP0VerInfo ( void );
void M14_A0_AUDIO_DisplayDSP0VerInfo ( UINT64 startTick );

SINT32 M14_A0_AUDIO_ReadAndWriteReg ( LX_AUD_REG_INFO_T *pstRegInfo );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_HAL_M14_A0_H_ */

/** @} */


