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

#include <linux/timer.h>


#include "demod_impl.h"
#include "demod_reg_h14eu.h"
#include "demod_common_h14eu.h"
#include "demod_dvb_h14eu.h"


#if 0
#endif

/******************************************************************************
	Global Type Definitions
******************************************************************************/
UINT16 gH14EUBB_FullPkerrCount = 7;

/******************************************************************************
	Static Variables & Function Prototypes Declarations
******************************************************************************/
static	UINT16			_gH14EUBB_DVB_UnlockCount_full_packeterr	= 0;


#if 1
/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_AutoSymbolRateDet
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  benable - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate )
{
	UINT8 	srd_en_c;
	UINT64	tempCalc_1;
	UINT32 	tempCalc_2;

	UINT8 	trcenterf_31_24;
	UINT8 	trcenterf_23_16;
	UINT8 	trcenterf_15_8;
	UINT8 	trcenterf_7_0;

	if(symrateMode == LX_DEMOD_SYMR_FIXED_MODE)
	{
		srd_en_c = 0;

		// TRCENTERF = [{48M/(4x symbol rate)}-1]x2^27 ==> 48MHz clock
		// TRCENTERF = [{41M/(4x symbol rate)}-1]x2^27 ==> 41MHz clock we have to choose  this equation.
		tempCalc_1 = 1375731712000;  // (41000/4) * 2^27
		do_div(tempCalc_1,SymbolRate);
		tempCalc_1 = tempCalc_1 - 134217728; // 2^27 = 134217728
		tempCalc_2 = (UINT32)tempCalc_1;

		trcenterf_31_24 = ((tempCalc_2 >> 24) & 0xff);
		trcenterf_23_16 = ((tempCalc_2 >> 16) & 0xff);;
		trcenterf_15_8	= ((tempCalc_2 >> 8) & 0xff);;
		trcenterf_7_0	= ((tempCalc_2 >> 0) & 0xff);;


		C_Sync_RdFL(c_sync_ctrl0);	//regAddr = 0x1800, C_SYNC_CTRL[0]
		C_Sync_Wr01(c_sync_ctrl0,srd_en_c,srd_en_c);
		C_Sync_WrFL(c_sync_ctrl0);
		//SRD_EN_C : SRD enable (0 : SRD off, 1 : SRD on)

		DVB_Common_RdFL(common_ctrl28);   //regAddr = 0x001c : COMMON_CTRL[28]
		DVB_Common_Wr01(common_ctrl28,trcenterf_31_24, trcenterf_31_24 );
		DVB_Common_WrFL(common_ctrl28);
		//TRCENTERF_8M[31:24]  : Select A/D sampling frequency.
		// TRCENTERF for 8MHz BW = [{48M/(4x64/7M)}-1]x2^27 = 41943040 = 0x2800000	--> in case of sampling frequency ( 48M )

		DVB_Common_RdFL(common_ctrl29);   //regAddr = 0x001d : COMMON_CTRL[29]
		DVB_Common_Wr01(common_ctrl29,trcenterf_23_16, trcenterf_23_16 );
		DVB_Common_WrFL(common_ctrl29);
		//TRCENTERF_8M[31:24]  : Select A/D sampling frequency.

		DVB_Common_RdFL(common_ctrl30);   //regAddr = 0x001e : COMMON_CTRL[30]
		DVB_Common_Wr01(common_ctrl30,trcenterf_15_8, trcenterf_15_8 );
		DVB_Common_WrFL(common_ctrl30);
		//TRCENTERF_8M[31:24]  : Select A/D sampling frequency.

		DVB_Common_RdFL(common_ctrl31);   //regAddr = 0x001f : COMMON_CTRL[31]
		DVB_Common_Wr01(common_ctrl31,trcenterf_7_0, trcenterf_7_0 );
		DVB_Common_WrFL(common_ctrl31);
		//TRCENTERF_8M[31:24]  : Select A/D sampling frequency.

		// TRCENTERF for 8MHz BW = [{48M/(4x64/7M)}-1]x2^27 = 41943040 = 0x2800000	--> in case of sampling frequency ( 48M )
		// TRCENTERF for 8MHz BW = [{41M/(4x64/7M)}-1]x2^27 =  0x00f80000	--> in case of sampling frequency ( 41M )

	}
	else
	{
		srd_en_c = 1;

		C_Sync_RdFL(c_sync_ctrl0);	//regAddr = 0x1800, C_SYNC_CTRL[0]
		C_Sync_Wr01(c_sync_ctrl0,srd_en_c,srd_en_c);
		C_Sync_WrFL(c_sync_ctrl0);
		//SRD_EN_C : SRD enable (0 : SRD off, 1 : SRD on)

	}


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_IsSymbolRateAuto
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  *bAuto - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto)
{
	UINT8 	srd_en_c;

	C_Sync_RdFL(c_sync_ctrl0);	//regAddr = 0x1800, C_SYNC_CTRL[0]
	C_Sync_Rd01(c_sync_ctrl0,srd_en_c,srd_en_c);
	//SRD_EN_C : SRD enable (0 : SRD off, 1 : SRD on)

	*bAuto = (srd_en_c == 0x1) ? TRUE : FALSE;



	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_IsQammodeAutoDet
 * Control automatic setting of DVBC config.
 *
 * @param *bAutoQam 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{


	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue(void)
{
	UINT8 cbldetrep_en;
	UINT8 tp_oen;
	UINT8 ifagc_oen;

#if 1

	UINT8 sam_logs_c_3_0;

	UINT8 tiroton;
#endif
	cbldetrep_en = 0x0;
	DVB_Common_RdFL(common_ctrl107);   //regAddr = 0x006b,COMMON_CTRL[107]
	DVB_Common_Wr01(common_ctrl107,cbldetrep_en,cbldetrep_en);
	DVB_Common_WrFL(common_ctrl107);	//regAddr = 0x006b,COMMON_CTRL[107]

	// CBLDETREP_EN : Repetition enable in case of auto detection mode(	0 : Disable, 	1 : Enable)

	tp_oen = 0x0;
	ifagc_oen = 0x0;
	DVB_Common_RdFL(common_ctrl110);   // regAddr = 0x006e,COMMON_CTRL[110]
	DVB_Common_Wr02(common_ctrl110,tp_oen,tp_oen, ifagc_oen, ifagc_oen);
	DVB_Common_WrFL(common_ctrl110);	  // regAddr = 0x006e,COMMON_CTRL[110]

#if 1

	sam_logs_c_3_0 = 0x1;

	C_Sync_RdFL(c_sync_ctrl18);   // regAddr = 0x1812, C_SYNC_CTRL[18]
	C_Sync_Wr01(c_sync_ctrl18, sam_logs_c_3_0, sam_logs_c_3_0);
	C_Sync_WrFL(c_sync_ctrl18);	  // regAddr = 0x1812, C_SYNC_CTRL[18]

	tiroton = 1;
	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
	T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
	T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);
#endif
	_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Set_Config_auto (BOOLEAN bAutoDetect)
{

	UINT8 useopm;
	UINT8 opm_1_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(bAutoDetect == TRUE)
	{
		useopm = 0; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		DVB_Common_RdFL(common_ctrl1);  //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr01(common_ctrl1,useopm,useopm); //USEOPM
		DVB_Common_WrFL(common_ctrl1);

	}
	else
	{
		useopm = 1; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		opm_1_0 = 3;

		DVB_Common_RdFL(common_ctrl1);  //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr02(common_ctrl1,useopm,useopm,opm_1_0,opm_1_0 ); //USEOPM, OPM[1:0]
		DVB_Common_WrFL(common_ctrl1);
		// OPM[1:0] : Set operation mode manually.
		// 00 : DVB-T2 mode
		// 01 : DVB-T mode
		// 10 : DVB-C2 mode
		// 11 : DVB-C mode

	}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Set_QamMode (LX_DEMOD_RF_MODE_T constel)
{
	UINT8 qamset_4_0 = 0x10;
	UINT8 tqamset_4_0= 0x10;

	switch(constel)
	{
		case LX_DEMOD_16QAM :
			{
				qamset_4_0 = 0x1;
				tqamset_4_0 = 0x1;
			}
			break;

		case LX_DEMOD_32QAM :
			{
				qamset_4_0 = 0x2;
				tqamset_4_0 = 0x2;
			}
			break;

		case LX_DEMOD_64QAM :
			{
				qamset_4_0 = 0x4;
				tqamset_4_0 = 0x4;
			}
			break;

		case LX_DEMOD_128QAM :
			{
				qamset_4_0 = 0x8;
				tqamset_4_0 = 0x8;
			}
			break;

		case LX_DEMOD_256QAM :
			{
				qamset_4_0 = 0x10;
				tqamset_4_0 = 0x10;
			}
			break;

		default :
			break;
	}

	C_Eq_RdFL(c_eq_ctrl8);	 //regAddr = 0x4808, C_EQ_CTRL[8]
	C_Eq_Wr01(c_eq_ctrl8, qamset_4_0, qamset_4_0);
	C_Eq_WrFL(c_eq_ctrl8);
	//QAMSET[4:0] : QAM mode condidate selection
	// QAMSET[4] : 256 QAM
	// QAMSET[3] : 128 QAM
	// QAMSET[2] : 64 QAM
	// QAMSET[1] : 32 QAM
	// QAMSET[0] : 16 QAM

	C_Eq_RdFL(c_eq_ctrl9);	 //regAddr = 0x4809, C_EQ_CTRL[9]
	C_Eq_Wr01(c_eq_ctrl9, tqamset_4_0, tqamset_4_0);
	C_Eq_WrFL(c_eq_ctrl9);
	//TQAMSET[4:0] : Control QAM-trial mode for EQ Slicer
	// TQAMSET[4] : 256 QAM
	// TQAMSET[3] : 128 QAM
	// TQAMSET[2] : 64 QAM
	// TQAMSET[1] : 32 QAM
	// TQAMSET[0] : 16 QAM

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Set_NeverLockWaitTime(UINT32 waitMs)
{

#if 0
	UINT8 opm_tdet_tm_4_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

		scsyslockchktime_7_0 = (UINT8)(waitMs / 10);

		DVB_Common_Wr01(common_ctrl108,opm_tdet_tm_4_0,opm_tdet_tm_4_0);
		DVB_Common_WrFL(common_ctrl108);	//regAddr = 0x006c,COMMON_CTRL[108]
		//OPM_TDET_TM[4:0] : Never lock Time [ms]
		// 0000 : 400
		// 0001 : 500
		// 0010 : 600
		// 0011 : 700
		// 0100 : 800
		// 0101 : 900
		// 0110 : 1000
		// 0111 : 1100
		// 1000 : 1200
		// 1001 : 1300
		// 1010 : 1400
		// 1011 : 1500
		// 1100 : 1600
		// 1101 : 1700
		// 1110 : 1800
		// 1111 : 1900

#endif


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Get_SymbolRateDetect
 * Get symbol rate in KHz (Don't use this function until revision A1)
 *
 * @param *pSymbolRate 	[OUT] UINT16 - symbol rate : unit: KHz.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate)
{

//read 0x1881, 0x1882

	UINT8	srd_lockindex1st_c_12_8 = 0;
	UINT8	srd_lockindex1st_c_7_0 = 0;


		UINT32 tempSymbolRate = 0;
		UINT32 tempSymbolRateKHz = 0;
		UINT32 symbolRateKHz = 0;

		*pSymbolRate = 0;

		//Detected symbol rate monitoring
		//symbol rate(MHz) = DETSYMRATE / 2^13

	C_Sync_RdFL(c_sync_mon1);  //regAddr = 0x1881, C_SYNC_MON[1]
	C_Sync_Rd01(c_sync_mon1,srd_lockindex1st_c_12_8,srd_lockindex1st_c_12_8);
	//SRD_LOCKINDEX1ST_C[12:8] : SRD 1st detected index
	// Location of maximum peak index in frequency domain (8K FFT)
	// Symbol rate[Ms/S] = [(8191-index)/8191]* sampling frequency

	tempSymbolRate = (srd_lockindex1st_c_12_8 << 8 ) ;

	C_Sync_RdFL(c_sync_mon2);  //regAddr = 0x1882, C_SYNC_MON[2]
	C_Sync_Rd01(c_sync_mon2,srd_lockindex1st_c_7_0,srd_lockindex1st_c_7_0); //SRD_LOCKINDEX1ST_C[7:0] : SRD 1st detected index
	tempSymbolRate += (srd_lockindex1st_c_7_0 << 0 ) ;


	symbolRateKHz = (8191 - tempSymbolRate) * 41000 ; // symbol rate [KHz] = (8191  - tempSymbolRate) * 1000 * 41 , (2^13) - 1  = 8191.
	symbolRateKHz = symbolRateKHz / 8191;

	tempSymbolRateKHz = symbolRateKHz / 10 ;

	if((symbolRateKHz -  tempSymbolRateKHz* 10) < 3)
		symbolRateKHz = (tempSymbolRateKHz)* 10;

	else if((symbolRateKHz -  tempSymbolRateKHz* 10) < 8)
		symbolRateKHz = (tempSymbolRateKHz )*10 + 5;

		else
		symbolRateKHz = (tempSymbolRateKHz )*10 + 10;


		*pSymbolRate = (UINT16)(symbolRateKHz);

	DEMOD_PRINT("[DEMOD H13] Symbol rate is %d [KHz]!!!\n",symbolRateKHz);




		return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus
 * Get symbol rate detection state on DVB-C
 * Don't use this function until revision A1.
 *
 * @param *pSymbolRateDet 	[OUT] SYMBOLRATE_STATE_T - status and detected symbol rate .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet)
{

	UINT8 srd_done_c;
	UINT16 symbolRate;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	pSymbolRateDet->status = LX_DEMOD_UNKOWN_STATE;
	pSymbolRateDet->symbolRate = 0;


	C_Sync_RdFL(c_sync_mon0);   //regAddr = 0x1880, C_SYNC_MON[0]
	C_Sync_Rd01(c_sync_mon0, srd_done_c, srd_done_c); //SRD_DONE_C : SRD done ( '0'->SRD not done, '1'->  SRD done )


	if(1 == srd_done_c)
	{
		pSymbolRateDet->status = LX_DEMOD_DETECTED;
		DEMOD_PRINT("[DEMOD H14] Success (%s:%d) Symbol rate is detected !!!\n", __F__, __L__);
		if (RET_OK != DEMOD_H14EUBB_DVBC_Get_SymbolRateDetect(&symbolRate))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Symbol rate couldn't be read !!!\n", __F__, __L__);
			return RET_ERROR;
		}
		pSymbolRateDet->symbolRate = symbolRate;
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Symbol rate couldn't be detected !!!\n", __F__, __L__);
		pSymbolRateDet->status = LX_DEMOD_DETECTION_FAIL;

	}




	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_RF_MODE_T 				qamMode;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	pQamModeDet->status = LX_DEMOD_DETECTION_FAIL;
	pQamModeDet->qamMode = LX_DEMOD_UNKOWN_STATE;

	if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if(lockState == LX_DEMOD_LOCKED)
	{
		pQamModeDet->status = LX_DEMOD_DETECTED;
		if (RET_OK != DEMOD_H14EUBB_Get_QAMMode(&qamMode ))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_QAMMode() !!!\n", __F__, __L__);
			return RET_ERROR;
		}
		pQamModeDet->qamMode = qamMode;

	}


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	LX_DEMOD_OPER_MODE_T 			operMode;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_H14EUBB_DVBC_Set_NeverLockWaitTime(LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
				{
					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

				}
				else
				{

#if 1
					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FIXED_MODE, pDemodInfo->setCfgParam.dvbc.symbolRate))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_QamMode(pDemodInfo->setCfgParam.dvbc.qamMode))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_QamMode() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


#endif
				}



				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
				{

					if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD H14] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);

					}
					if(LX_DEMOD_DVBC2 == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBC2;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING; //LX_DEMOD_RETUNE_RF;
						pDemodInfo->setCfgParam.dvbc2.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
						pDemodInfo->setCfgParam.dvbc2.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;
						pDemodInfo->setCfgParam.dvbc2.unPLP = 0;
						pDemodInfo->setCfgParam.dvbc2.sliceID = 0;
						pDemodInfo->setCfgParam.dvbc2.frequency = pDemodInfo->setCfgParam.dvbc.frequency;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBC2)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}
					else //(LX_DEMOD_DVBC == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBC;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBC)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}



				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_NewTuneReqeustTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBC_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;



													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;




											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;

				if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);

				}
				DEMOD_PRINT("[DEMOD H14] LX_DEMOD_SYNC_CHECKING() operMode = 0x%x!!!\n", operMode);

				if(LX_DEMOD_DVBC2 == operMode)
				{
					pDemodInfo->operMode = LX_DEMOD_DVBC2;
					pDemodInfo->setCfgParam.dvbc2.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
					pDemodInfo->setCfgParam.dvbc2.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;
					pDemodInfo->setCfgParam.dvbc2.unPLP = 0;
					pDemodInfo->setCfgParam.dvbc2.sliceID = 0;
					pDemodInfo->setCfgParam.dvbc2.frequency = pDemodInfo->setCfgParam.dvbc.frequency;

					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue())
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_StartFrequency(pDemodInfo->setCfgParam.dvbc2.frequency))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_StartFrequency() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					pDemodInfo->setCfgParam.dvbc2.unPLP = 0;
					pDemodInfo->setCfgParam.dvbc2.sliceID = 0;

					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbc2)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING(DVBC) : Going to LX_DEMOD_SYNC_CHECKING(DVBC2 forcely)..... ^0^\n");

					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
				 	/* nothing to do so far */

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbc.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
								if(packetError < LX_DEMOD_PACKET_ERROR_THD)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

								}
								else
								{
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

									}

								}

							}

						}
					}
					else
	{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;


						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);



					}

					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


				}
				else
			{
//					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
//					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
				break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_OPER_MODE_T operMode;

	if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if((operMode == LX_DEMOD_DVBC) || (operMode == LX_DEMOD_DVBC2))
	{
		if(operMode != LX_DEMOD_DVBC)
		{
			pDemodInfo->operMode = LX_DEMOD_DVBC2;
			pDemodInfo->setCfgParam.dvbc2.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
			pDemodInfo->setCfgParam.dvbc2.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;
			pDemodInfo->setCfgParam.dvbc2.unPLP = 0;
			pDemodInfo->setCfgParam.dvbc2.sliceID = 0;
			pDemodInfo->setCfgParam.dvbc2.frequency = pDemodInfo->setCfgParam.dvbc.frequency;
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) forcely changed to DVBC2 mode \n",__F__, __L__);

			return RET_OK;
		}

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild operation mode !! \n",__F__, __L__);
		return RET_ERROR;

	}
	if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{

		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= 160)
		{
			_gH14EUBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT("[DEMOD H14] (%s) _gH14EUBB_DVB_UnlockCount_full_packeterr = %d  !!!\n", __F__,_gH14EUBB_DVB_UnlockCount_full_packeterr );

			if(_gH14EUBB_DVB_UnlockCount_full_packeterr == gH14EUBB_FullPkerrCount)
			{
				DEMOD_PRINT("[DEMOD H14] ooops !! (%s) Force to NOT LOCK  !!!\n", __F__);
				_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

				/* software Reset for DVB channel decoder */
				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				DEMOD_PRINT("[DEMOD H14] EXCUTE, GBB_DVB_SoftwareReset() !!!\n");

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
			}
		}
		else
		{

			_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)	&& (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

		}

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}



	}
	else
	{
		pDemodInfo->lockCount = 0;
		_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBC_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
#if 0
			if(RET_OK != DEMOD_H14EUBB_NeverLock_Infinite_Scan(operMode, TRUE))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverLock_Infinite_Scan() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
#endif

			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= FALSE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xff;

//			pDemodInfo->setCfgParam.dvbc.tuneMode	= LX_DEMOD_TUNE_AUTO;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_2500;


		}



	}


	return RET_OK;



}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC_Get_DvbInfo
 * Get all information on DVB-C for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC_Get_DvbInfo(void)

