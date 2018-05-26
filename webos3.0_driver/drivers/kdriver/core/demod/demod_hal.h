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

#ifndef	_DEMOD_HAL_H_
#define	_DEMOD_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "demod_kapi.h"
#include "demod_module.h"

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
typedef UINT32			LX_ADEMOD_Result;	   /*  return codes 				   */

typedef	struct
{
	const LX_DEMOD_CFG_T*	(*GetCfg)		(void);


/***********************************************************************************
* DVBT/ DVBC/ VSB/ QAM/ ISDBT  common  function
************************************************************************************/


	int (*ResetHW)							(void);	//jeongpil.yun
	int (*SetI2C)							(void);	//jeongpil.yun
	int (*SetInclk)							(LX_DEMOD_INCLK_SEL_T *inclk);	//jeongpil.yun
	int (*AdcInit)							(void);	//jeongpil.yun

	int (*Get_Id)							(UINT16 *pId);	//jeongpil.yun
	int (*RepeaterEnable)					(BOOLEAN bEnable);
	int (*SoftwareReset)					(void);
	int (*Serial_Control)					(BOOLEAN bEnable);
	int (*Power_Save)						(BOOLEAN bEnable);
	int (*StdOperModeContrl)				(LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr);
	int (*NeverlockScan)					(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock);
	int (*TPOutCLKEnable)					(BOOLEAN bEnable);
	int (*Set_IF_Frq)						(UINT32 ifFrq);
	int (*Set_AGCPolarity)					(LX_DEMOD_AGC_POLAR_T agcPola);
	int (*Set_SpectrumCtrl)					(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable);
	int (*Get_SpectrumStatus)				(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv);
	int (*Get_IFAGC)						(UINT16 *pIfAgc);
	int (*Get_OperMode)						(LX_DEMOD_OPER_MODE_T *pOperMode);
	int (*Get_NeverLockStatus)				(LX_DEMOD_OPER_MODE_T scanMode, LX_DEMOD_LOCK_STATE_T *pNeverLockStatus);
	int (*Get_CarrierFreqOffset)			(SINT32 *pFreqOff);
	int (*Get_TPIFStatus)					(BOOLEAN *pbEnable);
	int (*Get_VABER)						(UINT32 *pVber);
	int (*Get_Packet_Error)					(UINT32 *pError);
	int (*Get_BandWidthMode)				(LX_DEMOD_BWMODE_T *ChannelBW );
	int (*Get_QAMMode)						(LX_DEMOD_RF_MODE_T *pQamMode );
	int (*Get_MseInfo)						(LX_DEMOD_MSE_T *pSigMse);
	int (*Get_Lock)							(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState);
	int (*Get_SysLockTime)					(UINT16 *pSyslocktime);
	int (*Get_FecLockTime)					(UINT16 *pFeclocktime);


/***********************************************************************************
* DVBT/ DVBC common  function
************************************************************************************/


/***********************************************************************************
* DVBC function
************************************************************************************/

	int (*DVBC_AutoSymbolRateDet)			(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate);
	int (*DVBC_IsSymbolRateAuto)			(BOOLEAN *bAuto);
	int (*DVBC_IsQammodeAutoDet)			(BOOLEAN *bAutoQam);
	int (*DVBC_Set_DefaultRegisterValue)	(void);
	int (*DVBC_Set_Config_auto)				(BOOLEAN bAutoDetect);
	int (*DVBC_Set_QamMode) 				(LX_DEMOD_RF_MODE_T constel);
	int (*DVBC_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*DVBC_Get_SymbolRateDetect)		(UINT16 *pSymbolRate);
	int (*DVBC_Get_SymbolRateStatus)		(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet);
	int (*DVBC_Get_QamModeDetectStatus)		(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
	int (*DVBC_Get_DvbInfo)					(void);

	int (*DVBC_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*DVBC_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);


/***********************************************************************************
* DVBC2 function
************************************************************************************/

	int (*DVBC2_IsQammodeAutoDet)			(BOOLEAN *bAutoQam);
	int (*DVBC2_Set_DefaultRegisterValue)	(void);
	int (*DVBC2_Set_Config_auto)			(BOOLEAN bAutoDetect);
	int (*DVBC2_Set_PartialConfig)			(LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam);
	int (*DVBC2_Set_FullConfig)				(LX_DEMOD_DVBC2_CONFIG_T *pDvbt2ConfigParam);
	int (*DVBC2_Set_QamMode) 				(UINT8 constel);
	int (*DVBC2_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*DVBC2_Set_StartFrequency)			(UINT32 frequency);
	int (*DVBC2_Get_QamModeDetectStatus)	(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
	int (*DVBC2_Get_multiPLP_ID)			(LX_DEMOD_DVBC2_MULTI_PLP_INFO_T *pParamMultiPLPInfo);
	int (*DVBC2_Get_DvbInfo)				(void);

	int (*DVBC2_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*DVBC2_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);




/***********************************************************************************
* DVBT function
************************************************************************************/

	int (*DVBT_Set_DefaultRegisterValue)	(LX_DEMOD_BWMODE_T band);
	int (*DVBT_Set_Config_auto)				(BOOLEAN bAutoDetect);
	int (*DVBT_Set_PartialConfig)			(LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
	int (*DVBT_Set_FullConfig)				(LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
	int (*DVBT_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*DVBT_Get_DelaySpreadStatus)		(UINT16 *pDelaySpread);
	int (*DVBT_Get_Hierach_HPSel)			(BOOLEAN *pBSetHp );
	int (*DVBT_Get_FFTMode)					(LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
	int (*DVBT_Get_GuradIntervalMode)		(LX_DEMOD_GIMODE_T *pGIMode );
	int (*DVBT_Get_HierachyMode)			(LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode );
	int (*DVBT_Get_LpCoderRate)				(LX_DEMOD_CODERATE_T *pLpCodeRate );
	int (*DVBT_Get_HpCoderRate)				(LX_DEMOD_CODERATE_T *pHpCodeRate );
	int (*DVBT_Get_CellId)					(UINT8 *pCellIDLength, UINT16 *cell_id);
	int (*DVBT_Get_TpsInfo)					(LX_DEMOD_DVBT_CONFIG_T *pTpsParams);
	int (*DVBT_Get_TotalInfo)				(void);
	int (*DVBT_Get_IFO_LOCK)				(BOOLEAN *pIfoLock);
	int (*DVBT_Get_CochanDetIndicator)		(BOOLEAN *pDetStatus);
	int (*DVBT_EqualizereReset)				(void);

	int (*DVBT_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*DVBT_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);

/***********************************************************************************
* DVBT2 function
************************************************************************************/

	int (*DVBT2_Set_DefaultRegisterValue)	(LX_DEMOD_BWMODE_T band);
	int (*DVBT2_Set_Config_auto)			(BOOLEAN bAutoDetect);
	int (*DVBT2_Set_PartialConfig)			(LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
	int (*DVBT2_Set_FullConfig)				(LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
	int (*DVBT2_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*DVBT2_Get_DelaySpreadStatus)		(UINT16 *pDelaySpread);
	int (*DVBT2_Get_FFTMode)				(LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
	int (*DVBT2_Get_GuradIntervalMode)		(LX_DEMOD_GIMODE_T *pGIMode );
	int (*DVBT2_Get_CoderRate)				(LX_DEMOD_CODERATE_T *pCodeRate );
	int (*DVBT2_Get_multiPLP_ID)			(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T *pParamMultiPLPInfo);
	int (*DVBT2_Get_PlpInfo)				(LX_DEMOD_DVBT2_CONFIG_T *pPlpParams);
	int (*DVBT2_Get_TotalInfo)				(void);

	int (*DVBT2_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*DVBT2_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);



/***********************************************************************************
* VSB/ QAM/ ISDBT common  function
************************************************************************************/



/***********************************************************************************
* VSB function
************************************************************************************/

	int (*VSB_Set_DefaultRegisterValue)		(void);	//jeongpil.yun
	int (*VSB_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*VSB_CochannelExist)				(BOOLEAN *pCochannel);  //jeongpil.yun
	int (*VSB_PreMonitor)					(void);
	int (*VSB_Monitor)						(DEMOD_CTX_T *pDemodInfo);
	int (*VSB_Get_MSEdynStatus)				(BOOLEAN *pbEnable);
	int (*VSB_Get_TotalInfo)				(void);

	int (*VSB_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*VSB_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);


/***********************************************************************************
* QAM function
************************************************************************************/

	int (*QAM_SoftwareResetFEC)				(void); //jeongpil.yun
	int (*QAM_Monitor)						(void);
	int (*QAM_ModeAutoDetection)			(BOOLEAN bEnable);
	int (*QAM_64Mode)						(void);  //jeongpil.yun
	int (*QAM_256Mode)						(void);  //jeongpil.yun
	int (*QAM_Set_DefaultRegisterValue)		(void);	//jeongpil.yun
	int (*QAM_Set_NeverLockWaitTime)		(UINT32 waitMs);

	int (*QAM_Get_TotalInfo)				(void);

	int (*QAM_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*QAM_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*QAM_EQ_Signal_Detector)			(BOOLEAN OnOff);





/***********************************************************************************
* ISDBT function
************************************************************************************/

	int (*ISDBT_Set_DefaultRegisterValue)	(void);
	int (*ISDBT_Set_Config_auto)			(BOOLEAN bAutoDetect);
	int (*ISDBT_Set_PartialConfig)			(LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
	int (*ISDBT_Set_FullConfig)				(LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
	int (*ISDBT_Set_NeverLockWaitTime)		(UINT32 waitMs);
	int (*ISDBT_Get_DelaySpreadStatus)		(UINT16 *pDelaySpread);
	int (*ISDBT_Get_FFTMode)				(LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
	int (*ISDBT_Get_GuradIntervalMode)		(LX_DEMOD_GIMODE_T *pGIMode );
	int (*ISDBT_Get_TMCCInfo)				(LX_DEMOD_ISDBT_TMCC_CONFIG_T *pTmccParams);
	int (*ISDBT_Get_TotalInfo)				(void);
	int (*ISDBT_Get_IFO_LOCK)				(BOOLEAN *pIfoLock);
	int (*ISDBT_Get_CochanDetIndicator)		(BOOLEAN *pDetStatus);
	int (*ISDBT_EqualizereReset)			(void);

	int (*ISDBT_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*ISDBT_Monitoring_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);


/***********************************************************************************
* ABB function
************************************************************************************/

	int (*ADEMOD_Demod_Open)				(UINT32 chipDevID, UINT32 handleI2C,  Handle_t *phDevice);
	int (*ADEMOD_Init)						(Handle_t phDevice, LX_DEMOD_RF_MODE_T rfMode);
	int (*ADEMOD_Set_IF_Frq)				(Handle_t phDevice, UINT32 pllClkSource, UINT32 ifFrq);
	int (*ADEMOD_SoftwareReset)				(Handle_t phDevice);
	int (*ADEMOD_Set_AftRange)				(Handle_t phDevice, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange);

	int (*ADEMOD_ResetHW)							(void); //jeongpil.yun
	int (*ADEMOD_SetI2C)							(void); //jeongpil.yun
	int (*ADEMOD_Set_AbbMode) 						(BOOLEAN bIsAbbMode);	//jeongpil.yun
	int (*ADEMOD_Set_WorkAround) 					(void);	//jeongpil.yun
	int (*ADEMOD_Set_CvbsRateConversion) 		(Handle_t phDevice, UINT32 pllClkSource );
	int (*ADEMOD_Set_HighCvbsRateOffset) 		(Handle_t	phDevice, BOOLEAN bInitOffsetHigh);
	int (*ADEMOD_Set_SifCtrl) 					(Handle_t phDevice,UINT32 pllClkSource );
	int (*ADEMOD_Set_CvbsDecCtrl) 				(Handle_t phDevice, LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel);
	int (*ADEMOD_Set_ClampingCtrl)				(Handle_t phDevice, UINT8 clampCount, UINT16 stepsize);
	int (*ADEMOD_Set_SifPathCtrl) 				(Handle_t phDevice, UINT32 pllClkSource, LX_DEMOD_ANALOG_SIFPATH_SEL_T sel);
	int (*ADEMOD_Set_SpectrumInv) 				(Handle_t phDevice,BOOLEAN bIsAbbMode, BOOLEAN bforce);
	int (*ADEMOD_Set_SpecialSifData) 			(Handle_t	phDevice,  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem);

	int (*ADEMOD_Obtaining_Signal_Lock)		(DEMOD_CTX_T *pDemodInfo);
	int (*ADEMOD_Monitoring_Signal_Lock) 	(DEMOD_CTX_T *pDemodInfo);

	int (*ADEMOD_Dbg_Get_RegDump) 				(Handle_t phDevice);
	int (*ADEMOD_Dbg_Set_RegValue) 				(Handle_t phDevice,  UINT32 RegAddr, UINT32 RegSize, UINT32 RegData);
	int (*ADEMOD_Dbg_Set_SmartTune)				(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam);
	int (*ADEMOD_Dbg_Get_RegValue) 				(Handle_t phDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData);
	int (*ADEMOD_Dbg_TestFunctions) 			(Handle_t phDevice, UINT32 argc, UINT32 *val);

/* Internal point function */

	LX_ADEMOD_Result (*ADEMOD_Preset_Set)		(Handle_t hDevice, UINT32 smartTuneFilter, UINT32 Section);
	LX_ADEMOD_Result (*ADEMOD_Demod_Close)		(Handle_t hDevice);
	LX_ADEMOD_Result (*ADEMOD_Demodulate)		(Handle_t hDevice, UINT32 InputMode, UINT32 VideoStandard, UINT32 AudioStandard);
	LX_ADEMOD_Result (*ADEMOD_Parameter_Set)	(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue, BOOL UpdateNow);
	LX_ADEMOD_Result (*ADEMOD_Parameter_Get)	(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue);
	LX_ADEMOD_Result (*ADEMOD_AGC_Set)			(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow);
	LX_ADEMOD_Result (*ADEMOD_AGC_Get)			(Handle_t hDevice, UINT32* pDescriptor);
	LX_ADEMOD_Result (*ADEMOD_ChanScan_Set) 	(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow);
	LX_ADEMOD_Result (*ADEMOD_SleepMode_Set)	(Handle_t hDevice, UINT32 Mode);
	LX_ADEMOD_Result (*ADEMOD_SleepMode_Get)	(Handle_t hDevice, UINT32* pMode);
	LX_ADEMOD_Result (*ADEMOD_HostI2C_Write)	(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size);
	LX_ADEMOD_Result (*ADEMOD_HostI2C_Read) 	(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size);

/*Internal point function */


	
}
DEMOD_HAL_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	DEMOD_HAL_T	g_demod_hal;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_HAL_H_ */

