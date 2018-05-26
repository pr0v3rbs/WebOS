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
#include "demod_reg_l9.h"
#include "demod_common_l9.h"
#include "demod_dvb_l9.h"


#if 0
#endif




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_AutoSymbolRateDet
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  benable - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate )
{
	UINT8	stdopdetcmode_1_0 = 0;
	UINT64	symbol_temp = 0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

#if 1
	switch(symrateMode)
	{

		case LX_DEMOD_SYMR_FIXED_MODE :
		{
			UINT8	srdfbsymrate0_7_0;
			UINT8	srdfbsymrate0_15_8;

			//set symbol rate mode
			stdopdetcmode_1_0 = 0x1;

			//Manual symbol rate setting
			//SRDFBSYMRATE = symbol rate(Mhz) x 2^13

			symbol_temp = ((SymbolRate * 8192) + 500) / 1000;

			srdfbsymrate0_7_0 = (UINT8) (symbol_temp & 0xff);
			srdfbsymrate0_15_8 = (UINT8) ((symbol_temp >> 8) & 0xff);
			DEMOD_PRINT("[DEMOD L9] symbol_temp = 0x%llx \n",symbol_temp);
			DEMOD_PRINT("[DEMOD L9] srdfbsymrate0_15_8 = 0x%x \n",srdfbsymrate0_15_8);
			DEMOD_PRINT("[DEMOD L9] srdfbsymrate0_7_0 = 0x%x \n",srdfbsymrate0_7_0);

			Sync_Wr01(srd_ctrl6, srdfbsymrate0_15_8, srdfbsymrate0_15_8); //SRDFBSYMRATE[15:8]:
			Sync_WrFL(srd_ctrl6);  //regAddr = 0x1016, SRD_CTRL[6], SRDFBSYMRATE0[15:8]

			Sync_Wr01(srd_ctrl7, srdfbsymrate0_7_0, srdfbsymrate0_7_0); //SRDFBSYMRATE0[7:0]
			Sync_WrFL(srd_ctrl7);  //regAddr = 0x1017, SRD_CTRL[7], SRDFBSYMRATE0[7:0]


		}
		break;


		case LX_DEMOD_SYMR_FULL_SCAN_MODE :
		{

			//set symbol rate mode
			stdopdetcmode_1_0 = 0x3;
		}
			break;


		default :
			stdopdetcmode_1_0 = 0x3;
			break;

	}


	Common_RdFL(common_ctrl8);   //regAddr = 0x0009, COMMON_CTRL[8]
	Common_Wr01(common_ctrl8,stdopdetcmode_1_0, stdopdetcmode_1_0 );
	Common_WrFL(common_ctrl8);
	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	//Pre-defined QAM mode is set by QAMMODE[0](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM


#endif
	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_IsSymbolRateAuto
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  *bAuto - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto)
{


	UINT8	stdopdetcmode_1_0 = 0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl8);   //regAddr = 0x0009, COMMON_CTRL[8]
	Common_Rd01(common_ctrl8, stdopdetcmode_1_0,stdopdetcmode_1_0);

	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	//Pre-defined QAM mode is set by QAMMODE[0](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM

	if ((0 == stdopdetcmode_1_0) || (1 == stdopdetcmode_1_0))
	{
		*bAuto = FALSE;
		DEMOD_PRINT("[DEMOD L9] Auto Symbolrate detection is disabled  !!!\n");
	}
	else
	{
		*bAuto = TRUE;
		DEMOD_PRINT("[DEMOD L9] Auto Symbolrate detection is enabled  !!!\n");
	}



	return RET_OK;
}