{

	UINT32 errorCount;
	UINT32 vBer;

	UINT16 	ifAGCGain = 0;
	LX_DEMOD_MSE_T sigMse;

	BOOLEAN isAutoSymbol = FALSE;
	BOOLEAN isAutoQammode = FALSE;

	LX_DEMOD_SYMBOLRATE_STATE_T symbolRateDet;
	LX_DEMOD_QAMMODE_STATE_T qamModeDet;

	LX_DEMOD_LOCK_STATE_T neverLockStatus;

	LX_DEMOD_LOCK_STATE_T samplingLock;

	LX_DEMOD_LOCK_STATE_T carrLockState;
	LX_DEMOD_LOCK_STATE_T fecLockState;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_TPERRCNTMODE_T errorMode;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	memset(&qamModeDet, 0x00, sizeof(LX_DEMOD_QAMMODE_STATE_T));

	//IF AGC
	if (RET_OK != DEMOD_H14EUBB_Get_IFAGC(&ifAGCGain))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_IFAGC() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] IF AGC Gain = 0x%x !!!\n",ifAGCGain);

 	//NeverLock status
	if (RET_OK != DEMOD_H14EUBB_Get_NeverLockStatus(LX_DEMOD_DVBC, &neverLockStatus))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_NeverLockStatus() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	switch(neverLockStatus)
			{

		case LX_DEMOD_IDLE :
			DEMOD_PRINT("[DEMOD H14] Initialization process  !!!\n");
			break;

		case LX_DEMOD_DETECTION_PROCESSING :
			DEMOD_PRINT("[DEMOD H14] Signal Checking process !!!\n");
				break;

		case LX_DEMOD_DETECTED :
			DEMOD_PRINT("[DEMOD H14] normal State (available channel)  !!!\n");
			break;

		case LX_DEMOD_DETECTION_FAIL :
			DEMOD_PRINT("[DEMOD H14] neverlock state (empty channel) !!!\n");

			break;

		default :

			DEMOD_PRINT("[DEMOD H14] INVALID neverLockStatus !!!\n");

			break;

			}

	// Carrier LOCK status  and offset monitoring


	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_CARR_LOCK,&carrLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}


	if(carrLockState == LX_DEMOD_LOCKED)
	{

		DEMOD_PRINT("[DEMOD H14] DVB CARRIER is LOCKED  !!!\n");

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] DVB CARRIER is NOT LOCKED  !!!\n");

	}

	// Sampling frequency LOCK status and offset monitoring
	if (RET_OK != DEMOD_H14EUBB_Get_Lock( LX_DEMOD_TR_LOCK, &samplingLock))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if(LX_DEMOD_LOCKED == samplingLock)
	{
		DEMOD_PRINT("[DEMOD H14] Sampling frequency is Locked   !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] Sampling frequency is NOT Locked !!!\n");
	}


	//Symbol rate detection

	if (RET_OK != DEMOD_H14EUBB_DVBC_IsSymbolRateAuto(&isAutoSymbol))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoSymbol)
			{
		if (RET_OK != DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus(&symbolRateDet))

				{

			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

					return RET_ERROR;

		}

		switch(symbolRateDet.status)

		{

			case LX_DEMOD_IDLE :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate status is idle status!!!\n");

				break;

				}

			case LX_DEMOD_DETECTED :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate  is detected !!!\n");

				DEMOD_PRINT("[DEMOD H14] Symbol rate = %d [KHz] !!!\n",symbolRateDet.symbolRate);

				break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate detection is processing !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_FAIL :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate detection is failed !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H14] INVALID Symbol rate detection status  !!!\n");

				break;
			}



		}
	}


	// QAM mode detect status and mode monitoring
	if (RET_OK != DEMOD_H14EUBB_DVBC_IsQammodeAutoDet(&isAutoQammode))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_IsQammodeAutoDet() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoQammode)
	{

		if (RET_OK != DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus(&qamModeDet))
	{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);

			return RET_ERROR;
		}

		switch(qamModeDet.status)
		{
			case LX_DEMOD_IDLE :
		{
				DEMOD_PRINT("[DEMOD H14] QAM mode detect is idle status!!!\n");

				break;
			}

			case LX_DEMOD_DETECTED :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode is detected!!!\n");

				DEMOD_PRINT("[DEMOD H14] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :

																		 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :

																		  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );

				break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode detection is processing !!!\n");

				break;
			}

			case LX_DEMOD_DETECTION_FAIL :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode detection is failed !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_MANUAL :
			{

				DEMOD_PRINT("[DEMOD H14] Manual QAM mode detection mode !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H14] INVALID QAM mode detection  !!!\n");

				break;
			}

		}

	}


 	//spectrum inversion status
	if (RET_OK !=  DEMOD_H14EUBB_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if(LX_DEMOD_INVERSION == spectrumInv)
	{
		DEMOD_PRINT("[DEMOD H14] Spectrum Inversion !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] Spectrum Normal !!!\n");
	}

	//Frame lock status
	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}

	if(fecLockState != LX_DEMOD_LOCKED)
	{
		DEMOD_PRINT("[DEMOD H14] DVB FEC IS NOT LOCKED !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] DVB FEC LOCK OK !!!\n");
	}

 	//MSE

	if (RET_OK != DEMOD_H14EUBB_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);


 	//BER after Viterbi
	if (RET_OK != DEMOD_H14EUBB_Get_VABER(&vBer))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SNR() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] vBER = %d  !!!\n",vBer);

	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_H14EUBB_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);


