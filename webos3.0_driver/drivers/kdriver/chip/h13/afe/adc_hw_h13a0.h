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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     wonsik.do
 *  @version    1.0
 *  @date       2012-04-30
 *  @note       Additional information.
 */

#ifndef	_ADC_HW_H13A0_H_
#define	_ADC_HW_H13A0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

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
int ADC_H13Ax_PowerOn(void);
int ADC_H13Ax_PowerOff(void);
int ADC_H13Ax_RegInit(void);
int ADC_H13Ax_GetBoundaryInform(ST_EXTAU *inform);
int ADC_H13Ax_SetTAUInform(S_EXTAU_VALUE *value);
int ADC_H13Ax_SetRGBPosition(S_RGB_POSITION *pos);
int ADC_H13Ax_GetRGBResult(S_RGB_OUT *result);
int ADC_H13Ax_GetRGBPosition(S_RGB_POSITION *pos);
int ADC_H13Ax_SetRGBSize(S_RGB_SIZE *size);
int ADC_H13Ax_GetSizeValue(S_VIDEO_SIZE_INFORM *size);
int ADC_H13Ax_SetPort(S_SEL_PORT *sel_port);
unsigned int ADC_H13Ax_GetPhaseValue(void);
int ADC_H13Ax_SetPhaseValue(unsigned int phase);
unsigned int ADC_H13Ax_GetScanType(void);
int ADC_H13Ax_SetGainValue(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int ADC_H13Ax_GetGainValue(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int ADC_H13Ax_SetOffsetValue(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
int ADC_H13Ax_GetOffsetValue(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
int ADC_H13Ax_SetInputVideo(int iIndex);
int ADC_H13Ax_Enable_Scart_Mode(BOOLEAN scart_enable);
void ADC_H13Ax_Vport_ALL_Power_Down(void);
int ADC_H13Ax_Channel_Power_Control(int vref_pdb, int bpdb, int gpdb, int rpdb);
int ADC_H13Ax_SetType(LX_AFE_ADC_INPUT_SOURCE_TYPE_T sel_type);
int ADC_H13Ax_Set_LLPLL(UINT32 arg);
int ADC_H13Ax_SetClock(int iIndex, SINT16 value);
int ADC_H13Ax_AGOCInit(void);
int ADC_H13Ax_GetSelectedPort(void);
int ADC_H13Ax_SetGainRGB( ST_GAIN_INFORM *gain);
int ADC_H13Ax_GetGainRGB( ST_GAIN_INFORM *gain);
int ADC_H13Ax_SetIRELevel(E_AGC_IRE ire , S_GAIN_TARGET *gain);
int ADC_H13Ax_GetOffsetRGB( S_OFFSET_INFORM *offset);
int ADC_H13Ax_SetOffsetRGB( S_OFFSET_INFORM *offset);
int ADC_H13Ax_GetSumRGB(S_RGB_SUM *sum);
int ADC_H13Ax_SetAgocWakeup(void);
int ADC_H13Ax_Reset_Digital(void);
int ADC_H13Ax_Reset_Digital_24MHZ(void);
int ADC_H13Ax_Reset_LLPLL(void);
int ADC_H13Ax_Check_Sync_Exist(void);
int ADC_H13Ax_Sync_Polarity_Override(int set);
int ADC_H13Ax_Enable_Detection_Interrupt(int enable);
int ADC_H13Ax_Enable_AOGC_Interrupt(int enable);
int ADC_H13Ax_Mute_Enable(int enable);
int ADC_H13Ax_Power_OnOff(void);
int ADC_H13Ax_Ext_Clamp(int sel_ext);
int ADC_H13Ax_Set_Clamp(UINT8 clamp_base, UINT8 clamp_place, UINT8 clamp_width);
int ADC_H13Ax_GetPCModeInfo(LX_AFE_PCMODE_INFO_T *stPCModeInfo);
int ADC_H13Ax_Get_LLPLL_Filter_Status(void);
int ADC_H13Ax_SetIRETestMode(int sel , E_AGC_IRE mode);
int ADC_H13Ax_Set_Coast(UINT8 precoast, UINT8 postcoast);
int ADC_H13Ax_Set_Hsync_Sel(UINT8 select);
int ADC_H13Ax_CVI_Sync_Exist(void);
int ADC_H13Ax_Set_SOG_Level(UINT32 level);
int ADC_H13Ax_Get_SOG_Level(UINT32 *level);
int ADC_H13Ax_Check_Htotal_Diff(UINT8 count, int Kadp_Index);
int ADC_H13Ax_Read_Pixel_Value(LX_AFE_ADC_PIXEL_VALUE_T *pixel_value_t);
int ADC_H13Ax_Green_Mid_Level_Select(UINT8 value);
int ADC_H13Ax_Read_VSync_Width(UINT32 *pvs_width);
int ADC_H13Ax_Read_HSync_Width(UINT32 *pvs_width);
int ADC_H13Ax_Set_DCO_MinMax_Value(UINT8 dco_min, UINT8 dco_max);
int ADC_H13Ax_Get_DCO_MinMax_Value(UINT8 *pdco_min, UINT8 *pdco_max);
void ADC_H13Ax_LVDS_Control(int enable);
void ADC_H13Ax_Set_Blank(int blank_sp);
int ADC_H13Ax_Set_Blank_Value(S_BLANK_INFORM *blank_t);
int ADC_H13Ax_Reset_LLPLL_Control(int enable_reset);
int ADC_H13Ax_Reset_Digital_Control(int enable_reset);
int ADC_H13Ax_LVDS_Reset_Control(int reset_enable);
int ADC_H13Ax_LVDS_Src_Control(LX_AFE_LVDS_SRC_CONTROL_T *pLVDS_Control_t);
int ADC_H13Ax_Component_APA(int iIndex);
int ADC_H13Ax_Component_APA_Test(UINT32 ADC_APA_Delay, UINT32 ADC_APA_Enable, UINT32 ADC_APA_Same_Count, UINT32 ADC_APA_Execute, UINT32 ADC_APA_Enable_Print);
int ADC_H13Ax_Get_Vtotal_Value(void);
int ADC_H13Ax_Set_Sync_Mode(ADC_SYNC_MODE sync_mode);
int ADC_H13Ax_Mute_Control(ADC_MUTE_CTRL adc_mute_control);
int ADC_H13Ax_Set_Comp_Params(int width, int progressive, int v_freq);
int ADC_H13Ax_Read_AGOC_State(void);
int ADC_H13Ax_Enable_Sync_Low_Level_Read(int enable);
int ADC_H13Ax_GetIRELevel( S_GAIN_TARGET *gain);
int ADC_H13Ax_Read_Sync_Low_Level(void);
int ADC_H13Ax_Post_Gain_Control(int percent);
int ADC_H13Ax_Check_LLPLL_status(void);
int ADC_H13Ax_LVDS_Channel_Enable_Control(int enable);
int ADC_H13Ax_Read_ACE_Reg(UINT32 addr , UINT32 *value);
int ADC_H13Ax_Write_ACE_Reg(UINT32 addr , UINT32 value);
int ADC_H13Ax_Get_Line_Count(UINT32 *pLineCount);
int ADC_H13Ax_Read_ACE_GPIO(UINT32 *pGPIO_Status);
int ADC_H13Ax_Component_Set_CP_Params(int kadp_index);
int ADC_H13Ax_Component_Get_CP_Data(LX_AFE_ADC_COMP_VBI_CP_T *stComp_VBI_CP_Data);
int ADC_H13Ax_Flush_Component_CP_Data(void);
int ADC_H13Ax_Set_ADC_Input_Filter_Sel(int value);
#ifdef	KDRV_CONFIG_PM
int ADC_H13Ax_RunSuspend(void);
int ADC_H13Ax_RunResume(void);
#endif
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

