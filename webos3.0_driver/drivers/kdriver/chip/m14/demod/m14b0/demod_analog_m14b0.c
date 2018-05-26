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

/*****************************************************************************
**
**  Name:demod_analog_m14a0.c
**
**  Description:    ABB tuning process control and lock status monitoring.
**
**  Functions
**  Implemented:   char*	DEMOD_M14B0_ANALOG_WhatAudioSystem
**                  int DEMOD_M14B0_ANALOG_Demod_Open
**                  int DEMOD_M14B0_ANALOG_Demod_Open
**                  int DEMOD_M14B0_ANALOG_Set_IF_Frq
**                  int DEMOD_M14B0_ANALOG_Set_AftRange
**                  int DEMOD_M14B0_ANALOG_ChangeAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetLowIFinput
**                  LX_ADEMOD_VideoStandard DEMOD_M14B0_ANALOG_GetVideoStandard
**                  int DEMOD_M14B0_ANALOG_SetSIFMode
**                  int DEMOD_M14B0_ANALOG_SetAGCTOP
**                  int DEMOD_M14B0_ANALOG_SmartTune
**                  int DEMOD_M14B0_ANALOG_CheckVideoCarrier
**                  int DEMOD_M14B0_ANALOG_CheckVideoSync
**                  int DEMOD_M14B0_ANALOG_ControlOutput
**                  int DEMOD_M14B0_ANALOG_Get_FreqOffset
**                  int DEMOD_M14B0_ANALOG_ChangeFilter4SIF
**                  int DEMOD_M14B0_ANALOG_CarrierlockScan
**                  int DEMOD_M14B0_ANALOG_SoftReset
**                  int DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl
**                  int DEMOD_M14B0_ANALOG_Set_CvbsRateConversion
**                  int DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset
**                  int DEMOD_M14B0_ANALOG_Set_SifCtrl
**                  int DEMOD_M14B0_ANALOG_Set_SifPathCtrl
**                  int DEMOD_M14B0_ANALOG_Set_SpectrumInv
**                  int DEMOD_M14B0_ANALOG_Obtaining_Signal_Lock
**                  int DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock
**
**                  int DEMOD_M14B0_ANALOG_Dbg_Get_RegDump
**                  int DEMOD_M14B0_ANALOG_Dbg_Set_RegisterValue
**                  int DEMOD_M14B0_ANALOG_Dbg_SmartTune
**                  int DEMOD_M14B0_ANALOG_Dbg_Get_RegisterValue
**                  int DEMOD_M14B0_ANALOG_Dbg_TestFunctions
**                  int DEMOD_M14B0_ANALOG_OptimizingTunerInBrazil
**                  int DEMOD_M14B0_ANALOG_OptimizingTunerInBrazil
**
**  References:    
**
**  Exports:       
**
**  Dependencies:   demod_impl.h for system configuration data.
**				 demod_analog_m14a0.h
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/div64.h> //do_div
	 
#include "sys_regs.h"
	 
#include "demod_impl.h"
#include "demod_analog_m14b0.h"


/**
 * Name Table for Audio System.
*/
static const LX_DEMOD_ANALOG_NAME_AUDIO_SYSTEM_t NAME_AUDIO_SYSTEM[] =
{
	{LX_DEMOD_ANALOG_SIF_SYSTEM_BG,			"AUD_SYS_BG"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_I,			"AUD_SYS_I"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_DK,			"AUD_SYS_DK"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_L,			"AUD_SYS_L"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_MN,			"AUD_SYS_M"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_LP,			"AUD_SYS_LP"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_END,		"AUD_SYS_END"},

	{LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN,	"AUD_SYS_UNKNOWN"},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN,	NULL},
	{LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN,	"AUD_SYS_ERROR"}
};

static  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T 	gPreviousAudio	 = 	LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

//static	UINT32	_glevelSIF[] 		= { 320, 400, 530, 800 };
static	UINT32	_gCurrentSamplRate_b0	= LX_ADEMOD_SAMPLING_RATE_240MHz;
static	BOOLEAN	_gbSIFFilterMode_b0	= FALSE;

extern BOOLEAN _gSpectrumInv;

extern	unsigned int g_NewTuneReqeustTime;
extern	unsigned int g_RetuneStartTime;
extern	UINT64 pauseTimeDemod ;
extern	UINT32 notifyinglockTimeOut;

#if 0
#endif

/**
 * return audio system.
 *
 * @param	audioSystem [in] audio system
 * @return	audio system
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
*/
char*	DEMOD_M14B0_ANALOG_WhatAudioSystem(LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem)
{
	UINT8	index = 0;

	while(NULL != NAME_AUDIO_SYSTEM[index].pszName)
	{
		if (audioSystem == NAME_AUDIO_SYSTEM[index].audioSystem)
			return NAME_AUDIO_SYSTEM[index].pszName;

		index++;
	}

	index++;

	return NAME_AUDIO_SYSTEM[index].pszName;
}