#ifndef DEMOD_PRINT

	printk("[DEMOD H14] IF AGC Gain = 0x%x !!!\n",ifAGCGain);
	printk("[DEMOD H14] DVB CARRIER is %s  !!!\n",(carrLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked" );
	printk("[DEMOD H14] Carrier offset Frequency = %d [KHz] !!!\n",carrierLock.carrOffset);
	printk("[DEMOD H14] Sampling frequency is %s   !!!\n",(samplingLock == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H14] QAM mode is %s   !!!\n",(qamModeDet.status == LX_DEMOD_DETECTED) ? "detected" : "NOT detected");

	if(qamModeDet.status == LX_DEMOD_DETECTED)
	{
		printk("[DEMOD H14] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :
										            (qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :
	                                               	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :
												 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :
												  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );
	}
	printk("[DEMOD H14 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																		(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	printk("[DEMOD H14] FEC lock is %s !!!\n",(fecLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H14] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD H14] vBER = %d  !!!\n",vBer);
	printk("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);

#endif

	return RET_OK;

}


#endif


#if 1




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_IsQammodeAutoDet
 * Control automatic setting of DVBC2 config.
 *
 * @param *bAutoQam 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{


	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue
 * Set defualt register value on DVB-C2.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue(void)
{

	UINT8 cbldetrep_en;
	UINT8 tp_oen;
	UINT8 ifagc_oen;

#if 1
	UINT8 tiroton;
#endif
	cbldetrep_en = 0x0;
	DVB_Common_RdFL(common_ctrl107);   //regAddr = 0x006b,COMMON_CTRL[107]
	DVB_Common_Wr01(common_ctrl107,cbldetrep_en,cbldetrep_en);
	DVB_Common_WrFL(common_ctrl107);	//regAddr = 0x006b,COMMON_CTRL[107]

	DVB_Common_Wr(common_ctrl109,0x20);
	DVB_Common_WrFL(common_ctrl109);

	tp_oen = 0x0;
	ifagc_oen = 0x0;
	DVB_Common_RdFL(common_ctrl110);   // regAddr = 0x006e,COMMON_CTRL[110]
	DVB_Common_Wr02(common_ctrl110,tp_oen,tp_oen, ifagc_oen, ifagc_oen);
	DVB_Common_WrFL(common_ctrl110);	  // regAddr = 0x006e,COMMON_CTRL[110]

#if 1
	tiroton = 1;
	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
	T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
	T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);
#endif
	_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_Config_auto (BOOLEAN bAutoDetect)
{

	UINT8 useopm;
	UINT8 opm_1_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(bAutoDetect == TRUE)
	{
		useopm = 0; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		DVB_Common_RdFL(common_ctrl1);  //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr01(common_ctrl1,useopm,useopm); //USEOPM
		DVB_Common_WrFL(common_ctrl1);

	}
	else
	{
		useopm = 1; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		opm_1_0 = 2;

		DVB_Common_RdFL(common_ctrl1);  //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr02(common_ctrl1,useopm,useopm,opm_1_0,opm_1_0 ); //USEOPM, OPM[1:0]
		DVB_Common_WrFL(common_ctrl1);
		// OPM[1:0] : Set operation mode manually.
		// 00 : DVB-T2 mode
		// 01 : DVB-T mode
		// 10 : DVB-C2 mode
		// 11 : DVB-C mode

	}

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_H14EUBB_DVBC2_Set_Config_auto is enabled.
 *
 * @param *pDvbt2ConfigParam 	[IN] LX_DEMOD_DVBC2_CONFIG_T - DVB-C2 config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_PartialConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam)
{


	UINT8 	c2_dplp_id_7_0;
	UINT8 	c2_ds0_id_7_0;
	UINT8 	n_sftrst_tp;
	UINT8 	c2_nplprst;


	UINT8	setPLPId;
	UINT8	setSliceId;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//PLP ID selection
	setPLPId = pDvbc2ConfigParam->unPLP;
	c2_dplp_id_7_0 = setPLPId;
	DEMOD_PRINT("[DEMOD H14] PLP ID is selected  !!!\n");

	C2_PLP1_RdFL(c2_plp_ctrl6);   //regAddr = 0x4206, C2_PLP_CTRL[6]
	C2_PLP1_Wr01(c2_plp_ctrl6, c2_dplp_id_7_0, c2_dplp_id_7_0); //C2_dPLP_ID[7:0] :Data PLP ID to be decoded
	C2_PLP1_WrFL(c2_plp_ctrl6);

	//SLICE ID selection
	setSliceId = pDvbc2ConfigParam->sliceID;
	c2_ds0_id_7_0 = setSliceId;
	DEMOD_PRINT("[DEMOD H14] SLICE ID is selected  !!!\n");

	C2_PLP1_RdFL(c2_plp_ctrl5);   //regAddr = 0x4205,C2_PLP_CTRL[5]
	C2_PLP1_Wr01(c2_plp_ctrl5, c2_ds0_id_7_0, c2_ds0_id_7_0); //C2_DS0_ID[7:0] :ID of DS0
	C2_PLP1_WrFL(c2_plp_ctrl5);


	//TP reset
	n_sftrst_tp = 0;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

	//C2 PLP reset
	c2_nplprst = 0;
	C2_PLP1_RdFL(c2_plp_ctrl1);   //regAddr = 0x4201, C2_PLP_CTRL[1]
	C2_PLP1_Wr01(c2_plp_ctrl1, c2_nplprst, c2_nplprst);
	C2_PLP1_WrFL(c2_plp_ctrl1);
	// C2_nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset


	OS_MsecSleep(10);

	n_sftrst_tp = 1;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

	c2_nplprst = 1;
	C2_PLP1_RdFL(c2_plp_ctrl1);   //regAddr = 0x4201, C2_PLP_CTRL[1]
	C2_PLP1_Wr01(c2_plp_ctrl1, c2_nplprst, c2_nplprst);
	C2_PLP1_WrFL(c2_plp_ctrl1);
	// C2_nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_FullConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_H14EUBB_DVBC2_Set_Config_auto is enabled.
 *
 * @param *pDvbt2ConfigParam 	[IN] LX_DEMOD_DVBC2_CONFIG_T - DVB-C2 config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_FullConfig (LX_DEMOD_DVBC2_CONFIG_T *pDvbc2ConfigParam)
{


	UINT8 	c2_dplp_id_7_0;
	UINT8 	c2_ds0_id_7_0;
	UINT8 	n_sftrst_tp;
	UINT8 	c2_nplprst;


	UINT8	setPLPId;
	UINT8	setSliceId;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//PLP ID selection
	setPLPId = pDvbc2ConfigParam->unPLP;
	c2_dplp_id_7_0 = setPLPId;
	DEMOD_PRINT("[DEMOD H14] PLP ID is selected  !!!\n");

	C2_PLP1_RdFL(c2_plp_ctrl6);   //regAddr = 0x4206, C2_PLP_CTRL[6]
	C2_PLP1_Wr01(c2_plp_ctrl6, c2_dplp_id_7_0, c2_dplp_id_7_0); //C2_dPLP_ID[7:0] :Data PLP ID to be decoded
	C2_PLP1_WrFL(c2_plp_ctrl6);

	//SLICE ID selection
	setSliceId = pDvbc2ConfigParam->sliceID;
	c2_ds0_id_7_0 = setSliceId;
	DEMOD_PRINT("[DEMOD H14] SLICE ID is selected  !!!\n");

	C2_PLP1_RdFL(c2_plp_ctrl5);   //regAddr = 0x4205,C2_PLP_CTRL[5]
	C2_PLP1_Wr01(c2_plp_ctrl5, c2_ds0_id_7_0, c2_ds0_id_7_0); //C2_DS0_ID[7:0] :ID of DS0
	C2_PLP1_WrFL(c2_plp_ctrl5);


	//TP reset
	n_sftrst_tp = 0;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

	//C2 PLP reset
	c2_nplprst = 0;
	C2_PLP1_RdFL(c2_plp_ctrl1);   //regAddr = 0x4201, C2_PLP_CTRL[1]
	C2_PLP1_Wr01(c2_plp_ctrl1, c2_nplprst, c2_nplprst);
	C2_PLP1_WrFL(c2_plp_ctrl1);
	// C2_nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset


	OS_MsecSleep(10);

	n_sftrst_tp = 1;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

	c2_nplprst = 1;
	C2_PLP1_RdFL(c2_plp_ctrl1);   //regAddr = 0x4201, C2_PLP_CTRL[1]
	C2_PLP1_Wr01(c2_plp_ctrl1, c2_nplprst, c2_nplprst);
	C2_PLP1_WrFL(c2_plp_ctrl1);
	// nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param SymbolRate 	[IN] UINT16 - symbol rate : unit: KHz.
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_QamMode (UINT8 constel)
{


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_NeverLockWaitTime(UINT32 waitMs)
{



	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Set_StartFrequency
 * Set start frequency.
 *
 * @param waitMs 	[IN] UINT32 - unit: KHz
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Set_StartFrequency(UINT32 frequency)
{

	UINT32 freqMHz;
	UINT32 tempFreq100Hz;
	UINT32 calculatedFreq_1;
	UINT32 calculatedFreq_2;

	UINT32 finalValueForTimesOf12;
	UINT32 finalValueForTimesOf24;

	UINT8 c2_startsc_g128_23_16;
	UINT8 c2_startsc_g128_15_8;
	UINT8 c2_startsc_g128_7_0;

	UINT8 c2_startsc_g64_23_16;
	UINT8 c2_startsc_g64_15_8;
	UINT8 c2_startsc_g64_7_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	freqMHz = frequency / 1000;
	tempFreq100Hz = frequency / 100;


	calculatedFreq_1 = (448*freqMHz - 1704) / 12;  //448 = 64 * 7  %12, %24 각각 셋팅.
	calculatedFreq_2 = (448*tempFreq100Hz - 17040)/ 12;  // %12, %24 각각 셋팅.


	if((calculatedFreq_2 - (calculatedFreq_1 * 10)) < 5)
	{
		finalValueForTimesOf12 = calculatedFreq_1 * 12;
	}
	else
	{
		finalValueForTimesOf12 = (calculatedFreq_1 + 1) * 12;

	}

	calculatedFreq_1 = (448*freqMHz - 1704) / 24;  //448 = 64 * 7  %12, %24 각각 셋팅.
	calculatedFreq_2 = (448*tempFreq100Hz - 17040)/ 24;  // %12, %24 각각 셋팅.

	if((calculatedFreq_2 - (calculatedFreq_1 * 10)) < 5)
	{
		finalValueForTimesOf24 = calculatedFreq_1 * 24;
	}
	else
	{
		finalValueForTimesOf24 = (calculatedFreq_1 + 1) * 24;

	}
	DEMOD_PRINT("[DEMOD H14] frequency = %d [KHz]!!!\n",frequency);
	DEMOD_PRINT("[DEMOD H14] finalValueForTimesOf12 = %d !!!\n",finalValueForTimesOf12);
	DEMOD_PRINT("[DEMOD H14] finalValueForTimesOf24 = %d !!!\n",finalValueForTimesOf24);

// GI : 1/128
	c2_startsc_g128_23_16 = (finalValueForTimesOf24 >> 16) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl0,c2_startsc_g128_23_16,c2_startsc_g128_23_16); //regAddr = 0x1500, C2_POST_SYNC_CTRL[0],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl0);
	// C2_STARTSC_G128[23:16] : C2 Start Subcarrier index(Default : 0x494DC = 300252)

	c2_startsc_g128_15_8 = (finalValueForTimesOf24 >> 8) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl1,c2_startsc_g128_15_8,c2_startsc_g128_15_8); //regAddr = 0x1501, C2_POST_SYNC_CTRL[1],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl1);
	// C2_STARTSC_G128[15:8] : C2 Start Subcarrier index(Default : 0x494DC = 300252)

	c2_startsc_g128_7_0 = (finalValueForTimesOf24 >> 0) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl2,c2_startsc_g128_7_0,c2_startsc_g128_7_0); //regAddr = 0x1502, C2_POST_SYNC_CTRL[2],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl2);
	// C2_STARTSC_G128[7:0]: C2 Start Subcarrier index(Default : 0x494DC = 300252)

// GI : 1/64
	c2_startsc_g64_23_16 = (finalValueForTimesOf12 >> 16) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl3,c2_startsc_g64_23_16,c2_startsc_g64_23_16); //regAddr = 0x1503, C2_POST_SYNC_CTRL[3],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl3);
	// C2_STARTSC_G64[23:16] : C2 Start Subcarrier index(Default : 0x494DC = 300252)

	c2_startsc_g64_15_8 = (finalValueForTimesOf12 >> 8) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl4,c2_startsc_g64_15_8,c2_startsc_g64_15_8); //regAddr = 0x1504, C2_POST_SYNC_CTRL[4],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl4);
	// C2_STARTSC_G64[15:8] : C2 Start Subcarrier index(Default : 0x494DC = 300252)

	c2_startsc_g64_7_0 = (finalValueForTimesOf12 >> 0) & 0xff;
	C2_Post_Sync_Wr01(c2_post_sync_ctrl5,c2_startsc_g64_7_0,c2_startsc_g64_7_0); //regAddr = 0x1505, C2_POST_SYNC_CTRL[5],
	C2_Post_Sync_WrFL(c2_post_sync_ctrl5);
	// C2_STARTSC_G64[7:0]: C2 Start Subcarrier index(Default : 0x494DC = 300252)


	return RET_OK;


	}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C2
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{



	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBC2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	LX_DEMOD_OPER_MODE_T 			operMode;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_StartFrequency(pDemodInfo->setCfgParam.dvbc2.frequency))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_StartFrequency() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_NeverLockWaitTime(LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc2.tuneMode)
				{
					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


				}
				else
				{

#if 0
					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbc2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_DVBC2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC2_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbc2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_H14EUBB_DVBC2_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif
				}

				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_RETUNE_RF : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{


				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc2.tuneMode)
				{

					if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD H14] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);

					}
					if(LX_DEMOD_DVBC == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBC;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING; //LX_DEMOD_RETUNE_RF;
						pDemodInfo->setCfgParam.dvbc.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
						pDemodInfo->setCfgParam.dvbc.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBC)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}
					else //(LX_DEMOD_DVBC2 == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBC2;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBC2)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}


				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_NewTuneReqeustTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBC_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);

				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;



													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;




											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;

				if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);

				}
				DEMOD_PRINT("[DEMOD H14] LX_DEMOD_SYNC_CHECKING() operMode = 0x%x!!!\n", operMode);

				if(LX_DEMOD_DVBC == operMode)
				{
					pDemodInfo->operMode = LX_DEMOD_DVBC;
					pDemodInfo->setCfgParam.dvbc.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
					pDemodInfo->setCfgParam.dvbc.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;

					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue())
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING(DVBC2) : Going to LX_DEMOD_SYNC_CHECKING(DVBC forcely)..... ^0^\n");

					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_L1_DONE, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
				 	/* nothing to do so far */
					DEMOD_PRINT( "^R^[DEMOD H14](%s) LX_DEMOD_SYNC_CHECKING : NOT LOCKED \n", __F__);

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbc2.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
								if(packetError < LX_DEMOD_PACKET_ERROR_THD)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

								}
								else
								{
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;


						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);



					}

					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}



				}
				else
				{
//					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
//					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBC2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_OPER_MODE_T operMode;

	if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if((operMode == LX_DEMOD_DVBC) || (operMode == LX_DEMOD_DVBC2))
	{
		if(operMode != LX_DEMOD_DVBC2)
		{
			pDemodInfo->operMode = LX_DEMOD_DVBC;
			pDemodInfo->setCfgParam.dvbc.tuneMode = pDemodInfo->setCfgParam.dvbc.tuneMode;
			pDemodInfo->setCfgParam.dvbc.channelBW = pDemodInfo->setCfgParam.dvbc.channelBW;
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) forcely changed to DVBC mode \n",__F__, __L__);

			return RET_OK;
		}

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild operation mode !! \n",__F__, __L__);
		return RET_ERROR;

	}

	if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= 160)
		{
			_gH14EUBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT("[DEMOD H14] (%s) _gH14EUBB_DVB_UnlockCount_full_packeterr = %d  !!!\n", __F__,_gH14EUBB_DVB_UnlockCount_full_packeterr );

			if(_gH14EUBB_DVB_UnlockCount_full_packeterr == gH14EUBB_FullPkerrCount)
			{
				DEMOD_PRINT("[DEMOD H14] ooops !! (%s) Force to NOT LOCK  !!!\n", __F__);
				_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

				/* software Reset for DVB channel decoder */
				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				DEMOD_PRINT("[DEMOD H14] EXCUTE, GBB_DVB_SoftwareReset() !!!\n");

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
			}
		}
		else
		{

			_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)	&& (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

		}

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}


	}
	else
	{
		pDemodInfo->lockCount = 0;
		_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBC_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
#if 0
			if(RET_OK != DEMOD_H14EUBB_NeverLock_Infinite_Scan(operMode, TRUE))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverLock_Infinite_Scan() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
#endif

			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= FALSE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xff;

//			pDemodInfo->setCfgParam.dvbc2.tuneMode	= LX_DEMOD_TUNE_AUTO;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_2500;


		}



	}


	return RET_OK;



}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Get_multiPLP_ID
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBC2_MULTI_PLP_INFO_T .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Get_multiPLP_ID( LX_DEMOD_DVBC2_MULTI_PLP_INFO_T *pPlpParams)
{
	UINT8 	t2_num_plp_7_0;

	UINT8 	plpCount = 0;
	UINT16 	plpDataStart = 0x4100;

	UINT16  plpDataTypeStart = 0x4080;

	UINT8  tempPLPID = 0;
	UINT8  tempType = 0;


	LX_DEMOD_DVBC2_PLP_BYTE_T tempType1 = LX_DEMOD_DVBC2_PLP_UNKNOWN;
	LX_DEMOD_DVBC2_PLP_BYTE_T tempType2 = LX_DEMOD_DVBC2_PLP_UNKNOWN;
	LX_DEMOD_DVBC2_PLP_BYTE_T currType = LX_DEMOD_DVBC2_PLP_UNKNOWN;



	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	memset(pPlpParams, 0x00, sizeof(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T));

	T2_PLP1_RdFL(t2_plp_mon50);  //regAddr = 0x4042, T2_PLP_MON[50]
	T2_PLP1_Rd01(t2_plp_mon50,t2_num_plp_7_0,t2_num_plp_7_0);

	pPlpParams->plpTotalCount = t2_num_plp_7_0;

	for(plpCount = 0; plpCount < t2_num_plp_7_0; plpCount++  )
	{
		if (RET_OK != DEMOD_I2C_Read( plpDataStart, 1, &tempPLPID))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_I2C_Read !!!\n", __F__, __L__);
			return RET_ERROR;
		}

		if(!(plpCount & 0x1))
		{
			if (RET_OK != DEMOD_I2C_Read( plpDataTypeStart, 1, &tempType))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_I2C_Read !!!\n", __F__, __L__);
				return RET_ERROR;
			}
			else
			{
				if(((tempType >> 4) & 0x7) == 0) 	tempType1 	= LX_DEMOD_DVBC2_PLP_COMMON;
				else 					   			tempType1 	= LX_DEMOD_DVBC2_PLP_DATA;

				if(((tempType >> 0) & 0x7) == 0) 	tempType2	= LX_DEMOD_DVBC2_PLP_COMMON;
				else					  			tempType2	= LX_DEMOD_DVBC2_PLP_DATA;

			}
			currType = tempType1;
                        plpDataTypeStart++;
		}
		else
		{
			DEMOD_PRINT("[DEMOD H14][Before]type1=0x%x, type2 = 0x%x,  currType = 0x%x!!!\n", tempType1, tempType2,currType );
			currType = tempType2;
			DEMOD_PRINT("[DEMOD H14][After] type1=0x%x, type2 = 0x%x,  currType = 0x%x!!!\n", tempType1, tempType2,currType );
		}

		if(LX_DEMOD_DVBC2_PLP_COMMON == currType)
		{
			pPlpParams->commonPlpID[pPlpParams->commonPlpCount] = tempPLPID;
			pPlpParams->commonPlpCount++;

		}
		else if(LX_DEMOD_DVBC2_PLP_DATA == currType)
		{
			pPlpParams->plpDataID[pPlpParams->plpDatacount] = tempPLPID;
			pPlpParams->plpDatacount++;

		}
		else
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild PLP type !!!\n", __F__, __L__);

		}
		plpDataStart++;


		DEMOD_PRINT("[DEMOD H14]  plpInfo[%d].plpDataID = 0x%x !!!\n", plpCount,tempPLPID);

	}

	DEMOD_PRINT("[DEMOD H14] PLP TotalCount = %d  !!!\n",pPlpParams->plpTotalCount);
	DEMOD_PRINT("[DEMOD H14] PLP Datacount = %d  !!!\n",pPlpParams->plpDatacount);
	DEMOD_PRINT("[DEMOD H14] PLP CommonPlpCount = %d  !!!\n",pPlpParams->commonPlpCount);

	for(plpCount=0; plpCount < pPlpParams->plpDatacount; plpCount++)
	{
		DEMOD_PRINT("[DEMOD H14] PLP Data ID[%d] = 0x%x \n",plpCount, pPlpParams->plpDataID[plpCount]);

	}
	for(plpCount=0; plpCount < pPlpParams->commonPlpCount; plpCount++)
	{
		DEMOD_PRINT("[DEMOD H14] PLP Common PLP ID[%d] = 0x%x \n",plpCount, pPlpParams->commonPlpID[plpCount]);

	}

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBC2_Get_DvbInfo
 * Get all information on DVB-C2 for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBC2_Get_DvbInfo(void)

