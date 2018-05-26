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


/*! \file ADC_module.c
 * \brief ADC module control Drivers linux module
 */


/**
 *	Necessary includes for device drivers
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>	/** irqreturn_t */
#include <linux/irq.h>
#include <linux/delay.h>	/** for mdelay	*/
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/version.h>

#include "os_util.h"
#include "afe_drv.h"

#include "afe_kapi.h"

#include "adc_module.h"

//#include "vport_reg_B0.h"
//#include "vport_reg.h"

#include "h13/afe/adc_hw_h13a0.h"
#include "h14/afe/adc_hw_h14a0.h"
#include "m14/afe/adc_hw_m14a0.h"
#include "h15/afe/adc_hw_h15a0.h"


/******************************************************************************
 *				DEFINES
 *****************************************************************************/
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

#define	ADC_TIMEOUT 1000 //1 sec
//#define ADC_THREAD_TIMEOUT	10
#define ADC_THREAD_TIMEOUT	20

#define H13_SYNC_LEVEL_100	0xC8	//0xB2
#define H13_SYNC_LEVEL_108	0xC4	//0xAE

#define H14_SYNC_LEVEL_100	0xC8	//0xB2
#define H14_SYNC_LEVEL_108	0xC4	//0xAE

#define M14_SYNC_LEVEL_100	0xC8	//0xB2
#define M14_SYNC_LEVEL_108	0xC4	//0xAE

#define H15_SYNC_LEVEL_100	0xD6	//0xDA	//0xD6	//0xC8	//0xB2
#define H15_SYNC_LEVEL_108	0xCD	//0xD2	//0xCC	//0xC4	//0xAE

//#define IRQ_PRINT	1
/**
 *	Global variables of the driver
 */

//spinlock_t adc_lock ;
struct semaphore adc_lock;

//DECLARE_MUTEX(adc_lock);

ST_WORK_QUEUE	*gWorkqueue;


int gResolution;

int gResolutionSearching = 0;
//extern volatile VPORT_RegAx_T *gVportRegAx;
//extern volatile VPORT_RegBx_T *gVportRegBx;
//extern			 VPORT_RegBx_T *gpRegVPORT;		// for shadow register

extern int _g_ADC_Unstable ;

LX_AFE_ADC_SET_INPUT_T gPortStatus[3];

LX_AFE_PCMODE_MODETABLE_T *gPCTable;

unsigned int gPCTableNum = 0;

extern LX_AFE_ADC_INPUT_SOURCE_TYPE_T	gADC_InputSrc; //added by dws
extern int gPrevious_index;
extern int gADC_IN_IFM;
#if 0
static long kernel_pid = -1;
DECLARE_WAIT_QUEUE_HEAD(wq_kernel_test);
static int wq_condition = 0;

static struct timer_list *timer;
#endif
unsigned int	g_comp_sync_level_108, g_comp_sync_level_100;

struct task_struct	*stADC_Thread;

OS_SEM_T	g_ADC_Sema;

ADC_SYNC_MODE g_ADC_Sync_Mode = ADC_SYNC_MODE_UNKNOWN;	//Initial ADC Sync Mode Unknown..

LX_AFE_ADC_INPUT_SOURCE_TYPE_T gADC_type;

