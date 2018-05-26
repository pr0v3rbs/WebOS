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
#include "demod_reg_h13.h"
#include "demod_common_h13.h"
#include "demod_dvb_h13.h"


#if 0
#endif

/***********************************************************************************
	Global Type Definitions
 ************************************************************************************/

 UINT16 _gFullDVBPkerrCount = 9;
UINT16 _gFullDVBPkerrThreshold = 170;

UINT8 gPowerOnInitDVB = 1;
unsigned int 			gPowerOnInitStartTimeDVB = 0;


/***********************************************************************************
	Static Variable & Function Prototypes Declarations
 ************************************************************************************/
static	UINT16			_gGBB_DVB_UnlockCount_full_packeterr	= 0;


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_AutoSymbolRateDet
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  benable - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMRDET_MODE_T symrateMode, UINT16 SymbolRate )
{
	UINT8	stdopdetcmode_1_0 = 0;
	UINT64	symbol_temp = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

#if 1
	switch(symrateMode)
	{

		case LX_DEMOD_SYMR_FIXED_MODE :
		{
			UINT8	srdfbsymrate_7_0;
			UINT8	srdfbsymrate_15_8;
			UINT8	srdfbsymrate_16;

			//set symbol rate mode
			stdopdetcmode_1_0 = 0x1;

			//Manual symbol rate setting
			//SRDFBSYMRATE = symbol rate(Mhz) x 2^13

			symbol_temp = ((SymbolRate * 8192) + 500) / 1000;

			srdfbsymrate_7_0 = (UINT8) (symbol_temp & 0xff);
			srdfbsymrate_15_8 = (UINT8) ((symbol_temp >> 8) & 0xff);
			srdfbsymrate_16 = 0;
			DEMOD_PRINT("[DEMOD H13] symbol_temp = 0x%llx \n",symbol_temp);
			DEMOD_PRINT("[DEMOD H13] srdfbsymrate0_15_8 = 0x%x \n",srdfbsymrate_15_8);
			DEMOD_PRINT("[DEMOD H13] srdfbsymrate0_7_0 = 0x%x \n",srdfbsymrate_7_0);

			Sync_Wr01(sync_ctrl6, srdfbsymrate_16, srdfbsymrate_16);
			Sync_WrFL(sync_ctrl6);  //regAddr = 0x1006, SYNC_CTRL[6], SRDFBSYMRATE[16]

			Sync_Wr01(sync_ctrl7, srdfbsymrate_15_8, srdfbsymrate_15_8); //SRDFBSYMRATE[15:8]:
			Sync_WrFL(sync_ctrl7);  //regAddr = 0x1007, SYNC_CTRL[7]

			Sync_Wr01(sync_ctrl8, srdfbsymrate_7_0, srdfbsymrate_7_0); //SRDFBSYMRATE0[7:0]
			Sync_WrFL(sync_ctrl8);  //regAddr = 0x1008, SYNC_CTRL[8]


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


	Common_RdFL(common_ctrl9);   //regAddr = 0x0009, COMMON_CTRL[9]
	Common_Wr01(common_ctrl9,stdopdetcmode_1_0, stdopdetcmode_1_0 );
	Common_WrFL(common_ctrl9);
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
 * DEMOD_H13_DVBC_IsSymbolRateAuto
 * Enable/Diable automatic symbol rate detection on DVB-C mode.
 *
 * @param  *bAuto - BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_IsSymbolRateAuto(BOOLEAN *bAuto)
{


	UINT8	stdopdetcmode_1_0 = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl9);  //regAddr = 0x0009, COMMON_CTRL[9]
	Common_Rd01(common_ctrl9, stdopdetcmode_1_0,stdopdetcmode_1_0);

	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	//Pre-defined QAM mode is set by QAMMODE[0](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM

	if ((0 == stdopdetcmode_1_0) || (1 == stdopdetcmode_1_0))
	{
		*bAuto = FALSE;
		DEMOD_PRINT("[DEMOD H13] Auto Symbolrate detection is disabled  !!!\n");
	}
	else
	{
		*bAuto = TRUE;
		DEMOD_PRINT("[DEMOD H13] Auto Symbolrate detection is enabled  !!!\n");
	}



	return RET_OK;
}






/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_IsQammodeAutoDet
 * Control automatic setting of DVBC config.
 *
 * @param *bAutoQam 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_IsQammodeAutoDet (BOOLEAN *bAutoQam)
{

	UINT8	qmden;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl10);   //regAddr = 0x000a, COMMON_CTRL[10]
	Common_Rd01(common_ctrl10, qmden, qmden); //QMDEN : QAM Mode Detection(QMD) function control : 1-> enable, 0->disable

	if (1 == qmden)
	{
		*bAutoQam = TRUE;
		DEMOD_PRINT("[DEMOD H13] Auto QAM mode detection is enabled  !!!\n");
	}
	else
	{
		*bAutoQam = FALSE;
		DEMOD_PRINT("[DEMOD H13] Auto QAM mode detection is disabled  !!!\n");
	}

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Set_DefaultRegisterValue(void)
{

	UINT8 qmdqammode0_2_0;
	UINT8 qmdqammode1_2_0;
	UINT8 qmdqammode2_2_0;
	UINT8 qmdqammode3_2_0;
	UINT8 qmdqammode4_2_0;

	UINT8 adcsel1v;
	UINT8 adcclkpllsel;

	UINT8 nousenosigdet;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


/*****************************************************************************************************************/
	// setting the sequence of QAM mode for auto detection (16QAM -> 32QAM -> 64QAM -> 128QAM -> 256QAM )
	qmdqammode0_2_0 = 2;  // 1st QAM (64QAM)
	qmdqammode1_2_0 = 4;  // 2nd QAM (256QAM)
	qmdqammode2_2_0 = 0;  // 3rd QAM (16QAM)
	qmdqammode3_2_0 = 1;  // 4th QAM (32QAM)
	qmdqammode4_2_0 = 3;  // 5th QAM (128QAM)

	Sync_RdFL(sync_ctrl26);  //regAddr = 0x101a, SYNC_CTRL[26]
	Sync_Wr02(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0, qmdqammode1_2_0, qmdqammode1_2_0); //QMDQAMMODE0[2:0], QMDQAMMODE1[2:0]
	Sync_WrFL(sync_ctrl26);

	Sync_RdFL(sync_ctrl25);  //regAddr = 0x1019, SYNC_CTRL[25]
	Sync_Wr02(sync_ctrl25, qmdqammode2_2_0, qmdqammode2_2_0, qmdqammode3_2_0, qmdqammode3_2_0); //QMDQAMMODE2[2:0], QMDQAMMODE3[2:0]
	Sync_WrFL(sync_ctrl25);

	Sync_RdFL(sync_ctrl24);  //regAddr = 0x1018, SYNC_CTRL[24]
	Sync_Wr01(sync_ctrl24, qmdqammode4_2_0, qmdqammode4_2_0); //QMDQAMMODE4[2:0]
	Sync_WrFL(sync_ctrl24);

	//QMDQAMMODE0[2:0] : Set the 1st QAM mode for DVB-C
	//This signal is used QAM mode signal at single QAM mode detection for QAM(J.83), DVB-C, ISDB-C
	//QAM and ISDB-C has only 64QAM and 256QAM so only QMDQAMMODE0[2] is valid for QAM mode set(0 means 64QAM, 1 means 256QAM)
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q (default)
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection

	//QMDQAMMODE1[2:0]:	Set the 2nd QAM mode for DVB-C, This is valid at QAM mode auto detection mode
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection (default)

	//QMDQAMMODE2[2:0] : Set the 3rd QAM mode for DVB-C
	//This is valid at QAM mode auto detection mode
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection (default)

	//QMDQAMMODE3[2:0] : Set the 4th QAM mode for DVB-C
	//This is valid at QAM mode auto detection mode
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection (default)

	//QMDQAMMODE4[2:0] : Set the 5th QAM mode for DVB-C
	//This is valid at QAM mode auto detection mode
	// 000 : 16Q
	// 001 : 32Q
	// 010 : 64Q
	// 011 : 128Q
	// 100~110 : 256Q
	// 111 : not check this register at QAM mode detection (default)
/*****************************************************************************************/

	adcsel1v = 1;
	adcclkpllsel = 1;

	Common_RdFL(common_ctrl4);	 //regAddr = 0x0004[7], COMMON_CTRL[4]
	Common_Wr02(common_ctrl4, adcsel1v, adcsel1v, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	//ADCCLKPLLSEL : Selection of ADC sampling clock source.  This is not validated at DTV SoC
	// 0 : use external oscillator input
	// 1 : use PLL output

	nousenosigdet = 1;
	EQ_CQS_RdFL(eq_cqs_ctl73);	 //regAddr = 0x2849, eq_cqs_ctl[73]
	EQ_CQS_Wr01(eq_cqs_ctl73, nousenosigdet, nousenosigdet);
	EQ_CQS_WrFL(eq_cqs_ctl73);
	//NOUSENOSIGDET : Selection of input signal for gain error calculation
	//Disable no signal detector function
	// 0 : enable no signal detector function
	// 1 : disable no signal detector function

	_gGBB_DVB_UnlockCount_full_packeterr = 0;

	return RET_OK;

}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Set_Config_auto
 * Control automatic setting of DVBC config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Set_Config_auto (BOOLEAN bAutoDetect)
{

	UINT8	qmden;
	UINT8 	qmdqammode0_2_0;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	qmden = (bAutoDetect == TRUE)?(1):(0);  //QMDEN
	Common_RdFL(common_ctrl10);   //regAddr = 0x000a, COMMON_CTRL[10]
	Common_Wr01(common_ctrl10, qmden, qmden);
	Common_WrFL(common_ctrl10);
	//QMDEN : QAM Mode Detection(QMD) function control
	//This is only valid for cable standard(DVB-C,QAM)
	// 0 : disable
	// 1 : enable


	if(bAutoDetect == TRUE)
	{
		//QAM mode setting
		qmdqammode0_2_0 = 0x2; //return to default (64 QAM)
		Sync_RdFL(sync_ctrl26);  //regAddr = 0x101a, SYNC_CTRL[26]
		Sync_Wr01(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0);
		Sync_WrFL(sync_ctrl26);
		//QMDQAMMODE0[2:0] : Set the 1st QAM mode for DVB-C
		//This signal is used QAM mode signal at single QAM mode detection for QAM(J.83), DVB-C, ISDB-C
		//QAM and ISDB-C has only 64QAM and 256QAM so only QMDQAMMODE0[2] is valid for QAM mode set(0 means 64QAM, 1 means 256QAM)
		// 000 : 16Q
		// 001 : 32Q
		// 010 : 64Q (default)
		// 011 : 128Q
		// 100~110 : 256Q
		// 111 : not check this register at QAM mode detection
	}

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Set_QamMode
 * Set DVBC config parameters manually.
 *
 * @param SymbolRate 	[IN] UINT16 - symbol rate : unit: KHz.
 * @param constel 		[IN] UINT8 - QAM mode : 16QAM(0), 32QAM(1), 64QAM(2), 128QAM(3), 256QAM(4).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Set_QamMode (LX_DEMOD_RF_MODE_T constel)
{

	UINT8 	qmdqammode0_2_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	switch(constel)
	{
		case LX_DEMOD_16QAM:
			qmdqammode0_2_0 = 0;
			break;

		case LX_DEMOD_32QAM:
			qmdqammode0_2_0 = 1;
			break;

		case LX_DEMOD_64QAM:
			qmdqammode0_2_0 = 2;
			break;
			
		case LX_DEMOD_128QAM:
			qmdqammode0_2_0 = 3;
			break;
			
		case LX_DEMOD_256QAM:
			qmdqammode0_2_0 = 4;
			break;
		default:
			qmdqammode0_2_0 = 4;
			break;

	}


	//QAM mode setting
	qmdqammode0_2_0 = constel;
	Sync_RdFL(sync_ctrl26);  //regAddr = 0x101a, SYNC_CTRL[26]
	Sync_Wr01(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0);
	Sync_WrFL(sync_ctrl26);
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
 * DEMOD_H13_DVBC_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 scsyslockchktime_7_0;
	UINT8 qmdwaittime_7_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = 0x3c; //600ms
	Common_Wr01(common_ctrl11,scsyslockchktime_7_0,scsyslockchktime_7_0);
	Common_WrFL(common_ctrl11);  //regAddr = 0x000b,COMMON_CTRL[11]
	//SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in 10msec resolution

	qmdwaittime_7_0 = (UINT8)(waitMs / 10);
	Sync_Wr01(sync_ctrl23,qmdwaittime_7_0,qmdwaittime_7_0);
	Sync_WrFL(sync_ctrl23);  //regAddr = 0x1017,SYNC_CTRL[23]
	//QMDWAITTIME[7:0] : Set of wait time for QAM mode detection in 10msec

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Get_SymbolRateDetect
 * Get symbol rate in KHz (Don't use this function until revision A1)
 *
 * @param *pSymbolRate 	[OUT] UINT16 - symbol rate : unit: KHz.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Get_SymbolRateDetect(UINT16 *pSymbolRate)
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

		Common_RdFL(common_mon5);  //regAddr = 0x0085, COMMON_MON[5]
		Common_Rd01(common_mon5,detsymrate_16,detsymrate_16); //DETSYMRATE[16]
		tempSymbolRate += (detsymrate_16 << 16 ) ;

		Common_RdFL(common_mon6);  //regAddr = 0x0086, COMMON_MON[6]
		Common_Rd01(common_mon6,detsymrate_15_8,detsymrate_15_8); //DETSYMRATE[15:8]
		tempSymbolRate += (detsymrate_15_8 << 8 ) ;

		Common_RdFL(common_mon7);  //regAddr = 0x0087, COMMON_MON[6]
		Common_Rd01(common_mon7,detsymrate_7_0,detsymrate_7_0); //DETSYMRATE[7:0]
		tempSymbolRate += (detsymrate_7_0 << 0 ) ;

		symbolRateKHz = (tempSymbolRate * 10000) / 8192 ; // symbol rate [KHz] = SYMBOLRATEDET x 2^(-13) * 1000 , 2^13 = 8192

		tempSymbolRateKHz = ((tempSymbolRate * 1000) / 8192) * 10 ;

		if((symbolRateKHz -  tempSymbolRateKHz) > 4)

			symbolRateKHz = (symbolRateKHz + 10)/10;
		else
			symbolRateKHz = (symbolRateKHz + 0)/10;

		*pSymbolRate = (UINT16)(symbolRateKHz);

		DEMOD_PRINT("[DEMOD H13] Symbol rate is %d [KHz]!!!\n",symbolRateKHz);


		return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Get_SymbolRateStatus
 * Get symbol rate detection state on DVB-C
 * Don't use this function until revision A1.
 *
 * @param *pSymbolRateDet 	[OUT] SYMBOLRATE_STATE_T - status and detected symbol rate .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Get_SymbolRateStatus(LX_DEMOD_SYMBOLRATE_STATE_T *pSymbolRateDet)
{

	UINT8 	srdlock;

	UINT16 symbolRate;

	pSymbolRateDet->status = LX_DEMOD_UNKOWN_STATE;
	pSymbolRateDet->symbolRate = 0;

	// Symbol rate
	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,srdlock,srdlock); //SRDLOCK: Symbol Rate Detection(SRD) status indication(0 : unlock, 1 : lock)

	if(1 == srdlock)
	{
		pSymbolRateDet->status = LX_DEMOD_DETECTED;
		DEMOD_PRINT("[DEMOD H13] Success (%s:%d) Symbol rate is detected !!!\n", __F__, __L__);
		if (RET_OK != DEMOD_H13_DVBC_Get_SymbolRateDetect(&symbolRate))
		{
			DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) Symbol rate couldn't be read !!!\n", __F__, __L__);
			return RET_ERROR;
		}
		pSymbolRateDet->symbolRate = symbolRate;
	}
	else
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) Symbol rate couldn't be detected !!!\n", __F__, __L__);
		pSymbolRateDet->status = LX_DEMOD_DETECTION_FAIL;

	}


	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBC_Get_QamModeDetectStatus
 * Get QAM mode detection state and detected QAM mode on DVB-C
 *
 * @param *pQamModeDet 	[OUT] SYMBOLRATE_STATE_T - status and detected QAM mode .
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Get_QamModeDetectStatus(LX_DEMOD_QAMMODE_STATE_T *pQamModeDet)
{

	UINT8 qmdstatus_1_0 = 0;
	UINT8 qmden = 0;
	UINT8 qmdqammode0_2_0 = 0;

	LX_DEMOD_RF_MODE_T qamMode;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	pQamModeDet->status = LX_DEMOD_UNKOWN_STATE;
	pQamModeDet->qamMode = LX_DEMOD_UNKOWN_STATE;

	Common_RdFL(common_ctrl10);   //regAddr = 0x000a, COMMON_CTRL[10]
	Common_Rd01(common_ctrl10, qmden, qmden); //QMDEN : QAM Mode Detection(QMD) function control : 1-> enable, 0->disable

	if(qmden == 1)
	{
		Sync_RdFL(sync_mon1);  //regAddr = 0x1081, SYS_MON[1]
		Sync_Rd01(sync_mon1,qmdstatus_1_0,qmdstatus_1_0);
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

				if (RET_OK != DEMOD_H13_Get_QAMMode(&qamMode ))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVB_Get_QAMMode() !!!\n", __F__, __L__);
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

		Sync_RdFL(sync_ctrl26);  //regAddr = 0x101a, SYNC_CTRL[26]
		Sync_Rd01(sync_ctrl26, qmdqammode0_2_0, qmdqammode0_2_0); //QMDQAMMODE0[2:0]
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
 * DEMOD_H13_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_DVBC_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H13_DVBC_Set_DefaultRegisterValue())
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(RET_OK != DEMOD_H13_DVBC_Set_NeverLockWaitTime(LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
				{
					if(RET_OK != DEMOD_H13_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

				}
				else
				{

#if 1
					if(RET_OK != DEMOD_H13_DVBC_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FULL_SCAN_MODE, LX_DEMOD_SYMBOL_RATE_6900_KHZ))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_H13_DVBC_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBC_AutoSymbolRateDet(LX_DEMOD_SYMR_FIXED_MODE, pDemodInfo->setCfgParam.dvbc.symbolRate))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_AutoSymbolRateDet() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBC_Set_QamMode(pDemodInfo->setCfgParam.dvbc.qamMode))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_QamMode() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}


#endif
				}



				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{

				if(RET_OK != DEMOD_H13_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbc.tuneMode)
				{

					if(RET_OK != DEMOD_H13_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

					DEMOD_PRINT( "^c^[DEMOD H13] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_H13_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBC_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);


				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

				if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{
					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
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

				if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
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
							if(RET_OK != DEMOD_H13_Get_Packet_Error(&packetError))
							{
								DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_VSB_PreMonitor() !!!\n", __F__, __L__);
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

									if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^^0^^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

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

										if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset(&freqOffset))
										{
											DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										pDemodInfo->freqOffset = freqOffset;

										pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

										elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

										DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^^0^^\n");
										DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
										DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

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

						if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset(&freqOffset))
						{
							DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
							DEMOD_RETURN_FAIL(__F__);
						}
						pDemodInfo->freqOffset = freqOffset;

						pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

						elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;


						DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : MSG_DVBC_FRONT_END_LOCKED..... ^0^\n");
						DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^0^^\n", elapsedTime);
						DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^0^\n", freqOffset);



					}

					if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
					{
						DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
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
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

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
 * DEMOD_H13_DVBC_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_DVBC_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;

	if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_FEC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
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

		if(pDemodInfo->signalStatus.packetError >= _gFullDVBPkerrThreshold)
		{
			_gGBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT( "^r^[DEMOD H13](%s) _gGBB_DVB_UnlockCount_full_packeterr = %d !!!\n", __F__,_gGBB_DVB_UnlockCount_full_packeterr);

			if(_gGBB_DVB_UnlockCount_full_packeterr == _gFullDVBPkerrCount)
			{
				DEMOD_PRINT( "^r^[DEMOD H13] oops !! (%s) Force to NOT LOCK !!\n", __F__);
				_gGBB_DVB_UnlockCount_full_packeterr = 0;

				if(RET_OK != DEMOD_H13_DVBC_Set_Config_auto(TRUE))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Set_Config_auto() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H13](%s) DEMOD_H13_DVBT_Set_Config_auto(TRUE) !!!\n", __F__);

				if(RET_OK != DEMOD_H13_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H13](%s) DEMOD_SoftwareReset() !!!\n", __F__);

				pDemodInfo->lockStatus		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;


			}

		}
		else
		{
			_gGBB_DVB_UnlockCount_full_packeterr = 0;

		if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
		{
			pDemodInfo->lockCount++;

		}
		if(pDemodInfo->lockCount == 2)
		{
			pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
			pDemodInfo->bNotifiedLock 	= TRUE;
			pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H13] MONITORING FUNC : MSG_DVBC_FRONT_END_LOCKED..... ^0^\n");

			if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
				pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

		}
		if(pDemodInfo->lockCount >= 2)
		{
			pDemodInfo->lockCount = 0;
		}

		if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
		{
			DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		}

	}

	else
	{
		pDemodInfo->lockCount = 0;
		_gGBB_DVB_UnlockCount_full_packeterr = 0;

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

				DEMOD_PRINT_ALWAYS( "[DEMOD H13] MONITORING FUNC : MSG_DVBC_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

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
 * DEMOD_H13_DVBC_Get_DvbInfo
 * Get all information on DVB-C for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBC_Get_DvbInfo(void)

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

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	//IF AGC
	if (RET_OK != DEMOD_H13_Get_IFAGC(&ifAGCGain))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_IFAGC() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H13] IF AGC Gain = 0x%x !!!\n",ifAGCGain);

 	//NeverLock status
	if (RET_OK != DEMOD_H13_Get_NeverLockStatus(LX_DEMOD_DVBC, &neverLockStatus))
	{

		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_NeverLockStatus() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	switch(neverLockStatus)
	{

		case LX_DEMOD_IDLE :
			DEMOD_PRINT("[DEMOD H13] Initialization process  !!!\n");
			break;

		case LX_DEMOD_DETECTION_PROCESSING :
			DEMOD_PRINT("[DEMOD H13] Signal Checking process !!!\n");
			break;

		case LX_DEMOD_DETECTED :
			DEMOD_PRINT("[DEMOD H13] normal State (available channel)  !!!\n");
			break;

		case LX_DEMOD_DETECTION_FAIL :
			DEMOD_PRINT("[DEMOD H13] neverlock state (empty channel) !!!\n");

			break;

		default :

			DEMOD_PRINT("[DEMOD H13] INVALID neverLockStatus !!!\n");

			break;

	}

	// Carrier LOCK status  and offset monitoring


	if (RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_CARR_LOCK,&carrLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}


	if(carrLockState == LX_DEMOD_LOCKED)
	{

		DEMOD_PRINT("[DEMOD H13] DVB CARRIER is LOCKED  !!!\n");

	}
	else
	{
		DEMOD_PRINT("[DEMOD H13] DVB CARRIER is NOT LOCKED  !!!\n");

	}

	// Sampling frequency LOCK status and offset monitoring
	if (RET_OK != DEMOD_H13_Get_Lock( LX_DEMOD_TR_LOCK, &samplingLock))
	{

		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	if(LX_DEMOD_LOCKED == samplingLock)
	{
		DEMOD_PRINT("[DEMOD H13] Sampling frequency is Locked   !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H13] Sampling frequency is NOT Locked !!!\n");
	}


	//Symbol rate detection

	if (RET_OK != DEMOD_H13_DVBC_IsSymbolRateAuto(&isAutoSymbol))
	{

		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoSymbol)
	{
		if (RET_OK != DEMOD_H13_DVBC_Get_SymbolRateStatus(&symbolRateDet))

		{

			DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_SymbolRateStatus() !!!\n", __F__, __L__);

			return RET_ERROR;

		}

		switch(symbolRateDet.status)

		{

			case LX_DEMOD_IDLE :

			{

				DEMOD_PRINT("[DEMOD H13] Symbol rate status is idle status!!!\n");

				break;

			}

			case LX_DEMOD_DETECTED :

			{

				DEMOD_PRINT("[DEMOD H13] Symbol rate  is detected !!!\n");

				DEMOD_PRINT("[DEMOD H13] Symbol rate = %d [KHz] !!!\n",symbolRateDet.symbolRate);

				break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :

			{

				DEMOD_PRINT("[DEMOD H13] Symbol rate detection is processing !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_FAIL :

			{

				DEMOD_PRINT("[DEMOD H13] Symbol rate detection is failed !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H13] INVALID Symbol rate detection status  !!!\n");

				break;
			}



		}
	}


	// QAM mode detect status and mode monitoring
	if (RET_OK != DEMOD_H13_DVBC_IsQammodeAutoDet(&isAutoQammode))
	{

		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_IsQammodeAutoDet() !!!\n", __F__, __L__);

		return RET_ERROR;

	}
	if(TRUE == isAutoQammode)
	{

		if (RET_OK != DEMOD_H13_DVBC_Get_QamModeDetectStatus(&qamModeDet))
		{
			DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_QamModeDetectStatus() !!!\n", __F__, __L__);

			return RET_ERROR;
		}

		switch(qamModeDet.status)
		{
			case LX_DEMOD_IDLE :
			{
				DEMOD_PRINT("[DEMOD H13] QAM mode detect is idle status!!!\n");

				break;
			}

			case LX_DEMOD_DETECTED :
			{

				DEMOD_PRINT("[DEMOD H13] QAM mode is detected!!!\n");

				DEMOD_PRINT("[DEMOD H13] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :

		                                                                 	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :

																		 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :

																		  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );

			break;

			}

			case LX_DEMOD_DETECTION_PROCESSING :
			{

				DEMOD_PRINT("[DEMOD H13] QAM mode detection is processing !!!\n");

				break;
			}

			case LX_DEMOD_DETECTION_FAIL :
			{

				DEMOD_PRINT("[DEMOD H13] QAM mode detection is failed !!!\n");

				break;

			}

			case LX_DEMOD_DETECTION_MANUAL :
			{

				DEMOD_PRINT("[DEMOD H13] Manual QAM mode detection mode !!!\n");

				break;

			}

			default :
			{
				DEMOD_PRINT("[DEMOD H13] INVALID QAM mode detection  !!!\n");

				break;
			}

		}

	}


 	//spectrum inversion status
	if (RET_OK !=  DEMOD_H13_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if(LX_DEMOD_INVERSION == spectrumInv)
	{
		DEMOD_PRINT("[DEMOD H13] Spectrum Inversion !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H13] Spectrum Normal !!!\n");
	}

	//Frame lock status
	if (RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_FEC_LOCK, &fecLockState) )
	{
		DEMOD_PRINT("[LX_DEMOD_DTV] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
		return RET_ERROR;

	}

	if(fecLockState != LX_DEMOD_LOCKED)
	{
		DEMOD_PRINT("[DEMOD H13] DVB FEC IS NOT LOCKED !!!\n");
	}
	else
	{
		DEMOD_PRINT("[DEMOD H13] DVB FEC LOCK OK !!!\n");
	}

 	//MSE

	if (RET_OK != DEMOD_H13_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H13] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);


 	//BER after Viterbi
	if (RET_OK != DEMOD_H13_Get_VABER(&vBer))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_SNR() !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H13] vBER = %d  !!!\n",vBer);

	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_H13_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H13] TP Error Count= %d !!!\n",errorCount);


#ifndef DEMOD_PRINT

	printk("[DEMOD H13] IF AGC Gain = 0x%x !!!\n",ifAGCGain);
	printk("[DEMOD H13] DVB CARRIER is %s  !!!\n",(carrLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked" );
	printk("[DEMOD H13] Carrier offset Frequency = %d [KHz] !!!\n",carrierLock.carrOffset);
	printk("[DEMOD H13] Sampling frequency is %s   !!!\n",(samplingLock == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H13] QAM mode is %s   !!!\n",(qamModeDet.status == LX_DEMOD_DETECTED) ? "detected" : "NOT detected");

	if(qamModeDet.status == LX_DEMOD_DETECTED)
	{
		printk("[DEMOD H13] QAM mode is %s !!!\n",(qamModeDet.qamMode == LX_DEMOD_16QAM) ? "16QAM" :
										            (qamModeDet.qamMode == LX_DEMOD_32QAM) ? "32QAM" :
	                                               	(qamModeDet.qamMode == LX_DEMOD_64QAM) ? "64QAM" :
												 	(qamModeDet.qamMode == LX_DEMOD_128QAM) ? "128QAM" :
												  	(qamModeDet.qamMode == LX_DEMOD_256QAM) ? "256QAM" : "UNKOWN" );
	}
	printk("[DEMOD H13 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																		(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	printk("[DEMOD H13] FEC lock is %s !!!\n",(fecLockState == LX_DEMOD_LOCKED) ? "locked" : "NOT locked");
	printk("[DEMOD H13] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);
	printk("[DEMOD H13] vBER = %d  !!!\n",vBer);
	printk("[DEMOD H13] TP Error Count= %d !!!\n",errorCount);

#endif

	return RET_OK;

}




#if 0
#endif

/**
 * DEMOD_H13_DVBT_Set_DefaultRegisterValue
 * Set defualt register value on DVB-T.
 * This function is applied only to A0 version, not to A1 version.
 *
 * @param none
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_DefaultRegisterValue(LX_DEMOD_BWMODE_T band)
{
	UINT8 adcsel1v;
	UINT8 adcclkpllsel;

	UINT8 fstoisicnf_3_0;
	UINT8 fstoisithr2_3_0;

	UINT8 stoadjwaitcnt_2_0;
	UINT8 fstomode_1_0;

	UINT8 	txpccth_di_7_0;
	UINT8 	txpccmax_di_7_0;
	UINT8 crtafcwaittime_di_7_0;

	UINT8 cirpwrctrl_d_3_0;
	UINT8 cirnrctrl_d_2_0;
	UINT8 cstostartcnt_1_0;

	UINT8 ctfnrlen_2_0;
	UINT8 crifolockfcc_di_7_0;

	UINT8 crgearshen;
	UINT8 stocirbnd_7_0;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	adcsel1v = 1;
	adcclkpllsel = 0;

	Common_RdFL(common_ctrl4);   //regAddr = 0x0004[7], COMMON_CTRL[4]
	Common_Wr02(common_ctrl4, adcsel1v, adcsel1v, adcclkpllsel, adcclkpllsel);
	Common_WrFL(common_ctrl4);
	//ADCSEL1V: Selection of ADC input range ( 0 : 2.0Vp-p, 1 : 1.0Vp-p)

	//ADCCLKPLLSEL : Selection of ADC sampling clock source.  This is not validated at DTV SoC
	// 0 : use external oscillator input
	// 1 : use PLL output


#if 1
	fstoisicnf_3_0 = 0x5;
	EQ_DI_RdFL(eq_di_ctrl30); //regAddr = 0x221e
	EQ_DI_Wr01(eq_di_ctrl30,fstoisicnf_3_0, fstoisicnf_3_0);
	EQ_DI_WrFL(eq_di_ctrl30);
	//FSTOISICNF[3:0] : Confidence counter of stage 2 for ISI FSTO
	// 0000 ~ 0111 : (64 + 64 x FSTOISICNF) x (CIRLOOPDLY + 1) symbols
	// 1000 ~ 1111 : (640 x (FSTOISICNF - 7)) x (CIRLOOPDLY + 1) symbols
	// 4k counter = 2k counter / 2, 8k counter = 2k counter / 4
	// Change to "0101" by S/W

	fstoisithr2_3_0 = 0x2;
	EQ_DI_RdFL(eq_di_ctrl32); //regAddr = 0x2220
	EQ_DI_Wr01(eq_di_ctrl32,fstoisithr2_3_0, fstoisithr2_3_0);
	EQ_DI_WrFL(eq_di_ctrl32);
	// FSTOISITHR2[3:0] : Detection threshold of stage 2 for ISI FSTO
	// Threshold = 32768 x FSTOISITHR2
	// Change to "0010" by S/W
#endif
	txpccth_di_7_0 = 0x2;
	Common_RdFL(common_ctrl90);	 //regAddr = 0x005a, COMMON_CTRL[90]
	Common_Wr01(common_ctrl90, txpccth_di_7_0, txpccth_di_7_0);
	Common_WrFL(common_ctrl90);
   // TXPCCTH_DI[7:0] : TPS/TMCC confidence counter value set

	txpccmax_di_7_0 = 0x5;
	Common_RdFL(common_ctrl91);	 //regAddr = 0x005b, COMMON_CTRL[91]
	Common_Wr01(common_ctrl91, txpccmax_di_7_0, txpccmax_di_7_0);
	Common_WrFL(common_ctrl91);
	// TXPCCMAX_DI[7:0] : TPS/TMCC maximum value of confidence counter set

	crtafcwaittime_di_7_0 = 0x18;
	Sync_RdFL(sync_ctrl78);  //regAddr = 0x104e, SYNC_CTRL[78]
	Sync_Wr01(sync_ctrl78,crtafcwaittime_di_7_0,crtafcwaittime_di_7_0);
	Sync_WrFL(sync_ctrl78);
	//CRTAFCWAITTIME_DI[7:0]: Number of waiting symbols for TAFC convergence after ifolock.

	crifolockfcc_di_7_0 = 0x8;
	Sync_RdFL(sync_ctrl89);  //regAddr = 0x1059, SYNC_CTRL[89]
	Sync_Wr01(sync_ctrl89,crifolockfcc_di_7_0,crifolockfcc_di_7_0);
	Sync_WrFL(sync_ctrl89);
	//CRIFOLOCKFCC_DI[7:0]: IFO false lock confidence count reference value set.

	if(gPowerOnInitDVB == 1)
	{
		gPowerOnInitDVB = 2;

		crgearshen = 0x0;
		Common_RdFL(common_ctrl68);	 //regAddr = 0x0044, COMMON_CTRL[68]
		Common_Wr01(common_ctrl68, crgearshen, crgearshen);
		Common_WrFL(common_ctrl68);
		// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
		// 0 : disable
		// 1 : enable

		gPowerOnInitStartTimeDVB = jiffies_to_msecs(jiffies);
	}
	else if(gPowerOnInitDVB == 0)
	{
		crgearshen = 0x1;
		Common_RdFL(common_ctrl68);  //regAddr = 0x0044, COMMON_CTRL[68]
		Common_Wr01(common_ctrl68, crgearshen, crgearshen);
		Common_WrFL(common_ctrl68);
		// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
		// 0 : disable
		// 1 : enable
	}
	else
	{
		gPowerOnInitDVB = 0;

	}


#if 0
	samgearshen = 0x0;
	Common_RdFL(common_ctrl61);	 //regAddr = 0x003d, COMMON_CTRL[61]
	Common_Wr01(common_ctrl61, samgearshen, samgearshen);
	Common_WrFL(common_ctrl61);
	// SAMGEARSHEN : Gear shift function control for sampling frequency error calculation
	// 0 : disable
	// 1 : enable

#endif

	if(LX_DEMOD_BW_6MHZ == band)
	{
		stoadjwaitcnt_2_0 = 0x0;
		EQ_DI_RdFL(eq_di_ctrl3); //regAddr = 0x2203
		EQ_DI_Wr01(eq_di_ctrl3,stoadjwaitcnt_2_0, stoadjwaitcnt_2_0);
		EQ_DI_WrFL(eq_di_ctrl3);
		// STOADJWAITCNT[2:0] : Selection of period for STO adjustment. Observing 'tpcorrect' signal during this period.
		// 000 : 256 (symbols), 001 : 448, 010 : 480, 011 : 512, 100 : 544, 101 : 576, 110 : 608, 111 : 1023

		fstomode_1_0 = 0x1;
		EQ_DI_RdFL(eq_di_ctrl25); //regAddr = 0x2219
		EQ_DI_Wr01(eq_di_ctrl25,fstomode_1_0, fstomode_1_0);
		EQ_DI_WrFL(eq_di_ctrl25);
		// FSTOMODE[1:0] : FSTO adjustment algorithm selection
		// 00 : manual adjustment
		// 01 : algorithm based on ISI with one time check
		// 10 : algorithm based on ISI with double check
		// 11 : algorithm based on correct


		cirnrctrl_d_2_0 = 0x3;
		EQ_DI_RdFL(eq_di_ctrl37); //regAddr = 0x2225
		EQ_DI_Wr01(eq_di_ctrl37,cirnrctrl_d_2_0, cirnrctrl_d_2_0);
		EQ_DI_WrFL(eq_di_ctrl37);
		// CIRNRCTRL_D[2:0] : CIR noise reduction power level control for DVB-T (1x = 6bits round)
		// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x,  Change to "010" by S/W

		cirpwrctrl_d_3_0 = 0x6;
		EQ_DI_RdFL(eq_di_ctrl35); //regAddr = 0x2223, EQ_DI_CTRL[35]
		EQ_DI_Wr01(eq_di_ctrl35,cirpwrctrl_d_3_0, cirpwrctrl_d_3_0);
		EQ_DI_WrFL(eq_di_ctrl35);
		// CIRPWRCTRL_D[3:0] :	CIR Power level control after power calculation for DVB-T
		// (1x = 3 bits round)
		// 0000 : 1x/64, 0001 : 1x/32, 0010 : 1x/16, 0011 : 1x/8, 0100 : 1x/4, 0101 : 1x/2
		// 0110 : 1x,	0111 : 2x, 1000 : 4x, 1001 : 8x,	101x : 16x, 11xx : 16x

		cstostartcnt_1_0 = 0x3;
		EQ_DI_RdFL(eq_di_ctrl15); //regAddr = 0x220f, EQ_DI_CTRL[15]
		EQ_DI_Wr01(eq_di_ctrl15,cstostartcnt_1_0, cstostartcnt_1_0);
		EQ_DI_WrFL(eq_di_ctrl15);
		// CSTOSTARTCNT[1:0] : CSTO start threshold selection
		// 00 : 1 symbol
		// 01 : 3 symbols
		// 10 : 7 symbols
		// 11 : 15 symbols

		ctfnrlen_2_0 = 0x2;
		EQ_DI_RdFL(eq_di_ctrl46); //regAddr = 0x222e, EQ_DI_CTRL[46]
		EQ_DI_Wr01(eq_di_ctrl46,ctfnrlen_2_0, ctfnrlen_2_0);
		EQ_DI_WrFL(eq_di_ctrl46);
		// CTFNRLEN[2:0]: CTFNR length(forgetting factor) selection (a value)
		// 000 : 1(bypass)
		// 001 : 1/2
		// 010 : 1/4
		// 011 : 1/8
		// 1xx : 1/16

		stocirbnd_7_0 = 0x20;
		EQ_DI_RdFL(eq_di_ctrl5); //regAddr = 0x2205, EQ_DI_CTRL[5]
		EQ_DI_Wr01(eq_di_ctrl5,stocirbnd_7_0, stocirbnd_7_0);
		EQ_DI_WrFL(eq_di_ctrl5);
		// STOCIRBND[7:0] : CIR Post bound region manully set
		// defaults are Tu/4 + Tu/24(Post) and Tu/24(Pre)


	}
	else if(LX_DEMOD_BW_7MHZ == band)
	{
		cirnrctrl_d_2_0 = 0x2;
		EQ_DI_RdFL(eq_di_ctrl37); //regAddr = 0x2225
		EQ_DI_Wr01(eq_di_ctrl37,cirnrctrl_d_2_0, cirnrctrl_d_2_0);
		EQ_DI_WrFL(eq_di_ctrl37);
		// CIRNRCTRL_D[2:0] : CIR noise reduction power level control for DVB-T (1x = 6bits round)
		// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x,  Change to "010" by S/W

	}
	else
	{
		/* do nothing so far */
	}

	_gGBB_DVB_UnlockCount_full_packeterr 	= 0;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_Config_auto
 * Control automatic setting of DVB-T config.
 *
 * @param bAutoDetect 	[IN] BOOLEAN - automatic setting (TRUE) / manual setting(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_Config_auto (BOOLEAN bAutoDetect)

{

	UINT8	txpauto_di;
	UINT8	mdauto_di;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	mdauto_di = (bAutoDetect == TRUE)?(1):(0);	//MDAUTO_DI
	Common_RdFL(common_ctrl83);	 //regAddr = 0x0053, COMMON_CTRL[83]
	Common_Wr01(common_ctrl83, mdauto_di, mdauto_di); //MDAUTO_DI : FFT/Guard Interval mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(common_ctrl83);

	txpauto_di = (bAutoDetect == TRUE)?(1):(0);  //QAMMODEAUTO
	Common_RdFL(common_ctrl87);	 //regAddr = 0x0057, COMMON_CTRL[87]
	Common_Wr01(common_ctrl87, txpauto_di, txpauto_di); //TXPAUTO_DI : Transmission mode auto/manual detection control(0->manual, 1->auto)
	Common_WrFL(common_ctrl87);

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_PartialConfig
 * Set hierachical signal path and bandwidth.
 * This function is used when LX_DEMOD_H13_DVBT_Set_Config_auto is enabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_PartialConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{

	UINT8 	hierhpsel_d;

	BOOLEAN bSetHp;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	bSetHp = pDvbtConfigParam->bSetHp;
	if(bSetHp)
	{
		hierhpsel_d = 0x1;
		DEMOD_PRINT("[DEMOD H13] High Priority signal is selected  !!!\n");
	}
	else
	{
		hierhpsel_d = 0x0;
		DEMOD_PRINT("[DEMOD H13] Low Priority signal is selected  !!!\n");
	}

	Common_RdFL(common_ctrl117);   //regAddr = 0x0075, COMMON_CTRL[117]
	Common_Wr01(common_ctrl117, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority
	Common_WrFL(common_ctrl117);
	//HIERHPSEL_D :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_FullConfig
 * Set all config parameters on DVB-T.
 * This function is used when LX_DEMOD_H13_DVBT_Set_Config_auto is disabled.
 *
 * @param *pDvbtConfigParam 	[IN] LX_DEMOD_DVBT_CONFIG_T - DVB-T config. structure
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_FullConfig (LX_DEMOD_DVBT_CONFIG_T *pDvbtConfigParam)
{

	UINT8 	hierhpsel_d;
	UINT8 	mdfftmode_di_1_0;
	UINT8 	mdgimode_di_1_0;
	UINT8 	tpshiermode_d_2_0;
	UINT8 	tpshpcoderate_d_2_0;
	UINT8 	tpslpcoderate_d_2_0;
	UINT8 	tpsqammode_d_1_0;

	UINT8 	txpccth_di_7_0;
	UINT8 	txpccmax_di_7_0;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	//Channel decoding signal selection
	if(pDvbtConfigParam->bSetHp)
	{
		hierhpsel_d = 0x1;
		DEMOD_PRINT("[DEMOD H13] High Priority signal is selected  !!!\n");
	}
	else
	{
		hierhpsel_d = 0x0;
		DEMOD_PRINT("[DEMOD H13] Low Priority signal is selected	!!!\n");
	}

	Common_RdFL(common_ctrl117);   //regAddr = 0x0075, COMMON_CTRL[117]
	Common_Wr01(common_ctrl117, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority
	Common_WrFL(common_ctrl117);
	//HIERHPSEL_D :Selection of signal for channel decoding
	//High/Low priority signal path slection at hierarchical transmission mode, This is only for DVB-T
	// 0 : low priority
	// 1 : high priority



	//FFT mode manual set
	switch(pDvbtConfigParam->FFTMode)
	{
		case LX_DEMOD_DVBT_ISDBT_FFT_2K :
			{
				mdfftmode_di_1_0 = 0x0;
				txpccth_di_7_0 	 = 0x1;
				txpccmax_di_7_0	 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set FFT Mode = %s !!!\n","2K ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_FFT_8K :
			{
				mdfftmode_di_1_0 = 0x1;
				txpccth_di_7_0 = 0x2;
				txpccmax_di_7_0	 = 0x5;
				DEMOD_PRINT("[DEMOD H13] Set FFT Mode = %s !!!\n","8K");
			}
			break;

		default :
			{
				mdfftmode_di_1_0 = 0x1;
				txpccth_di_7_0 	 = 0x2;
				txpccmax_di_7_0	 = 0x5;
				DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d)UNKNOWN FFT mode !!!\n",__F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(common_ctrl90);	 //regAddr = 0x005a, COMMON_CTRL[90]
	Common_Wr01(common_ctrl90, txpccth_di_7_0, txpccth_di_7_0);
	Common_WrFL(common_ctrl90);
	// TXPCCTH_DI[7:0] : TPS/TMCC confidence counter value set

	Common_RdFL(common_ctrl91);  //regAddr = 0x005b, COMMON_CTRL[91]
	Common_Wr01(common_ctrl91, txpccmax_di_7_0, txpccmax_di_7_0);
	Common_WrFL(common_ctrl91);
	// TXPCCMAX_DI[7:0] : TPS/TMCC maximum value of confidence counter set

	//GI(Guard Interval) mode manual set
	switch(pDvbtConfigParam->gIMode)
	{
		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_4 :
			{
				mdgimode_di_1_0 = 0x3;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_4 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_8 :
			{
				mdgimode_di_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_8 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_16 :
			{
				mdgimode_di_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_16 ");
			}
			break;

		case LX_DEMOD_DVBT_ISDBT_GIMODE_1_32 :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] Set Guard Interval = %s !!!\n","1_32 ");
			}
			break;
		default :
			{
				mdgimode_di_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN Guard Interval  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(common_ctrl83);	 //regAddr = 0x0053, COMMON_CTRL[83]
	Common_Wr02(common_ctrl83, mdfftmode_di_1_0, mdfftmode_di_1_0, mdgimode_di_1_0, mdgimode_di_1_0);
	Common_WrFL(common_ctrl83);

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
				DEMOD_PRINT("[DEMOD H13] Set Hierachical Mode = %s !!!\n","NONE");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_1 :
			{
				tpshiermode_d_2_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set Hierachical Mode = %s !!!\n","alpha_1");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_2 :
			{
				tpshiermode_d_2_0 = 0x2;
				DEMOD_PRINT("[DEMOD H13] Set Hierachical Mode = %s !!!\n","alpha_2");
			}
			break;

		case LX_DEMOD_DVB_TPS_HIERACHY_4 :
			{
				tpshiermode_d_2_0 = 0x3;
				DEMOD_PRINT("[DEMOD H13] Set Hierachical Mode = %s !!!\n","alpha_4");
			}
			break;
		default :
			{
				tpshiermode_d_2_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN Hierachical Mode  !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	//QAM mode manual set
	switch(pDvbtConfigParam->qamMode)
	{
		case LX_DEMOD_QPSK :
			{
				tpsqammode_d_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] Set QAM mode = %s !!!\n","QPSK");
			}
			break;

		case LX_DEMOD_16QAM :
			{
				tpsqammode_d_1_0 = 0x1;
				DEMOD_PRINT("[DEMOD H13] Set QAM mode = %s !!!\n","16QAM");
			}
			break;

		case LX_DEMOD_64QAM :
			{
				tpsqammode_d_1_0 = 0x2;
				DEMOD_PRINT("[DEMOD H13] Set QAM mode = %s !!!\n","64QAM");
			}
			break;

		default :
			{
				tpsqammode_d_1_0 = 0x0;
				DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN QAM mode !!!\n", __F__, __L__);
			}
			return RET_ERROR;
	}

	Common_RdFL(common_ctrl95);   //regAddr = 0x005f, COMMON_CTRL[95]
	Common_Wr02(common_ctrl95, tpshiermode_d_2_0, tpshiermode_d_2_0, tpsqammode_d_1_0, tpsqammode_d_1_0);
	Common_WrFL(common_ctrl95);

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
					DEMOD_PRINT("[DEMOD H13] Set HP code rate = %s !!!\n","1_2");
				}
				break;

			case LX_DEMOD_CODE_2_3 :
				{
					tpshpcoderate_d_2_0 = 0x1;
					DEMOD_PRINT("[DEMOD H13] Set HP code rate = %s !!!\n","2_3");
				}
				break;

			case LX_DEMOD_CODE_3_4 :
				{
					tpshpcoderate_d_2_0 = 0x2;
					DEMOD_PRINT("[DEMOD H13] Set HP code rate = %s !!!\n","3_4");
				}
				break;

			case LX_DEMOD_CODE_5_6 :
				{
					tpshpcoderate_d_2_0 = 0x3;
					DEMOD_PRINT("[DEMOD H13] Set HP code rate = %s !!!\n","5_6");
				}
				break;

			case LX_DEMOD_CODE_7_8 :
				{
					tpshpcoderate_d_2_0 = 0x4;
					DEMOD_PRINT("[DEMOD H13] Set HP code rate = %s !!!\n","7_8");
				}
				break;
			default :
				{
					tpshpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN HP code rate !!!\n", __F__, __L__);
				}
				return RET_ERROR;
		}

		Common_RdFL(common_ctrl96);   //regAddr = 0x0060, COMMON_CTRL[96]
		Common_Wr01(common_ctrl96, tpshpcoderate_d_2_0, tpshpcoderate_d_2_0);
		Common_WrFL(common_ctrl96);
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
					DEMOD_PRINT("[DEMOD H13] Set LP code rate = %s !!!\n","1_2");
				}
				break;

			case LX_DEMOD_CODE_2_3 :
				{
					tpslpcoderate_d_2_0 = 0x1;
					DEMOD_PRINT("[DEMOD H13] Set LP code rate = %s !!!\n","2_3");
				}
				break;

			case LX_DEMOD_CODE_3_4 :
				{
					tpslpcoderate_d_2_0 = 0x2;
					DEMOD_PRINT("[DEMOD H13] Set LP code rate = %s !!!\n","3_4");
				}
				break;

			case LX_DEMOD_CODE_5_6 :
				{
					tpslpcoderate_d_2_0 = 0x3;
					DEMOD_PRINT("[DEMOD H13] Set LP code rate = %s !!!\n","5_6");
				}
				break;

			case LX_DEMOD_CODE_7_8 :
				{
					tpslpcoderate_d_2_0 = 0x4;
					DEMOD_PRINT("[DEMOD H13] Set LP code rate = %s !!!\n","7_8");
				}
				break;
			default :
				{
					tpslpcoderate_d_2_0 = 0x0;
					DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN LP code rate !!!\n", __F__, __L__);
				}
				return RET_ERROR;
		}

		Common_RdFL(common_ctrl96);   //regAddr = 0x0060, COMMON_CTRL[96]
		Common_Wr01(common_ctrl96, tpslpcoderate_d_2_0, tpslpcoderate_d_2_0);
		Common_WrFL(common_ctrl96);
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
 * DEMOD_H13_DVBT_Set_NeverLockWaitTime
 * Set neverlock waiting period.
 *
 * @param waitMs 	[IN] UINT32 - unit: ms
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_NeverLockWaitTime(UINT32 waitMs)
{
	UINT8 scsyslockchktime_7_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	scsyslockchktime_7_0 = (UINT8)(waitMs / 10);

	Common_Wr01(common_ctrl11,scsyslockchktime_7_0,scsyslockchktime_7_0);
	Common_WrFL(common_ctrl11);  //regAddr = 0x000b,COMMON_CTRL[11]
	//SCSYCLOCKCHKTIME[7:0] : Set of wait time for system re-initialization check start in msec resolution

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_TafcWaitSymbol
 * Set TAFC wait symbol number.
 *
 * @param bDelayed 	[IN] BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_TafcWaitSymbol(BOOLEAN bDelayed)
{
	UINT8 crtafcwaittime_di_7_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	if(TRUE == bDelayed)
		crtafcwaittime_di_7_0 = 0xff;
	else
		crtafcwaittime_di_7_0 = 0x18;


	Sync_Wr01(sync_ctrl78,crtafcwaittime_di_7_0,crtafcwaittime_di_7_0);
	Sync_WrFL(sync_ctrl78);  //regAddr = 0x104e, SYNC_CTRL[78]
	//CRTAFCWAITTIME_DI[7:0]: Number of waiting symbols for TAFC convergence after ifolock.

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_CTFNR_Control
 * Set TAFC wait symbol number.
 *
 * @param ctfnrCtrl 	[IN] LX_DEMOD_CTFNR_CTRL_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_CTFNR_Control(LX_DEMOD_CTFNR_CTRL_T ctfnrCtrl)
{
	UINT8 ctfnrlen_2_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	switch(ctfnrCtrl)
	{
		case LX_DEMOD_CTFNR_BYPASS :
			{
				ctfnrlen_2_0 = 0;

			}
			break;

		case LX_DEMOD_CTFNR_1_2 :
			{
				ctfnrlen_2_0 = 1;
			}
			break;

		case LX_DEMOD_CTFNR_1_4 :
			{
				ctfnrlen_2_0 = 2;
			}
			break;

		case LX_DEMOD_CTFNR_1_8 :
			{
				ctfnrlen_2_0 = 3;
			}
			break;

		case LX_DEMOD_CTFNR_1_16 :
			{
				ctfnrlen_2_0 = 4;
			}
			break;

		default :
			{
				ctfnrlen_2_0 = 4;

			}
			break;


	}


	EQ_DI_RdFL(eq_di_ctrl46); //regAddr = 0x222e, EQ_DI_CTRL[46]
	EQ_DI_Wr01(eq_di_ctrl46,ctfnrlen_2_0, ctfnrlen_2_0);
	EQ_DI_WrFL(eq_di_ctrl46);
	// CTFNRLEN[2:0]: CTFNR length(forgetting factor) selection (a value)
	// 000 : 1(bypass)
	// 001 : 1/2
	// 010 : 1/4
	// 011 : 1/8
	// 1xx : 1/16


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_CirPwrControl
 * Set Cir Power level and NR level
 *
 * @param bPwrHigh 	[IN] BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_CTRL_T cirPwrCtrl)
{
	UINT8 cirnrctrl_d_2_0;
	UINT8 cirpwrctrl_d_3_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	if(cirPwrCtrl == LX_DEMOD_CIR_POWER_HIGH)
	{
		cirnrctrl_d_2_0 = 0x3;
		cirpwrctrl_d_3_0 = 0x6;

	}
	else if(cirPwrCtrl == LX_DEMOD_CIR_POWER_MID)
	{
		cirnrctrl_d_2_0 = 0x2;
		cirpwrctrl_d_3_0 = 0x6;
	}
	else
	{
		cirnrctrl_d_2_0 = 0x1;
		cirpwrctrl_d_3_0 = 0x5;
	}

	EQ_DI_RdFL(eq_di_ctrl37); //regAddr = 0x2225
	EQ_DI_Wr01(eq_di_ctrl37,cirnrctrl_d_2_0, cirnrctrl_d_2_0);
	EQ_DI_WrFL(eq_di_ctrl37);
	// CIRNRCTRL_D[2:0] : CIR noise reduction power level control for DVB-T (1x = 6bits round)
	// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x,  Change to "010" by S/W

	EQ_DI_RdFL(eq_di_ctrl35); //regAddr = 0x2223, EQ_DI_CTRL[35]
	EQ_DI_Wr01(eq_di_ctrl35,cirpwrctrl_d_3_0, cirpwrctrl_d_3_0);
	EQ_DI_WrFL(eq_di_ctrl35);
	// CIRPWRCTRL_D[3:0] :	CIR Power level control after power calculation for DVB-T
	// (1x = 3 bits round)
	// 0000 : 1x/64, 0001 : 1x/32, 0010 : 1x/16, 0011 : 1x/8, 0100 : 1x/4, 0101 : 1x/2
	// 0110 : 1x,	0111 : 2x, 1000 : 4x, 1001 : 8x,	101x : 16x, 11xx : 16x

	DEMOD_PRINT("[DEMOD H13 DTV](%s)(%s) \n",__F__, (cirPwrCtrl == LX_DEMOD_CIR_POWER_HIGH) ? "HIGH" :
					(cirPwrCtrl == LX_DEMOD_CIR_POWER_MID) ? "MID" : "LOW");

	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_CrGearshEn
 * enable/disable Carrier recovery bandwidth gear shift
 *
 * @param bEnable 	[IN] BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_CrGearshEn(BOOLEAN bEnable)
{
	UINT8 crgearshen;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	crgearshen = (bEnable == TRUE) ? 0x1 : 0x0;
	Common_RdFL(common_ctrl68);  //regAddr = 0x0044, COMMON_CTRL[68]
	Common_Wr01(common_ctrl68, crgearshen, crgearshen);
	Common_WrFL(common_ctrl68);
	// CRGEARSHEN : Gear shift function control for carrier frequency error calculation
	// 0 : disable
	// 1 : enable
	DEMOD_PRINT("[DEMOD H13 DTV](%s)(%s) \n",__F__, (bEnable == TRUE) ? "ON" : "OFF");

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Set_CirNormalize
 * Set Cir Power level and NR level
 *
 * @param none 	[IN] void
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Set_CirNormalize(void)
{

	UINT16 delaySpread;
	UINT8 cirCount;
	UINT8 validCirCount = 0;
	UINT8 cirTotalNum = 16;

	UINT8 maxCir = 0x0;
	UINT8 minCir = 0xff;

	UINT8 cirTempValue;
	UINT8 cirStartReg = 0xd8;

	BOOLEAN IsPreGhostExist;

	LX_DEMOD_CIR_POWER_CTRL_T cirPwrCtrl = LX_DEMOD_CIR_POWER_UNKNOWN;


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	if(RET_OK != DEMOD_H13_DVBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}

	if(delaySpread <= 430)
	{
		if (RET_OK != DEMOD_H13_DVBT_Set_CTFNR_Control(LX_DEMOD_CTFNR_1_16))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CTFNR_Control !!!\n", __F__, __L__);
			return RET_ERROR;
		}

		return RET_OK;
	}

	for(cirCount = 0; cirCount < cirTotalNum; cirCount++  )
	{
		if (RET_OK != DEMOD_I2C_Read( cirStartReg, 1, &cirTempValue))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_I2C_Read !!!\n", __F__, __L__);
			return RET_ERROR;
		}
		if(cirTempValue != 0x0)
		{
			if(cirTempValue > maxCir)
			{
				maxCir = cirTempValue;
			}
			if(cirTempValue < minCir)
			{
				minCir = cirTempValue;
			}
			validCirCount++;
		}
		cirStartReg++;

	}
	DEMOD_PRINT("[DEMOD H13 DTV](%s) validCirCount = %d \n",__F__,validCirCount);
	DEMOD_PRINT("[DEMOD H13 DTV](%s) maxCir = %d, minCir = %d \n",__F__, maxCir, minCir);

	if(validCirCount != 2 )
	{
		if(RET_OK != DEMOD_H13_DVBT_Get_DelaySpreadStatus(&delaySpread))
		{
			DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		DEMOD_PRINT("[DEMOD H13 DTV](%s) delay spread = %d \n",__F__,delaySpread);

		if(/*(delaySpread <= 600) ||*/ (delaySpread > 2048))
		{
			if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
			{
				DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
				return RET_ERROR;
			}

		}
		else
		{
			if(RET_OK != DEMOD_H13_DVBT_Get_PreGhostDetection(&IsPreGhostExist))
			{
				DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
				DEMOD_RETURN_FAIL(__F__);
			}
			if(IsPreGhostExist == TRUE)
			{
				if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
			else
			{
				return RET_OK;
			}

		}
	}
	else
	{
		if (RET_OK != DEMOD_H13_DVBT_Get_CirPwrControl(&cirPwrCtrl))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
			return RET_ERROR;
		}

		if(cirPwrCtrl == LX_DEMOD_CIR_POWER_HIGH)
		{
		    //max, min
		    if((maxCir == 0xff) && (minCir < 0x48))
		    {
				if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_LOW))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
					return RET_ERROR;
				}
		    }
			else
			{
				if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
				{
					DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
		}
	}
	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_CirPwrControl
 * Set Cir Power level and NR level
 *
 * @param *pCirPwrCtrl 	[OUT] LX_DEMOD_CIR_POWER_CTRL_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_CirPwrControl(LX_DEMOD_CIR_POWER_CTRL_T *pCirPwrCtrl)
{
	UINT8 cirnrctrl_d_2_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	EQ_DI_RdFL(eq_di_ctrl37); //regAddr = 0x2225
	EQ_DI_Rd01(eq_di_ctrl37,cirnrctrl_d_2_0, cirnrctrl_d_2_0);
	// CIRNRCTRL_D[2:0] : CIR noise reduction power level control for DVB-T (1x = 6bits round)
	// 000 : 1x/2, 001 : 1x, 010 : 2x, 011 : 4x, 100 : 8x, 101 : 16x, 110 : 32x, 111 : 64x,  Change to "010" by S/W


	if(cirnrctrl_d_2_0 == 0x3)
	{
		*pCirPwrCtrl = LX_DEMOD_CIR_POWER_HIGH;

	}
	else if(cirnrctrl_d_2_0 == 0x2)
	{
		*pCirPwrCtrl = LX_DEMOD_CIR_POWER_MID;
	}
	else if(cirnrctrl_d_2_0 == 0x1)
	{
		*pCirPwrCtrl = LX_DEMOD_CIR_POWER_LOW;
	}
	else
	{
		*pCirPwrCtrl = LX_DEMOD_CIR_POWER_UNKNOWN;

	}

	DEMOD_PRINT("[DEMOD H13 DTV](%s)(%s) \n",__F__, (*pCirPwrCtrl == LX_DEMOD_CIR_POWER_HIGH) ? "HIGH" :
					(*pCirPwrCtrl == LX_DEMOD_CIR_POWER_MID) ? "MID" :
					(*pCirPwrCtrl == LX_DEMOD_CIR_POWER_LOW) ? "LOW" :"UNKNOWN");

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_PreGhostDetection
 * Set Cir Power level and NR level
 *
 * @param *pIsExist 	[IN] BOOLEAN
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_PreGhostDetection(BOOLEAN *pIsExist)
{
	UINT8 stocurstatus_1_0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	EQ_DI_RdFL(eq_di_mon2); //regAddr = 0x2282,
	EQ_DI_Rd01(eq_di_mon2,stocurstatus_1_0, stocurstatus_1_0);

	if(stocurstatus_1_0 == 0x2)
	{
		*pIsExist = TRUE;
	}
	else
	{
		*pIsExist = FALSE;

	}
	DEMOD_PRINT("[DEMOD H13 DTV](%s) stocurstatus_1_0 = %d, pIsExist = %s \n",__F__, stocurstatus_1_0, (*pIsExist == TRUE) ? "TRUE" : "FALSE");

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_DelaySpreadStatus
 * Get delay spread state on DVB-T.
 *
 * @param *pDelaySpread 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_DelaySpreadStatus(UINT16 *pDelaySpread)
{

	UINT8	dlyspread_7_0;
	UINT8 	dlyspread_11_8;

	UINT16 delaySpread = 0;

	*pDelaySpread = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon70);  //regAddr = 0x00c6, COMMON_MON[70]
	Common_Rd01(common_mon70,dlyspread_11_8,dlyspread_11_8); //DLYSPREAD[11:8] : Delay spread indication of channel for DVB-T/ISDB-T

	delaySpread = (dlyspread_11_8  << 8);

	Common_RdFL(common_mon71);  //regAddr = 0x00c7, COMMON_MON[71]
	Common_Rd01(common_mon71,dlyspread_7_0,dlyspread_7_0); //DLYSPREAD[7:0]

	delaySpread += (dlyspread_7_0 << 0);

	*pDelaySpread = delaySpread;

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_Hierach_HPSel
 * Get the information if the hierachical HP is selected.
 *
 * @param *pBSetHp 	[OUT] BOOLEAN - HP (TRUE) / LP(FALSE).
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_Hierach_HPSel( BOOLEAN *pBSetHp )
{

	UINT8 	hierhpsel_d;

	*pBSetHp = TRUE;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_ctrl117);   //regAddr = 0x0075, COMMON_CTRL[117]
	Common_Rd01(common_ctrl117, hierhpsel_d, hierhpsel_d); //HIERHPSEL_D : '0'->Low priority, '1'-> High priority


	if(hierhpsel_d)	*pBSetHp = TRUE;
	else			*pBSetHp = FALSE;

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_FFTMode
 * Get the information of bandwidth on DVB-T.
 *
 * @param *pTxModeLock 	[OUT] LX_DEMOD_LOCK_STATE_T - FFT mode lock state.
 * @param *pFftMode 	[OUT] LX_DEMOD_FFTMODE_T - detected FFT mode.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_FFTMode( LX_DEMOD_LOCK_STATE_T *pTxModeLock, LX_DEMOD_FFTMODE_T *pFftMode )
{

	UINT8 	modelock_di;
	UINT8 	fftmoder_di_1_0;

	*pTxModeLock = LX_DEMOD_UNLOCKED;
	*pFftMode = LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon42);	//regAddr = 0x00aa, COMMON_MON[42]
	Common_Rd01(common_mon42,modelock_di,modelock_di); //MODELOCK_DI : Transmission mode lock status indication for DVB-T/ISDB-T
															// 0-> unlock, 1-> lock
	if(modelock_di)
	{
		*pTxModeLock = LX_DEMOD_LOCKED;

		DEMOD_PRINT("[DEMOD H13] TX mode LOCKED !!!\n");

		Common_RdFL(common_mon42);	//regAddr = 0x00aa, COMMON_MON[42]
		Common_Rd01(common_mon42,fftmoder_di_1_0,fftmoder_di_1_0);
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
		DEMOD_PRINT("[DEMOD H13] TX mode is NOT LOCKED !!!\n");
	}

	return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_GuradIntervalMode
 * Get the information of guard interval on DVB-T.
 *
 * @param *pGIMode 	[OUT] LX_DEMOD_GIMODE_T - 1/32, 1/16, 1/8 and 1/4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_GuradIntervalMode( LX_DEMOD_GIMODE_T *pGIMode )
{

	UINT8 	gimoder_di_1_0;

	*pGIMode = LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon42);	//regAddr = 0x00aa, COMMON_MON[42]
	Common_Rd01(common_mon42,gimoder_di_1_0,gimoder_di_1_0);
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
 * DEMOD_H13_DVBT_Get_HierachyMode
 * Get the information of hierachical mode on DVB-T.
 *
 * @param *pHierachMode[OUT] DEMOD_H13_DVB_TPS_HIERACHY_T - alpha = none, 1, 2 and 4.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_HierachyMode( LX_DEMOD_DVB_TPS_HIERACHY_T *pHierachMode )
{

	UINT8 	tpshiermoder_d_2_0;

	*pHierachMode = LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon47);  //regAddr = 0x00af, COMMON_MON[47]
	Common_Rd01(common_mon47,tpshiermoder_d_2_0,tpshiermoder_d_2_0);
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
 * DEMOD_H13_DVBT_Get_LpCoderRate
 * Get the information of LP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_LpCoderRate( LX_DEMOD_CODERATE_T *pLpCodeRate )
{

	UINT8 	tpslpcoderater_d_2_0;

	*pLpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon48);  //regAddr = 0x00b0, COMMON_MON[48]
	Common_Rd01(common_mon48,tpslpcoderater_d_2_0,tpslpcoderater_d_2_0);
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
 * DEMOD_H13_DVBT_Get_HpCoderRate
 * Get the information of HP code rate on DVB-T.
 *
 * @param *pLpCodeRate[OUT] LX_DEMOD_CODERATE_T - code rate : 1/2, 2/3, 3/4, 5/6 and 7/8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_HpCoderRate( LX_DEMOD_CODERATE_T *pHpCodeRate )
{

	UINT8 	tpshpcoderater_d_2_0;

	*pHpCodeRate = LX_DEMOD_CODE_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon48);  //regAddr = 0x00b0, COMMON_MON[48]
	Common_Rd01(common_mon48,tpshpcoderater_d_2_0,tpshpcoderater_d_2_0);
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
 * DEMOD_H13_DVBT_GetCellId
 * Get TPS cell ID on DVB-T.
 *
 * @param *pCellIDLength[OUT] UINT8 - cell ID length.
 * @param *cell_id[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_CellId(UINT8 *pCellIDLength, UINT16 *cell_id)
{

	UINT8	tpslengthr_d_5_0;
	UINT8 	tpscellidr_d_7_0;
	UINT8 	tpscellidr_d_15_8;

	UINT16 tempCellID = 0;

	*pCellIDLength = 0;
	*cell_id = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon46);  //regAddr = 0x00ae, COMMON_MON[46]
	Common_Rd01(common_mon46,tpslengthr_d_5_0,tpslengthr_d_5_0);
	//TPSLENGTHR_D[5:0] : Monitoring for TPS LENGTH

	*pCellIDLength = tpslengthr_d_5_0;

	Common_RdFL(common_mon49);  //regAddr = 0x00b1, COMMON_MON[49]
	Common_Rd01(common_mon49,tpscellidr_d_15_8,tpscellidr_d_15_8); 	//TPSCELLIDR_D[15:8] : Monitoring for CELLID (in DVB-T TPS decoder)

	tempCellID = (tpscellidr_d_15_8  << 8);

	Common_RdFL(common_mon50);  //regAddr = 0x00b2, COMMON_MON[50]
	Common_Rd01(common_mon50,tpscellidr_d_7_0,tpscellidr_d_7_0); //TPSCELLIDR_D[7:0] : Monitoring for CELLID (in DVB-T TPS decoder)
	tempCellID += (tpscellidr_d_7_0  << 0);

	*cell_id = tempCellID;

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_TpsInfo
 * Get TPS information on DVB-T
 *
 * @param *pTpsParams [OUT] LX_DEMOD_DVBT_CONFIG_T - TPS data structure.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_TpsInfo( LX_DEMOD_DVBT_CONFIG_T *pTpsParams)
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


	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	if(NULL == pTpsParams)
	{
		DEMOD_PRINT("[DEMOD H13] ERROR, DEMOD_H13_DVBT_CONFIG_T TpsParams is NULL !!!\n");
		return RET_ERROR;
	}

	// Get bSetHp

	if (RET_OK != DEMOD_H13_DVBT_Get_Hierach_HPSel( &bSetHp ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_Hierach_HPSel() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->bSetHp = bSetHp;
	DEMOD_PRINT("[DEMOD H13] Set HP selection = %s !!!\n",(bSetHp == TRUE) ? " HP Mode" : " LP Mode");


	// Get channelBW

	if (RET_OK != DEMOD_H13_Get_BandWidthMode( &channelBW ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_BandWidthMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->channelBW = channelBW;
	DEMOD_PRINT("[DEMOD H13] Bandwidth = %s !!!\n",
		(channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	if (RET_OK != DEMOD_H13_DVBT_Get_FFTMode( &txModeLock, &FFTMode ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->FFTMode = FFTMode;
	DEMOD_PRINT("[DEMOD H13] Tx mode = %s, FFTMode = %s  !!!\n",
		(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked",
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	if (RET_OK != DEMOD_H13_DVBT_Get_GuradIntervalMode( &gIMode ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->gIMode = gIMode;
	DEMOD_PRINT("[DEMOD H13] Guard Intterval = %s  !!!\n",
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );



	// Get Hierachical mode
	if (RET_OK != DEMOD_H13_DVBT_Get_HierachyMode( &hierachMode ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_HierachyMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hierachMode = hierachMode;
	DEMOD_PRINT("[DEMOD H13] Hierachical Mode = %s  !!!\n",
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	if (RET_OK != DEMOD_H13_DVBT_Get_LpCoderRate( &lpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_LpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->lpCodeRate = lpCodeRate;
	DEMOD_PRINT("[DEMOD H13] LP code rate = %s  !!!\n",
		(lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate

	if (RET_OK != DEMOD_H13_DVBT_Get_HpCoderRate( &hpCodeRate ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_HpCoderRate() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->hpCodeRate = hpCodeRate;
	DEMOD_PRINT("[DEMOD H13] HP code rate = %s  !!!\n",
		(hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	if (RET_OK != DEMOD_H13_Get_QAMMode(&qamMode ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVB_Get_QAMMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	pTpsParams->qamMode = qamMode;
	DEMOD_PRINT("[DEMOD H13] modulation  = %s  !!!\n",
		(qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_TotalInfo
 * Get all information on DVB-T for debug
 *
 * @param NONE.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_TotalInfo( void)
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

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	if (RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_AGC_LOCK, &agcLockStatus))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
		agcLockStatus = LX_DEMOD_UNKOWN_STATE;
	}
	if(LX_DEMOD_LOCKED == agcLockStatus)
	{
		if (RET_OK != DEMOD_H13_Get_IFAGC(&ifagcValue))
		{
			DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_IFAGC() !!!\n", __F__, __L__);
			ifagcValue = 0;
		}
	}

	if (RET_OK != DEMOD_H13_Get_NeverLockStatus(LX_DEMOD_DVBT, &neverLockStatus))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVB_Get_NeverLockStatus() !!!\n", __F__, __L__);
		neverLockStatus = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK !=  DEMOD_H13_Get_SpectrumStatus(&spectrumAuto, &spectrumInv))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_SpectrumStatus() !!!\n", __F__, __L__);
		spectrumInv = LX_DEMOD_UNKOWN_STATE;
	}

	if (RET_OK != DEMOD_H13_DVBT_Get_DelaySpreadStatus(&delaySpread))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_DelaySpreadStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}

	if (RET_OK != DEMOD_H13_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_SpectrumInvStatus() !!!\n", __F__, __L__);
		delaySpread = 0xffff;
	}
	//TP Error count
	errorMode = LX_DEMOD_I2C_READ_BASED;
	if (RET_OK !=  DEMOD_H13_Get_Packet_Error(&errorCount))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_TPError() !!!\n", __F__, __L__);
		return RET_ERROR;
	}
	DEMOD_PRINT("[DEMOD H13] TP Error Count= %d !!!\n",errorCount);

	if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset( &FreqOff ))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
		FreqOff = 0xffff;
	}

 	//MSE

	if (RET_OK != DEMOD_H13_Get_MseInfo(&sigMse))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBC_Get_SNR() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H13] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H13] IF AGC value = %d(0x%x) !!!\n",ifagcValue,ifagcValue);
	DEMOD_PRINT("[DEMOD H13] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_DETECTED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H13 DTV] Spectrum auto = %s, Inversion  = %s !!!\n",(spectrumAuto == LX_DEMOD_AUTO) ? " Auto" : " Manual",
																					(spectrumInv == LX_DEMOD_INVERSION) ? " Inverted" : " Normal" );
	DEMOD_PRINT("[DEMOD H13] Delay Spread = %d !!!\n",delaySpread);
	DEMOD_PRINT("[DEMOD H13] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	DEMOD_PRINT("[DEMOD H13] offset = %d [KHz]!!!\n",FreqOff);
	DEMOD_PRINT("[DEMOD H13] mse = %d, constPwr = %d, !!!\n",sigMse.mse, sigMse.constPwr);

	if (RET_OK != DEMOD_H13_DVBT_Get_TpsInfo( &tppsParams))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_TpsInfo() !!!\n", __F__, __L__);
		return RET_ERROR;
	}




#ifndef  DEMOD_PRINT

	printk("[DEMOD H13] AGC LOCK is %s !!!\n",(agcLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H13] IF AGC value = %d !!!\n",ifagcValue);
	printk("[DEMOD H13] Neverlock is %s !!!\n",(neverLockStatus == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");
	printk("[DEMOD H13] Spectrum mode = %s !!!\n",(spectrumInv == LX_DEMOD_NORMAL) ? " Normal" :
																	(spectrumInv == LX_DEMOD_INVERSION) ? "Inversion" : 	" Unknown");
	printk("[DEMOD H13] Delay Spread = %d !!!\n",delaySpread);
	printk("[DEMOD H13] FFT mode LOCK = %s !!!\n",(txModeLock == LX_DEMOD_LOCKED) ? " Locked" : " NOT Locked");


	// Get bSetHp
	printk("[DEMOD H13] Set HP selection = %s !!!\n",(tppsParams->bSetHp == TRUE) ? " HP Mode" : " LP Mode");

	// Get channelBW
	printk("[DEMOD H13] Bandwidth = %s  !!!\n",
		(tppsParams.channelBW == LX_DEMOD_BW_6MHZ) ? "6MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_7MHZ) ? "7MHz" :
		(tppsParams.channelBW == LX_DEMOD_BW_8MHZ) ? "8MHz" : "UNKNOWN" );


	// Get FFT mode
	printk("[DEMOD H13] FFTMode = %s  !!!\n",
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_2K) ? "FFT_2K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_8K" :
		(tppsParams.FFTMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) ? "FFT_4K" : " UNKNOWN MODE" );


	// Get Guard Interval mode
	printk("[DEMOD H13] Guard Intterval = %s  !!!\n",
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_32) ? "1_32" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_16) ? "1_16" :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_8) ?  "1_8"  :
		(tppsParams.gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) ?  "1_4"  : " UNKNOWN MODE" );

	// Get Hierachical mode
	tppsParams->hierachMode = hierachMode;
	printk("[DEMOD H13] Hierachical Mode = %s  !!!\n",
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_NONE) ? "NON Hierachical" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_1) ? 	 "alpha = 1" :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_2) ?  	 "alpha = 2"  :
		(tppsParams.hierachMode == LX_DEMOD_DVB_TPS_HIERACHY_4) ?  	 "alpha = 4"  : " UNKNOWN MODE" );



	// Get LP coder rate
	printk("[DEMOD H13] LP code rate = %s  !!!\n",
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.lpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// Get HP coder rate
	printk("[DEMOD H13] HP code rate = %s  !!!\n",
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_1_2) ?  "Code_1_2" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_2_3) ? 	"Code_2_3" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_3_4) ?  "Code_3_4" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_5_6) ?  "Code_5_6" :
		(tppsParams.hpCodeRate == LX_DEMOD_CODE_7_8) ?  "Code_7_8" : " Code UNKNOWN" );


	// get QAM mode
	printk("[DEMOD H13] LP code rate = %s  !!!\n",
		(tppsParams.qamMode == LX_DEMOD_QPSK) ?   "QPSK"  :
		(tppsParams.qamMode == LX_DEMOD_16QAM) ?  "16QAM" :
		(tppsParams.qamMode == LX_DEMOD_64QAM) ?  "64QAM" : " UNKOWN MODE" );

	printk("[DEMOD H13] TP Error Count= %d !!!\n",errorCount);
	printk("[DEMOD H13] MSE = %d, constPwr = %d !!!\n",sigMse.mse, sigMse.constPwr);

#endif

	return RET_OK;
}




/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_Get_IFO_LOCK
 * Get IFO lock on DVB-T.
 *
 * @param *pSnr [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_IFO_LOCK( BOOLEAN *pIfoLock)
{
	UINT8	syslock;
	UINT16 	intfreqoffsetvalue = 0;
	UINT8	crifo_di_7_0;
	UINT8	crifo_di_9_8;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,syslock,syslock); //SYSLOCK : System lock is come from ifolock for DVB-T, from  framelock for DVB-C


	Sync_RdFL(sync_mon36);  //regAddr = 0x10a4,SYNC_MON[36]
	Sync_Rd01(sync_mon36,crifo_di_9_8,crifo_di_9_8); //CRIFO_DI[9:8] : Indicates IFO offset

	Sync_RdFL(sync_mon37);  //regAddr = 0x10a5, SYNC_MON[37]
	Sync_Rd01(sync_mon37,crifo_di_7_0,crifo_di_7_0); //CRIFO_DI[7:0] : Indicates IFO offset

	intfreqoffsetvalue = (crifo_di_9_8 << 8) + crifo_di_7_0;

	DEMOD_PRINT("[DEMOD H13](%s) IFO LOCK = %s !!!\n", __F__, (syslock == 1) ? "LOCK" :"NOT LOCK");
	DEMOD_PRINT("[DEMOD H13](%s) IFO value = %d !!!\n", __F__,intfreqoffsetvalue);

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
 * DEMOD_H13_DVBT_Get_CochanDetIndicator
 * Get co-channel detection indicator  on DVB-T.
 *
 * @param *pDetStatus  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_Get_CochanDetIndicator( BOOLEAN *pDetStatus)
{
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

	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_DVBT_EqualizereReset.
 * Reset DVB EQ module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_DVBT_EqualizereReset(void)
{

	UINT8 softrsteqb;

	softrsteqb = 0; //SOFTRSTEQB -> 0 : reset Equalizer

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

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
 * DEMOD_H13_DVBT_Obtaining_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_DVBT_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	static LX_DEMOD_FLAG_T 			scanProcessing = LX_DEMOD_FLAG_START;
	static LX_DEMOD_LOCK_STATE_T 	scanLock = LX_DEMOD_UNLOCKED;
	static unsigned int 			sigStablestartTime = 0;


	switch (pDemodInfo->controlState)
	{

											/***************************************************/
											/***** Configure demod	for RETUNE. *****/
		case LX_DEMOD_RETUNE_RF:			/***************************************************/
			{

				if(RET_OK != DEMOD_H13_DVBT_Set_DefaultRegisterValue(pDemodInfo->setCfgParam.dvbt.channelBW))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_DefaultRegisterValue() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				DEMOD_PRINT("[DEMOD H13] [DEMOD H13] MONITORING FUNC : change to CIR power level(0x86) !!!\n");
				if(RET_OK != DEMOD_H13_DVBT_Set_NeverLockWaitTime(LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_NeverLockWaitTime() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
				{
					if(RET_OK != DEMOD_H13_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
				else
				{

#if 1
					if(RET_OK != DEMOD_H13_DVBT_Set_Config_auto(TRUE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBT_Set_PartialConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_PartialConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#else
					if(RET_OK != DEMOD_H13_DVBT_Set_Config_auto(FALSE))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

					if(RET_OK != DEMOD_H13_DVBT_Set_FullConfig(&(pDemodInfo->setCfgParam.dvbt)))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_FullConfig() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}

#endif

				}

				pDemodInfo->controlState = LX_DEMOD_TUNE_START;
				DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_TUNE_START..... ^0^\n");
				g_RetuneStartTime = jiffies_to_msecs(jiffies);

			}

			break;


											/***************************************************/
											/***** Configure tuner for desired channel No. *****/
		case LX_DEMOD_TUNE_START:			/***************************************************/
			{


				if(RET_OK != DEMOD_H13_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_TUNE_NORMAL != pDemodInfo->setCfgParam.dvbt.tuneMode)
				{

					if(RET_OK != DEMOD_H13_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_ON, &scanProcessing, &scanLock))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_NeverlockScan() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					pDemodInfo->controlState = LX_DEMOD_TUNE_SCAN;
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_TUNE_START : Going to LX_DEMOD_TUNE_SCAN..... ^0^\n");


				}
				else
				{


					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_TUNE_START : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

					DEMOD_PRINT( "^c^[DEMOD H13] ERR (%s:%d) Already Done !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");
					return RET_OK;
				}

				if(RET_OK != DEMOD_H13_NeverlockScan(pDemodInfo, LX_DEMOD_FLAG_OFF, &scanProcessing, &scanLock))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_NeverlockScan() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((LX_DEMOD_FLAG_FINISH == scanProcessing) && (LX_DEMOD_LOCKED == scanLock))
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_FINISH;
					pDemodInfo->controlState = LX_DEMOD_SYNC_CHECKING;

					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Signal is Found  !!!\n", __F__, __L__);
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_TUNE_SCAN : Going to LX_DEMOD_SYNC_CHECKING..... ^0^\n");

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

					DEMOD_PRINT_ALWAYS("\nLX_DEMOD_TUNE_SCAN : MSG_DVBT_FRONT_END_NOT_LOCKED..\n") ;
					DEMOD_PRINT_ALWAYS("LX_DEMOD_TUNE_SCAN : Elapse time is %d msec\n", elapsedTime) ;

					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Signal is NOT Found  !!!\n", __F__, __L__);



				}
				else
				{
					pDemodInfo->scanJobFlag = LX_DEMOD_FLAG_ING;
					DEMOD_PRINT( "^G^[DEMOD H13] (%s:%d) Detection is on processing	!!!\n", __F__, __L__);
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

				if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{
					DEMOD_H13_DVBT_Set_TafcWaitSymbol(TRUE);

					pDemodInfo->controlState	= LX_DEMOD_SIGNAL_STABLE;

					sigStablestartTime = jiffies_to_msecs(jiffies);

					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going to LX_DEMOD_SIGNAL_STABLE..... ^0^\n");
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

				LX_DEMOD_LOCK_STATE_T	txModeLock;
				LX_DEMOD_FFTMODE_T		fftMode;
				LX_DEMOD_GIMODE_T		gIMode;
				LX_DEMOD_RF_MODE_T 		qamMode;


				if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Lock() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if(LX_DEMOD_LOCKED == lockState)
				{

					elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;

					if(elapsedTime < LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT)
					{
						break;
					}
					else
					{
						if(RET_OK != DEMOD_H13_Get_Packet_Error(&packetError))
						{
							DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_Packet_Error() !!!\n", __F__, __L__);
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

								if((LX_DEMOD_BW_6MHZ == pDemodInfo->setCfgParam.dvbt.channelBW) /* && (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbt.tuneMode)*/)
								{

									// Get FFT mode
									if (RET_OK != DEMOD_H13_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
									{
										DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}


									// Get Guard Interval mode
									if (RET_OK != DEMOD_H13_DVBT_Get_GuradIntervalMode( &gIMode ))
									{
										DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

									// Get QAM mode
									if (RET_OK != DEMOD_H13_Get_QAMMode( &qamMode ))
									{
										DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_QAMMode() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

									if((txModeLock == LX_DEMOD_LOCKED) && (fftMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) &&
													 (gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) &&(qamMode == LX_DEMOD_64QAM))
									{	// To resolve pre-echo stream TW 8K, 1/4

										if (RET_OK != DEMOD_H13_DVBT_Set_CirNormalize())
										{
											DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}
										DEMOD_PRINT("[DEMOD H13] (%s) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__ );

									}
									else
									{
										if (RET_OK != DEMOD_H13_DVBT_Set_CTFNR_Control(LX_DEMOD_CTFNR_1_16))
										{
											DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CTFNR_Control !!!\n", __F__, __L__);
											return RET_ERROR;
										}

										if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
										{
											DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
											return RET_ERROR;
										}

									}

								}

								if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset(&freqOffset))
								{
									DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
									DEMOD_RETURN_FAIL(__F__);
								}
								pDemodInfo->freqOffset = freqOffset;

								pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

								elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

								DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^^0^^^\n");
								DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^^0^^^\n", elapsedTime);
								DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^^0^^^\n", freqOffset);

								if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
								{
									DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
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
								DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Packet error = %d .....  --_--\n", packetError);
								elapsedTime = jiffies_to_msecs(jiffies) - sigStablestartTime;
								if(elapsedTime > LX_DEMOD_LOCK_DEALY_200)
								{
									pDemodInfo->bNotifiedLock	= TRUE;
									pDemodInfo->bNotifiedUnlock = FALSE;
									pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
									pDemodInfo->signalStatus.bSignalValid = TRUE;

									if((LX_DEMOD_BW_6MHZ == pDemodInfo->setCfgParam.dvbt.channelBW) /* && (LX_DEMOD_TUNE_NORMAL == pDemodInfo->setCfgParam.dvbt.tuneMode)*/)
									{

										// Get FFT mode
										if (RET_OK != DEMOD_H13_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
										{
											DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}


										// Get Guard Interval mode
										if (RET_OK != DEMOD_H13_DVBT_Get_GuradIntervalMode( &gIMode ))
										{
											DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}

										// Get QAM mode
										if (RET_OK != DEMOD_H13_Get_QAMMode( &qamMode ))
										{
											DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_QAMMode() !!!\n", __F__, __L__);
											DEMOD_RETURN_FAIL(__F__);
										}

										if((txModeLock == LX_DEMOD_LOCKED) && (fftMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) &&
														 (gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) &&(qamMode == LX_DEMOD_64QAM))
										{	// To resolve pre-echo stream TW 8K, 1/4

											if (RET_OK != DEMOD_H13_DVBT_Set_CirNormalize())
											{
												DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__, __L__);
												DEMOD_RETURN_FAIL(__F__);
											}
											DEMOD_PRINT("[DEMOD H13] (%s) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__ );

										}
										else
										{
											if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
											{
												DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
												return RET_ERROR;
											}

										}

									}


									if (RET_OK != DEMOD_H13_Get_CarrierFreqOffset(&freqOffset))
									{
										DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_CarrierFreqOffset() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}
									pDemodInfo->freqOffset = freqOffset;

									pDemodInfo->controlState	= LX_DEMOD_SIGNAL_LOCKED;

									elapsedTime = jiffies_to_msecs(jiffies) - g_RetuneStartTime;

									DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : MSG_DVBT_FRONT_END_LOCKED..... ^^0^^\n");
									DEMOD_PRINT_ALWAYS( "[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Elapsed Time = %d [ms] ..... ^^0^^\n", elapsedTime);
									DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SIGNAL_STABLE : Frequency offset = %d [KHz] ..... ^^0^^\n", freqOffset);

									if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}

									if (RET_OK != DEMOD_Get_signal_status(pDemodInfo))
									{
										DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
										DEMOD_RETURN_FAIL(__F__);
									}


								}

							}

						}

					}


				}
				else
				{
					pDemodInfo->controlState	= LX_DEMOD_SYNC_CHECKING;
					DEMOD_PRINT( "^R^[DEMOD H13] LX_DEMOD_SYNC_CHECKING : Going back to LX_DEMOD_SYNC_CHECKING..... **;\n");

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
 * DEMOD_H13_DVBT_Monitoring_Signal_Lock.
 * Set channel change parameters
 *
 * @param *pDemodInfo [IN] DEMOD_CTX_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_DVBT_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo)
{

	LX_DEMOD_LOCK_STATE_T			lockState;
	LX_DEMOD_LOCK_STATE_T	txModeLock;
	LX_DEMOD_FFTMODE_T		fftMode;
	LX_DEMOD_GIMODE_T		gIMode;
	LX_DEMOD_RF_MODE_T		qamMode;

	unsigned int			powerOnElapsedTime;


	if(RET_OK != DEMOD_H13_Get_Lock(LX_DEMOD_PSYNC_LOCK, &lockState))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_Get_Lock() !!!\n", __F__, __L__);
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
			DEMOD_PRINT( "[DEMOD H13] FAIL (%s:%d) DEMOD_Get_signal_status() !!!\n", __F__, __L__);
			DEMOD_RETURN_FAIL(__F__);
		}

		if(pDemodInfo->signalStatus.packetError >= _gFullDVBPkerrThreshold)
		{
			_gGBB_DVB_UnlockCount_full_packeterr++;
			DEMOD_PRINT( "^r^[DEMOD H13](%s) _gGBB_DVB_UnlockCount_full_packeterr = %d !!!\n", __F__,_gGBB_DVB_UnlockCount_full_packeterr);

			if(_gGBB_DVB_UnlockCount_full_packeterr == _gFullDVBPkerrCount)
			{
				DEMOD_PRINT( "^r^[DEMOD H13] oops !! (%s) Force to NOT LOCK !!\n", __F__);
				_gGBB_DVB_UnlockCount_full_packeterr = 0;

				if(RET_OK != DEMOD_H13_DVBT_Set_Config_auto(TRUE))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_DVBT_Set_Config_auto() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H13](%s) DEMOD_H13_DVBT_Set_Config_auto(TRUE) !!!\n", __F__);

				if(RET_OK != DEMOD_H13_SoftwareReset())
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_SoftwareReset() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}
				DEMOD_PRINT("[DEMOD H13](%s) DEMOD_SoftwareReset() !!!\n", __F__);

				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;
				DEMOD_PRINT_ALWAYS( "[DEMOD H13] MONITORING FUNC : MSG_DVBT_FRONT_END_NOT_LOCKED..... -_-;;\n");


			}

		}
		else
		{
			_gGBB_DVB_UnlockCount_full_packeterr = 0;

			if(gPowerOnInitDVB == 2)
			{
				powerOnElapsedTime = jiffies_to_msecs(jiffies) - gPowerOnInitStartTimeDVB;

				DEMOD_PRINT("[DEMOD H13](%s:%d) powerOnElapsedTime = %d !!!\n", __F__, __L__,powerOnElapsedTime);

				if(powerOnElapsedTime >= 60000)
				{
					gPowerOnInitDVB = 0;

					if (RET_OK != DEMOD_H13_DVBT_Set_CrGearshEn(TRUE ))
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CrGearshEn() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
				}
			}


			if((LX_DEMOD_BW_6MHZ == pDemodInfo->setCfgParam.dvbt.channelBW) )
			{

				// Get FFT mode
				if (RET_OK != DEMOD_H13_DVBT_Get_FFTMode( &txModeLock, &fftMode ))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_FFTMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}


				// Get Guard Interval mode
				if (RET_OK != DEMOD_H13_DVBT_Get_GuradIntervalMode( &gIMode ))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Get_GuradIntervalMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				// Get QAM mode
				if (RET_OK != DEMOD_H13_Get_QAMMode( &qamMode ))
				{
					DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_Get_QAMMode() !!!\n", __F__, __L__);
					DEMOD_RETURN_FAIL(__F__);
				}

				if((txModeLock == LX_DEMOD_LOCKED) && (fftMode == LX_DEMOD_DVBT_ISDBT_FFT_8K) &&
								 (gIMode == LX_DEMOD_DVBT_ISDBT_GIMODE_1_4) &&(qamMode == LX_DEMOD_64QAM))
				{	// To resolve pre-echo stream TW 8K, 1/4


					if (RET_OK != DEMOD_H13_DVBT_Set_CirNormalize())
					{
						DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__, __L__);
						DEMOD_RETURN_FAIL(__F__);
					}
					DEMOD_PRINT("[DEMOD H13] (%s) DEMOD_H13_DVBT_Set_CirNormalize() !!!\n", __F__ );

				}
				else
				{
					if (RET_OK != DEMOD_H13_DVBT_Set_CTFNR_Control(LX_DEMOD_CTFNR_1_16))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CTFNR_Control !!!\n", __F__, __L__);
						return RET_ERROR;
					}

					if (RET_OK != DEMOD_H13_DVBT_Set_CirPwrControl(LX_DEMOD_CIR_POWER_MID))
					{
						DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_DVBT_Set_CirPwrControl !!!\n", __F__, __L__);
						return RET_ERROR;
					}

				}

			}


			if((LX_DEMOD_LOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedLock))
			{
				pDemodInfo->lockCount++;
				DEMOD_PRINT( "^r^[DEMOD H13](%s) pDemodInfo->lockCount = %d !!!\n", __F__,pDemodInfo->lockCount);

			}
			if(pDemodInfo->lockCount == 2)
			{
				pDemodInfo->lockStatus		= LX_DEMOD_LOCKED;
				pDemodInfo->bNotifiedLock 	= TRUE;
				pDemodInfo->bNotifiedUnlock = FALSE;
				pDemodInfo->signalStatus.bSignalValid = TRUE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H13] MONITORING FUNC : MSG_DVBT_FRONT_END_LOCKED..... ^0^\n");

				if(pauseTimeDemod != LX_DEMOD_LOCKED_PAUSE_TIME )
					pauseTimeDemod = LX_DEMOD_LOCKED_PAUSE_TIME;

			}
			if(pDemodInfo->lockCount >= 2)
			{
				pDemodInfo->lockCount = 0;
			}

	        if (RET_OK != DEMOD_Get_TunedCFG(pDemodInfo))
	       	{
			  DEMOD_PRINT( "^r^[DEMOD H13] FAIL (%s:%d) DEMOD_Get_TunedCFG() !!!\n", __F__, __L__);
			  DEMOD_RETURN_FAIL(__F__);
	       	}

	    }

	}
	else
	{
		pDemodInfo->lockCount = 0;
		_gGBB_DVB_UnlockCount_full_packeterr = 0;

		if(pauseTimeDemod != LX_DEMOD_LOCK_FAIL_PAUSE_TIME )
			pauseTimeDemod = LX_DEMOD_LOCK_FAIL_PAUSE_TIME;

		if((LX_DEMOD_UNLOCKED != pDemodInfo->lockStatus)  && (TRUE != pDemodInfo->bNotifiedUnlock))
		{
			pDemodInfo->unLockCount++;
			DEMOD_PRINT( "^r^[DEMOD H13](%s) pDemodInfo->unLockCount = %d !!!\n", __F__,pDemodInfo->unLockCount);

			if(pDemodInfo->unLockCount == 4)
			{
				pDemodInfo->lockStatus 		= LX_DEMOD_UNLOCKED;
				pDemodInfo->bNotifiedUnlock = TRUE;
				pDemodInfo->bNotifiedLock	= FALSE;
				pDemodInfo->signalStatus.bSignalValid = FALSE;

				DEMOD_PRINT_ALWAYS( "[DEMOD H13] MONITORING FUNC : MSG_DVBT_FRONT_END_NOT_LOCKED..... -_-\n");

			}
			if(pDemodInfo->unLockCount >= 4)
			{
				pDemodInfo->unLockCount = 0;
			}
		}
		else
		{
			pDemodInfo->unLockLongCount++;

			if(pDemodInfo->unLockLongCount == 20)
			{
				pDemodInfo->controlState = LX_DEMOD_RETUNE_RF;
				DEMOD_PRINT( "^R^[DEMOD H13] MONITORING FUNC : Going to LX_DEMOD_RETUNE_RF..... ^0^\n");

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
			pDemodInfo->signalStatus.packetError	= 0xffffffff;

			pauseTimeDemod							= LX_DEMOD_NOT_LOCKED_PAUSE_TIME;


			notifyinglockTimeOut					= LX_DEMOD_LOCK_DEALY_1500;


		}



	}


	return RET_OK;



}




#if 0
#endif



