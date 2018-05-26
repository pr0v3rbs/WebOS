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

#ifndef	_DEMOD_COMMON_H14GBB_H_
#define	_DEMOD_COMMON_H14GBB_H_

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
extern int DEMOD_H14GBB_ResetHW(void);
extern int DEMOD_H14GBB_SetI2C(void);
extern int DEMOD_H14GBB_SetInclk(LX_DEMOD_INCLK_SEL_T *inclk);


extern int DEMOD_H14GBB_RepeaterEnable(BOOLEAN bEnable);
extern int DEMOD_H14GBB_SoftwareReset(void);
extern int DEMOD_H14GBB_Serial_Control(BOOLEAN bEnable);
extern int DEMOD_H14GBB_Power_Save(BOOLEAN bEnable);
extern int DEMOD_H14GBB_StdOperModeContrl( LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr);
extern int DEMOD_H14GBB_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock);
extern int DEMOD_H14GBB_TPOutEnable(BOOLEAN bEnable);
extern int DEMOD_H14GBB_Set_IF_Frq(UINT32 ifFrq);
extern int DEMOD_H14GBB_Set_AGCPolarity(LX_DEMOD_AGC_POLAR_T agcPola);
extern int DEMOD_H14GBB_Set_SpectrumCtrl(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable);
extern int DEMOD_H14GBB_Get_Id(UINT16 *pId);
extern int DEMOD_H14GBB_Get_SpectrumStatus(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv);
extern int DEMOD_H14GBB_Get_IFAGC(UINT16 *pIfAgc);
extern int DEMOD_H14GBB_Get_OperMode(LX_DEMOD_OPER_MODE_T *pOperMode);
extern int DEMOD_H14GBB_Get_NeverLockStatus(LX_DEMOD_OPER_MODE_T scanMode, LX_DEMOD_LOCK_STATE_T *pNeverLockStatus);
extern int DEMOD_H14GBB_Get_CarrierFreqOffset(SINT32 *pFreqOff);
extern int DEMOD_H14GBB_Get_TPIFStatus(BOOLEAN *pbEnable);
extern int DEMOD_H14GBB_Get_VABER(UINT32 *pVber);
extern int DEMOD_H14GBB_Get_Packet_Error(UINT32 *pError);
extern int DEMOD_H14GBB_Get_BandWidthMode( LX_DEMOD_BWMODE_T *ChannelBW );
extern int DEMOD_H14GBB_Get_QAMMode( LX_DEMOD_RF_MODE_T *pQamMode );
extern int DEMOD_H14GBB_Get_MseInfo( LX_DEMOD_MSE_T *pSigMse);
extern int DEMOD_H14GBB_Get_Lock(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState);

extern int DEMOD_H14GBB_VSB_Get_SysLockTime(UINT16 *pSyslocktime);
extern int DEMOD_H14GBB_VSB_Get_FecLockTime(UINT16 *pFeclocktime);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	unsigned int g_NewTuneReqeustTime;
extern	unsigned int g_RetuneStartTime;

extern	UINT64 pauseTimeDemod ;


extern	UINT32 notifyinglockTimeOut;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_DRV_H_ */

/** @} */