/**
 * DEMOD_M14B0_ANALOG_Demod_Open.
 *
 * @param	chipDevID	[in] UINT32
 * @param	handleI2C	[in] UINT32
 * @param	*phDevice	[OUT] Handle_t
 * @return	OK/FAIL
 * @author	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_Demod_Open(UINT32 chipDevID, UINT32 handleI2C,  Handle_t *phDevice)
{
	Handle_t pFmHandle = NULL;
	
	if (retOK != DEMOD_M14_Analog_Demod_Open(chipDevID, &handleI2C, sizeof(handleI2C), &pFmHandle))
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Demod_Open  !!!\n",  __F__, __L__);

		return RET_ERROR;
	}
	*phDevice = pFmHandle;
	
	return RET_OK;
}

/**
 * DEMOD_M14B0_ANALOG_Init.
 *
 * @param	phDevice	[in] UINT32
 * @return	OK/FAIL
 * @author	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_Init(Handle_t phDevice, LX_DEMOD_RF_MODE_T rfMode)
{

	int ifFreq;
	UINT32 pllClkSource;

	if(rfMode == LX_DEMOD_NTSC)
	{
		ifFreq = -1 * LX_DEMOD_IF_FREQ_06_000_KHZ;
		pllClkSource = LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ;
	}
	else if((rfMode == LX_DEMOD_PAL)|| (rfMode == LX_DEMOD_SECAM))
	{
		ifFreq = -1 * LX_DEMOD_IF_FREQ_06_000_KHZ;
		pllClkSource = LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ;

	}
	else
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Init - Invaild rf mode !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);

	}
	if (retOK != DEMOD_M14B0_ANALOG_SetLowIFinput(phDevice, pllClkSource, ifFreq, 1))
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) DEMOD_M14B0_ANALOG_SetLowIFinput  !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}



	return RET_OK;
}


/**
 * DEMOD_M14B0_ANALOG_Init.
 *
 * @param	phDevice	[in] UINT32
 * @return	OK/FAIL
 * @author	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_Set_IF_Frq(Handle_t phDevice, UINT32 pllClkSource, UINT32 ifFrq)
{
	int ifFreq;

	ifFreq = -1 * (int)ifFrq;

	switch(pllClkSource)
	{
		case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x25e4))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x26e1))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;
			
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x26e3))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x26e4))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x26e5))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;

			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x27e3))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;

		case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x27e5))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;

		default :			
			DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild symbol rate, going to default setting(27MHz) !!!\n",  __F__, __L__);
			if (retOK != ADEMOD_Register_Set(phDevice, 0x452, 2, 0x26e3))
			{
				DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) ADEMOD_Register_Set !!!\n",  __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			break;
	}	

	if (retOK != DEMOD_M14B0_ANALOG_Set_CvbsRateConversion(phDevice, pllClkSource))
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Set_CvbsRateConversion !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if (retOK != DEMOD_M14B0_ANALOG_Set_SifPathCtrl(phDevice, pllClkSource, LX_DEMOD_ANALOG_SIF_PATH_INTERNAL ))
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Set_SifPathCtrl !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	

	if (retOK != DEMOD_M14B0_ANALOG_SetLowIFinput(phDevice, pllClkSource, ifFreq, 1))
	{
		DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) DEMOD_M14B0_ANALOG_SetLowIFinput !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	DEMOD_PRINT("(%s:%d) pllClkSource = %d,ifFreq = %d !!!\n", __F__, __L__, pllClkSource,ifFreq);

	return RET_OK;
}

/**
 * DEMOD_M14B0_ANALOG_Set_AftRange.
 *
 * @param	phDevice	[in] UINT32
 * @return	OK/FAIL
 * @author	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_Set_AftRange(Handle_t phDevice, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange)
{
	UINT32 rangeValue[3];

	rangeValue[0] = 0; // direct setting
	rangeValue[1] = aftRange.lo;
	rangeValue[2] = aftRange.hi;

	ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_VIDCR_RANGE, rangeValue, TRUE);
#if 1
	ADEMOD_Register_SetX(phDevice, 0x293, 3, 0x1fc0, TRUE); //need to be modfied
#else
	thresValue[0] = LX_ADEMOD_PRESET_OPTION_NONE;
	thresValue[1] = 300;
	ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_VIDCR_LOCKDET, thresValue, TRUE);
#endif


	return RET_OK;
}




#if 0
#endif
/**
 * tune.
 *
 * @param	*pSmartTuneParam 	[in] LX_DEMOD_SMARTTUNE_PARAM_T
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
 int DEMOD_M14B0_ANALOG_ChangeAudioSystem(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam)
{
	LX_ADEMOD_VideoStandard	videoStd 	= LX_ADEMOD_VIDEO_STANDARD_NONE;
	Handle_t 				phDevice;
	UINT32 					frequency;
	LX_DEMOD_TUNE_MODE_T	tuneMode;
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	phDevice	= pSmartTuneParam->phDevice;
	frequency	= pSmartTuneParam->frequency;
	tuneMode	= pSmartTuneParam->tuneMode;
	audioSystem = pSmartTuneParam->audioSystem;

	videoStd	= DEMOD_M14B0_ANALOG_GetVideoStandard(audioSystem);


	// Execute System Init preset
	if (retOK != ADEMOD_Preset_Set(phDevice, NULL, LX_ADEMOD_PRESET_SYSTEM_INIT))
	{
		DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) ADEMOD_Preset_Set !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	DEMOD_M14B0_ANALOG_SmartTune(pSmartTuneParam, TRUE);

	ADEMOD_SetGroupDelay(phDevice, videoStd);

	DEMOD_PRINT( "[ABB M14] Set %s !!!\n",  DEMOD_M14B0_ANALOG_WhatAudioSystem(audioSystem));

	_gbSIFFilterMode_b0 = FALSE;

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return RET_OK;
}


/**
 * tune.
 *
 * @param	*pSmartTuneParam [in] LX_DEMOD_SMARTTUNE_PARAM_T
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
 int DEMOD_M14B0_ANALOG_SetAudioSystem(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam)
{

	Handle_t 							phDevice = NULL;
	LX_DEMOD_TUNE_MODE_T				tuneMode = LX_DEMOD_TUNE_UNKNOWN;
	LX_DEMOD_RF_MODE_T					rfMode = LX_DEMOD_UNKOWN_MODE;
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T 	nextAudioSystem = LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

	phDevice 		= pSmartTuneParam->phDevice;
	rfMode 			= pSmartTuneParam->rfMode;
	tuneMode		= pSmartTuneParam->tuneMode;
	nextAudioSystem	= pSmartTuneParam->audioSystem;

    DEMOD_PRINT( "[ABB M14](%s:%d) !!!\n",__F__,__L__);

    DEMOD_PRINT( "[ABB M14](%s:%d) tune mode = %d !!!\n",__F__,__L__,tuneMode);

	DEMOD_PRINT( "[ABB M14] gPreviousAudio = %d, nextAudioSystem = %d !!!\n", gPreviousAudio,nextAudioSystem);

	if (LX_DEMOD_NTSC == pSmartTuneParam->rfMode)
	{
		if (gPreviousAudio == nextAudioSystem)
		{
			DEMOD_PRINT( "[ABB M14] Same %s : Only SmartTune() !!!\n",  DEMOD_M14B0_ANALOG_WhatAudioSystem(nextAudioSystem));
			DEMOD_M14B0_ANALOG_SmartTune( pSmartTuneParam, TRUE);
		}
		else
		{
			DEMOD_M14B0_ANALOG_ChangeAudioSystem( pSmartTuneParam);
		}

//		DEMOD_M14B0_ANALOG_SetSIFMode(phDevice);
		DEMOD_M14B0_ANALOG_SetAGCTOP(phDevice, pSmartTuneParam->frequency,LX_DEMOD_NTSC);

		gPreviousAudio = nextAudioSystem;

		return RET_OK;
	}
	else if (LX_DEMOD_PAL == rfMode)
	{

		if (LX_DEMOD_TUNE_AUTO_START == tuneMode)
		{
			DEMOD_PRINT( "[ABB M14] ** LX_DEMOD_TUNE_AUTO_START **** !!!\n");
			if (!((LX_DEMOD_ANALOG_SIF_SYSTEM_L == nextAudioSystem) || (LX_DEMOD_ANALOG_SIF_SYSTEM_LP == nextAudioSystem)))
			{
				nextAudioSystem = LX_DEMOD_ANALOG_SIF_SYSTEM_DK;
				DEMOD_PRINT( "[ABB M14] Auto Start : Not NTSC, Not L(P) ==> Force to DK !!!\n");
			}

			DEMOD_M14B0_ANALOG_ChangeAudioSystem( pSmartTuneParam);

		}

		else if ((_gbSIFFilterMode_b0) || (LX_DEMOD_TUNE_MANUAL == tuneMode))
		{
			DEMOD_M14B0_ANALOG_ChangeAudioSystem( pSmartTuneParam);
		}
		else
		{
			if (gPreviousAudio == nextAudioSystem)
			{
				DEMOD_PRINT( "[ABB M14] Same %s : Only SmartTune() !!!\n",	DEMOD_M14B0_ANALOG_WhatAudioSystem(nextAudioSystem));
				DEMOD_M14B0_ANALOG_SmartTune( pSmartTuneParam, TRUE);
			}
			else
			{
				DEMOD_M14B0_ANALOG_ChangeAudioSystem( pSmartTuneParam);
			}
		}


//		DEMOD_M14B0_ANALOG_SetSIFMode(phDevice);
		DEMOD_M14B0_ANALOG_SetAGCTOP(phDevice, pSmartTuneParam->frequency, LX_DEMOD_PAL);

		gPreviousAudio = nextAudioSystem;
	}
	return RET_OK;
}

/**
 * read IF Level.
 *
 * @param	phDevice	[in] 	Handle_t
 * @param	*pIFLevel	[out] 	IF Level - UINT32
 * @return	TRUE/FALSE
 * @author
*/
 int DEMOD_M14B0_ANALOG_CheckIFLevel(Handle_t phDevice,  UINT32 *pIFLevel)
{

	UINT32 aReadData[2] = { 0 };

    DEMOD_PRINT( "[TU ATV](%s:%d) !!!\n",__F__,__L__);


    if (retOK != ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_IF_LEVEL_STAT, &aReadData[0]))
    {
        DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) ADEMOD_Parameter_Get(LXDEMOD_M14B0_ANALOG_PARM_IF_LEVEL_STAT)\n",  __F__, __L__);
		*pIFLevel = 0;
		DEMOD_RETURN_FAIL(__F__);
    }

	*pIFLevel = aReadData[0];

	return RET_OK;

}