/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_IsQammodeAutoDet
 * Control automatic setting of DVBC config.
 *
 * @param *bAutoQam 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{

	UINT8	qmden;


	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl9);   //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Rd01(common_ctrl9, qmden, qmden); //QMDEN : QAM Mode Detection(QMD) function control : 1-> enable, 0->disable

	if (1 == qmden)
	{
		*bAutoQam = TRUE;
		DEMOD_PRINT("[DEMOD L9] Auto QAM mode detection is enabled  !!!\n");
	}
	else
	{
		*bAutoQam = FALSE;
		DEMOD_PRINT("[DEMOD L9] Auto QAM mode detection is disabled  !!!\n");
	}

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Set_DefaultRegisterValue(void)
{

	UINT8 simfastenb;
	UINT8 srd1stchken;
	UINT8 aiccopmode0_1_0;

	UINT8 qmdqammode0_2_0;
	UINT8 qmdqammode1_2_0;
	UINT8 qmdqammode2_2_0;
	UINT8 qmdqammode3_2_0;
	UINT8 qmdqammode4_2_0;

	UINT8 agcinsel;
	UINT8 adcsel1v;

	DEMOD_PRINT("^p^[KADP_DEMOD DTV] @ %s =====\n",__F__);

	simfastenb = 1;
	Common_RdFL(common_ctrl1);   //regAddr = 0x0002, COMMON_CTRL[1]
	Common_Wr01(common_ctrl1, simfastenb, simfastenb);
	Common_WrFL(common_ctrl1);
	//SIMFASTENB: Chip operation mode control
	// 0 : simluation mode(for chip test only),  1 : normal operation mode
	//This signal should be set to '1' at the start of operation

	adcsel1v = 1;
	Common_RdFL(common_ctrl4);   //regAddr = 0x0004, COMMON_CTRL[4]
	Common_Wr01(common_ctrl4, adcsel1v, adcsel1v);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	srd1stchken = 1;
	Common_RdFL(common_ctrl9);   //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Wr01(common_ctrl9, srd1stchken, srd1stchken);
	Common_WrFL(common_ctrl9);
	//SRD1STCHKEN: Control of fast start for demodulation with pre-defined symbol rate
	//0 : disable, 1 : enable, This signal should be set to '1' at the start of operation

	aiccopmode0_1_0 = 0;
	Common_RdFL(mdp_ctrl8);   //regAddr = 0x0024, MDP_CTRL[8]
	Common_Wr01(mdp_ctrl8, aiccopmode0_1_0, aiccopmode0_1_0);
	Common_WrFL(mdp_ctrl8);
	//AICCOPMODE0[1:0] :election of 1st AICC(continuous wave interference or analog TV signal) rejection function
    //This register should be set to 0x00 at DVB-C and QAM

	// setting the sequence of QAM mode for auto detection (16QAM -> 32QAM -> 64QAM -> 128QAM -> 256QAM )
	qmdqammode0_2_0 = 0;  // 1st QAM (16QAM)
	qmdqammode1_2_0 = 1;  // 2nd QAM (32QAM)
	qmdqammode2_2_0 = 2;  // 3rd QAM (64QAM)
	qmdqammode3_2_0 = 3;  // 4th QAM (128QAM)
	qmdqammode4_2_0 = 4;  // 5th QAM (256QAM)

	Sync_RdFL(qmd_ctrl4);  //regAddr = 0x1020, QMD_CTRL[4]
	Sync_Wr02(qmd_ctrl4, qmdqammode0_2_0, qmdqammode0_2_0, qmdqammode1_2_0, qmdqammode1_2_0); //QMDQAMMODE0[2:0], QMDQAMMODE1[2:0]
	Sync_WrFL(qmd_ctrl4);

	Sync_RdFL(qmd_ctrl3);  //regAddr = 0x1020, QMD_CTRL[3]
	Sync_Wr02(qmd_ctrl3, qmdqammode2_2_0, qmdqammode2_2_0, qmdqammode3_2_0, qmdqammode3_2_0); //QMDQAMMODE2[2:0], QMDQAMMODE3[2:0]
	Sync_WrFL(qmd_ctrl3);

	Sync_RdFL(qmd_ctrl2);  //regAddr = 0x1020, QMD_CTRL[2]
	Sync_Wr01(qmd_ctrl2, qmdqammode4_2_0, qmdqammode4_2_0); //QMDQAMMODE4[2:0]
	Sync_WrFL(qmd_ctrl2);

	agcinsel = 1;
	Common_RdFL(agc_ctrl0);   //regAddr = 0x0012, AGC_CTRL[0]
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
    //AGCINSEL : Selection of input signal for gain error calculation
    // 0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
    // 1 : use incoming signal
    // This signal should be set to '1' at the start of operation  on all RF mode.

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Set_Config_auto (BOOLEAN bAutoDetect)
{

	UINT8	qmden;


	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);


	qmden = (bAutoDetect == TRUE)?(1):(0);  //QMDEN
	Common_RdFL(common_ctrl9);   //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Wr01(common_ctrl9, qmden, qmden);
	Common_WrFL(common_ctrl9);
	//QMDEN : QAM Mode Detection(QMD) function control
	//This is only valid for cable standard(DVB-C,QAM)
	// 0 : disable
	// 1 : enable


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param SymbolRate 	[IN] UINT16 - symbol rate : unit: KHz.
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Set_QamMode (UINT8 constel)
{

	UINT8 	qmdqammode0_2_0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);


	//QAM mode setting
	qmdqammode0_2_0 = constel;
	Sync_RdFL(qmd_ctrl4);  //regAddr = 0x1020, QMD_CTRL[4]
	Sync_Wr01(qmd_ctrl4, qmdqammode0_2_0, qmdqammode0_2_0);
	Sync_WrFL(qmd_ctrl4);
	//QMDQAMMODE0[2:0] : Set the 1st QAM mode for DVB-C
	//This signal is used QAM mode signal at single QAM mode detection for QAM(J.83), DVB-C, ISDB-C
	//QAM and ISDB-C has only 64QAM and 256QAM so only QMDQAMMODE0[2] is valid for QAM mode set(0 means 64QAM, 1 means 256QAM)
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q (default)
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Get_SymbolRateDetect
 * Get symbol rate in KHz (Don't use this function until revision A1)
 *
 * @param *pSymbolRate 	[OUT] UINT16 - symbol rate : unit: KHz.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate)
{
		UINT8 	detsymrate_16 = 0;
		UINT8 	detsymrate_15_8 = 0;
		UINT8 	detsymrate_7_0 = 0;


		UINT32 tempSymbolRate = 0;
		UINT32 tempSymbolRateKHz = 0;
		UINT32 symbolRateKHz = 0;

		*pSymbolRate = 0;

		//Detected symbol rate monitoring
		//symbol rate(MHz) = DETSYMRATE / 2^13

		Common_RdFL(common_mon4);  //regAddr = 0x0085, COMMON_MON[4]
		Common_Rd01(common_mon4,detsymrate_16,detsymrate_16); //DETSYMRATE[16]
		tempSymbolRate += (detsymrate_16 << 16 ) ;

		Common_RdFL(common_mon5);  //regAddr = 0x0086, COMMON_MON[5]
		Common_Rd01(common_mon5,detsymrate_15_8,detsymrate_15_8); //DETSYMRATE[15:8]
		tempSymbolRate += (detsymrate_15_8 << 8 ) ;

		Common_RdFL(common_mon6);  //regAddr = 0x0087, COMMON_MON[6]
		Common_Rd01(common_mon6,detsymrate_7_0,detsymrate_7_0); //DETSYMRATE[7:0]
		tempSymbolRate += (detsymrate_7_0 << 0 ) ;

		symbolRateKHz = (tempSymbolRate * 10000) / 8192 ; // symbol rate [KHz] = SYMBOLRATEDET x 2^(-13) * 1000 , 2^13 = 8192

		tempSymbolRateKHz = ((tempSymbolRate * 1000) / 8192) * 10 ;

		if((symbolRateKHz -  tempSymbolRateKHz) > 4)

			symbolRateKHz = (symbolRateKHz + 10)/10;
		else
			symbolRateKHz = (symbolRateKHz + 0)/10;

		*pSymbolRate = (UINT16)(symbolRateKHz);

		DEMOD_PRINT("[DEMOD L9] Symbol rate is %d [KHz]!!!\n",symbolRateKHz);


		return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Get_SymbolRateStatus
 * Get symbol rate detection state on DVB-C
 * Don't use this function until revision A1.
 *
 * @param *pSymbolRateDet 	[OUT] SYMBOLRATE_STATE_T - status and detected symbol rate .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet)
{

	UINT8 	srdlock;

	UINT16 symbolRate;

	pSymbolRateDet->status = LX_DEMOD_UNKOWN_STATE;
	pSymbolRateDet->symbolRate = 0;

	// Symbol rate
	Common_RdFL(common_mon0);  //regAddr = 0x0080, AGC_MON[0]
	Common_Rd01(common_mon0,srdlock,srdlock); //SRDLOCK: Symbol Rate Detection(SRD) status indication(0 : unlock, 1 : lock)

	if(1 == srdlock)
	{
		pSymbolRateDet->status = LX_DEMOD_DETECTED;
		DEMOD_PRINT("[DEMOD L9] Success (%s:%d) Symbol rate is detected !!!\n", __F__, __L__);
		if (RET_OK != DEMOD_L9_DVBC_Get_SymbolRateDetect(&symbolRate))
		{
			DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) Symbol rate couldn't be read !!!\n", __F__, __L__);
			return RET_ERROR;
		}
		pSymbolRateDet->symbolRate = symbolRate;
	}
	else
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) Symbol rate couldn't be detected !!!\n", __F__, __L__);
		pSymbolRateDet->status = LX_DEMOD_DETECTION_FAIL;

	}


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{

	UINT8 qmdstatus_1_0 = 0;
	UINT8 qmden = 0;
	UINT8 qmdqammode0_2_0 = 0;

	LX_DEMOD_RF_MODE_T qamMode;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	pQamModeDet->status = LX_DEMOD_UNKOWN_STATE;
	pQamModeDet->qamMode = LX_DEMOD_UNKOWN_STATE;

	Common_RdFL(common_ctrl9);   //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Rd01(common_ctrl9, qmden, qmden); //QMDEN : QAM Mode Detection(QMD) function control : 1-> enable, 0->disable

	if(qmden == 1)
	{
		Sync_RdFL(sys_mon1);  //regAddr = 0x1081, SYS_MON[1]
		Sync_Rd01(sys_mon1,qmdstatus_1_0,qmdstatus_1_0);
		//QMDSTATUS[1:0] : QMD status indication(00 :init status, 01: processing, 10: lock, 11: fail)

		switch(qmdstatus_1_0)
		{
			case 0x00 :
			{
				pQamModeDet->status = LX_DEMOD_IDLE;
				break;
			}
			case 0x01 :
			{
				pQamModeDet->status = LX_DEMOD_DETECTION_PROCESSING;
				break;
			}
			case 0x02 :
			{
				pQamModeDet->status = LX_DEMOD_DETECTED;

				if (RET_OK != DEMOD_L9_Get_QAMMode(&qamMode ))
				{
					DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVB_Get_QAMMode() !!!\n", __F__, __L__);
					return RET_ERROR;
				}
				pQamModeDet->qamMode = qamMode;

				break;
			}
			case 0x03 :
			{
				pQamModeDet->status = LX_DEMOD_DETECTION_FAIL;
				break;
			}


		}
	}
	else
	{
		pQamModeDet->status = LX_DEMOD_DETECTION_MANUAL;

		Sync_RdFL(qmd_ctrl4);  //regAddr = 0x1020, QMD_CTRL[4]
		Sync_Rd01(qmd_ctrl4, qmdqammode0_2_0, qmdqammode0_2_0); //QMDQAMMODE0[2:0]
		switch(qmdqammode0_2_0)
		{
			case 0x00 :
				 pQamModeDet->qamMode = LX_DEMOD_16QAM;
				break;
			case 0x01 :
				 pQamModeDet->qamMode = LX_DEMOD_32QAM;
				break;
			case 0x02 :
				 pQamModeDet->qamMode = LX_DEMOD_64QAM;
				break;
			case 0x03 :
				 pQamModeDet->qamMode = LX_DEMOD_128QAM;
				break;
			case 0x04 :
				 pQamModeDet->qamMode = LX_DEMOD_256QAM;
				break;
			default :
				 pQamModeDet->qamMode = LX_DEMOD_UNKOWN_MODE;
				break;


		}


	}

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;


	if(pDemodInfo->bSetParams)
	{
		switch (pDemodInfo->controlState)
		{

												/***************************************************/
												/***** Configure demod	for RETUNE. *****/
			case LX_DEMOD_RETUNE_RF:			/***************************************************/
				{

					if(RET_OK != DEMOD_L9_DVBC_Set_DefaultRegisterValue())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
					{
						if(RET_OK != DEMOD_L9_DVBC_Set_Config_auto(TRUE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

					}
					else
					{

#if 1
						if(RET_OK != DEMOD_L9_DVBC_Set_Config_auto(TRUE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

#else
						if(RET_OK != DEMOD_L9_DVBC_Set_Config_auto(FALSE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FIXED_MODE, pDemodInfo->setCfgParam.dvbc.symbolRate))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBC_Set_QamMode(pDemodInfo->setCfgParam.dvbc.qamMode))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Set_QamMode() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}


#endif
					}

					pDemodInfo->controlState = LX_DEMOD_TUNE_START;
					DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
					g_RetuneStartTime = jiffies_to_msecs(jiffies);

				}

				break;


												/***************************************************/
												/***** Configure tuner for desired channel No. *****/
			case LX_DEMOD_TUNE_START:			/***************************************************/
				{


					if(RET_OK != DEMOD_L9_SoftwareReset())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_SoftwareReset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
					{

						if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


					}
					else
					{


						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

						DEMOD_PRINT( "^c^[DEMOD L9] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
						return RET_OK;
					}

					if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					}
					else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
						pDemodInfo->bNotifiedUnlock	= TRUE;
						pDemodInfo->bNotifiedLock	= FALSE;

						pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;


						currentTime = jiffies_to_msecs(jiffies);
						elapsedTime = currentTime - g_NewTuneReqeustTime;

						DEMOD_PRINT("\nMSG_DVBC_FRONT_END_NOT_LOCKED..\n") ;
						DEMOD_PRINT("Elapse time is %d msec\n", elapsedTime) ;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


					}
					else
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{
						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

						sigStablestartTime = jiffies_to_msecs(jiffies);

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
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
								if(RET_OK != DEMOD_L9_Get_Packet_Error(&packetError))
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_VSB_PreMonitor() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
								else
								{
									if(packetError < LX_DEMOD_PACKET_ERROR_THD)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock	= FALSE;

										if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

									}
									else
									{
										elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
										if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
										{
											pDemodInfo->bNotifiedLock	= TRUE;
											pDemodInfo->bNotifiedUnlock = FALSE;

											if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
											{
												DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
												DEMOD_RETURN_FAIL(__F__);
											}
											pDemodInfo->freqOffset = freqOffset;

											pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

											elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

										}

									}

								}

							}
						}
						else
						{
							pDemodInfo->bNotifiedLock	= TRUE;
							pDemodInfo->bNotifiedUnlock = FALSE;

							if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
							{
								DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							pDemodInfo->freqOffset = freqOffset;

							pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

							elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);



						}

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}


					}
					else
					{
						pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

					}


		   		}
				break;

			case LX_DEMOD_SIGNAL_LOCKED:	break;

			default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

		}
	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;

		}
		if(pDemodInfo->lockCount == 4)
		{
			pDemodInfo->lockStatus = LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock = TRUE;

		}
		if(pDemodInfo->lockCount > 4)
		{
			pDemodInfo->lockCount = 0;
		}


	}
	else
	{
		pDemodInfo->lockCount = 0;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->unLockCount > 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 200)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount > 200)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1500;


		}



	}


	return RET_OK;



}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBC_Get_DvbInfo
 * Get all information on DVB-C for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBC_Get_DvbInfo(void)