int (*ADC_RegInit)(void);
int (*ADC_GetBoundaryInform)(ST_EXTAU *inform);
int (*ADC_SetTAUInform)(S_EXTAU_VALUE *value);
int (*ADC_SetRGBPosition)(S_RGB_POSITION *pos);
int (*ADC_GetRGBResult)(S_RGB_OUT *result);
int (*ADC_GetRGBPosition)(S_RGB_POSITION *pos);
int (*ADC_SetRGBSize)(S_RGB_SIZE *size);
int (*ADC_GetSizeValue)(S_VIDEO_SIZE_INFORM *size);
int (*ADC_SetPort)(S_SEL_PORT *sel_port);
unsigned int (*ADC_GetPhaseValue)(void);
int (*ADC_SetPhaseValue)(unsigned int phase);
unsigned int (*ADC_GetScanType)(void);
int (*ADC_SetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int (*ADC_GetGainValue)(LX_AFE_ADC_GAIN_VALUE_T *stGain);
int (*ADC_SetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
int (*ADC_GetOffsetValue)(LX_AFE_ADC_OFFSET_VALUE_T *stOffset);
int (*ADC_SetInputVideo)(int iIndex);
int (*ADC_Enable_Scart_Mode)(BOOLEAN scart_enable);
void (*ADC_Vport_ALL_Power_Down)(void);
int (*ADC_Channel_Power_Control)(int vref_pdb, int bpdb, int gpdb, int rpdb);
int (*ADC_SetType)(LX_AFE_ADC_INPUT_SOURCE_TYPE_T sel_type);
int (*ADC_Set_LLPLL)(UINT32 arg);
int (*ADC_SetClock)(int iIndex, SINT16 value);
int (*ADC_AGOCInit)(void);
int (*ADC_GetSelectedPort)(void);
int (*ADC_SetGainRGB)( ST_GAIN_INFORM *gain);
int (*ADC_GetGainRGB)( ST_GAIN_INFORM *gain);
int (*ADC_SetIRELevel)(E_AGC_IRE ire , S_GAIN_TARGET *gain);
int (*ADC_GetOffsetRGB)( S_OFFSET_INFORM *offset);
int (*ADC_SetOffsetRGB)( S_OFFSET_INFORM *offset);
int (*ADC_GetSumRGB)(S_RGB_SUM *sum);
int (*ADC_SetAgocWakeup)(void);
int (*ADC_Reset_Digital)(void);
int (*ADC_Reset_Digital_24MHZ)(void);
int (*ADC_Reset_LLPLL)(void);
int (*ADC_Check_Sync_Exist)(void);
int (*ADC_Sync_Polarity_Override)(int set);
int (*ADC_Enable_Detection_Interrupt)(int enable);
int (*ADC_Enable_AOGC_Interrupt)(int enable);
int (*ADC_Mute_Enable)(int enable);
int (*ADC_Power_OnOff)(void);
int (*ADC_Ext_Clamp)(int sel_ext);
int (*ADC_Set_Clamp)(UINT8 clamp_base, UINT8 clamp_place, UINT8 clamp_width);
int (*ADC_GetPCModeInfo)(LX_AFE_PCMODE_INFO_T *stPCModeInfo);
int (*ADC_Get_LLPLL_Filter_Status)(void);
int (*ADC_SetIRETestMode)(int sel , E_AGC_IRE mode);
int (*ADC_Set_Coast)(UINT8 precoast, UINT8 postcoast);
int (*ADC_Set_Hsync_Sel)(UINT8 select);
int (*ADC_CVI_Sync_Exist)(void);
int (*ADC_Set_SOG_Level)(UINT32 level);
int (*ADC_Get_SOG_Level)(UINT32 *level);
int (*ADC_Check_Htotal_Diff)(UINT8 count, int Kadp_Index);
int (*ADC_Read_Pixel_Value)(LX_AFE_ADC_PIXEL_VALUE_T *pixel_value_t);
int (*ADC_Green_Mid_Level_Select)(UINT8 value);
int (*ADC_Read_VSync_Width)(UINT32 *pvs_width);
int (*ADC_Read_HSync_Width)(UINT32 *pvs_width);
void (*ADC_Set_Blank)(int blank_sp); // added by won.hur(won.hur@lge.com) @2011.08.17
void (*ADC_LVDS_Control)(int enable); // added by won.hur(won.hur@lge.com) @2011.08.17
int (*ADC_Component_APA)(int iIndex);
int (*ADC_Component_APA_Test)(UINT32 ADC_APA_Delay, UINT32 ADC_APA_Enable, UINT32 ADC_APA_Same_Count, UINT32 ADC_APA_Execute, UINT32 ADC_APA_Enable_Print);
int (*ADC_Get_Vtotal_Value)(void);
int (*ADC_Set_Sync_Mode)(ADC_SYNC_MODE sync_mode);
int (*ADC_LVDS_Channel_Enable_Control)(int enable);
int (*ADC_Mute_Control)(ADC_MUTE_CTRL adc_mute_control);
int (*ADC_Post_Gain_Control)(int percent);
int (*ADC_Set_Comp_Params)(int width, int progressive, int v_freq);
int (*ADC_Read_Sync_Low_Level)(void);
int (*ADC_Enable_Sync_Low_Level_Read)(int enable);
int (*ADC_Check_LLPLL_status)(void);
int (*ADC_LVDS_Src_Control)(LX_AFE_LVDS_SRC_CONTROL_T *pLVDS_Control_t);
int (*ADC_Read_ACE_Reg)(UINT32 addr , UINT32 *value);
int (*ADC_Write_ACE_Reg)(UINT32 addr , UINT32 value);
int (*ADC_Component_Set_CP_Params)(int kadp_index);
int (*ADC_Component_Get_CP_Data)(LX_AFE_ADC_COMP_VBI_CP_T *stComp_VBI_CP_Data);
int (*ADC_Flush_Component_CP_Data)(void);
int (*ADC_Get_Line_Count)(UINT32 *pLineCount);
int (*ADC_Read_ACE_GPIO)(UINT32 *pGPIO_Status);
int (*ADC_Set_ADC_Input_Filter_Sel)(int value);



DECLARE_WAIT_QUEUE_HEAD(WaitQueue_ADC);
static int		gForce_adc_thread_sleep = 1;
//static int		gADC_thread_running = 0;
int		g_Fast_IFM_mode = 0;
/******************************************************************************
 *				DATA STRUCTURES
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */


/******************************************************************************
 *				Local function
 *****************************************************************************/
/**
 *	Structure that declares the usual file
 *	access functions
 */

/**
 *
 * ADC_SetPCmodeTable
 *
 * @param	struct work_struct
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_SetPCmodeTable(LX_AFE_SET_PCMODE_TABLE_T *stTableInfo)
{

	if(( stTableInfo->Table_Size == 0) ||( stTableInfo->Table_Size > 4096) ) {
		AFE_ERROR("##### WRONG PCMode Table Size [%d] #####\n", stTableInfo->Table_Size);
		return -1;
	}

	gPCTable = (LX_AFE_PCMODE_MODETABLE_T *)kmalloc(stTableInfo->Table_Size, GFP_KERNEL);

	if(gPCTable != NULL)
	{
		if( copy_from_user(gPCTable, stTableInfo->pPCMode_Table, stTableInfo->Table_Size) )
		{
			AFE_PRINT("ERROR During copying PCModeTable !!!!! \n");
			return -EFAULT;
		}
		CVD_DEBUG("OK PCMode Table Copied\n");
		gPCTableNum = stTableInfo->Table_Count;
		CVD_DEBUG("PC Mode Table Size is [%d]\n",gPCTableNum);

		return 0;
	}
	else
	{

		return -1;
	}
}

static int ADC_Thread(void *data)
{
	while(1)
	{
		if(gForce_adc_thread_sleep > 0)
		{
			ADC_DEBUG("force sleep ADC Thread\n");
//			gForce_adc_thread_sleep = 0;
//			gADC_thread_running = 0;

			//added gettiming info zero when ADC module disconnected
			//ADC_Clear_Previous_Size_Info();		// reducing input change time
			//gPrevious_index = 0xffff;				// reducing input change time
			_g_ADC_Unstable = 1;
			//gResolution = -1;						// reducing input change time

			wait_event_interruptible(WaitQueue_ADC , gForce_adc_thread_sleep == 0);
			//interruptible_sleep_on(&WaitQueue_ADC);
//			gADC_thread_running = 1;

		}

		ADC_Periodic_Task();
		OS_MsecSleep(ADC_THREAD_TIMEOUT);

	}

	return 0;
}
/**
 * @brief
 *
 * @param enable
 *
 * @return
 */
int ADC_Enable_Periodic_Signal_Info_Read(UINT32 enable)
{
	ADC_DEBUG("%s with value [%d]\n", __func__, enable);

	if(enable == 3)	// Enable Fast IFM Mode
	{
		g_Fast_IFM_mode = 1;
		AFE_PRINT("ADC Fast IFM Mode Enabled !!!\n");
	}
	else if(enable == 2)	// Disable Fast IFM Mode
	{
		g_Fast_IFM_mode = 0;
		AFE_PRINT("ADC Fast IFM Mode Disabled !!!\n");
	}
	else if(enable > 0) {
			gForce_adc_thread_sleep = 0;
			wake_up_interruptible(&WaitQueue_ADC);
			CVD_DEBUG("Starting ADC Thread\n");
	}
	else {
		CVD_DEBUG("Stopping ADC Thread\n");
		gForce_adc_thread_sleep = 1;
	}
	return 0;
}

/**
 *
 * Input boundary detect
 *
 * @param	void
 * @return	0 : OK , -1 : NOK
 *
*/
void ADC_IBD(void *unused)
{


	ADC_IBD_main(gResolution);

	//return 0;
}

/**
 *
 * Auto adjust
 *
 * @param	void
 * @return	0 : OK , -1 : NOK
 *
*/
void ADC_AAD(void *unused)
{

	ADC_PHASE_main(gResolution);

	//return 0;
}

/**
 *
 * Gain/Offset calibration
 *
 * @param	void
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_GOC(LX_AFE_ADC_CALIB_INFO_T *calib_param)
{

	int ret = 0;
	//void *unused;
	//gResolution = -1;
	//unused = (void *)kmalloc(sizeof(4),GFP_KERNEL);
	ADC_Enable_AOGC_Interrupt(1);

	ADC_Reset_LLPLL();

	ret = ADC_AutoGainControl_main(calib_param, gResolution);

	//ADC_Reset_LLPLL();
	//ADC_Reset_Digital();
	ADC_Enable_AOGC_Interrupt(0);

	gPrevious_index = 0xffff;

//	ADC_IFM_ISR((void *)&unused);
	ADC_Clear_Previous_Size_Info();

	return ret;

}


/**
 *
 * Auto Phase for component
 *
 * @param	void
 * @return	0 : OK , -1 : NOK
 *
*/
void ADC_APC(void *unused)
{
	ADC_PHASE_for_Component(gResolution);
}
/**
 *
 * Work queue initialize
 *
 * @param	void
 * @return	0 : OK , -1 : NOK
 *
*/
int ADC_Init_Workqueue(void)
{

	gWorkqueue = (ST_WORK_QUEUE *)kmalloc(sizeof(ST_WORK_QUEUE), GFP_KERNEL);

	gWorkqueue->WorkQueue = create_workqueue("ADC_WORK");

	if(gWorkqueue->WorkQueue == NULL)
	{

		printk("qorkqueue allocation failed\n");
		return -1;
	}

//	INIT_WORK(&gWorkqueue->ifm , (void *)&ADC_IFM);
//	INIT_WORK(&gWorkqueue->ibd , (void *)&ADC_IBD);
	INIT_WORK(&gWorkqueue->aad , (void *)&ADC_AAD);
	INIT_WORK(&gWorkqueue->goc , (void *)&ADC_GOC);
	INIT_WORK(&gWorkqueue->apc , (void *)&ADC_APC);

//	queue_work(&gWorkQueue->WorkQueue , &gWorkQueue->ifm);
//	spin_lock_init(&adc_lock);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
		init_MUTEX(&adc_lock);
#else
		sema_init((&adc_lock),1);
#endif


	return 0;
}

int ADC_Exit_Workqueue(void)
{

	destroy_workqueue(gWorkqueue->WorkQueue);

	return 0;
}



/**
 *
 * VPORT_ADC_irq_handler irq handler
 *
 * @param	irq , device id , regs
 * @return	0 : OK , -1 : NOK
 *
*/
irqreturn_t VPORT_ADC_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	//int unused;
#if 0
	int adc_irq_handled = 0;

	{

		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_soy1detU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("soy1detU\n");
	#endif
			if(gVportRegBx->ch3_cst_006.cst_ro_soy1det == 0)		// disappear comp 2 signal
			{
				gPortStatus[1].adc_input_port = 0;
				gPortStatus[1].adc_input_source_type = 0;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = 1<<0;
		}

		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_soy1detL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("soy1detL\n");
	#endif

			if((adc_irq_handled == 0) && (gVportRegBx->ch3_adc_004.afe3ch_selmux == 0x1))
			{
				//ADC_IFM_ISR((void *)&unused);
				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}

			if(gVportRegBx->ch3_cst_006.cst_ro_soy1det == 1)		// appear comp 2 signal
			{
				gPortStatus[1].adc_input_port = LX_ADC_YPbPr_IN2;
				gPortStatus[1].adc_input_source_type = LX_ADC_INPUT_SOURCE_YPBPR;
			}



			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<1);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_soy0detU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("soy0detU\n");
	#endif

			if(gVportRegBx->ch3_cst_006.cst_ro_soy0det == 0)		// disappear comp 1 signal
			{
				gPortStatus[0].adc_input_port = 0;
				gPortStatus[0].adc_input_source_type = 0;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<2);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_soy0detL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("soy0detL\n");
	#endif

			if((adc_irq_handled == 0) && (gVportRegBx->ch3_adc_004.afe3ch_selmux == 0x0))
			{
				//ADC_IFM_ISR((void *)&unused);
				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}

			if(gVportRegBx->ch3_cst_006.cst_ro_soy0det == 1)		// appear comp 1 signal
			{
				gPortStatus[0].adc_input_port = LX_ADC_YPbPr_IN1;
				gPortStatus[0].adc_input_source_type = LX_ADC_INPUT_SOURCE_YPBPR;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<3);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_sogdetU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("sogdetU\n");
	#endif

			if(gVportRegBx->ch3_cst_006.cst_ro_sogdet == 0)		// disappear RGB signal
			{
				gPortStatus[2].adc_input_port = 0;
				gPortStatus[2].adc_input_source_type = 0;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<4);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_sogdetL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("sogdetL\n");
	#endif

				// PC Screen Saver Interrupt Issue
				/*
			if((adc_irq_handled == 0) && ((gVportRegBx->ch3_adc_004.afe3ch_selmux & 0x2) == 0x2))
			{
				// PC Screen Saver Interrupt Issue
				ADC_IFM((void *)&unused);
				adc_irq_handled = 1;
			}
			*/

			if(gVportRegBx->ch3_cst_006.cst_ro_sogdet == 1)		// appear RGB signal
			{
				gPortStatus[2].adc_input_port = LX_ADC_RGB_IN;
				gPortStatus[2].adc_input_source_type = LX_ADC_INPUT_SOURCE_RGB_PC;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<5);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_vsdetU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("vsdetU\n");
	#endif

			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<6);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_vsdetL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("vsdetL\n");
	#endif

			if((adc_irq_handled == 0) && ((gVportRegBx->ch3_adc_004.afe3ch_selmux & 0x2) == 0x2))
			{
				//ADC_IFM_ISR((void *)&unused);
				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}


			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<7);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_hsdetU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("hsdetU\n");
	#endif

			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<8);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_cst_ro_hsdetL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("hsdetL\n");
	#endif

			if((adc_irq_handled == 0) && ((gVportRegBx->ch3_adc_004.afe3ch_selmux & 0x2) == 0x2))
			{
	//			ADC_IFM_ISR((void *)&unused);

				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}


			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<9);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_resolution_change)		//
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("res_chg\n");
	#endif

			if(adc_irq_handled == 0)
			{
			//	ADC_IFM_ISR((void *)&unused);
				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}

	//		ADC_IFM((void *)&unused);

			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<10);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_no_signalU)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("No_SigU\n");
	#endif
			if(adc_irq_handled == 0)
			{
			//	ADC_IFM_ISR((void *)&unused);
				queue_work(gWorkqueue->WorkQueue , &gWorkqueue->ifm);
				adc_irq_handled = 1;
			}
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<11);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_no_signalL)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("No_SigL\n");
	#endif
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<12);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_dpms_state_change)
		{
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<13);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_aogc_done)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("AOGC DONE\n");
	#endif
			ADC_SetAgocWakeup();
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<14);
		}
		if(gVportRegBx->intr_vp2_intr.adc3ch_aogc_err)
		{
	#ifdef IRQ_PRINT
			AFE_PRINT("AOGC ERROR\n");
	#endif
			ADC_SetAgocWakeup();
			gVportRegBx->intr_vp2_clear.intr_vp2_clear = (1<<15);
		}
	}