int DEMOD_M14B0_ANALOG_SetLowIFinput(Handle_t phDevice, UINT32 pllClkSource, int lif_freq, UINT8 isdefault)
{


	UINT64 mix_freq=0;
	UINT16 intmix_freq=0;
	UINT32 addr=0x0A71;

	UINT64 calcSampleRate;

	if(pllClkSource > 26000)
	{
		calcSampleRate = pllClkSource*10;
		do_div(calcSampleRate, 8);
		mix_freq=(UINT64)(36000- calcSampleRate - (lif_freq));
		mix_freq = (mix_freq << 16);
		do_div(mix_freq, (pllClkSource));
		mix_freq = mix_freq * 10 + 5;
		do_div(mix_freq, 10);

	DEMOD_PRINT("calcSampleRate = %lld, mix_freq = %lld \n",calcSampleRate,mix_freq );

	}
	else
	{
		calcSampleRate = pllClkSource*10;
		do_div(calcSampleRate, 6);
		mix_freq=(UINT64)(44000- calcSampleRate - (lif_freq));
		mix_freq = (mix_freq << 16) * 10;
		do_div(mix_freq, (pllClkSource * 10));
		mix_freq = mix_freq * 10 + 5;
		do_div(mix_freq, 10);
	}
	DEMOD_PRINT("calcSampleRate = %lld, mix_freq = %lld \n",calcSampleRate,mix_freq );

	intmix_freq = (UINT16)mix_freq + 1;

	if(retOK !=ADEMOD_Register_Set(phDevice, addr, 2, intmix_freq))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	DEMOD_PRINT(", addr = 0x%x, intmix_freq = %d(0x%x) (mix_freq = 0x%llx)  \n",addr,intmix_freq, intmix_freq,mix_freq);

	return RET_OK;

}

/**
 * get LX_ADEMOD_VideoStandard.
 *
 * @param	audioSystem	[in] audioSystem
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
LX_ADEMOD_VideoStandard DEMOD_M14B0_ANALOG_GetVideoStandard(LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem)
{
	switch(audioSystem)
	{
		case LX_DEMOD_ANALOG_SIF_SYSTEM_BG:
			return LX_ADEMOD_VIDEO_STANDARD_PAL_B;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_I:
			return LX_ADEMOD_VIDEO_STANDARD_PAL_I;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_DK:
			return LX_ADEMOD_VIDEO_STANDARD_PAL_D;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_L:
			return LX_ADEMOD_VIDEO_STANDARD_SECAM_L;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_MN:
			return LX_ADEMOD_VIDEO_STANDARD_NTSC_M;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_LP:
			return LX_ADEMOD_VIDEO_STANDARD_SECAM_L1;
//			return LX_ADEMOD_VIDEO_STANDARD_SECAM_L;

			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_END:
		case LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN:
		default:
		break;
	}

	return LX_ADEMOD_VIDEO_STANDARD_PAL_B;
}


/**
 * tune.
 *
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_SetSIFMode(Handle_t phDevice)
{
	UINT32 data[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_MODE)];

    DEMOD_PRINT( "[ABB M14](%s:%d) !!!\n",__F__,__L__);


	data[0] = LX_ADEMOD_AUDIO_MODE_SIF;

	if (retOK != ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_AUD_MODE, data, TRUE))
	{
		DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) ADEMOD_Parameter_Set(LX_ADEMOD_PARM_AUD_MODE) !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


	return RET_OK;
}


/**
 * tune.
 *
 * @param	phDevice [in] Handle_t
 * @param	frequency [in] UINT32
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_SetAGCTOP(Handle_t phDevice, UINT32 frequency, LX_DEMOD_RF_MODE_T rfMode )
{

	UINT32		data[2];

    DEMOD_PRINT( "[ABB M14](%s:%d) !!!\n",__F__,__L__);


	data[0] = 0;
	if(rfMode == LX_DEMOD_NTSC)
		data[1] = 160;
	else
		data[1] = 160;

	if (retOK != ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_AGC_BACKOFF, data, TRUE))
	{
		DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) ADEMOD_Parameter_Set(LX_ADEMOD_PARM_AGC_BACKOFF) !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	DEMOD_PRINT( "[M14  ATV] ABB : AGC TOP %d dB  !!!\n",  data[1]/10);
	return RET_OK;
}


/**
 * tune.
 *
 * @param	audioSystem	[in] audioSystem
 * @param	bForce		[in] TRUE/FALSE
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam,  BOOLEAN bForce)
{

	LX_ADEMOD_SmartTuneDescriptor 		smartTune;
	LX_ADEMOD_VideoStandard				videoStd 	= LX_ADEMOD_VIDEO_STANDARD_NONE;
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T 	audioSystem;

	Handle_t 							phDevice;
	LX_DEMOD_TUNE_MODE_T				tuneMode;
	LX_DEMOD_RF_MODE_T					rfMode;


	DEMOD_PRINT( "[ABB M14] _____(%s(%c))_____\n",  __F__, bForce ? 'T' : 'F');

	phDevice	= pSmartTuneParam->phDevice;
	tuneMode	= pSmartTuneParam->tuneMode;
	audioSystem = pSmartTuneParam->audioSystem;
	rfMode 		= pSmartTuneParam->rfMode;

	videoStd	= DEMOD_M14B0_ANALOG_GetVideoStandard(audioSystem);

	if(gPreviousAudio != audioSystem)
	{
		bForce			= TRUE;
	}

	if (LX_ADEMOD_VIDEO_STANDARD_NONE == videoStd)
	{
		DEMOD_PRINT( "[ABB M14] ERR (%s:%d) INVALID audio system(%s) !!!\n",  __F__, __L__, DEMOD_M14B0_ANALOG_WhatAudioSystem(audioSystem));
		DEMOD_RETURN_FAIL(__F__);
	}


	DEMOD_PRINT(" === VideoStd => %x ====\n",videoStd);
	DEMOD_PRINT(" === (LX_ADEMOD_SignalSourceAnalog << 28) + videoStd => %x ====\n",(LX_ADEMOD_SignalSourceAnalog << 28) + videoStd);
	smartTune.Filter	= (LX_ADEMOD_SignalSourceAnalog << 28) + videoStd;
	smartTune.Freq		= pSmartTuneParam->frequency * 1000; // unit : Hz
	smartTune.SmplRate	= pSmartTuneParam->sampleRate; //LX_ADEMOD_SAMPLING_RATE_240MHz; //_gCurrentSamplRate_b0;
	smartTune.IF		= pSmartTuneParam->analogIF * 1000;	// unit : Hz
	smartTune.Update	= 1;
	DEMOD_PRINT("[ABB M14](%s) smartTune.IF %d Hz !!!\n",  __F__, smartTune.IF);
	DEMOD_PRINT("[ABB M14](%s) smartTune.SmplRate %d Hz !!!\n", __F__, smartTune.SmplRate);
	DEMOD_PRINT("[ABB M14](%s) smartTune.Filter %d  !!!\n", __F__, smartTune.Filter);
	DEMOD_PRINT("[ABB M14](%s) smartTune.Freq %d Hz !!!\n",  __F__, smartTune.Freq);

#if 0
	if(LX_DEMOD_IS_TUNE_SCAN(tuneMode))
	{
		DEMOD_PRINT("ssssssssssssssssssssssssssssssssssss !!!\n");
		if (retOK != ADEMOD_Demodulator_Set_ChannelScan(phDevice, &smartTune))
		{
			DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) ADEMOD_Demodulator_Set_ChannelScan !!!\n",  __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		// Mstar BCM 공통 적용으로 수정. 101130
		if((tuneMode == LX_DEMOD_TUNE_AUTO_START) && (rfMode == LX_DEMOD_NTSC))
		{
				UINT32 value;
				value = 4500000L;
				DEMOD_PRINT( "[ABB M14] Set to BW 4.5Mhz !!!\n");
				ADEMOD_Parameter_Set(phDevice,LX_ADEMOD_PARM_VID_BNDW,&value,TRUE);
		}

	}
	else
#endif
	{
		// Execute Video Mode preset
		if (retOK != ADEMOD_Preset_Set(phDevice, &smartTune, LX_ADEMOD_PRESET_MODE))
		{
			DEMOD_PRINT( "[ABB M14]] FAIL (%s:%d) ADEMOD_Preset_Set !!!\n",  __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}


		if (retOK != ADEMOD_Channel_Set(phDevice, &smartTune, bForce))
		{
			DEMOD_PRINT( "[ABB M14]] FAIL (%s:%d) ADEMOD_Channel_Set !!!\n",  __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		_gCurrentSamplRate_b0 = smartTune.SmplRate;
	}
	return RET_OK;
}


/**
 * Check Tuner Sync Lock State.
 *
 * @param	phDevice [IN] Handle
 * @param	pCarrierLockStatus [OUT] LX_DEMOD_LOCK_STATE_T
 * @return	TRUE/FALSE
 * @author 	KangDongHyun(Ronin@lge.com)
 */