{

	UINT32 errorCount;
	UINT32 vBer;

	UINT16 	ifAGCGain = 0;
	LX_DEMOD_MSE_T sigMse;

	BOOLEAN isAutoSymbol = FALSE;
	BOOLEAN isAutoQammode = FALSE;

	LX_DEMOD_SYMBOLRATE_STATE_T symbolRateDet;
	LX_DEMOD_QAMMODE_STATE_T qamModeDet;

	LX_DEMOD_LOCK_STATE_T neverLockStatus;

	LX_DEMOD_LOCK_STATE_T samplingLock;

	LX_DEMOD_LOCK_STATE_T carrLockState;
	LX_DEMOD_LOCK_STATE_T fecLockState;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_TPERRCNTMODE_T errorMode;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	memset(&qamModeDet, 0x00, sizeof(LX_DEMOD_QAMMODE_STATE_T));

	//IF AGC
	if (RET_OK != DEMOD_H14EUBB_Get_IFAGC(&ifAGCGain))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_IFAGC() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] IF AGC Gain = 0x%x !!!\n",ifAGCGain);

 	//NeverLock status
	if (RET_OK != DEMOD_H14EUBB_Get_NeverLockStatus(LX_DEMOD_DVBC, &neverLockStatus))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_NeverLockStatus() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	switch(neverLockStatus)
	{

		case LX_DEMOD_IDLE :
			DEMOD_PRINT("[DEMOD H14] Initialization process  !!!\n");
			break;

		case LX_DEMOD_DETECTION_PROCESSING :
			DEMOD_PRINT("[DEMOD H14] Signal Checking process !!!\n");
			break;

		case LX_DEMOD_DETECTED :
			DEMOD_PRINT("[DEMOD H14] normal State (available channel)  !!!\n");
			break;

		case LX_DEMOD_DETECTION_FAIL :
			DEMOD_PRINT("[DEMOD H14] neverlock state (empty channel) !!!\n");

			break;

		default :

			DEMOD_PRINT("[DEMOD H14] INVALID neverLockStatus !!!\n");

			break;

	}

	// Carrier LOCK status  and offset monitoring


	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_CARR_LOCK,&carrLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}


	if(carrLockState == LX_DEMOD_LOCKED)
	{

		DEMOD_PRINT("[DEMOD H14] DVB CARRIER is LOCKED  !!!\n");

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] DVB CARRIER is NOT LOCKED  !!!\n");

	}

	// Sampling frequency LOCK status and offset monitoring
	if (RET_OK != DEMOD_H14EUBB_Get_Lock( LX_DEMOD_TR_LOCK, &samplingLock))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if(LX_DEMOD_LOCKED == samplingLock)
	{
		DEMOD_PRINT("[DEMOD H14] Sampling frequency is Locked   !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] Sampling frequency is NOT Locked !!!\n");
	}


	//Symbol rate detection

	if (RET_OK != DEMOD_H14EUBB_DVBC_IsSymbolRateAuto(&isAutoSymbol))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoSymbol)
	{
		if (RET_OK != DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus(&symbolRateDet))

		{

			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

			return RET_ERROR;

		}

		switch(symbolRateDet.status)

		{

			case LX_DEMOD_IDLE :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate status is idle status!!!\n");

				break;

			}

			case LX_DEMOD_DETECTED :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate  is detected !!!\n");

				DEMOD_PRINT("[DEMOD H14] Symbol rate = %d [KHz] !!!\n",symbolRateDet.symbolRate);

				break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate detection is processing !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_FAIL :

			{

				DEMOD_PRINT("[DEMOD H14] Symbol rate detection is failed !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H14] INVALID Symbol rate detection status  !!!\n");

				break;
			}



		}
	}


	// QAM mode detect status and mode monitoring
	if (RET_OK != DEMOD_H14EUBB_DVBC_IsQammodeAutoDet(&isAutoQammode))
	{

		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_IsQammodeAutoDet() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoQammode)
	{

		if (RET_OK != DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus(&qamModeDet))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);

			return RET_ERROR;
		}

		switch(qamModeDet.status)
		{
			case LX_DEMOD_IDLE :
			{
				DEMOD_PRINT("[DEMOD H14] QAM mode detect is idle status!!!\n");

				break;
			}

			case LX_DEMOD_DETECTED :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode is detected!!!\n");

				DEMOD_PRINT("[DEMOD H14] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :

																		 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :

																		  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );

			break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode detection is processing !!!\n");

				break;
			}

			case LX_DEMOD_DETECTION_FAIL :
			{

				DEMOD_PRINT("[DEMOD H14] QAM mode detection is failed !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_MANUAL :
			{

				DEMOD_PRINT("[DEMOD H14] Manual QAM mode detection mode !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H14] INVALID QAM mode detection  !!!\n");

				break;
			}

		}

	}


 	//spectrum inversion status
	if (RET_OK !=  DEMOD_H14EUBB_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if(LX_DEMOD_INVERSION == spectrumInv)
	{
		DEMOD_PRINT("[DEMOD H14] Spectrum Inversion !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] Spectrum Normal !!!\n");
	}

	//Frame lock status
	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}

	if(fecLockState != LX_DEMOD_LOCKED)
	{
		DEMOD_PRINT("[DEMOD H14] DVB FEC IS NOT LOCKED !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] DVB FEC LOCK OK !!!\n");
	}

 	//MSE

	if (RET_OK != DEMOD_H14EUBB_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);


 	//BER after Viterbi
	if (RET_OK != DEMOD_H14EUBB_Get_VABER(&vBer))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SNR() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] vBER = %d  !!!\n",vBer);

	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_H14EUBB_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);


