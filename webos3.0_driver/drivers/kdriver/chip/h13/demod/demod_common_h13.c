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

#include "sys_regs.h"

#include "demod_impl.h"
#include "demod_common_h13.h"
#include "demod_reg_h13.h"

#if 0
#endif


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_ResetHW.
 * Reset GBB demod ID.
 *
 * @param none  [IN/OUT]
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/


int DEMOD_H13_ResetHW(void)
{
	
	ACE_REG_H13A0_RdFL(soft_reset_0);
	
	if(ACE_REG_H13A0_RdFd(soft_reset_0,swrst_f24m) == 1)
	{
		ACE_REG_H13A0_RdFL(soft_reset_0);
		ACE_REG_H13A0_Wr01(soft_reset_0,swrst_f24m,0);
		ACE_REG_H13A0_WrFL(soft_reset_0);
		DEMOD_PRINT_ALWAYS("[KDRV_DEMOD] NEED TO BE RESET swrst_f24m \n");
		
	}
	else
	{
		DEMOD_PRINT_ALWAYS("[KDRV_DEMOD] ALREADY RESET swrst_f24m \n");
	}




	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ResetHW start !! \n");

	ACE_REG_H13A0_RdFL(gbb_0);

	ACE_REG_H13A0_Wr01(gbb_0, reset, 1);

	ACE_REG_H13A0_WrFL(gbb_0);

	OS_MsecSleep(10);


	ACE_REG_H13A0_Wr01(gbb_0, reset, 0);

	ACE_REG_H13A0_WrFL(gbb_0);


	 OS_MsecSleep(20);


	 ACE_REG_H13A0_Wr01(gbb_0, reset, 1);

	 ACE_REG_H13A0_WrFL(gbb_0);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ResetHW end !! \n");


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_SetI2C.
 * select normal mode
 *
 * @param none  [IN/OUT]
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_SetI2C(void)
{
	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetI2C Start !! \n");

	ACE_REG_H13A0_RdFL(gbb_0);

	ACE_REG_H13A0_Wr01(gbb_0, i2csel, 0);

	ACE_REG_H13A0_WrFL(gbb_0);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetI2C end !! \n");
	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_SetInclk.
 * select inclk mode
 *
 * @param inclk  [IN] LX_DEMOD_INCLK_SEL_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_SetInclk(LX_DEMOD_INCLK_SEL_T *inclk)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk Start !! \n");

	ACE_REG_H13A0_RdFL(gbb_afe_0);

	switch(*inclk)
	{
		case LX_DEMOD_ARM_SETTING:
		{
			ACE_REG_H13A0_Wr01(gbb_afe_0, inck_sel, 0);
		}
		break;

		case LX_DEMOD_EXTERN_PIN:
		{

			ACE_REG_H13A0_Wr01(gbb_afe_0, inck_sel, 1);
		}
		break;

		default:
			DEMOD_PRINT("Invalid LX_DEMOD_INCLK_SEL_T type\n");
			return -1;
	}

	ACE_REG_H13A0_WrFL(gbb_afe_0);



	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk End !! \n");

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_RepeaterEnable.
 * Enable / Disable GBB I2C repeater function
 *
 * @param bEnable [IN] 	BOOLEAN - Enable/Disable I2C repeater
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_RepeaterEnable(BOOLEAN bEnable)
{
	UINT8 ni2crpten;

	ni2crpten = (bEnable == TRUE)?(0):(1); //NI2CRPTEN : Tuner I2C port control , 0 -> enable, 1-> disable

	Common_RdFL(common_ctrl2);   //regAddr = 0x0002, COMMON_CTRL[2]
	Common_Wr01(common_ctrl2, ni2crpten, ni2crpten);
	Common_WrFL(common_ctrl2);
	//NI2CRPTEN (bit7) : I2C repetition enable control, This is not validated at DTV SoC(0 : enable, 1 : disable)

	DEMOD_PRINT("^r^[DEMOD H13] success!!  DVB_RepeaterEnable =  %s ", (bEnable == TRUE)?"ON":"OFF");
	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_SoftwareReset.
 * Reset DVB module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_SoftwareReset(void)
{

	UINT8 softrstb;

	softrstb = 0; //SOFTRSTB -> 0 : reset

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
	Common_Wr01(common_ctrl0, softrstb, softrstb); 	//SOFTRSTB (bit7) : Function reset control(0 : reset, 1 : normal operation)
	Common_WrFL(common_ctrl0);

	OS_MsecSleep(20);

	softrstb = 1; //SOFTRSTB -> 1 : normal

	Common_Wr01(common_ctrl0, softrstb, softrstb);
	Common_WrFL(common_ctrl0);


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Serial_Control
 * Control TS output mode(serial/Parallel)
 *
 * @param bEnable 	[IN] Serial(TRUE) / Parallel(FALSE)
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Serial_Control(BOOLEAN bEnable)
{
	// not supported, only parellel output

	UINT8	tpsenb;

	tpsenb = (bEnable == TRUE)?(0):(1);


	Common_RdFL(common_ctrl113);  //regAddr = 0x0071, Common control register  Transport packet signal output control register
	Common_Wr01(common_ctrl113, tpsenb, tpsenb);//tpsenb : '1'-> Parallel, '0' -> Serial.
	Common_WrFL(common_ctrl113);

	DEMOD_PRINT("^r^[DEMOD H13 DTV] success!!  DEMOD_H13_VQI_Serial_Control =  %s ", (bEnable == TRUE)? "SERIAL":"PARALLEL");


	return	RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Power_Save
 * Enable/Disable power save mode
 *
 * @param bEnable 	[IN] power save mode(TRUE) / normal mode(FALSE)
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H13_Power_Save(BOOLEAN bEnable)
{

	UINT8 	gbbpdb;
	UINT8	softrstb;

	DEMOD_PRINT("[DEMOD H13 DTV] @ ######chungiii   %s =====\n",__F__);


	if(bEnable == TRUE)
	{

		softrstb = 0;
		Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
		Common_Wr01(common_ctrl0, softrstb, softrstb); //softrstb '0' -> reset, '1' -> normal operation.
		Common_WrFL(common_ctrl0);

		gbbpdb = 0;
		Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
		Common_Wr01(common_ctrl0, gbbpdb, gbbpdb); //GBBPDB : GBB whole function power down control (0 : demodulator power down, 1 : normal operation)
		Common_WrFL(common_ctrl0);

	}
	else
	{
		gbbpdb = 1;
		Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
		Common_Wr01(common_ctrl0, gbbpdb, gbbpdb); //GBBPDB : GBB whole function power down control (0 : demodulator power down, 1 : normal operation)
		Common_WrFL(common_ctrl0);

		softrstb = 1;
		Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
		Common_Wr01(common_ctrl0, softrstb, softrstb); //softrstb '0' -> reset, '1' -> normal operation.
		Common_WrFL(common_ctrl0);

	}


	DEMOD_PRINT("[DEMOD H13] success!!  DEMOD_H13_Power_Save =  %s  %d\n ", (bEnable == TRUE)? "ON":"OFF", gbbpdb);

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_StdOperModeContrl
 * Set standard operation mode, Bandwidth and symbol rate
 *
 * @param *InitParam [IN] LX_DEMOD_INIT_PARAM_T - operation RF Mode, bandWidth, Symbol rate
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_StdOperModeContrl( LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr)
{
	UINT8 	stdopmode_7_0;
	UINT8 	stdopdettmode_2_0;
	UINT8 	stdopdetcmode_1_0;

	UINT8	samfreq_21_16 = 0;
	UINT8	samfreq_15_8 = 0;
	UINT8	samfreq_7_0 = 0;


	UINT8 simfastenb;
	UINT8 pllsetauto;
	UINT8 pllctrl_13_8;
	UINT8 pllctrl_7_0;
	UINT8 sampling4xfen;

	UINT32 	samplingFreq;

	DEMOD_PRINT(" == @ %s  ==(oper = 0x%x)\n",__F__,operMode);

	simfastenb = 1;
	Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
	Common_Wr01(common_ctrl1, simfastenb, simfastenb);
	Common_WrFL(common_ctrl1);
	//SIMFASTENB: Chip operation mode control
	//This signal should be set to '1' at the start of operation
	// 0 : simluation mode(for chip test only)
	// 1 : normal operation mode

	pllsetauto = 0;
	Common_RdFL(common_ctrl5);	 //regAddr = 0x0005
	Common_Wr01(common_ctrl5, pllsetauto, pllsetauto);
	Common_WrFL(common_ctrl5);
	//PLLSETAUTO : Automatic PLL set value and sampling clock frequency control by operation standard(@0x08)
	// 0 : manual mode. Controlled by I2C
	// 1 : pre-defined mode mode


	switch(operMode)
	{
		case LX_DEMOD_ATSC:
			stdopmode_7_0 	= 0x80;

			samfreq_21_16 	= 0x18; //For 24MHz
			samfreq_15_8 	= 0x00;
			samfreq_7_0 	= 0x00;

			sampling4xfen 	= 0;
			pllctrl_13_8 	= 0x25;  //For 24MHz
			pllctrl_7_0 	= 0x64; // For 24MHz

			break;

		case LX_DEMOD_QAM:
			stdopmode_7_0 	= 0x8;

			samfreq_21_16 	= 0x18; //For 24MHz
			samfreq_15_8 	= 0x00;
			samfreq_7_0 	= 0x00;

			sampling4xfen 	= 1;
			pllctrl_13_8 	= 0x25;  //For 24MHz
			pllctrl_7_0 	= 0x64; // For 24MHz

			break;

		case LX_DEMOD_ISDBT:
			stdopmode_7_0 	= 0x20;

			samfreq_21_16 	= 0x18; //For 24MHz
			samfreq_15_8 	= 0x00;
			samfreq_7_0 	= 0x00;

			sampling4xfen 	= 0;
			pllctrl_13_8 	= 0x25;  //For 24MHz
			pllctrl_7_0 	= 0x64; // For 24MHz

			break;

		case LX_DEMOD_DVBT:
			stdopmode_7_0 	= 0x40;

			samfreq_21_16 	= 0x18; //For 24MHz
			samfreq_15_8 	= 0x00;
			samfreq_7_0 	= 0x00;

			sampling4xfen 	= 0;
			pllctrl_13_8 	= 0x25;  //For 24MHz
			pllctrl_7_0 	= 0x64; // For 24MHz

			break;

		case LX_DEMOD_DVBC:
			stdopmode_7_0 	= 0x4;

			samfreq_21_16 	= 0x1D; //For 29MHz
			samfreq_15_8 	= 0x00;
			samfreq_7_0 	= 0x00;

			sampling4xfen 	= 1;
			pllctrl_13_8 	= 0x26;  //For 29MHz
			pllctrl_7_0 	= 0xe5; // For 29MHz

			break;


		default :
			DEMOD_PRINT("[DEMOD H13](%s,%d) Invaild Opermode parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}


	switch(bandWd)
	{
		case LX_DEMOD_VSB_NO_OPERATION:
			stdopdettmode_2_0 = 0x0;
			break;

		case LX_DEMOD_DVBT_NO_OPERATION:
			stdopdettmode_2_0 = 0x0;
			break;

		case LX_DEMOD_BW_6MHZ:
			stdopdettmode_2_0 = 0x3;
			break;

		case LX_DEMOD_BW_7MHZ:
			stdopdettmode_2_0 = 0x2;
			break;

		case LX_DEMOD_BW_8MHZ:

			stdopdettmode_2_0 = 0x1;

			break;

		case LX_DEMOD_BW_7M_6MHZ:
			stdopdettmode_2_0 = 0x4;
			break;

		case LX_DEMOD_BW_8M_6MHZ:
			stdopdettmode_2_0 = 0x5;
			break;

		case LX_DEMOD_BW_8M_7MHZ:
			stdopdettmode_2_0 = 0x6;
			break;

		case LX_DEMOD_BW_8M_7M_6MHZ:
			stdopdettmode_2_0 = 0x7;
			break;


		default :
			DEMOD_PRINT("[DEMOD H13](%s,%d) Invalid Bandwidth parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}

	switch(symbr)
	{
		case LX_DEMOD_SYMR_QAM_NO_OPERATION_MODE:
		case LX_DEMOD_SYMR_DVBC_NO_OPERATION_MODE:
			stdopdetcmode_1_0 = 0x0;
			break;

		case LX_DEMOD_SYMR_FIXED_MODE:
			stdopdetcmode_1_0 = 0x1;
			break;


		case LX_DEMOD_SYMR_FULL_SCAN_MODE:
			stdopdetcmode_1_0 = 0x3;
			break;

		default :
			DEMOD_PRINT("[DEMOD H13](%s,%d) Invalid Symbol rate parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}



	Common_RdFL(common_ctrl8); //regAddr = 0x0008[7:0]
	Common_Wr01(common_ctrl8,stdopmode_7_0, stdopmode_7_0 );  //STDOPMODE[7:0]
	Common_WrFL(common_ctrl8);
	//STDOPMODE[7:0] : Standard mode selection
	// 00000010 : ISDB-C
	// 00000100 : DVB-C
	// 00001000 : QAM
	// 00010000 : DTMB(reserved)
	// 00100000 : ISDB-T
	// 01000000 : DVB-T
	// 10000000 : VSB


	Common_RdFL(common_ctrl9);   //regAddr = 0x0009[4:2] : stdopdettmode_2_0, 0x0009[1:0] : stdopdetcmode_1_0
	Common_Wr02(common_ctrl9,stdopdettmode_2_0, stdopdettmode_2_0, stdopdetcmode_1_0, stdopdetcmode_1_0 );
	Common_WrFL(common_ctrl9);
	//STDOPDETTMODE[2:0]  : Selection of bandwidth detection mode for terrestrial standard(VSB,DVB-T) reception.
	// 000 : no operation for terrestrial standard
	// 001 : 8MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 010 : 7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 011 : 6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 100 : 7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 101 : 8,6MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 110 : 8,7MHz(DVB-T), 6MHz(VSB, ISDB-T)
	// 111 : 8,7,6MHz(DVB-T), 6MHz(VSB, ISDB-T)

	// STDOPDETCMODE[1:0] : Selection of symbol rate detection mode for DVB-C standard reception and QAM mode detection mode for QAM standard reception
	// Pre-defined QAM mode is set by SYNC_CTRL[26](@0x101A[2:0])
	// 00 : no operation for cable standard
	// 01 : pre-defined symbol rate detection for DVB-C, pre-defined QAM mode detection for QAM
	// 1x : full scan detection of symbol rate for DVB-C, 64/256QAM detection for QAM

	Common_RdFL(common_ctrl13);   //regAddr = 0x000d, COMMON_CTRL[13]
	Common_Wr01(common_ctrl13,sampling4xfen, sampling4xfen );
	Common_WrFL(common_ctrl13);
	//SAMPLING4XFEN : Selection of ADC sampling frequency rate with respect to symbol rate mode at manual mode
	// 1 : 4x sampling
	// 0 : 2x sampling

	Common_RdFL(common_ctrl5);	 //regAddr = 0x0005
	Common_Wr01(common_ctrl5, pllctrl_13_8, pllctrl_13_8);
	Common_WrFL(common_ctrl5);
	// PLLCTRL[13:8], PLLCTRL[7:0]  : 0x26E1 for 200MHz for VSB & QAM, 0x26E5 for 232MHz for DVB-T/C

	Common_RdFL(common_ctrl6);	 //regAddr = 0x0006
	Common_Wr01(common_ctrl6, pllctrl_7_0, pllctrl_7_0);
	Common_WrFL(common_ctrl6);

	Common_RdFL(common_ctrl13); //regAddr = 0x000d, SAMFREQ[21:16] : Fs(MHz) = SAMFREQ / 2^16, SAMFREQ = Fs(MHz) x 2^16
	Common_Wr01(common_ctrl13,samfreq_21_16, samfreq_21_16 );  //SAMFREQ[21:16] : for DVB-T/C, SAMFREQ should be set to 0x1D
	Common_WrFL(common_ctrl13);

	Common_RdFL(common_ctrl14); //regAddr = 0x000E, SAMFREQ[15:8]
	Common_Wr01(common_ctrl14,samfreq_15_8, samfreq_15_8 );  //SAMFREQ[15:8] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl14);

	Common_RdFL(common_ctrl15); //regAddr = 0x000F, SAMFREQ[7:0]
	Common_Wr01(common_ctrl15,samfreq_7_0, samfreq_7_0 );  //SAMFREQ[7:0] : for DVB-T/C, SAMFREQ should be set to 0x00
	Common_WrFL(common_ctrl15);


	/*
	Set of ADC sampling frequency(Fs)
	Fs(MHz) = SAMFREQ / 2^16
	SAMFREQ = Fs(MHz) x 2^16
	for VSB,QAM, SAMFREQ should be set to 0x190000
	for DVB-T/C, SAMFREQ should be set to 0x1D0000
	*/

	samplingFreq = (samfreq_21_16 << 16) + (samfreq_15_8 << 8) + (samfreq_7_0 << 0);

	DEMOD_PRINT("[DEMOD H13] Sampling frequency is %d [MHz] (%d)!!!\n",samplingFreq/65536, samplingFreq );

	return RET_OK;

}


/**
 * DEMOD_H13_NeverlockScan
 * Get the neverlock flag indicator.
 *
 * @param	*pDemodInfo			[IN] DEMOD_CTX_T.
 * @param	ResetOn				[IN] 	LX_DEMOD_FLAG_T  - force to initialize the postjob.
 * @param	*pScanProcessing		[OUT] LX_DEMOD_FLAG_T- the flag that scan is done or not done.
 * @param	*pScanLock			[OUT] LX_DEMOD_LOCK_STATE_T - scan lock result.
 * @return if Succeeded - RET_OK else-TU_ERROR
 * @author 	Jeongpil.yun(jeongpil.yun@lge.com)
*/
int DEMOD_H13_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock)
{

	static 	LX_DEMOD_LOCK_STATE_T	_demodScanLock	= LX_DEMOD_UNKOWN_STATE;

	unsigned int elapsedTime = 0;
	static	unsigned int	_neverLockResetTime	= 0;

	static unsigned int _maxTimeOut = 0;


	LX_DEMOD_LOCK_STATE_T neverLockStatus;
	LX_DEMOD_OPER_MODE_T scanMode;


	scanMode = pDemodInfo->operMode;

	if(LX_DEMOD_FLAG_ON == ResetOn)
	{

		*pScanProcessing = LX_DEMOD_FLAG_ING;
		*pScanLock	= LX_DEMOD_UNLOCKED;

		switch(scanMode)
		{
			case LX_DEMOD_DVBT :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_450;
				break;

			case LX_DEMOD_DVBC :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_800;
				break;

			case LX_DEMOD_ATSC :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

			case LX_DEMOD_QAM :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

			case LX_DEMOD_ISDBT :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_800;
				break;

			default :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

		}


		_neverLockResetTime = jiffies_to_msecs(jiffies);

		return RET_OK;
	}


	if (RET_OK != DEMOD_H13_Get_NeverLockStatus(scanMode, &neverLockStatus))
	{
		DEMOD_PRINT("^r^[GBB_DVB DTV] FAIL (%s:%d) KADP_DEMOD_VQI_ChanScan_GetStatus() !!!\n", __F__, __L__);
		*pScanLock	= LX_DEMOD_UNLOCKED;
		return RET_ERROR;
	}

	switch (neverLockStatus)
	{
		case LX_DEMOD_IDLE :
			elapsedTime = jiffies_to_msecs(jiffies) - _neverLockResetTime;

			if(elapsedTime > _maxTimeOut)
			{
				*pScanProcessing	= LX_DEMOD_FLAG_FINISH;
				_demodScanLock		= LX_DEMOD_UNLOCKED;
				DEMOD_PRINT( "[DEMOD H13] NEVERSCAN : LX_DEMOD_NEVERLOCK DETECTION is on initializing \n");

			}
			break;

		case LX_DEMOD_DETECTION_PROCESSING:
			{

				elapsedTime = jiffies_to_msecs(jiffies) - _neverLockResetTime;

				if(elapsedTime > _maxTimeOut)
				{
					*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
					_demodScanLock		= LX_DEMOD_UNLOCKED;
					DEMOD_PRINT( "[DEMOD H13] NEVERSCAN : LX_DEMOD_NEVERLOCK DETECTION is on processing \n");

				}
			}
			break;

		case LX_DEMOD_DETECTION_FAIL:
			{
				*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
				_demodScanLock		= LX_DEMOD_UNLOCKED;
				DEMOD_PRINT( "[DEMOD H13] NEVERSCAN :  LX_DEMOD_NEVERLOCK DETECTION FAIL  \n");
			}
			break;

		case LX_DEMOD_DETECTED:
			{
				*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
				_demodScanLock		= LX_DEMOD_TR_LOCK;
				DEMOD_PRINT( "[DEMOD H13] NEVERSCAN :  LX_DEMOD_NEVERLOCK DETECTED  !! \n");

			}
			break;


		default:
			_demodScanLock	= LX_DEMOD_UNLOCKED;
			break;
	}

	*pScanLock	= _demodScanLock;

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_TPOutEnable
 * Enable/Diable the output of TP 
 *
 * @param bEnable	  [IN] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_TPOutEnable(BOOLEAN bEnable)
{

	UINT8 	tpouten;

	DEMOD_PRINT(" @DEMOD_H13_TPOutCLKEnable\n");

	tpouten = (bEnable == TRUE)?(1):(0);
	Common_RdFL(common_ctrl112);   //regAddr = 0x0070[7]
	Common_Wr01(common_ctrl112, tpouten, tpouten);
	Common_WrFL(common_ctrl112);

	DEMOD_TRACE(" DEMOD_H13_TPOutEnable(%s)\n",(bEnable == TRUE) ? "ENABLE":"DISABLE");

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Set_IF_Frq
 * Set IF frequency registers on DVB-C mode.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Set_IF_Frq(UINT32 ifFrq)
{

	UINT32  finalFreq;

	UINT8	iffreq_7_0;
	UINT8	iffreq_15_8;

	DEMOD_PRINT(" @DEMOD_H13_Set_IF_Frq\n");

	finalFreq = (UINT64)ifFrq * 4096 ;	 // 4096 = 2 ^12, IFFRQ = IF center [MHz] x 2^12
										//Fc(MHz) = IFFREQ / 2^12
										//IFFREQ = Fc(MHz) x 2^12
	finalFreq = finalFreq / 1000 ;

	iffreq_15_8 = (UINT8) ((finalFreq >> 8) & 0xff);
	Common_Wr01(common_ctrl16,iffreq_15_8,iffreq_15_8); // IFFREQ[15:8] :Set of center frequency(Fc) of input signal of ADC
	Common_WrFL(common_ctrl16);  //regAddr = 0x0010, COMMON_CTRL[16] :

	iffreq_7_0 = (UINT8) (finalFreq & 0xff);
	Common_Wr01(common_ctrl17,iffreq_7_0,iffreq_7_0); // IFFREQ[7:0] :Set of center frequency(Fc) of input signal of ADC
	Common_WrFL(common_ctrl17);  //regAddr = 0x0011, COMMON_CTRL[17]

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Set_AGCPolarity
 * Set IF frequency registers on DVB.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Set_AGCPolarity(LX_DEMOD_AGC_POLAR_T agcPola)
{

	UINT8 	agcoutpol;

	DEMOD_PRINT(" @DEMOD_H13_DVBT_Set_AGCPolarity\n");

	agcoutpol = (agcPola == LX_DEMOD_AGC_ACTIVE_HIGH)?(1):(0);
	Common_RdFL(common_ctrl3);   //regAddr = 0x0003, COMMON_CTRL[3]
	Common_Wr01(common_ctrl3, agcoutpol, agcoutpol);
	Common_WrFL(common_ctrl3);

	//AGCOUTPOL : Selection of the gain control signal output polarity
	// 0 : low means gain up
	// 1 : high means gain up

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Set_SpectrumCtrl.
 * Enable/Disable automatic spectrum mode detection.
 *
 * @param  bAutoEnable [IN] BOOLEAN - enable/disable.
 * @param  bSpecInvEnable [IN] BOOLEAN - enable/disable.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Set_SpectrumCtrl(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable)
{

	UINT8	specinvauto;
	UINT8 	specinv;

	if(bAutoEnable == TRUE)
	{
		specinvauto = 1;  // SPECINVAUTO : '0' -> manual detection by SPECINV(@0x02[2]), '1' -> automatic detection.

		Common_RdFL(common_ctrl2); //regAddr = 0x0002[3], Common control register.
		Common_Wr01(common_ctrl2, specinvauto, specinvauto);
		Common_WrFL(common_ctrl2);
		DEMOD_PRINT( "[DEMOD H13 DTV] Success  AUTO -> %s !!!\n", (bAutoEnable == TRUE)?"ON":"OFF");
	}
	else
	{
		specinvauto = 0;  // SPECINVAUTO : '0' -> manual detection by SPECINV(@0x02[2]), '1' -> automatic detection.
		specinv = (bSpecInvEnable == TRUE)?(0x1):(0x00); //SPECINV : Enable spectrum Inversion  :  '0'-> normal , '1' -> inversion.

		Common_RdFL(common_ctrl2); //regAddr = 0x0002[3], Common control register.
		Common_Wr02(common_ctrl2, specinvauto, specinvauto, specinv, specinv);
		Common_WrFL(common_ctrl2);

		DEMOD_PRINT( "[DEMOD H13 DTV] Success Auto -> manual, Inversion ->  %s !!!\n",(bSpecInvEnable == TRUE)?"ON":"OFF");
	}

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_Id.
 * Get GBB demod ID.
 *
 * @param *cell_id [OUT] UINT16 - GBB demod ID
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_Id(UINT16 *pId)
{
	
	*pId = 0;

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_SpectrumInvStatus
 * Get spectrum mode state on DVB.
 *
 * @param *pSpectrumAuto 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_AUTO, LX_DEMOD_MANUAL.
 * @param *pSpectrumInv 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_NORMAL, LX_DEMOD_INVERSION.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_SpectrumStatus(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv)
{

	UINT8 specinvstatus;
	UINT8 specinvauto;

	Common_RdFL(common_ctrl2); //regAddr = 0x0002[3], Common control register.
	Common_Rd01(common_ctrl2, specinvauto, specinvauto);

	Common_RdFL(common_mon5);  //regAddr = 0x0085, COMMON_MON[4]
	Common_Rd01(common_mon5,specinvstatus,specinvstatus); //SPECINVSTATUS : '0'-> inversion, '1'-> normal.

	if(specinvauto)
	{
		*pSpectrumAuto = LX_DEMOD_AUTO;
	}
	else
	{
		*pSpectrumAuto = LX_DEMOD_MANUAL;
	}

	if(specinvstatus)
	{
		*pSpectrumInv = LX_DEMOD_INVERSION;
	}
	else
	{
		*pSpectrumInv = LX_DEMOD_NORMAL;
	}

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_IFAGCValue
 * Get IF AGC value on DVB.
 *
 * @param *pIfagcValue 	[OUT] UINT16
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_IFAGC(UINT16 *pIfAgc)
{

	UINT8	agcaccout_15_8;
	UINT8	agcaccout_7_0;

	UINT16 ifagc = 0;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	*pIfAgc = 0;

	Common_RdFL(common_mon14);  //regAddr = 0x008e, COMMON_MON[14]
	Common_Rd01(common_mon14,agcaccout_15_8,agcaccout_15_8); //AGCACCOUT[15:8]: gain error accumulation value monitoring for AGC
	ifagc = (agcaccout_15_8 << 8);

	Common_RdFL(common_mon15);  //regAddr = 0x008f, COMMON_MON[15]
	Common_Rd01(common_mon15,agcaccout_7_0,agcaccout_7_0); //AGCACCOUT[7:0] : gain error accumulation value monitoring for AGC
	ifagc += (agcaccout_7_0 << 0);

	*pIfAgc = ifagc;

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_OperMode.
 * Get GBB operation mode.
 *
 * @param *pOperMode [OUT] LX_DEMOD_VQI_RF_MODE_T - GBB RF demodulation mode
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_OperMode(LX_DEMOD_OPER_MODE_T *pOperMode)
{

	UINT8	detstdopmode_7_0 = 0;


	*pOperMode = LX_DEMOD_UNKOWN_OPMODE;

	Common_RdFL(common_mon1); //regAddr = 0x0081, COMMON_MON0[1]
	Common_Rd01(common_mon1,detstdopmode_7_0, detstdopmode_7_0 );  //DETSTDOPMODE[7:0]

/*
	Standard mode selection
	00000010 :reserved
	00000100 : DVB-C
	00001000 : QAM
	00010000 : reserved
	00100000 : ISDBT
	01000000 : DVB-T
	10000000 : VSB
  */

	if(detstdopmode_7_0 & 0x08) *pOperMode = LX_DEMOD_QAM;
	else if(detstdopmode_7_0 & 0x80) *pOperMode = LX_DEMOD_ATSC;
	else if(detstdopmode_7_0 & 0x20) *pOperMode = LX_DEMOD_ISDBT;
	else if(detstdopmode_7_0 & 0x40) *pOperMode = LX_DEMOD_DVBT;
	else if(detstdopmode_7_0 & 0x4) *pOperMode = LX_DEMOD_DVBC;
	else *pOperMode = LX_DEMOD_UNKOWN_OPMODE;


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_NeverLockStatus
 * Get neverlock state on DVB.
 *
 * @param scanMode 	[OUT] LX_DEMOD_OPER_MODE_T
 * @param *pNeverLockStatus 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMODLOCKED, LX_DEMOD_UNLOCKED.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_NeverLockStatus(LX_DEMOD_OPER_MODE_T scanMode, LX_DEMOD_LOCK_STATE_T *pNeverLockStatus)
{
	UINT8 neverlock_1_0;
	UINT8 srdstatus_1_0;
	UINT8 tpslockeq_d;
	UINT8 syslock;

	*pNeverLockStatus = LX_DEMOD_UNKOWN_STATE;

	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,neverlock_1_0,neverlock_1_0); //NEVERLOCK[1:0] : Never lock for signal existence status indicaiton
	// 00 : init
	// 01 : processing
	// 10 : lock
	// 11 : fail


	Sync_RdFL(sync_mon1); //regAddr = 0x1081, SYS_MON[1]: System control monitoring
	Sync_Rd01(sync_mon1,srdstatus_1_0,srdstatus_1_0 );
	//SRDSTATUS[1:0] : SRD status indication
	// 00 : init
	// 01 : processing
	// 10 : lock
	// 11 : fail

	Common_RdFL(common_mon45);	//regAddr = 0x00ad, COMMON_MON[45]
	Common_Rd01(common_mon45,tpslockeq_d,tpslockeq_d); //TPSLOCKEQ_D: TPSLOCKEQ_D status indication for DVB-T(0 : unlock, 1 : lock)

	DEMOD_PRINT("[DEMOD H13](%s) neverlock_1_0 = %d !!!\n", __F__,neverlock_1_0);
	DEMOD_PRINT("[DEMOD H13](%s) srdstatus_1_0 = %d !!!\n", __F__,srdstatus_1_0);
	DEMOD_PRINT("[DEMOD H13](%s) tpslockeq_d = %d !!!\n", __F__,tpslockeq_d);

	if(scanMode == LX_DEMOD_QAM)
	{
		if((neverlock_1_0 == 3) || (srdstatus_1_0 == 3))
		{
			DEMOD_PRINT("[DEMOD H13](%s)neverlock_1_0[%d] srdstatus_1_0[%d]!!!\n", __F__,neverlock_1_0,srdstatus_1_0);
			neverlock_1_0 = 3;
		}
	}
	else if(scanMode == LX_DEMOD_ISDBT)
	{
		if((neverlock_1_0 == 2) && (tpslockeq_d != 1))
		{
			DEMOD_PRINT("[DEMOD H13](%s)neverlock_1_0[%d] tpslockeq_d[%d]!!!\n", __F__,neverlock_1_0,tpslockeq_d);
			neverlock_1_0 = 1;
		}

	}

	switch(neverlock_1_0)
	{
		case 0x00 : // Initialization process

			*pNeverLockStatus = LX_DEMOD_IDLE;
			break;

		case 0x01 : // Signal checking process

			*pNeverLockStatus = LX_DEMOD_DETECTION_PROCESSING;
			break;

		case 0x02 :// Normal state (available channel)

			*pNeverLockStatus = LX_DEMOD_DETECTED;
			break;

		case 0x03 :// neverlock state (empty channel)

			*pNeverLockStatus = LX_DEMOD_DETECTION_FAIL;
			break;

	}

	Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	Common_Rd01(common_mon0,syslock,syslock);
	//SYSLOCK : System operation status indication
	//System lock is come from ifolock for DVB-T, ISDB-T, synclock for VSB, and framelock for DVB-C, QAM, ISDB-C
	// 0 : unlock
	// 1 : lock

	DEMOD_PRINT("[DEMOD H13](%s) SYSLOCK = %s !!!\n", __F__,(syslock == 1) ? "LOCK" :"NOT LOCK");

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_CarrierFreqOffset
 * Get carrier offset frequency .
 *
 * @param *pFreqOff 	[OUT] SINT32.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_CarrierFreqOffset(SINT32 *pFreqOff)
{


		UINT8	carrfreqoffset_7_0;
		UINT8 	carrfreqoffset_15_8;

		SINT32 tempCarrierOffset = 0;
		SINT32 carrierOffset;

		*pFreqOff = 0;

		Common_RdFL(common_mon28);  //regAddr = 0x009c, COMMON_MON[28]
		Common_Rd01(common_mon28,carrfreqoffset_15_8,carrfreqoffset_15_8); //CARRFREQOFFSET[15:8]
		tempCarrierOffset = (carrfreqoffset_15_8 << 8 ) ;

		Common_RdFL(common_mon29);  //regAddr = 0x009d, COMMON_MON[29]
		Common_Rd01(common_mon29,carrfreqoffset_7_0,carrfreqoffset_7_0); //CARRFREQOFFSET[7:0]
		tempCarrierOffset += (carrfreqoffset_7_0 << 0 ) ;

		if ( tempCarrierOffset & 0x8000 ) tempCarrierOffset = ((0xffff0000) | tempCarrierOffset) ;



		carrierOffset =  tempCarrierOffset * 61 ;  	// Carrier frequency offset [MHz] = CARRIERFREQOFFSET / (2^14), 2^14 = 16384
													// 1000000[Hz] / 16384 = 61
		carrierOffset = carrierOffset / 1000 ;     	// [Hz] / 1000 -> [KHz]

		*pFreqOff = carrierOffset;

		DEMOD_PRINT("[DEMOD H13] Carrier Offset freq = %d [KHz] !!!\n",*pFreqOff);

		return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_TPIFStatus
 * Get the status of TP IF function (active or inactive)
 *
 * @param pbEnable	  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_TPIFStatus(BOOLEAN *pbEnable)
{
	UINT8 	tpifen;

	DEMOD_PRINT("[DEMOD H13] == @ %s!!!\n",__F__);

	Common_RdFL(common_ctrl112); //regAddr = 0x0070, COMMON_CTRL[112]
	Common_Rd01(common_ctrl112,tpifen, tpifen );

	//TPIFEN : PCR jitter control for TP signal output
	// 0 : disable(un controlled data output. It should have large PCR jitter)
	// 1 : enable(controlleded TP data generation)

	if(tpifen == 1)
	{
		*pbEnable = TRUE;
	}
	else
	{
		*pbEnable = FALSE;
	}
	DEMOD_PRINT("[DEMOD H13] TP IF function is %s!!\n",(*pbEnable == TRUE) ? "Active" : "Inactive");

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_VABER
 * Get BER before RS on DVB
 *
 * @param *pVber 	[OUT] UINT32.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_VABER(UINT32 *pVber)
{


	UINT8 	chberint;
	UINT8 	vaber_7_0;
	UINT8 	vaber_15_8;

	UINT16 tempBer = 0;

	*pVber = 0;

	chberint = 0x00;

	Common_RdFL(common_ctrl115);   //regAddr = 0x0073, COMMON_CTRL[115]
	Common_Wr01(common_ctrl115, chberint, chberint); //CHBERINT : '0'-> every 10^7 data, '1'-> every 10^5 data.
	Common_WrFL(common_ctrl115);

	OS_MsecSleep(10);

	Common_RdFL(common_mon120);  //regAddr = 0x00f8, COMMON_MON[120]
	Common_Rd01(common_mon120,vaber_15_8,vaber_15_8); //VABER[15:8]
	tempBer += (vaber_15_8 << 8);

	Common_RdFL(common_mon121);  //regAddr = 0x00f9, COMMON_MON[121]
	Common_Rd01(common_mon121,vaber_7_0,vaber_7_0); //VBBER[7:0]
	tempBer += (vaber_7_0 << 0);

	*pVber =  (UINT32)tempBer ;


	return RET_OK;


}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_Packet_Error
 * Get packet error on DVB
 *
 * @param *pSnr 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_Packet_Error(UINT32 *pError)
{

	UINT8 	permode_1_0;
	UINT8 	tperrcnt_7_0;

#if 0
	UINT8	tpiftperrcnt_7_0;

	Common_RdFL(common_mon116);  //regAddr = 0x00f4, COMMON_MON[116]
	Common_Rd01(common_mon116,tpiftperrcnt_7_0,tpiftperrcnt_7_0); //tpiftperrcnt_7_0[7:0]

	DEMOD_TRACE(" tpiftperrcnt_7_0 = %d\n",tpiftperrcnt_7_0);
#endif


    permode_1_0 = 0x3;  //accumulate at every I2C read of TPERRCNT(@0xFA)

	Common_RdFL(common_ctrl115);	//regAddr = 0x0073, COMMON_CTRL[115]
	Common_Wr01(common_ctrl115, permode_1_0, permode_1_0);
	Common_WrFL(common_ctrl115);
	//PERMODE[1:0] : Selection of TP error counter calculation mode for TPERRCNT(@0xFA)	for VSB/QAM
	//Always 1 second accumulated error counter calculation	for DVB-T/DVB-C
	// 00 : accumulate at every 1 second
	// 01 : percentage (%) of packet error rate (reserved)
	// 10 : permillage (¢¶) of packet error rate (reserved)
	// 11 : accumulate at every I2C read of TPERRCNT(@0xFA)


	*pError = 0xffffffff;

	Common_RdFL(common_mon122);  //regAddr = 0x00fa, COMMON_MON[122]
	Common_Rd01(common_mon122,tperrcnt_7_0,tperrcnt_7_0); //TPERRCNT[7:0]: Monitoring for value of TP error counter

	*pError =  (UINT32)tperrcnt_7_0 ;
	DEMOD_PRINT(" tperrcnt = %d, *pError = %d  \n",tperrcnt_7_0,*pError);

	//printk(" ###################chungiii  tperrcnt = %d, *pError = %d  \n",tperrcnt_7_0,*pError);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_BandWidthMode
 * Get the information of bandwidth .
 *
 * @param *ChannelBW 	[OUT] LX_DEMOD_BWMODE_T - 6, 7 and 8 MHZ.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_BandWidthMode( LX_DEMOD_BWMODE_T *ChannelBW )
{


	UINT8 	detrfbw_1_0;

	*ChannelBW = LX_DEMOD_BW_UNKNOWN;

	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);

	Common_RdFL(common_mon5);  //regAddr = 0x0085, COMMON_MON[5]
	Common_Rd01(common_mon5,detrfbw_1_0,detrfbw_1_0);
	//DETRFBW[1:0] : Detected RF bandwidth indication
	// 00 : 8MHz
	// 01 : 7MHz
	// 10 : 6MHz
	// 11 : reserved

	switch(detrfbw_1_0)
	{
		case 0x00 :
			*ChannelBW = LX_DEMOD_BW_8MHZ;
			break;
		case 0x01:
			*ChannelBW = LX_DEMOD_BW_7MHZ;
			break;
		case 0x02:
			*ChannelBW = LX_DEMOD_BW_6MHZ;
			break;
		default :
			*ChannelBW = LX_DEMOD_BW_UNKNOWN;
			break;
	}

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_QAMMode
 * Get the information of QAM mode .
 *
 * @param *pQamMode [OUT] LX_DEMOD_RF_MODE_T - 16QAM, 32QAM, 64QAM, 128QAM, 256QAM, QPSK.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_QAMMode( LX_DEMOD_RF_MODE_T *pQamMode )
{

	UINT8 qammode_2_0;
	UINT8 detstdopmode_7_0;

	LX_DEMOD_OPER_MODE_T mode;

	*pQamMode = LX_DEMOD_UNKOWN_MODE;

	Common_RdFL(common_mon1); //regAddr = 0x0081, COMMON_MON[1]
	Common_Rd01(common_mon1,detstdopmode_7_0, detstdopmode_7_0 );
	//DETSTDOPMODE[7:0]: Standard mode monitoring
	// 00000010 : ISDB-C
	// 00000100 : DVB-C
	// 00001000 : QAM
	// 00010000 : reserved
	// 00100000 : ISDB-T
	// 01000000 : DVB-T
	// 10000000 : VSB

	if(detstdopmode_7_0 & 0x40) mode = LX_DEMOD_DVBT;
	else if(detstdopmode_7_0 & 0x20) mode = LX_DEMOD_ISDBT;
	else if(detstdopmode_7_0 & 0x8) mode = LX_DEMOD_QAM;
	else if(detstdopmode_7_0 & 0x4) mode = LX_DEMOD_DVBC;
 	else mode = LX_DEMOD_UNKOWN_OPMODE;

	Common_RdFL(common_mon38);  //regAddr = 0x00a6, COMMON_MON[38]
	Common_Rd01(common_mon38,qammode_2_0,qammode_2_0);
	//QAMMODE[2:0]: QAM mode indecation
	//for VSB : not valid, always 8VSB
	//for DVB-T
		// 000 : QPSK
		// 001 : 16QAM
		// 010 : 64QAM
		// 011~111 : reserved
	//for ISDB-T : Layer A for single layer, Layer B for others
		// 000 : DQPSK
		// 001 : QPSK
		// 010 : 16QAM
		// 011 : 64QAM
		// 100~110 : reserved
		// 111 : unused hierarchical layer
	//for QAM
		// 0xx : 64QAM
		// 1xx : 256QAM
	//for DVB-C
		// 000 :  16QAM
		// 001 :  32QAM
		// 010 :  64QAM
		// 011 : 128QAM
		// 100 : 256QAM
		// 101~110 : reserved
	//for ISDB-C
		// 0xx : 64QAM
		// 1xx : 256QAM

	switch(mode)
	{
		case LX_DEMOD_DVBT:
			{
				switch(qammode_2_0)
				{
					case 0x00 :
						*pQamMode = LX_DEMOD_QPSK;
						break;
					case 0x01:
						*pQamMode = LX_DEMOD_16QAM;
						break;
					case 0x02 :
						*pQamMode = LX_DEMOD_64QAM;
						break;
					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}
			}
			break;

		case LX_DEMOD_DVBC:
			{
				switch(qammode_2_0)
				{
					case 0x00 :
						*pQamMode = LX_DEMOD_16QAM;
						break;
					case 0x01 :
						*pQamMode = LX_DEMOD_32QAM;
						break;
					case 0x02 :
						 *pQamMode = LX_DEMOD_64QAM;
						break;
					case 0x03 :
						 *pQamMode = LX_DEMOD_128QAM;
						break;
					case 0x04 :
						*pQamMode = LX_DEMOD_256QAM;
						break;
					default :
						 *pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;

				}
			}
			break;

		case LX_DEMOD_QAM:
			{
				switch(qammode_2_0  >> 2)
				{
					case 0x00 :
						*pQamMode = LX_DEMOD_64QAM;
						break;
					case 0x01:
						*pQamMode = LX_DEMOD_256QAM;
						break;
					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}
			}
			break;


		case LX_DEMOD_ISDBT:
			{
				switch(qammode_2_0)
				{
					case 0x00 :
						*pQamMode = LX_DEMOD_DQPSK;
						break;
					case 0x01:
						*pQamMode = LX_DEMOD_QPSK;
						break;
					case 0x02 :
						*pQamMode = LX_DEMOD_16QAM;
						break;
					case 0x03:
						*pQamMode = LX_DEMOD_64QAM;
						break;
					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}
			}
			break;


		default :
			DEMOD_PRINT("[DEMOD H13] FAIL,(%s:%d) UNKNOWN RF MODE !!!\n", __F__, __L__);
			return RET_ERROR;

	}
	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_MseInfo
 * Get signal MSE info.
 *
 * @param *pSigMse [OUT] LX_DEMOD_MSE_T.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_MseInfo( LX_DEMOD_MSE_T *pSigMse)
{

	UINT8	mse_7_0;
	UINT8 	mse_15_8;

	UINT8	bmse_7_0;
	UINT8 	bmse_15_8;

	UINT8	constpwr_15_8;
	UINT8	constpwr_7_0;

	UINT16 	tempMseValue = 0;
	UINT16 	tempBmseValue = 0;
	UINT16	constpwr = 0;


	if(NULL == pSigMse) return RET_ERROR;

 	//MSE, SNR = 10log(CONSTPWR/MSE)
	Common_RdFL(common_mon108);  //regAddr = 0x00ec, COMMON_MON[108]
	Common_Rd01(common_mon108,mse_15_8,mse_15_8); //MSE[15:8]:Monitoring for Mean Square Error(MSE) value
	tempMseValue = (mse_15_8 << 8);

	Common_RdFL(common_mon109);  //regAddr = 0x00ed, COMMON_MON[109]
	Common_Rd01(common_mon109,mse_7_0,mse_7_0); //MSE[7:0]: Monitoring for Mean Square Error(MSE) value
	tempMseValue += (mse_7_0 << 0);

	//BMSE, SNR = 10log(CONSTPWR/BMSE)
	Common_RdFL(common_mon106);  //regAddr = 0xea, EQCOMMON_MON[4]
	Common_Rd01(common_mon106,bmse_15_8,bmse_15_8); //MSE[15:8]:Monitoring for Mean Square Error(MSE) value
	tempBmseValue = (bmse_15_8 << 8);

	Common_RdFL(common_mon107);  //regAddr = 0xeb, EQCOMMON_MON[5]   //For 256QAM
	Common_Rd01(common_mon107,bmse_7_0,bmse_7_0); //MSE[7:0]
	tempBmseValue += (bmse_7_0 << 0);


	//constPwr
	Common_RdFL(common_mon104);  //regAddr = 0x00e8, COMMON_MON[104]
	Common_Rd01(common_mon104,constpwr_15_8,constpwr_15_8); //CONSTPWR[15:8]:Monitoring for constellation power value of internally used signal for signal to noise ratio calculation
	constpwr = (constpwr_15_8 << 8);

	Common_RdFL(common_mon105);  //regAddr = 0x00e9, COMMON_MON[105]
	Common_Rd01(common_mon105,constpwr_7_0,constpwr_7_0); //CONSTPWR[7:0]:Monitoring for constellation power value of internally used signal for signal to noise ratio calculation
	constpwr += (constpwr_7_0 << 0);

	pSigMse->mse = tempMseValue;
	pSigMse->bMse = tempBmseValue;
	pSigMse->constPwr = constpwr;

	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H13_Get_Lock
 * Check the DVB lock state
 *
 * @param eStatus 	[IN] LX_DEMOD_LOCK_STATUS_T - which lock flags are selected.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H13_Get_Lock(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState)
{

	LX_DEMOD_OPER_MODE_T operMode;
	UINT8 tempLockStatus = 0;

	UINT8 syslock;
	UINT8 synclock;
	UINT8 samlock_cqs;
	UINT8 txpswlock_di;
	UINT8 cirlock;
	UINT8 agclock;
	UINT8 modelock_di;
	UINT8 tpslockeq_d;
	UINT8 gsdone;
	UINT8 crfedstatus_di;

	UINT8 tmccflock_i;

//	DEMOD_PRINT("[DEMOD H13 DTV] @ %s =====\n",__F__);


	if (RET_OK != DEMOD_H13_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H13] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		*lockState = LX_DEMOD_UNKOWN_STATE;
		return RET_ERROR;
	}


    switch( whatLock )
    {

	    case LX_DEMOD_FEC_LOCK:
			{
//				if(detstdopmode_7_0 & 0x08) //QAM
				if(operMode == LX_DEMOD_QAM) //QAM
				{
					Common_RdFL(common_mon0); //regAddr = 0x0080[4], syslock check
					Common_Rd01(common_mon0, syslock, syslock);  //VDLOCK, FRAMELCK, MPEGLOCK : '0' -> unlocked, '1' -> locked

					tempLockStatus = syslock;

				}
				else if(operMode == LX_DEMOD_DVBC) //DVB-C
				{
					Common_RdFL(common_mon38);  //regAddr = 0x00a6, COMMON_MON[38]
					Common_Rd01(common_mon38,synclock,synclock); //SYNCLOCK : Synchronization status indication (0 : unlock, 1 : lock ) , cable modes(QAM,DVB_C,ISDB-C) use framelock

					tempLockStatus = synclock;
				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					return RET_ERROR;
				}

	    	}
			break;

		case LX_DEMOD_TR_LOCK:
			{

				if((operMode == LX_DEMOD_DVBC) ||(operMode == LX_DEMOD_QAM) ) //DVB-C, QAM
				{
					// Sampling LOCK status  and offset monitoring
					Sync_RdFL(sync_mon20);  //regAddr = 0x1094, SAMPLING_MON[0]
					Sync_Rd01(sync_mon20,samlock_cqs,samlock_cqs); //SAMLOCK_CQS:Timing recovery lock indication (0->unlock, 1-> lock)
					if(samlock_cqs == 1) tempLockStatus = 1;
					else tempLockStatus = 0;
				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					return RET_ERROR;
				}

			}
			break;

		case LX_DEMOD_CARR_LOCK:
			{

				if(operMode == LX_DEMOD_DVBC)//DVB-C
				{

					EQ_CQS_RdFL(eq_cqs_mon9);  //regAddr = 0x2889, GS_BURST_MON[0]
					EQ_CQS_Rd01(eq_cqs_mon9,gsdone,gsdone);
					//GSDONE: Indicator that shows the status of carrier recovery.
					// 0 : both of short and long loop for AFC are not locked
					// 1 : both of short and long loop for AFC are locked

					if(gsdone == 1) tempLockStatus = 1;
					else tempLockStatus = 0;
				}
				else if(operMode == LX_DEMOD_ATSC)//VSB
				{

					Sync_RdFL(sync_mon35); //regAddr = 0x10a3, Carrier monitoring
					Sync_Rd01(sync_mon35,crfedstatus_di,crfedstatus_di);	//crfedstatus_di : Indicate carrier recovery lock status of VSB, '0' -> Unlocked, '1' -> Locked.

					if(crfedstatus_di == 1) tempLockStatus = 1;
					else tempLockStatus = 0;

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					return RET_ERROR;
				}
			}
			break;



		case LX_DEMOD_PSYNC_LOCK:
			{

				if(operMode == LX_DEMOD_DVBT)//DVB-T
				{
//					DEMOD_H13_DVBT_Get_IFO_LOCK(&ifoLock);

					Common_RdFL(common_mon70);  //regAddr = 0x00c6, COMMON_MON[70]
					Common_Rd01(common_mon70,cirlock,cirlock); //CIRLOCK: CIRCLOCK status indication for DVB-T/ISDB-T use CIRLOCK (0 : unlock, 1 : lock)

					Common_RdFL(common_mon45);  //regAddr = 0x00ad, COMMON_MON[45]
					Common_Rd01(common_mon45,tpslockeq_d,tpslockeq_d); //TPSLOCKEQ_D: TPSLOCKEQ_D status indication for DVB-T(0 : unlock, 1 : lock)
					if((cirlock == 1) && (tpslockeq_d == 1))	tempLockStatus = 1;
					else 	tempLockStatus = 0;

//					DEMOD_PRINT(" cirlock = %d, tpslockeq_d = %d, ifoLock = %s  \n",cirlock,tpslockeq_d,(ifoLock == TRUE) ? "LOCKED" : "NOT LOCKED");

				}
				else if((operMode == LX_DEMOD_ATSC)) // || (operMode == LX_DEMOD_ISDBT)) //ATSC, ISDBT
				{
					Common_RdFL(common_mon38);	//regAddr = 0x00a6[7]
					Common_Rd01(common_mon38,synclock,synclock); //SYNCLOCK, '1' -> LOCK

					tempLockStatus = synclock;
				}
#if 1
				else if(operMode == LX_DEMOD_ISDBT) // ISDBT
				{
					Common_RdFL(common_mon53);	//regAddr = 0x00b5[7]
					Common_Rd01(common_mon53,tmccflock_i,tmccflock_i); //TMCCFLOCK_I
					// TMCCFLOCK_I : Framelock for ISDB-T status indication which is detected in the TMCC decoder(0 : unlock, 1 : lock)

					tempLockStatus = tmccflock_i;
				}
#endif
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					return RET_ERROR;
				}

			}
			break;

		case LX_DEMOD_TPS_LOCK:
			{
				if(operMode == LX_DEMOD_DVBT)//DVB-T
				{
					Common_RdFL(common_mon43);  //regAddr = 0x00ab, COMMON_MON[43]
					Common_Rd01(common_mon43,txpswlock_di,txpswlock_di); //TXPSWLOCK_DI : TPS/TMCC syncword lock status indication(0 : unlock, 1 : lock)
					if(txpswlock_di == 1) tempLockStatus = 1;
					else tempLockStatus = 0;
				}
			}
			break;

		case LX_DEMOD_AGC_LOCK:
				Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
				Common_Rd01(common_mon0,agclock,agclock); //AGCLOCK : AGC status indication(0 : unlock, 1 : lock)
				if(agclock == 1) tempLockStatus = 1;
				else tempLockStatus = 0;

			break;

		case LX_DEMOD_MODE_DET:
			{
				if((operMode == LX_DEMOD_DVBT) || (operMode == LX_DEMOD_ISDBT)) //DVB-T, ISDB-T
				{
					Common_RdFL(common_mon42);  //regAddr = 0x00aa, COMMON_MON[42]
					Common_Rd01(common_mon42,modelock_di,modelock_di); //MODELOCK_DI : Transmission mode lock status indication for DVB-T/ISDB-T(0 : unlock, 1 : lock)
					if(modelock_di == 1) tempLockStatus = 1;
					else tempLockStatus = 0;

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					return RET_ERROR;
				}

			}
			break;


		default:
			*lockState = LX_DEMOD_UNKOWN_STATE;
			return RET_ERROR;
	}

	if (tempLockStatus == 1 )
	{
		 *lockState = LX_DEMOD_LOCKED;
	}
	else
	{
		*lockState = LX_DEMOD_UNLOCKED;
	}
	return RET_OK;

}



