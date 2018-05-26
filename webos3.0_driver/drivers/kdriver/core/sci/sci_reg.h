/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  driver register interface header for sci device. ( used only within kdriver )
 *	this file lists exported function, types for the external modules.
 *
 *  @author		bongrae.cho (bongrae.cho@lge.com)
 *  @version	1.0
 *  @date		2012.02.23
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

#ifndef _SCI_REG_H_
#define _SCI_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#include "sci_kapi.h"

#include "sci_err.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define DISABLE 0
#define ENABLE  1

#define STATUS_READY            0
#define STATUS_DETECTED         1
#define STATUS_ACTIVE           2
#define STATUS_FAILED           3

#define IF_MOD_7816_3_COMPLIANT 0
#define IF_MOD_TDA8004          1

#define CLK_PREDIV_1            0
#define CLK_PREDIV_2            1

//
#define PAR_CRC_PARITY_NOT_INCLUDED     0
#define PAR_CRC_PARITY_INCLUDED         1

#define CRC_DIR_REVERSE_DIRECTION       0
#define CRC_FORWARD_DIRECTION           1

#define VCC_SEL_5V              1
#define VCC_SEL_3V              0

#define POE_EVEN                0
#define POE_ODD                 1

#define BGT_DLY_22_ETU          0
#define BGT_DLY_16_ETU          1

#define PAR_DUR_ETU_1_0         0
#define PAR_DUR_ETU_1_2         1
#define PAR_DUR_ETU_1_4         2
#define PAR_DUR_ETU_1_6         3
#define PAR_DUR_ETU_1_8         4
#define PAR_DUR_ETU_2_0         5

// MODE
#define LBE_BIG_BIT_ENDIAN      0
#define LBE_LITTLE_BIT_ENDIAN   1

#define CONV_REVERSE            0
#define CONV_DIRECT             1

#define EDC_LRC                 0
#define EDC_CRC                 1

#define T_BYTE_MODE             0
#define T_BLOCK_MODE            1

// DLY
#define RST_DUR_40_000_PERIOS   0
#define RST_DUR_43_200_PERIOS   1
#define RST_DUR_46_400_PERIOS   2
#define RST_DUR_49_600_PERIOS   3

#define CLK_DIV_2               0
#define CLK_DIV_4               1
#define CLK_DIV_6               2
#define CLK_DIV_8               3
#define CLK_DIV_12              4
#define CLK_DIV_16              5
#define CLK_DIV_24              6
#define CLK_DIV_32              7


#define MAX_RF_DEPTH	    0x3F
#define MAX_TF_DEPTH	    0x1F

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
	0x1a00 sci_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	data                         	: 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_DATA;