#ifndef DEMOD_PRINT

	printk("[DEMOD H14] IF AGC Gain = 0x%x !!!\n",ifAGCGain);
	printk("[DEMOD H14] DVB CARRIER is %s  !!!\n",(carrLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked" );
	printk("[DEMOD H14] Carrier offset Frequency = %d [KHz] !!!\n",carrierLock.carrOffset);
	printk("[DEMOD H14] Sampling frequency is %s   !!!\n",(samplingLock == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H14] QAM mode is %s   !!!\n",(qamModeDet.status == LX_DEMOD_DETECTED) ? "detected" : "NOT detected");

	if(qamModeDet.status == LX_DEMOD_DETECTED)
	{
		printk("[DEMOD H14] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :
										            (qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :
	                                               	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :
												 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :
												  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );
	}
	printk("[DEMOD H14 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																		(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	printk("[DEMOD H14] FEC lock is %s !!!\n",(fecLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H14] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD H14] vBER = %d  !!!\n",vBer);
	printk("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);

#endif

	return RET_OK;

}


#endif




#if 0
#endif

/**
 * DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	UINT8 	t2tdetrep_en;
	UINT8 tp_oen;
	UINT8 ifagc_oen;
#if 1
	UINT8 tiroton;
#endif

	t2tdetrep_en = 0x0;
	DVB_Common_RdFL(common_ctrl108);   // regAddr = 0x006c,COMMON_CTRL[108]
	DVB_Common_Wr01(common_ctrl108,t2tdetrep_en,t2tdetrep_en);
	DVB_Common_WrFL(common_ctrl108);	//regAddr = 0x006c,COMMON_CTRL[108]
	//T2TDETREP_EN : T2/T Detection repeatedly( 0 : Disable, 1 : Enable)

	tp_oen = 0x0;
	ifagc_oen = 0x0;
	DVB_Common_RdFL(common_ctrl110);   // regAddr = 0x006e,COMMON_CTRL[110]
	DVB_Common_Wr02(common_ctrl110,tp_oen,tp_oen, ifagc_oen, ifagc_oen);
	DVB_Common_WrFL(common_ctrl110);	  // regAddr = 0x006e,COMMON_CTRL[110]


#if 1
	tiroton = 1;
	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
	T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
	T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);
#endif

	_gH14EUBB_DVB_UnlockCount_full_packeterr 	= 0;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_Config_auto (BOOLEAN bAutoDetect)
{
	UINT8	gim_man_en;
	UINT8	fftm_man_en;

	UINT8	useopm;
	UINT8	opm_1_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(bAutoDetect == TRUE)
	{
		useopm = 0; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr01(common_ctrl1,useopm,useopm); //USEOPM
		DVB_Common_WrFL(common_ctrl1);

	}
	else
	{
		useopm = 1; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		opm_1_0 = 1;

		DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr02(common_ctrl1,useopm,useopm,opm_1_0,opm_1_0 ); //USEOPM, OPM[1:0]
		DVB_Common_WrFL(common_ctrl1);
		// OPM[1:0] : Set operation mode manually.
		//00 : DVB-T2 mode
		// 01 : DVB-T mode
		// 10 : DVB-C2 mode
		// 11 : DVB-C mode

	}

	gim_man_en = (bAutoDetect == TRUE)?(0):(1); 	//GIM_MAN_EN
	fftm_man_en = (bAutoDetect == TRUE)?(0):(1);	//FFTM_MAN_EN
	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl45);  //regAddr = 0x102d, T2_PRE_SYNC_CTRL[45]
	T2_Pre_Sync_Wr02(t2_pre_sync_ctrl45, gim_man_en, gim_man_en, fftm_man_en,fftm_man_en);
	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl45);

	//GIM_MAN_EN : Enable setting GI mode manually
	// 0 : Disable
	// 1 : Enable

	//FFTM_MAN_EN : Enable setting FFT mode manually
	// 0 : Disable
	// 1 : Enable

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{

	UINT8 	hpsel;

	BOOLEAN 			bSetHp;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	bSetHp = pDvbtConfigParam->bSetHp;
	if(bSetHp)
	{
		hpsel = 0x1;
		DEMOD_PRINT("[DEMOD H14] High Priority signal is selected  !!!\n");
	}
	else
	{
		hpsel = 0x0;
		DEMOD_PRINT("[DEMOD H14] Low Priority signal is selected  !!!\n");
	}

	TC_FEC_RdFL(tc_fec_ctrl0);   //regAddr = 0x5800, TC_FEC_CTRL[0]
	TC_FEC_Wr01(tc_fec_ctrl0, hpsel, hpsel); //HPSEL : '0'->Low priority, '1'-> High priority
	TC_FEC_WrFL(tc_fec_ctrl0);
	//HPSEL :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{

	UINT8 		hpsel;
	BOOLEAN 	bSetHp;

	UINT8	gim_man_2_0;
	UINT8	fftm_man_2_0;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	bSetHp = pDvbtConfigParam->bSetHp;
	if(bSetHp)
	{
		hpsel = 0x1;
		DEMOD_PRINT("[DEMOD H14] High Priority signal is selected  !!!\n");
	}
	else
	{
		hpsel = 0x0;
		DEMOD_PRINT("[DEMOD H14] Low Priority signal is selected	!!!\n");
	}

	TC_FEC_RdFL(tc_fec_ctrl0);   //regAddr = 0x5800, TC_FEC_CTRL[0]
	TC_FEC_Wr01(tc_fec_ctrl0, hpsel, hpsel); //HPSEL : '0'->Low priority, '1'-> High priority
	TC_FEC_WrFL(tc_fec_ctrl0);
	//HPSEL :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority

	//FFT mode manual set
	switch(pDvbtConfigParam->FFTMode)
	{
		case LX_DEMOD_DVBT_ISDBT_FFT_2K :
			{
				fftm_man_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] Set FFT Mode = %s !!!\n","2K ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_FFT_8K :
			{
				fftm_man_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set FFT Mode = %s !!!\n","8K");
			}
			break;

		default :
			{
				fftm_man_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d)UNKNOWN FFT mode !!!\n",__F__, __L__);
			}
			return RET_ERROR;
	}

	//GI(Guard Interval) mode manual set
	switch(pDvbtConfigParam->gIMode)
	{
		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_4 :
			{
				gim_man_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_4 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_8 :
			{
				gim_man_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_8 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_16 :
			{
				gim_man_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_16 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_32 :
			{
				gim_man_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_32 ");
			}
			break;
		default :
			{
				gim_man_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN Guard Interval  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl45);	 //regAddr = 0x102d, T2_PRE_SYNC_CTRL[45]
	T2_Pre_Sync_Wr02(t2_pre_sync_ctrl45, gim_man_2_0, gim_man_2_0, fftm_man_2_0,fftm_man_2_0);
	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl45);

	//GIM_MAN[2:0] : GI mode
	// 000 : 1/32
	// 001 : 1/16
	// 010 : 1/8
	// 011 : 1/4
	// 100 : 1/128
	// 101 : 19/128
	// 110 : 19/256
	// 111 : 1/64

	//FFTM_MAN[2:0] : FFT mode
	// 000 : FFT 2K
	// 001 : FFT 8K
	// 010 : FFT 4K
	// 011 : FFT 1K
	// 100 : FFT 16K
	// 101 : FFT 32K


	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_NeverLockWaitTime(UINT32 waitMs)
{

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Set_GIDectectCtrl
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Set_GIDectectCtrl(void)
{

	UINT8 	gim_1_0;
	UINT8 	detrfbw_2_0;

	UINT8 	clkscale_14_8;
	UINT8 	clkscale_7_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);


	T_Post_Sync_RdFL(t_post_sync_mon2);	 //regAddr = 0x1382, T_POST_SYNC_MON[2]
	T_Post_Sync_Rd01(t_post_sync_mon2, gim_1_0, gim_1_0);

	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl0);  //regAddr = 0x2400, T2TC2_Eq_TI_CTRL[0]
	T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_ctrl0,detrfbw_2_0,detrfbw_2_0);
	//DETRFBW[2:0] : Detected RF bandwidth indication
	// 000 : 8MHz
	// 001 : 7MHz
	// 010 : 6MHz
	// 011 : 5MHz
	// 100 : 1.7MHz
	// 101 ~ 111 : reserved

	if(gim_1_0 == 0)
	{
		if(detrfbw_2_0 == 0)
		{
			clkscale_14_8 = 0x72;
			clkscale_7_0 = 0x2d;

		}
		else if(detrfbw_2_0 == 1)
		{
			clkscale_14_8 = 0x63;
			clkscale_7_0 = 0xe7;

		}
		else
		{
			clkscale_14_8 = 0x55;
			clkscale_7_0 = 0xa1;
		}

	}
	else if(gim_1_0 == 1)
	{
		if(gim_1_0 == 0)
		{
			clkscale_14_8 = 0x6e;
			clkscale_7_0 = 0x1b;

		}
		else if(gim_1_0 == 1)
		{
			clkscale_14_8 = 0x60;
			clkscale_7_0 = 0x57;

		}
		else
		{
			clkscale_14_8 = 0x52;
			clkscale_7_0 = 0x93;

		}

	}
	else if(gim_1_0 == 2)
	{
		if(gim_1_0 == 0)
		{
			clkscale_14_8 = 0x68;
			clkscale_7_0 = 0xac;

		}
		else if(gim_1_0 == 1)
		{
			clkscale_14_8 = 0x5b;
			clkscale_7_0 = 0x96;

		}
		else
		{
			clkscale_14_8 = 0x4e;
			clkscale_7_0 = 0x80;

		}

	}
	else
	{
		if(gim_1_0 == 0)
		{
			clkscale_14_8 = 0x5e;
			clkscale_7_0 = 0x32;

		}
		else if(gim_1_0 == 1)
		{
			clkscale_14_8 = 0x52;
			clkscale_7_0 = 0x6c;

		}
		else
		{
			clkscale_14_8 = 0x46;
			clkscale_7_0 = 0xa5;

		}

	}

		TP_RdFL(tp_ctrl35);	 //regAddr = 0x5023, TP_CTRL[35]
		TP_Wr01(tp_ctrl35, clkscale_14_8, clkscale_14_8);
		TP_WrFL(tp_ctrl35);

		TP_RdFL(tp_ctrl36);	 //regAddr = 0x5023, TP_CTRL[35]
		TP_Wr01(tp_ctrl36, clkscale_7_0, clkscale_7_0);
		TP_WrFL(tp_ctrl36);



	return RET_OK;



}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{


	*pDelaySpread = 0;
	return RET_OK;



}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_Hierach_HPSel
 * Get the information if the hierachical HP is selected.
 *
 * @param *pBSetHp 	[OUT] BOOLEAN - HP (TRUE) / LP(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp )
{

	UINT8 	hpsel;

	*pBSetHp = TRUE;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	TC_FEC_RdFL(tc_fec_ctrl0);   //regAddr = 0x5800, TC_FEC_CTRL[0]
	TC_FEC_Rd01(tc_fec_ctrl0, hpsel, hpsel); //hpsel : '0'->Low priority, '1'-> High priority


	if(hpsel)	*pBSetHp = TRUE;
	else			*pBSetHp = FALSE;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{

	UINT8 	t2tc2_bkfftm_done;
	UINT8 	trm_1_0;

	*pTxModeLock = LX_DEMOD_UNLOCKED;
	*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	DVB_Common_RdFL(common_mon1);	//regAddr = 0x0081, COMMON_MON[1]
	DVB_Common_Rd01(common_mon1,t2tc2_bkfftm_done,t2tc2_bkfftm_done); //T2TC2_BKFFTM_DONE : FFT mode status indication.(0 : undone, 1 : done)
	if(t2tc2_bkfftm_done)
	{
		*pTxModeLock = LX_DEMOD_LOCKED;

		DEMOD_PRINT("[DEMOD H14] TX mode LOCKED !!!\n");

		T_Post_Sync_RdFL(t_post_sync_mon2);	//regAddr = 0x1382, T_POST_SYNC_MON[2]
		T_Post_Sync_Rd01(t_post_sync_mon2,trm_1_0,trm_1_0);
		// TRM[1:0] :Transmission mode.
		// 00 : 2K mode
		// 01 : 8K mode
		// 10 : see annex F
		// 11 : Reserved

		switch(trm_1_0)
		{
			case 0x00 :
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_2K;
				break;

			case 0x01:
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_8K;
				break;

			case 0x02:
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_4K;
				break;

			default :
				*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;
				break;
		}

	}
	else
	{
		*pTxModeLock = LX_DEMOD_UNLOCKED;
		DEMOD_PRINT("[DEMOD H14] TX mode is NOT LOCKED !!!\n");
	}

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{

	UINT8 	t2tc2_bkgim_done;
	UINT8 	gim_1_0;

	*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	DVB_Common_RdFL(common_mon1);	//regAddr = 0x0081, COMMON_MON[1]
	DVB_Common_Rd01(common_mon1,t2tc2_bkgim_done,t2tc2_bkgim_done); //T2TC2_BKGIM_DONE : Guard interval mode status indication.(0 : undone, 1 : done)

	if(t2tc2_bkgim_done)
	{

		DEMOD_PRINT("[DEMOD H14] GI mode is LOCKED !!!\n");

		T_Post_Sync_RdFL(t_post_sync_mon2);	//regAddr = 0x1382, T_POST_SYNC_MON[2]
		T_Post_Sync_Rd01(t_post_sync_mon2,gim_1_0,gim_1_0);
		// GIM[1:0] :Guard intervals.
	// 00 : 1/32
	// 01 : 1/16
	// 10 : 1/8
	// 11 : 1/4

		switch(gim_1_0)
	{
		case 0x00 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_32;
			break;
		case 0x01:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_16;
			break;
		case 0x02 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_8;
			break;
		case 0x03:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_4;
			break;
		default :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;
			break;
	}
	}
	else
	{
		*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

		DEMOD_PRINT("[DEMOD H14] Guard Interval mode is NOT LOCKED !!!\n");
	}

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_HierachyMode
 * Get the information of hierachical mode on DVB-T.
 *
 * @param *pHierachMode[OUT] DEMOD_H14EUBB_DVB_TPS_HIERACHY_T - alpha = none, 1, 2 and 4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode )
{

	UINT8 	hier_2_0;

	*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T_Post_Sync_RdFL(t_post_sync_mon1); //regAddr = 0x1381, T_POST_SYNC_MON[1]
	T_Post_Sync_Rd01(t_post_sync_mon1,hier_2_0,hier_2_0);
	//HIER[2:0] :Hierarchy information.
	// 000 : Non hierarchical
	// 001 : alpha = 1
	// 010 : alpha = 2
	// 011 : alpha = 4
	// 100 ~ 111 : see annex F

	switch(hier_2_0)
	{
		case 0x00 :
			*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_NONE;
			break;
		case 0x01:
			*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_1;
			break;
		case 0x02 :
			*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_2;
			break;
		case 0x03:
			*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_4;
			break;
		default :
			*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN;
			break;

	}

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_LpCoderRate
 * Get the information of LP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate )
{

	UINT8 	crl_2_0;

	*pLpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T_Post_Sync_RdFL(t_post_sync_mon2); //regAddr = 0x1382, T_POST_SYNC_MON[2]
	T_Post_Sync_Rd01(t_post_sync_mon2,crl_2_0,crl_2_0);
	//CRL[2:0]: Code rate for the LP level of modulation.
	// 000 : 1/2
	// 001 : 2/3
	// 010 : 3/4
	// 011 : 5/6
	// 100 : 7/8
	// 101 ~ 111 : reserved

	switch(crl_2_0)
	{
		case 0x00 :
			*pLpCodeRate = LX_DEMOD_CODE_1_2;
			break;
		case 0x01:
			*pLpCodeRate = LX_DEMOD_CODE_2_3;
			break;
		case 0x02 :
			*pLpCodeRate = LX_DEMOD_CODE_3_4;
			break;
		case 0x03:
			*pLpCodeRate = LX_DEMOD_CODE_5_6;
			break;
		case 0x04:
			*pLpCodeRate = LX_DEMOD_CODE_7_8;
			break;
		default :
			*pLpCodeRate = LX_DEMOD_CODE_UNKNOWN;
			break;

	}

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_HpCoderRate
 * Get the information of HP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate )
{

	UINT8 	crh_2_0;

	*pHpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T_Post_Sync_RdFL(t_post_sync_mon1); //regAddr = 0x1381, T_POST_SYNC_MON[1]
	T_Post_Sync_Rd01(t_post_sync_mon1,crh_2_0,crh_2_0);
	//CRH[2:0]: Code rate for the HP level of modulation.
	// 000 : 1/2
	// 001 : 2/3
	// 010 : 3/4
	// 011 : 5/6
	// 100 : 7/8
	// 101 ~ 111 : reserved

	switch(crh_2_0)
	{
		case 0x00 :
			*pHpCodeRate = LX_DEMOD_CODE_1_2;
			break;
		case 0x01:
			*pHpCodeRate = LX_DEMOD_CODE_2_3;
			break;
		case 0x02 :
			*pHpCodeRate = LX_DEMOD_CODE_3_4;
			break;
		case 0x03:
			*pHpCodeRate = LX_DEMOD_CODE_5_6;
			break;
		case 0x04:
			*pHpCodeRate = LX_DEMOD_CODE_7_8;
			break;
		default :
			*pHpCodeRate = LX_DEMOD_CODE_UNKNOWN;
			break;

	}


	return RET_OK;

}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_GetCellId
 * Get TPS cell ID on DVB-T.
 *
 * @param *pCellIDLength[OUT] UINT8 - cell ID length.
 * @param *cell_id[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id)
{

	UINT8	lenid_5_0;
	UINT8 	cellid_7_0;
	UINT8 	cellid_15_8;

	UINT16 tempCellID = 0;

	*pCellIDLength = 0;
	*cell_id = 0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T_Post_Sync_RdFL(t_post_sync_mon0); //regAddr = 0x1380, T_POST_SYNC_MON[0]
	T_Post_Sync_Rd01(t_post_sync_mon0,lenid_5_0,lenid_5_0);  //LENID[5:0]
	//LENID[5:0]   : TPS length indicator.
	// 010111 : when Cell Identification information is not transmitted
	// 011111 : when Cell Identification information is transmitted

	*pCellIDLength = lenid_5_0;

	T_Post_Sync_RdFL(t_post_sync_mon3); //regAddr = 0x1383, T_POST_SYNC_MON[3]
	T_Post_Sync_Rd01(t_post_sync_mon3,cellid_15_8,cellid_15_8);  //CELLID[15:8] : Cell identifier.

	tempCellID = (cellid_15_8  << 8);

	T_Post_Sync_RdFL(t_post_sync_mon4); //regAddr = 0x1384, T_POST_SYNC_MON[4]
	T_Post_Sync_Rd01(t_post_sync_mon4,cellid_7_0,cellid_7_0);   //CELLID[7:0] : Cell identifier.
	tempCellID += (cellid_7_0  << 0);

	*cell_id = tempCellID;

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_TpsInfo
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams)
{
	BOOLEAN bSetHp = TRUE;
	LX_DEMOD_BWMODE_T channelBW = LX_DEMOD_BW_UNKNOWN;
	LX_DEMOD_FFTMODE_T FFTMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN ;
	LX_DEMOD_GIMODE_T gIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;
	LX_DEMOD_DVB_TPS_HIERACHY_T hierachMode = LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN;
	LX_DEMOD_CODERATE_T lpCodeRate = LX_DEMOD_CODE_UNKNOWN;
	LX_DEMOD_CODERATE_T hpCodeRate = LX_DEMOD_CODE_UNKNOWN;
	LX_DEMOD_RF_MODE_T qamMode = LX_DEMOD_UNKOWN_MODE;

	LX_DEMOD_LOCK_STATE_T txModeLock;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(NULL == pTpsParams)
	{
		DEMOD_PRINT("[DEMOD H14] ERROR, DEMOD_H14EUBB_DVBT_CONFIG_T TpsParams is NULL !!!\n");
		return RET_ERROR;
	}

	// Get bSetHp

	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_Hierach_HPSel( &bSetHp ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_Hierach_HPSel() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->bSetHp = bSetHp;
	DEMOD_PRINT("[DEMOD H14] Set HP selection = %s !!!\n",(bSetHp == TRUE) ? " HP Mode" : " LP Mode");


	// Get channelBW

	if (RET_OK != DEMOD_H14EUBB_Get_BandWidthMode( &channelBW ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_BandWidthMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->channelBW = channelBW;
	DEMOD_PRINT("[DEMOD H14] Bandwidth = %s !!!\n",
		(channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_FFTMode( &txModeLock, &FFTMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->FFTMode = FFTMode;
	DEMOD_PRINT("[DEMOD H14] Tx mode = %s, FFTMode = %s  !!!\n",
		(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked",
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode( &gIMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->gIMode = gIMode;
	DEMOD_PRINT("[DEMOD H14] Guard Intterval = %s  !!!\n",
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );



	// Get Hierachical mode
	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_HierachyMode( &hierachMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_HierachyMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hierachMode = hierachMode;
	DEMOD_PRINT("[DEMOD H14] Hierachical Mode = %s  !!!\n",
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_LpCoderRate( &lpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_LpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->lpCodeRate = lpCodeRate;
	DEMOD_PRINT("[DEMOD H14] LP code rate = %s  !!!\n",
		(lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate

	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_HpCoderRate( &hpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_HpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hpCodeRate = hpCodeRate;
	DEMOD_PRINT("[DEMOD H14] HP code rate = %s  !!!\n",
		(hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	if (RET_OK != DEMOD_H14EUBB_Get_QAMMode(&qamMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVB_Get_QAMMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->qamMode = qamMode;
	DEMOD_PRINT("[DEMOD H14] modulation  = %s  !!!\n",
		(qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_TotalInfo
 * Get all information on DVB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_TotalInfo( void)
{

	LX_DEMOD_DVBT_CONFIG_T 		tppsParams;
	LX_DEMOD_LOCK_STATE_T 	agcLockStatus;
	LX_DEMOD_LOCK_STATE_T 	neverLockStatus;
	LX_DEMOD_LOCK_STATE_T 	txModeLock;
	LX_DEMOD_FFTMODE_T 			fftMode;

	UINT16 					delaySpread;
	UINT16 					ifagcValue = 0;
	UINT32 					errorCount;
	SINT32 					FreqOff;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_MSE_T sigMse;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);


	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_AGC_LOCK, &agcLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		agcLockStatus = LX_DEMOD_UNKOWN_STATE;
	}
	if(LX_DEMOD_LOCKED == agcLockStatus)
	{
		if (RET_OK != DEMOD_H14EUBB_Get_IFAGC(&ifagcValue))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_IFAGC() !!!\n", __F__, __L__);
			ifagcValue = 0;
		}
	}

	if (RET_OK != DEMOD_H14EUBB_Get_NeverLockStatus(LX_DEMOD_DVBT, &neverLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVB_Get_NeverLockStatus() !!!\n", __F__, __L__);
		neverLockStatus = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK !=  DEMOD_H14EUBB_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}

	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_SpectrumInvStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}
	//TP Error count
	if (RET_OK !=  DEMOD_H14EUBB_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);

	if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset( &FreqOff ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		FreqOff = 0xffff;
	}

 	//MSE

	if (RET_OK != DEMOD_H14EUBB_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14] IF AGC value = %d(0x%x) !!!\n",ifagcValue,ifagcValue);
	DEMOD_PRINT("[DEMOD H14] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_DETECTED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD H14] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD H14] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14] offset = %d [KHz]!!!\n",FreqOff);
	DEMOD_PRINT("[DEMOD H14] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	if (RET_OK != DEMOD_H14EUBB_DVBT_Get_TpsInfo( &tppsParams))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_TpsInfo() !!!\n", __F__, __L__);
		return RET_ERROR;
	}




#ifndef  DEMOD_PRINT

	printk("[DEMOD H14] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H14] IF AGC value = %d !!!\n",ifagcValue);
	printk("[DEMOD H14] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H14] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	printk("[DEMOD H14] Delay Spread = %d !!!\n",delaySpread);
	printk("[DEMOD H14] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");


	// Get bSetHp
	printk("[DEMOD H14] Set HP selection = %s !!!\n",(tppsParams->bSetHp == TRUE) ? " HP Mode" : " LP Mode");

	// Get channelBW
	printk("[DEMOD H14] Bandwidth = %s  !!!\n",
		(tppsParams.channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	printk("[DEMOD H14] FFTMode = %s  !!!\n",
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_4K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	printk("[DEMOD H14] Guard Intterval = %s  !!!\n",
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );

	// Get Hierachical mode
	tppsParams->hierachMode = hierachMode;
	printk("[DEMOD H14] Hierachical Mode = %s  !!!\n",
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	printk("[DEMOD H14] LP code rate = %s  !!!\n",
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate
	printk("[DEMOD H14] HP code rate = %s  !!!\n",
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	printk("[DEMOD H14] LP code rate = %s  !!!\n",
		(tppsParams.qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(tppsParams.qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(tppsParams.qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );

	printk("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);
	printk("[DEMOD H14] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);

#endif

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_IFO_LOCK
 * Get IFO lock on DVB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{

#if 0
	UINT8	syslock;
	UINT16 	intfreqoffsetvalue = 0;
	UINT8	crifo_di_7_0;
	UINT8	crifo_di_9_8;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,syslock,syslock); //SYSLOCK : System lock is come from ifolock for DVB-T, from  framelock for DVB-C


	Sync_RdFL(sync_mon36);  //regAddr = 0x10a4,SYNC_MON[36]
	Sync_Rd01(sync_mon36,crifo_di_9_8,crifo_di_9_8); //CRIFO_DI[9:8] : Indicates IFO offset

	Sync_RdFL(sync_mon37);  //regAddr = 0x10a5, SYNC_MON[37]
	Sync_Rd01(sync_mon37,crifo_di_7_0,crifo_di_7_0); //CRIFO_DI[7:0] : Indicates IFO offset

	intfreqoffsetvalue = (crifo_di_9_8 << 8) + crifo_di_7_0;

	DEMOD_PRINT("[DEMOD H14](%s) IFO LOCK = %s !!!\n", __F__, (syslock == 1) ? "LOCK" :"NOT LOCK");
	DEMOD_PRINT("[DEMOD H14](%s) IFO value = %d !!!\n", __F__,intfreqoffsetvalue);

	if(syslock == 1)
	{
		*pIfoLock = TRUE;
	}
	else
	{
		*pIfoLock = FALSE;
}
#endif
	*pIfoLock = TRUE;

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on DVB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{

#if 0
	UINT8 aiccrejbusy_3_0;
	UINT8 aiccrejstatus_3_0;

	Common_RdFL(common_mon16); //regAddr = 0x0090, COMMON_MON[16]
	Common_Rd02(common_mon16,aiccrejbusy_3_0,aiccrejbusy_3_0, aiccrejstatus_3_0,aiccrejstatus_3_0);
	//AICCREJSTATUS[3:0] : AICC rejector control status indication
		//AICCREJSTATUS[0] : for 1st rejector
		//AICCREJSTATUS[1] : for 2nd rejector
		//AICCREJSTATUS[2] : for 3rd rejector
		//AICCREJSTATUS[3] : for 4th rejector
	// 0 : off
	// 1 : on

	//AICCREJBUSY[3:0] : AICC rejector operation status indication
		//AICCREJSTATUS[0] : for 1st rejector
		//AICCREJSTATUS[1] : for 2nd rejector
		//AICCREJSTATUS[2] : for 3rd rejector
		//AICCREJSTATUS[3] : for 4th rejector
	// 0 : off
	// 1 : operation status

	if((aiccrejbusy_3_0 != 0x0) || (aiccrejstatus_3_0 != 0x0))
	{
		*pDetStatus = TRUE;
	}
	else
	{
		*pDetStatus = FALSE;

	}
#endif
	*pDetStatus = FALSE;

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT_EqualizereReset(void)
{


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	LX_DEMOD_OPER_MODE_T 			operMode;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue(pDemodInfo->setCfgParam.dvbt.channelBW))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_H14EUBB_DVBT_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
				{
					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{

#if 0
					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif

				}

				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
				{

					if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					notifyinglockTimeOut	= LX_DEMOD_LOCK_DEALY_4000;
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}


			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD H14] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);

					}
					if(LX_DEMOD_DVBT2 == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBT2;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING; //LX_DEMOD_RETUNE_RF;
						pDemodInfo->setCfgParam.dvbt2.tuneMode = pDemodInfo->setCfgParam.dvbt.tuneMode;
						pDemodInfo->setCfgParam.dvbt2.channelBW = pDemodInfo->setCfgParam.dvbt.channelBW;
//						pDemodInfo->setCfgParam.dvbt2.unPLP = 0;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBT2)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}
					else //(LX_DEMOD_DVBT == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBT;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (LX_DEMOD_DVBT)  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}


				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;



					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_NewTuneReqeustTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBT_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);



				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;




													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;




											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;

				if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);

				}
				DEMOD_PRINT("[DEMOD H14] LX_DEMOD_SYNC_CHECKING() operMode = 0x%x!!!\n", operMode);

				if(LX_DEMOD_DVBT2 == operMode)
				{
					pDemodInfo->operMode = LX_DEMOD_DVBT2;
					pDemodInfo->setCfgParam.dvbt2.tuneMode = pDemodInfo->setCfgParam.dvbt.tuneMode;
					pDemodInfo->setCfgParam.dvbt2.channelBW = pDemodInfo->setCfgParam.dvbt.channelBW;

					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue(pDemodInfo->setCfgParam.dvbt2.channelBW))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
#if 0

					if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_StartPLP_ID(&startPlpID ))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Get_StartPLP_ID() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					pDemodInfo->setCfgParam.dvbt2.unPLP = startPlpID;
#endif
					pDemodInfo->setCfgParam.dvbt2.unPLP = 0;

					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt2)))
					{
						DEMOD_PRINT("[DEMOD] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING :Set PLP ID to 0x%x..... \n",pDemodInfo->setCfgParam.dvbt2.unPLP);

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING(DVBT) : Going to LX_DEMOD_SYNC_CHECKING(DVBT2 forcely)..... ^0^\n");

					return RET_OK;
				}

#if 0
				if(RET_OK != DEMOD_H14EUBB_Set_TirotCtrl())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Set_TirotCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_TPS_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
				 	/* nothing to do so far */

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

//				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_TPS_LOCK, &lockState))
#if 1
				if(RET_OK != DEMOD_H14EUBB_DVBT_Set_GIDectectCtrl())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_GIDectectCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbt.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
								if(1) //(packetError < LX_DEMOD_PACKET_ERROR_THD_32700)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

								}
								else
								{
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Packet error = %d .....  --_--\n", packetError);
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
						{
							DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						pDemodInfo->signalStatus.bSignalValid = TRUE;

						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);


					}

					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}




				}
				else
				{
//					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
//					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_OPER_MODE_T 			operMode;

	if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if((operMode == LX_DEMOD_DVBT) || (operMode == LX_DEMOD_DVBT2))
	{
		if(operMode != LX_DEMOD_DVBT)
		{
			pDemodInfo->operMode = LX_DEMOD_DVBT2;
			pDemodInfo->setCfgParam.dvbt2.tuneMode = pDemodInfo->setCfgParam.dvbt.tuneMode;
			pDemodInfo->setCfgParam.dvbt2.channelBW = pDemodInfo->setCfgParam.dvbt.channelBW;
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) forcely changed to DVBT2 mode \n",__F__, __L__);
			DEMOD_PRINT("[DEMOD H14] (%s:%d) tuneMode(DVBT2)  = %d \n",__F__, __L__, pDemodInfo->setCfgParam.dvbt2.tuneMode);

			return RET_OK;
		}

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild operation mode !! \n",__F__, __L__);
		return RET_ERROR;

	}

	if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= 400)
		{
			_gH14EUBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT("[DEMOD H14] (%s) _gH14EUBB_DVB_UnlockCount_full_packeterr = %d  !!!\n", __F__,_gH14EUBB_DVB_UnlockCount_full_packeterr );

			if(_gH14EUBB_DVB_UnlockCount_full_packeterr == gH14EUBB_FullPkerrCount)
			{
				DEMOD_PRINT("[DEMOD H14] ooops !! (%s) Force to NOT LOCK  !!!\n", __F__);
				_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

				/* software Reset for DVB channel decoder */
				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				DEMOD_PRINT("[DEMOD H14] EXCUTE, GBB_DVB_SoftwareReset() !!!\n");

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
			}
		}
		else
		{

			_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)	&& (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

		}

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}



	}
	else
	{
		pDemodInfo->lockCount = 0;
		_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
#if 0
			if(RET_OK != DEMOD_H14EUBB_NeverLock_Infinite_Scan(operMode, TRUE))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverLock_Infinite_Scan() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
#endif
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H14] MONITORING FUNC : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= FALSE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xff;

