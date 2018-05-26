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
 *  driver interface header for demod device. ( used only within kdriver )
 *	demod device will teach you how to make device driver with new platform.
 *
 *  @author		jeongpil.yun (jeongpil.yun@lge.com)
 *  @version	1.0
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_demod
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */

#include <linux/delay.h>	/**< mdelay */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/kthread.h>

#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <linux/delay.h>	/**< mdelay */

#ifdef KDRV_CONFIG_PM	// added by SC Jung for quick booting
#include <linux/platform_device.h>
#endif
#include "os_util.h"
//#include "base_dev_cfg.h"
#include "base_device.h"

#include "ctop_regs.h"

#include "demod_drv.h"

#include "demod_impl.h"

#include "sys_regs.h"


#include "i2c_core.h"



/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
//#define L9_CTOP_DEMOD_I2C_ADDR		0x12
//#define L9_CTOP_DEMOD_I2C_RETRY		3


	/**
	 *	Global variables of the driver
	 */

#ifdef KDRV_CONFIG_PM
	unsigned int guiPmClk;
	unsigned int guiPmBbmode;
	unsigned int guiPmI2C;
#endif





/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

UINT64 pauseTimeDemod = 0xFFFFFFFFFFFFFFFFULL ;


static wait_queue_head_t 	g_DEMOD_Tuning_WaitQ;
LX_DEMOD_TASK_MSG_T	g_requestMsg;

unsigned int g_NewTuneReqeustTime;
unsigned int g_RetuneStartTime;

UINT32 notifyinglockTimeOut = LX_DEMOD_LOCK_DEALY_1500;

static DECLARE_WAIT_QUEUE_HEAD( g_DEMOD_Tuning_WaitQ );

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int 	DEMOD_UnLock_Notify(UINT8 deviceNum);
static void DEMOD_CheckDemodPayloads_and_Pause(UINT8 deviceNum, UINT64 timeout);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
DEMOD_HAL_T g_demod_hal;



/*========================================================================================
	Implementation Group
========================================================================================*/
#if 0
#endif