/*-----------------------------------------------------------------------------
	0x1a04 sci_nad ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	nad                             : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_NAD;

/*-----------------------------------------------------------------------------
	0x1a08 sci_pcb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pcb                             : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_PCB;

/*-----------------------------------------------------------------------------
	0x1a0c sci_len ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	len                             : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_LEN;

/*-----------------------------------------------------------------------------
	0x1a10 sci_edc1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	edc1                            : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_EDC1;

/*-----------------------------------------------------------------------------
	0x1a14 sci_edc2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	edc2                            : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_EDC2;

/*-----------------------------------------------------------------------------
	0x1a18 sci_err ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	phy                             : 1,	//     0
	byte                            : 1,	//     1
	wwt                             : 1,	//     2
	cwt                             : 1,	//     3
	bwt                             : 1,	//     4
	block                           : 1,	//     5
	rsvd                            :26;	//  6:31
} SCI_ERR;

/*-----------------------------------------------------------------------------
	0x1a1c sci_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rx_on                           : 1,	//     0
	tx_on                           : 1,	//     1
	ok                              : 1,	//     2
	bp                              : 1,	//     3
	card                            : 2,	//  4: 5
	rsvd                            :26;	//  6:31
} SCI_STAT;

/*-----------------------------------------------------------------------------
	0x1a20 sci_intr_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frf                             : 1,	//     0
	frne                            : 1,	//     1
	ftf                             : 1,	//     2
	fte                             : 1,	//     3
	eor                             : 1,	//     4
	eot                             : 1,	//     5
	err                             : 1,	//     6
	cd_pwr                          : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_INTR_STAT;

/*-----------------------------------------------------------------------------
	0x1a24 sci_cmd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rx_en                           : 1,	//     0
	tx_en                           : 1,	//     1
	rsvd1                           : 1,	//     2
	pwr_on                          : 1,	//     3
	pwr_off                         : 1,	//     4
	reset                           : 1,	//     5
	clr_fr                          : 1,	//     6
	clr_ft                          : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_CMD;

/*-----------------------------------------------------------------------------
	0x1a28 sci_fc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	t_fc_en                         : 1,	//     0
	r_fc_en                         : 1,	//     1
	par_retry                       : 3,	//  2: 4
	rsvd                            :27;	//  5:31
} SCI_FC;

/*-----------------------------------------------------------------------------
	0x1a2c sci_ext ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	if_mod                          : 1,	//     0
	p_mon_dly                       : 1,	//     1
	rsvd                            :30;	//  2:31
} SCI_EXT;

/*-----------------------------------------------------------------------------
	0x1a40 sci_dds ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwr_dly                         : 2,	//  0: 1
	clk_prediv                      : 1,	//     2
	vcc_hi                          : 1,	//     3
	det_hi                          : 1,	//     4
	p_mon_hi                        : 1,	//     5
	reset_low                       : 1,	//     6
	reset_type                      : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_DDS;

/*-----------------------------------------------------------------------------
	0x1a44 sci_set ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	par_dur                         : 3,	//  0: 2
	bgt_dly                         : 1,	//     3
	poe                             : 1,	//     4
	vcc_sel                         : 1,	//     5
	crc_dir                         : 1,	//     6
	par_crc                         : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_SET;

/*-----------------------------------------------------------------------------
	0x1a48 sci_conf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	t_par_en                        : 1,	//     0
	r_par_en                        : 1,	//     1
	ato_en                          : 1,	//     2
	t1_en                           : 1,	//     3
	deac_en                         : 1,	//     4
	ts_en                           : 1,	//     5
	abrt_en                         : 1,	//     6
	edc_en                          : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_CONF;

/*-----------------------------------------------------------------------------
	0x1a4c sci_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	t                               : 1,	//     0
	edc                             : 1,	//     1
	max_rpt                         : 3,	//  2: 4
	conv                            : 1,	//     5
	lbe                             : 1,	//     6
	tx_to_rx                        : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_MODE;

/*-----------------------------------------------------------------------------
	0x1a50 sci_di ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	di                              : 4,	//  0: 3
	rsvd                            :28;	//  4:31
} SCI_DI;

/*-----------------------------------------------------------------------------
	0x1a54 sci_tos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cwi                             : 4,	//  0: 3
	bwi                             : 4,	//  4: 7
	rsvd                            :24;	//  8:31
} SCI_TOS;

/*-----------------------------------------------------------------------------
	0x1a58 sci_br ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	br                              : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_BR;

/*-----------------------------------------------------------------------------
	0x1a5c sci_ft ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ft                              : 4,	//  0: 3
	rsvd                            :28;	//  4:31
} SCI_FT;

/*-----------------------------------------------------------------------------
	0x1a60 sci_intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frf                             : 1,	//     0
	frne                            : 1,	//     1
	ftf                             : 1,	//     2
	fte                             : 1,	//     3
	eor                             : 1,	//     4
	eot                             : 1,	//     5
	err                             : 1,	//     6
	cd_pwr                          : 1,	//     7
	rsvd                            :24;	//  8:31
} SCI_INTR_MASK;

/*-----------------------------------------------------------------------------
	0x1a64 sci_agt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	agt                             : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_AGT;

/*-----------------------------------------------------------------------------
	0x1a68 sci_wi ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	wi                              : 7,	//  0: 6
	rsvd                            :25;	//  7:31
} SCI_WI;

/*-----------------------------------------------------------------------------
	0x1a6c sci_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	clk_div                         : 3,	//  0: 2
	rst_dur                         : 2,	//  3: 4
	rsvd                            :27;	//  5:31
} SCI_DLY;

/*-----------------------------------------------------------------------------
	0x1a70 sci_pol_crc_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pol_crc_l                       : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_POL_CRC_L;

/*-----------------------------------------------------------------------------
	0x1a74 sci_pol_crc_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pol_crc_h                       : 8,	//  0: 7
	rsvd                            :24;	//  8:31
} SCI_POL_CRC_H;

/*-----------------------------------------------------------------------------
	0x1a78 sci_rf_depth ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rf_depth                        : 6,	//  0: 5
	rsvd                            :26;	//  6:31
} SCI_RF_DEPTH;

/*-----------------------------------------------------------------------------
	0x1a7c sci_tf_depth ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tf_depth                        : 5,	//  0: 4
	rsvd                            :27;	//  5:31
} SCI_TF_DEPTH;


typedef struct {
	SCI_DATA                        	sci_data                        ;	// 0x00 : ''
	SCI_NAD                         	sci_nad                         ;	// 0x04 : ''
	SCI_PCB                         	sci_pcb                         ;	// 0x08 : ''
	SCI_LEN                         	sci_len                         ;	// 0x0C : ''
	SCI_EDC1                         	sci_edc1                        ;	// 0x10 : ''
	SCI_EDC2                       		sci_edc2                        ;	// 0x14 : ''
	SCI_ERR                        		sci_err                         ;	// 0x18 : ''
	SCI_STAT                       		sci_stat                        ;	// 0x1C : ''
	SCI_INTR_STAT                  		sci_intr_stat                   ;	// 0x20 : ''
	SCI_CMD                        		sci_cmd                         ;	// 0x24 : ''
	SCI_FC                         		sci_fc                          ;	// 0x28 : ''
	SCI_EXT                        		sci_ext                         ;	// 0x2C : ''
	UINT32                         		rsvd[4]                         ;	// 0x30~3C : ''
	SCI_DDS                        		sci_dds                         ;	// 0x40 : ''
	SCI_SET                        		sci_set                         ;	// 0x44 : ''
	SCI_CONF                       		sci_conf                        ;	// 0x48 : ''
	SCI_MODE                       		sci_mode                        ;	// 0x4C : ''
	SCI_DI                         		sci_di                          ;	// 0x50 : ''
	SCI_TOS                        		sci_tos                         ;	// 0x54 : ''
	SCI_BR                         		sci_br                          ;	// 0x58 : ''
	SCI_FT                         		sci_ft                          ;	// 0x5C : ''
	SCI_INTR_MASK                  		sci_intr_mask                   ;	// 0x60 : ''
	SCI_AGT                        		sci_agt                         ;	// 0x64 : ''
	SCI_WI                         		sci_wi                          ;	// 0x68 : ''
	SCI_DLY                        		sci_dly                         ;	// 0x6C : ''
	SCI_POL_CRC_L                  		sci_pol_crc_l                   ;	// 0x70 : ''
	SCI_POL_CRC_H                  		sci_pol_crc_h                   ;	// 0x74 : ''
	SCI_RF_DEPTH                   		sci_rf_depth                    ;	// 0x78 : ''
	SCI_TF_DEPTH                   		sci_tf_depth                    ;	// 0x7C : ''
} SCI_REG_T;
/* 29 regs, 29 types */


