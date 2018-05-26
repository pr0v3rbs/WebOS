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

#include <asm/div64.h> //do_div

#include "sys_regs.h"

#include "demod_impl.h"
#include "demod_common_h14eu.h"
#include "demod_reg_h14eu.h"

#if 0
#endif


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_ResetHW.
 * Reset GBB demod ID.
 *
 * @param none  [IN/OUT]
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/


int DEMOD_H14EUBB_ResetHW(void)
{
	
	ACE_REG_H14A0_RdFL(soft_reset_0);
	
	if(ACE_REG_H14A0_RdFd(soft_reset_0,swrst_f24m) == 1)
	{
		ACE_REG_H14A0_RdFL(soft_reset_0);
		ACE_REG_H14A0_Wr01(soft_reset_0,swrst_f24m,0);
		ACE_REG_H14A0_WrFL(soft_reset_0);
		DEMOD_PRINT_ALWAYS("[KDRV_DEMOD] NEED TO BE RESET swrst_f24m \n");
		
	}
	else
	{
		DEMOD_PRINT_ALWAYS("[KDRV_DEMOD] ALREADY RESET swrst_f24m \n");
	}




	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ResetHW start !! \n");

	ACE_REG_H14A0_RdFL(gbb_0);

	ACE_REG_H14A0_Wr01(gbb_0, reset, 1);

	ACE_REG_H14A0_WrFL(gbb_0);

	OS_MsecSleep(10);


	ACE_REG_H14A0_Wr01(gbb_0, reset, 0);

	ACE_REG_H14A0_WrFL(gbb_0);


	 OS_MsecSleep(20);


	 ACE_REG_H14A0_Wr01(gbb_0, reset, 1);

	 ACE_REG_H14A0_WrFL(gbb_0);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_ResetHW end !! \n");


	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_SetI2C.
 * select normal mode
 *
 * @param none  [IN/OUT]
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_SetI2C(void)
{
	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetI2C Start !! \n");

	ACE_REG_H14A0_RdFL(gbb_0);

	ACE_REG_H14A0_Wr01(gbb_0, i2csel, 0);

	ACE_REG_H14A0_WrFL(gbb_0);

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetI2C end !! \n");
	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_SetInclk.
 * select inclk mode
 *
 * @param inclk  [IN] LX_DEMOD_INCLK_SEL_T
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_SetInclk(LX_DEMOD_INCLK_SEL_T *inclk)
{

	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk Start !! \n");

	ACE_REG_H14A0_RdFL(gbb_afe_0);

	switch(*inclk)
	{
		case LX_DEMOD_ARM_SETTING:
		{
			ACE_REG_H14A0_Wr01(gbb_afe_0, inck_sel, 0);
		}
		break;

		case LX_DEMOD_EXTERN_PIN:
		{

			ACE_REG_H14A0_Wr01(gbb_afe_0, inck_sel, 1);
		}
		break;

		default:
			DEMOD_PRINT("Invalid LX_DEMOD_INCLK_SEL_T type\n");
			return -1;
	}

	ACE_REG_H14A0_WrFL(gbb_afe_0);



	DEMOD_PRINT("[KDRV_DEMOD] DEMOD_SetInclk End !! \n");

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_RepeaterEnable.
 * Enable / Disable GBB I2C repeater function
 *
 * @param bEnable [IN] 	BOOLEAN - Enable/Disable I2C repeater
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_RepeaterEnable(BOOLEAN bEnable)
{
	UINT8 i2crptenb;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	i2crptenb = (bEnable == TRUE)?(0):(1); //I2CRPTENB : Tuner I2C port control , 0 -> enable, 1-> disable

	DVB_Common_RdFL(common_ctrl2);   //regAddr = 0x0002, COMMON_CTRL[2]
	DVB_Common_Wr01(common_ctrl2, i2crptenb, i2crptenb);
	DVB_Common_WrFL(common_ctrl2);
	//I2CRPTENB (bit1) : Select I2C Repeater Enable.(0 : enable, 1 : disable)

	DEMOD_PRINT("^r^[DEMOD H14] success!!  DVB_RepeaterEnable =  %s ", (bEnable == TRUE)?"ON":"OFF");
	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_SoftwareReset.
 * Reset DVB module by software.
 *
 * @param  NONE
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_SoftwareReset(void)
{

	UINT8 softrstb;

	softrstb = 0; //SOFTRSTB -> 0 : reset

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);


	DVB_Common_RdFL(common_ctrl0);  //regAddr = 0x0000, COMMON_CTRL[0]
	DVB_Common_Wr01(common_ctrl0, softrstb, softrstb); 	//SOFTRSTB (bit7) : Function reset control(0 : reset,  1 : normal operation)
	DVB_Common_WrFL(common_ctrl0);

	OS_MsecSleep(20);

	softrstb = 1; //SOFTRSTB -> 1 : normal

	DVB_Common_Wr01(common_ctrl0, softrstb, softrstb);
	DVB_Common_WrFL(common_ctrl0);


	return RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Serial_Control
 * Control TS output mode(serial/Parallel)
 *
 * @param bEnable 	[IN] Serial(TRUE) / Parallel(FALSE)
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Serial_Control(BOOLEAN bEnable)
{
	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);
	// not supported, only parellel output

	return	RET_OK;
}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Power_Save
 * Enable/Disable power save mode
 *
 * @param bEnable 	[IN] power save mode(TRUE) / normal mode(FALSE)
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/

int DEMOD_H14EUBB_Power_Save(BOOLEAN bEnable)
{
	UINT8 pll_pdb;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	pll_pdb = (bEnable == TRUE)?(0):(1);  //PLL_PDB : PLL Core Power down ( 0 : Power-down,  1 : Normal operation)

	DVB_Common_RdFL(common_ctrl120);  //regAddr = 0x0078, COMMON_CTRL[120]
	DVB_Common_Wr01(common_ctrl120, pll_pdb, pll_pdb); //PLL_PDB : PLL Core Power down ( 0 : Power-down,  1 : Normal operation)
	DVB_Common_WrFL(common_ctrl120);

	DEMOD_PRINT("[DEMOD H14] success!!  DEMOD_H14EUBB_DVB_Power_Save =  %s\n ", (bEnable == TRUE)? "ON":"OFF");

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_StdOperModeContrl
 * Set standard operation mode, Bandwidth and symbol rate
 *
 * @param *InitParam [IN] LX_DEMOD_INIT_PARAM_T - operation RF Mode, bandWidth, Symbol rate
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_StdOperModeContrl( LX_DEMOD_OPER_MODE_T operMode, LX_DEMOD_BWMODE_T bandWd, LX_DEMOD_SYMRDET_MODE_T symbr)
{

	UINT8 	opm_1_0;
	UINT8 	cable_en;


	UINT8 	trcenterf_31_24;
	UINT8 	trcenterf_23_16;
	UINT8 	trcenterf_15_8;
	UINT8 	trcenterf_7_0;

	UINT8 	detrfbw_2_0;

	UINT8   srd_en_c;

	DEMOD_PRINT(" == [DEMOD H14 DTV] @ %s  ==(oper = 0x%x)\n",__F__,operMode);

	switch(operMode)
	{
		case LX_DEMOD_DVBT2:
			opm_1_0 = 0x0;
			cable_en = 0;

			break;


		case LX_DEMOD_DVBT:
			opm_1_0 = 0x1;
			cable_en = 0;


			break;

		case LX_DEMOD_DVBC2:
			opm_1_0 = 0x2;
			cable_en = 1;


			break;

		case LX_DEMOD_DVBC:
			opm_1_0 = 0x3;
			cable_en = 1;

			break;

		default :
			DEMOD_PRINT("[DEMOD H14](%s,%d) Invaild Opermode parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}


	switch(bandWd)
	{

		case LX_DEMOD_BW_1_7MHZ:
			// 0x247157f0
			trcenterf_31_24	= 0x24;
			trcenterf_23_16	= 0x71;
			trcenterf_15_8	= 0x57;
			trcenterf_7_0	= 0xf0;

			detrfbw_2_0 	= 0x4;
			break;

		case LX_DEMOD_BW_5MHZ:
			// 0x06599999
			trcenterf_31_24	= 0x06;
			trcenterf_23_16	= 0x59;
			trcenterf_15_8	= 0x99;
			trcenterf_7_0	= 0x99;

			detrfbw_2_0 	= 0x3;
			break;

		case LX_DEMOD_BW_6MHZ:
			// 0x03f55555
			trcenterf_31_24	= 0x03;
			trcenterf_23_16	= 0xf5;
			trcenterf_15_8	= 0x55;
			trcenterf_7_0	= 0x55;

			detrfbw_2_0 	= 0x2;
			break;

		case LX_DEMOD_BW_7MHZ:
			//0x02400000
			trcenterf_31_24	= 0x02;
			trcenterf_23_16	= 0x40;
			trcenterf_15_8	= 0x00;
			trcenterf_7_0	= 0x00;

			detrfbw_2_0 	= 0x1;
			break;

		case LX_DEMOD_BW_8MHZ:
			// 0x00f80000
			trcenterf_31_24	= 0x00;
			trcenterf_23_16	= 0xf8;
			trcenterf_15_8	= 0x00;
			trcenterf_7_0	= 0x00;

			detrfbw_2_0 	= 0x0;
			break;

		default :
			DEMOD_PRINT("[DEMOD H14](%s,%d) Invalid Bandwidth parameter !!!\n",__F__, __L__);
			return RET_ERROR;
	}


#if 1
	switch(symbr)
	{
		case LX_DEMOD_SYMR_FULL_SCAN_MODE:
		case LX_DEMOD_SYMR_FIXED_MODE:
		case LX_DEMOD_SYMR_QAM_NO_OPERATION_MODE:
		case LX_DEMOD_SYMR_DVBC_NO_OPERATION_MODE:
			{
				srd_en_c = 1;

			}
			break;


		default :
			DEMOD_PRINT("[DEMOD H14](%s,%d) Invalid Symbol rate parameter !!!\n",__F__, __L__);
			break;
//			return RET_ERROR;
	}

#endif

	DVB_Common_RdFL(common_ctrl1); //regAddr = 0x0001, COMMON_CTRL[1]
	DVB_Common_Wr02(common_ctrl1,opm_1_0, opm_1_0, cable_en, cable_en );  //OPM[1:0], CABLE_EN
	DVB_Common_WrFL(common_ctrl1);
	// OPM[1:0] Set operation mode manually.
	// 00 : DVB-T2 mode
	// 01 : DVB-T mode
	// 10 : DVB-C2 mode
	// 11 : DVB-C mode

	// CABLE_EN : Selection of terrestrial or cable
	// 0 : Terrestrial
	// 1 : Cable

	DVB_Common_RdFL(common_ctrl28);   //regAddr = 0x001c : COMMON_CTRL[28]
	DVB_Common_Wr01(common_ctrl28,trcenterf_31_24, trcenterf_31_24 );
	DVB_Common_WrFL(common_ctrl28);
	//TRCENTERF_8M[31:24]  : Select A/D sampling frequency.
	// TRCENTERF for 8MHz BW = [{40.96M/(4x64/7M)}-1]x2^27 = 16106127 = 0x00F5C28F  --> in case of sampling frequency ( 41M )
	// TRCENTERF for 8MHz BW = [{48M/(4x64/7M)}-1]x2^27 = 41943040 = 0x2800000  --> in case of sampling frequency ( 48M )

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

	// TRCENTERF for 8MHz BW = [{48M/(4x64/7M)}-1]x2^27 = 41943040 = 0x2800000  --> in case of sampling frequency ( 48M )
	// TRCENTERF for 7MHz BW = [{48M/(4x8M)}-1]x2^27 = 67108864 = 0x4000000  --> in case of sampling frequency ( 48M )
	// TRCENTERF for 6MHz BW = [{48M/(4x48/7M)}-1]x2^27 = 100663296 = 0x6000000  --> in case of sampling frequency ( 48M )
	// TRCENTERF for 5MHz BW = [{48M/(4x40/7M)}-1]x2^27 = 147639501 = 0x8CCCCCD  --> in case of sampling frequency ( 48M )
	// TRCENTERF for 1.7MHz BW = [{48M/(4x131/71M)}-5]x2^27 = 201838873 = 0xC07D119  --> in case of sampling frequency ( 48M )

	// TRCENTERF for 8MHz BW = [{41M/(4x64/7M)}-1]x2^27 	= 0x00f80000  --> in case of sampling frequency ( 41M )
	// TRCENTERF for 7MHz BW = [{41M/(4x8M)}-1]x2^27 		= 0x02400000  --> in case of sampling frequency ( 41M )
	// TRCENTERF for 6MHz BW = [{41M/(4x48/7M)}-1]x2^27 	= 0x03f55555  --> in case of sampling frequency ( 41M )
	// TRCENTERF for 5MHz BW = [{41M/(4x40/7M)}-1]x2^27 	= 0x06599999  --> in case of sampling frequency ( 41M )
	// TRCENTERF for 1.7MHz BW = [{41M/(4x131/71M)}-5]x2^27  = 0x247157f0  --> in case of sampling frequency ( 41M )

	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl0);  //regAddr = 0x2400, T2TC2_Eq_TI_CTRL[0]
	T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl0,detrfbw_2_0,detrfbw_2_0);
	//DETRFBW[2:0] : Detected RF bandwidth indication
	// 000 : 8MHz
	// 001 : 7MHz
	// 010 : 6MHz
	// 011 : 5MHz
	// 100 : 1.7MHz
	// 101 ~ 111 : reserved

	C_Sync_RdFL(c_sync_ctrl0);	//regAddr = 0x1800, C_SYNC_CTRL[0]
	C_Sync_Wr01(c_sync_ctrl0,srd_en_c,srd_en_c);
	C_Sync_WrFL(c_sync_ctrl0);
	//SRD_EN_C : SRD enable (0 : SRD off, 1 : SRD on)


	return RET_OK;

}


/**
 * DEMOD_H14EUBB_NeverlockScan
 * Get the neverlock flag indicator.
 *
 * @param	*pDemodInfo			[IN] DEMOD_CTX_T.
 * @param	ResetOn				[IN] 	LX_DEMOD_FLAG_T  - force to initialize the postjob.
 * @param	*pScanProcessing		[OUT] LX_DEMOD_FLAG_T- the flag that scan is done or not done.
 * @param	*pScanLock			[OUT] LX_DEMOD_LOCK_STATE_T - scan lock result.
 * @return if Succeeded - RET_OK else-TU_ERROR
 * @author 	Jeongpil.yun(jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_NeverlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock)
{


	static 	LX_DEMOD_LOCK_STATE_T	_demodScanLock	= LX_DEMOD_UNKOWN_STATE;

	unsigned int elapsedTime = 0;
	static	unsigned int	_neverLockResetTime	= 0;

	static unsigned int _maxTimeOut = 0;


	LX_DEMOD_LOCK_STATE_T neverLockStatus;
	LX_DEMOD_OPER_MODE_T scanMode;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	scanMode = pDemodInfo->operMode;

	if(LX_DEMOD_FLAG_ON == ResetOn)
	{

		*pScanProcessing = LX_DEMOD_FLAG_ING;
		*pScanLock	= LX_DEMOD_UNLOCKED;

		switch(scanMode)
		{
			case LX_DEMOD_DVBT :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_2500;
				break;

			case LX_DEMOD_DVBT2 :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_2500;
				break;

			case LX_DEMOD_DVBC :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_1500;
				break;

			case LX_DEMOD_DVBC2 :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_1500;
				break;

			case LX_DEMOD_ATSC :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

			case LX_DEMOD_QAM :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

			case LX_DEMOD_ISDBT :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

			default :
				_maxTimeOut = LX_DEMOD_LOCK_DEALY_600;
				break;

		}


		_neverLockResetTime = jiffies_to_msecs(jiffies);

		return RET_OK;
	}


	if (RET_OK != DEMOD_H14EUBB_Get_NeverLockStatus(scanMode, &neverLockStatus))
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
				DEMOD_PRINT( "[DEMOD H14] NEVERSCAN : LX_DEMOD_NEVERLOCK DETECTION is on initializing \n");

			}
			break;

		case LX_DEMOD_DETECTION_PROCESSING:
			{

				elapsedTime = jiffies_to_msecs(jiffies) - _neverLockResetTime;

				if(elapsedTime > _maxTimeOut)
				{
					*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
					_demodScanLock		= LX_DEMOD_UNLOCKED;
					DEMOD_PRINT( "[DEMOD H14] NEVERSCAN : LX_DEMOD_NEVERLOCK DETECTION is on processing \n");

				}
			}
			break;

		case LX_DEMOD_DETECTION_FAIL:
			{
				*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
				_demodScanLock		= LX_DEMOD_UNLOCKED;
				DEMOD_PRINT( "[DEMOD H14] NEVERSCAN :  LX_DEMOD_NEVERLOCK DETECTION FAIL  \n");
			}
			break;

		case LX_DEMOD_DETECTED:
			{
				*pScanProcessing 	= LX_DEMOD_FLAG_FINISH;
				_demodScanLock		= LX_DEMOD_TR_LOCK;
				DEMOD_PRINT( "[DEMOD H14] NEVERSCAN :  LX_DEMOD_NEVERLOCK DETECTED  !! \n");

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
 * DEMOD_H14EUBB_NeverLock_Infinite_Scan
 * Set IF frequency registers on DVB-C mode.
 *
 * @param operMode 	[IN] LX_DEMOD_OPER_MODE_T.
 * @param benable 	[IN] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_NeverLock_Infinite_Scan(LX_DEMOD_OPER_MODE_T operMode, BOOLEAN benable)
{
	UINT8 t2tdetrep_en;
	UINT8 cbldetrep_en;



	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	if((operMode == LX_DEMOD_DVBT) || (operMode == LX_DEMOD_DVBT2))
	{
		t2tdetrep_en = (benable == TRUE) ? 0x1 : 0x0;
		DVB_Common_Wr01(common_ctrl108,t2tdetrep_en,t2tdetrep_en);
		DVB_Common_WrFL(common_ctrl108);	//regAddr = 0x006c,COMMON_CTRL[108]
		//T2TDETREP_EN : T2/T Detection repeatedly (0 : Disable, 1 : Enable)

	}
	else if((operMode == LX_DEMOD_DVBC) || (operMode == LX_DEMOD_DVBC2))
	{
		cbldetrep_en = (benable == TRUE) ? 0x1 : 0x0;
		DVB_Common_Wr01(common_ctrl107,cbldetrep_en,cbldetrep_en);
		DVB_Common_WrFL(common_ctrl107);	//regAddr = 0x006b,COMMON_CTRL[107]
		// CBLDETREP_EN : Repetition enable in case of auto detection mode (0 : Disable, 1 : Enable)


	}
	else
	{
		DEMOD_PRINT("[DEMOD H14](%s:%d) Invaild Operation mode !!!\n", __F__, __L__);

		return RET_ERROR;
	}

	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_TPOutCLKEnable
 * Enable/Diable the output of TP CLK
 *
 * @param bEnable	  [IN] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_TPOutCLKEnable(BOOLEAN bEnable)
{
	UINT8 tp_oen;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	tp_oen = (bEnable == TRUE)?(1):(0);  //TP_OEN : TP output enable ( 0: Disable, 1:Enable)

	DVB_Common_RdFL(common_ctrl110);  //regAddr = 0x006e, COMMON_CTRL[110]
	DVB_Common_Wr01(common_ctrl110, tp_oen, tp_oen); //TP_OEN : TP output enable ( 0: Disable, 1:Enable)
	DVB_Common_WrFL(common_ctrl110);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Set_IF_Frq
 * Set IF frequency registers on DVB-C mode.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Set_IF_Frq(UINT32 ifFrq)
{

	UINT64  finalFreq;

	UINT8	crcenterf_31_24;
	UINT8	crcenterf_23_16;
	UINT8	crcenterf_15_8;
	UINT8	crcenterf_7_0;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

//	finalFreq = ((UINT64)ifFrq * 2^32) / 41 ;  --> in case of sampling frequency ( 41M ) We will use 41M sampling frequency on H14 EU chip.
//	finalFreq = ((UINT64)ifFrq * 2^32) / 48 ;  --> in case of sampling frequency ( 48M )
//	finalFreq = finalFreq / 1000 ;

	finalFreq = ((UINT64)ifFrq * 4294967296);  // 2^32 = 4294967296
	do_div(finalFreq, 41) ;
	do_div(finalFreq, 1000);

	crcenterf_31_24 = (UINT8) ((finalFreq >> 24) & 0xff);
	crcenterf_23_16 = (UINT8) ((finalFreq >> 16) & 0xff);
	crcenterf_15_8 = (UINT8) ((finalFreq >> 8) & 0xff);
	crcenterf_7_0 = (UINT8) ((finalFreq >> 0) & 0xff);

	DEMOD_PRINT(" %s : finalFreq = 0x%llx\n",__F__,finalFreq );

	// Select IF center frequency.
	// CRCENTERF = (5.6M/40.96M)x2^32 = 587202560 = 0x23000000

	DVB_Common_Wr01(common_ctrl7,crcenterf_31_24,crcenterf_31_24); //CRCENTERF[31:24]
	DVB_Common_WrFL(common_ctrl7);  //regAddr = 0x0007,COMMON_CTRL[7]
	// CRCENTERF[31:24] : Select IF center frequency.
	// CRCENTERF, IF = 5MHz, 41MHz OSC = (5M/41M)x232 = 0x1F3831F3

	DVB_Common_Wr01(common_ctrl8,crcenterf_23_16,crcenterf_23_16); //CRCENTERF[23:16]
	DVB_Common_WrFL(common_ctrl8);  //regAddr = 0x0008,COMMON_CTRL[8]

	DVB_Common_Wr01(common_ctrl9,crcenterf_15_8,crcenterf_15_8); //CRCENTERF[15:8]
	DVB_Common_WrFL(common_ctrl9);  //regAddr = 0x0009,COMMON_CTRL[9]

	DVB_Common_Wr01(common_ctrl10,crcenterf_7_0,crcenterf_7_0); //CRCENTERF[7:0]
	DVB_Common_WrFL(common_ctrl10);  //regAddr = 0x000a,COMMON_CTRL[10]

	return RET_OK;
}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Set_AGCPolarity
 * Set IF frequency registers on DVB.
 *
 * @param ifFrq 	[IN] UINT32 - IF frequency.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Set_AGCPolarity(LX_DEMOD_AGC_POLAR_T agcPola)
{
	UINT8 agcpol;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	agcpol = (agcPola == LX_DEMOD_AGC_ACTIVE_HIGH ) ? 1: 0;

	DVB_Common_RdFL(common_ctrl2);   //regAddr = 0x0002, COMMON_CTRL[2]
	DVB_Common_Wr01(common_ctrl2, agcpol, agcpol);
	DVB_Common_WrFL(common_ctrl2);
	//AGCPOL  : Select the polarity of AGC PWM output signal.(0 : Inversion, 1 : Normal)

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Set_SpectrumCtrl.
 * Enable/Disable automatic spectrum mode detection.
 *
 * @param  bAutoEnable [IN] BOOLEAN - enable/disable.
 * @param  bSpecInvEnable [IN] BOOLEAN - enable/disable.
 * @return if Succeeded - RET_OK else-RET_ERROR.
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Set_SpectrumCtrl(BOOLEAN bAutoEnable, BOOLEAN bSpecInvEnable)
{


	UINT8	specman;
	UINT8 	t2tc2_m_specinv;

	UINT8 autospec;
	UINT8 manspecinv;

	LX_DEMOD_OPER_MODE_T operMode;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if(operMode == LX_DEMOD_DVBC)
	{
		if(bAutoEnable == TRUE)
		{
			autospec = 1;

			C_Eq_RdFL(c_eq_ctrl0);	 //regAddr = 0x4800, C_EQ_CTRL[0]
			C_Eq_Wr01(c_eq_ctrl0, autospec, autospec);
			C_Eq_WrFL(c_eq_ctrl0);
			//AUTOSPEC : Automaic Spectrum inversion detection (0 : Off, 1 : On)

			DEMOD_PRINT( "[DEMOD H14 DTV] Success DVBC Spectrum AUTO -> %s !!!\n", (bAutoEnable == TRUE)?"ON":"OFF");
		}
		else
		{
			autospec = 0;
			manspecinv = (bSpecInvEnable == TRUE)?(0x1):(0x00);

			C_Eq_RdFL(c_eq_ctrl0);	  //regAddr = 0x4800, C_EQ_CTRL[0]
			C_Eq_Wr02(c_eq_ctrl0, autospec, autospec, manspecinv, manspecinv);
			C_Eq_WrFL(c_eq_ctrl0);
			// MANSPECINV : Manually control Spectrum inversion (0 : Normal, 1 : Inverted)

			DEMOD_PRINT( "[DEMOD H14 DTV]  Success DVBC Spectrum Auto -> manual, Inversion ->  %s !!!\n",(bSpecInvEnable == TRUE)?"ON":"OFF");
		}
	}
	else
	{
		if(bAutoEnable == TRUE)
		{
			specman = 0;

			DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
			DVB_Common_Wr01(common_ctrl1, specman, specman);
			DVB_Common_WrFL(common_ctrl1);
			// SPECMAN : Selection of auto or manual spectrum inversion
			// 0 : Auto spectrum inversion
			// 1 : Manual spectrum inversion

			DEMOD_PRINT( "[DEMOD H14 DTV] Success Spectrum  AUTO -> %s !!!\n", (bAutoEnable == TRUE)?"ON":"OFF");
		}
		else
		{
			specman = 1;
			t2tc2_m_specinv = (bSpecInvEnable == TRUE)?(0x1):(0x00);

			DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
			DVB_Common_Wr02(common_ctrl1, specman, specman, t2tc2_m_specinv, t2tc2_m_specinv);
			DVB_Common_WrFL(common_ctrl1);
			// SPECINV : Selection of normal or inverted spectrum
			// 0 : Normal
			// 1 : Inverted

			DEMOD_PRINT( "[DEMOD H14 DTV] Success Spectrum Auto -> manual, Inversion ->  %s !!!\n",(bSpecInvEnable == TRUE)?"ON":"OFF");
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
int DEMOD_H14EUBB_Set_TirotCtrl(void)
{

	UINT8 	bkgim_2_0;
	UINT8 	bkgim_done;
	UINT8 	bkfftm_2_0;
	UINT8 	bkfftm_done;

	UINT8 	fecdecen;
	UINT8 	t2_l1pre_done;
	UINT8 	tiroton;

	LX_DEMOD_OPER_MODE_T 	operMode;


	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);


	if(RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		DEMOD_RETURN_FAIL(__F__);
	}


	if(operMode == LX_DEMOD_DVBT2)
	{

		T2_Pre_Sync_RdFL(t2_pre_sync_mon0); //regAddr = 0x1080, T2_PRE_SYNC_MON[0]
		T2_Pre_Sync_Rd04(t2_pre_sync_mon0,bkfftm_done,bkfftm_done,bkgim_done, bkgim_done, bkfftm_2_0, bkfftm_2_0, bkgim_2_0, bkgim_2_0 );

		T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon31);	 //regAddr = 0x24df, T2_PRE_SYNC_CTRL[31]
		T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon31, fecdecen, fecdecen);

		T2_PLP1_RdFL(t2_plp_mon56);  //regAddr = 0x4048, T2_PLP_MON[56]
		T2_PLP1_Rd01(t2_plp_mon56,t2_l1pre_done,t2_l1pre_done);

		if((bkfftm_done == 1) && (bkgim_done == 1))
		{

				if(((bkfftm_2_0 == 5) && (bkgim_2_0 == 5)) || ((bkfftm_2_0 == 4) && (bkgim_2_0 == 3)))
				{

						if(fecdecen == 1)
						{
							tiroton = 1;
							T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
							T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
							T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);

						}

				}
				else if(t2_l1pre_done == 1)
				{
					tiroton = 1;
					T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
					T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
					T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);

				}

		}
	}
	else
	{
		tiroton = 1;
		T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl2);	 //regAddr = 0x2402, T2TC2_EQ_TI_CTRL[2]
		T2TC2_Eq_Ti_Wr01(t2tc2_eq_ti_ctrl2, tiroton, tiroton);
		T2TC2_Eq_Ti_WrFL(t2tc2_eq_ti_ctrl2);

	}

	return RET_OK;



}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_Id.
 * Get GBB demod ID.
 *
 * @param *cell_id [OUT] UINT16 - GBB demod ID
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_Id(UINT16 *pId)
{
	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);
	*pId = 0;


	return RET_OK;
}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_SpectrumInvStatus
 * Get spectrum mode state on DVB.
 *
 * @param *pSpectrumAuto 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_AUTO, LX_DEMOD_MANUAL.
 * @param *pSpectrumInv 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMOD_NORMAL, LX_DEMOD_INVERSION.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_SpectrumStatus(LX_DEMOD_LOCK_STATE_T *pSpectrumAuto, LX_DEMOD_LOCK_STATE_T *pSpectrumInv)
{

	UINT8	specman;
	UINT8 	t2tc2_specinv;
	UINT8 	c_specinv;

	UINT8 autospec;

	UINT8 isSpecAuto;
	UINT8 isSpecInv;

	LX_DEMOD_OPER_MODE_T operMode;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	if(operMode == LX_DEMOD_DVBC)
	{
		C_Eq_RdFL(c_eq_ctrl0);	 //regAddr = 0x4800, C_EQ_CTRL[0]
		C_Eq_Rd01(c_eq_ctrl0, autospec, autospec);
		//AUTOSPEC : Automaic Spectrum inversion detection (0 : Off, 1 : On)

		isSpecAuto = autospec;

		C_Eq_RdFL(c_eq_mon4);	 //regAddr = 0x48c7, C_EQ_CTRL[4]
		C_Eq_Rd01(c_eq_mon4, c_specinv, c_specinv);
		//C_SPECINV : Spectrum inversion indicator (0 : Normal, 1 : Inverted)

		isSpecInv = c_specinv;
	}
	else
	{
		DVB_Common_RdFL(common_ctrl1);	 //regAddr = 0x0001, COMMON_CTRL[1]
		DVB_Common_Rd01(common_ctrl1, specman, specman);
		// Selection of auto or manual spectrum inversion
		// 0 : Auto spectrum inversion
		// 1 : Manual spectrum inversion

		isSpecAuto = (specman == 0) ? 1 : 0;

		DVB_Common_RdFL(common_mon1);  //regAddr = 0x0081, COMMON_MON[1]
		DVB_Common_Rd01(common_mon1,t2tc2_specinv,t2tc2_specinv);
		//T2TC2_SPECINV : Selection of normal or inverted spectrum(0 : Normal, 1 : Inverted)

		isSpecInv = t2tc2_specinv;
	}
	if(isSpecAuto)
	{
		*pSpectrumAuto = LX_DEMOD_AUTO;
	}
	else
	{
		*pSpectrumAuto = LX_DEMOD_MANUAL;
	}
	if(isSpecInv)
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
 * DEMOD_H14EUBB_Get_IFAGCValue
 * Get IF AGC value on DVB.
 *
 * @param *pIfagcValue 	[OUT] UINT16
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_IFAGC(UINT16 *pIfAgc)
{


	UINT8	agcsigpwr_15_8;
	UINT8	agcsigpwr_7_0;

	UINT16 ifagc = 0;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	*pIfAgc = 0;

	DVB_Common_RdFL(common_mon8);  //regAddr = 0x0088, COMMON_MON[8]
	DVB_Common_Rd01(common_mon8,agcsigpwr_15_8,agcsigpwr_15_8); //AGCSIGPWR[15:8]: Show the current AGC signal power.
	ifagc = (agcsigpwr_15_8 << 8);

	DVB_Common_RdFL(common_mon9);  //regAddr = 0x0089, COMMON_MON[9]
	DVB_Common_Rd01(common_mon9,agcsigpwr_7_0,agcsigpwr_7_0); //AGCSIGPWR[7:0]: Show the current AGC signal power.
	ifagc += (agcsigpwr_7_0 << 0);

	*pIfAgc = ifagc;

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_OperMode.
 * Get GBB operation mode.
 *
 * @param *pOperMode [OUT] LX_DEMOD_VQI_RF_MODE_T - GBB RF demodulation mode
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_OperMode(LX_DEMOD_OPER_MODE_T *pOperMode)
{

	UINT8	standard_1_0 = 0;

	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	*pOperMode = LX_DEMOD_UNKOWN_OPMODE;

	DVB_Common_RdFL(common_mon0); //regAddr = 0x0080, COMMON_MON0[0]
	DVB_Common_Rd01(common_mon0,standard_1_0, standard_1_0 );  //DETSTDOPMODE[7:0]

/*
	Detected standard (operation mode).
	00 : DVB-T2 mode
	01 : DVB-T mode
	10 : DVB-C2 mode
	11 : DVB-C mode

  */

	if(standard_1_0 == 0)		*pOperMode = LX_DEMOD_DVBT2;
	else if(standard_1_0 == 1) 	*pOperMode = LX_DEMOD_DVBT;
	else if(standard_1_0 == 2) 	*pOperMode = LX_DEMOD_DVBC2;
	else if(standard_1_0 == 3) 	*pOperMode = LX_DEMOD_DVBC;
	else *pOperMode = LX_DEMOD_UNKOWN_OPMODE;

	DEMOD_PRINT("[DEMOD H14] *pOperMode = %d \n",*pOperMode);

	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_NeverLockStatus
 * Get neverlock state on DVB.
 *
 * @param scanMode 	[OUT] LX_DEMOD_OPER_MODE_T
 * @param *pNeverLockStatus 	[OUT] LX_DEMOD_LOCK_STATE_T - LX_DEMODLOCKED, LX_DEMOD_UNLOCKED.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_NeverLockStatus(LX_DEMOD_OPER_MODE_T scanMode, LX_DEMOD_LOCK_STATE_T *pNeverLockStatus)
{
	UINT8 neverlock_1_0;


	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	*pNeverLockStatus = LX_DEMOD_UNKOWN_STATE;

	DVB_Common_RdFL(common_mon0);  //regAddr = 0x0080, COMMON_MON[0]
	DVB_Common_Rd01(common_mon0,neverlock_1_0,neverlock_1_0); //NEVERLOCK[1:0] : Never lock for signal existence status indicaiton
	// 00 : init
	// 01 : processing
	// 10 : lock
	// 11 : fail


	DEMOD_PRINT("[DEMOD H14](%s) neverlock_1_0 = %d !!!\n", __F__,neverlock_1_0);


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


	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_CarrierFreqOffset
 * Get carrier offset frequency .
 *
 * @param *pFreqOff 	[OUT] SINT32.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_CarrierFreqOffset(SINT32 *pFreqOff)
{

		UINT8 	cfo_31_24;
		UINT8 	cfo_23_16;
		UINT8 	cfo_15_8;
		UINT8 	cfo_7_0;

		SINT64 tempCarrierOffset = 0;
		SINT64 carrierOffset = 0;

		 LX_DEMOD_BWMODE_T channelBW;
		 UINT16 bwFactor = 0;

		DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

		if (RET_OK != DEMOD_H14EUBB_Get_BandWidthMode(&channelBW))
		{
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_BandWidthMode() !!!\n", __F__, __L__);
			return RET_ERROR;
		}

		*pFreqOff = 0;

		DVB_Common_RdFL(common_mon16);  //regAddr = 0x0090, COMMON_MON[16]
		DVB_Common_Rd01(common_mon16,cfo_31_24,cfo_31_24); //CFO[31:24]
		tempCarrierOffset = (cfo_31_24 << 24 ) ;

		DVB_Common_RdFL(common_mon17);  //regAddr = 0x0091, COMMON_MON[17]
		DVB_Common_Rd01(common_mon17,cfo_23_16,cfo_23_16); //CFO[23:16]
		tempCarrierOffset += (cfo_23_16 << 16 ) ;

		DVB_Common_RdFL(common_mon18);  //regAddr = 0x0092, COMMON_MON[18]
		DVB_Common_Rd01(common_mon18,cfo_15_8,cfo_15_8); //CFO[15:8]
		tempCarrierOffset += (cfo_15_8 << 8 ) ;

		DVB_Common_RdFL(common_mon19);  //regAddr = 0x0093, COMMON_MON[19]
		DVB_Common_Rd01(common_mon19,cfo_7_0,cfo_7_0); //CFO[7:0]
		tempCarrierOffset += (cfo_7_0 << 0 ) ;

		// CFO[31:24], CFO[23:16], CFO[15:8], CFO[7:0]   : Show the estimated carrier frequency offset.
		// CFO = {(64/7) x CFO x 1000} / 2^31 [kHz] at BW = 8MHz
		// CFO = {(8) x CFO x 1000} / 2^31 [kHz] at BW = 7MHz
		// CFO = {(48/7) x CFO x 1000} / 2^31 [kHz] at BW = 6MHz
		// CFO = {(40/7) x CFO x 1000} / 2^31 [kHz] at BW = 5MHz
		// CFO = {(131/71) x CFO x 1000} / 2^31 [kHz] at BW = 1.7MHz

		if ( tempCarrierOffset & 0x80000000 ) tempCarrierOffset = ((0xffffffff00000000) | tempCarrierOffset) ;

		switch(channelBW)
		{
			case LX_DEMOD_BW_8MHZ :
				bwFactor = 9143;  // (64/7) * 1000
				break;

			case LX_DEMOD_BW_7MHZ :
				bwFactor = 8000;  // (8) * 1000
				break;

			case LX_DEMOD_BW_6MHZ :
				bwFactor = 6857;  // (48/7) * 1000
				break;

			case LX_DEMOD_BW_5MHZ :
				bwFactor = 5714;  // (40/7) * 1000
				break;

			case LX_DEMOD_BW_1_7MHZ :
				bwFactor = 9143;  // (131/71) * 1000
				break;

			default :  // default : 8MHz
				bwFactor = 1845;  // (64/7) * 1000
				break;

		}



		carrierOffset =  tempCarrierOffset * bwFactor ;  	// Carrier frequency offset [MHz] = {(64/7) x CFO x 1000} / 2^31 [kHz] at BW = 8MHz, 64/7 * 1000 = 9143.
													// 1000000[Hz] / 16384 = 61
//		do_div((UINT64)carrierOffset, 2147483648 );     	// 2^31 = 2147483648


		*pFreqOff = (-1) * (SINT32)(carrierOffset >> 31);


		DEMOD_PRINT("[DEMOD H14] *********************************************!!!\n");
		DEMOD_PRINT("[DEMOD H14] Carrier Offset freq = %d [KHz] !!!\n",*pFreqOff);
		DEMOD_PRINT("[DEMOD H14] **********************************************!!!\n");


		return RET_OK;

}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_TPIFStatus
 * Get the status of TP IF function (active or inactive)
 *
 * @param pbEnable	  [OUT] BOOLEAN.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_TPIFStatus(BOOLEAN *pbEnable)
{
	DEMOD_PRINT("[DEMOD H14 DTV] @ %s =====\n",__F__);

	*pbEnable = TRUE;

	return RET_OK;


}


/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_VABER
 * Get BER before RS on DVB
 *
 * @param *pVber 	[OUT] UINT16.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_VABER(UINT32 *pVber)
{

	LX_DEMOD_OPER_MODE_T operMode;

	*pVber = 0xffffffff;

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}


	switch(operMode)
	{
		case LX_DEMOD_DVBT:
		case LX_DEMOD_DVBC:
			{
				UINT8	rsber_15_8;
				UINT8	rsber_7_0;

				UINT16	tempBer = 0;


				TC_FEC_RdFL(tc_fec_mon16);	 //regAddr = 0x5890,TC_FEC_MON[16]
				TC_FEC_Rd01(tc_fec_mon16, rsber_15_8, rsber_15_8); //RSBER [15:8] : BER before RS decoder

				tempBer = (rsber_15_8 << 8);

				TC_FEC_RdFL(tc_fec_mon17);	 //regAddr = 0x5891,TC_FEC_MON[17]
				TC_FEC_Rd01(tc_fec_mon17, rsber_7_0, rsber_7_0); //RSBER [7:0] : BER before RS decoder
				tempBer += (rsber_7_0 << 0);

				*pVber = (UINT32)tempBer;

			}
			break;

		case LX_DEMOD_DVBT2:
		case LX_DEMOD_DVBC2:
			{
				UINT8	noe_est_mdp_19_16;
				UINT8	noe_est_mdp_15_8;
				UINT8	noe_est_mdp_7_0;

				UINT8	noe_correct_mdp_19_16;
				UINT8	noe_correct_mdp_15_8;
				UINT8	noe_correct_mdp_7_0;

				UINT8	nob_add_2_0;

				UINT32	estimatedBer;
				UINT32	correctedBer;

				UINT32 bchBer;



				T2C2_FEC_RdFL(t2c2_fec_mon40);	 //regAddr = 0x30a8,T2C2_FEC_MON[40]
				T2C2_FEC_Rd01(t2c2_fec_mon40, noe_est_mdp_19_16, noe_est_mdp_19_16); //NOE_EST_MDP[19:16]

				T2C2_FEC_RdFL(t2c2_fec_mon41);	 //regAddr = 0x30a9,T2C2_FEC_MON[41]
				T2C2_FEC_Rd01(t2c2_fec_mon41, noe_est_mdp_15_8, noe_est_mdp_15_8); //NOE_EST_MDP[15:8]

				T2C2_FEC_RdFL(t2c2_fec_mon42);	 //regAddr = 0x30aa,T2C2_FEC_MON[42]
				T2C2_FEC_Rd01(t2c2_fec_mon42, noe_est_mdp_7_0, noe_est_mdp_7_0); //NOE_EST_MDP[7:0]

				estimatedBer = (noe_est_mdp_19_16 << 16) + (noe_est_mdp_15_8 << 8) + (noe_est_mdp_7_0 << 0);

				T2C2_FEC_RdFL(t2c2_fec_mon37);	 //regAddr = 0x30a5,T2C2_FEC_MON[37]
				T2C2_FEC_Rd01(t2c2_fec_mon37, noe_correct_mdp_19_16, noe_correct_mdp_19_16); //NOE_CORRECT_MDP[19:16]

				T2C2_FEC_RdFL(t2c2_fec_mon38);	 //regAddr = 0x30a6,T2C2_FEC_MON[37]
				T2C2_FEC_Rd01(t2c2_fec_mon38, noe_correct_mdp_15_8, noe_correct_mdp_15_8); //NOE_CORRECT_MDP[15:8]

				T2C2_FEC_RdFL(t2c2_fec_mon39);	 //regAddr = 0x30a7,T2C2_FEC_MON[37]
				T2C2_FEC_Rd01(t2c2_fec_mon39, noe_correct_mdp_7_0, noe_correct_mdp_7_0); //NOE_CORRECT_MDP[7:0]

				correctedBer = (noe_correct_mdp_19_16 << 16) + (noe_correct_mdp_15_8 << 8) + (noe_correct_mdp_7_0 << 0);

				T2C2_FEC_RdFL(t2c2_fec_mon43);	 //regAddr = 0x30ab,T2C2_FEC_MON[43]
				T2C2_FEC_Rd01(t2c2_fec_mon43, nob_add_2_0, nob_add_2_0); //NOB_ADD[2:0]


				bchBer = ((estimatedBer - correctedBer) / (nob_add_2_0 * 25)) * 100;



				*pVber =  bchBer ;


			}
			break;



		default :
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invalid opermode(0x%x) !!!\n", __F__, __L__,operMode);
			break;


	}


	return RET_OK;



}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_Packet_Error
 * Get packet error on DVB
 *
 * @param *pSnr 	[OUT] UINT8.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_Packet_Error(UINT32 *pError)
{
	LX_DEMOD_OPER_MODE_T operMode;

	*pError = 0xffffffff;

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	switch(operMode)
	{
		case LX_DEMOD_DVBT:
			{
				UINT8 pkerrcnt_ac_14_8;
				UINT8 pkerrcnt_ac_7_0;
				UINT8 mbrflg;

				mbrflg = 1;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				TC_FEC_RdFL(tc_fec_mon7);	//regAddr = 0x5887, TC_FEC_MON[7]
				TC_FEC_Rd01(tc_fec_mon7, pkerrcnt_ac_14_8, pkerrcnt_ac_14_8); //PKERRCNT_AC [14:8] : Packet error count until PKERR_CLR = '1'

				TC_FEC_RdFL(tc_fec_mon8);	//regAddr = 0x5888, TC_FEC_MON[8]
				TC_FEC_Rd01(tc_fec_mon8, pkerrcnt_ac_7_0, pkerrcnt_ac_7_0); //PKERRCNT_AC [7:0] : Packet error count until PKERR_CLR = '1'

				mbrflg = 0;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				*pError =  (UINT32)(pkerrcnt_ac_14_8 << 8) ;

				*pError +=  (UINT32)(pkerrcnt_ac_7_0 << 0) ;

			}
			break;

		case LX_DEMOD_DVBT2:
			{
				UINT8	fer_datapath_sw_7_0;

				T2C2_FEC_RdFL(t2c2_fec_mon35);	//regAddr = 0x30a3, T2C2_FEC_MON[35]
				T2C2_FEC_Rd01(t2c2_fec_mon35, fer_datapath_sw_7_0, fer_datapath_sw_7_0);


				*pError =  (UINT32)fer_datapath_sw_7_0 ;
			}
			break;

		case LX_DEMOD_DVBC :
			{
				UINT8	pkerrcnt_ac_14_8;
				UINT8	pkerrcnt_ac_7_0;

				TC_FEC_RdFL(tc_fec_mon7);	//regAddr = 0x5887, TC_FEC_MON[7]
				TC_FEC_Rd01(tc_fec_mon7, pkerrcnt_ac_14_8, pkerrcnt_ac_14_8); //PKERRCNT_AC [14:8]

				*pError =  (UINT32)(pkerrcnt_ac_14_8 << 8) ;

				TC_FEC_RdFL(tc_fec_mon8);	//regAddr = 0x5888, TC_FEC_MON[8]
				TC_FEC_Rd01(tc_fec_mon8, pkerrcnt_ac_7_0, pkerrcnt_ac_7_0); // PKERRCNT_AC [7:0]

				*pError +=  (UINT32)(pkerrcnt_ac_7_0 << 0) ;

			}
			break;

		case LX_DEMOD_DVBC2 :
			{
				UINT8	fer_datapath_sw_7_0;

				T2C2_FEC_RdFL(t2c2_fec_mon35);	//regAddr = 0x30a3, T2C2_FEC_MON[35]
				T2C2_FEC_Rd01(t2c2_fec_mon35, fer_datapath_sw_7_0, fer_datapath_sw_7_0);


				*pError =  (UINT32)fer_datapath_sw_7_0 ;

			}
			break;

		default :
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invalid opermode(0x%x) !!!\n", __F__, __L__,operMode);
			break;



	}

	DEMOD_PRINT("  *pError = %d  \n",*pError);


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_BandWidthMode
 * Get the information of bandwidth .
 *
 * @param *ChannelBW 	[OUT] LX_DEMOD_BWMODE_T - 6, 7 and 8 MHZ.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_BandWidthMode( LX_DEMOD_BWMODE_T *ChannelBW )
{

	UINT8 	detrfbw_2_0;

	*ChannelBW = LX_DEMOD_BW_UNKNOWN;

	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_ctrl0);  //regAddr = 0x2400, T2TC2_Eq_TI_CTRL[0]
	T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_ctrl0,detrfbw_2_0,detrfbw_2_0);
	//DETRFBW[2:0] : Detected RF bandwidth indication
	// 000 : 8MHz
	// 001 : 7MHz
	// 010 : 6MHz
	// 011 : 5MHz
	// 100 : 1.7MHz
	// 101 ~ 111 : reserved

	switch(detrfbw_2_0)
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
		case 0x03 :
			*ChannelBW = LX_DEMOD_BW_5MHZ;
			break;
		case 0x04:
			*ChannelBW = LX_DEMOD_BW_1_7MHZ;
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
 * DEMOD_H14EUBB_Get_QAMMode
 * Get the information of QAM mode .
 *
 * @param *pQamMode [OUT] LX_DEMOD_RF_MODE_T - 16QAM, 32QAM, 64QAM, 128QAM, 256QAM, QPSK.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_QAMMode( LX_DEMOD_RF_MODE_T *pQamMode )
{
	LX_DEMOD_OPER_MODE_T operMode;

	*pQamMode = LX_DEMOD_UNKOWN_MODE;

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H13_GetOperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

  //to do -> check mode lock indicator

	switch(operMode)
	{
		case LX_DEMOD_DVBT:
			{
				UINT8	qamm_1_0;

				T_Post_Sync_RdFL(t_post_sync_mon1); //regAddr = 0x1381, T_POST_SYNC_MON[1]
				T_Post_Sync_Rd01(t_post_sync_mon1,qamm_1_0,qamm_1_0);

				// QAMM[1:0] : Constellation patterns.
				// 00 : QPSK
				// 01 : 16QAM
				// 10 : 64QAM
				// 11 : Reserved
				DEMOD_PRINT("[DEMOD H14] (%s) qamm_1_0 = %d !!!\n", __F__,qamm_1_0);

				switch(qamm_1_0)
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

		case LX_DEMOD_DVBT2:
			{
				UINT8	dplp_qam_2_0;

				T2_PLP1_RdFL(t2_plp_mon69); //regAddr = 0x4055, T2_PLP_MON[69]
				T2_PLP1_Rd01(t2_plp_mon69,dplp_qam_2_0,dplp_qam_2_0);
				// Data PLP QAM mode
				// 000 : QPSK
				// 001 : 16QAM
				// 010 : 64QAM
				// 011 : 256QAM
				// others : reserved

				switch(dplp_qam_2_0)
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
					case 0x03 :
						*pQamMode = LX_DEMOD_256QAM;
						break;
					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}
			}
			break;

		case LX_DEMOD_DVBC :
			{
				UINT8	qamm_2_0;

				C_Eq_RdFL(c_eq_mon5); //regAddr = 0x48c8, C_EQ_MON[5]
				C_Eq_Rd01(c_eq_mon5,qamm_2_0,qamm_2_0);
				// Data PLP QAM mode
				// 001 : 16QAM
				// 010 : 32QAM
				// 011 : 64QAM
				// 100 : 128QAM
				// 101 : 256QAM
				// others : reserved

				switch(qamm_2_0)
				{
					case 0x01 :
						*pQamMode = LX_DEMOD_16QAM;
						break;
					case 0x02:
						*pQamMode = LX_DEMOD_32QAM;
						break;
					case 0x03 :
						*pQamMode = LX_DEMOD_64QAM;
						break;
					case 0x04 :
						*pQamMode = LX_DEMOD_128QAM;
						break;
					case 0x05:
						*pQamMode = LX_DEMOD_256QAM;
						break;
					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}

			}
			break;

		case LX_DEMOD_DVBC2 :
			{
				UINT8 ds0_dplp_mod_2_0;

				C2_PLP1_RdFL(c2_plp_mon44);	//regAddr = 0x424c, C2_PLP_MON[44]
				C2_PLP1_Rd01(c2_plp_mon44,ds0_dplp_mod_2_0,ds0_dplp_mod_2_0);
				// DS0_dPLP_MOD[2:0] : Data PLP QAM mode
				// 001 : 16 QAM
				// 010 : 64 QAM
				// 011 : 256 QAM
				// 100 : 1024 QAM
				// 101 : 4096 QAM
				// others : reserved

				switch(ds0_dplp_mod_2_0)
				{
					case 0x01:
						*pQamMode = LX_DEMOD_16QAM;
						break;

					case 0x02 :
						*pQamMode = LX_DEMOD_64QAM;
						break;

					case 0x03 :
						*pQamMode = LX_DEMOD_256QAM;
						break;

					case 0x04 :
						*pQamMode = LX_DEMOD_1024QAM;
						break;

					case 0x05 :
						*pQamMode = LX_DEMOD_4096QAM;
						break;

					default :
						*pQamMode = LX_DEMOD_UNKOWN_MODE;
						break;
				}

			}
			break;

		default :
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invalid opermode(0x%x) !!!\n", __F__, __L__,operMode);
			break;


	}


	return RET_OK;

}

/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_MseInfo
 * Get signal MSE info.
 *
 * @param *pSigMse [OUT] LX_DEMOD_MSE_T.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_MseInfo( LX_DEMOD_MSE_T *pSigMse)
{

	LX_DEMOD_OPER_MODE_T operMode;

	pSigMse->mse = 0xffff;
	pSigMse->bMse = 0xffff;
	pSigMse->constPwr = 0xffff;

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		return RET_ERROR;
	}

	DEMOD_PRINT("[DEMOD H14] DEMOD_H14EUBB_Get_OperMode() = %d !!!\n",operMode );

	switch(operMode)
	{
		case LX_DEMOD_DVBT:
			{
				UINT8	demodmse_13_8;
				UINT8	demodmse_7_0;
				UINT8 mbrflg;

				mbrflg = 1;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon21); //regAddr = 0x24d5, T2TC2_Eq_TI_MON[21]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon21,demodmse_13_8,demodmse_13_8);  //DEMODMSE[13:8]

				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon22); //regAddr = 0x24d6, T2TC2_Eq_TI_MON[22]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon22,demodmse_7_0,demodmse_7_0);  //DEMODMSE[7:0]

				mbrflg = 0;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				pSigMse->mse = (demodmse_13_8 << 8) + demodmse_7_0;

				pSigMse->bMse = 1;
				pSigMse->constPwr = 1;


			}
			break;

		case LX_DEMOD_DVBT2:
			{
				UINT8	demodmse_13_8;
				UINT8	demodmse_7_0;
				UINT8 mbrflg;

				mbrflg = 1;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon21); //regAddr = 0x24d5, T2TC2_Eq_TI_MON[21]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon21,demodmse_13_8,demodmse_13_8);  //DEMODMSE[13:8]

				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon22); //regAddr = 0x24d6, T2TC2_Eq_TI_MON[22]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon22,demodmse_7_0,demodmse_7_0);  //DEMODMSE[7:0]

				mbrflg = 0;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values

				pSigMse->mse = (demodmse_13_8 << 8) + demodmse_7_0;

				pSigMse->bMse = 1;
				pSigMse->constPwr = 1;

				DEMOD_PRINT("[DEMOD H14] demodmse_13_8 = 0x%x !!!\n",demodmse_13_8 );
				DEMOD_PRINT("[DEMOD H14] demodmse_7_0  = 0x%x !!!\n",demodmse_7_0 );
				DEMOD_PRINT("[DEMOD H14] pSigMse->mse  = 0x%x !!!\n",pSigMse->mse );
			}
			break;

		case LX_DEMOD_DVBC :
			{
				UINT8	mse_15_8;
				UINT8	mse_7_0;
				UINT8 mbrflg;

				mbrflg = 1;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				C_Eq_RdFL(c_eq_mon0); //regAddr = 0x48c3, C_EQ_MON[0]
				C_Eq_Rd01(c_eq_mon0,mse_15_8,mse_15_8);  //MSE[15:8]

				C_Eq_RdFL(c_eq_mon1);  //regAddr = 0x48c4, C_EQ_MON[1]
				C_Eq_Rd01(c_eq_mon1,mse_7_0,mse_7_0);  //MSE[7:0]

				mbrflg = 0;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values

				pSigMse->mse = (mse_15_8 << 8) + mse_7_0;

				pSigMse->bMse = 1;
				pSigMse->constPwr = 1;


			}
			break;

		case LX_DEMOD_DVBC2 :
			{
				UINT8	demodmse_13_8;
				UINT8	demodmse_7_0;
				UINT8 mbrflg;

				mbrflg = 1;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values


				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon21); //regAddr = 0x24d5, T2TC2_Eq_TI_MON[21]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon21,demodmse_13_8,demodmse_13_8);  //DEMODMSE[13:8]

				T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon22); //regAddr = 0x24d6, T2TC2_Eq_TI_MON[22]
				T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon22,demodmse_7_0,demodmse_7_0);  //DEMODMSE[7:0]

				mbrflg = 0;
				DVB_Common_RdFL(common_ctrl0);	//regAddr = 0x0000, COMMON_CTRL[0]
				DVB_Common_Wr01(common_ctrl0, mbrflg, mbrflg);	//MBRFLG
				DVB_Common_WrFL(common_ctrl0);
				// MBRFLG : Select I2C multi-byte read option.
				// 0 : Normal
				// 1 : Hold I2C values

				pSigMse->mse = (demodmse_13_8 << 8) + demodmse_7_0;

				pSigMse->bMse = 1;
				pSigMse->constPwr = 1;

				DEMOD_PRINT("[DEMOD H14] demodmse_13_8 = 0x%x !!!\n",demodmse_13_8 );
				DEMOD_PRINT("[DEMOD H14] demodmse_7_0  = 0x%x !!!\n",demodmse_7_0 );
				DEMOD_PRINT("[DEMOD H14] pSigMse->mse  = 0x%x !!!\n",pSigMse->mse );
			}
			break;

		default :
			DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) Invalid opermode(0x%x) !!!\n", __F__, __L__,operMode);
			break;


	}


	return RET_OK;


}