#endif


	return IRQ_HANDLED;
}

#if 0
void ADC_KernelRun(void)
{
	wq_condition = 1;
	wake_up_interruptible(&wq_kernel_test);

}

void ADC_KernelThreadTest(void)
{

	printk("start kernel thread\n");

	while(1)
	{
	//	wait_event_interruptible(wq_kernel_test , wq_condition);

		interruptible_sleep_on(&wq_kernel_test);
		wq_condition = 0;
		printk("kernel run one time\n");
	}

}


void ADC_RunTimer(unsigned long arg)
{

	printk("run timer \n");

	timer->expires += msecs_to_jiffies(1000);
	add_timer(timer);


}
#endif

/**
* ADC Module initialize
*
* @parm void
* @return int
*/
int ADC_Initialize(void)
{
	int ret;

	gResolution = -1;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H15, A0))
	{
		g_comp_sync_level_108 = H15_SYNC_LEVEL_108;
		g_comp_sync_level_100 = H15_SYNC_LEVEL_100;

		ADC_RegInit=         			ADC_H15Ax_RegInit;
		ADC_GetBoundaryInform=         ADC_H15Ax_GetBoundaryInform;
		ADC_SetTAUInform=              ADC_H15Ax_SetTAUInform;
		ADC_SetRGBPosition=            ADC_H15Ax_SetRGBPosition;
		ADC_GetRGBResult=              ADC_H15Ax_GetRGBResult;
		ADC_GetRGBPosition=            ADC_H15Ax_GetRGBPosition;
		ADC_SetRGBSize=                ADC_H15Ax_SetRGBSize;
		ADC_GetSizeValue=              ADC_H15Ax_GetSizeValue;
		ADC_SetPort=                   ADC_H15Ax_SetPort;
		ADC_GetPhaseValue=             ADC_H15Ax_GetPhaseValue;
		ADC_SetPhaseValue=             ADC_H15Ax_SetPhaseValue;
		ADC_GetScanType=               ADC_H15Ax_GetScanType;
		ADC_SetGainValue=              ADC_H15Ax_SetGainValue;
		ADC_GetGainValue=              ADC_H15Ax_GetGainValue;
		ADC_SetOffsetValue=            ADC_H15Ax_SetOffsetValue;
		ADC_GetOffsetValue=            ADC_H15Ax_GetOffsetValue;
		ADC_SetInputVideo=             ADC_H15Ax_SetInputVideo;
		ADC_Enable_Scart_Mode=         ADC_H15Ax_Enable_Scart_Mode;
		ADC_Vport_ALL_Power_Down=      ADC_H15Ax_Vport_ALL_Power_Down;
		ADC_Channel_Power_Control=		 ADC_H15Ax_Channel_Power_Control;
		ADC_SetType=                   ADC_H15Ax_SetType;
		ADC_Set_LLPLL=                 ADC_H15Ax_Set_LLPLL;
		ADC_SetClock=                  ADC_H15Ax_SetClock;
		ADC_AGOCInit=                  ADC_H15Ax_AGOCInit;
		ADC_GetSelectedPort=           ADC_H15Ax_GetSelectedPort;
		ADC_SetGainRGB=                ADC_H15Ax_SetGainRGB;
		ADC_GetGainRGB=                ADC_H15Ax_GetGainRGB;
		ADC_SetIRELevel=               ADC_H15Ax_SetIRELevel;
		ADC_GetOffsetRGB=              ADC_H15Ax_GetOffsetRGB;
		ADC_SetOffsetRGB=              ADC_H15Ax_SetOffsetRGB;
		ADC_GetSumRGB=                 ADC_H15Ax_GetSumRGB;
		ADC_SetAgocWakeup=             ADC_H15Ax_SetAgocWakeup;
		ADC_Reset_Digital=             ADC_H15Ax_Reset_Digital;
		ADC_Reset_Digital_24MHZ=       ADC_H15Ax_Reset_Digital_24MHZ;
		ADC_Reset_LLPLL=               ADC_H15Ax_Reset_LLPLL;
		ADC_Check_Sync_Exist=          ADC_H15Ax_Check_Sync_Exist;
		ADC_Sync_Polarity_Override=    ADC_H15Ax_Sync_Polarity_Override;
		ADC_Enable_Detection_Interrupt=ADC_H15Ax_Enable_Detection_Interrupt;
		ADC_Enable_AOGC_Interrupt=		ADC_H15Ax_Enable_AOGC_Interrupt;
		ADC_Mute_Enable=               ADC_H15Ax_Mute_Enable;
		ADC_Power_OnOff=               ADC_H15Ax_Power_OnOff;
		ADC_Ext_Clamp=                 ADC_H15Ax_Ext_Clamp;
		ADC_Set_Clamp=                 ADC_H15Ax_Set_Clamp;
		ADC_GetPCModeInfo=             ADC_H15Ax_GetPCModeInfo;
		ADC_Get_LLPLL_Filter_Status=   ADC_H15Ax_Get_LLPLL_Filter_Status;
		ADC_Set_Coast	=			 ADC_H15Ax_Set_Coast;
		ADC_Set_Hsync_Sel		= 	ADC_H15Ax_Set_Hsync_Sel;
		ADC_CVI_Sync_Exist		= ADC_H15Ax_CVI_Sync_Exist;
		ADC_Set_SOG_Level = ADC_H15Ax_Set_SOG_Level;
		ADC_Get_SOG_Level = ADC_H15Ax_Get_SOG_Level;
		ADC_Check_Htotal_Diff	= ADC_H15Ax_Check_Htotal_Diff;
		ADC_SetIRETestMode=		ADC_H15Ax_SetIRETestMode;
		ADC_Read_Pixel_Value	=	ADC_H15Ax_Read_Pixel_Value;
		ADC_Green_Mid_Level_Select	=	ADC_H15Ax_Green_Mid_Level_Select;
		ADC_Read_VSync_Width	=	ADC_H15Ax_Read_VSync_Width;
		ADC_Read_HSync_Width	=	ADC_H15Ax_Read_HSync_Width;
		ADC_Set_Blank=				ADC_H15Ax_Set_Blank;
		ADC_LVDS_Control=			ADC_H15Ax_LVDS_Control;
		ADC_Component_APA	=		ADC_H15Ax_Component_APA;
		ADC_Component_APA_Test	=	ADC_H15Ax_Component_APA_Test;
		ADC_Get_Vtotal_Value	= 	ADC_H15Ax_Get_Vtotal_Value;
		ADC_Set_Sync_Mode	= 	ADC_H15Ax_Set_Sync_Mode;
		ADC_Mute_Control	=	ADC_H15Ax_Mute_Control;
		ADC_LVDS_Channel_Enable_Control	=	ADC_H15Ax_LVDS_Channel_Enable_Control;
		ADC_Post_Gain_Control	=	ADC_H15Ax_Post_Gain_Control;
		ADC_Set_Comp_Params	=	ADC_H15Ax_Set_Comp_Params;
		ADC_Read_Sync_Low_Level	=	ADC_H15Ax_Read_Sync_Low_Level;
		ADC_Enable_Sync_Low_Level_Read	=	ADC_H15Ax_Enable_Sync_Low_Level_Read;
		ADC_Check_LLPLL_status	=	ADC_H15Ax_Check_LLPLL_status;
		ADC_LVDS_Src_Control	=	ADC_H15Ax_LVDS_Src_Control;
		ADC_Read_ACE_Reg	=	ADC_H15Ax_Read_ACE_Reg;
		ADC_Write_ACE_Reg	=	ADC_H15Ax_Write_ACE_Reg;
		ADC_Component_Set_CP_Params	=		ADC_H15Ax_Component_Set_CP_Params;
		ADC_Component_Get_CP_Data	=		ADC_H15Ax_Component_Get_CP_Data;
		ADC_Flush_Component_CP_Data	=		ADC_H15Ax_Flush_Component_CP_Data;
		ADC_Get_Line_Count	=		ADC_H15Ax_Get_Line_Count;
		ADC_Read_ACE_GPIO	=		ADC_H15Ax_Read_ACE_GPIO;
		ADC_Set_ADC_Input_Filter_Sel =  ADC_H15Ax_Set_ADC_Input_Filter_Sel;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		g_comp_sync_level_108 = H14_SYNC_LEVEL_108;
		g_comp_sync_level_100 = H14_SYNC_LEVEL_100;

		ADC_RegInit=         			ADC_H14Ax_RegInit;
		ADC_GetBoundaryInform=         ADC_H14Ax_GetBoundaryInform;
		ADC_SetTAUInform=              ADC_H14Ax_SetTAUInform;
		ADC_SetRGBPosition=            ADC_H14Ax_SetRGBPosition;
		ADC_GetRGBResult=              ADC_H14Ax_GetRGBResult;
		ADC_GetRGBPosition=            ADC_H14Ax_GetRGBPosition;
		ADC_SetRGBSize=                ADC_H14Ax_SetRGBSize;
		ADC_GetSizeValue=              ADC_H14Ax_GetSizeValue;
		ADC_SetPort=                   ADC_H14Ax_SetPort;
		ADC_GetPhaseValue=             ADC_H14Ax_GetPhaseValue;
		ADC_SetPhaseValue=             ADC_H14Ax_SetPhaseValue;
		ADC_GetScanType=               ADC_H14Ax_GetScanType;
		ADC_SetGainValue=              ADC_H14Ax_SetGainValue;
		ADC_GetGainValue=              ADC_H14Ax_GetGainValue;
		ADC_SetOffsetValue=            ADC_H14Ax_SetOffsetValue;
		ADC_GetOffsetValue=            ADC_H14Ax_GetOffsetValue;
		ADC_SetInputVideo=             ADC_H14Ax_SetInputVideo;
		ADC_Enable_Scart_Mode=         ADC_H14Ax_Enable_Scart_Mode;
		ADC_Vport_ALL_Power_Down=      ADC_H14Ax_Vport_ALL_Power_Down;
		ADC_Channel_Power_Control=		 ADC_H14Ax_Channel_Power_Control;
		ADC_SetType=                   ADC_H14Ax_SetType;
		ADC_Set_LLPLL=                 ADC_H14Ax_Set_LLPLL;
		ADC_SetClock=                  ADC_H14Ax_SetClock;
		ADC_AGOCInit=                  ADC_H14Ax_AGOCInit;
		ADC_GetSelectedPort=           ADC_H14Ax_GetSelectedPort;
		ADC_SetGainRGB=                ADC_H14Ax_SetGainRGB;
		ADC_GetGainRGB=                ADC_H14Ax_GetGainRGB;
		ADC_SetIRELevel=               ADC_H14Ax_SetIRELevel;
		ADC_GetOffsetRGB=              ADC_H14Ax_GetOffsetRGB;
		ADC_SetOffsetRGB=              ADC_H14Ax_SetOffsetRGB;
		ADC_GetSumRGB=                 ADC_H14Ax_GetSumRGB;
		ADC_SetAgocWakeup=             ADC_H14Ax_SetAgocWakeup;
		ADC_Reset_Digital=             ADC_H14Ax_Reset_Digital;
		ADC_Reset_Digital_24MHZ=       ADC_H14Ax_Reset_Digital_24MHZ;
		ADC_Reset_LLPLL=               ADC_H14Ax_Reset_LLPLL;
		ADC_Check_Sync_Exist=          ADC_H14Ax_Check_Sync_Exist;
		ADC_Sync_Polarity_Override=    ADC_H14Ax_Sync_Polarity_Override;
		ADC_Enable_Detection_Interrupt=ADC_H14Ax_Enable_Detection_Interrupt;
		ADC_Enable_AOGC_Interrupt=		ADC_H14Ax_Enable_AOGC_Interrupt;
		ADC_Mute_Enable=               ADC_H14Ax_Mute_Enable;
		ADC_Power_OnOff=               ADC_H14Ax_Power_OnOff;
		ADC_Ext_Clamp=                 ADC_H14Ax_Ext_Clamp;
		ADC_Set_Clamp=                 ADC_H14Ax_Set_Clamp;
		ADC_GetPCModeInfo=             ADC_H14Ax_GetPCModeInfo;
		ADC_Get_LLPLL_Filter_Status=   ADC_H14Ax_Get_LLPLL_Filter_Status;
		ADC_Set_Coast	=			 ADC_H14Ax_Set_Coast;
		ADC_Set_Hsync_Sel		= 	ADC_H14Ax_Set_Hsync_Sel;
		ADC_CVI_Sync_Exist		= ADC_H14Ax_CVI_Sync_Exist;
		ADC_Set_SOG_Level = ADC_H14Ax_Set_SOG_Level;
		ADC_Get_SOG_Level = ADC_H14Ax_Get_SOG_Level;
		ADC_Check_Htotal_Diff	= ADC_H14Ax_Check_Htotal_Diff;
		ADC_SetIRETestMode=		ADC_H14Ax_SetIRETestMode;
		ADC_Read_Pixel_Value	=	ADC_H14Ax_Read_Pixel_Value;
		ADC_Green_Mid_Level_Select	=	ADC_H14Ax_Green_Mid_Level_Select;
		ADC_Read_VSync_Width	=	ADC_H14Ax_Read_VSync_Width;
		ADC_Read_HSync_Width	=	ADC_H14Ax_Read_HSync_Width;
		ADC_Set_Blank=				ADC_H14Ax_Set_Blank;
		ADC_LVDS_Control=			ADC_H14Ax_LVDS_Control;
		ADC_Component_APA	=		ADC_H14Ax_Component_APA;
		ADC_Component_APA_Test	=	ADC_H14Ax_Component_APA_Test;
		ADC_Get_Vtotal_Value	= 	ADC_H14Ax_Get_Vtotal_Value;
		ADC_Set_Sync_Mode	= 	ADC_H14Ax_Set_Sync_Mode;
		ADC_Mute_Control	=	ADC_H14Ax_Mute_Control;
		ADC_LVDS_Channel_Enable_Control	=	ADC_H14Ax_LVDS_Channel_Enable_Control;
		ADC_Post_Gain_Control	=	ADC_H14Ax_Post_Gain_Control;
		ADC_Set_Comp_Params	=	ADC_H14Ax_Set_Comp_Params;
		ADC_Read_Sync_Low_Level	=	ADC_H14Ax_Read_Sync_Low_Level;
		ADC_Enable_Sync_Low_Level_Read	=	ADC_H14Ax_Enable_Sync_Low_Level_Read;
		ADC_Check_LLPLL_status	=	ADC_H14Ax_Check_LLPLL_status;
		ADC_LVDS_Src_Control	=	ADC_H14Ax_LVDS_Src_Control;
		ADC_Read_ACE_Reg	=	ADC_H14Ax_Read_ACE_Reg;
		ADC_Write_ACE_Reg	=	ADC_H14Ax_Write_ACE_Reg;
		ADC_Component_Set_CP_Params	=		ADC_H14Ax_Component_Set_CP_Params;
		ADC_Component_Get_CP_Data	=		ADC_H14Ax_Component_Get_CP_Data;
		ADC_Flush_Component_CP_Data	=		ADC_H14Ax_Flush_Component_CP_Data;
		ADC_Get_Line_Count	=		ADC_H14Ax_Get_Line_Count;
		ADC_Read_ACE_GPIO	=		ADC_H14Ax_Read_ACE_GPIO;
		ADC_Set_ADC_Input_Filter_Sel =  ADC_H14Ax_Set_ADC_Input_Filter_Sel;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		g_comp_sync_level_108 = M14_SYNC_LEVEL_108;
		g_comp_sync_level_100 = M14_SYNC_LEVEL_100;

		ADC_RegInit=         			ADC_M14Ax_RegInit;
		ADC_GetBoundaryInform=         ADC_M14Ax_GetBoundaryInform;
		ADC_SetTAUInform=              ADC_M14Ax_SetTAUInform;
		ADC_SetRGBPosition=            ADC_M14Ax_SetRGBPosition;
		ADC_GetRGBResult=              ADC_M14Ax_GetRGBResult;
		ADC_GetRGBPosition=            ADC_M14Ax_GetRGBPosition;
		ADC_SetRGBSize=                ADC_M14Ax_SetRGBSize;
		ADC_GetSizeValue=              ADC_M14Ax_GetSizeValue;
		ADC_SetPort=                   ADC_M14Ax_SetPort;
		ADC_GetPhaseValue=             ADC_M14Ax_GetPhaseValue;
		ADC_SetPhaseValue=             ADC_M14Ax_SetPhaseValue;
		ADC_GetScanType=               ADC_M14Ax_GetScanType;
		ADC_SetGainValue=              ADC_M14Ax_SetGainValue;
		ADC_GetGainValue=              ADC_M14Ax_GetGainValue;
		ADC_SetOffsetValue=            ADC_M14Ax_SetOffsetValue;
		ADC_GetOffsetValue=            ADC_M14Ax_GetOffsetValue;
		ADC_SetInputVideo=             ADC_M14Ax_SetInputVideo;
		ADC_Enable_Scart_Mode=         ADC_M14Ax_Enable_Scart_Mode;
		ADC_Vport_ALL_Power_Down=      ADC_M14Ax_Vport_ALL_Power_Down;
		ADC_Channel_Power_Control=		 ADC_M14Ax_Channel_Power_Control;
		ADC_SetType=                   ADC_M14Ax_SetType;
		ADC_Set_LLPLL=                 ADC_M14Ax_Set_LLPLL;
		ADC_SetClock=                  ADC_M14Ax_SetClock;
		ADC_AGOCInit=                  ADC_M14Ax_AGOCInit;
		ADC_GetSelectedPort=           ADC_M14Ax_GetSelectedPort;
		ADC_SetGainRGB=                ADC_M14Ax_SetGainRGB;
		ADC_GetGainRGB=                ADC_M14Ax_GetGainRGB;
		ADC_SetIRELevel=               ADC_M14Ax_SetIRELevel;
		ADC_GetOffsetRGB=              ADC_M14Ax_GetOffsetRGB;
		ADC_SetOffsetRGB=              ADC_M14Ax_SetOffsetRGB;
		ADC_GetSumRGB=                 ADC_M14Ax_GetSumRGB;
		ADC_SetAgocWakeup=             ADC_M14Ax_SetAgocWakeup;
		ADC_Reset_Digital=             ADC_M14Ax_Reset_Digital;
		ADC_Reset_Digital_24MHZ=       ADC_M14Ax_Reset_Digital_24MHZ;
		ADC_Reset_LLPLL=               ADC_M14Ax_Reset_LLPLL;
		ADC_Check_Sync_Exist=          ADC_M14Ax_Check_Sync_Exist;
		ADC_Sync_Polarity_Override=    ADC_M14Ax_Sync_Polarity_Override;
		ADC_Enable_Detection_Interrupt=ADC_M14Ax_Enable_Detection_Interrupt;
		ADC_Enable_AOGC_Interrupt=		ADC_M14Ax_Enable_AOGC_Interrupt;
		ADC_Mute_Enable=               ADC_M14Ax_Mute_Enable;
		ADC_Power_OnOff=               ADC_M14Ax_Power_OnOff;
		ADC_Ext_Clamp=                 ADC_M14Ax_Ext_Clamp;
		ADC_Set_Clamp=                 ADC_M14Ax_Set_Clamp;
		ADC_GetPCModeInfo=             ADC_M14Ax_GetPCModeInfo;
		ADC_Get_LLPLL_Filter_Status=   ADC_M14Ax_Get_LLPLL_Filter_Status;
		ADC_Set_Coast	=			 ADC_M14Ax_Set_Coast;
		ADC_Set_Hsync_Sel		= 	ADC_M14Ax_Set_Hsync_Sel;
		ADC_CVI_Sync_Exist		= ADC_M14Ax_CVI_Sync_Exist;
		ADC_Set_SOG_Level = ADC_M14Ax_Set_SOG_Level;
		ADC_Get_SOG_Level = ADC_M14Ax_Get_SOG_Level;
		ADC_Check_Htotal_Diff	= ADC_M14Ax_Check_Htotal_Diff;
		ADC_SetIRETestMode=		ADC_M14Ax_SetIRETestMode;
		ADC_Read_Pixel_Value	=	ADC_M14Ax_Read_Pixel_Value;
		ADC_Green_Mid_Level_Select	=	ADC_M14Ax_Green_Mid_Level_Select;
		ADC_Read_VSync_Width	=	ADC_M14Ax_Read_VSync_Width;
		ADC_Read_HSync_Width	=	ADC_M14Ax_Read_HSync_Width;
		ADC_Set_Blank=				ADC_M14Ax_Set_Blank;
		ADC_LVDS_Control=			ADC_M14Ax_LVDS_Control;
		ADC_Component_APA	=		ADC_M14Ax_Component_APA;
		ADC_Component_APA_Test	=	ADC_M14Ax_Component_APA_Test;
		ADC_Get_Vtotal_Value	= 	ADC_M14Ax_Get_Vtotal_Value;
		ADC_Set_Sync_Mode	= 	ADC_M14Ax_Set_Sync_Mode;
		ADC_Mute_Control	=	ADC_M14Ax_Mute_Control;
		ADC_LVDS_Channel_Enable_Control	=	ADC_M14Ax_LVDS_Channel_Enable_Control;
		ADC_Post_Gain_Control	=	ADC_M14Ax_Post_Gain_Control;
		ADC_Set_Comp_Params	=	ADC_M14Ax_Set_Comp_Params;
		ADC_Read_Sync_Low_Level	=	ADC_M14Ax_Read_Sync_Low_Level;
		ADC_Enable_Sync_Low_Level_Read	=	ADC_M14Ax_Enable_Sync_Low_Level_Read;
		ADC_Check_LLPLL_status	=	ADC_M14Ax_Check_LLPLL_status;
		ADC_LVDS_Src_Control	=	ADC_M14Ax_LVDS_Src_Control;
		ADC_Read_ACE_Reg	=	ADC_M14Ax_Read_ACE_Reg;
		ADC_Write_ACE_Reg	=	ADC_M14Ax_Write_ACE_Reg;
		ADC_Component_Set_CP_Params	=		ADC_M14Ax_Component_Set_CP_Params;
		ADC_Component_Get_CP_Data	=		ADC_M14Ax_Component_Get_CP_Data;
		ADC_Flush_Component_CP_Data	=		ADC_M14Ax_Flush_Component_CP_Data;
		ADC_Get_Line_Count	=		ADC_M14Ax_Get_Line_Count;
		ADC_Read_ACE_GPIO	=		ADC_M14Ax_Read_ACE_GPIO;
		ADC_Set_ADC_Input_Filter_Sel =  ADC_M14Ax_Set_ADC_Input_Filter_Sel;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		g_comp_sync_level_108 = H13_SYNC_LEVEL_108;
		g_comp_sync_level_100 = H13_SYNC_LEVEL_100;

		ADC_RegInit=         			ADC_H13Ax_RegInit;
		ADC_GetBoundaryInform=         ADC_H13Ax_GetBoundaryInform;
		ADC_SetTAUInform=              ADC_H13Ax_SetTAUInform;
		ADC_SetRGBPosition=            ADC_H13Ax_SetRGBPosition;
		ADC_GetRGBResult=              ADC_H13Ax_GetRGBResult;
		ADC_GetRGBPosition=            ADC_H13Ax_GetRGBPosition;
		ADC_SetRGBSize=                ADC_H13Ax_SetRGBSize;
		ADC_GetSizeValue=              ADC_H13Ax_GetSizeValue;
		ADC_SetPort=                   ADC_H13Ax_SetPort;
		ADC_GetPhaseValue=             ADC_H13Ax_GetPhaseValue;
		ADC_SetPhaseValue=             ADC_H13Ax_SetPhaseValue;
		ADC_GetScanType=               ADC_H13Ax_GetScanType;
		ADC_SetGainValue=              ADC_H13Ax_SetGainValue;
		ADC_GetGainValue=              ADC_H13Ax_GetGainValue;
		ADC_SetOffsetValue=            ADC_H13Ax_SetOffsetValue;
		ADC_GetOffsetValue=            ADC_H13Ax_GetOffsetValue;
		ADC_SetInputVideo=             ADC_H13Ax_SetInputVideo;
		ADC_Enable_Scart_Mode=         ADC_H13Ax_Enable_Scart_Mode;
		ADC_Vport_ALL_Power_Down=      ADC_H13Ax_Vport_ALL_Power_Down;
		ADC_Channel_Power_Control=		 ADC_H13Ax_Channel_Power_Control;
		ADC_SetType=                   ADC_H13Ax_SetType;
		ADC_Set_LLPLL=                 ADC_H13Ax_Set_LLPLL;
		ADC_SetClock=                  ADC_H13Ax_SetClock;
		ADC_AGOCInit=                  ADC_H13Ax_AGOCInit;
		ADC_GetSelectedPort=           ADC_H13Ax_GetSelectedPort;
		ADC_SetGainRGB=                ADC_H13Ax_SetGainRGB;
		ADC_GetGainRGB=                ADC_H13Ax_GetGainRGB;
		ADC_SetIRELevel=               ADC_H13Ax_SetIRELevel;
		ADC_GetOffsetRGB=              ADC_H13Ax_GetOffsetRGB;
		ADC_SetOffsetRGB=              ADC_H13Ax_SetOffsetRGB;
		ADC_GetSumRGB=                 ADC_H13Ax_GetSumRGB;
		ADC_SetAgocWakeup=             ADC_H13Ax_SetAgocWakeup;
		ADC_Reset_Digital=             ADC_H13Ax_Reset_Digital;
		ADC_Reset_Digital_24MHZ=       ADC_H13Ax_Reset_Digital_24MHZ;
		ADC_Reset_LLPLL=               ADC_H13Ax_Reset_LLPLL;
		ADC_Check_Sync_Exist=          ADC_H13Ax_Check_Sync_Exist;
		ADC_Sync_Polarity_Override=    ADC_H13Ax_Sync_Polarity_Override;
		ADC_Enable_Detection_Interrupt=ADC_H13Ax_Enable_Detection_Interrupt;
		ADC_Enable_AOGC_Interrupt=		ADC_H13Ax_Enable_AOGC_Interrupt;
		ADC_Mute_Enable=               ADC_H13Ax_Mute_Enable;
		ADC_Power_OnOff=               ADC_H13Ax_Power_OnOff;
		ADC_Ext_Clamp=                 ADC_H13Ax_Ext_Clamp;
		ADC_Set_Clamp=                 ADC_H13Ax_Set_Clamp;
		ADC_GetPCModeInfo=             ADC_H13Ax_GetPCModeInfo;
		ADC_Get_LLPLL_Filter_Status=   ADC_H13Ax_Get_LLPLL_Filter_Status;
		ADC_Set_Coast	=			 ADC_H13Ax_Set_Coast;
		ADC_Set_Hsync_Sel		= 	ADC_H13Ax_Set_Hsync_Sel;
		ADC_CVI_Sync_Exist		= ADC_H13Ax_CVI_Sync_Exist;
		ADC_Set_SOG_Level = ADC_H13Ax_Set_SOG_Level;
		ADC_Get_SOG_Level = ADC_H13Ax_Get_SOG_Level;
		ADC_Check_Htotal_Diff	= ADC_H13Ax_Check_Htotal_Diff;
		ADC_SetIRETestMode=		ADC_H13Ax_SetIRETestMode;
		ADC_Read_Pixel_Value	=	ADC_H13Ax_Read_Pixel_Value;
		ADC_Green_Mid_Level_Select	=	ADC_H13Ax_Green_Mid_Level_Select;
		ADC_Read_VSync_Width	=	ADC_H13Ax_Read_VSync_Width;
		ADC_Read_HSync_Width	=	ADC_H13Ax_Read_HSync_Width;
		ADC_Set_Blank=				ADC_H13Ax_Set_Blank;
		ADC_LVDS_Control=			ADC_H13Ax_LVDS_Control;
		ADC_Component_APA	=		ADC_H13Ax_Component_APA;
		ADC_Component_APA_Test	=	ADC_H13Ax_Component_APA_Test;
		ADC_Get_Vtotal_Value	= 	ADC_H13Ax_Get_Vtotal_Value;
		ADC_Set_Sync_Mode	= 	ADC_H13Ax_Set_Sync_Mode;
		ADC_Mute_Control	=	ADC_H13Ax_Mute_Control;
		ADC_LVDS_Channel_Enable_Control	=	ADC_H13Ax_LVDS_Channel_Enable_Control;
		ADC_Post_Gain_Control	=	ADC_H13Ax_Post_Gain_Control;
		ADC_Set_Comp_Params	=	ADC_H13Ax_Set_Comp_Params;
		ADC_Read_Sync_Low_Level	=	ADC_H13Ax_Read_Sync_Low_Level;
		ADC_Enable_Sync_Low_Level_Read	=	ADC_H13Ax_Enable_Sync_Low_Level_Read;
		ADC_Check_LLPLL_status	=	ADC_H13Ax_Check_LLPLL_status;
		ADC_LVDS_Src_Control	=	ADC_H13Ax_LVDS_Src_Control;
		ADC_Read_ACE_Reg	=	ADC_H13Ax_Read_ACE_Reg;
		ADC_Write_ACE_Reg	=	ADC_H13Ax_Write_ACE_Reg;
		ADC_Component_Set_CP_Params	=		ADC_H13Ax_Component_Set_CP_Params;
		ADC_Component_Get_CP_Data	=		ADC_H13Ax_Component_Get_CP_Data;
		ADC_Flush_Component_CP_Data	=		ADC_H13Ax_Flush_Component_CP_Data;
		ADC_Get_Line_Count	=		ADC_H13Ax_Get_Line_Count;
		ADC_Read_ACE_GPIO	=		ADC_H13Ax_Read_ACE_GPIO;
		ADC_Set_ADC_Input_Filter_Sel =  ADC_H13Ax_Set_ADC_Input_Filter_Sel;
	}