/*
 * @{
 * Naming for register pointer.
 * gpRealRegSCI : real register of SCI.
 * gpRegSCI     : shadow register.
 *
 * @def SCI_RdFL: Read  FLushing : Shadow <- Real.
 * @def SCI_WrFL: Write FLushing : Shadow -> Real.
 * @def SCI_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def SCI_Wr  : Write whole register(UINT32) from Shadow register.
 * @def SCI_Rd01 ~ SCI_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def SCI_Wr01 ~ SCI_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define SCI_RdFL(_r)			((gpRegSCI->_r)=(gpRealRegSCI->_r))
#define SCI_WrFL(_r)			((gpRealRegSCI->_r)=(gpRegSCI->_r))

#define SCI_Rd(_r)			*((volatile UINT32*)(&(gpRegSCI->_r)))
#define SCI_Wr(_r,_v)			((SCI_Rd(_r))=((volatile UINT32)(_v)))

#define SCI_Rd01(_r,_f01,_v01)													\
		    					do { 											\
		    						(_v01) = (gpRegSCI->_r._f01);				\
		    					} while(0)

#define SCI_Rd02(_r,_f01,_v01,_f02,_v02)										\
		    					do { 											\
		    						(_v01) = (gpRegSCI->_r._f01);				\
		    						(_v02) = (gpRegSCI->_r._f02);				\
		    					} while(0)

#define SCI_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegSCI->_r._f01);				\
									(_v02) = (gpRegSCI->_r._f02);				\
									(_v03) = (gpRegSCI->_r._f03);				\
								} while(0)

#define SCI_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegSCI->_r._f01);				\
									(_v02) = (gpRegSCI->_r._f02);				\
									(_v03) = (gpRegSCI->_r._f03);				\
									(_v04) = (gpRegSCI->_r._f04);				\
								} while(0)



#define SCI_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegSCI->_r._f01) = (_v01);				\
								} while(0)

#define SCI_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegSCI->_r._f01) = (_v01);				\
									(gpRegSCI->_r._f02) = (_v02);				\
								} while(0)

#define SCI_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegSCI->_r._f01) = (_v01);				\
									(gpRegSCI->_r._f02) = (_v02);				\
									(gpRegSCI->_r._f03) = (_v03);				\
								} while(0)

#define SCI_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegSCI->_r._f01) = (_v01);				\
									(gpRegSCI->_r._f02) = (_v02);				\
									(gpRegSCI->_r._f03) = (_v03);				\
									(gpRegSCI->_r._f04) = (_v04);				\
								} while(0)
							

/*----------------------------------------------------------------------------------------
	Extern Variable Declaration
----------------------------------------------------------------------------------------*/
extern volatile SCI_REG_T* gpRegSCI;
extern volatile SCI_REG_T* gpRealRegSCI;