/***********************************************************************************
* Common function
************************************************************************************/


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_MediaChange.
 * Set stand operation mode
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @param *pTranMedia  [IN] LX_DEMOD_INIT_PARAM_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_MediaChange(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_INIT_PARAM_T *pTranMedia)
{
	UINT32 ifFrequency;
	UINT32 sampleRate;
	UINT32 pllClkSource;
	Handle_t	phDevice;

	if(NULL == pDemodInfo)
	{
		DEMOD_RETURN_FAIL(__F__);
	}
	if(NULL == pTranMedia)
	{
		DEMOD_RETURN_FAIL(__F__);
	}

//A	if((pDemodInfo->operMode == pTranMedia->operMode) && (pTranMedia->operMode == LX_DEMOD_ANALOG)) return RET_OK;

	DEMOD_LockDevice();
	pDemodInfo->operMode 	= pTranMedia->operMode;
	pDemodInfo->symbr	 	= pTranMedia->symbr;
	pDemodInfo->IFFrq		= pTranMedia->ifFrq;
	DEMOD_UnlockDevice();

	ifFrequency = pDemodInfo->IFFrq;
	phDevice = pDemodInfo->setCfgParam.analog.handleABB;

	/* to do ->  task stop */
	//Send event for Tuning task
	g_requestMsg = LX_DEMOD_STOP_REQEST;
	wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

	if(pDemodInfo->operMode == LX_DEMOD_UNKOWN_OPMODE)
	{

		if( RET_OK != DEMOD_ANALOG_Set_AbbMode(TRUE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		else
		{
			pDemodInfo->bIsAbbMode = TRUE;

			if(RET_OK != DEMOD_ANALOG_Power_Save(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}

		if( RET_OK != DEMOD_ANALOG_Set_AbbMode(FALSE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		else
		{
			pDemodInfo->bIsAbbMode = FALSE;

			if(RET_OK != DEMOD_Power_Save(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
		}

		DEMOD_PRINT("[DEMOD] Success (%s:%d) Oper mode = %s, All Demods are going to power save mode !!!\n", __F__, __L__,"Analog or unkown");

	}
	else if(pDemodInfo->operMode == LX_DEMOD_DIGITAL_Sleep)
	{
		if(TRUE == pDemodInfo->bIsAbbMode)
		{
			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(FALSE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			pDemodInfo->bIsAbbMode = FALSE;
		}
	
		if(RET_OK != DEMOD_Power_Save(TRUE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Power_Save() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if( RET_OK != DEMOD_ANALOG_Set_AbbMode(TRUE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		pDemodInfo->bIsAbbMode = TRUE;

		DEMOD_PRINT("[DEMOD] (%s:%d) MODE : ABB () !!!\n", __F__, __L__);
		
	}

	else if(pDemodInfo->operMode == LX_DEMOD_ANALOG_Sleep)
	{
		if(FALSE == pDemodInfo->bIsAbbMode)
		{
			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			pDemodInfo->bIsAbbMode = TRUE;
		}
		if(RET_OK != DEMOD_ANALOG_Power_Save(TRUE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Power_Save() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		if( RET_OK != DEMOD_ANALOG_Set_AbbMode(FALSE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_Set_AbbMode = TRUE() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		pDemodInfo->bIsAbbMode = FALSE;

			
	}	
	else if(pDemodInfo->operMode == LX_DEMOD_ANALOG)
	{

		pllClkSource = LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ; ///LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ;
		sampleRate = LX_ADEMOD_SAMPLING_RATE_270MHz;
		
		pDemodInfo->setCfgParam.analog.pllClkSource = pllClkSource;
		pDemodInfo->setCfgParam.analog.sampleRate = sampleRate;


		if(FALSE == pDemodInfo->bIsAbbMode)
		{
			if(RET_OK != DEMOD_Power_Save(TRUE)) //Digital power save on.
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			DEMOD_PRINT("[DEMOD] Success (%s:%d) Oper mode = %s, going to power save mode !!!\n", __F__, __L__,"Analog or unkown");

			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(TRUE))
			{
				DEMOD_PRINT("[DEMOD] (%s:%d) FAIL, pDemodInfo->bIsAbbMode = TRUE!!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			else
			{
				pDemodInfo->bIsAbbMode = TRUE;
				DEMOD_PRINT("[DEMOD] (%s:%d) MODE : ABB () !!!\n", __F__, __L__);

#if 0
				if( RET_OK != DEMOD_ANALOG_Set_CvbsDecCtrl(phDevice, LX_DEMOD_ANALOG_ATV))
				{
					DEMOD_PRINT("[DEMOD] (%s:%d) FAIL, DEMOD_ANALOG_Set_CvbsDecCtrl(phDevice, LX_DEMOD_ANALOG_ATV) !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
			}

		}

		if(RET_OK != DEMOD_ANALOG_Power_Save(FALSE)) //Analog power save off.
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Analog_Power_Save() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(RET_OK != DEMOD_ANALOG_Set_IF_Frq(phDevice,pllClkSource, ifFrequency))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_IF_Frq() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}


		if(RET_OK != DEMOD_ANALOG_SoftwareReset(phDevice))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_SoftwareReset() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		DEMOD_PRINT("[DEMOD] Success (%s:%d) Oper mode = %s, going to power active mode !!!\n", __F__, __L__,"Analog");

	}
	else
	{

		if(TRUE == pDemodInfo->bIsAbbMode)
		{
			if(RET_OK != DEMOD_ANALOG_Power_Save(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d)  FAIL,DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			DEMOD_PRINT("[DEMOD] Success (%s:%d) Oper mode = %s, Analog is going to power save mode !!!\n", __F__, __L__,"Digtal");

#if 0
			if(LX_DEMOD_PAL == pDemodInfo->setCfgParam.analog.rfMode)
			{
				if( RET_OK != DEMOD_ANALOG_Set_CvbsDecCtrl(phDevice, LX_DEMOD_ANALOG_DTV))
				{
					DEMOD_PRINT("[DEMOD] (%s:%d)  FAIL,DEMOD_ANALOG_Set_CvbsDecCtrl(phDevice, LX_DEMOD_ANALOG_DTV) !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
			}
#endif
			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(FALSE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) FAIL, DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			else
			{
				pDemodInfo->bIsAbbMode = FALSE;
				DEMOD_PRINT("[DEMOD] (%s:%d)  pDemodInfo->bIsAbbMode = FALSE!!!\n", __F__, __L__);
			}
		}

		if(RET_OK != DEMOD_Power_Save(FALSE))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d)  FAIL,DEMOD_Power_Save() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(RET_OK != DEMOD_Set_IF_Frq(ifFrequency))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) FAIL, DEMOD_Set_IF_Frq() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		DEMOD_PRINT("[DEMOD] Success (%s:%d) Oper mode = %s, going to power active mode !!!\n", __F__, __L__,"Digital");

	}


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ChannelChange.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @param *pDemodCfg  [IN] LX_DEMOD_CONFIG_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ChannelChange(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_CONFIG_T *pDemodCfg)
{

	LX_DEMOD_OPER_MODE_T 	operMode = LX_DEMOD_UNKOWN_OPMODE;
	LX_DEMOD_BWMODE_T		bandWd = LX_DEMOD_BW_UNKNOWN;
	LX_DEMOD_SYMRDET_MODE_T symbr = LX_DEMOD_SYMR_UNKOWN_MODE;
	LX_DEMOD_TUNE_MODE_T	tuneMode = LX_DEMOD_TUNE_UNKNOWN;

	if(NULL == pDemodInfo)
	{
		DEMOD_RETURN_FAIL(__F__);
	}
	if(NULL == pDemodCfg)
	{
		DEMOD_RETURN_FAIL(__F__);
	}

	/* task stop */
	//Send event for Tuning task
	DEMOD_LockDevice();

	g_requestMsg = LX_DEMOD_STOP_REQEST;
	wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

	DEMOD_UnlockDevice();
	DEMOD_PRINT("[DEMOD] DEMOD Task STOP !!!\n");


	if(pDemodInfo->operMode == pDemodCfg->operMode)
	{
		DEMOD_PRINT("[DEMOD] The Opermode is the same (0x%x) !!!\n",pDemodInfo->operMode);

	}
	else
	{
		DEMOD_PRINT("[DEMOD] Oops, The Opermode is different!!\n");
		DEMOD_PRINT("[DEMOD] pDemodInfo->operMode = 0x%x, pDemodCfg->operMode = 0x%x\n",pDemodInfo->operMode, pDemodCfg->operMode);
		pDemodInfo->operMode = pDemodCfg->operMode;

	}
	DEMOD_PRINT("[DEMOD] (%s:%d) pDemodInfo->bIsAbbMode = %s!!!\n", __F__, __L__,(TRUE == pDemodInfo->bIsAbbMode) ? "TRUE" : "FALSE" );

	if(LX_DEMOD_ANALOG == pDemodInfo->operMode)
	{

		OS_MsecSleep(20);
		if(FALSE == pDemodInfo->bIsAbbMode)
		{
			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(TRUE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			else
			{
				pDemodInfo->bIsAbbMode = TRUE;
				
				DEMOD_PRINT("[DEMOD] (%s:%d) pDemodInfo->bIsAbbMode = TRUE!!!\n", __F__, __L__);
			}
		}
	}
	else
	{
		if(TRUE == pDemodInfo->bIsAbbMode)
		{
			if( RET_OK != DEMOD_ANALOG_Set_AbbMode(FALSE))
			{
				DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Power_Save() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			else
			{
				pDemodInfo->bIsAbbMode = FALSE;
				DEMOD_PRINT("[DEMOD] (%s:%d) pDemodInfo->bIsAbbMode = FALSE!!!\n", __F__, __L__);
			}
		}

	}

	DEMOD_LockDevice();

	switch(pDemodInfo->operMode)
	{
		case LX_DEMOD_DVBT :
			{
				pDemodInfo->setCfgParam.dvbt.tuneMode 		= pDemodCfg->dvbt.tuneMode;
				pDemodInfo->setCfgParam.dvbt.channelBW 		= pDemodCfg->dvbt.channelBW;
				pDemodInfo->setCfgParam.dvbt.bSpectrumInv	= pDemodCfg->dvbt.bSpectrumInv;
				pDemodInfo->setCfgParam.dvbt.bSetHp 		= pDemodCfg->dvbt.bSetHp;
				pDemodInfo->setCfgParam.dvbt.FFTMode	 	= pDemodCfg->dvbt.FFTMode;
				pDemodInfo->setCfgParam.dvbt.gIMode	 		= pDemodCfg->dvbt.gIMode;
				pDemodInfo->setCfgParam.dvbt.hierachMode 	= pDemodCfg->dvbt.hierachMode;
				pDemodInfo->setCfgParam.dvbt.lpCodeRate 	= pDemodCfg->dvbt.lpCodeRate;
				pDemodInfo->setCfgParam.dvbt.hpCodeRate 	= pDemodCfg->dvbt.hpCodeRate;
				pDemodInfo->setCfgParam.dvbt.qamMode 		= pDemodCfg->dvbt.qamMode;
				pDemodInfo->systemMode						= pDemodCfg->dvbt.qamMode;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.dvbt.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.dvbt.tuneMode;

			}
			break;

		case LX_DEMOD_DVBT2 :
			{


				pDemodInfo->setCfgParam.dvbt2.tuneMode 		= pDemodCfg->dvbt2.tuneMode;
				pDemodInfo->setCfgParam.dvbt2.channelBW 	= pDemodCfg->dvbt2.channelBW;
				pDemodInfo->setCfgParam.dvbt2.bSpectrumInv	= pDemodCfg->dvbt2.bSpectrumInv;
				pDemodInfo->setCfgParam.dvbt2.FFTMode	 	= pDemodCfg->dvbt2.FFTMode;
				pDemodInfo->setCfgParam.dvbt2.gIMode	 	= pDemodCfg->dvbt2.gIMode;
				pDemodInfo->setCfgParam.dvbt2.codeRate 		= pDemodCfg->dvbt2.codeRate;
				pDemodInfo->setCfgParam.dvbt2.qamMode 		= pDemodCfg->dvbt2.qamMode;
				pDemodInfo->setCfgParam.dvbt2.groupID		= pDemodCfg->dvbt2.groupID;
				pDemodInfo->setCfgParam.dvbt2.unPLP			= pDemodCfg->dvbt2.unPLP;
				pDemodInfo->systemMode						= pDemodCfg->dvbt2.qamMode;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.dvbt2.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.dvbt2.tuneMode;

			}
			break;


		case LX_DEMOD_DVBC :
			{
				pDemodInfo->setCfgParam.dvbc.tuneMode 		= pDemodCfg->dvbc.tuneMode;
				pDemodInfo->setCfgParam.dvbc.channelBW 		= pDemodCfg->dvbc.channelBW;
				pDemodInfo->setCfgParam.dvbc.bSpectrumInv 	= pDemodCfg->dvbc.bSpectrumInv;
				pDemodInfo->setCfgParam.dvbc.qamMode 		= pDemodCfg->dvbc.qamMode;
				pDemodInfo->setCfgParam.dvbc.symbolRate 	= pDemodCfg->dvbc.symbolRate;
				pDemodInfo->setCfgParam.dvbc.frequency		= pDemodCfg->dvbc.frequency;
				pDemodInfo->systemMode						= pDemodCfg->dvbc.qamMode;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.dvbc.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;

			}
			break;

		case LX_DEMOD_DVBC2 :
			{
				pDemodInfo->setCfgParam.dvbc2.tuneMode 		= pDemodCfg->dvbc2.tuneMode;
				pDemodInfo->setCfgParam.dvbc2.channelBW 	= pDemodCfg->dvbc2.channelBW;
				pDemodInfo->setCfgParam.dvbc2.bSpectrumInv 	= pDemodCfg->dvbc2.bSpectrumInv;
				pDemodInfo->setCfgParam.dvbc2.qamMode 		= pDemodCfg->dvbc2.qamMode;
				pDemodInfo->setCfgParam.dvbc2.groupID		= pDemodCfg->dvbc2.groupID;
				pDemodInfo->setCfgParam.dvbc2.unPLP			= pDemodCfg->dvbc2.unPLP;
				pDemodInfo->setCfgParam.dvbc2.sliceID		= pDemodCfg->dvbc2.sliceID;
				pDemodInfo->setCfgParam.dvbc2.frequency		= pDemodCfg->dvbc2.frequency;
				pDemodInfo->systemMode						= pDemodCfg->dvbc2.qamMode;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.dvbc2.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.dvbc2.tuneMode;

			}
			break;

		case LX_DEMOD_ATSC :
			{
				pDemodInfo->setCfgParam.vsb.tuneMode 		= pDemodCfg->vsb.tuneMode;
				pDemodInfo->setCfgParam.vsb.channelBW 		= pDemodCfg->vsb.channelBW;
				pDemodInfo->setCfgParam.vsb.bSpectrumInv 	= pDemodCfg->vsb.bSpectrumInv;
				pDemodInfo->systemMode						= LX_DEMOD_VSB;
				pDemodInfo->bIsTPoutEn 						= TRUE;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.vsb.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.vsb.tuneMode;

			}
			break;


		case LX_DEMOD_QAM :
			{
				pDemodInfo->setCfgParam.qam.tuneMode 		= pDemodCfg->qam.tuneMode;
				pDemodInfo->setCfgParam.qam.channelBW 		= pDemodCfg->qam.channelBW;
				pDemodInfo->setCfgParam.qam.bSpectrumInv 	= pDemodCfg->qam.bSpectrumInv;
				pDemodInfo->setCfgParam.qam.qamMode 		= pDemodCfg->qam.qamMode;
				pDemodInfo->systemMode						= pDemodCfg->qam.qamMode;

				operMode = pDemodInfo->operMode;
				bandWd	 = LX_DEMOD_VSB_NO_OPERATION; //pDemodInfo->setCfgParam.qam.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.qam.tuneMode;

			}
			break;

		case LX_DEMOD_ISDBT :
			{
				pDemodInfo->setCfgParam.isdbt.tuneMode 		= pDemodCfg->isdbt.tuneMode;
				pDemodInfo->setCfgParam.isdbt.channelBW 	= pDemodCfg->isdbt.channelBW;
				pDemodInfo->setCfgParam.isdbt.bSpectrumInv 	= pDemodCfg->isdbt.bSpectrumInv;
				pDemodInfo->setCfgParam.isdbt.FFTMode	 	= pDemodCfg->isdbt.FFTMode;
				pDemodInfo->setCfgParam.isdbt.gIMode	 	= pDemodCfg->isdbt.gIMode;
				pDemodInfo->setCfgParam.isdbt.tmcc		 	= pDemodCfg->isdbt.tmcc;

				operMode = pDemodInfo->operMode;
				bandWd	 = pDemodInfo->setCfgParam.isdbt.channelBW;
				symbr	 = pDemodInfo->symbr;
				tuneMode = pDemodInfo->setCfgParam.isdbt.tuneMode;


			}
			break;


		case LX_DEMOD_ANALOG :
			{
				Handle_t  phDevice;
				BOOLEAN bInv;
				
				phDevice = pDemodInfo->setCfgParam.analog.handleABB;
				
				pDemodInfo->setCfgParam.analog.rfMode 		= pDemodCfg->analog.rfMode;
				pDemodInfo->setCfgParam.analog.tuneMode	 	= pDemodCfg->analog.tuneMode;
				pDemodInfo->setCfgParam.analog.channelBW	= pDemodCfg->analog.channelBW;
				pDemodInfo->setCfgParam.analog.audioSystem	= pDemodCfg->analog.audioSystem;
				pDemodInfo->setCfgParam.analog.frequency	= pDemodCfg->analog.frequency;
				pDemodInfo->setCfgParam.analog.tunedFreq	= pDemodCfg->analog.tunedFreq;

				if(pDemodCfg->analog.audioSystem ==  LX_DEMOD_ANALOG_SIF_SYSTEM_LP )
				{
					pDemodCfg->analog.bSpectrumInv = FALSE;
				}
				
				if(pDemodInfo->setCfgParam.analog.bSpectrumInv !=  pDemodCfg->analog.bSpectrumInv )
				{
					bInv = pDemodCfg->analog.bSpectrumInv;
					DEMOD_ANALOG_Set_SpectrumInv(phDevice, bInv, FALSE );
				}

				pDemodInfo->setCfgParam.analog.bSpectrumInv	= pDemodCfg->analog.bSpectrumInv;


				operMode = pDemodInfo->operMode;


				DEMOD_PRINT("[DEMOD] (%s:%d) pDemodInfo->operMode = LX_DEMOD_ANALOG!!!\n", __F__, __L__);

			}
			break;

		case LX_DEMOD_UNKOWN_OPMODE:
			{
				/* nothing to do  so far */


			}
			break;
		default :
			{
				DEMOD_UnlockDevice();
				DEMOD_RETURN_FAIL(__F__);

			}
	}
	DEMOD_UnlockDevice();


	switch(operMode)
	{
		case LX_DEMOD_DVBT :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBT_Set_DefaultRegisterValue(bandWd))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBT_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_Set_SpectrumCtrl(FALSE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != tuneMode)
				{
					if(RET_OK != DEMOD_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{

#if 1
					if(RET_OK != DEMOD_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_DVBT_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBT_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif

				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");

			}
			break;

		case LX_DEMOD_DVBT2 :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBT2_Set_DefaultRegisterValue(bandWd))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBT2_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_Set_SpectrumCtrl(FALSE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != tuneMode)
				{
					if(RET_OK != DEMOD_DVBT2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#if 0
					if(RET_OK != DEMOD_DVBT2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
#endif
				}
				else
				{

#if 1
					if(RET_OK != DEMOD_DVBT2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBT2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


#else
					if(RET_OK != DEMOD_DVBT2_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBT2_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT2_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif


				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif

				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;


		case LX_DEMOD_DVBC :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBC_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBC2_Set_StartFrequency(pDemodInfo->setCfgParam.dvbc.frequency))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_StartFrequency() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBC_Set_NeverLockWaitTime(LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_Set_SpectrumCtrl(TRUE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_TUNE_NORMAL != tuneMode) && (LX_DEMOD_TUNE_SPEC_DVBC_FIXED_DATA != tuneMode))
				{
					if(RET_OK != DEMOD_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


				}
				else
				{

#if 0
					if(RET_OK != DEMOD_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_DVBC_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, pDemodInfo->setCfgParam.dvbc.symbolRate))  //need to be modified for H14
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC_Set_QamMode(pDemodInfo->setCfgParam.dvbc.qamMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_QamMode() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif
				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif


				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;

		case LX_DEMOD_DVBC2 :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBC2_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_DVBC2_Set_StartFrequency(pDemodInfo->setCfgParam.dvbc2.frequency))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_StartFrequency() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(RET_OK != DEMOD_DVBC2_Set_NeverLockWaitTime(LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_Set_SpectrumCtrl(TRUE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != tuneMode)
				{

					if(RET_OK != DEMOD_DVBC2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbc2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{
					if(RET_OK != DEMOD_DVBC2_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_DVBC2_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbc2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif


				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;


		case LX_DEMOD_ATSC :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_VSB_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				//if(RET_OK != DEMOD_VSB_Set_NeverLockWaitTime(LX_DEMOD_VSB_NEVERLOCK_WAIT_TIME))
				if(RET_OK != DEMOD_VSB_Set_NeverLockWaitTime(600))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_VSB_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(RET_OK != DEMOD_Set_SpectrumCtrl(FALSE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif

				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;

		case LX_DEMOD_QAM :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_QAM_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_QAM_Set_NeverLockWaitTime(LX_DEMOD_QAM_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != tuneMode)
				{

					if(RET_OK != DEMOD_QAM_ModeAutoDetection(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_Set_SpectrumCtrl(TRUE, FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{
					if(RET_OK != DEMOD_QAM_ModeAutoDetection(FALSE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_ModeAutoDetection() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_Set_SpectrumCtrl(TRUE, pDemodInfo->setCfgParam.qam.bSpectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					if(LX_DEMOD_64QAM == pDemodInfo->setCfgParam.qam.qamMode)
					{
						if(RET_OK != DEMOD_QAM_64Mode())
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_64Mode() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else if(LX_DEMOD_256QAM == pDemodInfo->setCfgParam.qam.qamMode)
					{
						if(RET_OK != DEMOD_QAM_256Mode())
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_QAM_256Mode() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else
					{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) Invalid QAM Mode !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
					}

				}

#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif

				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;

		case LX_DEMOD_ISDBT :
			{
				if(RET_OK != DEMOD_StdOperModeContrl(operMode, bandWd,	symbr))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_StdOperModeContrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_ISDBT_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_ISDBT_Set_NeverLockWaitTime(LX_DEMOD_ISDBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_Set_SpectrumCtrl(FALSE, FALSE))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Set_SpectrumCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != tuneMode)
				{
					if(RET_OK != DEMOD_ISDBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_ISDBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.isdbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{

					if(RET_OK != DEMOD_ISDBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_ISDBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.isdbt)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}


#if 0
				if(RET_OK != DEMOD_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
			break;


		case LX_DEMOD_ANALOG :
			{

				if(TRUE == pDemodInfo->setCfgParam.analog.bSpectrumInv)
				{
					switch(pDemodInfo->setCfgParam.analog.pllClkSource)
					{
						case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
							{
								pDemodInfo->setCfgParam.analog.audioSystem	= pDemodCfg->analog.audioSystem;
								pDemodInfo->setCfgParam.analog.frequency	= pDemodCfg->analog.frequency;
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;

									}

								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_42_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_41_250_KHZ;
									
									}

								}
							}
							break;
							
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
									
									}
								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
									
									}

								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
									
									}

								}
							}
							break;
					
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
									
									}

								}
							}
							break;
					
						case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_34_250_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_750_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_33_250_KHZ;
									
									}

								}
							}
							break;
					
						default :			
							DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
							break;
					}	
				}
				else
				{
					switch(pDemodInfo->setCfgParam.analog.pllClkSource)
					{
						case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;
									
									}
					
								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_45_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_46_750_KHZ;
									
									}
					
								}
							}
							break;
							
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
									
									}
					
								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
									
									}
					
								}
							}
							break;
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
									
									}
					
								}
							}
							break;
					
							
						case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
									
									}
					
								}
							}
							break;
					
						case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
							{
								if(LX_DEMOD_NTSC == pDemodInfo->setCfgParam.analog.rfMode)
								{
									pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_37_750_KHZ;
								}
								else
								{
									if(LX_DEMOD_TUNE_AUTO != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
									{
										if((LX_DEMOD_ANALOG_SIF_SYSTEM_BG == pDemodInfo->setCfgParam.analog.audioSystem)&&
											(pDemodInfo->setCfgParam.analog.frequency < LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY))
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_250_KHZ;
										}
										else
										{
											pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
										}
									}
									else
									{
										pDemodInfo->setCfgParam.analog.analogIF = LX_DEMOD_IF_FREQ_38_750_KHZ;
									
									}
					
								}
							}
							break;
					
						default :			
							DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate !!!\n",  __F__, __L__);
							break;
					}	

				}

				DEMOD_PRINT("[DEMOD] (%s:%d) analogIF = %d[KHz] !!!\n", __F__, __L__,pDemodInfo->setCfgParam.analog.analogIF);


#if 0
				phDevice = pDemodInfo->setCfgParam.analog.handleABB;

				if(RET_OK != DEMOD_ANALOG_SoftwareReset(phDevice))
				{
					DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ANALOG_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				DEMOD_LockDevice();

				//Send event for Tuning task
				g_requestMsg = LX_DEMOD_NEWTUNE_REQUEST;
				wake_up_interruptible_all(&g_DEMOD_Tuning_WaitQ);

				DEMOD_UnlockDevice();
				DEMOD_PRINT("[DEMOD] DEMOD Task START !!!\n");
			}
		default :
			break;

	}



	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
* DEMOD_Get_TunedCFG
* Get all tuned configuration parameters
*
* @param *pDemodInfo [IN] DEMOD_CTX_T
* @return if Succeeded - RET_OK else-RET_ERROR
* @see
* @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_TunedCFG(DEMOD_CTX_T *pDemodInfo)
{
	LX_DEMOD_OPER_MODE_T operMode;
	LX_DEMOD_LOCK_STATE_T	lockState;

	if((LX_DEMOD_ANALOG != pDemodInfo->getCfgParam.operMode) && (LX_DEMOD_ANALOG != pDemodInfo->operMode))
	{
		if(RET_OK != DEMOD_Get_OperMode(&operMode))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_OperMode() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(operMode != pDemodInfo->operMode)
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) CRITICAL ERROR !! (Real operation mode is differnt from pDemodInfo->operMode () !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		pDemodInfo->getCfgParam.operMode = operMode;
	}
	else
	{
		operMode = pDemodInfo->operMode;
		pDemodInfo->getCfgParam.operMode =  pDemodInfo->operMode;
	}
	DEMOD_PRINT("[DEMOD](%s:%d) operMode = %d !!!\n", __F__, __L__,operMode);

	switch(operMode)
	{
		case LX_DEMOD_DVBT :
			{
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;
				LX_DEMOD_LOCK_STATE_T 			txModeLock;
				LX_DEMOD_FFTMODE_T				fftMode;
				LX_DEMOD_GIMODE_T 				gIMode;
				LX_DEMOD_DVB_TPS_HIERACHY_T 	hierachMode;
				LX_DEMOD_CODERATE_T 			hpCodeRate;
				LX_DEMOD_CODERATE_T 			lpCodeRate;
				LX_DEMOD_RF_MODE_T 				qamMode;

				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{
#if 0
					//BandWidth ?
					if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
					}
#endif


					//FFT mode ?
					if(RET_OK != DEMOD_DVBT_Get_FFTMode(&txModeLock, &fftMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
					}


					//GI mode ?
					if(RET_OK != DEMOD_DVBT_Get_GuradIntervalMode(&gIMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
					}

					if(RET_OK != DEMOD_Get_Lock(LX_DEMOD_TPS_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


					if(((txModeLock != LX_DEMOD_UNLOCKED) && (gIMode != LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN))|| (lockState != LX_DEMOD_UNLOCKED) )
					{

						//QAM mode	?
						if(RET_OK != DEMOD_Get_QAMMode(&qamMode))
					{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_QAMMode() !!!\n", __F__, __L__);
					}
							pDemodInfo->getCfgParam.dvbt.qamMode		= qamMode;

					//High or Low prioty ?
					if(TRUE == pDemodInfo->setCfgParam.dvbt.bSetHp)
					{
						if(RET_OK != DEMOD_DVBT_Get_HpCoderRate(&hpCodeRate))
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_HpCoderRate() !!!\n", __F__, __L__);
						}

						pDemodInfo->getCfgParam.dvbt.hpCodeRate = hpCodeRate;

					}
					else
					{
						if(RET_OK != DEMOD_DVBT_Get_LpCoderRate(&lpCodeRate))
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_LpCoderRate() !!!\n", __F__, __L__);
						}

						pDemodInfo->getCfgParam.dvbt.lpCodeRate = lpCodeRate;
					}

						//Hierachy mode ?
						if(RET_OK != DEMOD_DVBT_Get_HierachyMode(&hierachMode))
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_HierachyMode() !!!\n", __F__, __L__);
						}
						pDemodInfo->getCfgParam.dvbt.hierachMode	= hierachMode;

						//Spectrum inversion ?
						if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
						{
							DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
						}
						if(LX_DEMOD_AUTO == spectrumAuto)
						{
							pDemodInfo->getCfgParam.dvbt.bSpectrumInv	= (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;
						}
						else
					{
							pDemodInfo->getCfgParam.dvbt.bSpectrumInv	= pDemodInfo->setCfgParam.dvbt.bSpectrumInv;
					}


					}
					else
					{
						DEMOD_PRINT("[DEMOD] FAIL-DVBT (%s:%d) FFT(%d) or GI(%d) is UNLOCKED !!  !!!\n", __F__, __L__,txModeLock,gIMode);

					}

					pDemodInfo->getCfgParam.dvbt.tuneMode		= pDemodInfo->setCfgParam.dvbt.tuneMode;
					pDemodInfo->getCfgParam.dvbt.channelBW		= pDemodInfo->setCfgParam.dvbt.channelBW;
					pDemodInfo->getCfgParam.dvbt.bSetHp 		= pDemodInfo->setCfgParam.dvbt.bSetHp;
					pDemodInfo->getCfgParam.dvbt.FFTMode		= (txModeLock == LX_DEMOD_LOCKED) ? fftMode : LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;
					pDemodInfo->getCfgParam.dvbt.gIMode 		= gIMode;

					pDemodInfo->systemMode						= pDemodInfo->getCfgParam.dvbt.qamMode;
				}

			}
			break;

		case LX_DEMOD_DVBT2 :
			{
				LX_DEMOD_DVBT2_CONFIG_T 		plpParams;
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;


				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{
					//BandWidth ?
					if(RET_OK != DEMOD_DVBT2_Get_PlpInfo( &plpParams))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_H14_DVBT2_Get_PlpInfo() !!!\n", __F__, __L__);
					}

					if(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN)
					{
						DEMOD_PRINT("[DEMOD] FAIL-DVBT (%s:%d) GI(%d) is UNLOCKED !!  !!!\n", __F__, __L__,plpParams.gIMode);
						DEMOD_RETURN_FAIL(__F__);
					}

					//Spectrum inversion ?
					if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
					}
					if(LX_DEMOD_AUTO == spectrumAuto)
					{
						pDemodInfo->getCfgParam.dvbt2.bSpectrumInv	= (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;
					}
					else
					{
						pDemodInfo->getCfgParam.dvbt2.bSpectrumInv	= pDemodInfo->getCfgParam.dvbt.bSpectrumInv;
					}


					pDemodInfo->getCfgParam.dvbt2.tuneMode		= pDemodInfo->setCfgParam.dvbt2.tuneMode;
					pDemodInfo->getCfgParam.dvbt2.channelBW		= pDemodInfo->setCfgParam.dvbt2.channelBW;
					pDemodInfo->getCfgParam.dvbt2.FFTMode		= plpParams.FFTMode;
					pDemodInfo->getCfgParam.dvbt2.gIMode 		= plpParams.gIMode;
					pDemodInfo->getCfgParam.dvbt2.codeRate		= plpParams.codeRate;
					pDemodInfo->getCfgParam.dvbt2.qamMode		= plpParams.qamMode;

					pDemodInfo->systemMode						= plpParams.qamMode;
				}

			}
			break;

		case LX_DEMOD_DVBC :
			{

				LX_DEMOD_BWMODE_T 				channelBW;
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;
				LX_DEMOD_QAMMODE_STATE_T 		qamModeDet;
				LX_DEMOD_SYMBOLRATE_STATE_T 	symbolRateDet;


				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{

					//BandWidth ?
					if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
					}

					//Spectrum inversion ?
					if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
					}

					if(LX_DEMOD_AUTO == spectrumAuto)
					{
						pDemodInfo->getCfgParam.dvbc.bSpectrumInv	= (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;;
					}
					else
					{
						pDemodInfo->getCfgParam.dvbc.bSpectrumInv	= pDemodInfo->setCfgParam.dvbc.bSpectrumInv;
					}

					//QAM mode	?
					if(RET_OK != DEMOD_DVBC_Get_QamModeDetectStatus(&qamModeDet))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_H13_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);
					}
					if((LX_DEMOD_DETECTED == qamModeDet.status) || (LX_DEMOD_DETECTION_MANUAL == qamModeDet.status))
					{
						pDemodInfo->getCfgParam.dvbc.qamMode		= qamModeDet.qamMode;
						pDemodInfo->systemMode						= qamModeDet.qamMode;

					}
					else
					{
						pDemodInfo->getCfgParam.dvbc.qamMode		= LX_DEMOD_UNKOWN_MODE;
						pDemodInfo->systemMode						= LX_DEMOD_UNKOWN_MODE;

					}


					// Symbol rate ?
					if(RET_OK != DEMOD_DVBC_Get_SymbolRateStatus(&symbolRateDet))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);
					}

					if(LX_DEMOD_DETECTED == symbolRateDet.status)
					{
						pDemodInfo->getCfgParam.dvbc.symbolRate = symbolRateDet.symbolRate;

					}
					else
					{
						pDemodInfo->getCfgParam.dvbc.symbolRate = pDemodInfo->setCfgParam.dvbc.symbolRate;

					}

					pDemodInfo->getCfgParam.dvbc.tuneMode		= pDemodInfo->setCfgParam.dvbc.tuneMode;
					pDemodInfo->getCfgParam.dvbc.channelBW		= pDemodInfo->setCfgParam.dvbc.channelBW;
				}

			}
			break;

		case LX_DEMOD_ATSC :
			{

				LX_DEMOD_BWMODE_T 				channelBW;
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;


				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{

					//BandWidth ?
					if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
					}


					//Spectrum inversion ?
					if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
					}
					if(LX_DEMOD_AUTO == spectrumAuto)
					{
						pDemodInfo->getCfgParam.vsb.bSpectrumInv = (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;;
					}
					else
					{
						pDemodInfo->getCfgParam.vsb.bSpectrumInv = pDemodInfo->setCfgParam.vsb.bSpectrumInv ;
					}

					pDemodInfo->getCfgParam.vsb.tuneMode 	= pDemodInfo->setCfgParam.vsb.tuneMode;
					pDemodInfo->getCfgParam.vsb.channelBW 	= channelBW;
					pDemodInfo->systemMode					= LX_DEMOD_VSB;

				}

			}
			break;


		case LX_DEMOD_QAM :
			{
				LX_DEMOD_BWMODE_T 				channelBW;
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;
				LX_DEMOD_RF_MODE_T		 		qamMode;

				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{

					//BandWidth ?
					if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
					}

					//Spectrum inversion ?
					if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
					}

					if(LX_DEMOD_AUTO == spectrumAuto)
					{
						pDemodInfo->getCfgParam.qam.bSpectrumInv	= (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;;
					}
					else
					{
						pDemodInfo->getCfgParam.qam.bSpectrumInv	= pDemodInfo->setCfgParam.qam.bSpectrumInv;
					}

					//QAM mode	?
					if(RET_OK != DEMOD_Get_QAMMode(&qamMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_QAMMode() !!!\n", __F__, __L__);
					}



					pDemodInfo->getCfgParam.qam.tuneMode		= pDemodInfo->setCfgParam.qam.tuneMode;
					pDemodInfo->getCfgParam.qam.channelBW		= pDemodInfo->setCfgParam.qam.channelBW;
					pDemodInfo->getCfgParam.qam.qamMode			= qamMode;

					pDemodInfo->systemMode						= qamMode;
				}

			}
			break;

		case LX_DEMOD_ISDBT :
			{
				LX_DEMOD_BWMODE_T 				channelBW;
				LX_DEMOD_LOCK_STATE_T 			spectrumAuto;
				LX_DEMOD_LOCK_STATE_T 			spectrumInv;
				LX_DEMOD_LOCK_STATE_T 			txModeLock;
				LX_DEMOD_FFTMODE_T				fftMode;
				LX_DEMOD_GIMODE_T 				gIMode;
				LX_DEMOD_ISDBT_TMCC_CONFIG_T 	tmccParams;


				if(LX_DEMOD_LOCKED == pDemodInfo->lockStatus)
				{
					//BandWidth ?
					if(RET_OK != DEMOD_Get_BandWidthMode(&channelBW ))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_BandWidthMode() !!!\n", __F__, __L__);
					}

					//Spectrum inversion ?
					if(RET_OK != DEMOD_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_SpectrumStatus() !!!\n", __F__, __L__);
					}
					if(LX_DEMOD_AUTO == spectrumAuto)
					{
						pDemodInfo->getCfgParam.isdbt.bSpectrumInv	= (spectrumInv == LX_DEMOD_INVERSION) ? TRUE : FALSE;
					}
					else
					{
						pDemodInfo->getCfgParam.isdbt.bSpectrumInv	= pDemodInfo->setCfgParam.isdbt.bSpectrumInv;
					}

					//FFT mode ?
					if(RET_OK != DEMOD_ISDBT_Get_FFTMode(&txModeLock, &fftMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
					}


					//GI mode ?
					if(RET_OK != DEMOD_ISDBT_Get_GuradIntervalMode(&gIMode))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
					}


					//TMCC info ?
					if(RET_OK != DEMOD_ISDBT_Get_TMCCInfo(&tmccParams))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_ISDBT_Get_TMCCInfo() !!!\n", __F__, __L__);
					}


					pDemodInfo->getCfgParam.isdbt.tuneMode		= pDemodInfo->setCfgParam.isdbt.tuneMode;
					pDemodInfo->getCfgParam.isdbt.channelBW		= pDemodInfo->setCfgParam.isdbt.channelBW	;
					pDemodInfo->getCfgParam.isdbt.FFTMode		= (txModeLock == LX_DEMOD_LOCKED) ? fftMode : LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;
					pDemodInfo->getCfgParam.isdbt.gIMode 		= gIMode;
					memcpy(&(pDemodInfo->getCfgParam.isdbt.tmcc), &tmccParams, sizeof(LX_DEMOD_ISDBT_TMCC_CONFIG_T) );
					pDemodInfo->systemMode						= tmccParams.layerBQamMode;

				}

			}
			break;


		case LX_DEMOD_ANALOG :
			{
				pDemodInfo->getCfgParam.analog.freqOffset = pDemodInfo->freqOffset;
			}
			break;

		case LX_DEMOD_UNKOWN_OPMODE:
			{
				/* nothing to do  so far */


			}
			break;
		default :
			break;
	}

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
* DEMOD_Get_signal_status
* Get signal status - SNR, BER, packet  error
*
* @param *pDemodInfo [IN] DEMOD_CTX_T
* @return if Succeeded - RET_OK else-RET_ERROR
* @see
* @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_Get_signal_status(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_MSE_T sigMse;
	UINT16 			ifAgc;
	UINT32			packetError;
	UINT32			vBer;			/* unit : e-10 */
	SINT32 			freqOff;


	if(NULL == pDemodInfo) return RET_ERROR;

	pDemodInfo->signalStatus.operMode 	= pDemodInfo->operMode;
	pDemodInfo->signalStatus.rfMode	= pDemodInfo->systemMode;

		if(RET_OK != DEMOD_Get_CarrierFreqOffset(&freqOff))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[DEMOD] carrier offset = %d !!!\n", freqOff);

		if(RET_OK != DEMOD_Get_IFAGC(&ifAgc))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_IFAGC() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[DEMOD] IF AGC = %d !!!\n", ifAgc);

		if(RET_OK != DEMOD_Get_MseInfo(&sigMse))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_MseInfo() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[DEMOD] MSE = %d, BMSE = %d, constPwr = %d  !!!\n", sigMse.mse, sigMse.bMse, sigMse.constPwr);

		if(RET_OK != DEMOD_Get_Packet_Error(&packetError))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Packet_Error() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[DEMOD] PKERROR = %d !!!\n", packetError);


		if(RET_OK != DEMOD_Get_VABER(&vBer))
		{
			DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_Get_Packet_Error() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[DEMOD] BER  = %d !!!\n", vBer);

	pDemodInfo->signalStatus.packetError 	= packetError;
	pDemodInfo->signalStatus.ber			= vBer;
	pDemodInfo->signalStatus.agc			= ifAgc;
	pDemodInfo->signalStatus.m.mse			= sigMse.mse;
	pDemodInfo->signalStatus.m.bMse			= sigMse.bMse;
	pDemodInfo->signalStatus.m.constPwr		= sigMse.constPwr;

	pDemodInfo->freqOffset 					= freqOff;


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
* DEMOD_UnLock_Notify
* This function is called from the TunerTask loop when
     in NOT_LOCKED state. If 2 seconds has elapsed since the
     core called DDI_TUNER_RequestSatLock and the core has not yet
     been notified, this function notifies the core of the
     not locked condition.
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

static int DEMOD_UnLock_Notify(UINT8 deviceNum)
{

	unsigned int currentTime = 0;
	unsigned int elapsedTime = 0;

	DEMOD_CTX_T *pDemodInfo;

	pDemodInfo = &g_demod_ctx_info[deviceNum];

	currentTime = jiffies_to_msecs(jiffies);


	if(!pDemodInfo->bNotifiedUnlock)
	{
		elapsedTime = currentTime - g_RetuneStartTime;

		if(elapsedTime > notifyinglockTimeOut)
		{
			pDemodInfo->bNotifiedUnlock = TRUE;
			pDemodInfo->bNotifiedLock = FALSE;
			pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;

			DEMOD_PRINT_ALWAYS("\n DEMOD_UnLock_Notify : MSG_FRONT_END_NOT_LOCKED..\n") ;
			DEMOD_PRINT_ALWAYS("DEMOD_UnLock_Notify : Elapse time is %d msec\n", elapsedTime) ;
		}

	}

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * CheckDemodPayloads_and_Pause.
 * Set channel change parameters
 *
 * @param deviceNum [IN] UINT8
 * @param timeout [IN] UINT64
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

static void DEMOD_CheckDemodPayloads_and_Pause(UINT8 deviceNum, UINT64 timeout)
{

	UINT64	ui64TaskTimeout;

	DEMOD_CTX_T *pDemodInfo;
	pDemodInfo = &g_demod_ctx_info[deviceNum];

	ui64TaskTimeout = timeout;

	wait_event_interruptible_timeout( g_DEMOD_Tuning_WaitQ,	\
									  g_requestMsg != LX_DEMOD_TASK_NONE_REQUEST,	\
									  msecs_to_jiffies(ui64TaskTimeout) );


	DEMOD_LockDevice();
	switch (g_requestMsg)
	{
		case LX_DEMOD_NEWTUNE_REQUEST:
			{
				DEMOD_PRINT_ALWAYS( "[GBB/ABB] LX_DEMOD_NEWTUNE_REQUEST !!!\n");
				if(pDemodInfo->operMode != LX_DEMOD_UNKOWN_OPMODE)
				{

					pDemodInfo->bSetParams 					= TRUE;

		   			pDemodInfo->lockStatus 					= LX_DEMOD_UNLOCKED;
					pDemodInfo->bNotifiedLock				= FALSE;
					pDemodInfo->bNotifiedUnlock 			= FALSE;

					pDemodInfo->controlState 				= LX_DEMOD_TUNE_START;
					pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;
					pDemodInfo->scanJobFlag					= LX_DEMOD_FLAG_START;
					pDemodInfo->bMonitorEnable 				= TRUE;

					pDemodInfo->bNotifiedUnlock 			= FALSE;
					pDemodInfo->bNotifiedLock 				= FALSE;
					pDemodInfo->unLockCount					= 0;
					pDemodInfo->unLockLongCount 			= 0;
					pDemodInfo->lockCount					= 0;



					pDemodInfo->signalStatus.agc 			= 0;
					pDemodInfo->signalStatus.ber 			= 0xffff;
					pDemodInfo->signalStatus.bSignalValid 	= FALSE;
					pDemodInfo->signalStatus.m.mse 			= 0xffff;
					pDemodInfo->signalStatus.packetError 	= 0xffffffff;

					pauseTimeDemod 							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;

					g_requestMsg 							= LX_DEMOD_TASK_NONE_REQUEST;

					g_NewTuneReqeustTime					= jiffies_to_msecs(jiffies);
					g_RetuneStartTime						= g_NewTuneReqeustTime;

					notifyinglockTimeOut 					= LX_DEMOD_LOCK_DEALY_2500;

					memset(&(pDemodInfo->getCfgParam), 0x0, sizeof(LX_DEMOD_CONFIG_T) );

					DEMOD_PRINT("\nDEMOD_NEWTUNE : Digtal Tune From Channel Manager !!\n");
					DEMOD_PRINT("\nDEMOD_NEWTUNE : pDemodInfo->operMode = %d !!\n",pDemodInfo->operMode);
					DEMOD_PRINT("\nDEMOD_NEWTUNE : pDemodInfo->acqState = %d !!\n",pDemodInfo->acqState);
					DEMOD_PRINT("\nDEMOD_NEWTUNE : pDemodInfo->bSetParams = %d !!\n",pDemodInfo->bSetParams);


				}
				else
				{
					pDemodInfo->bSetParams 					= FALSE;

					pDemodInfo->bNotifiedLock				= FALSE;
					pDemodInfo->bNotifiedUnlock 			= FALSE;
		   			pDemodInfo->lockStatus 					= LX_DEMOD_UNLOCKED;

					pDemodInfo->controlState 				= LX_DEMOD_TUNE_START;
					pDemodInfo->acqState					= LX_DEMOD_IDLE_PROCESS;
					pDemodInfo->scanJobFlag					= LX_DEMOD_FLAG_START;

					pDemodInfo->bMonitorEnable 				= FALSE;
					pDemodInfo->bNotifiedUnlock				= FALSE;
					pDemodInfo->bNotifiedLock 				= FALSE;
					pDemodInfo->unLockCount					= 0;
					pDemodInfo->unLockLongCount 			= 0;
					pDemodInfo->lockCount					= 0;

					pDemodInfo->signalStatus.agc 			= 0;
					pDemodInfo->signalStatus.ber 			= 0xffff;
					pDemodInfo->signalStatus.bSignalValid 	= FALSE;
					pDemodInfo->signalStatus.m.mse 			= 0xffff;
					pDemodInfo->signalStatus.packetError 	= 0xffffffff;


					pauseTimeDemod							= LX_DEMOD_FOREVER_PAUSE_TIME;
					g_requestMsg 							= LX_DEMOD_TASK_NONE_REQUEST;

					memset(&(pDemodInfo->getCfgParam), 0x0, sizeof(LX_DEMOD_CONFIG_T) );

					DEMOD_PRINT("\nDEMOD_NEWTUNE : Wrong tune From Channel Manager  !!\n");
				}

			}
			break;

		case LX_DEMOD_STOP_REQEST :
			{
				pDemodInfo->bSetParams					= FALSE;

				pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedLock				= FALSE;
				pDemodInfo->bNotifiedUnlock 			= FALSE;

				pDemodInfo->controlState				= LX_DEMOD_TUNE_START;
				pDemodInfo->acqState					= LX_DEMOD_IDLE_PROCESS;

				pDemodInfo->bMonitorEnable				= FALSE;
				pDemodInfo->bNotifiedUnlock 			= FALSE;
				pDemodInfo->bNotifiedLock				= FALSE;
				pDemodInfo->unLockCount 				= 0;
				pDemodInfo->unLockLongCount 			= 0;
				pDemodInfo->lockCount					= 0;

				pDemodInfo->signalStatus.agc			= 0;
				pDemodInfo->signalStatus.ber			= 0xffff;
				pDemodInfo->signalStatus.bSignalValid	= FALSE;
				pDemodInfo->signalStatus.m.mse			= 0xffff;
				pDemodInfo->signalStatus.packetError	= 0xffffffff;

				pauseTimeDemod							= LX_DEMOD_FOREVER_PAUSE_TIME;
				g_requestMsg							= LX_DEMOD_TASK_NONE_REQUEST;

				memset(&(pDemodInfo->getCfgParam), 0x0, sizeof(LX_DEMOD_CONFIG_T) );

				DEMOD_PRINT("\n LX_DEMOD_STOP_REQEST : Task STOP From Channel Manager  !!\n");
			}
			break;

			default :	break;

	}  /*switch (msg.typeB.msgID & 0xf)*/

	DEMOD_UnlockDevice();
//	DEMOD_PRINT(" DEMOD_CheckDemodPayloads_and_Pause : %s  \n",(g_requestMsg == LX_DEMOD_NEWTUNE_REQUEST) ? "New Tune" :
//															(g_requestMsg == LX_DEMOD_STOP_REQEST) ? "STOP" : "NO request");

}






/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Tuning_Task.
 * Set channel change parameters
 *
 * @param NONE  [IN/OUT] void
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Tuning_Task(void *i_pvParam)
{

	UINT8 deviceNum = 0 ;

	DEMOD_CTX_T *pDemodInfo;

	pDemodInfo = &g_demod_ctx_info[deviceNum];

				/*******************************************************/
	FOREVER 	/****** This is where TunerTask runs from forever ******/
	{			/*******************************************************/

		if(pDemodInfo->bSetParams)
		{

			DEMOD_LockDevice();
			switch(pDemodInfo->acqState) /* 3 possible tuner states NOT_LOCKED, LOCKED and NTSC_LOCKED */

			{
				case LX_DEMOD_TUNNING_PROCESS:
					{

//						pauseTimeDemod = LX_DEMOD_NOT_LOCKED_PAUSE_TIME;

						switch(pDemodInfo->operMode)
						{
							case LX_DEMOD_DVBT :
								DEMOD_DVBT_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBT2 :
								DEMOD_DVBT2_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBC :
								DEMOD_DVBC_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBC2 :
								DEMOD_DVBC2_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ATSC :
								DEMOD_VSB_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_QAM :
								DEMOD_QAM_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ISDBT :
								DEMOD_ISDBT_Obtaining_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ANALOG :
								DEMOD_ANALOG_Obtaining_Signal_Lock(pDemodInfo);
							default :
								break;

						}

						if(pDemodInfo->controlState == LX_DEMOD_SIGNAL_LOCKED )
						{
							pDemodInfo->acqState = LX_DEMOD_MONITOR_PROCESS;
							pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;
						}
						else if(pDemodInfo->controlState == LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK)
						{
							pDemodInfo->acqState = LX_DEMOD_MONITOR_PROCESS;
							pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;
							DEMOD_PRINT(" pDemodInfo->controlState == LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK \n  ");
						}
						else if(pDemodInfo->bNotifiedUnlock == TRUE)
						{
//							pDemodInfo->acqState = LX_DEMOD_MONITOR_PROCESS;
							pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;
						}


						else
						{
							/*** Notify core of not locked if 2 seconds elapsed ***/
							/*** since last call to HDIRequestSatLock.			***/
							DEMOD_UnLock_Notify(deviceNum);
						}


					}
					break;


				case LX_DEMOD_MONITOR_PROCESS:
					{

//						pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

						switch(pDemodInfo->operMode)
						{
							case LX_DEMOD_DVBT :
								DEMOD_DVBT_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBT2 :
								DEMOD_DVBT2_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBC :
								DEMOD_DVBC_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_DVBC2 :
								DEMOD_DVBC2_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ATSC :
								DEMOD_VSB_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_QAM :
								DEMOD_QAM_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ISDBT :
								DEMOD_ISDBT_Monitoring_Signal_Lock(pDemodInfo);
								break;

							case LX_DEMOD_ANALOG :
								DEMOD_ANALOG_Monitoring_Signal_Lock(pDemodInfo);
							default :
								break;

						}

					}
					break;

				case LX_DEMOD_IDLE_PROCESS :
					{

						pauseTimeDemod = LX_DEMOD_FOREVER_PAUSE_TIME;

					}
					break;

				default :
					pDemodInfo->acqState = LX_DEMOD_TUNNING_PROCESS;
					DEMOD_PRINT(" Oops, Invaild control !!, Going to LX_DEMOD_UNLOCKED \n  ");
					break;
			} /* switch */
			DEMOD_UnlockDevice();
		}

		DEMOD_CheckDemodPayloads_and_Pause(deviceNum, pauseTimeDemod);


	} /* FOREVER */

	return RET_OK;
}


#if 0
#endif


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_RepeaterEnable.
 * Enable / Disable GBB I2C repeater function
 *
 * @param bEnable [IN] 	BOOLEAN - Enable/Disable I2C repeater
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_RepeaterEnable(BOOLEAN bEnable)
{
	if(NULL == g_demod_hal.RepeaterEnable)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.RepeaterEnable(bEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_SoftwareReset
 * Reset GBB module by software.
 *
 * @param  NONE
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_SoftwareReset(void)
{
	if(NULL == g_demod_hal.SoftwareReset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.SoftwareReset())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Serial_Control
 * Control TS output mode(serial/Parallel)
 *
 * @param bEnable 	[IN] Serial(TRUE) / Parallel(FALSE)
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_Serial_Control(BOOLEAN bEnable)
{
	if(NULL == g_demod_hal.Serial_Control)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Serial_Control(bEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Power_Save
 * Enable/Disable power save mode
 *
 * @param bEnable 	[IN] power save mode(TRUE) / normal mode(FALSE)
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Power_Save(BOOLEAN bEnable)
{
	if(NULL == g_demod_hal.Power_Save)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Power_Save(bEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}




/***********************************************************************************
************************************************************************************/
/**
* DEMOD_StdOperModeContrl
* Set standard operation mode, Bandwidth and symbol rate
*
* @param operMode [IN] LX_DEMOD_OPER_MODE_T - DVBT, DVBC, ATSC, QAM, ISDBT
* @param bandWd [IN] LX_DEMOD_BWMODE_T
* @param symbr [IN] LX_DEMOD_SYMRDET_MODE_T.
* @return if Succeeded - RET_OK else-RET_ERROR
* @see
* @author  Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_StdOperModeContrl( LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr)
{
	if(NULL == g_demod_hal.StdOperModeContrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.StdOperModeContrl(operMode, bandWd, symbr ))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/**
 * DEMOD_NeverlockScan
 * Get the neverlock flag indicator.
 *
 * @param	*pDemodInfo			[IN] DEMOD_CTX_T.
 * @param	ResetOn				[IN] 	LX_DEMOD_FLAG_T  - force to initialize the postjob.
 * @param	*pScanProcessing		[OUT] LX_DEMOD_FLAG_T- the flag that scan is done or not done.
 * @param	*pScanLock			[OUT] LX_DEMOD_LOCK_STATE_T - scan lock result.
 * @return if Succeeded - RET_OK else-TU_ERROR
 * @author 	Jeongpil.yun(jeongpil.yun@lge.com)
*/
int DEMOD_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock)
{
	if(NULL == g_demod_hal.NeverlockScan)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.NeverlockScan(pDemodInfo, ResetOn, pScanProcessing, pScanLock))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_TPOutCLKEnable
 * Enable/Diable the output of TP CLK
 *
 * @param bEnable 	[IN] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_TPOutCLKEnable(BOOLEAN bEnable)
{
	if(NULL == g_demod_hal.TPOutCLKEnable)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.TPOutCLKEnable(bEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Set_IF_Frq
 * Set IF frequency registers on DVB-C mode.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Set_IF_Frq(UINT32 ifFrq)
{
	if(NULL == g_demod_hal.Set_IF_Frq)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Set_IF_Frq(ifFrq))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Set_AGCPolarity
 * Set IF frequency registers on DVB.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Set_AGCPolarity(LX_DEMOD_AGC_POLAR_T agcPola)
{
	if(NULL == g_demod_hal.Set_AGCPolarity)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Set_AGCPolarity(agcPola))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Set_SpectrumCtrl
 * Set IF frequency registers on DVB.
 *
 * @param bAutoEnable 		[IN] BOOLEAN - auto or manual.
 * @param bSpecInvEnable 	[IN] BOOLEAN - normal or inversion.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Set_SpectrumCtrl(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable)
{
	if(NULL == g_demod_hal.Set_SpectrumCtrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Set_SpectrumCtrl(bAutoEnable, bSpecInvEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_Id.
 * Get GBB demod ID.
 *
 * @param *cell_id [OUT] UINT16 - GBB demod ID
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_Id(UINT16 *pId)  //jeongpil.yun
{
	if(NULL == g_demod_hal.Get_Id)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_Id(pId))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_SpectrumStatus.
 * Get spectrum mode info.
 * It is only useful when auto spectrum mode detection is set.
 *
 * @param  *pSpectrumAuto [OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_AUTO or LX_DEMOD_MANUAL.
 * @param  *pSpectrumInv [OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_NORMAL or LX_DEMOD_INVERSION.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_SpectrumStatus(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv)
{
	if(NULL == g_demod_hal.Get_SpectrumStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_SpectrumStatus(pSpectrumAuto, pSpectrumInv))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_IFAGC
 * Get IF AGC value
 *
 * @param *pIfAgc 	[OUT] UINT16 .
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_IFAGC(UINT16 *pIfAgc)
{
	if(NULL == g_demod_hal.Get_IFAGC)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_IFAGC(pIfAgc))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_OperMode.
 * Get GBB operation mode.
 *
 * @param *pOperMode [OUT] LX_DEMOD_OPER_MODE_T - GBB operation mode
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_OperMode(LX_DEMOD_OPER_MODE_T *pOperMode)
{
	if(NULL == g_demod_hal.Get_OperMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_OperMode(pOperMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_CarrierFreqOffset
 * Get carrier offset frequency in KHz
 *
 * @param *pFreqOff 	[OUT] SINT32 - offset frequency: unit: KHz.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_CarrierFreqOffset(SINT32 *pFreqOff)
{
	if(NULL == g_demod_hal.Get_CarrierFreqOffset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_CarrierFreqOffset(pFreqOff))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_TPIFStatus
 * @param *pbEnable	  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_TPIFStatus(BOOLEAN *pbEnable)
{
	if(NULL == g_demod_hal.Get_TPIFStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_TPIFStatus(pbEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_VABER
 * Get BER before RS on DVB
 *
 * @param berInterval 	[IN] VABERCNTMODE_T - 10^6 data is default
 * @param *pVber 	[OUT] UINT32.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_VABER(UINT32 *pVber)
{
	if(NULL == g_demod_hal.Get_VABER)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_VABER(pVber))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_Packet_Error
 * Get packet error
 *
 * @param *pSnr 	[OUT] UINT8.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_Packet_Error(UINT32 *pError)
{
	if(NULL == g_demod_hal.Get_Packet_Error)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_Packet_Error(pError))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_BandWidthMode
 * Get the information of bandwidth .
 *
 * @param *ChannelBW 	[OUT] LX_DEMOD_BWMODE_T - 6, 7 and 8 MHZ.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_BandWidthMode( LX_DEMOD_BWMODE_T *ChannelBW )
{
	if(NULL == g_demod_hal.Get_BandWidthMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_BandWidthMode(ChannelBW))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 DEMOD_Get_QAMMode
 * Get the information of QAM mode .
 *
 * @param *pQamMode [OUT] LX_DEMOD_RF_MODE_T - 16QAM, 32QAM, 64QAM, 128QAM, 256QAM, QPSK.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_QAMMode( LX_DEMOD_RF_MODE_T *pQamMode )
{
	if(NULL == g_demod_hal.Get_QAMMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_QAMMode(pQamMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Get_MseInfo
 * Get signal MSE info.
 *
 * @param *pSigMse [OUT] LX_DEMOD_MSE_T.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_Get_MseInfo( LX_DEMOD_MSE_T *pSigMse)
{
	if(NULL == g_demod_hal.Get_MseInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_MseInfo(pSigMse))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_Lock
 * Check the DVB lock state
 *
 * @param whatLock 		[IN] LX_DEMOD_LOCK_STATUS_T - which lock flags are selected.
 * @param lockState 	[OUT] LX_DEMOD_LOCK_STATE_T - LOCKED OR NOT LOCKED.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_Get_Lock(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState)
{
	if(NULL == g_demod_hal.Get_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_Lock(whatLock, lockState))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

int DEMOD_Get_SysLockTime(UINT16 *pSyslocktime)
{
	if(NULL == g_demod_hal.Get_SysLockTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_SysLockTime(pSyslocktime))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

int DEMOD_Get_FecLockTime(UINT16 *pFeclocktime)
{
	if(NULL == g_demod_hal.Get_FecLockTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.Get_FecLockTime(pFeclocktime))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



#if 0
#endif

/***********************************************************************************
* DVB common FUNCTIONS
************************************************************************************/



#if 0
#endif

/***********************************************************************************
* DVBC function
************************************************************************************/


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_AutoSymbolRateDet
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  symrateMode  [IN]  LX_DEMOD_SYMRDET_MODE_T.
 * @param  SymbolRate  [IN]   UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate)
{
	if(NULL == g_demod_hal.DVBC_AutoSymbolRateDet)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_AutoSymbolRateDet(symrateMode, SymbolRate))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_IsSymbolRateAuto
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  *bAuto - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto)
{
	if(NULL == g_demod_hal.DVBC_IsSymbolRateAuto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_IsSymbolRateAuto(bAuto))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_IsQammodeAutoDet
 * Control automatic setting of DVBC config.
 *
 * @param *bAutoQam 	[OUT] BOOLEAN - dectection automatically  (TRUE) or manually(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{
	if(NULL == g_demod_hal.DVBC_IsQammodeAutoDet)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_IsQammodeAutoDet(bAutoQam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Set_DefaultRegisterValue(void)
{
	if(NULL == g_demod_hal.DVBC_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Set_DefaultRegisterValue())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Set_Config_auto (BOOLEAN bAutoDetect)
{
	if(NULL == g_demod_hal.DVBC_Set_Config_auto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Set_Config_auto(bAutoDetect))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_DVBC_Set_QamMode (LX_DEMOD_RF_MODE_T constel)
{
	if(NULL == g_demod_hal.DVBC_Set_QamMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Set_QamMode(constel))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.DVBC_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Get_SymbolRateDetect
 * Get symbol rate in KHz (Don't use this function until revision A1)
 *
 * @param *pSymbolRate 	[OUT] UINT16 - symbol rate : unit: KHz.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate)
{
	if(NULL == g_demod_hal.DVBC_Get_SymbolRateDetect)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Get_SymbolRateDetect(pSymbolRate))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Get_SymbolRateStatus
 * Get symbol rate detection state on DVB-C
 * Don't use this function until revision A1.
 *
 * @param *pSymbolRateDet 	[OUT] SYMBOLRATE_STATE_T - status and detected symbol rate .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet)
{
	if(NULL == g_demod_hal.DVBC_Get_SymbolRateStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Get_SymbolRateStatus(pSymbolRateDet))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{
	if(NULL == g_demod_hal.DVBC_Get_QamModeDetectStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Get_QamModeDetectStatus(pQamModeDet))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Get_DvbInfo
 * Get all information on DVB-C for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Get_DvbInfo(void)
{
	if(NULL == g_demod_hal.DVBC_Get_DvbInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Get_DvbInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBC_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{


	if(NULL == g_demod_hal.DVBC_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}

#if 0
#endif




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_IsQammodeAutoDet
 * Control automatic setting of DVBC2 config.
 *
 * @param *bAutoQam 	[OUT] BOOLEAN - dectection automatically  (TRUE) or manually(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{
	if(NULL == g_demod_hal.DVBC2_IsQammodeAutoDet)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_IsQammodeAutoDet(bAutoQam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_DefaultRegisterValue(void)
{
	if(NULL == g_demod_hal.DVBC2_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_DefaultRegisterValue())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_Config_auto (BOOLEAN bAutoDetect)
{
	if(NULL == g_demod_hal.DVBC2_Set_Config_auto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_Config_auto(bAutoDetect))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_DVBC2_Set_Config_auto is enabled.
 *
 * @param *pDvbc2ConfigParam 	[IN] LX_DEMOD_DVBC2_CONFIG_T - DVB-C2 config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_PartialConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam)
{
	if(NULL == g_demod_hal.DVBC2_Set_PartialConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_PartialConfig(pDvbc2ConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_FullConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_DVBC2_Set_Config_auto is enabled.
 *
 * @param *pDvbc2ConfigParam 	[IN] LX_DEMOD_DVBC2_CONFIG_T - DVB-C2 config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_FullConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam)
{
	if(NULL == g_demod_hal.DVBC2_Set_FullConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_FullConfig(pDvbc2ConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_DVBC2_Set_QamMode (UINT8 constel)
{
	if(NULL == g_demod_hal.DVBC2_Set_QamMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_QamMode(constel))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.DVBC2_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Set_StartFrequency
 * Set start frequency.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Set_StartFrequency(UINT32 frequency)
{
	if(NULL == g_demod_hal.DVBC2_Set_StartFrequency)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Set_StartFrequency(frequency))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{
	if(NULL == g_demod_hal.DVBC2_Get_QamModeDetectStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Get_QamModeDetectStatus(pQamModeDet))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Get_multiPLP_ID.
 * Set channel change parameters
 *
 * @param *pParamMultiPLPInfo  [OUT] LX_DEMOD_DVBC2_MULTI_PLP_INFO_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Get_multiPLP_ID(LX_DEMOD_DVBC2_MULTI_PLP_INFO_T *pParamMultiPLPInfo)
{

	if(NULL == g_demod_hal.DVBC2_Get_multiPLP_ID)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Get_multiPLP_ID(pParamMultiPLPInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Get_DvbInfo
 * Get all information on DVB-C2 for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Get_DvbInfo(void)
{
	if(NULL == g_demod_hal.DVBC2_Get_DvbInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Get_DvbInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBC2_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBC2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{


	if(NULL == g_demod_hal.DVBC2_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBC2_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}




#if 0
#endif
/***********************************************************************************
* DVBT function
************************************************************************************/


/**
 * DEMOD_DVBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	if(NULL == g_demod_hal.DVBT_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Set_DefaultRegisterValue(band))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Set_Config_auto (BOOLEAN bAutoDetect)
{
	if(NULL == g_demod_hal.DVBT_Set_Config_auto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Set_Config_auto(bAutoDetect))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{
	if(NULL == g_demod_hal.DVBT_Set_PartialConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Set_PartialConfig(pDvbtConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{
	if(NULL == g_demod_hal.DVBT_Set_FullConfig )
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Set_FullConfig (pDvbtConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.DVBT_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{
	if(NULL == g_demod_hal.DVBT_Get_DelaySpreadStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_DelaySpreadStatus(pDelaySpread))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_Hierach_HPSel
 * Get the information if the hierachical HP is selected.
 *
 * @param *pBSetHp 	[OUT] BOOLEAN - HP (TRUE) / LP(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp )
{
	if(NULL == g_demod_hal.DVBT_Get_Hierach_HPSel)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_Hierach_HPSel(pBSetHp))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{
	if(NULL == g_demod_hal.DVBT_Get_FFTMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_FFTMode(pTxModeLock, pFftMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{
	if(NULL == g_demod_hal.DVBT_Get_GuradIntervalMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_GuradIntervalMode(pGIMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_HierachyMode
 * Get the information of hierachical mode on DVB-T.
 *
 * @param *pHierachMode[OUT] DEMOD_H13_DVB_TPS_HIERACHY_T - alpha = none, 1, 2 and 4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode )
{
	if(NULL == g_demod_hal.DVBT_Get_HierachyMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_HierachyMode(pHierachMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_LpCoderRate
 * Get the information of LP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_DVB_TPS_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate )
{
	if(NULL == g_demod_hal.DVBT_Get_LpCoderRate)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_LpCoderRate(pLpCodeRate))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_HpCoderRate
 * Get the information of HP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_DVB_TPS_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate )
{
	if(NULL == g_demod_hal.DVBT_Get_HpCoderRate)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_HpCoderRate(pHpCodeRate))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_CellId
 * Get TPS cell ID on DVB-T.
 *
 * @param *pCellIDLength[OUT] UINT8 - cell ID length.
 * @param *cell_id[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id)
{
	if(NULL == g_demod_hal.DVBT_Get_CellId)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_CellId(pCellIDLength, cell_id))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_TpsInfo
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams)
{
	if(NULL == g_demod_hal.DVBT_Get_TpsInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_TpsInfo(pTpsParams))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_TotalInfo
 * Get all information on DVB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_TotalInfo( void)
{
	if(NULL == g_demod_hal.DVBT_Get_TotalInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_TotalInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_IFO_LOCK
 * Get IFO lock on DVB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{
	if(NULL == g_demod_hal.DVBT_Get_IFO_LOCK)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_IFO_LOCK(pIfoLock))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on DVB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{
	if(NULL == g_demod_hal.DVBT_Get_CochanDetIndicator)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Get_CochanDetIndicator(pDetStatus))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_EqualizereReset(void)
{
	if(NULL == g_demod_hal.DVBT_EqualizereReset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_EqualizereReset())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBT_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBC_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBT_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;



}

#if 0
#endif
/***********************************************************************************
* DVBT2 function
************************************************************************************/


/**
 * DEMOD_DVBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T2.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	if(NULL == g_demod_hal.DVBT2_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Set_DefaultRegisterValue(band))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Set_Config_auto
 * Control automatic setting of DVB-T2 config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Set_Config_auto (BOOLEAN bAutoDetect)
{
	if(NULL == g_demod_hal.DVBT2_Set_Config_auto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Set_Config_auto(bAutoDetect))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbt2ConfigParam 	[IN] LX_DEMOD_DVBT2_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Set_PartialConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam)
{
	if(NULL == g_demod_hal.DVBT2_Set_PartialConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Set_PartialConfig(pDvbt2ConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Set_FullConfig
 * Set all config parameters on DVB-T2.
 * This function is used when LX_DEMOD_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Set_FullConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam)
{
	if(NULL == g_demod_hal.DVBT2_Set_FullConfig )
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Set_FullConfig (pDvbt2ConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.DVBT2_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T2.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{
	if(NULL == g_demod_hal.DVBT2_Get_DelaySpreadStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_DelaySpreadStatus(pDelaySpread))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_FFTMode
 * Get the information of bandwidth on DVB-T2.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{
	if(NULL == g_demod_hal.DVBT2_Get_FFTMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_FFTMode(pTxModeLock, pFftMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T2.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_DVBT2_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{
	if(NULL == g_demod_hal.DVBT2_Get_GuradIntervalMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_GuradIntervalMode(pGIMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_LpCoderRate
 * Get the information of LP code rate on DVB-T2.
 *
 * @param *pCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_CoderRate( LX_DEMOD_CODERATE_T *pCodeRate )
{
	if(NULL == g_demod_hal.DVBT2_Get_CoderRate)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_CoderRate(pCodeRate))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_MultiPLPInfo.
 * Set channel change parameters
 *
 * @param *pDemodInfo [OUT] LX_DEMOD_DVBT2_CONFIG_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_PlpInfo(LX_DEMOD_DVBT2_CONFIG_T *pPlpParams)
{

	if(NULL == g_demod_hal.DVBT2_Get_PlpInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_PlpInfo(pPlpParams))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_multiPLP_ID.
 * Set channel change parameters
 *
 * @param *pParamMultiPLPInfo  [OUT] LX_DEMOD_DVBT2_MULTI_PLP_INFO_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_multiPLP_ID(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T *pParamMultiPLPInfo)
{

	if(NULL == g_demod_hal.DVBT2_Get_multiPLP_ID)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_multiPLP_ID(pParamMultiPLPInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Get_TotalInfo
 * Get all information on DVB-T2 for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Get_TotalInfo( void)
{
	if(NULL == g_demod_hal.DVBT2_Get_TotalInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Get_TotalInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBT2_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_DVBT2_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_DVBT2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.DVBT2_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.DVBT2_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;



}



#if 0
#endif


/***********************************************************************************
* VSB  function
************************************************************************************/

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Set_DefaultRegisterValue.
 * Initialize GBB VSB module .
 *
 * @param  NONE [IN] void
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Set_DefaultRegisterValue(void)  //jeongpil.yun
{
	if(NULL == g_demod_hal.VSB_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Set_DefaultRegisterValue())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.VSB_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_CochannelExist.
 * Get the informantion if co-channel(NTSC) signal exists or not on VSB mode.
 *
 * @param  *pCochannel [OUT] BOOLEAN - exists /doesn't exist.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_CochannelExist(BOOLEAN *pCochannel)	//jeongpil.yun
{
	if(NULL == g_demod_hal.VSB_CochannelExist)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_CochannelExist(pCochannel))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_PreMonitor.
 * Set registers against  various noise conditions before  monitoring.
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_PreMonitor(void)
{
	if(NULL == g_demod_hal.VSB_PreMonitor)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_PreMonitor())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Monitor.
 * Get the RF signal status - phase noise signal, ghost signal, sync SNR B condition, phase tracker SNR A condition..
 *
 * @param  *pDemodInfo [IN] DEMOD_CTX_T .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Monitor(DEMOD_CTX_T *pDemodInfo)
{
	if(NULL == g_demod_hal.VSB_Monitor)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Monitor(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Get_MSEdynStatus.
 * Get the flag whether MSE would be under operation limit (snr > 16dB : TRUE)
 *
 * @param  *pbEnable [OUT] BOOLEAN - get signal noise ratio
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Get_MSEdynStatus(BOOLEAN *pbEnable)
{
	if(NULL == g_demod_hal.VSB_Get_MSEdynStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Get_MSEdynStatus(pbEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Get_TotalInfo
 * Get all information on VSB for debug
 *
 * @param NONE [IN].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Get_TotalInfo( void)
{
	if(NULL == g_demod_hal.VSB_Get_TotalInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Get_TotalInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.VSB_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_VSB_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_VSB_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.VSB_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.VSB_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;



}



#if 0
#endif

/***********************************************************************************
* QAM function
************************************************************************************/


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_SoftwareResetFEC.
 * Reset QAM FEC module.
 *
 * @param [in] NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_SoftwareResetFEC(void) //jeongpil.yun
{
	if(NULL == g_demod_hal.QAM_SoftwareResetFEC)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_SoftwareResetFEC())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Monitor.
 * Monitoring
 *
 * @param  NONE [IN] void .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Monitor(void)
{
	if(NULL == g_demod_hal.QAM_Monitor)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Monitor())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_ModeAutoDetection.
 * Enable/Disable the detection of QAM mode
 *
 * @param  bEnable [IN] - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_ModeAutoDetection(BOOLEAN bEnable)
{
	if(NULL == g_demod_hal.QAM_ModeAutoDetection)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_ModeAutoDetection(bEnable))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_QAM_64Mode.
 * Set the QAM mode to QAM64 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_64Mode(void)  //jeongpil.yun
{
	if(NULL == g_demod_hal.QAM_64Mode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_64Mode())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_256Mode.
 * Set the QAM mode to QAM256 manually
 *
 * @param  NONE [IN/OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_256Mode(void)  //jeongpil.yun
{
	if(NULL == g_demod_hal.QAM_256Mode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_256Mode())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Set_DefaultRegisterValue.
 * Initialize LG1150 GBB for QAM mode.
 *
 * @param  NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Set_DefaultRegisterValue(void)  //jeongpil.yun
{
	if(NULL == g_demod_hal.QAM_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Set_DefaultRegisterValue())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.QAM_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Get_TotalInfo
 * Get all information on QAM for debug
 *
 * @param NONE [IN / OUT].
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Get_TotalInfo( void)
{
	if(NULL == g_demod_hal.QAM_Get_TotalInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Get_TotalInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{


	if(NULL == g_demod_hal.QAM_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.QAM_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_QAM_EQ_Signal_Detector.
 * Disable no signal detector function
 *
 * @param [in] NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_QAM_EQ_Signal_Detector(BOOLEAN OnOff)
{

	if(NULL == g_demod_hal.QAM_EQ_Signal_Detector)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.QAM_EQ_Signal_Detector(OnOff))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}





#if 0
#endif

/***********************************************************************************
* ISDBT function
************************************************************************************/




/**
 * DEMOD_ISDBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Set_DefaultRegisterValue(void)
{
	if(NULL == g_demod_hal.ISDBT_Set_DefaultRegisterValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Set_DefaultRegisterValue())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Set_Config_auto (BOOLEAN bAutoDetect)
{
	if(NULL == g_demod_hal.ISDBT_Set_Config_auto)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Set_Config_auto(bAutoDetect))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when DEMOD_ISDBT_Set_Config_auto is enabled.
 *
 * @param *pIsdbtConfigParam 	[IN] LX_DEMOD_ISDBT_CONFIG_T - ISDBT config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Set_PartialConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam)
{
	if(NULL == g_demod_hal.ISDBT_Set_PartialConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Set_PartialConfig(pIsdbtConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Set_FullConfig
 * Set all config parameters on ISDB-T.
 * This function is used when DEMOD_H13_ISDBT_Set_Config_auto is disabled.
 *
 * @param *pIsdbtConfigParam 	[IN] LX_DEMOD_ISDBT_CONFIG_T - ISDB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Set_FullConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam)
{
	if(NULL == g_demod_hal.ISDBT_Set_FullConfig)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Set_FullConfig(pIsdbtConfigParam))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Set_NeverLockWaitTime(UINT32 waitMs)
{
	if(NULL == g_demod_hal.ISDBT_Set_NeverLockWaitTime)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Set_NeverLockWaitTime(waitMs))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}





/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_DelaySpreadStatus
 * Get delay spread state on ISDB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{
	if(NULL == g_demod_hal.ISDBT_Get_DelaySpreadStatus)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_DelaySpreadStatus(pDelaySpread))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{
	if(NULL == g_demod_hal.ISDBT_Get_FFTMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_FFTMode(pTxModeLock, pFftMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_GuradIntervalMode
 * Get the information of guard interval on ISDB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{
	if(NULL == g_demod_hal.ISDBT_Get_GuradIntervalMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_GuradIntervalMode(pGIMode))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_TMCCInfo
 * Get TPS information on ISDB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_ISDBT_TMCC_CONFIG_T - TMCC data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_TMCCInfo( LX_DEMOD_ISDBT_TMCC_CONFIG_T *pTmccParams)
{
	if(NULL == g_demod_hal.ISDBT_Get_TMCCInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_TMCCInfo(pTmccParams))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_TotalInfo
 * Get all information on ISDB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_TotalInfo( void)
{
	if(NULL == g_demod_hal.ISDBT_Get_TotalInfo)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_TotalInfo())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_IFO_LOCK
 * Get IFO lock on ISDB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{
	if(NULL == g_demod_hal.ISDBT_Get_IFO_LOCK)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_IFO_LOCK(pIfoLock))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on ISDB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{
	if(NULL == g_demod_hal.ISDBT_Get_CochanDetIndicator)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Get_CochanDetIndicator(pDetStatus))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_EqualizereReset(void)
{
	if(NULL == g_demod_hal.ISDBT_EqualizereReset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_EqualizereReset())
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{


	if(NULL == g_demod_hal.ISDBT_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ISDBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.ISDBT_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ISDBT_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}

#if 0
#endif

// ANALOG

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Power_Save
 * Enable/Disable power save mode
 *
 * @param bEnable 	[IN] power save mode(TRUE) / normal mode(FALSE)
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Power_Save(BOOLEAN bEnable)
{

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Power_Save
 * Enable/Disable power save mode
 *
 * @param bEnable 	[IN] power save mode(TRUE) / normal mode(FALSE)
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Demod_Open(UINT32 chipDevID, UINT32 handleI2C, Handle_t *phDevice)
{
    Handle_t pFmHandle = NULL;

	if(NULL == g_demod_hal.ADEMOD_Demod_Open)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Demod_Open(chipDevID, handleI2C, &pFmHandle ))
		DEMOD_RETURN_FAIL(__F__);

	*phDevice = pFmHandle;
	
	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * ADEMOD_Demod_Init
 * Initialize analog demod
 *
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Init(Handle_t phDevice, LX_DEMOD_RF_MODE_T rfMode)
{

	if(NULL == g_demod_hal.ADEMOD_Init)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Init(phDevice, rfMode))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_IF_Frq
 * Set IF frequency registers on DVB-C mode.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Set_IF_Frq(Handle_t phDevice,UINT32 pllClkSource, UINT32 ifFrq)
{
	if(NULL == g_demod_hal.ADEMOD_Set_IF_Frq)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_IF_Frq(phDevice,pllClkSource, ifFrq))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_SoftwareReset
 * Reset GBB module by software.
 *
 * @param  NONE
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_SoftwareReset(Handle_t phDevice)
{
	if(NULL == g_demod_hal.ADEMOD_SoftwareReset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_SoftwareReset(phDevice))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_SoftwareReset
 * Reset GBB module by software.
 *
 * @param  NONE
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_AftRange(Handle_t phDevice, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange)
{
	if(NULL == g_demod_hal.ADEMOD_Set_AftRange)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_AftRange(phDevice,aftRange ))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_SoftwareReset
 * Reset GBB module by software.
 *
 * @param  NONE
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_CvbsDecCtrl(Handle_t phDevice, LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel)
{
	if(NULL == g_demod_hal.ADEMOD_Set_CvbsDecCtrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_CvbsDecCtrl(phDevice,sel ))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_AbbMode
 * Select ABB or GBB mode.
 *
 * @param  bIsAbbMode [IN] BOOLEAN
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Set_AbbMode(BOOLEAN bIsAbbMode)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_AbbMode Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_AbbMode)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_AbbMode(bIsAbbMode))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_AbbMode End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_WorkAround
 * Patch A0/A1 bug related to initialization.
 *
 * @param  None [IN] void
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_WorkAround(void)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_WorkAround Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_WorkAround)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_WorkAround())
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_WorkAround End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_CvbsRateConversion
 * Set CVBS rate converter according to PLL Clock.
 *
 * @param  phDevice [IN] Handle_t
 * @param  pllClkSource [IN] UINT32
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_CvbsRateConversion(Handle_t phDevice, UINT32 pllClkSource)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_CvbsRateConversion Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_CvbsRateConversion)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_CvbsRateConversion(phDevice, pllClkSource))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_CvbsRateConversion End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_HighCvbsRateOffset
 * Set CVBS rate converter offset.
 *
 * @param  phDevice [IN] Handle_t
 * @param  bInitOffsetHigh [IN] BOOLEAN
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_HighCvbsRateOffset(Handle_t phDevice, BOOLEAN bInitOffsetHigh)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_HighCvbsRateOffset Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_HighCvbsRateOffset)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_HighCvbsRateOffset(phDevice, bInitOffsetHigh))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_HighCvbsRateOffset End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_ClampingCtrl
 * Set clamp control parameters.
 *
 * @param  clampCount [IN] UINT8
 * @param  stepsize       [IN] UINT16
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_ClampingCtrl(UINT8 clampCount, UINT16 stepsize)
{
	UINT8 deviceNum = 0 ;

	DEMOD_CTX_T *pDemodInfo;
	Handle_t	phDevice;

	DEMOD_PRINT( "^R^[ABB M14] DEMOD_ANALOG_Set_ClampingCtrl	start..... ^0^\n");

	pDemodInfo = &g_demod_ctx_info[deviceNum];
	phDevice = pDemodInfo->setCfgParam.analog.handleABB;

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_ClampingCtrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_ClampingCtrl(phDevice, clampCount,stepsize ))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_ClampingCtrl End !! \n");

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_SifCtrl
 * Set center frequency of SIF rate converter according to PLL Clock.
 *
 * @param  phDevice [IN] Handle_t
 * @param  pllClkSource [IN] UINT32
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_SifCtrl(Handle_t phDevice, UINT32 pllClkSource)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SifCtrl Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_SifCtrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_SifCtrl(phDevice, pllClkSource))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_SifPathCtrl
 * Set  SIF Path.
 *
 * @param  phDevice [IN] Handle_t
 * @param  pllClkSource [IN] UINT32
 * @param  sel [IN] LX_DEMOD_ANALOG_SIFPATH_SEL_T
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_SifPathCtrl(Handle_t phDevice, UINT32 pllClkSource, LX_DEMOD_ANALOG_SIFPATH_SEL_T sel )
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SifPathCtrl Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_SifPathCtrl)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_SifPathCtrl(phDevice, pllClkSource,sel))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SifPathCtrl End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_SpectrumInv
 *Select spectrum inversion or normal.
 *
 * @param  phDevice [IN] Handle_t
 * @param  bInv [IN] BOOLEAN
 * @param  bforce [IN] BOOLEAN
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_SpectrumInv(Handle_t phDevice, BOOLEAN bInv, BOOLEAN bforce )
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SpectrumInv Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_SpectrumInv)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_SpectrumInv(phDevice, bInv,bforce))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] ADEMOD_Set_SpectrumInv End !! \n");

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ANALOG_Set_SpectrumInv
 *Select spectrum inversion or normal.
 *
 * @param  phDevice [IN] Handle_t
 * @param  audioSystem [IN]  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T
 *  @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_ANALOG_Set_SpecialSifData(Handle_t	phDevice,  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SpecialSifData Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Set_SpecialSifData)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Set_SpecialSifData(phDevice, audioSystem))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Set_SpecialSifData End !! \n");

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{


	if(NULL == g_demod_hal.ADEMOD_Obtaining_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Obtaining_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_ISDBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_ANALOG_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	if(NULL == g_demod_hal.ADEMOD_Monitoring_Signal_Lock)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Monitoring_Signal_Lock(pDemodInfo))
		DEMOD_RETURN_FAIL(__F__);

	return RET_OK;


}

int DEMOD_ANALOG_Dbg_Set_RegisterValue(Handle_t phDevice,  UINT32 RegAddr, UINT32 RegSize, UINT32 RegData)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_Set_RegisterValue Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Dbg_Set_RegValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Dbg_Set_RegValue(phDevice, RegAddr, RegSize, RegData))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_Set_RegisterValue End !! \n");

	return RET_OK;
}

int DEMOD_ANALOG_Dbg_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_SmartTune Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Dbg_Set_SmartTune)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Dbg_Set_SmartTune(pSmartTuneParam))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_SmartTune End !! \n");

	return RET_OK;
}


int DEMOD_ANALOG_Dbg_Get_RegDump(Handle_t phDevice)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Get_RegDump Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Dbg_Get_RegDump)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Dbg_Get_RegDump(phDevice))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Get_RegDump End !! \n");

	return RET_OK;
}


int DEMOD_ANALOG_Dbg_Get_RegisterValue(Handle_t phDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_Get_RegisterValue Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Dbg_Get_RegValue)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Dbg_Get_RegValue(phDevice, RegAddr, RegSize, pRegData))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_Get_RegisterValue End !! \n");

	return RET_OK;
}

int DEMOD_ANALOG_Dbg_TestFunctions(Handle_t hDemod, UINT32 argc, UINT32 *val)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_TestFunctions Start !! \n");

	if(NULL == g_demod_hal.ADEMOD_Dbg_TestFunctions)
		DEMOD_NOT_SUPPORTED(__F__);

	if( RET_OK != g_demod_hal.ADEMOD_Dbg_TestFunctions(hDemod, argc, val))
		DEMOD_RETURN_FAIL(__F__);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ANALOG_Dbg_TestFunctions End !! \n");

	return RET_OK;
}

/** @} */

