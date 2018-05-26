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
 * main driver implementation for HDMI device.
 * HDMI device will teach you how to make device driver with new platform.
 *
 * author     sunghyun.myoung (sh.myoung@lge.com)
 * version    1.0
 * date       2010.02.19
 * note       Additional information.
 *
 * @addtogroup lg115x_hdmi
 * @{
 */
#ifndef  HDMI_HW_H14_INC
#define  HDMI_HW_H14_INC

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "hdmi_kapi.h"
#include "hdmi_drv.h"
#include "hdmi_module.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int HDMI_H14_HWInitial(void);
int HDMI_H14_SetPort(UINT32 *port);
int HDMI_H14_GetMode(LX_HDMI_MODE_T *mode);
int HDMI_H14_GetAspectRatio(LX_HDMI_ASPECTRATIO_T *ratio);
int HDMI_H14_GetColorDomain(LX_HDMI_COLOR_DOMAIN_T *color);
int HDMI_H14_GetTimingInfo(LX_HDMI_TIMING_INFO_T *info);

int HDMI_H14_GetAviPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_H14_GetVsiPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_H14_GetSpdPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_H14_GetInfoPacket(LX_HDMI_INFO_PACKET_T *packet);

int HDMI_H14_SetPowerControl(UINT32 power);
int HDMI_H14_SetHPD(LX_HDMI_HPD_T *hpd);


int HDMI_H14_GetStatus(LX_HDMI_STATUS_T *status);

//audio related
int HDMI_H14_GetAudioInfo(LX_HDMI_AUDIO_INFO_T *pAudioInfo);
int HDMI_H14_GetAudioCopyInfo(LX_HDMI_AUDIO_COPY_INFO_T *pCopyInfo);
int HDMI_H14_SetArc(LX_HDMI_ARC_CTRL_T *pArcCtrl);
int HDMI_H14_SetMute(LX_HDMI_MUTE_CTRL_T *pMuteCtrl);
int HDMI_H14_GetAudioDebugInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo);


int HDMI_H14_GetRegister(UINT32 addr , UINT32 *value);
int HDMI_H14_SetRegister(UINT32 addr , UINT32 value);
int HDMI_H14_DisableInterrupt(void);

int HDMI_H14_Thread_Control(int sleep_enable);
#if 0


int HDMI_H14_PowerConsumption(UINT32 power);
int HDMI_H14_SetUDn3DControl(LX_HDMI_UD_3D_MODE_T *udn3d);


void HDMI_H14_SetPixelRepetition(void);
void HDMI_H14_SetAutoSyncPolarity(void);




#endif

#ifdef	KDRV_CONFIG_PM
int HDMI_H14_RunSuspend(void);
int HDMI_H14_RunResume(void);
#endif

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_REG_H14_INC  ----- */
/**  @} */