/*----------------------------------------------------------------------------------------
	Global Function Declaration
----------------------------------------------------------------------------------------*/
SCI_RETURN_T SCI_RegInit(void);
void SCI_RegCleanup(void);

void SCI_RegDump(void);
void SCI_RegWrite(UINT8 offset, UINT32 value);


void SCI_RegSetInit(void);
void SCI_RegSelVoltage(UINT8 val);
UINT8 SCI_RegGetVoltage(void);
UINT8 SCI_RegGetStatus(void);
UINT8 SCI_RegGetCardStatus(void);
UINT8 SCI_RegGetErrorStatus(void);
void SCI_RegEnableInterrupt(UINT8 mask);
void SCI_RegDisableInterrupt(UINT8 mask);
UINT8 SCI_RegGetInterruptStatus(void);
UINT8 SCI_RegGetInterruptMask(void);
void SCI_RegClearFIFO(void);
void SCI_RegEnableTransmit(void);
void SCI_RegEnableReceive(void);
void SCI_RegWriteData(UINT8 data);
UINT8 SCI_RegReadData(void);
void SCI_RegPowerOn(void);
void SCI_RegPowerOff(void);
void SCI_RegReset(void);
void SCI_RegSetBaud(UINT8 brValue, UINT8 ftValue);
void SCI_RegSetReceiveFIFODepth(UINT8 depth);
void SCI_RegSetTransmitFIFODepth(UINT8 depth);
UINT8 SCI_RegGetNAD(void);
void SCI_RegSetNAD(UINT8 nad);
UINT8 SCI_RegGetPCB(void);
void SCI_RegSetPCB(UINT8 pcb);
UINT8 SCI_RegGetLength(void);
void SCI_RegSetLength(UINT8 length);
void SCI_RegSendNullCmd(void);
void SCI_RegSetFlowControl(UINT8 parityRetry, UINT8 receiveEnable, UINT8 transmitEnable);
void SCI_RegSetTimeOut(UINT8 bwiValue, UINT8 cwiValue);
void SCI_RegSetDI(UINT8 diValue);
void SCI_RegSetAddionalGuardTime(UINT8 guardTime);
void SCI_RegSetWorkWaitTime(UINT8 waitTime);
UINT8 SCI_RegGetClkDiv(void);
void SCI_RegSetDelay(UINT8 resetDuration, UINT8 clkDiv);
void SCI_RegSetDelayPwrMonSignal(UINT8 delay);
void SCI_RegSetExtSmartCardChipInterface(UINT8 flag);
void SCI_RegSetCardResetActiveLow(UINT8 flag);
void SCI_RegSetPwrMonHigh(UINT8 flag);
void SCI_RegEnableModeAutoRXChange(void);
void SCI_RegDisableModeAutoRXChange(void);
void SCI_RegSetModeBigEndian(void);
void SCI_RegSetModeLittleEndian(void);
void SCI_RegSetModeConversion(UINT8 convValue);
void SCI_RegSetMaxRepetition(UINT8 value);
void SCI_RegSetModeDataCheckLRC(void);
void SCI_RegSetModeDataCheckCRC(void);
void SCI_RegSetModeByteORBlock(UINT8 flag);
void SCI_RegSetCRCDirection(UINT8 direction);
void SCI_RegSetParityEven(void);
void SCI_RegSetParityOdd(void);
void SCI_RegSetBGTDelay22etu(void);
void SCI_RegSetBGTDelay16etu(void);
void SCI_RegSetParityDuration(UINT8 flag);




#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SCI_REG_H_  */

/** @} */