//			pDemodInfo->setCfgParam.dvbt.tuneMode	= LX_DEMOD_TUNE_AUTO;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_2500;


		}



	}


	return RET_OK;



}



#if 0
#endif

/**
 * DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	UINT8 bch_lock_update_interval_7_0;
	UINT8	t2tdetrep_en;
	UINT8 tp_oen;
	UINT8 ifagc_oen;

#if 1
	UINT8 f8k_g128_eos_sel;
	UINT8 f8k_g128_sync_ofs_1_0;
	UINT8 f8k_g32_eos_sel;
	UINT8 f8k_g32_sync_ofs_1_0;

	UINT8 f8k_g16_eos_sel;
	UINT8 f8k_g16_sync_ofs_1_0;
	UINT8 f8k_g19_256_eos_sel;
	UINT8 f8k_g19_256_sync_ofs_1_0;

	UINT8 f8k_g8_eos_sel;
	UINT8 f8k_g8_sync_ofs_1_0;
	UINT8 f8k_g19_128_eos_sel;
	UINT8 f8k_g19_128_sync_ofs_1_0;

	UINT8 f8k_g4_eos_sel;
	UINT8 f8k_g4_sync_ofs_1_0;

	UINT8 f16k_g128_eos_sel;
	UINT8 f16k_g128_sync_ofs_1_0;
	UINT8 f16k_g32_eos_sel;
	UINT8 f16k_g32_sync_ofs_1_0;


	UINT8 f16k_g16_eos_sel;
	UINT8 f16k_g16_sync_ofs_1_0;
	UINT8 f16k_g19_256_eos_sel;
	UINT8 f16k_g19_256_sync_ofs_1_0;


	UINT8 f16k_g8_eos_sel;
	UINT8 f16k_g8_sync_ofs_1_0;
	UINT8 f16k_g19_128_eos_sel;
	UINT8 f16k_g19_128_sync_ofs_1_0;


	UINT8 f16k_g4_eos_sel;
	UINT8 f16k_g4_sync_ofs_1_0;

	UINT8 weight2k_7_0;
	UINT8 weight4k_7_0;
	UINT8 weight8k_7_0;
	UINT8 weight16k_7_0;

	UINT8 fstsp2dt01k;
	UINT8 fstsp2dt02k;
	UINT8 fstsp2dt04k;

	UINT8 tiroton;
#endif

	bch_lock_update_interval_7_0 = 0x40;
	T2C2_FEC_RdFL(t2c2_fec_ctrl104);   //regAddr = 0x3068, T2C2_FEC_CTRL[104]
	T2C2_FEC_Wr01(t2c2_fec_ctrl104, bch_lock_update_interval_7_0, bch_lock_update_interval_7_0);
	  //BCH_LOCK_UPDATE_INTERVAL[7:0] :Number of continuosly correct bch frames required to set bch_lock to high.

	t2tdetrep_en = 0x0;
	DVB_Common_RdFL(common_ctrl108);   // regAddr = 0x006c,COMMON_CTRL[108]
	DVB_Common_Wr01(common_ctrl108,t2tdetrep_en,t2tdetrep_en);
	DVB_Common_WrFL(common_ctrl108);	  //regAddr = 0x006c,COMMON_CTRL[108]

	tp_oen = 0x0;
	ifagc_oen = 0x0;
	DVB_Common_RdFL(common_ctrl110);   // regAddr = 0x006e,COMMON_CTRL[110]
	DVB_Common_Wr02(common_ctrl110,tp_oen,tp_oen, ifagc_oen, ifagc_oen);
	DVB_Common_WrFL(common_ctrl110);	  // regAddr = 0x006e,COMMON_CTRL[110]

#if 1
	f8k_g128_eos_sel 		= 0x1;
	f8k_g128_sync_ofs_1_0 	= 0x1;
	f8k_g32_eos_sel			= 0x1;
	f8k_g32_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl138);	 //regAddr = 0x10da, T2_PRE_SYNC_CTRL[138]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl138, f8k_g128_eos_sel, f8k_g128_eos_sel,
		                                  f8k_g128_sync_ofs_1_0,f8k_g128_sync_ofs_1_0,
		                                  f8k_g32_eos_sel, f8k_g32_eos_sel,
		                                  f8k_g32_sync_ofs_1_0, f8k_g32_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl138);


	f8k_g16_eos_sel 			= 0x1;
	f8k_g16_sync_ofs_1_0 		= 0x1;
	f8k_g19_256_eos_sel			= 0x1;
	f8k_g19_256_sync_ofs_1_0	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl139);	 //regAddr = 0x10db, T2_PRE_SYNC_CTRL[139]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl139, f8k_g16_eos_sel, f8k_g16_eos_sel,
		                                  f8k_g16_sync_ofs_1_0,f8k_g16_sync_ofs_1_0,
		                                  f8k_g19_256_eos_sel, f8k_g19_256_eos_sel,
		                                  f8k_g19_256_sync_ofs_1_0, f8k_g19_256_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl139);


	f8k_g8_eos_sel 				= 0x1;
	f8k_g8_sync_ofs_1_0 		= 0x1;
	f8k_g19_128_eos_sel			= 0x1;
	f8k_g19_128_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl140);	 //regAddr = 0x10dc, T2_PRE_SYNC_CTRL[140]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl140, f8k_g8_eos_sel, f8k_g8_eos_sel,
		                                  f8k_g8_sync_ofs_1_0,f8k_g8_sync_ofs_1_0,
		                                  f8k_g19_128_eos_sel, f8k_g19_128_eos_sel,
		                                  f8k_g19_128_sync_ofs_1_0, f8k_g19_128_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl140);


	f8k_g4_eos_sel 			= 0x1;
	f8k_g4_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl141);	 //regAddr = 0x10dd, T2_PRE_SYNC_CTRL[141]
	T2_Pre_Sync_Wr02(t2_pre_sync_ctrl141, f8k_g4_eos_sel, f8k_g4_eos_sel,
		                                  f8k_g4_sync_ofs_1_0,f8k_g4_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl141);


	f16k_g128_eos_sel 		= 0x1;
	f16k_g128_sync_ofs_1_0 	= 0x1;
	f16k_g32_eos_sel		= 0x1;
	f16k_g32_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl142);	 //regAddr = 0x10de, T2_PRE_SYNC_CTRL[142]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl142, f16k_g128_eos_sel, f16k_g128_eos_sel,
		                                  f16k_g128_sync_ofs_1_0,f16k_g128_sync_ofs_1_0,
		                                  f16k_g32_eos_sel, f16k_g32_eos_sel,
		                                  f16k_g32_sync_ofs_1_0, f16k_g32_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl142);



	f16k_g16_eos_sel 		= 0x1;
	f16k_g16_sync_ofs_1_0 	= 0x1;
	f16k_g19_256_eos_sel		= 0x1;
	f16k_g19_256_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl143);	 //regAddr = 0x10df,, T2_PRE_SYNC_CTRL[143]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl143, f16k_g16_eos_sel, f16k_g16_eos_sel,
		                                  f16k_g16_sync_ofs_1_0,f16k_g16_sync_ofs_1_0,
		                                  f16k_g19_256_eos_sel, f16k_g19_256_eos_sel,
		                                  f16k_g19_256_sync_ofs_1_0, f16k_g19_256_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl143);



	f16k_g8_eos_sel 		= 0x1;
	f16k_g8_sync_ofs_1_0 	= 0x1;
	f16k_g19_128_eos_sel			= 0x1;
	f16k_g19_128_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl144);	 //regAddr = 0x10e0, T2_PRE_SYNC_CTRL[144]
	T2_Pre_Sync_Wr04(t2_pre_sync_ctrl144, f16k_g8_eos_sel, f16k_g8_eos_sel,
		                                  f16k_g8_sync_ofs_1_0,f16k_g8_sync_ofs_1_0,
		                                  f16k_g19_128_eos_sel, f16k_g19_128_eos_sel,
		                                  f16k_g19_128_sync_ofs_1_0, f16k_g19_128_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl144);


	f16k_g4_eos_sel 		= 0x1;
	f16k_g4_sync_ofs_1_0 	= 0x1;

	T2_Pre_Sync_RdFL(t2_pre_sync_ctrl145);	 //regAddr = 0x10e1, T2_PRE_SYNC_CTRL[145]
	T2_Pre_Sync_Wr02(t2_pre_sync_ctrl145, f16k_g4_eos_sel, f16k_g4_eos_sel,
		                                  f16k_g4_sync_ofs_1_0,f16k_g4_sync_ofs_1_0);

	T2_Pre_Sync_WrFL(t2_pre_sync_ctrl145);

	weight2k_7_0 = 0x20;
	T2_Post_Sync_RdFL(t2_post_sync_ctrl97);	 //regAddr = 0x1161, T2_POST_SYNC_CTRL[97]
	T2_Post_Sync_Wr01(t2_post_sync_ctrl97, weight2k_7_0, weight2k_7_0);
	T2_Post_Sync_WrFL(t2_post_sync_ctrl97);

	weight4k_7_0 = 0x20;
	T2_Post_Sync_RdFL(t2_post_sync_ctrl99);	 //regAddr = 0x1163, T2_POST_SYNC_CTRL[99]
	T2_Post_Sync_Wr01(t2_post_sync_ctrl99, weight4k_7_0, weight4k_7_0);
	T2_Post_Sync_WrFL(t2_post_sync_ctrl99);

	weight8k_7_0 = 0x20;
	T2_Post_Sync_RdFL(t2_post_sync_ctrl101);	 //regAddr = 0x1165, T2_POST_SYNC_CTRL[101]
	T2_Post_Sync_Wr01(t2_post_sync_ctrl101, weight8k_7_0, weight8k_7_0);
	T2_Post_Sync_WrFL(t2_post_sync_ctrl101);

	weight16k_7_0 = 0x20;
	T2_Post_Sync_RdFL(t2_post_sync_ctrl103);	 //regAddr = 0x1167, T2_POST_SYNC_CTRL[103]
	T2_Post_Sync_Wr01(t2_post_sync_ctrl103, weight16k_7_0, weight16k_7_0);
	T2_Post_Sync_WrFL(t2_post_sync_ctrl103);

	fstsp2dt01k = 0;
	fstsp2dt02k = 0;
	fstsp2dt04k = 0;
	T2_Eq_Fi_RdFL(t2_eq_fi_ctrl83);	 //regAddr = 0x2053, T2_EQ_FI_CTRL[83]
	T2_Eq_Fi_Wr03(t2_eq_fi_ctrl83, fstsp2dt01k, fstsp2dt01k, fstsp2dt02k, fstsp2dt02k, fstsp2dt04k, fstsp2dt04k );
	T2_Eq_Fi_WrFL(t2_eq_fi_ctrl83);

	tiroton = 1;
	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
	T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
	T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);
#endif

	_gH14EUBB_DVB_UnlockCount_full_packeterr	  = 0;

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_Config_auto (BOOLEAN bAutoDetect)

{

	UINT8	useopm;
	UINT8	opm_1_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(bAutoDetect == TRUE)
	{
		useopm = 0; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr01(common_ctrl1,useopm,useopm); //USEOPM
		DVB_Common_WrFL(common_ctrl1);

	}
	else
	{
		useopm = 1; // USEOPM : USE Opreation mode of I2C (0 : Auto-OP mode detection, 1 : Manually USE)
		opm_1_0 = 0;

		DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Wr02(common_ctrl1,useopm,useopm,opm_1_0,opm_1_0 ); //USEOPM, OPM[1:0]
		DVB_Common_WrFL(common_ctrl1);
		// OPM[1:0] : Set operation mode manually.
		// 00 : DVB-T2 mode
		// 01 : DVB-T mode
		// 10 : DVB-C2 mode
		// 11 : DVB-C mode

	}

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbt2ConfigParam 	[IN] LX_DEMOD_DVBT2_CONFIG_T - DVB-T2 config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_PartialConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam)
{


	UINT8 	t2_dplp_id_7_0;
	UINT8 	n_sftrst_tp;
//	UINT8 	nplprst;

	UINT8	setPLPId;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	setPLPId = pDvbt2ConfigParam->unPLP;
	t2_dplp_id_7_0 = setPLPId;
	DEMOD_PRINT("[DEMOD H14] PLP ID(0x%x) is selected  !!!\n",setPLPId);

	T2_PLP1_RdFL(t2_plp_ctrl0);   //regAddr = 0x4000, T2_PLP_CTRL[0]
	T2_PLP1_Wr01(t2_plp_ctrl0, t2_dplp_id_7_0, t2_dplp_id_7_0); //T2_dPLP_ID[7:0] :Data PLP ID to be decoded
	T2_PLP1_WrFL(t2_plp_ctrl0);

	//TP reset
	n_sftrst_tp = 0;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

#if 0
	//T2 PLP reset
	nplprst = 0;
	T2_PLP1_RdFL(t2_plp_ctrl1);   //regAddr = 0x4001, T2_PLP_CTRL[1]
	T2_PLP1_Wr01(t2_plp_ctrl1, nplprst, nplprst); //nPLPRST :
	T2_PLP1_WrFL(t2_plp_ctrl1);
	// nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset
#endif

	OS_MsecSleep(10);

	n_sftrst_tp = 1;
	TP_RdFL(tp_ctrl1);   //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

#if 0
	nplprst = 1;
	T2_PLP1_RdFL(t2_plp_ctrl1);   //regAddr = 0x4001, T2_PLP_CTRL[1]
	T2_PLP1_Wr01(t2_plp_ctrl1, nplprst, nplprst); //nPLPRST :
	T2_PLP1_WrFL(t2_plp_ctrl1);
	// nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset

#endif

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_FullConfig (LX_DEMOD_DVBT2_CONFIG_T *pDvbt2ConfigParam)
{

	UINT8 	t2_dplp_id_7_0;
	UINT8 	n_sftrst_tp;

	UINT8	setPLPId;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	setPLPId = pDvbt2ConfigParam->unPLP;
	t2_dplp_id_7_0 = setPLPId;
	DEMOD_PRINT("[DEMOD H14] PLP ID(0x%x) is selected  !!!\n",setPLPId);

	T2_PLP1_RdFL(t2_plp_ctrl0);   //regAddr = 0x4000, T2_PLP_CTRL[0]
	T2_PLP1_Wr01(t2_plp_ctrl0, t2_dplp_id_7_0, t2_dplp_id_7_0); //T2_dPLP_ID[7:0] :Data PLP ID to be decoded
	T2_PLP1_WrFL(t2_plp_ctrl0);

	//TP reset
	n_sftrst_tp = 0;
	TP_RdFL(tp_ctrl1);	 //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

#if 0
	//T2 PLP reset
	nplprst = 0;
	T2_PLP1_RdFL(t2_plp_ctrl1);   //regAddr = 0x4001, T2_PLP_CTRL[1]
	T2_PLP1_Wr01(t2_plp_ctrl1, nplprst, nplprst); //nPLPRST :
	T2_PLP1_WrFL(t2_plp_ctrl1);
	// nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset
#endif

	OS_MsecSleep(10);

	n_sftrst_tp = 1;
	TP_RdFL(tp_ctrl1);	 //regAddr = 0x5001, TP_CTRL[1]
	TP_Wr01(tp_ctrl1, n_sftrst_tp, n_sftrst_tp); //N_SFTRST_TP :
	TP_WrFL(tp_ctrl1);
	// N_SFTRST_TP : TP formatter local reset
	// 0 : reset
	// 1 : no reset

#if 0
	nplprst = 1;
	T2_PLP1_RdFL(t2_plp_ctrl1);   //regAddr = 0x4001, T2_PLP_CTRL[1]
	T2_PLP1_Wr01(t2_plp_ctrl1, nplprst, nplprst); //nPLPRST :
	T2_PLP1_WrFL(t2_plp_ctrl1);
	// nPLPRST : PLP reset
	// 0 : reset
	// 1 : no reset

#endif


	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_GIDectectCtrl
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_GIDectectCtrl(void)
{

	UINT8 	bkfftm_done;
	UINT8 	bkfftm_2_0;

	UINT8	gim_sel;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);


	T2_Pre_Sync_RdFL(t2_pre_sync_mon0);	 //regAddr = 0x1080, T2_PRE_SYNC_MON[0]
	T2_Pre_Sync_Rd02(t2_pre_sync_mon0, bkfftm_done, bkfftm_done,bkfftm_2_0, bkfftm_2_0);

	if(bkfftm_done == 1)
	{
		if((bkfftm_2_0 == 3) ||(bkfftm_2_0 == 0))
		{
			gim_sel = 0;
			T2_Pre_Sync_RdFL(t2_pre_sync_ctrl58);	 //regAddr = 0x103a, T2_PRE_SYNC_CTRL[58]
			T2_Pre_Sync_Wr01(t2_pre_sync_ctrl58, gim_sel, gim_sel);
			T2_Pre_Sync_WrFL(t2_pre_sync_ctrl58);

		}
		else
		{
			gim_sel = 1;
			T2_Pre_Sync_RdFL(t2_pre_sync_ctrl58);	 //regAddr = 0x103a, T2_PRE_SYNC_CTRL[58]
			T2_Pre_Sync_Wr01(t2_pre_sync_ctrl58, gim_sel, gim_sel);
			T2_Pre_Sync_WrFL(t2_pre_sync_ctrl58);

		}
	}

	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H14EUBB_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl(void)
{

	UINT8 	misomode;
	UINT8 	misodone;
	UINT8 	dagcrefp_15_8;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);


	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon25); //regAddr = 0x24d9, T2TC2_Eq_TI_MON[25]
	T2TC2_Eq_Ti_Rd02(t2tc2_eq_ti_mon25,misomode,misomode,misodone, misodone );

	if((misomode == 1) && (misodone == 1))
	{
			dagcrefp_15_8 = 0x28;
			DVB_Common_RdFL(common_ctrl33);	 //regAddr = 0x0021, COMMON_CTRL[33]
			DVB_Common_Wr01(common_ctrl33, dagcrefp_15_8, dagcrefp_15_8);
			DVB_Common_WrFL(common_ctrl33);

	}
	else
	{
		dagcrefp_15_8 = 0x2f;
		DVB_Common_RdFL(common_ctrl33);  //regAddr = 0x0021, COMMON_CTRL[33]
		DVB_Common_Wr01(common_ctrl33, dagcrefp_15_8, dagcrefp_15_8);
		DVB_Common_WrFL(common_ctrl33);
	}


	return RET_OK;



}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Set_NeverLockWaitTime(UINT32 waitMs)
{
#if 0
	UINT8 scsyslockchktime_7_0;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = (UINT8)(waitMs / 10);

	Common_Wr01(common_ctrl11,scsyslockchktime_7_0,scsyslockchktime_7_0);
	Common_WrFL(common_ctrl11);  //regAddr = 0x000b,COMMON_CTRL[11]
	//SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in msec resolution

#endif

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{


	*pDelaySpread = 0;
	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pTpCorrected 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_TPCorrectedFlag(UINT8 *pTpCorrected)
{
	UINT8 bch_lock;


	T2C2_FEC_RdFL(t2c2_fec_mon47);	 //regAddr = 0x30af, T2C2_FEC_MON[47]
	T2C2_FEC_Rd01(t2c2_fec_mon47, bch_lock, bch_lock);
	// BCH_LOCK : High when # of continuous BCH data frame is correct. (# = value in 0x3068)
	// It clears to '0' whenever a host device read it through i2c protocol

	*pTpCorrected = bch_lock;
	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{


	UINT8 	t2_s2_3_0;
	UINT8	bwt_ext;

	*pTxModeLock = LX_DEMOD_LOCKED;
	*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T2_PLP1_RdFL(t2_plp_mon17);	//regAddr = 0x4021, T2_PLP_MON[1]
	T2_PLP1_Rd02(t2_plp_mon17,t2_s2_3_0,t2_s2_3_0, bwt_ext, bwt_ext);


	switch(t2_s2_3_0)
	{
		case 0x00 :
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_2K;
			break;

		case 0x02:
		case 0x0c:
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_8K;
			break;

		case 0x04:
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_4K;
			break;

		case 0x06:
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_1K;
			break;

		case 0x08:
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_16K;
			break;

		case 0x0a:
		case 0x0e:
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_32K;
			break;

		default :
			*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;
			break;
	}

	if(bwt_ext) DEMOD_PRINT(" Extended .....FFT mode \n");
	else 		DEMOD_PRINT(" Normal .....FFT mode \n");

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{

	UINT8 	gi_2_0;

	*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T2_PLP1_RdFL(t2_plp_mon18);	//regAddr = 0x4022, T2_PLP_MON[8]
	T2_PLP1_Rd01(t2_plp_mon18,gi_2_0,gi_2_0);
	//GIMODER_DI[1:0] : Guard Interval(GI) mode indication which is detected in the mode detector
	// 000 : 1/32
	// 001 : 1/16
	// 010 : 1/8
	// 011 : 1/4
	// 100 : 1/128
	// 101 : 19/128
	// 110 : 19/256
	// 111 : reserved

	switch(gi_2_0)
	{
		case 0x00 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_32;
			break;
		case 0x01:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_16;
			break;
		case 0x02 :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_8;
			break;
		case 0x03:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_4;
			break;
		case 0x04:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_1_128;
			break;
		case 0x05:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_19_128;
			break;
		case 0x06:
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_19_256;
			break;

		default :
			*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;
			break;
	}

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_CoderRate
 * Get the information of LP code rate on DVB-T2.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_CoderRate( LX_DEMOD_CODERATE_T *pCodeRate )
{


	UINT8 	dplp_cod_2_0;

	*pCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T2_PLP1_RdFL(t2_plp_mon69);	//regAddr = 0x4055, T2_PLP_MON[69]
	T2_PLP1_Rd01(t2_plp_mon69,dplp_cod_2_0,dplp_cod_2_0);
	// TPSLPCODERATER_D[2:0] : LP code rate for DVB-T indecator which is detected in the TPS decoder
	// 000 : 1/2
	// 001 : 3/5
	// 010 : 2/3
	// 011 : 3/4
	// 100 : 4/5
	// 101 : 5/6
	// others : reserved

	switch(dplp_cod_2_0)
	{
		case 0x00 :
			*pCodeRate = LX_DEMOD_CODE_1_2;
			break;
		case 0x01:
			*pCodeRate = LX_DEMOD_CODE_3_5;
			break;
		case 0x02 :
			*pCodeRate = LX_DEMOD_CODE_2_3;
			break;
		case 0x03:
			*pCodeRate = LX_DEMOD_CODE_3_4;
			break;
		case 0x04:
			*pCodeRate = LX_DEMOD_CODE_4_5;
			break;
		case 0x05:
			*pCodeRate = LX_DEMOD_CODE_5_6;
			break;
		default :
			*pCodeRate = LX_DEMOD_CODE_UNKNOWN;
			break;

	}


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_CurrentPLP_ID
 * Get current PLP ID on DVB-T2.
 *
 * @param *pCurrPlpID 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_CurrentPLP_ID(UINT8 *pCurrPlpID)
{
	UINT8 t2_dplp_id_7_0;

	T2_PLP1_RdFL(t2_plp_ctrl0);   //regAddr = 0x4000, T2_PLP_CTRL[0]
	T2_PLP1_Rd01(t2_plp_ctrl0,t2_dplp_id_7_0,t2_dplp_id_7_0);
	// T2_dPLP_ID[7:0] : Data PLP ID to be decoded

	*pCurrPlpID = t2_dplp_id_7_0;

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_StartPLP_ID
 * Get current PLP ID on DVB-T2.
 *
 * @param *pCurrPlpID 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_StartPLP_ID(UINT8 *pStartPlpID)
{
	UINT8 plp0_id_7_0;

	T2_PLP2_RdFL(t2_plp_id0); //regAddr = 0x4100, T2_PLP_ID[0]
	T2_PLP2_Rd01(t2_plp_id0,plp0_id_7_0,plp0_id_7_0);

	*pStartPlpID = plp0_id_7_0;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_PlpInfo
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_PlpInfo( LX_DEMOD_DVBT2_CONFIG_T *pPlpParams)
{
	LX_DEMOD_BWMODE_T channelBW = LX_DEMOD_BW_UNKNOWN;
	LX_DEMOD_FFTMODE_T FFTMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN ;
	LX_DEMOD_GIMODE_T gIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;
	LX_DEMOD_CODERATE_T codeRate = LX_DEMOD_CODE_UNKNOWN;
	LX_DEMOD_RF_MODE_T qamMode = LX_DEMOD_UNKOWN_MODE;

	LX_DEMOD_LOCK_STATE_T txModeLock;

	UINT8 currentPlpID;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if(NULL == pPlpParams)
	{
		DEMOD_PRINT("[DEMOD H14] ERROR, DEMOD_H14EUBB_DVBT_CONFIG_T TpsParams is NULL !!!\n");
		return RET_ERROR;
	}

	// Get channelBW

	if (RET_OK != DEMOD_H14EUBB_Get_BandWidthMode( &channelBW ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_BandWidthMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pPlpParams->channelBW = channelBW;
	DEMOD_PRINT("[DEMOD H14] Bandwidth = %s !!!\n",
		(channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_FFTMode( &txModeLock, &FFTMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pPlpParams->FFTMode = FFTMode;
	DEMOD_PRINT("[DEMOD H14] Tx mode = %s, FFTMode = %s  !!!\n",
		(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked",
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_1K) ? "FFT_1K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_4K) ? "FFT_4K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_16K) ? "FFT_16K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_32K) ? "FFT_32K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_GuradIntervalMode( &gIMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pPlpParams->gIMode = gIMode;
	DEMOD_PRINT("[DEMOD H14] Guard Intterval = %s  !!!\n",
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_19_256) ? "19_256" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_19_128) ? "19_128" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_128) ? "1_128" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );


	// Get LP coder rate
	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_CoderRate( &codeRate ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Get_LpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pPlpParams->codeRate = codeRate;
	DEMOD_PRINT("[DEMOD H14] code rate = %s  !!!\n",
		(codeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(codeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(codeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(codeRate == LX_DEMOD_CODE_3_5) ?  "Code_3_5" :
		(codeRate == LX_DEMOD_CODE_4_5) ?  "Code_4_5" :
		(codeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(codeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	if (RET_OK != DEMOD_H14EUBB_Get_QAMMode(&qamMode ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVB_Get_QAMMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pPlpParams->qamMode = qamMode;
	DEMOD_PRINT("[DEMOD H14] modulation  = %s  !!!\n",
		(qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(qamMode == LX_DEMOD_64QAM) ?  "64QAM" :
		(qamMode == LX_DEMOD_256QAM) ?  "256QAM" : " UNKOWN MODE" );

	// get current PLP ID
	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_CurrentPLP_ID(&currentPlpID ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Get_CurrentPLP_ID() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	pPlpParams->unPLP = currentPlpID;
	DEMOD_PRINT("[DEMOD H14]current PLP ID = 0x%x !!!\n",pPlpParams->unPLP);


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_multiPLP_ID
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_multiPLP_ID( LX_DEMOD_DVBT2_MULTI_PLP_INFO_T *pPlpParams)
{
	UINT8 	t2_num_plp_7_0;

	UINT8 	plpCount = 0;
	UINT16 	plpDataStart = 0x4100;

	UINT16  plpDataTypeStart = 0x4080;

	UINT8  tempPLPID = 0;
	UINT8  tempType = 0;


	LX_DEMOD_DVBT2_PLP_BYTE_T tempType1 = LX_DEMOD_DVBT2_PLP_UNKNOWN;
	LX_DEMOD_DVBT2_PLP_BYTE_T tempType2 = LX_DEMOD_DVBT2_PLP_UNKNOWN;
	LX_DEMOD_DVBT2_PLP_BYTE_T currType = LX_DEMOD_DVBT2_PLP_UNKNOWN;



	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	memset(pPlpParams, 0x00, sizeof(LX_DEMOD_DVBT2_MULTI_PLP_INFO_T));

	T2_PLP1_RdFL(t2_plp_mon50);  //regAddr = 0x4042, T2_PLP_MON[50]
	T2_PLP1_Rd01(t2_plp_mon50,t2_num_plp_7_0,t2_num_plp_7_0);

	pPlpParams->plpTotalCount = t2_num_plp_7_0;

	for(plpCount = 0; plpCount < t2_num_plp_7_0; plpCount++  )
	{
		if (RET_OK != DEMOD_I2C_Read( plpDataStart, 1, &tempPLPID))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_I2C_Read !!!\n", __F__, __L__);
			return RET_ERROR;
		}

		if(!(plpCount & 0x1))
		{
			if (RET_OK != DEMOD_I2C_Read( plpDataTypeStart, 1, &tempType))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_I2C_Read !!!\n", __F__, __L__);
				return RET_ERROR;
			}
			else
			{
				if(((tempType >> 4) & 0x7) == 0) 	tempType1 	= LX_DEMOD_DVBT2_PLP_COMMON;
				else 					   			tempType1 	= LX_DEMOD_DVBT2_PLP_DATA;

				if(((tempType >> 0) & 0x7) == 0) 	tempType2	= LX_DEMOD_DVBT2_PLP_COMMON;
				else					  			tempType2	= LX_DEMOD_DVBT2_PLP_DATA;

			}
			currType = tempType1;

			plpDataTypeStart++;
		}
		else
		{
			DEMOD_PRINT("[DEMOD H14][Before]type1=0x%x, type2 = 0x%x,  currType = 0x%x!!!\n", tempType1, tempType2,currType );
			currType = tempType2;
			DEMOD_PRINT("[DEMOD H14][After] type1=0x%x, type2 = 0x%x,  currType = 0x%x!!!\n", tempType1, tempType2,currType );
		}

		if(LX_DEMOD_DVBT2_PLP_COMMON == currType)
		{
			pPlpParams->commonPlpID[pPlpParams->commonPlpCount] = tempPLPID;
			pPlpParams->commonPlpCount++;

		}
		else if(LX_DEMOD_DVBT2_PLP_DATA == currType)
		{
			pPlpParams->plpDataID[pPlpParams->plpDatacount] = tempPLPID;
			pPlpParams->plpDatacount++;

		}
		else
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild PLP type !!!\n", __F__, __L__);

		}
		plpDataStart++;

		DEMOD_PRINT("[DEMOD H14]  plpInfo[%d].plpDataID = 0x%x !!!\n", plpCount,tempPLPID);

	}

	DEMOD_PRINT("[DEMOD H14] PLP TotalCount = %d  !!!\n",pPlpParams->plpTotalCount);
	DEMOD_PRINT("[DEMOD H14] PLP Datacount = %d  !!!\n",pPlpParams->plpDatacount);
	DEMOD_PRINT("[DEMOD H14] PLP CommonPlpCount = %d  !!!\n",pPlpParams->commonPlpCount);

	for(plpCount=0; plpCount < pPlpParams->plpDatacount; plpCount++)
	{
		DEMOD_PRINT("[DEMOD H14] PLP Data ID[%d] = 0x%x \n",plpCount, pPlpParams->plpDataID[plpCount]);

	}
	for(plpCount=0; plpCount < pPlpParams->commonPlpCount; plpCount++)
	{
		DEMOD_PRINT("[DEMOD H14] PLP Common PLP ID[%d] = 0x%x \n",plpCount, pPlpParams->commonPlpID[plpCount]);

	}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Get_TotalInfo
 * Get all information on DVB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_DVBT2_Get_TotalInfo( void)
{

	LX_DEMOD_DVBT2_CONFIG_T 		plpParams;
//	LX_DEMOD_LOCK_STATE_T 	agcLockStatus;
	LX_DEMOD_LOCK_STATE_T 	neverLockStatus;

	UINT16 					delaySpread;
	UINT16 					ifagcValue = 0;
	UINT32 					errorCount;
	SINT32 					FreqOff;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_MSE_T sigMse;

	LX_DEMOD_DVBT2_MULTI_PLP_INFO_T plpInfo;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

#if 0
	if (RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_AGC_LOCK, &agcLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
		agcLockStatus = LX_DEMOD_UNKOWN_STATE;
	}
	if(LX_DEMOD_LOCKED == agcLockStatus)
	{
		if (RET_OK != DEMOD_H14EUBB_Get_IFAGC(&ifagcValue))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_IFAGC() !!!\n", __F__, __L__);
			ifagcValue = 0;
		}
	}
#endif

	if (RET_OK != DEMOD_H14EUBB_Get_NeverLockStatus(LX_DEMOD_DVBT2, &neverLockStatus))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVB_Get_NeverLockStatus() !!!\n", __F__, __L__);
		neverLockStatus = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK !=  DEMOD_H14EUBB_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}

	//TP Error count
	if (RET_OK !=  DEMOD_H14EUBB_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);

	if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset( &FreqOff ))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		FreqOff = 0xffff;
	}

 	//MSE

	if (RET_OK != DEMOD_H14EUBB_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_MseInfo() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

//	DEMOD_PRINT("[DEMOD H14] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14] IF AGC value = %d(0x%x) !!!\n",ifagcValue,ifagcValue);
	DEMOD_PRINT("[DEMOD H14] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_DETECTED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H14 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD H14] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD H14] offset = %d [KHz]!!!\n",FreqOff);
	DEMOD_PRINT("[DEMOD H14] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_PlpInfo( &plpParams))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Get_TpsInfo() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	if (RET_OK != DEMOD_H14EUBB_DVBT2_Get_multiPLP_ID(&plpInfo))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Get_multiPLP_ID() !!!\n", __F__, __L__);
		return RET_ERROR;
	}



#ifndef  DEMOD_PRINT
	UINT8 plpCount;

//	printk("[DEMOD H14] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H14] IF AGC value = %d !!!\n",ifagcValue);
	printk("[DEMOD H14] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H14] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	printk("[DEMOD H14] Delay Spread = %d !!!\n",delaySpread);


	// Get bSetHp
	printk("[DEMOD H14] Set HP selection = %s !!!\n",(tppsParams->bSetHp == TRUE) ? " HP Mode" : " LP Mode");

	// Get channelBW
	printk("[DEMOD H14] Bandwidth = %s  !!!\n",
		(plpParams.channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(plpParams.channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(plpParams.channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	printk("[DEMOD H14] FFTMode = %s  !!!\n",
		(plpParams.FFTMode	== LX_DEMOD_DVBT_ISDBT_FFT_1K) ? "FFT_1K" :
		(plpParams.FFTMode 	== LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(plpParams.FFTMode 	== LX_DEMOD_DVBT_ISDBT_FFT_4K) ? "FFT_4K" :
		(plpParams.FFTMode	== LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(plpParams.FFTMode 	== LX_DEMOD_DVBT_ISDBT_FFT_16K) ? "FFT_16K" :
		(plpParams.FFTMode 	== LX_DEMOD_DVBT_ISDBT_FFT_32K) ? "FFT_32K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	printk("[DEMOD H14] Guard Intterval = %s  !!!\n",
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_19_256) ? "19_256" :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_19_128) ? "19_128" :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_128) ? "1_128" :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(plpParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );


	// Get Coder rate
	printk("[DEMOD H14] Code rate = %s  !!!\n",
		(plpParams.codeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(plpParams.codeRate == LX_DEMOD_CODE_2_3) ?  "Code_2_3" :
		(plpParams.codeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(plpParams.codeRate == LX_DEMOD_CODE_3_5) ?  "Code_3_5" :
		(plpParams.codeRate == LX_DEMOD_CODE_4_5) ?  "Code_4_5" :
		(plpParams.codeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(plpParams.codeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );




	// get QAM mode
	printk("[DEMOD H14]QAM MODE = %s  !!!\n",
		(plpParams.qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(plpParams.qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(plpParams.qamMode == LX_DEMOD_64QAM) ?  "64QAM" :
		(plpParams.qamMode == LX_DEMOD_256QAM) ?  "256QAM" : " UNKOWN MODE" );

	printk("[DEMOD H14] TP Error Count= %d !!!\n",errorCount);
	printk("[DEMOD H14] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);


	// get PLP info
	printk("[DEMOD H14] PLP TotalCount = %d  !!!\n",plpInfo.plpTotalCount);
	printk("[DEMOD H14] PLP Datacount = %d	!!!\n",plpInfo.plpDatacount);
	printk("[DEMOD H14] PLP CommonPlpCount = %d  !!!\n",plpInfo.commonPlpCount);

	for(plpCount=0; plpCount < plpInfo.plpDatacount; plpCount++)
	{
		printk("[DEMOD H14] PLP Data ID[%d] = 0x%x \n",plpCount, plpInfo.plpDataID[plpCount]);

	}
	for(plpCount=0; plpCount < plpInfo.commonPlpCount; plpCount++)
	{
		printk("[DEMOD H14] PLP Common PLP ID[%d] = 0x%x \n",plpCount, plpInfo.commonPlpID[plpCount]);

	}

#endif

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBT2_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;
	LX_DEMOD_OPER_MODE_T 			operMode;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue(pDemodInfo->setCfgParam.dvbt2.channelBW))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt2.tuneMode)
				{
					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

				}
				else
				{

#if 0
					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


#else
					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt2)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif

				}
				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{


				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt2.tuneMode)
				{

					if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					notifyinglockTimeOut	= LX_DEMOD_LOCK_DEALY_4000;
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{

					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

				}

			}

			break;


											/***************************************************/
											/***** Configure demod for scan mode. *****/
		case LX_DEMOD_TUNE_SCAN:			/***************************************************/
			{

				unsigned int currentTime = 0;
				unsigned int elapsedTime = 0;

				if((LX_DEMOD_FLAG_COMPLETE == pDemodInfo->scanJobFlag) || (LX_DEMOD_FLAG_FINISH == pDemodInfo->scanJobFlag))
				{

					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^c^[DEMOD H14] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_H14EUBB_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);

					}
					DEMOD_PRINT("[DEMOD H14] DEMOD_H14EUBB_NeverlockScan() operMode = 0x%x!!!\n", operMode);

					if(LX_DEMOD_DVBT == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBT;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING; //LX_DEMOD_RETUNE_RF;
						pDemodInfo->setCfgParam.dvbt.tuneMode = pDemodInfo->setCfgParam.dvbt2.tuneMode;
						pDemodInfo->setCfgParam.dvbt.channelBW = pDemodInfo->setCfgParam.dvbt2.channelBW;
						pDemodInfo->setCfgParam.dvbt.bSetHp = 1;
						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (operMode = LX_DEMOD_DVBT  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}
					else //(LX_DEMOD_DVBT2 == operMode)
					{
						pDemodInfo->operMode = LX_DEMOD_DVBT2;
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is Found (operMode = LX_DEMOD_DVBT2  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					}


				}
				else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
					pDemodInfo->bNotifiedUnlock	= TRUE;
					pDemodInfo->bNotifiedLock	= FALSE;
					pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;

					pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;
//					pDemodInfo->controlState 	= LX_DEMOD_SYNC_CHECKING;  //temporary



					currentTime = jiffies_to_msecs(jiffies);
					elapsedTime = currentTime - g_NewTuneReqeustTime;

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBT_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);



				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H14] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
				}

			}
			break;




													/********************************************/
													/***** Attempting to finish the tuning state machine *****/
		case LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK : 	/********************************************/
			{


			}
			break;




											/********************************************/
											/***** Attempting to obtain synch lock *****/
		case LX_DEMOD_SYNC_CHECKING : 		/********************************************/
			{

				LX_DEMOD_LOCK_STATE_T	lockState;
//				UINT8 startPlpID;

				if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);

				}
				DEMOD_PRINT("[DEMOD H14] LX_DEMOD_SYNC_CHECKING() operMode = 0x%x!!!\n", operMode);

				if(LX_DEMOD_DVBT == operMode)
				{
					pDemodInfo->operMode = LX_DEMOD_DVBT;
					pDemodInfo->setCfgParam.dvbt.tuneMode = pDemodInfo->setCfgParam.dvbt2.tuneMode;
					pDemodInfo->setCfgParam.dvbt.channelBW = pDemodInfo->setCfgParam.dvbt2.channelBW;
					pDemodInfo->setCfgParam.dvbt.bSetHp = 1;

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_DefaultRegisterValue(pDemodInfo->setCfgParam.dvbt.channelBW))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#if 0
					if(RET_OK != DEMOD_H14EUBB_Set_TirotCtrl())
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Set_TirotCtrl() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
#endif

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H14EUBB_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING(DVBT2) : Going to LX_DEMOD_SYNC_CHECKING(DVBT forcely)..... ^0^\n");


					return RET_OK;
				}