/***********************************************************************************
************************************************************************************/
/**
 * DEMOD_H14EUBB_Get_Lock
 * Check the DVB lock state
 *
 * @param eStatus 	[IN] LX_DEMOD_LOCK_STATUS_T - which lock flags are selected.
 * @return if Succeeded - RET_OK else-RET_ERROR
 * @see
 * @author 	Jeongpil.yun (jeongpil.yun@lge.com)
*/
int DEMOD_H14EUBB_Get_Lock(LX_DEMOD_LOCK_STATUS_T whatLock, LX_DEMOD_LOCK_STATE_T *lockState)
{

	LX_DEMOD_OPER_MODE_T operMode;
	UINT8 tempLockStatus = 0;

	UINT8 t2_l1pre_done;
	UINT8 t2_l1post_done;

	UINT8 c2_l1pre_done;
	UINT8 c2_l1post_done;


	UINT8 tps_frmlock;

	UINT8 fecdecen;

	UINT8 frmlock_c;

//	DEMOD_PRINT("^p^[DEMOD H14 DTV] @ %s =====\n",__F__);

	if (RET_OK != DEMOD_H14EUBB_Get_OperMode(&operMode))
	{
		DEMOD_PRINT("[DEMOD H14] FAIL (%s:%d) DEMOD_H14EUBB_Get_OperMode() !!!\n", __F__, __L__);
		*lockState = LX_DEMOD_UNKOWN_STATE;
		return RET_ERROR;
	}

    switch( whatLock )
    {


		case LX_DEMOD_L1_DONE:
			{
				if(operMode == LX_DEMOD_DVBT2) //DVB-T2
				{
					T2_PLP1_RdFL(t2_plp_mon56);  //regAddr = 0x4048, T2_PLP_MON[56]
					T2_PLP1_Rd02(t2_plp_mon56,t2_l1pre_done,t2_l1pre_done,t2_l1post_done,t2_l1post_done);
					if(t2_l1pre_done & t2_l1post_done) tempLockStatus = 1;
					else tempLockStatus = 0;

					DEMOD_PRINT("[DEMOD H14]l1pre_done = %d, l1post_done = %d, tempLockStatus = %d !!!\n", t2_l1pre_done,t2_l1post_done,tempLockStatus);
					//T2_L1PRE_DONE : L1 pre done signal(0 : not-done, 1 : done)
					//T2_L1POST_DONE : L1 post done signal(0 : not-done, 1 : done)

				}
				else if(operMode == LX_DEMOD_DVBC2) //DVB-C2
				{
					C2_PLP1_RdFL(c2_plp_mon3);  //regAddr = 0x4223, C2_PLP_MON[3]
					C2_PLP1_Rd02(c2_plp_mon3,c2_l1pre_done,c2_l1pre_done,c2_l1post_done,c2_l1post_done);
					if(c2_l1pre_done & c2_l1post_done) tempLockStatus = 1;
					else tempLockStatus = 0;

					DEMOD_PRINT("[DEMOD H14]l1pre_done = %d, l1post_done = %d, tempLockStatus = %d !!!\n", c2_l1pre_done,c2_l1post_done,tempLockStatus);
					//C2_L1PRE_DONE : L1 pre done signal(0 : not-done, 1 : done)
					//C2_L1POST_DONE : L1 post done signal(0 : not-done, 1 : done)

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					DEMOD_PRINT("[DEMOD H14](%s:%d) Invalid Operation mode !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
			break;

		case LX_DEMOD_TPS_LOCK:
			{
				if(operMode == LX_DEMOD_DVBT) //DVB-T
				{
					T_Post_Sync_RdFL(t_post_sync_mon5); //regAddr = 0x1385, T_POST_SYNC_MON[5]
					T_Post_Sync_Rd01(t_post_sync_mon5,tps_frmlock,tps_frmlock);  //TPS_FRMLOCK
					if(tps_frmlock) tempLockStatus = 1;
					else tempLockStatus = 0;

					//TPS_FRMLOCK : TPS detection signal.(0 : TPS not detected, 1 : TPS detected)

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					DEMOD_PRINT("[DEMOD H14](%s:%d) Invalid Operation mode !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
			break;

		case LX_DEMOD_FEC_DEC_EN:
			{
				if((operMode == LX_DEMOD_DVBT2)) //DVB-T2
				{
					T2_PLP1_RdFL(t2_plp_mon56);  //regAddr = 0x4048, T2_PLP_MON[56]
					T2_PLP1_Rd02(t2_plp_mon56,t2_l1pre_done,t2_l1pre_done,t2_l1post_done,t2_l1post_done);

					//T2_L1PRE_DONE : L1 pre done signal(0 : not-done, 1 : done)
					//T2_L1POST_DONE : L1 post done signal(0 : not-done, 1 : done)

					T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon31);  //regAddr = 0x24df, T2TC2_Eq_TI_MON[31]
					T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon31,fecdecen,fecdecen); //FECDECEN
					if(t2_l1pre_done & t2_l1post_done & fecdecen) tempLockStatus = 1;
					else tempLockStatus = 0;

				}
				else if((operMode == LX_DEMOD_DVBT)) //DVB-T
				{
					T_Post_Sync_RdFL(t_post_sync_mon5); //regAddr = 0x1385, T_POST_SYNC_MON[5]
					T_Post_Sync_Rd01(t_post_sync_mon5,tps_frmlock,tps_frmlock);  //TPS_FRMLOCK

					//TPS_FRMLOCK : TPS detection signal.(0 : TPS not detected, 1 : TPS detected)

					T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon31);  //regAddr = 0x24df, T2TC2_Eq_TI_MON[31]
					T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon31,fecdecen,fecdecen); //FECDECEN
					if(tps_frmlock & fecdecen) tempLockStatus = 1;
					else tempLockStatus = 0;

				}
				else if(operMode == LX_DEMOD_DVBC2) //DVB-C2
				{
					C2_PLP1_RdFL(c2_plp_mon3);  //regAddr = 0x4223, T2_PLP_MON[40]
					C2_PLP1_Rd02(c2_plp_mon3,c2_l1pre_done,c2_l1pre_done,c2_l1post_done,c2_l1post_done);

					//C2_L1PRE_DONE : L1 pre done signal(0 : not-done, 1 : done)
					//C2_L1POST_DONE : L1 post done signal(0 : not-done, 1 : done)

					T2TC2_Eq_Ti_RdFL(t2tc2_eq_ti_mon31);  //regAddr = 0x24df, T2TC2_Eq_TI_MON[31]
					T2TC2_Eq_Ti_Rd01(t2tc2_eq_ti_mon31,fecdecen,fecdecen); //FECDECEN
					if(c2_l1pre_done & c2_l1post_done & fecdecen) tempLockStatus = 1;
					else tempLockStatus = 0;

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					DEMOD_PRINT("[DEMOD H14](%s:%d) Invalid Operation mode !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
			break;

		case LX_DEMOD_FEC_LOCK:
			{
				if(operMode == LX_DEMOD_DVBC) //DVB-C
				{
					C_Sync_RdFL(c_sync_mon23);  //regAddr = 0x1897,C_SYNC_MON[23]
					C_Sync_Rd01(c_sync_mon23,frmlock_c,frmlock_c); //FRMLOCK_C
					if(frmlock_c) tempLockStatus = 1;
					else tempLockStatus = 0;

					//FRMLOCK_C : Frame Lock Indicator
					// 0 : Unlock
					// 1 : Lock

				}
				else
				{
					tempLockStatus = 0;
					*lockState = LX_DEMOD_UNKOWN_STATE;
					DEMOD_PRINT("[DEMOD H14](%s:%d) Invalid Operation mode !!!\n", __F__, __L__);
					return RET_ERROR;
				}

			}
			break;

		default:
			*lockState = LX_DEMOD_UNKOWN_STATE;
			DEMOD_PRINT("[DEMOD H14](%s:%d) Invalid whatLock !!!\n", __F__, __L__);
			return RET_ERROR;
	}

	if (tempLockStatus == 1 )
	{
		 *lockState = LX_DEMOD_LOCKED;
		 DEMOD_PRINT("[DEMOD H14](%s:%d) LX_DEMOD_LOCKED !!!\n", __F__, __L__);
	}
	else
	{
		*lockState = LX_DEMOD_UNLOCKED;
		DEMOD_PRINT("[DEMOD H14](%s:%d) LX_DEMOD_UNLOCKED !!!\n", __F__, __L__);
	}
	return RET_OK;

}








