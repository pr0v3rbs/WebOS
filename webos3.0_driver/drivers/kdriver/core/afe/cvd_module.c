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
 *  @author		wonsik.do
 *  @version	1.0
 *  @date		2010-11-16
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/signal.h>
#include <linux/semaphore.h>

#include "afe_drv.h"

#include "os_util.h"
//#include "hdmi_phy_reg.h"

#include "afe_cfg.h"

#include "cvd_module.h"
#ifdef INCLUDE_L9_CHIP_KDRV
#endif
//#include "pe_dcm.h"
#include "h13/afe/de_cvd_reg_h13bx.h"
#include "h13/afe/de_cvd_reg_h13ax.h"
#include "h13/afe/cvd_hw_h13a0.h"
#include "h13/afe/cvd_hw_h13b0.h"
#include "h13/afe/cvd_control_h13a0.h"
#include "h13/afe/cvd_control_h13b0.h"

#include "h13/afe/cvd_task_h13a0.h"
#include "h13/afe/cvd_task_h13b0.h"

#include "h14/afe/de_cvd_reg_h14ax.h"
#include "h14/afe/cvd_hw_h14a0.h"
#include "h14/afe/cvd_control_h14a0.h"
#include "h14/afe/cvd_task_h14a0.h"

#include "m14/afe/de_cvd_reg_m14ax.h"
#include "m14/afe/cvd_hw_m14a0.h"
#include "m14/afe/cvd_control_m14a0.h"
#include "m14/afe/cvd_task_m14a0.h"

#include "m14/afe/de_cvd_reg_m14bx.h"
#include "m14/afe/cvd_hw_m14b0.h"
#include "m14/afe/cvd_control_m14b0.h"
#include "m14/afe/cvd_task_m14b0.h"

#include "h15/afe/de_cvd_reg_h15ax.h"
#include "h15/afe/cvd_hw_h15a0.h"
#include "h15/afe/cvd_control_h15a0.h"
#include "h15/afe/cvd_task_h15a0.h"

#include <asm/io.h>
#include <linux/slab.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#ifdef INCLUDE_L9_CHIP_KDRV
#define USE_AFE_KDRV_FOR_L9
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
#define USE_AFE_KDRV_FOR_H13
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
#define USE_AFE_KDRV_FOR_H14
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
#define USE_AFE_KDRV_FOR_M14
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
#define USE_AFE_KDRV_FOR_H15
#endif


#define	MAINCVD_TIMEOUT	150	// 1000 msec
#define	L9_CVD_TIMEOUT	50	// 1000 msec


#define ENABLE_CVD_THREAD				///<< Must define this to make CVD driver work.
#define CVD_THREAD_VSYNC_CONTROL	1 	///<< CVD Thread is wakeup by CVD vsync


#define CVD_STATE_TRANSITION_MAX	15

#define ENABLE_CVD_STATE_TRANSITION_LIMIT 1
#define ENABLE_CVD_INTERRUPT_CONTROL 0

#define CVD_FC_FOR_SECAM_LOW	190
#define CVD_FC_FOR_SECAM_HIGH	240
#define CVD_FC_FOR_PALCn_LOW	170
#define CVD_FC_FOR_PALCn_HIGH	220
//#define CVD_KERNEL_DEBUG

//#define DCRESTORE_ACCUM_WIDTH_INITIAL			0x25

// Workaround for chroma lost on brasil streams : 20120202

// moved to cvd_hw.h
//#define L9_FAST_3DCOMB_WORKAROUND

// Workaround for stable AGC on weak RF signal
//#define CVD_AGC_PEAK_CONTROL_WORKAROUND
//#define L9_ADAPTIVE_AGC_PEAK_NOMINAL_CONTROL
#define AGC_PEAK_NOMINAL_INITIAL_VALUE	0x30
#define AGC_PEAK_NOMINAL_DEFAULT_VALUE	0x0a

#define USE_REGISTER_VALUE_FOR_SYNC_STATES
/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern int DEMOD_ANALOG_Set_ClampingCtrl(UINT8 clampCount, UINT16 stepsize);

extern int CVD_H13Bx_Channel_Change_Control(BOOLEAN bATV_Channel_Change);

extern int VBI_CC_InterruptFunc(unsigned int status);
extern int VBI_TTX_InterruptFunc(unsigned int status);
/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern LX_CVD_REG_T *pVportRegCfg;

extern CVD_SET_SYSTEM_MAIN_T g_SetColorSystemMain[];

extern CVD_SET_SYSTEM_3CS_H15A0_T g_SetColorSystem_3CS_H15A0[];
extern CVD_SET_SYSTEM_3CS_H14A0_T g_SetColorSystem_3CS_H14A0[];
extern CVD_SET_SYSTEM_3CS_M14B0_T g_SetColorSystem_3CS_M14B0[];
extern CVD_SET_SYSTEM_3CS_M14A0_T g_SetColorSystem_3CS_M14A0[];
extern CVD_SET_SYSTEM_3CS_H13B0_T g_SetColorSystem_3CS_H13B0[];
extern CVD_SET_SYSTEM_3CS_H13A0_T g_SetColorSystem_3CS_H13A0[];

extern BOOLEAN ENABLE_ANALOGCC;
extern BOOLEAN ENABLE_ANALOGTTX;
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

// H13A0 CVD Register Access
volatile DE_CVD_REG_H15Ax_T __iomem *gpRegCVD_H15Ax;
volatile DE_CVD_REG_H15Ax_T __iomem *gpRealRegCVD_H15Ax; 
volatile DE_CVD_REG_M14Bx_T __iomem *gpRegCVD_M14Bx;
volatile DE_CVD_REG_M14Bx_T __iomem *gpRealRegCVD_M14Bx; 
volatile DE_CVD_REG_H14Ax_T __iomem *gpRegCVD_H14Ax;
volatile DE_CVD_REG_H14Ax_T __iomem *gpRealRegCVD_H14Ax; 
volatile DE_CVD_REG_M14Ax_T __iomem *gpRegCVD_M14Ax;
volatile DE_CVD_REG_M14Ax_T __iomem *gpRealRegCVD_M14Ax; 
volatile DE_CVD_REG_H13Bx_T __iomem *gpRegCVD_H13Bx;
volatile DE_CVD_REG_H13Bx_T __iomem *gpRealRegCVD_H13Bx; 

volatile DE_CVD_REG_H13Ax_T __iomem *gpRegCVD_H13Ax;
volatile DE_CVD_REG_H13Ax_T __iomem *gpRealRegCVD_H13Ax; 

#ifdef INCLUDE_L9_CHIP_KDRV
#endif


BOOLEAN gCVDInitStatus = FALSE;

//UINT8 	gEnable_Monitor_3DCOMB_State = 0;

//spinlock_t 	cvd_lock;
//ULONG		cvd_irq_flags = 0;

#ifdef ENABLE_CVD_THREAD
struct task_struct	*stCVD_Thread;
//long	gPid_cvd_thread = -1;

static int		gForce_thread_sleep = 1;
static int		gCVD_thread_running = 0;
int		gWait_return = -1;

DECLARE_WAIT_QUEUE_HEAD(WaitQueue_CVD);
#endif
#ifdef	KDRV_CONFIG_PM
typedef	struct{
	BOOLEAN CVDInitStatus;
	LX_AFE_CVD_SELECT_T SelectMainSub;
}S_AFE_PM_DATA;

S_AFE_PM_DATA *gAfePmData;
#endif

extern OS_SEM_T	g_CVD_Sema;

//CVD_AGC_PEAK_CONTROL_T	g_CVD_AGC_Peak_Control;

CVD_STATUS_3CS_T	g_CVD_Status_3CS = {0,};

//LX_AFE_CVD_SUPPORT_COLOR_SYSTEM_T	g_CVD_Color_System_Support = LX_COLOR_SYSTEM_MULTI;
//int		gAutoProgramming	= 0;
CVD_PATTERN_DETECTION_T g_CVD_Pattern_Detection_t ;

BOOLEAN IS_CVD_INTERRUPT_REGISTERED	= false;
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#ifdef CVD_AGC_PEAK_CONTROL_WORKAROUND		//disabled
static int CVD_Adaptive_AGC_Peak_Control(CVD_AGC_PEAK_CONTROL_T *pCVD_AGC_Peak_Control );
#endif

static int MainCVD_Thread(void *data);
static int MainCVD_Thread_Vsync_Ctrl(void *data);

static int CVD_Get_No_Signal_Flag(void);
static int CVD_Get_Chromalock_Flag(void);

static int CVD_Get_Vline_625_Reg(void);	// Read user forced vline 625 value
//void (*CVD_Program_Color_System_PreJob)(CVD_SET_SYSTEM_MAIN_T *pSet_system_t);
static int CVD_Init_Global_Params(void);