#if 1
				if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_GIDectectCtrl())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_GIDectectCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_L1_DONE, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;
					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
				}
				else
				{
				 	/* nothing to do so far */

				}
			}


		   break;

										   	/********************************************/
										   	/***** Attempting to obtain signal stability *****/
	   	case LX_DEMOD_SIGNAL_STABLE: 		/********************************************/
			{
				LX_DEMOD_LOCK_STATE_T			lockState;
				SINT32							freqOffset;
				unsigned int 					elapsedTime;
				UINT32 							packetError;

#if 1

				if(RET_OK != DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif
#if 0
				if(RET_OK != DEMOD_H14EUBB_Set_TirotCtrl())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_DVBT2_Set_MiSoAgcCtrl() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
#endif

				if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					if (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbt2.tuneMode)
					{
						elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

						if(elapsedTime < LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT)
						{
							break;
						}
						else
						{
							if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							else
							{
								if(1) //(packetError < LX_DEMOD_PACKET_ERROR_THD)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock	= FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

								}
								else
								{
									DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Packet error = %d .....  --_--\n", packetError);
									elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
									if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock = FALSE;
										pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
										pDemodInfo->signalStatus.bSignalValid = TRUE;

										if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

									}

								}

							}

						}
					}
					else
					{
						pDemodInfo->bNotifiedLock	= TRUE;
						pDemodInfo->bNotifiedUnlock = FALSE;
						pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
						pDemodInfo->signalStatus.bSignalValid = TRUE;

						if (RET_OK != DEMOD_H14EUBB_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if(RET_OK != DEMOD_H14EUBB_Get_Packet_Error(&packetError))
						{
							DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						pDemodInfo->signalStatus.bSignalValid = TRUE;

						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);


					}

					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H14] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}




				}
				else
				{
//					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
//					DEMOD_PRINT( "^R^[DEMOD H14] LX_DEMOD_SIGNAL_STABLE : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

				}


	   		}
			break;

		case LX_DEMOD_SIGNAL_LOCKED:	break;

		default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_DVBT2_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_DVBT2_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_OPER_MODE_T 			operMode;
	UINT8 tpCorrectFlag;

	if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if((operMode == LX_DEMOD_DVBT) || (operMode == LX_DEMOD_DVBT2))
	{
		if(operMode != LX_DEMOD_DVBT2)
		{
			pDemodInfo->operMode = LX_DEMOD_DVBT;
			pDemodInfo->setCfgParam.dvbt.tuneMode = pDemodInfo->setCfgParam.dvbt2.tuneMode;
			pDemodInfo->setCfgParam.dvbt.channelBW = pDemodInfo->setCfgParam.dvbt2.channelBW;
			pDemodInfo->setCfgParam.dvbt.bSetHp = 1;
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) forcely changed to DVBT mode \n",__F__, __L__);
			DEMOD_PRINT("[DEMOD H14] (%s:%d) tuneMode (DVBT)  = %d \n",__F__, __L__, pDemodInfo->setCfgParam.dvbt.tuneMode);

			return RET_OK;
		}

	}
	else
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invaild operation mode !! \n",__F__, __L__);
		return RET_ERROR;

	}

	if(RET_OK != DEMOD_H14EUBB_Get_Lock(LX_DEMOD_FEC_DEC_EN, &lockState))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}
