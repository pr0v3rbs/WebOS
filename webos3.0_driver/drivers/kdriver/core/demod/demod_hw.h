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


#ifndef	_DEMOD_HW_H_
#define _DEMOD_HW_H_

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

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif


#define FOREVER 	while(! kthread_should_stop())
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	DEMOD_CTX_T g_demod_ctx_info[DEMOD_MAX_DEVICE];

/* Common  function */

extern int DEMOD_MediaChange(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_INIT_PARAM_T *pTranMedia);
extern int DEMOD_ChannelChange(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_CONFIG_T *pDemodCfg);
extern int DEMOD_Get_TunedCFG(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_Get_signal_status(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_Tuning_Task(void *i_pvParam);

extern int DEMOD_RepeaterEnable(BOOLEAN bEnable);
extern int DEMOD_SoftwareReset(void);
extern int DEMOD_Serial_Control(BOOLEAN bEnable);
extern int DEMOD_Power_Save(BOOLEAN bEnable);
extern int DEMOD_StdOperModeContrl( LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr);
extern int DEMOD_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock);
extern int DEMOD_TPOutCLKEnable(BOOLEAN bEnable);
extern int DEMOD_Set_IF_Frq(UINT32 ifFrq);
extern int DEMOD_Set_AGCPolarity(LX_DEMOD_AGC_POLAR_T agcPola);
extern int DEMOD_Set_SpectrumCtrl(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable);
extern int DEMOD_Get_Id(UINT16 *pId);  //jeongpil.yun
extern int DEMOD_Get_SpectrumStatus(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv);
extern int DEMOD_Get_IFAGC(UINT16 *pIfAgc);
extern int DEMOD_Get_OperMode(LX_DEMOD_OPER_MODE_T *pOperMode);
extern int DEMOD_Get_CarrierFreqOffset(SINT32 *pFreqOff);
extern int DEMOD_Get_TPIFStatus(BOOLEAN *pbEnable);
extern int DEMOD_Get_VABER(UINT32 *pVber);
extern int DEMOD_Get_Packet_Error(UINT32 *pError);
extern int DEMOD_Get_BandWidthMode( LX_DEMOD_BWMODE_T *ChannelBW );
extern int DEMOD_Get_QAMMode( LX_DEMOD_RF_MODE_T *pQamMode );
extern int DEMOD_Get_MseInfo( LX_DEMOD_MSE_T *pSigMse);
extern int DEMOD_Get_Lock(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState);
extern int DEMOD_Get_SysLockTime(UINT16 *pSyslocktime);
extern int DEMOD_Get_FecLockTime(UINT16 *pFeclocktime);





/* DVB function */


extern int DEMOD_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate);
extern int DEMOD_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto);
extern int DEMOD_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam);
extern int DEMOD_DVBC_Set_DefaultRegisterValue(void);
extern int DEMOD_DVBC_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_DVBC_Set_QamMode (LX_DEMOD_RF_MODE_T constel);
extern int DEMOD_DVBC_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate);
extern int DEMOD_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet);
extern int DEMOD_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
extern int DEMOD_DVBC_Get_DvbInfo(void);

extern int DEMOD_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_DVBC2_IsQammodeAutoDet (BOOLEAN *bAutoQam);
extern int DEMOD_DVBC2_Set_DefaultRegisterValue(void);
extern int DEMOD_DVBC2_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_DVBC2_Set_PartialConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam);
extern int DEMOD_DVBC2_Set_FullConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam);
extern int DEMOD_DVBC2_Set_QamMode (UINT8 constel);
extern int DEMOD_DVBC2_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_DVBC2_Set_StartFrequency(UINT32 frequency);
extern int DEMOD_DVBC2_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
extern int DEMOD_DVBC2_Get_multiPLP_ID(LX_DEMOD_DVBC2_MULTI_PLP_INFO_T *pParamMultiPLPInfo);
extern int DEMOD_DVBC2_Get_DvbInfo(void);

extern int DEMOD_DVBC2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_DVBC2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);


extern int DEMOD_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band);
extern int DEMOD_DVBT_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
extern int DEMOD_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
extern int DEMOD_DVBT_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp );
extern int DEMOD_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode );
extern int DEMOD_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate );
extern int DEMOD_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate );
extern int DEMOD_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id);
extern int DEMOD_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams);
extern int DEMOD_DVBT_Get_TotalInfo( void);
extern int DEMOD_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock);
extern int DEMOD_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus);
extern int DEMOD_DVBT_EqualizereReset(void);

extern int DEMOD_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_DVBT2_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band);
extern int DEMOD_DVBT2_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_DVBT2_Set_PartialConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
extern int DEMOD_DVBT2_Set_FullConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
extern int DEMOD_DVBT2_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_DVBT2_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_DVBT2_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_DVBT2_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_DVBT2_Get_CoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate );
extern int DEMOD_DVBT2_Get_multiPLP_ID(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T *pParamMultiPLPInfo);
extern int DEMOD_DVBT2_Get_PlpInfo( LX_DEMOD_DVBT2_CONFIG_T *pPlpParams);
extern int DEMOD_DVBT2_Get_TotalInfo( void);