static int CVD_Get_Reg_States_Detail(LX_AFE_CVD_STATES_DETAIL_T	*pcvd_states_detail_t);
static int CVD_Set_Color_System_3CS(CVD_STATE_T cvd_next_state);
static int CVD_Set_ycsep_Blend(int blend);
static int CVD_Set_clampagc_updn(int updn_value);
static int CVD_Set_dc_clamp_mode(int mode);

static irqreturn_t CVD_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs);
static int CVD_SetInterruptClear(void);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#ifdef CVD_AGC_PEAK_CONTROL_WORKAROUND		//disabled
static UINT32	g_AGC_Peak_Nominal_Value = AGC_PEAK_NOMINAL_DEFAULT_VALUE;
#endif

CVD_ADAPTIVE_PEAK_NOMINAL_CONTROL_T g_CVD_AGC_Peak_Nominal_Control ;

static	int CVD_Set_Initial_Color_System(void);
/*========================================================================================
	Implementation Group
========================================================================================*/

int CVD_Set_PQ_Mode(LX_AFE_CVD_PQ_MODE_T cvd_pq_mode)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_PQ_Mode(cvd_pq_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_PQ_Mode(cvd_pq_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_PQ_Mode(cvd_pq_mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_PQ_Mode(cvd_pq_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_PQ_Mode(cvd_pq_mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_PQ_Mode(cvd_pq_mode);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_for_Tunning(BOOLEAN bEnable)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_for_Tunning(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_for_Tunning(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_for_Tunning(bEnable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_for_Tunning(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_for_Tunning(bEnable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_for_Tunning(bEnable);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_SW_Reset(LX_AFE_CVD_SELECT_T select_main_sub)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_SW_Reset(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_SW_Reset(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_SW_Reset(select_main_sub);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_SW_Reset(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_SW_Reset(select_main_sub);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_SW_Reset(select_main_sub);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Power_Down(LX_AFE_CVD_SELECT_T select_main_sub, BOOLEAN PowerOnOFF)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Power_Down(select_main_sub, PowerOnOFF);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Power_Down(select_main_sub, PowerOnOFF);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Power_Down(select_main_sub, PowerOnOFF);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Power_Down(select_main_sub, PowerOnOFF);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Power_Down(select_main_sub, PowerOnOFF);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Power_Down(select_main_sub, PowerOnOFF);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Source_Type(LX_AFE_CVD_SET_INPUT_T	cvd_input_info)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Source_Type(cvd_input_info);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Source_Type(cvd_input_info);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Source_Type(cvd_input_info);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Source_Type(cvd_input_info);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Source_Type(cvd_input_info);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_Source_Type(cvd_input_info);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Clamp_Current_Control(UINT8 value)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Clamp_Current_Control(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Clamp_Current_Control(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Clamp_Current_Control(value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Clamp_Current_Control(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Clamp_Current_Control(value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Clamp_Current_Control(value);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Scart_Overlay(BOOLEAN bEnable)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Scart_Overlay(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Scart_Overlay(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Scart_Overlay(bEnable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Scart_Overlay(bEnable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Scart_Overlay(bEnable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_Scart_Overlay(bEnable);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Reg_Init(LX_AFE_CVD_SELECT_T select_main_sub)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Reg_Init(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Reg_Init(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Reg_Init(select_main_sub);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Reg_Init(select_main_sub);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Reg_Init(select_main_sub);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Reg_Init(select_main_sub);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Scart_AR(LX_AFE_SCART_AR_INFO_T	*pScart_ar_param)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Get_Scart_AR(pScart_ar_param);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Get_Scart_AR(pScart_ar_param);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Get_Scart_AR(pScart_ar_param);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Get_Scart_AR(pScart_ar_param);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Get_Scart_AR(pScart_ar_param);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Get_Scart_AR(pScart_ar_param);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Vport_Reg_Read(LX_AFE_REG_RW_T *pReg_addr_data_t)
{
	int ret = RET_OK;

	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;

	do {
		recvAddr = pReg_addr_data_t->addr;
		/*
		   if		(recvAddr <= 0x2000) recvAddr += DTVSOC_DE_BASE;
		   else if (recvAddr <= 0xffff) recvAddr += DTVSOC_VIRT_PL301_BASE;
		 */
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H15Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H14Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		recvAddr += (UINT32)gpRealRegCVD_M14Bx;
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_M14Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H13Bx;
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H13Ax;
	} else
#endif
		return RET_ERROR;	// Unkown chip revision

	currAddr = recvAddr;
	currAddr >>= 2;
	currAddr <<= 2;
	value = REG_RD(currAddr);
	if (currAddr == recvAddr) break;
	nextValue = REG_RD(currAddr+4);
	dBit = (recvAddr - currAddr)<<3;
	wBit = (32 - dBit);
	value  = GET_BITS(value, dBit, wBit);
	value += GET_PVAL(nextValue, wBit, dBit);

	} while (0);
	pReg_addr_data_t->data = value;

	return ret;
}

int CVD_Vport_Reg_Write(LX_AFE_REG_RW_T *pReg_addr_data_t)
{
	int ret = RET_OK;

	UINT32 value;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;

	do {
		recvAddr = pReg_addr_data_t->addr;
		value = pReg_addr_data_t->data;
		/*
		   if		(recvAddr <= 0x2000) recvAddr += DTVSOC_DE_BASE;
		   else if (recvAddr <= 0xffff) recvAddr += DTVSOC_VIRT_PL301_BASE;
		 */
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H15Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H14Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		recvAddr += (UINT32)gpRealRegCVD_M14Bx;
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_M14Ax;
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H13Bx;
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		recvAddr += (UINT32)gpRealRegCVD_H13Ax;
	} else
#endif
		return RET_ERROR;	// Unkown chip revision

		currAddr = recvAddr;
		currAddr >>= 2;
		currAddr <<= 2;
		if (currAddr == recvAddr) {
			REG_WD(recvAddr, value);
			break;
		}
		currValue = REG_RD(currAddr);
		nextValue = REG_RD((currAddr+4));
		dBit = (recvAddr - currAddr)<<3;
		wBit = (32 - dBit);
		currValue  = GET_BITS(currValue ,0	  ,dBit);
		currValue += GET_PVAL(value		,dBit ,wBit);
		nextValue  = GET_PVAL(nextValue ,dBit ,wBit);
		nextValue += GET_BITS(value		,0	  ,dBit);
		REG_WD(currAddr		,currValue);
		REG_WD((currAddr+4) ,nextValue);
	} while (0);

	return ret;
}

int CVD_Get_FB_Status(LX_AFE_SCART_MODE_T *pScart_fb_mode)
{
	int ret = RET_OK;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Get_FB_Status(pScart_fb_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Get_FB_Status(pScart_fb_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Get_FB_Status(pScart_fb_mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Get_FB_Status(pScart_fb_mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Get_FB_Status(pScart_fb_mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Get_FB_Status(pScart_fb_mode);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

static int CVD_Get_No_Signal_Flag(void)
{
	int ret;
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_No_Signal_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_No_Signal_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_No_Signal_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_No_Signal_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_No_Signal_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_No_Signal_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_HLock_Flag(void)
{
	int ret;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_HLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_HLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_HLock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_HLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_HLock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_HLock_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_VLock_Flag(void)
{
	int ret;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_VLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_VLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_VLock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_VLock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_VLock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_VLock_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Vline_625_Flag(void)
{
	int ret;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_Vline_625_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_Vline_625_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_Vline_625_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_Vline_625_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_Vline_625_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_Vline_625_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

static int CVD_Get_Chromalock_Flag(void)
{
	int ret;
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_Chromalock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_Chromalock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_Chromalock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_Chromalock_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_Chromalock_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_Chromalock_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Noise_Status(void)
{
	int ret;
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_Noise_Status();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_Noise_Status();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_Noise_Status();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_Noise_Status();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_Noise_Status();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_Noise_Status();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Channel_Power_Control(UINT32 on_off)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Channel_Power_Control(on_off);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Channel_Power_Control(on_off);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Channel_Power_Control(on_off);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Channel_Power_Control(on_off);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Channel_Power_Control(on_off);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Channel_Power_Control(on_off);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Bypass_Control(LX_AFE_CVD_BYPASS_CONTROL_T *cvd_bypass_control_t)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Bypass_Control(cvd_bypass_control_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Bypass_Control(cvd_bypass_control_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Bypass_Control(cvd_bypass_control_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Bypass_Control(cvd_bypass_control_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Bypass_Control(cvd_bypass_control_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Bypass_Control(cvd_bypass_control_t);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Vline_625_Reg(void)
{
	int ret;
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_Vline_625_Reg();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_Vline_625_Reg();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_Vline_625_Reg();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_Vline_625_Reg();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_Vline_625_Reg();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_Vline_625_Reg();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Hstate_Max(UINT32	hstate_max_value)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Hstate_Max(hstate_max_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Hstate_Max(hstate_max_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Hstate_Max(hstate_max_value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Hstate_Max(hstate_max_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Hstate_Max(hstate_max_value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_Hstate_Max(hstate_max_value);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Crunky_Status(LX_AFE_CVD_CK_T *pCK_Detection_t)
{
	int ret;
#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_Crunky_Status(pCK_Detection_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_Crunky_Status(pCK_Detection_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_Crunky_Status(pCK_Detection_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_Crunky_Status(pCK_Detection_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_Crunky_Status(pCK_Detection_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_Crunky_Status(pCK_Detection_t);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Scart_FB_En(int fb_en_ctrl)
{
	int ret = RET_OK;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Scart_FB_En(fb_en_ctrl);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Scart_FB_En(fb_en_ctrl);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Scart_FB_En(fb_en_ctrl);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Scart_FB_En(fb_en_ctrl);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Scart_FB_En(fb_en_ctrl);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_Scart_FB_En(fb_en_ctrl);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_OnOff_VDAC(BOOLEAN bonoff)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_OnOff_VDAC(bonoff);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_OnOff_VDAC(bonoff);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_OnOff_VDAC(bonoff);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_OnOff_VDAC(bonoff);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_OnOff_VDAC(bonoff);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_OnOff_VDAC(bonoff);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Task_3cdetect(void)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Task_3cdetect();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Task_3cdetect();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Task_3cdetect();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Task_3cdetect();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Task_3cdetect();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Task_3cdetect();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

/* Test Code for H13B0 */
int CVD_Channel_Change_Control(BOOLEAN bATV_Channel_Change)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Channel_Change_Control(bATV_Channel_Change);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_VDAC_mute_control(int enable)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_VDAC_mute_control(enable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_VDAC_mute_control(enable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Bx_VDAC_mute_control(enable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_VDAC_mute_control(enable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = RET_ERROR;
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Init_Global_Params(void)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Init_Global_Params();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Init_Global_Params();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Init_Global_Params();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Init_Global_Params();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Init_Global_Params();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Init_Global_Params();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Color_System_3CS(CVD_STATE_T cvd_next_state)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Color_System_3CS(cvd_next_state);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Color_System_3CS(cvd_next_state);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Color_System_3CS(cvd_next_state);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Color_System_3CS(cvd_next_state);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Color_System_3CS(cvd_next_state);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_Color_System_3CS(cvd_next_state);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_Reg_States_Detail(LX_AFE_CVD_STATES_DETAIL_T	*pcvd_states_detail_t)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Get_Reg_States_Detail(pcvd_states_detail_t);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Internal_Demod_Mode(BOOLEAN bInternal_Demod)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Internal_Demod_Mode(bInternal_Demod);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Internal_Demod_Mode(bInternal_Demod);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Blue_Mode(int blue)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Blue_Mode(blue);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Blue_Mode(blue);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Blue_Mode(blue);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Blue_Mode(blue);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Blue_Mode(blue);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_OnOff_DMD_DAC(BOOLEAN bonoff)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_OnOff_DMD_DAC(bonoff);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_OnOff_DMD_DAC(bonoff);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Blend_Ratio(int value)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Blend_Ratio(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_Blend_Ratio(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Blend_Ratio(value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_Blend_Ratio(value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_Blend_Ratio(value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Vsync_Enhance(int system)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Vsync_Enhance(system);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Vsync_Enhance(system);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Set_Hsync_Enhance(int enable)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_Hsync_Enhance(enable);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_Hsync_Enhance(enable);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

/* CVD_Init 	: Initialize CVD registers to default value
					: Enables Kenel_threads(CVD main & CHB)
					: Enable CVD Interrupt 						*/
int CVD_Init(LX_AFE_CVD_SELECT_T select_main_sub)
{
	int ret;

	ret	= 0;

#ifdef	KDRV_CONFIG_PM
	gAfePmData = (S_AFE_PM_DATA *)kmalloc(sizeof(S_AFE_PM_DATA) , GFP_KERNEL);
	gAfePmData->CVDInitStatus = FALSE;
	gAfePmData->SelectMainSub = select_main_sub;
#endif

	if(select_main_sub != LX_CVD_MAIN)
		return -1;

	//printk("gRegCfg [0x%x]\n", pVportRegCfg->vport_reg_base_addr);

	/*-----------------------------------------------------------------------------------
	 * [H15/H14/H13/M14] CVD Register Base Address configuration
	 *----------------------------------------------------------------------------------*/
	if (lx_chip_rev() >= LX_CHIP_REV( H15, A0))
	{
		gpRealRegCVD_H15Ax = (volatile DE_CVD_REG_H15Ax_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_H15Ax == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_H15Ax = (volatile DE_CVD_REG_H15Ax_T __iomem *)kmalloc(sizeof(DE_CVD_REG_H15Ax_T), GFP_ATOMIC);
		if(gpRegCVD_H15Ax == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV( H14, A0))
	{
		gpRealRegCVD_H14Ax = (volatile DE_CVD_REG_H14Ax_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_H14Ax == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_H14Ax = (volatile DE_CVD_REG_H14Ax_T __iomem *)kmalloc(sizeof(DE_CVD_REG_H14Ax_T), GFP_ATOMIC);
		if(gpRegCVD_H14Ax == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		gpRealRegCVD_M14Bx = (volatile DE_CVD_REG_M14Bx_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_M14Bx == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_M14Bx = (volatile DE_CVD_REG_M14Bx_T __iomem *)kmalloc(sizeof(DE_CVD_REG_M14Bx_T), GFP_ATOMIC);
		if(gpRegCVD_M14Bx == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV( M14, A0))
	{
		gpRealRegCVD_M14Ax = (volatile DE_CVD_REG_M14Ax_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_M14Ax == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_M14Ax = (volatile DE_CVD_REG_M14Ax_T __iomem *)kmalloc(sizeof(DE_CVD_REG_M14Ax_T), GFP_ATOMIC);
		if(gpRegCVD_M14Ax == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV( H13, B0))
	{
		gpRealRegCVD_H13Bx = (volatile DE_CVD_REG_H13Bx_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_H13Bx == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_H13Bx = (volatile DE_CVD_REG_H13Bx_T __iomem *)kmalloc(sizeof(DE_CVD_REG_H13Bx_T), GFP_ATOMIC);
		if(gpRegCVD_H13Bx == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}
	}
	/*-----------------------------------------------------------------------------------
	 * [H13Ax] CVD Register Base Address configuration
	 *----------------------------------------------------------------------------------*/
	else if (lx_chip_rev() >= LX_CHIP_REV( H13, A0))
	{
		gpRealRegCVD_H13Ax = (volatile DE_CVD_REG_H13Ax_T __iomem *)ioremap(pVportRegCfg->vport_reg_base_addr , pVportRegCfg->vport_reg_size);
		if(gpRealRegCVD_H13Ax == NULL)
		{
			printk("ERROR : can't allocate for register\n");
			return -1;
		}
		gpRegCVD_H13Ax = (volatile DE_CVD_REG_H13Ax_T __iomem *)kmalloc(sizeof(DE_CVD_REG_H13Ax_T), GFP_ATOMIC);
		if(gpRegCVD_H13Ax == NULL)
		{
			printk("ERROR : can't allocate for shadow register\n");
			return -1;
		}


	}
	else
	{
	}
	//spin_lock_init(&cvd_lock);
	/*
	   CVD_DEBUG("Real [0x%x], Reg [0x%x]\n", gpRealRegCVD_H13Ax, gpRegCVD_H13Ax);
	   {
	   UINT32 readAddr;
	   UINT32  j;
	   for(j =0x0;j<0x200;j+=4)
	   {
	   readAddr = (UINT32)gpRealRegCVD_H13Ax + j;
	   CVD_DEBUG("Offset[0x%x], Real [0x%x]=value[0x%x]\n", j,  readAddr, REG_RD(readAddr) );
	   }
	   }
	 */

	CVD_Init_Global_Params();

	/* M14 Latchup debug */
	CVD_Channel_Power_Control(0);

	CVD_Power_Down(LX_CVD_MAIN, FALSE);

	CVD_Reg_Init(LX_CVD_MAIN);

	//Init Register to NTSC Mode
	CVD_Set_Initial_Color_System();

#ifdef CVD_CH42_SKIP_WORKAROUND
	CVD_Clamp_Current_Control(0x3);
#endif

//	CVD_SW_Reset(LX_CVD_MAIN);

#ifdef ENABLE_CVD_THREAD

#if CVD_THREAD_VSYNC_CONTROL
	stCVD_Thread = kthread_create( MainCVD_Thread_Vsync_Ctrl, (void*)NULL, "cvd_thread");
#else
	stCVD_Thread = kthread_create( MainCVD_Thread, (void*)NULL, "cvd_thread");
#endif

	if(stCVD_Thread)
	{
		wake_up_process(stCVD_Thread);
		//gCVD_thread_running = 1;
		AFE_PRINT("CVD Thread [%d]\n", stCVD_Thread->pid);
	}


	else
	{
		AFE_ERROR("CVD Thread Already Created\n");
	}

	if(!IS_CVD_INTERRUPT_REGISTERED){

		if(lx_chip_rev() >= LX_CHIP_REV( H15,A0)){
			AFE_PRINT("CVD Interrupt registering to H15 Ax\n");
			ret = request_irq(H15_IRQ_CVD_BCPU, (irq_handler_t)CVD_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( H14,A0)){
			AFE_PRINT("CVD Interrupt registering to H14 Ax\n");
			ret = request_irq(H14_IRQ_CVD_BCPU, (irq_handler_t)CVD_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0)){
			AFE_PRINT("CVD Interrupt registering to M14 Bx\n");
			ret = request_irq(M14_B0_IRQ_DE_IDS_CVD, (irq_handler_t)CVD_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0)){
			AFE_PRINT("CVD Interrupt registering to M14 Ax\n");
			ret = request_irq(M14_A0_IRQ_CVD_BCPU, (irq_handler_t)CVD_IRQ_Handler, 0, "CVD", NULL);
		}
		else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0)){
			AFE_PRINT("CVD Interrupt registering to H13... \n");
			ret = request_irq(H13_IRQ_CVD_BCPU, (irq_handler_t)CVD_IRQ_Handler, 0, "CVD", NULL);
		}

		if(ret) AFE_ERROR("ERROR! CVD Interrupt(request_irq) has failed! CVD will not work properly!\n");
		else AFE_PRINT("CVD Interrupt register succeed!\n");

		IS_CVD_INTERRUPT_REGISTERED = true;
	}

#endif
	CVD_SetInterrupt(1);	//Enable CVD Vsync Interrupt

	CVD_Power_Down(LX_CVD_SUB, FALSE);

	gCVDInitStatus = TRUE;

	return 0;
}

void CVD_UnInit(void)
{
	if(gCVDInitStatus)
	{
		if(IS_CVD_INTERRUPT_REGISTERED){
			if(lx_chip_rev() >= LX_CHIP_REV( H15,A0))
				free_irq(H15_IRQ_CVD_BCPU, NULL);
			else if(lx_chip_rev() >= LX_CHIP_REV( H14,A0))
				free_irq(H14_IRQ_CVD_BCPU, NULL);
			else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0))
				free_irq(M14_B0_IRQ_DE_IDS_CVD, NULL);
			else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0))
				free_irq(M14_A0_IRQ_CVD_BCPU, NULL);
			else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0))
				free_irq(H13_IRQ_CVD_BCPU, NULL);

			IS_CVD_INTERRUPT_REGISTERED = false;
		}
		gCVDInitStatus = FALSE;
#ifdef	KDRV_CONFIG_PM
		kfree(gAfePmData);
#endif
	}

}


int CVD_Set_NTSC_Only(UINT32 arg)
{
	LX_AFE_VIDEO_SYSTEM_INFO_T system_info;

	if (copy_from_user(&system_info, (void __user *)arg, sizeof(LX_AFE_VIDEO_SYSTEM_INFO_T)))
	{
		return -1;
	}

	if(system_info.cvd_video_system == LX_COLOR_SYSTEM_NTSC_M)
	{
		AFE_PRINT("############ Setting NTSC ONLY MODE ##################\n");
		g_CVD_Status_3CS.cvd_ntsc_only_mode = TRUE;
	}
	else
		g_CVD_Status_3CS.cvd_ntsc_only_mode = FALSE;

	return 0;
}

static int MainCVD_Thread(void *data)	// not used
{
	while(1)
	{
		CVD_THREAD_DEBUG("gForce_thread_sleep[%d], gCVD_thread_running[%d] \n", gForce_thread_sleep, gCVD_thread_running);
		if(gForce_thread_sleep > 0)
		{
			CVD_THREAD_DEBUG("User force sleep CVD Thread\n");
			//gForce_thread_sleep = 0;
			gCVD_thread_running = 0;
			CVD_THREAD_DEBUG("CVD Sleep : gForce_thread_sleep[%d], gCVD_thread_running[%d] \n", gForce_thread_sleep, gCVD_thread_running);

			wait_event_interruptible(WaitQueue_CVD , gCVD_thread_running != 0);
			//interruptible_sleep_on(&WaitQueue_CVD);

			// added : in case CVD power off/on h/v unlock sometimes do not occur.
			g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoReadyToDetect;
			//	g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;
			//	g_CVD_Status_3CS.cvd_prev_color_system  = CVD_STATE_VideoNotReady;

//			gCVD_thread_running = 1;
			CVD_THREAD_DEBUG("CVD Wakeup : gForce_thread_sleep[%d], gCVD_thread_running[%d] \n", gForce_thread_sleep, gCVD_thread_running);
			//gWait_return = wait_event_interruptible(WaitQueue_CVD, gForce_thread_sleep>0);
		}

		CVD_Task_3cdetect();

		OS_MsecSleep(L9_CVD_TIMEOUT);
	}

	return 0;
}

int CVD_Start_Timer(UINT32 arg)
{
#ifdef ENABLE_CVD_THREAD

#if CVD_THREAD_VSYNC_CONTROL
	if(gForce_thread_sleep)
	{
		// added : in case CVD power off/on h/v unlock sometimes do not occur.
		g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoReadyToDetect;

		gForce_thread_sleep = 0;
		CVD_DEBUG("Starting CVD Thread\n");
	}
	else
		CVD_DEBUG("CVD Thread already running !!! : gForce_thread_sleep[%d], gCVD_thread_running[%d] \n", gForce_thread_sleep, gCVD_thread_running);
#else // not used
	if(gForce_thread_sleep)
	{
		CVD_DEBUG("CVD Thread forced sleep, but re-starting thread !!!\n");
	}

	gForce_thread_sleep = 0;
	gCVD_thread_running = 1;
	wake_up_interruptible(&WaitQueue_CVD);
	CVD_DEBUG("Starting CVD Thread\n");
#endif

#endif
	return 0;
}

int CVD_Stop_Timer(UINT32 arg)
{
#ifdef ENABLE_CVD_THREAD
		CVD_DEBUG("Stopping CVD Thread\n");
		gForce_thread_sleep = 1;

	return 0;
#endif
}

static int MainCVD_Thread_Vsync_Ctrl(void *data)
{
	while(1)
	{
		gCVD_thread_running = 0;
		//interruptible_sleep_on(&WaitQueue_CVD);
		wait_event_interruptible(WaitQueue_CVD , gCVD_thread_running != 0 );
//		gCVD_thread_running = 1;

		CVD_Task_3cdetect();
	}
	return 0;
}

int CVD_Vsync_Interrupt(void)
{
#ifdef ENABLE_CVD_THREAD
	static int vsync_intr_count = 0;

	if(gForce_thread_sleep == 0)
		vsync_intr_count ++;
	else
	{
		CVD_THREAD_DEBUG("CVD Thread Sleeping ~~\n");
		return 0;
	}

	if(vsync_intr_count > 2)	// 20msec *3 = 60msec, 16.7msec *3 = 50msec
	{
		if( gCVD_thread_running == 0 ) 
		{
			AFE_TRACE("Wake Up CVD Thread !!!\n");
			vsync_intr_count = 0;
			gCVD_thread_running = 1;
			wake_up_interruptible(&WaitQueue_CVD);
		}
		else
			CVD_THREAD_DEBUG("Previous CVD Thread not finished !!!\n");
	}
#endif
	return 0;
}

static irqreturn_t CVD_IRQ_Handler(int irq, void *dev_id, struct pt_regs *regs)
{
	UINT32 cvd_int_status = 0;
	UINT32 vbi_int_status = 0;

	/* Step 1 : Get interrupt status */
	if( lx_chip_rev() >= LX_CHIP_REV( H15, A0))
	{
		CVD_H15Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H15Ax_Rd(cvd_intr1_intr0);

		CVD_H15Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H15Ax_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if((vbi_int_status & H15A0_VBI_INTERRUPT_MASK) )
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		if((cvd_int_status & H15A0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_H15Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H15Ax_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_H15Ax_WrFL(cvd_intr1_clear0);
		CVD_H15Ax_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( H14, A0))
	{
		CVD_H14Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H14Ax_Rd(cvd_intr1_intr0);

		CVD_H14Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H14Ax_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if((vbi_int_status & H14A0_VBI_INTERRUPT_MASK) )
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		if((cvd_int_status & H14A0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_H14Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H14Ax_Wr(cvd_intr1_clear1, vbi_int_status);

		CVD_H14Ax_WrFL(cvd_intr1_clear0);
		CVD_H14Ax_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		CVD_M14Bx_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_M14Bx_Rd(cvd_intr1_intr0);

		CVD_M14Bx_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_M14Bx_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & M14B0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}

		}

		if((cvd_int_status & M14B0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_M14Bx_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_M14Bx_Wr(cvd_intr1_clear1, vbi_int_status);
			
		CVD_M14Bx_WrFL(cvd_intr1_clear0);
		CVD_M14Bx_WrFL(cvd_intr1_clear1);

	}
	else if( lx_chip_rev() >= LX_CHIP_REV( M14, A0))
	{
		CVD_M14Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_M14Ax_Rd(cvd_intr1_intr0);

		CVD_M14Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_M14Ax_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);
		
		if(vbi_int_status & M14A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		if((cvd_int_status & M14A0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_M14Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_M14Ax_Wr(cvd_intr1_clear1, vbi_int_status);
		
		CVD_M14Ax_WrFL(cvd_intr1_clear0);
		CVD_M14Ax_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( H13, B0))
	{
		CVD_H13Bx_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H13Bx_Rd(cvd_intr1_intr0);

		CVD_H13Bx_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H13Bx_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & H13A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}

		if((cvd_int_status & H13B0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_H13Bx_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H13Bx_Wr(cvd_intr1_clear1, vbi_int_status);
		
		CVD_H13Bx_WrFL(cvd_intr1_clear0);	
		CVD_H13Bx_WrFL(cvd_intr1_clear1);
	}
	else if( lx_chip_rev() >= LX_CHIP_REV( H13, A0))
	{
		CVD_H13Ax_RdFL(cvd_intr1_intr0);
		cvd_int_status = CVD_H13Ax_Rd(cvd_intr1_intr0);

		CVD_H13Ax_RdFL(cvd_intr1_intr1);
		vbi_int_status = CVD_H13Ax_Rd(cvd_intr1_intr1);

		AFE_TRACE("CVD ISR : intr0[0x%08x] intr1[0x%08x]\n", cvd_int_status, vbi_int_status);

		if(vbi_int_status & H13A0_VBI_INTERRUPT_MASK)
		{
			if(ENABLE_ANALOGCC){
				VBI_CC_InterruptFunc(vbi_int_status);
			}

			if(ENABLE_ANALOGTTX){
				VBI_TTX_InterruptFunc(vbi_int_status);
			}
		}
	
		if((cvd_int_status & H13A0_VSYNC_INTERRUPT_MASK) )
		{
			CVD_Vsync_Interrupt();
		}

		CVD_H13Ax_Wr(cvd_intr1_clear0, cvd_int_status);
		CVD_H13Ax_Wr(cvd_intr1_clear1, vbi_int_status);
		
		CVD_H13Ax_WrFL(cvd_intr1_clear0);
		CVD_H13Ax_WrFL(cvd_intr1_clear1);
	
	}
	else{
		AFE_ERROR("WARNING! Unknown chip revision at CVD IRQ!\n");
		// Add statement for other chip version
	}

	return IRQ_HANDLED;
}

static int CVD_SetInterruptClear(void)
{
	int ret = 0;
	UINT32 cvd_intr1_clear;

	if(lx_chip_rev() >= LX_CHIP_REV( H15,A0)){

		CVD_H15Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H15Ax_Rd(cvd_intr1_clear0);
		CVD_H15Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H15Ax_WrFL(cvd_intr1_clear0);

		CVD_H15Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H15Ax_Rd(cvd_intr1_clear0);
		CVD_H15Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H15Ax_WrFL(cvd_intr1_clear0);

	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H14,A0)){

		CVD_H14Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H14Ax_Rd(cvd_intr1_clear0);
		CVD_H14Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H14Ax_WrFL(cvd_intr1_clear0);

		CVD_H14Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H14Ax_Rd(cvd_intr1_clear0);
		CVD_H14Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H14Ax_WrFL(cvd_intr1_clear0);

	}
	else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0)){

		CVD_M14Bx_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_M14Bx_Rd(cvd_intr1_clear0);
		CVD_M14Bx_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_M14Bx_WrFL(cvd_intr1_clear0);

		CVD_M14Bx_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_M14Bx_Rd(cvd_intr1_clear0);
		CVD_M14Bx_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_M14Bx_WrFL(cvd_intr1_clear0);

	}
	else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0)){

		CVD_M14Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_M14Ax_Rd(cvd_intr1_clear0);
		CVD_M14Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_M14Ax_WrFL(cvd_intr1_clear0);

		CVD_M14Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_M14Ax_Rd(cvd_intr1_clear0);
		CVD_M14Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_M14Ax_WrFL(cvd_intr1_clear0);

	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H13,B0)){

		CVD_H13Bx_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H13Bx_Rd(cvd_intr1_clear0);
		CVD_H13Bx_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H13Bx_WrFL(cvd_intr1_clear0);

		CVD_H13Bx_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H13Bx_Rd(cvd_intr1_clear0);
		CVD_H13Bx_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H13Bx_WrFL(cvd_intr1_clear0);

	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0)){

		CVD_H13Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H13Ax_Rd(cvd_intr1_clear0);
		CVD_H13Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear |= M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H13Ax_WrFL(cvd_intr1_clear0);

		CVD_H13Ax_RdFL(cvd_intr1_clear0);
		cvd_intr1_clear = CVD_H13Ax_Rd(cvd_intr1_clear0);
		CVD_H13Ax_Wr(cvd_intr1_clear0, (cvd_intr1_clear &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_H13Ax_WrFL(cvd_intr1_clear0);

	}
	else{
		AFE_ERROR("WARNING! Unknown chip revision at CVD IRQ!\n");
		// Add statement for other chip version
	}

	return ret;
}

int CVD_SetInterrupt(int on_off)
{
	int ret = 0;

	UINT32 cvd_intr1_en;

	if(lx_chip_rev() >= LX_CHIP_REV( H15,A0)){
		CVD_H15Ax_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_H15Ax_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_H15Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en |= H15A0_VSYNC_INTERRUPT_MASK));
		else
			CVD_H15Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~H15A0_VSYNC_INTERRUPT_MASK));
		CVD_H15Ax_WrFL(cvd_intr1_enable0);
	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H14,A0)){
		CVD_H14Ax_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_H14Ax_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_H14Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en |= H14A0_VSYNC_INTERRUPT_MASK));
		else
			CVD_H14Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~H14A0_VSYNC_INTERRUPT_MASK));
		CVD_H14Ax_WrFL(cvd_intr1_enable0);
	}
	else if(lx_chip_rev() >= LX_CHIP_REV( M14,B0)){
		CVD_M14Bx_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_M14Bx_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_M14Bx_Wr(cvd_intr1_enable0, (cvd_intr1_en |= M14B0_VSYNC_INTERRUPT_MASK));
		else
			CVD_M14Bx_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~M14B0_VSYNC_INTERRUPT_MASK));
		CVD_M14Bx_WrFL(cvd_intr1_enable0);
	}
	else if(lx_chip_rev() >= LX_CHIP_REV( M14,A0)){
		CVD_M14Ax_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_M14Ax_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_M14Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en |= M14A0_VSYNC_INTERRUPT_MASK));
		else
			CVD_M14Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~M14A0_VSYNC_INTERRUPT_MASK));
		CVD_M14Ax_WrFL(cvd_intr1_enable0);
	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H13,B0)){
		CVD_H13Bx_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_H13Bx_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_H13Bx_Wr(cvd_intr1_enable0, (cvd_intr1_en |= H13B0_VSYNC_INTERRUPT_MASK));
		else
			CVD_H13Bx_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~H13B0_VSYNC_INTERRUPT_MASK));
		CVD_H13Bx_WrFL(cvd_intr1_enable0);
	}
	else if(lx_chip_rev() >= LX_CHIP_REV( H13,A0)){
		CVD_H13Ax_RdFL(cvd_intr1_enable0);
		cvd_intr1_en = CVD_H13Ax_Rd(cvd_intr1_enable0);
		if(on_off)
			CVD_H13Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en |= H13A0_VSYNC_INTERRUPT_MASK));
		else
			CVD_H13Ax_Wr(cvd_intr1_enable0, (cvd_intr1_en &= ~H13A0_VSYNC_INTERRUPT_MASK));
		CVD_H13Ax_WrFL(cvd_intr1_enable0);
	}
	else{
		AFE_ERROR("WARNING! Unknown chip revision at CVD IRQ!\n");
		// Add statement for other chip version
	}

	if(on_off == 0)
		CVD_SetInterruptClear();

	return ret;
}

int CVD_Program_Color_System(LX_AFE_CVD_PROGRAM_COLOR_SYSTEM_T program_color_system_t)
{

	if(program_color_system_t.color_system >= LX_NON_STANDARD ) return -1;

	CVD_Set_Color_System_3CS(program_color_system_t.color_system);

	CVD_DEBUG("CVD Main : Program Color System to [%d]\n", program_color_system_t.color_system);

	return 0;
}

#ifdef	KDRV_CONFIG_PM
int CVD_RunSuspend(void)
{
	LX_AFE_CVD_SET_INPUT_T	cvd_input_info = { LX_CVD_MAX_NUM, LX_CVD_INPUT_SOURCE_NUM, LX_AFE_CVBS_NONE, LX_AFE_CVBS_NONE, LX_CVD_INPUT_SOURCE_ATTRIBUTE_NUM} ;
	/*
	if (!gAfePmData->CVDInitStatus)
		return 0;
		*/
	AFE_PRINT(" CVD Suspending ... : port setting to initial \n");
	CVD_Set_Source_Type_Control(cvd_input_info);

	return 0;
}

int CVD_RunResume(void)
{

	/*
	if (!gAfePmData->CVDInitStatus)
		return 0;
		*/

	CVD_Init_Global_Params();

	//Initialize CVD semaphore
	//OS_InitMutex(&g_CVD_Sema, OS_SEM_ATTR_DEFAULT);

	/* M14 Latchup debug */
	CVD_Channel_Power_Control(0);

	CVD_Power_Down(LX_CVD_MAIN, FALSE);

	CVD_Reg_Init(LX_CVD_MAIN);

	//Init Register to NTSC Mode
	CVD_Set_Initial_Color_System();

#ifdef CVD_CH42_SKIP_WORKAROUND
	CVD_Clamp_Current_Control(0x3);
#endif

	CVD_SetInterrupt(1);	//Enable CVD Vsync Interrupt

	CVD_Power_Down(LX_CVD_SUB, FALSE);

	return 0;
}

#endif


int CVD_Set_SW_3DCOMB_Control(UINT32 arg)
{
	return 0;
}

int CVD_Set_Source_Type_Control(LX_AFE_CVD_SET_INPUT_T	cvd_input_info)
{

	static LX_AFE_CVD_SET_INPUT_T	prev_cvd_input_info = { LX_CVD_MAX_NUM, LX_CVD_INPUT_SOURCE_NUM, LX_AFE_CVBS_NONE, LX_AFE_CVBS_NONE, LX_CVD_INPUT_SOURCE_ATTRIBUTE_NUM} ;

	//AFE_PRINT("Entered CVD_Set_Source_Type_Control\n");

	if( (cvd_input_info.cvd_main_sub == prev_cvd_input_info.cvd_main_sub) && (cvd_input_info.cvd_input_source_type == prev_cvd_input_info.cvd_input_source_type) && (cvd_input_info.cvbs_input_port == prev_cvd_input_info.cvbs_input_port) && (cvd_input_info.cvd_input_source_attribute == prev_cvd_input_info.cvd_input_source_attribute))  // Same Input Setting.
	{
		AFE_PRINT("CVD Set Source Type to Same Port !!!!????\n");
		return 0;
	}
	else
		memcpy (&prev_cvd_input_info, &cvd_input_info, sizeof(LX_AFE_CVD_SET_INPUT_T)) ;	// Save Current Port Settings.

	CVD_Set_Source_Type(cvd_input_info);

	//		AFE_PRINT("Main CVD input\n");
	if(cvd_input_info.cvd_input_source_attribute == LX_CVD_INPUT_SOURCE_ATTRIBUTE_RF)
	{
		CVD_DEBUG("CVD : RF Input Mode\n");
		g_CVD_Status_3CS.in_rf_mode = TRUE;
	}
	else
	{
		CVD_DEBUG("CVD : AV Input Mode\n");
		g_CVD_Status_3CS.in_rf_mode = FALSE;
	}

//	g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoNotReady;
//	g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;
//	g_CVD_Status_3CS.cvd_prev_color_system  = CVD_STATE_VideoNotReady;		//121228 Added
	g_CVD_Status_3CS.port_change_detected = TRUE;

	return 0;

}

int CVD_Get_States_Detail(UINT32 arg)
{
	LX_AFE_CVD_STATES_DETAIL_T	cvd_states_detail_t;
	//UINT32 status_reg;

	CVD_Get_Reg_States_Detail(&cvd_states_detail_t);

	cvd_states_detail_t.MainCVD_State = g_CVD_Status_3CS.cvd_color_system;
	cvd_states_detail_t.Current_State = g_CVD_Status_3CS.cvd_color_system;
	cvd_states_detail_t.Next_State = g_CVD_Status_3CS.cvd_next_state;


	cvd_states_detail_t.CVD_Color_System = g_CVD_Status_3CS.cvd_color_system;
	cvd_states_detail_t.CS0_Color_System = g_CVD_Status_3CS.cs0_color_system;
	cvd_states_detail_t.CS1_Color_System = g_CVD_Status_3CS.cs1_color_system;
	cvd_states_detail_t.color_system_support = g_CVD_Status_3CS.color_system_support;

	cvd_states_detail_t.motion_diff = g_CVD_Pattern_Detection_t.global_motion_diff;
	cvd_states_detail_t.pattern_found = g_CVD_Pattern_Detection_t.pattern_found;
	cvd_states_detail_t.static_pattern_found = g_CVD_Pattern_Detection_t.static_pattern_found;
	cvd_states_detail_t.tunning_mode	= g_CVD_Status_3CS.in_atv_tunning;
	cvd_states_detail_t.lock_stable_count	= g_CVD_Status_3CS.cvd_lock_stable_count;
	cvd_states_detail_t.no_signal_count	= g_CVD_Status_3CS.cvd_no_signal_count;
	cvd_states_detail_t.black_level	= g_CVD_Status_3CS.cvd_black_level;

	if (copy_to_user((UINT32*)arg, (UINT32*)&cvd_states_detail_t, sizeof(LX_AFE_CVD_STATES_DETAIL_T)))
	{
		return -1;
	}

	return 0;
}


int CVD_Test_Program(LX_AFE_CVD_TEST_PARAM_T *stCVD_Test_Param)
{
	int ret = 0;

	switch(stCVD_Test_Param -> item_to_test)
	{
		case CVD_TEST_ENABLE_3DCOMB_STATE_PRINT:
			break;
		case CVD_TEST_HSTATEMAX_WRITE:
			CVD_Set_Hstate_Max(stCVD_Test_Param->hstate_max_value);
			break;
		case CVD_TEST_AGC_PEAK_NOMINAL:
			/*
			g_CVD_AGC_Peak_Control.bEnable = 			stCVD_Test_Param->agc_peak_enable;
			g_CVD_AGC_Peak_Control.white_ratio_th = 	stCVD_Test_Param->agc_pean_white_ratio_th;
			g_CVD_AGC_Peak_Control.white_frame_max_th = stCVD_Test_Param->agc_pean_white_frame_max_th;
			g_CVD_AGC_Peak_Control.white_frame_on = 	stCVD_Test_Param->agc_pean_white_frame_on;
			g_CVD_AGC_Peak_Control.white_frame_off = 	stCVD_Test_Param->agc_pean_white_frame_off;
			g_CVD_AGC_Peak_Control.print_en = 			stCVD_Test_Param->agc_pean_print_en;
			g_CVD_AGC_Peak_Nominal_Control.bEnable 				= stCVD_Test_Param->agc_peak_enable ;
			g_CVD_AGC_Peak_Nominal_Control.tnr_x_avg_t_th 		= stCVD_Test_Param->agc_peak_x_avg_t_th;
			g_CVD_AGC_Peak_Nominal_Control.tnr_x_avg_s_th 		= stCVD_Test_Param->agc_peak_x_avg_s_th;
			g_CVD_AGC_Peak_Nominal_Control.tpd_s_status_th 		= stCVD_Test_Param->agc_peak_s_staus_th;
			g_CVD_AGC_Peak_Nominal_Control.pattern_frame_max_th = stCVD_Test_Param->agc_peak_white_frame_max_th ;
			g_CVD_AGC_Peak_Nominal_Control.pattern_frame_on 	= stCVD_Test_Param->agc_peak_white_frame_on ;
			g_CVD_AGC_Peak_Nominal_Control.pattern_frame_off 	= stCVD_Test_Param->agc_peak_white_frame_off ;
			g_CVD_AGC_Peak_Nominal_Control.noise_th			 	= stCVD_Test_Param->agc_peak_noise_th ;
			g_CVD_AGC_Peak_Nominal_Control.print_en 			= stCVD_Test_Param->agc_peak_print_en ;
			*/
			break;
		case CVD_TEST_STABLE_SYNC_MODE:
#ifdef USE_AFE_KDRV_FOR_H15
			if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 )) {
				CVD_H15Ax_Enable_Stable_Sync_Mode(stCVD_Test_Param->stable_sync_value);		//for RF nosignal (dcrestore_lpf_en ON)
			} else 
#endif
#ifdef USE_AFE_KDRV_FOR_H14
			if ( lx_chip_rev() >= LX_CHIP_REV( H14, A0 )) {
				CVD_H14Ax_Enable_Stable_Sync_Mode(stCVD_Test_Param->stable_sync_value);		//for RF nosignal (dcrestore_lpf_en ON)
			} else 
#endif
#ifdef USE_AFE_KDRV_FOR_M14
			if ( lx_chip_rev() >= LX_CHIP_REV( M14, B0 )) {
				CVD_M14Bx_Enable_Stable_Sync_Mode(stCVD_Test_Param->stable_sync_value);		//for RF nosignal (dcrestore_lpf_en ON)
			}
			else if ( lx_chip_rev() >= LX_CHIP_REV( M14, A0 )) {
				CVD_M14Ax_Enable_Stable_Sync_Mode(stCVD_Test_Param->stable_sync_value);		//for RF nosignal (dcrestore_lpf_en ON)
			} else 
#endif
#ifdef USE_AFE_KDRV_FOR_H13
			if ( lx_chip_rev() >= LX_CHIP_REV( H13, B0 )) {
				CVD_H13Bx_Enable_Stable_Sync_Mode(stCVD_Test_Param->stable_sync_value);		//for RF nosignal (dcrestore_lpf_en ON)
			}
			break;
#endif
		case CVD_TEST_COMB2D_ONLY:
#ifdef USE_AFE_KDRV_FOR_H13
			if ( lx_chip_rev() == LX_CHIP_REV( H13, B0 )) {
				CVD_H13Bx_Comb2d_Only_Test(1, stCVD_Test_Param->comb2d_only_test_enable, stCVD_Test_Param->comb2d_only_on_time, \
						stCVD_Test_Param->comb2d_only_off_time, stCVD_Test_Param->comb2d_only_md_on_time, stCVD_Test_Param->comb2d_only_md_off_time);
			}
#endif
			break;
		case CVD_TEST_ADEMOD_TYPE:
			{
				BOOLEAN bInternal_Demod;

				if ( stCVD_Test_Param->internal_demod_type )
					bInternal_Demod = TRUE;
				else
					bInternal_Demod = FALSE;

				CVD_Set_Internal_Demod_Mode(bInternal_Demod);
			}
			break;
		case CVD_TEST_BLUE_MODE:
			{
				CVD_Set_Blue_Mode(stCVD_Test_Param->blue_mode_enable);
			}
			break;
		case CVD_TEST_DMD_OUT:
			{
				CVD_OnOff_DMD_DAC(stCVD_Test_Param->dmd_out_enable);
			}
			break;
		case CVD_TEST_SCART_RGB:
			{
				CVD_Set_Scart_FB_En(stCVD_Test_Param->fb_en);
				CVD_Set_Blend_Ratio(stCVD_Test_Param->blend_ratio);
			}
			break;
		case CVD_TEST_HSYNC_ENHANCE:
			{
				CVD_Set_Hsync_Enhance(stCVD_Test_Param->hsync_enhance_value);
			}
			break;
		case CVD_TEST_VSYNC_ENHANCE:
			{
				CVD_Set_Vsync_Enhance(stCVD_Test_Param->vsync_enhance_value);
			}
		case CVD_TEST_YCSEP:
			{
				CVD_Set_ycsep_Blend(stCVD_Test_Param->ycsep_blend_value);
			}
			break;
		case CVD_TEST_CLAMP_UPDN:
			{
				CVD_Set_clampagc_updn(stCVD_Test_Param->clamp_updn_value);
			}
			break;
		case CVD_TEST_DC_CLAMP_MODE:
			{
				CVD_Set_dc_clamp_mode(stCVD_Test_Param->dc_clamp_mode_value);
			}
			break;
		case CVD_TEST_ABB_CLAMP_PARAM:
			{
				if ( ( lx_chip_rev() < LX_CHIP_REV( H14, A0 ) ) && ( lx_chip_rev() >= LX_CHIP_REV( M14, B0 )) ) {
					AFE_PRINT("ABB Clamp Parameter Setting : clamp_count[0x%x], clamp_step_size[0x%x]\n", \
							stCVD_Test_Param->abb_clamp_count, stCVD_Test_Param->abb_clamp_step );
					ret = DEMOD_ANALOG_Set_ClampingCtrl((UINT8)stCVD_Test_Param->abb_clamp_count, (UINT16)stCVD_Test_Param->abb_clamp_step);
				}
			}
			break;
		default:
			ret = -1;
			break;
	}

	return ret;
}

void CVD_Get_Timing_Info(UINT32 arg)
{
	LX_AFE_CVD_TIMING_INFO_T	timing_info;
	CVD_STATE_T	cvd_state;

	if (copy_from_user(&timing_info, (void __user *)arg, sizeof(LX_AFE_CVD_TIMING_INFO_T)))
	{
		return;
	}
	cvd_state = g_CVD_Status_3CS.cvd_color_system;

	switch(cvd_state)
	{
		case CVD_STATE_NTSC:
			timing_info.cvd_standard = LX_NTSC_M;
			timing_info.u16_VFreq = 599;
			break;
		case CVD_STATE_NTSC443:
			timing_info.cvd_standard = LX_NTSC_443;
			timing_info.u16_VFreq = 599;
			break;
		case CVD_STATE_PALi:
			timing_info.cvd_standard = LX_PAL_BG;
			timing_info.u16_VFreq = 500;
			break;
		case CVD_STATE_PALm:
			timing_info.cvd_standard = LX_PAL_M;
			timing_info.u16_VFreq = 599;
			break;
		case CVD_STATE_PALCn:
			timing_info.cvd_standard = LX_PAL_CN;
			timing_info.u16_VFreq = 500;
			break;
		case CVD_STATE_PAL60:
			timing_info.cvd_standard = LX_PAL_60;
			timing_info.u16_VFreq = 599;
			break;
		case CVD_STATE_SECAM:
			timing_info.cvd_standard = LX_SECAM;
			timing_info.u16_VFreq = 500;
			break;
		default:
			timing_info.cvd_standard = LX_NON_STANDARD;//gogosing
			if (CVD_Get_Vline_625_Reg())	// 625 line : 50Hz System
				timing_info.u16_VFreq = 500;
			else
				timing_info.u16_VFreq = 599;
			break;
	}

	if( g_CVD_Status_3CS.vline_625_stable )		// stable 50Hz
		timing_info.u16_VFreq_Stable = 500;
	else
		timing_info.u16_VFreq_Stable = 599;

	timing_info.cvd_lock_stable_count = g_CVD_Status_3CS.cvd_lock_stable_count;
	timing_info.cvd_no_signal_count = g_CVD_Status_3CS.cvd_no_signal_count;
	/*
	//Instead of reading input VFreq., read forced VFreq. of CVD to determine CVD's running VFreq.
	//if (CVD_Get_Vline_625_Flag())	// 625 line : 50Hz System
	if (CVD_Get_Vline_625_Reg())	// 625 line : 50Hz System
			timing_info.u16_VFreq = 500;
	else
			timing_info.u16_VFreq = 599;
			*/

	if (copy_to_user((UINT32*)arg, (UINT32*)&timing_info, sizeof(LX_AFE_CVD_TIMING_INFO_T)))
	{
		return;
	}
}
int CVD_Get_States(UINT32 arg)
{
	LX_AFE_CVD_STATES_INFO_T	cvd_states_info;
	//UINT32 status_reg;

	if (copy_from_user(&cvd_states_info, (void __user *)arg, sizeof(LX_AFE_CVD_STATES_INFO_T)))
	{
		return -1;
	}

#ifdef USE_REGISTER_VALUE_FOR_SYNC_STATES
	/* Returns Current Sync Register values(NOT the status of CVD thread) */

	cvd_states_info.cvd_status =0;

	if(CVD_Get_No_Signal_Flag())
	{
		//110414 No_Signal_Flag is not working well
		cvd_states_info.cvd_status |= LX_NO_SIGNAL;
		//		CVD_ClampAGC_OnOff(0); // agc off on No signal for ch42 no signal workaround
	}
	if(CVD_Get_HLock_Flag())
		cvd_states_info.cvd_status |= LX_HLOCK;
	if(CVD_Get_VLock_Flag())
		cvd_states_info.cvd_status |= LX_VLOCK;
	if(g_CVD_Status_3CS.cvd_chromalock_flag)
		cvd_states_info.cvd_status |= LX_CHROMALOCK;
	if(g_CVD_Status_3CS.cvd_pal_flag)
		cvd_states_info.cvd_status |= LX_PAL_DETECTED;
	if(g_CVD_Status_3CS.cvd_secam_flag)
		cvd_states_info.cvd_status |= LX_SECAM_DETECTED;
	if(CVD_Get_Vline_625_Flag())
		cvd_states_info.cvd_status |= LX_625LINES_DETECTED;
	if(g_CVD_Status_3CS.cs0_secam_flag)
		cvd_states_info.cvd_status |= LX_CS0_SECAM_DETECTED;
	if(g_CVD_Status_3CS.cs0_pal_flag)
		cvd_states_info.cvd_status |= LX_CS0_PAL_DETECTED;
	if(g_CVD_Status_3CS.cs0_chromalock_flag)
		cvd_states_info.cvd_status |= LX_CS0_CHROMALOCK;
	if(CVD_Get_HNon_Standard_Flag())
		cvd_states_info.cvd_status |= LX_HNON_STANDARD;
	if(CVD_Get_VNon_Standard_Flag())
		cvd_states_info.cvd_status |= LX_VNON_STANDARD;
#else
	/* Returns Sync Status of CVD thread */
	if(g_CVD_Status_3CS.cvd_color_system < CVD_STATE_VideoNotReady)
		cvd_states_info.cvd_status = LX_HLOCK|LX_VLOCK;
	else
		cvd_states_info.cvd_status = LX_NO_SIGNAL;

#endif
	if (copy_to_user((UINT32*)arg, (UINT32*)&cvd_states_info, sizeof(LX_AFE_CVD_STATES_INFO_T)))
	{
		return -1;
	}

	return 0;
}

int CVD_Set_Color_System(UINT32 arg)
{
	LX_AFE_VIDEO_SYSTEM_INFO_T system_info;

	if (copy_from_user(&system_info, (void __user *)arg, sizeof(LX_AFE_VIDEO_SYSTEM_INFO_T)))
	{
		return -1;
	}

	if(system_info.cvd_video_system <= LX_COLOR_SYSTEM_MULTI)
	{
		g_CVD_Status_3CS.color_system_support = system_info.cvd_video_system;
		CVD_DEBUG("### Setting Color System to [0x%x] ###\n", g_CVD_Status_3CS.color_system_support);
	}
	else
	{
		CVD_DEBUG("### Invalid Color System [%d] ###\n", system_info.cvd_video_system);
		return -1;
	}

	return 0;
}

/**
 * @brief Initialize Color System Related Registers to NTSC System
 *
 * @return 
 */
int CVD_Set_Initial_Color_System(void)
{

	g_CVD_Status_3CS.noisy_signal_detected = 0;
	g_CVD_Status_3CS.no_color_detected = 0;

	g_CVD_Status_3CS.cvd_color_system = CVD_STATE_VideoNotReady;
	g_CVD_Status_3CS.cvd_next_state = CVD_STATE_VideoNotReady;
	g_CVD_Status_3CS.cvd_prev_color_system = CVD_STATE_VideoNotReady;

	g_CVD_Status_3CS.in_atv_tunning = 0;

	if(g_CVD_Status_3CS.color_system_support == 0)	//for resume restore previous color system 
		g_CVD_Status_3CS.color_system_support =  LX_COLOR_SYSTEM_MULTI;

	g_CVD_Status_3CS.in_rf_mode = FALSE;
	g_CVD_Status_3CS.prev_av_rf_mode = TRUE;
	g_CVD_Status_3CS.port_change_detected = FALSE;
	g_CVD_Status_3CS.cvd_ntsc_only_mode = FALSE;
	g_CVD_Status_3CS.cvd_lock_stable_count = 0;
	g_CVD_Status_3CS.cvd_no_signal_count = 0;

	g_CVD_Status_3CS.cvd_black_level = LX_AFE_CVD_BLACK_LEVEL_AUTO;
	g_CVD_Status_3CS.vline_625_stable = 1;
	//Init Register to NTSC Mode
#ifdef USE_AFE_KDRV_FOR_H15
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 )) {
		//		CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_H15Ax_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H15A0[CVD_STATE_NTSC]);
		CVD_H15Ax_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_H15A0[CVD_STATE_PAL60]);
		CVD_H15Ax_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_H15A0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_H15Ax_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_H14Ax_Init_Global_Params();
	} else 
#endif
#ifdef USE_AFE_KDRV_FOR_H14
	if ( lx_chip_rev() >= LX_CHIP_REV( H14, A0 )) {
		//		CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_H14Ax_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H14A0[CVD_STATE_NTSC]);
		CVD_H14Ax_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_H14A0[CVD_STATE_PAL60]);
		CVD_H14Ax_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_H14A0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_H14Ax_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_H14Ax_Init_Global_Params();
	} else 
#endif
#ifdef USE_AFE_KDRV_FOR_M14
	if ( lx_chip_rev() >= LX_CHIP_REV( M14, B0 )) {
		//		CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_M14Bx_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_M14B0[CVD_STATE_NTSC]);
		CVD_M14Bx_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_M14B0[CVD_STATE_PAL60]);
		CVD_M14Bx_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_M14B0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_M14Bx_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_M14Bx_Init_Global_Params();
	}
	else if ( lx_chip_rev() >= LX_CHIP_REV( M14, A0 )) {
		//		CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_M14Ax_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_M14A0[CVD_STATE_NTSC]);
		CVD_M14Ax_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_M14A0[CVD_STATE_PAL60]);
		CVD_M14Ax_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_M14A0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_M14Ax_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_M14Ax_Init_Global_Params();
	} else 
#endif
#ifdef USE_AFE_KDRV_FOR_H13
	if ( lx_chip_rev() >= LX_CHIP_REV( H13, B0 )) {
		//		CVD_H13Bx_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_H13Bx_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H13B0[CVD_STATE_NTSC]);
		CVD_H13Bx_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_H13B0[CVD_STATE_PAL60]);
		CVD_H13Bx_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_H13B0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_H13Bx_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_H13Bx_Init_Global_Params();
	}

	else if ( lx_chip_rev() >= LX_CHIP_REV( H13, A0 )) {
		//		CVD_H13Ax_Program_Color_System_buffer_only(&g_SetColorSystem_3CS[CVD_STATE_NTSC]);
		CVD_H13Ax_Program_Color_System_Main_Multi(&g_SetColorSystem_3CS_H13A0[CVD_STATE_NTSC]);
		CVD_H13Ax_Program_Color_System_CS(CVD_SEL_CS_CS0, &g_SetColorSystem_3CS_H13A0[CVD_STATE_PAL60]);
		CVD_H13Ax_Program_Color_System_CS(CVD_SEL_CS_CS1, &g_SetColorSystem_3CS_H13A0[CVD_STATE_PALm]);

		//added 141003 ATV white noise after DC on
		CVD_H13Ax_Set_PQ_Mode(LX_CVD_PQ_NTSC_M_RF);
	//	CVD_H13Ax_Init_Global_Params();
	}
	else
#endif 
	{	// For L9B0

	}

	AFE_PRINT("CVD Init Color System Ended\n");

	return 0;

}

/**
 * @brief Set CVD Black Level
 *
 * @return 
 */
int CVD_Set_Black_Level(LX_AFE_CVD_BLACK_LEVEL_T cvd_black_level)
{
	
	g_CVD_Status_3CS.cvd_black_level = cvd_black_level;

	return 0;

}

int CVD_Set_Use_Internal_Demod(BOOLEAN bInternal_Mode)
{
	CVD_DEBUG("%s entered :  %d \n",__func__, bInternal_Mode);

#ifdef USE_AFE_KDRV_FOR_M14
	if(!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_M14))
	{
		if(bInternal_Mode)
		{
			g_CVD_Status_3CS.use_internal_demod = 1;
		}
		else
		{
			g_CVD_Status_3CS.use_internal_demod = 0;
		}
	}
#endif

	return 0;
}

static int CVD_Set_ycsep_Blend(int blend)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_ycsep_Blend(blend);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_ycsep_Blend(blend);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_ycsep_Blend(blend);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Bx_Set_ycsep_Blend(blend);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_ycsep_Blend(blend);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_ycsep_Blend(blend);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

static int CVD_Set_clampagc_updn(int updn_value)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_clampagc_updn(updn_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_clampagc_updn(updn_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_clampagc_updn(updn_value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_clampagc_updn(updn_value);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_clampagc_updn(updn_value);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_clampagc_updn(updn_value);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

static int CVD_Set_dc_clamp_mode(int mode)
{
	int ret = RET_OK;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		CVD_H15Ax_Set_dc_clamp_mode(mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CVD_H14Ax_Set_dc_clamp_mode(mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CVD_M14Bx_Set_dc_clamp_mode(mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CVD_M14Ax_Set_dc_clamp_mode(mode);
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		CVD_H13Bx_Set_dc_clamp_mode(mode);
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CVD_H13Ax_Set_dc_clamp_mode(mode);
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_HNon_Standard_Flag(void)
{
	int ret;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_HNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_HNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_HNon_Standard_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_HNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_HNon_Standard_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_HNon_Standard_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

int CVD_Get_VNon_Standard_Flag(void)
{
	int ret;

	if(gCVDInitStatus == FALSE)
		return RET_ERROR;

#ifdef USE_AFE_KDRV_FOR_H15
	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		ret = CVD_H15Ax_Get_VNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H14
	if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		ret = CVD_H14Ax_Get_VNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_M14
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		ret = CVD_M14Bx_Get_VNon_Standard_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		ret = CVD_M14Ax_Get_VNon_Standard_Flag();
	} else
#endif

#ifdef USE_AFE_KDRV_FOR_H13
	if (lx_chip_rev() >= LX_CHIP_REV(H13, B0))
	{
		ret = CVD_H13Bx_Get_VNon_Standard_Flag();
	} else
	if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		ret = CVD_H13Ax_Get_VNon_Standard_Flag();
	} else
#endif
		ret = RET_ERROR;	// Unkown chip revision
	
	return ret;
}

