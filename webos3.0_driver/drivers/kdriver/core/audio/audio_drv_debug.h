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



/** @file audio_drv_debug.h
 *
 *  driver interface header for audio device. ( used only within kdriver )
 *	audio device will teach you how to make device driver with new platform.
 *
 *  @author	wonchang.shin (wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2012.11.05
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_DEBUG_H_
#define	_AUDIO_DRV_DEBUG_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
/* KDRV AUDIO DRIVER Version Info. */
#define	KDRV_AUDIO_DRV_VERSION		2014122500


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void	KDRV_AUDIO_SetFwDebugMask(UINT32 mask);
SINT32	KDRV_AUDIO_ReadAndWriteReg ( LX_AUD_REG_INFO_T *pstRegInfo );
SINT32	KDRV_AUDIO_CustomImc(LX_AUD_CUSTOM_IMC_T* pstCustomImc);
void	KDRV_AUDIO_GetModIdFromStr(LX_AUD_DEBUG_MOD_INFO_T *pModInfo);
SINT32	KDRV_AUDIO_GetFwDebugInfo(LX_AUD_FW_DEBUG_INFO_T *pDebugInfo);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_DECODER_H_ */