extern int DEMOD_DVBT2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_DVBT2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

/* VSB/QAM/ISDBT function */
extern int DEMOD_VQI_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock);


extern int DEMOD_VSB_Set_DefaultRegisterValue(void);  //jeongpil.yun
extern int DEMOD_VSB_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_VSB_CochannelExist(BOOLEAN *pCochannel);  //jeongpil.yun
extern int DEMOD_VSB_PreMonitor(void);
extern int DEMOD_VSB_Monitor(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_VSB_Get_MSEdynStatus(BOOLEAN *pbEnable);
extern int DEMOD_VSB_Get_TotalInfo( void);

extern int DEMOD_VSB_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_VSB_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_QAM_SoftwareResetFEC(void); //jeongpil.yun
extern int DEMOD_QAM_Monitor(void);
extern int DEMOD_QAM_ModeAutoDetection(BOOLEAN bEnable);
extern int DEMOD_QAM_64Mode(void);  //jeongpil.yun
extern int DEMOD_QAM_256Mode(void);  //jeongpil.yun
extern int DEMOD_QAM_Set_DefaultRegisterValue(void);  //jeongpil.yun
extern int DEMOD_QAM_Set_NeverLockWaitTime(UINT32 waitMs);

extern int DEMOD_QAM_Get_TotalInfo( void);

extern int DEMOD_QAM_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_QAM_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_QAM_EQ_Signal_Detector(BOOLEAN OnOff);

extern int DEMOD_ISDBT_Set_DefaultRegisterValue(void);
extern int DEMOD_ISDBT_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_ISDBT_Set_PartialConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
extern int DEMOD_ISDBT_Set_FullConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
extern int DEMOD_ISDBT_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_ISDBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_ISDBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_ISDBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_ISDBT_Get_TMCCInfo( LX_DEMOD_ISDBT_TMCC_CONFIG_T *pTmccParams);
extern int DEMOD_ISDBT_Get_TotalInfo( void);
extern int DEMOD_ISDBT_Get_IFO_LOCK( BOOLEAN *pIfoLock);
extern int DEMOD_ISDBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus);
extern int DEMOD_ISDBT_EqualizereReset(void);

extern int DEMOD_ISDBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_ISDBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

/* Analog */
extern int DEMOD_ANALOG_Power_Save(BOOLEAN bEnable);
extern int DEMOD_ANALOG_Demod_Open(UINT32 chipDevID, UINT32 handleI2C, Handle_t *phDevice);
extern int DEMOD_ANALOG_Init(Handle_t phDevice, LX_DEMOD_RF_MODE_T rfMode);
extern int DEMOD_ANALOG_Set_IF_Frq(Handle_t phDevice,UINT32 pllClkSource, UINT32 ifFrq);
extern int DEMOD_ANALOG_SoftwareReset(Handle_t phDevice);
extern int DEMOD_ANALOG_Set_AftRange(Handle_t phDevice, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange);
extern int DEMOD_ANALOG_Set_CvbsDecCtrl(Handle_t phDevice, LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel);
extern int DEMOD_ANALOG_Set_AbbMode(BOOLEAN bIsAbbMode);
extern int DEMOD_ANALOG_Set_WorkAround(void);
extern int DEMOD_ANALOG_Set_CvbsRateConversion(Handle_t phDevice, UINT32 pllClkSource);
extern int DEMOD_ANALOG_Set_HighCvbsRateOffset(Handle_t phDevice, BOOLEAN bInitOffsetHigh);
extern int DEMOD_ANALOG_Set_ClampingCtrl(UINT8 clampCount, UINT16 stepsize);
extern int DEMOD_ANALOG_Set_SifCtrl(Handle_t phDevice, UINT32 pllClkSource);
extern int DEMOD_ANALOG_Set_SifPathCtrl(Handle_t phDevice, UINT32 pllClkSource, LX_DEMOD_ANALOG_SIFPATH_SEL_T sel );
extern int DEMOD_ANALOG_Set_SpectrumInv(Handle_t phDevice, BOOLEAN bInv, BOOLEAN bforce );
extern int DEMOD_ANALOG_Set_SpecialSifData(Handle_t	phDevice,  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem);
extern int DEMOD_ANALOG_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_ANALOG_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_ANALOG_Dbg_Set_RegisterValue(Handle_t phDevice,  UINT32 RegAddr, UINT32 RegSize, UINT32 RegData);
extern int DEMOD_ANALOG_Dbg_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam);
extern int DEMOD_ANALOG_Dbg_Get_RegDump(Handle_t phDevice);
extern int DEMOD_ANALOG_Dbg_Get_RegisterValue(Handle_t phDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData);
extern int DEMOD_ANALOG_Dbg_TestFunctions(Handle_t hDemod, UINT32 argc, UINT32 *val);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_HW_H_ */

/** @} */