int DEMOD_M14B0_ANALOG_CheckVideoCarrier(Handle_t phDevice, LX_DEMOD_LOCK_STATE_T *pCarrierLockStatus)
{
	UINT32	lockState[2] = {0};

    DEMOD_PRINT( "[ABB M14](%s:%d) !!!\n",__F__,__L__);

	if (retOK != ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_LOCK_STAT, &lockState[0]))
	{
		DEMOD_PRINT( "[ABB M14]] FAIL (%s:%d) ADEMOD_Parameter_Get !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

//	DEMOD_PRINT( "[ABB M14] ABB CarrierCheck %d\n", lockState[0]);

	if(lockState[0]) // H/V Sync Lock
	{
		*pCarrierLockStatus = LX_DEMOD_LOCKED;
		DEMOD_PRINT( "^g^[M14  ABB] (%s:%d) ABB Carrier Lock [OK]\n",  __F__, __L__);
	}
	else
	{
		*pCarrierLockStatus = LX_DEMOD_UNLOCKED;
		DEMOD_PRINT( "^c^[M14  ABB] (%s:%d) ABB Carrier Lock [NG]\n",  __F__, __L__);
	}

	return RET_OK;
}

/**
 * CheckVideoSync
 *
 * @param	phDevice		[in]	Handle_t
 * @param	pLockState	[in/out]	LX_DEMOD_LOCK_STATE_T
 * @return	int
 * @author 	kyungdug.hong(kyungdug.hong@lge.com)
 */
static int DEMOD_M14B0_ANALOG_CheckVideoSync(Handle_t phDevice, LX_DEMOD_LOCK_STATE_T *pLockState)
{
	UINT32 	stat;


	ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_SYNC_STAT, &stat);
	if (retOK != ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_SYNC_STAT, &stat))
	{
		DEMOD_PRINT( "[ABB M14]] FAIL (%s:%d) ADEMOD_Parameter_Get !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

//	DEMOD_PRINT("H-sync status: %s,%s\n", (stat & 0x1) ? "locked" : "unlocked", (stat & 0x4) ? "stable" : "unstable");
//	DEMOD_PRINT("V-sync status: %s,%s\n", (stat & 0x2) ? "locked" : "unlocked", (stat & 0x8) ? "stable" : "unstable");

	if(!(stat & 0x1) && !(stat & 0x2))
	{
		DEMOD_PRINT( "^b^[M14 ] (%s:%d) ADEMOD UNLOCKED\n",  __F__, __L__);
		*pLockState = LX_DEMOD_UNLOCKED;
	}
	else
	{
		DEMOD_PRINT( "^g^[M14 ] (%s:%d) ADEMOD LOCKED\n",  __F__, __L__);
		*pLockState = LX_DEMOD_LOCKED;
	}

	return RET_OK;

}


/**
 * enable output.
 *
 * @param	phDevice 	[in] Handle_t
 * @param	bOutputON 	[in] output enable/disable
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_ControlOutput(Handle_t phDevice,  BOOLEAN bOutputON)
{
	/* Check */
	return RET_OK;
}

/**
 * check frequency offset.
 *
 * @param	*pDemodInfo 	[in] 	DEMOD_CTX_T
 * @param	*pFreqOffset 	[out] 	frequency offset
 * @return	RET_OK, RET_NOT_OK
 * @author 	jeongpil.yun@lge.com
 */
int DEMOD_M14B0_ANALOG_Get_FreqOffset(DEMOD_CTX_T *pDemodInfo, SINT32 *pFreqOffset)
{

	Handle_t 				phDevice;

	UINT8					count = 0;

	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T 	audioSystem;
	LX_DEMOD_LOCK_STATE_T 				carrierLockStatus;


    DEMOD_PRINT( "[TU ATV](%s:%d) !!!\n",__F__,__L__);

	phDevice = (Handle_t)(pDemodInfo->setCfgParam.analog.handleABB);
	audioSystem = pDemodInfo->setCfgParam.analog.audioSystem;

	*pFreqOffset = 0; // All related to AFT is controlled by Tuner 2158B  

	for(count = 0; count < 10; count++ )
	{
		if (RET_OK != DEMOD_M14B0_ANALOG_CheckVideoCarrier(phDevice, &carrierLockStatus))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoCarrier() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		DEMOD_PRINT("[ABB M14](%s:%d) carrierLockStatus = %d,count = %d !!!\n", __F__, __L__,carrierLockStatus,count);
		if(LX_DEMOD_LOCKED == carrierLockStatus) break;

		OS_MsecSleep(20);
	}
	if(count < 8)
	{
		ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_CARR_OFFS, (UINT32*)pFreqOffset);

		if (RET_OK != ADEMOD_Parameter_Get(phDevice, LX_ADEMOD_PARM_CARR_OFFS, (UINT32*)pFreqOffset))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Parameter_Get() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(audioSystem == LX_DEMOD_ANALOG_SIF_SYSTEM_LP)
		{
			*pFreqOffset = -(*pFreqOffset);

			if((*pFreqOffset > 2100) ||(*pFreqOffset < -1300))
			{
				*pFreqOffset = 0;
			}
		}
		else
		{
			if((*pFreqOffset > 2100) ||(*pFreqOffset < -2100))
			{
				*pFreqOffset = 0;
			}

		}

		*pFreqOffset = 0; // All related to AFT is controlled by Tuner 2158B  
		DEMOD_PRINT( "^P^[T_  ATV] Offset %d\n", *pFreqOffset);

	}
	else
	{
		*pFreqOffset = 0;
		DEMOD_PRINT( "^P^[T_  ATV] Offset %d, count = %d\n", *pFreqOffset, count);

	}

	return RET_OK;
}



/**
 * tune.
 *
 * @param	phDevice 	[in] Handle_t
 * @param	audioSystem	[in] LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T
 * @return	OK/FAIL
 * @author 	Jeongpil Yun (jeongpil.yun@lge.com)
 */
int DEMOD_M14B0_ANALOG_ChangeFilter4SIF(Handle_t	phDevice,  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem)
{


	UINT32				videoBW[1]= {0};
	UINT32				dataFreq[2];

    DEMOD_PRINT( "[TU ATV](%s:%d) !!!\n",__F__,__L__);
    printk( "[TU ATV](%s:%d) audioSystem = %d!!!\n",__F__,__L__,audioSystem);


	switch(audioSystem)
	{
		case LX_DEMOD_ANALOG_SIF_SYSTEM_BG:
			videoBW[0]	=  5350000;
			dataFreq[0]	= 5425000;
			dataFreq[1] = 6119999;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_I:
			videoBW[0]	=  5350000;
			dataFreq[0]	= 5925000; //5580000;
			dataFreq[1] = 6936000; //6575000;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_DK:
			videoBW[0]	= 5750000;
			dataFreq[0]	= 5580000; //5580000;    
			dataFreq[1] = 6817000; //6817000;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_MN:
			videoBW[0]	= 4500000;
			dataFreq[0]	= 4330000;
			dataFreq[1] = 4799000;
			break;

		case LX_DEMOD_ANALOG_SIF_SYSTEM_L: 
		case LX_DEMOD_ANALOG_SIF_SYSTEM_LP: 
			videoBW[0]	=  5555000;
			dataFreq[0]	= 5580000; //5580000;
			dataFreq[1] = 6575000; //6817000;
			break;

		default:
			DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) INVALID %s !!!\n",  __F__, __L__, DEMOD_M14B0_ANALOG_WhatAudioSystem(audioSystem));
			DEMOD_RETURN_FAIL(__F__);
			break;
	}

	if (retOK != ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_AUD_SIF_FRQ, dataFreq, TRUE))
	{
		DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) ADEMOD_Parameter_Set(LX_ADEMOD_PARM_AUD_SIF_FRQ) !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	
