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

#ifndef	_DEMOD_DVB_H14EUBB_H_
#define	_DEMOD_DVB_H14EUBB_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "demod_impl.h"

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

extern int DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate);
extern int DEMOD_H14EUBB_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto);
extern int DEMOD_H14EUBB_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam);
extern int DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue(void);
extern int DEMOD_H14EUBB_DVBC_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_H14EUBB_DVBC_Set_QamMode (LX_DEMOD_RF_MODE_T constel);
extern int DEMOD_H14EUBB_DVBC_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_H14EUBB_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate);
extern int DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet);
extern int DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
extern int DEMOD_H14EUBB_DVBC_Get_DvbInfo(void);

extern int DEMOD_H14EUBB_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_H14EUBB_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_H14EUBB_DVBC2_IsQammodeAutoDet (BOOLEAN *bAutoQam);
extern int DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue(void);
extern int DEMOD_H14EUBB_DVBC2_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_H14EUBB_DVBC2_Set_PartialConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam);
extern int DEMOD_H14EUBB_DVBC2_Set_FullConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam);

extern int DEMOD_H14EUBB_DVBC2_Set_QamMode (UINT8 constel);
extern int DEMOD_H14EUBB_DVBC2_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_H14EUBB_DVBC2_Set_StartFrequency(UINT32 frequency);
extern int DEMOD_H14EUBB_DVBC2_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet);
extern int DEMOD_H14EUBB_DVBC2_Get_multiPLP_ID( LX_DEMOD_DVBC2_MULTI_PLP_INFO_T *pPlpParams);
extern int DEMOD_H14EUBB_DVBC2_Get_DvbInfo(void);

extern int DEMOD_H14EUBB_DVBC2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_H14EUBB_DVBC2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);


extern int DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band);
extern int DEMOD_H14EUBB_DVBT_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_H14EUBB_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
extern int DEMOD_H14EUBB_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam);
extern int DEMOD_H14EUBB_DVBT_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_H14EUBB_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_H14EUBB_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp );
extern int DEMOD_H14EUBB_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_H14EUBB_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode );
extern int DEMOD_H14EUBB_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate );
extern int DEMOD_H14EUBB_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate );
extern int DEMOD_H14EUBB_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id);
extern int DEMOD_H14EUBB_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams);
extern int DEMOD_H14EUBB_DVBT_Get_TotalInfo( void);
extern int DEMOD_H14EUBB_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock);
extern int DEMOD_H14EUBB_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus);
extern int DEMOD_H14EUBB_DVBT_EqualizereReset(void);

extern int DEMOD_H14EUBB_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_H14EUBB_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);



extern int DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band);
extern int DEMOD_H14EUBB_DVBT2_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_H14EUBB_DVBT2_Set_PartialConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
extern int DEMOD_H14EUBB_DVBT2_Set_FullConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam);
extern int DEMOD_H14EUBB_DVBT2_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_H14EUBB_DVBT2_Set_GIDectectCtrl(void);
extern int DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl(void);
extern int DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_H14EUBB_DVBT2_Get_TPCorrectedFlag(UINT8 *pTpCorrected);
extern int DEMOD_H14EUBB_DVBT2_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_H14EUBB_DVBT2_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_H14EUBB_DVBT2_Get_CoderRate( LX_DEMOD_CODERATE_T *pCodeRate );
extern int DEMOD_H14EUBB_DVBT2_Get_CurrentPLP_ID(UINT8 *pCurrPlpID);
extern int DEMOD_H14EUBB_DVBT2_Get_StartPLP_ID(UINT8 *pStartPlpID);
extern int DEMOD_H14EUBB_DVBT2_Get_PlpInfo( LX_DEMOD_DVBT2_CONFIG_T *pPlpParams);
extern int DEMOD_H14EUBB_DVBT2_Get_multiPLP_ID( LX_DEMOD_DVBT2_MULTI_PLP_INFO_T *pPlpParams);
extern int DEMOD_H14EUBB_DVBT2_Get_TotalInfo( void);

extern int DEMOD_H14EUBB_DVBT2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_H14EUBB_DVBT2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_DRV_H_ */

/** @} */
