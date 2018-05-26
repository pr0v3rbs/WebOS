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

#ifndef	_DEMOD_VQI_H_
#define	_DEMOD_VQI_H_

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


extern int DEMOD_L9_VSB_Set_DefaultRegisterValue(void);  //jeongpil.yun
extern int DEMOD_L9_VSB_CochannelExist(BOOLEAN *pCochannel);  //jeongpil.yun
extern int DEMOD_L9_VSB_PreMonitor(void);
extern int DEMOD_L9_VSB_Monitor(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_L9_VSB_Get_MSEdynStatus(BOOLEAN *pbEnable);
extern int DEMOD_L9_VSB_Get_TotalInfo( void);

extern int DEMOD_L9_VSB_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_L9_VSB_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_L9_QAM_SoftwareResetFEC(void); //jeongpil.yun
extern int DEMOD_L9_QAM_Monitor(void);
extern int DEMOD_L9_QAM_ModeAutoDetection(BOOLEAN bEnable);
extern int DEMOD_L9_QAM_64Mode(void);  //jeongpil.yun
extern int DEMOD_L9_QAM_256Mode(void);  //jeongpil.yun
extern int DEMOD_L9_QAM_Set_DefaultRegisterValue(void);  //jeongpil.yun
extern int DEMOD_L9_QAM_Get_TotalInfo( void);

extern int DEMOD_L9_QAM_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_L9_QAM_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);


extern int DEMOD_L9_ISDBT_Set_DefaultRegisterValue(void);
extern int DEMOD_L9_ISDBT_Set_Config_auto (BOOLEAN bAutoDetect);
extern int DEMOD_L9_ISDBT_Set_PartialConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
extern int DEMOD_L9_ISDBT_Set_FullConfig (LX_DEMOD_ISDBT_CONFIG_T *pIsdbtConfigParam);
extern int DEMOD_L9_ISDBT_Set_NeverLockWaitTime(UINT32 waitMs);
extern int DEMOD_L9_ISDBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread);
extern int DEMOD_L9_ISDBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode );
extern int DEMOD_L9_ISDBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode );
extern int DEMOD_L9_ISDBT_Get_TMCCInfo( LX_DEMOD_ISDBT_TMCC_CONFIG_T *pTmccParams);
extern int DEMOD_L9_ISDBT_Get_TotalInfo( void);
extern int DEMOD_L9_ISDBT_Get_IFO_LOCK( BOOLEAN *pIfoLock);
extern int DEMOD_L9_ISDBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus);
extern int DEMOD_L9_ISDBT_EqualizereReset(void);

extern int DEMOD_L9_ISDBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_L9_ISDBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_DRV_H_ */

/** @} */
