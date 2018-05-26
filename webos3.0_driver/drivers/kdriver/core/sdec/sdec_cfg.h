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
 *  main configuration file for sdec device
 *  sdec device will teach you how to make device driver with new platform.
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


#ifndef	_SDEC_CFG_H_
#define	_SDEC_CFG_H_

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
#define	SDEC_MODULE			"sdec"
#define SDEC_MAX_DEVICE		1
#define SDEC_MAX_CORE		2



/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
// for LG1154
#define H13_SDEC_TOP_REG_BASE				(H13_TE_BASE)
#define H13_SDEC_IO_REG_BASE				(H13_TE_BASE + 0x200)
#define H13_SDEC_MPG_REG_BASE0				(H13_TE_BASE + 0x400)
#define H13_SDEC_MPG_REG_BASE1				(H13_TE_BASE + 0xC00)

// for LG1311 M14 A0
#define M14_A0_SDEC_TOP_REG_BASE			(M14_TE_BASE)
#define M14_A0_SDEC_IO_REG_BASE				(M14_TE_BASE + 0x200)
#define M14_A0_SDEC_MPG_REG_BASE0			(M14_TE_BASE + 0x400)
#define M14_A0_SDEC_MPG_REG_BASE1			(M14_TE_BASE + 0xC00)

// for LG1311 M14 B0
#define M14_B0_SDEC_TOP_REG_BASE			(M14_B0_TE_BASE)
#define M14_B0_SDEC_CORE0_IO_REG_BASE		(M14_B0_TE_BASE + 0x200)
#define M14_B0_SDEC_CORE1_IO_REG_BASE		(M14_B0_TE_BASE + 0x1A00)
#define M14_B0_SDEC_CORE0_MPG_REG_BASE0		(M14_B0_TE_BASE + 0x400)
#define M14_B0_SDEC_CORE0_MPG_REG_BASE1		(M14_B0_TE_BASE + 0xC00)
#define M14_B0_SDEC_CORE1_MPG_REG_BASE0		(M14_B0_TE_BASE + 0x1C00)
#define M14_B0_SDEC_CORE1_MPG_REG_BASE1		(M14_B0_TE_BASE + 0x2400)

// for LG1156 H14
#define H14_SDEC_TOP_REG_BASE				(H14_TE_BASE)	/* 0xC0006000 */
#define H14_SDEC_CORE0_IO_REG_BASE			(H14_TE_BASE + 0x200)
#define H14_SDEC_CORE1_IO_REG_BASE			(H14_TE_BASE + 0x1A00)
#define H14_SDEC_CORE0_MPG_REG_BASE0		(H14_TE_BASE + 0x400)
#define H14_SDEC_CORE0_MPG_REG_BASE1		(H14_TE_BASE + 0xC00)
#define H14_SDEC_CORE1_MPG_REG_BASE0		(H14_TE_BASE + 0x1C00)
#define H14_SDEC_CORE1_MPG_REG_BASE1		(H14_TE_BASE + 0x2400)

// for LG1210 H15
#define H15_SDEC_TOP_REG_BASE				(H15_TE_BASE + 0x0000)	/* 0xC8830000 */
#define H15_SDEC_CORE0_IO_REG_BASE			(H15_TE_BASE + 0x0200)
#define H15_SDEC_CORE1_IO_REG_BASE			(H15_TE_BASE + 0x1A00)
#define H15_SDEC_CORE0_MPG_REG_BASE0		(H15_TE_BASE + 0x0400)
#define H15_SDEC_CORE0_MPG_REG_BASE1		(H15_TE_BASE + 0x0C00)
#define H15_SDEC_CORE1_MPG_REG_BASE0		(H15_TE_BASE + 0x1C00)
#define H15_SDEC_CORE1_MPG_REG_BASE1		(H15_TE_BASE + 0x2400)


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT8		capa_lev;		///< capacity level of channel ( full : 1 / simple : 0 ). simple channel has only ts2pes parser for channle browser.
	UINT8		num_pidf;		///< number of pid filter in this channel
	UINT8		num_secf;		///< number of section filter in this channel
	UINT8		flt_dept;		///< filter depth
} LX_SDEC_CHA_INFO_T;


typedef struct
{
	UINT8			nCore;					///< number of cores
	UINT8			nChannel;				///< number of channels
	UINT8			nVdecOutPort;			///< number of vdec output port.
	UINT8			noPesBug;				///< pes h/w bug fixed?
	UINT8			staticGPB;				///< GPB is static?
	UINT8			inTsSelector;			///< has internal TS source selector
	UINT8			instantBoot;
	UINT8			newCDIP;				///< new CDIP? (M14Cx)
	UINT8			ciPlus;					///< support ci plus 1.4
	UINT8			noFixedClock;			///< not support fixed clock (H13)

	int 			irqNum[SDEC_MAX_CORE];
	UINT32			sdtopRegBase;
	UINT32			sdioRegBase[SDEC_MAX_CORE];
	UINT32			mpgRegBase[SDEC_MAX_CORE][2];

	const LX_SDEC_CHA_INFO_T	*chInfo;	///< channel information

} LX_SDEC_CFG_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void SDEC_CFG_PreInit(void);
LX_SDEC_CFG_T*	SDEC_CFG_GetConfig(void);
LX_MEMCFG_T*	SDEC_CFG_GetMemConfig(void);
LX_MEMCFG_T*	SDEC_CFG_GetCiPlusMemConfig(void);

int SDEC_CFG_IsValidChannel(UINT8 lxch);
int SDEC_CFG_IsNormalChannel(UINT8 lxch);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern LX_MEMCFG_T		gMemCfgSDEC[2];


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_CFG_H_ */

/** @} */