{

	UINT32 errorCount;
	UINT32 vBer;

	UINT16 	ifAGCGain = 0;
	LX_DEMOD_MSE_T sigMse;

	BOOLEAN isAutoSymbol;
	BOOLEAN isAutoQammode;

	LX_DEMOD_SYMBOLRATE_STATE_T symbolRateDet;
	LX_DEMOD_QAMMODE_STATE_T qamModeDet;

	LX_DEMOD_LOCK_STATE_T neverLockStatus;

	LX_DEMOD_LOCK_STATE_T samplingLock;

	LX_DEMOD_LOCK_STATE_T carrLockState;
	LX_DEMOD_LOCK_STATE_T fecLockState;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_TPERRCNTMODE_T errorMode;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	//IF AGC
	if (RET_OK != DEMOD_L9_Get_IFAGC(&ifAGCGain))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_IFAGC() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD L9] IF AGC Gain = 0x%x !!!\n",ifAGCGain);

 	//NeverLock status
	if (RET_OK != DEMOD_L9_Get_NeverLockStatus(LX_DEMOD_DVBC, &neverLockStatus))
	{

		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_NeverLockStatus() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	switch(neverLockStatus)
	{

		case LX_DEMOD_IDLE :
			DEMOD_PRINT("[DEMOD L9] Initialization process  !!!\n");
			break;

		case LX_DEMOD_DETECTION_PROCESSING :
			DEMOD_PRINT("[DEMOD L9] Signal Checking process !!!\n");
			break;

		case LX_DEMOD_DETECTED :
			DEMOD_PRINT("[DEMOD L9] normal State (available channel)  !!!\n");
			break;

		case LX_DEMOD_DETECTION_FAIL :
			DEMOD_PRINT("[DEMOD L9] neverlock state (empty channel) !!!\n");

			break;

		default :

			DEMOD_PRINT("[DEMOD L9] INVALID neverLockStatus !!!\n");

			break;

	}

	// Carrier LOCK status  and offset monitoring


	if (RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_CARR_LOCK,&carrLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}


	if(carrLockState == LX_DEMOD_LOCKED)
	{

		DEMOD_PRINT("[DEMOD L9] DVB CARRIER is LOCKED  !!!\n");

	}
	else
	{
		DEMOD_PRINT("[DEMOD L9] DVB CARRIER is NOT LOCKED  !!!\n");

	}

	// Sampling frequency LOCK status and offset monitoring
	if (RET_OK != DEMOD_L9_Get_Lock( LX_DEMOD_TR_LOCK, &samplingLock))
	{

		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if(LX_DEMOD_LOCKED == samplingLock)
	{
		DEMOD_PRINT("[DEMOD L9] Sampling frequency is Locked   !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD L9] Sampling frequency is NOT Locked !!!\n");
	}


	//Symbol rate detection

	if (RET_OK != DEMOD_L9_DVBC_IsSymbolRateAuto(&isAutoSymbol))
	{

		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoSymbol)
	{
		if (RET_OK != DEMOD_L9_DVBC_Get_SymbolRateStatus(&symbolRateDet))

		{

			DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

			return RET_ERROR;

		}

		switch(symbolRateDet.status)

		{

			case LX_DEMOD_IDLE :

			{

				DEMOD_PRINT("[DEMOD L9] Symbol rate status is idle status!!!\n");

				break;

			}

			case LX_DEMOD_DETECTED :

			{

				DEMOD_PRINT("[DEMOD L9] Symbol rate  is detected !!!\n");

				DEMOD_PRINT("[DEMOD L9] Symbol rate = %d [KHz] !!!\n",symbolRateDet.symbolRate);

				break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :

			{

				DEMOD_PRINT("[DEMOD L9] Symbol rate detection is processing !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_FAIL :

			{

				DEMOD_PRINT("[DEMOD L9] Symbol rate detection is failed !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD L9] INVALID Symbol rate detection status  !!!\n");

				break;
			}



		}
	}


	// QAM mode detect status and mode monitoring
	if (RET_OK != DEMOD_L9_DVBC_IsQammodeAutoDet(&isAutoQammode))
	{

		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_IsQammodeAutoDet() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoQammode)
	{

		if (RET_OK != DEMOD_L9_DVBC_Get_QamModeDetectStatus(&qamModeDet))
		{
			DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);

			return RET_ERROR;
		}

		switch(qamModeDet.status)
		{
			case LX_DEMOD_IDLE :
			{
				DEMOD_PRINT("[DEMOD L9] QAM mode detect is idle status!!!\n");

				break;
			}

			case LX_DEMOD_DETECTED :
			{

				DEMOD_PRINT("[DEMOD L9] QAM mode is detected!!!\n");

				DEMOD_PRINT("[DEMOD L9] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :

																		 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :

																		  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );

			break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :
			{

				DEMOD_PRINT("[DEMOD L9] QAM mode detection is processing !!!\n");

				break;
			}

			case LX_DEMOD_DETECTION_FAIL :
			{

				DEMOD_PRINT("[DEMOD L9] QAM mode detection is failed !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_MANUAL :
			{

				DEMOD_PRINT("[DEMOD L9] Manual QAM mode detection mode !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD L9] INVALID QAM mode detection  !!!\n");

				break;
			}

		}

	}


 	//spectrum inversion status
	if (RET_OK !=  DEMOD_L9_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if(LX_DEMOD_INVERSION == spectrumInv)
	{
		DEMOD_PRINT("[DEMOD L9] Spectrum Inversion !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD L9] Spectrum Normal !!!\n");
	}

	//Frame lock status
	if (RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}

	if(fecLockState != LX_DEMOD_LOCKED)
	{
		DEMOD_PRINT("[DEMOD L9] DVB FEC IS NOT LOCKED !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD L9] DVB FEC LOCK OK !!!\n");
	}

 	//MSE

	if (RET_OK != DEMOD_L9_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD L9] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);


 	//BER after Viterbi
	if (RET_OK != DEMOD_L9_Get_VABER(&vBer))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SNR() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD L9] vBER = %d  !!!\n",vBer);

	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_L9_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD L9] TP Error Count= %d !!!\n",errorCount);


#ifndef DEMOD_PRINT

	printk("[DEMOD L9] IF AGC Gain = 0x%x !!!\n",ifAGCGain);
	printk("[DEMOD L9] DVB CARRIER is %s  !!!\n",(carrLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked" );
	printk("[DEMOD L9] Carrier offset Frequency = %d [KHz] !!!\n",carrierLock.carrOffset);
	printk("[DEMOD L9] Sampling frequency is %s   !!!\n",(samplingLock == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD L9] QAM mode is %s   !!!\n",(qamModeDet.status == LX_DEMOD_DETECTED) ? "detected" : "NOT detected");

	if(qamModeDet.status == LX_DEMOD_DETECTED)
	{
		printk("[DEMOD L9] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :
										            (qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :
	                                               	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :
												 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :
												  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );
	}
	printk("[DEMOD L9 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																		(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	printk("[DEMOD L9] FEC lock is %s !!!\n",(fecLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD L9] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD L9] vBER = %d  !!!\n",vBer);
	printk("[DEMOD L9] TP Error Count= %d !!!\n",errorCount);

#endif

	return RET_OK;

}




#if 0
#endif

/**
 * DEMOD_L9_DVBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	UINT8 srd1stchken;
	UINT8 crfgain_di_1_0;
	UINT8 agcinsel;
	UINT8 simfastenb;
	UINT8 adcsel1v;
	UINT8 qmden;
	UINT8 daften;
	UINT8 cirautomode_d;
	UINT8 cirpwrctrl_3_0;
	UINT8 crifohold_di;
	UINT8 cirth_7_0;
	UINT8 cirth_9_8;
	UINT8 cirlockctrl;
	UINT8 csimode;
	UINT8 fstomode_1_0;
	UINT8 aiccrejautosync_di;
	UINT8 aiccopmode0_1_0;
	UINT8 aiccopmode1_1_0;
	UINT8 aiccopmode2_1_0;
	UINT8 aiccopmode3_1_0;

	UINT8 agcen;
	UINT8 agcoutpol;
	UINT8 aiccrejautoeq_di;
	UINT8 dagc2ndgearshen;
	UINT8 dagc2ndbw_3_0;
	UINT8 scneverlock1shoten;
	UINT8 srdsinglestdrstcon;
	UINT8 stoogimargin_7_0;

	UINT8 aiccoffth_19_16;

	UINT8 aiccdetth_19_16;
	UINT8 aiccdetth_15_8;
	UINT8 aiccdetth_7_0;


	UINT8 agciffixb;
	UINT8 agcrffixb;

	UINT8 aiccalpha_3_0;
	UINT8 aiccalphastop_3_0;

	UINT8 txpswlockth_di_3_0;
	UINT8 autorptrsten;

	UINT8 crlfbw_3_0;
	UINT8 crgearshen;
	UINT8 samgearshen;

	
	UINT8 stochkint_1_0;
	UINT8 stochkcc_2_0;

	DEMOD_PRINT("^p^[KADP_DEMOD DTV] @ %s =====\n",__F__);

	simfastenb = 1;
	Common_RdFL(common_ctrl1);   //regAddr = 0x0002, COMMON_CTRL[1]
	Common_Wr01(common_ctrl1, simfastenb, simfastenb);
	Common_WrFL(common_ctrl1);
	//SIMFASTENB: Chip operation mode control
	// 0 : simluation mode(for chip test only),  1 : normal operation mode
	//This signal should be set to '1' at the start of operation

	adcsel1v = 0;
	Common_RdFL(common_ctrl4);   //regAddr = 0x0004, COMMON_CTRL[4]
	Common_Wr01(common_ctrl4, adcsel1v, adcsel1v);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	srd1stchken = 1;
	qmden = 0;
	daften = 0;
	Common_RdFL(common_ctrl9);   //regAddr = 0x000a, COMMON_CTRL[9]
	Common_Wr03(common_ctrl9, srd1stchken, srd1stchken, qmden, qmden, daften, daften);
	Common_WrFL(common_ctrl9);
	//SRD1STCHKEN: Control of fast start for demodulation with pre-defined symbol rate
				//0 : disable, 1 : enable, This signal should be set to '1' at the start of operation
	// DAFTEN : Digital Automatic Frequency Tuning(DAFT) function control
				// This is only valid for cable standard(DVB-C,QAM) (0 : disable, 1 : enable)
	//QMDEN : QAM Mode Detection(QMD) function control
				//This is only valid for cable standard(DVB-C,QAM) (0 : disable, 1 : enable)

	crfgain_di_1_0 = 2;
	Sync_RdFL(carrier_ctrl_di0);  //regAddr = 0x1020, QMD_CTRL[2]
	Sync_Wr01(carrier_ctrl_di0, crfgain_di_1_0, crfgain_di_1_0); //QMDQAMMODE4[2:0]
	Sync_WrFL(carrier_ctrl_di0);

	agcinsel = 1;
	Common_RdFL(agc_ctrl0);   //regAddr = 0x0012, AGC_CTRL[0]
	Common_Wr01(agc_ctrl0, agcinsel, agcinsel);
	Common_WrFL(agc_ctrl0);
    //AGCINSEL : Selection of input signal for gain error calculation
    // 0 : use ths signal of output of the pre-processing(e.g. digital gain error control)
    // 1 : use incoming signal
    // This signal should be set to '1' at the start of operation  on all RF mode.

	// For avoiding macro-blocking in TW field stream.
	cirautomode_d = 1;
	EQ_DI_RdFL(cir_ctrl0);  //regAddr = 0x2214, CIR_CTRL[0]
	EQ_DI_Wr01(cir_ctrl0, cirautomode_d, cirautomode_d); //CIRAUTOMODE_D : "CIRAUTOEN" signal enable control(DVB-T only)
																//0 : disable, 1 : enable
	EQ_DI_WrFL(cir_ctrl0);

	cirpwrctrl_3_0 = 7;
	EQ_DI_RdFL(cir_ctrl1);  //regAddr = 0x2215, CIR_CTRL[1]
	EQ_DI_Wr01(cir_ctrl1, cirpwrctrl_3_0, cirpwrctrl_3_0); //CIRPWRCTRL[3:0] :CIR Power level control (1x = 9bits saturation)
	EQ_DI_WrFL(cir_ctrl1);

	crifohold_di = 0;
	Sync_RdFL(carrier_ctrl_di10);  //regAddr = 0x105c, CARRIER_CTRL_DI[10]
	Sync_Wr01(carrier_ctrl_di10, crifohold_di, crifohold_di); //CRIFOHOLD_DI :IFO holding control. 0 : no holding 1 : IFO hold after TMCC / TPS lock
	Sync_WrFL(carrier_ctrl_di10);

	cirth_7_0 = 0x27;
	EQ_DI_RdFL(cir_ctrl3); //regAddr = 0x2217, CIR_CTRL[3] : CIR control register
	EQ_DI_Wr01(cir_ctrl3,cirth_7_0, cirth_7_0 );  //CIRTH[7:0]: CIR detection threshold value set
	EQ_DI_WrFL(cir_ctrl3);

	cirth_9_8 = 0x0;
	cirlockctrl = 0x0;
	EQ_DI_RdFL(cir_ctrl2); //regAddr = 0x2216, CIR_CTRL[2] : CIR control register
	EQ_DI_Wr02(cir_ctrl2,cirth_9_8, cirth_9_8,cirlockctrl,cirlockctrl );  //CIRTH[9:8]: CIR detection threshold value set
	EQ_DI_WrFL(cir_ctrl2);

	csimode = 0x0;
	EQ_DI_RdFL(dieq_ctrl5); //regAddr = 0x2222, DIEQ_CTRL[5] : DVB-T Equalizer control register
	EQ_DI_Wr01(dieq_ctrl5,csimode,csimode );
	EQ_DI_WrFL(dieq_ctrl5);
	//CSIMODE : CSI algorithm selection (0 : algorithm based on IIR filtering with channel power and MSE, 1 : algorithm based on channel power)

	fstomode_1_0 = 0x3;
	EQ_DI_RdFL(fsto_ctrl0); //regAddr = 0x2210, FSTO_CTRL[0] : Fine STO control register //internal scan is blocked for AGC speed
	EQ_DI_Wr01(fsto_ctrl0,fstomode_1_0, fstomode_1_0);
	EQ_DI_WrFL(fsto_ctrl0);
	//FSTOMODE[1:0]:: FSTO adjustment algorithm selection
	// 00 : manual adjustment
	// 01 : algorithm based on ISI
	// 10 : algorithm based on ISI and correct
	// 11 : algorithm based on correct

	aiccrejautosync_di = 0;
	Sync_RdFL(sampling_ctrl_di0); //regAddr = 0x1032, SAMPLING_CTRL_DI[0]: Sampling control register : DVB-T
	Sync_Wr01(sampling_ctrl_di0,aiccrejautosync_di,aiccrejautosync_di );
	Sync_WrFL(sampling_ctrl_di0);
	//AICCREJAUTOSYNC_DI : Samplamaskauto_di setting (0 : samplamaskauto_di = i2crw_samplamaskauto_di, 1 : 1)

	agcen = 1;
	Common_RdFL(agc_ctrl0);   //regAddr = 0x0012,  AGC_CTRL[0] :AGC control register
	Common_Wr01(agc_ctrl0, agcen, agcen);
	Common_WrFL(agc_ctrl0);
	//AGCEN : AGC function operation control
	// 0 : disable(not calculate gain error of incoming signal)
	// 1 : enable	gain of incoming signal is controlled by RFAGC and IFAGC

	agcoutpol = 1;
	Common_RdFL(common_ctrl3);   //regAddr = 0x0003, COMMON_CTRL[3]:Common control register
	Common_Wr01(common_ctrl3, agcoutpol, agcoutpol);
	Common_WrFL(common_ctrl3);
	//AGCOUTPOL : Selection of the gain control signal output polarity
	// 0 : low means gain up
	// 1 : high means gain up

	aiccrejautoeq_di = 0x0;
	EQ_DI_RdFL(dieq_ctrl4); //regAddr = 0x2221, DIEQ_CTRL[4] : DVB-T Equalizer control register
	EQ_DI_Wr01(dieq_ctrl4,aiccrejautoeq_di, aiccrejautoeq_di );
	EQ_DI_WrFL(dieq_ctrl4);
    //AICCREJAUTOEQ_DI: CSI auto change mode using AICC detection information
	// 0 : manual
	// 1 : auto change (old or new) when AICC detection flag on

	dagc2ndgearshen = 0;
	dagc2ndbw_3_0 = 0x9;
	Common_RdFL(mdp_ctrl24);   //regAddr = 0x0035, MDP_CTRL[24]:MDP control register -->value :  0xe8  //for TW field stream
	Common_Wr02(mdp_ctrl24, dagc2ndgearshen,dagc2ndgearshen,dagc2ndbw_3_0, dagc2ndbw_3_0);   //DAGC2NDBW[3:0], DAGC2NDGEARSHEN
	Common_WrFL(mdp_ctrl24);
	//DAGC2NDBW[3:0] : Selection of bandwidth for gain error calculation of 2nd digital AGC
	// 0000 : 1/256x	   0001 : 1/128x   0010 : 1/64x   0011 : 1/32x
	// 0100 : 1/16x		0101 : 1/8x 	 0110 : 1/4x	 0111 : 1/2x
	// 1000 : 1x, default	1001 : 2x		  1010 : 4x 	   1011 : 8x
	// 1100 : 16x			 1101 : 32x 	  1110 : 64x	  1111 : 128x

	// DAGC2NDGEARSHEN : Gear shift function control for gain error update at the 2nd digital AGC
	// 0 : disable,	1 : enable

	scneverlock1shoten = 0;
	Sync_RdFL(daft_ctrl0); //regAddr = 0x1000, DAFT_CTRL[0]: DAFT control register
	Sync_Wr01(daft_ctrl0,scneverlock1shoten,scneverlock1shoten );  //SCNEVERLOCK1SHOTEN
	Sync_WrFL(daft_ctrl0);

	srdsinglestdrstcon = 0;
	Common_RdFL(common_ctrl9);   //regAddr = 0x000a,  COMMON_CTRL[9] :Common control register
	Common_Wr01(common_ctrl9, srdsinglestdrstcon, srdsinglestdrstcon);
	Common_WrFL(common_ctrl9);
	//SRDSINGLESTDRSTCON : Control of single standard reception check function
	//	In case of single standard check function is enabled, ths system is always operation with defined standard mode without re-initialization
	// 0 : disable,  1 : enable

	stoogimargin_7_0 = 0x20;  //field test in TW
	EQ_DI_RdFL(sto_ctrl5); //regAddr = 0x2206, STO_CTRL[5] : STO control register
	EQ_DI_Wr01(sto_ctrl5,stoogimargin_7_0, stoogimargin_7_0 );  //STOOGIMARGIN[7:0] : CIR Pre bound margin for outside GI case
	EQ_DI_WrFL(sto_ctrl5);

	aiccoffth_19_16 = 0x0;
	aiccdetth_19_16 = 0x0;
	Common_RdFL(mdp_ctrl3);   //regAddr = 0x001f, MDP_CTRL[3]:MDP control register
	Common_Wr02(mdp_ctrl3, aiccoffth_19_16, aiccoffth_19_16, aiccdetth_19_16, aiccdetth_19_16);   //AICCDETTH[19:16], AICCOFFTH[19:16]
	Common_WrFL(mdp_ctrl3);

	aiccdetth_15_8 = 0x0;
	Common_RdFL(mdp_ctrl4);   //regAddr = 0x0020, MDP_CTRL[4]:MDP control register
	Common_Wr01(mdp_ctrl4, aiccdetth_15_8, aiccdetth_15_8);   //AICCDETTH[15:8]
	Common_WrFL(mdp_ctrl5);

	aiccdetth_7_0 = 0x1;
	Common_RdFL(mdp_ctrl5);   //regAddr = 0x0021, MDP_CTRL[5]:MDP control register
	Common_Wr01(mdp_ctrl5, aiccdetth_7_0, aiccdetth_7_0);   //AICCDETTH[7:0]
	Common_WrFL(mdp_ctrl5);

	aiccopmode0_1_0 = 1;
	aiccopmode1_1_0 = 1;
	aiccopmode2_1_0 = 0;
	aiccopmode3_1_0 = 0;

	Common_RdFL(mdp_ctrl8);   //regAddr = 0x0024,  MDP_CTRL[8] :MDP control Register.
	Common_Wr04(mdp_ctrl8, aiccopmode0_1_0, aiccopmode0_1_0,aiccopmode1_1_0,aiccopmode1_1_0,aiccopmode2_1_0,aiccopmode2_1_0,aiccopmode3_1_0,aiccopmode3_1_0);
	Common_WrFL(mdp_ctrl8);
	//AICCOPMODE0[1:0], AICCOPMODE1[1:0], AICCOPMODE2[1:0], AICCOPMODE3[1:0]
	//Selection of 1st AICC(continuous wave interference or analog TV signal) rejection function
	// 00 : always turn off
	// 01 : always turn for of pre-defined frequency which is set by AICCFIXFREQ0(@0x2E~0x30)
	// 10 : conditionally turn on or turn off for pre-defined frequency which is set by AICCFIXFREQ0(@0x2E~0x30)
	// 11 : scan mode which means turn on at the detected frequency in any frequency
	// This register should be set to 0x00 at DVB-C and QAM

	agciffixb = 0x0;
	agcrffixb = 0x0;
	Common_RdFL(agc_ctrl1);   //regAddr = 0x0013, AGC_CTRL[1]:AGC control register
	Common_Wr02(agc_ctrl1, agciffixb, agciffixb,agcrffixb,agcrffixb);
	Common_WrFL(agc_ctrl1);

	aiccalpha_3_0 = 0x7;
	aiccalphastop_3_0 = 0xb;
	Common_RdFL(mdp_ctrl2);   //regAddr = 0x001e, MDP_CTRL[2]
	Common_Wr02(mdp_ctrl2,aiccalpha_3_0,aiccalpha_3_0, aiccalphastop_3_0,aiccalphastop_3_0 );
	Common_WrFL(mdp_ctrl2);

	txpswlockth_di_3_0 = 0xf;
	Common_RdFL(eq_ctrl5);   //regAddr = 0x0059, EQ_CTRL[5]:MDP control register
	Common_Wr01(eq_ctrl5, txpswlockth_di_3_0, txpswlockth_di_3_0);   //TXPSWLOCKTH_DI[3:0]:TXP Syncword Lock threshold value set
	Common_WrFL(eq_ctrl5);

	autorptrsten = 0;
	Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
	Common_Wr01(common_ctrl1, autorptrsten, autorptrsten);
	Common_WrFL(common_ctrl1);
	// AUTORPTRSTEN : Auto repeated reset enable control which is generated in case of continuous transport packet error
	// during pre-defined interval (about 2.5sec) (0 : disable, 1 : enable)

	crlfbw_3_0 = 4;
	crgearshen = 0;
	Common_RdFL(carrier_ctrl0);	 //regAddr = 0x0044, CARRIER_CTRL[0]
	Common_Wr02(carrier_ctrl0, crlfbw_3_0, crlfbw_3_0, crgearshen, crgearshen);
	Common_WrFL(carrier_ctrl0);
	// CRLFBW[3:0] : Set of bandwidth for carrier frequency error calculation
	// x000: 1/16x, x001:1/8x, x010:1/4x, x011:1/2x, x100:1x, default, x101:2x, x110:4x, x111:8x

	samgearshen = 0;
	Common_RdFL(sampling_ctrl0);	 //regAddr = 0x003d,
	Common_Wr01(sampling_ctrl0, samgearshen, samgearshen);
	Common_WrFL(sampling_ctrl0);

	stochkint_1_0 = 0x1;  
	stochkcc_2_0 = 0x3;
	EQ_DI_RdFL(sto_ctrl1); //regAddr = 0x2202, STO_CTRL[1] : STO control register
	EQ_DI_Wr02(sto_ctrl1,stochkint_1_0, stochkint_1_0,stochkcc_2_0,stochkcc_2_0);
	EQ_DI_WrFL(sto_ctrl1);
	//STOCHKCC[2:0] :Selection of reference tp error (per packet) for correction check
					//000 : 1, 001 : 2, 010 : 3, 011 : 4, 100 : 5, 101 : 6, 110 : 8, 111 : 16
	//STOCHKINT[1:0]:STO tp correction check interval selection
		// 00 : 768, 01 : 1024, 10 : 1280, 11 : 2048

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Set_Config_auto (BOOLEAN bAutoDetect)

{

	UINT8	txpauto_di;
	UINT8	mdauto_di;
//	UINT8 	cirth_7_0;
//	UINT8 	cirth_9_8;
//	UINT8 	csimode;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);


	mdauto_di = (bAutoDetect == TRUE)?(1):(0);	//MDAUTO_DI
	Common_RdFL(eq_ctrl1);   //regAddr = 0x0053, EQ_CTRL[1]
	Common_Wr01(eq_ctrl1, mdauto_di, mdauto_di); //MDAUTO_DI : FFT/Guard Interval mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(eq_ctrl1);

	txpauto_di = (bAutoDetect == TRUE)?(1):(0);  //QAMMODEAUTO
	Common_RdFL(eq_ctrl3);   //regAddr = 0x0057, EQ_CTRL[3]
	Common_Wr01(eq_ctrl3, txpauto_di, txpauto_di); //TXPAUTO_DI : Transmission mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(eq_ctrl3);

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_L9_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{


	UINT8 	hierhpsel_d;
	UINT8	stdopdettmode_2_0;

	//UINT8	aiccfixfreq1_23_16;
	//UINT8	aiccfixfreq0_23_16;

	UINT8 stdopmode_7_0;
	UINT8 stdopdetcmode_1_0;

	UINT8 samfreq_21_16;
	UINT8 samfreq_15_8;
	UINT8 samfreq_7_0;


	BOOLEAN 			bSetHp;
	LX_DEMOD_BWMODE_T 	channelBW;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	bSetHp = pDvbtConfigParam->bSetHp;
	if(bSetHp)
	{
		hierhpsel_d = 0x1;
		DEMOD_PRINT("[DEMOD L9] High Priority signal is selected  !!!\n");
	}
	else
	{
		hierhpsel_d = 0x0;
		DEMOD_PRINT("[DEMOD L9] Low Priority signal is selected  !!!\n");
	}

	Common_RdFL(fec_ctrl1);   //regAddr = 0x0075, FEC_CTRL[1]
	Common_Wr01(fec_ctrl1, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority
	Common_WrFL(fec_ctrl1);
	//HIERHPSEL_D :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority


	//selection of RF bandwidth of received signal
	channelBW = pDvbtConfigParam->channelBW;
	switch(channelBW)
	{
		case LX_DEMOD_DVBT_NO_OPERATION:
		{
			stdopmode_7_0 = 0x0;

			stdopdettmode_2_0 = 0x0;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;
			break;
		}
		case LX_DEMOD_BW_8MHZ:
		{
			stdopmode_7_0 = 0x40;

			stdopdettmode_2_0 = 0x1;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;


			break;
		}
		case LX_DEMOD_BW_7MHZ:
		{
			stdopmode_7_0 = 0x40;

			stdopdettmode_2_0 = 0x2;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;


			break;
		}
		case LX_DEMOD_BW_6MHZ:
		{

			stdopmode_7_0 = 0x40;

			stdopdetcmode_1_0 = 0x0;
			stdopdettmode_2_0 = 0x3;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;

			break;
		}


		default :
			DEMOD_PRINT("[DEMOD L9](%s,%d) Not supported bandwidth parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}

	Common_RdFL(common_ctrl7); //regAddr = 0x0008, COMMON_CTRL[7]
	Common_Wr01(common_ctrl7,stdopmode_7_0, stdopmode_7_0 );
	Common_WrFL(common_ctrl7);

	//STDOPMODE[7:0] : Standard mode selection
	// 00000010 : ISDB-C
	// 00000100 : DVB-C
	// 00001000 : QAM
	// 00010000 : DTMB(reserved)
	// 00100000 : ISDB-T
	// 01000000 : DVB-T
	// 10000000 : VSB

	Common_RdFL(common_ctrl8);   //regAddr = 0x0009, COMMON_CTRL[8]
	Common_Wr02(common_ctrl8,stdopdettmode_2_0, stdopdettmode_2_0, stdopdetcmode_1_0, stdopdetcmode_1_0 );
	Common_WrFL(common_ctrl8);
	//STDOPDETTMODE[2:0]  : Selection of bandwidth detection mode for terrestrial standard(VSB,DVB-T,ISDB-T) reception
	// 000 : no operation for terrestrial standard
	// 001 : 8MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 010 : 7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 011 : 6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 100 : 7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 101 : 8,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 110 : 8,7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 111 : 8,7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)

	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	//Pre-defined QAM mode is set by QAMMODE[0](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM


	Common_RdFL(common_ctrl11); //regAddr = 0x000D, SAMFREQ[21:16] : Fs(MHz) = SAMFREQ / 2^16, SAMFREQ = Fs(MHz) x 2^16
	Common_Wr01(common_ctrl11,samfreq_21_16, samfreq_21_16 );  //SAMFREQ[21:16] : for DVB-T/C, SAMFREQ should be set to 0x1D
	Common_WrFL(common_ctrl11);

	Common_RdFL(common_ctrl12); //regAddr = 0x000E, SAMFREQ[15:8]
	Common_Wr01(common_ctrl12,samfreq_15_8, samfreq_15_8 );  //SAMFREQ[15:8] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl12);

	Common_RdFL(common_ctrl13); //regAddr = 0x000F, SAMFREQ[7:0]
	Common_Wr01(common_ctrl13,samfreq_7_0, samfreq_7_0 );  //SAMFREQ[7:0] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl13);

	//Set of ADC sampling frequency(Fs)
	//Fs(MHz) = SAMFREQ / 2^16
	//SAMFREQ = Fs(MHz) x 2^16
	//for VSB,QAM, SAMFREQ should be set to 0x190000
	//for DVB-T/C, SAMFREQ should be set to 0x1D0000




	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_L9_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{

	UINT8 	hierhpsel_d;
	UINT8	stdopdettmode_2_0;
	UINT8 	mdfftmode_di_1_0;
	UINT8 	mdgimode_di_1_0;
	UINT8 	tpshiermode_d_2_0;
	UINT8 	tpshpcoderate_d_2_0;
	UINT8 	tpslpcoderate_d_2_0;
	UINT8 	tpsqammode_d_1_0;

	UINT8 stdopmode_7_0;
	UINT8 stdopdetcmode_1_0;

	UINT8 samfreq_21_16;
	UINT8 samfreq_15_8;
	UINT8 samfreq_7_0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	if(pDvbtConfigParam->bSetHp)
	{
		hierhpsel_d = 0x1;
		DEMOD_PRINT("[DEMOD L9] High Priority signal is selected  !!!\n");
	}
	else
	{
		hierhpsel_d = 0x0;
		DEMOD_PRINT("[DEMOD L9] Low Priority signal is selected	!!!\n");
	}

	Common_RdFL(fec_ctrl1);   //regAddr = 0x0075, FEC_CTRL[1]
	Common_Wr01(fec_ctrl1, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority
	Common_WrFL(fec_ctrl1);
	//HIERHPSEL_D :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority

	//selection of RF bandwidth of received signal
	switch(pDvbtConfigParam->channelBW)
	{

		case LX_DEMOD_DVBT_NO_OPERATION:
		{
			stdopmode_7_0 = 0x0;

			stdopdettmode_2_0 = 0x0;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;
			break;
		}
		case LX_DEMOD_BW_8MHZ:
		{
			stdopmode_7_0 = 0x40;

			stdopdettmode_2_0 = 0x1;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;


			break;
		}
		case LX_DEMOD_BW_7MHZ:
		{
			stdopmode_7_0 = 0x40;

			stdopdettmode_2_0 = 0x2;
			stdopdetcmode_1_0 = 0x0;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;


			break;
		}
		case LX_DEMOD_BW_6MHZ:
		{

			stdopmode_7_0 = 0x40;

			stdopdetcmode_1_0 = 0x0;
			stdopdettmode_2_0 = 0x3;

			samfreq_21_16 = 0x1d;
			samfreq_15_8 = 0;
			samfreq_7_0 = 0;

			break;
		}


		default :
			DEMOD_PRINT("[DEMOD L9](%s,%d) Not supported bandwidth parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}


	Common_RdFL(common_ctrl7); //regAddr = 0x0008, COMMON_CTRL[7]
	Common_Wr01(common_ctrl7,stdopmode_7_0, stdopmode_7_0 );
	Common_WrFL(common_ctrl7);

	//STDOPMODE[7:0] : Standard mode selection
	// 00000010 : ISDB-C
	// 00000100 : DVB-C
	// 00001000 : QAM
	// 00010000 : DTMB(reserved)
	// 00100000 : ISDB-T
	// 01000000 : DVB-T
	// 10000000 : VSB


	Common_RdFL(common_ctrl8);   //regAddr = 0x0009, COMMON_CTRL[8]
	Common_Wr02(common_ctrl8,stdopdettmode_2_0, stdopdettmode_2_0, stdopdetcmode_1_0, stdopdetcmode_1_0 );
	Common_WrFL(common_ctrl8);
	//STDOPDETTMODE[2:0]  : Selection of bandwidth detection mode for terrestrial standard(VSB,DVB-T,ISDB-T) reception
	// 000 : no operation for terrestrial standard
	// 001 : 8MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 010 : 7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 011 : 6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 100 : 7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 101 : 8,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 110 : 8,7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 111 : 8,7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)

	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	//Pre-defined QAM mode is set by QAMMODE[0](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM

	Common_RdFL(common_ctrl11); //regAddr = 0x000D, SAMFREQ[21:16] : Fs(MHz) = SAMFREQ / 2^16, SAMFREQ = Fs(MHz) x 2^16
	Common_Wr01(common_ctrl11,samfreq_21_16, samfreq_21_16 );  //SAMFREQ[21:16] : for DVB-T/C, SAMFREQ should be set to 0x1D
	Common_WrFL(common_ctrl11);

	Common_RdFL(common_ctrl12); //regAddr = 0x000E, SAMFREQ[15:8]
	Common_Wr01(common_ctrl12,samfreq_15_8, samfreq_15_8 );  //SAMFREQ[15:8] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl12);

	Common_RdFL(common_ctrl13); //regAddr = 0x000F, SAMFREQ[7:0]
	Common_Wr01(common_ctrl13,samfreq_7_0, samfreq_7_0 );  //SAMFREQ[7:0] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl13);

	//Set of ADC sampling frequency(Fs)
	//Fs(MHz) = SAMFREQ / 2^16
	//SAMFREQ = Fs(MHz) x 2^16
	//for VSB,QAM, SAMFREQ should be set to 0x190000
	//for DVB-T/C, SAMFREQ should be set to 0x1D0000


	//FFT mode manual set
	switch(pDvbtConfigParam->FFTMode)
	{
		case LX_DEMOD_DVBT_ISDBT_FFT_2K :
			{
				mdfftmode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] Set FFT Mode = %s !!!\n","2K ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_FFT_8K :
			{
				mdfftmode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD L9] Set FFT Mode = %s !!!\n","8K");
			}
			break;

		default :
			{
				mdfftmode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d)UNKNOWN FFT mode !!!\n",__F__, __L__);
			}
			return RET_ERROR;
	}

	//GI(Guard Interval) mode manual set
	switch(pDvbtConfigParam->gIMode)
	{
		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_4 :
			{
				mdgimode_di_1_0 = 0x3;
				DEMOD_PRINT("[DEMOD L9] Set Guard Interval = %s !!!\n","1_4 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_8 :
			{
				mdgimode_di_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD L9] Set Guard Interval = %s !!!\n","1_8 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_16 :
			{
				mdgimode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD L9] Set Guard Interval = %s !!!\n","1_16 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_32 :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] Set Guard Interval = %s !!!\n","1_32 ");
			}
			break;
		default :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] FAIL,(%s:%d) UNKNOWN Guard Interval  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(eq_ctrl1);   //regAddr = 0x0053, EQ_CTRL[1]
	Common_Wr02(eq_ctrl1, mdfftmode_di_1_0, mdfftmode_di_1_0, mdgimode_di_1_0, mdgimode_di_1_0);
	Common_WrFL(eq_ctrl1);

	//MDFFTMODE_DI[1:0] : FFT mode manual set
	// 00 : 2K
	// 01 : 8K
	// 10 : 4K
	// 11 : Reserved

	//MDGIMODE_DI[1:0]: GI(Guard Interval) mode manual set
	// 00 : 1/32
	// 01 : 1/16
	// 10 : 1/8
	// 11 : 1/4


	//Hierarchical mode manual set
	switch(pDvbtConfigParam->hierachMode)
	{
		case LX_DEMOD_DVB_TPS_HIERACHY_NONE :
			{
				tpshiermode_d_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] Set Hierachical Mode = %s !!!\n","NONE");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_1 :
			{
				tpshiermode_d_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD L9] Set Hierachical Mode = %s !!!\n","alpha_1");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_2 :
			{
				tpshiermode_d_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD L9] Set Hierachical Mode = %s !!!\n","alpha_2");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_4 :
			{
				tpshiermode_d_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD L9] Set Hierachical Mode = %s !!!\n","alpha_4");
			}
			break;
		default :
			{
				tpshiermode_d_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] FAIL,(%s:%d) UNKNOWN Hierachical Mode  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	//QAM mode manual set
	switch(pDvbtConfigParam->qamMode)
	{
		case LX_DEMOD_QPSK :
			{
				tpsqammode_d_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] Set QAM mode = %s !!!\n","QPSK");
			}
			break;

		case LX_DEMOD_16QAM :
			{
				tpsqammode_d_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD L9] Set QAM mode = %s !!!\n","16QAM");
			}
			break;

		case LX_DEMOD_64QAM :
			{
				tpsqammode_d_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD L9] Set QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				tpsqammode_d_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD L9] FAIL,(%s:%d) UNKNOWN QAM mode !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(eq_ctrl9);   //regAddr = 0x005f, EQ_CTRL[9]
	Common_Wr02(eq_ctrl9, tpshiermode_d_2_0, tpshiermode_d_2_0, tpsqammode_d_1_0, tpsqammode_d_1_0);
	Common_WrFL(eq_ctrl9);

	// TPSHIERMODE_D[2:0] : Hierarchical mode manual set
	// 000 : a = Non hierarchical
	// 001 : a = 1
	// 010 : a = 2
	// 011 : a = 4
	// 111~100 : reserved

	//TPSQAMMODE_D[1:0] : QAM mode manual set
	// 00 : QPSK
	// 01 : 16-QAM
	// 10 : 64-QAM
	// 11 : reserved


	//Hierachical mode code rate
	if(pDvbtConfigParam->bSetHp)  //code rate of high priority signal manual set
	{
		switch(pDvbtConfigParam->hpCodeRate)
		{
			case LX_DEMOD_CODE_1_2 :
				{
					tpshpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD L9] Set HP code rate = %s !!!\n","1_2");
				}
				break;

			case LX_DEMOD_CODE_2_3 :
				{
					tpshpcoderate_d_2_0 = 0x1;
					DEMOD_PRINT("[DEMOD L9] Set HP code rate = %s !!!\n","2_3");
				}
				break;

			case LX_DEMOD_CODE_3_4 :
				{
					tpshpcoderate_d_2_0 = 0x2;
					DEMOD_PRINT("[DEMOD L9] Set HP code rate = %s !!!\n","3_4");
				}
				break;

			case LX_DEMOD_CODE_5_6 :
				{
					tpshpcoderate_d_2_0 = 0x3;
					DEMOD_PRINT("[DEMOD L9] Set HP code rate = %s !!!\n","5_6");
				}
				break;

			case LX_DEMOD_CODE_7_8 :
				{
					tpshpcoderate_d_2_0 = 0x4;
					DEMOD_PRINT("[DEMOD L9] Set HP code rate = %s !!!\n","7_8");
				}
				break;
			default :
				{
					tpshpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD L9] FAIL,(%s:%d) UNKNOWN HP code rate !!!\n", __F__, __L__);
				}
				return RET_ERROR;
		}

		Common_RdFL(eq_ctrl10);   //regAddr = 0x0060, EQ_CTRL[10]
		Common_Wr01(eq_ctrl10, tpshpcoderate_d_2_0, tpshpcoderate_d_2_0);
		Common_WrFL(eq_ctrl10);
		//TPSHPCODERATE_D[2:0] : 'HP code rate manual set
		//000 : 1/2
		//001 : 2/3
		//010 : 3/4
		//011 : 5/6
		//100 : 7/8
		//111~101 : reserved


	}
	else  //code rate of low priority signal manual set
	{
		switch(pDvbtConfigParam->lpCodeRate)
		{
			case LX_DEMOD_CODE_1_2 :
				{
					tpslpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD L9] Set LP code rate = %s !!!\n","1_2");
				}
				break;

			case LX_DEMOD_CODE_2_3 :
				{
					tpslpcoderate_d_2_0 = 0x1;
					DEMOD_PRINT("[DEMOD L9] Set LP code rate = %s !!!\n","2_3");
				}
				break;

			case LX_DEMOD_CODE_3_4 :
				{
					tpslpcoderate_d_2_0 = 0x2;
					DEMOD_PRINT("[DEMOD L9] Set LP code rate = %s !!!\n","3_4");
				}
				break;

			case LX_DEMOD_CODE_5_6 :
				{
					tpslpcoderate_d_2_0 = 0x3;
					DEMOD_PRINT("[DEMOD L9] Set LP code rate = %s !!!\n","5_6");
				}
				break;

			case LX_DEMOD_CODE_7_8 :
				{
					tpslpcoderate_d_2_0 = 0x4;
					DEMOD_PRINT("[DEMOD L9] Set LP code rate = %s !!!\n","7_8");
				}
				break;
			default :
				{
					tpslpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD L9] FAIL,(%s:%d) UNKNOWN LP code rate !!!\n", __F__, __L__);
				}
				return RET_ERROR;
		}

		Common_RdFL(eq_ctrl10);   //regAddr = 0x0060, EQ_CTRL[10]
		Common_Wr01(eq_ctrl10, tpslpcoderate_d_2_0, tpslpcoderate_d_2_0);
		Common_WrFL(eq_ctrl10);
		//TPSLPCODERATE_D[2:0] : LP code rate manual set
		//000 : 1/2
		//001 : 2/3
		//010 : 3/4
		//011 : 5/6
		//100 : 7/8
		//111~101 : reserved

	}


	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 srdwaittime_7_0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	srdwaittime_7_0 = (UINT8)(waitMs / 10);

	Sync_Wr01(srd_ctrl9,srdwaittime_7_0,srdwaittime_7_0);
	Sync_WrFL(srd_ctrl9);  //regAddr = 0x1019, SRD_CTRL[9] : Set of wait time for symbol rate detection in 10 msec, default : 0x64 (1000ms)
	//SRDWAITTIME[7:0] : SRD wait time for DVB-T, QAM SRD status generation, SRD status becomes fail after srdwaittime. (10ms resolution)

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{

	UINT8	dlyspread_7_0;
	UINT8 	dlyspread_11_8;

	UINT16 delaySpread = 0;

	*pDelaySpread = 0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(cir_mon0);  //regAddr = 0x00c6, CIR_MON[0]
	Common_Rd01(cir_mon0,dlyspread_11_8,dlyspread_11_8); //DLYSPREAD[11:8] : Delay spread indication of channel for DVB-T/ISDB-T

	delaySpread = (dlyspread_11_8  << 8);

	Common_RdFL(cir_mon1);  //regAddr = 0x00c7, CIR_MON[1]
	Common_Rd01(cir_mon1,dlyspread_7_0,dlyspread_7_0); //DLYSPREAD[7:0]

	delaySpread += (dlyspread_7_0 << 0);

	*pDelaySpread = delaySpread;

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_Hierach_HPSel
 * Get the information if the hierachical HP is selected.
 *
 * @param *pBSetHp 	[OUT] BOOLEAN - HP (TRUE) / LP(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp )
{

	UINT8 	hierhpsel_d;

	*pBSetHp = TRUE;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(fec_ctrl1);   //regAddr = 0x0075, FEC_CTRL[1]
	Common_Rd01(fec_ctrl1, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority


	if(hierhpsel_d)	*pBSetHp = TRUE;
	else			*pBSetHp = FALSE;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{

	UINT8 	modelock_di;
	UINT8 	fftmoder_di_1_0;

	*pTxModeLock = LX_DEMOD_UNLOCKED;
	*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(dieq_mon0);  //regAddr = 0x00aa, DIEQ_MON[0]
	Common_Rd01(dieq_mon0,modelock_di,modelock_di); //MODELOCK_DI : Transmission mode lock status indication for DVB-T/ISDB-T
															// 0-> unlock, 1-> lock
	if(modelock_di)
	{
		*pTxModeLock = LX_DEMOD_LOCKED;

		DEMOD_PRINT("[DEMOD L9] TX mode LOCKED !!!\n");

		Common_RdFL(dieq_mon0);  //regAddr = 0x00aa, DIEQ_MON[0]
		Common_Rd01(dieq_mon0,fftmoder_di_1_0,fftmoder_di_1_0);
		//FFTMODER_DI[1:0] :FFT mode indication which is detected in the mode detector
		// 00 : 2K
		// 01 : 8K
		// 10 : 4K (ISDB-T only)
		// 11 : Reserved

		switch(fftmoder_di_1_0)
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
		DEMOD_PRINT("[DEMOD L9] TX mode is NOT LOCKED !!!\n");
	}

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{

	UINT8 	gimoder_di_1_0;

	*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(dieq_mon0);  //regAddr = 0xaa, DIEQ_MON[0]
	Common_Rd01(dieq_mon0,gimoder_di_1_0,gimoder_di_1_0);
	//GIMODER_DI[1:0] : Guard Interval(GI) mode indication which is detected in the mode detector
	// 00 : 1/32
	// 01 : 1/16
	// 10 : 1/8
	// 11 : 1/4

	switch(gimoder_di_1_0)
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


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_HierachyMode
 * Get the information of hierachical mode on DVB-T.
 *
 * @param *pHierachMode[OUT] DEMOD_L9_DVB_TPS_HIERACHY_T - alpha = none, 1, 2 and 4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode )
{

	UINT8 	tpshiermoder_d_2_0;

	*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(deq_mon2);  //regAddr = 0x00af, DEQ_MON[2]
	Common_Rd01(deq_mon2,tpshiermoder_d_2_0,tpshiermoder_d_2_0);
	//TPSHIERMODER_D[2:0] :Hierarchical mode for DVB-T indecator which is detected in the TPS decoder
	// 000 : a = non-hierarchical
	// 001 : a = 1
	// 010 : a = 2
	// 011 : a = 4
	// 111~100 : reserved

	switch(tpshiermoder_d_2_0)
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
 * DEMOD_L9_DVBT_Get_LpCoderRate
 * Get the information of LP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate )
{

	UINT8 	tpslpcoderater_d_2_0;

	*pLpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(deq_mon3);  //regAddr = 0x00b0, DEQ_MON[3]
	Common_Rd01(deq_mon3,tpslpcoderater_d_2_0,tpslpcoderater_d_2_0);
	//TPSLPCODERATER_D[2:0] : LP code rate for DVB-T indecator which is detected in the TPS decoder
	// 000 : 1/2
	// 001 : 2/3
	// 010 : 3/4
	// 011 : 5/6
	// 100 : 7/8
	// 111~101 : reserved

	switch(tpslpcoderater_d_2_0)
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
 * DEMOD_L9_DVBT_Get_HpCoderRate
 * Get the information of HP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate )
{

	UINT8 	tpshpcoderater_d_2_0;

	*pHpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(deq_mon3);  //regAddr = 0x00b0, DEQ_MON[3]
	Common_Rd01(deq_mon3,tpshpcoderater_d_2_0,tpshpcoderater_d_2_0);
	//TPSHPCODERATER_D[2:0]: HP code rate for DVB-T indecator which is detected in the TPS decoder
	// 000 : 1/2
	// 001 : 2/3
	// 010 : 3/4
	// 011 : 5/6
	// 100 : 7/8
	// 111~101 : reserved

	switch(tpshpcoderater_d_2_0)
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
 * DEMOD_L9_DVBT_GetCellId
 * Get TPS cell ID on DVB-T.
 *
 * @param *pCellIDLength[OUT] UINT8 - cell ID length.
 * @param *cell_id[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id)
{

	UINT8	tpslengthr_d_5_0;
	UINT8 	tpscellidr_d_7_0;
	UINT8 	tpscellidr_d_15_8;

	UINT16 tempCellID = 0;

	*pCellIDLength = 0;
	*cell_id = 0;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(deq_mon1);  //regAddr = 0x00ae, DEQ_MON[1]
	Common_Rd01(deq_mon1,tpslengthr_d_5_0,tpslengthr_d_5_0);
	//TPSLENGTHR_D[5:0] : Monitoring for TPS LENGTH

	*pCellIDLength = tpslengthr_d_5_0;

	Common_RdFL(deq_mon4);  //regAddr = 0x00b1, DEQ_MON[4]
	Common_Rd01(deq_mon4,tpscellidr_d_15_8,tpscellidr_d_15_8); 	//TPSCELLIDR_D[15:8] : Monitoring for CELLID (in DVB-T TPS decoder)

	tempCellID = (tpscellidr_d_15_8  << 8);

	Common_RdFL(deq_mon5);  //regAddr = 0x00b2, DEQ_MON[5]
	Common_Rd01(deq_mon5,tpscellidr_d_7_0,tpscellidr_d_7_0); //TPSCELLIDR_D[7:0] : Monitoring for CELLID (in DVB-T TPS decoder)
	tempCellID += (tpscellidr_d_7_0  << 0);

	*cell_id = tempCellID;

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_TpsInfo
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams)
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


	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	if(NULL == pTpsParams)
	{
		DEMOD_PRINT("[DEMOD L9] ERROR, DEMOD_L9_DVBT_CONFIG_T TpsParams is NULL !!!\n");
		return RET_ERROR;
	}

	// Get bSetHp

	if (RET_OK != DEMOD_L9_DVBT_Get_Hierach_HPSel( &bSetHp ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_Hierach_HPSel() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->bSetHp = bSetHp;
	DEMOD_PRINT("[DEMOD L9] Set HP selection = %s !!!\n",(bSetHp == TRUE) ? " HP Mode" : " LP Mode");


	// Get channelBW

	if (RET_OK != DEMOD_L9_Get_BandWidthMode( &channelBW ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_BandWidthMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->channelBW = channelBW;
	DEMOD_PRINT("[DEMOD L9] Bandwidth = %s !!!\n",
		(channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	if (RET_OK != DEMOD_L9_DVBT_Get_FFTMode( &txModeLock, &FFTMode ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->FFTMode = FFTMode;
	DEMOD_PRINT("[DEMOD L9] Tx mode = %s, FFTMode = %s  !!!\n",
		(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked",
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	if (RET_OK != DEMOD_L9_DVBT_Get_GuradIntervalMode( &gIMode ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->gIMode = gIMode;
	DEMOD_PRINT("[DEMOD L9] Guard Intterval = %s  !!!\n",
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );



	// Get Hierachical mode
	if (RET_OK != DEMOD_L9_DVBT_Get_HierachyMode( &hierachMode ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_HierachyMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hierachMode = hierachMode;
	DEMOD_PRINT("[DEMOD L9] Hierachical Mode = %s  !!!\n",
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	if (RET_OK != DEMOD_L9_DVBT_Get_LpCoderRate( &lpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_LpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->lpCodeRate = lpCodeRate;
	DEMOD_PRINT("[DEMOD L9] LP code rate = %s  !!!\n",
		(lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate

	if (RET_OK != DEMOD_L9_DVBT_Get_HpCoderRate( &hpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_HpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hpCodeRate = hpCodeRate;
	DEMOD_PRINT("[DEMOD L9] HP code rate = %s  !!!\n",
		(hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	if (RET_OK != DEMOD_L9_Get_QAMMode(&qamMode ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVB_Get_QAMMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->qamMode = qamMode;
	DEMOD_PRINT("[DEMOD L9] modulation  = %s  !!!\n",
		(qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_TotalInfo
 * Get all information on DVB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_TotalInfo( void)
{

	LX_DEMOD_DVBT_CONFIG_T 		tppsParams;
	LX_DEMOD_LOCK_STATE_T 	agcLockStatus;
	LX_DEMOD_LOCK_STATE_T 	neverLockStatus;
	LX_DEMOD_LOCK_STATE_T 	txModeLock;
	LX_DEMOD_FFTMODE_T 			fftMode;

	UINT16 					delaySpread;
	UINT16 					ifagcValue = 0;
	LX_DEMOD_TPERRCNTMODE_T errorMode;
	UINT32 					errorCount;
	SINT32 					FreqOff;

	LX_DEMOD_LOCK_STATE_T spectrumAuto;
	LX_DEMOD_LOCK_STATE_T spectrumInv;

	LX_DEMOD_MSE_T sigMse;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);


	if (RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_AGC_LOCK, &agcLockStatus))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
		agcLockStatus = LX_DEMOD_UNKOWN_STATE;
	}
	if(LX_DEMOD_LOCKED == agcLockStatus)
	{
		if (RET_OK != DEMOD_L9_Get_IFAGC(&ifagcValue))
		{
			DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_IFAGC() !!!\n", __F__, __L__);
			ifagcValue = 0;
		}
	}

	if (RET_OK != DEMOD_L9_Get_NeverLockStatus(LX_DEMOD_DVBT, &neverLockStatus))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVB_Get_NeverLockStatus() !!!\n", __F__, __L__);
		neverLockStatus = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK !=  DEMOD_L9_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK != DEMOD_L9_DVBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}

	if (RET_OK != DEMOD_L9_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_SpectrumInvStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}
	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_L9_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD L9] TP Error Count= %d !!!\n",errorCount);

	if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset( &FreqOff ))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		FreqOff = 0xffff;
	}

 	//MSE

	if (RET_OK != DEMOD_L9_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD L9] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD L9] IF AGC value = %d(0x%x) !!!\n",ifagcValue,ifagcValue);
	DEMOD_PRINT("[DEMOD L9] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_DETECTED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD L9 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD L9] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD L9] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD L9] offset = %d [KHz]!!!\n",FreqOff);
	DEMOD_PRINT("[DEMOD L9] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	if (RET_OK != DEMOD_L9_DVBT_Get_TpsInfo( &tppsParams))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Get_TpsInfo() !!!\n", __F__, __L__);
		return RET_ERROR;
	}




#ifndef  DEMOD_PRINT

	printk("[DEMOD L9] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD L9] IF AGC value = %d !!!\n",ifagcValue);
	printk("[DEMOD L9] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD L9] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	printk("[DEMOD L9] Delay Spread = %d !!!\n",delaySpread);
	printk("[DEMOD L9] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");


	// Get bSetHp
	printk("[DEMOD L9] Set HP selection = %s !!!\n",(tppsParams->bSetHp == TRUE) ? " HP Mode" : " LP Mode");

	// Get channelBW
	printk("[DEMOD L9] Bandwidth = %s  !!!\n",
		(tppsParams.channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	printk("[DEMOD L9] FFTMode = %s  !!!\n",
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_4K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	printk("[DEMOD L9] Guard Intterval = %s  !!!\n",
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );

	// Get Hierachical mode
	tppsParams->hierachMode = hierachMode;
	printk("[DEMOD L9] Hierachical Mode = %s  !!!\n",
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	printk("[DEMOD L9] LP code rate = %s  !!!\n",
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate
	printk("[DEMOD L9] HP code rate = %s  !!!\n",
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	printk("[DEMOD L9] LP code rate = %s  !!!\n",
		(tppsParams.qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(tppsParams.qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(tppsParams.qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );

	printk("[DEMOD L9] TP Error Count= %d !!!\n",errorCount);
	printk("[DEMOD L9] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);

#endif

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_IFO_LOCK
 * Get IFO lock on DVB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{
	UINT8	syslock;
	UINT16 	intfreqoffsetvalue = 0;
	UINT8	crifo_di_7_0;
	UINT8	crifo_di_9_8;

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,syslock,syslock); //SYSLOCK : System lock is come from ifolock for DVB-T, from  framelock for DVB-C


	Sync_RdFL(carrier_mon1);  //regAddr = 0x10a4, CARRIER_MON[1]
	Sync_Rd01(carrier_mon1,crifo_di_9_8,crifo_di_9_8); //CRIFO_DI[9:8] : Indicates IFO offset

	Sync_RdFL(carrier_mon2);  //regAddr = 0x10a5, CARRIER_MON[2]
	Sync_Rd01(carrier_mon2,crifo_di_7_0,crifo_di_7_0); //CRIFO_DI[7:0] : Indicates IFO offset

	intfreqoffsetvalue = (crifo_di_9_8 << 8) + crifo_di_7_0;

	DEMOD_PRINT("[DEMOD L9](%s) IFO LOCK = %s !!!\n", __F__, (syslock == 1) ? "LOCK" :"NOT LOCK");
	DEMOD_PRINT("[DEMOD L9](%s) IFO value = %d !!!\n", __F__,intfreqoffsetvalue);

	if(syslock == 1)
	{
		*pIfoLock = TRUE;
	}
	else
	{
		*pIfoLock = FALSE;
}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on DVB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{
	UINT8 aiccrejbusy_3_0;
	UINT8 aiccrejstatus_3_0;

	Common_RdFL(sync_mon2); //regAddr = 0x0090, sync_mon2
	Common_Rd02(sync_mon2,aiccrejbusy_3_0,aiccrejbusy_3_0, aiccrejstatus_3_0,aiccrejstatus_3_0);
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

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_L9_DVBT_EqualizereReset(void)
{

	UINT8 softrsteqb;

	softrsteqb = 0; //SOFTRSTEQB -> 0 : reset Equalizer

	DEMOD_PRINT("^p^[DEMOD L9 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
	Common_Wr01(common_ctrl0, softrsteqb, softrsteqb); //SOFTRSTEQB : Equalizer function reset control(0 : reset, 1 : normal operation)
	Common_WrFL(common_ctrl0);

	OS_MsecSleep(20);

	softrsteqb = 1; //SOFTRSTEQB -> 1 : normal

	Common_Wr01(common_ctrl0, softrsteqb, softrsteqb);
	Common_WrFL(common_ctrl0);

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;


	if(pDemodInfo->bSetParams)
	{
		switch (pDemodInfo->controlState)
		{

												/***************************************************/
												/***** Configure demod	for RETUNE. *****/
			case LX_DEMOD_RETUNE_RF:			/***************************************************/
				{

					if(RET_OK != DEMOD_L9_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BW_8MHZ))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_L9_DVBT_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
					{
						if(RET_OK != DEMOD_L9_DVBT_Set_Config_auto(TRUE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
					}
					else
					{

#if 1
						if(RET_OK != DEMOD_L9_DVBT_Set_Config_auto(TRUE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

#else
						if(RET_OK != DEMOD_L9_DVBT_Set_Config_auto(FALSE))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

						if(RET_OK != DEMOD_L9_DVBT_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt)))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_DVBT_Set_FullConfig() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}

#endif

					}

					pDemodInfo->controlState = LX_DEMOD_TUNE_START;
					DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
					g_RetuneStartTime = jiffies_to_msecs(jiffies);

				}

				break;


												/***************************************************/
												/***** Configure tuner for desired channel No. *****/
			case LX_DEMOD_TUNE_START:			/***************************************************/
				{


					if(RET_OK != DEMOD_L9_SoftwareReset())
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
					{

						if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
						{
							DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


					}
					else
					{


						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

						DEMOD_PRINT( "^c^[DEMOD L9] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
						return RET_OK;
					}

					if(RET_OK != DEMOD_L9_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
						pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

					}
					else if((LX_DEMOD_FLAG_FINISH == scanProcessing)&& (LX_DEMOD_UNLOCKED == scanLock))
					{
						pDemodInfo->scanJobFlag 	= LX_DEMOD_FLAG_FINISH;
						pDemodInfo->bNotifiedUnlock	= TRUE;
						pDemodInfo->bNotifiedLock	= FALSE;

						pDemodInfo->controlState 	= LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK;



						currentTime = jiffies_to_msecs(jiffies);
						elapsedTime = currentTime - g_NewTuneReqeustTime;

						DEMOD_PRINT("\nMSG_DVBT_FRONT_END_NOT_LOCKED..\n") ;
						DEMOD_PRINT("Elapse time is %d msec\n", elapsedTime) ;

						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);



					}
					else
					{
						pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
						DEMOD_PRINT( "^G^[DEMOD L9] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(LX_DEMOD_LOCKED == lockState)
					{
						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

						sigStablestartTime = jiffies_to_msecs(jiffies);

						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
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

					if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
					{
						DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Lock() !!!\n", __F__, __L__);
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
								if(RET_OK != DEMOD_L9_Get_Packet_Error(&packetError))
								{
									DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_Packet_Error() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
								else
								{
									if(packetError < LX_DEMOD_PACKET_ERROR_THD)
									{
										pDemodInfo->bNotifiedLock	= TRUE;
										pDemodInfo->bNotifiedUnlock	= FALSE;

										if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
										DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

									}
									else
									{
										elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
										if(elapsedTime > LX_DEMOD_LOCK_DEALY_100)
										{
											pDemodInfo->bNotifiedLock	= TRUE;
											pDemodInfo->bNotifiedUnlock = FALSE;

											if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
											{
												DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_L9_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
												DEMOD_RETURN_FAIL(__F__);
											}
											pDemodInfo->freqOffset = freqOffset;

											pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

											elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
											DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);

										}

									}

								}

							}
						}
						else
						{
							pDemodInfo->bNotifiedLock	= TRUE;
							pDemodInfo->bNotifiedUnlock = FALSE;

							if (RET_OK != DEMOD_L9_Get_CarrierFreqOffset(&freqOffset))
							{
								DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
								DEMOD_RETURN_FAIL(__F__);
							}
							pDemodInfo->freqOffset = freqOffset;

							pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

							elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : VSB_LOCK_OK ..... ^0^\n");
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);
							DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^\n", elapsedTime);


						}

						if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD L9] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
						{
							DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}



					}
					else
					{
						pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
						DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

					}


		   		}
				break;

			case LX_DEMOD_SIGNAL_LOCKED:	break;

			default: pDemodInfo->controlState = LX_DEMOD_TUNE_START; break;

		}
	}


	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_L9_DVBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_L9_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_L9_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD L9] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(LX_DEMOD_LOCKED == lockState)
	{
		pDemodInfo->unLockCount = 0;
		pDemodInfo->unLockLongCount	= 0;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;

		}
		if(pDemodInfo->lockCount == 4)
		{
			pDemodInfo->lockStatus = LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock = TRUE;
			if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
				pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		}
		if(pDemodInfo->lockCount > 4)
		{
			pDemodInfo->lockCount = 0;
		}


	}
	else
	{
		pDemodInfo->lockCount = 0;
		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (FALSE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus = LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;

			}
			if(pDemodInfo->unLockCount > 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 200)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD L9] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

			}
			if(pDemodInfo->unLockLongCount > 200)
			{
				pDemodInfo->unLockLongCount = 0;
			}
		}
		if((pDemodInfo->controlState == LX_DEMOD_RETUNE_RF) && (pDemodInfo->bSetParams != FALSE))
		{
			pDemodInfo->acqState					= LX_DEMOD_TUNNING_PROCESS;

			pDemodInfo->scanJobFlag 				= LX_DEMOD_FLAG_START;

			pDemodInfo->lockCount					= 0;
			pDemodInfo->unLockCount 				= 0;
			pDemodInfo->unLockLongCount				= 0;

			pDemodInfo->signalStatus.agc			= 0;
			pDemodInfo->signalStatus.ber			= 0xffff;
			pDemodInfo->signalStatus.bSignalValid	= FALSE;
			pDemodInfo->signalStatus.m.mse			= 0xffff;
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1500;


		}



	}


	return RET_OK;



}




#if 0
#endif



