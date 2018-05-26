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
 *  application interface header for demod device
 *
 *  author		jeongpil.yun (jeongpil.yun@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *
 *  @addtogroup lg1150_demod
 *	@{
 */

#ifndef	_DEMOD_KAPI_H_
#define	_DEMOD_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	DEMOD_IOC_MAGIC		'd'
/**
@name DEMOD IOCTL List
ioctl list for demod device.

@{
@def DEMOD_IO_HW_RESET
Haredware Reset demod module(CTR61[0]).
When demod device receives above IOCTL, then reset itself.

@def DEMOD_IO_I2C_SEL
Set I2C sel( CTR26[15]) for demod.
Set to '0' in normal mode. If set to '1' it is in standalone mode.

@def DEMOD_IOW_INCLK_SEL
Select input clock source(CTR35[0]) for demod.
Set to '0' for ARM setting(default). If set to '1', the external oscillator is used as clock source for demod.

@def DEMOD_IOW_BBMODE_SEL (CTR61[2:1])
control demodulation mode.
if set to '00', the mode is unknown.
if set to '01', the mode is VSB/QAM.
if set to '10', the mode is DVB-T.
if set to '11', the mode is DVB-C.
*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
//common  (33)

#define	DEMOD_IO_HW_RESET					_IO(DEMOD_IOC_MAGIC,  0 )
#define DEMOD_IOW_I2C_OPEN					_IOW(DEMOD_IOC_MAGIC, 1, LX_DEMOD_I2C_DESC_T )
#define DEMOD_IO_I2C_CLOSE					_IO(DEMOD_IOC_MAGIC, 2 )
#define DEMOD_IOW_MEDIA_CHANGE				_IOW(DEMOD_IOC_MAGIC, 3, LX_DEMOD_INIT_PARAM_T )
#define DEMOD_IOW_CHANNEL_CHANGE			_IOW(DEMOD_IOC_MAGIC, 4, LX_DEMOD_CONFIG_T )
#define DEMOD_IOW_SERIAL_CONTROL			_IOW(DEMOD_IOC_MAGIC, 5, BOOLEAN )
#define DEMOD_IOW_SET_IFAGC_POL				_IOW(DEMOD_IOC_MAGIC, 6, LX_DEMOD_AGC_POLAR_T )

#define DEMOD_IOR_GET_CHANNEL_INFO			_IOR(DEMOD_IOC_MAGIC, 7, LX_DEMOD_CONFIG_T )
#define DEMOD_IOR_GET_SIGNAL_STATUS			_IOR(DEMOD_IOC_MAGIC, 8, LX_DEMOD_SIGNAL_CTX_T )

#define DEMOD_IOR_I2C_READ					_IOR(DEMOD_IOC_MAGIC, 9, LX_DEMOD_I2C_COMM_T )
#define DEMOD_IOW_I2C_WRITE					_IOW(DEMOD_IOC_MAGIC, 10, LX_DEMOD_I2C_COMM_T )

#define DEMOD_IOW_REPEATER_ENB				_IOW(DEMOD_IOC_MAGIC, 31, BOOLEAN )
#define DEMOD_IOW_POWER_SAVE				_IOW(DEMOD_IOC_MAGIC, 32, BOOLEAN )
#define DEMOD_IOW_TPOUT_ENB					_IOW(DEMOD_IOC_MAGIC, 33, BOOLEAN )
#define DEMOD_IOW_SET_IF_FREQ				_IOW(DEMOD_IOC_MAGIC, 34, UINT32 )
#define DEMOD_IOW_SET_SPECTRUM_CTRL			_IOW(DEMOD_IOC_MAGIC, 35, LX_DEMOD_SPECTRUM_CTRL_T )
#define DEMOD_IOR_GET_ID					_IOR(DEMOD_IOC_MAGIC, 36, UINT16 )
#define DEMOD_IOR_GET_SPECTRUM_CTRL			_IOR(DEMOD_IOC_MAGIC, 37, LX_DEMOD_SPECTRUM_CTRL_T )
#define DEMOD_IOR_GET_IFAGC					_IOR(DEMOD_IOC_MAGIC, 38, UINT16 )
#define DEMOD_IOR_GET_OPERATION_MODE		_IOR(DEMOD_IOC_MAGIC, 39, LX_DEMOD_OPER_MODE_T )
#define DEMOD_IOR_GET_LOCK_STATUS_NOTIFY	_IOR(DEMOD_IOC_MAGIC, 40, LX_DEMOD_FLAG_T )
#define DEMOD_IOR_GET_OFFSETFRQ				_IOR(DEMOD_IOC_MAGIC, 41, SINT32 )
#define DEMOD_IOR_GET_TPIF_ENB				_IOR(DEMOD_IOC_MAGIC, 42, BOOLEAN )
#define DEMOD_IOR_GET_VABER					_IOR(DEMOD_IOC_MAGIC, 43, UINT16 )
#define DEMOD_IOR_GET_MSE_INFO				_IOR(DEMOD_IOC_MAGIC, 44, LX_DEMOD_MSE_T )
#define DEMOD_IOR_GET_PKERROR				_IOR(DEMOD_IOC_MAGIC, 45, UINT8 )
#define DEMOD_IOR_GET_BANDWIDTH				_IOR(DEMOD_IOC_MAGIC, 46, LX_DEMOD_BWMODE_T )
#define DEMOD_IOR_GET_QAM_MODE				_IOR(DEMOD_IOC_MAGIC, 47, LX_DEMOD_RF_MODE_T )
#define DEMOD_IOR_GET_LOCK					_IOR(DEMOD_IOC_MAGIC, 48, LX_DEMOD_LOCK_STATE_T )
#define DEMOD_IO_SOFT_RESET					_IO(DEMOD_IOC_MAGIC, 49 )
#define DEMOD_IOW_MONITOR_ENB				_IOW(DEMOD_IOC_MAGIC, 50, BOOLEAN )

#define DEMOD_IOW_ADC_CTRL_FOR_TEST			_IOW(DEMOD_IOC_MAGIC, 51, LX_DEMOD_ADC_VOLTAGE_SEL_T )
#define DEMOD_IOW_PRINT_ON_OFF				_IOW(DEMOD_IOC_MAGIC, 52, UINT32 )
#define DEMOD_IOR_GET_SYSLOCKTIME			_IOW(DEMOD_IOC_MAGIC, 53, UINT32 )
#define DEMOD_IOR_GET_FECLOCKTIME			_IOW(DEMOD_IOC_MAGIC, 54, UINT32 )

//DVBC (8)

#define DEMOD_IOW_DVBC_SET_SYM_RATE			_IOW(DEMOD_IOC_MAGIC, 81, LX_DEMOD_SYMBOL_CTX_T )
#define DEMOD_IOW_DVBC_SET_CONFIG_AUTO		_IOW(DEMOD_IOC_MAGIC, 82, BOOLEAN )
#define DEMOD_IOW_DVBC_SET_QAM_MODE			_IOW(DEMOD_IOC_MAGIC, 83, UINT32 )
#define DEMOD_IOW_DVBC_SET_NEVERLOCK_TIME	_IOW(DEMOD_IOC_MAGIC, 84, UINT32 )
#define DEMOD_IOR_DVBC_GET_SYM_STATUS		_IOR(DEMOD_IOC_MAGIC, 85, LX_DEMOD_SYMBOLRATE_STATE_T )
#define DEMOD_IOR_DVBC_GET_QAM_STATUS		_IOR(DEMOD_IOC_MAGIC, 86, LX_DEMOD_QAMMODE_STATE_T )
#define DEMOD_IO_DVBC_PRINT_INFO			_IO(DEMOD_IOC_MAGIC, 87)
#define DEMOD_IO_DVBC_SET_DEFAULT_VALUE		_IO(DEMOD_IOC_MAGIC, 88 )

//DVBC2(3)
#define DEMOD_IOR_DVBC2_GET_MULTIPLP_INFO	_IOW(DEMOD_IOC_MAGIC, 89, LX_DEMOD_DVBC2_MULTI_PLP_INFO_T )
#define DEMOD_IO_DVBC2_PRINT_INFO			_IO(DEMOD_IOC_MAGIC, 90)
#define DEMOD_IOW_DVBC2_SET_CONFIG_PARTIAL	_IOW(DEMOD_IOC_MAGIC, 91, LX_DEMOD_DVBC2_CONFIG_T )


//DVBT(7)
#define DEMOD_IO_DVBT_SET_DEFAULT_VALUE		_IO(DEMOD_IOC_MAGIC, 121 )
#define DEMOD_IOW_DVBT_SET_CONFIG_AUTO		_IOW(DEMOD_IOC_MAGIC, 122, BOOLEAN )
#define DEMOD_IOW_DVBT_SET_CONFIG_PARTIAL	_IOW(DEMOD_IOC_MAGIC, 123, LX_DEMOD_DVBT_CONFIG_T )
#define DEMOD_IOW_DVBT_SET_CONFIG_FULL		_IOW(DEMOD_IOC_MAGIC, 124, LX_DEMOD_DVBT_CONFIG_T )
#define DEMOD_IOW_DVBT_SET_NEVERLOCK_TIME	_IOW(DEMOD_IOC_MAGIC, 125, UINT32 )
#define DEMOD_IO_DVBT_PRINT_INFO			_IO(DEMOD_IOC_MAGIC, 126)
#define DEMOD_IOR_DVBT_GET_CELL_ID			_IOR(DEMOD_IOC_MAGIC, 127, UINT16)


//DVBT2(3)
#define DEMOD_IOR_DVBT2_GET_MULTIPLP_INFO	_IOW(DEMOD_IOC_MAGIC, 140, LX_DEMOD_DVBT2_MULTI_PLP_INFO_T )
#define DEMOD_IO_DVBT2_PRINT_INFO			_IO(DEMOD_IOC_MAGIC, 141)
#define DEMOD_IOW_DVBT2_SET_CONFIG_PARTIAL	_IOW(DEMOD_IOC_MAGIC, 142, LX_DEMOD_DVBT2_CONFIG_T )


//VSB(4)
#define DEMOD_IO_VSB_SET_DEFAULT_VALUE		_IO(DEMOD_IOC_MAGIC, 151 )
#define DEMOD_IO_VSB_PRINT_INFO				_IO(DEMOD_IOC_MAGIC, 152 )
#define DEMOD_IOR_GET_MSE_DYN				_IOR(DEMOD_IOC_MAGIC, 153, BOOLEAN )
#define DEMOD_IOR_IS_COCHANNEL				_IOR(DEMOD_IOC_MAGIC, 154, BOOLEAN )

//QAM (2)
#define DEMOD_IO_QAM_SET_DEFAULT_VALUE		_IO(DEMOD_IOC_MAGIC, 171 )
#define DEMOD_IO_QAM_PRINT_INFO				_IO(DEMOD_IOC_MAGIC, 172 )


//ISDBT (6)
#define DEMOD_IO_ISDBT_SET_DEFAULT_VALUE	_IO(DEMOD_IOC_MAGIC, 191 )
#define DEMOD_IOW_ISDBT_SET_CONFIG_AUTO		_IOW(DEMOD_IOC_MAGIC, 192, BOOLEAN )
#define DEMOD_IOW_ISDBT_SET_CONFIG_PARTIAL	_IOW(DEMOD_IOC_MAGIC, 193, LX_DEMOD_ISDBT_CONFIG_T )
#define DEMOD_IOW_ISDBT_SET_CONFIG_FULL		_IOW(DEMOD_IOC_MAGIC, 194, LX_DEMOD_ISDBT_CONFIG_T )
#define DEMOD_IOW_ISDBT_SET_NEVERLOCK_TIME	_IOW(DEMOD_IOC_MAGIC, 195, UINT32 )
#define DEMOD_IO_ISDBT_PRINT_INFO			_IO(DEMOD_IOC_MAGIC, 196)


//ANALOG ()
#define DEMOD_IOW_ANALOG_I2C_OPEN					_IOW(DEMOD_IOC_MAGIC, 211, LX_DEMOD_I2C_DESC_T )
#define DEMOD_IO_ANALOG_I2C_CLOSE					_IO(DEMOD_IOC_MAGIC, 212 )
#define DEMOD_IOW_ANALOG_DEMOD_OPEN					_IOW(DEMOD_IOC_MAGIC, 213, UINT32 )
#define DEMOD_IOW_ANALOG_INIT						_IOW(DEMOD_IOC_MAGIC, 214, LX_DEMOD_RF_MODE_T )
#define DEMOD_IO_ANALOG_SOFT_RESET					_IO(DEMOD_IOC_MAGIC, 215 )
#define DEMOD_IOW_ANALOG_SET_CVDLOCK				_IOW(DEMOD_IOC_MAGIC, 216, BOOLEAN )
#define DEMOD_IOW_ANALOG_SET_AFT_RANGE				_IOW(DEMOD_IOC_MAGIC, 217, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T )
#define	DEMOD_IO_HW_RESET_ABB						_IO(DEMOD_IOC_MAGIC,  218 )
#define DEMOD_IO_ANALOG_Set_WORK_AROUND				_IO(DEMOD_IOC_MAGIC,  219 )
#define DEMOD_IOW_ANALOG_SET_RATE_CONV_CTRL			_IOW(DEMOD_IOC_MAGIC, 220, BOOLEAN )
#define DEMOD_IOW_ANALOG_SET_HIGH_RATE_CONV_OFFSET	_IOW(DEMOD_IOC_MAGIC, 221, BOOLEAN )
#define DEMOD_IOW_ANALOG_SET_SIF_CTRL				_IOW(DEMOD_IOC_MAGIC, 222, BOOLEAN )
#define DEMOD_IOW_ANALOG_SET_CVBS_DEC_SEL			_IOW(DEMOD_IOC_MAGIC, 223, LX_DEMOD_ANALOG_CVBSDEC_SEL_T )
#define DEMOD_IOW_ANALOG_SET_SIF_PATH_CTRL			_IOW(DEMOD_IOC_MAGIC, 224, LX_DEMOD_ANALOG_SIFPATH_SEL_T )
#define DEMOD_IOW_ANALOG_SET_SPECIAL_AUD_MODE		_IOW(DEMOD_IOC_MAGIC, 225, LX_DEMOD_ANALOG_SIFPATH_SEL_T )

#define DEMOD_IOR_ANALOG_I2C_READ					_IOR(DEMOD_IOC_MAGIC, 230, LX_DEMOD_I2C_COMM_T )
#define DEMOD_IOW_ANALOG_I2C_WRITE					_IOW(DEMOD_IOC_MAGIC, 231, LX_DEMOD_I2C_COMM_T )
#define DEMOD_IO_ANALOG_I2C_REGDUMP					_IO(DEMOD_IOC_MAGIC, 232 )
#define DEMOD_IOW_ANALOG_SET_ABB_MODE				_IOW(DEMOD_IOC_MAGIC, 233, BOOLEAN )
#define DEMOD_IOW_ANALOG_DEBUG_ABB					_IOW(DEMOD_IOC_MAGIC, 234, LX_ADEMOD_DEBUG_COMMAND_T)
#define DEMOD_IOW_ANALOG_DEBUG_SET_IF_FREQ			_IOW(DEMOD_IOC_MAGIC, 235, LX_DEMOD_ANALOG_PLL_PARAM_T )
#define DEMOD_IOW_ANALOG_DEBUG_SET_SPECTRUMINV		_IOW(DEMOD_IOC_MAGIC, 236, BOOLEAN )




#define DEMOD_IOC_MAXNR				250//255
/** @} */




/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * demod ioctl parameter.
 * useful when complex parameter should be passed to demod kernel driver.
 * @see DEMOD_IOW_WRITE_STRUCT
 */
typedef enum
{
	LX_DEMOD_ARM_SETTING = 0,   ///< Selected if internal OSC is used
	LX_DEMOD_EXTERN_PIN         ///< Selected if External OSC is used
} LX_DEMOD_INCLK_SEL_T;


typedef struct
{
	UINT8					rsvd[7];					/**< reserved area */
}
LX_DEMOD_CFG_T;

typedef struct
{
	UINT32				slvAddr;
	UINT8				portI2C;
} LX_DEMOD_I2C_DESC_T;


#define LX_DEMOD_IF_FREQ_05_000_KHZ			5000
#define LX_DEMOD_IF_FREQ_06_000_KHZ			6000

#define LX_DEMOD_IF_FREQ_45_750_KHZ		 	45750
#define LX_DEMOD_IF_FREQ_46_250_KHZ		 	46250
#define LX_DEMOD_IF_FREQ_46_750_KHZ		 	46750

#define LX_DEMOD_IF_FREQ_42_250_KHZ		 	42250
#define LX_DEMOD_IF_FREQ_41_750_KHZ		 	41750
#define LX_DEMOD_IF_FREQ_41_250_KHZ		 	41250

#define LX_DEMOD_IF_FREQ_37_750_KHZ		 	37750
#define LX_DEMOD_IF_FREQ_38_250_KHZ		 	38250
#define LX_DEMOD_IF_FREQ_38_750_KHZ		 	38750

#define LX_DEMOD_IF_FREQ_34_250_KHZ		 	34250
#define LX_DEMOD_IF_FREQ_33_750_KHZ		 	33750
#define LX_DEMOD_IF_FREQ_33_250_KHZ		 	33250

#define LX_DEMOD_NOT_LOCKED_PAUSE_TIME					30		/* pause 30 msec  in NOT_LOCKED state */
#define LX_DEMOD_LOCKED_PAUSE_TIME						600		/* pause 600msec  in LOCKED state */
#define LX_DEMOD_LOCK_FAIL_PAUSE_TIME					300		/* pause 300msec  in LOCKED state */
#define LX_DEMOD_FOREVER_PAUSE_TIME						0xFFFFFFFFFFFFFFFFULL       /* pause forever in Analog_IDLE state */


#define LX_DEMOD_PACKET_ERROR_THD 				200
#define LX_DEMOD_PACKET_ERROR_THD_32700 		32700
#define LX_DEMOD_PACKET_ERROR_THD_100			100

#define LX_DEMOD_DVBC_NEVERLOCK_WAIT_TIME		200 //QAM mode check time
#define LX_DEMOD_DVBT_NEVERLOCK_WAIT_TIME		500 //1500//500
#define LX_DEMOD_ISDBT_NEVERLOCK_WAIT_TIME		450
#define LX_DEMOD_QAM_NEVERLOCK_WAIT_TIME		500
#define LX_DEMOD_VSB_NEVERLOCK_WAIT_TIME		500

#define LX_DEMOD_VSB_SIG_STABLE_WAIT_TIMEOUT	80
#define LX_DEMOD_QAM_SIG_STABLE_WAIT_TIMEOUT	30
#define LX_DEMOD_ISDBT_SIG_STABLE_WAIT_TIMEOUT	30
#define LX_DEMOD_DVBT_SIG_STABLE_WAIT_TIMEOUT	30
#define LX_DEMOD_ANALOG_SIG_STABLE_WAIT_TIMEOUT	30



#define LX_DEMOD_DVB_DVBC_CONST_16QAM			0		///< DVBC QAM modulation mode -> QAM16
#define LX_DEMOD_DVB_DVBC_CONST_32QAM			1		///< DVBC QAM modulation mode -> QAM32
#define LX_DEMOD_DVB_DVBC_CONST_64QAM			2		///< DVBC QAM modulation mode -> QAM64
#define LX_DEMOD_DVB_DVBC_CONST_128QAM			3		///< DVBC QAM modulation mode -> QAM128
#define LX_DEMOD_DVB_DVBC_CONST_256QAM			4		///< DVBC QAM modulation mode -> QAM256

#define LX_DEMOD_LOCK_DEALY_50		50
#define LX_DEMOD_LOCK_DEALY_100		100
#define LX_DEMOD_LOCK_DEALY_150		150
#define LX_DEMOD_LOCK_DEALY_170		170
#define LX_DEMOD_LOCK_DEALY_200		200
#define LX_DEMOD_LOCK_DEALY_220		220
#define LX_DEMOD_LOCK_DEALY_250		250
#define LX_DEMOD_LOCK_DEALY_300		300
#define LX_DEMOD_LOCK_DEALY_400		400
#define LX_DEMOD_LOCK_DEALY_500		500
#define LX_DEMOD_LOCK_DEALY_450		450
#define LX_DEMOD_LOCK_DEALY_600		600
#define LX_DEMOD_LOCK_DEALY_800		800

#define LX_DEMOD_LOCK_DEALY_1000		1000
#define LX_DEMOD_LOCK_DEALY_1100		1100
#define LX_DEMOD_LOCK_DEALY_1500		1500
#define LX_DEMOD_LOCK_DEALY_2000		2000
#define LX_DEMOD_LOCK_DEALY_2500		2500
#define LX_DEMOD_LOCK_DEALY_3000		3000
#define LX_DEMOD_LOCK_DEALY_3500		3500
#define LX_DEMOD_LOCK_DEALY_4000		4000
#define LX_DEMOD_LOCK_DEALY_4500		4500
#define LX_DEMOD_LOCK_DEALY_5000		5000

#define	LX_DEMOD_SYMBOL_RATE_5056_KHZ	5056
#define	LX_DEMOD_SYMBOL_RATE_5309_KHZ	5309
#define	LX_DEMOD_SYMBOL_RATE_5360_KHZ	5360
#define	LX_DEMOD_SYMBOL_RATE_5957_KHZ	5957
#define	LX_DEMOD_SYMBOL_RATE_6000_KHZ	6000
#define	LX_DEMOD_SYMBOL_RATE_6111_KHZ	6111
#define	LX_DEMOD_SYMBOL_RATE_6125_KHZ	6125
#define	LX_DEMOD_SYMBOL_RATE_6875_KHZ	6875
#define	LX_DEMOD_SYMBOL_RATE_6900_KHZ	6900
#define	LX_DEMOD_SYMBOL_RATE_6950_KHZ	6950
#define	LX_DEMOD_SYMBOL_RATE_6957_KHZ	6957 /* = 8/115 * 100, DVB-C Annec A Max Symbol Rate */
#define	LX_DEMOD_SYMBOL_RATE_UNKNOWN	0xFFFF


#define	LX_DEMOD_ANALOG_SYMBOL_RATE_24000_KHZ	24000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_25000_KHZ	25000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_26000_KHZ	26000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_27000_KHZ	27000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_28000_KHZ	28000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_29000_KHZ	29000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_30000_KHZ	30000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_31000_KHZ	31000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_32000_KHZ	32000
#define	LX_DEMOD_ANALOG_SYMBOL_RATE_33000_KHZ	33000


/*
**  Sampling rates
*/
#define LX_ADEMOD_SAMPLING_RATE_NONE			(0)
#define LX_ADEMOD_SAMPLING_RATE_240MHz			(240000000L)
#define LX_ADEMOD_SAMPLING_RATE_250MHz			(250000000L)
#define LX_ADEMOD_SAMPLING_RATE_248MHz			(248000000L)
#define LX_ADEMOD_SAMPLING_RATE_252MHz			(252000000L)
#define LX_ADEMOD_SAMPLING_RATE_256MHz			(256000000L)
#define LX_ADEMOD_SAMPLING_RATE_260MHz			(260000000L)
#define LX_ADEMOD_SAMPLING_RATE_264MHz			(264000000L)
#define LX_ADEMOD_SAMPLING_RATE_268MHz			(268000000L)
#define LX_ADEMOD_SAMPLING_RATE_270MHz			(270000000L)
#define LX_ADEMOD_SAMPLING_RATE_272MHz			(272000000L)
#define LX_ADEMOD_SAMPLING_RATE_276MHz			(276000000L)
#define LX_ADEMOD_SAMPLING_RATE_280MHz			(280000000L)
#define LX_ADEMOD_SAMPLING_RATE_284MHz			(284000000L)
#define LX_ADEMOD_SAMPLING_RATE_288MHz			(288000000L)
#define LX_ADEMOD_SAMPLING_RATE_292MHz			(292000000L)
#define LX_ADEMOD_SAMPLING_RATE_296MHz			(296000000L)
#define LX_ADEMOD_SAMPLING_RATE_300MHz			(300000000L)
#define LX_ADEMOD_SAMPLING_RATE_304MHz			(304000000L)


#define LX_DEMOD_ANALOG_AFT_NORMAL_LIMIT		2000
#define LX_DEMOD_ANALOG_PAL_B_7MHZ_BOUNDARY		300000 //KHz
#define LX_ABB_M14_DEVICE     (0x11500000)



/*----------------------------------------------------------------------------------------
    Type Definitions (enum)
----------------------------------------------------------------------------------------*/


/**
 * Operation mode selection .
 */
typedef enum
{
	/* TERRESTRIAL */
	LX_DEMOD_DVBT = 0,			///< Oper mode -> DVBT
	LX_DEMOD_DVBT2,				///< Oper mode -> DVBT2
	LX_DEMOD_ATSC,				///< Oper mode -> ATSC
	LX_DEMOD_ISDBT, 			///< Oper mode -> ISDBT
	LX_DEMOD_DTMB,				///< Oper mode -> DTMB

	/* CABLE */
	LX_DEMOD_DVBC,				///< Oper mode -> DVBC
	LX_DEMOD_DVBC2,				///< Oper mode -> DVBC2
	LX_DEMOD_QAM,				///< Oper mode -> QAM
	LX_DEMOD_ISDBC, 			///< Oper mode -> ISDBC

	/* SATELLITE */
	LX_DEMOD_DVBS,						///< Oper mode -> DVBS
	LX_DEMOD_DVBS2,						///< Oper mode -> DVBS2
	LX_DEMOD_BS,						///< Oper mode -> BS
	LX_DEMOD_CS,						///< Oper mode -> CS

	LX_DEMOD_ANALOG,					///< Oper mode -> ANALOG

	LX_DEMOD_OPER_END,						///< Oper mode -> END

	LX_DEMOD_ANALOG_Sleep,				///< Oper mode -> ANALOG Sleep
	LX_DEMOD_DIGITAL_Sleep,				///< Oper mode -> DIGTITAL Sleep
	
	LX_DEMOD_UNKOWN_OPMODE		///< Oper mode ->  UNKNOWN
} LX_DEMOD_OPER_MODE_T;


/**
 * tune mode.
 */
typedef enum
{
	/*	---------- NORMAL 	---------------------	*/
	LX_DEMOD_TUNE_NORMAL					= 0x10,		/* 0001 XXXX */
		LX_DEMOD_TUNE_NOR_CHANNEL,
		LX_DEMOD_TUNE_NOR_FREQUENCY,
		LX_DEMOD_TUNE_NOR_FINE,				/* ATV : No AFT */

	/*	---------- SCAN 	---------------------	*/
	LX_DEMOD_TUNE_MANUAL					= 0x20,		/* 0010 XXXX */
		LX_DEMOD_TUNE_MAN_CHANNEL,
		LX_DEMOD_TUNE_MAN_FREQUENCY,
		LX_DEMOD_TUNE_MAN_SYSTEM,				/* ATV : PAL */
		LX_DEMOD_TUNE_MAN_FINE_UP,			/* ATV : PAL */
		LX_DEMOD_TUNE_MAN_FINE_DN,			/* ATV : PAL */
		LX_DEMOD_TUNE_MAN_SEARCH_UP,			/* ATV : PAL */
		LX_DEMOD_TUNE_MAN_SEARCH_DN,			/* ATV : PAL */

	LX_DEMOD_TUNE_AUTO						= 0x30,		/* 0011 XXXX */
		LX_DEMOD_TUNE_AUTO_FULL,
		LX_DEMOD_TUNE_AUTO_CHANNEL,
		LX_DEMOD_TUNE_AUTO_FREQUENCY,
		LX_DEMOD_TUNE_AUTO_START,				/* ATV : Full Scan */
		LX_DEMOD_TUNE_AUTO_CONTINUE,			/* ATV : Full Scan */

	/*	---------- SPECIFIC 	---------------------	*/
	LX_DEMOD_TUNE_SPECIFIC					= 0x40,		/* 0100 XXXX */
		LX_DEMOD_TUNE_SPEC_DVBT_HMLP,			/* DVBT : Hierarchy Mode */
		LX_DEMOD_TUNE_SPEC_DVBC_FIXED_DATA,	/* DVBC : Use Fixed NIT data */

	/*	---------- OTHERS 	---------------------	*/
	LX_DEMOD_TUNE_STOP						= 0x50, 	/* 1001 XXXX */
	LX_DEMOD_TUNE_FINISH,								/* Don't use this */

	/*	---------- UNKNOWN 	---------------------	*/
	LX_DEMOD_TUNE_UNKNOWN					= 0x80,		/* 1000 XXXX */

	/*	---------- MASK 	---------------------	*/
	LX_DEMOD_TUNE_MODE_MASK					= 0xF0,		/* 1111 XXXX */
} LX_DEMOD_TUNE_MODE_T;


#define		LX_DEMOD_IS_TUNE_NORMAL(tuneMode)	(LX_DEMOD_TUNE_NORMAL	== (tuneMode & LX_DEMOD_TUNE_MODE_MASK))
#define		LX_DEMOD_IS_TUNE_MANUAL(tuneMode)	(LX_DEMOD_TUNE_MANUAL	== (tuneMode & LX_DEMOD_TUNE_MODE_MASK))
#define		LX_DEMOD_IS_TUNE_AUTO(tuneMode)	(LX_DEMOD_TUNE_AUTO 	== (tuneMode & LX_DEMOD_TUNE_MODE_MASK))
#define		LX_DEMOD_IS_TUNE_STOP(tuneMode)	(LX_DEMOD_TUNE_STOP 	== (tuneMode & LX_DEMOD_TUNE_MODE_MASK))

#define		LX_DEMOD_IS_TUNE_SCAN(tuneMode)	(LX_DEMOD_IS_TUNE_AUTO(tuneMode)||(LX_DEMOD_TUNE_MAN_SEARCH_UP == tuneMode)||\
										(LX_DEMOD_TUNE_MAN_SEARCH_DN == tuneMode ))


/**
 * DVBT/ ISDBT	FFT mode
 */
typedef enum
{
	LX_DEMOD_DVBT_ISDBT_FFT_1K, 		///< DVB-T FFT mode -> 1K
	LX_DEMOD_DVBT_ISDBT_FFT_2K, 		///< DVB-T FFT mode -> 2K
	LX_DEMOD_DVBT_ISDBT_FFT_4K, 		///< DVB-T FFT mode -> 4K , ISDBT only
	LX_DEMOD_DVBT_ISDBT_FFT_8K, 		///< DVB-T FFT mode -> 8K
	LX_DEMOD_DVBT_ISDBT_FFT_16K, 		///< DVB-T FFT mode -> 16K
	LX_DEMOD_DVBT_ISDBT_FFT_32K,		///< DVB-T FFT mode -> 32K
	LX_DEMOD_DVBT_ISDBT_FFT_UNKNOWN 	///< DVB-T FFT mode -> UNKNOWN
} LX_DEMOD_FFTMODE_T;

/**
 * DVBT Guard Interval
 */
typedef enum
{
	LX_DEMOD_DVBT_ISDBT_GIMODE_19_256,		///< DVB-T 2Guard Interval-> 19/256
	LX_DEMOD_DVBT_ISDBT_GIMODE_19_128,		///< DVB-T2 Guard Interval-> 19/128
	LX_DEMOD_DVBT_ISDBT_GIMODE_1_128,		///< DVB-T2 Guard Interval-> 1/128
	LX_DEMOD_DVBT_ISDBT_GIMODE_1_32,		///< DVB-T Guard Interval-> 1/32
	LX_DEMOD_DVBT_ISDBT_GIMODE_1_16,		///< DVB-T Guard Interval-> 1/16
	LX_DEMOD_DVBT_ISDBT_GIMODE_1_8, 	///< DVB-T Guard Interval-> 1/8
	LX_DEMOD_DVBT_ISDBT_GIMODE_1_4, 	///< DVB-T Guard Interval-> 1/4
	LX_DEMOD_DVBT_ISDBT_GIMODE_UNKNOWN	///< DVB-T Guard Interval-> UNKNOWN
} LX_DEMOD_GIMODE_T;

/**
 * TP Error Count Display mode .
 */
typedef enum
{
	LX_DEMOD_I2C_READ_BASED,			///< TP Error Count Display mode -> I2C read based
	LX_DEMOD_PERMILLAGE,				///< TP Error Count Display mode -> permillage based
	LX_DEMOD_PERCENTAGE,				///< TP Error Count Display mode -> Percentage based
	LX_DEMOD_EVERY_1_SECOND 			///< TP Error Count Display mode -> Every 1 second duration

} LX_DEMOD_TPERRCNTMODE_T;

/**
 * ADC voltage selection .
 */
typedef enum
{
	LX_DEMOD_ADC_SEL_1V,		///< ADC voltage 1V selection
	LX_DEMOD_ADC_SEL_2V 		///< ADC voltage 2V selection

} LX_DEMOD_ADC_VOLTAGE_SEL_T;

/**
 * DVB Bandwidth
 */
typedef enum
{
	LX_DEMOD_BW_1_7MHZ,			///< DVB-T2   Bandwidth -> 1.7MHz (fix)
	LX_DEMOD_BW_5MHZ,			///< DVB-T2   Bandwidth -> 5MHz (fix)
	LX_DEMOD_BW_6MHZ,			///< DVB-T/C Bandwidth -> 6MHz (fix)
	LX_DEMOD_BW_7MHZ,			///< DVB-T/C Bandwidth -> 7MHz (fix)
	LX_DEMOD_BW_8MHZ,			///< DVB-T/C Bandwidth -> 8MHz (fix)
	LX_DEMOD_BW_7M_6MHZ,		///< DVB-T/C Bandwidth -> 7MHz, 6MHz (auto-detection)
	LX_DEMOD_BW_8M_6MHZ,		///< DVB-T/C Bandwidth -> 8MHz, 6MHz (auto-detection)
	LX_DEMOD_BW_8M_7MHZ,		///< DVB-T/C Bandwidth -> 8MHz, 7MHz (auto-detection)
	LX_DEMOD_BW_8M_7M_6MHZ, 	///< DVB-T/C Bandwidth -> 8MHz , 7MHz and  6MHz (auto-detection)
	LX_DEMOD_DVBT_NO_OPERATION, ///< DVB-T/C Bandwidth -> No operation for DVBC
	LX_DEMOD_VSB_NO_OPERATION,	///< QAM Bandwidth -> No operation for ATSC

	LX_DEMOD_BW_UNKNOWN,		///< DVB-T/C Bandwidth -> UNKNOWN

} LX_DEMOD_BWMODE_T;


/**
 * Symbol rate control.
 */
typedef enum
{
	LX_DEMOD_SYMR_FIXED_MODE,					///< Symbol rate inversion value always comes from i2c register(0x39~0x3C) (For L9 B0, not used)
	LX_DEMOD_SYMR_FULL_SCAN_MODE,				///<For L9 B0, upto 2 favorite symbol rate and full scan detection of symbol rate
	LX_DEMOD_SYMR_UNKOWN_MODE,						///<Unkown mode
	LX_DEMOD_SYMR_DVBC_NO_OPERATION_MODE,				///< For L9 B0, no operation for cable standard
	LX_DEMOD_SYMR_QAM_NO_OPERATION_MODE,	///< For L9 B0, no operation for cable standard

} LX_DEMOD_SYMRDET_MODE_T;




/**
 *Lock Funcion Input parameter
 *	-  lock status and mode detection status .
 */
typedef enum
{
	LX_DEMOD_AGC_LOCK,			///<  AGC Lock status
	LX_DEMOD_TR_LOCK,			///< Timing Recovery LOCK status
	LX_DEMOD_CARR_LOCK, 		///< Carrier  Recovery LOCK status
	LX_DEMOD_TPS_LOCK,			///< DVBT TPS Lock status
	LX_DEMOD_TMCC_LOCK, 		///< ISDBT TPS Lock status
	LX_DEMOD_MODE_DET,			///< DVBT,ISDBT Mode Detection status
	LX_DEMOD_FEC_LOCK,			///< QAM,DVBC FEC Lock status
	LX_DEMOD_PSYNC_LOCK, 		///< DVBT, ATSC,  ISDBT SYNC Lock status
	LX_DEMOD_L1_DONE, 			///< DVBT2
	LX_DEMOD_FEC_DEC_EN, 		///< DVBT2

} LX_DEMOD_LOCK_STATUS_T;

/**
 *	lock status and mode detection status .
 */
typedef enum
{
	LX_DEMOD_UNLOCKED,						///< VQI Lock status -> UNLOCKED
	LX_DEMOD_LOCKED,						///< VQI Lock status -> LOCKED
	LX_DEMOD_IDLE,							///< VQI Mode detection status -> IDLE
	LX_DEMOD_DETECTED,						///< v Mode detection status -> Detected
	LX_DEMOD_DETECTION_PROCESSING,			///< VQI Mode detection status -> On processing
	LX_DEMOD_DETECTION_FAIL, 				///< VQI Mode detection status -> FAIL
	LX_DEMOD_DETECTION_MANUAL,				///< VQI Mode detection status -> Detection disabled
	LX_DEMOD_AUTO,							///< VQI Spectrm mode control -> auto
	LX_DEMOD_MANUAL,						///< VQI Spectrm mode control -> manual
	LX_DEMOD_INVERSION, 					///< VQI Spectrm mode status -> Inverted
	LX_DEMOD_NORMAL,						///< VQI I,Q Spectrum mode or I,Q input status -> normal
	LX_DEMOD_IQSWAP,						///< VQI I,Q input status -> Swapped
	LX_DEMOD_UNKOWN_STATE					///< VQI Unkown status

} LX_DEMOD_LOCK_STATE_T;

/**
 * AGC output polarization control.
 */
typedef enum
{
	LX_DEMOD_AGC_ACTIVE_HIGH,			///< Active High
	LX_DEMOD_AGC_ACTIVE_LOW,			///< Active Low
} LX_DEMOD_AGC_POLAR_T;


/**
 * RF modulation mode .
 */
typedef enum
{
	LX_DEMOD_16QAM = 0,			///< modulation type -> QAM16
	LX_DEMOD_32QAM,				///< modulation type -> QAM32
	LX_DEMOD_64QAM,				///< modulation type -> QAM64
	LX_DEMOD_128QAM,			///< modulation type -> QAM128
	LX_DEMOD_256QAM,			///< modulation type -> QAM256
	LX_DEMOD_1024QAM,			///< modulation type -> QAM1024
	LX_DEMOD_4096QAM,			///< modulation type -> QAM4096
	LX_DEMOD_QPSK,				///< modulation type -> QPSK
	LX_DEMOD_DQPSK,				///< modulation type -> DQPSK
	LX_DEMOD_VSB,				///< modulation type -> VSB
	LX_DEMOD_8PSK,
	LX_DEMOD_NTSC,				///< modulation type -> NTSC
	LX_DEMOD_PAL,				///< modulation type -> PAL
	LX_DEMOD_SECAM,				///< modulation type -> SECAM

	LX_DEMOD_MODE_END,
	LX_DEMOD_UNKOWN_MODE		///< modulation type -> UNKNOWN
} LX_DEMOD_RF_MODE_T;




/**
 * Code rate
 */
typedef enum
{
	LX_DEMOD_CODE_1_2,		///< code rate-> 1/2
	LX_DEMOD_CODE_2_3,		///< code rate-> 2/3
	LX_DEMOD_CODE_3_4,		///< code rate-> 3/4
	LX_DEMOD_CODE_3_5,		///< code rate-> 3/5
	LX_DEMOD_CODE_4_5,		///< code rate-> 4/5
	LX_DEMOD_CODE_5_6,		///< code rate-> 5/6
	LX_DEMOD_CODE_7_8,		///< code rate-> 7/8
	LX_DEMOD_CODE_UNKNOWN	///< code rate-> UNKNOWN
} LX_DEMOD_CODERATE_T;


/**
 * DVBT Hierachical mode
 */
typedef enum
{
	LX_DEMOD_DVB_TPS_HIERACHY_NONE,		///< DVBT Hierachical mode-> None hierarchical mode
	LX_DEMOD_DVB_TPS_HIERACHY_1,		///< DVBT Hierachical mode-> alpha = 1
	LX_DEMOD_DVB_TPS_HIERACHY_2,		///< DVBT Hierachical mode-> alpha = 2
	LX_DEMOD_DVB_TPS_HIERACHY_4,		///< DVBT Hierachical mode-> alpha = 4
	LX_DEMOD_DVB_TPS_HIERACHY_UNKNOWN	///< DVBT Hierachical mode-> alpha = UNKNOWN
} LX_DEMOD_DVB_TPS_HIERACHY_T;

/**
 * ISDBT Length of time interleaving
 */
typedef enum
{
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_0,			///< ISDBT The length of time interleaving is  0
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_1, 		///< ISDBT The length of time interleaving is  1
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_2, 		///< ISDBT The length of time interleaving is  2
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_4, 		///< ISDBT The length of time interleaving is  4
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_8, 		///< ISDBT The length of time interleaving is  8
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_16, 		///< ISDBT The length of time interleaving is  16
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_32, 		///< ISDBT The length of time interleaving is  32
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_reserved, 	///< ISDBT The length of time interleaving is  reserved
	LX_DEMOD_ISDBT_TIME_INTERLEAVING_UNUSED 	///< ISDBT The length of time interleaving is  un-used layer

} LX_DEMOD_ISDBT_TMCC_TIME_INT_LEN_T;


typedef enum
{
	LX_DEMOD_RETUNE_RF,
	LX_DEMOD_TUNE_START,
	LX_DEMOD_TUNE_SCAN,
	LX_DEMOD_TUNE_SCAN_FINISH_UNLOCK,
	LX_DEMOD_SYNC_CHECKING,
	LX_DEMOD_SIGNAL_STABLE,
	LX_DEMOD_SIGNAL_LOCKED


} LX_DEMOD_CONTROL_STATE_T;

typedef enum DEMOD_FLAG
{
	LX_DEMOD_FLAG_ON			= 0x30,
	LX_DEMOD_FLAG_OFF,

	LX_DEMOD_FLAG_VALID		= 0x40,
	LX_DEMOD_FLAG_INVALID,

	LX_DEMOD_FLAG_SUCCESS 	= 0x50,
	LX_DEMOD_FLAG_FAIL,

	LX_DEMOD_FLAG_ENABLE		= 0x60,
	LX_DEMOD_FLAG_DISABLE,

	LX_DEMOD_FLAG_START		= 0x90,
	LX_DEMOD_FLAG_ING,
	LX_DEMOD_FLAG_STOP,
	LX_DEMOD_FLAG_FINISH,
	LX_DEMOD_FLAG_COMPLETE,

	DEMOD_FLAG_UNKNOWN 	= 0xF0
} LX_DEMOD_FLAG_T;


/**
 * DEMOD Tuning Task Message Status
 *
 */
typedef  enum
{
	LX_DEMOD_TASK_NONE_REQUEST	= 0,	///< None
	LX_DEMOD_NEWTUNE_REQUEST	= 1,	///< New tune request
	LX_DEMOD_STOP_REQEST		= 2
} LX_DEMOD_TASK_MSG_T;

/**
 * DifineDEMOD state machine status
 *
 */
typedef  enum
{
	LX_DEMOD_TUNNING_PROCESS	= 0,	///< Tunning state machine
	LX_DEMOD_MONITOR_PROCESS	= 1,	///< monitoring state machine
	LX_DEMOD_IDLE_PROCESS		= 2
} LX_DEMOD_ACQUIRE_PROCESS_T;

/**
 @brief DVBT2 basic types of PLP available.
*/
typedef enum {
    LX_DEMOD_DVBT2_PLP_COMMON,                                  /**< DVBT2 basic PLP common type. */
    LX_DEMOD_DVBT2_PLP_DATA,                                     /**< DVBT2 basic PLP data type. */
    LX_DEMOD_DVBT2_PLP_UNKNOWN
} LX_DEMOD_DVBT2_PLP_BYTE_T;

/**
 @brief DVBC2 basic types of PLP available.
*/
typedef enum {
    LX_DEMOD_DVBC2_PLP_COMMON,                                  /**< DVBC2 basic PLP common type. */
    LX_DEMOD_DVBC2_PLP_DATA,                                     /**< DVBC2 basic PLP data type. */
    LX_DEMOD_DVBC2_PLP_UNKNOWN
} LX_DEMOD_DVBC2_PLP_BYTE_T;


/**
 * Analog Audio Sound System Type Definition
 */
typedef enum
{
	LX_DEMOD_ANALOG_SIF_SYSTEM_BG		= 0x00,	// keep the value. 	refer to IF_PLL_DATA_TYPE_t
	LX_DEMOD_ANALOG_SIF_SYSTEM_I,				// keep the value. 	refer to IF_PLL_DATA_TYPE_t
	LX_DEMOD_ANALOG_SIF_SYSTEM_DK,			// keep the value. 	refer to IF_PLL_DATA_TYPE_t
	LX_DEMOD_ANALOG_SIF_SYSTEM_L,				// keep the value. 	refer to IF_PLL_DATA_TYPE_t
	LX_DEMOD_ANALOG_SIF_SYSTEM_MN,			// keep the value.	refer to IF_PLL_DATA_TYPE_t
	LX_DEMOD_ANALOG_SIF_SYSTEM_LP,			/* 20080724 : Added by Goldman for FE/TU */
	LX_DEMOD_ANALOG_SIF_SYSTEM_END,			/* 20080724 : Added by Goldman for FE/TU */
 	LX_DEMOD_ANALOG_SIF_SYSTEM_UNKNOWN 	= 0xf0
} LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T;

/**
 *CVBS DEC select
 **/
typedef enum
{
	 LX_DEMOD_ANALOG_DTV,
	 LX_DEMOD_ANALOG_ATV,
	 LX_DEMOD_ANALOG_ATV_RC
} LX_DEMOD_ANALOG_CVBSDEC_SEL_T;



/**
 * audio system
 **/
typedef struct {
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem;
	char				*pszName;
} LX_DEMOD_ANALOG_NAME_AUDIO_SYSTEM_t;



/**
 * SIF path 
 */
typedef enum
{
	LX_DEMOD_ANALOG_SIF_PATH_DIRECT			= 0x00,	///< SIF path  control - 1
	LX_DEMOD_ANALOG_SIF_PATH_INTERNAL,				///< SIF path  control - 2
	LX_DEMOD_ANALOG_SIF_PATH_EXTERNAL,				///< SIF path  control - 3
	LX_DEMOD_ANALOG_SIF_PATH_FROM_EXT_TUNER,		///< SIF path  control - 4
 	LX_DEMOD_ANALOG_SIF_PATH_UNKNOWN 		= 0xf0

} LX_DEMOD_ANALOG_SIFPATH_SEL_T;





/*----------------------------------------------------------------------------------------
	Type Definitions(struct)
----------------------------------------------------------------------------------------*/

typedef struct
{
	UINT16 				address;
	UINT16 				nBytes;
	UINT8 				buf[4];
} LX_DEMOD_I2C_COMM_T;



/**
 * Init parameter control.
 */
typedef struct
{
	LX_DEMOD_OPER_MODE_T	operMode;
	LX_DEMOD_BWMODE_T		bandWd;
	LX_DEMOD_SYMRDET_MODE_T symbr;
	UINT32 					ifFrq;

}LX_DEMOD_INIT_PARAM_T;

/**
 * Lock parameter control.
 */
typedef struct
{
	LX_DEMOD_LOCK_STATUS_T whatLock;
	LX_DEMOD_LOCK_STATE_T lockState;

} LX_DEMOD_LOCK_PARAM_T;

/**
 * Spectrum mode
 *
 */

typedef  struct
{
	BOOLEAN bAutoEnable;
	BOOLEAN bSpecInvEnable;

}LX_DEMOD_SPECTRUM_CTRL_T;



/**
 * DVBT symbol lock status and symbol rate info.
 */
typedef struct LX_DEMOD_SYMBOLRATE_STATE
{
	LX_DEMOD_LOCK_STATE_T status;		///< DVB-T/C lock status and mode detection status
	UINT16 symbolRate;						///< Auto-detected symbol rate

} LX_DEMOD_SYMBOLRATE_STATE_T;

/**
 * DVB-T/C QAM mode status and QAM mode info.
 */
typedef struct LX_DEMOD_QAMMODE_STATE
{
	LX_DEMOD_LOCK_STATE_T status;		///< DVB-T/C lock status and mode detection status
	LX_DEMOD_RF_MODE_T	 qamMode;		///< Auto-detected QAM mode

} LX_DEMOD_QAMMODE_STATE_T;


/**
 * ISDBT TMCC config
 */
typedef struct
{
	///< Layer A signal
	LX_DEMOD_RF_MODE_T					layerAQamMode;
	LX_DEMOD_CODERATE_T			 		layerACodeRate;
	LX_DEMOD_ISDBT_TMCC_TIME_INT_LEN_T 	layerATimeInterleavingLen;
	UINT8 								layerASegmentNum;

	///< Layer B signal
	LX_DEMOD_RF_MODE_T					layerBQamMode;
	LX_DEMOD_CODERATE_T					layerBCodeRate;
	LX_DEMOD_ISDBT_TMCC_TIME_INT_LEN_T	layerBTimeInterleavingLen;
	UINT8								layerBSegmentNum;

	///< Layer C signal
	LX_DEMOD_RF_MODE_T	 				layerCQamMode;
	LX_DEMOD_CODERATE_T			 		layerCCodeRate;
	LX_DEMOD_ISDBT_TMCC_TIME_INT_LEN_T 	layerCTimeInterleavingLen;
	UINT8 								layerCSegmentNum;


} LX_DEMOD_ISDBT_TMCC_CONFIG_T;

/**
 * ISDBT total parameter info.
 */
typedef struct
{
	LX_DEMOD_BWMODE_T 				channelBW;						///< Channel Bandwidth
	LX_DEMOD_TUNE_MODE_T			tuneMode;						///< tune Mode
	BOOLEAN							bSpectrumInv;
	LX_DEMOD_FFTMODE_T 				FFTMode;						///< FFT mode
	LX_DEMOD_GIMODE_T 				gIMode;							///< Guard Interval mode
	LX_DEMOD_ISDBT_TMCC_CONFIG_T 	tmcc;							///< TMCC

}LX_DEMOD_ISDBT_CONFIG_T;

/**
 * Special data : DVB-C.
 */
typedef struct
{
	LX_DEMOD_TUNE_MODE_T	tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 		channelBW;						///< Channel Bandwidth
	BOOLEAN					bSpectrumInv;
	LX_DEMOD_RF_MODE_T		qamMode;						///< QAM mode
	UINT16					symbolRate;
	UINT32					frequency;						///< For DVBC2 - Frequency

} LX_DEMOD_DVBC_CONFIG_T;

/**
 * DVBC2  total parameter info.
 */
typedef struct
{
	LX_DEMOD_TUNE_MODE_T			tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 				channelBW;			///< Channel Bandwidth
	BOOLEAN							bSpectrumInv;
	LX_DEMOD_GIMODE_T 				gIMode;				///< Guard Interval mode
	LX_DEMOD_CODERATE_T 			codeRate;			///< High priority signal code rate at hierachical transmission modee
	LX_DEMOD_RF_MODE_T				qamMode;			///< QAM mode
	UINT8							groupID;			///< For DVBC2
	UINT8							sliceID;			///< For DVBC2 -SLICE ID
	UINT8							unPLP;				///< For DVBC2 - PLP ID
	UINT32							frequency;			///< For DVBC2 - Frequency
}LX_DEMOD_DVBC2_CONFIG_T;

/**
*DVBC2 : PLP info.
*/

typedef struct
{
	UINT8 						plpDataID;
	LX_DEMOD_DVBC2_PLP_BYTE_T 	type;

} LX_DEMOD_DVBC2_PLP_INFO_T;


typedef struct
{
	UINT8 plpTotalCount; /*count of multiple TS */

	UINT8 plpDatacount;
	UINT8 plpDataID[256]; /* array of PLP ID */

	UINT8 commonPlpCount;
	UINT8 commonPlpID[256];

	UINT8 sliceCount;
	UINT8 sliceID[256];

} LX_DEMOD_DVBC2_MULTI_PLP_INFO_T;




typedef struct
{
	LX_DEMOD_TUNE_MODE_T	tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 		channelBW;						///< Channel Bandwidth
	BOOLEAN					bSpectrumInv;
	LX_DEMOD_RF_MODE_T		qamMode;				///< QAM mode

} LX_DEMOD_QAM_CONFIG_T;

typedef struct
{
	LX_DEMOD_TUNE_MODE_T	tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 		channelBW;						///< Channel Bandwidth
	BOOLEAN					bSpectrumInv;

} LX_DEMOD_VSB_CONFIG_T;

/**
 * DVBT total parameter info.
 */
typedef struct
{
	LX_DEMOD_TUNE_MODE_T			tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 				channelBW;			///< Channel Bandwidth
	BOOLEAN							bSpectrumInv;
	BOOLEAN 						bSetHp;				///< High/Low priority signal path slection at hierachical transmission mode
	LX_DEMOD_FFTMODE_T 				FFTMode;			///< FFT mode
	LX_DEMOD_GIMODE_T 				gIMode;				///< Guard Interval mode
	LX_DEMOD_DVB_TPS_HIERACHY_T 	hierachMode;		///< Hierachical transmission mode
	LX_DEMOD_CODERATE_T 			lpCodeRate;			///< Low priority signal code rate at hierachical transmission mode
	LX_DEMOD_CODERATE_T 			hpCodeRate;			///< High priority signal code rate at hierachical transmission modee
	LX_DEMOD_RF_MODE_T				qamMode;			///< QAM mode

}LX_DEMOD_DVBT_CONFIG_T;

/**
 * DVBT2  total parameter info.
 */
typedef struct
{
	LX_DEMOD_TUNE_MODE_T			tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 				channelBW;			///< Channel Bandwidth
	BOOLEAN							bSpectrumInv;
	LX_DEMOD_FFTMODE_T 				FFTMode;			///< FFT mode
	LX_DEMOD_GIMODE_T 				gIMode;				///< Guard Interval mode
	LX_DEMOD_CODERATE_T 			codeRate;			///< High priority signal code rate at hierachical transmission modee
	LX_DEMOD_RF_MODE_T				qamMode;			///< QAM mode
	UINT8							groupID;			///< For DVBT2
	UINT8							unPLP;				///< For DVBT2 - PLP ID
}LX_DEMOD_DVBT2_CONFIG_T;



/**
*DVBT2 : PLP info.
*/

typedef struct
{
	UINT8 						plpDataID;
	LX_DEMOD_DVBT2_PLP_BYTE_T 	type;

} LX_DEMOD_DVBT2_PLP_INFO_T;


typedef struct
{
	UINT8 plpTotalCount; /*count of multiple TS */

	UINT8 plpDatacount;
	UINT8 plpDataID[256]; /* array of PLP ID */

	UINT8 commonPlpCount;
	UINT8 commonPlpID[256];

} LX_DEMOD_DVBT2_MULTI_PLP_INFO_T;

#ifndef Handle_t
typedef void *          Handle_t;       /*  memory pointer type             */
#endif


/**
*ANALOG
*/
typedef struct
{
	Handle_t							handleABB;						///< handler
	UINT32								frequency;						///< For ABB - Frequency
	UINT32								analogIF;						///< For ABB - internal operation IF Frequency
	UINT32								sampleRate;						///< For ABB - internal operation sampling Frequency
	UINT32								pllClkSource;					///< For ABB - internal operation sampling Frequency
	UINT32								tunedFreq;						///< For ABB - Tuned Frequency
	SINT32								freqOffset;						///< frequency offset
	LX_DEMOD_RF_MODE_T					rfMode;							///<RF mode
	LX_DEMOD_TUNE_MODE_T				tuneMode;						///< tune Mode
	LX_DEMOD_BWMODE_T 					channelBW;						///< Channel Bandwidth
	BOOLEAN								bSpectrumInv;					///< spectrum inversion
	BOOLEAN								cvdLock;						///< CVD Lock flag
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T	audioSystem;						///< audio mode

} LX_DEMOD_ANALOG_CONFIG_T;

/**
*ANALOG
*/
typedef struct
{
	UINT32					chipDevID;
	UINT32					handleI2C;


} LX_DEMOD_ANALOG_DEMOD_OPEN_PARAM_T;

typedef struct
{
	UINT32					lo;
	UINT32					hi;


} LX_DEMOD_ANALOG_ATFRANGE_PARAM_T;

typedef struct
{
	UINT32 pllClkSource;
	UINT32 sampleRate;
	UINT32 ifFrq;


} LX_DEMOD_ANALOG_PLL_PARAM_T;



/**
 * DEMOD total parameter info.
 */
typedef struct
{

	LX_DEMOD_OPER_MODE_T 	operMode;
	LX_DEMOD_DVBT_CONFIG_T 	dvbt;						///< DVBT
	LX_DEMOD_DVBT2_CONFIG_T dvbt2;						///< DVBT2
	LX_DEMOD_ISDBT_CONFIG_T isdbt;						///< ISDBT
	LX_DEMOD_DVBC_CONFIG_T 	dvbc;						///<DVBC
	LX_DEMOD_DVBC2_CONFIG_T	dvbc2;						///< DVBC2
	LX_DEMOD_QAM_CONFIG_T 	qam;						///< QAM
	LX_DEMOD_VSB_CONFIG_T		vsb;						///< VSB
	LX_DEMOD_ANALOG_CONFIG_T	analog;						///< ANALOG
}LX_DEMOD_CONFIG_T;


/**
 * signal state.
 */

typedef struct
{
	UINT16	mse;
	UINT16 	bMse;
	UINT16	constPwr;

}LX_DEMOD_MSE_T;

typedef struct
{
	LX_DEMOD_OPER_MODE_T 	operMode;
	LX_DEMOD_RF_MODE_T	 	rfMode;
	BOOLEAN 				bSignalValid;
	UINT32					packetError;
	UINT32					ber;			/* unit : e-10 */
	UINT16					agc;
	LX_DEMOD_MSE_T			 m;

} LX_DEMOD_SIGNAL_CTX_T;

/**
 * DVBC symbol rate structure.
 */

typedef struct
{

	LX_DEMOD_SYMRDET_MODE_T symbolMode;
	UINT32 					symbolRate;
}LX_DEMOD_SYMBOL_CTX_T;

/**
 * Nordig C/N reference spec according to both code rate and QAM mode.
 */
typedef struct
{
	LX_DEMOD_DVB_TPS_HIERACHY_T hierachMode;		///< Hierachical transmission mode
	BOOLEAN 					bSetHp;				///< High/Low priority signal path slection at hierachical transmission mode
	LX_DEMOD_RF_MODE_T			constel;			///< QAM mode
	LX_DEMOD_CODERATE_T			code_rate;			///< code rate
	UINT32						cn_ref;				///< Nordig C/N reference spec.
}LX_DEMOD_SQI_CN_NORDIGP1_T;

/**
 * DVBT.
 */

/**
 * CIR power level control.
 */
typedef enum
{
	LX_DEMOD_CIR_POWER_HIGH,			///< CIR power level control - High
	LX_DEMOD_CIR_POWER_MID,				///< CIR power level control - Middle
	LX_DEMOD_CIR_POWER_LOW,				///< CIR power level control - Low
	LX_DEMOD_CIR_POWER_UNKNOWN 		///< CIR power level control - unknown
} LX_DEMOD_CIR_POWER_CTRL_T;

/**
 * CFTNR  control.
 */
typedef enum
{
	LX_DEMOD_CTFNR_BYPASS,			///< CTFNR  control - Bypass
	LX_DEMOD_CTFNR_1_2,				///< CTFNR  control - 1/2
	LX_DEMOD_CTFNR_1_4,				///< CTFNR  control - 1/4
	LX_DEMOD_CTFNR_1_8, 			///< CTFNR  control - 1/8
	LX_DEMOD_CTFNR_1_16				///< CTFNR  control - 1/16
} LX_DEMOD_CTFNR_CTRL_T;


typedef struct 
{
    UINT32 id;
	UINT8  size;
    UINT32 value[5];
} LX_ADEMOD_DEBUG_COMMAND_T;

typedef struct
{
	Handle_t 							phDevice;						///< handler
	UINT32								frequency;						///< For ABB - Frequency
	UINT32								analogIF;						///< For ABB - internal operation IF Frequency
	UINT32								sampleRate;						///< For ABB - internal operation sampling Frequency
	LX_DEMOD_RF_MODE_T					rfMode;							///<RF mode
	LX_DEMOD_TUNE_MODE_T				tuneMode;						///< Channel Bandwidth
	LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T	audioSystem;					///< tune Mode

} LX_DEMOD_SMARTTUNE_PARAM_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_DRV_H_ */

/** @} */
