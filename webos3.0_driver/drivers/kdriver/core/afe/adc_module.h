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


/*! \file ADC_module.h
 * \brief VPORT ADC module header
 */

#ifndef ADC_MODULE
#define ADC_MODULE

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define __ARM__

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
#endif

#include "base_types.h"
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define ADC_DETECTION_INTERRUPT_MASK	0x0000068A
#define ADC_PERIODIC_SIGNAL_INFO_READ

//#define L8_PHASE_NOISE_WORKAROUND
//#define L8_COMP_HSYNC_PHASE_SEARCH
//#define ADC_SOG_LVL_WORKAROUND

#define ADC_L9_R_PRE_OFFSET_VALUE	0x800
#define ADC_L9_G_PRE_OFFSET_VALUE	0x800
#define ADC_L9_B_PRE_OFFSET_VALUE	0x800

#define SUPPORT_SCART_RGB_ULTRA_BLACK

#ifdef	SUPPORT_SCART_RGB_ULTRA_BLACK
#define	SCART_RGB_OFFSET_ADD	0x4C	//16 level added
#endif

//#define USE_ADC_TIMER

#define	INPUT_SIGNAL_INFORM_REPEAT	4	// should be pow by 2
#define	INPUT_SIGNAL_INFORM_REPEAT_POW	2	// should be pow by 2

#define	TABLE_COUNT		275	
 
#define	FREQ_CONV_HZ	240000

#define ENHANCED_IFM_SPEED_MODE		//140528 : faster IFM timing


/*
	Device inform
*/


/*
	structure define
*/

typedef struct{
	unsigned int height_offset;
	unsigned int width_offset;
	unsigned int control;
	unsigned int level;
}S_EXTAU_VALUE;

typedef struct{
	unsigned int port;
	bool hsselover;
	bool hsseluser;
	bool vsselover;
	bool vsseluser;
	bool compover;
	bool compuser;
}S_SEL_PORT;

typedef struct{
	unsigned int out_b;
	unsigned int out_g;
	unsigned int out_r;

}S_RGB_OUT;

typedef struct{
	unsigned int	sum_R;
	unsigned int	sum_G;
	unsigned int	sum_B;
}S_RGB_SUM;


typedef struct{
	unsigned int width_b;
	unsigned int width_g;
	unsigned int width_r;

	unsigned int height_b;
	unsigned int height_g;
	unsigned int height_r;
}S_RGB_SIZE;

typedef struct{
	unsigned int width_b;
	unsigned int width_g;
	unsigned int width_r;

	unsigned int height_b;
	unsigned int height_g;
	unsigned int height_r;
}S_RGB_POSITION;


typedef struct{
	unsigned int	gain_R;
	unsigned int	gain_G;
	unsigned int	gain_B;
}ST_GAIN_INFORM;


typedef struct{
	//120103 wonsik.do : removed floating point in kernel driver
//	float gain_R;
//	float gain_G;
//	float gain_B;
	UINT64 gain_R;
	UINT64 gain_G;
	UINT64 gain_B;
} S_GAIN_TARGET;

typedef struct{
	int offset_R;
	int offset_G;
	int offset_B;

} S_OFFSET_INFORM;

typedef struct {
	int r_blank_target_value;
	int g_blank_target_value;
	int b_blank_target_value;
	int width;
	int r_shift_value;
	int g_shift_value;
	int b_shift_value;
} S_BLANK_INFORM;

typedef struct 
{
	unsigned long	ext_vsize;
	unsigned long	vd_vsize;
	unsigned long	vd_hsize;
	unsigned long	polarity;
}S_VIDEO_SIZE_INFORM;


typedef struct
{
	unsigned int extau_up;
	unsigned int extau_down;
	unsigned int extau_left;
	unsigned int extau_right;

	float extau_up_offset;
	float extau_left_offset;
}ST_EXTAU;

typedef struct{
	struct work_struct	ifm;
	struct work_struct	ibd;
	struct work_struct	aad;
	struct work_struct	goc;
	struct work_struct	apc;

	struct workqueue_struct *WorkQueue;
}ST_WORK_QUEUE;