#if 1
	if (retOK != ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_VID_BNDW, videoBW, TRUE))
	{
		DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) ADEMOD_Parameter_Set(LX_ADEMOD_PARM_VID_BNDW) !!!\n",  __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	
#endif

	DEMOD_PRINT( "[M14 ABB] _ADEMOD_ChangeFilter4SIF(%s) : VIDEO BW(%d Hz), %d Hz ~ %d Hz !!!\n",
				DEMOD_M14B0_ANALOG_WhatAudioSystem(audioSystem), videoBW[0], dataFreq[0], dataFreq[1]);


	_gbSIFFilterMode_b0	= TRUE;

	return RET_OK;
}


/**
 * DEMOD_M14B0_ANALOG_CarrierlockScan
 * Get the carrier lock flag indicator.
 *
 * @param	*pDemodInfo			[IN] DEMOD_CTX_T.
 * @param	ResetOn				[IN] 	LX_DEMOD_FLAG_T  - force to initialize the postjob.
 * @param	*pScanProcessing		[OUT] LX_DEMOD_FLAG_T- the flag that scan is done or not done.
 * @param	*pScanLock			[OUT] LX_DEMOD_LOCK_STATE_T - scan lock result.
 * @return if Succeeded - RET_OK else-TU_ERROR
 * @author 	Jeongpil.yun(jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_CarrierlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock)
{

	Handle_t 				phDevice;

	unsigned int 			elapsedTime = 0;
	static	unsigned int	_neverLockResetTime	= 0;

	static unsigned int 	_maxTimeOut = 0;


	LX_DEMOD_LOCK_STATE_T carrierLockStatus;
	LX_DEMOD_RF_MODE_T scanMode;


	scanMode = pDemodInfo->systemMode;
	phDevice = (Handle_t)(pDemodInfo->setCfgParam.analog.handleABB);

	if(LX_DEMOD_FLAG_ON == ResetOn)
	{

		*pScanProcessing = LX_DEMOD_FLAG_ING;
		*pScanLock	= LX_DEMOD_UNLOCKED;
		_maxTimeOut = LX_DEMOD_LOCK_DEALY_150;


		_neverLockResetTime = jiffies_to_msecs(jiffies);

		return RET_OK;
	}


	if (RET_OK != DEMOD_M14B0_ANALOG_CheckVideoCarrier(phDevice, &carrierLockStatus))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoCarrier() !!!\n", __F__, __L__);
		*pScanLock	= LX_DEMOD_UNLOCKED;
		DEMOD_RETURN_FAIL(__F__);
	}

	if (carrierLockStatus != LX_DEMOD_LOCKED)
	{
			elapsedTime = jiffies_to_msecs(jiffies) - _neverLockResetTime;

			if(elapsedTime > _maxTimeOut)
			{
				*pScanProcessing	= LX_DEMOD_FLAG_FINISH;
				*pScanLock			= LX_DEMOD_UNLOCKED;
				DEMOD_PRINT( "[ABB M14] Carrier lock : LX_DEMOD_UNLOCKED \n");

			}
	}
	else
	{
		*pScanProcessing	= LX_DEMOD_FLAG_FINISH;
		*pScanLock			= LX_DEMOD_LOCKED;
		DEMOD_PRINT( "[ABB M14] NEVERSCAN :  LX_DEMOD_NEVERLOCK DETECTED	!! \n");


	}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_SoftReset.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_SoftReset(Handle_t	phDevice)
{

	ADEMOD_Register_Set(phDevice, 0x404, 1, 0x0);
	OS_MsecSleep(10);
	ADEMOD_Register_Set(phDevice, 0x404, 1, 0xc1);
	OS_MsecSleep(5);
	ADEMOD_Register_Set(phDevice, 0x404, 1, 0xc3);
	OS_MsecSleep(5);
	ADEMOD_Register_Set(phDevice, 0x404, 1, 0xc7);
	OS_MsecSleep(5);
	ADEMOD_Register_Set(phDevice, 0x404, 1, 0xdf);

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_CvbsRateConversion.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	bActive 	[in] 	BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_CvbsRateConversion(Handle_t	phDevice, UINT32 pllClkSource)
{
	UINT32 prevData = 0;

	if(retOK != ADEMOD_Register_Set(phDevice, 0xb01, 1, 0xa))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb01, 0xa) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb02, 1, 0xa))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb02, 0xa) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

#if 1
	if(retOK != ADEMOD_Register_Get(phDevice, 0xb46, 2, &prevData))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x2) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, 0xf))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb61, 2, 0xf))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb61, 0x4) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, prevData))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	
#endif



	switch(pllClkSource)
	{
		case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
#if 1
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0xcc);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0xcc);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0xcc);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0xcc);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0xcc);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x8c);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
#else
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0x80);	
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0xcc);	
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0xf0);	
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0xcc);	
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0xcc);	
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x8c);	
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x0);

#endif
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0xC4);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x20);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0xB0);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0x72);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0x68);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x91);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
			break;
			
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x0);
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0xd4);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x41);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0x1d);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0xd4);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0x41);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x9d);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0x97);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x0a);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0x5a);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0x7d);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0xa9);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0xa0);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
			break;

			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0xa6);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x01);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0x6d);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0x1a);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0xd0);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0xa6);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
			break;

		case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0xac);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x7d);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0xc3);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0xda);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0x37);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0xac);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x00);
			break;

		default :			
			DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild sample rate, going to default setting(27MHz) !!!\n",  __F__, __L__);
			ADEMOD_Register_Set(phDevice, 0xb10, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb11, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb12, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb13, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb14, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb15, 1, 0x99);
			ADEMOD_Register_Set(phDevice, 0xb16, 1, 0x0);
			break;
	}	

#if 0
	for(i=0;i < 7;i++)	
	{
		ADEMOD_Register_Get(phDevice, (reg + i), 1, &data);
//		DEMOD_PRINT("reg = 0x%x, data = 0x%x   \n",(reg + i), data );

	}
#endif



	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_CvbsRateConversion.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	bActive 	[in] 	BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset(Handle_t	phDevice, BOOLEAN bInitOffsetHigh)
{

	UINT32 prevData;

	if(TRUE == bInitOffsetHigh)
	{

		if(retOK != ADEMOD_Register_Set(phDevice, 0xb01, 1, 0xa))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb01, 0xa) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb02, 1, 0xa))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb02, 0xa) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(retOK != ADEMOD_Register_Get(phDevice, 0xb46, 2, &prevData))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x2) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, 0xf))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb61, 2, 0xf))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb61, 0x4) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, prevData))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

	}
	else
	{

		if(retOK != ADEMOD_Register_Set(phDevice, 0xb01, 1, 0x3f))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb01, 0xa) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb02, 1, 0x3f))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb02, 0xa) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(retOK != ADEMOD_Register_Get(phDevice, 0xb46, 2, &prevData))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x2) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, 0x1))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb61, 2, 0x1))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb61, 0x4) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		
		if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, prevData))
		{
			DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
	}


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_ClampingCtrl.
 *Control clamp control.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	clampCount 	[in] 	UINT8
 * @param	stepsize     	[in] 	UINT16
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_ClampingCtrl(Handle_t phDevice, UINT8 clampCount, UINT16 stepsize)
{

	UINT32 prevData = 0;
	UINT32 RdClampingCount = 0;
	UINT32 RdstepSize = 0;
	


	if(retOK != ADEMOD_Register_Set(phDevice, 0xb01, 1, (UINT32)clampCount))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb01, 0x%x) !!!\n", __F__, __L__,clampCount);
		DEMOD_RETURN_FAIL(__F__);
	}
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb02, 1, (UINT32)clampCount))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb02, 0x%x) !!!\n", __F__, __L__,clampCount);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(retOK != ADEMOD_Register_Get(phDevice, 0xb46, 2, &prevData))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x%x) !!!\n", __F__, __L__,prevData);
		DEMOD_RETURN_FAIL(__F__);
	}
	
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, (UINT32)stepsize))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb61, 2, (UINT32)stepsize))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb61, 0x%x) !!!\n", __F__, __L__,stepsize);
		DEMOD_RETURN_FAIL(__F__);
	}
	
	if(retOK != ADEMOD_Register_Set(phDevice, 0xb46, 2, prevData))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xb56, audioMinClipData) !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


	if(retOK != ADEMOD_Register_Get(phDevice, 0xb01, 1, &RdClampingCount))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x%x) !!!\n", __F__, __L__,prevData);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(retOK != ADEMOD_Register_Get(phDevice, 0xb61, 2, &RdstepSize))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Get(0xb56, 0x%x) !!!\n", __F__, __L__,prevData);
		DEMOD_RETURN_FAIL(__F__);
	}

	if((RdClampingCount == (UINT32)clampCount) && (RdstepSize == (UINT32)stepsize))
	{
		DEMOD_PRINT("[ABB M14] Success !! (%s:%d) RdClampingCount(0x%x), RdstepSize(0x%x)  !!!\n", __F__, __L__,RdClampingCount,RdstepSize);
		return RET_OK;

	}
	else
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) RdClampingCount(0x%x), RdstepSize(0x%x)  !!!\n", __F__, __L__,RdClampingCount,RdstepSize);
		DEMOD_RETURN_FAIL(__F__);
	
	}
		
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl.
 * Select CVBS DEC sel.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl(Handle_t phDevice, LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel )
{
	
	if(LX_DEMOD_ANALOG_DTV == sel)
	{
		ACE_REG_M14B0_RdFL(abb_2);	  
		
		ACE_REG_M14B0_Wr01(abb_2, abb_mux_sel_cvbs, 0x0);   
		
		ACE_REG_M14B0_WrFL(abb_2);	
			
		DEMOD_PRINT("[ABB M14]SUCCESS (%s:%d) Going to LX_DEMOD_ANALOG_DTV... !!!\n", __F__, __L__);
	}
	else if(LX_DEMOD_ANALOG_ATV == sel)
	{
		ACE_REG_M14B0_RdFL(abb_2);	  
		
		ACE_REG_M14B0_Wr01(abb_2, abb_mux_sel_cvbs, 0x1);   
		
		ACE_REG_M14B0_WrFL(abb_2);	

		CTOP_CTRL_M14B0_RdFL(RIGHT,ctr130);	  
		
		CTOP_CTRL_M14B0_Wr01(RIGHT,ctr130, scart_feedback_enb, 0x0);   
		
		CTOP_CTRL_M14B0_WrFL(RIGHT,ctr130);	

		DEMOD_PRINT("[ABB M14] SUCCESS (%s:%d) Going to LX_DEMOD_ANALOG_ATV... !!!\n", __F__, __L__);
	}
	else if(LX_DEMOD_ANALOG_ATV_RC == sel)
	{
		ACE_REG_M14B0_RdFL(abb_2);	  
		
		ACE_REG_M14B0_Wr01(abb_2, abb_mux_sel_cvbs, 0x1);   
		
		ACE_REG_M14B0_WrFL(abb_2);	

		CTOP_CTRL_M14B0_RdFL(RIGHT,ctr130);	  
		
		CTOP_CTRL_M14B0_Wr01(RIGHT,ctr130, scart_feedback_enb, 0x1);   
		
		CTOP_CTRL_M14B0_WrFL(RIGHT, ctr130);



		DEMOD_PRINT("[ABB M14] SUCCESS (%s:%d) Going to LX_DEMOD_ANALOG_ATV_RC... !!!\n", __F__, __L__);
	}
	else
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) Invalid sel !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	
	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_SifCtrl.
 * Set Analog SIF Control.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	pllClkSource 	[in] 	UINT32
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_SifCtrl(Handle_t	phDevice, UINT32 pllClkSource)
{



	// ABB CENTERF_SIF
	switch(pllClkSource)
	{
		case LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ :
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x80);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0x5c);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x64);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x3b);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x47);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0xe1);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x7a);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0xa0);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x41);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x43);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;
			
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0xc4);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x3c);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x51);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0xdb);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0xb6);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x6d);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0x98);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x7a);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x57);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;
			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x69);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x84);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0xe5);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0x3a);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x4a);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x5d);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;

			
		case LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0xdf);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x7b);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0xef);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0x91);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0xc9);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x67);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;

		case LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ:
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0x2c);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x03);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x71);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;

		default :			
			DEMOD_PRINT( "^r^[M14  ATV] FAIL (%s:%d) Invaild sample rate, going to default setting(27MHz) !!!\n",  __F__, __L__);
			ADEMOD_Register_Set(phDevice, 0xb20, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb21, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb22, 1, 0x00);
			ADEMOD_Register_Set(phDevice, 0xb23, 1, 0xc4);
			ADEMOD_Register_Set(phDevice, 0xb24, 1, 0x3c);
			ADEMOD_Register_Set(phDevice, 0xb25, 1, 0x51);
			ADEMOD_Register_Set(phDevice, 0xb26, 1, 0x00);
			break;
	}	





	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl.
 * Select CVBS DEC sel.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	pllClkSource 	[in] 	UINT32
 * @param	sel 	[in] 	LX_DEMOD_ANALOG_SIFPATH_SEL_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_SifPathCtrl(Handle_t phDevice, UINT32 pllClkSource, LX_DEMOD_ANALOG_SIFPATH_SEL_T sel )
{
	

	switch(sel)
	{
		case LX_DEMOD_ANALOG_SIF_PATH_DIRECT:
			{

				// ABB_DMD_SIFDAC_INTFLT
				ADEMOD_Register_Set(phDevice, 0x462, 1, 0x1);

				// ABB_DMD_SIFDAC_MUTE
					ADEMOD_Register_Set(phDevice, 0x464, 1, 0x1);
				
				// ABB_DMD_SIFDAC_PDBM
					ADEMOD_Register_Set(phDevice, 0x460, 1, 0x0);


				if(retOK !=DEMOD_M14B0_ANALOG_Set_SifCtrl(phDevice, pllClkSource))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

			}
			break;
			
		case LX_DEMOD_ANALOG_SIF_PATH_INTERNAL:
			{

				// ABB_DMD_SIFDAC_INTFLT
				ADEMOD_Register_Set(phDevice, 0x462, 1, 0x1);

				// ABB_DMD_SIFDAC_MUTE
				ADEMOD_Register_Set(phDevice, 0x464, 1, 0x0);

				// ABB_DMD_SIFDAC_PDBM
				ADEMOD_Register_Set(phDevice, 0x460, 1, 0x1);

			}
			break;

		case LX_DEMOD_ANALOG_SIF_PATH_EXTERNAL:
			{

				// ABB_DMD_SIFDAC_INTFLT
				ADEMOD_Register_Set(phDevice, 0x462, 1, 0x0);
			
				// ABB_DMD_SIFDAC_MUTE
				ADEMOD_Register_Set(phDevice, 0x464, 1, 0x0);
			
				// ABB_DMD_SIFDAC_PDBM
				ADEMOD_Register_Set(phDevice, 0x460, 1, 0x1);


			}
			break;

		case LX_DEMOD_ANALOG_SIF_PATH_FROM_EXT_TUNER:
			{

				// ABB_DMD_SIFDAC_INTFLT
				ADEMOD_Register_Set(phDevice, 0x462, 1, 0x1);
			
				// ABB_DMD_SIFDAC_MUTE
				ADEMOD_Register_Set(phDevice, 0x464, 1, 0x1);
			
				// ABB_DMD_SIFDAC_PDBM
				ADEMOD_Register_Set(phDevice, 0x460, 1, 0x0);

			}
			break;
			

		default :
			{
				DEMOD_PRINT("[ABB M14] FAIL (%s:%d) Invalid SIF PATH !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);

			}
			break;

	}
	
	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_SpectrumInv.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Set_SpectrumInv(Handle_t phDevice, BOOLEAN bInv, BOOLEAN bforce)
{
	UINT32 spectrum[LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_SPCTR_INV)];

	spectrum[0] = (bInv == TRUE) ? 1 : 0;

	if(retOK != ADEMOD_Parameter_Set(phDevice, LX_ADEMOD_PARM_SPCTR_INV, spectrum, bforce))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Parameter_Set() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	_gSpectrumInv = bInv;
	
	DEMOD_PRINT("[ABB M14] (%s:%d) SPECTURM INV = %s !!!\n", __F__, __L__,(spectrum[0] == 1) ? "TRUE" : "FALSE");


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Set_ClampingCtrl.
 * On/Off clamp signal from CVD.
 *
 * @param  NONE
 * @param	phDevice 	[in] 	Handle_t
 * @param	Enable 	[in] 	BOOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_ClampingCtrlEnable(Handle_t phDevice, BOOLEAN Enable)
{
	UINT32 wrData;

	if(Enable == TRUE)
	{
		wrData = 0x0;
		DEMOD_PRINT("[ABB M14] (%s:%d) Clamping control ON !!!\n", __F__, __L__);
	}
	else
	{
		wrData = 0x1;
		DEMOD_PRINT("[ABB M14] (%s:%d) Clamping control OFF !!!\n", __F__, __L__);
	}

 
	 if(retOK != ADEMOD_Register_Set(phDevice, 0xb00, 1, wrData))
	 {
		 DEMOD_PRINT("[ABB M14] FAIL (%s:%d) ADEMOD_Register_Set(0xB00, CLAMP_CLR) !!!\n", __F__, __L__);
		 DEMOD_RETURN_FAIL(__F__);
	 }

	 return RET_OK;
	 	

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_ANALOG_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;


	DEMOD_PRINT( "^R^[ABB M14] pDemodInfo->controlState= %d..... ^0^\n",pDemodInfo->controlState);

	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				Handle_t	phDevice;

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_START  start..... ^0^\n");

				phDevice = pDemodInfo->setCfgParam.analog.handleABB;

				gPreviousAudio	 =	LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

				DEMOD_M14B0_ANALOG_SoftReset(phDevice);

				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_RETUNE_RF : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_START	end..... ^0^\n");
			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{
				LX_DEMOD_SMARTTUNE_PARAM_T smartTuneParam;
				BOOLEAN bSpectrumInv;
				Handle_t	phDevice;
				UINT8 clampCount;
				UINT16 stepsize;

				
				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_START  start..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_NOT_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_NOT_LOCKED_PAUSE_TIME;
				

				pDemodInfo->getCfgParam.analog.frequency = pDemodInfo->setCfgParam.analog.frequency;

				phDevice = 	pDemodInfo->setCfgParam.analog.handleABB;

				bSpectrumInv =  pDemodInfo->setCfgParam.analog.bSpectrumInv;

				smartTuneParam.phDevice		= pDemodInfo->setCfgParam.analog.handleABB;
				smartTuneParam.analogIF 	= pDemodInfo->setCfgParam.analog.analogIF;
				smartTuneParam.audioSystem 	= pDemodInfo->setCfgParam.analog.audioSystem;
				smartTuneParam.frequency	= pDemodInfo->setCfgParam.analog.frequency;
				smartTuneParam.rfMode		= pDemodInfo->setCfgParam.analog.rfMode;
				smartTuneParam.tuneMode		= pDemodInfo->setCfgParam.analog.tuneMode;
				smartTuneParam.sampleRate	= pDemodInfo->setCfgParam.analog.sampleRate;

				DEMOD_PRINT("[ABB M14](%s:%d)smartTuneParam.tuneMode = %d !!!\n", __F__, __L__,smartTuneParam.tuneMode);

				if (retOK != DEMOD_M14B0_ANALOG_SetAudioSystem(&smartTuneParam))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_SetAudioSystem() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_AUTO == (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
				{
					if(RET_OK != DEMOD_M14B0_ANALOG_ClampingCtrlEnable(phDevice, FALSE))
					{
						DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_ClampingCtrlEnable(FALSE) !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{
					if(RET_OK != DEMOD_M14B0_ANALOG_ClampingCtrlEnable(phDevice, TRUE))
					{
						DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_ClampingCtrlEnable(TRUE) !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				
					clampCount = 0xa;
					stepsize = 0xf;
					
					if(RET_OK != DEMOD_M14B0_ANALOG_Set_ClampingCtrl(phDevice, clampCount, stepsize))
					{
						DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Set_ClampingCtrl !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}



				}

				if(LX_DEMOD_TUNE_NORMAL != (pDemodInfo->setCfgParam.analog.tuneMode & 0xf0))
				{
					
					if(RET_OK != DEMOD_M14B0_ANALOG_CarrierlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CarrierlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "[ABB M14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");

				}
				else  //normal mode
				{

					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "[ABB M14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");


				}

//					DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset(phDevice, TRUE);
					DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_START	end..... ^0^\n");

			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_SCAN  start..... ^0^\n");
				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "[ABB M14] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "[ABB M14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_M14B0_ANALOG_CarrierlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CarrierlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SIGNAL_STABLE;

					DEMOD_PRINT( "[ABB M14] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "[ABB M14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
					pDemodInfo->signalStatus.bSignalValid = FALSE;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;



					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_RetuneStartTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_ANALOG_FRONT_END_NOT_LOCKED(%d KHz)..\n",pDemodInfo->setCfgParam.analog.tunedFreq) ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT_ALWAYS( "[ABB M14] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);



				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "[ABB M14] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_SCAN  end..... ^0^\n");
			}
			break;




													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK  start...end..... ^0^\n");

			}
			break;




											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				Handle_t				phDevice;
				LX_DEMOD_LOCK_STATE_T	lockState;
				LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T	audioSystem = LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_SYNC_CHECKING  start..... ^0^\n");

				phDevice = pDemodInfo->setCfgParam.analog.handleABB;
				audioSystem = pDemodInfo->setCfgParam.analog.audioSystem;

#if VIDEO_SYNC_LOCK_SELECTED_FOR_FINAL_LOCK

				if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoSync(phDevice, &lockState))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoSync() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#else
				if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoCarrier(phDevice, &lockState))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoCarrier() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

#endif
				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;
					DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_SYNC_CHECKING  end..... ^0^\n");
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime = 0;

				Handle_t				phDevice;
				LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T	audioSystem = LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_SIGNAL_STABLE  start..... ^0^\n");
				phDevice = pDemodInfo->setCfgParam.analog.handleABB;
				audioSystem = pDemodInfo->setCfgParam.analog.audioSystem;

#if VIDEO_SYNC_LOCK_SELECTED_FOR_FINAL_LOCK
				if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoSync(phDevice, &lockState))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoSync() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#else
				if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoCarrier(phDevice, &lockState))
				{
					DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoCarrier() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				
#endif

				if(LX_DEMOD_LOCKED == lockState)
				{

		
					pDemodInfo->bNotifiedLock	= TRUE;
					pDemodInfo->bNotifiedUnlock	= FALSE;

					pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
					pDemodInfo->signalStatus.bSignalValid = TRUE;


					if (RET_OK != DEMOD_M14B0_ANALOG_Get_FreqOffset(pDemodInfo, &freqOffset))
					{
						DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Get_FreqOffset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->freqOffset = freqOffset;

					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

					elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

					DEMOD_PRINT_ALWAYS( "[ABB M14] LX_DEMOD_SIGNAL_STABLE : MSG_ANALOG_FRONT_END_LOCKED..... ^^^0^^^\n");
					DEMOD_PRINT_ALWAYS( "[ABB M14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
					DEMOD_PRINT_ALWAYS( "^R^[ABB M14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz](Center = %d KHz) ..... ^^^0^^^\n", freqOffset,pDemodInfo->setCfgParam.analog.tunedFreq);

#if 1
					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
					{
						DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
#endif
//						DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset(phDevice, FALSE);


				}

				DEMOD_PRINT( "^R^[ABB M14] LX_DEMOD_SIGNAL_STABLE  end..... ^0^\n");
	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default:
			DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) pDemodInfo->controlState = %d !!!\n", __F__, __L__,pDemodInfo->controlState);
			pDemodInfo->controlState = LX_DEMOD_TUNE_START; 


			break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	Handle_t				phDevice;
	SINT32 	freqOffset=0;

	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T	audioSystem = LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN;

	DEMOD_PRINT( "^R^[ABB M14] DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock  start..... ^0^\n");
	phDevice = pDemodInfo->setCfgParam.analog.handleABB;
	audioSystem = pDemodInfo->setCfgParam.analog.audioSystem;

//	RegisterDump(phDevice, NULL,NULL);

//	DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset(phDevice, FALSE);

#if VIDEO_SYNC_LOCK_SELECTED_FOR_FINAL_LOCK
	if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoSync(phDevice, &lockState))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoSync() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#else
	if(RET_OK != DEMOD_M14B0_ANALOG_CheckVideoCarrier(phDevice, &lockState))
	{
		DEMOD_PRINT("[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_CheckVideoCarrier() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
	
#endif

#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)  //&& (TRUE == pDemodInfo->setCfgParam.analog.cvdLock))
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;
			DEMOD_PRINT( "[ABB M14](%s) pDemodInfo->lockCount = %d !!!\n", __F__,pDemodInfo->lockCount);

		}
		if(pDemodInfo->lockCount == 1)
		{
			pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock 	= TRUE;
			pDemodInfo->bNotifiedUnlock = FALSE;
			pDemodInfo->signalStatus.bSignalValid = TRUE;

			DEMOD_PRINT_ALWAYS( "[ABB M14] MONITORING FUNC : MSG_ANALOG_FRONT_END_LOCKED..... ^0^\n");

			if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
				pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		}
		if(pDemodInfo->lockCount >= 1)
		{
			pDemodInfo->lockCount = 0;
		}

		
		if (RET_OK != DEMOD_M14B0_ANALOG_Get_FreqOffset(pDemodInfo, &freqOffset))
		{
			DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) DEMOD_M14B0_ANALOG_Get_FreqOffset() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}
		pDemodInfo->freqOffset = freqOffset;
		DEMOD_PRINT( "[ABB M14](%s:%d)freqOffset = %d KHz !!!\n", __F__, __L__,freqOffset);
		DEMOD_PRINT( "[ABB M14](%s:%d)frequency center = %d KHz !!!\n", __F__, __L__,pDemodInfo->setCfgParam.analog.frequency);
		DEMOD_PRINT( "[ABB M14](%s:%d)tuned frq = %d KHz !!!\n", __F__, __L__,pDemodInfo->setCfgParam.analog.tunedFreq);

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "[ABB M14] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

	}
	else
	{

		pDemodInfo->lockCount = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;
			DEMOD_PRINT( "[ABB M14](%s) pDemodInfo->unLockCount = %d !!!\n", __F__,pDemodInfo->unLockCount);

			if(pDemodInfo->unLockCount == 20)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				DEMOD_PRINT_ALWAYS( "[ABB M14] MONITORING FUNC : MSG_ANALOG_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 20)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{

		}


	}

	DEMOD_PRINT( "^R^[ABB M14] DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock  end..... ^0^\n");

	return RET_OK;


}




#if 0
#endif

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Get_RegDump.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice	[in]	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Dbg_Get_RegDump(Handle_t	phDevice)
{

		RegisterDump(phDevice);

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Dbg_Set_RegisterValue.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice	[in]	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Dbg_Set_RegisterValue(Handle_t phDevice,  UINT32 RegAddr, UINT32 RegSize, UINT32 RegData)
{

		ADEMOD_Register_Set(phDevice, RegAddr, RegSize, RegData);

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Dbg_SmartTune.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	*pSmartTuneParam	[in]	LX_DEMOD_SMARTTUNE_PARAM_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Dbg_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam)
{


	DEMOD_M14B0_ANALOG_SmartTune(pSmartTuneParam,  TRUE);

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Dbg_Get_RegisterValue.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice	[in]	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Dbg_Get_RegisterValue(Handle_t phDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData)
{

		ADEMOD_Register_Get(phDevice,RegAddr, RegSize, pRegData);

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_M14B0_ANALOG_Dbg_TestFunctions.
 * Reset Analog module by software.
 *
 * @param  NONE
 * @param	phDevice	[in]	Handle_t
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_M14B0_ANALOG_Dbg_TestFunctions(Handle_t phDevice, UINT32 argc, UINT32 *val)
{

		KDRV_Dbg_Menu(phDevice,argc, val);

	return RET_OK;
}

#if 0
#endif

/**
 * ABB Optimizing tuenr setting for Brazil.
 *
 * @param	void
 * @return	void
 * @author 	sungbin.na(sungbin.na@lge.com)
 */
