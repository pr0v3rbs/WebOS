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
#ifndef  HDMI_HW_M14C0_INC
#define  HDMI_HW_M14C0_INC

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
int HDMI_M14C0_HWInitial(void);
int HDMI_M14C0_SetPort(UINT32 *port);
int HDMI_M14C0_GetMode(LX_HDMI_MODE_T *mode);
int HDMI_M14C0_GetAspectRatio(LX_HDMI_ASPECTRATIO_T *ratio);
int HDMI_M14C0_GetColorDomain(LX_HDMI_COLOR_DOMAIN_T *color);
int HDMI_M14C0_GetTimingInfo(LX_HDMI_TIMING_INFO_T *info);

int HDMI_M14C0_GetAviPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_M14C0_GetVsiPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_M14C0_GetSpdPacket(LX_HDMI_INFO_PACKET_T *packet);
int HDMI_M14C0_GetInfoPacket(LX_HDMI_INFO_PACKET_T *packet);

int HDMI_M14C0_SetPowerControl(UINT32 power);
int HDMI_M14C0_SetHPD(LX_HDMI_HPD_T *hpd);


int HDMI_M14C0_GetStatus(LX_HDMI_STATUS_T *status);

//audio related
int HDMI_M14C0_GetAudioInfo(LX_HDMI_AUDIO_INFO_T *pAudioInfo);
int HDMI_M14C0_GetAudioCopyInfo(LX_HDMI_AUDIO_COPY_INFO_T *pCopyInfo);
int HDMI_M14C0_SetArc(LX_HDMI_ARC_CTRL_T *pArcCtrl);
int HDMI_M14C0_SetMute(LX_HDMI_MUTE_CTRL_T *pMuteCtrl);
int HDMI_M14C0_GetAudioDebugInfo(LX_HDMI_DEBUG_AUDIO_INFO_T *pAudioDebugInfo);


int HDMI_M14C0_GetRegister(UINT32 addr , UINT32 *value);
int HDMI_M14C0_SetRegister(UINT32 addr , UINT32 value);
int HDMI_M14C0_DisableInterrupt(void);

int HDMI_M14C0_Read_EDID_Data(int port_num , UINT8 *pedid_data);
int HDMI_M14C0_Write_EDID_Data(int port_num , UINT8 *pedid_data);
int HDMI_M14C0_Get_Phy_Status(LX_HDMI_PHY_INFORM_T *sp_hdmi_phy_status);
int HDMI_M14C0_Write_HDCP_Key(UINT8 *hdcp_key_data);
int HDMI_M14C0_Read_HDCP_Key(UINT8 *hdcp_key_data);

int HDMI_M14C0_Thread_Control(int sleep_enable);
int HDMI_M14C0_Get_Aksv_Data(int port_num, UINT8 *pAksv_Data);
int HDMI_M14C0_MHL_Send_RCP(UINT8 rcp_data);
int HDMI_M14C0_MHL_Send_WriteBurst(LX_HDMI_MHL_WRITEBURST_DATA_T *spWriteburst_data);
int HDMI_M14C0_MHL_Read_WriteBurst(LX_HDMI_MHL_WRITEBURST_DATA_T *spWriteburst_data);
int HDMI_M14C0_Module_Call_Type(LX_HDMI_CALL_TYPE_T	hdmi_call_type);
int HDMI_M14C0_Reset_Internal_Edid(int port_num, int edid_resetn);
int HDMI_M14C0_Enable_External_DDC_Access(int port_num, int enable);
int HDMI_M14C0_MHL_Receive_RCP(LX_HDMI_RCP_RECEIVE_T *sp_MHL_RCP_rcv_msg);
int HDMI_M14C0_Get_Driver_Status(LX_HDMI_DRIVER_STATUS_T *spHDMI_Driver_Status);
int HDMI_M14C0_Key_Control(LX_HDMI_CONTROL_KEY_TYPE_T key_control_cmd);
int HDMI_M14C0_EnableMHL(BOOLEAN bEnable);
int HDMI_M14C0_Set_MHL_ODT(LX_HDMI_MHL_ODT_IMPEDANCE_T odt_value);
int HDMI_M14C0_Driver_Debug(LX_HDMI_DRV_DEBUG_T *sp_hdmi_drv_debug);
#if 0


int HDMI_M14C0_PowerConsumption(UINT32 power);
int HDMI_M14C0_SetUDn3DControl(LX_HDMI_UD_3D_MODE_T *udn3d);


void HDMI_M14C0_SetPixelRepetition(void);
void HDMI_M14C0_SetAutoSyncPolarity(void);




#endif

#ifdef	KDRV_CONFIG_PM
int HDMI_M14C0_RunSuspend(void);
int HDMI_M14C0_RunResume(void);
#endif

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_REG_M14_INC  ----- */
/**  @} */