#endif
	else
	{
		return -1;
	}

	//Initialize ADC semaphore
	OS_InitMutex(&g_ADC_Sema, OS_SEM_ATTR_DEFAULT);

	/*
	if(ADC_Init_Workqueue() < 0)
	{
		ret = -1;
	}
	else
	{
		ret = 0;
	}
	*/

	// present input source status
	memset(&gPortStatus , 0 , sizeof(LX_AFE_ADC_SET_INPUT_T)*3);

	gADC_type = LX_ADC_INPUT_SOURCE_NONE;

	ret = ADC_RegInit();

	gPCTable = NULL;

#ifdef ADC_PERIODIC_SIGNAL_INFO_READ
	//Disable Detection Interrupt
	ADC_Enable_Detection_Interrupt(0);
#else
	ADC_Enable_Detection_Interrupt(1);
#endif

	stADC_Thread = kthread_create( ADC_Thread, (void*)NULL, "adc_thread");

	if(stADC_Thread)
	{
		wake_up_process(stADC_Thread);
		//gCVD_thread_running = 1;
		AFE_PRINT("ADC Thread [%d]\n", stADC_Thread->pid);
	}


	else
	{
		AFE_ERROR("ADC Thread Already Created\n");
	}
	// for test
#if 0
	kernel_pid = kernel_thread((void *)&ADC_KernelThreadTest, (void*)NULL, CLONE_KERNEL);

	timer = (struct timer_list *)kmalloc(sizeof(struct timer_list), GFP_KERNEL);

	init_timer(timer);

	timer->expires = msecs_to_jiffies(1000) + jiffies;
	timer->function = ADC_RunTimer;
	timer->data = 0;

	add_timer(timer);

	printk("timer return is %d %d\n" , ret , (int)timer->expires);

	ret = 0;
#endif
	return ret;
}

#ifdef	KDRV_CONFIG_PM
int ADC_RunSuspend(void)
{
	int ret;

	LX_AFE_ADC_SET_INPUT_T stInput = {LX_ADC_INPUT_SOURCE_NONE, LX_ADC_IN_NUM};

	AFE_PRINT("ADC RunSuspend ... \n");

	ret = ADC_SetInputFormat(&stInput);

	return ret;
}

int ADC_RunResume(void)
{
	int ret = 0;

	//gResolution = -1;

	// present input source status
	//memset(&gPortStatus , 0 , sizeof(LX_AFE_ADC_SET_INPUT_T)*3);

	//gADC_type = LX_ADC_INPUT_SOURCE_NONE;

	ret = ADC_RegInit();

	return ret;
}
#endif
#if 0
int ADC_RunRegister(S_INTERFACE_REG *reg)
{
	if(reg->read_write)	// write
	{
		ADC_SetI2CData(reg->sub , reg->addr , reg->size , reg->value);

	}
	else			// read
	{
		ADC_GetI2CData(reg->sub , reg->addr , reg->size);

	}

	return 0;
}
#endif