typedef struct {
	UINT8	number;
	UINT8	first_sog_lvl;
	UINT8	quantity;
} ST_SOG_LEVEL;

/*
	Enum define
*/

typedef enum
{
	IRE0	= 0,
	IRE73	= 1,
	IRE950	= 2,
	IRE1023	= 3,
	IRE0_RGB	=4,
	IRE0_COMP	=5
} E_AGC_IRE;

typedef enum
{
	IRE73_L9	= 0,
	IRE146_L9	= 1,
	IRE877_L9	= 2,
	IRE950_L9	= 3,
	IRE0_RGB_L9		=4,
	IRE0_COMP_L9	=5
} E_AGC_IRE_L9;

typedef enum
{
	ADC_SEPERATED_SYNC_MODE,
	ADC_COMPOSITE_SYNC_MODE,
	ADC_SYNC_MODE_UNKNOWN
} ADC_SYNC_MODE;

typedef enum
{
	ADC_MUTE_CTRL_NORMAL 	= 0x0,
	ADC_MUTE_CTRL_MUTE		= 0x1,
	ADC_MUTE_CTRL_AUTO		= 0x2,
}	ADC_MUTE_CTRL;

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

// Function pointer
extern int(*ADC_RegInit)(void);
extern int(*ADC_GetBoundaryInform)(ST_EXTAU *inform);
extern int (*ADC_SetTAUInform)(S_EXTAU_VALUE *value);
extern int (*ADC_SetRGBPosition)(S_RGB_POSITION *pos);
extern int (*ADC_GetRGBResult)(S_RGB_OUT *result);
extern int (*ADC_GetRGBPosition)(S_RGB_POSITION *pos);
extern int (*ADC_SetRGBSize)(S_RGB_SIZE *size);
extern int (*ADC_GetSizeValue)(S_VIDEO_SIZE_INFORM *size);
extern int (*ADC_SetPort)(S_SEL_PORT *sel_port);
extern unsigned int (*ADC_GetPhaseValue)(void);
extern int (*ADC_SetPhaseValue)(unsigned int phase);
extern unsigned int (*ADC_GetScanType)(void);
extern int (*ADC_SetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
extern int (*ADC_GetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
extern int (*ADC_SetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
extern int (*ADC_GetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
extern int (*ADC_SetInputVideo)(int iIndex);
extern int (*ADC_Enable_Scart_Mode)(BOOLEAN scart_enable);
extern void (*ADC_Vport_ALL_Power_Down)(void);
extern int (*ADC_Channel_Power_Control)(int vref_pdb, int bpdb, int gpdb, int rpdb);
extern int (*ADC_SetType)(LX_AFE_ADC_INPUT_SOURCE_TYPE_T sel_type);
extern int (*ADC_Set_LLPLL)(UINT32 arg);
extern int (*ADC_SetClock)(int iIndex, SINT16 value);
extern int (*ADC_AGOCInit)(void);
extern int (*ADC_GetSelectedPort)(void);
extern int (*ADC_SetGainRGB)( ST_GAIN_INFORM *gain);
extern int (*ADC_GetGainRGB)( ST_GAIN_INFORM *gain);
extern int (*ADC_SetIRELevel)(E_AGC_IRE ire , S_GAIN_TARGET *gain);
extern int (*ADC_GetOffsetRGB)( S_OFFSET_INFORM *offset);
extern int (*ADC_SetOffsetRGB)( S_OFFSET_INFORM *offset);
extern int (*ADC_GetSumRGB)(S_RGB_SUM *sum);
extern int (*ADC_SetAgocWakeup)(void);
extern int (*ADC_Reset_Digital)(void);
extern int (*ADC_Reset_Digital_24MHZ)(void);
extern int (*ADC_Reset_LLPLL)(void);
extern int (*ADC_Check_Sync_Exist)(void);
extern int (*ADC_Sync_Polarity_Override)(int set);
extern int (*ADC_Enable_Detection_Interrupt)(int enable);
extern int (*ADC_Enable_AOGC_Interrupt)(int enable);
extern int (*ADC_Mute_Enable)(int enable);
extern int (*ADC_Power_OnOff)(void);
extern int (*ADC_Ext_Clamp)(int sel_ext);
extern int (*ADC_Set_Clamp)(UINT8 clamp_base, UINT8 clamp_place, UINT8 clamp_width);
extern int (*ADC_GetPCModeInfo)(LX_AFE_PCMODE_INFO_T *stPCModeInfo);
extern int (*ADC_Get_LLPLL_Filter_Status)(void);
extern int (*ADC_SetIRETestMode)(int sel , E_AGC_IRE mode);
extern int (*ADC_Set_Coast)(UINT8 precoast, UINT8 postcoast);
extern int (*ADC_Set_Hsync_Sel)(UINT8 select);
extern int (*ADC_CVI_Sync_Exist)(void);
extern int (*ADC_Set_SOG_Level)(UINT32 level);
extern int (*ADC_Get_SOG_Level)(UINT32 *level);
extern int (*ADC_Check_Htotal_Diff)(UINT8 count, int Kadp_Index);
extern int (*ADC_Read_Pixel_Value)(LX_AFE_ADC_PIXEL_VALUE_T *pixel_value_t);
extern int (*ADC_Green_Mid_Level_Select)(UINT8 value);
extern int (*ADC_Read_VSync_Width)(UINT32 *pvs_width);
extern int (*ADC_Read_HSync_Width)(UINT32 *pvs_width);
extern void (*ADC_Set_Blank)(int blank_sp); // added by won.hur(won.hur@lge.com) @2011.08.17
extern void (*ADC_LVDS_Control)(int enable); // added by won.hur(won.hur@lge.com) @2011.08.17
extern int (*ADC_Component_APA)(int iIndex);
extern int (*ADC_Component_APA_Test)(UINT32 ADC_APA_Delay, UINT32 ADC_APA_Enable, UINT32 ADC_APA_Same_Count, UINT32 ADC_APA_Execute, UINT32 ADC_APA_Enable_Print);
extern int (*ADC_Get_Vtotal_Value)(void);
extern int (*ADC_Set_Sync_Mode)(ADC_SYNC_MODE sync_mode);
extern int (*ADC_LVDS_Channel_Enable_Control)(int enable);
extern int (*ADC_Mute_Control)(ADC_MUTE_CTRL adc_mute_control);
extern int (*ADC_Post_Gain_Control)(int percent);
extern int (*ADC_Set_Comp_Params)(int width, int progressive, int v_freq);
extern int (*ADC_Read_Sync_Low_Level)(void);
extern int (*ADC_Enable_Sync_Low_Level_Read)(int enable);
extern int (*ADC_Check_LLPLL_status)(void);
extern int (*ADC_LVDS_Src_Control)(LX_AFE_LVDS_SRC_CONTROL_T *pLVDS_Control_t);
extern int (*ADC_Read_ACE_Reg)(UINT32 addr , UINT32 *value);
extern int (*ADC_Write_ACE_Reg)(UINT32 addr , UINT32 value);
extern int (*ADC_Component_Set_CP_Params)(int kadp_index);
extern int (*ADC_Component_Get_CP_Data)(LX_AFE_ADC_COMP_VBI_CP_T *stComp_VBI_CP_Data);
extern int (*ADC_Flush_Component_CP_Data)(void);
extern int (*ADC_Get_Line_Count)(UINT32 *pLineCount);
extern int (*ADC_Read_ACE_GPIO)(UINT32 *pGPIO_Status);
extern int (*ADC_Set_ADC_Input_Filter_Sel)(int value);



/*
	global variable
*/
//extern volatile VPORT_Reg_T *gVportReg;

/*
	function prototype
*/
//void ADC_IFM(void *unused);
//void ADC_IBD(void *unused);
void ADC_AAD(void *unused);
int ADC_GOC(LX_AFE_ADC_CALIB_INFO_T *calib_param);

void ADC_IFM_ISR(void *unused);

int ADC_IBD_Mesurement(ST_EXTAU *extau , int iIndex);
int ADC_IBD_main(int iIndex);
int ADC_IBD_Setting(ST_EXTAU *extau , int iIndex);
int ADC_IBD_GetBackPorch(ST_EXTAU *extau);

int ADC_IFM_main(int *iIndex);
//int ADC_GetInputSignalInform(S_VIDEO_SIZE_INFORM *inform);
int ADC_IFM_GetInputSignal(S_VIDEO_SIZE_INFORM *inform);
int ADC_IFM_Routine(S_VIDEO_SIZE_INFORM *adc_size);


int ADC_PHASE_main(int iIndex);
int ADC_PHASE_for_Component(int iIndex);

int ADC_AutoGainControl_main(LX_AFE_ADC_CALIB_INFO_T *calib_param, int iIndex);
int ADC_AutoOffsetControl_main(int iIndex);
int ADC_AutoOffsetControlByInput_main(int iIndex);

int ADC_Initialize(void);
int ADC_PCAdjust(LX_AFE_ADJ_PCMODE_INFO_T *stAdjMode);
int ADC_AutoAdjust(LX_AFE_ADJ_PC_T *stAdj);
void ADC_GetTimingInfo(LX_AFE_ADC_TIMING_INFO_T *stTiming);
int ADC_SetInputFormat(LX_AFE_ADC_SET_INPUT_T *stInput);

#if 0
int ADC_SetGainValue(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int ADC_GetGainValue(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int ADC_SetOffsetValue(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
int ADC_GetOffsetValue(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
#else
extern int (*ADC_SetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
extern int (*ADC_GetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
extern int (*ADC_SetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
extern int (*ADC_GetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
#endif

int ADC_SetPCmodeTable(LX_AFE_SET_PCMODE_TABLE_T *stTableInfo);

extern int (*ADC_Enable_Scart_Mode)(BOOLEAN scart_enable);

int ADC_Set_PCMODE_Resoultion(UINT32 arg);

int ADC_GetInputSignalInform(S_VIDEO_SIZE_INFORM *inform);

int ADC_Phase_Convert_to_UI_Value(int reg_phase_value, int gResolution_index);
irqreturn_t VPORT_ADC_irq_handler(int irq, void *dev_id, struct pt_regs *regs);

UINT64 ADC_PHASE_Mesurement_Simple(int iIndex, UINT16 htotal);

int ADC_Enable_Periodic_Signal_Info_Read(UINT32 enable);
int ADC_Execute_Format_Detection(void);
int ADC_Enable_Component_Auto_Phase(UINT32 enable);
int ADC_Test_Program(LX_AFE_ADC_TEST_PARAM_T *stADC_Test_Param);
int ADC_IFM_SearchTable(S_VIDEO_SIZE_INFORM *inform);

int ADC_Periodic_Task(void);
int ADC_Clear_Previous_Size_Info(void);
int ADC_Get_Component_Pseudo_Pulse(LX_AFE_ADC_COMP_PSEUDO_PULSE_T *pComp_PSP_t);
int ADC_Get_ACE_FB_Status(LX_AFE_SCART_MODE_T *pSCMode);
int ADC_Set_SCART_RGB_Operation_Mode(LX_AFE_SCART_BYPASS_MODE_T scart_rgb_mode);
// for test
//int ADC_RunRegister(S_INTERFACE_REG *reg);
void ADC_KernelRun(void);
//int ADC_PHASE_Mesurement_for_Component_HSync(int iIndex);
//int ADC_PHASE_Mesurement_for_Component_HSync_Sampling(int iIndex);
//int ADC_PHASE_Mesurement_for_Component_HSync_Fast(int iIndex);
#ifdef	KDRV_CONFIG_PM
int ADC_RunSuspend(void);
int ADC_RunResume(void);
#endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif 