void DEMOD_M14B0_ANALOG_OptimizingTunerInBrazil(Handle_t phDevice)
{
	int i;

	/**
	 * FIELD_DATA_T
	 */
	typedef struct LX_ADEMOD_TABLE{
		UINT32 	reg;
		UINT32   count;
		UINT32   value;
	}FIELD_DATA_T;


	#define REG_CNT 	20

	FIELD_DATA_T brazil_FieldSetting[REG_CNT] =
	{
		{ 0x31B, 4, 0xE42905 },
		{ 0x330, 1, 0x05 },
		{ 0x340, 1, 0x09 },
		{ 0x341, 1, 0x08 },
		{ 0x342, 1, 0x09 },
		{ 0x343, 1, 0x08 },
		{ 0x92d, 2, 0x3d9f },
		{ 0x32c, 4, 0x1080 },
		{ 0x2b3, 2, 0x7fb },
		{ 0x2b5, 2, 0x88e },
		{ 0x281, 2, 0x1fa },
		{ 0x283, 2, 0x1fa },
		{ 0x285, 2, 0x2f9 },
		{ 0x287, 2, 0x38a },
		{ 0x289, 2, 0x1c83 },
		{ 0x28b, 2, 0x447 },
		{ 0x2bb, 2, 0x12 },
		{ 0x2bd, 2, 0xce },
		{ 0x2bf, 2, 0x1b },
		{ 0x2c1, 2, 0x583 },
	};

	for( i = 0; i < REG_CNT; i++)
	{
		ADEMOD_Register_SetX(phDevice,brazil_FieldSetting[i].reg, brazil_FieldSetting[i].count, brazil_FieldSetting[i].value, TRUE);
	}


}

/**
 * ABB Optimizing tuenr setting for Brazil.
 *
 * @param	void
 * @return	void
 * @author 	sungbin.na(sungbin.na@lge.com)
 */
int DEMOD_M14B0_ANALOG_OptimizingTunerSetting(Handle_t phDevice)
{


    DEMOD_PRINT( "[TU ATV](%s:%d) !!!\n",__F__,__L__);

//	ADEMOD_Register_Set(phDevice,0x92f,2,0x1f4);
//	DEMOD_PRINT( " [ASIA Area] Tuner Tuner Data setting addr : 0x92f, data : 0x1f4 \n");

	return RET_OK;

}