#if 0
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedLock = %d, (lockCount = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedLock,pDemodInfo->lockCount);
	DEMOD_PRINT( " pDemodInfo->lockStatus = %d, pDemodInfo->bNotifiedUnlock = %d,(unlockCnt = %d) \n",pDemodInfo->lockStatus,pDemodInfo->bNotifiedUnlock,pDemodInfo->unLockCount);
	DEMOD_PRINT( " lockState = %d) \n",lockState);
#endif
	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(RET_OK != DEMOD_H14EUBB_DVBT2_Get_TPCorrectedFlag(&tpCorrectFlag))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_Packet_Error() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		DEMOD_PRINT("[DEMOD H14] (%s) packeterr = %d, TP corrected flag = %d !!!\n", __F__,pDemodInfo->signalStatus.packetError,tpCorrectFlag );
		if((pDemodInfo->signalStatus.packetError >= 160)&& (tpCorrectFlag == 0))
		{
			_gH14EUBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT("[DEMOD H14] (%s) _gH14EUBB_DVB_UnlockCount_full_packeterr = %d  !!!\n", __F__,_gH14EUBB_DVB_UnlockCount_full_packeterr );

			if(_gH14EUBB_DVB_UnlockCount_full_packeterr == gH14EUBB_FullPkerrCount)
			{
				DEMOD_PRINT("[DEMOD H14] ooops !! (%s) Force to NOT LOCK  !!!\n", __F__);
				_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

				/* software Reset for DVB channel decoder */
				if(RET_OK != DEMOD_H14EUBB_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				DEMOD_PRINT("[DEMOD H14] EXCUTE, GBB_DVB_SoftwareReset() !!!\n");

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
			}
		}
		else
		{

			_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)	&& (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;

			}
			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

		}

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

	}
	else
	{
		pDemodInfo->lockCount = 0;
		_gH14EUBB_DVB_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
				DEMOD_PRINT_ALWAYS( "[DEMOD H14] MONITORING FUNC : MSG_DVBT_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
#if 0
			if(RET_OK != DEMOD_H14EUBB_NeverLock_Infinite_Scan(operMode, TRUE))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_NeverLock_Infinite_Scan() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
#endif

			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H14] MONITORING FUNC  : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount >= 20)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;
			pDemodInfo->lockStatus					= LX_DEMOD_UNLOCKED;
			pDemodInfo->bNotifiedLock				= FALSE;
			pDemodInfo->bNotifiedUnlock 			= FALSE;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xff;

//			pDemodInfo->setCfgParam.dvbt2.tuneMode	= LX_DEMOD_TUNE_AUTO;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_2500;


		}



	}


	return RET_OK;



}




#if 0
#endif


